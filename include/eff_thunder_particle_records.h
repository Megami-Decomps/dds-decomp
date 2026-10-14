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

/* Copied spark constructor input; gaps remain opaque without stronger evidence. */
typedef struct EffThunderSparkParams {
    f32 origin[4];                   /* 0x00 */
    f32 loweredPosition[4];          /* 0x10 */
    f32 position[4];                 /* 0x20 */
    u16 systemParam;                 /* 0x30 */
    u8 pad32[0x0A];
    f32 waveAmplitude;               /* 0x3C */
    u8 pad40[8];
    u16 halfLife;                    /* 0x48 */
    u8 pad4A[2];
    f32 width;                       /* 0x4C */
    void *dispatchArg;               /* 0x50 */
    u8 pad54[0x10];
    f32 heightOffset;                /* 0x64 */
    u32 sparkCount;                  /* 0x68 */
    u8 loop;                         /* 0x6C */
    u8 pad6D[3];
    s32 duration;                    /* 0x70 */
    s32 startDelaySpread;            /* 0x74 */
    s32 fadeInTime;                  /* 0x78 */
    s32 fadeOutTime;                 /* 0x7C */
    f32 orbitRadius;                 /* 0x80 */
    f32 radiusRandomness;            /* 0x84 */
    f32 heightOffsetRange;           /* 0x88 */
    f32 angularSpeed;                /* 0x8C */
    f32 angularSpeedRandomness;      /* 0x90 */
    f32 angularDamping;              /* 0x94 */
    f32 verticalSpeed;               /* 0x98 */
    f32 verticalSpeedRandomness;     /* 0x9C */
    f32 verticalDamping;             /* 0xA0 */
} EffThunderSparkParams;

/* Allocation header followed by sparkCount records of EffThunderSpark. */
typedef struct EffThunderSparkWork {
    EffThunderSparkParams head;      /* 0x00 */
    EffThunderSpark *sparks;         /* 0xA4 */
    u32 tintColor;                   /* 0xA8 */
    SdfMemBlock *allocationHandle;   /* 0xAC */
} EffThunderSparkWork;

typedef char EffThunderFragSizeCheck[
    (sizeof(EffThunderFrag) == 0x0C) ? 1 : -1];
typedef char EffThunderFragColorOffsetCheck[
    ((u32)&((EffThunderFrag *)0)->color == 0x08) ? 1 : -1];
typedef char EffThunderSparkSizeCheck[
    (sizeof(EffThunderSpark) == 0x20) ? 1 : -1];
typedef char EffThunderSparkSystemOffsetCheck[
    ((u32)&((EffThunderSpark *)0)->system == 0x1C) ? 1 : -1];
typedef char EffThunderSparkParamsSizeCheck[
    (sizeof(EffThunderSparkParams) == 0xA4) ? 1 : -1];
typedef char EffThunderSparkParamsSparkCountOffsetCheck[
    ((u32)&((EffThunderSparkParams *)0)->sparkCount == 0x68) ? 1 : -1];
typedef char EffThunderSparkWorkSizeCheck[
    (sizeof(EffThunderSparkWork) == 0xB0) ? 1 : -1];
typedef char EffThunderSparkWorkSparksOffsetCheck[
    ((u32)&((EffThunderSparkWork *)0)->sparks == 0xA4) ? 1 : -1];
typedef char EffThunderSparkWorkAllocationOffsetCheck[
    ((u32)&((EffThunderSparkWork *)0)->allocationHandle == 0xAC) ? 1 : -1];

#endif
