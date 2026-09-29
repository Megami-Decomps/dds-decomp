#include "common.h"

extern u32 func_00128780(u32, u32, u32, u32, u32, u32);

extern u32 func_001281E0(u32);

typedef struct {
    u8 pad00[0xC];
    u32 value0C; /* 0x0C */
    u32 value10; /* 0x10 */
    u32 value14; /* 0x14 */
    u8 pad18[8];
    s32 value20; /* 0x20 */
} WorldObjectData;

typedef struct {
    u8 pad00[6];
    u16 value06; /* 0x06 */
    u8 pad08[0x10];
    WorldObjectData *data; /* 0x18 */
} WorldObject;


INCLUDE_ASM(const s32, "game/code_001102C8", func_001102C8);

u16 func_00110400(s32 object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = ((WorldObject *)object)->value06;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110418);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110458);

u32 func_00110490(s16 *values) {
    values[2] = *values;
    return (u32)~(s32)*values >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_001104B0);

INCLUDE_ASM(const s32, "game/code_001102C8", func_001104F8);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110578);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110638);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110710);

INCLUDE_ASM(const s32, "game/code_001102C8", func_001107C8);

void func_00110860(s32 object, s8 value) {
    if (((WorldObject *)object)->data != NULL) {
        ((WorldObject *)object)->data->value20 = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110880);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110928);

void func_001109B8(s32 object, u32 value) {
    WorldObjectData *objectData;

    objectData = ((WorldObject *)object)->data;
    func_001109F0();
    objectData->value0C = value;
}

u32 func_001109F0(s32 object) {
    return ((WorldObject *)object)->data->value0C;
}

void func_00110A00(s32 object, u32 value) {
    WorldObjectData *objectData;

    objectData = ((WorldObject *)object)->data;
    func_00110A38();
    objectData->value10 = value;
}

u32 func_00110A38(s32 object) {
    return ((WorldObject *)object)->data->value10;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110A48);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110AB0);

void func_00110B48(s32 object, u32 input) {
    WorldObjectData *objectData;
    u32 result;

    objectData = ((WorldObject *)object)->data;
    result = func_001281E0(input);
    objectData->value14 = result;
}

void func_00110B78(s32 object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    WorldObjectData *objectData;
    u32 result;

    objectData = ((WorldObject *)object)->data;
    result = func_00128780(arg1, arg2, arg3, arg4, arg5, arg6);
    objectData->value14 = result;
}
