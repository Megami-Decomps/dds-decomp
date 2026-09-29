#include "common.h"

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110C70(u64, u64, u64);

extern u32 D_00435DA0;

typedef struct EffectObjectData {
    u32 handle;
    u32 word04;
    u32 word08;
    u32 word0C;
    s32 activeId;
    u32 word14;
    u32 word18;
    u32 pendingValue;
    u32 timer;
} EffectObjectData;

typedef struct EffectObject {
    u8 pad00[0x18];
    EffectObjectData *data;
} EffectObject;

u32 func_00113308(EffectObject *object) {
    return object->data->handle;
}

u32 func_00113318(EffectObject *object) {
    return object->data->word08;
}

void func_00113328(EffectObject *object, u32 value) {
    object->data->word14 = value;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113338);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113408);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113560);

void func_00113660(EffectObject *object, u32 value) {
    EffectObjectData *data;

    data = object->data;
    dds3SetObjectFlags(object, 0x2000);
    data->pendingValue = value;
    data->timer = 0;
}

void func_001136A0(EffectObject *object) {
    EffectObjectData *data;

    data = object->data;
    data->timer = 0x1e;
    data->pendingValue = 0;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_001136B8);

void func_00113760(EffectObject *object) {
    EffectObjectData *data;

    func_00113CD0();
    effObjFreeInner(object);
    data = object->data;
    if (data->handle != -1) {
        data->handle = -1;
    }
    if (data->word08 != 0) {
        func_0023CD98(data->word08);
        data->word08 = 0;
    }
    func_00111D68(object);
    func_00111A68(data->word0C);
    func_00328E48(object->data);
}

INCLUDE_ASM(const s32, "game/code_00113308", func_001137D8);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113AB0);

void func_00113CD0(s32 object) {
    EffectObjectData *data;

    data = ((EffectObject *)object)->data;
    if (data->activeId != -1) {
        func_00116958(object, 10);
        data->activeId = 0xffffffff;
    }
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113D18);

u32 func_00113F00(s32 object) {
    return ((EffectObject *)object)->data->handle;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113F10);

void func_00113FD0(void) {
    func_00110B50();
}

void func_00113FE8(s32 object, u32 value) {
    ((EffectObject *)object)->data->word0C = value;
}

void func_00113FF8(s32 object, u32 value) {
    ((EffectObject *)object)->data->word04 = value;
}

u32 func_00114008(u64 id) {
    EffectObject *object;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    object = (EffectObject *)func_00110C70(world, id, 6);
    return object->data->word04;
}

void func_00114048(s32 object, u32 value) {
    ((EffectObject *)object)->data->word08 = value;
}

u32 func_00114058(s32 object) {
    return ((EffectObject *)object)->data->word08;
}

void func_00114068(u32 arg0) {
    D_00435DA0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00114070);

void func_00114110(s32 object) {
    EffectObjectData *data;

    effObjFreeInner();
    data = ((EffectObject *)object)->data;
    func_00111A68(data->handle);
    func_00328E48(data);
}

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128A0);

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128B0);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114150);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114428);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114618);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114640);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114770);

INCLUDE_SDATA(const s32, "game/code_00113308", D_00435DA0);

