#include "common.h"

void func_00112BE0(s32 arg0) {
    s32 temp_v0;

    func_0010F810();
    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00111A68(*(u32 *)(temp_v0 + 0x80));
    func_00328E48(temp_v0);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C20);

u32 func_00112D08(void) {
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D10);

u32 func_00112DD8(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80);
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

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001130E8);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00113100);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00113110);




INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412878);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412888);

