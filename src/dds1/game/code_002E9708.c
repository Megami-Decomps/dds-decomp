#include "common.h"

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

extern void sdfPrintFormattedDevMessage(const char *fmt, ...);

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

extern u32 D_003BD494;

u32 sndSendCommandPacket(u32 command, u32 value, void *data, u32 size);

u32 func_002E87A8(u32 command, u32 value, void *data, u32 size);
typedef struct SdfStreamTextureHead {
    u8 pad00[0xC];
    s32 resourceWord;
} SdfStreamTextureHead;

typedef struct SoundNode {
    u8 pad00[8];
    struct SoundNode *next;
    u8 active;
    u8 pad0D[7];
    u8 audioMode; /* 0=no audio, 1=mono, 2=stereo */
    u8 loopMode;
    u8 playbackMode;
    u8 pad17[5];
    s32 bufferSize;
    u32 buffers[2];
    u8 pad28[0xC];
    s32 userValue;
    SdfStreamTextureHead *textureHead;
    u16 width;
    u16 height;
    s32 frameHeaderWord;
    u8 pad44[0xC];
    u8 headerReady;
    u8 done;
    u8 count;
    u8 start;
    u32 sampleBuffer;
    u8 pad58[4];
    s32 (*read)(struct SoundNode *, u32, s32, void *, s32);
    u32 source;
    u8 pad64[0x28];
} SoundNode;
typedef s32 (*SdfStreamRead)(SoundNode *, u32, s32, void *, s32);

typedef struct SoundFormat {
    u8 hasAudio;
    u8 stereo;
    u8 loopMode;
    u8 playbackMode;
} SoundFormat;

extern SoundNode *sdfSoundNodeHead;
extern s32 sceIpuSync(s32, s32);
extern u32 sdfAllocateBlockBySizeThreshold(s32);
extern void sdfSoundInitNodeFromFormat(SoundNode *, SoundFormat *);
extern void sdfStreamOpen(SoundNode *, SoundFormat *, s32, s32);
extern void sdfSoundInitFormattedNode(SoundNode *, SoundFormat *, SdfStreamRead, u32);
extern s32 D_003BDA9C;
extern s32 D_003BDAA0;
extern void func_002CF7B8(s32);
extern s32 func_0030B5D0(s32);
extern u64 sdfDevReadResourceWithExtraSpace(u64, u32 *, u32 *, s32);

extern void func_002EB578(SoundNode *node, u8 *data, s32 size);

extern s32 sdfCreateConfiguredBufferedResourceList(s32);

extern void sdfAppendResourceListItem(s32, u64);

typedef struct SdfStreamNode {
    struct SdfStreamNode *prev;
    struct SdfStreamNode *next;
    u8 pad8[5];
    u8 queued;
} SdfStreamNode;

extern SdfStreamNode *sdfStreamNodeListHead;

extern SdfStreamNode *sdfStreamNodeListTail;

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

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EA448);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48A0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48B8);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48E0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4900);

INCLUDE_RODATA(const s32, "game/code_002E9708", jtbl_003B4920);

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

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EAEB8);

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
    s32 fixupOffset;
    u32 fixupSize;
    u8 pad18[8];
} PackedRelocationHeader;

s32 sdfRelocatePackedResourcePayload(s32 resource) {
    s32 payload;

    payload = resource + 0x20;
    sdfRelocatePackedResourceWords(payload, payload, payload + ((PackedRelocationHeader *)resource)->fixupOffset, ((PackedRelocationHeader *)resource)->fixupSize);
    return payload;
}

u64 sdfLoadPackedResourceWithRelocatedPayload(u64 name, s32 *out) {
    u32 info[4];
    u64 buffer = sdfReadNamedResource(name, info, 0);
    *out = sdfRelocatePackedResourcePayload(info[0]);
    return buffer;
}

s32 sdfRelocatePackedResourceWordsFromHeader(s32 resource) {
    s32 payload;

    payload = resource + 0x20;
    sdfRelocatePackedResourceWords(payload, payload, payload + ((PackedRelocationHeader *)resource)->fixupOffset, ((PackedRelocationHeader *)resource)->fixupSize);
    return payload;
}

u64 sdfReadPackedResourceAndRelocateHeader(u64 name, s32 *out) {
    u32 info[4];
    u64 buffer = sdfReadNamedResource(name, info, 0);
    *out = sdfRelocatePackedResourceWordsFromHeader(info[0]);
    return buffer;
}

void sdfRelocatePackedResourceWords(s32 *words, s32 base, u8 *table, s32 size) {
    u8 *cursor = table;
    s32 value;
    s32 i;

    while (cursor - table < size) {
        value = *cursor++;
        if ((value & 1) == 0) {
            value >>= 1;
        } else if ((value & 2) == 0) {
            value = (value | *cursor++ << 8) >> 2;
        } else if ((value & 4) == 0) {
            value = (value | cursor[0] << 8 | cursor[1] << 16) >> 3;
            cursor += 2;
        } else {
            value = (value >> 3) + 2;
            for (i = 0; i < value; i++) {
                words++;
                *words += base;
            }
            continue;
        }
        words += value;
        *words += base;
    }
}

void sdfStreamNodeUnlink(SdfStreamNode *node, s32 inInterrupt) {
    s32 interruptsEnabled = 0;
    SdfStreamNode *prev;
    SdfStreamNode *next;
    if (inInterrupt == 0) {
        interruptsEnabled = func_00312C08();
    }
    if (node->queued != 0) {
        prev = node->prev;
        next = node->next;
        if (prev == 0) {
            sdfStreamNodeListHead = next;
        } else {
            prev->next = next;
        }
        if (next == 0) {
            sdfStreamNodeListTail = prev;
        } else {
            next->prev = prev;
        }
        node->queued = 0;
    }
    if (inInterrupt == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfStreamNodeAppend(SdfStreamNode *node, s32 inInterrupt) {
    s32 interruptsEnabled = 0;
    if (inInterrupt == 0) {
        interruptsEnabled = func_00312C08();
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
    if (inInterrupt == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfSoundAppendNode(SoundNode *node) {
    SoundNode **tail = &sdfSoundNodeHead;
    SoundNode *current = *tail;
    if (current != NULL) {
        tail = &current->next;
        while ((current = *tail) != NULL) {
            tail = &current->next;
        }
    }
    *tail = node;
    node->next = NULL;
}

void sdfSoundRemoveNode(SoundNode *node) {
    SoundNode **link = &sdfSoundNodeHead;
    SoundNode *current = *link;
    while (current != NULL) {
        if (current == node) {
            *link = current->next;
            return;
        }
        link = &current->next;
        current = *link;
    }
}

void sdfAllocateStreamFrameBuffers(SoundNode *node) {
    s32 channels = 4;
    s32 size;
    if (node->audioMode != 0) {
        channels = 2;
    }
    size = node->width * node->height;
    size *= channels;
    node->bufferSize = size;
    node->buffers[0] = sdfAllocateBlockBySizeThreshold(size);
    node->buffers[1] = sdfAllocateBlockBySizeThreshold(size);
}
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB578);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB650);

void sdfSoundInitNodeFromFormat(SoundNode *node, SoundFormat *format) {
    memset(node, 0, 0x8C);
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
/* Stream frame header precedes the bytes passed to the frame decoder. */
typedef struct SoundFrameHeader {
    u8 pad00[8];
    u16 width;
    u16 height;
    s32 frameHeaderWord;
} SoundFrameHeader;

void sdfStreamOpen(SoundNode *node, SoundFormat *format, s32 source, s32 sourceSize) {
    s32 interruptsEnabled;
    u8 *src = (u8 *)source;

    sdfSoundInitNodeFromFormat(node, format);
    node->width = ((SoundFrameHeader *)src)->width;
    node->frameHeaderWord = ((SoundFrameHeader *)src)->frameHeaderWord;
    node->height = ((SoundFrameHeader *)src)->height;
    sdfAllocateStreamFrameBuffers(node);
    func_002EB578(node, src + 0x10, sourceSize - 0x10);
    interruptsEnabled = func_00312C08();
    sdfStreamNodeAppend((SdfStreamNode *)node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    func_002EC230(0);
}

void sdfSoundInitFormattedNode(SoundNode *node, SoundFormat *format, SdfStreamRead read, u32 source) {
    sdfSoundInitNodeFromFormat(node, format);
    node->read = read;
    node->source = source;
    node->active = 1;
    node->sampleBuffer = sdfAllocateBlockBySizeThreshold(0x10100) + 0x100;
}

extern SdfStreamTextureHead *sdfTexAllocateHeadForDimensions(s32, s32, s32, s32, s32);

void sdfStreamInitializeFromHeader(SoundNode *node) {
    SoundFrameHeader header;
    s32 status;
    s32 interruptsEnabled;

    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->source, 0, &status, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->source, 1, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->frameHeaderWord = header.frameHeaderWord;
    if (node->userValue == 0) {
        s32 width = node->width;
        s32 height = node->height;
        s32 format;
        SdfStreamTextureHead *texture;

        if (width > 64) {
            width = (width + 63) & ~63;
        }
        if (node->audioMode == 0) {
            format = 0;
            if (height > 32) {
                height = (height + 31) & ~31;
            }
        } else {
            format = 2;
            if (height > 64) {
                height = (height + 63) & ~63;
            }
        }
        texture = sdfTexAllocateHeadForDimensions(width, height, format, 2, 0);
        node->textureHead = texture;
        node->userValue = texture->resourceWord;
    }
    sdfAllocateStreamFrameBuffers(node);
    interruptsEnabled = func_00312C08();
    sdfStreamNodeAppend((SdfStreamNode *)node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EBB60);

extern void *memcpy(void *dst, const void *src, u32 n);

void sndFillStreamFeedRing(SoundNode *feed) {
    s32 slot;
    s32 count;
    s32 got;
    u32 dst;
    s32 eof;

    count = feed->count;
    if (feed->done == 0 && count < 7) {
        do {
            slot = feed->start;
            eof = 0;
            dst = feed->sampleBuffer;
            slot += count;
            if (slot >= 8) {
                slot -= 8;
            }
            dst += slot << 13;
            dst = (dst & 0x0FFFFFFF) | 0x20000000;
            got = feed->read(feed, feed->source, 0, &eof, 0);
            if (got < 0x2000) {
                if (eof == 0) {
                    return;
                }
                if (got > 0) {
                    feed->read(feed, feed->source, 1, (void *)dst, got);
                    count++;
                }
                feed->done = 1;
            } else {
                if (got == 0x2000) {
                    if (eof != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->source, 1, (void *)dst, 0x2000);
                count++;
            }
            if (slot == 7) {
                memcpy((void *)(((feed->sampleBuffer - 0x100) & 0x0FFFFFFF) | 0x20000000), (void *)(dst + 0x1F00), 0x100);
            }
            feed->count = count;
            if (feed->done != 0) {
                break;
            }
        } while (count < 7);
    }
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EBEB8);

typedef struct SoundIpuBuffer {
    u8 pad00[0x18];
    u8 active;
    u8 pad19[3];
    s32 size;
    u32 buffers[2];
} SoundIpuBuffer;

void sdfSoundQueueIpuBuffer(SoundIpuBuffer *stream) {
    vu32 *ipuData = (vu32 *)0x1000B010;
    vu32 *ipuSize = (vu32 *)0x1000B020;
    vu32 *ipuControl = (vu32 *)0x1000B000;
    u8 active = stream->active;
    *ipuData = stream->buffers[active] & 0x0fffffff;
    *ipuSize = stream->size / 16;
    *ipuControl = 0x100;
    stream->active = active ^ 1;
}

s32 sdfSoundSyncIpu(void) {
    vu32 *ipuCommand = (vu32 *)0x10002000;
    s32 status;
    *ipuCommand = 0x40000000;
    sceIpuSync(0, 0);
    status = *ipuCommand;
    sceIpuSync(0, 0);
    *ipuCommand = 0x40000008;
    sceIpuSync(0, 0);
    return status;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC060);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC230);

extern u32 D_003BDAA4;
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
        work = (IpuWorker *)D_003BDAA4;
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
            if (work->unk15 != 0) {
                work->unk0E = 0;
                work->tickCount = 0;
                work->unk65 = 0;
            } else {
                work->drained = 1;
            }
        }
        if (work->drained == 0) {
            interruptsEnabled = func_00312C08();
            sdfStreamNodeAppend((SdfStreamNode *)work, 0);
            if (interruptsEnabled != 0) {
                EIntr();
            }
        }
        func_002EC230(0);
    }
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC3C0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC3F0);

u32 sdfMidiPreviousEntry(MidiChannel *channel) {
    u32 result = 0;
    if (channel->enabled != 0) {
        result = channel->earlierEntries[channel->index];
    }
    return result;
}

u32 sndGetSelectedChannelEntry(MidiChannel *channel) {
    u32 result = 0;
    if (channel->enabled != 0) {
        result = channel->entries[channel->index];
    }
    return result;
}

void sdfAdvanceBufferedPlayback(MidiPlaybackState *state) {
    s32 interruptsEnabled = func_00312C08();
    s32 pending = state->pending;
    s32 remaining = pending - 1;
    if (pending > 0) {
        state->pending = remaining;
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
    sdfTexEnqueuePacketWithSemaphore(*selectedBuffer, *selectedBuffer + state->bufferSize - 0x10);
    sdfAdvanceBufferedPlayback(state);
    return 1;
}
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC5E0);

void sdfSoundInitAndAppendNode(SoundNode *node, s32 format, s32 source, s32 sourceSize, s32 value) {
    sdfStreamOpen(node, format, source, sourceSize);
    node->userValue = value;
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

void sdfSoundInitFormattedAndAppendNode(SoundNode *node, SoundFormat *format, SdfStreamRead read, u32 source, s32 value) {
    sdfSoundInitFormattedNode(node, format, read, source);
    node->userValue = value;
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

s32 func_002EC900(void) {
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

