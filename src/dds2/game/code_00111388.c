#include "common.h"

typedef struct WorldSlotObject {
    u8 pad00[0x18];
    u32 *slots;
} WorldSlotObject;

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

extern ActionObj *func_00110AA8();

extern void *func_00328D68(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    u32 *work;
} ObjWithWork;

s32 func_00111388(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}

ActionObj *evtSpawnActionObj2(s32 firstValue, s32 secondValue) {
    ActionObj *obj = func_00110AA8(2);

    obj->unk4 = firstValue;
    obj->unk8 = secondValue;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_00111388", func_001113F0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111480);

/* Read a 32-bit value from the object's array of world slots. */
s32 dds3GetWorldSlotValue(u8 *object, s32 index) {
    return *(s32 *)(*(u8 **)(object + 0x18) + (index << 2));
}

INCLUDE_ASM(const s32, "game/code_00111388", func_001114E8);

INCLUDE_ASM(const s32, "game/code_00111388", func_001115B0);

s32 func_00111628(ObjWithWork *obj) {
    obj->work = func_00328D68(0x10);
    memset(obj->work, 0, 0x10);
    return 1;
}

void dds3ReleaseWorldSlotResource(WorldSlotObject *object) {
    u32 *resource;

    resource = object->slots;
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*resource, 0, 1);
    sdfReleaseChipBlock(resource);
}
