#include "common.h"

typedef struct {
    u32 unk0;
    u32 path;
    u32 value;
    u32 key;
} SlotData;

typedef struct {
    u8 pad0[0x18];
    SlotData *data;
} SlotObject;

extern u32 func_00116D38(u32);

INCLUDE_ASM(const s32, "game/code_00111610", func_00111610);

void func_00111698(SlotObject *obj, u32 value) {
    obj->data->value = value;
}

void func_001116A8(SlotObject *obj, u32 key) {
    obj->data->key = key;
}

void func_001116B8(SlotObject *obj) {
    SlotData *data;
    u32 path;

    data = obj->data;
    if (data->path != 0) {
        func_00116F08(data->path);
    }
    path = func_00116D38(data->key);
    data->path = path;
}

void func_001116F8(SlotObject *obj) {
    SlotData *data;
    s32 path;

    data = obj->data;
    path = data->path;
    if (path != 0) {
        func_00116F08(path);
        data->path = 0;
    }
}

u32 func_00111730(SlotObject *obj) {
    return obj->data->path;
}

void func_00111740(void) {
    func_00111478();
}

INCLUDE_ASM(const s32, "game/code_00111610", func_00111758);

INCLUDE_ASM(const s32, "game/code_00111610", func_001117A8);

INCLUDE_SDATA(const s32, "game/code_00111610", D_003BA9C0);

