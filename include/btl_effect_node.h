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
    s128 vec0;       /* 0x00 */
    f32 f10;         /* 0x10 */
    f32 f14;         /* 0x14 */
    f32 f18;         /* 0x18 */
    f32 f1C;         /* 0x1C */
    f32 f20;         /* 0x20 */
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
typedef char BtlEffectResource_nodes_offset_must_be_0x2C[
    ((u32)&((BtlEffectResource *)0)->nodes == 0x2C) ? 1 : -1];
typedef char BtlEffectResource_size_must_be_0x40[
    (sizeof(BtlEffectResource) == 0x40) ? 1 : -1];

#endif /* BTL_EFFECT_NODE_H */
