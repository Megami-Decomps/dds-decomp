#include "common.h"

extern u64 func_00219318(void);

extern s32 func_00211DE0(void);

extern s32 func_001AA6F8(void);

extern s32 func_001B2CC0(void);

extern s32 func_00212CB8(u32, u32, u32);

extern s32 func_001B33C8(void);

extern s32 func_001ABB10(void);

extern u32 func_001AC360(u64, u64, u64);

extern s8 D_00436CAC;

extern void func_002112C8(s32, u16);

extern void func_002152D8(s32, s32);

extern void func_00215C70(s32, s32);

extern void func_00215F28(s32, s32);

extern void func_00216760(s32, s32);

extern void func_00216888(s32, s32);

extern void func_002160A0();

extern void func_00216CA0();

extern void func_00216988();

extern s32 *D_00436CB8;

extern s32 func_00212B38();

extern u32 func_002192D8(void);

extern void func_00216E98(s32, s32);

extern s32 mdlFlagTest(s32);

extern s32 D_00435DD0;

extern s32 func_001AA700(void *);

extern s32 func_001AA740(void *);

extern s32 func_001AA708(void *);

extern s32 func_001AA758(void *);

extern void *allocateBattleIndexList(s32);

extern u32 func_001E8058();

extern s32 func_001B2D38();

extern void func_001AC0F8(s32, void *, s32, s32, s32);

extern s32 func_001E8060(void *, u32);

extern void func_001E8018(void *);

extern s32 func_00213A58(void *, s32);

extern s8 D_00419C30[];

extern s8 D_00419C58[];

extern s32 func_001B24F8(s32, s32);

extern u8 D_00438B66;

extern s32 func_00214098();

extern s32 func_00213F58(s32, s32, s32);

extern s32 func_002147B0(s32, s32, s32);

extern s32 func_001B2900(void *, s32);

extern s32 func_00213950(s32, s32);

extern s32 func_001B2900(void *, s32);

extern s32 func_001AE8C0(void *, s32);

extern s32 func_001AEAB8(void *, s32);

extern s32 func_001AEB20(void *, s32);

extern s32 func_00214C18(void *, s32);

extern s32 func_001B2F50(void *, s32);

extern s8 D_00438F84;

extern s32 sdfNamedChunkFindId(void *, void *);

extern void func_0021A1D8(s32, s32);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002112C8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211360);

u32 func_002115B0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002115B8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211658);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002119E0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211D20);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211DE0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211EA8);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419A88);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00211F38);

s32 func_002122D8(u8 *unit, s32 multiplier) {
    u8 *stats = unit + 0x120;
    s32 current = func_001AA700(stats);
    s32 maximum = func_001AA740(stats);
    if ((u32)(maximum * multiplier) < (u32)(current * 100)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212340);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002123C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212470);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212520);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212550);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002125B0);

u8 func_00212620(void) {
    s64 temp_v0;

    temp_v0 = func_001B2CC0();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212640);

s32 func_002126C8(s32 battler) {
    if (*(u32 *)(battler + 0x114) & 0x10000) {
        return 0;
    }
    if (*(u32 *)(battler + 0x110) & 0x200) {
        if (*(s32 *)(D_00435DD0 + 0x3c) < 2) {
            return 0;
        }
    }
    return mdlFlagTest(0x290) != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212728);

s32 func_00212788(s32 arg0, s32 arg1) {
    return (func_001AA840(arg0 + 0x120, arg1) & arg1) != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002127B8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212838);

s32 func_00212948(s32 unused, s32 mask) {
    s32 battler = *(s32 *)(func_001AA6F8() + 0x24c);
    for (; battler != 0; battler = *(s32 *)(battler + 0x364)) {
        if ((*(u64 *)(battler + 0x110) & 0x201) == 0x201 &&
            func_00212788(battler, mask)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002129C8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212A40);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212AB0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212B38);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212CB8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212E60);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212F20);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00212FA0);

u8 func_00213020(u32 arg0, u32 arg1) {
    s64 temp_v0;

    temp_v0 = func_00212CB8(arg0, arg1, 0);
    return temp_v0 != 0;
}

u8 func_00213040(u32 arg0, u32 arg1) {
    s64 temp_v0;

    temp_v0 = func_00212CB8(arg0, arg1, 1);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213060);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002130E8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213170);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002131F8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213280);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213308);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213390);

u8 func_002133F8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00212B38(arg0, 10);
    return temp_v0 != 0;
}

s32 func_00213418(void) {
    return func_00212B38() != 0;
}

s32 func_00213438(s32 battler) {
    u32 flags;

    if (*(u32 *)(battler + 0x110) & 0x200) {
        return 0;
    }
    flags = *(u16 *)(battler + 0x120) & 0x2000;
    return flags != 0;
}

s32 func_00213460(void) {
    return ((*(s32 *)(*(s32 *)D_00436CB8 + 0xc) & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213478);

s32 battleCounterReachedLimit(s32 unused, u32 limit) {
    extern u32 func_001B39E8(s32);
    if (func_001B39E8(4) < limit) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002134C8);

INCLUDE_ASM(const s32, "game/code_002112C8", battleTurnReachedLimit);

s32 func_00213548(s32 unused, u32 limit) {
    if (limit < *(u32 *)(func_001AA6F8() + 0x274)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", battleIsReadyWithoutTurns);

s32 func_002135B8(u8 *unit, s32 count) {
    void *flags = unit + 0x120;
    u32 amount = func_001AA708(flags);
    u32 total = func_001AA758(flags) * count;
    if (total < amount * 100) {
        return 0;
    }
    return 1;
}

s32 func_00213620(s32 battler, u32 limit) {
    s32 status = battler + 0x120;
    u32 value = func_001AA708(status);
    u32 maximum = func_001AA758(status);
    if (value * 100 < maximum * limit) {
        return 0;
    }
    return 1;
}

s32 func_00213688(s32 battler, u32 limit) {
    s32 status = battler + 0x120;
    u32 value = func_001AA708(status);
    func_001AA758(status);
    if (limit < value) {
        return 0;
    }
    return 1;
}

s32 func_002136D8(s32 battler, u32 limit) {
    s32 status = battler + 0x120;
    u32 value = func_001AA708(status);
    func_001AA758(status);
    if (value < limit) {
        return 0;
    }
    return 1;
}

u8 func_00213728(void) {
    s64 temp_v0;

    temp_v0 = func_001B33C8();
    return temp_v0 != 0;
}

s32 func_00213748(u8 *unit) {
    u32 i;
    u32 count;
    s32 battle;
    void *list;
    if (*(u32 *)(unit + 0x110) & 0x400) {
        return 0;
    }
    battle = *(s32 *)D_00436CB8;
    list = allocateBattleIndexList(13);
    func_001AC0F8(battle, list, 2, 0, 0);
    count = func_001E8058(list);
    for (i = 0; i < count; i++) {
        if (func_001B2D38(func_001E8060(list, i)) != 0) {
            func_001E8018(list);
            return 1;
        }
    }
    func_001E8018(list);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213818);

s32 func_00213900(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) & 0x1000) > 0);
}

u8 func_00213910(void) {
    s64 temp_v0;

    temp_v0 = func_001ABB10();
    return temp_v0 == 0;
}

u8 func_00213930(void) {
    s64 temp_v0;

    temp_v0 = func_001B2D38();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213950);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213A58);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213B38);

s32 func_00213BB8(s32 unused, s32 mask) {
    s32 battler = *(s32 *)(func_001AA6F8() + 0x24c);
    for (; battler != 0; battler = *(s32 *)(battler + 0x364)) {
        if ((*(u64 *)(battler + 0x110) & 0x201) == 0x201 &&
            func_00213A58(battler, mask)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213C38);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213CA8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213D38);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213DA0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213E18);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213E80);

s32 func_00213EF8(s32 actor) {
    if (func_001B24F8(actor, 0) != 0) {
        if (D_00438B66 == 0) {
            func_0020D128(D_00419C30);
        }
        return 1;
    }
    if (D_00438B66 == 0) {
        func_0020D128(D_00419C58);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00213F58);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214098);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214168);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214188);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002141A8);

s32 func_00214230(s32 unused, s32 battler) {
    s32 node = *(s32 *)(func_001AA6F8() + 0x248);
    for (; node != 0; node = *(s32 *)(node + 0x178)) {
        s32 target = *(s32 *)(node + 0x18);
        if (target != 0 &&
            (*(u64 *)(target + 0x110) & 0x221) == 0x201 &&
            func_00213F58(battler, *(s16 *)(node + 0x150), 0)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002142C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214330);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002143A0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214470);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214510);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214578);

s32 func_002145E0(void) {
    return D_00436CAC < 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002145F0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214658);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002146C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002146E0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214700);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002147B0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214928);

s32 func_00214948(u8 *unit, s32 action, u32 mask) {
    u32 flags = *(u32 *)(unit + 0x110);
    if (flags & 1) {
        if (flags & mask) {
            if (!(flags & 0x20)) {
                if (action & 0x100000) {
                    s32 i;
                    for (i = 0; i < 19; i++) {
                        s32 index = func_00213950(action, i);
                        if (index == 0x80) {
                            continue;
                        }
                        if (func_001B2900(unit, index) != 0 ||
                            func_001AE8C0(unit, index) != 0 ||
                            func_001AEAB8(unit, index) != 0 ||
                            func_001AEB20(unit, index) != 0) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (func_001B2900(unit, action) != 0 ||
                    func_001AE8C0(unit, action) != 0 ||
                    func_001AEAB8(unit, action) != 0) {
                    return 0;
                }
                return func_001AEB20(unit, action) == 0;
            }
        }
    }
    return 2;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214AA0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214B00);

s32 func_00214B60(void *unit) {
    if (func_00214C18(unit, 0x1b2) != 0 ||
        func_00214C18(unit, 0x1b6) != 0 ||
        func_00214C18(unit, 0x1ba) != 0 ||
        func_00214C18(unit, 0x1be) != 0) {
        return 1;
    }
    return func_00214C18(unit, 0x1c2) != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", battleActionMatchesUnit);

s32 func_00214C18(void *unit, s32 action) {
    func_001AA6F8();
    if ((*(u64 *)((u8 *)unit + 0x110) & 0x421) == 0x401) {
        if (func_001B2F50(unit, action) != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214C78);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00214DF8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00215118);

u64 func_00215268(u64 arg0, u32 *arg1, u32 *arg2) {
    u32 temp_v0;
    u64 temp_v1;

    temp_v1 = allocateBattleIndexList(0xd);
    temp_v0 = func_001AC360(arg0, temp_v1, 0);
    *arg1 = temp_v0;
    temp_v0 = func_001E8058(temp_v1);
    *arg2 = temp_v0;
    return temp_v1;
}

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419B38);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419B68);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419B88);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419BB8);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_00419BE0);

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

INCLUDE_ASM(const s32, "game/code_002112C8", func_00215F28);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002160A0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002161B0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002162C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002163C8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216760);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216888);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216988);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216B40);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216CA0);

u32 func_00216D10(u32 arg0, u32 arg1) {
    func_00217170(arg0, arg1, 1);
    return 1;
}

u32 func_00216D30(u32 arg0, u32 arg1) {
    func_00217170(arg0, arg1, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216D50);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216E98);

u32 func_00216ED0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_00219318();
    func_001E8030(*(u32 *)(arg0 + 0x60), temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00216F08);

u32 func_00217020(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217028);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217170);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002172B8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217378);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217470);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217650);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217898);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217B20);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217DC8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00217EB8);

u32 func_00217FE8(void) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = 0;
    if (**(s8 **)(temp_v0 + 0x718) == '\0') {
        temp_v1 = 100;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218018);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002180F8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218150);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002181E8);

void func_00218250(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    **(u32 **)(temp_v0 + 0x718) = 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218278);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218320);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218418);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218520);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218630);

s32 func_00218690(void) {
    return *(u32 *)(func_001AA6F8() + 0x2a0) == 0x303 ? 1 : 2;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002186C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218798);

void func_00218968(void) {
    func_0011AEE0(1);
}

s32 func_00218980(s32 battler, s32 action, s32 defaultValue) {
    if (action == 11) {
        if (*(u32 *)(battler + 0x110) & 0x400) {
            if (*(u16 *)(battler + 0x124) == 0x107) {
                return 1;
            }
        }
    }
    return defaultValue;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002189B8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218A78);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218AF0);

void func_00218B78(void) {
    s32 state = func_001AA6F8();
    s32 resource = *(s32 *)(state + 0x718);
    *(s32 *)resource = 0;
    *(s32 *)(resource + 4) = 0;
    *(u8 *)(resource + 8) = 0;
    func_0011AEE0(6);
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218BA8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218D00);

s32 func_00218D88(s32 record) {
    if ((*(u32 *)(record + 8) & 8) == 0) {
        return -1;
    }
    return **(u32 **)(func_001AA6F8() + 0x718) == *(u32 *)(record + 0x18) ? 12 : -1;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218DE0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218E98);

s32 func_00218EE8(s32 battler, s32 action) {
    if ((*(u32 *)(battler + 0x110) & 0x400) == 0) {
        return 0;
    }
    if (*(u16 *)(battler + 0x124) != 0x108) {
        return 0;
    }
    return action == 15;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218F18);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00218FD0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002190A0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219170);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219210);

void func_00219278(void) {
    func_00219210();
}

s32 func_00219290(void) {
    s32 context = func_001AA6F8();
    u32 *value;
    if (*(u32 *)(context + 0x2a0) != 0x306) {
        return 0;
    }
    value = *(u32 **)(context + 0x718);
    if (value == 0) {
        return 0;
    }
    return *value != 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002192D8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219318);

void func_002193B8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    **(u32 **)(temp_v0 + 0x718) = 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002193E0);

s32 func_00219488(s32 unused, s32 ignored, s32 action) {
    s32 record = *(s32 *)(func_001AA6F8() + 0x718);
    switch (action) {
    case 0x129:
        *(u8 *)(record + 0xc) = 1;
        break;
    case 0x12a:
        *(u8 *)(record + 0xc) = 0;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_002194E8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002195E0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219760);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219848);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002198D8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219950);

INCLUDE_ASM(const s32, "game/code_002112C8", func_002199C8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219A70);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219B48);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219BD0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219CC8);

INCLUDE_RODATA(const s32, "game/code_002112C8", D_0041A378);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219D40);

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219E38);

s32 func_00219F28(s32 battler, s32 action) {
    if ((*(u32 *)(battler + 0x110) & 0x400) == 0) {
        return 0;
    }
    if (*(u16 *)(battler + 0x124) != 0x136) {
        return 0;
    }
    return action == 19;
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_00219F58);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A098);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A1D8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A308);

void func_0021A3A0(s32 arg0) {
    s32 temp_v0;

    *(u32 *)(arg0 + 0x1c) = 0x80808080;
    temp_v0 = *(s32 *)(arg0 + 0xc);
    *(u16 *)(arg0 + 0x14) = *(u16 *)(arg0 + 0x14) & 0xfffd;
    if (temp_v0 != 0) {
        do {
            func_0021A3A0(temp_v0);
            temp_v0 = *(s32 *)(temp_v0 + 4);
        } while (temp_v0 != *(s32 *)(arg0 + 0xc));
    }
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A408);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A490);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A778);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A8F8);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021A978);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B070);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B168);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B250);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B310);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B368);

void func_0021B4A8(void) {
    func_0021B368();
}

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B4C0);

INCLUDE_ASM(const s32, "game/code_002112C8", func_0021B520);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CC0);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CC8);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CD0);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CD8);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CE0);

INCLUDE_SDATA(const s32, "game/code_002112C8", D_00436CE8);

