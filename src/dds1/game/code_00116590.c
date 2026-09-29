#include "common.h"

typedef struct {
    u8 pad0[8];
    u32 unitHandle;
    u8 padC[0x58];
    u32 flags;
    u8 pad68[0xC];
    u32 value74;
} UnitObjectData;

typedef struct {
    u8 pad0[0x18];
    UnitObjectData *data;
} UnitObject;

u32 func_00116590(void) {
    return 1;
}

u32 func_00116598(UnitObject *obj) {
    return obj->data->value74;
}

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u8 unk8[0x14]; /* 0x8 */
    s32 unk1C;    /* 0x1C */
} ActionObj;

extern ActionObj *func_00110880();
extern void func_00112750();

ActionObj *func_001165A8(s32 a) {
    ActionObj *obj = func_00110880(9);

    obj->unk4 = a;
    func_00112750(obj);
    return obj;
}

void func_001165F0(void) {
    func_00110928();
}

INCLUDE_ASM(const s32, "game/code_00116590", func_00116608);

void dds3ClearUnitObjectLowFlags(UnitObject *obj) {
    obj->data->flags = obj->data->flags & 0xfffffffc;
}

void func_001166D0(u32 value, UnitObject *obj) {
    func_00221BE0(obj->data->unitHandle, value);
}

void func_001166F0(UnitObject *obj) {
    func_00221C50(obj->data->unitHandle);
}

INCLUDE_ASM(const s32, "game/code_00116590", func_00116710);

INCLUDE_ASM(const s32, "game/code_00116590", func_001167B8);

INCLUDE_ASM(const s32, "game/code_00116590", func_00116820);
