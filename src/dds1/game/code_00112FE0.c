#include "common.h"

typedef struct {
    u8 pad0[0xC];
    u32 valueC;
    u8 pad10[0x74];
    s32 mode;
} CameraData;

typedef struct {
    u8 pad0[0x18];
    CameraData *data;
} CameraObject;

u32 dds3GetCameraMode(CameraObject *camera) {
    return camera->data->mode;
}

void dds3SetCameraMode(CameraObject *camera, s32 mode) {
    if (camera->data->mode != mode) {
        camera->data->mode = mode;
    }
}

u32 func_00113008(CameraObject *camera) {
    return camera->data->valueC;
}

INCLUDE_ASM(const s32, "game/code_00112FE0", func_00113018);
