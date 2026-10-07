#include "common.h"
#include "dds3obj.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "pcp_vu0.h"
#include "mnu.h"
#include "mnu_shop.h"
#include "mdl.h"
#include "dat_state.h"

struct FrFontGlyph;
extern u32 func_001978E8(s32, s32, s32, u32, char *, s32);
extern s32 func_001958A0(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern char D_003BC7A0[];

#define MNU_PANEL_ITEM_COUNT 5
#define MNU_PANEL_STATE_BYTES 0x64
#define MNU_PANEL_GROUP_BYTES 0x28
#define MNU_SPRITE_STATE_BYTES 0x20
#define MNU_SIMPLE_SPRITE_BYTES 0x28
#define MNU_PANEL_ITEM_BYTES 0x90
#define MNU_TRANSITION_LIMIT 0x100
#define MNU_TRANSITION_STEP 8
#define MNU_GRADIENT_FADE_STEP 0x20
#define MNU_PROFILE_PHASE_PERIOD 0x200
#define MNU_PROFILE_PHASE_STEP 12
#define MNU_NO_SELECTION 0xffffffff
#define MNU_PANEL_TEXTURE_COUNT 9
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
#define MNU_COMMAND_USE_STATUS_BOUNDARY 0x200
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

extern void func_00285960(DatPartyRecord *entry, s32 arg1, u32 index, PartyPanel *panel);

extern void func_002E7F20(f32, f32, f32);
extern void mdlUpdateContextRotationBasisFromQuaternion(s32);

extern EffWorldNode *evtCreateWorldObjectAtTransform(f32 *, f32 *);
extern char D_003BC7C8[];


extern void func_002878D8(s32 arg0);

extern s32 D_003BC7B4;

extern s32 datCommandSelectors;

extern s32 datCommandRecords;

extern u32 effCreateStatusBatch(u32);

extern s32 mnuLookupRangeEntry(u16);

extern MenuPanelHandles *mnuCreatePanelSpriteHandles(u32, s32, s32);
extern void mnuReleaseResourceList(MenuPanelHandles *);

extern s32 sdfAllocSizeClassBlock(u32);

extern s32 func_002877A8(void);

typedef struct MenuPanelItem MenuPanelItem;

typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
    MenuPanelItem *children[5]; /* 0x0C */
    u32 selection;    /* 0x20 */
    s32 initialValue; /* 0x24: initialized to 0x100 */
} MenuPanelGroup;

extern void mnuClearPanelGroupSelection(MenuPanelGroup *);

extern MenuPanelItem *mnuCreatePanelItem(void);
extern void mnuInitializePanelGroupGridSlots(MenuPanelItem *, s32, s32);
extern void mnuInitializePanelItemGridSlots(MenuPanelItem *, s32, s32);
extern void mnuFreePanelItemWork(MenuPanelItem *);
extern void mnuStorePanelItemValue(MenuPanelItem *, u32);
extern void mnuSetPanelItemSelection(MenuPanelItem *, s32);
extern void mnuSetPanelItemOption(MenuPanelItem *, u32);
extern void sdfReleaseChipBlock();

extern char D_003B2608[]; /* "battle stage test" */

extern u8 D_0037CE60[];

extern u8 D_0037CE70[];


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


typedef struct MenuList MenuList;


extern void func_00281D40(s32, s32, s32, MenuPageWindow *, s32, s32);
extern void func_00282360(s32, s32, s32, MenuPageWindow *, s32, s32);


extern s32 D_0037CD80[];

void mnuDispatchListPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu, s32 panelIndex, s32 param) {
    MenuPageSlot *panel = &menu->slots[panelIndex];
    s32 mode = panel->kind;

    if (menu->selected >= 0) {
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

void func_002828D0(s32 *position, MenuPageWindow *menu, s32 panelIndex) {
    MenuPageSlot *panel = &menu->slots[panelIndex];
    PartyPanel *layout = menu->records;
    u32 panelFlags = panel->flags;
    s32 secondCount = layout->unk4;
    s32 totalCount;
    s32 firstCount = layout->unk0;
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
        break;
    }
    default:
        break;
    }
    return;
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

    func_002828D0(positionOffset, menu, 0);
    node = panel->windowSprites;
    if (node != NULL) {
        node->unkC = overrideValue;
    }
    x += menu->scrollOffset * 0x10;
    menu->scrollOffset = (s32)((f32)menu->scrollOffset / 1.19999993f);
    /* Both arms are identical in retail; kept as written. */
    if (menu->flags & 0x100) {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selected, param);
    } else {
        mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selected, param);
    }
    node = panel->windowSprites;
    if (node != NULL) {
        node->unkC = 0;
    }
}

/* Draw the selected panel or all visible/additional panels, then advance the transition. */
void mnuDrawStageTestList(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 param) {
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
            func_002828D0(positionOffset, menu, panelIndex);
            mnuDispatchListPanel(x + positionOffset[0], y + positionOffset[1], z, menu, panelIndex, param);
        }
    }
    mnuAdvancePanelTransition(menu);
}

/* Same as mnuDrawStageTestList with no override value; arg5 is unused. */
void mnuDrawPanelListDefault(s32 x, s32 y, s32 z, MenuPageWindow *menu, s32 param, s32 unused) {
    mnuDrawStageTestList(x, y, z, 0, menu, param);
}


typedef struct MenuPanelState {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    u32 state; /* 0x14 */
    u32 firstValueA; /* 0x18 */
    u32 firstValueB; /* 0x1C */
    MenuGridSlot firstSlot; /* 0x20 */
    u32 secondValueA; /* 0x28 */
    u32 secondValueB; /* 0x2C */
    MenuGridSlot secondSlot; /* 0x30 */
    MenuGridSlot thirdSlot; /* 0x38 */
    u32 thirdValueA; /* 0x40 */
    u32 thirdValueB; /* 0x44 */
    MenuGridSlot fourthSlot; /* 0x48 */
    u32 fourthValueA; /* 0x50 */
    u32 fourthValueB; /* 0x54 */
    u32 fourthValueC; /* 0x58 */
    u8 pad5C[4];
    MenuPanelHandles *resourceHandle; /* 0x60 */
} MenuPanelState;

typedef char MenuPanelStateSizeCheck[(sizeof(MenuPanelState) == MNU_PANEL_STATE_BYTES) ? 1 : -1];
typedef char MenuPanelStateResourceHandleOffsetCheck[((u32)&((MenuPanelState *)0)->resourceHandle == 0x60) ? 1 : -1];

/* Allocate a zeroed native panel state with the requested dimensions. */
MenuPanelState *mnuCreatePanelState(s32 width, s32 height) {
    MenuPanelState *panel = sdfAllocSizeClassBlock(MNU_PANEL_STATE_BYTES);
    memset(panel, 0, MNU_PANEL_STATE_BYTES);
    panel->width = width;
    panel->height = height;
    return panel;
}

void mnuDestroyPanelState(MenuPanelState *panel) {
    MenuPanelHandles *resourceHandle;

    resourceHandle = panel->resourceHandle;
    if (resourceHandle != 0) {
        mnuReleaseResourceList(resourceHandle);
    }
    sdfReleaseChipBlock(panel);
}

void func_00282CA8(MenuPanelState *panel, u32 valueA, u32 valueB, u32 resource,
                                    u32 index) {
    panel->firstValueA = valueA;
    panel->firstValueB = valueB;
    itfGridStorePosition(&panel->firstSlot, resource, index);
}

void func_00282CD0(MenuPanelState *panel, u32 valueA, u32 valueB, u32 resource,
                                    u32 index) {
    panel->secondValueA = valueA;
    panel->secondValueB = valueB;
    itfGridStorePosition(&panel->secondSlot, resource, index);
}

void mnuInitializePanelResource(MenuPanelState *panel, s32 resource, s32 target) {
    panel->resourceHandle = mnuCreatePanelSpriteHandles(2, resource, target);
}

void func_00282D28(MenuPanelState *panel, u32 valueA, u32 valueB, u32 resource,
                                    u32 index) {
    panel->thirdValueA = valueA;
    panel->thirdValueB = valueB;
    itfGridStorePosition(&panel->thirdSlot, resource, index);
}

void func_00282D50(MenuPanelState *panel, u32 valueA, u32 valueB, u32 resource,
                                    u32 index, u32 additionalValue) {
    panel->fourthValueA = valueA;
    panel->fourthValueB = valueB;
    itfGridStorePosition(&panel->fourthSlot, resource, index);
    panel->fourthValueC = additionalValue;
}

void mnuSetPanelState(MenuPanelState *panel, u32 state) {
    panel->state = state;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00282DA0);

/* Create the five panel items owned by this group and clear its selection. */
s32 mnuCreatePanelGroup(s32 parent) {
    MenuPanelGroup *group = sdfAllocSizeClassBlock(MNU_PANEL_GROUP_BYTES);
    s32 panelIndex;
    for (panelIndex = 0; panelIndex < MNU_PANEL_ITEM_COUNT; panelIndex++) {
        MenuPanelItem *panelItem = mnuCreatePanelItem();
        mnuInitializePanelGroupGridSlots(panelItem, parent, panelIndex);
        group->children[panelIndex] = panelItem;
    }
    mnuClearPanelGroupSelection(group);
    group->initialValue = 0x100;
    return (s32)group;
}

/* Release every owned panel item before releasing the group allocation. */
void mnuDestroyPanelGroup(MenuPanelGroup *group) {
    s32 panelIndex;
    for (panelIndex = 0; panelIndex < MNU_PANEL_ITEM_COUNT; panelIndex++) {
        mnuFreePanelItemWork(group->children[panelIndex]);
    }
    sdfReleaseChipBlock(group);
}

/* Configure all five panel items against the same grid object. */
void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 gridObject) {
    s32 panelIndex;
    for (panelIndex = 0; panelIndex < MNU_PANEL_ITEM_COUNT; panelIndex++) {
        mnuInitializePanelItemGridSlots(group->children[panelIndex], gridObject, panelIndex);
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00283110);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selection, u32 option) {
    mnuSetPanelItemSelection(group->children[index], selection);
    mnuSetPanelItemOption(group->children[index], option);
}

/* Create a zeroed sprite-resource state with its native initial value. */
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

/* Create the DDS1 sprite record, including its five resource sets and blend weight. */
MenuSimpleSpriteState *mnuAllocateSimpleSprite(
    struct EffectSlotSet *resourceSet0, struct EffectSlotSet *resourceSet1,
    struct EffectSlotSet *resourceSet2, struct EffectSlotSet *resourceSet3,
    struct EffectSlotSet *resourceSet4) {
    MenuSimpleSpriteState *sprite = sdfAllocSizeClassBlock(MNU_SIMPLE_SPRITE_BYTES);
    memset(sprite, 0, MNU_SIMPLE_SPRITE_BYTES);
    sprite->resourceSets[0] = resourceSet0;
    sprite->resourceSets[1] = resourceSet1;
    sprite->resourceSets[2] = resourceSet2;
    sprite->resourceSets[3] = resourceSet3;
    sprite->resourceSets[4] = resourceSet4;
    sprite->blendFactor = 0x100;
    return sprite;
}

void mnuFreeSimpleSpriteWork(MenuSimpleSpriteState *sprite) {
    sdfReleaseChipBlock(sprite);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283838);


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

/* Set both effect positions; only the first Y comes from the active menu entry. */
void mnuSetPairedEffectPositions(MenuEffectPair *pair) {
    MenuEffectNode *first = pair->first;
    MenuEffectNode *second = pair->second;
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

/* Release the two effect-batch handles in the native object-word layout. */
void mnuReleasePairedEffectBatches(s32 *objectWords) {
    u32 effectIndex;
    for (effectIndex = 0; effectIndex < 2; effectIndex++) {
        effDestroyPackedBatch(objectWords[effectIndex + 16]);
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

/* Release the nine sprite texture handles, then the paired effect batches. */
void mnuReleaseSpriteTextures(s32 *objectWords) {
    u32 textureIndex;
    for (textureIndex = 0; textureIndex < MNU_PANEL_TEXTURE_COUNT; textureIndex++) {
        effDestroyResourceSlotSet(objectWords[textureIndex + 7]);
    }
    mnuReleasePairedEffectBatches(objectWords);
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00284418);

INCLUDE_ASM(const s32, "game/code_00282850", func_002845F8);

void func_00284880(void) {
}

struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    MenuGridSlot groupGridSlots[7]; /* 0x24 */
    MenuGridSlot gridSlots[5]; /* 0x5C */
    u8 pad84[4];
    u32 initialValue; /* 0x88 */
    u32 selectionRamp; /* 0x8C */
};

/* Allocate a zeroed native panel item and initialize its three default values. */
MenuPanelItem *mnuCreatePanelItem(void) {
    MenuPanelItem *panelItem = sdfAllocSizeClassBlock(MNU_PANEL_ITEM_BYTES);
    memset(panelItem, 0, MNU_PANEL_ITEM_BYTES);
    panelItem->value14 = 0x63;
    panelItem->value10 = 0x8c;
    panelItem->initialValue = 0x100;
    return panelItem;
}

void mnuInitializePanelGroupGridSlots(MenuPanelItem *item, s32 gridObject, s32 panelIndex) {
    s32 entryIndices[5] = {0, 2, 1, 3, 4};

    itfGridStorePosition(&item->groupGridSlots[0], gridObject, 3);
    itfSetGridEntryQuantizedAndRefresh(item->groupGridSlots[0].set, item->groupGridSlots[0].index, 0xD30, 0x20, 0, 0);
    itfGridStorePosition(&item->groupGridSlots[1], gridObject, 8);
    itfSetGridEntryQuantizedAndRefresh(item->groupGridSlots[1].set, item->groupGridSlots[1].index, 0, 0, 0, 0);
    itfGridStorePosition(&item->groupGridSlots[2], gridObject, 10);
    itfGridStorePosition(&item->groupGridSlots[3], gridObject, 14);
    itfSetGridEntryQuantizedAndRefresh(item->groupGridSlots[3].set, item->groupGridSlots[3].index, 0x540, 0x58, 0, 0);
    itfGridStorePosition(&item->groupGridSlots[4], gridObject, entryIndices[panelIndex] + 0x20);
    itfSetGridEntryQuantizedAndRefresh(item->groupGridSlots[4].set, item->groupGridSlots[4].index, 0x100, -0x48, 0, 0);
    itfGridStorePosition(&item->groupGridSlots[5], gridObject, 4);
    itfSetGridEntryQuantizedAndRefresh(item->groupGridSlots[5].set, item->groupGridSlots[5].index, 0x140, 0x10, 0, 0);
    itfGridStorePosition(&item->groupGridSlots[6], gridObject, 5);
    itfSetGridEntryQuantizedAndRefresh(item->groupGridSlots[6].set, item->groupGridSlots[6].index, 0x140, 0x10, 0, 0);
}

/* Bind five grid object/index references and initialize their quantized bounds.
 * The x/y members in this path hold object addresses and entry indices, not coordinates. */
void mnuInitializePanelItemGridSlots(MenuPanelItem *item, s32 gridObject, s32 panelIndex) {
    s32 entryIndices[5] = {0, 4, 1, 2, 3};

    itfGridStorePosition(&item->gridSlots[0], gridObject, 7);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[0].set, item->gridSlots[0].index, 0, 0, 0, 0);
    itfGridStorePosition(&item->gridSlots[1], gridObject, 5);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[1].set, item->gridSlots[1].index, 0x440, 0x58, 0, 0);
    itfGridStorePosition(&item->gridSlots[2], gridObject, 6);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[2].set, item->gridSlots[2].index, 0x440, 0x58, 0, 0);
    itfGridStorePosition(&item->gridSlots[3], gridObject, 9);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[3].set, item->gridSlots[3].index, 0x620, 0x50, 0, 0);
    itfGridStorePosition(&item->gridSlots[4], gridObject, entryIndices[panelIndex]);
    itfSetGridEntryQuantizedAndRefresh(item->gridSlots[4].set, item->gridSlots[4].index, 0x100, -0x48, 0, 0);
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

void mnuFreePanelItemWork(MenuPanelItem *item) {
    sdfReleaseChipBlock(item);
}

extern void func_00284C48(s32, s32, s32, u32, s32, MenuPanelItem *, s32);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284C48);

void mnuDrawPanelItemValue(s32 x, s32 y, s32 depth, s32 mode, MenuPanelItem *item, s32 layer) {
    char buffer[16];
    struct FrFontGlyph *glyph;
    s32 value;
    u32 opacity = item->initialValue;
    u32 color;

    func_002BF4E0(x, y, depth, opacity, 0,
                 (s32)item->groupGridSlots[0].set, item->groupGridSlots[0].index, layer);
    func_00284C48(x, y, depth, opacity, mode, item, layer);
    func_002BF4E0(x, y, depth, opacity, 0,
                 (s32)item->groupGridSlots[1].set, item->groupGridSlots[1].index, layer);
    func_002BF4E0(x, y, depth, opacity, 0,
                 (s32)item->groupGridSlots[4].set, item->groupGridSlots[4].index, layer);
    if (mode == 1 || (mode == 0 && (item->selection != 0 || item->option != 0))) {
        func_002BF4E0(x, y, depth, opacity, 0,
                     (s32)item->gridSlots[0].set, item->gridSlots[0].index, layer);
        func_002BF4E0(x, y, depth, opacity, 0,
                     (s32)item->gridSlots[4].set, item->gridSlots[4].index, layer);
    }

    value = item->value18;
    value += item->option;
    if (mode == 1 || (mode == 0 && (item->selection != 0 || item->option != 0))) {
        color = 0xC0907080;
    } else {
        color = 0xA09DC380;
    }
    color = uiBlendColors(color, color & ~0xFF, opacity);
    func_003014F0(buffer, D_003BC7A0, value);
    glyph = (struct FrFontGlyph *)func_001978E8(x + 0x340, y + 0x28, depth, color, buffer, 0);
    func_001958A0(glyph, 1, layer);
    frFontQueueGlyphInSelectedSlot(glyph);
}


void mnuSetProfilePanelValues(MenuProfilePanel *panel, s32 value, s32 option) {
    panel->capValue = value;
    panel->option = option;
}

/* Create a profile panel from the selection state's current profile ID and record. */
MenuProfilePanel *mnuCreateProfilePanel(s32 selectionState) {
    MenuProfilePanel *panel = (MenuProfilePanel *)sdfAllocSizeClassBlock(sizeof(MenuProfilePanel));
    s32 profileId;
    u32 profileRecordAddress;
    memset(panel, 0, sizeof(MenuProfilePanel));
    profileId = scrGetSelectedOperandIndex(selectionState);
    profileRecordAddress = ptyGetCurrentProfileRecord(selectionState);
    mnuSetProfilePanelValues(panel, prfGetCapValue((u16)profileId), *(u32 *)profileRecordAddress);
    panel->opacity = 0x100;
    return panel;
}

void mnuFreeProfilePanelWork(MenuProfilePanel *panel) {
    sdfReleaseChipBlock(panel);
}

void mnuCacheProfilePanelGridPositions(MenuProfilePanel *panel, u32 grid, u32 fillIndex,
                                     u32 backgroundIndex, u32 completedIndex) {
    itfGridStorePosition(&panel->fill, grid, fillIndex);
    itfSetGridEntryQuantizedAndRefresh((s32)panel->fill.set, panel->fill.index, 0, 0, 0, 0);
    itfGridStorePosition(&panel->background, grid, backgroundIndex);
    itfGridStorePosition(&panel->completed, grid, completedIndex);
}

extern void func_00285208(s32, s32, s32, MenuProfilePanel *, s32);
INCLUDE_ASM(const s32, "game/code_00282850", func_00285208);

/* Draw first, then advance the native phase by twelve with a single period subtraction. */
void mnuDrawAndAdvanceProfilePanel(s32 x, s32 y, s32 z, MenuProfilePanel *panel, s32 option) {
    s32 phase;
    s32 nextPhase;
    func_00285208(x, y, z, panel, option);
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

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void mnuDrainPanelTransitions(u32 stateAddress, u32 callbackArgument) {
    s32 entryCount;

    entryCount = *(s32 *)stateAddress;
    while (entryCount != 0) {
        func_002854B0(1, 0, stateAddress, callbackArgument);
        entryCount = *(s32 *)stateAddress;
    }
}

s32 mnuHasPopupSelectionFlag(s32 *flags) {
    return (*flags & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285670);

/* Compare the popup state's current entry address, not an entry payload value. */

u8 mnuIsPopupEntryValue(s32 stateAddress, s32 entryAddress) {
    return ((MenuPopupState *)stateAddress)->entryAddress == entryAddress;
}

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void mnuSetPopupEntry(s32 entrySlotAddress, s32 entryAddress) {
    u16 retainedFlags;

    retainedFlags = *(u16 *)entryAddress;
    *(s32 *)entrySlotAddress = entryAddress;
    *(s32 *)entryAddress = retainedFlags;
}

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void mnuSetPopupEntryFlagged(s32 entrySlotAddress, s32 entryAddress) {
    u16 retainedFlags;

    retainedFlags = *(u16 *)entryAddress;
    *(s32 *)entrySlotAddress = entryAddress;
    *(s32 *)entryAddress = retainedFlags | MNU_POPUP_INSERT_BEFORE_TOP;
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
        mnuSetPopupEntryFlagged(entrySlotAddress, ((MenuPopupState *)stateAddress)->entryAddress);
        return;
    }
}

void func_00285960(DatPartyRecord *entry, s32 unused, u32 index, PartyPanel *panel) {
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
            func_00285960(partyEntry, 0, partyIndex, panel);
            panel->slots[partyIndex].index = partyIndex;
        } else {
            panel->slots[partyIndex].unk8 = -1;
        }
    }
}

extern s8 D_00324510[];
extern u8 D_00324530[];

/* Translate requested inputs using native sign-bit/trigger-bit and nonzero tests.
 * Input bit zero has exclusive priority over every other result bit. */
s32 mnuMapPadMaskToFlags(s32 buttonMask) {
    s32 inputFlags = 0;

    if (buttonMask & MNU_INPUT_PRIORITY_BIT) {
        if (D_00324510[0x21] < 0) {
            inputFlags = MNU_INPUT_PRIORITY_BIT;
        }
    }
    if (buttonMask & 0x2) {
        if (D_00324510[0x23] < 0) {
            inputFlags |= 0x2;
        }
    }
    if (buttonMask & 0x10) {
        if (D_00324530[6] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x10;
        }
    }
    if (buttonMask & 0x20) {
        if (D_00324530[7] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x20;
        }
    }
    if (buttonMask & 0x40) {
        if (D_00324530[4] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x40;
        }
    }
    if (buttonMask & 0x80) {
        if (D_00324530[5] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x80;
        }
    }
    if (buttonMask & 0x4) {
        if (D_00324510[0x22] < 0) {
            inputFlags |= 0x4;
        }
    }
    if (buttonMask & 0x8) {
        if (D_00324510[0x20] < 0) {
            inputFlags |= 0x8;
        }
    }
    if (buttonMask & 0x200) {
        if (D_00324530[0xA] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x200;
        }
    }
    if (buttonMask & 0x100) {
        if (D_00324530[8] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x100;
        }
    }
    if (buttonMask & 0x800) {
        if (D_00324530[0xB] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x800;
        }
    }
    if (buttonMask & 0x400) {
        if (D_00324530[9] & MNU_PAD_TRIGGER_BIT) {
            inputFlags |= 0x400;
        }
    }
    if (buttonMask & 0x1000) {
        if (D_00324510[0x2C] < 0) {
            inputFlags |= 0x1000;
        }
    }
    if (buttonMask & 0x2000) {
        if (D_00324510[0x2D] < 0) {
            inputFlags |= 0x2000;
        }
    }
    if (buttonMask & 0x20000) {
        if (D_00324510[0x2A] < 0) {
            inputFlags |= 0x20000;
        }
    }
    if (buttonMask & 0x10000) {
        if (D_00324510[0x28] < 0) {
            inputFlags |= 0x10000;
        }
    }
    if (buttonMask & 0x80000) {
        if (D_00324510[0x2B] < 0) {
            inputFlags |= 0x80000;
        }
    }
    if (buttonMask & 0x40000) {
        if (D_00324510[0x29] < 0) {
            inputFlags |= 0x40000;
        }
    }
    if (buttonMask & 0x10) {
        if (D_00324510[0x26] != 0) {
            inputFlags |= 0x100000;
        }
    }
    if (buttonMask & 0x20) {
        if (D_00324510[0x27] != 0) {
            inputFlags |= 0x200000;
        }
    }
    if (buttonMask & 0x40) {
        if (D_00324510[0x24] != 0) {
            inputFlags |= 0x400000;
        }
    }
    if (buttonMask & 0x80) {
        if (D_00324510[0x25] != 0) {
            inputFlags |= 0x800000;
        }
        if (D_00324510[0x2A] != 0) {
            inputFlags |= 0x1000000;
        }
    }
    if (buttonMask & 0x40) {
        if (D_00324510[0x28] != 0) {
            inputFlags |= 0x2000000;
        }
    }
    if (buttonMask & 0x80) {
        if (D_00324510[0x2B] != 0) {
            inputFlags |= 0x4000000;
        }
    }
    if (buttonMask & 0x40) {
        if (D_00324510[0x29] != 0) {
            inputFlags |= 0x8000000;
        }
    }
    if (inputFlags & MNU_INPUT_PRIORITY_BIT) {
        inputFlags = inputFlags & MNU_INPUT_PRIORITY_BIT;
    }
    return inputFlags;
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
        if (inputFlags & 0xF0) {
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

extern s8 D_0037CE40[];
extern u16 D_0037CE18[];
extern u16 D_0037CE1A[];

/* Match a half-open range, then return its signed mapping value; unmatched IDs return zero. */
s32 mnuLookupRangeEntry(u16 rangeId) {
    u16 *rangeBounds = D_0037CE18;
    s8 *mappingRows = D_0037CE40;
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
    return (alternate == 0) ? D_0037CE18[boundWordIndex] : D_0037CE1A[boundWordIndex];
}

/* For nonnegative indices, select among nonzero signed mapping values in row order. */
s32 mnuGetIndexedNonzeroEffect(s32 valueIndex) {
    s32 nonzeroCount = 0;
    s32 mappingIndex;
    s8 *mappingCursor = D_0037CE40;
    for (mappingIndex = 0; mappingIndex < 6; mappingIndex++, mappingCursor += 3) {
        s32 mappingValue = *mappingCursor;
        if (mappingValue != 0) nonzeroCount++;
        if (valueIndex == nonzeroCount - 1) return mappingValue;
    }
    return 0;
}

extern s8 D_0037CE42[];

/* Accumulate selected mapping values, then read the requested adjacent range bound. */
u16 mnuLookupPartyTableValue(u32 valueCount, s32 baseIndex, s32 alternate) {
    u32 valueIndex;
    s32 mappingSum = 0;

    for (valueIndex = 0; valueIndex < valueCount; valueIndex++) {
        mappingSum += mnuGetIndexedNonzeroEffect(valueIndex);
    }
    if (alternate == 0) {
        return D_0037CE18[D_0037CE42[(baseIndex + mappingSum) * 3]];
    }
    return D_0037CE1A[D_0037CE42[(baseIndex + mappingSum) * 3]];
}

/* Only secondary-kind-2 entries expose the paired value. */
u16 mnuGetSecondaryValueIfKind2(s32 commandId) {
    RangeEntry *command = (RangeEntry *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + datCommandRecords);

    if (command->secondaryKind != 2) {
        return 0;
    }
    return command->secondaryValue;
}

/* Return the native value/cost kind from the low-sixteen-bit command ID. */
u8 mnuGetRangeEntryKind(u32 commandId) {
    return ((RangeEntry *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + datCommandRecords))->kind;
}

/* HP-kind values use max HP as a percentage basis; other kinds retain the stored value. */
u16 mnuGetAdjustedEntryValue(s32 commandId, s32 actorAddress) {
    RangeEntry *command = (RangeEntry *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + datCommandRecords);
    u16 entryValue = command->value;
    u16 flatAddition = command->addition;
    if (mnuGetRangeEntryKind(commandId & MNU_COMMAND_ID_MASK) == MNU_COST_KIND_HP) {
        entryValue = flatAddition + ((DatPartyRecord *)actorAddress)->maxHp * entryValue / MNU_PERCENT_SCALE;
    }
    return entryValue;
}

u16 mnuGetAdjustedPartyRangeValue(s32 id) {
    s32 index = id & 0xFFFF;
    RangeEntry *record = (RangeEntry *)(index * 0x38 + datCommandRecords);
    u16 scale = record->value;
    u16 addition = record->addition;

    if (mnuGetRangeEntryKind(index) == 1) {
        s32 count = 0;
        s32 sum = 0;
        DatPartyRecord *entry = datGameState->party;
        s32 i;

        for (i = 0; i < 5; i++) {
            u16 flags = entry->flags;

            if ((flags & 1) && (flags & 2)) {
                count++;
                sum += entry->maxHp;
            }
            entry++;
        }
        if (count == 0) {
            return 0;
        }
        scale = addition + sum / count * scale / 100;
    }
    return scale;
}

/* Compare the stored raw HP/MP cost; equality is affordable and other kinds pass. */
s32 mnuCanAffordEntryCost(u16 commandId, s32 actorAddress) {
    u16 cost = ((RangeEntry *)datCommandRecords)[commandId].value;
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

/* Return -1 for insufficient raw cost, else 0 for flagged IDs below the boundary, or 1. */
s32 mnuGetEntryUseStatus(s32 actorAddress, u16 commandId) {
    if (mnuCanAffordEntryCost(commandId, actorAddress) == 0) {
        return -1;
    }
    if (!(((RangeEntry *)datCommandRecords)[commandId].flags & 1)) {
        return 1;
    }
    if (commandId < MNU_COMMAND_USE_STATUS_BOUNDARY) {
        return 0;
    }
    return 1;
}

/* Report insufficient raw HP/MP cost; equality and unhandled kinds return zero. */
s32 mnuIsEntryCostUnaffordable(u16 commandId, s32 actorAddress) {
    s32 costKind = ((RangeEntry *)datCommandRecords)[commandId].kind;
    u16 cost = ((RangeEntry *)datCommandRecords)[commandId].value;

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
    RangeEntry *command = (RangeEntry *)((commandId & MNU_COMMAND_ID_MASK) * MNU_COMMAND_RECORD_BYTES + datCommandRecords);
    u16 cost = command->value;

    switch (command->kind) {
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
    category = *(u8 *)(datCommandRecords + commandId * MNU_COMMAND_RECORD_BYTES + 8);
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

void func_002866B0(u16 ability, s32 target, DatPartyRecord *entry) {
    sdfApplyCommandResults(ability);
}

u32 func_002866C8(s32 context, s32 ability, s32 target, DatPartyRecord *entry) {
    return 0;
}

s32 ptySkillApplyFieldUseEffect(s32 context, u16 ability, s32 target, s32 selectedEntry) {
    DatPartyRecord *entry = (DatPartyRecord *)selectedEntry;
    s32 multiTarget = 0;
    s32 applied = 0;
    s32 mask;

    if (func_002866C8(context, ability, target, entry) != 0) {
        return 1;
    }

    if (mnuGetAbilityByteCategory(ability) == 1) {
        mask = mnuGetMatchingPartyEntryMask((s32)entry);

        if (func_002111A0(ability, mask) != 0) {
            return 0;
        }
        func_002866B0(ability, target, entry);
        func_00281780(context, mnuFindMatchingPartyEntryIndex((s32)entry), 0, 0);
    } else {
        s32 partyIndex;
        s32 queueArgument;

        for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++) {
            entry = &datGameState->party[partyIndex];
            if ((entry->flags & 1) != 0 && (entry->flags & 2) != 0) {
                mask = mnuGetMatchingPartyEntryMask((s32)entry);

                if (func_002111A0(ability, mask) == 0) {
                    func_002866B0(ability, target, entry);
                    applied = 1;
                }
            }
        }

        if (applied == 0) {
            return 0;
        }

        queueArgument = 0;
        for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++) {
            entry = &datGameState->party[partyIndex];
            if ((entry->flags & 1) != 0 && (entry->flags & 2) != 0) {
                func_00281780(context, mnuFindMatchingPartyEntryIndex((s32)entry), 0, queueArgument);
            }
            queueArgument += 3;
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

u16 mnuGetPartyEntryMenuValue(DatPartyRecord *entry);

/* Count inventory plus one for every matching slot value, without an occupancy test.
 * This counter accepts only IDs 0xA0..0xBE. */
s32 ptyCountBulletItem(s32 bulletId) {
    s32 totalCount;
    s32 partyIndex;

    if (bulletId < 0xA0) {
        return 0;
    }
    if (bulletId >= 0xBF) {
        return 0;
    }
    totalCount = datGameState->inventory.counts[bulletId];
    for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++) {
        if (bulletId == mnuGetPartyEntryMenuValue(&datGameState->party[partyIndex])) {
            totalCount++;
        }
    }
    return totalCount;
}

u32 mnuSetPartyEntryMenuValue(s32 entry, u16 value) {
    ((DatPartyRecord *)entry)->menuValue = value;
    return 1;
}

u16 mnuGetPartyEntryMenuValue(DatPartyRecord *entry) {
    return entry->menuValue;
}

extern u16 D_0037CE00[];
extern void dspStartEntry(s32);

/* Match an item ID, select its mode-dependent SE ID, and start it; id changes roles. */
s32 sndPlayPartyItemSe(u32 id, s32 soundMode) {
    u16 *soundRow = D_0037CE00;
    u32 rowIndex;

    for (rowIndex = 0; rowIndex < 3; rowIndex++, soundRow += 3) {
        if (id == soundRow[0]) {
            if (soundMode != 0) {
                id = soundRow[2];
                id += 0x17;
            } else {
                id = soundRow[1];
                id += 10;
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
s32 btlItemApplyPermanentBonus(u16 itemId, DatPartyRecord *unit) {
    s32 statIndex = -1;
    s32 accepted = 0;

    switch (itemId - 0x50) {
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

s32 btlItemApplyDirectEffect(s32 context, u16 item, s32 mode,
                             DatPartyRecord *unit) {
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


/* Return the first matching status index in native priority order, or -1. */
s32 mnuGetSelectionFromFlags(s32 actorAddress) {
    u16 statusFlags = ((DatPartyRecord *)actorAddress)->status;
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
    func_002878D8(-1);
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
    func_002878D8(-1);
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

/* Return the motion byte selected by the low half of the encoded entry index. */
u8 func_00287198(s32 encodedIndex) {
    return evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].motionIndex;
}

/* Return the indexed entry's animation frame. */
f32 func_002871C0(s32 encodedIndex) {
    return evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].frame;
}

/* Copy exactly three position components; any output W component is untouched. */
void func_002871E8(s32 encodedIndex, f32 *position) {
    position[0] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].position[0];
    position[1] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].position[1];
    position[2] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].position[2];
}

/* Copy exactly three stored rotation components. */
void func_00287220(s32 encodedIndex, f32 *rotation) {
    rotation[0] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].rotation[0];
    rotation[1] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].rotation[1];
    rotation[2] = evtStageTestState.entries[encodedIndex & EVT_STAGE_ENTRY_INDEX_MASK].rotation[2];
}

extern void *memcpy(void *dest, const void *src, u32 size);
extern u8 D_003B2520[];
extern u8 D_003B2530[];
extern u8 D_003B2540[];
extern u8 D_003B2550[];
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

    memcpy(firstColor, D_003B2520, sizeof(firstColor));
    memcpy(secondColor, D_003B2530, sizeof(secondColor));
    memcpy(backgroundColor, D_003B2540, sizeof(backgroundColor));
    memcpy(drawColor, D_003B2550, sizeof(drawColor));
    evtToggleSavedDrawVectors(0, 5.0f, 0.0f);
    kwlnSetLightColorTarget(0, 0, firstColor);
    kwlnSetLightDirectionTarget(0, 0, secondColor);
    kwlnSetBackgroundColorTarget(0, backgroundColor);
    kwlnSetDrawColorTarget(0, drawColor);
    evtSetDrawVectorTarget(0, 255.0f, 255.0f, 2000.0f, 30000.0f);
    sdfSceneProjectionParameters.fov = 0.7551905f;
    PCP_COPY_VECTOR(sdfViewTargetVector, D_0037CE70);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_0037CE60);
    PCP_COPY_VECTOR(sdfViewUpVector, D_0037CE80);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, sdfViewMatrix);
}

/* Set the horizontal projection offset while retaining the native GS-coordinate bias. */
void mnuSetStageTestCameraOffset(f32 offset) {
    sdfSceneProjectionParameters.offsetY = EVT_STAGE_PROJECTION_BIAS;
    sdfSceneProjectionParameters.offsetX = offset + EVT_STAGE_PROJECTION_BIAS;
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

/* Release the viewer's model/effect and discard its pending effect, except in mode 1. */
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

/* Stop the viewer and reset both projection offsets to the native coordinate bias. */
void mnuResetWorkFloats(void) {
    btlStopStage();
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

    if (evtStageTestState.model != 0 && evtStageTestState.queue.slot[0].modelId == (s32)evtStageTestState.entries[entryIndex].modelId) {
        return 0;
    }
    btlStopStage();
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

INCLUDE_ASM(const s32, "game/code_00282850", func_002877A8);

INCLUDE_ASM(const s32, "game/code_00282850", func_002878D8);

extern u8 *D_003BAA20;
extern void mdlStoreTertiaryVectorVU(void *);

/* Store uniform model scale through vf10 and return it; useTable selects the model-record factor. */
f32 mnuSetModelScaleVector(void *model, s32 useTable) {
    f32 scale = 1.0f;
    f32 scaleVector[4];

    if (useTable != 0) {
        scale = *(f32 *)(D_003BAA20 + evtStageTestState.queue.slot[0].modelId * EVT_STAGE_MODEL_RECORD_BYTES + 0x10);
    }
    scaleVector[0] = scale;
    scaleVector[1] = scale;
    scaleVector[2] = scale;
    scaleVector[3] = 1.0f;
    VU0_LOAD_VF(vf10, scaleVector);
    mdlStoreTertiaryVectorVU(model);
    return scale;
}

/* Reset target XYZ to (0, 0, -400) and eye XYZ to zero; leave their fourth words untouched. */
void mnuResetWorkPair(void) {
    *(s32 *)(D_0037CE70 + 0) = 0;
    *(s32 *)(D_0037CE70 + 4) = 0;
    *(f32 *)(D_0037CE70 + 8) = EVT_STAGE_DEFAULT_TARGET_Z;
    *(s32 *)(D_0037CE60 + 0) = 0;
    *(s32 *)(D_0037CE60 + 4) = 0;
    *(s32 *)(D_0037CE60 + 8) = 0;
}

extern void mdlStorePrimaryVectorVU(void *);

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
        mnuSetModelScaleVector((void *)model, 0);
        position[2] = ((StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries))->position[2];
        mnuResetWorkPair();
    } else {
        scale = mnuSetModelScaleVector((void *)model, 1);
        stageEntry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries);
        position[0] -= stageEntry->position[0] - stageEntry->position[0] * scale;
        position[1] -= stageEntry->position[1] - stageEntry->position[1] * scale;
        position[2] = 0.0f;
        *(f32 *)(D_0037CE70 + 8) = (EVT_STAGE_DEFAULT_TARGET_Z - stageEntry->position[2]) * scale;
    }
    VU0_LOAD_VF(vf10, position);
    mdlStorePrimaryVectorVU((void *)model);
}

/* Convert the active entry's degree angles to radians and update the model rotation basis. */
void evtStageTestApplyEntryRotation(s32 model) {
    StageTestEntry *stageEntry = (StageTestEntry *)(evtStageTestState.queue.slot[0].entryIndex * EVT_STAGE_ENTRY_BYTES + (s32)evtStageTestState.entries);

    func_002E7F20(stageEntry->rotation[0] * 3.14159265f / 180.0f, stageEntry->rotation[1] * 3.14159265f / 180.0f,
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
    PCP_COPY_VECTOR(sdfViewTargetVector, D_0037CE70);
    PCP_COPY_VECTOR(sdfViewEyeVector, D_0037CE60);
    PCP_COPY_VECTOR(sdfViewUpVector, D_0037CE80);
    sdfCameraBuildProjection(&sdfSceneProjectionParameters);
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
    VU0_STORE_VF_UNCLOBBERED(vf10, &target);
    sdfVuBuildLookAtBasis(&eye, &target, sdfViewUpVector);
    VU0_STORE_MATRIX_UNCLOBBERED(sdfViewMatrix);
    sdfConsBuildMatrixPacket(D_003270F0 + drawBufferIndex * 8000, &sdfSceneProjectionParameters, sdfViewMatrix);
    sdfConsCacheTransformedNode(&sdfSceneProjectionParameters, sdfViewMatrix);
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
void evtStageTestQueueMotionSegment(s32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames) {
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

/* Create the fixed-transform camera target and return its update callback. */
void *evtCreateBattleStageTestCamera(void) {
    f32 position[4] = {401.0f, -593.0f, -1208.25f, 0.0f};
    f32 orientation[4] = {0.22f, 0.12f, 0.03f, 1.0f};
    EffWorldNode *cameraTarget = evtCreateWorldObjectAtTransform(position, orientation);

    cameraTarget->value = (u32)D_003BC7C8;
    return func_00288458;
}

extern SdfPoolNode D_00325708;
extern s32 D_003BC7D0;
extern s32 D_003BC7D4;
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void kwlnDrawSpriteCell(void *, s32, s32, s32, s32);
extern void evtCreateWorldObjectForKey(s32, s32);

/* Draw the battle-stage selector; confirmation creates the selected world object
 * and returns its camera callback before the four trigger-bit ID adjustments. */
void *evtBattleStageTestScreen(void) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(EVT_STAGE_SCREEN_WORK_BYTES);

    sdfInitPacketList(packetList);
    kwlnDrawSpriteCell(packetList, 0x84, 0x46, 0x14, 9);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7840, 0x7BA0, 0xFEFFFF, 0, "BATTLE STAGE"));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7A80, 0x7C60, 0xFEFFFF, 6, "F%03d_%03d", D_003BC7D0, D_003BC7D4));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R = EVENT SELECT"));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7D80, 0xFEFFFF, 0, "RR  = ENTER"));
    D_00325708.append((SdfListHead *)&D_00325708, packetList);
    if (D_00324510[0x21] < 0) {
        evtCreateWorldObjectForKey(D_003BC7D0, D_003BC7D4);
        return evtCreateBattleStageTestCamera;
    }
    if (D_00324510[0x25] & MNU_PAD_TRIGGER_BIT) {
        D_003BC7D0++;
    }
    if (D_00324510[0x24] & MNU_PAD_TRIGGER_BIT) {
        D_003BC7D0--;
    }
    if (D_00324510[0x2A] & MNU_PAD_TRIGGER_BIT) {
        D_003BC7D4++;
    }
    if (D_00324510[0x28] & MNU_PAD_TRIGGER_BIT) {
        D_003BC7D4--;
    }
    return 0;
}

void evtDestroyBattleStageTestWorldNode(void) {
    evtDestroySecondaryWorldNode();
}

/* Destroy the named battle-stage-test task hierarchy and disable the debug graph. */
void evtBattleStageTestStopTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B2608, 1);
    kwlnDebugGraphSetEnabled(0);
}

/* Enable the debug graph and create the battle-stage selector task with its cleanup callback. */
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
        func_002EDC50(taskWork->payload);
        sdfReleaseChipBlock(taskWork->allocation);
        sdfReleaseChipBlock((void *)taskWork);
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

