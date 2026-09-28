#include "common.h"

extern s32 *battleFindGroupedEntity();

extern u8 D_003BBB0D;

extern s32 battleFindModelEntry();

extern s32 countBattleTasksByKind(u32);

extern u32 func_00207BF0(void);

extern u32 func_001A3360(u64, u64, u64);

extern void *allocateBattleIndexList(s32);

extern u32 func_001DAE48();

extern s32 func_001A8818();

extern s32 func_001A2B00(void);

extern s32 func_001A8EA8(void);

extern s32 func_00200628();

extern s32 func_002007A8(u32, u32, u32);

extern s32 func_001A87A0(void);

extern s8 D_003BB870;

extern s32 *D_003BB87C;

extern s8 D_003D7588[];

typedef struct BattleRuntimeState {
    u32 flags;
    u16 state;
    u8 unk_06;
    u8 pending;
    s8 active;
    u8 unk_09[3];
    u32 options;
    u8 unk_10[0x28];
    void *ownedData;
    u8 unk_3C[4];
    void *resource;
    void *request;
    void *handle;
} BattleRuntimeState;

extern BattleRuntimeState D_003D7580;

extern char D_003BB8A0[];

extern char D_003BB898[];

extern char D_003BB8A8[];

extern s32 D_003BAA60;

extern s32 createSemaphore(s32, s32, s32);

extern u32 D_003BD878;

extern s32 D_00367940[];

extern s32 D_00367960[];

extern s8 D_003A5A80[];

extern s8 D_003A5AA8[];

extern void func_003014F0();

extern u32 func_0020A3F0(void);

extern u32 func_00207BB0(void);

extern u32 func_00207B28(void);

extern u32 func_002099A0(void);

extern u32 func_00208C68(void);

extern s32 func_001A17F0(void);

typedef struct BtlUnit {
    u8 unk_00[0x110];
    u32 flags;
    u8 unk_114[0xC];
    u16 unk_120;
    u8 unk_122[2];
    u16 unk_124;
    u8 unk_126[0x21E];
    struct BtlUnit *next;
} BtlUnit;

typedef struct BtlState {
    u8 unk_000[0x6A4];
    s32 unk_6A4;
    s32 unk_6A8;
    u8 unk_6AC[8];
    s32 unk_6B4;
    s32 unk_6B8;
    u8 unk_6BC[8];
    s32 unk_6C4;
    u8 unk_6C8[0x10];
    s32 unk_6D8;
    u8 unk_6DC[8];
    s32 unk_6E4;
    s32 unk_6E8;
    u8 unk_6EC[8];
    s32 unk_6F4;
    u8 unk_6F8[0x10];
    s32 unk_708;
    s32 table0[0x20];
    s32 table1[0x180];
    s32 table2[0x20];
    s8 unk_E0C;
    u8 unk_E0D;
    s16 unk_E0E;
} BtlState;

/* W14_HDR_END */

typedef struct SoundResourceNode {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 resourceHandle;
    u32 unk_14;
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

extern void func_0020B348();

extern void func_0020DC38();

extern s32 func_00201A40();

extern void func_001A8CE0(s32);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF030);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF0C8);

u32 func_001FF558(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF560);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF8D8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFC18);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFCD8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFDA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5988);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFE30);

extern s32 func_001A17F8(void *);
extern s32 func_001A1838(void *);

s32 func_00200120(u8 *unit, s32 multiplier) {
    u8 *stats = unit + 0x120;
    s32 current = func_001A17F8(stats);
    s32 maximum = func_001A1838(stats);
    if ((u32)(maximum * multiplier) < (u32)(current * 100)) {
        return 0;
    }
    return 1;
}

s32 func_00200188(s32 unused, s32 multiplier) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x401) == 0x401) {
            if (func_00200120(unit, multiplier) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

u32 func_00200208(void) {
    u32 temp_v0;

    temp_v0 = func_00200FB0();
    if (temp_v0 == 0) {
        return temp_v0;
    }
    *(u16 *)(*(s32 *)D_003BB87C + 0x88) = 0;
    return 1;
}

s32 func_00200238(s32 unused, u32 limit) {
    u32 count = 0;
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            count++;
        }
        unit = *(u8 **)(unit + 0x344);
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

s32 func_00200298(s32 unused, u32 limit) {
    u32 count = 0;
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if ((*(u16 *)(unit + 0x12e) & 0x800) == 0) {
                count++;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

u8 func_00200308(void) {
    s32 temp_v0;

    temp_v0 = func_001A87A0();
    return temp_v0 != 0;
}

s32 func_00200328(s32 unused, u32 limit) {
    u32 count = 0;
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            count++;
        }
        unit = *(u8 **)(unit + 0x344);
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

s32 func_00200388(s32 arg0, s32 arg1) {
    return (func_001A1938(arg0 + 0x120, arg1) & arg1) != 0;
}

s32 func_002003B8(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x401) == 0x401) {
            if (func_00200388((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200438(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_00200388((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_002004B8(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_00200388((s32)unit, action) == 0) {
                return 0;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 1;
}

s32 func_00200530(s32 unused, s32 kind) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (*(u16 *)(unit + 0x124) == kind) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_002005A0(u8 *actor, s32 kind) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (*(u16 *)(unit + 0x124) == kind) {
                if (*(u64 *)(unit + 0x108) != *(u64 *)(actor + 0x108)) {
                    return 1;
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200628);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002007A8);

extern s32 func_001A8448(void *, s32);
extern const s32 D_003A5A08[10];

s32 func_00200948(void *actor) {
    s32 actions[10];
    s32 i;
    memcpy(actions, D_003A5A08, sizeof(actions));
    for (i = 0; i < 10; i++) {
        if (func_001A8448(actor, actions[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 func_00200A08(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_00200628((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200A88(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (func_00200628((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

u8 func_00200B08(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002007A8(arg0, arg1, 0);
    return temp_v0 != 0;
}

u8 func_00200B28(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002007A8(arg0, arg1, 1);
    return temp_v0 != 0;
}

s32 func_00200B48(s32 unused, u32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_002007A8((u32)unit, action, 0) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200BD0(s32 unused, u32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_002007A8((u32)unit, action, 1) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200C58(s32 unused, u32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (func_002007A8((u32)unit, action, 0) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200CE0(s32 unused, u32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (func_002007A8((u32)unit, action, 1) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200D68(s32 unused, u32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_002007A8((u32)unit, action, 0) == 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200DF0(s32 unused, u32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (func_002007A8((u32)unit, action, 0) == 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00200E78(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if ((*(u32 *)(unit + 0x110) & 0x1000) == 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

u8 func_00200EE0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_00200628(arg0, 10);
    return temp_v0 != 0;
}

s32 func_00200F00(void) {
    return func_00200628() != 0;
}

s32 func_00200F20(s32 battler) {
    u32 flags;

    if (*(u32 *)(battler + 0x110) & 0x200) {
        return 0;
    }
    flags = *(u16 *)(battler + 0x120) & 0x2000;
    return flags != 0;
}

s32 func_00200F48(void) {
    return ((*(s32 *)(*(s32 *)D_003BB87C + 0xc) & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200F60);

s32 battleCounterReachedLimit(s32 unused, u32 limit) {
    extern u32 func_001A9488(s32);
    if (func_001A9488(4) < limit) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200FB0);

s32 battleTurnReachedLimit(s32 unused, u32 limit) {
    s32 *battle = (s32 *)func_001A17F0();
    if ((u32)battle[0x250 / 4] < limit) {
        return 0;
    }
    return 1;
}

s32 battleIsReadyWithoutTurns(void) {
    u8 *battle = (u8 *)func_001A17F0();
    if (*(u16 *)(battle + 0x248) == 2) {
        if (*(s32 *)(battle + 0x250) == 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_001A1800(void *);
extern s32 func_001A1850(void *);

s32 func_00201070(u8 *unit, s32 count) {
    void *flags = unit + 0x120;
    u32 amount = func_001A1800(flags);
    u32 total = func_001A1850(flags) * count;
    if (total < amount * 100) {
        return 0;
    }
    return 1;
}

u8 func_002010D8(void) {
    s32 temp_v0;

    temp_v0 = func_001A8EA8();
    return temp_v0 != 0;
}

extern void func_001A30F8(s32, void *, s32, s32, s32);
extern s32 func_001DAE50(void *, u32);
extern void func_001DAE08(void *);

s32 func_002010F8(u8 *unit) {
    u32 i;
    u32 count;
    s32 battle;
    void *list;
    if (*(u32 *)(unit + 0x110) & 0x400) {
        return 0;
    }
    battle = *(s32 *)D_003BB87C;
    list = allocateBattleIndexList(13);
    func_001A30F8(battle, list, 2, 0, 0);
    count = func_001DAE48(list);
    for (i = 0; i < count; i++) {
        if (func_001A8818(func_001DAE50(list, i)) != 0) {
            func_001DAE08(list);
            return 1;
        }
    }
    func_001DAE08(list);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002011C8);

s32 func_002012B0(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) & 0x1000) > 0);
}

u8 func_002012C0(void) {
    s32 temp_v0;

    temp_v0 = func_001A2B00();
    return temp_v0 == 0;
}

u8 func_002012E0(void) {
    s32 temp_v0;

    temp_v0 = func_001A8818();
    return temp_v0 != 0;
}

extern const s32 D_003A5A30[20];

s32 func_00201300(s32 mask, s32 index) {
    s32 table[20];
    memcpy(table, D_003A5A30, sizeof(table));
    if ((mask & table[index]) == 0) {
        return 0x80;
    }
    if (index != 0) {
        return index - 1;
    }
    return -1;
}


INCLUDE_ASM(const s32, "game/code_001FF030", func_00201408);

extern s32 func_00201408(void *, s32);

s32 func_002014E8(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x401) == 0x401) {
            if (func_00201408(unit, action) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00201568(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x201) == 0x201) {
            if (func_00201408(unit, action) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_002015E8(s32 unused, s32 kind) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (*(u16 *)(unit + 0x124) != kind) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00201658(u8 *actor, s32 kind) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (*(u16 *)(unit + 0x124) != kind) {
                if (*(u64 *)(unit + 0x108) != *(u64 *)(actor + 0x108)) {
                    return 1;
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_002016E0(s32 unused, s32 action) {
    s32 unit = *(s32 *)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if (func_00202178(unit, action, 0x200) == 0) {
            return 1;
        }
        unit = *(s32 *)(unit + 0x344);
    }
    return 0;
}

s32 func_00201748(s32 unused, s32 action) {
    s32 unit = *(s32 *)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if (func_00202178(unit, action, 0x200) == 1) {
            return 0;
        }
        unit = *(s32 *)(unit + 0x344);
    }
    return 1;
}

s32 func_002017C0(s32 unused, s32 action) {
    s32 unit = *(s32 *)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if (func_00202178(unit, action, 0x400) == 0) {
            return 1;
        }
        unit = *(s32 *)(unit + 0x344);
    }
    return 0;
}

s32 func_00201828(s32 unused, s32 action) {
    s32 unit = *(s32 *)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if (func_00202178(unit, action, 0x400) == 1) {
            return 0;
        }
        unit = *(s32 *)(unit + 0x344);
    }
    return 1;
}

extern s32 func_001A8050(s32, s32);
extern u8 D_003BD476;

s32 func_002018A0(s32 actor) {
    if (func_001A8050(actor, 0) != 0) {
        if (D_003BD476 == 0) {
            func_001FB0A8(D_003A5A80);
        }
        return 1;
    }
    if (D_003BD476 == 0) {
        func_001FB0A8(D_003A5AA8);
    }
    return 0;
}

extern s32 func_00201900(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201900);


s32 func_00201A40(mask, action)
u32 mask;
s32 action;
{
    u8 *node = *(u8 **)(func_001A17F0() + 0x224);
    while (node != 0) {
        u8 *owner = *(u8 **)(node + 0x18);
        if (owner != 0) {
            u32 flags = *(u32 *)(owner + 0x110);
            if ((flags & 1) && (flags & mask) && !(flags & 0x20)) {
                s32 i;
                for (i = 0; i < 8; i++) {
                    if (func_00201900(action, *(s16 *)(node + 0x148 + i * 4), 1) != 0) {
                        return 1;
                    }
                }
            }
        }
        node = *(u8 **)(node + 0x16c);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201B10);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201B30);


s32 func_00201B50(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (func_00201900(action, *(s16 *)(unit + 0x2b0), 0) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00201BD8(s32 unused, s32 action) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_00201900(action, *(s16 *)(unit + 0x2b0), 0) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00201C60(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (func_00200948(unit) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00201CD0(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x421) == 0x401) {
            if (func_00200948(unit) != 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00201D40(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if ((*(u32 *)(unit + 0x110) & 0x1000) != 0) {
                return 0;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 1;
}

s32 func_00201DA8(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if ((*(u32 *)(unit + 0x110) & 0x1000) == 0) {
                return 0;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 1;
}

s32 func_00201E10(void) {
    return D_003BB870 < 1;
}

extern s32 func_00201FE0(s32, s32, s32);

s32 func_00201E20(s32 unused, s32 action) {
    s32 unit = *(s32 *)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if (func_00201FE0(unit, action, 0x200) != 0) {
            return 1;
        }
        unit = *(s32 *)(unit + 0x344);
    }
    return 0;
}

s32 func_00201E88(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    while (unit != 0) {
        if ((*(u64 *)(unit + 0x110) & 0x221) == 0x201) {
            if (*(u16 *)(unit + 0x12a) == 0) {
                return 1;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201EF0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201F10);

s32 func_00201F30(s32 unused, s32 query, u32 mask) {
    u8 *node = *(u8 **)(func_001A17F0() + 0x224);
    while (node != 0) {
        u8 *owner = *(u8 **)(node + 0x18);
        if (owner != 0) {
            u32 flags = *(u32 *)(owner + 0x110);
            if ((flags & 1) && (flags & mask) && !(flags & 0x20)) {
                s32 i;
                for (i = 0; i < 8; i++) {
                    if (*(s32 *)(node + 0x148 + i * 4) == query) {
                        return 1;
                    }
                }
            }
        }
        node = *(u8 **)(node + 0x16c);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201FE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202158);

extern s32 func_001A8448(void *, s32);
extern s32 func_001A53D8(void *, s32);
extern s32 func_001A5578(void *, s32);
extern s32 func_001A55A8(void *, s32);

s32 func_00202178(u8 *unit, s32 action, u32 mask) {
    u32 flags = *(u32 *)(unit + 0x110);
    if (flags & 1) {
        if (flags & mask) {
            if (!(flags & 0x20)) {
                if (action & 0x100000) {
                    s32 i;
                    for (i = 0; i < 19; i++) {
                        s32 index = func_00201300(action, i);
                        if (index == 0x80) {
                            continue;
                        }
                        if (func_001A8448(unit, index) != 0 ||
                            func_001A53D8(unit, index) != 0 ||
                            func_001A5578(unit, index) != 0 ||
                            func_001A55A8(unit, index) != 0) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (func_001A8448(unit, action) != 0 ||
                    func_001A53D8(unit, action) != 0 ||
                    func_001A5578(unit, action) != 0) {
                    return 0;
                }
                return func_001A55A8(unit, action) == 0;
            }
        }
    }
    return 2;
}

s32 func_002022D0(s32 unused, s32 action) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    while (node != 0) {
        if (func_00202178(node, action, 0x200) == 0) {
            return 0;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 1;
}

s32 func_00202330(s32 unused, s32 action) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    while (node != 0) {
        if (func_00202178(node, action, 0x400) == 0) {
            return 0;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 1;
}

extern s32 func_00202448(void *, s32);

s32 func_00202390(void *unit) {
    if (func_00202448(unit, 0x1b2) != 0 ||
        func_00202448(unit, 0x1b6) != 0 ||
        func_00202448(unit, 0x1ba) != 0 ||
        func_00202448(unit, 0x1be) != 0) {
        return 1;
    }
    return func_00202448(unit, 0x1c2) != 0;
}

s32 battleActionMatchesUnit(u8 *unit, s32 action) {
    s32 *battle = (s32 *)*D_003BB87C;
    if (battle[0x148 / 4] == action) {
        if (*(u32 *)(unit + 0x114) & 0x1000) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_001A8A30(void *, s32);

s32 func_00202448(void *unit, s32 action) {
    func_001A17F0();
    if ((*(u64 *)((u8 *)unit + 0x110) & 0x421) == 0x401) {
        if (func_001A8A30(unit, action) != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002024A8);

u64 func_002025F8(u64 arg0, u32 *arg1, u32 *arg2) {
    u32 temp_v0;
    u64 temp_v1;

    temp_v1 = allocateBattleIndexList(0xd);
    temp_v0 = func_001A3360(arg0, temp_v1, 0);
    *arg1 = temp_v0;
    temp_v0 = func_001DAE48(temp_v1);
    *arg2 = temp_v0;
    return temp_v1;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5AA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5BD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202668);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202F90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203098);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203248);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002033C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002034D0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002035E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002036E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203A80);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203BA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203CA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203E38);

s32 func_00203F98(s32 actor) {
    void *list = allocateBattleIndexList(13);
    func_001A30F8(actor, list, 1, 1, 0);
    func_001DAE48(list);
    func_001DAE20(*(s32 *)(actor + 0x60), *(s32 *)(actor + 0x18));
    func_001DAE08(list);
    return 1;
}

u32 func_00204008(u32 arg0, u32 arg1) {
    func_002042E8(arg0, arg1, 1);
    return 1;
}

u32 func_00204028(u32 arg0, u32 arg1) {
    func_002042E8(arg0, arg1, 0);
    return 1;
}

u32 func_00204048(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00207BF0();
    func_001DAE20(*(u32 *)(arg0 + 0x60), temp_v0);
    return 1;
}


INCLUDE_ASM(const s32, "game/code_001FF030", func_00204080);

u32 func_00204198(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002041A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002042E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204430);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002044F0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002045E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204838);

u32 func_00204AC0(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204AC8);

u32 func_00204B40(void) {
    return 0xffffffff;
}

s32 filterRestrictedBattleCommand(s32 battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((*(u16 *)(battler + 0x120) & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

u8 func_00204B78(u32 arg0, s32 arg1) {
    return arg1 == 0xf;
}

s32 battleSelectDisabledCommand(s32 battler) {
    if (battler == 0) {
        return 15;
    }
    return (*(u16 *)(battler + 0x120) & 0x2000) ? 15 : -1;
}

void func_00204BA8(u8 *unit, u8 *command) {
    u8 *battle;
    u8 *effect;
    s32 species;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return;
    }
    battle = (u8 *)func_001A17F0();
    species = *(u16 *)(unit + 0x124);
    effect = *(u8 **)(battle + 0x694);
    if (species < 0x105) {
        if (species >= 0x102 && (*(u16 *)(command + 0x26) & 0x10)) {
            *(u16 *)(unit + 0x120) |= 0x2000;
            *(s32 *)effect = *(s32 *)(battle + 0x250);
        }
    }
    if (species == 0x102 && (*(u16 *)(command + 0x26) & 0x100)) {
        if (*(s32 *)command < 0 || *(s32 *)(command + 4) < 0 || *(s32 *)(command + 8) != 0) {
            if (*(s32 *)effect != *(s32 *)(battle + 0x250)) {
                *(u16 *)(unit + 0x120) &= ~0x2000;
            }
        }
    }
}

s32 func_00204C88(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit;
    if (*(u16 *)(battle + 0x24c) != 2) {
        return -1;
    }
    unit = *(u8 **)(battle + 0x228);
    while (unit != 0) {
        if (*(u32 *)(unit + 0x110) & 1) {
            if (*(u16 *)(unit + 0x124) == 0x104) {
                u16 status = *(u16 *)(unit + 0x120);
                if (status & 0x2000) {
                    *(u16 *)(unit + 0x120) = status & ~0x2000;
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204D08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204D98);

void battleResetUnitPlacement(void) {
    u8 *unit = *(u8 **)((u8 *)func_001A17F0() + 0x228);

    if (unit == 0) {
        return;
    }
    do {
        if ((*(u32 *)(unit + 0x110) & 1) != 0) {
            s32 species = *(u16 *)(unit + 0x124);
            if (species < 0x105) {
                if (species >= 0x102) {
                    if (*(u16 *)(unit + 0x120) & 0x2000) {
                        *(s32 *)(unit + 0x90) = 0;
                        *(f32 *)(unit + 0x94) = -100.0f;
                        *(f32 *)(unit + 0x98) = 60.0f;
                        *(s32 *)(unit + 0x9c) = 0;
                        *(f32 *)(unit + 0xb4) = 180.0f;
                        *(f32 *)(unit + 0xb0) = 220.0f;
                    } else {
                        func_001D4CA8(unit, 1, species);
                    }
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    } while (unit != 0);
}

extern void func_001D5990(void *);

void func_00204FE0(void) {
    u8 *unit = *(u8 **)((u8 *)func_001A17F0() + 0x228);
    while (unit != 0) {
        u32 flags = *(u32 *)(unit + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                s32 species = *(u16 *)(unit + 0x124);
                if (species >= 0x105) {
                    unit = *(u8 **)(unit + 0x344);
                    continue;
                }
                if (species >= 0x102) {
                    *(u16 *)(unit + 0x120) &= ~0x2000;
                    func_001D5990(unit);
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205070);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002051B0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002052F0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205420);

void func_00205520(u8 *unit) {
    u8 *battle = (u8 *)func_001A17F0();
    u32 flags = *(u32 *)(unit + 0x110);
    if (flags & 0x200) {
        if (*(u16 *)(unit + 0x124) == 1) {
            *(u32 *)(unit + 0xe0) = 1;
            *(u32 *)(unit + 0x110) = flags | 0x1000;
            *(u16 *)(unit + 0x120) |= 0x1000;
            if (*(s32 *)(battle + 0x27c) == 0x107) {
                *(u32 *)(unit + 0x114) |= 0x200;
            }
        }
    } else {
        *(u32 *)(unit + 0x110) = flags | 0x1000;
        *(u32 *)(unit + 0xe0) = 0x132;
    }
    *(u32 *)(unit + 0x114) |= 0x20;
}

void func_002055B8(s32 arg0) {
    u16 temp_v0;

    if (*(s32 *)(arg0 + 0x110) & 0x200) {
        temp_v0 = *(u16 *)(arg0 + 0x124);
        if (temp_v0 == 1) {
            if (*(s32 *)(arg0 + 0xe0) == temp_v0) {
                *(s32 *)(arg0 + 0xe0) = 0x11;
            }
        }
    }
}

u32 func_002055F0(void) {
    return 2;
}

extern s32 func_001A8018(void *);
extern void mdlFlagSet(u32);
extern void mdlFlagClear(u32);

s32 func_002055F8(void) {
    u8 *unit = *(u8 **)((u8 *)func_001A17F0() + 0x228);
    while (unit != 0) {
        if (*(u32 *)(unit + 0x110) & 1) {
            if (func_001A8018(unit) != 0) {
                if (*(u32 *)(unit + 0x110) & 0x200) {
                    mdlFlagSet(0x811);
                } else {
                    mdlFlagClear(0x811);
                }
                return 6;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return -1;
}

s32 battleInitializeResources(s32 unused, s32 resource) {
    extern u8 D_00360EE0[];
    extern u8 D_00360EF0[];
    extern void func_001DC2A8(s32, u8 *, u8 *);
    func_001DC2A8(resource, D_00360EE0, D_00360EF0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002056C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002056E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205730);

void func_00205838(void) {
    u8 *effect = *(u8 **)((u8 *)func_001A17F0() + 0x694);
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
        func_001D5990(actor);
        *(u32 *)(actor + 0x110) |= 8;
        *(f32 *)(effect + 0x10) = -125.0f;
        *(f32 *)(effect + 0x14) = 20.0f;
    }
}

typedef struct BattleEffectState {
    u32 actor, flags, value;
    u16 timer;
    u8 active, phase;
    u32 effect;
    f32 speed;
} BattleEffectState;

void battleResetEffectState(void) {
    u8 *battle = (u8 *)func_001A17F0();
    BattleEffectState *data = *(BattleEffectState **)(battle + 0x694);
    data->active = 1;
    data->speed = 20.0f;
    data->flags = 0;
    data->phase = 0;
    data->value = 0;
    data->timer = 0;
    data->effect = 0;
    data->actor = 0;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5D50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205918);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205B18);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205BD8);

void func_00205EE0(void) {
    func_00205BD8();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205EF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206128);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206180);

s32 func_002062B8(BtlUnit *unit, s32 arg1) {
    u32 id;
    if (!(unit->flags & 1)) {
        return arg1;
    }
    if (!(unit->flags & 0x400)) {
        return arg1;
    }
    if ((*(BattleEffectState **)(func_001A17F0() + 0x694))->active != 1) {
        return arg1;
    }
    id = unit->unk_124;
    if (id == 0x124) {
        return arg1;
    }
    switch (arg1) {
    case 1:
        if (id == 0x107) {
            return 1;
        }
        if (id == 0x108) {
            return 0xC8;
        }
        break;
    case 2:
        if (id == 0x107) {
            return 0x66;
        }
        if (id == 0x108) {
            return 0xC8;
        }
        break;
    default:
        if (id == 0x107) {
            return arg1 + 0x64;
        }
        if (id == 0x108) {
            return arg1 + 0xC8;
        }
        break;
    }
    return arg1;
}


u8 *func_00206388(s32 category, s32 species) {
    u8 *unit;
    if (category != 1) {
        return 0;
    }
    if (species != 0x124) {
        return 0;
    }
    unit = *(u8 **)((u8 *)func_001A17F0() + 0x228);
    while (unit != 0) {
        u32 flags = *(u32 *)(unit + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 2) {
                    if (*(s32 *)(unit + 0xc8) == 0x124) {
                        return unit;
                    }
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return 0;
}

s32 func_00206418(s32 arg0) {
    s32 temp_v0 = *(u16 *)(arg0 + 0x124);

    if ((temp_v0 >= 0x107) && ((temp_v0 < 0x109) || (temp_v0 == 0x124))) {
        return 0x124;
    }
    return *(s32 *)(arg0 + 0xe0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206450);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206608);

s32 func_00206888(u8 *task) {
    BattleEffectState *effect;
    if ((*(u32 *)(task + 8) & 8) == 0) {
        return -1;
    }
    effect = *(BattleEffectState **)((u8 *)func_001A17F0() + 0x694);
    return effect->actor == *(u32 *)(task + 0x18) ? 12 : -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002068E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002069C0);

s32 func_00206F38(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(func_001A17F0() + 0x694));
    if (temp_v0 == 0) {
        return 0;
    }
    return (temp_v0 ^ arg0) == 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206F78);

s32 battleHasDifferentActiveTarget(u32 target) {
    if (func_00207B68() == 0) {
        return 1;
    }
    return func_00207BF0() != target;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207070);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002071B0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002072F0);

extern void *func_001D8DE8(void *, s32, s32);
extern void func_001D4860(void *);

s32 func_00207640(u8 *unit) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *effect;
    u8 *other;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return 1;
    }
    effect = *(u8 **)(battle + 0x694);
    if (effect[0xe] != 0) {
        return 1;
    }
    if (*(s32 *)(unit + 0xc8) == 0x124) {
        return 1;
    }
    other = *(u8 **)(battle + 0x228);
    while (other != 0) {
        u32 flags = *(u32 *)(other + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (other != unit) {
                    if (flags & 0xe0) {
                        return 1;
                    }
                }
            }
        }
        other = *(u8 **)(other + 0x344);
    }
    func_001D4860(func_001D8DE8(unit, 8, 10));
    *(u32 *)(unit + 0x110) &= ~0x100;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207718);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207948);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207B00);

u32 func_00207B28(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x108) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u8 *)(temp_v1 + 0xe);
}

s32 func_00207B68(void) {
    s32 temp_v0 = 0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_001A17F0();
    if (*(s32 *)(temp_v1 + 0x27c) != 0x108) {
        return temp_v0;
    }
    temp_v2 = *(s32 *)(temp_v1 + 0x694);
    if (temp_v2 == 0) {
        return temp_v0;
    }
    return (*(s32 *)(temp_v2 + 0) != 0);
}

u32 func_00207BB0(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x108) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u32 *)(temp_v1 + 0x8);
}

u32 func_00207BF0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(*(s32 *)(temp_v0 + 0x694));
}

s32 func_00207C18(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit = *(u8 **)(battle + 0x228);
    u8 *effect = *(u8 **)(battle + 0x694);
    while (unit != 0) {
        if ((*(u32 *)(unit + 0x110) & 0x400) &&
            *(u16 *)(unit + 0x124) == 0x107) {
            break;
        }
        unit = *(u8 **)(unit + 0x344);
    }
    if (unit == 0) {
        return 1;
    }
    if (!(*(u32 *)(unit + 0x110) & 0xe0)) {
        return 0;
    }
    return *(u8 **)(effect + 4) == unit;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207CA0);

extern s8 D_003BD86C;
extern s32 sdfNamedChunkFindId(void *, void *);
extern void func_00207CA0(s32, s32);

s32 func_00207DD0(void *query) {
    u8 *effect = *(u8 **)(func_001A17F0() + 0x694);
    u8 *unit = *(u8 **)effect;
    u8 *model;
    u8 *descriptor;
    s32 index;
    s32 selected;
    if (unit == 0) {
        return 1;
    }
    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return 1;
    }
    model = *(u8 **)(*(u8 **)(unit + 0x320) + 0x8c);
    descriptor = *(u8 **)(model + 0x18);
    index = sdfNamedChunkFindId(descriptor, query);
    if (index == -1) {
        return 1;
    }
    selected = *(s32 *)(*(u8 **)(*(u8 **)descriptor + 0xc) + index * 4);
    D_003BD86C = 1;
    func_00207CA0(selected, *(s32 *)(descriptor + 0x1c));
    return D_003BD86C;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207E68);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207FF0);

s32 func_00208248(u8 *unit, s32 animation) {
    s32 species;
    u32 flags = *(u32 *)(unit + 0x110);
    if ((flags & 1) == 0) {
        return animation;
    }
    if ((flags & 0x400) == 0) {
        return animation;
    }
    species = *(u16 *)(unit + 0x124);
    switch (species) {
    case 0x12e:
        return animation + 100;
    case 0x10a:
        return animation + 300;
    case 0x12f:
        return animation + 200;
    default:
        return animation;
    }
}

void battleMarkSpecialUnit(u8 *unit) {
    s32 species;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return;
    }
    species = *(u16 *)(unit + 0x124);
    if (species != 0x10a) {
        if (species < 0x10a) {
            return;
        }
        if (species >= 0x130) {
            return;
        }
        if (species < 0x12e) {
            return;
        }
    }
    *(u32 *)(unit + 0x114) |= 0x200;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002082E8);

extern u64 func_001A0CB0(void);

u64 func_00208358(u64 owner) {
    u8 **slot = *(u8 ***)(func_001A17F0() + 0x694);
    u8 *model = *slot;
    u8 *entry;
    if (model != 0) {
        return func_001A0CB0();
    }
    model = (u8 *)func_001DA948();
    *slot = model;
    func_001A1990(model + 0x120, 0x10a);
    func_00207E68();
    entry = (u8 *)func_001D7F90(*slot, 1, 0x10a, 0);
    if (owner != 0) {
        *(u64 *)(entry + 8) = owner;
        *entry = 4;
    }
    func_001D4860(entry);
    return *(u64 *)(entry + 0x38);
}

void func_00208400(void) {
    s32 *data;
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    data = *(s32 **)(temp_v0 + 0x694);
    temp_v0 = *data;
    if (temp_v0 != 0) {
        func_001DAB20(temp_v0);
        *data = 0;
    }
}

u64 func_00208440(u64 owner) {
    u8 *data = *(u8 **)(func_001A17F0() + 0x694);
    u8 *entry = (u8 *)func_001D9038(*(void **)data, 12);
    if (owner != 0) {
        *(u64 *)(entry + 8) = owner;
        *entry = 4;
    }
    *(u64 *)(entry + 0x40) = 0x8000000000000003ULL;
    func_001D4860(entry);
    return *(u64 *)(entry + 0x38);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002084B0);

s32 func_002085E8(void) {
    u8 *unit = *(u8 **)(func_001A17F0() + 0x228);
    s32 count = 0;
    while (unit != 0) {
        u32 flags = *(u32 *)(unit + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if ((flags & 0xe0) == 0) {
                    count++;
                } else {
                    *(u32 *)(unit + 0x110) = ((flags & ~1) | 0x80) & ~0x40;
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return count == 0;
}

u32 func_00208660(void) {
    return 0xffffffff;
}

s32 func_00208668(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) & 0x200) < 1);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208678);

s32 func_002087E0(u8 *unit, s32 index) {
    u32 flags = *(u32 *)(unit + 0x110);
    if ((flags & 0x400) == 0) {
        return index;
    }
    if (*(u16 *)(unit + 0x124) != 0x10d) {
        return index;
    }
    if ((flags & 1) == 0) {
        return -1;
    }
    {
        u8 *data = *(u8 **)(func_001A17F0() + 0x694);
        if (index >= 17) {
            return index;
        }
        if (index < 15) {
            return index;
        }
        return *(u16 *)(data + 4) != 0 ? 16 : 15;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208860);

void func_002089F0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x124) != 0x10d) {
        return;
    }
    func_001A17F0();
    func_001D4CA8(arg0, 1, 0x11d);
    func_00208860(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208A50);

void func_00208C20(void) {
    func_001F53F0();
}

u32 func_00208C38(u32 arg0, u32 arg1, s32 arg2) {
    if ((0x171 < arg2) && ((arg2 < 0x175 || (arg2 == 0x1a1)))) {
        return 200;
    }
    return 100;
}

u32 func_00208C68(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x116) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u16 *)(temp_v1 + 0x4);
}

extern void func_001D4CA8();
extern void func_001F53F0(void);

void func_00208CA8(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit = *(u8 **)(battle + 0x228);
    while (unit != 0) {
        if (*(u32 *)(unit + 0x110) & 0x400) {
            if (*(u16 *)(unit + 0x124) == 0x10d) {
                break;
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    if (unit != 0) {
        *(s32 *)(battle + 0x5b8) = 0;
        func_001D4CA8(unit, 1, 0x11d);
        func_001F53F0();
    }
}

extern u32 effMiscRand(char *);
extern char D_003A5D98[];

extern char D_00324550[];

void func_00208D30(void) {
    u8 *data = *(u8 **)(func_001A17F0() + 0x694);
    *(u32 *)(data + 4) = 0x80808080;
    *(u32 *)(data + 8) = 0;
    *(u32 *)(data + 0xc) = effMiscRand(D_00324550) % 6;
    func_001FB0A8(D_003A5D98, *(u32 *)(data + 0xc));
}

s32 func_00208D98(u8 *unit, s32 command) {
    u32 flags = *(u32 *)(unit + 0x110);
    if (flags & 1) {
        if (flags & 0x400) {
            u8 *effect = *(u8 **)((u8 *)func_001A17F0() + 0x694);
            if (*(u8 *)(unit + 0x11c) == *(u32 *)(effect + 0xc)) {
                if (*(u32 *)(effect + 8) & 4) {
                    return command;
                }
            }
            return -1;
        }
    }
    return command;
}

void func_00208E10(unit)
u8 *unit;
{
    u8 *battleData;
    u8 *entry;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return;
    }
    battleData = *(u8 **)(func_001A17F0() + 0x694);
    *(u32 *)(unit + 0x114) |= 0x200;
    *(f32 *)(unit + 0xb0) = 100.0f;
    *(f32 *)(unit + 0xb4) = 25.0f;
    *(f32 *)(unit + 0xb8) = 100.0f;
    *(f32 *)(unit + 0xbc) = 25.0f;
    if (*(s32 *)(battleData + 0xc) != *(u8 *)(unit + 0x11c)) {
        entry = (u8 *)func_001D0958(unit);
        if (entry != 0) {
            *(u16 *)(entry + 4) = 0;
            *(u16 *)(unit + 0x124) = 0x10f;
        }
    }
}

void func_00208EA8(void) {
    func_00208E10();
}

void *battleFindActiveMember(s32 group, s32 type) {
    s32 *unit;
    if (group != 1) {
        return 0;
    }
    if (type != 0x10e) {
        return 0;
    }
    unit = *(s32 **)(*(s32 *)(func_001A17F0() + 0x694));
    if (unit == 0) {
        return 0;
    }
    return (unit[0x110 / 4] & 2) ? unit : 0;
}

u64 func_00208F10(u64 owner) {
    u8 **slot = *(u8 ***)(func_001A17F0() + 0x694);
    u8 *model = *slot;
    u8 *entry;
    if (model != 0) {
        return func_001A0CB0();
    }
    model = (u8 *)func_001DA948();
    *slot = model;
    func_001A1990(model + 0x120, 0x10e);
    entry = (u8 *)func_001D7F90(*slot, 1, 0x10e, 0);
    if (owner != 0) {
        *(u64 *)(entry + 8) = owner;
        *entry = 4;
    }
    func_001D4860(entry);
    return *(u64 *)(entry + 0x38);
}

void func_00208FB0(void) {
    s32 *data;
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    data = *(s32 **)(temp_v0 + 0x694);
    temp_v0 = *data;
    if (temp_v0 != 0) {
        func_001DAB20(temp_v0);
        *data = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208FF0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209140);

u32 func_00209220(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = 4;
    if ((*(u32 *)(arg1 + 0x110) & 0x400) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209238);

s32 func_00209400(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x250) == 0) {
        return -1;
    }
    if ((*(s32 *)(temp_v0 + 0x1f4) & 0x800) != 0) {
        return -1;
    }
    if (*(u16 *)(temp_v0 + 0x24c) != 1) {
        return -1;
    }
    return battleFindScriptResource(D_003BB898);
}

s32 func_00209460(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit = *(u8 **)(battle + 0x228);
    u8 *effect = *(u8 **)(battle + 0x694);
    while (unit != 0) {
        u32 flags = *(u32 *)(unit + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (*(s32 *)(effect + 0xc) == unit[0x11c]) {
                    break;
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    return (*(u32 *)(unit + 0x110) & 0x20) ? 1 : -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209528);

extern void func_001DF358(void *, void *);

s32 func_002096E8(u8 *entry) {
    u8 *unit = *(u8 **)(entry + 0xf4);
    u8 *data;
    u32 kind;
    if (unit == 0) {
        return 1;
    }
    if ((*(u32 *)(*(u8 **)(unit + 0x18) + 0x110) & 0x400) == 0) {
        return 1;
    }
    data = *(u8 **)((u8 *)func_001A17F0() + 0x694);
    if (*(u32 *)(data + 8) & 4) {
        return 1;
    }
    kind = *(u32 *)(entry + 0x104);
    if (kind < 9) {
        if (kind >= 4) {
            func_001DF358(entry, entry);
            *(u32 *)(entry + 0xf0) |= 0x1000;
            return 0;
        }
    }
    return 1;
}

s32 battleCheckAttachedMember(u8 *entry) {
    u8 *unit = *(u8 **)(entry + 0xf4);
    if (unit == 0) {
        return 1;
    }
    if ((*(u32 *)(*(u8 **)(unit + 0x18) + 0x110) & 0x400) == 0) {
        return 1;
    }
    {
        s32 *data = *(s32 **)(func_001A17F0() + 0x694);
        s32 flags = data[2] & 4;
        if (flags) {
            return 1;
        }
        return 0;
    }
}

s32 func_002097D0(u8 *unit) {
    s32 *data;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return -1;
    }
    data = *(s32 **)(func_001A17F0() + 0x694);
    return (data[2] & 4) ? -1 : 0;
}

s32 func_00209818(u8 *unit, s32 command, u8 mode) {
    u8 *effect;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return command;
    }
    effect = *(u8 **)((u8 *)func_001A17F0() + 0x694);
    if (*(u8 *)(unit + 0x11c) != *(u32 *)(effect + 0xc)) {
        return -1;
    }
    if (command == 13) {
        return -1;
    }
    if (command == 10) {
        return 0;
    }
    if (command == 2) {
        return 0;
    }
    if (command == 11) {
        return mode == 1 ? 14 : command;
    }
    return command;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002098B8);

u32 func_002099A0(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x10b) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u32 *)(temp_v1 + 0xc);
}

s32 func_002099E0(void) {
    s32 temp_v0 = 0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_001A17F0();
    if (*(s32 *)(temp_v1 + 0x27c) != 0x10b) {
        return temp_v0;
    }
    temp_v2 = *(s32 *)(temp_v1 + 0x694);
    if (temp_v2 == 0) {
        return temp_v0;
    }
    return ((*(s32 *)(temp_v2 + 0x8) & 2) > 0);
}

void func_00209A28(void) {
    u8 *data;
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    data = *(u8 **)(temp_v0 + 0x694);
    data[1] = 1;
    *data = 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209A58);

s32 func_00209B70(void) {
    s32 temp_v0 = -1;
    s32 temp_v1;

    temp_v1 = *(s32 *)(func_001A17F0() + 0x694);
    if (*(s8 *)(temp_v1 + 0) != 0) {
        if (*(s8 *)(temp_v1 + 1) != 0) {
            *(u8 *)(temp_v1 + 1) = 0;
            return battleFindScriptResource(D_003BB8A0);
        }
        *(u8 *)(temp_v1 + 0) = 0;
        return -1;
    }
    return temp_v0;
}

void func_00209BC8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    **(u16 **)(temp_v0 + 0x694) = 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5D98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5DB8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209C90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209EB8);

u32 func_0020A3F0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u16 *)(*(s32 *)(temp_v0 + 0x694));
}

s32 func_0020A418(void) {
    u8 *unit = *(u8 **)((u8 *)func_001A17F0() + 0x228);
    u16 species = 0;
    while (unit != 0) {
        u32 flags = *(u32 *)(unit + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                species = *(u16 *)(unit + 0x124);
                if ((u16)(species - 0x119) < 2) {
                    break;
                }
            }
        }
        unit = *(u8 **)(unit + 0x344);
    }
    if (unit == 0) {
        return 0;
    }
    return species == 0x119;
}

void func_0020A4C0(u8 *unit, s32 action) {
    u8 *data;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return;
    }
    if ((u16)(*(u16 *)(unit + 0x124) - 0x119) >= 2) {
        return;
    }
    data = *(u8 **)(func_001A17F0() + 0x694);
    if (*(u16 *)(unit + 0x124) == 0x11a && *(u16 *)data >= 3) {
        return;
    }
    func_001AA868(unit, action);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A560);

s32 battleMapCommandToSkill(u32 command) {
    switch (command) {
    case 0: return 0x142;
    case 1: return 0x142;
    case 2: return 0x13d;
    case 3: return 0x13e;
    case 4: return 0x13f;
    case 5: return 0x140;
    case 6: return 0x141;
    default: return -1;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A780);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A860);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AB08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ABE0);

s32 func_0020AD20(s32 skill) {
    switch (skill) {
    case 0x13d: return 0xb9;
    case 0x13e: return 0xbb;
    case 0x13f: return 0xbd;
    case 0x140: return 0xbf;
    case 0x141: return 0xc1;
    case 0x142: return 0xc3;
    default: return -1;
    }
}

s32 battleMapSkillRange(u32 skill) {
    if (skill < 0x143) {
        if (skill >= 0x13d) {
            return 0x12c;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ADA8);

s32 func_0020AF00(u8 *unit, s32 action) {
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return -1;
    }
    if (*((u8 *)D_003BAA60 + action * 32 + 3) == 0) {
        return -1;
    }
    switch (*(u16 *)(unit + 0x124)) {
    case 0x11b: return action != 0x1ad ? 11 : 18;
    case 0x13d: return 12;
    case 0x13e: return 13;
    case 0x13f: return 14;
    case 0x140: return 15;
    case 0x141: return 16;
    case 0x142: return 17;
    default: return -1;
    }
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6018);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AFB8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B190);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B348);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B560);

s32 func_0020B580(u8 *unit) {
    u8 *entry = *(u8 **)(unit + 0xf4);
    u32 flags = *(u32 *)(*(u8 **)(entry + 0x18) + 0x110);
    u8 *other;
    if (flags & 0x200) {
        if ((flags & 0x1000) == 0) {
            return 0;
        }
        if (func_001DAE48(*(u32 *)(entry + 0x60)) == 1) {
            other = (u8 *)func_001DAE50(*(u32 *)(entry + 0x60), 0);
            if ((*(u32 *)(other + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_002044F0(unit, unit);
        } else {
            func_001DC760();
            func_0020B348(unit, unit, 0);
        }
    } else {
        func_0020AFB8();
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B640);

s32 func_0020B770(u8 *unit) {
    u8 *entry = *(u8 **)(unit + 0xf4);
    u8 *other;
    if (*(u32 *)(*(u8 **)(entry + 0x18) + 0x110) & 0x200) {
        if (func_001DAE48(*(u32 *)(entry + 0x60)) == 1) {
            other = (u8 *)func_001DAE50(*(u32 *)(entry + 0x60), 0);
            if ((*(u32 *)(other + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_0020B190(unit, other);
        } else {
            func_001DC760();
            func_0020B348(unit, unit, 0);
        }
        *(u32 *)(unit + 0x110) = 0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B818);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6338);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6358);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020BE30);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020C9E8);

u32 func_0020CB28(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 2;
    if (arg0 != 0x1d7) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020CB38);

void func_0020CCA8(void) {
    func_0020ADA8();
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6398);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6438);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6460);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6480);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020CCC0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D168);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D2E0);

s32 func_0020D4B0(u8 *unit, u8 *other, s32 condition) {
    s32 kind;
    if ((*(u32 *)(unit + 0x110) & 0x200) == 0) {
        return 1;
    }
    if (condition != 0) {
        return 0;
    }
    kind = *(u16 *)(other + 0x124);
    switch (kind) {
    case 0x13d:
    case 0x13e:
        return 0;
    default:
        return 1;
    }
}

void func_0020D4F0(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 != 0xd5) {
        return;
    }
    func_003014F0(arg2, "%s%03X_%02X.BED", D_003BB8A8, *(u16 *)(D_003BAA60 + 0x1aa4), *(s32 *)(arg0 + 0x38) - 0x13d);
}

u32 func_0020D548(void) {
    return 7;
}

s32 func_0020D550(s32 battler, s32 action) {
    u8 *unit = (u8 *)battler;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return -1;
    }
    if (*(u8 *)((u8 *)D_003BAA60 + action * 32 + 3) == 0) {
        return -1;
    }
    if (*(u16 *)(unit + 0x124) == 0x114) {
        return 11;
    }
    return -1;
}

s32 func_0020D598(s32 battler, s32 action) {
    u8 *unit = (u8 *)battler;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return -1;
    }
    if (*((u8 *)D_003BAA60 + action * 32 + 3) == 0) {
        return -1;
    }
    if (*(u16 *)(unit + 0x124) == 0x111) {
        return 11;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D5E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D668);

s32 func_0020D690(s32 battler, s32 action) {
    u8 *unit = (u8 *)battler;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return -1;
    }
    if (*((u8 *)D_003BAA60 + action * 32 + 3) == 0) {
        return -1;
    }
    if (*(u16 *)(unit + 0x124) == 0x109) {
        return 11;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D6D8);

s32 battleNormalizeActionForSkill(s32 battler, s32 action) {
    if ((*(u32 *)(battler + 0x110) & 0x400) == 0 || *(u16 *)(battler + 0x124) != 0x13c) {
        return action;
    }
    switch (action) {
    case 2: return 0;
    case 13: return -1;
    default: return action;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D858);

u32 func_0020D998(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0x7d;
    if (arg0 != 0x143) {
        temp_v0 = 0;
    }
    return temp_v0;
}

u8 func_0020D9A8(s32 arg0) {
    return arg0 != 0xd3;
}

s32 battleNormalizeActionForStatus(s32 battler, s32 action) {
    if ((*(u32 *)(battler + 0x110) & 0x400) == 0 || *(u16 *)(battler + 0x124) != 0x115) {
        return action;
    }
    switch (action) {
    case 2: return 0;
    case 13: return -1;
    default: return action;
    }
}

s32 func_0020D9F8(s32 battler, s32 action) {
    u8 *unit = (u8 *)battler;
    if ((*(u32 *)(unit + 0x110) & 0x400) == 0) {
        return -1;
    }
    if (*((u8 *)D_003BAA60 + action * 32 + 3) == 0) {
        return -1;
    }
    if (*(u16 *)(unit + 0x124) == 0x113) {
        return 11;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DA40);

extern void func_00204838(void *, void *, void *, s32, s32, f32, f32, f32);
extern void func_001DB698(void *);

void func_0020DB90(u8 *actor) {
    u8 *position = actor + 0x30;
    u8 *rotation = actor + 0xc0;
    func_00204838(actor, position, rotation, 0, 1, 0.25f, 0.0f, 0.5f);
    *(f32 *)(actor + 0x130) = 30.0f;
    *(u32 *)(actor + 0xf0) |= 0x41;
    *(f32 *)(actor + 0x50) += 500.0f;
    *(f32 *)(actor + 0xe0) += 500.0f;
    func_001DB698(position);
    func_001DB698(rotation);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DC38);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DE50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DE70);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E058);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E170);

s32 func_0020E868(u8 *unit) {
    u8 *entry = *(u8 **)(unit + 0xf4);
    u8 *other;
    if (*(u32 *)(*(u8 **)(entry + 0x18) + 0x110) & 0x200) {
        if (func_001DAE48(*(u32 *)(entry + 0x60)) == 1) {
            other = (u8 *)func_001DAE50(*(u32 *)(entry + 0x60), 0);
            if ((*(u32 *)(other + 0x110) & 0x400) == 0) {
                return 0;
            }
            func_0020DB90(unit);
        } else {
            func_001DC760();
            func_0020DC38(unit, unit, 0);
            *(u32 *)(unit + 0x110) = 0;
        }
        return 1;
    }
    return 0;
}

extern void func_001DC760(void);
extern void func_001DC3A0(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void func_0020DA40(u8 *);
INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E910);

s32 func_0020EA40(u8 *unit) {
    u16 flags = *(u16 *)((u8 *)D_003BAA60 + *(s32 *)(unit + 0x114) * 32 + 0x1c);
    if (flags & 0x4000) {
        func_001DC760();
        if (!(flags & 0x10)) {
            func_0020DA40(unit);
        } else {
            func_001DC3A0(unit, -203.0f, -531.1f, -1259.0f,
                           0.124f, -0.07f, -0.021f, 0.981f,
                           -203.0f, -46.1f, -1259.0f, -0.144f,
                           -0.066f, -0.003f, 0.978f, 45.0f, 30.0f);
        }
        *(s32 *)(unit + 0x110) = 0;
    } else if (flags & 0x8000) {
        func_001DC760();
        func_0020DC38(unit, unit, 0);
    } else if (flags & 8) {
        if (func_001DAE48(*(u32 *)(*(u8 **)(unit + 0xf4) + 0x60)) == 1) {
            func_001DC760();
            func_0020DB90(unit);
            *(s32 *)(unit + 0x110) = 0;
        } else {
            func_001DC760();
            func_0020DA40(unit);
        }
    } else {
        return 0;
    }
    func_001DC568();
    return 1;
}

s32 battleGetPhaseCommand(void) {
    u8 *battle = (u8 *)func_001A17F0();
    switch (*(u16 *)(battle + 0x25c)) {
    case 0: return 0x11c;
    case 1: return 0x11b;
    default: return -1;
    }
}

s32 battleGetAlternatePhaseCommand(void) {
    u8 *battle = (u8 *)func_001A17F0();
    switch (*(u16 *)(battle + 0x25c)) {
    case 0: return 0x11f;
    case 1: return 0x120;
    default: return -1;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020EC20);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ECF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ED90);

extern char D_003A66C0[];

void func_0020F808(void) {
    u8 *battle = (u8 *)func_001A17F0();
    void (*cleanup)(void) = *(void (**)(void))(battle + 0x5d0);
    if (cleanup != 0) {
        cleanup();
    }
    func_001FB0A8(D_003A66C0);
}

extern char D_003A66D8[];

void battleReleaseBossData(void) {
    u8 *battle = (u8 *)func_001A17F0();
    void (*cleanup)(void);
    if ((*(u32 *)(battle + 0x1f4) & 0x80000) == 0) {
        return;
    }
    cleanup = *(void (**)(void))(battle + 0x594);
    if (cleanup != 0) {
        cleanup();
    }
    func_0020F808();
    if (*(void **)(battle + 0x694) != 0) {
        func_002CF5C0(*(void **)(battle + 0x694));
        *(void **)(battle + 0x694) = 0;
    }
    *(u32 *)(battle + 0x1f4) &= ~0x80000;
    func_001FB0A8(D_003A66D8);
}

extern char D_003A66F0[];

s32 battleFindScriptResource(char *name) {
    char path[128];
    u8 *battle = (u8 *)func_001A17F0();
    if (*(s32 *)(battle + 0x1e8) == 0) {
        return -1;
    }
    func_003014F0(path, D_003A66F0, *(s16 *)(battle + 0x1c0), name);
    return func_0010BF30(*(s32 *)(battle + 0x1e8), path);
}

void func_0020F940(s32 skill) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 handle;
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return;
    }
    handle = func_0010BC68(*(s32 *)(*(u8 **)(battle + 0x29c) + 0x20) - 1,
                            *(s32 *)(battle + 0x1e8), skill);
    func_0010C028(handle, 0);
    func_00101A80(*(s32 *)(battle + 0x29c), handle);
    *(s32 *)(battle + 0x2a0) = handle;
}

s32 battleReleaseScriptResource(void) {
    extern s32 func_001019C8(s32);
    u8 *battle = (u8 *)func_001A17F0();
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return 1;
    }
    if (func_001019C8(*(s32 *)(battle + 0x2a0)) == 0) {
        *(s32 *)(battle + 0x2a0) = 0;
        return 1;
    }
    return 0;
}

extern char D_003BB8B8[];

s32 func_0020FA08(void) {
    u8 *battle = (u8 *)func_001A17F0();
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return 0;
    }
    if (*(u32 *)(battle + 0x1c4) & 1) {
        return 0;
    }
    if (*(s32 *)(battle + 0x1e8) == 0) {
        return 0;
    }
    return battleFindScriptResource(D_003BB8B8) != -1;
}

extern s32 func_0010BC68(s32, s32, s32);

void func_0020FA70(void) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 skill;
    s32 handle;
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return;
    }
    skill = battleFindScriptResource(D_003BB8B8);
    if (skill == -1) {
        return;
    }
    handle = func_0010BC68(*(s32 *)(*(u8 **)(battle + 0x29c) + 0x20) - 1,
                            *(s32 *)(battle + 0x1e8), skill);
    func_0010C028(handle, 0);
    func_00101A80(*(s32 *)(battle + 0x29c), handle);
    *(s32 *)(battle + 0x2a0) = handle;
    *(u32 *)(battle + 0x1c4) |= 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FB00);

extern char D_003BB8C0[];

s32 battleHasScriptResource(void) {
    u8 *battle = (u8 *)func_001A17F0();

    if (*(s16 *)(battle + 0x1c0) == -1) {
        return 0;
    }
    if (*(s32 *)(battle + 0x1e8) == 0) {
        return 0;
    }
    if ((*(s32 *)(battle + 0x1f4) & 0x800) == 0 || *(u8 *)(battle + 0x258) != 1) {
        return 0;
    }
    return battleFindScriptResource(D_003BB8C0) != -1;
}

void func_0020FB98(void) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 skill;
    s32 handle;
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return;
    }
    skill = battleFindScriptResource(D_003BB8C0);
    if (skill == -1) {
        return;
    }
    handle = func_0010BC68(*(s32 *)(*(u8 **)(battle + 0x29c) + 0x20) - 1,
                            *(s32 *)(battle + 0x1e8), skill);
    func_0010C028(handle, 0);
    func_00101A80(*(s32 *)(battle + 0x29c), handle);
    *(s32 *)(battle + 0x2a0) = handle;
    *(u32 *)(battle + 0x1c4) |= 2;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FC28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A66C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A66D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A66F0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FC48);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FF50);

extern char D_003A67A0[], D_003A67B8[];

s32 func_002100A8(void) {
    u8 *battle = (u8 *)func_001A17F0();
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return 1;
    }
    if (func_002E92C0(*(s32 *)(battle + 0x1e4)) == 0) {
        func_001FB0A8(D_003A67A0);
        return 0;
    }
    if (func_00241B80(*(s16 *)(battle + 0x1c0)) == 2) {
        return 1;
    }
    func_001FB0A8(D_003A67B8);
    return 0;
}

extern char D_003A67D0[];

void battleReleaseEventData(void) {
    u8 *battle = (u8 *)func_001A17F0();
    void *data;
    if ((*(u32 *)(battle + 0x1c8) & 2) == 0) {
        return;
    }
    data = *(void **)(battle + 0x1dc);
    if (data != 0) {
        func_00160B00(data);
        *(void **)(battle + 0x1dc) = 0;
    }
    data = *(void **)(battle + 0x1d8);
    if (data != 0) {
        func_00160800(data);
        *(void **)(battle + 0x1d8) = 0;
    }
    *(s16 *)(battle + 0x1cc) = 0;
    *(s32 *)(battle + 0x1d0) = -1;
    *(u32 *)(battle + 0x1c8) &= ~2;
    func_001FB0A8(D_003A67D0);
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A67A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A67B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A67D0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002101C8);

extern char D_003A6838[];

void func_00210288(void) {
    u8 *battle = (u8 *)func_001A17F0();
    void *data;
    battleReleaseEventData();
    data = *(void **)(battle + 0x1ec);
    if (data != 0) {
        func_002D0918(data);
        *(void **)(battle + 0x1ec) = 0;
    }
    func_001FB0A8(D_003A6838);
}

extern s32 func_00241BF0(s16, s32);

s32 func_002102D8(void) {
    s32 first = func_0010D428(0);
    s32 second = func_0010D428(1);
    s32 action = func_0010D428(2);
    u8 *unit;
    u8 *battle;
    s32 result;
    if (first == 0) {
        unit = (u8 *)func_001DACF8(second);
    } else {
        unit = (u8 *)func_001DAD60(second);
    }
    if (unit == 0) {
        return 1;
    }
    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return 1;
    }
    battle = (u8 *)func_001A17F0();
    result = func_00241BF0(*(s16 *)(battle + 0x1c0), action);
    if (result == 0) {
        return 1;
    }
    *(s32 *)(battle + 0x1d4) = result;
    *(u8 **)(battle + 0x1e0) = unit;
    *(s32 *)(battle + 0x1d0) = action;
    *(s16 *)(battle + 0x1cc) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002103A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210450);

u8 func_00210520(void) {
    s32 temp_v0;

    temp_v0 = countBattleTasksByKind(0x1a);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210540);

extern char D_003A6848[];

s32 func_002105F8(void) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 index = func_0010D428(0);
    if (func_002E92C0(*(s32 *)(battle + 0x1e4)) != 0) {
        soundSetSequenceVolumePan(*(s32 *)(battle + 0x1e4) + index, 0x7f, 0x3f);
        func_001FB0A8(D_003A6848, *(s32 *)(battle + 0x1e4) + index);
    }
    return 1;
}

typedef struct BattleTask {
    u8 active;
    u8 unk_01[0xf];
    u8 phase;
    u8 unk_11[0xf];
    s16 kind;
    u8 unk_22[0x2a];
    s32 (*update)(void *);
} BattleTask;

typedef struct BattleTaskData {
    void *battler;
    s32 action;
    s32 finished;
} BattleTaskData;

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210670);

void *battleCreateActionTask(void *battler, s32 action) {
    extern BattleTask *func_001D4748(s32);
    extern BattleTaskData *func_001D47D8(BattleTask *);
    extern s32 func_00210670(void *);
    BattleTask *task = func_001D4748(12);
    BattleTaskData *data;

    task->active = 1;
    task->kind = 0x62;
    task->update = func_00210670;
    task->phase = 0;
    data = func_001D47D8(task);
    data->battler = battler;
    data->action = action;
    data->finished = 0;
    return task;
}

typedef struct BattleCombatant {
    u8 unk_00[0x110];
    u32 status;
    u8 unk_114[0x1a];
    u16 ailment;
    u8 unk_130[0x214];
    struct BattleCombatant *next;
} BattleCombatant;

s32 battleHasRestrictedUnit(void) {
    BattleCombatant *unit = *(BattleCombatant **)((u8 *)func_001A17F0() + 0x228);
    while (unit != 0) {
        u32 status = unit->status;
        if (status & 0x200) {
            if (status & 0xe0) {
                return 1;
            }
            if (unit->ailment & 0x4000) {
                return 1;
            }
        }
        unit = unit->next;
    }
    return 0;
}

s32 battleListHasMarkedFlag(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(entries[i] + 0xe) & 0x7fff) == 0x4000) {
            return 1;
        }
    }
    return 0;
}

s32 battleListCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (*(u16 *)(entries[i] + 6) < *(u16 *)(entries[i] + 8)) {
            return 0;
        }
    }
    return 1;
}

s32 battleListSecondaryCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (*(u16 *)(entries[i] + 0xa) < *(u16 *)(entries[i] + 0xc)) {
            return 0;
        }
    }
    return 1;
}

s32 battleListHasMatchingFlag(u8 **entries, s32 count, u32 flags) {
    s32 i;
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(entries[i] + 0xe) & 0x7fff) & flags) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210928);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210A18);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6838);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6848);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6860);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210BA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210D00);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210EB0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002110B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002111A0);

typedef struct BattleModelEntry {
    s32 kind;
    s32 id;
    s32 refs;
    s8 state;
    u8 unk_0d[3];
    void *resource;
    void *actor;
    struct BattleModelEntry *prev;
    struct BattleModelEntry *next;
} BattleModelEntry;

extern void *func_002CFF68(s32);

BattleModelEntry *battleCreateModelEntry(void) {
    BattleModelEntry *entry = func_002CFF68(sizeof(BattleModelEntry));
    u8 *battle;
    BattleModelEntry *head;
    entry->refs = 1;
    entry->state = 0;
    battle = (u8 *)func_001A17F0();
    entry->prev = 0;
    head = *(BattleModelEntry **)(battle + 0x240);
    if (head != 0) {
        head->prev = entry;
        entry->next = *(BattleModelEntry **)(battle + 0x240);
    } else {
        entry->next = 0;
    }
    *(BattleModelEntry **)(battle + 0x240) = entry;
    return entry;
}

extern char D_003A68F8[];
extern void func_00288788(void *);
extern void func_001F3F40(void *);
extern void func_002CFF98(void *);

void battleReleaseModelEntry(BattleModelEntry *entry) {
    if (--entry->refs != 0) {
        return;
    }
    if (entry->resource != 0) {
        func_00288788(entry->resource);
    }
    if (entry->actor != 0) {
        func_001F3F40(entry->actor);
    }
    if (entry->next != 0) {
        entry->next->prev = entry->prev;
    }
    if (entry->prev != 0) {
        entry->prev->next = entry->next;
    } else {
        *(BattleModelEntry **)((u8 *)func_001A17F0() + 0x240) = entry->next;
    }
    func_002CFF98(entry);
    func_001FB0A8(D_003A68F8, entry->kind, entry->id);
}

void battleReleaseAllModelEntries(void) {
    BattleModelEntry *entry = *(BattleModelEntry **)((u8 *)func_001A17F0() + 0x240);
    BattleModelEntry *next;
    while (entry != 0) {
        next = entry->next;
        battleReleaseModelEntry(entry);
        entry = next;
    }
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A68F8);

void battleFormatModelResourcePath(s32 isDevil, s32 modelId, char *filename) {
    extern void func_003014F0(char *, const char *, const char *, s32);
    if (isDevil == 0) {
        func_003014F0(filename, "%spc%03X_ms.LB", "/model/human/", modelId);
    } else {
        func_003014F0(filename, "%s%03X_ms.LB", "/model/devil/", modelId);
    }
}

extern s32 func_00217068(s32, s32, s32);
extern s32 func_00288BE8(void *);

s32 func_002114E8(u8 *task) {
    s32 result;
    if (*(s8 *)(task + 0xc) != 0) {
        return 1;
    }
    if (func_00217068(*(s32 *)task, *(s32 *)(task + 4), 0) == 0 ||
        func_00217068(*(s32 *)task, *(s32 *)(task + 4), 0) == -1) {
        return 0;
    }
    if (*(void **)(task + 0x10) == 0) {
        return 1;
    }
    result = func_00288BE8(*(void **)(task + 0x10));
    return (s8)result;
}

s32 battleFindModelEntry(kind, id)
s32 kind;
s32 id;
{
    BattleModelEntry *entry = *(BattleModelEntry **)((u8 *)func_001A17F0() + 0x240);
    while (entry != 0) {
        if (entry->kind == kind && entry->id == id) {
            return (s32)entry;
        }
        entry = entry->next;
    }
    return 0;
}

void func_002115E0(void) {
}

void func_002115E8(void) {
    battleReleaseAllModelEntries();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211600);

void func_00211708(void) {
    s32 temp_v0;

    temp_v0 = battleFindModelEntry();
    if (temp_v0 != 0) {
        battleReleaseModelEntry((BattleModelEntry *)temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211740);

s32 battleGetEntryState(s32 kind, s32 value) {
    u8 *entry = (u8 *)battleFindModelEntry(kind, value);
    if (entry != 0) {
        return *(s8 *)(entry + 0xc);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002118A8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002118D8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002119A8);

void func_00211A28(s32 *arg0) {
    s32 *temp_a0 = arg0;
    s32 temp_v0;
    s32 i = 0xff;

    do {
        temp_v0 = *temp_a0;
        i--;
        *temp_a0 = (temp_v0 & 0xffffff) | (temp_v0 << 0x18);
        temp_a0++;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211A60);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211B88);

extern void func_002D6258(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_00211CE8(s32 packet, s32 first, s32 second, s32 color) {
    func_002D6258(packet, second, first, 0x7000, 0x7900, 0x9000, 0x7900, 0x7000,
                  0x8700, 0x9000, 0x8700, color, 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211D40);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002121E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002124E8);

void battleReleaseOwnedData(void) {
    extern void func_002CFF98(void *);
    void *data = D_003D7580.ownedData;
    if (data != 0) {
        func_002CFF98(data);
        D_003D7580.ownedData = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00212680);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002127A8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00212998);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002131F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213280);

void func_00213368(void) {
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213370);

extern void func_001054D0(s32, s32, f32);
extern u8 D_00325870[];

void func_00213538(void) {
    u8 *entry;
    s32 i;
    u64 clearValue;
    func_001054D0(0x200, 0xe0, 0.0f);
    func_001055F8();
    entry = D_00325870;
    i = 0;
    clearValue = 0x80008000ULL;
    entry += 0x1b70;
    do {
        i++;
        *(u64 *)(entry - 0x10) = clearValue;
        *(u64 *)entry = clearValue;
        entry += 0x1f40;
    } while (i != 2);
    D_003D7580.options |= 2;
}

void battleReleaseRuntimeResource(void) {
    extern void releaseSharedEffectReference(void *);
    extern void func_00105618(void);
    extern void func_001055C0(void);
    void *resource = D_003D7580.resource;
    if (resource != 0) {
        releaseSharedEffectReference(resource);
        D_003D7580.resource = 0;
    }
    func_00105618();
    func_001055C0();
}

extern void *func_002D0518(s32);
extern void *func_002D0A48(void *);
extern void *func_002D5510(s32);
extern void *func_002D3FD0(s32);
extern void func_002D4540(void *);
extern void func_002D38B8(void *, void *, s32, s32, s32, s32, void *, s32, s32, s32);
extern void func_002D4588(void *, void *);
extern u8 D_00325860[];
typedef struct BattleGraphicsCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} BattleGraphicsCallback;
extern BattleGraphicsCallback D_00325708;

void battleInitializeGraphicsRuntime(void) {
    BattleRuntimeState *runtime = &D_003D7580;
    void *surface;
    void *context;
    runtime->handle = func_002D0518(0x70000);
    runtime->request = func_002D0A48(runtime->handle);
    surface = func_002D5510(0);
    context = func_002D3FD0(16);
    func_002D4540(context);
    func_002D38B8(surface, context, 0, 0, 0x200, 0xe0, runtime->request, 0, 0, 0);
    func_002D4588(D_00325860, context);
    D_00325708.invoke(&D_00325708, surface);
}

extern void sdfCreateDescriptorPacket(void *, s32, s32, s32, s32, s32, void *, s32);
extern void func_002D0A10(void *);
extern u8 *D_003BA8F8;

void func_002136C0(void) {
    BattleRuntimeState *runtime = &D_003D7580;
    void *surface = func_002D5510(0);
    sdfCreateDescriptorPacket(surface, *(s32 *)(D_003BA8F8 + 0x10), 0, 0, 0x200, 0xe0, runtime->request, 0);
    D_00325708.invoke(&D_00325708, surface);
    func_002D0A10(runtime->handle);
    runtime->handle = 0;
    runtime->request = 0;
    runtime->options |= 1;
}

extern void func_002D5CD0(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void battleInitializeOverlayGraphics(void) {
    void *surface = func_002D5510(0);
    void *context = func_002D3FD0(16);
    func_002D4540(context);
    func_002D5CD0(surface, context, *(s32 *)(D_003BA8F8 + 0x10), 0, 0, 0, 0, 0x200, 0xe0, 0, 0);
    func_002D4588(D_00325860, context);
    D_00325708.invoke(&D_00325708, surface);
    D_003D7580.options |= 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213808);

extern void *memset(void *, s32, u32);

void battleClearRuntimeState(void) {
    BattleRuntimeState *state = &D_003D7580;
    memset(state, 0, sizeof(*state));
    state->flags = 0;
    state->state = 0;
    state->unk_06 = 0;
    state->pending = 0;
    state->active = 0;
    state->options = 0;
    state->resource = 0;
    state->handle = 0;
    state->request = 0;
}

void battleResetRuntimeState(void);

void battleResetAsyncState(void) {
    extern void func_002D0A10(void *);
    void *handle = D_003D7580.handle;
    if (handle != 0) {
        func_002D0A10(handle);
        D_003D7580.handle = 0;
        D_003D7580.request = 0;
    }
    battleResetRuntimeState();
}

void battleActivateRuntime(u8 condition) {
    extern s32 func_001061E8(void);
    BattleRuntimeState *battle = &D_003D7580;
    battle->unk_06 = condition;
    battle->flags = 0;
    battle->state = 1;
    battle->active = 1;
    battle->pending = 0;
    battle->options = 0;
    if (func_001061E8() != 0) {
        battle->options |= 4;
    }
}

void battleResetRuntimeState(void) {
    D_003D7580.state = 0;
    D_003D7580.active = 0;
    battleReleaseOwnedData();
}

s32 func_00213B50(void) {
    return D_003D7588[0];
}

s32 func_00213B60(void) {
    u16 state;

    if (D_003D7580.active == 0) {
        return 1;
    }
    state = D_003D7580.state;
    if (state == 0) {
        return 1;
    }
    return state == 2;
}

s32 func_00213B90(void) {
    u16 state;

    if (D_003D7580.active == 0) {
        return 1;
    }
    state = D_003D7580.state;
    if (state == 0) {
        return 1;
    }
    return state == 4;
}

void func_00213BC0(void) {
    if (D_003D7580.active != 0) {
        D_003D7580.pending = 1;
    }
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BF8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213BE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002142C0);

extern void *func_00197748(s32, s32, u32, u32, s32, s32);
extern void func_001958A0(void *, s32, s32);
extern s32 func_00194920(void *);

s32 func_00214438(s32 width, s32 height, s32 mode) {
    void *packet = func_00197748(width << 4, height << 3, 0xff0000, 0xa09dc380, mode, 0);
    func_001958A0(packet, 0, 0x60);
    return func_00194920(packet);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214490);

extern void func_001FB140(u8 *, u8 *, s32, s32, u32, u32);
extern s32 func_00214490(u8 *, u8 *, s32, u8 *, s32);

s32 func_00214588(u8 *first, u8 *second, s32 mode, u8 *settings, s32 extra) {
    s32 offset = *(s32 *)(settings + 0xc) * 24 + 4;
    func_001FB140(first - 4, second - 4, mode, offset, 0x80806020, 0x30000000);
    return func_00214490(first, second, mode, settings, extra);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214618);

extern s32 D_003BAA70;
extern s32 D_003BAA74;
extern s32 D_003BAA84;

void func_00214768(void) {
    BtlState *state = (BtlState *)func_001A17F0();
    u32 i;
    state->unk_708 = 0;
    state->unk_6A8 = 7;
    state->unk_6B4 = 7;
    state->unk_6C4 = 0xF;
    state->unk_6D8 = 0x1D;
    state->unk_6E4 = 0xF;
    state->unk_6E8 = 0x20;
    state->unk_6F4 = 0xF;
    if (state->unk_E0C == 0) {
        state->unk_6B8 = 0x180;
    } else {
        state->unk_6B8 = 0x20;
    }
    for (i = 0; i < 0x20; i++) {
        state->table0[i] = D_003BAA70 + i * 0x11;
    }
    for (i = 0; i < 0x180; i++) {
        state->table1[i] = D_003BAA74 + i * 0x11;
    }
    for (i = 0; i < 0x20; i++) {
        state->table2[i] = D_003BAA84 + 0xFA0 + i * 0x19;
    }
    state->unk_6A4 = 0;
    state->unk_E0E = -1;
}


INCLUDE_ASM(const s32, "game/code_001FF030", func_00214868);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00215A50);

void func_00215FE0(void) {
    D_003BBB0D = 0;
    dds3WorkClear();
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7128);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7138);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7148);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7158);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7168);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7178);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7188);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7198);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7208);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7218);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7228);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7238);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7248);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7258);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7268);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7278);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7288);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7298);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72C8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00215FF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002162E0);

void func_002166A8(void) {
    s32 i;

    D_003BD878 = createSemaphore(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        D_00367940[i] = 0;
        D_00367960[i] = 0;
    }
}

s32 *battleFindGroupedEntity(group, type)
    s32 group;

    s32 type;

{
    s32 *entry = (s32 *)D_00367940[group];
    while (entry != 0) {
        if (*(u16 *)((u8 *)entry + 0xa) == type) {
            break;
        }
        entry = (s32 *)*entry;
    }
    return entry;
}

s32 battleGroupContainsId(s32 group, s32 id) {
    s32 *entry = (s32 *)D_00367960[group];
    while (entry != 0) {
        if (entry[1] == id) {
            return 1;
        }
        entry = (s32 *)*entry;
    }
    return 0;
}

extern void *func_002CFEB8(s32);

typedef struct BattleGroupIdEntry {
    struct BattleGroupIdEntry *next;
    s32 id;
} BattleGroupIdEntry;

void battleAddGroupId(s32 group, s32 id) {
    BattleGroupIdEntry *node = func_002CFEB8(sizeof(BattleGroupIdEntry));
    BattleGroupIdEntry **head = (BattleGroupIdEntry **)&D_00367960[group];
    node->id = id;
    node->next = *head;
    *head = node;
}

void func_002167E0(s32 arg0, s32 arg1) {
    s32 *temp_v0;
    s32 *temp_v1;

    temp_v0 = &D_00367960[arg0];
    temp_v1 = (s32 *)*temp_v0;
    if (temp_v1 == 0) {
        return;
    }
    do {
        if (*(temp_v1 + 1) == arg1) {
            *temp_v0 = *temp_v1;
            func_002CFF98(temp_v1);
            break;
        } else {
            temp_v0 = temp_v1;
            temp_v1 = (s32 *)*temp_v1;
        }
    } while (temp_v1 != 0);
}

typedef struct BattleGroupSlot {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
    s32 unk_C;
} BattleGroupSlot;

typedef struct BattleGroupNode {
    struct BattleGroupNode *next;
    struct BattleGroupNode *prev;
    s16 group;
    s16 type;
    u8 flag;
    u8 unk_0D[3];
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    BattleGroupSlot slots[8];
    s32 unk_A0;
    s32 unk_A4;
    s32 unk_A8;
    f32 unk_AC;
    f32 unk_B0;
} BattleGroupNode;

void func_00216840(s32 group, s32 type, s32 flag, s32 arg3, s32 arg4, s32 arg5) {
    BattleGroupNode *node;
    BattleGroupNode *head;
    s32 i;
    func_00216A70(group, type);
    node = func_002CFEB8(sizeof(BattleGroupNode));
    head = (BattleGroupNode *)D_00367940[group];
    if (head != NULL) {
        head->prev = node;
    }
    D_00367940[group] = (s32)node;
    node->next = head;
    node->group = group;
    node->type = type;
    node->unk_14 = arg3;
    node->unk_18 = arg4;
    node->unk_1C = arg5;
    node->prev = NULL;
    node->unk_10 = 0;
    for (i = 0; i != 8; i++) {
        node->slots[i].unk_0 = 0;
        node->slots[i].unk_8 = 0;
        node->slots[i].unk_C = 0;
    }
    node->flag = flag & 1;
    node->unk_A0 = 0;
    node->unk_A4 = 0;
    node->unk_A8 = 0;
    node->unk_AC = 1.0f;
    node->unk_B0 = 100.0f;
}


INCLUDE_ASM(const s32, "game/code_001FF030", func_00216958);

void func_00216A70(void) {
    s32 *temp_v0;

    temp_v0 = battleFindGroupedEntity();
    func_00216958(temp_v0);
}

void battleReleaseAllEntities(void) {
    u32 i = 0;
    s32 *head = D_00367940;
    do {
        s32 *node = (s32 *)*head;
        while (node != 0) {
            s32 *next = (s32 *)*node;
            func_00216958(node);
            node = next;
        }
        i++;
        head++;
    } while (i < 8);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216B00);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB880);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB888);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB890);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB898);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8A0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8A8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8B0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8B4);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8B8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8C0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8C8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8D0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8D8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8E0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8E8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8F0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8F8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB900);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB908);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB910);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB918);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB920);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB928);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB930);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB938);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB93E);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB940);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB948);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB950);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB958);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB960);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB968);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB970);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB978);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB980);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB988);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB990);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB998);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9A0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9A8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9B0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9B8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9C0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9C8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9D0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9D8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9E0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9E8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9F0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB9F8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA00);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA08);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA10);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA18);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA20);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA28);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA30);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA38);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA40);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA48);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA50);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA58);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA60);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA68);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA70);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA78);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA80);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA88);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA90);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBA98);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAA0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAA8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAB0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAB8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAC0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAC8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAD0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAD8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAE0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAE8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAF0);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBAF8);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB00);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB08);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB0D);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB0E);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB0F);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB10);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB14);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB18);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB1C);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB1E);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB20);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB24);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB28);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB2C);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB30);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB38);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB40);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB48);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB50);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BBB58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7300);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7310);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7320);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7330);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7340);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7360);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7370);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7380);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7390);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7400);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7420);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7430);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7450);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7460);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7470);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7480);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7500);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7510);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7520);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7540);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7550);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7560);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7570);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7580);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7590);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7600);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7610);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7620);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7630);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7640);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7650);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7660);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7670);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7680);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7690);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7700);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7710);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7720);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7730);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7740);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7750);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7760);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7770);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7780);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7790);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7800);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7810);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7820);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7830);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7840);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7860);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7870);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7880);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7890);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7900);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7910);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7920);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7930);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7950);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7960);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7980);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7ED0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8000);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8010);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8020);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8030);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8040);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8050);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8060);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8070);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8080);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8090);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8100);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8110);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8120);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8130);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8140);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8150);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8160);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8170);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8180);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8190);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8200);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8210);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8220);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8230);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8240);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8250);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8260);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8270);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8280);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8290);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8300);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8310);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8320);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8330);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8340);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8360);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8370);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8380);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8390);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8400);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8420);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8430);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8450);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8460);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8470);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8480);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8500);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8510);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8520);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8540);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8558);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8570);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8588);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8600);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8618);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8630);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8648);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8660);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8678);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8690);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8708);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8720);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8738);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8750);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8768);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8780);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8798);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8810);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8828);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8840);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8860);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8870);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8880);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8890);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8900);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8910);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8920);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8930);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8950);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8960);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8980);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8ED0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9000);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9010);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9020);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9030);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9040);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9050);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9060);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9070);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9080);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9090);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9100);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9110);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9120);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9130);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9140);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9150);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9160);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9170);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9180);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9190);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9200);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9210);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9220);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9230);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9240);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9250);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9260);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9270);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9280);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9290);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9300);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9310);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9320);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9330);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9340);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9360);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9370);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9380);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9390);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9400);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9420);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9430);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9450);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9460);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9470);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9480);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9500);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9510);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9520);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9540);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9550);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9560);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9570);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9580);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9590);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9600);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9610);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9620);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9630);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9640);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9650);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9660);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9670);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9680);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9690);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9700);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9710);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9720);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9730);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9740);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9750);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9760);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9770);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9780);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9790);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9800);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9810);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9820);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9830);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9840);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9860);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9870);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9880);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9890);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9900);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9910);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9920);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9930);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9950);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9960);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9980);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EF8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FB8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA000);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA018);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA030);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA048);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA060);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA078);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA090);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA100);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA110);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA128);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA140);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA158);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA168);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA178);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA188);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA198);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA208);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA218);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA228);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA238);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA248);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA258);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA270);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA288);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA2A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA2D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA2F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA328);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA378);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA3A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA3C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA3F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA418);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA468);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA4B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA4E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA508);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA558);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA580);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA5A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA5D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA5F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA620);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA648);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA670);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA698);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA6C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA6E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA710);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA738);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA760);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA788);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA7B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA7D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA800);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA828);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA878);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA8A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA8C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA8F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA918);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA968);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA9B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA9E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAAA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAAD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAAF8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AABA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AABC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AABE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AACA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AACC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AACE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AADA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AADC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AADE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAEA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAEC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAEE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAFA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAFC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAFE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB008);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB028);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB048);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB068);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB088);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB0A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB0C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB0E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB108);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB128);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB148);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB168);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB188);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB1A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB1C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB1E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB208);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB228);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB248);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB268);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB288);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB2A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB2C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB2E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB308);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB328);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB348);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB368);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB388);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB3A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB3C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB3E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB408);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB428);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB448);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB468);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB488);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB4A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB4C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB4E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB508);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB528);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB548);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB568);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB588);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB5A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB5C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB5E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB608);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB628);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB648);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB668);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB688);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB6A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB6C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB6E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB708);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB728);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB748);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB768);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB788);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB7A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB7C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB7E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB808);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB828);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB848);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB868);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB888);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB8A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB8C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB8E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB908);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB928);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB958);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB988);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA38);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA68);

