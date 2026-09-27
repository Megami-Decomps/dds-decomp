#include "common.h"

extern u64 func_00163258(u64, u64);

void func_001634A0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_001632E0(temp_v0);
}

void func_001634C0(void) {
    func_001632E0();
}

void func_001634D8(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0x5c));
    func_002D0918(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163508);

void func_00163518(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_00163520(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x1c) = *(float *)(arg1 + 0x54) * arg0;
    *(float *)(arg1 + 0x20) = *(float *)(arg1 + 0x58) * arg0;
}

u32 func_00163540(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163548);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163780);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163AF8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163CD0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163E10);

void func_00163FD0(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0x5c));
    func_002D0918(*(u32 *)(arg0 + 0x60));
}

void func_00164000(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_00163E10(temp_v0);
}

void func_00164020(void) {
    func_00163E10();
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164038);

void func_00164048(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_00164050(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x1c) = *(float *)(arg1 + 0x54) * arg0;
    *(float *)(arg1 + 0x20) = *(float *)(arg1 + 0x58) * arg0;
}

u32 func_00164070(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164078);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001642B0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001645A0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001646F8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164838);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164A30);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164A98);

void func_00164AA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xa8) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164AB0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164BA8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165110);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165418);

void func_001655D0(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0x60));
    func_002D0918(*(u32 *)(arg0 + 100));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165600);

void func_00165630(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

u32 func_00165638(u32 arg0) {
    return arg0;
}

void func_00165640(s32 arg0) {
    func_0015CCD0(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void func_00165668(s32 arg0) {
    func_0015CDF0(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void func_00165690(s32 arg0) {
    func_0015CC58(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001656B8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165758);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165D80);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165ED0);

void func_001660C8(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0x5c));
    func_0015B8B8(*(u32 *)(arg0 + 0x60));
    func_002D0918(*(u32 *)(arg0 + 100));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166100);

void func_00166130(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166138);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001661D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166810);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166960);

void func_00166AF0(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0x50));
    func_002D0918(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166B20);

void func_00166B30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x4c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166B38);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166C18);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00167070);
