#include "common.h"

extern u32 ptyBuildProfileCapSkillList(u32, s32);

extern s32 mdlFlagTest(u32);

typedef struct LevelStep {
    u8 threshold;
    u8 value;
} LevelStep;

extern LevelStep D_00370D08[];

extern s32 D_003BAA00;

INCLUDE_ASM(const s32, "game/code_002653A0", func_002653A0);

extern void itfUpdateFadeColor();
extern void func_00264EF0();
extern void func_00264B08();
extern void func_00264D90();

void mnuRefreshPanelLayer(u8 *work) {
    s32 y = 0x100 - *(s32 *)(work + 0x1574);

    itfUpdateFadeColor(work);
    func_00264EF0(work);
    func_00264B08(work);
    func_00264D90(work);
    func_00266250(0x2C0, 0x3D8, 0, y, work + 0x3E4, 0x53);
    func_002653A0(work);
}

typedef struct {
    u8 pad00[4];
    u16 kind;       /* 0x04 */
    u8 pad06[0xE];
    u16 animation;  /* 0x14 */
} TitleEntry;

void func_002654E8(s32 arg0) {
    brsStepAnimDecay(arg0);
}

u8 func_00265500(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= D_00370D08[i].threshold) {
            return D_00370D08[i].value;
        }
    }
    return D_00370D08[0].value;
}

u8 func_00265540(s32 position, s32 increment) {
    u8 *table = (u8 *)D_00370D08;
    s32 i = 2;
    u8 *limit = table + 4;
    s32 end = position + increment;
    do {
        if (position < *limit && end >= *limit) {
            return limit[1];
        }
        limit -= 2;
    } while (--i >= 0);
    return 0;
}

u32 func_00265590(void) {
    return 1;
}

u32 func_00265598(void) {
    return 0;
}

extern u8 D_0036F40C[];

INCLUDE_ASM(const s32, "game/code_002653A0", ptyComputeTotalExp);

s32 brsCalcApGain(u8 *unit, s32 baseApTotal, s32 perUnitBonus) {
    s32 r;
    if (*(u16 *)(unit + 0x0E) & 0x40) {
        return 0;
    }
    r = baseApTotal;
    r += perUnitBonus;
    if ((*(u16 *)(unit + 0x0) & 2) == 0) {
        r = perUnitBonus;
        r += baseApTotal;
    }
    return r;
}

s32 brsCalcExpGain(u8 *unit, s32 exp, s32 a2) {
    s32 result;

    if ((*(u16 *)unit & 2) != 0) {
        result = exp;
    } else {
        result = 0;
        if (ptyHasSkill(unit, 0x21F) != 0) {
            result = exp / 2;
        }
        if (ptyHasSkill(unit, 0x220) != 0) {
            result = exp;
        }
    }
    return result;
}

s32 mnuIsTitleEntryAvailable(TitleEntry *entry) {
    if (mdlFlagTest(0x902) == 0 && entry->kind == 4) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildRewardRows);

INCLUDE_ASM(const s32, "game/code_002653A0", ptyCalcLevelUps);

extern s32 D_003BAA00;

s32 mnuCountAdvancingTitleAnimations(void) {
    s32 offset = 0;
    s32 count = 0;
    s32 remaining = 4;
    do {
        s32 step = ptyCalcLevelUps(D_003BAA00 + 0xa60 + offset);
        count += step > 0;
        offset += 0x1a4;
    } while (--remaining >= 0);
    return count;
}

INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildLevelUpList);

INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildProfileCapList);

s32 mnuAdvanceTitleEntryAnimation(TitleEntry *entry) {
    s32 step = ptyCalcLevelUps(entry);
    entry->animation += step;
    ptyRecomputeMaxHpMp(entry);
    return step;
}

s32 btlAddBaseStats(u8 *src, u8 *obj) {
    s32 i;

    for (i = 0; i < 5; i++) {
        s8 *stat = (s8 *)(obj + 0x16 + i);

        *stat += src[i * 4];
        if (*stat >= 100) {
            *stat = 99;
        }
    }
    ptyRecomputeMaxHpMp(obj);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002653A0", ptyAccumulateStatGains);

INCLUDE_ASM(const s32, "game/code_002653A0", ptyBuildProfileCapSkillList);

void mnuInitTitleParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
    memset(state, 0, 0x10);
    state[0] = first;
    state[1] = second;
    state[2] = third;
    state[3] = fourth;
}

extern s32 ptyCalcLevelUps(u8 *);
extern s32 ptyComputeTotalExp(u8 *, s32);
extern s32 ptyAddProfilePoints(u8 *, s32);
extern s8 ptyGetCurrentProfileId(u8 *);
extern u32 prfGetCapValue(u16);
extern void mnuInitTitleParameters(u32 *, u32, u32, u32, u32);

void brsBuildUnitProgressRow(u8 *state, u8 *entry) {
    s32 levelDelta;
    s32 profilePoints;

    memset(state, 0, 0x2C);
    *(u32 *)(state + 0x8) = (u32)entry;
    levelDelta = ptyCalcLevelUps(entry);
    mnuInitTitleParameters((u32 *)(state + 0xC), 0x6E0, 0x50,
        *(s32 *)(entry + 0x10) - ptyComputeTotalExp(entry, levelDelta),
        ptyComputeTotalExp(entry, levelDelta + 1) - ptyComputeTotalExp(entry, levelDelta));
    profilePoints = ptyAddProfilePoints(entry, 0);
    mnuInitTitleParameters((u32 *)(state + 0x1C), 0x3C0, 0x50, profilePoints,
        prfGetCapValue(ptyGetCurrentProfileId(entry) & 0xFFFF));
}

void func_00266130(u32 fontContext) {
    func_001953D8(fontContext, 0xc, 0x10);
    frFontSetFlagAndMeasureGlyphs(fontContext, 0xfffffffffffffffc);
}

extern u32 func_002C1630(u32, u32, s32);

u32 func_00266168(u32 a, u32 b, u32 c, s32 blend, u8 *resource) {
    ptyGetCurrentProfileId(*(u32 *)(resource + 8));
    return func_002C1630(0x80808080, 0x80808000, blend);
}

INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildActiveUnitProgressRows);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266250);

INCLUDE_RODATA(const s32, "game/code_002653A0", D_003AFBA0);

