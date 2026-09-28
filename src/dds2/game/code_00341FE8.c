#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_00341650(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_003421E8(s32 arg0);

u32 func_003417A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

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

extern FE250Entry D_0047ABD0[];

extern s32 func_00342168(s32 id);

extern s32 D_00438B80;

void func_00341FE8(s32 arg0, s32 arg1, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = arg0;
    packet[1] = arg1;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    func_003417A8(0x170, 0, packet, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342040);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342168);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003421E8);

u32 func_003422C0(char *filename) {
    u32 length = strlen(filename);
    return func_003417A8(0xA0, 0, filename, length);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003422F8);

u32 func_00342358(u32 channel) {
    return func_003417A8((channel & 0xF) | 0x1C0, 0, NULL, 0);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342388);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003423B8);

void func_003423E8(s32 id) {
    u32 packet[4];
    if (func_00342168(id) != 0) {
        packet[0] = id;
        func_003417A8(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (D_00438B80 == id) {
            D_00438B80 = -1;
        }
    }
}

u8 func_00342440(s32 arg0) {
    return D_0047ABD0[arg0].unk4;
}

u8 func_00342458(s32 arg0) {
    return D_0047ABD0[arg0].unk5;
}

s32 func_00342470(s32 arg0) {
    FE250Entry *entry = &D_0047ABD0[arg0];
    s32 diff = entry->unk4 - entry->unk5;

    if (diff <= 0) {
        diff = 0;
    }
    return diff;
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342498);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424A8);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424B8);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424D8);

void func_00342538(s32 arg0) {
    CmdPacket packet;

    func_003421E8(arg0);
    packet.unk0 = arg0;
    packet.unk4 = 0;
    packet.unk8 = 0x17F;
    func_00341650(0x20, 0, &packet, 0x10);
}

void func_00342580(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_00341650(0xd0, 0, temp_v0, 0x10);
}

INCLUDE_SDATA(const s32, "game/code_00341FE8", D_00438B80);

