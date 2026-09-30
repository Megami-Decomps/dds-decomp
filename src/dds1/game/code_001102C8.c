#include "common.h"

extern u32 func_00128780(u32, u32, u32, u32, u32, u32);

extern u32 func_001281E0(u32);

typedef struct {
    u8 pad00[0xC];
    u32 value0C; /* 0x0C */
    u32 value10; /* 0x10 */
    u32 handle14; /* 0x14: stored result from either resource call below */
    u8 pad18[8];
    s32 value20; /* 0x20 */
} WorldObjectData;

typedef struct {
    u8 pad00[4];
    s16 index;   /* 0x04 */
    u16 value06; /* 0x06 */
    u8 pad08[0x10];
    WorldObjectData *data; /* 0x18 */
} WorldObject;

extern WorldObject *D_003BA9BC;


INCLUDE_ASM(const s32, "game/code_001102C8", func_001102C8);

u16 func_00110400(s32 object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = ((WorldObject *)object)->value06;
    }
    return value;
}

u32 func_00110418(WorldObject *object, u32 value) {
    if (object->value06 == 0) {
        return 0;
    }
    if (object->index < 0) {
        return 0;
    }
    ((u32 *)(D_003BA9BC->data->handle14))[object->index * 2] = value;
    return 1;
}

u32 func_00110458(WorldObject *object) {
    if (object->value06 == 0) {
        return 0;
    }
    if (object->index < 0) {
        return 0;
    }
    return ((u32 *)(D_003BA9BC->data->handle14))[object->index * 2];
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
u32 func_00110490(s16 *values) {
    values[2] = *values;
    return (u32)~(s32)*values >> 0x1f;
}

u32 func_001104B0(s16 *values) {
    if (values[2] < 0) {
        return 0;
    }
    values[2] = *(u16 *)((u8 *)D_003BA9BC->data->handle14 + values[2] * 8 + 4);
    return (u32)~(s32)values[2] >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_001104F8);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110578);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110638);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110710);

INCLUDE_ASM(const s32, "game/code_001102C8", func_001107C8);

void func_00110860(WorldObject *object, s8 value) {
    if (object->data != NULL) {
        object->data->value20 = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110880);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110928);

void func_001109B8(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    func_001109F0();
    data->value0C = value;
}

u32 func_001109F0(WorldObject *object) {
    return object->data->value0C;
}

void func_00110A00(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    func_00110A38();
    data->value10 = value;
}

u32 func_00110A38(WorldObject *object) {
    return object->data->value10;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110A48);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110AB0);

void func_00110B48(WorldObject *object, u32 resourceId) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_001281E0(resourceId);
    data->handle14 = handle;
}

void func_00110B78(WorldObject *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_00128780(arg1, arg2, arg3, arg4, arg5, arg6);
    data->handle14 = handle;
}
