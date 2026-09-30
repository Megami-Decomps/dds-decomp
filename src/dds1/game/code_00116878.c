#include "common.h"

extern s32 func_00110880(s32 kind);

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
s32 func_00116878(s32 a, s32 b, s32 c, s32 d, s32 e) {
    ScriptObj *object = (ScriptObj *)func_00110880(10);
    ObjWork *work = (ObjWork *)object->work;

    work->unk8 = c;
    object->unk4 = a;
    object->unk8 = e;
    work->unk0 = d;
    work->unk4 = b;
    work->unkC = NULL;
    return (s32)object;
}

INCLUDE_ASM(const s32, "game/code_00116878", func_001168F0);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    void *work;
} ObjWithWork;

u32 func_00116968(ObjWithWork *obj) {
    u32 *work = (u32 *)obj->work;
    u32 result;

    if (work[3] != 0) {
        result = sdfModelCreateWithItems(work[3], work[1]);
    } else {
        result = sdfModelCreateWithItems(((u32 *)work[0])[6], work[1]);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00116878", func_001169B0);

extern void *func_002CFEB8(s32 size);

s32 dds3AllocateObjectWork(ObjWithWork *obj) {
    obj->work = func_002CFEB8(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
