#include "common.h"
extern void *func_001E5DA8(void *, s32, s32);
extern s64 btlStartTask(void *);

extern s32 func_001AA6F8(void);

extern void func_001E98C0();

extern u32 func_00220958(void);

extern u8 D_003BF6C0[][5];

extern u8 *D_00435E44;

extern u8 D_003BF950[];

extern u32 btlHasMarkedEntry14(u32);

extern void func_001E9F30(u32);

extern void func_00208DA0(void);

extern void btlFlagUnitDefeatCandidate(u32);

extern u32 func_0021F808(void);

extern void func_001E9890(void);

extern void func_001EC868(u32, u32, f32);

extern void func_001ECBF8();

extern u32 func_001E8058(u32);

extern u32 func_001E8060(u32, u32);

extern void func_001E9A88(void);

extern void btlUnitSetCameraOffset(u32);

extern void func_00224020(u32);

extern void func_00224EE8(u32);

extern void func_001E88A8(u32);

extern void func_001E9660(u32, f32, f32, f32, f32, f32, f32, f32, f32);

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
    u8 pad0[0x110];
    u32 flags;
    u32 stateFlags;
    u8 pad118[8];
    u16 statusFlags;
    u8 pad122[2];
    u16 mode;
    u8 pad126[0xA];
    u32 pendingAction;
    u32 action;
    u8 pad138[4];
    s32 actionTimer;
    u8 pad140[0x200];
    u32 rendererHandle;
    u8 pad344[0x20];
    struct ActionUnit *next;
} ActionUnit;

typedef struct BattleActionScene {
    u8 pad00[0x24C];
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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021EE10);

void func_0021EEF8(u32 unused, u32 actor) {
    u8 *state = ((BattleActionScene *)func_001AA6F8())->state;
    if (*(u32 *)(actor + 0x28) & 0x8000) {
        if (*(s8 *)(state + 1) != 0) {
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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F040);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F0E8);

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
                                btlStartTask(func_001E5DA8(unit, 6, 0xA));
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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220368);

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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002205C0);

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
            func_001E98C0(unit);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220A78);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220B20);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220C38);

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
        return *(f32 *)(D_00435E44 + 0xc00);
    }
    return 1.0f;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220F68);

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
        func_00208DA0();
        btlFlagUnitDefeatCandidate(*(u32 *)(unit->stateFlags + 0x18));
        return 1;
    }
    return 0;
}

u32 func_00221128(ActionUnit *unit) {
    if (unit->action == 0x1a4) {
        func_001E98C0(unit);
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
        func_001E98C0(unit);
        return 1;
    }
    return 0;
}
u32 btlTickAction6B(u32 unit) {
    if (*(u32 *)(unit + 0x134) != 0x6b) {
        return 0;
    }
    /* The callee takes no arguments (see code_001DACF8.c), so retail
     * leaves $a0 holding the compared constant across these calls. */
    if (*(s32 *)(unit + 0x13c) >= 0) {
        if (*(s32 *)(unit + 0x13c) >= 0xF) {
            func_001E9890();
            func_001ECBF8(unit, unit);
        } else {
            func_001E98C0();
        }
        ++*(s32 *)(unit + 0x13c);
    } else {
        func_001E98C0();
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
        func_001E9890();
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
    if (*(u32 *)(unit + 0xc4) == 1 && *(u32 *)(unit + 0xc8) == 0x10b) {
        return 0;
    }
    return 1;
}

u32 func_00221858(u32 unit) {
    if (*(u32 *)(unit + 0xc4) == 1 && *(u32 *)(unit + 0xc8) == 0x10b) {
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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221988);

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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221EA0);

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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222028);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222100);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222298);

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
    *(f32 *)(unit + 0x50) += 650.0f;
    *(f32 *)(unit + 0xe0) += 650.0f;
    *(f32 *)(unit + 0x154) = 30.0f;
    *(u32 *)(unit + 0x110) |= 0x41;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222768);

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041ACA8);

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041ACC0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002228C0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222A08);

u32 func_00222D18(ActionUnit *unit) {
    u32 actor = unit->stateFlags;
    u32 owner = *(u32 *)(actor + 0x18);
    if (*(u32 *)(owner + 0x110) & 0x200) {
        if (func_001E8058(*(u32 *)(actor + 0x60)) == 1) {
            u32 target = func_001E8060(*(u32 *)(actor + 0x60), 0);
            if ((*(u32 *)(target + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_001E9A88();
            btlUnitSetCameraOffset((u32)unit);
            unit->pendingAction = 0;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222DA8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222E58);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222F18);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223280);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223350);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223BD8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223D10);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223DD8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00223ED0);

u32 func_00223FB0(ActionUnit *unit) {
    if (unit->action == 0x10b) {
        unit->flags = unit->flags | 0x800;
        return 1;
    }
    return 0;
}

u32 func_00223FE0(ActionUnit *unit) {
    if (unit->action == 0x10b) {
        func_001E98C0(unit);
        return 1;
    }
    return 0;
}
u32 func_00224010(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = 0xe0;
    if (arg1 != 0x12d) {
        temp_v0 = 0;
    }
    return temp_v0;
}

void func_00224020(u32 unit) {
    func_00217898(unit, unit + 0x30, unit + 0xc0, 0, 1,
                  0.8f, 1.5f, 0.25f);
    *(f32 *)(unit + 0x50) += 650.0f;
    *(f32 *)(unit + 0xe0) += 650.0f;
    *(u32 *)(unit + 0x110) |= 0x41;
    *(f32 *)(unit + 0x154) = 30.0f;
    func_001E88A8(unit + 0x30);
    func_001E88A8(unit + 0xc0);
}

void func_002240C0(u32 unit) {
    func_001E9660(unit, -851.6f, -144.4f, -2098.0f, -0.068f,
                    -0.141f, -0.004f, 0.979f, 40.0f);
}

u32 func_002240F8(ActionUnit *unit) {
    u32 actor = unit->stateFlags;
    u32 owner = *(u32 *)(actor + 0x18);
    if (*(u32 *)(owner + 0x110) & 0x200) {
        if (func_001E8058(*(u32 *)(actor + 0x60)) == 1) {
            u32 target = func_001E8060(*(u32 *)(actor + 0x60), 0);
            if ((*(u32 *)(target + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_001E9A88();
            func_00224020((u32)unit);
            unit->pendingAction = 0;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224188);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224238);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002242F8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224500);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224598);

s64 btlUnitWrapB(void) {
    return func_00224598();
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002247D0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224D28);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00224DF0);

void func_00224EE8(u32 unit) {
    func_00217898(unit, unit + 0x30, unit + 0xc0, 0, 1,
                  0.8f, 1.0f, 0.3f);
    *(f32 *)(unit + 0x50) += 750.0f;
    *(f32 *)(unit + 0xe0) += 750.0f;
    *(u32 *)(unit + 0x110) |= 0x41;
    *(f32 *)(unit + 0x154) = 30.0f;
    func_001E88A8(unit + 0x30);
    func_001E88A8(unit + 0xc0);
}

void func_00224F88(u32 unit) {
    func_001E9660(unit, 81.4f, -37.8f, -1866.2f, -0.112f,
                    0.01f, -0.017f, 0.982f, 40.0f);
}

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041B3B8);

INCLUDE_SDATA(const s32, "game/code_0021EE10", D_00436CF0);

