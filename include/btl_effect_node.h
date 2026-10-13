#ifndef BTL_EFFECT_NODE_H
#define BTL_EFFECT_NODE_H

#include "common.h"

/* Shared motion/approach parameters in a battle effect resource row. */
typedef struct BtlEffectNode {
    s16 triggerKind; /* 0x00 */
    u8 pad02[2];
    s16 rateKind; /* 0x04 */
    u8 pad06[2];
    f32 scale; /* 0x08 */
    f32 reachOffset; /* 0x0C */
    u8 pad10[2];
    u16 frameCount; /* 0x12 */
} BtlEffectNode;

/* Effect-node rows follow the 0x2C-byte variable-length table header. */
typedef struct BtlEffectResource {
    s128 baseOffset;   /* 0x00: copied into the actor body and muzzle offsets. */
    f32 actorScale;    /* 0x10: applied to the actor scale and effect vectors. */
    f32 zOffset;       /* 0x14: applied as the actor position Z offset. */
    f32 reach;         /* 0x18: copied to the actor reach field. */
    f32 height;        /* 0x1C: copied to the actor height field. */
    f32 cameraRadius;  /* 0x20: copied to the actor camera-clearance value. */
    u8 unknown24[8]; /* 0x24: opaque table-header bytes */
    BtlEffectNode nodes[1]; /* 0x2C: variable-length row array */
} BtlEffectResource;

typedef char BtlEffectNode_size_must_be_0x14[
    (sizeof(BtlEffectNode) == 0x14) ? 1 : -1];
typedef char BtlEffectNode_scale_offset_must_be_0x08[
    ((u32)&((BtlEffectNode *)0)->scale == 0x08) ? 1 : -1];
typedef char BtlEffectNode_reachOffset_offset_must_be_0x0C[
    ((u32)&((BtlEffectNode *)0)->reachOffset == 0x0C) ? 1 : -1];
typedef char BtlEffectNode_frameCount_offset_must_be_0x12[
    ((u32)&((BtlEffectNode *)0)->frameCount == 0x12) ? 1 : -1];
typedef char BtlEffectResource_baseOffset_offset_must_be_0x00[
    ((u32)&((BtlEffectResource *)0)->baseOffset == 0x00) ? 1 : -1];
typedef char BtlEffectResource_actorScale_offset_must_be_0x10[
    ((u32)&((BtlEffectResource *)0)->actorScale == 0x10) ? 1 : -1];
typedef char BtlEffectResource_zOffset_offset_must_be_0x14[
    ((u32)&((BtlEffectResource *)0)->zOffset == 0x14) ? 1 : -1];
typedef char BtlEffectResource_reach_offset_must_be_0x18[
    ((u32)&((BtlEffectResource *)0)->reach == 0x18) ? 1 : -1];
typedef char BtlEffectResource_height_offset_must_be_0x1C[
    ((u32)&((BtlEffectResource *)0)->height == 0x1C) ? 1 : -1];
typedef char BtlEffectResource_cameraRadius_offset_must_be_0x20[
    ((u32)&((BtlEffectResource *)0)->cameraRadius == 0x20) ? 1 : -1];
typedef char BtlEffectResource_nodes_offset_must_be_0x2C[
    ((u32)&((BtlEffectResource *)0)->nodes == 0x2C) ? 1 : -1];
typedef char BtlEffectResource_size_must_be_0x40[
    (sizeof(BtlEffectResource) == 0x40) ? 1 : -1];

#endif /* BTL_EFFECT_NODE_H */
