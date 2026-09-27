#include "common.h"

void func_001165A0(u32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = *(s32 *)(temp_v1 + 0x18);
    func_00328E48(*(u32 *)(temp_v0 + 0x78));
    func_00111A68(*(u32 *)(temp_v0 + 0x74));
    func_00328E48(*(u32 *)(temp_v1 + 0x18));
    *(u32 *)(temp_v1 + 0x18) = 0;
    func_0010F810(arg0);
}

INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", func_001165F0);
