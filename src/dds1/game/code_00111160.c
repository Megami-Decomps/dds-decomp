#include "common.h"

s32 func_00111160(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

extern ActionObj *func_00110880();

ActionObj *evtSpawnActionObj2(s32 firstValue, s32 secondValue) {
    ActionObj *obj = func_00110880(2);

    obj->unk4 = firstValue;
    obj->unk8 = secondValue;
    return obj;
}

extern s32 dds3GetWorldSlotValue();
extern u32 dds3ResetObjectValueCursor();
extern s32 dds3SeekWorldNode();
extern void func_001102C8();
extern u32 dds3WriteIndexedWorldObjectWord();

void func_001111C8(s32 object, u8 *node) {
    s32 slot = dds3GetWorldSlotValue(object, func_00111160(node[0xF]));

    dds3ResetObjectValueCursor(slot);
    if (dds3SeekWorldNode(slot, node) != 1) {
        func_001102C8(slot, 1);
        dds3WriteIndexedWorldObjectWord(slot, node);
    }
}

INCLUDE_ASM(const s32, "game/code_00111160", func_00111258);

/* Read a 32-bit value from the object's array of world slots. */
s32 dds3GetWorldSlotValue(u8 *object, s32 index) {
    return *(s32 *)(*(u8 **)(object + 0x18) + (index << 2));
}

INCLUDE_ASM(const s32, "game/code_00111160", func_001112C0);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111388);

extern void *func_002CFEB8(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    u32 *work;
} ObjWithWork;

s32 dds3AllocateClearedObjectWork(ObjWithWork *obj) {
    obj->work = func_002CFEB8(0x10);
    memset(obj->work, 0, 0x10);
    return 1;
}

void dds3ReleaseWorldSlotResource(ObjWithWork *obj) {
    u32 *slot;

    slot = obj->work;
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*slot, 0, 1);
    sdfReleaseChipBlock(slot);
}
