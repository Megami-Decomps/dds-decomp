#ifndef EFF_PCP_SHARED_TRAIL_H
#define EFF_PCP_SHARED_TRAIL_H

#include "common.h"

struct EffBlurTemplate;

/* Shared by ref-counted trail tasks; constructors allocate this full record. */
typedef struct EffPCPSharedTrail {
    f32 pos[4];
    u32 flags;
    u32 color;
    u32 frame;
    f32 sizeInput; /* 0x1C: converted to the rendered extent by the camera helper. */
    u32 limit;
    u32 unk24;
    u32 colors[2];
    f32 scale;
    s32 minSize;
    struct EffBlurTemplate *obj;
} EffPCPSharedTrail;

typedef char EffPCPSharedTrail_size_must_be_0x3C[
    (sizeof(EffPCPSharedTrail) == 0x3C) ? 1 : -1];
typedef char EffPCPSharedTrail_flags_offset_must_be_0x10[
    ((u32)&((EffPCPSharedTrail *)0)->flags == 0x10) ? 1 : -1];
typedef char EffPCPSharedTrail_sizeInput_offset_must_be_0x1C[
    ((u32)&((EffPCPSharedTrail *)0)->sizeInput == 0x1C) ? 1 : -1];
typedef char EffPCPSharedTrail_colors_offset_must_be_0x28[
    ((u32)&((EffPCPSharedTrail *)0)->colors == 0x28) ? 1 : -1];
typedef char EffPCPSharedTrail_obj_offset_must_be_0x38[
    ((u32)&((EffPCPSharedTrail *)0)->obj == 0x38) ? 1 : -1];

#endif
