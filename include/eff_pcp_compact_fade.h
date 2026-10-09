#ifndef EFF_PCP_COMPACT_FADE_H
#define EFF_PCP_COMPACT_FADE_H

#include "eff.h"
#include "eff_blur.h"

/* Both compact-fade lifecycles use the same 0x3C chip allocation. The
 * resource union is selected by the short rectangle or long textured-blur
 * constructor and each path has its own matching release routine. */
typedef struct EffPCPCompactFadeWork {
    f32 position[4];
    u8 flags;
    u8 pad11[3];
    u32 color;
    u32 baseColor;
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 startExtent;
    s32 endExtent;
    f32 unk34;
    union {
        EffResourceRectWork *rectangle;
        EffBlurTemplate *blur;
    } resource;
} EffPCPCompactFadeWork;

typedef char EffPCPCompactFadeWork_size_must_be_0x3C[
    (sizeof(EffPCPCompactFadeWork) == 0x3C) ? 1 : -1];
typedef char EffPCPCompactFadeWork_flags_offset_must_be_0x10[
    ((u32)&((EffPCPCompactFadeWork *)0)->flags == 0x10) ? 1 : -1];
typedef char EffPCPCompactFadeWork_frame_offset_must_be_0x1C[
    ((u32)&((EffPCPCompactFadeWork *)0)->frame == 0x1C) ? 1 : -1];
typedef char EffPCPCompactFadeWork_unk34_offset_must_be_0x34[
    ((u32)&((EffPCPCompactFadeWork *)0)->unk34 == 0x34) ? 1 : -1];
typedef char EffPCPCompactFadeWork_resource_offset_must_be_0x38[
    ((u32)&((EffPCPCompactFadeWork *)0)->resource == 0x38) ? 1 : -1];

#endif /* EFF_PCP_COMPACT_FADE_H */
