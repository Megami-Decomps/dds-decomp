#include "common.h"
#include "pcp_vu0.h"
extern void *btlCreateUnitFadeOutTask(void *, s32, s32);
extern s64 btlStartTask(void *);

extern s32 func_001AA6F8(void);

extern void btlSetRuntimeFlag2000();

extern u32 func_00220958(void);

extern u8 D_003BF6C0[][5];

extern u8 *D_00435E44;

extern u8 D_003BF950[];

extern u32 btlHasMarkedEntry14(u32);

extern void func_001E9F30(u32);

extern void btlClearAllUnitDefeatCandidates(void);

extern void btlFlagUnitDefeatCandidate(u32);

extern u32 func_0021F808(void);

extern void btlClearRuntimeFlag2000(void);

extern void func_001EC868(u32, u32, f32);

extern void func_001ECBF8();

extern u32 btlGetIndexListCount(u32);

extern u32 btlGetIndexListEntry(u32, u32);

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlUnitSetCameraOffset(u32);

extern void func_00224020(u32);

extern void func_00224EE8(u32);

extern void func_001E88A8(u32);

extern void btlInitMotionTransformFromComponents(u32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void func_00217898(u32, u32, u32, u32, u32, f32, f32, f32);

extern void func_0023CE10(u32, u32);

extern void func_0023CE18(u32, f32);

extern void func_002228C0(u32);

extern void func_002240C0(u32);

extern void func_00224F88(u32);

extern void func_001ADFE0(u32, u32, u32);

extern void btlSetUnitRotation(u32, const u8 *);

extern void func_002218C8(void);

extern s32 func_00222450();

extern s32 func_00224598();

typedef struct ActionUnit {
    u8 pad0[8];
    u32 sequenceFlags; /* 0x08 */
    u32 actorFlags;    /* 0x0C */
    u8 pad10[8];
    s32 parentUnit;    /* 0x18: owner of this action */
    u8 pad1C[4];
    f32 verticalOffset; /* 0x20: lifted for special action visual */
    s32 parentAction;  /* 0x24 */
    u8 pad28[0x28];
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

/* The state at unit +0x114 links its owner to a selected target handle. */
typedef struct ActionStateLink {
    u8 pad00[0x18];
    u32 owner;          /* 0x18: ActionUnit address */
    u8 pad1C[0x44];
    u32 targetHandle;   /* 0x60 */
} ActionStateLink;

typedef struct BattleActionScene {
    u8 pad00[0x208];
    s32 soundSequence; /* 0x208: base ID for stationed sound */
    u8 pad20C[0x40];
    ActionUnit *units;
    u8 pad250[0x50];
    u32 mode;
    u8 pad2A4[0x474];
    u8 *state;
} BattleActionScene;

/* Battle mode controls whether word zero is an actor handle or action flags. */
typedef struct BattleActionState {
    s32 actorHandle; /* 0x00: actor-owning modes */
    f32 scale; /* 0x04 */
} BattleActionState;

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

/* Each action-table entry is 0x20 bytes; only the observed words are exposed. */
typedef struct BattleActionTableEntry {
    u8 pad00[3];
    u8 resourceType; /* 0x03: selects the resource class for the action */
    u16 displayCode; /* 0x04: label formatting parameter */
    u8 pad06[0x16];
    u16 flags;       /* 0x1C: special action handling */
    u8 pad1E[2];
} BattleActionTableEntry;

typedef struct BattleActorResource {
    u8 pad00[0xC4];
    u32 kind;  /* 0xC4: model/resource kind */
    u32 index; /* 0xC8: model/resource index */
} BattleActorResource;

typedef struct BattleActionScaleTable {
    u8 pad00[0xC00];
    f32 actionScale;
    f32 ratioMultiplier;
    f32 ratioMaximum;
} BattleActionScaleTable;

typedef struct BattleActionTask {
    u8 pad00[0x28];
    s32 delay;         /* 0x28 */
    u8 pad2C[0x14];
    u64 resourceOwner; /* 0x40 */
} BattleActionTask;

/* Handle returned by btlFindUnitByActor; these fields drive its action task. */
typedef struct BattleActorHandle {
    u8 pad00[0xC];
    u32 flags;       /* 0x0C */
    u8 pad10[8];
    u32 owner;       /* 0x18 */
    u8 pad1C[4];
    s32 phase;       /* 0x20 */
    u8 pad24[0x3C];
    s32 actorIndices; /* 0x60 */
} BattleActorHandle;

extern void func_001E2758(ActionUnit *);
extern void func_001E22D8(ActionUnit *, s32, s32, f32);
/* When the action-state byte changes, restore the marked unit's saved motion. */
void func_0021EE10(void) {
    BattleActionScene *scene = (BattleActionScene *)func_001AA6F8();
    u8 *state = scene->state;
    ActionUnit *unit;

    if (((BattleActionByteState *)state)->previous != ((BattleActionByteState *)state)->current) {
        state[1] = state[0];
        unit = scene->units;
        if (unit != 0) {
            while (unit != 0) {
                if (unit->flags & 1) {
                    if (unit->flags & 0x400) {
                        if (unit->mode == 0x115) {
                            break;
                        }
                    }
                }
                unit = unit->next;
            }
            if (unit != 0) {
                func_001E2758(unit);
                unit->motionStateA = 0;
                unit->motionStateB = 0;
                func_001E22D8(unit, unit->savedMotionIndex, unit->savedMotionB, unit->savedMotionScale);
            }
        }
    }
}

void func_0021EEF8(u32 unused, u32 actor) {
    u8 *state = ((BattleActionScene *)func_001AA6F8())->state;
    if (*(u32 *)(actor + 0x28) & 0x8000) {
        if (((BattleActionByteState *)state)->previous != 0) {
            *state = 0;
        } else {
            *state = 1;
        }
    }
}

u32 func_0021EF48(ActionUnit *unit, ActionUnit *actor, u32 action) {
    u8 *state = ((BattleActionScene *)func_001AA6F8())->state;
    state[2] = action == 0x196;
    if ((unit->flags & 0x200) &&
        (actor->flags & 0x400) &&
        actor->mode == 0x115) {
        return func_0021F808() ? 4 : 0;
    }
    return 0;
}

s32 func_0021EFD8(ActionUnit *unit) {
    if (unit == 0) {
        return func_0021F808() ? 0xf : -1;
    }
    if ((unit->flags & 0x400) &&
        unit->mode == 0x115 &&
        func_0021F808()) {
        return 0xf;
    }
    return -1;
}

extern u8 *D_00435E30;
extern char D_0041AAC8[]; /* format string */
extern char D_00436CF0[];
extern s32 func_0035C860(char *, const char *, ...);
void func_0021F040(s32 unit, u32 action, char *buffer) {
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
        func_0035C860(buffer, D_0041AAC8, D_00436CF0, ((BattleActionTableEntry *)D_00435E30)[action].displayCode, value);
    }
}

extern void func_001E3108(ActionUnit *, void *);
extern void btlSetUnitPosition(ActionUnit *, void *);
void func_0021F0E8(void) {
    BattleActionScene *scene = (BattleActionScene *)func_001AA6F8();
    ActionUnit *unit;
    ActionUnit *lead = 0;
    f32 shift;
    f32 pos[4];

    for (unit = scene->units; unit != 0; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x115) {
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
        PCP_COPY_VECTOR((u8 *)lead + 0x30, pos);
        btlSetUnitPosition(lead, pos);
        for (unit = scene->units; unit != 0; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit != lead) {
                        func_001E3108(unit, pos);
                        pos[0] = pos[0] + shift;
                        PCP_COPY_VECTOR((u8 *)unit + 0x30, pos);
                        btlSetUnitPosition(unit, pos);
                    }
                }
            }
        }
    }
}

s32 btlStartOtherMarkedUnitTasks(void) {
    ActionUnit *unit = ((BattleActionScene *)func_001AA6F8())->units;
    ActionUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x115) {
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
                            if (unit->mode != 0x115) {
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

s32 btlIsSpecialMotion(ActionUnit *actor) {
    if ((actor->flags & 0x400) == 0) {
        return 0;
    }
    switch (actor->mode) {
    case 0x12A:
    case 0x12B:
    case 0x12C:
    case 0x12D:
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F3E8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F698);

u32 btlHasActiveSpecialMotionActor(void) {
    ActionUnit *unit = ((BattleActionScene *)func_001AA6F8())->units;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 action = unit->mode;
                if (action < 0x12e) {
                    if (action >= 0x12a) {
                        return 1;
                    }
                }
            }
        }
        unit = unit->next;
    }
    return 0;
}

u32 func_0021F808(void) {
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
    if (battle->mode != 0x30e) {
        return 0;
    }
    return *(s8 *)battle->state != 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F848);

/* Same transition for the alternate marked-unit motion (mode 0x11B). */
void func_00220368(void) {
    BattleActionScene *scene = (BattleActionScene *)func_001AA6F8();
    u8 *state = scene->state;
    ActionUnit *unit;

    if (state[1] != state[0]) {
        state[1] = state[0];
        unit = scene->units;
        if (unit != 0) {
            while (unit != 0) {
                if (unit->flags & 1) {
                    if (unit->flags & 0x400) {
                        if (unit->mode == 0x11B) {
                            break;
                        }
                    }
                }
                unit = unit->next;
            }
            if (unit != 0) {
                func_001E2758(unit);
                unit->motionStateA = 0;
                unit->motionStateB = 0;
                func_001E22D8(unit, unit->savedMotionIndex, unit->savedMotionB, unit->savedMotionScale);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220450);

u32 btlSetBattleActionFlag(u32 unused1, u32 unused2, u32 action) {
    u8 *state = ((BattleActionScene *)func_001AA6F8())->state;
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

s32 func_002205C0(ActionUnit *unit, s32 action) {
    u32 flags = unit->flags;

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

s32 btlIsSupportedActorAction(ActionUnit *actor, s32 action) {
    if ((actor->flags & 0x400) == 0) {
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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220700);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220810);

u32 func_00220918(void) {
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
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
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
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
    func_0011AEE0(9);
}

void func_002209B0(ActionUnit *unit) {
    if ((unit->flags & 0x200) != 0 &&
        unit->mode == 9) {
        unit->stateFlags |= 0x2000;
        unit->statusFlags |= 0x4000;
    }
}

u32 btlStartAction19A(ActionUnit *unit) {
    u32 action = unit->action;
    if (action < 0x19C) {
        if (action >= 0x19A) {
            unit->flags |= 0x800;
            func_001E9F30((u32)unit);
            return 1;
        }
    }
    return 0;
}

u32 btlTickAction19A(ActionUnit *unit) {
    u32 action = unit->action;
    if (action < 0x19C) {
        if (action >= 0x19A) {
            btlSetRuntimeFlag2000(unit);
            return 1;
        }
    }
    return 0;
}

extern s32 func_001B2430(ActionUnit *, s32);
extern void func_001B5288(ActionUnit *, u32 *);
s32 func_00220A78(ActionUnit *unit, u32 *entry) {
    ActionUnit **state;

    if (unit->flags & 0x400) {
        state = (ActionUnit **)((BattleActionScene *)func_001AA6F8())->state;
        entry[0x28 / 4] &= ~1;
        entry[0x28 / 4] &= ~2;
        if (func_001B2430(unit, 0)) {
            if (*state != 0 && *state != unit) {
                func_001B5288(unit, entry);
            } else {
                *state = unit;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220B20);

extern u8 *btlCreateScriptResourceTask(ActionUnit *, s32);
extern u8 *sndCreateStationedSeTask(s32);
void func_00220C38(ActionUnit *unit) {
    BattleActionScene *scene = (BattleActionScene *)func_001AA6F8();
    ActionUnit **slot = (ActionUnit **)scene->state;
    u8 *task;
    u8 *sound;

    if (*slot != 0) {
        task = btlCreateScriptResourceTask(*slot, (*slot)->mode == 0x10E ? 0x61 : 0x62);
        ((BattleActionTask *)task)->resourceOwner = ((ActionUnit *)unit->parentUnit)->ownerId;
        ((BattleActionTask *)task)->delay = 0xE;
        btlStartTask(task);
        sound = sndCreateStationedSeTask(scene->soundSequence + ((*slot)->mode == 0x10E ? 3 : 2));
        sound[0] = 5;
        *(u64 *)(sound + 8) = *(u64 *)(task + 0x38);
        btlStartTask(sound);
        *slot = 0;
        unit->actorFlags &= ~8;
    }
}

u32 btlGetSelectedActorAction(void) {
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
    ActionUnit *unit = *(ActionUnit **)battle->state;
    if (unit == NULL) {
        return 0x10e;
    }
    return unit->mode;
}

void func_00220D48(ActionUnit *unit, u32 state) {
    if ((unit->flags & 0x400) &&
        unit->mode == 0x116 &&
        *(s32 *)state < 0) {
        BattleActionFlagState *battleState = (BattleActionFlagState *)((BattleActionScene *)func_001AA6F8())->state;
        battleState->active = 0;
        battleState->phase = 0;
    }
}

u32 func_00220D98(u32 unused1, u32 unused2, u32 action) {
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
    BattleActionFlagState *target = (BattleActionFlagState *)battle->state;
    if (action == 0x1a5) {
        target->active = 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220DD8);

f32 btlGetActionScaleFactor(ActionUnit *unit, ActionUnit *other) {
    f32 factor = 1.0f;
    BattleActionFlagState *state;
    if ((unit->flags & 0x200) == 0) {
        return factor;
    }
    if ((other->flags & 0x400) == 0) {
        return factor;
    }
    if (other->mode != 0x116) {
        return factor;
    }
    state = (BattleActionFlagState *)((BattleActionScene *)func_001AA6F8())->state;
    if (state->active != 0 && state->phase < 4) {
        return ((BattleActionScaleTable *)D_00435E44)->actionScale;
    }
    return 1.0f;
}

s32 func_00220F68(ActionUnit *unit) {
    BattleActionScene *scene = (BattleActionScene *)func_001AA6F8();
    ActionUnit *found;

    if (unit == 0) {
        found = scene->units;
        if (found == 0) {
            return -1;
        }
        while (found != 0) {
            if (found->flags & 1) {
                if (found->flags & 0x400) {
                    if (found->mode == 0x116) {
                        break;
                    }
                }
            }
            found = found->next;
        }
        if (found == 0) {
            return -1;
        }
        return found->actionStatus == 0x10 ? 0x10 : -1;
    }
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (unit->mode != 0x116) {
        return -1;
    }
    return *scene->state != 0 ? 0x10 : -1;
}

u32 func_00221060(ActionUnit *unit, u32 command) {
    if ((unit->flags & 0x400) == 0) {
        return 0;
    }
    if (unit->mode != 0x116) {
        return 0;
    }
    return command == 0x10;
}

u32 func_00221090(void) {
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
    if (battle->mode != 0x30f) {
        return 0;
    }
    return *(u8 *)battle->state != 0;
}

u32 func_002210D0(ActionUnit *unit) {
    if (unit->action == 0x1a4) {
        unit->flags |= 0x800;
        func_001E9F30((u32)unit);
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(((ActionStateLink *)unit->stateFlags)->owner);
        return 1;
    }
    return 0;
}

u32 func_00221128(ActionUnit *unit) {
    if (unit->action == 0x1a4) {
        btlSetRuntimeFlag2000(unit);
        return 1;
    }
    return 0;
}
INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221158);

u32 func_00221390(ActionUnit *unit) {
    if (unit->action == 0x6b) {
        if (btlHasMarkedEntry14((u32)unit)) {
            unit->actionTimer = 0;
        } else {
            unit->actionTimer = -1;
        }
        return 1;
    }
    return 0;
}

u32 func_002213E0(ActionUnit *unit) {
    if (unit->action == 0x6b) {
        btlSetRuntimeFlag2000(unit);
        return 1;
    }
    return 0;
}
u32 btlTickAction6B(u32 unit) {
    if (((ActionUnit *)unit)->action != 0x6b) {
        return 0;
    }
    /* The callee takes no arguments (see code_001DACF8.c), so retail
     * leaves $a0 holding the compared constant across these calls. */
    if (((ActionUnit *)unit)->actionTimer >= 0) {
        if (((ActionUnit *)unit)->actionTimer >= 0xF) {
            btlClearRuntimeFlag2000();
            func_001ECBF8(unit, unit);
        } else {
            btlSetRuntimeFlag2000();
        }
        ++((ActionUnit *)unit)->actionTimer;
    } else {
        btlSetRuntimeFlag2000();
    }
    return 1;
}

u32 func_00221498(ActionUnit *unit) {
    if (unit->action == 0x6c) {
        unit->actionTimer = 0;
        return 0;
    }
    return 0;
}

u32 func_002214C0(ActionUnit *unit) {
    if (unit->action != 0x6c) {
        return 0;
    }
    if (btlHasMarkedEntry14((u32)unit) && unit->actionTimer == 0x25) {
        btlClearRuntimeFlag2000();
        func_001EC868((u32)unit, (u32)unit, 0.0f);
    }
    ++unit->actionTimer;
    return 1;
}
void btlResetActionScale(void) {
    BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
    BattleActionState *state = (BattleActionState *)battle->state;
    state->scale = 1.0f;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221568);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221760);

void btlDestroyActionActor(void) {
    s32 *actorHandle;
    s32 actor;

    actor = func_001AA6F8();
    actorHandle = &((BattleActionState *)((BattleActionScene *)actor)->state)->actorHandle;
    actor = *actorHandle;
    if (actor != 0) {
        btlDestroyUnit(actor);
        *actorHandle = 0;
    }
}

u32 func_00221828(u32 unit) {
    if (((BattleActorResource *)unit)->kind == 1 && ((BattleActorResource *)unit)->index == 0x10b) {
        return 0;
    }
    return 1;
}

u32 func_00221858(u32 unit) {
    if (((BattleActorResource *)unit)->kind == 1 && ((BattleActorResource *)unit)->index == 0x10b) {
        return 0;
    }
    return 1;
}

void btlMarkSpecialActionUnit(ActionUnit *actor) {
    if ((actor->flags & 0x400) == 0) {
        return;
    }
    switch (actor->mode) {
    case 0x11D:
    case 0x11E:
    case 0x11F:
    case 0x120:
    case 0x121:
        actor->stateFlags |= 0x20000;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002218C8);

extern void btlBossDebugPrintf(const char *, ...);
s32 func_00221988(ActionUnit *unit) {
    f32 *state;
    u8 *table;

    if (unit->sequenceFlags & 8) {
        if (((ActionUnit *)unit->parentUnit)->flags & 0x400) {
            state = (f32 *)((BattleActionScene *)func_001AA6F8())->state;
            if (unit->parentAction == 0x19F) {
                table = D_00435E44;
                state[1] = state[1] * ((BattleActionScaleTable *)table)->ratioMultiplier;
                if (((BattleActionScaleTable *)table)->ratioMaximum < state[1]) {
                    state[1] = ((BattleActionScaleTable *)table)->ratioMaximum;
                }
                btlBossDebugPrintf("btl:boss BRAHMA ratio = %f\n", state[1]);
            }
        }
    }
}

f32 func_00221A30(ActionUnit *unit, u32 actor, u32 action, u32 mode) {
    f32 factor = 1.0f;
    if (action == 0x19f && mode == 1 && (unit->flags & 0x400)) {
        BattleActionScene *battle = (BattleActionScene *)func_001AA6F8();
        factor = ((BattleActionState *)battle->state)->scale;
    }
    return factor;
}
s32 btlFindSpecialActionIndex(void) {
    ActionUnit *unit = ((BattleActionScene *)func_001AA6F8())->units;
    s32 selected = -1;
    while (unit != 0 && selected == -1) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                switch (unit->mode) {
                case 0x11d: selected = 0; break;
                case 0x11e: selected = 1; break;
                case 0x11f: selected = 2; break;
                case 0x120: selected = 3; break;
                case 0x121: selected = 4; break;
                }
            }
        }
        unit = unit->next;
    }
    return selected;
}

u32 btlGetSpecialActionIndex(ActionUnit *unit) {
    u32 value = 0;
    switch (unit->mode) {
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

u32 btlGetSpecialActionGroupEntry(ActionUnit *unit, u32 group) {
    u32 action = btlGetSpecialActionIndex(unit);
    return D_003BF6C0[group][action];
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221BC8);

void func_00221E28(ActionUnit *unit, u32 group, f32 opacity) {
    if ((unit->flags & 0x400) == 0) {
        func_0023CE10(unit->rendererHandle, group);
        func_0023CE18(unit->rendererHandle, opacity);
    } else {
        group = btlGetSpecialActionGroupEntry(unit, group);
        func_0023CE10(unit->rendererHandle, group);
        func_0023CE18(unit->rendererHandle, opacity);
    }
}

void func_00221EA0(ActionUnit *unit, u32 *entry) {
    u8 *state;

    if (unit->flags & 0x400) {
        if (unit->mode != 0x121) {
            state = ((BattleActionScene *)func_001AA6F8())->state;
            entry[0x28 / 4] &= ~1;
            entry[0x28 / 4] &= ~2;
            if (func_001B2430(unit, 0)) {
                func_001B5288(unit, entry);
                state[8] = 1;
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

u32 func_00221FE8(u32 unit, u32 action) {
    switch (action) {
    case 0x178:
    case 0x17d:
    case 0x19d:
        return 0xeb;
    }
    return 0;
}

extern s32 btlFindUnitByActor(ActionUnit *);
extern void fldAppendSceneGroupHandle(s32);
extern void btlAppendIndexListEntry(s32, u32);
s32 func_00222028(void) {
    BattleActionScene *scene = (BattleActionScene *)func_001AA6F8();
    ActionUnit *unit;
    ActionUnit *found;
    s32 handle;
    s32 mode;

    if (((BattleActionByteState *)scene->state)->markedActive == 0) {
        return -1;
    }
    found = 0;
    for (unit = scene->units; unit != 0 && found == 0; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                mode = unit->mode;
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
    if (found->flags & 0xE0) {
        return -1;
    }
    handle = btlFindUnitByActor(found);
    fldAppendSceneGroupHandle(handle);
    ((BattleActorHandle *)handle)->phase = 0x11;
    ((BattleActorHandle *)handle)->flags |= 8;
    btlAppendIndexListEntry(((BattleActorHandle *)handle)->actorIndices, ((BattleActorHandle *)handle)->owner);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222100);

s32 btlActionResourceTypeToMotionId(ActionUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BattleActionTableEntry *)D_00435E30)[action].resourceType == 0) {
        return -1;
    }
    if (((BattleActionTableEntry *)D_00435E30)[action].resourceType >= 0xB &&
        ((BattleActionTableEntry *)D_00435E30)[action].resourceType <= 0x19) {
        return -1;
    }
    switch (((BattleActionTableEntry *)D_00435E30)[action].resourceType) {
    case 3: return 0xF;
    case 4: return 0x13;
    case 5: return 0x12;
    case 6: return 0x10;
    case 7: return 0x11;
    default: return 0xB;
    }
}

void btlRefreshSpecialActionUnits(void) {
    ActionUnit *unit = ((BattleActionScene *)func_001AA6F8())->units;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 action = unit->mode;
                if (action < 0x122) {
                    if (action >= 0x11d) {
                        btlSetUnitRotation((u32)unit, D_003BF950);
                        unit->flags &= ~0x80000;
                    }
                }
            }
        }
        unit = unit->next;
    }
    func_002218C8();
}

u32 btlOffsetSpecialActionValue(ActionUnit *unit, u32 base) {
    u32 flags = unit->flags;
    if ((flags & 1) == 0) {
        return base;
    }
    if ((flags & 0x400) == 0) {
        return base;
    }
    switch (unit->mode) {
    case 0x11d: return base;
    case 0x11e: return base + 100;
    case 0x11f: return base + 200;
    case 0x120: return base + 300;
    case 0x121: return base + 400;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222450);

s64 btlUnitWrapA(void) {
    return func_00222450();
}

void btlUnitSetCameraOffset(u32 unit) {
    func_00217898(unit, unit + 0x30, unit + 0xc0, 0, 1,
                  0.8f, -0.65f, 0.5f);
    ((ActionUnit *)unit)->cameraPointAHeight += 650.0f;
    ((ActionUnit *)unit)->cameraPointBHeight += 650.0f;
    ((ActionUnit *)unit)->cameraOffset = 30.0f;
    ((ActionUnit *)unit)->flags |= 0x41;
}

extern s32 effMiscRandMod(s32, s32);
extern char D_0041ACA8[]; /* "BRAHMA:I-0 ++++\n" */
extern char D_0041ACC0[];
extern void btlSetEffectCameraKeys(u32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_00222768(u32 unit) {
    switch (effMiscRandMod(0, 2)) {
    case 0:
        btlBossDebugPrintf(D_0041ACA8);
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetEffectCameraKeys(unit, -1823.5f, -270.6f, -1359.2f, -0.137f, -0.247f, 0.027f, 0.95f, -1302.8f,
                      -42.7f, -2059.2f, -0.123f, -0.148f, 0.011f, 0.972f, 40.0f, 30.0f);
        break;
    case 1:
        btlBossDebugPrintf(D_0041ACC0);
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetEffectCameraKeys(unit, 149.7f, -65.4f, -1514.7f, -0.168f, 0.02f, -0.011f, 0.976f, 1438.0f,
                      -482.6f, -1234.3f, -0.099f, 0.195f, -0.027f, 0.966f, 40.0f, 30.0f);
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041ACA8);

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041ACC0);

void func_002228C0(u32 unit) {
    switch (effMiscRandMod(0, 2)) {
    case 0:
        btlBossDebugPrintf("BRAHMA:ALL-0 ++++\n");
        btlSetEffectCameraKeys(unit, 723.9f, -139.3f, -1529.8f, 0.05f, -0.176f, 0.018f, -0.973f, 677.2f,
                      -17.3f, -1980.7f, 0.098f, -0.114f, 0.02f, -0.979f, 40.0f, 30.0f);
        break;
    case 1:
        btlBossDebugPrintf("BRAHMA:ALL-1 ++++\n");
        btlSetEffectCameraKeys(unit, -803.2f, -117.2f, -1426.5f, -0.072f, -0.187f, 0.001f, 0.971f, -738.1f,
                      -78.6f, -1749.6f, -0.1f, -0.138f, 0.001f, 0.977f, 40.0f, 20.0f);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222A08);

u32 func_00222D18(ActionUnit *unit) {
    u32 actor = unit->stateFlags;
    u32 owner = ((ActionStateLink *)actor)->owner;
    if (((ActionUnit *)owner)->flags & 0x200) {
        if (btlGetIndexListCount(((ActionStateLink *)actor)->targetHandle) == 1) {
            u32 target = btlGetIndexListEntry(((ActionStateLink *)actor)->targetHandle, 0);
            if ((((ActionUnit *)target)->flags & 0x400) == 0) {
                return 0;
            }
            btlFlagAllUnitDefeatCandidatesTask();
            btlUnitSetCameraOffset((u32)unit);
            unit->pendingAction = 0;
            return 1;
        }
    }
    return 0;
}

extern void btlFaceLinkedTargetAndFlagDirection(s32, s32);
s32 func_00222DA8(s32 object) {
    s32 state = ((ActionUnit *)object)->stateFlags;

    if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x200) != 0) {
        if (btlGetIndexListCount(((ActionStateLink *)state)->targetHandle) == 1) {
            s32 owner = btlGetIndexListEntry(((ActionStateLink *)state)->targetHandle, 0);
            if ((((ActionUnit *)owner)->flags & 0x400) != 0) {
                if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x1000) == 0) {
                    return 0;
                }
                btlFaceLinkedTargetAndFlagDirection(object, object);
                return 1;
            }
        }
    } else {
        btlFlagAllUnitDefeatCandidatesTask();
        func_002228C0(object);
        return 1;
    }
    return 0;
}

extern void func_00217470(s32, s32, f32, f32, f32);
s32 func_00222E58(s32 object) {
    s32 state = ((ActionUnit *)object)->stateFlags;

    if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x200) != 0) {
        if (btlGetIndexListCount(((ActionStateLink *)state)->targetHandle) == 1) {
            s32 owner = btlGetIndexListEntry(((ActionStateLink *)state)->targetHandle, 0);
            if ((((ActionUnit *)owner)->flags & 0x400) != 0) {
                if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x1000) == 0) {
                    return 0;
                }
                func_00217470(object, object, -0.8f, 0.5f, 35.0f);
                ((ActionUnit *)object)->verticalOffset += 500.0f;
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222F18);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223280);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223350);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223BD8);

s32 func_00223D10(ActionUnit *unit) {
    u32 flags = ((BattleActionTableEntry *)D_00435E30)[unit->action].flags;

    if (flags & 0x1000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* retail keeps both copies of this call, so the arms presumably differed in the original (e.g. a compiled-out debug call) */
        if ((flags & 0x10) == 0) {
            func_002228C0((u32)unit);
        } else {
            func_002228C0((u32)unit);
        }
        return 1;
    }
    if (flags & 0x2000) {
        if (btlGetIndexListCount(((ActionStateLink *)unit->stateFlags)->targetHandle) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlUnitSetCameraOffset((u32)unit);
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        func_002228C0((u32)unit);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223DD8);

extern u8 *btlCreateEffObjB(s32, s32);
extern u8 *btlCreateEffObjD(s32, s32);
extern s64 func_001A9920(void);
void func_00223ED0(ActionUnit *unit, u32 action, u32 unused, u64 owner) {
    s32 *state;
    u8 *task;
    s32 kind;
    s64 value;

    if (action == 0x19F) {
        state = *(s32 **)((u8 *)func_001AA6F8() + 0x718);
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
        task = btlCreateEffObjB(unit->parentUnit, kind);
        task[0] = 4;
        *(u64 *)(task + 8) = owner;
        ((BattleActionTask *)task)->resourceOwner = func_001A9920();
        btlStartTask(task);
        task = btlCreateEffObjD(unit->parentUnit, 0x19F);
        task[0] = 4;
        *(u64 *)(task + 8) = owner;
        value = func_001A9920();
        ((BattleActionTask *)task)->delay = 0x26;
        ((BattleActionTask *)task)->resourceOwner = value;
        btlStartTask(task);
        state[3] += 1;
    }
}

u32 func_00223FB0(ActionUnit *unit) {
    if (unit->action == 0x10b) {
        unit->flags = unit->flags | 0x800;
        return 1;
    }
    return 0;
}

u32 func_00223FE0(ActionUnit *unit) {
    if (unit->action == 0x10b) {
        btlSetRuntimeFlag2000(unit);
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

void func_00224020(u32 unit) {
    func_00217898(unit, unit + 0x30, unit + 0xc0, 0, 1,
                  0.8f, 1.5f, 0.25f);
    ((ActionUnit *)unit)->cameraPointAHeight += 650.0f;
    ((ActionUnit *)unit)->cameraPointBHeight += 650.0f;
    ((ActionUnit *)unit)->flags |= 0x41;
    ((ActionUnit *)unit)->cameraOffset = 30.0f;
    func_001E88A8(unit + 0x30);
    func_001E88A8(unit + 0xc0);
}

void func_002240C0(u32 unit) {
    btlInitMotionTransformFromComponents(unit, -851.6f, -144.4f, -2098.0f, -0.068f,
                    -0.141f, -0.004f, 0.979f, 40.0f);
}

u32 func_002240F8(ActionUnit *unit) {
    u32 actor = unit->stateFlags;
    u32 owner = ((ActionStateLink *)actor)->owner;
    if (((ActionUnit *)owner)->flags & 0x200) {
        if (btlGetIndexListCount(((ActionStateLink *)actor)->targetHandle) == 1) {
            u32 target = btlGetIndexListEntry(((ActionStateLink *)actor)->targetHandle, 0);
            if ((((ActionUnit *)target)->flags & 0x400) == 0) {
                return 0;
            }
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224020((u32)unit);
            unit->pendingAction = 0;
            return 1;
        }
    }
    return 0;
}

s32 func_00224188(s32 object) {
    s32 state = ((ActionUnit *)object)->stateFlags;

    if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x200) != 0) {
        if (btlGetIndexListCount(((ActionStateLink *)state)->targetHandle) == 1) {
            s32 owner = btlGetIndexListEntry(((ActionStateLink *)state)->targetHandle, 0);
            if ((((ActionUnit *)owner)->flags & 0x400) != 0) {
                if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x1000) == 0) {
                    return 0;
                }
                btlFaceLinkedTargetAndFlagDirection(object, object);
                return 1;
            }
        }
    } else {
        btlFlagAllUnitDefeatCandidatesTask();
        func_002240C0(object);
        return 1;
    }
    return 0;
}

s32 func_00224238(s32 object) {
    s32 state = ((ActionUnit *)object)->stateFlags;

    if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x200) != 0) {
        if (btlGetIndexListCount(((ActionStateLink *)state)->targetHandle) == 1) {
            s32 owner = btlGetIndexListEntry(((ActionStateLink *)state)->targetHandle, 0);
            if ((((ActionUnit *)owner)->flags & 0x400) != 0) {
                if ((((ActionUnit *)((ActionStateLink *)state)->owner)->flags & 0x1000) == 0) {
                    return 0;
                }
                func_00217470(object, object, -0.8f, 0.225f, 35.0f);
                ((ActionUnit *)object)->verticalOffset += 500.0f;
                func_001E88A8(object);
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002242F8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224500);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224598);

s64 btlUnitWrapB(void) {
    return func_00224598();
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002247D0);

s32 func_00224D28(ActionUnit *unit) {
    u32 flags = ((BattleActionTableEntry *)D_00435E30)[unit->action].flags;

    if (flags & 0x1000) {
        btlFlagAllUnitDefeatCandidatesTask();
        /* retail keeps both copies of this call, so the arms presumably differed in the original (e.g. a compiled-out debug call) */
        if ((flags & 0x10) == 0) {
            func_002240C0((u32)unit);
        } else {
            func_002240C0((u32)unit);
        }
        return 1;
    }
    if (flags & 0x2000) {
        if (btlGetIndexListCount(((ActionStateLink *)unit->stateFlags)->targetHandle) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            func_00224020((u32)unit);
            return 1;
        }
        btlFlagAllUnitDefeatCandidatesTask();
        func_002240C0((u32)unit);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224DF0);

void func_00224EE8(u32 unit) {
    func_00217898(unit, unit + 0x30, unit + 0xc0, 0, 1,
                  0.8f, 1.0f, 0.3f);
    ((ActionUnit *)unit)->cameraPointAHeight += 750.0f;
    ((ActionUnit *)unit)->cameraPointBHeight += 750.0f;
    ((ActionUnit *)unit)->flags |= 0x41;
    ((ActionUnit *)unit)->cameraOffset = 30.0f;
    func_001E88A8(unit + 0x30);
    func_001E88A8(unit + 0xc0);
}

void func_00224F88(u32 unit) {
    btlInitMotionTransformFromComponents(unit, 81.4f, -37.8f, -1866.2f, -0.112f,
                    0.01f, -0.017f, 0.982f, 40.0f);
}

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041B3B8);

INCLUDE_SDATA(const s32, "game/code_0021EE10", D_00436CF0);

