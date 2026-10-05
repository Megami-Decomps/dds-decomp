#include "common.h"

extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void mnuArmMantraLimitLineFlags(u32, u32);

typedef struct MenuSearchState {
    u8 requestedId;
    u8 pad1[4];
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

typedef struct MantraNodePos {
    u32 kind : 4;
    s32 modelFlagState : 4;
    u32 reserved : 8;
    s16 id;
    s16 firstKey;
    s16 secondKey;
    struct MantraNodePos *entries[6];
} MantraNodePos;

typedef struct MantraMenuSlot {
    u8 nodeId;
    u8 pad01[7];
} MantraMenuSlot;

/* The same menu work layout is used by the node-value/index getters. */
typedef struct MantraMenuWork {
    u8 pad000[0x554];
    u32 flags;
    u8 pad558[8];
    s32 resourceId;
    u8 pad564[8];
    u32 spriteHandles[6];
    u8 pad584[0x30];
    u32 nodeIds[8];
    s32 selectedIndex;
    s32 nodeCount;
    u8 pad5DC[0x394];
    MantraMenuSlot slots[5];
    u8 pad998[0x14];
    u32 displaySprite;
    u8 pad9B0[0x10];
    u32 drawPool;
} MantraMenuWork;

typedef struct MenuSearchObject {
    u8 pad0[4];
    MenuSearchList *list;
    u8 pad8[0x238];
    MantraMenuWork work;
} MenuSearchObject;

extern s32 mnuGetMantraNodePositionRecord(s32);
extern s32 mnuGetNodeValueByIndex();
extern s32 ptyAnyActivePartyMemberAtProfileCap(u16, u16);
extern s32 ptyGetProfileRecordCap(u16);
extern s32 ptyGetProfileRecordValue(s32, u16);

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

s32 mnuSelectPreferredMantraNode(MenuSearchObject *object, MenuSearchState *state) {
    MantraMenuWork *work = &object->work;
    MantraMenuSlot *slot;
    MantraNodePos *record;
    MantraNodePos *entry;
    MantraNodePos **entries;
    MenuSearchNode *node;
    MenuSearchValue *value;
    s32 i, j;
    s32 cap;

    for (i = 0, slot = work->slots; i < 5; i++, slot++) {
        if (slot->nodeId != 0) {
            record = (MantraNodePos *)mnuGetMantraNodePositionRecord(slot->nodeId);
            value = (MenuSearchValue *)mnuGetNodeValueByIndex(object, i);
            if (ptyAnyActivePartyMemberAtProfileCap(record->id, value->id) == 0) {
                for (j = 0; j < 6; j++) {
                    entry = record->entries[j];
                    if (entry != 0 && entry->kind == 2 && entry->id == state->requestedId) {
                        state->selectedIndex = i;
                        return 1;
                    }
                }
            }
        }
    }
    for (i = 0, slot = work->slots; i < 5; i++, slot++) {
        if (slot->nodeId != 0) {
            record = (MantraNodePos *)mnuGetMantraNodePositionRecord(slot->nodeId);
            mnuGetNodeValueByIndex(object, i);
            for (j = 0; j < 6; j++) {
                entry = record->entries[j];
                if (entry != 0 && entry->kind == 2 && entry->id == state->requestedId) {
                    state->selectedIndex = i;
                    return 1;
                }
            }
        }
    }
    node = object->list->head;
    for (i = 0; i < 5; i++, node = node->next) {
        value = node->value;
        record = (MantraNodePos *)mnuGetMantraNodePositionRecord(state->requestedId);
        for (j = 0, entries = record->entries; j < 6; j++, entries++) {
            if (*entries != 0) {
                cap = ptyGetProfileRecordCap((*entries)->id);
                if (cap == ptyGetProfileRecordValue((s32)value, (*entries)->id)) {
                    state->selectedIndex = i;
                    return 1;
                }
            }
        }
    }
    return 0;
}

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

void mnuSelectMantraLimitLine(MenuSearchObject *object, u16 id) {
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
        mnuArmMantraLimitLineFlags(object->work.drawPool, flags);
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
    MenuSearchObject *menu = (MenuSearchObject *)object;
    u32 selected = func_002890A8(object);
    MantraLimitSlot *slot = (MantraLimitSlot *)menu->work.spriteHandles[selected];
    u32 flags = 0;
    s32 i;

    for (i = 0; i < 18; i++) {
        u16 *value = slot->values + limitIds[i];

        if ((*value & 0xF) != 3) {
            flags |= 1U << i;
        }
    }

    mnuQueueMantraLimitLineFlags(menu->work.drawPool, flags);
    mnuSetMantraBackgroundSelection(menu->work.drawPool, flags);
}

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}

void func_0028FD10(u32 object) {
    func_0028FD30(object, 0xffffffffffffffff, 0xffffffffffffffff);
}

