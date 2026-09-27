#include "common.h"

void func_00110BB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00128890();
    *(u32 *)(temp_v0 + 0x14) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110BE0);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110C58);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110CC0);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110D00);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110D88);
