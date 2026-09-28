#include "common.h"
#include "dds3obj.h"

extern void *func_00111610(void *arg);
extern void *objGetSlot(void *arg0, s32 index);

extern ObjBase *func_00112888(void *obj);

s32 func_00111758(u8 arg);
void *objGetExtData(void *obj);
void *objSetSlotByKind(void *arg0, ObjData *arg1);
void *objExchangeSlot(void *arg0, void *arg1, s32 index);
void func_001111C8(void *arg0, void *arg1);
void func_00111698(void *arg0, void *arg1);
void func_001116A8(void *arg0, void *arg1);
void func_001116B8(void *arg0);
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

void *objGetExtData(void *obj) {
    return func_00112888(obj)->extData;
}

void *objSetSlotByKind(void *obj, ObjData *data) {
    if (data == NULL) {
        return NULL;
    }
    return objExchangeSlot(obj, data, func_00111758(data->kind));
}

void *objExchangeSlot(void *obj, void *data, s32 index) {
    void *old;

    old = objGetSlot(obj, index);
    func_00112888(obj)->slots[index] = data;
    return old;
}

void *objGetSlot(void *obj, s32 index) {
    return func_00112888(obj)->slots[index];
}

u32 objGetUnk04(void *obj) {
    return func_00112888(obj)->unk4;
}

u32 objGetUnk0C(void *obj) {
    return func_00112888(obj)->unkC;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111B40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BD8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E30);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111F40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112008);

s32 objInvokeSlot5Handler(void *obj) {
    void *handler;

    handler = objGetSlot(obj, 5);
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

    existing = objGetSlot(obj, 1);
    if (existing == NULL) {
        data = func_00111610(obj);
        objSetSlotByKind(obj, data);
        return;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127A0);

s32 objInvokeSlot1Handler(void *obj, void *context) {
    void *handler;

    handler = objGetSlot(obj, 1);
    if (handler == NULL) {
        return 0;
    }
    func_00111698(handler, context);
    return 1;
}

void objRunSlot1Handlers(void *obj, void *context) {
    void *handler;

    handler = objGetSlot(obj, 1);
    func_001116A8(handler, context);
    func_001116B8(obj);
}

void objReleaseSlot1Data(void *obj) {
    func_001116F8(objGetSlot(obj, 1));
}

void objGetSlot1Data(void *obj) {
    func_00111730(objGetSlot(obj, 1));
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_003BA9C8);

