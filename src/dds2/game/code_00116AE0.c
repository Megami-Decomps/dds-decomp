#include "common.h"

extern void *func_00328D68(s32 size);
extern s32 dds3AppendWorldObjectNode(s32 kind);
extern void *sdfModelCreateWithItems(void *data, void *listRef);
extern s32 func_003340E0(s32 arg0, void *arg1, s32 arg2);
extern s32 sdfMotionInitializeAtZeroTime(void *arg0, s32 arg1, s32 arg2);

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

/* Same as evtCreateScriptObject but stores arg3 at work+0xC instead of 0x10. The store
   order is what fixes retail's saved-register order: sched1 sorts these independent
   stores by luid, and the saved regs go by live length (store position - copy position). */
s32 func_00116B58(s32 a, s32 b, s32 c, s32 d, s32 e) {
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

/* Attach the named script to the object's work area, registering a task;
   stays asm: retail's $6 argument copy has no plain-C shape. */
INCLUDE_ASM(const s32, "game/code_00116AE0", evtAttachScriptToObject);

s32 dds3AllocateObjectWork(ObjWithWork *obj) {
    obj->work = func_00328D68(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
