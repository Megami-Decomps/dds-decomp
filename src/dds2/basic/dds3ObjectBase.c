#include "common.h"

#include "dds3obj.h"
#include "dds3Admin.h"

extern void *dds3GetSlot(void *arg0, s32 index);

void func_001113F0(void *arg0, void *arg1);

void dds3SetSlotValue(void *arg0, void *arg1);

s32 func_00111980(u8 arg);

void *dds3SetSlotByKind(ObjBase *object, ObjData *data);

void *dds3ExchangeSlot(void *arg0, void *arg1, s32 index);

extern void *dds3SpawnSlotRingObj3(void *arg);

void dds3SetSlotKey(void *arg0, void *arg1);

void dds3ReplaceObjectResource(void *arg0);

extern AdminWork *dds3GetObjectOwnedHandle();

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111A68);

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
    return dds3ExchangeSlot(object, data, func_00111980(data->kind));
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

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111D68);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112168);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112230);

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

