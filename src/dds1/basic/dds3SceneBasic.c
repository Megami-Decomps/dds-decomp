#include "common.h"
#include "dds3obj.h"

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
