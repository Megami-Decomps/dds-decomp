#include "common.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_result.h"
#include "kwln.h"

extern u32 ptyBuildProfileCapSkillList(DatPartyRecord *, PrfSkillList *);

extern s32 mdlFlagTest(u32);

typedef struct LevelStep {
    u8 threshold;
    u8 value;
} LevelStep;

extern LevelStep brsLevelStepThresholds[];

extern s32 ptyCalcLevelUps(DatPartyRecord *);
extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern void ptyRecomputeMaxHpMp(DatPartyRecord *);
extern s32 ptyHasSkill(DatPartyRecord *, u16);


extern struct { s32 v[6]; } D_0036D4B0;
extern u32 uiBlendColors(u32, u32, s32);
extern void itfDrawCountText(s32, s32, s32, s32, const BrsRewardSummary *, s32);
extern void mnuQueueRightAlignedFormattedInfoText(s32, s32, s32, s32, const BrsRewardSummary *, s32);
extern void func_002650C8(s32, s32, s32, u32, BrsRewardSummary *, s32, BrsSkillPackageWork *);

void mnuTitleDrawFadeMenuEntries(BrsSkillPackageWork *work) {
    BrsRewardSummary *res = &work->rewards;
    s32 rowOffset = 0x80;
    u32 color = uiBlendColors(0x80808080, 0x80808000, 0x100 - work->fadeProgress);

    itfDrawCountText(D_0036D4B0.v[0], D_0036D4B0.v[1] + rowOffset, 0, color, res, 0x53);
    mnuQueueRightAlignedFormattedInfoText(D_0036D4B0.v[2], D_0036D4B0.v[3] + rowOffset, 0, color, res, 0x53);
    func_002650C8(D_0036D4B0.v[4], D_0036D4B0.v[5] + rowOffset, 0, color, res, 0x53, work);
}


extern void itfUpdateFadeColor(BrsSkillPackageWork *);
extern void mnuDrawTitleFadeSprites(BrsSkillPackageWork *);
extern void brsStepAnimDecay(BrsSkillPackageWork *);
extern void func_00266250(s32, s32, s32, s32, BrsActiveProgressList *, s32);

extern void func_00264B08();
extern void func_00264D90();

void mnuRefreshPanelLayer(BrsSkillPackageWork *work) {
    s32 y = 0x100 - work->fadeProgress;

    itfUpdateFadeColor(work);
    mnuDrawTitleFadeSprites(work);
    func_00264B08(work);
    func_00264D90(work);
    func_00266250(0x2C0, 0x3D8, 0, y, &work->partyProgress, 0x53);
    mnuTitleDrawFadeMenuEntries(work);
}


void brsDecaySharedAnimCounter(BrsSkillPackageWork *animationState) {
    brsStepAnimDecay(animationState);
}

u8 brsGetLevelStepForValue(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= brsLevelStepThresholds[i].threshold) {
            return brsLevelStepThresholds[i].value;
        }
    }
    return brsLevelStepThresholds[0].value;
}

u8 brsGetLevelStepCrossedBy(s32 position, s32 increment) {
    u8 *table = (u8 *)brsLevelStepThresholds;
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


INCLUDE_ASM(const s32, "game/code_002653A0", ptyComputeTotalExp);

#define BRS_ACTIVE_PARTY_FLAG 2
#define BRS_AP_BLOCKED_FLAG 0x40
#define BRS_HALF_EXP_SKILL 0x21F
#define BRS_FULL_EXP_SKILL 0x220


s32 brsCalcApGain(DatPartyRecord *unit, s32 baseApTotal, s32 perUnitBonus) {
    s32 gain;
    if (unit->status & BRS_AP_BLOCKED_FLAG) {
        return 0;
    }
    gain = baseApTotal;
    gain += perUnitBonus;
    if ((unit->flags & BRS_ACTIVE_PARTY_FLAG) == 0) {
        gain = perUnitBonus;
        gain += baseApTotal;
    }
    return gain;
}

/* Active party members take full EXP; benched members need the half/full
 * EXP skills (0x21F/0x220 respectively). The third caller arg is unused. */
s32 brsCalcExpGain(DatPartyRecord *unit, s32 exp, s32 unused) {
    s32 result;

    if ((unit->flags & BRS_ACTIVE_PARTY_FLAG) != 0) {
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

s32 mnuIsTitleEntryAvailable(DatPartyRecord *entry) {
    if (mdlFlagTest(0x902) == 0 && entry->unitId == 4) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildRewardRows);

INCLUDE_ASM(const s32, "game/code_002653A0", ptyCalcLevelUps);


s32 mnuCountAdvancingTitleAnimations(void) {
    s32 i;
    s32 count = 0;
    for (i = 0; i < 5; i++) {
        s32 step = ptyCalcLevelUps(&datGameState->party[i]);
        count += step > 0;
    }
    return count;
}


INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildLevelUpList);



extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern s32 ptyTestProfileFlag0(DatPartyRecord *, u16);
extern u32 prfBuildSkillListState0(DatPartyRecord *, DatProfileRecord *, PrfSkillList *);
extern u32 prfGetCapValue(u16);

INCLUDE_ASM(const s32, "game/code_002653A0", brsBuildProfileCapList);

s32 mnuAdvanceTitleEntryAnimation(DatPartyRecord *entry) {
    s32 step = ptyCalcLevelUps(entry);
    entry->level += step;
    ptyRecomputeMaxHpMp(entry);
    return step;
}


s32 btlAddBaseStats(s32 *src, DatPartyRecord *obj) {
    s32 i;

    for (i = 0; i < 5; i++) {
        s8 *stat = &obj->baseStats[i];

        *stat += src[i];
        if (*stat >= 100) {
            *stat = 99;
        }
    }
    ptyRecomputeMaxHpMp(obj);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002653A0", ptyAccumulateStatGains);


extern void scrClearFlags(DatPartyRecord *);
extern s32 scrSetFlag(DatPartyRecord *, u16);
extern void scrSetSecondaryScriptFlag(DatPartyRecord *, u16);

/* Apply the capped profile's pending skills and return their list to the caller. */
u32 ptyBuildProfileCapSkillList(DatPartyRecord *unit, PrfSkillList *output) {
    PrfSkillList skills;
    DatProfileRecord *profile;
    u32 count = 0;
    u32 i;

    memset(&skills, 0, sizeof(skills));
    profile = ptyGetCurrentProfileRecord(unit);
    if (unit->profileId != 0) {
        if (prfGetCapValue(unit->profileId) == profile->value) {
            if (ptyTestProfileFlag0(unit, unit->profileId) == 0) {
                prfBuildSkillListState0(unit, profile, &skills);
                count = skills.count;
                if (count != 0) {
                    scrClearFlags(unit);
                    for (i = 0; i < skills.count; i++) {
                        u16 skill = skills.skills[i];
                        scrSetFlag(unit, skill);
                        scrSetSecondaryScriptFlag(unit, skill);
                    }
                }
            }
        }
    }
    *output = skills;
    return count;
}

void mnuTitleInitFourParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
    memset(state, 0, 0x10);
    state[0] = first;
    state[1] = second;
    state[2] = third;
    state[3] = fourth;
}

extern u32 ptyAddProfilePoints(DatPartyRecord *, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 prfGetCapValue(u16);
extern void mnuTitleInitFourParameters(u32 *, u32, u32, u32, u32);



/* Set up the level and profile progress bars for one party member. */
void brsBuildUnitProgressRow(BrsProgressRow *state, DatPartyRecord *entry) {
    s32 levelDelta;
    s32 profilePoints;

    memset(state, 0, 0x2C);
    state->unit = entry;
    levelDelta = ptyCalcLevelUps(entry);
    mnuTitleInitFourParameters(state->levelProgress, 0x6E0, 0x50,
        entry->totalExp - ptyComputeTotalExp(entry, levelDelta),
        ptyComputeTotalExp(entry, levelDelta + 1) - ptyComputeTotalExp(entry, levelDelta));
    profilePoints = ptyAddProfilePoints(entry, 0);
    mnuTitleInitFourParameters(state->profileProgress, 0x3C0, 0x50, profilePoints,
        prfGetCapValue(ptyGetCurrentProfileId(entry) & 0xFFFF));
}

void mnuSetFontChainDimensionsAndMeasure(u32 fontContext) {
    frFontSetGlyphChainDimensions(fontContext, 0xc, 0x10);
    frFontSetFlagAndMeasureGlyphs(fontContext, 0xfffffffffffffffc);
}

extern u32 uiBlendColors(u32, u32, s32);

/* The row renderer also passes its context word; this blend helper ignores it. */
u32 mnuBlendNeutralColorAlpha(u32 a, u32 b, u32 c, s32 blend, BrsProgressRow *resource, s32 context) {
    ptyGetCurrentProfileId(resource->unit);
    return uiBlendColors(0x80808080, 0x80808000, blend);
}


void brsBuildActiveUnitProgressRows(BrsActiveProgressList *output) {
    s32 i;

    memset(output, 0, sizeof(*output));
    for (i = 0; i < 5; i++) {
        DatPartyRecord *unit = &datGameState->party[i];
        if ((unit->flags & 1) != 0) {
            brsBuildUnitProgressRow(&output->rows[output->count++], unit);
        }
    }
}

extern char D_003AFBA0[];
extern u8 D_00324510[2][2][16];
extern KwlnTask *kwlnTaskGetTaskByName(const char *);
extern u32 kwlnTaskGetUserValue(KwlnTask *);
extern void func_002665E0(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00266668(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00266B10(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00266BC0(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00266E28(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00268AB8(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_002673C8(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00268D40(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00267850(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00267E20(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00267FF0(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00268590(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_00266908(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern s32 brsPollResultCounterCompletion(void);
extern void func_002E8DD0(u32 sound);

void func_00266250(s32 x, s32 y, s32 z, s32 alpha, BrsActiveProgressList *list, s32 context) {
    KwlnTask *task;
    BrsSkillPackageWork *work = NULL;
    DatPartyRecord *unit;
    s32 i;
    s32 count;

    task = kwlnTaskGetTaskByName(D_003AFBA0);
    if (task != NULL) {
        work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    }
    if (((s8)D_00324510[1][0][1] < 0 || (D_00324510[1][0][3] & 2)) && work->opacity >= 0x80) {
        work->unkD4D = 1;
    }
    count = list->count;
    for (i = 0; i < count; i++) {
        if (work != NULL) {
            BrsProgressRow *row = &list->rows[i];

            func_002665E0(x, y, z, work, row, context, i);
            func_00266668(x, y, z, work, row, context, i);
            func_00266B10(x, y, z, work, row, context, i);
            func_00266BC0(x, y, z, work, row, context, i);
            mnuBlendNeutralColorAlpha(x, y, z, alpha, row, context);
            func_00266E28(x, y, z, work, row, context, i);
            func_00268AB8(x, y, z, work, row, context, i);
            func_002673C8(x, y, z, work, row, context, i);
            func_00268D40(x, y, z, work, row, context, i);
            func_00267850(x, y, z, work, row, context, i);
            func_00267E20(x, y, z, work, row, context, i);
            func_00267FF0(x, y, z, work, row, context, i);
            func_00268590(x, y, z, work, row, context, i);
            func_00266908(x, y, z, work, row, context, i);
            count = list->count;
        }
        unit = list->rows[i].unit;
        if (unit->flags & 2) {
            if (i + 1 >= count || (list->rows[i + 1].unit->flags & 2)) {
                y += 0x1E0;
            } else {
                y += 0x258;
            }
        } else {
            y += 0x1E0;
        }
    }
    if (brsPollResultCounterCompletion() != 0 && work != NULL && work->resultPhase == 1) {
        work->resultPhase = 2;
        func_002E8DD0(20);
    }
}

INCLUDE_RODATA(const s32, "game/code_002653A0", D_003AFBA0);

