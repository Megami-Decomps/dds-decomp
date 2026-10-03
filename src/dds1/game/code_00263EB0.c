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
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern char D_003BC550[];
extern char D_003BC558[];

typedef struct MenuSumBytes {
    u8 pad00[4];
    u16 itemId;
    u8 pad06[0xE];
    u16 level;
    s8 values[MENU_SUM_COUNT];
} MenuSumBytes;

typedef struct BrsLevelStepState {
    MenuSumBytes *entry;
    s32 increment;
} BrsLevelStepState;

typedef struct MenuSumTable {
    u8 pad00[0x98];
    BrsLevelStepState *state;
    u8 pad9C[0x334];
    s32 values[MENU_SUM_COUNT];
    u8 pad3E4[0x11A0];
    u32 unk1584;
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

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00263EF8);

extern void mnuDrawItemPanelBackdrop(s32);
extern void func_00263B78(s32, s32);

s64 mnuDrawItemPanelDuringRequest(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, request);
}

s64 func_00264238(s32 input) {
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

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002642D0);

s64 func_00264498(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, request);
}

s64 func_002644F0(s32 input) {
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
s64 mnuRunPanelWithIdleFallback(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    s32 *panelState = (s32 *)(context + 0x54);
    s64 result;

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

s64 mnuRunItemPanelWithInactiveBackdrop(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel(context, 1, request);
}

s64 func_002646F8(s32 input) {
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

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002647D0);

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC558);

