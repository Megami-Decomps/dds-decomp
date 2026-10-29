#ifndef EFF_PARTICLE_EMITTER_VARIANTS_H
#define EFF_PARTICLE_EMITTER_VARIANTS_H

#include "eff.h"
#include "eff_ballistic_launch_mode.h"

/* Kind-specific state following the shared 0x150-byte emitter prefix. */
typedef struct EffDiscEmitter {
    EffEmitterHead head;
    u8 flag150;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 initialVerticalSpeed;
    f32 phaseStepDegreesPerFrame;
    f32 initialWaveAmplitude;
    f32 waveAmplitudeStep;
    f32 verticalSpeedScalePercent;
    f32 jitterA;     /* 0x170 */
    f32 jitterB;     /* 0x174 */
} EffDiscEmitter;

typedef char EffDiscEmitter_size_check[
    (sizeof(EffDiscEmitter) == 0x178) ? 1 : -1];
typedef char EffDiscEmitter_spread_offset_check[
    ((u32)&((EffDiscEmitter *)0)->spread == 0x154) ? 1 : -1];
typedef char EffDiscEmitter_jitterB_offset_check[
    ((u32)&((EffDiscEmitter *)0)->jitterB == 0x174) ? 1 : -1];

typedef struct EffBallisticEmitter {
    EffEmitterHead head;
    EffBallisticLaunchMode launchMode; /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170 */
} EffBallisticEmitter;

typedef char EffBallisticEmitter_size_check[
    (sizeof(EffBallisticEmitter) == 0x174) ? 1 : -1];
typedef char EffBallisticEmitter_launchMode_offset_check[
    ((u32)&((EffBallisticEmitter *)0)->launchMode == 0x150) ? 1 : -1];
typedef char EffBallisticEmitter_radius_offset_check[
    ((u32)&((EffBallisticEmitter *)0)->radius == 0x15C) ? 1 : -1];

#endif
