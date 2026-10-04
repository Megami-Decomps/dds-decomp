#include "common.h"

extern u32 func_0029D790();
typedef struct PrfSkillList {
    u32 flags[8];
    s32 count;
    u16 skills[8];
} PrfSkillList;

extern u32 *ptyGetCurrentProfileRecord(s32);
extern u32 ptyGetProfileRecordCap(u16);
extern s32 func_00314990(s32, u16);
extern s32 prfBuildSkillListState0(s32, s32, s32);
extern void scrClearAllSecondaryScriptFlags(u8 *);
extern s32 scrSetFlag(u8 *, u16);
extern void scrSetSecondaryScriptFlag(u8 *, u16);

extern u8 *datGameState;

typedef struct {
    u8 pad0[6];
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
} TitleSeq;

typedef struct TitleMenuWork {
    u8 pad00[0x9C];
    TitleSeq **sequence;      /* 0x009C */
    u8 padA0[0xAE08];
    s8 opacityReady;          /* 0xAEA8 */
    u8 padAEA9[7];
    s32 iconResource;         /* 0xAEB0 */
    u32 opacity;              /* 0xAEB4 */
    u8 padAEB8[0x828];
    s32 fadeProgress;         /* 0xB6E0 */
    u8 padB6E4[0x10];
    s32 sequenceMode;         /* 0xB6F4 */
} TitleMenuWork;

extern s32 btlAddBaseStats(u8 *, TitleSeq *);

/* Base-stat block of the title sequence unit; the five stats sit at +0x16. */
typedef struct {
    u8 pad00[0x16];
    s8 baseStats[5];
} BrsStatUnit;

extern u32 uiBlendColors(u32, u32, s32);

void itfUpdateFadeColor(u8 *work);

extern struct { s32 v[6]; } D_003D6500;

extern void mnuCampDrawMenuIconLayer(s32, s32, s32, u32, u8 *, s32, u8 *);

extern void func_0029CB70(s32, s32, s32, u32, u8 *, s32, u8 *);

extern void func_0029C880(s32, s32, s32, u32, u8 *, s32, u8 *);

void mnuTitleDrawFadeMenuEntries(u8 *work) {
    u8 *res = work + 0x5C;
    u32 color = uiBlendColors(0xFFF06480, 0xFFF06400, 0x100 - ((TitleMenuWork *)work)->fadeProgress);

    mnuCampDrawMenuIconLayer(D_003D6500.v[0], D_003D6500.v[1], 0, color, res, 0x53, work);
    func_0029CB70(D_003D6500.v[2], D_003D6500.v[3], 0, color, res, 0x53, work);
    func_0029C880(D_003D6500.v[4], D_003D6500.v[5], 0, color, res, 0x53, work);
}

void mnuTitleRenderFadeAndPanels(u8 *work) {
    s32 remaining = 0x100 - ((TitleMenuWork *)work)->fadeProgress;

    itfUpdateFadeColor(work);
    func_0029C618(work);
    func_0029C120(work);
    func_0029C3F0(work);
    func_0029DB58(0x1D0, 0x3B8, 0, remaining, work + 0x408, 0x53);
    mnuTitleDrawFadeMenuEntries(work);
}

void brsDecaySharedAnimCounter(void) {
    func_0029C810();
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

/* Same party-unit reward header layout as DDS1, including AP status. */
typedef struct BrsExpUnit {
    u16 flags;
    u8 pad02[0xC];
    u16 apStatus;
} BrsExpUnit;

typedef struct BrsProfileApRecord {
    u8 pad00[0xA];
    s16 apMultiplier;
    u8 pad0C[0xD4];
} BrsProfileApRecord;

extern BrsProfileApRecord *D_00435E18;
extern s32 func_001514A8(void);
extern s16 func_001514B8(void);

s32 func_0029CF00(u8 *unit, s32 baseApTotal, s32 perUnitBonus) {
    s32 gain;
    s32 profileIndex;

    if (((BrsExpUnit *)unit)->apStatus & BRS_AP_BLOCKED_FLAG) {
        return 0;
    }
    gain = baseApTotal;
    gain += perUnitBonus;
    if ((((BrsExpUnit *)unit)->flags & BRS_ACTIVE_PARTY_FLAG) == 0) {
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

u32 func_0029D000(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029D008);

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyCalcLevelUps);

s32 mnuCountAdvancingTitleAnimations(void) {
    s32 offset = 0;
    s32 count = 0;
    s32 remaining = 4;
    do {
        s32 step = ptyCalcLevelUps(datGameState + 0xa60 + offset);
        count += step > 0;
        offset += 0x1c4;
    } while (--remaining >= 0);
    return count;
}

typedef struct BrsLevelUpRow {
    u32 unit;
    s32 levelUps;
    s32 partyIndex;
    u8 pad0C[0xC];
} BrsLevelUpRow;

typedef struct BrsLevelUpList {
    BrsLevelUpRow rows[5];
    s32 count;
} BrsLevelUpList;

typedef struct BrsProfileUnit {
    u16 flags;
    u8 pad02[2];
    u16 unitId;
    u8 pad06[0x4F];
    u8 profileId;
} BrsProfileUnit;

s32 func_0029D2D8(BrsLevelUpList *list) {
    s32 *rowData = &list->rows[0].levelUps;
    s32 offset = 0;
    s32 partyIndex = 0;
    BrsProfileUnit *unit;

    memset(list, 0, 0x7C);
    list->count = 0;
    while (partyIndex < 5) {
        unit = (BrsProfileUnit *)(datGameState + 0xA60 + offset);
        offset += 0x1C4;
        if ((unit->flags & 1) != 0) {
            s32 levelUps;

            if (unit->unitId == 9) {
                partyIndex++;
                continue;
            }

            levelUps = ptyCalcLevelUps((u8 *)unit);
            if (levelUps <= 0) {
                partyIndex++;
                continue;
            }

            {
                s32 rowIndex = list->count * 6;
                u32 *unitRow = &((u32 *)list)[rowIndex];
                s32 *levelRow = &rowData[rowIndex];

                *levelRow = levelUps;
                *unitRow = (u32)unit;
                rowData[list->count * 6 + 1] = partyIndex;
                list->count++;
            }
        }
        partyIndex++;
    }
    return list->count;
}

/* Build rows for active, capped profiles, excluding unit ID 9. */
s32 brsBuildProfileCapList(BrsLevelUpList *list) {
    PrfSkillList skills;
    s32 *rowData = &list->rows[0].levelUps;
    s32 offset = 0;
    s32 remaining = 4;
    BrsProfileUnit *unit;
    u32 *profile;

    memset(list, 0, 0x7C);
    list->count = 0;
    do {
        unit = (BrsProfileUnit *)(datGameState + 0xA60 + offset);
        offset += 0x1C4;
        if ((unit->flags & 1) != 0) {
            if (unit->unitId != 9) {
                profile = ptyGetCurrentProfileRecord((s32)unit);
                prfBuildSkillListState0((s32)unit, (s32)profile, (s32)&skills);
                if (unit->profileId != 0) {
                    if (ptyGetProfileRecordCap(unit->profileId) == *profile) {
                        if (func_00314990((s32)unit, unit->profileId) == 0) {
                            s32 rowIndex = list->count * 6;
                            u32 *unitRow = &((u32 *)list)[rowIndex];
                            s32 *skillRow = &rowData[rowIndex];

                            *skillRow = skills.count;
                            *unitRow = (u32)unit;
                            list->count++;
                        }
                    }
                }
            }
        }
    } while (--remaining >= 0);
    return list->count;
}

s32 mnuAdvanceTitleEntryAnimation(u8 *entry) {
    s32 step = ptyCalcLevelUps(entry);
    *(u16 *)(entry + 0x14) += step;
    ptyRecomputeMaxHpMp(entry);
    return step;
}

s32 btlAddBaseStats(u8 *src, TitleSeq *seq) {
    s32 i;

    for (i = 0; i < 5; i++) {
        s8 *stat = &((BrsStatUnit *)seq)->baseStats[i];

        *stat += src[i * 4];
        if (*stat >= 100) {
            *stat = 99;
        }
    }
    ptyRecomputeMaxHpMp((u8 *)seq);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029CC90", ptyAccumulateStatGains);

u32 func_0029D790(u8 *entry, PrfSkillList *out) {
    PrfSkillList result;
    u32 applied = 0;
    u32 *slot;

    memset(&result, 0, 0x34);
    slot = ptyGetCurrentProfileRecord((s32)entry);
    if (entry[0x55] != 0 && ptyGetProfileRecordCap(entry[0x55]) == *slot && func_00314990((s32)entry, entry[0x55]) == 0) {
        prfBuildSkillListState0((s32)entry, (s32)slot, (s32)&result);
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

extern s32 ptyCalcLevelUps(u8 *);

extern s32 ptyComputeTotalExp(u8 *, s32);

extern u32 ptyAddProfileRecordValueClamped(u8 *, u32);

extern s32 func_00314C10(s32);

extern u32 ptyGetProfileRecordCap(u16);

/* Match the progress-row layout in DDS1 game/code_002653A0.c. */
typedef struct BrsUnitExp {
    u8 pad00[0x10];
    s32 totalExp;           /* 0x10 */
} BrsUnitExp;

typedef struct BrsProgressRow {
    u8 pad00[8];
    u32 unit;               /* 0x08 */
    u32 levelProgress[4];   /* 0x0C */
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
    profilePoints = ptyAddProfileRecordValueClamped(entry, 0);
    mnuTitleInitFourParameters(((BrsProgressRow *)state)->profileProgress, 0x3C0, 0x50, profilePoints,
        ptyGetProfileRecordCap(func_00314C10((s32)entry) & 0xFFFF));
}

u32 mnuBlendNeutralColorAlpha(u32 a, u32 b, u32 c, s32 blend, u8 *resource) {
    func_00314C10(*(u32 *)(resource + 8));
    return uiBlendColors(0x80808080, 0x80808000, blend);
}

typedef struct BrsActiveProgressList {
    BrsProgressRow rows[5];
    u32 count;
} BrsActiveProgressList;

void func_0029DA98(BrsActiveProgressList *output) {
    u32 *count = &output->count;
    s32 offset = 0;
    s32 remaining = 4;

    memset(output, 0, sizeof(*output));
    do {
        BrsProfileUnit *unit = (BrsProfileUnit *)(datGameState + 0xA60 + offset);
        offset += 0x1C4;
        if ((unit->flags & 1) != 0) {
            if (unit->unitId == 9) {
                continue;
            }
            brsBuildUnitProgressRow((u8 *)output + (*count)++ * sizeof(BrsProgressRow), (u8 *)unit);
        }
    } while (--remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0029CC90", func_0029DB58);

INCLUDE_RODATA(const s32, "game/code_0029CC90", D_00428550);

