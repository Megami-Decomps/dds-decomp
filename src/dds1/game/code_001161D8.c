#include "common.h"

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u8 unk8[0x14]; /* 0x8 */
    s32 unk1C;    /* 0x1C */
} ActionObj;

extern ActionObj *func_00110880();
extern void func_00112750();

extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();

ActionObj *func_001161D8(s32 a, s32 b, s32 c) {
    ActionObj *obj = func_00110880(8);

    obj->unk4 = a;
    effObjSetInnerFirstVec(obj, b);
    effObjSetInnerSecondVec(obj, c);
    effObjInnerVecBackup(obj->unk1C);
    return obj;
}

u32 func_00116250(s32 object) {
    return **(u32 **)(object + 0x18);
}

INCLUDE_ASM(const s32, "game/code_001161D8", func_00116260);
