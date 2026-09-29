#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 volume;
    /* 0xA */ u16 pan;
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

void func_00341D20(u32 command) {
    func_00341650(command | 0x50, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341D48);

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341D90);

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341DD8);

void sndSetSequenceVolumePan(s32 trackId, s32 volume, s32 pan) {
    CmdPacket packet;
    func_003421E8(trackId);
    packet.trackId = trackId;
    packet.volume = volume;
    packet.pan = (u8)pan;
    func_00341650(0x90, 0, &packet, 0xC);
}

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341E80);
