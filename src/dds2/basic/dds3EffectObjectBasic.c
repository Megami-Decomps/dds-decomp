#include "common.h"

void func_001147D8(u32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    puVar1 = *(u32 **)(temp_v0 + 0x18);
    func_00114640(puVar1);
    func_0010F810(arg0);
    func_00111A68(*puVar1);
    func_00328E48(*(u32 *)(temp_v0 + 0x18));
    *(u32 *)(temp_v0 + 0x18) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114828);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114BF0);

u32 func_00114CD0(s32 arg0) {
    return **(u32 **)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114CE0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114D80);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114E58);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114EA0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114EE8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114F30);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114F50);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115020);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115068);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150B0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001151C8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115208);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115248);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115318);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115358);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115500);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115518);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115580);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115600);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001156C8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001156E0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115AA8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115AC0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115B20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115B58);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115B98);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BD8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C50);

void func_00115DF0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) | arg1;
}

void func_00115E08(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) & ~arg1;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115E20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115E88);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115EE8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412950);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412980);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129F8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A10);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A28);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A40);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A50);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A68);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A80);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A98);

