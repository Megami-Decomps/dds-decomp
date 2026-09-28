#include "common.h"

typedef struct {
    u8 pad0[0x44];
    u32 value44;
    u8 pad48[0x38];
    u32 handle80;
    u8 pad84[4];
    u32 value88;
} ObjectData;

typedef struct {
    u8 pad0[0x18];
    ObjectData *data;
} Object;

extern u32 func_001117A8(u32);
extern s32 func_002CFEB8(u32);

extern s32 func_00112888(void);

INCLUDE_ASM(const s32, "game/code_00112888", func_00112888);

void func_00112930(u32 arg0, u32 value) {
    ObjectData *data;

    data = (ObjectData *)func_00112888();
    data->value44 = value;
}

u32 func_00112958(Object *obj) {
    ObjectData *data;
    u32 handle;

    effObjInnerCreate();
    data = (ObjectData *)func_002CFEB8(0x90);
    obj->data = data;
    handle = func_001117A8((u32)obj);
    data->handle80 = handle;
    dds3SetObjectFlags(obj, 0x62);
    data->value88 = 0;
    return 1;
}
