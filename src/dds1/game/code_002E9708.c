#include "common.h"

extern s32 D_003BD620;

extern s32 D_003BD624;

extern s32 D_003BD628;

extern s32 D_003BD62C;

extern u32 D_003BD630;

extern u32 D_003BD61C;

extern u64 func_002EB090(u32);
extern u64 func_002D3288(u32);

extern u64 func_002EB028(u64, u32 *, u64);

extern u32 D_003BD49C;

extern u32 D_003BD498;
extern char D_00398948[];
extern s32 D_003BDA90;
extern s32 SignalSema(s32);
extern void FlushCache(s32);
extern s32 createSemaphore(s32, s32, s32);
extern s32 GetThreadId(void);
extern void sceSifSetRpcQueue(void *, s32);
extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);
extern void sceSifRpcLoop(void *);
extern u8 D_003FEAC0[];
extern s32 func_002E99A0();

extern void func_002D0B50(void *out);

extern void func_002E4C28(char *fmt, ...);

typedef struct MidiChannel {
    u8 pad00[0x19];
    u8 index;
    u8 enabled;
    u8 pad1B[5];
    u32 earlierEntries[2];
    u32 entries[8];
} MidiChannel;

extern u32 D_003BD494;

u32 func_002E8900(u32 arg0, u32 arg1, void *arg2, u32 arg3);

u32 func_002E87A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);
typedef struct SoundNode {
    u8 pad00[8];
    struct SoundNode *next;
} SoundNode;

extern SoundNode *D_003BDA98;
extern s32 sceIpuSync(s32, s32);
void func_002E9708(void) {
    func_002E87A8(0x180, 0, 0, 0);
}

void func_002E9730(void) {
    func_002E87A8(400, 0, 0, 0);
}

void func_002E9758(s32 arg0) {
    func_002E87A8(((arg0 + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

s32 sdfSoundSendNamedCommand(const char *name, u8 channel) {
    if (D_003BD498 != 0) {
        return 1;
    }
    strcpy(D_00398948, name);
    func_002E8900((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 func_002E97E0(void) {
    return D_003BD498;
}

void func_002E97E8(void) {
    func_002E8900(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E9810);

void func_002E9840(u32 arg0) {
    if (0x10 < arg0) {
        arg0 = 0x10;
    }
    if (arg0 == 0) {
        arg0 = 1;
    }
    func_002E8900((arg0 - 1) | 0x1d0, 0, 0, 0);
}

u32 func_002E9880(u32 arg0) {
    if (D_003BD49C != 0) {
        return 0;
    }
    D_003BD494 = arg0;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E98A0);

u32 func_002E98E8(void) {
    return D_003BD49C;
}

void func_002E98F0(void) {
    func_002E8900(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E9918);

s32 sdfSoundHandleRpcEvent(s32 arg0, u32 event) {
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
        SignalSema(D_003BDA90);
        break;
    case 0:
    case 2:
    case 7:
        SignalSema(D_003BDA90);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E99A0);

void sdfSoundStartRpcServer(void) {
    u8 queue[0x20];
    u8 server[0x50];
    s32 semaphore = createSemaphore(0, 1, 0);
    D_003BDA90 = semaphore;
    if (semaphore <= 0) {
        for (;;) {
        }
    }
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x54524E53, func_002E99A0, D_003FEAC0, 0, 0, queue);
    sceSifRpcLoop(queue);
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E9C80);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002E9FB0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EA2E0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EA448);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48A0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48B8);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B48E0);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4900);

INCLUDE_RODATA(const s32, "game/code_002E9708", jtbl_003B4920);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EA5C0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EAC98);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EAD88);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4DB8);

INCLUDE_RODATA(const s32, "game/code_002E9708", D_003B4E30);

void soundPrintMemoryInfo(void) {
    s32 info[6];
    func_002D0B50(info);
    func_002E4C28(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info[0], info[1], info[2], info[3], info[4], info[5]);
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EAEB8);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EAF70);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB028);

u64 func_002EB040(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg0, temp_v2, 0);
    temp_v1 = func_002D3288(temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB090);

u64 func_002EB118(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg0, temp_v2, 0);
    temp_v1 = func_002EB090(temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

s32 func_002EB168(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_002EB278(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

u64 func_002EB1A8(u64 arg0, s32 *out) {
    u32 info[4];
    u64 buffer = func_002EB028(arg0, info, 0);
    *out = func_002EB168(info[0]);
    return buffer;
}

s32 func_002EB1F0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_002EB278(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

u64 func_002EB230(u64 arg0, s32 *out) {
    u32 info[4];
    u64 buffer = func_002EB028(arg0, info, 0);
    *out = func_002EB1F0(info[0]);
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB278);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB360);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB3F0);

void sdfSoundAppendNode(SoundNode *node) {
    SoundNode **tail = &D_003BDA98;
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
    SoundNode **link = &D_003BDA98;
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

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB510);
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB578);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB650);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB8C0);
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB930);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EB9C8);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EBA28);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EBB60);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EBC98);

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

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC2F0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC3C0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC3F0);

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

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC4D8);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC560);
INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC5E0);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC748);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC780);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC818);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC850);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC900);

void func_002EC950(void) {
    func_003004E8();
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002EC968);

void func_002ECA40(u32 arg0) {
    D_003BD61C = arg0;
}

void func_002ECA48(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4) {
    D_003BD620 = arg0 * 0x10 + 0x7000;
    D_003BD624 = arg1 * 8 + 0x7900;
    D_003BD628 = D_003BD620 + arg2 * 0x10;
    D_003BD62C = D_003BD624 + arg3 * 8;
    D_003BD630 = arg4;
}

INCLUDE_ASM(const s32, "game/code_002E9708", func_002ECA80);

INCLUDE_ASM(const s32, "game/code_002E9708", func_002ECCF8);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD494);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD498);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD49C);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A0);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A4);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD4A8);

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

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD610);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD614);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD618);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD61C);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD620);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD624);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD628);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD62C);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD630);

INCLUDE_SDATA(const s32, "game/code_002E9708", D_003BD633);

