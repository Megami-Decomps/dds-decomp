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

typedef struct BattleActionUnit {
    u8 pad0[0x110];
    u32 flags;
} BattleActionUnit;

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00224FC0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002251A0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00225368);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002254C8);

void func_00225778(u32 unit) {
    func_002254C8(unit);
}

u32 func_00225798(u32 unit) {
    u32 actor = *(u32 *)(unit + 0x114);
    u32 owner = *(u32 *)(actor + 0x18);
    if (*(u32 *)(owner + 0x110) & 0x200) {
        if (func_001E8058(*(u32 *)(actor + 0x60)) == 1) {
            u32 target = func_001E8060(*(u32 *)(actor + 0x60), 0);
            if ((*(u32 *)(target + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_00208D58();
            func_00224EE8(unit);
            *(u32 *)(unit + 0x130) = 0;
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
    u32 battle;
    if (action != 0x109) {
        return 0;
    }
    battle = func_001AA6F8();
    *(u32 *)(battle + 0x21c) |= 0x20000;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002263D8);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002264E8);

s32 func_00226540(u32 unused1, u32 unused2, s32 action) {
    return action == 0x109 ? 0x1194 : 0x64;
}

void func_00226558(u8 value) {
    s32 battle;

    battle = func_001AA6F8();
    if (*(s32 *)(battle + 0x2a0) == 0x31b) {
        **(u8 **)(battle + 0x718) = value;
    }
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226598);

u32 func_00226670(void) {
    return 0xffffffff;
}

s32 btlFilterRestrictedCommand(s32 battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((*(u16 *)(battler + 0x120) & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

u8 func_002266A8(u32 arg0, s32 arg1) {
    return arg1 == 0xf;
}

s32 btlSelectDisabledCommand(s32 battler) {
    if (battler == 0) {
        return 15;
    }
    return (*(u16 *)(battler + 0x120) & 0x2000) ? 15 : -1;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002266D8);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_002267A0);

void func_00226820(u32 unit) {
    if (*(u32 *)(unit + 8) & 8) {
        func_001AA6F8();
    }
}

u32 func_00226850(s32 arg0) {
    if (*(s32 *)(arg0 + 0x134) == 0x187) {
        *(u32 *)(arg0 + 0x13c) = 0;
    }
    return 0;
}

u32 func_00226868(u32 unit) {
    if (*(u32 *)(unit + 0x134) != 0x187) {
        return 0;
    }
    if (btlHasMarkedEntry14(unit)) {
        if (*(s32 *)(unit + 0x13c) >= 0x34) {
            func_001E9890();
            func_001E9660(unit, 517.3f, -476.0f, -947.2f, 0.177f,
                           0.283f, 0.042f, 0.933f, 40.0f);
        }
        ++*(s32 *)(unit + 0x13c);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226900);

void func_002269E0(void) {
    u32 node = *(u32 *)(func_001AA6F8() + 0x24c);
    while (node != 0) {
        u32 flags = *(u32 *)(node + 0x110);
        if (flags & 1) {
            if ((flags & 0x400) && *(u16 *)(node + 0x124) == 0x118) {
                *(u16 *)(node + 0x120) &= ~0x2000;
                func_001E2758(node);
            }
        }
        node = *(u32 *)(node + 0x364);
    }
}

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226A60);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226AB0);

INCLUDE_ASM(const s32, "game/code_00224FC0", func_00226BB8);

void btlResetEffectState(void) {
    BattleEffectState *state = *(BattleEffectState **)(func_001AA6F8() + 0x718);
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

