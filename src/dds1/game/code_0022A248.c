#include "common.h"

extern s8 D_003BD88C;

extern u32 D_003BBDF0;

void func_0022A850(s32 arg0);

void func_0022AB00(s32 arg0);

void func_0022AB90(void);

extern u32 D_003BD890;

extern u32 D_003BD894;

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A248);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A850);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A8D8);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A960);

s32 func_0022AAF0(void) {
    return D_003BD88C != 0;
}

void func_0022AB00(s32 arg0) {
    if (arg0 == 0) {
        D_003BD88C = 0;
        D_003BD890 = 0;
        D_003BD894 = 0;
        return;
    }
    D_003BD894 = (s32)arg0;
    D_003BD88C = 3;
    D_003BD890 = 0;
}

void func_0022AB28(s32 arg0) {
    if (arg0 == 0) {
        D_003BD88C = 5;
        D_003BD894 = 1;
        D_003BD890 = 0;
    } else {
        D_003BD890 = arg0;
        D_003BD88C = 5;
        D_003BD894 = arg0;
    }
}

void func_0022AB58(void) {
}

u32 func_0022AB60(void) {
    func_0022AB58();
    return 0;
}

void *func_0022AB80(void) {
    return (void *)func_0022AB90;
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AB90);

void func_0022AEB8(void) {
    func_0010BDB8();
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AED0);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD18);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AF18);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD38);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD48);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD58);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD68);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD78);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AF50);

void func_0022B618(void) {
    D_003BBDF0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B620);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B6C0);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B710);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B7A0);


INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDEC);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF0);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF4);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF8);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE00);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE08);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE10);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE18);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE20);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE28);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE30);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE38);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE40);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE48);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE50);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE58);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE60);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE68);


INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE70);

