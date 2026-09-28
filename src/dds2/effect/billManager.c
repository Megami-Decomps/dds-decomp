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

void billReleaseChild(BillObj *obj) {
    s32 child;

    child = (s32)obj->unk30;
    if (child != 0) {
        func_00157E50(child);
    }
    func_00328E48(obj);
}

void billProcessChild(BillObj *obj) {
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

void func_00158F58(BillObj *obj) {
    func_00159848(obj->unk30);
    func_00328E48(obj);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00158F88);

INCLUDE_ASM(const s32, "effect/billManager", func_00159158);

INCLUDE_ASM(const s32, "effect/billManager", func_001591D8);

void billResolveEntry(s32 table, s32 index, s32 output) {
    s16 kind;
    s32 data;
    s32 *entry;
    s32 base;

    base = *(s32 *)(table + 4);
    entry = (s32 *)(*(s32 *)(table + 8) + index * 0x14);
    data = *entry;
    *(s32 **)(output + 0xc) = entry;
    base = base + data;
    *(u32 *)(output + 4) = 0;
    kind = *(s16 *)(base + 0x12);
    *(s32 *)(output + 0x10) = base;
    *(s32 *)(output + 8) = (s32)kind;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001594C8);

INCLUDE_ASM(const s32, "effect/billManager", func_00159678);

INCLUDE_ASM(const s32, "effect/billManager", func_00159848);

INCLUDE_ASM(const s32, "effect/billManager", func_001598D8);

INCLUDE_ASM(const s32, "effect/billManager", billCreateIndexed);

u64 func_001599F8(u64 owner, u64 resource) {
    u64 allocation;
    u64 billboard;
    u32 header[4];

    allocation = func_00343ED0(resource, header, 0);
    billboard = billCreateIndexed(owner, header[0]);
    func_003297C8(allocation);
    return billboard;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00159A50);

void billDispatchByKind(BillObj *obj) {
    D_003AA998[obj->unk2C].func();
}

void billInvokeCallback(BillObj *obj) {
    obj->unk28();
}
