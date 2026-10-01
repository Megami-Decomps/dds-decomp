#include "common.h"
#include "btl.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern u32 btlGetIndexListCount();

extern char D_003BB8A0[];

extern char D_003BB898[];

extern char D_003BB8A8[];

extern s32 D_003BAA60;

extern void func_003014F0();

extern s32 func_001A17F0(void);

extern void func_001D6300(void *, void *);

extern void btlSetUnitPosition(BtlUnit *, void *);

typedef struct BtlUnit {
    u8 unk_00[0x30];
    f32 position[4]; /* 0x30: current world position, passed to btlSetUnitPosition */
    u8 unk_40[0x50];
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
    u16 unk_120;
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

extern s32 func_0020DC38();

extern void (*D_00360E38[])(BtlTask *, u32, s32);

extern s32 btlGetIndexListEntry(void *, u32);

/* Linked-command target/direction and motion fields; DDS2 moves the target/flags +0x20. */
typedef struct BtlLinkedCommand {
    u8 pad00[0x30];
    f32 position[4]; /* 0x30: source pose for the linked command */
    u8 pad40[0x10];
    f32 coordinate50; /* 0x50: increased when motion begins */
    u8 pad54[0x6C];
    f32 rotation[4]; /* 0xC0: current orientation, paired with coordinate50 */
    u8 padD0[0x10];
    f32 coordinateE0; /* 0xE0: increased along with coordinate50 */
    u8 padE4[0xC];
    u32 flags;         /* 0xF0 */
    u8 padF4[0x24];
    u32 targetList;    /* 0x118 */
    u8 pad11C[0x14];
    f32 value130;      /* 0x130: initialized to 30 for this motion */
} BtlLinkedCommand;

extern void *btlCreateUnitFadeOutTask(void *, s32, s32);

extern s64 btlStartTask(void *);

extern void func_001DB698();

extern s8 D_003BB880[];

extern s8 D_003BB888[];

extern s8 D_003BB890[];

/* Event entry carrying a battle task and its action kind. */
typedef struct BtlEventEntry {
    u8 pad00[0xF0];
    u32 flags;
    BtlTask *task; /* 0xF4 */
    u8 padF8[0xC];
    u32 kind;      /* 0x104 */
} BtlEventEntry;

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern char D_003A5FF0[];

extern char D_003A6018[];

extern void func_0020AFB8();

extern void func_0020B190(u8 *, void *);

/* 0x20-byte action metadata entries referenced by a unit's action index. */
typedef struct BtlActionTableRow {
    u8 pad00[3];
    u8 enabled; /* 0x03: zero rejects the action */
    u8 pad04[0x18];
    u16 flags;  /* 0x1C: special animation selection bits */
    u8 pad1E[2];
} BtlActionTableRow;

extern void btlClearRuntimeFlag2000(void);

s32 btlDispatchActionAnimationB(BtlUnit *unit) {
    u16 flags = ((BtlActionTableRow *)D_003BAA60)[unit->unk_114].flags;
    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            func_0020AFB8((u8 *)unit);
        } else {
            btlSetEffectCameraKeys((void *)unit, 59.2f, -700.6f, -1901.2f,
                           0.092f, 0.023f, -0.009f, 0.987f,
                           59.2f, -160.6f, -1901.2f, -0.106f,
                           0.025f, -0.014f, 0.985f, 45.0f, 30.0f);
        }
        unit->flags = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        func_0020B348((void *)unit, (void *)unit, 0);
    } else if (flags & 8) {
        if (btlGetIndexListCount(((BtlEventEntry *)unit)->task->unk_60) == 1) {
            void *other = (void *)btlGetIndexListEntry((void *)((BtlEventEntry *)unit)->task->unk_60, 0);
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020B190((u8 *)unit, other);
            unit->flags = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020AFB8((u8 *)unit);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

void func_0020CCA8(void) {
    func_0020ADA8();
}

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020CCC0);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020D168);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020D2E0);

s32 btlAllowsSpeciesCondition(BtlUnit *unit, BtlUnit *other, s32 condition) {
    s32 kind;
    if ((unit->flags & 0x200) == 0) {
        return 1;
    }
    if (condition != 0) {
        return 0;
    }
    kind = other->mode;
    switch (kind) {
    case 0x13d:
    case 0x13e:
        return 0;
    default:
        return 1;
    }
}

void btlFormatBattleEffectResourceName(s32 record, s32 action, s32 pathBuffer) {
    if (action != 0xd5) {
        return;
    }
    func_003014F0(pathBuffer, "%s%03X_%02X.BED", D_003BB8A8, *(u16 *)(D_003BAA60 + 0x1aa4), *(s32 *)(record + 0x38) - 0x13d);
}

u32 func_0020D548(void) {
    return 7;
}

s32 func_0020D550(s32 battler, s32 action) {
    BtlUnit *unit = (BtlUnit *)battler;
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)D_003BAA60)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x114) {
        return 11;
    }
    return -1;
}

s32 func_0020D598(s32 battler, s32 action) {
    BtlUnit *unit = (BtlUnit *)battler;
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)D_003BAA60)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x111) {
        return 11;
    }
    return -1;
}

s32 func_0020D5E0(BtlUnit *unit, s32 unused1, s32 unused2) {
    BtlTask *link = unit->task;
    BtlUnit *other;
    f32 pos[4] __attribute__((aligned(16)));

    if (link == 0) {
        return 1;
    }
    other = link->unit;
    if ((other->flags & 0x400) == 0) {
        return 0;
    }
    if (other->mode != 0x111) {
        return 0;
    }
    if (unit->unk_114 == 0x175) {
        PCP_COPY_VECTOR(pos, other->position);
        pos[2] += 600.0f;
        btlSetUnitPosition(other, pos);
    }
    return 0;
}

extern s32 func_0020D5E0();

s64 func_0020D668(void *unit, s8 unused1, s8 unused2) {
    return func_0020D5E0(unit, unused1, unused2);
}

s32 func_0020D690(s32 battler, s32 action) {
    BtlUnit *unit = (BtlUnit *)battler;
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)D_003BAA60)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x109) {
        return 11;
    }
    return -1;
}

s32 btlDeactivateOthersOnSpecialUnitDefeat(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x13C) {
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
                            if (unit->mode != 0x13C) {
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

s32 btlNormalizeActionForSkill(s32 battler, s32 action) {
    BtlUnit *unit = (BtlUnit *)battler;
    if ((unit->flags & 0x400) == 0 || unit->mode != 0x13c) {
        return action;
    }
    switch (action) {
    case 2: return 0;
    case 13: return -1;
    default: return action;
    }
}

void btlRecenterUnitsOnLead(void) {
    BtlState *work = (BtlState *)func_001A17F0();
    BtlUnit *unit;
    BtlUnit *lead = 0;
    f32 shift;
    f32 pos[4];

    for (unit = work->units; unit != 0; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x137) {
                    lead = unit;
                    break;
                }
            }
        }
    }
    if (lead != 0) {
        func_001D6300(lead, pos);
        shift = -pos[0];
        pos[0] = 0;
        PCP_COPY_VECTOR(lead->position, pos);
        btlSetUnitPosition(lead, pos);
        for (unit = work->units; unit != 0; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit != lead) {
                        func_001D6300(unit, pos);
                        pos[0] = pos[0] + shift;
                        PCP_COPY_VECTOR(unit->position, pos);
                        btlSetUnitPosition(unit, pos);
                    }
                }
            }
        }
    }
}

u32 btlMapActionToCode(s32 action) {
    u32 result;

    result = 0x7d;
    if (action != 0x143) {
        result = 0;
    }
    return result;
}

u8 func_0020D9A8(s32 action) {
    return action != 0xd3;
}

s32 btlNormalizeActionForStatus(s32 battler, s32 action) {
    BtlUnit *unit = (BtlUnit *)battler;
    if ((unit->flags & 0x400) == 0 || unit->mode != 0x115) {
        return action;
    }
    switch (action) {
    case 2: return 0;
    case 13: return -1;
    default: return action;
    }
}

s32 func_0020D9F8(s32 battler, s32 action) {
    BtlUnit *unit = (BtlUnit *)battler;
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)D_003BAA60)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x113) {
        return 11;
    }
    return -1;
}

void btlSelectRandomDefeatCamera(void *unit) {
    btlFlagAllUnitDefeatCandidatesTask();
    switch (effMiscRandMod(0, 2)) {
    case 0:
        func_003003F0(D_003A5FF0);
        btlSetEffectCameraKeys(unit, -307.2f, -72.7f, -1292.7f, -0.096f, -0.085f, -0.004f, 0.983f, 148.3f, -84.1f, -1407.1f,
                      -0.079f, 0.064f, -0.018f, 0.986f, 40.0f, 25.0f);
        break;
    case 1:
        func_003003F0(D_003A6018);
        btlSetEffectCameraKeys(unit, -207.7f, -76.6f, -1176.0f, -0.108f, -0.099f, -0.002f, 0.98f, -282.2f, -106.6f, -1451.2f,
                      -0.071f, -0.1f, -0.006f, 0.984f, 40.0f, 25.0f);
        break;
    }
}

extern void func_00204838(void *, void *, void *, s32, s32, f32, f32, f32);

extern void func_001DB698(void *);

void btlRaiseLinkedActionPose(BtlLinkedCommand *actor) {
    u8 *position = (u8 *)actor->position;
    u8 *rotation = (u8 *)&actor->rotation;
    func_00204838(actor, position, rotation, 0, 1, 0.25f, 0.0f, 0.5f);
    actor->value130 = 30.0f;
    actor->flags |= 0x41;
    actor->coordinate50 += 500.0f;
    actor->coordinateE0 += 500.0f;
    func_001DB698(position);
    func_001DB698(rotation);
}

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020DC38);

s64 func_0020DE50(void) {
    return func_0020DC38();
}

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020DE70);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020E058);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020E170);

extern void btlSelectRandomDefeatCamera(void *);

s32 func_0020E868(BtlUnit *unit) {
    BtlTask *entry = ((BtlEventEntry *)unit)->task;
    BtlUnit *other;
    if (entry->unit->flags & 0x200) {
        if (btlGetIndexListCount((void *)entry->unk_60) == 1) {
            other = (BtlUnit *)btlGetIndexListEntry((void *)entry->unk_60, 0);
            if ((other->flags & 0x400) == 0) {
                return 0;
            }
            btlRaiseLinkedActionPose((BtlLinkedCommand *)unit);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020DC38((void *)unit, (void *)unit, 0);
            unit->flags = 0;
        }
        return 1;
    }
    return 0;
}

s32 func_0020E910(BtlUnit *unit) {
    u16 flags = ((BtlActionTableRow *)D_003BAA60)[unit->unk_114].flags;
    if (flags & 0x1000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            btlSelectRandomDefeatCamera((void *)unit);
        } else {
            btlSetEffectCameraKeys((void *)unit, -203.0f, -531.1f, -1259.0f,
                           0.124f, -0.07f, -0.021f, 0.981f,
                           -203.0f, -46.1f, -1259.0f, -0.144f,
                           -0.066f, -0.003f, 0.978f, 45.0f, 30.0f);
        }
        return 1;
    }
    if (flags & 0x2000) {
        if (btlGetIndexListCount((void *)((BtlEventEntry *)unit)->task->unk_60) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseLinkedActionPose((BtlLinkedCommand *)unit);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSelectRandomDefeatCamera((void *)unit);
        }
        return 1;
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8A8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8B0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8B4);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8B8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8C0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8C8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8D0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8D8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8E0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8E8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8F0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8F8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB900);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB908);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB910);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB918);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB920);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB928);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB930);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB938);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB93E);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB940);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB948);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB950);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB958);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB960);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB968);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB970);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB978);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB980);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB988);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB990);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB998);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9A0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9A8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9B0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9B8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9C0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9C8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9D0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9D8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9E0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9E8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9F0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9F8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA00);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA08);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA10);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA18);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA20);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA28);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA30);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA38);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA40);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA48);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA50);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA58);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA60);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA68);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA70);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA78);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA80);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA88);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA90);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA98);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAA0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAA8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAB0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAB8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAC0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAC8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAD0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAD8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAE0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAE8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAF0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAF8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB00);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB08);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB0D);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB0E);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB0F);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB10);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB14);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB18);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB1C);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB1E);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB20);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB24);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB28);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB2C);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB30);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB38);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB40);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB48);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB50);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB58);

