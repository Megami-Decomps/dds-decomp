#include "common.h"

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u8 unk8[0x10]; /* 0x8 */
    u32 *value;    /* 0x18: nested value pointer */
    s32 unk1C;     /* 0x1C */
} ActionObj;

extern ActionObj *func_00110880();
extern void dds3EnsureSlotData();

extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();

/* Create an inner-vector object and snapshot its vector state after initialization. */
ActionObj *dds3SpawnInnerVecObj8(s32 initialValue, void *firstVector, void *secondVector) {
    ActionObj *obj = func_00110880(8);

    obj->unk4 = initialValue;
    effObjSetInnerFirstVec(obj, firstVector);
    effObjSetInnerSecondVec(obj, secondVector);
    effObjInnerVecBackup(obj->unk1C);
    return obj;
}

u32 dds3GetResourceOwnerHandle(ActionObj *object) {
    return *object->value;
}

INCLUDE_ASM(const s32, "game/code_001161D8", func_00116260);
