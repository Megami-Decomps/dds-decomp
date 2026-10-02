#include "common.h"

extern void fldShutdownLmapResources(void);
extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern char fldLocalMapTaskName[]; /* "LmapMain" */

extern s32 kwlnTaskGetTaskByName(u32);

extern u32 D_003BD25C;

extern u32 D_003BD260;

extern u32 D_003BD970;

extern u32 D_003BD974;

extern s32 mdlFlagTest(u32);
extern u32 sdfCounterGetDisplayWordPointer(void);
extern void func_003014F0(char *, char *, s32);
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void dspStartEntry(s32);
extern void evtCaptureMessageWindowSoundMode(s32);
extern char D_003BD250[];
extern s32 evtGetMessageWindowControlState(void);
extern void func_0024DD78(void);
extern void evtFinishMessageWindowAndNotify(void);
extern void func_002E96D8(u32);
extern s32 fldReleaseLocalMapResources(void);
extern s64 sdfDestroyActiveCounterRuntime(void);
extern void fldReleaseMapRequestQueues(void);
extern void fldReleaseCameraColorEffect(void);
extern void func_002CF430(void);
extern void evtSetSolarOverlayFullyTransparent(void);
extern void dspCloseChannel(void);
extern void sdfQueueNonzeroResourceId(s32);
extern void evtDestroySecondaryWorldNode(void);
extern s32 D_003BD254;
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void *, void *, void *);
extern s32 fldLocalMapTrackSlotFromMode();
extern s32 fldLmapTaskUpdate(void);
extern s32 D_003BD23C;
extern s32 D_003BD96C;
extern void fldInitializeLmapState(void);
extern s32 fldStartAndPollLocalMapTrack(void);
extern void fldInitializeLocalMapScene(void);
extern s32 func_002C3220(void);
extern void func_002C3420(void);
extern s32 sndFindPackedTrackLoadStatus(s32);
extern void sndEnsureMidiBankResident(s32);
extern s32 fldLocalMapTrackState;
extern void evtCreateWorldObjectForKey(s32, s32);
extern void dds3SetWorldObject(void *);
extern void *dds3GetWorldSecondaryObject(void);
extern void fldCreateLocalMapCamera(void);
extern s32 mdlCollectLowFlagBits(void);
extern s32 fldCountMaskBitsBeforeOrdinal(s32, s32);
extern s32 sdfCreateMaskedCounterChannels(s32, s32);
extern s32 func_002C38B0(s32);
extern void func_002C3AC8(s32, s32);
extern s32 fldLoadLocalMapResources();
extern void fldCreateMapRequestQueues(void);
extern u32 sdfReadNamedResource(const char *, u32 *, u32 *);
extern void fldApplyLightSetIndex(s32);
extern void fldInitializeCameraColorResource(void);
extern void func_002CF420(void);
extern void sndStartTrackDefault(s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern void sdfCounterInitializeDisplayAnimation(void);
extern u32 D_003BA8EC;
extern s32 D_003BD240;
extern u32 D_003BD258;

/* Cursor window over a doubly linked node list (prev at 0x18, next at 0x1C). */
typedef struct LmapNode {
    u8 unk0[0x18];
    struct LmapNode *prev; /* 0x18 */
    struct LmapNode *next; /* 0x1C */
    struct LmapList *child; /* 0x20 */
} LmapNode;

typedef struct LmapList {
    u8 unk0[6];
    u16 windowCapacity; /* 0x6: maximum visible span */
    s16 cursorOffset;   /* 0x8: cursor position relative to windowStart */
    u8 unkA[2];
    u32 flags;       /* 0xC: bit 0 locks the list */
    LmapNode *windowStart; /* 0x10 */
    LmapNode *rangeFirst;  /* 0x14 */
    LmapNode *cursor;      /* 0x18 */
    LmapNode *rangeLast;   /* 0x1C */
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    u8 pad30[4];
    void (*selected)(struct LmapList *);
    void (*draw)(s32, s32, s32, struct LmapList *, s32);
} LmapList;

extern LmapNode *sdfGridSeekFirstNode(LmapList *);
extern LmapNode *sdfGridSeekLastNode(LmapList *);
extern LmapNode *fldLmapAdvanceWindowStart(LmapList *);
extern u32 itfGetGridListLinkFlags(LmapList *);
extern void uiDrawUniformRgbRange(s32 *, s32 *, s32, u32, s32);
extern LmapNode *fldLmapExpandWindowBackward(LmapList *);

/* Move the visible start forward without moving the cursor. */
LmapNode *fldLmapAdvanceWindowStart(LmapList *list) {
    LmapNode *cursor = list->cursor;
    LmapNode *windowStart = list->windowStart;

    if (cursor == list->rangeLast) {
        return cursor;
    }
    windowStart = windowStart->next;
    if (windowStart == NULL) {
        return cursor;
    }
    list->cursorOffset--;
    list->windowStart = windowStart;
    return cursor;
}

/* Move the visible start backward only when a full forward span is available. */
LmapNode *fldLmapExpandWindowBackward(LmapList *list) {
    LmapNode *cursor = list->cursor;
    LmapNode *windowStart = list->windowStart;
    LmapNode *scanNode;
    s32 stepIndex;

    if (cursor == list->rangeFirst) {
        return cursor;
    }
    scanNode = windowStart;
    for (stepIndex = 0; stepIndex < list->windowCapacity; stepIndex++) {
        if (scanNode == NULL) {
            return cursor;
        }
        scanNode = scanNode->next;
    }
    windowStart = windowStart->prev;
    list->windowStart = windowStart;
    list->cursorOffset++;
    return cursor;
}

/* Advance the unlocked cursor, wrapping at the range end and following its window. */
LmapNode *fldLmapAdvanceCursor(LmapList *list) {
    LmapNode *cursor = list->cursor;
    LmapNode *nextNode;

    if (cursor == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cursor;
    }
    if (cursor == list->rangeLast) {
        sdfGridSeekFirstNode(list);
        return list->cursor;
    }
    nextNode = cursor->next;
    if (nextNode == 0) {
        return cursor;
    }
    cursor = nextNode;
    list->cursor = cursor;
    list->cursorOffset++;
    if (list->cursorOffset >= list->windowCapacity - 1) {
        cursor = fldLmapAdvanceWindowStart(list);
    }
    return cursor;
}

/* Rewind the unlocked cursor, wrapping at the range start and following its window. */
LmapNode *fldLmapRewindCursor(LmapList *list) {
    LmapNode *cursor = list->cursor;
    LmapNode *previousNode;

    if (cursor == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cursor;
    }
    if (cursor == list->rangeFirst) {
        sdfGridSeekLastNode(list);
        return list->cursor;
    }
    previousNode = cursor->prev;
    if (previousNode == 0) {
        return cursor;
    }
    cursor = previousNode;
    list->cursor = cursor;
    list->cursorOffset--;
    if (list->cursorOffset <= 0) {
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
    steps = list->windowCapacity * 2 - list->cursorOffset;
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
    steps = list->windowCapacity + list->cursorOffset;
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

extern s32 D_003BD238;
extern LmapList *D_003BD968;
extern void fldLmapSubmitScaledSpritePacket(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_002C22F0(s32, s32, s32, LmapList *, s32);

void func_002C2A20(s32 x, s32 y, s32 z, LmapList *list, s32 channel) {
    u32 color;

    D_003BD238++;
    color = (list->flags & 2) ? 0x80608060 : 0x80606060;
    if (!(list->flags & 0x40)) {
        fldLmapSubmitScaledSpritePacket(list->x + x, list->y + y, z, list->width,
                                       list->height, 0x40000000, color, channel);
        if (list->flags & 2) {
            fldLocalMapDrawScrollIndicators(x, y, z, 0x60806080, list, channel);
        }
    }
    func_002C22F0(x, y, z, list, channel);
    if (list->draw != 0) {
        list->draw(list->x + x, list->y + y, z, list, channel);
    }
    if ((list->flags & 0x10) && D_003BD238 >= 2) {
        list->flags &= ~1;
        list->flags &= ~2;
        D_003BD968->flags |= 2;
        D_003BD968->flags &= ~1;
        list->flags &= ~0x10;
    } else {
        D_003BD968 = list;
        if (list->flags & 0x21) {
            if (list->cursor->child != 0) {
                func_002C2A20(x + ((list->width + 8) << 4), y, z, list->cursor->child, channel);
            }
        } else if ((list->flags & 2) && list->selected != 0) {
            list->selected(list);
        }
    }
    D_003BD238--;
}


typedef struct LmapDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct LmapDrawSurface *, void *); /* 0x10 */
    u8 pad14[0xC];
} LmapDrawSurface; /* 0x20 */

extern LmapDrawSurface kwlnDrawSurfaces[];
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfPktInit(void *, s32, s32, s32, s32);
extern void *sdfFormatSifPacket();
extern void *func_0011D3E8();

/* Build one positioned SIF command and submit it on the requested draw surface. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 width, s32 height, s32 command, s32 surfaceIndex) {
    void *packetList = sdfAllocPacketAligned(0x20);
    LmapDrawSurface *drawSurface;
    u8 packetHeader[0x10];

    sdfInitPacketList(packetList);
    sdfPktInit(packetHeader, x + 0x7000, y + 0x7900, width, height);
    sdfAppendPacket(packetList, sdfFormatSifPacket(packetHeader, command));
    drawSurface = &kwlnDrawSurfaces[surfaceIndex];
    drawSurface->submit(drawSurface, packetList);
}

/* Build an untextured rectangle with a separate outline color. */
void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 z, s32 width, s32 height, s32 fillColor, s32 borderColor, s32 surfaceIndex) {
    void *packetList = sdfAllocPacketAligned(0x20);
    LmapDrawSurface *drawSurface;

    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, func_0011D3E8(x + 0x7000, y + 0x7900, z, width * 16, height * 8, fillColor, borderColor));
    drawSurface = &kwlnDrawSurfaces[surfaceIndex];
    drawSurface->submit(drawSurface, packetList);
}

s32 fldLmapTaskUpdate(void) {
    s32 state = D_003BD96C;
    s32 result;

    if (state == 0) {
        if (fldStartAndPollLocalMapTrack() != 0) {
            D_003BD96C = 1;
        }
        return 0;
    }
    if (state == 1) {
        fldInitializeLocalMapScene();
        D_003BD96C = 2;
        return 0;
    }
    if (state == 2) {
        result = func_002C3220();
        if (result == -1) {
            return result;
        }
        if (result == 2) {
            return 0;
        }
        func_002C3420();
    }
    return 0;
}

void fldStartLmapTask(s32 mode) {
    if (mode != 0) {
        D_003BD23C = fldLocalMapTrackSlotFromMode();
    } else {
        D_003BD23C = 1;
    }
    kwlnTaskCreate(fldLocalMapTaskName, 0x2AF8, 0, 0, fldLmapTaskUpdate, 0, 0);
    D_003BD96C = 0;
    fldInitializeLmapState();
}

void fldStopLmapTask(void) {
    fldShutdownLmapResources();
    kwlnTaskDestroyWithHierarchyByName(fldLocalMapTaskName, 1);
}

s32 fldLmapTaskExists(void) {
    return kwlnTaskGetTaskByName((u32)fldLocalMapTaskName) != 0;
}

void func_002C2EF8(const char *fmt, ...) {
}

/* Field-map mode index -> track slot. Indices 7 and 12 are the only values in
 * range with no arm of their own, so they fall through to the default of 1. */
s32 fldLocalMapTrackSlotFromMode(s32 index) {
    s32 slot = 1;

    switch (index - 2) {
    case 0:
        slot = 1;
        break;
    case 1:
        slot = 4;
        break;
    case 2:
        slot = 9;
        break;
    case 3:
        slot = 6;
        break;
    case 4:
        slot = 5;
        break;
    case 5:
        slot = 10;
        break;
    case 6:
        slot = 3;
        break;
    case 8:
        slot = 8;
        break;
    case 9:
        slot = 2;
        break;
    case 10:
        slot = 7;
        break;
    }
    return slot;
}

void fldInitializeLmapState(void) {
    s64 flagSet;

    D_003BD25C = 1;
    D_003BD970 = 0;
    D_003BD974 = 0;
    flagSet = mdlFlagTest(0x413);
    D_003BD260 = (u32)(flagSet == 0);
}

void fldShutdownLmapResources(void) {
    func_002E96D8(0x400001);
    fldReleaseLocalMapResources();
    sdfDestroyActiveCounterRuntime();
    fldReleaseMapRequestQueues();
    fldReleaseCameraColorEffect();
    func_002CF430();
    evtSetSolarOverlayFullyTransparent();
    dspCloseChannel();
    sdfQueueNonzeroResourceId(D_003BD254);
    evtDestroySecondaryWorldNode();
}

s32 fldStartAndPollLocalMapTrack(void) {
    if (fldLocalMapTrackState == 1) {
        if (sndFindPackedTrackLoadStatus(0x400000) == 0) {
            sndEnsureMidiBankResident(0x400000);
            fldLocalMapTrackState = 2;
        } else {
            fldLocalMapTrackState = 0xFF;
        }
    } else if (fldLocalMapTrackState == 2) {
        if (sndFindPackedTrackLoadStatus(0x400000) == 1) {
            fldLocalMapTrackState = 0xFF;
        }
    }
    if (fldLocalMapTrackState == 0xFF) {
        fldLocalMapTrackState = 1;
        return 1;
    }
    return 0;
}

void fldInitializeLocalMapScene(void) {
    s32 mask;
    s32 flags;
    s32 index;

    if (mdlFlagTest(0x27)) {
        evtCreateWorldObjectForKey(1, 2);
    } else {
        evtCreateWorldObjectForKey(1, 1);
    }
    mask = 0x3FF;
    dds3SetWorldObject(NULL);
    fldCreateLocalMapCamera();
    dds3SetWorldObject(dds3GetWorldSecondaryObject());
    D_003BA8EC = 0x80000000;
    flags = mdlCollectLowFlagBits();
    if (flags != 0) {
        mask = flags;
    }
    index = fldCountMaskBitsBeforeOrdinal(mask, D_003BD23C);
    D_003BD240 = index;
    sdfCreateMaskedCounterChannels(mask, index);
    func_002C3AC8(D_003BD240, func_002C38B0(mask));
    fldLoadLocalMapResources(mask);
    fldCreateMapRequestQueues();
    dspCloseChannel();
    D_003BD254 = sdfReadNamedResource("/lmap/lmpmsg.bmd", &D_003BD258, NULL);
    evtCreateMessageWindowIfMissing(D_003BD258);
    evtSetSolarOverlayFullyVisible();
    fldApplyLightSetIndex(1);
    fldInitializeCameraColorResource();
    func_002CF420();
    sndStartTrackDefault(0x400001);
    kwlnFadeOutStart(255, 255, 255, 30);
    sdfCounterInitializeDisplayAnimation();
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3220);

extern void func_00132010(void);
extern void evtSetDrawSurfaceIndex(s32);
extern void func_00108CB8(s32);
extern void func_001093B8(s32, s32, s32, s32, u32, s32, s32, s32);
extern void func_002C4850(s32);
extern void func_002C7950(void);
extern void func_002C6448(s32);
extern s32 func_00134CD8(void);

void func_002C3420(void) {
    func_00132010();
    evtSetDrawSurfaceIndex(84);
    func_00108CB8(0);
    func_001093B8(0, 0, 170, 195, 0x3300101E, 0x101E, 0x101E, 0x101E);

    switch (D_003BD25C) {
    case 1:
    case 2:
        func_002C4850(1);
        func_002C7950();
        func_002C6448(0);
        break;
    case 3:
        func_002C4850(0);
        func_002C7950();
        func_002C6448(1);
        break;
    case 4:
        func_002C4850(0);
        func_002C6448(1);
        break;
    case 5:
        func_002C6448(0);
        break;
    }
    if (D_003BD260 != 0) {
        func_00134CD8();
    }
}

s32 mdlCollectLowFlagBits(void) {
    s32 bits;

    bits = mdlFlagTest(0x400);
    bits |= mdlFlagTest(0x401) << 1;
    bits |= mdlFlagTest(0x402) << 2;
    bits |= mdlFlagTest(0x403) << 3;
    bits |= mdlFlagTest(0x404) << 4;
    bits |= mdlFlagTest(0x405) << 5;
    bits |= mdlFlagTest(0x406) << 6;
    bits |= mdlFlagTest(0x407) << 7;
    bits |= mdlFlagTest(0x408) << 8;
    bits |= mdlFlagTest(0x409) << 9;
    return bits;
}

s32 mdlCollectFlagBitsIntoMask(void) {
    s32 bits;

    bits = mdlFlagTest(0x40A) << 3;
    bits |= mdlFlagTest(0x40B) << 5;
    bits |= mdlFlagTest(0x40C) << 6;
    bits |= mdlFlagTest(0x40D) << 7;
    bits |= mdlFlagTest(0x40E) << 8;
    bits |= mdlFlagTest(0x40F) << 9;
    return bits;
}

/* Highest set flag wins; the final 0x410 test is still executed even
 * though its result does not affect the returned stage. */
u32 fldGetLmapStage(void) {
    s64 flagSet;
    u32 stage;

    flagSet = mdlFlagTest(0x412);
    stage = 2;
    if (flagSet == 0) {
        flagSet = mdlFlagTest(0x411);
        stage = 1;
        if (flagSet == 0) {
            mdlFlagTest(0x410);
            stage = 0;
        }
    }
    return stage;
}

void fldDisplayLocalMapCounterMessage(void) {
    char text[32];

    func_003014F0(text, D_003BD250, sdfCounterGetDisplayWordPointer());
    evtCopyEntryStringToActiveWindow(0, text);
    evtSetMessageWindowOptionWhenOpen(0);
    dspStartEntry(0);
    evtCaptureMessageWindowSoundMode(1);
}

s32 fldLmapToggleOverlay(void) {
    s32 result = 0;

    if (evtGetMessageWindowControlState() != 0) {
        func_0024DD78();
    } else {
        evtFinishMessageWindowAndNotify();
        result = 1;
    }
    return result;
}

extern s32 sdfCounterGetDisplayValue(void);
extern void mdlFlagClear(s32 flag);
extern void mdlFlagSet(s32 flag);

void sdfClearCounterDisplayFlags(void) {
    u32 flags = 1 << (sdfCounterGetDisplayValue() - 1);

    if (flags & 8) {
        mdlFlagClear(0x40A);
    }
    if (flags & 0x20) {
        mdlFlagClear(0x40B);
        mdlFlagSet(0x23A);
        mdlFlagClear(0x23C);
    }
    if (flags & 0x40) {
        mdlFlagClear(0x40C);
    }
    if (flags & 0x80) {
        mdlFlagClear(0x40D);
    }
    if (flags & 0x100) {
        mdlFlagClear(0x40E);
        mdlFlagSet(0x23B);
        mdlFlagClear(0x239);
    }
    if (flags & 0x200) {
        mdlFlagClear(0x40F);
    }
}

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD238);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD23C);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD240);

INCLUDE_SDATA(const s32, "game/code_002C2620", fldLocalMapTrackState);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD250);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD254);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD258);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD25C);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD260);

