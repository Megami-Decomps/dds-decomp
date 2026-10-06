#include "mnu.h"
#include "mnu_result.h"

extern u32 kwlnTaskGetUserValue();

extern void func_0026C900(void);

extern void func_0029AA48(BrsSkillPackageWork *);

extern void func_0029AC20(BrsSkillPackageWork *, s32);
extern void func_0029B950(DatPartyRecord *, BrsSkillPackageWork *);
extern u8 *D_00435E48;
extern void evtCopyEntryStringToActiveWindow(s32 index, void *value);
extern void *memset(void *destination, s32 value, u32 size);

extern s32 evtStageTestUpdateCamera(void);

extern s32 evtGetMessageWindowControlState(void);

extern void mnuSetPopupEntryFlagged(s32 *, char *);

extern char D_003D64C8[];
extern void mnuClearItemSelectionSlots(BrsSkillPackageWork *);
extern void mnuRefreshPartyUnitVitalsPanels(DatPartyRecord *, BrsSkillPackageWork *);


extern u8 brsGetLevelStepCrossedBy(s32, s32);
extern u8 brsGetLevelStepForValue(s32);
extern s32 func_0035C860(char *, const char *, ...);
extern void dspSetActive(s32);
extern s32 dspStartEntry(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern char D_004379B0[];
extern char D_004379B8[];

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

extern s32 mnuMapPadMaskToFlags(s32);
extern s32 mnuGetPanelGroupSelection(s32);
extern void mnuSetPanelGroupSelection(s32, s32);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern char D_003D6490[];

s32 mnuHandleStatPointAssignment(u64 request) {
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
    result = func_002C4038((s32)&work->transition, &work->transition.state, 0, request);
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
        dspStartEntry(0x17);
        evtSetMessageWindowOptionWhenOpen(0);
        evtStoreValueAndCaptureWindowPanelValue(0xA3);
        mnuSetPopupEntryFlagged(&work->transition.state, D_003D6490);
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

s32 mnuDrawItemPanelDuringRequest(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    func_0029AA48(context);
    func_0029AC20(context, 1);
    return menuSetHandler((s32)context, 1, request);
}

s32 func_0029B378(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

u32 mnuResetGroupSelectionAndStartMessage(void) {
    BrsSkillPackageWork *context;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    mnuSetPanelGroupSelection(context->panelHandle, -1);
    dspStartEntry(0x17);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0xa3);
    return 1;
}

u32 func_0029B410(void) {
    return 1;
}

extern s8 evtGetCapturedWindowPanelValue(void);
extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern void mnuClearPanelGroupSelection(s32);
extern void mnuBindPresentMenuEntry(void *, s32 *);
extern char D_003D64AC[];

s32 mnuCommitAssignedStatPoints(u64 request) {
    BrsSkillPackageWork *work;
    DatPartyRecord *entry;
    s32 result;

    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    entry = work->selectedRewardRow->unit;
    evtStageTestUpdateCamera();
    result = func_002C4038((s32)&work->transition, &work->transition.state, 0, request);
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
            mnuSetPopupEntryFlagged(&work->transition.state, D_003D64AC);
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

s32 func_0029B600(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    func_0029AA48(context);
    func_0029AC20(context, 1);
    return menuSetHandler((s32)context, 1, request);
}

s32 func_0029B658(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

s32 mnuShowProgressLevelChangePopup(void) {
    char text[0x20];
    BrsSkillPackageWork *context;
    DatPartyRecord *item;
    s32 gain;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    item = context->selectedRewardRow->unit;
    gain = context->selectedRewardRow->values.amount;
    context->crossedSteps = brsGetLevelStepCrossedBy(item->level - gain, gain);
    if (context->crossedSteps != 0) {
        func_0035C860(text, D_004379B8, D_00435E48 + item->unitId * 17);
        evtCopyEntryStringToActiveWindow(0, text);
        func_0035C860(text, D_004379B0, brsGetLevelStepForValue(item->level));
        evtCopyEntryStringToActiveWindow(1, text);
        dspSetActive(1);
        dspStartEntry(0x18);
        sndSetSequenceVolumePan(7, 0x7F, 0x3F);
    }
    return 1;
}

u32 func_0029B778(void) {
    return 1;
}

/* On an idle panel, apply the extra fallback only when the auxiliary check also fails. */
s32 mnuRunPanelWithIdleFallback(u64 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    s32 *panelState = &context->transition.state;
    s32 result;

    evtStageTestUpdateCamera();
    result = func_002C4038((s32)&context->transition, panelState, 0, request);
    if (result == 0) {
        if ((*panelState == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(panelState, D_003D64C8);
        }
        result = 0;
    }
    return result;
}

s32 mnuRunItemPanelWithInactiveBackdrop(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    func_0029AA48(context);
    func_0029AC20(context, 0);
    return menuSetHandler((s32)context, 1, request);
}

s32 func_0029B868(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

extern u32 effMiscRand(void *);
extern u16 D_003D6332[][17];
extern s32 mdlFlagTest(s32);

s32 mnuChooseWeightedItem(s32 index) {
    s32 random = (u8)effMiscRand(NULL);
    u16 *entry = D_003D6332[index];
    u32 i;

    for (i = 0; i < 8; i++, entry += 2) {
        if (random < entry[1]) {
            s32 item = entry[0];

            if (mdlFlagTest(0x990) == 0 && (u32)(item - 0x6D) < 0x13) {
                item = 9;
            }
            return item;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B950);

u32 func_0029BB28(void) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    DatPartyRecord *item = context->selectedRewardRow->unit;
    s32 mode;

    func_0029B950(item, context);
    mode = context->rewardMode;
    if (mode != 0 && mode != 5) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + item->unitId * 17);
        dspStartEntry(0x19);
    }
    memset(context->statGains, 0, sizeof(context->statGains));
    return 1;
}

u32 func_0029BBC0(void) {
    evtFinishMessageWindowAndNotify();
    return 1;
}


s32 mnuSelectEventFlagCode(void) {
    if (mdlFlagTest(0x31)) return 8;
    if (mdlFlagTest(0x25)) return 1;
    if (mdlFlagTest(0x1C)) return 2;
    return mdlFlagTest(0x13) ? 5 : 1;
}

INCLUDE_SDATA(const s32, "game/code_0029AFC0", D_004379B8);

