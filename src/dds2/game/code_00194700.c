#include "common.h"

extern u32 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194700);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194770);

INCLUDE_ASM(const s32, "game/code_00194700", func_001947A8);

u32 func_001947F0(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001947F8(u32 *arg0) {
    return *arg0;
}

void func_00194800(void) {
}

u32 func_00194808(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194810);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194850);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194890);

INCLUDE_ASM(const s32, "game/code_00194700", func_001948D0);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194928);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194988);

void func_001949D8(void) {
}

void func_001949E0(void) {
    func_001027D8(0, 0, 0, 0);
}

u32 func_00194A08(u32 arg0) {
    return arg0;
}

void func_00194A10(void) {
}

void func_00194A18(void) {
}

void func_00194A20(void) {
}

void func_00194A28(void) {
}

void func_00194A30(void) {
}

void func_00194A38(void) {
}

void func_00194A40(void) {
}

void func_00194A48(void) {
}

void func_00194A50(void) {
}

void func_00194A58(void) {
}

void func_00194A60(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194A68);

void func_00194A70(void) {
}

u32 func_00194A78(void) {
    return 0;
}

u32 func_00194A80(void) {
    return 0;
}

void func_00194A88(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194A90);

void func_00194A98(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194AA0);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194AA8);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194AF8);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194B28);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194BD0);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195008);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195060);

INCLUDE_ASM(const s32, "game/code_00194700", func_001950F0);

void func_00195570(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x3c);
    if (temp_v0 != 0) {
        func_0032BBB0(temp_v0);
        *(u32 *)((s32)arg0 + 0x3c) = 0;
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001955B0);

u32 func_001955C0(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_001955C8(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001955D0);

void func_00195618(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_00195620(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195628);

void func_00195638(s32 arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_0032BBB0(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
    temp_v1 = func_00343ED0(arg1, temp_v2, 0);
    temp_v0 = func_0032C138(temp_v2[0]);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
    func_003297C8(temp_v1);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001956A8);

void func_001957C0(void) {
    func_001027D8(0, 0, 0, 0);
}

void func_001957E8(void) {
    func_001027D8(0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195810);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195890);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195978);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195A30);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195AB0);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195B38);




INCLUDE_RODATA(const s32, "game/code_00194700", D_004146A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414708);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414718);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414728);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414738);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414748);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414758);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414768);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414778);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414788);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414798);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414808);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414818);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414828);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414838);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414848);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414858);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414868);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414878);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414888);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414898);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414908);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414918);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414928);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414938);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414948);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414958);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414968);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414978);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414990);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149A0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149B0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149C0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149D0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149E0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149F0);

