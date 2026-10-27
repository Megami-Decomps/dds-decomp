#ifndef EFF_NODE_H
#define EFF_NODE_H

#include "common.h"

/* Live effect node allocated in a 0x10-byte size-class block. */
typedef u32 EffNodeInstanceWord;
typedef s32 EffNodeConditionResult;

/* A zero condition result expires the dependency; any nonzero value keeps it. */
enum {
    EFF_NODE_CONDITION_EXPIRED = 0
};

typedef struct EffNode {
    u32 type;       /* 0x00: type-operation row */
    s32 arg;        /* 0x04 */
    EffNodeInstanceWord instanceWord; /* 0x08: type-specific instance pointer word */
    f32 unkC;       /* 0x0C */
} EffNode;

typedef char EffNode_size_must_be_0x10[(sizeof(EffNode) == 0x10) ? 1 : -1];
typedef char EffNode_instanceWord_offset_must_be_0x08[
    ((u32)&((EffNode *)0)->instanceWord == 0x08) ? 1 : -1];

/* One 0x30-byte row of the type-indexed manager/callback table. Instance
 * values remain words because the concrete effect type owns their formats. */
typedef struct EffTypeOps {
    EffNodeInstanceWord (*create)(s32 arg, s32 parameter);   /* 0x00 */
    void (*update)(EffNodeInstanceWord instanceWord);         /* 0x04 */
    void (*destroy)(EffNodeInstanceWord instanceWord);        /* 0x08 */
    void (*restart)(EffNodeInstanceWord instanceWord);        /* 0x0C */
    EffNodeConditionResult (*queryCondition)(EffNodeInstanceWord instanceWord); /* 0x10 */
    void (*setVector)(EffNodeInstanceWord instanceWord, const void *vector); /* 0x14 */
    void (*setMatrix)(EffNodeInstanceWord instanceWord, const void *matrix); /* 0x18 */
    void (*setParameterWord)(EffNodeInstanceWord instanceWord, u32 value);   /* 0x1C */
    void (*dispatchOptionalFlag)(EffNodeInstanceWord instanceWord, u8 flag); /* 0x20 */
    s32 (*queryOptional)(EffNodeInstanceWord instanceWord);   /* 0x24 */
    EffNodeInstanceWord (*cloneInstanceWord)(EffNodeInstanceWord instanceWord); /* 0x28 */
    void (*applyScale)(EffNodeInstanceWord instanceWord, f32 factor); /* 0x2C */
} EffTypeOps;

typedef char EffTypeOps_size_must_be_0x30[(sizeof(EffTypeOps) == 0x30) ? 1 : -1];

extern const EffTypeOps EffMajorTable[];

EffNode *dds3EffectCreate(u16 type, u16 arg, s32 parameter);
void dds3EffectDelete(EffNode *node);
void dds3EffectDispatchUpdate(EffNode *node);
void dds3EffectReset(EffNode *node);
void dds3EffectSetScale(EffNode *node, f32 factor);
EffNodeConditionResult effInvokeNodeConditionOrAcceptDefault(EffNode *node);
void effCopyVectorToNodeInstance(EffNode *node, const void *vector);
void effApplyNodeTransformMatrix(EffNode *node, const void *matrix);
void effSetNodeParameterValue(EffNode *node, u32 parameterWord);
void effDispatchOptionalNodeFlag(EffNode *node, u8 flag);
s32 effInvokeOptionalNodeInstanceCallback(EffNode *node);
EffNode *dds3EffectCopy(const EffNode *source);

#endif
