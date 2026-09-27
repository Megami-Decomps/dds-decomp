#include "common.h"

extern u64 func_0014FE28(void);

extern u64 func_0014FE48(void);

extern u64 func_0014FD20(void);

extern u64 func_00151E08(u64, u64);

extern u64 func_00151D88(u64, u64);

extern u64 func_00151E60(u32);

void func_00114570(u32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    puVar1 = *(u32 **)(temp_v0 + 0x18);
    func_001143D8(puVar1);
    func_0010F5E8(arg0);
    func_00111840(*puVar1);
    func_002CFF98(*(u32 *)(temp_v0 + 0x18));
    *(u32 *)(temp_v0 + 0x18) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001145C0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114988);

u32 func_00114A68(s32 arg0) {
    return **(u32 **)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114A78);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114B18);

void func_00114BF0(s32 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00151E60(*(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc));
    func_00114B18(temp_v0, arg1, arg2);
}

void func_00114C38(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00151D88(1, arg0);
    func_00114B18(temp_v0, arg1, arg2);
}

void func_00114C80(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00151E08(1, arg0);
    func_00114B18(temp_v0, arg1, arg2);
}

void func_00114CC8(s32 arg0) {
    func_00152200(*(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc));
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114CE8);

void func_00114DB8(s32 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00151E60(*(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc));
    func_00114CE8(temp_v0, arg1, arg2);
}

void func_00114E00(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00151D88(0, arg0);
    func_00114CE8(temp_v0, arg1, arg2);
}

void func_00114E48(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00151E08(0, arg0);
    func_00114CE8(temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114E90);

void func_00114F60(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_0014FD20();
    func_00114E90(temp_v0, arg1, arg2);
}

void func_00114FA0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_0014FE48();
    func_00114E90(temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114FE0);

void func_001150B0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_0014FE28();
    func_00114FE0(temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F0);

void func_00115298(void) {
    func_001150F0();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001152B0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115318);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115398);

void func_00115460(void) {
    func_00115398();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115478);

void func_00115840(void) {
    func_00115478();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115858);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001158B8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001158F0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115930);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115970);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001159E8);

void func_00115B88(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) | arg1;
}

void func_00115BA0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) & ~arg1;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BB8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C80);
