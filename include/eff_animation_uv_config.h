#ifndef EFF_ANIMATION_UV_CONFIG_H
#define EFF_ANIMATION_UV_CONFIG_H

#include "common.h"
#include "eff_curve.h"

/* Common UV header of the scroll-animation payload (EffClassWork::payload); the
 * kind-specific serialized file parameters follow it. */
typedef struct EffAnimationUvConfig {
    EffScalarTrack scale;
    f32 uExtent;  // 0x2C
    f32 vExtent;  // 0x30
    f32 vStep;    // 0x34
    f32 uStep;    // 0x38
} EffAnimationUvConfig;
typedef char EffAnimationUvConfig_uExtent_offset_check[
    ((u32)&((EffAnimationUvConfig *)0)->uExtent == 0x2C) ? 1 : -1];

#endif
