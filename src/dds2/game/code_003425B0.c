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
#define SDF_IPU_DMA_START 0x100
#define SDF_IPU_CMD_REGISTER 0x10002000
#define SDF_IPU_FDEC 0x40000000
#define SDF_IPU_FDEC_BYTE 0x40000008

extern s32 D_00439204;

extern u32 sdfSoundCommandBusy;

extern u32 sdfSoundCommandStatus;

extern u64 sdfTexAcquireResourceTexture(u32);

extern u64 sdfReadNamedResource(u64, u32 *, u32 *);

extern u64 sndBuildResourceHandleListFromOffsets(u32);

extern u32 D_00438D0C;

extern s32 D_00438D10;

extern s32 D_00438D14;

extern s32 D_00438D18;

extern s32 D_00438D1C;

extern u32 D_00438D20;

u32 func_00341650(u32, u32, void *, u32);

u32 sndSendCommandPacket(u32, u32, void *, u32);

extern u32 D_00438B84;

extern void sdfGetGeneralHeapStats(void *out);

extern void sdfPrintFormattedDevMessage(char *fmt, ...);

typedef struct MidiChannel {
    u8 pad00[0x19];
    u8 index;
    u8 enabled;
    u8 pad1B[5];
    u32 earlierEntries[2];
    u32 entries[8];
} MidiChannel;

extern char D_0040BAF8[];

extern s32 sdfSoundRpcSemaphore;

extern s32 sdfCreateSemaphore(s32, s32, s32);

extern s32 GetThreadId(void);

extern void sceSifSetRpcQueue(void *, s32);

extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);

extern void sceSifRpcLoop(void *);

extern u8 D_0047B440[];

extern s32 func_00342848();

typedef struct SdfStreamTextureHead {
    u8 pad00[0xC];
    s32 resourceWord;
} SdfStreamTextureHead;

typedef struct SdfStreamFrameNode {
    u8 pad00[8];
    struct SdfStreamFrameNode *next; /* 0x08: sound list link */
    u8 active;
    u8 pad0D[2];
    u8 drained;
    u8 pad10[4];
    u8 audioMode;     /* 0x14: 0=none, 1=mono, 2=stereo */
    u8 loopMode;      /* 0x15 */
    u8 playbackMode;  /* 0x16 */
    u8 pad17[5];
    s32 bufferSize;   /* 0x1C */
    u32 buffers[2];   /* 0x20 */
    s32 textureResources[2];
    u8 pad30[4];
    s32 resourceWord; /* 0x34: retained resource handle */
    SdfStreamTextureHead *textureHead; /* 0x38 */
    u16 width;        /* 0x3C */
    u16 height;       /* 0x3E */
    s32 sourceBytes;  /* 0x40: from stream header */
    u8 pad44[8];
    u32 unk4C;
    u8 headerReady;   /* 0x50 */
    u8 done;          /* 0x51 */
    u8 filledSlots;   /* 0x52 */
    u8 firstSlot;     /* 0x53 */
    u32 scratchBuffer; /* 0x54 */
    u8 pad58[4];
    s32 (*read)(struct SdfStreamFrameNode *, u32, s32, void *, s32); /* 0x5C */
    u32 source;       /* 0x60 */
    u8 pad64[0x28];
} SdfStreamFrameNode;
typedef s32 (*SdfStreamRead)(SdfStreamFrameNode *, u32, s32, void *, s32);
extern SdfStreamFrameNode *sdfSoundNodeHead;

typedef struct SdfStreamNode {
    struct SdfStreamNode *prev;
    struct SdfStreamNode *next;
    u8 pad8[5];
    u8 queued;
} SdfStreamNode;

extern SdfStreamNode *sdfStreamNodeListHead;
extern SdfStreamNode *sdfStreamNodeListTail;

extern s32 sceIpuSync(s32, s32);

typedef struct SoundIpuBuffer {
    u8 pad00[0x18];
    u8 active;
    u8 pad19[3];
    s32 size;
    u32 buffers[2];
} SoundIpuBuffer;

/* The relocation command stream begins at payload + relocationOffset.
 * Its byte-coded entries in sdfRelocatePackedResourceWords add payload to selected words. */
typedef struct SdfRelocResource {
    u8 pad00[0x10];
    s32 relocationOffset;
    u32 relocationBytes;
    u8 payload[1];
} SdfRelocResource;

typedef struct SdfResourceList {
    u8 pad00[0x10];
    s32 count;        /* 0x10 */
    s32 offsets[1];   /* 0x14: relative to the resource base */
} SdfResourceList;

typedef struct SdfStreamHeader {
    u8 pad00[8];
    u16 width;        /* 0x08 */
    u16 height;       /* 0x0A */
    s32 sourceBytes;  /* 0x0C */
} SdfStreamHeader;

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

extern s32 func_0036DE70(void);

extern void func_003450D8(s32);

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

extern char *D_0040BBD8[];

extern GsMemBlock *sdfGetTextureListHead(void);

extern char *D_0040BBE8[];

extern GsMemBlock *sdfGetTextureBlockListHead(void);

void func_003425B0(void) {
    func_00341650(0x180, 0, 0, 0);
}

void func_003425D8(void) {
    func_00341650(400, 0, 0, 0);
}

void func_00342600(s32 channel) {
    func_00341650(((channel + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

s32 sdfSoundSendNamedCommand(const char *name, u8 channel) {
    if (sdfSoundCommandBusy != 0) {
        return 1;
    }
    strcpy(D_0040BAF8, name);
    sndSendCommandPacket((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 sdfSoundIsCommandBusy(void) {
    return sdfSoundCommandBusy;
}

void sdfSoundStopNamedPlayback(void) {
    sndSendCommandPacket(0x100, 0, 0, 0);
}

void func_003426B8(u8 channel) {
    sndSendCommandPacket(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

void sdfSoundSetChannelCount(u32 channels) {
    if (0x10 < channels) {
        channels = 0x10;
    }
    if (channels == 0) {
        channels = 1;
    }
    sndSendCommandPacket((channels - 1) | 0x1d0, 0, 0, 0);
}

u32 sdfSoundTryQueueCommand(u32 command) {
    if (sdfSoundCommandStatus != 0) {
        return 0;
    }
    D_00438B84 = command;
    return command;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342748);

u32 sdfSoundGetCommandStatus(void) {
    return sdfSoundCommandStatus;
}

void func_00342798(void) {
    sndSendCommandPacket(0x100, 0, 0, 0);
}

void func_003427C0(u8 channel) {
    sndSendCommandPacket(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

extern void FlushCache(s32);
extern s32 SignalSema(s32);

/* Event 5 invalidates the EE cache before waking the waiting sound thread. */
s32 sdfSoundHandleRpcEvent(s32 unused, u32 event) {
    switch (event) {
    case 5:
        FlushCache(0);
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

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342848);

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
    sceSifRegisterRpc(server, 0x54524E53, func_00342848, D_0047B440, 0, 0, queue);
    sceSifRpcLoop(queue);
}

extern s32 D_004391EC;
extern void func_003417E0(s32, void *, s32);
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
    func_003417E0(D_004391EC, sndMidiTrackState, 0x8D0);
    sndSendCommandPacket(0x1F0, 0, 0, 0);
}

void func_00342E58(u32 kind, u8 *dst) {
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
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 6:
        table = sndMidiTrackState;
        src = table + 0x510;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 7:
        table = sndMidiTrackState;
        src = table + 0x590;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 8:
        table = sndMidiTrackState;
        src = table + 0x610;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 9:
        table = sndMidiTrackState;
        src = table + 0x690;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 10:
        table = sndMidiTrackState;
        src = table + 0x710;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 11:
        table = sndMidiTrackState;
        src = table + 0x790;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 12:
        table = sndMidiTrackState;
        src = table + 0x810;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343188);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003432F0);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E5B0);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E5C8);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E5F0);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E610);

INCLUDE_RODATA(const s32, "game/code_003425B0", jtbl_0042E630);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343468);

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
            name = D_0040BBD8[node->type];
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
            name = D_0040BBE8[node->type];
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

INCLUDE_RODATA(const s32, "game/code_003425B0", sdfGsMemoryDumpHeader);

INCLUDE_RODATA(const s32, "game/code_003425B0", sdfGsMemoryDumpRowFormat);

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
extern char D_00438C08[];

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
    sdfPrintFormattedDevMessage(D_00438C08, first);
}

extern s32 sdfDevCreateCommandState(u64 name);
extern s32 sdfDevQueueControlAndWait(s32 state);
extern void sdfDevQueueReadAndWait(s32 state, s32 buffer, s32 size);
extern void sdfDevWaitThenReleaseCommandState(s32 state);
extern s32 sdfAllocGeneralBlock(s32 size);
extern s32 sdfResourceRetainAddress(s32 handle);
extern void sdfDecrementAllocationReferenceCount(s32 handle);

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

u64 sdfReadNamedResource(u64 source, u32 *info, u32 *options) {
    return sdfDevReadResourceWithExtraSpace(source, info, options, 0);
}

u64 sdfLoadNamedResourceAndReleaseLookupHandle(u64 source) {
    u64 buffer;
    u64 result;
    u32 info[4];

    buffer = sdfReadNamedResource(source, info, 0);
    result = sdfTexAcquireResourceTexture(info[0]);
    sdfReleaseResourceAllocation(buffer);
    return result;
}

extern s32 sdfCreateConfiguredBufferedResourceList(s32);
extern void sdfAppendResourceListItem(s32, u64);

u64 sndBuildResourceHandleListFromOffsets(u32 resource) {
    s32 i = 0;
    s32 count = ((SdfResourceList *)resource)->count;
    s32 handle = sdfCreateConfiguredBufferedResourceList(count);
    s32 *entry;
    if (count != i) {
        entry = (s32 *)(resource + 0x14);
        do {
            i++;
            sdfAppendResourceListItem(handle, sdfTexAcquireResourceTexture(resource + *entry));
            entry++;
        } while (i != count);
    }
    return handle;
}

u64 sndLoadNamedOffsetResourceList(u64 source) {
    u64 buffer;
    u64 result;
    u32 info[4];

    buffer = sdfReadNamedResource(source, info, 0);
    result = sndBuildResourceHandleListFromOffsets(info[0]);
    sdfReleaseResourceAllocation(buffer);
    return result;
}

/* Relocate words in the payload and return its address, retaining integer address arithmetic. */
s32 sdfRelocatePackedResourcePayload(SdfRelocResource *resource) {
    s32 payload;

    /* Required to match: integer address arithmetic, not &resource->payload. */
    payload = (s32)resource + SDF_RELOC_HEADER_BYTES;
    sdfRelocatePackedResourceWords(payload, payload, payload + resource->relocationOffset, resource->relocationBytes);
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
s32 sdfRelocatePackedResourceWordsFromHeader(SdfRelocResource *resource) {
    s32 payload;

    /* Required to match: the typed member address changes one instruction. */
    payload = (s32)resource + SDF_RELOC_HEADER_BYTES;
    sdfRelocatePackedResourceWords(payload, payload, payload + resource->relocationOffset, resource->relocationBytes);
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
void sdfStreamNodeUnlink(SdfStreamNode *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    SdfStreamNode *previousNode;
    SdfStreamNode *nextNode;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = func_0036DE70();
    }
    if (node->queued != 0) {
        previousNode = node->prev;
        nextNode = node->next;
        if (previousNode == 0) {
            sdfStreamNodeListHead = nextNode;
        } else {
            previousNode->next = nextNode;
        }
        if (nextNode == 0) {
            sdfStreamNodeListTail = previousNode;
        } else {
            nextNode->prev = previousNode;
        }
        node->queued = 0;
    }
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfStreamNodeAppend(SdfStreamNode *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = func_0036DE70();
    }
    if (node->queued != 0) {
        sdfStreamNodeUnlink(node, 1);
    }
    node->queued = 1;
    if (sdfStreamNodeListTail == 0) {
        sdfStreamNodeListHead = node;
    } else {
        sdfStreamNodeListTail->next = node;
    }
    node->prev = sdfStreamNodeListTail;
    node->next = 0;
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


extern s32 sdfAllocateBlockBySizeThreshold(s32);

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

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344420);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003444F8);

/* The four bytes used to initialize the stream node; matches the DDS1 format. */
typedef struct SoundFormat {
    u8 hasAudio;
    u8 stereo;
    u8 loopMode;
    u8 playbackMode;
} SoundFormat;

extern void *memset(void *, s32, u32);

/* Clear the node, map the format's two selector bytes to mode 0/1/2, and copy loop/playback modes. */
void sdfSoundInitNodeFromFormat(u8 *nodeBytes, u8 *formatBytes) {
    memset(nodeBytes, 0, SDF_STREAM_NODE_BYTES);
    if (((SoundFormat *)formatBytes)->hasAudio == 0) {
        ((SdfStreamFrameNode *)nodeBytes)->audioMode = 0;
    } else {
        if (((SoundFormat *)formatBytes)->stereo == 0) {
            ((SdfStreamFrameNode *)nodeBytes)->audioMode = 1;
        } else {
            ((SdfStreamFrameNode *)nodeBytes)->audioMode = 2;
        }
    }
    ((SdfStreamFrameNode *)nodeBytes)->loopMode = ((SoundFormat *)formatBytes)->loopMode;
    ((SdfStreamFrameNode *)nodeBytes)->playbackMode = ((SoundFormat *)formatBytes)->playbackMode;
}

extern void func_00344420();

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfStreamOpen(u8 *nodeBytes, s32 format, u8 *frameBytes, s32 sourceSize) {
    s32 interruptsEnabled;
    sdfSoundInitNodeFromFormat(nodeBytes, format);
    ((SdfStreamFrameNode *)nodeBytes)->width = ((SdfStreamHeader *)frameBytes)->width;
    ((SdfStreamFrameNode *)nodeBytes)->sourceBytes = ((SdfStreamHeader *)frameBytes)->sourceBytes;
    ((SdfStreamFrameNode *)nodeBytes)->height = ((SdfStreamHeader *)frameBytes)->height;
    sdfAllocateStreamFrameBuffers(nodeBytes);
    func_00344420(nodeBytes, frameBytes + SDF_STREAM_FRAME_HEADER_BYTES, sourceSize - SDF_STREAM_FRAME_HEADER_BYTES);
    interruptsEnabled = func_0036DE70();
    sdfStreamNodeAppend((SdfStreamNode *)nodeBytes, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    func_003450D8(0);
}

extern s32 sdfAllocateBlockBySizeThreshold(s32);
extern void sdfSoundInitNodeFromFormat();

/* Store the callback/source token and allocate an eight-slot feed ring plus its leading mirror bytes. */
void sdfSoundInitFormattedNode(u8 *nodeBytes, s32 format, SdfStreamRead readSource, u32 source) {
    sdfSoundInitNodeFromFormat(nodeBytes, format);
    ((SdfStreamFrameNode *)nodeBytes)->read = readSource;
    ((SdfStreamFrameNode *)nodeBytes)->source = source;
    ((SdfStreamFrameNode *)nodeBytes)->active = 1;
    ((SdfStreamFrameNode *)nodeBytes)->scratchBuffer = sdfAllocateBlockBySizeThreshold(SDF_STREAM_SCRATCH_BYTES) + SDF_STREAM_PREFIX_BYTES;
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
    node->sourceBytes = header.sourceBytes;
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
    interruptsEnabled = func_0036DE70();
    sdfStreamNodeAppend((SdfStreamNode *)node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

extern void sdfFreeMemoryFromEitherHeap(void *);
extern void sdfTexQueueResourceRelease(s32);
extern void sdfTexQueuePendingWork(s32);

void func_00344A08(SdfStreamFrameNode *node) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;

    interruptsEnabled = func_0036DE70();
    wasActive = 0;
    node->drained = 1;
    sdfSoundRemoveNode(node);
    sdfStreamNodeUnlink((SdfStreamNode *)node, 1);
    if (D_00439204 == (u32)node) {
        *(volatile u32 *)0x10002010 = 0x40000000;
        sceIpuSync(0, 0);
        dmaEnable = *(volatile u32 *)0x1000F520;
        *(volatile u32 *)0x1000F520 = 0x10000;
        *(volatile u32 *)0x1000B400 = 1;
        *(volatile u32 *)0x1000B000 = 0;
        *(volatile u32 *)0x1000F520 = dmaEnable;
        D_00439204 = 0;
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
        func_003450D8(0);
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

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344D60);

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfSoundQueueIpuBuffer(SoundIpuBuffer *stream) {
    vu32 *dmaAddress = (vu32 *)SDF_IPU_OUTPUT_DMA_ADDRESS;
    vu32 *dmaQwords = (vu32 *)SDF_IPU_OUTPUT_DMA_QWC;
    vu32 *dmaControl = (vu32 *)SDF_IPU_OUTPUT_DMA_CTRL;
    u8 bufferIndex = stream->active;
    *dmaAddress = stream->buffers[bufferIndex] & SDF_EE_PHYSICAL_MASK;
    *dmaQwords = stream->size / SDF_STREAM_QWORD_BYTES;
    *dmaControl = SDF_IPU_DMA_START;
    stream->active = bufferIndex ^ 1;
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

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344F08);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003450D8);

extern void sceIpuStopDMA(void *);

/* IPU stream worker: queued stream node plus the DMA progress counters polled by this thread. */
/* DMA control block embedded in the worker at 0x68. */
typedef struct IpuDmaState {
    u8 pad0[8];
} IpuDmaState;

typedef struct IpuWorker {
    SdfStreamNode *prev;
    SdfStreamNode *next;
    u8 pad08[5];
    u8 queued;
    u8 unk0E;
    u8 drained;      /* 0x0F: set when the cycle wraps without the drain flag */
    u8 firstStop;    /* 0x10: set on the first DMA stop, cleared by the worker */
    u8 unk11;
    u8 pad12[3];
    u8 unk15;
    u8 pad16[4];
    u8 unk1A;
    u8 pad1B[0x25];
    u32 cycleLength; /* 0x40: cycle the tick counter counts up to */
    u32 tickCount;   /* 0x44: incremented per completed DMA, reset at cycleLength */
    u8 pad48[0x1D];
    u8 unk65;
    u8 pad66[2];
    IpuDmaState dma; /* 0x68: embedded block, address handed to sceIpuStopDMA */
    u32 unk70;
} IpuWorker;

void sdfIpuDmaCompletionWorker(void) {
    IpuWorker *work;
    s32 interruptsEnabled;

    for (;;) {
        SleepThread();
        work = (IpuWorker *)D_00439204;
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
        D_00439204 = 0;
        work->tickCount++;
        if (work->tickCount == work->cycleLength) {
            if (work->unk15 != 0) {
                work->unk0E = 0;
                work->tickCount = 0;
                work->unk65 = 0;
            } else {
                work->drained = 1;
            }
        }
        if (work->drained == 0) {
            interruptsEnabled = func_0036DE70();
            sdfStreamNodeAppend((SdfStreamNode *)work, 0);
            if (interruptsEnabled != 0) {
                EIntr();
            }
        }
        func_003450D8(0);
    }
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345268);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345298);

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
    s32 interruptsEnabled = func_0036DE70();
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
    func_003450D8(0);
}

extern void sdfTexEnqueuePacketWithSemaphore(s32, s32);
extern void func_003444F8();

/* Return zero only when nothing is pending; otherwise call the zero-buffer handler if needed, queue and advance. */
s32 sdfSubmitBufferedPlayback(MidiPlaybackState *state) {
    u32 *selectedBuffer;
    if (state->pending == 0) {
        return 0;
    }
    /* Equivalent to &state->buffers[state->bufferIndex]; index-first arithmetic matches retail. */
    selectedBuffer = (u32 *)(state->bufferIndex * 4 + (s32)state + 0x28);
    if (*selectedBuffer == 0) {
        func_003444F8();
    }
    sdfTexEnqueuePacketWithSemaphore(*selectedBuffer, *selectedBuffer + state->bufferSize - SDF_STREAM_QWORD_BYTES);
    sdfAdvanceBufferedPlayback(state);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345488);

void sdfSoundInitAndAppendNode(u8 *state, s32 format, s32 source, s32 size, s32 resource) {
    sdfStreamOpen(state, format, source, size);
    ((SdfStreamFrameNode *)state)->resourceWord = resource;
    sdfSoundAppendNode((SdfStreamFrameNode *)state);
}

typedef struct SdfStreamParams {
    u8 mode;
    u8 param1;
    u8 param2;
    u8 param3;
} SdfStreamParams;

extern s32 sdfTexGetPrimaryResourceWord();

void sdfStreamCreateWithParams(s32 state, SdfStreamParams *params, s32 source, s32 size, u8 *resource) {
    SdfStreamParams local = *params;
    switch (resource[0x1A]) {
    case 0:
        local.mode = 0;
        break;
    case 2:
        local.mode = 1;
        break;
    }
    sdfSoundInitAndAppendNode(state, &local, source, size, sdfTexGetPrimaryResourceWord(resource));
}

void sdfSoundInitFormattedAndAppendNode(u8 *state, s32 format, SdfStreamRead read, u32 source, s32 resource) {
    sdfSoundInitFormattedNode(state, format, read, source);
    ((SdfStreamFrameNode *)state)->resourceWord = resource;
    sdfSoundAppendNode((SdfStreamFrameNode *)state);
}

extern s32 D_004391FC;
extern s32 D_00439200;
extern s32 D_00439214;
extern u8 sdfIpuStreamThreadStack[];
extern s32 sdfAddHandler();
extern s32 sdfCreateThread();
extern void func_003668B8();
extern void sceIpuInit();
extern void _StartThread();
extern void sdfIpuDmaCompletionWorker();
extern void func_00345268();
extern void func_00345298();

void sdfSoundInitIpuStream(void) {
    sceIpuInit();
    *(vu32 *)0x10002000 = 0x90000000;
    sdfSoundNodeHead = 0;
    D_00439204 = 0;
    D_004391FC = sdfAddHandler(1, 3, func_00345268, -1, 0);
    func_003668B8(3);
    D_00439200 = sdfAddHandler(1, 4, func_00345298, -1, 0);
    func_003668B8(4);
    D_00439214 = sdfCreateThread(sdfIpuDmaCompletionWorker, sdfIpuStreamThreadStack, 0x800, 0x46);
    _StartThread(D_00439214, 0);
}

extern s32 D_004391FC;
extern s32 D_00439200;
extern void func_00328668();
extern s32 func_00366850();

s32 func_003457A8(void) {
    if (D_004391FC != 0) {
        func_00328668(D_004391FC);
        D_004391FC = 0;
    }
    func_00366850(3);
    if (D_00439200 != 0) {
        func_00328668(D_00439200);
        D_00439200 = 0;
    }
    return func_00366850(4);
}

void func_003457F8(void) {
    func_0035B7D8();
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345810);

void func_003458E8(u32 value) {
    D_00438D0C = value;
}

void sdfSetGridScaledDrawBounds(s32 firstStart, s32 secondStart, s32 firstLength, s32 secondLength, u32 value) {
    D_00438D10 = firstStart * 0x10 + 0x7000;
    D_00438D14 = secondStart * 8 + 0x7900;
    D_00438D18 = D_00438D10 + firstLength * 0x10;
    D_00438D1C = D_00438D14 + secondLength * 8;
    D_00438D20 = value;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345928);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345BA0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B84);

INCLUDE_SDATA(const s32, "game/code_003425B0", sdfSoundCommandBusy);

INCLUDE_SDATA(const s32, "game/code_003425B0", sdfSoundCommandStatus);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B90);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B94);

INCLUDE_SDATA(const s32, "game/code_003425B0", fileIdleUpdateCallback);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BA0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BA8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BB0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BB8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BC0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BC8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BD0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BD8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BE0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BE8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BF0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438BF8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C00);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C08);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C10);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C18);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C20);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C28);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C30);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C38);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C40);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C48);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C50);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C58);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C60);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C68);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C70);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C78);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C80);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C88);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C90);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438C98);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CA0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CA8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CB0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CB8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CC0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CC8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CD0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CD8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CE0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CE8);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CF0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438CF8);

INCLUDE_SDATA(const s32, "game/code_003425B0", sdfGsMemoryTypeFormat);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D04);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D08);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D0C);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D10);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D14);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D18);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D1C);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D20);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D23);

