#include "common.h"

extern s32 func_001AA6F8(void);

void func_0021B5C0(void) {
    s32 *piVar1;
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    piVar1 = *(s32 **)(temp_v0 + 0x718);
    temp_v0 = *piVar1;
    if (temp_v0 != 0) {
        func_001E7D30(temp_v0);
        *piVar1 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021B600);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021B670);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021B6C0);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021B788);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021B828);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C0C8);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C390);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C428);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C548);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C5E0);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C7F8);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C818);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041A5E0);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041A5F8);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021CF18);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021E778);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021E8C0);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EA38);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EAF8);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EB28);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EB78);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EBF0);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EC30);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021EC60);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021ED08);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021ED58);

u64 func_0021EDD8(u64 arg0) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = 0;
    if (*(s8 *)(*(s32 *)(temp_v0 + 0x718) + 2) != '\0') {
        temp_v1 = arg0;
    }
    return temp_v1;
}

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AAC8);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AAD8);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AAF8);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB18);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB28);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB38);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB48);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB58);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB68);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB78);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB88);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB98);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041ABA8);

