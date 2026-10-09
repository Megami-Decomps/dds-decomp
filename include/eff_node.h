#ifndef EFF_NODE_H
#define EFF_NODE_H

#include "common.h"

/* Live effect node allocated in a 0x10-byte size-class block. */
typedef struct EffNode {
    u32 type;       /* 0x00: type-operation row */
    s32 arg;        /* 0x04 */
    s32 instance;   /* 0x08: type-specific instance word */
    f32 unkC;       /* 0x0C */
} EffNode;

typedef char EffNode_size_must_be_0x10[(sizeof(EffNode) == 0x10) ? 1 : -1];

EffNode *effCloneSourceWithTypeHandler(EffNode *source);

#endif
