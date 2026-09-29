#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_00341650(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_003421E8(s32 arg0);

u32 func_003417A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341650);

void func_003417A0(void) {
}

INCLUDE_ASM(const s32, "game/code_00341650", func_003417A8);

INCLUDE_ASM(const s32, "game/code_00341650", func_003417E0);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341878);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341958);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341A00);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341AD8);

void func_00341BB8(s32 trackId) {
    CmdPacket packet;

    func_003421E8(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.unk8 = 0x7F;
    func_00341650(0x20, 0, &packet, 0x10);
}

void func_00341C00(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}

void func_00341C30(s32 trackId) {
    CmdPacket packet;

    func_003421E8(trackId);
    packet.trackId = trackId;
    packet.unk8 = 0x7F;
    func_00341650(0x130, 0, &packet, 0x10);
}

void func_00341C78(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}
