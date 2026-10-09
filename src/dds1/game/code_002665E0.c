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

extern f32 sdfSinPoly(f32);

void func_00267850(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->levelAnimation[index].iconState) {
    case 0:
        work->levelAnimation[index].iconColor = 0;
        work->levelAnimation[index].iconPosition[0] = 0xA6;
        work->levelAnimation[index].iconPosition[1] = 0x22;
        work->levelAnimation[index].iconAngle = 0xB4;
        break;
    case 1:
        work->levelAnimation[index].iconAngle = (work->levelAnimation[index].iconAngle + 15) % 360;
        work->levelAnimation[index].iconColor =
            (u32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->levelAnimation[index].iconPosition[1] = 0x24 - bob;
        work->levelAnimation[index].iconColor = (work->levelAnimation[index].iconColor > 0) ? ((work->levelAnimation[index].iconColor > 0x80) ? 0x80 : work->levelAnimation[index].iconColor) : 0;
        y = work->levelAnimation[index].iconPosition[1];
        work->levelAnimation[index].iconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->levelAnimation[index].iconColor >= 0x80) {
            work->levelAnimation[index].iconState += 2;
            work->levelAnimation[index].iconAngle = 0x78;
        }
        break;
    case 2:
        work->levelAnimation[index].iconState++;
        work->levelAnimation[index].iconAngle = 0x78;
        break;
    default:
        work->levelAnimation[index].iconColor -= 10;
        work->levelAnimation[index].iconColor = (work->levelAnimation[index].iconColor > 0) ? ((work->levelAnimation[index].iconColor > 0x80) ? 0x80 : work->levelAnimation[index].iconColor) : 0;
        y = work->levelAnimation[index].iconPosition[1] - 1;
        work->levelAnimation[index].iconPosition[1] = y;
        work->levelAnimation[index].iconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->levelAnimation[index].iconColor <= 0) {
            work->levelAnimation[index].iconState = 0;
        }
        break;
    }
    switch (work->levelAnimation[index].completionState) {
    case 0:
        work->levelAnimation[index].auxiliaryColor = 0;
        work->levelAnimation[index].auxiliaryPosition[0] = 0x95;
        work->levelAnimation[index].auxiliaryPosition[1] = 0x15;
        break;
    case 1:
        work->levelAnimation[index].auxiliaryColor += 10;
        work->levelAnimation[index].auxiliaryColor = (work->levelAnimation[index].auxiliaryColor > 0) ? ((work->levelAnimation[index].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[index].auxiliaryColor) : 0;
        if (work->levelAnimation[index].auxiliaryColor >= 0x80) {
            work->levelAnimation[index].completionState++;
            work->levelAnimation[index].completionColor = 0;
        }
        break;
    case 2:
        work->levelAnimation[index].completionColor++;
        work->levelAnimation[index].completionColor = (work->levelAnimation[index].completionColor > 0) ? ((work->levelAnimation[index].completionColor > 8) ? 8 : work->levelAnimation[index].completionColor) : 0;
        if (work->levelAnimation[index].completionColor >= 8) {
            work->levelAnimation[index].completionState++;
        }
        break;
    default:
        work->levelAnimation[index].auxiliaryColor -= 0x10;
        work->levelAnimation[index].auxiliaryColor = (work->levelAnimation[index].auxiliaryColor > 0) ? ((work->levelAnimation[index].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[index].auxiliaryColor) : 0;
        if (work->levelAnimation[index].auxiliaryColor <= 0) {
            work->levelAnimation[index].completionState = 0;
        }
        break;
    }
}


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

