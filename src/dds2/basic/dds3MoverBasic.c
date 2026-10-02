#include "common.h"
#include "pcp_vu0.h"

/* Stack transform node shared with effObjInnerVecInit and its vector modifiers. */
typedef struct EffLocalNode {
    u8 pad00[0x40];
    u128 vec40;
    u128 vec50;
    u128 vec60;
    u8 pad70[0x58];
    u32 unkC8;
    u8 padCC[4];
} EffLocalNode;

typedef struct MoverTarget {
    u8 pad00[0xF];
    u8 kind;
    u8 pad10[8];
    void *data;
    EffLocalNode *inner;
} MoverTarget;

typedef struct {
    u32 unk00;
    u32 flags;
} MoverPath;

typedef struct {
    MoverTarget *target;
    MoverPath *path;
    s32 (*update)(EffLocalNode *, MoverTarget *);
} MoverWork;

typedef struct {
    u8 pad00[0x18];
    MoverWork *work;
} MoverObject;

typedef struct {
    u8 pad00[0x20];
    u128 position;
} MoverPositionData;

typedef struct {
    u8 pad00[0x88];
    u32 flags;
    f32 parameter;
} MoverScalarData;

extern void func_001171A0(MoverPath *);
extern void dds3PreparePathVectorPair(MoverPath *);
extern f32 sdfSampleActiveLinearCurve(MoverPath *);
extern void dds3InterpolatePathOutput(MoverPath *, f32 *);
extern void dds3LoadWorldTransformParams(MoverTarget *, f32 *);
extern s32 sdfStepWrappingFloatCounter(MoverPath *);
extern void effObjSetInnerFirstVec(MoverTarget *, void *);
extern void effObjSetInnerSecondVec(MoverTarget *, void *);
extern void effObjInnerVecInit(EffLocalNode *);
extern void effObjMulInnerThirdVec(MoverTarget *, u128 *);
extern void effObjQuatMulInnerSecondVec(MoverTarget *, u128 *);
extern void effObjAddInnerFirstVec(MoverTarget *, u128 *);

/* Apply enabled path channels, or build a relative transform through the callback. */
s32 dds3UpdateMoverTransform(MoverObject *object)
{
    f32 vector[4];
    f32 pathOutput[10];
    EffLocalNode node;
    MoverWork *work = object->work;
    MoverTarget *target = work->target;
    EffLocalNode *inner = target->inner;
    s32 (*update)(EffLocalNode *, MoverTarget *);
    MoverScalarData *scalar;
    f32 value;

    if (work->path != NULL) {
        if (work->path->flags & 1) {
            func_001171A0(work->path);
            VU0_STORE_VF(vf10, vector);
            effObjSetInnerFirstVec(target, vector);
            if (target->kind == 6) {
                PCP_COPY_VECTOR(&((MoverPositionData *)target->data)->position, vector);
            }
        }
        if (work->path->flags & 2) {
            dds3PreparePathVectorPair(work->path);
            VU0_STORE_VF(vf10, vector);
            effObjSetInnerSecondVec(target, vector);
        }
        if (work->path->flags & 4) {
            if (target->kind == 4) {
                value = sdfSampleActiveLinearCurve(work->path);
                scalar = target->data;
                scalar->parameter = value;
                scalar->flags |= 1;
            }
        }
        if (work->path->flags & 0x10) {
            if (target->kind == 9) {
                dds3InterpolatePathOutput(work->path, pathOutput);
                dds3LoadWorldTransformParams(target, pathOutput);
            }
        }
        sdfStepWrappingFloatCounter(work->path);
    } else {
        update = work->update;
        inner->unkC8 = 0;
        if (update != NULL) {
            effObjInnerVecInit(&node);
            if (update(&node, target) == 1) {
                effObjMulInnerThirdVec(target, &node.vec60);
                effObjQuatMulInnerSecondVec(target, &node.vec50);
                effObjAddInnerFirstVec(target, &node.vec40);
            }
        }
    }
    return 1;
}

