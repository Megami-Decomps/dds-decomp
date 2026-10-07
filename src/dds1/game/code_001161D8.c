#include "common.h"
#include "eff_light.h"


extern s32 effObjInnerCreate(EffWorldNode *object);
extern EffWorldNode *dds3AppendWorldObjectNode();
extern void dds3EnsureSlotData();

extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();

/* Create an inner-vector object and snapshot its vector state after initialization. */
EffWorldNode *dds3SpawnInnerVecObj8(s32 initialValue, void *firstVector, void *secondVector) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(8);

    obj->key = initialValue;
    effObjSetInnerFirstVec(obj, firstVector);
    effObjSetInnerSecondVec(obj, secondVector);
    effObjInnerVecBackup(obj->inner);
    return obj;
}

u32 dds3GetResourceOwnerHandle(EffWorldNode *object) {
    return *((u32 *)object->data);
}


/* Allocate the kind-9 light payload and its auxiliary buffer. */
u32 dds3InitializeInnerVectorEffectObject(EffWorldNode *object) {
    EffLightData *obj;

    effObjInnerCreate(object);
    obj = (EffLightData *)sdfAllocSizeClassBlock(0x7C);
    object->data = obj;
    obj->resourceState = dds3CreateSlotResourceState(object);
    dds3SetObjectFlags(object, 0x62);
    obj->flags = 0;
    obj->ambientColor[0] = 0.7f;
    obj->ambientColor[1] = 0.7f;
    obj->ambientColor[2] = 0.7f;
    obj->value14 = 1.0f;
    obj->value18 = 0.5f;
    obj->secondaryColor[0] = 0.2f;
    obj->secondaryColor[1] = 0.2f;
    obj->secondaryColor[2] = 0.2f;
    obj->secondaryColor[3] = 1.0f;
    obj->parameter50 = 100.0f;
    obj->parameter54 = 800.0f;
    obj->parameter58 = 7.0f;
    obj->word0C = 0;
    obj->word10 = 0;
    obj->word1C = 0;
    obj->word5C = 0;
    obj->buffer = sdfAllocSizeClassBlock(0xE0);
    return 1;
}
