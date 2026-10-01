#include "common.h"

extern s32 func_002877A8(void);

extern s32 kwlnFadeIsActive(void);

extern s8 D_003BC529;

extern s8 D_003BC52A;

extern s8 D_003BC52B;

extern s8 D_003BC528;

INCLUDE_ASM(const s32, "game/code_00260208", func_00260208);

INCLUDE_ASM(const s32, "game/code_00260208", func_00260370);

typedef struct {
    u32 value;
    u32 mode;
} MenuCommand;

typedef struct {
    u8 pad00[0x30];
    MenuCommand *command; /* 0x30 */
    u8 pad34[0x74];
    s32 frames;           /* 0xA8 */
    s32 mode;             /* 0xAC: command phase, 0–3 */
} MenuCommandWork;

void mnuStorePendingMenuCommandValue(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 1;
    }
}

void func_00260550(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 2;
    }
}

void func_00260570(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 1;
    }
}

void func_00260590(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 2;
    }
}

void mnuSetCommandPhase(MenuCommandWork *work, u32 mode) {
    work->mode = mode;
    work->frames = 0;
}

s32 mnuStaffTickState(MenuCommandWork *work) {
    s32 count;

    switch (work->mode) {
    case 0:
        count = work->frames + 1;
        work->frames = count;
        if ((f32)count > 10.0f) {
            return 0;
        }
        return -1;
    case 1:
        return 1;
    case 2:
        count = work->frames + 1;
        work->frames = count;
        if ((f32)count > 10.0f) {
            return 2;
        }
        return -1;
    case 3:
        return 3;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00260670);

/* Phase machine of the menu command: phases 4, 5 and 8 wait ten frames before reporting themselves, 6 and 7 report at once. */
s32 func_002609D8(MenuCommandWork *work) {
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

INCLUDE_ASM(const s32, "game/code_00260208", func_00260AB0);

INCLUDE_ASM(const s32, "game/code_00260208", func_00261688);

INCLUDE_ASM(const s32, "game/code_00260208", func_00261760);

void brsTaskStart(void) {
    mnuStaffCreateTasks();
    D_003BC52A = 0;
    D_003BC52B = 1;
}

s8 brsTaskIsUpdateBlocked(void) {
    return D_003BC52B;
}

u32 brsTaskAllowUpdate(void) {
    D_003BC52A = 1;
    return 1;
}

void brsTaskPollDone(void) {
    brsTaskConsumeDone();
}

s8 brsTaskHasPendingRows(void) {
    return D_003BC529;
}

s8 brsTaskIsUiUpdateAllowed(s32 context) {
    if (*(s32 *)(context + 0xd44) != 0) {
        D_003BC52B = 0;
    }
    return D_003BC52B ? 0 : D_003BC52A;
}

typedef struct MenuIconRef {
    u16 id;
    u8 param;
    u8 pad3;
} MenuIconRef;

/* Reward bundle: three icon/counter refs followed by the macca amount. */
typedef struct MenuIconBatch {
    MenuIconRef refs[3]; /* 0x00 */
    u32 resource;        /* 0x0C */
} MenuIconBatch;

extern void func_00119900(s32, s32);

void gstApplyCounterDeltaTable(MenuIconRef *refs) {
    u32 i;

    for (i = 0; i < 3; i++) {
        u16 id = refs->id;
        u8 param = refs->param;

        refs++;
        if (id != 0) {
            func_00119900(id, param);
        }
    }
}

void gstApplyBundleMacca(MenuIconBatch *batch) {
    datAddCurrencyClamped(batch->resource);
}

extern s32 ptyComputeTotalExp(u8 *, s32);

typedef struct BrsUnitExperience {
    u8 pad00[0x10];
    u32 totalExp;       /* 0x10 */
    u16 level;          /* 0x14 */
} BrsUnitExperience;

/* Cap stored EXP at the EXP required for level 99. */
void ptyClampExp(u32 *unit) {
    u8 buf[0x1A4];
    s32 exp;

    memcpy(buf, unit, 0x1A4);
    ((BrsUnitExperience *)buf)->level = 0x63;
    exp = ptyComputeTotalExp(buf, 0);
    if (exp < ((BrsUnitExperience *)unit)->totalExp) {
        ((BrsUnitExperience *)unit)->totalExp = exp;
    }
}

INCLUDE_ASM(const s32, "game/code_00260208", brsApplyPartyRewards);

/* Apply item/icon rewards before awarding the party's accumulated gains. */
void brsApplyRewardBundle(u32 partyWork, MenuIconBatch *batch, u32 rewardState) {
    gstApplyCounterDeltaTable(batch->refs);
    gstApplyBundleMacca(batch);
    brsApplyPartyRewards(partyWork, rewardState);
}

extern void mnuReleaseStaffMenuResources(s32 *);
extern void mnuInitializeStaffPageWindows(s32, s32 *, s32, s32);
extern s32 mnuCreatePanelGroup(s32);
extern void mnuUpdateFiveListEntries(s32, s32);
extern s32 mnuCreateSpriteState(s32, s32, s32);
extern void evtStageTestInit(s32);
extern void mnuForwardTableByte(s32);

/* Fields of the battle-result panel needed while opening its skill package. */
typedef struct BrsSkillPackageWork {
    u8 pad00[0x58];
    s32 fadeTarget;          /* 0x058 */
    u8 pad5C[0x34];
    s32 unitHandle;          /* 0x090 */
    u8 pad94[0x1AC];
    s32 selectedRow;         /* 0x240 */
    u8 pad244[0x2B4];
    s32 group[2];            /* 0x4F8 */
    s32 spriteArg0;          /* 0x500 */
    u8 pad504[8];
    s32 spriteArg1;          /* 0x50C */
    u8 pad510[4];
    s32 panelGroup;          /* 0x514 */
    u8 pad518[0x58];
    s32 setupState;          /* 0x570 */
    u8 pad574[0x79C];
    s32 panelHandle;         /* 0xD10 */
    s32 spriteHandle;        /* 0xD14 */
    u32 assets;              /* 0xD1C */
} BrsSkillPackageWork;

typedef struct BrsSelectedRow {
    s32 unit;                 /* 0x00: pointer to a party unit */
    u8 pad04[0x14];
} BrsSelectedRow;            /* 0x18 */

typedef struct BrsRowUnit {
    u8 pad00[4];
    u16 unitId;              /* 0x04 */
} BrsRowUnit;

/* Create the group and sprite backing the skill-package panel for the
 * selected reward row, then forward its unit's ID to the menu. */
void brsOpenSkillPackagePanel(BrsSkillPackageWork *work) {
    s32 *group = work->group;
    s32 panel;

    mnuReleaseStaffMenuResources(group);
    mnuInitializeStaffPageWindows((s32)work + 0x680, group, 0, (s32)work + 0x574);
    panel = mnuCreatePanelGroup(work->panelGroup);
    work->panelHandle = panel;
    mnuUpdateFiveListEntries(panel, work->unitHandle);
    work->spriteHandle =
        mnuCreateSpriteState(work->spriteArg1,
                             work->spriteArg0,
                             work->panelGroup);
    evtStageTestInit(0);
    mnuForwardTableByte(((BrsRowUnit *)
        (((BrsSelectedRow *)((u8 *)work + 0x2CC))[work->selectedRow].unit))->unitId);
}

void brsCloseSkillPackagePanel(s32 work) {
    BrsSkillPackageWork *ctx = (BrsSkillPackageWork *)work;
    s32 panelContext = work + 0x680;

    effDestroyResourceSlotSet(ctx->unitHandle);
    mnuClearEntries(panelContext);
    mnuReleasePartyIconBundles(panelContext);
    mnuShutdownContext(panelContext);
    mnuDestroyPanelGroup(ctx->panelHandle);
    func_002832F8(ctx->spriteHandle);
    mnuReleaseAssets(work + 0xD1C);
    mnuReleaseStaffMenuTextureHandles(work + 0x4F8);
    mnuReleaseStaffResourceGroups(work + 0x4F8);
    mnuResetWorkFloats();
}

extern char D_003AFA88[];
extern char D_003AFA98[];
extern void func_002E9340(s32);
extern void mnuInitPartyPanelSlots(s32);
extern void mnuAppendCampSpriteRequests(s32, s32);
extern void effRequestResourceByMode(char *, char *, s32, s32);
extern void mnuRequestBaseAssets(s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);

s32 mnuStaffInitPanel(s32 work) {
    if (*(s32 *)(work + 0x570) != 0) {
        return 0;
    }
    func_002E9340(0x50000);
    mnuInitPartyPanelSlots(work + 0x574);
    mnuAppendCampSpriteRequests(*(s32 *)(work + 0x58), work + 0x4F8);
    effRequestResourceByMode(D_003AFA88, D_003AFA98, 0, work + 0x90);
    mnuRequestBaseAssets(work + 0xD1C);
    *(s32 *)(work + 0x570) = 1;
    kwlnFadeInStart(0, 0, 0, 1);
    kwlnFadeInStart(0, 0, 0, 0);
    return 1;
}

extern s32 mnuStaffSlotsAllFilled(s32, s32);
extern s32 mnuInitializeCampAssetSprites(s32);
extern void brsOpenSkillPackagePanel(BrsSkillPackageWork *);
extern void kwlnFadeOutStart(s32, s32, s32, s32);

s32 brsAdvanceSkillPackagePanel(s32 work) {
    BrsSkillPackageWork *ctx = (BrsSkillPackageWork *)work;

    if (ctx->setupState == 0) {
        return 1;
    }
    if (ctx->setupState == 2) {
        return 0;
    }
    if (mnuStaffSlotsAllFilled(ctx->fadeTarget, work + 0x4F8) == 0) {
        return 1;
    }
    if (func_002877A8() == 1) {
        return 1;
    }
    if (ctx->unitHandle == 0) {
        return 1;
    }
    if (mnuInitializeCampAssetSprites(work + 0xD1C) == 0) {
        return 1;
    }
    brsOpenSkillPackagePanel(ctx);
    ctx->setupState = 2;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 0;
}

extern s32 D_003BAA00;

/* Five party slots, followed by the number of reward rows in this batch. */
typedef struct BrsRewardRow {
    s32 unit;           /* 0x00 */
    s32 amount;         /* 0x04 */
    u8 pad08[0x10];
} BrsRewardRow;

typedef struct BrsRewardBatch {
    BrsRewardRow rows[5]; /* 0x00 */
    s32 count;            /* 0x78 */
} BrsRewardBatch;

typedef struct BrsPartyRow {
    s32 flags;          /* 0x00 */
    s32 amount;         /* 0x04 */
    u8 pad08[0x24];
} BrsPartyRow;

typedef struct BrsRewardUnit {
    u16 flags;          /* 0x00: bit 0 indicates an occupied party slot */
} BrsRewardUnit;

typedef struct BrsTaskState {
    u8 pad00[0x344];
    s32 pendingRows;    /* 0x344 */
} BrsTaskState;

/* Match each reward record to the five active party units and tag its row. */
void brsMarkPartyRows(u8 *dst, u8 *state, s32 flags) {
    s32 i;

    for (i = 0; i < ((BrsRewardBatch *)state)->count; i++) {
        u8 *d = dst;
        u8 *unit = *(u8 **)&D_003BAA00 + 0xA60;
        s32 j;

        for (j = 4; j >= 0; j--) {
            if ((((BrsRewardUnit *)unit)->flags & 1) != 0) {
                BrsRewardRow *row = &((BrsRewardBatch *)state)->rows[i];
                if (row->unit == (s32)unit) {
                    ((BrsPartyRow *)d)->flags |= flags;
                    if (flags & 2) {
                        ((BrsPartyRow *)d)->amount = row->amount;
                    }
                }
            }
            d += 0x2C;
            unit += 0x1A4;
        }
    }
}

void brsMarkPartyRowsFromLists(u32 partyRows, u32 primaryRewards, u32 secondaryRewards) {
    brsMarkPartyRows(partyRows, primaryRewards, 2);
    brsMarkPartyRows(partyRows, secondaryRewards, 1);
}

/* Latch whether the battle-result task still has pending reward rows. */
void brsTaskLatchPendingRows(s32 task) {
    if (((BrsTaskState *)task)->pendingRows == 0) {
        D_003BC529 = 0;
    } else {
        D_003BC529 = 1;
    }
}

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFA88);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFA98);

INCLUDE_ASM(const s32, "game/code_00260208", brsCreateTaskContext);

extern void mnuDrainPanelTransitions(s32, s32);
extern s32 brsAdvanceSkillPackagePanel(s32);
extern void brsCloseSkillPackagePanel(s32);
extern void func_002BC618(s32);
extern void dspCloseChannel(void);
extern void func_002D0918(s32);

void brsStaffTaskDestroy(s32 arg0) {
    s32 context = kwlnTaskGetUserValue();

    if (*(s32 *)(context + 0xD44) != 0) {
        effDestroyResourceSlotSet(*(s32 *)(context + 0xD44));
    }
    mnuDrainPanelTransitions(context + 8, arg0);
    if (brsAdvanceSkillPackagePanel(context) == 0) {
        brsCloseSkillPackagePanel(context);
    }
    func_002BC618(*(s32 *)(context + 0x58));
    dspCloseChannel();
    func_002D0918(*(s32 *)context);
    D_003BC528 = 2;
}

extern char D_003BC530[];
extern char D_003AFAB8[];
extern char D_003AFAC8[];
extern void brsMessageInputStep(void);
extern void mnuStaffRunPanel1(void);
extern void mnuStaffRunPanel2(void);
extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern void *brsCreateTaskContext(void);

s32 mnuStaffCreateTasks(void) {
    s32 result;
    void *work = brsCreateTaskContext();

    kwlnTaskCreate(D_003BC530, 0x405, 1, 0, brsMessageInputStep, 0, work);
    kwlnTaskCreate(D_003AFAB8, 0x2B15, 1, 0, mnuStaffRunPanel1, 0, work);
    result = kwlnTaskCreate(D_003AFAC8, 0x5211, 1, 0, mnuStaffRunPanel2, brsStaffTaskDestroy, work);
    D_003BC528 = 1;
    return result;
}

extern char D_003BC530[];
extern char D_003AFAB8[];
extern char D_003AFAC8[];
extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

u32 mnuStaffDestroyTasks(void) {
    s8 state = D_003BC528;

    if (state == 1) {
        kwlnTaskDestroyWithHierarchyByName(D_003BC530, 0);
        kwlnTaskDestroyWithHierarchyByName(D_003AFAB8, 0);
        kwlnTaskDestroyWithHierarchyByName(D_003AFAC8, 0);
        D_003BC52B = state;
        return 1;
    }
    return 0;
}

s32 brsTaskConsumeDone(void) {
    s32 state = D_003BC528;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC528 = 0;
    }
    return 0;
}

u32 brsTaskTryDestroy(void) {
    if (D_003BC528 == 1) {
        mnuStaffDestroyTasks();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_002629A8);

/* func_00262A30 @ 0x00262A30, 88 bytes.
 * Near-miss 12/13 words: retail materialises several base+offset pointers
 * (`base+0x14`, `base+0x20`, `off+0xEF0`, `off+0x1230`) and writes through
 * them; ee-gcc folds everything to `base+off` plus constant displacements.
 * Writes: base+off+{0xF10=a3, 0xF0C=a2(u16), 0x1250=a4, 0x124C=a2(u16),
 * 0xF14=a5, 0x1254=a6} with off = index*104.
 */
INCLUDE_ASM(const s32, "game/code_00260208", func_00262A30);

s32 brsTaskIsFadeIdle(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002877A8() != 1;
}

void mnuRefreshSelectedUnitPanels(u32 unused, s32 menu) {
    mnuInitPartyPanelSlots(menu + 0x574);
    mnuUpdateHandleStates(menu + 0x680);
    func_00280048(menu + 0x680);
}

typedef struct MenuPanelBlock {
    s32 data[0x69];
} MenuPanelBlock;

void mnuStaffCopyPanelBlock(MenuPanelBlock *src, u8 *base) {
    *(MenuPanelBlock *)(base + 0x9C) = *src;
}

extern s32 D_0036D3C0[];
extern s32 effMiscRand(s32);

s32 mnuStaffPickRoll(void) {
    u32 roll = effMiscRand(0) & 0xFF;
    u32 i;

    for (i = 0; i < 4; i++) {
        if ((s32)roll < D_0036D3C0[i]) {
            return i + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00260208", brsSelectLevelBonusMode);

INCLUDE_ASM(const s32, "game/code_00260208", brsSelectNextUnit);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAB8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAC8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAD8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAE8);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC510);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC518);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC520);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC528);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC529);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC52A);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC52B);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC530);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC538);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC540);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC548);

