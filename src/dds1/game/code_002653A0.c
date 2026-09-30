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

void brsDecaySharedAnimCounter(s32 animationState) {
    brsStepAnimDecay(animationState);
}

u8 brsGetLevelStepForValue(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= D_00370D08[i].threshold) {
            return D_00370D08[i].value;
        }
    }
    return D_00370D08[0].value;
}

u8 brsGetLevelStepCrossedBy(s32 position, s32 increment) {
    u8 *table = (u8 *)D_00370D08;
    s32 i = 2;
    u8 *limit = table + 4;
    s32 nextPosition = position + increment;
    do {
        if (position < *limit && nextPosition >= *limit) {
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

#define BRS_ACTIVE_PARTY_FLAG 2
#define BRS_AP_BLOCKED_FLAG 0x40
#define BRS_HALF_EXP_SKILL 0x21F
#define BRS_FULL_EXP_SKILL 0x220

/* Party-unit flags and AP restriction shared by reward calculations. */
typedef struct BrsExpUnit {
    u16 flags;          /* 0x00: bit 1 means active party member */
    u8 pad02[0xC];
    u16 apStatus;       /* 0x0E: bit 6 prevents AP gain */
} BrsExpUnit;

s32 brsCalcApGain(u8 *unit, s32 baseApTotal, s32 perUnitBonus) {
    s32 gain;
    if (((BrsExpUnit *)unit)->apStatus & BRS_AP_BLOCKED_FLAG) {
        return 0;
    }
    gain = baseApTotal;
    gain += perUnitBonus;
    if ((((BrsExpUnit *)unit)->flags & BRS_ACTIVE_PARTY_FLAG) == 0) {
        gain = perUnitBonus;
        gain += baseApTotal;
    }
    return gain;
}

/* Active party members take full EXP; benched members need the half/full
 * EXP skills (0x21F/0x220 respectively). The third caller arg is unused. */
s32 brsCalcExpGain(u8 *unit, s32 exp, s32 unused) {
    s32 result;

    if ((((BrsExpUnit *)unit)->flags & BRS_ACTIVE_PARTY_FLAG) != 0) {
        result = exp;
    } else {
        result = 0;
        if (ptyHasSkill(unit, BRS_HALF_EXP_SKILL) != 0) {
            result = exp / 2;
        }
        if (ptyHasSkill(unit, BRS_FULL_EXP_SKILL) != 0) {
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

/* Five contiguous signed base stats begin at offset 0x16 in the party unit. */
typedef struct {
    u8 pad00[0x16];
    s8 baseStats[5];
} BrsStatUnit;

s32 btlAddBaseStats(u8 *src, u8 *obj) {
    s32 i;

    for (i = 0; i < 5; i++) {
        s8 *stat = &((BrsStatUnit *)obj)->baseStats[i];

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

void mnuTitleInitFourParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
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
extern void mnuTitleInitFourParameters(u32 *, u32, u32, u32, u32);

typedef struct BrsUnitExp {
    u8 pad00[0x10];
    s32 totalExp;        /* 0x10 */
} BrsUnitExp;

typedef struct BrsProgressRow {
    u8 pad00[8];
    u32 unit;            /* 0x08 */
    u32 levelProgress[4]; /* 0x0C */
    u32 profileProgress[4]; /* 0x1C */
} BrsProgressRow;

/* Set up the level and profile progress bars for one party member. */
void brsBuildUnitProgressRow(u8 *state, u8 *entry) {
    s32 levelDelta;
    s32 profilePoints;

    memset(state, 0, 0x2C);
    ((BrsProgressRow *)state)->unit = (u32)entry;
    levelDelta = ptyCalcLevelUps(entry);
    mnuTitleInitFourParameters(((BrsProgressRow *)state)->levelProgress, 0x6E0, 0x50,
        ((BrsUnitExp *)entry)->totalExp - ptyComputeTotalExp(entry, levelDelta),
        ptyComputeTotalExp(entry, levelDelta + 1) - ptyComputeTotalExp(entry, levelDelta));
    profilePoints = ptyAddProfilePoints(entry, 0);
    mnuTitleInitFourParameters(((BrsProgressRow *)state)->profileProgress, 0x3C0, 0x50, profilePoints,
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

