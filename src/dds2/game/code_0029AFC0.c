#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

extern void func_0026C900(void);

extern void func_0029AA48(s32);

extern void func_0029AC20(s32, s32);
extern void func_0029B950(void *item, void *context);
extern u8 *D_00435E48;
extern void func_0026C918(s32 index, void *value);
extern void *memset(void *destination, s32 value, u32 size);

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

typedef struct MenuProgressItem {
    u8 pad00[4];
    u16 index;
    u8 pad06[0xE];
    u16 value;
} MenuProgressItem;

typedef struct MenuProgressChange {
    MenuProgressItem *item;
    s32 gain;
} MenuProgressChange;

typedef struct MenuProgressContext {
    u8 pad00[0x9C];
    MenuProgressChange *change;
    u8 padA0[0xB650];
    s32 crossedSteps;
} MenuProgressContext;

extern u8 brsGetLevelStepCrossedBy(s32, s32);
extern u8 brsGetLevelStepForValue(s32);
extern s32 func_0035C860(char *, const char *, ...);
extern void dspSetActive(s32);
extern s32 dspStartEntry(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern char D_004379B0[];
extern char D_004379B8[];

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

s32 func_0029B6A0(void) {
    char text[0x20];
    MenuProgressContext *context;
    MenuProgressItem *item;
    s32 gain;

    context = (MenuProgressContext *)kwlnTaskGetUserValue();
    item = context->change->item;
    gain = context->change->gain;
    context->crossedSteps = brsGetLevelStepCrossedBy(item->value - gain, gain);
    if (context->crossedSteps != 0) {
        func_0035C860(text, D_004379B8, D_00435E48 + item->index * 17);
        func_0026C918(0, text);
        func_0035C860(text, D_004379B0, brsGetLevelStepForValue(item->value));
        func_0026C918(1, text);
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
    u8 *context = (u8 *)kwlnTaskGetUserValue();
    u8 *item = **(u8 ***)(context + 0x9C);
    s32 mode;

    func_0029B950(item, context);
    mode = *(s32 *)(context + 0xB6F4);
    if (mode != 0 && mode != 5) {
        func_0026C918(0, D_00435E48 + *(u16 *)(item + 4) * 17);
        dspStartEntry(0x19);
    }
    memset(context + 0x3F4, 0, 0x14);
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

