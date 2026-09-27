#include "common.h"

void func_00160B58(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x174);
    if (temp_v0 != 0) {
        func_003297C8(temp_v0);
    }
    func_0015B510(arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00160B98);

INCLUDE_ASM(const s32, "effect/parManager", func_00160EF8);

INCLUDE_ASM(const s32, "effect/parManager", func_001612D8);

INCLUDE_ASM(const s32, "effect/parManager", func_001616A8);

INCLUDE_ASM(const s32, "effect/parManager", func_001617F8);

void func_001618C8(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "effect/parManager", func_001618E0);

INCLUDE_ASM(const s32, "effect/parManager", func_00161958);

INCLUDE_ASM(const s32, "effect/parManager", func_00161A10);

void func_00161B20(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 4) + 4) = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00161B38);

INCLUDE_ASM(const s32, "effect/parManager", func_00161D08);

INCLUDE_ASM(const s32, "effect/parManager", func_00161EE8);

INCLUDE_ASM(const s32, "effect/parManager", func_00161F68);

INCLUDE_ASM(const s32, "effect/parManager", func_00161FB0);

INCLUDE_ASM(const s32, "effect/parManager", func_00161FE8);

INCLUDE_ASM(const s32, "effect/parManager", func_001621A8);

INCLUDE_ASM(const s32, "effect/parManager", func_001621F8);

INCLUDE_ASM(const s32, "effect/parManager", func_00162248);

u16 func_001622D0(s32 arg0) {
    return *(u16 *)(arg0 + 0x142);
}

INCLUDE_ASM(const s32, "effect/parManager", func_001622D8);

INCLUDE_ASM(const s32, "effect/parManager", func_001622E8);
