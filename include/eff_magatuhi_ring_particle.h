#ifndef EFF_MAGATUHI_RING_PARTICLE_H
#define EFF_MAGATUHI_RING_PARTICLE_H

#include "common.h"

/* Per-particle state allocated after EffMagatuhiRingWork by both titles. */
typedef struct EffMagatuhiRingParticle {
    s32 age;        /* 0x00: negative while waiting; otherwise elapsed frames */
    f32 height;     /* 0x04 */
    f32 angle;      /* 0x08 */
    f32 angleStep;  /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
} EffMagatuhiRingParticle;

typedef char EffMagatuhiRingParticleSizeCheck[
    (sizeof(EffMagatuhiRingParticle) == 0x18) ? 1 : -1];
typedef char EffMagatuhiRingParticleAgeOffsetCheck[
    ((u32)&((EffMagatuhiRingParticle *)0)->age == 0x00) ? 1 : -1];
typedef char EffMagatuhiRingParticleHeightOffsetCheck[
    ((u32)&((EffMagatuhiRingParticle *)0)->height == 0x04) ? 1 : -1];
typedef char EffMagatuhiRingParticleAngleOffsetCheck[
    ((u32)&((EffMagatuhiRingParticle *)0)->angle == 0x08) ? 1 : -1];
typedef char EffMagatuhiRingParticleAngleStepOffsetCheck[
    ((u32)&((EffMagatuhiRingParticle *)0)->angleStep == 0x0C) ? 1 : -1];
typedef char EffMagatuhiRingParticleRadiusOffsetCheck[
    ((u32)&((EffMagatuhiRingParticle *)0)->radius == 0x10) ? 1 : -1];
typedef char EffMagatuhiRingParticleRadiusStepOffsetCheck[
    ((u32)&((EffMagatuhiRingParticle *)0)->radiusStep == 0x14) ? 1 : -1];

#endif /* EFF_MAGATUHI_RING_PARTICLE_H */
