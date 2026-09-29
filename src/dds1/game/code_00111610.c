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

typedef struct {
    u8 unk0[0x10];
} SlotEntry;

typedef struct {
    u8 unk0[4];        /* 0x0 */
    u32 unk4;          /* 0x4 */
    SlotEntry *entry;     /* 0x8: slot from D_00329A68 */
    u8 unkC[0xC];      /* 0xC */
    SlotData *data;    /* 0x18 */
} SlotObjectFull;

extern SlotObjectFull *func_00110880();
extern u32 func_0010FA80();
extern SlotEntry D_00329A68[];
extern s32 D_003BA9C0;

SlotObjectFull *func_00111610(u32 arg0) {
    SlotObjectFull *obj = func_00110880(3);
    SlotData *data = obj->data;
    u32 hash = func_0010FA80();
    s32 slot;

    data->unk0 = arg0;
    slot = D_003BA9C0;
    obj->unk4 = hash;
    obj->entry = &D_00329A68[slot];
    D_003BA9C0 = slot + 1;
    D_003BA9C0 = D_003BA9C0 % 10;
    return obj;
}

void dds3SetSlotValue(SlotObject *obj, u32 value) {
    obj->data->value = value;
}

void dds3SetSlotKey(SlotObject *obj, u32 key) {
    obj->data->key = key;
}

void dds3ReloadSlotPath(SlotObject *obj) {
    SlotData *data;
    u32 path;

    data = obj->data;
    if (data->path != 0) {
        func_00116F08(data->path);
    }
    path = func_00116D38(data->key);
    data->path = path;
}

void dds3ReleaseSlotPath(SlotObject *obj) {
    SlotData *data;
    s32 path;

    data = obj->data;
    path = data->path;
    if (path != 0) {
        func_00116F08(path);
        data->path = 0;
    }
}

u32 dds3GetSlotPath(SlotObject *obj) {
    return obj->data->path;
}

void func_00111740(void) {
    func_00111478();
}

INCLUDE_ASM(const s32, "game/code_00111610", func_00111758);

INCLUDE_ASM(const s32, "game/code_00111610", func_001117A8);

INCLUDE_SDATA(const s32, "game/code_00111610", D_003BA9C0);

