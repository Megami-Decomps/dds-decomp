#ifndef EFF_PCP_COMPACT_WORK_H
#define EFF_PCP_COMPACT_WORK_H

#include "eff_blur.h"

/* Scatter and scale callbacks share this owner; creation selects the resource
 * view while all timeline and color state stays in the common prefix. */
typedef struct EffPCPCompactWork {
    f32 position[4];
    u8 flags;
    u8 pad11[3];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u32 baseColor;
    s32 startExtent;
    s32 endExtent;
    s32 frame;
    union {
        EffBlurScatterWork *scatter;
        EffBlurScaleWork *scale;
    } resource;
} EffPCPCompactWork;

typedef char EffPCPCompactWork_size_must_be_0x38[
    (sizeof(EffPCPCompactWork) == 0x38) ? 1 : -1];
typedef char EffPCPCompactWork_duration_offset_must_be_0x14[
    ((u32)&((EffPCPCompactWork *)0)->duration == 0x14) ? 1 : -1];
typedef char EffPCPCompactWork_color_offset_must_be_0x20[
    ((u32)&((EffPCPCompactWork *)0)->color == 0x20) ? 1 : -1];
typedef char EffPCPCompactWork_frame_offset_must_be_0x30[
    ((u32)&((EffPCPCompactWork *)0)->frame == 0x30) ? 1 : -1];
typedef char EffPCPCompactWork_resource_offset_must_be_0x34[
    ((u32)&((EffPCPCompactWork *)0)->resource == 0x34) ? 1 : -1];

#endif /* EFF_PCP_COMPACT_WORK_H */
