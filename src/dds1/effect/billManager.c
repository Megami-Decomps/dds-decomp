#include "common.h"

/* Billboard instance. Field witnesses: +0x28/+0x2C/+0x30 are copied by
   func_00151D88/func_00151E60, +0x30 holds the child released by
   func_00151210, and +0x58 is compared by func_00152200. */
typedef struct BillObj {
    u8 pad[0x28];   /* 0x0 */
    void *unk28;    /* 0x28 */
    u16 unk2C;      /* 0x2C */
    u8 pad2E[2];    /* 0x2E */
    void *unk30;    /* 0x30 */
    u8 pad34[0x24]; /* 0x34 */
    u32 unk58;      /* 0x58 */
} BillObj;

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

void *func_00151D88(s32 index, u32 data);
void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
void func_002D0918(void *arg);
void func_002CFF98(void *arg);
void func_00150260(void *arg);
void func_001502B0(void *arg0, void *arg1);
void func_00151C58(void *arg);

INCLUDE_ASM(const s32, "effect/billManager", func_001502B0);

INCLUDE_ASM(const s32, "effect/billManager", func_00150750);

INCLUDE_ASM(const s32, "effect/billManager", func_00150840);

INCLUDE_ASM(const s32, "effect/billManager", func_00150EB0);

INCLUDE_ASM(const s32, "effect/billManager", func_00151010);

INCLUDE_ASM(const s32, "effect/billManager", func_00151178);

INCLUDE_ASM(const s32, "effect/billManager", func_001511C0);

void func_00151210(BillObj *obj) {
    if (obj->unk30 != NULL) {
        func_00150260(obj->unk30);
    }
    func_002CFF98(obj);
}

void func_00151248(BillObj *obj) {
    func_001502B0(obj, obj->unk30);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151260);

INCLUDE_ASM(const s32, "effect/billManager", func_001512E8);

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

INCLUDE_ASM(const s32, "effect/billManager", func_00151D88);

void *func_00151E08(s32 arg0, s32 arg1) {
    void *tmp;
    void *res;
    u32 buf[4];

    tmp = func_002EB028(arg1, buf, 0);
    res = func_00151D88(arg0, buf[0]);
    func_002D0918(tmp);
    return res;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151E60);

INCLUDE_ASM(const s32, "effect/billManager", func_00151F00);

INCLUDE_ASM(const s32, "effect/billManager", func_00151F38);
