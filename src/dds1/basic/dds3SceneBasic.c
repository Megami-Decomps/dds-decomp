#include "common.h"

typedef struct {
    u8 pad0[0x14];
    u32 state;
} SceneObject;

typedef struct {
    u8 pad0[0x18];
    SceneObject *object;
} Scene;

void dds3ClearSceneObjectState(Scene *scene) {
    SceneObject *object;

    object = scene->object;
    func_00128890();
    object->state = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110BE0);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110C58);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110CC0);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110D00);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110D88);
