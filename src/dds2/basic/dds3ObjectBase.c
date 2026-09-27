#include "common.h"

extern u32 func_00112AB0(void);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111A68);

void func_00111B30(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112AB0();
    *puVar1 = *puVar1 | arg1;
}

void func_00111B60(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112AB0();
    *puVar1 = *puVar1 & ~arg1;
}

u8 func_00111B98(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112AB0();
    return (*puVar1 & arg1) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BC8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111C10);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111C30);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111C90);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111CF8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111D28);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111D48);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111D68);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112168);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112230);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001122E8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112328);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112518);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112978);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001129C8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001129E8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112A28);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112A70);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112A90);
