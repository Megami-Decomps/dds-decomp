#include "mnu.h"
#include "btl.h"
#include "btl_command.h"
#include "pcp_vu0.h"

extern s32 btlGetRuntime(void);

typedef struct BattleCommandRecord {
    u8 pad00[8];
    u8 enabled;
    u8 pad09[0xD];
    u16 kind;
    u8 pad18[0x20];
} BattleCommandRecord;

extern BattleCommandRecord *datCommandRecords;

typedef struct BtlParams {
    u8 pad0[0xBF4];
    f32 ratioScale;
    f32 ratioMax;
    u8 padBFC[0x10];
    f32 specialActionScale;
} BtlParams;

extern BtlParams *datBattleParameters;

extern u32 btlHasMarkedEntry14(u32);

extern void btlClearRuntimeFlag2000(void);

extern u32 btlGetIndexListCount(u32);

extern u32 btlGetIndexListEntry(u32, u32);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void func_00224EE8(u32);

extern void btlInitMotionTransformFromComponents(u32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void func_001E2758(void *);

typedef struct BattleActionUnit BattleActionUnit;

typedef struct BattleActor {
    u8 pad00[8];
    u32 dispatchFlags;
    u8 pad0C[0xC];
    BattleActionUnit *owner;
    u8 pad1C[8];
    u32 commandId;
    u8 pad28[0x38];
    u32 targetIndexList; /* 0x60: passed to btlGetIndexListCount/Entry */
} BattleActor;

struct BattleActionUnit {
    u8 pad00[8];
    u32 dispatchFlags;
    u8 pad0C[0x24];
    f32 position[4];
    u8 pad40[0xD0];
    u32 flags;
    BattleActor *actor;
    u8 pad118[4];
    u8 lookupId;
    u8 pad11D[3];
    u16 entryFlags;
    u8 pad122[2];
    u16 kind;
    u8 pad126[0xA];
    u32 transitionState;
    u32 type;
    u8 pad138[4];
    s32 frameCounter;
    u8 pad140[0x224];
    BattleActionUnit *next;
};

typedef struct BattleActionContext {
    u8 pad00[0x21C];
    u32 flags;
    u8 pad220[0x2C];
    BattleActionUnit *firstUnit;
    u8 pad250[0x18];
    u16 formationMode;
    u8 pad26A[6];
    u16 mode;
    u8 pad272[0x2E];
    s32 battleId;
    u8 pad2A4[0x474];
    BattleEffectState *effect;
} BattleActionContext;

extern void btlSetEffectCameraKeys(s32, f32, f32, f32, f32, f32, f32, f32, f32,
    f32, f32, f32, f32, f32, f32, f32, f32);
extern void btlClearAllUnitDefeatCandidatesTask(void);
extern u32 effMiscRandMod(void *, u32);
extern s32 btlBossDebugPrintf(const char *, ...);
extern void btlFlagLinkedGroupDefeatCandidatesTask(s32);
extern char D_0041B3B8[];

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
extern s32 btlHasLinkedEffectNodeTrigger(void *);
extern void func_001E3108(BattleActionUnit *, f32 *);


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

extern BattleActionUnit *btlGetTargetUnitForLink(BattleActionUnit *);
extern void btlFlagUnitDefeatCandidate(BattleActionUnit *);
extern void btlApplyCombinedActorFlags(u8 *);

void func_00225368(BattleActionUnit *command) {
    BattleActionUnit *target = btlGetTargetUnitForLink(command);

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

extern void btlSetUnitPosition(BattleActionUnit *, f32 *);
extern void func_003364B8(f32);
extern void func_00336818(f32);
extern void sdfComposeVuMatrixFromRegisters(void);

s32 func_002254C8(BattleActionUnit *command, BtlCamState *camera, s32 rotate) {
    BattleActionContext *runtime = (BattleActionContext *)btlGetRuntime();
    BattleActionUnit *unit;
    u32 kind;
    f32 angle;
    f32 position[4];

    if (command->actor == NULL) {
        return 1;
    }
    unit = btlGetTargetUnitForLink(command);
    if (unit == NULL) {
        return 1;
    }
    if (unit->flags & 0x400) {
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
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->kind == 0x127) {
                    break;
                }
            }
        }
        unit = unit->next;
    }
    if (unit != NULL) {
        PCP_COPY_VECTOR(position, unit->position);
        position[2] += 340.0f;
        btlSetUnitPosition(unit, position);
    }
    switch (kind) {
    case 0:
        btlInitMotionTransformFromComponents((u32)camera, -612.2f, -26.6f, -1527.1f,
            -0.098f, -0.152f, 0.001f, 0.975f, 40.0f);
        break;
    case 1:
        btlInitMotionTransformFromComponents((u32)camera, 106.1f, -78.3f, -1692.6f,
            -0.07f, 0.021f, -0.015f, 0.988f, 40.0f);
        break;
    case 2:
        btlInitMotionTransformFromComponents((u32)camera, 706.1f, -25.4f, -1706.9f,
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

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225778);

/* For a group-0x200 owner with one group-0x400 target, mark defeat candidates
 * and clear the action transition. Other owner/target combinations do nothing. */
u32 btlTryTransitionSingleTargetAction(BattleActionUnit *unit) {
    BattleActor *actor = unit->actor;
    BattleActionUnit *owner = actor->owner;
    if (owner->flags & 0x200) {
        if (btlGetIndexListCount(actor->targetIndexList) == 1) {
            BattleActionUnit *target = (BattleActionUnit *)btlGetIndexListEntry(actor->targetIndexList, 0);
            if ((target->flags & 0x400) == 0) {
                return 0;
            }
            btlFlagAllUnitsDefeatCandidate();
            func_00224EE8((u32)unit);
            unit->transitionState = 0;
            return 1;
        }
    }
    return 0;
}

extern void btlFaceLinkedTargetAndFlagDirection(s32, s32);
extern void func_00224F88(u32);

s32 btlHandleTargetDirectionOrAction(BattleActionUnit *unit) {
    BattleActor *actor = unit->actor;
    if (actor->owner->flags & 0x200) {
        if (btlGetIndexListCount(actor->targetIndexList) == 1) {
            s32 target = btlGetIndexListEntry(actor->targetIndexList, 0);
            if (((BattleActionUnit *)target)->flags & 0x400) {
                if ((actor->owner->flags & 0x1000) == 0) {
                    return 0;
                }
                btlFaceLinkedTargetAndFlagDirection(unit, unit);
                return 1;
            }
        }
    } else {
        btlFlagAllUnitDefeatCandidatesTask();
        func_00224F88(unit);
        return 1;
    }
    return 0;
}

extern void func_00217470(s32, s32, f32, f32, f32);
extern void func_001E88A8(u32);

typedef struct LiftUnitState {
    u8 pad00[0x18];
    struct LiftUnit *owner; /* 0x18 */
    u8 pad1C[0x44];
    u32 targetHandle;       /* 0x60 */
} LiftUnitState;

typedef struct LiftUnit {
    u8 pad00[0x20];
    f32 verticalOffset; /* 0x20 */
    u8 pad24[0xEC];
    u32 flags;          /* 0x110 */
    s32 state;          /* 0x114: state-link address, as in the template */
} LiftUnit;

s32 btlLiftUnitForLinkedTarget(s32 object) {
    s32 state = ((LiftUnit *)object)->state;

    if ((((LiftUnit *)((LiftUnitState *)state)->owner)->flags & 0x200) != 0) {
        if (btlGetIndexListCount(((LiftUnitState *)state)->targetHandle) == 1) {
            s32 owner = btlGetIndexListEntry(((LiftUnitState *)state)->targetHandle, 0);
            if ((((LiftUnit *)owner)->flags & 0x400) != 0) {
                if ((((LiftUnit *)((LiftUnitState *)state)->owner)->flags & 0x1000) == 0) {
                    return 0;
                }
                func_00217470(object, object, 1.25f, 0.0f, 30.0f);
                ((LiftUnit *)object)->verticalOffset += 150.0f;
                func_001E88A8(object);
                return 1;
            }
        }
    }
    return 0;
}

typedef struct BattleActionTableEntry {
    u8 pad00[3];
    u8 resourceType; /* 0x03: selects the resource class for the action */
    u16 displayCode; /* 0x04: label formatting parameter */
    u8 pad06[0x16];
    u16 flags;       /* 0x1C: special action handling */
    u8 pad1E[2];
} BattleActionTableEntry;

extern u8 *datActionAnimationRecords;

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002259A0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225B48);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225BF8);

s32 btlDispatchActionByResourceFlags(BattleActionUnit *unit) {
    u16 flags = ((BattleActionTableEntry *)datActionAnimationRecords)[unit->type].flags;

    if (flags & 0x1000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & 0x10) == 0) {
            func_00224F88((u32)unit);
        } else {
            func_00224F88((u32)unit);
        }
        return 1;
    }
    if (flags & 0x2000) {
        if (btlGetIndexListCount(unit->actor->targetIndexList) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224EE8((u32)unit);
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        func_00224F88((u32)unit);
        return 1;
    }
    return 0;
}

extern s32 func_002254C8(BattleActionUnit *, BtlCamState *, s32);

s32 func_002261A8(BattleActionUnit *unit) {
    u16 flags = ((BattleActionTableEntry *)datActionAnimationRecords)[unit->type].flags;

    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* Both arms are identical in retail; kept as written. */
        if ((flags & 0x10) == 0) {
            func_00224F88((u32)unit);
        } else {
            func_00224F88((u32)unit);
        }
        unit->transitionState = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        func_002254C8(unit, (BtlCamState *)unit, 0);
    } else if (flags & 0x8) {
        if (btlGetIndexListCount(unit->actor->targetIndexList) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224EE8((u32)unit);
            unit->transitionState = 0;
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

s32 btlFilterActionByUnitFlags(BattleActionUnit *unit, s32 action) {
    u32 flags = unit->flags;
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

f32 func_00226308(BattleActionUnit *unit, s32 actor, s32 command, s32 mode) {
    f32 scale = 1.0f;

    if (mode == 1) {
        if ((unit->flags & 0x400) != 0 && unit->kind == 0x127) {
            switch (datCommandRecords[command].kind) {
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

struct DatUnitStatus;
extern s32 datGetStatWithStatusOverride(struct DatUnitStatus *, s32);
extern void btlAppendIndexListEntry(s32, u32);

s32 func_002263D8(BattleActor *actor) {
    BattleActionContext *battle;
    u8 *statIndex;
    BattleActionUnit *unit;
    BattleActionUnit *target;
    s8 minimum;

    if (!(actor->dispatchFlags & 8)) {
        return 0;
    }
    if (actor->commandId != 0x108) {
        return 0;
    }
    if (!(actor->owner->flags & 0x400)) {
        return 0;
    }
    battle = (BattleActionContext *)btlGetRuntime();
    statIndex = (u8 *)&battle->effect->actor;
    if (*statIndex >= 5) {
        return 0;
    }
    target = NULL;
    minimum = 99;
    for (unit = battle->firstUnit; unit != NULL; unit = unit->next) {
        u32 flags = unit->flags;
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
        value = datGetStatWithStatusOverride(
            (struct DatUnitStatus *)&unit->entryFlags, *statIndex);
        if (value < minimum) {
            minimum = value;
            target = unit;
        }
    }
    if (target == NULL) {
        return 0;
    }
    btlAppendIndexListEntry(actor->targetIndexList, (u32)target);
    return 1;
}

s32 btlCheckActionUnitResourceEligibility(BattleActionUnit *unit, s32 type) {
    s32 offset = type * 0x20;
    u8 resourceType;
    u32 resourceKind;
    s32 result = 0xF;

    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    resourceType = ((BattleActionTableEntry *)(offset + (s32)datActionAnimationRecords))->resourceType;
    if (resourceType == 0) {
        return -1;
    }
    resourceKind = (resourceType + 0xF5) & 0xFF;
    if (resourceKind < 0xF) {
        return -1;
    }
    if (unit->kind != 0x127) {
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
        /* Only the low byte at +0x00 changes; other paths use the full word as an actor. */
        ((u8 *)&battle->effect->actor)[0] = value;
    }
}

extern u8 *btlGetSideIndexedActorStatusTable(s32, s32);

s32 func_00226598(BattleActionUnit *command) {
    BattleActionUnit *unit = btlGetTargetUnitForLink(command);
    u8 *table;
    s32 kind;
    f32 pos[4];

    if ((unit->flags & 0x400) == 0) {
        return 0;
    }
    if (unit->kind == 0x113) {
        table = btlGetSideIndexedActorStatusTable(
            *(s32 *)((u8 *)unit + 0xC4), *(s32 *)((u8 *)unit + 0xC8));
        if (btlHasLinkedEffectNodeTrigger(command) == 0) {
            s32 slot = *(s32 *)((u8 *)command->actor + 0x44);
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

s32 btlFilterRestrictedCommand(BattleActionUnit *battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((battler->entryFlags & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

/* This command selector ignores the unit and tests only the requested command. */
u8 btlIsCommandCodeF(u32 unusedUnit, s32 command) {
    return command == 0xf;
}

s32 btlSelectDisabledCommand(BattleActionUnit *battler) {
    if (battler == 0) {
        return 15;
    }
    return (battler->entryFlags & 0x2000) ? 15 : -1;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", btlTrackSpecialEnemyCommandRestrictionByTurn);

s32 btlClearUnitRestrictionFlag(void) {
    BattleActionContext *battle = (BattleActionContext *)btlGetRuntime();
    BattleActionUnit *unit;
    if (battle->mode != 2) {
        return -1;
    }
    unit = battle->firstUnit;
    while (unit != 0) {
        if (unit->flags & 1) {
            if (unit->kind == 0x118) {
                u16 entryFlags = unit->entryFlags;
                if (entryFlags & 0x2000) {
                    unit->entryFlags = entryFlags & ~0x2000;
                }
            }
        }
        unit = unit->next;
    }
    return -1;
}

s64 func_00226820(BattleActionUnit *unit) {
    if (unit->dispatchFlags & 8) {
        return btlGetRuntime();
    }
}

/* Type 0x187 uses this frame counter for its later marked-entry motion. */
u32 func_00226850(BattleActionUnit *unit) {
    if (unit->type == 0x187) {
        unit->frameCounter = 0;
    }
    return 0;
}

/* After frame 0x34, repeatedly apply this fixed transform while marked. */
u32 func_00226868(BattleActionUnit *unit) {
    if (unit->type != 0x187) {
        return 0;
    }
    if (btlHasMarkedEntry14((u32)unit)) {
        if (unit->frameCounter >= 0x34) {
            btlClearRuntimeFlag2000();
            btlInitMotionTransformFromComponents((u32)unit, 517.3f, -476.0f, -947.2f, 0.177f,
                           0.283f, 0.042f, 0.933f, 40.0f);
        }
        ++unit->frameCounter;
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
        if ((unit->flags & 1) != 0) {
            if (unit->mode == 0x118) {
                if ((unit->statBits & 0x2000) != 0) {
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
    BattleActionUnit *unit = ((BattleActionContext *)btlGetRuntime())->firstUnit;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if ((flags & 0x400) && unit->kind == 0x118) {
                unit->entryFlags &= ~0x2000;
                func_001E2758(unit);
            }
        }
        unit = unit->next;
    }
}

/* This effect path keeps its own view of the unit word at +0x12E. */
typedef struct BattleEffectUnitMask {
    u8 pad00[0x110];
    u32 flags;
    u8 pad114[0x1A];
    u16 statusFlags; /* 0x12E */
} BattleEffectUnitMask;

/* Park this unit in the battle effect slot and drop the 0x100 and 0x8 flags. */
void btlBindEffectUnitAndClearStateFlags(BattleActionUnit *unit) {
    BattleEffectUnitMask *view = (BattleEffectUnitMask *)unit;
    BattleActionContext *battle = (BattleActionContext *)btlGetRuntime();
    u32 flags = view->flags & ~0x100;
    u16 status = view->statusFlags;

    flags &= ~8;
    status &= 0x4000;
    *(BattleActionUnit **)battle->effect = unit;
    view->flags = flags;
    view->statusFlags = status;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226AB0);

/* This effect path treats the word normally used as an actor pointer at +0x114
 * as bit flags; keep the view separate from BattleActionUnit. */
typedef struct BattleEffectUnitView {
    u8 pad00[0x110];
    u32 flags;
    u32 stateBits;
} BattleEffectUnitView;

/* Here +0x10 is a float, although other effect paths use it as a handle. */
typedef struct BattleEffectResetView {
    BattleEffectUnitView *actor;
    u8 pad04[0xC];
    f32 value10;
    f32 speed;
} BattleEffectResetView;

void btlBeginEffectActorFadeOut(void) {
    BattleEffectResetView *effect = (BattleEffectResetView *)((BattleActionContext *)btlGetRuntime())->effect;
    BattleEffectUnitView *actor = effect->actor;
    if (actor != 0) {
        u32 state = actor->stateBits;
        u32 flags = actor->flags;
        state &= ~0x80;
        state &= ~0x100;
        flags |= 0x100;
        effect->actor = 0;
        actor->flags = flags;
        actor->stateBits = state;
        func_001E2758(actor);
        actor->flags |= 8;
        effect->value10 = -125.0f;
        effect->speed = 20.0f;
    }
}

void btlResetEffectState(void) {
    BattleEffectState *state = ((BattleActionContext *)btlGetRuntime())->effect;
    state->active = 1;
    state->speed = 20.0f;
    state->flags = 0;
    state->phase = 0;
    state->value = 0;
    state->timer = 0;
    state->effect = 0;
    state->actor = 0;
}

INCLUDE_RODATA(const s32, "game/code_00224FC0", D_0041B4D0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226C98);

s32 btlCheckActiveEffectForSpecialTarget(BtlUnit *actor, BtlUnit *target, s32 command, s32 bits) {
    BattleEffectState *effect;
    if (!(target->flags & 0x400)) {
        return 0;
    }
    switch (target->mode) {
    case 0x12E:
    case 0x12F:
        break;
    default:
        return 0;
    }
    effect = ((BattleActionContext *)btlGetRuntime())->effect;
    if (effect->active != 1) {
        return 0;
    }
    if (actor->flags & 0x200) {
        if (command != 0) {
            if (datCommandRecords[command].enabled == 0) {
                return 0;
            }
        }
    }
    return (bits * 2) & 4;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226F58);

INCLUDE_RODATA(const s32, "game/code_00224FC0", D_0041B4F8);

