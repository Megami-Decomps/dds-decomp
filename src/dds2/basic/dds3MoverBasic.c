#include "common.h"
#include "sdf_vu0.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "eff_object.h"
#include "eff_transform.h"
#include "pcp_vu0.h"

extern void dds3BackupPoseSet(dds3PoseSet_t *transform);


#define DDS3_MOVER_FOV_UPDATE_BIT 1

/* Apply enabled absolute path channels and step the path counter once, or apply
 * a callback-built relative transform when its return value is exactly 1.
 * Always return 1, even when no channel or callback transform was applied. */
s32 MoverBasicMainprocuder(dds3Basic_t *moverNode)
{
    f32 pathVector[4];
    WorldTransformParams transformParams;
    dds3PoseSet_t relativeTransform;
    Dds3SlotResource *slotResource = moverNode->data;
    dds3Basic_t *targetObject = slotResource->target;
    dds3PoseSet_t *inner = targetObject->inner;
    s32 (*updateCallback)(dds3PoseSet_t *, dds3Basic_t *);
    CameraData *cameraData;
    f32 fieldOfView;

    if (slotResource->path != NULL) {
        if (slotResource->path->counter.flags & DDS3_PATH_POSITION_CHANNEL) {
            dds3PathBasicControlGetPos(slotResource->path);
            SDF_VU0_STOREVECTOR(pathVector);
            dds3SetPosPoseSet(targetObject, (u128 *)pathVector);
            if (((u8 *)&targetObject->kindTag)[3] == EFF_WORLD_KIND_EFFECT_TRANSFORM) {
                SDF_VU0_COPYVECTOR(&((EffectTransformData *)targetObject->data)->position, pathVector);
            }
        }
        if (slotResource->path->counter.flags & DDS3_PATH_ROTATION_CHANNEL) {
            dds3PathBasicControlGetRot(slotResource->path);
            SDF_VU0_STOREVECTOR(pathVector);
            dds3SetRotPoseSet(targetObject, (u128 *)pathVector);
        }
        if (slotResource->path->counter.flags & DDS3_PATH_SCALAR_CHANNEL) {
            if (((u8 *)&targetObject->kindTag)[3] == EFF_WORLD_KIND_CAMERA) {
                fieldOfView = dds3PathBasicControlGetFovy(slotResource->path);
                cameraData = targetObject->data;
                cameraData->fieldOfView = fieldOfView;
                cameraData->fovUpdatePending |= DDS3_MOVER_FOV_UPDATE_BIT;
            }
        }
        if (slotResource->path->counter.flags & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
            if (((u8 *)&targetObject->kindTag)[3] == EFF_WORLD_KIND_LIGHT) {
                dds3PathBasicControlNextStep(slotResource->path, &transformParams);
                dds3SetLightObjectPathResoruce(targetObject, &transformParams);
            }
        }
        /* Advance even when no applicable channel was enabled for this target. */
        dds3PathBasicControlIsFinished(&slotResource->path->counter);
    } else {
        updateCallback = slotResource->update;
        /* This unidentified word is cleared even when no callback exists. */
        inner->unkC8 = 0;
        if (updateCallback != NULL) {
            dds3BackupPoseSet(&relativeTransform);
            if (updateCallback(&relativeTransform, targetObject) == 1) {
                dds3GetSclPoseSetMCR(targetObject, (u128 *)relativeTransform.scale);
                dds3AddRotPoseSet(targetObject, (u128 *)&relativeTransform.rotation);
                dds3GetPosPoseSetMCR(targetObject, (u128 *)relativeTransform.position);
            }
        }
    }
    return 1;
}
