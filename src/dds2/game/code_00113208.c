#include "common.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "eff_object.h"





extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);

extern dds3Basic_t *dds3GetFirstWorldObjectNodeOfKind2(void);


extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *object);

CameraEyePositionMode dds3CameraObjectBasicGetMode(dds3Basic_t *object) {
    return ((CameraData *)object->workdata)->eyeIsRelative;
}

void dds3SetCameraMode(dds3Basic_t *camera, CameraEyePositionMode mode) {
    if (((CameraData *)camera->workdata)->eyeIsRelative != mode) {
        ((CameraData *)camera->workdata)->eyeIsRelative = mode;
    }
}

dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((EffectObjectData *)object->workdata)->modelHolder;
}

dds3Basic_t *dds3AddUnitObjectBasic(s32 worldId, u128 *position, u128 *rotation) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_FOLLOW_MODEL);
    EffectObjectData *data;

    if (obj != NULL) {
        obj->key = worldId;
        dds3ObjectBaseCreateMover(obj);
        dds3SetPosPoseSet(obj, position);
        dds3SetRotPoseSet(obj, rotation);
        dds3SetRadiusPoseSet(obj->poseset);
        data = ((EffectObjectData *)obj->workdata);
        data->handle = (dds3ObjectBaseControlWorkData_t *)-1;
        data->word04 = 0;
        dds3ObjectBaseSetControlBasicType(
            obj, dds3GetFirstWorldObjectNodeOfKind2(), DDS3_OBJECT_HANDLER_SLOT);
        dds3ObjectBaseRegistArea(obj);
        return obj;
    }
    return NULL;
}
