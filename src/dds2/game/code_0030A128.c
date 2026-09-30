#include "common.h"

extern s32 func_0030AC10(void);

extern u32 D_004388AC;

extern u32 D_004388B0;

extern u32 D_0043908C;

extern u32 D_00439090;

extern s32 mdlFlagTest(u32);

extern s32 func_00101740(u32);

extern char D_0042D240[]; /* "LmapMain" */

extern void fldShutdownLmapResources(void);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

typedef struct LmapTaskState {
    u32 value0;       /* Purpose not established */
    u32 value4;       /* Cleared when the Lmap task initializes */
    u32 variant;      /* 1..3, selected by the two model flags */
} LmapTaskState;

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

extern LmapNode *func_0030A0E8(LmapList *);
extern LmapNode *func_0030A108(LmapList *);
extern LmapNode *func_0030A128(LmapList *);
extern LmapNode *func_0030A160(LmapList *);
extern void *func_003292A8(s32 size);
extern u32 *sdfMemoryGetBlockAddress(u32 handle);
extern s32 func_0030AAB0(s32);
extern void func_0030A8A8(void);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void *, void *, void *);
extern s32 D_00438890;
extern void func_00342580(u32);
extern s32 func_0030DAE8(void);
extern void func_0030C640(void);
extern void fldReleaseMapRequestQueues(void);
extern void fldReleaseCameraColorEffect(void);
extern void func_00316E70(void);
extern void evtSetSolarOverlayFullyTransparent(void);
extern void dspCloseChannel(void);
extern void func_003298C0(s32);
extern void evtDestroySecondaryWorldNode(void);
extern s32 D_004388A4;
extern s32 sdfCounterGetDisplayWordPointer(void);
extern void func_0035C860(char *, char *, ...);
extern void func_0026C918(s32, void *);
extern void func_0026C648(s32);
extern void dspStartEntry(s32);
extern void evtCaptureMessageWindowSoundMode(s32);
extern char D_004388A0[];
extern s64 evtGetMessageWindowControlState(void);
extern void func_0026C900(void);
extern void func_0026C710(void);
extern void fldInitializeLmapTaskVariant(LmapTaskState *task);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A128);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A160);

LmapNode *func_0030A1D0(LmapList *list) {
    LmapNode *cur = list->cur;
    LmapNode *next;

    if (cur == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cur;
    }
    if (cur == list->hi) {
        func_0030A0E8(list);
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
        cur = func_0030A128(list);
    }
    return cur;
}

LmapNode *func_0030A270(LmapList *list) {
    LmapNode *cur = list->cur;
    LmapNode *next;

    if (cur == 0) {
        return 0;
    }
    if (list->flags & 1) {
        return cur;
    }
    if (cur == list->lo) {
        func_0030A108(list);
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
        cur = func_0030A160(list);
    }
    return cur;
}

LmapNode *func_0030A300(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->capacity * 2 - list->count;
    for (i = 0; i < steps; i++) {
        result = func_0030A1D0(list);
    }
    return result;
}

LmapNode *func_0030A378(LmapList *list) {
    LmapNode *result = 0;
    s32 i;
    s32 steps;

    if (list->flags & 1) {
        return 0;
    }
    steps = list->capacity + list->count;
    for (i = 0; i < steps; i++) {
        result = func_0030A270(list);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A3E8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A528);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A700);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A7C8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A8A8);

void fldStartLmapTask(s32 arg0) {
    void *handle = func_003292A8(0x88);
    u32 *block = sdfMemoryGetBlockAddress((u32)handle);

    memset(block, 0, 0x88);
    if (arg0 != 0) {
        D_00438890 = func_0030AAB0(arg0);
    } else {
        D_00438890 = 1;
    }
    fldInitializeLmapTaskVariant((LmapTaskState *)block);
    kwlnTaskCreate(D_0042D240, 0x2AF8, 0, 0, func_0030A8A8, 0, block);
}

void fldStopLmapTask(void) {
    fldShutdownLmapResources();
    kwlnTaskDestroyWithHierarchyByName(D_0042D240, 1);
}

s32 fldLmapTaskExists(void) {
    return func_00101740((u32)D_0042D240) != 0;
}

void func_0030AA68(const char *fmt, ...) {
}

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D240);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AAB0);

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
    func_0030DAE8();
    func_0030C640();
    fldReleaseMapRequestQueues();
    fldReleaseCameraColorEffect();
    func_00316E70();
    evtSetSolarOverlayFullyTransparent();
    dspCloseChannel();
    func_003298C0(D_004388A4);
    evtDestroySecondaryWorldNode();
}

u8 func_0030ABF0(void) {
    s64 status;

    status = func_0030AC10();
    return status != 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AC10);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B078);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B1E8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B470);

s32 func_0030B568(void) {
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

s32 func_0030B600(void) {
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

void func_0030B680(void) {
    char text[32];

    func_0035C860(text, D_004388A0, sdfCounterGetDisplayWordPointer());
    func_0026C918(0, text);
    func_0026C648(0);
    dspStartEntry(0);
    evtCaptureMessageWindowSoundMode(1);
}

s32 fldLmapToggleOverlay(void) {
    s32 result = 0;

    if (evtGetMessageWindowControlState() != 0) {
        func_0026C900();
    } else {
        func_0026C710();
        result = 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B728);

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

