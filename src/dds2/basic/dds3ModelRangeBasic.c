#include "sdk_ps2.h"
#include "common.h"
#include "sdf_motion_loop.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_model.h"
#include "eff_transform.h"
#include "sdf_draw.h"
#include "evt_script_model.h"
#include "evt_script_model_api.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "dds3_model_range.h"
#include "dds3_path.h"
#include "eff.h"
#include "dds3_world_secondary.h"
#include "dds3_path_state.h"

s32 dds3AllocModelBasicWorkData(dds3Basic_t *obj) {
    obj->workdata = sdfChipAllocDirect(0x18);
    memset(obj->workdata, 0, 0x18);
    return 1;
}

/* The kind-11 destroy hook queues its optional resource allocation before
 * freeing its data block. dds3DeleteBasic frees the object after this returns. */
void dds3ModelBasicFreeModel(dds3Basic_t *object) {
    const Dds3ModelRangeDataView *rangeData;
    const void *ownedResourceData;
    sdfMemHandle_t *allocation;

    rangeData = object->workdata;
    ownedResourceData = rangeData->ownedResourceData;
    if (ownedResourceData != NULL) {
        allocation = sdfMemGetHandleByPtr(ownedResourceData);
        sdfMemFreeDelayedDirect(allocation);
    }
    sdfChipFreeDirect((void *)rangeData);
}

typedef struct ActionSub {
    u8 unk0[0x10];
    s32 unk10;
} ActionSub;

dds3Basic_t *dds3AddModelRangeBasicNew(s32 objectKey, void *resourceData, const f32 *position, const char *objectName) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_ACTION_B);

    obj->name = objectName;
    ((ActionSub *)obj->workdata)->unk10 = 0;
    obj->key = objectKey;
    return obj;
}

void dds3SetPathBasicSpline(dds3Basic_t *path, u32 value) {
    ((Dds3PathStatePayload *)path->workdata)->value = value;
}

u32 dds3SearchSceneBasicWorkData(u32 id) {
    dds3Basic_t *path;
    dds3Basic_t *world;

    world = dds3GetControlScene();
    path = dds3SearchSceneBasic(world, id, 6);
    return ((Dds3PathStatePayload *)path->workdata)->value;
}
