#include "common.h"
#include "eff_dependency.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "eff.h"
#include "sdf_draw.h"

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



typedef struct {
    u8 pad0[0x40];
    f32 vector[4];
    u8 pad50[0x10];
    f32 parameter;
} EffectParameters;

typedef struct EffectObj {
    u8 pad0[4];              /* 0x0 */
    u32 worldCounter; /* 0x4 copied from the constructor's worldCounter */
    u8 pad8[7];              /* 0x8 */
    u8 kind;                 /* 0xF checked ==7 by effObjGetReadyData */
    u8 pad10[8];             /* 0x10 */
    EffectDependencyState *data;         /* 0x18 */
    EffectParameters *params; /* 0x1C vector base read by effObjLoadReadyParameterVector/effObjGetIntParam */
} EffectObj;

void effObjFreeInner(void *arg);
void dds3DestroyObjectBase(void *arg);
void sdfReleaseChipBlock(void *arg);
/* Dispatchers take (bill handle, 16-byte vector, extra); the vector is
   loaded with lqc2 and the extra is forwarded to effObjCreateWithVectors. */
EffectObj *effObjCreateKindTwo(void *bill, void *vec, s32 extra);
void billSetKind1Entry(void *arg);
EffectObj *effObjCreateBillNode(void *bill, void *vec, s32 extra);
EffectObj *effObjCreateWithBoundBill(void *bill, void *vec, s32 extra);
EffectObj *effObjCreateBillboardInWorld(void *bill, void *vec, s32 extra);
void *func_001150F0();
void func_00115398(void);
/* Old-style (K&R) callee: callers pass (kind, value) positionally. */
EffectObj *effObjCreateMagatuhiForKind();
extern const f32 D_0039F800[10];
extern const f32 D_0039F828[20];
extern void *sdfAllocSizeClassBlock(s32 size);
EffectDependencyState *effObjGetReadyData(EffectObj *obj);
void *sdfReleaseResourceAllocation(void *arg);
struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);

extern void *func_0014FE28(void);

extern void *effLoadResourceNode(void);

struct EffNode;
struct EffNodeDescriptor;
extern struct EffNode *effCreateNodeFromDescriptor(struct EffNodeDescriptor *descriptor);
extern u32 dds3AdvanceWorldCounter(void);
extern void effCopyVector(void *source, void *destination);

extern EffWorldNode *dds3GetFirstWorldObjectNodeOfKind2(void);
extern void dds3EnsureWorldNodeInSlot(void *id, void *owner);

extern BillObj *billCreateFromResource(s32 kind, const char *path);

extern void *billCreateIndexed(s32 kind, u32 billId);

extern void *billCloneObjectRetainingSharedData(void *arg);

extern s32 sdfLoadMapRecordLookAtBasis(SdfModel *model, s32 id);

extern void effEventReleaseNode(void *node);

extern void *effEventCreate(void *bill, u32 id, void *vec);

extern EffectObj *dds3AppendWorldObjectNode(s32 kind);

extern void dds3EnsureSlotData(void *obj);

extern void effObjSetInnerFirstVec(void *obj, void *vec);

extern void effObjSetInnerSecondVec(void *obj, void *vec);

extern void effObjInnerVecBackup(void *params);
extern void effMagatuhiCopyFloatBlock(void *, const void *);
extern void effMagatuhiSetControlPointParams(void *, const void *);



/* Release dependencies, object base, then slot data; the object and data must exist. */
void effObjReleaseObjectData(EffectObj *obj) {
    EffectDependencyState *data;

    data = obj->data;
    effObjReleaseStateDependencies(data);
    effObjFreeInner(obj);
    dds3DestroyObjectBase(data->objectHandle);
    sdfReleaseChipBlock(obj->data);
    obj->data = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001145C0);

struct BillObj;
struct EffNode;

extern u8 dds3TestObjectFlags(void *obj, s32 flags);
extern void effUpdateNode(struct EffNode *node);
extern void billInvokeCallback(struct BillObj *bill);
extern void func_00190328(void *node);

s32 func_00114988(EffectObj *obj) {
    EffectDependencyState *data;

    data = obj->data;
    effUpdateConfiguredBillboard((EffWorldNode *)obj);
    if ((data->flags & 1) == 0) {
        return 1;
    }
    if (dds3TestObjectFlags(obj, 1)) {
        return 1;
    }

    switch (data->state) {
    case 1:
    case 7:
    case 8:
        if (data->handle != NULL) {
            effUpdateNode(data->handle);
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            billInvokeCallback(data->handle);
        }
        break;
    case 5:
        if (data->node != NULL) {
            func_00190328(data->node);
        }
        break;
    }
    return 1;
}

/* Return the object-base handle without checking the object or its slot data. */
ObjBase *effObjGetObjectHandle(EffWorldNode *object) {
    return ((EffectDependencyState *)object->data)->objectHandle;
}

EffectObj *effObjCreateWithVectors(u32 worldCounter, void *firstVec, void *secondVec) {
    EffectObj *obj;
    EffectDependencyState *data;

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
    data->handle = NULL;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    return obj;
}


EffectObj *effObjCreateKindTwo(void *bill, void *vec, s32 extra) {
    u8 vector[0x10];
    EffectObj *obj;
    EffectDependencyState *data;
    ObjBase *handle;
    EffWorldNode *id;

    memset(vector, 0, sizeof(vector));
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), vec, (void *)extra);
    if (obj == NULL) {
        return NULL;
    }
    VU0_LOAD_VF(vf10, vec);
    VU0_STORE_VF(vf10, vector);
    effCopyVector(bill, vector);
    data = obj->data;
    data->handle = bill;
    data->flags = 0;
    data->state = 2;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = effObjGetObjectHandle((EffWorldNode *)obj);
    handle->resourceState = 2;
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        handle->slots[5] = id;
        dds3EnsureWorldNodeInSlot(id, obj);
    }
    return obj;
}

/* Clone the shared bill and forward both vector arguments; discard constructor failure. */
void effObjSpawnSharedBillClone(EffectObj *obj, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCloneObjectRetainingSharedData(obj->data->handle);
    effObjCreateKindTwo(bill, firstVector, secondVectorAddress);
}

/* Create a kind-one indexed bill; the native constructor result is discarded. */
void effObjCreateIndexedKindOne(u32 billId, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCreateIndexed(1, billId);
    effObjCreateKindTwo(bill, firstVector, secondVectorAddress);
}

/* Create a kind-one resource bill; the native constructor result is discarded. */
void effObjCreateResourceKindOne(const char *path, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCreateFromResource(1, path);
    effObjCreateKindTwo(bill, firstVector, secondVectorAddress);
}

/* Select the stored bill's kind-one entry; no object/data/bill checks are made. */
void func_00114CC8(EffectObj *obj) {
    billSetKind1Entry(obj->data->handle);
}

/* Create a state-three bill effect and attach the first kind-two world node if present.
   secondVectorAddress retains the integer ABI used for the second vector pointer. */
EffectObj *effObjCreateBillNode(void *bill, void *firstVector, s32 secondVectorAddress) {
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    EffectObj *obj;
    EffectDependencyState *data;
    ObjBase *objectHandle;
    EffWorldNode *worldNode;

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
    data->handle = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = effObjGetObjectHandle((EffWorldNode *)obj);
    objectHandle->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

/* Clone the shared bill for the state-three constructor; ignore its return value. */
void effObjSpawnSharedBillNodeClone(EffectObj *obj, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCloneObjectRetainingSharedData(obj->data->handle);
    effObjCreateBillNode(bill, firstVector, secondVectorAddress);
}

/* Create a kind-zero indexed bill for the state-three constructor. */
void effObjCreateIndexedKindZero(u32 billId, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCreateIndexed(0, billId);
    effObjCreateBillNode(bill, firstVector, secondVectorAddress);
}

/* Create a kind-zero resource bill for the state-three constructor. */
void effObjCreateResourceKindZero(const char *path, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCreateFromResource(0, path);
    effObjCreateBillNode(bill, firstVector, secondVectorAddress);
}

/* Bind the supplied bill through its node-instance vector copy, selecting state one.
   The object-base state word remains a separate native value of two. */
EffectObj *effObjCreateWithBoundBill(void *bill, void *firstVector, s32 secondVectorAddress) {
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    EffectObj *obj;
    EffectDependencyState *data;
    ObjBase *objectHandle;
    EffWorldNode *worldNode;

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
    data->handle = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = effObjGetObjectHandle((EffWorldNode *)obj);
    objectHandle->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[5] = worldNode;
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
void effObjSpawnLoadedResourceEffect(u32 unused, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = effLoadResourceNode();
    effObjCreateWithBoundBill(bill, firstVector, secondVectorAddress);
}

/* The world-bill entry uses the same state-one/node-instance initialization as binding. */
EffectObj *effObjCreateBillboardInWorld(void *bill, void *firstVector, s32 secondVectorAddress) {
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    EffectObj *obj;
    EffectDependencyState *data;
    ObjBase *objectHandle;
    EffWorldNode *worldNode;

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
    data->handle = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = effObjGetObjectHandle((EffWorldNode *)obj);
    objectHandle->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

/* Feed the third source factory's result to the world-bill constructor. */
void func_001150B0(u32 unused, void *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = func_0014FE28();
    effObjCreateBillboardInWorld(bill, firstVector, secondVectorAddress);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F0);

void func_00115298(void) {
    func_001150F0();
}

/* Resolve the resource identifier, create from it, then release the temporary resource.
   The legacy constructor call and its argument types are deliberately unchanged. */
void *effObjCreateFromResolvedResource(void *resource, void *firstVector, void *secondVector) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    created = func_001150F0(resolvedId, firstVector, secondVector);
    sdfReleaseResourceAllocation(resourceHandle);
    return created;
}

/* For state-five data with a bill and vector, release the old event node and replace it.
   The lookup uses only the low sixteen bits; its result replaces the released node. */
void effObjReplaceActiveEventNode(EffectObj *obj, u32 entryId) {
    EffectDependencyState *data;

    data = obj->data;
    if (data->state != EFF_OBJ_STATE_EVENT_NODE) {
        return;
    }
    if (data->vector == NULL) {
        return;
    }
    if (data->handle == NULL) {
        return;
    }
    if (data->node != NULL) {
        effEventReleaseNode(data->node);
        data->node = NULL;
    }
    data->node = effEventCreate(data->handle, entryId & EFF_OBJ_ENTRY_ID_MASK, data->vector);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115398);

void func_00115460(void) {
    func_00115398();
}

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
    EffectDependencyState *data;
    ObjBase *objectHandle;
    EffWorldNode *worldNode;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    secondVector[3] = 1.0f;
    memcpy(twoRows, D_0039F800, sizeof(twoRows));
    memcpy(fourRows, D_0039F828, sizeof(fourRows));
    bill = effCreateNodeFromDescriptor(descriptor);
    obj = effObjCreateWithVectors(dds3AdvanceWorldCounter(), firstVector, secondVector);
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
    data->handle = bill;
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
    objectHandle = effObjGetObjectHandle((EffWorldNode *)obj);
    objectHandle->resourceState = 2;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[5] = worldNode;
        dds3EnsureWorldNodeInSlot(worldNode, obj);
    }
    return obj;
}

EffectObj *func_00115840(s32 kind, struct EffNodeDescriptor *descriptor) {
    return effObjCreateMagatuhiForKind(kind, descriptor);
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
EffectDependencyState *effObjGetReadyData(EffectObj *obj) {
    EffectDependencyState *data;

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
    VU0_LOAD_VF(vf10, obj->params->vector);
    return 1;
}

/* Truncate the ready scalar float to s32; zero also represents unavailable data. */
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

/* Dispatch bound-bill setup or four-row history initialization; two-row state does nothing.
   Native implicit-int fall-through leaves the return value unspecified. */
effObjDispatchReadyState(EffectObj *obj) {
    if (obj->kind == EFF_OBJ_KIND) {
        EffectDependencyState *data = obj->data;

        switch (data->state) {
        case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
            break;
        case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
            effMagatuhiInitializeInterpolatedHistory(data->handle);
            break;
        case EFF_OBJ_STATE_BOUND_BILL:
            effMagatuhiDispatchByKind(data->handle);
            break;
        }
    }
}
/* Copy ready inputs into two or four vector rows, then dispatch the whole existing block.
   An unavailable input leaves its destination row/scalar unchanged, not zeroed. */
s32 effObjCopyMagatuhiSourceParameters(EffectObj *obj, EffectObj *first, EffectObj *second, EffectObj *third, EffectObj *fourth)
{
    EffectDependencyState *data;
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
        effMagatuhiCopyFloatBlock(data->handle, parameterRows);
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
        effMagatuhiSetControlPointParams(data->handle, parameterRows);
        return 1;
    }
    return 0;
}

/* OR the requested flag bits into slot data; no object/data validation is performed. */
void effObjSetFlags(EffectObj *obj, s32 flags) {
    obj->data->flags |= flags;
}

/* Clear the requested flag bits; retain the signed mask parameter used by DDS1. */
void effObjClearFlags(EffectObj *obj, s32 flags) {
    obj->data->flags &= ~flags;
}

/* Accept owner kinds [4,10) or seventeen, reset link bookkeeping, and return one.
   Unsupported kinds return zero without changing the destination; owner must exist. */
s32 effObjBindValidatedOwner(EffectObj *obj, EffectObj *owner) {
    EffectDependencyState *data;
    u8 kind;

    kind = owner->kind;
    if (kind < EFF_OBJ_OWNER_KIND_FIRST || (kind >= EFF_OBJ_OWNER_KIND_END && kind != EFF_OBJ_OWNER_KIND_EXTRA)) {
        return 0;
    }
    data = obj->data;
    data->word18 = NULL;
    data->owner = owner;
    data->entryId = 0;
    data->flags |= EFF_OBJ_FLAG_OWNER_LINK;
    data->flags &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    data->word10 = 0;
    data->ownerKind = owner->kind;
    data->word14 = 0;
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
    EffectDependencyState *data;
    EffectObj *owner;
    BillPayload *bill;

    data = obj->data;
    if (data->flags & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
        owner = data->owner;
        if (owner->kind == EFF_OBJ_OWNER_BILL_KIND) {
            bill = owner->data->handle;
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
    EffectDependencyState *data;

    if (effObjBindValidatedOwner(obj, owner) == 0) {
        return 1;
    }
    data = obj->data;
    data->entryId = entryId;
    data->flags |= EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
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

