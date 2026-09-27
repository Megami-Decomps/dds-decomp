#include "common.h"

extern u64 func_00111610(u64);
extern s64 func_00111AD0(u64, u64);

extern u32 func_00112888(void);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111840);

void func_00111908(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112888();
    *puVar1 = *puVar1 | arg1;
}

void func_00111938(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112888();
    *puVar1 = *puVar1 & ~arg1;
}

u8 func_00111970(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112888();
    return (*puVar1 & arg1) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001119A0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001119E8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111A08);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111A68);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111AD0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111B00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111B20);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111B40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BD8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E30);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111F40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112008);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001120C0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112100);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001122F0);

void func_00112750(u64 arg0) {
    s64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00111AD0(arg0, 1);
    if (temp_v0 == 0) {
        temp_v1 = func_00111610(arg0);
        func_00111A08(arg0, temp_v1);
        return;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127A0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127C0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112800);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112848);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112868);
