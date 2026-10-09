#include "common.h"
#include "sdf_motion.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_model.h"
#include "eff_transform.h"
#include "sdf_draw.h"
#include "evt_script_model.h"




/* Instantiate a script object of kind 10 and fill in its parameters. */
EffWorldNode *evtCreateScriptObject(s32 key, SdfItemListRef *itemList, MotionTable *motionTable, EffWorldNode *fallbackModelObject, const char *name) {
    EffWorldNode *object = dds3AppendWorldObjectNode(EFF_WORLD_KIND_SCRIPT_OBJECT);
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
EffWorldNode *evtCreateScriptObjectWithResource(s32 key, SdfItemListRef *itemList, MotionTable *motionTable, void *loadedModelResource, const char *name) {
    EffWorldNode *object = dds3AppendWorldObjectNode(EFF_WORLD_KIND_SCRIPT_OBJECT);
    EvtScriptModelWork *work = object->data;

    object->key = key;
    work->loadedModelResource = loadedModelResource;
    work->itemList = itemList;
    work->motionTable = motionTable;
    object->value = name;
    return object;
}


SdfModel *evtCreateModelFromObject(EffWorldNode *obj) {
    EvtScriptModelWork *work = obj->data;
    SdfModel *model;

    if (work->loadedModelResource != NULL) {
        model = sdfModelCreateWithItems(work->loadedModelResource, work->itemList);
    } else {
        model = sdfModelCreateWithItems(work->fallbackModelObject->data, work->itemList);
    }
    return model;
}

Motion *evtAttachScriptToObject(EffWorldNode *object, SdfModel *owner) {
    Motion *motion = NULL;
    EvtScriptModelWork *work;

    if (owner == NULL) {
        return NULL;
    }
    work = object->data;
    if (work->motionTable != NULL) {
        motion = sdfCreateMotion(owner, work->motionTable);
        sdfMotionInitializeAtZeroTime(motion, 0, 1);
    }
    return motion;
}


s32 dds3AllocateObjectWork(EffWorldNode *obj) {
    obj->data = sdfAllocSizeClassBlock(0x18);
    memset(obj->data, 0, 0x18);
    return 1;
}
