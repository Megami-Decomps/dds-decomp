#ifndef SDF_VU_BLEND_NODE_H
#define SDF_VU_BLEND_NODE_H

#include "common.h"

typedef struct VuBlendNode {
    f32 position[4];            /* 0x00: position vector consumed by VU clipping */
    u8 opaque10[0x10];          /* 0x10: only the W lane is used by clipping */
    f32 color[4];               /* 0x20: copied and interpolated as a vector */
    f32 xy[2];                  /* 0x30: interpolated XY pair */
    u8 opaque38[8];             /* 0x38: other lanes are unresolved */
    struct VuBlendNode *next;   /* 0x40 */
    struct VuBlendNode *sourceA; /* 0x44 */
    struct VuBlendNode *sourceB; /* 0x48 */
    f32 weight;                 /* 0x4C */
    u8 region;                  /* 0x50: bit 0 selects the W = 0 clipping path */
    u8 clipFlags;               /* 0x51: clip-plane bits used for triangle culling */
    u8 opaque52[0x0E];          /* 0x52 */
} VuBlendNode;

typedef char VuBlendNode_size_must_be_0x60[
    (sizeof(VuBlendNode) == 0x60) ? 1 : -1];
typedef char VuBlendNode_color_offset_must_be_0x20[
    ((unsigned long)&((VuBlendNode *)0)->color == 0x20) ? 1 : -1];
typedef char VuBlendNode_xy_offset_must_be_0x30[
    ((unsigned long)&((VuBlendNode *)0)->xy == 0x30) ? 1 : -1];
typedef char VuBlendNode_next_offset_must_be_0x40[
    ((unsigned long)&((VuBlendNode *)0)->next == 0x40) ? 1 : -1];
typedef char VuBlendNode_weight_offset_must_be_0x4C[
    ((unsigned long)&((VuBlendNode *)0)->weight == 0x4C) ? 1 : -1];
typedef char VuBlendNode_region_offset_must_be_0x50[
    ((unsigned long)&((VuBlendNode *)0)->region == 0x50) ? 1 : -1];
typedef char VuBlendNode_clipFlags_offset_must_be_0x51[
    ((unsigned long)&((VuBlendNode *)0)->clipFlags == 0x51) ? 1 : -1];

#endif /* SDF_VU_BLEND_NODE_H */
