#include "mnu.h"
#include "mnu_result.h"

extern s32 kwlnTaskGetUserValue();

extern void func_0024DD78(void);
extern u8 *D_003BAA70;
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern u8 brsGetLevelStepCrossedBy(s32, s32);
extern u8 brsGetLevelStepForValue(s32);
extern s32 func_003014F0(char *, const char *, ...);
extern void dspSetActive(s32);
extern void dspStartEntry(s32);
extern void evtStageTestQueueMotion(s32, u32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern char D_003BC550[];
extern char D_003BC558[];
extern char *D_0036D3A8[];
extern void mnuClearItemSelectionSlots(BrsSkillPackageWork *);
extern void mnuRefreshPartyUnitVitalsPanels(DatPartyRecord *, BrsSkillPackageWork *);


/* All five signed-byte plus table-word totals must meet the minimum. */
s32 mnuCheckTableSums(DatPartyRecord *bytes, BrsSkillPackageWork *table) {
    s32 *tableValues = table->statGains;
    s8 *byteValues = bytes->baseStats;
    s32 index = 0;

    do {
        s32 total = *byteValues + *tableValues;

        byteValues++;
        tableValues++;
        if (total < MENU_SUM_MINIMUM) {
            return 0;
        }
        index++;
    } while (index < MENU_SUM_COUNT);
    return 1;
}

extern s32 evtStageTestUpdateCamera(void);
extern s32 evtGetMessageWindowControlState(void);
extern s32 mnuMapPadMaskToFlags(s32);
extern s32 mnuGetPanelGroupSelection(s32);
extern void mnuSetPanelGroupSelection(s32, s32);
extern void mnuSetPopupEntryFlagged(s32 *, char *);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern char D_0036D440[];

s32 func_00263EF8(void *request) {
    BrsSkillPackageWork *work;
    DatPartyRecord *entry;
    s32 inputFlags;
    s32 selection;
    s32 changed = 0;
    s32 result;

    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    inputFlags = mnuMapPadMaskToFlags(0xF3);
    entry = work->selectedRewardRow->unit;
    evtStageTestUpdateCamera();
    result = func_00285670(&work->transition, &work->transition.state, 0, request);
    if (result != 0) {
        return result;
    }
    if (work->transition.state != 0 || evtGetMessageWindowControlState() != 0) {
        return 0;
    }

    if (work->selectionInitialized == 0) {
        mnuSetPanelGroupSelection(work->panelHandle, 0);
        work->selectionInitialized = 1;
    }

    if (work->assignedStatPoints == work->availableStatPoints ||
        mnuCheckTableSums(entry, work) != 0) {
        dspStartEntry(0x16);
        evtSetMessageWindowOptionWhenOpen(0);
        evtStoreValueAndCaptureWindowPanelValue(0x1D);
        mnuSetPopupEntryFlagged(&work->transition.state, D_0036D440);
    } else {
        selection = mnuGetPanelGroupSelection(work->panelHandle);
        if ((inputFlags & 0x81) != 0) {
            if (entry->baseStats[selection] + work->statGains[selection] < 99) {
                inputFlags = 1;
                changed = 1;
                work->assignedStatPoints++;
                work->statGains[selection]++;
            } else {
                inputFlags = 0x8000;
            }
        }
        if ((inputFlags & 0x40) != 0) {
            if (work->assignedStatPoints > 0 && work->statGains[selection] > 0) {
                changed = 1;
                work->assignedStatPoints--;
                work->statGains[selection]--;
            } else {
                inputFlags = 0x8000;
            }
        }
        if ((inputFlags & 2) != 0) {
            changed = 1;
            mnuClearItemSelectionSlots(work);
        }
        if (changed != 0) {
            memcpy(entry, &work->previewUnit, sizeof(*entry));
            mnuRefreshPartyUnitVitalsPanels(entry, work);
        }
        if ((inputFlags & 0x10) != 0) {
            selection--;
        }
        if ((inputFlags & 0x20) != 0) {
            selection++;
        }
        if (selection < 0) {
            selection = 4;
        }
        if (selection >= 5) {
            selection = 0;
        }
        mnuSetPanelGroupSelection(work->panelHandle, selection);
    }
    mnuPlayInputSound(0, inputFlags, 0);
    return 0;
}

extern void mnuDrawItemPanelBackdrop(BrsSkillPackageWork *);
extern void func_00263B78(BrsSkillPackageWork *, s32);

s32 mnuDrawItemPanelDuringRequest(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, (void *)request);
}

s32 func_00264238(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)input);
}

u32 mnuBeginPanelEntryAndCaptureSoundMode(void) {
    BrsSkillPackageWork *context;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    mnuSetPanelGroupSelection(context->panelHandle, -1);
    dspStartEntry(0x16);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

extern s8 evtGetCapturedWindowPanelValue(void);
extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern void mnuClearPanelGroupSelection(s32);
extern void mnuBindPresentMenuEntry(void *, s32 *);
extern char D_0036D45C[];

s32 func_002642D0(void *request) {
    BrsSkillPackageWork *work;
    DatPartyRecord *entry;
    s32 result;

    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    entry = work->selectedRewardRow->unit;
    evtStageTestUpdateCamera();
    result = func_00285670(&work->transition, &work->transition.state, 0, request);
    if (result != 0) {
        return result;
    }
    if (work->transition.state == 0 && evtGetMessageWindowControlState() == 0) {
        s8 capturedValue = evtGetCapturedWindowPanelValue();
        s32 *stats = work->statGains;

        if (capturedValue == 0) {
            btlAddBaseStats(stats, work->selectedRewardRow->unit);
            mnuClearItemSelectionSlots(work);
            mnuClearPanelGroupSelection(work->panelHandle);
            mnuSetPopupEntryFlagged(&work->transition.state, D_0036D45C);
            work->commitComplete = 1;
        } else {
            mnuClearItemSelectionSlots(work);
            mnuSetPanelGroupSelection(work->panelHandle, 0);
            memcpy(entry, &work->previewUnit, sizeof(*entry));
            mnuRefreshPartyUnitVitalsPanels(entry, work);
            mnuBindPresentMenuEntry(&work->transition, &work->transition.state);
        }
    }
    return 0;
}

s32 func_00264498(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, (void *)request);
}

s32 func_002644F0(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)input);
}

u32 func_00264538(void) {
    char text[0x20];
    BrsSkillPackageWork *work = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    BrsRewardRow *state = work->selectedRewardRow;
    DatPartyRecord *entry = state->unit;
    u32 step = brsGetLevelStepCrossedBy(entry->level - state->values.amount, state->values.amount);

    work->crossedSteps = step;
    if (step != 0) {
        func_003014F0(text, D_003BC558, D_003BAA70 + entry->unitId * 17);
        evtCopyEntryStringToActiveWindow(0, text);
        func_003014F0(text, D_003BC550, brsGetLevelStepForValue(entry->level));
        evtCopyEntryStringToActiveWindow(1, text);
        dspSetActive(1);
        dspStartEntry(0x17);
        sndSetSequenceVolumePan(7, 0x7F, 0x3F);
    }
    return 1;
}

u32 func_00264608(void) {
    return 1;
}

extern s32 evtStageTestUpdateCamera(void);
extern s32 evtGetMessageWindowControlState(void);
extern void mnuSetPopupEntryFlagged(s32 *, char *);
extern char D_0036D478[];

/* On an idle panel, apply the extra fallback only when the auxiliary check also fails. */
s32 mnuRunPanelWithIdleFallback(void *request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    s32 *panelState = &context->transition.state;
    s32 result;

    evtStageTestUpdateCamera();
    result = func_00285670(&context->transition, panelState, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panelState == 0 && evtGetMessageWindowControlState() == 0) {
        mnuSetPopupEntryFlagged(panelState, D_0036D478);
    }
    return 0;
}

s32 mnuRunItemPanelWithInactiveBackdrop(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel(context, 1, (void *)request);
}

s32 func_002646F8(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)input);
}


u32 func_00264740(void) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    DatPartyRecord *item = context->selectedRewardRow->unit;

    if (context->rewardMode != 0) {
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + item->unitId * 17);
        dspStartEntry(0x18);
    }
    memset(context->statGains, 0, sizeof(context->statGains));
    return 1;
}

u32 func_002647B0(void) {
    evtFinishMessageWindowAndNotify();
    return 1;
}

s32 func_002647D0(BrsSkillPackageWork *work) {
    DatPartyRecord *entry = work->selectedRewardRow->unit;
    s32 result;

    switch (work->rewardMode) {
    case 4: {
        s32 rewardIndex = work->rewardIndex;

        work->statGains[rewardIndex]++;
        evtCopyEntryStringToActiveWindow(1, D_0036D3A8[rewardIndex]);
    }
    /* fall through */
    case 1:
    case 2:
    case 3:
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + entry->unitId * 17);
        dspStartEntry(work->rewardMode + 0x18);
        break;
    default:
        break;
    }

    evtStageTestQueueMotion(2, 1);
    result = work->rewardMode < 4;
    if (result != 0 && work->rewardMode > 0) {
        result = evtStageTestSetPendingEffect(0);
    }
    return result;
}

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC558);

