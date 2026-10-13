#ifndef EFF_BILL_COLUMN_PARTICLE_H
#define EFF_BILL_COLUMN_PARTICLE_H

#include "common.h"

/* One 0x30-byte particle state row used by the billboard column updater. */
typedef struct EffBillColumnParticle {
    s32 age;             /* 0x00: spawn sentinel or elapsed particle frames */
    u32 opaque04;        /* 0x04: preserved state not read by the updater */
    f32 radius;          /* 0x08: current radial distance */
    f32 radiusStep;      /* 0x0C: radial change per frame */
    f32 drag;            /* 0x10: vertical drag applied per frame */
    f32 verticalOffset;  /* 0x14: current vertical position */
    f32 spin;            /* 0x18: angular speed */
    f32 spinPhase;       /* 0x1C: initial angular phase */
    f32 width;           /* 0x20: ribbon width */
    f32 extent;          /* 0x24: ribbon length along its direction */
    f32 flare;           /* 0x28: radial flare amount */
    f32 height;          /* 0x2C: vertical start height, advanced by rise */
} EffBillColumnParticle;

typedef char EffBillColumnParticleSizeCheck[
    (sizeof(EffBillColumnParticle) == 0x30) ? 1 : -1];
typedef char EffBillColumnParticleAgeOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->age == 0x00) ? 1 : -1];
typedef char EffBillColumnParticleOpaqueOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->opaque04 == 0x04) ? 1 : -1];
typedef char EffBillColumnParticleRadiusOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->radius == 0x08) ? 1 : -1];
typedef char EffBillColumnParticleRadiusStepOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->radiusStep == 0x0C) ? 1 : -1];
typedef char EffBillColumnParticleDragOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->drag == 0x10) ? 1 : -1];
typedef char EffBillColumnParticleVerticalOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->verticalOffset == 0x14) ? 1 : -1];
typedef char EffBillColumnParticleSpinOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->spin == 0x18) ? 1 : -1];
typedef char EffBillColumnParticleSpinPhaseOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->spinPhase == 0x1C) ? 1 : -1];
typedef char EffBillColumnParticleWidthOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->width == 0x20) ? 1 : -1];
typedef char EffBillColumnParticleExtentOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->extent == 0x24) ? 1 : -1];
typedef char EffBillColumnParticleFlareOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->flare == 0x28) ? 1 : -1];
typedef char EffBillColumnParticleHeightOffsetCheck[
    ((u32)&((EffBillColumnParticle *)0)->height == 0x2C) ? 1 : -1];

#endif /* EFF_BILL_COLUMN_PARTICLE_H */
