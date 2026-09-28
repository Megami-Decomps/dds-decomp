#include "common.h"

void func_001129B8(s32 arg0) {
    s32 temp_v0;

    func_0010F5E8();
    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00111840(*(u32 *)(temp_v0 + 0x80));
    func_002CFF98(temp_v0);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001129F8);

u32 func_00112AE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112AE8);

u32 func_00112BB0(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112BC0);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C08);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D00);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D98);

void func_00112E58(void) {
    func_00110928();
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112E70);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112E90);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112EA8);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112EC0);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112ED8);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112EE8);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_0039F6F8);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_0039F708);

