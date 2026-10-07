#include "common.h"
#include "dds3obj.h"


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


/* Inner-vector object hanging off a world object: transform and rotation state. */
typedef struct InnerVecObj {
    f32 pos00;
    f32 pos04;
    f32 scale08;
    u32 unk0C;
    u32 unk10;
    f32 pos14;
    f32 scale18;
    u32 unk1C;
    u8 pad20[0x20];
    f32 pos40;
    f32 pos44;
    f32 rot48;
    f32 rot4C;
    f32 rot50;
    f32 rot54;
    f32 rot58;
    u32 unk5C;
    u8 pad60[4];
    u32 unk64;
    u8 pad68[0xC];
    ObjBase *unk74;
    void *extra78;
} InnerVecObj;

/* Build the inner-vector object of a world object and fill in its default state. */
u32 dds3InitializeInnerVectorEffectObject(EffWorldNode *object) {
    InnerVecObj *obj;

    effObjInnerCreate();
    obj = (InnerVecObj *)sdfAllocSizeClassBlock(0x7C);
    object->data = obj;
    obj->unk74 = dds3CreateSlotResourceState(object);
    dds3SetObjectFlags(object, 0x62);
    obj->unk64 = 0;
    obj->pos00 = 0.7f;
    obj->pos04 = 0.7f;
    obj->scale08 = 0.7f;
    obj->pos14 = 1.0f;
    obj->scale18 = 0.5f;
    obj->pos40 = 0.2f;
    obj->pos44 = 0.2f;
    obj->rot48 = 0.2f;
    obj->rot4C = 1.0f;
    obj->rot50 = 100.0f;
    obj->rot54 = 800.0f;
    obj->rot58 = 7.0f;
    obj->unk0C = 0;
    obj->unk10 = 0;
    obj->unk1C = 0;
    obj->unk5C = 0;
    obj->extra78 = sdfAllocSizeClassBlock(0xE0);
    return 1;
}
