#include "common.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "eff_object.h"
#include "eff_transform.h"
#include "pcp_vu0.h"

extern void effObjInnerVecInit(ObjectTransform *transform);


#define DDS3_MOVER_POSITION_COPY_KIND 6
#define DDS3_MOVER_CAMERA_KIND 4
#define DDS3_MOVER_WORLD_TRANSFORM_KIND 9
#define DDS3_MOVER_FOV_UPDATE_BIT 1

/* Apply enabled absolute path channels and step the path counter once, or apply
 * a callback-built relative transform when its return value is exactly 1.
 * Always return 1, even when no channel or callback transform was applied. */
s32 dds3UpdateMoverTransform(EffWorldNode *moverNode)
{
    f32 pathVector[4];
    WorldTransformParams transformParams;
    ObjectTransform relativeTransform;
    Dds3SlotResource *slotResource = moverNode->data;
    EffWorldNode *targetObject = slotResource->target;
    ObjectTransform *inner = targetObject->inner;
    s32 (*updateCallback)(ObjectTransform *, EffWorldNode *);
    CameraData *cameraData;
    f32 fieldOfView;

    if (slotResource->path != NULL) {
        if (slotResource->path->flags & DDS3_PATH_POSITION_CHANNEL) {
            dds3InterpolatePathVectorVU(slotResource->path);
            VU0_STORE_VF(vf10, pathVector);
            effObjSetInnerPosition(targetObject, (u128 *)pathVector);
            if (((u8 *)&targetObject->kindTag)[3] == DDS3_MOVER_POSITION_COPY_KIND) {
                PCP_COPY_VECTOR(&((EffectTransformData *)targetObject->data)->position, pathVector);
            }
        }
        if (slotResource->path->flags & DDS3_PATH_ROTATION_CHANNEL) {
            dds3InterpolatePathQuaternionVU(slotResource->path);
            VU0_STORE_VF(vf10, pathVector);
            effObjSetInnerRotation(targetObject, (u128 *)pathVector);
        }
        if (slotResource->path->flags & DDS3_PATH_SCALAR_CHANNEL) {
            if (((u8 *)&targetObject->kindTag)[3] == DDS3_MOVER_CAMERA_KIND) {
                fieldOfView = sdfSampleActiveLinearCurve(slotResource->path);
                cameraData = targetObject->data;
                cameraData->fieldOfView = fieldOfView;
                cameraData->fovUpdatePending |= DDS3_MOVER_FOV_UPDATE_BIT;
            }
        }
        if (slotResource->path->flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
            if (((u8 *)&targetObject->kindTag)[3] == DDS3_MOVER_WORLD_TRANSFORM_KIND) {
                dds3InterpolatePathOutput(slotResource->path, &transformParams);
                dds3LoadWorldTransformParams(targetObject, &transformParams);
            }
        }
        /* Advance even when no applicable channel was enabled for this target. */
        sdfStepWrappingFloatCounter(slotResource->path);
    } else {
        updateCallback = slotResource->update;
        /* This unidentified word is cleared even when no callback exists. */
        inner->unkC8 = 0;
        if (updateCallback != NULL) {
            effObjInnerVecInit(&relativeTransform);
            if (updateCallback(&relativeTransform, targetObject) == 1) {
                effObjMultiplyInnerScale(targetObject, (u128 *)relativeTransform.scale);
                effObjMultiplyInnerRotation(targetObject, (u128 *)&relativeTransform.rotation);
                effObjAddInnerPosition(targetObject, (u128 *)relativeTransform.position);
            }
        }
    }
    return 1;
}
