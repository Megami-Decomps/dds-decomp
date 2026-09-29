#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_002E8900(u32 command, u32 channel, void *packet, u32 size);

u32 func_002E87A8(u32 command, u32 channel, void *packet, u32 size);

void func_002E9340(s32 trackId);

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E87A8);

void func_002E88F8(u32 unused) {
}

u32 func_002E8900(u32 command, u32 channel, void *packet, u32 size) {
    u32 result = func_002E87A8(command, channel, packet, size);

    func_002E88F8(result);
    return result;
}

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E8938);

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E89D0);

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E8AB0);

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E8B58);

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E8C30);

/* Starts a track with the default 0x7f setting after preparing its identifier. */
void func_002E8D10(s32 trackId) {
    CmdPacket packet;

    func_002E9340(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */
void func_002E8D58(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_002E87A8(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */
void func_002E8D88(s32 trackId) {
    CmdPacket packet;

    func_002E9340(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    func_002E87A8(0x130, 0, &packet, 0x10);
}

void func_002E8DD0(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_002E87A8(0x30, 0, packet, 0x10);
}
