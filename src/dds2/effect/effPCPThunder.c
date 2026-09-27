#include "common.h"

extern u64 func_0016AEB0(u64, u64);

void func_0016B0F8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_0016AF38(temp_v0);
}

void func_0016B118(void) {
    func_0016AF38();
}

void func_0016B130(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x5c));
    func_003297C8(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B160);

void func_0016B170(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_0016B178(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x1c) = *(float *)(arg1 + 0x54) * arg0;
    *(float *)(arg1 + 0x20) = *(float *)(arg1 + 0x58) * arg0;
}

u32 func_0016B198(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B1A0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B3D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B750);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B928);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BA68);

void func_0016BC28(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x5c));
    func_003297C8(*(u32 *)(arg0 + 0x60));
}

void func_0016BC58(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_0016AEB0(arg0, 0);
    func_0016BA68(temp_v0);
}

void func_0016BC78(void) {
    func_0016BA68();
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BC90);

void func_0016BCA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_0016BCA8(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x1c) = *(float *)(arg1 + 0x54) * arg0;
    *(float *)(arg1 + 0x20) = *(float *)(arg1 + 0x58) * arg0;
}

u32 func_0016BCC8(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BCD0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BF08);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C1F8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C350);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C490);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C688);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C6F0);

void func_0016C700(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xa8) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C708);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C800);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016CD68);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D070);

void func_0016D228(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x60));
    func_003297C8(*(u32 *)(arg0 + 100));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D258);

void func_0016D288(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

u32 func_0016D290(u32 arg0) {
    return arg0;
}

void func_0016D298(s32 arg0) {
    func_001648C0(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void func_0016D2C0(s32 arg0) {
    func_001649E0(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void func_0016D2E8(s32 arg0) {
    func_00164848(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D310);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D3B0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D9D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DB28);

void func_0016DD20(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x5c));
    func_001634A8(*(u32 *)(arg0 + 0x60));
    func_003297C8(*(u32 *)(arg0 + 100));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DD58);

void func_0016DD88(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DD90);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DE30);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E468);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E5B8);

void func_0016E748(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x50));
    func_003297C8(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E778);

void func_0016E788(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x4c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E790);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E870);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016ECC8);
