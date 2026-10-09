#include "common.h"
#include "mnu_result.h"

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;



struct EffectSlotSet;
extern void func_00306C28(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void *memcpy(void *, const void *, u32);
extern u32 D_004285E0[4];


extern u32 uiBlendColors(u32, u32, u32);

/* Fade a result row's background in; the row is ready once it is fully opaque. */
void func_0029DF18(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 context, s32 index) {
    s32 fade = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[index].backgroundState == 0) {
        work->fadeAnimation[index].backgroundOpacity = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->fadeAnimation[index].backgroundOpacity >= 0x80) {
            work->fadeAnimation[index].backgroundState = 1;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428560);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428570);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029DFB0);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E220);

/* Fade a result row's portrait in; the row is ready once the opacity reaches 0x60. */
void func_0029E478(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 context, s32 index) {
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

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E548);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E820);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029EE80);

extern f32 sdfSinPoly(f32);

void func_0029F440(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->levelAnimation[index].progressIconEnabled) {
    case 0:
        work->levelAnimation[index].progressIconOpacity = 0;
        work->levelAnimation[index].progressIconPosition[0] = 0xA6;
        work->levelAnimation[index].progressIconPosition[1] = 0x22;
        work->levelAnimation[index].progressIconAngle = 0xB4;
        break;
    case 1:
        work->levelAnimation[index].progressIconAngle = (work->levelAnimation[index].progressIconAngle + 15) % 360;
        work->levelAnimation[index].progressIconOpacity =
            (u32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->levelAnimation[index].progressIconPosition[1] = 0x24 - bob;
        work->levelAnimation[index].progressIconOpacity = (work->levelAnimation[index].progressIconOpacity > 0) ? ((work->levelAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->levelAnimation[index].progressIconOpacity) : 0;
        y = work->levelAnimation[index].progressIconPosition[1];
        work->levelAnimation[index].progressIconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->levelAnimation[index].progressIconOpacity >= 0x80) {
            work->levelAnimation[index].progressIconEnabled += 2;
            work->levelAnimation[index].progressIconAngle = 0x78;
        }
        break;
    case 2:
        work->levelAnimation[index].progressIconEnabled++;
        work->levelAnimation[index].progressIconAngle = 0x78;
        break;
    default:
        work->levelAnimation[index].progressIconOpacity -= 10;
        work->levelAnimation[index].progressIconOpacity = (work->levelAnimation[index].progressIconOpacity > 0) ? ((work->levelAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->levelAnimation[index].progressIconOpacity) : 0;
        y = work->levelAnimation[index].progressIconPosition[1] - 1;
        work->levelAnimation[index].progressIconPosition[1] = y;
        work->levelAnimation[index].progressIconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->levelAnimation[index].progressIconOpacity <= 0) {
            work->levelAnimation[index].progressIconEnabled = 0;
        }
        break;
    }
    switch (work->levelAnimation[index].iconState) {
    case 0:
        work->levelAnimation[index].unk54 = 0;
        work->levelAnimation[index].unk58[0] = 0x95;
        work->levelAnimation[index].unk58[1] = 0x15;
        break;
    case 1:
        work->levelAnimation[index].unk54 += 10;
        work->levelAnimation[index].unk54 = (work->levelAnimation[index].unk54 > 0) ? ((work->levelAnimation[index].unk54 > 0x80) ? 0x80 : work->levelAnimation[index].unk54) : 0;
        if (work->levelAnimation[index].unk54 >= 0x80) {
            work->levelAnimation[index].iconState++;
            work->levelAnimation[index].iconOpacity = 0;
        }
        break;
    case 2:
        work->levelAnimation[index].iconOpacity++;
        work->levelAnimation[index].iconOpacity = (work->levelAnimation[index].iconOpacity > 0) ? ((work->levelAnimation[index].iconOpacity > 8) ? 8 : work->levelAnimation[index].iconOpacity) : 0;
        if (work->levelAnimation[index].iconOpacity >= 8) {
            work->levelAnimation[index].iconState++;
        }
        break;
    default:
        work->levelAnimation[index].unk54 -= 0x10;
        work->levelAnimation[index].unk54 = (work->levelAnimation[index].unk54 > 0) ? ((work->levelAnimation[index].unk54 > 0x80) ? 0x80 : work->levelAnimation[index].unk54) : 0;
        if (work->levelAnimation[index].unk54 <= 0) {
            work->levelAnimation[index].iconState = 0;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029FA98);

void func_0029FBE0(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->profileAnimation[index].progressIconEnabled) {
    case 0:
        work->profileAnimation[index].progressIconOpacity = 0;
        work->profileAnimation[index].progressIconPosition[0] = 0x12F;
        work->profileAnimation[index].progressIconPosition[1] = 0x22;
        work->profileAnimation[index].progressIconAngle = 0xB4;
        break;
    case 1:
        work->profileAnimation[index].progressIconAngle = (work->profileAnimation[index].progressIconAngle + 15) % 360;
        work->profileAnimation[index].progressIconOpacity =
            (u32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->profileAnimation[index].progressIconPosition[1] = 0x24 - bob;
        work->profileAnimation[index].progressIconOpacity = (work->profileAnimation[index].progressIconOpacity > 0) ? ((work->profileAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->profileAnimation[index].progressIconOpacity) : 0;
        y = work->profileAnimation[index].progressIconPosition[1];
        work->profileAnimation[index].progressIconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->profileAnimation[index].progressIconOpacity >= 0x80) {
            work->profileAnimation[index].progressIconEnabled += 2;
            work->profileAnimation[index].progressIconAngle = 0x78;
        }
        break;
    case 2:
        work->profileAnimation[index].progressIconEnabled++;
        work->profileAnimation[index].progressIconAngle = 0x78;
        break;
    default:
        work->profileAnimation[index].progressIconOpacity -= 10;
        work->profileAnimation[index].progressIconOpacity = (work->profileAnimation[index].progressIconOpacity > 0) ? ((work->profileAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->profileAnimation[index].progressIconOpacity) : 0;
        y = work->profileAnimation[index].progressIconPosition[1] - 1;
        work->profileAnimation[index].progressIconPosition[1] = y;
        work->profileAnimation[index].progressIconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->profileAnimation[index].progressIconOpacity <= 0) {
            work->profileAnimation[index].progressIconEnabled = 0;
        }
        break;
    }
    switch (work->profileAnimation[index].iconState) {
    case 0:
        work->profileAnimation[index].unk54 = 0;
        work->profileAnimation[index].unk58[0] = 0x122;
        work->profileAnimation[index].unk58[1] = 0x15;
        break;
    case 1:
        work->profileAnimation[index].unk54 = 0x80;
        work->profileAnimation[index].iconState++;
        work->profileAnimation[index].iconOpacity = 0;
        break;
    case 2:
        work->profileAnimation[index].iconOpacity = 0xFF;
        work->profileAnimation[index].iconState++;
        break;
    default:
        work->profileAnimation[index].iconOpacity -= 8;
        work->profileAnimation[index].iconOpacity = (work->profileAnimation[index].iconOpacity > 0)
            ? ((work->profileAnimation[index].iconOpacity > 0xFF) ? 0xFF : work->profileAnimation[index].iconOpacity)
            : 0;
        break;
    }
}

void func_002A0148(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 depth, s32 index) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285E0, sizeof(color));

    if (work->teardownResource != NULL) {
        if (work->profileAnimation[index].iconState >= 2) {
            packed = work->profileAnimation[index].iconOpacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            func_00306C28(x + 0x1180, y + 0x130, z, color, 0,
                         work->teardownResource, 0x16, depth);
            func_00306C28(x + 0x17E0, y + 0x130, z, color, 0,
                         work->teardownResource, 0x17, depth);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029DF18", func_002A0278);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428590);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285E0);

