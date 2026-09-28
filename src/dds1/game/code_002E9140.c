#include "common.h"

extern s32 func_002E92C0(s32 id);

extern s32 D_003BD490;

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
    /* 0x0 */ u32 id;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
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

u32 func_002E8900(u32 arg0, u32 arg1, void *arg2, u32 arg3);

u32 func_002E87A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_002E9340(s32 arg0);

void func_002E9140(s32 arg0, s32 arg1, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = arg0;
    packet[1] = arg1;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    func_002E8900(0x170, 0, packet, 0x20);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9198);

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E92C0);

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9340);

u32 func_002E9418(char *filename) {
    u32 length = strlen(filename);
    return func_002E8900(0xA0, 0, filename, length);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9450);

u32 func_002E94B0(u32 channel) {
    return func_002E8900((channel & 0xF) | 0x1C0, 0, NULL, 0);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E94E0);

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9510);

void soundReleaseMidiTrack(s32 id) {
    u32 packet[4];
    if (func_002E92C0(id) != 0) {
        packet[0] = id;
        func_002E8900(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (D_003BD490 == id) {
            D_003BD490 = -1;
        }
    }
}

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

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9630);

void func_002E9690(s32 id) {
    CmdPacket packet;

    func_002E9340(id);
    packet.id = id;
    packet.unk4 = 0;
    packet.setting = 0x17F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

void func_002E96D8(u32 arg0) {
    u32 temp_v0 [4];

    temp_v0[0] = arg0;
    func_002E87A8(0xd0, 0, temp_v0, 0x10);
}

INCLUDE_SDATA(const s32, "game/code_002E9140", D_003BD490);

