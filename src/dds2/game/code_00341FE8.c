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

extern FE250Entry D_0047ABD0[];

extern s32 func_00342168(s32 id);

extern s32 D_00438B80;

extern u8 D_0047AA40[];

extern u8 D_0047AA50[];

extern u8 D_0047B310[];

extern s32 D_004391D0;

extern void func_003666D8(void *a0, void *a1);

/* Converts world coordinates to the sound engine's one-tenth scale. */
void sndSendSpatialPosition(s32 trackId, s32 parameter, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = trackId;
    packet[1] = parameter;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    func_003417A8(0x170, 0, packet, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342040);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342168);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003421E8);

u32 sndSendFilenameCommand(char *filename) {
    u32 length = strlen(filename);
    return func_003417A8(0xA0, 0, filename, length);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003422F8);

u32 func_00342358(u32 channel) {
    return func_003417A8((channel & 0xF) | 0x1C0, 0, NULL, 0);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_00342388);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003423B8);

void sndReleaseMidiTrack(s32 id) {
    u32 packet[4];
    if (func_00342168(id) != 0) {
        packet[0] = id;
        func_003417A8(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (D_00438B80 == id) {
            D_00438B80 = -1;
        }
    }
}

u8 func_00342440(s32 index) {
    return D_0047ABD0[index].unk4;
}

u8 func_00342458(s32 index) {
    return D_0047ABD0[index].unk5;
}

s32 sndGetNonnegativeEntryBalance(s32 index) {
    FE250Entry *entry = &D_0047ABD0[index];
    s32 difference = entry->unk4 - entry->unk5;

    if (difference <= 0) {
        difference = 0;
    }
    return difference;
}

u8 *func_00342498(void) {
    return D_0047AA50;
}

FE250Entry *func_003424A8(void) {
    return D_0047ABD0;
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424B8);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424D8);

/* Sends a prepared track identifier with the 0x17f setting. */

void sndStartTrackExtended(s32 trackId) {
    CmdPacket packet;

    func_003421E8(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x17F;
    func_00341650(0x20, 0, &packet, 0x10);
}

void func_00342580(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0xd0, 0, packet, 0x10);
}

INCLUDE_SDATA(const s32, "game/code_00341FE8", D_00438B80);

