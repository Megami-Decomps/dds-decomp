#include "common.h"

void func_0017E680(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E688);

void func_0017E7A8(u32 arg0) {
    func_00333918(*(u32 *)((s32)arg0 + 0x6c));
    func_0017ECA0(arg0);
    func_003297C8(*(u32 *)((s32)arg0 + 0x70));
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E7E0);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017EA78);

void func_0017ECA0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x68) != 0) {
        func_003297C8(*(s32 *)(arg0 + 0x68));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ECD0);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED00);

void func_0017ED30(s32 arg0, s32 arg1, u32 arg2) {
    *(u32 *)(arg1 * 0x14 + *(s32 *)(arg0 + 0x40) + 0x10) = arg2;
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED50);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017ED78);
