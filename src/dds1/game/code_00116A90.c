#include "common.h"

typedef struct {
    u8 pad0[0x10];
    u32 value;
} PathState;

typedef struct {
    u8 pad0[0x18];
    PathState *state;
} PathObject;

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 func_00110A48(u64, u64, u64);

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116A90);

void func_00116AD8(PathObject *path, u32 value) {
    path->state->value = value;
}

u32 func_00116AE8(u64 id) {
    PathObject *path;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    path = (PathObject *)func_00110A48(world, id, 6);
    return path->state->value;
}

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116B28);

u32 func_00116B78(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116B80);

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116CE8);

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116D38);
