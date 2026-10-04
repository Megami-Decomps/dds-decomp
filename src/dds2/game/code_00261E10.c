#include "common.h"

/* The dispatcher passes its last argument to entry callbacks as opaque data,
 * not as a function address. Modes select polling, primary and secondary actions. */
#define EVT_DISPATCH_OPERATION_POLL 0
#define EVT_DISPATCH_OPERATION_PRIMARY 1
#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define EVT_PROGRESS_SLOT_COUNT 8
#define EVT_PROGRESS_RECORD_WORDS 3
#define EVT_ALLOWED_ITEM_COUNT 3
#define EVT_PROGRESS_UNSIGNED_LIMIT 999999U
#define EVT_PROGRESS_FLAG_GATE_COUNT 1

extern s32 evtAdvanceSlotFlags(void);

extern s32 evtGetMessageWindowControlState(void);

extern s32 func_002C4038(s32, s32 *, u64, u64);

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
    u8 pad00[0xC];
    u8 dispatchWork[0x4C];
    s32 dispatchState;
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
    s8 rewardMode;
    u8 padCF;
    s8 rewardRow;
    s8 remainingRewards;
    s8 rewardIndex;
    s8 announceNextReward;
    s8 grantPendingReward;
    s8 rewardDelay;
    u8 padD6[2];
    s32 rewardKind;
    s32 rewardValue;
    u8 padE0[0x2A9];
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

void evtInstallStateTable(s32 stateAddress) {
    if (((EvtStateTableContext *)stateAddress)->dispatchMode == 2) {
        ((EvtStateTableContext *)stateAddress)->stateTable = (s32)D_003CE498;
        mnuSetPopupEntry(stateAddress + 0x58, (s32)(D_003CE498 + 0x118));
    }
}

/* Mirror the chosen slot into both the active scene record and dispatch state. */
s32 evtInitializeSelectedSlot(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 selectionRecordAddress;
    s32 selectedSlot;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((EvtStateTableContext *)stateAddress)->primaryObject == 0) {
        func_00260020(stateAddress);
    }
    selectionRecordAddress = ((EvtSceneNode *)((EvtStateTableContext *)stateAddress)->primaryObject->node)->selectionRecord;
    selectedSlot = mnuCampHasEligibleOwnedItems(stateAddress);
    ((EvtStateTableContext *)stateAddress)->cachedSelection = *(s32 *)(datGameState + 0x3c);
    ((EvtSelectionRecord *)selectionRecordAddress)->slot = selectedSlot;
    ((EvtStateTableContext *)stateAddress)->selectedSlot = selectedSlot;
    return 1;
}

s32 evtAdvancePhaseOne(void) {
    EvtStateTableContext *state = (EvtStateTableContext *)kwlnTaskGetUserValue();
    if (state->stateCode == 1) {
        mnuSetCommandPhase((s32)state, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261F48);

s32 func_00262190(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297320(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSync(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void evtInstallStateTableB(s32 stateAddress) {
    if (((EvtStateTableContext *)stateAddress)->dispatchMode == 1) {
        ((EvtStateTableContext *)stateAddress)->stateTable = (s32)D_003CE4B4;
        mnuSetPopupEntry(stateAddress + 0x58, (s32)(D_003CE4B4 + 0xfc));
    }
}

s32 func_00262270(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)stateAddress)->stateCode == 1) {
        func_00261670(stateAddress);
    }
    return 1;
}

s32 evtSelectStateAction(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 linkedNodeAddress;
    if (((EvtStateTableContext *)stateAddress)->stateCode == 5) {
        mnuSetCommandPhase(stateAddress, 3);
    } else if (((EvtStateTableContext *)stateAddress)->stateCode == 7) {
        mnuSetCommandPhase(stateAddress, 9);
        linkedNodeAddress = ((EvtStateTableContext *)stateAddress)->secondaryObject->node;
        ((EvtSceneNode *)linkedNodeAddress)->callback = (s32)func_00295D38;
        func_00297200(linkedNodeAddress, 0xa);
    }
    ((EvtStateTableContext *)stateAddress)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262330);

s32 func_00262598(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncB(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void evtInstallStateTableC(s32 stateAddress) {
    if (((EvtStateTableContext *)stateAddress)->dispatchMode == 1) {
        ((EvtStateTableContext *)stateAddress)->stateTable = (s32)D_003CE4D0;
        mnuSetPopupEntry(stateAddress + 0x58, (s32)(D_003CE4D0 + 0xe0));
    }
}

s32 func_00262678(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)stateAddress)->stateCode == 1) {
        func_002619A8(stateAddress, 1);
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

s32 func_002629A8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00262A00(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void func_00262A48(s32 stateAddress) {
    if (((EvtStateTableContext *)stateAddress)->dispatchMode == 1) {
        ((EvtStateTableContext *)stateAddress)->stateTable = (s32)D_003CE4EC;
        mnuSetPopupEntry(stateAddress + 0x58, (s32)(D_003CE4EC + 0xc4));
    }
}

s32 func_00262A88(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)stateAddress)->stateCode == 1) {
        func_002619A8(stateAddress, 3);
    }
    return 1;
}

s32 mnuResetCommandStepAndSelectPhase(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 linkedNodeAddress;
    if (((EvtStateTableContext *)stateAddress)->stateCode == 5) {
        mnuSetCommandPhase(stateAddress, 3);
    } else if (((EvtStateTableContext *)stateAddress)->stateCode == 7) {
        mnuSetCommandPhase(stateAddress, 9);
        linkedNodeAddress = ((EvtStateTableContext *)stateAddress)->secondaryObject->node;
        ((EvtSceneNode *)linkedNodeAddress)->callback = (s32)func_00295D38;
        func_00297200(linkedNodeAddress, 0xa);
    }
    ((EvtStateTableContext *)stateAddress)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262B50);

s32 func_00262DB8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00262E10(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void evtInstallStateTableD(s32 stateAddress) {
    if (((EvtStateTableContext *)stateAddress)->dispatchMode == 2) {
        ((EvtStateTableContext *)stateAddress)->stateTable = (s32)D_003CE508;
        mnuSetPopupEntry(stateAddress + 0x58, (s32)(D_003CE508 + 0xa8));
    }
}

s32 evtEnableStateFlag(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((EvtStateTableContext *)stateAddress)->stateCode == 1 && !func_00261B98(stateAddress)) {
        ((EvtStateTableContext *)stateAddress)->dispatchMode = 2;
    }
    return 1;
}

s32 evtEnterProgressCommandPhase(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 linkedNodeAddress;
    if (((EvtStateTableContext *)stateAddress)->stateCode == 5) {
        mnuSetCommandPhase(stateAddress, 3);
    } else if (((EvtStateTableContext *)stateAddress)->stateCode == 7) {
        mnuSetCommandPhase(stateAddress, 9);
        linkedNodeAddress = ((EvtStateTableContext *)stateAddress)->secondaryObject->node;
        ((EvtSceneNode *)linkedNodeAddress)->callback = (s32)func_00295D38;
        func_00297200(linkedNodeAddress, 0xa);
    }
    ((EvtStateTableContext *)stateAddress)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262F78);

s32 func_00263180(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_002631D8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

/* Return a slot's remaining threshold, or -1 for an unavailable slot. */
s32 evtGetRemainingSlotThreshold(u32 slotIndex) {
    s32 threshold;
    s32 remaining;
    if (slotIndex >= EVT_PROGRESS_SLOT_COUNT) {
        return -1;
    }
    threshold = D_003CE148[slotIndex * EVT_PROGRESS_RECORD_WORDS];
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
    char formattedText[0x40];
    evtCopyEntryStringToActiveWindow(0, D_00435E48 + 0x11);
    func_0035C860(formattedText, D_00437840, ((EvtProgressState *)datGameState)->total);
    evtCopyEntryStringToActiveWindow(1, formattedText);
    evtCopyEntryStringToActiveWindow(2, D_003C9A20[((EvtProgressState *)datGameState)->slotIndex]);
    func_0035C860(formattedText, D_00437840, evtGetRemainingSlotThreshold(((EvtProgressState *)datGameState)->slotIndex + 1));
    evtCopyEntryStringToActiveWindow(3, formattedText);
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

s32 evtOpenProgressResultPopupWhenIdle(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = (s32 *)(stateAddress + 0x58);
    s32 dispatchResult = func_002C4038(stateAddress + 0xc, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (dispatchResult == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_002633F0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297320(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00263448(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

s32 func_002635E8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = (s32 *)(stateAddress + 0x58);
    s32 dispatchResult = func_002C4038(stateAddress + 0xc, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (dispatchResult == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00263658(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297320(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_002636B0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

u32 evtResetStateProgressTimer(void) {
    EvtStateTableContext *state;

    state = (EvtStateTableContext *)kwlnTaskGetUserValue();
    state->progressTimer = 0;
    mnuSelectLastListNode(state->primaryObject->node);
    return 1;
}

/* Delay the next state table until dispatch is idle and 20 progress ticks elapse. */
s32 evtQueryStateProgress(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = (s32 *)(stateAddress + 0x58);
    s32 dispatchResult = func_002C4038(stateAddress + 0xc, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                s32 progressTicks = ((EvtStateTableContext *)stateAddress)->progressTimer;
                if ((f32)progressTicks < 20.0f) {
                    ((EvtStateTableContext *)stateAddress)->progressTimer = progressTicks + 1;
                } else {
                    mnuSetPopupEntry(dispatchSlot, D_003CE690);
                }
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtDispatchProgressCallback(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00294930(stateAddress, ((EvtStateTableContext *)stateAddress)->progressTimer);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncE(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

s32 evtApplyBaseRateProgressStep(void) {
    EvtStateTableContext *state = (EvtStateTableContext *)kwlnTaskGetUserValue();
    state->entryMultiplier = 1;
    mnuCampAdvanceCounter(-1, (s32)state);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 linkedNodeAddress;
    if (((EvtStateTableContext *)stateAddress)->stateCode == 0xa) {
        ((EvtStateTableContext *)stateAddress)->stateStep = 0xa;
        mnuSetCommandPhase(stateAddress, 6);
        linkedNodeAddress = ((EvtStateTableContext *)stateAddress)->secondaryObject->node;
        ((EvtSceneNode *)linkedNodeAddress)->callback = (s32)func_002958B0;
        mnuStorePendingMenuCommandValue(linkedNodeAddress, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263920);

s32 func_00263B98(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00298648(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00263BF0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263C38);

s32 evtUpdateSlotItemCompletionState(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 nextStage = ((EvtEntryRecord *)((EvtSceneNode *)((EvtStateTableContext *)stateAddress)->primaryObject->node)->entryRecord)->stage + 1;
    s32 itemId = ((EvtEntryRecord *)((EvtSceneNode *)((EvtStateTableContext *)stateAddress)->secondaryObject->node)->entryRecord)->itemId;
    if (nextStage == 4) {
        if (((SaveItemCounts *)datGameState)->counts[itemId] == 0) {
            func_002B86E8(((EvtStateTableContext *)stateAddress)->secondaryObject->node);
        }
        if (((EvtSceneNode *)((EvtStateTableContext *)stateAddress)->secondaryObject->node)->completionState == 0) {
            ((EvtStateTableContext *)stateAddress)->dispatchMode = 2;
        }
    }
    return 1;
}

s32 evtPollStageSelectionAndAdvance(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 linkedNodeAddress;
    switch (mnuTickExtendedCommandPhase(stateAddress)) {
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
        mnuSetCommandPhase(stateAddress, 6);
        linkedNodeAddress = ((EvtStateTableContext *)stateAddress)->secondaryObject->node;
        ((EvtSceneNode *)linkedNodeAddress)->callback = (s32)func_002958B0;
        mnuStorePendingMenuCommandValue(linkedNodeAddress, 0);
        ((EvtStateTableContext *)stateAddress)->stateStep = 0xa;
        return 0;
    default:
        return 0;
    }
}

/* The stage-selection caller forwards its task to the user-value lookup. */
void evtMarkSceneFollowupReadyAndQueueAction(s32 taskAddress) {
    s32 stateAddress = kwlnTaskGetUserValue(taskAddress);
    mnuSetCommandPhase(stateAddress, 8);
    func_00297220(((EvtStateTableContext *)stateAddress)->secondaryObject->node, 10);
    ((EvtStateTableContext *)stateAddress)->sceneReady = 1;
}

void evtAccumulateEligibleStageMultiplierValue(EvtStateTableContext *state) {
    s32 *entryValues = &((EvtEntryRecord *)((EvtSceneNode *)state->secondaryObject->node)->entryRecord)->stage;
    if (func_002C5498(entryValues[1])) {
        *(s32 *)(datGameState + 0xa50) += entryValues[0] * state->entryMultiplier;
    }
}

s32 evtIsAllowedId(s32 itemId) {
    u32 allowedItemIndex;
    for (allowedItemIndex = 0; allowedItemIndex < EVT_ALLOWED_ITEM_COUNT; allowedItemIndex++) {
        if (D_003CE3F8[allowedItemIndex] == itemId) {
            return 1;
        }
    }
    return 0;
}

/* Add progress, treating zero as one. The unsigned check also clamps negative totals. */
s32 func_00263F50(s32 progressDelta) {
    if (progressDelta == 0) {
        progressDelta = 1;
    }
    ((EvtProgressState *)datGameState)->total += progressDelta;
    if ((u32)((EvtProgressState *)datGameState)->total > EVT_PROGRESS_UNSIGNED_LIMIT) {
        ((EvtProgressState *)datGameState)->total = 999999;
    }
    return ((EvtProgressState *)datGameState)->total;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263FB0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264120);

s32 evtDispatchSceneReadyFollowup(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    if (((EvtStateTableContext *)stateAddress)->sceneReady == 1) {
        func_00297970(stateAddress);
    } else {
        func_00298648(stateAddress);
    }
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_002642B8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

s32 evtPlayDispatchModeCue(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    dspSetActive(1);
    switch (((EvtStateTableContext *)stateAddress)->dispatchMode) {
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
    s32 stateAddress = kwlnTaskGetUserValue();
    switch (((EvtStateTableContext *)stateAddress)->dispatchMode) {
    case 1:
        mnuSetCommandPhase(stateAddress, 6);
        break;
    case 2:
        if (((EvtStateTableContext *)stateAddress)->stateTable != (s32)D_003CE498) {
            mnuSetCommandPhase(stateAddress, 5);
        }
        break;
    }
    ((EvtStateTableContext *)stateAddress)->dispatchMode = 0;
    return 1;
}

s32 evtSetPopupEntryWhenMessageWindowIdle(u64 callbackContext) {
    s32 stateAddress;
    s32 result;
    s32 *dispatchSlot;

    stateAddress = kwlnTaskGetUserValue();
    dispatchSlot = (s32 *)(stateAddress + 0x58);
    result = func_002C4038(stateAddress + 0xc, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if ((*dispatchSlot == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(dispatchSlot, ((EvtStateTableContext *)stateAddress)->stateTable);
        }
        result = 0;
    }
    return result;
}

extern void func_00294B40(s32, s32, s32, void *, s32, s32);
extern void func_00294D50(s32, s32, s32, void *, s32, s32);
extern void func_00294EB8(s32, s32, s32, void *, s32, s32);
extern void func_00295030(s32, s32, s32, void *, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, void *, s32);
extern void func_002969D8(s32, s32, s32, void *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);
extern void mnuClearWindowPanelTransitionFlag(void *);
extern void func_00296B48(s32, s32, s32, s32, s32, s32);
extern void func_00296D90(void *, s32);
extern void func_00296E98(s32, u32, s32, s32);

/* Draw the active dispatch mode before advancing its callback state. */
s32 func_00264480(s32 callbackContext) {
    EvtStateTableContext *state = (EvtStateTableContext *)kwlnTaskGetUserValue();

    func_0025FD78((s32)state);
    switch (state->dispatchMode) {
    case 1:
        func_00294B40(0, 0, 0, state, 0x100, 0x53);
        func_00294EB8(0, 0, 0, state, 0x100, 0x53);
        func_00295030(0, 0, 0, state, 0x100, 0, 0x53);
        mnuDrawIfActive(0, 0, 0, state->secondaryObject, 0x53);
        func_002969D8(0, 0, 0, state, 0x53);
        func_00296D90(state, 0xA09DC380);
        func_00296E98((s32)state, 0x100, 3, 0x53);
        break;
    case 2:
        if (state->stateTable == (s32)D_003CE498) {
            func_00294B40(0, 0, 0, state, 0x100, 0x53);
            func_00294D50(0, 0, 0, state, 0x100, 0x53);
            mnuDrawListChildrenWithCountdown(
                0, 0, 0, (u8 *)state->primaryObject->node, 0x53);
            func_00296D90(state, 0xA09DC380);
            func_00296E98((s32)state, 0x100, 4, 0x53);
        } else {
            func_00294B40(0, 0, 0, state, 0x100, 0x53);
            func_00294EB8(0, 0, 0, state, 0x100, 0x53);
            func_00295030(0, 0, 0, state, 0x100, 0, 0x53);
            mnuClearWindowPanelTransitionFlag(state->primaryObject);
            func_00296B48(0, 0, 0, (s32)state, 0x100, 0x53);
            func_00296D90(state, 0xA09DC380);
            func_00296E98((s32)state, 0x100, 2, 0x53);
        }
        break;
    }
    return func_002C4038((s32)state->dispatchWork, &state->dispatchState, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_002646C8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264710);

u32 func_00264848(void) {
    return 1;
}

extern void ptyAdjustItemQuantity(s32, s32);
extern void datAddCurrencyClamped(s32);
extern s32 func_00260DF0(s32);
extern u8 func_00260FE8(s32, s32);
extern u8 func_00261018(s32, s32);
extern s32 mnuCampResolveOwnedItemVariant(s32, s32);
extern s32 mnuCampGetCompactEntryId(s32, s32);
extern s32 func_002C54B0(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern char (*D_00435E5C)[25];
extern u8 D_003CE5E8[];
extern char D_00437848[];
extern char D_00437850[];

s32 func_00264850(s32 callbackContext) {
    char text[64];
    EvtStateTableContext *state;
    s32 *dispatchSlot;
    s32 result;
    s32 column;
    s32 sequence;

    state = (EvtStateTableContext *)kwlnTaskGetUserValue();
    dispatchSlot = &state->dispatchState;
    result = func_002C4038((s32)state->dispatchWork, dispatchSlot,
                         EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if (*dispatchSlot == 0 &&
            (result = evtGetMessageWindowControlState()) == 0) {
            if (state->grantPendingReward != 0) {
                if (state->rewardKind == 0) {
                    ptyAdjustItemQuantity(state->rewardValue, 1);
                } else {
                    datAddCurrencyClamped(state->rewardValue);
                }
                state->grantPendingReward = 0;
            }
            if (state->announceNextReward != 0) {
                state->announceNextReward = 0;
                dspStartEntry(0x2F);
            } else if (state->remainingRewards == 0) {
                mnuSetPopupEntryFlagged(dispatchSlot, D_003CE5E8);
            } else if (state->rewardDelay <= 0) {
                if (state->rewardMode < 0) {
                    column = func_00260DF0(state->rewardRow);
                    state->rewardKind = func_00260FE8(state->rewardRow, column);
                    state->rewardValue = mnuCampResolveOwnedItemVariant(state->rewardRow, column);
                } else {
                    state->rewardKind = func_00261018(state->rewardRow, state->rewardIndex);
                    state->rewardValue = mnuCampGetCompactEntryId(state->rewardRow, state->rewardIndex);
                }
                if (state->rewardKind == 0 && func_002C54B0(state->rewardValue) != 0) {
                    state->rewardDelay = 60;
                    sequence = 0x300003;
                } else {
                    state->rewardDelay = 30;
                    sequence = 0x300002;
                }
                sndSetSequenceVolumePan(sequence, 0x7F, 0x3F);
            } else {
                state->rewardDelay--;
                if (state->rewardDelay > 0) {
                    return 0;
                }
                if (state->rewardKind == 0) {
                    evtCopyEntryStringToActiveWindow(0, D_00435E5C[state->rewardValue]);
                    evtCopyEntryStringToActiveWindow(1, D_00437848);
                    dspStartEntry(0x2D);
                } else {
                    func_0035C860(text, D_00437850, state->rewardValue);
                    evtCopyEntryStringToActiveWindow(0, text);
                    dspStartEntry(0x2E);
                }
                state->remainingRewards--;
                state->rewardIndex++;
                if (state->remainingRewards != 0) {
                    state->announceNextReward = 1;
                }
                state->grantPendingReward = 1;
            }
        }
        result = 0;
    }
    return result;
}

s32 func_00264AB8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00264B10(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

/* Mark each newly completed slot and advance the persistent slot index. */
s32 evtAdvanceSlotFlags(void) {
    s32 advancedSlots = 0;
    while (evtGetRemainingSlotThreshold(((EvtProgressState *)datGameState)->slotIndex + advancedSlots + 1) == 0) {
        mdlFlagSet(D_003CE14C[(((EvtProgressState *)datGameState)->slotIndex + advancedSlots) * EVT_PROGRESS_RECORD_WORDS + EVT_PROGRESS_RECORD_WORDS]);
        advancedSlots++;
    }
    ((EvtProgressState *)datGameState)->slotIndex += advancedSlots;
    return advancedSlots;
}

u32 evtUpdateSlotAdvanceCount(void) {
    u8 advancedSlots;
    s32 stateAddress;

    stateAddress = kwlnTaskGetUserValue();
    advancedSlots = evtAdvanceSlotFlags();
    ((EvtStateTableContext *)stateAddress)->advancedSlots = advancedSlots;
    if ((((EvtStateTableContext *)stateAddress)->unkC8 == 0) && (((EvtStateTableContext *)stateAddress)->followupMode == '\x01')) {
        dspStartEntry(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

s32 evtOpenSlotAdvancePopupWhenIdle(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = (s32 *)(stateAddress + 0x58);
    s32 dispatchResult = func_002C4038(stateAddress + 0xc, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                mnuSetPopupEntryFlagged(dispatchSlot, D_003CE604);
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00264CE0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00264D38(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

/* Raise a gate's flag and play its cue only on the first threshold crossing. */
s32 evtTriggerProgressFlagGate(s32 unusedContext) {
    EvtFlagGate *gateEntry = (EvtFlagGate *)D_003CE400;
    u32 gateIndex;
    for (gateIndex = 0; gateIndex < EVT_PROGRESS_FLAG_GATE_COUNT; gateIndex++, gateEntry++) {
        if ((u32)(((EvtProgressState *)datGameState)->slotIndex + 1) >= (u32)gateEntry->threshold) {
            u32 progressFlag = gateEntry->flag;
            if (mdlFlagTest(progressFlag) == 0) {
                mdlFlagSet(progressFlag);
                dspStartEntry(gateEntry->cue);
                return 1;
            }
        }
    }
    return 0;
}

s32 evtShowSlotText(void) {
    char formattedText[0x40];
    if (*(s8 *)(kwlnTaskGetUserValue() + 0xcc) > 0) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + 0x11);
        func_0035C860(formattedText, D_00437840, D_003CE148[((EvtProgressState *)datGameState)->slotIndex * EVT_PROGRESS_RECORD_WORDS]);
        evtCopyEntryStringToActiveWindow(1, formattedText);
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

s32 evtTriggerProgressGateThenOpenPopup(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = (s32 *)(stateAddress + 0x58);
    s32 dispatchResult = func_002C4038(stateAddress + 0xc, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                if (evtTriggerProgressFlagGate(stateAddress) == 0) {
                    mnuSetPopupEntryFlagged(dispatchSlot, D_003CE620);
                }
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00264F98(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 func_00264FF0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(stateAddress + 0xc, (s32 *)(stateAddress + 0x58), EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00265038);

s32 evtIsLastSlot(s32 slotIndex) {
    s32 activeSlots = 0;
    u32 entryIndex;
    for (entryIndex = 0; entryIndex < EVT_PROGRESS_SLOT_COUNT; entryIndex++) {
        if (((EvtSlot *)D_003CE1A8)[entryIndex].flag != 0) {
            activeSlots++;
        }
    }
    return slotIndex + 1 == activeSlots;
}

INCLUDE_RODATA(const s32, "game/code_00261E10", D_00424D08);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437840);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437848);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437850);

