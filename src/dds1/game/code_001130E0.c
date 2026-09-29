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

f32 func_00113110(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
}

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
    effObjFreeInner(obj);
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

typedef struct WorldSubState {
    u8 unk0[0x10];   /* 0x0 */
    f32 vec[4];      /* 0x10 */
} WorldSubState;

typedef struct WorldObj {
    u8 unk0[4];           /* 0x0 */
    s32 unk4;             /* 0x4 */
    u8 unk8[0x10];        /* 0x8 */
    WorldSubState *state; /* 0x18 */
    s32 unk1C;            /* 0x1C */
} WorldObj;

extern WorldObj *func_00110880();
extern void func_00112750();
extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();

WorldObj *func_00113CE8(s32 a, f32 *vec, void *second) {
    f32 zero[4];
    WorldObj *obj;

    memset(zero, 0, 0x10);
    zero[3] = 1.0f;
    obj = func_00110880(6);
    obj->unk4 = a;
    func_00112750(obj);
    effObjSetInnerSecondVec(obj, second);
    effObjSetInnerFirstVec(obj, zero);
    effObjInnerVecBackup(obj->unk1C);
    obj->state->vec[0] = vec[0];
    obj->state->vec[1] = vec[1];
    obj->state->vec[2] = vec[2];
    obj->state->vec[3] = vec[3];
    return obj;
}

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

    effObjFreeInner();
    data = obj->data;
    func_00111840(data->handle);
    func_002CFF98(data);
}

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F720);

INCLUDE_RODATA(const s32, "game/code_001130E0", D_0039F730);

INCLUDE_ASM(const s32, "game/code_001130E0", func_00113F28);

INCLUDE_ASM(const s32, "game/code_001130E0", func_001141C0);

void func_001143B0(u8 *obj) {
    f32 *src = *(f32 **)(obj + 0x1C);
    f32 *dst = *(f32 **)(obj + 0x18);
    dst[4] = src[0x10];
    dst[5] = src[0x11];
    dst[6] = src[0x12];
}

INCLUDE_ASM(const s32, "game/code_001130E0", func_001143D8);

extern void effObjInnerCreate();
extern void *func_002CFEB8(s32 size);
extern u32 func_001117A8();

s32 func_00114508(BasicObject *obj) {
    BasicObjectData *data;

    effObjInnerCreate(obj);
    obj->data = func_002CFEB8(0x50);
    memset(obj->data, 0, 0x50);
    data = obj->data;
    data->handle = func_001117A8(obj);
    dds3SetObjectFlags(obj, 0x60);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_001130E0", D_003BA9D0);

