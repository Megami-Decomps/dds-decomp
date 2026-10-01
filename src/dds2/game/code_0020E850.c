#include "common.h"
#include "btl.h"

extern u32 D_00436CB0;

extern void btlCmdSimpleB(s32, s32);

extern void btlCmdSimpleA(s32, s32);

extern void btlCmdSimpleD(s32, s32);

extern void btlCmdSimpleE(s32, s32);

extern void btlCmdSimpleJ(s32, s32);

extern void btlCmdSimpleC(s32, s32);

typedef struct BtlJyokyoOwner {
    u8 pad00[0x120];
    u16 flags;       /* 0x120 */
    u8 pad122[2];
    u16 unk124;      /* 0x124 */
    u8 pad126[8];
    u16 statusFlags; /* 0x12E */
} BtlJyokyoOwner;

typedef struct BtlEffectSlots {
    u8 pad00[0x21C];
    u32 flags;       /* 0x21C */
    u8 pad220[0x2AC];
    s32 id;          /* 0x4CC */
    s32 alternateId; /* 0x4D0 */
    s32 thirdId;     /* 0x4D4 */
} BtlEffectSlots;

extern s32 btlDispatchPackedActionWithScratch(s32 context, BtlJyokyoOwner *owner, s32 mask);


extern s32 btlActionScratchWork;

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

extern s32 sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(s32);

extern void func_002152D8(s32, s32);

extern void func_00215C70(s32, s32);

extern void btlSelectLowestHealthRateTarget(s32, s32);

extern void btlSelectLowestRankTarget(s32, s32);

extern void func_00216888(s32, s32);

extern void func_00216D30();

extern void func_00216D10();

extern void btlAppendSelfAfterTargetScan();

extern void func_00216988();

extern void btlAppendEffectActorToCommandIndices(s32, s32);

extern void btlAppendCurrentUnitIdToCommandIndices(s32, s32);

extern void btlSelectTargetsByMode();

typedef struct EffChildCounters {
    u8 pad00[0x338];
    u8 firstCountdown;
    u8 secondCountdown;
} EffChildCounters;

typedef struct EffCounterOwner {
    u8 pad00[0x14];
    u32 value14;
    u8 pad18[8];
    EffChildCounters *child;
} EffCounterOwner;

extern s8 D_0037F510[];

typedef struct BtlWaitTask {
    s32 value;
    u32 ticks;
} BtlWaitTask;

extern s32 (*D_003BF4A0[])(s32, u32);

typedef struct BtlEffLink {
    s32 actor;  /* 0x00 */
    s32 arg;    /* 0x04 */
    u32 unk08;  /* 0x08 */
} BtlEffLink;

typedef struct BtlEffTask {
    u8 kind;           /* 0x00 */
    u8 pad01[0xF];
    u8 unk10;          /* 0x10 */
    u8 pad11[0xF];
    u16 id;            /* 0x20 */
    u8 pad22[2];
    u16 flags;         /* 0x24 */
    u8 pad26[0x1A];
    u64 ownerData;      /* 0x40 */
    u8 pad48[4];
    s32 (*callback)(); /* 0x4C */
    void (*destroy)(); /* 0x50 */
} BtlEffTask;

typedef struct BtlEffActor {
    u8 pad00[0x108];
    u64 ownerData;    /* 0x108 */
    s32 flags;        /* 0x110: bit 9 selects the one-step offset */
} BtlEffActor;

typedef struct BtlExtendedLink {
    u8 pad00[0x20];
    BtlEffActor *owner; /* 0x20 */
    s32 state;          /* 0x24 */
    u8 parameter;       /* 0x28 */
} BtlExtendedLink;

extern BtlEffTask *btlAllocTask(s32 size);
extern BtlEffLink *btlGetTaskArguments();

typedef struct BtlHistActor {
    u8 pad00[0x2D0];
    s16 unk2D0; /* 0x2D0 */
} BtlHistActor;

typedef struct BtlHistObj {
    u8 pad00[0x18];
    BtlHistActor *actor; /* 0x18 */
    u8 pad1C[4];
    s32 mode;            /* 0x20 */
    s32 unk24;           /* 0x24 */
    u8 pad28[0x126];
    s8 counter;          /* 0x14E */
    u8 pad14F;
    s32 hist[8];         /* 0x150 */
    s32 status170;        /* 0x170: reset along with the history counter */
    u8 pad174[4];
    struct BtlHistObj *next; /* 0x178 */
} BtlHistObj;

extern void btlShiftActorStateHistory(BtlHistObj *obj, s8 flag);
extern s8 btlHistoryCounter;

typedef struct BtlPackedCtx {
    s32 context;
    s32 unk4;
} BtlPackedCtx;
extern s32 D_00435E5C;
extern void func_001A45C0(s32, s32, s32, s32);
extern void func_001B8580(s32);
extern void btlReplaceDialogTasksAndQueueMessage(s32, s32);
extern u32 func_001B8538(void);
extern u32 func_001B8740(void);
extern char D_00436CA8[];
extern s32 func_0035C860();
extern void func_001A4858(s32, s32, char *);
extern s32 D_00435E64;
extern u8 D_003BEB28[];
extern u8 D_003BEB30[];
extern s32 D_003BEB38[];
extern s32 D_003BEB48[];
extern s32 D_003BEB4C[];
extern s32 D_003BEB50[];
extern s32 D_003BEB58[];
extern s32 D_003BEB60[];
extern void *btlGetIndexedUiResource();
extern u32 btlRollAiBucket(void);
extern s32 btlPollCategoryLabelTask();
extern s32 btlPollActorOrEntryLabelTask();
extern void func_0020FA98();
extern s32 btlPollTimedTaskLink(BtlEffLink *link);
extern s32 btlPollActorDialogTask();
extern s32 func_00210530();
extern s32 btlPollTimedPresentationTask();
extern s32 btlPollEffectWaitTask(BtlEffLink *link);
extern s32 btlAdvanceActorEffectLabelTask();

typedef struct BtlEffLinkEx {
    u8 pad00[0x20];
    BtlEffActor *owner; /* 0x20 */
    s32 arg;            /* 0x24 */
    s32 unk28;          /* 0x28 */
    u8 pad2C[4];
    u8 kind;            /* 0x30 */
} BtlEffLinkEx;
extern s32 func_0020F5E0();


void func_0020E850(EffCounterOwner *owner, u32 value) {
    owner->value14 = value;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020E858);

void func_0020EA10(void) {
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EA18);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EB40);

/* Accepts a task's unit only when status bit 5 is set and task bit 6 is clear.
 * If bit 5 is the sole low status bit, the final check uses an external value. */
s32 func_0020EBD0(BtlTask *task) {
    s32 result = 0;
    BtlJyokyoOwner *unit = (BtlJyokyoOwner *)task->unit;
    u16 flags;

    if (task->flags & 0x40) {
        return result;
    }
    flags = unit->statusFlags;
    if (!(flags & 0x20)) {
        return result;
    }
    if ((flags & 0x7FFF) != 0x20) {
        return 1;
    }
    return btlRollAiBucket() < 0x46;
}

INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419910);

INCLUDE_RODATA(const s32, "game/code_0020E850", D_00419920);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020EC20);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F048);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F200);

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F3B0);

/* Adds one to the base unless bit 9 of the owner's flags is set. */
s32 effOffsetIfOwnerFlagClear(BtlEffActor *owner, s32 base) {
    return base + (((owner->flags >> 9) ^ 1U) & 1);
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020F5E0);

void effDecrementFirstCountdown(EffCounterOwner *owner) {
    EffChildCounters *unit = owner->child;
    u8 count = unit->firstCountdown;
    if (count != 0) {
        unit->firstCountdown = count - 1;
    }
}

BtlEffTask *btlCreateLinkedEffectTask(BtlEffActor *owner, s32 arg, u8 kind) {
    BtlEffTask *obj = btlAllocTask(0x34);
    BtlEffLinkEx *link;

    obj->kind = 1;
    obj->unk10 = 0;
    switch (kind) {
    case 0:
        obj->id = 0x3C;
        break;
    case 1:
        obj->id = 0x3D;
        break;
    }
    obj->flags |= 2;
    obj->ownerData = owner->ownerData;
    obj->callback = func_0020F5E0;
    obj->destroy = effDecrementFirstCountdown;
    link = (BtlEffLinkEx *)btlGetTaskArguments(obj);
    link->kind = kind;
    link->owner = owner;
    link->arg = arg;
    link->unk28 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0020E850", func_0020FA98);

void effDecrementSecondCountdown(EffCounterOwner *owner) {
    EffChildCounters *unit = owner->child;
    u8 count = unit->secondCountdown;
    if (count != 0) {
        unit->secondCountdown = count - 1;
    }
}

BtlEffTask *btlCreateEffectCounterTask(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0x2C);
    BtlExtendedLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3E;
    obj->ownerData = owner->ownerData;
    obj->callback = func_0020FA98;
    obj->destroy = effDecrementSecondCountdown;
    link = (BtlExtendedLink *)btlGetTaskArguments(obj);
    link->parameter = arg;
    link->owner = owner;
    link->state = 0;
    return obj;
}

s32 btlPollActorOrEntryLabelTask(BtlEffLink *link) {
    s32 work = func_001AA6F8();
    s32 actor = link->actor;

    if (link->unk08 == 0) {
        s32 arg = link->arg;

        if (arg == 0) {
            if ((*(u64 *)(actor + 0x110) & 0x1400) == 0 && (((BtlJyokyoOwner *)actor)->flags & 0x10) == 0) {
                func_001B8580((s32)btlGetIndexedUiResource(actor));
            } else if (((BtlEffectSlots *)work)->flags & 0x400) {
                if (((BtlEffActor *)actor)->flags & 0x400) {
                    func_001B8580(D_003BEB60[0]);
                } else {
                    func_001B8580(D_003BEB58[0]);
                }
            } else {
                func_001B8580(D_003BEB58[0]);
            }
        } else {
            func_001B8580(D_00435E64 + arg * 17);
        }
    }
    if (func_001B8538() == 0 || link->unk08 >= 0x1E) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateEffObjD(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x43;
    obj->ownerData = owner->ownerData;
    obj->callback = btlPollActorOrEntryLabelTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

s32 btlPollTimedTaskLink(BtlEffLink *link) {
    func_001AA6F8();
    if (link->unk08 == 0) {
        func_001B8580(D_00435E5C + link->arg * 25);
    }
    if (func_001B8538() == 0 || link->unk08 >= 0x1E) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateOwnerLinkedTimedTask(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x44;
    obj->ownerData = owner->ownerData;
    obj->callback = btlPollTimedTaskLink;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

s32 btlPollCategoryLabelTask(BtlEffLink *link) {
    s32 actor;

    func_001AA6F8();
    actor = link->actor;
    if (link->unk08 == 0) {
        switch (link->arg) {
        case 10:
            func_001B8580((s32)D_003BEB28);
            break;
        case 5: {
            u32 flags = ((BtlEffActor *)actor)->flags;
            if (flags & 0x200) {
                if (!(flags & 0x1000)) {
                    func_001B8580(D_003BEB38[0]);
                    break;
                }
            }
            func_001B8580(D_003BEB48[0]);
            break;
        }
        case 6:
            func_001B8580((s32)D_003BEB30);
            break;
        case 9:
            func_001B8580(D_003BEB4C[0]);
            break;
        case 11:
            func_001B8580(D_003BEB50[0]);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            return 1;
        }
    }
    if (func_001B8538() == 0 || link->unk08 >= 0x1E) {
        return 1;
    }
    link->unk08++;
    return 0;
}

/* Preserve the owner's effect data in the new task. */
BtlEffTask *btlCreateEffObjA(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->id = 0x45;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->callback = btlPollCategoryLabelTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

s32 btlPollActorDialogTask(BtlEffLink *link) {
    s32 work = func_001AA6F8();
    s32 actor = link->actor;

    if (link->unk08 == 0) {
        if (actor != 0) {
            func_001A45C0(((BtlEffectSlots *)work)->id, 0, ((BtlJyokyoOwner *)actor)->unk124, (((BtlJyokyoOwner *)actor)->flags & 0x20) ? 0xE : 0xF);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlEffectSlots *)work)->id, link->arg);
    }
    if (func_001B8740() == 0 || link->unk08 >= 0x2D) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateEffObjB(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->id = 0x3F;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->callback = btlPollActorDialogTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

s32 func_00210530(BtlEffLink *link) {
    s32 work = func_001AA6F8();
    s32 actor = link->actor;

    if (actor != 0 && !(((BtlJyokyoOwner *)actor)->statusFlags & 1)) {
        return 1;
    }
    if (link->unk08 == 0) {
        if (actor != 0) {
            func_001A45C0(((BtlEffectSlots *)work)->alternateId, 0, ((BtlJyokyoOwner *)actor)->unk124, (((BtlJyokyoOwner *)actor)->flags & 0x20) ? 0xE : 0xF);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlEffectSlots *)work)->alternateId, link->arg);
    }
    if (func_001B8740() == 0 || link->unk08 >= 0x2D) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateEffObjC(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->id = 0x40;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->callback = func_00210530;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

u32 func_00210688(void) {
    return 1;
}

BtlEffTask *btlCreateEffectTask3E(BtlEffActor *owner, u16 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x41;
    obj->ownerData = owner->ownerData;
    obj->callback = func_00210688;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    *(u16 *)&link->arg = arg;
    link->unk08 = 0;
    return obj;
}

s32 btlPollTimedPresentationTask(BtlEffLink *link) {
    s32 work = func_001AA6F8();
    s32 actor = link->actor;

    if (link->unk08 == 0) {
        if (actor != 0) {
            func_001A45C0(((BtlEffectSlots *)work)->thirdId, 0, ((BtlJyokyoOwner *)actor)->unk124, (((BtlJyokyoOwner *)actor)->flags & 0x20) ? 1 : 2);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlEffectSlots *)work)->thirdId, link->arg);
    }
    if (func_001B8740() == 0 || link->unk08 >= 0x2D) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateOwnerLinkedTimedPresentation(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x42;
    obj->ownerData = owner->ownerData;
    obj->callback = btlPollTimedPresentationTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

s32 btlPollEffectWaitTask(BtlEffLink *link) {
    s32 work = func_001AA6F8();

    if (link->unk08 == 0) {
        if (link->actor != 0) {
            func_001A45C0(((BtlEffectSlots *)work)->id, 0, *(u16 *)&link->arg, 0xD);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlEffectSlots *)work)->id, 0x75);
    }
    if (func_001B8740() == 0 || link->unk08 >= 0x2D) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateEffectWaitTask(BtlEffActor *owner, u16 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->id = 0x46;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->callback = btlPollEffectWaitTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    *(u16 *)&link->arg = arg;
    link->unk08 = 0;
    return obj;
}

/* Waits for the task startup delay, then finishes when its two actor slots are clear. */
s32 btlWaitEffectTask(BtlWaitTask *task) {
    if (task->ticks == 0) {
        func_001B7E40(task->value, 0);
        func_001C7DB8(0, 8);
    }
    if (task->ticks >= 0x11) {
        if (btlGetRegisteredTaskValueOrDefault() == 1) {
            if (D_0037F510[0x21] < 0 || D_0037F510[0x23] < 0) {
                func_001C7DB8(1, 8);
                func_001B8038();
                return 1;
            }
        }
    } else {
        task->ticks += 1;
    }
    return 0;
}

BtlEffTask *btlCreateEffectTask44(BtlEffActor *owner) {
    BtlEffTask *obj = btlAllocTask(8);
    BtlEffLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x47;
    obj->ownerData = owner->ownerData;
    obj->callback = btlWaitEffectTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = 0;
    return obj;
}

s32 btlAdvanceActorEffectLabelTask(BtlEffLink *link) {
    s32 work = func_001AA6F8();
    s32 actor = link->actor;
    char text[0x100];

    if (link->unk08 == 0) {
        if (actor != 0) {
            func_001A45C0(((BtlEffectSlots *)work)->id, 0, ((BtlJyokyoOwner *)actor)->unk124, (((BtlJyokyoOwner *)actor)->flags & 0x20) ? 0xE : 0xF);
            func_0035C860(text, D_00436CA8, link->arg < 0 ? -link->arg : link->arg);
            func_001A4858(((BtlEffectSlots *)work)->id, 1, text);
        }
        btlReplaceDialogTasksAndQueueMessage(((BtlEffectSlots *)work)->id, 0xD5);
    }
    if (func_001B8740() == 0 || link->unk08 >= 0x2D) {
        return 1;
    }
    link->unk08++;
    return 0;
}

BtlEffTask *btlCreateTimedActorEffectLinkTask(BtlEffActor *owner, s32 arg) {
    BtlEffTask *obj = btlAllocTask(0xC);
    BtlEffLink *link;

    obj->kind = 1;
    obj->id = 0x48;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->callback = btlAdvanceActorEffectLabelTask;
    link = btlGetTaskArguments(obj);
    link->actor = (s32)owner;
    link->arg = arg;
    link->unk08 = 0;
    return obj;
}

extern s32 scrReadIntParameter(s32 index);
extern BtlEffActor *btlFindUnitByModeClear(s32 id);
extern BtlEffActor *btlFindUnitByModeFlagged(s32 id);
extern BtlEffTask *btlCreateEffObjB(BtlEffActor *actor, s32 arg);
extern void btlStartTask(BtlEffTask *task);
extern s64 btlAdvanceRuntimeSequenceCounter();

s32 btlCmdSpawnEffectTaskForSelectedUnit(void) {
    s32 mode;
    s32 unitId;
    s32 arg;
    BtlEffActor *unit;
    BtlEffTask *task;

    mode = scrReadIntParameter(0);
    unitId = scrReadIntParameter(1);
    arg = scrReadIntParameter(2);
    if (mode == 0) {
        unit = btlFindUnitByModeClear(unitId);
    } else {
        unit = btlFindUnitByModeFlagged(unitId);
    }
    if (unit == NULL) {
        return 1;
    }
    task = btlCreateEffObjB(unit, arg);
    task->ownerData = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(task);
    return 1;
}

u32 btlNextScaledRandom(u32 limit) {
    D_00436CB0 = D_00436CB0 * 0x41c64e6d + 0x3039;
    return (D_00436CB0 >> 0x10) * (limit & 0xffff) >> 0x10;
}

/* Inclusive random selection between either ordering of the endpoints. */
s32 btlRandomInclusiveRange(s32 lower, s32 upper) {
    s32 swap;

    if (upper < lower) {
        swap = upper;
        upper = lower;
        lower = swap;
    }
    upper = upper - lower + 1;
    return (s32)btlNextScaledRandom(0xFFFF) % upper + lower;
}

s32 btlAllocAndCheck(s32 object) {
    s32 allocation = sdfAllocAndClearQuadwords(0x10);
    s32 actor = (s32)((BtlTask *)object)->unit;

    btlActionScratchWork = allocation;
    *(s32 *)allocation = object;
    if (btlIsLowHpActionReady(actor, 0) != 0) {
        sdfReleaseChipBlock(btlActionScratchWork);
        return 1;
    }
    sdfReleaseChipBlock(btlActionScratchWork);
    return 0;
}

u32 btlAssignTaskResultAndArgument(s32 task) {
    ((BtlTask *)task)->result = 0xb;
    ((BtlTask *)task)->arg = 0xc2;
    return 1;
}

void btlAdvanceHistoryCounter(BtlHistObj *obj) {
    obj->counter++;
    obj->counter = obj->counter <= 0 ? 0 : obj->counter >= 0x21 ? 0x20 : obj->counter;
    btlShiftActorStateHistory(obj, 0);
    btlHistoryCounter++;
    btlHistoryCounter = btlHistoryCounter <= 0 ? 0 : btlHistoryCounter >= 0x21 ? 0x20 : btlHistoryCounter;
}


void btlResetBattleHistoryCounters(void) {
    BtlHistObj *node = *(BtlHistObj **)(func_001AA6F8() + 0x248);
    if (node != 0) {
        do {
            if (node->actor != NULL) {
                node->counter = 0;
                node->status170 = 0;
            }
            node = node->next;
        } while (node != 0);
    }
    btlHistoryCounter = 0;
}

s32 btlDispatchPackedActionWithScratch(s32 context, BtlJyokyoOwner *owner, s32 mask) {
    s32 *work = (s32 *)sdfAllocAndClearQuadwords(0x10);
    s32 result;

    btlActionScratchWork = (s32)work;
    work[1] = owner->unk124;
    work[0] = context;
    result = btlDispatchPackedEffectAction((s32)owner, mask);
    sdfReleaseChipBlock(btlActionScratchWork);
    return result;
}

/* Top ten bits select the callback; the lower 22 bits are its argument. */
s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction) {
    u32 type = packedAction >> 22;
    s32 result = 0;

    packedAction &= 0x3FFFFF;
    if (type != 0) {
        result = D_003BF4A0[type](context, packedAction) != 0;
    }
    return result;
}

typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSpecies {
    u8 pad00[0x40];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *D_00435DF4;
extern void func_00211658(BtlJyokyoOwner *unit, u16 species, s32 *row, s32 arg);
extern u32 btlPickWeightedAiSlot();
extern s32 func_00211EA8();

/* Choose a row and a weighted slot of the unit's species AI table and run that action. */
s32 func_00210F58(BtlTask *task) {
    s32 *work = (s32 *)sdfAllocAndClearQuadwords(0x10);
    BtlJyokyoOwner *unit;
    u16 species;
    s32 row;
    s32 index;

    unit = (BtlJyokyoOwner *)task->unit;
    btlActionScratchWork = (s32)work;
    species = unit->unk124;
    work[0] = (s32)task;
    work[1] = species;
    func_00211658(unit, species, &row, 0);
    index = btlPickWeightedAiSlot(unit, species, row);
    func_00211EA8(task, D_00435DF4[species].slot[row * 5 + index].actionId,
                  D_00435DF4[species].slot[row * 5 + index].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
    return 1;
}

void btlShiftActorStateHistory(BtlHistObj *obj, s8 flag) {
    s32 i;

    for (i = 6; i >= 0; i--) {
        obj->hist[i + 1] = obj->hist[i];
    }
    if ((u32)(obj->mode - 1) < 4) {
        if (flag == 0) {
            obj->hist[0] = obj->unk24;
        } else {
            obj->hist[0] = obj->actor->unk2D0;
        }
    } else {
        obj->hist[0] = 0;
    }
}

void btlCmdWithArgA(s32 context) {
    func_002152D8(context, 0);
}

void btlCmdWithArgB(s32 context) {
    func_00215C70(context, 0);
}

void btlCmdWithArgC(s32 context) {
    btlSelectLowestHealthRateTarget(context, 0);
}

void func_002110E8(void) {
    btlSelectTargetsByMode();
}

void func_00211108(s32 context) {
    btlAppendCurrentUnitIdToCommandIndices(context, 0);
}

extern void func_00215D78(s32, s32);

void btlCmdSimpleA(s32 context, s32 value) {
    func_00215D78(context, value);
}

void btlCmdWithArgD(s32 context) {
    btlSelectLowestRankTarget(context, 0);
}

void btlCmdWithArgE(s32 context) {
    func_00216888(context, 0);
}

extern void btlSelectTargetsPassingCheck(s32, s32);

void btlCmdSimpleB(s32 context, s32 value) {
    btlSelectTargetsPassingCheck(context, value);
}

extern void btlSelectTargetsByActionMask(s32, s32);

void btlCmdSimpleC(s32 context, s32 value) {
    btlSelectTargetsByActionMask(context, value);
}

extern void btlSelectTargetsWithoutActionMask(s32, s32);

void btlCmdSimpleD(s32 context, s32 value) {
    btlSelectTargetsWithoutActionMask(context, value);
}

extern void func_002162C0(s32, s32);

void btlCmdSimpleE(s32 context, s32 value) {
    func_002162C0(context, value);
}

void btlCmdSimpleG(void) {
    func_00216D10();
}

void func_00211228(void) {
    func_00216D30();
}

void btlCmdSimpleH(void) {
    btlAppendSelfAfterTargetScan();
}

void btlCmdSimpleI(void) {
    func_00216988();
}

void btlCmdWithArgF(s32 context) {
    btlAppendEffectActorToCommandIndices(context, 0);
}

extern void btlSelectTargetsBlockingElement(s32, s32);

void btlCmdSimpleJ(s32 context, s32 value) {
    btlSelectTargetsBlockingElement(context, value);
}

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C78);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C80);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C88);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C90);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436C98);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CA0);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CA8);

INCLUDE_SDATA(const s32, "game/code_0020E850", btlHistoryCounter);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CB0);

INCLUDE_SDATA(const s32, "game/code_0020E850", D_00436CB4);

INCLUDE_SDATA(const s32, "game/code_0020E850", btlActionScratchWork);

