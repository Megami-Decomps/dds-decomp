#include "common.h"

extern s32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BC38);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BCA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BD18);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BDD0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BF18);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C030);

u8 func_0025C098(s32 arg0) {
    return *(s32 *)(arg0 + 0x6c) != 0;
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C0A8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C0D8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C1C8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C278);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C350);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C418);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C4C8);

void func_0025C568(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *arg0 = temp_v0 + 1;
    if (0x3c < temp_v0 + 1) {
        *arg0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C588);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C7E0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C830);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C8D0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CA50);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CFA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D100);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2C0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2F8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D628);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D798);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D7F8);

s32 *func_0025DA90(void) {
    s32 *temp_v0 = (s32 *)func_002CFEB8(0x14);

    memset(temp_v0, 0, 0x14);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DAD0);

u32 func_0025DB58(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_002CFF98();
    return temp_v0;
}

void func_0025DB80(s32 *arg0) {
    s32 temp_v0 = arg0[2];

    while (temp_v0 != NULL) {
        temp_v0 = func_0025DB58(temp_v0);
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DBB0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DCC8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DD80);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DDF0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DE60);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA18);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D8);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E8);

