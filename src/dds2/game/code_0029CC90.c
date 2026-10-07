#include "common.h"
#include "kwln.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_result.h"

extern u32 func_0029D790(DatPartyRecord *, PrfSkillList *);
extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);
extern s32 func_00314990(DatPartyRecord *, u16);
extern s32 prfBuildSkillListState0(DatPartyRecord *, DatProfileRecord *, PrfSkillList *);
extern void scrClearAllSecondaryScriptFlags(DatPartyRecord *);
extern s32 scrSetFlag(DatPartyRecord *, u16);
extern void scrSetSecondaryScriptFlag(DatPartyRecord *, u16);
extern s32 ptyCalcLevelUps(DatPartyRecord *);
extern void ptyRecomputeMaxHpMp(DatPartyRecord *);
extern s32 ptyHasSkill(DatPartyRecord *, u16);



extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);


extern u32 uiBlendColors(u32, u32, s32);

void itfUpdateFadeColor(BrsSkillPackageWork *work);

extern struct { s32 v[6]; } D_003D6500;

extern void mnuCampDrawMenuIconLayer(s32, s32, s32, u32, const BrsRewardSummary *, s32, BrsSkillPackageWork *);

extern void func_0029CB70(s32, s32, s32, u32, const BrsRewardSummary *, s32, BrsSkillPackageWork *);

extern void func_0029C880(s32, s32, s32, u32, BrsRewardSummary *, s32, BrsSkillPackageWork *);

extern void mnuDrawUnitProgressRows(s32 x, s32 y, s32 z, s32 alpha, BrsActiveProgressList *list, s32 context);

void mnuTitleDrawFadeMenuEntries(BrsSkillPackageWork *work) {
    BrsRewardSummary *res = &work->rewards;
    u32 color = uiBlendColors(0xFFF06480, 0xFFF06400, 0x100 - work->fadeProgress);

    mnuCampDrawMenuIconLayer(D_003D6500.v[0], D_003D6500.v[1], 0, color, res, 0x53, work);
    func_0029CB70(D_003D6500.v[2], D_003D6500.v[3], 0, color, res, 0x53, work);
    func_0029C880(D_003D6500.v[4], D_003D6500.v[5], 0, color, res, 0x53, work);
}

void mnuTitleRenderFadeAndPanels(BrsSkillPackageWork *work) {
    s32 remaining = 0x100 - work->fadeProgress;

    itfUpdateFadeColor(work);
    brsDrawResultPanelSprites(work);
    func_0029C120(work);
    func_0029C3F0(work);
    mnuDrawUnitProgressRows(0x1D0, 0x3B8, 0, remaining, &work->partyProgress, 0x53);
    mnuTitleDrawFadeMenuEntries(work);
}

extern void func_0029C810(BrsSkillPackageWork *);

void brsDecaySharedAnimCounter(BrsSkillPackageWork *work) {
    func_0029C810(work);
}

extern u8 brsLevelStepThresholds[];

/* Read the value paired with the highest of three thresholds not above input. */
u8 brsGetLevelStepForValue(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= brsLevelStepThresholds[i * 2]) {
            return brsLevelStepThresholds[i * 2 + 1];
        }
    }
    return brsLevelStepThresholds[1];
}

u8 brsGetLevelStepCrossedBy(s32 position, s32 increment) {
    u8 *table = brsLevelStepThresholds;
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

u32 func_0029CE80(void) {
    return 1;
}

u32 func_0029CE88(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyComputeTotalExp);

#define BRS_ACTIVE_PARTY_FLAG 2
#define BRS_AP_BLOCKED_FLAG 0x40
#define BRS_HALF_EXP_SKILL 0x23F
#define BRS_FULL_EXP_SKILL 0x240


typedef struct BrsProfileApRecord {
    u8 pad00[0xA];
    s16 apMultiplier;
    u8 pad0C[0xD4];
} BrsProfileApRecord;

extern BrsProfileApRecord *D_00435E18;
extern s32 func_001514A8(void);
extern s16 func_001514B8(void);

s32 func_0029CF00(DatPartyRecord *unit, s32 baseApTotal, s32 perUnitBonus) {
    s32 gain;
    s32 profileIndex;

    if (unit->status & BRS_AP_BLOCKED_FLAG) {
        return 0;
    }
    gain = baseApTotal;
    gain += perUnitBonus;
    if ((unit->flags & BRS_ACTIVE_PARTY_FLAG) == 0) {
        gain = perUnitBonus;
        gain += baseApTotal;
    }

    profileIndex = func_001514A8();
    if (profileIndex >= 0) {
        gain += func_001514B8() * D_00435E18[profileIndex].apMultiplier;
    }
    return gain;
}

/* Active party members take full EXP; benched members need the half/full
 * EXP skills (0x23F/0x240 respectively). */
s32 brsCalcExpGain(DatPartyRecord *unit, s32 exp) {
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

u32 func_0029D000(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D008);


INCLUDE_ASM(const s32, "game/code_0029CC90", ptyCalcLevelUps);

s32 mnuCountAdvancingTitleAnimations(void) {
    s32 i;
    s32 count = 0;
    for (i = 0; i < 5; i++) {
        s32 step = ptyCalcLevelUps(&datGameState->party[i]);
        count += step > 0;
    }
    return count;
}



INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D2D8);

/* Build rows for active, capped profiles, excluding unit ID 9. */
INCLUDE_ASM(const s32, "game/code_0029CC90", brsBuildProfileCapList);

s32 mnuAdvanceTitleEntryAnimation(DatPartyRecord *entry) {
    s32 step = ptyCalcLevelUps(entry);
    entry->level += step;
    ptyRecomputeMaxHpMp(entry);
    return step;
}

s32 btlAddBaseStats(s32 *src, DatPartyRecord *seq) {
    s32 i;

    for (i = 0; i < 5; i++) {
        s8 *stat = &seq->baseStats[i];

        *stat += src[i];
        if (*stat >= 100) {
            *stat = 99;
        }
    }
    ptyRecomputeMaxHpMp(seq);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyAccumulateStatGains);

u32 func_0029D790(DatPartyRecord *entry, PrfSkillList *out) {
    PrfSkillList result;
    u32 applied = 0;
    DatProfileRecord *slot;

    memset(&result, 0, 0x34);
    slot = ptyGetCurrentProfileRecord(entry);
    if (entry->profileId != 0 && ptyGetProfileRecordCap(entry->profileId) == slot->value && func_00314990(entry, entry->profileId) == 0) {
        prfBuildSkillListState0(entry, slot, &result);
        applied = result.count;
        if (applied != 0) {
            u32 i = 0;
            u16 *p = result.skills;

            scrClearAllSecondaryScriptFlags(entry);
            if (result.count != 0) {
                do {
                    u16 flag = p[i];

                    i++;
                    scrSetFlag(entry, flag);
                    scrSetSecondaryScriptFlag(entry, flag);
                } while (i < result.count);
            }
        }
    }
    *out = result;
    return applied;
}

void mnuTitleInitFourParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
    memset(state, 0, 0x10);
    state[0] = first;
    state[1] = second;
    state[2] = third;
    state[3] = fourth;
}


extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);

extern u32 ptyAddProfileRecordValueClamped(DatPartyRecord *, u32);

extern s32 func_00314C10(DatPartyRecord *);

extern u32 ptyGetProfileRecordCap(u16);


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
    profilePoints = ptyAddProfileRecordValueClamped(entry, 0);
    mnuTitleInitFourParameters(state->profileProgress, 0x3C0, 0x50, profilePoints,
        ptyGetProfileRecordCap(func_00314C10(entry) & 0xFFFF));
}

u32 mnuBlendNeutralColorAlpha(u32 a, u32 b, u32 c, s32 blend, BrsProgressRow *resource, s32 context) {
    func_00314C10(resource->unit);
    return uiBlendColors(0x80808080, 0x80808000, blend);
}


void func_0029DA98(BrsActiveProgressList *output) {
    u32 *count = &output->count;
    s32 i;

    memset(output, 0, sizeof(*output));
    for (i = 0; i < 5; i++) {
        DatPartyRecord *unit = &datGameState->party[i];
        if ((unit->flags & 1) != 0) {
            if (unit->unitId == 9) {
                continue;
            }
            brsBuildUnitProgressRow(&output->rows[(*count)++], unit);
        }
    }
}

extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
extern u32 kwlnTaskGetUserValue(KwlnTask *task);
extern s8 D_0037F510[];
extern char D_00428550[];
extern void func_0029DF18(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029DFB0(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, BrsActiveProgressList *, s32, s32);
extern void func_0029E478(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029E820(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_002A05C0(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029EE80(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_002A08D8(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029F440(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029FA98(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029FBE0(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_002A0148(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern void func_0029E220(s32, s32, s32, BrsSkillPackageWork *, BrsProgressRow *, s32, s32);
extern s32 brsPollResultCounterCompletion(void);
extern void func_00341C78(u32 sound);

void mnuDrawUnitProgressRows(s32 x, s32 y, s32 z, s32 alpha, BrsActiveProgressList *list, s32 context) {
    KwlnTask *task;
    BrsSkillPackageWork *work = NULL;
    DatPartyRecord *unit;
    s32 i;
    s32 count;
    s32 offset = 0;
    s32 rowY;

    task = kwlnTaskGetTaskByName(D_00428550);
    if (task != NULL) {
        work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    }
    if ((D_0037F510[0x21] < 0 || (D_0037F510[0x23] & 2)) && work->opacity >= 0x80) {
        work->unkAEB9 = 1;
    }
    count = list->count;
    for (i = 0; i < count; i++) {
        if (work != NULL) {
            BrsProgressRow *row = &list->rows[i];

            rowY = y + offset;
            func_0029DF18(x, y, z, work, row, context, i);
            func_0029DFB0(x, y, z, work, row, list, context, i);
            func_0029E478(x, y, z, work, row, context, i);
            mnuBlendNeutralColorAlpha(x, y, z, alpha, row, context);
            func_0029E820(x, rowY, z, work, row, context, i);
            func_002A05C0(x, rowY, z, work, row, context, i);
            func_0029EE80(x, rowY, z, work, row, context, i);
            func_002A08D8(x, rowY, z, work, row, context, i);
            func_0029F440(x, rowY, z, work, row, context, i);
            func_0029FA98(x, rowY, z, work, row, context, i);
            func_0029FBE0(x, rowY, z, work, row, context, i);
            func_002A0148(x, rowY, z, work, row, context, i);
            func_0029E220(x, y, z, work, row, context, i);
            count = list->count;
        }
        unit = list->rows[i].unit;
        if (unit->flags & 2) {
            if (i + 1 >= count || (list->rows[i + 1].unit->flags & 2)) {
                y += 0x1F8;
            } else {
                y += 0x250;
                offset = -0x30;
            }
        } else {
            offset = -0x30;
            y += 0x1A0;
        }
    }
    if (brsPollResultCounterCompletion() != 0 && work != NULL && work->resultPhase == 1) {
        work->resultPhase = 2;
        func_00341C78(20);
    }
}

INCLUDE_RODATA(const s32, "game/code_0029CC90", D_00428550);

