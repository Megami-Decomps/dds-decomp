#include "common.h"

#define SDF_RELOC_HEADER_BYTES 0x20
#define SDF_STREAM_NODE_BYTES 0x8C
#define SDF_STREAM_FRAME_HEADER_BYTES 0x10
#define SDF_STREAM_SCRATCH_BYTES 0x10100
#define SDF_STREAM_PREFIX_BYTES 0x100
#define SDF_STREAM_SLOT_BYTES 0x2000
#define SDF_STREAM_SLOT_SHIFT 13
#define SDF_STREAM_RING_SLOTS 8
#define SDF_STREAM_MAX_FILLED 7
#define SDF_STREAM_TRAILER_START 0x1F00
#define SDF_STREAM_QWORD_BYTES 16
#define SDF_STREAM_PIXEL_BYTES_WIDE 4
#define SDF_STREAM_PIXEL_BYTES_NARROW 2
#define SDF_STREAM_PAGE_WIDTH 64
#define SDF_STREAM_PAGE_WIDTH_MASK 63
#define SDF_STREAM_WIDE_PAGE_HEIGHT 32
#define SDF_STREAM_WIDE_HEIGHT_MASK 31
#define SDF_STREAM_NARROW_PAGE_HEIGHT 64
#define SDF_STREAM_NARROW_HEIGHT_MASK 63
#define SDF_STREAM_PSMCT32 0
#define SDF_STREAM_PSMCT16 2
#define SDF_EE_PHYSICAL_MASK 0x0FFFFFFF
#define SDF_EE_UNCACHED_BASE 0x20000000
#define SDF_IPU_OUTPUT_DMA_ADDRESS 0x1000B010
#define SDF_IPU_OUTPUT_DMA_QWC 0x1000B020
#define SDF_IPU_OUTPUT_DMA_CTRL 0x1000B000
#define SDF_IPU_INPUT_DMA_ADDRESS 0x1000B410
#define SDF_IPU_INPUT_DMA_QWC 0x1000B420
#define SDF_IPU_INPUT_DMA_TAG_ADDRESS 0x1000B430
#define SDF_IPU_INPUT_DMA_CTRL 0x1000B400
#define SDF_IPU_DMA_START 0x100
#define SDF_IPU_DMA_NORMAL_START 0x101
#define SDF_IPU_DMA_CHAIN_START 0x105
#define SDF_IPU_CMD_REGISTER 0x10002000
#define SDF_IPU_FDEC 0x40000000
#define SDF_IPU_FDEC_BYTE 0x40000008

extern s32 D_003BD620;

extern s32 D_003BD624;

extern s32 D_003BD628;

extern s32 D_003BD62C;

extern u32 D_003BD630;

extern u32 D_003BD61C;

extern u64 sndBuildResourceHandleListFromOffsets(u32);
extern u64 sdfTexAcquireResourceTexture(u32);

extern u64 sdfReadNamedResource(u64, u32 *, u32 *);

extern u32 sdfSoundCommandStatus;

extern u32 sdfSoundCommandBusy;
extern char D_00398948[];
extern s32 sdfSoundRpcSemaphore;
extern s32 SignalSema(s32);
extern void FlushCache(s32);
extern s32 sdfCreateSemaphore(s32, s32, s32);
extern s32 GetThreadId(void);
extern void sceSifSetRpcQueue(void *, s32);
extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);
extern void sceSifRpcLoop(void *);
extern u8 D_003FEAC0[];
extern s32 func_002E99A0();

extern void sdfGetGeneralHeapStats(void *out);

extern s32 sdfPrintFormattedDevMessage(const char *fmt, ...);
extern char D_003B4880[];
extern char D_003B48A0[];
extern char D_003B48B8[];
extern char D_003B48E0[];
extern char D_003B4900[];
extern char *D_00398968[];

typedef struct MidiChannel {
    u8 pad00[0x19];
    u8 index;
    u8 enabled;
    u8 pad1B[5];
    u32 earlierEntries[2];
    u32 entries[8];
} MidiChannel;
typedef struct MidiPlaybackState {
    u8 pad00[0x13];
    u8 completed;
    u8 pad14;
    u8 looping;
    u8 pad16[3];
    u8 bufferIndex;
    u8 pending;
    u8 pad1B[0xD];
    u32 buffers[2];
    u32 bufferSize;
    u8 pad34[0xC];
    s32 limit;
    u8 pad44[4];
    s32 processed;
} MidiPlaybackState;

extern s32 func_00312C08(void);
extern void func_002EC230(s32);
extern s32 func_002EC060(void *);

extern u32 D_003BD494;

u32 sndSendCommandPacket(u32 command, u32 value, void *data, u32 size);

u32 func_002E87A8(u32 command, u32 value, void *data, u32 size);
typedef struct SdfStreamTextureHead {
    u8 pad00[0xC];
    s32 resourceWord;
} SdfStreamTextureHead;

/* Opaque IPU DMA environment saved at the end of the native stream node. */
typedef struct IpuDmaState {
    u8 pad0[8];
} IpuDmaState;

/* One 0x8C allocation owns the independent stream/sound links, feed ring
 * and IPU completion state; these are not separate prefix-only objects. */
typedef struct SdfStreamFrameNode {
    struct SdfStreamFrameNode *streamPrev;
    struct SdfStreamFrameNode *streamNext;
    struct SdfStreamFrameNode *next; /* Independent singly-linked sound list. */
    u8 active;
    u8 queued;
    u8 unk0E;
    u8 drained;
    u8 firstStop;
    u8 unk11;
    u8 pad12;
    u8 unk13;
    u8 audioMode; /* 0=no audio, 1=mono, 2=stereo */
    u8 loopMode;
    u8 playbackMode;
    u8 pad17;
    u8 bufferIndex; /* IPU output buffer selector, toggled after each submission. */
    u8 pad19;
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
    u32 cycleLength; /* Header word counted against completed IPU transfers. */
    u32 tickCount;
    u8 pad48[4];
    u32 unk4C;
    u8 headerReady;
    u8 done;
    u8 filledSlots;
    u8 firstSlot;
    u32 scratchBuffer;
    u8 pad58[4];
    s32 (*read)(struct SdfStreamFrameNode *, u32, s32, void *, s32);
    u32 source;
    u8 pad64;
    u8 unk65;
    u8 pad66[2];
    IpuDmaState dma;
    u32 unk70;
    u8 pad74[0x18];
} SdfStreamFrameNode;
typedef s32 (*SdfStreamRead)(SdfStreamFrameNode *, u32, s32, void *, s32);

typedef struct SoundFormat {
    u8 hasAudio;
    u8 stereo;
    u8 loopMode;
    u8 playbackMode;
} SoundFormat;

extern SdfStreamFrameNode *sdfSoundNodeHead;
extern SdfStreamFrameNode *D_003BDAA8;
extern s32 sceIpuSync(s32, s32);
extern void *sdfAllocateBlockBySizeThreshold(s32);
extern void sdfStreamOpen(SdfStreamFrameNode *, SoundFormat *, s32, s32);
extern void sdfSoundInitFormattedNode(SdfStreamFrameNode *, SoundFormat *, SdfStreamRead, u32);
extern s32 D_003BDA9C;
extern s32 D_003BDAA0;
extern void func_002CF7B8(s32);
extern s32 func_0030B5D0(s32);
extern u64 sdfDevReadResourceWithExtraSpace(u64, u32 *, u32 *, s32);

extern void func_002EB578(SdfStreamFrameNode *node, u8 *data, s32 size);

extern s32 sdfCreateConfiguredBufferedResourceList(s32);

extern void sdfAppendResourceListItem(s32, u64);

extern SdfStreamFrameNode *sdfStreamNodeListHead;

extern SdfStreamFrameNode *sdfStreamNodeListTail;

extern void sdfTexEnqueuePacketWithSemaphore(s32, s32);

extern void func_002EB650();

typedef struct SdfStreamParams {
    u8 mode;
    u8 param1;
    u8 param2;
    u8 param3;
} SdfStreamParams;

extern s32 sdfTexGetPrimaryResourceWord();

void func_002E9708(void) {
    func_002E87A8(0x180, 0, 0, 0);
}

void func_002E9730(void) {
    func_002E87A8(400, 0, 0, 0);
}

void func_002E9758(s32 channel) {
    func_002E87A8(((channel + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

s32 sdfSoundSendNamedCommand(const char *name, u8 channel) {
    if (sdfSoundCommandBusy != 0) {
        return 1;
    }
    strcpy(D_00398948, name);
    sndSendCommandPacket((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 sdfSoundIsCommandBusy(void) {
    return sdfSoundCommandBusy;
}

void sdfSoundStopNamedPlayback(void) {
    sndSendCommandPacket(0x100, 0, 0, 0);
}

void func_002E9810(u8 channel) {
    sndSendCommandPacket(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

void sdfSoundSetChannelCount(u32 channelCount) {
    if (0x10 < channelCount) {
        channelCount = 0x10;
    }
    if (channelCount == 0) {
        channelCount = 1;
    }
    sndSendCommandPacket((channelCount - 1) | 0x1d0, 0, 0, 0);
}

u32 sdfSoundTryQueueCommand(u32 command) {
    if (sdfSoundCommandStatus != 0) {
        return 0;
    }
    D_003BD494 = command;
    return command;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E98A0);

u32 sdfSoundGetCommandStatus(void) {
    return sdfSoundCommandStatus;
}

void func_002E98F0(void) {
    sndSendCommandPacket(0x100, 0, 0, 0);
}

void func_002E9918(u8 channel) {
    sndSendCommandPacket(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

s32 sdfSoundHandleRpcEvent(s32 unused, u32 event) {
    switch (event) {
    case 1:
        break;
    case 3:
        break;
    case 6:
        break;
    case 5:
        FlushCache(0);
        /* Fall through: flush and wake the waiting thread. */
    case 4:
        SignalSema(sdfSoundRpcSemaphore);
        break;
    case 0:
    case 2:
    case 7:
        SignalSema(sdfSoundRpcSemaphore);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E99A0);

void sdfSoundStartRpcServer(void) {
    u8 queue[0x20];
    u8 server[0x50];
    s32 semaphore = sdfCreateSemaphore(0, 1, 0);
    sdfSoundRpcSemaphore = semaphore;
    if (semaphore <= 0) {
        for (;;) {
        }
    }
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x54524E53, func_002E99A0, D_003FEAC0, 0, 0, queue);
    sceSifRpcLoop(queue);
}

extern s32 D_003BDA8C;
extern void func_002E8938(s32, void *, s32);
extern u8 sndMidiTrackState[];

void sdfSoundSetTableEntry(u32 kind, u8 *src) {
    u8 *table;
    u8 *dst;
    s32 i;

    switch (kind) {
    case 0:
        table = sndMidiTrackState;
        dst = table + 0x210;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 1:
        table = sndMidiTrackState;
        dst = table + 0x290;
        for (i = 0; i < 0x80; i++) {
            *dst++ = src[i];
        }
        break;
    case 2:
        table = sndMidiTrackState;
        dst = table + 0x310;
        for (i = 0; i < 0x80; i++) {
            *dst++ = src[i];
        }
        break;
    case 3:
        table = sndMidiTrackState;
        dst = table + 0x390;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 4:
        table = sndMidiTrackState;
        dst = table + 0x410;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 5:
        table = sndMidiTrackState;
        dst = table + 0x490;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 6:
        table = sndMidiTrackState;
        dst = table + 0x510;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 7:
        table = sndMidiTrackState;
        dst = table + 0x590;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 8:
        table = sndMidiTrackState;
        dst = table + 0x610;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 9:
        table = sndMidiTrackState;
        dst = table + 0x690;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 10:
        table = sndMidiTrackState;
        dst = table + 0x710;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 11:
        table = sndMidiTrackState;
        dst = table + 0x790;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 12:
        table = sndMidiTrackState;
        dst = table + 0x810;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    }
    func_002E8938(D_003BDA8C, sndMidiTrackState, 0x8D0);
    sndSendCommandPacket(0x1F0, 0, 0, 0);
}

void sdfSoundGetTableEntry(u32 kind, u8 *dst) {
    u8 *table;
    u8 *src;
    s32 i;

    sndSendCommandPacket(0x200, 0, 0, 0);
    switch (kind) {
    case 0:
        table = sndMidiTrackState;
        src = table + 0x210;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 1:
        table = sndMidiTrackState;
        src = table + 0x290;
        for (i = 0; i < 0x80; i++) {
            dst[i] = *src++;
        }
        return;
    case 2:
        table = sndMidiTrackState;
        src = table + 0x310;
        for (i = 0; i < 0x80; i++) {
            dst[i] = *src++;
        }
        return;
    case 3:
        table = sndMidiTrackState;
        src = table + 0x390;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 4:
        table = sndMidiTrackState;
        src = table + 0x410;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 5:
        table = sndMidiTrackState;
        src = table + 0x490;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 6:
        table = sndMidiTrackState;
        src = table + 0x510;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 7:
        table = sndMidiTrackState;
        src = table + 0x590;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 8:
        table = sndMidiTrackState;
        src = table + 0x610;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 9:
        table = sndMidiTrackState;
        src = table + 0x690;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 10:
        table = sndMidiTrackState;
        src = table + 0x710;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 11:
        table = sndMidiTrackState;
        src = table + 0x790;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 12:
        table = sndMidiTrackState;
        src = table + 0x810;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EA2E0);
INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4880);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48A0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48B8);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48E0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4900);

void func_002EA448(u32 *src) {
    u32 *callStack[2];
    u32 *cursor;
    u32 tag;
    u32 tagAddress;
    s32 tagType;
    s32 tagCount;
    s32 depth = 0;
    s32 done = 0;

    sdfPrintFormattedDevMessage(D_003B4880, src);
    cursor = src;
    for (;;) {
        if ((u32)cursor & 0xF) {
            sdfPrintFormattedDevMessage(D_003B48A0, cursor);
            return;
        }
        tag = cursor[0];
        tagAddress = cursor[1];
        tagType = (tag >> 28) & 7;
        tagCount = tag & 0xFFFF;
        sdfPrintFormattedDevMessage(D_003B48B8, cursor, D_00398968[tagType], tag, tagAddress, cursor[2], cursor[3]);
        cursor += 4;
        switch (tagType) {
        case 0:
            done = 1;
            break;
        case 1:
            cursor += tagCount * 4;
            break;
        case 2:
            cursor = (u32 *)tagAddress;
            break;
        case 3:
        case 4:
            break;
        case 5:
            if (depth == 2) {
                sdfPrintFormattedDevMessage(D_003B48E0);
                return;
            }
            callStack[depth++] = cursor;
            cursor = (u32 *)tagAddress;
            break;
        case 6:
            if (depth == 0) {
                done = 1;
            } else {
                cursor = callStack[--depth];
            }
            break;
        case 7:
            cursor += tagCount * 4;
            done = 1;
            break;
        }
        if (done != 0) {
            sdfPrintFormattedDevMessage(D_003B4900, cursor);
            return;
        }
    }
}




INCLUDE_ASM(const s32, "game/code_002E9708", func_002EA5C0);

typedef struct GsMemBlock {
    struct GsMemBlock *link0; /* 0x00 */
    struct GsMemBlock *link4; /* 0x04 */
    u32 type;                 /* 0x08 */
    u32 unkC;                 /* 0x0C */
    u32 unk10;                /* 0x10 */
} GsMemBlock;

extern char sdfGsMemoryDumpHeader[]; /* " <<< GS memory information >>>..." */
extern char sdfGsMemoryDumpRowFormat[]; /* " %08X : %08X %08X %8s %08X %d\n" */
extern char sdfGsMemoryTypeFormat[]; /* "%d" */
extern char *D_00398A28[];
extern GsMemBlock *sdfGetTextureListHead(void);
extern char *D_00398A38[];
extern GsMemBlock *sdfGetTextureBlockListHead(void);

void sdfDumpGsMemoryForward(void) {
    char buf[8];
    GsMemBlock *head;
    GsMemBlock *node;
    GsMemBlock *walk;
    char *name;

    sdfPrintFormattedDevMessage(sdfGsMemoryDumpHeader);
    head = sdfGetTextureListHead();
    node = head;
    while (node != 0) {
        if (node->type < 4) {
            name = D_00398A28[node->type];
        } else {
            sdfPrintFormattedDevMessage(buf, sdfGsMemoryTypeFormat, node->type);
            name = buf;
        }
        sdfPrintFormattedDevMessage(sdfGsMemoryDumpRowFormat, node, node->link0, node->link4, name, node->unkC, node->unk10);
        walk = head;
        while (walk != node) {
            walk = walk->link4;
        }
        node = node->link4;
    }
}

void sdfDumpGsMemoryBackward(void) {
    char buf[8];
    GsMemBlock *head;
    GsMemBlock *node;
    GsMemBlock *walk;
    char *name;

    sdfPrintFormattedDevMessage(sdfGsMemoryDumpHeader);
    head = sdfGetTextureBlockListHead();
    node = head;
    while (node != 0) {
        if (node->type < 4) {
            name = D_00398A38[node->type];
        } else {
            sdfPrintFormattedDevMessage(buf, sdfGsMemoryTypeFormat, node->type);
            name = buf;
        }
        sdfPrintFormattedDevMessage(sdfGsMemoryDumpRowFormat, node, node->link0, node->link4, name, node->unkC, node->unk10);
        walk = head;
        while (walk != node) {
            walk = walk->link0;
        }
        node = node->link0;
    }
}

INCLUDE_RODATA(const s32, "game/code_002E9708", sdfGsMemoryDumpHeader);

INCLUDE_RODATA(const s32, "game/code_002E9708", sdfGsMemoryDumpRowFormat);

void sndPrintMemoryInfo(void) {
    s32 info[6];
    sdfGetGeneralHeapStats(info);
    sdfPrintFormattedDevMessage(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info[0], info[1], info[2], info[3], info[4], info[5]);
}

typedef struct SdfChipStats {
    u32 totalBytes;
    u32 freeBytes;
    u32 blockCount;
    u32 emptyBlocks;
    u32 partialBlocks;
    u32 usedCells[7];
} SdfChipStats;

extern void sdfGetChipHeapStats(SdfChipStats *stats);
extern char D_003BD518[];

/* Print the chip heap totals and how many cells are in use per size class (1..16, 17..32, ...). */
void sdfPrintChipHeapInfo(void) {
    SdfChipStats stats;
    u32 limit = 16;
    s32 i = 0;
    u32 first;
    u32 *used;

    sdfGetChipHeapStats(&stats);
    sdfPrintFormattedDevMessage(" <<< chip memory information >>>\n                 total : 0x%06X\n            free total : 0x%06X\n            page count : %d\n       free page count : %d\n fragmented page count : %d\n", stats.totalBytes, stats.freeBytes, stats.blockCount, stats.emptyBlocks, stats.partialBlocks);
    sdfPrintFormattedDevMessage("\n several size alloc count...\n");
    first = 0;
    used = stats.usedCells;
    do {
        sdfPrintFormattedDevMessage("       for %3d .. %4d : %d\n", first, limit, *used++);
        i++;
        first = limit + 1;
        limit *= 2;
    } while (i != 7);
    sdfPrintFormattedDevMessage(D_003BD518, first);
}

extern s32 sdfDevCreateCommandState(u64 name);
extern s32 sdfDevQueueControlAndWait(s32 state);
extern void sdfDevQueueReadAndWait(s32 state, s32 buffer, s32 size);
extern void sdfDevWaitThenReleaseCommandState(s32 state);

/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
u64 sdfDevReadResourceWithExtraSpace(u64 name, u32 *outData, u32 *outSize, s32 extra) {
    s32 state = sdfDevCreateCommandState(name);
    s32 size = sdfDevQueueControlAndWait(state);
    s32 handle = sdfAllocGeneralBlock(size + extra);
    s32 address = sdfResourceRetainAddress(handle);

    sdfDevQueueReadAndWait(state, address, size);
    sdfDevWaitThenReleaseCommandState(state);
    if (outData != NULL) {
        *outData = address;
    } else {
        sdfDecrementAllocationReferenceCount(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}

u64 sdfReadNamedResource(u64 name, u32 *info, u32 *flags) {
    return sdfDevReadResourceWithExtraSpace(name, info, flags, 0);
}

u64 sdfLoadNamedResourceAndReleaseLookupHandle(u64 name) {
    u64 handle;
    u64 resource;
    u32 info[4];

    handle = sdfReadNamedResource(name, info, 0);
    resource = sdfTexAcquireResourceTexture(info[0]);
    sdfReleaseResourceAllocation(handle);
    return resource;
}

/* Count and relative offsets of resources linked into a sound handle. */
typedef struct SoundResourceList {
    u8 pad00[0x10];
    s32 count;
    s32 relativeOffsets[1];
} SoundResourceList;

u64 sndBuildResourceHandleListFromOffsets(u32 resource) {
    s32 i = 0;
    s32 count = ((SoundResourceList *)resource)->count;
    s32 handle = sdfCreateConfiguredBufferedResourceList(count);
    s32 *entry;
    if (count != i) {
        entry = ((SoundResourceList *)resource)->relativeOffsets;
        do {
            i++;
            sdfAppendResourceListItem(handle, sdfTexAcquireResourceTexture(resource + *entry));
            entry++;
        } while (i != count);
    }
    return handle;
}

u64 sndLoadNamedOffsetResourceList(u64 name) {
    u64 handle;
    u64 resource;
    u32 info[4];

    handle = sdfReadNamedResource(name, info, 0);
    resource = sndBuildResourceHandleListFromOffsets(info[0]);
    sdfReleaseResourceAllocation(handle);
    return resource;
}

/* Packed relocation header; payload begins at +0x20, followed by its fixup table. */
typedef struct PackedRelocationHeader {
    u8 pad00[0x10];
    s32 relocationOffset;
    u32 relocationBytes;
    u8 pad18[8];
} PackedRelocationHeader;

/* Relocate words in the payload and return its address, retaining integer address arithmetic. */
s32 sdfRelocatePackedResourcePayload(s32 resource) {
    s32 payload;

    payload = resource + SDF_RELOC_HEADER_BYTES;
    sdfRelocatePackedResourceWords(payload, payload, payload + ((PackedRelocationHeader *)resource)->relocationOffset, ((PackedRelocationHeader *)resource)->relocationBytes);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
u64 sdfLoadPackedResourceWithRelocatedPayload(u64 name, s32 *outPayload) {
    u32 info[4];
    u64 buffer = sdfReadNamedResource(name, info, 0);
    *outPayload = sdfRelocatePackedResourcePayload(info[0]);
    return buffer;
}

/* This entry point uses the same payload-relative relocation table. */
s32 sdfRelocatePackedResourceWordsFromHeader(s32 resource) {
    s32 payload;

    payload = resource + SDF_RELOC_HEADER_BYTES;
    sdfRelocatePackedResourceWords(payload, payload, payload + ((PackedRelocationHeader *)resource)->relocationOffset, ((PackedRelocationHeader *)resource)->relocationBytes);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
u64 sdfReadPackedResourceAndRelocateHeader(u64 name, s32 *outPayload) {
    u32 info[4];
    u64 buffer = sdfReadNamedResource(name, info, 0);
    *outPayload = sdfRelocatePackedResourceWordsFromHeader(info[0]);
    return buffer;
}

/* Byte-coded commands advance by word deltas, or relocate a run of following words.
 * Low tag bits select one-, two-, or three-byte deltas; tag 7 encodes a run.
 * tableBytes bounds the encoded byte stream, not the number of relocated words.
 */
void sdfRelocatePackedResourceWords(s32 *words, s32 relocationBase, u8 *table, s32 tableBytes) {
    u8 *commandCursor = table;
    s32 value;
    s32 runIndex;

    while (commandCursor - table < tableBytes) {
        value = *commandCursor++;
        if ((value & 1) == 0) {
            value >>= 1;
        } else if ((value & 2) == 0) {
            value = (value | *commandCursor++ << 8) >> 2;
        } else if ((value & 4) == 0) {
            value = (value | commandCursor[0] << 8 | commandCursor[1] << 16) >> 3;
            commandCursor += 2;
        } else {
            value = (value >> 3) + 2;
            for (runIndex = 0; runIndex < value; runIndex++) {
                words++;
                *words += relocationBase;
            }
            continue;
        }
        words += value;
        *words += relocationBase;
    }
}

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfStreamNodeUnlink(SdfStreamFrameNode *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    SdfStreamFrameNode *previousNode;
    SdfStreamFrameNode *nextNode;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = func_00312C08();
    }
    if (node->queued != 0) {
        previousNode = node->streamPrev;
        nextNode = node->streamNext;
        if (previousNode == 0) {
            sdfStreamNodeListHead = nextNode;
        } else {
            previousNode->streamNext = nextNode;
        }
        if (nextNode == 0) {
            sdfStreamNodeListTail = previousNode;
        } else {
            nextNode->streamPrev = previousNode;
        }
        node->queued = 0;
    }
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfStreamNodeAppend(SdfStreamFrameNode *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = func_00312C08();
    }
    if (node->queued != 0) {
        sdfStreamNodeUnlink(node, 1);
    }
    node->queued = 1;
    if (sdfStreamNodeListTail == 0) {
        sdfStreamNodeListHead = node;
    } else {
        sdfStreamNodeListTail->streamNext = node;
    }
    node->streamPrev = sdfStreamNodeListTail;
    node->streamNext = 0;
    sdfStreamNodeListTail = node;
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Walk link addresses to append to the independent singly-linked sound list. */
void sdfSoundAppendNode(SdfStreamFrameNode *node) {
    SdfStreamFrameNode **tailLink = &sdfSoundNodeHead;
    SdfStreamFrameNode *currentNode = *tailLink;
    if (currentNode != NULL) {
        tailLink = &currentNode->next;
        while ((currentNode = *tailLink) != NULL) {
            tailLink = &currentNode->next;
        }
    }
    *tailLink = node;
    node->next = NULL;
}

/* Unlink the first matching sound node; absence leaves the list unchanged. */
void sdfSoundRemoveNode(SdfStreamFrameNode *node) {
    SdfStreamFrameNode **nodeLink = &sdfSoundNodeHead;
    SdfStreamFrameNode *currentNode = *nodeLink;
    while (currentNode != NULL) {
        if (currentNode == node) {
            *nodeLink = currentNode->next;
            return;
        }
        nodeLink = &currentNode->next;
        currentNode = *nodeLink;
    }
}

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfAllocateStreamFrameBuffers(SdfStreamFrameNode *node) {
    s32 bytesPerPixel = SDF_STREAM_PIXEL_BYTES_WIDE;
    s32 size;
    if (node->audioMode != 0) {
        bytesPerPixel = SDF_STREAM_PIXEL_BYTES_NARROW;
    }
    size = node->width * node->height;
    size *= bytesPerPixel;
    node->bufferSize = size;
    node->buffers[0] = sdfAllocateBlockBySizeThreshold(size);
    node->buffers[1] = sdfAllocateBlockBySizeThreshold(size);
}
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB578);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB650);

/* Clear the node, map the format's two selector bytes to mode 0/1/2, and copy loop/playback modes. */
void sdfSoundInitNodeFromFormat(SdfStreamFrameNode *node, SoundFormat *format) {
    memset(node, 0, SDF_STREAM_NODE_BYTES);
    if (format->hasAudio == 0) {
        node->audioMode = 0;
    } else if (format->stereo == 0) {
        node->audioMode = 1;
    } else {
        node->audioMode = 2;
    }
    node->loopMode = format->loopMode;
    node->playbackMode = format->playbackMode;
}
/* Native 0x10 serialized header: dimensions and the DMA-cycle limit. */
typedef struct SdfStreamHeader {
    u8 pad00[8];
    u16 width;
    u16 height;
    u32 cycleLength;
} SdfStreamHeader;

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfStreamOpen(SdfStreamFrameNode *node, SoundFormat *format, s32 sourceAddress, s32 sourceSize) {
    s32 interruptsEnabled;
    u8 *frameBytes = (u8 *)sourceAddress;

    sdfSoundInitNodeFromFormat(node, format);
    node->width = ((SdfStreamHeader *)frameBytes)->width;
    node->cycleLength = ((SdfStreamHeader *)frameBytes)->cycleLength;
    node->height = ((SdfStreamHeader *)frameBytes)->height;
    sdfAllocateStreamFrameBuffers(node);
    func_002EB578(node, frameBytes + SDF_STREAM_FRAME_HEADER_BYTES, sourceSize - SDF_STREAM_FRAME_HEADER_BYTES);
    interruptsEnabled = func_00312C08();
    sdfStreamNodeAppend(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    func_002EC230(0);
}

/* Store the callback/source token and allocate an eight-slot feed ring plus its leading mirror bytes. */
void sdfSoundInitFormattedNode(SdfStreamFrameNode *node, SoundFormat *format, SdfStreamRead readSource, u32 source) {
    sdfSoundInitNodeFromFormat(node, format);
    node->read = readSource;
    node->source = source;
    node->active = 1;
    node->scratchBuffer = sdfAllocateBlockBySizeThreshold(SDF_STREAM_SCRATCH_BYTES) + SDF_STREAM_PREFIX_BYTES;
}

extern SdfStreamTextureHead *sdfTexAllocateHeadForDimensions(s32, s32, s32, s32, s32);

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfStreamInitializeFromHeader(SdfStreamFrameNode *node) {
    SdfStreamHeader header;
    s32 readStatus;
    s32 interruptsEnabled;

    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->source, 0, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->source, 1, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->cycleLength = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        SdfStreamTextureHead *texture;

        if (textureWidth > SDF_STREAM_PAGE_WIDTH) {
            textureWidth = (textureWidth + SDF_STREAM_PAGE_WIDTH_MASK) & ~SDF_STREAM_PAGE_WIDTH_MASK;
        }
        if (node->audioMode == 0) {
            pixelFormat = SDF_STREAM_PSMCT32;
            if (textureHeight > SDF_STREAM_WIDE_PAGE_HEIGHT) {
                textureHeight = (textureHeight + SDF_STREAM_WIDE_HEIGHT_MASK) & ~SDF_STREAM_WIDE_HEIGHT_MASK;
            }
        } else {
            pixelFormat = SDF_STREAM_PSMCT16;
            if (textureHeight > SDF_STREAM_NARROW_PAGE_HEIGHT) {
                textureHeight = (textureHeight + SDF_STREAM_NARROW_HEIGHT_MASK) & ~SDF_STREAM_NARROW_HEIGHT_MASK;
            }
        }
        texture = sdfTexAllocateHeadForDimensions(textureWidth, textureHeight, pixelFormat, 2, 0);
        node->textureHead = texture;
        node->resourceWord = texture->resourceWord;
    }
    sdfAllocateStreamFrameBuffers(node);
    interruptsEnabled = func_00312C08();
    sdfStreamNodeAppend(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

extern u32 D_003BDAA4;
extern void sdfFreeMemoryFromEitherHeap(void *);
extern void sdfTexQueueResourceRelease(s32);
extern void sdfTexQueuePendingWork(s32);

void func_002EBB60(SdfStreamFrameNode *node) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;

    interruptsEnabled = func_00312C08();
    wasActive = 0;
    node->drained = 1;
    sdfSoundRemoveNode(node);
    sdfStreamNodeUnlink(node, 1);
    if (D_003BDAA4 == (u32)node) {
        *(volatile u32 *)0x10002010 = 0x40000000;
        sceIpuSync(0, 0);
        dmaEnable = *(volatile u32 *)0x1000F520;
        *(volatile u32 *)0x1000F520 = 0x10000;
        *(volatile u32 *)0x1000B400 = 1;
        *(volatile u32 *)0x1000B000 = 0;
        *(volatile u32 *)0x1000F520 = dmaEnable;
        D_003BDAA4 = 0;
        wasActive = 1;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfFreeMemoryFromEitherHeap((void *)node->unk4C);
    sdfFreeMemoryFromEitherHeap((void *)node->buffers[0]);
    sdfFreeMemoryFromEitherHeap((void *)node->buffers[1]);
    sdfTexQueueResourceRelease(node->textureResources[0]);
    sdfTexQueueResourceRelease(node->textureResources[1]);
    if (node->scratchBuffer != 0) {
        sdfFreeMemoryFromEitherHeap((void *)(node->scratchBuffer - 0x100));
    }
    if (node->textureHead != 0) {
        sdfTexQueuePendingWork((s32)node->textureHead);
    }
    if (wasActive != 0) {
        func_002EC230(0);
    }
}

extern void *memcpy(void *dst, const void *src, u32 n);

/* Fill at most seven of eight 8-KiB slots. A short non-EOF query waits for more
 * data; EOF permits a partial final read. Copy the final slot's trailing bytes
 * into the prefix mirror, retaining the original copy even for a partial read.
 */
void sndFillStreamFeedRing(SdfStreamFrameNode *feed) {
    s32 writeSlot;
    s32 filledSlots;
    s32 availableBytes;
    u32 destinationAddress;
    s32 endOfStream;

    filledSlots = feed->filledSlots;
    if (feed->done == 0 && filledSlots < SDF_STREAM_MAX_FILLED) {
        do {
            writeSlot = feed->firstSlot;
            endOfStream = 0;
            destinationAddress = feed->scratchBuffer;
            writeSlot += filledSlots;
            if (writeSlot >= SDF_STREAM_RING_SLOTS) {
                writeSlot -= SDF_STREAM_RING_SLOTS;
            }
            destinationAddress += writeSlot << SDF_STREAM_SLOT_SHIFT;
            destinationAddress = (destinationAddress & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE;
            availableBytes = feed->read(feed, feed->source, 0, &endOfStream, 0);
            if (availableBytes < SDF_STREAM_SLOT_BYTES) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->source, 1, (void *)destinationAddress, availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == SDF_STREAM_SLOT_BYTES) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->source, 1, (void *)destinationAddress, SDF_STREAM_SLOT_BYTES);
                filledSlots++;
            }
            if (writeSlot == SDF_STREAM_MAX_FILLED) {
                memcpy((void *)(((feed->scratchBuffer - SDF_STREAM_PREFIX_BYTES) & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE), (void *)(destinationAddress + SDF_STREAM_TRAILER_START), SDF_STREAM_PREFIX_BYTES);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < SDF_STREAM_MAX_FILLED);
    }
}

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void func_002EBEB8(SdfStreamFrameNode *stream) {
    if (stream->active == 0) {
        D_003BDAA8 = stream;
        *(vu32 *)SDF_IPU_INPUT_DMA_TAG_ADDRESS = stream->unk4C & SDF_EE_PHYSICAL_MASK;
        *(vu32 *)SDF_IPU_INPUT_DMA_QWC = 0;
        *(vu32 *)SDF_IPU_INPUT_DMA_CTRL = SDF_IPU_DMA_CHAIN_START;
    } else {
        if (stream->filledSlots == 0) {
            stream->pad12 = 1;
            return;
        }
        D_003BDAA8 = stream;
        stream->pad64 = 1;
        *(vu32 *)SDF_IPU_INPUT_DMA_ADDRESS = (stream->scratchBuffer +
            (stream->firstSlot << SDF_STREAM_SLOT_SHIFT)) & SDF_EE_PHYSICAL_MASK;
        *(vu32 *)SDF_IPU_INPUT_DMA_QWC = SDF_STREAM_SLOT_BYTES / SDF_STREAM_QWORD_BYTES;
        *(vu32 *)SDF_IPU_INPUT_DMA_CTRL = SDF_IPU_DMA_NORMAL_START;
    }
    stream->pad12 = 0;
}

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfSoundQueueIpuBuffer(SdfStreamFrameNode *stream) {
    vu32 *dmaAddress = (vu32 *)SDF_IPU_OUTPUT_DMA_ADDRESS;
    vu32 *dmaQwords = (vu32 *)SDF_IPU_OUTPUT_DMA_QWC;
    vu32 *dmaControl = (vu32 *)SDF_IPU_OUTPUT_DMA_CTRL;
    u8 bufferIndex = stream->bufferIndex;
    *dmaAddress = stream->buffers[bufferIndex] & SDF_EE_PHYSICAL_MASK;
    *dmaQwords = stream->bufferSize / SDF_STREAM_QWORD_BYTES;
    *dmaControl = SDF_IPU_DMA_START;
    stream->bufferIndex = bufferIndex ^ 1;
}

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 sdfSoundSyncIpu(void) {
    vu32 *ipuCommand = (vu32 *)SDF_IPU_CMD_REGISTER;
    s32 decodedBits;
    *ipuCommand = SDF_IPU_FDEC;
    sceIpuSync(0, 0);
    decodedBits = *ipuCommand;
    sceIpuSync(0, 0);
    *ipuCommand = SDF_IPU_FDEC_BYTE;
    sceIpuSync(0, 0);
    return decodedBits;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC060);

/* Remove and advance the first list item matching the IPU cleanup test. */
void func_002EC230(s32 skipInterruptGuard) {
    SdfStreamFrameNode *node;
    s32 interruptsEnabled = 0;

    if (D_003BDAA4 != 0) {
        return;
    }
    if (skipInterruptGuard == 0) {
        interruptsEnabled = func_00312C08();
    }

    node = sdfStreamNodeListHead;
    while (node != 0) {
        if (node->unk1A + node->unk13 < 2 &&
            (node->active != 1 || node->filledSlots != 0)) {
            sdfStreamNodeUnlink(node, 1);
            func_002EC060(node);
            break;
        }
        node = node->streamNext;
    }

    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

extern void sceIpuStopDMA(void *);


/* Sleep for IPU completion, advance cycle state and requeue non-drained nodes;
 * the DMA worker runs indefinitely and retains the native completion-byte gates. */
void sdfIpuDmaCompletionWorker(void) {
    SdfStreamFrameNode *work;
    s32 interruptsEnabled;

    for (;;) {
        SleepThread();
        work = (SdfStreamFrameNode *)D_003BDAA4;
        if (work == NULL) {
            continue;
        }
        sceIpuStopDMA(&work->dma);
        if (work->unk70 == 0) {
            work->unk11 = 0;
        }
        if (work->firstStop == 0) {
            work->firstStop = 1;
        }
        work->unk1A++;
        D_003BDAA4 = 0;
        work->tickCount++;
        if (work->tickCount == work->cycleLength) {
            if (work->loopMode != 0) {
                work->unk0E = 0;
                work->tickCount = 0;
                work->unk65 = 0;
            } else {
                work->drained = 1;
            }
        }
        if (work->drained == 0) {
            interruptsEnabled = func_00312C08();
            sdfStreamNodeAppend(work, 0);
            if (interruptsEnabled != 0) {
                EIntr();
            }
        }
        func_002EC230(0);
    }
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC3C0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC3F0);

/* Return the indexed earlier entry only when enabled; the byte index is unchecked. */
u32 sdfMidiPreviousEntry(MidiChannel *channel) {
    u32 selectedEntry = 0;
    if (channel->enabled != 0) {
        selectedEntry = channel->earlierEntries[channel->index];
    }
    return selectedEntry;
}

/* Return the indexed current entry only when enabled; the byte index is unchecked. */
u32 sndGetSelectedChannelEntry(MidiChannel *channel) {
    u32 selectedEntry = 0;
    if (channel->enabled != 0) {
        selectedEntry = channel->entries[channel->index];
    }
    return selectedEntry;
}

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfAdvanceBufferedPlayback(MidiPlaybackState *state) {
    s32 interruptsEnabled = func_00312C08();
    s32 pendingBuffers = state->pending;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        state->pending = remainingBuffers;
        state->bufferIndex ^= 1;
        state->completed++;
    }
    state->processed++;
    if (state->processed == state->limit && state->looping != 0) {
        state->processed = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    func_002EC230(0);
}

/* Return zero only when nothing is pending; otherwise call the zero-buffer handler if needed, queue and advance. */
s32 sdfSubmitBufferedPlayback(MidiPlaybackState *state) {
    u32 *selectedBuffer;
    if (state->pending == 0) {
        return 0;
    }
    /* Equivalent to &state->buffers[state->bufferIndex]; index-first arithmetic matches retail. */
    selectedBuffer = (u32 *)(state->bufferIndex * 4 + (s32)state + 0x28);
    if (*selectedBuffer == 0) {
        func_002EB650();
    }
    sdfTexEnqueuePacketWithSemaphore(*selectedBuffer, *selectedBuffer + state->bufferSize - SDF_STREAM_QWORD_BYTES);
    sdfAdvanceBufferedPlayback(state);
    return 1;
}
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC5E0);

void sdfSoundInitAndAppendNode(SdfStreamFrameNode *node, s32 format, s32 source, s32 sourceSize, s32 value) {
    sdfStreamOpen(node, format, source, sourceSize);
    node->resourceWord = value;
    sdfSoundAppendNode(node);
}

void sdfStreamCreateWithParams(s32 node, SdfStreamParams *params, s32 sourceData, s32 sourceSize, u8 *source) {
    SdfStreamParams local = *params;
    switch (source[0x1A]) {
    case 0:
        local.mode = 0;
        break;
    case 2:
        local.mode = 1;
        break;
    }
    sdfSoundInitAndAppendNode(node, &local, sourceData, sourceSize, sdfTexGetPrimaryResourceWord(source));
}

void sdfSoundInitFormattedAndAppendNode(SdfStreamFrameNode *node, SoundFormat *format, SdfStreamRead read, u32 source, s32 value) {
    sdfSoundInitFormattedNode(node, format, read, source);
    node->resourceWord = value;
    sdfSoundAppendNode(node);
}

extern u32 D_003BDAA4;
extern s32 D_003BDAB4;
extern void sceIpuInit(void);
extern s32 sdfAddHandler(s32, s32, void *, s32, s32);
extern void func_0030B638(s32);
extern s32 sdfCreateThread(void *, void *, s32, s32);
extern void _StartThread();
extern u8 sdfIpuStreamThreadStack[];
extern s32 func_002EC3C0();
extern s32 func_002EC3F0();
extern void sdfIpuDmaCompletionWorker();
void sdfSoundInitIpuStream(void) {
    s32 thread;

    sceIpuInit();
    *(volatile s32 *)0x10002000 = 0x90000000;
    sdfSoundNodeHead = 0;
    D_003BDAA4 = 0;
    D_003BDA9C = sdfAddHandler(1, 3, func_002EC3C0, -1, 0);
    func_0030B638(3);
    D_003BDAA0 = sdfAddHandler(1, 4, func_002EC3F0, -1, 0);
    func_0030B638(4);
    thread = sdfCreateThread(sdfIpuDmaCompletionWorker, sdfIpuStreamThreadStack, 0x800, 0x46);
    D_003BDAB4 = thread;
    _StartThread(thread, 0);
}

s32 sdfSoundShutdownIpuStreamHandlers(void) {
    if (D_003BDA9C != 0) {
        func_002CF7B8(D_003BDA9C);
        D_003BDA9C = 0;
    }
    func_0030B5D0(3);
    if (D_003BDAA0 != 0) {
        func_002CF7B8(D_003BDAA0);
        D_003BDAA0 = 0;
    }
    return func_0030B5D0(4);
}

void func_002EC950(void) {
    func_003004E8();
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC968);

void func_002ECA40(u32 value) {
    D_003BD61C = value;
}

void sdfSetGridScaledDrawBounds(s32 column, s32 row, s32 width, s32 height, u32 mode) {
    D_003BD620 = column * 0x10 + 0x7000;
    D_003BD624 = row * 8 + 0x7900;
    D_003BD628 = D_003BD620 + width * 0x10;
    D_003BD62C = D_003BD624 + height * 8;
    D_003BD630 = mode;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002ECA80);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002ECCF8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD494);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfSoundCommandBusy);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfSoundCommandStatus);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A4);

INCLUDE_SDATA(const s32, "game/code_002E9708", fileIdleUpdateCallback);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4B0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4B8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4C0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4C8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4D0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4D8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4E0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4E8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4F0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4F8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD500);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD508);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD510);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD518);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD520);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD528);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD530);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD538);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD540);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD548);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD550);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD558);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD560);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD568);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD570);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD578);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD580);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD588);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD590);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD598);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5A0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5A8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5B0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5B8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5C0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5C8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5D0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5D8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5E0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5E8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5F0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD5F8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD600);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD608);

INCLUDE_SDATA(const s32, "game/code_002E9708", sdfGsMemoryTypeFormat);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD614);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD618);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD61C);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD620);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD624);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD628);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD62C);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD630);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD633);

