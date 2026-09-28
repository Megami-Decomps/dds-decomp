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

extern s32 createSemaphore(s32, s32, s32);

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

extern s32 sceIpuSync(s32, s32);

typedef struct SoundIpuBuffer {
    u8 pad00[0x18];
    u8 active;
    u8 pad19[3];
    s32 size;
    u32 buffers[2];
} SoundIpuBuffer;

void func_003425B0(void) {
    func_00341650(0x180, 0, 0, 0);
}

void func_003425D8(void) {
    func_00341650(400, 0, 0, 0);
}

void func_00342600(s32 arg0) {
    func_00341650(((arg0 + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

s32 sdfSoundSendNamedCommand(const char *name, u8 channel) {
    if (D_00438B88 != 0) {
        return 1;
    }
    strcpy(D_0040BAF8, name);
    func_003417A8((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 func_00342688(void) {
    return D_00438B88;
}

void func_00342690(void) {
    func_003417A8(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_003426B8);

void func_003426E8(u32 arg0) {
    if (0x10 < arg0) {
        arg0 = 0x10;
    }
    if (arg0 == 0) {
        arg0 = 1;
    }
    func_003417A8((arg0 - 1) | 0x1d0, 0, 0, 0);
}

u32 func_00342728(u32 arg0) {
    if (D_00438B8C != 0) {
        return 0;
    }
    D_00438B84 = arg0;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342748);

u32 func_00342790(void) {
    return D_00438B8C;
}

void func_00342798(void) {
    func_003417A8(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_003427C0);

INCLUDE_ASM(const s32, "game/code_003425B0", sdfSoundHandleRpcEvent);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00342848);

void sdfSoundStartRpcServer(void) {
    u8 queue[0x20];
    u8 server[0x50];
    s32 semaphore = createSemaphore(0, 1, 0);
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

void soundPrintMemoryInfo(void) {
    s32 info[6];
    func_00329A00(info);
    func_0033DAD8(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info[0], info[1], info[2], info[3], info[4], info[5]);
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343D60);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343E18);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343ED0);

u64 func_00343EE8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_0032C138(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00343F38);

u64 func_00343FC0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_00343F38(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

s32 func_00344010(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_00344120(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

u64 func_00344050(u64 arg0, s32 *out) {
    u32 info[4];
    u64 buffer = func_00343ED0(arg0, info, 0);
    *out = func_00344010(info[0]);
    return buffer;
}

s32 func_00344098(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_00344120(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

u64 func_003440D8(u64 arg0, s32 *out) {
    u32 info[4];
    u64 buffer = func_00343ED0(arg0, info, 0);
    *out = func_00344098(info[0]);
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344120);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344208);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344298);

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

INCLUDE_ASM(const s32, "game/code_003425B0", func_003443B8);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344420);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003444F8);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344768);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003447D8);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00344870);

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

u32 soundGetSelectedChannelEntry(MidiChannel *channel) {
    u32 result = 0;
    if (channel->enabled != 0) {
        result = channel->entries[channel->index];
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345380);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345408);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345488);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003455F0);

INCLUDE_ASM(const s32, "game/code_003425B0", func_00345628);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003456C0);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003456F8);

INCLUDE_ASM(const s32, "game/code_003425B0", func_003457A8);

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

