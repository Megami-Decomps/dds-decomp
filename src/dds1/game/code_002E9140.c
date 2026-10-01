#include "common.h"

extern s32 sndFindPackedTrackLoadStatus(s32 id);

extern s32 sndRequestedMidiBankId;

/* Per-track volume/balance record; unk4 and unk5 are the two balance bytes
   read by sndGetNonnegativeEntryBalance. */
typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
} SndTrackVolume;

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

typedef struct SndTrackSlot {
    s32 id;      /* 0x00 */
    u8 flagA;    /* 0x04 */
    u8 flagB;    /* 0x05 */
    u8 pad06[2];
} SndTrackSlot;

typedef struct FE0C0 {
    /* 0x000 */ u8 pad000[0x208];
    /* 0x208 */ u32 unk208;
    /* 0x20C */ u32 unk20C;
} SndMixerBlock;

/* Second view of the same block: the 13 track slots live at 0x190. */
typedef struct SndMixerBlockSlots {
    /* 0x000 */ u8 pad000[0x190];
    /* 0x190 */ SndTrackSlot slots[13];
} SndMixerBlockSlots;

extern void func_0030B458(void *dst, void *src);

extern SndTrackVolume D_003FE0D0[];

extern SndTrackVolume sndTrackBalanceEntries[];

extern SndMixerBlock sndMidiTrackState;

u32 sndSendCommandPacket();

u32 func_002E87A8(u32 command, u32 channel, void *packet, u32 size);

void sndEnsureMidiBankResident(s32 trackId);

/* Converts world coordinates to the sound engine's one-tenth scale. */
void sndSendSpatialPosition(s32 trackId, s32 parameter, f32 x, f32 y, f32 z) {
    u32 packet[8];

    packet[0] = trackId;
    packet[1] = parameter;
    packet[2] = (s32)(x * 0.1f);
    packet[3] = (s32)(y * 0.1f);
    packet[4] = (s32)(z * 0.1f);
    sndSendCommandPacket(0x170, 0, packet, 0x20);
}

typedef struct SndListenerState {
    s32 header[2];
    s32 value[6];
} SndListenerState;

extern SndListenerState D_003FB030;

/* Scale six world-space values and send them to the sound engine when they changed. */
void sndUpdateScaledListenerState(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f) {
    SndListenerState state;

    state.value[0] = a * 0.1f;
    state.value[1] = b * 0.1f;
    state.value[2] = c * 0.1f;
    state.value[3] = d * 0.1f;
    state.value[4] = e * 0.1f;
    state.value[5] = f * 0.1f;
    if (D_003FB030.value[0] != state.value[0] || D_003FB030.value[1] != state.value[1] ||
        D_003FB030.value[2] != state.value[2] || D_003FB030.value[3] != state.value[3] ||
        D_003FB030.value[4] != state.value[4] || D_003FB030.value[5] != state.value[5]) {
        sndSendCommandPacket(0x160, 0, &state, 0x20);
        D_003FB030 = state;
    }
}

/* Returns 1 when the track id (high half of `packed`) is in the slot table, 2 when it is the
   current track, else 0. */
s32 sndFindPackedTrackLoadStatus(s32 packed) {
    SndMixerBlockSlots *work = (SndMixerBlockSlots *)&sndMidiTrackState;
    s32 id = packed >> 16;
    SndTrackSlot *slot;
    s32 i;

    func_0030B458(work, (u8 *)work + 0x8D0);
    slot = work->slots;
    for (i = 0; i < 13; i++) {
        if (slot->id == id) {
            return 1;
        }
        slot++;
    }
    return sndRequestedMidiBankId == id ? 2 : 0;
}

extern s32 func_003014F0();
extern void mnuBuildSoundResourcePath();
extern void sdfSleepWithAlarm();
extern void (*D_003BD4A8)(void);

/* Make sure the MIDI bank named by the packed track id is resident, loading it if not. */
void sndEnsureMidiBankResident(s32 packed) {
    char name[0x10];
    char path[0x100];
    s32 status = sndFindPackedTrackLoadStatus(packed);
    s32 id;

    switch (status) {
    case 0:
        id = packed >> 16;
        func_003014F0(name, "MIDI%04X.SMG", id);
        mnuBuildSoundResourcePath(path, name);
        sndSendCommandPacket(0xA0, 0, path, strlen(path) + 1);
        sndRequestedMidiBankId = id;
        break;
    case 1:
        break;
    case 2:
        do {
            sdfSleepWithAlarm(2);
            if (D_003BD4A8 != NULL) {
                D_003BD4A8();
            }
        } while (sndFindPackedTrackLoadStatus(packed) != 1);
        break;
    }
}

u32 sndSendFilenameCommand(char *filename) {
    u32 length = strlen(filename);
    return sndSendCommandPacket(0xA0, 0, filename, length);
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9450);

u32 sndSendChannelControlCommand(u32 channel) {
    return sndSendCommandPacket((channel & 0xF) | 0x1C0, 0, NULL, 0);
}

/* Channel-group command packets: the group is bits 3-6 of the channel byte. */
void func_002E94E0(u8 channel) {
    sndSendCommandPacket(((channel >> 3) & 0xF) | 0x140, 0, 0, 0);
}

void func_002E9510(u8 channel) {
    sndSendCommandPacket(((channel >> 3) & 0xF) | 0x150, 0, 0, 0);
}

void sndReleaseMidiTrack(s32 id) {
    u32 packet[4];
    if (sndFindPackedTrackLoadStatus(id) != 0) {
        packet[0] = id;
        sndSendCommandPacket(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (sndRequestedMidiBankId == id) {
            sndRequestedMidiBankId = -1;
        }
    }
}

u8 func_002E9598(s32 index) {
    return sndTrackBalanceEntries[index].unk4;
}

u8 func_002E95B0(s32 index) {
    return sndTrackBalanceEntries[index].unk5;
}

s32 sndGetNonnegativeEntryBalance(s32 index) {
    SndTrackVolume *entry = &sndTrackBalanceEntries[index];
    s32 difference = entry->unk4 - entry->unk5;

    if (difference <= 0) {
        difference = 0;
    }
    return difference;
}

SndTrackVolume *func_002E95F0(void) {
    return D_003FE0D0;
}

SndTrackVolume *func_002E9600(void) {
    return sndTrackBalanceEntries;
}

u32 func_002E9610(u32 *outSecondaryValue) {
    if (outSecondaryValue != NULL) {
        *outSecondaryValue = sndMidiTrackState.unk20C;
    }
    return sndMidiTrackState.unk208;
}

INCLUDE_ASM(const s32, "game/code_002E9140", func_002E9630);

/* Sends a prepared track identifier with the 0x17f setting. */
void sndStartTrackExtended(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
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

INCLUDE_SDATA(const s32, "game/code_002E9140", sndRequestedMidiBankId);

