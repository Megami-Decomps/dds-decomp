#include "dsp_entry_api.h"
#include "evt_message_window_api.h"
#include "common.h"
#include "evt_world_object_api.h"
#include "file_pac.h"
#include "file_request_api.h"
#include "eff_resource_slots.h"
#include "sdf_pac_packet.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "itf_grid_text.h"
#include "fld_lmap_task.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"
#include "kwln_fade.h"
#include "sdf_counter_channel.h"

typedef struct SdfSlotSet SdfSlotSet;

extern s32 fldAdvanceLmapResourceLoad(LmapTaskState *);

extern void fldDrawSkyGradient(void);
extern void func_00137888(void);
extern void sdfAdjustCounterValueAndRedraw(s32);
extern void sdfDrawPositionedSlotImage(s32, s32, s32, s32, s32, s32, s32);
extern void mnuDrawAnimatedTransition(s32);
extern void fldDrawSelectedMapMarker(void);

extern u32 D_004388AC;

extern u32 D_004388B0;

extern u32 D_0043908C;

extern s32 D_00439090; /* fade timer: fldUpdateLocalMapCounterSelection tests >= 31 with slti */



extern char fldLocalMapTaskName[]; /* "LmapMain" */

extern void fldShutdownLmapResources(void);


typedef GridTextListItem LmapNode;
typedef GridTextWidget LmapList;

extern s32 sdfGridSeekFirstNode(GridTextWidget *);
extern s32 sdfGridSeekLastNode(GridTextWidget *);
extern LmapNode *fldLmapAdvanceWindowStart(LmapList *);
extern u32 itfGetGridListLinkFlags(LmapList *);
extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);
extern LmapNode *fldLmapExpandWindowBackward(LmapList *);
extern s32 fldLocalMapTrackSlotFromMode(s32);
extern s32 fldLmapTaskUpdate(KwlnTask *);
extern s32 D_00438890;
extern void sndReleaseTrackHandle(u32);
extern void sdfReleaseAllSpriteSlots(void);
extern void sdfDestroyActiveCounterRuntime(void);
extern void fldReleaseMapRequestQueues(void);
extern void fldReleaseCameraColorEffect(void);
extern void sdfDisableFlagListViewTargetRelativeMode(void);
extern void evtSetSolarOverlayFullyTransparent(void);
extern s32 dspCloseChannel(void);
extern void evtDestroySecondaryWorldNode(void);
extern SdfMemBlock *D_004388A4;
extern void sprintf(char *, char *, ...);
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void evtStoreValueAndCaptureWindowPanelValue(s32);
extern char D_004388A0[];
extern void dspRequestMessageWindowModeOne(void);

extern void dds3SetWorldObject(void *);
#include "dds3_world_secondary.h"
extern void fldCreateLocalMapCamera(void);
extern s32 fldPackLocalMapFlagStates(void);
extern s32 fldCountMaskBitsBeforeOrdinal(s32, s32);
extern s32 sdfCreateMaskedCounterChannels(s32, s32);
extern s32 func_0030B880(s32);
extern void sdfInitializeMapCounterSelection(s32, s32);
extern void fldCreateMapRequestQueues(void);
extern void evtCreateMessageWindowIfMissing(s32);
extern void evtSetSolarOverlayFullyVisible(void);
extern void fldApplyLightSetIndex(s32);
extern void fldInitializeCameraColorResource(void);
extern void sdfEnableFlagListViewTargetRelativeMode(void);
extern void sndStartTrackDefault(s32);
extern void sdfCounterInitializeDisplayAnimation(void);
extern u32 kwlnDrawControlFlags;
extern u32 D_00435CBC;
extern s32 D_00438894;
extern u32 D_004388A8;
extern void fldInitializeLmapTaskVariant(LmapTaskState *task);

/* Move the visible start forward without moving the cursor. */
LmapNode *fldLmapAdvanceWindowStart(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *windowStart = list->firstVisible;

    if (cursor == list->tail) {
        return cursor;
    }
    windowStart = windowStart->next;
    if (windowStart == NULL) {
        return cursor;
    }
    list->cursorRow--;
    list->firstVisible = windowStart;
    return cursor;
}

/* Move the visible start backward only when a full forward span is available. */
LmapNode *fldLmapExpandWindowBackward(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *windowStart = list->firstVisible;
    LmapNode *scanNode;
    s32 stepIndex;

    if (cursor == list->head) {
        return cursor;
    }
    scanNode = windowStart;
    for (stepIndex = 0; stepIndex < list->rows; stepIndex++) {
        if (scanNode == NULL) {
            return cursor;
        }
        scanNode = scanNode->next;
    }
    windowStart = windowStart->previous;
    list->firstVisible = windowStart;
    list->cursorRow++;
    return cursor;
}

/* Advance the unlocked cursor, wrapping at the range end and following its window. */
LmapNode *fldLmapAdvanceCursor(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *nextNode;

    if (cursor == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cursor;
    }
    if (cursor == list->tail) {
        sdfGridSeekFirstNode(list);
        return list->selected;
    }
    nextNode = cursor->next;
    if (nextNode == 0) {
        return cursor;
    }
    cursor = nextNode;
    list->selected = cursor;
    list->cursorRow++;
    if (list->cursorRow >= list->rows - 1) {
        cursor = fldLmapAdvanceWindowStart(list);
    }
    return cursor;
}

/* Rewind the unlocked cursor, wrapping at the range start and following its window. */
LmapNode *fldLmapRewindCursor(LmapList *list) {
    LmapNode *cursor = list->selected;
    LmapNode *previousNode;

    if (cursor == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cursor;
    }
    if (cursor == list->head) {
        sdfGridSeekLastNode(list);
        return list->selected;
    }
    previousNode = cursor->previous;
    if (previousNode == 0) {
        return cursor;
    }
    cursor = previousNode;
    list->selected = cursor;
    list->cursorRow--;
    if (list->cursorRow <= 0) {
        cursor = fldLmapExpandWindowBackward(list);
    }
    return cursor;
}

/* Jump forward by twice the window capacity minus the cursor's window offset. */
LmapNode *fldLmapAdvanceThroughWindow(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->rows * 2 - list->cursorRow;
    for (i = 0; i < steps; i++) {
        result = fldLmapAdvanceCursor(list);
    }
    return result;
}

/* Jump backward by the window capacity plus the cursor's window offset. */
LmapNode *fldLmapRewindThroughWindow(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->rows + list->cursorRow;
    for (i = 0; i < steps; i++) {
        result = fldLmapRewindCursor(list);
    }
    return result;
}

void fldLocalMapDrawScrollIndicators(s32 offsetX, s32 offsetY, s32 z, u32 color,
                   LmapList *list, s32 channel) {
    s32 coordinates[2][3];
    u32 flags = itfGetGridListLinkFlags(list);
    s32 halfWidth = list->width / 2;
    s32 baseX = list->x + offsetX;
    coordinates[0][0] = baseX + (halfWidth << 4);
    coordinates[0][1] = baseX + ((halfWidth - 4) << 4);
    coordinates[0][2] = baseX + ((halfWidth + 4) << 4);
    if (flags & 1) {
        s32 baseY = list->y + offsetY;
        coordinates[1][0] = baseY - 64;
        coordinates[1][2] = coordinates[1][1] = baseY - 24;
        uiDrawUniformRgbRange(coordinates[0], coordinates[1], z, color, channel);
    }
    if (flags & 2) {
        s32 height = list->height;
        s32 baseY = list->y + offsetY;
        coordinates[1][0] = baseY + ((height + 8) << 3);
        coordinates[1][2] = coordinates[1][1] = baseY + ((height + 4) << 3);
        uiDrawUniformRgbRange(coordinates[0], coordinates[1], z, color, channel);
    }
}
extern s32 D_00438888;
extern LmapList *D_00439088;
extern void fldLmapSubmitScaledSpritePacket(s32, s32, s32, s32, s32, s32, s32, s32);
extern void itfDrawGridTextRows(s32, s32, s32, LmapList *, s32);

void fldLmapDrawListTree(s32 x, s32 y, s32 z, LmapList *list, s32 channel) {
    u32 color;

    D_00438888++;
    color = (list->flags & 2) ? 0x80608060 : 0x80606060;
    if (!(list->flags & 0x40)) {
        fldLmapSubmitScaledSpritePacket(list->x + x, list->y + y, z, list->width,
                                       list->height, 0x40000000, color, channel);
        if (list->flags & 2) {
            fldLocalMapDrawScrollIndicators(x, y, z, 0x60806080, list, channel);
        }
    }
    itfDrawGridTextRows(x, y, z, list, channel);
    if (list->onDraw != 0) {
        list->onDraw(list->x + x, list->y + y, z, list, channel);
    }
    if ((list->flags & 0x10) && D_00438888 >= 2) {
        list->flags &= ~1;
        list->flags &= ~2;
        D_00439088->flags |= 2;
        D_00439088->flags &= ~1;
        list->flags &= ~0x10;
    } else {
        D_00439088 = list;
        if (list->flags & 0x21) {
            if (list->selected->child != 0) {
                fldLmapDrawListTree(x + ((list->width + 8) << 4), y, z, list->selected->child, channel);
            }
        } else if ((list->flags & 2) && list->onSelect != 0) {
            list->onSelect(list);
        }
    }
    D_00438888--;
}



extern SdfPoolNode kwlnDrawSurfaces[];
#include "sdf_packet_alloc.h"
extern void *sdfCreateBorderedRectanglePacket();

/* Build one positioned SIF command and submit it on the requested draw surface. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 argument, s32 index, const char *command, s32 surfaceIndex) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfPoolNode *drawSurface;
    SifCommand packetHeader;

    sdfInitPacketList(packetList);
    sdfPktInit(&packetHeader, x + 0x7000, y + 0x7900, argument, index);
    sdfAppendPacket(packetList, (u32)sdfFormatSifPacket(&packetHeader, command));
    drawSurface = &kwlnDrawSurfaces[surfaceIndex];
    drawSurface->append(drawSurface, packetList);
}

/* Build an untextured rectangle with a separate outline color. */
void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height, s32 fillColor, s32 borderColor, s32 surfaceIndex) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfPoolNode *drawSurface;

    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(x + 0x7000, y + 0x7900, z, width * 16, height * 8, fillColor, borderColor));
    drawSurface = &kwlnDrawSurfaces[surfaceIndex];
    drawSurface->append(drawSurface, packetList);
}

extern u8 fldUpdateLmapResourceLoad(LmapTaskState *);
extern void fldInitializeLocalMapScene(void);
extern s32 fldUpdateLocalMapCounterSelection(void);
extern void fldDrawLocalMapOverlay(void);

s32 fldLmapTaskUpdate(KwlnTask *task) {
    LmapTaskState *state = (LmapTaskState *)kwlnTaskGetUserValue(task);
    s32 phase = state->phase;
    s32 result;

    D_0043888C = state;
    switch (phase) {
    case 0:
        if (fldUpdateLmapResourceLoad(state)) {
            D_0043888C->phase = 1;
        }
        break;
    case 1:
        fldInitializeLocalMapScene();
        D_0043888C->phase = 2;
        break;
    case 2:
        result = fldUpdateLocalMapCounterSelection();
        if (result == -1) {
            return result;
        }
        if (result == phase) {
            return 0;
        }
        fldDrawLocalMapOverlay();
        break;
    }
    return 0;
}

void fldStartLmapTask(s32 mode) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(0x88);
    LmapTaskState *taskData = (LmapTaskState *)sdfMemoryGetBlockAddress(allocation);

    memset(taskData, 0, 0x88);
    if (mode != 0) {
        D_00438890 = fldLocalMapTrackSlotFromMode(mode);
    } else {
        D_00438890 = 1;
    }
    fldInitializeLmapTaskVariant(taskData);
    kwlnTaskCreate(fldLocalMapTaskName, 0x2AF8, 0, 0, fldLmapTaskUpdate, 0, (u32)taskData);
}

void fldStopLmapTask(void) {
    fldShutdownLmapResources();
    kwlnTaskDestroyWithHierarchyByName(fldLocalMapTaskName, 1);
}

s32 fldLmapTaskExists(void) {
    return kwlnTaskGetTaskByName(fldLocalMapTaskName) != NULL;
}

void fldLocalMapDebugPrintfNoop(const char *fmt, ...) {
}

/* Map a local-map mode to its track-selection ordinal; unmapped modes use 1. */
INCLUDE_RODATA(const s32, "game/code_0030A128", fldLocalMapTaskName);

s32 fldLocalMapTrackSlotFromMode(s32 mode) {
    s32 slot = 1;

    switch (mode - 4) {
    case 0:
        slot = 1;
        break;
    case 3:
        slot = 3;
        break;
    case 4:
        slot = 2;
        break;
    case 5:
        slot = 7;
        break;
    case 6:
        slot = 8;
        break;
    case 7:
        slot = 4;
        break;
    case 8:
        slot = 6;
        break;
    case 9:
        slot = 5;
        break;
    }
    return slot;
}

/* The 0x1C flag takes precedence over 0x13 when selecting the map variant. */
void fldInitializeLmapTaskVariant(LmapTaskState *task) {
    s64 flagSet;
    u32 variant;

    D_0043908C = 0;
    D_004388AC = 1;
    D_00439090 = 0;
    D_004388B0 = 0;
    flagSet = mdlFlagTest(0x1c);
    variant = 3;
    if (flagSet == 0) {
        flagSet = mdlFlagTest(0x13);
        variant = 2;
        if (flagSet == 0) {
            variant = 1;
        }
    }
    task->variant = variant;
    task->phase = 0;
}

void fldShutdownLmapResources(void) {
    sndReleaseTrackHandle(0x400001);
    sdfReleaseAllSpriteSlots();
    sdfDestroyActiveCounterRuntime();
    fldReleaseMapRequestQueues();
    fldReleaseCameraColorEffect();
    sdfDisableFlagListViewTargetRelativeMode();
    evtSetSolarOverlayFullyTransparent();
    dspCloseChannel();
    sdfQueueGeneralAllocationRelease(D_004388A4);
    evtDestroySecondaryWorldNode();
}

u8 fldUpdateLmapResourceLoad(LmapTaskState *task) {
    return fldAdvanceLmapResourceLoad(task) != 0;
}

typedef struct LmapFileEntry {
    char name[48];
    u32 kind;
    s32 argument;
} LmapFileEntry;

extern LmapFileEntry D_00400598[];
extern char D_00438898[];
extern char D_0042D280[];
extern char D_0042D298[];
extern char D_0042D2A8[];
extern char D_0042D2B8[];
extern void sdfInstallNonzeroSpriteSlots(SdfSlotSet *);
extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);
extern s32 sndFindPackedTrackLoadStatus(s32);
extern void sndEnsureMidiBankResident(s32);
extern void *sdfPacGetExtensionData(SdfPacStreamPacketHeader *);
extern s32 btlStageTaskCleanupCallback(void *);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D280);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D298);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D2A8);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D2B8);

s32 fldAdvanceLmapResourceLoad(LmapTaskState *task)
{
    LmapLoadState *load = &task->load;
    LmapFileEntry *files = D_00400598;
    PacWork *node;
    s32 slotIndex;

    switch (load->phase) {
    case 0:
        load->phase = 2;
        load->entry = 0;
        load->unknown0C = 0;
        break;
    case 1:
        sdfInstallNonzeroSpriteSlots((SdfSlotSet *)task);
        evtPrintDeveloperConsoleMessage(D_0042D280);
        return 1;
    case 2:
        load->file = NULL;
        switch (files[load->entry].kind) {
        case 1:
            effRequestResourceByMode(D_00438898, files[load->entry].name, 0, &task->spriteSlots[files[load->entry].argument]);
            break;
        case 2:
            if (task->variant != files[load->entry].argument) {
                load->entry++;
                if (load->entry < 6U) {
                    load->phase = 2;
                } else {
                    load->phase = 1;
                }
                return 0;
            }
            load->archive = fileQueuePlainDispatchRequest(files[load->entry].name);
            break;
        case 4:
            if (sndFindPackedTrackLoadStatus(files[load->entry].argument) == 0) {
                sndEnsureMidiBankResident(files[load->entry].argument);
            } else {
                /* Native 0x0030AD7C stores 4 before the common phase-3 store. */
                load->phase = 4;
            }
            break;
        case 5:
            if (files[load->entry].argument == 0) {
                task->allocation = sdfReadNamedResource(files[load->entry].name, &task->resourceAddress, NULL);
            }
            load->phase = 4;
            return 0;
        }
        load->phase = 3;
        break;
    case 3:
        switch (files[load->entry].kind) {
        case 3:
            if (fileIsRequestReadyInCurrentMode(load->file)) {
                load->phase = 4;
            }
            break;
        case 1:
            if (task->spriteSlots[files[load->entry].argument] != 0) {
                load->phase = 4;
            }
            break;
        case 2:
            if (fileRequestIsReady(load->archive)) {
                load->phase = 4;
            }
            break;
        case 4:
            if (sndFindPackedTrackLoadStatus(files[load->entry].argument) == 1) {
                load->phase = 4;
            }
            break;
        }
        break;
    case 4:
        if (load->file != NULL) {
            fileGetLoadedDataAddress(load->file);
            evtPrintDeveloperConsoleMessage(D_0042D298, fileGetResourceSize(load->file));
        }
        if (files[load->entry].kind == 2) {
            node = ((FilePacRequest *)load->archive)->packet.queueHead;
            if (node != NULL) {
                /* Indexed banks form the late cursor; see docs/idioms.md,
                 * "Local-map PAC slot induction". */
                slotIndex = 0;
                do {
                    if (node->packet[0] == 1) {
                        evtPrintDeveloperConsoleMessage(D_0042D2A8, sdfPacGetExtensionData((SdfPacStreamPacketHeader *)node));
                        if (task->variant == 1) {
                            task->spriteSlots[slotIndex + 1] = (u32)effCreateResourceSlotSetFromAllocation(node->resourceHandle, 0);
                        } else if (task->variant == 2) {
                            task->spriteSlots[slotIndex + 9] = (u32)effCreateResourceSlotSetFromAllocation(node->resourceHandle, 0);
                        } else if (task->variant == 3) {
                            task->spriteSlots[slotIndex + 17] = (u32)effCreateResourceSlotSetFromAllocation(node->resourceHandle, 0);
                        }
                        sdfReleaseResourceAllocation(node->resourceHandle);
                    }
                    node = node->next;
                    slotIndex++;
                } while (node != NULL);
            }
        }
        evtPrintDeveloperConsoleMessage(D_0042D2A8, files[load->entry].name);
        if (load->file != NULL) {
            sdfReleaseResourceAllocation((SdfMemBlock *)fileGetResourceHandle(load->file));
            filePollEntryCleanup(load->file);
            evtPrintDeveloperConsoleMessage(D_0042D2B8);
        }
        if (load->archive != NULL) {
            btlStageTaskCleanupCallback(load->archive);
            load->archive = NULL;
        }
        load->entry++;
        if (load->entry < 6U) {
            load->phase = 2;
        } else {
            load->phase = 1;
        }
        break;
    }
    return 0;
}

void fldInitializeLocalMapScene(void) {
    s32 mask;
    s32 flags;
    s32 index;
    s32 mode = 3;

    if (!mdlFlagTest(0x1C)) {
        mode = 2;
        if (!mdlFlagTest(0x13)) {
            mode = 1;
        }
    }
    kwlnDrawControlFlags |= 0x2000000;
    mask = 0x3FF;
    evtCreateWorldObjectForKey(1, mode);
    dds3SetWorldObject(NULL);
    fldCreateLocalMapCamera();
    dds3SetWorldObject(dds3GetWorldSecondaryObject());
    D_00435CBC = 0x80000000;
    fldApplyLightSetIndex(mode);
    flags = fldPackLocalMapFlagStates();
    if (flags != 0) {
        mask = flags;
    }
    index = fldCountMaskBitsBeforeOrdinal(mask, D_00438890);
    D_00438894 = index;
    sdfCreateMaskedCounterChannels(mask, index);
    sdfInitializeMapCounterSelection(D_00438894, func_0030B880(mask));
    fldCreateMapRequestQueues();
    dspCloseChannel();
    D_004388A4 = sdfReadNamedResource("/lmap/lmpmsg.bmd", &D_004388A8, NULL);
    evtCreateMessageWindowIfMissing(D_004388A8);
    evtSetSolarOverlayFullyVisible();
    fldInitializeCameraColorResource();
    sdfEnableFlagListViewTargetRelativeMode();
    sndStartTrackDefault(0x400001);
    if (mdlFlagTest(0x13)) {
        kwlnFadeOutStart(0, 0, 0, 30);
    } else {
        kwlnFadeOutStart(255, 255, 255, 30);
    }
    sdfCounterInitializeDisplayAnimation();
}

extern s32 sdfPollCounterSelectionInput(void);
extern void sdfCounterTickCountdownAndMapTimers(void);
extern void fldUpdateMapRequestQueues(s32);
extern void sdfCounterStepDownAnimationValue(void);
extern void sdfLatchBaseVectorsForSelection(void);
extern void evtBeginSolarOverlayFadeOut(s32);
extern s32 sdfStepSelectedMapCameraTransition(void);
extern s32 fldAdvanceLocalMapCameraTransition(void);
extern s8 D_0037F510[];
extern s32 D_00439094;
extern void sdfCounterAdvanceBoundedAnimationValue(void);
extern s32 fldLmapToggleOverlay(void);
extern void fldDisplayLocalMapCounterMessage(void);
extern s32 sdfUpdateSelectedCounterMapCameraTransition(void);
extern s8 evtGetCapturedWindowPanelValue(void);
extern void evtBeginSolarOverlayFadeIn(s32);
/* Legacy calls pass the RGB bytes as promoted integer arguments. */
extern void sdfClearCounterDisplayFlags(void);
extern s32 sdfCounterGetDisplayValue(void);
extern void fldSetPendingSceneAction(u32);
extern void sdfInitInnerVectors(void);
extern void fldUpdateSelectedValueTransition(void);
extern void fldUpdateSwayOffset(void);

s32 fldUpdateLocalMapCounterSelection(void) {
    s32 result = 0;

    switch (D_004388AC) {
    case 1:
        result = sdfPollCounterSelectionInput();
        sdfCounterTickCountdownAndMapTimers();
        fldUpdateMapRequestQueues(1);
        sdfCounterStepDownAnimationValue();
        if (result == -2) {
            D_00439094 = 0;
            D_004388AC = 3;
            sdfLatchBaseVectorsForSelection();
            sdfCounterInitializeDisplayAnimation();
            evtBeginSolarOverlayFadeOut(10);
        }
        sdfStepSelectedMapCameraTransition();
        if (result == 1) {
            D_004388AC = 2;
            sdfLatchBaseVectorsForSelection();
        }
        break;
    case 2:
        result = sdfPollCounterSelectionInput();
        sdfCounterTickCountdownAndMapTimers();
        fldUpdateMapRequestQueues(1);
        sdfCounterStepDownAnimationValue();
        if (result == -2) {
            D_00439094 = 0;
            D_004388AC = 3;
            sdfLatchBaseVectorsForSelection();
            sdfCounterInitializeDisplayAnimation();
        }
        fldAdvanceLocalMapCameraTransition();
        if (result == 0) {
            D_004388AC = 1;
            sdfLatchBaseVectorsForSelection();
        }
        break;
    case 3:
        sdfCounterTickCountdownAndMapTimers();
        fldUpdateMapRequestQueues(0);
        if (D_0037F510[0x21] < 0) {
            D_00439094 = 55;
        }
        D_00439094++;
        if (D_00439094 >= 35) {
            sdfCounterAdvanceBoundedAnimationValue();
        }
        if (D_00439094 >= 55 && fldLmapToggleOverlay()) {
            if (D_0043908C == 0) {
                fldDisplayLocalMapCounterMessage();
                D_0043908C = 1;
            } else {
                D_0043908C = 0;
                D_004388AC = 4;
                sdfLatchBaseVectorsForSelection();
            }
        }
        sdfUpdateSelectedCounterMapCameraTransition();
        break;
    case 4:
        result = evtGetCapturedWindowPanelValue();
        if (result == 1) {
            D_004388AC = result;
            evtBeginSolarOverlayFadeIn(30);
            D_00439094 = 0;
        } else {
            D_004388AC = 5;
            if (mdlFlagTest(0x13)) {
                kwlnFadeInStart(0, 0, 0, 30);
            } else {
                kwlnFadeInStart(255, 255, 255, 30);
            }
            D_00439094 = 0;
        }
        break;
    case 5:
        D_00439090++;
        if (D_00439090 >= 31) {
            D_004388AC = 6;
        }
        break;
    case 6:
        result = -1;
        sdfClearCounterDisplayFlags();
        fldSetPendingSceneAction(sdfCounterGetDisplayValue());
        break;
    }

    sdfInitInnerVectors();
    fldUpdateSelectedValueTransition();
    fldUpdateSwayOffset();
    return result;
}

void fldDrawLocalMapOverlay(void) {
    fldDrawSkyGradient();
    sdfDrawPositionedSlotImage(0, 0, 0, 0x80, 0x21, 0, 0x53);
    sdfDrawPositionedSlotImage(0, 0, 0, 0x80, 0x22, 0, 0x53);

    switch (D_004388AC) {
    case 1:
    case 2:
        sdfAdjustCounterValueAndRedraw(1);
        fldDrawSelectedMapMarker();
        mnuDrawAnimatedTransition(0);
        break;
    case 3:
        sdfAdjustCounterValueAndRedraw(0);
        fldDrawSelectedMapMarker();
        mnuDrawAnimatedTransition(1);
        break;
    case 4:
        sdfAdjustCounterValueAndRedraw(0);
        mnuDrawAnimatedTransition(1);
        break;
    case 5:
        mnuDrawAnimatedTransition(0);
        break;
    }

    if (D_004388B0 != 0) {
        func_00137888();
    }
}

s32 fldPackLocalMapFlagStates(void) {
    s32 bits;

    bits = mdlFlagTest(0x400);
    bits |= mdlFlagTest(0x402) << 1;
    bits |= mdlFlagTest(0x401) << 2;
    bits |= mdlFlagTest(0x405) << 3;
    bits |= mdlFlagTest(0x407) << 4;
    bits |= mdlFlagTest(0x406) << 5;
    bits |= mdlFlagTest(0x403) << 6;
    bits |= mdlFlagTest(0x404) << 7;
    return bits;
}

s32 mdlCollectFlagBitsIntoMask(void) {
    s32 bits;

    bits = mdlFlagTest(0x40F) << 1;
    bits |= mdlFlagTest(0x410) << 6;
    bits |= mdlFlagTest(0x411) << 7;
    bits |= mdlFlagTest(0x412) << 3;
    bits |= mdlFlagTest(0x413) << 5;
    bits |= mdlFlagTest(0x414) << 4;
    return bits;
}

u32 sdfGetInitialMapCameraTransitionState(void) {
    return 1;
}

void fldDisplayLocalMapCounterMessage(void) {
    char text[32];

    sprintf(text, D_004388A0, sdfCounterGetDisplayWordPointer());
    evtCopyEntryStringToActiveWindow(0, text);
    evtSetMessageWindowOptionWhenOpen(0);
    dspStartEntry(0);
    evtStoreValueAndCaptureWindowPanelValue(1);
}

s32 fldLmapToggleOverlay(void) {
    s32 result = 0;

    if (evtGetMessageWindowControlState() != 0) {
        dspRequestMessageWindowModeOne();
    } else {
        evtFinishMessageWindowAndNotify();
        result = 1;
    }
    return result;
}

extern s32 sdfCounterGetDisplayValue(void);

void sdfClearCounterDisplayFlags(void) {
    u32 flags = 1 << (sdfCounterGetDisplayValue() - 1);

    if (flags & 2) {
        mdlFlagClear(0x40F);
    }
    if (flags & 8) {
        mdlFlagClear(0x412);
    }
    if (flags & 0x10) {
        mdlFlagClear(0x414);
    }
    if (flags & 0x20) {
        mdlFlagClear(0x413);
    }
    if (flags & 0x40) {
        mdlFlagClear(0x410);
    }
    if (flags & 0x80) {
        mdlFlagClear(0x411);
    }
}

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438888);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_0043888C);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438890);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438894);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438898);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A0);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A4);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A8);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388AC);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388B0);

