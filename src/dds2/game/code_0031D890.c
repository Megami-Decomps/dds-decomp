#include "common.h"

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031D890);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031D928);

u32 * func_0031D948(s32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0;
    puVar1 = *(u32 **)(arg0 + 4);
    if (0 < *(s32 *)(arg0 + 8)) {
        do {
            if ((*puVar1 & 1) == 0) {
                *puVar1 = *puVar1 | 1;
                return puVar1;
            }
            temp_v0 = temp_v0 + 1;
            puVar1 = puVar1 + 5;
        } while (temp_v0 < *(s32 *)(arg0 + 8));
    }
    return (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031D998);

void func_0031DA20(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffe;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DA38);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DEB8);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DF48);

u32 * func_0031DF68(s32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0;
    puVar1 = *(u32 **)(arg0 + 4);
    if (0 < *(s32 *)(arg0 + 8)) {
        do {
            if ((*puVar1 & 1) == 0) {
                *puVar1 = *puVar1 | 1;
                return puVar1;
            }
            temp_v0 = temp_v0 + 1;
            puVar1 = puVar1 + 4;
        } while (temp_v0 < *(s32 *)(arg0 + 8));
    }
    return (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DFB8);

void func_0031E008(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffe;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E020);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E198);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E240);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E2E8);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E410);

INCLUDE_SDATA(const s32, "game/code_0031D890", D_00438958);

