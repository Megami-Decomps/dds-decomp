#include "common.h"

extern u32 func_0012A6F0(u32);

extern u32 func_0012AC90(u32, u32, u32, u32, u32, u32);

typedef struct WorldObjectData {
    u8 pad00[0xC];
    u32 value0C;
    u32 value10;
    u32 handle14;
    u8 pad18[8];
    s32 value20;
} WorldObjectData;

typedef struct WorldObject {
    u8 pad00[6];
    u16 value06; /* 0x06 */
    u8 pad08[0x10];
    WorldObjectData *data; /* 0x18 */
} WorldObject;

INCLUDE_ASM(const s32, "game/code_001104F0", func_001104F0);

u16 func_00110628(s32 object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = ((WorldObject *)object)->value06;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110640);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110680);

u32 func_001106B8(s16 *values) {
    values[2] = *values;
    return (u32)~(s32)*values >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_001106D8);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110720);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001107A0);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110860);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110938);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001109F0);

void func_00110A88(WorldObject *object, s8 value) {
    if (object->data != NULL) {
        object->data->value20 = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110AA8);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110B50);

void func_00110BE0(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    func_00110C18();
    data->value0C = value;
}

u32 func_00110C18(WorldObject *object) {
    return object->data->value0C;
}

void func_00110C28(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    func_00110C60();
    data->value10 = value;
}

u32 func_00110C60(WorldObject *object) {
    return object->data->value10;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110C70);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110CD8);

void func_00110D70(WorldObject *object, u32 resourceId) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_0012A6F0(resourceId);
    data->handle14 = handle;
}

void func_00110DA0(WorldObject *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_0012AC90(arg1, arg2, arg3, arg4, arg5, arg6);
    data->handle14 = handle;
}
