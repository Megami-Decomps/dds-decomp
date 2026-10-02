#include "common.h"

typedef struct MenuActionOwner MenuActionOwner;

extern u8 brsUiUpdateAllowed;

extern s32 datGameState;

extern s32 kwlnFadeIsActive(void);

extern s32 func_002C6CE8(void);

extern void mnuSetCommandPhase(MenuActionOwner *, u32);

extern void func_00297200(MenuActionOwner *, u32);

extern void mnuStorePendingMenuCommandValue(MenuActionOwner *, u32);

extern s8 brsUpdateBlocked;

extern s8 brsPendingRowsLatched;

extern s8 brsTaskState;

extern void sndEnsureMidiBankResident(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void mnuAppendCampSpriteRequests(s32, s32);

extern void effRequestResourceByMode(char *, char *, s32, s32);

extern void mnuRequestEffectResources(s32);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern char D_00428358[];

extern char D_00428368[];

typedef struct MenuAction {
    u32 value;
    u32 mode;
} MenuAction;

struct MenuActionOwner {
    u8 pad00[0x30];
    MenuAction *action;
    u8 pad34[0x88];
    s32 frames;           /* 0xBC */
    s32 mode;             /* 0xC0: command phase */
};

typedef struct MenuIconRef {
    u16 id;
    u8 param;
    u8 pad3;
} MenuIconRef;

/* Three reward references followed by the bundle's macca value. */
typedef struct MenuIconBatch {
    MenuIconRef icons[3];
    u32 resource;
} MenuIconBatch;

extern void func_0011A118(s32, s32);

extern char brsStaffInputTaskName[];

extern char mnuStaffPrimaryPanelTaskName[];

extern char mnuStaffSecondaryPanelTaskName[];

extern char brsStaffInputTaskName[];

extern char mnuStaffPrimaryPanelTaskName[];

extern char mnuStaffSecondaryPanelTaskName[];

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

typedef struct MenuPanelBlock {
    s32 data[0x71];
} MenuPanelBlock;

typedef struct BrsPartyUnit {
    u16 flags;              /* 0x00: bit 0 indicates an occupied party slot */
    u8 pad02[0xC];
    u16 statusFlags;        /* 0x0E: bit 0x4000 enables battle rewards */
    u32 totalExp;           /* 0x10 */
    u16 level;              /* 0x14 */
    u8 pad16[0x3F];
    u8 currentProfileId;    /* 0x55 */
    u8 pad56[0x16E];
} BrsPartyUnit;

typedef struct BrsRewardValues {
    s32 amount;              /* 0x04 */
    s32 experienceGain;      /* 0x08 */
    u8 pad0C[8];
    s32 partySlot;           /* 0x14 */
} BrsRewardValues;

typedef struct BrsRewardRow {
    BrsPartyUnit *unit;     /* 0x00 */
    BrsRewardValues values; /* 0x04 */
} BrsRewardRow;

typedef struct BrsRewardBatch {
    BrsRewardRow rows[5];
    s32 count;              /* 0x78 */
} BrsRewardBatch;

extern u32 *ptyGetCurrentProfileRecord(s32 unit);
extern u32 ptyAddProfileRecordValueClamped(u8 *unit, u32 amount);
extern void func_00299988(u32, s32, u16, u32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00296E98);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297000);

void mnuStorePendingMenuCommandValue(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 1;
    }
}

void func_002971E0(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 2;
    }
}

void func_00297200(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 1;
    }
}

void func_00297220(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 2;
    }
}

void mnuSetCommandPhase(MenuActionOwner *owner, u32 value) {
    owner->mode = value;
    owner->frames = 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297250);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297320);

/* Phase machine of the menu command: phases 4, 5 and 8 wait for the frame
 * counter to pass 10.0f before reporting themselves, 6 and 7 report at once.
 * The counter is compared as a float because retail loads 10.0f into $f1 and
 * converts the counter with cvt.s.w (lui at,0x4120 / mtc1 / cvt.s.w / c.lt.s).
 * Phase 8 uses c.le.s, so it fires one frame earlier than 4 and 5. */
s32 mnuTickExtendedCommandPhase(MenuActionOwner *work) {
    switch (work->mode) {
    case 4:
        work->frames = work->frames + 1;
        if ((f32)work->frames > 10.0f) {
            return 4;
        }
        break;
    case 5:
        work->frames = work->frames + 1;
        if ((f32)work->frames > 10.0f) {
            return 5;
        }
        break;
    case 7:
        return 7;
    case 8:
        work->frames = work->frames + 1;
        if ((f32)work->frames >= 10.0f) {
            return 8;
        }
        break;
    case 6:
        return 6;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297970);

/* Phase machine for phases 9-12: 9 waits for the frame counter to pass 10.0f, 10 and 12 for it to reach 10.0f, 11 reports at once. */
s32 mnuTickCommandWaitPhase(MenuActionOwner *work) {
    switch (work->mode) {
    case 9:
        work->frames = work->frames + 1;
        if ((f32)work->frames > 10.0f) {
            return 9;
        }
        break;
    case 11:
        return 11;
    case 10:
        work->frames = work->frames + 1;
        if ((f32)work->frames >= 10.0f) {
            return 10;
        }
        break;
    case 12:
        work->frames = work->frames + 1;
        if ((f32)work->frames >= 10.0f) {
            return 12;
        }
        break;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298648);

void brsTaskStart(void) {
    mnuStaffCreateTasks();
    brsUiUpdateAllowed = 0;
    brsUpdateBlocked = 1;
}

s8 brsTaskIsUpdateBlocked(void) {
    return brsUpdateBlocked;
}

u32 brsTaskAllowUpdate(void) {
    brsUiUpdateAllowed = 1;
    return 1;
}

void brsTaskPollDone(void) {
    brsTaskConsumeDone();
}

s8 brsTaskHasPendingRows(void) {
    return brsPendingRowsLatched;
}

s8 brsTaskIsUiUpdateAllowed(s32 context) {
    if (*(s32 *)(context + 0xAEB0) != 0) {
        brsUpdateBlocked = 0;
    }
    return brsUpdateBlocked ? 0 : brsUiUpdateAllowed;
}

void gstApplyCounterDeltaTable(MenuIconRef *refs) {
    u32 i;

    for (i = 0; i < 3; i++) {
        u16 id = refs->id;
        u8 param = refs->param;

        refs++;
        if (id != 0) {
            func_0011A118(id, param);
        }
    }
}

void gstApplyBundleMacca(MenuIconBatch *batch) {
    datAddCurrencyClamped(batch->resource);
}

extern s32 ptyComputeTotalExp(u8 *, s32);

typedef struct BrsUnitExperience {
    u8 pad00[0x10];
    u32 totalExp;           /* 0x10 */
    u16 level;              /* 0x14 */
} BrsUnitExperience;

/* Cap stored EXP at the EXP required for level 99. */
void ptyClampExp(u32 *unit) {
    u8 buf[0x1C4];
    u32 exp;

    memcpy(buf, unit, 0x1C4);
    ((BrsUnitExperience *)buf)->level = 0x63;
    exp = ptyComputeTotalExp(buf, 0);
    if (exp < ((BrsUnitExperience *)unit)->totalExp) {
        ((BrsUnitExperience *)unit)->totalExp = exp;
    }
}

/* Snapshot eligible party members, then apply the queued EXP and profile
 * rewards to each referenced unit. */
void func_00299018(u32 partyWork, BrsRewardBatch *batch) {
    s32 partyIndex;
    s32 partyOffset;
    s32 rewardOffset;
    s32 rewardIndex;
    BrsPartyUnit *currentPartyUnit;
    BrsPartyUnit *unit;
    s32 profilePointGain;
    s32 experienceGain;
    BrsRewardValues *values;

    for (partyOffset = 0, partyIndex = 0; partyIndex < 5;
         partyIndex++, partyOffset += sizeof(BrsPartyUnit)) {
        currentPartyUnit = (BrsPartyUnit *)(datGameState + 0xA60 + partyOffset);

        if ((currentPartyUnit->flags & 1) != 0 &&
            (currentPartyUnit->statusFlags & 0x4000) != 0) {
            unit = currentPartyUnit;
            func_00299988(partyWork, partyIndex, unit->level,
                          unit->totalExp,
                          *ptyGetCurrentProfileRecord((s32)currentPartyUnit),
                          0, 0);
        }
    }

    rewardIndex = 0;
    if (batch->count > 0) {
        values = &batch->rows[0].values;
        rewardOffset = 0;
        do {
            unit = *(BrsPartyUnit **)((u8 *)batch + rewardOffset);
            profilePointGain = values->amount;
            experienceGain = values->experienceGain;

            func_00299988(partyWork, values->partySlot, unit->level,
                          unit->totalExp,
                          *ptyGetCurrentProfileRecord((s32)unit),
                          experienceGain, profilePointGain);
            unit->totalExp += experienceGain;
            ptyClampExp((u32 *)unit);
            if (unit->currentProfileId != 0) {
                ptyAddProfileRecordValueClamped((u8 *)unit,
                                                profilePointGain);
            }
            rewardIndex++;
            values = (BrsRewardValues *)((u8 *)values + sizeof(BrsRewardRow));
            rewardOffset += sizeof(BrsRewardRow);
        } while (rewardIndex < batch->count);
    }
}

void brsApplyRewardBundle(u32 partyWork, MenuIconBatch *batch,
                          BrsRewardBatch *rewardState) {
    gstApplyCounterDeltaTable(batch->icons);
    gstApplyBundleMacca(batch);
    func_00299018(partyWork, rewardState);
}

extern void mnuReleaseStaffMenuResources(s32);
extern void mnuInitializeCampPanelResources(s32, s32, s32, s32);
extern s32 mnuCreatePanelGroup(s32, s32, s32);
extern s32 mnuCreateSpriteState(s32, s32, s32);
extern void evtStageTestInit(s32);

typedef struct BrsPartyRow {
    s32 flags;
    s32 amount;
    u8 pad08[0x24];
} BrsPartyRow;
/* Battle-result panel fields used while opening its skill-package display. */
typedef struct BrsSkillPackageWork {
    s32 handle;
    u8 pad04[4];
    u8 transition[0x50];
    s32 fadeTarget;          /* 0x058 */
    MenuIconBatch rewards;
    u8 pad6C[0x28];
    s32 unitHandle;          /* 0x094 */
    u8 pad98[0x1CC];
    s32 selectedRow;         /* 0x264 */
    u8 pad268[0xC];
    BrsRewardBatch secondaryRewards;
    BrsRewardBatch primaryRewards;
    u8 rewardState[0x9C];
    BrsPartyRow partyRows[5];
    u8 pad4E4[0x38];
    s32 panelGroup;          /* 0x51C: start of the group passed to setup */
    s32 spriteArg0;          /* 0x520 */
    s32 spriteArg1;          /* 0x524 */
    u8 pad528[0x58];
    s32 setupState;          /* 0x580 */
    u8 pad584[0xA7B0];
    s32 panelHandle;         /* 0xAD34 */
    s32 spriteHandle;        /* 0xAD38 */
    u8 padAD3C[0x174];
    s32 teardownHandle;     /* 0xAEB0 */
    u8 padAEB4[0x82C];
    s32 unusedB6E0;
    u8 padB6E4[0x20];
} BrsSkillPackageWork;

typedef struct BrsSelectedRow {
    s32 unit;                /* 0x00 */
    u8 pad04[0x14];
} BrsSelectedRow;            /* 0x18 */

typedef struct BrsRowUnit {
    u8 pad00[4];
    u16 unitId;              /* 0x04 */
} BrsRowUnit;

/* Build the selected reward row's skill-package group and sprite, then
 * forward the selected unit ID to the menu. */
void brsOpenSkillPackagePanel(BrsSkillPackageWork *work) {
    s32 *group = &work->panelGroup;
    s32 panel;

    mnuReleaseStaffMenuResources((s32)group);
    mnuInitializeCampPanelResources((s32)work + 0x690, (s32)group, 0, (s32)work + 0x584);
    panel = mnuCreatePanelGroup(work->spriteArg0, work->spriteArg1, 0);
    work->panelHandle = panel;
    mnuUpdateFiveListEntries(panel, work->unitHandle);
    work->spriteHandle =
        mnuCreateSpriteState(work->spriteArg0, work->spriteArg1, work->panelGroup);
    evtStageTestInit(0);
    mnuForwardTableByte(((BrsRowUnit *)
        (((BrsSelectedRow *)((u8 *)work + 0x2F0))[work->selectedRow].unit))->unitId);
}

extern void effDestroyResourceSlotSet(s32);
extern void mnuClearEntries();
extern void mnuReleasePartyIconBundles();
extern void mnuShutdownContext();
extern void mnuDestroyPanelGroup(s32);
extern void mnuFreeSpriteStateWork(s32);
extern void mnuDestroyEffectResources(s32);
extern void mnuReleaseStaffMenuTextureHandles();
extern void mnuReleaseTitleEffectSprites();
extern void mnuResetWorkFloats(void);

void brsCloseSkillPackagePanel(s32 work) {
    BrsSkillPackageWork *ctx = (BrsSkillPackageWork *)work;
    s32 panelContext = work + 0x690;

    effDestroyResourceSlotSet(ctx->unitHandle);
    mnuClearEntries(panelContext);
    mnuReleasePartyIconBundles(panelContext);
    mnuShutdownContext(panelContext);
    mnuDestroyPanelGroup(ctx->panelHandle);
    mnuFreeSpriteStateWork(ctx->spriteHandle);
    mnuDestroyEffectResources(work + 0xAD40);
    mnuReleaseStaffMenuTextureHandles(work + 0x51C);
    mnuReleaseTitleEffectSprites(work + 0x51C);
    mnuResetWorkFloats();
}

s32 brsStartPartyPanelResourcesOnce(s32 work) {
    if (*(s32 *)(work + 0x580) != 0) {
        return 0;
    }
    sndEnsureMidiBankResident(0x50000);
    mnuInitPartyPanelSlots(work + 0x584);
    mnuAppendCampSpriteRequests(*(s32 *)(work + 0x58), work + 0x51C);
    effRequestResourceByMode(D_00428358, D_00428368, 0, work + 0x94);
    mnuRequestEffectResources(0xAD40 + work);
    *(s32 *)(work + 0x580) = 1;
    kwlnFadeInStart(0, 0, 0, 1);
    kwlnFadeInStart(0, 0, 0, 0);
    return 1;
}

extern s32 movAreTitleEffectsReady(s32, s32);
extern s32 mnuBindCampEffectWhenLoaded(s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);

s32 brsAdvanceSkillPackagePanel(s32 work) {
    BrsSkillPackageWork *ctx = (BrsSkillPackageWork *)work;

    if (ctx->setupState == 0) {
        return 1;
    }
    if (ctx->setupState == 2) {
        return 0;
    }
    if (movAreTitleEffectsReady(ctx->fadeTarget, work + 0x51C) == 0) {
        return 1;
    }
    if (func_002C6CE8() == 1) {
        return 1;
    }
    if (ctx->unitHandle == 0) {
        return 1;
    }
    if (mnuBindCampEffectWhenLoaded(work + 0xAD40) == 0) {
        return 1;
    }
    brsOpenSkillPackagePanel(ctx);
    ctx->setupState = 2;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 0;
}

/* Five party slots followed by the number of rewards in this batch. */

typedef struct BrsTaskState {
    u8 pad00[0x368];
    s32 pendingRows;
} BrsTaskState;

/* Tag each matching occupied party slot with its reward flags and amount. */
void brsMarkPartyRows(u32 dst, u32 state, s32 flags) {
    s32 i;

    for (i = 0; i < ((BrsRewardBatch *)state)->count; i++) {
        u8 *d = (u8 *)dst;
        u8 *unit = (u8 *)datGameState + 0xA60;
        s32 j;

        for (j = 4; j >= 0; j--) {
            if ((((BrsPartyUnit *)unit)->flags & 1) != 0) {
                BrsRewardRow *row = &((BrsRewardBatch *)state)->rows[i];
                if (row->unit == (BrsPartyUnit *)unit) {
                    ((BrsPartyRow *)d)->flags |= flags;
                    if (flags & 2) {
                        ((BrsPartyRow *)d)->amount = row->values.amount;
                    }
                }
            }
            d += 0x2C;
            unit += 0x1C4;
        }
    }
}

void brsMarkPartyRowsFromLists(u32 partyRows, u32 primaryRewards, u32 secondaryRewards) {
    brsMarkPartyRows(partyRows, primaryRewards, 2);
    brsMarkPartyRows(partyRows, secondaryRewards, 1);
}

/* Latch whether the result task still has pending reward rows. */
void brsTaskLatchPendingRows(s32 context) {
    if (((BrsTaskState *)context)->pendingRows == 0) {
        brsPendingRowsLatched = 0;
    } else {
        brsPendingRowsLatched = 1;
    }
}

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern s32 mnuAllocateValueRecord(s32);
extern char D_003D05C8[];

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428358);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428368);

void *brsCreateRewardTaskWork(void) {
    MenuIconBatch *rewards;
    s32 handle;
    BrsPartyRow *party;
    void *rewardState;
    BrsRewardBatch *primary;
    BrsRewardBatch *secondary;
    BrsSkillPackageWork *work;

    handle = sdfAllocGeneralBlock(sizeof(BrsSkillPackageWork));
    work = sdfResourceRetainAddress(handle);
    memset(work, 0, sizeof(BrsSkillPackageWork));
    work->handle = handle;
    mnuClearPanelTransitionState(work->transition);
    work->fadeTarget = mnuAllocateValueRecord(1);
    evtCreateMessageWindowIfMissing(D_003D05C8);
    func_0026C580(200);
    rewards = &work->rewards;
    func_001AA400(rewards);
    party = work->partyRows;
    rewardState = work->rewardState;
    func_0029D008(rewardState, rewards);
    brsApplyRewardBundle((u32)work, rewards, (BrsRewardBatch *)rewardState);
    primary = &work->primaryRewards;
    func_0029D2D8(primary);
    secondary = &work->secondaryRewards;
    func_0029D3D8(secondary);
    func_0029DA98(party);
    brsMarkPartyRowsFromLists((u32)party, (u32)primary, (u32)secondary);
    work->unusedB6E0 = 0x100;
    brsTaskLatchPendingRows((s32)work);
    work->teardownHandle = 0;
    effRequestResourceByMode(D_00428358, "easy_r01.spr", 0,
                            (s32)&work->teardownHandle);
    return work;
}

extern s32 kwlnTaskGetUserValue();
extern void effDestroyResourceSlotSet(s32);
extern void mnuDrainPanelTransitions(s32, s32);
extern s32 brsAdvanceSkillPackagePanel(s32);
extern void brsCloseSkillPackagePanel(s32);
extern void func_00303D58(s32);
extern void dspCloseChannel(void);
extern void sdfReleaseResourceAllocation(s32);

/* Release the panel and task resources, then mark the result task finished. */
void brsStaffTaskDestroy(s32 taskArg) {
    s32 context = kwlnTaskGetUserValue();

    if (((BrsSkillPackageWork *)context)->teardownHandle != 0) {
        effDestroyResourceSlotSet(((BrsSkillPackageWork *)context)->teardownHandle);
    }
    mnuDrainPanelTransitions(context + 8, taskArg);
    if (brsAdvanceSkillPackagePanel(context) == 0) {
        brsCloseSkillPackagePanel(context);
    }
    func_00303D58(((BrsSkillPackageWork *)context)->fadeTarget);
    dspCloseChannel();
    sdfReleaseResourceAllocation(*(s32 *)context);
    brsTaskState = 2;
}

extern s32 kwlnTaskCreate(void *name, s32 flags, s32 prio, s32 stacked, void *update, void *destroy, void *data);
extern void *brsCreateRewardTaskWork(void);
extern void brsMessageInputStep(void);
extern void mnuStaffRunPanel1(void);
extern void mnuStaffRunPanel2(void);
extern void brsStaffTaskDestroy(s32);

s32 mnuStaffCreateTasks(void) {
    s32 result;
    void *work = brsCreateRewardTaskWork();

    kwlnTaskCreate(brsStaffInputTaskName, 0x405, 1, 0, brsMessageInputStep, 0, work);
    kwlnTaskCreate(mnuStaffPrimaryPanelTaskName, 0x2B15, 1, 0, mnuStaffRunPanel1, 0, work);
    result = kwlnTaskCreate(mnuStaffSecondaryPanelTaskName, 0x5211, 1, 0, mnuStaffRunPanel2, brsStaffTaskDestroy, work);
    brsTaskState = 1;
    return result;
}

u32 mnuStaffDestroyTasks(void) {
    s8 state = brsTaskState;

    if (state == 1) {
        kwlnTaskDestroyWithHierarchyByName(brsStaffInputTaskName, 0);
        kwlnTaskDestroyWithHierarchyByName(mnuStaffPrimaryPanelTaskName, 0);
        kwlnTaskDestroyWithHierarchyByName(mnuStaffSecondaryPanelTaskName, 0);
        brsUpdateBlocked = state;
        return 1;
    }
    return 0;
}

s32 brsTaskConsumeDone(void) {
    s32 state = brsTaskState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        brsTaskState = 0;
    }
    return 0;
}

u32 brsTaskTryDestroy(void) {
    if (brsTaskState == 1) {
        mnuStaffDestroyTasks();
        return 1;
    }
    return 0;
}

s32 func_002998D8(void) {
    extern s32 func_00101740(const char *);
    s32 task;
    u8 *work;

    task = func_00101740(mnuStaffPrimaryPanelTaskName);
    if (task == 0) {
        return task;
    }
    work = (u8 *)kwlnTaskGetUserValue(task);
    if (256 - *(s32 *)(work + 0xB6E0) <= 0 && brsTaskIsUiUpdateAllowed((s32)work) != 0) {
        if (*(s8 *)(work + 0xAEA8) == 1) {
            return 1;
        }
    }
    return *(s8 *)(work + 0xAEA8);
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299988);

s32 brsTaskIsFadeIdle(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002C6CE8() != 1;
}

void mnuRefreshSelectedUnitPanels(u32 unused, s32 menu) {
    mnuInitPartyPanelSlots(menu + 0x584);
    func_002BCA98(menu + 0x690);
    func_002BCAB0(menu + 0x690);
}

void mnuStaffCopyPanelBlock(MenuPanelBlock *src, u8 *base) {
    *(MenuPanelBlock *)(base + 0xA0) = *src;
}

extern s32 effMiscRand(s32);
extern s32 D_003D6308[];

/* Choose a reward bucket using this mode's cumulative thresholds. */
s32 mnuStaffPickRollByMode(u32 mode) {
    u32 roll = effMiscRand(0) & 0xFF;
    u32 i;

    for (i = 0; i < 5; i++) {
        if ((s32)roll < D_003D6308[mode * 5 + i]) {
            return i + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299B98);

INCLUDE_RODATA(const s32, "game/code_00296E98", mnuStaffPrimaryPanelTaskName);

INCLUDE_RODATA(const s32, "game/code_00296E98", mnuStaffSecondaryPanelTaskName);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_004283B0);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_004283C0);

INCLUDE_SDATA(const s32, "game/code_00296E98", brsTaskState);

INCLUDE_SDATA(const s32, "game/code_00296E98", brsPendingRowsLatched);

INCLUDE_SDATA(const s32, "game/code_00296E98", brsUiUpdateAllowed);

INCLUDE_SDATA(const s32, "game/code_00296E98", brsUpdateBlocked);

INCLUDE_SDATA(const s32, "game/code_00296E98", brsStaffInputTaskName);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437998);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_004379A0);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_004379A8);
