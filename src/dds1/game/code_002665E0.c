#include "common.h"
#include "mnu_result.h"

extern u32 uiBlendColors(u32, u32, s32);


void func_002665E0(s32 a0, s32 a1, s32 a2, BrsSkillPackageWork *work, s32 a4, s32 a5, s32 index) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[index].backgroundState == 0) {
        u32 opacity = uiBlendColors(0x80808080, 0x80808000, remaining) & 0xFF;

        work->fadeAnimation[index].backgroundOpacity = opacity;
        if (work->fadeAnimation[index].backgroundOpacity >= 0x80) {
            work->fadeAnimation[index].backgroundState = 1;
        }
    }
}
INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBB0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBC0);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00266668);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00266908);

void func_00266B10(s32 a0, s32 a1, s32 a2, BrsSkillPackageWork *work, s32 a4, s32 a5, s32 index) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[index].portraitReady == 0) {
        u32 opacity = uiBlendColors(0x80808060, 0x80808000, remaining) & 0xFF;

        work->fadeAnimation[index].portraitOpacity = opacity;
        work->fadeAnimation[index].portraitPosition[0] = 0;
        work->fadeAnimation[index].portraitPosition[1] = 0;
        if (work->fadeAnimation[index].portraitOpacity >= 0x60) {
            work->fadeAnimation[index].portraitReady = 1;
        }
    }
}
INCLUDE_ASM(const s32, "game/code_002665E0", func_00266BC0);


INCLUDE_ASM(const s32, "game/code_002665E0", func_00266E28);

INCLUDE_ASM(const s32, "game/code_002665E0", func_002673C8);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00267850);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00267E20);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00267FF0);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00268590);

INCLUDE_ASM(const s32, "game/code_002665E0", func_002687C0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBF0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC30);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC40);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC50);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC60);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC70);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC570);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC578);

