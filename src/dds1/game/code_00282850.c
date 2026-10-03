#include "common.h"
#include "pcp_vu0.h"
#include "mnu.h"

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
    u8 pad00[6];
    u16 primary;     /* 0x06 */
    u16 denominator; /* 0x08 */
    u16 secondary;   /* 0x0A */
} MenuItemCounts;





extern void mdlAddEntryFlaggedEx(s32, s32, s32, f32, f32);
extern void mdlAddEntryPlainEx(s32, s32, s32, f32, f32);
extern void evtStageTestQueueMotionSegment(s32, f32, f32);














extern void func_00284108(s32, s32, s32, s32, s32, s32, s32 *);


extern void evtStageTestCreateModelEffect(s32);
extern void btlUpdateJobPositionFromModel(s32);
extern void mdlProcessContextNodesAndTransforms(s32, s32);
extern void mnuApplyModelCamera(s32);
extern void evtStageTestApplyEntryRotation(s32);
extern void evtStageTestUpdateCamera(void);
extern s32 kwlnGetDrawBufferIndex(void);
extern void sdfCameraBuildProjection(void *);
extern void sdfConsBuildMatrixPacket(void *packet, void *node, void *matrix);
extern void sdfConsCacheTransformedNode(void *node, void *matrix);
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern u8 sdfViewTargetVector[];
extern u8 sdfViewEyeVector[];
extern u8 sdfViewUpVector[];
extern u8 D_00324590[];
extern u8 D_0037CE80[];
extern u8 sdfViewMatrix[];
extern u8 D_003270F0[];
extern void evtStageTestAdvanceMotionQueue(void);
extern void func_00281780(s32, s32, s32, s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u8 fldIsFlagActive(void);
extern u8 fldTestSecondarySceneFlag(void);
extern void fldSetPrimarySceneFlag(void);
extern void fldSetSecondarySceneFlag(void);
extern s32 mdlRequestAsset(s32, s32, s32);

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
extern s32 datGameState;
extern void func_00285960(u8 *entry, s32 arg1, u32 index, PartyPanel *panel);

typedef struct AffinityRow {
    s32 affinity[4];
} AffinityRow;

extern void func_002E7F20(f32, f32, f32);
extern void mdlUpdateContextRotationBasisFromQuaternion(s32);

typedef struct StageCameraTarget {
    u8 pad[8];
    void *unk8;
} StageCameraTarget;
extern StageCameraTarget *evtCreateWorldObjectAtTransform(f32 *, f32 *);
extern u8 D_003BC7C8[];

extern s32 mdlGetNodeRefHalf(u32 node, s32 index);

extern void func_002878D8(s32 arg0);

extern s32 D_003BC7B4;

extern s32 datCommandSelectors;

extern s32 datCommandRecords;

extern u32 effCreateStatusBatch(u32);

extern s32 mnuLookupRangeEntry(u16);

extern u32 func_0027D4A0(u32);

extern s32 sdfAllocSizeClassBlock(u32);

extern s32 func_002877A8(void);

typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
    s32 children[5]; /* 0x0C */
    u32 selection;    /* 0x20 */
    s32 initialValue; /* 0x24: initialized to 0x100 */
} MenuPanelGroup;

extern void mnuClearPanelGroupSelection(MenuPanelGroup *);

extern char D_003B2608[]; /* "battle stage test" */

extern u8 D_0037CE60[];

extern u8 D_0037CE70[];

extern f32 sdfSceneProjectionParameters[];

/* Battle stage test viewer state. Retail addresses it partly through
 * D_003DC600 (= &evtStageTestState.queue.slot[0], hence the negative offsets), so the
 * D_003DC5EC..D_003DC654 symbols are all interior fields of this one object. */
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

typedef struct StageTestState {
    s32 mode;                /* 0x00 */
    s32 assetRequest;        /* 0x04: result of mdlRequestAsset */
    s32 model;               /* 0x08 */
    s8 flag;                 /* 0x0C */
    StageTestEntry *entries; /* 0x10 */
    StageTestQueue queue;    /* 0x14: flags followed by the two selection slots */
    s32 effect;              /* 0x68 */
    s32 pendingEffect;       /* 0x6C */
} StageTestState;

extern StageTestState evtStageTestState;

extern void btlStopStage(void);

extern void *evtBattleStageTestScreen(void);

typedef struct MenuStageTestState {
    u32 flags;
    u8 pad04[4];
    s32 *layout;       /* 0x08: two counts, summed for the list length */
    u8 pad0C[0x678];
    s32 selectedPanel; /* 0x684 */
    s32 scrollOffset;  /* 0x688 */
} MenuStageTestState;

typedef struct MenuStagePanelHeader {
    s32 mode;  /* 0x00 */
    u32 flags; /* 0x04 */
    u8 pad08[0x12C];
} MenuStagePanelHeader; /* 0x134 */

extern void func_00281D40(s32, s32, s32, MenuStageTestState *, s32, s32);
extern void func_00282360(s32, s32, s32, MenuStageTestState *, s32, s32);

typedef struct MenuStageLayoutCounts {
    s32 first;
    s32 second;
} MenuStageLayoutCounts;

extern s32 D_0037CD80[];

void mnuDispatchListPanel(s32 x, s32 y, s32 z, MenuStageTestState *menu, s32 panelIndex, s32 param) {
    MenuStagePanelHeader *panel = (MenuStagePanelHeader *)((u8 *)menu + 0x78) + panelIndex;
    s32 mode = panel->mode;

    if (menu->selectedPanel >= 0) {
        x -= 0x270;
        y += 0x20;
        mode = 1;
    }
    if (panel->flags & 0x80) {
        mode = 1;
    }
    switch (mode) {
    case 1:
        func_00281D40(x, y, z, menu, panelIndex, param);
        return;
    case 2:
        func_00282360(x, y, z, menu, panelIndex, param);
        break;
    }
}

void func_002828D0(s32 *position, MenuStageTestState *menu, s32 panelIndex) {
    MenuStagePanelHeader *panel =
        (MenuStagePanelHeader *)((u8 *)menu + 0x78) + panelIndex;
    MenuStageLayoutCounts *layout = (MenuStageLayoutCounts *)menu->layout;
    u32 panelFlags = panel->flags;
    s32 secondCount = layout->second;
    s32 totalCount;
    s32 firstCount = layout->first;
    s32 group;

    totalCount = firstCount + secondCount;

    if (!(panelFlags & 0x80)) {
        group = panelIndex < firstCount ? 1 : 2;
    } else {
        group = 1;
    }

    switch (group) {
    case 1:
        position[1] = panelIndex * 0x320 + 0xA8;
        position[0] = 0xEF0;
        if (panelIndex == 1) {
            position[0] = 0xE50;
        }
        break;
    case 2: {
        s32 entryIndex = ((firstCount * 5 + totalCount) << 1) - 12;
        position[1] =
            (D_0037CD80[entryIndex] +
             (panelIndex - firstCount) * *(entryIndex + D_0037CD80 + 1))
            << 3;
        position[0] = 0x1060;
        goto done;
    }
    default:
        goto done;
    }
done:
    return;
}

/* Advance the panel's current transition value toward its 0x100 limit. */
void mnuAdvancePanelTransition(s32 panel) {
    if (*(s32 *)(panel + 4) < 0x100) {
        *(s32 *)(panel + 4) = *(s32 *)(panel + 4) + 8;
    }
}



typedef struct MenuStageNode {
    u8 pad00[0xC];
    s32 overrideValue;
} MenuStageNode;

typedef struct MenuStagePanel {
    u8 pad00[0xE0];
    MenuStageNode *node;
} MenuStagePanel;

/* Apply a temporary override to the selected node while drawing its panel. */
void mnuDrawPanelWithTemporaryOverride(s32 x, s32 y, s32 z, s32 overrideValue, MenuStageTestState *menu, s32 param) {
    s32 positionOffset[2];
    MenuStagePanel *panel = (MenuStagePanel *)((s32)menu + menu->selectedPanel * 0x134 + 0x78);
    MenuStageNode *node;

    func_002828D0(positionOffset, menu, 0);
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = overrideValue;
    }
    x += menu->scrollOffset * 0x10;
    menu->scrollOffset = (s32)((f32)menu->scrollOffset / 1.19999993f);
    /* Both arms are identical in retail; kept as written. */
    if (menu->flags & 0x100) {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selectedPanel, param);
    } else {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selectedPanel, param);
    }
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = 0;
    }
}

void mnuDrawStageTestList(s32 x, s32 y, s32 z, s32 overrideValue, MenuStageTestState *menu, s32 param) {
    s32 positionOffset[2];
    s32 *layout = menu->layout;
    s32 count;
    s32 i;

    count = layout[0];
    count += layout[1];
    if (menu->selectedPanel >= 0) {
        mnuDrawPanelWithTemporaryOverride(x, y, z, overrideValue, menu, param);
    } else {
        for (i = 0; i < count; i++) {
            func_002828D0(positionOffset, menu, i);
            mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, i, param);
        }
    }
    mnuAdvancePanelTransition((s32)menu);
}

/* Same as mnuDrawStageTestList with no override value; arg5 is unused. */
void mnuDrawPanelListDefault(s32 x, s32 y, s32 z, MenuStageTestState *menu, s32 param, s32 unused) {
    mnuDrawStageTestList(x, y, z, 0, menu, param);
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
    u32 secondValueA; /* 0x28 */
    u32 secondValueB; /* 0x2C */
    MenuPoint secondPosition; /* 0x30 */
    MenuPoint thirdPosition; /* 0x38 */
    u32 thirdValueA; /* 0x40 */
    u32 thirdValueB; /* 0x44 */
    MenuPoint fourthPosition; /* 0x48 */
    u32 fourthValueA; /* 0x50 */
    u32 fourthValueB; /* 0x54 */
    u32 fourthValueC; /* 0x58 */
    u8 pad5C[4];
    u32 resourceHandle; /* 0x60 */
} MenuPanelState;

void *mnuCreatePanelState(s32 width, s32 height) {
    MenuPanelState *panel = sdfAllocSizeClassBlock(0x64);
    memset(panel, 0, 0x64);
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

void func_00282CA8(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->firstValueA = valueA;
    panel->firstValueB = valueB;
    itfGridStorePosition(&panel->firstPosition, x, y);
}

void func_00282CD0(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->secondValueA = valueA;
    panel->secondValueB = valueB;
    itfGridStorePosition(&panel->secondPosition, x, y);
}

void mnuInitializePanelResource(MenuPanelState *panel) {
    u32 resourceHandle;

    resourceHandle = func_0027D4A0(2);
    panel->resourceHandle = resourceHandle;
}

void func_00282D28(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->thirdValueA = valueA;
    panel->thirdValueB = valueB;
    itfGridStorePosition(&panel->thirdPosition, x, y);
}

void func_00282D50(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y, u32 additionalValue) {
    panel->fourthValueA = valueA;
    panel->fourthValueB = valueB;
    itfGridStorePosition(&panel->fourthPosition, x, y);
    panel->fourthValueC = additionalValue;
}

void mnuSetPanelState(MenuPanelState *panel, u32 state) {
    panel->state = state;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00282DA0);

s32 mnuCreatePanelGroup(s32 parent) {
    MenuPanelGroup *group = sdfAllocSizeClassBlock(0x28);
    s32 i;
    for (i = 0; i < 5; i++) {
        s32 child = mnuCreatePanelItem();
        func_002848E0(child, parent, i);
        group->children[i] = child;
    }
    mnuClearPanelGroupSelection(group);
    group->initialValue = 0x100;
    return (s32)group;
}

void mnuDestroyPanelGroup(MenuPanelGroup *group) {
    s32 i;
    for (i = 0; i < 5; i++) {
        mnuFreePanelItemWork(group->children[i]);
    }
    sdfReleaseChipBlock(group);
}

extern void mnuPositionPanelItemPoints(s32, s32, s32);
void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 data) {
    s32 i;
    for (i = 0; i < 5; i++) {
        mnuPositionPanelItemPoints(group->children[i], data, i);
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00283110);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 value, s32 option) {
    /* Required to match: retain byte-offset indexing for the child slot. */
    s32 offset = index * 4;
    s32 base = (s32)group + 0xC;
    s32 *item = (s32 *)(base + offset);
    mnuSetPanelItemSelection(*item, value);
    mnuSetPanelItemOption(*item, option);
}

typedef struct MenuSpriteState {
    u8 pad00[0x10];
    s32 x;
    s32 y;
    s32 z;
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} MenuSpriteState;

void *mnuCreateSpriteState(s32 x, s32 y, s32 z) {
    MenuSpriteState *item = sdfAllocSizeClassBlock(0x20);
    memset(item, 0, 0x20);
    item->x = x;
    item->y = y;
    item->z = z;
    item->initialValue = 0x100;
    return item;
}

void mnuFreeSpriteStateWork(void) {
    sdfReleaseChipBlock();
}

/* Select the range entry's sprite variant before submitting its draw request. */
void mnuDrawRangeSpriteVariant(u32 x, u32 y, u32 depth, u32 color,
                                    u16 rangeId, s32 alternate, u32 drawArg, u32 texture) {
    s32 rangeIndex;
    s32 variant;

    variant = 0x11;
    if (alternate != 0) {
        variant = 0x12;
    }
    rangeIndex = mnuLookupRangeEntry(rangeId);
    func_002BF4E0(x, y, depth, color, 1, drawArg, rangeIndex * 2 + variant, texture);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002833B0);

u32 *mnuAllocateSimpleSprite(u32 x, u32 y, u32 z, u32 color, u32 texture) {
    u32 *sprite = sdfAllocSizeClassBlock(0x28);
    memset(sprite, 0, 0x28);
    sprite[4] = x;
    sprite[5] = y;
    sprite[6] = z;
    sprite[7] = color;
    sprite[8] = texture;
    sprite[9] = 0x100;
    return sprite;
}

void mnuFreeSimpleSpriteWork(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283838);

typedef struct MenuGradientFade {
    s32 active;
    s32 color;
    s32 blend;
} MenuGradientFade;

void mnuResetGradientFadeColor(MenuGradientFade *state, s32 color) {
    state->color = color;
    state->active = 0;
    state->blend = 0;
}

void func_00283BF0(u32 *out, u32 value) {
    *out = value;
}

extern u32 uiBlendColors(u32, u32, u32);
extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, u32, u32);

void mnuDrawAndStepGradientFade(MenuGradientFade *state, s32 surface) {
    s32 colors[4];
    s32 color = state->color;

    color = uiBlendColors(color, color & ~0xFF, state->blend);
    panelSetVec4((u32 *)colors, 0, 0, color, color);

    uiDrawGradientColorRect(0, 0x700, 0, 0x2000, 0x700, (u32)colors, surface);
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
    u8 pad1A[0x1E];
    s32 configurationHandle; /* 0x38 */
    u8 pad3C[0x04];
    MenuEffectNode *first;  /* 0x40 */
    MenuEffectNode *second; /* 0x44 */
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

s32 mnuRateByThreshold(s32 object) {
    s32 value = *(s32 *)(object + 0x10);

    if (value < 0x32) {
        return (value >= 0x14) ? 1 : 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283D10);

/* Cycle through four indexed settings while refreshing the paired effects. */
void mnuCyclePairedEffectSetting(MenuEffectPair *pair) {
    s32 *settings;
    s32 setting;

    func_00283D10(pair);
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00283EE0);

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
        effDestroyPackedBatch(list[i + 16]);
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00284108);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2420);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2430);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2450);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2468);

void mnuDrawPanelSequenceByRow(s32 x, s32 y, s32 depth, s32 color, s32 variant, s32 texture) {
    s32 tableA[9] = {6, 3, 0xE, 0xE, 5, 4, 0xC, 0xA, 8};
    s32 tableB[9] = {6, 0, 0xD, 0xD, 2, 1, 0xB, 9, 7};

    if (y == 1) {
        func_00284108(x, y, depth, color, variant, texture, tableA);
    } else {
        func_00284108(x, y, depth, color, variant, texture, tableB);
    }
}

void mnuReleaseSpriteTextures(s32 *object) {
    u32 i;
    for (i = 0; i < 9; i++) {
        effDestroyResourceSlotSet(object[i + 7]);
    }
    mnuReleasePairedEffectBatches(object);
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00284418);

INCLUDE_ASM(const s32, "game/code_00282850", func_002845F8);

void func_00284880(void) {
}

typedef struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    u8 pad24[0x38];
    MenuPoint points[5]; /* 0x5C */
    u8 pad84[4];
    u32 initialValue; /* 0x88 */
    u32 selectionRamp; /* 0x8C */
} MenuPanelItem;

s32 mnuCreatePanelItem(void) {
    MenuPanelItem *item = sdfAllocSizeClassBlock(0x90);
    memset(item, 0, 0x90);
    item->value14 = 0x63;
    item->value10 = 0x8c;
    item->initialValue = 0x100;
    return (s32)item;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002848E0);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B24D0);

void mnuPositionPanelItemPoints(s32 obj, s32 param, s32 index) {
    s32 table[5] = {0, 4, 1, 2, 3};
    MenuPanelItem *item = (MenuPanelItem *)obj;

    itfGridStorePosition(&item->points[0], param, 7);
    itfSetGridEntryQuantizedAndRefresh(item->points[0].x, item->points[0].y, 0, 0, 0, 0);
    itfGridStorePosition(&item->points[1], param, 5);
    itfSetGridEntryQuantizedAndRefresh(item->points[1].x, item->points[1].y, 0x440, 0x58, 0, 0);
    itfGridStorePosition(&item->points[2], param, 6);
    itfSetGridEntryQuantizedAndRefresh(item->points[2].x, item->points[2].y, 0x440, 0x58, 0, 0);
    itfGridStorePosition(&item->points[3], param, 9);
    itfSetGridEntryQuantizedAndRefresh(item->points[3].x, item->points[3].y, 0x620, 0x50, 0, 0);
    itfGridStorePosition(&item->points[4], param, table[index]);
    itfSetGridEntryQuantizedAndRefresh(item->points[4].x, item->points[4].y, 0x100, -0x48, 0, 0);
}

void mnuStorePanelItemValue(MenuPanelItem *item, u32 value) {
    item->value10 = value;
}

void func_00284C00(MenuPanelItem *item, s32 value, s32 option) {
    item->value18 = value;
    item->option = option;
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

void mnuFreePanelItemWork(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00284C48);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284EB8);

/* The compact 0x3C-byte profile panel stores a cap, option and initial grid point. */
typedef struct MenuProfilePanel {
    u8 pad00[0x10];
    s32 capValue;
    s32 option;
    MenuPoint gridOrigin;
    u8 pad20[0x1C];
} MenuProfilePanel;

void mnuSetProfilePanelValues(s32 item, s32 value, s32 option) {
    ((MenuProfilePanel *)item)->capValue = value;
    ((MenuProfilePanel *)item)->option = option;
}

u32 *mnuCreateProfilePanel(s32 source) {
    u32 *item = (u32 *)sdfAllocSizeClassBlock(0x3c);
    s32 first;
    u32 second;
    memset(item, 0, 0x3c);
    first = scrGetSelectedOperandIndex(source);
    second = ptyGetCurrentProfileRecord(source);
    mnuSetProfilePanelValues(item, prfGetCapValue((u16)first), *(u32 *)second);
    item[14] = 0x100;
    return item;
}

void mnuFreeProfilePanelWork(void) {
    sdfReleaseChipBlock();
}

void mnuCacheProfilePanelGridPositions(s32 item, u32 grid, u32 unused, u32 firstIndex,
                                    u32 secondIndex) {
    itfGridStorePosition((u32 *)(item + 0x18));
    itfSetGridEntryQuantizedAndRefresh(((MenuProfilePanel *)item)->gridOrigin.x, ((MenuProfilePanel *)item)->gridOrigin.y, 0, 0, 0, 0);
    itfGridStorePosition(item + 0x20, grid, firstIndex);
    itfGridStorePosition(item + 0x28, grid, secondIndex);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285208);

void mnuDrawAndAdvanceProfilePanel(s32 x, s32 y, s32 z, u32 *item, s32 option) {
    s32 previous;
    s32 next;
    func_00285208(x, y, z, item, option);
    previous = item[13];
    next = previous + 12;
    if (previous < 0x200) {
        item[13] = next;
        if (next < 0x200) {
            return;
        }
        previous = next;
    }
    item[13] = previous - 0x200;
}

void mnuClearPanelTransitionState(u32 item) {
    memset(item, 0, 0x4c);
}

typedef u32 (*MenuPopupCallback)();

typedef struct MenuPopupEntry {
    u32 flags;
    MenuPopupCallback enter;
    MenuPopupCallback leave;
    MenuPopupCallback start;
    MenuPopupCallback update;
    MenuPopupCallback finish;
    MenuPopupCallback canEnter;
} MenuPopupEntry;

typedef struct MenuPopupState {
    s32 count;
    MenuPopupEntry *entries[16];
    s32 entryAddress;
    s32 lastEntryAddress;
} MenuPopupState;

void func_002854B0(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument) {
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

void mnuDrainPanelTransitions(u32 item, u32 option) {
    s32 currentValue;

    currentValue = *(s32 *)item;
    while (currentValue != 0) {
        func_002854B0(1, 0, item, option);
        currentValue = *(s32 *)item;
    }
}

s32 mnuHasPopupSelectionFlag(s32 *flags) {
    return (*flags & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285670);

/* Popup state retains the entry address at +0x44. */

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00285960);

void mnuInitPartyPanelSlots(PartyPanel *panel) {
    u32 i;
    u8 *entry;
    s32 offset = 0;

    memset(panel, 0, 0x10C);
    panel->unk0 = 0;
    panel->unk4 = 0;
    for (i = 0; i < 5; i++) {
        entry = (u8 *)(datGameState + offset + 0xA60);
        offset += 0x1A4;
        if (*(u16 *)entry & 1) {
            func_00285960(entry, 0, i, panel);
            panel->slots[i].index = i;
        } else {
            panel->slots[i].unk8 = -1;
        }
    }
}

extern s8 D_00324510[];
extern u8 D_00324530[];

/* Translate a mask of pad buttons into held/pressed flags: low bits test each digital button (sign bit or pressure bit), the high bits
 * report the same inputs as plain non-zero tests. Select (bit 0) overrides everything else. */
s32 mnuMapPadMaskToFlags(s32 buttons) {
    s32 out = 0;

    if (buttons & 0x1) {
        if (D_00324510[0x21] < 0) {
            out = 1;
        }
    }
    if (buttons & 0x2) {
        if (D_00324510[0x23] < 0) {
            out |= 0x2;
        }
    }
    if (buttons & 0x10) {
        if (D_00324530[6] & 2) {
            out |= 0x10;
        }
    }
    if (buttons & 0x20) {
        if (D_00324530[7] & 2) {
            out |= 0x20;
        }
    }
    if (buttons & 0x40) {
        if (D_00324530[4] & 2) {
            out |= 0x40;
        }
    }
    if (buttons & 0x80) {
        if (D_00324530[5] & 2) {
            out |= 0x80;
        }
    }
    if (buttons & 0x4) {
        if (D_00324510[0x22] < 0) {
            out |= 0x4;
        }
    }
    if (buttons & 0x8) {
        if (D_00324510[0x20] < 0) {
            out |= 0x8;
        }
    }
    if (buttons & 0x200) {
        if (D_00324530[0xA] & 2) {
            out |= 0x200;
        }
    }
    if (buttons & 0x100) {
        if (D_00324530[8] & 2) {
            out |= 0x100;
        }
    }
    if (buttons & 0x800) {
        if (D_00324530[0xB] & 2) {
            out |= 0x800;
        }
    }
    if (buttons & 0x400) {
        if (D_00324530[9] & 2) {
            out |= 0x400;
        }
    }
    if (buttons & 0x1000) {
        if (D_00324510[0x2C] < 0) {
            out |= 0x1000;
        }
    }
    if (buttons & 0x2000) {
        if (D_00324510[0x2D] < 0) {
            out |= 0x2000;
        }
    }
    if (buttons & 0x20000) {
        if (D_00324510[0x2A] < 0) {
            out |= 0x20000;
        }
    }
    if (buttons & 0x10000) {
        if (D_00324510[0x28] < 0) {
            out |= 0x10000;
        }
    }
    if (buttons & 0x80000) {
        if (D_00324510[0x2B] < 0) {
            out |= 0x80000;
        }
    }
    if (buttons & 0x40000) {
        if (D_00324510[0x29] < 0) {
            out |= 0x40000;
        }
    }
    if (buttons & 0x10) {
        if (D_00324510[0x26] != 0) {
            out |= 0x100000;
        }
    }
    if (buttons & 0x20) {
        if (D_00324510[0x27] != 0) {
            out |= 0x200000;
        }
    }
    if (buttons & 0x40) {
        if (D_00324510[0x24] != 0) {
            out |= 0x400000;
        }
    }
    if (buttons & 0x80) {
        if (D_00324510[0x25] != 0) {
            out |= 0x800000;
        }
        if (D_00324510[0x2A] != 0) {
            out |= 0x1000000;
        }
    }
    if (buttons & 0x40) {
        if (D_00324510[0x28] != 0) {
            out |= 0x2000000;
        }
    }
    if (buttons & 0x80) {
        if (D_00324510[0x2B] != 0) {
            out |= 0x4000000;
        }
    }
    if (buttons & 0x40) {
        if (D_00324510[0x29] != 0) {
            out |= 0x8000000;
        }
    }
    if (out & 1) {
        out = out & 1;
    }
    return out;
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
        if (buttons & 0xF0) {
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

void mnuPlayDefaultInputSounds(u32 buttons) {
    mnuPlayInputSoundKind(buttons, 0);
}

/* Party-panel entry stride is 0x1A4 in DDS1 (0x1C4 in DDS2). */
typedef struct MenuPanelEntry {
    u16 flags;          /* 0x00 */
    u8 pad02[2];
    u16 tableIndex;     /* 0x04 */
    u8 pad06[0x4C];
    u16 menuValue;      /* 0x52 */
    u8 pad54[0x150];
} MenuPanelEntry;

extern s32 datGameState;
s32 mnuFindMatchingPartyEntryIndex(s32 object) {
    s32 i;
    u8 *entry = (u8 *)(datGameState + 0xA60);
    for (i = 0; i < 5; i++, entry += 0x1A4) {
        if ((((MenuPanelEntry *)entry)->flags & 1) &&
            ((MenuPanelEntry *)object)->tableIndex == ((MenuPanelEntry *)entry)->tableIndex) {
            return i;
        }
    }
    return 0;
}

extern s8 D_0037CE40[];
extern u16 D_0037CE18[];
extern u16 D_0037CE1A[];

s32 mnuLookupRangeEntry(u16 rangeId) {
    u16 *table = D_0037CE18;
    s8 *entries = D_0037CE40;
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
    return (alt == 0) ? D_0037CE18[index] : D_0037CE1A[index];
}

s32 mnuGetIndexedNonzeroEffect(s32 index) {
    s32 count = 0;
    s32 i;
    s8 *entry = D_0037CE40;
    for (i = 0; i < 6; i++, entry += 3) {
        s32 value = *entry;
        if (value != 0) count++;
        if (index == count - 1) return value;
    }
    return 0;
}

extern s8 D_0037CE42[];

u16 mnuLookupPartyTableValue(u32 count, s32 base, s32 which) {
    u32 i;
    s32 sum = 0;

    for (i = 0; i < count; i++) {
        sum += mnuGetIndexedNonzeroEffect(i);
    }
    if (which == 0) {
        return D_0037CE18[D_0037CE42[(base + sum) * 3]];
    }
    return D_0037CE1A[D_0037CE42[(base + sum) * 3]];
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

u16 mnuGetAdjustedPartyRangeValue(s32 id) {
    s32 index = id & 0xFFFF;
    RangeEntry *record = (RangeEntry *)(index * 0x38 + datCommandRecords);
    u16 scale = record->value;
    u16 addition = record->addition;

    if (mnuGetRangeEntryKind(index) == 1) {
        s32 count = 0;
        s32 sum = 0;
        u16 *entry = (u16 *)(datGameState + 0xA60);
        s32 i;

        for (i = 0; i < 5; i++) {
            u16 flags = entry[0];

            if ((flags & 1) && (flags & 2)) {
                count++;
                sum += entry[4];
            }
            entry += 0x1A4 / 2;
        }
        if (count == 0) {
            return 0;
        }
        scale = addition + sum / count * scale / 100;
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

s32 mnuGetEntryUseStatus(s32 object, u16 id) {
    if (mnuCanAffordEntryCost(id, object) == 0) {
        return -1;
    }
    if (!(((RangeEntry *)datCommandRecords)[id].flags & 1)) {
        return 1;
    }
    if (id < 0x200) {
        return 0;
    }
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

void func_002866B0(u16 ability) {
    func_00118E38(ability);
}

u32 func_002866C8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", ptySkillApplyFieldUseEffect);

u8 mnuIsAbilityValueMarked(u32 id) {
    return *(s8 *)((id & 0xffff) * 2 + datCommandSelectors) == '\x01';
}

extern u32 datAffinityRecords;

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
    if (flags == 0x20008000) {
        return 7;
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
    if (id < 0xa0) {
        return 0;
    }
    return id < 0xbf;
}

s32 func_00286A00(s32 id) {
    if (id < 0x60) {
        return 0;
    }
    return id < 0x7f;
}

u16 mnuGetPartyEntryMenuValue(s32 arg0);

typedef struct PtyBulletInventory {
    u8 pad00[0x12A0];
    u8 itemCount[0xBF];      /* 0x12A0: indexed by item ID */
} PtyBulletInventory;

/* Count a bullet item in the active party: inventory plus one per unit
 * that has the item equipped. Only IDs 0xA0..0xBE are bullet items. */
s32 ptyCountBulletItem(s32 bulletId) {
    s32 count;
    s32 i;

    if (bulletId < 0xA0) {
        return 0;
    }
    if (bulletId >= 0xBF) {
        return 0;
    }
    count = ((PtyBulletInventory *)datGameState)->itemCount[bulletId];
    for (i = 0; i < 5; i++) {
        if (bulletId == mnuGetPartyEntryMenuValue(datGameState + i * 0x1A4 + 0xA60)) {
            count++;
        }
    }
    return count;
}

u32 mnuSetPartyEntryMenuValue(s32 entry, u16 value) {
    ((MenuPanelEntry *)entry)->menuValue = value;
    return 1;
}

u16 mnuGetPartyEntryMenuValue(s32 entry) {
    return ((MenuPanelEntry *)entry)->menuValue;
}

extern u16 D_0037CE00[];
extern void dspStartEntry(s32);

s32 sndPlayPartyItemSe(u32 id, s32 mode) {
    u16 *entry = D_0037CE00;
    u32 i;

    for (i = 0; i < 3; i++, entry += 3) {
        if (id == entry[0]) {
            if (mode != 0) {
                id = entry[2];
                id += 0x17;
            } else {
                id = entry[1];
                id += 10;
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
s32 btlItemApplyPermanentBonus(u16 item, BtlPermanentBonusUnit *unit) {
    s32 stat = -1;
    s32 valid = 0;

    switch (item - 0x50) {
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

s32 btlItemApplyDirectEffect(s32 context, u16 item, s32 mode,
                             BtlPermanentBonusUnit *unit) {
    s32 result = btlItemApplyPermanentBonus(item, unit);
    switch (result) {
    case 1:
        func_00281780(context, mnuFindMatchingPartyEntryIndex((s32)unit), 1, 0);
        sndSetSequenceVolumePan(16, 127, 63);
        return 1;
    case 2:
        return result;
    }
    switch (item) {
    case 57:
        if (fldIsFlagActive() != 0) {
            sndPlayPartyItemSe(57, 1);
            return 2;
        }
        sndPlayPartyItemSe(57, 0);
        fldSetPrimarySceneFlag();
        break;
    case 58:
        if (fldTestSecondarySceneFlag() != 0) {
            sndPlayPartyItemSe(58, 1);
            return 2;
        }
        sndPlayPartyItemSe(58, 0);
        fldSetSecondarySceneFlag();
        break;
    default:
        return 0;
    }
    func_00281780(context, mnuFindMatchingPartyEntryIndex((s32)unit), 2, 0);
    sndSetSequenceVolumePan(7, 127, 63);
    return 1;
}

typedef struct MenuSelectionEntry {
    u8 pad00[0xE];
    u16 flags;
} MenuSelectionEntry;

s32 mnuGetSelectionFromFlags(s32 entry) {
    u16 flags = ((MenuSelectionEntry *)entry)->flags;
    if (flags & 0x400) return 0;
    if (flags & 0x100) return 1;
    if (flags & 0x80) return 2;
    if (flags & 0x40) return 3;
    if (flags & 0x10) return 4;
    return -1;
}

s32 mnuGetMatchingPartyEntryMask(s32 object) {
    s32 i;
    u8 *entry = (u8 *)(datGameState + 0xA60);
    for (i = 0; i < 5; i++, entry += 0x1A4) {
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

u8 func_00286F48(void) {
    return D_003BC7B4 != 0;
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
    func_002878D8(-1);
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
    func_002878D8(-1);
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

u8 func_00287198(s32 encodedIndex) {
    return evtStageTestState.entries[encodedIndex & 0xffff].motionIndex;
}

f32 func_002871C0(s32 encodedIndex) {
    return evtStageTestState.entries[encodedIndex & 0xffff].frame;
}

void func_002871E8(s32 encodedIndex, f32 *out) {
    out[0] = evtStageTestState.entries[encodedIndex & 0xffff].position[0];
    out[1] = evtStageTestState.entries[encodedIndex & 0xffff].position[1];
    out[2] = evtStageTestState.entries[encodedIndex & 0xffff].position[2];
}

void func_00287220(s32 encodedIndex, f32 *out) {
    out[0] = evtStageTestState.entries[encodedIndex & 0xffff].rotation[0];
    out[1] = evtStageTestState.entries[encodedIndex & 0xffff].rotation[1];
    out[2] = evtStageTestState.entries[encodedIndex & 0xffff].rotation[2];
}

extern void *memcpy(void *dest, const void *src, u32 size);
extern u8 D_003B2520[];
extern u8 D_003B2530[];
extern u8 D_003B2540[];
extern u8 D_003B2550[];
extern void evtToggleSavedDrawVectors(s32 frames, f32 first, f32 second);
extern void func_00107FD8(s32 mode, s32 slot, f32 *color);
extern void func_001080D8(s32 mode, s32 slot, f32 *color);
extern void kwlnSetBackgroundColorTarget(s32 mode, f32 *color);
extern void kwlnSetDrawColorTarget(s32 mode, f32 *color);
extern void evtSetDrawVectorTarget(s32 mode, f32 x, f32 y, f32 z, f32 w);

void evtStageTestResetViewAndLighting(void)
{
    f32 firstColor[4];
    f32 secondColor[4];
    f32 firstVector[4];
    f32 secondVector[4];

    memcpy(firstColor, D_003B2520, sizeof(firstColor));
    memcpy(secondColor, D_003B2530, sizeof(secondColor));
    memcpy(firstVector, D_003B2540, sizeof(firstVector));
    memcpy(secondVector, D_003B2550, sizeof(secondVector));
    evtToggleSavedDrawVectors(0, 5.0f, 0.0f);
    func_00107FD8(0, 0, firstColor);
    func_001080D8(0, 0, secondColor);
    kwlnSetBackgroundColorTarget(0, firstVector);
    kwlnSetDrawColorTarget(0, secondVector);
    evtSetDrawVectorTarget(0, 255.0f, 255.0f, 2000.0f, 30000.0f);
    sdfSceneProjectionParameters[3] = 0.7551905f;
    PCP_COPY_VECTOR(sdfViewTargetVector, D_0037CE70);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_0037CE60);
    PCP_COPY_VECTOR(sdfViewUpVector, D_0037CE80);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfConsCacheTransformedNode(sdfSceneProjectionParameters, sdfViewMatrix);
}

void mnuSetStageTestCameraOffset(f32 offset) {
    sdfSceneProjectionParameters[5] = 2048.0f;
    sdfSceneProjectionParameters[4] = offset + 2048.0f;
    evtStageTestResetViewAndLighting();
}

extern s8 D_0037CE90[];
extern u8 D_0037CE98[];

void evtStageTestInit(s32 mode) {
    f32 offset;
    s32 i;
    s32 value;

    evtStageTestState.mode = mode;
    evtStageTestState.assetRequest = 0;
    evtStageTestState.model = 0;
    evtStageTestState.flag = 0;
    if (mode == 0) {
        evtStageTestState.entries = (StageTestEntry *)D_0037CE98;
        offset = -140.0f;
    } else {
        evtStageTestState.entries = NULL;
        offset = 140.0f;
    }
    value = D_0037CE90[mode];
    evtStageTestState.queue.flags = 0;
    for (i = 0; i < 2; i++) {
        evtStageTestState.queue.slot[i].assetResource = value;
        evtStageTestState.queue.slot[i].modelId = -1;
        evtStageTestState.queue.slot[i].assetOption = 0;
        evtStageTestState.queue.slot[i].flags = 0;
    }
    mnuSetStageTestCameraOffset(offset);
}

void btlStopStage(void) {
    if (evtStageTestState.mode != 1) {
        evtStageTestState.pendingEffect = -1;
        if (evtStageTestState.effect != 0) {
            evtStageTestDestroyModelEffect();
        }
        if (evtStageTestState.model != 0) {
            mdlDestroyContext(evtStageTestState.model);
            evtStageTestState.model = 0;
        }
    }
}

void mnuResetWorkFloats(void) {
    btlStopStage();
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

u32 mnuHasPendingBlockFlag(u32 *flags) {
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

    if (evtStageTestState.model != 0 && evtStageTestState.queue.slot[0].modelId == (s32)evtStageTestState.entries[index].modelId) {
        return 0;
    }
    btlStopStage();
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

INCLUDE_ASM(const s32, "game/code_00282850", func_002877A8);

INCLUDE_ASM(const s32, "game/code_00282850", func_002878D8);

extern u8 *D_003BAA20;
extern void mdlStoreTertiaryVectorVU(void *);

f32 mnuSetModelScaleVector(void *model, s32 useTable) {
    f32 scale = 1.0f;
    f32 vector[4];

    if (useTable != 0) {
        scale = *(f32 *)(D_003BAA20 + evtStageTestState.queue.slot[0].modelId * 624 + 0x10);
    }
    vector[0] = scale;
    vector[1] = scale;
    vector[2] = scale;
    vector[3] = 1.0f;
    VU0_LOAD_VF(vf10, vector);
    mdlStoreTertiaryVectorVU(model);
    return scale;
}

void mnuResetWorkPair(void) {
    *(s32 *)(D_0037CE70 + 0) = 0;
    *(s32 *)(D_0037CE70 + 4) = 0;
    *(f32 *)(D_0037CE70 + 8) = -400.0f;
    *(s32 *)(D_0037CE60 + 0) = 0;
    *(s32 *)(D_0037CE60 + 4) = 0;
    *(s32 *)(D_0037CE60 + 8) = 0;
}

extern void mdlStorePrimaryVectorVU(void *);

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
        mnuSetModelScaleVector((void *)model, 0);
        vec[2] = ((StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries))->position[2];
        mnuResetWorkPair();
    } else {
        scale = mnuSetModelScaleVector((void *)model, 1);
        entry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries);
        vec[0] -= entry->position[0] - entry->position[0] * scale;
        vec[1] -= entry->position[1] - entry->position[1] * scale;
        vec[2] = 0.0f;
        *(f32 *)(D_0037CE70 + 8) = (-400.0f - entry->position[2]) * scale;
    }
    VU0_LOAD_VF(vf10, vec);
    mdlStorePrimaryVectorVU((void *)model);
}

void evtStageTestApplyEntryRotation(s32 model) {
    StageTestEntry *entry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * 60 + (s32)evtStageTestState.entries);

    func_002E7F20(entry->rotation[0] * 3.14159265f / 180.0f, entry->rotation[1] * 3.14159265f / 180.0f,
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
    evtStageTestResetViewAndLighting();
    PCP_COPY_VECTOR(sdfViewTargetVector, D_0037CE70);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_0037CE60);
    PCP_COPY_VECTOR(sdfViewUpVector, D_0037CE80);
    sdfCameraBuildProjection(sdfSceneProjectionParameters);
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(600.0f, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324590);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, &eye);
    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, &at);
    sdfVuBuildLookAtBasis(&eye, &at, sdfViewUpVector);
    VU0_STORE_MATRIX_UNCLOBBERED(sdfViewMatrix);
    sdfConsBuildMatrixPacket(D_003270F0 + slot * 8000, sdfSceneProjectionParameters, sdfViewMatrix);
    sdfConsCacheTransformedNode(sdfSceneProjectionParameters, sdfViewMatrix);
}

s8 evtStageTestUpdate(s32 frame) {
    s8 result = func_002877A8();

    if (result == 1) {
        return 1;
    }
    if (evtStageTestState.mode != 1) {
        if (evtStageTestState.model != 0) {
            mnuApplyModelCamera(evtStageTestState.model);
            evtStageTestApplyEntryRotation(evtStageTestState.model);
            evtStageTestUpdateCamera();
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
void evtStageTestQueueMotionSegment(s32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames) {
    StageTestSlot *slot = evtStageTestState.queue.slot;

    slot->state = 1;
    slot->motionIndex = motionIndex;
    slot->blendLeadFrames = (s32)blendLeadFrames;
    slot->blendDurationFrames = (s32)blendDurationFrames;
}

void evtStageTestForceDefaultMotion(void) {
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

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2520);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2530);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2540);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2550);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2560);

INCLUDE_ASM(const s32, "game/code_00282850", func_002881F0);

u32 func_00288458(void) {
    func_002881F0();
    return 0;
}

void *evtCreateBattleStageTestCamera(void) {
    f32 position[4] = {401.0f, -593.0f, -1208.25f, 0.0f};
    f32 orientation[4] = {0.22f, 0.12f, 0.03f, 1.0f};
    StageCameraTarget *target = evtCreateWorldObjectAtTransform(position, orientation);

    target->unk8 = D_003BC7C8;
    return func_00288458;
}

typedef struct StageGraphicsCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} StageGraphicsCallback;
extern StageGraphicsCallback D_00325708;
extern s32 D_003BC7D0;
extern s32 D_003BC7D4;
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, s32);
extern void kwlnDrawSpriteCell(void *, s32, s32, s32, s32);
extern s32 sdfCreateFormattedSifCommand();
extern void evtCreateWorldObjectForKey(s32, s32);

void *evtBattleStageTestScreen(void) {
    void *packets = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(packets);
    kwlnDrawSpriteCell(packets, 0x84, 0x46, 0x14, 9);
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7840, 0x7BA0, 0xFEFFFF, 0, "BATTLE STAGE"));
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7A80, 0x7C60, 0xFEFFFF, 6, "F%03d_%03d", D_003BC7D0, D_003BC7D4));
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R = EVENT SELECT"));
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x7900, 0x7D80, 0xFEFFFF, 0, "RR  = ENTER"));
    D_00325708.invoke(&D_00325708, packets);
    if (D_00324510[0x21] < 0) {
        evtCreateWorldObjectForKey(D_003BC7D0, D_003BC7D4);
        return evtCreateBattleStageTestCamera;
    }
    if (D_00324510[0x25] & 2) {
        D_003BC7D0++;
    }
    if (D_00324510[0x24] & 2) {
        D_003BC7D0--;
    }
    if (D_00324510[0x2A] & 2) {
        D_003BC7D4++;
    }
    if (D_00324510[0x28] & 2) {
        D_003BC7D4--;
    }
    return 0;
}

void evtDestroyBattleStageTestWorldNode(void) {
    evtDestroySecondaryWorldNode();
}

void evtBattleStageTestStopTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B2608, 1);
    kwlnDebugGraphSetEnabled(0);
}

void btlCreateStageTestTask(void) {
    kwlnDebugGraphSetEnabled(1);
    kwlnTaskCreate(D_003B2608, 0x2b0c, 1, 1, evtBattleStageTestScreen, evtDestroyBattleStageTestWorldNode, 0);
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
        func_002EDC50(object->payload);
        sdfReleaseChipBlock(object->allocation);
        sdfReleaseChipBlock((void *)object);
        return 0;
    }
    return 1;
}

void func_00288788(void) {
    btlDestroyStageTask();
}

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2608);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC778);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC780);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC788);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC790);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC798);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7A0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7A8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B4);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7BC);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7C0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7C8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7D0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7D4);

