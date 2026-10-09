#include "common.h"
#include "ee_mmi.h"
#include "sdf_resource.h"
#include "sdf_packed_resource.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_stream_read.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"

#define SDF_STREAM_NODE_BYTES 0x8C
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

extern DevRequest *sndBuildResourceHandleListFromOffsets(const void *);
extern SdfTex *sdfTexAcquireResourceTexture(void *);


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
extern u32 D_003FEAC0[16];

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
extern s32 func_00312C08(void);
extern void sdfDispatchNextStreamNode(s32);
extern s32 sdfStartStreamNodeIpuTransfer(SdfStreamFrameNode *);

typedef void *(*SdfSoundReadCallback)(s32);
typedef union SdfSoundCommand {
    u32 word;
    SdfSoundReadCallback read;
} SdfSoundCommand;

extern SdfSoundCommand D_003BD494;

u32 sndSendCommandPacket(u32 command, u32 value, void *data, u32 size);

u32 func_002E87A8(u32 command, u32 value, void *data, u32 size);


extern SdfStreamFrameNode *sdfSoundNodeHead;
extern SdfStreamFrameNode *D_003BDAA8;
extern s32 D_003BDA94;
extern s32 D_003BDAB4;
extern s32 iWakeupThread(s32 threadId);
extern s32 sceIpuSync(s32, s32);
extern void *sdfAllocateBlockBySizeThreshold(s32);
extern void sdfStreamOpen(SdfStreamFrameNode *, SoundFormat *, s32, s32);
extern void sdfSoundInitFormattedNode(SdfStreamFrameNode *, SoundFormat *, SdfStreamRead, u32);
extern s32 D_003BDA9C;
extern s32 D_003BDAA0;
extern void func_002CF7B8(s32);
extern s32 func_0030B5D0(s32);

extern DevRequest *sdfCreateConfiguredBufferedResourceList(u32);

extern void sdfAppendResourceListItem(DevRequest *, u32);

extern SdfStreamFrameNode *sdfStreamNodeListHead;

extern SdfStreamFrameNode *sdfStreamNodeListTail;

extern void sdfTexEnqueuePacketWithSemaphore(s32 address, void *packet);

extern void sdfBuildStreamFrameTransferPackets(SdfStreamFrameNode *node);

extern u32 sdfTexGetPrimaryResourceWord(SdfTex *texture);

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
    D_003BD494.word = command;
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
    case SDF_DEV_EVENT_SEEK_REPLY:
        break;
    case SDF_DEV_EVENT_WRITE_REPLY:
        break;
    case SDF_DEV_EVENT_READ_REPLY:
        FlushCache(0);
        /* Fall through: flush and wake the waiting thread. */
    case SDF_DEV_EVENT_SIZE_REPLY:
        SignalSema(sdfSoundRpcSemaphore);
        break;
    case SDF_DEV_EVENT_INACTIVE:
    case SDF_DEV_EVENT_OPENED:
    case SDF_DEV_EVENT_CLOSED:
        SignalSema(sdfSoundRpcSemaphore);
        break;
    }
    return 0;
}

typedef struct SdfSoundRpcRequest {
    u8 pad00[8];
    s32 destination;
    s32 byteCount;
} SdfSoundRpcRequest;

typedef struct SdfSoundResidentBuffer {
    u8 *data;
    u8 pad04[0x24];
} SdfSoundResidentBuffer;

extern SdfSoundRpcRequest *D_003BDA74;
extern s32 D_003BDA78;
extern DevState *D_003BDA7C;
extern SdfSoundResidentBuffer D_003FEA98;
extern char *mnuBuildVoiceResourcePath(char *, char *);
extern DevState *sdfDevCreateCallbackState(const char *, void *, s32);
extern s32 sdfDevQueueActiveOperation(DevState *);
extern void func_002E8938(s32, void *, s32);

u32 *func_002E99A0(u32 command, SdfSoundRpcRequest *request) {
    char path[0x100];

    switch (command) {
    case 0: {
        DevState *state;
        if (D_00398948[0] == 0) {
            D_003FEAC0[0] = 0;
            break;
        }
        sdfSoundCommandBusy = 1;
        mnuBuildVoiceResourcePath(path, D_00398948);
        state = sdfDevCreateCallbackState(path, sdfSoundHandleRpcEvent, 0);
        D_003BDA7C = state;
        if (state != NULL) {
            WaitSema(sdfSoundRpcSemaphore);
            state = D_003BDA7C;
            if (state->result != 0) {
                sdfDevReactivate(state);
                sdfDevQueueReleaseState(D_003BDA7C);
                D_003BDA7C = NULL;
                state = NULL;
            }
        }
        D_003FEAC0[0] = (u32)state;
        break;
    }
    case 1: {
        SdfMemBlock *allocation;
        void *buffer;
        D_003BDA74 = request;
        allocation = sdfAllocGeneralBlock(request->byteCount);
        buffer = (void *)sdfResourceRetainAddress(allocation);
        if (buffer != NULL) {
            sdfDevQueueRead(D_003BDA7C, buffer, D_003BDA74->byteCount);
            WaitSema(sdfSoundRpcSemaphore);
            func_002E8938(D_003BDA74->destination, buffer, D_003BDA74->byteCount);
            sdfReleaseResourceAllocation(allocation);
            D_003FEAC0[0] = D_003BDA74->byteCount;
        } else {
            D_003FEAC0[0] = 0;
        }
        break;
    }
    case 2:
        sdfDevQueueActiveOperation(D_003BDA7C);
        WaitSema(sdfSoundRpcSemaphore);
        sdfDevQueueReleaseState(D_003BDA7C);
        D_003BDA7C = NULL;
        sdfSoundCommandBusy = 0;
        D_003FEAC0[0] = 0;
        break;
    case 3:
        D_003BDA78 = 0;
        D_003FEAC0[0] = 0;
        break;
    case 4: {
        s32 bytes;
        D_003BDA74 = request;
        func_002E8938(request->destination, D_003FEA98.data + D_003BDA78, request->byteCount);
        bytes = D_003BDA74->byteCount;
        D_003FEAC0[0] = bytes;
        D_003BDA78 += bytes;
        break;
    }
    case 5:
        D_003FEAC0[0] = 0;
        break;
    case 6: {
        u32 word = D_003BD494.word;
        if (word != 0) {
            sdfSoundCommandStatus = 1;
        }
        D_003FEAC0[0] = word;
        break;
    }
    case 7: {
        void *buffer;
        D_003BDA74 = request;
        buffer = D_003BD494.read(request->byteCount);
        if (buffer != NULL) {
            func_002E8938(D_003BDA74->destination, buffer, D_003BDA74->byteCount);
            D_003FEAC0[0] = D_003BDA74->byteCount;
        } else {
            D_003FEAC0[0] = -1;
        }
        break;
    }
    case 8:
        sdfSoundCommandStatus = 0;
        D_003FEAC0[0] = 0;
        break;
    }
    return D_003FEAC0;
}

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


extern char sdfGsMemoryDumpHeader[]; /* " <<< GS memory information >>>..." */
extern char sdfGsMemoryDumpRowFormat[]; /* " %08X : %08X %08X %8s %08X %d\n" */
extern char sdfGsMemoryTypeFormat[]; /* "%d" */
extern char *D_00398A28[];
extern SdfTexResource *sdfGetTextureListHead(void);
extern char *D_00398A38[];
extern SdfTexResource *sdfGetTextureBlockListHead(void);

void sdfDumpGsMemoryForward(void) {
    char buf[8];
    SdfTexResource *head;
    SdfTexResource *node;
    SdfTexResource *walk;
    char *name;

    sdfPrintFormattedDevMessage(sdfGsMemoryDumpHeader);
    head = sdfGetTextureListHead();
    node = head;
    while (node != 0) {
        if (node->allocationMode < 4) {
            name = D_00398A28[node->allocationMode];
        } else {
            sdfPrintFormattedDevMessage(buf, sdfGsMemoryTypeFormat, node->allocationMode);
            name = buf;
        }
        sdfPrintFormattedDevMessage(sdfGsMemoryDumpRowFormat, node, node->next, node->prev, name, node->word, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->prev;
        }
        node = node->prev;
    }
}

void sdfDumpGsMemoryBackward(void) {
    char buf[8];
    SdfTexResource *head;
    SdfTexResource *node;
    SdfTexResource *walk;
    char *name;

    sdfPrintFormattedDevMessage(sdfGsMemoryDumpHeader);
    head = sdfGetTextureBlockListHead();
    node = head;
    while (node != 0) {
        if (node->allocationMode < 4) {
            name = D_00398A38[node->allocationMode];
        } else {
            sdfPrintFormattedDevMessage(buf, sdfGsMemoryTypeFormat, node->allocationMode);
            name = buf;
        }
        sdfPrintFormattedDevMessage(sdfGsMemoryDumpRowFormat, node, node->next, node->prev, name, node->word, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->next;
        }
        node = node->next;
    }
}

INCLUDE_RODATA(const s32, "game/code_002E9708", sdfGsMemoryDumpHeader);

INCLUDE_RODATA(const s32, "game/code_002E9708", sdfGsMemoryDumpRowFormat);

void sndPrintMemoryInfo(void) {
    s32 info[6];
    sdfGetGeneralHeapStats(info);
    sdfPrintFormattedDevMessage(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info[SDF_HEAP_STAT_TOTAL_BYTES], info[SDF_HEAP_STAT_FREE_BYTES],
                    info[SDF_HEAP_STAT_LARGEST_FREE], info[SDF_HEAP_STAT_SMALLEST_FREE],
                    info[SDF_HEAP_STAT_BLOCK_COUNT], info[SDF_HEAP_STAT_FREE_BLOCK_COUNT]);
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


/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
SdfMemBlock *sdfDevReadResourceWithExtraSpace(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    DevState *state = sdfDevCreateCommandState(name);
    s32 size = sdfDevQueueControlAndWait(state);
    SdfMemBlock *handle = sdfAllocGeneralBlock(size + extra);
    u32 address = sdfResourceRetainAddress(handle);

    sdfDevQueueReadAndWait(state, (void *)address, size);
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

SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize) {
    return sdfDevReadResourceWithExtraSpace(name, outAddress, outSize, 0);
}

SdfTex *sdfLoadNamedResourceAndReleaseLookupHandle(const char *name) {
    SdfMemBlock *handle;
    SdfTex *resource;
    u32 info[4];

    handle = sdfReadNamedResource(name, info, 0);
    resource = sdfTexAcquireResourceTexture((void *)info[0]);
    sdfReleaseResourceAllocation(handle);
    return resource;
}

/* Serialized texture-resource count and offsets relative to the blob's base. */
typedef struct SoundResourceList {
    u8 pad00[0x10];
    s32 count;
    s32 relativeOffsets[1];
} SoundResourceList;

DevRequest *sndBuildResourceHandleListFromOffsets(const void *resource) {
    const SoundResourceList *list = resource;
    s32 i = 0;
    s32 count = list->count;
    DevRequest *handle = sdfCreateConfiguredBufferedResourceList(count);
    const s32 *entry;
    if (count != i) {
        entry = list->relativeOffsets;
        do {
            i++;
            sdfAppendResourceListItem(handle, (u32)sdfTexAcquireResourceTexture((u8 *)resource + *entry));
            entry++;
        } while (i != count);
    }
    return handle;
}

DevRequest *sndLoadNamedOffsetResourceList(const char *name) {
    SdfMemBlock *handle;
    DevRequest *resource;
    u32 info[4];

    handle = sdfReadNamedResource(name, info, 0);
    resource = sndBuildResourceHandleListFromOffsets((const void *)info[0]);
    sdfReleaseResourceAllocation(handle);
    return resource;
}

/* Relocate the payload following the packed header and return its address. */
void *sdfRelocatePackedResourcePayload(SdfPackedRelocationHeader *resource) {
    u8 *payload = (u8 *)resource + SDF_PACKED_RESOURCE_HEADER_BYTES;

    sdfRelocatePackedResourceWords((s32 *)payload, (s32)payload,
                                   payload + resource->relocationOffset, resource->relocationByteCount);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
SdfMemBlock *sdfLoadPackedResourceWithRelocatedPayload(const char *name, s32 *outPayload) {
    u32 info[4];
    SdfMemBlock *buffer = sdfReadNamedResource(name, info, 0);
    *outPayload = (s32)sdfRelocatePackedResourcePayload((SdfPackedRelocationHeader *)(u32)info[0]);
    return buffer;
}

/* This entry point uses the same payload-relative relocation table. */
void *sdfRelocatePackedResourceWordsFromHeader(SdfPackedRelocationHeader *resource) {
    u8 *payload = (u8 *)resource + SDF_PACKED_RESOURCE_HEADER_BYTES;

    sdfRelocatePackedResourceWords((s32 *)payload, (s32)payload,
                                   payload + resource->relocationOffset, resource->relocationByteCount);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
SdfMemBlock *sdfReadPackedResourceAndRelocateHeader(const char *name, s32 *outPayload) {
    u32 info[4];
    SdfMemBlock *buffer = sdfReadNamedResource(name, info, 0);
    *outPayload = (s32)sdfRelocatePackedResourceWordsFromHeader((SdfPackedRelocationHeader *)(u32)info[0]);
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
    if (node->audioMode != SDF_STREAM_AUDIO_DISABLED) {
        bytesPerPixel = SDF_STREAM_PIXEL_BYTES_NARROW;
    }
    size = node->width * node->height;
    size *= bytesPerPixel;
    node->bufferSize = size;
    node->frameBuffers[0] = sdfAllocateBlockBySizeThreshold(size);
    node->frameBuffers[1] = sdfAllocateBlockBySizeThreshold(size);
}
INCLUDE_ASM(const s32, "game/code_002E9708", sdfBuildStreamInputDmaChain);

INCLUDE_ASM(const s32, "game/code_002E9708", sdfBuildStreamFrameTransferPackets);

/* Clear the node, map the format's two selector bytes to mode 0/1/2, and copy loop/playback modes. */
void sdfSoundInitNodeFromFormat(SdfStreamFrameNode *node, SoundFormat *format) {
    memset(node, 0, SDF_STREAM_NODE_BYTES);
    if (format->hasAudio == 0) {
        node->audioMode = SDF_STREAM_AUDIO_DISABLED;
    } else if (format->stereo == 0) {
        node->audioMode = SDF_STREAM_AUDIO_MONO;
    } else {
        node->audioMode = SDF_STREAM_AUDIO_STEREO;
    }
    node->loopMode = format->loopMode;
    node->playbackMode = format->playbackMode;
}
/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfStreamOpen(SdfStreamFrameNode *node, SoundFormat *format, s32 sourceAddress, s32 sourceSize) {
    s32 interruptsEnabled;
    u8 *frameBytes = (u8 *)sourceAddress;

    sdfSoundInitNodeFromFormat(node, format);
    node->width = ((SdfStreamFrameHeader *)frameBytes)->width;
    node->cycleLength = ((SdfStreamFrameHeader *)frameBytes)->cycleLength;
    node->height = ((SdfStreamFrameHeader *)frameBytes)->height;
    sdfAllocateStreamFrameBuffers(node);
    sdfBuildStreamInputDmaChain(node, frameBytes + SDF_STREAM_FRAME_HEADER_BYTES, sourceSize - SDF_STREAM_FRAME_HEADER_BYTES);
    interruptsEnabled = func_00312C08();
    sdfStreamNodeAppend(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfDispatchNextStreamNode(0);
}

/* Store the callback/source token and allocate an eight-slot feed ring plus its leading mirror bytes. */
void sdfSoundInitFormattedNode(SdfStreamFrameNode *node, SoundFormat *format, SdfStreamRead readSource, u32 source) {
    sdfSoundInitNodeFromFormat(node, format);
    node->read = readSource;
    node->source = source;
    node->active = 1;
    node->scratchBuffer = (u8 *)sdfAllocateBlockBySizeThreshold(SDF_STREAM_SCRATCH_BYTES) + SDF_STREAM_PREFIX_BYTES;
}

extern SdfTexResource *sdfTexAllocateHeadForDimensions(s32, s32, s32, s32, s32);

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfStreamInitializeFromHeader(SdfStreamFrameNode *node) {
    SdfStreamFrameHeader header;
    s32 readStatus;
    s32 interruptsEnabled;

    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->source, SDF_STREAM_READ_QUERY, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->source, SDF_STREAM_READ_COPY, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->cycleLength = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        SdfTexResource *texture;

        if (textureWidth > SDF_STREAM_PAGE_WIDTH) {
            textureWidth = (textureWidth + SDF_STREAM_PAGE_WIDTH_MASK) & ~SDF_STREAM_PAGE_WIDTH_MASK;
        }
        if (node->audioMode == SDF_STREAM_AUDIO_DISABLED) {
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
        node->resourceWord = texture->word;
    }
    sdfAllocateStreamFrameBuffers(node);
    interruptsEnabled = func_00312C08();
    sdfStreamNodeAppend(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

extern SdfStreamFrameNode *D_003BDAA4;
extern void sdfFreeMemoryFromEitherHeap(void *);
extern void sdfTexQueueResourceRelease(s32);
extern void sdfTexQueuePendingWork(SdfTexResource *texture);

void sdfDestroyStreamFrameNode(SdfStreamFrameNode *node) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;

    interruptsEnabled = func_00312C08();
    wasActive = 0;
    node->drained = 1;
    sdfSoundRemoveNode(node);
    sdfStreamNodeUnlink(node, 1);
    if (D_003BDAA4 == node) {
        *(volatile u32 *)0x10002010 = 0x40000000;
        sceIpuSync(0, 0);
        dmaEnable = *(volatile u32 *)0x1000F520;
        *(volatile u32 *)0x1000F520 = 0x10000;
        *(volatile u32 *)0x1000B400 = 1;
        *(volatile u32 *)0x1000B000 = 0;
        *(volatile u32 *)0x1000F520 = dmaEnable;
        D_003BDAA4 = NULL;
        wasActive = 1;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfFreeMemoryFromEitherHeap(node->inputDmaChain);
    sdfFreeMemoryFromEitherHeap(node->frameBuffers[0]);
    sdfFreeMemoryFromEitherHeap(node->frameBuffers[1]);
    sdfTexQueueResourceRelease((s32)node->transferPacketBuffers[0]);
    sdfTexQueueResourceRelease((s32)node->transferPacketBuffers[1]);
    if (node->scratchBuffer != 0) {
        sdfFreeMemoryFromEitherHeap(node->scratchBuffer - 0x100);
    }
    if (node->textureHead != 0) {
        sdfTexQueuePendingWork(node->textureHead);
    }
    if (wasActive != 0) {
        sdfDispatchNextStreamNode(0);
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
            destinationAddress = (u32)feed->scratchBuffer;
            writeSlot += filledSlots;
            if (writeSlot >= SDF_STREAM_RING_SLOTS) {
                writeSlot -= SDF_STREAM_RING_SLOTS;
            }
            destinationAddress += writeSlot << SDF_STREAM_SLOT_SHIFT;
            destinationAddress = (destinationAddress & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE;
            availableBytes = feed->read(feed, feed->source, SDF_STREAM_READ_QUERY, &endOfStream, 0);
            if (availableBytes < SDF_STREAM_SLOT_BYTES) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->source, SDF_STREAM_READ_COPY, (void *)destinationAddress, availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == SDF_STREAM_SLOT_BYTES) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->source, SDF_STREAM_READ_COPY, (void *)destinationAddress, SDF_STREAM_SLOT_BYTES);
                filledSlots++;
            }
            if (writeSlot == SDF_STREAM_MAX_FILLED) {
                memcpy((void *)(((u32)(feed->scratchBuffer - SDF_STREAM_PREFIX_BYTES) & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE), (void *)(destinationAddress + SDF_STREAM_TRAILER_START), SDF_STREAM_PREFIX_BYTES);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < SDF_STREAM_MAX_FILLED);
    }
}

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfSoundStartIpuInputDma(SdfStreamFrameNode *stream) {
    if (stream->active == 0) {
        D_003BDAA8 = stream;
        *(vu32 *)SDF_IPU_INPUT_DMA_TAG_ADDRESS = (u32)stream->inputDmaChain & SDF_EE_PHYSICAL_MASK;
        *(vu32 *)SDF_IPU_INPUT_DMA_QWC = 0;
        *(vu32 *)SDF_IPU_INPUT_DMA_CTRL = SDF_IPU_DMA_CHAIN_START;
    } else {
        if (stream->filledSlots == 0) {
            stream->inputDmaStartPending = 1;
            return;
        }
        D_003BDAA8 = stream;
        stream->inputFeedDmaInFlight = 1;
        *(vu32 *)SDF_IPU_INPUT_DMA_ADDRESS = ((u32)stream->scratchBuffer +
            (stream->firstSlot << SDF_STREAM_SLOT_SHIFT)) & SDF_EE_PHYSICAL_MASK;
        *(vu32 *)SDF_IPU_INPUT_DMA_QWC = SDF_STREAM_SLOT_BYTES / SDF_STREAM_QWORD_BYTES;
        *(vu32 *)SDF_IPU_INPUT_DMA_CTRL = SDF_IPU_DMA_NORMAL_START;
    }
    stream->inputDmaStartPending = 0;
}

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfSoundQueueIpuBuffer(SdfStreamFrameNode *stream) {
    vu32 *dmaAddress = (vu32 *)SDF_IPU_OUTPUT_DMA_ADDRESS;
    vu32 *dmaQwords = (vu32 *)SDF_IPU_OUTPUT_DMA_QWC;
    vu32 *dmaControl = (vu32 *)SDF_IPU_OUTPUT_DMA_CTRL;
    u8 bufferIndex = stream->bufferIndex;
    *dmaAddress = (u32)stream->frameBuffers[bufferIndex] & SDF_EE_PHYSICAL_MASK;
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

INCLUDE_ASM(const s32, "game/code_002E9708", sdfStartStreamNodeIpuTransfer);

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfDispatchNextStreamNode(s32 skipInterruptGuard) {
    SdfStreamFrameNode *node;
    s32 interruptsEnabled = 0;

    if (D_003BDAA4 != NULL) {
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
            sdfStartStreamNodeIpuTransfer(node);
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
        work = D_003BDAA4;
        if (work == NULL) {
            continue;
        }
        sceIpuStopDMA(&work->dma);
        if (work->dma.inputQwords == 0) {
            work->unk11 = 0;
        }
        if (work->playbackPhase == SDF_STREAM_PLAYBACK_INITIAL) {
            work->playbackPhase = SDF_STREAM_PLAYBACK_FIRST_COMPLETION;
        }
        work->unk1A++;
        D_003BDAA4 = NULL;
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
        sdfDispatchNextStreamNode(0);
    }
}

s32 sdfWakeIpuCompletionWorker(void) {
    iWakeupThread(D_003BDAB4);
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

s32 sdfCompleteIpuInputFeedDma(void) {
    SdfStreamFrameNode *stream = D_003BDAA8;

    if (stream != NULL) {
        D_003BDA94 = stream->unk48;
        D_003BDAA8 = NULL;
        if (stream->unk11 != 0 && stream->active == 1) {
            if (stream->inputFeedDmaInFlight != 0) {
                stream->inputFeedDmaInFlight = 0;
                stream->firstSlot++;
                if (stream->firstSlot == SDF_STREAM_RING_SLOTS) {
                    stream->firstSlot = 0;
                }
                stream->filledSlots--;
            }
            sndFillStreamFeedRing(stream);
            sdfSoundStartIpuInputDma(stream);
        }
    }
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

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
void sdfAdvanceBufferedPlayback(SdfStreamFrameNode *node) {
    s32 interruptsEnabled = func_00312C08();
    s32 pendingBuffers = node->unk1A;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        node->unk1A = remainingBuffers;
        node->transferPacketIndex ^= 1;
        node->unk13++;
    }
    node->unk48++;
    if (node->unk48 == node->cycleLength && node->loopMode != 0) {
        node->unk48 = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfDispatchNextStreamNode(0);
}

/* Return zero only when nothing is pending; otherwise call the zero-buffer handler if needed, queue and advance. */
s32 sdfSubmitBufferedPlayback(SdfStreamFrameNode *node) {
    u32 *selectedBuffer;
    if (node->unk1A == 0) {
        return 0;
    }
    selectedBuffer = (u32 *)(node->transferPacketIndex * 4 + (s32)node + 0x28);
    if (*selectedBuffer == 0) {
        sdfBuildStreamFrameTransferPackets(node);
    }
    sdfTexEnqueuePacketWithSemaphore(*selectedBuffer, (void *)(*selectedBuffer + node->transferPacketBytes - SDF_STREAM_QWORD_BYTES));
    sdfAdvanceBufferedPlayback(node);
    return 1;
}
extern u32 D_003BD614;

/* Advance playback cadence, refill active feeds and retry a deferred IPU input. */
void sdfAdvanceStreamPlayback(s32 cadence) {
    SdfStreamFrameNode *node = D_003BDAA4;
    s32 elapsed;
    s32 interruptsEnabled;

    if (node != NULL && node->inputDmaStartPending != 0) {
        sdfSoundStartIpuInputDma(node);
    }
    node = sdfSoundNodeHead;
    while (node != NULL) {
        if (node->unk13 != 0) {
            node->unk13--;
        }
        elapsed = node->playbackCadenceRemainder;
        /* The playback provider views the same 0x8C stream allocation. */
        switch (node->playbackPhase) {
        case SDF_STREAM_PLAYBACK_FIRST_COMPLETION:
            sdfSubmitBufferedPlayback(node);
            node->playbackPhase = SDF_STREAM_PLAYBACK_CADENCED;
            node->playbackCadenceRemainder = node->playbackMode;
            break;
        case SDF_STREAM_PLAYBACK_CADENCED:
            if (D_003BD614 != 1) {
                elapsed += node->playbackMode;
                if (elapsed >= cadence) {
                    if (sdfSubmitBufferedPlayback(node) != 0) {
                        elapsed -= cadence;
                    }
                }
                node->playbackCadenceRemainder = elapsed;
            }
            break;
        }
        if (node->active == 1) {
            sdfStreamInitializeFromHeader(node);
            if (D_003BD614 != 1) {
                if (node->filledSlots < SDF_STREAM_RING_SLOTS) {
                    interruptsEnabled = func_00312C08();
                    sndFillStreamFeedRing(node);
                    if (interruptsEnabled != 0) {
                        EIntr();
                    }
                }
                node->read(node, node->source, SDF_STREAM_READ_RESUME, NULL, 0);
            }
        }
        node = node->next;
    }
    sdfDispatchNextStreamNode(0);
}

void sdfSoundInitAndAppendNode(SdfStreamFrameNode *node, SoundFormat *format, s32 source, s32 sourceSize, s32 value) {
    sdfStreamOpen(node, format, source, sourceSize);
    node->resourceWord = value;
    sdfSoundAppendNode(node);
}

void sdfStreamCreateWithParams(SdfStreamFrameNode *node, SdfStreamParams *params, s32 sourceData, s32 sourceSize, SdfTex *source) {
    SdfStreamParams local = *params;
    switch (source->pixelFormat) {
    case 0:
        local.hasAudio = 0;
        break;
    case 2:
        local.hasAudio = 1;
        break;
    }
    sdfSoundInitAndAppendNode(node, &local, sourceData, sourceSize, sdfTexGetPrimaryResourceWord(source));
}

void sdfSoundInitFormattedAndAppendNode(SdfStreamFrameNode *node, SoundFormat *format, SdfStreamRead read, u32 source, s32 value) {
    sdfSoundInitFormattedNode(node, format, read, source);
    node->resourceWord = value;
    sdfSoundAppendNode(node);
}

extern void sceIpuInit(void);
extern s32 sdfAddHandler(s32, s32, void *, s32, s32);
extern void func_0030B638(s32);
extern s32 sdfCreateThread(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
extern void _StartThread();
extern u8 sdfIpuStreamThreadStack[];
extern void sdfIpuDmaCompletionWorker();
void sdfSoundInitIpuStream(void) {
    s32 thread;

    sceIpuInit();
    *(volatile s32 *)0x10002000 = 0x90000000;
    sdfSoundNodeHead = 0;
    D_003BDAA4 = NULL;
    D_003BDA9C = sdfAddHandler(1, 3, sdfWakeIpuCompletionWorker, -1, 0);
    func_0030B638(3);
    D_003BDAA0 = sdfAddHandler(1, 4, sdfCompleteIpuInputFeedDma, -1, 0);
    func_0030B638(4);
    thread = sdfCreateThread((void *)sdfIpuDmaCompletionWorker, sdfIpuStreamThreadStack, 0x800, 0x46);
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

