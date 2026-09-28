#include "common.h"
#include "eff.h"

extern u64 billCreateIndexed(u64, u32);

extern u64 func_00343ED0(u64, u32 *, u64);

void *func_00328D68(s32 size);

void *func_00157D38(void *arg);

void func_001594C8(BillObj *arg0, s32 arg1);

void *func_00159678(void *arg);

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

void func_00158E00(BillObj *obj) {
    s32 child;

    child = (s32)obj->unk30;
    if (child != 0) {
        func_00157E50(child);
    }
    func_00328E48(obj);
}

void func_00158E38(BillObj *obj) {
    func_00157EA0(obj, obj->unk30);
}

BillObj *billAllocList(void *arg0) {
    BillData *data;
    BillObj *newobj;
    s32 n;

    data = NULL;
    if (arg0 != NULL) {
        data = func_00159678(arg0);
    }
    n = data->entryCount;
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
    n = data->entryCount;
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
