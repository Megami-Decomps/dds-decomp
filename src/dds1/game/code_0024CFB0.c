#include "dsp_entry_api.h"
#include "scr_integer_parameter_api.h"
#include "scr.h"
#include "bf_flw0_header.h"
#include "dsp_message_window_api.h"
#include "dsp_channel.h"
#include "scr_integer_return_value_api.h"
#include "dsp_active_api.h"
#include "evt_message_window_api.h"
#include "evt_world_object_api.h"
#include "kwln_task_state.h"
#include "kwln_task_state_query.h"
#include "itf_mes_window.h"
#include "itf_mes_panel_mode.h"
#include "itf_mes_entry_start_api.h"
#include "mnu.h"
#include "mnu_list.h"
#include "mnu_terminal_work.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "kwln_fade.h"
#include "dat_state.h"
#include "evt_world.h"
#include "kwln_task_lifecycle.h"
#include "dds3obj.h"
#include "sdf_texture_file.h"
#include "eff_random.h"
#include "eff_transform.h"
#include "eff_light.h"
#include "mdl_flag_api.h"
#include "mnu_event_flag_entries.h"
#include "eff_event_draw.h"

struct sdfTexHandle_t;
struct MenuTerminalWork;
extern void mnuTerminalSelectResourceBank(struct MenuTerminalWork *);
extern void mnuApplyFadeTrackMode(s32, struct MenuTerminalWork *);

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

extern s32 gMsgID;

extern u8 gMsgFlag;

extern s8 gMsgInvFlag;

extern s8 gSelMsgPos;

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



extern u32 fclWorkValue[];

extern void mnuDrawTerminalEffectIfAvailable(s32 state);


#include "dds3_world_secondary.h"
extern void evtCopyEntryStringToActiveWindow(s32 slotIndex, const char *sourceText);
extern void itfMesMngSetPhase(s32 windowIndex, s32 status);
extern void itfMesMngSetSelPhase(s32 windowIndex, s32 status);
extern MenuPopupEntry mnuTerminalPopupEntries[];
extern void itfMesMngRelease(ItfMesWindowIndex window);
extern u8 *D_003BAA70;
extern u8 *D_003BAA74;
extern u8 *D_003BAA78;
extern u8 *D_003BAA84;

extern MnuSceneFlagEntry mnuSceneFlagEventEntries[MNU_SCENE_FLAG_ENTRY_COUNT];
extern MnuPartyFlagEntry mnuPartyFlagEventEntries[MNU_PARTY_FLAG_ENTRY_COUNT];


/* Recreate the message window from the terminal's loaded message resource. */
void evtCloseDisplayChannelAndEnsureMessageWindow(struct MenuTerminalWork *work) {
    fclDestroyMessage();
    fclInitMessage(work->messageResources.messageData);
}

/* Start the first eligible special, scene, or active-party flag event.
 * Only panel kinds zero/one are accepted. Mark the handled flag immediately;
 * this is not a wait for dialogue completion, and dispatch results are ignored. */
s32 dspStartFlagEvent(struct MenuTerminalWork *work) {
    s32 panelKind = work->mode;
    s32 index;
    datUnitWork_t *slot;

    if (panelKind < 2) {
        if (panelKind >= 0) {
            if (evtBitCheck(DSP_FLAG_EVENT_REQUIRED) != 0 && evtBitCheck(DSP_FLAG_EVENT_HANDLED) == 0) {
                dspSetActive(1);
                fclStartMessage(DSP_FLAG_EVENT_SPECIAL_ENTRY);
                evtBitOn(DSP_FLAG_EVENT_HANDLED);
                return 1;
            }
            for (index = 0; index < sizeof(mnuSceneFlagEventEntries) / sizeof(mnuSceneFlagEventEntries[0]); index++) {
                if (evtBitCheck(mnuSceneFlagEventEntries[index].requiredFlag) != 0 && evtBitCheck(mnuSceneFlagEventEntries[index].handledFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(work);
                    dspSetActive(1);
                    evtCopyEntryStringToActiveWindow(0, (const void *)(D_003BAA84 + mnuSceneFlagEventEntries[index].areaIndex * DSP_AREA_TEXT_BYTES));
                    evtCopyEntryStringToActiveWindow(1, (const void *)(D_003BAA78 + mnuSceneFlagEventEntries[index].nameIndex * DSP_NAME_TEXT_BYTES));
                    evtCopyEntryStringToActiveWindow(2, (const void *)(D_003BAA74 + mnuSceneFlagEventEntries[index].dialogIndex * DSP_DIALOG_TEXT_BYTES));
                    fclStartMessage(DSP_FLAG_EVENT_SCENE_ENTRY);
                    evtBitOn(mnuSceneFlagEventEntries[index].handledFlag);
                    return 1;
                }
            }
            for (index = 0; index < EVT_PARTY_SLOT_COUNT; index++) {
                slot = &DDS3_GBWK->party[index];
                if ((slot->flags & 1) != 0 && evtBitCheck(mnuPartyFlagEventEntries[slot->unitId].requiredFlag) != 0
                    && evtBitCheck(mnuPartyFlagEventEntries[slot->unitId].handledFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(work);
                    dspSetActive(1);
                    evtCopyEntryStringToActiveWindow(0, (const void *)(D_003BAA70 + slot->unitId * DSP_DIALOG_TEXT_BYTES));
                    fclStartMessage(DSP_FLAG_EVENT_PARTY_ENTRY);
                    evtBitOn(mnuPartyFlagEventEntries[slot->unitId].handledFlag);
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Initialize the current terminal task's resource bank and mode-zero fade track. */
s32 mnuPrepareTerminalPanelState(dds3ProcessID_t *task) {
    struct MenuTerminalWork *work = (struct MenuTerminalWork *)dds3GetProcessData(task);

    mnuTerminalSelectResourceBank(work);
    mnuApplyFadeTrackMode(0, work);
    return 1;
}

/* Registered terminal panel leave hook: no side effects; returns one. */
u32 mnuTerminalPanelLeaveNoOp(void) {
    return 1;
}

/* Propagate the panel worker's nonzero result before considering a flag-event popup.
 * When its popup slot and message control are idle, fall back if no event starts. */
s32 dspUpdateFlagEvent(dds3ProcessID_t *request) {
    s32 workAddress = dds3GetProcessData(request);
    struct MenuTerminalWork *work = (struct MenuTerminalWork *)(u32)workAddress;
    s32 *popupSlot = &work->popupState;
    s32 result = menuRunPanel((void *)workAddress, 0, (void *)request);
    if (result != 0) {
        return result;
    }
    if (*popupSlot == 0) {
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            if (dspStartFlagEvent(work) == 0) {
                mnuSetPopupEntryFlagged(popupSlot, mnuTerminalPopupEntries);
            }
        }
    }
    return 0;
}

/* Prepare the current terminal work and dispatch panel mode one. */
s32 mnuDispatchTerminalPanel(dds3ProcessID_t *request) {
    s32 workAddress = dds3GetProcessData(request);
    mnuDrawTerminalEffectIfAvailable(workAddress);
    return menuRunPanel((void *)workAddress, 1, (void *)request);
}

/* Request the message-window transition before dispatching terminal panel mode two. */
s32 mnuDispatchTerminalPanelExit(dds3ProcessID_t *request) {
    s32 workAddress = dds3GetProcessData(request);
    dspRequestMessageWindowModeOne();
    return menuRunPanel((void *)workAddress, 2, (void *)request);
}

void evtClearActiveFlag(s32 flagIndex);

/* Reset display slots for a zero exit marker, otherwise start the existing fade-in.
 * The active-flag call's index is the zero the fade's arguments leave in a0. */
u32 mnuApplyTerminalExitFadeOrReset(dds3ProcessID_t *task) {
    struct MenuTerminalWork *work;

    work = (struct MenuTerminalWork *)(u32)dds3GetProcessData(task);
    if (work->reduced == 0) {
        evtClearActiveFlag(0);
        evtSetBoundedDisplayValue(0, 1);
        evtSetBoundedDisplayValue(1, 0);
    }
    else {
        dds3FadeOut(0, 0, 0, 0xf);
    }
    return 1;
}

/* Select the terminal's mode-one fade track before starting the black fade-out. */
s32 mnuStartTerminalPanelFadeOut(dds3ProcessID_t *task) {
    struct MenuTerminalWork *work = (struct MenuTerminalWork *)dds3GetProcessData(task);

    mnuApplyFadeTrackMode(1, work);
    dds3FadeIn(0, 0, 0, 0);
    return 1;
}

extern s32 evtIsActiveFlagSet(s32 flagIndex);
extern char D_003AF710[];
extern char D_0036AE48[];

/* Propagate the panel worker result; create a ready popup only for an empty slot.
 * Kind zero waits for the named task unless active flag zero overrides the wait;
 * other kinds wait for the fade to finish. */
s32 mnuUpdateTerminalReadyPopup(dds3ProcessID_t *request) {
    s32 workAddress = dds3GetProcessData(request);
    struct MenuTerminalWork *work = (struct MenuTerminalWork *)(u32)workAddress;
    s32 *popupSlot = &work->popupState;
    s32 result = menuRunPanel((void *)workAddress, 0, (void *)request);
    s32 ready;

    if (result != 0) {
        return result;
    }
    if (*popupSlot == 0) {
        if (work->mode == 0) {
            ready = dds3SearchProcessName(D_003AF710) == NULL;
            if (evtIsActiveFlagSet(0) != 0) {
                ready = 1;
            }
        } else {
            ready = dds3FadeCheck() == 0;
        }
        if (ready) {
            mnuSetPopupEntryFlagged(popupSlot, (MenuPopupEntry *)D_0036AE48);
        }
    }
    return 0;
}

/* Draw the available terminal effect, then dispatch panel mode one. */
s32 mnuDispatchTerminalPanelModeOne(dds3ProcessID_t *request) {
    s32 workAddress = dds3GetProcessData(request);
    mnuDrawTerminalEffectIfAvailable(workAddress);
    return menuRunPanel((void *)workAddress, 1, (void *)request);
}

/* Dispatch the current work in terminal panel mode two. */
s32 mnuDispatchTerminalPanelModeTwo(dds3ProcessID_t *request) {
    s32 workAddress = dds3GetProcessData(request);

    return menuRunPanel((void *)workAddress, 2, (void *)request);
}

/* Clear the terminal task's stored selection word and report completion. */
u32 mnuClearTerminalPanelSelection(dds3ProcessID_t *task) {
    s32 workAddress;

    workAddress = dds3GetProcessData(task);
    *(u32 *)(workAddress + 0x90) = 0;
    return 1;
}

struct MenuTerminalWork;
extern void mnuReleaseWorkResources(struct MenuTerminalWork *work);
extern void mnuTerminalBuildMenus(struct MenuTerminalWork *host);

/* Release the current task's menu resources before rebuilding its terminal menus. */
s32 mnuRebuildTerminalMenuResources(dds3ProcessID_t *task) {
    struct MenuTerminalWork *work = (struct MenuTerminalWork *)dds3GetProcessData(task);

    mnuReleaseWorkResources(work);
    mnuTerminalBuildMenus(work);
    return 1;
}

/* Registered terminal rebuild start hook: no side effects; returns zero. */
u32 mnuTerminalRebuildStartNoOp(void) {
    return 0;
}

/* Registered terminal rebuild update hook: no side effects; returns zero. */
u32 mnuTerminalRebuildUpdateNoOp(void) {
    return 0;
}

/* Registered terminal rebuild finish hook: no side effects; returns zero. */
u32 mnuTerminalRebuildFinishNoOp(void) {
    return 0;
}

u32 func_0024D608(void) {
    return 0xffffffff;
}

/* Copy the source node's nested value only on a successful lookup.
 * The destination and both nested data pointers are caller-owned. */
s32 evtCopyWorldObjectEntryValue(const char *objectName, s32 destination) {
    dds3Basic_t *source;
    dds3Basic_t *destinationNode;

    source = dds3SceneBasicSearchNameAndType((dds3Basic_t *)dds3GetControlScene(), EFF_WORLD_KIND_LIGHT,
                                                  objectName);
    if (source != NULL) {
        destinationNode = (dds3Basic_t *)destination;
        *(s32 *)((u8 *)destinationNode->data + 0x80) =
            (s32)((EffLightData *)source->data)->buffer;
        return 1;
    }
    return 0;
}

/* Recreate the secondary world selection only when its packed key changes.
 * Preserve the signed shift/add rather than treating this as an unsigned OR. */
void fclLoadStage(s32 high, s32 low) {
    Dds3AreaRoomKey areaRoomKey = (high << 16) + low;
    if (dds3GetMapNo(dds3GetControlScene()) != areaRoomKey) {
        evtLoadArea(high, low);
    }
}

extern void dds3DistBlurOff(s32 duration);
extern void dds3SetBlur(s32 duration, s32 alpha, s32 scale);
extern void dds3MultiBlurOff(s32 duration);

/* Reset draw offsets and the five enabled screen-effect modes. */
void fclFilterOFF(void) {
    dds3SetBlur(0, 0, 0);
    dds3MultiBlurOff(0);
    dds3DistBlurOff(0);
    Eve_BlurOFF();
    Eve_PartialMultiBlurOFF();
    Eve_PartialMultiFilterOFF();
    Eve_DistortionBlurOFF();
    Eve_FilterOFF();
}

extern s32 evtCommand_FIELD_CLS(void);

/* Shut down the stage before clearing its draw transitions. */
void evtShutdownStageAndResetDrawTransitions(void) {
    evtCommand_FIELD_CLS();
    fclFilterOFF();
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
void fclInitPanelPos(s32 firstWord, s32 secondWord, s32 thirdWord, s32 firstHalfword, s32 secondHalfword, s32 lastWord, QuadRecord *record) {
    record->unk00 = firstWord;
    record->unk04 = secondWord;
    record->unk08 = thirdWord;
    record->unk0C = firstHalfword;
    record->unk0E = secondHalfword;
    record->unk10 = lastWord;
}

/* Destroy a nonzero, registered task. The declared return value is not set. */
s32 fclScriptForcedEnd(dds3ProcessID_t *task) {
    if (task != 0) {
        if (dds3GetProcessStat(task) != KWLN_TASK_DETACHED) {
            dds3KillProcess(task, 0);
        }
    }
}

extern dds3ProcessID_t *scrStartScript2(s32 priority, const BfFlw0Header *script, ScrProcedureIndex procedureIndex);

/* Replace an optional task slot, clear active flag zero, and return the new task.
 * Creation is still attempted if no slot is supplied. */
s32 fclScriptStart(BfFlw0Header *script, s32 processValue, s32 *taskSlot) {
    dds3ProcessID_t *task;

    if (taskSlot != 0) {
        fclScriptForcedEnd((dds3ProcessID_t *)*taskSlot);
    }
    task = scrStartScript2(EVT_SCRIPT_PROCESS_PRIORITY, script, processValue);
    evtClearActiveFlag(0);
    if (taskSlot != 0) {
        *taskSlot = (s32)task;
    }
    return (s32)task;
}

/* Emit active game-entry indices in ascending order, excluding entry zero.
 * DDS1 scans 1..191; the caller must provide enough trailing halfwords. */
void evtCollectActiveGameIndices(ActiveList *list) {
    s32 index;
    list->entryCount = 0;
    for (index = 1; index < EVT_ACTIVE_ENTRY_LIMIT; index++) {
        if (DDS3_GBWK->inventory.counts[index] != 0) {
            s32 outputIndex = list->entryCount++;
            list->entryIndices[outputIndex] = index;
        }
    }
}

/* Three-way ascending comparison of the two unsigned bytes. */
s32 cmpItemOrder(u8 *left, u8 *right) {
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
void shuffleIndices(u8 *buffer, u32 length, s32 swapCount) {
    s32 swapIndex;
    for (swapIndex = 0; swapIndex < swapCount; swapIndex++) {
        u8 *first = buffer + dds3GetRandA(0) % length;
        u8 *second = buffer + dds3GetRandA(0) % length;
        u8 value = *first;
        *first = *second;
        *second = value;
    }
}

/* Store the resource handle in word zero and the loader's data address in word one. */
void evtLoadResourcePair(const char *resourcePath, EvtResourcePair *record) {
    record->handle = sdfDevLoadFileEx(resourcePath, (u32 *)&record->messageData, 0);
}

/* Release the handle without clearing either word of the caller's record. */
void evtReleaseResourcePairHandle(EvtResourcePair *record) {
    sdfMemFreeDirect(record->handle);
}

struct ItfMesSub;
extern ItfMesWindowIndex itfMesMngInitialize(struct ItfMesSub *sub);

/* Create the singleton message window only when its handle is negative. */
s32 fclInitMessage(struct ItfMesSub *sub) {
    if (gMsgID < 0) {
        gMsgID = itfMesMngInitialize(sub);
        itfMesMngChangeWindowType(gMsgID, ITF_MES_PANEL_MODE_WIDE, 0);
        return 1;
    }
    return 0;
}

/* Apply page mode zero and the caller's opaque page value to an open window. */
EvtMessageWindowPageValueResult evtSetMessageWindowPageValue(s32 pageValue) {
    if (gMsgID < 0) {
        return EVT_MESSAGE_WINDOW_PAGE_VALUE_NO_WINDOW;
    }
    itfMesMngChangeWindowType(gMsgID, ITF_MES_PANEL_MODE_STANDARD, pageValue);
    return EVT_MESSAGE_WINDOW_PAGE_VALUE_UPDATED;
}

/* Start an entry, mark the first panel value pending, and enable control state one. */
s32 fclStartMessage(s32 entry) {
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngMarkBitStat(gMsgID, DSP_WINDOW_ENTRY_FLAG);
    itfMesStartEntry(gMsgID, entry, 0);
    itfMesMngSetSelPhase(gMsgID, -1);
    gMsgFlag = DSP_WINDOW_CONTROL_ACTIVE;
    return 1;
}

extern s32 dspReadWindowSecondPanelValue(void);

/* Store an option-list entry for deferred construction and snapshot the
 * panel's second halfword. */
s32 evtStoreValueAndCaptureWindowPanelValue(ItfMesEntryIndex entryIndex) {
    if (gMsgID < 0) {
        return 0;
    }
    D_003BC410 = entryIndex;
    dspCapturedSoundMode = dspReadWindowSecondPanelValue();
    return 1;
}

/* Update the byte-sized option only while the singleton window is open. */
void evtSetMessageWindowOptionWhenOpen(ItfMesOptionBitIndex selectedBitIndex) {
    if (gMsgID >= 0) {
        gSelMsgPos = selectedBitIndex;
    }
}

/* Return the retained option byte, even when the window is closed. */
s8 fclGetCurrentSelMessagePos(void) {
    return gSelMsgPos;
}

extern s32 itfMesMngGetSelectNo(s32 windowIndex);

/* Read the second panel halfword, or -1 when no message window exists.
 * Despite the inherited name, this is not a sound-driver query. */
s32 dspReadWindowSecondPanelValue(void) {
    if (gMsgID < 0) {
        return -1;
    }
    return itfMesMngGetSelectNo(gMsgID);
}

/* Return the captured signed-byte panel value without querying the live window. */
s8 evtGetCapturedWindowPanelValue(void) {
    return dspCapturedSoundMode;
}

extern void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock);

/* Reset an open window, optionally notify it first, and report whether it existed.
 * Cleanup leaves the singleton handle intact; fclDestroyMessage destroys it. */
u32 evtCleanupMessageWindow(s32 notify) {
    u32 cleaned;

    cleaned = 0;
    if (-1 < gMsgID) {
        itfMesMngSetPhase(gMsgID, 0);
        if (notify != 0) {
            itfMesMngMsgWndCls(gMsgID);
        }
        itfMesMngRequestMessageRelease(gMsgID, 0);
        dspSetActive(1);
        gMsgFlag = 0;
        cleaned = 1;
    }
    return cleaned;
}

/* Finish/reset the singleton window with notification and return whether it existed. */
u32 evtFinishMessageWindowAndNotify(void) {
    return evtCleanupMessageWindow(1);
}

/* Destroy an existing singleton window and invalidate its handle and control gates. */
s32 fclDestroyMessage(void) {
    s32 window = gMsgID;
    if (window < 0) {
        return 0;
    }
    itfMesMngRelease(window);
    gMsgID = -1;
    gMsgFlag = 0;
    gMsgInvFlag = 0;
    return 1;
}

/* Return the visible signed control-state byte, or the query's no-active result. */
EvtMessageWindowControlResult fclChkMessage(void) {
    if (gMsgID < 0) {
        return EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL;
    }
    if (gMsgInvFlag != 0) {
        if ((s8)gMsgFlag == DSP_WINDOW_CONTROL_GATED) {
            return EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL;
        }
    }
    return (s8)gMsgFlag;
}

extern s32 itfMesMngGetSelPhase(s32 windowIndex);

/* Snapshot the second panel halfword only after the first halfword is nonnegative. */
s32 dspCaptureWindowSecondPanelValue(void) {
    if (gMsgID < 0) {
        return 0;
    }
    if (itfMesMngGetSelPhase(gMsgID) < 0) {
        return 0;
    }
    dspCapturedSoundMode = dspReadWindowSecondPanelValue();
    return 1;
}

extern s8 itfMesMngGetPhase(s32 windowIndex);
extern s32 itfMesMngSetSelectDefKey(s32 window, s32 id, s32 value);
extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

/* Advance the singleton window through its active, gated and cleanup phases. */
void dspAdvanceMessageWindowControl(s32 notify) {
    s32 handle = gMsgID;
    if (handle < 0) {
        return;
    }
    switch ((s8)gMsgFlag) {
    case 0:
        return;
    case DSP_WINDOW_CONTROL_ACTIVE:
        if (itfMesMngGetPhase(handle) < 0) {
            gMsgFlag = DSP_WINDOW_CONTROL_GATED;
        }
        return;
    case DSP_WINDOW_CONTROL_GATED:
        if (D_003BC410 >= 0) {
            itfMesMngSetSelectDefKey(handle, 3, 1);
            itfMesMngSetSelect(gMsgID, gSelMsgPos);
            itfMesBuildOptionList(gMsgID, D_003BC410);
            D_003BC410 = -1;
        }
        if (gMsgInvFlag != 0) {
            return;
        }
        if (dspCaptureWindowSecondPanelValue() != 0) {
            return;
        }
        gMsgFlag = 3;
        return;
    case 3:
        evtCleanupMessageWindow(notify);
        return;
    default:
        return;
    }
}

/* Request mode one from the existing message-window worker. */
void dspRequestMessageWindowModeOne(void) {
    dspAdvanceMessageWindowControl(1);
}

/* Copy a string address into a window table slot; neither argument is an item id. */
extern void itfMesMngSetVarImmediate(s32 window, u32 slotIndex, const char *sourceText);

void evtCopyEntryStringToActiveWindow(s32 slotIndex, const char *sourceText) {
    itfMesMngSetVarImmediate(gMsgID, slotIndex, sourceText);
}

/* Return the gate byte independently of the singleton window's existence. */
s8 dspGetWindowStateGate(void) {
    return gMsgInvFlag;
}

/* Enable clears both disable flags and restores panel/control state one.
 * Disable sets the gate and flags, but deliberately leaves the control byte alone.
 * Callers must already have a valid window. */
void dspSetActive(s32 enabled) {
    if (enabled) {
        itfMesMngClearBitStat(gMsgID, DSP_WINDOW_DISABLE_FLAG_MAIN);
        itfMesMngClearBitStat(gMsgID, DSP_WINDOW_DISABLE_FLAG_SECONDARY);
        gMsgInvFlag = 0;
        itfMesMngSetPhase(gMsgID, 1);
        gMsgFlag = DSP_WINDOW_CONTROL_ACTIVE;
    } else {
        itfMesMngMarkBitStat(gMsgID, DSP_WINDOW_DISABLE_FLAG_MAIN);
        itfMesMngMarkBitStat(gMsgID, DSP_WINDOW_DISABLE_FLAG_SECONDARY);
        gMsgInvFlag = 1;
    }
}

extern void itfMesMngSetMesPosition(s32 window, s32 x, s32 y);
extern void itfMesMngSetOffsetY1(s32 windowIndex, s32 topDelta);

/* Convert x/y to the window's 16-by-8 coordinate units and set its panel offset.
 * The original signed shifts are retained, including their input assumptions. */
void evtMoveMessageWindowWithPanelOffset(s32 x, s32 y) {
    itfMesMngSetMesPosition(gMsgID, x << 4, y << 3);
    itfMesMngSetOffsetY1(gMsgID, -((DSP_WINDOW_PANEL_BASE_Y - y) << 3));
}

/* Recognize all three scheduler queues. Preserve the repeated queries. */
s32 fclChkMessageDraw(dds3ProcessID_t *task) {
    if (dds3GetProcessStat(task) == KWLN_TASK_DELAYED_START) {
        return 1;
    }
    if (dds3GetProcessStat(task) == KWLN_TASK_ACTIVE) {
        return 1;
    }
    return dds3GetProcessStat(task) == KWLN_TASK_DESTROY_PENDING;
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
s32 evtSetBoundedDisplayValue(s32 displayValueIndex, s32 displayValue) {
    if (displayValueIndex >= EVT_DISPLAY_VALUE_COUNT) {
        return 0;
    }
    fclWorkValue[displayValueIndex] = displayValue;
    return 1;
}

/* Clamp only the upper side to the last display slot; negative indices pass through. */
u32 evtGetBoundedDisplayValue(s32 displayValueIndex) {
    displayValueIndex = (displayValueIndex < EVT_DISPLAY_VALUE_COUNT) ? displayValueIndex : EVT_LAST_DISPLAY_VALUE;
    return fclWorkValue[displayValueIndex];
}

/* Script opcode: set the supplied active-entry flag without bounds checking. */
s32 evtOpSetActiveEntryFlag(void) {
    s32 flagIndex = scrGetIntPara(0);

    evtActiveEntryFlags.flags[flagIndex] = 1;
    return 1;
}

/* Script opcode: return a display-table value, clamping only its upper index.
 * It does not activate or test an active-entry flag. */
s32 evtOpReadDisplayValue(void) {
    s32 displayValueIndex = scrGetIntPara(0);
    if (displayValueIndex >= EVT_DISPLAY_VALUE_COUNT) {
        displayValueIndex = EVT_LAST_DISPLAY_VALUE;
    }
    scrSetIntReturnValue(fclWorkValue[displayValueIndex]);
    return 1;
}

/* Acquire the loaded data's texture reference, then release its temporary resource.
 * DDS1 passes the size output as a pointer, unlike DDS2's address-valued slot. */
struct sdfTexHandle_t *fclLoadTexture(const char *path) {
    u32 info[2];
    struct sdfMemHandle_t *allocation = sdfDevLoadFileEx(path, info, &info[1]);
    struct sdfTexHandle_t *texture = sdfTexSet((SdfTextureFileHeader *)(info[0]));

    sdfMemFreeDirect(allocation);
    return texture;
}

/* The outer panel owner is unresolved; its list pointer is shared by
 * the viewport flag and height consumers. */
typedef struct EvtListPanelRecord {
    u8 pad0[0x14];
    struct MenuList *list;
} EvtListPanelRecord;

extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);

/* Draw top/bottom viewport indicators; each flag selects its brighter color.
 * The two triangles retain their signed x-radius and y-offset calculations. */
INCLUDE_SDATA(const s32, "game/code_0024CFB0", gMsgID);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", gMsgFlag);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", gMsgInvFlag);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", D_003BC410);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", gSelMsgPos);

INCLUDE_SDATA(const s32, "game/code_0024CFB0", dspCapturedSoundMode);

void evtDrawListViewportIndicators(s32 x, s32 topY, s32 bottomY, s32 size, EvtListPanelRecord *record) {
    s32 coordinates[2][3];
    s32 xRadius = (size << 4) >> 1;
    s32 yOffset = size << 3;
    u32 flags = record->list->flags;
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

extern void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

void evtDrawListViewportPanel(s32 x, s32 y, s32 width, EvtListPanelRecord *record) {
    s32 height = mnuGetListViewportHeight(record->list) + 0x80;
    uiDrawUniformColorRect(x, y, 0, width, height, 0x30303040, 0x53);
    evtDrawListViewportIndicators(x + width - 0xA0, y, y + height, 8, record);
}

/* Draw a plain background rectangle with the shared panel color and command. */
void evtDrawPlainPanel(u32 x, u32 y, u32 width, u32 height) {
    uiDrawUniformColorRect(x, y, 0, width, height, EVT_PANEL_BACKGROUND_COLOR, EVT_PANEL_DRAW_COMMAND);
}
