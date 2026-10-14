#ifndef SND_TRACK_SLOT_H
#define SND_TRACK_SLOT_H

#include "common.h"

/* The mixer lookup and balance query share this eight-byte MIDI track slot.
 * Only the ID and two balance-query components are interpreted here. */
typedef struct SndTrackSlot {
    s32 id; /* 0x00 */
    u8 balanceFirstComponent; /* 0x04 */
    u8 balanceSecondComponent; /* 0x05 */
    u8 unknown_06[2];
} SndTrackSlot;

typedef char SndTrackSlot_size_must_be_8[
    (sizeof(SndTrackSlot) == 8) ? 1 : -1];
typedef char SndTrackSlot_id_offset_must_be_0[
    ((u32)&((SndTrackSlot *)0)->id == 0) ? 1 : -1];
typedef char SndTrackSlot_balanceFirstComponent_offset_must_be_4[
    ((u32)&((SndTrackSlot *)0)->balanceFirstComponent == 4) ? 1 : -1];
typedef char SndTrackSlot_balanceSecondComponent_offset_must_be_5[
    ((u32)&((SndTrackSlot *)0)->balanceSecondComponent == 5) ? 1 : -1];
typedef char SndTrackSlot_unknown_06_offset_must_be_6[
    ((u32)&((SndTrackSlot *)0)->unknown_06 == 6) ? 1 : -1];

#endif /* SND_TRACK_SLOT_H */
