#include "common.h"

extern s8 D_003BD88C;

extern u32 D_003BBDF0;

void evtInitializeVisualData(s32 arg0);

void evtBeginSolarOverlayFadeIn(s32 arg0);

void func_0022AB90(void);
s32 evtGetMirroredSolarPhase(void);
void func_0022A8D8(u32 arg0);
void evtDrawFadingSolarOverlayFrame(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u32 arg5, s32 arg6);

extern s32 D_003BD890;

extern s32 D_003BD894;
extern u32 D_003BA8EC;
extern char D_003ACD18[]; /* "EventTest" */
void kwlnTaskCreate(void *name, s32 priority, s32 unk2, s32 unk3, void *update, void *destroy, void *data);
void kwlnTaskDestroyWithHierarchyByName(void *name, s32 flag);
void evtDestroySecondaryWorldNode(void);
s32 evtGetSolarPhase(s32 object);

/* Same visual-value layout as the sequel: two 8-halfword tables and phase. */
typedef struct EventVisualData {
    u8 pad00[0x3C];
    s16 firstValues[8];
    u8 pad4C[0x50];
    s16 secondValues[8];
    u8 padAC[0x50];
    u8 solarPhase;
} EventVisualData;

typedef struct EventListNode {
    u16 orderKey;
    u8 pad02[0x2E];
    struct EventListNode *next; /* 0x30 */
    struct EventListNode *prev; /* 0x34 */
} EventListNode;

typedef struct {
    u8 pad00[0x50];
    s32 count;           /* 0x50 */
    EventListNode *first; /* 0x54 */
    EventListNode *last;  /* 0x58 */
} EventList;

INCLUDE_ASM(const s32, "game/code_0022A248", evtDrawFadingSolarOverlayFrame);

/* Seed both visual-value tables and cache the current solar phase. */
void evtInitializeVisualData(s32 object) {
    s32 solarPhase = evtGetSolarPhase(object);
    s16 *values = ((EventVisualData *)object)->firstValues;
    ((EventVisualData *)object)->solarPhase = solarPhase;
    values[0] = 0x39;
    values[1] = 0x33;
    values[2] = 0x1D;
    values[3] = 0x23;
    values[5] = 5;
    values[7] = 10;
    values = ((EventVisualData *)object)->secondValues;
    values[0] = 0x37;
    values[1] = 0x32;
    values[2] = 15;
    values[3] = 15;
    values[5] = 10;
    values[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A8D8);

void evtAdvanceSolarOverlayFadeAndDraw(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5) {
    s32 mirrored;
    /* The phase arrives narrowed to a byte; a wider type changes the compare. */
    mirrored = (s8)evtGetMirroredSolarPhase();
    func_0022A8D8(arg4);
    if ((D_003BD88C & 1) != 0) {
        if ((D_003BD88C & 2) != 0) {
            if (D_003BD890 < D_003BD894) {
                D_003BD890++;
            } else {
                D_003BD88C = 0;
            }
        } else if ((D_003BD88C & 4) != 0) {
            if (D_003BD890 > 0) {
                D_003BD890--;
            } else {
                /* Clears only bit 0 while testing bit 2: the original quirk, kept verbatim. */
                D_003BD88C &= ~1;
            }
        }
        if ((D_003BD88C & 1) != 0) {
            f32 ratio = (f32)D_003BD890 / (f32)D_003BD894;
            s32 scaled = (s32)((f32)arg3 * ratio);
            evtDrawFadingSolarOverlayFrame(arg0, arg1, arg2, scaled, mirrored, arg4, arg5);
            return;
        }
    }
    if ((D_003BD88C & 4) == 0) {
        evtDrawFadingSolarOverlayFrame(arg0, arg1, arg2, arg3, mirrored, arg4, arg5);
    }
}

s32 evtHasSolarOverlayTransitionState(void) {
    return D_003BD88C != 0;
}

void evtBeginSolarOverlayFadeIn(s32 value) {
    if (value == 0) {
        D_003BD88C = 0;
        D_003BD890 = 0;
        D_003BD894 = 0;
        return;
    }
    D_003BD894 = (s32)value;
    D_003BD88C = 3;
    D_003BD890 = 0;
}

void evtBeginSolarOverlayFadeOut(s32 value) {
    if (value == 0) {
        D_003BD88C = 5;
        D_003BD894 = 1;
        D_003BD890 = 0;
    } else {
        D_003BD890 = value;
        D_003BD88C = 5;
        D_003BD894 = value;
    }
}

void func_0022AB58(void) {
}

u32 func_0022AB60(void) {
    func_0022AB58();
    return 0;
}

void *func_0022AB80(void) {
    return (void *)func_0022AB90;
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AB90);

void func_0022AEB8(void) {
    scrDestroyAllNamedProcesses();
}

void evtStartTestTask(void) {
    D_003BA8EC = 0x80000000;
    kwlnTaskCreate(D_003ACD18, 0x2AF9, 1, 1, func_0022AB90, func_0022AEB8, 0);
}

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD18);

void evtStopTestTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003ACD18, 1);
    kwlnTaskDestroyWithHierarchyByName("PolygonMovie", 0);
    evtDestroySecondaryWorldNode();
}

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD38);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD48);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD58);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD68);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD78);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AF50);

void func_0022B618(void) {
    D_003BBDF0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B620);

void evtUnlinkListNode(EventList *owner, EventListNode *node) {
    EventListNode *next = node->next;
    EventListNode *previous = node->prev;
    if (previous == 0) {
        owner->first = next;
    } else {
        previous->next = next;
    }
    {
        EventListNode *earlier = node->prev;
        EventListNode *later = node->next;
        if (later == 0) {
            owner->last = earlier;
        } else {
            later->prev = earlier;
        }
    }
    {
        s32 count = owner->count;
        node->prev = 0;
        node->next = 0;
        owner->count = count - 1;
    }
}

/* Walk the linked list and reinsert the first out-of-order successor. */
void evtReorderListNodes(EventList *owner) {
    if (owner != 0) {
        EventListNode *current = owner->first;
        while (current != 0) {
            EventListNode *next = current->next;
            EventListNode *scan = next;
            while (scan != 0) {
                if (scan->orderKey < current->orderKey) {
                    evtUnlinkListNode(owner, scan);
                    func_0022B620(owner, scan);
                    next = scan->next;
                    break;
                }
                scan = scan->next;
            }
            current = next;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B7A0);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDEC);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF0);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF4);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF8);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE00);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE08);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE10);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE18);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE20);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE28);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE30);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE38);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE40);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE48);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE50);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE58);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE60);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE68);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE70);

