#include "common.h"
#include "ee_mmi.h"

extern s32 *btlFindGroupedEntity();

extern u8 D_003BBB0D;

extern s32 btlFindModelEntry();

extern s32 btlCountTasksByKind(u32);

extern u32 func_001A3360(u64, u64, u64);

extern void *btlAllocateIndexList(s32);

extern u32 func_001DAE48();

extern s8 D_003D7588[];

typedef struct BattleRuntimeState {
    u32 flags;
    u16 state;
    u8 unk_06;
    u8 pending;
    s8 active;
    u8 unk_09[3];
    u32 options;
    u32 color10;
    u32 color14;
    u32 color18;
    u32 color1C;
    u32 color20;
    f32 unk_24;
    s32 gridWidth;
    s32 gridHeight;
    s32 cellWidth;
    s32 cellHeight;
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

extern s32 sdfCreateSemaphore(s32, s32, s32);

extern u32 D_003BD878;

extern s32 D_00367940[];

extern s32 D_00367960[];

extern void func_003014F0();

extern s32 func_001A17F0(void);

typedef struct BattleEffectState {
    u32 actor, flags, value;
    u16 timer;
    u8 active, phase;
    u32 effect;
    f32 speed;
} BattleEffectState;

typedef struct BtlUnitModel {
    u8 unk_00[0x8C];
    u32 *flags;
} BtlUnitModel;

typedef struct BtlUnit {
    u8 unk_00[0xC8];
    u32 species;
    u8 unk_CC[0x20];
    s32 unk_EC;
    u8 unk_F0[0x18];
    u64 identity; /* 0x108: compared to exclude the current actor */
    u32 flags;
    u32 unk_114;
    u8 unk_118[8];
    u16 unk_120;
    u8 unk_122[2];
    u16 mode;
    u8 unk_126[8];
    u16 unk_12E;
    u8 unk_130[0x1EC];
    u32 unk_31C;
    struct BtlUnitModel *model;
    u8 unk_324[0x20];
    struct BtlUnit *next;
} BtlUnit;

typedef struct BtlTask {
    u8 unk_00[8];
    u32 flags;
    u8 unk_0C[0xC];
    BtlUnit *unit;
    u8 unk_1C[4];
    s32 result;
    s32 arg;
    u8 unk_28[0x38];
    s32 unk_60;
    u8 unk_64[0x108];
    struct BtlTask *next;
} BtlTask;

typedef struct BtlState {
    u8 unk_000[0x1C0];
    s16 unk_1C0;
    u8 unk_1C2[0x32];
    u32 unk_1F4;
    u8 unk_1F8[4];
    u32 unk_1FC;
    u8 unk_200[0x24];
    BtlTask *tasks;
    BtlUnit *units;
    u8 unk_22C[0x20];
    u16 mode;
    u8 unk_24E[0x446];
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

extern s32 func_0020DC38();

extern s32 func_001DAE50(void *, u32);

extern void func_001DAE08(void *);

extern u8 *D_003BAA50;

extern s64 btlStartTask(void *);

extern s8 D_003BB880[];

extern s8 D_003BB888[];

extern s8 D_003BB890[];

extern void func_001DC760(void);

extern void func_001DC3A0(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void func_001DC568(void);

extern void func_001DC3A0(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void func_0020DA40(u8 *);

extern void func_001DC760(void);

void func_0020DB90(u8 *actor);

s32 battleDispatchActionAnimation(u8 *unit) {
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

s32 btlGetPhaseCommand(void) {
    u8 *battle = (u8 *)func_001A17F0();
    switch (*(u16 *)(battle + 0x25c)) {
    case 0: return 0x11c;
    case 1: return 0x11b;
    default: return -1;
    }
}

s32 btlGetAlternatePhaseCommand(void) {
    u8 *battle = (u8 *)func_001A17F0();
    switch (*(u16 *)(battle + 0x25c)) {
    case 0: return 0x11f;
    case 1: return 0x120;
    default: return -1;
    }
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_0020EC20);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_0020ECF8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_0020ED90);

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

void btlReleaseBossData(void) {
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

typedef struct BtlScriptContext {
    u8 pad00[0x1C0];
    s16 scenarioId;      /* 0x1C0: used to format the script path */
    u8 pad1C2[0x26];
    s32 scriptHandle;    /* 0x1E8 */
} BtlScriptContext;

s32 btlFindScriptResource(char *name) {
    char path[128];
    BtlScriptContext *battle = (BtlScriptContext *)func_001A17F0();
    if (battle->scriptHandle == 0) {
        return -1;
    }
    func_003014F0(path, D_003A66F0, battle->scenarioId, name);
    return bfFindScriptIndexByName(battle->scriptHandle, path);
}

void func_0020F940(s32 skill) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 handle;
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)(*(u8 **)(battle + 0x29c) + 0x20) - 1,
                            *(s32 *)(battle + 0x1e8), skill);
    scrSetCurrentActor(handle, 0);
    func_00101A80(*(s32 *)(battle + 0x29c), handle);
    *(s32 *)(battle + 0x2a0) = handle;
}

s32 btlReleaseScriptResource(void) {
    extern s32 kwlnTaskIsRegistered(s32);
    u8 *battle = (u8 *)func_001A17F0();
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return 1;
    }
    if (kwlnTaskIsRegistered(*(s32 *)(battle + 0x2a0)) == 0) {
        *(s32 *)(battle + 0x2a0) = 0;
        return 1;
    }
    return 0;
}

extern char D_003BB8B8[];

s32 btlCanStartPrimaryScriptTask(void) {
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
    return btlFindScriptResource(D_003BB8B8) != -1;
}

extern s32 scrCreateTaskForProcessId(s32, s32, s32);

void btlStartPrimaryScriptTask(void) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 skill;
    s32 handle;
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return;
    }
    skill = btlFindScriptResource(D_003BB8B8);
    if (skill == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)(*(u8 **)(battle + 0x29c) + 0x20) - 1,
                            *(s32 *)(battle + 0x1e8), skill);
    scrSetCurrentActor(handle, 0);
    func_00101A80(*(s32 *)(battle + 0x29c), handle);
    *(s32 *)(battle + 0x2a0) = handle;
    *(u32 *)(battle + 0x1c4) |= 1;
}

s64 btlReleaseScriptResourceA(void) {
    return btlReleaseScriptResource();
}

extern char D_003BB8C0[];

s32 btlHasScriptResource(void) {
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
    return btlFindScriptResource(D_003BB8C0) != -1;
}

void btlStartSecondaryScriptTask(void) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 skill;
    s32 handle;
    if (*(s16 *)(battle + 0x1c0) == -1) {
        return;
    }
    skill = btlFindScriptResource(D_003BB8C0);
    if (skill == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)(*(u8 **)(battle + 0x29c) + 0x20) - 1,
                            *(s32 *)(battle + 0x1e8), skill);
    scrSetCurrentActor(handle, 0);
    func_00101A80(*(s32 *)(battle + 0x29c), handle);
    *(s32 *)(battle + 0x2a0) = handle;
    *(u32 *)(battle + 0x1c4) |= 2;
}

s64 btlReleaseScriptResourceB(void) {
    return btlReleaseScriptResource();
}

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66D8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66F0);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_0020FC48);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_0020FF50);

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
    if (evtGetTaskValueWord(*(s16 *)(battle + 0x1c0)) == 2) {
        return 1;
    }
    func_001FB0A8(D_003A67B8);
    return 0;
}

extern char D_003A67D0[];

void btlReleaseEventData(void) {
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

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A67A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A67B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A67D0);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002101C8);

extern char D_003A6838[];

void btlReleaseEventAssets(void) {
    u8 *battle = (u8 *)func_001A17F0();
    void *data;
    btlReleaseEventData();
    data = *(void **)(battle + 0x1ec);
    if (data != 0) {
        func_002D0918(data);
        *(void **)(battle + 0x1ec) = 0;
    }
    func_001FB0A8(D_003A6838);
}

extern s32 func_00241BF0(s16, s32);

s32 btlCommandSelectEventAction(void) {
    s32 first = scrReadIntParameter(0);
    s32 second = scrReadIntParameter(1);
    s32 action = scrReadIntParameter(2);
    u8 *unit;
    u8 *battle;
    s32 result;
    if (first == 0) {
        unit = (u8 *)btlFindUnitByModeClear(second);
    } else {
        unit = (u8 *)btlFindUnitByModeFlagged(second);
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

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002103A0);

extern s32 scrReadIntParameter(s32);

extern u8 *btlFindUnitByModeClear(s32);

extern u8 *btlFindUnitByModeFlagged(s32);

extern void *btlCreateModelChangeTask(void *, s32, s32, s32, s32, s32);

s32 func_00210450(void) {
    s32 choice = scrReadIntParameter(0);
    s32 unitIndex = scrReadIntParameter(1);
    s32 first = scrReadIntParameter(2);
    s32 second = scrReadIntParameter(3);
    u8 *unit;
    if ((u32)unitIndex >= 0x180) {
        return 1;
    }
    if ((u32)first >= 0x180) {
        return 1;
    }
    if (choice == 0) {
        unit = btlFindUnitByModeClear(unitIndex);
    } else {
        unit = btlFindUnitByModeFlagged(unitIndex);
    }
    if (unit == NULL) {
        return 1;
    }
    btlStartTask(btlCreateModelChangeTask(unit, choice != 0, first, second, 0x18, 0));
    return 1;
}

u8 func_00210520(void) {
    s32 temp_v0;

    temp_v0 = btlCountTasksByKind(0x1a);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00210540);

extern char D_003A6848[];

s32 btlCommandSetSequenceVolumePan(void) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 index = scrReadIntParameter(0);
    if (func_002E92C0(*(s32 *)(battle + 0x1e4)) != 0) {
        sndSetSequenceVolumePan(*(s32 *)(battle + 0x1e4) + index, 0x7f, 0x3f);
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

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00210670);

void *btlCreateActionTask(void *battler, s32 action) {
    extern BattleTask *btlAllocTask(s32);
    extern BattleTaskData *func_001D47D8(BattleTask *);
    extern s32 func_00210670(void *);
    BattleTask *task = btlAllocTask(12);
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

s32 btlHasRestrictedUnit(void) {
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

s32 btlListHasMarkedFlag(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(entries[i] + 0xe) & 0x7fff) == 0x4000) {
            return 1;
        }
    }
    return 0;
}

s32 btlListCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (*(u16 *)(entries[i] + 6) < *(u16 *)(entries[i] + 8)) {
            return 0;
        }
    }
    return 1;
}

s32 btlListSecondaryCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (*(u16 *)(entries[i] + 0xa) < *(u16 *)(entries[i] + 0xc)) {
            return 0;
        }
    }
    return 1;
}

s32 btlListHasMatchingFlag(u8 **entries, s32 count, u32 flags) {
    s32 i;
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(entries[i] + 0xe) & 0x7fff) & flags) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00210928);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00210A18);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6838);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6848);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6860);

u16 func_00210BA8(u8 **entries, s32 count, s32 unused, s32 command) {
    s32 result = -1;
    if (D_003BAA50[command * 0x38 + 9] & 1) {
        switch (*(u16 *)(D_003BAA50 + command * 0x38 + 0x16)) {
        case 2:
        case 5:
        case 7:
        case 9:
        case 11:
        case 15:
            result = btlListCountersWithinLimits(entries, count) ? 3 : 0;
            break;
        }
        if (result != 0) {
            switch (*(u16 *)(D_003BAA50 + command * 0x38 + 0x1A)) {
            case 2:
            case 5:
            case 7:
            case 9:
            case 11:
            case 15:
                result = btlListSecondaryCountersWithinLimits(entries, count) ? 3 : 0;
                break;
            }
            if (result != 0) {
                if (D_003BAA50[command * 0x38 + 0x24] == 2) {
                    result = btlListHasMatchingFlag(entries, count, *(u16 *)(D_003BAA50 + command * 0x38 + 0x26)) == 0 ? 3 : 0;
                }
            }
        }
    }
    return (result < 0) ? 0 : result;
}

typedef struct BtlCommandRecord {
    u8 flags;
    u8 unk_01[7];
    u8 special;
    u8 ruleFlags;
    u8 unk_0A[2];
    u16 restriction;
    u8 unk_0E[0x16];
    u32 attributeBits;
    u8 unk_28[0x10];
} BtlCommandRecord;

extern s8 *D_003BAA4C;

extern u8 *D_003BAA50;

s32 btlGetCommandBlockReason(BtlTask *task, s32 command) {
    BtlUnit *owner;
    BtlCommandRecord *record;
    void *list;
    s32 count;
    s32 flaggedCount;
    s32 i;
    if (((BtlState *)func_001A17F0())->unk_1FC & 0x800) {
        return 0;
    }
    if (command <= 0) {
        return 0;
    }
    if (D_003BAA4C[command * 2 + 1] == 2) {
        owner = task->unit;
        if (owner->flags & 0x200) {
            if (owner->mode == 4) {
                if (mdlFlagTest(0x61) == 0) {
                    return 6;
                }
            }
        }
    }
    record = (BtlCommandRecord *)(command * 0x38 + (s32)D_003BAA50);
    if ((record->attributeBits & 0x400000FF) == 0x40000002) {
        if ((~record->restriction & 0x7FFF) == 0x4000) {
            if (btlHasRestrictedUnit() == 0) {
                return 2;
            }
        }
    }
    flaggedCount = 0;
    list = btlAllocateIndexList(0xD);
    func_001A3360((s32)task, (s32)list, 0);
    count = func_001DAE48(list);
    /* Keep this byte-table load separate from record for the matching address calculation. */
    if (D_003BAA50[command * 0x38] & 8) {
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)func_001DAE50(list, i))->unk_12E & 0x800) {
                flaggedCount++;
            }
        }
    }
    func_001DAE08(list);
    if (count != 0) {
        if (count != flaggedCount) {
            return 0;
        }
    }
    return 9;
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00210EB0);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002110B8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002111A0);

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

BattleModelEntry *btlCreateModelEntry(void) {
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

extern void sndReleaseSlotOwner(void *);

extern void func_002CFF98(void *);

void btlReleaseModelEntry(BattleModelEntry *entry) {
    if (--entry->refs != 0) {
        return;
    }
    if (entry->resource != 0) {
        func_00288788(entry->resource);
    }
    if (entry->actor != 0) {
        sndReleaseSlotOwner(entry->actor);
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

void btlReleaseAllModelEntries(void) {
    BattleModelEntry *entry = *(BattleModelEntry **)((u8 *)func_001A17F0() + 0x240);
    BattleModelEntry *next;
    while (entry != 0) {
        next = entry->next;
        btlReleaseModelEntry(entry);
        entry = next;
    }
}

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A68F8);

void btlFormatModelResourcePath(s32 isDevil, s32 modelId, char *filename) {
    extern void func_003014F0(char *, const char *, const char *, s32);
    if (isDevil == 0) {
        func_003014F0(filename, "%spc%03X_ms.LB", "/model/human/", modelId);
    } else {
        func_003014F0(filename, "%s%03X_ms.LB", "/model/devil/", modelId);
    }
}

extern s32 mdlRequestAsset(s32, s32, s32);

extern s32 fileRequestIsReady(void *);

s32 func_002114E8(u8 *task) {
    s32 result;
    if (*(s8 *)(task + 0xc) != 0) {
        return 1;
    }
    if (mdlRequestAsset(*(s32 *)task, *(s32 *)(task + 4), 0) == 0 ||
        mdlRequestAsset(*(s32 *)task, *(s32 *)(task + 4), 0) == -1) {
        return 0;
    }
    if (*(void **)(task + 0x10) == 0) {
        return 1;
    }
    result = fileRequestIsReady(*(void **)(task + 0x10));
    return (s8)result;
}

s32 btlFindModelEntry(kind, id)
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
    btlReleaseAllModelEntries();
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00211600);

void func_00211708(void) {
    s32 temp_v0;

    temp_v0 = btlFindModelEntry();
    if (temp_v0 != 0) {
        btlReleaseModelEntry((BattleModelEntry *)temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00211740);

s32 btlGetEntryState(s32 kind, s32 value) {
    u8 *entry = (u8 *)btlFindModelEntry(kind, value);
    if (entry != 0) {
        return *(s8 *)(entry + 0xc);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002118A8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002118D8);

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 btlMulColor(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

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

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00211A60);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00211B88);

extern void sdfBuildPacketE(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_00211CE8(s32 packet, s32 first, s32 second, s32 color) {
    sdfBuildPacketE(packet, second, first, 0x7000, 0x7900, 0x9000, 0x7900, 0x7000,
                  0x8700, 0x9000, 0x8700, color, 0);
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00211D40);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002121E8);

extern f32 func_002F9F60(f32);

extern f32 func_002FA060(f32);

extern void *func_002CFEB8(s32);

void btlInitVisibilityGrid(void) {
    f32 angle = 3.1415927f / 6.0f; /* 30 degrees */
    s32 x;
    s32 y;
    D_003D7580.cellWidth = func_002F9F60(angle) * 16.0f + 1.25f;
    D_003D7580.cellHeight = (func_002FA060(angle) * 16.0f + 16.0f + 2.5f) * 0.5f;
    D_003D7580.gridWidth = 0x200 / (D_003D7580.cellWidth * 2) + 1;
    D_003D7580.gridHeight = 0x1C0 / (D_003D7580.cellHeight * 2) + 1;
    D_003D7580.ownedData = func_002CFEB8(D_003D7580.gridWidth * D_003D7580.gridHeight);
    D_003D7580.unk_3C[0] = 0;
    for (y = 0; y < D_003D7580.gridHeight; y++) {
        for (x = 0; x < D_003D7580.gridWidth; x++) {
            ((s8 *)D_003D7580.ownedData)[y * D_003D7580.gridWidth + x] = -0x80;
        }
    }
}

void btlReleaseOwnedData(void) {
    extern void func_002CFF98(void *);
    void *data = D_003D7580.ownedData;
    if (data != 0) {
        func_002CFF98(data);
        D_003D7580.ownedData = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00212680);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002127A8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00212998);

void btlInitFadeColors(void) {
    btlInitVisibilityGrid();
    D_003D7580.color10 = 0x80808080;
    if (D_003D7580.unk_06 < 2) {
        D_003D7580.color1C = 0x20FFFFFF;
        D_003D7580.color14 = 0x20FFFFFF;
    } else {
        D_003D7580.color1C = 0x00FFFFFF;
        D_003D7580.color14 = 0x08FFFFFF;
    }
    D_003D7580.unk_24 = -0.045f;
    D_003D7580.color18 = 0x00808080;
    D_003D7580.color20 = 0;
}

typedef struct BattleGraphicsCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} BattleGraphicsCallback;

extern BattleGraphicsCallback D_00325708;

extern u32 sdfCreateInitializedPacketList(void);

extern void func_00212998(u32, u32, u32, u32, u32, s32);

s32 btlUpdateFadeIn(void) {
    u32 packets = sdfCreateInitializedPacketList();
    if ((D_003D7580.color18 & 0xFF000000) != 0x80000000) {
        D_003D7580.color18 += 0x10000000;
    }
    func_00212998(packets, D_003D7580.color14, D_003D7580.color1C, D_003D7580.color10, D_003D7580.color18, -0x100);
    D_00325708.invoke(&D_00325708, (void *)packets);
    if ((D_003D7580.color14 & 0xFF000000) > 0x08000000) {
        D_003D7580.color14 -= 0x08000000;
        D_003D7580.color1C -= 0x08000000;
        return 0;
    }
    if (D_003D7580.color1C & 0xFF000000) {
        D_003D7580.color1C -= 0x02000000;
        return 0;
    }
    return 1;
}

void func_00213368(void) {
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00213370);

extern void func_001054D0(s32, s32, f32);

extern u8 D_00325870[];

void func_00213538(void) {
    u8 *entry;
    s32 i;
    u64 clearValue;
    func_001054D0(0x200, 0xe0, 0.0f);
    kwlnTextureSetReferenceFlagIfPresent();
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

void btlReleaseRuntimeResource(void) {
    extern void effReleaseSharedReference(void *);
    extern void func_00105618(void);
    extern void kwlnTextureReleaseHeldReference(void);
    void *resource = D_003D7580.resource;
    if (resource != 0) {
        effReleaseSharedReference(resource);
        D_003D7580.resource = 0;
    }
    func_00105618();
    kwlnTextureReleaseHeldReference();
}

extern void *func_002D0518(s32);

extern void *sdfResourceRetainAddress(void *);

extern void *sdfAllocatePacketList(s32);

extern void *sdfAllocPacketAligned(s32);

extern void func_002D4540(void *);

extern void func_002D38B8(void *, void *, s32, s32, s32, s32, void *, s32, s32, s32);

extern void func_002D4588(void *, void *);

extern u8 D_00325860[];

void btlInitializeGraphicsRuntime(void) {
    BattleRuntimeState *runtime = &D_003D7580;
    void *surface;
    void *context;
    runtime->handle = func_002D0518(0x70000);
    runtime->request = sdfResourceRetainAddress(runtime->handle);
    surface = sdfAllocatePacketList(0);
    context = sdfAllocPacketAligned(16);
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
    void *surface = sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket(surface, *(s32 *)(D_003BA8F8 + 0x10), 0, 0, 0x200, 0xe0, runtime->request, 0);
    D_00325708.invoke(&D_00325708, surface);
    func_002D0A10(runtime->handle);
    runtime->handle = 0;
    runtime->request = 0;
    runtime->options |= 1;
}

extern void func_002D5CD0(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void btlInitializeOverlayGraphics(void) {
    void *surface = sdfAllocatePacketList(0);
    void *context = sdfAllocPacketAligned(16);
    func_002D4540(context);
    func_002D5CD0(surface, context, *(s32 *)(D_003BA8F8 + 0x10), 0, 0, 0, 0, 0x200, 0xe0, 0, 0);
    func_002D4588(D_00325860, context);
    D_00325708.invoke(&D_00325708, surface);
    D_003D7580.options |= 1;
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00213808);

extern void *memset(void *, s32, u32);

void btlClearRuntimeState(void) {
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

void btlResetRuntimeState(void);

void btlResetAsyncState(void) {
    extern void func_002D0A10(void *);
    void *handle = D_003D7580.handle;
    if (handle != 0) {
        func_002D0A10(handle);
        D_003D7580.handle = 0;
        D_003D7580.request = 0;
    }
    btlResetRuntimeState();
}

void btlActivateRuntime(u8 condition) {
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

void btlResetRuntimeState(void) {
    D_003D7580.state = 0;
    D_003D7580.active = 0;
    btlReleaseOwnedData();
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

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6A90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6AA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6AB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6AC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6AD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6AE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6AF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B68);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6B98);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6BB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6BC8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6BE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6BF8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C28);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6C90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6CA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6CB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6CC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6CD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6CE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6CF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6D90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6DA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6DF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6E90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6EA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6EB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6EC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A6EF8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00213BE0);

typedef struct PadButtons {
    s8 unk_0;
    s8 stick;
    u8 unk_2[2];
    u8 b4, b5, b6, b7;
} PadButtons;

extern PadButtons D_00398628[];

typedef struct MenuList {
    u32 count;
    u32 cursor;
    u32 top;
    u32 rows;
} MenuList;

s32 mnuListMoveCursor(MenuList *list) {
    if (D_00398628->b6 & 2) {
        if (list->cursor != 0) {
            list->cursor--;
            if (list->cursor == list->top && list->cursor != 0) {
                list->top = list->cursor - 1;
            }
        } else {
            list->cursor = list->count - 1;
            list->top = list->count - list->rows;
        }
    } else if (D_00398628->b7 & 2) {
        if (list->cursor >= list->count - 1) {
            list->cursor = 0;
            list->top = 0;
        } else {
            list->cursor++;
            if (list->cursor == list->top + list->rows - 1 && list->top < list->count - list->rows) {
                list->top++;
            }
        }
    } else if (D_00398628->b5 & 2) {
        if (list->top + list->rows * 2 < list->count) {
            list->top += list->rows;
            list->cursor += list->rows;
        } else {
            list->cursor = list->count - 1;
            list->top = list->count - list->rows;
        }
    } else if (D_00398628->b4 & 2) {
        if (list->top >= list->rows) {
            list->top -= list->rows;
            list->cursor -= list->rows;
        } else {
            list->cursor = 0;
            list->top = 0;
        }
    } else if (D_00398628->stick < 0) {
        return 1;
    }
    return 0;
}

extern void *func_00197748(s32, s32, u32, u32, s32, s32);

extern void func_001958A0(void *, s32, s32);

extern s32 func_00194920(void *);

s32 func_00214438(s32 width, s32 height, s32 mode) {
    void *packet = func_00197748(width << 4, height << 3, 0xff0000, 0xa09dc380, mode, 0);
    func_001958A0(packet, 0, 0x60);
    return func_00194920(packet);
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00214490);

extern void func_001FB140(u8 *, u8 *, s32, s32, u32, u32);

extern s32 func_00214490(u8 *, u8 *, s32, u8 *, s32);

s32 func_00214588(u8 *first, u8 *second, s32 mode, u8 *settings, s32 extra) {
    s32 offset = *(s32 *)(settings + 0xc) * 24 + 4;
    func_001FB140(first - 4, second - 4, mode, offset, 0x80806020, 0x30000000);
    return func_00214490(first, second, mode, settings, extra);
}

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, s32);

extern s32 func_002E4960(s32, s32, s32, s32, void *, u32);

extern u8 D_003BBAA8[];

typedef struct BtlMenuDrawer {
    u8 unk_00[0x10];
    void (*draw)(struct BtlMenuDrawer *, s32);
} BtlMenuDrawer;

extern BtlMenuDrawer D_00325748;

s32 func_00214618(u8 *x, u8 *y, s32 mode, u8 *menu, s32 extra) {
    void *handle;
    u32 first;
    u32 count;
    u32 index;
    u32 end;
    u32 selected;
    s32 rowY;
    func_001FB140(x - 4, y - 4, mode, *(s32 *)(menu + 0xC) * 24 + 4, 0x80806020, 0x30000000);
    handle = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(handle);
    first = *(u32 *)(menu + 8);
    count = *(u32 *)(menu + 0xC);
    end = first + count;
    index = first;
    selected = *(u32 *)(menu + 4);
    if (index < end) {
        rowY = (s32)y * 8 + 0x7900;
        for (; index < end; index++) {
            s32 flag = 4;
            if (index != selected) {
                flag = 0;
            }
            sdfAppendPacket(handle, func_002E4960((s32)x * 16 + 0x7000, rowY, 0xFF0000, flag, D_003BBAA8, index));
            rowY += 0xC0;
        }
    }
    D_00325748.draw(&D_00325748, (s32)handle);
    return func_00214490(x + 0x2C, y, mode, menu, extra);
}

extern s32 D_003BAA70;

extern s32 D_003BAA74;

extern s32 D_003BAA84;

void btlInitDrawTables(void) {
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

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00214868);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00215A50);

void func_00215FE0(void) {
    D_003BBB0D = 0;
    dds3WorkClear();
}

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7128);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7138);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7148);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7158);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7168);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7178);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7188);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7198);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A71A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A71B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A71C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A71D8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A71E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A71F8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7208);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7218);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7228);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7238);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7248);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7258);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7268);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7278);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7288);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7298);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A72A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A72B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A72C8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00215FF8);

INCLUDE_ASM(const s32, "game/code_0020EA40", func_002162E0);

void func_002166A8(void) {
    s32 i;

    D_003BD878 = sdfCreateSemaphore(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        D_00367940[i] = 0;
        D_00367960[i] = 0;
    }
}

s32 *btlFindGroupedEntity(group, type)
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

s32 btlGroupContainsId(s32 group, s32 id) {
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

void btlAddGroupId(s32 group, s32 id) {
    BattleGroupIdEntry *node = func_002CFEB8(sizeof(BattleGroupIdEntry));
    BattleGroupIdEntry **head = (BattleGroupIdEntry **)&D_00367960[group];
    node->id = id;
    node->next = *head;
    *head = node;
}

void btlRemoveGroupId(s32 arg0, s32 arg1) {
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
    u16 group;
    u16 type;
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

void battleCreateGroupNode(s32 group, s32 type, s32 flag, s32 arg3, s32 arg4, s32 arg5) {
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

extern void func_002177D0(s32);

extern void func_00219CE8(s32);

extern void func_002D0918(s32);

extern void sdfResourceListRelease(void *, s32);

extern void func_002CFF98(void *);

void battleDestroyGroupNode(BattleGroupNode *node) {
    BattleGroupNode *prev;
    BattleGroupNode *next;
    u8 flag;
    s32 i;
    if (node == NULL) {
        return;
    }
    prev = node->prev;
    next = node->next;
    if (prev == NULL) {
        D_00367940[node->group] = (s32)next;
    } else {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    flag = node->flag;
    node->flag = 0;
    if (node->unk_10 != 0) {
        do {
            func_002177D0(node->unk_10);
        } while (node->unk_10 != 0);
    }
    if (flag != 0) {
        sdfResourceListRelease((void *)node->unk_14, 1);
        func_002D0A10((void *)node->unk_1C);
        for (i = 0; i != 8; i++) {
            if (node->slots[i].unk_C != 0) {
                func_002D0918(node->slots[i].unk_C);
            }
        }
    }
    func_00219CE8(node->unk_A8);
    func_002D0918(node->unk_A0);
    func_002CFF98(node);
}

void func_00216A70(void) {
    s32 *temp_v0;

    temp_v0 = btlFindGroupedEntity();
    battleDestroyGroupNode(temp_v0);
}

void btlReleaseAllEntities(void) {
    u32 i = 0;
    s32 *head = D_00367940;
    do {
        s32 *node = (s32 *)*head;
        while (node != 0) {
            s32 *next = (s32 *)*node;
            battleDestroyGroupNode(node);
            node = next;
        }
        i++;
        head++;
    } while (i < 8);
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_00216B00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A72F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7300);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7310);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7320);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7330);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7340);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7350);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7360);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7370);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7380);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7390);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A73A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A73B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A73C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A73D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A73E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A73F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7400);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7410);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7420);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7430);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7440);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7450);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7460);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7470);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7480);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7490);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A74A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A74B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A74C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A74D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A74E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A74F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7500);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7510);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7520);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7530);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7540);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7550);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7560);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7570);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7580);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7590);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A75A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A75B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A75C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A75D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A75E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A75F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7600);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7610);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7620);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7630);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7640);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7650);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7660);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7670);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7680);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7690);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A76A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A76B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A76C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A76D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A76E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A76F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7700);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7710);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7720);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7730);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7740);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7750);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7760);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7770);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7780);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7790);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A77A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A77B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A77C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A77D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A77E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A77F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7800);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7810);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7820);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7830);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7840);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7850);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7860);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7870);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7880);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7890);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A78A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A78B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A78C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A78D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A78E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A78F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7900);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7910);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7920);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7930);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7940);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7950);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7960);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7970);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7980);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7990);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A79A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A79B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A79C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A79D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A79E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A79F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7A90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7AA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7AB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7AC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7AD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7AE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7AF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7B90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7BA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7BB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7BC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7BD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7BE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7BF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7C90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7CA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7CB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7CC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7CD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7CE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7CF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7D90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7DA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7DB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7DC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7DD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7DE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7DF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7E90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7EA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7EB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7EC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7ED0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7EE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7EF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7F90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7FA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7FB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7FC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7FD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7FE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A7FF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8000);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8010);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8020);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8030);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8040);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8050);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8060);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8070);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8080);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8090);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A80A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A80B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A80C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A80D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A80E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A80F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8100);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8110);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8120);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8130);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8140);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8150);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8160);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8170);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8180);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8190);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A81A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A81B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A81C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A81D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A81E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A81F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8200);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8210);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8220);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8230);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8240);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8250);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8260);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8270);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8280);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8290);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A82A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A82B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A82C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A82D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A82E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A82F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8300);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8310);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8320);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8330);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8340);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8350);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8360);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8370);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8380);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8390);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A83A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A83B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A83C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A83D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A83E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A83F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8400);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8410);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8420);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8430);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8440);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8450);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8460);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8470);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8480);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8490);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A84A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A84B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A84C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A84D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A84E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A84F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8500);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8510);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8520);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8530);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8540);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8558);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8570);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8588);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A85A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A85B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A85D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A85E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8600);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8618);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8630);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8648);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8660);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8678);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8690);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A86A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A86C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A86D8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A86F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8708);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8720);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8738);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8750);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8768);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8780);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8798);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A87B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A87C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A87E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A87F8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8810);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8828);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8840);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8850);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8860);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8870);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8880);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8890);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A88A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A88B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A88C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A88D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A88E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A88F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8900);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8910);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8920);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8930);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8940);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8950);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8960);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8970);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8980);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8990);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A89A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A89B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A89C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A89D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A89E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A89F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8A90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8AA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8AB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8AC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8AD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8AE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8AF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8B90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8BA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8BB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8BC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8BD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8BE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8BF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8C90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8CA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8CB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8CC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8CD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8CE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8CF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8D90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8DA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8DB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8DC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8DD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8DE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8DF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8E90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8EA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8EB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8EC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8ED0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8EE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8EF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8F90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8FA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8FB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8FC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8FD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8FE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A8FF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9000);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9010);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9020);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9030);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9040);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9050);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9060);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9070);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9080);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9090);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A90A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A90B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A90C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A90D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A90E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A90F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9100);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9110);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9120);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9130);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9140);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9150);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9160);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9170);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9180);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9190);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A91A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A91B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A91C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A91D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A91E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A91F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9200);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9210);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9220);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9230);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9240);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9250);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9260);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9270);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9280);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9290);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A92A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A92B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A92C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A92D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A92E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A92F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9300);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9310);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9320);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9330);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9340);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9350);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9360);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9370);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9380);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9390);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A93A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A93B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A93C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A93D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A93E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A93F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9400);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9410);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9420);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9430);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9440);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9450);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9460);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9470);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9480);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9490);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A94A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A94B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A94C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A94D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A94E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A94F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9500);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9510);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9520);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9530);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9540);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9550);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9560);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9570);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9580);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9590);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A95A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A95B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A95C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A95D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A95E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A95F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9600);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9610);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9620);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9630);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9640);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9650);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9660);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9670);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9680);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9690);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A96A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A96B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A96C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A96D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A96E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A96F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9700);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9710);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9720);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9730);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9740);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9750);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9760);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9770);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9780);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9790);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A97A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A97B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A97C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A97D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A97E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A97F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9800);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9810);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9820);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9830);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9840);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9850);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9860);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9870);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9880);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9890);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A98A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A98B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A98C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A98D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A98E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A98F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9900);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9910);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9920);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9930);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9940);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9950);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9960);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9970);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9980);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9990);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A99A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A99B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A99C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A99D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A99E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A99F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9A90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9AA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9AB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9AC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9AD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9AE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9AF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9B90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9BA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9BB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9BC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9BD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9BE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9BF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9C90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9CA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9CB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9CC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9CD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9CE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9CF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9D90);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9DA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9DB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9DC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9DD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9DE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9DF0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E58);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E88);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9E98);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9EB0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9EC8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9EE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9EF8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9F10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9F28);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9F40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9F58);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9F70);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9F88);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9FA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9FB8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9FD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A9FE8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA000);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA018);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA030);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA048);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA060);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA078);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA090);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA0A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA0B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA0D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA0E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA100);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA110);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA128);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA140);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA158);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA168);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA178);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA188);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA198);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA1A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA1C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA1D8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA1F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA208);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA218);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA228);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA238);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA248);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA258);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA270);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA288);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA2A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA2D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA2F8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA328);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA350);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA378);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA3A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA3C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA3F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA418);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA440);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA468);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA490);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA4B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA4E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA508);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA530);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA558);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA580);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA5A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA5D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA5F8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA620);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA648);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA670);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA698);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA6C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA6E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA710);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA738);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA760);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA788);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA7B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA7D8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA800);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA828);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA850);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA878);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA8A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA8C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA8F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA918);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA940);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA968);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA990);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA9B8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AA9E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAA08);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAA30);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAA58);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAA80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAAA8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAAD0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAAF8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAB20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAB48);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAB60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAB80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AABA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AABC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AABE0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAC00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAC20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAC40);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAC60);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAC80);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AACA0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AACC0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AACE8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAD08);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAD28);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAD48);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAD68);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAD88);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AADA8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AADC8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AADE8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAE08);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAE28);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAE48);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAE68);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAE88);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAEA8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAEC8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAEE8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAF08);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAF28);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAF48);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAF68);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAF88);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAFA8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAFC8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AAFE8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB008);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB028);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB048);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB068);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB088);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB0A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB0C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB0E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB108);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB128);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB148);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB168);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB188);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB1A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB1C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB1E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB208);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB228);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB248);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB268);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB288);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB2A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB2C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB2E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB308);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB328);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB348);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB368);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB388);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB3A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB3C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB3E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB408);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB428);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB448);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB468);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB488);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB4A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB4C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB4E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB508);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB528);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB548);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB568);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB588);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB5A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB5C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB5E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB608);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB628);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB648);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB668);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB688);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB6A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB6C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB6E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB708);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB728);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB748);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB768);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB788);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB7A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB7C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB7E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB808);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB828);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB848);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB868);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB888);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB8A8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB8C8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB8E8);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB908);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB928);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB940);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB958);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB970);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB988);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB9A0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB9B0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB9C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB9D0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB9E0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003AB9F0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003ABA00);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003ABA10);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003ABA20);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003ABA38);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003ABA50);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003ABA68);

