#include "common.h"

extern u64 func_00159978(u64, u32);

extern u64 func_00343ED0(u64, u32 *, u64);

INCLUDE_ASM(const s32, "effect/billManager", func_00157EA0);

INCLUDE_ASM(const s32, "effect/billManager", func_00158340);

INCLUDE_ASM(const s32, "effect/billManager", func_00158430);

INCLUDE_ASM(const s32, "effect/billManager", func_00158AA0);

INCLUDE_ASM(const s32, "effect/billManager", func_00158C00);

INCLUDE_ASM(const s32, "effect/billManager", func_00158D68);

INCLUDE_ASM(const s32, "effect/billManager", func_00158DB0);

INCLUDE_ASM(const s32, "effect/billManager", func_00158E00);

INCLUDE_ASM(const s32, "effect/billManager", func_00158E38);

INCLUDE_ASM(const s32, "effect/billManager", func_00158E50);

INCLUDE_ASM(const s32, "effect/billManager", func_00158ED8);

INCLUDE_ASM(const s32, "effect/billManager", func_00158F58);

INCLUDE_ASM(const s32, "effect/billManager", func_00158F88);

INCLUDE_ASM(const s32, "effect/billManager", func_00159158);

INCLUDE_ASM(const s32, "effect/billManager", func_001591D8);

void func_00159490(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 *piVar3;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 4);
    piVar3 = (s32 *)(*(s32 *)(arg0 + 8) + arg1 * 0x14);
    temp_v1 = *piVar3;
    *(s32 **)(arg2 + 0xc) = piVar3;
    temp_v2 = temp_v2 + temp_v1;
    *(u32 *)(arg2 + 4) = 0;
    temp_v0 = *(s16 *)(temp_v2 + 0x12);
    *(s32 *)(arg2 + 0x10) = temp_v2;
    *(s32 *)(arg2 + 8) = (s32)temp_v0;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001594C8);

INCLUDE_ASM(const s32, "effect/billManager", func_00159678);

INCLUDE_ASM(const s32, "effect/billManager", func_00159848);

INCLUDE_ASM(const s32, "effect/billManager", func_001598D8);

INCLUDE_ASM(const s32, "effect/billManager", func_00159978);

u64 func_001599F8(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg1, temp_v2, 0);
    temp_v1 = func_00159978(arg0, temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00159A50);

INCLUDE_ASM(const s32, "effect/billManager", func_00159AF0);

INCLUDE_ASM(const s32, "effect/billManager", func_00159B28);
