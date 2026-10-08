#include "mnu.h"
#include "kwln.h"
#include "dat_state.h"
#include "evt_world.h"

struct SdfTex;

#define EVT_ACTIVE_ENTRY_LIMIT 0xC0
#define EVT_DISPLAY_VALUE_COUNT 0x10
#define EVT_LAST_DISPLAY_VALUE 0xF
#define EVT_SCRIPT_PROCESS_PRIORITY 0x7D0
#define DSP_WINDOW_ENTRY_FLAG 0x200000
#define DSP_WINDOW_DISABLE_FLAG_MAIN 0x800000
#define DSP_WINDOW_DISABLE_FLAG_SECONDARY 0x100000
#define DSP_WINDOW_CONTROL_ACTIVE 1
#define DSP_WINDOW_CONTROL_GATED 2
#define DSP_WINDOW_PANEL_BASE_Y 0x15F
#define EVT_PANEL_DRAW_COMMAND 0x53
#define EVT_PANEL_BACKGROUND_COLOR 0x30303040
#define EVT_SCROLL_TOP_FLAG 1
#define EVT_SCROLL_BOTTOM_FLAG 2
#define DSP_FLAG_EVENT_REQUIRED 0x902
#define DSP_FLAG_EVENT_HANDLED 0x907
#define DSP_FLAG_EVENT_SPECIAL_ENTRY 5
#define DSP_FLAG_EVENT_SCENE_ENTRY 3
#define DSP_FLAG_EVENT_PARTY_ENTRY 4
#define EVT_PARTY_SLOT_COUNT 5
#define DSP_AREA_TEXT_BYTES 0x19
#define DSP_NAME_TEXT_BYTES 0x13
#define DSP_DIALOG_TEXT_BYTES 0x11

extern s32 D_003BC410;

extern s32 dspWindowHandle;

extern u8 dspWindowControlState;

extern s8 dspWindowStateGate;

extern s8 evtMessageWindowOption;

extern s8 dspCapturedSoundMode;


typedef struct EvtActiveFlagTable {
    s32 unk0;
    s32 unk4;
    s8 flags[0];
} EvtActiveFlagTable;

typedef struct {
    u8 entryCount;
    u8 pad;
    u16 entryIndices[0];
} ActiveList;

extern EvtActiveFlagTable evtActiveEntryFlags;

extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);

extern struct SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress);

extern s32 kwlnTaskGetUserValue();

extern u32 effMiscRand(void *);

extern u32 evtDisplayValues[];

extern void func_0024A2D8(s32 arg0);


extern void func_0024DD78(void);
extern void *dds3GetWorldSecondaryObject(void);
extern s32 dds3GetWorldObjectValue(EffWorldNode *world);
extern EffWorldNode *dds3FindIndexedObjectChainNodeByName(EffWorldNode *world, s32 index, const u8 *name);
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern void dspSetActive();
extern void itfMesSetWindowHighFlags(s32, s32);
extern void itfMesClearWindowHighFlags(s32, s32);
extern void itfPanelSetStatus(s32, s32);
extern void itfPanelSetPairFirst(s32, s32);
extern void itfMesStartEntry(s32, s32, s32);
extern void mnuSetPopupEntryFlagged(s32 *, char *);
extern char D_0036ACF8[];
extern void itfMesDestroyWindowIfPresent(s32);
extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);
extern s32 D_003BAA70;
extern s32 D_003BAA74;
extern s32 D_003BAA78;
extern s32 D_003BAA84;

typedef struct SceneFlagEntry {
    s32 requiredFlag; /* 0x00 */
    s32 handledFlag;  /* 0x04 */
    u16 areaIndex;   /* 0x08 */
    u8 nameIndex;    /* 0x0A */
    u8 pad0B;
    u16 dialogIndex; /* 0x0C */
    u16 pad0E;
} SceneFlagEntry;

typedef struct PartyFlagPair {
    s32 requiredFlag;
    s32 handledFlag;
} PartyFlagPair;


extern SceneFlagEntry mnuSceneFlagEventEntries[4];
extern PartyFlagPair mnuPartyFlagEventEntries[];

/* Close the current display channel before ensuring a replacement window.
 * DDS1 retains the legacy argument taken from the context's resource pair. */
void evtCloseDisplayChannelAndEnsureMessageWindow(void *context) {
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(*(u32 *)((u8 *)context + 0x60));
}

/* Start the first eligible special, scene, or active-party flag event.
 * Only panel kinds zero/one are accepted. Mark the handled flag immediately;
 * this is not a wait for dialogue completion, and dispatch results are ignored. */
s32 dspStartFlagEvent(s32 context) {
    s32 panelKind = *(s32 *)(context + 0x7C);
    s32 index;
    DatPartyRecord *slot;

    if (panelKind < 2) {
        if (panelKind >= 0) {
            if (mdlFlagTest(DSP_FLAG_EVENT_REQUIRED) != 0 && mdlFlagTest(DSP_FLAG_EVENT_HANDLED) == 0) {
                dspSetActive(1);
                dspStartEntry(DSP_FLAG_EVENT_SPECIAL_ENTRY);
                mdlFlagSet(DSP_FLAG_EVENT_HANDLED);
                return 1;
            }
            for (index = 0; index < sizeof(mnuSceneFlagEventEntries) / sizeof(mnuSceneFlagEventEntries[0]); index++) {
                if (mdlFlagTest(mnuSceneFlagEventEntries[index].requiredFlag) != 0 && mdlFlagTest(mnuSceneFlagEventEntries[index].handledFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(context);
                    dspSetActive(1);
                    evtCopyEntryStringToActiveWindow(0, D_003BAA84 + mnuSceneFlagEventEntries[index].areaIndex * DSP_AREA_TEXT_BYTES);
                    evtCopyEntryStringToActiveWindow(1, D_003BAA78 + mnuSceneFlagEventEntries[index].nameIndex * DSP_NAME_TEXT_BYTES);
                    evtCopyEntryStringToActiveWindow(2, D_003BAA74 + mnuSceneFlagEventEntries[index].dialogIndex * DSP_DIALOG_TEXT_BYTES);
                    dspStartEntry(DSP_FLAG_EVENT_SCENE_ENTRY);
                    mdlFlagSet(mnuSceneFlagEventEntries[index].handledFlag);
                    return 1;
                }
            }
            for (index = 0; index < EVT_PARTY_SLOT_COUNT; index++) {
                slot = &datGameState->party[index];
                if ((slot->flags & 1) != 0 && mdlFlagTest(mnuPartyFlagEventEntries[slot->unitId].requiredFlag) != 0
                    && mdlFlagTest(mnuPartyFlagEventEntries[slot->unitId].handledFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(context);
                    dspSetActive(1);
                    evtCopyEntryStringToActiveWindow(0, D_003BAA70 + slot->unitId * DSP_DIALOG_TEXT_BYTES);
                    dspStartEntry(DSP_FLAG_EVENT_PARTY_ENTRY);
                    mdlFlagSet(mnuPartyFlagEventEntries[slot->unitId].handledFlag);
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Initialize the current terminal task's resource bank and mode-zero fade track. */
s32 mnuPrepareTerminalPanelState(void) {
    s32 *work = (s32 *)kwlnTaskGetUserValue();

    mnuTerminalSelectResourceBank(work);
    mnuApplyFadeTrackMode(0, work);
    return 1;
}

u32 func_0024D260(void) {
    return 1;
}

/* Propagate the panel worker's nonzero result before considering a flag-event popup.
 * When its popup slot and message control are idle, fall back if no event starts. */
s32 dspUpdateFlagEvent(s32 request) {
    s32 workAddress = kwlnTaskGetUserValue();
    s32 *popupSlot = (s32 *)(workAddress + 0x54);
    s32 result = menuRunPanel((void *)workAddress, 0, (void *)request);
    if (result != 0) {
        return result;
    }
    if (*popupSlot == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (dspStartFlagEvent(workAddress) == 0) {
                mnuSetPopupEntryFlagged(popupSlot, D_0036ACF8);
            }
        }
    }
    return 0;
}

/* Prepare the current terminal work and dispatch panel mode one. */
s32 mnuDispatchTerminalPanel(s32 request) {
    s32 workAddress = kwlnTaskGetUserValue();
    func_0024A2D8(workAddress);
    return menuRunPanel((void *)workAddress, 1, (void *)request);
}

/* Request the message-window transition before dispatching terminal panel mode two. */
s32 mnuDispatchTerminalPanelExit(s32 request) {
    s32 workAddress = kwlnTaskGetUserValue();
    func_0024DD78();
    return menuRunPanel((void *)workAddress, 2, (void *)request);
}

/* Reset display slots for a zero exit marker, otherwise start the existing fade-in.
 * Preserve the legacy zero-argument active-flag call; no index is supplied here. */
u32 mnuApplyTerminalExitFadeOrReset(void) {
    s32 workAddress;

    workAddress = kwlnTaskGetUserValue();
    if (*(s32 *)(workAddress + 0xdc) == 0) {
        evtClearActiveFlag();
        evtSetBoundedDisplayValue(0, 1);
        evtSetBoundedDisplayValue(1, 0);
    }
    else {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    return 1;
}

/* Select the terminal's mode-one fade track before starting the black fade-out. */
s32 mnuStartTerminalPanelFadeOut(void) {
    s32 workAddress = kwlnTaskGetUserValue();

    mnuApplyFadeTrackMode(1, workAddress);
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

extern KwlnTask *kwlnTaskGetTaskByName(const char *);
extern s32 kwlnFadeIsActive(void);
extern s32 evtIsActiveFlagSet(s32);
extern char D_003AF710[];
extern char D_0036AE48[];

/* Propagate the panel worker result; create a ready popup only for an empty slot.
 * Kind zero waits for the named task unless active flag zero overrides the wait;
 * other kinds wait for the fade to finish. */
s32 mnuUpdateTerminalReadyPopup(s32 request) {
    s32 workAddress = kwlnTaskGetUserValue();
    s32 *popupSlot = (s32 *)(workAddress + 0x54);
    s32 result = menuRunPanel((void *)workAddress, 0, (void *)request);
    s32 ready;

    if (result != 0) {
        return result;
    }
    if (*popupSlot == 0) {
        if (*(s32 *)(workAddress + 0x7C) == 0) {
            ready = kwlnTaskGetTaskByName(D_003AF710) == NULL;
            if (evtIsActiveFlagSet(0) != 0) {
                ready = 1;
            }
        } else {
            ready = kwlnFadeIsActive() == 0;
        }
        if (ready) {
            mnuSetPopupEntryFlagged(popupSlot, D_0036AE48);
        }
    }
    return 0;
}

/* Prepare the current work and dispatch panel mode one; distinct callback role unknown. */
s32 func_0024D500(s32 request) {
    s32 workAddress = kwlnTaskGetUserValue();
    func_0024A2D8(workAddress);
    return menuRunPanel((void *)workAddress, 1, (void *)request);
}

/* Dispatch current work in panel mode two without the message transition request. */
s32 func_0024D550(s32 request) {
    s32 workAddress = kwlnTaskGetUserValue();

    return menuRunPanel((void *)workAddress, 2, (void *)request);
}

/* Clear the terminal task's stored selection word and report completion. */
u32 mnuClearTerminalPanelSelection(void) {
    s32 workAddress;

    workAddress = kwlnTaskGetUserValue();
    *(u32 *)(workAddress + 0x90) = 0;
    return 1;
}

/* Release the current task's menu resources before rebuilding its terminal menus. */
s32 mnuRebuildTerminalMenuResources(void) {
    s32 *work = (s32 *)kwlnTaskGetUserValue();

    mnuReleaseWorkResources(work);
    mnuTerminalBuildMenus(work);
    return 1;
}

u32 func_0024D5F0(void) {
    return 0;
}

u32 func_0024D5F8(void) {
    return 0;
}

u32 func_0024D600(void) {
    return 0;
}

u32 func_0024D608(void) {
    return 0xffffffff;
}

/* Copy the source node's nested value only on a successful lookup.
 * The destination and both nested data pointers are caller-owned. */
s32 evtCopyWorldObjectEntryValue(s32 objectId, s32 destination) {
    EffWorldNode *source;
    EffWorldNode *destinationNode;

    source = dds3FindIndexedObjectChainNodeByName((EffWorldNode *)dds3GetWorldSecondaryObject(), 9,
                                                  (const u8 *)objectId);
    if (source != NULL) {
        destinationNode = (EffWorldNode *)destination;
        *(s32 *)((u8 *)destinationNode->data + 0x80) = *(s32 *)((u8 *)source->data + 0x78);
        return 1;
    }
    return 0;
}

/* Recreate the secondary world selection only when its packed key changes.
 * Preserve the signed shift/add rather than treating this as an unsigned OR. */
void evtSwitchWorldValueIfChanged(s32 high, s32 low) {
    s32 key = (high << 16) + low;
    if (dds3GetWorldObjectValue(dds3GetWorldSecondaryObject()) != key) {
        evtCreateWorldObjectForKey(high, low);
    }
}

/* Reset draw offsets and the five enabled screen-effect modes. */
void evtResetDrawTransitions(void) {
    kwlnDrawSetOverlayTransition(0, 0, 0);
    kwlnDrawSetupC70B(0);
    kwlnDrawEnableCd0(0);
    effDisableRectangleBlur();
    effDisableTexturedBlur();
    effDisableTexturedSquare();
    effDisableFilterBlur();
    effDisableColorRectangle();
}

/* Shut down the stage before clearing its draw transitions. */
void evtShutdownStageAndResetDrawTransitions(void) {
    evtCommandShutdownStage();
    evtResetDrawTransitions();
}

typedef struct QuadRecord {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s32 unk10;
} QuadRecord;

/* Fill three words, two adjacent halfwords and a final word.
 * These fields are write-only here; their rendering meanings are unknown.
 * DDS1 takes word-sized inputs for the halfword stores and truncates them. */
void evtFillQuadRecordFields(s32 firstWord, s32 secondWord, s32 thirdWord, s32 firstHalfword, s32 secondHalfword, s32 lastWord, QuadRecord *record) {
    record->unk00 = firstWord;
    record->unk04 = secondWord;
    record->unk08 = thirdWord;
    record->unk0C = firstHalfword;
    record->unk0E = secondHalfword;
    record->unk10 = lastWord;
}

/* Destroy a nonzero, registered task. The declared return value is not set. */
s32 evtDestroyRegisteredTaskIfPresent(s32 task) {
    if (task != 0) {
        if (kwlnTaskGetRegisteredState(task)) {
            kwlnTaskDestroyWithHierarchy(task, 0);
        }
    }
}

/* Replace an optional task slot, clear active flag zero, and return the new task.
 * Creation is still attempted if no slot is supplied. */
s32 evtReplaceScriptProcessTask(s32 processId, s32 processValue, s32 *taskSlot) {
    s32 task;

    if (taskSlot != 0) {
        evtDestroyRegisteredTaskIfPresent(*taskSlot);
    }
    task = scrCreateTaskForProcessId(EVT_SCRIPT_PROCESS_PRIORITY, processId, processValue);
    evtClearActiveFlag(0);
    if (taskSlot != 0) {
        *taskSlot = task;
    }
    return task;
}

/* Emit active game-entry indices in ascending order, excluding entry zero.
 * DDS1 scans 1..191; the caller must provide enough trailing halfwords. */
void evtCollectActiveGameIndices(ActiveList *list) {
    s32 index;
    list->entryCount = 0;
    for (index = 1; index < EVT_ACTIVE_ENTRY_LIMIT; index++) {
        if (datGameState->inventory.counts[index] != 0) {
            s32 outputIndex = list->entryCount++;
            list->entryIndices[outputIndex] = index;
        }
    }
}

/* Three-way ascending comparison of the two unsigned bytes. */
s32 evtCompareBytesAscending(u8 *left, u8 *right) {
    u8 leftValue = *left;
    u8 rightValue = *right;

    if (rightValue < leftValue) {
        return 1;
    }
    return (leftValue < rightValue) ? -1 : 0;
}

/* Keep non-excluded bytes in order and return the retained count.
 * Only retained source positions are cleared; this does not zero the tail. */
s32 evtCompactFilteredBytes(u8 *buffer, s32 length, u8 excluded) {
    s32 index;
    s32 retainedCount = 0;
    for (index = 0; index < length; index++) {
        if (buffer[index] != excluded) {
            u8 value = buffer[index];
            buffer[index] = 0;
            buffer[retainedCount++] = value;
        }
    }
    return retainedCount;
}

/* Perform the requested number of random swaps, not a Fisher-Yates shuffle.
 * Positive swapCount requires nonzero length; equal positions are allowed. */
void evtRandomSwapBytes(u8 *buffer, u32 length, s32 swapCount) {
    s32 swapIndex;
    for (swapIndex = 0; swapIndex < swapCount; swapIndex++) {
        u8 *first = buffer + effMiscRand(0) % length;
        u8 *second = buffer + effMiscRand(0) % length;
        u8 value = *first;
        *first = *second;
        *second = value;
    }
}

/* Store the resource handle in word zero and the loader's data address in word one. */
void evtLoadResourcePair(u32 resourceId, u32 *record) {
    u32 handle;

    handle = (u32)sdfReadNamedResource((const char *)(u32)resourceId, record + 1, 0);
    *record = handle;
}

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(u32 *record) {
    sdfReleaseResourceAllocation(*record);
}

/* Create the singleton message window only when its handle is negative.
 * The legacy DDS1 argument is unused; the page setup remains unconditional. */
s32 evtCreateMessageWindowIfMissing(s32 unused) {
    if (dspWindowHandle < 0) {
        dspWindowHandle = itfMesCreateWindow();
        itfMesSetWindowPageAndRefresh(dspWindowHandle, 2, 0);
        return 1;
    }
    return 0;
}

/* Apply page mode zero and the caller's opaque page value to an open window. */
s32 evtSetMessageWindowPageValue(s32 pageValue) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    itfMesSetWindowPageAndRefresh(dspWindowHandle, 0, pageValue);
    return 1;
}

/* Start an entry, mark the first panel value pending, and enable control state one. */
s32 dspStartEntry(s32 entry) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    itfMesSetWindowHighFlags(dspWindowHandle, DSP_WINDOW_ENTRY_FLAG);
    itfMesStartEntry(dspWindowHandle, entry, 0);
    itfPanelSetPairFirst(dspWindowHandle, -1);
    dspWindowControlState = DSP_WINDOW_CONTROL_ACTIVE;
    return 1;
}

/* Store the caller's opaque value and snapshot the panel's second halfword.
 * This routine itself performs no sound-driver operation. */
s32 evtStoreValueAndCaptureWindowPanelValue(s32 requestedValue) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    D_003BC410 = requestedValue;
    dspCapturedSoundMode = dspReadWindowSecondPanelValue();
    return 1;
}

/* Update the byte-sized option only while the singleton window is open. */
void evtSetMessageWindowOptionWhenOpen(s32 option) {
    if (dspWindowHandle >= 0) {
        evtMessageWindowOption = option;
    }
}

/* Return the retained option byte, even when the window is closed. */
s8 evtGetMessageWindowOption(void) {
    return evtMessageWindowOption;
}

/* Read the second panel halfword, or -1 when no message window exists.
 * Despite the inherited name, this is not a sound-driver query. */
s32 dspReadWindowSecondPanelValue(void) {
    if (dspWindowHandle < 0) {
        return -1;
    }
    return itfPanelGetPairSecond(dspWindowHandle);
}

/* Return the captured signed-byte panel value without querying the live window. */
s8 evtGetCapturedWindowPanelValue(void) {
    return dspCapturedSoundMode;
}

/* Reset an open window, optionally notify it first, and report whether it existed.
 * Cleanup leaves the singleton handle intact; dspCloseChannel destroys it. */
u32 evtCleanupMessageWindow(s32 notify) {
    u32 cleaned;

    cleaned = 0;
    if (-1 < dspWindowHandle) {
        itfPanelSetStatus(dspWindowHandle, 0);
        if (notify != 0) {
            itfMesFinishWindowAndClearStatus(dspWindowHandle);
        }
        itfMesCleanupWindow(dspWindowHandle, 0);
        dspSetActive(1);
        dspWindowControlState = 0;
        cleaned = 1;
    }
    return cleaned;
}

/* Finish/reset the singleton window with notification; ignore the existence result. */
void evtFinishMessageWindowAndNotify(void) {
    evtCleanupMessageWindow(1);
}

/* Destroy an existing singleton window and invalidate its handle and control gates. */
s32 dspCloseChannel(void) {
    s32 window = dspWindowHandle;
    if (window < 0) {
        return 0;
    }
    itfMesDestroyWindowIfPresent(window);
    dspWindowHandle = -1;
    dspWindowControlState = 0;
    dspWindowStateGate = 0;
    return 1;
}

/* A closed window reports zero; the state gate hides only control state two. */
s32 evtGetMessageWindowControlState(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    if (dspWindowStateGate != 0) {
        if ((s8)dspWindowControlState == DSP_WINDOW_CONTROL_GATED) {
            return 0;
        }
    }
    return (s8)dspWindowControlState;
}

/* Snapshot the second panel halfword only after the first halfword is nonnegative. */
s32 dspCaptureWindowSecondPanelValue(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    if (itfPanelGetPairFirst(dspWindowHandle) < 0) {
        return 0;
    }
    dspCapturedSoundMode = dspReadWindowSecondPanelValue();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024CFB0", func_0024DC98);

/* Request mode one from the existing message-window worker. */
void func_0024DD78(void) {
    func_0024DC98(1);
}

/* Copy a string address into a window table slot; neither argument is an item id. */
void evtCopyEntryStringToActiveWindow(s32 slotIndex, s32 sourceAddress) {
    itfMesCopyStringToWindowTableSlot(dspWindowHandle, slotIndex, sourceAddress);
}

/* Return the gate byte independently of the singleton window's existence. */
s8 dspGetWindowStateGate(void) {
    return dspWindowStateGate;
}

/* Enable clears both disable flags and restores panel/control state one.
 * Disable sets the gate and flags, but deliberately leaves the control byte alone.
 * Callers must already have a valid window. */
void dspSetActive(s32 enabled) {
    if (enabled) {
        itfMesClearWindowHighFlags(dspWindowHandle, DSP_WINDOW_DISABLE_FLAG_MAIN);
        itfMesClearWindowHighFlags(dspWindowHandle, DSP_WINDOW_DISABLE_FLAG_SECONDARY);
        dspWindowStateGate = 0;
        itfPanelSetStatus(dspWindowHandle, 1);
        dspWindowControlState = DSP_WINDOW_CONTROL_ACTIVE;
    } else {
        itfMesSetWindowHighFlags(dspWindowHandle, DSP_WINDOW_DISABLE_FLAG_MAIN);
        itfMesSetWindowHighFlags(dspWindowHandle, DSP_WINDOW_DISABLE_FLAG_SECONDARY);
        dspWindowStateGate = 1;
    }
}

/* Convert x/y to the window's 16-by-8 coordinate units and set its panel offset.
 * The original signed shifts are retained, including their input assumptions. */
void evtMoveMessageWindowWithPanelOffset(s32 x, s32 y) {
    itfMesBlk24MoveTo(dspWindowHandle, x << 4, y << 3);
    itfPanelEmitRecord(dspWindowHandle, -((DSP_WINDOW_PANEL_BASE_Y - y) << 3));
}

/* Recognize registered states one through three. Preserve the repeated queries. */
s32 evtIsTaskInActiveStates(s32 task) {
    if (kwlnTaskGetRegisteredState(task) == 1) {
        return 1;
    }
    if (kwlnTaskGetRegisteredState(task) == 2) {
        return 1;
    }
    return kwlnTaskGetRegisteredState(task) == 3;
}

/* Clear one active-entry flag; the caller supplies a valid index. */
void evtClearActiveFlag(s32 flagIndex) {
    evtActiveEntryFlags.flags[flagIndex] = 0;
}

/* Test one active-entry flag; the caller supplies a valid index. */
s32 evtIsActiveFlagSet(s32 flagIndex) {
    return evtActiveEntryFlags.flags[flagIndex] != 0;
}

/* Reject only indices at/above the upper bound; negative indices are not checked. */
s32 evtSetBoundedDisplayValue(s32 index, s32 value) {
    if (index >= EVT_DISPLAY_VALUE_COUNT) {
        return 0;
    }
    evtDisplayValues[index] = value;
    return 1;
}

/* Clamp only the upper side to the last display slot; negative indices pass through. */
u32 evtGetBoundedDisplayValue(s32 index) {
    index = (index < EVT_DISPLAY_VALUE_COUNT) ? index : EVT_LAST_DISPLAY_VALUE;
    return evtDisplayValues[index];
}

/* Script opcode: set the supplied active-entry flag without bounds checking. */
s32 evtOpSetActiveEntryFlag(void) {
    s32 flagIndex = scrReadIntParameter(0);

    evtActiveEntryFlags.flags[flagIndex] = 1;
    return 1;
}

/* Script opcode: return a display-table value, clamping only its upper index.
 * It does not activate or test an active-entry flag. */
s32 evtOpReadDisplayValue(void) {
    s32 index = scrReadIntParameter(0);
    if (index >= EVT_DISPLAY_VALUE_COUNT) {
        index = EVT_LAST_DISPLAY_VALUE;
    }
    scrSetIntegerReturnValue(evtDisplayValues[index]);
    return 1;
}

/* Acquire the loaded data's texture reference, then release its temporary resource.
 * DDS1 passes the size output as a pointer, unlike DDS2's address-valued slot. */
u32 evtLoadTextureFromResourcePath(u32 path) {
    u32 info[2];
    u32 allocation = (u32)sdfReadNamedResource((const char *)(u32)path, info, &info[1]);
    u32 texture = (u32)sdfTexAcquireResourceTexture((void *)info[0]);

    sdfReleaseResourceAllocation(allocation);
    return texture;
}

typedef struct EvtListViewportState {
    u8 pad0[4];
    u32 flags;
} EvtListViewportState;

typedef struct EvtListPanelRecord {
    u8 pad0[0x14];
    EvtListViewportState *viewport;
} EvtListPanelRecord;

extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);

/* Draw top/bottom viewport indicators; each flag selects its brighter color.
 * The two triangles retain their signed x-radius and y-offset calculations. */
INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspWindowHandle);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspWindowControlState);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspWindowStateGate);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC410);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", evtMessageWindowOption);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspCapturedSoundMode);

void evtDrawListViewportIndicators(s32 x, s32 topY, s32 bottomY, s32 size, EvtListPanelRecord *record) {
    s32 coordinates[2][3];
    s32 xRadius = (size << 4) >> 1;
    s32 yOffset = size << 3;
    u32 flags = record->viewport->flags;
    u32 colors[2] = {0x8080C040, 0x30306040};

    coordinates[0][0] = x;
    coordinates[0][1] = x - xRadius;
    coordinates[0][2] = x + xRadius;
    coordinates[1][0] = topY;
    coordinates[1][2] = coordinates[1][1] = topY + yOffset;
    uiDrawUniformRgbRange(coordinates[0], coordinates[1], 0,
                          (flags & EVT_SCROLL_TOP_FLAG) ? colors[0] : colors[1], EVT_PANEL_DRAW_COMMAND);

    coordinates[1][0] = bottomY;
    coordinates[1][2] = coordinates[1][1] = bottomY - yOffset;
    uiDrawUniformRgbRange(coordinates[0], coordinates[1], 0,
                          (flags & EVT_SCROLL_BOTTOM_FLAG) ? colors[0] : colors[1], EVT_PANEL_DRAW_COMMAND);
}

void evtDrawListViewportPanel(s32 x, s32 y, s32 width, s32 record) {
    s32 height = mnuGetListViewportHeight(*(s32 *)(record + 0x14)) + 0x80;
    uiDrawUniformColorRect(x, y, 0, width, height, 0x30303040, 0x53);
    evtDrawListViewportIndicators(x + width - 0xA0, y, y + height, 8, record);
}

/* Draw a plain background rectangle with the shared panel color and command. */
void evtDrawPlainPanel(u32 x, u32 y, u32 width, u32 height) {
    uiDrawUniformColorRect(x, y, 0, width, height, EVT_PANEL_BACKGROUND_COLOR, EVT_PANEL_DRAW_COMMAND);
}
