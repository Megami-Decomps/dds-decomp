#include "common.h"
#include "sdf_motion.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_model.h"
#include "eff_transform.h"
#include "sdf_draw.h"



/* Script object work area (0x18). */
typedef struct ObjWork {
    EffWorldNode *unk0;   /* 0x00: fallback model resource */
    void *unk4;   /* 0x04: item list for the model */
    MotionTable *unk8;   /* 0x08: script table */
    void *unkC;   /* 0x0C: loaded script resource, null when unloaded */
    u8 pad10[8];
} ObjWork;

/* Instantiate a script object of kind 10 and fill in its parameters. */
EffWorldNode *evtCreateScriptObject(s32 a, void *b, MotionTable *c, EffWorldNode *d, const char *e) {
    EffWorldNode *object = dds3AppendWorldObjectNode(10);
    ObjWork *work = object->data;

    work->unk8 = c;
    object->key = a;
    object->value = e;
    work->unk0 = d;
    work->unk4 = b;
    work->unkC = NULL;
    return object;
}

/* Create a script object with its loaded resource already supplied. */
EffWorldNode *evtCreateScriptObjectWithResource(s32 a, void *b, MotionTable *c, void *d, const char *e) {
    EffWorldNode *object = dds3AppendWorldObjectNode(10);
    ObjWork *work = object->data;

    object->key = a;
    work->unkC = d;
    work->unk4 = b;
    work->unk8 = c;
    object->value = e;
    return object;
}


SdfModel *evtCreateModelFromObject(EffWorldNode *obj) {
    ObjWork *work = obj->data;
    SdfModel *model;

    if (work->unkC != NULL) {
        model = sdfModelCreateWithItems(work->unkC, work->unk4);
    } else {
        model = sdfModelCreateWithItems(work->unk0->data, work->unk4);
    }
    return model;
}

Motion *evtAttachScriptToObject(EffWorldNode *object, SdfModel *owner) {
    Motion *motion = NULL;
    ObjWork *work;

    if (owner == NULL) {
        return NULL;
    }
    work = object->data;
    if (work->unk8 != NULL) {
        motion = sdfCreateMotion(owner, work->unk8);
        sdfMotionInitializeAtZeroTime(motion, 0, 1);
    }
    return motion;
}


s32 dds3AllocateObjectWork(EffWorldNode *obj) {
    obj->data = sdfAllocSizeClassBlock(0x18);
    memset(obj->data, 0, 0x18);
    return 1;
}
