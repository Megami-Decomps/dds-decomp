#include "common.h"
#include "btl_command.h"
#include "pcp_vu0.h"

extern u64 func_00219318(void);

extern s32 btlRollAiBucket(void);

extern u32 btlNextScaledRandom(u32);

extern u32 btlPreviousAiCandidateBucket;

extern s32 btlGetRuntime(void);

extern s32 btlIsSelectedActorStatusAndRecordClear();

extern s32 func_00212CB8(u32, u32, u32);

extern s32 btlHasAvailableOption(void);

extern s32 func_001ABB10(void);

extern u32 func_001AC360(u64, u64, u64);

extern s8 btlHistoryCounter;

extern void func_00211EA8();

extern s32 btlActorEntryIsExpired();

extern s32 btlGetActorEntryCode();

extern s32 D_003BF660[];

extern u32 btlPickWeightedAiSlot();

extern s32 fldGetSelectedUnitStat();

extern void func_002152D8(s32, s32);

extern void func_00215C70(s32, s32);

extern void *memset(void *, s32, u32);

extern void func_00216888(s32, s32);

extern s32 func_00215118(s32, u16 *, u16);

extern char D_00436CC8[], D_00436CD0[], D_00436CD8[];

extern void *btlCreateActionTask(void *, s32);

extern s32 btlFindScriptResource(char *);

extern u8 *btlGetSideIndexedActorStatusTable(s32, s32);

extern s32 btlHasLinkedEffectNodeTrigger(void *);

extern u32 btlAppendSelfAfterTargetScan();

extern void btlSelectLinkedTargets(s32, s32, s8);

extern void func_00216988();

extern s32 btlIsActorCategoryMarked(s32);

extern void btlPrepareRandomizedActionCameraPose(s32, s32, s32);

typedef struct BattleCtx {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0x80];
    u16 turns;
    u8 pad92[0xBC];
    s8 lowHpActionHold; /* 0x14E: positive suppresses btlIsLowHpActionReady */
    u8 pad14F;
    s32 action;
} BattleCtx;

typedef struct BattleSub {
    s32 task;
    s32 targetMode;
    u8 pad08[4];
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
    struct BattleActorHandle *actionActors; /* 0x248: linked action handles */
    BtlUnit *actorList;
    u8 pad250[0x1C];
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

typedef struct BattleNamedResource BattleNamedResource;


/* Same action-state link layout used by code_0021EE10.c. */
typedef struct ActionStateLink {
    u8 pad00[0x18];
    BtlUnit *unit; /* 0x18 */
    u8 pad1C[0x28];
    s32 slot; /* 0x44: action-kind table index, also reached through command->link */
    u8 pad48[0x18];
    u32 targetHandle; /* 0x60 */
    u8 pad64[0x10C];
    s32 lastMode; /* 0x170: mode of the unit picked last time */
} ActionStateLink;

/* Battle-select controller at work+0x718: the unit being selected and the previous one. */
typedef struct BtlSelectCtrl {
    BtlUnit *unit;
    BtlUnit *prevUnit;
    s8 pending;
} BtlSelectCtrl;


/* Queued action slot: some queries inspect the full word, others its ID. */
typedef union BattleActionSlot {
    s32 word;
    s16 actionId;
} BattleActionSlot;

/* Handle returned by btlFindUnitByActor; these fields drive its action task. */
typedef struct BattleActorHandle {
    u8 pad00[8];
    u32 pendingFlags; /* 0x08 */
    u32 flags; /* 0x0C */
    u8 pad10[8];
    BtlUnit *unit; /* 0x18 */
    u8 pad1C[4];
    s32 phase; /* 0x20 */
    u8 pad24[0x3C];
    s32 actorIndices; /* 0x60 */
    u8 pad64[0xEC];
    BattleActionSlot actions[8]; /* 0x150: 0x20 bytes */
    u8 pad170[8];
    struct BattleActorHandle *next; /* 0x178 */
} BattleActorHandle;

/* Action and sound tasks both carry their mutable control word at +0x28. */
typedef struct BattleTaskControl {
    u8 pad00[0x28];
    u32 control;
    u8 pad2C[0x14];
    u64 targetId; /* 0x40 */
} BattleTaskControl;

extern s32 btlFindUnitByActor(BtlUnit *);

extern void fldAppendSceneGroupHandle(s32);

extern void btlAppendIndexListEntry();

extern BattleCtx **btlActionScratchWork;

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

extern char D_00419A88[];

extern void btlDebugPrintf(const char *, ...);

extern s32 func_001E2E58(u8 *, s32);

extern void btlSetEffectCameraKeys(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void evtUnitSetStoredParameter(void *, s32);

extern void evtSetTransitionMotionScale(void *, f32);

extern void evtStoreUnitMotionShortParameters(void *, s32, s32);

/* One of five per-side records (0x1C4 bytes each) at datGameState + 0xA60. */
typedef struct BattleSlotRecord {
    u16 flags;
    u16 pad02;
    u16 group;
    u8 pad06[8];
    u16 actionMask;
    u8 pad10[0x1C4 - 0x10];
} BattleSlotRecord;

extern s32 func_001AA840();

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(void *);

extern s32 btlMatchesActorEntryCodeCondition();

extern u32 btlGetSubtaskTargetMode(void);

extern u32 btlAppendEffectActorToCommandIndices(s32);

extern s32 mdlFlagTest(s32);

extern s32 datGameState;

extern s32 func_001AA700(void *);

extern s32 func_001AA740(void *);

extern s32 func_001AA708(void *);

extern s32 func_001AA758(void *);

extern void *btlAllocateIndexList(s32);

extern u32 btlGetIndexListCount();

extern s32 btlAreUnitStatusAndEntryFlagsClear();

extern void func_001AC0F8(s32, void *, s32, s32, s32);

extern s32 btlGetIndexListEntry(void *, u32);

extern void btlFreeIndexList(void *);

extern s32 btlUnitHasNegativeActionQueryResult(void *, s32);

extern s8 D_00419C30[];

extern s8 D_00419C58[];

extern s32 btlWouldUiValueFallBelowQuarter(s32, s32);

extern u8 sdfPfsDebugMode;

extern s32 btlAnyUnitHasActionInSlots();

extern s32 func_001B3200(s32);

extern s32 func_00213F58(s32, s32, s32);

extern s32 btlUnitBlocksElementQuery(s32, s32, s32);

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

extern s32 btlElementToBitIndex(s32, s32);

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

extern s32 func_001AE8C0(void *, s32);

extern s32 btlHasSpecialAbility274(void *, s32);

extern s32 btlHasEnabledSpecialAbilityForSlot(void *, s32);

extern s32 btlGroup400UnitHasAction(void *, s32);

extern s32 func_001B2F50(void *, s32);

extern s8 D_00438F84;

extern s32 sdfNamedChunkFindId(void *, void *);

extern void func_0021A1D8(s32, s32);

extern s32 btlHasAdjacentActorRecordStatus(void);

extern s32 btlIsActorHighStateFlagClear(s32 item);

extern char D_00419B38[];

extern char D_00419B68[];

extern char D_00419B88[];

extern s32 func_001ABF50();
extern u32 func_001B39E8(s32);


extern void btlUnitGetMuzzlePosVU(BtlUnit *);

extern BtlUnit *btlGetTargetUnitForLink();

extern void *btlCreateUnitFadeOutTask(void *, s32, s32);

extern s64 btlStartTask(void *);

extern void func_001E3108(void *, void *);

extern void btlSetUnitPosition(void *, void *);

extern s32 btlIsUnitDefeatTriggeredByValueDelta(s32, s32);

extern s32 btlIsActiveActor();

extern void effObjSetInnerFirstVec();

extern void func_001E2758(void *);

extern void fldAppendTaskToGroup(void *);

extern s32 btlDispatchStateHandler(void *, s32);

void btlRunWeightedAiAction(ActionStateLink *task, s32 row) {
    u16 species;
    s32 index;

    btlActionScratchWork = sdfAllocAndClearQuadwords(0x10);
    species = task->unit->mode;
    index = btlPickWeightedAiSlot((s32)task->unit, species, row);
    func_00211EA8(task, datEnemyAiRecords[species].slot[row * 5 + index].actionId, datEnemyAiRecords[species].slot[row * 5 + index].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211360);

u32 func_002115B0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002115B8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211658);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002119E0);

/* Weighted pick of a table row: returns the first slot whose cumulative
   weight reaches `roll` (btlRollAiBucket) among the five slots of row `index`. */
u32 btlPickWeightedAiSlot(s32 unit, s32 species, s32 index) {
    u32 roll;
    u32 total;
    u32 i;

    btlGetRuntime();
    roll = btlRollAiBucket();
    total = 0;
    for (i = 0; i < 5; i++) {
        u32 weight = datEnemyAiRecords[species].slot[index * 5 + i].weight;

        total = (total + weight) & 0xFFFF;
        if (total >= roll && weight != 0) {
            return i;
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

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211EA8);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419A88);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211F38);

s32 btlIsUnitAtOrBelowHealthRate(BtlUnit *unit, s32 multiplier) {
    u8 *stats = (u8 *)&unit->statBits;
    s32 current = func_001AA700(stats);
    s32 maximum = func_001AA740(stats);
    if ((u32)(maximum * multiplier) < (u32)(current * 100)) {
        return 0;
    }
    return 1;
}

s32 btlHasBossAtOrBelowHealthRate(s32 unused, s32 multiplier) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x401) == 0x401 &&
            btlIsUnitAtOrBelowHealthRate(unit, multiplier)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasUnitAtOrBelowHealthRate(s32 unused, s32 multiplier) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            u8 *stats = (u8 *)&unit->statBits;
            s32 current = func_001AA700(stats);
            s32 maximum = func_001AA740(stats);
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
            u8 *stats = (u8 *)&unit->statBits;
            s32 current = func_001AA700(stats);
            s32 maximum = func_001AA740(stats);
            if ((u32)(current * 100) >= (u32)(maximum * multiplier)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlResetAiCounterAtLimit() {
    if (btlAiCounterReachedLimit()) {
        (*btlActionScratchWork)->turns = 0;
        return 1;
    }
    return 0;
}

/* Count group 0x400 units that are active and not marked 0x20. */
s32 btlIsGroup400CountAtMost(s32 unused, u32 limit) {
    u32 count = 0;
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x421) == 0x401) {
            count++;
        }
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

/* Count eligible group 0x200 units without condition 0x800. */
s32 btlIsGroup200EligibleCountAtMost(s32 unused, u32 limit) {
    u32 count = 0;
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            if (!(unit->conditionFlags & 0x800)) {
                count++;
            }
        }
    }
    if (limit < count) {
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
        if (*(s32 *)(datGameState + 0x3c) < 2) {
            return 0;
        }
    }
    return mdlFlagTest(0x290) != 0;
}

/* Test whether the count of active group 0x200 units fits within limit. */
s32 btlIsGroup200CountAtMost(s32 unused, u32 limit) {
    u32 count = 0;
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
            count++;
        }
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

/* Intersect a unit's stat mask with the requested condition bits. */
s32 btlUnitHasActionMask(s32 actor, s32 mask) {
    return (func_001AA840(&((BtlUnit *)actor)->statBits, mask) & mask) != 0;
}

s32 btlAnyGroup400HasActionMask(s32 unused, s32 mask) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x401) == 0x401 &&
            btlUnitHasActionMask((s32)unit, mask)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasActorOrSlotMatchingActionQuery(s32 unused, u32 query) {
    BtlUnit *unit;
    BattleSlotRecord *record;
    s32 i;

    for (unit = ((BattleWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->mode == ((query >> 16) & 0x3F)) {
            if (unit->flags & 1) {
                if (btlUnitHasActionMask((s32)unit, query & 0xFFFF)) {
                    return 1;
                }
            }
        }
    }
    record = (BattleSlotRecord *)(datGameState + 0xA60);
    for (i = 0; i < 5; i++, record++) {
        if (record->flags & 1) {
            if (!(record->flags & 2)) {
                if (record->group == ((query >> 16) & 0x3F)) {
                    if ((query & 0xFFFF) == 0x7FFF) {
                        return ((func_001AA840() & query) & 0xFFFF) != 0;
                    }
                    if ((record->actionMask & 0x7FFF & query) != 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

s32 btlAnyGroup200HasActionMask(s32 unused, s32 mask) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x201) == 0x201 &&
            btlUnitHasActionMask((s32)unit, mask)) {
            return 1;
        }
    }
    return 0;
}

/* Require every eligible group 0x200 unit to have a requested condition bit. */
s32 btlAllGroup200HaveActionMask(s32 unused, s32 mask) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201 &&
            !btlUnitHasActionMask((s32)unit, mask)) {
            return 0;
        }
    }
    return 1;
}

/* Look for an active group 0x200 unit in the requested mode. */
s32 btlHasGroup200UnitMode(s32 unused, s32 mode) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x221) == 0x201 &&
            unit->mode == mode) {
            return 1;
        }
    }
    return 0;
}

/* Exclude the supplied unit ID while checking group 0x400 in this mode. */
s32 btlHasOtherGroup400UnitMode(s32 excludedUnit, s32 mode) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    for (; unit != 0; unit = unit->nextActor) {
        if ((*(u64 *)&unit->flags & 0x421) == 0x401 &&
            unit->mode == mode &&
            unit->owner != ((BtlUnit *)excludedUnit)->owner) {
            return 1;
        }
    }
    return 0;
}

/* Kinds 0xB/0xC scan the even/odd entry codes; other kinds test their own
   entry. Every failure falls out to the single trailing `return 0`. K&R:
   callers pass one argument or none. */
s32 btlMatchesActorEntryCodeCondition(actor, kind)
    s32 actor;
    s32 kind;
{
    s32 i;
    s32 j;

    if (kind == 0xB) {
        for (i = 0; i < 10; i += 2) {
            if (btlActorEntryIsExpired(actor, D_003BF660[i]) != 0 && btlGetActorEntryCode(actor, D_003BF660[i]) > 0) {
                return 1;
            }
        }
    } else if (kind == 0xC) {
        for (j = 1; j < 10; j += 2) {
            if (btlActorEntryIsExpired(actor, D_003BF660[j]) != 0 && btlGetActorEntryCode(actor, D_003BF660[j]) <= 0) {
                return 1;
            }
        }
    } else if (btlActorEntryIsExpired(actor, D_003BF660[kind]) != 0 || kind == 0xA || kind == 0xD) {
        if ((kind & 1) == 0 || kind == 0xD) {
            if (btlGetActorEntryCode(actor, D_003BF660[kind]) > 0) {
                return 1;
            }
        } else {
            if (btlGetActorEntryCode(actor, D_003BF660[kind]) <= 0) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212CB8);

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

INCLUDE_ASM(const s32, "game/code_002112C8", btlUnitHasAllTenActions);

s32 func_00212F20(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (((*(u64 *)&battler->flags) & 0x221) == 0x201 &&
            btlMatchesActorEntryCodeCondition(battler, action)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00212FA0(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            btlMatchesActorEntryCodeCondition(battler, action)) {
            return 1;
        }
    }
    return 0;
}

u8 btlCheckUnitActionModeZero(u32 unit, u32 action) {
    s64 result;

    result = func_00212CB8(unit, action, 0);
    return result != 0;
}

u8 btlCheckUnitActionModeOne(u32 unit, u32 action) {
    s64 result;

    result = func_00212CB8(unit, action, 1);
    return result != 0;
}

s32 func_00213060(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            func_00212CB8((s32)battler, action, 0)) {
            return 1;
        }
    }
    return 0;
}

s32 func_002130E8(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            func_00212CB8((s32)battler, action, 1)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00213170(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            func_00212CB8((s32)battler, action, 0)) {
            return 1;
        }
    }
    return 0;
}

s32 func_002131F8(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            func_00212CB8((s32)battler, action, 1)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00213280(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            func_00212CB8((s32)battler, action, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_00213308(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            func_00212CB8((s32)battler, action, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyGroup200LacksFlag1000(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            !(battler->flags & 0x1000)) {
            return 1;
        }
    }
    return 0;
}

u8 btlUnitPassesActionTenCheck(u32 unit) {
    s64 result;

    result = btlMatchesActorEntryCodeCondition(unit, 10);
    return result != 0;
}

s32 func_00213418(void) {
    return btlMatchesActorEntryCodeCondition() != 0;
}

s32 func_00213438(BtlUnit *unit) {
    u32 flags;

    if (unit->flags & 0x200) {
        return 0;
    }
    flags = unit->statBits & 0x2000;
    return flags != 0;
}

s32 btlHasContextFlagTwo(void) {
    return (((*btlActionScratchWork)->flags & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_002112C8", btlCheckCounterLimit);

s32 btlCounterReachedLimit(s32 unused, u32 limit) {
    if (func_001B39E8(4) < limit) {
        return 0;
    }
    return 1;
}

s32 btlAiCounterReachedLimit(s32 unused, u32 limit) {
    BattleCtx *ctx = *btlActionScratchWork;

    ctx->turns = ctx->turns + 1;
    ctx->turns = ctx->turns == 0 ? 0 : ctx->turns >= 0x100 ? 0xFF : ctx->turns;
    if ((*btlActionScratchWork)->turns < limit) {
        return 0;
    }
    return 1;
}

s32 btlTurnReachedLimit(s32 unused, u32 limit) {
    if (((BattleWork *)btlGetRuntime())->turnCount < limit) {
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

s32 btlIsReadyWithoutTurns(void) {
    BattleWork *battle = (BattleWork *)btlGetRuntime();
    if (battle->phase == 2) {
        if (battle->turnCount == 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlIsUnitStatAtOrBelowRate(BtlUnit *unit, s32 percentage) {
    void *stats = &unit->statBits;
    u32 current = func_001AA708(stats);
    u32 scaledMaximum = func_001AA758(stats) * percentage;
    if (scaledMaximum < current * 100) {
        return 0;
    }
    return 1;
}

s32 btlUnitStatAtOrAboveRate(BtlUnit *unit, u32 percentage) {
    void *stats = &unit->statBits;
    u32 current = func_001AA708(stats);
    u32 maximum = func_001AA758(stats);
    if (current * 100 < maximum * percentage) {
        return 0;
    }
    return 1;
}

s32 btlUnitStatAtMost(BtlUnit *unit, u32 limit) {
    void *stats = &unit->statBits;
    u32 current = func_001AA708(stats);
    func_001AA758(stats);
    if (limit < current) {
        return 0;
    }
    return 1;
}

s32 btlUnitStatAtLeast(BtlUnit *unit, u32 limit) {
    void *stats = &unit->statBits;
    u32 current = func_001AA708(stats);
    func_001AA758(stats);
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

s32 btlAnyIndexedUnitPassesQuery(BtlUnit *unit) {
    u32 i;
    u32 count;
    s32 battle;
    void *list;
    if (unit->flags & 0x400) {
        return 0;
    }
    battle = (s32)*btlActionScratchWork;
    list = btlAllocateIndexList(13);
    func_001AC0F8(battle, list, 2, 0, 0);
    count = btlGetIndexListCount(list);
    for (i = 0; i < count; i++) {
        if (btlAreUnitStatusAndEntryFlagsClear(btlGetIndexListEntry(list, i)) != 0) {
            btlFreeIndexList(list);
            return 1;
        }
    }
    btlFreeIndexList(list);
    return 0;
}

s32 btlIsLowHpActionReady(BtlUnit *unit) {
    s32 roll = 0;
    s16 pick;
    BattleCtx *ctx = *btlActionScratchWork;
    u16 rank = unit->unk134;
    s32 delayPending = func_001B39E8(4) < (u32)(rank + 0xF);

    if (ctx->lowHpActionHold <= 0) {
        /* The canonical header leaves HP/max-HP in pad126. */
        if (delayPending == 0 && *(u16 *)unit->pad126 * 100 / *(u16 *)(unit->pad126 + 2) < 0x1E && btlIsGroup400CountAtMost(unit, 2) != 0) {
            pick = btlRollAiBucket();
            roll = pick < 0x1E;
        }
    }
    if (roll == 1 && btlIsSelectedActorStatusAndRecordClear(unit) != 0) {
        return 1;
    }
    return 0;
}

s32 btlUnitHasFlag1000(s32 unit) {
    return (((s32)((BtlUnit *)unit)->flags & 0x1000) > 0);
}

u8 func_00213910(void) {
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

s32 func_00213B38(s32 unused, s32 mask) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x401) == 0x401 &&
            btlUnitHasNegativeActionQueryResult(battler, mask)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00213BB8(s32 unused, s32 mask) {
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
            battler->mode != mode) {
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
        resolvedSelection = ((BtlUnit *)actor)->mode;
    }
    battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            battler->mode != resolvedSelection &&
            battler->owner != ((BtlUnit *)actor)->owner) {
            return 1;
        }
    }
    return 0;
}

s32 btlAnyUnitPassesCheck200(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup((u8 *)battler, action, 0x200) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_00213DA0(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup((u8 *)battler, action, 0x200) == 1) {
            return 0;
        }
    }
    return 1;
}

s32 btlAnyUnitPassesCheck400(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup((u8 *)battler, action, 0x400) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_00213E80(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup((u8 *)battler, action, 0x400) == 1) {
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

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213F58);

s32 btlAnyUnitHasActionInSlots(mask, action)
    s32 mask;
    s32 action;
{
    u8 *actor;
    BtlUnit *unit;
    u32 flags;
    s32 i;
    for (actor = (u8 *)((BattleWork *)btlGetRuntime())->actionActors; actor != 0; actor = (u8 *)((BattleActorHandle *)actor)->next) {
        unit = ((BattleActorHandle *)actor)->unit;
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
            if (func_00213F58(action, ((BattleActorHandle *)actor)->actions[i].actionId, 1) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", btlAnyGroup200HasAction);

INCLUDE_ASM(const s32, "game/code_002112C8", btlAnyGroup400HasAction);

/* The signed action-slot halfword at +0x2D0 is absent from the canonical header. */
s32 func_002141A8(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            func_00213F58(action, *(s16 *)((u8 *)battler + 0x2D0), 0)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00214230(s32 unused, s32 battler) {
    s32 node = (s32)((BattleWork *)btlGetRuntime())->actionActors;
    for (; node != 0; node = (s32)((BattleActorHandle *)node)->next) {
        BtlUnit *target = ((BattleActorHandle *)node)->unit;
        if (target != 0 &&
            (target->flags64 & 0x221) == 0x201 &&
            func_00213F58(battler, ((BattleActorHandle *)node)->actions[0].actionId, 0)) {
            return 1;
        }
    }
    return 0;
}

s32 func_002142C0(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            btlUnitHasAllTenActions((s32)battler)) {
            return 1;
        }
    }
    return 0;
}

s32 func_00214330(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x421) == 0x401 &&
            btlUnitHasAllTenActions((s32)battler)) {
            return 1;
        }
    }
    return 0;
}

extern s8 *datCommandSelectors;

extern s32 datCommandRecords;

/* Scan active group-0x200 actors for a queued action whose table entry has
 * nonzero byte 8 and class byte 9 equal to 2. */
s32 btlHasEligibleQueuedSpecialAction(void) {
    u8 *actor;
    BtlUnit *unit;
    u8 *actionEntry;
    s16 actionId;
    s32 i;
    for (actor = (u8 *)((BattleWork *)btlGetRuntime())->actionActors; actor != 0; actor = (u8 *)((BattleActorHandle *)actor)->next) {
        unit = ((BattleActorHandle *)actor)->unit;
        if (unit == 0) {
            continue;
        }
        if ((unit->flags64 & 0x221) != 0x201) {
            continue;
        }
        for (i = 0; i < 8; i++) {
            actionId = ((BattleActorHandle *)actor)->actions[i].actionId;
            if (actionId == 0) {
                continue;
            }
            if ((u32)(*(u8 *)(datCommandSelectors + actionId * 2) - 0x10) < 2U) {
                continue;
            }
            actionEntry = (u8 *)(actionId * 0x38 + datCommandRecords);
            if (actionEntry[8] == 0) {
                continue;
            }
            if (actionEntry[9] != 2) {
                continue;
            }
            return 1;
        }
    }
    return 0;
}

/* Required to match: preserve the original wide flag read beside the state-word test. */
s32 btlHasUnitWithStatusBit(s32 battler, s32 scanAll) {
    if (scanAll != 0) {
        battler = (s32)((BattleWork *)btlGetRuntime())->actorList;
        while (battler != 0) {
            if ((*(u64 *)&((BtlUnit *)battler)->flags & 0x421) == 0x401 &&
                (((BtlUnit *)battler)->stateFlags & 0x800000) != 0) {
                return 1;
            }
            battler = (s32)((BtlUnit *)battler)->nextActor;
        }
        return 0;
    }
    if ((*(u64 *)&((BtlUnit *)battler)->flags & 0x21) == 1 &&
        (((BtlUnit *)battler)->stateFlags & 0x800000) != 0) {
        return 1;
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
        if (btlUnitBlocksElementQuery((s32)battler, action, 0x200)) {
            return 1;
        }
    }
    return 0;
}

/* The tested stat halfword at +0x12A is still padding in the canonical header. */
s32 btlAnyGroupUnitHasZeroStat(void) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if ((*(u64 *)&battler->flags & 0x221) == 0x201 &&
            *(u16 *)(battler->pad126 + 4) == 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlAnyUnitHasQueuedQuery(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_002112C8", btlAnyGroup400HasQuery);

INCLUDE_ASM(const s32, "game/code_002112C8", btlAnyGroup200HasQuery);

s32 btlAnyUnitHasQueuedQuery(s32 unused, s32 id, s32 mask) {
    u8 *actor;
    BtlUnit *unit;
    u32 flags;
    s32 i;
    for (actor = (u8 *)((BattleWork *)btlGetRuntime())->actionActors; actor != 0; actor = (u8 *)((BattleActorHandle *)actor)->next) {
        unit = ((BattleActorHandle *)actor)->unit;
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
            if (((BattleActorHandle *)actor)->actions[i].word == id) {
                return 1;
            }
        }
    }
    return 0;
}

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
                        value = func_001ABF50((void *)unit, index);
                        if (btlHasEnabledSpecialAbilityForSlot((void *)unit, index) != 0 || (value & 0x20000) ||
                            (stat == 0x20000 && btlTestSelectedItemCategoryMask((void *)unit, index) != 0)) {
                            return 1;
                        }
                    }
                    return 0;
                }
                value = func_001ABF50((void *)unit, action);
                if (btlHasEnabledSpecialAbilityForSlot((void *)unit, action) != 0 || (value & 0x20000) ||
                    (stat == 0x20000 && btlTestSelectedItemCategoryMask((void *)unit, action) != 0)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214928);

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
                            func_001AE8C0(unit, index) != 0 ||
                            btlHasSpecialAbility274(unit, index) != 0 ||
                            btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (btlTestSelectedItemCategoryMask(unit, action) != 0 ||
                    func_001AE8C0(unit, action) != 0 ||
                    btlHasSpecialAbility274(unit, action) != 0) {
                    return 0;
                }
                return btlHasEnabledSpecialAbilityForSlot(unit, action) == 0;
            }
        }
    }
    return 2;
}

s32 btlAllUnitsPassCheck200(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup((u8 *)battler, action, 0x200) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 btlAllUnitsPassCheck400(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup((u8 *)battler, action, 0x400) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 btlUnitHasEitherSpecialAction(void *unit) {
    if (btlGroup400UnitHasAction(unit, 0x1b2) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1b6) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1ba) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1be) != 0) {
        return 1;
    }
    return btlGroup400UnitHasAction(unit, 0x1c2) != 0;
}

s32 btlActionMatchesUnit(s32 unit, s32 action) {
    if ((*btlActionScratchWork)->action == action) {
        if (((BtlUnit *)unit)->stateFlags & 0x1000) {
            return 1;
        }
    }
    return 0;
}

s32 btlGroup400UnitHasAction(void *unit, s32 action) {
    btlGetRuntime();
    if ((((BtlUnit *)unit)->flags64 & 0x421) == 0x401) {
        if (func_001B2F50(unit, action) != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214C78);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214DF8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00215118);

s32 btlBuildActorIndexListAndCount(s32 actor, u32 *matched, u32 *count) {
    u32 value;
    s32 indexList;

    indexList = btlAllocateIndexList(0xd);
    value = func_001AC360(actor, indexList, 0);
    *matched = value;
    value = btlGetIndexListCount(indexList);
    *count = value;
    return indexList;
}

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419B38);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419B68);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419B88);

INCLUDE_RODATA(const s32, "game/code_002112C8", btlRequiredActionCategories);

INCLUDE_RODATA(const s32, "game/code_002112C8", btlElementMasks);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419C30);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419C58);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419C80);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419CB0);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419CD8);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419D00);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419D20);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419DA0);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419E20);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419EA0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002152D8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00215C70);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00215D78);

s32 btlSelectLowestHealthRateTarget(s32 task) {
    u32 matched;
    u32 count;
    s32 list;
    u16 picked[12];
    s32 lowestPercent;
    u32 lowestIndex;
    u32 i;
    s32 target;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0:
        lowestPercent = 0x63;
        memset(picked, 0, sizeof(picked));
        lowestIndex = 0x20;
        for (i = 0; i < count; i++) {
            u8 *stats = (u8 *)&((BtlUnit *)btlGetIndexListEntry(list, i))->statBits;
            s32 current = func_001AA700(stats);
            s32 percent = current * 100 / func_001AA740(stats);

            if (lowestPercent >= percent && current != 0) {
                lowestPercent = percent;
                lowestIndex = i;
            }
        }
        if (lowestIndex != 0x20) {
            target = btlGetIndexListEntry(list, lowestIndex);
        } else {
            target = func_00215118(list, picked, count);
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, target);
        break;
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

s32 btlSelectTargetsByActionMask(s32 task, s32 mask) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            if (btlUnitHasActionMask(btlGetIndexListEntry(list, i), mask)) {
                picked[i] = 1;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

s32 btlSelectTargetsWithoutActionMask(s32 task, s32 mask) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            if (!btlUnitHasActionMask(btlGetIndexListEntry(list, i), mask)) {
                picked[i] = 1;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002162C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002163C8);

s32 btlSelectLowestRankTarget(s32 task) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};
        u16 best = 0x7FFF;
        u16 bestIndex = 0;

        for (i = 0; i < count; i++) {
            u16 value = ((BtlUnit *)btlGetIndexListEntry(list, i))->unk134;

            if (best >= value) {
                picked[bestIndex] = 0;
                best = value;
                picked[i] = 1;
                bestIndex = i;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216888);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216988);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216B40);

u32 btlAppendSelfAfterTargetScan(s32 battle) {
    void *list = btlAllocateIndexList(13);
    func_001AC0F8(battle, list, 1, 1, 0);
    btlGetIndexListCount(list);
    btlAppendIndexListEntry(((BtlTask *)battle)->unk_60, (u32)((BtlTask *)battle)->unit);
    btlFreeIndexList(list);
    return 1;
}

u32 func_00216D10(u32 task, u32 input) {
    btlSelectLinkedTargets(task, input, 1);
    return 1;
}

u32 func_00216D30(u32 task, u32 input) {
    btlSelectLinkedTargets(task, input, 0);
    return 1;
}

s32 btlSelectTargetsByMode(s32 task, s32 mode) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};
        BtlUnit *unit;

        for (i = 0; i < count; i++) {
            unit = btlGetIndexListEntry(list, i);
            if (mode != 0) {
                if (unit->mode == mode) {
                    picked[i] = 1;
                }
            } else if (((ActionStateLink *)task)->lastMode == unit->mode) {
                picked[i] = 1;
            }
        }
        unit = (BtlUnit *)func_00215118(list, picked, count);
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, unit);
        if (mode == 0) {
            ((ActionStateLink *)task)->lastMode = unit->mode;
        }
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

extern u32 btlGetEffectActor(void);

u32 btlAppendEffectActorToCommandIndices(s32 task) {
    u32 actor = btlGetEffectActor();
    btlAppendIndexListEntry(((BtlTask *)task)->unk_60, actor);
    return 1;
}

u32 btlAppendCurrentUnitIdToCommandIndices(s32 task) {
    u64 unitId;

    unitId = func_00219318();
    btlAppendIndexListEntry(((BtlTask *)task)->unk_60, unitId);
    return 1;
}

s32 btlSelectTargetsBlockingElement(s32 task, s32 action) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            if (btlUnitBlocksElementQuery(btlGetIndexListEntry(list, i), action, 0x200)) {
                picked[i] = 1;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

u32 func_00217020(void) {
    return 1;
}

s32 btlSelectTargetsPassingCheck(s32 task, s32 action) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            BtlUnit *unit = btlGetIndexListEntry(list, i);

            if (unit->flags & 0x200) {
                if (btlUnitBlocksElementQueryForGroup((u8 *)unit, action, 0x200) == 1) {
                    picked[i] = 1;
                }
            } else {
                if (btlUnitBlocksElementQueryForGroup((u8 *)unit, action, 0x400) == 1) {
                    picked[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

void btlSelectLinkedTargets(s32 task, s32 unused, s8 linked) {
    u32 matched;
    u32 count;
    s32 list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            BtlUnit *unit = btlGetIndexListEntry(list, i);

            if ((*(u64 *)&unit->flags & 0x221) == 0x201) {
                if (linked == 0) {
                    if (unit->flags & 0x1000) {
                        picked[i] = 1;
                    }
                } else if (!(unit->flags & 0x1000)) {
                    picked[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->targetHandle, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->targetHandle, list);
        break;
    }
    btlFreeIndexList(list);
}

extern s32 btlCanUseLinkedActor();

/* Pick the acting unit for a linked command: a usable linked unit without flag 0x1000, else the link's own unit. */
BtlUnit *btlGetTargetUnitForLink(BtlLinkedCommand *command) {
    s32 kind = command->actionCode;

    if ((u32)(kind - 1) >= 0x29F) {
        return command->link->unit;
    }
    if (datCommandSelectors[kind * 2 + 1] != 1) {
        return command->link->unit;
    }
    if (command->linkedA == NULL && command->linkedB == NULL) {
        return command->link->unit;
    }
    if (btlCanUseLinkedActor(command) == 0) {
        return command->link->unit;
    }
    if (command->linkedA != NULL) {
        if (command->linkedB != NULL) {
            return command->link->unit;
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
    return command->link->unit;
}


void btlFaceLinkedTargetAndFlagDirection(u8 *command, u8 *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = btlGetTargetUnitForLink(command);
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

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217470);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217650);


extern void btlFlagAllUnitsDefeatCandidate(void);
extern void btlCopyMotionTransform();
extern void btlUnitFaceTarget(BtlUnit *, BtlUnit *);
extern void func_001E88A8();
extern f32 func_00353228(f32);

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
    front->distance = front->distance + targetExtent * 1.5f / func_00353228(angle);
    minDistance = userExtent / func_00353228(angle);
    if (front->distance < minDistance) {
        front->distance = minDistance;
    }
    btlCopyMotionTransform(back, front);
    back->distance += 250.0f;
    if (mirror != 0) {
        func_001E88A8(front);
        func_001E88A8(back);
    }
    btlUnitFaceTarget(user, target);
    btlUnitFaceTarget(target, user);
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217B20);

void btlStartUnitActionIfPairedSelected(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *found;
    BtlUnit *other;
    s32 handle;

    /* The selection gate tests the first byte of the stored unit pointer, not the whole pointer. */
    if (*(s8 *)work->sub == 0) {
        found = 0;
        other = 0;
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            u32 flags = unit->flags;

            if (flags & 1) {
                if (flags & 0x400) {
                    if (unit->mode == 0x101) {
                        other = unit;
                    } else if (!(flags & 0xE0)) {
                        found = unit;
                    }
                }
            }
        }
        if (found != 0) {
            if (other != 0) {
                if (other->flags & 0xE0) {
                    handle = btlFindUnitByActor(found);
                    fldAppendSceneGroupHandle(handle);
                    ((BattleActorHandle *)handle)->phase = 0x11;
                    ((BattleActorHandle *)handle)->flags |= 8;
                    btlAppendIndexListEntry(((BattleActorHandle *)handle)->actorIndices, (u32)((BattleActorHandle *)handle)->unit);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217EB8);

u32 btlGetSelectionEmptyValue(void) {
    s32 battle;
    u32 value;

    battle = btlGetRuntime();
    value = 0;
    if (*(s8 *)((BattleWork *)battle)->sub == '\0') {
        value = 100;
    }
    return value;
}

/* Kind is the first halfword of each 20-byte action record after the table header. */
typedef struct BtlActionKindRow {
    s16 kind;
    u8 pad02[18];
} BtlActionKindRow;

typedef struct BtlActionKindTable {
    u8 pad00[0x2C];
    BtlActionKindRow rows[1]; /* The resource contains additional records. */
} BtlActionKindTable;

s32 btlShiftUnitUpForScriptAction(u8 *command) {
    BtlUnit *user = btlGetTargetUnitForLink(command);
    u8 *table;
    s32 kind;
    f32 pos[4];

    if (!(user->flags & 0x400)) {
        return 0;
    }
    if (user->mode == 0x5F || user->mode == 0x101) {
        table = btlGetSideIndexedActorStatusTable(user->resourceKind, user->resourceIndex);
        if (btlHasLinkedEffectNodeTrigger(command) == 0) {
            kind = ((BtlActionKindTable *)table)->rows[((BtlLinkedCommand *)command)->link->slot].kind;
            if (kind == 2 || kind == 7) {
                func_001E3108(user, pos);
                pos[2] += 350.0f;
                btlSetUnitPosition(user, pos);
            }
        }
        return 0;
    }
    return 0;
}

extern s32 btlRestoreUnitMinimumValueAndClearStatus();

s64 btlRestoreEnemyUnitWhenModelFlagSet(s32 battler, s32 resource) {
    if (((BtlUnit *)battler)->flags & 0x400) {
        if (mdlFlagTest(0x82b)) {
            return btlRestoreUnitMinimumValueAndClearStatus(battler, resource);
        }
    }
}

s32 func_00218150(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *battler;
    s32 count;
    if (work->phaseControl != 1) {
        return -1;
    }
    count = 0;
    for (battler = work->actorList; battler != 0; battler = battler->nextActor) {
        u32 flags = battler->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    if (!(battler->conditionFlags & 0x2000)) {
                        return -1;
                    }
                    count++;
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    mdlFlagSet(0x804);
    return 6;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002181E8);

void func_00218250(void) {
    s32 battle;

    battle = btlGetRuntime();
    *(u32 *)((BattleWork *)battle)->sub = 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", btlClaimCommandSlot);

void btlStartReadyUnitAction(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)work->sub;
    BtlUnit *unit;
    s32 handle;

    if (ctrl->unit != 0) {
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            u32 flags = unit->flags;

            if (!(flags & 1)) {
                continue;
            }
            if (!(flags & 0x400)) {
                continue;
            }
            if (flags & 0xE0) {
                continue;
            }
            break;
        }
        if (unit != 0) {
            handle = btlFindUnitByActor(unit);
            fldAppendSceneGroupHandle(handle);
            ((BattleActorHandle *)handle)->phase = 0x11;
            ((BattleActorHandle *)handle)->flags |= 8;
            btlAppendIndexListEntry(((BattleActorHandle *)handle)->actorIndices, (u32)((BattleActorHandle *)handle)->unit);
            ctrl->prevUnit = ctrl->unit;
            ctrl->unit = 0;
        }
    }
}

void btlStartPrevUnitScriptAction(BattleActorHandle *handle) {
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub;
    s32 script;
    BattleTaskControl *task;

    if (ctrl->prevUnit == 0) {
        return;
    }
    if (btlIsActiveActor(ctrl->prevUnit) == 0) {
        return;
    }
    if ((handle->pendingFlags & 8) == 0) {
        return;
    }
    script = -1;
    switch (ctrl->prevUnit->mode) {
    case 0x105:
    case 0x139:
        script = btlFindScriptResource(D_00436CC8);
        break;
    case 0x106:
    case 0x13A:
        script = btlFindScriptResource(D_00436CD0);
        break;
    case 0x104:
    case 0x138:
        script = btlFindScriptResource(D_00436CD8);
        break;
    }
    if (script != -1) {
        task = btlCreateActionTask(handle, script);
        task->targetId = handle->unit->owner;
        task->control = 0x14;
        btlStartTask(task);
    }
    ctrl->prevUnit = 0;
    handle->flags &= ~8;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218520);

s32 btlOverrideSpecialModeCheckResult(BtlUnit *unit, s32 kind, s32 fallback) {
    if (kind == 0xB) {
        if (unit->flags & 0x400) {
            switch (unit->mode) {
            case 0x104:
            case 0x105:
            case 0x106:
            case 0x138:
            case 0x139:
            case 0x13A:
                return 1;
            }
        }
    }
    return fallback;
}

s32 func_00218690(void) {
    return ((BattleWork *)btlGetRuntime())->mode == 0x303 ? 1 : 2;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002186C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218798);

void func_00218968(void) {
    func_0011AEE0(1);
}

s32 btlOverrideActionResultForEnemyMode(s32 battler, s32 action, s32 defaultValue) {
    if (action == 11) {
        if (((BtlUnit *)battler)->flags & 0x400) {
            if (((BtlUnit *)battler)->mode == 0x107) {
                return 1;
            }
        }
    }
    return defaultValue;
}

extern s32 btlIsCurrentValueBelowQuarterThreshold(u8 *);

s32 btlIsUnitListReady(void) {
    BtlUnit *unit;
    for (unit = ((BattleWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        u32 flags = unit->flags;
        if (!(flags & 1)) {
            continue;
        }
        if (!(flags & 0x200)) {
            continue;
        }
        if (flags & 0xE0) {
            continue;
        }
        if (unit->conditionFlags != 0) {
            continue;
        }
        if (!(flags & 0x1000)) {
            if (btlIsCurrentValueBelowQuarterThreshold((u8 *)unit) != 0) {
                continue;
            }
        }
        if (unit->mode == 1) {
            break;
        }
    }
    return unit == 0;
}

void btlAttachActionEffectToUnit(BtlUnit *unit) {
    f32 vec[3];
    ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit = unit;
    /* Required to match: update flags/stateFlags as words 0/1 of flags64. */
    ((u32 *)&unit->flags64)[0] &= ~0x100;
    ((u32 *)&unit->flags64)[0] &= ~8;
    ((u32 *)&unit->flags64)[1] |= 0x180;
    unit->conditionFlags = 0;
    vec[0] = 0.0f;
    vec[1] = 10000.0f;
    vec[2] = -10000.0f;
    effObjSetInnerFirstVec(unit->effectObject, vec);
}

void btlCommitSelectedUnit(void) {
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub;
    BtlUnit *unit = ctrl->unit;

    if (unit != 0) {
        /* Direct union-component updates change the retail instruction sequence. */
        ((u32 *)&unit->flags64)[1] &= ~0x80;
        ((u32 *)&unit->flags64)[1] &= ~0x100;
        ((u32 *)&unit->flags64)[0] |= 0x100;
        ctrl->unit = 0;
        func_001E2758(unit);
        ((u32 *)&unit->flags64)[0] |= 8;
        if (ctrl->pending != 0) {
            *(u16 *)unit->pad126 = 1;
            ctrl->pending = 0;
        }
    }
}

void btlResetUnitSelectionStateAndSetMode(void) {
    s32 state = btlGetRuntime();
    s32 resource = (s32)((BattleWork *)state)->sub;
    ((BtlSelectCtrl *)resource)->unit = 0;
    ((BtlSelectCtrl *)resource)->prevUnit = 0;
    ((BtlSelectCtrl *)resource)->pending = 0;
    func_0011AEE0(6);
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218BA8);

s32 btlFlagAbadonHpMpTrigger(s32 unit, s32 unused, s32 action) {
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub;
    if (action != 0x17A) {
        return 0;
    }
    if ((((BtlUnit *)unit)->flags & 0x400) && ((BtlUnit *)unit)->mode == 0x108 &&
        btlHasActiveSubtask() != 0) {
        ctrl->pending = 1;
        btlBossDebugPrintf("btl:ABADON HpMp 1\n");
    }
    return 0;
}

typedef struct BattleActionRecord {
    u8 pad0[8];
    u32 flags;
    u8 padC[0xC];
    BtlUnit *unit;
} BattleActionRecord;

s32 btlCheckActionRecordUnit(BattleActionRecord *record) {
    if ((record->flags & 8) == 0) {
        return -1;
    }
    return ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit == record->unit ? 12 : -1;
}

extern u8 *btlCreateCommandSoundUpdateTask(void);

extern u8 *btlCreateSecondaryCommandSoundTask(void);

extern u8 *btlCreateCommandSoundTask(u8 *, s32);

extern u8 *btlCreateEffObjB(s32, s32);

extern u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

s32 btlStartActionRecordTasks(BattleActionRecord *record) {
    u8 *task;
    if (!(record->flags & 8)) {
        return -1;
    }
    if (((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit != record->unit) {
        return -1;
    }
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask((u8 *)record, 9));
    btlStartTask(btlCreateEffObjB((s32)record->unit, 0xD8));
    task = fldCreateSceneGroupAction((u8 *)record, 0x64, 1);
    ((BattleTaskControl *)task)->control = 0x16;
    btlStartTask(task);
    return 0x1B;
}

s32 btlGetSpecialEnemySubtaskGuardResponse(BtlUnit *unit) {
    if (unit == 0) {
        return 0xF;
    }
    if (unit->flags & 0x400) {
        if (unit->mode == 0x108) {
            if (btlHasActiveSubtask() != 0) {
                return 0xF;
            }
        }
    }
    return -1;
}

s32 btlIsSpecialEnemyActionCode(s32 battler, s32 action) {
    if ((((BtlUnit *)battler)->flags & 0x400) == 0) {
        return 0;
    }
    if (((BtlUnit *)battler)->mode != 0x108) {
        return 0;
    }
    return action == 15;
}

s32 btlSelectSoleEligibleActor(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *actor;
    s32 count;
    u64 last;
    u64 current;
    if (btlHasActiveSubtask() != 0) {
        current = func_00219318();
        last = 0;
        for (actor = work->actorList, count = 0; actor != 0; actor = actor->nextActor) {
            u32 flags = actor->flags;
            if (flags & 1) {
                if (flags & 0x200) {
                    if (!(flags & 0xE0)) {
                        if (!(actor->conditionFlags & 0x800)) {
                            count++;
                            last = (u64)actor;
                        }
                    }
                }
            }
        }
        if (count == 1) {
            if (last == 0 || last == current) {
                return 7;
            }
        }
    }
    return -1;
}

s32 btlRaiseUnitForCommandSlot(u8 *command) {
    BtlUnit *user = btlGetTargetUnitForLink(command);
    u8 *table;
    s32 kind;
    f32 pos[4];

    if (!(user->flags & 0x400)) {
        return 0;
    }
    if (user->mode == 0x108) {
        table = btlGetSideIndexedActorStatusTable(user->resourceKind, user->resourceIndex);
        if (btlHasLinkedEffectNodeTrigger(command) == 0) {
            kind = ((BtlActionKindTable *)table)->rows[((BtlLinkedCommand *)command)->link->slot].kind;
            if (kind == 2 || kind == 7) {
                func_001E3108(user, pos);
                pos[2] += 1250.0f;
                btlSetUnitPosition(user, pos);
            }
        }
        return 0;
    }
    return 0;
}

/* One-shot linked effect: its parent action, elapsed counter and spawn latch. */
typedef struct BtlLinkedEffectTask {
    u8 pad00[0x114];
    BattleActionRecord *record; /* 0x114 */
    u8 pad118[0x18];
    s32 elapsed; /* 0x130 */
    u8 pad134[8];
    s32 spawned; /* 0x13C */
} BtlLinkedEffectTask;

s32 btlSpawnLinkedActionEffect(u8 *task) {
    if (((BtlLinkedEffectTask *)task)->spawned == 0) {
        if (func_001E2E58((u8 *)((BtlLinkedEffectTask *)task)->record->unit, 0x10) + 0xF <=
            ((BtlLinkedEffectTask *)task)->elapsed) {
            btlSetEffectCameraKeys(task, -6.8f, -476.8f, -525.0f, 0.184f, 0.008f, -0.011f, 0.974f, 0.3f,
                          -214.2f, -1419.8f, -0.101f, 0.012f, -0.013f, 0.986f, 40.0f, 12.0f);
            ((BtlLinkedEffectTask *)task)->elapsed = 0;
            ((BtlLinkedEffectTask *)task)->spawned = 1;
        }
    }
    return 1;
}

/* The sound and model-load tasks both store their caller-supplied owner at +8. */
typedef struct BtlOwnedTask {
    u8 pad00[8];
    u64 owner;
    u8 pad10[0x28];
    u64 result; /* 0x38: returned by the model-load task starter */
} BtlOwnedTask;

extern u8 *sndCreateStationedSeTask(s32);

void btlStartActionRecordSoundTask(BattleActionRecord *record, u64 owner, s32 controlBase) {
    BtlUnit *unit;
    u8 *task;
    if (record->flags & 8) {
        unit = record->unit;
        if (unit->flags & 0x400) {
            if (unit->mode == 0x108) {
                task = sndCreateStationedSeTask(((BattleWork *)btlGetRuntime())->soundTaskBase + 6);
                ((BtlOwnedTask *)task)->owner = owner;
                task[0] = 4;
                ((BattleTaskControl *)task)->control = controlBase + 0x28;
                btlStartTask(task);
            }
        }
    }
}

void btlResetActionEffectOnUnit(void) {
    BtlUnit *unit = ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit;
    if (unit != NULL) {
        f32 vec[3];
        vec[0] = 0.0f;
        vec[1] = 10000.0f;
        vec[2] = -10000.0f;
        /* As above, preserve the original 32-bit writes to the status pair. */
        ((u32 *)&unit->flags64)[0] &= ~8;
        ((u32 *)&unit->flags64)[1] |= 0x180;
        effObjSetInnerFirstVec(unit->effectObject, vec);
    }
}

void func_00219278(void) {
    btlResetActionEffectOnUnit();
}

s32 btlHasActiveSubtask(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BattleSub *sub;
    if (work->mode != 0x306) {
        return 0;
    }
    sub = work->sub;
    if (sub == 0) {
        return 0;
    }
    return sub->task != 0;
}

u32 btlGetSubtaskTargetMode(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    if (work->mode != 0x306) {
        return 0;
    }
    if (work->sub == 0) {
        return 0;
    }
    return work->sub->targetMode;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219318);

void btlClearSubtaskHandle(void) {
    BattleWork *work;

    work = (BattleWork *)btlGetRuntime();
    work->sub->task = 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", btlClaimCommandSlotAndTarget);

s32 btlSetSubtaskControlEnabled(s32 unused, s32 ignored, s32 action) {
    BattleSub *sub = ((BattleWork *)btlGetRuntime())->sub;
    switch (action) {
    case 0x129:
        sub->controlEnabled = 1;
        break;
    case 0x12a:
        sub->controlEnabled = 0;
        break;
    }
    return 0;
}

void btlStartReadyUnitActionCopy(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)work->sub;
    BtlUnit *unit;
    s32 handle;

    if (ctrl->unit != 0) {
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            u32 flags = unit->flags;

            if (!(flags & 1)) {
                continue;
            }
            if (!(flags & 0x400)) {
                continue;
            }
            if (flags & 0xE0) {
                continue;
            }
            break;
        }
        if (unit != 0) {
            handle = btlFindUnitByActor(unit);
            fldAppendSceneGroupHandle(handle);
            ((BattleActorHandle *)handle)->phase = 0x11;
            ((BattleActorHandle *)handle)->flags |= 8;
            btlAppendIndexListEntry(((BattleActorHandle *)handle)->actorIndices, (u32)((BattleActorHandle *)handle)->unit);
            ctrl->prevUnit = ctrl->unit;
            ctrl->unit = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002195E0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219760);

s32 btlGetSubtaskActorMotionClass(void) {
    s32 *slot = (s32 *)((BattleWork *)btlGetRuntime())->sub;
    if (slot[2] == 0) {
        return 0x64;
    }
    if (btlIsActiveActor(slot[2]) == 0) {
        return 0x64;
    }
    switch (((BtlUnit *)slot[2])->mode) {
    case 0x109:
        return 0x64;
    case 0x10A:
        return 0x63;
    case 0x132:
        return 0x63;
    case 0x133:
        return 0x62;
    case 0x135:
        return 0x63;
    case 0x136:
        return 0x62;
    }
    return 0x64;
}

extern char D_0041A378[]; /* "md_01all_02" */
extern u64 dds3GetWorldSecondaryObject(void);
extern s32 dds3FindIndexedObjectChainNodeByName(u64, s32, char *);
extern void evtSetObjectTransitionWork(s32, s32);

s32 func_002198D8(u8 *unit) {
    s32 handle;
    if (!(((BtlUnit *)unit)->flags & 0x400)) {
        return 1;
    }
    if (((BattleWork *)btlGetRuntime())->state22C == 5) {
        return 1;
    }
    handle = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(), 6, D_0041A378);
    if (handle == 0) {
        return 1;
    }
    evtSetObjectTransitionWork(handle, 1);
    return 1;
}

s32 func_00219950(u8 *unit) {
    s32 handle;
    if (!(((BtlUnit *)unit)->flags & 0x400)) {
        return 1;
    }
    if (((BattleWork *)btlGetRuntime())->state22C == 5) {
        return 1;
    }
    handle = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(), 6, D_0041A378);
    if (handle == 0) {
        return 1;
    }
    evtSetObjectTransitionWork(handle, 2);
    return 1;
}

s32 btlTriggerLinkedActionMotionAlternate(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    if ((link->unit->flags & 0x200) != 0 &&
        btlGetIndexListCount(link->targetHandle) == 1) {
        BtlUnit *target = (BtlUnit *)btlGetIndexListEntry(link->targetHandle, 0);
        if ((target->flags & 0x400) != 0) {
            if ((link->unit->flags & 0x1000) == 0) {
                return 0;
            }
            if (target->mode != 0x136) {
                return 0;
            }
            btlFaceLinkedTargetAndFlagDirection((u8 *)command, (u8 *)command);
            return 1;
        }
    }
    return 0;
}

extern void func_00217470(s32, s32, f32, f32, f32);

s32 btlTriggerLinkedActionMotion(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    BtlUnit *actor = link->unit;
    if ((actor->flags & 0x200) != 0 &&
        btlGetIndexListCount(link->targetHandle) == 1) {
        BtlUnit *target = (BtlUnit *)btlGetIndexListEntry(link->targetHandle, 0);
        if ((target->flags & 0x400) != 0) {
            if ((link->unit->flags & 0x1000) == 0) {
                return 0;
            }
            if (target->mode != 0x136) {
                return 0;
            }
            func_00217470((s32)command, (s32)command, 0.0f, 0.1499999911f, 35.0f);
            command->camera.distance += 150.0f;
            func_001E88A8(command);
            return 1;
        }
    }
    return 0;
}

s32 btlStartLinkedActionMotionPrimary(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    if (btlIsActorCategoryMarked((s32)command) || (link->unit->flags & 0x200)) {
        return 0;
    }
    switch (command->actionCode) {
    case 0x10C:
        command->motionProgress = 0;
        return 0;
    case 0x17E:
        btlPrepareRandomizedActionCameraPose((s32)command, (s32)command + 0x30, (s32)command + 0xC0);
        command->unk12C = 4;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219BD0);

extern s32 btlHasMarkedEntry14(s32, s32);

extern void btlClearRuntimeFlag2000(void);

extern void func_001EC868(s32, s32, f32);

s32 btlAdvanceTimedActionState(BtlLinkedCommand *command) {
    if (command->actionCode != 0x10c) {
        return 0;
    }
    if (btlHasMarkedEntry14((s32)command, 0x10c) != 0) {
        if (command->motionProgress >= 0x12) {
            btlClearRuntimeFlag2000();
            func_001EC868((s32)command, (s32)command, 0.0f);
        }
        command->motionProgress++;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_002112C8", D_0041A378);

s32 btlRemapCommandKind(BtlUnit *unit, s32 kind) {
    u32 flags = unit->flags;
    BattleSub *sub;

    if (!(flags & 0x400)) {
        return kind;
    }
    if (!(flags & 2)) {
        return kind;
    }
    sub = ((BattleWork *)btlGetRuntime())->sub;
    switch (kind) {
    case 2:
    case 9:
    case 10:
        return 0;
    case 13:
        return -1;
    case 11:
        if (unit->mode == 0x136) {
            if (sub->controlEnabled != 0) {
                return 0x14;
            }
        }
        return 1;
    default:
        if (unit->mode != 0x136) {
            return kind;
        }
        if (kind == 1) {
            if (sub->controlEnabled != 0) {
                return 0x14;
            }
        }
        if (kind == 0x12) {
            if (sub->controlEnabled != 0) {
                return 0x15;
            }
        }
        return kind;
    }
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219E38);

s32 func_00219F28(s32 battler, s32 action) {
    if ((((BtlUnit *)battler)->flags & 0x400) == 0) {
        return 0;
    }
    if (((BtlUnit *)battler)->mode != 0x136) {
        return 0;
    }
    return action == 19;
}

void btlRecenterActorsAroundLead(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *lead = 0;
    f32 shift;
    f32 pos[4];

    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x10D) {
                    lead = unit;
                    break;
                }
            }
        }
    }
    if (lead != 0) {
        func_001E3108(lead, pos);
        shift = -pos[0];
        pos[0] = 0;
        PCP_COPY_VECTOR(&lead->positionX, pos);
        btlSetUnitPosition(lead, pos);
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit != lead) {
                        func_001E3108(unit, pos);
                        pos[0] = pos[0] + shift;
                        PCP_COPY_VECTOR(&unit->positionX, pos);
                        btlSetUnitPosition(unit, pos);
                    }
                }
            }
        }
    }
}

s32 func_0021A098(void) {
    BtlUnit *unit = ((BattleWork *)btlGetRuntime())->actorList;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x10D) {
                    if (unit->flags & 0x20) {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->nextActor) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit->flags & 2) {
                        if (!(unit->flags & 0xE0)) {
                            if (unit->mode != 0x10D) {
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

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A1D8);

typedef struct NamedChunkNode {
    u8 pad0[4];
    struct NamedChunkNode *next;
    u8 pad8[4];
    struct NamedChunkNode *firstChild;
    u8 pad10[4];
    u16 flags;
    u8 pad16[6];
    u32 color;
} NamedChunkNode;



typedef struct NamedChunkData {
    u8 pad0[0xC];
    NamedChunkNode **entries;
} NamedChunkData;

typedef struct NamedChunkDescriptor {
    NamedChunkData *data;
    u8 pad4[0x18];
    s32 argument;
} NamedChunkDescriptor;

typedef struct NamedChunkHolder {
    u8 pad0[0x18];
    NamedChunkDescriptor *chunk;
} NamedChunkHolder;

/* Mode 0x111/0x112 select adjacent resource channels. */
struct BattleNamedResource {
    u8 pad0[0x8C];
    NamedChunkHolder *holder; /* 0x8C */
    u8 pad90[0x62];
    s16 group111;   /* 0xF2 */
    s16 group112;   /* 0xF4 */
    u8 padF6[0x14];
    s16 first111;   /* 0x10A */
    s16 first112;   /* 0x10C */
    u8 pad10E[0x14];
    s16 second111;  /* 0x122 */
    s16 second112;  /* 0x124 */
    u8 pad126[0x16];
    f32 value111;   /* 0x13C */
    f32 value112;   /* 0x140 */
};

s8 btlDispatchNamedChunkNode(s32 name) {
    BtlUnit *battler = ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit;
    BattleNamedResource *resource;
    NamedChunkDescriptor *chunk;
    s32 index;
    NamedChunkData *data;
    NamedChunkNode **entries;
    NamedChunkNode *node;
    if (battler == 0) {
        return 1;
    }
    if (!(battler->flags & 2)) {
        return 1;
    }
    resource = (BattleNamedResource *)battler->ext;
    chunk = resource->holder->chunk;
    index = sdfNamedChunkFindId(chunk, (void *)name);
    if (index == -1) {
        return 1;
    }
    data = chunk->data;
    entries = data->entries;
    node = entries[index];
    D_00438F84 = 1;
    func_0021A1D8((s32)node, chunk->argument);
    return D_00438F84;
}

void btlResetNamedChunkNodeTree(NamedChunkNode *node) {
    NamedChunkNode *child;

    node->color = 0x80808080;
    child = node->firstChild;
    node->flags = node->flags & 0xfffd;
    if (child != 0) {
        do {
            btlResetNamedChunkNodeTree(child);
            child = child->next;
        } while (child != node->firstChild);
    }
}

void btlClearNamedChunkFlags(s32 name) {
    BtlUnit *battler = ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit;
    if (battler != 0 && (battler->flags & 2) != 0) {
        BattleNamedResource *resource = (BattleNamedResource *)battler->ext;
        NamedChunkDescriptor *chunk = resource->holder->chunk;
        s32 index = sdfNamedChunkFindId(chunk, (void *)name);
        if (index != -1) {
            NamedChunkData *data = chunk->data;
            NamedChunkNode **entries = data->entries;
            btlResetNamedChunkNodeTree(entries[index]);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A490);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A778);

INCLUDE_ASM(const s32, "game/code_002112C8", btlReturnUnitToGroup);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A978);

void btlSetUnitResourceFloatByMode(BtlUnit *unit, s32 group, f32 value) {
    if (((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit != unit) {
        if (!(unit->flags & 0x400)) {
            evtUnitSetStoredParameter(unit->ext, group);
            evtSetTransitionMotionScale(unit->ext, value);
            return;
        }
        switch (unit->mode) {
        case 0x110:
            evtUnitSetStoredParameter(unit->ext, group);
            evtSetTransitionMotionScale(unit->ext, value);
            break;
        case 0x111:
            ((BattleNamedResource *)unit->ext)->group111 = group;
            ((BattleNamedResource *)unit->ext)->value111 = value;
            break;
        case 0x112:
            ((BattleNamedResource *)unit->ext)->group112 = group;
            ((BattleNamedResource *)unit->ext)->value112 = value;
            break;
        }
    }
}

void btlSetUnitResourceHalvesByMode(BtlUnit *unit, s32 first, s32 second) {
    if (((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit != unit) {
        if (!(unit->flags & 0x400)) {
            evtStoreUnitMotionShortParameters(unit->ext, first, second);
            return;
        }
        switch (unit->mode) {
        case 0x110:
            evtStoreUnitMotionShortParameters(unit->ext, first, second);
            break;
        case 0x111:
            ((BattleNamedResource *)unit->ext)->first111 = first;
            ((BattleNamedResource *)unit->ext)->second111 = second;
            break;
        case 0x112:
            ((BattleNamedResource *)unit->ext)->first112 = first;
            ((BattleNamedResource *)unit->ext)->second112 = second;
            break;
        }
    }
}

s32 btlResolveBoundActionCode(s32 battler, s32 action) {
    s32 slot = (s32)((BattleWork *)btlGetRuntime())->sub;
    if (((BtlSelectCtrl *)slot)->unit == (BtlUnit *)battler) {
        return -1;
    }
    if ((((BtlUnit *)battler)->flags & 0x400) == 0) {
        return action;
    }
    if (action == 15 && ((BtlUnit *)battler)->mode == 0x112) {
        return 20;
    }
    if (action == 1 || action == 11) {
        if (((BtlSelectCtrl *)slot)->pending != 0) {
            s32 selected = ((BtlUnit *)battler)->mode;
            if (selected < 0x113) {
                if (selected >= 0x111) {
                    return (((BtlUnit *)battler)->stateFlags & 0x80000) ? 10 : 21;
                }
            }
        }
    }
    return action;
}

s32 btlMotionOffsetForActor(s32 actor, s32 base) {
    u32 flags = ((BtlUnit *)actor)->flags;
    if ((flags & 1) == 0) {
        return base;
    }
    if ((flags & 0x400) == 0) {
        return base;
    }
    switch (((BtlUnit *)actor)->mode) {
    case 0x110:
        return base;
    case 0x111:
        return base + 100;
    case 0x112:
        return base + 200;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B368);

void func_0021B4A8(void) {
    func_0021B368();
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B4C0);

extern s64 btlAdvanceRuntimeSequenceCounter(void);

extern u32 btlCreateUnit(void);

extern void func_001AA898(s32, s32);

extern u8 *btlCreateModelLoadPollTask(s32, s32, s32, s32);

s64 btlEnsureHeroUnitTask(u64 owner) {
    s32 *slot = (s32 *)((BattleWork *)btlGetRuntime())->sub;
    u8 *task;
    if (*slot != 0) {
        return btlAdvanceRuntimeSequenceCounter();
    }
    *slot = btlCreateUnit();
    func_001AA898(*slot + 0x120, 0x110);
    task = btlCreateModelLoadPollTask(*slot, 1, 0x110, 0);
    if (owner != 0) {
        ((BtlOwnedTask *)task)->owner = owner;
        task[0] = 4;
    }
    btlStartTask(task);
    return ((BtlOwnedTask *)task)->result;
}

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CC0);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CC8);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CD0);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CD8);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CE0);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CE8);

