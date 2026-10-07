#include "common.h"
#include "eff_object.h"




extern EffWorldNode *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData(void *object);

extern void effObjSetInnerFirstVec(EffWorldNode *node, u128 *vector);

extern void effObjSetInnerSecondVec(EffWorldNode *node, u128 *vector);

extern void effObjInnerVecBackup(ObjectTransform *node);

extern void *dds3GetFirstWorldObjectNodeOfKind2(void);

extern void *dds3ExchangeSlot(void *object, void *slotData, s32 slotIndex);

extern s32 dds3RegisterObjectInHandlerIndex(void *object);

s32 dds3GetCameraMode(EffWorldNode *object) {
    return ((CameraData *)object->data)->eyeIsRelative;
}

void dds3SetCameraMode(EffWorldNode *object, s32 value) {
    if (((CameraData *)object->data)->eyeIsRelative != value) {
        ((CameraData *)object->data)->eyeIsRelative = value;
    }
}

ObjBase *func_00113230(EffWorldNode *object) {
    return ((EffectObjectData *)object->data)->modelHolder;
}

EffWorldNode *dds3SpawnCameraSlotObj5(s32 a, void *firstVector, void *secondVector) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(5);
    EffectObjectData *data;

    if (obj != NULL) {
        obj->key = a;
        dds3EnsureSlotData(obj);
        effObjSetInnerFirstVec(obj, firstVector);
        effObjSetInnerSecondVec(obj, secondVector);
        effObjInnerVecBackup(obj->inner);
        data = ((EffectObjectData *)obj->data);
        data->handle = (ObjBase *)-1;
        data->word04 = 0;
        dds3ExchangeSlot(obj, dds3GetFirstWorldObjectNodeOfKind2(), 5);
        dds3RegisterObjectInHandlerIndex(obj);
        return obj;
    }
}
