#ifndef EFF_PCP_BOSS_H
#define EFF_PCP_BOSS_H

#include "common.h"
#include "eff.h"
#include "eff_param.h"

/* Boss effect work: two parameter-set handles released on free. */
typedef struct {
    f32 position[4]; /* 0x00 */
    u32 parameterVector[4]; /* 0x10 copied from the parameter block */
    s32 frame;      /* 0x20 counts updates */
    u32 color;      /* 0x24 set by effPCPBossSetParameter, starts 0x80808080 */
    EffParamWork *modelResource; /* Kind 3: supplies the model effect. */
    EffParamWork *auxiliaryResource; /* Kind 6: receives the same basis, vector and tint. */
} EffPCPBossWork;

/* Trail parameters copied verbatim on spawn. Trail colors use the owner frame;
   cell colors and offset ramping use each cell's initially non-positive age. */
typedef struct {
    f32 position[4];
    u8 drawTrail;
    u8 pad11[3];
    f32 frameStep;    /* Initial step for the model motion at +0x20. */
    f32 scale;        /* Applied to model scale and randomized cell extents. */
    f32 trailWidth;
    s32 trailColorStartFrame;
    s32 trailColorTransitionFrames;
    u32 trailStartCenterColor;
    u32 trailStartEdgeColor;
    u32 trailEndCenterColor;
    u32 trailEndEdgeColor;
    u16 drawBucket;  /* 0x38 */
    u8 pad3A[2];
    u8 hasCells;      /* 0x3C */
    u8 cellCountFromFrame; /* Mode 1 uses the owner frame as the active sample count. */
    u8 pad3E[2];
    s32 cellStartFrame;
    u32 delaySpread;
    s32 cellDuration;
    s32 cellFadeIn;
    s32 cellFadeOut;
    s32 offsetRampFrames;
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    s32 cellColorStartAge;
    s32 cellColorTransitionFrames;
    u32 cellStartCenterColor;
    u32 cellStartEdgeColor;
    u32 cellEndCenterColor;
    u32 cellEndEdgeColor;
    u32 incrementBits; /* 0x84 */
    u8 directionMode; /* 0: negative Y; 1: positive Y; 2: model-relative direction. */
    u8 pad89[3];
} EffBossHead; /* 0x8C */

/* Parameter block as read by effBossCreateWithGroups: two words follow the head. */
typedef struct {
    EffBossHead head;
    s32 rotationStartAge;
    f32 angularSpeed;
} EffBossParams;

/* Random offset distance and the extents at the base/tip of a trail cell.
   Both extents share one random factor; age starts at a non-positive delay. */
typedef struct {
    f32 offsetDistance;
    f32 baseExtent;
    f32 tipExtent;
    s32 age;
    u8 flip;          /* 0x10 */
    u8 pad11[3];
} EffBossCell; /* 0x14 */

typedef struct {
    EffRecordPool *drawPool;
    f32 direction[3]; /* Unit Y initially; mode 2 refreshes it from the model. */
    u8 pad10[4];
    f32 rotationAngle;
    s32 rotationStartAge;
    f32 angularSpeed;
    EffBossCell *cells; /* 0x20 */
} EffBossGroup; /* 0x24 */

typedef struct {
    EffBossHead head;
    EffBossGroup *groups; /* 0x8C */
    SdfMemBlock *groupsHandle; /* 0x90 */
    u32 groupCount;   /* 0x94 */
    u16 cellCount;    /* 0x98 */
    u8 pad9A[2];
    u32 frame;
    u32 color;        /* 0xA0 */
    ParSystem *system; /* 0xA4: allocated cell system */
    EffParamWork *paramWork; /* 0xA8 */
} EffBossWork;

/* Five packed RGBA values paired with the draw pool's five vertex positions.
   Native update writes tinted colors here; these are not vertex indices. */
typedef struct {
    u32 color[5];
} EffBossColorSlot;

typedef char EffPCPBossWork_size_must_be_0x30[(sizeof(EffPCPBossWork) == 0x30) ? 1 : -1];
typedef char EffBossHead_size_must_be_0x8C[(sizeof(EffBossHead) == 0x8C) ? 1 : -1];
typedef char EffBossParams_size_must_be_0x94[(sizeof(EffBossParams) == 0x94) ? 1 : -1];
typedef char EffBossCell_size_must_be_0x14[(sizeof(EffBossCell) == 0x14) ? 1 : -1];
typedef char EffBossGroup_size_must_be_0x24[(sizeof(EffBossGroup) == 0x24) ? 1 : -1];
typedef char EffBossWork_size_must_be_0xAC[(sizeof(EffBossWork) == 0xAC) ? 1 : -1];
typedef char EffBossColorSlot_size_must_be_0x14[(sizeof(EffBossColorSlot) == 0x14) ? 1 : -1];

#endif /* EFF_PCP_BOSS_H */
