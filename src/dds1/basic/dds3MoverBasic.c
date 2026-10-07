#include "common.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "eff_transform.h"
#include "pcp_vu0.h"

typedef struct {
    EffWorldNode *target;
    Dds3PathCurveWork *path;
    s32 (*update)(ObjectTransform *, EffWorldNode *);
} MoverWork;

typedef struct {
    u8 pad00[0x20];
    u128 position;
} MoverPositionData;

typedef struct {
    u8 pad00[0x88];
    u32 fovUpdatePending;
    f32 fieldOfView;
} MoverScalarData;

extern void dds3InterpolatePathVectorVU(Dds3PathCurveWork *);
extern void dds3PreparePathVectorPair(Dds3PathCurveWork *);
extern f32 sdfSampleActiveLinearCurve(Dds3PathCurveWork *);
extern void dds3InterpolatePathOutput(Dds3PathCurveWork *, WorldTransformParams *);
extern void dds3LoadWorldTransformParams(EffWorldNode *, WorldTransformParams *);
extern s32 sdfStepWrappingFloatCounter(Dds3PathCurveWork *);
extern void effObjSetInnerFirstVec(EffWorldNode *, void *);
extern void effObjSetInnerSecondVec(EffWorldNode *, void *);
extern void effObjInnerVecInit(ObjectTransform *);
extern void effObjMulInnerThirdVec(EffWorldNode *, void *);
extern void effObjQuatMulInnerSecondVec(EffWorldNode *, u128 *);
extern void effObjAddInnerFirstVec(EffWorldNode *, void *);

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
s32 dds3UpdateMoverTransform(EffWorldNode *object)
{
    f32 pathVector[4];
    WorldTransformParams transformParams;
    ObjectTransform relativeTransform;
    MoverWork *work = object->data;
    EffWorldNode *target = work->target;
    ObjectTransform *inner = target->inner;
    s32 (*updateCallback)(ObjectTransform *, EffWorldNode *);
    MoverScalarData *cameraData;
    f32 fieldOfView;

    if (work->path != NULL) {
        if (work->path->flags & DDS3_MOVER_POSITION_CHANNEL_BIT) {
            dds3InterpolatePathVectorVU(work->path);
            VU0_STORE_VF(vf10, pathVector);
            effObjSetInnerFirstVec(target, pathVector);
            if (((u8 *)&target->kindTag)[3] == DDS3_MOVER_POSITION_COPY_KIND) {
                PCP_COPY_VECTOR(&((MoverPositionData *)target->data)->position, pathVector);
            }
        }
        if (work->path->flags & DDS3_MOVER_ROTATION_CHANNEL_BIT) {
            dds3PreparePathVectorPair(work->path);
            VU0_STORE_VF(vf10, pathVector);
            effObjSetInnerSecondVec(target, pathVector);
        }
        if (work->path->flags & DDS3_MOVER_FOV_CHANNEL_BIT) {
            if (((u8 *)&target->kindTag)[3] == DDS3_MOVER_CAMERA_KIND) {
                fieldOfView = sdfSampleActiveLinearCurve(work->path);
                cameraData = target->data;
                cameraData->fieldOfView = fieldOfView;
                cameraData->fovUpdatePending |= DDS3_MOVER_FOV_UPDATE_BIT;
            }
        }
        if (work->path->flags & DDS3_MOVER_WORLD_TRANSFORM_CHANNEL_BIT) {
            if (((u8 *)&target->kindTag)[3] == DDS3_MOVER_WORLD_TRANSFORM_KIND) {
                dds3InterpolatePathOutput(work->path, &transformParams);
                dds3LoadWorldTransformParams(target, &transformParams);
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
                effObjMulInnerThirdVec(target, relativeTransform.scale);
                effObjQuatMulInnerSecondVec(target, (u128 *)&relativeTransform.rotation);
                effObjAddInnerFirstVec(target, relativeTransform.position);
            }
        }
    }
    return 1;
}

