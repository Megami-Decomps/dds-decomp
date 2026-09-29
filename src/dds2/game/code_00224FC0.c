#include "common.h"

/* 5/10 words match. With this TU's default -O2 the call in the final
 * conditional is sibling-call-optimized to j, while retail has jal+epilogue.
 * Do not change per-file flags to force it. */

extern s32 func_001AA6F8(void);

extern u32 btlHasMarkedEntry14(u32);

extern void func_001E9890(void);

extern u32 func_001E8058(u32);

extern u32 func_001E8060(u32, u32);

extern void func_00208D58(void);

extern void func_00224EE8(u32);

extern void func_001E9660(u32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void func_001E2758(void *);

typedef struct BattleEffectState {
    u32 actor, flags, value;
    u16 timer;
    u8 active, phase;
    u32 effect;
    f32 speed;
} BattleEffectState;

typedef struct BattleActionUnit BattleActionUnit;

typedef struct BattleActor {
    u8 pad00[0x18];
    BattleActionUnit *owner;
    u8 pad1C[0x44];
    u32 actionEntity;
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
    u8 pad250[0x50];
    s32 battleId;
    u8 pad2A4[0x474];
    BattleEffectState *effect;
} BattleActionContext;

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00224FC0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002251A0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225368);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002254C8);

void func_00225778(u32 unit) {
    func_002254C8(unit);
}

u32 func_00225798(BattleActionUnit *unit) {
    BattleActor *actor = unit->actor;
    BattleActionUnit *owner = actor->owner;
    if (owner->flags & 0x200) {
        if (func_001E8058(actor->actionEntity) == 1) {
            BattleActionUnit *target = (BattleActionUnit *)func_001E8060(actor->actionEntity, 0);
            if ((target->flags & 0x400) == 0) {
                return 0;
            }
            func_00208D58();
            func_00224EE8((u32)unit);
            unit->transitionState = 0;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225828);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002258D8);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002259A0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225B48);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225BF8);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002260E0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002261A8);

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

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226308);

u32 func_00226398(u32 unit, u32 actor, u32 action) {
    BattleActionContext *battle;
    if (action != 0x109) {
        return 0;
    }
    battle = (BattleActionContext *)func_001AA6F8();
    battle->flags |= 0x20000;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002263D8);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002264E8);

s32 func_00226540(u32 unused1, u32 unused2, s32 action) {
    return action == 0x109 ? 0x1194 : 0x64;
}

void func_00226558(u8 value) {
    BattleActionContext *battle;

    battle = (BattleActionContext *)func_001AA6F8();
    if (battle->battleId == 0x31b) {
        *(u8 *)battle->effect = value;
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

u8 func_002266A8(u32 arg0, s32 arg1) {
    return arg1 == 0xf;
}

s32 btlSelectDisabledCommand(BattleActionUnit *battler) {
    if (battler == 0) {
        return 15;
    }
    return (battler->entryFlags & 0x2000) ? 15 : -1;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002266D8);

s32 func_002267A0(void) {
    u8 *battle = (u8 *)func_001AA6F8();
    u8 *unit;
    if (*(u16 *)(battle + 0x270) != 2) {
        return -1;
    }
    unit = *(u8 **)(battle + 0x24C);
    while (unit != 0) {
        if (*(u32 *)(unit + 0x110) & 1) {
            if (*(u16 *)(unit + 0x124) == 0x118) {
                u16 status = *(u16 *)(unit + 0x120);
                if (status & 0x2000) {
                    *(u16 *)(unit + 0x120) = status & ~0x2000;
                }
            }
        }
        unit = *(u8 **)(unit + 0x364);
    }
    return -1;
}

void func_00226820(BattleActionUnit *unit) {
    if (unit->dispatchFlags & 8) {
        func_001AA6F8();
    }
}

u32 func_00226850(BattleActionUnit *unit) {
    if (unit->type == 0x187) {
        unit->frameCounter = 0;
    }
    return 0;
}

u32 func_00226868(BattleActionUnit *unit) {
    if (unit->type != 0x187) {
        return 0;
    }
    if (btlHasMarkedEntry14((u32)unit)) {
        if (unit->frameCounter >= 0x34) {
            func_001E9890();
            func_001E9660((u32)unit, 517.3f, -476.0f, -947.2f, 0.177f,
                           0.283f, 0.042f, 0.933f, 40.0f);
        }
        ++unit->frameCounter;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226900);

void func_002269E0(void) {
    BattleActionUnit *node = ((BattleActionContext *)func_001AA6F8())->firstUnit;
    while (node != 0) {
        u32 flags = node->flags;
        if (flags & 1) {
            if ((flags & 0x400) && node->kind == 0x118) {
                node->entryFlags &= ~0x2000;
                func_001E2758(node);
            }
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226A60);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226AB0);

void func_00226BB8(void) {
    u8 *effect = *(u8 **)((u8 *)func_001AA6F8() + 0x718);
    u8 *actor = *(u8 **)effect;
    if (actor != 0) {
        u32 state = *(u32 *)(actor + 0x114);
        u32 flags = *(u32 *)(actor + 0x110);
        state &= ~0x80;
        state &= ~0x100;
        flags |= 0x100;
        *(u8 **)effect = 0;
        *(u32 *)(actor + 0x110) = flags;
        *(u32 *)(actor + 0x114) = state;
        func_001E2758(actor);
        *(u32 *)(actor + 0x110) |= 8;
        *(f32 *)(effect + 0x10) = -125.0f;
        *(f32 *)(effect + 0x14) = 20.0f;
    }
}

void btlResetEffectState(void) {
    BattleEffectState *state = ((BattleActionContext *)func_001AA6F8())->effect;
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

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226E98);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226F58);

INCLUDE_RODATA(const s32, "game/code_00224FC0", D_0041B4F8);

