#include "common.h"

#include "dds3obj.h"

extern void *dds3GetSlot(void *arg0, s32 index);

void func_001113F0(void *arg0, void *arg1);

void dds3SetSlotValue(void *arg0, void *arg1);

extern s32 func_00112AB0(void);

s32 func_00111980(u8 arg);

void *dds3SetSlotByKind(ObjBase *object, ObjData *data);

void *dds3ExchangeSlot(void *arg0, void *arg1, s32 index);

extern void *func_00111838(void *arg);

void dds3SetSlotKey(void *arg0, void *arg1);

void dds3ReplaceObjectResource(void *arg0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111A68);

void dds3SetObjectFlags(u32 unused, u32 flags) {
    ObjBase *base;

    base = (ObjBase *)func_00112AB0();
    base->flags = base->flags | flags;
}

void dds3ClearObjectFlags(u32 unused, u32 flags) {
    ObjBase *base;

    base = (ObjBase *)func_00112AB0();
    base->flags = base->flags & ~flags;
}

u8 dds3TestObjectFlags(u32 unused, u32 flags) {
    ObjBase *base;

    base = (ObjBase *)func_00112AB0();
    return (base->flags & flags) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BC8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3GetExtData);

void *dds3SetSlotByKind(ObjBase *object, ObjData *data) {
    if (data == NULL) {
        return NULL;
    }
    return dds3ExchangeSlot(object, data, func_00111980(data->kind));
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ExchangeSlot);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3GetSlot);

u32 dds3GetUnk04(void) {
    ObjBase *base;

    base = (ObjBase *)func_00112AB0();
    return base->unk4;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3GetUnk0C);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111D68);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112168);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112230);

s32 dds3InvokeSlot5Handler(void *object) {
    void *slot;

    slot = dds3GetSlot(object, 5);
    if (slot == NULL) {
        return 0;
    }
    func_001113F0(slot, object);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112328);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112518);

void func_00112978(void *obj) {
    void *existing;
    void *data;

    existing = dds3GetSlot(obj, 1);
    if (existing == NULL) {
        data = func_00111838(obj);
        dds3SetSlotByKind(obj, data);
        return;
    }
}

s64 func_001129C8(void) {
    return dds3InvokeSlot1Handler();
}

s32 dds3InvokeSlot1Handler(void *object, void *value) {
    void *slot;

    slot = dds3GetSlot(object, 1);
    if (slot == NULL) {
        return 0;
    }
    dds3SetSlotValue(slot, value);
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
