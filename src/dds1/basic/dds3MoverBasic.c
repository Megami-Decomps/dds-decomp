#include "common.h"
#include "pcp_vu0.h"

/* Stack transform node shared with effObjInnerVecInit and its vector modifiers. */
typedef struct EffLocalNode {
    u8 pad00[0x40];
    u128 position;
    u128 rotation;
    u128 scale;
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
    u32 fovUpdatePending;
    f32 fieldOfView;
} MoverScalarData;

extern void func_00116F38(MoverPath *);
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

#define DDS3_MOVER_POSITION_CHANNEL_BIT 1
#define DDS3_MOVER_ROTATION_CHANNEL_BIT 2
#define DDS3_MOVER_FOV_CHANNEL_BIT 4
#define DDS3_MOVER_WORLD_TRANSFORM_CHANNEL_BIT 0x10
#define DDS3_MOVER_POSITION_COPY_KIND 6
#define DDS3_MOVER_CAMERA_KIND 4
#define DDS3_MOVER_WORLD_TRANSFORM_KIND 9
#define DDS3_MOVER_FOV_UPDATE_BIT 1

/* Apply enabled absolute path channels and step the path counter once, or apply
 * a callback-built relative transform when its return value is exactly 1.
 * Always return 1, even when no channel or callback transform was applied. */
s32 dds3UpdateMoverTransform(MoverObject *object)
{
    f32 pathVector[4];
    f32 transformParams[10];
    EffLocalNode relativeTransform;
    MoverWork *work = object->work;
    MoverTarget *target = work->target;
    EffLocalNode *inner = target->inner;
    s32 (*updateCallback)(EffLocalNode *, MoverTarget *);
    MoverScalarData *cameraData;
    f32 fieldOfView;

    if (work->path != NULL) {
        if (work->path->flags & DDS3_MOVER_POSITION_CHANNEL_BIT) {
            func_00116F38(work->path);
            VU0_STORE_VF(vf10, pathVector);
            effObjSetInnerFirstVec(target, pathVector);
            if (target->kind == DDS3_MOVER_POSITION_COPY_KIND) {
                PCP_COPY_VECTOR(&((MoverPositionData *)target->data)->position, pathVector);
            }
        }
        if (work->path->flags & DDS3_MOVER_ROTATION_CHANNEL_BIT) {
            dds3PreparePathVectorPair(work->path);
            VU0_STORE_VF(vf10, pathVector);
            effObjSetInnerSecondVec(target, pathVector);
        }
        if (work->path->flags & DDS3_MOVER_FOV_CHANNEL_BIT) {
            if (target->kind == DDS3_MOVER_CAMERA_KIND) {
                fieldOfView = sdfSampleActiveLinearCurve(work->path);
                cameraData = target->data;
                cameraData->fieldOfView = fieldOfView;
                cameraData->fovUpdatePending |= DDS3_MOVER_FOV_UPDATE_BIT;
            }
        }
        if (work->path->flags & DDS3_MOVER_WORLD_TRANSFORM_CHANNEL_BIT) {
            if (target->kind == DDS3_MOVER_WORLD_TRANSFORM_KIND) {
                dds3InterpolatePathOutput(work->path, transformParams);
                dds3LoadWorldTransformParams(target, transformParams);
            }
        }
        /* Advance even when no applicable channel was enabled for this target. */
        sdfStepWrappingFloatCounter(work->path);
    } else {
        updateCallback = work->update;
        /* This unidentified word is cleared even when no callback exists. */
        inner->unkC8 = 0;
        if (updateCallback != NULL) {
            effObjInnerVecInit(&relativeTransform);
            if (updateCallback(&relativeTransform, target) == 1) {
                effObjMulInnerThirdVec(target, &relativeTransform.scale);
                effObjQuatMulInnerSecondVec(target, &relativeTransform.rotation);
                effObjAddInnerFirstVec(target, &relativeTransform.position);
            }
        }
    }
    return 1;
}

