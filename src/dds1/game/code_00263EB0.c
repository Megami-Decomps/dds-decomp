#include "common.h"

extern s32 kwlnTaskGetUserValue();

extern void func_0024DD78(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

#define MENU_SUM_MINIMUM 99
#define MENU_SUM_COUNT 5

typedef struct MenuSumBytes {
    u8 pad00[0x16];
    s8 values[MENU_SUM_COUNT];
} MenuSumBytes;

typedef struct MenuSumTable {
    u8 pad00[0x3D0];
    s32 values[MENU_SUM_COUNT];
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

void func_002641E0(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_00264238(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, input);
}

u32 func_00264280(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSetPanelGroupSelection(*(u32 *)(context + 0xd10), 0xffffffffffffffff);
    dspStartEntry(0x16);
    func_0024DAE8(0);
    evtCaptureMessageWindowSoundMode(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002642D0);

void func_00264498(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_002644F0(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, input);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264538);

u32 func_00264608(void) {
    return 1;
}

extern s32 evtStageTestUpdateCamera(void);
extern s64 evtGetMessageWindowControlState(void);
extern void mnuSetPopupEntryFlagged(s32 *, char *);
extern char D_0036D478[];

/* On an idle panel, apply the extra fallback only when the auxiliary check also fails. */
s64 mnuRunPanelWithIdleFallback(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    s32 *panelState = (s32 *)(context + 0x54);
    s64 result;

    evtStageTestUpdateCamera();
    result = func_00285670(context + 8, panelState, 0, request);
    if (result == 0) {
        if ((*panelState == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(panelState, D_0036D478);
        }
        result = 0;
    }
    return result;
}

void func_002646A0(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_002646F8(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, input);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264740);

u32 func_002647B0(void) {
    func_0024DBB0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002647D0);

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC558);

