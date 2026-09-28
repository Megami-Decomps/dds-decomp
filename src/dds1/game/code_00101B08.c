#include "common.h"

extern s32 func_001028A0(void);

extern u32 D_003BA9BC;

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101B08);

void func_00101B78(u32 arg0, u32 arg1, u32 arg2) {
    func_00101060(1, arg0, arg1, arg2);
}

void func_00101BA8(u32 arg0, u32 arg1, u32 arg2) {
    func_00101060(0, arg0, arg1, arg2);
}

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101BD8);

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101D60);

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101E40);

INCLUDE_ASM(const s32, "game/code_00101B08", func_001024D8);

u32 func_00102850(void) {
    func_0010FA00(D_003BA9BC);
    return 0;
}

u32 func_00102878(void) {
    func_0010FA40(D_003BA9BC);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00101B08", func_001028A0);

u32 func_001028C8(void) {
    s32 temp_v0;

    temp_v0 = func_001028A0();
    return *(u32 *)(temp_v0 + 4);
}

INCLUDE_ASM(const s32, "game/code_00101B08", func_001028E8);










INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039E018);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA83C);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA844);


INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA848);

