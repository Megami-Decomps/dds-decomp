#include "common.h"

typedef struct MenuPanelTransition {
    u16 frame;
    u16 panelId;
    u16 mode;
    u16 action;
} MenuPanelTransition;

extern s32 func_00292478(void *, s32, s32);

extern u32 D_00438FC8;

extern u32 *mnuPanelSoundEntryPool;

extern u64 mnuSpawnPanelSlotB(u32, u64, u64, u64, u64, u64);

extern u64 mnuFindPanelSlotById(u32, u64, u64);

extern s32 func_002890A8(s32);

extern void func_0026D168(s32, s32, s32);

extern s32 evtAllocateMantraSelectionWork(s32, s32);

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

typedef struct MenuPanelSelector {
    u16 pad00;
    s16 index;
} MenuPanelSelector;

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
    u8 pad00[0x5D4];
    s32 savedSelection;
    s32 collectedCount;
    u8 pad5DC[0x3D0];
    u32 resource;
    u32 flags;
} MenuPanelState;

typedef struct MenuPanelObject {
    u8 pad00[4];
    MenuPanelList *list;
    u8 pad08[0x798];
    MenuPanelSelector *defaultSelector;
    MenuPanelSelector *alternateSelector;
    u8 pad7A8[4];
    MenuPanelSlot *slots[18];
    u32 collectedValues[8];
    s32 savedSelection;
    s32 collectedCount;
    u8 pad81C[4];
    s8 selectionIndex;
    u8 pad821[0x3CB];
    u32 resource;
    u32 flags;
    u8 padBF4[0xC];
    s32 selectionController;
} MenuPanelObject;

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427560);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FD30);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FEF0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275B0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275E8);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290240);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290328);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290410);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427668);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002906E0);

u32 mnuGetDefaultPanelSelector(MenuPanelObject *object) {
    return (u32)object->defaultSelector;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290A78);

s32 mnuUpdateSelectedPanelSlot(MenuPanelObject *object) {
    s32 index = func_002890A8((s32)object);
    /* Required to match: typed &object->slots[index] changes two instructions. */
    MenuPanelSlot **slot = (MenuPanelSlot **)(index * 4 + (s32)object + 0x7ac);
    s32 source = object->list->selected->value;
    if (*slot != 0) {
        func_0026D168((s32)*slot, source, 0);
    } else {
        *slot = (MenuPanelSlot *)evtAllocateMantraSelectionWork(source, 0);
    }
    return 1;
}

u16 mnuGetSelectedPanelValue(MenuPanelObject *object) {
    s32 values;

    values = (s32)object->slots[object->list->selected->index]->values;
    if (object->alternateSelector != 0) {
        return *(u16 *)(object->alternateSelector->index * 2 + values);
    }
    return *(u16 *)(object->defaultSelector->index * 2 + values);
}

u16 mnuGetPanelValueAt(MenuPanelObject *object, s32 index) {
    return *(u16 *)
                    (((index << 0x10) >> 0xf) +
                    (s32)object->slots[object->list->selected->index]->values);
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290C20);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290E48);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291038);

extern void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags);
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

/* Write each panel slot's 0xB0 saved flag words back to its list node's script entries and log the slot number. */
INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276C0);

void mnuStoreMantraPanelFlagsToScript(MenuPanelObject *object) {
    MenuPanelNode *node = object->list->head;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        u32 context = node->value;
        u16 *values = object->slots[slotIndex]->values;
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
        u16 *values = object->slots[slotIndex]->values;
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

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291400);

void mnuActivatePanelSelection(MenuPanelObject *object, s8 selection) {
    MenuPanelState *state = (MenuPanelState *)((u8 *)object + 0x240);
    if (mnuQueueUnitPanelSelection(object->selectionController, selection) != 0) {
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
    MenuPanelState *state = (MenuPanelState *)((u8 *)object + 0x240);
    if (mnuQueueUnitPanelSelection(object->selectionController, selection) != 0) {
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

void itfClearSelectionFlags(u8 *object) {
    MenuPanelState *state = (MenuPanelState *)(object + 0x240);
    state->flags &= 0xff0000ff;
}

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427740);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292478);

void func_00292998(MenuPanelObject *object) {
    u32 resource;
    s32 record;
    u64 effectHandle;

    resource = object->resource;
    mnuGetMantraPanelPositionRecord(0);
    record = func_00291400(0, 8);
    effectHandle = mnuFindPanelSlotById(resource, 8, 0);
    mnuQueuePanelAnimationTransition(effectHandle, 7, 0);
    effectHandle = mnuSpawnPanelSlotB(resource, 8, 1, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(effectHandle, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(effectHandle, 8, 0);
    *(u16 *)(record + 2) = (*(u16 *)(record + 2) & 0xfff0) | 1;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292A60);

s32 func_00292B90(s32 object) {
    return func_002917C0(object, 0, 8);
}


extern MenuPanelSelector *mnuGetMantraPanelPositionRecord(s32);
extern void func_00278F60(u32);
extern void func_002790F0(s32, s32, u32);
extern void mnuSpawnMantraIconAtPosition(s32, s32, u32);
void itfPositionMantraSelectionController(MenuPanelObject *object) {
    s16 *record;

    record = (s16 *)mnuGetMantraPanelPositionRecord(8);
    func_00278F60(object->selectionController);
    func_002790F0((s32)((f32)record[2] / 10.0f * 40.0f), (s32)((f32)record[3] / 10.0f * 39.0f), object->selectionController);
}

/* Install the panel's default selector and position its selection controller. */
void itfInstallDefaultMantraSelector(MenuPanelObject *object) {
    u8 *base = (u8 *)object + 0x240;
    s16 *record;

    record = (s16 *)mnuGetMantraPanelPositionRecord(0x71);
    *(s16 **)(base + 0x560) = record;
    /* Record coordinates are tenths; the two screen axes use different scales. */
    mnuSpawnMantraIconAtPosition((s32)((f32)record[2] / 10.0f * 40.0f), (s32)((f32)record[3] / 10.0f * 39.0f), object->selectionController);
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292CF0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004277A0);

s32 func_00292EA8(MenuPanelObject *object) {
    MenuPanelTransition steps[6] = {
        {0, 0, 0, 6}, {0, 0x16, 0, 0},
        {30, 0x16, 0, 9}, {30, 0x16, 0, 11},
        {30, 0x16, 0, 12}, {60, 0x16, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MenuPanelState *state = (MenuPanelState *)((u8 *)object + 0x240);
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
    MenuPanelState *state = (MenuPanelState *)((u8 *)object + 0x240);
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
    MenuPanelState *state = (MenuPanelState *)((u8 *)object + 0x240);
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
    MenuPanelState *state = (MenuPanelState *)((u8 *)object + 0x240);
    if (node != 0) {
        u32 *slot = object->collectedValues;
        do {
            u32 value = node->value;
            count++;
            node = node->next;
            *slot++ = value;
        } while (node != 0);
    }
    state->collectedCount = count;
    state->savedSelection = object->selectionIndex;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002933A8);



INCLUDE_ASM(const s32, "game/code_0028FD30", func_002933F0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293DB0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293FD0);

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
        pool->entries = (MenuPanelEntry *)((u8 *)pool + 0xC);
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

void func_002945B8(u32 value) {
    D_00438FC8 = value;
}
INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437940);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437948);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437950);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437958);

INCLUDE_SDATA(const s32, "game/code_0028FD30", mnuPanelSoundEntryPool);

