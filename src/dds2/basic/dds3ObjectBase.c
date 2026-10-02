#include "common.h"

#include "dds3obj.h"
#include "dds3Admin.h"
#include "pcp_vu0.h"

extern void *dds3GetSlot(void *arg0, s32 index);

void dds3EnsureWorldNodeInSlot(void *arg0, void *arg1);

void dds3SetSlotValue(void *arg0, void *arg1);

s32 dds3GetObjectSlotRingOccupancy(u8 arg);

void *dds3SetSlotByKind(ObjBase *object, ObjData *data);

void *dds3ExchangeSlot(void *arg0, void *arg1, s32 index);

extern void *dds3SpawnSlotRingObj3(void *arg);

void dds3SetSlotKey(void *arg0, void *arg1);

void dds3ReplaceObjectResource(void *arg0);

extern AdminWork *dds3GetObjectOwnedHandle();

void mdlDestroyContext(s32 arg0);
void evtReleaseUnitTransitionWork(void *arg0);
void sdfReleaseDevSlot(s32 arg0, s32 arg1, s32 arg2);
void sdfDestroyMotion(void *arg);
void func_00111480(void *slot, void *owner);
void dds3RemoveWorldObjectNode(void *node);
void dds3DestroyWorldIndexNode(u32 node);
void sdfReleaseChipBlock(void *block);
void dds3ReleaseObjectBaseResources(World *world);

/* ObjBase plus the runtime fields past 0x38. */
typedef struct ObjBaseFull {
    u32 flags;
    u32 worldIndexNode;
    u32 resourceState;
    u32 resourceHandle;
    void *slots[8];
    void *extData;
    s32 devSlot; /* 0x34: released by sdfReleaseDevSlot */
    void *motion; /* 0x38: released by sdfDestroyMotion */
    s32 mode;    /* 0x3C */
    f32 weight;  /* 0x40 */
    u32 unk44;
} ObjBaseFull;

/* Free an object base: owner, the slot nodes, its devices and finally the block itself. */
void dds3DestroyObjectBase(ObjBaseFull *base) {
    void *owner;
    void *slot;
    s32 i;

    owner = base->slots[0];
    slot = dds3GetSlot(owner, 5);
    if (slot != NULL) {
        func_00111480(slot, owner);
    }
    for (i = 0; i < 8; i++) {
        if (i < 3) {
            if (i > 0) {
                if (base->slots[i] != NULL) {
                    dds3RemoveWorldObjectNode(base->slots[i]);
                }
            }
        }
    }
    dds3ReleaseObjectBaseResources(owner);
    if (base->devSlot != 0) {
        sdfReleaseDevSlot(base->devSlot, 1, 1);
    }
    dds3DestroyWorldIndexNode(base->worldIndexNode);
    sdfReleaseChipBlock(base);
}

void dds3SetObjectFlags(u32 unused, u32 flags) {
    ObjBase *base;

    base = (ObjBase *)dds3GetObjectOwnedHandle();
    base->flags = base->flags | flags;
}

void dds3ClearObjectFlags(u32 unused, u32 flags) {
    ObjBase *base;

    base = (ObjBase *)dds3GetObjectOwnedHandle();
    base->flags = base->flags & ~flags;
}

u8 dds3TestObjectFlags(u32 unused, u32 flags) {
    ObjBase *base;

    base = (ObjBase *)dds3GetObjectOwnedHandle();
    return (base->flags & flags) != 0;
}

void dds3SetExtData(void *obj, void *data) {
    ObjBase *base;

    base = (ObjBase *)dds3GetObjectOwnedHandle(obj);
    dds3GetExtData(obj);
    base->extData = data;
}

void *dds3GetExtData(void) {
    return ((ObjBase *)dds3GetObjectOwnedHandle())->extData;
}

void *dds3SetSlotByKind(ObjBase *object, ObjData *data) {
    if (data == NULL) {
        return NULL;
    }
    return dds3ExchangeSlot(object, data, dds3GetObjectSlotRingOccupancy(data->kind));
}

void *dds3ExchangeSlot(void *obj, void *data, s32 index) {
    void *old;

    old = dds3GetSlot(obj, index);
    ((ObjBase *)dds3GetObjectOwnedHandle(obj))->slots[index] = data;
    return old;
}

void *dds3GetSlot(void *obj, s32 index) {
    return ((ObjBase *)dds3GetObjectOwnedHandle(obj))->slots[index];
}

u32 dds3GetUnk04(void) {
    ObjBase *base;

    base = (ObjBase *)dds3GetObjectOwnedHandle();
    return base->unk4;
}

u32 dds3GetUnk0C(void) {
    return ((ObjBase *)dds3GetObjectOwnedHandle())->unkC;
}

/* Tear down the model/context behind an object's primary handle and mark it released (state 3). */
void dds3ReleaseObjectBaseResources(World *world) {
    ObjBaseFull *base;
    WorldInfo *info;

    base = (ObjBaseFull *)dds3GetObjectOwnedHandle(world);
    if (base->resourceHandle != 0) {
        if (base->resourceState != 1) {
            if (base->resourceState == 0) {
                mdlDestroyContext(base->resourceHandle);
                info = world->info;
                if (info->primaryObject != NULL) {
                    evtReleaseUnitTransitionWork(info->primaryObject);
                    info->primaryObject = NULL;
                }
            }
        } else {
            sdfReleaseDevSlot(base->resourceHandle, 1, 1);
            sdfDestroyMotion(base->motion);
        }
        base->resourceHandle = 0;
        base->resourceState = 3;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

extern u32 evtCreateModelFromObject(void *object);
extern s32 evtAttachScriptToObject(void *object, void *model);
extern void dds3LoadOrBuildObjectMatrix(u8 *object);
extern void sdfModelUpdateRootTransforms(void *model, s32 frame);

void func_00112168(void *object) {
    ObjBaseFull *base;
    void *slot;
    void *motion;
    void *model;

    base = (ObjBaseFull *)dds3GetObjectOwnedHandle(object);
    slot = dds3GetSlot(object, 3);
    model = (void *)evtCreateModelFromObject(slot);
    motion = (void *)evtAttachScriptToObject(slot, model);
    dds3LoadOrBuildObjectMatrix((u8 *)object);
    VU0_STORE_MATRIX((u8 *)model + 0x20);
    sdfModelUpdateRootTransforms(model, 0);
    sdfModelUpdateRootTransforms(model, 1);
    base->resourceState = 1;
    base->resourceHandle = (u32)model;
    base->motion = motion;
    base->weight = 1.0f;
    base->mode = 0;
    base->unk44 = 0;
}

/* Mode word and blend weight at the end of ObjBase (0x3C / 0x40). */
typedef struct ObjMode {
    u8 pad00[0x3C];
    s32 mode;    /* 0x3C */
    f32 weight;  /* 0x40 */
} ObjMode;

/* Select the object's mode 0..6; modes 0, 4 and 5 use full weight, the others zero. */
void dds3SetObjectModeAndDefaultWeight(void *obj, u32 mode) {
    ObjMode *base = (ObjMode *)dds3GetObjectOwnedHandle(obj);

    switch (mode) {
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

s32 dds3InvokeSlot5Handler(void *object) {
    void *handler;

    handler = dds3GetSlot(object, 5);
    if (handler == NULL) {
        return 0;
    }
    dds3EnsureWorldNodeInSlot(handler, object);
    return 1;
}

extern void evtSetDrawSurfaceIndex(u32 surfaceIndex);
extern void evtSubmitGsRegister47(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00108BD8(s32 arg);
extern void func_00108D80(void);
extern void func_00108E20(void);
extern void func_00109248();
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

    base = (ObjBase *)dds3GetObjectOwnedHandle(object);
    evtSetDrawSurfaceIndex(0x4A);
    func_00108BD8(0);
    func_00108D80();
    evtSubmitGsRegister47(1, 1, 0x80, 2, 0, 0, 1, 1);
    func_00109248(0, 0, 0x200, 0x1C0, 0x0FFFFFFF, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
    evtSubmitGsRegister47(1, 1, 0x80, 2, 0, 0, 1, 1);
    evtSetDrawSurfaceIndex(0x4B);
    evtSubmitGsRegister47(1, 1, 0x80, 2, 0, 0, 1, 1);
    func_00108D80();
    evtSetDrawSurfaceIndex(0x4C);
    evtSubmitGsRegister47(1, 1, 0x80, 2, 0, 0, 1, 1);
    func_00108D80();

    context = (ObjRenderContext *)base->unkC;
    inner = context->inner;
    inner->flags19 |= 0x20;
    mdlProcessContextNodesAndTransforms(context, D_00380818);
    inner->flags19 &= ~0x20;
    dds3SetObjectFlags(object, 0x10000);

    evtSetDrawSurfaceIndex(0x4E);
    func_00108E20();
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 1, 0, 0, 1, 2);
    func_00109248(0, 0, 0x200, 0x1C0, 0x0EFFFFFF, 0x30586670, 0x30586670, 0x30586670, 0x30586670);
    evtSubmitGsRegister47(1, 5, 0x80, 1, 0, 0, 1, 2);
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112518);

void dds3EnsureSlotData(void *object) {
    void *existing;
    void *data;

    existing = dds3GetSlot(object, 1);
    if (existing == NULL) {
        data = dds3SpawnSlotRingObj3(object);
        dds3SetSlotByKind(object, data);
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

void dds3RunSlot1Handlers(void *obj, void *context) {
    void *handler;

    handler = dds3GetSlot(obj, 1);
    dds3SetSlotKey(handler, context);
    dds3ReplaceObjectResource(obj);
}

void dds3ReleaseSlot1Data(void *obj) {
    dds3ReleaseObjectResource(dds3GetSlot(obj, 1));
}

void dds3GetSlot1Data(void *obj) {
    dds3GetObjectResourceHandle(dds3GetSlot(obj, 1));
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_00435D98);
