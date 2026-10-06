#include "common.h"
#include "mnu.h"

typedef struct MenuPanelTransition {
    u16 frame;
    u16 panelId;
    u16 mode;
    u16 action;
} MenuPanelTransition;

extern s32 func_00292478(void *, s32, s32);


extern u32 *mnuPanelSoundEntryPool;

extern u64 mnuSpawnPanelSlotB(u32, u64, u64, u64, u64, u64);

extern u64 mnuFindPanelSlotById(u32, u64, u64);

typedef struct MenuContainer MenuContainer;
extern u32 func_002890A8(MenuContainer *);

typedef struct MenuPanelNode {
    s32 index;
    u8 pad04[0x54];
    struct MenuPanelNode *next;
    u8 pad5C[0x14];
    s32 value;
} MenuPanelNode;

typedef struct MenuPanelList {
    u8 pad00[0x10];
    MenuPanelNode *head;
    u8 pad14[8];
    MenuPanelNode *selected;
} MenuPanelList;

typedef struct MenuPanelSlot {
    u8 pad00[8];
    u16 *values;
} MenuPanelSlot;

/* The selector also carries word-wide flags in its packed representation. */
typedef union MenuPanelSelector {
    u32 packed;
    struct {
        u16 flags;
        s16 index;
    } fields;
} MenuPanelSelector;
typedef struct MantraNodePos {
    MenuPanelSelector selector;
    s16 x;
    s16 y;
    struct MantraNodePos *neighbors[6];
} MantraNodePos;

typedef struct MenuPanelEntry {
    s32 soundHandle;
    s32 framesRemaining;
} MenuPanelEntry;

typedef struct MenuPanelEntryPool {
    u32 allocation;
    MenuPanelEntry *entries;
    s32 count;
} MenuPanelEntryPool;

typedef struct MenuPanelState {
    u8 pad00[0x560];
    MantraNodePos *defaultSelector;
    MantraNodePos *alternateSelector;
    u8 pad568[4];
    MenuPanelSlot *slots[18];
    u32 collectedValues[8];
    s32 savedSelection;
    s32 collectedCount;
    u8 pad5DC[4];
    s8 selectionIndex;
    u8 pad5E1[0x3C3];
    s32 navigationState;
    u8 pad9A8[4];
    u32 resource;
    union {
        u32 flags;
        u8 flagBytes[4];
    };
    u8 unk9B4;
    u8 unk9B5;
    u16 unk9B6;
    u8 pad9B8[8];
    s32 selectionController;
} MenuPanelState;

typedef struct MenuPanelObject {
    u8 pad00[4];
    MenuPanelList *list;
    u8 pad08[0x238];
    MenuPanelState state;
} MenuPanelObject;
extern void func_00291338(void);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427560);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FD30);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FEF0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275B0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275E8);

s32 func_00290240(MenuPanelObject *object, s8 direction) {
    MantraNodePos *position = object->state.defaultSelector;
    s32 grid[2];
    s32 neighbor;

    grid[0] = position->x / 10 + (position->x & 1);
    grid[1] = position->y / 10;
    switch (direction) {
    /* Vertical moves depend on the staggered row; horizontal moves do not. */
    case 1: neighbor = (grid[1] & 1) ? 5 : 0; break;
    case 4: neighbor = (grid[1] & 1) ? 3 : 2; break;
    case 2: neighbor = 1; break;
    case 8: neighbor = 4; break;
    /* Upper diagonals, then lower diagonals. */
    case 9: neighbor = 5; break;
    case 3: neighbor = 0; break;
    case 12: neighbor = 3; break;
    case 6: neighbor = 2; break;
    default: return -1;
    }
    return neighbor;
}


s32 func_00290328(MenuPanelObject *object, MantraNodePos *list, s8 position) {
    u16 selected;
    MantraNodePos *entry;
    u16 value;
    s32 result;

    if (list == NULL || position == -1) {
        return -1;
    }
    selected = func_002890A8((MenuContainer *)object);
    entry = list->neighbors[position];
    result = 0;
    if (entry == NULL) {
        result = 1;
    } else {
        value = object->state.slots[selected]->values[entry->selector.fields.index];
        if ((value & 15) == 3) {
            result = 2;
        } else if ((entry->selector.packed & 0x100) != 0 && ((value >> 8) & 8) != 0) {
            switch (position) {
                case 1:
                case 4:
                    result = 3;
                    break;
                default:
                    result = 2;
                    break;
            }
        }
    }
    return result;
}

extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern s32 func_00290240(MenuPanelObject *, s8);
extern MantraNodePos *func_002906E0(MenuPanelObject *, u16, s8);
typedef struct MantraNeighborIds {
    u16 id;
    u16 neighbors[6];
} MantraNeighborIds;
extern MantraNeighborIds D_003D0130[50];

s32 mnuNavigateMantraSelector(MenuPanelObject *object, s8 flags) {
    MenuPanelState *state = &object->state;
    MantraNodePos *node;
    MantraNodePos *neighbor;
    s32 selected;
    s8 position;
    s8 attempts = 0;
    s8 result;
    u16 i;
    u16 id;

    selected = (u16)func_002890A8((MenuContainer *)object);
    if (state->navigationState == 0 || state->navigationState == 7) {
        node = state->defaultSelector;
        position = func_00290240(object, flags);
        for (;;) {
            result = func_00290328(object, node, position);
            switch (result) {
                case -1:
                    return 0;
                case 0:
                    state->defaultSelector = node->neighbors[position];
                    if (state->navigationState == 0) {
                        state->navigationState = 4;
                    }
                    return 1;
                case 1:
                    if ((flags & 0xa) != 0 || attempts == 2) {
                        for (i = 0; i < 50; i++) {
                            if (D_003D0130[i].id == node->selector.fields.index) {
                                id = D_003D0130[i].neighbors[position];
                                if (id != 0) {
                                    neighbor = mnuGetMantraNodePositionRecord(id);
                                    if ((state->slots[selected]->values[neighbor->selector.fields.index] & 15) != 3) {
                                        state->defaultSelector = neighbor;
                                        if (state->navigationState == 0) {
                                            state->navigationState = 4;
                                        }
                                        return 1;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if (attempts == 2) {
                        return 0;
                    }
                    /* Try the opposite direction when this neighbor is unavailable. */
                case 2:
                    if (position == 1 || position == 4) {
                        return 0;
                    }
                    if (position == 0) {
                        position = 5;
                    } else if (position == 5) {
                        position = 0;
                    } else if (position == 2) {
                        position = 3;
                    } else if (position == 3) {
                        position = 2;
                    }
                    attempts++;
                    if (attempts == 2) {
                        neighbor = func_002906E0(object, state->defaultSelector->selector.fields.index, position);
                        if (neighbor != NULL) {
                            state->defaultSelector = neighbor;
                            if (state->navigationState == 0) {
                                state->navigationState = 4;
                            }
                            return 1;
                        }
                    } else if (attempts >= 3) {
                        return 0;
                    }
                    break;
                case 3:
                    state->defaultSelector = node->neighbors[position]->neighbors[position];
                    if (state->navigationState == 0) {
                        state->navigationState = 4;
                    }
                    return 1;
            }
        }
    }
    return 0;
}

extern s32 mnuGetActiveMantraModelFlagState(void);
extern s32 mdlFlagTest(s32);

extern void *memcpy(void *, const void *, u32);
extern const s16 D_00427668[4][3];
extern const u16 D_00427680[3][5];
extern const u16 D_004276A0[3][5];

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437940);

MantraNodePos *func_002906E0(MenuPanelObject *object, u16 id, s8 direction) {
    s32 i;
    s8 activeFlags = 0;
    u16 bridgeIds[4] = {0x61, 0x62, 0x63, 0x64};
    s16 destinations[4][3];
    u16 horizontalNodes[3][5];
    u16 verticalNodes[3][5];
    s8 state;

    memcpy(destinations, D_00427668, sizeof(destinations));
    memcpy(horizontalNodes, D_00427680, sizeof(horizontalNodes));
    memcpy(verticalNodes, D_004276A0, sizeof(verticalNodes));
    state = mnuGetActiveMantraModelFlagState();

    if (state == 3) {
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        if (mdlFlagTest(0x977 + i)) {
            activeFlags |= 1 << i;
        }
    }
    for (i = 0; i < sizeof(bridgeIds) / sizeof(bridgeIds[0]); i++) {
        if (bridgeIds[i] == id) {
            if (i >= 2) {
                if (direction == 5 || direction == 0) {
                    return mnuGetMantraNodePositionRecord(destinations[i][state]);
                }
            } else {
                if (direction == 2 || direction == 3) {
                    return mnuGetMantraNodePositionRecord(destinations[i][state]);
                }
            }
            return NULL;
        }
    }
    for (i = 0; i < 5; i++) {
        if (horizontalNodes[state][i] == id &&
            (direction == 5 || direction == 0)) {
            if (i < 2) {
                if (activeFlags & 1) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[0]);
                }
                if (activeFlags & 2) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[1]);
                }
            } else {
                if (activeFlags & 2) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[1]);
                }
                if (activeFlags & 1) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[0]);
                }
            }
        }
    }
    for (i = 0; i < 5; i++) {
        if (verticalNodes[state][i] == id &&
            (direction == 2 || direction == 3)) {
            if (i < 2) {
                if (activeFlags & 4) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[2]);
                }
                if (activeFlags & 8) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[3]);
                }
            } else {
                if (activeFlags & 8) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[3]);
                }
                if (activeFlags & 4) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[2]);
                }
            }
        }
    }
    return NULL;
}

u32 mnuGetDefaultPanelSelector(MenuPanelObject *object) {
    return (u32)object->state.defaultSelector;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290A78);

INCLUDE_ASM(const s32, "game/code_0028FD30", mnuUpdateSelectedPanelSlot);

u16 mnuGetSelectedPanelValue(MenuPanelObject *object) {
    s32 values;

    values = (s32)object->state.slots[object->list->selected->index]->values;
    if (object->state.alternateSelector != 0) {
        return *(u16 *)(object->state.alternateSelector->selector.fields.index * 2 + values);
    }
    return *(u16 *)(object->state.defaultSelector->selector.fields.index * 2 + values);
}

u16 mnuGetPanelValueAt(MenuPanelObject *object, s32 index) {
    return *(u16 *)
                    (((index << 0x10) >> 0xf) +
                    (s32)object->state.slots[object->list->selected->index]->values);
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290C20);


typedef struct MantraModelFlag {
    u16 panelIndex : 8;
    u16 kind : 4;
    u16 state : 1;
    u16 unused : 3;
    u16 modelFlag;
    u8 variant;
    u8 pad05[3];
} MantraModelFlag;

extern void *memset(void *, s32, u32);
extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);

void mnuSynchronizeMantraModelFlags(s32 mode) {
    MantraModelFlag flags[112];
    MantraNodePos *node;
    s32 i = 175;
    s32 count;
    s32 variant;

    memset(flags, 0, sizeof(flags));
    node = mnuGetMantraNodePositionRecord(0);
    count = 0;
    for (; i >= 0; i--, node++) {
        if (node->selector.fields.index != 0) {
            s32 kind = node->selector.packed & 15;

            if (kind == 2 || kind == 4) {
                u32 packed;

                flags[count].panelIndex = node->selector.fields.index;
                packed = node->selector.packed;
                flags[count].kind = packed & 15;
                flags[count].state = (packed >> 8) & 1;
                flags[count].modelFlag = count + 0x923;
                count++;
            } else if (kind == 3) {
                for (variant = 0; variant < 6; variant++) {
                    u32 packed;

                    flags[count].panelIndex = node->selector.fields.index;
                    packed = node->selector.packed;
                    flags[count].kind = packed & 15;
                    flags[count].state = (packed >> 8) & 1;
                    flags[count].modelFlag = count + 0x923;
                    flags[count].variant = variant;
                    count++;
                }
            }
        }
    }
    for (i = 0; i < 112; i++) {
        if (flags[i].kind == 3) {
            switch (mode) {
            case 0:
                if (mdlFlagTest(flags[i].modelFlag)) {
                    mdlFlagSet(flags[i].modelFlag + 1);
                }
                i += 5;
                break;
            case 1:
                if (mdlFlagTest(flags[i].modelFlag + 1)) {
                    mdlFlagSet(flags[i].modelFlag);
                }
                i += 5;
                break;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291038);

extern void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags);
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

/* Write each panel slot's 0xB0 saved flag words back to its list node's script entries and log the slot number. */
INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427668);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427680);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276A0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276C0);

void mnuStoreMantraPanelFlagsToScript(MenuPanelObject *object) {
    MenuPanelNode *node = object->list->head;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        u32 context = node->value;
        u16 *values = object->state.slots[slotIndex]->values;
        s32 i;

        for (i = 0; i < 0xB0; i++) {
            scrSetEntryLowFlags(context, i, *values++);
        }
        evtPrintDeveloperConsoleMessage("[%d]Mantra Data Saved!!!!!!!!!!!!!!!!!\n", slotIndex++);
    }
}

extern u32 scrGetEntryLowFlags(s32 arg0, u16 index);

/* Read each list node's 0xB0 script entry flags into its panel slot and log the slot number. */
void mnuLoadMantraPanelFlagsFromScript(MenuPanelObject *object) {
    MenuPanelNode *node = object->list->head;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        s32 context = node->value;
        u16 *values = object->state.slots[slotIndex]->values;
        s32 i;

        for (i = 0; i < 0xB0; i++) {
            *values++ = scrGetEntryLowFlags(context, i);
        }
        evtPrintDeveloperConsoleMessage("[%d]Mantra Data Load!!!!!!!!!!!!!!!!!\n", slotIndex++);
    }
}

extern s32 scrGetSelectedScriptEntryId(s32 arg0);
extern u32 ptyGetProfileRecordCap(u16 scriptId);
extern u32 ptyGetProfileRecordValue(u32 work, u16 scriptId);

s32 mnuValidateProfileEntry(MenuPanelSlot *slot, s32 arg1) {
    u16 target = scrGetSelectedScriptEntryId(arg1) & 0xFFFF;
    u16 *dst = slot->values;
    s32 i;

    for (i = 0; i < 0xB0; i++) {
        u32 flags = scrGetEntryLowFlags(arg1, i);

        *dst++ = flags;
        if (i == target) {
            if (((flags >> 8) & 1) != 0) {
                target = 0;
            }
        }
    }
    if (target != 0 && ptyGetProfileRecordCap(target) == ptyGetProfileRecordValue(arg1, target)) {
        return target;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291338);

typedef struct MenuPanelPositionRecord {
    u16 id;
    u8 pad02;
    s8 flags;
} MenuPanelPositionRecord;

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291400);
extern const MenuPanelPositionRecord *func_00291400(s32 selector, u16 id);


void mnuActivatePanelSelection(MenuPanelObject *object, s8 selection) {
    MenuPanelState *state = &object->state;
    if (mnuQueueUnitPanelSelection(object->state.selectionController, selection) != 0) {
        u32 flags;
        u32 option;
        mnuMoveNodeCursorToTargetIndex(object, selection);
        mnuTransitionActivePanelAnimations(state->resource, 0);
        flags = state->flags;
        func_00291590(object, 5, 1, selection, (flags >> 28) & 1, 0);
        option = (selection & 0xf) << 24;
        state->flags = (state->flags & 0xf0ffffff) | option;
    }
}

void mnuSetPanelSelection(MenuPanelObject *object, s8 selection) {
    MenuPanelState *state = &object->state;
    if (mnuQueueUnitPanelSelection(object->state.selectionController, selection) != 0) {
        mnuMoveNodeCursorToTargetIndex(object, selection);
        state->flags = (state->flags & 0xf0ffffff)
            | ((selection & 0xf) << 24);
    }
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291590);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002917C0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291A20);


INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291C68);
INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291DD0);

void itfClearSelectionFlags(MenuPanelObject *object) {
    MenuPanelState *state = &object->state;
    state->flags &= 0xff0000ff;
}

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427740);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292478);

void func_00292998(MenuPanelObject *object) {
    u32 resource;
    s32 record;
    u64 effectHandle;

    resource = object->state.resource;
    mnuGetMantraPanelPositionRecord(0);
    record = func_00291400(0, 8);
    effectHandle = mnuFindPanelSlotById(resource, 8, 0);
    mnuQueuePanelAnimationTransition(effectHandle, 7, 0);
    effectHandle = mnuSpawnPanelSlotB(resource, 8, 1, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(effectHandle, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(effectHandle, 8, 0);
    *(u16 *)(record + 2) = (*(u16 *)(record + 2) & 0xfff0) | 1;
}

extern MantraNodePos *mnuGetMantraPanelPositionRecord(s16);
extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, u32);
extern void mnuSpawnMantraIconAtPosition(s32, s32, u32);
extern void func_00278EA8(u32);
extern void func_00279080(u32);

void func_00292A60(MenuPanelObject *object) {
    MenuPanelState *state = &object->state;

    state->defaultSelector = mnuGetMantraPanelPositionRecord(8);
    mnuSpawnMantraShortLoopIconAtPosition((s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
        (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f), object->state.selectionController);
    mnuSpawnMantraIconAtPosition((s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
        (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f), object->state.selectionController);
    func_00278EA8(object->state.selectionController);
    func_00279080(object->state.selectionController);
    state->flags &= ~0x20000000;
}

s32 func_00292B90(s32 object) {
    return func_002917C0(object, 0, 8);
}


extern void func_00278F60(u32);
extern void func_002790F0(s32, s32, u32);
void itfPositionMantraSelectionController(MenuPanelObject *object) {
    MantraNodePos *record;

    record = mnuGetMantraPanelPositionRecord(8);
    func_00278F60(object->state.selectionController);
    func_002790F0((s32)((f32)record->x / 10.0f * 40.0f), (s32)((f32)record->y / 10.0f * 39.0f), object->state.selectionController);
}

extern void itfInstallDefaultMantraSelector(MenuPanelObject *);

void itfInstallDefaultMantraSelector(MenuPanelObject *object) {
    MantraNodePos *record;
    MenuPanelState *state = &object->state;

    record = mnuGetMantraPanelPositionRecord(0x71);
    state->defaultSelector = record;
    mnuSpawnMantraIconAtPosition((s32)((f32)record->x / 10.0f * 40.0f),
        (s32)((f32)record->y / 10.0f * 39.0f), object->state.selectionController);
}

s32 func_00292CF0(MenuPanelObject *object) {
    MenuPanelTransition steps[23] = {
        {0, 22, 0, 0},
        {30, 22, 0, 4},
        {90, 22, 0, 1},
        {120, 0, 1, 6},
        {120, 75, 1, 0},
        {150, 75, 1, 4},
        {210, 75, 1, 1},
        {240, 15, 1, 0},
        {270, 15, 1, 4},
        {330, 15, 1, 1},
        {360, 0, 2, 6},
        {360, 69, 2, 0},
        {390, 69, 2, 4},
        {450, 69, 2, 1},
        {480, 39, 2, 0},
        {510, 39, 2, 4},
        {570, 39, 2, 1},
        {600, 113, 2, 0},
        {630, 113, 2, 7},
        {630, 113, 2, 5},
        {770, 113, 2, 8},
        {800, 113, 2, 1},
        {800, 0, 2, 15},
    };
    MenuPanelState *state = &object->state;
    u32 i;

    for (i = 0; i < 23; i++) {
        if (steps[i].frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((steps[i].mode & 15) << 24);
            if (func_00292478(object, steps[i].panelId, steps[i].action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}



s32 func_00292EA8(MenuPanelObject *object) {
    MenuPanelTransition steps[6] = {
        {0, 0, 0, 6}, {0, 0x16, 0, 0},
        {30, 0x16, 0, 9}, {30, 0x16, 0, 11},
        {30, 0x16, 0, 12}, {60, 0x16, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MenuPanelState *state = &object->state;
    u32 i;

    for (i = 0; i < 6; i++, step++) {
        if (step->frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((step->mode & 15) << 24);
            if (func_00292478(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

s32 func_00292FF0(MenuPanelObject *object) {
    MenuPanelTransition steps[7] = {
        {0, 0, 1, 13}, {0, 15, 1, 12},
        {30, 0, 2, 13}, {30, 0x71, 2, 12},
        {60, 0, 0, 13}, {60, 0x16, 0, 12},
        {100, 0x16, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MenuPanelState *state = &object->state;
    u32 i;

    for (i = 0; i < 7; i++, step++) {
        if (step->frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((step->mode & 15) << 24);
            if (func_00292478(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

s32 func_00293148(MenuPanelObject *object) {
    MenuPanelTransition steps[8] = {
        {0, 0, 0, 10}, {30, 1, 0, 0},
        {60, 1, 0, 4}, {150, 1, 0, 1},
        {180, 1, 0, 3}, {180, 1, 0, 14},
        {280, 1, 0, 2}, {280, 0, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MenuPanelState *state = &object->state;
    u32 i;

    for (i = 0; i < 8; i++, step++) {
        if (step->frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((step->mode & 15) << 24);
            if (func_00292478(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002932B0);

void mnuCollectPanelNodeValues(MenuPanelObject *object) {
    MenuPanelNode *node = object->list->head;
    s32 count = 0;
    MenuPanelState *state = &object->state;
    if (node != 0) {
        u32 *slot = object->state.collectedValues;
        do {
            u32 value = node->value;
            count++;
            node = node->next;
            *slot++ = value;
        } while (node != 0);
    }
    state->collectedCount = count;
    state->savedSelection = object->state.selectionIndex;
}

void func_002933A8(MenuPanelObject *object) {
    object->state.flagBytes[0] = 3;
    object->state.unk9B5 = 0;
    object->state.unk9B6 = 0;
    object->state.flags = (object->state.flags & 0xF0FFFFFF) | 0x20000000;
    func_00291338();
}



INCLUDE_ASM(const s32, "game/code_0028FD30", func_002933F0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293DB0);

s32 func_00293FD0(MenuPanelObject *object, u16 id) {
    MenuPanelState *state = &object->state;
    const MenuPanelPositionRecord *record;
    s32 selector;
    s32 result = -1;

    for (selector = 0; selector < 3; selector++) {
        if (((state->flags >> 24) & 0xF) == selector) {
            continue;
        }
        record = func_00291400(selector, id);
        if ((record->flags & 1) != 0) {
            result = selector;
            break;
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00294060);

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern void *memset(void *, s32, u32);
/* Allocate twenty countdown entries for sounds attached to this panel. */
void mnuInitPanelSoundEntries(void) {
    s32 handle;
    MenuPanelEntryPool *pool;

    if (mnuPanelSoundEntryPool == 0) {
        handle = sdfAllocGeneralBlock(0xAC);
        mnuPanelSoundEntryPool = sdfMemoryGetBlockAddress(handle);
        memset(mnuPanelSoundEntryPool, 0, 0xAC);
        pool = (MenuPanelEntryPool *)mnuPanelSoundEntryPool;
        pool->entries = (MenuPanelEntry *)(pool + 1);
        pool->count = 0x14;
        pool->allocation = handle;
    }
}

void mnuReleasePanelEntryPool(void) {
    if (mnuPanelSoundEntryPool != (u32 *)0x0) {
        sdfReleaseResourceAllocation(((MenuPanelEntryPool *)mnuPanelSoundEntryPool)->allocation);
    }
    mnuPanelSoundEntryPool = (u32 *)0x0;
}

extern void sndSetSequenceVolumePan(s32, s32, s32);
void mnuTickPanelSoundEntries(void) {
    s32 i;
    MenuPanelEntry *entry = ((MenuPanelEntryPool *)mnuPanelSoundEntryPool)->entries;

    for (i = 0; i < ((MenuPanelEntryPool *)mnuPanelSoundEntryPool)->count; i++, entry++) {
        if (entry->soundHandle != 0) {
            if (entry->framesRemaining == 0) {
                sndSetSequenceVolumePan(entry->soundHandle, 0x7F, 0x3F);
                entry->soundHandle = 0;
            }
            if (entry->framesRemaining > 0) {
                entry->framesRemaining--;
            }
        }
    }
}

MenuPanelEntry *mnuFindFreePanelEntry(void) {
    MenuPanelEntryPool *pool = (MenuPanelEntryPool *)mnuPanelSoundEntryPool;
    s32 count = pool->count;
    MenuPanelEntry *entry = pool->entries;
    s32 index;
    for (index = 0; index < count; index++, entry++) {
        if (entry->soundHandle == 0) {
            return entry;
        }
    }
    return 0;
}

void mnuStorePanelEntry(s32 soundHandle, s32 framesRemaining) {
    MenuPanelEntry *entry = mnuFindFreePanelEntry();
    entry->soundHandle = soundHandle;
    entry->framesRemaining = framesRemaining;
}

void func_002945B8(MenuTerminalContext *value) {
    D_00438FC8 = value;
}
INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437950);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437958);

INCLUDE_SDATA(const s32, "game/code_0028FD30", mnuPanelSoundEntryPool);

