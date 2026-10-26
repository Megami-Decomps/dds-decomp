#include "common.h"
#include "dds3_basic_lifecycle.h"
#include "dds3_object_flags.h"
#include "dds3obj.h"
#include "sdf_chip.h"
#include "eff_object.h"
#include "eff_dependency.h"
#include "eff_event.h"

extern dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *object);





/* Each object kind keeps its handle in a different structure. */
dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *worldNode) {
    dds3ObjectBaseControlWorkData_t *handle;

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


void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *object, u32 value) {
    dds3ObjectBaseControlWorkData_t *base;

    base = dds3GetBaseControlWorkData(object);
    base->unk44 = value;
}


Dds3BasicCreateResult dds3CreateCameraData(dds3Basic_t *object) {
    CameraData *inner;
    dds3ObjectBaseControlWorkData_t *objectBase;

    dds3InitPoseSet(object);
    inner = sdfChipAllocDirect(0x90);
    object->data = inner;
    objectBase = dds3CreateSlotResourceState(object);
    inner->handle = objectBase;
    dds3ObjectBaseOnFlag(object, 0x62);
    inner->fovUpdatePending = 0;
    return 1;
}
