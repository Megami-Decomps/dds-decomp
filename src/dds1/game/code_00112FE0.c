#include "common.h"
#include "dds3obj.h"
#include "eff_object.h"


s32 dds3CameraObjectBasicGetMode(dds3Basic_t *camera) {
    return ((CameraData *)camera->data)->eyeIsRelative;
}

void dds3SetCameraMode(dds3Basic_t *camera, s32 mode) {
    if (((CameraData *)camera->data)->eyeIsRelative != mode) {
        ((CameraData *)camera->data)->eyeIsRelative = mode;
    }
}

dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((EffectObjectData *)object->data)->modelHolder;
}



extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);
extern dds3Basic_t *dds3GetFirstWorldObjectNodeOfKind2(void);
extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *object);

dds3Basic_t *dds3AddUnitObjectBasic(s32 a, void *firstVector, void *secondVector) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_FOLLOW_MODEL);
    EffectObjectData *data;

    if (obj != NULL) {
        obj->key = a;
        dds3ObjectBaseCreateMover(obj);
        dds3SetPosPoseSet(obj, firstVector);
        dds3SetRotPoseSet(obj, secondVector);
        dds3SetRadiusPoseSet(obj->inner);
        data = ((EffectObjectData *)obj->data);
        data->handle = (dds3ObjectBaseControlWorkData_t *)-1;
        data->word04 = 0;
        dds3ObjectBaseSetControlBasicType(obj, dds3GetFirstWorldObjectNodeOfKind2(), 5);
        dds3ObjectBaseRegistArea(obj);
        return obj;
    }
    return NULL;
}
