#include "common.h"

extern s32 func_0031CF88(f32, f32);

/* Records are 0x34 bytes; bit 0 of flags marks a claimed slot. */
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

void itfDeactivateModelInstance(ModelInstance *item);

typedef struct ModelInstanceList {
    ModelInstance *items;
    s32 count;
} ModelInstanceList;

typedef struct ModelInstanceWork {
    u32 handle;
    s32 count;
    ModelInstanceList *lists;
    u32 unkC;
} ModelInstanceWork;

extern u32 func_003292A8(s32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031C940);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CA10);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CAE8);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CBC8);

extern u32 D_0045C840[];
void func_0031CDE8(u32 *source, s32 count) {
    s32 i;

    memset(D_0045C840, 0, 0x1C);
    for (i = 0; i < count; i++) {
        D_0045C840[i] = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CE60);

INCLUDE_ASM(const s32, "game/code_0031C940", itfDrawUniformlyScaledIndexedImage);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031CF88);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D120);

ModelInstanceWork *func_0031D260(s32 count, s32 *counts) {
    s32 listBytes = count * 8;
    s32 size = listBytes + 16;
    s32 i;
    u32 handle;
    ModelInstanceWork *work;
    ModelInstanceList *list;
    u8 *records;

    for (i = 0; i < count; i++) size += counts[i] * sizeof(ModelInstance);
    evtPrintDeveloperConsoleMessage("SpriteWork Object Size %d\n", size);
    handle = func_003292A8(size);
    work = sdfMemoryGetBlockAddress(handle);
    memset(work, 0, size);
    work->handle = handle;
    work->count = count;
    work->lists = (ModelInstanceList *)(work + 1);
    list = work->lists;
    records = (u8 *)list + listBytes;
    for (i = 0; i < count; i++, list++) {
        list->items = (ModelInstance *)records;
        list->count = counts[i];
        records += counts[i] * sizeof(ModelInstance);
    }
    return work;
}

/* Clear all slots; the loop index intentionally wraps to 16 bits. */
void itfClearModelInstances(ModelInstanceList *list) {
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

/* Store an eight-bit parameter in flags bits 3..10, preserving other flags. */
void itfSetModelInstanceParameterBits(ModelInstanceList *list, u32 parameter) {
    ModelInstance *item;
    s32 index;

    index = 0;
    item = list->items;
    if (0 < list->count) {
        do {
            index = index + 1;
            item->flags = (item->flags & 0xfffff807) | ((parameter & 0xff) << 3);
            item = item + 1;
        } while (index < list->count);
    }
}

void itfDeactivateModelInstances(ModelInstanceList *list) {
    ModelInstance *item = list->items;
    s32 index = 0;
    if (list->count > 0) {
        do {
            itfDeactivateModelInstance(item);
            item += 1;
            index++;
        } while (index < list->count);
    }
}

/* Return the first unclaimed record after resetting its time and value pair. */
s32 itfClaimFreeModelInstance(ModelInstanceList *list) {
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

void itfDeactivateModelInstance(ModelInstance *item) {
    item->flags = item->flags & 0xfffffffe;
}

void itfSetModelInstanceValuePair(ModelInstance *model, s32 value) {
    value &= 0xFFFF;
    model->firstValue = value;
    model->secondValue = value;
}

/* Write the second three-component vector without changing its other state. */
void itfSetModelInstanceSecondaryVector(ModelInstance *model, f32 x, f32 y, f32 z) {
    model->vector10[0] = x;
    model->vector10[1] = y;
    model->vector10[2] = z;
}

/* Write the first vector and reset its associated word at +0x0C. */
void itfSetModelInstancePrimaryVector(ModelInstance *model, f32 x, f32 y, f32 z) {
    model->vector0[0] = x;
    model->vector0[1] = y;
    model->vector0[2] = z;
    model->valueC = 0;
}

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D558);

INCLUDE_ASM(const s32, "game/code_0031C940", func_0031D680);
