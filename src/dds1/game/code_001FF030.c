#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "btl_action.h"

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
#define BTL_TASK_CONDITION_HANDLE_GONE 4
#define BTL_TASK_CONDITION_HANDLE_RUNNING_OR_GONE 5

extern u32 btlGetEffectActor(void);

extern u32 func_001A3360(s32, s32, s32);

extern void *btlAllocateIndexList(s32);

extern u32 btlGetIndexListCount();

extern s32 btlAreUnitStatusAndEntryFlagsClear();

extern s32 func_001A2B00(void);

extern s32 btlHasAvailableOption(void);

extern s32 btlMatchesActorEntryCodeCondition();

extern s32 func_002007A8(u32, u32, u32);

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

extern char D_003BB8A0[];

extern char D_003BB898[];

extern BtlActionAnimationRecord *datActionAnimationRecords;

extern s8 D_003A5A80[];

extern s8 D_003A5AA8[];

extern u32 btlGetEventEffectValue(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetEffectActive(void);

extern u32 btlGetSelectedBossEffectId(void);

extern u32 btlGetSpecialModeEffectValue(void);

extern s32 btlGetRuntime(void);
extern void btlBossDebugPrintf(s32, ...);
extern s32 btlFindUnitByActor(s32);

extern void func_001D6300(void *, void *);

extern u8 *datCommandRecords;

extern s8 *datCommandSelectors;

extern s32 mdlFlagTest(s32);

extern u8 *btlCreateStiffenDamageShakeTask(u8 *, f32);

extern void btlSetUnitPosition(BtlUnit *, void *);

extern void btlSetUnitRotation(BtlUnit *, void *);

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

typedef struct AiSpecies {
    u8 pad00[0x40];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *datEnemyAiRecords;

extern u8 sdfPfsDebugMode;

extern char D_003A5988[];

extern void btlDebugPrintf(const char *, ...);




extern s32 btlSetLinkedDefeatCameraPresetA();

extern s32 btlAnyUnitHasActionInSlots();

extern s32 func_001A8CE0(s32);

extern u32 func_001A9488(s32);

extern u8 D_00360EE0[];

extern u8 D_00360EF0[];

extern void btlInitMotionTransformFromVectors(s32, u8 *, u8 *);

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(void *);

extern s32 btlRunAiAction();

extern u32 btlPickWeightedAiSlot();

extern u32 btlNextScaledRandom(u32);

extern u32 btlPreviousAiCandidateBucket;

extern s32 func_001FFE30(BtlTask *, s32, u16 *, s32 *);

extern void (*btlAiActionHandlers[])(BtlTask *, u32, s32);

extern void btlCopyIndexList(s32, s32);

extern void *memset(void *, s32, u32);

extern s32 func_002024A8(s32, u16 *, u16);

/* Pick a weighted slot in one species row, run its action and release the shared scratch allocation. */
void btlRunWeightedAiAction(BtlTask *task, s32 rowIndex) {
    u16 speciesId;
    s32 slotIndex;

    btlActionScratchWork = sdfAllocAndClearQuadwords(sizeof(BtlAiScratchWork));
    speciesId = task->unit->mode;
    slotIndex = btlPickWeightedAiSlot(task->unit, speciesId, rowIndex);
    btlRunAiAction(task, datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].actionId, datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF0C8);

u32 func_001FF558(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF560);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF8D8);

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
        btlBossDebugPrintf((s32)D_003A5988);
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
    task->result = lookup.result;
    task->arg = lookup.slot;
    task->unit->actionSlot = lookup.slot;
    btlAiActionHandlers[op](task, payload, lookup.result);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5988);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFE30);

extern s32 btlReadCurrentUnitHp(void *);

extern s32 btlComputeSkillAdjustedMaxHp(void *);

/* Compare HP against a signed percentage using the native products and unsigned comparison. */
s32 btlIsUnitAtOrBelowHealthRate(BtlUnit *unit, s32 healthPercent) {
    void *statAddress = &unit->statBits;
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_ACTIVE_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if ((unitCursor->conditionFlags & 0x800) == 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
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
s32 btlUnitHasActionMask(s32 unitAddress, s32 actionMask) {
    return (btlReadUnitStatusMask(unitAddress + 0x120, actionMask) & actionMask) != 0;
}

/* Test active enemy-side units for any requested action bit; do not filter bit 0x20. */
s32 btlAnyGroup400HasActionMask(s32 unused, s32 actionMask) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_ACTIVE_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (btlUnitHasActionMask((s32)unitCursor, actionMask) != 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (btlUnitHasActionMask((s32)unitCursor, actionMask) != 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (btlUnitHasActionMask((s32)unitCursor, actionMask) == 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (((BtlUnit *)unitCursor)->mode == unitMode) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (((BtlUnit *)unitCursor)->mode == unitMode) {
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

INCLUDE_ASM(const s32, "game/code_001FF030", func_002007A8);

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
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

    queryResult = func_002007A8(unitAddress, actionQuery, 0);
    return queryResult != 0;
}

/* Normalize the mode-one action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeOne(u32 unitAddress, u32 actionQuery) {
    s32 queryResult;

    queryResult = func_002007A8(unitAddress, actionQuery, 1);
    return queryResult != 0;
}

/* Return whether an eligible party unit has a nonzero mode-zero query result. */
s32 btlAnyPartyPassesEntryCheck(s32 unused, u32 actionQuery) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (func_002007A8((u32)unitCursor, actionQuery, 0) != 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (func_002007A8((u32)unitCursor, actionQuery, 1) != 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (func_002007A8((u32)unitCursor, actionQuery, 0) != 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (func_002007A8((u32)unitCursor, actionQuery, 1) != 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if (func_002007A8((u32)unitCursor, actionQuery, 0) == 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_ENEMY_QUERY_MASK) == BTL_ENEMY_ACTIVE_FLAGS) {
            if (func_002007A8((u32)unitCursor, actionQuery, 0) == 0) {
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
        if ((*(u64 *)&unitCursor->flags & BTL_PARTY_QUERY_MASK) == BTL_PARTY_ACTIVE_FLAGS) {
            if ((((BtlUnit *)unitCursor)->flags & 0x1000) == 0) {
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

    if (unit->flags & 0x200) {
        return 0;
    }
    flags = unit->statBits & 0x2000;
    return flags != 0;
}

/* Test scratch-context bit 1; no gameplay meaning for this flag is established here. */
s32 btlHasContextFlagTwo(void) {
    return ((btlActionScratchWork->actor->flags & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", btlCheckCounterLimit);

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

extern s32 btlReadCurrentUnitMp(void *);

extern s32 btlComputeSkillAdjustedMaxMp(void *);

/* Compare a unit stat with a percentage of its maximum.
 * The stat's identity is not established by these two accessors. */
s32 btlIsUnitStatAtOrBelowRate(BtlUnit *unit, s32 percentage) {
    void *statAddress = &unit->statBits;
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

extern void func_001A30F8(s32, void *, s32, s32, s32);

extern s32 btlGetIndexListEntry(void *, u32);

extern void btlFreeIndexList(void *);

/* Query the context's index list, freeing it on both the first success and exhausted-list paths. */
s32 btlAnyIndexedUnitPassesQuery(BtlUnit *unit) {
    u32 entryIndex;
    u32 entryCount;
    s32 contextAddress;
    void *indexList;
    if (unit->flags & 0x400) {
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
    u32 actionTime = unit->actionTime;
    u32 currentTime = func_001A9488(4);
    s16 bucketRoll;

    if (context->lowHpActionHold <= 0) {
        if (currentTime >= actionTime + BTL_LOW_HP_ACTION_DELAY) {
            if (unit->hp * BTL_HEALTH_RATE_SCALE / unit->maxHp < BTL_LOW_HP_PERCENT_LIMIT) {
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
    return (((s32)((BtlUnit *)unitAddress)->flags & 0x1000) > 0);
}

u8 func_002012C0(void) {
    s32 result;

    result = func_001A2B00();
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

extern s32 func_001A2F50();

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
        if ((*(u64 *)&unit->flags & 0x401) == 0x401) {
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
        if ((*(u64 *)&unit->flags & 0x201) == 0x201) {
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
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->mode != kind) {
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
        if ((*(u64 *)&unit->flags & 0x421) == 0x401) {
            if (((BtlUnit *)unit)->mode != kind) {
                if (unit->identity != actor->identity) {
                    return 1;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyUnitPassesCheck200(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 0) {
            return 1;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlCanQueryElementAgainstParty(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
            return 0;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 1;
}

s32 btlAnyUnitPassesCheck400(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 0) {
            return 1;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlCanQueryElementAgainstEnemies(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 1) {
            return 0;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 1;
}

extern s32 btlWouldUiValueFallBelowQuarter(s32, s32);

extern u8 sdfPfsDebugMode;

s32 btlCheckActorEligibilityWithDebug(s32 actor) {
    if (btlWouldUiValueFallBelowQuarter(actor, 0) != 0) {
        if (sdfPfsDebugMode == 0) {
            btlBossDebugPrintf((s32)D_003A5A80);
        }
        return 1;
    }
    if (sdfPfsDebugMode == 0) {
        btlBossDebugPrintf((s32)D_003A5AA8);
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
                if (datCommandSelectors[actionId * 2] == value) {
                    if (force != 0) {
                        return 1;
                    }
                    if ((u8)(datCommandRecords[actionId * 0x38 + 9] - 1) < 2) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    if (datCommandSelectors[actionId * 2] == mask) {
        if (force != 0) {
            return 1;
        }
        if ((u8)(datCommandRecords[actionId * 0x38 + 9] - 1) < 2) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyUnitHasActionInSlots(mask, action)
u32 mask;
s32 action;
{
    u8 *node = (u8 *)((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        u8 *owner = (u8 *)((BtlTask *)node)->unit;
        if (owner != 0) {
            u32 flags = ((BtlUnit *)owner)->flags;
            if ((flags & 1) && (flags & mask) && !(flags & 0x20)) {
                s32 i;
                for (i = 0; i < 8; i++) {
                    /* Required to match: byte-offset indexing keeps the original induction register. */
                    if (func_00201900(action, *(s16 *)(node + 0x148 + i * 4), 1) != 0) {
                        return 1;
                    }
                }
            }
        }
        node = (u8 *)((BtlTask *)node)->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", btlAnyGroup200HasAction);

INCLUDE_ASM(const s32, "game/code_001FF030", btlAnyGroup400HasAction);

s32 func_00201B50(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((*(u64 *)&unit->flags & 0x421) == 0x401) {
            if (func_00201900(action, ((BtlUnit *)unit)->actionSlot, 0) != 0) {
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
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            if (func_00201900(action, ((BtlUnit *)unit)->actionSlot, 0) != 0) {
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
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
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
        if ((*(u64 *)&unit->flags & 0x421) == 0x401) {
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
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->flags & 0x1000) != 0) {
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
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->flags & 0x1000) == 0) {
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

extern s32 btlUnitBlocksElementQuery(s32, s32, s32);

s32 btlAnyUnitBlocksGroup200Element(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQuery(unit, action, 0x200) != 0) {
            return 1;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyGroupUnitHasZeroStat(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->unk_12A == 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", btlAnyGroup400HasQuery);

INCLUDE_ASM(const s32, "game/code_001FF030", btlAnyGroup200HasQuery);

s32 btlAnyUnitHasQueuedQuery(s32 unused, s32 query, u32 mask) {
    u8 *node = (u8 *)((BtlState *)btlGetRuntime())->tasks;
    while (node != 0) {
        u8 *owner = (u8 *)((BtlTask *)node)->unit;
        if (owner != 0) {
            u32 flags = ((BtlUnit *)owner)->flags;
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

extern s32 btlHasEnabledSpecialAbilityForSlot(void *, s32);

extern s32 fldGetSelectedUnitStat();

s32 btlUnitBlocksElementQuery(s32 unit, s32 action, s32 mask) {
    u32 flags = ((BtlUnit *)unit)->flags;
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
                        value = func_001A2F50((void *)unit, index);
                        if (btlHasEnabledSpecialAbilityForSlot((void *)unit, index) != 0 || (value & 0x20000) ||
                            (stat == 0x20000 && btlTestSelectedItemCategoryMask((void *)unit, index) != 0)) {
                            return 1;
                        }
                    }
                    return 0;
                }
                value = func_001A2F50((void *)unit, action);
                if (btlHasEnabledSpecialAbilityForSlot((void *)unit, action) != 0 || (value & 0x20000) ||
                    (stat == 0x20000 && btlTestSelectedItemCategoryMask((void *)unit, action) != 0)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202158);

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

extern s32 btlHasMappedSpecialAbilityForSlot(void *, s32);

extern s32 btlHasSpecialAbilityWhenArgumentUnset(void *, s32);

extern s32 btlHasEnabledSpecialAbilityForSlot(void *, s32);

s32 btlUnitBlocksElementQueryForGroup(u8 *unit, s32 action, u32 mask) {
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 1) {
        if (flags & mask) {
            if (!(flags & 0x20)) {
                if (action & 0x100000) {
                    s32 i;
                    for (i = 0; i < 19; i++) {
                        s32 index = btlElementToBitIndex(action, i);
                        if (index == 0x80) {
                            continue;
                        }
                        if (btlTestSelectedItemCategoryMask(unit, index) != 0 ||
                            btlHasMappedSpecialAbilityForSlot(unit, index) != 0 ||
                            btlHasSpecialAbilityWhenArgumentUnset(unit, index) != 0 ||
                            btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (btlTestSelectedItemCategoryMask(unit, action) != 0 ||
                    btlHasMappedSpecialAbilityForSlot(unit, action) != 0 ||
                    btlHasSpecialAbilityWhenArgumentUnset(unit, action) != 0) {
                    return 0;
                }
                return btlHasEnabledSpecialAbilityForSlot(unit, action) == 0;
            }
        }
    }
    return 2;
}

s32 btlAllUnitsPassCheck200(s32 unused, s32 action) {
    s32 node = (s32)((BtlState *)btlGetRuntime())->units;
    while (node != 0) {
        if (btlUnitBlocksElementQueryForGroup(node, action, 0x200) == 0) {
            return 0;
        }
        node = (s32)((BtlUnit *)node)->next;
    }
    return 1;
}

s32 btlAllUnitsPassCheck400(s32 unused, s32 action) {
    s32 node = (s32)((BtlState *)btlGetRuntime())->units;
    while (node != 0) {
        if (btlUnitBlocksElementQueryForGroup(node, action, 0x400) == 0) {
            return 0;
        }
        node = (s32)((BtlUnit *)node)->next;
    }
    return 1;
}

extern s32 btlGroup400UnitHasAction(void *, s32);

s32 btlUnitHasEitherSpecialAction(void *unit) {
    if (btlGroup400UnitHasAction(unit, 0x1b2) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1b6) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1ba) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1be) != 0) {
        return 1;
    }
    return btlGroup400UnitHasAction(unit, 0x1c2) != 0;
}

/* Require the complete first queued-action word and unit status bit 0x1000.
 * Comparing only actions[0].actionId would discard the upper halfword. */
s32 btlActionMatchesUnit(u8 *unit, s32 action) {
    BtlActionTask *context = btlActionScratchWork->actor;
    if (context->actions[0].word == action) {
        if (((BtlUnit *)unit)->stateFlags & 0x1000) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_001A8A30(void *, s32);

s32 btlGroup400UnitHasAction(void *unit, s32 action) {
    btlGetRuntime();
    if ((*(u64 *)&((BtlUnit *)unit)->flags & 0x421) == 0x401) {
        if (func_001A8A30(unit, action) != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002024A8);

s32 btlBuildActorIndexListAndCount(s32 actor, u32 *matchingCount, u32 *listCount) {
    u32 result;
    s32 list;

    list = btlAllocateIndexList(0xd);
    result = func_001A3360(actor, list, 0);
    *matchingCount = result;
    result = btlGetIndexListCount(list);
    *listCount = result;
    return list;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", btlRequiredActionCategories);

INCLUDE_RODATA(const s32, "game/code_001FF030", btlElementMasks);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5AA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5BD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202668);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202F90);

/* Picks the target with the lowest nonzero health value among units that block the
 * element query; when none qualifies, every listed unit is a candidate. */
s32 btlSelectLowestHealthElementBlockTarget(s32 actor, s32 action) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u32 best;
    u32 bestIndex;
    u16 found;
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        found = 0;
        best = 0x7FFF;
        bestIndex = 0;
        for (i = 0; i < count; i++) {
            s32 unit = btlGetIndexListEntry((void *)list, i);
            if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
                u16 current = btlReadCurrentUnitHp(&((BtlUnit *)unit)->statBits);
                if (best >= current && current != 0) {
                    best = current;
                    found++;
                    bestIndex = i;
                }
            }
        }
        if (found != 0) {
            btlAppendIndexListEntry(((BtlTask *)actor)->targetList, btlGetIndexListEntry((void *)list, bestIndex));
        } else {
            u32 n = count;
            for (i = 0; i < n; i++) {
                flags[i] = 1;
            }
            btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        }
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectLowestHealthRateTarget(s32 actor) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    s32 best;
    u32 bestIndex;
    u32 i;
    s32 result;

    switch (matching) {
    case 0:
        best = 0x63;
        memset(flags, 0, sizeof(flags));
        bestIndex = 0x20;
        for (i = 0; i < count; i++) {
            void *stats = &((BtlUnit *)btlGetIndexListEntry((void *)list, i))->statBits;
            s32 current = btlReadCurrentUnitHp(stats);
            s32 percent = current * 100 / btlComputeSkillAdjustedMaxHp(stats);

            if (best >= percent && current != 0) {
                best = percent;
                bestIndex = i;
            }
        }
        if (bestIndex != 0x20) {
            result = btlGetIndexListEntry((void *)list, bestIndex);
        } else {
            result = func_002024A8(list, flags, count);
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, result);
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectTargetsByActionMask(s32 actor, s32 mask) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (btlUnitHasActionMask(btlGetIndexListEntry((void *)list, i), mask) != 0) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectTargetsWithoutActionMask(s32 actor, s32 mask) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (btlUnitHasActionMask(btlGetIndexListEntry((void *)list, i), mask) == 0) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectTargetsByMode(s32 actor, s32 mode) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)btlGetIndexListEntry((void *)list, i))->mode == mode) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002036E8);

INCLUDE_ASM(const s32, "game/code_001FF030", btlSelectLowestRankTarget);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203BA8);

s32 btlSelectTargetsExcludingActorUnit(s32 actor) {
    void *list = btlAllocateIndexList(13);
    u32 mode;
    u32 count;
    u16 picked[12];
    u32 i;
    s32 found;

    switch (((BtlTask *)actor)->unit->mode) {
    case 3:
    case 5:
    case 6:
        mode = (((BtlTask *)actor)->unit->flags & 0x1000) ? 0 : 2;
        break;
    default:
        mode = 0;
        break;
    }
    func_001A30F8(actor, list, 1, 2, 0);
    count = btlGetIndexListCount(list);
    if (count == 0) {
        mode = 0;
    }
    switch (mode) {
    case 0:
        found = 0;
        memset(picked, 0, sizeof(picked));
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)btlGetIndexListEntry(list, i))->identity != ((BtlTask *)actor)->unit->identity) {
                picked[found] = 1;
                found++;
            }
        }
        if (found == 0) {
            btlAppendIndexListEntry(((BtlTask *)actor)->targetList, (s32)((BtlTask *)actor)->unit);
            btlFreeIndexList(list);
            return 1;
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8((s32)list, picked, count));
        btlFreeIndexList(list);
        return 1;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, (s32)list);
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203E38);

s32 btlAppendSelfAfterTargetScan(s32 actor) {
    void *list = btlAllocateIndexList(13);
    func_001A30F8(actor, list, 1, 1, 0);
    btlGetIndexListCount(list);
    btlAppendIndexListEntry(((BtlTask *)actor)->targetList, (s32)((BtlTask *)actor)->unit);
    btlFreeIndexList(list);
    return 1;
}

u32 func_00204008(u32 task, u32 input) {
    btlSelectLinkedTargets(task, input, 1);
    return 1;
}

u32 func_00204028(u32 task, u32 input) {
    btlSelectLinkedTargets(task, input, 0);
    return 1;
}

u32 btlAppendEffectActorToCommandIndices(s32 task) {
    u32 actor;

    actor = btlGetEffectActor();
    btlAppendIndexListEntry(((BtlTask *)task)->targetList, actor);
    return 1;
}

s32 btlSelectTargetsBlockingElement(s32 actor, s32 mask) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (btlUnitBlocksElementQuery(btlGetIndexListEntry((void *)list, i), mask, 0x200) != 0) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

u32 func_00204198(void) {
    return 1;
}

s32 btlSelectTargetsPassingCheck(s32 actor, s32 action) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            s32 unit = btlGetIndexListEntry((void *)list, i);

            if (((BtlUnit *)unit)->flags & 0x200) {
                if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
                    flags[i] = 1;
                }
            } else {
                if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 1) {
                    flags[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

void btlSelectLinkedTargets(s32 actor, s32 input, s8 invert) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            BtlUnit *unit = (BtlUnit *)btlGetIndexListEntry((void *)list, i);

            if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
                if (invert == 0) {
                    if (unit->flags & 0x1000) {
                        flags[i] = 1;
                    }
                } else if (!(unit->flags & 0x1000)) {
                    flags[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->targetList, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->targetList, list);
        break;
    }
    btlFreeIndexList((void *)list);
}

extern s32 btlCanUseLinkedActor();


/* Resolve a command's linked actor, retaining each independent task fallback. */
BtlUnit *btlGetTargetUnitForLink(BtlLinkedCommand *command) {
    s32 kind = command->actionCode;

    if ((u32)(kind - 1) >= 0x25F) {
        return command->task->unit;
    }
    if (datCommandSelectors[kind * 2 + 1] != 1) {
        return command->task->unit;
    }
    if (command->linkedA == NULL && command->linkedB == NULL) {
        return command->task->unit;
    }
    if (btlCanUseLinkedActor(command) == 0) {
        return command->task->unit;
    }
    if (command->linkedA != NULL) {
        if (command->linkedB != NULL) {
            return command->task->unit;
        }
        if (!(command->linkedA->flags & 0x1000)) {
            return command->linkedA;
        }
    }
    if (command->linkedB != NULL) {
        if (!(command->linkedB->flags & 0x1000)) {
            return command->linkedB;
        }
    }
    return command->task->unit;
}

extern void btlUnitGetMuzzlePosVU(BtlUnit *);


void btlFaceLinkedTargetAndFlagDirection(u8 *command, u8 *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = btlGetTargetUnitForLink((BtlLinkedCommand *)command);
    target = (BtlUnit *)btlGetIndexListEntry(((BtlLinkedCommand *)command)->targetList, 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagMatchingUnitsDefeatCandidate(target->flags & 0x600);
    } else {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagUnitDefeatCandidate(target);
    }
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, targetPos);
    btlUnitFaceTarget(target, user);
    if (userPos[0] < targetPos[0]) {
        ((BtlLinkedCommand *)command)->flags |= 0x200;
    } else {
        ((BtlLinkedCommand *)command)->flags &= ~0x200;
    }
}

extern s32 func_001D6050(BtlUnit *, s32);
extern f32 func_001A47F0(BtlTask *);
extern f32 func_002F9F60(f32);
extern f32 func_002FA060(f32);
extern f32 func_002FA148(f32);
extern void func_002DD688(f32);
extern void func_001DB698();

/* Frame one unit approaching its target (DDS2 func_00217470 without the explicit angle): the pull-back and the
   swing angle interpolate with how far the command's state has advanced (ratio, capped at 1). */
void func_002045E8(BtlLinkedCommand *command, BtlCamState *out, f32 frontLift, f32 backLift) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 dir[4];
    f32 extent;
    f32 length;
    f32 span;
    f32 ratio;
    f32 angle;
    f32 width;

    user = btlGetTargetUnitForLink(command);
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    extent = user->reach * user->scale;
    span = func_001D6050(user, user->unkEC);
    span /= func_001A47F0(command->task);
    ratio = (f32)command->state / span;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    out->fov = command->camera.fov;
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    userPos[1] += user->height * user->scale * frontLift;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
    targetPos[1] += target->height * target->scale * backLift;
    VU0_LOAD_VF(vf10, targetPos);
    VU0_LOAD_VF(vf11, userPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    length *= ratio * (0.6f - 0.55f) + 0.55f;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    VU0_SCALE_VF_MFC1(vf10, length);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out->position);
    angle = ratio * 0.0f;
    angle += 0.6108652f;
    width = extent + length * func_002FA060(angle);
    length *= func_002F9F60(angle);
    length += width / func_002FA148(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (command->flags & 0x200) {
        angle = -angle;
    }
    func_002DD688(angle);
    VU0_LOAD_VF(vf10, dir);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
    func_001DB698(out);
}


extern void btlFlagAllUnitsDefeatCandidate(void);
extern void btlCopyMotionTransform();
extern void btlUnitFaceTarget(BtlUnit *, BtlUnit *);


/* Frame a two-unit exchange: place the camera pair between the units' muzzle positions and push it out far enough to see both. */
void btlBuildLinkedCommandCameraPair(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back, s8 mirror, s8 swapRoles, f32 sideScale, f32 backLift, f32 frontLift) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 userExtent;
    f32 targetExtent;
    f32 minDistance;
    f32 angle;
    f32 length;

    btlFlagAllUnitsDefeatCandidate();
    front->fov = command->camera.fov;
    if (swapRoles == 0) {
        user = btlGetTargetUnitForLink(command);
        target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    } else {
        target = btlGetTargetUnitForLink(command);
        user = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    }
    userExtent = user->reach * user->scale;
    targetExtent = target->reach * target->scale;
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    userPos[1] += user->height * user->scale * frontLift;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, targetPos);
    targetPos[1] += target->height * target->scale * backLift;
    if (targetPos[0] <= userPos[0]) {
        targetPos[0] = targetPos[0] + targetExtent * sideScale;
    } else {
        targetPos[0] = targetPos[0] - targetExtent * sideScale;
    }
    VU0_LOAD_VF(vf10, userPos);
    VU0_STORE_VF(vf10, front->position);
    VU0_LOAD_VF(vf11, targetPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    front->distance = length;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, front->direction);
    angle = front->fov * 1.3333333f * 0.5f;
    front->distance = front->distance + targetExtent * 1.5f / func_002FA148(angle);
    minDistance = userExtent / func_002FA148(angle);
    if (front->distance < minDistance) {
        front->distance = minDistance;
    }
    btlCopyMotionTransform(back, front);
    back->distance += 250.0f;
    if (mirror != 0) {
        func_001DB698(front);
        func_001DB698(back);
    }
    btlUnitFaceTarget(user, target);
    btlUnitFaceTarget(target, user);
}

u32 func_00204AC0(void) {
    return 0xffffffff;
}

s32 btlClassifyLinkedSkillRequest(s32 unused, s32 unit, s32 index) {
    s32 result = 0;
    if (!(((BtlUnit *)unit)->flags & 0x400)) {
        return result;
    }
    if (datCommandRecords[index * 0x38 + 2] == 2) {
        if (btlWouldUiValueFallBelowQuarter(unit, 0) != 0 && mdlFlagTest(0x802) != 0) {
            return 1;
        }
        return 4;
    }
    return 0;
}

u32 func_00204B40(void) {
    return 0xffffffff;
}

s32 btlFilterRestrictedCommand(s32 battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((((BtlUnit *)battler)->statBits & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

u8 btlIsCommandCodeF(u32 unused, s32 command) {
    return command == 0xf;
}

s32 btlSelectDisabledCommand(s32 battler) {
    if (battler == 0) {
        return 15;
    }
    return (((BtlUnit *)battler)->statBits & 0x2000) ? 15 : -1;
}

/* Command condition words and the restriction flags tested by this boss mode. */
typedef struct BtlCommandStatus {
    s32 first;
    s32 second;
    s32 third;
    u8 pad0C[0x1A];
    u16 restrictionFlags; /* 0x26 */
} BtlCommandStatus;

void btlTrackSpecialEnemyCommandRestrictionByTurn(u8 *unit, u8 *command) {
    u8 *battle;
    u8 *effect;
    s32 species;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    battle = (u8 *)btlGetRuntime();
    species = ((BtlUnit *)unit)->mode;
    effect = (u8 *)((BtlState *)battle)->effect;
    if (species < 0x105) {
        if (species >= 0x102 && (((BtlCommandStatus *)command)->restrictionFlags & 0x10)) {
            ((BtlUnit *)unit)->statBits |= 0x2000;
            *(s32 *)effect = ((BtlState *)battle)->turnCount;
        }
    }
    if (species == 0x102 && (((BtlCommandStatus *)command)->restrictionFlags & 0x100)) {
        if (((BtlCommandStatus *)command)->first < 0 || ((BtlCommandStatus *)command)->second < 0 ||
            ((BtlCommandStatus *)command)->third != 0) {
            if (*(s32 *)effect != ((BtlState *)battle)->turnCount) {
                ((BtlUnit *)unit)->statBits &= ~0x2000;
            }
        }
    }
}

s32 btlClearUnitRestrictionFlag(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    if (battle->mode != 2) {
        return -1;
    }
    unit = battle->units;
    while (unit != 0) {
        if (unit->flags & 1) {
            if (unit->mode == 0x104) {
                u16 entryFlags = unit->statBits;
                if (entryFlags & 0x2000) {
                    unit->statBits = entryFlags & ~0x2000;
                }
            }
        }
        unit = unit->next;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204D08);

s32 btlDisableNonBossUnits(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    s32 kind;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                kind = unit->mode;
                if (kind < 0x105) {
                    if (kind >= 0x102) {
                        if (unit->flags & 0x20) {
                            result = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit->flags & 2) {
                        if (!(unit->flags & 0xE0)) {
                            switch (unit->mode) {
                            case 0x102:
                            case 0x103:
                            case 0x104:
                                break;
                            default:
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->flags &= ~1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

void btlResetUnitPlacement(void) {
    u8 *unit = (u8 *)((BtlState *)btlGetRuntime())->units;

    if (unit == 0) {
        return;
    }
    do {
        if ((((BtlUnit *)unit)->flags & 1) != 0) {
            s32 species = ((BtlUnit *)unit)->mode;
            if (species < 0x105) {
                if (species >= 0x102) {
                    if (((BtlUnit *)unit)->statBits & 0x2000) {
                        ((BtlUnit *)unit)->bodyOffsetXBits = 0;
                        ((BtlUnit *)unit)->bodyOffsetY = -100.0f;
                        ((BtlUnit *)unit)->bodyOffsetZ = 60.0f;
                        ((BtlUnit *)unit)->bodyOffsetWBits = 0;
                        ((BtlUnit *)unit)->reach = 180.0f;
                        ((BtlUnit *)unit)->height = 220.0f;
                    } else {
                        btlInitializeEffectVectorsFromSourceRecords(unit, 1, species);
                    }
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    } while (unit != 0);
}

extern void btlRefreshUnitMotionSelection(void *);

void btlClearSpecialEnemyEntryFlags(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 species = ((BtlUnit *)unit)->mode;
                if (species >= 0x105) {
                    unit = (u8 *)((BtlUnit *)unit)->next;
                    continue;
                }
                if (species >= 0x102) {
                    ((BtlUnit *)unit)->statBits &= ~0x2000;
                    btlRefreshUnitMotionSelection(unit);
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
}

void func_00205070(void) {
    BtlUnit *target = NULL;
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    f32 position[4];
    f32 shift;

    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x105) {
                    target = unit;
                    break;
                }
            }
        }
    }
    if (target != NULL) {
        func_001D6300(target, position);
        shift = -position[0];
        position[0] = 0.0f;
        PCP_COPY_VECTOR(target->position, position);
        btlSetUnitPosition(target, position);
        for (unit = state->units; unit != NULL; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit != target) {
                        func_001D6300(unit, position);
                        position[0] += shift;
                        PCP_COPY_VECTOR(unit->position, position);
                        btlSetUnitPosition(unit, position);
                    }
                }
            }
        }
    }
}

s32 btlDisableUnitsIfSpeciesFlagged(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x105) {
                    if (unit->flags & 0x20) {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit->flags & 2) {
                        if (!(unit->flags & 0xE0)) {
                            if (unit->mode != 0x105) {
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->flags &= ~1;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

typedef struct BtlSlotEntry {
    u16 flags;
    u8 unk_02[2];
    u16 kind;
    u16 unk_06;
    u16 unk_08;
    u8 unk_0A[4];
    u16 unk_0E;
    u8 unk_10[0x12];
    u16 effectData[24];
    u8 unk_52[0x1A4 - 0x52];
} BtlSlotEntry;

typedef struct BtlSlotTable {
    u8 unk_000[0xA60];
    BtlSlotEntry entry[5];
} BtlSlotTable;

extern BtlSlotTable *datGameState;

/* The effect slot is reused for mode-specific work; keep each handler's payload view. */
void btlSelectSlotEntries(void) {
    s32 *effectState = (s32 *)((BtlState *)btlGetRuntime())->effect;
    BtlSlotEntry *slotEntry;
    BtlSlotEntry *readyEntry = NULL;
    BtlSlotEntry *activeEntry = NULL;
    u32 i;
    slotEntry = datGameState->entry;
    for (i = 0; i < 5; i++) {
        u16 flags = slotEntry->flags;
        effectState[i] = flags;
        if (flags & 1) {
            if (slotEntry->kind == 3) {
                readyEntry = slotEntry;
            }
            if (slotEntry->kind == 1) {
                slotEntry->flags = flags | 2;
                activeEntry = slotEntry;
            } else {
                slotEntry->flags = flags & ~2;
            }
        }
        slotEntry++;
    }
    activeEntry->unk_0E = 0;
    if (readyEntry != NULL) {
        readyEntry->unk_0E = 0;
    }
    memcpy((u8 *)effectState + 0x14, activeEntry->effectData, 0x30);
    for (i = 0; i < 24; i++) {
        activeEntry->effectData[i] = 0;
    }
    activeEntry->unk_06 = activeEntry->unk_08;
}

void btlRestoreSlotEntriesFromEffect(void) {
    s32 *effectState = (s32 *)((BtlState *)btlGetRuntime())->effect;
    BtlSlotEntry *activeEntry = NULL;
    u32 i;

    for (i = 0; i < 5; i++) {
        if (effectState[i] & 1) {
            if (effectState[i] & 2) {
                datGameState->entry[i].flags |= 2;
            } else {
                datGameState->entry[i].flags &= ~2;
            }
            if (datGameState->entry[i].kind == 1) {
                activeEntry = &datGameState->entry[i];
            }
        }
    }
    memcpy(activeEntry->effectData, (u8 *)effectState + 0x14, 0x30);
    activeEntry->unk_06 = activeEntry->unk_08;
}


void btlInitializeUnitDisplaySpeciesAndFlags(u8 *unit) {
    u8 *battle = (u8 *)btlGetRuntime();
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 0x200) {
        if (((BtlUnit *)unit)->mode == 1) {
            ((BtlUnit *)unit)->displaySpecies = 1;
            ((BtlUnit *)unit)->flags = flags | 0x1000;
            ((BtlUnit *)unit)->statBits |= 0x1000;
            if (((BtlState *)battle)->battleMode == 0x107) {
                ((BtlUnit *)unit)->stateFlags |= 0x200;
            }
        }
    } else {
        ((BtlUnit *)unit)->flags = flags | 0x1000;
        ((BtlUnit *)unit)->displaySpecies = 0x132;
    }
    ((BtlUnit *)unit)->stateFlags |= 0x20;
}

void btlNormalizeDefaultPlayerDisplaySpecies(s32 unit) {
    u16 mode;

    if (((BtlUnit *)unit)->flags & 0x200) {
        mode = ((BtlUnit *)unit)->mode;
        if (mode == 1) {
            if (((BtlUnit *)unit)->displaySpecies == mode) {
                ((BtlUnit *)unit)->displaySpecies = 0x11;
            }
        }
    }
}

u32 func_002055F0(void) {
    return 2;
}

extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);

extern void mdlFlagSet(u32);

extern void mdlFlagClear(u32);

s32 func_002055F8(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (((BtlUnit *)unit)->flags & 1) {
            if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0) {
                if (((BtlUnit *)unit)->flags & 0x200) {
                    mdlFlagSet(0x811);
                } else {
                    mdlFlagClear(0x811);
                }
                return 6;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return -1;
}

s32 btlInitializeResources(s32 unused, s32 resource) {
    btlInitMotionTransformFromVectors(resource, D_00360EE0, D_00360EF0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", btlInitResourcesWrap);

void btlBindEffectUnitAndClearStateFlags(BtlUnit *unit) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 flags = unit->flags & ~0x100;
    u16 status = unit->conditionFlags;
    flags &= ~8;
    status &= 0x4000;
    *(BtlUnit **)battle->effect = unit;
    unit->flags = flags;
    unit->conditionFlags = status;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205730);

void btlBeginEffectActorFadeOut(void) {
    BattleEffectState *effect = ((BtlState *)btlGetRuntime())->effect;
    BtlUnit *actor = *(BtlUnit **)effect;
    if (actor != 0) {
        u32 state = actor->stateFlags;
        u32 flags = actor->flags;
        state &= ~0x80;
        state &= ~0x100;
        flags |= 0x100;
        *(BtlUnit **)effect = 0;
        actor->flags = flags;
        actor->stateFlags = state;
        btlRefreshUnitMotionSelection(actor);
        actor->flags |= 8;
        /* +0x10 is a float in this effect variant, but a u32 in BattleEffectState. */
        *(f32 *)((u8 *)effect + 0x10) = -125.0f;
        effect->speed = 20.0f;
    }
}

void btlResetEffectState(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    BattleEffectState *data = ((BtlState *)battle)->effect;
    data->active = 1;
    data->speed = 20.0f;
    data->flags = 0;
    data->phase = 0;
    data->value = 0;
    data->timer = 0;
    data->effect = 0;
    data->actor = 0;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5D50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205918);

s32 btlCheckActiveEffectForSpecialTarget(BtlUnit *actor, BtlUnit *target, s32 command, s32 bits) {
    BattleEffectState *effect;
    if (!(target->flags & 0x400)) {
        return 0;
    }
    switch (target->mode) {
    case 0x107:
    case 0x108:
        break;
    default:
        return 0;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->active != 1) {
        return 0;
    }
    if (actor->flags & 0x200) {
        if (command != 0) {
            if (datCommandRecords[command * 0x38 + 8] == 0) {
                return 0;
            }
        }
    }
    return (bits * 2) & 4;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205BD8);

void func_00205EE0(void) {
    func_00205BD8();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205EF8);

s32 btlIsEffectPhaseInRange(s32 unused, s32 value) {
    BattleEffectState *effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->active != 1) {
        return 0;
    }
    switch (value) {
    case 0x12:
    case 0x13:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206180);

s32 btlAdjustDamageKind(BtlUnit *unit, s32 damageKind) {
    u32 id;
    if (!(unit->flags & 1)) {
        return damageKind;
    }
    if (!(unit->flags & 0x400)) {
        return damageKind;
    }
    if (((BtlState *)btlGetRuntime())->effect->active != 1) {
        return damageKind;
    }
    id = unit->mode;
    if (id == 0x124) {
        return damageKind;
    }
    switch (damageKind) {
    case 1:
        if (id == 0x107) {
            return 1;
        }
        if (id == 0x108) {
            return 0xC8;
        }
        break;
    case 2:
        if (id == 0x107) {
            return 0x66;
        }
        if (id == 0x108) {
            return 0xC8;
        }
        break;
    default:
        if (id == 0x107) {
            return damageKind + 0x64;
        }
        if (id == 0x108) {
            return damageKind + 0xC8;
        }
        break;
    }
    return damageKind;
}

u8 *btlFindFlaggedSpecialSpeciesUnit(s32 category, s32 species) {
    u8 *unit;
    if (category != 1) {
        return 0;
    }
    if (species != 0x124) {
        return 0;
    }
    unit = (u8 *)((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 2) {
                    if (((BtlUnit *)unit)->species == 0x124) {
                        return unit;
                    }
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlGetAdjustedUnitDisplaySpecies(s32 unit) {
    s32 mode = ((BtlUnit *)unit)->mode;

    if ((mode >= 0x107) && ((mode < 0x109) || (mode == 0x124))) {
        return 0x124;
    }
    return ((BtlUnit *)unit)->displaySpecies;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206450);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206608);

s32 btlGetEffectTaskActorMatchCode(u8 *task) {
    BattleEffectState *effect;
    if ((((BtlTask *)task)->flags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    return effect->actor == (u32)((BtlTask *)task)->unit ? 12 : -1;
}

extern s32 btlCreateCommandSoundUpdateTask();

extern s32 btlCreateSecondaryCommandSoundTask();

extern s32 btlCreateCommandSoundTask();

extern s32 btlCreateEffObjB();

extern u8 *fldCreateSceneGroupAction(BtlTask *, u32, s32);

/* Start the selected actor's finale task group; its scene action carries a 22-tick start delay. */
s32 btlEffectTaskStartFinale(BtlTask *task) {
    BattleEffectState *effect;
    BtlRuntimeTask *group;

    if ((task->flags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->actor != (u32)task->unit) {
        return -1;
    }
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask(task, 9));
    btlStartTask(btlCreateEffObjB(task->unit, 0xB4));
    group = (BtlRuntimeTask *)fldCreateSceneGroupAction(task, 0x64, 1);
    group->startDelay = 0x16;
    btlStartTask(group);
    effect->phase = 1;
    return (task->unit->conditionFlags & 0x480) ? 0x18 : 0x1A;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002069C0);

s32 btlIsEffectActor(s32 actor) {
    s32 effectActor;

    effectActor = ((BtlState *)btlGetRuntime())->effect->actor;
    if (effectActor == 0) {
        return 0;
    }
    return (effectActor ^ actor) == 0;
}

extern s32 btlHasEffectActor(void);

s32 btlGetSoleTargetKind(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *target;
    BtlUnit *unit;
    BtlUnit *last;
    s32 count;
    if (btlHasEffectActor() == 0) {
        return -1;
    }
    target = (BtlUnit *)btlGetEffectActor();
    last = NULL;
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x200) {
                if (!(unit->flags & 0xE0)) {
                    if (!(unit->conditionFlags & 0x800)) {
                        count++;
                        last = unit;
                    }
                }
            }
        }
    }
    if (count == 1 && (last == NULL || last == target)) {
        return 7;
    }
    return -1;
}

s32 btlHasDifferentActiveTarget(u32 target) {
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    return btlGetEffectActor() != target;
}

extern s32 btlHasEffectActor(void);

s32 btlSetLinkFlagOff(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x107) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            *other->ext->flags &= ~1;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->flags & 4) {
            *other->ext->flags &= ~1;
        } else {
            *other->ext->flags |= 1;
        }
        return 0;
    }
    return 1;
}

extern s32 btlHasEffectActor(void);

s32 btlSetLinkFlagOn(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x107) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            *other->ext->flags |= 1;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->flags & 4) {
            *other->ext->flags &= ~1;
        } else {
            *other->ext->flags |= 1;
        }
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002072F0);

extern void *btlCreateUnitFadeOutTask(void *, s32, s32);

extern s64 btlStartTask(void *);

s32 btlTryScheduleMarkedUnitTask(u8 *unit) {
    u8 *battle = (u8 *)btlGetRuntime();
    u8 *effect;
    u8 *other;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return 1;
    }
    effect = (u8 *)((BtlState *)battle)->effect;
    if (((BattleEffectState *)effect)->active != 0) {
        return 1;
    }
    if (((BtlUnit *)unit)->species == 0x124) {
        return 1;
    }
    other = (u8 *)((BtlState *)battle)->units;
    while (other != 0) {
        u32 flags = ((BtlUnit *)other)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (other != unit) {
                    if (flags & 0xe0) {
                        return 1;
                    }
                }
            }
        }
        other = (u8 *)((BtlUnit *)other)->next;
    }
    btlStartTask(btlCreateUnitFadeOutTask(unit, 8, 10));
    ((BtlUnit *)unit)->flags &= ~0x100;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207718);

extern void btlSetActorEffectParameterOrMuzzlePosition();

extern void btlInterpolateVectorStep();


/* Arm action 0x189 once, aiming its camera at the active mode-0x107 target. */
s32 btlUnitStartAimAtTarget(BtlLinkedCommand *command) {
    u8 *battle = (u8 *)btlGetRuntime();
    u8 *target;
    u32 flags;
    f32 distance;

    if (command->actionCode == 0x171) {
        return 1;
    }
    if (command->actionCode != 0x189) {
        return 0;
    }
    for (target = (u8 *)((BtlState *)battle)->units; target != 0; target = (u8 *)((BtlUnit *)target)->next) {
        flags = ((BtlUnit *)target)->flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 2) != 0) {
                    if (((BtlUnit *)target)->mode == 0x107) {
                        break;
                    }
                }
            }
        }
    }
    if (target == 0) {
        return 1;
    }
    if (command->state != 0x1E || command->motionProgress != 0) {
        return 1;
    }
    btlCopyMotionTransform(&command->frontCamera, &command->camera);
    command->motionParameter = 10.0f;
    command->motionProgress = 1;
    command->state = 0;
    btlSetActorEffectParameterOrMuzzlePosition(target, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, command->backCamera.position);
    command->backCamera.position[1] += 150.0f;
    btlInterpolateVectorStep(&command->frontCamera);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, command->backCamera.position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    command->backCamera.distance = distance;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, command->backCamera.direction);
    command->backCamera.distance += 45.0f;
    func_001DB698(command->backCamera.position);
    return 1;
}

s32 btlIsSpecialActionKind(BtlLinkedCommand *command) {
    switch (command->actionCode) {
    case 0x171:
        return 1;
    case 0x189:
        return 1;
    default:
        return 0;
    }
}

u32 btlGetEffectActive(void) {
    s32 battle;
    s32 effect;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->battleMode != 0x108) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return ((BattleEffectState *)effect)->active;
}

s32 btlHasEffectActor(void) {
    s32 absent = 0;
    s32 battle;
    s32 effect;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->battleMode != 0x108) {
        return absent;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return absent;
    }
    return (((BattleEffectState *)effect)->actor != 0);
}

u32 btlGetEffectValue(void) {
    s32 battle;
    s32 effect;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->battleMode != 0x108) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return ((BattleEffectState *)effect)->value;
}

u32 btlGetEffectActor(void) {
    s32 battle;

    battle = btlGetRuntime();
    return ((BtlState *)battle)->effect->actor;
}

s32 btlIsSpecialEnemyEffectLinkSatisfied(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    u8 *unit = (u8 *)((BtlState *)battle)->units;
    u8 *effect = (u8 *)((BtlState *)battle)->effect;
    while (unit != 0) {
        if ((((BtlUnit *)unit)->flags & 0x400) &&
            ((BtlUnit *)unit)->mode == 0x107) {
            break;
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (unit == 0) {
        return 1;
    }
    if (!(((BtlUnit *)unit)->flags & 0xe0)) {
        return 0;
    }
    return *(u8 **)(effect + 4) == unit;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207CA0);

extern s8 D_003BD86C;

extern s32 sdfNamedChunkFindId(void *, void *);

extern void func_00207CA0(s32, s32);
/* Named chunk indirection follows the same +0x18/+0x0C layout as DDS2. */
typedef struct BtlNamedChunkData {
    u8 pad00[0xC];
    void **entries; /* 0x0C: indexed chunk node pointers */
} BtlNamedChunkData;

typedef struct BtlNamedChunkDescriptor {
    BtlNamedChunkData *data;
    u8 pad04[0x18];
    s32 argument; /* 0x1C */
} BtlNamedChunkDescriptor;

typedef struct BtlNamedChunkHolder {
    u8 pad00[0x18];
    BtlNamedChunkDescriptor *chunk;
} BtlNamedChunkHolder;

s32 btlDispatchNamedChunkNode(void *query) {
    u8 *effect = (u8 *)((BtlState *)btlGetRuntime())->effect;
    u8 *unit = *(u8 **)effect;
    u8 *model;
    u8 *descriptor;
    s32 index;
    s32 selected;
    if (unit == 0) {
        return 1;
    }
    if ((((BtlUnit *)unit)->flags & 2) == 0) {
        return 1;
    }
    model = (u8 *)((BtlUnit *)unit)->ext->flags;
    descriptor = (u8 *)((BtlNamedChunkHolder *)model)->chunk;
    index = sdfNamedChunkFindId(descriptor, query);
    if (index == -1) {
        return 1;
    }
    selected = (s32)((BtlNamedChunkDescriptor *)descriptor)->data->entries[index];
    D_003BD86C = 1;
    func_00207CA0(selected, ((BtlNamedChunkDescriptor *)descriptor)->argument);
    return D_003BD86C;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207E68);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207FF0);

s32 btlAdjustSpeciesAnimation(u8 *unit, s32 animation) {
    s32 species;
    u32 flags = ((BtlUnit *)unit)->flags;
    if ((flags & 1) == 0) {
        return animation;
    }
    if ((flags & 0x400) == 0) {
        return animation;
    }
    species = ((BtlUnit *)unit)->mode;
    switch (species) {
    case 0x12e:
        return animation + 100;
    case 0x10a:
        return animation + 300;
    case 0x12f:
        return animation + 200;
    default:
        return animation;
    }
}

void btlMarkSpecialUnit(u8 *unit) {
    s32 species;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    species = ((BtlUnit *)unit)->mode;
    if (species != 0x10a) {
        if (species < 0x10a) {
            return;
        }
        if (species >= 0x130) {
            return;
        }
        if (species < 0x12e) {
            return;
        }
    }
    ((BtlUnit *)unit)->stateFlags |= 0x200;
}

u8 *btlGetReadyUnitForSpecies(s32 mode, u32 species) {
    u8 *unit;
    if (mode != 1) {
        return NULL;
    }
    switch (species) {
    case 0x10A:
    case 0x12E:
    case 0x12F:
        break;
    default:
        return NULL;
    }
    unit = *(u8 **)((BtlState *)btlGetRuntime())->effect;
    if (unit == NULL) {
        return NULL;
    }
    return (((BtlUnit *)unit)->flags & 2) ? unit : NULL;
}

extern u64 btlAdvanceRuntimeSequenceCounter(void);

/* Create/load the special unit only for an empty slot. A populated slot returns
 * a fresh sequence ID without launching a task; a nonzero prerequisite waits
 * until that task handle is gone.
 */
u64 btlCreateSpecialUnitAndLoadModel(u64 prerequisiteHandle) {
    u8 **slot = (u8 **)((BtlState *)btlGetRuntime())->effect;
    u8 *model = *slot;
    BtlRuntimeTask *entry;
    if (model != 0) {
        return btlAdvanceRuntimeSequenceCounter();
    }
    model = (u8 *)btlCreateUnit();
    *slot = model;
    func_001A1990(model + 0x120, 0x10a);
    func_00207E68();
    entry = (BtlRuntimeTask *)btlCreateModelLoadPollTask(*slot, 1, 0x10a, 0);
    if (prerequisiteHandle != 0) {
        entry->conditionHandle = prerequisiteHandle;
        entry->conditionKind = BTL_TASK_CONDITION_HANDLE_GONE;
    }
    btlStartTask(entry);
    return entry->handle;
}

void btlDestroySpecialUnitSlot(void) {
    s32 *data;
    s32 unit;

    unit = btlGetRuntime();
    data = (s32 *)((BtlState *)unit)->effect;
    unit = *data;
    if (unit != 0) {
        btlDestroyUnit(unit);
        *data = 0;
    }
}

/* Launch a subtask from the active slot and return its new scheduler handle.
 * Zero leaves the constructor's condition intact; otherwise wait for the
 * prerequisite task to disappear. The fixed high-bit owner value is preserved.
 */
u64 btlStartSubtaskWithInput(u64 prerequisiteHandle) {
    u8 *subtaskSlot = (u8 *)((BtlState *)btlGetRuntime())->effect;
    BtlRuntimeTask *task = (BtlRuntimeTask *)func_001D9038(*(void **)subtaskSlot, 12);
    if (prerequisiteHandle != 0) {
        task->conditionHandle = prerequisiteHandle;
        task->conditionKind = BTL_TASK_CONDITION_HANDLE_GONE;
    }
    task->ownerId = 0x8000000000000003ULL;
    btlStartTask(task);
    return task->handle;
}

extern s32 btlDispatchNamedChunkNode(void *);

extern s8 D_003BB880[];

extern s8 D_003BB888[];

extern s8 D_003BB890[];

void btlUpdateReadyUnits(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    s32 ready;
    if (state->battleFlags & 0x80000) {
        unit = state->units;
        while (unit != NULL) {
            if (unit->flags & 0x400) {
                if (unit->flags & 0x80) {
                    switch (unit->species) {
                    case 0x10A:
                        ready = btlDispatchNamedChunkNode(D_003BB880);
                        break;
                    case 0x12E:
                        ready = btlDispatchNamedChunkNode(D_003BB888);
                        break;
                    case 0x12F:
                        ready = btlDispatchNamedChunkNode(D_003BB890);
                        break;
                    default:
                        ready = 1;
                        break;
                    }
                    if (ready != 0) {
                        unit->flags = (unit->flags & ~0x80) | 0x40;
                    }
                }
            }
            unit = unit->next;
        }
    }
}

s32 btlCheckEnemyUnitReadiness(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    s32 count = 0;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if ((flags & 0xe0) == 0) {
                    count++;
                } else {
                    ((BtlUnit *)unit)->flags = ((flags & ~1) | 0x80) & ~0x40;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return count == 0;
}

u32 func_00208660(void) {
    return 0xffffffff;
}

s32 func_00208668(s32 unit) {
    return (((s32)((BtlUnit *)unit)->flags & 0x200) < 1);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208678);

s32 btlRemapSpecialUnitCommandIndex(u8 *unit, s32 index) {
    u32 flags = ((BtlUnit *)unit)->flags;
    if ((flags & 0x400) == 0) {
        return index;
    }
    if (((BtlUnit *)unit)->mode != 0x10d) {
        return index;
    }
    if ((flags & 1) == 0) {
        return -1;
    }
    {
        u8 *data = (u8 *)((BtlState *)btlGetRuntime())->effect;
        if (index >= 17) {
            return index;
        }
        if (index < 15) {
            return index;
        }
        return *(u16 *)(data + 4) != 0 ? 16 : 15;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208860);

void btlTriggerSpecialUnitActionAndResetPose(s32 unit) {
    if (((BtlUnit *)unit)->mode != 0x10d) {
        return;
    }
    btlGetRuntime();
    btlInitializeEffectVectorsFromSourceRecords(unit, 1, 0x11d);
    func_00208860(unit, 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208A50);

void func_00208C20(void) {
    func_001F53F0();
}

u32 btlIsInSpecialModeRange(u32 unused0, u32 unused1, s32 action) {
    if ((0x171 < action) && ((action < 0x175 || (action == 0x1a1)))) {
        return 200;
    }
    return 100;
}

u32 btlGetSpecialModeEffectValue(void) {
    s32 battle;
    s32 effect;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->battleMode != 0x116) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return *(u16 *)(effect + 0x4);
}

extern void btlInitializeEffectVectorsFromSourceRecords();

extern void func_001F53F0(void);

void btlTriggerSpecialUnitAction(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    u8 *unit = (u8 *)((BtlState *)battle)->units;
    while (unit != 0) {
        if (((BtlUnit *)unit)->flags & 0x400) {
            if (((BtlUnit *)unit)->mode == 0x10d) {
                break;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (unit != 0) {
        ((BtlState *)battle)->unk_5B8 = 0;
        btlInitializeEffectVectorsFromSourceRecords(unit, 1, 0x11d);
        func_001F53F0();
    }
}

struct EffRandState;
extern u32 effMiscRand(struct EffRandState *);

extern char D_003A5D98[];

extern struct EffRandState effSharedRandomState;

/* Boss-selection view of battle->effect: +0x0C is a full-width lookup ID
 * here, unlike the timer/phase view in BattleEffectState. */
typedef struct BtlBossEffectPayload {
    s32 actor;
    u32 color;
    s32 options;
    s32 selectedId;
} BtlBossEffectPayload;

void btlInitRandomBossSelection(void) {
    u8 *data = (u8 *)((BtlState *)btlGetRuntime())->effect;
    ((BtlBossEffectPayload *)data)->color = 0x80808080;
    ((BtlBossEffectPayload *)data)->options = 0;
    ((BtlBossEffectPayload *)data)->selectedId = effMiscRand(&effSharedRandomState) % 6;
    btlBossDebugPrintf((s32)D_003A5D98, ((BtlBossEffectPayload *)data)->selectedId);
}

s32 btlFilterBossCommandBySelection(u8 *unit, s32 command) {
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 1) {
        if (flags & 0x400) {
            u8 *effect = (u8 *)((BtlState *)btlGetRuntime())->effect;
            if (((BtlUnit *)unit)->lookupId == ((BtlBossEffectPayload *)effect)->selectedId) {
                if (((BtlBossEffectPayload *)effect)->options & 4) {
                    return command;
                }
            }
            return -1;
        }
    }
    return command;
}

void func_00208E10(unit)
u8 *unit;
{
    u8 *battleData;
    u8 *entry;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    battleData = (u8 *)((BtlState *)btlGetRuntime())->effect;
    ((BtlUnit *)unit)->stateFlags |= 0x200;
    ((BtlUnit *)unit)->height = 100.0f;
    ((BtlUnit *)unit)->reach = 25.0f;
    ((BtlUnit *)unit)->unkB8 = 100.0f;
    ((BtlUnit *)unit)->unkBC = 25.0f;
    if (((BtlBossEffectPayload *)battleData)->selectedId != ((BtlUnit *)unit)->lookupId) {
        entry = (u8 *)btlFindUnitByActor((s32)unit);
        if (entry != 0) {
            *(u16 *)(entry + 4) = 0;
            ((BtlUnit *)unit)->mode = 0x10f;
        }
    }
}

void func_00208EA8(void) {
    func_00208E10();
}

void *btlFindActiveMember(s32 group, s32 type) {
    s32 *unit;
    if (group != 1) {
        return 0;
    }
    if (type != 0x10e) {
        return 0;
    }
    unit = *(s32 **)((BtlState *)btlGetRuntime())->effect;
    if (unit == 0) {
        return 0;
    }
    return (unit[0x110 / 4] & 2) ? unit : 0;
}

/* Same optional task prerequisite as the special-unit path; an occupied slot
 * consumes a fresh sequence ID rather than returning an existing task handle.
 */
u64 btlEnsureEffectUnitModelLoadTask(u64 prerequisiteHandle) {
    u8 **slot = (u8 **)((BtlState *)btlGetRuntime())->effect;
    u8 *model = *slot;
    BtlRuntimeTask *entry;
    if (model != 0) {
        return btlAdvanceRuntimeSequenceCounter();
    }
    model = (u8 *)btlCreateUnit();
    *slot = model;
    func_001A1990(model + 0x120, 0x10e);
    entry = (BtlRuntimeTask *)btlCreateModelLoadPollTask(*slot, 1, 0x10e, 0);
    if (prerequisiteHandle != 0) {
        entry->conditionHandle = prerequisiteHandle;
        entry->conditionKind = BTL_TASK_CONDITION_HANDLE_GONE;
    }
    btlStartTask(entry);
    return entry->handle;
}

void btlDestroyActiveMemberSlot(void) {
    s32 *data;
    s32 unit;

    unit = btlGetRuntime();
    data = (s32 *)((BtlState *)unit)->effect;
    unit = *data;
    if (unit != 0) {
        btlDestroyUnit(unit);
        *data = 0;
    }
}

extern s32 mdlGetNodeField2C(s32, s32);

extern void evtSetUnitAlphaTransition(void *, s32, s32);

void btlStepFocusAngle(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 *slot;
    BtlUnit *player;
    BtlUnit *unit;
    if (!(battle->battleFlags & 0x80000)) {
        return;
    }
    slot = (u32 *)battle->effect;
    player = *(BtlUnit **)slot;
    if (player == NULL) {
        return;
    }
    if (!(player->flags & 2)) {
        return;
    }
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->lookupId == slot[3]) {
                    break;
                }
            }
        }
    }
    if (unit == NULL) {
        return;
    }
    mdlGetNodeField2C((s32)player->ext->flags, 0);
    if (slot[2] & 4) {
        if ((slot[1] & 0xFF000000) != 0x80000000) {
            slot[1] += 0x10000000;
        }
    } else {
        if (slot[1] & 0xFF000000) {
            slot[1] += 0xF0000000;
        }
    }
    evtSetUnitAlphaTransition(((BtlUnit *)*(u32 **)slot)->ext, 0, slot[1]);
}

typedef struct BtlEffectTarget {
    u8 pad00[8];
    u32 flags;    /* 0x08 */
    s32 targetId; /* 0x0C */
} BtlEffectTarget;

s32 btlCheckLinkedActionEffectTarget(BtlUnit *actor, BtlUnit *target, s32 command) {
    BtlEffectTarget *effect = (BtlEffectTarget *)((BtlState *)btlGetRuntime())->effect;
    s32 unit;
    u32 blocked;

    if (!(actor->flags & 0x200)) {
        return 0;
    }
    if (!(target->flags & 0x400)) {
        return 0;
    }
    if (datCommandSelectors[command * 2 + 1] != 1) {
        unit = btlFindUnitByActor((s32)actor);
        if (unit == 0) {
            return 0;
        }
        blocked = func_001A3360(unit, 0, 0);
    } else {
        blocked = datCommandRecords[command * 0x38 + 8];
    }
    if (blocked != 0) {
        return 4;
    }
    if (target->lookupId != effect->targetId) {
        return 4;
    }
    effect->flags |= 1;
    return 0;
}

u32 btlSelectResponseCodeForEnemyFlag(u32 unused, s32 unit) {
    u32 result;

    result = 4;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        result = 0;
    }
    return result;
}

extern char D_003A5DB8[];

s32 btlSwapRandomBossSelection(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlBossEffectPayload *effect = (BtlBossEffectPayload *)battle->effect;
    s32 options;
    BtlUnit *actor;
    BtlUnit *unit;
    BtlUnit *oldUnit;
    BtlUnit *newUnit;
    s32 entry;
    u32 newId;
    u32 unitFlags;

    options = effect->options;
    effect->options = options & ~2;
    if (battle->mode != 1) {
        return -1;
    }
    actor = (BtlUnit *)effect->actor;
    if (actor == NULL) {
        return -1;
    }
    if ((actor->flags & 2) == 0) {
        return -1;
    }
    if ((options & 1) == 0) {
        return -1;
    }

    newId = effMiscRand(&effSharedRandomState) % 6;
    if (newId == effect->selectedId) {
        newId = (newId + 1) % 6;
    }
    oldUnit = NULL;
    newUnit = NULL;
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        unitFlags = unit->flags;
        if (unitFlags & 1) {
            if (unitFlags & 0x400) {
                oldUnit = unit->lookupId == effect->selectedId ? unit : oldUnit;
                newUnit = unit->lookupId == newId ? unit : newUnit;
            }
        }
    }
    if (oldUnit->flags & 0x20) {
        return -1;
    }

    effect->selectedId = newId;
    if (oldUnit != newUnit) {
        newUnit->mode = 0x10E;
        oldUnit->mode = 0x10F;
        newUnit->hp = oldUnit->hp;
        newUnit->unk_12A = oldUnit->unk_12A;
        newUnit->conditionFlags = oldUnit->conditionFlags;
        oldUnit->conditionFlags = 0;
        oldUnit->hp = oldUnit->maxHp;
        oldUnit->unk_12A = *(u16 *)((u8 *)oldUnit + 0x12C);
        entry = btlFindUnitByActor((s32)newUnit);
        *(u16 *)(entry + 4) = 1;
        entry = btlFindUnitByActor((s32)oldUnit);
        *(u16 *)(entry + 4) = 0;
    }
    btlBossDebugPrintf((s32)D_003A5DB8, oldUnit->lookupId, newUnit->lookupId, oldUnit, newUnit);
    effect->options = ((effect->options & ~4) | 2) & ~1;
    return -1;
}

s32 btlFindSubsequentTurnScript(void) {
    s32 battle;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->turnCount == 0) {
        return -1;
    }
    if ((((BtlState *)battle)->battleFlags & 0x800) != 0) {
        return -1;
    }
    if (((BtlState *)battle)->mode != 1) {
        return -1;
    }
    return btlFindScriptResource(D_003BB898);
}

s32 btlCheckSelectedBossUnitFlag(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    u8 *unit = (u8 *)((BtlState *)battle)->units;
    u8 *effect = (u8 *)((BtlState *)battle)->effect;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (((BtlBossEffectPayload *)effect)->selectedId == ((BtlUnit *)unit)->lookupId) {
                    break;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return (((BtlUnit *)unit)->flags & 0x20) ? 1 : -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209528);

extern void func_001DF358(void *, void *);

/* Event entry carrying a battle task and its action kind. */
typedef struct BtlEventEntry {
    u8 pad00[0xF0];
    u32 flags;
    BtlTask *task; /* 0xF4 */
    u8 padF8[0xC];
    u32 kind;      /* 0x104 */
} BtlEventEntry;

s32 btlDispatchEligibleBossEvent(u8 *entry) {
    BtlTask *task = ((BtlEventEntry *)entry)->task;
    u8 *data;
    u32 kind;
    if (task == 0) {
        return 1;
    }
    if ((task->unit->flags & 0x400) == 0) {
        return 1;
    }
    data = (u8 *)((BtlState *)btlGetRuntime())->effect;
    if (((BtlBossEffectPayload *)data)->options & 4) {
        return 1;
    }
    kind = ((BtlEventEntry *)entry)->kind;
    if (kind < 9) {
        if (kind >= 4) {
            func_001DF358(entry, entry);
            ((BtlEventEntry *)entry)->flags |= 0x1000;
            return 0;
        }
    }
    return 1;
}

s32 btlCheckAttachedMember(u8 *entry) {
    u8 *unit = (u8 *)((BtlEventEntry *)entry)->task;
    if (unit == 0) {
        return 1;
    }
    if ((((BtlTask *)unit)->unit->flags & 0x400) == 0) {
        return 1;
    }
    {
        s32 *data = (s32 *)((BtlState *)btlGetRuntime())->effect;
        s32 flags = ((BtlBossEffectPayload *)data)->options & 4;
        if (flags) {
            return 1;
        }
        return 0;
    }
}

s32 btlCheckBossOptionAllowed(u8 *unit) {
    s32 *data;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return -1;
    }
    data = (s32 *)((BtlState *)btlGetRuntime())->effect;
    return (((BtlBossEffectPayload *)data)->options & 4) ? -1 : 0;
}

s32 btlRemapSelectedBossCommand(u8 *unit, s32 command, u8 mode) {
    u8 *effect;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return command;
    }
    effect = (u8 *)((BtlState *)btlGetRuntime())->effect;
    if (((BtlUnit *)unit)->lookupId != ((BtlBossEffectPayload *)effect)->selectedId) {
        return -1;
    }
    if (command == 13) {
        return -1;
    }
    if (command == 10) {
        return 0;
    }
    if (command == 2) {
        return 0;
    }
    if (command == 11) {
        return mode == 1 ? 14 : command;
    }
    return command;
}

extern void btlFlagUnitDefeatCandidate(BtlUnit *);

typedef struct BtlEffectLink {
    BtlUnit *actor;
    u32 unk4;
    u32 flags; /* 8 */
    s32 code;  /* 0xC */
} BtlEffectLink;

void btlSyncEffectActorToUnit(BtlUnit *unit, u8 *task) {
    BtlEffectLink *effect;
    BtlUnit *target;
    BtlUnit *actor;

    if (unit->flags & 0x400) {
        effect = (BtlEffectLink *)((BtlState *)btlGetRuntime())->effect;
        target = effect->actor;
        if (target != 0) {
            if (target->flags & 2) {
                if (unit->lookupId == effect->code) {
                    if ((*(u16 *)(task + 0x26) & 0x800) == 0) {
                        effect->flags |= 4;
                        PCP_COPY_VECTOR(target->position, unit->position);
                        btlSetUnitPosition(target, (u8 *)unit + 0x60);
                        actor = effect->actor;
                        PCP_COPY_VECTOR(actor->rotation, unit->rotation);
                        btlSetUnitRotation(actor, (u8 *)unit + 0x70);
                        btlFlagUnitDefeatCandidate(effect->actor);
                    }
                }
            }
        }
    }
}

u32 btlGetSelectedBossEffectId(void) {
    s32 battle;
    s32 effect;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->battleMode != 0x10b) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return ((BtlBossEffectPayload *)effect)->selectedId;
}

s32 btlHasBossEffectOption(void) {
    s32 absent = 0;
    s32 battle;
    s32 effect;

    battle = btlGetRuntime();
    if (((BtlState *)battle)->battleMode != 0x10b) {
        return absent;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return absent;
    }
    return ((((BtlBossEffectPayload *)effect)->options & 2) > 0);
}

/* Effect-data overlay for the one-shot scripted resource trigger. */
typedef struct BtlEventTriggers {
    s8 pending;       /* 0x00: starts the trigger */
    s8 resourceReady; /* 0x01: consumed when the resource is requested */
} BtlEventTriggers;

void btlArmEventResourceTrigger(void) {
    u8 *data;
    s32 battle;

    battle = btlGetRuntime();
    data = (u8 *)((BtlState *)battle)->effect;
    ((BtlEventTriggers *)data)->resourceReady = 1;
    ((BtlEventTriggers *)data)->pending = 0;
}

extern void fldAppendSceneGroupHandle();

extern void btlAppendIndexListEntry();

typedef struct BtlMarkState {
    u8 marked;  /* 0 */
    s8 variant; /* 1 */
} BtlMarkState;

s32 btlPickRandomMarkedTask(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlMarkState *mark = (BtlMarkState *)state->effect;
    BtlTask *candidates[16];
    BtlTask *task;
    BtlUnit *unit;
    s32 count;

    if (state->mode != 1) {
        return -1;
    }
    count = 0;
    for (task = state->tasks; task != 0; task = task->next) {
        if (task->flags & 8) {
            unit = task->unit;
            if (unit->flags & 1) {
                if (unit->flags & 0x200) {
                    if ((unit->conditionFlags & 0x7FFF) == 0x2000) {
                        candidates[count++] = task;
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    task = candidates[effMiscRandMod(0, count)];
    unit = task->unit;
    fldAppendSceneGroupHandle(task);
    task->result = 2;
    if (mark->variant != 0) {
        task->arg = 0xD1;
    } else {
        task->arg = 0xD2;
    }
    btlAppendIndexListEntry(task->targetList, (s32)unit);
    mark->marked = 1;
    return -1;
}

s32 btlConsumeReadyEventScriptResource(void) {
    s32 absent = -1;
    s32 data;

    data = (s32)((BtlState *)btlGetRuntime())->effect;
    if (((BtlEventTriggers *)data)->pending != 0) {
        if (((BtlEventTriggers *)data)->resourceReady != 0) {
            ((BtlEventTriggers *)data)->resourceReady = 0;
            return btlFindScriptResource(D_003BB8A0);
        }
        ((BtlEventTriggers *)data)->pending = 0;
        return -1;
    }
    return absent;
}

void btlClearEventEffectValue(void) {
    s32 battle;

    battle = btlGetRuntime();
    *(u16 *)((BtlState *)battle)->effect = 0;
}

s32 btlRemapBossAction(BtlUnit *unit, s32 action, u8 option) {
    u32 flags = unit->flags;
    u16 unitMode;
    if (!(flags & 0x400)) {
        return action;
    }
    if (!(flags & 1)) {
        return -1;
    }
    unitMode = unit->mode;
    if ((u16)(unitMode - 0x119) >= 2) {
        return -1;
    }
    btlGetRuntime();
    if (action == 10) {
        return 0;
    }
    if (action == 11) {
        switch (unit->mode) {
        case 0x11A:
            return option != 1;
        default:
            return option != 1;
        }
    }
    return action;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5D98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5DB8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209C90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209EB8);

u32 btlGetEventEffectValue(void) {
    s32 battle;

    battle = btlGetRuntime();
    return *(u16 *)((BtlState *)battle)->effect;
}

s32 btlHasFirstSpecialEnemySpecies(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    u16 species = 0;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                species = ((BtlUnit *)unit)->mode;
                if ((u16)(species - 0x119) < 2) {
                    break;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (unit == 0) {
        return 0;
    }
    return species == 0x119;
}

void btlDispatchSpecialEnemyActionWhenPhaseAllows(u8 *unit, s32 action) {
    u8 *data;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    if ((u16)(((BtlUnit *)unit)->mode - 0x119) >= 2) {
        return;
    }
    data = (u8 *)((BtlState *)btlGetRuntime())->effect;
    if (((BtlUnit *)unit)->mode == 0x11a && *(u16 *)data >= 3) {
        return;
    }
    btlRestoreUnitMinimumValueAndClearStatus(unit, action);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A560);

s32 btlMapCommandToSkill(u32 command) {
    switch (command) {
    case 0: return 0x142;
    case 1: return 0x142;
    case 2: return 0x13d;
    case 3: return 0x13e;
    case 4: return 0x13f;
    case 5: return 0x140;
    case 6: return 0x141;
    default: return -1;
    }
}

/* Map linked-command responses; response 1 on these boss modes starts a one-tick shake task. */
s32 btlMapLinkedCommandResult(BtlUnit *unit, s32 arg1) {
    BtlRuntimeTask *task;

    if (!(unit->flags & 0x400)) {
        return arg1;
    }
    switch (unit->mode) {
    case 0x11B:
        if (arg1 == 2) {
            return 0;
        }
        if (arg1 == 0xD) {
            return -1;
        }
        break;
    case 0x13D:
    case 0x13E:
    case 0x13F:
    case 0x140:
    case 0x141:
    case 0x142:
        switch (arg1) {
        case 2:
            return 0;
        case 3:
            return 4;
        case 5:
            return 4;
        case 6:
            return 4;
        case 7:
            return 4;
        case 8:
            return 4;
        case 1:
            task = (BtlRuntimeTask *)btlCreateStiffenDamageShakeTask((u8 *)unit, 8.0f);
            task->startDelay = 1;
            btlStartTask(task);
            return -1;
        case 4:
            break;
        }
        break;
    }
    return arg1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A860);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AB08);

s32 btlFadeOtherEnemyUnitsWhenSpecialModeActive(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x11B) {
                    if (unit->flags & 0x20) {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit->flags & 2) {
                        if (!(unit->flags & 0xE0)) {
                            if (unit->mode != 0x11B) {
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->flags &= ~1;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

s32 btlMapSkillToCode(s32 skill) {
    switch (skill) {
    case 0x13d: return 0xb9;
    case 0x13e: return 0xbb;
    case 0x13f: return 0xbd;
    case 0x140: return 0xbf;
    case 0x141: return 0xc1;
    case 0x142: return 0xc3;
    default: return -1;
    }
}

s32 btlMapSkillRange(u32 skill) {
    if (skill < 0x143) {
        if (skill >= 0x13d) {
            return 0x12c;
        }
    }
    return 0;
}

/* Restore Hariti and the children's positions, with left-side children first. */
void btlRestoreHaritiFormation(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;

    for (; unit != NULL; unit = unit->next) {
        u32 flags = unit->flags;

        if (flags & 1) {
            if (flags & 0x400) {
                switch (unit->mode) {
                case 0x11B:
                    unit->position[0] = 0.0f;
                    unit->position[1] = 0.0f;
                    unit->position[2] = 248.813782f;
                    unit->flags = flags & ~0x80000;
                    break;
                case 0x13E:
                    unit->position[0] = -521.0f;
                    unit->position[1] = -1100.0f;
                    unit->position[2] = 537.0f;
                    unit->position[3] = 0.0f;
                    break;
                case 0x140:
                    unit->position[0] = -707.0f;
                    unit->position[1] = -690.0f;
                    unit->position[2] = 315.0f;
                    unit->position[3] = 0.0f;
                    break;
                case 0x141:
                    unit->position[0] = -826.0f;
                    unit->position[1] = -250.0f;
                    unit->position[2] = 131.0f;
                    unit->position[3] = 0.0f;
                    break;
                case 0x13D:
                    unit->position[0] = 521.0f;
                    unit->position[1] = -1100.0f;
                    unit->position[2] = 737.0f;
                    unit->position[3] = 0.0f;
                    break;
                case 0x13F:
                    unit->position[0] = 707.0f;
                    unit->position[1] = -690.0f;
                    unit->position[2] = 465.0f;
                    unit->position[3] = 0.0f;
                    break;
                case 0x142:
                    unit->position[0] = 826.0f;
                    unit->position[1] = -250.0f;
                    unit->position[2] = 281.0f;
                    unit->position[3] = 0.0f;
                    break;
                }
                btlSetUnitPosition(unit, unit->position);
            }
        }
    }
}

/* Map populated boss-action descriptors to species-specific variants; -1 rejects. */
s32 btlMapSpeciesToActionVariant(u8 *unit, s32 action) {
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return -1;
    }
    if (datActionAnimationRecords[action].kind == 0) {
        return -1;
    }
    switch (((BtlUnit *)unit)->mode) {
    case 0x11b: return action != 0x1ad ? 11 : 18;
    case 0x13d: return 12;
    case 0x13e: return 13;
    case 0x13f: return 14;
    case 0x140: return 15;
    case 0x141: return 16;
    case 0x142: return 17;
    default: return -1;
    }
}

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern char D_003A5FF0[];

extern char D_003A6018[];

/* Random battle camera shot (A-E:0..2). Must stay defined above its callers in this
   file: retail's btlChooseDefeatCameraByActionAndTargets sees it as nothrow (bnez vs bnel in the branch slot). */
/* Select one of three fixed camera-key sets; keep K&R for the no-argument caller. */
INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6018);

void btlChooseRandomPresetCameraKeys(unit)
    u8 *unit;
{
    u32 kind;

    btlFlagAllUnitDefeatCandidatesTask();
    kind = effMiscRandMod(0, 3);
    switch (kind) {
    case 0:
        func_003003F0(D_003A5FF0);
        btlSetEffectCameraKeys(unit, -34.8f, -203.4f, -1877.1f, -0.042f, 0.006f, -0.012f, 0.99f, 59.4f, -184.8f, -2072.9f,
                      -0.041f, 0.024f, -0.013f, 0.99f, 40.0f, 20.0f);
        break;
    case 1:
        func_003003F0(D_003A6018);
        btlSetEffectCameraKeys(unit, -391.6f, -567.8f, -2206.8f, 0.032f, -0.062f, -0.014f, 0.989f, -407.1f, -447.8f, -2343.8f,
                      -0.003f, -0.062f, -0.012f, 0.989f, 40.0f, 20.0f);
        break;
    case 2:
        func_003003F0("A-E:2++++++++++++++++++++++++++\n");
        btlSetEffectCameraKeys(unit, 479.7f, -240.2f, -2018.3f, -0.023f, 0.105f, -0.016f, 0.985f, 552.2f, -250.7f, -2300.2f,
                      -0.023f, 0.105f, -0.016f, 0.985f, 40.0f, 20.0f);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B190);

extern void btlInitMotionTransformFromComponents(BtlCamState *, f32, f32, f32, f32, f32, f32, f32, f32);
extern void func_002DD608(f32);
extern void func_002DD968(f32);
extern void sdfComposeVuMatrixFromRegisters(void);

s32 btlSetLinkedDefeatCameraPresetA(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate) {
    u16 *runtime = (u16 *)btlGetRuntime();
    BtlUnit *unit;
    u32 kind;
    f32 angle;

    if (command->task == NULL) {
        return 1;
    }
    unit = btlGetTargetUnitForLink(command);
    if (unit == NULL) {
        return 1;
    }
    if (unit->flags & 0x400) {
        return 1;
    }
    if (runtime[0x122] == 3) {
        kind = unit->lookupId;
    } else {
        kind = unit->lookupId == 0 ? 0 : 2;
    }
    btlFlagAllUnitDefeatCandidatesTask();
    switch (kind) {
    case 0:
        btlInitMotionTransformFromComponents(camera, -869.5f, -67.11f, -1933.83f,
            -0.08f, -0.16f, 0.0f, 0.98f, 40.0f);
        break;
    case 1:
        btlInitMotionTransformFromComponents(camera, 167.34f, -68.93f, -1994.86f,
            -0.1f, 0.05f, -0.02f, 0.99f, 40.0f);
        break;
    case 2:
        btlInitMotionTransformFromComponents(camera, 976.31f, -67.47f, -1885.64f,
            -0.08f, 0.2f, -0.03f, 0.97f, 40.0f);
        break;
    }
    if (rotate == 1 && runtime[0x122] == 3) {
        switch (kind) {
        case 0:
            func_002DD608(-0.13089969f);
            func_002DD968(0.13089969f);
            sdfComposeVuMatrixFromRegisters();
            break;
        case 1:
            func_002DD608(-0.13089969f);
            break;
        case 2:
            angle = -0.13089969f;
            func_002DD608(angle);
            func_002DD968(angle);
            sdfComposeVuMatrixFromRegisters();
            break;
        }
        /* vu0 routine: rotate the camera direction by the prepared matrix. */
        VU0_LOAD_VF(vf10, camera->direction);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, camera->direction);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B560);

s32 btlTryStartTargetFacingActionEffect(u8 *unit) {
    u8 *entry = (u8 *)((BtlEventEntry *)unit)->task;
    u32 flags = ((BtlTask *)entry)->unit->flags;
    u8 *other;
    if (flags & 0x200) {
        if ((flags & 0x1000) == 0) {
            return 0;
        }
        if (btlGetIndexListCount(((BtlTask *)entry)->targetList) == 1) {
            other = (u8 *)btlGetIndexListEntry(((BtlTask *)entry)->targetList, 0);
            if ((((BtlUnit *)other)->flags & 0x400) == 0) {
                return 0;
            }
            btlFaceLinkedTargetAndFlagDirection(unit, unit);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSetLinkedDefeatCameraPresetA(unit, unit, 0);
        }
    } else {
        btlChooseRandomPresetCameraKeys();
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B640);

s32 btlHandleTargetedDefeatAction(u8 *unit) {
    u8 *entry = (u8 *)((BtlEventEntry *)unit)->task;
    u8 *other;
    if (((BtlTask *)entry)->unit->flags & 0x200) {
        if (btlGetIndexListCount(((BtlTask *)entry)->targetList) == 1) {
            other = (u8 *)btlGetIndexListEntry(((BtlTask *)entry)->targetList, 0);
            if ((((BtlUnit *)other)->flags & 0x400) == 0) {
                return 0;
            }
            func_0020B190(unit, other);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSetLinkedDefeatCameraPresetA(unit, unit, 0);
        }
        ((BtlUnit *)unit)->flags = 0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B818);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6338);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6358);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020BE30);

extern void func_0020B190(u8 *, void *);

/* Group-camera policy takes precedence over target-camera policy.
 * Return 1 after selecting keys, or 0 when neither camera policy is requested. */

s32 btlChooseDefeatCameraByActionAndTargets(u8 *unit) {
    u16 flags = datActionAnimationRecords[((BtlLinkedCommand *)unit)->actionCode].flags;
    if (flags & BTL_ANIMATION_GROUP_DEFEAT_CAMERA) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA)) {
            btlChooseRandomPresetCameraKeys(unit);
        } else {
            btlSetEffectCameraKeys(unit, 59.2f, -700.6f, -1901.2f,
                           0.092f, 0.023f, -0.009f, 0.987f,
                           59.2f, -160.6f, -1901.2f, -0.106f,
                           0.025f, -0.014f, 0.985f, 45.0f, 30.0f);
        }
        return 1;
    } else if (flags & BTL_ANIMATION_TARGET_DEFEAT_CAMERA) {
        if (btlGetIndexListCount(((BtlEventEntry *)unit)->task->targetList) == 1) {
            void *other = (void *)btlGetIndexListEntry((void *)((BtlEventEntry *)unit)->task->targetList, 0);
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020B190(unit, other);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlChooseRandomPresetCameraKeys(unit);
        }
        return 1;
    }
    return 0;
}

u32 func_0020CB28(s32 action) {
    u32 result;

    result = 2;
    if (action != 0x1d7) {
        result = 0;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6398);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6438);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6460);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6480);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB880);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB888);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB890);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB898);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8A0);

