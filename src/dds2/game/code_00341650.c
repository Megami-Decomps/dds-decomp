#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_00341650(u32 command, u32 channel, void *packet, u32 size);

void sndEnsureMidiBankResident(s32 trackId);

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size);

/* Sound command work block: header, 16 channel records, 13 track slots. */
typedef struct SndChannel {
    u8 index;     /* 0x0 */
    u8 unk1;      /* 0x1 */
    s16 unk2;     /* 0x2 */
    u32 unk4;     /* 0x4 */
    u32 unk8;     /* 0x8 */
    u32 unkC;     /* 0xC */
} SndChannel;

typedef struct SndTrackSlot {
    s32 id;       /* 0x0 */
    u8 flagA;     /* 0x4 */
    u8 flagB;     /* 0x5 */
    u8 pad6[2];
} SndTrackSlot;

typedef struct SndWork {
    s32 header;                /* 0x000 */
    u8 pad004[0xC];
    SndChannel channels[16];   /* 0x010 */
    u8 pad110[0x80];
    SndTrackSlot slots[13];    /* 0x190 */
    u8 pad1F8[8];
    u32 unk200;                /* 0x200 */
    u32 unk204;                /* 0x204 */
} SndWork;

extern SndWork sndMidiTrackState;
extern u32 func_003414D0(s32 size);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341650);

void func_003417A0(u32 unused) {
}

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size) {
    u32 result = func_00341650(command, channel, packet, size);

    func_003417A0(result);
    return result;
}

INCLUDE_ASM(const s32, "game/code_00341650", func_003417E0);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341878);

void sndInitializeChannelAndTrackState(s32 unused, s32 header) {
    SndChannel *channel;
    s32 i;

    sndMidiTrackState.header = header;
    channel = sndMidiTrackState.channels;
    for (i = 0; i < 16; i++) {
        channel->index = i;
        channel->unk1 = 0x20;
        channel->unk2 = 0;
        channel->unk8 = 0;
        channel->unk4 = 0;
        channel->unkC = 0;
        channel++;
    }
    sndMidiTrackState.channels[0].unk8 = func_003414D0(0x3B600);
    sndMidiTrackState.unk200 = 0;
    sndMidiTrackState.unk204 = 0;
    for (i = 0; i < 13; i++) {
        sndMidiTrackState.slots[i].id = -1;
        sndMidiTrackState.slots[i].flagA = 0;
        sndMidiTrackState.slots[i].flagB = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00341650", func_00341A00);

INCLUDE_ASM(const s32, "game/code_00341650", func_00341AD8);

/* Starts a track with the default 0x7f setting after preparing its identifier. */

void sndStartTrackDefault(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    func_00341650(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */

void sndSendSingleWordControlPacket(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */

void sndStartTrackAlternate(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    func_00341650(0x130, 0, &packet, 0x10);
}

void func_00341C78(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}
