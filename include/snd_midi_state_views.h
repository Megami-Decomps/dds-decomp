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

/* Paired MIDI getters index the two history words and eight entry words
 * directly with the stored byte index. The owner's allocation extent is not
 * established here, so this view asserts fields/array counts only. */
typedef struct SndMidiChannel {
    u8 opaque000[0x19];
    u8 index;
    u8 enabled;
    u8 opaque01B[5];
    u32 earlierEntries[2];
    u32 entries[8];
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
    ((u32)&((SndMidiChannel *)0)->index == 0x19 &&
     (u32)&((SndMidiChannel *)0)->enabled == 0x1A &&
     (u32)&((SndMidiChannel *)0)->earlierEntries == 0x20 &&
     sizeof(((SndMidiChannel *)0)->earlierEntries) == 2 * sizeof(u32) &&
     (u32)&((SndMidiChannel *)0)->entries == 0x28 &&
     sizeof(((SndMidiChannel *)0)->entries) == 8 * sizeof(u32)) ? 1 : -1];

#endif
