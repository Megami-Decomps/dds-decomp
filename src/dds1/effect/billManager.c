#include "common.h"
#include "eff.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

typedef struct {
    s32 offset;    /* 0x0 offset added to the table base */
    u8 pad4[0x10]; /* 0x4 */
} BillEntry; /* 0x14 bytes */

typedef struct {
    u8 pad00[0x12];
    s16 value; /* 0x12 */
} BillRecord;

typedef struct {
    u8 pad[4];       /* 0x0 */
    s32 base;        /* 0x4 base added to the entry offset */
    BillEntry *entries; /* 0x8 */
} BillTable;

typedef struct {
    s32 unk0;       /* 0x0 */
    u32 unk4;       /* 0x4 cleared on setup */
    s32 recordValue; /* 0x8 read from the s16 at recordAddress + 0x12 */
    BillEntry *entry; /* 0xC */
    s32 recordAddress; /* 0x10 table base + entry offset */
} BillOut;

extern BillDispatch D_0034E060[];
extern BillDispatch D_0034E068[];

void *func_002CFEB8(s32 size);
void *func_002EB028(s32 arg0, u32 *arg1, s32 arg2);
void func_002D0918(void *arg);
void sdfReleaseChipBlock(void *arg);
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

void billReleaseChild(BillObj *obj) {
    if (obj->unk30 != NULL) {
        func_00150260(obj->unk30);
    }
    sdfReleaseChipBlock(obj);
}

void billProcessChild(BillObj *obj) {
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
    n = data->entryCount;
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
    n = data->entryCount;
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

void billReleaseList(BillObj *obj) {
    func_00151C58(obj->unk30);
    sdfReleaseChipBlock(obj);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151398);

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effBillModulateColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

INCLUDE_ASM(const s32, "effect/billManager", func_001515E8);

/* Resolves an indexed billboard record and caches its signed +0x12 value. */
void billResolveEntry(BillTable *table, s32 index, BillOut *out) {
    s32 base;
    BillEntry *entry;
    s32 offset;
    s16 value;

    base = table->base;
    entry = table->entries + index;
    offset = entry->offset;
    out->entry = entry;
    base = base + offset;
    out->unk4 = 0;
    value = ((BillRecord *)base)->value;
    out->recordAddress = base;
    out->recordValue = value;
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

void *billCreateFromResource(s32 kind, s32 resource) {
    void *allocation;
    void *billboard;
    u32 header[4];

    allocation = func_002EB028(resource, header, 0);
    billboard = billCreateIndexed(kind, header[0]);
    func_002D0918(allocation);
    return billboard;
}

extern void func_00151178(BillObj *obj);

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
BillObj *func_00151E60(BillObj *source) {
    BillObj *copy;
    BillData *data;

    if (source->unk2C == 1) {
        copy = billCloneList(source);
        func_00151178(copy);
        copy->unk2C = source->unk2C;
        copy->unk28 = source->unk28;
    } else {
        copy = billAllocChild(NULL);
        func_00151178(copy);
        copy->unk2C = source->unk2C;
        copy->unk28 = source->unk28;
        data = source->unk30;
        data->unk8 = data->unk8 + 1;
        copy->unk30 = data;
    }
    return copy;
}

void billDispatchByKind(BillObj *obj) {
    D_0034E068[obj->unk2C].func();
}

void billInvokeCallback(BillObj *obj) {
    obj->unk28();
}
