#ifndef BTL_ACTOR_WORK_H
#define BTL_ACTOR_WORK_H

#include "btl.h"
#include "common.h"
#include "btl_sound.h"

/* Observed prefix of the DDS1 singleton battle work. Actor/task registration,
 * resource-node lists, and packed battle tint share this owner; intervening
 * words without a proved role remain opaque. */
typedef struct BtlActorWork {
    u8 pad_00[0x208];
    s32 unk208;
    u8 pad20C[0x1C];
    BtlUnit *actorList; /* 0x228 */
    struct SoundTask *taskTail; /* Newest registration; reverse traversal. */
    struct SoundTask *taskHead; /* Oldest registration; forward traversal. */
    struct SoundResourceNode *soundResourceHead; /* Allocated resource nodes. */
    struct ActiveSoundNode *soundList; /* Independent active-node list. */
    struct SoundSlotOwner *soundSlotOwners; /* Shared category/id owners. */
    u8 pad240[0x24];
    u32 unk264;
    s32 unk268;
    u8 pad26C[0x38];
    s32 unk2A4;
    u8 pad2A8[0x210];
    struct SoundResourceNode *soundResourceSlots[BTL_SOUND_ENTRY_COUNT]; /* 0x4B8 */
    u8 pad57C[8];
    u8 fadeEnabled; /* 0x584; 0 raises the tint, 1 lowers it. */
    u8 pad585[3];
    u32 fadeColor; /* 0x588; packed tint used by actor rendering. */
    u8 pad58C[0x58];
    s32 (*hook5E4)(BtlUnit *); /* Actor-state predicate for defeat transition. */
    s32 (*hook5E8)(BtlUnit *); /* Fallback predicate when hook5E4 returns 0. */
} BtlActorWork;

typedef char BtlActorWorkExtent[(sizeof(BtlActorWork) == 0x5EC) ? 1 : -1];
typedef char BtlActorWorkActorListOffset[((unsigned int)&((BtlActorWork *)0)->actorList == 0x228) ? 1 : -1];
typedef char BtlActorWorkTaskTailOffset[((unsigned int)&((BtlActorWork *)0)->taskTail == 0x22C) ? 1 : -1];
typedef char BtlActorWorkTaskHeadOffset[((unsigned int)&((BtlActorWork *)0)->taskHead == 0x230) ? 1 : -1];
typedef char BtlActorWorkResourceHeadOffset[
    ((unsigned int)&((BtlActorWork *)0)->soundResourceHead == 0x234) ? 1 : -1
];
typedef char BtlActorWorkActiveSoundOffset[((unsigned int)&((BtlActorWork *)0)->soundList == 0x238) ? 1 : -1];
typedef char BtlActorWorkSlotOwnerOffset[
    ((unsigned int)&((BtlActorWork *)0)->soundSlotOwners == 0x23C) ? 1 : -1
];
typedef char BtlActorWorkResourceSlotsOffset[
    ((unsigned int)&((BtlActorWork *)0)->soundResourceSlots == 0x4B8) ? 1 : -1
];
typedef char BtlActorWorkFadeEnabledOffset[
    ((unsigned int)&((BtlActorWork *)0)->fadeEnabled == 0x584) ? 1 : -1
];
typedef char BtlActorWorkFadeColorOffset[((unsigned int)&((BtlActorWork *)0)->fadeColor == 0x588) ? 1 : -1];
typedef char BtlActorWorkHookOffset[((unsigned int)&((BtlActorWork *)0)->hook5E4 == 0x5E4) ? 1 : -1];
typedef char BtlActorWorkFallbackHookOffset[((unsigned int)&((BtlActorWork *)0)->hook5E8 == 0x5E8) ? 1 : -1];

#endif
