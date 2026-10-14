#ifndef ACTIVE_SOUND_NODE_H
#define ACTIVE_SOUND_NODE_H

#include "common.h"

/* Per-unit active sound-list entry, allocated at exactly 0x14 bytes. */
typedef struct ActiveSoundNode {
    u32 flags; /* 0x00: bits 1/8 are inspected by the resource-lifetime checks. */
    u8 loadState; /* 0x04: sound-file task sets this to 2 while loading. */
    u8 pad05[3];  /* 0x05: no accesses established. */
    u32 packedTrackPosition; /* 0x08: key passed to packed-track load queries. */
    struct ActiveSoundNode *previous; /* 0x0C */
    struct ActiveSoundNode *next; /* 0x10 */
} ActiveSoundNode;

typedef char ActiveSoundNode_size_must_be_0x14[
    (sizeof(ActiveSoundNode) == 0x14) ? 1 : -1];
typedef char ActiveSoundNode_loadState_offset_must_be_0x04[
    ((u32)&((ActiveSoundNode *)0)->loadState == 0x04) ? 1 : -1];
typedef char ActiveSoundNode_packedTrackPosition_offset_must_be_0x08[
    ((u32)&((ActiveSoundNode *)0)->packedTrackPosition == 0x08) ? 1 : -1];
typedef char ActiveSoundNode_previous_offset_must_be_0x0C[
    ((u32)&((ActiveSoundNode *)0)->previous == 0x0C) ? 1 : -1];
typedef char ActiveSoundNode_next_offset_must_be_0x10[
    ((u32)&((ActiveSoundNode *)0)->next == 0x10) ? 1 : -1];

#endif /* ACTIVE_SOUND_NODE_H */
