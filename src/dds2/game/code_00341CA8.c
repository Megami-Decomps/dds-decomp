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

u32 func_00341DD8(s32 command, char *text) {
    s32 cmd = command;
    u32 len = strlen(text);
    return sndSendCommandPacket(cmd | 0x80, 0, text, len);
}

/* Send a 12-byte sequence command; the trailing struct word is not transmitted. */
void sndSetSequenceVolumePan(s32 trackId, s32 volume, s32 pan) {
    SequenceVolumePanPacket packet;
    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.volume = volume;
    packet.pan = (u8)pan;
    func_00341650(0x90, 0, &packet, 0xC);
}

/* Map a wrapped listener-relative angle and distance onto sequence volume/pan. */
void func_00341E80(s32 trackId, f32 angle, f32 distance) {
    s32 degrees = (s32)angle % 360;
    s32 side = 1;
    s32 pan;
    f32 volume;

    if (degrees > 180) {
        degrees -= 360;
    }
    if (degrees < -180) {
        degrees += 360;
    }
    if (degrees < 0) {
        degrees = -degrees;
        side = -1;
    }
    volume = 1.0f;
    if (degrees < 45) {
        pan = 63 - (s32)(degrees * 0.71111f) * side;
    } else if (degrees <= 90) {
        pan = 63 - (s32)(degrees * 0.7f) * side;
        volume = 1.0f - (degrees - 45) * 0.004444f;
    } else {
        volume = 0.75f - (degrees - 90) * 0.005556f;
        pan = 63 - (s32)((180 - degrees) * 0.7f) * side;
    }
    if (distance > 1.0f) {
        volume /= distance * 0.333f + 1.0f;
    }
    sndSetSequenceVolumePan(trackId, (s32)(volume * 127.0f), pan);
}
