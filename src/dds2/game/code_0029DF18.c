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

extern void mnuSetTitleSequenceVolumePan(u32);
extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 func_002A0278(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);

void func_0029E820(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *entry, s32 context, s32 index) {
    s32 level = work->levelAnimation[index].level;
    s32 fade = 0x100 - work->fadeProgress;
    s32 nextLevel = level + 1;
    s32 currentExp;
    s32 finished;

    nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
    switch (work->levelAnimation[index].drawPhase) {
    case 0:
        work->levelAnimation[index].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->levelAnimation[index].alpha >= 0x80) {
            work->levelAnimation[index].drawPhase = 1;
        }
        break;
    case 1:
        work->levelAnimation[index].drawPhase++;
        if (work->levelAnimation[index].applied > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (level < 99 && entry->unit->hp != 0 && (entry->unit->status & 0x4000) == 0) {
            if (work->levelAnimation[index].drawPhase == 3) {
                goto flash;
            }
            work->levelAnimation[index].appliedStep = func_002A0278(work, entry, index, 0);
            currentExp = work->levelAnimation[index].remaining + work->levelAnimation[index].appliedStep;
            work->levelAnimation[index].remaining = currentExp;
            if (currentExp >= (s32)ptyComputeTotalExp(entry->unit, nextLevel - entry->unit->level) && work->levelAnimation[index].drawPhase == 2) {
                work->levelAnimation[index].level++;
                work->levelAnimation[index].level = work->levelAnimation[index].level < 2 ? 1 : (work->levelAnimation[index].level > 99 ? 99 : work->levelAnimation[index].level);
                if (work->unkAEB9 != 0) {
                    finished = 0;
                    while (work->levelAnimation[index].level < 99) {
                        nextLevel = work->levelAnimation[index].level + 1;
                        nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
                        if (currentExp >= (s32)ptyComputeTotalExp(entry->unit, nextLevel - entry->unit->level)) {
                            work->levelAnimation[index].level++;
                            work->levelAnimation[index].level = work->levelAnimation[index].level < 2 ? 1 : (work->levelAnimation[index].level > 99 ? 99 : work->levelAnimation[index].level);
                        } else {
                            finished = 1;
                        }
                        if (work->levelAnimation[index].level >= 99) {
                            finished = 1;
                        }
                        if (finished != 0) {
                            break;
                        }
                    }
                }
                work->levelAnimation[index].progressIconEnabled = 1;
                work->levelAnimation[index].iconState = 1;
                work->levelAnimation[index].unk2D = 1;
                work->levelAnimation[index].flashOpacity = 0x80;
                work->levelAnimation[index].flashFrame = 0;
                work->levelAnimation[index].alpha = 0;
                work->levelAnimation[index].drawPhase++;
                mnuSetTitleSequenceVolumePan(0x12);
            }
        } else {
            work->levelAnimation[index].applied = 0;
        }
        if (work->levelAnimation[index].drawPhase == 3) {
flash:
            work->levelAnimation[index].flashFrame++;
            work->levelAnimation[index].flashFrame = work->levelAnimation[index].flashFrame <= 0 ? 0 : (work->levelAnimation[index].flashFrame > 15 ? 15 : work->levelAnimation[index].flashFrame);
            if (work->levelAnimation[index].flashFrame >= 15) {
                work->levelAnimation[index].drawPhase++;
            }
        } else if (work->levelAnimation[index].drawPhase == 4) {
            work->levelAnimation[index].flashOpacity -= 16;
            work->levelAnimation[index].flashOpacity = work->levelAnimation[index].flashOpacity <= 0 ? 0 : (work->levelAnimation[index].flashOpacity > 128 ? 128 : work->levelAnimation[index].flashOpacity);
            if (work->levelAnimation[index].flashOpacity <= 0) {
                work->levelAnimation[index].unk2D = 0;
            }
            work->levelAnimation[index].alpha += 8;
            work->levelAnimation[index].alpha = work->levelAnimation[index].alpha <= 0 ? 0 : (work->levelAnimation[index].alpha > 128 ? 128 : work->levelAnimation[index].alpha);
            if (work->levelAnimation[index].alpha >= 128) {
                work->levelAnimation[index].drawPhase = 2;
                if (work->levelAnimation[index].applied > 0 && work->resultPhase == 0) {
                    mnuSetTitleSequenceVolumePan(0x14);
                    work->resultPhase = 1;
                }
            }
        }
        break;
    }
}

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

extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);

s32 func_002A0278(BrsSkillPackageWork *work, BrsProgressRow *entry, s32 index, s8 mode) {
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
        if (work->levelAnimation[index].applied > 0) {
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
            if (work->unkAEB9 != 0) {
                step = work->levelAnimation[index].applied;
            }
            work->levelAnimation[index].applied -= step;
            if (work->levelAnimation[index].applied < 0) {
                step += work->levelAnimation[index].applied;
                work->levelAnimation[index].applied = 0;
            }
            work->levelAnimation[index].applied = work->levelAnimation[index].applied <= 0 ? 0 : (work->levelAnimation[index].applied > 0x1000000 ? 0x1000000 : work->levelAnimation[index].applied);
            result = step;
        }
    } else {
        range = ptyGetProfileRecordCap(ptyGetCurrentProfileId(entry->unit));
        if (work->profileAnimation[index].applied > 0) {
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
            if (work->unkAEB9 != 0) {
                step = work->profileAnimation[index].applied;
            }
            work->profileAnimation[index].applied -= step;
            if (work->profileAnimation[index].applied < 0) {
                step += work->profileAnimation[index].applied;
                work->profileAnimation[index].applied = 0;
            }
            work->profileAnimation[index].applied = work->profileAnimation[index].applied <= 0 ? 0 : (work->profileAnimation[index].applied > 0x1000000 ? 0x1000000 : work->profileAnimation[index].applied);
            result = step;
        }
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428590);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285E0);

