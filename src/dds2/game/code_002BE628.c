#include "common.h"
#include "pcp_vu0.h"
#include "mnu.h"
extern s32 D_00437C9C;
extern s32 func_002B8E30();
extern s32 mnuScrollListToEnd();
extern void mnuClearWindowPanelTransitionFlag();
extern void func_002BE730();
extern void func_002BED10();
extern s32 func_002C6008();

extern s32 mnuLookupRangeEntry(u16);

extern u64 ptyGetCombinedRecordAndSlotValue(u64, s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_002C6CE8(void);

extern u32 func_002B9FF8(u32);

extern u32 effCreateStatusBatch(u32);

extern s32 datCommandRecords;

extern s32 datCommandSelectors;

extern void mnuSetPopupEntry(s32, s32);

extern f32 sdfSceneProjectionParameters[];

extern s8 D_003E7970[];
extern u8 D_003E7978[];

extern void evtStageTestQueueMotionSegment(u32, f32, f32);
extern void evtStageTestCreateModelEffect(s32);
extern void evtStageTestUpdateCamera(void);
extern void btlUpdateJobPositionFromModel(s32);
extern void mdlProcessContextNodesAndTransforms(s32, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern u32 ptyGetSkillNibbleState(s32, u16);

extern void evtStageTestStop(void);

extern u8 D_003E7940[];

extern u8 D_003E7950[];

extern s32 func_00328D68(u32);

extern void mnuPositionPanelItemPoints(s32, s32, s32);

extern s8 D_003E7928[];
extern s32 mdlRequestAsset(s32, s32, s32);

typedef struct MenuSelectionEntry {
    u8 pad00[0xE];
    u16 flags;
} MenuSelectionEntry;

extern u16 D_003E7900[];

extern u16 D_003E7902[];

extern s8 D_003E792A[];

typedef struct RangeEntry {
    u8 pad00;
    u8 flags;
    u8 pad02;
    u8 kind;
    u16 value;
    u16 addition;
    u8 pad08[0x1C];
    u8 secondaryKind; /* 0x24 */
    u8 pad25;
    u16 secondaryValue; /* 0x26 */
    u8 pad28[0x10];
} RangeEntry;

/* Counts checked before cursor moves; the primary/denominator pair also
 * supplies the fixed-point comparison used to sort available entries. */
typedef struct MenuItemCounts {
    u16 flags;       /* 0x00 */
    u8 pad02[4];
    u16 primary;     /* 0x06 */
    u16 denominator; /* 0x08 */
    u16 secondary;   /* 0x0A */
} MenuItemCounts;

typedef struct AffinityRow {
    s32 affinity[4];
} AffinityRow;

extern u32 datAffinityRecords;

extern void mdlAddEntryFlaggedEx(s32, s32, s32, f32, f32);

extern void mdlAddEntryPlainEx(s32, s32, s32, f32, f32);

extern void evtStageTestAdvanceMotionQueue(void);

extern s32 mdlGetNodeRefHalf(u32 node, s32 index);

typedef struct StageCameraTarget {
    u8 pad[8];
    void *unk8;
} StageCameraTarget;

extern StageCameraTarget *evtCreateWorldObjectAtTransform(f32 *, f32 *);

extern u8 D_00437CB0[];

extern void func_002C2AD0();

extern void sdfReleaseChipBlock();

extern u32 effMiscRand(s32);

extern u8 *datGameState;

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

typedef struct StageGraphicsCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} StageGraphicsCallback;

extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, s32);
extern void kwlnDrawSpriteCell(void *, s32, s32, s32, s32);
extern s32 sdfCreateFormattedSifCommand();
extern void evtCreateWorldObjectForKey(s32, s32);
extern s32 D_00437CB8;
extern s32 D_00437CBC;
extern StageGraphicsCallback D_00380708;
extern s8 D_0037F510[];

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE628);

typedef struct MenuSubBlock {
    u8 pad00[0x100];
    s32 value; /* 0x100 */
    u8 pad104[0xF20];
} MenuSubBlock;

typedef struct MenuPanelSlots {
    MenuSubBlock sub[2];
    u8 pad2048[0xF0];
} MenuPanelSlots;

typedef struct MenuPanelSet {
    u8 pad00[0x84];
    MenuPanelSlots panel[5]; /* 0x84, stride 0x2138 */
} MenuPanelSet;

/* Sets the value word of both sub-blocks of every panel. */
void mnuSetPanelSlotValues(MenuPanelSet *menu, s32 value) {
    MenuPanelSlots *panel = menu->panel;
    s32 i;
    s32 j;

    for (i = 0; i < 5; i++, panel++) {
        for (j = 0; j < 2; j++) {
            panel->sub[j].value = value;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE730);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BED10);

void func_002BEE38(u32 *entry) {
    entry[3] = 0x18;
    entry[4] = 5;
    *entry = 0;
}

typedef struct MenuQueuedCommand {
    u32 word00;
    s32 kind;         /* 0x04 */
    s32 option;       /* 0x08: chosen from the roster flag */
    u8 pad0C[8];
    s32 initialValue; /* 0x14 */
    s32 argument;     /* 0x18 */
} MenuQueuedCommand;

/* Retail returns int here without a return statement: the last call is a plain jal, not a sibcall. */
s32 mnuQueueListEntry(u8 *menu, s32 window, u32 kind, s32 argument) {
    u8 *base = menu + window * 0x2138;
    u8 *block = base + 0x78;
    s32 *count = (s32 *)(base + 0x17C);
    s32 best = 0x200;
    s32 bestIndex = 0;
    s32 i;
    u8 *entry;
    u32 flags;

    for (i = 0; i < 2; i++, count += 0x409) {
        if (*count < best) {
            best = *count;
            bestIndex = i;
        }
    }
    flags = *(u16 *)(datGameState + window * 0x1C4 + 0xA60);
    entry = block + bestIndex * 0x1024 + 0xF0;
    ((MenuQueuedCommand *)entry)->initialValue = 0x200;
    ((MenuQueuedCommand *)entry)->argument = argument;
    ((MenuQueuedCommand *)entry)->kind = kind;
    if ((flags & 2) != 0) {
        ((MenuQueuedCommand *)entry)->option = 0;
    } else {
        ((MenuQueuedCommand *)entry)->option = 1;
    }
    switch (kind) {
    case 0:
        func_002BE730(entry);
        *(s32 *)(menu + 0xA6A0) = 0;
        break;
    case 1:
        func_002BED10(entry);
        *(s32 *)(menu + 0xA6A0) = 0;
        break;
    case 2:
        func_002BEE38(entry);
        break;
    }
}

void mnuClearSpriteRecord(u32 *entry) {
    entry[0] = 0;
    entry[1] = 0;
    entry[2] = 0;
    entry[3] = 0;
    entry[4] = 0;
    entry[5] = 0;
    entry[6] = 0;
}

void mnuClearPairedSpriteRecords(s32 menu, s32 index) {
    s32 record;
    s32 remaining;

    remaining = 1;
    record = index * 0x2138 + menu + 0x168;
    do {
        remaining = remaining - 1;
        mnuClearSpriteRecord(record);
        record = record + 0x1024;
    } while (-1 < remaining);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF000);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF238);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF478);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF660);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B0D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF830);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BFEA0);

typedef struct MenuListNode {
    u8 pad00[0xC];
    s32 overrideValue;
} MenuListNode;

typedef struct MenuListPanel {
    s32 mode;        /* 0x00 */
    u32 layoutFlags; /* 0x04: bit 0x40 forces the regular list spacing */
    u8 pad08[0xD4];
    MenuListNode *node;
    u8 padE0[0x2058];
} MenuListPanel;

typedef struct MenuListState {
    u32 flags;
    s32 transitionValue; /* 0x04: advances toward 0x100 */
    s32 *entryCount; /* 0x08 */
    u8 pad0C[0xA68C];
    s32 selectedPanel; /* 0xA698 */
    s32 scrollOffset; /* 0xA69C */
} MenuListState;

extern void func_002BF830(s32, s32, s32, MenuListState *, s32, s32);
extern void func_002BFEA0(s32, s32, s32, MenuListState *, s32, s32);

void mnuDispatchListPanel(s32 x, s32 y, s32 z, MenuListState *menu, s32 panelIndex, s32 param) {
    MenuListPanel *panel = (MenuListPanel *)((u8 *)menu + 0x78) + panelIndex;
    s32 mode = panel->mode;

    if (menu->selectedPanel >= 0) {
        mode = 1;
    }
    if (panel->layoutFlags & 0x40) {
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

void mnuCalcListEntryOffset(s32 *out, MenuListState *menu, s32 index) {
    MenuSpacing spacing = {0x310, 0x370, 0x190};
    s32 count = *menu->entryCount;
    s32 mode;

    if (!(((MenuListPanel *)((s32)menu + index * 0x2138 + 0x78))->layoutFlags & 0x40)) {
        mode = index < count ? 1 : 2;
    } else {
        mode = 1;
    }
    if (menu->selectedPanel >= 0) {
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

/* Advance the panel's current transition value toward its 0x100 limit. */
void mnuAdvancePanelTransition(MenuListState *menu) {
    if (menu->transitionValue < 0x100) {
        menu->transitionValue = menu->transitionValue + 8;
    }
}

/* Apply a temporary override to the selected node while drawing its panel. */
void mnuDrawPanelWithTemporaryOverride(s32 x, s32 y, s32 z, s32 overrideValue, MenuListState *menu, s32 param) {
    s32 positionOffset[2];
    MenuListPanel *panel = (MenuListPanel *)((s32)menu + menu->selectedPanel * 0x2138 + 0x78);
    MenuListNode *node;

    mnuCalcListEntryOffset(positionOffset, menu, 0);
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = overrideValue;
    }
    x += menu->scrollOffset * 0x10;
    menu->scrollOffset = (s32)((f32)menu->scrollOffset / 1.19999993f);
    /* Both arms are identical in retail; kept as written. */
    if (menu->flags & 0x80) {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selectedPanel, param);
    } else {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selectedPanel, param);
    }
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = 0;
    }
}

void mnuDrawListPanels(s32 x, s32 y, s32 z, s32 overrideValue, MenuListState *menu, s32 param) {
    s32 positionOffset[2];
    s32 *layout = menu->entryCount;
    s32 count;
    s32 i;

    count = layout[0];
    count += layout[1];
    if (menu->selectedPanel >= 0) {
        mnuDrawPanelWithTemporaryOverride(x, y, z, overrideValue, menu, param);
    } else {
        for (i = 0; i < count; i++) {
            mnuCalcListEntryOffset(positionOffset, menu, i);
            mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, i, param);
        }
    }
    mnuAdvancePanelTransition((s32)menu);
}

void mnuDrawPanelListDefault(s32 x, s32 y, s32 depth, s32 source, s32 mode, s32 option) {
    mnuDrawListPanels(x, y, depth, 0, (MenuListState *)source, mode);
}

typedef struct MenuPoint {
    s32 x;
    s32 y;
} MenuPoint;

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
    u32 resourceHandle; /* 0x88 */
} MenuPanelState;

void *mnuCreatePanelState(s32 width, s32 height) {
    MenuPanelState *panel = (MenuPanelState *)func_00328D68(0x8C);

    memset(panel, 0, 0x8C);
    panel->width = width;
    panel->height = height;
    return panel;
}

void mnuDestroyPanelState(MenuPanelState *panel) {
    s32 resourceHandle;

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

void mnuInitializePanelResource(MenuPanelState *panel) {
    u32 resourceHandle;

    resourceHandle = func_002B9FF8(5);
    panel->resourceHandle = resourceHandle;
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

extern s32 mnuCreatePanelItem(void);

extern void func_002C26D8(s32, s32, s32, s32, s32);

typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
    s32 texture;       /* 0x0C */
    s32 entries[5];    /* 0x10 */
    u32 selection;     /* 0x24 */
    s32 initialValue;  /* 0x28: initialized to 0x100 */
} MenuPanelGroup;

extern void mnuClearPanelGroupSelection(MenuPanelGroup *);

s32 mnuCreatePanelGroup(s32 owner, s32 texture, s32 mode) {
    MenuPanelGroup *group = (MenuPanelGroup *)func_00328D68(0x2C);
    s32 *slot = group->entries;
    s32 index;
    for (index = 0; index < 5; index++) {
        s32 entry = mnuCreatePanelItem();
        func_002C26D8(entry, owner, texture, mode, index);
        *slot++ = entry;
    }
    mnuClearPanelGroupSelection(group);
    group->texture = texture;
    group->initialValue = 0x100;
    return (s32)group;
}

void mnuDestroyPanelGroup(MenuPanelGroup *group) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_002C2AD0(group->entries[i]);
    }
    sdfReleaseChipBlock(group);
}

void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 data) {
    s32 i;

    for (i = 0; i < 5; i++) {
        mnuPositionPanelItemPoints(group->entries[i], data, i);
    }
}

void mnuSetPanelGroupSelection(MenuPanelGroup *group, u32 selection) {
    group->selection = selection;
}

void mnuClearPanelGroupSelection(MenuPanelGroup *group) {
    group->selection = 0xffffffff;
}

u32 mnuGetPanelGroupSelection(MenuPanelGroup *group) {
    return group->selection;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0D18);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selected, u32 flags) {
    /* Required to match: retain byte-offset indexing for the child slot. */
    s32 offset = index * 4 + 0x10;
    u32 *entry = (u32 *)((u8 *)group + offset);
    mnuSetPanelItemSelection(*entry, selected);
    mnuSetPanelItemOption(*entry, flags);
}

void mnuSetIndexedPanelGroupValue(MenuPanelGroup *group, s32 index, u32 value) {
    mnuStorePanelItemValue(group->entries[index], value);
}

/* Apply each of the five packed values to its corresponding panel entry. */
void mnuApplyPackedGroupValues(MenuPanelGroup *group, u64 value) {
    u32 child;
    u64 entryValue;
    s32 nextIndex;
    u32 *entries;
    s32 index;

    entries = (u32 *)group->entries;
    index = 0;
    do {
        nextIndex = index + 1;
        entryValue = ptyGetCombinedRecordAndSlotValue(value, index);
        child = *entries;
        entries = entries + 1;
        mnuStorePanelItemValue(child, entryValue);
        index = nextIndex;
    } while (nextIndex < 5);
}

typedef struct MenuSpriteState {
    u8 pad00[0x10];
    s32 x;
    s32 y;
    s32 z;
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} MenuSpriteState;

void *mnuCreateSpriteState(s32 x, s32 y, s32 z) {
    MenuSpriteState *item = func_00328D68(0x20);
    memset(item, 0, 0x20);
    item->x = x;
    item->y = y;
    item->z = z;
    item->initialValue = 0x100;
    return item;
}

void func_002C1050(void) {
    sdfReleaseChipBlock();
}

/* The sequel selects its draw variant from the range index plus eight. */
void mnuDrawRangeSpriteVariant(u32 x, u32 y, u32 depth, u32 color,
                                    u16 rangeId, u32 drawArg, u32 texture) {
    s32 rangeIndex;

    rangeIndex = mnuLookupRangeEntry(rangeId);
    func_00306CD0(x, y, depth, color, 1, drawArg, rangeIndex + 8, texture);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C10F0);

void *mnuAllocateSimpleSprite(s32 x, s32 y, s32 z) {
    MenuSpriteState *item = func_00328D68(0x20);
    memset(item, 0, 0x20);
    item->x = x;
    item->y = y;
    item->z = z;
    item->initialValue = 0x100;
    return item;
}

void func_002C16D8(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C16F0);

typedef struct MenuGradientFade {
    s32 active;
    s32 color;
    s32 blend;
} MenuGradientFade;

void func_002C1B58(MenuGradientFade *state, s32 color) {
    state->color = color;
    state->active = 0;
    state->blend = 0;
}

void func_002C1B68(u32 *out, u32 value) {
    *out = value;
}

extern u32 uiBlendColors(u32, u32, u32);
extern void func_003089B8(u32, u32, u32, u32, u32, u32, u32);

void func_002C1B70(MenuGradientFade *state, s32 surface) {
    s32 colors[4];
    s32 color = state->color;

    color = uiBlendColors(color, color & ~0xFF, state->blend);
    panelSetVec4((u32 *)colors, 0, 0, color, color);

    func_003089B8(0, 0x700, 0, 0x2000, 0x700, (u32)colors, surface);
    if (state->active != 0) {
        state->blend += 0x20;
        if (state->blend > 0x100) {
            state->blend = 0x100;
        }
    } else {
        state->blend -= 0x20;
        if (state->blend < 0) {
            state->blend = 0;
        }
    }
}

void mnuDrawRepeatedPanelSprites(u8 *object, s32 x, s32 y, s32 depth, s32 count, s32 drawArg, s32 variant, s32 texture) {
    s32 i;

    for (i = 0; i < count; i++) {
        func_00306CD0(object, x, y, depth, 1, drawArg, variant, texture);
        object += 0xA0;
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

typedef struct MenuEffectPair {
    u8 pad00[0x14];
    s32 *settings; /* 0x14: four selectable effect settings */
    u8 settingIndex; /* 0x18 */
    s8 positionY; /* 0x19 */
    u8 pad1A[0x0E];
    s32 configurationHandle; /* 0x28 */
    u8 pad2C[0x0C];
    MenuEffectNode *first;  /* 0x38 */
    MenuEffectNode *second; /* 0x3C */
} MenuEffectPair;

/* Feed two mirrored effect positions from the active menu entry. */
void mnuSetPairedEffectPositions(MenuEffectPair *pair) {
    MenuEffectNode *first = pair->first;
    MenuEffectNode *second = pair->second;
    MenuEffectPosition *firstData = first->position;
    MenuEffectPosition *secondData = second->position;
    s32 *pos = firstData->coordinates;

    pos[0] = 10;
    pos[1] = pair->positionY;
    pos[2] = 10;
    pos = secondData->coordinates;
    pos[1] = 5;
    pos[0] = 10;
    pos[2] = 10;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1D10);

/* Cycle through four indexed settings while refreshing the paired effects. */
void mnuCyclePairedEffectSetting(MenuEffectPair *pair) {
    s32 *settings;
    s32 setting;

    func_002C1D10(pair);
    mnuSetPairedEffectPositions(pair);
    settings = pair->settings;
    setting = 0;
    if (settings != 0) {
        setting = settings[(s8)pair->settingIndex];
    }
    effConfigureWithDefaultSetting(pair->configurationHandle, 0, (s32)pair->first, 0, setting, 0);
    pair->settingIndex += 1;
    if ((s8)pair->settingIndex >= 4) {
        pair->settingIndex = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1E48);

void mnuCreatePairedEffects(MenuEffectPair *pair) {
    u32 effectHandle;

    effectHandle = effCreateStatusBatch(3);
    pair->first = (MenuEffectNode *)effectHandle;
    effectHandle = effCreateStatusBatch(3);
    pair->second = (MenuEffectNode *)effectHandle;
}

void mnuReleasePairedEffectBatches(s32 *list) {
    u32 i;

    for (i = 0; i < 2; i++) {
        effDestroyPackedBatch(list[i + 14]);
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

void mnuReleaseSpriteTextures(u32 *group) {
    u32 *entry = group + 7;
    u32 index = 0;
    do {
        effDestroyResourceSlotSet(*entry++);
        index++;
    } while (index < 7);
    mnuReleasePairedEffectBatches(group);
}

u32 mnuGetPanelRatioColor(s32 useDefault, s32 index, s32 option) {
    u32 color = 0xA09DC380;
    if (!useDefault) {
        switch (mnuClassifyQuarterHalfPercent(index, option)) {
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

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C22D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C24E8);

/* DDS2's panel item is wider than the DDS1 variant, with five points at +0x74. */
typedef struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    u32 value24;
    u32 value28;
    u8 pad2C[0x48];
    MenuPoint points[5]; /* 0x74 */
    u8 pad9C[4];
    u32 initialValue; /* 0xA0 */
    u32 selectionRamp; /* 0xA4 */
    u8 padA8[4];
} MenuPanelItem;

s32 mnuCreatePanelItem(void) {
    MenuPanelItem *item = (MenuPanelItem *)func_00328D68(0xAC);

    memset(item, 0, 0xAC);
    item->value14 = 0x63;
    item->value10 = 0x8c;
    item->initialValue = 0x100;
    return (s32)item;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C26D8);

/* Arrange five panel points using the parent layout's fixed anchor slots. */
void mnuPositionPanelItemPoints(s32 obj, s32 param, s32 index) {
    s32 table[5] = {0, 4, 1, 2, 3};
    MenuPanelItem *item = (MenuPanelItem *)obj;

    itfGridStorePosition(&item->points[0], param, 7);
    itfSetGridEntryQuantizedAndRefresh(item->points[0].x, item->points[0].y, -0x50, -0x50, 0, 0);
    itfGridStorePosition(&item->points[1], param, 5);
    itfSetGridEntryQuantizedAndRefresh(item->points[1].x, item->points[1].y, 0x390, -8, 0, 0);
    itfGridStorePosition(&item->points[2], param, 6);
    itfSetGridEntryQuantizedAndRefresh(item->points[2].x, item->points[2].y, 0x390, -8, 0, 0);
    itfGridStorePosition(&item->points[3], param, 9);
    itfSetGridEntryQuantizedAndRefresh(item->points[3].x, item->points[3].y, 0x5D0, 0, 0, 0);
    itfGridStorePosition(&item->points[4], param, table[index]);
    itfSetGridEntryQuantizedAndRefresh(item->points[4].x, item->points[4].y, 0x130, -0x30, 0, 0);
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

void func_002C2AD0(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C2AE8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C3010);

void mnuSetProfilePanelValues(MenuPanelItem *item, s32 value, s32 option) {
    item->value10 = value;
    item->value14 = option;
}

u32 *mnuCreateProfilePanel(s32 source) {
    u32 *item = (u32 *)func_00328D68(0x48);
    s32 first;
    u32 second;
    u32 i;

    memset(item, 0, 0x48);
    first = scrGetSelectedScriptEntryId(source);
    second = ptyGetCurrentProfileRecord(source);
    mnuSetProfilePanelValues(item, ptyGetProfileRecordCap((u16)first), *(u32 *)second);
    for (i = 0; i < 5; i++) {
        item[11 + i] = effMiscRand(0) % 0xC0 + 0x40;
    }
    item[17] = 0x100;
    return item;
}

void func_002C3390(void) {
    sdfReleaseChipBlock();
}

void mnuSetGroupProperties(u32 *entry, u32 first, u32 second, u32 third, u32 fourth) {
    entry[6] = first;
    entry[7] = second;
    entry[9] = third;
    entry[10] = fourth;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C33C0);

void mnuDrawAndAdvanceProfilePanel(s32 x, s32 y, s32 z, u32 *item, s32 option) {
    s32 previous;
    s32 next;

    func_002C33C0(x, y, z, item, option);
    previous = item[16];
    next = previous + 1;
    if (previous < 0x200) {
        item[16] = next;
        if (next < 0x200) {
            return;
        }
        previous = next;
    }
    item[16] = previous - 0x200;
}

void mnuClearPanelTransitionState(u32 item) {
    memset(item, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C3E78);

void mnuDrainPanelTransitions(u32 item, u32 option) {
    s32 currentValue;

    currentValue = *(s32 *)item;
    while (currentValue != 0) {
        func_002C3E78(1, 0, item, option);
        currentValue = *(s32 *)item;
    }
}

s32 mnuHasPopupSelectionFlag(s32 *flags) {
    return (*flags & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4038);

/* Popup state retains the entry address at +0x44, as in DDS1. */
typedef struct MenuPopupState {
    u8 pad00[0x44];
    s32 entryAddress;
} MenuPopupState;

u8 mnuIsPopupEntryValue(s32 item, s32 value) {
    return ((MenuPopupState *)item)->entryAddress == value;
}

void mnuSetPopupEntry(s32 out, s32 entry) {
    u16 lowHalf;

    lowHalf = *(u16 *)entry;
    *(s32 *)out = entry;
    *(s32 *)entry = lowHalf;
}

void mnuSetPopupEntryFlagged(s32 out, s32 entry) {
    u16 lowHalf;

    lowHalf = *(u16 *)entry;
    *(s32 *)out = entry;
    *(s32 *)entry = lowHalf | 0x20000;
}

void mnuAttachAndMarkMenuEntry(s32 out, s32 entry) {
    u16 lowHalf;

    lowHalf = *(u16 *)entry;
    *(s32 *)out = entry;
    *(s32 *)entry = lowHalf | 0x60000;
}

void mnuBindPresentMenuEntry(s32 item, u32 out) {
    if (((MenuPopupState *)item)->entryAddress != 0) {
        mnuSetPopupEntryFlagged(out, ((MenuPopupState *)item)->entryAddress);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4328);

typedef struct PartyPanelSlot {
    s32 unk0;
    s32 index;
    s32 unk8;
    s32 pad[10];
} PartyPanelSlot;

typedef struct PartyPanel {
    s32 unk0;
    s32 unk4;
    PartyPanelSlot slots[5];
} PartyPanel;

extern void func_002C4328(u8 *entry, s32 arg1, u32 index, PartyPanel *panel);

void mnuInitPartyPanelSlots(PartyPanel *panel) {
    u32 i;
    u8 *entry;
    s32 offset = 0;

    memset(panel, 0, 0x10C);
    panel->unk0 = 0;
    panel->unk4 = 0;
    for (i = 0; i < 5; i++) {
        entry = datGameState + offset + 0xA60;
        offset += 0x1C4;
        if (*(u16 *)entry & 1) {
            func_002C4328(entry, 0, i, panel);
            panel->slots[i].index = i;
        } else {
            panel->slots[i].unk8 = -1;
        }
    }
}

extern u8 D_0037F530[];

/* Translate a mask of pad buttons into held/pressed flags: low bits test each digital button (sign bit or pressure bit), the high bits
 * report the same inputs as plain non-zero tests. Select (bit 0) overrides everything else. */
s32 mnuMapPadMaskToFlags(s32 buttons) {
    s32 out = 0;

    if (buttons & 0x1) {
        if (D_0037F510[0x21] < 0) {
            out = 1;
        }
    }
    if (buttons & 0x2) {
        if (D_0037F510[0x23] < 0) {
            out |= 0x2;
        }
    }
    if (buttons & 0x10) {
        if (D_0037F530[6] & 2) {
            out |= 0x10;
        }
    }
    if (buttons & 0x20) {
        if (D_0037F530[7] & 2) {
            out |= 0x20;
        }
    }
    if (buttons & 0x40) {
        if (D_0037F530[4] & 2) {
            out |= 0x40;
        }
    }
    if (buttons & 0x80) {
        if (D_0037F530[5] & 2) {
            out |= 0x80;
        }
    }
    if (buttons & 0x4) {
        if (D_0037F510[0x22] < 0) {
            out |= 0x4;
        }
    }
    if (buttons & 0x8) {
        if (D_0037F510[0x20] < 0) {
            out |= 0x8;
        }
    }
    if (buttons & 0x200) {
        if (D_0037F530[0xA] & 2) {
            out |= 0x200;
        }
    }
    if (buttons & 0x100) {
        if (D_0037F530[8] & 2) {
            out |= 0x100;
        }
    }
    if (buttons & 0x800) {
        if (D_0037F530[0xB] & 2) {
            out |= 0x800;
        }
    }
    if (buttons & 0x400) {
        if (D_0037F530[9] & 2) {
            out |= 0x400;
        }
    }
    if (buttons & 0x1000) {
        if (D_0037F510[0x2C] < 0) {
            out |= 0x1000;
        }
    }
    if (buttons & 0x2000) {
        if (D_0037F510[0x2D] < 0) {
            out |= 0x2000;
        }
    }
    if (buttons & 0x20000) {
        if (D_0037F510[0x2A] < 0) {
            out |= 0x20000;
        }
    }
    if (buttons & 0x10000) {
        if (D_0037F510[0x28] < 0) {
            out |= 0x10000;
        }
    }
    if (buttons & 0x80000) {
        if (D_0037F510[0x2B] < 0) {
            out |= 0x80000;
        }
    }
    if (buttons & 0x40000) {
        if (D_0037F510[0x29] < 0) {
            out |= 0x40000;
        }
    }
    if (buttons & 0x10) {
        if (D_0037F510[0x26] != 0) {
            out |= 0x100000;
        }
    }
    if (buttons & 0x20) {
        if (D_0037F510[0x27] != 0) {
            out |= 0x200000;
        }
    }
    if (buttons & 0x40) {
        if (D_0037F510[0x24] != 0) {
            out |= 0x400000;
        }
    }
    if (buttons & 0x80) {
        if (D_0037F510[0x25] != 0) {
            out |= 0x800000;
        }
        if (D_0037F510[0x2A] != 0) {
            out |= 0x1000000;
        }
    }
    if (buttons & 0x40) {
        if (D_0037F510[0x28] != 0) {
            out |= 0x2000000;
        }
    }
    if (buttons & 0x80) {
        if (D_0037F510[0x2B] != 0) {
            out |= 0x4000000;
        }
    }
    if (buttons & 0x40) {
        if (D_0037F510[0x29] != 0) {
            out |= 0x8000000;
        }
    }
    if (out & 1) {
        out = out & 1;
    }
    return out;
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

void mnuPlayInputSound(s32 unused, s32 buttons, s32 *state) {
    if (buttons & 0x8000) {
        sndSetSequenceVolumePan(0xD, 0x7F, 0x3F);
        return;
    }
    if (buttons & 0x4000) {
        sndSetSequenceVolumePan(0xC, 0x7F, 0x3F);
        return;
    }
    if (buttons != 0) {
        if (buttons & 1) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (buttons & 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
        }
        if (buttons & 0xCF0) {
            if (state != NULL) {
                if ((*state & 3) != 2) {
                    sndSetSequenceVolumePan(0, 0x7F, 0x3F);
                }
            } else {
                sndSetSequenceVolumePan(0, 0x7F, 0x3F);
            }
        }
    }
}

void mnuPlayInputSoundKind(s32 buttons, s8 kind) {
    if (buttons & 0x8000) {
        sndSetSequenceVolumePan(0xD, 0x7F, 0x3F);
        return;
    }
    if (buttons != 0) {
        if (buttons & 0x4000) {
            sndSetSequenceVolumePan(0xC, 0x7F, 0x3F);
        }
        if (buttons & 1) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (buttons & 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
        }
        if (buttons & 0x1000) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (buttons & 0x2000) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (buttons & 0xF0FF0) {
            switch (kind) {
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

void func_002C4B40(u32 buttons) {
    mnuPlayInputSoundKind(buttons, 0);
}

typedef struct MenuPanelEntry {
    u16 flags;
    u8 unknown02[2];
    u16 tableIndex; /* 0x04 */
    u8 unknown06[4];
    u16 weight;     /* 0x0A */
    u8 unknown0C[2];
    u16 state;      /* 0x0E */
    u8 unknown10[4];
    u16 marker;
    u8 unknown16[0x3C];
    u16 menuValue; /* 0x52 */
    u8 unknown54[0x15E];
    u16 currentId;
    u8 unknown1B4[0x10];
} MenuPanelEntry;

s32 mnuFindMatchingPartyEntryIndex(s32 object) {
    s32 i;
    u8 *entry = datGameState + 0xA60;
    for (i = 0; i < 5; i++, entry += 0x1C4) {
        if ((((MenuPanelEntry *)entry)->flags & 1) &&
            ((MenuPanelEntry *)object)->tableIndex == ((MenuPanelEntry *)entry)->tableIndex) {
            return i;
        }
    }
    return 0;
}

s32 mnuLookupRangeEntry(u16 rangeId) {
    u16 *table = D_003E7900;
    s8 *entries = D_003E7928;
    u32 key = rangeId & 0xffff;
    u32 i;
    for (i = 0; i < 0x14; i += 2, table += 2) {
        if (key < table[0]) {
            continue;
        }
        if (key >= table[1]) {
            continue;
        }
        {
            u32 j = 0;
            s8 *entry = entries + 1;
            for (; j < 6; j++, entry += 3) {
                if (i != entry[1]) {
                    continue;
                }
                return entry[0];
            }
        }
    }
    return 0;
}

u16 mnuPickPairedTableValue(s32 index, s32 alt) {
    return (alt == 0) ? D_003E7900[index] : D_003E7902[index];
}

s32 mnuGetIndexedNonzeroEffect(s32 index) {
    s32 count = 0;
    s32 i;
    s8 *entry = D_003E7928;
    for (i = 0; i < 6; i++, entry += 3) {
        s32 value = *entry;
        if (value != 0) count++;
        if (index == count - 1) return value;
    }
    return 0;
}

u16 mnuLookupPartyTableValue(u32 count, s32 base, s32 which) {
    u32 i;
    s32 sum = 0;

    for (i = 0; i < count; i++) {
        sum += mnuGetIndexedNonzeroEffect(i);
    }
    if (which == 0) {
        return D_003E7900[D_003E792A[(base + sum) * 3]];
    }
    return D_003E7902[D_003E792A[(base + sum) * 3]];
}

/* Only secondary-kind-2 entries expose the paired value. */
u16 mnuGetSecondaryValueIfKind2(s32 entryId) {
    RangeEntry *entry = (RangeEntry *)((entryId & 0xffff) * 56 + datCommandRecords);

    if (entry->secondaryKind != 2) {
        return 0;
    }
    return entry->secondaryValue;
}

u8 mnuGetRangeEntryKind(u32 id) {
    return ((RangeEntry *)((id & 0xffff) * 0x38 + datCommandRecords))->kind;
}

u16 mnuGetAdjustedEntryValue(s32 id, s32 object) {
    RangeEntry *entry = (RangeEntry *)((id & 0xFFFF) * 0x38 + datCommandRecords);
    u16 base = entry->value;
    u16 addition = entry->addition;
    if (mnuGetRangeEntryKind(id & 0xFFFF) == 1) {
        base = addition + ((MenuItemCounts *)object)->denominator * base / 100;
    }
    return base;
}

s32 mnuGetRangeEntryFlatValue(s32 id) {
    s32 index = id & 0xFFFF;
    RangeEntry *record = (RangeEntry *)(index * 0x38 + datCommandRecords);
    s32 scale = record->value;
    s32 addition = record->addition;

    if (mnuGetRangeEntryKind(index) == 1) {
        return scale + addition;
    }
    return scale;
}

s32 mnuCanAffordEntryCost(u16 id, s32 item) {
    u16 minimum = ((RangeEntry *)datCommandRecords)[id].value;
    s32 kind = mnuGetRangeEntryKind(id);

    switch (kind) {
    case 1:
        if (((MenuItemCounts *)item)->primary < minimum) {
            return 0;
        }
        break;
    case 2:
        if (((MenuItemCounts *)item)->secondary < minimum) {
            return 0;
        }
        break;
    }
    return 1;
}

extern s32 mnuCanAffordEntryCost(u16, s32);

s32 mnuGetEntryUseStatus(s32 context, u16 id) {
    if (mnuCanAffordEntryCost(id, context) == 0) return -1;
    if ((((RangeEntry *)(datCommandRecords + id * 56))->flags & 1) == 0) return 1;
    if (id < 0x220) return 0;
    return 1;
}

s32 mnuIsEntryCostUnaffordable(u16 id, s32 object) {
    s32 kind = ((RangeEntry *)datCommandRecords)[id].kind;
    u16 value = ((RangeEntry *)datCommandRecords)[id].value;

    switch (kind) {
    case 1:
        if (((MenuItemCounts *)object)->primary < value) {
            return 1;
        }
        break;
    case 2:
        if (((MenuItemCounts *)object)->secondary < value) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 mnuConsumeEntryCost(s32 id, u8 *cursor) {
    RangeEntry *record = (RangeEntry *)((id & 0xFFFF) * 0x38 + datCommandRecords);
    u16 amount = record->value;

    switch (record->kind) {
    case 1:
        if (((MenuItemCounts *)cursor)->primary < amount) {
            return 0;
        }
        datMoveCursorX(cursor, -amount);
        return 1;
    case 2:
        if (((MenuItemCounts *)cursor)->secondary < amount) {
            return 0;
        }
        datMoveCursorY(cursor, -amount);
        return 1;
    default:
        return 1;
    }
}

s32 mnuGetAbilityByteCategory(u16 ability) {
    u8 value;
    if (ability == 0) {
        return 1;
    }
    value = *(u8 *)(datCommandRecords + ability * 56 + 8);
    switch (value) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

void func_002C5128(u16 ability) {
    func_00119548(ability);
}

u32 func_002C5140(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", ptySkillApplyFieldUseEffect);

u8 mnuIsAbilityValueMarked(u32 id) {
    return *(s8 *)((id & 0xffff) * 2 + datCommandSelectors) == '\x01';
}

s32 ptyGetAffinityKind(s32 affinityId, s32 index) {
    s32 flags = ((AffinityRow *)datAffinityRecords)[(affinityId - 0x1AB) & 0xFFFF].affinity[index];

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
    s32 value = ((AffinityRow *)datAffinityRecords)[(affinityId - 0x1AB) & 0xFFFF].affinity[index];

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

u32 ptyCountBulletItem(s32 id) {
    u32 value;
    s32 index;
    if (id < 0xA0) return 0;
    if (id >= 0xBF) return 0;
    value = *(u8 *)(id + (s32)datGameState + 0x1340);
    for (index = 0; index < 5; index++) {
        MenuPanelEntry *entry = (MenuPanelEntry *)(datGameState + 0xA60) + index;
        if (id == mnuGetPartyEntryMenuValue((s32)entry)) {
            value++;
        }
    }
    return value;
}

u32 mnuSetPartyEntryMenuValue(s32 entry, u16 value) {
    ((MenuPanelEntry *)entry)->menuValue = value;
    return 1;
}

u16 mnuGetPartyEntryMenuValue(s32 entry) {
    return ((MenuPanelEntry *)entry)->menuValue;
}

u32 mnuSetPartyEntryCurrentId(u32 entry, u32 id) {
    ((MenuPanelEntry *)entry)->currentId = (s16)id;
    mnuMarkEntryBlocked(id);
    ptyRecomputeMaxHpMp(entry);
    return 1;
}

u16 mnuGetPartyEntryCurrentId(s32 entry) {
    return ((MenuPanelEntry *)entry)->currentId;
}

MenuPanelEntry *mnuFindPartySlotByCurrentId(u32 id) {
    s32 index;
    MenuPanelEntry *entry = (MenuPanelEntry *)(datGameState + 0xA60);
    for (index = 0; index < 5; index++, entry++) {
        if ((entry->flags & 1) && id == entry->currentId) {
            return entry;
        }
    }
    return 0;
}

MenuPanelEntry *mnuFindReserveSlotByCurrentId(u32 id) {
    s32 index;
    MenuPanelEntry *entry = (MenuPanelEntry *)(datGameState + 0x1CA10);
    for (index = 0; index < 16; index++, entry++) {
        if (entry->marker != 0 && (entry->flags & 1) && id == entry->currentId) {
            return entry;
        }
    }
    return 0;
}

void mnuMarkEntryBlocked(s32 index) {
    s32 offset = 0x1E730 + index;
    if (index != 0) {
        datGameState[offset] |= 1;
    }
}

void mnuClearEntryBlocked(s32 index) {
    s32 offset = 0x1E730 + index;
    if (index != 0) {
        datGameState[offset] &= ~1;
    }
}

s32 mnuIsEntryBlocked(s32 index) {
    if (index == 0) {
        return 1;
    }
    return *(u8 *)(index + (s32)datGameState + 0x1E730) & 1;
}

s32 mnuHasOwnedUnblockedItem(void) {
    s32 index;
    for (index = 0xC0; index < 0x100; index++) {
        if (!mnuIsEntryBlocked(index) && *(u8 *)(index + (s32)datGameState + 0x1340) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 sndPlayPartyItemSe(u32 id, s32 mode) {
    u16 *entry = D_003E78D8;
    u32 i;

    for (i = 0; i < 5; i++, entry += 4) {
        if (id == entry[0]) {
            switch (mode) {
            case 0:
                id = entry[1];
                id += 0x10;
                break;
            case 1:
                id = entry[2];
                id += 0x17;
                break;
            default:
                id = entry[3];
                id += 0x17;
                break;
            }
            dspStartEntry(id);
            return 1;
        }
    }
    return 0;
}

/* Party-entry vitals and permanent bonuses; the unused bytes retain the retail layout. */
typedef struct BtlPermanentBonusUnit {
    u8 pad00[6];
    u16 currentHp;   /* 0x06 */
    u16 maxHp;       /* 0x08 */
    u16 currentMp;   /* 0x0A */
    u16 maxMp;       /* 0x0C */
    u16 statusFlags; /* 0x0E: bit 0x4000 prevents the refill below */
    u8 pad10[6];
    s8 baseStats[5];
    u8 pad1B;
    u16 hpBonus;     /* 0x1C: added by ptyComputeMaxHp */
    u16 mpBonus;     /* 0x1E: added by ptyComputeMaxMp */
} BtlPermanentBonusUnit;

extern s32 datComputeSkillBoostedMaxHp(BtlPermanentBonusUnit *);
extern s32 datComputeSkillBoostedMaxMp(BtlPermanentBonusUnit *);

/* Apply a permanent stat/capacity item and refill eligible vitals.
 * Returns 0 for other items, 1 when accepted, or 2 when capped and already full. */
INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B300);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B350);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B370);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B3D0);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B440);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B4C0);

s32 btlItemApplyPermanentBonus(u16 item, BtlPermanentBonusUnit *unit) {
    s32 stat = -1;
    s32 valid = 0;

    switch (item - 0x59) {
    case 0:
        stat = 0;
        valid = 1;
        break;
    case 1:
        stat = 1;
        valid = 1;
        break;
    case 2:
        stat = 2;
        valid = 1;
        break;
    case 3:
        stat = 3;
        valid = 1;
        break;
    case 4:
        stat = 4;
        valid = 1;
        break;
    case 5:
        if (unit->maxHp >= 0x3E7 && unit->currentHp >= unit->maxHp &&
            unit->currentMp >= unit->maxMp) {
            return 2;
        }
        unit->hpBonus += 10;
        if (unit->hpBonus >= 0x3E8) {
            unit->hpBonus = 0x3E7;
        }
        valid = 1;
        break;
    case 6:
        if (unit->maxMp >= 0x3E7 && unit->currentHp >= unit->maxHp &&
            unit->currentMp >= unit->maxMp) {
            return 2;
        }
        unit->mpBonus += 10;
        if (unit->mpBonus >= 0x3E8) {
            unit->mpBonus = 0x3E7;
        }
        valid = 1;
        break;
    default:
        break;
    }

    if (valid == 0) {
        return 0;
    }
    if (stat >= 0) {
        if (unit->baseStats[stat] >= 0x63 &&
            unit->currentHp >= unit->maxHp && unit->currentMp >= unit->maxMp) {
            return 2;
        }
        unit->baseStats[stat] += 2;
        if (unit->baseStats[stat] >= 0x64) {
            unit->baseStats[stat] = 0x63;
        }
    }

    unit->maxHp = datComputeSkillBoostedMaxHp(unit);
    unit->maxMp = datComputeSkillBoostedMaxMp(unit);
    if ((unit->statusFlags & 0x4000) == 0) {
        unit->currentMp = unit->maxMp;
        unit->currentHp = unit->maxHp;
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

s32 mnuGetSelectionFromFlags(MenuSelectionEntry *entry) {
    u16 flags = entry->flags;
    if (flags & 0x400) return 0;
    if (flags & 0x100) return 1;
    if (flags & 0x80) return 2;
    if (flags & 0x40) return 3;
    if (flags & 0x10) return 4;
    return -1;
}

s32 mnuGetMatchingPartyEntryMask(s32 object) {
    s32 i;
    u8 *entry = datGameState + 0xA60;
    for (i = 0; i < 5; i++, entry += 0x1C4) {
        if ((((MenuPanelEntry *)entry)->flags & 1) &&
            ((MenuPanelEntry *)entry)->tableIndex == ((MenuPanelEntry *)object)->tableIndex) {
            return 1 << i;
        }
    }
    return 0;
}

s32 mnuClassifyQuarterHalfPercent(s32 amount, s32 divisor) {
    s32 percent;

    if (divisor != 0) {
        percent = amount * 100 / divisor;
        if (percent < 25) {
            return 2;
        }
        if (percent < 50) {
            return 1;
        }
    }
    return 0;
}

MenuPanelEntry *mnuPickBestPartyEntry(u32 *table) {
    MenuPanelEntry *best = NULL;
    MenuPanelEntry *entry;
    s32 pass;
    s32 bestWeight;
    s32 i;

    for (pass = 0; pass < 2; pass++) {
        bestWeight = 0;
        entry = (MenuPanelEntry *)(datGameState + 0xA60);
        for (i = 0; i < 5; i++, entry++) {
            if ((entry->flags & 1) && table[entry->tableIndex] == 0) {
                s32 usable = 0;

                if (!(entry->state & 0x10)) {
                    if (pass == 0) {
                        if (!(entry->flags & 2)) {
                            usable = 1;
                        }
                    } else if (entry->flags & 2) {
                        usable = 1;
                    }
                }

                if (usable && bestWeight < entry->weight) {
                    bestWeight = entry->weight;
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

MenuPanelEntry *mnuFindPartyEntryBySelection(s32 *out) {
    u16 table[5][2] = {{0x400, 0xD4}, {0x100, 0xCF}, {0x80, 0xA9}, {0x40, 0xCE}, {0x10, 0xA8}};
    u32 i;

    for (i = 0; i < 5; i++) {
        MenuPanelEntry *entry;
        s32 j;

        for (j = 0, entry = (MenuPanelEntry *)(datGameState + 0xA60); j < 5; j++, entry++) {
            if ((entry->flags & 1) && entry->state == table[i][0]) {
                *out = table[i][1];
                return entry;
            }
        }
    }
    return 0;
}

s32 mnuTryUseFieldSkill(s32 partyPanel, s32 skill, s32 target, s32 commit) {
    s32 id;
    MenuPanelEntry *entry = mnuFindPartyEntryBySelection(&id);

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
 * field-6/field-8 ratios without converting to floating point. */
s32 mnuComparePartyEntryCostRatio(u32 *left, u32 *right) {
    MenuItemCounts *a = (MenuItemCounts *)*left;
    MenuItemCounts *b = (MenuItemCounts *)*right;
    s32 leftRatio = (a->primary << 10) / a->denominator;
    s32 rightRatio = (b->primary << 10) / b->denominator;
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
    MenuPanelEntry *entry;
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
                used[entry->tableIndex] = 1;
                break;
            case 1:
                used[entry->tableIndex] = result;
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

/* Clamp the motion selector to the loaded model's available motions. */
void evtStageTestSetEntryIndex(s32 encodedIndex, s32 value) {
    s32 index = encodedIndex & 0xFFFF;

    if (value < 0) {
        value = 0;
    }
    if (evtStageTestState.model != 0 && value >= mdlGetNodeRefHalf(evtStageTestState.model, 0)) {
        value = mdlGetNodeRefHalf(evtStageTestState.model, 0) - 1;
    }
    evtStageTestState.entries[index].motionIndex = value;
    func_002C6E20(-1);
}

/* Advance the selected entry's animation frame and request a stage refresh. */
void evtStageTestAddEntryValue(s32 encodedIndex, f32 delta) {
    s32 index = encodedIndex & 0xFFFF;
    StageTestEntry *entry;

    if (delta < 0.0f && ((StageTestEntry *)(index * 60 + (s32)evtStageTestState.entries))->frame - delta < 0.0f) {
        return;
    }
    entry = (StageTestEntry *)(index * 60 + (s32)evtStageTestState.entries);
    entry->frame += delta;
    func_002C6E20(-1);
}

void mnuOffsetPanelPosition(s32 index, s32 dx, s32 dy, s32 dz) {
    s32 offset = (index & 0xFFFF) * sizeof(StageTestEntry);
    StageTestEntry *entry = (StageTestEntry *)(offset + (s32)evtStageTestState.entries);
    f32 x = entry->position[0] + (f32)dx;
    f32 y = entry->position[1] + (f32)dy;
    f32 z = entry->position[2] + (f32)dz;

    entry->position[0] = x;
    entry->position[1] = y;
    entry->position[2] = z;
}

void mnuOffsetPanelTarget(s32 index, s32 dx, s32 dy, s32 dz) {
    s32 offset = (index & 0xFFFF) * sizeof(StageTestEntry);
    StageTestEntry *entry = (StageTestEntry *)(offset + (s32)evtStageTestState.entries);
    f32 x = entry->rotation[0] + (f32)dx;
    f32 y = entry->rotation[1] + (f32)dy;
    f32 z = entry->rotation[2] + (f32)dz;

    entry->rotation[0] = x;
    entry->rotation[1] = y;
    entry->rotation[2] = z;
}

u8 func_002C66D0(u16 index) {
    return evtStageTestState.entries[index].motionIndex;
}

f32 func_002C66F8(u16 index) {
    return evtStageTestState.entries[index].frame;
}

void func_002C6720(s32 index, f32 *out) {
    out[0] = evtStageTestState.entries[index & 0xFFFF].position[0];
    out[1] = evtStageTestState.entries[index & 0xFFFF].position[1];
    out[2] = evtStageTestState.entries[index & 0xFFFF].position[2];
}

void func_002C6758(s32 index, f32 *out) {
    out[0] = evtStageTestState.entries[index & 0xFFFF].rotation[0];
    out[1] = evtStageTestState.entries[index & 0xFFFF].rotation[1];
    out[2] = evtStageTestState.entries[index & 0xFFFF].rotation[2];
}

extern s32 kwlnGetDrawBufferIndex(void);
extern void sdfCameraBuildProjection(void *);
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
extern void func_00107EF8(s32 mode, s32 slot, f32 *color);
extern void func_00107FF8(s32 mode, s32 slot, f32 *color);
extern void kwlnSetBackgroundColorTarget(s32 mode, f32 *color);
extern void kwlnSetDrawColorTarget(s32 mode, f32 *color);
extern void evtSetDrawVectorTarget(s32 mode, f32 x, f32 y, f32 z, f32 w);

void func_002C6790(void)
{
    f32 firstColor[4];
    f32 secondColor[4];
    f32 firstVector[4];
    f32 secondVector[4];

    memcpy(firstColor, D_0042B528, sizeof(firstColor));
    memcpy(secondColor, D_0042B538, sizeof(secondColor));
    memcpy(firstVector, D_0042B548, sizeof(firstVector));
    memcpy(secondVector, D_0042B558, sizeof(secondVector));
    evtToggleSavedDrawVectors(0, 5.0f, 0.0f);
    func_00107EF8(0, 0, firstColor);
    func_00107FF8(0, 0, secondColor);
    kwlnSetBackgroundColorTarget(0, firstVector);
    kwlnSetDrawColorTarget(0, secondVector);
    evtSetDrawVectorTarget(0, 255.0f, 255.0f, 2000.0f, 30000.0f);
    sdfSceneProjectionParameters[3] = 0.7551905f;
    PCP_COPY_VECTOR(sdfViewTargetVector, D_003E7950);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_003E7940);
    PCP_COPY_VECTOR(sdfViewUpVector, D_003E7960);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfConsCacheTransformedNode(sdfSceneProjectionParameters, sdfViewMatrix);
}

void func_002C6958(f32 value) {
    sdfSceneProjectionParameters[5] = 2048.0f;
    sdfSceneProjectionParameters[4] = value + 2048.0f;
    func_002C6790();
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
    func_002C6958(offset);
}

void evtStageTestStop(void) {
    StageTestState *state = &evtStageTestState;

    if (state->mode == 1) {
        return;
    }
    state->pendingEffect = -1;
    if (state->effect != 0) {
        evtStageTestDestroyModelEffect();
    }
    if (state->model != 0) {
        mdlDestroyContext(state->model);
        state->model = 0;
        state->modelUpdateStarted = 0;
    }
}

void mnuResetWorkFloats(void) {
    evtStageTestStop();
    sdfSceneProjectionParameters[4] = 2048.0f;
    sdfSceneProjectionParameters[5] = 2048.0f;
}

/* Remember the model asset request result for the stage viewer. */
s32 evtStageTestRequestModelAsset(s32 resource, s32 modelId, s32 option) {
    evtStageTestState.assetRequest = mdlRequestAsset(resource, modelId, option);
    return evtStageTestState.assetRequest;
}

void mnuForwardTableByte(s32 encodedIndex) {
    StageTestSlot *slot = evtStageTestState.queue.slot;

    evtStageTestRequestModelAsset(slot->assetResource, evtStageTestState.entries[encodedIndex & 0xffff].modelId, 0);
}

u32 func_002C6B28(u32 *flags) {
    return *flags & 1;
}

/* Promote the staged selection and clear its pending flag. Returns 1 only
 * when a slot was committed; the poller supplies &evtStageTestState.queue. */
s32 mnuCommitPendingBlock(StageTestQueue *queue) {
    if (!(queue->flags & 1)) {
        return 0;
    }
    queue->slot[0] = queue->slot[1];
    queue->flags &= ~1;
    return 1;
}

void evtStageTestClearPendingFlag(void) {
    evtStageTestState.queue.flags &= ~1;
}

s32 evtStageTestSelectEntry(s32 encodedIndex, s32 initialMotionIndex, s32 assetOption) {
    s32 index = encodedIndex & 0xFFFF;
    s32 bank;
    StageTestSlot *slot;

    if (evtStageTestState.model != 0 && evtStageTestState.queue.slot[0].modelId == evtStageTestState.entries[index].modelId) {
        return 0;
    }
    evtStageTestStop();
    bank = 0;
    if (evtStageTestState.queue.flags & 2) {
        bank = 1;
    }
    evtStageTestState.queue.flags |= 2;
    evtStageTestState.queue.flags &= ~4;
    if (bank) {
        evtStageTestState.queue.flags |= 1;
    }
    slot = &evtStageTestState.queue.slot[bank];
    slot->entryIndex = index;
    slot->modelId = evtStageTestState.entries[index].modelId;
    slot->assetOption = assetOption;
    slot->initialMotionIndex = initialMotionIndex;
    slot->state = 0;
    return 1;
}

void evtStageTestSelectEntryWithoutInitialValue(u16 id, u32 option) {
    evtStageTestSelectEntry(id, 0xffffffffffffffff, option);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6CE8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6E20);



extern s32 D_00435DF0;
extern void mdlStoreTertiaryVectorVU(s32);

f32 mnuSetModelScaleVector(s32 model, s32 useTable) {
    f32 scale = 1.0f;
    f32 vec[4];

    if (useTable != 0) {
        scale = *(f32 *)(D_00435DF0 + evtStageTestState.queue.slot[0].modelId * 0x270 + 0x10);
    }
    vec[0] = scale;
    vec[1] = scale;
    vec[2] = scale;
    vec[3] = 1.0f;
    VU0_LOAD_VF(vf10, vec);
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

void mnuResetWorkPair(void) {
    ((MenuWorkCamera *)D_003E7950)->x = 0;
    ((MenuWorkCamera *)D_003E7950)->y = 0;
    ((MenuWorkCamera *)D_003E7950)->depth = -400.0f;
    ((MenuWorkPosition *)D_003E7940)->x = 0;
    ((MenuWorkPosition *)D_003E7940)->y = 0;
    ((MenuWorkPosition *)D_003E7940)->z = 0;
}

extern void mdlStorePrimaryVectorVU(s32);

void mnuApplyModelCamera(s32 model) {
    f32 vec[4];
    StageTestEntry *entry;
    f32 scale;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    entry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries);
    vec[0] = entry->position[0];
    vec[1] = entry->position[1];
    if (evtStageTestState.flag != 1) {
        mnuSetModelScaleVector(model, 0);
        vec[2] = ((StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries))->position[2];
        mnuResetWorkPair();
    } else {
        scale = mnuSetModelScaleVector(model, 1);
        entry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries);
        vec[0] -= entry->position[0] - entry->position[0] * scale;
        vec[1] -= entry->position[1] - entry->position[1] * scale;
        vec[2] = 0.0f;
        ((MenuWorkCamera *)D_003E7950)->depth = (-400.0f - entry->position[2]) * scale;
    }
    VU0_LOAD_VF(vf10, vec);
    mdlStorePrimaryVectorVU(model);
}

void evtStageTestApplyEntryRotation(s32 model) {
    StageTestEntry *entry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries);

    func_00340DC8(entry->rotation[0] * 3.14159265f / 180.0f, entry->rotation[1] * 3.14159265f / 180.0f,
                  entry->rotation[2] * 3.14159265f / 180.0f);
    mdlUpdateContextRotationBasisFromQuaternion(model);
}


/* vu0 routine: copies the stage-test camera vectors into the view work area, builds the look-at basis for eye 600 units along the view direction, and hands the matrix to the model packet at the current slot */
void evtStageTestUpdateCamera(void)
{
    s128 eye;
    s128 at;
    s32 slot;

    slot = kwlnGetDrawBufferIndex();
    func_002C6790();
    PCP_COPY_VECTOR(sdfViewTargetVector, D_003E7950);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_003E7940);
    PCP_COPY_VECTOR(sdfViewUpVector, D_003E7960);
    sdfCameraBuildProjection(sdfSceneProjectionParameters);
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
    VU0_STORE_VF_UNCLOBBERED(vf10, &at);
    sdfVuBuildLookAtBasis(&eye, &at, sdfViewUpVector);
    VU0_STORE_MATRIX_UNCLOBBERED(sdfViewMatrix);
    sdfConsBuildMatrixPacket(D_003820F0 + slot * 8000, sdfSceneProjectionParameters, sdfViewMatrix);
    sdfConsCacheTransformedNode(sdfSceneProjectionParameters, sdfViewMatrix);
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

s32 evtStageTestCountFlags(s32 mode) {
    StageTestSlot *slot = evtStageTestState.queue.slot;
    s32 index = slot->entryIndex;
    s32 count = 0;
    u32 i;

    for (i = 0; i < 8; i++) {
        u8 flag;

        if (mode == 0) {
            flag = *(evtStageTestState.entries[index].column[0] + i);
        } else {
            flag = *(evtStageTestState.entries[index].column[1] + i);
        }
        if (flag) {
            count++;
        }
    }
    return count;
}

/* Queue a motion from the selected entry's column, with a 15-frame blend. */
void evtStageTestQueueMotion(s32 kind, u32 index) {
    StageTestSlot *slot = evtStageTestState.queue.slot;
    f32 blendLeadFrames = 0.0f;
    s32 motionIndex;

    if (index < 8) {
        switch (kind) {
        default:
            blendLeadFrames = 15.0f;
            motionIndex = *(evtStageTestState.entries[slot->entryIndex].column[0] + index);
            break;
        case 1:
            motionIndex = *(evtStageTestState.entries[slot->entryIndex].column[1] + index);
            slot->flags &= ~1;
            break;
        case 2:
            motionIndex = *(evtStageTestState.entries[slot->entryIndex].column[1] + index);
            slot->flags |= 1;
            break;
        }
        evtStageTestQueueMotionSegment(motionIndex, blendLeadFrames, 15.0f);
    }
}

/* Queue a motion and its blend timing; inputs are truncated to whole frames.
 * The lead shifts initial motion time backwards, not to a start-frame endpoint. */
void evtStageTestQueueMotionSegment(u32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames) {
    StageTestSlot *slot = evtStageTestState.queue.slot;

    slot->state = 1;
    slot->motionIndex = motionIndex;
    slot->blendLeadFrames = (s32)blendLeadFrames;
    slot->blendDurationFrames = (s32)blendDurationFrames;
}

void func_002C7530(void) {
    evtStageTestState.queue.slot[0].state = 4;
}

s32 evtStageTestHasPendingMotion(void) {
    s32 state = evtStageTestState.queue.slot[0].state;

    if ((state == 0) || (state == 3)) {
        return 0;
    }
    return 1;
}

/* Play the queued motion once, then start the entry's fallback unless suppressed.
 * State 4 also permits the fallback before the active motion finishes. */
void evtStageTestAdvanceMotionQueue(void) {
    StageTestSlot *slot = evtStageTestState.queue.slot;
    s32 index;
    s32 node;

    if (slot->state != 0 && slot->state != 3 && (node = evtStageTestGetActiveModel()) != 0) {
        if (slot->state == 1) {
            index = slot->motionIndex;

            if (index < mdlGetNodeRefHalf(node, 0)) {
                mdlAddEntryPlainEx(node, 0, index, (s32)slot->blendLeadFrames, (s32)slot->blendDurationFrames);
                slot->state = 2;
            }
        } else if (!(slot->flags & 1) && (*(u8 *)(*(s32 *)(node + 0x1C) + 0x30) == 5 || slot->state == 4)) {
            index = evtStageTestState.entries[slot->entryIndex].motionIndex;

            if (index < mdlGetNodeRefHalf(node, 0)) {
                mdlAddEntryFlaggedEx(node, 0, index, (s32)slot->blendLeadFrames, (s32)slot->blendDurationFrames);
                slot->state = 3;
            }
        }
    }
}

void evtStageTestCreateModelEffect(s32 unused) {
    if (evtStageTestState.effect != 0) {
        evtStageTestDestroyModelEffect();
    }
    evtStageTestState.effect = sndCreateSystemEffectHandle(evtStageTestState.model, 0x30);
}

void evtStageTestDestroyModelEffect(void) {
    StageTestState *stage = &evtStageTestState;
    u32 effectHandle = stage->effect;

    if (effectHandle == 0) {
        return;
    }
    sndDestroyFileQueueWrapper(effectHandle);
    stage->effect = 0;
}

void evtStageTestSetPendingEffect(u32 effect) {
    evtStageTestState.pendingEffect = effect;
}

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

void *evtCreateBattleStageTestCamera(void) {
    f32 position[4] = {401.0f, -593.0f, -1208.25f, 0.0f};
    f32 orientation[4] = {0.22f, 0.12f, 0.03f, 1.0f};
    StageCameraTarget *target = evtCreateWorldObjectAtTransform(position, orientation);

    target->unk8 = D_00437CB0;
    return func_002C79B8;
}

void *evtBattleStageTestScreen(void) {
    void *packets = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(packets);
    kwlnDrawSpriteCell(packets, 0x84, 0x46, 0x14, 9);
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x7BA0, 0xFEFFFF, 0, "BATTLE STAGE"));
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7A80, 0x7C60, 0xFEFFFF, 6, "F%03d_%03d", D_00437CB8, D_00437CBC));
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R = EVENT SELECT"));
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7900, 0x7D80, 0xFEFFFF, 0, "RR  = ENTER"));
    D_00380708.invoke(&D_00380708, packets);
    if (D_0037F510[0x21] < 0) {
        evtCreateWorldObjectForKey(D_00437CB8, D_00437CBC);
        return evtCreateBattleStageTestCamera;
    }
    if (D_0037F510[0x25] & 2) {
        D_00437CB8++;
    }
    if (D_0037F510[0x24] & 2) {
        D_00437CB8--;
    }
    if (D_0037F510[0x2A] & 2) {
        D_00437CBC++;
    }
    if (D_0037F510[0x28] & 2) {
        D_00437CBC--;
    }
    return 0;
}

void evtDestroyBattleStageTestWorldNode(void) {
    evtDestroySecondaryWorldNode();
}

void evtBattleStageTestStopTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0042B610, 1);
    kwlnDebugGraphSetEnabled(0);
}


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

s32 btlDestroyStageTask(object)
    StageTestTaskWork *object;
{
    if (object->kind == 6) {
        s32 resource = object->resource;
        if (resource != 0) {
            sdfDevQueueReleaseState(resource);
        }
        func_00346AF8(object->payload);
        sdfReleaseChipBlock(object->allocation);
        sdfReleaseChipBlock((void *)object);
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

