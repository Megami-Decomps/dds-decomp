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

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

typedef struct ActionSub {
    u8 unk0[0x10];
    s32 unk10;
} ActionSub;

extern ActionObj *func_00110880();

ActionObj *func_00116A90(s32 a, s32 b, s32 c, s32 d) {
    ActionObj *obj = func_00110880(0xB);

    obj->unk8 = d;
    ((ActionSub *)obj->unk18)->unk10 = 0;
    obj->unk4 = a;
    return obj;
}

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

ActionObj *func_00116B28(s32 a, void *work, s32 c) {
    ActionObj *obj = func_00110880(0xD);

    obj->unk18 = work;
    obj->unk4 = a;
    obj->unk8 = c;
    return obj;
}

u32 func_00116B78(PathObject *path) {
    return (u32)path->state;
}

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116B80);

ActionObj *func_00116CE8(s32 a, void *work, s32 c) {
    ActionObj *obj = func_00110880(0x10);

    obj->unk4 = a;
    obj->unk8 = c;
    obj->unk18 = work;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_00116A90", func_00116D38);
