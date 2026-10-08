#include "common.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "btl_state.h"
#include "eff_transform.h"
#include "dat_state.h"
#include "dat_command.h"
#include "evt_unit.h"
#include "mdl.h"

#define BTL_AI_SLOT_COUNT 5

#define BTL_AI_WEIGHT_MASK 0xFFFF

#define BTL_HEALTH_RATE_SCALE 100

#define BTL_PARTY_QUERY_MASK 0x221

#define BTL_PARTY_ACTIVE_MASK 0x201

#define BTL_PARTY_ACTIVE_FLAGS 0x201

#define BTL_ENEMY_QUERY_MASK 0x421

#define BTL_ENEMY_ACTIVE_MASK 0x401

#define BTL_ENEMY_ACTIVE_FLAGS 0x401

#define BTL_REQUIRED_ACTION_CATEGORY_COUNT 10

#define BTL_ENTRY_CODE_SCAN_COUNT 10

#define BTL_AI_COUNTER_SATURATION 0x100

#define BTL_AI_COUNTER_MAX 0xFF

#define BTL_LOW_HP_ACTION_DELAY 0xF

#define BTL_LOW_HP_PERCENT_LIMIT 0x1E

#define BTL_LOW_HP_BUCKET_LIMIT 0x1E

#define BTL_LOW_HP_ENEMY_COUNT_LIMIT 2

extern s32 btlRollAiBucket(void);

extern u32 btlNextScaledRandom(u32);

extern u32 btlPreviousAiCandidateBucket;

extern s32 btlGetRuntime(void);

extern s32 btlIsSelectedActorStatusAndRecordClear();

extern s32 func_00212CB8(u32, u32, u32);

extern s32 btlHasAvailableOption(void);

extern s32 func_001ABB10(void);

extern s8 btlHistoryCounter;

extern void func_00211EA8();

extern s32 btlActorEntryIsExpired();

extern s32 btlGetActorEntryCode();

extern s32 D_003BF660[];

extern u32 btlPickWeightedAiSlot();

extern s32 fldGetSelectedUnitStat();

typedef struct BattleSub {
    union {
        s32 task;
        f32 scale;
        struct {
            u8 pad0[2];
            s8 active;
        } b;
    };
    union {
        s32 targetMode;
        BtlUnit *targetActor;
    };
    s8 b8;
    u8 pad09;
    s8 alternateFormation;
    u8 pad0B;
    s8 controlEnabled;
} BattleSub;

typedef struct BattleWork {
    u8 pad0[0x1E4];
    s16 scriptGroup; /* 0x1E4: selects the script resource path */
    u8 pad1E6[0x22];
    s32 soundTaskBase; /* 0x208: offset +6 selects the stationed sound task */
    s32 resourceHandle; /* 0x20C */
    u8 pad210[8];
    u32 resourceFlags; /* 0x218 */
    u8 pad21C[0x10];
    s32 state22C; /* A value of 5 blocks the world-effect helpers. */
    u8 pad230[0x18];
    struct ActionStateLink *actionActors; /* 0x248: linked action handles */
    BtlUnit *actorList;
    u8 pad250[0x18];
    u16 unk268;
    u8 pad26A[2];
    u16 phase;
    u8 pad26E[2];
    u16 phaseControl; /* 0x270: required phase in func_00218150 */
    u8 pad272[2];
    s32 turnCount;
    u8 pad278[4];
    u8 encounterMode; /* 0x27C */
    u8 pad27D[0x23];
    s32 mode;
    u8 pad2A4[0x474];
    struct BattleSub *sub; /* 0x718: reused as BtlSelectCtrl in unit-selection modes. */
} BattleWork;

/* Native 0x10-byte AI selection scratch. Its producer retains the command
 * actor and species/mode; the conditional-action dispatcher uses +8 as a
 * table row and +0xC as its query selector. This is not the singleton battle work. */
typedef struct BtlAiScratchWork {
    ActionStateLink *actor;
    s32 speciesId;
    s32 rowIndex;
    s32 conditionKind;
} BtlAiScratchWork;

extern BtlAiScratchWork *btlActionScratchWork;

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

/* AICALC.TBL: each decision tier tests three packed predicates, then
 * selects a route in most-specific-first order; route 8 makes no selection. */
typedef struct AiDecisionRow {
    u32 predicates[3];
    u8 routes[8];
} AiDecisionRow;

typedef struct AiSpecies {
    u8 pad00[4];
    AiDecisionRow decisions[3];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *datEnemyAiRecords;

extern char D_00419A88[];

extern void btlDebugPrintf(const char *, ...);

extern u16 btlReadUnitStatusMask(DatPartyRecord *);

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(void *);

extern s32 btlMatchesActorEntryCodeCondition();

extern s32 mdlFlagTest(s32);

extern s32 btlReadCurrentUnitHp(DatPartyRecord *);

extern s32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *);

extern u16 btlReadCurrentUnitMp(DatPartyRecord *);

extern s32 btlComputeSkillAdjustedMaxMp(DatPartyRecord *);

extern s32 btlAreUnitStatusAndEntryFlagsClear();

extern void func_001AC0F8(s32, BtlIndexList *, s32, s32, s32);

extern s32 btlUnitHasNegativeActionQueryResult(void *, s32);

extern s8 D_00419C30[];

extern s8 D_00419C58[];

extern s32 btlWouldUiValueFallBelowQuarter(s32, s32);

extern u8 sdfPfsDebugMode;

extern s32 btlAnyUnitHasActionInSlots();

extern s32 func_001B3200(s32);

extern s32 func_00213F58(s32, s16, s8);

extern s32 btlUnitBlocksElementQuery(BtlUnit *, s32, s32);

extern s32 btlUnitBlocksElementQueryForGroup(BtlUnit *, s32, u32);

extern s32 btlTestSelectedItemCategoryMask(BtlUnit *, s32);

extern s32 btlElementToBitIndex(s32, s32);

extern s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *, u32);

extern s32 func_001B2F50(void *, s32);

extern s32 btlHasAdjacentActorRecordStatus(void);

extern s32 btlIsActorHighStateFlagClear(s32 item);

extern char D_00419B38[];

extern char D_00419B68[];

extern char D_00419B88[];

extern s32 func_001ABF50(BtlUnit *, s32);

extern u32 func_001B39E8(s32);

INCLUDE_ASM(const s32, "game/code_00211360", func_00211360);

u32 func_002115B0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00211360", func_002115B8);

extern u32 func_002115B8(u32 route);

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

s32 func_00211658(s32 context, s32 species, u32 *selected, u32 requestedRow) {
    s8 matches[3][3];
    u32 predicates[3];
    u32 firstTier, endTier;
    u32 tier, predicateIndex;
    if (requestedRow == 0) {
        firstTier = 0;
        endTier = 3;
    } else {
        firstTier = requestedRow - 1;
        endTier = requestedRow;
    }
    for (tier = 0; tier < 3; tier++) {
        for (predicateIndex = 0; predicateIndex < 3; predicateIndex++) {
            matches[tier][predicateIndex] = 0;
        }
    }
    for (tier = firstTier; tier < endTier; tier++) {
        predicates[0] = datEnemyAiRecords[species].decisions[tier].predicates[0];
        predicates[1] = datEnemyAiRecords[species].decisions[tier].predicates[1];
        predicates[2] = datEnemyAiRecords[species].decisions[tier].predicates[2];
        for (predicateIndex = 0; predicateIndex < 3; predicateIndex++) {
            matches[tier][predicateIndex] = btlDispatchPackedEffectAction(context, predicates[predicateIndex]);
        }
    }
    for (tier = 0; tier < 3; tier++) {
        if (matches[tier][0] && matches[tier][1] && matches[tier][2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[0] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[0];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 0;
                return 1;
            }
        }
        if (matches[tier][0] && matches[tier][1]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[1] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[1];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 1;
                return 1;
            }
        }
        if (matches[tier][0] && matches[tier][2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[2] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[2];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 2;
                return 1;
            }
        }
        if (matches[tier][1] && matches[tier][2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[3] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[3];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 3;
                return 1;
            }
        }
        if (matches[tier][0]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[4] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[4];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 4;
                return 1;
            }
        }
        if (matches[tier][1]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[5] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[5];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 5;
                return 1;
            }
        }
        if (matches[tier][2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[6] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[6];
                *selected = func_002115B8(*selected);
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 6;
                return 1;
            }
        }
        if (datEnemyAiRecords[species].decisions[tier].routes[7] != 8) {
            *selected = datEnemyAiRecords[species].decisions[tier].routes[7];
            *selected = func_002115B8(*selected);
            btlActionScratchWork->rowIndex = tier;
            btlActionScratchWork->conditionKind = 7;
            return 1;
        }
    }
    *selected = 0;
    btlActionScratchWork->rowIndex = 0;
    btlActionScratchWork->conditionKind = 8;
    return 1;
}

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

s32 func_002119E0(s32 context, s32 species, u32 *selected, u32 requestedRow) {
    s8 matches[3];
    u32 predicates[3];
    u32 firstTier, endTier;
    u32 tier, predicateIndex;
    if (requestedRow == 0) {
        firstTier = 0;
        endTier = 3;
    } else {
        firstTier = requestedRow - 1;
        endTier = requestedRow;
    }
    for (tier = firstTier; tier < endTier; tier++) {
        predicates[0] = datEnemyAiRecords[species].decisions[tier].predicates[0];
        predicates[1] = datEnemyAiRecords[species].decisions[tier].predicates[1];
        predicates[2] = datEnemyAiRecords[species].decisions[tier].predicates[2];
        for (predicateIndex = 0; predicateIndex < 3; predicateIndex++) {
            matches[predicateIndex] = btlDispatchPackedEffectAction(context, predicates[predicateIndex]);
        }
        if (matches[0] && matches[1] && matches[2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[0] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[0];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 0;
                return 1;
            }
        }
        if (matches[0] && matches[1]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[1] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[1];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 1;
                return 1;
            }
        }
        if (matches[0] && matches[2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[2] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[2];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 2;
                return 1;
            }
        }
        if (matches[1] && matches[2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[3] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[3];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 3;
                return 1;
            }
        }
        if (matches[0]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[4] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[4];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 4;
                return 1;
            }
        }
        if (matches[1]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[5] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[5];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 5;
                return 1;
            }
        }
        if (matches[2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[6] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[6];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 6;
                return 1;
            }
        }
        if (datEnemyAiRecords[species].decisions[tier].routes[7] != 8) {
            *selected = datEnemyAiRecords[species].decisions[tier].routes[7];
            btlActionScratchWork->rowIndex = tier;
            btlActionScratchWork->conditionKind = 7;
            return 1;
        }
    }
    *selected = 0;
    btlActionScratchWork->rowIndex = 0;
    btlActionScratchWork->conditionKind = 8;
    return 1;
}

/* Pick the first nonzero-weight slot whose 16-bit cumulative weight reaches the roll.
 * The unit argument is unused; preserve the runtime lookup, debug failure path and zero fallback. */
u32 btlPickWeightedAiSlot(s32 unusedUnit, s32 speciesId, s32 rowIndex) {
    u32 weightRoll;
    u32 cumulativeWeight;
    u32 slotIndex;

    btlGetRuntime();
    weightRoll = btlRollAiBucket();
    cumulativeWeight = 0;
    for (slotIndex = 0; slotIndex < BTL_AI_SLOT_COUNT; slotIndex++) {
        u32 weight = datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].weight;

        cumulativeWeight = (cumulativeWeight + weight) & BTL_AI_WEIGHT_MASK;
        if (cumulativeWeight >= weightRoll && weight != 0) {
            return slotIndex;
        }
    }
    btlDebugPrintf("AI_BUGBUGBUGBUGBUG           \n");
    if (sdfPfsDebugMode == 0) {
        btlBossDebugPrintf(D_00419A88);
    }
    return 0;
}

s32 btlRollAiBucket(void) {
    u32 roll = ((btlNextScaledRandom(0x1000) + btlNextScaledRandom(0x1000)) & 0xFFF) / 41;
    u32 reroll;

    if (btlPreviousAiCandidateBucket >= roll - 3 && btlPreviousAiCandidateBucket <= roll + 3) {
        reroll = (btlNextScaledRandom(0x1000) + btlNextScaledRandom(0x1000)) & 0xFFF;
        btlPreviousAiCandidateBucket = roll;
        return reroll / 41;
    }
    btlPreviousAiCandidateBucket = roll;
    return roll;
}

INCLUDE_ASM(const s32, "game/code_00211360", func_00211EA8);

INCLUDE_RODATA(const s32, "game/code_00211360", D_00419A88);

INCLUDE_ASM(const s32, "game/code_00211360", func_00211F38);

/* Compare HP against a signed percentage using the native products and unsigned comparison. */
s32 btlIsUnitAtOrBelowHealthRate(BtlUnit *unit, s32 healthPercent) {
    DatPartyRecord *statAddress = &unit->partyRecord;
    s32 currentHp = btlReadCurrentUnitHp(statAddress);
    s32 maximumHp = btlComputeSkillAdjustedMaxHp(statAddress);
    if ((u32)(maximumHp * healthPercent) < (u32)(currentHp * BTL_HEALTH_RATE_SCALE)) {
        return 0;
    }
    return 1;
}

/* Return whether an active enemy-side unit passes the HP percentage check; bit 0x20 is not filtered here. */
s32 btlHasBossAtOrBelowHealthRate(s32 unused, s32 healthPercent) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_ACTIVE_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            btlIsUnitAtOrBelowHealthRate(unitCursor, healthPercent)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasUnitAtOrBelowHealthRate(s32 unused, s32 multiplier) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            DatPartyRecord *stats = &unit->partyRecord;
            s32 current = btlReadCurrentUnitHp(stats);
            s32 maximum = btlComputeSkillAdjustedMaxHp(stats);
            if ((u32)(maximum * multiplier) >= (u32)(current * 100)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlHasUnitAtOrAboveHealthRate(s32 unused, s32 multiplier) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            DatPartyRecord *stats = &unit->partyRecord;
            s32 current = btlReadCurrentUnitHp(stats);
            s32 maximum = btlComputeSkillAdjustedMaxHp(stats);
            if ((u32)(current * 100) >= (u32)(maximum * multiplier)) {
                return 1;
            }
        }
    }
    return 0;
}

/* Reset the scratch-context counter only when its query succeeds; retain the native no-argument query. */
s32 btlResetAiCounterAtLimit() {
    if (btlAiCounterReachedLimit()) {
        btlActionScratchWork->actor->aiCounter = 0;
        return 1;
    }
    return 0;
}

/* Check an inclusive upper bound on active enemy-side units with bit 0x20 clear. */
s32 btlIsGroup400CountAtMost(s32 unused, u32 maximumCount) {
    u32 matchingCount = 0;
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            matchingCount++;
        }
    }
    if (maximumCount < matchingCount) {
        return 0;
    }
    return 1;
}

/* Check an inclusive upper bound on eligible party units, additionally excluding condition bit 0x800. */
s32 btlIsGroup200EligibleCountAtMost(s32 unused, u32 maximumCount) {
    u32 matchingCount = 0;
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (!(unitCursor->partyRecord.status & 0x800)) {
                matchingCount++;
            }
        }
    }
    if (maximumCount < matchingCount) {
        return 0;
    }
    return 1;
}

u8 func_00212620(void) {
    s64 result;

    result = btlIsSelectedActorStatusAndRecordClear();
    return result != 0;
}

s32 btlValidateItemStillAvailable(s32 item) {
    if (btlHasAdjacentActorRecordStatus()) {
        if (btlIsActorHighStateFlagClear(item)) {
            return 1;
        }
        btlDebugPrintf("                  ::Item is already thrown away");
        btlBossDebugPrintf(D_00419B38);
    } else {
        btlDebugPrintf(D_00419B68);
        btlBossDebugPrintf(D_00419B88);
    }
    return 0;
}

s32 btlIsModelGateActiveForEligibleUnit(BtlUnit *unit) {
    if (unit->stateFlags & 0x10000) {
        return 0;
    }
    if (unit->flags & 0x200) {
        if (datGameState->header.currency < 2) {
            return 0;
        }
    }
    return mdlFlagTest(0x290) != 0;
}

/* Check an inclusive upper bound on active party-side units with bit 0x20 clear. */
s32 btlIsGroup200CountAtMost(s32 unused, u32 maximumCount) {
    u32 matchingCount = 0;
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            matchingCount++;
        }
    }
    if (maximumCount < matchingCount) {
        return 0;
    }
    return 1;
}

/* Return whether the native unit-status query intersects any requested action-mask bit. */
s32 btlUnitHasAnyStatusInMask(BtlUnit *unit, s32 actionMask) {
    return (btlReadUnitStatusMask(&unit->partyRecord) & actionMask) != 0;
}

/* Test active enemy-side units for any requested action bit; do not filter bit 0x20. */
s32 btlAnyGroup400HasActionMask(s32 unused, s32 actionMask) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_ACTIVE_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            btlUnitHasAnyStatusInMask(unitCursor, actionMask)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasActorOrSlotMatchingActionQuery(s32 unused, u32 query) {
    BtlUnit *unit;
    DatPartyRecord *record;
    s32 i;

    for (unit = ((BattleWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->partyRecord.unitId == ((query >> 16) & 0x3F)) {
            if (unit->flags & 1) {
                if (btlUnitHasAnyStatusInMask(unit, query & 0xFFFF)) {
                    return 1;
                }
            }
        }
    }
    record = datGameState->party;
    for (i = 0; i < 5; i++, record++) {
        if (record->flags & 1) {
            if (!(record->flags & 2)) {
                if (record->unitId == ((query >> 16) & 0x3F)) {
                    if ((query & 0xFFFF) == 0x7FFF) {
                        return ((btlReadUnitStatusMask(record) & query) & 0xFFFF) != 0;
                    }
                    if ((record->status & 0x7FFF & query) != 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/* Test party-side units for any requested action bit; DDS2 deliberately does not filter bit 0x20 here. */
s32 btlAnyGroup200HasActionMask(s32 unused, s32 actionMask) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_ACTIVE_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            btlUnitHasAnyStatusInMask(unitCursor, actionMask)) {
            return 1;
        }
    }
    return 0;
}

/* Require every eligible party unit to intersect the requested mask; an empty selection succeeds. */
s32 btlAllGroup200HaveActionMask(s32 unused, s32 actionMask) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            !btlUnitHasAnyStatusInMask(unitCursor, actionMask)) {
            return 0;
        }
    }
    return 1;
}

/* Find an eligible party-side unit with the requested mode. */
s32 btlHasGroup200UnitMode(s32 unused, s32 unitMode) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            unitCursor->partyRecord.unitId == unitMode) {
            return 1;
        }
    }
    return 0;
}

/* Find an eligible enemy-side unit of the requested mode with a different native owner ID. */
s32 btlHasOtherGroup400UnitMode(s32 excludedUnit, s32 unitMode) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            unitCursor->partyRecord.unitId == unitMode &&
            unitCursor->owner != ((BtlUnit *)excludedUnit)->owner) {
            return 1;
        }
    }
    return 0;
}

/* Kinds 0xB/0xC scan the even/odd entry codes; other kinds test their own
   entry. Every failure falls out to the single trailing `return 0`. K&R:
   callers pass one argument or none. */
s32 btlMatchesActorEntryCodeCondition(actorAddress, conditionIndex)
    s32 actorAddress;
    s32 conditionIndex;
{
    s32 evenIndex;
    s32 oddIndex;

    if (conditionIndex == 0xB) {
        for (evenIndex = 0; evenIndex < BTL_ENTRY_CODE_SCAN_COUNT; evenIndex += 2) {
            if (btlActorEntryIsExpired(actorAddress, D_003BF660[evenIndex]) != 0 && btlGetActorEntryCode(actorAddress, D_003BF660[evenIndex]) > 0) {
                return 1;
            }
        }
    } else if (conditionIndex == 0xC) {
        for (oddIndex = 1; oddIndex < BTL_ENTRY_CODE_SCAN_COUNT; oddIndex += 2) {
            if (btlActorEntryIsExpired(actorAddress, D_003BF660[oddIndex]) != 0 && btlGetActorEntryCode(actorAddress, D_003BF660[oddIndex]) <= 0) {
                return 1;
            }
        }
    } else if (btlActorEntryIsExpired(actorAddress, D_003BF660[conditionIndex]) != 0 || conditionIndex == 0xA || conditionIndex == 0xD) {
        if ((conditionIndex & 1) == 0 || conditionIndex == 0xD) {
            if (btlGetActorEntryCode(actorAddress, D_003BF660[conditionIndex]) > 0) {
                return 1;
            }
        } else {
            if (btlGetActorEntryCode(actorAddress, D_003BF660[conditionIndex]) <= 0) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00211360", func_00212CB8);

extern const s32 btlRequiredActionCategories[10];

/* Require all ten category queries to succeed; retain the complete local copy of the native category table. */
s32 btlUnitHasAllTenActions(void *actor) {
    s32 requiredCategories[BTL_REQUIRED_ACTION_CATEGORY_COUNT];
    s32 categoryIndex;
    memcpy(requiredCategories, btlRequiredActionCategories, sizeof(requiredCategories));
    for (categoryIndex = 0; categoryIndex < BTL_REQUIRED_ACTION_CATEGORY_COUNT; categoryIndex++) {
        if (btlTestSelectedItemCategoryMask(actor, requiredCategories[categoryIndex]) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Return whether any eligible party unit meets the requested entry-code condition. */
s32 btlAnyPartyMeetsEntryCodeCondition(s32 unused, s32 conditionIndex) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if (((*(u64 *)&unitCursor->flags) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            btlMatchesActorEntryCodeCondition(unitCursor, conditionIndex)) {
            return 1;
        }
    }
    return 0;
}

/* Return whether any eligible enemy unit meets the requested entry-code condition. */
s32 btlAnyEnemyMeetsEntryCodeCondition(s32 unused, s32 conditionIndex) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            btlMatchesActorEntryCodeCondition(unitCursor, conditionIndex)) {
            return 1;
        }
    }
    return 0;
}

/* Normalize the mode-zero action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeZero(u32 unitAddress, u32 actionQuery) {
    s64 queryResult;

    queryResult = func_00212CB8(unitAddress, actionQuery, 0);
    return queryResult != 0;
}

/* Normalize the mode-one action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeOne(u32 unitAddress, u32 actionQuery) {
    s64 queryResult;

    queryResult = func_00212CB8(unitAddress, actionQuery, 1);
    return queryResult != 0;
}

/* Return whether an eligible party unit has a nonzero mode-zero query result. */
s32 btlAnyPartyPassesEntryCheck(s32 unused, s32 actionQuery) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            func_00212CB8((s32)unitCursor, actionQuery, 0)) {
            return 1;
        }
    }
    return 0;
}

/* Return whether an eligible party unit has a nonzero mode-one query result. */
s32 btlAnyPartyPassesInverseEntryCheck(s32 unused, s32 actionQuery) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            func_00212CB8((s32)unitCursor, actionQuery, 1)) {
            return 1;
        }
    }
    return 0;
}

/* Return whether an eligible enemy unit has a nonzero mode-zero query result. */
s32 btlAnyEnemyPassesEntryCheck(s32 unused, s32 actionQuery) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            func_00212CB8((s32)unitCursor, actionQuery, 0)) {
            return 1;
        }
    }
    return 0;
}

/* Return whether an eligible enemy unit has a nonzero mode-one query result. */
s32 btlAnyEnemyPassesInverseEntryCheck(s32 unused, s32 actionQuery) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            func_00212CB8((s32)unitCursor, actionQuery, 1)) {
            return 1;
        }
    }
    return 0;
}

/* Return whether an eligible party unit has a zero mode-zero query result. */
s32 btlAnyPartyFailsEntryCheck(s32 unused, s32 actionQuery) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            func_00212CB8((s32)unitCursor, actionQuery, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Return whether an eligible enemy unit has a zero mode-zero query result. */
s32 btlAnyEnemyFailsEntryCheck(s32 unused, s32 actionQuery) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS &&
            func_00212CB8((s32)unitCursor, actionQuery, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Look for an eligible party unit with native flag 0x1000 clear; its meaning is not established here. */
s32 btlAnyGroup200LacksFlag1000(void) {
    BtlUnit *unitCursor = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unitCursor != 0; unitCursor = unitCursor->nextActor) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS &&
            !(unitCursor->flags & 0x1000)) {
            return 1;
        }
    }
    return 0;
}

/* Normalize entry-code condition 10 to a byte boolean without changing the native short-arity calls. */
u8 btlUnitPassesActionTenCheck(u32 unitAddress) {
    s64 queryResult;

    queryResult = btlMatchesActorEntryCodeCondition(unitAddress, 10);
    return queryResult != 0;
}

s32 func_00213418(void) {
    return btlMatchesActorEntryCodeCondition() != 0;
}

s32 func_00213438(BtlUnit *unit) {
    u32 flags;

    if (unit->flags & 0x200) {
        return 0;
    }
    flags = unit->partyRecord.flags & 0x2000;
    return flags != 0;
}

/* Test scratch-context bit 1; no gameplay meaning for this flag is established here. */
s32 btlHasContextFlagTwo(void) {
    /* Preserve the AI query's signed-word interpretation of this unsigned mask. */
    return (((s32)btlActionScratchWork->actor->flags & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_00211360", btlCheckCounterLimit);

/* Return whether counter query 4 has reached the requested inclusive lower bound. */
s32 btlCounterReachedLimit(s32 unused, u32 minimumCount) {
    if (func_001B39E8(4) < minimumCount) {
        return 0;
    }
    return 1;
}

/* Increment the stored counter, retain native wrap/clamp behavior, then reread it for the limit test. */
s32 btlAiCounterReachedLimit(s32 unused, u32 minimumCount) {
    ActionStateLink *counterState = btlActionScratchWork->actor;

    counterState->aiCounter = counterState->aiCounter + 1;
    counterState->aiCounter = counterState->aiCounter == 0 ? 0 : counterState->aiCounter >= BTL_AI_COUNTER_SATURATION ? BTL_AI_COUNTER_MAX : counterState->aiCounter;
    if (btlActionScratchWork->actor->aiCounter < minimumCount) {
        return 0;
    }
    return 1;
}

/* Compare the battle turn count with an inclusive lower bound. */
s32 btlTurnReachedLimit(s32 unused, u32 minimumTurns) {
    if (((BattleWork *)btlGetRuntime())->turnCount < minimumTurns) {
        return 0;
    }
    return 1;
}

s32 btlTurnCountAtMost(s32 unused, u32 limit) {
    if (limit < (u32)((BattleWork *)btlGetRuntime())->turnCount) {
        return 0;
    }
    return 1;
}

/* Return true only in phase 2 with no counted turns. */
s32 btlIsReadyWithoutTurns(void) {
    BattleWork *battleState = (BattleWork *)btlGetRuntime();
    if (battleState->phase == 2) {
        if (battleState->turnCount == 0) {
            return 1;
        }
    }
    return 0;
}

/* Compare a unit stat with a percentage of its maximum.
 * The stat's identity is not established by these two accessors. */
s32 btlIsUnitStatAtOrBelowRate(BtlUnit *unit, s32 percentage) {
    DatPartyRecord *statAddress = &unit->partyRecord;
    u32 currentValue = btlReadCurrentUnitMp(statAddress);
    u32 scaledMaximum = btlComputeSkillAdjustedMaxMp(statAddress) * percentage;
    if (scaledMaximum < currentValue * BTL_HEALTH_RATE_SCALE) {
        return 0;
    }
    return 1;
}

s32 btlUnitStatAtOrAboveRate(BtlUnit *unit, u32 percentage) {
    DatPartyRecord *stats = &unit->partyRecord;
    u32 current = btlReadCurrentUnitMp(stats);
    u32 maximum = btlComputeSkillAdjustedMaxMp(stats);
    if (current * 100 < maximum * percentage) {
        return 0;
    }
    return 1;
}

s32 btlUnitStatAtMost(BtlUnit *unit, u32 limit) {
    DatPartyRecord *stats = &unit->partyRecord;
    u32 current = btlReadCurrentUnitMp(stats);
    btlComputeSkillAdjustedMaxMp(stats);
    if (limit < current) {
        return 0;
    }
    return 1;
}

s32 btlUnitStatAtLeast(BtlUnit *unit, u32 limit) {
    DatPartyRecord *stats = &unit->partyRecord;
    u32 current = btlReadCurrentUnitMp(stats);
    btlComputeSkillAdjustedMaxMp(stats);
    if (current < limit) {
        return 0;
    }
    return 1;
}

u8 func_00213728(void) {
    s64 result;

    result = btlHasAvailableOption();
    return result != 0;
}

/* Query the context's index list, freeing it on both the first success and exhausted-list paths. */
s32 btlAnyIndexedUnitPassesQuery(BtlUnit *unit) {
    u32 entryIndex;
    u32 entryCount;
    s32 contextAddress;
    BtlIndexList *indexList;
    if (unit->flags & 0x400) {
        return 0;
    }
    contextAddress = (s32)btlActionScratchWork->actor;
    indexList = btlAllocateIndexList(13);
    func_001AC0F8(contextAddress, indexList, 2, 0, 0);
    entryCount = btlGetIndexListCount(indexList);
    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        if (btlAreUnitStatusAndEntryFlagsClear(btlGetIndexListEntry(indexList, entryIndex)) != 0) {
            btlFreeIndexList(indexList);
            return 1;
        }
    }
    btlFreeIndexList(indexList);
    return 0;
}

/* Gate the low-HP action on hold, delay, HP ratio, enemy count, bucket roll, and selected-actor status.
 * Preserve native unsigned timing and division behavior, including the absence of a max-HP guard. */
s32 btlIsLowHpActionReady(BtlUnit *unit) {
    s32 conditionMet = 0;
    s16 bucketRoll;
    ActionStateLink *context = btlActionScratchWork->actor;
    u16 actionTime = unit->partyRecord.level;
    s32 delayPending = func_001B39E8(4) < (u32)(actionTime + BTL_LOW_HP_ACTION_DELAY);

    if (context->lowHpActionHold <= 0) {
        if (delayPending == 0 && unit->partyRecord.hp * BTL_HEALTH_RATE_SCALE / unit->partyRecord.maxHp < BTL_LOW_HP_PERCENT_LIMIT && btlIsGroup400CountAtMost(unit, BTL_LOW_HP_ENEMY_COUNT_LIMIT) != 0) {
            bucketRoll = btlRollAiBucket();
            conditionMet = bucketRoll < BTL_LOW_HP_BUCKET_LIMIT;
        }
    }
    if (conditionMet == 1 && btlIsSelectedActorStatusAndRecordClear(unit) != 0) {
        return 1;
    }
    return 0;
}

/* Test native unit flag 0x1000 without assigning it an unverified gameplay meaning. */
s32 btlUnitHasFlag1000(s32 unitAddress) {
    return (((s32)((BtlUnit *)unitAddress)->flags & 0x1000) > 0);
}

u8 btlIsCommandAvailable(void) {
    s64 result;

    result = func_001ABB10();
    return result == 0;
}

u8 func_00213930(void) {
    s64 result;

    result = btlAreUnitStatusAndEntryFlagsClear();
    return result != 0;
}

extern const s32 btlElementMasks[20];

s32 btlElementToBitIndex(s32 mask, s32 index) {
    s32 table[20];
    memcpy(table, btlElementMasks, sizeof(table));
    if ((mask & table[index]) == 0) {
        return 0x80;
    }
    if (index != 0) {
        return index - 1;
    }
    return -1;
}

s32 btlUnitHasNegativeActionQueryResult(void *unit, s32 mask) {
    s32 i;
    s32 value;
    if (mask & 0x100000) {
        for (i = 0; i < 0x13; i++) {
            value = btlElementToBitIndex(mask, i);
            if (value == 0x80) {
                continue;
            }
            value = func_001ABF50(unit, value);
            if ((u16)value < 100) {
                continue;
            }
            if ((u32)value & 0x80000000) {
                return 1;
            }
        }
        return 0;
    }
    value = func_001ABF50(unit, mask);
    if ((u16)value >= 100) {
        if (value < 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyEnemyHasNegativeActionResult(s32 unused, s32 mask) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x401) == 0x401 &&
            btlUnitHasNegativeActionQueryResult(battler, mask)) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyPartyUnitHasNegativeActionResult(s32 unused, s32 mask) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x201) == 0x201 &&
            btlUnitHasNegativeActionQueryResult(battler, mask)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasGroup200DifferentUnitMode(s32 unused, s32 mode) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            battler->partyRecord.unitId != mode) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasDistinctTargetSelection(s32 actor, s32 selection) {
    BtlUnit *battler;
    s32 resolvedSelection;
    if (selection != 0) {
        resolvedSelection = selection;
    } else {
        resolvedSelection = ((BtlUnit *)actor)->partyRecord.unitId;
    }
    battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            battler->partyRecord.unitId != resolvedSelection &&
            battler->owner != ((BtlUnit *)actor)->owner) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyUnitPassesCheck200(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup(battler, action, 0x200) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanQueryElementAgainstParty(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup(battler, action, 0x200) == 1) {
            return 0;
        }
    }
    return 1;
}

s32 btlAnyUnitPassesCheck400(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup(battler, action, 0x400) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanQueryElementAgainstEnemies(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup(battler, action, 0x400) == 1) {
            return 0;
        }
    }
    return 1;
}

s32 btlCheckActorEligibilityWithDebug(s32 actor) {
    if (btlWouldUiValueFallBelowQuarter(actor, 0) != 0) {
        if (sdfPfsDebugMode == 0) {
            btlBossDebugPrintf(D_00419C30);
        }
        return 1;
    }
    if (sdfPfsDebugMode == 0) {
        btlBossDebugPrintf(D_00419C58);
    }
    return 0;
}

s32 func_00213F58(s32 mask, s16 actionId, s8 force) {
    s32 i;
    s32 value;

    if (mask & 0x100000) {
        for (i = 0; i < 0x13; i++) {
            value = btlElementToBitIndex(mask, i);
            if (value != 0x80) {
                if (datCommandSelectors[actionId].stat == value) {
                    if (force != 0) {
                        return 1;
                    }
                    if ((u8)(datCommandRecords[actionId].options - 1) < 2) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    if (datCommandSelectors[actionId].stat == mask) {
        if (force != 0) {
            return 1;
        }
        if ((u8)(datCommandRecords[actionId].options - 1) < 2) {
            return 1;
        }
    }
    return 0;
}

/* Return whether an eligible unit has a queued action in the requested category. */
s32 btlAnyUnitHasActionInSlots(mask, action)
    s32 mask;
    s32 action;
{
    ActionStateLink *actor;
    BtlUnit *unit;
    u32 flags;
    s32 i;
    for (actor = ((BattleWork *)btlGetRuntime())->actionActors; actor != 0; actor = actor->next) {
        unit = actor->unit;
        if (unit == 0) {
            continue;
        }
        flags = unit->flags;
        if (!(flags & 1)) {
            continue;
        }
        if (!(flags & mask)) {
            continue;
        }
        if (flags & 0x20) {
            continue;
        }
        for (i = 0; i < 8; i++) {
            if (func_00213F58(action, actor->actions[i].actionId, 1) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00211360", btlAnyGroup200HasAction);

INCLUDE_ASM(const s32, "game/code_00211360", btlAnyGroup400HasAction);

s32 func_002141A8(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            func_00213F58(action, battler->partyRecord.actionSlot, 0)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00214230(s32 unused, s32 battler) {
    s32 node = (s32)((BattleWork *)btlGetRuntime())->actionActors;
    for (; node != 0; node = (s32)((ActionStateLink *)node)->next) {
        BtlUnit *target = ((ActionStateLink *)node)->unit;
        if (target != 0 &&
            (btlUnitStatusPair(target) & 0x221) == 0x201 &&
            func_00213F58(battler, ((ActionStateLink *)node)->actions[0].actionId, 0)) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyPartyUnitHasFullActionSet(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            btlUnitHasAllTenActions((s32)battler)) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyEnemyHasFullActionSet(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            btlUnitHasAllTenActions((s32)battler)) {
            return 1;
        }
    }
    return 0;
}

/* Scan active group-0x200 actors for a queued action whose table entry has
 * nonzero byte 8 and class byte 9 equal to 2. */
s32 btlHasEligibleQueuedSpecialAction(void) {
    u8 *actor;
    BtlUnit *unit;
    DatCommandRecord *actionEntry;
    s16 actionId;
    s32 i;
    for (actor = (u8 *)((BattleWork *)btlGetRuntime())->actionActors; actor != 0; actor = (u8 *)((ActionStateLink *)actor)->next) {
        unit = ((ActionStateLink *)actor)->unit;
        if (unit == 0) {
            continue;
        }
        if ((btlUnitStatusPair(unit) & 0x221) != 0x201) {
            continue;
        }
        for (i = 0; i < 8; i++) {
            actionId = ((ActionStateLink *)actor)->actions[i].actionId;
            if (actionId == 0) {
                continue;
            }
            if ((u32)((u8)datCommandSelectors[actionId].stat - 0x10) < 2U) {
                continue;
            }
            actionEntry = (DatCommandRecord *)(actionId * 0x38 + (s32)datCommandRecords);
            if (actionEntry->targetType == 0) {
                continue;
            }
            if (actionEntry->options != 2) {
                continue;
            }
            return 1;
        }
    }
    return 0;
}

/* Test one active battler, or any active enemy, for the extended status flag. */
s32 btlHasUnitWithStatusBit(BtlUnit *unit, s32 scanEnemies) {
    if (scanEnemies != 0) {
        BtlUnit *cursor = ((BtlState *)btlGetRuntime())->units;
        while (cursor != NULL) {
            if ((btlUnitStatusPair(cursor) & 0x421) == 0x401) {
                if ((cursor->stateFlags & 0x800000) != 0) {
                    return 1;
                }
            }
            cursor = cursor->nextActor;
        }
    } else {
        if ((btlUnitStatusPair(unit) & 0x21) == 1) {
            if ((unit->stateFlags & 0x800000) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlAreUnitsMissingStatusFlag(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    while (battler != 0) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            (battler->flags & 0x1000) != 0) {
            return 0;
        }
        battler = battler->nextActor;
    }
    return 1;
}

s32 btlAreUnitsHoldingStatusFlag(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    while (battler != 0) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            (battler->flags & 0x1000) == 0) {
            return 0;
        }
        battler = battler->nextActor;
    }
    return 1;
}

s32 btlIsHistoryCounterEmpty(void) {
    return btlHistoryCounter < 1;
}

s32 btlAnyUnitBlocksGroup200Element(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQuery(battler, action, 0x200)) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyGroupUnitHasZeroStat(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((btlUnitStatusPair(battler) & 0x221) == 0x201 &&
            battler->partyRecord.mp == 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlAnyUnitHasQueuedQuery(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00211360", btlAnyGroup400HasQuery);

INCLUDE_ASM(const s32, "game/code_00211360", btlAnyGroup200HasQuery);

/* Test the full queued words, rather than the halfword IDs used by category queries. */
s32 btlAnyUnitHasQueuedQuery(s32 unused, s32 id, s32 mask) {
    ActionStateLink *actor;
    BtlUnit *unit;
    u32 flags;
    s32 i;
    for (actor = ((BattleWork *)btlGetRuntime())->actionActors; actor != 0; actor = actor->next) {
        unit = actor->unit;
        if (unit == 0) {
            continue;
        }
        flags = unit->flags;
        if (!(flags & 1)) {
            continue;
        }
        if (!(flags & mask)) {
            continue;
        }
        if (flags & 0x20) {
            continue;
        }
        for (i = 0; i < 8; i++) {
            if (actor->actions[i].word == id) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlUnitBlocksElementQuery(BtlUnit *unit, s32 action, s32 mask) {
    u32 flags = unit->flags;
    s32 stat;
    s32 value;
    if (flags & 1) {
        if (flags & mask) {
            if (!(flags & 0x20)) {
                stat = fldGetSelectedUnitStat();
                if (action & 0x100000) {
                    s32 i;
                    for (i = 0; i < 19; i++) {
                        s32 index = btlElementToBitIndex(action, i);
                        if (index == 0x80) {
                            continue;
                        }
                        value = func_001ABF50(unit, index);
                        if (btlHasEnabledSpecialAbilityForSlot(unit, index) != 0 || (value & 0x20000) ||
                            (stat == 0x20000 && btlTestSelectedItemCategoryMask(unit, index) != 0)) {
                            return 1;
                        }
                    }
                    return 0;
                }
                value = func_001ABF50(unit, action);
                if (btlHasEnabledSpecialAbilityForSlot(unit, action) != 0 || (value & 0x20000) ||
                    (stat == 0x20000 && btlTestSelectedItemCategoryMask(unit, action) != 0)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00211360", func_00214928);

INCLUDE_RODATA(const s32, "game/code_00211360", D_00419B38);

INCLUDE_RODATA(const s32, "game/code_00211360", D_00419B68);

INCLUDE_RODATA(const s32, "game/code_00211360", D_00419B88);

INCLUDE_RODATA(const s32, "game/code_00211360", btlRequiredActionCategories);

INCLUDE_RODATA(const s32, "game/code_00211360", btlElementMasks);

INCLUDE_RODATA(const s32, "game/code_00211360", D_00419C30);

INCLUDE_RODATA(const s32, "game/code_00211360", D_00419C58);

