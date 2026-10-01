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

extern s32 dds3GetWorldSlotValue();
extern u32 dds3ResetObjectValueCursor();
extern s32 dds3SeekWorldNode();
extern void func_001104F0();
extern u32 dds3WriteIndexedWorldObjectWord();

void func_001113F0(s32 object, u8 *node) {
    s32 slot = dds3GetWorldSlotValue(object, func_00111388(node[0xF]));

    dds3ResetObjectValueCursor(slot);
    if (dds3SeekWorldNode(slot, node) != 1) {
        func_001104F0(slot, 1);
        dds3WriteIndexedWorldObjectWord(slot, node);
    }
}

INCLUDE_ASM(const s32, "game/code_00111388", func_00111480);

/* Read a 32-bit value from the object's array of world slots. */
s32 dds3GetWorldSlotValue(u8 *object, s32 index) {
    return *(s32 *)(*(u8 **)(object + 0x18) + (index << 2));
}

extern u32 func_00110628();
extern void *dds3AppendWorldIndexNode();
extern u32 dds3ReadIndexedWorldObjectWord();
extern u32 dds3AdvanceObjectValueCursor();
extern void dds3DestroyWorldIndexNode();

/* Copy the slot's world-object words (optionally filtered) into a fresh index node. */
void *func_001114E8(s32 object, s32 index, s32 (*filter)(u32)) {
    s32 slot = dds3GetWorldSlotValue(object, index);
    void *result;
    u32 word;

    if (func_00110628(slot) == 0) {
        return NULL;
    }
    result = dds3AppendWorldIndexNode(0);
    dds3ResetObjectValueCursor(slot);
    do {
        word = dds3ReadIndexedWorldObjectWord(slot);
        if (filter == NULL || filter(word) != 0) {
            func_001104F0(result, 1);
            dds3WriteIndexedWorldObjectWord(result, word);
        }
    } while (dds3AdvanceObjectValueCursor(slot) != 0);
    if (func_00110628(result) == 0) {
        dds3DestroyWorldIndexNode(result);
        return NULL;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00111388", func_001115B0);

s32 dds3AllocateClearedObjectWork(ObjWithWork *obj) {
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
