#ifndef EFF_THUNDER_PARTICLE_RECORDS_H
#define EFF_THUNDER_PARTICLE_RECORDS_H

#include "eff.h"

/* Per-fragment countdowns and the packed color consumed by thunder updates. */
typedef struct EffThunderFrag {
    u32 delayFrames;  /* 0x00 */
    u32 activeFrames; /* 0x04 */
    u32 color;        /* 0x08 */
} EffThunderFrag;

/* One orbiting spark and its independently owned particle cell system. */
typedef struct EffThunderSpark {
    s32 age;              /* 0x00 */
    f32 verticalOffset;   /* 0x04 */
    f32 verticalSpeed;    /* 0x08 */
    f32 orbitAngle;       /* 0x0C */
    f32 angularSpeed;     /* 0x10 */
    f32 orbitRadius;      /* 0x14 */
    f32 heightOffset;     /* 0x18 */
    ParSystem *system;    /* 0x1C */
} EffThunderSpark;

typedef char EffThunderFragSizeCheck[
    (sizeof(EffThunderFrag) == 0x0C) ? 1 : -1];
typedef char EffThunderFragColorOffsetCheck[
    ((u32)&((EffThunderFrag *)0)->color == 0x08) ? 1 : -1];
typedef char EffThunderSparkSizeCheck[
    (sizeof(EffThunderSpark) == 0x20) ? 1 : -1];
typedef char EffThunderSparkSystemOffsetCheck[
    ((u32)&((EffThunderSpark *)0)->system == 0x1C) ? 1 : -1];

#endif
