#include "evt_world.h"
#include "dat_state.h"

#define EVT_ACTIVE_ENTRY_LIMIT 0x100
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
#define EVT_PARTY_SLOT_COUNT 5
#define MNU_MANTRA_POSITION_RECORD_BYTES 0x20
#define MNU_MANTRA_SELECTION_WORK_BYTES 0x16C
#define PRF_ATTRIBUTE_SLOT_COUNT 5
#define PRF_ATTRIBUTE_MULTIPLE_SENTINEL 5

extern s32 mnuMantraPanelPositionTable;

extern EvtLoadedRecord *mnuMantraNodePositionTable;

extern s32 func_0026CD50(u32);

extern s32 evtGetMessageWindowControlState(void);

extern u32 sdfReadNamedResource(u32, u32 *, u32);

extern s32 dspWindowHandle;

extern s8 dspWindowControlState;

extern void func_0026C900(void);

extern s32 D_00437888;

extern s8 dspCapturedSoundMode;

extern s8 evtMessageWindowOption;

extern s8 dspWindowStateGate;

extern u32 evtDisplayValues[];

void sdfReleaseResourceAllocation(u32 sprite);




typedef struct {
    u8 entryCount;
    u8 pad;
    u16 entryIndices[0];
} ActiveList;

extern void *dds3GetWorldSecondaryObject(void);
extern s32 dds3GetWorldObjectValue(EffWorldNode *world);
extern void evtCreateWorldObjectForKey(s32, s32);
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

/* Fill three words, two adjacent halfwords and a final word.
 * These fields are write-only here; their rendering meanings are unknown.
 * DDS2 already receives the two halfword values as s16. */
void evtFillQuadRecordFields(u32 firstWord, u32 secondWord, u32 thirdWord, s16 firstHalfword, s16 secondHalfword, u32 lastWord, u32 *record) {
    record[0] = firstWord;
    record[1] = secondWord;
    record[2] = thirdWord;
    *(s16 *)&record[3] = firstHalfword;
    *((s16 *)&record[3] + 1) = secondHalfword;
    record[4] = lastWord;
}

/* Destroy a nonzero, registered task. The declared return value is not set. */
s32 evtDestroyRegisteredTaskIfPresent(s32 task) {
    if (task != 0) {
        if (kwlnTaskGetRegisteredState(task) != 0) {
            kwlnTaskDestroyWithHierarchy(task, 0);
        }
    }
}

extern s32 scrCreateTaskForProcessId();
/* Replace an optional task slot, clear active flag zero, and return the new task.
 * Creation is still attempted if no slot is supplied. */
s32 evtReplaceScriptProcessTask(s32 processId, s32 processValue, s32 *taskSlot) {
    s32 task;

    if (taskSlot != NULL) {
        evtDestroyRegisteredTaskIfPresent(*taskSlot);
    }
    task = scrCreateTaskForProcessId(EVT_SCRIPT_PROCESS_PRIORITY, processId, processValue);
    evtClearActiveFlag(0);
    if (taskSlot != NULL) {
        *taskSlot = task;
    }
    return task;
}

/* Emit active game-entry indices in ascending order, excluding entry zero.
 * DDS2 scans 1..255; the caller must provide enough trailing halfwords. */
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

extern u32 effMiscRand();
/* Perform the requested number of random swaps, not a Fisher-Yates shuffle.
 * Positive swapCount requires nonzero length; equal positions are allowed. */
void evtRandomSwapBytes(u8 *buffer, u32 length, s32 swapCount) {
    u8 *first;
    u8 *second;
    u8 value;

    if (swapCount > 0) {
        s32 remaining = swapCount;
        do {
            remaining--;
            first = buffer + effMiscRand(0) % length;
            second = buffer + effMiscRand(0) % length;
            value = *first;
            *first = *second;
            *second = value;
        } while (remaining != 0);
    }
}

/* Store the resource handle in word zero and the loader's data address in word one. */
void evtLoadResourcePair(u32 resourceId, EvtResourcePair *record) {
    u32 handle;

    handle = sdfReadNamedResource(resourceId, &record->unk04, 0);
    record->handle = handle;
}

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(EvtResourcePair *record) {
    sdfReleaseResourceAllocation(record->handle);
}

extern s32 itfMesCreateWindow(void);
/* Create the singleton message window only when its handle is negative.
 * The page setup remains unconditional after the allocation attempt. */
s32 evtCreateMessageWindowIfMissing(void) {
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

extern void itfMesSetWindowHighFlags(s32, u32);
extern void itfMesStartEntry(s32, s32, s32);
extern void itfPanelSetPairFirst(s32, s32);
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
    D_00437888 = requestedValue;
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

extern void itfMesDestroyWindowIfPresent(s32);
/* Destroy an existing singleton window and invalidate its handle and control gates. */
s32 dspCloseChannel(void) {
    if (dspWindowHandle < 0) {
        return 0;
    }
    itfMesDestroyWindowIfPresent(dspWindowHandle);
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
    if (dspWindowStateGate != 0 && dspWindowControlState == DSP_WINDOW_CONTROL_GATED) {
        return 0;
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

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026C7F8);

/* Pass the supplied value to mode one of the existing message-window worker. */
void func_0026C8E8(u32 value) {
    func_0026C7F8(value, 1);
}

/* Request value one from the existing mode-one message-window worker. */
void func_0026C900(void) {
    func_0026C8E8(1);
}

/* Copy a string address into a window table slot; neither argument is an item id. */
void evtCopyEntryStringToActiveWindow(s32 slotIndex, s32 sourceAddress) {
    itfMesCopyStringToWindowTableSlot(dspWindowHandle, slotIndex, sourceAddress);
}

/* Return the gate byte independently of the singleton window's existence. */
s8 dspGetWindowStateGate(void) {
    return dspWindowStateGate;
}

extern void itfMesClearWindowHighFlags(s32, u32);
extern void itfPanelSetStatus(s32, s32);
/* Enable clears both disable flags and restores panel/control state one.
 * Disable sets the gate and flags, but deliberately leaves the control byte alone.
 * Callers must already have a valid window. */
void dspSetActive(s32 enabled) {
    if (enabled != 0) {
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
 * DDS2's original multiplication form is retained rather than copied from DDS1. */
void evtMoveMessageWindowWithPanelOffset(s32 x, s32 y) {
    itfMesBlk24MoveTo(dspWindowHandle, x * 16, y * 8);
    itfPanelEmitRecord(dspWindowHandle, -((DSP_WINDOW_PANEL_BASE_Y - y) * 8));
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

extern s8 evtActiveEntryFlags[8];

/* Clear one active-entry flag; the caller supplies a valid index. */
void evtClearActiveFlag(s32 flagIndex) {
    evtActiveEntryFlags[8 + flagIndex] = 0;
}

/* Test one active-entry flag; the caller supplies a valid index. */
s32 evtIsActiveFlagSet(s32 flagIndex) {
    return evtActiveEntryFlags[8 + flagIndex] != 0;
}

/* Reject only indices at/above the upper bound; negative indices are not checked. */
s32 evtSetBoundedDisplayValue(s32 index, s32 value) {
    if (index < EVT_DISPLAY_VALUE_COUNT) {
    } else {
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
    evtActiveEntryFlags[8 + flagIndex] = 1;
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

extern s32 sdfTexAcquireResourceTexture(u32);
/* Acquire the loaded data's texture reference, then release its temporary resource.
 * DDS2 passes the size-output address as an integer, unlike DDS1's pointer slot. */
s32 evtLoadTextureFromResourcePath(u32 path) {
    u32 info[2];
    u32 allocation;
    s32 texture;

    allocation = sdfReadNamedResource(path, &info[0], (u32)&info[1]);
    texture = sdfTexAcquireResourceTexture(info[0]);
    sdfReleaseResourceAllocation(allocation);
    return texture;
}

extern u32 D_00437890[];
extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);

/* Draw top/bottom viewport indicators; each flag selects its brighter color.
 * DDS2 copies its two colors from the data table rather than using DDS1 literals. */
void evtDrawListViewportIndicators(s32 x, s32 topY, s32 bottomY, s32 size, s32 record) {
    s32 coordinates[2][3];
    s32 xRadius = (size << 4) >> 1;
    s32 yOffset = size << 3;
    u32 flags = *(u32 *)(*(s32 *)(record + 0x18) + 4);
    u32 colors[2];

    memcpy(colors, D_00437890, sizeof(colors));
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

extern s32 mnuGetListViewportHeight(s32);

extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, u32, s32);

extern void evtDrawListViewportIndicators(s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[0x18];
    s32 heightSource; /* 0x18: passed to mnuGetListViewportHeight for the panel height */
} EvtPanelRecord;

typedef struct EvtMantraNodePositionRecord {
    union {
        u32 packedHeader; /* Flags and signed ID also have a native word view. */
        struct {
            u16 flags;
            s16 id;
        };
    };
    s16 firstKey;  /* 0x04 */
    s16 secondKey; /* 0x06 */
    struct EvtMantraNodePositionRecord *neighbors[6];
} EvtMantraNodePositionRecord; /* 0x20 */

void evtDrawListViewportPanel(s32 x, s32 y, s32 width, EvtPanelRecord *record) {
    s32 height = mnuGetListViewportHeight(record->heightSource) + 0x80;

    uiDrawUniformColorRect(x, y, 0, width, height, 0x30303040, 0x53);
    evtDrawListViewportIndicators(x + width - 0xA0, y, y + height, 8, (s32)record);
}

/* Draw a plain background rectangle with the shared panel color and command. */
void evtDrawPlainPanel(u32 x, u32 y, u32 width, u32 height) {
    uiDrawUniformColorRect(x, y, 0, width, height, EVT_PANEL_BACKGROUND_COLOR, EVT_PANEL_DRAW_COMMAND);
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CD50);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026CE90);

/* Replace the node-position resource, releasing an existing table first. */
void mnuLoadMantraNodePositionTable(u32 resourceId) {
    if (mnuMantraNodePositionTable != 0) {
        mnuReleaseMantraNodePositionTable();
    }
    mnuMantraNodePositionTable = (EvtLoadedRecord *)func_0026CD50(resourceId);
}

/* Release the retained node-position handle and clear the global table address. */
void mnuReleaseMantraNodePositionTable(void) {
    sdfReleaseResourceAllocation(mnuMantraNodePositionTable->handle);
    mnuMantraNodePositionTable = 0;
}

/* Return a 32-byte record address for a signed halfword index; callers sign-extend the index at the call
 * (func_0028DC08 +0x34), and this body re-extends it (sll/sra 16). */
s32 mnuGetMantraNodePositionRecord(s16 index) {
    return mnuMantraNodePositionTable->recordsAddress + index * 32;
}

/* Find the first position matching two signed-halfword, staggered-grid keys.
 * The second coordinate's odd bit adds five to the first key; both scale by ten.
 * A loaded table is required; a failed search returns address zero. */
s32 mnuFindMantraNodePositionRecord(s32 firstCoordinate, s32 secondCoordinate) {
    EvtLoadedRecord *loaded = mnuMantraNodePositionTable;
    s16 firstKey;
    s16 secondKey;
    EvtMantraNodePositionRecord *record;
    s32 index;

    firstCoordinate = (s16)firstCoordinate;
    secondCoordinate = (s16)secondCoordinate;
    firstKey = (s16)(firstCoordinate * 10 + ((secondCoordinate & 1) * 5));
    secondKey = (s16)(secondCoordinate * 10);
    record = (EvtMantraNodePositionRecord *)loaded->recordsAddress;

    for (index = 0; index < (s32)loaded->recordCount; index++) {
        if (record->firstKey == firstKey && record->secondKey == secondKey) {
            return (s32)record;
        }
        record = (EvtMantraNodePositionRecord *)((u8 *)record + MNU_MANTRA_POSITION_RECORD_BYTES);
    }
    return 0;
}

/* Return the loaded node-position count; the table must already exist. */
u32 mnuGetMantraNodePositionRecordCount(void) {
    return mnuMantraNodePositionTable->recordCount;
}

/* Replace the panel-position resource, releasing an existing table first. */
void mnuLoadMantraPanelPositionTable(u32 resourceId) {
    if (mnuMantraPanelPositionTable != 0) {
        mnuReleaseMantraPanelPositionTable();
    }
    mnuMantraPanelPositionTable = func_0026CD50(resourceId);
}

/* Release the retained panel-position handle and clear the global table address. */
void mnuReleaseMantraPanelPositionTable(void) {
    sdfReleaseResourceAllocation(((EvtLoadedRecord *)mnuMantraPanelPositionTable)->handle);
    mnuMantraPanelPositionTable = 0;
}

/* Return a 32-byte panel record using the same signed low-halfword index convention. */
s32 mnuGetMantraPanelPositionRecord(s32 index) {
    return ((EvtLoadedRecord *)mnuMantraPanelPositionTable)->recordsAddress + ((index << 0x10) >> 0xb);
}

extern u32 sdfAllocGeneralBlock(u32);
extern void *sdfMemoryGetBlockAddress(u32);
extern void func_0026D168(void *, s32, s32);
typedef struct EvtMantraWork {
    u32 allocation;
    u32 capacity;
    void *entries;
    u8 data[0x160];
} EvtMantraWork; /* 0x16C bytes */
/* Allocate/zero the selection work and point its entries at its inline storage.
 * The existing initializer is called only for nonzero initialValue. */
EvtMantraWork *evtAllocateMantraSelectionWork(s32 initialValue, s32 mode) {
    u32 allocation = sdfAllocGeneralBlock(MNU_MANTRA_SELECTION_WORK_BYTES);
    EvtMantraWork *work = sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, MNU_MANTRA_SELECTION_WORK_BYTES);
    work->allocation = allocation;
    work->capacity = 0xB0;
    work->entries = work->data;
    if (initialValue != 0) {
        func_0026D168(work, initialValue, mode);
    }
    return work;
}

s32 evtReleaseMantraSelectionWork(EvtMantraWork *work) {
    if (work != NULL) {
        sdfReleaseResourceAllocation(work->allocation);
    }
}

extern s32 mnuGetActiveMantraModelFlagState(void);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern s32 func_00316020(DatPartyRecord *, u16);
extern s32 func_00314B00(DatPartyRecord *, u16);
extern u32 ptyGetProfileRecordCap(u16);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);
extern s32 func_00315C68(u32, u32, DatPartyRecord *, u16, u32 *);
extern s32 func_00315FA0(u32, DatPartyRecord *, u16);
extern s32 func_0028F128(u16, s8);
extern s32 func_0026D590(u16, s32);
extern const char D_00425098[];

/* Populate mantra selection flags from rank, profile state and node rules. */
void func_0026D168(void *selection, s32 unitAddress, s32 mode) {
    EvtMantraWork *work = selection;
    DatPartyRecord *unit = (DatPartyRecord *)unitAddress;
    s32 rank;
    u32 selected;
    u16 *entry;
    EvtMantraNodePositionRecord *node;
    s32 i = 0;

    rank = mnuGetActiveMantraModelFlagState();
    evtPrintDeveloperConsoleMessage(D_00425098, rank);
    selected = scrGetSelectedScriptEntryId(unit);
    entry = work->entries;
    node = (EvtMantraNodePositionRecord *)mnuMantraNodePositionTable->recordsAddress;
    for (; i < (s32)mnuMantraNodePositionTable->recordCount; i++, node++, entry++) {
        *entry = 0;
        if (node->id != 0) {
            s32 rankMet;
            if (rank < ((s32)(node->packedHeader << 24) >> 28)) {
                rankMet = 0;
            } else {
                rankMet = 1;
            }
            if (func_00316020(unit, node->id) == 0) {
                goto unavailable;
            } else {
                if (selected == node->id) {
                    *entry |= 0x400;
                }
                if (func_00314B00(unit, node->id)) {
                    *entry |= 0x200;
                }
                if (ptyGetProfileRecordCap(node->id) == ptyGetProfileRecordValue(unit, node->id)) {
                    *entry |= 0x100;
                }
                if ((node->packedHeader & 0xF) == 3) {
                    if (func_00315C68(0, 4, unit, node->id, 0) == 0) {
                        goto unavailable;
                    }
                    if (mode != 0) {
                        if (func_0028F128(node->id, (s8)unit->unitId) == 0) {
                            goto unavailable;
                        }
                        *entry = (*entry & 0xFFF0) | 1;
                    } else {
                        *entry = (*entry & 0xFFF0) | 1;
                    }
                } else if (func_00315FA0(0, unit, node->id) != 0 || rankMet) {
                    switch ((s32)(node->packedHeader << 28) >> 28) {
                    case 2:
                        if (func_0026D590(node->id, mode)) {
                            *entry |= 0x800;
                        }
                        if (func_00315C68(1, 2, unit, node->id, 0)) {
                            if ((*entry >> 8) & 8) {
                                if (node->packedHeader & 0x100) {
                                    *entry = (*entry & 0xFFF0) | 0x101;
                                } else {
                                    *entry = (*entry & 0xFFF0) | 1;
                                }
                            } else {
                                *entry = (*entry & 0xFFF0) | 2;
                            }
                        } else {
                            *entry = (*entry & 0xFFF0) | 2;
                        }
                        if ((node->packedHeader & 0x100) && ((*entry >> 8) & 8)) {
                            *entry = (*entry & 0xFFF0) | 0x101;
                        }
                        break;
                    case 1:
                    case 4:
                        if (func_00315FA0(1, unit, node->id)) {
                            *entry = (*entry & 0xFFF0) | 1;
                        } else if (func_00315C68(1, 2, unit, node->id, 0)) {
                            *entry = (*entry & 0xFFF0) | 1;
                        } else if ((*entry >> 8) & 2) {
                            *entry = (*entry & 0xFFF0) | 1;
                        } else {
                            *entry = (*entry & 0xFFF0) | 2;
                        }
                        break;
                    }
                } else {
                    goto unavailable;
                }
            }
        }
        continue;
    unavailable:
        *entry = (*entry & 0xFFF0) | 3;
    }
}




extern u32 ptyGetProfileRecordCap(u16 scriptId);
/* The shared value provider owns a DatPartyRecord, not an integer address. */
extern u32 ptyGetProfileRecordValue(DatPartyRecord *work, u16 scriptId);

/* 1 when some active party member other than unit `skipId` has `scriptId` at its profile record cap. */
s32 ptyAnyActivePartyMemberAtProfileCap(u16 scriptId, u16 skipId) {
    s32 i;

    for (i = 0; i < EVT_PARTY_SLOT_COUNT; i++) {
        DatPartyRecord *slot;

        if (datGameState->party[i].flags & 1) {
            slot = &datGameState->party[i];
            if (slot->unitId != skipId) {
                if (ptyGetProfileRecordCap(scriptId) == ptyGetProfileRecordValue(slot, scriptId)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D590);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D710);

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026D7E8);

/* The sixth work stores the combined availability state for the five profiles. */
void func_0026D988(EvtMantraWork **selectionWorks) {
    EvtMantraNodePositionRecord *node;
    u16 *combinedEntry;
    s32 recordCount;
    s32 remaining;
    s32 profileIndex;

    node = (EvtMantraNodePositionRecord *)mnuMantraNodePositionTable->recordsAddress;
    combinedEntry = selectionWorks[5]->entries;
    recordCount = (s32)mnuGetMantraNodePositionRecordCount();
    if (recordCount <= 0) {
        return;
    }
    remaining = recordCount;
    do {
        if (node->id != 0) {
            profileIndex = 0;
            while (selectionWorks[profileIndex] != NULL && profileIndex < 5) {
                u16 *profileEntries = selectionWorks[profileIndex]->entries;
                if ((u16)((profileEntries[node->id] >> 8) & 1)) {
                    *combinedEntry = (*combinedEntry & 0xFF0F) | 0x10;
                    break;
                }
                if (((*combinedEntry & 0xF0) >> 4) != 1) {
                    *combinedEntry = (*combinedEntry & 0xFF0F) | 0x20;
                }
                profileIndex++;
            }
        }
        remaining--;
        combinedEntry++;
        node++;
    } while (remaining != 0);
}

extern s32 prfGetIndexedProfileByte(s32, s32);
/* Scan the low-halfword entry id's five attributes into caller-owned summary words.
 * A nonzero summary[0] makes the next nonzero attribute report sentinel five.
 * Zero is treated as empty even after storing attribute index zero; neither
 * summary word is initialized here, and summary[1] keeps the latest value. */
void prfSummarizeNonzeroEntryAttributes(s32 entryId, s32 *summary) {
    s32 attributeIndex;
    s32 attributeValue;

    for (attributeIndex = 0; attributeIndex < PRF_ATTRIBUTE_SLOT_COUNT; attributeIndex++) {
        attributeValue = prfGetIndexedProfileByte(entryId & 0xFFFF, attributeIndex);
        if (attributeValue != 0) {
            summary[1] = attributeValue;
            if (summary[0] == 0) {
                summary[0] = attributeIndex;
            } else {
                summary[0] = PRF_ATTRIBUTE_MULTIPLE_SENTINEL;
                return;
            }
        }
    }
}

void func_0026DB20(void) {
}


INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026DB28);

/* Return whether bit 15 is set for the supplied entry. */
s32 func_0026DB48(u32 context, u8 entry) {
    return scrTestEntryFlag(context, entry, 0xf);
}

void func_0026DB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026DB70);

/* Call the existing flag routine with entry zero and selector zero; ignore its result. */
void func_0026DB90(u32 context) {
    scrTestEntryFlag(context, 0, 0);
}

void func_0026DBB0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026C1D0", func_0026DBB8);

/* Call the existing flag routine with entry zero and selector one; ignore its result. */
void func_0026DBD8(u32 context) {
    scrTestEntryFlag(context, 0, 1);
}

INCLUDE_RODATA(const s32, "game/code_0026C1D0", D_00425098);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowHandle);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowControlState);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowStateGate);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437888);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", evtMessageWindowOption);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspCapturedSoundMode);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437890);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", mnuMantraNodePositionTable);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", mnuMantraPanelPositionTable);

