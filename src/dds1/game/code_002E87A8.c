#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 id;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_002E8900(u32 arg0, u32 arg1, void *arg2, u32 arg3);

u32 func_002E87A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_002E9340(s32 arg0);

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E87A8);

void func_002E88F8(u32 arg0) {
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

void func_002E8D10(s32 id) {
    CmdPacket packet;

    func_002E9340(id);
    packet.id = id;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

void func_002E8D58(u32 id) {
    u32 packet[4];

    packet[0] = id;
    func_002E87A8(0x30, 0, packet, 0x10);
}

void func_002E8D88(s32 id) {
    CmdPacket packet;

    func_002E9340(id);
    packet.id = id;
    packet.setting = 0x7F;
    func_002E87A8(0x130, 0, &packet, 0x10);
}

void func_002E8DD0(u32 id) {
    u32 packet[4];

    packet[0] = id;
    func_002E87A8(0x30, 0, packet, 0x10);
}
