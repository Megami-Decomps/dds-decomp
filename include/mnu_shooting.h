#ifndef MNU_SHOOTING_H
#define MNU_SHOOTING_H

#include "common.h"

struct SdfMemBlock;
struct MnuSectionObjectList;
struct MnuModelNode;
struct MnuEffectWork;
struct WideSlotPool;
struct CompactSlotPool;

/* A model group is a complete 0x10-byte list descriptor. */
typedef struct MnuNodeList {
    struct MnuModelNode *nodes;
    s32 count;
    u8 pad08[8];
} MnuNodeList;

typedef struct MnuSectionModelWork {
    struct SdfMemBlock *allocation;
    s32 count;
    MnuNodeList *groups;
    u32 unkC;
} MnuSectionModelWork;

typedef char MnuNodeList_size_must_be_0x10[
    (sizeof(MnuNodeList) == 0x10) ? 1 : -1];
typedef char MnuSectionModelWork_size_must_be_0x10[
    (sizeof(MnuSectionModelWork) == 0x10) ? 1 : -1];

/* DDS2 shooting task allocation, cleared as 0x1E0 bytes by its constructor. */
typedef struct MnuShootingWork {
    struct SdfMemBlock *allocation;
    struct MnuSectionObjectList *objects;
    struct MnuSectionObjectList *playerObjects;
    struct MnuSectionObjectList *mapObjects;
    struct MnuSectionObjectList *drawObjects;
    struct MnuSectionObjectList *progressWork;
    struct MnuSectionObjectList *alternateProgressWork;
    struct MnuSectionModelWork *modelWork;
    struct MnuEffectWork *effectWork;
    struct MnuSectionObjectList *work24;
    struct WideSlotPool *spriteWork;
    struct CompactSlotPool *tintWork;
    u32 resourceSlots[7];
    u8 pad4C[0xC];
    s32 state;
    s32 (*initialize)(u8 *work);
    s32 (*update)(u8 *work);
    u8 pad64[4];
    u32 unk68;
    u32 unk6C;
    u32 unk70Bit0 : 1;
    u32 initialized : 1;
    u32 unk70Rest : 30;
    s32 currentScore;               /* 0x74 */
    s32 peakScore;
    s32 pendingScore;
    s32 progress;
    s32 step;
    s16 completed;
    s16 countdown;
    s32 updateCount;
    u8 pad90[6];
    s16 round;                      /* 0x96 */
    s16 phase;
    s16 result;
    s32 phaseTicks;
    u8 padA0[0x10];
    s32 unkB0;
    u8 padB4[0xB4];
    /* SDK fade payloads: round number, score triplet, and choice renderers. */
    u8 roundFade[0x1C];             /* 0x168 */
    u8 scoreFade[0x18];             /* 0x184 */
    u8 choiceFade[0x24];            /* 0x19C */
    u8 pad1C0[0x14];
    s32 choiceIndex;                /* 0x1D4 */
    u16 unk1D8;
    u8 pad1DA[6];
} MnuShootingWork;

typedef char MnuShootingWork_size_must_be_0x1E0[
    (sizeof(MnuShootingWork) == 0x1E0) ? 1 : -1];

typedef char ShootingPoolOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->spriteWork == 0x28 &&
     (unsigned long)&((MnuShootingWork *)0)->tintWork == 0x2C) ? 1 : -1];
#endif /* MNU_SHOOTING_H */
