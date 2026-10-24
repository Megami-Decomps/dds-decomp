#include "sdk_ps2.h"
#include "common.h"
#include "sdf_motion_loop.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_model.h"
#include "eff_transform.h"
#include "sdf_draw.h"
#include "evt_script_model.h"




/* Instantiate a script object of kind 10 and fill in its parameters. */
dds3Basic_t *dds3AddModelBasic(s32 key, sdfModelDef_t *itemList, MotionTable *motionTable, dds3Basic_t *fallbackModelObject, const char *name) {
    dds3Basic_t *object = dds3AddSceneBasicData(EFF_WORLD_KIND_SCRIPT_OBJECT);
    EvtScriptModelWork *work = object->data;

    work->motionTable = motionTable;
    object->key = key;
    object->value = name;
    work->fallbackModelObject = fallbackModelObject;
    work->itemList = itemList;
    work->loadedModelResource = NULL;
    return object;
}

/* Create a script object with its loaded resource already supplied. */
dds3Basic_t *dds3AddModelBasic2(s32 key, sdfModelDef_t *itemList, MotionTable *motionTable, DevRequest *loadedModelResource, const char *name) {
    dds3Basic_t *object = dds3AddSceneBasicData(EFF_WORLD_KIND_SCRIPT_OBJECT);
    EvtScriptModelWork *work = object->data;

    object->key = key;
    work->loadedModelResource = loadedModelResource;
    work->itemList = itemList;
    work->motionTable = motionTable;
    object->value = name;
    return object;
}


sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *obj) {
    EvtScriptModelWork *work = obj->data;
    sdfModelRootNode_t *model;

    if (work->loadedModelResource != NULL) {
        model = sdfModelMake(work->loadedModelResource, work->itemList);
    } else {
        model = sdfModelMake(work->fallbackModelObject->data, work->itemList);
    }
    return model;
}

sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *object, sdfModelRootNode_t *owner) {
    sdfMotionCtrlr_t *motion = NULL;
    EvtScriptModelWork *work;

    if (owner == NULL) {
        return NULL;
    }
    work = object->data;
    if (work->motionTable != NULL) {
        motion = sdfMotionCtrlrNew(owner, work->motionTable);
        sdfMotionChange(motion, 0, SDF_MOTION_LOOP_ENABLED);
    }
    return motion;
}



s32 dds3AllocModelBasicWorkData(dds3Basic_t *obj) {
    obj->data = sdfChipAllocDirect(0x18);
    memset(obj->data, 0, 0x18);
    return 1;
}
