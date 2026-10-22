#include "common.h"
#include "sdf_chip.h"
#include "eff_object.h"
#include "eff_dependency.h"
#include "eff_event.h"

extern ObjBase *dds3LightObjectBasicGetControlWorkData(EffWorldNode *object);


/* Each object kind keeps its handle in a different structure. */
ObjBase *dds3GetBaseControlWorkData(EffWorldNode *worldNode) {
    ObjBase *handle;

    handle = 0;
    switch ((worldNode->kindTag >> 24) - 4) {
    case 0: handle = dds3GetCameraObjectBaseControlWorkData(worldNode); break;
    case 1: handle = dds3UnitObjectBasicGetControlWorkData(worldNode); break;
    case 2: handle = dds3MatterObjectBasicGetControlWorkData(worldNode); break;
    case 3: handle = dds3EffectObjectBasicGetControlWorkData(worldNode); break;
    case 4: handle = dds3EventObjectBasicGetControlWorkData(worldNode); break;
    case 5: handle = dds3LightObjectBasicGetControlWorkData(worldNode); break;
    }
    return handle;
}



/* Store the opaque auxiliary value in the shared object base. */
void dds3ObjectBaseSetControlBasicSvs(EffWorldNode *object, u32 value) {
    ObjBase *base;

    base = dds3GetBaseControlWorkData(object);
    base->unk44 = value;
}

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

/* Allocate the inner state and associate it with its world-object handle. */
s32 dds3CreateCameraData(EffWorldNode *object) {
    CameraData *inner;
    ObjBase *objectBase;

    dds3InitPoseSet(object);
    inner = sdfChipAllocDirect(sizeof(CameraData));
    object->data = inner;
    objectBase = dds3CreateSlotResourceState(object);
    inner->handle = objectBase;
    dds3ObjectBaseOnFlag(object, 0x62);
    inner->fovUpdatePending = 0;
    return 1;
}
