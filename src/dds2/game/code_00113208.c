#include "common.h"
#include "dds3obj.h"
#include "eff_object.h"





extern void dds3SetRadiusPoseSet(ObjectTransform *node);

extern EffWorldNode *dds3GetFirstWorldObjectNodeOfKind2(void);


extern s32 dds3ObjectBaseRegistArea(void *object);

s32 dds3CameraObjectBasicGetMode(EffWorldNode *object) {
    return ((CameraData *)object->data)->eyeIsRelative;
}

void dds3SetCameraMode(EffWorldNode *camera, s32 mode) {
    if (((CameraData *)camera->data)->eyeIsRelative != mode) {
        ((CameraData *)camera->data)->eyeIsRelative = mode;
    }
}

ObjBase *dds3UnitObjectBasicGetControlWorkData(EffWorldNode *object) {
    return ((EffectObjectData *)object->data)->modelHolder;
}

EffWorldNode *dds3AddUnitObjectBasic(s32 a, void *firstVector, void *secondVector) {
    EffWorldNode *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_FOLLOW_MODEL);
    EffectObjectData *data;

    if (obj != NULL) {
        obj->key = a;
        dds3ObjectBaseCreateMover(obj);
        dds3SetPosPoseSet(obj, firstVector);
        dds3SetRotPoseSet(obj, secondVector);
        dds3SetRadiusPoseSet(obj->inner);
        data = ((EffectObjectData *)obj->data);
        data->handle = (ObjBase *)-1;
        data->word04 = 0;
        dds3ObjectBaseSetControlBasicType(obj, dds3GetFirstWorldObjectNodeOfKind2(), 5);
        dds3ObjectBaseRegistArea(obj);
        return obj;
    }
    return NULL;
}
