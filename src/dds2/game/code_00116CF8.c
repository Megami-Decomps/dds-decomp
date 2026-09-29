#include "common.h"

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110C70(u64, u64, u64);

typedef struct {
    u8 pad00[0x10];
    u32 value;
} PathState;

typedef struct {
    u8 pad00[0x18];
    PathState *state;
} PathObject;

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116CF8);

void func_00116D40(PathObject *path, u32 value) {
    path->state->value = value;
}

u32 func_00116D50(u64 id) {
    PathObject *path;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    path = (PathObject *)func_00110C70(world, id, 6);
    return path->state->value;
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116D90);

u32 func_00116DE0(s32 path) {
    return (u32)((PathObject *)path)->state;
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116DE8);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116F50);

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116FA0);
