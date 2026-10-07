#include "common.h"
#include "eff_object.h"

extern void *sdfAllocSizeClassBlock(s32);
extern s32 effObjInnerCreate(EffWorldNode *object);

/* Each object kind keeps its handle in a different structure. */
ObjBase *dds3GetObjectOwnedHandle(EffWorldNode *object) {
    ObjBase *handle;

    handle = 0;
    switch ((object->kindTag >> 24) - 4) {
    case 0: handle = dds3GetCameraHandle(object); break;
    case 1: handle = dds3GetEffectObjectModelHolder(object); break;
    case 2: handle = (ObjBase *)effObjGetDataHandle(object); break;
    case 3: handle = (ObjBase *)effObjGetObjectHandle(object); break;
    case 4: handle = (ObjBase *)dds3GetResourceOwnerHandle(object); break;
    case 5: handle = (ObjBase *)func_00116598(object); break;
    }
    return handle;
}



/* Store the opaque auxiliary value in the shared object base. */
void dds3SetOwnedWorldInnerValue(EffWorldNode *object, u32 value) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    base->unk44 = value;
}

/* Allocate the inner state and associate it with its world-object handle. */
u32 dds3CreateCameraData(EffWorldNode *object) {
    CameraData *inner;
    ObjBase *objectBase;

    effObjInnerCreate(object);
    inner = sdfAllocSizeClassBlock(0x90);
    object->data = inner;
    objectBase = dds3CreateSlotResourceState(object);
    inner->handle = objectBase;
    dds3SetObjectFlags(object, 0x62);
    inner->fovUpdatePending = 0;
    return 1;
}
