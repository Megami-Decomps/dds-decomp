#include "common.h"

typedef struct ObjectSubstate {
    u8 pad0[0xC];
    u32 valueC;
    u8 pad10[0x74];
    s32 cameraMode;
} ObjectSubstate;

typedef struct ObjectWithSubstate {
    u8 pad0[0x18];
    ObjectSubstate *substate;
} ObjectWithSubstate;

u32 dds3GetCameraMode(ObjectWithSubstate *object) {
    return object->substate->cameraMode;
}

void dds3SetCameraMode(ObjectWithSubstate *object, s32 value) {
    if (object->substate->cameraMode != value) {
        object->substate->cameraMode = value;
    }
}

u32 func_00113230(ObjectWithSubstate *object) {
    return object->substate->valueC;
}

INCLUDE_ASM(const s32, "game/code_00113208", func_00113240);
