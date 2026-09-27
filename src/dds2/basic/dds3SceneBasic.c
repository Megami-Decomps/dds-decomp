#include "common.h"

void func_00110DE0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_0012ADA0();
    *(u32 *)(temp_v0 + 0x14) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E08);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E80);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110EE8);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110F28);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110FB0);
