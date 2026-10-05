#include "mnu.h"

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

typedef struct MenuSumBytes {
    u8 pad00[4];
    u16 itemId;
    u8 pad06[0xE];
    u16 level;
    s8 values[MENU_SUM_COUNT];
} __attribute__((aligned(4))) MenuSumBytes;

typedef struct BrsLevelStepState {
    MenuSumBytes *entry;
    s32 increment;
} BrsLevelStepState;

typedef struct MenuSumTable {
    u8 pad00[0x54];
    s32 panelState; /* 0x54 */
    u8 pad58[0x40];
    BrsLevelStepState *state;
    u32 previewUnit[0x69]; /* 0x9C */
    u8 pad240[0x188];
    s32 assignedPoints; /* 0x3C8 */
    s32 availablePoints; /* 0x3CC */
    s32 values[MENU_SUM_COUNT];
    u8 pad3E4[0x92C];
    s32 panelGroup; /* 0xD10 */
    u8 padD14[0x864];
    u32 selectionInitialized; /* 0x1578 */
    u8 pad157C[4];
    u32 commitComplete; /* 0x1580 */
    u32 unk1584;
    s32 rewardMode;
    s32 rewardIndex;
} MenuSumTable;

/* All five signed-byte plus table-word totals must meet the minimum. */
s32 mnuCheckTableSums(s32 bytes, s32 table) {
    s32 *tableValues = ((MenuSumTable *)table)->values;
    s8 *byteValues = ((MenuSumBytes *)bytes)->values;
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
extern void mnuPlayInputSound(s32, s32, s32);
extern char D_0036D440[];

s32 func_00263EF8(u64 request) {
    MenuSumTable *work;
    MenuSumBytes *entry;
    s32 inputFlags;
    s32 selection;
    s32 changed = 0;
    s32 result;

    work = (MenuSumTable *)kwlnTaskGetUserValue();
    inputFlags = mnuMapPadMaskToFlags(0xF3);
    entry = work->state->entry;
    evtStageTestUpdateCamera();
    result = func_00285670((s32)work + 8, &work->panelState, 0, request);
    if (result != 0) {
        return result;
    }
    if (work->panelState != 0 || evtGetMessageWindowControlState() != 0) {
        return 0;
    }

    if (work->selectionInitialized == 0) {
        mnuSetPanelGroupSelection(work->panelGroup, 0);
        work->selectionInitialized = 1;
    }

    if (work->availablePoints == work->assignedPoints ||
        mnuCheckTableSums((s32)entry, (s32)work) != 0) {
        dspStartEntry(0x16);
        evtSetMessageWindowOptionWhenOpen(0);
        evtStoreValueAndCaptureWindowPanelValue(0x1D);
        mnuSetPopupEntryFlagged(&work->panelState, D_0036D440);
    } else {
        selection = mnuGetPanelGroupSelection(work->panelGroup);
        if ((inputFlags & 0x81) != 0) {
            if (entry->values[selection] + work->values[selection] < 99) {
                inputFlags = 1;
                changed = 1;
                work->availablePoints++;
                work->values[selection]++;
            } else {
                inputFlags = 0x8000;
            }
        }
        if ((inputFlags & 0x40) != 0) {
            if (work->availablePoints > 0 && work->values[selection] > 0) {
                changed = 1;
                work->availablePoints--;
                work->values[selection]--;
            } else {
                inputFlags = 0x8000;
            }
        }
        if ((inputFlags & 2) != 0) {
            changed = 1;
            mnuClearItemSelectionSlots(work);
        }
        if (changed != 0) {
            memcpy(entry, work->previewUnit, 0x1A4);
            mnuRefreshPartyUnitVitalsPanels((u32)entry, (u32)work);
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
        mnuSetPanelGroupSelection(work->panelGroup, selection);
    }
    mnuPlayInputSound(0, inputFlags, 0);
    return 0;
}

extern void mnuDrawItemPanelBackdrop(s32);
extern void func_00263B78(s32, s32);

s32 mnuDrawItemPanelDuringRequest(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, request);
}

s32 func_00264238(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, input);
}

u32 mnuBeginPanelEntryAndCaptureSoundMode(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSetPanelGroupSelection(*(u32 *)(context + 0xd10), 0xffffffffffffffff);
    dspStartEntry(0x16);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

extern s8 evtGetCapturedWindowPanelValue(void);
extern s32 btlAddBaseStats(u8 *, u8 *);
extern void mnuClearPanelGroupSelection(s32);
extern void mnuBindPresentMenuEntry(void *, s32 *);
extern char D_0036D45C[];

s32 func_002642D0(u64 request) {
    MenuSumTable *work;
    MenuSumBytes *entry;
    s32 result;

    work = (MenuSumTable *)kwlnTaskGetUserValue();
    entry = work->state->entry;
    evtStageTestUpdateCamera();
    result = func_00285670((s32)work + 8, &work->panelState, 0, request);
    if (result != 0) {
        return result;
    }
    if (work->panelState == 0 && evtGetMessageWindowControlState() == 0) {
        s8 capturedValue = evtGetCapturedWindowPanelValue();
        s32 *stats = work->values;

        if (capturedValue == 0) {
            btlAddBaseStats((u8 *)stats, (u8 *)work->state->entry);
            mnuClearItemSelectionSlots(work);
            mnuClearPanelGroupSelection(work->panelGroup);
            mnuSetPopupEntryFlagged(&work->panelState, D_0036D45C);
            work->commitComplete = 1;
        } else {
            mnuClearItemSelectionSlots(work);
            mnuSetPanelGroupSelection(work->panelGroup, 0);
            memcpy(entry, work->previewUnit, 0x1A4);
            mnuRefreshPartyUnitVitalsPanels((u32)entry, (u32)work);
            mnuBindPresentMenuEntry((u8 *)work + 8, &work->panelState);
        }
    }
    return 0;
}

s32 func_00264498(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, request);
}

s32 func_002644F0(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, input);
}

u32 func_00264538(void) {
    char text[0x20];
    MenuSumTable *work = (MenuSumTable *)kwlnTaskGetUserValue();
    BrsLevelStepState *state = work->state;
    MenuSumBytes *entry = state->entry;
    u32 step = brsGetLevelStepCrossedBy(entry->level - state->increment, state->increment);

    work->unk1584 = step;
    if (step != 0) {
        func_003014F0(text, D_003BC558, D_003BAA70 + entry->itemId * 17);
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
s32 mnuRunPanelWithIdleFallback(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    s32 *panelState = (s32 *)(context + 0x54);
    s32 result;

    evtStageTestUpdateCamera();
    result = func_00285670(context + 8, panelState, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panelState == 0 && evtGetMessageWindowControlState() == 0) {
        mnuSetPopupEntryFlagged(panelState, D_0036D478);
    }
    return 0;
}

s32 mnuRunItemPanelWithInactiveBackdrop(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel(context, 1, request);
}

s32 func_002646F8(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, input);
}


u32 func_00264740(void) {
    u8 *context = (u8 *)kwlnTaskGetUserValue();
    u8 *item = *(u8 **)(*(u8 **)(context + 0x98));

    if (*(s32 *)(context + 0x1588) != 0) {
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + *(u16 *)(item + 4) * 17);
        dspStartEntry(0x18);
    }
    memset(context + 0x3D0, 0, 0x14);
    return 1;
}

u32 func_002647B0(void) {
    evtFinishMessageWindowAndNotify();
    return 1;
}

s32 func_002647D0(MenuSumTable *work) {
    MenuSumBytes *entry = work->state->entry;
    s32 result;

    switch (work->rewardMode) {
    case 4: {
        s32 rewardIndex = work->rewardIndex;

        work->values[rewardIndex]++;
        evtCopyEntryStringToActiveWindow(1, D_0036D3A8[rewardIndex]);
    }
    /* fall through */
    case 1:
    case 2:
    case 3:
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + entry->itemId * 17);
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

