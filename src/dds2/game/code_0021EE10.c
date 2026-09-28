#include "common.h"

extern s32 func_001AA6F8(void);

extern void func_001E98C0();

extern u32 func_00220958(void);

extern u8 D_003BF6C0[][5];

extern u8 *D_00435E44;

extern u8 D_003BF950[];

extern u32 func_001EA190(u32);

extern void func_001E9F30(u32);

extern void func_00208DA0(void);

extern void func_001E21A0(u32);

extern u32 func_0021F808(void);

extern void func_001E9890(void);

extern void func_001EC868(u32, u32, f32);

extern void func_001ECBF8();

extern u32 func_001E8058(u32);

extern u32 func_001E8060(u32, u32);

extern void func_001E9A88(void);

extern void func_002226F0(u32);

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

extern void func_001E3448(u32, const u8 *);

extern void func_002218C8(void);

extern void func_00222450();

extern void func_00224598();

typedef struct ActionUnit {
    u8 pad0[0x110];
    u32 flags;
    u32 stateFlags;
    u8 pad118[8];
    u16 statusFlags;
    u8 pad122[2];
    u16 mode;
    u8 pad126[0xE];
    u32 action;
} ActionUnit;

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021EE10);

void func_0021EEF8(u32 unused, u32 actor) {
    u8 *state = *(u8 **)(func_001AA6F8() + 0x718);
    if (*(u32 *)(actor + 0x28) & 0x8000) {
        if (*(s8 *)(state + 1) != 0) {
            *state = 0;
        } else {
            *state = 1;
        }
    }
}

u32 func_0021EF48(u32 unit, u32 actor, u32 action) {
    u8 *state = *(u8 **)(func_001AA6F8() + 0x718);
    state[2] = action == 0x196;
    if ((*(u32 *)(unit + 0x110) & 0x200) &&
        (*(u32 *)(actor + 0x110) & 0x400) &&
        *(u16 *)(actor + 0x124) == 0x115) {
        return func_0021F808() ? 4 : 0;
    }
    return 0;
}

s32 func_0021EFD8(u32 unit) {
    if (unit == 0) {
        return func_0021F808() ? 0xf : -1;
    }
    if ((*(u32 *)(unit + 0x110) & 0x400) &&
        *(u16 *)(unit + 0x124) == 0x115 &&
        func_0021F808()) {
        return 0xf;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F040);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F0E8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F238);

s32 func_0021F378(u32 id) {
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

s32 func_0021F3B0(s32 actor) {
    if ((*(u32 *)(actor + 0x110) & 0x400) == 0) {
        return 0;
    }
    switch (*(u16 *)(actor + 0x124)) {
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

u32 func_0021F798(void) {
    u32 node = *(u32 *)(func_001AA6F8() + 0x24c);
    while (node != 0) {
        u32 flags = *(u32 *)(node + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                s32 action = *(u16 *)(node + 0x124);
                if (action < 0x12e) {
                    if (action >= 0x12a) {
                        return 1;
                    }
                }
            }
        }
        node = *(u32 *)(node + 0x364);
    }
    return 0;
}

u32 func_0021F808(void) {
    s32 battle = func_001AA6F8();
    if (*(u32 *)(battle + 0x2a0) != 0x30e) {
        return 0;
    }
    return *(s8 *)*(u32 *)(battle + 0x718) != 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_0021F848);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220368);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220450);

u32 btlSetBattleActionFlag(u32 unused1, u32 unused2, u32 action) {
    u8 *state = *(u8 **)(func_001AA6F8() + 0x718);
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

s32 func_002206A0(ActionUnit *actor, s32 action) {
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
    s32 battle = func_001AA6F8();
    u32 data;
    if (*(u32 *)(battle + 0x2a0) != 0x314) {
        return 0;
    }
    data = *(u32 *)(battle + 0x718);
    if (data != 0) {
        return *(u8 *)(data + 1);
    }
    return 0;
}

u32 func_00220958(void) {
    s32 battle = func_001AA6F8();
    u32 data;
    if (*(u32 *)(battle + 0x2a0) != 0x314) {
        return 0;
    }
    data = *(u32 *)(battle + 0x718);
    if (data != 0) {
        return *(u8 *)data;
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

u32 btlStartAction19A(u32 unit) {
    u32 action = *(u32 *)(unit + 0x134);
    if (action < 0x19C) {
        if (action >= 0x19A) {
            *(u32 *)(unit + 0x110) |= 0x800;
            func_001E9F30(unit);
            return 1;
        }
    }
    return 0;
}

u32 btlTickAction19A(u32 unit) {
    u32 action = *(u32 *)(unit + 0x134);
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

u32 func_00220D18(void) {
    s32 battle = func_001AA6F8();
    s32 unit = **(s32 **)(battle + 0x718);
    if (unit == 0) {
        return 0x10e;
    }
    return *(u16 *)(unit + 0x124);
}

void func_00220D48(u32 unit, u32 state) {
    if ((*(u32 *)(unit + 0x110) & 0x400) &&
        *(u16 *)(unit + 0x124) == 0x116 &&
        *(s32 *)state < 0) {
        u8 *battleState = *(u8 **)(func_001AA6F8() + 0x718);
        battleState[0] = 0;
        *(u16 *)(battleState + 2) = 0;
    }
}

u32 func_00220D98(u32 unused1, u32 unused2, u32 action) {
    s32 battle = func_001AA6F8();
    u8 *target = *(u8 **)(battle + 0x718);
    if (action == 0x1a5) {
        *target = 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220DD8);

f32 func_00220EE8(u32 unit, u32 other) {
    f32 factor = 1.0f;
    u8 *state;
    if ((*(u32 *)(unit + 0x110) & 0x200) == 0) {
        return factor;
    }
    if ((*(u32 *)(other + 0x110) & 0x400) == 0) {
        return factor;
    }
    if (*(u16 *)(other + 0x124) != 0x116) {
        return factor;
    }
    state = *(u8 **)(func_001AA6F8() + 0x718);
    if (state[0] != 0 && *(u16 *)(state + 2) < 4) {
        return *(f32 *)(D_00435E44 + 0xc00);
    }
    return 1.0f;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00220F68);

u32 func_00221060(u32 unit, u32 command) {
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return 0;
    }
    if (*(u16 *)(unit + 0x124) != 0x116) {
        return 0;
    }
    return command == 0x10;
}

u32 func_00221090(void) {
    s32 battle = func_001AA6F8();
    if (*(u32 *)(battle + 0x2a0) != 0x30f) {
        return 0;
    }
    return *(u8 *)*(u32 *)(battle + 0x718) != 0;
}

u32 func_002210D0(u32 unit) {
    if (*(u32 *)(unit + 0x134) == 0x1a4) {
        *(u32 *)(unit + 0x110) |= 0x800;
        func_001E9F30(unit);
        func_00208DA0();
        func_001E21A0(*(u32 *)(*(u32 *)(unit + 0x114) + 0x18));
        return 1;
    }
    return 0;
}

u32 func_00221128(u32 unit) {
    if (*(u32 *)(unit + 0x134) == 0x1a4) {
        func_001E98C0(unit);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221158);

u32 func_00221390(u32 unit) {
    if (*(u32 *)(unit + 0x134) == 0x6b) {
        if (func_001EA190(unit)) {
            *(s32 *)(unit + 0x13c) = 0;
        } else {
            *(s32 *)(unit + 0x13c) = -1;
        }
        return 1;
    }
    return 0;
}

u32 func_002213E0(u32 unit) {
    if (*(u32 *)(unit + 0x134) == 0x6b) {
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

u32 func_00221498(s32 arg0) {
    if (*(s32 *)(arg0 + 0x134) == 0x6c) {
        *(u32 *)(arg0 + 0x13c) = 0;
        return 0;
    }
    return 0;
}

u32 func_002214C0(u32 unit) {
    if (*(u32 *)(unit + 0x134) != 0x6c) {
        return 0;
    }
    if (func_001EA190(unit) && *(u32 *)(unit + 0x13c) == 0x25) {
        func_001E9890();
        func_001EC868(unit, unit, 0.0f);
    }
    ++*(u32 *)(unit + 0x13c);
    return 1;
}

void func_00221538(void) {
    s32 battle = func_001AA6F8();
    s32 unit = *(s32 *)(battle + 0x718);
    *(f32 *)(unit + 4) = 1.0f;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221568);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221760);

void func_002217E8(void) {
    s32 *piVar1;
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    piVar1 = *(s32 **)(temp_v0 + 0x718);
    temp_v0 = *piVar1;
    if (temp_v0 != 0) {
        func_001E7D30(temp_v0);
        *piVar1 = 0;
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

void func_00221888(s32 actor) {
    if ((*(u32 *)(actor + 0x110) & 0x400) == 0) {
        return;
    }
    switch (*(u16 *)(actor + 0x124)) {
    case 0x11D:
    case 0x11E:
    case 0x11F:
    case 0x120:
    case 0x121:
        *(u32 *)(actor + 0x114) |= 0x20000;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002218C8);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221988);

f32 func_00221A30(u32 unit, u32 actor, u32 action, u32 mode) {
    f32 factor = 1.0f;
    if (action == 0x19f && mode == 1 && (*(u32 *)(unit + 0x110) & 0x400)) {
        u32 battle = func_001AA6F8();
        factor = *(f32 *)(*(u32 *)(battle + 0x718) + 4);
    }
    return factor;
}

s32 func_00221A80(void) {
    u32 node = *(u32 *)(func_001AA6F8() + 0x24c);
    s32 selected = -1;
    while (node != 0 && selected == -1) {
        u32 flags = *(u32 *)(node + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                switch (*(u16 *)(node + 0x124)) {
                case 0x11d: selected = 0; break;
                case 0x11e: selected = 1; break;
                case 0x11f: selected = 2; break;
                case 0x120: selected = 3; break;
                case 0x121: selected = 4; break;
                }
            }
        }
        node = *(u32 *)(node + 0x364);
    }
    return selected;
}

u32 func_00221B28(u32 unit) {
    u32 value = 0;
    switch (*(u16 *)(unit + 0x124)) {
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

u32 func_00221B88(u32 unit, u32 group) {
    u32 action = func_00221B28(unit);
    return D_003BF6C0[group][action];
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00221BC8);

void func_00221E28(u32 unit, u32 group, f32 opacity) {
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        func_0023CE10(*(u32 *)(unit + 0x340), group);
        func_0023CE18(*(u32 *)(unit + 0x340), opacity);
    } else {
        group = func_00221B88(unit, group);
        func_0023CE10(*(u32 *)(unit + 0x340), group);
        func_0023CE18(*(u32 *)(unit + 0x340), opacity);
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

void func_00222330(void) {
    u32 node = *(u32 *)(func_001AA6F8() + 0x24c);
    while (node != 0) {
        u32 flags = *(u32 *)(node + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                s32 action = *(u16 *)(node + 0x124);
                if (action < 0x122) {
                    if (action >= 0x11d) {
                        func_001E3448(node, D_003BF950);
                        *(u32 *)(node + 0x110) &= ~0x80000;
                    }
                }
            }
        }
        node = *(u32 *)(node + 0x364);
    }
    func_002218C8();
}

u32 func_002223D8(u32 unit, u32 base) {
    u32 flags = *(u32 *)(unit + 0x110);
    if ((flags & 1) == 0) {
        return base;
    }
    if ((flags & 0x400) == 0) {
        return base;
    }
    switch (*(u16 *)(unit + 0x124)) {
    case 0x11d: return base;
    case 0x11e: return base + 100;
    case 0x11f: return base + 200;
    case 0x120: return base + 300;
    case 0x121: return base + 400;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222450);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002226D0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002226F0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222768);

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041ACA8);

INCLUDE_RODATA(const s32, "game/code_0021EE10", D_0041ACC0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002228C0);

INCLUDE_ASM(const s32, "game/code_0021EE10", func_00222A08);

u32 func_00222D18(u32 unit) {
    u32 actor = *(u32 *)(unit + 0x114);
    u32 owner = *(u32 *)(actor + 0x18);
    if (*(u32 *)(owner + 0x110) & 0x200) {
        if (func_001E8058(*(u32 *)(actor + 0x60)) == 1) {
            u32 target = func_001E8060(*(u32 *)(actor + 0x60), 0);
            if ((*(u32 *)(target + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_001E9A88();
            func_002226F0(unit);
            *(u32 *)(unit + 0x130) = 0;
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

u32 func_00223FB0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x134) == 0x10b) {
        *(u32 *)(arg0 + 0x110) = *(u32 *)(arg0 + 0x110) | 0x800;
        return 1;
    }
    return 0;
}

u32 func_00223FE0(u32 unit) {
    if (*(u32 *)(unit + 0x134) == 0x10b) {
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

u32 func_002240F8(u32 unit) {
    u32 actor = *(u32 *)(unit + 0x114);
    u32 owner = *(u32 *)(actor + 0x18);
    if (*(u32 *)(owner + 0x110) & 0x200) {
        if (func_001E8058(*(u32 *)(actor + 0x60)) == 1) {
            u32 target = func_001E8060(*(u32 *)(actor + 0x60), 0);
            if ((*(u32 *)(target + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_001E9A88();
            func_00224020(unit);
            *(u32 *)(unit + 0x130) = 0;
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

INCLUDE_ASM(const s32, "game/code_0021EE10", func_002247B0);

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

