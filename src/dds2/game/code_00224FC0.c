#include "mnu.h"
#include "btl.h"

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
    u8 pad00[0x18];
    BattleActionUnit *owner;
    u8 pad1C[0x44];
    u32 targetIndexList; /* 0x60: passed to btlGetIndexListCount/Entry */
} BattleActor;

struct BattleActionUnit {
    u8 pad00[8];
    u32 dispatchFlags;
    u8 pad0C[0x104];
    u32 flags;
    BattleActor *actor;
    u8 pad118[8];
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
    u8 pad250[0x20];
    u16 mode;
    u8 pad272[0x2E];
    s32 battleId;
    u8 pad2A4[0x474];
    BattleEffectState *effect;
} BattleActionContext;

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00224FC0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002251A0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225368);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002254C8);

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

extern void func_002254C8(u32, u32, u32);

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
        func_002254C8((u32)unit, (u32)unit, 0);
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

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002263D8);

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

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226598);

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

