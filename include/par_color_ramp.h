#ifndef PAR_COLOR_RAMP_H
#define PAR_COLOR_RAMP_H

#include "common.h"

/* Per-emitter color ramp initialized by parInitColorRamp. */
typedef struct ParColorRamp {
    s32 mode;          /* 0x00: 1 = no ramp, 2 = color0-to-color1 only */
    u32 color0;        /* 0x04 */
    u32 color1;        /* 0x08 */
    u32 color2;        /* 0x0C */
    u32 alpha;         /* 0x10 */
    s32 fadeInFrames;  /* 0x14 */
    s32 fadeOutFrames; /* 0x18 */
    s32 frames;        /* 0x1C */
    s32 rampFrames;    /* 0x20 */
    f32 r01;           /* 0x24 */
    f32 r12;           /* 0x28 */
    f32 g01;           /* 0x2C */
    f32 g12;           /* 0x30 */
    f32 b01;           /* 0x34 */
    f32 b12;           /* 0x38 */
    u32 fadeIn;        /* 0x3C */
    u32 fadeOut;       /* 0x40 */
} ParColorRamp;

typedef char ParColorRamp_size_must_be_0x44[
    (sizeof(ParColorRamp) == 0x44) ? 1 : -1];
typedef char ParColorRamp_r01_at_0x24[
    ((u32)&((ParColorRamp *)0)->r01 == 0x24) ? 1 : -1];
typedef char ParColorRamp_fadeOut_at_0x40[
    ((u32)&((ParColorRamp *)0)->fadeOut == 0x40) ? 1 : -1];

#endif
