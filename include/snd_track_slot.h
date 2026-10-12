#ifndef SND_TRACK_SLOT_H
#define SND_TRACK_SLOT_H

#include "common.h"

/* Eight-byte MIDI track slot; only the track ID is interpreted by current clients. */
typedef struct SndTrackSlot {
    s32 id; /* 0x00 */
    u8 unknown_04[4];
} SndTrackSlot;

typedef char SndTrackSlot_size_must_be_8[
    (sizeof(SndTrackSlot) == 8) ? 1 : -1];
typedef char SndTrackSlot_id_offset_must_be_0[
    ((u32)&((SndTrackSlot *)0)->id == 0) ? 1 : -1];
typedef char SndTrackSlot_unknown_04_offset_must_be_4[
    ((u32)&((SndTrackSlot *)0)->unknown_04 == 4) ? 1 : -1];

#endif /* SND_TRACK_SLOT_H */
