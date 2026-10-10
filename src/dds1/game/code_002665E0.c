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

void func_00267FF0(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->profileAnimation[index].iconState) {
    case 0:
        work->profileAnimation[index].iconColor = 0;
        work->profileAnimation[index].iconPosition[0] = 0x12F;
        work->profileAnimation[index].iconPosition[1] = 0x22;
        work->profileAnimation[index].iconAngle = 0xB4;
        break;
    case 1:
        work->profileAnimation[index].iconAngle = (work->profileAnimation[index].iconAngle + 15) % 360;
        work->profileAnimation[index].iconColor =
            (u32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->profileAnimation[index].iconPosition[1] = 0x24 - bob;
        work->profileAnimation[index].iconColor = (work->profileAnimation[index].iconColor > 0) ? ((work->profileAnimation[index].iconColor > 0x80) ? 0x80 : work->profileAnimation[index].iconColor) : 0;
        y = work->profileAnimation[index].iconPosition[1];
        work->profileAnimation[index].iconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->profileAnimation[index].iconColor >= 0x80) {
            work->profileAnimation[index].iconState += 2;
            work->profileAnimation[index].iconAngle = 0x78;
        }
        break;
    case 2:
        work->profileAnimation[index].iconState++;
        work->profileAnimation[index].iconAngle = 0x78;
        break;
    default:
        work->profileAnimation[index].iconColor -= 10;
        work->profileAnimation[index].iconColor = (work->profileAnimation[index].iconColor > 0) ? ((work->profileAnimation[index].iconColor > 0x80) ? 0x80 : work->profileAnimation[index].iconColor) : 0;
        y = work->profileAnimation[index].iconPosition[1] - 1;
        work->profileAnimation[index].iconPosition[1] = y;
        work->profileAnimation[index].iconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->profileAnimation[index].iconColor <= 0) {
            work->profileAnimation[index].iconState = 0;
        }
        break;
    }
    switch (work->profileAnimation[index].completionState) {
    case 0:
        work->profileAnimation[index].auxiliaryColor = 0;
        work->profileAnimation[index].auxiliaryPosition[0] = 0x122;
        work->profileAnimation[index].auxiliaryPosition[1] = 0x15;
        break;
    case 1:
        work->profileAnimation[index].auxiliaryColor += 10;
        work->profileAnimation[index].auxiliaryColor = (work->profileAnimation[index].auxiliaryColor > 0)
            ? ((work->profileAnimation[index].auxiliaryColor > 0x80) ? 0x80 : work->profileAnimation[index].auxiliaryColor)
            : 0;
        if (work->profileAnimation[index].auxiliaryColor >= 0x80) {
            work->profileAnimation[index].completionState++;
            work->profileAnimation[index].completionColor = 0;
        }
        break;
    case 2:
        work->profileAnimation[index].completionColor += 10;
        work->profileAnimation[index].completionColor = (work->profileAnimation[index].completionColor > 0)
            ? ((work->profileAnimation[index].completionColor > 0xFF) ? 0xFF : work->profileAnimation[index].completionColor)
            : 0;
        if (work->profileAnimation[index].completionColor >= 0xFF) {
            work->profileAnimation[index].completionState++;
        }
        break;
    default:
        work->profileAnimation[index].completionColor -= 4;
        work->profileAnimation[index].completionColor = (work->profileAnimation[index].completionColor > 0x80)
            ? ((work->profileAnimation[index].completionColor > 0xFF) ? 0xFF : work->profileAnimation[index].completionColor)
            : 0x80;
        break;
    }
}


INCLUDE_ASM(const s32, "game/code_002665E0", func_00268590);

extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 prfGetCapValue(u16);

s32 func_002687C0(BrsSkillPackageWork *work, BrsProgressRow *entry, s32 index, s8 mode) {
    s32 result = 0;
    s32 levelDelta = work->levelAnimation[index].level - entry->unit->level;
    s32 nextLevelExp = ptyComputeTotalExp(entry->unit, levelDelta + 1);
    s32 range = nextLevelExp - ptyComputeTotalExp(entry->unit, levelDelta);
    s32 step;
    s32 denominator;

    if (range == 0) {
        range = ptyComputeTotalExp(entry->unit, levelDelta + 1);
    }
    if (mode == 0) {
        if (work->levelAnimation[index].remaining > 0) {
            if (work->levelAnimation[index].skipRamp == 0) {
                work->levelAnimation[index].frames++;
                work->levelAnimation[index].frames = work->levelAnimation[index].frames <= 0 ? 0 : (work->levelAnimation[index].frames > 120 ? 120 : work->levelAnimation[index].frames);
                denominator = 150 - work->levelAnimation[index].frames;
                step = 1;
                if (range >= denominator) {
                    step = range / denominator;
                }
            } else {
                step = 10000;
            }
            if (work->unkD4D != 0) {
                step = work->levelAnimation[index].remaining;
            }
            work->levelAnimation[index].remaining -= step;
            if (work->levelAnimation[index].remaining < 0) {
                step += work->levelAnimation[index].remaining;
                work->levelAnimation[index].remaining = 0;
            }
            work->levelAnimation[index].remaining = work->levelAnimation[index].remaining <= 0 ? 0 : (work->levelAnimation[index].remaining > 0x1000000 ? 0x1000000 : work->levelAnimation[index].remaining);
            result = step;
        }
    } else {
        range = prfGetCapValue(ptyGetCurrentProfileId(entry->unit));
        if (work->profileAnimation[index].remaining > 0) {
            if (work->profileAnimation[index].skipRamp == 0) {
                work->profileAnimation[index].frames++;
                work->profileAnimation[index].frames = work->profileAnimation[index].frames <= 0 ? 0 : (work->profileAnimation[index].frames > 120 ? 120 : work->profileAnimation[index].frames);
                denominator = 150 - work->profileAnimation[index].frames;
                step = 1;
                if (range >= denominator) {
                    step = range / denominator;
                }
            } else {
                step = 10000;
            }
            if (work->unkD4D != 0) {
                step = work->profileAnimation[index].remaining;
            }
            work->profileAnimation[index].remaining -= step;
            if (work->profileAnimation[index].remaining < 0) {
                step += work->profileAnimation[index].remaining;
                work->profileAnimation[index].remaining = 0;
            }
            work->profileAnimation[index].remaining = work->profileAnimation[index].remaining <= 0 ? 0 : (work->profileAnimation[index].remaining > 0x1000000 ? 0x1000000 : work->profileAnimation[index].remaining);
            result = step;
        }
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBF0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC30);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC40);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC50);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC60);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC70);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC570);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC578);

