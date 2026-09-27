#include "common.h"

extern u32 func_001730B8(u32);

extern u32 func_00173018(u32);

extern u64 func_00163258(u64, u64);

void func_00170B88(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    temp_v2 = func_00163258(arg0, 2);
    func_001708A0(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170BF0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170CE0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170D68);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170F28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171510);

void func_00171520(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x7c) = arg1;
}

void func_00171528(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x3c) = *(float *)(arg1 + 0x3c) * arg0;
    *(float *)(arg1 + 0x4c) = *(float *)(arg1 + 0x4c) * arg0;
    *(float *)(arg1 + 0x50) = *(float *)(arg1 + 0x50) * arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171550);

void func_001717E0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    temp_v2 = func_00163258(arg0, 2);
    func_00171550(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171848);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171938);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001719C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171B28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172120);

void func_00172130(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x6c) = arg1;
}

void func_00172138(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x40) = *(float *)(arg1 + 0x40) * arg0;
    *(float *)(arg1 + 0x44) = *(float *)(arg1 + 0x44) * arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172158);

void func_00172400(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    temp_v2 = func_00163258(arg0, 2);
    func_00172158(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172468);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172568);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001725F0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001726E8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172C48);

void func_00172C58(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

void func_00172C60(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172C68);

void func_00172D70(s32 arg0) {
    if (*(s32 *)(arg0 + 0x30) != 0) {
        func_00173068(*(s32 *)(arg0 + 0x30));
    }
    func_002DAA68(*(u32 *)(arg0 + 0x28));
    func_002D0918(*(u32 *)(arg0 + 0x2c));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172DB0);

void func_00172F88(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_00173018(arg1);
    *(u32 *)(arg0 + 0x30) = temp_v0;
}

void func_00172FB8(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_001730B8(*(u32 *)(arg1 + 0x30));
    *(u32 *)(arg0 + 0x30) = temp_v0;
}

s32 func_00172FE8(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x20) + arg1 * 0x60;
}

s32 func_00173000(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x24) + arg1 * 0x18;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173018);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173068);

u32 func_001730B8(u32 arg0) {
    *(s32 *)((s32)arg0 + 4) = *(s32 *)((s32)arg0 + 4) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001730D0);

void func_001732E8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_001730D0(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173330);

void func_00173378(s32 arg0) {
    func_00175D88(*(u32 *)(arg0 + 0x184));
    func_002D0918(*(u32 *)(arg0 + 0x188));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001733A8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173738);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001738C8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173AC0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173AD8);

void func_00173AE0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x180) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173AE8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173B48);

void func_00173D78(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00173B48(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173DC0);

void func_00173E08(s32 arg0) {
    func_00175D88(*(u32 *)(arg0 + 0x18c));
    func_002D0918(*(u32 *)(arg0 + 400));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173E38);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001741B0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174350);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001745F8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174610);

void func_00174618(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x184) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174620);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174680);

void func_00174880(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00174680(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001748C8);

void func_00174910(s32 arg0) {
    func_00175D88(*(u32 *)(arg0 + 0x194));
    func_002D0918(*(u32 *)(arg0 + 0x198));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174940);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174D30);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174ED0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001751A8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001751C0);

void func_001751C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001751D0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175230);

void func_00175420(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00175230(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175468);

void func_001754B0(s32 arg0) {
    func_00175D88(*(u32 *)(arg0 + 0x134));
    func_002D0918(*(u32 *)(arg0 + 0x138));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001754E0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175780);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175908);
