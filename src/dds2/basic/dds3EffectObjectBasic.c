#include "common.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "eff.h"

#define EFF_OBJ_KIND 7
#define EFF_OBJ_STATE_BOUND_BILL 1
#define EFF_OBJ_STATE_BILL_NODE 3
#define EFF_OBJ_STATE_EVENT_NODE 5
#define EFF_OBJ_STATE_PARAMETERS_READY 6
#define EFF_OBJ_STATE_MAGATUHI_TWO_ROWS 7
#define EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS 8
#define EFF_OBJ_VECTOR_BYTES 0x10
#define EFF_OBJ_ENTRY_ID_MASK 0xFFFF
#define EFF_OBJ_FLAG_OWNER_LINK 4
#define EFF_OBJ_FLAG_OWNER_BILL_ENTRY 8
#define EFF_OBJ_OWNER_KIND_FIRST 4
#define EFF_OBJ_OWNER_KIND_END 10
#define EFF_OBJ_OWNER_KIND_EXTRA 0x11
#define EFF_OBJ_OWNER_BILL_KIND 5

extern u64 func_001579C8(void);

extern u64 effLoadResourceNode(void);

struct EffNode;
struct EffNodeDescriptor;
extern struct EffNode *effCreateNodeFromDescriptor(struct EffNodeDescriptor *descriptor);

extern u64 billCreateFromResource(u64, u64);

extern u64 billCreateIndexed(u64, u64);

extern u64 billCloneObjectRetainingSharedData(u32);

extern s32 sdfLoadMapRecordLookAtBasis(void *param, s32 id);

extern void effEventReleaseNode(void *node);

extern void *effEventCreate(void *bill, u32 id, void *vec);

extern void *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData(void *obj);

extern void effObjSetInnerFirstVec(void *obj, void *vec);

extern void effObjSetInnerSecondVec(void *obj, void *vec);

extern void effObjInnerVecBackup(void *params);
extern void effMagatuhiCopyFloatBlock(void *, const void *);
extern void effMagatuhiSetControlPointParams(void *, const void *);

extern void *sdfReadNamedResource(void *resource, u32 *resolvedId, s32 options);

extern void *sdfReleaseResourceAllocation(void *arg);

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
    void *vector; /* 0x28 passed to effEventCreate */
    void *node; /* 0x2C released and replaced by effObjReplaceActiveEventNode */
} EffectData; /* 0x30 bytes */


typedef struct EffectObj {
    u8 pad0[4];       /* 0x0 */
    u32 worldCounter; /* 0x4 copied from the constructor's worldCounter */
    u8 pad8[7];       /* 0x8 */
    u8 kind;           /* 0xF checked ==7 by effObjGetReadyData */
    u8 pad10[8];       /* 0x10 */
    EffectData *data; /* 0x18 */
    ObjectTransform *params; /* Separately allocated position, rotation and scale owner. */
} EffectObj;

EffectData *effObjGetReadyData(EffectObj *obj);

/* Release dependencies, object base, then slot data; the object and data must exist. */
void effObjReleaseObjectData(u32 object) {
    EffectData *data;
    EffectObj *obj;

    obj = (EffectObj *)object;
    data = obj->data;
    func_00114640(data);
    effObjFreeInner(object);
    dds3DestroyObjectBase((u32)data->objectHandle);
    sdfReleaseChipBlock((u32)obj->data);
    obj->data = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114828);

struct BillObj;
struct EffNode;

extern void func_00116078(EffectObj *obj);
extern u8 dds3TestObjectFlags(u32 unused, u32 flags);
extern void effUpdateNode(struct EffNode *node);
extern void billInvokeCallback(struct BillObj *bill);
extern void func_00197F60(void *node);

s32 func_00114BF0(EffectObj *obj) {
    EffectData *data;

    data = obj->data;
    func_00116078(obj);
    if ((data->flags & 1) == 0) {
        return 1;
    }
    if (dds3TestObjectFlags((u32)obj, 1)) {
        return 1;
    }

    switch (data->state) {
    case 1:
    case 7:
    case 8:
        if (data->bill != NULL) {
            effUpdateNode(data->bill);
        }
        break;
    case 2:
    case 3:
        if (data->bill != NULL) {
            billInvokeCallback(data->bill);
        }
        break;
    case 5:
        if (data->node != NULL) {
            func_00197F60(data->node);
        }
        break;
    }
    return 1;
}

/* Return the object-base address as the native u32 handle, without validation. */
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


extern u32 dds3AdvanceWorldCounter(void);
extern void *dds3GetFirstWorldObjectNodeOfKind2(void);
extern void effCopyVector(void *source, void *destination);

EffectObj *effObjCreateKindTwo(bill, vec, extra)
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
    data->bill = bill;
    data->flags = 0;
    data->state = 2;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle(obj);
    ((ObjBase *)handle)->resourceState = 2;
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        ((ObjBase *)handle)->slots[5] = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

/* Clone the shared bill and forward both vector addresses; retain the wide DDS2 ABI. */
void effObjSpawnSharedBillClone(EffectObj *obj, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = billCloneObjectRetainingSharedData((u32)obj->data->bill);
    effObjCreateKindTwo(bill, firstVectorAddress, secondVectorAddress);
}
/* Create a kind-one indexed bill; the native constructor result is discarded. */
void effObjCreateIndexedKindOne(u64 billId, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = billCreateIndexed(1, billId);
    effObjCreateKindTwo(bill, firstVectorAddress, secondVectorAddress);
}
/* Create a kind-one resource bill; the native constructor result is discarded. */
void effObjCreateResourceKindOne(u64 resourceId, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = billCreateFromResource(1, resourceId);
    effObjCreateKindTwo(bill, firstVectorAddress, secondVectorAddress);
}

/* Select the stored bill's kind-one entry; no object/data/bill checks are made. */
void func_00114F30(EffectObj *obj) {
    billSetKind1Entry((u32)obj->data->bill);
}

/* Create a state-three bill effect and attach the first kind-two world node if present.
   K&R declarations preserve the native vector-pointer and integer-address arguments. */
EffectObj *effObjCreateBillNode(bill, firstVector, secondVectorAddress)
    void *bill;
    void *firstVector;
    s32 secondVectorAddress;
{
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    EffectObj *obj;
    EffectData *data;
    void *objectHandle;
    void *worldNode;

    memset(copiedVector, 0, sizeof(copiedVector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), firstVector, (void *)secondVectorAddress);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, firstVector);
    VU0_STORE_VF(vf10, copiedVector);
    effCopyVector(bill, copiedVector);
    data = obj->data;
    data->state = EFF_OBJ_STATE_BILL_NODE;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = effObjGetObjectHandle(obj);
    ((ObjBase *)objectHandle)->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        ((ObjBase *)objectHandle)->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

/* Clone the shared bill for the state-three constructor; ignore its return value. */
void effObjSpawnSharedBillNodeClone(EffectObj *obj, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = billCloneObjectRetainingSharedData((u32)obj->data->bill);
    effObjCreateBillNode(bill, firstVectorAddress, secondVectorAddress);
}

/* Create a kind-zero indexed bill for the state-three constructor. */
void effObjCreateIndexedKindZero(u64 billId, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = billCreateIndexed(0, billId);
    effObjCreateBillNode(bill, firstVectorAddress, secondVectorAddress);
}

/* Create a kind-zero resource bill for the state-three constructor. */
void effObjCreateResourceKindZero(u64 resourceId, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = billCreateFromResource(0, resourceId);
    effObjCreateBillNode(bill, firstVectorAddress, secondVectorAddress);
}

/* Bind the supplied bill through its node-instance vector copy, selecting state one.
   The object-base state word remains a separate native value of two. */
EffectObj *effObjCreateWithBoundBill(bill, firstVector, secondVectorAddress)
    void *bill;
    void *firstVector;
    s32 secondVectorAddress;
{
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    EffectObj *obj;
    EffectData *data;
    void *objectHandle;
    void *worldNode;

    memset(copiedVector, 0, sizeof(copiedVector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), firstVector, (void *)secondVectorAddress);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, firstVector);
    VU0_STORE_VF(vf10, copiedVector);
    effCopyVectorToNodeInstance(bill, copiedVector);
    data = obj->data;
    data->state = EFF_OBJ_STATE_BOUND_BILL;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = effObjGetObjectHandle(obj);
    ((ObjBase *)objectHandle)->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        ((ObjBase *)objectHandle)->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

/* Create the descriptor-sourced node and return its bound world object. */
EffectObj *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, s32 secondVectorAddress) {
    struct EffNode *bill;

    bill = effCreateNodeFromDescriptor(descriptor);
    return effObjCreateWithBoundBill(bill, firstVector, secondVectorAddress);
}

/* Create the loaded-resource node, then bind it; constructor failure is not returned. */
void effObjSpawnLoadedResourceEffect(u64 unused, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = effLoadResourceNode();
    effObjCreateWithBoundBill(bill, firstVectorAddress, secondVectorAddress);
}

/* The world-bill entry uses the same state-one/node-instance initialization as binding. */
EffectObj *effObjCreateBillboardInWorld(bill, firstVector, secondVectorAddress)
    void *bill;
    void *firstVector;
    s32 secondVectorAddress;
{
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    EffectObj *obj;
    EffectData *data;
    void *objectHandle;
    void *worldNode;

    memset(copiedVector, 0, sizeof(copiedVector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), firstVector, (void *)secondVectorAddress);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, firstVector);
    VU0_STORE_VF(vf10, copiedVector);
    effCopyVectorToNodeInstance(bill, copiedVector);
    data = obj->data;
    data->state = EFF_OBJ_STATE_BOUND_BILL;
    data->bill = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = effObjGetObjectHandle(obj);
    ((ObjBase *)objectHandle)->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        ((ObjBase *)objectHandle)->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

/* Feed the third source factory's result to the world-bill constructor. */
void func_00115318(u64 unused, u64 firstVectorAddress, u64 secondVectorAddress) {
    u64 bill;

    bill = func_001579C8();
    effObjCreateBillboardInWorld(bill, firstVectorAddress, secondVectorAddress);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115358);

void func_00115500(void) {
    func_00115358();
}

/* Resolve the resource identifier, create from it, then release the temporary resource.
   The legacy constructor call and its argument types are deliberately unchanged. */
void *effObjCreateFromResolvedResource(void *resource, void *firstVector, void *secondVector) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    created = func_00115358(resolvedId, firstVector, secondVector);
    sdfReleaseResourceAllocation(resourceHandle);
    return created;
}

/* For state-five data with a bill and vector, release the old event node and replace it.
   The lookup uses only the low sixteen bits; its result replaces the released node. */
void effObjReplaceActiveEventNode(EffectObj *obj, u32 entryId) {
    EffectData *data;

    data = obj->data;
    if (data->state != EFF_OBJ_STATE_EVENT_NODE) {
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
    data->node = effEventCreate(data->bill, entryId & EFF_OBJ_ENTRY_ID_MASK, data->vector);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115600);

void func_001156C8(void) {
    func_00115600();
}

extern const f32 D_00412980[10];
extern const f32 D_004129A8[20];

EffectObj *effObjCreateMagatuhiForKind(kind, descriptor)
    s32 kind;
    struct EffNodeDescriptor *descriptor;
{
    f32 firstVector[4];
    f32 secondVector[4];
    f32 twoRows[10];
    f32 fourRows[20];
    struct EffNode *bill;
    EffectObj *obj;
    EffectData *data;
    void *objectHandle;
    void *worldNode;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    secondVector[3] = 1.0f;
    memcpy(twoRows, D_00412980, sizeof(twoRows));
    memcpy(fourRows, D_004129A8, sizeof(fourRows));
    bill = effCreateNodeFromDescriptor(descriptor);
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), firstVector,
                                 secondVector);
    if (obj == NULL) {
        return NULL;
    }
    data = obj->data;
    data->flags = 0;
    if (kind == 1) {
        data->state = EFF_OBJ_STATE_MAGATUHI_TWO_ROWS;
    } else {
        data->state = EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS;
    }
    data->bill = bill;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    data->node = sdfAllocSizeClassBlock(0x10);
    memset(data->node, 0, 0x10);
    switch (kind) {
    case 1:
        data->vector = sdfAllocSizeClassBlock(sizeof(twoRows));
        memcpy(data->vector, twoRows, sizeof(twoRows));
        effMagatuhiCopyFloatBlock(bill, data->vector);
        break;
    case 2:
        data->vector = sdfAllocSizeClassBlock(sizeof(fourRows));
        memcpy(data->vector, fourRows, sizeof(fourRows));
        effMagatuhiSetControlPointParams(bill, data->vector);
        break;
    }
    objectHandle = effObjGetObjectHandle(obj);
    ((ObjBase *)objectHandle)->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        ((ObjBase *)objectHandle)->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

void func_00115AA8(void) {
    effObjCreateMagatuhiForKind();
}

/* Resolve a named resource for the kind/value constructor, then release that resource. */
void *effObjCreateKindFromResource(s32 kind, void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    created = effObjCreateMagatuhiForKind(kind, resolvedId);
    sdfReleaseResourceAllocation(resourceHandle);
    return created;
}

/* Return data only for a non-NULL effect-kind object in parameter-ready state.
   Matching objects are still assumed to contain valid slot data. */
EffectData *effObjGetReadyData(EffectObj *obj) {
    EffectData *data;

    data = NULL;
    if (obj == NULL) {
        return data;
    }
    if (obj->kind != EFF_OBJ_KIND) {
        return data;
    }
    data = obj->data;
    if (data->state != EFF_OBJ_STATE_PARAMETERS_READY) {
        data = NULL;
    }
    return data;
}

/* Load the ready parameter vector into vf10; return one for success, zero otherwise. */
s32 effObjLoadReadyParameterVector(EffectObj *obj) {
    if (effObjGetReadyData(obj) == NULL) {
        return 0;
    }
    VU0_LOAD_VF(vf10, obj->params->position);
    return 1;
}

/* Truncate the ready scalar float to s32; zero also represents unavailable data. */
s32 effObjGetIntParam(EffectObj *obj) {
    ObjectTransform *parameters;

    if (effObjGetReadyData(obj) == NULL) {
        return 0;
    }
    parameters = obj->params;
    return (s32)parameters->scale[0];
}

extern void effMagatuhiInitializeInterpolatedHistory(void *bill);
extern void effMagatuhiDispatchByKind(void *bill);

/* Dispatch bound-bill setup or four-row history initialization; two-row state does nothing.
   Native implicit-int fall-through leaves the return value unspecified. */
effObjDispatchReadyState(EffectObj *obj) {
    if (obj->kind == EFF_OBJ_KIND) {
        EffectData *data = obj->data;

        switch (data->state) {
        case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
            break;
        case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
            effMagatuhiInitializeInterpolatedHistory(data->bill);
            break;
        case EFF_OBJ_STATE_BOUND_BILL:
            effMagatuhiDispatchByKind(data->bill);
            break;
        }
    }
}

/* Copy ready inputs into two or four vector rows, then dispatch the whole existing block.
   An unavailable input leaves its destination row/scalar unchanged, not zeroed. */
s32 effObjCopyMagatuhiSourceParameters(EffectObj *obj, EffectObj *first, EffectObj *second, EffectObj *third, EffectObj *fourth)
{
    EffectData *data;
    f32 *parameterRows;

    if (obj->kind != EFF_OBJ_KIND) {
        return 0;
    }
    data = obj->data;
    switch (data->state) {
    case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
        parameterRows = data->vector;
        if (effObjLoadReadyParameterVector(first)) {
            VU0_STORE_VF(vf10, parameterRows);
            parameterRows[8] = effObjGetIntParam(first);
        }
        if (effObjLoadReadyParameterVector(second)) {
            VU0_STORE_VF(vf10, parameterRows + 4);
            parameterRows[9] = effObjGetIntParam(second);
        }
        effMagatuhiCopyFloatBlock(data->bill, parameterRows);
        return 1;
    case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
        parameterRows = data->vector;
        if (effObjLoadReadyParameterVector(first)) {
            VU0_STORE_VF(vf10, parameterRows);
            parameterRows[16] = effObjGetIntParam(first);
        }
        if (effObjLoadReadyParameterVector(second)) {
            VU0_STORE_VF(vf10, parameterRows + 4);
            parameterRows[17] = effObjGetIntParam(second);
        }
        if (effObjLoadReadyParameterVector(third)) {
            VU0_STORE_VF(vf10, parameterRows + 8);
            parameterRows[18] = effObjGetIntParam(third);
        }
        if (effObjLoadReadyParameterVector(fourth)) {
            VU0_STORE_VF(vf10, parameterRows + 12);
            parameterRows[19] = effObjGetIntParam(fourth);
        }
        effMagatuhiSetControlPointParams(data->bill, parameterRows);
        return 1;
    }
    return 0;
}

/* OR the requested flag bits into slot data; no object/data validation is performed. */
void effObjSetFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags | flags;
}

/* Clear the requested flag bits; retain the unsigned mask parameter used by DDS2. */
void effObjClearFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags & ~flags;
}

/* Accept owner kinds [4,10) or seventeen, reset link bookkeeping, and return one.
   Unsupported kinds return zero without changing the destination; owner must exist. */
s32 effObjBindValidatedOwner(EffectObj *obj, EffectObj *owner) {
    EffectData *data;
    u8 kind;

    kind = owner->kind;
    if (kind < EFF_OBJ_OWNER_KIND_FIRST || (kind >= EFF_OBJ_OWNER_KIND_END && kind != EFF_OBJ_OWNER_KIND_EXTRA)) {
        return 0;
    }
    data = obj->data;
    data->unk18 = NULL;
    data->owner = owner;
    data->entryId = 0;
    data->flags |= EFF_OBJ_FLAG_OWNER_LINK;
    data->flags &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
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

/* Forward the saved entry only for flagged kind-five owners whose bill state is zero.
   Owner/bill/parameter pointers are trusted once the flag selects this path. */
void effObjForwardOwnerBillEntry(EffectObj *obj) {
    EffectData *data;
    EffectObj *owner;
    BillPayload *bill;

    data = obj->data;
    if (data->flags & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
        owner = data->owner;
        if (owner->kind == EFF_OBJ_OWNER_BILL_KIND) {
            bill = owner->data->bill;
            if (bill->state != 0) {
                return;
            }
            sdfLoadMapRecordLookAtBasis(((BillParam *)bill->params)->param, data->entryId);
        }
    }
}

/* Save a truncated u16 entry and enable forwarding after owner validation.
   Native status is always one, including rejected owners. */
s32 effObjBindOwnerBillEntry(EffectObj *obj, EffectObj *owner, s32 entryId) {
    EffectData *data;

    if (effObjBindValidatedOwner(obj, owner) == 0) {
        return 1;
    }
    data = obj->data;
    data->entryId = entryId;
    data->flags |= EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
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

