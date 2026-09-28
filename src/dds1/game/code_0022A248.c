#include "common.h"

extern s8 D_003BD88C;

extern u32 D_003BBDF0;

void initializeEventVisualData(s32 arg0);

void func_0022AB00(s32 arg0);

void func_0022AB90(void);

extern s32 D_003BD890;

extern s32 D_003BD894;
extern u32 D_003BA8EC;
extern char D_003ACD18[]; /* "EventTest" */
void kwlnTaskCreate(void *name, s32 priority, s32 unk2, s32 unk3, void *update, void *destroy, void *data);
void kwlnTaskDestroyWithHierarchyByName(void *name, s32 flag);
void func_0021FE38(void);
s32 func_002286E8(s32 object);

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

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A248);

void initializeEventVisualData(s32 object) {
    s32 state = func_002286E8(object);
    s16 *values = (s16 *)(object + 0x3C);
    *(u8 *)(object + 0xFC) = state;
    values[0] = 0x39;
    values[1] = 0x33;
    values[2] = 0x1D;
    values[3] = 0x23;
    values[5] = 5;
    values[7] = 10;
    values = (s16 *)(object + 0x9C);
    values[0] = 0x37;
    values[1] = 0x32;
    values[2] = 15;
    values[3] = 15;
    values[5] = 10;
    values[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A8D8);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022A960);

s32 func_0022AAF0(void) {
    return D_003BD88C != 0;
}

void func_0022AB00(s32 arg0) {
    if (arg0 == 0) {
        D_003BD88C = 0;
        D_003BD890 = 0;
        D_003BD894 = 0;
        return;
    }
    D_003BD894 = (s32)arg0;
    D_003BD88C = 3;
    D_003BD890 = 0;
}

void func_0022AB28(s32 arg0) {
    if (arg0 == 0) {
        D_003BD88C = 5;
        D_003BD894 = 1;
        D_003BD890 = 0;
    } else {
        D_003BD890 = arg0;
        D_003BD88C = 5;
        D_003BD894 = arg0;
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
    func_0010BDB8();
}

void startEventTestTask(void) {
    D_003BA8EC = 0x80000000;
    kwlnTaskCreate(D_003ACD18, 0x2AF9, 1, 1, func_0022AB90, func_0022AEB8, 0);
}

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD18);

void stopEventTestTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003ACD18, 1);
    kwlnTaskDestroyWithHierarchyByName("PolygonMovie", 0);
    func_0021FE38();
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

void unlinkEventListNode(EventList *owner, EventListNode *node) {
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

void reorderEventListNodes(EventList *owner) {
    if (owner != 0) {
        EventListNode *current = owner->first;
        while (current != 0) {
            EventListNode *next = current->next;
            EventListNode *scan = next;
            while (scan != 0) {
                if (scan->orderKey < current->orderKey) {
                    unlinkEventListNode(owner, scan);
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

