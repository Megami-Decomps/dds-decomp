#include "common.h"

extern u32 D_00438B88;

extern u32 D_00438B8C;

extern u64 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

extern u64 func_00343F38(u32);

extern u32 D_00438D0C;

extern s32 D_00438D10;

extern s32 D_00438D14;

extern s32 D_00438D18;

extern s32 D_00438D1C;

extern u32 D_00438D20;

u32 func_00341650(u32 arg0, u32 arg1, void *arg2, u32 arg3);

u32 func_003417A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

extern u32 D_00438B84;

extern void func_00329A00(void *out);

extern void func_0033DAD8(char *fmt, ...);

typedef struct MidiChannel {
    u8 pad00[0x19];
    u8 index;
    u8 enabled;
    u8 pad1B[5];
    u32 earlierEntries[2];
    u32 entries[8];
} MidiChannel;

extern char D_0040BAF8[];

extern s32 D_004391F0;

extern s32 sdfCreateSemaphore(s32, s32, s32);

extern s32 GetThreadId(void);

extern void sceSifSetRpcQueue(void *, s32);

extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);

extern void sceSifRpcLoop(void *);

extern u8 D_0047B440[];

extern s32 func_00342848();

typedef struct SoundNode {
    u8 pad00[8];
    struct SoundNode *next;
} SoundNode;

extern SoundNode *D_004391F8;

typedef struct SdfStreamNode {
    struct SdfStreamNode *prev;
    struct SdfStreamNode *next;
    u8 pad8[5];
    u8 queued;
} SdfStreamNode;

extern SdfStreamNode *D_0043920C;
extern SdfStreamNode *D_00439210;

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
    u32 relocationCount;
    u8 payload[1];
} SdfRelocResource;

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
    if (D_00438B88 != 0) {
        return 1;
    }
    strcpy(D_0040BAF8, name);
    func_003417A8((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 sdfSoundIsCommandBusy(void) {
    return D_00438B88;
}

void func_00342690(void) {
    func_003417A8(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_003426B8);

void sdfSoundSetChannelCount(u32 channels) {
    if (0x10 < channels) {
        channels = 0x10;
    }
    if (channels == 0) {
        channels = 1;
    }
    func_003417A8((channels - 1) | 0x1d0, 0, 0, 0);
}

u32 sdfSoundTryQueueCommand(u32 command) {
    if (D_00438B8C != 0) {
        return 0;
    }
    D_00438B84 = command;
    return command;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342748);

u32 sdfSoundGetCommandStatus(void) {
    return D_00438B8C;
}

void func_00342798(void) {
    func_003417A8(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_003427C0);

extern void FlushCache(s32);
extern s32 SignalSema(s32);

s32 sdfSoundHandleRpcEvent(s32 unused, u32 event) {
    switch (event) {
    case 5:
        FlushCache(0);
    case 4:
        SignalSema(D_004391F0);
        break;
    case 0:
    case 2:
    case 7:
        SignalSema(D_004391F0);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342848);

void sdfSoundStartRpcServer(void) {
    u8 queue[0x20];
    u8 server[0x50];
    s32 semaphore = sdfCreateSemaphore(0, 1, 0);
    D_004391F0 = semaphore;
    if (semaphore <= 0) {
        for (;;) {
        }
    }
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x54524E53, func_00342848, D_0047B440, 0, 0, queue);
    sceSifRpcLoop(queue);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342B28);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342E58);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343188);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003432F0);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E5B0);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E5C8);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E5F0);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042E610);

INCLUDE_RODATA(const s32, "game/code_003425B0", jtbl_0042E630);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343468);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343B40);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343C30);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042EAC8);

INCLUDE_RODATA(const s32, "game/code_003425B0", D_0042EB40);

void sndPrintMemoryInfo(void) {
    s32 info[6];
    func_00329A00(info);
    func_0033DAD8(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info[0], info[1], info[2], info[3], info[4], info[5]);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343D60);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343E18);

extern u64 func_00343E18(u64, u32 *, u64, u64);

u64 func_00343ED0(u64 arg0, u32 *info, u64 arg2) {
    return func_00343E18(arg0, info, arg2, 0);
}

u64 func_00343EE8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_0032C138(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

extern s32 func_00332E58(s32);
extern void func_00332F08(s32, u64);

u64 func_00343F38(u32 resource) {
    s32 i = 0;
    s32 count = *(s32 *)(resource + 0x10);
    s32 handle = func_00332E58(count);
    s32 *entry;
    if (count != i) {
        entry = (s32 *)(resource + 0x14);
        do {
            i++;
            func_00332F08(handle, func_0032C138(resource + *entry));
            entry++;
        } while (i != count);
    }
    return handle;
}

u64 func_00343FC0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_00343F38(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

s32 func_00344010(SdfRelocResource *resource) {
    s32 payload;

    /* Required to match: integer address arithmetic, not &resource->payload. */
    payload = (s32)resource + 0x20;
    sdfRelocatePackedResourceWords(payload, payload, payload + resource->relocationOffset, resource->relocationCount);
    return payload;
}

u64 func_00344050(u64 arg0, s32 *out) {
    u32 info[4];
    u64 buffer = func_00343ED0(arg0, info, 0);
    *out = func_00344010(info[0]);
    return buffer;
}

s32 func_00344098(SdfRelocResource *resource) {
    s32 payload;

    /* Required to match: the typed member address changes one instruction. */
    payload = (s32)resource + 0x20;
    sdfRelocatePackedResourceWords(payload, payload, payload + resource->relocationOffset, resource->relocationCount);
    return payload;
}

u64 func_003440D8(u64 arg0, s32 *out) {
    u32 info[4];
    u64 buffer = func_00343ED0(arg0, info, 0);
    *out = func_00344098(info[0]);
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_003425B0", sdfRelocatePackedResourceWords);

void func_00344208(SdfStreamNode *node, s32 inInterrupt) {
    s32 interruptsEnabled = 0;
    SdfStreamNode *prev;
    SdfStreamNode *next;
    if (inInterrupt == 0) {
        interruptsEnabled = func_0036DE70();
    }
    if (node->queued != 0) {
        prev = node->prev;
        next = node->next;
        if (prev == 0) {
            D_0043920C = next;
        } else {
            prev->next = next;
        }
        if (next == 0) {
            D_00439210 = prev;
        } else {
            next->prev = prev;
        }
        node->queued = 0;
    }
    if (inInterrupt == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

void func_00344298(SdfStreamNode *node, s32 inInterrupt) {
    s32 interruptsEnabled = 0;
    if (inInterrupt == 0) {
        interruptsEnabled = func_0036DE70();
    }
    if (node->queued != 0) {
        func_00344208(node, 1);
    }
    node->queued = 1;
    if (D_00439210 == 0) {
        D_0043920C = node;
    } else {
        D_00439210->next = node;
    }
    node->prev = D_00439210;
    node->next = 0;
    D_00439210 = node;
    if (inInterrupt == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfSoundAppendNode(SoundNode *node) {
    SoundNode **tail = &D_004391F8;
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
    SoundNode **link = &D_004391F8;
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

INCLUDE_ASM(const s32, "game/code_003425B0", sdfAllocateStreamFrameBuffers);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344420);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003444F8);

extern void *memset(void *, s32, u32);

void sdfSoundInitNodeFromFormat(u8 *dst, u8 *src) {
    memset(dst, 0, 0x8C);
    if (src[0] == 0) {
        dst[0x14] = 0;
    } else {
        if (src[1] == 0) {
            dst[0x14] = 1;
        } else {
            dst[0x14] = 2;
        }
    }
    dst[0x15] = src[2];
    dst[0x16] = src[3];
}

extern void func_00344420();

void func_003447D8(u8 *state, s32 arg1, u8 *src, s32 size) {
    s32 interruptsEnabled;
    sdfSoundInitNodeFromFormat(state, arg1);
    *(u16 *)(state + 0x3C) = *(u16 *)(src + 8);
    *(s32 *)(state + 0x40) = *(s32 *)(src + 0xC);
    *(u16 *)(state + 0x3E) = *(u16 *)(src + 0xA);
    sdfAllocateStreamFrameBuffers(state);
    func_00344420(state, src + 0x10, size - 0x10);
    interruptsEnabled = func_0036DE70();
    func_00344298((SdfStreamNode *)state, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    func_003450D8(0);
}

extern s32 func_003283E0(s32);
extern void sdfSoundInitNodeFromFormat();

void sdfSoundInitFormattedNode(u8 *state, s32 arg1, s32 arg2, s32 arg3) {
    sdfSoundInitNodeFromFormat(state, arg1);
    *(s32 *)(state + 0x5C) = arg2;
    *(s32 *)(state + 0x60) = arg3;
    state[0xC] = 1;
    *(s32 *)(state + 0x54) = func_003283E0(0x10100) + 0x100;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_003448D0);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344A08);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344B40);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344D60);

/* IPU DMA takes a 28-bit physical address and counts 16-byte quadwords. */
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

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344F08);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003450D8);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345198);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345268);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345298);

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
    s32 interruptsEnabled = func_0036DE70();
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
    func_003450D8(0);
}

extern void func_0032AEA0(s32, s32);
extern void func_003444F8();

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
    func_0032AEA0(*selectedBuffer, *selectedBuffer + state->bufferSize - 0x10);
    sdfAdvanceBufferedPlayback(state);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345488);

void sdfSoundInitAndAppendNode(u8 *state, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_003447D8(state, arg1, arg2, arg3);
    *(s32 *)(state + 0x34) = arg4;
    sdfSoundAppendNode((SoundNode *)state);
}

typedef struct SdfStreamParams {
    u8 mode;
    u8 param1;
    u8 param2;
    u8 param3;
} SdfStreamParams;

extern s32 sdfTexGetPrimaryResourceWord();

void func_00345628(s32 arg0, SdfStreamParams *params, s32 arg2, s32 arg3, u8 *source) {
    SdfStreamParams local = *params;
    switch (source[0x1A]) {
    case 0:
        local.mode = 0;
        break;
    case 2:
        local.mode = 1;
        break;
    }
    sdfSoundInitAndAppendNode(arg0, &local, arg2, arg3, sdfTexGetPrimaryResourceWord(source));
}

void sdfSoundInitFormattedAndAppendNode(u8 *state, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    sdfSoundInitFormattedNode(state, arg1, arg2, arg3);
    *(s32 *)(state + 0x34) = arg4;
    sdfSoundAppendNode((SoundNode *)state);
}

extern s32 D_004391FC;
extern s32 D_00439200;
extern s32 D_00439204;
extern s32 D_00439214;
extern u8 D_0047B480[];
extern s32 sdfAddHandler();
extern s32 func_00328318();
extern void func_003668B8();
extern void sceIpuInit();
extern void _StartThread();
extern void func_00345198();
extern void func_00345268();
extern void func_00345298();

void func_003456F8(void) {
    sceIpuInit();
    *(vu32 *)0x10002000 = 0x90000000;
    D_004391F8 = 0;
    D_00439204 = 0;
    D_004391FC = sdfAddHandler(1, 3, func_00345268, -1, 0);
    func_003668B8(3);
    D_00439200 = sdfAddHandler(1, 4, func_00345298, -1, 0);
    func_003668B8(4);
    D_00439214 = func_00328318(func_00345198, D_0047B480, 0x800, 0x46);
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

void func_003458E8(u32 arg0) {
    D_00438D0C = arg0;
}

void func_003458F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4) {
    D_00438D10 = arg0 * 0x10 + 0x7000;
    D_00438D14 = arg1 * 8 + 0x7900;
    D_00438D18 = D_00438D10 + arg2 * 0x10;
    D_00438D1C = D_00438D14 + arg3 * 8;
    D_00438D20 = arg4;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345928);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345BA0);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B84);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B88);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B8C);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B90);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B94);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438B98);

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

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D00);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D04);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D08);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D0C);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D10);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D14);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D18);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D1C);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D20);

INCLUDE_SDATA(const s32, "game/code_003425B0", D_00438D23);

