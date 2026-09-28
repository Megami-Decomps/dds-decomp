#include "common.h"

typedef struct {
    u8 pad0[0x14];
    u32 state;
} SceneObject;

typedef struct {
    u8 pad0[0x18];
    SceneObject *object;
} Scene;

void func_00110DE0(Scene *scene) {
    SceneObject *object;

    object = scene->object;
    func_0012ADA0();
    object->state = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E08);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E80);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110EE8);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110F28);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110FB0);
