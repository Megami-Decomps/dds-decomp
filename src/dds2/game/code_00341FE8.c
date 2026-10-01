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

u32 func_003417A8(u32 command, u32 channel, void *packet, u32 size);

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

extern SndTrackVolume D_0047ABD0[];

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

typedef struct SndListenerState {
    s32 header[2];
    s32 value[6];
} SndListenerState;

extern SndListenerState D_004779B0;

/* Scale six world-space values and send them to the sound engine when they changed. */
void sndUpdateScaledListenerState(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f) {
    SndListenerState state;

    state.value[0] = a * 0.1f;
    state.value[1] = b * 0.1f;
    state.value[2] = c * 0.1f;
    state.value[3] = d * 0.1f;
    state.value[4] = e * 0.1f;
    state.value[5] = f * 0.1f;
    if (D_004779B0.value[0] != state.value[0] || D_004779B0.value[1] != state.value[1] ||
        D_004779B0.value[2] != state.value[2] || D_004779B0.value[3] != state.value[3] ||
        D_004779B0.value[4] != state.value[4] || D_004779B0.value[5] != state.value[5]) {
        func_003417A8(0x160, 0, &state, 0x20);
        D_004779B0 = state;
    }
}

typedef struct SndTrackSlot {
    s32 id;      /* 0x00 */
    u8 flagA;    /* 0x04 */
    u8 flagB;    /* 0x05 */
    u8 pad06[2];
} SndTrackSlot;

typedef struct SndMixerBlockSlots {
    /* 0x000 */ u8 pad000[0x190];
    /* 0x190 */ SndTrackSlot slots[13];
} SndMixerBlockSlots;

/* Returns 1 when the track id (high half of `packed`) is in the slot table, 2 when it is the
   current track, else 0. */
s32 func_00342168(s32 packed) {
    SndMixerBlockSlots *work = (SndMixerBlockSlots *)D_0047AA40;
    s32 id = packed >> 16;
    SndTrackSlot *slot;
    s32 i;

    func_003666D8(work, (u8 *)work + 0x8D0);
    slot = work->slots;
    for (i = 0; i < 13; i++) {
        if (slot->id == id) {
            return 1;
        }
        slot++;
    }
    return D_00438B80 == id ? 2 : 0;
}

extern s32 func_0035C860();
extern void mnuBuildSoundResourcePath();
extern void sdfSleepWithAlarm();
extern void (*D_00438B98)(void);

/* Make sure the MIDI bank named by the packed track id is resident, loading it if not. */
void sndEnsureMidiBankResident(s32 packed) {
    char name[0x10];
    char path[0x100];
    s32 status = func_00342168(packed);
    s32 id;

    switch (status) {
    case 0:
        id = packed >> 16;
        func_0035C860(name, "MIDI%04X.SMG", id);
        mnuBuildSoundResourcePath(path, name);
        func_003417A8(0xA0, 0, path, strlen(path) + 1);
        D_00438B80 = id;
        break;
    case 1:
        break;
    case 2:
        do {
            sdfSleepWithAlarm(2);
            if (D_00438B98 != NULL) {
                D_00438B98();
            }
        } while (func_00342168(packed) != 1);
        break;
    }
}

u32 sndSendFilenameCommand(char *filename) {
    u32 length = strlen(filename);
    return func_003417A8(0xA0, 0, filename, length);
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003422F8);

u32 sndSendChannelControlCommand(u32 channel) {
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
    SndTrackVolume *entry = &D_0047ABD0[index];
    s32 difference = entry->unk4 - entry->unk5;

    if (difference <= 0) {
        difference = 0;
    }
    return difference;
}

u8 *func_00342498(void) {
    return D_0047AA50;
}

SndTrackVolume *sndGetTrackSlotTable(void) {
    return D_0047ABD0;
}

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424B8);

INCLUDE_ASM(const s32, "game/code_00341FE8", func_003424D8);

/* Sends a prepared track identifier with the 0x17f setting. */

void sndStartTrackExtended(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
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

