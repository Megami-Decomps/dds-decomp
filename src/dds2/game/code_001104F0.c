#include "common.h"

extern u32 func_0012A6F0(u32);

extern u32 func_0012AC90(u32, u32, u32, u32, u32, u32);

typedef struct WorldObjectState {
    u8 pad00[0xC];
    u32 valueC;
    u32 value10;
    u32 handle14;
    u8 pad18[8];
    s32 value20;
} WorldObjectState;

typedef struct WorldObjectStateOwner {
    u8 pad00[0x18];
    WorldObjectState *state;
} WorldObjectStateOwner;

INCLUDE_ASM(const s32, "game/code_001104F0", func_001104F0);

u16 func_00110628(s32 arg0) {
    u16 temp_v0;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v0 = *(u16 *)((s32)arg0 + 6);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110640);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110680);

u32 func_001106B8(s16 *arg0) {
    arg0[2] = *arg0;
    return (u32)~(s32)*arg0 >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_001106D8);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110720);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001107A0);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110860);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110938);

INCLUDE_ASM(const s32, "game/code_001104F0", func_001109F0);

void func_00110A88(WorldObjectStateOwner *object, s8 value) {
    if (object->state != NULL) {
        object->state->value20 = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110AA8);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110B50);

void func_00110BE0(WorldObjectStateOwner *object, u32 value) {
    WorldObjectState *state;

    state = object->state;
    func_00110C18();
    state->valueC = value;
}

u32 func_00110C18(WorldObjectStateOwner *object) {
    return object->state->valueC;
}

void func_00110C28(WorldObjectStateOwner *object, u32 value) {
    WorldObjectState *state;

    state = object->state;
    func_00110C60();
    state->value10 = value;
}

u32 func_00110C60(WorldObjectStateOwner *object) {
    return object->state->value10;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110C70);

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110CD8);

void func_00110D70(WorldObjectStateOwner *object, u32 resourceId) {
    WorldObjectState *state;
    u32 handle;

    state = object->state;
    handle = func_0012A6F0(resourceId);
    state->handle14 = handle;
}

void func_00110DA0(WorldObjectStateOwner *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    WorldObjectState *state;
    u32 handle;

    state = object->state;
    handle = func_0012AC90(arg1, arg2, arg3, arg4, arg5, arg6);
    state->handle14 = handle;
}
