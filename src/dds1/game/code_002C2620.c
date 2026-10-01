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
extern void func_0024DAE8(s32);
extern void dspStartEntry(s32);
extern void evtCaptureMessageWindowSoundMode(s32);
extern char D_003BD250[];
extern s32 evtGetMessageWindowControlState(void);
extern void func_0024DD78(void);
extern void func_0024DBB0(void);
extern void func_002E96D8(u32);
extern s32 fldReleaseLocalMapResources(void);
extern s64 func_002C4630(void);
extern void fldReleaseMapRequestQueues(void);
extern void fldReleaseCameraColorEffect(void);
extern void func_002CF430(void);
extern void evtSetSolarOverlayFullyTransparent(void);
extern void dspCloseChannel(void);
extern void func_002D0A10(s32);
extern void evtDestroySecondaryWorldNode(void);
extern s32 D_003BD254;
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void *, void *, void *);
extern s32 func_002C2F40(void);
extern s32 fldLmapTaskUpdate(void);
extern s32 D_003BD23C;
extern s32 D_003BD96C;
extern void fldInitializeLmapState(void);
extern s32 func_002C3060(void);
extern void func_002C30F0(void);
extern s32 func_002C3220(void);
extern void func_002C3420(void);
extern s32 func_002E92C0(s32);
extern void func_002E9340(s32);
extern s32 D_003BD248;

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

extern LmapNode *func_002C25E0(LmapList *);
extern LmapNode *func_002C2600(LmapList *);
extern LmapNode *func_002C2620(LmapList *);
extern LmapNode *func_002C2658(LmapList *);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2620);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2658);

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
        func_002C25E0(list);
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
        cur = func_002C2620(list);
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
        func_002C2600(list);
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
        cur = func_002C2658(list);
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

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2BF8);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2CC0);

s32 fldLmapTaskUpdate(void) {
    s32 state = D_003BD96C;
    s32 result;

    if (state == 0) {
        if (func_002C3060() != 0) {
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

INCLUDE_RODATA(const s32, "game/code_002C2620", D_003B3CA0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2F40);

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
    func_002C4630();
    fldReleaseMapRequestQueues();
    fldReleaseCameraColorEffect();
    func_002CF430();
    evtSetSolarOverlayFullyTransparent();
    dspCloseChannel();
    func_002D0A10(D_003BD254);
    evtDestroySecondaryWorldNode();
}

s32 func_002C3060(void) {
    if (D_003BD248 == 1) {
        if (func_002E92C0(0x400000) == 0) {
            func_002E9340(0x400000);
            D_003BD248 = 2;
        } else {
            D_003BD248 = 0xFF;
        }
    } else if (D_003BD248 == 2) {
        if (func_002E92C0(0x400000) == 1) {
            D_003BD248 = 0xFF;
        }
    }
    if (D_003BD248 == 0xFF) {
        D_003BD248 = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C30F0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3220);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3420);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3510);

s32 func_002C35C8(void) {
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
    func_0024DAE8(0);
    dspStartEntry(0);
    evtCaptureMessageWindowSoundMode(1);
}

s32 fldLmapToggleOverlay(void) {
    s32 result = 0;

    if (evtGetMessageWindowControlState() != 0) {
        func_0024DD78();
    } else {
        func_0024DBB0();
        result = 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3738);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD238);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD23C);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD240);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD248);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD250);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD254);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD258);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD25C);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD260);

