#include "common.h"
#include "dat_state.h"

extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern void mnuArmMantraLimitLineFlags(u32, u32);

typedef struct MenuSearchState {
    u8 requestedId;
    u8 pad1[4];
    s8 selectedIndex;
    s16 targetId;
} MenuSearchState;

typedef struct MenuSearchNode {
    u8 pad0[0x58];
    struct MenuSearchNode *next;
    u8 pad5C[0x14];
    DatPartyRecord *value; /* 0x70: party record carried by the SDK node */
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

typedef struct MantraProfileRequirement {
    u16 id;
    u16 reserved;
} MantraProfileRequirement;

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
    u8 paddingC04[4];
} MenuSearchObject; /* Full 0xC08-byte status-resource allocation. */

extern s32 mnuGetMantraNodePositionRecord(s16);
extern s32 mnuGetActiveMantraModelFlagState(void);

extern const MantraProfileRequirement D_003D0078[16];
extern const MantraProfileRequirement D_003D00B8[12];
extern const MantraProfileRequirement D_003D00E8[18];
extern const char D_00427390[];
extern const char D_004273C0[];
extern const char D_004273E0[];
extern const char D_004273F0[];

extern s32 mnuGetNodeValueByIndex();

extern s32 ptyAnyActivePartyMemberAtProfileCap(u16, u16);

extern u32 ptyGetProfileRecordCap(u16);

extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);

/* Full selection work allocated by evtAllocateMantraSelectionWork. */
typedef struct MantraLimitSlot {
    u32 allocation;
    u32 capacity;
    u16 *values;
    u8 data[0x160];
} MantraLimitSlot;

typedef char MenuSearchObject_size_must_be_0xC08[(sizeof(MenuSearchObject) == 0xC08) ? 1 : -1];
typedef char MantraLimitSlot_size_must_be_0x16C[(sizeof(MantraLimitSlot) == 0x16C) ? 1 : -1];

struct MantraPanelPool;
struct MantraPanelAnimation;
extern u32 func_002890A8(void *object);
extern struct MantraPanelAnimation *mnuSpawnPanelSlotA(struct MantraPanelPool *, s32, s8, s16, s16, u32);
extern void mnuOffsetPanelAndSetVisualParams(struct MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern u32 mnuQueuePanelAnimationTransition(struct MantraPanelAnimation *, u32, s16);
extern void mnuStorePanelEntry(s32, s32);

/* Mark the selected mantra node and start its panel animation. */
void func_0028E568(MenuSearchObject *menu, u16 nodeId) {
    u32 selected;
    MantraLimitSlot *slot;
    u16 *entry;
    struct MantraPanelAnimation *panel;

    mnuGetMantraNodePositionRecord((s16)nodeId);
    selected = func_002890A8(menu);
    slot = (MantraLimitSlot *)menu->work.spriteHandles[selected];
    entry = slot->values + nodeId;
    *entry = (*entry & 0xFFF0) | 1;
    panel = mnuSpawnPanelSlotA((struct MantraPanelPool *)menu->work.displaySprite,
                              nodeId, 8, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(panel, 3, 0);
    mnuStorePanelEntry(0x20009, 0x14);
}

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028E638);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427258);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028EB38);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427278);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004272C8);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028EF50);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028F128);

/* Select a menu row whose mantra profile cannot advance at the current model
 * state; if none qualifies, search party profiles against the active rank set. */
s32 func_0028F380(MenuSearchObject *object, MenuSearchState *state) {
    MantraNodePos *record;
    MantraNodePos *entry;
    MantraMenuSlot *slot;
    MenuSearchNode *node;
    DatPartyRecord *party;
    const MantraProfileRequirement *requirements;
    u32 value;
    u32 cap;
    s32 modelFlagState;
    s32 slotIndex;
    s32 entryIndex;
    s32 requirementCount;

    slotIndex = 0;
    slot = &object->work.slots[0];
    modelFlagState = mnuGetActiveMantraModelFlagState();
    evtPrintDeveloperConsoleMessage(D_00427390, modelFlagState);

    for (; slotIndex < 5; slotIndex++, slot++) {
        if (slot->nodeId != 0) {
            entryIndex = 0;
            record = (MantraNodePos *)mnuGetMantraNodePositionRecord(slot->nodeId);
            for (; entryIndex < 6; entryIndex++) {
                entry = record->entries[entryIndex];
                if (entry != 0 && modelFlagState < entry->modelFlagState) {
                    evtPrintDeveloperConsoleMessage(D_004273C0, slotIndex);
                    state->requestedId = record->id;
                    state->selectedIndex = slotIndex;
                    return 1;
                }
            }
        }
    }

    evtPrintDeveloperConsoleMessage(D_004273E0);
    if (modelFlagState == 0) {
        requirements = D_003D0078;
        requirementCount = 16;
    } else if (modelFlagState == 1) {
        requirements = D_003D00B8;
        requirementCount = 12;
    } else {
        requirements = D_003D00E8;
        requirementCount = 18;
    }

    node = object->list->head;
    slotIndex = 0;
    for (; node != 0; slotIndex++, node = node->next) {
        party = node->value;
        for (entryIndex = 0; entryIndex < requirementCount; entryIndex++) {
            u16 id = requirements[entryIndex].id;

            if (id != 0) {
                value = ptyGetProfileRecordValue(party, id);
                cap = ptyGetProfileRecordCap(id);
                if (value == cap) {
                    evtPrintDeveloperConsoleMessage(D_004273F0, slotIndex,
                                                    party->unitId, id);
                    state->requestedId = id;
                    state->selectedIndex = slotIndex;
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 mnuSelectPreferredMantraNode(MenuSearchObject *object, MenuSearchState *state) {
    MantraMenuWork *work = &object->work;
    MantraMenuSlot *slot;
    MantraNodePos *record;
    MantraNodePos *entry;
    MantraNodePos **entries;
    MenuSearchNode *node;
    DatPartyRecord *value;
    s32 i, j;
    s32 cap;

    for (i = 0, slot = work->slots; i < 5; i++, slot++) {
        if (slot->nodeId != 0) {
            record = (MantraNodePos *)mnuGetMantraNodePositionRecord(slot->nodeId);
            value = (DatPartyRecord *)mnuGetNodeValueByIndex(object, i);
            if (ptyAnyActivePartyMemberAtProfileCap(record->id, value->unitId) == 0) {
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
                if (cap == ptyGetProfileRecordValue(value, (*entries)->id)) {
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
        if (current->value->unitId == state->targetId) {
            state->selectedIndex = index;
            return 0;
        }
        current = current->next;
        index++;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004272F8);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427380);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427390);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004273C0);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004273E0);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004273F0);

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

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}
