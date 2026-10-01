#include "common.h"

extern u8 D_003BA709;

extern u32 D_003BA70C;
extern u32 D_003BA710;
extern u32 D_003BA714;

extern u32 D_003BD680;

extern u32 D_003BA700;

extern u8 D_003BA708;

INCLUDE_ASM(const s32, "game/code_00100000", func_00100000);

INCLUDE_ASM(const s32, "game/code_00100000", _start);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D0);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D8);

void func_00100500(void) {
    D_003BA708 = 1;
}

u32 func_00100510(void) {
    return D_003BA700;
}

u32 func_00100518(void) {
    return D_003BD680;
}

void func_00100520(void) {
    D_003BA710 = 0;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

void func_00100538(void) {
    D_003BA70C = 0;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void func_00100548(u32 value) {
    D_003BA710 = value;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_00100560);

extern u32 D_003BA710;
extern u32 D_003BA714;
extern u32 D_003BA718;

void func_00100588(void) {
    D_003BA70C = 0;
    D_003BA718 = 4;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void func_001005A0(void) {
}

void func_001005A8(void) {
}

void func_001005B0(void) {
    D_003BA709 = 0;
}

void func_001005B8(void) {
    D_003BA709 = 1;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_001005C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001006E0);

INCLUDE_ASM(const s32, "game/code_00100000", func_00100858);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA700);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA704);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA708);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA709);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA70C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA710);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA714);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA718);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA71C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA720);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA724);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA728);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA72A);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA72C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA730);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA734);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA738);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA740);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA748);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA750);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA758);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA760);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA768);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA770);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA778);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA780);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA788);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA790);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA798);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7A0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7A8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7B0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7B8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7C0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7C8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7D0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7D8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7E0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7E8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7F0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7F8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA7FC);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedStartTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA804);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA808);

INCLUDE_SDATA(const s32, "game/code_00100000", kwlnDelayedDestroyTaskHead);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA810);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA814);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA818);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA81C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_003BA820);

