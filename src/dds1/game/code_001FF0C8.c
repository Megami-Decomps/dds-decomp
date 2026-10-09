#include "common.h"
#include "sdf_chip.h"
#include "btl_task_condition.h"
#include "eff_transform.h"
#include "btl_state.h"
#include "btl_command.h"
#include "btl_model_record.h"
#include "sdf_draw.h"
#include "evt_unit.h"
#include "mdl.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "dat_state.h"
#include "dat_command.h"

#define BTL_AI_SLOT_COUNT 5

#define BTL_AI_WEIGHT_MASK 0xFFFF

#define BTL_HEALTH_RATE_SCALE 100

#define BTL_PARTY_QUERY_MASK 0x221

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

extern s32 btlAreUnitStatusAndEntryFlagsClear();

extern s32 btlGetCommandFailureReason(BtlUnit *, s32);

extern s32 btlHasAvailableOption(void);

extern s32 btlMatchesActorEntryCodeCondition();

extern s32 func_002007A8(BtlUnit *, s32, s8);

extern s32 btlIsSelectedActorStatusAndRecordClear();

extern s8 btlHistoryCounter;

/* Command-actor state retained by AI queries; this is not a battle unit.
 * The first queued action is also the word tested by btlActionMatchesUnit. */
typedef union BtlActionSlot {
    s32 word;
    s16 actionId;
} BtlActionSlot;

typedef struct BtlActionTask {
    u8 pad00[0xC];
    s32 flags; /* 0x0C: AI context flags */
    u8 pad10[0x78];
    u16 aiCounter; /* 0x88: wraps as a halfword, then clamps to 0xFF */
    u8 pad8A[0xBC];
    s8 lowHpActionHold; /* 0x146: positive suppresses the low-HP action */
    u8 pad147;
    BtlActionSlot actions[8]; /* 0x148 */
    u8 pad168[4];
    struct BtlActionTask *next; /* 0x16C, same link as BtlTask */
} BtlActionTask;

/* Native 0x10-byte AI selection scratch. Its producer retains the command
 * actor and species/mode; the conditional-action dispatcher uses +8 as a
 * table row and +0xC as its query selector. This is not the singleton battle work. */
typedef struct BtlAiScratchWork {
    BtlActionTask *actor;
    s32 speciesId;
    s32 rowIndex;
    s32 conditionKind;
} BtlAiScratchWork;

extern BtlAiScratchWork *btlActionScratchWork;

extern s8 D_003A5A80[];

extern s8 D_003A5AA8[];

extern s32 btlGetRuntime(void);

extern void btlBossDebugPrintf(const char *, ...);

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

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

extern u8 sdfPfsDebugMode;

extern char D_003A5988[];

extern void btlDebugPrintf(const char *, ...);

extern s32 func_001A8CE0(s32);
extern s32 btlCounterReachedLimit(s32, u32);
extern s32 btlAnyUnitHasQueuedQuery(s32, s32, u32);

extern u32 func_001A9488(s32);



extern s32 btlRunAiAction();

extern u32 btlPickWeightedAiSlot();

extern u32 btlNextScaledRandom(u32);

extern u32 btlPreviousAiCandidateBucket;

extern s32 func_001FFE30(BtlTask *, s32, u16 *, s32 *);

extern void (*btlAiActionHandlers[])(BtlTask *, u32, s32);

INCLUDE_ASM(const s32, "game/code_001FF0C8", func_001FF0C8);

u32 func_001FF558(void) {
    return 1;
}

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

s32 btlSelectTierMatrixAiRoute(s32 context, s32 species, u32 *selected, u32 requestedRow) {
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
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 0;
                return 1;
            }
        }
        if (matches[tier][0] && matches[tier][1]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[1] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[1];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 1;
                return 1;
            }
        }
        if (matches[tier][0] && matches[tier][2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[2] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[2];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 2;
                return 1;
            }
        }
        if (matches[tier][1] && matches[tier][2]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[3] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[3];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 3;
                return 1;
            }
        }
        if (matches[tier][0]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[4] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[4];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 4;
                return 1;
            }
        }
        if (matches[tier][1]) {
            if (datEnemyAiRecords[species].decisions[tier].routes[5] != 8) {
                *selected = datEnemyAiRecords[species].decisions[tier].routes[5];
                btlActionScratchWork->rowIndex = tier;
                btlActionScratchWork->conditionKind = 5;
                return 1;
            }
        }
        if (matches[tier][2]) {
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

s32 btlSelectConditionalAiRoute(s32 context, s32 species, u32 *selected, u32 requestedRow) {
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
        btlBossDebugPrintf(D_003A5988);
    }
    return 0;
}

/* Sum of two 0..0xFFF rolls, folded into 0..0xFFF; /41 turns it into one of 100 buckets. */
static inline u32 btlRollTwice(void) {
    u32 value = btlNextScaledRandom(0x1000);
    value += btlNextScaledRandom(0x1000);
    return value & 0xFFF;
}

/* Rolls a bucket; if it lands within 3 of the previous one, rolls again. */
u32 btlRollAiBucket(void) {
    u32 slot = btlRollTwice() / 0x29;
    if (btlPreviousAiCandidateBucket >= slot - 3 && btlPreviousAiCandidateBucket <= slot + 3) {
        u32 reroll = btlRollTwice();
        btlPreviousAiCandidateBucket = slot;
        return reroll / 0x29;
    }
    btlPreviousAiCandidateBucket = slot;
    return slot;
}

/* Out parameters of the action-id lookup. */
typedef struct BtlActionLookup {
    u16 slot;
    s32 result;
} BtlActionLookup;

/* `packed` holds the handler index in its top 10 bits and the handler argument in the low 22. */
s32 btlRunAiAction(BtlTask *task, s32 id, u32 packed) {
    BtlActionLookup lookup;
    u32 op = packed >> 22;
    u32 payload = packed & 0x3fffff;

    func_001FFE30(task, id & 0xffff, &lookup.slot, &lookup.result);
    task->indexWork.phase = lookup.result;
    task->indexWork.skillId = lookup.slot;
    task->unit->partyRecord.unk190 = lookup.slot;
    btlAiActionHandlers[op](task, payload, lookup.result);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001FF0C8", D_003A5988);

INCLUDE_ASM(const s32, "game/code_001FF0C8", func_001FFE30);

extern s32 btlReadCurrentUnitHp(DatPartyRecord *);

extern u16 btlReadUnitStatusMask(DatPartyRecord *);

extern u32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *);

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
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_ACTIVE_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (btlIsUnitAtOrBelowHealthRate(unitCursor, healthPercent) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Reset the scratch-context counter only when its query succeeds; retain the native no-argument query. */
u32 btlResetAiCounterAtLimit(void) {
    u32 limitReached;

    limitReached = btlAiCounterReachedLimit();
    if (limitReached == 0) {
        return limitReached;
    }
    btlActionScratchWork->actor->aiCounter = 0;
    return 1;
}

/* Check an inclusive upper bound on active enemy-side units with bit 0x20 clear. */
s32 btlIsGroup400CountAtMost(s32 unused, u32 maximumCount) {
    u32 matchingCount = 0;
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            matchingCount++;
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    if (maximumCount < matchingCount) {
        return 0;
    }
    return 1;
}

/* Check an inclusive upper bound on eligible party units, additionally excluding condition bit 0x800. */
s32 btlIsGroup200EligibleCountAtMost(s32 unused, u32 maximumCount) {
    u32 matchingCount = 0;
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if ((unitCursor->partyRecord.status & 0x800) == 0) {
                matchingCount++;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    if (maximumCount < matchingCount) {
        return 0;
    }
    return 1;
}

u8 func_00200308(void) {
    s32 result;

    result = btlIsSelectedActorStatusAndRecordClear();
    return result != 0;
}

/* Check an inclusive upper bound on active party-side units with bit 0x20 clear. */
s32 btlIsGroup200CountAtMost(s32 unused, u32 maximumCount) {
    u32 matchingCount = 0;
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            matchingCount++;
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
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
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_ACTIVE_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (btlUnitHasAnyStatusInMask(unitCursor, actionMask) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Test party-side units for any requested action bit; DDS1 also requires bit 0x20 clear. */
s32 btlAnyGroup200HasActionMask(s32 unused, s32 actionMask) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (btlUnitHasAnyStatusInMask(unitCursor, actionMask) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Require every eligible party unit to intersect the requested mask; an empty selection succeeds. */
s32 btlAllGroup200HaveActionMask(s32 unused, s32 actionMask) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (btlUnitHasAnyStatusInMask(unitCursor, actionMask) == 0) {
                return 0;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 1;
}

/* Find an eligible party-side unit with the requested mode. */
s32 btlHasGroup200UnitMode(s32 unused, s32 unitMode) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (((BtlUnit *)unitCursor)->partyRecord.unitId == unitMode) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Find an eligible enemy-side unit of the requested mode with a different native identity. */
s32 btlHasOtherGroup400UnitMode(BtlUnit *excludedUnit, s32 unitMode) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (((BtlUnit *)unitCursor)->partyRecord.unitId == unitMode) {
                if (unitCursor->identity != excludedUnit->identity) {
                    return 1;
                }
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

extern s32 D_00360E98[];

extern s32 btlActorEntryIsExpired();

extern s32 btlGetActorEntryCode();

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
            if (btlActorEntryIsExpired(actorAddress, D_00360E98[evenIndex]) != 0 && btlGetActorEntryCode(actorAddress, D_00360E98[evenIndex]) > 0) {
                return 1;
            }
        }
    } else if (conditionIndex == 0xC) {
        for (oddIndex = 1; oddIndex < BTL_ENTRY_CODE_SCAN_COUNT; oddIndex += 2) {
            if (btlActorEntryIsExpired(actorAddress, D_00360E98[oddIndex]) != 0 && btlGetActorEntryCode(actorAddress, D_00360E98[oddIndex]) <= 0) {
                return 1;
            }
        }
    } else if (btlActorEntryIsExpired(actorAddress, D_00360E98[conditionIndex]) != 0 || conditionIndex == 0xA || conditionIndex == 0xD) {
        if ((conditionIndex & 1) == 0 || conditionIndex == 0xD) {
            if (btlGetActorEntryCode(actorAddress, D_00360E98[conditionIndex]) > 0) {
                return 1;
            }
        } else {
            if (btlGetActorEntryCode(actorAddress, D_00360E98[conditionIndex]) <= 0) {
                return 1;
            }
        }
    }
    return 0;
}

extern s32 btlMatchActorEntryCode(BtlUnit *, s32);

s32 func_002007A8(BtlUnit *unit, s32 conditionIndex, s8 inverse) {
    s32 index;
    s16 result;

    if (conditionIndex == 0xB) {
        for (index = 0; index < BTL_ENTRY_CODE_SCAN_COUNT; index += 2) {
            result = btlMatchActorEntryCode(unit, D_00360E98[index]);
            if (inverse == 0) {
                if (result == 1) {
                    return 1;
                }
            } else if (result == 2) {
                return 1;
            }
        }
    } else if (conditionIndex == 0xC) {
        for (index = 1; index < BTL_ENTRY_CODE_SCAN_COUNT; index += 2) {
            result = btlMatchActorEntryCode(unit, D_00360E98[index]);
            if (inverse == 0) {
                if (result == 2) {
                    return 1;
                }
            } else if (result == 1) {
                return 1;
            }
        }
    } else {
        result = btlMatchActorEntryCode(unit, D_00360E98[conditionIndex]);
        if ((conditionIndex & 1) == 0) {
            if (inverse == 0) {
                if (result == 1) {
                    return 1;
                }
            } else if (result == 2) {
                return 1;
            }
        } else {
            if (inverse == 0) {
                if (result == 2) {
                    return 1;
                }
            } else if (result == 1) {
                return 1;
            }
        }
    }
    return 0;
}

extern s32 btlTestSelectedItemCategoryMask(BtlUnit *, s32);

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
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (btlMatchesActorEntryCodeCondition((s32)unitCursor, conditionIndex) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Return whether any eligible enemy unit meets the requested entry-code condition. */
s32 btlAnyEnemyMeetsEntryCodeCondition(s32 unused, s32 conditionIndex) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (btlMatchesActorEntryCodeCondition((s32)unitCursor, conditionIndex) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Normalize the mode-zero action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeZero(u32 unitAddress, u32 actionQuery) {
    s32 queryResult;

    queryResult = func_002007A8((BtlUnit *)unitAddress, (s32)actionQuery, 0);
    return queryResult != 0;
}

/* Normalize the mode-one action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeOne(u32 unitAddress, u32 actionQuery) {
    s32 queryResult;

    queryResult = func_002007A8((BtlUnit *)unitAddress, (s32)actionQuery, 1);
    return queryResult != 0;
}

/* Return whether an eligible party unit has a nonzero mode-zero query result. */
s32 btlAnyPartyPassesEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (func_002007A8(unitCursor, actionQuery, 0) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Return whether an eligible party unit has a nonzero mode-one query result. */
s32 btlAnyPartyPassesInverseEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (func_002007A8(unitCursor, actionQuery, 1) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Return whether an eligible enemy unit has a nonzero mode-zero query result. */
s32 btlAnyEnemyPassesEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (func_002007A8(unitCursor, actionQuery, 0) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Return whether an eligible enemy unit has a nonzero mode-one query result. */
s32 btlAnyEnemyPassesInverseEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (func_002007A8(unitCursor, actionQuery, 1) != 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Return whether an eligible party unit has a zero mode-zero query result. */
s32 btlAnyPartyFailsEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (func_002007A8(unitCursor, actionQuery, 0) == 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Return whether an eligible enemy unit has a zero mode-zero query result. */
s32 btlAnyEnemyFailsEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (func_002007A8(unitCursor, actionQuery, 0) == 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Look for an eligible party unit with native flag 0x1000 clear; its meaning is not established here. */
s32 btlAnyGroup200LacksFlag1000(void) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((btlUnitStatusPair(unitCursor) & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if ((((BtlUnit *)unitCursor)->status.flags & 0x1000) == 0) {
                return 1;
            }
        }
        unitCursor = (u8 *)((BtlUnit *)unitCursor)->next;
    }
    return 0;
}

/* Normalize entry-code condition 10 to a byte boolean without changing the native short-arity calls. */
u8 btlUnitPassesActionTenCheck(u32 unitAddress) {
    s32 queryResult;

    queryResult = btlMatchesActorEntryCodeCondition(unitAddress, 10);
    return queryResult != 0;
}

s32 func_00200F00(void) {
    return btlMatchesActorEntryCodeCondition() != 0;
}

s32 func_00200F20(BtlUnit *unit) {
    u32 flags;

    if (unit->status.flags & 0x200) {
        return 0;
    }
    flags = unit->partyRecord.flags & 0x2000;
    return flags != 0;
}

/* Test scratch-context bit 1; no gameplay meaning for this flag is established here. */
s32 btlHasContextFlagTwo(void) {
    return ((btlActionScratchWork->actor->flags & 2) > 0);
}

s32 btlCheckCounterLimit(s32 unused, u32 minimumCount) {
    return btlCounterReachedLimit(unused, minimumCount);
}

/* Return whether counter query 4 has reached the requested inclusive lower bound. */
s32 btlCounterReachedLimit(s32 unused, u32 minimumCount) {
    if (func_001A9488(4) < minimumCount) {
        return 0;
    }
    return 1;
}

/* Increment the stored counter, retain native wrap/clamp behavior, then reread it for the limit test. */
s32 btlAiCounterReachedLimit(s32 unused, u32 minimumCount) {
    BtlActionTask *counterState = btlActionScratchWork->actor;
    counterState->aiCounter++;
    counterState->aiCounter = counterState->aiCounter <= 0 ? 0 : counterState->aiCounter >= BTL_AI_COUNTER_SATURATION ? BTL_AI_COUNTER_MAX : counterState->aiCounter;
    if (btlActionScratchWork->actor->aiCounter < minimumCount) {
        return 0;
    }
    return 1;
}

/* Compare the battle turn count with an inclusive lower bound. */
s32 btlTurnReachedLimit(s32 unused, u32 minimumTurns) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    if ((u32)battleState->turnCount < minimumTurns) {
        return 0;
    }
    return 1;
}

/* Return true only in phase 2 with no counted turns. */
s32 btlIsReadyWithoutTurns(void) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    if (battleState->turnPhase == 2) {
        if (battleState->turnCount == 0) {
            return 1;
        }
    }
    return 0;
}

extern u16 btlReadCurrentUnitMp(DatPartyRecord *);

extern u32 btlComputeSkillAdjustedMaxMp(DatPartyRecord *);

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

u8 func_002010D8(void) {
    s32 result;

    result = btlHasAvailableOption();
    return result != 0;
}

extern void func_001A30F8(s32, BtlIndexList *, s32, s32, s32);

/* Query the context's index list, freeing it on both the first success and exhausted-list paths. */
s32 btlAnyIndexedUnitPassesQuery(BtlUnit *unit) {
    u32 entryIndex;
    u32 entryCount;
    s32 contextAddress;
    BtlIndexList *indexList;
    if (unit->status.flags & 0x400) {
        return 0;
    }
    contextAddress = (s32)btlActionScratchWork->actor;
    indexList = btlAllocateIndexList(13);
    func_001A30F8(contextAddress, indexList, 2, 0, 0);
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
    BtlActionTask *context = btlActionScratchWork->actor;
    s32 conditionMet = 0;
    u32 actionTime = unit->partyRecord.level;
    u32 currentTime = func_001A9488(4);
    s16 bucketRoll;

    if (context->lowHpActionHold <= 0) {
        if (currentTime >= actionTime + BTL_LOW_HP_ACTION_DELAY) {
            if (unit->partyRecord.hp * BTL_HEALTH_RATE_SCALE / unit->partyRecord.maxHp < BTL_LOW_HP_PERCENT_LIMIT) {
                if (btlIsGroup400CountAtMost((s32)unit, BTL_LOW_HP_ENEMY_COUNT_LIMIT) != 0) {
                    bucketRoll = btlRollAiBucket();
                    conditionMet = bucketRoll < BTL_LOW_HP_BUCKET_LIMIT;
                }
            }
        }
    }
    if (conditionMet == 1) {
        if (btlIsSelectedActorStatusAndRecordClear(unit) != 0) {
            return 1;
        }
    }
    return 0;
}

/* Test native unit flag 0x1000 without assigning it an unverified gameplay meaning. */
s32 btlUnitHasFlag1000(s32 unitAddress) {
    return (((s32)((BtlUnit *)unitAddress)->status.flags & 0x1000) > 0);
}

u8 btlIsCommandAvailable(BtlUnit *unit, s32 command) {
    s32 result;

    result = btlGetCommandFailureReason(unit, command);
    return result == 0;
}

u8 func_002012E0(void) {
    s32 result;

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

extern s32 func_001A2F50(BtlUnit *, s32);

s32 btlUnitHasNegativeActionQueryResult(void *unit, s32 mask) {
    s32 i;
    s32 value;
    if (mask & 0x100000) {
        for (i = 0; i < 0x13; i++) {
            value = btlElementToBitIndex(mask, i);
            if (value == 0x80) {
                continue;
            }
            value = func_001A2F50(unit, value);
            if ((u16)value < 100) {
                continue;
            }
            if ((u32)value & 0x80000000) {
                return 1;
            }
        }
        return 0;
    }
    value = func_001A2F50(unit, mask);
    if ((u16)value >= 100) {
        if (value < 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlUnitHasNegativeActionQueryResult(void *, s32);

s32 btlAnyEnemyHasNegativeActionResult(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x401) == 0x401) {
            if (btlUnitHasNegativeActionQueryResult(unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyPartyUnitHasNegativeActionResult(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x201) == 0x201) {
            if (btlUnitHasNegativeActionQueryResult(unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlHasGroup200DifferentUnitMode(s32 unused, s32 kind) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->partyRecord.unitId != kind) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlHasOtherGroup400DifferentUnitMode(BtlUnit *actor, s32 kind) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x421) == 0x401) {
            if (((BtlUnit *)unit)->partyRecord.unitId != kind) {
                if (unit->identity != actor->identity) {
                    return 1;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

extern s32 btlUnitBlocksElementQueryForGroup(BtlUnit *, s32, u32);

s32 btlAnyUnitPassesCheck200(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 0) {
            return 1;
        }
        unit = unit->next;
    }
    return 0;
}

s32 btlCanQueryElementAgainstParty(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
            return 0;
        }
        unit = unit->next;
    }
    return 1;
}

s32 btlAnyUnitPassesCheck400(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 0) {
            return 1;
        }
        unit = unit->next;
    }
    return 0;
}

s32 btlCanQueryElementAgainstEnemies(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 1) {
            return 0;
        }
        unit = unit->next;
    }
    return 1;
}

extern s32 btlWouldUiValueFallBelowQuarter(BtlUnit *, s32);

extern u8 sdfPfsDebugMode;

s32 btlCheckActorEligibilityWithDebug(s32 actor) {
    if (btlWouldUiValueFallBelowQuarter((BtlUnit *)actor, 0) != 0) {
        if (sdfPfsDebugMode == 0) {
            btlBossDebugPrintf(D_003A5A80);
        }
        return 1;
    }
    if (sdfPfsDebugMode == 0) {
        btlBossDebugPrintf(D_003A5AA8);
    }
    return 0;
}

extern s32 func_00201900(s32, s16, s8);

s32 func_00201900(s32 mask, s16 actionId, s8 force) {
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
s32 btlAnyUnitHasActionInSlots(s32 mask, s32 action) {
    BtlTask *task;
    BtlActionSlot *slots;
    BtlUnit *unit;
    u32 flags;
    s32 i;

    for (task = ((BtlState *)btlGetRuntime())->tasks; task != NULL; task = task->next) {
        unit = task->unit;
        if (unit == NULL) {
            continue;
        }
        flags = unit->status.flags;
        if (!(flags & 1)) {
            continue;
        }
        if (!(flags & mask)) {
            continue;
        }
        if (flags & 0x20) {
            continue;
        }
        slots = ((BtlActionTask *)task)->actions;
        for (i = 0; i < 8; i++) {
            if (func_00201900(action, slots[i].actionId, 1) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlAnyGroup200HasAction(s32 unused, s32 action) {
    return btlAnyUnitHasActionInSlots(0x200, action);
}

s32 btlAnyGroup400HasAction(s32 unused, s32 action) {
    return btlAnyUnitHasActionInSlots(0x400, action);
}

s32 func_00201B50(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x421) == 0x401) {
            if (func_00201900(action, (s16)((BtlUnit *)unit)->partyRecord.unk190, 0) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201BD8(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
            if (func_00201900(action, (s16)((BtlUnit *)unit)->partyRecord.unk190, 0) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyPartyUnitHasFullActionSet(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
            if (btlUnitHasAllTenActions(unit) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyEnemyHasFullActionSet(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x421) == 0x401) {
            if (btlUnitHasAllTenActions(unit) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAreUnitsMissingStatusFlag(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->status.flags & 0x1000) != 0) {
                return 0;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 1;
}

s32 btlAreUnitsHoldingStatusFlag(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->status.flags & 0x1000) == 0) {
                return 0;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 1;
}

s32 btlIsHistoryCounterEmpty(void) {
    return btlHistoryCounter < 1;
}

extern s32 btlUnitBlocksElementQuery(BtlUnit *, s32, s32);

s32 btlAnyUnitBlocksGroup200Element(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQuery(unit, action, 0x200) != 0) {
            return 1;
        }
        unit = unit->next;
    }
    return 0;
}

s32 btlAnyGroupUnitHasZeroStat(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->partyRecord.mp == 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyGroup400HasQuery(s32 unused, s32 query) {
    return btlAnyUnitHasQueuedQuery(unused, query, 0x400);
}

s32 btlAnyGroup200HasQuery(s32 unused, s32 query) {
    return btlAnyUnitHasQueuedQuery(unused, query, 0x200);
}

s32 btlAnyUnitHasQueuedQuery(s32 unused, s32 query, u32 mask) {
    u8 *node = (u8 *)((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        u8 *owner = (u8 *)((BtlTask *)node)->unit;
        if (owner != 0) {
            u32 flags = ((BtlUnit *)owner)->status.flags;
            if ((flags & 1) && (flags & mask) && !(flags & 0x20)) {
                s32 i;
                for (i = 0; i < 8; i++) {
                    if (((BtlActionTask *)node)->actions[i].word == query) {
                        return 1;
                    }
                }
            }
        }
        node = (u8 *)((BtlTask *)node)->next;
    }
    return 0;
}

extern s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *, u32);

extern s32 fldGetSelectedUnitStat();

s32 btlUnitBlocksElementQuery(BtlUnit *unit, s32 action, s32 mask) {
    u32 flags = unit->status.flags;
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
                        value = func_001A2F50(unit, index);
                        if (btlHasEnabledSpecialAbilityForSlot(unit, index) != 0 || (value & 0x20000) ||
                            (stat == 0x20000 && btlTestSelectedItemCategoryMask(unit, index) != 0)) {
                            return 1;
                        }
                    }
                    return 0;
                }
                value = func_001A2F50(unit, action);
                if (btlHasEnabledSpecialAbilityForSlot(unit, action) != 0 || (value & 0x20000) ||
                    (stat == 0x20000 && btlTestSelectedItemCategoryMask(unit, action) != 0)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 func_00202158(void) {
    return func_001A8CE0(0);
}

INCLUDE_RODATA(const s32, "game/code_001FF0C8", btlRequiredActionCategories);

INCLUDE_RODATA(const s32, "game/code_001FF0C8", btlElementMasks);

INCLUDE_RODATA(const s32, "game/code_001FF0C8", D_003A5A80);

INCLUDE_RODATA(const s32, "game/code_001FF0C8", D_003A5AA8);

