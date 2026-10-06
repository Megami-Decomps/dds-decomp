#ifndef MNU_SHOOTING_H
#define MNU_SHOOTING_H

#include "common.h"
#include "mnu_fade.h"

struct SdfMemBlock;
struct MnuSectionObjectList;
struct MdlCtx;
struct MnuEffectWork;
struct WideSlotPool;
struct CompactSlotPool;

/* Nodes passed to the menu model helpers are complete 0x50-byte records. */
typedef struct MnuModelNode {
    f32 primary[4];
    f32 rotationQuaternion[4];
    f32 tertiary[4];
    f32 x;
    f32 y;
    f32 z;
    u32 positionFlag;
    struct MdlCtx *model;
    u32 flags;
    u16 value48;
    u16 value4A;
    f32 savedModelValue;
} MnuModelNode;

typedef char MnuModelNodeLayoutAssert[
    (sizeof(MnuModelNode)==0x50 &&
     (unsigned long)&((MnuModelNode*)0)->model==0x40 &&
     (unsigned long)&((MnuModelNode*)0)->savedModelValue==0x4C)?1:-1];

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
    union {
        u32 resourceSlots[7];
        struct {
            u32 slot30;
            struct EffectSlotSet *fadeSprites;
            u32 remainingSlots[5];
        } fadeResources;
    };
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
    FadeEntry strip;
    FadeNumber lowerNumber;
    FadeNumber upperNumber;
    FadeGauge gauge;
    u8 pad14C[0x1C];
    /* SDK fade payloads: round number, score triplet, and choice renderers. */
    FadeNumber roundFade;             /* 0x168 */
    FadeEntry scoreFade;             /* 0x184 */
    FadeNumber choiceFade;            /* 0x19C */
    FadeNumber frame;
    s32 choiceIndex;                /* 0x1D4 */
    u16 unk1D8;
    u8 pad1DA[6];
} MnuShootingWork;

typedef char MnuShootingWork_size_must_be_0x1E0[
    (sizeof(MnuShootingWork) == 0x1E0) ? 1 : -1];

typedef char ShootingPoolOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->spriteWork == 0x28 &&
     (unsigned long)&((MnuShootingWork *)0)->tintWork == 0x2C) ? 1 : -1];
typedef char ShootingFadeOffsetsAssert[((unsigned long)&((MnuShootingWork*)0)->strip==0xB4 && (unsigned long)&((MnuShootingWork*)0)->lowerNumber==0xCC && (unsigned long)&((MnuShootingWork*)0)->upperNumber==0xE8 && (unsigned long)&((MnuShootingWork*)0)->gauge==0x104 && (unsigned long)&((MnuShootingWork*)0)->roundFade==0x168 && (unsigned long)&((MnuShootingWork*)0)->scoreFade==0x184 && (unsigned long)&((MnuShootingWork*)0)->choiceFade==0x19C && (unsigned long)&((MnuShootingWork*)0)->frame==0x1B8 && (unsigned long)&((MnuShootingWork*)0)->fadeResources.fadeSprites==0x34)?1:-1];
#endif /* MNU_SHOOTING_H */
