#include "common.h"
#include "mnu_result.h"
#include "mnu_list.h"
#include "mnu_staff.h"


extern u8 brsUiUpdateAllowed;


extern s32 kwlnFadeIsActive(void);

extern s32 func_002C6CE8(void);

extern void mnuSetCommandPhase(MenuTerminalContext *, u32);

extern s32 func_00298648(MenuTerminalContext *);
extern void func_00294B40(s32, s32, s32, MenuTerminalContext *, s32, s32);
extern void func_00296D90(MenuTerminalContext *, s32);
extern void func_00294EB8(s32, s32, s32, MenuTerminalContext *, s32, s32);
extern void func_00295030(s32, s32, s32, MenuTerminalContext *, s32, s32, s32);
extern void func_00296430(s32, s32, s32, MenuTerminalContext *, u32, s32);
extern void mnuDrawIfActive(s32, s32, s32, MenuWindowContainer *, s32);
extern void itfEmitSelectedGlyph(MenuTerminalContext *, s32, s32, u32, u32);
extern void func_00296C58(s32, s32, s32, MenuTerminalContext *, s32);
extern void func_002968B8(s32, s32, s32, MenuTerminalContext *, s32, s32);
extern void func_00296298(s32, s32, s32, MenuTerminalContext *, s32);
extern void func_002967A0(s32, s32, s32, MenuTerminalContext *, s32);


extern void func_00297200(struct MenuList *, u32);

extern void mnuStorePendingMenuCommandValue(struct MenuList *, u32);

extern s8 brsUpdateBlocked;

extern s8 brsPendingRowsLatched;

extern s8 brsTaskState;

extern void sndEnsureMidiBankResident(s32);


extern void mnuAppendCampSpriteRequests(s32, StaffSlots *);

extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);

extern void mnuRequestEffectResources(MenuEffectResources *);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern char D_00428358[];

extern char D_00428368[];



extern void ptyAdjustItemQuantity(s32, s32);

extern char brsStaffInputTaskName[];

extern char mnuStaffPrimaryPanelTaskName[];

extern char mnuStaffSecondaryPanelTaskName[];

extern char brsStaffInputTaskName[];

extern char mnuStaffPrimaryPanelTaskName[];

extern char mnuStaffSecondaryPanelTaskName[];

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern s32 kwlnTaskGetTaskByName(const char *);


extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *unit);
extern u32 ptyAddProfileRecordValueClamped(DatPartyRecord *unit, u32 amount);
extern void func_00299988(BrsSkillPackageWork *, s32, s32, u32, s32, s32, s32);

typedef s16 BrsIconRecord[4];

enum {
    BRS_ICON_ID = 1,
    BRS_ICON_X = 2,
    BRS_ICON_Y = 3,
};

extern BrsIconRecord D_003D03F0[];
extern void func_00306CD0(s32, s32, s32, u32, s32, struct EffectSlotSet *, s32, s32);

/* Draw the selected result icon, then the fixed companion at the same alpha. */
void func_00296E98(s32 unused, u32 alpha, s32 iconIndex, s32 option) {
    f32 strength;
    struct EffectSlotSet *layer;

    layer = D_00438FC8->effectSlots[0];
    strength = (f32)alpha * 0.00390625f;
    iconIndex += 3;
    func_00306CD0(D_003D03F0[iconIndex][BRS_ICON_X] << 4,
                  D_003D03F0[iconIndex][BRS_ICON_Y] << 3, 0,
                  (u32)(strength * 256.0f), 0, layer,
                  D_003D03F0[iconIndex][BRS_ICON_ID], option);
    func_00306CD0(D_003D03F0[8][BRS_ICON_X] << 4,
                         D_003D03F0[8][BRS_ICON_Y] << 3, 0,
                         (u32)(strength * 256.0f), 0, layer,
                         D_003D03F0[8][BRS_ICON_ID], option);
}

void func_00297000(s32 unused, u32 value, s32 entryIndex, s32 drawArg) {
    f32 normalized;
    f32 companionPosition;
    struct EffectSlotSet *layer;
    s32 companionX;
    s32 selectedIndex;

    layer = D_00438FC8->effectSlots[0];
    normalized = (f32)value * 0.00390625f;
    selectedIndex = entryIndex + 3;
    func_00306CD0(D_003D03F0[selectedIndex][BRS_ICON_X] << 4,
                  D_003D03F0[selectedIndex][BRS_ICON_Y] << 3, 0,
                  (u32)(normalized * 256.0f), 0, layer,
                  D_003D03F0[selectedIndex][BRS_ICON_ID], drawArg);

    companionPosition = (normalized + normalized * normalized) * 0.5f;
    if (normalized < 0.5f) {
        normalized = 0.1f;
    } else {
        normalized = (normalized - 0.5f) * 2.0f;
    }
    companionX = (s32)((f32)D_003D03F0[8][BRS_ICON_X] -
                       (1.0f - companionPosition) * 128.0f);

    func_00306CD0(companionX << 4,
                         D_003D03F0[8][BRS_ICON_Y] << 3, 0,
                         (u32)(normalized * 256.0f), 0, layer,
                         D_003D03F0[8][BRS_ICON_ID], drawArg);
}

void mnuStorePendingMenuCommandValue(struct MenuList *list, u32 value) {
    MenuTerminalWindowState *window;

    window = list->context;
    if (window != NULL) {
        window->command.value = value;
        window->command.mode = 1;
    }
}

void func_002971E0(struct MenuList *list, u32 value) {
    MenuTerminalWindowState *window;

    window = list->context;
    if (window != NULL) {
        window->command.value = value;
        window->command.mode = 2;
    }
}

void func_00297200(struct MenuList *list, u32 value) {
    MenuTerminalWindowState *window;

    window = list->context;
    if (window != NULL) {
        window->command.value = value;
        window->command.mode = 1;
    }
}

void func_00297220(struct MenuList *list, u32 value) {
    MenuTerminalWindowState *window;

    window = list->context;
    if (window != NULL) {
        window->command.value = value;
        window->command.mode = 2;
    }
}

void mnuSetCommandPhase(MenuTerminalContext *owner, u32 value) {
    owner->phase = value;
    owner->commandFrames = 0;
}

s32 func_00297250(MenuTerminalContext *owner) {
    switch (owner->phase) {
    case 0:
    case 14:
        owner->commandFrames = owner->commandFrames + 1;
        if ((f32)owner->commandFrames > 10.0f) {
            return 0;
        }
        return -1;
    case 15:
        owner->commandFrames = owner->commandFrames + 1;
        if ((f32)owner->commandFrames > 10.0f) {
            return 1;
        }
        return -1;
    case 1:
        return 1;
    case 2:
        owner->commandFrames = owner->commandFrames + 1;
        if ((f32)owner->commandFrames > 10.0f) {
            return 2;
        }
        return -1;
    case 3:
        return 3;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297320);

/* Phase machine of the menu command: phases 4, 5 and 8 wait for the frame
 * counter to pass 10.0f before reporting themselves, 6 and 7 report at once.
 * The counter is compared as a float because retail loads 10.0f into $f1 and
 * converts the counter with cvt.s.w (lui at,0x4120 / mtc1 / cvt.s.w / c.lt.s).
 * Phase 8 uses c.le.s, so it fires one frame earlier than 4 and 5. */
s32 mnuTickExtendedCommandPhase(MenuTerminalContext *work) {
    switch (work->phase) {
    case 4:
        work->commandFrames = work->commandFrames + 1;
        if ((f32)work->commandFrames > 10.0f) {
            return 4;
        }
        break;
    case 5:
        work->commandFrames = work->commandFrames + 1;
        if ((f32)work->commandFrames > 10.0f) {
            return 5;
        }
        break;
    case 7:
        return 7;
    case 8:
        work->commandFrames = work->commandFrames + 1;
        if ((f32)work->commandFrames >= 10.0f) {
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
s32 mnuTickCommandWaitPhase(MenuTerminalContext *work) {
    switch (work->phase) {
    case 9:
        work->commandFrames = work->commandFrames + 1;
        if ((f32)work->commandFrames > 10.0f) {
            return 9;
        }
        break;
    case 11:
        return 11;
    case 10:
        work->commandFrames = work->commandFrames + 1;
        if ((f32)work->commandFrames >= 10.0f) {
            return 10;
        }
        break;
    case 12:
        work->commandFrames = work->commandFrames + 1;
        if ((f32)work->commandFrames >= 10.0f) {
            return 12;
        }
        break;
    }
    return -1;
}

/* Draw the selected terminal/shop page through its entry, change and exit phases. */
s32 func_00298648(MenuTerminalContext *scene) {
    s32 category;
    f32 progress;

    category = scene->ownedWindows[0]->list->cursor->camp.value + 1;
    func_00294B40(0, 0, 0, scene, 0x100, 0x53);
    func_00296D90(scene, 0xA09DC380);
    switch (scene->phase) {
    case 9:
        func_00294EB8(0, 0, 0, scene, 0x100, 0x53);
        func_00295030(0, 0, 0, scene, 0x100, 1, 0x53);
        progress = (f32)scene->commandFrames / 10.0f;
        func_00296430(0, 0, 0, scene, (u32)(progress * 256.0f), 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4, 0xA09DC360, 0x53);
        func_00296C58(0, 0, 0, scene, 0x53);
        func_002968B8(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        switch (category) {
        case 1:
        case 2:
        case 3:
            func_00296E98((s32)scene, (u32)((1.0f - progress) * 256.0f), 3, 0x53);
            func_00296E98((s32)scene, (u32)(progress * 256.0f), 1, 0x53);
            break;
        case 4:
            func_00296E98((s32)scene, (u32)((1.0f - progress) * 256.0f), 2, 0x53);
            func_00296E98((s32)scene, (u32)(progress * 256.0f), 0, 0x53);
            break;
        }
        break;
    case 10:
        func_00294EB8(0, 0, 0, scene, 0x100, 0x53);
        func_00295030(0, 0, 0, scene, 0x100, 1, 0x53);
        progress = (f32)scene->commandFrames / 10.0f;
        progress = 1.0f - progress;
        func_00296430(0, 0, 0, scene, (u32)(progress * 256.0f), 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4, 0xA09DC360, 0x53);
        func_00296C58(0, 0, 0, scene, 0x53);
        func_002968B8(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        switch (category) {
        case 1:
        case 2:
        case 3:
            func_00296E98((s32)scene, (u32)(progress * 256.0f), 1, 0x53);
            func_00296E98((s32)scene, (u32)((1.0f - progress) * 256.0f), 3, 0x53);
            break;
        case 4:
            func_00296E98((s32)scene, (u32)(progress * 256.0f), 0, 0x53);
            func_00296E98((s32)scene, (u32)((1.0f - progress) * 256.0f), 2, 0x53);
            break;
        }
        break;
    case 12:
        func_00294EB8(0, 0, 0, scene, 0x100, 0x53);
        func_00296298(0, 0, 0, scene, 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        func_00296C58(0, 0, 0, scene, 0x53);
        progress = (f32)scene->commandFrames / 10.0f;
        progress = 1.0f - progress;
        func_00295030(0, 0, 0, scene, (s32)(progress * 256.0f), 1, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4,
                            0xA09DC300 | (s32)(progress * 96.0f), 0x53);
        func_002968B8(0, 0, 0, scene, (s32)(progress * 256.0f), 0x53);
        switch (category) {
        case 1:
        case 2:
        case 3:
            func_00296E98((s32)scene, 0x100, 1, 0x53);
            break;
        case 4:
            func_00296E98((s32)scene, 0x100, 0, 0x53);
            break;
        }
        break;
    case 11:
        func_00294EB8(0, 0, 0, scene, 0x100, 0x53);
        func_00295030(0, 0, 0, scene, 0x100, 1, 0x53);
        func_00296298(0, 0, 0, scene, 0x53);
        mnuDrawIfActive(0, 0, 0, scene->window, 0x53);
        itfEmitSelectedGlyph(scene, 1, 4, 0xA09DC360, 0x53);
        func_00296C58(0, 0, 0, scene, 0x53);
        func_002967A0(0, 0, 0, scene, 0x53);
        switch (category) {
        case 1:
        case 2:
        case 3:
            func_00296E98((s32)scene, 0x100, 1, 0x53);
            break;
        case 4:
            func_00296E98((s32)scene, 0x100, 0, 0x53);
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


/* Cap stored EXP at the EXP required for level 99. */
void ptyClampExp(DatPartyRecord *unit) {
    DatPartyRecord snapshot;
    u32 exp;

    memcpy(&snapshot, unit, sizeof(snapshot));
    snapshot.level = 0x63;
    exp = ptyComputeTotalExp(&snapshot, 0);
    if (exp < unit->totalExp) {
        unit->totalExp = exp;
    }
}

/* Snapshot eligible party members, then apply the queued EXP and profile
 * rewards to each referenced unit. */
void func_00299018(BrsSkillPackageWork *partyWork, BrsRewardBatch *batch) {
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
            func_00299988(partyWork, partyIndex, unit->level,
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

            func_00299988(partyWork, values->partySlot, unit->level,
                          unit->totalExp, ptyGetCurrentProfileRecord(unit)->value,
                          experienceGain, profilePointGain);
            unit->totalExp += experienceGain;
            ptyClampExp(unit);
            if (unit->profileId != 0) {
                ptyAddProfileRecordValueClamped(unit, profilePointGain);
            }
            rewardIndex++;
            values = (BrsRewardValues *)((u8 *)values + sizeof(BrsRewardRow));
            rewardOffset += sizeof(BrsRewardRow);
        } while (rewardIndex < batch->count);
    }
}

void brsApplyRewardBundle(BrsSkillPackageWork *partyWork, BrsRewardSummary *batch,
                          BrsRewardBatch *rewardState) {
    gstApplyCounterDeltaTable(batch->icons);
    gstApplyBundleMacca(batch);
    func_00299018(partyWork, rewardState);
}

extern void mnuReleaseStaffMenuResources(s32);
extern void evtStageTestInit(s32);



extern void mnuForwardTableByte(s32);

/* Build the selected reward row's skill-package group and sprite, then
 * forward the selected unit ID to the menu. */
void brsOpenSkillPackagePanel(BrsSkillPackageWork *work) {
    u32 *group = work->staffSlots.baseResources;
    MenuPanelGroup *panel;

    mnuReleaseStaffMenuResources((s32)group);
    mnuInitializeCampPanelResources(&work->partyWindow, &work->staffSlots, 0, &work->partyPanel);
    panel = mnuCreatePanelGroup(work->staffSlots.baseResources[1], work->staffSlots.pairResources[0], 0);
    work->panelHandle = panel;
    mnuUpdateFiveListEntries(panel, work->unitHandle);
    work->spriteHandle =
        mnuCreateSpriteState((struct EffectSlotSet *)work->staffSlots.baseResources[1],
                             (struct EffectSlotSet *)work->staffSlots.pairResources[0],
                             (struct EffectSlotSet *)work->staffSlots.baseResources[0]);
    evtStageTestInit(0);
    mnuForwardTableByte(work->primaryRewards.rows[work->selectedRow].unit->unitId);
}

extern void effDestroyResourceSlotSet(s32);
extern void mnuClearEntries(MenuPageWindow *);
extern void mnuReleasePartyIconBundles(MenuPageWindow *);
extern void mnuShutdownContext();
extern void mnuDestroyEffectResources(MenuEffectResources *);
extern void mnuReleaseStaffMenuTextureHandles();
extern void mnuReleaseTitleEffectSprites(StaffSlots *);
extern void mnuResetWorkFloats(void);

void brsCloseSkillPackagePanel(BrsSkillPackageWork *ctx) {
    MenuPageWindow *panelContext = &ctx->partyWindow;

    effDestroyResourceSlotSet(ctx->unitHandle);
    mnuClearEntries(panelContext);
    mnuReleasePartyIconBundles(panelContext);
    mnuShutdownContext((s32)panelContext);
    mnuDestroyPanelGroup(ctx->panelHandle);
    mnuFreeSpriteStateWork(ctx->spriteHandle);
    mnuDestroyEffectResources(&ctx->campEffect.resources);
    mnuReleaseStaffMenuTextureHandles((s32)&ctx->staffSlots);
    mnuReleaseTitleEffectSprites(&ctx->staffSlots);
    mnuResetWorkFloats();
}

s32 brsStartPartyPanelResourcesOnce(BrsSkillPackageWork *work) {
    if (work->setupState != 0) {
        return 0;
    }
    sndEnsureMidiBankResident(0x50000);
    mnuInitPartyPanelSlots(&work->partyPanel);
    mnuAppendCampSpriteRequests(work->fadeTarget, &work->staffSlots);
    effRequestResourceByMode(D_00428358, D_00428368, 0, (s32)&work->unitHandle);
    mnuRequestEffectResources(&work->campEffect.resources);
    work->setupState = 1;
    kwlnFadeInStart(0, 0, 0, 1);
    kwlnFadeInStart(0, 0, 0, 0);
    return 1;
}

extern s32 movAreTitleEffectsReady(s32, StaffSlots *);
extern s32 mnuBindCampEffectWhenLoaded(MenuCampEffect *);
extern void kwlnFadeOutStart(s32, s32, s32, s32);

s32 brsAdvanceSkillPackagePanel(BrsSkillPackageWork *ctx) {

    if (ctx->setupState == 0) {
        return 1;
    }
    if (ctx->setupState == 2) {
        return 0;
    }
    if (movAreTitleEffectsReady(ctx->fadeTarget, &ctx->staffSlots) == 0) {
        return 1;
    }
    if (func_002C6CE8() == 1) {
        return 1;
    }
    if (ctx->unitHandle == 0) {
        return 1;
    }
    if (mnuBindCampEffectWhenLoaded(&ctx->campEffect) == 0) {
        return 1;
    }
    brsOpenSkillPackagePanel(ctx);
    ctx->setupState = 2;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 0;
}


/* Tag each matching occupied party slot with its reward flags and amount. */
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

/* Latch whether the result task still has pending reward rows. */
void brsTaskLatchPendingRows(BrsSkillPackageWork *context) {
    if (context->primaryRewards.count == 0) {
        brsPendingRowsLatched = 0;
    } else {
        brsPendingRowsLatched = 1;
    }
}

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern s32 mnuAllocateValueRecord(s32);
extern char D_003D05C8[];
extern s32 func_0029D008(BrsRewardBatch *, BrsRewardSummary *);
extern s32 func_0029D2D8(BrsRewardBatch *);
extern s32 brsBuildProfileCapList(BrsRewardBatch *);
extern void func_0029DA98(BrsActiveProgressList *);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428358);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428368);

BrsSkillPackageWork *brsCreateRewardTaskWork(void) {
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
    mnuClearPanelTransitionState(&work->transition.data);
    work->fadeTarget = mnuAllocateValueRecord(1);
    evtCreateMessageWindowIfMissing(D_003D05C8);
    evtSetMessageWindowPageValue(200);
    rewards = &work->rewards;
    func_001AA400(rewards);
    party = work->partyProgress.rows;
    rewardState = &work->rewardState;
    func_0029D008(rewardState, rewards);
    brsApplyRewardBundle(work, rewards, rewardState);
    primary = &work->primaryRewards;
    func_0029D2D8(primary);
    secondary = &work->secondaryRewards;
    brsBuildProfileCapList(secondary);
    func_0029DA98(&work->partyProgress);
    brsMarkPartyRowsFromLists(party, primary, secondary);
    work->fadeProgress = 0x100;
    brsTaskLatchPendingRows(work);
    work->teardownHandle = 0;
    effRequestResourceByMode(D_00428358, "easy_r01.spr", 0,
                            (s32)&work->teardownHandle);
    return work;
}

extern u32 kwlnTaskGetUserValue();
extern void effDestroyResourceSlotSet(s32);
extern s32 brsAdvanceSkillPackagePanel(BrsSkillPackageWork *);
extern void brsCloseSkillPackagePanel(BrsSkillPackageWork *);
extern void func_00303D58(s32);
extern s32 dspCloseChannel(void);
extern void sdfReleaseResourceAllocation(s32);

/* Release the panel and task resources, then mark the result task finished. */
void brsStaffTaskDestroy(s32 taskArg) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (context->teardownHandle != 0) {
        effDestroyResourceSlotSet(context->teardownHandle);
    }
    mnuDrainPanelTransitions(&context->transition.data, taskArg);
    if (brsAdvanceSkillPackagePanel(context) == 0) {
        brsCloseSkillPackagePanel(context);
    }
    func_00303D58(context->fadeTarget);
    dspCloseChannel();
    sdfReleaseResourceAllocation(context->handle);
    brsTaskState = 2;
}

extern s32 kwlnTaskCreate(void *name, s32 flags, s32 prio, s32 stacked, void *update, void *destroy, void *data);
extern BrsSkillPackageWork *brsCreateRewardTaskWork(void);
extern s32 brsMessageInputStep(void *);
extern s32 mnuStaffRunPanel1(s32);
extern s32 mnuStaffRunPanel2(s32);
extern void brsStaffTaskDestroy(s32);

s32 mnuStaffCreateTasks(void) {
    s32 result;
    BrsSkillPackageWork *work = brsCreateRewardTaskWork();

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
    s32 task;
    BrsSkillPackageWork *work;

    task = kwlnTaskGetTaskByName(mnuStaffPrimaryPanelTaskName);
    if (task == 0) {
        return task;
    }
    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    if (256 - work->fadeProgress <= 0 && brsTaskIsUiUpdateAllowed(work) != 0) {
        if (work->opacityReady == 1) {
            return 1;
        }
    }
    return work->opacityReady;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299988);

s32 brsTaskIsFadeIdle(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002C6CE8() != 1;
}

void mnuRefreshSelectedUnitPanels(u32 unused, BrsSkillPackageWork *menu) {
    mnuInitPartyPanelSlots(&menu->partyPanel);
    func_002BCA98(&menu->partyWindow);
    func_002BCAB0(&menu->partyWindow);
}

void mnuStaffCopyPanelBlock(DatPartyRecord *src, BrsSkillPackageWork *base) {
    base->previewUnit = *src;
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

