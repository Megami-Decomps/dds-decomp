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
extern s8 D_003BDA80;

typedef struct FE250Entry {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
} FE250Entry;

typedef struct CmdPacket {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

typedef struct FE0C0 {
    /* 0x000 */ u8 pad000[0x208];
    /* 0x208 */ u32 unk208;
    /* 0x20C */ u32 unk20C;
} FE0C0;

extern FE250Entry D_003FE0D0[];
extern FE250Entry D_003FE250[];
extern FE0C0 D_003FE0C0;
extern u32 effMiscRand(void *state);

extern u32 D_003BD494;

u32 func_002E8900(u32 arg0, u32 arg1, void *arg2, u32 arg3);
u32 func_002E87A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);
void func_002E9340(s32 arg0);
INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8398);

u32 func_002E83F8(void *arg0, u32 arg1) {
    return effMiscRand(arg0) % arg1;
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8430);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E84A0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8548);

void func_002E8628(void) {
    sceSifAllocIopHeap();
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8640);

void func_002E86C8(void) {
    for (;;) {
        do {
            SleepThread();
        } while (D_003BDA80 != 0);
        while (func_002E8640() == 0) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8700);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E87A8);

void func_002E88F8(u32 arg0) {
}

u32 func_002E8900(u32 arg0, u32 arg1, void *arg2, u32 arg3) {
    u32 ret = func_002E87A8(arg0, arg1, arg2, arg3);

    func_002E88F8(ret);
    return ret;
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8938);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E89D0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8AB0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8B58);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8C30);

void func_002E8D10(s32 arg0) {
    CmdPacket packet;

    func_002E9340(arg0);
    packet.unk0 = arg0;
    packet.unk4 = 0;
    packet.unk8 = 0x7F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

void func_002E8D58(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_002E87A8(0x30, 0, temp_v0, 0x10);
}

void func_002E8D88(s32 arg0) {
    CmdPacket packet;

    func_002E9340(arg0);
    packet.unk0 = arg0;
    packet.unk8 = 0x7F;
    func_002E87A8(0x130, 0, &packet, 0x10);
}

void func_002E8DD0(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_002E87A8(0x30, 0, temp_v0, 0x10);
}

void func_002E8E00(void) {
    func_002E87A8(0x1a0, 0, 0, 0);
}

void func_002E8E28(void) {
    func_002E87A8(0x210, 0, 0, 0);
}

void func_002E8E50(void) {
    func_002E87A8(0x40, 0, 0, 0);
}

void func_002E8E78(u32 arg0) {
    func_002E87A8(arg0 | 0x50, 0, 0, 0);
}

u32 func_002E8EA0(s32 arg0, char *arg1) {
    u32 len = strlen(arg1);

    return func_002E8900(arg0 | 0x70, 0, arg1, len);
}

u32 func_002E8EE8(s32 arg0, char *arg1) {
    u32 len = strlen(arg1);

    return func_002E8900(arg0 | 0x60, 0, arg1, len);
}

u32 func_002E8F30(s32 arg0, char *arg1) {
    u32 len = strlen(arg1);

    return func_002E8900(arg0 | 0x80, 0, arg1, len);
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8F78);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8FD8);

void func_002E9140(s32 arg0, s32 arg1, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = arg0;
    packet[1] = arg1;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    func_002E8900(0x170, 0, packet, 0x20);
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9198);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E92C0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9340);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9418);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9450);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E94B0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E94E0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9510);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9540);

u8 func_002E9598(s32 arg0) {
    return D_003FE250[arg0].unk4;
}

u8 func_002E95B0(s32 arg0) {
    return D_003FE250[arg0].unk5;
}

s32 func_002E95C8(s32 arg0) {
    FE250Entry *entry = &D_003FE250[arg0];
    s32 diff = entry->unk4 - entry->unk5;

    if (diff <= 0) {
        diff = 0;
    }
    return diff;
}

FE250Entry *func_002E95F0(void) {
    return D_003FE0D0;
}

FE250Entry *func_002E9600(void) {
    return D_003FE250;
}

u32 func_002E9610(u32 *arg0) {
    if (arg0 != NULL) {
        *arg0 = D_003FE0C0.unk20C;
    }
    return D_003FE0C0.unk208;
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9630);

void func_002E9690(s32 arg0) {
    CmdPacket packet;

    func_002E9340(arg0);
    packet.unk0 = arg0;
    packet.unk4 = 0;
    packet.unk8 = 0x17F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

void func_002E96D8(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_002E87A8(0xd0, 0, temp_v0, 0x10);
}

void func_002E9708(void) {
    func_002E87A8(0x180, 0, 0, 0);
}

void func_002E9730(void) {
    func_002E87A8(400, 0, 0, 0);
}

void func_002E9758(s32 arg0) {
    func_002E87A8(((arg0 + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9788);

u32 func_002E97E0(void) {
    return D_003BD498;
}

void func_002E97E8(void) {
    func_002E8900(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9810);

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

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E98A0);

u32 func_002E98E8(void) {
    return D_003BD49C;
}

void func_002E98F0(void) {
    func_002E8900(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9918);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9948);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E99A0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9BE8);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9C80);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E9FB0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EA2E0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EA448);

INCLUDE_RODATA(const s32, "game/code_002E8398", D_003B48A0);

INCLUDE_RODATA(const s32, "game/code_002E8398", D_003B48B8);

INCLUDE_RODATA(const s32, "game/code_002E8398", D_003B48E0);

INCLUDE_RODATA(const s32, "game/code_002E8398", D_003B4900);

INCLUDE_RODATA(const s32, "game/code_002E8398", jtbl_003B4920);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EA5C0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EAC98);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EAD88);

INCLUDE_RODATA(const s32, "game/code_002E8398", D_003B4DB8);

INCLUDE_RODATA(const s32, "game/code_002E8398", D_003B4E30);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EAE78);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EAEB8);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EAF70);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB028);

u64 func_002EB040(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg0, temp_v2, 0);
    temp_v1 = func_002D3288(temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB090);

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

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB1A8);

s32 func_002EB1F0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x20;
    func_002EB278(temp_v0, temp_v0, temp_v0 + *(s32 *)(arg0 + 0x10), *(u32 *)(arg0 + 0x14));
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB230);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB278);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB360);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB3F0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB490);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB4D0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB510);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB578);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB650);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB8C0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB930);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EB9C8);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EBA28);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EBB60);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EBC98);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EBEB8);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EBF88);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EBFF0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC060);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC230);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC2F0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC3C0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC3F0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC488);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC4B0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC4D8);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC560);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC5E0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC748);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC780);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC818);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC850);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC900);

void func_002EC950(void) {
    func_003004E8();
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002EC968);

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

INCLUDE_ASM(const s32, "game/code_002E8398", func_002ECA80);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002ECCF8);
