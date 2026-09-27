#include "common.h"

extern void *func_00111610(void *arg);
extern u32 func_00111AD0(void *arg0, s32 index);

extern u32 func_00112888(void);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111840);

void func_00111908(void *obj, s32 flag) {
    u32 *flags;

    flags = (u32 *)func_00112888();
    *flags = *flags | flag;
}

void func_00111938(void *obj, s32 flag) {
    u32 *flags;

    flags = (u32 *)func_00112888();
    *flags = *flags & ~flag;
}

u8 func_00111970(void *obj, s32 flag) {
    u32 *flags;

    flags = (u32 *)func_00112888();
    return (*flags & flag) != 0;
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

void func_00112750(void *arg0) {
    u32 exists;
    void *val;

    exists = func_00111AD0(arg0, 1);
    if (exists == 0) {
        val = func_00111610(arg0);
        func_00111A08(arg0, val);
        return;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127A0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127C0);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112800);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112848);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112868);
