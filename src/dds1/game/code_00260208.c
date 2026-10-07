#include "common.h"
#include "mnu_result.h"
#include "mnu_list.h"
#include "mnu_shop.h"

extern s32 kwlnTaskGetTaskByName(char *);
extern s32 kwlnTaskGetUserValue();
extern s8 brsTaskIsUiUpdateAllowed(BrsSkillPackageWork *);
extern s32 func_002877A8(void);

extern s32 kwlnFadeIsActive(void);

extern s8 brsPendingRowsLatched;

extern s8 brsUiUpdateAllowed;

extern s8 brsUpdateBlocked;

extern s8 brsTaskState;

typedef s16 BrsIconRecord[4];

enum {
    BRS_ICON_ID = 1,
    BRS_ICON_X = 2,
    BRS_ICON_Y = 3,
};

extern BrsIconRecord D_0036C728[];
extern s32 D_003BC520;
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);

/* Draw a selected result icon and its fixed companion at the same opacity. */
void func_00260208(s32 unused, u32 alpha, s32 iconIndex, s32 option) {
    f32 strength;
    s32 layer;

    layer = D_003BC520;
    strength = (f32)alpha * 0.00390625f;
    iconIndex += 3;
    func_002BF4E0(D_0036C728[iconIndex][BRS_ICON_X] << 4,
                  D_0036C728[iconIndex][BRS_ICON_Y] << 3, 0,
                  (u32)(strength * 256.0f), 0, layer,
                  D_0036C728[iconIndex][BRS_ICON_ID], option);
    func_002BF4E0(D_0036C728[8][BRS_ICON_X] << 4,
                         D_0036C728[8][BRS_ICON_Y] << 3, 0,
                         (u32)(strength * 256.0f), 0, layer,
                         D_0036C728[8][BRS_ICON_ID], option);
}

void func_00260370(s32 unused, u32 value, s32 iconIndex, s32 option) {
    f32 normalized;
    f32 companionPosition;
    s32 layer;
    s32 companionX;
    s32 selectedIndex;

    layer = D_003BC520;
    normalized = (f32)value * 0.00390625f;
    selectedIndex = iconIndex + 3;
    func_002BF4E0(D_0036C728[selectedIndex][BRS_ICON_X] << 4,
                  D_0036C728[selectedIndex][BRS_ICON_Y] << 3, 0,
                  (u32)(normalized * 256.0f), 0, layer,
                  D_0036C728[selectedIndex][BRS_ICON_ID], option);

    companionPosition = (normalized + normalized * normalized) * 0.5f;
    if (normalized < 0.5f) {
        normalized = 0.1f;
    } else {
        normalized = (normalized - 0.5f) * 2.0f;
    }
    companionX = (s32)((f32)D_0036C728[8][BRS_ICON_X] -
                       (1.0f - companionPosition) * 128.0f);

    func_002BF4E0(companionX << 4,
                         D_0036C728[8][BRS_ICON_Y] << 3, 0,
                         (u32)(normalized * 256.0f), 0, layer,
                         D_0036C728[8][BRS_ICON_ID], option);
}

void mnuStorePendingMenuCommandValue(struct MenuList *list, u32 value) {
    MnuShopListContext *command;

    command = list->context;
    if (command != (MnuShopListContext *)0x0) {
        command->countdown = value;
        command->mode = 1;
    }
}

void func_00260550(struct MenuList *list, u32 value) {
    MnuShopListContext *command;

    command = list->context;
    if (command != (MnuShopListContext *)0x0) {
        command->countdown = value;
        command->mode = 2;
    }
}

void func_00260570(struct MenuList *list, u32 value) {
    MnuShopListContext *command;

    command = list->context;
    if (command != (MnuShopListContext *)0x0) {
        command->countdown = value;
        command->mode = 1;
    }
}

void func_00260590(struct MenuList *list, u32 value) {
    MnuShopListContext *command;

    command = list->context;
    if (command != (MnuShopListContext *)0x0) {
        command->countdown = value;
        command->mode = 2;
    }
}

void mnuSetCommandPhase(ShopScene *work, u32 mode) {
    work->action = mode;
    work->frames = 0;
}

s32 mnuStaffTickState(ShopScene *work) {
    s32 count;

    switch (work->action) {
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

/* Phase machine of the menu command: phases 4, 5 and 8 wait for the frame
 * counter to pass 10.0f before reporting themselves, 6 and 7 report at once.
 * The counter is compared as a float because retail loads 10.0f into $f1 and
 * converts the counter with cvt.s.w (lui at,0x4120 / mtc1 / cvt.s.w / c.lt.s).
 * Phase 8 uses c.le.s, so it fires one frame earlier than 4 and 5. */
s32 mnuTickExtendedCommandPhase(ShopScene *work) {
    switch (work->action) {
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

/* Phase machine for phases 9-12: 9 waits for the frame counter to pass 10.0f, 10 and 12 for it to reach 10.0f, 11 reports at once. */
s32 mnuTickCommandWaitPhase(ShopScene *work) {
    switch (work->action) {
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

extern void func_0025E308(s32, s32, s32, ShopScene *, s32, s32);
extern void func_00260100(ShopScene *, s32);
extern void mnuDrawIconTriple(s32, s32, s32, s32, s32, s32);
extern void func_0025E6B0(s32, s32, s32, ShopScene *, s32, s32);
extern void func_0025F7F0(s32, s32, s32, ShopScene *, u32, s32);
extern void mnuDrawIfActive(s32, s32, s32, MenuWindowContainer *, s32);
extern void itfEmitSelectedGlyph(ShopScene *, s32, s32, u32, u32);
extern void func_0025FFC8(s32, s32, s32, ShopScene *, s32);
extern void func_0025FC38(s32, s32, s32, ShopScene *, s32, s32);
extern void func_0025F680(s32, s32, s32, ShopScene *, s32);
extern void func_0025FB30(s32, s32, s32, ShopScene *, s32);

/* Draw the selected shop page through its entry, change and exit phases. */
INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC510);

s32 func_00261760(ShopScene *scene) {
    s32 categoryMap[2] = {0, 2};
    s32 category;
    f32 progress;

    if (scene->extraOption != 0) {
        category = scene->sprite->list->cursor->index;
    } else {
        category = categoryMap[scene->sprite->list->cursor->index];
    }
    func_0025E308(0, 0, 0, scene, 0x100, 0x53);
    func_00260100(scene, 0xA09DC380);
    switch (scene->action) {
    case 9:
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025E6B0(0, 0, 0, scene, 0x100, 0x53);
        progress = (f32)scene->frames / 10.0f;
        func_0025F7F0(0, 0, 0, scene, (u32)(progress * 256.0f), 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4, 0xA09DC360, 0x53);
        func_0025FFC8(0, 0, 0, scene, 0x53);
        func_0025FC38(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        switch (category) {
        case 0:
        case 1:
            func_00260208((s32)scene, (u32)((1.0f - progress) * 256.0f), 3, 0x53);
            func_00260208((s32)scene, (u32)(progress * 256.0f), 1, 0x53);
            break;
        case 2:
            func_00260208((s32)scene, (u32)((1.0f - progress) * 256.0f), 2, 0x53);
            func_00260208((s32)scene, (u32)(progress * 256.0f), 0, 0x53);
            break;
        }
        break;
    case 10:
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025E6B0(0, 0, 0, scene, 0x100, 0x53);
        progress = (f32)scene->frames / 10.0f;
        progress = 1.0f - progress;
        func_0025F7F0(0, 0, 0, scene, (u32)(progress * 256.0f), 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4, 0xA09DC360, 0x53);
        func_0025FFC8(0, 0, 0, scene, 0x53);
        func_0025FC38(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        switch (category) {
        case 0:
        case 1:
            func_00260208((s32)scene, (u32)(progress * 256.0f), 1, 0x53);
            func_00260208((s32)scene, (u32)((1.0f - progress) * 256.0f), 3, 0x53);
            break;
        case 2:
            func_00260208((s32)scene, (u32)(progress * 256.0f), 0, 0x53);
            func_00260208((s32)scene, (u32)((1.0f - progress) * 256.0f), 2, 0x53);
            break;
        }
        break;
    case 12:
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025F680(0, 0, 0, scene, 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        func_0025FFC8(0, 0, 0, scene, 0x53);
        progress = (f32)scene->frames / 10.0f;
        progress = 1.0f - progress;
        func_0025E6B0(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        itfEmitSelectedGlyph(scene, 1, 4,
                            0xA09DC300 | (s32)(progress * 96.0f), 0x53);
        func_0025FC38(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        switch (category) {
        case 0:
        case 1:
            func_00260208((s32)scene, 0x100, 1, 0x53);
            break;
        case 2:
            func_00260208((s32)scene, 0x100, 0, 0x53);
            break;
        }
        break;
    case 11:
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025E6B0(0, 0, 0, scene, 0x100, 0x53);
        func_0025F680(0, 0, 0, scene, 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4, 0xA09DC360, 0x53);
        func_0025FFC8(0, 0, 0, scene, 0x53);
        func_0025FB30(0, 0, 0, scene, 0x53);
        switch (category) {
        case 0:
        case 1:
            func_00260208((s32)scene, 0x100, 1, 0x53);
            break;
        case 2:
            func_00260208((s32)scene, 0x100, 0, 0x53);
            break;
        }
        break;
    }
    return 0;
}


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

s8 brsTaskIsUiUpdateAllowed(BrsSkillPackageWork *context) {
    if (context->teardownHandle != 0) {
        brsUpdateBlocked = 0;
    }
    return brsUpdateBlocked ? 0 : brsUiUpdateAllowed;
}


extern void ptyAdjustItemQuantity(s32, s32);

void gstApplyCounterDeltaTable(MenuIconRef *refs) {
    u32 i;

    for (i = 0; i < 3; i++) {
        u16 id = refs->id;
        u8 param = refs->param;

        refs++;
        if (id != 0) {
            ptyAdjustItemQuantity(id, param);
        }
    }
}

void gstApplyBundleMacca(BrsRewardSummary *batch) {
    datAddCurrencyClamped(batch->macca);
}

extern s32 ptyComputeTotalExp(DatPartyRecord *, s32);

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *unit);
extern u32 ptyAddProfilePoints(DatPartyRecord *unit, s32 increment);
extern void func_00262A30(BrsSkillPackageWork *, s32, u16, u32, s32, s32, s32);

/* Cap stored EXP at the EXP required for level 99. */
void ptyClampExp(DatPartyRecord *unit) {
    DatPartyRecord snapshot;
    s32 exp;

    memcpy(&snapshot, unit, sizeof(snapshot));
    snapshot.level = 0x63;
    exp = ptyComputeTotalExp(&snapshot, 0);
    if (exp < unit->totalExp) {
        unit->totalExp = exp;
    }
}

void brsApplyPartyRewards(BrsSkillPackageWork *partyWork, BrsRewardBatch *batch) {
    s32 partyIndex;
    s32 partyOffset;
    s32 rewardOffset;
    s32 rewardIndex;
    DatPartyRecord *currentPartyUnit;
    DatPartyRecord *unit;
    s32 profilePointGain;
    s32 experienceGain;
    BrsRewardValues *values;

    for (partyOffset = 0, partyIndex = 0; partyIndex < 5;
         partyIndex++, partyOffset += sizeof(DatPartyRecord)) {
        currentPartyUnit =
            (DatPartyRecord *)((u8 *)datGameState->party + partyOffset);

        if ((currentPartyUnit->flags & 1) != 0 &&
            (currentPartyUnit->status & 0x4000) != 0) {
            unit = currentPartyUnit;
            func_00262A30(partyWork, partyIndex, unit->level,
                          unit->totalExp,
                          ptyGetCurrentProfileRecord(currentPartyUnit)->value, 0, 0);
        }
    }

    rewardIndex = 0;
    if (batch->count > 0) {
        values = &batch->rows[0].values;
        rewardOffset = 0;
        do {
            unit = ((BrsRewardRow *)((u8 *)batch->rows + rewardOffset))->unit;
            profilePointGain = values->amount;
            experienceGain = values->secondaryValue;

            func_00262A30(partyWork, values->partySlot, unit->level,
                          unit->totalExp, ptyGetCurrentProfileRecord(unit)->value,
                          experienceGain, profilePointGain);
            unit->totalExp += experienceGain;
            ptyClampExp(unit);
            if (unit->profileId != 0) {
                ptyAddProfilePoints(unit, profilePointGain);
            }
            rewardIndex++;
            values = (BrsRewardValues *)((u8 *)values + sizeof(BrsRewardRow));
            rewardOffset += sizeof(BrsRewardRow);
        } while (rewardIndex < batch->count);
    }
}

/* Apply item/icon rewards before awarding the party's accumulated gains. */
void brsApplyRewardBundle(BrsSkillPackageWork *partyWork, BrsRewardSummary *batch,
                          BrsRewardBatch *rewardState) {
    gstApplyCounterDeltaTable(batch->icons);
    gstApplyBundleMacca(batch);
    brsApplyPartyRewards(partyWork, rewardState);
}

extern void mnuReleaseStaffMenuResources(s32 *);
extern void mnuInitializeStaffPageWindows(s32, StaffSlots *, s32, s32);
extern void evtStageTestInit(s32);
extern void mnuForwardTableByte(s32);
extern void mnuReleaseStaffResourceGroups(StaffSlots *);



/* Create the group and sprite backing the skill-package panel for the
 * selected reward row, then forward its unit's ID to the menu. */
void brsOpenSkillPackagePanel(BrsSkillPackageWork *work) {
    u32 *group = work->staffSlots.baseResources;
    MenuPanelGroup *panel;

    mnuReleaseStaffMenuResources(group);
    mnuInitializeStaffPageWindows((s32)&work->partyWindow, &work->staffSlots, 0, (s32)&work->partyPanel);
    panel = mnuCreatePanelGroup(work->staffSlots.pairResources[0]);
    work->panelHandle = panel;
    mnuUpdateFiveListEntries(panel, work->unitHandle);
    work->spriteHandle =
        mnuCreateSpriteState((struct EffectSlotSet *)work->staffSlots.baseResources[5],
                             (struct EffectSlotSet *)work->staffSlots.baseResources[2],
                             (struct EffectSlotSet *)work->staffSlots.pairResources[0]);
    evtStageTestInit(0);
    mnuForwardTableByte(work->primaryRewards.rows[work->selectedRow].unit->unitId);
}

void brsCloseSkillPackagePanel(BrsSkillPackageWork *ctx) {
    s32 panelContext = (s32)&ctx->partyWindow;

    effDestroyResourceSlotSet(ctx->unitHandle);
    mnuClearEntries(panelContext);
    mnuReleasePartyIconBundles(panelContext);
    mnuShutdownContext(panelContext);
    mnuDestroyPanelGroup(ctx->panelHandle);
    mnuFreeSpriteStateWork(ctx->spriteHandle);
    mnuReleaseAssets(&ctx->assets);
    mnuReleaseStaffMenuTextureHandles(ctx->staffSlots.baseResources);
    mnuReleaseStaffResourceGroups(&ctx->staffSlots);
    mnuResetWorkFloats();
}

extern char D_003AFA88[];
extern char D_003AFA98[];
extern void sndEnsureMidiBankResident(s32);
extern void mnuInitPartyPanelSlots(s32);
extern void mnuAppendCampSpriteRequests(s32, StaffSlots *);
extern void effRequestResourceByMode(char *, char *, s32, s32);
extern void mnuRequestBaseAssets(MenuAssets *);
extern void kwlnFadeInStart(s32, s32, s32, s32);

s32 mnuStaffInitPanel(BrsSkillPackageWork *work) {
    if (work->setupState != 0) {
        return 0;
    }
    sndEnsureMidiBankResident(0x50000);
    mnuInitPartyPanelSlots((s32)&work->partyPanel);
    mnuAppendCampSpriteRequests(work->fadeTarget, &work->staffSlots);
    effRequestResourceByMode(D_003AFA88, D_003AFA98, 0, (s32)&work->unitHandle);
    mnuRequestBaseAssets(&work->assets);
    work->setupState = 1;
    kwlnFadeInStart(0, 0, 0, 1);
    kwlnFadeInStart(0, 0, 0, 0);
    return 1;
}

extern s32 mnuStaffSlotsAllFilled(s32, StaffSlots *);
extern s32 mnuInitializeCampAssetSprites(MenuAssets *);
extern void brsOpenSkillPackagePanel(BrsSkillPackageWork *);
extern void kwlnFadeOutStart(s32, s32, s32, s32);

s32 brsAdvanceSkillPackagePanel(BrsSkillPackageWork *ctx) {

    if (ctx->setupState == 0) {
        return 1;
    }
    if (ctx->setupState == 2) {
        return 0;
    }
    if (mnuStaffSlotsAllFilled(ctx->fadeTarget, &ctx->staffSlots) == 0) {
        return 1;
    }
    if (func_002877A8() == 1) {
        return 1;
    }
    if (ctx->unitHandle == 0) {
        return 1;
    }
    if (mnuInitializeCampAssetSprites(&ctx->assets) == 0) {
        return 1;
    }
    brsOpenSkillPackagePanel(ctx);
    ctx->setupState = 2;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 0;
}


/* Match each reward record to the five active party units and tag its row. */
void brsMarkPartyRows(BrsProgressRow *dst, BrsRewardBatch *state, s32 flags) {
    s32 i;

    for (i = 0; i < state->count; i++) {
        BrsProgressRow *d = dst;
        DatPartyRecord *unit = datGameState->party;
        s32 j;

        for (j = 4; j >= 0; j--) {
            if ((unit->flags & 1) != 0) {
                BrsRewardRow *row = &state->rows[i];
                if (row->unit == unit) {
                    d->flags |= flags;
                    if (flags & 2) {
                        d->amount = row->values.amount;
                    }
                }
            }
            d++;
            unit++;
        }
    }
}

void brsMarkPartyRowsFromLists(BrsProgressRow *partyRows, BrsRewardBatch *primaryRewards, BrsRewardBatch *secondaryRewards) {
    brsMarkPartyRows(partyRows, primaryRewards, 2);
    brsMarkPartyRows(partyRows, secondaryRewards, 1);
}

/* Latch whether the battle-result task still has pending reward rows. */
void brsTaskLatchPendingRows(BrsSkillPackageWork *task) {
    if (task->primaryRewards.count == 0) {
        brsPendingRowsLatched = 0;
    } else {
        brsPendingRowsLatched = 1;
    }
}

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern void mnuClearPanelTransitionState(void *);
extern s32 mnuAllocateValueRecord(s32);
extern void evtCreateMessageWindowIfMissing(void *);
extern void evtSetMessageWindowPageValue(s32);
extern void func_001A1530(BrsRewardSummary *);
extern s32 brsBuildRewardRows(BrsRewardBatch *, BrsRewardSummary *);
extern s32 brsBuildLevelUpList(BrsRewardBatch *);
extern s32 brsBuildProfileCapList(BrsRewardBatch *);
extern void brsBuildActiveUnitProgressRows(BrsActiveProgressList *);
extern char D_0036C858[];
extern char D_003AFAA8[];

BrsSkillPackageWork *brsCreateTaskContext(void) {
    BrsRewardSummary *rewards;
    s32 handle;
    BrsProgressRow *party;
    BrsRewardBatch *rewardState;
    BrsRewardBatch *primary;
    BrsRewardBatch *secondary;
    BrsSkillPackageWork *work;

    handle = sdfAllocGeneralBlock(sizeof(BrsSkillPackageWork));
    work = sdfResourceRetainAddress(handle);
    memset(work, 0, sizeof(BrsSkillPackageWork));
    work->handle = handle;
    mnuClearPanelTransitionState(work->transition.data);
    work->fadeTarget = mnuAllocateValueRecord(1);
    evtCreateMessageWindowIfMissing(D_0036C858);
    evtSetMessageWindowPageValue(200);
    rewards = &work->rewards;
    func_001A1530(rewards);
    party = work->partyProgress.rows;
    rewardState = &work->rewardState;
    brsBuildRewardRows(rewardState, rewards);
    brsApplyRewardBundle(work, rewards, rewardState);
    primary = &work->primaryRewards;
    brsBuildLevelUpList(primary);
    secondary = &work->secondaryRewards;
    brsBuildProfileCapList(secondary);
    brsBuildActiveUnitProgressRows(&work->partyProgress);
    brsMarkPartyRowsFromLists(party, primary, secondary);
    work->fadeProgress = 0x100;
    brsTaskLatchPendingRows(work);
    work->teardownHandle = 0;
    effRequestResourceByMode(D_003AFA88, D_003AFAA8, 0,
                             (s32)&work->teardownHandle);
    return work;
}

extern void mnuDrainPanelTransitions(s32, s32);
extern void func_002BC618(s32);
extern void dspCloseChannel(void);
extern void sdfReleaseResourceAllocation(s32);

void brsStaffTaskDestroy(s32 arg0) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (context->teardownHandle != 0) {
        effDestroyResourceSlotSet(context->teardownHandle);
    }
    mnuDrainPanelTransitions((s32)context->transition.data, arg0);
    if (brsAdvanceSkillPackagePanel(context) == 0) {
        brsCloseSkillPackagePanel(context);
    }
    func_002BC618(context->fadeTarget);
    dspCloseChannel();
    sdfReleaseResourceAllocation(context->handle);
    brsTaskState = 2;
}

extern char brsStaffInputTaskName[];
extern char mnuStaffPrimaryPanelTaskName[];
extern char mnuStaffSecondaryPanelTaskName[];
extern s32 brsMessageInputStep(void *);
extern s32 mnuStaffRunPanel1(s32);
extern s32 mnuStaffRunPanel2(s32);
extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

s32 mnuStaffCreateTasks(void) {
    s32 result;
    BrsSkillPackageWork *work = brsCreateTaskContext();

    kwlnTaskCreate(brsStaffInputTaskName, 0x405, 1, 0, brsMessageInputStep, 0, work);
    kwlnTaskCreate(mnuStaffPrimaryPanelTaskName, 0x2B15, 1, 0, mnuStaffRunPanel1, 0, work);
    result = kwlnTaskCreate(mnuStaffSecondaryPanelTaskName, 0x5211, 1, 0, mnuStaffRunPanel2, brsStaffTaskDestroy, work);
    brsTaskState = 1;
    return result;
}

extern char brsStaffInputTaskName[];
extern char mnuStaffPrimaryPanelTaskName[];
extern char mnuStaffSecondaryPanelTaskName[];
extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

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

s32 func_002629A8(void) {
    s32 task = kwlnTaskGetTaskByName(mnuStaffPrimaryPanelTaskName);
    BrsSkillPackageWork *work;

    if (task == 0) {
        return 0;
    }
    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    if (0x100 - work->fadeProgress <= 0 &&
        brsTaskIsUiUpdateAllowed(work) != 0 &&
        work->opacityReady == 1) {
        return 1;
    }
    return work->opacityReady;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00262A30);

s32 brsTaskIsFadeIdle(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002877A8() != 1;
}

void mnuRefreshSelectedUnitPanels(DatPartyRecord *unused, BrsSkillPackageWork *menu) {
    mnuInitPartyPanelSlots((s32)&menu->partyPanel);
    mnuUpdateHandleStates(&menu->partyWindow);
    func_00280048((s32)&menu->partyWindow);
}


void mnuStaffCopyPanelBlock(DatPartyRecord *src, BrsSkillPackageWork *base) {
    base->previewUnit = *src;
}

extern s32 mnuStaffRollThresholds[];
extern s32 effMiscRand(s32);

s32 mnuStaffPickRoll(void) {
    u32 roll = effMiscRand(0) & 0xFF;
    u32 i;

    for (i = 0; i < 4; i++) {
        if ((s32)roll < mnuStaffRollThresholds[i]) {
            return i + 1;
        }
    }
    return 0;
}


void brsSelectLevelBonusMode(DatPartyRecord *source, BrsSkillPackageWork *work) {
    s32 eligible[5];
    s32 eligibleCount;
    s32 mode;
    s32 i;

    work->rewardMode = 0;
    if ((effMiscRand(0) & 1) > 0) {
        return;
    }

    mode = mnuStaffPickRoll();
    work->rewardIndex = -1;
    work->rewardMode = mode;
    if (mode != 4) {
        return;
    }

    eligibleCount = 0;
    for (i = 0; i < 5; i++) {
        if (source->baseStats[i] + 1 < 99) {
            eligible[eligibleCount] = i;
            eligibleCount++;
        }
    }

    if (eligibleCount == 0) {
        work->rewardMode = 2;
        return;
    }

    work->rewardIndex = eligible[(u32)effMiscRand(0) % (u32)eligibleCount];
}

extern void mnuClearEntries(s32 *window);
extern void mnuReleasePartyIconBundles(s32 window);
extern void mnuSelectPage(MenuPageWindow *window, s32 index);
extern void mnuResetPartyPanelFade(s32 window, s32 index, s32 unused,
                                   s32 retainScale);
extern void mnuSetWindowResource(s32 index, s32 window, s32 resource,
                                 s32 option);
extern void mnuSetPageParams(MenuSprites *sprites, s32 mode);
extern void mnuAttachPartyIconBundle(s32 index, s32 window, u32 resource);
extern void evtStageTestSelectEntryWithoutInitialValue(u16 id, u32 option);
extern void evtStageTestQueueMotion(s32 kind, u32 index);
extern void func_002E8DD0(u32 sequence);
extern void sndStartTrackDefault(s32 track);

void brsSelectNextUnit(BrsSkillPackageWork *work, s32 selectLevelUp) {
    if (selectLevelUp == 0) {
        s32 *selectedIndex = &work->selectedRow;
        BrsRewardRow *row = &work->secondaryRewards.rows[(*selectedIndex)++];

        work->pendingSkillIndex = 0;
        work->selectedRewardRow = row;
    } else {
        s32 selectedRow = work->selectedRow;
        MenuPageWindow *window = &work->partyWindow;
        s32 page = work->primaryRewards.rows[selectedRow].values.secondaryValue;
        s32 *selectedIndex = &work->selectedRow;

        mnuClearEntries((s32 *)window);
        mnuReleasePartyIconBundles((s32)window);
        mnuSelectPage(window, page);
        mnuResetPartyPanelFade((s32)window, page, 0, 0);
        mnuSetWindowResource(page, (s32)window, work->staffSlots.pairResources[0],
                             work->staffSlots.pairResources[1]);
        mnuSetPageParams(work->partyWindow.slots[page].windowSprites, 2);
        mnuAttachPartyIconBundle(page, (s32)window, work->staffSlots.pairResources[0]);

        ((MenuIconBundle *)work->partyWindow.slots[page].iconBundle)->fade = 0x100;
        window->flags |= 0x400;
        work->selectedRewardRow = &work->primaryRewards.rows[(*selectedIndex)++];
        brsSelectLevelBonusMode(work->selectedRewardRow->unit, work);
        evtStageTestSelectEntryWithoutInitialValue(
            work->selectedRewardRow->unit->unitId, 0);
        evtStageTestQueueMotion(1, 0);

        if (*selectedIndex < work->primaryRewards.count) {
            mnuForwardTableByte(
                work->primaryRewards.rows[*selectedIndex].unit->unitId);
        }
        func_002E8DD0(0x50001);
        sndStartTrackDefault(0x50001);
    }

    mnuStaffCopyPanelBlock(work->selectedRewardRow->unit, work);
}

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFA88);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFA98);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAA8);

INCLUDE_RODATA(const s32, "game/code_00260208", mnuStaffPrimaryPanelTaskName);

INCLUDE_RODATA(const s32, "game/code_00260208", mnuStaffSecondaryPanelTaskName);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAD8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAE8);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC520);

INCLUDE_SDATA(const s32, "game/code_00260208", brsTaskState);

INCLUDE_SDATA(const s32, "game/code_00260208", brsPendingRowsLatched);

INCLUDE_SDATA(const s32, "game/code_00260208", brsUiUpdateAllowed);

INCLUDE_SDATA(const s32, "game/code_00260208", brsUpdateBlocked);

INCLUDE_SDATA(const s32, "game/code_00260208", brsStaffInputTaskName);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC538);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC540);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC548);

