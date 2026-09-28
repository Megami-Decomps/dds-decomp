#include "common.h"

extern u64 billCreateIndexed(u64, u32);

extern u64 func_00343ED0(u64, u32 *, u64);

/* Billboard instance. Kind in unk2C (0 = data-driven child in unk30,
   1 = entry list at unk60). Floats/int witnesses: defaults set by
   func_00151178, color/mode by code_00151F58 setters, child released by
   func_00151210, list compared by func_00152200. */
typedef struct BillObj {
    f32 unk0;        /* 0x0 */
    f32 unk4;        /* 0x4 */
    f32 unk8;        /* 0x8 */
    f32 unkC;        /* 0xC */
    f32 unk10;       /* 0x10 */
    f32 unk14;       /* 0x14 */
    f32 unk18;       /* 0x18 */
    f32 unk1C;       /* 0x1C */
    f32 unk20;       /* 0x20 */
    u32 unk24;       /* 0x24 */
    void (*unk28)(); /* 0x28 invoked by func_00151F38 */
    u16 unk2C;       /* 0x2C kind */
    u16 unk2E;       /* 0x2E */
    void *unk30;     /* 0x30 child (kind 0) or data (kind 1) */
    u8 pad34[8];     /* 0x34 */
    u16 unk3C;       /* 0x3C kind-1 slot set by func_00151260 */
    u8 pad3E[10];    /* 0x3E */
    u32 unk48;       /* 0x48 */
    u32 unk4C;       /* 0x4C */
    u16 unk50;       /* 0x50 set to 1 by func_001512E8/func_00151260 */
    u8 pad52[6];     /* 0x52 */
    u32 unk58;       /* 0x58 compared by func_00152200 */
    s32 unk5C;       /* 0x5C entry count read by func_00152288 */
    void *unk60;     /* 0x60 entry list */
} BillObj;

void *func_00328D68(s32 size);

void *func_00157D38(void *arg);

/* Kind-0 child (0x34 bytes from func_001511C0). Counters at +0x8/+0x14
   bumped by func_00151E60/func_001512E8, entry count at +0xC. */
typedef struct BillData {
    u8 pad[8];    /* 0x0 */
    s32 unk8;     /* 0x8 */
    s32 unkC;     /* 0xC */
    u8 pad10[4];  /* 0x10 */
    s32 unk14;    /* 0x14 */
    u8 pad18[24]; /* 0x18 */
} BillData; /* 0x34 bytes */

void func_001594C8(BillObj *arg0, s32 arg1);

void *func_00159678(void *arg);

/* Dispatch entry (0xC bytes). func creates an instance (func_00151D88)
   or runs a command on one (func_00151F00); unk4 is copied onto the new
   instance's unk28 by func_00151D88. */
typedef struct {
    void *(*func)(); /* 0x0 */
    void (*unk4)();  /* 0x4 */
    u32 unk8;        /* 0x8 */
} BillDispatch; /* 0xC bytes */

extern BillDispatch D_003AA998[];

INCLUDE_ASM(const s32, "effect/billManager", func_00157EA0);

INCLUDE_ASM(const s32, "effect/billManager", func_00158340);

INCLUDE_ASM(const s32, "effect/billManager", func_00158430);

INCLUDE_ASM(const s32, "effect/billManager", func_00158AA0);

INCLUDE_ASM(const s32, "effect/billManager", func_00158C00);

INCLUDE_ASM(const s32, "effect/billManager", func_00158D68);

BillObj *billAllocChild(void *arg0) {
    BillObj *obj;

    obj = func_00328D68(0x34);
    obj->unk30 = NULL;
    if (arg0 != NULL) {
        obj->unk30 = func_00157D38(arg0);
    }
    return obj;
}

void func_00158E00(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x30);
    if (temp_v0 != 0) {
        func_00157E50(temp_v0);
    }
    func_00328E48(arg0);
}

void func_00158E38(s32 arg0) {
    func_00157EA0(arg0, *(u32 *)(arg0 + 0x30));
}

BillObj *billAllocList(void *arg0) {
    BillData *data;
    BillObj *newobj;
    s32 n;

    data = NULL;
    if (arg0 != NULL) {
        data = func_00159678(arg0);
    }
    n = data->unkC;
    newobj = func_00328D68(n * 20 + 0x6C);
    newobj->unk30 = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    newobj->unk3C = 0;
    func_001594C8(newobj, 0);
    return newobj;
}

BillObj *billCloneList(BillObj *obj) {
    BillData *data;
    s32 n;
    BillObj *newobj;

    data = obj->unk30;
    n = data->unkC;
    data->unk14 = data->unk14 + 1;
    newobj = func_00328D68(n * 20 + 0x6C);
    newobj->unk30 = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    func_001594C8(newobj, 0);
    return newobj;
}

void func_00158F58(u32 arg0) {
    func_00159848(*(u32 *)((s32)arg0 + 0x30));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00158F88);

INCLUDE_ASM(const s32, "effect/billManager", func_00159158);

INCLUDE_ASM(const s32, "effect/billManager", func_001591D8);

void func_00159490(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 *piVar3;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 4);
    piVar3 = (s32 *)(*(s32 *)(arg0 + 8) + arg1 * 0x14);
    temp_v1 = *piVar3;
    *(s32 **)(arg2 + 0xc) = piVar3;
    temp_v2 = temp_v2 + temp_v1;
    *(u32 *)(arg2 + 4) = 0;
    temp_v0 = *(s16 *)(temp_v2 + 0x12);
    *(s32 *)(arg2 + 0x10) = temp_v2;
    *(s32 *)(arg2 + 8) = (s32)temp_v0;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001594C8);

INCLUDE_ASM(const s32, "effect/billManager", func_00159678);

INCLUDE_ASM(const s32, "effect/billManager", func_00159848);

INCLUDE_ASM(const s32, "effect/billManager", func_001598D8);

INCLUDE_ASM(const s32, "effect/billManager", billCreateIndexed);

u64 func_001599F8(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg1, temp_v2, 0);
    temp_v1 = billCreateIndexed(arg0, temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00159A50);

void billDispatchByKind(BillObj *obj) {
    D_003AA998[obj->unk2C].func();
}

void billInvokeCallback(BillObj *obj) {
    obj->unk28();
}
