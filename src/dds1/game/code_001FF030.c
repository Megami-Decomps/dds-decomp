#include "common.h"
#include "btl.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern u32 btlGetEffectActor(void);

extern u32 func_001A3360(s32, s32, s32);

extern void *btlAllocateIndexList(s32);

extern u32 btlGetIndexListCount();

extern s32 btlAreUnitStatusAndEntryFlagsClear();

extern s32 func_001A2B00(void);

extern s32 btlHasAvailableOption(void);

extern s32 func_00200628();

extern s32 func_002007A8(u32, u32, u32);

extern s32 btlIsSelectedActorStatusAndRecordClear();

extern s8 D_003BB870;

extern s32 *D_003BB87C;

extern char D_003BB8A0[];

extern char D_003BB898[];

extern s32 D_003BAA60;

extern s8 D_003A5A80[];

extern s8 D_003A5AA8[];

extern u32 btlGetEventEffectValue(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetEffectActive(void);

extern u32 btlGetSelectedBossEffectId(void);

extern u32 btlGetSpecialModeEffectValue(void);

extern s32 func_001A17F0(void);

extern void func_001D6300(void *, void *);

extern u8 *D_003BAA50;

extern s8 *D_003BAA4C;

extern s32 mdlFlagTest(s32);

extern u8 *btlCreateStiffenDamageShakeTask(u8 *, f32);

extern void btlSetUnitPosition(BtlUnit *, void *);

extern void btlSetUnitRotation(BtlUnit *, void *);

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

typedef struct AiSpecies {
    u8 pad00[0x40];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *D_003BAA24;

extern u8 D_003BD476;

extern char D_003A5988[];

extern void btlDebugPrintf(const char *, ...);

typedef struct BtlUnit {
    u8 unk_00[0x30];
    f32 position[4]; /* 0x30: world position, target of btlSetUnitPosition */
    f32 rotation[4]; /* 0x40: world orientation, target of btlSetUnitRotation */
    u8 unk_50[0x30];
    f32 scale80;     /* 0x80 */
    u8 unk_84[0xC];
    s32 position90; /* 0x90 */
    f32 position94;
    f32 position98;
    s32 position9C;
    u8 unk_A0[0x10];
    f32 positionB0;
    f32 positionB4;
    f32 positionB8;
    f32 positionBC;
    u8 unk_C0[8];
    u32 species;
    u8 unk_CC[0x14];
    s32 displaySpecies; /* 0xE0 */
    u8 unk_E4[8];
    s32 unk_EC;
    u32 unk_F0;
    BtlTask *task;       /* 0xF4 */
    BtlUnit *linkedA;    /* 0xF8 */
    BtlUnit *linkedB;    /* 0xFC */
    u8 unk_100[8];
    u64 identity; /* 0x108: compared to exclude the current actor */
    u32 flags;
    u32 unk_114;
    u8 unk_118[4];
    u8 lookupId;      /* 0x11C */
    u8 unk_11D[3];
    u16 unk_120;      /* 0x120: unit stat flags (0x1000/0x2000), also read as a stat block */
    u8 unk_122[2];
    u16 mode;
    u16 hp;           /* 0x126 */
    u16 maxHp;        /* 0x128 */
    u16 unk_12A;      /* 0x12A */
    u8 unk_12C[2];
    u16 unk_12E;
    u8 unk_130[4];
    u16 actionTime;   /* 0x134: tick count of the unit's last action */
    u8 unk_136[0x17A];
    s16 actionSlot;   /* 0x2B0 */
    u8 unk_2B2[0x6A];
    u32 unk_31C;
    struct BtlUnitModel *model;
    u8 unk_324[0x20];
    struct BtlUnit *next;
} BtlUnit;

/* The group scans load both adjacent status words in one 64-bit access. */
typedef struct BtlUnitStatus {
    u8 pad_000[0x110];
    u64 combinedFlags;
} BtlUnitStatus;

typedef struct BtlState {
    u8 unk_000[0x1C0];
    s16 unk_1C0;
    u8 unk_1C2[2];
    u32 scriptFlags;      /* 0x1C4 */
    u32 eventFlags;       /* 0x1C8 */
    s16 eventActive;      /* 0x1CC */
    u8 unk_1CE[2];
    s32 eventAction;      /* 0x1D0 */
    s32 eventResult;      /* 0x1D4 */
    void *eventRequest;   /* 0x1D8 */
    void *eventData;      /* 0x1DC */
    BtlUnit *eventUnit;   /* 0x1E0 */
    s32 sequenceHandle;   /* 0x1E4 */
    s32 scriptHandle;     /* 0x1E8 */
    void *eventAssets;    /* 0x1EC */
    u8 unk_1F0[4];
    u32 unk_1F4;
    u8 unk_1F8[4];
    u32 unk_1FC;
    u8 unk_200[0x24];
    BtlTask *tasks;
    BtlUnit *units;
    u8 unk_22C[0x1C];
    u16 turnPhase;    /* 0x248 */
    u8 unk_24A[2];
    u16 mode;
    u8 unk_24E[2];
    s32 turnCount;    /* 0x250 */
    u8 unk_254[4];
    u8 eventReady;        /* 0x258 */
    u8 unk_259[3];
    u16 phase;            /* 0x25C */
    u8 unk_25E[0x1E];
    s32 battleMode;   /* 0x27C */
    u8 unk_280[0x1C];
    s32 scriptProcess;    /* 0x29C */
    s32 scriptTask;       /* 0x2A0 */
    u8 unk_2A4[0x2F0];
    void (*bossCleanup)(void); /* 0x594 */
    u8 unk_598[0x20];
    s32 unk_5B8;
    u8 unk_5BC[0x14];
    void (*cleanup)(void); /* 0x5D0 */
    u8 unk_5D4[0xC0];
    struct BattleEffectState *effect;
    u8 unk_698[0xC];
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

extern s32 func_0020B348();

extern s32 btlAnyUnitHasActionInSlots();

extern s32 func_001A8CE0(s32);

extern u32 func_001A9488(s32);

extern u8 D_00360EE0[];

extern u8 D_00360EF0[];

extern void btlInitMotionTransformFromVectors(s32, u8 *, u8 *);

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(void *);

extern s32 btlRunAiAction();

extern u32 btlPickWeightedAiSlot();

extern u32 btlNextScaledRandom(u32);

extern u32 D_003BB878;

extern s32 func_001FFE30(BtlTask *, s32, u16 *, s32 *);

extern void (*D_00360E38[])(BtlTask *, u32, s32);

extern void btlCopyIndexList(s32, s32);

extern void *memset(void *, s32, u32);

extern s32 func_002024A8(s32, u16 *, u16);

void btlRunWeightedAiAction(BtlTask *task, s32 row) {
    u16 species;
    s32 index;

    D_003BB87C = sdfAllocAndClearQuadwords(0x10);
    species = task->unit->mode;
    index = btlPickWeightedAiSlot(task->unit, species, row);
    btlRunAiAction(task, D_003BAA24[species].slot[row * 5 + index].actionId, D_003BAA24[species].slot[row * 5 + index].actionArg);
    sdfReleaseChipBlock(D_003BB87C);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF0C8);

u32 func_001FF558(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF560);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF8D8);

/* Weighted pick of a table row: returns the first slot whose cumulative
   weight reaches `roll` (btlRollAiBucket) among the five slots of row `index`. */
u32 btlPickWeightedAiSlot(s32 unit, s32 species, s32 index) {
    u32 roll;
    u32 total;
    u32 i;

    func_001A17F0();
    roll = btlRollAiBucket();
    total = 0;
    for (i = 0; i < 5; i++) {
        u32 weight = D_003BAA24[species].slot[index * 5 + i].weight;

        total = (total + weight) & 0xFFFF;
        if (total >= roll && weight != 0) {
            return i;
        }
    }
    btlDebugPrintf("AI_BUGBUGBUGBUGBUG           \n");
    if (D_003BD476 == 0) {
        btlBossDebugPrintf(D_003A5988);
    }
    return 0;
}

/* Sum of two 0..0xFFF rolls, folded into 0..0xFFF; /41 turns it into one of 100 buckets. */
static inline u32 btlRollTwice(void) {
    u32 value = btlNextScaledRandom(0x1000);
    value += btlNextScaledRandom(0x1000);
    return value & 0xFFF;
}

/* Rolls a bucket; if it lands within 3 of the previous one, rolls again. */
u32 btlRollAiBucket(void) {
    u32 slot = btlRollTwice() / 0x29;
    if (D_003BB878 < slot - 3 || D_003BB878 > slot + 3) {
        D_003BB878 = slot;
    } else {
        u32 reroll = btlRollTwice();
        D_003BB878 = slot;
        return reroll / 0x29;
    }
    return slot;
}

/* Out parameters of the action-id lookup. */
typedef struct BtlActionLookup {
    u16 slot;
    s32 result;
} BtlActionLookup;

/* `packed` holds the handler index in its top 10 bits and the handler argument in the low 22. */
s32 btlRunAiAction(BtlTask *task, s32 id, u32 packed) {
    BtlActionLookup lookup;
    u32 op = packed >> 22;
    u32 payload = packed & 0x3fffff;

    func_001FFE30(task, id & 0xffff, &lookup.slot, &lookup.result);
    task->result = lookup.result;
    task->arg = lookup.slot;
    task->unit->actionSlot = lookup.slot;
    D_00360E38[op](task, payload, lookup.result);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5988);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFE30);

extern s32 func_001A17F8(void *);

extern s32 func_001A1838(void *);

s32 btlIsUnitAtOrBelowHealthRate(BtlUnit *unit, s32 multiplier) {
    void *stats = &unit->unk_120;
    s32 current = func_001A17F8(stats);
    s32 maximum = func_001A1838(stats);
    if ((u32)(maximum * multiplier) < (u32)(current * 100)) {
        return 0;
    }
    return 1;
}

s32 btlHasBossAtOrBelowHealthRate(s32 unused, s32 multiplier) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x401) == 0x401) {
            if (btlIsUnitAtOrBelowHealthRate(unit, multiplier) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

u32 btlResetAiCounterAtLimit(void) {
    u32 result;

    result = btlAiCounterReachedLimit();
    if (result == 0) {
        return result;
    }
    *(u16 *)(*(s32 *)D_003BB87C + 0x88) = 0;
    return 1;
}

s32 btlIsGroup400CountAtMost(s32 unused, u32 limit) {
    u32 count = 0;
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            count++;
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

s32 btlIsGroup200EligibleCountAtMost(s32 unused, u32 limit) {
    u32 count = 0;
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->unk_12E & 0x800) == 0) {
                count++;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

u8 func_00200308(void) {
    s32 result;

    result = btlIsSelectedActorStatusAndRecordClear();
    return result != 0;
}

s32 btlIsGroup200CountAtMost(s32 unused, u32 limit) {
    u32 count = 0;
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            count++;
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (limit < count) {
        return 0;
    }
    return 1;
}

s32 btlUnitHasActionMask(s32 unit, s32 mask) {
    return (func_001A1938(unit + 0x120, mask) & mask) != 0;
}

s32 btlAnyGroup400HasActionMask(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x401) == 0x401) {
            if (btlUnitHasActionMask((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyGroup200HasActionMask(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (btlUnitHasActionMask((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAllGroup200HaveActionMask(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (btlUnitHasActionMask((s32)unit, action) == 0) {
                return 0;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 1;
}

/* Find an active unit on the 0x200 side with the requested unit mode. */
s32 btlHasGroup200UnitMode(s32 unused, s32 kind) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->mode == kind) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

/* Check the opposing side for another unit of the same mode but a different identity. */
s32 btlHasOtherGroup400UnitMode(BtlUnit *actor, s32 kind) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (((BtlUnit *)unit)->mode == kind) {
                if (unit->identity != actor->identity) {
                    return 1;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

extern s32 D_00360E98[];

extern s32 btlActorEntryIsExpired();

extern s32 btlGetActorEntryCode();

/* Kinds 0xB/0xC scan the even/odd entry codes; other kinds test their own
   entry. Every failure falls out to the single trailing `return 0`. K&R:
   callers pass one argument or none. */
s32 func_00200628(actor, kind)
    s32 actor;
    s32 kind;
{
    s32 i;
    s32 j;

    if (kind == 0xB) {
        for (i = 0; i < 10; i += 2) {
            if (btlActorEntryIsExpired(actor, D_00360E98[i]) != 0 && btlGetActorEntryCode(actor, D_00360E98[i]) > 0) {
                return 1;
            }
        }
    } else if (kind == 0xC) {
        for (j = 1; j < 10; j += 2) {
            if (btlActorEntryIsExpired(actor, D_00360E98[j]) != 0 && btlGetActorEntryCode(actor, D_00360E98[j]) <= 0) {
                return 1;
            }
        }
    } else if (btlActorEntryIsExpired(actor, D_00360E98[kind]) != 0 || kind == 0xA || kind == 0xD) {
        if ((kind & 1) == 0 || kind == 0xD) {
            if (btlGetActorEntryCode(actor, D_00360E98[kind]) > 0) {
                return 1;
            }
        } else {
            if (btlGetActorEntryCode(actor, D_00360E98[kind]) <= 0) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002007A8);

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

extern const s32 D_003A5A08[10];

s32 btlUnitHasAllTenActions(void *actor) {
    s32 actions[10];
    s32 i;
    memcpy(actions, D_003A5A08, sizeof(actions));
    for (i = 0; i < 10; i++) {
        if (btlTestSelectedItemCategoryMask(actor, actions[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 func_00200A08(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (func_00200628((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00200A88(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (func_00200628((s32)unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

u8 btlCheckUnitActionModeZero(u32 unit, u32 action) {
    s32 result;

    result = func_002007A8(unit, action, 0);
    return result != 0;
}

u8 btlCheckUnitActionModeOne(u32 unit, u32 action) {
    s32 result;

    result = func_002007A8(unit, action, 1);
    return result != 0;
}

s32 func_00200B48(s32 unused, u32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (func_002007A8((u32)unit, action, 0) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00200BD0(s32 unused, u32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (func_002007A8((u32)unit, action, 1) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00200C58(s32 unused, u32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (func_002007A8((u32)unit, action, 0) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00200CE0(s32 unused, u32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (func_002007A8((u32)unit, action, 1) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00200D68(s32 unused, u32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (func_002007A8((u32)unit, action, 0) == 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00200DF0(s32 unused, u32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (func_002007A8((u32)unit, action, 0) == 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyGroup200LacksFlag1000(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->flags & 0x1000) == 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

u8 btlUnitPassesActionTenCheck(u32 unit) {
    s32 result;

    result = func_00200628(unit, 10);
    return result != 0;
}

s32 func_00200F00(void) {
    return func_00200628() != 0;
}

s32 func_00200F20(s32 battler) {
    u32 flags;

    if (((BtlUnit *)battler)->flags & 0x200) {
        return 0;
    }
    flags = ((BtlUnit *)battler)->unk_120 & 0x2000;
    return flags != 0;
}

s32 btlHasContextFlagTwo(void) {
    return ((*(s32 *)(*(s32 *)D_003BB87C + 0xc) & 2) > 0);
}

s64 btlCheckCounterLimit(s32 unused, u32 limit) {
    return btlCounterReachedLimit(unused, limit);
}

s32 btlCounterReachedLimit(s32 unused, u32 limit) {
    if (func_001A9488(4) < limit) {
        return 0;
    }
    return 1;
}

typedef struct BtlCounterState {
    u8 unk_00[0x88];
    u16 count; /* 0x88: saturates at 0xFF */
} BtlCounterState;

s32 btlAiCounterReachedLimit(s32 unused, u32 limit) {
    BtlCounterState *state = *(BtlCounterState **)D_003BB87C;
    state->count++;
    state->count = state->count <= 0 ? 0 : state->count >= 0x100 ? 0xFF : state->count;
    if ((*(BtlCounterState **)D_003BB87C)->count < limit) {
        return 0;
    }
    return 1;
}

s32 btlTurnReachedLimit(s32 unused, u32 limit) {
    s32 *battle = (s32 *)func_001A17F0();
    if ((u32)battle[0x250 / 4] < limit) {
        return 0;
    }
    return 1;
}

s32 btlIsReadyWithoutTurns(void) {
    u8 *battle = (u8 *)func_001A17F0();
    if (((BtlState *)battle)->turnPhase == 2) {
        if (((BtlState *)battle)->turnCount == 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_001A1800(void *);

extern s32 func_001A1850(void *);

/* Compare a unit stat with a percentage of its maximum.
 * The stat's identity is not established by these two accessors. */
s32 btlIsUnitStatAtOrBelowRate(BtlUnit *unit, s32 percentage) {
    void *stats = &unit->unk_120;
    u32 current = func_001A1800(stats);
    u32 scaledMaximum = func_001A1850(stats) * percentage;
    if (scaledMaximum < current * 100) {
        return 0;
    }
    return 1;
}

u8 func_002010D8(void) {
    s32 result;

    result = btlHasAvailableOption();
    return result != 0;
}

extern void func_001A30F8(s32, void *, s32, s32, s32);

extern s32 btlGetIndexListEntry(void *, u32);

extern void btlFreeIndexList(void *);

s32 btlAnyIndexedUnitPassesQuery(u8 *unit) {
    u32 i;
    u32 count;
    s32 battle;
    void *list;
    if (((BtlUnit *)unit)->flags & 0x400) {
        return 0;
    }
    battle = *(s32 *)D_003BB87C;
    list = btlAllocateIndexList(13);
    func_001A30F8(battle, list, 2, 0, 0);
    count = btlGetIndexListCount(list);
    for (i = 0; i < count; i++) {
        if (btlAreUnitStatusAndEntryFlagsClear(btlGetIndexListEntry(list, i)) != 0) {
            btlFreeIndexList(list);
            return 1;
        }
    }
    btlFreeIndexList(list);
    return 0;
}

s32 btlIsLowHpActionReady(BtlUnit *unit) {
    s32 battle = *D_003BB87C;
    s32 hit = 0;
    u32 base = unit->actionTime;
    u32 now = func_001A9488(4);
    s16 roll;

    if (*(s8 *)(battle + 0x146) <= 0) {
        if (now >= base + 0xF) {
            if (unit->hp * 100 / unit->maxHp < 0x1E) {
                if (btlIsGroup400CountAtMost((s32)unit, 2) != 0) {
                    roll = btlRollAiBucket();
                    hit = roll < 0x1E;
                }
            }
        }
    }
    if (hit == 1) {
        if (btlIsSelectedActorStatusAndRecordClear(unit) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlUnitHasFlag1000(s32 unit) {
    return (((s32)((BtlUnit *)unit)->flags & 0x1000) > 0);
}

u8 func_002012C0(void) {
    s32 result;

    result = func_001A2B00();
    return result == 0;
}

u8 func_002012E0(void) {
    s32 result;

    result = btlAreUnitStatusAndEntryFlagsClear();
    return result != 0;
}

extern const s32 D_003A5A30[20];

s32 btlElementToBitIndex(s32 mask, s32 index) {
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

extern s32 func_001A2F50();

s32 btlUnitHasNegativeActionQueryResult(void *unit, s32 mask) {
    s32 i;
    s32 value;
    if (mask & 0x100000) {
        for (i = 0; i < 0x13; i++) {
            value = btlElementToBitIndex(mask, i);
            if (value == 0x80) {
                continue;
            }
            value = func_001A2F50(unit, value);
            if ((u16)value < 100) {
                continue;
            }
            if ((u32)value & 0x80000000) {
                return 1;
            }
        }
        return 0;
    }
    value = func_001A2F50(unit, mask);
    if ((u16)value >= 100) {
        if (value < 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlUnitHasNegativeActionQueryResult(void *, s32);

s32 func_002014E8(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x401) == 0x401) {
            if (btlUnitHasNegativeActionQueryResult(unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201568(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x201) == 0x201) {
            if (btlUnitHasNegativeActionQueryResult(unit, action) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlHasGroup200DifferentUnitMode(s32 unused, s32 kind) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->mode != kind) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlHasOtherGroup400DifferentUnitMode(BtlUnit *actor, s32 kind) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (((BtlUnit *)unit)->mode != kind) {
                if (unit->identity != actor->identity) {
                    return 1;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyUnitPassesCheck200(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 0) {
            return 1;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201748(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
            return 0;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 1;
}

s32 btlAnyUnitPassesCheck400(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 0) {
            return 1;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201828(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 1) {
            return 0;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 1;
}

extern s32 btlWouldUiValueFallBelowQuarter(s32, s32);

extern u8 D_003BD476;

s32 btlCheckActorEligibilityWithDebug(s32 actor) {
    if (btlWouldUiValueFallBelowQuarter(actor, 0) != 0) {
        if (D_003BD476 == 0) {
            btlBossDebugPrintf(D_003A5A80);
        }
        return 1;
    }
    if (D_003BD476 == 0) {
        btlBossDebugPrintf(D_003A5AA8);
    }
    return 0;
}

extern s32 func_00201900(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201900);

/* Action IDs occupy the low halfword of each 0x4-byte queued action slot. */
typedef union BtlActionSlot {
    s32 word;
    s16 actionId;
} BtlActionSlot;

typedef struct BtlActionTask {
    u8 pad00[0x148];
    BtlActionSlot actions[8]; /* 0x148 */
    u8 pad168[4];
    struct BtlActionTask *next; /* 0x16C, same link as BtlTask */
} BtlActionTask;

s32 btlAnyUnitHasActionInSlots(mask, action)
u32 mask;
s32 action;
{
    u8 *node = (u8 *)((BtlState *)func_001A17F0())->tasks;
    while (node != 0) {
        u8 *owner = (u8 *)((BtlTask *)node)->unit;
        if (owner != 0) {
            u32 flags = ((BtlUnit *)owner)->flags;
            if ((flags & 1) && (flags & mask) && !(flags & 0x20)) {
                s32 i;
                for (i = 0; i < 8; i++) {
                    /* Required to match: byte-offset indexing keeps the original induction register. */
                    if (func_00201900(action, *(s16 *)(node + 0x148 + i * 4), 1) != 0) {
                        return 1;
                    }
                }
            }
        }
        node = (u8 *)((BtlTask *)node)->next;
    }
    return 0;
}

s64 btlAnyGroup200HasAction(s32 unused, s32 action) {
    return btlAnyUnitHasActionInSlots(0x200, action);
}

s64 btlAnyGroup400HasAction(s32 unused, s32 action) {
    return btlAnyUnitHasActionInSlots(0x400, action);
}

s32 func_00201B50(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (func_00201900(action, ((BtlUnit *)unit)->actionSlot, 0) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201BD8(s32 unused, s32 action) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (func_00201900(action, ((BtlUnit *)unit)->actionSlot, 0) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201C60(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (btlUnitHasAllTenActions(unit) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00201CD0(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
            if (btlUnitHasAllTenActions(unit) != 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAreUnitsMissingStatusFlag(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->flags & 0x1000) != 0) {
                return 0;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 1;
}

s32 btlAreUnitsHoldingStatusFlag(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if ((((BtlUnit *)unit)->flags & 0x1000) == 0) {
                return 0;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 1;
}

s32 func_00201E10(void) {
    return D_003BB870 < 1;
}

extern s32 btlUnitBlocksElementQuery(s32, s32, s32);

s32 btlAnyUnitBlocksGroup200Element(s32 unused, s32 action) {
    s32 unit = (s32)((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if (btlUnitBlocksElementQuery(unit, action, 0x200) != 0) {
            return 1;
        }
        unit = (s32)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 btlAnyGroupUnitHasZeroStat(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
            if (((BtlUnit *)unit)->unk_12A == 0) {
                return 1;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s64 btlAnyGroup400HasQuery(s32 unused, s32 query, u32 unused2) {
    return btlAnyUnitHasQueuedQuery(unused, query, 0x400);
}

s64 btlAnyGroup200HasQuery(s32 unused, s32 query, u32 unused2) {
    return btlAnyUnitHasQueuedQuery(unused, query, 0x200);
}

s32 btlAnyUnitHasQueuedQuery(s32 unused, s32 query, u32 mask) {
    u8 *node = (u8 *)((BtlState *)func_001A17F0())->tasks;
    while (node != 0) {
        u8 *owner = (u8 *)((BtlTask *)node)->unit;
        if (owner != 0) {
            u32 flags = ((BtlUnit *)owner)->flags;
            if ((flags & 1) && (flags & mask) && !(flags & 0x20)) {
                s32 i;
                for (i = 0; i < 8; i++) {
                    if (((BtlActionTask *)node)->actions[i].word == query) {
                        return 1;
                    }
                }
            }
        }
        node = (u8 *)((BtlTask *)node)->next;
    }
    return 0;
}

extern s32 btlHasEnabledSpecialAbilityForSlot(void *, s32);

extern s32 fldGetSelectedUnitStat();

s32 btlUnitBlocksElementQuery(s32 unit, s32 action, s32 mask) {
    u32 flags = ((BtlUnit *)unit)->flags;
    s32 stat;
    s32 value;
    if (flags & 1) {
        if (flags & mask) {
            if (!(flags & 0x20)) {
                stat = fldGetSelectedUnitStat();
                if (action & 0x100000) {
                    s32 i;
                    for (i = 0; i < 19; i++) {
                        s32 index = btlElementToBitIndex(action, i);
                        if (index == 0x80) {
                            continue;
                        }
                        value = func_001A2F50((void *)unit, index);
                        if (btlHasEnabledSpecialAbilityForSlot((void *)unit, index) != 0 || (value & 0x20000) ||
                            (stat == 0x20000 && btlTestSelectedItemCategoryMask((void *)unit, index) != 0)) {
                            return 1;
                        }
                    }
                    return 0;
                }
                value = func_001A2F50((void *)unit, action);
                if (btlHasEnabledSpecialAbilityForSlot((void *)unit, action) != 0 || (value & 0x20000) ||
                    (stat == 0x20000 && btlTestSelectedItemCategoryMask((void *)unit, action) != 0)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s64 func_00202158(void) {
    return func_001A8CE0(0);
}

extern s32 btlTestSelectedItemCategoryMask(void *, s32);

extern s32 btlHasMappedSpecialAbilityForSlot(void *, s32);

extern s32 btlHasSpecialAbilityWhenArgumentUnset(void *, s32);

extern s32 btlHasEnabledSpecialAbilityForSlot(void *, s32);

s32 btlUnitBlocksElementQueryForGroup(u8 *unit, s32 action, u32 mask) {
    u32 flags = ((BtlUnit *)unit)->flags;
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
                            btlHasSpecialAbilityWhenArgumentUnset(unit, index) != 0 ||
                            btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (btlTestSelectedItemCategoryMask(unit, action) != 0 ||
                    btlHasMappedSpecialAbilityForSlot(unit, action) != 0 ||
                    btlHasSpecialAbilityWhenArgumentUnset(unit, action) != 0) {
                    return 0;
                }
                return btlHasEnabledSpecialAbilityForSlot(unit, action) == 0;
            }
        }
    }
    return 2;
}

s32 btlAllUnitsPassCheck200(s32 unused, s32 action) {
    s32 node = (s32)((BtlState *)func_001A17F0())->units;
    while (node != 0) {
        if (btlUnitBlocksElementQueryForGroup(node, action, 0x200) == 0) {
            return 0;
        }
        node = (s32)((BtlUnit *)node)->next;
    }
    return 1;
}

s32 btlAllUnitsPassCheck400(s32 unused, s32 action) {
    s32 node = (s32)((BtlState *)func_001A17F0())->units;
    while (node != 0) {
        if (btlUnitBlocksElementQueryForGroup(node, action, 0x400) == 0) {
            return 0;
        }
        node = (s32)((BtlUnit *)node)->next;
    }
    return 1;
}

extern s32 btlGroup400UnitHasAction(void *, s32);

s32 btlUnitHasEitherSpecialAction(void *unit) {
    if (btlGroup400UnitHasAction(unit, 0x1b2) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1b6) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1ba) != 0 ||
        btlGroup400UnitHasAction(unit, 0x1be) != 0) {
        return 1;
    }
    return btlGroup400UnitHasAction(unit, 0x1c2) != 0;
}

s32 btlActionMatchesUnit(u8 *unit, s32 action) {
    s32 *battle = (s32 *)*D_003BB87C;
    if (battle[0x148 / 4] == action) {
        if (((BtlUnit *)unit)->unk_114 & 0x1000) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_001A8A30(void *, s32);

s32 btlGroup400UnitHasAction(void *unit, s32 action) {
    func_001A17F0();
    if ((((BtlUnitStatus *)unit)->combinedFlags & 0x421) == 0x401) {
        if (func_001A8A30(unit, action) != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002024A8);

s32 btlBuildActorIndexListAndCount(s32 actor, u32 *matchingCount, u32 *listCount) {
    u32 result;
    s32 list;

    list = btlAllocateIndexList(0xd);
    result = func_001A3360(actor, list, 0);
    *matchingCount = result;
    result = btlGetIndexListCount(list);
    *listCount = result;
    return list;
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

/* Picks the target with the lowest nonzero health value among units that block the
 * element query; when none qualifies, every listed unit is a candidate. */
s32 func_00203098(s32 actor, s32 action) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
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
            s32 unit = btlGetIndexListEntry((void *)list, i);
            if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
                u16 current = func_001A17F8(&((BtlUnit *)unit)->unk_120);
                if (best >= current && current != 0) {
                    best = current;
                    found++;
                    bestIndex = i;
                }
            }
        }
        if (found != 0) {
            btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, btlGetIndexListEntry((void *)list, bestIndex));
        } else {
            u32 n = count;
            for (i = 0; i < n; i++) {
                flags[i] = 1;
            }
            btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        }
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectLowestHealthRateTarget(s32 actor) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    s32 best;
    u32 bestIndex;
    u32 i;
    s32 result;

    switch (matching) {
    case 0:
        best = 0x63;
        memset(flags, 0, sizeof(flags));
        bestIndex = 0x20;
        for (i = 0; i < count; i++) {
            void *stats = &((BtlUnit *)btlGetIndexListEntry((void *)list, i))->unk_120;
            s32 current = func_001A17F8(stats);
            s32 percent = current * 100 / func_001A1838(stats);

            if (best >= percent && current != 0) {
                best = percent;
                bestIndex = i;
            }
        }
        if (bestIndex != 0x20) {
            result = btlGetIndexListEntry((void *)list, bestIndex);
        } else {
            result = func_002024A8(list, flags, count);
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, result);
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectTargetsByActionMask(s32 actor, s32 mask) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (btlUnitHasActionMask(btlGetIndexListEntry((void *)list, i), mask) != 0) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 btlSelectTargetsWithoutActionMask(s32 actor, s32 mask) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (btlUnitHasActionMask(btlGetIndexListEntry((void *)list, i), mask) == 0) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

s32 func_002035E0(s32 actor, s32 mode) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)btlGetIndexListEntry((void *)list, i))->mode == mode) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002036E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203A80);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203BA8);

s32 func_00203CA8(s32 actor) {
    void *list = btlAllocateIndexList(13);
    u32 mode;
    u32 count;
    u16 picked[12];
    u32 i;
    s32 found;

    switch (((BtlTask *)actor)->unit->mode) {
    case 3:
    case 5:
    case 6:
        mode = (((BtlTask *)actor)->unit->flags & 0x1000) ? 0 : 2;
        break;
    default:
        mode = 0;
        break;
    }
    func_001A30F8(actor, list, 1, 2, 0);
    count = btlGetIndexListCount(list);
    if (count == 0) {
        mode = 0;
    }
    switch (mode) {
    case 0:
        found = 0;
        memset(picked, 0, sizeof(picked));
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)btlGetIndexListEntry(list, i))->identity != ((BtlTask *)actor)->unit->identity) {
                picked[found] = 1;
                found++;
            }
        }
        if (found == 0) {
            btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, (s32)((BtlTask *)actor)->unit);
            btlFreeIndexList(list);
            return 1;
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8((s32)list, picked, count));
        btlFreeIndexList(list);
        return 1;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, (s32)list);
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203E38);

s32 btlAppendSelfAfterTargetScan(s32 actor) {
    void *list = btlAllocateIndexList(13);
    func_001A30F8(actor, list, 1, 1, 0);
    btlGetIndexListCount(list);
    btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, (s32)((BtlTask *)actor)->unit);
    btlFreeIndexList(list);
    return 1;
}

u32 func_00204008(u32 task, u32 input) {
    btlSelectLinkedTargets(task, input, 1);
    return 1;
}

u32 func_00204028(u32 task, u32 input) {
    btlSelectLinkedTargets(task, input, 0);
    return 1;
}

u32 btlAppendEffectActorToCommandIndices(s32 task) {
    u32 actor;

    actor = btlGetEffectActor();
    btlAppendIndexListEntry(((BtlTask *)task)->unk_60, actor);
    return 1;
}

s32 btlSelectTargetsBlockingElement(s32 actor, s32 mask) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            if (btlUnitBlocksElementQuery(btlGetIndexListEntry((void *)list, i), mask, 0x200) != 0) {
                flags[i] = 1;
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

u32 func_00204198(void) {
    return 1;
}

s32 btlSelectTargetsPassingCheck(s32 actor, s32 action) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            s32 unit = btlGetIndexListEntry((void *)list, i);

            if (((BtlUnit *)unit)->flags & 0x200) {
                if (btlUnitBlocksElementQueryForGroup(unit, action, 0x200) == 1) {
                    flags[i] = 1;
                }
            } else {
                if (btlUnitBlocksElementQueryForGroup(unit, action, 0x400) == 1) {
                    flags[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
    return 1;
}

void btlSelectLinkedTargets(s32 actor, s32 input, s8 invert) {
    u32 matching;
    u32 count;
    u16 flags[12];
    s32 list = btlBuildActorIndexListAndCount(actor, &matching, &count);
    u16 i;

    switch (matching) {
    case 0:
        memset(flags, 0, sizeof(flags));
        for (i = 0; i < count; i++) {
            BtlUnit *unit = (BtlUnit *)btlGetIndexListEntry((void *)list, i);

            if ((((BtlUnitStatus *)unit)->combinedFlags & 0x221) == 0x201) {
                if (invert == 0) {
                    if (unit->flags & 0x1000) {
                        flags[i] = 1;
                    }
                } else if (!(unit->flags & 0x1000)) {
                    flags[i] = 1;
                }
            }
        }
        btlAppendIndexListEntry(((BtlTask *)actor)->unk_60, func_002024A8(list, flags, count));
        break;
    case 1:
    case 2:
        btlCopyIndexList(((BtlTask *)actor)->unk_60, list);
        break;
    }
    btlFreeIndexList((void *)list);
}

extern s32 btlCanUseLinkedActor();

/* Guard clauses: each `return unit->task->unit` fallback is its own exit. */
BtlUnit *btlGetTargetUnitForLink(BtlUnit *unit) {
    s32 kind = unit->unk_114;

    if ((u32)(kind - 1) >= 0x25F) {
        return unit->task->unit;
    }
    if (D_003BAA4C[kind * 2 + 1] != 1) {
        return unit->task->unit;
    }
    if (unit->linkedA == NULL && unit->linkedB == NULL) {
        return unit->task->unit;
    }
    if (btlCanUseLinkedActor(unit) == 0) {
        return unit->task->unit;
    }
    if (unit->linkedA != NULL) {
        if (unit->linkedB != NULL) {
            return unit->task->unit;
        }
        if (!(unit->linkedA->flags & 0x1000)) {
            return unit->linkedA;
        }
    }
    if (unit->linkedB != NULL) {
        if (!(unit->linkedB->flags & 0x1000)) {
            return unit->linkedB;
        }
    }
    return unit->task->unit;
}

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

/* Linked-command target/direction and motion fields; DDS2 moves the target/flags +0x20. */
typedef struct BtlLinkedCommand {
    u8 pad00[0x50];
    f32 coordinate50; /* 0x50: increased when motion begins */
    u8 pad54[0x8C];
    f32 coordinateE0; /* 0xE0: increased along with coordinate50 */
    u8 padE4[0xC];
    u32 flags;         /* 0xF0 */
    u8 padF4[0x24];
    u32 targetList;    /* 0x118 */
    u8 pad11C[0x14];
    f32 value130;      /* 0x130: initialized to 30 for this motion */
} BtlLinkedCommand;

void btlFaceLinkedTargetAndFlagDirection(u8 *command, u8 *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = btlGetTargetUnitForLink((BtlUnit *)command);
    target = (BtlUnit *)btlGetIndexListEntry(((BtlLinkedCommand *)command)->targetList, 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagMatchingUnitsDefeatCandidate(target->flags & 0x600);
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
        ((BtlLinkedCommand *)command)->flags |= 0x200;
    } else {
        ((BtlLinkedCommand *)command)->flags &= ~0x200;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002045E8);

typedef struct BtlCamState {
    f32 position[4];  /* 0x00 */
    f32 direction[4]; /* 0x10 */
    f32 distance;     /* 0x20 */
    f32 fov;          /* 0x24 */
} BtlCamState;

extern void btlFlagAllUnitsDefeatCandidate(void);
extern void btlCopyMotionTransform();
extern void btlUnitFaceTarget(BtlUnit *, BtlUnit *);
extern void func_001DB698();
extern f32 func_002FA148(f32);

/* Frame a two-unit exchange: place the camera pair between the units' muzzle positions and push it out far enough to see both. */
void func_00204838(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back, s8 mirror, s8 swapRoles, f32 sideScale, f32 backLift, f32 frontLift) {
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
    front->fov = ((f32 *)command)[9];
    if (swapRoles == 0) {
        user = btlGetTargetUnitForLink((BtlUnit *)command);
        target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    } else {
        target = btlGetTargetUnitForLink((BtlUnit *)command);
        user = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    }
    userExtent = user->positionB4 * user->scale80;
    targetExtent = target->positionB4 * target->scale80;
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    userPos[1] += user->positionB0 * user->scale80 * frontLift;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, targetPos);
    targetPos[1] += target->positionB0 * target->scale80 * backLift;
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
    front->distance = front->distance + targetExtent * 1.5f / func_002FA148(angle);
    minDistance = userExtent / func_002FA148(angle);
    if (front->distance < minDistance) {
        front->distance = minDistance;
    }
    btlCopyMotionTransform(back, front);
    back->distance += 250.0f;
    if (mirror != 0) {
        func_001DB698(front);
        func_001DB698(back);
    }
    btlUnitFaceTarget(user, target);
    btlUnitFaceTarget(target, user);
}

u32 func_00204AC0(void) {
    return 0xffffffff;
}

s32 func_00204AC8(s32 unused, s32 unit, s32 index) {
    s32 result = 0;
    if (!(((BtlUnit *)unit)->flags & 0x400)) {
        return result;
    }
    if (D_003BAA50[index * 0x38 + 2] == 2) {
        if (btlWouldUiValueFallBelowQuarter(unit, 0) != 0 && mdlFlagTest(0x802) != 0) {
            return 1;
        }
        return 4;
    }
    return 0;
}

u32 func_00204B40(void) {
    return 0xffffffff;
}

s32 btlFilterRestrictedCommand(s32 battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((((BtlUnit *)battler)->unk_120 & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

u8 btlIsCommandCodeF(u32 unused, s32 command) {
    return command == 0xf;
}

s32 btlSelectDisabledCommand(s32 battler) {
    if (battler == 0) {
        return 15;
    }
    return (((BtlUnit *)battler)->unk_120 & 0x2000) ? 15 : -1;
}

/* Command condition words and the restriction flags tested by this boss mode. */
typedef struct BtlCommandStatus {
    s32 first;
    s32 second;
    s32 third;
    u8 pad0C[0x1A];
    u16 restrictionFlags; /* 0x26 */
} BtlCommandStatus;

void btlTrackSpecialEnemyCommandRestrictionByTurn(u8 *unit, u8 *command) {
    u8 *battle;
    u8 *effect;
    s32 species;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    battle = (u8 *)func_001A17F0();
    species = ((BtlUnit *)unit)->mode;
    effect = (u8 *)((BtlState *)battle)->effect;
    if (species < 0x105) {
        if (species >= 0x102 && (((BtlCommandStatus *)command)->restrictionFlags & 0x10)) {
            ((BtlUnit *)unit)->unk_120 |= 0x2000;
            *(s32 *)effect = ((BtlState *)battle)->turnCount;
        }
    }
    if (species == 0x102 && (((BtlCommandStatus *)command)->restrictionFlags & 0x100)) {
        if (((BtlCommandStatus *)command)->first < 0 || ((BtlCommandStatus *)command)->second < 0 ||
            ((BtlCommandStatus *)command)->third != 0) {
            if (*(s32 *)effect != ((BtlState *)battle)->turnCount) {
                ((BtlUnit *)unit)->unk_120 &= ~0x2000;
            }
        }
    }
}

s32 btlClearUnitRestrictionFlag(void) {
    BtlState *battle = (BtlState *)func_001A17F0();
    BtlUnit *unit;
    if (battle->mode != 2) {
        return -1;
    }
    unit = battle->units;
    while (unit != 0) {
        if (unit->flags & 1) {
            if (unit->mode == 0x104) {
                u16 entryFlags = unit->unk_120;
                if (entryFlags & 0x2000) {
                    unit->unk_120 = entryFlags & ~0x2000;
                }
            }
        }
        unit = unit->next;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204D08);

s32 btlDisableNonBossUnits(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    s32 kind;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                kind = unit->mode;
                if (kind < 0x105) {
                    if (kind >= 0x102) {
                        if (unit->flags & 0x20) {
                            result = 1;
                            break;
                        }
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
                            switch (unit->mode) {
                            case 0x102:
                            case 0x103:
                            case 0x104:
                                break;
                            default:
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->flags &= ~1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

void btlResetUnitPlacement(void) {
    u8 *unit = *(u8 **)((u8 *)func_001A17F0() + 0x228);

    if (unit == 0) {
        return;
    }
    do {
        if ((((BtlUnit *)unit)->flags & 1) != 0) {
            s32 species = ((BtlUnit *)unit)->mode;
            if (species < 0x105) {
                if (species >= 0x102) {
                    if (((BtlUnit *)unit)->unk_120 & 0x2000) {
                        ((BtlUnit *)unit)->position90 = 0;
                        ((BtlUnit *)unit)->position94 = -100.0f;
                        ((BtlUnit *)unit)->position98 = 60.0f;
                        ((BtlUnit *)unit)->position9C = 0;
                        ((BtlUnit *)unit)->positionB4 = 180.0f;
                        ((BtlUnit *)unit)->positionB0 = 220.0f;
                    } else {
                        btlInitializeEffectVectorsFromSourceRecords(unit, 1, species);
                    }
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    } while (unit != 0);
}

extern void func_001D5990(void *);

void btlClearSpecialEnemyEntryFlags(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                s32 species = ((BtlUnit *)unit)->mode;
                if (species >= 0x105) {
                    unit = (u8 *)((BtlUnit *)unit)->next;
                    continue;
                }
                if (species >= 0x102) {
                    ((BtlUnit *)unit)->unk_120 &= ~0x2000;
                    func_001D5990(unit);
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
}

void func_00205070(void) {
    BtlUnit *target = NULL;
    BtlState *state = (BtlState *)func_001A17F0();
    BtlUnit *unit;
    f32 position[4];
    f32 shift;

    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x105) {
                    target = unit;
                    break;
                }
            }
        }
    }
    if (target != NULL) {
        func_001D6300(target, position);
        shift = -position[0];
        position[0] = 0.0f;
        PCP_COPY_VECTOR(target->position, position);
        btlSetUnitPosition(target, position);
        for (unit = state->units; unit != NULL; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit != target) {
                        func_001D6300(unit, position);
                        position[0] += shift;
                        PCP_COPY_VECTOR(unit->position, position);
                        btlSetUnitPosition(unit, position);
                    }
                }
            }
        }
    }
}

s32 btlDisableUnitsIfSpeciesFlagged(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x105) {
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
                            if (unit->mode != 0x105) {
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

typedef struct BtlSlotEntry {
    u16 flags;
    u8 unk_02[2];
    u16 kind;
    u16 unk_06;
    u16 unk_08;
    u8 unk_0A[4];
    u16 unk_0E;
    u8 unk_10[0x12];
    u16 data[24];
    u8 unk_52[0x1A4 - 0x52];
} BtlSlotEntry;

typedef struct BtlSlotTable {
    u8 unk_000[0xA60];
    BtlSlotEntry entry[5];
} BtlSlotTable;

extern BtlSlotTable *D_003BAA00;

void btlSelectSlotEntries(void) {
    s32 *effect = *(s32 **)((u8 *)func_001A17F0() + 0x694);
    BtlSlotEntry *entry;
    BtlSlotEntry *ready = NULL;
    BtlSlotEntry *active = NULL;
    u32 i;
    entry = D_003BAA00->entry;
    for (i = 0; i < 5; i++) {
        u16 flags = entry->flags;
        effect[i] = flags;
        if (flags & 1) {
            if (entry->kind == 3) {
                ready = entry;
            }
            if (entry->kind == 1) {
                entry->flags = flags | 2;
                active = entry;
            } else {
                entry->flags = flags & ~2;
            }
        }
        entry++;
    }
    active->unk_0E = 0;
    if (ready != NULL) {
        ready->unk_0E = 0;
    }
    memcpy((u8 *)effect + 0x14, active->data, 0x30);
    for (i = 0; i < 24; i++) {
        active->data[i] = 0;
    }
    active->unk_06 = active->unk_08;
}

void func_00205420(void) {
    s32 *effect = *(s32 **)((u8 *)func_001A17F0() + 0x694);
    BtlSlotEntry *selected = NULL;
    u32 i;

    for (i = 0; i < 5; i++) {
        if (effect[i] & 1) {
            if (effect[i] & 2) {
                D_003BAA00->entry[i].flags |= 2;
            } else {
                D_003BAA00->entry[i].flags &= ~2;
            }
            if (D_003BAA00->entry[i].kind == 1) {
                selected = &D_003BAA00->entry[i];
            }
        }
    }
    memcpy(selected->data, (u8 *)effect + 0x14, 0x30);
    selected->unk_06 = selected->unk_08;
}

void btlInitializeUnitDisplaySpeciesAndFlags(u8 *unit) {
    u8 *battle = (u8 *)func_001A17F0();
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 0x200) {
        if (((BtlUnit *)unit)->mode == 1) {
            ((BtlUnit *)unit)->displaySpecies = 1;
            ((BtlUnit *)unit)->flags = flags | 0x1000;
            ((BtlUnit *)unit)->unk_120 |= 0x1000;
            if (((BtlState *)battle)->battleMode == 0x107) {
                ((BtlUnit *)unit)->unk_114 |= 0x200;
            }
        }
    } else {
        ((BtlUnit *)unit)->flags = flags | 0x1000;
        ((BtlUnit *)unit)->displaySpecies = 0x132;
    }
    ((BtlUnit *)unit)->unk_114 |= 0x20;
}

void btlNormalizeDefaultPlayerDisplaySpecies(s32 unit) {
    u16 mode;

    if (((BtlUnit *)unit)->flags & 0x200) {
        mode = ((BtlUnit *)unit)->mode;
        if (mode == 1) {
            if (((BtlUnit *)unit)->displaySpecies == mode) {
                ((BtlUnit *)unit)->displaySpecies = 0x11;
            }
        }
    }
}

u32 func_002055F0(void) {
    return 2;
}

extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);

extern void mdlFlagSet(u32);

extern void mdlFlagClear(u32);

s32 func_002055F8(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        if (((BtlUnit *)unit)->flags & 1) {
            if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0) {
                if (((BtlUnit *)unit)->flags & 0x200) {
                    mdlFlagSet(0x811);
                } else {
                    mdlFlagClear(0x811);
                }
                return 6;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return -1;
}

s32 btlInitializeResources(s32 unused, s32 resource) {
    btlInitMotionTransformFromVectors(resource, D_00360EE0, D_00360EF0);
    return 1;
}

s64 btlInitResourcesWrap(s32 unused, s32 resource) {
    return btlInitializeResources(unused, resource);
}

void func_002056E0(BtlUnit *unit) {
    BtlState *battle = (BtlState *)func_001A17F0();
    u32 flags = unit->flags & ~0x100;
    u16 status = unit->unk_12E;
    flags &= ~8;
    status &= 0x4000;
    *(BtlUnit **)battle->effect = unit;
    unit->flags = flags;
    unit->unk_12E = status;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205730);

void btlBeginEffectActorFadeOut(void) {
    BattleEffectState *effect = ((BtlState *)func_001A17F0())->effect;
    BtlUnit *actor = *(BtlUnit **)effect;
    if (actor != 0) {
        u32 state = actor->unk_114;
        u32 flags = actor->flags;
        state &= ~0x80;
        state &= ~0x100;
        flags |= 0x100;
        *(BtlUnit **)effect = 0;
        actor->flags = flags;
        actor->unk_114 = state;
        func_001D5990(actor);
        actor->flags |= 8;
        /* +0x10 is a float in this effect variant, but a u32 in BattleEffectState. */
        *(f32 *)((u8 *)effect + 0x10) = -125.0f;
        effect->speed = 20.0f;
    }
}

void btlResetEffectState(void) {
    u8 *battle = (u8 *)func_001A17F0();
    BattleEffectState *data = ((BtlState *)battle)->effect;
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

s32 btlCheckActiveEffectForSpecialTarget(BtlUnit *actor, BtlUnit *target, s32 command, s32 bits) {
    BattleEffectState *effect;
    if (!(target->flags & 0x400)) {
        return 0;
    }
    switch (target->mode) {
    case 0x107:
    case 0x108:
        break;
    default:
        return 0;
    }
    effect = *(BattleEffectState **)(func_001A17F0() + 0x694);
    if (effect->active != 1) {
        return 0;
    }
    if (actor->flags & 0x200) {
        if (command != 0) {
            if (D_003BAA50[command * 0x38 + 8] == 0) {
                return 0;
            }
        }
    }
    return (bits * 2) & 4;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205BD8);

void func_00205EE0(void) {
    func_00205BD8();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205EF8);

s32 btlIsEffectPhaseInRange(s32 unused, s32 value) {
    BattleEffectState *effect = *(BattleEffectState **)(func_001A17F0() + 0x694);
    if (effect->active != 1) {
        return 0;
    }
    switch (value) {
    case 0x12:
    case 0x13:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206180);

s32 btlAdjustDamageKind(BtlUnit *unit, s32 damageKind) {
    u32 id;
    if (!(unit->flags & 1)) {
        return damageKind;
    }
    if (!(unit->flags & 0x400)) {
        return damageKind;
    }
    if ((*(BattleEffectState **)(func_001A17F0() + 0x694))->active != 1) {
        return damageKind;
    }
    id = unit->mode;
    if (id == 0x124) {
        return damageKind;
    }
    switch (damageKind) {
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
            return damageKind + 0x64;
        }
        if (id == 0x108) {
            return damageKind + 0xC8;
        }
        break;
    }
    return damageKind;
}

u8 *btlFindFlaggedSpecialSpeciesUnit(s32 category, s32 species) {
    u8 *unit;
    if (category != 1) {
        return 0;
    }
    if (species != 0x124) {
        return 0;
    }
    unit = (u8 *)((BtlState *)func_001A17F0())->units;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 2) {
                    if (((BtlUnit *)unit)->species == 0x124) {
                        return unit;
                    }
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return 0;
}

s32 func_00206418(s32 unit) {
    s32 mode = ((BtlUnit *)unit)->mode;

    if ((mode >= 0x107) && ((mode < 0x109) || (mode == 0x124))) {
        return 0x124;
    }
    return ((BtlUnit *)unit)->displaySpecies;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206450);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206608);

s32 btlGetEffectTaskActorMatchCode(u8 *task) {
    BattleEffectState *effect;
    if ((((BtlTask *)task)->flags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)func_001A17F0())->effect;
    return effect->actor == (u32)((BtlTask *)task)->unit ? 12 : -1;
}

extern s32 btlCreateCommandSoundUpdateTask();

extern s32 btlCreateSecondaryCommandSoundTask();

extern s32 btlCreateCommandSoundTask();

extern s32 btlCreateEffObjB();

extern u8 *fldCreateSceneGroupAction(BtlTask *, u32, s32);

s32 btlEffectTaskStartFinale(BtlTask *task) {
    BattleEffectState *effect;
    u8 *group;

    if ((task->flags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)func_001A17F0())->effect;
    if (effect->actor != (u32)task->unit) {
        return -1;
    }
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask(task, 9));
    btlStartTask(btlCreateEffObjB(task->unit, 0xB4));
    group = fldCreateSceneGroupAction(task, 0x64, 1);
    *(s32 *)(group + 0x28) = 0x16;
    btlStartTask(group);
    effect->phase = 1;
    return (*(u16 *)((u8 *)task->unit + 0x12E) & 0x480) ? 0x18 : 0x1A;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002069C0);

s32 btlIsEffectActor(s32 actor) {
    s32 effectActor;

    effectActor = ((BtlState *)func_001A17F0())->effect->actor;
    if (effectActor == 0) {
        return 0;
    }
    return (effectActor ^ actor) == 0;
}

extern s32 btlHasEffectActor(void);

s32 btlGetSoleTargetKind(void) {
    BtlState *state = (BtlState *)func_001A17F0();
    BtlUnit *target;
    BtlUnit *unit;
    BtlUnit *last;
    s32 count;
    if (btlHasEffectActor() == 0) {
        return -1;
    }
    target = (BtlUnit *)btlGetEffectActor();
    last = NULL;
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x200) {
                if (!(unit->flags & 0xE0)) {
                    if (!(unit->unk_12E & 0x800)) {
                        count++;
                        last = unit;
                    }
                }
            }
        }
    }
    if (count == 1 && (last == NULL || last == target)) {
        return 7;
    }
    return -1;
}

s32 btlHasDifferentActiveTarget(u32 target) {
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    return btlGetEffectActor() != target;
}

extern s32 btlHasEffectActor(void);

s32 btlSetLinkFlagOff(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)func_001A17F0())->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x107) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            *other->model->flags &= ~1;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->flags & 4) {
            *other->model->flags &= ~1;
        } else {
            *other->model->flags |= 1;
        }
        return 0;
    }
    return 1;
}

extern s32 btlHasEffectActor(void);

s32 btlSetLinkFlagOn(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)func_001A17F0())->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x107) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            *other->model->flags |= 1;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->flags & 4) {
            *other->model->flags &= ~1;
        } else {
            *other->model->flags |= 1;
        }
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002072F0);

extern void *btlCreateUnitFadeOutTask(void *, s32, s32);

extern s64 btlStartTask(void *);

s32 btlTryScheduleMarkedUnitTask(u8 *unit) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *effect;
    u8 *other;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return 1;
    }
    effect = (u8 *)((BtlState *)battle)->effect;
    if (((BattleEffectState *)effect)->active != 0) {
        return 1;
    }
    if (((BtlUnit *)unit)->species == 0x124) {
        return 1;
    }
    other = (u8 *)((BtlState *)battle)->units;
    while (other != 0) {
        u32 flags = ((BtlUnit *)other)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (other != unit) {
                    if (flags & 0xe0) {
                        return 1;
                    }
                }
            }
        }
        other = (u8 *)((BtlUnit *)other)->next;
    }
    btlStartTask(btlCreateUnitFadeOutTask(unit, 8, 10));
    ((BtlUnit *)unit)->flags &= ~0x100;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207718);

extern void btlSetActorEffectParameterOrMuzzlePosition();

extern void btlInterpolateVectorStep();

typedef struct BtlAimUnit {
    u8 pad00[0x30];
    u8 anim[0x90];     /* 0x30 */
    f32 origin[4];     /* 0xC0 */
    f32 direction[4];  /* 0xD0 */
    f32 distance;      /* 0xE0 */
    u8 padE4[0x2C];
    s32 state;         /* 0x110 */
    s32 kind;          /* 0x114 */
    u8 pad118[4];
    s32 armed;         /* 0x11C */
    u8 pad120[0x10];
    f32 scale;         /* 0x130 */
} BtlAimUnit;

s32 btlUnitStartAimAtTarget(BtlAimUnit *unit) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *target;
    u32 flags;
    f32 distance;

    if (unit->kind == 0x171) {
        return 1;
    }
    if (unit->kind != 0x189) {
        return 0;
    }
    for (target = (u8 *)((BtlState *)battle)->units; target != 0; target = (u8 *)((BtlUnit *)target)->next) {
        flags = ((BtlUnit *)target)->flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 2) != 0) {
                    if (((BtlUnit *)target)->mode == 0x107) {
                        break;
                    }
                }
            }
        }
    }
    if (target == 0) {
        return 1;
    }
    if (unit->state != 0x1E || unit->armed != 0) {
        return 1;
    }
    btlCopyMotionTransform(unit->anim, unit);
    unit->scale = 10.0f;
    unit->armed = 1;
    unit->state = 0;
    btlSetActorEffectParameterOrMuzzlePosition(target, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, unit->origin);
    unit->origin[1] += 150.0f;
    btlInterpolateVectorStep(unit->anim);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, unit->origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    unit->distance = distance;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, unit->direction);
    unit->distance += 45.0f;
    func_001DB698(unit->origin);
    return 1;
}

s32 btlIsSpecialActionKind(s32 unit) {
    switch (((BtlUnit *)unit)->unk_114) {
    case 0x171:
        return 1;
    case 0x189:
        return 1;
    default:
        return 0;
    }
}

u32 btlGetEffectActive(void) {
    s32 battle;
    s32 effect;

    battle = func_001A17F0();
    if (((BtlState *)battle)->battleMode != 0x108) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return ((BattleEffectState *)effect)->active;
}

s32 btlHasEffectActor(void) {
    s32 absent = 0;
    s32 battle;
    s32 effect;

    battle = func_001A17F0();
    if (((BtlState *)battle)->battleMode != 0x108) {
        return absent;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return absent;
    }
    return (((BattleEffectState *)effect)->actor != 0);
}

u32 btlGetEffectValue(void) {
    s32 battle;
    s32 effect;

    battle = func_001A17F0();
    if (((BtlState *)battle)->battleMode != 0x108) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return ((BattleEffectState *)effect)->value;
}

u32 btlGetEffectActor(void) {
    s32 battle;

    battle = func_001A17F0();
    return ((BtlState *)battle)->effect->actor;
}

s32 btlIsSpecialEnemyEffectLinkSatisfied(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit = (u8 *)((BtlState *)battle)->units;
    u8 *effect = (u8 *)((BtlState *)battle)->effect;
    while (unit != 0) {
        if ((((BtlUnit *)unit)->flags & 0x400) &&
            ((BtlUnit *)unit)->mode == 0x107) {
            break;
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (unit == 0) {
        return 1;
    }
    if (!(((BtlUnit *)unit)->flags & 0xe0)) {
        return 0;
    }
    return *(u8 **)(effect + 4) == unit;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207CA0);

extern s8 D_003BD86C;

extern s32 sdfNamedChunkFindId(void *, void *);

extern void func_00207CA0(s32, s32);

/* Named chunk indirection follows the same +0x18/+0x0C layout as DDS2. */
typedef struct BtlNamedChunkData {
    u8 pad00[0xC];
    void **entries; /* 0x0C: indexed chunk node pointers */
} BtlNamedChunkData;

typedef struct BtlNamedChunkDescriptor {
    BtlNamedChunkData *data;
    u8 pad04[0x18];
    s32 argument; /* 0x1C */
} BtlNamedChunkDescriptor;

typedef struct BtlNamedChunkHolder {
    u8 pad00[0x18];
    BtlNamedChunkDescriptor *chunk;
} BtlNamedChunkHolder;

s32 btlDispatchNamedChunkNode(void *query) {
    u8 *effect = *(u8 **)(func_001A17F0() + 0x694);
    u8 *unit = *(u8 **)effect;
    u8 *model;
    u8 *descriptor;
    s32 index;
    s32 selected;
    if (unit == 0) {
        return 1;
    }
    if ((((BtlUnit *)unit)->flags & 2) == 0) {
        return 1;
    }
    model = (u8 *)((BtlUnit *)unit)->model->flags;
    descriptor = (u8 *)((BtlNamedChunkHolder *)model)->chunk;
    index = sdfNamedChunkFindId(descriptor, query);
    if (index == -1) {
        return 1;
    }
    selected = (s32)((BtlNamedChunkDescriptor *)descriptor)->data->entries[index];
    D_003BD86C = 1;
    func_00207CA0(selected, ((BtlNamedChunkDescriptor *)descriptor)->argument);
    return D_003BD86C;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207E68);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207FF0);

s32 btlAdjustSpeciesAnimation(u8 *unit, s32 animation) {
    s32 species;
    u32 flags = ((BtlUnit *)unit)->flags;
    if ((flags & 1) == 0) {
        return animation;
    }
    if ((flags & 0x400) == 0) {
        return animation;
    }
    species = ((BtlUnit *)unit)->mode;
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

void btlMarkSpecialUnit(u8 *unit) {
    s32 species;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    species = ((BtlUnit *)unit)->mode;
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
    ((BtlUnit *)unit)->unk_114 |= 0x200;
}

u8 *btlGetReadyUnitForSpecies(s32 mode, u32 species) {
    u8 *unit;
    if (mode != 1) {
        return NULL;
    }
    switch (species) {
    case 0x10A:
    case 0x12E:
    case 0x12F:
        break;
    default:
        return NULL;
    }
    unit = **(u8 ***)(func_001A17F0() + 0x694);
    if (unit == NULL) {
        return NULL;
    }
    return (((BtlUnit *)unit)->flags & 2) ? unit : NULL;
}

extern u64 btlAdvanceRuntimeSequenceCounter(void);

u64 btlCreateSpecialUnitAndLoadModel(u64 owner) {
    u8 **slot = *(u8 ***)(func_001A17F0() + 0x694);
    u8 *model = *slot;
    u8 *entry;
    if (model != 0) {
        return btlAdvanceRuntimeSequenceCounter();
    }
    model = (u8 *)btlCreateUnit();
    *slot = model;
    func_001A1990(model + 0x120, 0x10a);
    func_00207E68();
    entry = (u8 *)btlCreateModelLoadPollTask(*slot, 1, 0x10a, 0);
    if (owner != 0) {
        *(u64 *)(entry + 8) = owner;
        *entry = 4;
    }
    btlStartTask(entry);
    return *(u64 *)(entry + 0x38);
}

void btlDestroySpecialUnitSlot(void) {
    s32 *data;
    s32 unit;

    unit = func_001A17F0();
    data = (s32 *)((BtlState *)unit)->effect;
    unit = *data;
    if (unit != 0) {
        btlDestroyUnit(unit);
        *data = 0;
    }
}

/* Allocate and launch a subtask from the active battle task slot. */
u64 btlStartSubtaskWithInput(u64 input) {
    u8 *subtaskSlot = (u8 *)((BtlState *)func_001A17F0())->effect;
    u8 *task = (u8 *)func_001D9038(*(void **)subtaskSlot, 12);
    if (input != 0) {
        *(u64 *)(task + 8) = input;
        *task = 4;
    }
    *(u64 *)(task + 0x40) = 0x8000000000000003ULL;
    btlStartTask(task);
    return *(u64 *)(task + 0x38);
}

extern s32 btlDispatchNamedChunkNode(void *);

extern s8 D_003BB880[];

extern s8 D_003BB888[];

extern s8 D_003BB890[];

void btlUpdateReadyUnits(void) {
    BtlState *state = (BtlState *)func_001A17F0();
    BtlUnit *unit;
    s32 ready;
    if (state->unk_1F4 & 0x80000) {
        unit = state->units;
        while (unit != NULL) {
            if (unit->flags & 0x400) {
                if (unit->flags & 0x80) {
                    switch (unit->species) {
                    case 0x10A:
                        ready = btlDispatchNamedChunkNode(D_003BB880);
                        break;
                    case 0x12E:
                        ready = btlDispatchNamedChunkNode(D_003BB888);
                        break;
                    case 0x12F:
                        ready = btlDispatchNamedChunkNode(D_003BB890);
                        break;
                    default:
                        ready = 1;
                        break;
                    }
                    if (ready != 0) {
                        unit->flags = (unit->flags & ~0x80) | 0x40;
                    }
                }
            }
            unit = unit->next;
        }
    }
}

s32 btlCheckEnemyUnitReadiness(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    s32 count = 0;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if ((flags & 0xe0) == 0) {
                    count++;
                } else {
                    ((BtlUnit *)unit)->flags = ((flags & ~1) | 0x80) & ~0x40;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return count == 0;
}

u32 func_00208660(void) {
    return 0xffffffff;
}

s32 func_00208668(s32 unit) {
    return ((*(s32 *)(unit + 0x110) & 0x200) < 1);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208678);

s32 btlRemapSpecialUnitCommandIndex(u8 *unit, s32 index) {
    u32 flags = ((BtlUnit *)unit)->flags;
    if ((flags & 0x400) == 0) {
        return index;
    }
    if (((BtlUnit *)unit)->mode != 0x10d) {
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

void func_002089F0(s32 unit) {
    if (((BtlUnit *)unit)->mode != 0x10d) {
        return;
    }
    func_001A17F0();
    btlInitializeEffectVectorsFromSourceRecords(unit, 1, 0x11d);
    func_00208860(unit, 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208A50);

void func_00208C20(void) {
    func_001F53F0();
}

u32 btlIsInSpecialModeRange(u32 unused0, u32 unused1, s32 action) {
    if ((0x171 < action) && ((action < 0x175 || (action == 0x1a1)))) {
        return 200;
    }
    return 100;
}

u32 btlGetSpecialModeEffectValue(void) {
    s32 battle;
    s32 effect;

    battle = func_001A17F0();
    if (((BtlState *)battle)->battleMode != 0x116) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return *(u16 *)(effect + 0x4);
}

extern void btlInitializeEffectVectorsFromSourceRecords();

extern void func_001F53F0(void);

void btlTriggerSpecialUnitAction(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit = (u8 *)((BtlState *)battle)->units;
    while (unit != 0) {
        if (((BtlUnit *)unit)->flags & 0x400) {
            if (((BtlUnit *)unit)->mode == 0x10d) {
                break;
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (unit != 0) {
        ((BtlState *)battle)->unk_5B8 = 0;
        btlInitializeEffectVectorsFromSourceRecords(unit, 1, 0x11d);
        func_001F53F0();
    }
}

extern u32 effMiscRand(char *);

extern char D_003A5D98[];

extern char D_00324550[];

/* Boss-selection view of battle->effect: +0x0C is a full-width lookup ID
 * here, unlike the timer/phase view in BattleEffectState. */
typedef struct BtlBossEffectPayload {
    s32 actor;
    u32 color;
    s32 options;
    s32 selectedId;
} BtlBossEffectPayload;

void btlInitRandomBossSelection(void) {
    u8 *data = *(u8 **)(func_001A17F0() + 0x694);
    ((BtlBossEffectPayload *)data)->color = 0x80808080;
    ((BtlBossEffectPayload *)data)->options = 0;
    ((BtlBossEffectPayload *)data)->selectedId = effMiscRand(D_00324550) % 6;
    btlBossDebugPrintf(D_003A5D98, ((BtlBossEffectPayload *)data)->selectedId);
}

s32 btlFilterBossCommandBySelection(u8 *unit, s32 command) {
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 1) {
        if (flags & 0x400) {
            u8 *effect = *(u8 **)((u8 *)func_001A17F0() + 0x694);
            if (((BtlUnit *)unit)->lookupId == ((BtlBossEffectPayload *)effect)->selectedId) {
                if (((BtlBossEffectPayload *)effect)->options & 4) {
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
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    battleData = *(u8 **)(func_001A17F0() + 0x694);
    ((BtlUnit *)unit)->unk_114 |= 0x200;
    ((BtlUnit *)unit)->positionB0 = 100.0f;
    ((BtlUnit *)unit)->positionB4 = 25.0f;
    ((BtlUnit *)unit)->positionB8 = 100.0f;
    ((BtlUnit *)unit)->positionBC = 25.0f;
    if (((BtlBossEffectPayload *)battleData)->selectedId != ((BtlUnit *)unit)->lookupId) {
        entry = (u8 *)btlFindUnitByActor(unit);
        if (entry != 0) {
            *(u16 *)(entry + 4) = 0;
            ((BtlUnit *)unit)->mode = 0x10f;
        }
    }
}

void func_00208EA8(void) {
    func_00208E10();
}

void *btlFindActiveMember(s32 group, s32 type) {
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
        return btlAdvanceRuntimeSequenceCounter();
    }
    model = (u8 *)btlCreateUnit();
    *slot = model;
    func_001A1990(model + 0x120, 0x10e);
    entry = (u8 *)btlCreateModelLoadPollTask(*slot, 1, 0x10e, 0);
    if (owner != 0) {
        *(u64 *)(entry + 8) = owner;
        *entry = 4;
    }
    btlStartTask(entry);
    return *(u64 *)(entry + 0x38);
}

void btlDestroyActiveMemberSlot(void) {
    s32 *data;
    s32 unit;

    unit = func_001A17F0();
    data = (s32 *)((BtlState *)unit)->effect;
    unit = *data;
    if (unit != 0) {
        btlDestroyUnit(unit);
        *data = 0;
    }
}

extern s32 mdlGetNodeField2C(s32, s32);

extern void func_00221EF0(void *, s32, s32);

void btlStepFocusAngle(void) {
    BtlState *battle = (BtlState *)func_001A17F0();
    u32 *slot;
    BtlUnit *player;
    BtlUnit *unit;
    if (!(battle->unk_1F4 & 0x80000)) {
        return;
    }
    slot = *(u32 **)((u8 *)battle + 0x694);
    player = *(BtlUnit **)slot;
    if (player == NULL) {
        return;
    }
    if (!(player->flags & 2)) {
        return;
    }
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->lookupId == slot[3]) {
                    break;
                }
            }
        }
    }
    if (unit == NULL) {
        return;
    }
    mdlGetNodeField2C((s32)player->model->flags, 0);
    if (slot[2] & 4) {
        if ((slot[1] & 0xFF000000) != 0x80000000) {
            slot[1] += 0x10000000;
        }
    } else {
        if (slot[1] & 0xFF000000) {
            slot[1] += 0xF0000000;
        }
    }
    func_00221EF0(((BtlUnit *)*(u32 **)slot)->model, 0, slot[1]);
}

typedef struct BtlEffectTarget {
    u8 pad00[8];
    u32 flags;    /* 0x08 */
    s32 targetId; /* 0x0C */
} BtlEffectTarget;

s32 func_00209140(BtlUnit *actor, BtlUnit *target, s32 command) {
    BtlEffectTarget *effect = (BtlEffectTarget *)((BtlState *)func_001A17F0())->effect;
    s32 unit;
    u32 blocked;

    if (!(actor->flags & 0x200)) {
        return 0;
    }
    if (!(target->flags & 0x400)) {
        return 0;
    }
    if (D_003BAA4C[command * 2 + 1] != 1) {
        unit = btlFindUnitByActor(actor);
        if (unit == 0) {
            return 0;
        }
        blocked = func_001A3360(unit, 0, 0);
    } else {
        blocked = D_003BAA50[command * 0x38 + 8];
    }
    if (blocked != 0) {
        return 4;
    }
    if (target->lookupId != effect->targetId) {
        return 4;
    }
    effect->flags |= 1;
    return 0;
}

u32 btlSelectResponseCodeForEnemyFlag(u32 unused, s32 unit) {
    u32 result;

    result = 4;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209238);

s32 btlFindSubsequentTurnScript(void) {
    s32 battle;

    battle = func_001A17F0();
    if (((BtlState *)battle)->turnCount == 0) {
        return -1;
    }
    if ((((BtlState *)battle)->unk_1F4 & 0x800) != 0) {
        return -1;
    }
    if (((BtlState *)battle)->mode != 1) {
        return -1;
    }
    return btlFindScriptResource(D_003BB898);
}

s32 btlCheckSelectedBossUnitFlag(void) {
    u8 *battle = (u8 *)func_001A17F0();
    u8 *unit = (u8 *)((BtlState *)battle)->units;
    u8 *effect = (u8 *)((BtlState *)battle)->effect;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (((BtlBossEffectPayload *)effect)->selectedId == ((BtlUnit *)unit)->lookupId) {
                    break;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    return (((BtlUnit *)unit)->flags & 0x20) ? 1 : -1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209528);

extern void func_001DF358(void *, void *);

/* Event entry carrying a battle task and its action kind. */
typedef struct BtlEventEntry {
    u8 pad00[0xF0];
    u32 flags;
    BtlTask *task; /* 0xF4 */
    u8 padF8[0xC];
    u32 kind;      /* 0x104 */
} BtlEventEntry;

s32 btlDispatchEligibleBossEvent(u8 *entry) {
    BtlTask *task = ((BtlEventEntry *)entry)->task;
    u8 *data;
    u32 kind;
    if (task == 0) {
        return 1;
    }
    if ((task->unit->flags & 0x400) == 0) {
        return 1;
    }
    data = *(u8 **)((u8 *)func_001A17F0() + 0x694);
    if (((BtlBossEffectPayload *)data)->options & 4) {
        return 1;
    }
    kind = ((BtlEventEntry *)entry)->kind;
    if (kind < 9) {
        if (kind >= 4) {
            func_001DF358(entry, entry);
            ((BtlEventEntry *)entry)->flags |= 0x1000;
            return 0;
        }
    }
    return 1;
}

s32 btlCheckAttachedMember(u8 *entry) {
    u8 *unit = (u8 *)((BtlEventEntry *)entry)->task;
    if (unit == 0) {
        return 1;
    }
    if ((((BtlTask *)unit)->unit->flags & 0x400) == 0) {
        return 1;
    }
    {
        s32 *data = *(s32 **)(func_001A17F0() + 0x694);
        s32 flags = ((BtlBossEffectPayload *)data)->options & 4;
        if (flags) {
            return 1;
        }
        return 0;
    }
}

s32 btlCheckBossOptionAllowed(u8 *unit) {
    s32 *data;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return -1;
    }
    data = *(s32 **)(func_001A17F0() + 0x694);
    return (((BtlBossEffectPayload *)data)->options & 4) ? -1 : 0;
}

s32 btlRemapSelectedBossCommand(u8 *unit, s32 command, u8 mode) {
    u8 *effect;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return command;
    }
    effect = *(u8 **)((u8 *)func_001A17F0() + 0x694);
    if (((BtlUnit *)unit)->lookupId != ((BtlBossEffectPayload *)effect)->selectedId) {
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

extern void btlFlagUnitDefeatCandidate(BtlUnit *);

typedef struct BtlEffectLink {
    BtlUnit *actor;
    u32 unk4;
    u32 flags; /* 8 */
    s32 code;  /* 0xC */
} BtlEffectLink;

void btlSyncEffectActorToUnit(BtlUnit *unit, u8 *task) {
    BtlEffectLink *effect;
    BtlUnit *target;
    BtlUnit *actor;

    if (unit->flags & 0x400) {
        effect = (BtlEffectLink *)((BtlState *)func_001A17F0())->effect;
        target = effect->actor;
        if (target != 0) {
            if (target->flags & 2) {
                if (unit->lookupId == effect->code) {
                    if ((*(u16 *)(task + 0x26) & 0x800) == 0) {
                        effect->flags |= 4;
                        PCP_COPY_VECTOR(target->position, unit->position);
                        btlSetUnitPosition(target, (u8 *)unit + 0x60);
                        actor = effect->actor;
                        PCP_COPY_VECTOR(actor->rotation, unit->rotation);
                        btlSetUnitRotation(actor, (u8 *)unit + 0x70);
                        btlFlagUnitDefeatCandidate(effect->actor);
                    }
                }
            }
        }
    }
}

u32 btlGetSelectedBossEffectId(void) {
    s32 battle;
    s32 effect;

    battle = func_001A17F0();
    if (((BtlState *)battle)->battleMode != 0x10b) {
        return 0;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return 0;
    }
    return ((BtlBossEffectPayload *)effect)->selectedId;
}

s32 btlHasBossEffectOption(void) {
    s32 absent = 0;
    s32 battle;
    s32 effect;

    battle = func_001A17F0();
    if (((BtlState *)battle)->battleMode != 0x10b) {
        return absent;
    }
    effect = (s32)((BtlState *)battle)->effect;
    if (effect == 0) {
        return absent;
    }
    return ((((BtlBossEffectPayload *)effect)->options & 2) > 0);
}

/* Effect-data overlay for the one-shot scripted resource trigger. */
typedef struct BtlEventTriggers {
    s8 pending;       /* 0x00: starts the trigger */
    s8 resourceReady; /* 0x01: consumed when the resource is requested */
} BtlEventTriggers;

void btlArmEventResourceTrigger(void) {
    u8 *data;
    s32 battle;

    battle = func_001A17F0();
    data = (u8 *)((BtlState *)battle)->effect;
    ((BtlEventTriggers *)data)->resourceReady = 1;
    ((BtlEventTriggers *)data)->pending = 0;
}

extern void fldAppendSceneGroupHandle();

extern void btlAppendIndexListEntry();

typedef struct BtlMarkState {
    u8 marked;  /* 0 */
    s8 variant; /* 1 */
} BtlMarkState;

s32 btlPickRandomMarkedTask(void) {
    BtlState *state = (BtlState *)func_001A17F0();
    BtlMarkState *mark = (BtlMarkState *)state->effect;
    BtlTask *candidates[16];
    BtlTask *task;
    BtlUnit *unit;
    s32 count;

    if (state->mode != 1) {
        return -1;
    }
    count = 0;
    for (task = state->tasks; task != 0; task = task->next) {
        if (task->flags & 8) {
            unit = task->unit;
            if (unit->flags & 1) {
                if (unit->flags & 0x200) {
                    if ((unit->unk_12E & 0x7FFF) == 0x2000) {
                        candidates[count++] = task;
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    task = candidates[effMiscRandMod(0, count)];
    unit = task->unit;
    fldAppendSceneGroupHandle(task);
    task->result = 2;
    if (mark->variant != 0) {
        task->arg = 0xD1;
    } else {
        task->arg = 0xD2;
    }
    btlAppendIndexListEntry(task->unk_60, (s32)unit);
    mark->marked = 1;
    return -1;
}

s32 btlConsumeReadyEventScriptResource(void) {
    s32 absent = -1;
    s32 data;

    data = *(s32 *)(func_001A17F0() + 0x694);
    if (((BtlEventTriggers *)data)->pending != 0) {
        if (((BtlEventTriggers *)data)->resourceReady != 0) {
            ((BtlEventTriggers *)data)->resourceReady = 0;
            return btlFindScriptResource(D_003BB8A0);
        }
        ((BtlEventTriggers *)data)->pending = 0;
        return -1;
    }
    return absent;
}

void btlClearEventEffectValue(void) {
    s32 battle;

    battle = func_001A17F0();
    *(u16 *)((BtlState *)battle)->effect = 0;
}

s32 btlRemapBossAction(BtlUnit *unit, s32 action, u8 option) {
    u32 flags = unit->flags;
    u16 unitMode;
    if (!(flags & 0x400)) {
        return action;
    }
    if (!(flags & 1)) {
        return -1;
    }
    unitMode = unit->mode;
    if ((u16)(unitMode - 0x119) >= 2) {
        return -1;
    }
    func_001A17F0();
    if (action == 10) {
        return 0;
    }
    if (action == 11) {
        switch (unit->mode) {
        case 0x11A:
            return option != 1;
        default:
            return option != 1;
        }
    }
    return action;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5D98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5DB8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209C90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209EB8);

u32 btlGetEventEffectValue(void) {
    s32 battle;

    battle = func_001A17F0();
    return *(u16 *)((BtlState *)battle)->effect;
}

s32 btlHasFirstSpecialEnemySpecies(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    u16 species = 0;
    while (unit != 0) {
        u32 flags = ((BtlUnit *)unit)->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                species = ((BtlUnit *)unit)->mode;
                if ((u16)(species - 0x119) < 2) {
                    break;
                }
            }
        }
        unit = (u8 *)((BtlUnit *)unit)->next;
    }
    if (unit == 0) {
        return 0;
    }
    return species == 0x119;
}

void btlDispatchSpecialEnemyActionWhenPhaseAllows(u8 *unit, s32 action) {
    u8 *data;
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return;
    }
    if ((u16)(((BtlUnit *)unit)->mode - 0x119) >= 2) {
        return;
    }
    data = *(u8 **)(func_001A17F0() + 0x694);
    if (((BtlUnit *)unit)->mode == 0x11a && *(u16 *)data >= 3) {
        return;
    }
    btlRestoreUnitMinimumValueAndClearStatus(unit, action);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A560);

s32 btlMapCommandToSkill(u32 command) {
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

s32 func_0020A780(BtlUnit *unit, s32 arg1) {
    u8 *task;

    if (!(unit->flags & 0x400)) {
        return arg1;
    }
    switch (unit->mode) {
    case 0x11B:
        if (arg1 == 2) {
            return 0;
        }
        if (arg1 == 0xD) {
            return -1;
        }
        break;
    case 0x13D:
    case 0x13E:
    case 0x13F:
    case 0x140:
    case 0x141:
    case 0x142:
        switch (arg1) {
        case 2:
            return 0;
        case 3:
            return 4;
        case 5:
            return 4;
        case 6:
            return 4;
        case 7:
            return 4;
        case 8:
            return 4;
        case 1:
            task = btlCreateStiffenDamageShakeTask((u8 *)unit, 8.0f);
            *(s32 *)(task + 0x28) = 1;
            btlStartTask(task);
            return -1;
        case 4:
            break;
        }
        break;
    }
    return arg1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A860);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AB08);

s32 func_0020ABE0(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x11B) {
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
                            if (unit->mode != 0x11B) {
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

s32 btlMapSkillToCode(s32 skill) {
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

s32 btlMapSkillRange(u32 skill) {
    if (skill < 0x143) {
        if (skill >= 0x13d) {
            return 0x12c;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ADA8);

s32 btlMapSpeciesToActionVariant(u8 *unit, s32 action) {
    if ((((BtlUnit *)unit)->flags & 0x400) == 0) {
        return -1;
    }
    if (*((u8 *)D_003BAA60 + action * 32 + 3) == 0) {
        return -1;
    }
    switch (((BtlUnit *)unit)->mode) {
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

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern char D_003A5FF0[];

extern char D_003A6018[];

/* Random battle camera shot (A-E:0..2). Must stay defined above its callers in this
   file: retail's btlChooseDefeatCameraByActionAndTargets sees it as nothrow (bnez vs bnel in the branch slot). */
INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6018);

void func_0020AFB8(unit)
    u8 *unit;
{
    u32 kind;

    btlFlagAllUnitDefeatCandidatesTask();
    kind = effMiscRandMod(0, 3);
    switch (kind) {
    case 0:
        func_003003F0(D_003A5FF0);
        btlSetEffectCameraKeys(unit, -34.8f, -203.4f, -1877.1f, -0.042f, 0.006f, -0.012f, 0.99f, 59.4f, -184.8f, -2072.9f,
                      -0.041f, 0.024f, -0.013f, 0.99f, 40.0f, 20.0f);
        break;
    case 1:
        func_003003F0(D_003A6018);
        btlSetEffectCameraKeys(unit, -391.6f, -567.8f, -2206.8f, 0.032f, -0.062f, -0.014f, 0.989f, -407.1f, -447.8f, -2343.8f,
                      -0.003f, -0.062f, -0.012f, 0.989f, 40.0f, 20.0f);
        break;
    case 2:
        func_003003F0("A-E:2++++++++++++++++++++++++++\n");
        btlSetEffectCameraKeys(unit, 479.7f, -240.2f, -2018.3f, -0.023f, 0.105f, -0.016f, 0.985f, 552.2f, -250.7f, -2300.2f,
                      -0.023f, 0.105f, -0.016f, 0.985f, 40.0f, 20.0f);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B190);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B348);

s64 func_0020B560(void) {
    return func_0020B348();
}

s32 func_0020B580(u8 *unit) {
    u8 *entry = (u8 *)((BtlEventEntry *)unit)->task;
    u32 flags = ((BtlTask *)entry)->unit->flags;
    u8 *other;
    if (flags & 0x200) {
        if ((flags & 0x1000) == 0) {
            return 0;
        }
        if (btlGetIndexListCount(((BtlTask *)entry)->unk_60) == 1) {
            other = (u8 *)btlGetIndexListEntry(((BtlTask *)entry)->unk_60, 0);
            if ((((BtlUnit *)other)->flags & 0x400) == 0) {
                return 0;
            }
            btlFaceLinkedTargetAndFlagDirection(unit, unit);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020B348(unit, unit, 0);
        }
    } else {
        func_0020AFB8();
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B640);

s32 func_0020B770(u8 *unit) {
    u8 *entry = (u8 *)((BtlEventEntry *)unit)->task;
    u8 *other;
    if (((BtlTask *)entry)->unit->flags & 0x200) {
        if (btlGetIndexListCount(((BtlTask *)entry)->unk_60) == 1) {
            other = (u8 *)btlGetIndexListEntry(((BtlTask *)entry)->unk_60, 0);
            if ((((BtlUnit *)other)->flags & 0x400) == 0) {
                return 0;
            }
            func_0020B190(unit, other);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020B348(unit, unit, 0);
        }
        ((BtlUnit *)unit)->flags = 0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B818);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6338);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6358);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020BE30);

extern void func_0020B190(u8 *, void *);

/* 0x20-byte action metadata entries referenced by a unit's action index. */
typedef struct BtlActionTableRow {
    u8 pad00[3];
    u8 enabled; /* 0x03: zero rejects the action */
    u8 pad04[0x18];
    u16 flags;  /* 0x1C: special animation selection bits */
    u8 pad1E[2];
} BtlActionTableRow;

s32 btlChooseDefeatCameraByActionAndTargets(u8 *unit) {
    u16 flags = ((BtlActionTableRow *)D_003BAA60)[((BtlUnit *)unit)->unk_114].flags;
    if (flags & 0x1000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            func_0020AFB8(unit);
        } else {
            btlSetEffectCameraKeys(unit, 59.2f, -700.6f, -1901.2f,
                           0.092f, 0.023f, -0.009f, 0.987f,
                           59.2f, -160.6f, -1901.2f, -0.106f,
                           0.025f, -0.014f, 0.985f, 45.0f, 30.0f);
        }
        return 1;
    } else if (flags & 0x2000) {
        if (btlGetIndexListCount(((BtlEventEntry *)unit)->task->unk_60) == 1) {
            void *other = (void *)btlGetIndexListEntry((void *)((BtlEventEntry *)unit)->task->unk_60, 0);
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020B190(unit, other);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020AFB8(unit);
        }
        return 1;
    }
    return 0;
}

u32 func_0020CB28(s32 action) {
    u32 result;

    result = 2;
    if (action != 0x1d7) {
        result = 0;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6398);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6438);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6460);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6480);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB880);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB888);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB890);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB898);

INCLUDE_SDATA(const s32, "game/code_001FF030", D_003BB8A0);

