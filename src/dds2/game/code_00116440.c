#include "common.h"

typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    u32 *resource;
} WorldResourceOwner;

/* Create an inner-vector object and snapshot its vector state after initialization. */
typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u8 unk8[0x10]; /* 0x8 */
    u32 *value;    /* 0x18: nested value pointer */
    s32 unk1C;     /* 0x1C */
} ActionObj;

extern ActionObj *func_00110AA8();

extern void effObjSetInnerFirstVec();

extern void effObjSetInnerSecondVec();

extern void effObjInnerVecBackup();

ActionObj *dds3SpawnInnerVecObj8(s32 initialValue, void *firstVector, void *secondVector) {
    ActionObj *obj = func_00110AA8(8);

    obj->unk4 = initialValue;
    effObjSetInnerFirstVec(obj, firstVector);
    effObjSetInnerSecondVec(obj, secondVector);
    effObjInnerVecBackup(obj->unk1C);
    return obj;
}

u32 dds3GetResourceOwnerHandle(WorldResourceOwner *object) {
    return *object->resource;
}

INCLUDE_ASM(const s32, "game/code_00116440", func_001164C8);
