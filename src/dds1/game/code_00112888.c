#include "common.h"
#include "sdf_chip.h"
#include "eff_object.h"
#include "eff_dependency.h"
#include "eff_event.h"

extern ObjBase *dds3GetLightObjectResource(EffWorldNode *object);


/* Each object kind keeps its handle in a different structure. */
ObjBase *dds3GetObjectOwnedHandle(EffWorldNode *worldNode) {
    ObjBase *handle;

    handle = 0;
    switch ((worldNode->kindTag >> 24) - 4) {
    case 0: handle = dds3GetCameraHandle(worldNode); break;
    case 1: handle = dds3GetEffectObjectModelHolder(worldNode); break;
    case 2: handle = effObjGetDataHandle(worldNode); break;
    case 3: handle = effObjGetObjectHandle(worldNode); break;
    case 4: handle = dds3GetResourceOwnerHandle(worldNode); break;
    case 5: handle = dds3GetLightObjectResource(worldNode); break;
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
s32 dds3CreateCameraData(EffWorldNode *object) {
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
