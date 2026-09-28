#include "common.h"
#include "dds3obj.h"

extern void *func_00111610(void *arg);
extern void *dds3GetSlot(void *arg0, s32 index);

extern ObjBase *func_00112888(void *obj);

s32 func_00111758(u8 arg);
void *dds3GetExtData(void *obj);
void *dds3SetSlotByKind(void *arg0, ObjData *arg1);
void *dds3ExchangeSlot(void *arg0, void *arg1, s32 index);
void func_001111C8(void *arg0, void *arg1);
void dds3SetSlotValue(void *arg0, void *arg1);
void dds3SetSlotKey(void *arg0, void *arg1);
void dds3ReloadSlotPath(void *arg0);
void func_002177D0(s32 arg0, s32 arg1);
void func_00222200(u32 arg0);
void func_002D7AC8(s32 arg0, s32 arg1, s32 arg2);
void func_002DB308(void *arg);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111840);

void dds3SetObjectFlags(void *obj, s32 flags) {
    ObjBase *base;

    base = func_00112888(obj);
    base->flags = base->flags | flags;
}

void dds3ClearObjectFlags(void *obj, s32 flags) {
    ObjBase *base;

    base = func_00112888(obj);
    base->flags = base->flags & ~flags;
}

u8 dds3TestObjectFlags(void *obj, s32 flags) {
    ObjBase *base;

    base = func_00112888(obj);
    return (base->flags & flags) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001119A0);

void *dds3GetExtData(void *obj) {
    return func_00112888(obj)->extData;
}

void *dds3SetSlotByKind(void *obj, ObjData *data) {
    if (data == NULL) {
        return NULL;
    }
    return dds3ExchangeSlot(obj, data, func_00111758(data->kind));
}

void *dds3ExchangeSlot(void *obj, void *data, s32 index) {
    void *old;

    old = dds3GetSlot(obj, index);
    func_00112888(obj)->slots[index] = data;
    return old;
}

void *dds3GetSlot(void *obj, s32 index) {
    return func_00112888(obj)->slots[index];
}

u32 dds3GetUnk04(void *obj) {
    return func_00112888(obj)->unk4;
}

u32 dds3GetUnk0C(void *obj) {
    return func_00112888(obj)->unkC;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111B40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BD8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E30);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111F40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112008);

s32 dds3InvokeSlot5Handler(void *obj) {
    void *handler;

    handler = dds3GetSlot(obj, 5);
    if (handler == NULL) {
        return 0;
    }
    func_001111C8(handler, obj);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112100);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001122F0);

void func_00112750(void *obj) {
    void *existing;
    void *data;

    existing = dds3GetSlot(obj, 1);
    if (existing == NULL) {
        data = func_00111610(obj);
        dds3SetSlotByKind(obj, data);
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
    dds3ReloadSlotPath(obj);
}

void dds3ReleaseSlot1Data(void *obj) {
    dds3ReleaseSlotPath(dds3GetSlot(obj, 1));
}

void dds3GetSlot1Data(void *obj) {
    dds3GetSlotPath(dds3GetSlot(obj, 1));
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_003BA9C8);

