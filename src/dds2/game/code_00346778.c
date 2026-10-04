#include "common.h"

typedef struct DevState DevState;
typedef struct MemBlock MemBlock;
typedef struct SdfStreamTextureHead SdfStreamTextureHead;

enum {
    PAC_HEADER_BYTES = 0x10,
    PAC_EXTENSION_BYTES_MASK = 0xF0,
    PAC_ALIGNMENT_BYTES = 0x40,
    PAC_ALIGNMENT_MASK = 0x3F,
    PAC_CALLBACK_FINISHED = 1,
    PAC_CALLBACK_ALIGN = 2,
    PAC_CALLBACK_DROP_TAIL = 4,
    PAC_PHASE_FINISHED = 0xFF,
    PAC_INITIAL_PACKET_COUNTER = -1,
    PAC_STATE_USE_PACKET_MEMORY = 1,
    PAC_STATE_ALLOCATE_HIGH = 2
};

/* Native decoder records shared with sdfPacDecode. */
typedef struct PacHead {
    u8 command;
    u8 flags;
    u8 pad2[2];
    s32 payloadSize;
    u8 pad8[4];
    s32 decodedSize;
    u8 payload[1];
} PacHead;

typedef struct PacWork {
    struct PacWork *next;
    struct PacState *owner;
    s32 resourceHandle;
    u8 *dataCursor;
    u8 packet[1];
} PacWork;

typedef struct PacAlloc {
    s32 entryCount;
    s32 entryIndex;
    u8 pad8[24];
    s32 resource;
} PacAlloc;

typedef struct PacBuf {
    s32 result;
    s32 resourceSlot;
    u8 *cursor;
    s32 remainingBytes;
} PacBuf;

/* Event 0 supplies a payload-size word; event 1 omits it. Keep the
 * packet callback's native short-arity interface unprototyped. */
typedef struct PacState {
    u8 phase;
    u8 flags;
    u16 packetCounter;
    s32 (*packetCallback)();
    void (*onInput)(struct PacState *);
    void (*onComplete)(struct PacState *);
    u8 *inputCursor;
    s32 inputAvailable;
    s32 consumedBytes;
    u8 *outputCursor;
    s32 pendingBytes;
    PacBuf *decoder;
    PacBuf *resourceBuffer;
    PacAlloc *allocation;
    PacWork *queueHead;
    PacWork *queueTail;
} PacState;
/* The built-in packet callback is referenced as an address in this unit. */
extern u8 sdfPacDispatchPacket[];

typedef struct SdfMovieDescriptor {
    u16 unk00;
    u16 unk02;
    u32 unk04;
    u16 unk08;
    u16 unk0A;
    s32 source;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 pad13;
} SdfMovieDescriptor;

/* The two stream variants have distinct native allocations: 0x14 and 0x78. */
typedef struct MovLinearStream {
    MemBlock *allocation;
    u8 *bufferStart;
    u8 *readCursor;
    u8 *writeCursor;
    s32 bufferedBytes;
} MovLinearStream;

/* The first 0x40 bytes receive the movie-PAC file header. This is not
 * the generic PAC decoder state used below. */
typedef struct MovPacStream {
    u8 pad00[0x18];
    s32 packetBytes;
    s32 blockBytes;
    u8 pad20[0x20];
    MemBlock *payloadAllocation;
    u8 *blockMask;
    s32 blockIndex;
    MemBlock *allocation;
    void *pendingCursor;
    u8 *pacBuffer;
    s32 pacReadOffset;
    s32 pacBufferedBytes;
    u8 *ringBuffer;
    s32 ringOffset;
    s32 ringLength;
    s32 unk6C;
    s32 scratchSize;
    u8 *scratch;
} MovPacStream;

/* Native 0x8C sound/IPU stream node, owned inline by the movie object. */
typedef struct SdfStreamFrameNode {
    u8 pad00[8];
    struct SdfStreamFrameNode *next;
    u8 active;
    u8 pad0D[2];
    u8 drained;
    u8 pad10[4];
    u8 audioMode;
    u8 loopMode;
    u8 playbackMode;
    u8 pad17[3];
    u8 unk1A;
    u8 pad1B;
    s32 bufferSize;
    u32 buffers[2];
    s32 textureResources[2];
    u8 pad30[4];
    s32 resourceWord;
    SdfStreamTextureHead *textureHead;
    u16 width;
    u16 height;
    s32 sourceBytes;
    u8 pad44[8];
    u32 unk4C;
    u8 headerReady;
    u8 done;
    u8 filledSlots;
    u8 firstSlot;
    u32 scratchBuffer;
    u8 pad58[4];
    s32 (*read)(struct SdfStreamFrameNode *, u32, s32, void *, s32);
    u32 source;
    u8 pad64[0x28];
} SdfStreamFrameNode;

typedef struct MovObj {
    u8 active;
    u8 state;
    u8 stopRequested;
    u8 isPac;
    u16 unk04;
    u16 unk06;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    DevState *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    void *stream; /* MovLinearStream or MovPacStream, selected by isPac. */
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    SdfStreamFrameNode soundNode;
} MovObj;

extern void *sdfAllocAndClearQuadwords(s32 size);
extern void *sdfAllocGeneralBlock(s32 size);
extern s32 sdfResourceRetainAddress(void *block);
extern void *sdfDevCreateCallbackState(s32 path, void *callback, s32 context);
extern s32 sdfCreateSemaphore(s32 initialCount, s32 maximumCount, s32 options);
extern s32 func_0035D5B0(const char *text, s32 delimiter);
extern s32 func_0035CB10(const char *text, const char *suffix);
extern void sdfSoundInitFormattedAndAppendNode(void *node, u8 *format,
                                                void *callback, void *owner, s32 source);
extern s32 func_00345EB0(void *, s32, s32, s32, s32);
extern s32 func_003460D8(void *, s32, s32, s32, s32);
extern s32 func_00346468(void *, void *, s32, u8 *, s32);
extern s32 func_00346608(void *, void *, s32, u8 *, s32);
extern s32 D_00438D08;
extern s32 D_0043921C;
extern u8 D_00438D28[];

/* Initialize an inline sound/IPU node and the selected linear or movie-PAC stream. */
void func_00346778(MovObj *owner, SdfMovieDescriptor *descriptor, const char *name) {
    u8 soundFormat[4];
    void *work;
    void *allocation;
    u8 *resource;
    MovLinearStream *stream;
    MovPacStream *pacWork;
    char *extension;
    s32 isPac;

    memset(owner, 0, 0xB0);
    owner->active = 1;
    extension = (char *)func_0035D5B0(name, '.');
    isPac = 0;
    if (extension != NULL) {
        isPac = func_0035CB10(extension, (const char *)&D_00438D28) == 0;
    }
    owner->isPac = isPac;
    owner->remainingBytes = 0x7FFFFFFF;
    owner->unk04 = descriptor->unk00;
    owner->unk08 = descriptor->unk04;
    owner->unk06 = descriptor->unk02;
    owner->unk0C = descriptor->unk08;
    owner->unk0E = descriptor->unk0A;
    soundFormat[0] = descriptor->unk10;
    soundFormat[1] = descriptor->unk11;
    soundFormat[2] = 0;
    soundFormat[3] = descriptor->unk12;

    if (!isPac) {
        work = sdfAllocAndClearQuadwords(0x14);
        owner->stream = work;
        stream = work;
        allocation = sdfAllocGeneralBlock(0x20000);
        stream->allocation = allocation;
        {
            u8 *buffer = (u8 *)sdfResourceRetainAddress(allocation);
            stream->bufferStart = buffer;
            stream->readCursor = buffer;
            stream->writeCursor = buffer;
        }
        owner->state = 0;
        owner->deviceState = sdfDevCreateCallbackState((s32)name,
                                                        (void *)func_00345EB0, (s32)owner);
        sdfSoundInitFormattedAndAppendNode(&owner->soundNode, soundFormat,
                                            (void *)func_00346468, owner, descriptor->source);
        return;
    }

    if (D_00438D08 < 0) {
        D_00438D08 = sdfCreateSemaphore(1, 1, 0);
    }
    D_0043921C = -1;
    work = sdfAllocAndClearQuadwords(0x78);
    owner->stream = work;
    pacWork = work;
    allocation = sdfAllocGeneralBlock(0x24000);
    pacWork->allocation = allocation;
    resource = (u8 *)sdfResourceRetainAddress(allocation);
    pacWork->pendingCursor = resource;
    pacWork->pacBuffer = resource + 0x4000;
    pacWork->ringBuffer = (u8 *)((u32)resource + 0x14000);
    pacWork->scratchSize = 0x20;
    pacWork->scratch = (u8 *)work + 0x20;
    owner->state = 0;
    owner->deviceState = sdfDevCreateCallbackState((s32)name,
                                                    (void *)func_003460D8, (s32)owner);
    owner->pacEnabled = 1;
    owner->packetLimit = 0x7F;
    sdfSoundInitFormattedAndAppendNode(&owner->soundNode, soundFormat,
                                        (void *)func_00346608, owner, descriptor->source);
}

extern s32 sdfDevQueueActiveOperation(void *);
extern void sdfCreateSemaphoreFromOptions(void);
extern void sdfReleaseResourceAllocation(void *);
extern void sdfReleaseChipBlock(void *);
extern void func_00344A08(void *);
extern void func_00342798(void);

/* Wait for active work's release phase, free its owned buffers, then deactivate it. */
void sdfCancelAndReleasePacWork(MovObj *job) {
    void *ownedBuffers;

    if (job->active != 0) {
        job->stopRequested = 1;
        /* Queue phase 5 as phase 7; phase 6 is the release gate below. */
        if (job->state == 5) {
            job->state = 7;
            sdfDevQueueActiveOperation(job->deviceState);
        }
        while (job->state != 6) {
            sdfCreateSemaphoreFromOptions();
        }
        ownedBuffers = job->stream;
        if (job->isPac == 0) {
            sdfReleaseResourceAllocation(((MovLinearStream *)ownedBuffers)->allocation);
            sdfReleaseChipBlock(ownedBuffers);
        } else {
            sdfReleaseResourceAllocation(((MovPacStream *)ownedBuffers)->payloadAllocation);
            sdfReleaseResourceAllocation(((MovPacStream *)ownedBuffers)->allocation);
            sdfReleaseChipBlock(ownedBuffers);
        }
        func_00344A08(&job->soundNode);
        if (job->pacEnabled != 0) {
            func_00342798();
        }
        job->active = 0;
    }
}

/* Preserve the drained flag and unresolved byte-1A release predicate. */
s32 sdfPacCheckDecoderStatus(MovObj *job) {
    if (job->soundNode.drained == 0) {
        return 0;
    }
    return job->soundNode.unk1A == 0;
}

/* Start the packet counter at -1 and select a custom or built-in packet callback. */
void sdfPacInitializeDispatchPacket(PacState *packet, void *callbackAddress) {
    memset(packet, 0, sizeof(*packet));
    packet->packetCounter = PAC_INITIAL_PACKET_COUNTER;
    if (callbackAddress != NULL) {
        packet->packetCallback = (s32 (*)())callbackAddress;
    } else {
        packet->packetCallback = (s32 (*)())sdfPacDispatchPacket;
    }
}

/* Retain packet-owned payload memory rather than allocating a separate resource. */
void func_00346AD8(PacState *state) {
    state->flags = state->flags | PAC_STATE_USE_PACKET_MEMORY;
}

/* Select the high-address allocator for packet payload resources. */
void func_00346AE8(PacState *state) {
    state->flags = state->flags | PAC_STATE_ALLOCATE_HIGH;
}

/* Advance the queue cursor before returning each node to the chip allocator. */
void func_00346AF8(PacState *state) {
    PacWork *cursor = state->queueHead;
    PacWork *current = cursor;

    while (cursor != NULL) {
        cursor = cursor->next;
        sdfReleaseChipBlock(current);
        current = cursor;
    }
}

/* Mark the caller's phase byte finished. */
void func_00346B30(u8 *phaseByte) {
    *phaseByte = PAC_PHASE_FINISHED;
}

/* Consume byteCount bytes without bounds checks, updating the cursor and both counters. */
void sdfPacAdvanceInput(PacState *state, s32 byteCount) {
    state->inputCursor = state->inputCursor + byteCount;
    state->inputAvailable = state->inputAvailable - byteCount;
    state->consumedBytes = state->consumedBytes + byteCount;
}

/* Align the stream, then report a packet header and its payload byte count to the callback. */
void sdfPacAdvanceCallbackBoundary(PacState *state) {
    PacHead *inputHeader;
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
    inputHeader = (PacHead *)state->inputCursor;
    /* Advance only the fixed header; retain its original address for the callback. */
    sdfPacAdvanceInput(state, PAC_HEADER_BYTES);
    packetBytes = inputHeader->payloadSize;
    state->packetCounter++;
    /* The high nibble encodes extension bytes, not a bit offset. */
    headerBytes = (inputHeader->flags & PAC_EXTENSION_BYTES_MASK) + PAC_HEADER_BYTES;
    payloadBytes = packetBytes - headerBytes;
    state->pendingBytes = payloadBytes;
    packetCallback = state->packetCallback;
    callbackResult = packetCallback(state, 0, inputHeader, payloadBytes);
    if (callbackResult == PAC_CALLBACK_FINISHED) {
        func_00346B30(&state->phase);
        return;
    }
    if (callbackResult == PAC_CALLBACK_ALIGN) {
        state->phase = 1;
        state->pendingBytes = (state->pendingBytes + PAC_ALIGNMENT_MASK) & ~PAC_ALIGNMENT_MASK;
    } else {
        state->phase = 2;
    }
}


extern void sdfReleaseChipBlock(void *);

/* Notify completion with the tail's header: result 1 finishes; result 4 releases the tail. */
void sdfDecodePacNodeAndAdvanceTail(PacState *state) {
    s32 callbackResult;
    PacWork *tailPacket;
    PacWork *scanPacket;
    PacWork *nextPacket;

    callbackResult = state->packetCallback(state, 1, state->queueTail->packet);
    if (callbackResult == PAC_CALLBACK_FINISHED) {
        func_00346B30(&state->phase);
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
        sdfReleaseChipBlock(tailPacket);
    }
    state->phase = 0;
}

INCLUDE_SDATA(const s32, "game/code_00346778", D_00438D28);

