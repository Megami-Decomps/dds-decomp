#include "common.h"

typedef struct {
    u8 pad0[0x80];
    u32 handle;
    u8 pad84[4];
    u32 flags;
    f32 value8C;
} CameraData;

typedef struct {
    u8 pad0[0x18];
    CameraData *data;
} CameraObject;

void dds3DestroyCameraData(CameraObject *camera) {
    CameraData *data;

    effObjFreeInner();
    data = camera->data;
    func_00111A68(data->handle);
    func_00328E48(data);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C20);

u32 func_00112D08(void) {
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D10);

u32 dds3GetCameraHandle(CameraObject *camera) {
    return camera->data->handle;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112DE8);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112E30);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112F28);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112FC0);

void func_00113080(void) {
    func_00110B50();
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00113098);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001130B8);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001130D0);

void dds3SetCameraValue(CameraObject *camera, f32 value) {
    CameraData *state = camera->data;
    state->value8C = value;
    state->flags |= 1;
}

f32 dds3GetCameraValue(CameraObject *camera) {
    return camera->data->value8C;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00113110);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412878);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412888);

