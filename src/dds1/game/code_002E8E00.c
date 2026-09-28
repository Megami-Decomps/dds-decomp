#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_002E8900(u32 arg0, u32 arg1, void *arg2, u32 arg3);

u32 func_002E87A8(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_002E9340(s32 arg0);

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

void func_002E8F78(s32 id, s32 volume, s32 pan) {
    CmdPacket packet;
    func_002E9340(id);
    packet.unk0 = id;
    packet.unk8 = volume;
    packet.unkA = (u8)pan;
    func_002E87A8(0x90, 0, &packet, 0xC);
}

INCLUDE_ASM(const s32, "game/code_002E8E00", func_002E8FD8);
