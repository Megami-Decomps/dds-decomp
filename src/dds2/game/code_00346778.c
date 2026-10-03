#include "common.h"

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

typedef struct SdfPacFlags {
    u8 pad00;
    u8 flags;
} SdfPacFlags;

typedef struct SdfPacInput {
    u8 pad00[0x10];
    s32 inputCursor;
    s32 inputAvailable;
    s32 consumedBytes;
} SdfPacInput;

typedef struct PacOwnedBuffers {
    void *primary;
    u8 pad04[0x3C];
    void *secondary;
    u8 pad44[8];
    void *tertiary;
} PacOwnedBuffers;

typedef struct PacTransferState {
    u8 pad00[0xF];
    u8 status0F;
    u8 pad10[0xA];
    u8 status1A;
    u8 pad1B[0x3D];
} PacTransferState;

typedef struct SdfPacWork {
    u8 active;
    u8 phase;
    u8 unk02;
    u8 bufferKind;
    u8 pad04[0xC];
    void *operation;
    u8 pad14[8];
    PacOwnedBuffers *buffers;
    u8 releaseSharedState;
    u8 pad21[3];
    PacTransferState decoder;
} SdfPacWork;

typedef struct SdfPacDispatchPacket {
    u16 unk00;
    s16 packetCounter;
    void *packetCallback;
    u8 pad08[0x30];
} SdfPacDispatchPacket;

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

typedef struct SdfMovieStream {
    void *allocation;
    u8 *bufferStart;
    u8 *readCursor;
    u8 *writeCursor;
    u8 pad10[4];
} SdfMovieStream;

typedef struct SdfMoviePacWork {
    u8 pad00[0x4C];
    void *allocation;
    u8 *resource;
    u8 *ringStart;
    u8 pad58[8];
    u8 *blockStart;
    u8 pad64[0xC];
    s32 scratchSize;
    u8 *scratch;
} SdfMoviePacWork;

typedef struct SdfMovieOwner {
    u8 active;
    u8 state;
    u8 stopRequested;
    u8 isPac;
    u16 unk04;
    u16 unk06;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    void *deviceState;
    u8 pad14[4];
    s32 remainingBytes;
    void *stream;
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    u8 soundNode[0x8C];
} SdfMovieOwner;

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

void func_00346778(SdfMovieOwner *owner, SdfMovieDescriptor *descriptor, const char *name) {
    u8 soundFormat[4];
    void *work;
    void *allocation;
    u8 *resource;
    SdfMovieStream *stream;
    SdfMoviePacWork *pacWork;
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
        sdfSoundInitFormattedAndAppendNode(owner->soundNode, soundFormat,
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
    pacWork->resource = resource;
    pacWork->ringStart = resource + 0x4000;
    pacWork->blockStart = (u8 *)((u32)resource + 0x14000);
    pacWork->scratchSize = 0x20;
    pacWork->scratch = (u8 *)work + 0x20;
    owner->state = 0;
    owner->deviceState = sdfDevCreateCallbackState((s32)name,
                                                    (void *)func_003460D8, (s32)owner);
    owner->pacEnabled = 1;
    owner->packetLimit = 0x7F;
    sdfSoundInitFormattedAndAppendNode(owner->soundNode, soundFormat,
                                        (void *)func_00346608, owner, descriptor->source);
}

extern s32 sdfDevQueueActiveOperation(void *);
extern void sdfCreateSemaphoreFromOptions(void);
extern void sdfReleaseResourceAllocation(void *);
extern void sdfReleaseChipBlock(void *);
extern void func_00344A08(void *);
extern void func_00342798(void);

/* Wait for active work's release phase, free its owned buffers, then deactivate it. */
void sdfCancelAndReleasePacWork(SdfPacWork *job) {
    PacOwnedBuffers *ownedBuffers;

    if (job->active != 0) {
        job->unk02 = 1;
        /* Queue phase 5 as phase 7; phase 6 is the release gate below. */
        if (job->phase == 5) {
            job->phase = 7;
            sdfDevQueueActiveOperation(job->operation);
        }
        while (job->phase != 6) {
            sdfCreateSemaphoreFromOptions();
        }
        ownedBuffers = job->buffers;
        if (job->bufferKind == 0) {
            sdfReleaseResourceAllocation(ownedBuffers->primary);
            sdfReleaseChipBlock(ownedBuffers);
        } else {
            sdfReleaseResourceAllocation(ownedBuffers->secondary);
            sdfReleaseResourceAllocation(ownedBuffers->tertiary);
            sdfReleaseChipBlock(ownedBuffers);
        }
        func_00344A08(&job->decoder);
        if (job->releaseSharedState != 0) {
            func_00342798();
        }
        job->active = 0;
    }
}

/* Preserve the decoder's two-byte status predicate; the byte meanings are unresolved. */
s32 sdfPacCheckDecoderStatus(SdfPacWork *job) {
    if (job->decoder.status0F == 0) {
        return 0;
    }
    return job->decoder.status1A == 0;
}

/* Start the packet counter at -1 and select a custom or built-in packet callback. */
void sdfPacInitializeDispatchPacket(SdfPacDispatchPacket *packet, void *callbackAddress) {
    memset(packet, 0, sizeof(*packet));
    packet->packetCounter = PAC_INITIAL_PACKET_COUNTER;
    if (callbackAddress != NULL) {
        packet->packetCallback = callbackAddress;
    } else {
        packet->packetCallback = sdfPacDispatchPacket;
    }
}

/* Retain packet-owned payload memory rather than allocating a separate resource. */
void func_00346AD8(SdfPacFlags *state) {
    state->flags = state->flags | PAC_STATE_USE_PACKET_MEMORY;
}

/* Select the high-address allocator for packet payload resources. */
void func_00346AE8(SdfPacFlags *state) {
    state->flags = state->flags | PAC_STATE_ALLOCATE_HIGH;
}

INCLUDE_ASM(const s32, "game/code_00346778", func_00346AF8);

/* Mark the caller's phase byte finished. */
void func_00346B30(u8 *phaseByte) {
    *phaseByte = PAC_PHASE_FINISHED;
}

/* Consume byteCount bytes without bounds checks, updating the cursor and both counters. */
void sdfPacAdvanceInput(SdfPacInput *input, s32 byteCount) {
    input->inputCursor = input->inputCursor + byteCount;
    input->inputAvailable = input->inputAvailable - byteCount;
    input->consumedBytes = input->consumedBytes + byteCount;
}

/* Fixed fields of a packet header; packetBytes includes its header and extensions. */
typedef struct PacPacketPrefix {
    u8 command;
    u8 flags;
    u8 pad02[2];
    u32 packetBytes;
} PacPacketPrefix;

/* Stream updates use prefix; callback bookkeeping uses work at the same offsets. */
typedef union PacCallbackState {
    SdfPacInput prefix;
    struct {
        u8 phase;
        u8 flags;
        u16 packetCounter;
        s32 (*packetCallback)(void *, s32, void *, s32);
        u8 pad08[8];
        PacPacketPrefix *inputCursor;
        s32 inputAvailable;
        u32 consumedBytes;
        u32 pad1C;
        u32 pendingBytes;
    } work;
} PacCallbackState;

/* Align the stream, then report a packet header and its payload byte count to the callback. */
void sdfPacAdvanceCallbackBoundary(PacCallbackState *state) {
    PacPacketPrefix *inputHeader;
    u32 alignmentOffset;
    u32 packetBytes;
    u32 headerBytes;
    u32 payloadBytes;
    s32 (*packetCallback)(void *, s32, void *, s32);
    s32 callbackResult;

    alignmentOffset = state->work.consumedBytes & PAC_ALIGNMENT_MASK;
    if (alignmentOffset != 0) {
        state->work.phase = 1;
        state->work.pendingBytes = PAC_ALIGNMENT_BYTES - alignmentOffset;
        return;
    }
    inputHeader = state->work.inputCursor;
    /* Advance only the fixed header; retain its original address for the callback. */
    sdfPacAdvanceInput(&state->prefix, PAC_HEADER_BYTES);
    packetBytes = inputHeader->packetBytes;
    state->work.packetCounter++;
    /* The high nibble encodes extension bytes, not a bit offset. */
    headerBytes = (inputHeader->flags & PAC_EXTENSION_BYTES_MASK) + PAC_HEADER_BYTES;
    payloadBytes = packetBytes - headerBytes;
    state->work.pendingBytes = payloadBytes;
    packetCallback = state->work.packetCallback;
    callbackResult = packetCallback(state, 0, inputHeader, payloadBytes);
    if (callbackResult == PAC_CALLBACK_FINISHED) {
        func_00346B30(&state->work.phase);
        return;
    }
    if (callbackResult == PAC_CALLBACK_ALIGN) {
        state->work.phase = 1;
        state->work.pendingBytes = (state->work.pendingBytes + PAC_ALIGNMENT_MASK) & ~PAC_ALIGNMENT_MASK;
    } else {
        state->work.phase = 2;
    }
}

typedef struct PacQueuedPacket {
    struct PacQueuedPacket *next; /* 0x00 */
    u8 unk04[0x10];
} PacQueuedPacket;

/* Completion view of the PAC state: packet callback and queued packet links. */
typedef struct PacCompletionState {
    u8 phase;      /* 0x00 */
    u8 pad01[3];
    s32 (*packetCallback)(struct PacCompletionState *, s32, void *); /* 0x04 */
    u8 pad08[0x28];
    PacQueuedPacket *queueHead; /* 0x30 */
    PacQueuedPacket *queueTail; /* 0x34 */
} PacCompletionState;

extern void sdfReleaseChipBlock(void *);

/* Notify completion with the tail's header: result 1 finishes; result 4 releases the tail. */
void sdfDecodePacNodeAndAdvanceTail(PacCompletionState *state) {
    s32 callbackResult;
    PacQueuedPacket *tailPacket;
    PacQueuedPacket *scanPacket;
    PacQueuedPacket *nextPacket;

    callbackResult = state->packetCallback(state, 1, (u8 *)state->queueTail + PAC_HEADER_BYTES);
    if (callbackResult == PAC_CALLBACK_FINISHED) {
        func_00346B30((u8 *)state);
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

