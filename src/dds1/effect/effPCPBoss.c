#include "common.h"

extern u64 func_00163258(u64, u64);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184538);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184630);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184890);

void func_001849C0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00184890(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184A08);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184B30);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184BC8);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001855B8);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001855C8);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001855D0);

void func_00185670(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    temp_v2 = func_00163258(arg0, 2);
    func_001855D0(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001856D8);

void func_00185758(u32 arg0) {
    func_001629F0(*(u32 *)((s32)arg0 + 0x2c));
    func_001629F0(*(u32 *)((s32)arg0 + 0x28));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185790);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001858C8);

void func_001858E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

u32 func_001858E8(void) {
    return 0;
}

u32 func_001858F0(void) {
    return 0;
}

u32 func_001858F8(void) {
    return 0;
}

void func_00185900(void) {
}

void func_00185908(void) {
}

void func_00185910(void) {
}

u32 func_00185918(void) {
    return 0;
}

u32 func_00185920(void) {
    return 0;
}

u32 func_00185928(void) {
    return 0;
}

void func_00185930(void) {
}

void func_00185938(void) {
}

void func_00185940(void) {
}

void func_00185948(void) {
}

void func_00185950(void) {
}

void func_00185958(void) {
}

u32 func_00185960(void) {
    return 0;
}

void func_00185968(void) {
}

void func_00185970(void) {
}

void func_00185978(void) {
}

void func_00185980(void) {
}

void func_00185988(void) {
}

void func_00185990(void) {
}

void func_00185998(void) {
}

void func_001859A0(void) {
}

void func_001859A8(void) {
}

u32 func_001859B0(void) {
    return 0;
}

u32 func_001859B8(void) {
    return 0;
}

u32 func_001859C0(void) {
    return 0;
}

void func_001859C8(void) {
}

void func_001859D0(void) {
}

void func_001859D8(void) {
}

void func_001859E0(void) {
}

u32 func_001859E8(void) {
    return 0;
}

u32 func_001859F0(void) {
    return 0;
}

u32 func_001859F8(void) {
    return 0;
}

void func_00185A00(void) {
}

void func_00185A08(void) {
}

void func_00185A10(void) {
}

void func_00185A18(void) {
}

u32 func_00185A20(void) {
    return 0;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185A28);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185A30);

void func_00185A38(void) {
}

void func_00185A40(void) {
}

void func_00185A48(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185A50);

void func_00185AB8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00163258(arg0, 0);
    temp_v1 = func_00163258(arg0, 1);
    func_00185A50(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185B00);

void func_00185B78(void) {
    func_00184B30();
}

void func_00185B90(void) {
    func_00184BC8();
}

void func_00185BA8(void) {
    func_001855B8();
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185BC0);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185BD8);

void func_00185DE0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_00185BD8(temp_v0);
}

void func_00185E00(void) {
    func_00185BD8();
}
