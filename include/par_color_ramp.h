#ifndef PAR_COLOR_RAMP_H
#define PAR_COLOR_RAMP_H

#include "common.h"

/* Per-emitter color ramp initialized by dds3ParticleSetColorTable. */
typedef struct ParColorRamp {
    s32 mode;          /* 0x00: 1 = no ramp, 2 = color0-to-color1 only */
    u32 color[3];      /* 0x04: ramp stops 0, 1, 2 */
    u32 alpha;         /* 0x10 */
    s32 fadeInFrames;  /* 0x14 */
    s32 fadeOutFrames; /* 0x18 */
    s32 frames;        /* 0x1C */
    s32 rampFrames;    /* 0x20 */
    f32 rStep[2];      /* 0x24: per-frame red step, segments 0-1 and 1-2 */
    f32 gStep[2];      /* 0x2C */
    f32 bStep[2];      /* 0x34 */
    u32 fadeIn;        /* 0x3C */
    u32 fadeOut;       /* 0x40 */
} ParColorRamp;

/* Evaluate a color through the emitter ramp at the particle's current age. */
u32 dds3ParticleColorAnim(ParColorRamp *colorRamp, u32 color, s32 age);

typedef char ParColorRamp_size_must_be_0x44[
    (sizeof(ParColorRamp) == 0x44) ? 1 : -1];
typedef char ParColorRamp_rStep_at_0x24[
    ((u32)&((ParColorRamp *)0)->rStep == 0x24) ? 1 : -1];
typedef char ParColorRamp_fadeOut_at_0x40[
    ((u32)&((ParColorRamp *)0)->fadeOut == 0x40) ? 1 : -1];

#endif
