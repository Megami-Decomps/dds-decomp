#include "common.h"
#include "ee_mmi.h"
#include "eff.h"
#include "pcp_vu0.h"

/* The entry offset is relative to the table's record base. */
typedef struct {
    s32 offset;
    u8 pad4[0x10];
} BillEntry; /* 0x14 bytes */

typedef struct {
    u8 pad00[0x12];
    s16 value;
} BillRecord;

typedef struct {
    u8 pad[4];
    s32 base;
    BillEntry *entries;
} BillTable;

typedef struct {
    s32 unk0;
    u32 unk4;
    s32 recordValue;
    BillEntry *entry;
    s32 recordAddress;
} BillOut;

extern BillObj *billCreateIndexed(s32 index, u32 data);

extern u64 sdfReadNamedResource(u64, u32 *, u64);

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

BillObj *billAllocChild(void *resourceData) {
    BillObj *obj;

    obj = func_00328D68(0x34);
    obj->entryList = NULL;
    if (resourceData != NULL) {
        obj->entryList = func_00157D38(resourceData);
    }
    return obj;
}

void billReleaseChild(BillObj *obj) {
    s32 child;

    child = (s32)obj->entryList;
    if (child != 0) {
        effReleaseSharedTextureRecord(child);
    }
    sdfReleaseChipBlock(obj);
}

void billProcessChild(BillObj *obj) {
    func_00157EA0(obj, obj->entryList);
}

BillObj *billAllocList(void *resourceData) {
    BillData *data;
    BillObj *newobj;
    s32 n;

    data = NULL;
    if (resourceData != NULL) {
        data = func_00159678(resourceData);
    }
    n = data->entryCount;
    newobj = func_00328D68(n * 20 + 0x6C);
    newobj->entryList = data;
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

    data = obj->entryList;
    n = data->entryCount;
    data->listRefCount = data->listRefCount + 1;
    newobj = func_00328D68(n * 20 + 0x6C);
    newobj->entryList = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    func_001594C8(newobj, 0);
    return newobj;
}

void billReleaseList(BillObj *obj) {
    billReleaseSharedEntryBlock(obj->entryList);
    sdfReleaseChipBlock(obj);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00158F88);

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

INCLUDE_ASM(const s32, "effect/billManager", func_001591D8);

/* Resolves an indexed billboard record and caches its signed +0x12 value. */
void billResolveEntry(BillTable *table, s32 index, BillOut *out) {
    s16 kind;
    s32 offset;
    BillEntry *entry;
    s32 base;

    base = table->base;
    entry = table->entries + index;
    offset = entry->offset;
    out->entry = entry;
    base = base + offset;
    out->unk4 = 0;
    kind = ((BillRecord *)base)->value;
    out->recordAddress = base;
    out->recordValue = (s32)kind;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001594C8);

INCLUDE_ASM(const s32, "effect/billManager", func_00159678);

/* Shared entry-list block: reference count at +0x14, entry count at +0x10 and entry pointers at +0x18. */
typedef struct BillEntryBlock {
    void *allocation; /* 0x00 */
    u8 pad04[0xC];
    s32 entryCount;   /* 0x10 */
    s32 refCount;     /* 0x14 */
    void **entries;   /* 0x18 */
} BillEntryBlock;

extern void effReleaseSharedTextureRecord(void *arg);

/* Drop one reference; the last one releases every entry and the block itself. */
void billReleaseSharedEntryBlock(void *arg) {
    BillEntryBlock *block = arg;
    s32 i;

    block->refCount--;
    if (block->refCount == 0) {
        i = 0;
        while (i < block->entryCount) {
            effReleaseSharedTextureRecord(block->entries[i]);
            i++;
        }
        func_003297C8(block->allocation);
    }
}

typedef struct BillVec4 {
    f32 v[4];
} BillVec4;

typedef struct BillSourceRecord {
    u32 unk00;          /* 0x00 */
    u8 pad04[8];
    BillVec4 vector;    /* 0x0C */
    f32 x;              /* 0x1C */
    f32 y;              /* 0x20 */
    f32 z;              /* 0x24 */
    f32 w;              /* 0x28 */
} BillSourceRecord;

typedef struct BillSnapshot {
    f32 x;              /* 0x00 */
    f32 y;              /* 0x04 */
    f32 z;              /* 0x08 */
    f32 w;              /* 0x0C */
    u32 unk10;          /* 0x10 */
    BillVec4 vector;    /* 0x14 */
} BillSnapshot;

extern BillSourceRecord *func_00158F88(BillObj *obj, void *entries);

/* Copy the billboard's current source record (by kind) into a snapshot. */
void billCopyCurrentRecordToSnapshot(BillObj *obj, BillSnapshot *snapshot) {
    BillSourceRecord *record;

    if (obj->kind == 1) {
        record = func_00158F88(obj, obj->unk60);
    } else if (obj->kind == 0 || obj->kind == 3) {
        record = obj->entryList;
    } else {
        return;
    }
    snapshot->x = record->x;
    snapshot->unk10 = record->unk00;
    snapshot->y = record->y;
    snapshot->z = record->z;
    snapshot->w = record->w;
    snapshot->vector = record->vector;
}

extern BillDispatch D_003AA990[];

extern void func_00158D68(void *);

BillObj *billCreateIndexed(s32 index, u32 data) {
    BillObj *newobj;

    newobj = D_003AA990[index].func(data);
    func_00158D68(newobj);
    newobj->kind = index;
    newobj->callback = D_003AA990[index].callback;
    return newobj;
}

u64 billCreateFromResource(u32 owner, u64 resource) {
    u64 allocation;
    BillObj *billboard;
    u32 header[4];

    allocation = sdfReadNamedResource(resource, header, 0);
    billboard = billCreateIndexed(owner, header[0]);
    func_003297C8(allocation);
    return billboard;
}

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
BillObj *billCloneObjectRetainingSharedData(BillObj *source) {
    BillObj *copy;
    BillData *data;

    if (source->kind == 1) {
        copy = billCloneList(source);
        func_00158D68(copy);
        copy->kind = source->kind;
        copy->callback = source->callback;
    } else {
        copy = billAllocChild(NULL);
        func_00158D68(copy);
        copy->kind = source->kind;
        copy->callback = source->callback;
        data = source->entryList;
        data->childRefCount = data->childRefCount + 1;
        copy->entryList = data;
    }
    return copy;
}

void billDispatchByKind(BillObj *obj) {
    D_003AA998[obj->kind].func();
}

void billInvokeCallback(BillObj *obj) {
    obj->callback();
}
