#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "file_pac.h"
#include "sdf_movie_stream.h"
#include "sdf_movie_state.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"

enum {
    PAC_HEADER_BYTES = 0x10,
    PAC_EXTENSION_BYTES_MASK = 0xF0,
    PAC_ALIGNMENT_BYTES = 0x40,
    PAC_ALIGNMENT_MASK = 0x3F,
    PAC_CALLBACK_FINISHED = 1,
    PAC_CALLBACK_ALIGN = 2,
    PAC_CALLBACK_DROP_TAIL = 4,
    PAC_CALLBACK_EVENT_PACKET_HEADER = 0,
    PAC_CALLBACK_EVENT_PACKET_COMPLETE = 1,
    PAC_PHASE_FINISHED = -1,
    PAC_INITIAL_PACKET_COUNTER = -1,
    PAC_STATE_USE_PACKET_MEMORY = 1,
    PAC_STATE_ALLOCATE_HIGH = 2
};

typedef struct PacAlloc {
    s32 entryCount;
    s32 entryIndex;
    u8 pad8[24];
    s32 resource;
} PacAlloc;

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void *callback, void *callbackContext);
extern s32 movieStreamSema;
extern s32 movieSemaCount;
extern u8 D_00438D28[];


/* Initialize an inline sound/IPU node and the selected linear or movie-PAC stream. */
void movieOpenStream(movieStream_t *owner, SdfMovieDescriptor *descriptor, const char *name) {
    SoundFormat soundFormat;
    struct sdfMemHandle_t *allocation;
    u8 *resource;
    MovLinearStream *stream;
    MovPacStream *pacWork;
    char *extension;
    s32 isPac;

    memset(owner, 0, 0xB0);
    owner->active = 1;
    extension = strrchr(name, '.');
    isPac = 0;
    if (extension != NULL) {
        isPac = strcasecmp(extension, (const char *)&D_00438D28) == 0;
    }
    owner->isPac = isPac;
    owner->remainingBytes = 0x7FFFFFFF;
    owner->drawX = descriptor->drawX;
    owner->drawZ = descriptor->drawZ;
    owner->drawY = descriptor->drawY;
    owner->drawWidth = descriptor->drawWidth;
    owner->drawHeight = descriptor->drawHeight;
    soundFormat.hasAudio = descriptor->hasAudio;
    soundFormat.stereo = descriptor->stereo;
    soundFormat.loopMode = 0;
    soundFormat.playbackCadenceStep = descriptor->playbackCadenceStep;

    if (!isPac) {
        stream = sdfAllocAndClearQuadwords(sizeof(*stream));
        owner->stream.linear = stream;
        allocation = sdfMemAllocDirect(0x20000);
        stream->allocation = allocation;
        {
            u8 *buffer = (u8 *)sdfMemLock(allocation);
            stream->bufferStart = buffer;
            stream->readCursor = buffer;
            stream->writeCursor = buffer;
        }
        owner->state = SDF_MOVIE_STATE_INITIAL;
        owner->deviceState = sdfFileCmdReadOpen(name,
                                                        (void *)movieGetStreamSize, owner);
        sdfSoundInitFormattedAndAppendNode(&owner->soundNode, &soundFormat,
                                            movieCopyChunk, owner, descriptor->source);
        return;
    }

    if (movieStreamSema < 0) {
        movieStreamSema = sdfCreateSema(1, 1, 0);
    }
    movieSemaCount = -1;
    pacWork = sdfAllocAndClearQuadwords(sizeof(*pacWork));
    owner->stream.pac = pacWork;
    allocation = sdfMemAllocDirect(0x24000);
    pacWork->allocation = allocation;
    resource = (u8 *)sdfMemLock(allocation);
    pacWork->pacInputBuffer = resource;
    pacWork->pacBuffer = resource + 0x4000;
    pacWork->ringBuffer = resource + 0x14000;
    pacWork->scratchSize = 0x20;
    pacWork->scratch = pacWork->scratchBuffer;
    owner->state = SDF_MOVIE_STATE_INITIAL;
    owner->deviceState = sdfFileCmdReadOpen(name,
                                                    (void *)movieReadChunk, owner);
    owner->pacEnabled = 1;
    owner->packetLimit = 0x7F;
    sdfSoundInitFormattedAndAppendNode(&owner->soundNode, &soundFormat,
                                        movieParseHeader, owner, descriptor->source);
}

/* This no-op also serves as a void(u32) completion callback and ignores its word. */
extern void sdfCreateSemaphoreFromOptions();
extern void sdfIpuFree(sdfIpuDecodeCb_t *);
extern void p2sdSyncMovie(void);

/* Wait for active work's release phase, free its owned buffers, then deactivate it. */
void movieCloseStream(movieStream_t *job) {
    if (job->active != 0) {
        job->stopRequested = 1;
        /* Queue phase 5 as phase 7; phase 6 is the release gate below. */
        if (job->state == SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE) {
            job->state = SDF_MOVIE_STATE_STOP_REQUESTED;
            sdfFileCmdClose(job->deviceState);
        }
        while (job->state != SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK) {
            sdfCreateSemaphoreFromOptions();
        }
        if (job->isPac == 0) {
            MovLinearStream *stream = job->stream.linear;
            sdfMemFreeDirect(stream->allocation);
            sdfChipFreeDirect(stream);
        } else {
            MovPacStream *stream = job->stream.pac;
            sdfMemFreeDirect(stream->payloadAllocation);
            sdfMemFreeDirect(stream->allocation);
            sdfChipFreeDirect(stream);
        }
        sdfIpuFree(&job->soundNode);
        if (job->pacEnabled != 0) {
            p2sdSyncMovie();
        }
        job->active = 0;
    }
}

/* Decoder completion requires drained input and no completed output buffers pending. */
s32 sdfMovieIsFinished(movieStream_t *job) {
    if (job->soundNode.drained == 0) {
        return 0;
    }
    return job->soundNode.completedBufferCount == 0;
}

/* Start the packet counter at -1 and select a custom or built-in packet callback. */
void sdfPacInit(PacState *packet, void *callbackAddress) {
    memset(packet, 0, sizeof(*packet));
    packet->packetCounter = PAC_INITIAL_PACKET_COUNTER;
    if (callbackAddress != NULL) {
        packet->packetCallback = (s32 (*)())callbackAddress;
    } else {
        packet->packetCallback = sdfPacStdCallback;
    }
}

/* Retain packet-owned payload memory rather than allocating a separate resource. */
void sdfPacSetStay(PacState *state) {
    state->flags = state->flags | PAC_STATE_USE_PACKET_MEMORY;
}

/* Select the high-address allocator for packet payload resources. */
void sdfPacSetFromTail(PacState *state) {
    state->flags = state->flags | PAC_STATE_ALLOCATE_HIGH;
}

/* Advance the queue cursor before returning each node to the chip allocator. */
void sdfPacFree(PacState *state) {
    PacWork *cursor = state->queueHead;
    PacWork *current = cursor;

    while (cursor != NULL) {
        cursor = cursor->next;
        sdfChipFreeDirect(current);
        current = cursor;
    }
}

/* Mark the signed PAC state as finished. */
void sdfPacMarkPhaseFinished(s8 *phase) {
    *phase = PAC_PHASE_FINISHED;
}

/* Consume byteCount bytes without bounds checks, updating the cursor and both counters. */
void sdfPacUseSrc(PacState *state, s32 byteCount) {
    state->inputCursor = state->inputCursor + byteCount;
    state->inputAvailable = state->inputAvailable - byteCount;
    state->consumedBytes = state->consumedBytes + byteCount;
}

/* Align the stream, then report a packet header and its payload byte count to the callback. */
void sdfPacAdvanceCallbackBoundary(PacState *state) {
    SdfPacStreamPacketHeader *inputHeader;
    u32 alignmentOffset;
    u32 packetBytes;
    u32 headerBytes;
    u32 payloadBytes;
    s32 (*packetCallback)();
    s32 callbackResult;

    alignmentOffset = state->consumedBytes & PAC_ALIGNMENT_MASK;
    if (alignmentOffset != 0) {
        state->phase = 1;
        state->pendingBytes = PAC_ALIGNMENT_BYTES - alignmentOffset;
        return;
    }
    inputHeader = (SdfPacStreamPacketHeader *)state->inputCursor;
    /* Advance only the fixed header; retain its original address for the callback. */
    sdfPacUseSrc(state, PAC_HEADER_BYTES);
    packetBytes = inputHeader->packetByteCount;
    state->packetCounter++;
    /* The high nibble encodes extension bytes, not a bit offset. */
    headerBytes = (inputHeader->flags & PAC_EXTENSION_BYTES_MASK) + PAC_HEADER_BYTES;
    payloadBytes = packetBytes - headerBytes;
    state->pendingBytes = payloadBytes;
    packetCallback = state->packetCallback;
    callbackResult = packetCallback(state, PAC_CALLBACK_EVENT_PACKET_HEADER, inputHeader, payloadBytes);
    if (callbackResult == PAC_CALLBACK_FINISHED) {
        sdfPacMarkPhaseFinished(&state->phase);
        return;
    }
    if (callbackResult == PAC_CALLBACK_ALIGN) {
        state->phase = 1;
        state->pendingBytes = (state->pendingBytes + PAC_ALIGNMENT_MASK) & ~PAC_ALIGNMENT_MASK;
    } else {
        state->phase = 2;
    }
}



/* Notify completion with the tail's header: result 1 finishes; result 4 releases the tail. */
void sdfDecodePacNodeAndAdvanceTail(PacState *state) {
    s32 callbackResult;
    PacWork *tailPacket;
    PacWork *scanPacket;
    PacWork *nextPacket;

    callbackResult = state->packetCallback(state, PAC_CALLBACK_EVENT_PACKET_COMPLETE, state->queueTail->packet);
    if (callbackResult == PAC_CALLBACK_FINISHED) {
        sdfPacMarkPhaseFinished(&state->phase);
        return;
    }
    if (callbackResult == PAC_CALLBACK_DROP_TAIL) {
        tailPacket = state->queueTail;
        scanPacket = state->queueHead;
        if (scanPacket == tailPacket) {
            state->queueTail = NULL;
            state->queueHead = NULL;
        } else {
            while ((nextPacket = scanPacket->next) != tailPacket) {
                scanPacket = nextPacket;
            }
            /* The multi-item path clears the link without reassigning queueTail here. */
            scanPacket->next = NULL;
        }
        sdfChipFreeDirect(tailPacket);
    }
    state->phase = 0;
}

INCLUDE_SDATA(const s32, "game/code_00346778", D_00438D28);
