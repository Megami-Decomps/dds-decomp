#ifndef EFF_MAGATUHI_ORBIT_H
#define EFF_MAGATUHI_ORBIT_H

#include "eff.h"

/* Orbit parameters copied into the work record; trailing bytes stay opaque. */
typedef struct EffMagatuhiOrbitParams {
    f32 origin[4];
    u8 respawn;            /* 0x10 */
    u8 pad11[3];
    s32 lifetimeFrames;    /* 0x14 */
    s32 delaySpread;       /* 0x18 */
    s32 fadeIn;            /* 0x1C */
    s32 fadeOut;           /* 0x20 */
    f32 angleStep;         /* 0x24 */
    f32 angleJitter;       /* 0x28 */
    f32 heightStep;        /* 0x2C */
    f32 heightJitter;      /* 0x30 */
    f32 startRadius;       /* 0x34 */
    f32 endRadius;         /* 0x38 */
    f32 startJitter;       /* 0x3C */
    f32 endJitter;         /* 0x40 */
    u32 particleCount;     /* 0x44 */
    u32 maxSteps;          /* 0x48 */
    u8 pad4C[0x94];
} EffMagatuhiOrbitParams;

/* One particle slot; replay visits the first slot in each triplet. */
typedef struct EffMagatuhiOrbitParticle {
    s32 age;        /* 0x00: negative while waiting; otherwise elapsed frames */
    f32 height;     /* 0x04 */
    f32 heightStep; /* 0x08 */
    f32 angle;      /* 0x0C */
    f32 angleStep;  /* 0x10 */
    f32 radius;     /* 0x14 */
    f32 radiusStep; /* 0x18 */
} EffMagatuhiOrbitParticle;

/* Work is followed by the orbit slots in the same retained allocation. */
typedef struct EffMagatuhiOrbitWork {
    f32 matrix[16];
    EffMagatuhiOrbitParams head;
    EffMagatuhiOrbitParticle *particles;
    EffMagatuhiOwner *managedResource;
    u32 tintColor;
    SdfMemBlock *allocationHandle;
} EffMagatuhiOrbitWork;

typedef char EffMagatuhiOrbitParamsSizeCheck[
    (sizeof(EffMagatuhiOrbitParams) == 0xE0) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsOriginOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->origin == 0x00) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsRespawnOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->respawn == 0x10) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsLifetimeOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->lifetimeFrames == 0x14) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsDelayOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->delaySpread == 0x18) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsFadeInOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->fadeIn == 0x1C) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsFadeOutOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->fadeOut == 0x20) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsAngleStepOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->angleStep == 0x24) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsAngleJitterOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->angleJitter == 0x28) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsHeightStepOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->heightStep == 0x2C) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsHeightJitterOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->heightJitter == 0x30) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsStartRadiusOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->startRadius == 0x34) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsEndRadiusOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->endRadius == 0x38) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsStartJitterOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->startJitter == 0x3C) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsEndJitterOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->endJitter == 0x40) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsCountOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->particleCount == 0x44) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsMaxStepsOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->maxSteps == 0x48) ? 1 : -1];
typedef char EffMagatuhiOrbitParamsOpaqueTailOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParams *)0)->pad4C == 0x4C) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleSizeCheck[
    (sizeof(EffMagatuhiOrbitParticle) == 0x1C) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleAgeOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->age == 0x00) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleHeightOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->height == 0x04) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleHeightStepOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->heightStep == 0x08) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleAngleOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->angle == 0x0C) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleAngleStepOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->angleStep == 0x10) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleRadiusOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->radius == 0x14) ? 1 : -1];
typedef char EffMagatuhiOrbitParticleRadiusStepOffsetCheck[
    ((u32)&((EffMagatuhiOrbitParticle *)0)->radiusStep == 0x18) ? 1 : -1];
typedef char EffMagatuhiOrbitWorkSizeCheck[
    (sizeof(EffMagatuhiOrbitWork) == 0x130) ? 1 : -1];
typedef char EffMagatuhiOrbitWorkHeadOffsetCheck[
    ((u32)&((EffMagatuhiOrbitWork *)0)->head == 0x40) ? 1 : -1];
typedef char EffMagatuhiOrbitWorkParticleOffsetCheck[
    ((u32)&((EffMagatuhiOrbitWork *)0)->particles == 0x120) ? 1 : -1];
typedef char EffMagatuhiOrbitWorkOwnerOffsetCheck[
    ((u32)&((EffMagatuhiOrbitWork *)0)->managedResource == 0x124) ? 1 : -1];
typedef char EffMagatuhiOrbitWorkTintOffsetCheck[
    ((u32)&((EffMagatuhiOrbitWork *)0)->tintColor == 0x128) ? 1 : -1];
typedef char EffMagatuhiOrbitWorkAllocationOffsetCheck[
    ((u32)&((EffMagatuhiOrbitWork *)0)->allocationHandle == 0x12C) ? 1 : -1];

#endif /* EFF_MAGATUHI_ORBIT_H */
