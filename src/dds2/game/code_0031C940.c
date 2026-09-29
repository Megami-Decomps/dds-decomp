#include "common.h"

typedef struct ModelInstance {
    f32 vector0[3];
    u32 valueC;
    f32 vector10[3];
    u8 pad1C[4];
    u32 flags;
    u16 firstValue;
    u16 secondValue;
    u16 elapsed;
    u16 duration;
    u8 pad2C[8];
} ModelInstance;

void func_0031D508(ModelInstance *item);

typedef struct ModelInstanceList {
    ModelInstance *items;
    s32 count;
} ModelInstanceList;

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031C940);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CA10);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CAE8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CBC8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CDE8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CE60);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CF68);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CF88);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D120);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D260);

void func_0031D380(ModelInstanceList *list) {
    ModelInstance *item;
    u32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            memset(item, 0, 0x34);
            index = (index + 1) & 0xffff;
            item = item + 1;
        } while ((s32)index < list->count);
    }
}

void func_0031D3F0(ModelInstanceList *list, u32 value) {
    ModelInstance *item;
    s32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            index = index + 1;
            item->flags = (item->flags & 0xfffff807) | ((value & 0xff) << 3);
            item = item + 1;
        } while (index < list->count);
    }
}

void func_0031D440(ModelInstanceList *list) {
    ModelInstance *item = list->items;
    s32 index = 0;
    if (list->count > 0) {
        do {
            func_0031D508(item);
            item += 1;
            index++;
        } while (index < list->count);
    }
}

s32 func_0031D4A8(ModelInstanceList *list) {
    ModelInstance *item;
    s32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            if ((item->flags & 1) == 0) {
                item->duration = 10;
                item->flags = item->flags | 1;
                item->elapsed = 0;
                item->firstValue = 0;
                item->secondValue = 0;
                return (s32)item;
            }
            index = index + 1;
            item = item + 1;
        } while (index < list->count);
    }
    return 0;
}

void func_0031D508(ModelInstance *item) {
    item->flags = item->flags & 0xfffffffe;
}

void func_0031D520(ModelInstance *model, s32 value) {
    value &= 0xFFFF;
    model->firstValue = value;
    model->secondValue = value;
}

void func_0031D530(ModelInstance *model, f32 x, f32 y, f32 z) {
    model->vector10[0] = x;
    model->vector10[1] = y;
    model->vector10[2] = z;
}

void func_0031D540(ModelInstance *model, f32 x, f32 y, f32 z) {
    model->vector0[0] = x;
    model->vector0[1] = y;
    model->vector0[2] = z;
    model->valueC = 0;
}

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D558);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D680);
