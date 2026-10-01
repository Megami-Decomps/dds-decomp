#include "common.h"

extern void fldShutdownLmapResources(void);
extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern char D_003B3CA0[]; /* "LmapMain" */

extern s32 kwlnTaskGetTaskByName(u32);

extern u32 D_003BD25C;

extern u32 D_003BD260;

extern u32 D_003BD970;

extern u32 D_003BD974;

extern s32 mdlFlagTest(u32);
extern u32 sdfCounterGetDisplayWordPointer(void);
extern void func_003014F0(char *, char *, s32);
extern void func_0024DD90(s32, void *);
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
extern s32 func_002C2F40();
extern s32 fldLmapTaskUpdate(void);
extern s32 D_003BD23C;
extern s32 D_003BD96C;
extern void fldInitializeLmapState(void);
extern s32 fldStartAndPollLocalMapTrack(void);
extern void func_002C30F0(void);
extern s32 func_002C3220(void);
extern void func_002C3420(void);
extern s32 sndFindPackedTrackLoadStatus(s32);
extern void sndEnsureMidiBankResident(s32);
extern s32 fldLocalMapTrackState;

/* Ring window over a doubly linked node list (prev at 0x18, next at 0x1C). */
typedef struct LmapNode {
    u8 unk0[0x18];
    struct LmapNode *prev; /* 0x18 */
    struct LmapNode *next; /* 0x1C */
} LmapNode;

typedef struct LmapList {
    u8 unk0[6];
    u16 capacity;    /* 0x6 */
    s16 count;       /* 0x8 */
    u8 unkA[2];
    u32 flags;       /* 0xC: bit 0 locks the list */
    LmapNode *first; /* 0x10 */
    LmapNode *lo;    /* 0x14 */
    LmapNode *cur;   /* 0x18 */
    LmapNode *hi;    /* 0x1C */
} LmapList;

extern LmapNode *sdfGridSeekFirstNode(LmapList *);
extern LmapNode *sdfGridSeekLastNode(LmapList *);
extern LmapNode *fldLmapAdvanceWindowStart(LmapList *);
extern LmapNode *fldLmapExpandWindowBackward(LmapList *);

LmapNode *fldLmapAdvanceWindowStart(LmapList *list) {
    LmapNode *cur = list->cur;
    LmapNode *node = list->first;

    if (cur == list->hi) {
        return cur;
    }
    node = node->next;
    if (node == NULL) {
        return cur;
    }
    list->count--;
    list->first = node;
    return cur;
}

LmapNode *fldLmapExpandWindowBackward(LmapList *list) {
    LmapNode *cur = list->cur;
    LmapNode *head = list->first;
    LmapNode *node;
    s32 i;

    if (cur == list->lo) {
        return cur;
    }
    node = head;
    for (i = 0; i < list->capacity; i++) {
        if (node == NULL) {
            return cur;
        }
        node = node->next;
    }
    head = head->prev;
    list->first = head;
    list->count++;
    return cur;
}

LmapNode *fldLmapAdvanceCursor(LmapList *list) {
    LmapNode *cur = list->cur;
    LmapNode *next;

    if (cur == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cur;
    }
    if (cur == list->hi) {
        sdfGridSeekFirstNode(list);
        return list->cur;
    }
    next = cur->next;
    if (next == 0) {
        return cur;
    }
    cur = next;
    list->cur = cur;
    list->count++;
    if (list->count >= list->capacity - 1) {
        cur = fldLmapAdvanceWindowStart(list);
    }
    return cur;
}

LmapNode *fldLmapRewindCursor(LmapList *list) {
    LmapNode *cur = list->cur;
    LmapNode *next;

    if (cur == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cur;
    }
    if (cur == list->lo) {
        sdfGridSeekLastNode(list);
        return list->cur;
    }
    next = cur->prev;
    if (next == 0) {
        return cur;
    }
    cur = next;
    list->cur = cur;
    list->count--;
    if (list->count <= 0) {
        cur = fldLmapExpandWindowBackward(list);
    }
    return cur;
}

LmapNode *fldLmapAdvanceThroughWindow(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->capacity * 2 - list->count;
    for (i = 0; i < steps; i++) {
        result = fldLmapAdvanceCursor(list);
    }
    return result;
}

LmapNode *fldLmapRewindThroughWindow(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->capacity + list->count;
    for (i = 0; i < steps; i++) {
        result = fldLmapRewindCursor(list);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C28E0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2A20);

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

/* Build a one-packet SIF command at (x, y) in GS coordinates and submit it on draw surface `surface`. */
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 width, s32 height, s32 command, s32 surface) {
    void *list = sdfAllocPacketAligned(0x20);
    LmapDrawSurface *target;
    u8 header[0x10];

    sdfInitPacketList(list);
    sdfPktInit(header, x + 0x7000, y + 0x7900, width, height);
    sdfAppendPacket(list, sdfFormatSifPacket(header, command));
    target = &kwlnDrawSurfaces[surface];
    target->submit(target, list);
}

/* Variant that formats a textured sprite packet (func_0011D3E8) instead of a SIF command. */
void fldLmapSubmitScaledSpritePacket(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d, s32 e, s32 surface) {
    void *list = sdfAllocPacketAligned(0x20);
    LmapDrawSurface *target;

    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011D3E8(x + 0x7000, y + 0x7900, a, b * 16, c * 8, d, e));
    target = &kwlnDrawSurfaces[surface];
    target->submit(target, list);
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
        func_002C30F0();
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

void fldStartLmapTask(s32 arg0) {
    if (arg0 != 0) {
        D_003BD23C = func_002C2F40();
    } else {
        D_003BD23C = 1;
    }
    kwlnTaskCreate(D_003B3CA0, 0x2AF8, 0, 0, fldLmapTaskUpdate, 0, 0);
    D_003BD96C = 0;
    fldInitializeLmapState();
}

void fldStopLmapTask(void) {
    fldShutdownLmapResources();
    kwlnTaskDestroyWithHierarchyByName(D_003B3CA0, 1);
}

s32 fldLmapTaskExists(void) {
    return kwlnTaskGetTaskByName((u32)D_003B3CA0) != 0;
}

void func_002C2EF8(const char *fmt, ...) {
}

/* Field-map mode index -> track slot. Indices 7 and 12 are the only values in
 * range with no arm of their own, so they fall through to the default of 1. */
INCLUDE_RODATA(const s32, "game/code_002C2620", D_003B3CA0);

s32 func_002C2F40(s32 index) {
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

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C30F0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3220);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3420);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3510);

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
    func_0024DD90(0, text);
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

