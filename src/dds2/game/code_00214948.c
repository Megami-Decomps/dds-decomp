#include "btl_motion_transform.h"
#include "common.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "btl_task_condition.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "eff_transform.h"
#include "dat_state.h"
#include "dat_command.h"
#include "evt_unit.h"
#include "mdl.h"
#include "dds3obj.h"



/* Existing link-only projection retained for the resource/sound queue.
 * No actor-status or linked-command member is accessed through this type.
 * Its unused historical suffix is not authority for another live owner. */
typedef struct ActionUnit {
    u8 pad0[8];
    u32 sequenceFlags; /* 0x08 */
    u32 actorFlags;    /* 0x0C */
    u8 pad10[8];
    s32 parentUnit;    /* 0x18: owner of this action */
    u8 pad1C[4];
    f32 verticalOffset; /* 0x20: lifted for special action visual */
    s32 parentAction;  /* 0x24 */
    u8 pad28[8];
    f32 position[4]; /* 0x30: formation actor position */
    u8 pad40[0x10];
    f32 cameraPointAHeight; /* 0x50 */
    u8 pad54[0x8C];
    f32 cameraPointBHeight; /* 0xE0 */
    u8 padE4[8];
    s32 actionStatus; /* 0xEC: checked before action 0x10 */
    u8 padF0[8];
    s16 motionStateA; /* 0xF8: cleared before restoring the unit's motion */
    s16 motionStateB; /* 0xFA: exact meaning not established */
    s32 savedMotionIndex; /* 0xFC: passed as the motion table index */
    s32 savedMotionB; /* 0x100: passed to the motion setter */
    f32 savedMotionScale; /* 0x104 */
    u64 ownerId;        /* 0x108: parent battle unit owner */
    u32 flags;
    u32 stateFlags;
    u8 pad118[8];
    u16 statusFlags;
    u8 pad122[2];
    u16 mode;
    u8 pad126[6];
    u16 motionRequest; /* 0x12C */
    u8 pad12E[2];
    u32 pendingAction;
    u32 action;
    u8 pad138[4];
    s32 actionTimer;
    u8 pad140[0x14];
    f32 cameraOffset; /* 0x154 */
    u8 pad158[0x1E8];
    u32 rendererHandle;
    u8 pad344[0x20];
    struct ActionUnit *next;
} ActionUnit;

/* Mode-specific scene storage retains primary actors. */
typedef struct BattleActionScene {
    u8 pad00[0x208];
    s32 soundSequence; /* 0x208: base ID for stationed sound */
    u8 pad20C[0x40];
    BtlUnit *units;
    u8 pad250[0x50];
    u32 mode;
    u8 pad2A4[0x474];
    u8 *state;
} BattleActionScene;

extern BtlUnit *btlGetSelectedOrCurrentActor(void);

extern s32 btlRollAiBucket(void);

extern u32 btlNextScaledRandom(u32);

extern s32 btlGetRuntime(void);

extern u32 func_001AC360(u64, BtlIndexList *, u64);

extern void func_002152D8(s32, s32);

extern s32 func_00215C70(s32, s32);

extern void *memset(void *, s32, u32);

extern s32 func_00216888(s32, s32);

extern void *func_00215118(BtlIndexList *, u16 *, u16);

extern char D_00436CC8[], D_00436CD0[], D_00436CD8[];

extern void *btlCreateActionTask(void *, s32);

extern s32 btlFindScriptResource(char *);

extern u8 *btlGetSideIndexedActorStatusTable(s32, s32);

extern s32 btlHasLinkedEffectNodeTrigger(BtlLinkedCommand *);

extern u32 btlAppendSelfAfterTargetScan();

extern void btlSelectLinkedTargets(s32, s32, s8);

extern s32 btlSelectTargetsExcludingActorUnit(s32);

extern s32 btlIsActorCategoryMarked(s32);

extern void btlPrepareRandomizedActionCameraPose(s32, s32, s32);

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

extern BtlRuntimeTask *sndCreateStationedSeTask(u32);

extern u32 btlCreateScriptResourceTask(BtlUnit *, u32);

extern void fldAppendSceneGroupHandle(ActionStateLink *);

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

extern s32 func_001E2E58(BtlUnit *, s32);

extern void btlSetEffectCameraKeys(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void evtUnitSetStoredParameter(EvtUnit *, s32);

extern void evtSetTransitionMotionScale(EvtUnit *, f32);

extern void evtStoreUnitMotionShortParameters(void *, s32, s32);

extern u32 btlGetSubtaskTargetMode(void);

extern u32 btlAppendEffectActorToCommandIndices(s32);

extern s32 mdlFlagTest(s32);

extern s32 btlReadCurrentUnitHp(DatPartyRecord *);

extern s32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *);

extern u16 btlReadCurrentUnitMp(DatPartyRecord *);

extern void func_001AC0F8(s32, BtlIndexList *, s32, s32, s32);

extern s32 btlUnitBlocksElementQuery(BtlUnit *, s32, s32);

extern s32 btlUnitBlocksElementQueryForGroup(BtlUnit *, s32, u32);

extern s32 btlTestSelectedItemCategoryMask(BtlUnit *, s32);

extern s32 btlElementToBitIndex(s32, s32);

extern s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *, u32);

extern s32 btlHasSpecialAbility274(BtlUnit *, u32);

extern s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *, u32);

extern s32 btlGroup400UnitHasAction(void *, s32);

extern s32 func_001B2F50(void *, s32);

extern s8 D_00438F84;

extern s32 sdfNamedChunkFindId(SdfModel *, const char *);

extern void btlFadeAndTintNamedChunkTree(SdfDrawNode *, s32);

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

extern BtlUnit *btlGetTargetUnitForLink(BtlLinkedCommand *);

extern void btlUnitFaceTarget(BtlUnit *, BtlUnit *);

extern void btlClearAllUnitDefeatCandidates(void);

extern void btlFlagUnitDefeatCandidate(BtlUnit *);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern BtlRuntimeTask *btlCreateUnitFadeOutTask(BtlUnit *, u32, u32);

extern u64 btlStartTask(void *);

extern void func_001E3108(void *, f32 *);

extern void btlSetUnitPosition(BtlUnit *, f32 *);

extern s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *, s32);

extern s32 btlIsActiveActor();

extern void effObjSetInnerFirstVec();

extern void btlRefreshUnitMotionSelection(BtlUnit *);

extern void fldAppendTaskToGroup(void *);

extern void btlDispatchStateHandler(void *, s32);

/* Return whether the native unit-status query intersects any requested action-mask bit. */ s32 btlUnitHasAnyStatusInMask(BtlUnit *unit, s32 actionMask);

s32 btlUnitBlocksElementQueryForGroup(BtlUnit *unit, s32 action, u32 mask) {
    u32 flags = unit->status.flags;
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
                            btlHasSpecialAbility274(unit, index) != 0 ||
                            btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (btlTestSelectedItemCategoryMask(unit, action) != 0 ||
                    btlHasMappedSpecialAbilityForSlot(unit, action) != 0 ||
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
        if (btlUnitBlocksElementQueryForGroup(battler, action, 0x200) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 btlAllUnitsPassCheck400(s32 unused, s32 action) {
    BtlUnit *battler = ((BattleWork *)btlGetRuntime())->actorList;
    for (; battler != 0; battler = battler->nextActor) {
        if (btlUnitBlocksElementQueryForGroup(battler, action, 0x400) == 0) {
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

/* Require the complete first queued-action word and unit status bit 0x1000.
 * Comparing only actions[0].actionId would discard the upper halfword. */
s32 btlActionMatchesUnit(s32 unit, s32 action) {
    if (btlActionScratchWork->actor->actions[0].word == action) {
        if (((BtlUnit *)unit)->status.stateFlags & 0x1000) {
            return 1;
        }
    }
    return 0;
}

s32 btlGroup400UnitHasAction(void *unit, s32 action) {
    btlGetRuntime();
    if ((btlUnitStatusPair((BtlUnit *)unit) & 0x421) == 0x401) {
        if (func_001B2F50(unit, action) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_00214C78(BtlUnit *actor, s32 command, BtlUnit **outFirst, BtlUnit **outSecond) {
    BtlState *state = (BtlState *)btlGetRuntime();
    DatAffinityRecord *affinity = &datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND];
    u32 requiredCount = 0;
    s32 *requirementCursor;
    u32 i;
    BtlUnit *head;
    BtlUnit *unit;

    requirementCursor = affinity->requirements;
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount < 2) {
        return 0;
    }

    unit = state->units;
    head = unit;
    for (; unit != NULL; unit = unit->nextActor) {
        BtlUnit *partner;

        if (!(unit->status.flags & 1)) {
            continue;
        }
        if (!(unit->status.flags & 0x400)) {
            continue;
        }
        if (unit->partyRecord.status & 0x2A0E) {
            continue;
        }
        if (actor == unit) {
            continue;
        }

        if (requiredCount == 3) {
            for (partner = head; partner != NULL; partner = partner->nextActor) {
                if (!(partner->status.flags & 1)) {
                    continue;
                }
                if (!(partner->status.flags & 0x400)) {
                    continue;
                }
                if (partner->partyRecord.status & 0x2A0E) {
                    continue;
                }
                if (actor == partner) {
                    continue;
                }
                if (unit == partner) {
                    continue;
                }
                if (outFirst != NULL) {
                    *outFirst = unit;
                }
                if (outSecond != NULL) {
                    *outSecond = partner;
                }
                return 1;
            }
        } else {
            if (outFirst != NULL) {
                *outFirst = unit;
            }
            if (outSecond != NULL) {
                *outSecond = NULL;
            }
            return 1;
        }
    }
    return 0;
}

extern void func_0035B6E0(const char *fmt, ...);

extern u32 fldCountSceneSlots(void);

extern s32 btlFindCommandPartnersByAffinity(BtlUnit *, s32, BtlUnit **, BtlUnit **);

extern s32 func_00214C78(BtlUnit *, s32, BtlUnit **, BtlUnit **);

extern s32 func_001ABDE8(BtlUnit *, BtlUnit *, BtlUnit *, s32, s32);

extern char D_00419C80[], D_00419CB0[], D_00419CD8[], D_00419D00[];

/* Whether unit can start combo command cmd with two partners, none of which may be
 * blocked by action mask 0x10 or by func_001ABDE8 against the other two. */
s32 func_00214DF8(BtlUnit *unit, s32 command) {
    BtlUnit *members[3];
    BtlState *state;
    u32 i = 0;

    memset(members, 0, sizeof(members));
    state = (BtlState *)btlGetRuntime();
    if ((btlUnitStatusPair(unit) & 0x421) == 0x401) {
        if (!(datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].flags & 2) && !(datBattleSceneRecords[state->battleMode].flags & 1)) {
            if (datCommandSelectors[command].kind != 1) {
                func_0035B6E0(D_00419C80);
                return 0;
            }
            if ((u32)(command - DAT_AFFINITY_FIRST_COMMAND) >= 0x75) {
                func_0035B6E0(D_00419CB0);
                return 0;
            }
            if (fldCountSceneSlots() < btlGetSlotValueAdjustedForSpecialAbility(unit, command)) {
                func_0035B6E0(D_00419CD8);
                return 0;
            }
            if (btlFindCommandPartnersByAffinity(unit, command, &members[1], &members[2]) != 0) {
                members[0] = unit;
                for (; i < 3; i++) {
                    if (members[i] != NULL && btlUnitHasAnyStatusInMask(members[i], 0x10)) {
                        return 0;
                    }
                }
                for (i = 0; i < 3; i++) {
                    if (members[i % 3] != NULL &&
                        func_001ABDE8(members[i % 3], members[(i + 1) % 3], members[(i + 2) % 3], 0, command)) {
                        return 0;
                    }
                }
                return 1;
            }
        } else if (func_00214C78(unit, command, &members[1], &members[2]) != 0) {
            members[0] = unit;
            for (i = 0; i < 3; i++) {
                if (members[i] != NULL && btlUnitHasAnyStatusInMask(members[i], 0x10)) {
                    return 0;
                }
            }
            for (i = 0; i < 3; i++) {
                if (members[i % 3] != NULL &&
                    func_001ABDE8(members[i % 3], members[(i + 1) % 3], members[(i + 2) % 3], 0, command)) {
                    return 0;
                }
            }
            return 1;
        }
    }
    func_0035B6E0(D_00419D00);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00215118);

BtlIndexList *btlBuildActorIndexListAndCount(s32 actor, u32 *matched, u32 *count) {
    u32 value;
    BtlIndexList *indexList;

    indexList = btlAllocateIndexList(0xd);
    value = func_001AC360(actor, indexList, 0);
    *matched = value;
    value = btlGetIndexListCount(indexList);
    *count = value;
    return indexList;
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419C80);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419CB0);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419CD8);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419D00);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419D20);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419DA0);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419E20);

INCLUDE_RODATA(const s32, "game/code_00214948", D_00419EA0);

INCLUDE_ASM(const s32, "game/code_00214948", func_002152D8);

INCLUDE_ASM(const s32, "game/code_00214948", func_00215C70);

/* Prefer the lowest nonzero HP among element-blocking units; otherwise use the list. */
s32 btlSelectLowestHealthElementBlockTarget(s32 actor, s32 action) {
    u32 matching;
    u32 count;
    u16 flags[12];
    BtlIndexList *list = btlBuildActorIndexListAndCount(actor, &matching, &count);
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
            BtlUnit *unit = btlGetIndexListEntry(list, i);
            if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
                u16 current = btlReadCurrentUnitHp(&unit->partyRecord);
                if (current <= best && current != 0) {
                    best = current;
                    found++;
                    bestIndex = i;
                }
            }
        }
        if (found != 0) {
            btlAppendIndexListEntry(((ActionStateLink *)actor)->indexWork.indices, btlGetIndexListEntry(list, bestIndex));
        } else {
            for (i = 0; i < count; i++) {
                flags[i] = 1;
            }
            btlAppendIndexListEntry(((ActionStateLink *)actor)->indexWork.indices, func_00215118(list, flags, count));
        }
        break;
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)actor)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

s32 btlSelectLowestHealthRateTarget(s32 task) {
    u32 matched;
    u32 count;
    BtlIndexList *list;
    u16 picked[12];
    s32 lowestPercent;
    u32 lowestIndex;
    u32 i;
    BtlUnit *target;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0:
        lowestPercent = 0x63;
        memset(picked, 0, sizeof(picked));
        lowestIndex = 0x20;
        for (i = 0; i < count; i++) {
            DatPartyRecord *stats = &((BtlUnit *)btlGetIndexListEntry(list, i))->partyRecord;
            s32 current = btlReadCurrentUnitHp(stats);
            s32 percent = current * 100 / btlComputeSkillAdjustedMaxHp(stats);

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
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, target);
        break;
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

s32 btlSelectTargetsByActionMask(s32 task, s32 mask) {
    u32 matched;
    u32 count;
    BtlIndexList *list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            if (btlUnitHasAnyStatusInMask(btlGetIndexListEntry(list, i), mask)) {
                picked[i] = 1;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

s32 btlSelectTargetsWithoutActionMask(s32 task, s32 mask) {
    u32 matched;
    u32 count;
    BtlIndexList *list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            if (!btlUnitHasAnyStatusInMask(btlGetIndexListEntry(list, i), mask)) {
                picked[i] = 1;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_002162C0);

/* Out-values of btlBuildActorIndexListAndCount, kept together in one local
 * so their stack slots carry the struct member alias sets. */
typedef struct BtlIndexQuery {
    u32 matching;
    u32 count;
} BtlIndexQuery;

/* Combines three AI predicate target rows before choosing an index-list entry. */
s32 func_002163C8(s32 actor) {
    u16 flags[3][12];
    s32 kinds[3];
    s32 values[3]; /* Decoded payloads are retained but not consumed by this selector. */
    s32 enabled[3];
    BtlIndexQuery query;
    BtlIndexList *list = btlBuildActorIndexListAndCount(actor, &query.matching, &query.count);
    u16 i;
    u16 j;

    switch (query.matching) {
    case 0:
        for (i = 0; i < 3; i++) {
            memset(flags[i], 0, sizeof(flags[i]));
            enabled[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            kinds[i] = datEnemyAiRecords[btlActionScratchWork->speciesId]
                .decisions[btlActionScratchWork->rowIndex].predicates[i];
            values[i] = kinds[i] & 0x3FFFFF;
            kinds[i] = (kinds[i] & 0xFFC00000) >> 22;
        }
        switch (btlActionScratchWork->conditionKind) {
        case 0:
            enabled[0] = enabled[1] = enabled[2] = 1;
            break;
        case 1:
            enabled[0] = enabled[1] = 1;
            break;
        case 2:
            enabled[0] = enabled[2] = 1;
            break;
        case 3:
            enabled[1] = enabled[2] = 1;
            break;
        case 4:
            enabled[0] = 1;
            break;
        case 5:
            enabled[1] = 1;
            break;
        case 6:
            enabled[2] = 1;
            break;
        default:
            enabled[0] = 0x50;
            break;
        }
        if (enabled[0] != 0x50) {
            for (i = 0; i < 3; i++) {
                if (enabled[i] != 0) {
                    switch (kinds[i]) {
                    case 1:
                    case 9:
                    case 21:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                        for (j = 0; j < query.count; j++) {
                            if (((BtlUnit *)btlGetIndexListEntry(list, j))->owner ==
                                ((ActionStateLink *)actor)->unit->owner) {
                                flags[i][j] = 1;
                            }
                        }
                        break;
                    /* The other recognized predicate kinds accept every list entry. */
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    default:
                        for (j = 0; j < query.count; j++) {
                            btlGetIndexListEntry(list, j);
                            flags[i][j] = 1;
                        }
                        break;
                    }
                }
            }
            for (i = 0; i < 12; i++) {
                flags[0][i] |= flags[1][i];
                flags[0][i] |= flags[2][i];
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)actor)->indexWork.indices,
                               func_00215118(list, flags[0], query.count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)actor)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}


s32 btlSelectLowestRankTarget(s32 task) {
    u32 matched;
    u32 count;
    BtlIndexList *list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};
        u16 best = 0x7FFF;
        u16 bestIndex = 0;

        for (i = 0; i < count; i++) {
            u16 value = ((BtlUnit *)btlGetIndexListEntry(list, i))->partyRecord.level;

            if (value <= best) {
                picked[bestIndex] = 0;
                best = value;
                picked[i] = 1;
                bestIndex = i;
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00216888);

s32 btlSelectTargetsExcludingActorUnit(s32 actor) {
    BtlIndexList *list = btlAllocateIndexList(13);
    u32 mode = 0;
    u32 count;
    u16 picked[12];
    u32 i;
    s32 found;

    if ((((ActionStateLink *)actor)->unit->status.flags & 0x400) == 0) {
        switch (((ActionStateLink *)actor)->unit->partyRecord.unitId) {
        case 3:
        case 5:
        case 6:
        case 7:
            mode = ((((ActionStateLink *)actor)->unit->status.flags & 0x1000) == 0 &&
                    (((ActionStateLink *)actor)->unit->partyRecord.flags & 0x10) == 0) ? 2 : 0;
            break;
        default:
            mode = 0;
            break;
        }
    }
    func_001AC0F8(actor, list, 1, 2, 0);
    count = btlGetIndexListCount(list);
    if (count == 0) {
        mode = 0;
    }
    switch (mode) {
    case 0:
        found = 0;
        memset(picked, 0, sizeof(picked));
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)btlGetIndexListEntry(list, i))->owner != ((ActionStateLink *)actor)->unit->owner) {
                picked[found] = 1;
                found++;
            }
        }
        if (found == 0) {
            btlAppendIndexListEntry(((ActionStateLink *)actor)->indexWork.indices, ((ActionStateLink *)actor)->unit);
            btlFreeIndexList(list);
            return 1;
        }
        btlAppendIndexListEntry(((ActionStateLink *)actor)->indexWork.indices, func_00215118(list, picked, count));
        btlFreeIndexList(list);
        return 1;
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)actor)->indexWork.indices, list);
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00216B40);

u32 btlAppendSelfAfterTargetScan(s32 battle) {
    BtlIndexList *list = btlAllocateIndexList(13);
    func_001AC0F8(battle, list, 1, 1, 0);
    btlGetIndexListCount(list);
    btlAppendIndexListEntry(((ActionStateLink *)battle)->indexWork.indices, ((ActionStateLink *)battle)->unit);
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
    BtlIndexList *list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};
        BtlUnit *unit;

        for (i = 0; i < count; i++) {
            unit = btlGetIndexListEntry(list, i);
            if (mode != 0) {
                if (unit->partyRecord.unitId == mode) {
                    picked[i] = 1;
                }
            } else if (((ActionStateLink *)task)->lastMode == unit->partyRecord.unitId) {
                picked[i] = 1;
            }
        }
        unit = (BtlUnit *)func_00215118(list, picked, count);
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, unit);
        if (mode == 0) {
            ((ActionStateLink *)task)->lastMode = unit->partyRecord.unitId;
        }
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

extern BtlUnit *btlGetEffectActor(void);

u32 btlAppendEffectActorToCommandIndices(s32 task) {
    BtlUnit *actor = btlGetEffectActor();
    btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, actor);
    return 1;
}

u32 btlAppendCurrentUnitIdToCommandIndices(s32 task) {
    BtlUnit *unit;

    unit = btlGetSelectedOrCurrentActor();
    btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, unit);
    return 1;
}

s32 btlSelectTargetsBlockingElement(s32 task, s32 action) {
    u32 matched;
    u32 count;
    BtlIndexList *list;
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
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
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
    BtlIndexList *list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            BtlUnit *unit = btlGetIndexListEntry(list, i);

            if (unit->status.flags & 0x200) {
                if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
                    picked[i] = 1;
                }
            } else {
                if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 1) {
                    picked[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
        break;
    }
    btlFreeIndexList(list);
    return 1;
}

void btlSelectLinkedTargets(s32 task, s32 unused, s8 linked) {
    u32 matched;
    u32 count;
    BtlIndexList *list;
    u16 i;

    list = btlBuildActorIndexListAndCount(task, &matched, &count);
    switch (matched) {
    case 0: {
        u16 picked[12] = {0};

        for (i = 0; i < count; i++) {
            BtlUnit *unit = btlGetIndexListEntry(list, i);

            if ((btlUnitStatusPair(unit) & 0x221) == 0x201) {
                if (linked == 0) {
                    if (unit->status.flags & 0x1000) {
                        picked[i] = 1;
                    }
                } else if (!(unit->status.flags & 0x1000)) {
                    picked[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((ActionStateLink *)task)->indexWork.indices, func_00215118(list, picked, count));
        break;
    }
    case 1:
    case 2:
        btlCopyIndexList(((ActionStateLink *)task)->indexWork.indices, list);
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
    if (datCommandSelectors[kind].kind != 1) {
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
        if (!(command->linkedA->status.flags & 0x1000)) {
            return command->linkedA;
        }
    }
    if (command->linkedB != NULL) {
        if (!(command->linkedB->status.flags & 0x1000)) {
            return command->linkedB;
        }
    }
    return command->link->unit;
}

void btlFaceLinkedTargetAndFlagDirection(BtlLinkedCommand *command, void *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = btlGetTargetUnitForLink(command);
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    if (!(user->status.flags & target->status.flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagMatchingUnitsDefeatCandidate(target->status.flags & 0x600);
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
        command->flags |= 0x200;
    } else {
        command->flags &= ~0x200;
    }
}

extern f32 func_00353040(f32);

extern f32 func_00353140(f32);

extern f32 func_00353228(f32);

extern void func_00336538(f32);

extern void btlFlagAllUnitsDefeatCandidate(void);



extern f32 btlUnitGetTopY(BtlUnit *);

/* Frame one unit approaching its target: out->position sits between the muzzle positions, pulled back along the
   line by 0.6 of the gap; angle (degrees) swings the pull-back, mirrored when the command faces left (0x200). */
void func_00217470(BtlLinkedCommand *command, BtlCamState *out, f32 frontLift, f32 backLift, f32 angle) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 dir[4];
    f32 extent;
    f32 length;

    user = btlGetTargetUnitForLink(command);
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    extent = user->reach * user->scale;
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
    length *= 0.6f;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    VU0_SCALE_VF_MFC1(vf10, length);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out->position);
    angle *= 0.017453293f;
    extent += length * func_00353140(angle);
    length *= func_00353040(angle);
    length += extent / func_00353228(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (command->flags & 0x200) {
        angle = -angle;
    }
    func_00336538(angle);
    VU0_LOAD_VF(vf10, dir);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
}

void func_00217650(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back, s8 mirror, f32 sideScale, f32 backLift) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 userExtent;
    f32 targetExtent;
    f32 angle;
    f32 length;
    f32 minDistance;

    btlFlagAllUnitsDefeatCandidate();
    front->fov = command->camera.fov;
    user = btlGetTargetUnitForLink(command);
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    userExtent = user->reach * user->scale;
    targetExtent = target->reach * target->scale;
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    userPos[1] = -btlUnitGetTopY(user);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, targetPos);
    targetPos[1] += target->height * target->scale * backLift;
    if (targetPos[0] <= userPos[0]) {
        userPos[0] -= userExtent;
        targetPos[0] += targetExtent * sideScale;
    } else {
        userPos[0] += userExtent;
        targetPos[0] -= targetExtent * sideScale;
    }
    VU0_LOAD_VF(vf10, targetPos);
    VU0_STORE_VF(vf10, front->position);
    VU0_LOAD_VF(vf11, userPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    front->distance = length;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, front->direction);
    angle = front->fov * 1.3333333f * 0.5f;
    front->distance += userExtent * 2.5f / func_00353228(angle);
    minDistance = targetExtent / func_00353228(angle);
    if (front->distance < minDistance) {
        front->distance = minDistance;
    }
    btlCopyMotionTransform(back, front);
    back->distance += 250.0f;
    if (mirror != 0) {
        btlAdjustCameraDirectionForDefaultPlane(front);
        btlAdjustCameraDirectionForDefaultPlane(back);
    }
    btlUnitFaceTarget(user, target);
    btlUnitFaceTarget(target, user);
}

/* Frame a two-unit exchange: place the camera pair between the units' muzzle positions and push it out far enough to see both. */
void btlBuildLinkedCommandCameraPair(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back, f32 sideScale, f32 backLift, f32 frontLift, s8 mirror, s8 swapRoles) {
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
        btlAdjustCameraDirectionForDefaultPlane(front);
        btlAdjustCameraDirectionForDefaultPlane(back);
    }
    btlUnitFaceTarget(user, target);
    btlUnitFaceTarget(target, user);
}

void func_00217B20(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back, s8 mirror, s8 swapRoles, f32 targetSideScale, f32 userSideScale, f32 backLift, f32 frontLift) {
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
        targetPos[0] = targetPos[0] + targetExtent * targetSideScale;
        userPos[0] = userPos[0] - userExtent * userSideScale;
    } else {
        targetPos[0] = targetPos[0] - targetExtent * targetSideScale;
        userPos[0] = userPos[0] + userExtent * userSideScale;
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
        btlAdjustCameraDirectionForDefaultPlane(front);
        btlAdjustCameraDirectionForDefaultPlane(back);
    }
    btlUnitFaceTarget(user, target);
    btlUnitFaceTarget(target, user);
}

void btlStartUnitActionIfPairedSelected(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *found;
    BtlUnit *other;
    ActionStateLink *handle;

    /* The selection gate tests the first byte of the stored unit pointer, not the whole pointer. */
    if (*(s8 *)work->sub == 0) {
        found = 0;
        other = 0;
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            u32 flags = unit->status.flags;

            if (flags & 1) {
                if (flags & 0x400) {
                    if (unit->partyRecord.unitId == 0x101) {
                        other = unit;
                    } else if (!(flags & 0xE0)) {
                        found = unit;
                    }
                }
            }
        }
        if (found != 0) {
            if (other != 0) {
                if (other->status.flags & 0xE0) {
                    handle = btlFindUnitByActor(found);
                    fldAppendSceneGroupHandle(handle);
                    handle->indexWork.phase = 0x11;
                    handle->flags |= 8;
                    btlAppendIndexListEntry(handle->indexWork.indices, handle->unit);
                }
            }
        }
    }
}

/* Queue the matching boss's resource/sound pair once its status mask passes.
 * Condition 5 lets the sound start when that task runs or is already gone.
 */
void func_00217EB8(ActionStateLink *action) {
    BattleActionScene *scene = (BattleActionScene *)btlGetRuntime();
    s8 *state = (s8 *)scene->state;
    BtlUnit *unit;
    BtlRuntimeTask *task;
    BtlRuntimeTask *sound;

    if (*state == 0) {
        unit = scene->units;
        if (unit != 0) {
            while (unit != 0) {
                if (unit->status.flags & 1) {
                    if (unit->status.flags & 0x400) {
                        if (unit->partyRecord.unitId == 0x101) {
                            break;
                        }
                    }
                }
                unit = unit->nextActor;
            }
            if (unit != 0) {
                if (unit->status.flags & 0xE0) {
                    task = (BtlRuntimeTask *)btlCreateScriptResourceTask(unit, 0x64);
                    task->ownerId = action->unit->owner;
                    task->startDelay = 0xE;
                    btlStartTask(task);
                    sound = sndCreateStationedSeTask(scene->soundSequence);
                    sound->startCondition.kind = BTL_TASK_CONDITION_HANDLE_RUNNING_OR_ABSENT;
                    sound->startCondition.value.handle = task->handle;
                    btlStartTask(sound);
                    action->flags &= ~8;
                    *state = 1;
                }
            }
        }
    }
}

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

s32 btlShiftUnitUpForScriptAction(BtlLinkedCommand *command) {
    BtlUnit *user = btlGetTargetUnitForLink(command);
    u8 *table;
    s32 kind;
    f32 pos[4];

    if (!(user->status.flags & 0x400)) {
        return 0;
    }
    if (user->partyRecord.unitId == 0x5F || user->partyRecord.unitId == 0x101) {
        table = btlGetSideIndexedActorStatusTable(user->resourceKind, user->resourceIndex);
        if (btlHasLinkedEffectNodeTrigger(command) == 0) {
            kind = ((BtlActionKindTable *)table)->rows[command->link->indexWork.slot].kind;
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

extern void btlRestoreUnitMinimumValueAndClearStatus(BtlUnit *, BtlOperandEntry *);

s64 btlRestoreEnemyUnitWhenModelFlagSet(BtlUnit *battler, BtlOperandEntry *resource) {
    if (battler->status.flags & 0x400) {
        if (mdlFlagTest(0x82b)) {
            btlRestoreUnitMinimumValueAndClearStatus(battler, resource);
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
        u32 flags = battler->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    if (!(battler->partyRecord.status & 0x2000)) {
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

INCLUDE_ASM(const s32, "game/code_00214948", func_002181E8);

void func_00218250(void) {
    s32 battle;

    battle = btlGetRuntime();
    *(u32 *)((BattleWork *)battle)->sub = 0;
}

void btlClaimCommandSlot(BtlUnit *unit, BtlOperandEntry *entry) {
    BtlUnit **state;

    if (unit->status.flags & 0x400) {
        state = (BtlUnit **)((BattleActionScene *)btlGetRuntime())->state;
        entry->flags &= ~1;
        entry->flags &= ~2;
        if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0)) {
            if (*state != 0 && *state != unit) {
                btlRestoreUnitMinimumValueAndClearStatus(unit, entry);
            } else {
                *state = unit;
                return;
            }
        }
    }
}

void btlStartReadyUnitAction(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)work->sub;
    BtlUnit *unit;
    ActionStateLink *handle;

    if (ctrl->unit != 0) {
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            u32 flags = unit->status.flags;

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
            handle->indexWork.phase = 0x11;
            handle->flags |= 8;
            btlAppendIndexListEntry(handle->indexWork.indices, handle->unit);
            ctrl->prevUnit = ctrl->unit;
            ctrl->unit = 0;
        }
    }
}

/* Start the previous selection's script when eligible, then consume the selection
 * even if no script was found. The script task's start delay is 20 ticks.
 */
void btlStartPrevUnitScriptAction(ActionStateLink *handle) {
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub;
    s32 script;
    BtlRuntimeTask *task;
    u32 flags;

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
    switch (ctrl->prevUnit->partyRecord.unitId) {
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
        task->ownerId = handle->unit->owner;
        task->startDelay = 0x14;
        btlStartTask(task);
    }
    flags = handle->flags;
    ctrl->prevUnit = 0;
    handle->flags = flags & ~8;
}

extern void evtPrepareUnitMotionState(EvtUnit *, s32, s32, s32, s32);



/* These two three-ID resource families retain the saved motion for selectors 16/17. */
void func_00218520(BtlUnit *unit, s32 selector, s32 firstParameter, s32 secondParameter, s32 mode, f32 frameStep) {
    if (unit->status.flags & 0x400) {
        if (selector < 18) {
            if (selector >= 16) {
                u32 resourceId = unit->resourceIndex;
                switch (resourceId) {
                case 0x104:
                case 0x105:
                case 0x106:
                case 0x138:
                case 0x139:
                case 0x13A:
                    selector = unit->effectIndex;
                    mode = 1;
                    firstParameter = unit->unkF8;
                    secondParameter = unit->unkFA;
                    break;
                }
            }
        }
    }
    evtPrepareUnitMotionState(unit->ext, selector, firstParameter, secondParameter, mode);
    unit->ext->owner->first->frameStep = frameStep;
    if (secondParameter == 0) {
        mdlAddEntryFlagged(unit->ext->owner, 0, selector);
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
}

s32 btlOverrideSpecialModeCheckResult(BtlUnit *unit, s32 kind, s32 fallback) {
    if (kind == 0xB) {
        if (unit->status.flags & 0x400) {
            switch (unit->partyRecord.unitId) {
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

extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *);

/* Enemy forms replace a few action modes according to their health state. */
s32 func_002186C0(BtlUnit *unit, s32 action) {
    u32 flags = unit->status.flags;
    if (!(flags & 0x400)) {
        return action;
    }
    if (!(flags & 2)) {
        return action;
    }
    switch (unit->partyRecord.unitId) {
    case 0x14B:
    case 0x14C:
    case 0x14D:
        switch (action) {
        case 9:
        case 10:
            return 0;
        case 11:
            return 1;
        case 13:
            return -1;
        }
        break;
    default:
        switch (action) {
        case 2:
        case 9:
            return btlIsCurrentValueBelowQuarterThreshold(unit) ? 10 : 0;
        case 13:
            return -1;
        }
        break;
    }
    return action;
}

extern void btlFlagAllUnitDefeatCandidatesTask();

extern u32 effMiscRandMod(void *, u32);

extern void btlBossDebugPrintf(const char *, ...);

/* Choose one of the special action's three camera paths. */
s32 func_00218798(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    if (btlIsActorCategoryMarked((s32)command) || (link->unit->status.flags & 0x200)) {
        return 0;
    }
    switch (command->actionCode) {
    case 0x211:
        btlFlagAllUnitDefeatCandidatesTask();
        switch (effMiscRandMod(0, 3)) {
        case 0:
            btlBossDebugPrintf("3SIK:LKG-0 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -970.8f, -594.5f, -817.3f, 0.169f, -0.431f, -0.096f, 0.871f, -515.3f,
                -871.1f, -1273.9f, 0.271f, -0.195f, -0.068f, 0.931f, 40.0f, 25.0f);
            return 1;
        case 1:
            btlBossDebugPrintf("3SIK:LKG-1 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                295.9f, -443.1f, 1327.6f, -0.028f, -0.976f, -0.143f, -0.091f, -543.2f,
                -558.4f, 1271.9f, 0.021f, -0.956f, -0.193f, 0.175f, 40.0f, 25.0f);
            return 1;
        case 2:
            btlBossDebugPrintf("3SIK:LKG-2 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                718.1f, -571.1f, 362.3f, -0.15f, -0.82f, -0.231f, -0.482f, 1404.2f,
                -884.0f, 160.6f, -0.198f, -0.691f, -0.192f, -0.653f, 40.0f, 15.0f);
            return 1;
        }
    }
    return 0;
}

void func_00218968(void) {
    ptyRebalanceFrontline(1);
}

s32 btlOverrideActionResultForEnemyMode(s32 battler, s32 action, s32 defaultValue) {
    if (action == 11) {
        if (((BtlUnit *)battler)->status.flags & 0x400) {
            if (((BtlUnit *)battler)->partyRecord.unitId == 0x107) {
                return 1;
            }
        }
    }
    return defaultValue;
}

s32 btlIsUnitListReady(void) {
    BtlUnit *unit;
    for (unit = ((BattleWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if (!(flags & 1)) {
            continue;
        }
        if (!(flags & 0x200)) {
            continue;
        }
        if (flags & 0xE0) {
            continue;
        }
        if (unit->partyRecord.status != 0) {
            continue;
        }
        if (!(flags & 0x1000)) {
            if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0) {
                continue;
            }
        }
        if (unit->partyRecord.unitId == 1) {
            break;
        }
    }
    return unit == 0;
}

extern void btlAttachActionEffectToUnit(BtlUnit *unit);

void btlAttachActionEffectToUnit(BtlUnit *unit) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlSelectCtrl *ctrl = &battle->effect->selection;
    f32 position[3];

    ctrl->unit = unit;
    unit->status.flags &= ~0x100;
    unit->status.flags &= ~8;
    unit->status.stateFlags |= 0x180;
    unit->partyRecord.status = 0;
    position[0] = 0.0f;
    position[1] = 10000.0f;
    position[2] = -10000.0f;
    effObjSetInnerFirstVec(unit->effectObject, position);
}

void btlCommitSelectedUnit(void) {
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub;
    BtlUnit *unit = ctrl->unit;

    if (unit != 0) {
        u32 state = unit->status.stateFlags;
        u32 flags = unit->status.flags | 0x100;
        state &= ~0x80;
        state &= ~0x100;
        unit->status.stateFlags = state;
        ctrl->unit = 0;
        unit->status.flags = flags;
        btlRefreshUnitMotionSelection(unit);
        unit->status.flags |= 8;
        if (ctrl->pending != 0) {
            unit->partyRecord.hp = 1;
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
    ptyRebalanceFrontline(6);
}

void func_00218BA8(BtlUnit *unit, u8 *arg1) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    u8 *sub = (u8 *)work->sub;
    BtlUnit *actor;
    u32 flags;
    u32 actorFlags;

    flags = *(u32 *)(arg1 + 0x28);
    if (flags & 0x2000) {
        btlAttachActionEffectToUnit(unit);
        *(s32 *)(sub + 4) = 0;
        sub[8] = 0;
    } else if (flags & 0x4000) {
        btlCommitSelectedUnit();
        *(s32 *)(sub + 4) = 0;
        sub[8] = 0;

        actor = work->actorList;
        while (actor != 0) {
            actorFlags = actor->status.flags;
            if (actorFlags & 1) {
                if (actorFlags & 0x400) {
                    if (actor->partyRecord.unitId == 0x108) {
                        break;
                    }
                }
            }
            actor = actor->nextActor;
        }
        if (actor != 0) {
            btlRefreshUnitMotionSelection(actor);
        }
    }
    if ((unit->status.flags & 0x400) && unit->partyRecord.unitId == 0x108) {
        s32 damage = *(s32 *)(sub + 4) - *(s32 *)arg1;
        *(s32 *)(sub + 4) = damage;
        btlBossDebugPrintf("btl:ABADON damage = %d\n", damage);
    }
}

s32 btlFlagAbadonHpMpTrigger(s32 unit, s32 unused, s32 action) {
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub;
    if (action != 0x17A) {
        return 0;
    }
    if ((((BtlUnit *)unit)->status.flags & 0x400) && ((BtlUnit *)unit)->partyRecord.unitId == 0x108 &&
        btlHasActiveSubtask() != 0) {
        ctrl->pending = 1;
        btlBossDebugPrintf("btl:ABADON HpMp 1\n");
    }
    return 0;
}

/* Pending command records return 12 only when their owner is the selected unit. */
s32 btlCheckActionRecordUnit(ActionStateLink *record) {
    if ((record->pendingFlags & 8) == 0) {
        return -1;
    }
    return ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit == record->unit ? 12 : -1;
}

extern u8 *btlCreateCommandSoundUpdateTask(void);

extern u8 *btlCreateSecondaryCommandSoundTask(void);

extern u8 *btlCreateCommandSoundTask(u8 *, s32);

extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);

extern BtlRuntimeTask *fldCreateSceneGroupAction(ActionStateLink *, u32, s32);

/* Start the selected action's task group; its scene action carries a 22-tick start delay. */
s32 btlStartActionRecordTasks(ActionStateLink *record) {
    BtlRuntimeTask *task;
    if (!(record->pendingFlags & 8)) {
        return -1;
    }
    if (((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit != record->unit) {
        return -1;
    }
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask((u8 *)record, 9));
    btlStartTask(btlCreateEffObjB(record->unit, 0xD8));
    task = fldCreateSceneGroupAction(record, 0x64, 1);
    task->startDelay = 0x16;
    btlStartTask(task);
    return 0x1B;
}

s32 btlGetSpecialEnemySubtaskGuardResponse(BtlUnit *unit) {
    if (unit == 0) {
        return 0xF;
    }
    if (unit->status.flags & 0x400) {
        if (unit->partyRecord.unitId == 0x108) {
            if (btlHasActiveSubtask() != 0) {
                return 0xF;
            }
        }
    }
    return -1;
}

s32 btlIsSpecialEnemyActionCode(s32 battler, s32 action) {
    if ((((BtlUnit *)battler)->status.flags & 0x400) == 0) {
        return 0;
    }
    if (((BtlUnit *)battler)->partyRecord.unitId != 0x108) {
        return 0;
    }
    return action == 15;
}

s32 btlSelectSoleEligibleActor(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *actor;
    s32 count;
    BtlUnit *last;
    BtlUnit *current;
    if (btlHasActiveSubtask() != 0) {
        current = btlGetSelectedOrCurrentActor();
        last = 0;
        for (actor = work->actorList, count = 0; actor != 0; actor = actor->nextActor) {
            u32 flags = actor->status.flags;
            if (flags & 1) {
                if (flags & 0x200) {
                    if (!(flags & 0xE0)) {
                        if (!(actor->partyRecord.status & 0x800)) {
                            count++;
                            last = actor;
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

s32 btlRaiseUnitForCommandSlot(BtlLinkedCommand *command) {
    BtlUnit *user = btlGetTargetUnitForLink(command);
    u8 *table;
    s32 kind;
    f32 pos[4];

    if (!(user->status.flags & 0x400)) {
        return 0;
    }
    if (user->partyRecord.unitId == 0x108) {
        table = btlGetSideIndexedActorStatusTable(user->resourceKind, user->resourceIndex);
        if (btlHasLinkedEffectNodeTrigger(command) == 0) {
            kind = ((BtlActionKindTable *)table)->rows[command->link->indexWork.slot].kind;
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

/* Install these camera keys once, after motion 0x10's duration plus 15 task ticks. */
s32 btlSpawnLinkedActionEffect(u8 *task) {
    if (((BtlLinkedCommand *)task)->motionProgress == 0) {
        if (func_001E2E58(((BtlLinkedCommand *)task)->link->unit, 0x10) + 0xF <=
            ((BtlLinkedCommand *)task)->state) {
            btlSetEffectCameraKeys((s32)task, -6.8f, -476.8f, -525.0f, 0.184f, 0.008f, -0.011f, 0.974f, 0.3f,
                          -214.2f, -1419.8f, -0.101f, 0.012f, -0.013f, 0.986f, 40.0f, 12.0f);
            ((BtlLinkedCommand *)task)->state = 0;
            ((BtlLinkedCommand *)task)->motionProgress = 1;
        }
    }
    return 1;
}

/* For the eligible action/unit pair, gate sound on a disappearing prerequisite
 * task and use delayBase + 40 as the signed start-delay countdown.
 */
void btlStartActionRecordSoundTask(ActionStateLink *record, u64 prerequisiteHandle, s32 delayBase) {
    BtlUnit *unit;
    u8 *task;
    if (record->pendingFlags & 8) {
        unit = record->unit;
        if (unit->status.flags & 0x400) {
            if (unit->partyRecord.unitId == 0x108) {
                task = (u8 *)sndCreateStationedSeTask(((BattleWork *)btlGetRuntime())->soundTaskBase + 6);
                ((BtlRuntimeTask *)task)->startCondition.value.handle = prerequisiteHandle;
                ((BtlRuntimeTask *)task)->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                ((BtlRuntimeTask *)task)->startDelay = delayBase + 0x28;
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
        unit->status.flags &= ~8;
        unit->status.stateFlags |= 0x180;
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

BtlUnit *btlGetSelectedOrCurrentActor(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *selected = (BtlUnit *)work->sub->task;
    BtlUnit *unit;

    if (selected != 0) {
        return selected;
    }
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    break;
                }
            }
        }
    }
    return unit;
}

void btlClearSubtaskHandle(void) {
    BattleWork *work;

    work = (BattleWork *)btlGetRuntime();
    work->sub->task = 0;
}

void btlClaimCommandSlotAndTarget(BtlUnit *unit, BtlOperandEntry *entry) {
    BtlUnit **state;

    if (unit->status.flags & 0x400) {
        state = (BtlUnit **)((BattleActionScene *)btlGetRuntime())->state;
        entry->flags &= ~1;
        entry->flags &= ~2;
        if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0)) {
            if (*state != 0 && *state != unit) {
                btlRestoreUnitMinimumValueAndClearStatus(unit, entry);
            } else {
                *state = unit;
                state[2] = unit;
                return;
            }
        }
    }
}

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
    ActionStateLink *handle;

    if (ctrl->unit != 0) {
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            u32 flags = unit->status.flags;

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
            handle->indexWork.phase = 0x11;
            handle->flags |= 8;
            btlAppendIndexListEntry(handle->indexWork.indices, handle->unit);
            ctrl->prevUnit = ctrl->unit;
            ctrl->unit = 0;
        }
    }
}

extern BtlRuntimeTask *btlCreateImmediateCompletionTask(void);

/* Once the previous selected unit is active, queue its mode's script resource and
 * stationed sound, then an immediate task that waits for the resource task to go.
 */
void func_002195E0(ActionStateLink *record) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlSelectCtrl *ctrl = (BtlSelectCtrl *)work->sub;
    s32 resource;
    s32 variant;
    BtlRuntimeTask *task;
    BtlRuntimeTask *follow;
    u32 flags;

    if (ctrl->prevUnit == 0) {
        return;
    }
    if (btlIsActiveActor(ctrl->prevUnit) == 0) {
        return;
    }
    if (!(record->pendingFlags & 8)) {
        return;
    }
    switch (ctrl->prevUnit->partyRecord.unitId) {
    case 0x109:
    case 0x131:
    case 0x134:
        resource = 0x64;
        break;
    case 0x10A:
    case 0x132:
    case 0x135:
        resource = 0x63;
        break;
    case 0x133:
    case 0x136:
        resource = 0x62;
        break;
    default:
        return;
    }
    switch (ctrl->prevUnit->partyRecord.unitId) {
    case 0x109:
    case 0x10A:
    case 0x134:
        variant = 0;
        break;
    case 0x131:
    case 0x132:
    case 0x133:
    case 0x135:
        variant = 1;
        break;
    case 0x136:
        variant = 2;
        break;
    default:
        return;
    }
    task = (BtlRuntimeTask *)btlCreateScriptResourceTask(ctrl->prevUnit, resource);
    task->startDelay = 0xE;
    btlStartTask(task);
    follow = sndCreateStationedSeTask(work->soundTaskBase + variant);
    follow->startDelay = 0xE;
    btlStartTask(follow);
    follow = btlCreateImmediateCompletionTask();
    follow->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    follow->startCondition.value.handle = task->handle;
    follow->ownerId = record->unit->owner;
    btlStartTask(follow);
    flags = record->flags;
    ctrl->prevUnit = 0;
    record->flags = flags & ~8;
}

extern f32 D_003BF6A0[];

extern void btlSetUnitRotation(BtlUnit *, s128 *);

/* Reset the special actor groups to their fixed facing and battle position. */
void func_00219760(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;

    for (; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 id = (u16)unit->partyRecord.unitId;
                if (id < 0x109) {
                    continue;
                }
                if (id >= 0x10B) {
                    if (id >= 0x137) {
                        continue;
                    }
                    if (id < 0x131) {
                        continue;
                    }
                }
                btlSetUnitRotation(unit, (s128 *)D_003BF6A0);
                /* Stop automatic facing before installing the reset pose. */
                unit->status.flags &= ~0x80000;
                unit->position[0] = 0.0f;
                unit->position[1] = 0.0f;
                unit->position[2] = 250.0f;
                unit->position[3] = 0.0f;
                btlSetUnitPosition(unit, unit->position);
            }
        }
    }
}

s32 btlGetSubtaskActorMotionClass(void) {
    s32 *slot = (s32 *)((BattleWork *)btlGetRuntime())->sub;
    if (slot[2] == 0) {
        return 0x64;
    }
    if (btlIsActiveActor(slot[2]) == 0) {
        return 0x64;
    }
    switch (((BtlUnit *)slot[2])->partyRecord.unitId) {
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

extern void *dds3GetWorldSecondaryObject(void);

extern void dds3SetObjectPayloadWord8(EffWorldNode *object, u32 value);

s32 func_002198D8(u8 *unit) {
    EffWorldNode *object;
    if (!(((BtlUnit *)unit)->status.flags & 0x400)) {
        return 1;
    }
    if (((BattleWork *)btlGetRuntime())->state22C == 5) {
        return 1;
    }
    object = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(), 6, (const u8 *)D_0041A378);
    if (object == NULL) {
        return 1;
    }
    dds3SetObjectPayloadWord8(object, 1);
    return 1;
}

s32 func_00219950(u8 *unit) {
    EffWorldNode *object;
    if (!(((BtlUnit *)unit)->status.flags & 0x400)) {
        return 1;
    }
    if (((BattleWork *)btlGetRuntime())->state22C == 5) {
        return 1;
    }
    object = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(), 6, (const u8 *)D_0041A378);
    if (object == NULL) {
        return 1;
    }
    dds3SetObjectPayloadWord8(object, 2);
    return 1;
}

s32 btlTriggerLinkedActionMotionAlternate(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    if ((link->unit->status.flags & 0x200) != 0 &&
        btlGetIndexListCount(link->indexWork.indices) == 1) {
        BtlUnit *target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
        if ((target->status.flags & 0x400) != 0) {
            if ((link->unit->status.flags & 0x1000) == 0) {
                return 0;
            }
            if (target->partyRecord.unitId != 0x136) {
                return 0;
            }
            btlFaceLinkedTargetAndFlagDirection(command, command);
            return 1;
        }
    }
    return 0;
}

s32 btlTriggerLinkedActionMotion(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    BtlUnit *actor = link->unit;
    if ((actor->status.flags & 0x200) != 0 &&
        btlGetIndexListCount(link->indexWork.indices) == 1) {
        BtlUnit *target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
        if ((target->status.flags & 0x400) != 0) {
            if ((link->unit->status.flags & 0x1000) == 0) {
                return 0;
            }
            if (target->partyRecord.unitId != 0x136) {
                return 0;
            }
            func_00217470(command, &command->camera, 0.0f, 0.1499999911f, 35.0f);
            command->camera.distance += 150.0f;
            btlAdjustCameraDirectionForDefaultPlane(&command->camera);
            return 1;
        }
    }
    return 0;
}

s32 btlStartLinkedActionMotionPrimary(BtlLinkedCommand *command) {
    ActionStateLink *link = command->link;
    if (btlIsActorCategoryMarked((s32)command) || (link->unit->status.flags & 0x200)) {
        return 0;
    }
    switch (command->actionCode) {
    case 0x10C:
        command->motionProgress = 0;
        return 0;
    case 0x17E:
        btlPrepareRandomizedActionCameraPose((s32)command, (s32)command + 0x30, (s32)command + 0xC0);
        command->stepKind = 4;
        return 1;
    }
    return 0;
}

s32 func_00219BD0(BtlLinkedCommand *command, s32 unusedGroup200, s32 unusedGroup400) {
    ActionStateLink *link = command->link;

    if (btlIsActorCategoryMarked((s32)command)) {
        goto rejected;
    }
    if (link->unit->status.flags & 0x200) {
        goto rejected;
    }
    if (command->actionCode != 0x17E) {
        goto rejected;
    }
    func_00217B20(command, &command->frontCamera, &command->backCamera,
                  0, 0, 1.0f, 2.5f, 0.5f, 0.075f);
    command->frontCamera.distance += 100.0f;
    command->backCamera.distance += 150.0f;
    command->motionParameter = 30.0f;
    command->flags |= 0x841;
    btlAdjustCameraDirectionForDefaultPlane(&command->frontCamera);
    btlAdjustCameraDirectionForDefaultPlane(&command->backCamera);
    return 1;

rejected:
    return 0;
}

extern s32 btlHasMarkedEntry14();

extern void btlClearRuntimeFlag2000(void);

extern void func_001EC868(BtlLinkedCommand *, BtlCamState *, f32);

s32 btlAdvanceTimedActionState(BtlLinkedCommand *command) {
    if (command->actionCode != 0x10c) {
        return 0;
    }
    if (btlHasMarkedEntry14((s32)command, 0x10c) != 0) {
        if (command->motionProgress >= 0x12) {
            btlClearRuntimeFlag2000();
            func_001EC868(command, &command->camera, 0.0f);
        }
        command->motionProgress++;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041A378);

s32 btlRemapCommandKind(BtlUnit *unit, s32 kind) {
    u32 flags = unit->status.flags;
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
        if (unit->partyRecord.unitId == 0x136) {
            if (sub->controlEnabled != 0) {
                return 0x14;
            }
        }
        return 1;
    default:
        if (unit->partyRecord.unitId != 0x136) {
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

/* Query the special actor's stored action, or its live substate when given. */
s32 btlGetSpecialEnemyActionStatus(BtlUnit *unit) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    BtlUnit *actor;

    if (unit == NULL) {
        actor = battle->units;
        while (actor != NULL) {
            if (actor->status.flags & 1) {
                if (actor->status.flags & 0x400) {
                    if (actor->partyRecord.unitId == 0x136) {
                        break;
                    }
                }
            }
            actor = actor->nextActor;
        }
        if (actor != NULL) {
            return actor->unkEC == 0x13 ? 0x13 : -1;
        }
        return -1;
    }
    if (unit->status.flags & 0x400) {
        if (unit->partyRecord.unitId == 0x136) {
            return ((BattleSub *)battle->state)->controlEnabled != 0 ? 0x13 : 0;
        }
    }
    return -1;
}

s32 func_00219F28(s32 battler, s32 action) {
    if ((((BtlUnit *)battler)->status.flags & 0x400) == 0) {
        return 0;
    }
    if (((BtlUnit *)battler)->partyRecord.unitId != 0x136) {
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
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x10D) {
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
        PCP_COPY_VECTOR(lead->position, pos);
        btlSetUnitPosition(lead, pos);
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x400) {
                    if (unit != lead) {
                        func_001E3108(unit, pos);
                        pos[0] = pos[0] + shift;
                        PCP_COPY_VECTOR(unit->position, pos);
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
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x10D) {
                    if (unit->status.flags & 0x20) {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x400) {
                    if (unit->status.flags & 2) {
                        if (!(unit->status.flags & 0xE0)) {
                            if (unit->partyRecord.unitId != 0x10D) {
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->status.flags &= ~1;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

/* vu0 routine: normalize packed RGBA and fade the named chunk tree. */
void btlFadeAndTintNamedChunkTree(SdfDrawNode *node, s32 color) {
    SdfDrawNode *child;
    u32 source[4];
    u32 tint[4];
    u32 packed[4];

    node->flags |= SDF_DRAW_NODE_FLAG_USE_NODE_COLOR;
    node->color &= 0xFF000000;
    if (node->color > 0x3FFFFFF) {
        node->color -= 0x4000000;
        D_00438F84 = 0;
    } else {
        node->color = 0;
    }
    source[0] = color;
    EE_MMI_RGBA_UNPACK(source, 1.0f / 128.0f);
    VU0_MOVE_VF(vf11, vf10);
    tint[0] = node->color | 0x808080;
    EE_MMI_RGBA_UNPACK(tint, 1.0f / 128.0f);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed[0]);
    child = node->children;
    node->color = packed[0];
    if (child != NULL) {
        do {
            btlFadeAndTintNamedChunkTree(child, color);
            child = child->next;
        } while (child != node->children);
    }
}

s8 btlDispatchNamedChunkNode(const char *name) {
    BtlUnit *battler = ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit;
    EvtUnit *resource;
    SdfModel *chunk;
    s32 index;
    DevRequest *data;
    SdfDrawNode **entries;
    SdfDrawNode *node;
    if (battler == 0) {
        return 1;
    }
    if (!(battler->status.flags & 2)) {
        return 1;
    }
    resource = battler->ext;
    chunk = resource->owner->inner;
    index = sdfNamedChunkFindId(chunk, name);
    if (index == -1) {
        return 1;
    }
    data = chunk->list;
    entries = data->buffer;
    node = entries[index];
    D_00438F84 = 1;
    btlFadeAndTintNamedChunkTree(node, chunk->color);
    return D_00438F84;
}

void btlResetNamedChunkNodeTree(SdfDrawNode *node) {
    SdfDrawNode *child;

    node->color = 0x80808080;
    child = node->children;
    node->flags &= ~SDF_DRAW_NODE_FLAG_USE_NODE_COLOR;
    if (child != 0) {
        do {
            btlResetNamedChunkNodeTree(child);
            child = child->next;
        } while (child != node->children);
    }
}

void btlClearNamedChunkFlags(const char *name) {
    BtlUnit *battler = ((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit;
    if (battler != 0 && (battler->status.flags & 2) != 0) {
        EvtUnit *resource = battler->ext;
        SdfModel *chunk = resource->owner->inner;
        s32 index = sdfNamedChunkFindId(chunk, name);
        if (index != -1) {
            DevRequest *data = chunk->list;
            SdfDrawNode **entries = data->buffer;
            btlResetNamedChunkNodeTree(entries[index]);
        }
    }
}

extern void btlInitializeEffectVectorsFromSourceRecords(BtlUnit *, s32, s32);

/* The three formation actors retain a complete vector and their current dimensions. */
static inline void btlCopyFormationDimensions(BtlUnit *unit) {
    PCP_COPY_VECTOR(unit->muzzleOffset, unit->bodyOffset);
    unit->unkBC = unit->reach;
    unit->unkB8 = unit->height;
}

/* btlUpdateSpecialActorFormation */
void btlUpdateSpecialActorFormation(void) {
    BtlUnit *left = NULL;
    BtlUnit *right = NULL;
    BtlUnit *core = NULL;
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BtlUnit *unit;
    BattleSub *sub = work->sub;

    for (unit = work->actorList; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                switch (unit->partyRecord.unitId) {
                case 0x110:
                    core = unit;
                    break;
                case 0x111:
                    left = unit;
                    break;
                case 0x112:
                    right = unit;
                    break;
                }
            }
        }
    }
    if (sub->b8 != 0) {
        if (core != NULL) {
            btlInitializeEffectVectorsFromSourceRecords(core, 1, core->partyRecord.unitId);
            core->bodyOffset[2] += 1050.0f;
            core->reach = 100.0f;
        }
        if (sub->alternateFormation != 0) {
            if (left != NULL) {
                left->bodyOffset[0] = -385.0f;
                left->bodyOffset[1] = -390.0f;
                left->bodyOffset[2] = 2525.0f;
                left->reach = 320.0f;
                left->height = 600.0f;
                left->bodyOffset[3] = 0.0f;
            }
            if (right != NULL) {
                right->bodyOffset[0] = 320.0f;
                right->bodyOffset[1] = -390.0f;
                right->bodyOffset[2] = 2165.0f;
                right->reach = 300.0f;
                right->height = 520.0f;
                right->bodyOffset[3] = 0.0f;
            }
        } else if (left == sub->targetActor) {
            if (left != NULL) {
                left->bodyOffset[0] = -105.0f;
                left->bodyOffset[1] = -300.0f;
                left->bodyOffset[2] = 2525.0f;
                left->reach = 300.0f;
                left->height = 600.0f;
                left->bodyOffset[3] = 0.0f;
            }
            if (right != NULL) {
                btlInitializeEffectVectorsFromSourceRecords(right, 1, right->partyRecord.unitId);
            }
        } else {
            if (left != NULL) {
                btlInitializeEffectVectorsFromSourceRecords(left, 1, left->partyRecord.unitId);
            }
            if (right != NULL) {
                right->bodyOffset[0] = 10.0f;
                right->bodyOffset[1] = -390.0f;
                right->bodyOffset[2] = 2165.0f;
                right->reach = 300.0f;
                right->height = 530.0f;
                right->bodyOffset[3] = 0.0f;
            }
        }
    } else {
        if (core != NULL) {
            btlInitializeEffectVectorsFromSourceRecords(core, 1, core->partyRecord.unitId);
        }
        if (left != NULL) {
            btlInitializeEffectVectorsFromSourceRecords(left, 1, left->partyRecord.unitId);
        }
        if (right != NULL) {
            btlInitializeEffectVectorsFromSourceRecords(right, 1, right->partyRecord.unitId);
        }
    }
    if (core != NULL) {
        btlCopyFormationDimensions(core);
    }
    if (left != NULL) {
        btlCopyFormationDimensions(left);
    }
    if (right != NULL) {
        btlCopyFormationDimensions(right);
    }
}



extern char D_00436CE0[];

extern char D_00436CE8[];

void func_0021A778(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = battle->units;

    while (unit != NULL) {
        s32 flags = unit->status.flags;

        if (flags & 2) {
            if (flags & 0x400) {
                s32 unitId = unit->partyRecord.unitId;

                if (unitId < 0x113) {
                    if (unitId >= 0x111) {
                        s32 nodeIndex = unitId == 0x111 ? 1 : 2;

                        if (mdlGetNodeMotionIndex(unit->ext->owner, nodeIndex) == 0x11) {
                            if (unit->ext->slotC[nodeIndex] < mdlGetNodeFrameAsInt(unit->ext->owner, nodeIndex)) {
                                btlClearNamedChunkFlags(unit->partyRecord.unitId == 0x111 ? D_00436CE0 : D_00436CE8);
                                unit->status.stateFlags &= ~0x80000;
                            }
                        } else if (unit->status.flags & 0xE0) {
                            if (btlDispatchNamedChunkNode(unit->partyRecord.unitId == 0x111 ? D_00436CE0 : D_00436CE8)) {
                                unit->status.stateFlags |= 0x80000;
                            } else {
                                unit->status.stateFlags &= ~0x80000;
                            }
                        }
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
}

void btlReturnUnitToGroup(ActionStateLink *task) {
    BtlUnit *unit;

    if (task->pendingFlags & 8) {
        unit = task->unit;
        if (unit->partyRecord.status & 0x4000) {
            return;
        }
        unit->status.flags &= ~0x20;
        unit->status.flags &= ~0x08000000;
        unit->status.flags |= 1;
        btlRefreshUnitMotionSelection(unit);
        fldAppendTaskToGroup(task);
        btlDispatchStateHandler(task, 2);
    }
}


INCLUDE_ASM(const s32, "game/code_00214948", func_0021A978);

void btlSetUnitResourceFloatByMode(BtlUnit *unit, s32 group, f32 value) {
    if (((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit != unit) {
        if (!(unit->status.flags & 0x400)) {
            evtUnitSetStoredParameter(unit->ext, group);
            evtSetTransitionMotionScale(unit->ext, value);
            return;
        }
        switch (unit->partyRecord.unitId) {
        case 0x110:
            evtUnitSetStoredParameter(unit->ext, group);
            evtSetTransitionMotionScale(unit->ext, value);
            break;
        case 0x111:
            unit->ext->tableValues[1] = group;
            unit->ext->unk138[1] = value;
            break;
        case 0x112:
            unit->ext->tableValues[2] = group;
            unit->ext->unk138[2] = value;
            break;
        }
    }
}

void btlSetUnitResourceHalvesByMode(BtlUnit *unit, s32 first, s32 second) {
    if (((BtlSelectCtrl *)((BattleWork *)btlGetRuntime())->sub)->unit != unit) {
        if (!(unit->status.flags & 0x400)) {
            evtStoreUnitMotionShortParameters(unit->ext, first, second);
            return;
        }
        switch (unit->partyRecord.unitId) {
        case 0x110:
            evtStoreUnitMotionShortParameters(unit->ext, first, second);
            break;
        case 0x111:
            unit->ext->unk108[1] = first;
            unit->ext->unk120[1] = second;
            break;
        case 0x112:
            unit->ext->unk108[2] = first;
            unit->ext->unk120[2] = second;
            break;
        }
    }
}

s32 btlResolveBoundActionCode(s32 battler, s32 action) {
    s32 slot = (s32)((BattleWork *)btlGetRuntime())->sub;
    if (((BtlSelectCtrl *)slot)->unit == (BtlUnit *)battler) {
        return -1;
    }
    if ((((BtlUnit *)battler)->status.flags & 0x400) == 0) {
        return action;
    }
    if (action == 15 && ((BtlUnit *)battler)->partyRecord.unitId == 0x112) {
        return 20;
    }
    if (action == 1 || action == 11) {
        if (((BtlSelectCtrl *)slot)->pending != 0) {
            s32 selected = ((BtlUnit *)battler)->partyRecord.unitId;
            if (selected < 0x113) {
                if (selected >= 0x111) {
                    return (((BtlUnit *)battler)->status.stateFlags & 0x80000) ? 10 : 21;
                }
            }
        }
    }
    return action;
}

s32 btlMotionOffsetForActor(s32 actor, s32 base) {
    u32 flags = ((BtlUnit *)actor)->status.flags;
    if ((flags & 1) == 0) {
        return base;
    }
    if ((flags & 0x400) == 0) {
        return base;
    }
    switch (((BtlUnit *)actor)->partyRecord.unitId) {
    case 0x110:
        return base;
    case 0x111:
        return base + 100;
    case 0x112:
        return base + 200;
    }
    return base;
}

extern void btlSetUnitRotation(BtlUnit *, s128 *);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

extern void btlUpdateSpecialActorFormation(void);

extern f32 D_003BF6B0[4];

void func_0021B368(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = battle->units;
    f32 position[4] __attribute__((aligned(16)));
    BtlActorStatusRecord *table;
    s32 mode;

    while (unit != 0) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                mode = unit->partyRecord.unitId;
                if (mode < 0x113) {
                    if (mode >= 0x110) {
                        btlSetUnitRotation(unit, D_003BF6B0);
                        unit->status.flags &= ~0x80000;
                        /* Retail resets xyz; the copied fourth lane is unspecified. */
                        VEC3_SPLAT(position, 0.0f);
                        PCP_COPY_VECTOR(unit->position, position);
                        btlSetUnitPosition(unit, position);
                    }
                }
                mode = unit->partyRecord.unitId;
                if (mode < 0x113) {
                    if (mode >= 0x111) {
                        switch (unit->unkEC) {
                        case 0x10:
                            unit->unkF8 = 0;
                            unit->unkFA = 0;
                            btlApplyUnitMotionSelection(unit, unit->effectIndex,
                                                       unit->effectParameter,
                                                       unit->effectScale);
                            break;
                        case 0xB:
                            table = (BtlActorStatusRecord *)
                                btlGetSideIndexedActorStatusTable(unit->resourceKind,
                                                                 unit->resourceIndex);
                            btlApplyUnitMotionSelection(unit, 0xA, 1, table->motions[10].alphaFrameScale);
                            break;
                        }
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    btlUpdateSpecialActorFormation();
}

void func_0021B4A8(void) {
    func_0021B368();
}

BtlUnit *btlGetReadyUnitForSpecies(s32 mode, u32 species) {
    BtlUnit *unit;

    if (mode != 1) {
        return NULL;
    }
    switch (species) {
    case 0x110:
    case 0x111:
    case 0x112:
        break;
    default:
        return NULL;
    }
    unit = *(BtlUnit **)((BattleWork *)btlGetRuntime())->sub;
    if (unit == NULL) {
        return NULL;
    }
    return (unit->status.flags & 2) ? unit : NULL;
}

extern u64 btlAdvanceRuntimeSequenceCounter(void);

extern void func_001AA898(DatPartyRecord *, s32);

/* Create/load the unit only for an empty slot. A populated slot returns a fresh
 * sequence ID without launching a task; a nonzero prerequisite waits until
 * that task handle is gone.
 */
s64 btlEnsureHeroUnitTask(u64 prerequisiteHandle) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit **slot = &battle->effect->guard.unit;
    BtlRuntimeTask *task;
    if (*slot != 0) {
        return btlAdvanceRuntimeSequenceCounter();
    }
    *slot = btlCreateUnit();
    func_001AA898(&(*slot)->partyRecord, 0x110);
    task = btlCreateModelLoadPollTask(*slot, 1, 0x110, 0);
    if (prerequisiteHandle != 0) {
        task->startCondition.value.handle = prerequisiteHandle;
        task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    }
    btlStartTask(task);
    return task->handle;
}

/* The boss classification is checked by entry lookup and HEKATO scaling. */

#define BTL_UNIT_BOSS_FLAG 0x400

/* The debug text in btlAccumulateBossRatioScale names skill 0x1A9. */

#define BTL_SKILL_HEKATO 0x1A9

typedef struct BtlUnit BtlUnit;

extern BtlActionAnimationRecord *datActionAnimationRecords;

extern s32 btlGetSlotValueAdjustedForSpecialAbility(BtlUnit *, s32);

extern s32 btlAdjustPointsForCombatFlags(BtlUnit *, s32, s32, s32, s32);

extern s8 btlGetCommandResultKindFromFlags(s32, s32, s32);



extern void func_003364B8(f32);

extern void func_00336818(f32);

extern void sdfComposeVuMatrixFromRegisters(void);

extern s32 btlSetLinkedDefeatCameraPresetA();

extern void btlDestroyUnit(BtlUnit *);

void btlCancelCurrentSubtask(void) {
    BattleGuardState *state;
    BtlUnit *unit;

    state = &((BtlState *)btlGetRuntime())->effect->guard;
    unit = state->unit;
    if (unit != 0) {
        btlDestroyUnit(unit);
        state->unit = 0;
    }
}

/* Launch a subtask from the active slot and return its new scheduler handle.
 * Zero leaves the constructor's condition intact; otherwise wait for the
 * prerequisite task to disappear. The fixed high-bit owner value is preserved.
 */
u64 btlStartSubtaskWithInput(u64 prerequisiteHandle) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlRuntimeTask *task = func_001E5FF8(battle->effect->guard.unit, 0xC);
    if (prerequisiteHandle != 0) {
        task->startCondition.value.handle = prerequisiteHandle;
        task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    }
    task->ownerId = 0x8000000000000003;
    btlStartTask(task);
    return task->handle;
}

void btlMarkActiveBossUnitExtensionFlags(BtlUnit *unit) {
    if (unit->status.flags & BTL_UNIT_BOSS_FLAG) {
        if (unit->status.flags & 2) {
            unit->ext->flags |= 0x1000000;
            unit->status.stateFlags |= 0x20000;
            unit->status.stateFlags |= 0x400000;
        }
    }
}

f32 btlGetBossPresenceActionScale(BtlUnit *unit, BtlUnit *target) {
    BtlUnit *other;
    f32 scale = 1.0f;
    if (unit->status.flags & 0x200) {
        if (target->partyRecord.unitId == 0x110) {
            for (other = ((BattleWork *)btlGetRuntime())->actorList; other != 0; other = other->nextActor) {
                if (other->status.flags & 1) {
                    if (other->status.flags & BTL_UNIT_BOSS_FLAG) {
                        if (!(other->status.flags & 0xE0)) {
                            if (other->partyRecord.unitId >= 0x111 && other->partyRecord.unitId < 0x113) {
                                break;
                            }
                        }
                    }
                }
            }
            scale = other != 0 ? 1000.0f : 1.0f;
        }
    }
    return scale;
}

/* Store the flag-adjusted value and signed result kind in the owning command actor. */
void btlSetSkillTaskResults(ActionStateLink *task, s32 flags, s32 otherFlags, s32 skillId) {
    s32 percent = 100;
    if (skillId >= 0x1AB && skillId < 0x220) {
        percent *= btlGetSlotValueAdjustedForSpecialAbility(task->unit, skillId);
    }
    task->indexWork.adjustedValue = btlAdjustPointsForCombatFlags(task->unit, flags, otherFlags, percent, skillId);
    task->indexWork.resultKind = btlGetCommandResultKindFromFlags(flags, otherFlags, skillId);
}

INCLUDE_ASM(const s32, "game/code_00214948", func_0021B828);

extern BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *, s32, s32, f32);
extern BtlRuntimeTask *btlCreateUnitFaceBodyTask(BtlUnit *, BtlUnit *);

void func_0021C0C8(ActionStateLink *handle, s32 unused, u64 completionOwner, u64 prerequisite) {
    BtlState *battle;
    BattleActionState *state;
    BtlUnit *unit;
    BtlUnit *hero;
    BtlOperandGroup *group;
    BtlRuntimeTask *task;
    u32 count;
    u32 i;
    s32 hasInactive;
    s32 isFormation;
    s32 delay;
    s32 startDelay;

    if (!(handle->pendingFlags & 8)) {
        return;
    }
    hero = NULL;
    battle = (BtlState *)btlGetRuntime();
    state = &battle->effect->action;
    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                s32 unitId = unit->partyRecord.unitId;

                if (unitId != 0x111) {
                    if (unitId < 0x112) {
                        if (unitId == 0x110) {
                            hero = unit;
                        }
                    }
                }
            }
        }
    }
    if (handle->unit->status.flags & 0x200) {
        if (state->pending != 0 && hero != NULL) {
            isFormation = handle->indexWork.slot == 0x19;
            btlUpdateSpecialActorFormation();
            delay = func_001E2E58(handle->unit, handle->indexWork.slot);
            task = btlAllocateIndexedUnitEffectTask(hero, 0x10, 2, 1.0f);
            task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
            task->startCondition.value.handle = prerequisite;
            if (delay < 7 || isFormation) {
                startDelay = 0;
            } else {
                startDelay = delay - 6;
            }
            task->startDelay = startDelay;
            task->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(task);
            if (handle->indexWork.parameter == 0) {
                handle->indexWork.stage = 1;
                handle->indexWork.parameter = 0xD7;
            }
        }
        i = 0;
        group = handle->indexWork.groups;
        count = btlGetIndexListCount(handle->indexWork.indices);
        hasInactive = 0;
        if (count != 0) {
            do {
                s32 unitId = ((BtlUnit *)btlGetIndexListEntry(handle->indexWork.indices, i++))->partyRecord.unitId;

                if (unitId < 0x113) {
                    if (unitId >= 0x111) {
                        if (group->inactive != 0) {
                            hasInactive = 1;
                        }
                    }
                }
                group++;
            } while (i < count);
        }
        if (hasInactive != 0) {
            task = btlCreateImmediateCompletionTask();
            task->startCondition.kind = BTL_TASK_CONDITION_OWNER_ABSENT;
            task->startCondition.value.handle = completionOwner;
            task->startDelay = 0x23;
            task->ownerId = handle->unit->owner;
            btlStartTask(task);
        }
    } else if ((handle->unit->status.flags & 0x400) && hero != NULL && handle->unit != NULL) {
        s32 unitId = handle->unit->partyRecord.unitId;

        if (unitId < 0x113) {
            if (unitId >= 0x111) {
                if (handle->indexWork.slot == 0xE && btlGetIndexListCount(handle->indexWork.indices) == 1) {
                    unit = btlGetIndexListEntry(handle->indexWork.indices, 0);
                    if (unit->status.flags & 0x200) {
                        task = btlCreateUnitFaceBodyTask(hero, unit);
                        task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
                        task->startCondition.value.handle = prerequisite;
                        task->flags |= 2;
                        btlStartTask(task);
                    }
                }
            }
        }
    }
}

void btlPrepareDefeatEffectCamera(u8 *obj) {
    btlFlagAllUnitDefeatCandidatesTask(obj);
    btlSetEffectCameraKeys((s32)obj, -176.6f, -137.8f, -1564.9f, -0.01f, -0.038f, -0.012f, 0.99f, -217.8f,
                  -253.6f, -2272.8f, -0.009f, -0.038f, -0.013f, 0.99f, 40.0f, 15.0f);
}

void btlSetCameraPresetForBossUnitMode(BtlLinkedCommand *fx) {
    switch (fx->link->unit->partyRecord.unitId) {
    case 0x111:
        btlSetEffectCameraKeys((s32)fx, 533.4f, -167.8f, -1104.7f, -0.052f, 0.285f, -0.028f, 0.947f, 430.5f,
                      -91.7f, -1373.1f, -0.089f, 0.198f, -0.03f, 0.967f, 40.0f, 15.0f);
        break;
    case 0x112:
        btlSetEffectCameraKeys((s32)fx, -440.9f, -333.4f, -1307.2f, 0.026f, -0.223f, -0.017f, 0.965f, -844.6f,
                      -358.7f, -1194.1f, 0.024f, -0.346f, -0.02f, 0.928f, 40.0f, 10.0f);
        break;
    }
}

s32 btlInitializeEffectVectors(BtlLinkedCommand *command) {
    BtlCamState *pose = &command->frontCamera;
    command->camera.direction[0] = 1.0f;
    func_001EC868(command, pose, 25.0f);
    btlCopyMotionTransform(&command->backCamera, pose);
    func_00336538(-0.87266463f);
        VU0_STORE_VF(vf10, pose->direction);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, command->backCamera.direction);
    command->motionParameter = 125.0f;
    command->flags |= 0x841;
    return 1;
}

s32 btlSetLinkedDefeatCameraPresetA(command, camera, rotate)
BtlLinkedCommand *command;
BtlCamState *camera;
s32 rotate;
{
    u16 *runtime = (u16 *)btlGetRuntime();
    BtlUnit *unit;
    u32 kind;
    f32 angle;

    if (command->link == NULL) {
        return 1;
    }
    unit = btlGetTargetUnitForLink(command);
    if (unit == NULL) {
        return 1;
    }
    if (unit->status.flags & 0x400) {
        return 1;
    }
    if (runtime[0x134] == 3) {
        kind = unit->lookupId;
    } else {
        kind = unit->lookupId == 0 ? 0 : 2;
    }
    btlFlagAllUnitDefeatCandidatesTask();
    switch (kind) {
    case 0:
        btlInitMotionTransformFromComponents(camera, -1401.0f, -931.0f, -2245.4f,
            0.103f, -0.19f, -0.032f, 0.967f, 40.0f);
        break;
    case 1:
        btlInitMotionTransformFromComponents(camera, 131.1f, -940.3f, -2451.9f,
            0.107f, 0.012f, -0.01f, 0.985f, 40.0f);
        break;
    case 2:
        btlInitMotionTransformFromComponents(camera, 971.8f, -875.7f, -2539.8f,
            0.099f, 0.118f, 0.0f, 0.979f, 40.0f);
        break;
    }
    if (rotate == 1 && runtime[0x134] == 3) {
        switch (kind) {
        case 0:
            func_003364B8(-0.13089969f);
            func_00336818(0.13089969f);
            sdfComposeVuMatrixFromRegisters();
            break;
        case 1:
            func_003364B8(-0.13089969f);
            break;
        case 2:
            angle = -0.13089969f;
            func_003364B8(angle);
            func_00336818(angle);
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

INCLUDE_ASM(const s32, "game/code_00214948", func_0021C7F8);

extern void func_00336798(f32);

extern void sdfMultiplyVuMatrixInPlace(void);

extern void btlClearAllUnitDefeatCandidatesTask(void);

extern void btlFlagLinkedGroupDefeatCandidatesTask(s32);

/* VURI boss action camera: when this actor is the defeat target, pick a randomized camera key pair (or a
 * randomized action pose) by boss form (unit mode 0x110/0x111/0x112) and action code; otherwise frame the
 * actor from a fixed pose turned 50 degrees toward its facing side. */
s32 func_0021C818(BtlLinkedCommand *command, s8 side, s8 targetSide) {
    ActionStateLink *link = command->link;

    if (btlIsActorCategoryMarked((s32)command) || (link->unit->status.flags & 0x200)) {
        return 0;
    }
    if (side != 1 || targetSide == side) {
        func_001EC868(command, &command->frontCamera, 25.0f);
        btlCopyMotionTransform(&command->backCamera, &command->frontCamera);
        if (command->frontCamera.direction[0] > 0.0f) {
            func_00336538(-0.87266463f);
        } else {
            func_00336538(0.87266463f);
        }
        func_00336798(-0.34906585f);
        sdfMultiplyVuMatrixInPlace();
        VU0_STORE_VF(vf10, command->frontCamera.direction);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, command->backCamera.direction);
        command->motionParameter = 45.0f;
        command->flags |= 0x841;
        return 1;
    }
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
    switch (link->unit->partyRecord.unitId) {
    case 0x110:
        switch (command->actionCode) {
        case 0x198:
            btlBossDebugPrintf("VURI:I-G ++++\n");
            btlPrepareRandomizedActionCameraPose((s32)command, (s32)&command->frontCamera, (s32)&command->backCamera);
            btlFlagAllUnitDefeatCandidatesTask();
            command->stepKind = 4;
            return 1;
        case 0x197:
            btlFlagAllUnitDefeatCandidatesTask();
            switch (effMiscRandMod(0, 2)) {
            case 0:
                btlBossDebugPrintf("VURI:I-Z_G-0 ++++\n");
                btlSetEffectCameraKeys((s32)command,
                    45.6f, -377.9f, -2539.6f, 0.041f, 0.009f, -0.012f, 0.99f, -7.0f,
                    -508.3f, -1566.6f, 0.087f, 0.008f, -0.011f, 0.987f, 40.0f, 10.0f);
                return 1;
            case 1:
                btlBossDebugPrintf("VURI:I-Z_G-1 ++++\n");
                btlSetEffectCameraKeys((s32)command,
                    8.1f, -1095.9f, -2087.1f, 0.173f, 0.007f, -0.01f, 0.976f, 25.3f,
                    -912.4f, -1436.5f, 0.175f, 0.007f, -0.01f, 0.975f, 40.0f, 15.0f);
                return 1;
            }
            /* retail keeps the out-of-range roll falling into the generic picks (b to the mod-3 roll) */
        default:
            switch (effMiscRandMod(0, 3)) {
            case 0:
                btlBossDebugPrintf("VURI:I-0 ++++\n");
                btlSetEffectCameraKeys((s32)command,
                    772.4f, -794.7f, -1209.8f, 0.082f, 0.152f, 0.006f, 0.975f, -1562.8f,
                    -2553.1f, -1132.5f, 0.257f, -0.26f, -0.08f, 0.917f, 40.0f, 30.0f);
                return 1;
            case 1:
                btlBossDebugPrintf("VURI:I-1 ++++\n");
                btlSetEffectCameraKeys((s32)command,
                    -61.2f, -75.7f, -877.3f, -0.179f, -0.018f, -0.003f, 0.974f, 938.7f,
                    -281.4f, -649.2f, -0.175f, 0.186f, -0.04f, 0.956f, 40.0f, 25.0f);
                return 1;
            case 2:
                btlBossDebugPrintf("VURI:I-2 ++++\n");
                btlPrepareRandomizedActionCameraPose((s32)command, (s32)&command->frontCamera, (s32)&command->backCamera);
                btlFlagAllUnitDefeatCandidatesTask();
                command->stepKind = 4;
                return 1;
            }
            break;
        }
        break;
    case 0x111:
        switch (command->actionCode) {
        case 0x10D:
        case 0x10E:
            btlBossDebugPrintf("VURI_A:N_A ++++\n");
            btlPrepareRandomizedActionCameraPose((s32)command, (s32)&command->frontCamera, (s32)&command->backCamera);
            btlFlagAllUnitDefeatCandidatesTask();
            command->stepKind = 4;
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        switch (effMiscRandMod(0, 3)) {
        case 0:
            btlBossDebugPrintf("VURI_A:I-0 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -20.1f, -1268.7f, -1612.9f, 0.16f, 0.152f, 0.014f, 0.966f, 1220.2f,
                -1216.5f, -1519.4f, 0.162f, 0.242f, 0.03f, 0.947f, 40.0f, 25.0f);
            return 1;
        case 1:
            btlBossDebugPrintf("VURI_A:I-1 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -2784.2f, -1846.4f, -636.1f, -0.175f, 0.452f, 0.103f, -0.857f, -326.1f,
                -1724.4f, -1809.0f, -0.25f, 0.019f, 0.014f, -0.958f, 40.0f, 30.0f);
            return 1;
        case 2:
            btlBossDebugPrintf("VURI_A:I-2 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -2936.0f, -1966.4f, 834.8f, -0.206f, 0.685f, 0.229f, -0.646f, -2221.2f,
                -1778.1f, 395.6f, -0.206f, 0.685f, 0.229f, -0.646f, 40.0f, 25.0f);
            return 1;
        }
        break;
    case 0x112:
        switch (command->actionCode) {
        case 0x10D:
        case 0x10E:
            btlBossDebugPrintf("VURI_A:N_B ++++\n");
            btlPrepareRandomizedActionCameraPose((s32)command, (s32)&command->frontCamera, (s32)&command->backCamera);
            btlFlagAllUnitDefeatCandidatesTask();
            command->stepKind = 4;
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        switch (effMiscRandMod(0, 3)) {
        case 0:
            btlBossDebugPrintf("VURI_B:I-0 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -1653.6f, -1414.0f, -776.2f, -0.164f, 0.419f, 0.087f, -0.878f, -347.6f,
                -1306.9f, -1321.1f, -0.182f, 0.17f, 0.04f, -0.958f, 40.0f, 30.0f);
            return 1;
        case 1:
            btlBossDebugPrintf("VURI_B:I-1 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                1988.7f, -555.7f, -1275.0f, -0.077f, -0.28f, -0.015f, -0.946f, 644.4f,
                -1129.4f, -1643.7f, -0.158f, -0.1f, -0.008f, -0.972f, 40.0f, 20.0f);
            return 1;
        case 2:
            btlBossDebugPrintf("VURI_B:I-2 ++++\n");
            btlSetEffectCameraKeys((s32)command,
                3048.7f, -568.1f, 238.9f, -0.007f, -0.685f, 0.0f, -0.715f, 1432.3f,
                -1044.6f, -1558.1f, -0.133f, -0.219f, -0.022f, -0.956f, 40.0f, 25.0f);
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041A5E0);

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041A5F8);

INCLUDE_ASM(const s32, "game/code_00214948", func_0021CF18);

INCLUDE_ASM(const s32, "game/code_00214948", func_0021E778);

INCLUDE_ASM(const s32, "game/code_00214948", func_0021E8C0);

s32 btlMapBossEntryKindToIndex(BtlUnit *unit, s32 index) {
    if (!(unit->status.flags & BTL_UNIT_BOSS_FLAG)) {
        return -1;
    }
    if (datActionAnimationRecords[index].kind == 0) {
        return -1;
    }
    if (datActionAnimationRecords[index].kind >= 11 && datActionAnimationRecords[index].kind < 26) {
        return -1;
    }
    switch (datActionAnimationRecords[index].kind) {
    case 1: return 0xC;
    case 2: return 0xD;
    case 3: return 0xE;
    case 4: return 0xF;
    case 5: return 0x10;
    case 6: return 0x11;
    case 7: return 0x12;
    case 8: return 0x13;
    case 9: return 0x14;
    case 10: return 0x15;
    default: return 0xD;
    }
}

s32 btlGetBossEntryKind(BtlUnit *unit, s32 index) {
    if (!(unit->status.flags & BTL_UNIT_BOSS_FLAG)) {
        return -1;
    }
    if (datActionAnimationRecords[index].kind == 0) {
        return -1;
    }
    return datActionAnimationRecords[index].kind;
}

s32 btlRemapBossResponseForActionPhase(BtlUnit *unit, s32 value) {
    s32 mode;
    if (!(unit->status.flags & BTL_UNIT_BOSS_FLAG)) {
        return value;
    }
    mode = unit->partyRecord.unitId;
    if (mode < 0x113) {
        if (mode >= 0x111) {
            if (value == 4) {
                return 5;
            }
            if (value == 0xB) {
                return 0;
            }
        }
    }
    return value;
}

s32 btlPlayStationedSoundForActiveBossAction(BtlUnit *unit) {
    s32 mode;
    if (unit->status.flags & BTL_UNIT_BOSS_FLAG) {
        if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0)) {
            mode = unit->partyRecord.unitId;
            if (mode < 0x113) {
                if (mode >= 0x111) {
                    btlStartTask(sndCreateStationedSeTask(((BattleWork *)btlGetRuntime())->soundTaskBase + 1));
                }
            }
        }
    }
}

s32 btlGetBossSceneStateWhenActive(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    if (work->mode != 0x30B) {
        return 0;
    }
    if (work->sub == 0) {
        return 0;
    }
    return work->sub->b8;
}

void btlResetBossRatioScale(void) {
    ((BattleWork *)btlGetRuntime())->sub->scale = 1.0f;
}

/* Hekato multiplies the active ratio and clamps it to its own parameter maximum. */
s32 btlAccumulateBossRatioScale(ActionStateLink *task) {
    f32 *ratio;
    SdfBattleParameters *params;
    if (task->pendingFlags & 8) {
        if (task->unit->status.flags & BTL_UNIT_BOSS_FLAG) {
            ratio = &((BattleWork *)btlGetRuntime())->sub->scale;
            if (task->indexWork.skillId == BTL_SKILL_HEKATO) {
                params = datBattleParameters;
                *ratio *= params->hekatoRatioScale;
                if (*ratio > params->hekatoRatioMax) {
                    *ratio = params->hekatoRatioMax;
                }
                btlBossDebugPrintf("btl:boss HEKATO ratio = %f\n", *ratio);
            }
        }
    }
}

f32 btlGetBossRatioScale(BtlUnit *unit, s32 unused, s32 kind, s32 flag) {
    f32 scale = 1.0f;
    if (kind == BTL_SKILL_HEKATO && flag == 1) {
        if (unit->status.flags & BTL_UNIT_BOSS_FLAG) {
            scale = ((BattleWork *)btlGetRuntime())->sub->scale;
        }
    }
    return scale;
}

BtlUnit *btlFindUnitByMode(void) {
    BattleWork *work = (BattleWork *)btlGetRuntime();
    BattleSub *sub = work->sub;
    BtlUnit *unit;
    if (sub->b.active == 0) {
        return 0;
    }
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                if (sub->targetMode == unit->partyRecord.unitId) {
                    return unit;
                }
            }
        }
    }
    return 0;
}

u64 btlMaskValueWhenSubtaskInactive(u64 value) {
    BattleWork *work;
    u64 result;

    work = btlGetRuntime();
    result = 0;
    if (work->sub->b.active != '\0') {
        result = value;
    }
    return result;
}

extern void btlSetRuntimeFlag2000(void);

extern u32 func_00220958(void);

extern u8 D_003BF6C0[][5];

extern u8 D_003BF950[];

extern void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *);

extern u32 btlIsMarkedActionSceneStateActive(void);

extern void func_001ECBF8(BtlLinkedCommand *, BtlCamState *);

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlUnitSetCameraOffset(BtlLinkedCommand *);

extern void btlRaiseActionCameraPoints(BtlLinkedCommand *);

extern void func_00224EE8(BtlLinkedCommand *);

extern void btlChooseBrahmaGroupCamera(u32);

extern void func_002240C0(u32);

extern void func_00224F88(u32);

extern void func_001ADFE0(u32, u32, u32);

extern void btlSetUnitRotation(BtlUnit *, s128 *);

extern void func_002218C8(void);

extern s32 func_00222450();

extern s32 btlSetLinkedDefeatCameraPresetB();

typedef struct BattleActionFlagState {
    u8 active; /* 0x00 */
    u8 pad01;
    u16 phase; /* 0x02 */
} BattleActionFlagState;

/* Signed byte view used when comparing action transitions and actor activity. */
typedef struct BattleActionByteState {
    s8 current;       /* 0x00 */
    s8 previous;      /* 0x01 */
    u8 pad02[6];
    s8 markedActive;  /* 0x08 */
} BattleActionByteState;

/* Handle returned by btlFindUnitByActor; these fields drive its action task. */
extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

/* When the action-state byte changes, restore the marked unit's saved motion. */
void btlRestoreMarkedUnitMotionOnStateChange(void) {
    BattleActionScene *scene = (BattleActionScene *)btlGetRuntime();
    u8 *state = scene->state;
    BtlUnit *unit;

    if (((BattleActionByteState *)state)->previous != ((BattleActionByteState *)state)->current) {
        state[1] = state[0];
        unit = scene->units;
        if (unit != 0) {
            while (unit != 0) {
                if (unit->status.flags & 1) {
                    if (unit->status.flags & 0x400) {
                        if (unit->partyRecord.unitId == 0x115) {
                            break;
                        }
                    }
                }
                unit = unit->nextActor;
            }
            if (unit != 0) {
                btlRefreshUnitMotionSelection(unit);
                unit->unkF8 = 0;
                unit->unkFA = 0;
                btlApplyUnitMotionSelection(unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
            }
        }
    }
}

void btlToggleActionByteForFlaggedActor(u32 unused, u32 actor) {
    u8 *state = ((BattleActionScene *)btlGetRuntime())->state;
    if (*(u32 *)(actor + 0x28) & 0x8000) {
        if (((BattleActionByteState *)state)->previous != 0) {
            *state = 0;
        } else {
            *state = 1;
        }
    }
}

u32 btlGetMarkedUnitActionResponse(BtlUnit *unit, BtlUnit *actor, u32 action) {
    u8 *state = ((BattleActionScene *)btlGetRuntime())->state;
    state[2] = action == 0x196;
    if ((unit->status.flags & 0x200) &&
        (actor->status.flags & 0x400) &&
        actor->partyRecord.unitId == 0x115) {
        return btlIsMarkedActionSceneStateActive() ? 4 : 0;
    }
    return 0;
}

s32 btlGetMarkedUnitActionStatus(BtlUnit *unit) {
    if (unit == 0) {
        return btlIsMarkedActionSceneStateActive() ? 0xf : -1;
    }
    if ((unit->status.flags & 0x400) &&
        unit->partyRecord.unitId == 0x115 &&
        btlIsMarkedActionSceneStateActive()) {
        return 0xf;
    }
    return -1;
}

extern char D_0041AAC8[]; /* format string */

extern char D_00436CF0[];

extern s32 func_0035C860(char *, const char *, ...);

void btlFormatSpecialMotionDisplayCode(s32 unit, u32 action, char *buffer) {
    u32 value;

    if (action == 0x196) {
        switch (*(s32 *)(unit + 0x38)) {
        case 0x12A:
            value = 1;
            break;
        case 0x12B:
            value = 2;
            break;
        case 0x12C:
            value = 5;
            break;
        case 0x12D:
            value = 6;
            break;
        default:
            return;
        }
        func_0035C860(buffer, D_0041AAC8, D_00436CF0, datActionAnimationRecords[action].displayCode, value);
    }
}

void btlCenterMarkedFormationAroundLead(void) {
    BattleActionScene *scene = (BattleActionScene *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *lead = 0;
    f32 shift;
    f32 pos[4];

    for (unit = scene->units; unit != 0; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x115) {
                    lead = unit;
                    break;
                }
            }
        }
    }
    if (lead != 0) {
        func_001E3108(lead, pos);
        pos[2] = 200.0f;
        shift = -pos[0];
        pos[0] = 0;
        PCP_COPY_VECTOR(lead->position, pos);
        btlSetUnitPosition(lead, pos);
        for (unit = scene->units; unit != 0; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x400) {
                    if (unit != lead) {
                        func_001E3108(unit, pos);
                        pos[0] = pos[0] + shift;
                        PCP_COPY_VECTOR(unit->position, pos);
                        btlSetUnitPosition(unit, pos);
                    }
                }
            }
        }
    }
}

s32 btlStartOtherMarkedUnitTasks(void) {
    BtlUnit *unit = ((BattleActionScene *)btlGetRuntime())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x115) {
                    if (unit->status.flags & 0x20) {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x400) {
                    if (unit->status.flags & 2) {
                        if (!(unit->status.flags & 0xE0)) {
                            if (unit->partyRecord.unitId != 0x115) {
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->status.flags &= ~1;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

s32 btlMapActorMotionId(u32 id) {
    switch (id) {
    case 0x12A:
    case 0x12B:
    case 0x12C:
    case 0x12D:
        return 0x11B;
    default:
        return 0;
    }
}

u8 func_0021F3A0(s32 arg0) {
    return arg0 != 0x196;
}

s32 btlIsSpecialMotion(BtlUnit *actor) {
    if ((actor->status.flags & 0x400) == 0) {
        return 0;
    }
    switch (actor->partyRecord.unitId) {
    case 0x12A:
    case 0x12B:
    case 0x12C:
    case 0x12D:
        return 1;
    default:
        return 0;
    }
}

/* Pick a permitted shadow skill from the corresponding party member's list. */
INCLUDE_RODATA(const s32, "game/code_00214948", D_0041AAC8);

void func_0021F3E8(ActionStateLink *actor) {
    s32 choices[24];
    s32 partyId;
    DatPartyRecord *entry;
    s32 i;
    s32 count;

    actor->indexWork.phase = 1;
    actor->indexWork.skillId = 0;
    if (!(actor->pendingFlags & 8)) return;
    switch (actor->unit->partyRecord.unitId) {
    case 0x12A: partyId = 1; break;
    case 0x12B: partyId = 2; break;
    case 0x12C: partyId = 5; break;
    case 0x12D: partyId = 6; break;
    default: return;
    }
    entry = NULL;
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].unitId == partyId) {
            entry = &datGameState->party[i];
            break;
        }
    }
    if (entry == NULL) return;
    count = 0;
    for (i = 0; i < 24; i++) {
        s32 skillId = entry->effectData[i];
        s32 category;
        if ((u32)(skillId - 1) >= 0x21F) continue;
        category = datCommandSelectors[skillId].kind;
        if (category == 2) continue;
        if (category == 1) continue;
        switch (datCommandSelectors[skillId].stat) {
        case -1:
        case 15:
        case 16:
        case 17:
        case 18:
            continue;
        }
        if (!(datCommandRecords[skillId].unk_01 & 2)) continue;
        switch (skillId) {
        case 0x09:
        case 0x12:
        case 0x1B:
        case 0x24:
        case 0x2D:
        case 0x34:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5F:
        case 0xAB:
        case 0xAC:
        case 0xBF:
            break;
        default:
            choices[count++] = skillId;
            break;
        }
    }
    if (count != 0) {
        actor->indexWork.phase = 2;
        actor->indexWork.skillId = choices[effMiscRandMod(NULL, count)];
        btlBossDebugPrintf("btl:boss CHERUN shadow = %X\n", actor->indexWork.skillId);
    }
}

extern const char D_0041AAF8[];
s32 func_0021F698(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = battle->units;
    BtlUnit *choices[3];
    BtlUnit **nextChoice;
    u32 count = 0;
    s32 unitId;
    BattleSummonState *state;

    if (unit != NULL) {
        nextChoice = choices;
        do {
            u32 flags = unit->status.flags;
            if (flags & 1) {
                if (flags & 0x200) {
                    if (!(flags & 0xE0) && count < 3) {
                        *nextChoice++ = unit;
                        count++;
                    }
                }
            }
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    if (count == 0) {
        return 0x12A;
    }
    unit = choices[effMiscRandMod(NULL, count)];
    btlBossDebugPrintf(D_0041AAF8, unit->partyRecord.unitId);
    state = &battle->effect->summon;
    unitId = unit->partyRecord.unitId;
    state->selectedUnitId = unitId;
    switch (unitId) {
    case 1:
        return 0x12A;
    case 2:
        return 0x12B;
    case 5:
        return 0x12C;
    case 6:
        return 0x12D;
    default:
        return 0x12A;
    }
}


u32 btlHasActiveSpecialMotionActor(void) {
    BtlUnit *unit = ((BattleActionScene *)btlGetRuntime())->units;
    while (unit != 0) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 action = unit->partyRecord.unitId;
                if (action < 0x12e) {
                    if (action >= 0x12a) {
                        return 1;
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return 0;
}

u32 btlIsMarkedActionSceneStateActive(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->battleMode != 0x30e) {
        return 0;
    }
    return battle->effect->summon.active != 0;
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041AAF8);

INCLUDE_ASM(const s32, "game/code_00214948", func_0021F848);

/* Same transition for the alternate marked-unit motion (mode 0x11B). */
void func_00220368(void) {
    BattleActionScene *scene = (BattleActionScene *)btlGetRuntime();
    u8 *state = scene->state;
    BtlUnit *unit;

    if (state[1] != state[0]) {
        state[1] = state[0];
        unit = scene->units;
        if (unit != 0) {
            while (unit != 0) {
                if (unit->status.flags & 1) {
                    if (unit->status.flags & 0x400) {
                        if (unit->partyRecord.unitId == 0x11B) {
                            break;
                        }
                    }
                }
                unit = unit->nextActor;
            }
            if (unit != 0) {
                btlRefreshUnitMotionSelection(unit);
                unit->unkF8 = 0;
                unit->unkFA = 0;
                btlApplyUnitMotionSelection(unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
            }
        }
    }
}

/* This command mode allocates exactly three state bytes. */

void func_00220450(BtlUnit *unused, s32 *delta) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleMarkedCommandState *state = &battle->effect->markedCommand;
    ActionStateLink *actor;

    if (delta[10] & 0x8000) {
        actor = battle->tasks;
        if (actor != NULL) {
            while (actor != NULL) {
                if (actor->pendingFlags & 8) {
                    BtlUnit *unit = actor->unit;
                    u32 flags = unit->status.flags;

                    if (flags & 1) {
                        if (flags & 0x400) {
                            if (unit->partyRecord.unitId == 0x11B) {
                                break;
                            }
                        }
                    }
                }
                actor = actor->next;
            }
            if (actor != NULL) {
                switch (state->current) {
                case 0:
                    state->requested = 1;
                    actor->actionNumber = 4;
                    break;
                case 1:
                    state->requested = 0;
                    actor->actionNumber = 2;
                    break;
                }
            }
        }
    }
}

u32 btlSetBattleActionFlag(u32 unused1, u32 unused2, u32 action) {
    u8 *state = ((BattleActionScene *)btlGetRuntime())->state;
    if (action >= 0x103) {
        if (action >= 0x105) {
            if (action == 0x19C) {
                state[2] = 1;
            }
        } else {
            state[2] = 0;
        }
    }
    return 0;
}

s32 btlRemapMarkedUnitCommand(BtlUnit *unit, s32 action) {
    u32 flags = unit->status.flags;

    if ((flags & 0x400) == 0) {
        return action;
    }
    if ((flags & 2) == 0) {
        return action;
    }
    if (action == 1) {
        if (func_00220918() == 1) {
            return 0x11;
        }
    }
    if (action == 0) {
        if (func_00220918() == 1) {
            return 0x10;
        }
    }
    if (action == 0xA) {
        if (func_00220918() == 1) {
            return 0x12;
        }
    }
    if (action == 0xD) {
        return -1;
    }
    if (action < 0x15) {
        if (action >= 0x13) {
            return func_00220918() != 0 ? 0x14 : 0x13;
        }
    }
    return action;
}

s32 btlIsSupportedActorAction(BtlUnit *actor, s32 action) {
    if ((actor->status.flags & 0x400) == 0) {
        return 0;
    }
    switch (action) {
    case 7:
    case 8:
    case 16:
    case 18:
        return 1;
    default:
        return 0;
    }
}

s32 btlGetHealthyAllyActionStatus(BtlUnit *unit) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    BtlUnit *actor;

    if (unit == NULL) {
        actor = battle->units;
        while (actor != NULL) {
            if (actor->status.flags & 1) {
                if (actor->status.flags & 0x400) {
                    if (actor->partyRecord.unitId == 0x11B) {
                        break;
                    }
                }
            }
            actor = actor->nextActor;
        }
        if (actor != NULL) {
            s32 status = actor->unkEC;

            if (status != 0x10) {
                if (status != 0x12) {
                    return -1;
                }
            }
            return status;
        }
        return -1;
    }
    if (unit->status.flags & 0x400) {
        if (unit->partyRecord.unitId == 0x11B) {
            if (battle->state[2] != 0) {
                return func_00220918() != 0 ? 8 : 7;
            }
        }
    }
    return -1;
}

s32 func_00220810(BtlLinkedCommand *command) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 actionCode = command->actionCode;
    BtlUnit *unit;
    f32 position[4];
    f32 originalPosition;
    f32 adjustedPosition;

    if (actionCode < 0x105) {
        if (actionCode >= 0x103) {
            for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
                if (unit->status.flags & 1) {
                    if (unit->status.flags & 0x400) {
                        if (unit->partyRecord.unitId == 0x11B) {
                            break;
                        }
                    }
                }
            }
            if (unit == NULL) {
                return 0;
            }
            func_001E3108(unit, position);
            originalPosition = position[2];
            if (command->actionCode == 0x103) {
                adjustedPosition = originalPosition + 400.0f;
            } else {
                adjustedPosition = originalPosition + 575.0f;
            }
            position[2] = adjustedPosition;
            btlSetUnitPosition(unit, position);
        }
    }
    return 0;
}

u32 func_00220918(void) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    u8 *data;
    if (battle->mode != 0x314) {
        return 0;
    }
    data = battle->state;
    if (data != 0) {
        return data[1];
    }
    return 0;
}

u32 func_00220958(void) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    u8 *data;
    if (battle->mode != 0x314) {
        return 0;
    }
    data = battle->state;
    if (data != 0) {
        return *data;
    }
    return 0;
}

void func_00220998(void) {
    ptyRebalanceFrontline(9);
}

void btlMarkUnitActionAndStatusForMode(BtlUnit *unit) {
    if ((unit->status.flags & 0x200) != 0 &&
        unit->partyRecord.unitId == 9) {
        unit->status.stateFlags |= 0x2000;
        unit->partyRecord.flags |= 0x4000;
    }
}

u32 btlStartAction19A(BtlLinkedCommand *unit) {
    u32 action = unit->actionCode;
    if (action < 0x19C) {
        if (action >= 0x19A) {
            unit->flags |= 0x800;
            btlFaceActionParticipantsTowardLinkedTarget(unit);
            return 1;
        }
    }
    return 0;
}

u32 btlTickAction19A(BtlLinkedCommand *unit) {
    u32 action = unit->actionCode;
    if (action < 0x19C) {
        if (action >= 0x19A) {
            btlSetRuntimeFlag2000();
            return 1;
        }
    }
    return 0;
}

s32 btlSelectMarkedActorAndClearEntryFlags(BtlUnit *unit, BtlOperandEntry *entry) {
    BtlUnit **state;

    if (unit->status.flags & 0x400) {
        state = (BtlUnit **)((BattleActionScene *)btlGetRuntime())->state;
        entry->flags &= ~1;
        entry->flags &= ~2;
        if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0)) {
            if (*state != 0 && *state != unit) {
                btlRestoreUnitMinimumValueAndClearStatus(unit, entry);
            } else {
                *state = unit;
            }
        }
    }
}

void btlQueueLoneFreeTeamHandle(void)
{
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *actor;
    BtlUnit *first;
    BtlUnit *second;
    s32 firstUnavailable;
    s32 secondUnavailable;
    ActionStateLink *handle;

    if (battle->effect->linked.actor != 0) {
        first = NULL;
        second = NULL;
        firstUnavailable = 1;
        secondUnavailable = 1;
        for (actor = battle->units; actor != NULL; actor = actor->nextActor) {
            if (actor->status.flags & 1) {
                if (actor->status.flags & 0x400) {
                    switch (actor->partyRecord.unitId) {
                    case 0x10E:
                        first = actor;
                        if (!(actor->status.flags & 0xE0)) {
                            firstUnavailable = 0;
                        }
                        break;
                    case 0x10F:
                        second = actor;
                        if (!(actor->status.flags & 0xE0)) {
                            secondUnavailable = 0;
                        }
                        break;
                    }
                }
            }
        }
        if (!((firstUnavailable == 0 && secondUnavailable == 0) ||
              (firstUnavailable != 0 && secondUnavailable != 0))) {
            BtlUnit *selectedUnit = firstUnavailable ? second : first;

            handle = btlFindUnitByActor(selectedUnit);
            fldAppendSceneGroupHandle(handle);
            handle->indexWork.phase = 0x11;
            handle->flags |= 8;
            btlAppendIndexListEntry(handle->indexWork.indices, handle->unit);
        }
    }
}

/* Consume the selected actor after starting resource and sound tasks. Sound's
 * condition observes the resource task's running phase, not its completion.
 */
void btlQueueSelectedActorResourceAndSound(ActionUnit *unit) {
    BattleActionScene *scene = (BattleActionScene *)btlGetRuntime();
    BtlUnit **slot = (BtlUnit **)scene->state;
    BtlRuntimeTask *task;
    BtlRuntimeTask *sound;

    if (*slot != 0) {
        task = (BtlRuntimeTask *)btlCreateScriptResourceTask(*slot, (*slot)->partyRecord.unitId == 0x10E ? 0x61 : 0x62);
        task->ownerId = ((BtlUnit *)unit->parentUnit)->owner;
        task->startDelay = 0xE;
        btlStartTask(task);
        sound = sndCreateStationedSeTask(scene->soundSequence + ((*slot)->partyRecord.unitId == 0x10E ? 3 : 2));
        sound->startCondition.kind = BTL_TASK_CONDITION_HANDLE_RUNNING_OR_ABSENT;
        sound->startCondition.value.handle = task->handle;
        btlStartTask(sound);
        *slot = 0;
        unit->actorFlags &= ~8;
    }
}

u32 btlGetSelectedActorAction(void) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    BtlUnit *unit = *(BtlUnit **)battle->state;
    if (unit == NULL) {
        return 0x10e;
    }
    return unit->partyRecord.unitId;
}

void btlClearActionPhaseOnNegativeState(BtlUnit *unit, u32 state) {
    if ((unit->status.flags & 0x400) &&
        unit->partyRecord.unitId == 0x116 &&
        *(s32 *)state < 0) {
        BattleActionFlagState *battleState = (BattleActionFlagState *)((BattleActionScene *)btlGetRuntime())->state;
        battleState->active = 0;
        battleState->phase = 0;
    }
}

u32 btlActivateMarkedActionFromCommand(u32 unused1, u32 unused2, u32 action) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    BattleActionFlagState *target = (BattleActionFlagState *)battle->state;
    if (action == 0x1a5) {
        target->active = 1;
    }
    return 0;
}

extern u32 btlIsLinkedActionSceneStateActive(void);

s32 func_00220DD8(BtlUnit *unit, s32 command) {
    if (!(unit->status.flags & 0x400)) {
        return command;
    }
    if (!(unit->status.flags & 2)) {
        return command;
    }
    if (unit->partyRecord.unitId == 0x116) {
        switch (command) {
        case 2:
        case 9:
            return btlIsLinkedActionSceneStateActive() ? 0x10 : 0;
        case 13:
            return btlIsLinkedActionSceneStateActive() ? 0x11 : -1;
        default:
            if (btlIsLinkedActionSceneStateActive()) {
                switch (command) {
                case 3:
                    return 0x12;
                case 4:
                    return 0x13;
                case 7:
                    return 0x14;
                case 14:
                    return 0x15;
                }
            }
            break;
        }
    }
    return command;
}


f32 btlGetActionScaleFactor(BtlUnit *unit, BtlUnit *other) {
    f32 factor = 1.0f;
    BattleActionFlagState *state;
    if ((unit->status.flags & 0x200) == 0) {
        return factor;
    }
    if ((other->status.flags & 0x400) == 0) {
        return factor;
    }
    if (other->partyRecord.unitId != 0x116) {
        return factor;
    }
    state = (BattleActionFlagState *)((BattleActionScene *)btlGetRuntime())->state;
    if (state->active != 0 && state->phase < 4) {
        return datBattleParameters->actionScale;
    }
    return 1.0f;
}

s32 func_00220F68(BtlUnit *unit) {
    BattleActionScene *scene = (BattleActionScene *)btlGetRuntime();
    BtlUnit *found;

    if (unit == 0) {
        found = scene->units;
        if (found == 0) {
            return -1;
        }
        while (found != 0) {
            if (found->status.flags & 1) {
                if (found->status.flags & 0x400) {
                    if (found->partyRecord.unitId == 0x116) {
                        break;
                    }
                }
            }
            found = found->nextActor;
        }
        if (found == 0) {
            return -1;
        }
        return found->unkEC == 0x10 ? 0x10 : -1;
    }
    if ((unit->status.flags & 0x400) == 0) {
        return -1;
    }
    if (unit->partyRecord.unitId != 0x116) {
        return -1;
    }
    return *scene->state != 0 ? 0x10 : -1;
}

u32 btlAcceptLinkedActorCommand(BtlUnit *unit, u32 command) {
    if ((unit->status.flags & 0x400) == 0) {
        return 0;
    }
    if (unit->partyRecord.unitId != 0x116) {
        return 0;
    }
    return command == 0x10;
}

u32 btlIsLinkedActionSceneStateActive(void) {
    BattleActionScene *battle = (BattleActionScene *)btlGetRuntime();
    if (battle->mode != 0x30f) {
        return 0;
    }
    return *(u8 *)battle->state != 0;
}

u32 btlStartLinkedDefeatCandidateAction(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x1a4) {
        unit->flags |= 0x800;
        btlFaceActionParticipantsTowardLinkedTarget(unit);
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(unit->link->unit);
        return 1;
    }
    return 0;
}

u32 btlTickLinkedDefeatCandidateAction(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x1a4) {
        btlSetRuntimeFlag2000();
        return 1;
    }
    return 0;
}

s32 func_00221158(ActionStateLink *link) {
    BattleEffectPayload *effect;
    BtlOperandGroup *group;
    BtlUnit *entry;
    u32 count;
    s32 i;
    u32 k;
    s32 matched;
    s32 hasHpLoss;

    if (link == 0) {
        return -1;
    }
    if (link->unit == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->query.enabled == 0) {
        return -1;
    }
    matched = 0;
    hasHpLoss = 0;
    group = link->indexWork.groups;
    if (link->unit->status.flags & 0x200) {
        count = btlGetIndexListCount(link->indexWork.indices);
        for (i = 0; i < count; i++, group++) {
            entry = btlGetIndexListEntry(link->indexWork.indices, i);
            if (entry->status.flags & 0x400) {
                if (entry->partyRecord.unitId == 0x116) {
                    matched = 1;
                    if (group->reflected == 0) {
                        for (k = 0; k < group->count; k++) {
                            if (group->entries[k].hpDelta < 0) {
                                hasHpLoss = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    } else {
        count = btlGetIndexListCount(link->indexWork.indices);
        for (i = 0; i < count; i++, group++) {
            entry = btlGetIndexListEntry(link->indexWork.indices, i);
            if (entry->status.flags & 0x200) {
                if (group->reflected != 0) {
                    matched = 1;
                    for (k = 0; k < group->count; k++) {
                        if (group->entries[k].hpDelta < 0) {
                            hasHpLoss = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (!matched) {
        return -1;
    }
    if (link->indexWork.parameter == 0xB) {
        effect->query.count += 1;
        return 0xF3;
    }
    return hasHpLoss ? 0xF5 : -1;
}

u32 btlInitializeMarkedActionTimer(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x6b) {
        if (btlHasMarkedEntry14((u32)unit)) {
            unit->motionProgress = 0;
        } else {
            unit->motionProgress = -1;
        }
        return 1;
    }
    return 0;
}

u32 btlStartMarkedActionRuntimeUpdate(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x6b) {
        btlSetRuntimeFlag2000();
        return 1;
    }
    return 0;
}

u32 btlTickAction6B(BtlLinkedCommand *unit) {
    if (unit->actionCode != 0x6b) {
        return 0;
    }
    /* The callee takes no arguments (see code_001DACF8.c), so retail
     * leaves $a0 holding the compared constant across these calls. */
    if (unit->motionProgress >= 0) {
        if (unit->motionProgress >= 0xF) {
            btlClearRuntimeFlag2000();
            func_001ECBF8(unit, &unit->camera);
        } else {
            btlSetRuntimeFlag2000();
        }
        ++unit->motionProgress;
    } else {
        btlSetRuntimeFlag2000();
    }
    return 1;
}

u32 btlResetDelayedActionTimer(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x6c) {
        unit->motionProgress = 0;
        return 0;
    }
    return 0;
}

u32 btlTickDelayedMarkedAction(BtlLinkedCommand *unit) {
    if (unit->actionCode != 0x6c) {
        return 0;
    }
    if (btlHasMarkedEntry14((u32)unit) && unit->motionProgress == 0x25) {
        btlClearRuntimeFlag2000();
        func_001EC868(unit, &unit->camera, 0.0f);
    }
    ++unit->motionProgress;
    return 1;
}

void btlResetActionScale(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleActionState *state = &battle->effect->action;
    state->scale = 1.0f;
}

/* Mode 789 retains one actor and chains model, light and fade tasks before
 * positioning it. Both transforms wait for the same completed fade. */
extern f32 D_003BF910[4] __attribute__((aligned(16)));

extern f32 D_003BF920[4] __attribute__((aligned(16)));

u64 btlCreateLinkedActorTransformTasks(u64 prerequisiteHandle) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleActionState *effect = &battle->effect->action;
    BtlRuntimeTask *load;
    BtlRuntimeTask *light;
    BtlRuntimeTask *fade;
    BtlRuntimeTask *task;
    BtlUnit *unit;
    u64 owner;

    if (effect->actor != NULL) {
        return btlAdvanceRuntimeSequenceCounter();
    }
    effect->actor = btlCreateUnit();
    func_001AA898(&effect->actor->partyRecord, 0x10B);
    load = btlCreateModelLoadPollTask(effect->actor, 1, 0x10B, 0);
    if (prerequisiteHandle != 0) {
        load->startCondition.value.handle = prerequisiteHandle;
        load->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    }
    btlStartTask(load);
    owner = 0x8000000000000003ULL;
    light = btlCreateUnitBaseLightTask(effect->actor);
    light->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    light->startCondition.value.handle = load->handle;
    light->ownerId = owner;
    btlStartTask(light);
    fade = func_001E5FF8(effect->actor, 0);
    fade->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    fade->startCondition.value.handle = light->handle;
    fade->ownerId = owner;
    btlStartTask(fade);
    task = btlCreateUnitRotationInterpolationTask(effect->actor, D_003BF920, 1, 1.0f);
    task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    task->startCondition.value.handle = fade->handle;
    task->ownerId = owner;
    btlStartTask(task);
    task = btlCreateUnitPositionLerpTowardTargetTask(effect->actor, D_003BF910, 1.0f);
    task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
    task->startCondition.value.handle = fade->handle;
    task->ownerId = owner;
    btlStartTask(task);
    unit = effect->actor;
    PCP_COPY_VECTOR(unit->position, D_003BF910);
    PCP_COPY_VECTOR(unit->rotation, D_003BF920);
    unit->status.flags &= ~0x80000;
    unit->status.flags &= ~1;
    unit->status.stateFlags |= 0x20000;
    return load->handle;
}

extern f32 *D_0037F770[];

extern void evtSetUnitRgbTransition(struct EvtUnit *unit, s32 duration, u32 color);

void btlRestoreLinkedActorSceneColor(void) {
    BattleEffectPayload *effect = ((BtlState *)btlGetRuntime())->effect;
    BtlUnit *actor;
    u32 packed[4];

    if (effect != NULL) {
        actor = effect->action.actor;
        if (actor != NULL && (actor->status.flags & 2)) {
            VU0_LOAD_VF(vf10, D_0037F770[0]);
            EE_MMI_RGBA_PACK(packed[0]);
            evtSetUnitRgbTransition(actor->ext, 0, packed[0]);
        }
    }
}

void btlDestroyActionActor(void) {
    BattleActionState *state;
    BtlUnit *actor;

    state = &((BtlState *)btlGetRuntime())->effect->action;
    actor = state->actor;
    if (actor != 0) {
        btlDestroyUnit(actor);
        state->actor = 0;
    }
}

u32 func_00221828(u32 unit) {
    if (((BtlUnit *)unit)->resourceKind == 1 && ((BtlUnit *)unit)->resourceIndex == 0x10b) {
        return 0;
    }
    return 1;
}

u32 func_00221858(u32 unit) {
    if (((BtlUnit *)unit)->resourceKind == 1 && ((BtlUnit *)unit)->resourceIndex == 0x10b) {
        return 0;
    }
    return 1;
}

void btlMarkSpecialActionUnit(BtlUnit *actor) {
    if ((actor->status.flags & 0x400) == 0) {
        return;
    }
    switch (actor->partyRecord.unitId) {
    case 0x11D:
    case 0x11E:
    case 0x11F:
    case 0x120:
    case 0x121:
        actor->status.stateFlags |= 0x20000;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00214948", func_002218C8);

/* Brahma's action 0x19F updates the second state word, independently of Hekato. */
s32 btlAdvanceBrahmaRatioOnAction(ActionStateLink *unit) {
    BattleActionState *state;
    SdfBattleParameters *table;

    if (unit->pendingFlags & 8) {
        if (unit->unit->status.flags & 0x400) {
            state = &((BtlState *)btlGetRuntime())->effect->action;
            if (unit->indexWork.skillId == 0x19F) {
                table = datBattleParameters;
                state->scale = state->scale * table->brahmaRatioMultiplier;
                if (table->brahmaRatioMaximum < state->scale) {
                    state->scale = table->brahmaRatioMaximum;
                }
                btlBossDebugPrintf("btl:boss BRAHMA ratio = %f\n", state->scale);
            }
        }
    }
}

f32 btlGetBrahmaActionScale(BtlUnit *unit, u32 actor, u32 action, u32 mode) {
    f32 factor = 1.0f;
    if (action == 0x19f && mode == 1 && (unit->status.flags & 0x400)) {
        BtlState *battle = (BtlState *)btlGetRuntime();
        factor = battle->effect->action.scale;
    }
    return factor;
}

s32 btlFindSpecialActionIndex(void) {
    BtlUnit *unit = ((BattleActionScene *)btlGetRuntime())->units;
    s32 selected = -1;
    while (unit != 0 && selected == -1) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                switch (unit->partyRecord.unitId) {
                case 0x11d: selected = 0; break;
                case 0x11e: selected = 1; break;
                case 0x11f: selected = 2; break;
                case 0x120: selected = 3; break;
                case 0x121: selected = 4; break;
                }
            }
        }
        unit = unit->nextActor;
    }
    return selected;
}

u32 btlGetSpecialActionIndex(BtlUnit *unit) {
    u32 value = 0;
    switch (unit->partyRecord.unitId) {
    case 0x11d:
        value = 0;
        break;
    case 0x11e:
        value = 1;
        break;
    case 0x11f:
        value = 2;
        break;
    case 0x120:
        value = 3;
        break;
    case 0x121:
        value = 4;
        break;
    }
    return value;
}

u32 btlGetSpecialActionGroupEntry(BtlUnit *unit, u32 group) {
    u32 action = btlGetSpecialActionIndex(unit);
    return D_003BF6C0[group][action];
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00221BC8);

void btlApplySpecialActionRenderGroup(BtlUnit *unit, u32 group, f32 opacity) {
    if ((unit->status.flags & 0x400) == 0) {
        evtUnitSetStoredParameter(unit->ext, group);
        evtSetTransitionMotionScale(unit->ext, opacity);
    } else {
        group = btlGetSpecialActionGroupEntry(unit, group);
        evtUnitSetStoredParameter(unit->ext, group);
        evtSetTransitionMotionScale(unit->ext, opacity);
    }
}

void btlPrepareSpecialActionSelection(BtlUnit *unit, BtlOperandEntry *entry) {
    BattleActionState *state;

    if (unit->status.flags & 0x400) {
        if (unit->partyRecord.unitId != 0x121) {
            state = &((BtlState *)btlGetRuntime())->effect->action;
            entry->flags &= ~1;
            entry->flags &= ~2;
            if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0)) {
                btlRestoreUnitMinimumValueAndClearStatus(unit, entry);
                state->pending = 1;
            }
        }
    }
}

u32 func_00221F40(u32 unit, u32 actor, u32 action) {
    switch (action) {
    case 0x178:
    case 0x17d:
    case 0x19d:
        func_001ADFE0(unit, 1, 0x7f);
        func_001ADFE0(unit, 4, 0x7f);
        func_001ADFE0(unit, 0x10, 0x7f);
        func_001ADFE0(unit, 0x40, 0x7f);
        func_001ADFE0(unit, 0x100, 0x7f);
        break;
    }
    return 0;
}

u32 btlGetMarkedActionMotionCode(u32 unit, u32 action) {
    switch (action) {
    case 0x178:
    case 0x17d:
    case 0x19d:
        return 0xeb;
    }
    return 0;
}

s32 btlQueueMarkedSpecialActorSceneGroup(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *found;
    ActionStateLink *handle;
    s32 mode;

    if (((BattleActionByteState *)scene->effect)->markedActive == 0) {
        return -1;
    }
    found = 0;
    for (unit = scene->units; unit != 0 && found == 0; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                mode = unit->partyRecord.unitId;
                if (mode < 0x122) {
                    if (mode >= 0x11D) {
                        found = unit;
                    }
                }
            }
        }
    }
    if (found == 0) {
        return -1;
    }
    if (found->status.flags & 0xE0) {
        return -1;
    }
    handle = btlFindUnitByActor(found);
    fldAppendSceneGroupHandle(handle);
    handle->indexWork.phase = 0x11;
    handle->flags |= 8;
    btlAppendIndexListEntry(handle->indexWork.indices, handle->unit);
    return -1;
}

typedef struct BattleUnitReplacement {
    u16 unitId;
    u16 replacementId;
    u16 scriptGroup;
    u16 soundOffset;
} BattleUnitReplacement;
extern BattleUnitReplacement D_003BF930[4];
extern BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *, s32, s32, f32);

void func_00222100(ActionStateLink *link) {
    BtlState *battle;
    BattleActionState *state;
    BtlRuntimeTask *script;
    BtlRuntimeTask *task;
    u16 replacementId;
    u16 scriptGroup;
    u16 soundOffset;
    u16 savedFlags;
    u16 i;

    if ((link->pendingFlags & 8) == 0) {
        return;
    }
    battle = (BtlState *)btlGetRuntime();
    state = &battle->effect->action;
    if (state->pending == 0) {
        return;
    }
    replacementId = 0;
    scriptGroup = 0x64;
    soundOffset = 0;
    for (i = 0; i < 4; i++) {
        if (D_003BF930[i].unitId == link->unit->partyRecord.unitId) {
            replacementId = D_003BF930[i].replacementId;
            scriptGroup = D_003BF930[i].scriptGroup;
            soundOffset = D_003BF930[i].soundOffset;
        }
    }
    if (replacementId == 0) {
        return;
    }
    savedFlags = link->unit->partyRecord.flags;
    func_001AA898(&link->unit->partyRecord, replacementId);
    link->unit->partyRecord.flags = savedFlags;
    script = (BtlRuntimeTask *)btlCreateScriptResourceTask(link->unit, scriptGroup);
    script->startCondition.kind = BTL_TASK_CONDITION_KIND_ABSENT;
    script->startCondition.value.handle = 0x2E;
    script->startDelay = 0xE;
    script->ownerId = link->unit->owner;
    btlStartTask(script);
    task = btlAllocateIndexedUnitEffectTask(link->unit, 0xB, 2, 1.0f);
    task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_RUNNING_OR_ABSENT;
    task->startCondition.value.handle = script->handle;
    btlStartTask(task);
    task = sndCreateStationedSeTask(battle->sequenceHandle + soundOffset);
    task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_RUNNING_OR_ABSENT;
    task->startCondition.value.handle = script->handle;
    btlStartTask(task);
    link->flags &= ~8;
    state->pending = 0;
}

s32 btlActionResourceTypeToMotionId(BtlUnit *unit, s32 action) {
    if ((unit->status.flags & 0x400) == 0) {
        return -1;
    }
    if (datActionAnimationRecords[action].kind == 0) {
        return -1;
    }
    if (datActionAnimationRecords[action].kind >= 0xB &&
        datActionAnimationRecords[action].kind <= 0x19) {
        return -1;
    }
    switch (datActionAnimationRecords[action].kind) {
    case 3: return 0xF;
    case 4: return 0x13;
    case 5: return 0x12;
    case 6: return 0x10;
    case 7: return 0x11;
    default: return 0xB;
    }
}

void btlRefreshSpecialActionUnits(void) {
    BtlUnit *unit = ((BattleActionScene *)btlGetRuntime())->units;
    while (unit != 0) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 action = unit->partyRecord.unitId;
                if (action < 0x122) {
                    if (action >= 0x11d) {
                        btlSetUnitRotation(unit, (s128 *)D_003BF950);
                        unit->status.flags &= ~0x80000;
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    func_002218C8();
}

u32 btlOffsetSpecialActionValue(BtlUnit *unit, u32 base) {
    u32 flags = unit->status.flags;
    if ((flags & 1) == 0) {
        return base;
    }
    if ((flags & 0x400) == 0) {
        return base;
    }
    switch (unit->partyRecord.unitId) {
    case 0x11d: return base;
    case 0x11e: return base + 100;
    case 0x11f: return base + 200;
    case 0x120: return base + 300;
    case 0x121: return base + 400;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00222450);

INCLUDE_ASM(const s32, "game/code_00214948", btlUnitWrapA);

void btlUnitSetCameraOffset(BtlLinkedCommand *unit) {
    btlBuildLinkedCommandCameraPair(unit, &unit->frontCamera, &unit->backCamera, 0.8f, -0.65f, 0.5f, 0, 1);
    unit->frontCamera.distance += 650.0f;
    unit->backCamera.distance += 650.0f;
    unit->motionParameter = 30.0f;
    unit->flags |= 0x41;
}

extern char D_0041ACA8[]; /* "BRAHMA:I-0 ++++\n" */

extern char D_0041ACC0[];

void btlChooseBrahmaIndividualCamera(u32 unit) {
    switch (effMiscRandMod(0, 2)) {
    case 0:
        btlBossDebugPrintf(D_0041ACA8);
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetEffectCameraKeys((s32)unit, -1823.5f, -270.6f, -1359.2f, -0.137f, -0.247f, 0.027f, 0.95f, -1302.8f,
                      -42.7f, -2059.2f, -0.123f, -0.148f, 0.011f, 0.972f, 40.0f, 30.0f);
        break;
    case 1:
        btlBossDebugPrintf(D_0041ACC0);
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetEffectCameraKeys((s32)unit, 149.7f, -65.4f, -1514.7f, -0.168f, 0.02f, -0.011f, 0.976f, 1438.0f,
                      -482.6f, -1234.3f, -0.099f, 0.195f, -0.027f, 0.966f, 40.0f, 30.0f);
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041ACA8);

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041ACC0);

void btlChooseBrahmaGroupCamera(u32 unit) {
    switch (effMiscRandMod(0, 2)) {
    case 0:
        btlBossDebugPrintf("BRAHMA:ALL-0 ++++\n");
        btlSetEffectCameraKeys((s32)unit, 723.9f, -139.3f, -1529.8f, 0.05f, -0.176f, 0.018f, -0.973f, 677.2f,
                      -17.3f, -1980.7f, 0.098f, -0.114f, 0.02f, -0.979f, 40.0f, 30.0f);
        break;
    case 1:
        btlBossDebugPrintf("BRAHMA:ALL-1 ++++\n");
        btlSetEffectCameraKeys((s32)unit, -803.2f, -117.2f, -1426.5f, -0.072f, -0.187f, 0.001f, 0.971f, -738.1f,
                      -78.6f, -1749.6f, -0.1f, -0.138f, 0.001f, 0.977f, 40.0f, 20.0f);
        break;
    }
}

extern void btlClearAllUnitDefeatCandidatesTask(void);

extern void btlApplyCombinedActorFlags(u8 *);

void btlChooseBrahmaMoveCamera(BtlLinkedCommand *command) {
    BtlUnit *target = btlGetTargetUnitForLink(command);
    if (target == 0) {
        return;
    }
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(target);
    btlApplyCombinedActorFlags((u8 *)command);
    switch (target->lookupId) {
    case 0:
        switch (btlFindSpecialActionIndex()) {
        case 2:
            btlBossDebugPrintf("BRAHMA:Move-0-0[3] ++++\n");
            btlSetEffectCameraKeys((s32)command,
                1275.6f, -1510.8f, -1025.3f, 0.186f, 0.203f, 0.03f, 0.951f, 1610.9f,
                -1633.2f, -1267.5f, 0.153f, 0.236f, 0.028f, 0.949f, 40.0f, 15.0f);
            break;
        case 3:
            btlBossDebugPrintf("BRAHMA:Move-0-0[4] ++++\n");
            btlSetEffectCameraKeys((s32)command,
                1983.1f, -1391.6f, -578.2f, 0.122f, 0.373f, 0.034f, 0.906f, 2317.9f,
                -2135.2f, -572.6f, 0.193f, 0.41f, 0.072f, 0.876f, 40.0f, 20.0f);
            break;
        default:
            btlBossDebugPrintf("BRAHMA:Move-0-0[other] ++++\n");
            btlSetEffectCameraKeys((s32)command,
                1229.7f, 47.4f, -401.0f, -0.162f, 0.312f, -0.068f, 0.924f, 1556.3f,
                246.0f, -564.7f, -0.189f, 0.32f, -0.079f, 0.915f, 40.0f, 15.0f);
            break;
        }
        /* The initial camera continues into the shared movement camera. */
    case 1:
    case 2:
        if (btlFindSpecialActionIndex() == 1) {
            btlBossDebugPrintf("BRAHMA:Move-1-2-0[2] ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -1178.1f, 354.6f, -585.7f, -0.232f, -0.338f, 0.079f, 0.899f, -1316.4f,
                476.2f, -784.9f, -0.236f, -0.289f, 0.067f, 0.915f, 40.0f, 15.0f);
        } else {
            btlBossDebugPrintf("BRAHMA:Move-1-2-0[other] ++++\n");
            btlSetEffectCameraKeys((s32)command,
                -1162.6f, 91.5f, -257.2f, -0.137f, -0.349f, 0.039f, 0.916f, -1350.3f,
                263.5f, -672.4f, -0.186f, -0.292f, 0.045f, 0.927f, 40.0f, 15.0f);
        }
        break;
    }
}

u32 btlApplySingleTargetCameraOffset(BtlLinkedCommand *unit) {
    ActionStateLink *actor = unit->link;
    BtlUnit *owner = actor->unit;
    if (owner->status.flags & 0x200) {
        if (btlGetIndexListCount(actor->indexWork.indices) == 1) {
            BtlUnit *target = btlGetIndexListEntry(actor->indexWork.indices, 0);
            if ((target->status.flags & 0x400) == 0) {
                return 0;
            }
            btlFlagAllUnitDefeatCandidatesTask();
            btlUnitSetCameraOffset(unit);
            unit->state = 0;
            return 1;
        }
    }
    return 0;
}

s32 btlAimAtLinkedTargetOrGroupCamera(BtlLinkedCommand *object) {
    ActionStateLink *state = object->link;

    if ((state->unit->status.flags & 0x200) != 0) {
        if (btlGetIndexListCount(state->indexWork.indices) == 1) {
            BtlUnit *owner = btlGetIndexListEntry(state->indexWork.indices, 0);
            if ((owner->status.flags & 0x400) != 0) {
                if ((state->unit->status.flags & 0x1000) == 0) {
                    return 0;
                }
                btlFaceLinkedTargetAndFlagDirection(object, object);
                return 1;
            }
        }
    } else {
        btlFlagAllUnitDefeatCandidatesTask();
        btlChooseBrahmaGroupCamera((u32)object);
        return 1;
    }
    return 0;
}

s32 btlLiftTowardLinkedTarget(BtlLinkedCommand *object) {
    ActionStateLink *state = object->link;

    if ((state->unit->status.flags & 0x200) != 0) {
        if (btlGetIndexListCount(state->indexWork.indices) == 1) {
            BtlUnit *owner = btlGetIndexListEntry(state->indexWork.indices, 0);
            if ((owner->status.flags & 0x400) != 0) {
                if ((state->unit->status.flags & 0x1000) == 0) {
                    return 0;
                }
                func_00217470(object, &object->camera, -0.8f, 0.5f, 35.0f);
                object->camera.distance += 500.0f;
                return 1;
            }
        }
    }
    return 0;
}

extern s32 btlCanUseActorCategoryFlag4(s32);

extern void btlSetupCameraPoseAimUnit(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001ECCB0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern char D_0041ADA8[];

s32 func_00222F18(BtlLinkedCommand *command, s8 modeA, s8 modeB) {
    s32 cameraKind;

    if (btlIsActorCategoryMarked((s32)command)) {
        return 0;
    }
    if (command->link->unit->status.flags & 0x200) {
        if (modeA == 1 || modeB != 1) {
            return 0;
        }
        if (btlHasLinkedEffectNodeTrigger(command)) {
            btlChooseBrahmaMoveCamera(command);
            command->flags |= 0x800;
            return 1;
        }
        cameraKind = datActionAnimationRecords[command->actionCode].cameraKind;
        if (cameraKind < 8) {
            if (cameraKind >= 6) {
                btlSetupCameraPoseAimUnit(command, &command->frontCamera, &command->backCamera);
                return 1;
            }
        }
        func_001ECCB0(command, &command->frontCamera, &command->backCamera);
        return 1;
    }
    if (modeA != 1 || modeB == 1) {
        if (btlCanUseActorCategoryFlag4((s32)command) == 0) {
            command->flags |= 0x800;
            btlChooseBrahmaIndividualCamera((u32)command);
            return 1;
        }
    }
    switch (command->actionCode) {
    case 0x175:
    case 0x176:
    case 0x177:
    case 0x178:
    case 0x17D:
    case 0x19D:
    case 0x19E:
    case 0x19F:
        command->flags |= 0x800;
        return 1;
    case 0x9B:
    case 0x9C:
    case 0x9F:
        command->motionProgress = 0;
        break;
    }
    btlFlagAllUnitDefeatCandidatesTask();
    switch (effMiscRandMod(NULL, 3)) {
    case 0:
        btlBossDebugPrintf(D_0041ACA8);
        btlSetEffectCameraKeys((s32)command,
            -821.6f, -1.0f, -1625.0f, -0.133f, -0.143f, 0.007f, 0.971f, -705.2f,
            -1316.4f, -603.8f, -0.139f, -0.135f, 0.006f, 0.971f, 40.0f, 25.0f);
        return 1;
    case 1:
        btlBossDebugPrintf(D_0041ACC0);
        btlSetEffectCameraKeys((s32)command,
            2440.9f, -375.9f, -2238.3f, -0.072f, 0.178f, -0.024f, 0.971f, 1175.7f,
            -1698.8f, -585.1f, -0.091f, 0.192f, -0.029f, 0.967f, 40.0f, 20.0f);
        return 1;
    case 2:
        btlBossDebugPrintf(D_0041ADA8);
        btlSetEffectCameraKeys((s32)command,
            1539.2f, -237.2f, -1616.7f, -0.107f, 0.187f, -0.033f, 0.967f, 1297.3f,
            -780.2f, -1088.7f, -0.136f, 0.187f, -0.039f, 0.963f, 40.0f, 15.0f);
        return 1;
    default:
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041ADA8);

s32 btlSetSpecialLinkedActionCamera(BtlLinkedCommand *command) {
    if (btlIsActorCategoryMarked((s32)command) == 0) {
        if (command->link->unit->status.flags & 0x200) {
            if (command->stepKind == 14) {
                func_00217470(command, &command->camera, -1.4f, 0.67f, 25.0f);
                command->camera.distance += 1250.0f;
                return 1;
            }
        } else {
            switch (command->actionCode) {
            case 0x175:
            case 0x176:
            case 0x177:
            case 0x178:
            case 0x17D:
            case 0x19D:
            case 0x19E:
            case 0x19F:
                btlSetRuntimeFlag2000();
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00223350);

s32 func_00223BD8(BtlLinkedCommand *unit) {
    s32 triggerFrame = -1;

    switch (unit->actionCode) {
    case 0x9B:
        triggerFrame = 0;
        break;
    case 0x9C:
        triggerFrame = 0x4E;
        break;
    case 0x9F:
        triggerFrame = 0x3E;
        break;
    }
    if (btlHasMarkedEntry14((s32)unit) && triggerFrame >= 0) {
        if (unit->motionProgress >= triggerFrame) {
            btlClearRuntimeFlag2000();
        }
        if (unit->motionProgress == triggerFrame) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSetEffectCameraKeys((s32)unit,
                                  336.8f, 60.8f, -1233.9f,
                                  -0.157f, 0.089f, -0.028f, 0.974f,
                                  445.5f, 171.2f, -1552.3f,
                                  -0.157f, 0.092f, -0.028f, 0.974f,
                                  40.0f, 25.0f);
        }
        unit->motionProgress++;
    }
    return 0;
}

s32 btlSelectActionCameraByTableFlags(BtlLinkedCommand *unit) {
    u32 flags = datActionAnimationRecords[unit->actionCode].flags;

    if (flags & BTL_ANIMATION_GROUP_DEFEAT_CAMERA) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA) == 0) {
            btlChooseBrahmaGroupCamera((u32)unit);
        } else {
            btlChooseBrahmaGroupCamera((u32)unit);
        }
        return 1;
    }
    if (flags & BTL_ANIMATION_TARGET_DEFEAT_CAMERA) {
        if (btlGetIndexListCount(unit->link->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlUnitSetCameraOffset(unit);
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        btlChooseBrahmaGroupCamera((u32)unit);
        return 1;
    }
    return 0;
}

s32 func_00223DD8(BtlLinkedCommand *unit) {
    u32 flags = datActionAnimationRecords[unit->actionCode].flags;

    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA) == 0) {
            btlChooseBrahmaGroupCamera((u32)unit);
        } else {
            btlChooseBrahmaGroupCamera((u32)unit);
        }
        unit->state = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        func_00222450(unit, unit, 0);
    } else if (flags & 0x8) {
        if (btlGetIndexListCount(unit->link->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlUnitSetCameraOffset(unit);
            unit->state = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlChooseBrahmaGroupCamera((u32)unit);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

extern BtlRuntimeTask *btlCreateEffObjD(BtlUnit *, s32);

/* For this action, issue two differently-owned effect tasks with the same
 * prerequisite; only the second receives an explicit 38-tick start delay.
 */
void btlSpawnBrahmaActionEffectTasks(ActionStateLink *unit, u32 action, u32 unused, u64 prerequisiteHandle) {
    s32 *state;
    BtlRuntimeTask *task;
    s32 kind;
    s64 value;

    if (action == 0x19F) {
        state = (s32 *)((BattleWork *)btlGetRuntime())->sub;
        switch (state[3]) {
        case 0:
            kind = 0xF8;
            break;
        case 1:
            kind = 0xFA;
            break;
        default:
            kind = 0xFC;
            break;
        }
        task = btlCreateEffObjB(unit->unit, kind);
        task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        task->startCondition.value.handle = prerequisiteHandle;
        task->ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(task);
        task = btlCreateEffObjD(unit->unit, 0x19F);
        task->startCondition.kind = BTL_TASK_CONDITION_HANDLE_ABSENT;
        task->startCondition.value.handle = prerequisiteHandle;
        value = btlAdvanceRuntimeSequenceCounter();
        task->startDelay = 0x26;
        task->ownerId = value;
        btlStartTask(task);
        state[3] += 1;
    }
}

u32 func_00223FB0(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x10b) {
        unit->flags = unit->flags | 0x800;
        return 1;
    }
    return 0;
}

u32 func_00223FE0(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x10b) {
        btlSetRuntimeFlag2000();
        return 1;
    }
    return 0;
}

u32 func_00224010(u32 unused, s32 motion) {
    u32 result;

    result = 0xe0;
    if (motion != 0x12d) {
        result = 0;
    }
    return result;
}

void btlRaiseActionCameraPoints(BtlLinkedCommand *unit) {
    btlBuildLinkedCommandCameraPair(unit, &unit->frontCamera, &unit->backCamera, 0.8f, 1.5f, 0.25f, 0, 1);
    unit->frontCamera.distance += 650.0f;
    unit->backCamera.distance += 650.0f;
    unit->flags |= 0x41;
    unit->motionParameter = 30.0f;
    btlAdjustCameraDirectionForDefaultPlane(&unit->frontCamera);
    btlAdjustCameraDirectionForDefaultPlane(&unit->backCamera);
}

void func_002240C0(u32 unit) {
    btlInitMotionTransformFromComponents((BtlCamState *)unit, -851.6f, -144.4f, -2098.0f, -0.068f,
                    -0.141f, -0.004f, 0.979f, 40.0f);
}

u32 btlRaiseSingleTargetCameraPoints(BtlLinkedCommand *unit) {
    ActionStateLink *actor = unit->link;
    BtlUnit *owner = actor->unit;
    if (owner->status.flags & 0x200) {
        if (btlGetIndexListCount(actor->indexWork.indices) == 1) {
            BtlUnit *target = btlGetIndexListEntry(actor->indexWork.indices, 0);
            if ((target->status.flags & 0x400) == 0) {
                return 0;
            }
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseActionCameraPoints(unit);
            unit->state = 0;
            return 1;
        }
    }
    return 0;
}

s32 btlAimLinkedTargetOrSetCameraTransform(BtlLinkedCommand *object) {
    ActionStateLink *state = object->link;

    if ((state->unit->status.flags & 0x200) != 0) {
        if (btlGetIndexListCount(state->indexWork.indices) == 1) {
            BtlUnit *owner = btlGetIndexListEntry(state->indexWork.indices, 0);
            if ((owner->status.flags & 0x400) != 0) {
                if ((state->unit->status.flags & 0x1000) == 0) {
                    return 0;
                }
                btlFaceLinkedTargetAndFlagDirection(object, object);
                return 1;
            }
        }
    } else {
        btlFlagAllUnitDefeatCandidatesTask();
        func_002240C0((u32)object);
        return 1;
    }
    return 0;
}

s32 btlLiftLinkedTargetAndUpdateMotion(BtlLinkedCommand *object) {
    ActionStateLink *state = object->link;

    if ((state->unit->status.flags & 0x200) != 0) {
        if (btlGetIndexListCount(state->indexWork.indices) == 1) {
            BtlUnit *owner = btlGetIndexListEntry(state->indexWork.indices, 0);
            if ((owner->status.flags & 0x400) != 0) {
                if ((state->unit->status.flags & 0x1000) == 0) {
                    return 0;
                }
                func_00217470(object, &object->camera, -0.8f, 0.225f, 35.0f);
                object->camera.distance += 500.0f;
                btlAdjustCameraDirectionForDefaultPlane((BtlCamState *)object);
                return 1;
            }
        }
    }
    return 0;
}

extern void btlSetupCameraPoseAimUnit(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001ECCB0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001E3108(void *, f32 *);

s32 func_002242F8(BtlLinkedCommand *command, s8 modeA, s8 modeB) {
    BtlState *battle;
    BtlUnit *unit;
    s32 cameraKind;
    f32 position[4];
    f32 z;
    f32 adjustedZ;

    if (btlIsActorCategoryMarked((s32)command)) {
        return 0;
    }
    battle = btlGetRuntime();
    if (command->link->unit->status.flags & 0x200) {
        if (modeA == 1 || modeB != 1) {
            return 0;
        }
        if (btlHasLinkedEffectNodeTrigger(command)) {
            btlFaceLinkedTargetAndFlagDirection(command, command);
            command->flags |= 0x800;
            command->stepKind = 0xE;
            return 1;
        }
        cameraKind = datActionAnimationRecords[command->actionCode].cameraKind;
        if (cameraKind < 8) {
            if (cameraKind >= 6) {
                btlSetupCameraPoseAimUnit(command, &command->frontCamera, &command->backCamera);
                return 1;
            }
        }
        func_001ECCB0(command, &command->frontCamera, &command->backCamera);
        return 1;
    }
    if (command->actionCode == 0x105 || command->actionCode == 0x12D) {
        for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x400) {
                    if (unit->partyRecord.unitId == 0x126) {
                        break;
                    }
                }
            }
        }
        if (unit != NULL) {
            func_001E3108(unit, position);
            z = position[2];
            if (command->actionCode == 0x105) {
                adjustedZ = z + -400.0f;
            } else {
                adjustedZ = z + 150.0f;
            }
            position[2] = adjustedZ;
            btlSetUnitPosition(unit, position);
        }
        return 0;
    }
    btlPrepareRandomizedActionCameraPose((s32)command, (s32)&command->frontCamera, (s32)&command->backCamera);
    command->stepKind = 4;
    return 1;
}

/* Update linked motion only for a group-0x200 owner with request 0xE; return 1 on update. */
s32 func_00224500(s32 object) {
    s32 state;
    s32 battler;

    if (btlIsActorCategoryMarked(object) == 0) {
        state = (s32)((BtlLinkedCommand *)object)->link;
        battler = (s32)((ActionStateLink *)state)->unit;
        if ((((BtlUnit *)battler)->status.flags & 0x200) != 0) {
            if (((BtlLinkedCommand *)object)->stepKind == 0xE) {
                func_00217470((BtlLinkedCommand *)object, (BtlCamState *)object, -0.8f, 0.225f, 35.0f);
                ((BtlLinkedCommand *)object)->camera.distance += 500.0f;
                btlAdjustCameraDirectionForDefaultPlane((BtlCamState *)object);
                return 1;
            }
        }
    }
    return 0;
}

s32 btlSetLinkedDefeatCameraPresetB(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate) {
    u16 *runtime = (u16 *)btlGetRuntime();
    BtlUnit *unit;
    u32 kind;
    f32 angle;

    if (command->link == NULL) {
        return 1;
    }
    unit = btlGetTargetUnitForLink(command);
    if (unit == NULL) {
        return 1;
    }
    if (unit->status.flags & 0x400) {
        return 1;
    }
    if (runtime[0x134] == 3) {
        kind = unit->lookupId;
    } else {
        kind = unit->lookupId == 0 ? 0 : 2;
    }
    btlFlagAllUnitDefeatCandidatesTask();
    switch (kind) {
    case 0:
        btlInitMotionTransformFromComponents(camera, -785.9f, -20.1f, -1457.7f,
            -0.108f, -0.2f, 0.008f, 0.965f, 40.0f);
        break;
    case 1:
        btlInitMotionTransformFromComponents(camera, 25.4f, -36.7f, -1778.6f,
            -0.081f, 0.014f, -0.015f, 0.988f, 40.0f);
        break;
    case 2:
        btlInitMotionTransformFromComponents(camera, 622.8f, -36.7f, -1458.3f,
            -0.093f, 0.166f, -0.036f, 0.972f, 40.0f);
        break;
    }
    if (rotate == 1 && runtime[0x134] == 3) {
        switch (kind) {
        case 0:
            func_003364B8(-0.13089969f);
            func_00336818(0.13089969f);
            sdfComposeVuMatrixFromRegisters();
            break;
        case 1:
            func_003364B8(-0.13089969f);
            break;
        case 2:
            angle = -0.13089969f;
            func_003364B8(angle);
            func_00336818(angle);
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

INCLUDE_ASM(const s32, "game/code_00214948", btlUnitWrapB);

INCLUDE_ASM(const s32, "game/code_00214948", func_002247D0);

s32 btlSelectRaisedCameraFromActionFlags(BtlLinkedCommand *unit) {
    u32 flags = datActionAnimationRecords[unit->actionCode].flags;

    if (flags & BTL_ANIMATION_GROUP_DEFEAT_CAMERA) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA) == 0) {
            func_002240C0((u32)unit);
        } else {
            func_002240C0((u32)unit);
        }
        return 1;
    }
    if (flags & BTL_ANIMATION_TARGET_DEFEAT_CAMERA) {
        if (btlGetIndexListCount(unit->link->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseActionCameraPoints(unit);
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        func_002240C0((u32)unit);
        return 1;
    }
    return 0;
}

s32 func_00224DF0(BtlLinkedCommand *unit) {
    u32 flags = datActionAnimationRecords[unit->actionCode].flags;

    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA) == 0) {
            func_002240C0((u32)unit);
        } else {
            func_002240C0((u32)unit);
        }
        unit->state = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetLinkedDefeatCameraPresetB(unit, &unit->camera, 0);
    } else if (flags & 0x8) {
        if (btlGetIndexListCount(unit->link->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseActionCameraPoints(unit);
            unit->state = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_002240C0((u32)unit);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

void func_00224EE8(BtlLinkedCommand *unit) {
    btlBuildLinkedCommandCameraPair(unit, &unit->frontCamera, &unit->backCamera, 0.8f, 1.0f, 0.3f, 0, 1);
    unit->frontCamera.distance += 750.0f;
    unit->backCamera.distance += 750.0f;
    unit->flags |= 0x41;
    unit->motionParameter = 30.0f;
    btlAdjustCameraDirectionForDefaultPlane(&unit->frontCamera);
    btlAdjustCameraDirectionForDefaultPlane(&unit->backCamera);
}

void func_00224F88(u32 unit) {
    btlInitMotionTransformFromComponents((BtlCamState *)unit, 81.4f, -37.8f, -1866.2f, -0.112f,
                    0.01f, -0.017f, 0.982f, 40.0f);
}

extern s32 btlGetRuntime(void);

extern void btlClearRuntimeFlag2000(void);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void func_00224EE8(BtlLinkedCommand *);

extern void btlRefreshUnitMotionSelection(BtlUnit *);

typedef struct BattleActionContext {
    u8 pad00[0x21C];
    u32 flags;
    u8 pad220[0x2C];
    BtlUnit *firstUnit;
    u8 pad250[0x18];
    u16 formationMode;
    u8 pad26A[6];
    u16 mode;
    u8 pad272[0x2E];
    s32 battleId;
    u8 pad2A4[0x474];
    BattleEffectPayload *effect;
} BattleActionContext;

extern void btlSetEffectCameraKeys(s32, f32, f32, f32, f32, f32, f32, f32, f32,
    f32, f32, f32, f32, f32, f32, f32, f32);

extern void btlClearAllUnitDefeatCandidatesTask(void);

extern u32 effMiscRandMod(void *, u32);

extern void btlBossDebugPrintf(const char *format, ...);

extern void btlFlagLinkedGroupDefeatCandidatesTask(s32);

extern char D_0041B3B8[];

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041B3B8);

void func_00224FC0(s32 actor) {
    btlFlagAllUnitDefeatCandidatesTask();
    switch (effMiscRandMod(NULL, 3)) {
    case 0:
        btlBossDebugPrintf(D_0041B3B8);
        btlSetEffectCameraKeys(actor,
            -779.2f, -58.1f, -1169.4f, -0.128f, -0.276f, 0.029f, 0.943f, 648.2f,
            -57.0f, -1638.5f, -0.089f, 0.189f, -0.026f, 0.968f, 40.0f, 20.0f);
        break;
    case 1:
        btlBossDebugPrintf("SATAN:ATTACK-1 ++++\n");
        btlSetEffectCameraKeys(actor,
            -1151.0f, 14.1f, -635.3f, -0.172f, -0.484f, 0.087f, 0.843f, -566.4f,
            -16.9f, -1472.1f, -0.114f, -0.182f, 0.012f, 0.967f, 40.0f, 25.0f);
        break;
    case 2:
        btlBossDebugPrintf("SATAN:ATTACK-2 ++++\n");
        btlSetEffectCameraKeys(actor,
            789.1f, -88.3f, -251.3f, -0.305f, 0.444f, -0.171f, 0.814f, 727.7f,
            -9.4f, -1334.9f, -0.137f, 0.222f, -0.039f, 0.954f, 40.0f, 10.0f);
        break;
    }
}


void func_002251A0(s32 actor) {
    switch (effMiscRandMod(NULL, 3)) {
    case 0:
        btlBossDebugPrintf("SATAN:I-0 ++++\n");
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetEffectCameraKeys(actor,
            -211.2f, -446.8f, -639.3f, -0.266f, -0.088f, 0.016f, 0.95f, 621.5f,
            -764.2f, -563.1f, -0.086f, 0.279f, -0.035f, 0.945f, 40.0f, 20.0f);
        break;
    case 1:
        btlBossDebugPrintf("SATAN:I-0 ++++\n");
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetEffectCameraKeys(actor,
            -583.4f, -1381.4f, -1169.0f, 0.118f, -0.198f, -0.034f, 0.962f, -632.8f,
            -425.2f, -989.0f, -0.167f, -0.226f, 0.028f, 0.949f, 40.0f, 20.0f);
        break;
    case 2:
        btlBossDebugPrintf("SATAN:I-0 ++++\n");
        /* This preset marks only the actor's linked group. */
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(actor);
        btlSetEffectCameraKeys(actor,
            706.9f, 48.6f, -363.8f, -0.333f, 0.341f, -0.146f, 0.855f, -674.4f,
            -115.1f, -856.2f, -0.23f, -0.259f, 0.053f, 0.926f, 40.0f, 25.0f);
        break;
    }
}

extern void btlApplyCombinedActorFlags(u8 *);

void btlSelectTargetCameraPose(BtlLinkedCommand *command) {
    BtlUnit *target = btlGetTargetUnitForLink(command);

    if (target == NULL) {
        return;
    }
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(target);
    btlApplyCombinedActorFlags((u8 *)command);
    switch (target->lookupId) {
    case 0:
        btlSetEffectCameraKeys((s32)command,
            464.7f, -124.1f, -797.1f, -0.11f, 0.202f, -0.039f, 0.962f, 527.6f,
            -60.9f, -1267.3f, -0.133f, 0.15f, -0.037f, 0.968f, 40.0f, 10.0f);
        break;
    case 1:
    case 2:
        btlSetEffectCameraKeys((s32)command,
            -350.8f, -62.0f, -962.5f, -0.079f, -0.207f, -0.001f, 0.964f, -736.7f,
            -32.3f, -1170.6f, -0.118f, -0.248f, 0.012f, 0.95f, 40.0f, 8.0f);
        break;
    }
}

extern void func_003364B8(f32);

extern void func_00336818(f32);

extern void sdfComposeVuMatrixFromRegisters(void);

s32 btlSetSpecialDefeatCameraPreset(BtlLinkedCommand *command, BtlCamState *camera, s32 rotate) {
    BattleActionContext *runtime = (BattleActionContext *)btlGetRuntime();
    BtlUnit *unit;
    u32 kind;
    f32 angle;
    f32 position[4];

    if (command->link == NULL) {
        return 1;
    }
    unit = btlGetTargetUnitForLink(command);
    if (unit == NULL) {
        return 1;
    }
    if (unit->status.flags & 0x400) {
        return 1;
    }
    if (runtime->formationMode == 3) {
        kind = unit->lookupId;
    } else {
        kind = unit->lookupId == 0 ? 0 : 2;
    }
    btlFlagAllUnitDefeatCandidatesTask();
    unit = runtime->firstUnit;
    while (unit != NULL) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                if (unit->partyRecord.unitId == 0x127) {
                    break;
                }
            }
        }
        unit = unit->nextActor;
    }
    if (unit != NULL) {
        PCP_COPY_VECTOR(position, unit->position);
        position[2] += 340.0f;
        btlSetUnitPosition(unit, position);
    }
    switch (kind) {
    case 0:
        btlInitMotionTransformFromComponents(camera, -612.2f, -26.6f, -1527.1f,
            -0.098f, -0.152f, 0.001f, 0.975f, 40.0f);
        break;
    case 1:
        btlInitMotionTransformFromComponents(camera, 106.1f, -78.3f, -1692.6f,
            -0.07f, 0.021f, -0.015f, 0.988f, 40.0f);
        break;
    case 2:
        btlInitMotionTransformFromComponents(camera, 706.1f, -25.4f, -1706.9f,
            -0.062f, 0.156f, -0.023f, 0.977f, 40.0f);
        break;
    }
    if (rotate == 1 && runtime->formationMode == 3) {
        switch (kind) {
        case 0:
            func_003364B8(-0.13089969f);
            func_00336818(0.13089969f);
            sdfComposeVuMatrixFromRegisters();
            break;
        case 1:
            func_003364B8(-0.13089969f);
            break;
        case 2:
            angle = -0.13089969f;
            func_003364B8(angle);
            func_00336818(angle);
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

INCLUDE_ASM(const s32, "game/code_00214948", func_00225778);

/* For a group-0x200 owner with one group-0x400 target, mark defeat candidates
 * and clear the action transition. Other owner/target combinations do nothing. */
u32 btlTryTransitionSingleTargetAction(BtlLinkedCommand *unit) {
    ActionStateLink *actor = unit->link;
    BtlUnit *owner = actor->unit;
    if (owner->status.flags & 0x200) {
        if (btlGetIndexListCount(actor->indexWork.indices) == 1) {
            BtlUnit *target = btlGetIndexListEntry(actor->indexWork.indices, 0);
            if ((target->status.flags & 0x400) == 0) {
                return 0;
            }
            btlFlagAllUnitsDefeatCandidate();
            func_00224EE8(unit);
            unit->state = 0;
            return 1;
        }
    }
    return 0;
}

extern void func_00224F88(u32);

s32 btlHandleTargetDirectionOrAction(BtlLinkedCommand *unit) {
    ActionStateLink *actor = unit->link;
    if (actor->unit->status.flags & 0x200) {
        if (btlGetIndexListCount(actor->indexWork.indices) == 1) {
            BtlUnit *target = btlGetIndexListEntry(actor->indexWork.indices, 0);
            if (target->status.flags & 0x400) {
                if ((actor->unit->status.flags & 0x1000) == 0) {
                    return 0;
                }
                btlFaceLinkedTargetAndFlagDirection(unit, unit);
                return 1;
            }
        }
    } else {
        btlFlagAllUnitDefeatCandidatesTask();
        func_00224F88((u32)unit);
        return 1;
    }
    return 0;
}

extern void func_00217470(BtlLinkedCommand *, BtlCamState *, f32, f32, f32);

s32 btlLiftUnitForLinkedTarget(BtlLinkedCommand *object) {
    ActionStateLink *actor = object->link;

    if ((actor->unit->status.flags & 0x200) != 0) {
        if (btlGetIndexListCount(actor->indexWork.indices) == 1) {
            BtlUnit *owner = btlGetIndexListEntry(actor->indexWork.indices, 0);
            if ((owner->status.flags & 0x400) != 0) {
                if ((actor->unit->status.flags & 0x1000) == 0) {
                    return 0;
                }
                func_00217470((BtlLinkedCommand *)object, (BtlCamState *)object, 1.25f, 0.0f, 30.0f);
                object->camera.distance += 150.0f;
                btlAdjustCameraDirectionForDefaultPlane((BtlCamState *)object);
                return 1;
            }
        }
    }
    return 0;
}

extern void btlSetupCameraPoseAimUnit(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001ECCB0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

s32 func_002259A0(BtlLinkedCommand *command, s8 modeA, s8 modeB) {
    s32 cameraKind;
    u32 shot;

    if (btlIsActorCategoryMarked((s32)command)) {
        return 0;
    }
    btlGetRuntime();
    if (command->link->unit->status.flags & 0x200) {
        if (modeA == 1 || modeB != 1) {
            return 0;
        }
        if (btlHasLinkedEffectNodeTrigger(command)) {
            btlSelectTargetCameraPose(command);
            command->flags |= 0x800;
            return 1;
        }
        cameraKind = datActionAnimationRecords[command->actionCode].cameraKind;
        if (cameraKind < 8) {
            if (cameraKind >= 6) {
                btlSetupCameraPoseAimUnit(command, &command->frontCamera, &command->backCamera);
                return 1;
            }
        }
        func_001ECCB0(command, &command->frontCamera, &command->backCamera);
        return 1;
    }
    if (modeA != 1 || modeB == 1) {
        command->flags |= 0x800;
        func_002251A0((s32)command);
        return 1;
    }
    if (command->actionCode == 0x109) {
        command->flags |= 0x800;
        return 1;
    }
    shot = effMiscRandMod(NULL, 3);
    switch (shot) {
    case 0:
    case 1:
        func_002251A0((s32)command);
        break;
    case 2:
        btlPrepareRandomizedActionCameraPose((s32)command, (s32)&command->frontCamera, (s32)&command->backCamera);
        command->stepKind = 4;
        break;
    }
    return 1;
}

extern s32 btlIsActorCategoryMarked(s32);

extern void btlSetRuntimeFlag2000(void);

s32 func_00225B48(BtlLinkedCommand *unit) {
    if (btlIsActorCategoryMarked((s32)unit) == 0) {
        if (unit->link->unit->status.flags & 0x200) {
            if (unit->stepKind == 14) {
                func_00217470((BtlLinkedCommand *)unit, (BtlCamState *)unit, -0.8f, 0.225f, 35.0f);
                unit->camera.distance += 500.0f;
                btlAdjustCameraDirectionForDefaultPlane((BtlCamState *)unit);
                return 1;
            }
        } else if (unit->actionCode == 0x109) {
            btlSetRuntimeFlag2000();
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00214948", func_00225BF8);

s32 btlDispatchActionByResourceFlags(BtlLinkedCommand *unit) {
    u16 flags = datActionAnimationRecords[unit->actionCode].flags;

    if (flags & BTL_ANIMATION_GROUP_DEFEAT_CAMERA) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA) == 0) {
            func_00224F88((u32)unit);
        } else {
            func_00224F88((u32)unit);
        }
        return 1;
    }
    if (flags & BTL_ANIMATION_TARGET_DEFEAT_CAMERA) {
        if (btlGetIndexListCount(unit->link->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224EE8(unit);
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        func_00224F88((u32)unit);
        return 1;
    }
    return 0;
}

extern s32 btlSetSpecialDefeatCameraPreset(BtlLinkedCommand *, BtlCamState *, s32);

s32 func_002261A8(BtlLinkedCommand *unit) {
    u16 flags = datActionAnimationRecords[unit->actionCode].flags;

    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & BTL_ANIMATION_FIXED_DEFEAT_CAMERA) == 0) {
            func_00224F88((u32)unit);
        } else {
            func_00224F88((u32)unit);
        }
        unit->state = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetSpecialDefeatCameraPreset(unit, (BtlCamState *)unit, 0);
    } else if (flags & 0x8) {
        if (btlGetIndexListCount(unit->link->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224EE8(unit);
            unit->state = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224F88((u32)unit);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

s32 btlFilterActionByUnitFlags(BtlUnit *unit, s32 action) {
    u32 flags = unit->status.flags;
    if ((flags & 0x400) == 0) {
        return action;
    }
    if ((flags & 2) == 0) {
        return action;
    }
    switch (action) {
    case 2:
    case 9:
        return 0;
    case 13:
        return -1;
    }
    return action;
}

f32 func_00226308(BtlUnit *unit, s32 actor, s32 command, s32 mode) {
    f32 scale = 1.0f;

    if (mode == 1) {
        if ((unit->status.flags & 0x400) != 0 && unit->partyRecord.unitId == 0x127) {
            switch (datCommandRecords[command].hpType) {
            case 3:
            case 4:
            case 5:
            case 8:
            case 10:
            case 11:
            case 13:
                scale = 1.0f;
                break;
            default:
                scale = datBattleParameters->specialActionScale;
                break;
            }
        }
    }
    return scale;
}

u32 btlFlagBattleForSpecialAction(u32 unit, u32 actor, u32 action) {
    BattleActionContext *battle;
    if (action != 0x109) {
        return 0;
    }
    battle = (BattleActionContext *)btlGetRuntime();
    battle->flags |= 0x20000;
    return 0;
}

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

s32 btlSelectLowestStatTarget(ActionStateLink *actor) {
    BtlState *battle;
    u8 *statIndex;
    BtlUnit *unit;
    BtlUnit *target;
    s8 minimum;

    if (!(actor->pendingFlags & 8)) {
        return 0;
    }
    if (actor->indexWork.skillId != 0x108) {
        return 0;
    }
    if (!(actor->unit->status.flags & 0x400)) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    statIndex = &battle->effect->statIndex;
    if (*statIndex >= DAT_BASE_STAT_COUNT) {
        return 0;
    }
    target = NULL;
    minimum = 99;
    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        s8 value;

        if (!(flags & 1)) {
            continue;
        }
        if (!(flags & 0x200)) {
            continue;
        }
        if (flags & 0xE0) {
            continue;
        }
        value = datGetStatWithStatusOverride(&unit->partyRecord, *statIndex);
        if (value < minimum) {
            minimum = value;
            target = unit;
        }
    }
    if (target == NULL) {
        return 0;
    }
    btlAppendIndexListEntry(actor->indexWork.indices, target);
    return 1;
}

s32 btlCheckActionUnitResourceEligibility(BtlUnit *unit, s32 type) {
    u8 resourceType;
    u32 resourceKind;
    s32 result = 0xF;

    if ((unit->status.flags & 0x400) == 0) {
        return -1;
    }
    resourceType = datActionAnimationRecords[type].kind;
    if (resourceType == 0) {
        return -1;
    }
    resourceKind = (resourceType + 0xF5) & 0xFF;
    if (resourceKind < 0xF) {
        return -1;
    }
    if (unit->partyRecord.unitId != 0x127) {
        result = -1;
    }
    return result;
}

s32 func_00226540(u32 unused1, u32 unused2, s32 action) {
    return action == 0x109 ? 0x1194 : 0x64;
}

void btlSetSpecialBattleEffectActorByte(u8 value) {
    BattleActionContext *battle;

    battle = (BattleActionContext *)btlGetRuntime();
    if (battle->battleId == 0x31b) {
        /* Mode 0x31B owns a one-byte statistic selector, not an actor word. */
        battle->effect->statIndex = value;
    }
}

extern u8 *btlGetSideIndexedActorStatusTable(s32, s32);

s32 func_00226598(BtlLinkedCommand *command) {
    BtlUnit *unit = btlGetTargetUnitForLink(command);
    u8 *table;
    s32 kind;
    f32 pos[4];

    if ((unit->status.flags & 0x400) == 0) {
        return 0;
    }
    if (unit->partyRecord.unitId == 0x113) {
        table = btlGetSideIndexedActorStatusTable(
            unit->resourceKind, unit->resourceIndex);
        if (btlHasLinkedEffectNodeTrigger(command) == 0) {
            s32 slot = command->link->indexWork.slot;
            kind = *(s16 *)(table + slot * 0x14 + 0x2C);
            if (kind == 2 || kind == 7) {
                func_001E3108(unit, pos);
                pos[2] += 400.0f;
                btlSetUnitPosition(unit, pos);
            }
        }
    }
    return 0;
}

u32 func_00226670(void) {
    return 0xffffffff;
}

s32 btlFilterRestrictedCommand(BtlUnit *battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((battler->partyRecord.flags & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

/* This command selector ignores the unit and tests only the requested command. */
u8 btlIsCommandCodeF(u32 unusedUnit, s32 command) {
    return command == 0xf;
}

s32 btlSelectDisabledCommand(BtlUnit *battler) {
    if (battler == 0) {
        return 15;
    }
    return (battler->partyRecord.flags & 0x2000) ? 15 : -1;
}

INCLUDE_ASM(const s32, "game/code_00214948", btlTrackSpecialEnemyCommandRestrictionByTurn);

s32 btlClearUnitRestrictionFlag(void) {
    BattleActionContext *battle = (BattleActionContext *)btlGetRuntime();
    BtlUnit *unit;
    if (battle->mode != 2) {
        return -1;
    }
    unit = battle->firstUnit;
    while (unit != 0) {
        if (unit->status.flags & 1) {
            if (unit->partyRecord.unitId == 0x118) {
                u16 entryFlags = unit->partyRecord.flags;
                if (entryFlags & 0x2000) {
                    unit->partyRecord.flags = entryFlags & ~0x2000;
                }
            }
        }
        unit = unit->nextActor;
    }
    return -1;
}

s64 func_00226820(ActionStateLink *unit) {
    if (unit->pendingFlags & 8) {
        return btlGetRuntime();
    }
}

/* Type 0x187 uses this frame counter for its later marked-entry motion. */
u32 func_00226850(BtlLinkedCommand *unit) {
    if (unit->actionCode == 0x187) {
        unit->motionProgress = 0;
    }
    return 0;
}

/* After frame 0x34, repeatedly apply this fixed transform while marked. */
u32 func_00226868(BtlLinkedCommand *unit) {
    if (unit->actionCode != 0x187) {
        return 0;
    }
    if (btlHasMarkedEntry14((u32)unit)) {
        if (unit->motionProgress >= 0x34) {
            btlClearRuntimeFlag2000();
            btlInitMotionTransformFromComponents((BtlCamState *)unit, 517.3f, -476.0f, -947.2f, 0.177f,
                           0.283f, 0.042f, 0.933f, 40.0f);
        }
        ++unit->motionProgress;
    }
    return 1;
}

extern void btlInitializeEffectVectorsFromSourceRecords();

void btlResetUnitPlacement(void) {
    BtlUnit *unit = *(BtlUnit **)((u8 *)btlGetRuntime() + 0x24C);

    if (unit == NULL) {
        return;
    }
    do {
        if ((unit->status.flags & 1) != 0) {
            if (unit->partyRecord.unitId == 0x118) {
                if ((unit->partyRecord.flags & 0x2000) != 0) {
                    unit->bodyOffset[0] = 0.0f;
                    unit->bodyOffset[1] = -100.0f;
                    unit->bodyOffset[2] = 60.0f;
                    unit->bodyOffset[3] = 0.0f;
                    unit->reach = 180.0f;
                    unit->height = 220.0f;
                } else {
                    btlInitializeEffectVectorsFromSourceRecords(unit, 1, 0x118);
                }
            }
        }
        unit = unit->nextActor;
    } while (unit != NULL);
}

/* Release the command restriction for each active group-0x400 unit of kind 0x118. */
void btlClearSpecialEnemyEntryFlags(void) {
    BtlUnit *unit = ((BattleActionContext *)btlGetRuntime())->firstUnit;
    while (unit != 0) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if ((flags & 0x400) && unit->partyRecord.unitId == 0x118) {
                unit->partyRecord.flags &= ~0x2000;
                btlRefreshUnitMotionSelection(unit);
            }
        }
        unit = unit->nextActor;
    }
}

/* Park this unit in the battle effect slot and drop the 0x100 and 0x8 flags. */
extern void btlBindEffectUnitAndClearStateFlags(BtlUnit *);

INCLUDE_ASM(const s32, "game/code_00214948", btlBindEffectUnitAndClearStateFlags);

extern void effObjSetInnerSecondVec(EffWorldNode *, u128 *);
extern void effMiscQuatMultiplyVU(void);
extern const s32 D_0041B4D0[];

void func_00226AB0(BtlUnit *source) {
    BattleEffectPayload *effect = ((BattleActionContext *)btlGetRuntime())->effect;
    BtlUnit *unit = effect->linked.actor;
    f32 position[4];
    f32 rotation[4] __attribute__((aligned(16)));

    if (unit == 0) {
        return;
    }

    unit->status.stateFlags &= ~0x80;
    unit->status.stateFlags &= ~0x100;
    btlSetUnitPosition(unit, unit->position);
    btlSetUnitRotation(effect->linked.actor, (s128 *)effect->linked.actor->rotation);
    unit = effect->linked.actor;

    if (unit->status.flags & 2) {
        f32 translatedX;
        f32 translatedZ;
        f32 height;

        PCP_COPY_VECTOR(position, source->position);
        translatedX = position[0] + 420.0f;
        translatedZ = position[2] + -50.0f;
        position[0] = translatedX;
        height = effect->linked.height;
        position[2] = translatedZ;
        position[1] = height;
        effObjSetInnerFirstVec(unit->effectObject, (u128 *)position);

        VU0_LOAD_VF(vf10, source->rotation);
        VU0_LOAD_VF(vf11, D_0041B4D0);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        effObjSetInnerSecondVec(effect->linked.actor->effectObject, (u128 *)rotation);
    }

    effect->linked.actor->status.stateFlags |= 0x180;
}

void btlBeginEffectActorFadeOut(void) {
    BattleEffectPayload *effect = ((BattleActionContext *)btlGetRuntime())->effect;
    BtlUnit *actor = effect->linked.actor;
    if (actor != 0) {
        u32 state = actor->status.stateFlags;
        u32 flags = actor->status.flags | 0x100;
        state &= ~0x80;
        state &= ~0x100;
        effect->linked.actor = 0;
        actor->status.flags = flags;
        actor->status.stateFlags = state;
        btlRefreshUnitMotionSelection(actor);
        actor->status.flags |= 8;
        effect->linked.height = -125.0f;
        effect->linked.speed = 20.0f;
    }
}

void btlResetEffectState(void) {
    BattleEffectPayload *state = ((BattleActionContext *)btlGetRuntime())->effect;
    state->linked.active = 1;
    state->linked.speed = 20.0f;
    state->linked.linkedUnit = NULL;
    state->linked.phase = 0;
    state->linked.value = 0;
    state->linked.timer = 0;
    state->linked.effect = 0;
    state->linked.actor = NULL;
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041B4D0);

INCLUDE_ASM(const s32, "game/code_00214948", func_00226C98);

s32 btlCheckActiveEffectForSpecialTarget(BtlUnit *actor, BtlUnit *target, s32 command, s32 bits) {
    BattleEffectPayload *effect;
    if (!(target->status.flags & 0x400)) {
        return 0;
    }
    switch (target->partyRecord.unitId) {
    case 0x12E:
    case 0x12F:
        break;
    default:
        return 0;
    }
    effect = ((BattleActionContext *)btlGetRuntime())->effect;
    if (effect->linked.active != 1) {
        return 0;
    }
    if (actor->status.flags & 0x200) {
        if (command != 0) {
            if (datCommandRecords[command].targetType == 0) {
                return 0;
            }
        }
    }
    return (bits * 2) & 4;
}

extern void btlInitializeEffectVectorsFromSourceRecords(BtlUnit *, s32, s32);

extern void btlBeginEffectActorFadeOut(void);

extern void func_001E3108(void *, f32 *);

extern void effObjSetInnerFirstVec(EffWorldNode *, u128 *);

extern void func_00226AB0(BtlUnit *);

void btlUpdateLinkedEffectUnitTransforms(void) {
    BtlUnit *twin = NULL;
    BtlUnit *mainUnit = NULL;
    BtlState *battle;
    BtlUnit *unit;
    BattleEffectPayload *effect;
    f32 position[4];
    s32 flags;

    battle = (BtlState *)btlGetRuntime();
    unit = battle->units;
    effect = battle->effect;
    while (unit != NULL) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                switch (unit->partyRecord.unitId) {
                case 0x12E:
                    mainUnit = unit;
                    break;
                case 0x12F:
                    twin = unit;
                    break;
                }
            }
        }
        unit = unit->nextActor;
    }

    if (effect->linked.active == 1 || effect->linked.actor == 0) {
        mainUnit->position[0] = 0.0f;
        btlSetUnitPosition(mainUnit, mainUnit->position);
        PCP_COPY_VECTOR(twin->position, mainUnit->position);
        btlSetUnitPosition(twin, mainUnit->position);
        btlSetUnitRotation(mainUnit, (s128 *)mainUnit->rotation);
        PCP_COPY_VECTOR(twin->rotation, mainUnit->rotation);
        btlSetUnitRotation(twin, (s128 *)mainUnit->rotation);
        if (mainUnit->resourceIndex != twin->resourceIndex) {
            btlInitializeEffectVectorsFromSourceRecords(mainUnit,
                mainUnit->resourceKind, mainUnit->resourceIndex);
            btlInitializeEffectVectorsFromSourceRecords(twin,
                twin->resourceKind, twin->resourceIndex);
        } else {
            btlInitializeEffectVectorsFromSourceRecords(mainUnit,
                mainUnit->resourceKind, mainUnit->resourceIndex);
            twin->bodyOffset[0] = 0.0f;
            twin->bodyOffset[1] = -380.0f;
            twin->reach = 170.0f;
            twin->height = 325.0f;
            twin->bodyOffset[2] = 0.0f;
            twin->bodyOffset[3] = 0.0f;
        }
        twin->status.stateFlags &= ~0x100;
        btlBeginEffectActorFadeOut();
        if ((mainUnit->status.flags & 0xE0) && !(twin->status.flags & 0xE0) &&
            mainUnit->resourceIndex == 0x12E && (mainUnit->status.flags & 2)) {
            func_001E3108(mainUnit, position);
            position[1] += 1000000.0f;
            effObjSetInnerFirstVec(mainUnit->effectObject,
                (u128 *)position);
        }
        if ((twin->status.flags & 0xE0) && !(mainUnit->status.flags & 0xE0) &&
            twin->resourceIndex == 0x12F && (twin->status.flags & 2)) {
            func_001E3108(twin, position);
            position[1] += 1000000.0f;
            effObjSetInnerFirstVec(twin->effectObject,
                (u128 *)position);
        }
    } else {
        if (twin != NULL) {
            twin->status.stateFlags &= ~0x100;
            PCP_COPY_VECTOR(position, twin->position);
            position[0] += 420.0f;
            btlSetUnitPosition(twin, position);
            btlSetUnitRotation(twin, (s128 *)twin->rotation);
            btlInitializeEffectVectorsFromSourceRecords(twin, 1, 0x12F);
            twin->status.stateFlags |= 0x100;
            func_00226AB0(twin);
            twin->status.stateFlags |= 0x200;
            twin->bodyOffset[1] = -245.0f;
            twin->reach = 155.0f;
            twin->height = 340.0f;
            twin->bodyOffset[0] = 0.0f;
            twin->bodyOffset[2] = 0.0f;
            twin->bodyOffset[3] = 0.0f;
        }
        if (mainUnit != NULL) {
            /* The snapshot is not used by the following main-unit refresh. */
            PCP_COPY_VECTOR(position, twin->position);
            btlSetUnitPosition(mainUnit, mainUnit->position);
            btlSetUnitRotation(mainUnit, (s128 *)mainUnit->rotation);
            btlInitializeEffectVectorsFromSourceRecords(mainUnit, 1, 0x12E);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00214948", D_0041B4F8);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CC0);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CC8);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CD0);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CD8);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CE0);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CE8);

INCLUDE_SDATA(const s32, "game/code_00214948", D_00436CF0);

