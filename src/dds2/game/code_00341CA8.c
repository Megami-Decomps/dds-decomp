#include "common.h"

typedef struct SequenceVolumePanPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 volume;
    /* 0xA */ u16 pan;
    /* 0xC */ u32 unkC;
} SequenceVolumePanPacket;

u32 func_00341650(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void sndEnsureMidiBankResident(s32 arg0);

u32 sndSendCommandPacket(u32 arg0, u32 arg1, void *arg2, u32 arg3);

void func_00341CA8(void) {
    func_00341650(0x1a0, 0, 0, 0);
}

void func_00341CD0(void) {
    func_00341650(0x210, 0, 0, 0);
}

void func_00341CF8(void) {
    func_00341650(0x40, 0, 0, 0);
}

/* Dispatch the command family selected by the caller, without a payload. */
void sndDispatchCommandWithoutPayload(u32 command) {
    func_00341650(command | 0x50, 0, 0, 0);
}

/* Send a NUL-terminated command string; the transport excludes the terminator. */
u32 sndSubmitTextCommandPayload(s32 command, char *text) {
    u32 length = strlen(text);

    return sndSendCommandPacket(command | 0x70, 0, text, length);
}

u32 func_00341D90(s32 command, char *text) {
    u32 length = strlen(text);

    return sndSendCommandPacket(command | 0x60, 0, text, length);
}

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341DD8);

/* Send a 12-byte sequence command; the trailing struct word is not transmitted. */
void sndSetSequenceVolumePan(s32 trackId, s32 volume, s32 pan) {
    SequenceVolumePanPacket packet;
    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.volume = volume;
    packet.pan = (u8)pan;
    func_00341650(0x90, 0, &packet, 0xC);
}

INCLUDE_ASM(const s32, "game/code_00341CA8", func_00341E80);
