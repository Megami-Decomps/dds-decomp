#include "common.h"

#include "dds3obj.h"
#include "dds3Admin.h"

extern void *dds3GetSlot(void *arg0, s32 index);

void func_001113F0(void *arg0, void *arg1);

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
void func_00110B50(void *node);
void dds3DestroyWorldIndexNode(u32 node);
void sdfReleaseChipBlock(void *block);
void func_00111D68(World *world);

/* ObjBase plus the runtime fields past 0x38. */
typedef struct ObjBaseFull {
    u32 flags;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    void *slots[8];
    void *extData;
    s32 unk34;
    void *unk38;
    s32 mode;    /* 0x3C */
    f32 weight;  /* 0x40 */
    u32 unk44;
} ObjBaseFull;

/* Free an object base: owner, the slot nodes, its devices and finally the block itself. */
void func_00111A68(ObjBaseFull *base) {
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
                    func_00110B50(base->slots[i]);
                }
            }
        }
    }
    func_00111D68(owner);
    if (base->unk34 != 0) {
        sdfReleaseDevSlot(base->unk34, 1, 1);
    }
    dds3DestroyWorldIndexNode(base->unk4);
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

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BC8);

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
void func_00111D68(World *world) {
    ObjBaseFull *base;
    WorldInfo *info;

    base = (ObjBaseFull *)dds3GetObjectOwnedHandle(world);
    if (base->unkC != 0) {
        if (base->unk8 != 1) {
            if (base->unk8 == 0) {
                mdlDestroyContext(base->unkC);
                info = world->info;
                if (info->primaryObject != NULL) {
                    evtReleaseUnitTransitionWork(info->primaryObject);
                    info->primaryObject = NULL;
                }
            }
        } else {
            sdfReleaseDevSlot(base->unkC, 1, 1);
            sdfDestroyMotion(base->unk38);
        }
        base->unkC = 0;
        base->unk8 = 3;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112168);

/* Mode word and blend weight at the end of ObjBase (0x3C / 0x40). */
typedef struct ObjMode {
    u8 pad00[0x3C];
    s32 mode;    /* 0x3C */
    f32 weight;  /* 0x40 */
} ObjMode;

/* Select the object's mode 0..6; modes 0, 4 and 5 use full weight, the others zero. */
void func_00112230(void *obj, u32 mode) {
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
    func_001113F0(handler, object);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112328);

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

s64 func_001129C8(void) {
    return dds3InvokeSlot1Handler();
}

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

