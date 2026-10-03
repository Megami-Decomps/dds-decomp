#include "common.h"

extern s32 evtAdvanceSlotFlags(void);

extern s64 evtGetMessageWindowControlState(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 kwlnFadeIsActive(void);

extern s32 kwlnTaskGetUserValue();
extern void func_002619A8(s32, s32);
extern void func_0025FD78(s32);
extern void func_00297320(s32);
extern void func_00297970(s32);
extern s32 datGameState;

/* Shared save-state item quantities, also used by the camp menu. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

extern s32 D_003CE148[];
extern u8 D_003CE4EC[];
extern void func_00297220(s32, u32);
extern s32 mnuCampAdvanceCounter(s32, s32);

extern void mnuSetPopupEntry(s32, s32);

extern u8 D_003CE498[];
extern s32 D_00435E48;
extern char D_00437840[];
extern s32 D_003C9A20[];
extern void evtCopyEntryStringToActiveWindow();
extern s32 func_0035C860(char *, const char *, ...);
extern void evtClearActiveFlag();
extern void evtSetBoundedDisplayValue();
extern void func_00260020();
extern s32 mnuCampHasEligibleOwnedItems();
extern void func_002B86E8();
extern s32 D_003CE14C[];
extern u8 D_003CE620[];
extern u8 D_003CE400[];
extern s32 mnuTickExtendedCommandPhase();
extern s32 mdlFlagTest();
extern void mdlFlagSet();
extern void dspStartEntry();
extern u8 D_003CE1A8[];
typedef struct EvtSlot {
    u32 threshold;
    s32 flag;
    struct {
        u8 kind;
        s32 id;
    } sub[8];
} EvtSlot;

typedef struct EvtSceneObject {
    u8 pad00[0x18];
    s32 node;
} EvtSceneObject;

typedef struct EvtSceneNode {
    u8 pad00[0x1C];
    s32 entryRecord; /* 0x1C: per-node entry and stage information */
    s32 completionState;
    u8 pad24[8];
    s32 callback; /* 0x2C: scene action callback */
    s32 selectionRecord;
} EvtSceneNode;

typedef struct EvtEntryRecord {
    u8 pad00[0x60];
    s32 stage;
    s32 itemId;
} EvtEntryRecord;

typedef struct EvtSelectionRecord {
    u8 pad00[0x12];
    u16 slot;
} EvtSelectionRecord;


typedef struct EvtStateTableContext {
    u8 pad00[0x5C];
    s32 stateTable;
    u8 pad60[0x1C];
    EvtSceneObject *primaryObject;
    EvtSceneObject *secondaryObject;
    u8 pad84[0xC];
    s32 entryMultiplier;
    s32 dispatchMode;
    u8 pad98[0xE];
    u16 selectedSlot;       /* 0xA6 */
    u8 padA8[8];
    s32 cachedSelection;    /* 0xB0 */
    u8 padB4[4];
    s32 progressTimer;
    u8 padBC[4];
    s32 stateCode;
    u16 stateStep;
    u8 padC6[2];
    s32 unkC8;
    u8 advancedSlots;       /* 0xCC */
    s8 followupMode;        /* 0xCD */
    u8 padCE[0x2BB];
    s8 sceneReady;          /* 0x389: selects the follow-up dispatch */
} EvtStateTableContext;

typedef struct EvtFlagGate {
    s8 threshold;
    u8 pad1;
    u16 cue;
    u32 flag;
} EvtFlagGate;

/* Persistent thresholds and completed-slot cursor in the shared game state. */
typedef struct EvtProgressState {
    u8 pad00[0x1E654];
    s32 total;     /* 0x1E654: accumulated progress against slot thresholds */
    s32 slotIndex; /* 0x1E658: last completed slot */
} EvtProgressState;

extern s32 func_002C5498();
extern u8 D_003CE604[];
extern u16 D_003CE3F8[];
extern void func_00261670();
extern void func_00294930();
extern void func_00298648();
extern void mnuSetPopupEntryFlagged();

extern void mnuSetCommandPhase(s32, u32);

extern void func_0026C900(void);

extern u8 D_003CE4B4[];

extern void func_00297200(s32, u32);

extern void func_00295D38();

extern u8 D_003CE4D0[];

extern u8 D_003CE508[];

extern s32 func_00261B98(s32);

extern u8 D_003CE690[];

extern void mnuStorePendingMenuCommandValue(s32, u32);

extern void func_002958B0();


s32 evtIsFadeDispatchIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

void evtInstallStateTable(s32 event) {
    if (((EvtStateTableContext *)event)->dispatchMode == 2) {
        ((EvtStateTableContext *)event)->stateTable = (s32)D_003CE498;
        mnuSetPopupEntry(event + 0x58, (s32)(D_003CE498 + 0x118));
    }
}

/* Mirror the chosen slot into both the active scene record and dispatch state. */
s32 evtInitializeSelectedSlot(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 selectionRecord;
    s32 slot;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((EvtStateTableContext *)context)->primaryObject == 0) {
        func_00260020(context);
    }
    selectionRecord = ((EvtSceneNode *)((EvtStateTableContext *)context)->primaryObject->node)->selectionRecord;
    slot = mnuCampHasEligibleOwnedItems(context);
    ((EvtStateTableContext *)context)->cachedSelection = *(s32 *)(datGameState + 0x3c);
    ((EvtSelectionRecord *)selectionRecord)->slot = slot;
    ((EvtStateTableContext *)context)->selectedSlot = slot;
    return 1;
}

s32 evtAdvancePhaseOne(void) {
    EvtStateTableContext *context = (EvtStateTableContext *)kwlnTaskGetUserValue();
    if (context->stateCode == 1) {
        mnuSetCommandPhase((s32)context, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261F48);

s64 func_00262190(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297320(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 evtSetupDispatchSync(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void evtInstallStateTableB(s32 event) {
    if (((EvtStateTableContext *)event)->dispatchMode == 1) {
        ((EvtStateTableContext *)event)->stateTable = (s32)D_003CE4B4;
        mnuSetPopupEntry(event + 0x58, (s32)(D_003CE4B4 + 0xfc));
    }
}

s32 func_00262270(void) {
    s32 context = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)context)->stateCode == 1) {
        func_00261670(context);
    }
    return 1;
}

s32 evtSelectStateAction(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 sceneNode;
    if (((EvtStateTableContext *)context)->stateCode == 5) {
        mnuSetCommandPhase(context, 3);
    } else if (((EvtStateTableContext *)context)->stateCode == 7) {
        mnuSetCommandPhase(context, 9);
        sceneNode = ((EvtStateTableContext *)context)->secondaryObject->node;
        ((EvtSceneNode *)sceneNode)->callback = (s32)func_00295D38;
        func_00297200(sceneNode, 0xa);
    }
    ((EvtStateTableContext *)context)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262330);

s64 func_00262598(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 evtSetupDispatchSyncB(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void evtInstallStateTableC(s32 event) {
    if (((EvtStateTableContext *)event)->dispatchMode == 1) {
        ((EvtStateTableContext *)event)->stateTable = (s32)D_003CE4D0;
        mnuSetPopupEntry(event + 0x58, (s32)(D_003CE4D0 + 0xe0));
    }
}

s32 func_00262678(void) {
    s32 context = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)context)->stateCode == 1) {
        func_002619A8(context, 1);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 sceneNode;
    if (((EvtStateTableContext *)context)->stateCode == 5) {
        mnuSetCommandPhase(context, 3);
    } else if (((EvtStateTableContext *)context)->stateCode == 7) {
        mnuSetCommandPhase(context, 9);
        sceneNode = ((EvtStateTableContext *)context)->secondaryObject->node;
        ((EvtSceneNode *)sceneNode)->callback = (s32)func_00295D38;
        func_00297200(sceneNode, 0xa);
    }
    ((EvtStateTableContext *)context)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262740);

s64 func_002629A8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00262A00(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void func_00262A48(s32 event) {
    if (((EvtStateTableContext *)event)->dispatchMode == 1) {
        ((EvtStateTableContext *)event)->stateTable = (s32)D_003CE4EC;
        mnuSetPopupEntry(event + 0x58, (s32)(D_003CE4EC + 0xc4));
    }
}

s32 func_00262A88(void) {
    s32 context = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)context)->stateCode == 1) {
        func_002619A8(context, 3);
    }
    return 1;
}

s32 mnuResetCommandStepAndSelectPhase(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 sceneNode;
    if (((EvtStateTableContext *)context)->stateCode == 5) {
        mnuSetCommandPhase(context, 3);
    } else if (((EvtStateTableContext *)context)->stateCode == 7) {
        mnuSetCommandPhase(context, 9);
        sceneNode = ((EvtStateTableContext *)context)->secondaryObject->node;
        ((EvtSceneNode *)sceneNode)->callback = (s32)func_00295D38;
        func_00297200(sceneNode, 0xa);
    }
    ((EvtStateTableContext *)context)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262B50);

s64 func_00262DB8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00262E10(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void evtInstallStateTableD(s32 event) {
    if (((EvtStateTableContext *)event)->dispatchMode == 2) {
        ((EvtStateTableContext *)event)->stateTable = (s32)D_003CE508;
        mnuSetPopupEntry(event + 0x58, (s32)(D_003CE508 + 0xa8));
    }
}

s32 evtEnableStateFlag(void) {
    s32 context = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)context)->stateCode == 1 && !func_00261B98(context)) {
        ((EvtStateTableContext *)context)->dispatchMode = 2;
    }
    return 1;
}

s32 evtEnterProgressCommandPhase(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 sceneNode;
    if (((EvtStateTableContext *)context)->stateCode == 5) {
        mnuSetCommandPhase(context, 3);
    } else if (((EvtStateTableContext *)context)->stateCode == 7) {
        mnuSetCommandPhase(context, 9);
        sceneNode = ((EvtStateTableContext *)context)->secondaryObject->node;
        ((EvtSceneNode *)sceneNode)->callback = (s32)func_00295D38;
        func_00297200(sceneNode, 0xa);
    }
    ((EvtStateTableContext *)context)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262F78);

s64 func_00263180(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002631D8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

/* Return a slot's remaining threshold, or -1 for an unavailable slot. */
s32 evtGetRemainingSlotThreshold(u32 slotIndex) {
    s32 threshold;
    s32 remaining;
    if (slotIndex >= 8) {
        return -1;
    }
    threshold = D_003CE148[slotIndex * 3];
    if (threshold == 0) {
        return -1;
    }
    remaining = threshold - ((EvtProgressState *)datGameState)->total;
    if (remaining > 0) {
        return remaining;
    }
    return 0;
}

s32 evtShowResultText(void) {
    char text[0x40];
    evtCopyEntryStringToActiveWindow(0, D_00435E48 + 0x11);
    func_0035C860(text, D_00437840, ((EvtProgressState *)datGameState)->total);
    evtCopyEntryStringToActiveWindow(1, text);
    evtCopyEntryStringToActiveWindow(2, D_003C9A20[((EvtProgressState *)datGameState)->slotIndex]);
    func_0035C860(text, D_00437840, evtGetRemainingSlotThreshold(((EvtProgressState *)datGameState)->slotIndex + 1));
    evtCopyEntryStringToActiveWindow(3, text);
    if (evtGetRemainingSlotThreshold(((EvtProgressState *)datGameState)->slotIndex + 1) >= 0) {
        dspStartEntry(((EvtProgressState *)datGameState)->slotIndex + 0x1a);
    } else {
        dspStartEntry(0x21);
    }
    return 1;
}

u32 func_00263378(void) {
    return 1;
}

s64 evtOpenProgressResultPopupWhenIdle(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *result = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, result, 0, callback);
    if (state == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(result, D_003CE498);
        }
        return 0;
    }
    return state;
}

s64 func_002633F0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297320(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00263448(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

s64 func_002635E8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *result = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, result, 0, callback);
    if (state == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(result, D_003CE498);
        }
        return 0;
    }
    return state;
}

s64 func_00263658(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297320(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002636B0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

u32 evtResetStateProgressTimer(void) {
    EvtStateTableContext *context;

    context = (EvtStateTableContext *)kwlnTaskGetUserValue();
    context->progressTimer = 0;
    mnuSelectLastListNode(context->primaryObject->node);
    return 1;
}

/* Delay the next state table until dispatch is idle and 20 progress ticks elapse. */
s64 evtQueryStateProgress(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                s32 elapsedTicks = ((EvtStateTableContext *)context)->progressTimer;
                if ((f32)elapsedTicks < 20.0f) {
                    ((EvtStateTableContext *)context)->progressTimer = elapsedTicks + 1;
                } else {
                    mnuSetPopupEntry(window, D_003CE690);
                }
            }
        }
        return 0;
    }
    return state;
}

s64 evtDispatchProgressCallback(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00294930(context, ((EvtStateTableContext *)context)->progressTimer);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 evtSetupDispatchSyncE(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 evtApplyBaseRateProgressStep(void) {
    EvtStateTableContext *context = (EvtStateTableContext *)kwlnTaskGetUserValue();
    context->entryMultiplier = 1;
    mnuCampAdvanceCounter(-1, (s32)context);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 sceneNode;
    if (((EvtStateTableContext *)context)->stateCode == 0xa) {
        ((EvtStateTableContext *)context)->stateStep = 0xa;
        mnuSetCommandPhase(context, 6);
        sceneNode = ((EvtStateTableContext *)context)->secondaryObject->node;
        ((EvtSceneNode *)sceneNode)->callback = (s32)func_002958B0;
        mnuStorePendingMenuCommandValue(sceneNode, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263920);

s64 func_00263B98(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00298648(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00263BF0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263C38);

s32 evtUpdateSlotItemCompletionState(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 stage = ((EvtEntryRecord *)((EvtSceneNode *)((EvtStateTableContext *)context)->primaryObject->node)->entryRecord)->stage + 1;
    s32 id = ((EvtEntryRecord *)((EvtSceneNode *)((EvtStateTableContext *)context)->secondaryObject->node)->entryRecord)->itemId;
    if (stage == 4) {
        if (((SaveItemCounts *)datGameState)->counts[id] == 0) {
            func_002B86E8(((EvtStateTableContext *)context)->secondaryObject->node);
        }
        if (((EvtSceneNode *)((EvtStateTableContext *)context)->secondaryObject->node)->completionState == 0) {
            ((EvtStateTableContext *)context)->dispatchMode = 2;
        }
    }
    return 1;
}

s32 evtPollStageSelectionAndAdvance(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 sceneNode;
    switch (mnuTickExtendedCommandPhase(context)) {
    case 6:
        return 1;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 0;
    case 8:
        mnuSetCommandPhase(context, 6);
        sceneNode = ((EvtStateTableContext *)context)->secondaryObject->node;
        ((EvtSceneNode *)sceneNode)->callback = (s32)func_002958B0;
        mnuStorePendingMenuCommandValue(sceneNode, 0);
        ((EvtStateTableContext *)context)->stateStep = 0xa;
        return 0;
    default:
        return 0;
    }
}

void evtMarkSceneFollowupReadyAndQueueAction(void) {
    s32 context = kwlnTaskGetUserValue();
    mnuSetCommandPhase(context, 8);
    func_00297220(((EvtStateTableContext *)context)->secondaryObject->node, 10);
    ((EvtStateTableContext *)context)->sceneReady = 1;
}

void evtAccumulateEligibleStageMultiplierValue(EvtStateTableContext *context) {
    s32 *entry = &((EvtEntryRecord *)((EvtSceneNode *)context->secondaryObject->node)->entryRecord)->stage;
    if (func_002C5498(entry[1])) {
        *(s32 *)(datGameState + 0xa50) += entry[0] * context->entryMultiplier;
    }
}

s32 evtIsAllowedId(s32 id) {
    u32 i;
    for (i = 0; i < 3; i++) {
        if (D_003CE3F8[i] == id) {
            return 1;
        }
    }
    return 0;
}

s32 func_00263F50(s32 delta) {
    if (delta == 0) {
        delta = 1;
    }
    ((EvtProgressState *)datGameState)->total += delta;
    if ((u32)((EvtProgressState *)datGameState)->total > 999999U) {
        ((EvtProgressState *)datGameState)->total = 999999;
    }
    return ((EvtProgressState *)datGameState)->total;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263FB0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264120);

s64 evtDispatchSceneReadyFollowup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    if (((EvtStateTableContext *)context)->sceneReady == 1) {
        func_00297970(context);
    } else {
        func_00298648(context);
    }
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002642B8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 evtPlayDispatchModeCue(void) {
    s32 context = kwlnTaskGetUserValue();
    dspSetActive(1);
    switch (((EvtStateTableContext *)context)->dispatchMode) {
    case 1:
        dspStartEntry(5);
        break;
    case 2:
        dspStartEntry(6);
        break;
    }
    return 1;
}

s32 evtApplyDispatchModeState(void) {
    s32 context = kwlnTaskGetUserValue();
    switch (((EvtStateTableContext *)context)->dispatchMode) {
    case 1:
        mnuSetCommandPhase(context, 6);
        break;
    case 2:
        if (((EvtStateTableContext *)context)->stateTable != (s32)D_003CE498) {
            mnuSetCommandPhase(context, 5);
        }
        break;
    }
    ((EvtStateTableContext *)context)->dispatchMode = 0;
    return 1;
}

s64 evtSetPopupEntryWhenMessageWindowIdle(u64 callback) {
    s32 context;
    s64 state;
    s32 *window;

    context = kwlnTaskGetUserValue();
    window = (s32 *)(context + 0x58);
    state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if ((*window == 0) && (state = evtGetMessageWindowControlState(), state == 0)) {
            mnuSetPopupEntryFlagged(window, ((EvtStateTableContext *)context)->stateTable);
        }
        state = 0;
    }
    return state;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264480);

s64 func_002646C8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264710);

u32 func_00264848(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264850);

s64 func_00264AB8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00264B10(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

/* Mark each newly completed slot and advance the persistent slot index. */
s32 evtAdvanceSlotFlags(void) {
    s32 i = 0;
    while (evtGetRemainingSlotThreshold(((EvtProgressState *)datGameState)->slotIndex + i + 1) == 0) {
        mdlFlagSet(D_003CE14C[(((EvtProgressState *)datGameState)->slotIndex + i) * 3 + 3]);
        i++;
    }
    ((EvtProgressState *)datGameState)->slotIndex += i;
    return i;
}

u32 evtUpdateSlotAdvanceCount(void) {
    u8 advancedSlots;
    s32 context;

    context = kwlnTaskGetUserValue();
    advancedSlots = evtAdvanceSlotFlags();
    ((EvtStateTableContext *)context)->advancedSlots = advancedSlots;
    if ((((EvtStateTableContext *)context)->unkC8 == 0) && (((EvtStateTableContext *)context)->followupMode == '\x01')) {
        dspStartEntry(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

s64 evtOpenSlotAdvancePopupWhenIdle(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                mnuSetPopupEntryFlagged(window, D_003CE604);
            }
        }
        return 0;
    }
    return state;
}

s64 func_00264CE0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00264D38(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

/* Raise a gate's flag and play its cue only on the first threshold crossing. */
s32 evtTriggerProgressFlagGate(s32 unusedContext) {
    EvtFlagGate *entry = (EvtFlagGate *)D_003CE400;
    u32 i;
    for (i = 0; i < 1; i++, entry++) {
        if ((u32)(((EvtProgressState *)datGameState)->slotIndex + 1) >= (u32)entry->threshold) {
            u32 flag = entry->flag;
            if (mdlFlagTest(flag) == 0) {
                mdlFlagSet(flag);
                dspStartEntry(entry->cue);
                return 1;
            }
        }
    }
    return 0;
}

s32 evtShowSlotText(void) {
    char text[0x40];
    if (*(s8 *)(kwlnTaskGetUserValue() + 0xcc) > 0) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + 0x11);
        func_0035C860(text, D_00437840, D_003CE148[((EvtProgressState *)datGameState)->slotIndex * 3]);
        evtCopyEntryStringToActiveWindow(1, text);
        evtCopyEntryStringToActiveWindow(2, D_003C9A20[((EvtProgressState *)datGameState)->slotIndex]);
        if (evtGetRemainingSlotThreshold(((EvtProgressState *)datGameState)->slotIndex + 1) >= 0) {
            dspStartEntry(0x24);
        } else {
            dspStartEntry(0x25);
        }
    }
    return 1;
}

u32 func_00264EF8(void) {
    return 1;
}

s64 evtTriggerProgressGateThenOpenPopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                if (evtTriggerProgressFlagGate(context) == 0) {
                    mnuSetPopupEntryFlagged(window, D_003CE620);
                }
            }
        }
        return 0;
    }
    return state;
}

s64 func_00264F98(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00264FF0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00265038);

s32 evtIsLastSlot(s32 slotIndex) {
    s32 activeSlots = 0;
    u32 i;
    for (i = 0; i < 8; i++) {
        if (((EvtSlot *)D_003CE1A8)[i].flag != 0) {
            activeSlots++;
        }
    }
    return slotIndex + 1 == activeSlots;
}

INCLUDE_RODATA(const s32, "game/code_00261E10", D_00424D08);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437840);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437848);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437850);

