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
    /* 0x0 */ u32 trackId;
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

u32 func_002E8900();

u32 func_002E87A8(u32 command, u32 channel, void *packet, u32 size);

void func_002E9340(s32 trackId);

/* Converts world coordinates to the sound engine's one-tenth scale. */
void soundSendSpatialPosition(s32 trackId, s32 parameter, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = trackId;
    packet[1] = parameter;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    func_002E8900(0x170, 0, packet, 0x20);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9198);

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E92C0);

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9340);

u32 sndSendFilenameCommand(char *filename) {
    u32 length = strlen(filename);
    return func_002E8900(0xA0, 0, filename, length);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9450);

u32 func_002E94B0(u32 channel) {
    return func_002E8900((channel & 0xF) | 0x1C0, 0, NULL, 0);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E94E0);

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9510);

void sndReleaseMidiTrack(s32 id) {
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

u8 func_002E9598(s32 index) {
    return D_003FE250[index].unk4;
}

u8 func_002E95B0(s32 index) {
    return D_003FE250[index].unk5;
}

s32 func_002E95C8(s32 index) {
    FE250Entry *entry = &D_003FE250[index];
    s32 difference = entry->unk4 - entry->unk5;

    if (difference <= 0) {
        difference = 0;
    }
    return difference;
}

FE250Entry *func_002E95F0(void) {
    return D_003FE0D0;
}

FE250Entry *func_002E9600(void) {
    return D_003FE250;
}

u32 func_002E9610(u32 *outSecondaryValue) {
    if (outSecondaryValue != NULL) {
        *outSecondaryValue = D_003FE0C0.unk20C;
    }
    return D_003FE0C0.unk208;
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9630);

/* Sends a prepared track identifier with the 0x17f setting. */
void func_002E9690(s32 trackId) {
    CmdPacket packet;

    func_002E9340(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x17F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

void func_002E96D8(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_002E87A8(0xd0, 0, packet, 0x10);
}

INCLUDE_SDATA(const s32, "game/code_002E9140", D_003BD490);

