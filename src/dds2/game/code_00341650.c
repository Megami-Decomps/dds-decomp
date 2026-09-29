#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_00341650(u32 command, u32 channel, void *packet, u32 size);

void func_003421E8(s32 trackId);

u32 func_003417A8(u32 command, u32 channel, void *packet, u32 size);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341650);

void func_003417A0(void) {
}

INCLUDE_ASM(const s32, "game/code_00341650", func_003417A8);

INCLUDE_ASM(const s32, "game/code_00341650", func_003417E0);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341878);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341958);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341A00);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341AD8);

/* Starts a track with the default 0x7f setting after preparing its identifier. */

void sndStartTrackDefault(s32 trackId) {
    CmdPacket packet;

    func_003421E8(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    func_00341650(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */

void func_00341C00(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */

void sndStartTrackAlternate(s32 trackId) {
    CmdPacket packet;

    func_003421E8(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    func_00341650(0x130, 0, &packet, 0x10);
}

void func_00341C78(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}
