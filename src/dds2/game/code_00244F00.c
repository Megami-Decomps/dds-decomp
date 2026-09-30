#include "common.h"

extern u32 D_00438FA8;

extern u32 D_00438FAC;

void evtInitializeVisualData(s32 arg0);

extern s8 D_00438FA4;

s32 evtGetSolarPhase(s32 object);

extern char D_004221D8[]; /* "EventTest" */

void kwlnTaskDestroyWithHierarchyByName(void *name, s32 flag);

void evtDestroySecondaryWorldNode(void);

typedef struct EventVisualData {
    u8 pad00[0x3C];
    s16 firstValues[8];
    u8 pad4C[0x50];
    s16 secondValues[8];
    u8 padAC[0x50];
    u8 solarPhase;
} EventVisualData;

extern u32 D_00435CBC;

void kwlnTaskCreate(void *name, s32 priority, s32 unk2, s32 unk3, void *update, void *destroy, void *data);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00244F00);

/* Seed both visual-value tables and cache the current raw solar phase. */
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

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245590);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245618);

s32 func_002457A8(void) {
    return D_00438FA4 != 0;
}

void func_002457B8(s32 value) {
    if (value == 0) {
        D_00438FA4 = 0;
        D_00438FA8 = 0;
        D_00438FAC = 0;
        return;
    }
    D_00438FAC = (s32)value;
    D_00438FA4 = 3;
    D_00438FA8 = 0;
}

void func_002457E0(s32 value) {
    if (value == 0) {
        D_00438FA4 = 5;
        D_00438FAC = 1;
        D_00438FA8 = 0;
    } else {
        D_00438FA8 = value;
        D_00438FA4 = 5;
        D_00438FAC = value;
    }
}

void func_00245810(void) {
}

u32 func_00245818(void) {
    return 0;
}

void func_00245820(void) {
    func_0010BFE0();
}

void evtStartTestTask(void) {
    D_00435CBC = 0x80000000;
    kwlnTaskCreate(D_004221D8, 0x2AF9, 1, 1, func_00245818, func_00245820, 0);
}

INCLUDE_ASM(const s32, "game/code_00244F00", evtStopTestTasks);

INCLUDE_ASM(const s32, "game/code_00244F00", func_002458B8);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245F80);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245F88);

typedef struct EventListNode {
    u16 orderKey;
    u8 pad02[0x2E];
    struct EventListNode *next; /* 0x30 */
    struct EventListNode *prev; /* 0x34 */
} EventListNode;

typedef struct {
    u8 pad00[0x50];
    s32 count;            /* 0x50 */
    EventListNode *first; /* 0x54 */
    EventListNode *last;  /* 0x58 */
} EventList;

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
                    func_00245F88(owner, scan);
                    next = scan->next;
                    break;
                }
                scan = scan->next;
            }
            current = next;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00244F00", func_00246108);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_0043722C);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437230);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437234);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437238);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437240);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437248);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437250);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437258);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437260);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437268);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437270);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437278);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437280);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437288);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437290);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437298);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A0);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A8);

