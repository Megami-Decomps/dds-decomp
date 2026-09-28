#include "common.h"

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
    void (*unk28)(); /* 0x28 invoked by billInvokeCallback */
    u16 unk2C;       /* 0x2C kind */
    u16 unk2E;       /* 0x2E */
    void *unk30;     /* 0x30 child (kind 0) or data (kind 1) */
    u8 pad34[8];     /* 0x34 */
    u16 unk3C;       /* 0x3C kind-1 slot set by billAllocList */
    u8 pad3E[10];    /* 0x3E */
    u32 unk48;       /* 0x48 */
    u32 unk4C;       /* 0x4C */
    u16 unk50;       /* 0x50 set to 1 by billCloneList/billAllocList */
    u8 pad52[6];     /* 0x52 */
    u32 unk58;       /* 0x58 compared by func_00152200 */
    s32 unk5C;       /* 0x5C entry count read by func_00152288 */
    void *unk60;     /* 0x60 entry list */
} BillObj;

/* Kind-0 child (0x34 bytes from billAllocChild). Counters at +0x8/+0x14
   bumped by func_00151E60/billCloneList, entry count at +0xC. */
typedef struct BillData {
    u8 pad[8];    /* 0x0 */
    s32 unk8;     /* 0x8 */
    s32 unkC;     /* 0xC */
    u8 pad10[4];  /* 0x10 */
    s32 unk14;    /* 0x14 */
    u8 pad18[24]; /* 0x18 */
} BillData; /* 0x34 bytes */

typedef struct {
    s32 unk0;      /* 0x0 offset added to the table base */
    u8 pad4[0x10]; /* 0x4 */
} BillEntry; /* 0x14 bytes */

typedef struct {
    u8 pad[4];       /* 0x0 */
    s32 unk4;        /* 0x4 base added to the entry offset */
    BillEntry *unk8; /* 0x8 */
} BillTable;

typedef struct {
    s32 unk0;       /* 0x0 */
    u32 unk4;       /* 0x4 cleared on setup */
    s32 unk8;       /* 0x8 filled from the s16 at unk10 + 0x12 */
    BillEntry *unkC; /* 0xC */
    s32 unk10;      /* 0x10 table base + entry offset */
} BillOut;

/* Dispatch entry (0xC bytes). func creates an instance (billCreateIndexed)
   or runs a command on one (billDispatchByKind); unk4 is copied onto the new
   instance's unk28 by billCreateIndexed. */
typedef struct {
    void *(*func)(); /* 0x0 */
    void (*unk4)();  /* 0x4 */
    u32 unk8;        /* 0x8 */
} BillDispatch; /* 0xC bytes */

extern BillDispatch D_0034E060[];
extern BillDispatch D_0034E068[];

void *func_002CFEB8(s32 size);
void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
void func_002D0918(void *arg);
void func_002CFF98(void *arg);
void func_00150260(void *arg);
void func_001502B0(void *arg0, void *arg1);
void func_00151C58(void *arg);
void *func_00150148(void *arg);
void func_001518D8(BillObj *arg0, s32 arg1);
void *func_00151A88(void *arg);

INCLUDE_ASM(const s32, "effect/billManager", func_001502B0);

INCLUDE_ASM(const s32, "effect/billManager", func_00150750);

INCLUDE_ASM(const s32, "effect/billManager", func_00150840);

INCLUDE_ASM(const s32, "effect/billManager", func_00150EB0);

INCLUDE_ASM(const s32, "effect/billManager", func_00151010);

INCLUDE_ASM(const s32, "effect/billManager", func_00151178);

BillObj *billAllocChild(void *arg0) {
    BillObj *obj;

    obj = func_002CFEB8(0x34);
    obj->unk30 = NULL;
    if (arg0 != NULL) {
        obj->unk30 = func_00150148(arg0);
    }
    return obj;
}

void func_00151210(BillObj *obj) {
    if (obj->unk30 != NULL) {
        func_00150260(obj->unk30);
    }
    func_002CFF98(obj);
}

void func_00151248(BillObj *obj) {
    func_001502B0(obj, obj->unk30);
}

BillObj *billAllocList(void *arg0) {
    BillData *data;
    BillObj *newobj;
    s32 n;

    data = NULL;
    if (arg0 != NULL) {
        data = func_00151A88(arg0);
    }
    n = data->unkC;
    newobj = func_002CFEB8(n * 20 + 0x6C);
    newobj->unk30 = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    newobj->unk3C = 0;
    func_001518D8(newobj, 0);
    return newobj;
}

BillObj *billCloneList(BillObj *obj) {
    BillData *data;
    s32 n;
    BillObj *newobj;

    data = obj->unk30;
    n = data->unkC;
    data->unk14 = data->unk14 + 1;
    newobj = func_002CFEB8(n * 20 + 0x6C);
    newobj->unk30 = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    func_001518D8(newobj, 0);
    return newobj;
}

void func_00151368(BillObj *obj) {
    func_00151C58(obj->unk30);
    func_002CFF98(obj);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151398);

INCLUDE_ASM(const s32, "effect/billManager", func_00151568);

INCLUDE_ASM(const s32, "effect/billManager", func_001515E8);

void func_001518A0(BillTable *table, s32 index, BillOut *out) {
    s32 base;
    BillEntry *entry;
    s32 offset;
    s16 val;

    base = table->unk4;
    entry = table->unk8 + index;
    offset = entry->unk0;
    out->unkC = entry;
    base = base + offset;
    out->unk4 = 0;
    val = *(s16 *)(base + 0x12);
    out->unk10 = base;
    out->unk8 = val;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001518D8);

INCLUDE_ASM(const s32, "effect/billManager", func_00151A88);

INCLUDE_ASM(const s32, "effect/billManager", func_00151C58);

INCLUDE_ASM(const s32, "effect/billManager", func_00151CE8);

BillObj *billCreateIndexed(s32 index, u32 data) {
    BillObj *newobj;

    newobj = D_0034E060[index].func(data);
    func_00151178(newobj);
    newobj->unk2C = index;
    newobj->unk28 = D_0034E060[index].unk4;
    return newobj;
}

void *func_00151E08(s32 arg0, s32 arg1) {
    void *tmp;
    void *res;
    u32 buf[4];

    tmp = func_002EB028(arg1, buf, 0);
    res = billCreateIndexed(arg0, buf[0]);
    func_002D0918(tmp);
    return res;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151E60);

void billDispatchByKind(BillObj *obj) {
    D_0034E068[obj->unk2C].func();
}

void billInvokeCallback(BillObj *obj) {
    obj->unk28();
}
