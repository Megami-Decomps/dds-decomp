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

typedef char BtlEffectNode_size_must_be_0x14[
    (sizeof(BtlEffectNode) == 0x14) ? 1 : -1];
typedef char BtlEffectNode_scale_offset_must_be_0x08[
    ((u32)&((BtlEffectNode *)0)->scale == 0x08) ? 1 : -1];
typedef char BtlEffectNode_reachOffset_offset_must_be_0x0C[
    ((u32)&((BtlEffectNode *)0)->reachOffset == 0x0C) ? 1 : -1];
typedef char BtlEffectNode_frameCount_offset_must_be_0x12[
    ((u32)&((BtlEffectNode *)0)->frameCount == 0x12) ? 1 : -1];

#endif /* BTL_EFFECT_NODE_H */
