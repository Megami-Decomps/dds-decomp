#include "common.h"
#include "dds3_path.h"

#include "dds3obj.h"
#include "eff_object.h"
#include "dds3Admin.h"
#include "pcp_vu0.h"
#include "mdl.h"

extern void *dds3GetSlot(void *arg0, s32 index);
struct ObjectWithResource;
extern Dds3PathCurveWork *dds3GetObjectResourceHandle(struct ObjectWithResource *);

void dds3EnsureWorldNodeInSlot(void *arg0, void *arg1);

void dds3SetSlotValue(void *arg0, void *arg1);

s32 dds3SelectSlotForObjectKind(u8 arg);

void *dds3SetSlotByKind(ObjBase *object, ObjData *data);

void *dds3ExchangeSlot(void *arg0, void *arg1, s32 index);

extern void *dds3SpawnSlotRingObj3(void *arg);

void dds3SetSlotKey(void *arg0, void *arg1);

void dds3ReplaceObjectResource(void *arg0);


void mdlDestroyContext(MdlCtx *model);
void evtReleaseUnitTransitionWork(void *arg0);
void sdfReleaseDevSlot(s32 arg0, s32 arg1, s32 arg2);
void sdfDestroyMotion(void *arg);
void func_00111480(void *slot, void *owner);
void dds3RemoveWorldObjectNode(void *node);
void dds3DestroyWorldIndexNode(NodeB *node);
void sdfReleaseChipBlock(void *block);
void dds3ReleaseObjectBaseResources(EffWorldNode *object);


#define DDS3_OBJECT_SLOT_COUNT 8
#define DDS3_OBJECT_WORLD_SLOT_LIMIT 3
#define DDS3_OBJECT_OWNER_SLOT 0
#define DDS3_OBJECT_HANDLER_SLOT 5
#define DDS3_OBJECT_DATA_SLOT 1
#define DDS3_OBJECT_RESOURCE_MODEL_CONTEXT 0
#define DDS3_OBJECT_RESOURCE_DEV_MOTION 1
#define DDS3_OBJECT_RESOURCE_RELEASED 3

/* Process the owner's entry in the auxiliary handler index, destroy world nodes
 * in slots 1/2, release owned resources/devices, then free the index and base. */
void dds3DestroyObjectBase(ObjBase *base) {
    void *owner;
    void *handler;
    s32 slotIndex;

    owner = base->slots[DDS3_OBJECT_OWNER_SLOT];
    handler = dds3GetSlot(owner, DDS3_OBJECT_HANDLER_SLOT);
    if (handler != NULL) {
        func_00111480(handler, owner);
    }
    for (slotIndex = 0; slotIndex < DDS3_OBJECT_SLOT_COUNT; slotIndex++) {
        if (slotIndex < DDS3_OBJECT_WORLD_SLOT_LIMIT) {
            if (slotIndex > DDS3_OBJECT_OWNER_SLOT) {
                if (base->slots[slotIndex] != NULL) {
                    dds3RemoveWorldObjectNode(base->slots[slotIndex]);
                }
            }
        }
    }
    dds3ReleaseObjectBaseResources(owner);
    if (base->devSlot != 0) {
        sdfReleaseDevSlot(base->devSlot, 1, 1);
    }
    dds3DestroyWorldIndexNode((NodeB *)base->worldIndexNode);
    sdfReleaseChipBlock(base);
}

/* Set mask bits in the object's resolved base. */
void dds3SetObjectFlags(void *object, u32 mask) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    base->flags = base->flags | mask;
}

/* Clear mask bits in the object's resolved base. */
void dds3ClearObjectFlags(void *object, u32 mask) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    base->flags = base->flags & ~mask;
}

/* Return whether any requested mask bit is set, not whether all bits are set. */
u8 dds3TestObjectFlags(void *object, u32 mask) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    return (base->flags & mask) != 0;
}

void *dds3GetExtData(void *object);

/* Replace the extension pointer without releasing its previous value.
 * Keep the existing extension getter call before the store. */
void dds3SetExtData(void *object, void *extensionData) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    dds3GetExtData(object);
    base->extData = extensionData;
}

/* Return the stored extension pointer; no copy or ownership change. */
void *dds3GetExtData(void *object) {
    return dds3GetObjectOwnedHandle(object)->extData;
}

/* Exchange the slot selected by the data kind; NULL data leaves every slot alone. */
void *dds3SetSlotByKind(ObjBase *object, ObjData *slotData) {
    if (slotData == NULL) {
        return NULL;
    }
    return dds3ExchangeSlot(object, slotData, dds3SelectSlotForObjectKind(slotData->kind));
}

/* Replace one caller-selected slot and return its previous pointer; no release. */
void *dds3ExchangeSlot(void *object, void *slotData, s32 slotIndex) {
    void *previousData;

    previousData = dds3GetSlot(object, slotIndex);
    dds3GetObjectOwnedHandle(object)->slots[slotIndex] = slotData;
    return previousData;
}

/* Read an indexed slot; the caller supplies a valid index. */
void *dds3GetSlot(void *object, s32 slotIndex) {
    return dds3GetObjectOwnedHandle(object)->slots[slotIndex];
}

/* Return the world-index node word also used when destroying the full base. */
u32 dds3GetObjectIndexNode(void *object) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    return base->worldIndexNode;
}

/* Return the primary resource-handle word, whose interpretation depends on state. */
u32 dds3GetObjectBaseResourceHandle(void *object) {
    return dds3GetObjectOwnedHandle(object)->resourceHandle;
}

/* A nonzero handle releases model/context state (0) or device/motion state (1),
 * then clears the handle and marks state 3. Other states skip backend release;
 * an absent handle leaves state untouched, and the stored motion pointer remains. */
void dds3ReleaseObjectBaseResources(EffWorldNode *object) {
    ObjBase *base;
    EffectObjectData *data;

    base = dds3GetObjectOwnedHandle(object);
    if (base->resourceHandle != 0) {
        if (base->resourceState != DDS3_OBJECT_RESOURCE_DEV_MOTION) {
            if (base->resourceState == DDS3_OBJECT_RESOURCE_MODEL_CONTEXT) {
                mdlDestroyContext((MdlCtx *)base->resourceHandle);
                data = object->data;
                if (data->transitionWork != NULL) {
                    evtReleaseUnitTransitionWork(data->transitionWork);
                    data->transitionWork = NULL;
                }
            }
        } else {
            sdfReleaseDevSlot(base->resourceHandle, 1, 1);
            sdfDestroyMotion(base->motion);
        }
        base->resourceHandle = 0;
        base->resourceState = DDS3_OBJECT_RESOURCE_RELEASED;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

extern SdfModel *evtCreateModelFromObject(void *object);
extern Motion *evtAttachScriptToObject(void *object, SdfModel *model);
extern void dds3LoadOrBuildObjectMatrix(EffWorldNode *object);
extern void sdfModelUpdateRootTransforms(SdfModel *model, s32 frame);

void func_00112168(void *object) {
    ObjBase *base;
    void *slot;
    Motion *motion;
    SdfModel *model;

    base = dds3GetObjectOwnedHandle(object);
    slot = dds3GetSlot(object, 3);
    model = evtCreateModelFromObject(slot);
    motion = evtAttachScriptToObject(slot, model);
    dds3LoadOrBuildObjectMatrix((EffWorldNode *)object);
    VU0_STORE_MATRIX(model->matrix);
    sdfModelUpdateRootTransforms(model, 0);
    sdfModelUpdateRootTransforms(model, 1);
    base->resourceState = 1;
    base->resourceHandle = (u32)model;
    base->motion = motion;
    base->weight = 1.0f;
    base->mode = 0;
    base->unk44 = 0;
}


/* Modes 0, 4 and 5 use weight 1; the other valid modes use 0.
 * Values outside 0..6 leave both the current mode and weight unchanged. */
void dds3SetObjectModeAndDefaultWeight(void *object, u32 requestedMode) {
    ObjBase *base = dds3GetObjectOwnedHandle(object);

    switch (requestedMode) {
    case 0:
        base->mode = 0;
        base->weight = 1.0f;
        break;
    case 1:
        base->mode = 1;
        base->weight = 0.0f;
        break;
    case 2:
        base->mode = 2;
        base->weight = 0.0f;
        break;
    case 3:
        base->mode = 3;
        base->weight = 0.0f;
        break;
    case 4:
        base->mode = 4;
        base->weight = 1.0f;
        break;
    case 5:
        base->mode = 5;
        base->weight = 1.0f;
        break;
    case 6:
        base->mode = 6;
        base->weight = 0.0f;
        break;
    }
}

/* Ensure the owner appears in its auxiliary handler's world index.
 * Return 0 for an absent handler, otherwise 1 after updating the index. */
s32 dds3RegisterObjectInHandlerIndex(void *object) {
    void *handler;

    handler = dds3GetSlot(object, DDS3_OBJECT_HANDLER_SLOT);
    if (handler == NULL) {
        return 0;
    }
    dds3EnsureWorldNodeInSlot(handler, object);
    return 1;
}

extern void evtSetDrawSurfaceIndex(u32 surfaceIndex);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32 arg);
extern void func_00108D80(void);
extern void func_00108E20(void);
extern void evtSubmitGradientRectAtDepth();
extern void mdlProcessContextNodesAndTransforms(void *context, const void *state);
extern s32 D_00380818[4];

typedef struct ObjRenderContextInner {
    u8 pad00[0x19];
    u8 flags19;
} ObjRenderContextInner;

typedef struct ObjRenderContext {
    u8 pad00[0x18];
    ObjRenderContextInner *inner;
} ObjRenderContext;

void func_00112328(void *object) {
    ObjBase *base;
    ObjRenderContext *context;
    ObjRenderContextInner *inner;

    base = dds3GetObjectOwnedHandle(object);
    evtSetDrawSurfaceIndex(0x4A);
    evtSubmitPrimaryAlphaBlendMode(0);
    func_00108D80();
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    evtSubmitGradientRectAtDepth(0, 0, 0x200, 0x1C0, 0x0FFFFFFF, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    evtSetDrawSurfaceIndex(0x4B);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    func_00108D80();
    evtSetDrawSurfaceIndex(0x4C);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    func_00108D80();

    context = (ObjRenderContext *)base->resourceHandle;
    inner = context->inner;
    inner->flags19 |= 0x20;
    mdlProcessContextNodesAndTransforms(context, D_00380818);
    inner->flags19 &= ~0x20;
    dds3SetObjectFlags(object, 0x10000);

    evtSetDrawSurfaceIndex(0x4E);
    func_00108E20();
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 2);
    evtSubmitGradientRectAtDepth(0, 0, 0x200, 0x1C0, 0x0EFFFFFF, 0x30586670, 0x30586670, 0x30586670, 0x30586670);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 1, 0, 0, 1, 2);
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112518);

/* Create and attach kind-3 slot data only when the data slot is empty.
 * Existing data is retained; attachment still uses the new object's kind. */
void dds3EnsureSlotData(void *object) {
    void *existingData;
    void *newData;

    existingData = dds3GetSlot(object, DDS3_OBJECT_DATA_SLOT);
    if (existingData == NULL) {
        newData = dds3SpawnSlotRingObj3(object);
        dds3SetSlotByKind(object, newData);
        return;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001129C8);

s32 dds3InvokeSlot1Handler(void *object, void *context) {
    void *handler;

    handler = dds3GetSlot(object, 1);
    if (handler == NULL) {
        return 0;
    }
    dds3SetSlotValue(handler, context);
    return 1;
}

/* Store the source object on the data-slot handler, then replace the supplied
 * object's curve work. The replacement call uses object, not handler;
 * unlike the conditional dispatcher, this path does not check for NULL. */
void dds3RunSlot1Handlers(void *object, void *sourceObject) {
    void *handler;

    handler = dds3GetSlot(object, DDS3_OBJECT_DATA_SLOT);
    dds3SetSlotKey(handler, sourceObject);
    dds3ReplaceObjectResource(object);
}

/* Release the data slot's curve work and clear its handle, not the slot pointer.
 * The caller must have established the data slot. */
void dds3ReleaseSlot1Data(void *object) {
    dds3ReleaseObjectResource(dds3GetSlot(object, DDS3_OBJECT_DATA_SLOT));
}

/* Return the data slot's retained curve work; the data slot must exist. */
Dds3PathCurveWork *dds3GetSlot1Data(void *object) {
    return dds3GetObjectResourceHandle(dds3GetSlot(object, DDS3_OBJECT_DATA_SLOT));
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_00435D98);

