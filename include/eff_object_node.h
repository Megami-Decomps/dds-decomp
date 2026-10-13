#ifndef EFF_OBJECT_NODE_H
#define EFF_OBJECT_NODE_H

#include "common.h"

/* Intrusive model-object list node; the object payload stays opaque here. */
typedef struct EffectObjectNode {
    void *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

typedef char EffectObjectNode_size_must_be_0xC[
    (sizeof(EffectObjectNode) == 0xC) ? 1 : -1];
typedef char EffectObjectNode_prev_offset_must_be_4[
    ((u32)&((EffectObjectNode *)0)->prev == 0x4) ? 1 : -1];
typedef char EffectObjectNode_next_offset_must_be_8[
    ((u32)&((EffectObjectNode *)0)->next == 0x8) ? 1 : -1];

#endif /* EFF_OBJECT_NODE_H */
