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

void func_00341CA8(void) {
    func_00341650(0x1a0, 0, 0, 0);
}

void func_00341CD0(void) {
    func_00341650(0x210, 0, 0, 0);
}

void func_00341CF8(void) {
    func_00341650(0x40, 0, 0, 0);
}

void func_00341D20(u32 arg0) {
    func_00341650(arg0 | 0x50, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341D48);

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341D90);

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341DD8);

void sndSetSequenceVolumePan(s32 id, s32 volume, s32 pan) {
    CmdPacket packet;
    func_003421E8(id);
    packet.unk0 = id;
    packet.unk8 = volume;
    packet.unkA = (u8)pan;
    func_00341650(0x90, 0, &packet, 0xC);
}

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341E80);
