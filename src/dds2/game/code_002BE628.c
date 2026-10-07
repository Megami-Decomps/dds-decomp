#include "common.h"
#include "dat_command.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "pcp_vu0.h"
#include "mnu.h"
#include "mdl.h"
#include "eff.h"
#include "dat_state.h"

extern void func_00306CD0(s32, s32, s32, u32, s32, s32, s32, s32);

#define MNU_PANEL_ITEM_COUNT 5
#define MNU_PANEL_STATE_BYTES 0x8C
#define MNU_PANEL_GROUP_BYTES 0x2C
#define MNU_SPRITE_STATE_BYTES 0x20
#define MNU_SIMPLE_SPRITE_BYTES 0x20
#define MNU_PANEL_ITEM_BYTES 0xAC
#define MNU_PROFILE_PANEL_BYTES 0x48
#define MNU_TRANSITION_LIMIT 0x100
#define MNU_TRANSITION_STEP 8
#define MNU_GRADIENT_FADE_STEP 0x20
#define MNU_PROFILE_PHASE_PERIOD 0x200
#define MNU_PROFILE_PHASE_STEP 1
#define MNU_NO_SELECTION 0xffffffff
#define MNU_PANEL_TEXTURE_COUNT 7
#define MNU_POPUP_STATE_BYTES 0x4c
#define MNU_POPUP_INSERT_BEFORE_TOP 0x20000
#define MNU_POPUP_ENTRY_MARK_BITS 0x60000
#define MNU_PARTY_SLOT_COUNT 5
#define MNU_INPUT_PRIORITY_BIT 1
#define MNU_PAD_TRIGGER_BIT 2
#define MNU_COMMAND_RECORD_BYTES 0x38
#define MNU_COMMAND_ID_MASK 0xFFFF
#define MNU_COST_KIND_HP 1
#define MNU_COST_KIND_MP 2
#define MNU_PERCENT_SCALE 100
#define MNU_COMMAND_USE_STATUS_BOUNDARY 0x220
#define MNU_RATIO_QUARTER_PERCENT 25
#define MNU_RATIO_HALF_PERCENT 50

#define EVT_STAGE_ENTRY_INDEX_MASK 0xFFFF
#define EVT_STAGE_ENTRY_BYTES 60
#define EVT_STAGE_MODEL_RECORD_BYTES 624
#define EVT_STAGE_COLUMN_VALUE_COUNT 8
#define EVT_STAGE_QUEUE_PENDING 1
#define EVT_STAGE_QUEUE_REQUESTING 2
#define EVT_STAGE_QUEUE_SETUP_COMPLETE 4
#define EVT_STAGE_MOTION_IDLE 0
#define EVT_STAGE_MOTION_QUEUED 1
#define EVT_STAGE_MOTION_PLAYING 2
#define EVT_STAGE_MOTION_FALLBACK_STARTED 3
#define EVT_STAGE_MOTION_FORCE_FALLBACK 4
#define EVT_STAGE_MOTION_SUPPRESS_FALLBACK 1
#define EVT_STAGE_DEFAULT_BLEND_FRAMES 15.0f
#define EVT_STAGE_PROJECTION_BIAS 2048.0f
#define EVT_STAGE_DEFAULT_TARGET_Z -400.0f
#define EVT_STAGE_SCREEN_WORK_BYTES 0x20
#define EVT_STAGE_RESOURCE_TASK_KIND 6
#define EVT_STAGE_USE_ENTRY_MOTION 0xffffffffffffffff

typedef struct MenuPanelItem MenuPanelItem;
extern s32 D_00437C9C;
extern s32 func_002B8E30();
extern s32 mnuScrollListToEnd();
extern void mnuClearWindowPanelTransitionFlag();
extern void func_002BE730();
extern void func_002BED10();
extern s32 func_002C6008();

extern s32 mnuLookupRangeEntry(u16);

extern s32 ptyGetCombinedRecordAndSlotValue(s32, s32);


extern s32 func_002C6CE8(void);

extern struct MenuIconState *func_002B9FF8(u32 mode, s32 resource, ...);
extern void mnuReleaseResourceList(struct MenuIconState *list);

extern u32 effCreateStatusBatch(u32);


extern s32 datCommandSelectors;



extern s8 D_003E7970[];
extern u8 D_003E7978[];

extern void evtStageTestQueueMotionSegment(u32, f32, f32);
extern void evtStageTestCreateModelEffect(s32);
extern void evtStageTestUpdateCamera(void);
extern void btlUpdateJobPositionFromModel(s32);
extern void mdlProcessContextNodesAndTransforms(s32, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, u16, s32, s32);
extern u32 ptyGetSkillNibbleState(s32, u16);

extern void evtStageTestStop(void);

extern u8 D_003E7940[];

extern u8 D_003E7950[];

extern s32 sdfAllocSizeClassBlock(u32);

extern void mnuInitializePanelItemGridSlots(MenuPanelItem *, s32, s32);

extern s8 D_003E7928[];
extern s32 mdlRequestAsset(s32, s32, s32);


extern u16 D_003E7900[];

extern u16 D_003E7902[];

extern s8 D_003E792A[];



extern void mdlAddEntryFlaggedEx(s32, s32, s32, f32, f32);

extern void mdlAddEntryPlainEx(s32, s32, s32, f32, f32);

extern void evtStageTestAdvanceMotionQueue(void);



extern EffWorldNode *evtCreateWorldObjectAtTransform(f32 *, f32 *);

extern char D_00437CB0[];

extern void mnuFreePanelItemWork(MenuPanelItem *);

extern void sdfReleaseChipBlock();

extern u32 effMiscRand(s32);


extern u16 D_003E78D8[];

typedef struct StageTestEntry {
    u8 modelId;      /* 0x00 */
    u8 motionIndex;  /* 0x01 */
    u8 column[2][8]; /* 0x02: per-column flag bytes (0x02.. and 0x0A..) */
    u8 pad12[6];
    f32 frame;       /* 0x18 */
    f32 position[3]; /* 0x1C */
    f32 pad28;
    f32 rotation[3]; /* 0x2C */
    f32 pad38;
} StageTestEntry;

typedef struct StageTestSlot {
    s32 entryIndex; /* 0x00 */
    s32 assetResource; /* 0x04: mode-selected resource for the model request */
    s32 modelId;    /* 0x08 */
    s32 assetOption; /* 0x0C: third argument forwarded to mdlRequestAsset */
    s32 initialMotionIndex; /* 0x10: negative uses the entry's motionIndex */
    u32 flags;      /* 0x14: bit 0 suppresses the fallback motion */
    s32 state;      /* 0x18: 0 idle, 1 queued, 2 playing, 3 fallback started, 4 forced fallback */
    s32 motionIndex; /* 0x1C */
    s32 blendLeadFrames; /* 0x20: negated initial motion time during blending */
    s32 blendDurationFrames; /* 0x24: duration used to normalize the blend weight */
} StageTestSlot;

/* The request poller passes this embedded queue, beginning at the flags word.
 * Each slot is 0x28 bytes; slot 0 is active and slot 1 stages the next selection. */
typedef struct StageTestQueue {
    u32 flags; /* bit 0: pending slot; bit 1: request in progress; bit 2: setup complete */
    StageTestSlot slot[2]; /* 0x04 and 0x2C */
} StageTestQueue; /* 0x54 */

/* Battle stage test viewer state. Retail addresses it partly through
 * D_00457EC8 (= &evtStageTestState.queue.slot[0], hence the negative offsets), so the
 * D_00457EB4..D_00457F1C symbols are all interior fields of this one object. */
typedef struct StageTestState {
    s32 mode;                /* 0x00 */
    s32 assetRequest;        /* 0x04: result of mdlRequestAsset */
    s32 model;               /* 0x08 */
    s8 flag;                 /* 0x0C */
    StageTestEntry *entries; /* 0x10 */
    StageTestQueue queue;    /* 0x14: flags followed by the two selection slots */
    s32 effect;              /* 0x68 */
    s32 pendingEffect;       /* 0x6C */
    s32 modelUpdateStarted;  /* 0x70: DDS2 only, defers effect/motion work on the first update */
} StageTestState;

extern StageTestState evtStageTestState;

extern void func_00340DC8(f32, f32, f32);

extern char D_0042B610[];


extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void kwlnDrawSpriteCell(void *, s32, s32, s32, s32);
extern void evtCreateWorldObjectForKey(s32, s32);
extern s32 D_00437CB8;
extern s32 D_00437CBC;
extern SdfPoolNode D_00380708;
extern s8 D_0037F510[];

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE628);




typedef struct MenuList MenuList;


/* Set the separate +0x100 word in both content banks of every page. */
void mnuSetPanelSlotValues(MenuPageWindow *menu, s32 value) {
    MenuPageSlot *panel = menu->slots;
    s32 i;
    s32 j;

    for (i = 0; i < 5; i++, panel++) {
        for (j = 0; j < 2; j++) {
            panel->contents[j].unk100 = value;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE730);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BED10);

void func_002BEE38(MenuQueuedCommand *entry) {
    entry->unkC = 0x18;
    entry->unk10 = 5;
    entry->unk0 = 0;
}


/* Queue in the least-valued command bank; use bank zero if neither value
 * is below 0x200. Retail has no explicit return: its last call is a plain jal. */
s32 mnuQueueListEntry(MenuPageWindow *menu, s32 window, u32 kind, s32 argument) {
    MenuPageSlot *panel = &menu->slots[window];
    s32 *count = &panel->contents[0].command.initialValue;
    s32 best = 0x200;
    s32 bestIndex = 0;
    s32 i;
    MenuQueuedCommand *entry;
    u32 flags;

    /* Selection words are one content-bank stride apart. */
    for (i = 0; i < 2; i++, count += sizeof(MenuPageSlotContent) / sizeof(*count)) {
        if (*count < best) {
            best = *count;
            bestIndex = i;
        }
    }
    flags = datGameState->party[window].flags;
    entry = &panel->contents[bestIndex].command;
    entry->initialValue = 0x200;
    entry->argument = argument;
    entry->kind = kind;
    if ((flags & 2) != 0) {
        entry->option = 0;
    } else {
        entry->option = 1;
    }
    switch (kind) {
    case 0:
        func_002BE730(entry);
        menu->fade = 0;
        break;
    case 1:
        func_002BED10(entry);
        menu->fade = 0;
        break;
    case 2:
        func_002BEE38(entry);
        break;
    }
}

void mnuClearSpriteRecord(MenuQueuedCommand *entry) {
    entry->unk0 = 0;
    entry->kind = 0;
    entry->option = 0;
    entry->unkC = 0;
    entry->unk10 = 0;
    entry->initialValue = 0;
    entry->argument = 0;
}

/* Clear both command headers; they are embedded in separate content banks. */
void mnuClearPairedSpriteRecords(MenuPageWindow *menu, s32 index) {
    MenuQueuedCommand *entry;
    s32 remaining;

    remaining = 1;
    entry = &menu->slots[index].contents[0].command;
    do {
        remaining = remaining - 1;
        mnuClearSpriteRecord(entry);
        entry = (MenuQueuedCommand *)((u8 *)entry + sizeof(MenuPageSlotContent));
    } while (-1 < remaining);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF000);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF238);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF478);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF660);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B0D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF830);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BFEA0);


extern void func_002BF830(s32, s32, s32, MenuPageWindow *, s32, s32);
extern void func_002BFEA0(s32, s32, s32, MenuPageWindow *, s32, s32);

void mnuDispatchListPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu, s32 panelIndex, s32 param) {
    MenuPageSlot *panel = &menu->slots[panelIndex];
    s32 mode = panel->kind;

    if (menu->selected >= 0) {
        mode = 1;
    }
    if (panel->flags & 0x40) {
        mode = 1;
    }
    switch (mode) {
    case 1:
        func_002BF830(x, y, z, menu, panelIndex, param);
        return;
    case 2:
        func_002BFEA0(x, y, z, menu, panelIndex, param);
        break;
    }
}

typedef struct MenuSpacing {
    s32 step;
    s32 gap;
    s32 tail;
} MenuSpacing;

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B118);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B130);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B140);

void mnuCalcListEntryOffset(s32 *out, MenuPageWindow *menu, s32 index) {
    MenuSpacing spacing = {0x310, 0x370, 0x190};
    s32 count = menu->records->unk0;
    s32 mode;

    if (!(menu->slots[index].flags & 0x40)) {
        mode = index < count ? 1 : 2;
    } else {
        mode = 1;
    }
    if (menu->selected >= 0) {
        out[0] = 0xC80;
        out[1] = 0x20;
    } else {
        out[0] = 0xF00;
        out[1] = 0x28;
    }
    switch (mode) {
    case 1:
        out[1] += index * spacing.step;
        break;
    case 2:
        out[1] += (count - 1) * spacing.step;
        if (index >= count) {
            out[1] += spacing.gap;
        }
        out[1] += (index - count) * spacing.tail;
        break;
    }
}

/* Increment while below the transition limit; the native add is not clamped. */
void mnuAdvancePanelTransition(MenuPageWindow *menu) {
    if (menu->transitionValue < MNU_TRANSITION_LIMIT) {
        menu->transitionValue = menu->transitionValue + MNU_TRANSITION_STEP;
    }
}

/* Apply a temporary override to the selected node while drawing its panel. */
void mnuDrawPanelWithTemporaryOverride(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 param) {
    s32 positionOffset[2];
    MenuPageSlot *panel = &menu->slots[menu->selected];
    MenuSprites *node;

    mnuCalcListEntryOffset(positionOffset, menu, 0);
    node = panel->contents[0].windowSprites;
    if (node != NULL) {
        node->unkC = overrideValue;
    }
    x += menu->scrollOffset * 0x10;
    menu->scrollOffset = (s32)((f32)menu->scrollOffset / 1.19999993f);
    /* Both arms are identical in retail; kept as written. */
    if (menu->flags & 0x80) {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selected, param);
    } else {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selected, param);
    }
    node = panel->contents[0].windowSprites;
    if (node != NULL) {
        node->unkC = 0;
    }
}

/* Draw the selected panel or all visible/additional panels, then advance the transition. */
void mnuDrawListPanels(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 param) {
    s32 positionOffset[2];
    PartyPanel *layout = menu->records;
    s32 panelCount;
    s32 panelIndex;

    panelCount = layout->unk0;
    panelCount += layout->unk4;
    if (menu->selected >= 0) {
        mnuDrawPanelWithTemporaryOverride(x, y, z, overrideValue, menu, param);
    } else {
        for (panelIndex = 0; panelIndex < panelCount; panelIndex++) {
            mnuCalcListEntryOffset(positionOffset, menu, panelIndex);
            mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, panelIndex, param);
        }
    }
    mnuAdvancePanelTransition(menu);
}

void mnuDrawPanelListDefault(s32 x, s32 y, s32 depth, s32 source, s32 mode, s32 option) {
    mnuDrawListPanels(x, y, depth, 0, (MenuPageWindow *)source, mode);
}


typedef struct MenuPanelState {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    u32 state; /* 0x14 */
    u32 firstValueA; /* 0x18 */
    u32 firstValueB; /* 0x1C */
    MenuPoint firstPosition; /* 0x20 */
    MenuPoint corners[5]; /* 0x28 */
    MenuPoint guideStart; /* 0x50 */
    MenuPoint guideEnd; /* 0x58 */
    MenuPoint secondPosition; /* 0x60 */
    u32 secondValueA; /* 0x68 */
    u32 secondValueB; /* 0x6C */
    MenuPoint thirdPosition; /* 0x70 */
    u32 thirdValueA; /* 0x78 */
    u32 thirdValueB; /* 0x7C */
    u32 thirdValueC; /* 0x80 */
    u8 pad84[4];
    struct MenuIconState *resourceHandle; /* 0x88 */
} MenuPanelState;

typedef char MenuPanelStateSizeCheck[(sizeof(MenuPanelState) == MNU_PANEL_STATE_BYTES) ? 1 : -1];
typedef char MenuPanelStateResourceHandleOffsetCheck[((u32)&((MenuPanelState *)0)->resourceHandle == 0x88) ? 1 : -1];

/* Allocate a zeroed native panel state with the requested dimensions. */
MenuPanelState *mnuCreatePanelState(s32 width, s32 height) {
    MenuPanelState *panel = (MenuPanelState *)sdfAllocSizeClassBlock(MNU_PANEL_STATE_BYTES);

    memset(panel, 0, MNU_PANEL_STATE_BYTES);
    panel->width = width;
    panel->height = height;
    return panel;
}

void mnuDestroyPanelState(MenuPanelState *panel) {
    struct MenuIconState *resourceHandle;

    resourceHandle = panel->resourceHandle;
    if (resourceHandle != 0) {
        mnuReleaseResourceList(resourceHandle);
    }
    sdfReleaseChipBlock(panel);
}

void func_002C07D8(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->firstValueA = valueA;
    panel->firstValueB = valueB;
    itfGridStorePosition(&panel->firstPosition, x, y);
}

void mnuSetPanelCornerGeometry(MenuPanelState *panel, s32 x, s32 y, s32 guideX, s32 guideTopY, s32 guideBottomY) {
    itfGridStorePosition(&panel->guideStart, guideX, guideTopY);
    itfGridStorePosition(&panel->guideEnd, guideX, guideBottomY);
    panel->corners[0].x = x;
    panel->corners[0].y = y;
    panel->corners[1].x = x + 0xC0;
    panel->corners[1].y = y - 0x20;
    panel->corners[2].x = x - 0x50;
    panel->corners[2].y = y + 0x68;
    panel->corners[3].x = x + 0x1D0;
    panel->corners[3].y = y + 0x68;
    panel->corners[4].x = x + 0xC0;
    panel->corners[4].y = y + 0xF0;
}

void mnuInitializePanelResource(MenuPanelState *panel, s32 resource) {
    panel->resourceHandle = func_002B9FF8(5, resource);
}

void func_002C08E0(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->secondValueA = valueA;
    panel->secondValueB = valueB;
    itfGridStorePosition(&panel->secondPosition, x, y);
}

void func_002C0908(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 additionalValue) {
    panel->thirdValueA = valueA;
    panel->thirdValueB = valueB;
    itfGridStorePosition(&panel->thirdPosition, x, 0);
    panel->thirdValueC = additionalValue;
}

void mnuSetPanelState(MenuPanelState *panel, u32 state) {
    panel->state = state;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0958);

extern MenuPanelItem *mnuCreatePanelItem(void);

extern void mnuInitializePanelGroupGridSlots(MenuPanelItem *, s32, s32, s32, s32);

extern void mnuClearPanelGroupSelection(MenuPanelGroup *);

extern void mnuSetPanelItemSelection(MenuPanelItem *, s32);
extern void mnuSetPanelItemOption(MenuPanelItem *, u32);
extern void mnuStorePanelItemValue(MenuPanelItem *, u32);

/* Create the five panel items owned by this group and clear its selection. */
MenuPanelGroup *mnuCreatePanelGroup(s32 owner, s32 texture, s32 mode) {
    MenuPanelGroup *group = (MenuPanelGroup *)sdfAllocSizeClassBlock(MNU_PANEL_GROUP_BYTES);
    MenuPanelItem **itemCursor = group->entries;
    s32 panelIndex;
    for (panelIndex = 0; panelIndex < MNU_PANEL_ITEM_COUNT; panelIndex++) {
        MenuPanelItem *panelItem = mnuCreatePanelItem();
        mnuInitializePanelGroupGridSlots(panelItem, owner, texture, mode, panelIndex);
        *itemCursor++ = panelItem;
    }
    mnuClearPanelGroupSelection(group);
    group->texture = texture;
    group->initialValue = 0x100;
    return group;
}

/* Release every owned panel item before releasing the group allocation. */
void mnuDestroyPanelGroup(MenuPanelGroup *group) {
    s32 panelIndex;

    for (panelIndex = 0; panelIndex < MNU_PANEL_ITEM_COUNT; panelIndex++) {
        mnuFreePanelItemWork(group->entries[panelIndex]);
    }
    sdfReleaseChipBlock(group);
}

/* Configure all five panel items against the same grid object. */
void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 gridObject) {
    s32 panelIndex;

    for (panelIndex = 0; panelIndex < MNU_PANEL_ITEM_COUNT; panelIndex++) {
        mnuInitializePanelItemGridSlots(group->entries[panelIndex], gridObject, panelIndex);
    }
}

void mnuSetPanelGroupSelection(MenuPanelGroup *group, u32 selection) {
    group->selection = selection;
}

/* Store the native unsigned no-selection sentinel. */
void mnuClearPanelGroupSelection(MenuPanelGroup *group) {
    group->selection = MNU_NO_SELECTION;
}

u32 mnuGetPanelGroupSelection(MenuPanelGroup *group) {
    return group->selection;
}

INCLUDE_ASM(const s32, "game/code_002BE628", mnuDrawAndAdvancePanelGroup);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selection, u32 option) {
    mnuSetPanelItemSelection(group->entries[index], selection);
    mnuSetPanelItemOption(group->entries[index], option);
}

void mnuSetIndexedPanelGroupValue(MenuPanelGroup *group, s32 index, u32 value) {
    mnuStorePanelItemValue(group->entries[index], value);
}

/* Apply the selected item's five stat values to the panel entries. */
void mnuApplyPackedGroupValues(MenuPanelGroup *group, s32 itemId) {
    u32 child;
    s32 entryValue;
    s32 nextIndex;
    u32 *entries;
    s32 index;

    entries = (u32 *)group->entries;
    index = 0;
    do {
        nextIndex = index + 1;
        entryValue = ptyGetCombinedRecordAndSlotValue(itemId, index);
        child = *entries;
        entries = entries + 1;
        mnuStorePanelItemValue((MenuPanelItem *)child, entryValue);
        index = nextIndex;
    } while (nextIndex < 5);
}

/* Create the sprite-resource state with its native initial value. */
MenuSpriteState *mnuCreateSpriteState(struct EffectSlotSet *resourceSet0,
                                      struct EffectSlotSet *resourceSet1,
                                      struct EffectSlotSet *resourceSet2) {
    MenuSpriteState *spriteState = sdfAllocSizeClassBlock(MNU_SPRITE_STATE_BYTES);
    memset(spriteState, 0, MNU_SPRITE_STATE_BYTES);
    spriteState->resourceSets[0] = resourceSet0;
    spriteState->resourceSets[1] = resourceSet1;
    spriteState->resourceSets[2] = resourceSet2;
    spriteState->initialValue = 0x100;
    return spriteState;
}

void mnuFreeSpriteStateWork(MenuSpriteState *spriteState) {
    sdfReleaseChipBlock(spriteState);
}

/* The sequel selects its draw variant from the range index plus eight. */
void mnuDrawRangeSpriteVariant(u32 x, u32 y, u32 depth, u32 color,
                                    u16 rangeId, u32 drawArg, u32 texture) {
    s32 rangeIndex;

    rangeIndex = mnuLookupRangeEntry(rangeId);
    func_00306CD0(x, y, depth, color, 1, drawArg, rangeIndex + 8, texture);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C10F0);

/* Create the DDS2 simple sprite state from its three resource sets. */
MenuSpriteState *mnuAllocateSimpleSprite(struct EffectSlotSet *resourceSet0,
                                       struct EffectSlotSet *resourceSet1,
                                       struct EffectSlotSet *resourceSet2) {
    MenuSpriteState *sprite = sdfAllocSizeClassBlock(MNU_SIMPLE_SPRITE_BYTES);
    memset(sprite, 0, MNU_SIMPLE_SPRITE_BYTES);
    sprite->resourceSets[0] = resourceSet0;
    sprite->resourceSets[1] = resourceSet1;
    sprite->resourceSets[2] = resourceSet2;
    sprite->initialValue = 0x100;
    return sprite;
}

void mnuFreeSimpleSpriteWork(MenuSpriteState *sprite) {
    sdfReleaseChipBlock(sprite);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C16F0);


void mnuResetGradientFadeColor(MenuGradientFade *state, s32 color) {
    state->color = color;
    state->active = 0;
    state->blend = 0;
}

void func_002C1B68(u32 *out, u32 value) {
    *out = value;
}

extern u32 uiBlendColors(u32, u32, u32);
extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, u32, u32);

/* Draw two clear corners and two blended corners, then step the bounded blend value. */
void mnuDrawAndStepGradientFade(MenuGradientFade *state, s32 surface) {
    s32 cornerColors[4];
    s32 color = state->color;

    color = uiBlendColors(color, color & ~0xFF, state->blend);
    panelSetVec4((u32 *)cornerColors, 0, 0, color, color);

    uiDrawGradientColorRect(0, 0x700, 0, 0x2000, 0x700, (u32)cornerColors, surface);
    if (state->active != 0) {
        state->blend += MNU_GRADIENT_FADE_STEP;
        if (state->blend > MNU_TRANSITION_LIMIT) {
            state->blend = MNU_TRANSITION_LIMIT;
        }
    } else {
        state->blend -= MNU_GRADIENT_FADE_STEP;
        if (state->blend < 0) {
            state->blend = 0;
        }
    }
}

void mnuDrawRepeatedPanelSprites(s32 x, s32 y, s32 depth, s32 fade, s32 count, s32 drawArg, s32 variant, s32 texture) {
    s32 i;

    for (i = 0; i < count; i++) {
        func_00306CD0(x, y, depth, fade, 1, drawArg, variant, texture);
        x += 0xA0;
    }
}

typedef struct MenuEffectPosition {
    u8 pad00[0x20];
    s32 *coordinates;
} MenuEffectPosition;

typedef struct MenuEffectNode {
    u8 pad00[8];
    MenuEffectPosition *position;
} MenuEffectNode;

/* Set both effect positions; only the first Y comes from the active menu entry. */
void mnuSetPairedEffectPositions(MenuPageBar *pair) {
    MenuEffectNode *first = pair->effects[0];
    MenuEffectNode *second = pair->effects[1];
    MenuEffectPosition *firstPosition = first->position;
    MenuEffectPosition *secondPosition = second->position;
    s32 *coordinates = firstPosition->coordinates;

    coordinates[0] = 10;
    coordinates[1] = pair->positionY;
    coordinates[2] = 10;
    coordinates = secondPosition->coordinates;
    coordinates[1] = 5;
    coordinates[0] = 10;
    coordinates[2] = 10;
}

/* Divide a quantized horizontal span into the two effect-grid regions. */
void func_002C1D10(MenuPageBar *pair) {
    s32 start = ((pair->quantizedSpan * 8) / 100) * 16;
    s32 end = ((pair->quantizedSpan * 77) / 100) * 16;
    s32 offset = -0x60;
    s32 bounds[2];

    bounds[0] = 0;
    bounds[1] = end;
    itfGridSetQuantizedBounds(pair->textures[3], 0,
                              start + offset, bounds[0],
                              bounds[1] - start + offset, bounds[0]);
    end += offset;
    itfGridSetQuantizedBounds(pair->textures[4], 0, end, 0, end, 0);
}

/* Cycle through four indexed settings while refreshing the paired effects. */
void mnuCyclePairedEffectSetting(MenuPageBar *pair) {
    s32 *settings;
    s32 setting;

    func_002C1D10(pair);
    mnuSetPairedEffectPositions(pair);
    settings = pair->settings;
    setting = 0;
    if (settings != 0) {
        setting = settings[(s8)pair->settingIndex];
    }
    effConfigureWithDefaultSetting(pair->textures[3], 0, (s32)pair->effects[0], 0, setting, 0);
    pair->settingIndex += 1;
    if ((s8)pair->settingIndex >= 4) {
        pair->settingIndex = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1E48);

void mnuCreatePairedEffects(MenuPageBar *pair) {
    u32 effectHandle;

    effectHandle = effCreateStatusBatch(3);
    pair->effects[0] = (MenuEffectNode *)effectHandle;
    effectHandle = effCreateStatusBatch(3);
    pair->effects[1] = (MenuEffectNode *)effectHandle;
}

/* The public word-pointer boundary refers to the same complete panel owner. */
void mnuReleasePairedEffectBatches(s32 *objectWords) {
    MenuPageBar *pair = (MenuPageBar *)objectWords;
    u32 effectIndex;

    for (effectIndex = 0; effectIndex < 2; effectIndex++) {
        effDestroyPackedBatch((s32)pair->effects[effectIndex]);
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1FF0);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B160);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B180);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B220);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B270);

void mnuDrawPanelSequenceByRow(s32 x, s32 y, s32 depth, s32 color, s32 variant, s32 texture) {
    s32 tableA[7] = {0x29, 0x23, 0x24, 0x25, 0x25, 0x2A, 0x2C};
    s32 tableB[7] = {0x29, 0x26, 0x27, 0x28, 0x28, 0x2B, 0x2C};

    if (y == 1) {
        func_002C1FF0(x, y, depth, color, variant, texture, tableB, 7);
    } else {
        func_002C1FF0(x, y, depth, color, variant, texture, tableA, 7);
    }
}

/* Release the seven sprite texture handles, then the paired effect batches. */
void mnuReleaseSpriteTextures(u32 *objectWords) {
    s32 *textureCursor = ((MenuPageBar *)objectWords)->textures;
    u32 textureIndex = 0;
    do {
        effDestroyResourceSlotSet(*textureCursor++);
        textureIndex++;
    } while (textureIndex < MNU_PANEL_TEXTURE_COUNT);
    mnuReleasePairedEffectBatches((s32 *)objectWords);
}

/* Select the packed ratio color; a zero divisor retains the default color. */
u32 mnuGetPanelRatioColor(s32 useDefault, s32 amount, s32 divisor) {
    u32 color = 0xA09DC380;
    if (!useDefault) {
        switch (mnuClassifyQuarterHalfPercent(amount, divisor)) {
        case 1:
            color = 0xB4A06480;
            break;
        case 2:
            color = 0x89554780;
            break;
        }
    }
    return color;
}

typedef struct FrFontGlyph FrFontGlyph;
extern char D_00437C88[];
extern s32 func_0035C860(char *, const char *, ...);
extern u32 func_0019F5E8(s32, s32, s32, u32, char *, s32);
extern s32 func_0019D550(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C22D0);

extern void func_002C22D0(s32, s32, s32, u32, u32, s32, s32, MenuPageBar *, u32);
extern void func_002C1E48(s32, s32, s32, u32, MenuPageBar *, u32);

void mnuDrawAndAdvanceRatioPanel(s32 x, s32 y, s32 depth, u32 color, s32 value,
                  s32 limit, MenuPageBar *pair, u32 flags) {
    s32 fade = pair->fade;
    s32 barWidth;
    s32 quantizedWidth;
    EffectSlotSet *texture;
    BdWork *work;

    func_002C22D0(x, y, depth, color, fade, value, limit, pair, flags);
    func_00306CD0(x, y, depth, fade, 1, pair->textures[0], 0, flags);
    if (value != 0) {
        texture = (EffectSlotSet *)pair->textures[1];
        work = texture->workEntries;
        barWidth = pair->quantizedSpan * 77 / 100;
        quantizedWidth = barWidth * 16;
        work->width = quantizedWidth;
        work->parameters[2] = ~(77 - barWidth);
        func_00306CD0(x, y, depth, fade, 1, (s32)texture, 0, flags);
        func_00306CD0(x + quantizedWidth, y, depth, fade, 1, pair->textures[2], 0, flags);
        func_002C1E48(x, y, depth, fade, pair, flags);
    }
    if (pair->fadeOut == 0) {
        if (pair->fade < 256) {
            pair->fade += 32;
        }
        if (pair->fade > 256) {
            pair->fade = 256;
        }
    } else {
        if (pair->fade > 0) {
            pair->fade -= 32;
        }
        if (pair->fade < 0) {
            pair->fade = 0;
        }
    }
}

/* DDS2's panel item is wider than the DDS1 variant, with five points at +0x74. */
struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    u32 value24;
    u32 value28;
    MenuGridSlot spriteGridSlots[9]; /* 0x2C */
    MenuGridSlot gridSlots[5]; /* 0x74 */
    u8 pad9C[4];
    u32 initialValue; /* 0xA0 */
    u32 selectionRamp; /* 0xA4 */
    s32 phase;
};

/* Allocate a zeroed native panel item and initialize its three default values. */
MenuPanelItem *mnuCreatePanelItem(void) {
    MenuPanelItem *panelItem = (MenuPanelItem *)sdfAllocSizeClassBlock(MNU_PANEL_ITEM_BYTES);

    memset(panelItem, 0, MNU_PANEL_ITEM_BYTES);
    panelItem->value14 = 0x63;
    panelItem->value10 = 0x8c;
    panelItem->initialValue = 0x100;
    return panelItem;
}

/* Bind the panel item's nine sprite cells to their grid entries (the extra pair only when an extra grid
 * exists) and pick the panel's label entry. */
void mnuInitializePanelGroupGridSlots(MenuPanelItem *item, s32 primaryGrid, s32 secondaryGrid, s32 extraGrid, s32 panelIndex) {
    s32 panelEntryIds[5] = {'F', 'H', 'G', 'I', 'J'};

    itfGridStorePosition(&item->spriteGridSlots[0], secondaryGrid, 4);
    itfGridStorePosition(&item->spriteGridSlots[1], secondaryGrid, 5);
    itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[1].set, item->spriteGridSlots[1].index, 0xD40, 0x40, 0, 0);
    itfGridStorePosition(&item->spriteGridSlots[2], primaryGrid, 0x51);
    itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[2].set, item->spriteGridSlots[2].index, 0x4B0, 0x48, 0, 0);
    itfGridStorePosition(&item->spriteGridSlots[3], primaryGrid, 0x53);
    itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[3].set, item->spriteGridSlots[3].index, 0x4B0, 0x48, 0, 0);
    itfGridStorePosition(&item->spriteGridSlots[4], primaryGrid, 0x52);
    itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[4].set, item->spriteGridSlots[4].index, 0x460, 0x20, 0, 0);
    itfGridStorePosition(&item->spriteGridSlots[5], primaryGrid, 0x54);
    itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[5].set, item->spriteGridSlots[5].index, 0x460, 0x20, 0, 0);
    if (extraGrid != 0) {
        itfGridStorePosition(&item->spriteGridSlots[6], primaryGrid, 0x56);
        itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[6].set, item->spriteGridSlots[6].index, 0x4B0, 0x48, 0, 0);
        itfGridStorePosition(&item->spriteGridSlots[7], extraGrid, 0x19);
        itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[7].set, item->spriteGridSlots[7].index, 0x460, 0x20, 0, 0);
    } else {
        item->spriteGridSlots[6].set = 0;
        item->spriteGridSlots[6].index = 0;
        item->spriteGridSlots[7].set = 0;
        item->spriteGridSlots[7].index = 0;
    }
    itfGridStorePosition(&item->spriteGridSlots[8], primaryGrid, panelEntryIds[panelIndex]);
    itfSetGridEntryQuantizedAndRefresh(item->spriteGridSlots[8].set, item->spriteGridSlots[8].index, 0x130, -0x30, 0, 0);
}

/* Bind five grid object/index references and initialize their quantized bounds.
 * The x/y members in this path hold object addresses and entry indices, not coordinates. */
void mnuInitializePanelItemGridSlots(MenuPanelItem *item, s32 gridObject, s32 panelIndex) {
    s32 entryIndices[5] = {0, 4, 1, 2, 3};

    itfGridStorePosition(&item->gridSlots[0], gridObject, 7);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[0].set, item->gridSlots[0].index, -0x50, -0x50, 0, 0);
    itfGridStorePosition(&item->gridSlots[1], gridObject, 5);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[1].set, item->gridSlots[1].index, 0x390, -8, 0, 0);
    itfGridStorePosition(&item->gridSlots[2], gridObject, 6);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[2].set, item->gridSlots[2].index, 0x390, -8, 0, 0);
    itfGridStorePosition(&item->gridSlots[3], gridObject, 9);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[3].set, item->gridSlots[3].index, 0x5D0, 0, 0, 0);
    itfGridStorePosition(&item->gridSlots[4], gridObject, entryIndices[panelIndex]);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[4].set, item->gridSlots[4].index, 0x130, -0x30, 0, 0);
}

void func_002C2A88(MenuPanelItem *item, u32 value) {
    item->value10 = value;
}

void mnuSetGroupPair(u32 *entry, u32 left, u32 right) {
    entry[6] = left;
    entry[7] = right;
}

void mnuStorePanelItemValue(MenuPanelItem *item, u32 value) {
    item->value24 = value;
}

void func_002C2AA8(MenuPanelItem *item, u32 value) {
    item->value28 = value;
}

void mnuSetPanelItemSelection(MenuPanelItem *item, s32 selection) {
    if (item->selection != selection) {
        item->selectionRamp = 0x100;
    }
    item->selection = selection;
}

void mnuSetPanelItemOption(MenuPanelItem *item, u32 option) {
    item->option = option;
}

void mnuFreePanelItemWork(MenuPanelItem *item) {
    sdfReleaseChipBlock(item);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C2AE8);

extern void func_002C2AE8(s32, s32, s32, u32, s32, MenuPanelItem *, u32);
extern void frFontSetChainFlag(FrFontGlyph *, u8);

void mnuDrawAndAdvancePanelItem(s32 x, s32 y, s32 depth, s32 mode, u32 textMode,
                  MenuPanelItem *item, u32 flags) {
    char text[16];
    s32 fontFlags = 0;
    s32 value;
    u32 fade = item->initialValue;
    u32 color;
    FrFontGlyph *glyph;

    func_00306CD0(x, y, depth, fade, 0, (s32)item->spriteGridSlots[0].set,
                  item->spriteGridSlots[0].index, flags);
    func_00306CD0(x, y, depth, fade, 0, (s32)item->spriteGridSlots[1].set,
                  item->spriteGridSlots[1].index, flags);
    func_002C2AE8(x, y, depth, fade, mode, item, flags);
    func_00306CD0(x, y, depth, fade, 0, (s32)item->spriteGridSlots[8].set,
                  item->spriteGridSlots[8].index, flags);
    if (mode == 1 || (mode == 0 && (item->selection != 0 || item->option != 0))) {
        func_00306CD0(x, y, depth, fade, 0, (s32)item->gridSlots[0].set,
                      item->gridSlots[0].index, flags);
        func_00306CD0(x, y, depth, fade, 0, (s32)item->gridSlots[4].set,
                      item->gridSlots[4].index, flags);
    }
    value = item->value18;
    value += item->option;
    if (mode == 1 || (mode == 0 && (item->selection != 0 || item->option != 0))) {
        fontFlags = 3;
    }
    switch (textMode) {
    case 1:
        fontFlags = 1;
        break;
    case 3:
        fontFlags = 4;
        break;
    case 4:
        fontFlags = 3;
        break;
    }
    value += item->value24;
    if (value > item->value14) {
        value = item->value14;
    }
    color = uiBlendColors(0xA09DC380, 0xA09DC300, fade);
    func_0035C860(text, D_00437C88, value);
    glyph = (FrFontGlyph *)func_0019F5E8(x + 0x2D0, y, depth, color, text, 0);
    frFontSetChainFlag(glyph, fontFlags);
    func_0019D550(glyph, 1, flags);
    frFontQueueGlyphInSelectedSlot(glyph);
    item->phase += 24;
    if (item->phase > 512) {
        item->phase -= 512;
    }
}


void mnuSetProfilePanelValues(MenuProfilePanel *panel, s32 value, s32 option) {
    panel->unk10 = value;
    panel->unk14 = option;
}

/* Create a profile panel and initialize its five native random words. */
u32 *mnuCreateProfilePanel(s32 selectionState) {
    MenuProfilePanel *panel = (MenuProfilePanel *)sdfAllocSizeClassBlock(MNU_PROFILE_PANEL_BYTES);
    s32 profileId;
    u32 profileRecordAddress;
    u32 randomWordIndex;

    memset(panel, 0, MNU_PROFILE_PANEL_BYTES);
    profileId = scrGetSelectedScriptEntryId(selectionState);
    profileRecordAddress = ptyGetCurrentProfileRecord(selectionState);
    mnuSetProfilePanelValues(panel, ptyGetProfileRecordCap((u16)profileId), *(u32 *)profileRecordAddress);
    for (randomWordIndex = 0; randomWordIndex < 5; randomWordIndex++) {
        panel->unk2C[randomWordIndex] = effMiscRand(0) % 0xC0 + 0x40;
    }
    panel->unk44 = 0x100;
    return (u32 *)panel;
}

void mnuFreeProfilePanelWork(void) {
    sdfReleaseChipBlock();
}

void mnuSetGroupProperties(u32 *entry, u32 first, u32 second, u32 third, u32 fourth) {
    entry[6] = first;
    entry[7] = second;
    entry[9] = third;
    entry[10] = fourth;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C33C0);

/* Draw first, then advance the native phase by one with a single period subtraction. */
void mnuDrawAndAdvanceProfilePanel(s32 x, s32 y, s32 z, u32 *panelWords, s32 option) {
    MenuProfilePanel *panel = (MenuProfilePanel *)panelWords;
    s32 phase;
    s32 nextPhase;

    func_002C33C0(x, y, z, panelWords, option);
    phase = panel->phase;
    nextPhase = phase + MNU_PROFILE_PHASE_STEP;
    if (phase < MNU_PROFILE_PHASE_PERIOD) {
        panel->phase = nextPhase;
        if (nextPhase < MNU_PROFILE_PHASE_PERIOD) {
            return;
        }
        phase = nextPhase;
    }
    panel->phase = phase - MNU_PROFILE_PHASE_PERIOD;
}


/* Clear the complete native popup-transition state, including saved entry addresses. */
void mnuClearPanelTransitionState(u32 stateAddress) {
    memset(stateAddress, 0, MNU_POPUP_STATE_BYTES);
}

void func_002C3E78(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument) {
    MenuPopupCallback callback;
    MenuPopupEntry *saved;
    s32 i;

    if (state == NULL) {
        return;
    }
    switch (action) {
        case 0:
            callback = entry->enter;
            if (callback != NULL) {
                if (entry->leave != NULL) {
                    for (i = 0; i < state->count; i++) {
                        if (entry->enter == state->entries[i]->enter) {
                            return;
                        }
                    }
                    state->entries[state->count++] = entry;
                    if ((entry->flags & 0x20000) && state->count >= 2) {
                        saved = state->entries[state->count - 1];
                        state->entries[state->count - 1] = state->entries[state->count - 2];
                        state->entries[state->count - 2] = saved;
                    }
                }
                callback(argument);
            }
            break;
        case 1:
        case 2:
            if (state->count != 0) {
                saved = state->entries[state->count - 1];
                if (saved->leave != NULL && action == 1) {
                    saved->leave(argument);
                }
                state->count--;
            }
            break;
    }
}

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void mnuDrainPanelTransitions(u32 stateAddress, u32 callbackArgument) {
    s32 entryCount;

    entryCount = *(s32 *)stateAddress;
    while (entryCount != 0) {
        func_002C3E78(1, NULL, (MenuPopupState *)stateAddress, callbackArgument);
        entryCount = *(s32 *)stateAddress;
    }
}

s32 mnuHasPopupSelectionFlag(s32 *flags) {
    return (*flags & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4038);


/* Compare the popup state's current entry address, not an entry payload value. */
u8 mnuIsPopupEntryValue(s32 stateAddress, s32 entryAddress) {
    return ((MenuPopupState *)stateAddress)->entryAddress == entryAddress;
}

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void mnuSetPopupEntry(s32 *entrySlot, void *entry) {
    u16 retainedFlags;

    retainedFlags = *(u16 *)entry;
    *entrySlot = (s32)entry;
    *(s32 *)entry = retainedFlags;
}

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void mnuSetPopupEntryFlagged(s32 *entrySlot, void *entry) {
    u16 retainedFlags;

    retainedFlags = *(u16 *)entry;
    *entrySlot = (s32)entry;
    *(s32 *)entry = retainedFlags | MNU_POPUP_INSERT_BEFORE_TOP;
}

/* Bind the entry with both native marking bits after preserving low flags. */
void mnuAttachAndMarkMenuEntry(s32 entrySlotAddress, s32 entryAddress) {
    u16 retainedFlags;

    retainedFlags = *(u16 *)entryAddress;
    *(s32 *)entrySlotAddress = entryAddress;
    *(s32 *)entryAddress = retainedFlags | MNU_POPUP_ENTRY_MARK_BITS;
}

/* Bind the current popup entry only when its saved address is nonzero. */
void mnuBindPresentMenuEntry(s32 stateAddress, u32 entrySlotAddress) {
    if (((MenuPopupState *)stateAddress)->entryAddress != 0) {
        mnuSetPopupEntryFlagged((s32 *)entrySlotAddress, (void *)((MenuPopupState *)stateAddress)->entryAddress);
        return;
    }
}

void func_002C4328(DatPartyRecord *entry, s32 unused, u32 index, PartyPanel *panel) {
    s32 i;

    if (entry->flags & 2)
        panel->unk0++;
    else
        panel->unk4++;
    panel->slots[index].unk8 = entry->unitId - 1;
    panel->slots[index].level = entry->level;
    panel->slots[index].hp = entry->hp;
    panel->slots[index].mp = entry->mp;
    panel->slots[index].maxHp = entry->maxHp;
    panel->slots[index].maxMp = entry->maxMp;
    for (i = 0; i < 5; i++) {
        panel->slots[index].stats[i] = entry->baseStats[i];
    }
}

/* Populate occupied party slots; empty slots retain the native unknown-field sentinel. */
void mnuInitPartyPanelSlots(PartyPanel *panel) {
    u32 partyIndex;
    DatPartyRecord *partyEntry;

    memset(panel, 0, 0x10C);
    panel->unk0 = 0;
    panel->unk4 = 0;
    for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++) {
        partyEntry = &datGameState->party[partyIndex];
        if (partyEntry->flags & 1) {
            func_002C4328(partyEntry, 0, partyIndex, panel);
            panel->slots[partyIndex].index = partyIndex;
        } else {
            panel->slots[partyIndex].unk8 = -1;
        }
    }
}

extern u8 D_0037F530[];

/* Translate requested inputs using native sign-bit/trigger-bit and nonzero tests.
 * Input bit zero has exclusive priority over every other result bit. */
s32 mnuMapPadMaskToFlags(s32 buttonMask) {
    s32 inputFlags = 0;

    if (buttonMask & MNU_INPUT_PRIORITY_BIT) {
        if (D_0037F510[0x21] < 0) {
            inputFlags = MNU_INPUT_PRIORITY_BIT;
        }
    }
    if (buttonMask & 0x2) {
        if (D_0037F510[0x23] < 0) {
            inputFlags |= 0x2;
        }
    }
    if (buttonMask & 0x10) {
        if (D_0037F530[6] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x10;
        }
    }
    if (buttonMask & 0x20) {
        if (D_0037F530[7] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x20;
        }
    }
    if (buttonMask & 0x40) {
        if (D_0037F530[4] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x40;
        }
    }
    if (buttonMask & 0x80) {
        if (D_0037F530[5] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x80;
        }
    }
    if (buttonMask & 0x4) {
        if (D_0037F510[0x22] < 0) {
            inputFlags |= 0x4;
        }
    }
    if (buttonMask & 0x8) {
        if (D_0037F510[0x20] < 0) {
            inputFlags |= 0x8;
        }
    }
    if (buttonMask & 0x200) {
        if (D_0037F530[0xA] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x200;
        }
    }
    if (buttonMask & 0x100) {
        if (D_0037F530[8] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x100;
        }
    }
    if (buttonMask & 0x800) {
        if (D_0037F530[0xB] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x800;
        }
    }
    if (buttonMask & 0x400) {
        if (D_0037F530[9] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x400;
        }
    }
    if (buttonMask & 0x1000) {
        if (D_0037F510[0x2C] < 0) {
            inputFlags |= 0x1000;
        }
    }
    if (buttonMask & 0x2000) {
        if (D_0037F510[0x2D] < 0) {
            inputFlags |= 0x2000;
        }
    }
    if (buttonMask & 0x20000) {
        if (D_0037F510[0x2A] < 0) {
            inputFlags |= 0x20000;
        }
    }
    if (buttonMask & 0x10000) {
        if (D_0037F510[0x28] < 0) {
            inputFlags |= 0x10000;
        }
    }
    if (buttonMask & 0x80000) {
        if (D_0037F510[0x2B] < 0) {
            inputFlags |= 0x80000;
        }
    }
    if (buttonMask & 0x40000) {
        if (D_0037F510[0x29] < 0) {
            inputFlags |= 0x40000;
        }
    }
    if (buttonMask & 0x10) {
        if (D_0037F510[0x26] != 0) {
            inputFlags |= 0x100000;
        }
    }
    if (buttonMask & 0x20) {
        if (D_0037F510[0x27] != 0) {
            inputFlags |= 0x200000;
        }
    }
    if (buttonMask & 0x40) {
        if (D_0037F510[0x24] != 0) {
            inputFlags |= 0x400000;
        }
    }
    if (buttonMask & 0x80) {
        if (D_0037F510[0x25] != 0) {
            inputFlags |= 0x800000;
        }
        if (D_0037F510[0x2A] != 0) {
            inputFlags |= 0x1000000;
        }
    }
    if (buttonMask & 0x40) {
        if (D_0037F510[0x28] != 0) {
            inputFlags |= 0x2000000;
        }
    }
    if (buttonMask & 0x80) {
        if (D_0037F510[0x2B] != 0) {
            inputFlags |= 0x4000000;
        }
    }
    if (buttonMask & 0x40) {
        if (D_0037F510[0x29] != 0) {
            inputFlags |= 0x8000000;
        }
    }
    if (inputFlags & MNU_INPUT_PRIORITY_BIT) {
        inputFlags = inputFlags & MNU_INPUT_PRIORITY_BIT;
    }
    return inputFlags;
}

void mnuHandleListPageJumpInput(s32 active, u8 *menu, u32 *buttons) {
    s32 top = 0;
    s32 bottom = 0;
    u8 *list = *(u8 **)(menu + 0x18);

    if (active != 0) {
        if (*(s32 *)(list + 0x20) >= *(s32 *)(list + 0xC)) {
            if (*buttons & 0x400) {
                top = func_002B8E30(list);
            }
            if (*buttons & 0x800) {
                bottom = mnuScrollListToEnd(*(u8 **)(menu + 0x18));
            }
            if (top == 0) {
                *buttons &= ~0x400;
            }
            if (bottom == 0) {
                *buttons &= ~0x800;
            }
            mnuClearWindowPanelTransitionFlag(menu);
            return;
        }
    }
    *buttons &= ~0x400;
    *buttons &= ~0x800;
}

void mnuHandlePanelListPageJumpInput(u32 item, u32 option) {
    mnuHandleListPageJumpInput(*(u32 *)((s32)item + 0x90), item, option);
}

/* Bits 0x8000 and 0x4000 return early in that order; state bits may suppress navigation SE. */
void mnuPlayInputSound(s32 unused, s32 inputFlags, u32 *stateFlags) {
    if (inputFlags & 0x8000) {
        sndSetSequenceVolumePan(0xD, 0x7F, 0x3F);
        return;
    }
    if (inputFlags & 0x4000) {
        sndSetSequenceVolumePan(0xC, 0x7F, 0x3F);
        return;
    }
    if (inputFlags != 0) {
        if (inputFlags & MNU_INPUT_PRIORITY_BIT) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (inputFlags & 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
        }
        if (inputFlags & 0xCF0) {
            if (stateFlags != NULL) {
                if ((*stateFlags & 3) != 2) {
                    sndSetSequenceVolumePan(0, 0x7F, 0x3F);
                }
            } else {
                sndSetSequenceVolumePan(0, 0x7F, 0x3F);
            }
        }
    }
}

/* Only bit 0x8000 returns early; other flags may play multiple sounds of the selected kind. */
void mnuPlayInputSoundKind(s32 inputFlags, s8 soundKind) {
    if (inputFlags & 0x8000) {
        sndSetSequenceVolumePan(0xD, 0x7F, 0x3F);
        return;
    }
    if (inputFlags != 0) {
        if (inputFlags & 0x4000) {
            sndSetSequenceVolumePan(0xC, 0x7F, 0x3F);
        }
        if (inputFlags & MNU_INPUT_PRIORITY_BIT) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (inputFlags & 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
        }
        if (inputFlags & 0x1000) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (inputFlags & 0x2000) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (inputFlags & 0xF0FF0) {
            switch (soundKind) {
            case 1:
                sndSetSequenceVolumePan(6, 0x7F, 0x3F);
                break;
            case 2:
                sndSetSequenceVolumePan(1, 0x7F, 0x3F);
                break;
            case 3:
                sndSetSequenceVolumePan(0x15, 0x7F, 0x3F);
                break;
            default:
                sndSetSequenceVolumePan(0, 0x7F, 0x3F);
                break;
            }
        }
    }
}

/* Dispatch the input flags with the native default sound kind. */
void mnuPlayDefaultInputSounds(u32 inputFlags) {
    mnuPlayInputSoundKind(inputFlags, 0);
}


/* Find the first occupied slot with the same table ID; zero also serves as no-match. */
s32 mnuFindMatchingPartyEntryIndex(s32 targetEntryAddress) {
    s32 partyIndex;
    DatPartyRecord *partyEntry = datGameState->party;
    for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++, partyEntry++) {
        if ((partyEntry->flags & 1) &&
            ((DatPartyRecord *)targetEntryAddress)->unitId == partyEntry->unitId) {
            return partyIndex;
        }
    }
    return 0;
}

/* Match a half-open range, then return its signed mapping value; unmatched IDs return zero. */
s32 mnuLookupRangeEntry(u16 rangeId) {
    u16 *rangeBounds = D_003E7900;
    s8 *mappingRows = D_003E7928;
    u32 rangeKey = rangeId & MNU_COMMAND_ID_MASK;
    u32 boundWordIndex;
    for (boundWordIndex = 0; boundWordIndex < 0x14; boundWordIndex += 2, rangeBounds += 2) {
        if (rangeKey < rangeBounds[0]) {
            continue;
        }
        if (rangeKey >= rangeBounds[1]) {
            continue;
        }
        {
            u32 mappingIndex = 0;
            s8 *mappingCursor = mappingRows + 1;
            for (; mappingIndex < 6; mappingIndex++, mappingCursor += 3) {
                if (boundWordIndex != mappingCursor[1]) {
                    continue;
                }
                return mappingCursor[0];
            }
        }
    }
    return 0;
}

/* Read a bound word from the primary table or its one-word-shifted alternate view. */
u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate) {
    return (alternate == 0) ? D_003E7900[boundWordIndex] : D_003E7902[boundWordIndex];
}

/* For nonnegative indices, select among nonzero signed mapping values in row order. */
s32 mnuGetIndexedNonzeroEffect(s32 valueIndex) {
    s32 nonzeroCount = 0;
    s32 mappingIndex;
    s8 *mappingCursor = D_003E7928;
    for (mappingIndex = 0; mappingIndex < 6; mappingIndex++, mappingCursor += 3) {
        s32 mappingValue = *mappingCursor;
        if (mappingValue != 0) nonzeroCount++;
        if (valueIndex == nonzeroCount - 1) return mappingValue;
    }
    return 0;
}

/* Accumulate selected mapping values, then read the requested adjacent range bound. */
u16 mnuLookupPartyTableValue(u32 valueCount, s32 baseIndex, s32 alternate) {
    u32 valueIndex;
    s32 mappingSum = 0;

    for (valueIndex = 0; valueIndex < valueCount; valueIndex++) {
        mappingSum += mnuGetIndexedNonzeroEffect(valueIndex);
    }
    if (alternate == 0) {
        return D_003E7900[D_003E792A[(baseIndex + mappingSum) * 3]];
    }
    return D_003E7902[D_003E792A[(baseIndex + mappingSum) * 3]];
}

/* Only secondary-kind-2 entries expose the paired value. */
u16 mnuGetSecondaryValueIfKind2(s32 commandId) {
    DatCommandRecord *command = (DatCommandRecord *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + (s32)datCommandRecords);

    if (command->attribute.parts.kind != 2) {
        return 0;
    }
    return command->attribute.parts.flagMask;
}

/* Return the native value/cost kind from the low-sixteen-bit command ID. */
u8 mnuGetRangeEntryKind(u32 commandId) {
    return ((DatCommandRecord *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + (s32)datCommandRecords))->costMode;
}

/* HP-kind values use max HP as a percentage basis; other kinds retain the stored value. */
u16 mnuGetAdjustedEntryValue(s32 commandId, s32 actorAddress) {
    DatCommandRecord *command = (DatCommandRecord *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + (s32)datCommandRecords);
    u16 entryValue = command->costPercentage;
    u16 flatAddition = command->costBase;
    if (mnuGetRangeEntryKind(commandId & MNU_COMMAND_ID_MASK) == MNU_COST_KIND_HP) {
        entryValue = flatAddition + ((DatPartyRecord *)actorAddress)->maxHp * entryValue / MNU_PERCENT_SCALE;
    }
    return entryValue;
}

s32 mnuGetRangeEntryFlatValue(s32 id) {
    s32 index = id & 0xFFFF;
    DatCommandRecord *record = (DatCommandRecord *)(index * 0x38 + (s32)datCommandRecords);
    s32 scale = record->costPercentage;
    s32 addition = record->costBase;

    if (mnuGetRangeEntryKind(index) == 1) {
        return scale + addition;
    }
    return scale;
}

/* Compare the stored raw HP/MP cost; equality is affordable and other kinds pass. */
s32 mnuCanAffordEntryCost(u16 commandId, s32 actorAddress) {
    u16 cost = datCommandRecords[commandId].costPercentage;
    s32 costKind = mnuGetRangeEntryKind(commandId);

    switch (costKind) {
    case MNU_COST_KIND_HP:
        if (((DatPartyRecord *)actorAddress)->hp < cost) {
            return 0;
        }
        break;
    case MNU_COST_KIND_MP:
        if (((DatPartyRecord *)actorAddress)->mp < cost) {
            return 0;
        }
        break;
    }
    return 1;
}

extern s32 mnuCanAffordEntryCost(u16, s32);

/* Return -1 for insufficient raw cost, else 0 for flagged IDs below the boundary, or 1. */
s32 mnuGetEntryUseStatus(s32 actorAddress, u16 commandId) {
    if (mnuCanAffordEntryCost(commandId, actorAddress) == 0) return -1;
    if ((((DatCommandRecord *)((s32)datCommandRecords + commandId * MNU_COMMAND_RECORD_BYTES))->unk_01 & 1) == 0) return 1;
    if (commandId < MNU_COMMAND_USE_STATUS_BOUNDARY) return 0;
    return 1;
}

/* Report insufficient raw HP/MP cost; equality and unhandled kinds return zero. */
s32 mnuIsEntryCostUnaffordable(u16 commandId, s32 actorAddress) {
    s32 costKind = datCommandRecords[commandId].costMode;
    u16 cost = datCommandRecords[commandId].costPercentage;

    switch (costKind) {
    case MNU_COST_KIND_HP:
        if (((DatPartyRecord *)actorAddress)->hp < cost) {
            return 1;
        }
        break;
    case MNU_COST_KIND_MP:
        if (((DatPartyRecord *)actorAddress)->mp < cost) {
            return 1;
        }
        break;
    }
    return 0;
}

/* Deduct an affordable stored HP/MP cost; unhandled kinds succeed without a deduction. */
s32 mnuConsumeEntryCost(s32 commandId, u8 *actorEntry) {
    DatCommandRecord *command = (DatCommandRecord *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + (s32)datCommandRecords);
    u16 cost = command->costPercentage;

    switch (command->costMode) {
    case MNU_COST_KIND_HP:
        if (((DatPartyRecord *)actorEntry)->hp < cost) {
            return 0;
        }
        datAdjustCurrentHp(actorEntry, -cost);
        return 1;
    case MNU_COST_KIND_MP:
        if (((DatPartyRecord *)actorEntry)->mp < cost) {
            return 0;
        }
        datAdjustCurrentMp(actorEntry, -cost);
        return 1;
    default:
        return 1;
    }
}

/* Map the native category byte 0/1/2 to 1/2/3; command zero bypasses the record read. */
s32 mnuGetAbilityByteCategory(u16 commandId) {
    u8 category;
    if (commandId == 0) {
        return 1;
    }
    category = datCommandRecords[commandId].unk_08;
    switch (category) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

void func_002C5128(u16 ability, s32 target, DatPartyRecord *entry) {
    sdfApplyCommandResults(ability);
}

u32 func_002C5140(s32 context, s32 ability, s32 target, DatPartyRecord *entry) {
    return 0;
}

s32 ptySkillApplyFieldUseEffect(s32 context, u16 ability, s32 target, s32 selectedEntry) {
    DatPartyRecord *entry = (DatPartyRecord *)selectedEntry;
    s32 multiTarget = 0;
    s32 applied = 0;
    s32 mask;

    if (func_002C5140(context, ability, target, entry) != 0) {
        return 1;
    }

    if (mnuGetAbilityByteCategory(ability) == 1) {
        mask = mnuGetMatchingPartyEntryMask((s32)entry);

        if (func_0022C600(ability, mask) != 0) {
            return 0;
        }
        func_002C5128(ability, target, entry);
        mnuQueueListEntry((MenuPageWindow *)context,
                          mnuFindMatchingPartyEntryIndex((s32)entry), 0, 0);
    } else {
        s32 i;

        for (i = 0; i < MNU_PARTY_SLOT_COUNT; i++) {
            entry = &datGameState->party[i];
            if ((entry->flags & 1) != 0 && (entry->flags & 2) != 0) {
                mask = mnuGetMatchingPartyEntryMask((s32)entry);

                if (func_0022C600(ability, mask) == 0) {
                    func_002C5128(ability, target, entry);
                    applied = 1;
                }
            }
        }

        if (applied == 0) {
            return 0;
        }

        for (i = 0; i < MNU_PARTY_SLOT_COUNT; i++) {
            entry = &datGameState->party[i];
            if ((entry->flags & 1) != 0 && (entry->flags & 2) != 0) {
                mnuQueueListEntry((MenuPageWindow *)context,
                                  mnuFindMatchingPartyEntryIndex((s32)entry), 0, i * 3);
            }
        }
        multiTarget = 1;
    }

    if (mnuGetSecondaryValueIfKind2(ability) & 0x4000) {
        sndSetSequenceVolumePan(0x17, 0x7F, 0x3F);
    } else if (multiTarget == 0) {
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
    } else {
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
    }
    return 1;
}

/* Test for the exact signed-byte marker one, not merely a nonzero selector byte. */
u8 mnuIsAbilityValueMarked(u32 commandId) {
    return *(s8 *)((commandId & MNU_COMMAND_ID_MASK) * 2 + datCommandSelectors) == '\x01';
}

s32 ptyGetAffinityKind(s32 affinityId, s32 index) {
    s32 flags = datAffinityRecords[(affinityId - DAT_AFFINITY_FIRST_COMMAND) & 0xFFFF].requirements[index];

    if (flags == -1) {
        return -1;
    }
    if (flags == 0x20000000) {
        return 0;
    }
    if (flags == 0x20000002) {
        return 1;
    }
    if (flags == 0x20000004) {
        return 2;
    }
    if (flags == 0x20000008) {
        return 3;
    }
    if (flags == 0x20000010) {
        return 4;
    }
    if (flags == 0x20000020) {
        return 5;
    }
    if (flags == 0x20000040) {
        return 6;
    }
    if (flags == 0x20000080) {
        return 7;
    }
    if (flags == 0x20000100) {
        return 8;
    }
    if (flags & 0x40000000) {
        return -2;
    }
    return -1;
}

s32 ptyGetAffinityFlagsWithoutOverride(s32 affinityId, s32 index) {
    s32 value = datAffinityRecords[(affinityId - DAT_AFFINITY_FIRST_COMMAND) & 0xFFFF].requirements[index];

    if (value == -1) {
        return 0;
    }
    if (value & 0x40000000) {
        return value & ~0x40000000;
    }
    return value;
}

s32 mnuIsBulletItemId(s32 id) {
    if (id < 0xA0) {
        return 0;
    }
    return id < 0xC0;
}

s32 func_002C5498(s32 id) {
    if (id < 0x60) {
        return 0;
    }
    return id < 0x6D;
}

s32 func_002C54B0(s32 id) {
    if (id < 0xC0) {
        return 0;
    }
    return id < 0x100;
}


extern u16 mnuGetPartyEntryMenuValue(s32);

/* Count inventory plus one for every matching slot value, without an occupancy test.
 * This counter still rejects 0xBF, although DDS2's bullet-ID predicate accepts it. */
u32 ptyCountBulletItem(s32 bulletId) {
    u32 totalCount;
    s32 partyIndex;
    if (bulletId < 0xA0) return 0;
    if (bulletId >= 0xBF) return 0;
    totalCount = datGameState->inventory.counts[bulletId];
    for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++) {
        DatPartyRecord *partyEntry = &datGameState->party[partyIndex];
        if (bulletId == mnuGetPartyEntryMenuValue((s32)partyEntry)) {
            totalCount++;
        }
    }
    return totalCount;
}

u32 mnuSetPartyEntryMenuValue(s32 entry, u16 value) {
    ((DatPartyRecord *)entry)->menuValue = value;
    return 1;
}

u16 mnuGetPartyEntryMenuValue(s32 entry) {
    return ((DatPartyRecord *)entry)->menuValue;
}

u32 mnuSetPartyEntryCurrentId(u32 entry, u32 id) {
    ((DatPartyRecord *)entry)->itemId = id;
    mnuMarkEntryBlocked(id);
    ptyRecomputeMaxHpMp(entry);
    return 1;
}

u16 mnuGetPartyEntryCurrentId(s32 entry) {
    return ((DatPartyRecord *)entry)->itemId;
}

DatPartyRecord *mnuFindPartySlotByCurrentId(u32 id) {
    s32 index;
    DatPartyRecord *entry = datGameState->party;
    for (index = 0; index < 5; index++, entry++) {
        if ((entry->flags & 1) && id == entry->itemId) {
            return entry;
        }
    }
    return 0;
}

DatPartyRecord *mnuFindReserveSlotByCurrentId(u32 id) {
    s32 index;
    DatPartyRecord *entry = datGameState->templates;
    for (index = 0; index < 16; index++, entry++) {
        if (entry->level != 0 && (entry->flags & 1) && id == entry->itemId) {
            return entry;
        }
    }
    return 0;
}

void mnuMarkEntryBlocked(s32 index) {
    if (index != 0) {
        datGameState->itemBlockedFlags[index - 0xC0] |= 1;
    }
}

void mnuClearEntryBlocked(s32 index) {
    if (index != 0) {
        datGameState->itemBlockedFlags[index - 0xC0] &= ~1;
    }
}

s32 mnuIsEntryBlocked(s32 index) {
    if (index == 0) {
        return 1;
    }
    return datGameState->itemBlockedFlags[index - 0xC0] & 1;
}

s32 mnuHasOwnedUnblockedItem(void) {
    s32 index;
    for (index = 0xC0; index < 0x100; index++) {
        if (!mnuIsEntryBlocked(index) && datGameState->inventory.counts[index] != 0) {
            return 1;
        }
    }
    return 0;
}

/* Match an item ID, select its mode-dependent SE ID, and start it; id changes roles. */
s32 sndPlayPartyItemSe(u32 id, s32 soundMode) {
    u16 *soundRow = D_003E78D8;
    u32 rowIndex;

    for (rowIndex = 0; rowIndex < 5; rowIndex++, soundRow += 4) {
        if (id == soundRow[0]) {
            switch (soundMode) {
            case 0:
                id = soundRow[1];
                id += 0x10;
                break;
            case 1:
                id = soundRow[2];
                id += 0x17;
                break;
            default:
                id = soundRow[3];
                id += 0x17;
                break;
            }
            dspStartEntry(id);
            return 1;
        }
    }
    return 0;
}


extern s32 datComputeSkillBoostedMaxHp(DatPartyRecord *);
extern s32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

/* Apply a permanent stat/capacity item and refill eligible vitals.
 * Returns 0 for other items, 1 when accepted, or 2 when capped and already full. */
INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B300);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B350);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B370);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B3D0);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B440);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B4C0);

s32 btlItemApplyPermanentBonus(u16 itemId, DatPartyRecord *unit) {
    s32 statIndex = -1;
    s32 accepted = 0;

    switch (itemId - 0x59) {
    case 0:
        statIndex = 0;
        accepted = 1;
        break;
    case 1:
        statIndex = 1;
        accepted = 1;
        break;
    case 2:
        statIndex = 2;
        accepted = 1;
        break;
    case 3:
        statIndex = 3;
        accepted = 1;
        break;
    case 4:
        statIndex = 4;
        accepted = 1;
        break;
    case 5:
        if (unit->maxHp >= 0x3E7 && unit->hp >= unit->maxHp &&
            unit->mp >= unit->maxMp) {
            return 2;
        }
        unit->hpBonus += 10;
        if (unit->hpBonus >= 0x3E8) {
            unit->hpBonus = 0x3E7;
        }
        accepted = 1;
        break;
    case 6:
        if (unit->maxMp >= 0x3E7 && unit->hp >= unit->maxHp &&
            unit->mp >= unit->maxMp) {
            return 2;
        }
        unit->mpBonus += 10;
        if (unit->mpBonus >= 0x3E8) {
            unit->mpBonus = 0x3E7;
        }
        accepted = 1;
        break;
    default:
        break;
    }

    if (accepted == 0) {
        return 0;
    }
    if (statIndex >= 0) {
        if (unit->baseStats[statIndex] >= 0x63 &&
            unit->hp >= unit->maxHp && unit->mp >= unit->maxMp) {
            return 2;
        }
        unit->baseStats[statIndex] += 2;
        if (unit->baseStats[statIndex] >= 0x64) {
            unit->baseStats[statIndex] = 0x63;
        }
    }

    unit->maxHp = datComputeSkillBoostedMaxHp(unit);
    unit->maxMp = datComputeSkillBoostedMaxMp(unit);
    /* This status bit suppresses both native refill stores. */
    if ((unit->status & 0x4000) == 0) {
        unit->mp = unit->maxMp;
        unit->hp = unit->maxHp;
    }
    return 1;
}

s32 ptyChooseFirstAvailableRosterId(void) {
    if (ptyIsRosterEntryPresent(1)) return 1;
    if (ptyIsRosterEntryPresent(2)) return 2;
    if (ptyIsRosterEntryPresent(5)) return 5;
    return ptyIsRosterEntryPresent(8) ? 8 : 1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5A28);

/* Return the first matching status index in native priority order, or -1. */
s32 mnuGetSelectionFromFlags(DatPartyRecord *actorEntry) {
    u16 statusFlags = actorEntry->status;
    if (statusFlags & 0x400) return 0;
    if (statusFlags & 0x100) return 1;
    if (statusFlags & 0x80) return 2;
    if (statusFlags & 0x40) return 3;
    if (statusFlags & 0x10) return 4;
    return -1;
}

/* Return one bit for the first occupied matching table ID, or zero when absent. */
s32 mnuGetMatchingPartyEntryMask(s32 targetEntryAddress) {
    s32 partyIndex;
    DatPartyRecord *partyEntry = datGameState->party;
    for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++, partyEntry++) {
        if ((partyEntry->flags & 1) &&
            partyEntry->unitId == ((DatPartyRecord *)targetEntryAddress)->unitId) {
            return 1 << partyIndex;
        }
    }
    return 0;
}

/* Classify the truncated signed percentage: below 25 -> 2, below 50 -> 1, else 0.
 * A zero divisor returns zero without division. */
s32 mnuClassifyQuarterHalfPercent(s32 amount, s32 divisor) {
    s32 ratioPercent;

    if (divisor != 0) {
        ratioPercent = amount * MNU_PERCENT_SCALE / divisor;
        if (ratioPercent < MNU_RATIO_QUARTER_PERCENT) {
            return 2;
        }
        if (ratioPercent < MNU_RATIO_HALF_PERCENT) {
            return 1;
        }
    }
    return 0;
}

DatPartyRecord *mnuPickBestPartyEntry(u32 *table) {
    DatPartyRecord *best = NULL;
    DatPartyRecord *entry;
    s32 pass;
    s32 bestMp;
    s32 i;

    for (pass = 0; pass < 2; pass++) {
        bestMp = 0;
        entry = datGameState->party;
        for (i = 0; i < 5; i++, entry++) {
            if ((entry->flags & 1) && table[entry->unitId] == 0) {
                s32 usable = 0;

                if (!(entry->status & 0x10)) {
                    if (pass == 0) {
                        if (!(entry->flags & 2)) {
                            usable = 1;
                        }
                    } else if (entry->flags & 2) {
                        usable = 1;
                    }
                }

                if (usable && bestMp < entry->mp) {
                    bestMp = entry->mp;
                    best = entry;
                }
            }
        }
        if (best != NULL) {
            return best;
        }
    }
    return 0;
}

DatPartyRecord *mnuFindPartyEntryBySelection(s32 *out) {
    u16 table[5][2] = {{0x400, 0xD4}, {0x100, 0xCF}, {0x80, 0xA9}, {0x40, 0xCE}, {0x10, 0xA8}};
    u32 i;

    for (i = 0; i < 5; i++) {
        DatPartyRecord *entry;
        s32 j;

        for (j = 0, entry = datGameState->party; j < 5; j++, entry++) {
            if ((entry->flags & 1) && entry->status == table[i][0]) {
                *out = table[i][1];
                return entry;
            }
        }
    }
    return 0;
}

s32 mnuTryUseFieldSkill(s32 partyPanel, s32 skill, s32 target, s32 commit) {
    s32 id;
    DatPartyRecord *entry = mnuFindPartyEntryBySelection(&id);

    if (entry == 0) {
        return 3;
    }
    if (ptyGetSkillNibbleState(target, id) != 0) {
        if (mnuIsEntryCostUnaffordable(id, target) != 0) {
            return 0;
        }
        if (commit != 0) {
            ptySkillApplyFieldUseEffect(skill, id & 0xFFFF, target, (s32)entry);
            mnuConsumeEntryCost(id & 0xFFFF, (u8 *)target);
            mnuInitPartyPanelSlots((PartyPanel *)partyPanel);
            func_002BCA98(skill);
            func_002BCAB0(skill);
        }
        return 2;
    }
    return 1;
}

/* Give flag-bit-1 entries precedence, then compare their 10-bit fixed-point
 * HP/max-HP ratios without converting to floating point. */
s32 mnuComparePartyEntryCostRatio(u32 *left, u32 *right) {
    DatPartyRecord *a = (DatPartyRecord *)*left;
    DatPartyRecord *b = (DatPartyRecord *)*right;
    s32 leftRatio = (a->hp << 10) / a->maxHp;
    s32 rightRatio = (b->hp << 10) / b->maxHp;
    if (a->flags & 2) {
        if (!(b->flags & 2)) {
            return -1;
        }
    } else if (b->flags & 2) {
        return 1;
    }
    if (rightRatio < leftRatio) return 1;
    if (leftRatio < rightRatio) return -1;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6008);

s32 mnuUseFieldSkillOnParty(s32 partyPanel, s32 skill, s32 commit) {
    u32 used[32];
    DatPartyRecord *entry;
    s32 pass;
    s32 more;
    s32 result;

    for (pass = 0; pass < 2; pass++) {
        memset(used, 0, sizeof(used));
        more = 1;
        do {
            entry = mnuPickBestPartyEntry(used);
            if (entry == 0) {
                break;
            }
            if (pass == 0) {
                result = mnuTryUseFieldSkill(partyPanel, skill, (s32)entry, commit);
            } else {
                result = func_002C6008(partyPanel, skill, entry, commit);
            }
            switch (result) {
            case 0:
                used[entry->unitId] = 1;
                break;
            case 1:
                used[entry->unitId] = result;
                break;
            case 2:
                return pass + 1;
            default:
                more = 0;
                break;
            }
        } while (more != 0);
    }
    return 0;
}

u8 func_002C6480(void) {
    return D_00437C9C != 0;
}

void evtStageTestSetModelScalingEnabled(s32 enabled) {
    if (enabled == 0) {
        evtStageTestState.flag = 0;
    } else {
        evtStageTestState.flag = 1;
    }
}

u32 evtStageTestGetActiveModel(void) {
    return evtStageTestState.model;
}

u32 evtStageTestGetSlotModelId(void) {
    return evtStageTestState.queue.slot[0].modelId;
}

/* Apply the native motion-index bounds and refresh the entry pose.
 * An empty loaded motion table can still select -1 after the upper-bound check. */
void evtStageTestSetEntryIndex(s32 encodedIndex, s32 motionIndex) {
    s32 entryIndex = encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK;

    if (motionIndex < 0) {
        motionIndex = 0;
    }
    if (evtStageTestState.model != 0 && motionIndex >= mdlGetNodeRefHalf((MdlCtx *)evtStageTestState.model, 0)) {
        motionIndex = mdlGetNodeRefHalf((MdlCtx *)evtStageTestState.model, 0) - 1;
    }
    evtStageTestState.entries[entryIndex].motionIndex = motionIndex;
    func_002C6E20(-1);
}

/* Advance the indexed entry's animation frame and request a stage refresh.
 * The native negative-delta guard tests frame - delta, not frame + delta. */
void evtStageTestAddEntryValue(s32 encodedIndex, f32 delta) {
    s32 entryIndex = encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK;
    StageTestEntry *stageEntry;

    if (delta < 0.0f && ((StageTestEntry *)(entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries))->frame - delta < 0.0f) {
        return;
    }
    stageEntry = (StageTestEntry *)(entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries);
    stageEntry->frame += delta;
    func_002C6E20(-1);
}

/* Add integer XYZ deltas to the indexed entry's floating-point position. */
void mnuOffsetPanelPosition(s32 encodedIndex, s32 dx, s32 dy, s32 dz) {
    s32 entryOffset = (encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK) * sizeof(StageTestEntry);
    StageTestEntry *stageEntry = (StageTestEntry *)(entryOffset + (s32)evtStageTestState.entries);
    f32 x = stageEntry->position[0] + (f32)dx;
    f32 y = stageEntry->position[1] + (f32)dy;
    f32 z = stageEntry->position[2] + (f32)dz;

    stageEntry->position[0] = x;
    stageEntry->position[1] = y;
    stageEntry->position[2] = z;
}

/* Add integer XYZ deltas to the indexed entry's stored rotation angles. */
void mnuOffsetPanelTarget(s32 encodedIndex, s32 dx, s32 dy, s32 dz) {
    s32 entryOffset = (encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK) * sizeof(StageTestEntry);
    StageTestEntry *stageEntry = (StageTestEntry *)(entryOffset + (s32)evtStageTestState.entries);
    f32 x = stageEntry->rotation[0] + (f32)dx;
    f32 y = stageEntry->rotation[1] + (f32)dy;
    f32 z = stageEntry->rotation[2] + (f32)dz;

    stageEntry->rotation[0] = x;
    stageEntry->rotation[1] = y;
    stageEntry->rotation[2] = z;
}

/* Return the motion byte; this twin receives an already-narrowed entry index. */
u8 func_002C66D0(u16 entryIndex) {
    return evtStageTestState.entries[entryIndex].motionIndex;
}

/* Return the indexed entry's animation frame. */
f32 func_002C66F8(u16 entryIndex) {
    return evtStageTestState.entries[entryIndex].frame;
}

/* Copy exactly three position components; any output W component is untouched. */
void func_002C6720(s32 encodedIndex, f32 *position) {
    position[0] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].position[0];
    position[1] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].position[1];
    position[2] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].position[2];
}

/* Copy exactly three stored rotation components. */
void func_002C6758(s32 encodedIndex, f32 *rotation) {
    rotation[0] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].rotation[0];
    rotation[1] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].rotation[1];
    rotation[2] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].rotation[2];
}

extern u32 kwlnGetDrawBufferIndex(void);
extern void sdfConsBuildMatrixPacket(void *packet, void *node, void *matrix);
extern void sdfConsCacheTransformedNode(void *node, void *matrix);
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern u8 D_003E7960[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewEyeVector[];
extern u8 sdfViewUpVector[];
extern u8 D_0037F590[];
extern u8 sdfViewMatrix[];
extern u8 D_003820F0[];
extern void *memcpy(void *dest, const void *src, u32 size);
extern u8 D_0042B528[];
extern u8 D_0042B538[];
extern u8 D_0042B548[];
extern u8 D_0042B558[];
extern void evtToggleSavedDrawVectors(s32 frames, f32 first, f32 second);
extern void kwlnSetLightColorTarget(s32 mode, s32 slot, f32 *color);
extern void kwlnSetLightDirectionTarget(s32 mode, s32 slot, f32 *color);
extern void kwlnSetBackgroundColorTarget(s32 mode, f32 *color);
extern void kwlnSetDrawColorTarget(s32 mode, f32 *color);
extern void evtSetDrawVectorTarget(s32 mode, f32 x, f32 y, f32 z, f32 w);

/* Restore color targets, projection settings and the stage viewer's view vectors. */
void evtStageTestResetViewAndLighting(void)
{
    f32 firstColor[4];
    f32 secondColor[4];
    f32 backgroundColor[4];
    f32 drawColor[4];

    memcpy(firstColor, D_0042B528, sizeof(firstColor));
    memcpy(secondColor, D_0042B538, sizeof(secondColor));
    memcpy(backgroundColor, D_0042B548, sizeof(backgroundColor));
    memcpy(drawColor, D_0042B558, sizeof(drawColor));
    evtToggleSavedDrawVectors(0, 5.0f, 0.0f);
    kwlnSetLightColorTarget(0, 0, firstColor);
    kwlnSetLightDirectionTarget(0, 0, secondColor);
    kwlnSetBackgroundColorTarget(0, backgroundColor);
    kwlnSetDrawColorTarget(0, drawColor);
    evtSetDrawVectorTarget(0, 255.0f, 255.0f, 2000.0f, 30000.0f);
    sdfSceneProjectionParameters.fov = 0.7551905f;
    PCP_COPY_VECTOR(sdfViewTargetVector, D_003E7950);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_003E7940);
    PCP_COPY_VECTOR(sdfViewUpVector, D_003E7960);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, sdfViewMatrix);
}

/* Set the horizontal projection offset while retaining the native GS-coordinate bias. */
void mnuSetStageTestCameraOffset(f32 offset) {
    sdfSceneProjectionParameters.offsetY = EVT_STAGE_PROJECTION_BIAS;
    sdfSceneProjectionParameters.offsetX = offset + EVT_STAGE_PROJECTION_BIAS;
    evtStageTestResetViewAndLighting();
}

void evtStageTestInit(s32 mode) {
    f32 offset;
    s32 i;
    s32 value;

    evtStageTestState.mode = mode;
    evtStageTestState.assetRequest = 0;
    evtStageTestState.model = 0;
    evtStageTestState.flag = 0;
    evtStageTestState.modelUpdateStarted = 0;
    if (mode == 0) {
        evtStageTestState.entries = (StageTestEntry *)D_003E7978;
        offset = -140.0f;
    } else {
        evtStageTestState.entries = NULL;
        offset = 140.0f;
    }
    value = D_003E7970[mode];
    evtStageTestState.queue.flags = 0;
    for (i = 0; i < 2; i++) {
        evtStageTestState.queue.slot[i].assetResource = value;
        evtStageTestState.queue.slot[i].modelId = -1;
        evtStageTestState.queue.slot[i].assetOption = 0;
        evtStageTestState.queue.slot[i].flags = 0;
    }
    mnuSetStageTestCameraOffset(offset);
}

/* Release the viewer's model/effect and discard its pending effect, except in mode 1. */
void evtStageTestStop(void) {
    StageTestState *stage = &evtStageTestState;

    if (stage->mode == 1) {
        return;
    }
    stage->pendingEffect = -1;
    if (stage->effect != 0) {
        evtStageTestDestroyModelEffect();
    }
    if (stage->model != 0) {
        mdlDestroyContext(stage->model);
        stage->model = 0;
        stage->modelUpdateStarted = 0;
    }
}

/* Stop the viewer and reset both projection offsets to the native coordinate bias. */
void mnuResetWorkFloats(void) {
    evtStageTestStop();
    sdfSceneProjectionParameters.offsetX = EVT_STAGE_PROJECTION_BIAS;
    sdfSceneProjectionParameters.offsetY = EVT_STAGE_PROJECTION_BIAS;
}

/* Remember the model asset request result for the stage viewer. */
s32 evtStageTestRequestModelAsset(s32 resource, s32 modelId, s32 option) {
    evtStageTestState.assetRequest = mdlRequestAsset(resource, modelId, option);
    return evtStageTestState.assetRequest;
}

/* Request an indexed entry's model using the active slot's resource and option zero. */
void mnuForwardTableByte(s32 encodedIndex) {
    StageTestSlot *activeSlot = evtStageTestState.queue.slot;

    evtStageTestRequestModelAsset(activeSlot->assetResource, evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].modelId, 0);
}

/* Test the stage queue's pending-selection bit; the poller passes its flags word. */
u32 mnuHasPendingBlockFlag(u32 *queueFlags) {
    return *queueFlags & EVT_STAGE_QUEUE_PENDING;
}

/* Promote the staged selection and clear its pending flag. Returns 1 only
 * when a slot was committed; the poller supplies &evtStageTestState.queue. */
s32 mnuCommitPendingBlock(StageTestQueue *queue) {
    if (!(queue->flags & EVT_STAGE_QUEUE_PENDING)) {
        return 0;
    }
    queue->slot[0] = queue->slot[1];
    queue->flags &= ~EVT_STAGE_QUEUE_PENDING;
    return 1;
}

/* Discard the pending-selection marker without copying either selection slot. */
void evtStageTestClearPendingFlag(void) {
    evtStageTestState.queue.flags &= ~EVT_STAGE_QUEUE_PENDING;
}

/* Return zero for an already-loaded matching model ID; otherwise schedule a selection.
 * An in-progress request uses the pending slot rather than replacing the active slot. */
s32 evtStageTestSelectEntry(s32 encodedIndex, s32 initialMotionIndex, s32 assetOption) {
    s32 entryIndex = encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK;
    s32 slotIndex;
    StageTestSlot *selectionSlot;

    if (evtStageTestState.model != 0 && evtStageTestState.queue.slot[0].modelId == evtStageTestState.entries[entryIndex].modelId) {
        return 0;
    }
    evtStageTestStop();
    slotIndex = 0;
    if (evtStageTestState.queue.flags & EVT_STAGE_QUEUE_REQUESTING) {
        slotIndex = 1;
    }
    evtStageTestState.queue.flags |= EVT_STAGE_QUEUE_REQUESTING;
    evtStageTestState.queue.flags &= ~EVT_STAGE_QUEUE_SETUP_COMPLETE;
    if (slotIndex) {
        evtStageTestState.queue.flags |= EVT_STAGE_QUEUE_PENDING;
    }
    selectionSlot = &evtStageTestState.queue.slot[slotIndex];
    selectionSlot->entryIndex = entryIndex;
    selectionSlot->modelId = evtStageTestState.entries[entryIndex].modelId;
    selectionSlot->assetOption = assetOption;
    selectionSlot->initialMotionIndex = initialMotionIndex;
    selectionSlot->state = EVT_STAGE_MOTION_IDLE;
    return 1;
}

/* Select an entry using its own stored motion byte; retain the native all-bits-set literal type. */
void evtStageTestSelectEntryWithoutInitialValue(u16 entryIndex, u32 assetOption) {
    evtStageTestSelectEntry(entryIndex, EVT_STAGE_USE_ENTRY_MOTION, assetOption);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6CE8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6E20);


extern s32 D_00435DF0;
extern void mdlStoreTertiaryVectorVU(s32);

/* Store uniform model scale through vf10 and return it; useTable selects the model-record factor. */
f32 mnuSetModelScaleVector(s32 model, s32 useTable) {
    f32 scale = 1.0f;
    f32 scaleVector[4];

    if (useTable != 0) {
        scale = *(f32 *)(D_00435DF0 + evtStageTestState.queue.slot[0].modelId * EVT_STAGE_MODEL_RECORD_BYTES + 0x10);
    }
    scaleVector[0] = scale;
    scaleVector[1] = scale;
    scaleVector[2] = scale;
    scaleVector[3] = 1.0f;
    VU0_LOAD_VF(vf10, scaleVector);
    mdlStoreTertiaryVectorVU(model);
    return scale;
}

typedef struct MenuWorkCamera {
    s32 x;
    s32 y;
    f32 depth; /* third component uses float in D_003E7950 */
} MenuWorkCamera;

typedef struct MenuWorkPosition {
    s32 x;
    s32 y;
    s32 z;
} MenuWorkPosition;

/* Reset target XYZ to (0, 0, -400) and eye XYZ to zero; leave their fourth words untouched. */
void mnuResetWorkPair(void) {
    ((MenuWorkCamera *)D_003E7950)->x = 0;
    ((MenuWorkCamera *)D_003E7950)->y = 0;
    ((MenuWorkCamera *)D_003E7950)->depth = EVT_STAGE_DEFAULT_TARGET_Z;
    ((MenuWorkPosition *)D_003E7940)->x = 0;
    ((MenuWorkPosition *)D_003E7940)->y = 0;
    ((MenuWorkPosition *)D_003E7940)->z = 0;
}

extern void mdlStorePrimaryVectorVU(s32);

/* Apply the active entry's model position and scale-dependent view depth.
 * Retain the post-call entry rereads and subtraction-based scaling expressions. */
void mnuApplyModelCamera(s32 model) {
    f32 position[4];
    StageTestEntry *stageEntry;
    f32 scale;

    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    stageEntry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries);
    position[0] = stageEntry->position[0];
    position[1] = stageEntry->position[1];
    if (evtStageTestState.flag != 1) {
        mnuSetModelScaleVector(model, 0);
        position[2] = ((StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries))->position[2];
        mnuResetWorkPair();
    } else {
        scale = mnuSetModelScaleVector(model, 1);
        stageEntry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries);
        position[0] -= stageEntry->position[0] - stageEntry->position[0] * scale;
        position[1] -= stageEntry->position[1] - stageEntry->position[1] * scale;
        position[2] = 0.0f;
        ((MenuWorkCamera *)D_003E7950)->depth = (EVT_STAGE_DEFAULT_TARGET_Z - stageEntry->position[2]) * scale;
    }
    VU0_LOAD_VF(vf10, position);
    mdlStorePrimaryVectorVU(model);
}

/* Convert the active entry's degree angles to radians and update the model rotation basis. */
void evtStageTestApplyEntryRotation(s32 model) {
    StageTestEntry *stageEntry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries);

    func_00340DC8(stageEntry->rotation[0] * 3.14159265f / 180.0f, stageEntry->rotation[1] * 3.14159265f / 180.0f,
                  stageEntry->rotation[2] * 3.14159265f / 180.0f);
    mdlUpdateContextRotationBasisFromQuaternion(model);
}


/* vu0 routine: copies the stage-test camera vectors into the view work area, builds the look-at basis for eye 600 units along the view direction, and hands the matrix to the model packet at the current slot */
void evtStageTestUpdateCamera(void)
{
    s128 eye;
    s128 target;
    s32 drawBufferIndex;

    drawBufferIndex = kwlnGetDrawBufferIndex();
    evtStageTestResetViewAndLighting();
    PCP_COPY_VECTOR(sdfViewTargetVector, D_003E7950);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_003E7940);
    PCP_COPY_VECTOR(sdfViewUpVector, D_003E7960);
    sdfCameraBuildProjection(&sdfSceneProjectionParameters);
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(600.0f, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F590);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, &eye);
    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, &target);
    sdfVuBuildLookAtBasis(&eye, &target, sdfViewUpVector);
    VU0_STORE_MATRIX_UNCLOBBERED(sdfViewMatrix);
    sdfConsBuildMatrixPacket(D_003820F0 + drawBufferIndex * 8000, &sdfSceneProjectionParameters, sdfViewMatrix);
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, sdfViewMatrix);
}

s8 evtStageTestUpdate(s32 frame) {
    s8 result = func_002C6CE8();

    if (result == 1) {
        return 1;
    }
    if (evtStageTestState.mode != 1) {
        if (evtStageTestState.model != 0) {
            mnuApplyModelCamera(evtStageTestState.model);
            evtStageTestApplyEntryRotation(evtStageTestState.model);
            evtStageTestUpdateCamera();
            if (evtStageTestState.modelUpdateStarted == 0) {
                evtStageTestState.modelUpdateStarted = 1;
            } else {
                if (evtStageTestState.pendingEffect >= 0) {
                    evtStageTestCreateModelEffect(evtStageTestState.pendingEffect);
                    evtStageTestState.pendingEffect = -1;
                }
                if (evtStageTestState.effect != 0) {
                    btlUpdateJobPositionFromModel(evtStageTestState.effect);
                }
                mdlProcessContextNodesAndTransforms(evtStageTestState.model, frame);
                evtStageTestAdvanceMotionQueue();
            }
        }
    }
    return result;
}

/* Count nonzero bytes in the active entry's motion column; any nonzero kind selects column 1. */
s32 evtStageTestCountFlags(s32 columnKind) {
    StageTestSlot *activeSlot = evtStageTestState.queue.slot;
    s32 entryIndex = activeSlot->entryIndex;
    s32 nonzeroCount = 0;
    u32 columnIndex;

    for (columnIndex = 0; columnIndex < EVT_STAGE_COLUMN_VALUE_COUNT; columnIndex++) {
        u8 columnValue;

        if (columnKind == 0) {
            columnValue = *(evtStageTestState.entries[entryIndex].column[0] + columnIndex);
        } else {
            columnValue = *(evtStageTestState.entries[entryIndex].column[1] + columnIndex);
        }
        if (columnValue) {
            nonzeroCount++;
        }
    }
    return nonzeroCount;
}

/* Index a motion column directly and queue a 15-frame blend.
 * Kinds 1/2 use column 1 with zero lead and clear/set fallback suppression;
 * all other kinds use column 0 with a 15-frame lead and retain that flag. */
void evtStageTestQueueMotion(s32 motionKind, u32 columnIndex) {
    StageTestSlot *activeSlot = evtStageTestState.queue.slot;
    f32 blendLeadFrames = 0.0f;
    s32 motionIndex;

    if (columnIndex < EVT_STAGE_COLUMN_VALUE_COUNT) {
        switch (motionKind) {
        default:
            blendLeadFrames = EVT_STAGE_DEFAULT_BLEND_FRAMES;
            motionIndex = *(evtStageTestState.entries[activeSlot->entryIndex].column[0] + columnIndex);
            break;
        case 1:
            motionIndex = *(evtStageTestState.entries[activeSlot->entryIndex].column[1] + columnIndex);
            activeSlot->flags &= ~EVT_STAGE_MOTION_SUPPRESS_FALLBACK;
            break;
        case 2:
            motionIndex = *(evtStageTestState.entries[activeSlot->entryIndex].column[1] + columnIndex);
            activeSlot->flags |= EVT_STAGE_MOTION_SUPPRESS_FALLBACK;
            break;
        }
        evtStageTestQueueMotionSegment(motionIndex, blendLeadFrames, EVT_STAGE_DEFAULT_BLEND_FRAMES);
    }
}

/* Queue a motion and its blend timing; inputs are truncated to whole frames.
 * The lead shifts initial motion time backwards, not to a start-frame endpoint. */
void evtStageTestQueueMotionSegment(u32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames) {
    StageTestSlot *activeSlot = evtStageTestState.queue.slot;

    activeSlot->state = EVT_STAGE_MOTION_QUEUED;
    activeSlot->motionIndex = motionIndex;
    activeSlot->blendLeadFrames = (s32)blendLeadFrames;
    activeSlot->blendDurationFrames = (s32)blendDurationFrames;
}

/* Request the fallback path without waiting for the active motion's end-state byte. */
void evtStageTestForceDefaultMotion(void) {
    evtStageTestState.queue.slot[0].state = EVT_STAGE_MOTION_FORCE_FALLBACK;
}

/* Idle and fallback-started states are settled; every other state is reported as pending. */
s32 evtStageTestHasPendingMotion(void) {
    s32 motionState = evtStageTestState.queue.slot[0].state;

    if ((motionState == EVT_STAGE_MOTION_IDLE) || (motionState == EVT_STAGE_MOTION_FALLBACK_STARTED)) {
        return 0;
    }
    return 1;
}

/* Start a queued motion, then the entry's fallback unless suppressed.
 * The native upper-bound checks do not reject negative motion indices.
 * Fallback is permitted by model-state byte 5 or an explicit force request. */
void evtStageTestAdvanceMotionQueue(void) {
    StageTestSlot *activeSlot = evtStageTestState.queue.slot;
    s32 motionIndex;
    s32 model;

    if (activeSlot->state != EVT_STAGE_MOTION_IDLE && activeSlot->state != EVT_STAGE_MOTION_FALLBACK_STARTED && (model = evtStageTestGetActiveModel()) != 0) {
        if (activeSlot->state == EVT_STAGE_MOTION_QUEUED) {
            motionIndex = activeSlot->motionIndex;

            if (motionIndex < mdlGetNodeRefHalf((MdlCtx *)model, 0)) {
                mdlAddEntryPlainEx(model, 0, motionIndex, (s32)activeSlot->blendLeadFrames, (s32)activeSlot->blendDurationFrames);
                activeSlot->state = EVT_STAGE_MOTION_PLAYING;
            }
        } else if (!(activeSlot->flags & EVT_STAGE_MOTION_SUPPRESS_FALLBACK) && (*(u8 *)(*(s32 *)(model + 0x1C) + 0x30) == 5 || activeSlot->state == EVT_STAGE_MOTION_FORCE_FALLBACK)) {
            motionIndex = evtStageTestState.entries[activeSlot->entryIndex].motionIndex;

            if (motionIndex < mdlGetNodeRefHalf((MdlCtx *)model, 0)) {
                mdlAddEntryFlaggedEx(model, 0, motionIndex, (s32)activeSlot->blendLeadFrames, (s32)activeSlot->blendDurationFrames);
                activeSlot->state = EVT_STAGE_MOTION_FALLBACK_STARTED;
            }
        }
    }
}

/* Replace the attached model effect with the native fixed request; argument is unused. */
void evtStageTestCreateModelEffect(s32 unused) {
    if (evtStageTestState.effect != 0) {
        evtStageTestDestroyModelEffect();
    }
    evtStageTestState.effect = sndCreateSystemEffectHandle(evtStageTestState.model, 0x30);
}

/* Destroy a nonzero model-effect handle and clear it; zero is already released. */
void evtStageTestDestroyModelEffect(void) {
    StageTestState *stage = &evtStageTestState;
    u32 effectHandle = stage->effect;

    if (effectHandle == 0) {
        return;
    }
    sndDestroyFileQueueWrapper(effectHandle);
    stage->effect = 0;
}

/* Store a request value: the update path tests its signed field for nonnegative.
 * The model-effect creator itself ignores the forwarded value. */
void evtStageTestSetPendingEffect(u32 requestValue) {
    evtStageTestState.pendingEffect = requestValue;
}

/* Report whether a model-effect handle is present. */
s32 evtStageTestHasModelEffect(void) {
    return evtStageTestState.effect != 0;
}

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B518);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B528);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B538);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B548);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B558);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7750);

u32 func_002C79B8(void) {
    func_002C7750();
    return 0;
}

/* Create the fixed-transform camera target and return its update callback. */
void *evtCreateBattleStageTestCamera(void) {
    f32 position[4] = {401.0f, -593.0f, -1208.25f, 0.0f};
    f32 orientation[4] = {0.22f, 0.12f, 0.03f, 1.0f};
    EffWorldNode *cameraTarget = evtCreateWorldObjectAtTransform(position, orientation);

    cameraTarget->value = (u32)D_00437CB0;
    return func_002C79B8;
}

/* Draw the battle-stage selector; confirmation creates the selected world object
 * and returns its camera callback before the four trigger-bit ID adjustments. */
void *evtBattleStageTestScreen(void) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(EVT_STAGE_SCREEN_WORK_BYTES);

    sdfInitPacketList(packetList);
    kwlnDrawSpriteCell(packetList, 0x84, 0x46, 0x14, 9);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7840, 0x7BA0, 0xFEFFFF, 0, "BATTLE STAGE"));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7A80, 0x7C60, 0xFEFFFF, 6, "F%03d_%03d", D_00437CB8, D_00437CBC));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R = EVENT SELECT"));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7D80, 0xFEFFFF, 0, "RR  = ENTER"));
    D_00380708.append((SdfListHead *)&D_00380708, packetList);
    if (D_0037F510[0x21] < 0) {
        evtCreateWorldObjectForKey(D_00437CB8, D_00437CBC);
        return evtCreateBattleStageTestCamera;
    }
    if (D_0037F510[0x25] & MNU_PAD_TRIGGER_BIT) {
        D_00437CB8++;
    }
    if (D_0037F510[0x24] & MNU_PAD_TRIGGER_BIT) {
        D_00437CB8--;
    }
    if (D_0037F510[0x2A] & MNU_PAD_TRIGGER_BIT) {
        D_00437CBC++;
    }
    if (D_0037F510[0x28] & MNU_PAD_TRIGGER_BIT) {
        D_00437CBC--;
    }
    return 0;
}

void evtDestroyBattleStageTestWorldNode(void) {
    evtDestroySecondaryWorldNode();
}

/* Destroy the named battle-stage-test task hierarchy and disable the debug graph. */
void evtBattleStageTestStopTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0042B610, 1);
    kwlnDebugGraphSetEnabled(0);
}


/* Enable the debug graph and create the battle-stage selector task with its cleanup callback. */
void btlCreateStageTestTask(void) {
    kwlnDebugGraphSetEnabled(1);
    kwlnTaskCreate(D_0042B610, 0x2B0C, 1, 1, evtBattleStageTestScreen, evtDestroyBattleStageTestWorldNode, 0);
}

typedef struct StageTestTaskWork {
    u8 pad00;
    u8 kind;            /* 0x01: kind 6 owns stage-test resources */
    u8 pad02[6];
    void *allocation;   /* 0x08 */
    s32 resource;       /* 0x0C */
    u8 pad10[0x20];
    u8 payload[1];      /* 0x30: passed to the stage cleanup routine */
} StageTestTaskWork;

/* Clean up kind-6 task resources, then free its allocation and task work.
 * Return zero when handled, or one for another task kind; retain the K&R definition. */
s32 btlDestroyStageTask(taskWork)
    StageTestTaskWork *taskWork;
{
    if (taskWork->kind == EVT_STAGE_RESOURCE_TASK_KIND) {
        s32 resource = taskWork->resource;
        if (resource != 0) {
            sdfDevQueueReleaseState(resource);
        }
        func_00346AF8(taskWork->payload);
        sdfReleaseChipBlock(taskWork->allocation);
        sdfReleaseChipBlock((void *)taskWork);
        return 0;
    }
    return 1;
}

void func_002C7CE8(void) {
    btlDestroyStageTask();
}

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B610);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C58);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C60);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C68);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C70);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C78);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C80);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C88);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C90);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C98);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C9C);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CA0);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CA4);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CA8);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CB0);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CB8);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CBC);

