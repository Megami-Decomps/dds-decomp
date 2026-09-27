#include "common.h"

void func_00158F68(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x174);
    if (temp_v0 != 0) {
        func_002D0918(temp_v0);
    }
    func_00153920(arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00158FA8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159308);

INCLUDE_ASM(const s32, "effect/parManager", func_001596E8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159AB8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159C08);

void func_00159CD8(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159CF0);

INCLUDE_ASM(const s32, "effect/parManager", func_00159D68);

INCLUDE_ASM(const s32, "effect/parManager", func_00159E20);

void func_00159F30(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 4) + 4) = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159F48);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A118);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A2F8);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A378);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A3C0);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A3F8);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A5B8);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A608);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A658);

u16 func_0015A6E0(s32 arg0) {
    return *(u16 *)(arg0 + 0x142);
}

INCLUDE_ASM(const s32, "effect/parManager", func_0015A6E8);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A6F8);
