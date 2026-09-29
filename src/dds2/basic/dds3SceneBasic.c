#include "common.h"
#include "dds3obj.h"

void dds3ClearSceneObjectState(Scene *scene) {
    SceneObject *object;

    object = scene->object;
    fldReleaseFieldResources();
    object->state = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E08);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E80);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110EE8);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110F28);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110FB0);
