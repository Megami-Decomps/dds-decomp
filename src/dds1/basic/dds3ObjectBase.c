#include "common.h"
#include "dds3obj.h"

extern void *dds3SpawnSlotRingObj3(void *arg);
extern void *dds3GetSlot(void *arg0, s32 index);

extern ObjBase *dds3GetObjectOwnedHandle(void *obj);

s32 dds3GetObjectSlotRingOccupancy(u8 arg);
void *dds3GetExtData(void *obj);
void *dds3SetSlotByKind(void *arg0, ObjData *arg1);
void *dds3ExchangeSlot(void *arg0, void *arg1, s32 index);
void dds3EnsureWorldNodeInSlot(void *arg0, void *arg1);
void dds3SetSlotValue(void *arg0, void *arg1);
void dds3SetSlotKey(void *arg0, void *arg1);
void dds3ReplaceObjectResource(void *arg0);
void mdlDestroyContext(s32 arg0);
void evtReleaseUnitTransitionWork(void *arg0);
void sdfReleaseDevSlot(s32 arg0, s32 arg1, s32 arg2);
void sdfDestroyMotion(void *arg);
void func_00111258(void *slot, void *owner);
void dds3RemoveWorldObjectNode(void *node);
void dds3DestroyWorldIndexNode(u32 node);
void sdfReleaseChipBlock(void *block);
void func_00111B40(World *world);

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
void func_00111840(ObjBaseFull *base) {
    void *owner;
    void *slot;
    s32 i;

    owner = base->slots[0];
    slot = dds3GetSlot(owner, 5);
    if (slot != NULL) {
        func_00111258(slot, owner);
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
    func_00111B40(owner);
    if (base->devSlot != 0) {
        sdfReleaseDevSlot(base->devSlot, 1, 1);
    }
    dds3DestroyWorldIndexNode(base->worldIndexNode);
    sdfReleaseChipBlock(base);
}

void dds3SetObjectFlags(void *obj, s32 flags) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(obj);
    base->flags = base->flags | flags;
}

void dds3ClearObjectFlags(void *obj, s32 flags) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(obj);
    base->flags = base->flags & ~flags;
}

u8 dds3TestObjectFlags(void *obj, s32 flags) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(obj);
    return (base->flags & flags) != 0;
}

void func_001119A0(void *obj, void *data) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(obj);
    dds3GetExtData(obj);
    base->extData = data;
}

void *dds3GetExtData(void *obj) {
    return dds3GetObjectOwnedHandle(obj)->extData;
}

void *dds3SetSlotByKind(void *obj, ObjData *data) {
    if (data == NULL) {
        return NULL;
    }
    return dds3ExchangeSlot(obj, data, dds3GetObjectSlotRingOccupancy(data->kind));
}

void *dds3ExchangeSlot(void *obj, void *data, s32 index) {
    void *old;

    old = dds3GetSlot(obj, index);
    dds3GetObjectOwnedHandle(obj)->slots[index] = data;
    return old;
}

void *dds3GetSlot(void *obj, s32 index) {
    return dds3GetObjectOwnedHandle(obj)->slots[index];
}

u32 dds3GetUnk04(void *obj) {
    return dds3GetObjectOwnedHandle(obj)->unk4;
}

u32 dds3GetUnk0C(void *obj) {
    return dds3GetObjectOwnedHandle(obj)->unkC;
}

/* Tear down the model/context behind an object's primary handle and mark it released (state 3). */
void func_00111B40(World *world) {
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

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BD8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E30);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111F40);

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

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112100);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001122F0);

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

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127A0);

s32 dds3InvokeSlot1Handler(void *obj, void *context) {
    void *handler;

    handler = dds3GetSlot(obj, 1);
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

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_003BA9C8);

