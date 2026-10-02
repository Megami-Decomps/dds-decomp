#include "common.h"

extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void mnuArmMantraLimitLineFlags(u32, u32);

typedef struct MenuSearchState {
    u8 pad0[5];
    s8 selectedIndex;
    s16 targetId;
} MenuSearchState;

typedef struct MenuSearchValue {
    u8 pad0[4];
    u16 id;
} MenuSearchValue;

typedef struct MenuSearchNode {
    u8 pad0[0x58];
    struct MenuSearchNode *next;
    u8 pad5C[0x14];
    MenuSearchValue *value; /* 0x70: item whose ID is compared */
} MenuSearchNode;

typedef struct MenuSearchList {
    u8 pad0[0x10];
    MenuSearchNode *head;
} MenuSearchList;

typedef struct MenuSearchObject {
    u8 pad0[4];
    MenuSearchList *list;
    u8 pad8[0xBF8];
    u32 drawPool;
} MenuSearchObject;

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E350);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E568);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E638);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427258);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EB38);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427278);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004272C8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EF50);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F128);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004272F8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F570);

/* Select the first list item whose ID matches the requested ID.
 * If absent, retain the previous selection; the return value is always zero. */
s32 mnuSelectMatchingNode(MenuSearchObject *object, MenuSearchState *state) {
    MenuSearchNode *current = object->list->head;
    s32 index = 0;
    while (current != 0) {
        if (current->value->id == state->targetId) {
            state->selectedIndex = index;
            return 0;
        }
        current = current->next;
        index++;
    }
    return 0;
}

void func_0028F7B8(MenuSearchObject *object, u16 id) {
    u32 flags = 0;
    u16 limitIds[18] = {
        0x61, 0x3B, 0x5B, 0x23, 0x40, 0x62,
        0x53, 0x5E, 0x1C, 0x5A, 0x63, 0x54,
        0x15, 0x44, 0x64, 0x07, 0x5C, 0x0E
    };
    s32 i;

    for (i = 0; i < 18; i++) {
        if (limitIds[i] == id) {
            flags = 1U << i;
            break;
        }
    }
    evtPrintDeveloperConsoleMessage(
        "-----------------------LimitLineSetting!!!!!!!!![%x]\n", flags);
    if (flags != 0) {
        mnuArmMantraLimitLineFlags(object->drawPool, flags);
    }
}

typedef struct MantraLimitSlot {
    u8 pad0[8];
    u16 *values;
} MantraLimitSlot;

extern u32 func_002890A8(void *object);
extern void mnuQueueMantraLimitLineFlags(s32 pool, u32 flags);
extern void mnuSetMantraBackgroundSelection(u32 pool, u32 value);

void func_0028F8A8(u8 *object) {
    u16 limitIds[18] = {
        0x61, 0x3B, 0x5B, 0x23, 0x40, 0x62,
        0x53, 0x5E, 0x1C, 0x5A, 0x63, 0x54,
        0x15, 0x44, 0x64, 0x07, 0x5C, 0x0E
    };
    u32 selected = func_002890A8(object);
    MantraLimitSlot *slot = *(MantraLimitSlot **)(object + 0x7AC + selected * 4);
    u32 flags = 0;
    s32 i;

    for (i = 0; i < 18; i++) {
        u16 *value = slot->values + limitIds[i];

        if ((*value & 0xF) != 3) {
            flags |= 1U << i;
        }
    }

    mnuQueueMantraLimitLineFlags(*(s32 *)(object + 0xC00), flags);
    mnuSetMantraBackgroundSelection(*(u32 *)(object + 0xC00), flags);
}

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}

void func_0028FD10(u32 object) {
    func_0028FD30(object, 0xffffffffffffffff, 0xffffffffffffffff);
}

