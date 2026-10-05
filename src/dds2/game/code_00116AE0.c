#include "common.h"

typedef struct Motion Motion;
typedef struct SdfMotionManager SdfMotionManager;
typedef struct SdfMotionCommandTable SdfMotionCommandTable;

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 dds3AppendWorldObjectNode(s32 kind);
extern void *sdfModelCreateWithItems(void *data, void *listRef);
extern Motion *func_003340E0(SdfMotionManager *, SdfMotionCommandTable *);
extern void sdfMotionInitializeAtZeroTime(void *, s32, s32);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    void *work;
} ObjWithWork;

/* Script object header (0x1C). */
typedef struct ScriptObj {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0xC];
    void *work;
} ScriptObj;

/* Script object work area (0x18). */
typedef struct ObjWork {
    void *unk0;   /* 0x00: fallback model resource */
    void *unk4;   /* 0x04: item list for the model */
    void *unk8;   /* 0x08: script table */
    void *unkC;   /* 0x0C: loaded script resource, null when unloaded */
    u8 pad10[8];
} ObjWork;

/* Instantiate a script object of kind 10 and fill in its parameters. */
s32 evtCreateScriptObject(s32 a, s32 b, s32 c, s32 d, s32 e) {
    ScriptObj *object = (ScriptObj *)dds3AppendWorldObjectNode(10);
    ObjWork *work = (ObjWork *)object->work;

    work->unk8 = c;
    object->unk4 = a;
    object->unk8 = e;
    work->unk0 = d;
    work->unk4 = b;
    work->unkC = NULL;
    return (s32)object;
}

/* Create a script object with its loaded resource already supplied. */
s32 evtCreateScriptObjectWithResource(s32 a, s32 b, s32 c, s32 d, s32 e) {
    ScriptObj *object = (ScriptObj *)dds3AppendWorldObjectNode(10);
    ObjWork *work = (ObjWork *)object->work;

    object->unk4 = a;
    work->unkC = d;
    work->unk4 = b;
    work->unk8 = c;
    object->unk8 = e;
    return (s32)object;
}

/* Create the model from the loaded resource, or from the fallback. */
s32 evtCreateModelFromObject(ObjWithWork *object) {
    ObjWork *work = (ObjWork *)object->work;
    void *model;

    if (work->unkC != NULL) {
        model = sdfModelCreateWithItems(work->unkC, work->unk4);
    } else {
        model = sdfModelCreateWithItems(((ObjWithWork *)work->unk0)->work, work->unk4);
    }
    return (s32)model;
}

Motion *evtAttachScriptToObject(ScriptObj *object, SdfMotionManager *owner) {
    Motion *motion = NULL;
    ObjWork *work;

    if (owner == NULL) {
        return NULL;
    }
    work = object->work;
    if (work->unk8 != NULL) {
        motion = func_003340E0(owner, work->unk8);
        sdfMotionInitializeAtZeroTime(motion, 0, 1);
    }
    return motion;
}

s32 dds3AllocateObjectWork(ObjWithWork *obj) {
    obj->work = sdfAllocSizeClassBlock(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
