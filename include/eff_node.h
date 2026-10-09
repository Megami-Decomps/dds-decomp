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

/* One 0x30-byte row of the type-indexed manager/callback table. Instance
 * values remain words because the concrete effect type owns their formats. */
typedef struct EffTypeOps {
    s32 (*create)(s32 arg, s32 parameter);                  /* 0x00 */
    void (*update)(s32 instanceWord);                        /* 0x04 */
    void (*destroy)(s32 instanceWord);                       /* 0x08 */
    void (*restart)(s32 instanceWord);                       /* 0x0C */
    s32 (*queryCondition)(s32 instanceWord);                 /* 0x10 */
    void (*setVector)(s32 instanceWord, const void *vector); /* 0x14 */
    void (*setMatrix)(s32 instanceWord, const void *matrix); /* 0x18 */
    void (*setParameterWord)(s32 instanceWord, u32 value);   /* 0x1C */
    void (*dispatchOptionalFlag)(s32 instanceWord, u8 flag); /* 0x20 */
    s32 (*queryOptional)(s32 instanceWord);                  /* 0x24 */
    u32 (*cloneInstanceWord)(u32 instanceWord);              /* 0x28 */
    void (*applyScale)(s32 instanceWord, f32 factor);        /* 0x2C */
} EffTypeOps;

typedef char EffTypeOps_size_must_be_0x30[(sizeof(EffTypeOps) == 0x30) ? 1 : -1];

extern EffTypeOps effNodeTypeOperations[];

EffNode *effCreateNode(u16 type, u16 arg, s32 parameter);
void effDestroyNode(EffNode *node);
void effUpdateNode(EffNode *node);
void effRestartNodeInstance(EffNode *node);
void effApplyNodeScale(EffNode *node, f32 factor);
s32 effInvokeNodeConditionOrAcceptDefault(EffNode *node);
void effCopyVectorToNodeInstance(EffNode *node, const void *vector);
void effApplyNodeTransformMatrix(EffNode *node, const void *matrix);
void effSetNodeParameterValue(EffNode *node, u32 value);
void effDispatchOptionalNodeFlag(EffNode *node, u8 flag);
s32 effInvokeOptionalNodeInstanceCallback(EffNode *node);
EffNode *effCloneSourceWithTypeHandler(EffNode *source);

#endif
