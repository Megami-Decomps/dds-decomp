#ifndef MNU_SHOOTING_H
#define MNU_SHOOTING_H

#include "common.h"
#include "mnu_fade.h"

struct SdfMemBlock;
struct MnuSectionObjectList;
struct MdlCtx;
struct FileQueue;
struct FileRequest;
struct WideSlotPool;
struct CompactSlotPool;

/* Records are 0x34 bytes; bit 0 of flags marks a claimed slot. */
typedef struct ModelInstance {
    f32 position[3];
    u32 valueC;
    f32 positionStep[3];
    u8 pad1C[4];
    u32 flags;
    u16 remainingLifetime; /* decremented by each unpaused timed update */
    u16 initialLifetime; /* retained as the draw ratio denominator; zero disables timed motion */
    s16 animationFrame;
    s16 animationLength;
    u8 pad2C[4];
    f32 scale;
} ModelInstance;

typedef struct ModelInstanceList {
    ModelInstance *items;
    s32 count;
} ModelInstanceList;

typedef struct ModelInstanceWork {
    struct SdfMemBlock *allocation;
    s32 count;
    ModelInstanceList *lists;
    u32 unkC;
} ModelInstanceWork;

typedef char ModelInstanceLayoutsAssert[
    (sizeof(ModelInstance) == 0x34 &&
     sizeof(ModelInstanceList) == 0x08 &&
     sizeof(ModelInstanceWork) == 0x10 &&
     (unsigned long)&((ModelInstanceWork *)0)->allocation == 0x00 &&
     (unsigned long)&((ModelInstance *)0)->flags == 0x20 &&
     (unsigned long)&((ModelInstance *)0)->scale == 0x30 &&
     (unsigned long)&((ModelInstanceWork *)0)->lists == 0x08) ? 1 : -1];

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

typedef struct MnuEffectPositionStep {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} MnuEffectPositionStep;

/* The effect-work allocator lays out 0x20-byte queue records after its lists. */
typedef struct MnuEffectRecord {
    struct FileQueue *queue;
    u32 flags;
    s32 delay;
    u32 unkC;
    MnuEffectPositionStep positionStep;
} MnuEffectRecord;

typedef struct MnuEffectList {
    MnuEffectRecord *records;
    s32 count;
} MnuEffectList;

typedef struct MnuEffectWork {
    struct SdfMemBlock *allocation;
    s32 count;
    MnuEffectList *lists;
    u32 unkC;
} MnuEffectWork;

typedef char MnuEffectLayoutsAssert[
    (sizeof(MnuEffectPositionStep)==0x10 &&
     sizeof(MnuEffectRecord)==0x20 &&
     (unsigned long)&((MnuEffectRecord*)0)->queue==0 &&
     (unsigned long)&((MnuEffectRecord*)0)->flags==4 &&
     (unsigned long)&((MnuEffectRecord*)0)->delay==8 &&
     (unsigned long)&((MnuEffectRecord*)0)->positionStep==0x10 &&
     sizeof(MnuEffectList)==8 &&
     (unsigned long)&((MnuEffectWork *)0)->allocation==0 &&
     sizeof(MnuEffectWork)==0x10)?1:-1];

MnuEffectRecord *mnuClaimPositionedEffectRecord(MnuEffectList *, f32, f32, f32,
    MnuEffectPositionStep *, f32, s32);
MnuEffectRecord *mnuStartPositionedEffectRecord(MnuEffectList *, s32, f32, f32, f32);

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
    struct ModelInstanceWork *work24;
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
    struct FileQueue *queueCopies[3]; /* 0x4C: copied queue views retained from the request */
    s32 state;
    s32 (*initialize)(u8 *work);
    s32 (*update)(u8 *work);
    struct FileRequest *packageRequest; /* 0x64 */
    u32 unk68;
    u32 unk6C;
    /* The pause bitfield and round transitions share this complete flag word. */
    union {
        u32 flags;
        struct {
            u32 unk70Bit0 : 1;
            u32 initialized : 1;
            u32 unk70Rest : 30;
        };
    };
    s32 currentScore;               /* 0x74 */
    s32 peakScore;
    s32 pendingScore;
    s32 progress;
    s32 step;
    s16 completed;
    s16 countdown;
    s32 updateCount;
    s32 unk90;                     /* integer input tilt; arithmetic uses floats */
    s16 unk94;                     /* selected mode copied from menu work flags */
    s16 round;                      /* 0x96 */
    s16 phase;
    s16 result;
    s32 phaseTicks;
    u8 padA0[8];
    s32 unkA8; /* 0xA8: func_0031A0B8 switch state. */
    s32 unkAC; /* 0xAC: func_0031A0B8 per-state tick. */
    s32 unkB0;
    FadeEntry strip;
    FadeNumber lowerNumber;
    FadeNumber upperNumber;
    FadeGauge gauge;
    FadeNumber pauseFade;             /* 0x14C: pause-menu choice glyphs. */
    /* SDK fade payloads: round number, score triplet, and choice renderers. */
    FadeNumber roundFade;             /* 0x168 */
    FadeEntry scoreFade;             /* 0x184 */
    FadeNumber choiceFade;            /* 0x19C */
    FadeNumber frame;
    s32 choiceIndex;                /* 0x1D4 */
    u16 unk1D8;
    u8 pad1DA[2];
    struct FileRequest *packageDataRequest; /* 0x1DC */
} MnuShootingWork;

typedef char MnuShootingWork_size_must_be_0x1E0[
    (sizeof(MnuShootingWork) == 0x1E0) ? 1 : -1];

typedef char ShootingPackageOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->queueCopies == 0x4C &&
     (unsigned long)&((MnuShootingWork *)0)->packageRequest == 0x64 &&
     (unsigned long)&((MnuShootingWork *)0)->packageDataRequest == 0x1DC) ? 1 : -1];

typedef char ShootingInputOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->flags == 0x70 &&
     (unsigned long)&((MnuShootingWork *)0)->unk90 == 0x90 &&
     (unsigned long)&((MnuShootingWork *)0)->unk94 == 0x94) ? 1 : -1];
typedef char ShootingCallbackOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->unkA8 == 0xA8 &&
     (unsigned long)&((MnuShootingWork *)0)->unkAC == 0xAC) ? 1 : -1];

typedef char ShootingPoolOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->spriteWork == 0x28 &&
     (unsigned long)&((MnuShootingWork *)0)->tintWork == 0x2C) ? 1 : -1];
typedef char ShootingPauseFadeOffsetsAssert[
    ((unsigned long)&((MnuShootingWork *)0)->pauseFade == 0x14C &&
     (unsigned long)&((MnuShootingWork *)0)->pauseFade.displayValue == 0x164) ? 1 : -1];
typedef char ShootingFadeOffsetsAssert[((unsigned long)&((MnuShootingWork*)0)->strip==0xB4 && (unsigned long)&((MnuShootingWork*)0)->lowerNumber==0xCC && (unsigned long)&((MnuShootingWork*)0)->upperNumber==0xE8 && (unsigned long)&((MnuShootingWork*)0)->gauge==0x104 && (unsigned long)&((MnuShootingWork*)0)->roundFade==0x168 && (unsigned long)&((MnuShootingWork*)0)->scoreFade==0x184 && (unsigned long)&((MnuShootingWork*)0)->choiceFade==0x19C && (unsigned long)&((MnuShootingWork*)0)->frame==0x1B8 && (unsigned long)&((MnuShootingWork*)0)->fadeResources.fadeSprites==0x34)?1:-1];
#endif /* MNU_SHOOTING_H */
