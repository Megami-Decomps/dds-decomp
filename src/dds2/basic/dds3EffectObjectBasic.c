#include "common.h"
#include "pcp_vu0.h"

extern u64 func_001579C8(void);

extern u64 func_001579E8(void);

extern u64 func_001578C0(void);

extern u64 billCreateFromResource(u64, u64);

extern u64 billCreateIndexed(u64, u64);

extern u64 billCloneObjectRetainingSharedData(u32);

extern s32 sdfLoadMapRecordLookAtBasis(void *param, s32 id);

extern void effEventReleaseNode(void *node);

extern void *func_00197D68(void *bill, u32 id, void *vec);

extern void *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData(void *obj);

extern void effObjSetInnerFirstVec(void *obj, void *vec);

extern void effObjSetInnerSecondVec(void *obj, void *vec);

extern void effObjInnerVecBackup(void *params);

extern void *func_00343ED0(void *resource, u32 *resolvedId, s32 options);

extern void *func_003297C8(void *arg);

typedef struct {
    void *objectHandle; /* 0x0 returned by effObjGetObjectHandle */
    u32 flags; /* 0x4 effect flag bits */
    s32 state;   /* 0x8: checked for states 5 (node update) and 6 (parameter access) */
    void *bill; /* 0xC bill object */
    u32 unk10;   /* 0x10 cleared when an owner is attached */
    u32 unk14;   /* 0x14 cleared when an owner is attached */
    void *unk18; /* 0x18 cleared when an owner is attached */
    u8 pad1C[4]; /* 0x1C */
    void *owner; /* 0x20 owning effect object */
    u16 entryId; /* 0x24 forwarded to the bill parameter lookup */
    u16 ownerKind; /* 0x26 copied from owner's kind */
    void *vector; /* 0x28 passed to func_00197D68 */
    void *node; /* 0x2C released and replaced by effObjReplaceActiveEventNode */
} EffectData; /* 0x30 bytes */

typedef struct {
    u8 pad0[0x60];
    f32 parameter;
} EffectParameters;

typedef struct {
    u8 pad0[4];       /* 0x0 */
    u32 worldCounter; /* 0x4 copied from the constructor's worldCounter */
    u8 pad8[7];       /* 0x8 */
    u8 kind;           /* 0xF checked ==7 by effObjGetReadyData */
    u8 pad10[8];       /* 0x10 */
    EffectData *data; /* 0x18 */
    EffectParameters *params; /* 0x1C vector base; scalar parameter at +0x60 */
} EffectObj;

EffectData *effObjGetReadyData(EffectObj *obj);

/* Release the effect's dependent resources before clearing its data handle. */
void effObjReleaseObjectData(u32 object) {
    EffectData *data;
    EffectObj *obj;

    obj = (EffectObj *)object;
    data = obj->data;
    func_00114640(data);
    effObjFreeInner(object);
    func_00111A68((u32)data->objectHandle);
    sdfReleaseChipBlock((u32)obj->data);
    obj->data = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114828);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114BF0);

u32 effObjGetObjectHandle(EffectObj *obj) {
    return (u32)obj->data->objectHandle;
}

EffectObj *effObjCreateWithVectors(u32 worldCounter, void *firstVec, void *secondVec) {
    EffectObj *obj;
    EffectData *data;

    obj = dds3AppendWorldObjectNode(7);
    if (obj == NULL) {
        return NULL;
    }
    obj->worldCounter = worldCounter;
    dds3EnsureSlotData(obj);
    effObjSetInnerFirstVec(obj, firstVec);
    effObjSetInnerSecondVec(obj, secondVec);
    effObjInnerVecBackup(obj->params);
    data = obj->data;
    data->flags = 0;
    data->state = 0;
    data->bill = NULL;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114D80);

/* Resolve the object's billboard and forward its two draw arguments. */
void func_00114E58(EffectObj *obj, u64 vector, u64 extra) {
    u64 bill;

    bill = billCloneObjectRetainingSharedData((u32)obj->data->bill);
    func_00114D80(bill, vector, extra);
}
/* Create an indexed billboard of kind one and dispatch it. */
void effObjCreateIndexedKindOne(u64 billId, u64 vector, u64 extra) {
    u64 bill;

    bill = billCreateIndexed(1, billId);
    func_00114D80(bill, vector, extra);
}
/* Create a resource-backed billboard of kind one and dispatch it. */
void effObjCreateResourceKindOne(u64 resourceId, u64 vector, u64 extra) {
    u64 bill;

    bill = billCreateFromResource(1, resourceId);
    func_00114D80(bill, vector, extra);
}

/* Select the billboard object's kind-one entry. */
void func_00114F30(EffectObj *obj) {
    billSetKind1Entry((u32)obj->data->bill);
}

extern u32 dds3AdvanceWorldCounter(void);
extern void *func_001115B0(void);
extern void effCopyVector(void *source, void *destination);
extern void func_00157790(void *source, void *destination);


EffectObj *func_00114F50(bill, vec, extra)
    void *bill;
    void *vec;
    s32 extra;
{
    u8 vector[0x10];
    EffectObj *obj;
    EffectData *data;
    void *handle;
    void *id;

    memset(vector, 0, sizeof(vector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), vec, (void *)extra);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, vec);
    VU0_STORE_VF(vf10, vector);
    effCopyVector(bill, vector);
    data = obj->data;
    data->state = 3;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    *(s32 *)((u8 *)handle + 8) = 2;
    id = func_001115B0();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

/* Resolve the object's billboard and dispatch through the second handler. */
void func_00115020(EffectObj *obj, u64 vector, u64 extra) {
    u64 bill;

    bill = billCloneObjectRetainingSharedData((u32)obj->data->bill);
    func_00114F50(bill, vector, extra);
}

/* Create an indexed billboard of kind zero for the second handler. */
void effObjCreateIndexedKindZero(u64 billId, u64 vector, u64 extra) {
    u64 bill;

    bill = billCreateIndexed(0, billId);
    func_00114F50(bill, vector, extra);
}

/* Create a resource-backed billboard of kind zero for the second handler. */
void effObjCreateResourceKindZero(u64 resourceId, u64 vector, u64 extra) {
    u64 bill;

    bill = billCreateFromResource(0, resourceId);
    func_00114F50(bill, vector, extra);
}

EffectObj *func_001150F8(bill, vec, extra)
    void *bill;
    void *vec;
    s32 extra;
{
    u8 vector[0x10];
    EffectObj *obj;
    EffectData *data;
    void *handle;
    void *id;

    memset(vector, 0, sizeof(vector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), vec, (void *)extra);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, vec);
    VU0_STORE_VF(vf10, vector);
    func_00157790(bill, vector);
    data = obj->data;
    data->state = 1;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    *(s32 *)((u8 *)handle + 8) = 2;
    id = func_001115B0();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

/* Dispatch a newly allocated handle from the first parameter source. */
void func_001151C8(u64 unused, u64 vector, u64 extra) {
    u64 handle;

    handle = func_001578C0();
    func_001150F8(handle, vector, extra);
}

/* Dispatch a newly allocated handle from the second parameter source. */
void func_00115208(u64 unused, u64 vector, u64 extra) {
    u64 handle;

    handle = func_001579E8();
    func_001150F8(handle, vector, extra);
}

EffectObj *func_00115248(bill, vec, extra)
    void *bill;
    void *vec;
    s32 extra;
{
    u8 vector[0x10];
    EffectObj *obj;
    EffectData *data;
    void *handle;
    void *id;

    memset(vector, 0, sizeof(vector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), vec, (void *)extra);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, vec);
    VU0_STORE_VF(vf10, vector);
    func_00157790(bill, vector);
    data = obj->data;
    data->state = 1;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    *(s32 *)((u8 *)handle + 8) = 2;
    id = func_001115B0();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

/* Dispatch a newly allocated handle from the third parameter source. */
void func_00115318(u64 unused, u64 vector, u64 extra) {
    u64 handle;

    handle = func_001579C8();
    func_00115248(handle, vector, extra);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115358);

void func_00115500(void) {
    func_00115358();
}

void *effObjCreateFromResolvedResource(void *resource, void *vector, void *extra) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = func_00343ED0(resource, &resolvedId, 0);
    created = func_00115358(resolvedId, vector, extra);
    func_003297C8(resourceHandle);
    return created;
}

void effObjReplaceActiveEventNode(EffectObj *obj, u32 entryId) {
    EffectData *data;

    data = obj->data;
    if (data->state != 5) {
        return;
    }
    if (data->vector == NULL) {
        return;
    }
    if (data->bill == NULL) {
        return;
    }
    if (data->node != NULL) {
        effEventReleaseNode(data->node);
        data->node = NULL;
    }
    data->node = func_00197D68(data->bill, entryId & 0xFFFF, data->vector);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115600);

void func_001156C8(void) {
    func_00115600();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001156E0);

void func_00115AA8(void) {
    func_001156E0();
}

void *effObjCreateKindFromResource(s32 kind, void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = func_00343ED0(resource, &resolvedId, 0);
    created = func_001156E0(kind, resolvedId);
    func_003297C8(resourceHandle);
    return created;
}

EffectData *effObjGetReadyData(EffectObj *obj) {
    EffectData *data;

    data = NULL;
    if (obj == NULL) {
        return data;
    }
    if (obj->kind != 7) {
        return data;
    }
    data = obj->data;
    if (data->state != 6) {
        data = NULL;
    }
    return data;
}

s32 effObjLoadReadyParameterVector(EffectObj *obj) {
    if (effObjGetReadyData(obj) == NULL) {
        return 0;
    }
    VU0_LOAD_VF(vf10, (u8 *)obj->params + 0x40);
    return 1;
}

/* Return the object's scalar parameter as an integer when data is present. */
s32 effObjGetIntParam(EffectObj *obj) {
    EffectParameters *parameters;

    if (effObjGetReadyData(obj) == NULL) {
        return 0;
    }
    parameters = obj->params;
    return (s32)parameters->parameter;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BD8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C50);

void effObjSetFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags | flags;
}

void effObjClearFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags & ~flags;
}

s32 effObjBindValidatedOwner(EffectObj *obj, EffectObj *owner) {
    EffectData *data;
    u8 kind;

    kind = owner->kind;
    if (kind < 4 || (kind >= 10 && kind != 0x11)) {
        return 0;
    }
    data = obj->data;
    data->unk18 = NULL;
    data->owner = owner;
    data->entryId = 0;
    data->flags |= 4;
    data->flags &= ~8;
    data->unk10 = 0;
    data->ownerKind = owner->kind;
    data->unk14 = 0;
    return 1;
}

/* Billboard payload: state word and the SDF parameter block it was built from. */
typedef struct {
    u8 pad0[8];   /* 0x0 */
    u32 state;     /* 0x8: only zero permits forwarding the lookup */
    void *params;  /* 0xC: parameter block */
} BillPayload;

typedef struct {
    u8 pad0[0x18]; /* 0x0 */
    void *param;   /* 0x18: forwarded to sdfLoadMapRecordLookAtBasis */
} BillParam;

void effObjForwardOwnerBillEntry(EffectObj *obj) {
    EffectData *data;
    EffectObj *owner;
    BillPayload *bill;

    data = obj->data;
    if (data->flags & 8) {
        owner = data->owner;
        if (owner->kind == 5) {
            bill = owner->data->bill;
            if (bill->state != 0) {
                return;
            }
            sdfLoadMapRecordLookAtBasis(((BillParam *)bill->params)->param, data->entryId);
        }
    }
}

s32 effObjBindOwnerBillEntry(EffectObj *obj, EffectObj *owner, s32 entryId) {
    EffectData *data;

    if (effObjBindValidatedOwner(obj, owner) == 0) {
        return 1;
    }
    data = obj->data;
    data->entryId = entryId;
    data->flags |= 8;
    return 1;
}

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412950);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412980);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129F8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A10);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A28);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A40);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A50);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A68);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A80);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A98);

