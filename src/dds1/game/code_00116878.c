#include "common.h"

typedef struct ActionSub {
    s32 unk0;   /* 0x0 */
    s32 unk4;   /* 0x4 */
    s32 unk8;   /* 0x8 */
    s32 unkC;   /* 0xC */
} ActionSub;

typedef struct ActionObj {
    u8 unk0[4];        /* 0x0 */
    s32 unk4;          /* 0x4 */
    s32 unk8;          /* 0x8 */
    u8 unkC[0xC];      /* 0xC */
    ActionSub *sub;    /* 0x18 */
} ActionObj;

extern ActionObj *func_00110880();

ActionObj *func_00116878(s32 a, s32 b, s32 c, s32 d, s32 e) {
    ActionObj *obj = func_00110880(0xA);

    obj->sub->unk0 = d;
    obj->sub->unk4 = b;
    obj->sub->unk8 = c;
    obj->sub->unkC = 0;
    obj->unk4 = a;
    obj->unk8 = e;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_00116878", func_001168F0);

INCLUDE_ASM(const s32, "game/code_00116878", func_00116968);

INCLUDE_ASM(const s32, "game/code_00116878", func_001169B0);

extern void *func_002CFEB8(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    void *work;
} ObjWithWork;

s32 func_00116A10(ObjWithWork *obj) {
    obj->work = func_002CFEB8(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
