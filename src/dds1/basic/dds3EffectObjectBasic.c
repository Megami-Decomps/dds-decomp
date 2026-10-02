#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    void *objectHandle; /* 0x0 passed to dds3DestroyObjectBase, returned by effObjGetObjectHandle */
    u32 flags;   /* 0x4 effect flag bits */
    s32 state;   /* 0x8: checked for states 5 (node update) and 6 (parameter access) */
    void *bill;   /* 0xC passed to billCloneObjectRetainingSharedData/billSetKind1Entry */
    u32 unk10;   /* 0x10 cleared by effObjBindValidatedOwner */
    u32 unk14;   /* 0x14 cleared by effObjBindValidatedOwner */
    void *unk18; /* 0x18 cleared by effObjBindValidatedOwner */
    u8 pad1C[4]; /* 0x1C */
    void *owner; /* 0x20 owning effect object */
    u16 entryId; /* 0x24 forwarded to the bill parameter lookup */
    u16 ownerKind; /* 0x26 copied from owner's kind */
    void *vector; /* 0x28 passed to func_00190130 */
    void *node; /* 0x2C released and replaced by effObjReplaceActiveEventNode */
} EffectData; /* 0x30 bytes */

typedef struct {
    u8 pad0[0x60];
    f32 parameter;
} EffectParameters;

typedef struct {
    u8 pad0[4];              /* 0x0 */
    u32 worldCounter; /* 0x4 copied from the constructor's worldCounter */
    u8 pad8[7];              /* 0x8 */
    u8 kind;                 /* 0xF checked ==7 by effObjGetReadyData */
    u8 pad10[8];             /* 0x10 */
    EffectData *data;         /* 0x18 */
    EffectParameters *params; /* 0x1C vector base read by effObjLoadReadyParameterVector/effObjGetIntParam */
} EffectObj;

void func_001143D8(void *arg);
void effObjFreeInner(void *arg);
void dds3DestroyObjectBase(void *arg);
void sdfReleaseChipBlock(void *arg);
/* Dispatchers take (bill handle, 16-byte vector, extra); the vector is
   loaded with lqc2 and the extra is forwarded to effObjCreateWithVectors. */
EffectObj *effObjCreateWithBill(void *bill, void *vec, s32 extra);
void billSetKind1Entry(void *arg);
EffectObj *effObjCreateBillNode(void *bill, void *vec, s32 extra);
EffectObj *effObjCreateWithBoundBill(void *bill, void *vec, s32 extra);
EffectObj *func_00114FE0(void *bill, void *vec, s32 extra);
void *func_001150F0();
void func_00115398(void);
/* Old-style (K&R) callee: callers pass (kind, value) positionally. */
void *func_00115478();
EffectData *effObjGetReadyData(EffectObj *obj);
void *func_002D0918(void *arg);
void *sdfReadNamedResource(void *resource, u32 *resolvedId, s32 options);

extern void *func_0014FE28(void);

extern void *effLoadResourceNode(void);

extern void *func_0014FD20(void);
extern u32 dds3AdvanceWorldCounter(void);
extern void effCopyVector(void *source, void *destination);
extern void effCopyVectorToNodeInstance(void *source, void *destination);

extern void *dds3GetFirstWorldObjectNodeOfKind2(void);
extern void dds3EnsureWorldNodeInSlot(void *id, void *owner);

extern void *billCreateFromResource(s32 kind, s32 resourceId);

extern void *billCreateIndexed(s32 kind, u32 billId);

extern void *billCloneObjectRetainingSharedData(void *arg);

extern s32 sdfLoadMapRecordLookAtBasis(void *param, s32 id);

extern void effEventReleaseNode(void *node);

extern void *func_00190130(void *bill, u32 id, void *vec);

extern EffectObj *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData(void *obj);

extern void effObjSetInnerFirstVec(void *obj, void *vec);

extern void effObjSetInnerSecondVec(void *obj, void *vec);

extern void effObjInnerVecBackup(void *params);



/* Release the effect's dependent resources before clearing its data handle. */
void effObjReleaseObjectData(EffectObj *obj) {
    EffectData *data;

    data = obj->data;
    func_001143D8(data);
    effObjFreeInner(obj);
    dds3DestroyObjectBase(data->objectHandle);
    sdfReleaseChipBlock(obj->data);
    obj->data = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001145C0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114988);

void *effObjGetObjectHandle(EffectObj *obj) {
    return obj->data->objectHandle;
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

EffectObj *effObjCreateWithBill(void *bill, void *vec, s32 extra) {
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
    data->bill = bill;
    data->flags = 0;
    data->state = 2;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    *(s32 *)((u8 *)handle + 8) = 2;
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

/* Resolve the object's billboard and forward its vector and extra argument. */
void func_00114BF0(EffectObj *obj, void *vec, s32 extra) {
    void *handle;

    handle = billCloneObjectRetainingSharedData(obj->data->bill);
    effObjCreateWithBill(handle, vec, extra);
}

/* Create an indexed billboard of kind one and dispatch it. */
void effObjCreateIndexedKindOne(u32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateIndexed(1, billId);
    effObjCreateWithBill(handle, vec, extra);
}

/* Create a resource-backed billboard of kind one and dispatch it. */
void effObjCreateResourceKindOne(s32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateFromResource(1, billId);
    effObjCreateWithBill(handle, vec, extra);
}

void func_00114CC8(EffectObj *obj) {
    billSetKind1Entry(obj->data->bill);
}

EffectObj *effObjCreateBillNode(void *bill, void *vec, s32 extra) {
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
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

void func_00114DB8(EffectObj *obj, void *vec, s32 extra) {
    void *handle;

    handle = billCloneObjectRetainingSharedData(obj->data->bill);
    effObjCreateBillNode(handle, vec, extra);
}

void effObjCreateIndexedKindZero(u32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateIndexed(0, billId);
    effObjCreateBillNode(handle, vec, extra);
}

void effObjCreateResourceKindZero(s32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateFromResource(0, billId);
    effObjCreateBillNode(handle, vec, extra);
}

EffectObj *effObjCreateWithBoundBill(void *bill, void *vec, s32 extra) {
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
    effCopyVectorToNodeInstance(bill, vector);
    data = obj->data;
    data->state = 1;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    *(s32 *)((u8 *)handle + 8) = 2;
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

void func_00114F60(u32 unused, void *vec, s32 extra) {
    void *handle;

    handle = func_0014FD20();
    effObjCreateWithBoundBill(handle, vec, extra);
}

void func_00114FA0(u32 unused, void *vec, s32 extra) {
    void *handle;

    handle = effLoadResourceNode();
    effObjCreateWithBoundBill(handle, vec, extra);
}

EffectObj *func_00114FE0(void *bill, void *vec, s32 extra) {
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
    effCopyVectorToNodeInstance(bill, vector);
    data = obj->data;
    data->state = 1;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    *(s32 *)((u8 *)handle + 8) = 2;
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        *(void **)((u8 *)handle + 0x24) = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

void func_001150B0(u32 unused, void *vec, s32 extra) {
    void *handle;

    handle = func_0014FE28();
    func_00114FE0(handle, vec, extra);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F0);

void func_00115298(void) {
    func_001150F0();
}

void *effObjCreateFromResolvedResource(void *resource, void *vec, void *extra) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    created = func_001150F0(resolvedId, vec, extra);
    func_002D0918(resourceHandle);
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
    data->node = func_00190130(data->bill, entryId & 0xFFFF, data->vector);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115398);

void func_00115460(void) {
    func_00115398();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115478);

void func_00115840(void) {
    func_00115478();
}

void *effObjCreateKindFromResource(s32 kind, void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    created = func_00115478(kind, resolvedId);
    func_002D0918(resourceHandle);
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

extern void effMagatuhiInitializeInterpolatedHistory(void *bill);
extern void effMagatuhiDispatchByKind(void *bill);

/* Run the Magatuhi setup matching the ready effect's state (1 or 8). */
func_00115970(EffectObj *obj) {
    if (obj->kind == 7) {
        EffectData *data = obj->data;

        switch (data->state) {
        case 7:
            break;
        case 8:
            effMagatuhiInitializeInterpolatedHistory(data->bill);
            break;
        case 1:
            effMagatuhiDispatchByKind(data->bill);
            break;
        }
    }
}
INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001159E8);

void effObjSetFlags(EffectObj *obj, s32 flags) {
    obj->data->flags |= flags;
}

void effObjClearFlags(EffectObj *obj, s32 flags) {
    obj->data->flags &= ~flags;
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

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F7D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F800);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F828);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F878);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F890);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8C0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8E8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F900);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F918);

