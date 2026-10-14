#ifndef SND_MIDI_STATE_VIEWS_H
#define SND_MIDI_STATE_VIEWS_H

#include "common.h"
#include "snd_track_slot.h"

/* Sparse typed view of the MIDI state words used by the sound driver. */
typedef struct SndMidiStateWords {
    u8 opaque000[0x208];
    u32 opaque208;
    u32 opaque20C;
} SndMidiStateWords;

/* The track-slot array is a separate view of the same state allocation. */
typedef struct SndMixerBlockSlots {
    u8 opaque000[0x190];
    SndTrackSlot slots[13];
} SndMixerBlockSlots;

/* Command 0x160 carries the opaque two-word prefix and six scaled values. */
typedef struct SndListenerState {
    s32 opaque00[2];
    s32 scaledValues[6];
} SndListenerState;

/* The paired MIDI getters use one byte index for two entry tables: the
 * previous-entry getter reads the two-word history table, while the current
 * entry getter reads the eight-word table. Bytes outside the observed fields
 * stay opaque; the owning allocation's extent is not established here. */
typedef struct SndMidiChannel {
    u8 opaque000[0x19];
    u8 entryIndex;
    u8 isEnabled;
    u8 opaque01B[5];
    u32 previousEntries[2];
    u32 currentEntries[8];
} SndMidiChannel;

typedef char SndMidiStateWordsLayoutAssert[
    (sizeof(SndMidiStateWords) == 0x210 &&
     (u32)&((SndMidiStateWords *)0)->opaque208 == 0x208 &&
     (u32)&((SndMidiStateWords *)0)->opaque20C == 0x20C) ? 1 : -1];

typedef char SndMixerBlockSlotsLayoutAssert[
    ((u32)&((SndMixerBlockSlots *)0)->slots == 0x190 &&
     sizeof(((SndMixerBlockSlots *)0)->slots) == 13 * sizeof(SndTrackSlot)) ? 1 : -1];

typedef char SndListenerStateLayoutAssert[
    (sizeof(SndListenerState) == 0x20 &&
     (u32)&((SndListenerState *)0)->scaledValues == 8) ? 1 : -1];

typedef char SndMidiChannelLayoutAssert[
    ((u32)&((SndMidiChannel *)0)->entryIndex == 0x19 &&
     (u32)&((SndMidiChannel *)0)->isEnabled == 0x1A &&
     (u32)&((SndMidiChannel *)0)->previousEntries == 0x20 &&
     sizeof(((SndMidiChannel *)0)->previousEntries) == 2 * sizeof(u32) &&
     (u32)&((SndMidiChannel *)0)->currentEntries == 0x28 &&
     sizeof(((SndMidiChannel *)0)->currentEntries) == 8 * sizeof(u32) &&
     sizeof(SndMidiChannel) == 0x48) ? 1 : -1];

#endif
