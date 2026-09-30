#include "common.h"

/* The boss classification is checked by entry lookup and HEKATO scaling. */
#define BTL_UNIT_BOSS_FLAG 0x400
/* The debug text in btlAccumulateBossRatioScale names skill 0x1A9. */
#define BTL_SKILL_HEKATO 0x1A9

typedef struct BtlUnit BtlUnit;

typedef struct BtlUnitExt {
    u8 pad0[0xA8];
    u32 flags;
} BtlUnitExt;

typedef struct BtlSub718 {
    union {
        s32 task;
        f32 scale;
        struct {
            u8 pad0[2];
            s8 active;
        } b;
    };
    s32 targetMode;
    s8 b8;
} BtlSub718;

typedef struct BtlWork {
    u8 pad0[0x208];
    s32 unk208;
    u8 pad20C[0x40];
    BtlUnit *actorList;
    u8 pad250[0x18];
    u16 unk268;
    u8 pad26A[0x36];
    s32 mode;
    u8 pad2A4[0x474];
    BtlSub718 *sub;
} BtlWork;

typedef struct BtlTask {
    u8 kind;
    u8 pad1[7];
    u64 arg;
    u8 pad10[0x28];
    u64 result;
    u64 flags;
} BtlTask;

typedef struct BtlSkillTask {
    u8 pad0[8];
    u32 flags;
    u8 pad0C[0xC];
    BtlUnit *unit;
    u8 pad1C[8];
    s32 kind;
    u8 pad28[0x20];
    s32 adjustedValue;
    s8 resultKind;
    u8 pad4D[0x13];
    s32 list60;
} BtlSkillTask;

typedef struct BtlEffect {
    u8 pad0[0x10];
    f32 vec10[4];
    u8 pad20[0x10];
    f32 vec30[4];
    f32 vec40[4];
    f32 f50;
    u8 pad54[0x6C];
    f32 vecC0[4];
    f32 vecD0[4];
    f32 fE0;
    u8 padE4[0x2C];
    u32 flags;
    BtlSkillTask *task;
    u8 pad118[0x18];
    s32 unk130;
    s32 index134;
    u8 pad138[0x1C];
    f32 unk154;
} BtlEffect;

typedef struct BtlEntry {
    u8 pad0[3];
    u8 kind;
    u8 pad4[0x18];
    u16 flags1C;
    u8 pad1E[2];
} BtlEntry;

typedef struct BtlParams {
    u8 pad0[0xBF4];
    f32 ratioScale;
    f32 ratioMax;
} BtlParams;

struct BtlUnit {
    u8 pad0[0x110];
    u32 flags;
    u32 stateFlags;
    u8 pad118[4];
    u8 state;
    u8 pad11D[7];
    u16 mode;
    u8 pad126[0x21A];
    BtlUnitExt *ext;
    u8 pad344[0x20];
    BtlUnit *next;
};

extern BtlParams *D_00435E44;
extern BtlEntry *D_00435E30;
extern BtlWork *func_001AA6F8(void);
extern s32 btlBossDebugPrintf(const char *, ...);
extern BtlTask *func_001E5FF8(s32, s32);
extern void btlStartTask(BtlTask *);
extern s32 func_001B2430(BtlUnit *, s32);
extern BtlTask *sndCreateStationedSeTask(s32);
extern s32 func_001AB9F0(BtlUnit *, s32);
extern s32 func_001B3610(BtlUnit *, s32, s32, s32, s32);
extern s8 func_001B36B8(s32, s32, s32);
extern void func_001EC868(void *, f32 *, f32);
extern void func_001E9598(void *, f32 *);
extern void func_00336538(f32);
extern void func_001E9A88();
extern void func_001E9660(BtlEffect *, f32, f32, f32, f32, f32, f32, f32, f32);
extern BtlUnit *func_002172B8(BtlEffect *);
extern void func_003364B8(f32);
extern void func_00336818(f32);
extern void func_00336AA8(void);
extern void func_001E96C8(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern s32 func_0021C5E0();

void btlCancelCurrentSubtask(void) {
    BtlSub718 *sub;
    s32 task;

    sub = func_001AA6F8()->sub;
    task = sub->task;
    if (task != 0) {
        btlDestroyUnit(task);
        sub->task = 0;
    }
}

/* Allocate and launch a subtask from the active battle task slot. */
u64 btlStartSubtaskWithInput(u64 input) {
    BtlTask *task = func_001E5FF8(func_001AA6F8()->sub->task, 0xC);
    if (input != 0) {
        task->arg = input;
        task->kind = 4;
    }
    task->flags = 0x8000000000000003;
    btlStartTask(task);
    return task->result;
}

void func_0021B670(BtlUnit *unit) {
    if (unit->flags & BTL_UNIT_BOSS_FLAG) {
        if (unit->flags & 2) {
            unit->ext->flags |= 0x1000000;
            unit->stateFlags |= 0x20000;
            unit->stateFlags |= 0x400000;
        }
    }
}

f32 func_0021B6C0(BtlUnit *unit, BtlUnit *target) {
    BtlUnit *other;
    f32 scale = 1.0f;
    if (unit->flags & 0x200) {
        if (target->mode == 0x110) {
            for (other = func_001AA6F8()->actorList; other != 0; other = other->next) {
                if (other->flags & 1) {
                    if (other->flags & BTL_UNIT_BOSS_FLAG) {
                        if (!(other->flags & 0xE0)) {
                            if (other->mode >= 0x111 && other->mode < 0x113) {
                                break;
                            }
                        }
                    }
                }
            }
            scale = other != 0 ? 1000.0f : 1.0f;
        }
    }
    return scale;
}

void btlSetSkillTaskResults(BtlSkillTask *task, s32 arg1, s32 arg2, s32 skillId) {
    s32 percent = 100;
    if (skillId >= 0x1AB && skillId < 0x220) {
        percent *= func_001AB9F0(task->unit, skillId);
    }
    task->adjustedValue = func_001B3610(task->unit, arg1, arg2, percent, skillId);
    task->resultKind = func_001B36B8(arg1, arg2, skillId);
}

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021B828);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C0C8);

void func_0021C390(u8 *obj) {
    func_001E9A88(obj);
    func_001E96C8(obj, -176.6f, -137.8f, -1564.9f, -0.01f, -0.038f, -0.012f, 0.99f, -217.8f,
                  -253.6f, -2272.8f, -0.009f, -0.038f, -0.013f, 0.99f, 40.0f, 15.0f);
}

void func_0021C428(BtlEffect *fx) {
    switch (fx->task->unit->mode) {
    case 0x111:
        func_001E96C8(fx, 533.4f, -167.8f, -1104.7f, -0.052f, 0.285f, -0.028f, 0.947f, 430.5f,
                      -91.7f, -1373.1f, -0.089f, 0.198f, -0.03f, 0.967f, 40.0f, 15.0f);
        break;
    case 0x112:
        func_001E96C8(fx, -440.9f, -333.4f, -1307.2f, 0.026f, -0.223f, -0.017f, 0.965f, -844.6f,
                      -358.7f, -1194.1f, 0.024f, -0.346f, -0.02f, 0.928f, 40.0f, 10.0f);
        break;
    }
}

s32 btlInitializeEffectVectors(BtlEffect *fx) {
    f32 *vec = fx->vec30;
    fx->vec10[0] = 1.0f;
    func_001EC868(fx, vec, 25.0f);
    func_001E9598(fx->vecC0, vec);
    func_00336538(-0.87266463f);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(fx->vec40) : "memory");
    __asm__ volatile(".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\tvmaddz.xyzw vf10, vf30, vf10z\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(fx->vecD0) : "memory");
    fx->unk154 = 125.0f;
    fx->flags |= 0x841;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C5E0);

void func_0021C7F8(void) {
    func_0021C5E0();
}

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021C818);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041A5E0);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041A5F8);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021CF18);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021E778);

INCLUDE_ASM(const s32, "game/code_0021B5C0", func_0021E8C0);

s32 btlMapBossEntryKindToIndex(BtlUnit *unit, s32 index) {
    if (!(unit->flags & BTL_UNIT_BOSS_FLAG)) {
        return -1;
    }
    if (D_00435E30[index].kind == 0) {
        return -1;
    }
    if (D_00435E30[index].kind >= 11 && D_00435E30[index].kind < 26) {
        return -1;
    }
    switch (D_00435E30[index].kind) {
    case 1: return 0xC;
    case 2: return 0xD;
    case 3: return 0xE;
    case 4: return 0xF;
    case 5: return 0x10;
    case 6: return 0x11;
    case 7: return 0x12;
    case 8: return 0x13;
    case 9: return 0x14;
    case 10: return 0x15;
    default: return 0xD;
    }
}

s32 btlGetBossEntryKind(BtlUnit *unit, s32 index) {
    if (!(unit->flags & BTL_UNIT_BOSS_FLAG)) {
        return -1;
    }
    if (D_00435E30[index].kind == 0) {
        return -1;
    }
    return D_00435E30[index].kind;
}

s32 func_0021EB28(BtlUnit *unit, s32 value) {
    s32 mode;
    if (!(unit->flags & BTL_UNIT_BOSS_FLAG)) {
        return value;
    }
    mode = unit->mode;
    if (mode < 0x113) {
        if (mode >= 0x111) {
            if (value == 4) {
                return 5;
            }
            if (value == 0xB) {
                return 0;
            }
        }
    }
    return value;
}

void func_0021EB78(BtlUnit *unit) {
    s32 mode;
    if (unit->flags & BTL_UNIT_BOSS_FLAG) {
        if (func_001B2430(unit, 0)) {
            mode = unit->mode;
            if (mode < 0x113) {
                if (mode >= 0x111) {
                    btlStartTask(sndCreateStationedSeTask(func_001AA6F8()->unk208 + 1));
                }
            }
        }
    }
}

s32 func_0021EBF0(void) {
    BtlWork *work = func_001AA6F8();
    if (work->mode != 0x30B) {
        return 0;
    }
    if (work->sub == 0) {
        return 0;
    }
    return work->sub->b8;
}

void btlResetBossRatioScale(void) {
    func_001AA6F8()->sub->scale = 1.0f;
}

void btlAccumulateBossRatioScale(BtlSkillTask *task) {
    f32 *ratio;
    BtlParams *params;
    if (task->flags & 8) {
        if (task->unit->flags & BTL_UNIT_BOSS_FLAG) {
            ratio = &func_001AA6F8()->sub->scale;
            if (task->kind == BTL_SKILL_HEKATO) {
                params = D_00435E44;
                *ratio *= params->ratioScale;
                if (*ratio > params->ratioMax) {
                    *ratio = params->ratioMax;
                }
                btlBossDebugPrintf("btl:boss HEKATO ratio = %f\n", *ratio);
            }
        }
    }
}

f32 btlGetBossRatioScale(BtlUnit *unit, s32 unused, s32 kind, s32 flag) {
    f32 scale = 1.0f;
    if (kind == BTL_SKILL_HEKATO && flag == 1) {
        if (unit->flags & BTL_UNIT_BOSS_FLAG) {
            scale = func_001AA6F8()->sub->scale;
        }
    }
    return scale;
}

BtlUnit *btlFindUnitByMode(void) {
    BtlWork *work = func_001AA6F8();
    BtlSub718 *sub = work->sub;
    BtlUnit *unit;
    if (sub->b.active == 0) {
        return 0;
    }
    for (unit = work->actorList; unit != 0; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x200) {
                if (sub->targetMode == unit->mode) {
                    return unit;
                }
            }
        }
    }
    return 0;
}

u64 btlMaskValueWhenSubtaskInactive(u64 value) {
    BtlWork *work;
    u64 result;

    work = func_001AA6F8();
    result = 0;
    if (work->sub->b.active != '\0') {
        result = value;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AAC8);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AAD8);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AAF8);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB18);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB28);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB38);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB48);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB58);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB68);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB78);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB88);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041AB98);

INCLUDE_RODATA(const s32, "game/code_0021B5C0", D_0041ABA8);

