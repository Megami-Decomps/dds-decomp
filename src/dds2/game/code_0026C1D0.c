#include "kwln.h"
#include "mnu_list.h"
#include "kwln_task_state.h"
#include "itf_mes_window.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

typedef struct SdfTex SdfTex;

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

extern s32 evtGetMessageWindowControlState(void);

extern s32 dspWindowHandle;

extern s8 dspWindowControlState;

extern void func_0026C900(void);

extern s32 D_00437888;

extern s8 dspCapturedSoundMode;

extern s8 evtMessageWindowOption;

extern s8 dspWindowStateGate;

extern u32 evtDisplayValues[];

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
        if (kwlnTaskGetRegisteredState((KwlnTask *)task) != 0) {
            kwlnTaskDestroyWithHierarchy((KwlnTask *)task, 0);
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

    handle = (u32)sdfReadNamedResource((const char *)(u32)resourceId, &record->unk04, 0);
    record->handle = handle;
}

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(EvtResourcePair *record) {
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(record->handle));
}

struct ItfMesSub;

extern s32 itfMesCreateWindow(struct ItfMesSub *);

/* Create the singleton message window only when its handle is negative.
 * The page setup remains unconditional after the allocation attempt. */
s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *definition) {
    if (dspWindowHandle < 0) {
        dspWindowHandle = itfMesCreateWindow(definition);
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

extern s8 itfPanelGetStatus(s32 index);

extern s32 func_001A4A10(s32 window, s32 first, s32 second);

extern void itfMesBuildOptionList(s32 window, s32 entryIndex);

/* Advance the singleton window through its active, gated and cleanup phases. */
void func_0026C7F8(s32 notify, s32 refresh) {
    s32 handle = dspWindowHandle;

    if (handle < 0) {
        return;
    }
    switch (dspWindowControlState) {
    case 0:
        return;
    case DSP_WINDOW_CONTROL_ACTIVE:
        if (itfPanelGetStatus(handle) < 0) {
            dspWindowControlState = DSP_WINDOW_CONTROL_GATED;
        }
        return;
    case DSP_WINDOW_CONTROL_GATED:
        if (D_00437888 >= 0) {
            if (refresh != 0) {
                func_001A4A10(handle, 3, 1);
                handle = dspWindowHandle;
            }
            itfMesCountClearBits(handle, evtMessageWindowOption);
            itfMesBuildOptionList(dspWindowHandle, D_00437888);
            D_00437888 = -1;
        }
        if (dspWindowStateGate != 0) {
            return;
        }
        if (dspCaptureWindowSecondPanelValue() != 0) {
            return;
        }
        dspWindowControlState = 3;
        return;
    case 3:
        evtCleanupMessageWindow(notify);
        return;
    default:
        return;
    }
}

/* Pass the supplied value to mode one of the existing message-window worker. */
void func_0026C8E8(u32 value) {
    func_0026C7F8(value, 1);
}

/* Request value one from the existing mode-one message-window worker. */
void func_0026C900(void) {
    func_0026C8E8(1);
}

/* Copy a string address into a window table slot; neither argument is an item id. */
extern void itfMesCopyStringToWindowTableSlot(s32, u32, const void *);

void evtCopyEntryStringToActiveWindow(s32 slotIndex, const void *sourceText) {
    itfMesCopyStringToWindowTableSlot(dspWindowHandle, slotIndex, sourceText);
}

/* Return the gate byte independently of the singleton window's existence. */
s8 dspGetWindowStateGate(void) {
    return dspWindowStateGate;
}

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

/* Recognize all three scheduler queues. Preserve the repeated queries. */
s32 evtIsTaskInActiveStates(s32 task) {
    if (kwlnTaskGetRegisteredState((KwlnTask *)task) == KWLN_TASK_DELAYED_START) {
        return 1;
    }
    if (kwlnTaskGetRegisteredState((KwlnTask *)task) == KWLN_TASK_ACTIVE) {
        return 1;
    }
    return kwlnTaskGetRegisteredState((KwlnTask *)task) == KWLN_TASK_DESTROY_PENDING;
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

extern SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress);

/* Acquire the loaded data's texture reference, then release its temporary resource.
 * DDS2 passes the size-output address as an integer, unlike DDS1's pointer slot. */
s32 evtLoadTextureFromResourcePath(u32 path) {
    u32 info[2];
    u32 allocation;
    s32 texture;

    allocation = sdfReadNamedResource((const char *)(u32)path, &info[0], (u32 *)&info[1]);
    texture = (s32)sdfTexAcquireResourceTexture((void *)info[0]);
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    return texture;
}

extern u32 D_00437890[];

extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);

/* The outer panel owner is unresolved; its list pointer is shared by
 * the viewport flag and height consumers. */
typedef struct {
    u8 pad0[0x18];
    struct MenuList *list;
} EvtPanelRecord;

/* Draw top/bottom viewport indicators; each flag selects its brighter color.
 * DDS2 copies its two colors from the data table rather than using DDS1 literals. */
void evtDrawListViewportIndicators(s32 x, s32 topY, s32 bottomY, s32 size, EvtPanelRecord *record) {
    s32 coordinates[2][3];
    s32 xRadius = (size << 4) >> 1;
    s32 yOffset = size << 3;
    u32 flags = record->list->flags;
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

extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, u32, s32);

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
    s32 height = mnuGetListViewportHeight(record->list) + 0x80;

    uiDrawUniformColorRect(x, y, 0, width, height, 0x30303040, 0x53);
    evtDrawListViewportIndicators(x + width - 0xA0, y, y + height, 8, record);
}

/* Draw a plain background rectangle with the shared panel color and command. */
void evtDrawPlainPanel(u32 x, u32 y, u32 width, u32 height) {
    uiDrawUniformColorRect(x, y, 0, width, height, EVT_PANEL_BACKGROUND_COLOR, EVT_PANEL_DRAW_COMMAND);
}
INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowHandle);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowControlState);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspWindowStateGate);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437888);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", evtMessageWindowOption);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", dspCapturedSoundMode);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", D_00437890);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", mnuMantraNodePositionTable);

INCLUDE_SDATA(const s32, "game/code_0026C1D0", mnuMantraPanelPositionTable);

