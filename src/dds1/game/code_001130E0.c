#include "common.h"

typedef struct {
    u32 handle;
    u32 value4;
    u32 value8;
    u32 valueC;
    u32 value10;
    u32 value14;
    u32 pad18;
    u32 value1C;
    u32 timer;
} BasicObjectData;

typedef struct {
    u8 pad0[0x18];
    BasicObjectData *data;
} BasicObject;

extern u32 D_003BA9D0;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110A48(u64, u64, u64);

u32 func_001130E0(BasicObject *obj) {
    return obj->data->handle;
}

u32 func_001130F0(BasicObject *obj) {
    return obj->data->value8;
}

void func_00113100(BasicObject *obj, u32 value) {
    obj->data->value14 = value;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113110);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001131E0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113338);

void func_00113438(BasicObject *obj, u32 value) {
    BasicObjectData *data;

    data = obj->data;
    dds3SetObjectFlags(obj, 0x2000);
    data->value1C = value;
    data->timer = 0;
}

void func_00113478(BasicObject *obj) {
    BasicObjectData *data;

    data = obj->data;
    data->timer = 0x1e;
    data->value1C = 0;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113490);

void func_00113538(BasicObject *obj) {
    BasicObjectData *data;

    func_00113AA8();
    func_0010F5E8(obj);
    data = obj->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->value8 != 0) {
        func_00222200(data->value8);
        data->value8 = 0;
    }
    func_00111B40(obj);
    func_00111840(data->valueC);
    func_002CFF98(obj->data);
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_001135B0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113888);

void func_00113AA8(BasicObject *obj) {
    BasicObjectData *data;

    data = obj->data;
    if (data->value10 != -1) {
        func_001166F0(obj, 10);
        data->value10 = 0xffffffff;
    }
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113AF0);

u32 func_00113CD8(BasicObject *obj) {
    return obj->data->handle;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113CE8);

void func_00113DA8(void) {
    func_00110928();
}

void func_00113DC0(BasicObject *obj, u32 value) {
    obj->data->valueC = value;
}

void func_00113DD0(BasicObject *obj, u32 value) {
    obj->data->value4 = value;
}

u32 func_00113DE0(u64 id) {
    BasicObject *obj;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    obj = (BasicObject *)func_00110A48(world, id, 6);
    return obj->data->value4;
}

void func_00113E20(BasicObject *obj, u32 value) {
    obj->data->value8 = value;
}

u32 func_00113E30(BasicObject *obj) {
    return obj->data->value8;
}

void func_00113E40(u32 arg0) {
    D_003BA9D0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113E48);

void func_00113EE8(BasicObject *obj) {
    BasicObjectData *data;

    func_0010F5E8();
    data = obj->data;
    func_00111840(data->handle);
    func_002CFF98(data);
}

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F720);

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F730);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113F28);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001141C0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001143B0);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001143D8);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00114508);

INCLUDE_SDATA(const s32, "game/code_001130E0", D_003BA9D0);

