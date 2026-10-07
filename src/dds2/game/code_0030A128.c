#include "common.h"
#include "sdf.h"
#include "itf_grid_text.h"

extern s32 func_0030AC10(void);

extern void func_00134A18(void);
extern void func_00137888(void);
extern void func_0030C8E8(s32);
extern void sdfDrawPositionedSlotImage(s32, s32, s32, s32, s32, s32, s32);
extern void mnuDrawAnimatedTransition(s32);
extern void fldDrawSelectedMapMarker(void);

extern u32 D_004388AC;

extern u32 D_004388B0;

extern u32 D_0043908C;

extern s32 D_00439090; /* fade timer: func_0030B1E8 tests >= 31 with slti */

extern s32 mdlFlagTest(u32);

extern s32 kwlnTaskGetTaskByName(u32);

extern char fldLocalMapTaskName[]; /* "LmapMain" */

extern void fldShutdownLmapResources(void);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

typedef struct LmapTaskState {
    u32 value0;       /* Purpose not established */
    u32 value4;       /* Cleared when the Lmap task initializes */
    u32 variant;      /* 1..3, selected by the two model flags */
} LmapTaskState;

typedef GridTextListItem LmapNode;
typedef GridTextWidget LmapList;

extern s32 sdfGridSeekFirstNode(GridTextWidget *);
extern s32 sdfGridSeekLastNode(GridTextWidget *);
extern LmapNode *fldLmapAdvanceWindowStart(LmapList *);
extern u32 itfGetGridListLinkFlags(LmapList *);
extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);
extern LmapNode *fldLmapExpandWindowBackward(LmapList *);
extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfMemoryGetBlockAddress(SdfMemBlock *block);
extern s32 fldLocalMapTrackSlotFromMode(s32);
extern void func_0030A8A8(void);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void *, void *, void *);
extern s32 D_00438890;
extern void func_00342580(u32);
extern void sdfReleaseAllSpriteSlots(void);
extern void sdfDestroyActiveCounterRuntime(void);
extern void fldReleaseMapRequestQueues(void);
extern void fldReleaseCameraColorEffect(void);
extern void func_00316E70(void);
extern void evtSetSolarOverlayFullyTransparent(void);
extern s32 dspCloseChannel(void);
extern void sdfQueueNonzeroResourceId(s32);
extern void evtDestroySecondaryWorldNode(void);
extern SdfMemBlock *D_004388A4;
extern s32 sdfCounterGetDisplayWordPointer(void);
extern void func_0035C860(char *, char *, ...);
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void dspStartEntry(s32);
extern void evtStoreValueAndCaptureWindowPanelValue(s32);
extern char D_004388A0[];
extern s64 evtGetMessageWindowControlState(void);
extern void func_0026C900(void);
extern void evtFinishMessageWindowAndNotify(void);
extern void evtCreateWorldObjectForKey(s32, s32);
extern void dds3SetWorldObject(void *);
extern void *dds3GetWorldSecondaryObject(void);
extern void fldCreateLocalMapCamera(void);
extern s32 fldPackLocalMapFlagStates(void);
extern s32 fldCountMaskBitsBeforeOrdinal(s32, s32);
extern s32 sdfCreateMaskedCounterChannels(s32, s32);
extern s32 func_0030B880(s32);
extern void sdfInitializeMapCounterSelection(s32, s32);
extern void func_0030E880(void);
extern SdfMemBlock *sdfReadNamedResource(const char *, u32 *, u32 *);
extern void evtCreateMessageWindowIfMissing(s32);
extern void evtSetSolarOverlayFullyVisible(void);
extern void fldApplyLightSetIndex(s32);
extern void fldInitializeCameraColorResource(void);
extern void func_00316E60(void);
extern void sndStartTrackDefault(s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
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
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void sdfPktInit(void *, s32, s32, s32, s32);
extern void *sdfFormatSifPacket();
extern void *func_0011F250();

/* Build one positioned SIF command and submit it on the requested draw surface. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 width, s32 height, s32 command, s32 surfaceIndex) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfPoolNode *drawSurface;
    u8 packetHeader[0x10];

    sdfInitPacketList(packetList);
    sdfPktInit(packetHeader, x + 0x7000, y + 0x7900, width, height);
    sdfAppendPacket(packetList, (u32)sdfFormatSifPacket(packetHeader, command));
    drawSurface = &kwlnDrawSurfaces[surfaceIndex];
    drawSurface->append((SdfListHead *)drawSurface, packetList);
}

/* Build an untextured rectangle with a separate outline color. */
void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height, s32 fillColor, s32 borderColor, s32 surfaceIndex) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfPoolNode *drawSurface;

    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, (u32)func_0011F250(x + 0x7000, y + 0x7900, z, width * 16, height * 8, fillColor, borderColor));
    drawSurface = &kwlnDrawSurfaces[surfaceIndex];
    drawSurface->append((SdfListHead *)drawSurface, packetList);
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A8A8);

void fldStartLmapTask(s32 mode) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(0x88);
    u32 *taskData = (u32 *)sdfMemoryGetBlockAddress(allocation);

    memset(taskData, 0, 0x88);
    if (mode != 0) {
        D_00438890 = fldLocalMapTrackSlotFromMode(mode);
    } else {
        D_00438890 = 1;
    }
    fldInitializeLmapTaskVariant((LmapTaskState *)taskData);
    kwlnTaskCreate(fldLocalMapTaskName, 0x2AF8, 0, 0, func_0030A8A8, 0, taskData);
}

void fldStopLmapTask(void) {
    fldShutdownLmapResources();
    kwlnTaskDestroyWithHierarchyByName(fldLocalMapTaskName, 1);
}

s32 fldLmapTaskExists(void) {
    return kwlnTaskGetTaskByName((u32)fldLocalMapTaskName) != 0;
}

void func_0030AA68(const char *fmt, ...) {
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
    task->value4 = 0;
}

void fldShutdownLmapResources(void) {
    func_00342580(0x400001);
    sdfReleaseAllSpriteSlots();
    sdfDestroyActiveCounterRuntime();
    fldReleaseMapRequestQueues();
    fldReleaseCameraColorEffect();
    func_00316E70();
    evtSetSolarOverlayFullyTransparent();
    dspCloseChannel();
    sdfQueueNonzeroResourceId((s32)D_004388A4);
    evtDestroySecondaryWorldNode();
}

u8 func_0030ABF0(void) {
    s64 status;

    status = func_0030AC10();
    return status != 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AC10);

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
    func_0030E880();
    dspCloseChannel();
    D_004388A4 = sdfReadNamedResource("/lmap/lmpmsg.bmd", &D_004388A8, NULL);
    evtCreateMessageWindowIfMissing(D_004388A8);
    evtSetSolarOverlayFullyVisible();
    fldInitializeCameraColorResource();
    func_00316E60();
    sndStartTrackDefault(0x400001);
    if (mdlFlagTest(0x13)) {
        kwlnFadeOutStart(0, 0, 0, 30);
    } else {
        kwlnFadeOutStart(255, 255, 255, 30);
    }
    sdfCounterInitializeDisplayAnimation();
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B1E8);

void fldDrawLocalMapOverlay(void) {
    func_00134A18();
    sdfDrawPositionedSlotImage(0, 0, 0, 0x80, 0x21, 0, 0x53);
    sdfDrawPositionedSlotImage(0, 0, 0, 0x80, 0x22, 0, 0x53);

    switch (D_004388AC) {
    case 1:
    case 2:
        func_0030C8E8(1);
        fldDrawSelectedMapMarker();
        mnuDrawAnimatedTransition(0);
        break;
    case 3:
        func_0030C8E8(0);
        fldDrawSelectedMapMarker();
        mnuDrawAnimatedTransition(1);
        break;
    case 4:
        func_0030C8E8(0);
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

u32 func_0030B678(void) {
    return 1;
}

void fldDisplayLocalMapCounterMessage(void) {
    char text[32];

    func_0035C860(text, D_004388A0, sdfCounterGetDisplayWordPointer());
    evtCopyEntryStringToActiveWindow(0, text);
    evtSetMessageWindowOptionWhenOpen(0);
    dspStartEntry(0);
    evtStoreValueAndCaptureWindowPanelValue(1);
}

s32 fldLmapToggleOverlay(void) {
    s32 result = 0;

    if (evtGetMessageWindowControlState() != 0) {
        func_0026C900();
    } else {
        evtFinishMessageWindowAndNotify();
        result = 1;
    }
    return result;
}

extern s32 sdfCounterGetDisplayValue(void);
extern void mdlFlagClear(s32 flag);

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

