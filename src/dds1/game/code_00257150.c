#include "common.h"

INCLUDE_ASM(const s32, "game/code_00257150", func_00257150);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257200);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257270);

INCLUDE_ASM(const s32, "game/code_00257150", func_002573E8);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257670);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257718);

INCLUDE_ASM(const s32, "game/code_00257150", func_002579B0);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257BD8);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257C10);

INCLUDE_ASM(const s32, "game/code_00257150", func_00257DF0);

void func_00257E78(s32 arg0) {
    s32 temp_v0 = *(s16 *)(arg0 + 0x59C);
    s32 temp_v1 = *(s16 *)(arg0 + 0x59E);
    s32 *temp_v2 = (s32 *)(arg0 + 0x49C);
    s32 temp_v3 = 9;

    do {
        temp_v2[0] = temp_v0;
        temp_v2[1] = temp_v1;
        temp_v2 += 2;
        temp_v3--;
    } while (temp_v3 >= 0);
}

void func_00257EB0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *arg0 = temp_v0 + 1;
    if (0x3c < temp_v0 + 1) {
        *arg0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00257150", func_00257ED0);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258258);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258508);

INCLUDE_RODATA(const s32, "game/code_00257150", D_003AF950);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258620);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258A70);

void func_00258AF0(u32 *arg0, u32 arg1) {
    arg0[1] = arg1;
    *arg0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00257150", func_00258B00);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258B90);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258EB8);

INCLUDE_ASM(const s32, "game/code_00257150", func_00258FD0);

INCLUDE_ASM(const s32, "game/code_00257150", func_002593E0);

INCLUDE_ASM(const s32, "game/code_00257150", func_00259498);

INCLUDE_ASM(const s32, "game/code_00257150", func_00259890);

INCLUDE_ASM(const s32, "game/code_00257150", func_00259B40);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025A680);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025AA20);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025AB38);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025AC50);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025AD68);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025AE80);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025B0F0);

INCLUDE_RODATA(const s32, "game/code_00257150", D_003AF9B8);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025B350);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025B7B0);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025B7D8);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025B888);

INCLUDE_ASM(const s32, "game/code_00257150", func_0025BA20);
