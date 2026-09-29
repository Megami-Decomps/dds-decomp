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

extern ActionObj *func_00110AA8();

ActionObj *evtSpawnActionObjB(s32 a, s32 b, s32 c, s32 d) {
    ActionObj *obj = func_00110AA8(0xB);

    obj->unk8 = d;
    ((ActionSub *)obj->unk18)->unk10 = 0;
    obj->unk4 = a;
    return obj;
}

void dds3SetPathStateValue(PathObject *path, u32 value) {
    path->state->value = value;
}

u32 dds3GetPathStateValueById(u64 id) {
    PathObject *path;
    u64 world;

    world = dds3GetWorldSecondaryObject();
    path = (PathObject *)func_00110C70(world, id, 6);
    return path->state->value;
}

ActionObj *evtSpawnActionObjD(s32 a, void *work, s32 c) {
    ActionObj *obj = func_00110AA8(0xD);

    obj->unk18 = work;
    obj->unk4 = a;
    obj->unk8 = c;
    return obj;
}

u32 dds3GetPathState(s32 path) {
    return (u32)((PathObject *)path)->state;
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116DE8);

ActionObj *evtSpawnActionObj10(s32 a, void *work, s32 c) {
    ActionObj *obj = func_00110AA8(0x10);

    obj->unk4 = a;
    obj->unk8 = c;
    obj->unk18 = work;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_00116CF8", func_00116FA0);
