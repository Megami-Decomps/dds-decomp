#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[0x80];
    u32 handle;
} CameraData;

typedef struct {
    u8 pad0[0x18];
    CameraData *data;
} CameraObject;

void dds3DestroyCameraData(CameraObject *camera) {
    CameraData *data;

    effObjFreeInner();
    data = camera->data;
    func_00111840(data->handle);
    func_002CFF98(data);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001129F8);

u32 func_00112AE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112AE8);

u32 dds3GetCameraHandle(CameraObject *camera) {
    return camera->data->handle;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112BC0);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C08);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D00);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D98);

void func_00112E58(void) {
    func_00110928();
}

void func_00112E70(void *obj) {
    VU0_LOAD_MATRIX(*(void **)((u8 *)obj + 0x18));
}

void func_00112E90(void *obj, void *src) {
    PCP_COPY_VECTOR(*(u8 **)((u8 *)obj + 0x18) + 0x60, src);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112EA8);

void func_00112EC0(u8 *obj, f32 value) {
    u8 *state = *(u8 **)(obj + 0x18);
    *(f32 *)(state + 0x8C) = value;
    *(u32 *)(state + 0x88) |= 1;
}

f32 func_00112ED8(u8 *obj) {
    return *(f32 *)(*(u8 **)(obj + 0x18) + 0x8C);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112EE8);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_0039F6F8);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_0039F708);

