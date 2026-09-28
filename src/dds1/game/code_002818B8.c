#include "common.h"

void func_002818B8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 1;
    temp_v0 = arg1 * 0x134 + arg0 + 0x16c;
    do {
        temp_v1 = temp_v1 - 1;
        func_00281898(temp_v0);
        temp_v0 = temp_v0 + 0x20;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281908);

INCLUDE_ASM(const s32, "game/code_002818B8", func_002819F8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281AE8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281BE0);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281D40);

INCLUDE_RODATA(const s32, "game/code_002818B8", D_003B23D8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00282360);


INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC750);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC758);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC760);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC768);


INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC770);

