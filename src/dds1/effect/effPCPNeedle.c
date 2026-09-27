#include "common.h"

extern u64 func_00163258(u64, u64);

void func_001763B8(s32 arg0) {
    func_0015B8B8(*(u32 *)(arg0 + 0x68));
    func_00176B50(*(u32 *)(arg0 + 0x6c));
    func_002D0918(*(u32 *)(arg0 + 0x70));
}

void func_001763F0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_00163258(arg0, 0);
    func_001760F8(temp_v0);
}

void func_00176410(void) {
    func_001760F8();
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00176428);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00176A00);

void func_00176A10(s32 arg0) {
    func_001770F8(*(u32 *)(arg0 + 0x6c));
}
