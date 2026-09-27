#include "common.h"

extern u32 func_0017AC70(u32);

extern u32 func_0017AD10(u32);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001787E0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178848);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178938);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001789C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178B80);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179168);

void func_00179178(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x7c) = arg1;
}

void func_00179180(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x3c) = *(float *)(arg1 + 0x3c) * arg0;
    *(float *)(arg1 + 0x4c) = *(float *)(arg1 + 0x4c) * arg0;
    *(float *)(arg1 + 0x50) = *(float *)(arg1 + 0x50) * arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001791A8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179438);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001794A0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179590);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179618);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179780);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179D78);

void func_00179D88(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x6c) = arg1;
}

void func_00179D90(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x40) = *(float *)(arg1 + 0x40) * arg0;
    *(float *)(arg1 + 0x44) = *(float *)(arg1 + 0x44) * arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179DB0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A058);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A0C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A1C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A248);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A340);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A8A0);

void func_0017A8B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

void func_0017A8B8(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A8C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A9C8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AA08);

void func_0017ABE0(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_0017AC70(arg1);
    *(u32 *)(arg0 + 0x30) = temp_v0;
}

void func_0017AC10(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_0017AD10(*(u32 *)(arg1 + 0x30));
    *(u32 *)(arg0 + 0x30) = temp_v0;
}

s32 func_0017AC40(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x20) + arg1 * 0x60;
}

s32 func_0017AC58(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x24) + arg1 * 0x18;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AC70);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017ACC0);

u32 func_0017AD10(u32 arg0) {
    *(s32 *)((s32)arg0 + 4) = *(s32 *)((s32)arg0 + 4) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AD28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AF40);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AF88);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AFD0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B000);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B390);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B520);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B718);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B730);

void func_0017B738(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x180) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B740);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B7A0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B9D0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BA18);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BA60);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BA90);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BE08);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BFA8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C250);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C268);

void func_0017C270(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x184) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C278);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C2D8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C4D8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C520);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C568);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C598);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C988);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CB28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE00);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE18);

void func_0017CE20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE88);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D078);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D0C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D108);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D138);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D3D8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D560);
