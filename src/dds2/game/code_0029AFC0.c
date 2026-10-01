#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

extern void func_0026C900(void);

extern void func_0029AA48(s32);

extern void func_0029AC20(s32, s32);

extern s32 evtStageTestUpdateCamera(void);

extern s64 evtGetMessageWindowControlState(void);

extern void mnuSetPopupEntryFlagged(s32 *, char *);

extern char D_003D64C8[];

typedef struct MenuSumBytes {
    u8 pad00[0x16];
    s8 values[MENU_SUM_COUNT];
} MenuSumBytes;

typedef struct MenuSumTable {
    u8 pad00[0x3F4];
    s32 values[MENU_SUM_COUNT];
} MenuSumTable;

/* All five signed-byte plus table-word totals must meet the minimum. */
s32 mnuCheckTableSums(MenuSumBytes *bytes, MenuSumTable *table) {
    s32 *tableValues = table->values;
    s8 *byteValues = bytes->values;
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

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B008);

s64 mnuDrawItemPanelDuringRequest(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0029AA48(context);
    func_0029AC20(context, 1);
    return menuSetHandler(context, 1, request);
}

s64 func_0029B378(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

u32 mnuResetGroupSelectionAndStartMessage(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSetPanelGroupSelection(*(u32 *)(context + 0xad34), 0xffffffffffffffff);
    dspStartEntry(0x17);
    evtSetMessageWindowOptionWhenOpen(0);
    evtCaptureMessageWindowSoundMode(0xa3);
    return 1;
}

u32 func_0029B410(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B418);

s64 func_0029B600(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0029AA48(context);
    func_0029AC20(context, 1);
    return menuSetHandler(context, 1, request);
}

s64 func_0029B658(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B6A0);

u32 func_0029B778(void) {
    return 1;
}

/* On an idle panel, apply the extra fallback only when the auxiliary check also fails. */
s64 mnuRunPanelWithIdleFallback(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    s32 *panelState = (s32 *)(context + 0x54);
    s64 result;

    evtStageTestUpdateCamera();
    result = func_002C4038(context + 8, panelState, 0, request);
    if (result == 0) {
        if ((*panelState == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(panelState, D_003D64C8);
        }
        result = 0;
    }
    return result;
}

s64 mnuRunItemPanelWithInactiveBackdrop(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0029AA48(context);
    func_0029AC20(context, 0);
    return menuSetHandler(context, 1, request);
}

s64 func_0029B868(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B8B0);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029B950);

INCLUDE_ASM(const s32, "game/code_0029AFC0", func_0029BB28);

u32 func_0029BBC0(void) {
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern s32 mdlFlagTest(s32);

s32 mnuSelectEventFlagCode(void) {
    if (mdlFlagTest(0x31)) return 8;
    if (mdlFlagTest(0x25)) return 1;
    if (mdlFlagTest(0x1C)) return 2;
    return mdlFlagTest(0x13) ? 5 : 1;
}

INCLUDE_SDATA(const s32, "game/code_0029AFC0", D_004379B8);

