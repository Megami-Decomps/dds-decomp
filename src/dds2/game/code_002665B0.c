#include "mnu.h"
#include "mnu_staff.h"
#include "kwln.h"
#include "evt_world.h"
#include "mnu_list.h"
#include "eff.h"
#include "dat_state.h"

#define MNU_PARTY_SLOT_COUNT 5
#define MNU_PARTY_RECORD_BYTES 0x1C4
#define MNU_PERCENT_PAIR_BYTES 0xA0
#define MNU_PERCENT_PANEL_BYTES 0x50
#define MNU_TERMINAL_SCENE_BYTES 0x3F8
#define MNU_MENU_HOST_BYTES 0xA82C
#define MNU_EFFECT_BATCH_COUNT 7
#define MNU_SELECTED_SLOT_COUNT 2
#define MNU_TEXT_DRAW_PRIORITY 0x52
#define MNU_TERMINAL_DEFAULT_BGM 0x20000
#define MNU_BGM_BANK_MASK 0xFFFF0000
#define MNU_RECOVERY_STATUS_KEEP_MASK 0xFA2F
#define MNU_TERMINAL_EXIT_PROCESS 0x322
#define MNU_COLOR_LOW_BYTE_MASK 0xFF

extern KwlnTask *kwlnTaskCreate();
extern void func_00101968(KwlnTask *, KwlnTask *);
extern s32 mnuPrepareTerminalPopupAndDispatch(s32);
extern s32 func_00268550(s32);
extern s32 func_00268588(s32);
extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);


extern void func_00266C08(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern s32 mnuFirstPresentMainCharacterIndex(void);
extern void evtCreateEventScriptProcess(s32);
extern void evtClearActiveFlag(s32);
extern void evtSetBoundedDisplayValue(s32, s32);
extern s8 D_00437859;

typedef struct MenuSlotState {
    s32 heapHandle;
    u8 pad04[4];
    MenuPopupState transitionWork; /* 0x08 */
    s32 dispatchStatus; /* 0x54 */
    u32 dispatchValue; /* 0x58 */
    EvtResourcePair messageResources; /* 0x5C */
    s32 resourceBank[4]; /* 0x64: encoded base-resource owners */
    EffectSlotSet *alternateBatch; /* 0x74 */
    struct MenuList *menuList;
    struct MenuList *progressList;
    struct MenuList *secondaryList;
    s32 reduced;    /* 0x84 */
    s32 slot;       /* 0x88 */
    u8 pad8C[0x18];
    s32 panelFade; /* 0xA4 */
    s32 effect[7]; /* 0xA8 */
    s32 selectedSlots[2]; /* 0xC4: current slot, then previous slot */
    u8 padCC[0x14];
    s32 slotCopy;   /* 0xE0 */
    s32 mode;       /* 0xE4 */
    u32 imageHandles[2]; /* 0xE8: native camp texture pair */
    u8 padF0[0x5C];
    s32 stage;      /* 0x14C */
    s32 panelHoldFrames; /* 0x150 */
    s32 bgmHandle;  /* 0x154: encoded bank/track handle */
    DspScrollingStripState panels[2]; /* 0x158 */
    MenuCampEffect campEffect; /* 0x280 */
    MenuGradientFade gradientFade; /* 0x3E8 */
    s32 reducedMode;     /* 0x3F4 */
} MenuSlotState;

typedef char MenuSlotState_size_check[(sizeof(MenuSlotState) == 0x3F8) ? 1 : -1];
typedef char MenuSlotState_bank0_check[((u32)&((MenuSlotState *)0)->resourceBank[0] == 0x64) ? 1 : -1];
typedef char MenuSlotState_bank1_check[((u32)&((MenuSlotState *)0)->resourceBank[1] == 0x68) ? 1 : -1];
typedef char MenuSlotState_bank2_check[((u32)&((MenuSlotState *)0)->resourceBank[2] == 0x6C) ? 1 : -1];
typedef char MenuSlotState_bank3_check[((u32)&((MenuSlotState *)0)->resourceBank[3] == 0x70) ? 1 : -1];
typedef char MenuSlotState_alternate_check[((u32)&((MenuSlotState *)0)->alternateBatch == 0x74) ? 1 : -1];
typedef char MenuSlotState_panel0_check[((u32)&((MenuSlotState *)0)->panels[0] == 0x158) ? 1 : -1];
typedef char MenuSlotState_panel1_check[((u32)&((MenuSlotState *)0)->panels[1] == 0x1EC) ? 1 : -1];
typedef char MenuSlotState_camp_check[((u32)&((MenuSlotState *)0)->campEffect == 0x280) ? 1 : -1];
typedef char MenuSlotState_gradient_check[((u32)&((MenuSlotState *)0)->gradientFade == 0x3E8) ? 1 : -1];
typedef char MenuSlotState_reduced_check[((u32)&((MenuSlotState *)0)->reducedMode == 0x3F4) ? 1 : -1];

extern void func_002665E8(MenuSlotState *);
extern u32 effLoadIndexedResource(s32, s32, s32);
extern void effResolveAndReleaseResource(u32 *);
/* Historical callers intentionally omit the second reset argument. */
extern void effReleaseSlotTextureReferencesAndResetWork();
extern EffectSlotSet *effCreateResourceSlotSet(u32 *, u32, u32);
extern void func_00266460(u32, MenuEffectResources *);
extern u32 effConfigureWithDefaultSetting(u32, u32, u32, u32, u32, u32);
extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *, s32, s32, s32, s32, s32);
extern s32 itfGridLookupValueOrDefault(EffectSlotSet *, s32);
extern void mnuCallInitWide(s32, s32, s32, s32, s32);
extern void mnuDrawCampIconBackdrop(MenuCampEffect *, s32);

/* The directory and strings are owned by code_00265AD8. */
extern const char D_00424E48[];
extern const char *D_003CE7A8[4];
extern const char *D_003CE7B8[]; /* Two entries in external .data, not small data. */
extern const char *D_003CE7C0[4];


extern void evtLoadResourcePair(u32, EvtResourcePair *);
extern void evtCreateMessageWindowIfMissing(s32);
extern void mnuSnapshotCampTextureHandles(u32 *);
extern void mnuBuildEligibleSlotList();
extern void mnuResetGradientFadeColor(MenuGradientFade *, s32);

extern s32 func_0035C860(char *, const char *, ...);
extern u32 uiBlendColors(u32, u32, u32);
extern u32 func_0019F5E8(s32, s32, s32, u32, char *, s32);
extern char mnuNumberSpriteFormat[];

typedef struct EffectPair {
    s32 firstValue;
    s32 secondValue;
} EffectPair;

extern EffectPair D_00437878[];
extern void itfDrawGridWithResolvedSlot(u32, u32, u32, u32, u32, u32, u32);

typedef struct EffectInner {
    u8 pad00[0x20];
    EffectPair *pair; /* 0x20 */
} EffectInner;

typedef struct EffectObject {
    u8 pad00[8];
    EffectInner *inner; /* 0x08 */
} EffectObject;

extern EffectObject *effCreateStatusBatch(s32);

extern s32 mdlFlagTest(u32);

extern s32 evtGetMessageWindowControlState(void);

extern s32 fldGetModeFrameRecordIndex(s32);

extern s32 kwlnFadeIsActive(void);

extern struct MenuListNode *func_002B86E8(struct MenuList *);

extern void mnuDrawTerminalBackdrop(s32);

extern s8 mnuTerminalTaskState;

extern u32 kwlnTaskGetUserValue();

extern void mnuDrawAndAdvanceProfilePanel(s32, s32, s32, MenuProfilePanel *, s32);

extern void mnuDrawAndStepGradientFade(MenuGradientFade *, s32);

extern void func_002C1B68(u32 *, u32);

extern s32 movAreTitleEffectsReady(s32, s32);

extern void mnuReleaseStaffMenuResources(s32);


extern s32 sdfAllocGeneralBlock(s32);

extern s32 sdfResourceRetainAddress(s32);

extern EffectList *mnuAllocateValueRecord(u32);


extern void mnuAppendCampSpriteRequests(s32, s32);

extern MenuProfilePanel *mnuCreateProfilePanel(DatPartyRecord *selectionState);

extern void mnuSetGroupProperties(MenuProfilePanel *, u32, u32, u32, u32);

extern void mnuDrawListPanels(s32, s32, s32, s32, s32, s32);

extern void func_002C16F0(s32, s32, s32, DatPartyRecord *, s32, s32, s32);

extern void mnuTerminalSetTrack(s8, s8);

extern void *mnuWalkNodeList(s32, struct MenuList *);

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_00424F00[];

extern const char D_00424F10[];

extern const char D_00424F20[];

extern KwlnTask *D_0043785C;

extern struct MenuList *mnuCreateListState();


extern u8 D_00437870[];

extern s32 sdfAllocSizeClassBlock(s32);

extern s32 mnuPercentOrHundred(u16, u16);

extern void mnuDrawPanelSequenceByRow(s32, s32, s32, s32, s32, s32);

extern s32 effDestroyPackedBatch(s32);
extern void mnuFreeProfilePanelWork(void *);


typedef struct MenuListNode MenuListNode;

typedef struct MenuList MenuList;




extern u8 D_003CE944[];

extern u32 mnuDestroyListState(struct MenuList *);

extern void mnuReleaseCampTextureHandlesAndClearOutput(u32 *);


/* Destroy the camp effect's retained animation without freeing the effect. */
void func_002665B0(MenuCampEffect *context) {
    effDestroyPackedBatch((s32)context->resources.animationHandle);
}

/* Return whether model flag 0x31 is set; its storyline meaning is not asserted. */
u8 func_002665C8() {
    s64 unlocked;

    unlocked = mdlFlagTest(0x31);
    return unlocked != 0;
}

/* Load the terminal's resource bank and configure its active panel layout. */
void func_002665E8(MenuSlotState *scene) {
    s32 i;

    scene->reducedMode = func_002665C8(scene);
    if (scene->reducedMode == 0) {
        for (i = 0; i < 3; i++) {
            scene->resourceBank[i] = (s32)effLoadIndexedResource(
                (s32)D_00424E48, (s32)D_003CE7A8[i], 1);
            effResolveAndReleaseResource((u32 *)scene->resourceBank[i]);
        }
        scene->resourceBank[3] = (s32)effLoadIndexedResource(
            (s32)D_00424E48,
            (s32)D_003CE7C0[mnuFirstPresentMainCharacterIndex()], 1);
        effResolveAndReleaseResource((u32 *)scene->resourceBank[3]);
        mnuInitScrollingStripState(&scene->panels[0], 0,
            (void *)scene->resourceBank[0], 0x46, 0x43);
        func_0026BE28(&scene->panels[0], 1, 0x10, 0x20);
        mnuInitScrollingStripState(&scene->panels[1], 0,
            (void *)scene->resourceBank[0], 0x46, 0x43);
        func_0026BE28(&scene->panels[1], 0, 0x10, 0x20);
        func_0026BEB0(&scene->panels[1], 0x1470, 0xCB8, 0);
        func_00266460((u32)scene->resourceBank[2], &scene->campEffect.resources);
    } else {
        for (i = 0; i < 2; i++) {
            scene->resourceBank[i] = (s32)effLoadIndexedResource(
                (s32)D_00424E48, (s32)D_003CE7B8[i], 1);
            effResolveAndReleaseResource((u32 *)scene->resourceBank[i]);
        }
        mnuInitScrollingStripState(&scene->panels[0], 1,
            (void *)scene->resourceBank[0], 0x46, 0x43);
        func_0026BE28(&scene->panels[0], 1, 0x10, 0x20);
        mnuInitScrollingStripState(&scene->panels[1], 1,
            (void *)scene->resourceBank[0], 0x46, 0x43);
        func_0026BE28(&scene->panels[1], 0, 0x10, 0x20);
        func_0026BEB0(&scene->panels[1], 0x1470, 0xCB8, 0);
    }
    scene->alternateBatch = effCreateResourceSlotSet(
        (u32 *)scene->resourceBank[0], 7, 1);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266808);

/* Release four resources in full mode, only the first two in reduced mode. */
void mnuReleaseResourceGroup(s32 address) {
    MenuSlotState *group = (MenuSlotState *)address;
    if (group->reducedMode == 0) {
        effResolveAndReleaseResource((u32 *)group->resourceBank[0]);
        effResolveAndReleaseResource((u32 *)group->resourceBank[1]);
        effResolveAndReleaseResource((u32 *)group->resourceBank[2]);
        effResolveAndReleaseResource((u32 *)group->resourceBank[3]);
        return;
    }
    effResolveAndReleaseResource((u32 *)group->resourceBank[0]);
    effResolveAndReleaseResource((u32 *)group->resourceBank[1]);
}

/* Release/reset the mode-dependent texture sets, preserving the first short-arity call. */
void mnuReleaseMenuResourceGroup(s32 address, u32 value) {
    MenuSlotState *group = (MenuSlotState *)address;
    if (group->reducedMode == 0) {
        effReleaseSlotTextureReferencesAndResetWork((u8 *)group->resourceBank[0]);
        effReleaseSlotTextureReferencesAndResetWork((u8 *)group->resourceBank[1], value);
        effReleaseSlotTextureReferencesAndResetWork((u8 *)group->resourceBank[2], value);
        effReleaseSlotTextureReferencesAndResetWork((u8 *)group->resourceBank[3], value);
        return;
    }
    effReleaseSlotTextureReferencesAndResetWork((u8 *)group->resourceBank[0]);
    effReleaseSlotTextureReferencesAndResetWork((u8 *)group->resourceBank[1], value);
}

/* Request the existing resource-group texture release with its extra value zero. */
void mnuReleaseResourceGroupTextureHandles(u32 address) {
    mnuReleaseMenuResourceGroup(address, 0);
}

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);
typedef struct FrFontGlyph FrFontGlyph;
extern u32 func_0019F6C8(s32, s32, s32, u32, char *, s32);
extern void frFontSetChainFlag(FrFontGlyph *, u8);
extern s32 func_0019D550(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern u32 mnuGetPanelRatioColor(s32, s32, s32);
extern void mnuDrawAndAdvanceRatioPanel(s32, s32, s32, u32, s32, s32, MenuPageBar *, u32);
extern s32 mnuGetSelectionFromFlags(DatPartyRecord *);
/* Fixed-width text rows used by both font drawing and message substitution.
 * The font helper decodes single-byte and two-byte characters from this data. */
typedef struct MenuTextEntry {
    u8 encodedText[32];
} MenuTextEntry;

extern MenuTextEntry D_003A41A8[];
extern MenuTextEntry D_003A47E8[];

/* Select an encoded text row, draw it at the supplied grid cell, and queue the glyph.
 * The signed-byte slot is not bounds checked; DDS2 subtracts 0x120 from gridX. */
void mnuQueueFontGlyphFromSelectedAtlasSlot(s32 gridX, s32 gridY, s32 depth, s32 value, s8 slot, s8 alternate) {
    u8 *text;
    FrFontGlyph *handle;

    if (alternate == 0) {
        text = D_003A41A8[slot].encodedText;
    } else {
        text = D_003A47E8[slot].encodedText;
    }
    handle = (FrFontGlyph *)itfCreateConvertedTextGlyph(gridX - 0x120, gridY, depth, value, text, 0);
    func_0019D550(handle, 1, MNU_TEXT_DRAW_PRIORITY);
    frFontQueueGlyphInSelectedSlot(handle);
}


/* Price recovery from missing HP/MP plus the five charged status bits.
 * Preserve DDS2's arithmetic and separate truncations; deficits are not clamped. */
s32 mnuTerminalScoreBox(DatPartyRecord *unit) {
    f32 missingMp = unit->maxMp - unit->mp;
    f32 missingHp = unit->maxHp - unit->hp;
    s32 statusCost = 0;

    if (unit->status & 0x400) {
        statusCost = 100;
    }
    if (unit->status & 0x100) {
        statusCost += 50;
    }
    if (unit->status & 0x80) {
        statusCost += 100;
    }
    if (unit->status & 0x40) {
        statusCost += 100;
    }
    if (unit->status & 0x10) {
        statusCost += 100;
    }
    return (s32)(missingHp * 1.8f) + (s32)(missingMp * (missingMp / 40.0f + 5.0f)) + statusCost;
}

/* Mark nodes unaffordable; the secondary payload word carries recovery cost. */
void mnuRefreshThresholdNodeFlags(MenuList *list) {
    MenuListNode *node = list->first;
    if (node != 0) {
        DatGameState *base = datGameState;
        do {
            u32 currency = (u32)base->header.currency;
            if (currency < (u32)node->terminal.requiredAmount) {
                node->flags48 |= 1;
            } else {
                node->flags48 &= ~1u;
            }
            node = node->next;
        } while (node != 0);
    }
}

/* Draw formatted numeric text using a blend toward the color with its low byte clear. */
void mnuCreateNumberSprite(s32 x, s32 y, s32 layer, s32 blendWeight, s32 number, u32 color, s32 priority) {
    char text[16];
    FrFontGlyph *sprite;

    func_0035C860(text, mnuNumberSpriteFormat, number);
    sprite = (FrFontGlyph *)func_0019F5E8(x, y, layer, uiBlendColors(color, color & ~MNU_COLOR_LOW_BYTE_MASK, blendWeight), text, 0);
    func_0019D550(sprite, 1, priority);
    frFontQueueGlyphInSelectedSlot(sprite);
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424E60);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266C08);

/* Allocate adjacent HP/MP percentage panels, preserving the native 0x50 stride.
 * The source is a party-vitals record; the context supplies the retained camp texture. */
s32 mnuCreateDualPercentPanel(DatPartyRecord *unit, MenuSlotState *host) {
    s32 panel = sdfAllocSizeClassBlock(MNU_PERCENT_PAIR_BYTES);
    mnuDrawPanelSequenceByRow(panel, 0, 0, 0x1e,
        mnuPercentOrHundred(unit->hp, unit->maxHp),
        host->imageHandles[1]);
    mnuDrawPanelSequenceByRow(panel + MNU_PERCENT_PANEL_BYTES, 1, 0, 0x1e,
        mnuPercentOrHundred(unit->mp, unit->maxMp),
        host->imageHandles[1]);
    return panel;
}

/* Release both texture sets and the backing allocation for a nonzero panel pair. */
void mnuReleaseDualPercentPanel(s32 panel) {
    if (panel != 0) {
        mnuReleaseSpriteTextures((u32 *)panel);
        mnuReleaseSpriteTextures((u32 *)(panel + MNU_PERCENT_PANEL_BYTES));
        sdfReleaseChipBlock(panel);
        return;
    }
}

/* Rebuild each progress node's HP/MP percentage panel from its party slot. */
void mnuCreateThresholdNodePanels(MenuSlotState *host) {
    MenuListNode *node = host->progressList->first;
    while (node != 0) {
        s32 partyIndex = node->terminal.entryId;
        node->childPanel =
            mnuCreateDualPercentPanel(&datGameState->party[partyIndex], host);
        node = node->next;
    }
}

/* Release each progress node's child panel, leaving the nodes/list intact. */
void mnuDestroyThresholdNodePanels(MenuSlotState *host) {
    MenuListNode *node;

    for (node = host->progressList->first; node != 0; node = node->next) {
        mnuReleaseDualPercentPanel(node->childPanel);
    }
}


extern void mnuBuildTerminalNodeList(MenuSlotState *);

INCLUDE_ASM(const s32, "game/code_002665B0", mnuBuildTerminalNodeList);

/* Destroy the progress-list allocation retained by the terminal work. */
void mnuReleaseProgressWorkList(MenuSlotState *host) {
    mnuDestroyListState(host->progressList);
}

/* Release the selected recovery panel, then pass its owning list to the follow-up. */
void mnuReleaseSelectedProgressPanel(MenuSlotState *host) {
    mnuReleaseDualPercentPanel(host->progressList->cursor->childPanel);
    func_002B86E8(host->progressList);
}

extern void func_00267238();
extern u32 mnuBlendListNodeColorByFlags(u32, MenuListNode *);
void func_00267238(s32 x, s32 y, s32 unused, MenuList *list, MenuListNode *node, s32 priority) {
    s32 width = list->scale;
    MenuSlotState *host = (MenuSlotState *)list->context;
    s32 isCurrent = node == list->cursor;
    u32 chainFlags = 0;
    u32 color;
    FrFontGlyph *glyph;

    if (node->flags48 & 1) {
        width /= 2;
    }
    if (isCurrent) {
        func_00306CD0(x, y, 0, width, 0, (EffectSlotSet *)host->resourceBank[0], 0x18, priority);
        chainFlags = 4;
    }
    color = (node->flags48 & 1) ? 0xA09DC320 : 0xA09DC380;
    color = mnuBlendListNodeColorByFlags(color, node);
    color = uiBlendColors(color, color & ~0xFF, width);
    glyph = (FrFontGlyph *)itfCreateConvertedTextGlyph(x + 0xF0, y, 0, color, (const u8 *)node->title, 0);
    frFontSetChainFlag(glyph, chainFlags);
    func_0019D550(glyph, 1, priority);
    frFontQueueGlyphInSelectedSlot(glyph);
}


typedef struct MenuSlotKind {
    s16 kind;
    s16 unk2;
} MenuSlotKind;

extern MenuSlotKind D_0038A3B8[];

/* Same slot kind, or both kinds in the 0xF/0x1D/0x1E group. */
s32 mnuSlotKindsInSameGroup(s32 index, s32 kind) {
    s16 current = D_0038A3B8[index].kind;

    if (kind == current) {
        return 1;
    }
    if (kind == 0xF || kind == 0x1D || kind == 0x1E) {
        if (current == 0xF) {
            return 1;
        }
        if (current == 0x1D) {
            return 1;
        }
        if (current == 0x1E) {
            return 1;
        }
    }
    return 0;
}

extern MenuSlotKind D_0038A480[];
void mnuBuildEligibleSlotList(MenuSlotState *host) {
    MenuList *list;
    s32 kind;
    s32 i;

    list = mnuCreateListState(0, 7, 0x16);
    list->scale = 0;
    list->drawCallback = func_00267238;
    list->context = host;
    host->secondaryList = list;
    if (host->reduced == 0) {
        kind = D_0038A3B8[host->slot].kind;
    } else {
        kind = D_0038A480[host->slot].kind;
    }
    for (i = 0; i < 50; i++) {
        char *name;
        MenuListNode *node;

        if (host->reduced == 0 && host->slot == i) {
            continue;
        }
        if (mdlFlagTest(D_0038A3B8[i].unk2) == 0 &&
            D_0038A3B8[i].unk2 != 0) {
            continue;
        }
        if (mnuSlotKindsInSameGroup(i, kind) == 0) {
            continue;
        }
        name = (char *)D_003A41A8[i].encodedText;
        if (strlen(name) != 0) {
            node = mnuListAppendNode(host->secondaryList, D_00437870);
            node->camp.value = i;
            node->title = name;
        }
    }
}

u32 func_002674F8(void) {
    return 0;
}

/* Draw the command node, dimming flagged rows and marking the current selection. */
void mnuThresholdNodeDrawCallback(s32 x, s32 y, s32 unused, MenuList *list, MenuListNode *node, s32 priority) {
    s32 width = list->scale;
    MenuSlotState *host = (MenuSlotState *)list->context;
    s32 index = node->camp.value;
    s32 isCurrent = node == list->cursor;

    if (node->flags48 & 1) {
        width /= 2;
    }
    if (isCurrent) {
        func_00306CD0(x, y, 0, width, 0, (EffectSlotSet *)host->resourceBank[0], 0x16, priority);
        index += 1;
    }
    func_00306CD0(x + 0x50, y - 0x10, 0, width, 0, (EffectSlotSet *)host->resourceBank[0], index, priority);
}

/* Omit the input-array position `excluded`, not all entries with that same value. */
MenuList *mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, void *owner) {
    MenuList *list = mnuCreateListState(0, count, 0x16, owner);
    s32 entryIndex;
    list->context = owner;
    list->drawCallback = mnuThresholdNodeDrawCallback;
    list->scale = 0;
    for (entryIndex = 0; entryIndex < count; entryIndex++) {
        if (entryIndex != excluded) {
            MenuListNode *node = mnuListAppendNode(list, D_00437870);
            node->camp.value = items[entryIndex];
        }
    }
    return list;
}

/* Mark the selected command-list node only for modes zero/one and an idle owner. */
void mnuHighlightProgressNodeFromOwnerSelection(MenuSlotState *host) {
    s32 state = host->reduced;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (host->secondaryList->count == 0) {
            MenuListNode *selected = mnuWalkNodeList(2 - func_002674F8(),
                                              host->menuList);
            selected->flags48 |= 1;
        }
    }
}

/* Highlight the mode-zero or mode-two command node only while the progress list is idle. */
void mnuHighlightProgressNodeByMode(MenuSlotState *host) {
    s32 state = host->reduced;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_002674F8();
    }
    if (host->progressList->count == 0) {
        MenuListNode *node = mnuWalkNodeList(selectedIndex, host->menuList);
        node->flags48 |= 1;
    }
}

/* Build the mode-specific command list and the separate party recovery list.
 * DDS2 retains its native literal table and exclusion predicate. */
void mnuTerminalBuildMenus(MenuSlotState *host) {
    s32 table[3][5] = {
        {0xB, 0xD, 0xF, 0x11, 0x13},
        {0xB, 0xD, 0x2E, 0x30, 0},
        {0x11, 0x30, 0, 0, 0},
    };
    s32 row;
    s32 count;
    s32 excluded = -1;

    switch (host->reduced) {
    case 0:
        row = 0;
        count = 5;
        if (func_002674F8() != 0) {
            excluded = 1;
        }
        break;
    case 1:
        row = 1;
        count = 4;
        if (func_002674F8() != 0) {
            excluded = 1;
        }
        break;
    default:
        row = 2;
        count = 2;
        break;
    }
    host->menuList = mnuBuildThresholdNodeList(table[row], count, excluded, host);
    mnuBuildTerminalNodeList(host);
    mnuSnapshotCampTextureHandles(host->imageHandles);
    mnuCreateThresholdNodePanels(host);
    mnuBuildEligibleSlotList(host);
    mnuHighlightProgressNodeFromOwnerSelection(host);
    mnuHighlightProgressNodeByMode(host);
}

/* Release command/progress lists, child percentage panels and camp texture handles.
 * Preserve their existing order and the single-iteration list loop. */
void mnuReleaseWorkResources(u8 *work) {
    MenuSlotState *host = (MenuSlotState *)work;
    u32 i;

    for (i = 0; i < 1; i++) {
        mnuDestroyListState((&host->menuList)[i]);
    }
    mnuDestroyThresholdNodePanels(host);
    mnuReleaseCampTextureHandlesAndClearOutput(host->imageHandles);
    mnuReleaseProgressWorkList(host);
    mnuDestroyListState(host->secondaryList);
}

/* Close via the native fade/script choice, including DDS2's extra party/flag gates.
 * All paths clear active flag zero and set display slot one. */
void mnuTerminalFadeOrClose(s32 flag, s32 scene) {
    if (flag == 0) {
        s32 state = ((MenuSlotState *)scene)->reduced;

        if (state < 3) {
            if (state > 0) {
                kwlnFadeOutStart(0, 0, 0, 0xF);
            } else if (mdlFlagTest(0x429) != 0 || mnuFirstPresentMainCharacterIndex() != 0 || func_002665C8(scene) != 0) {
                kwlnFadeOutStart(0, 0, 0, 0xF);
            } else {
                evtCreateEventScriptProcess(MNU_TERMINAL_EXIT_PROCESS);
            }
        } else if (mdlFlagTest(0x429) != 0 || mnuFirstPresentMainCharacterIndex() != 0 || func_002665C8(scene) != 0) {
            kwlnFadeOutStart(0, 0, 0, 0xF);
        } else {
            evtCreateEventScriptProcess(MNU_TERMINAL_EXIT_PROCESS);
        }
    } else {
        evtCreateEventScriptProcess(MNU_TERMINAL_EXIT_PROCESS);
    }
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(1, 1);
}

/* Return to mode zero and copy the owner's selected entry into the saved slot. */
void mnuResetProgressModeFromOwner(u8 *work) {
    MenuSlotState *host = (MenuSlotState *)work;
    MenuList *owner = host->secondaryList;
    host->reduced = 0;
    host->slot = owner->cursor->camp.value;
}

/* Allocate/zero the progress host, retain its allocation, and begin resource setup. */
MenuProgressHost *mnuCreateProgressHost(void) {
    s32 heap = sdfAllocGeneralBlock(MNU_MENU_HOST_BYTES);
    MenuProgressHost *host = (MenuProgressHost *)sdfResourceRetainAddress(heap);
    memset((void *)host, 0, MNU_MENU_HOST_BYTES);
    host->heapHandle = heap;
    host->titleEffectHandle = mnuAllocateValueRecord(1);
    mnuInitPartyPanelSlots(&host->partyPanel);
    mnuAppendCampSpriteRequests(host->titleEffectHandle, (s32)host + 8);
    host->loadState = 1;
    return host;
}

/* Release staff/title texture work before the value record and allocation. */
void mnuReleaseStaffAndTitleVisualResources(MenuProgressHost *host) {
    mnuReleaseStaffMenuTextureHandles((s32)&host->staffSlots);
    mnuReleaseTitleEffectSprites(&host->staffSlots);
    func_00303D58(host->titleEffectHandle);
    sdfReleaseResourceAllocation(host->heapHandle);
}

/* Return one while initialization is pending (including state zero), zero when ready.
 * On resource readiness, release loading resources and store state two. */
s32 mnuPollTitleEffectsReady(MenuProgressHost *host) {
    s32 state = host->loadState;
    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    if (movAreTitleEffectsReady(host->titleEffectHandle, (s32)host + 8) == 0) {
        return 1;
    }
    mnuReleaseStaffMenuResources((s32)host + 8);
    host->loadState = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267B40);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267C48);

/* Create/configure the profile-panel effect only when its retained handle is zero. */
void mnuEnsureProfilePanelEffect(DatPartyRecord *selectionState, MenuProgressHost *host) {
    if (host->currentEffect == 0) {
        MenuProfilePanel *effect = mnuCreateProfilePanel(selectionState);
        host->currentEffect = effect;
        mnuSetGroupProperties(effect, host->staffSlots.baseResources[0], (u32)host->staffSlots.pairResources[1], 1, 2);
    }
}

/* Release the retained profile panel and clear its handle. */
void mnuCloseCurrentProfilePanel(MenuProgressHost *host) {
    mnuFreeProfilePanelWork(host->currentEffect);
    host->currentEffect = 0;
}

/* Draw/advance the retained profile panel with the caller's coordinates, mode, and option. */
void mnuDrawCurrentProfilePanel(s32 x, s32 y, s32 mode, MenuProgressHost *host, s32 option) {
    mnuDrawAndAdvanceProfilePanel(x, y, mode, host->currentEffect, option);
}

/* Draw loaded progress panels only in state two; preserve the accumulated draw flags. */
s32 mnuDrawLoadedProgressPanels(DatPartyRecord *resource, MenuProgressHost *host, s32 mode) {
    if (host->loadState != 2) {
        return 0;
    }
    host->partyWindow.flags |= 0x280;
    mnuDrawListPanels(0, 0, 0, resource->profileId, (s32)&host->partyWindow, mode);
    func_002C16F0(0, 0, 0, resource, resource->profileId,
                   (s32)host->effectResource, mode);
    return 1;
}

typedef struct MenuFadeWork {
    u8 pad00[0x84];
    s32 reduced;      /* 0x84 */
    u8 pad88[0xCC];
    s32 bgmHandle;    /* 0x154: encoded bank/track handle */
} MenuFadeWork;

extern void sndStartTrackExtended(s32);
extern void func_00342580(s32);

/* Mode one optionally starts the BGM and releases the group; other modes use the
 * counterpart sound action and texture release. Only enable == 1 sends sound calls.
 * Preserve the identical retail arms and the independent draw-gate update. */
void mnuTerminalSetTrack(s8 mode, s8 enable) {
    s32 address = kwlnTaskGetUserValue(D_0043785C);
    MenuFadeWork *work = (MenuFadeWork *)address;

    if (mode == 1) {
        if (enable == 1) {
            /* Both arms are identical in retail; kept as written. */
            if (work->reduced == 0) {
                sndStartTrackExtended(work->bgmHandle);
            } else {
                sndStartTrackExtended(work->bgmHandle);
            }
        }
        D_00437859 = 1;
        mnuReleaseResourceGroup(address);
    } else {
        if (enable == 1) {
            func_00342580(work->bgmHandle);
        }
        D_00437859 = 0;
        mnuReleaseMenuResourceGroup(address, 1);
    }
}

/* Apply the supplied terminal track mode with enable fixed to one. */
void mnuEnableTerminalTrackMode(s8 index) {
    mnuTerminalSetTrack(index, 1);
}

/* Allocate seven effect batches and seed their two opaque parameter words.
 * EffectPair also carries drawing positions elsewhere, so its fields stay role-neutral. */
void mnuTerminalCreateEffects(MenuSlotState *state) {
    EffectObject *obj;

    obj = effCreateStatusBatch(1);
    state->effect[0] = (s32)obj;
    obj->inner->pair->firstValue = 0x14;
    obj->inner->pair->secondValue = 1;
    obj = effCreateStatusBatch(1);
    state->effect[1] = (s32)obj;
    obj->inner->pair->firstValue = 0xF;
    obj->inner->pair->secondValue = 0;
    obj = effCreateStatusBatch(8);
    state->effect[2] = (s32)obj;
    obj->inner->pair->firstValue = 6;
    obj->inner->pair->secondValue = 1;
    obj = effCreateStatusBatch(8);
    state->effect[3] = (s32)obj;
    obj->inner->pair->firstValue = 6;
    obj->inner->pair->secondValue = 0;
    obj = effCreateStatusBatch(1);
    state->effect[4] = (s32)obj;
    obj->inner->pair->firstValue = 6;
    obj->inner->pair->secondValue = 1;
    obj = effCreateStatusBatch(1);
    state->effect[5] = (s32)obj;
    obj->inner->pair->firstValue = 6;
    obj->inner->pair->secondValue = 0;
    obj = effCreateStatusBatch(1);
    state->effect[6] = (s32)obj;
    obj->inner->pair->firstValue = 0x78;
    obj->inner->pair->secondValue = 0;
}

/* Destroy every retained effect batch; the slots and terminal allocation are not cleared. */
void mnuDestroyAllMenuSlotEffectBatches(s32 object) {
    s32 *batch = (s32 *)(object + 0xA8);
    u32 i;

    for (i = 0; i < MNU_EFFECT_BATCH_COUNT; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

extern s32 fldGetCurrentBgmHandle(void);
extern void sndEnsureMidiBankResident(u32);

/* Select the native default/current BGM handle and make only its bank bits resident.
 * DDS2's default has a zero low halfword; DDS1's default includes track one. */
void mnuTerminalSelectResourceBank(MenuSlotState *host) {
    if (host->reduced == 0) {
        host->bgmHandle = MNU_TERMINAL_DEFAULT_BGM;
    } else {
        host->bgmHandle = fldGetCurrentBgmHandle();
    }
    sndEnsureMidiBankResident(host->bgmHandle & MNU_BGM_BANK_MASK);
}

extern void func_003425B0(void);
extern void func_003425D8(void);

/* Mode zero starts the selected BGM in full mode or invokes the reduced-mode action.
 * Other modes use their counterpart actions; keep the native selection field. */
void mnuApplyFadeTrackMode(s32 mode, MenuSlotState *host) {
    if (mode == 0) {
        if (host->reduced == 0) {
            sndStartTrackExtended(host->bgmHandle);
        } else {
            func_003425B0();
        }
    } else if (host->reduced == 0) {
        func_00342580(host->bgmHandle);
    } else {
        func_003425D8();
    }
}

/* Allocate the terminal scene and its lists/effects/message resource.
 * Both cursor slots start at -1; DDS2 also initializes its gradient indicator. */
u8 *mnuTerminalCreateScene(s32 reduced, s32 slot) {
    s32 handle;
    u8 *obj;
    u32 i;

    handle = sdfAllocGeneralBlock(MNU_TERMINAL_SCENE_BYTES);
    obj = (u8 *)sdfResourceRetainAddress(handle);
    memset(obj, 0, MNU_TERMINAL_SCENE_BYTES);
    *(s32 *)obj = handle;
    mnuClearPanelTransitionState(&((MenuSlotState *)obj)->transitionWork);
    mnuTerminalCreateEffects((MenuSlotState *)obj);
    ((MenuSlotState *)obj)->reduced = reduced;
    ((MenuSlotState *)obj)->mode = reduced;
    ((MenuSlotState *)obj)->slot = slot;
    ((MenuSlotState *)obj)->slotCopy = slot;
    mnuTerminalBuildMenus((MenuSlotState *)obj);
    evtLoadResourcePair((u32)"/facility/msg/terminal/mes_data.bmd", &((MenuSlotState *)obj)->messageResources);
    evtCreateMessageWindowIfMissing(((MenuSlotState *)obj)->messageResources.unk04);
    for (i = 0; i < MNU_SELECTED_SLOT_COUNT; i++) {
        ((MenuSlotState *)obj)->selectedSlots[i] = -1;
    }
    ((MenuSlotState *)obj)->panelHoldFrames = 0xF;
    mnuTerminalSelectResourceBank((MenuSlotState *)obj);
    mnuApplyFadeTrackMode(0, (MenuSlotState *)obj);
    mnuResetGradientFadeColor(&((MenuSlotState *)obj)->gradientFade, 0x60);
    return obj;
}

extern void func_00266808(u32 *work);
extern s32 dspCloseChannel(void);
extern void evtReleaseResourcePairHandle(EvtResourcePair *record);
extern void sdfReleaseResourceAllocation(s32 handle);
extern s32 mnuCheckResourceTask(void);
extern void mnuStopResourceTask(void);
extern void func_001285E8(s32 a, s32 b);
extern void fldProcessDeferredSceneCommand(void);

/* Release the terminal task's resources/effects, then hand its mode/slot to the field.
 * Native final work-field reads remain after allocation release and outside the NULL guard. */
void mnuReleaseTerminalWorkAndResumeField(s32 arg) {
    MenuSlotState *work = (MenuSlotState *)kwlnTaskGetUserValue();

    if (work != NULL) {
        mnuReleaseWorkResources((u8 *)work);
        func_00266808((u32 *)work);
        mnuDestroyAllMenuSlotEffectBatches((s32)work);
        mnuDrainPanelTransitions(&work->transitionWork, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(&work->messageResources);
        sdfReleaseResourceAllocation(work->heapHandle);
        mnuTerminalTaskState = 2;
    }
    if (mnuCheckResourceTask() != 0) {
        mnuStopResourceTask();
    }
    func_001285E8(work->reduced, work->slot);
    fldProcessDeferredSceneCommand();
}

/* Draw/step the task's gradient indicator, then select its message-control mode. */
s32 mnuUpdateTerminalMessageWindowIndicator(void) {
    MenuGradientFade *context = &((MenuSlotState *)kwlnTaskGetUserValue())->gradientFade;
    mnuDrawAndStepGradientFade(context, 0x53);
    if (evtGetMessageWindowControlState() != 0) {
        func_002C1B68(&context->active, 1);
    } else {
        func_002C1B68(&context->active, 0);
    }
    return 0;
}

/* Create terminal update/draw/exit tasks plus DDS2's parented term_fade task. */
INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F00);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F10);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F20);

void func_00268380(s32 reduced, s32 slot) {
    u8 *scene = mnuTerminalCreateScene(reduced, slot);
    KwlnTask *drawTask;
    KwlnTask *fadeTask;

    D_0043785C = kwlnTaskCreate(D_00424F00, 0x404, 1, 1,
        mnuPrepareTerminalPopupAndDispatch, NULL, (u32)scene);
    drawTask = kwlnTaskCreate(D_00424F10, 0x2B14, 1, 1,
        func_00268550, NULL, (u32)scene);
    kwlnTaskCreate(D_00424F20, 0x5210, 1, 1,
        func_00268588, mnuReleaseTerminalWorkAndResumeField, (u32)scene);
    fadeTask = kwlnTaskCreate("term_fade", 0x2B15, 1, 0,
        mnuUpdateTerminalMessageWindowIndicator, NULL, (u32)scene);
    func_00101968(drawTask, fadeTask);
    mnuTerminalTaskState = 1;
}


/* Destroy the three named terminal tasks and clear the retained task handle. */
void fldStopSceneTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00424F00, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F10, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F20, 0);
    D_0043785C = 0;
}

/* Report one only for active state one; consume completed state two by clearing it. */
s32 fldPollSceneState(void) {
    s32 state = mnuTerminalTaskState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        mnuTerminalTaskState = 0;
    }
    return 0;
}

/* Seed the task's popup slot before dispatching mode zero with the supplied request. */
s32 mnuPrepareTerminalPopupAndDispatch(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuSetPopupEntry((s32 *)(context + 0x54), D_003CE944);
    return menuSetHandler((void *)context, 0, (void *)request);
}

/* Dispatch current task work through panel mode one; distinct callback role unknown. */
s32 func_00268550(s32 request) {
    return menuSetHandler((void *)kwlnTaskGetUserValue(), 1, (void *)request);
}

/* Dispatch current task work through panel mode two; distinct callback role unknown. */
s32 func_00268588(s32 request) {
    return menuSetHandler((void *)kwlnTaskGetUserValue(), 2, (void *)request);
}

/* Require the fade to be inactive before testing message-window control for idle. */
s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

extern char D_003CE848[];

s32 func_002685F0(s32 action, s32 context) {
    switch (action) {
    case 3:
        if (*(s32 *)(*(s32 *)(context + 0x7C) + 0x20) == 0) {
            *(u8 **)(context + 0x58) = D_003CE848;
            mnuSetPopupEntry((s32 *)(context + 0x54), D_003CE848 + 0xC4);
            *(s32 *)(context + 0x90) = 1;
            **(u32 **)(context + 0x54) |= 0x20000;
            return 1;
        }
        break;
    case 2:
        if (*(s32 *)(*(s32 *)(context + 0x80) + 0x20) == 0) {
            *(u8 **)(context + 0x58) = D_003CE848;
            mnuSetPopupEntry((s32 *)(context + 0x54), D_003CE848 + 0xC4);
            *(s32 *)(context + 0x90) = 2;
            **(u32 **)(context + 0x54) |= 0x20000;
            return 1;
        }
        break;
    }
    return 0;
}

/* Restore current HP/MP to their stored maxima and clear exactly the charged status bits.
 * No boosted-max calculation or range validation is performed here. */
void fldSaveSceneOptionsAndClearFlags(DatPartyRecord *option) {
    u16 statusFlags = option->status;
    u16 maxHp = option->maxHp;
    u16 maxMp = option->maxMp;
    u16 retainedStatus = statusFlags & MNU_RECOVERY_STATUS_KEEP_MASK;

    option->hp = maxHp;
    option->mp = maxMp;
    option->status = retainedStatus;
}


/* Draw and animate the mode-dependent terminal backdrop. */
void mnuDrawTerminalBackdrop(s32 address) {
    MenuSlotState *state = (MenuSlotState *)address;
    EffectSlotSet *bank;
    BdWork *work;
    s32 resource;

    if (state->stage == 0 || D_00437859 == 0) {
        return;
    }
    if (state->reducedMode == 0) {
        switch (state->mode) {
        case 0:
            func_00306CD0(0, 0, 0, 0x100, 0, (EffectSlotSet *)state->resourceBank[1], 0, MNU_TEXT_DRAW_PRIORITY);
            bank = (EffectSlotSet *)state->resourceBank[3];
            bank->workEntries[0].geometry.angleDegrees = 90.0f;
            func_00306CD0(0x7B0, 0x698, 0, 0x100, 2, (EffectSlotSet *)state->resourceBank[3], 0, MNU_TEXT_DRAW_PRIORITY);
            break;
        case 1:
        case 2:
            mnuDrawCampIconBackdrop(&state->campEffect, MNU_TEXT_DRAW_PRIORITY);
            break;
        }
    } else {
        func_00306CD0(0, 0, 0, 0x100, 0, (EffectSlotSet *)state->resourceBank[1], 0, MNU_TEXT_DRAW_PRIORITY);
        resource = state->resourceBank[1];
        func_00306CD0(0x60, -0x30, 0, 0x100, 0, (EffectSlotSet *)resource, 1, MNU_TEXT_DRAW_PRIORITY);
        bank = (EffectSlotSet *)resource;
        work = &bank->workEntries[1];
        work->geometry.angleDegrees += 0.6f;
        if (work->geometry.angleDegrees > 360.0f) {
            work->geometry.angleDegrees -= 360.0f;
        }
    }
}


void mnuDrawAndStepTerminalPanelFade(s32 decrement, MenuSlotState *state) {
    if (D_00437859 == 0) {
        return;
    }
    if (state->reducedMode != 0) {
        state->panelHoldFrames = 0;
    } else {
        switch (state->mode) {
        case 0:
            func_00306CD0(0, 0, 0, state->panelFade, 1,
                         (EffectSlotSet *)state->resourceBank[1], 1, 0x52);
            ((EffectSlotSet *)state->resourceBank[3])->workEntries[1].geometry.angleDegrees = 90.0f;
            func_00306CD0(0x7B0, 0x698, 0, state->panelFade, 3,
                         (EffectSlotSet *)state->resourceBank[3], 1, 0x52);
            if (state->panelHoldFrames > 0) {
                state->panelHoldFrames--;
                return;
            }
            break;
        case 1:
        case 2:
            state->panelHoldFrames = 0;
            break;
        }
    }
    if (decrement == 0) {
        if (state->panelFade < 0x100) {
            state->panelFade += 12;
        }
        if (state->panelFade > 0x100) {
            state->panelFade = 0x100;
        }
    } else {
        if (state->panelFade > 0) {
            state->panelFade -= 0x11;
        }
        if (state->panelFade < 0) {
            state->panelFade = 0;
        }
    }
    func_0026BEC0(0, 0, 0, state->panelFade, &state->panels[0], 0x52);
    func_0026BEC0(0, 0xCF8, 0, state->panelFade, &state->panels[1], 0x52);
}

/* Classify panel fade: zero, nonzero below sixty, or at least sixty.
 * This is a blend weight, not a remaining-frame countdown. */
s32 fldClassifyRemainingFrames(void *address) {
    MenuSlotState *work = (MenuSlotState *)address;
    s32 fade = work->panelFade;

    if (fade == 0) {
        return 0;
    }
    return fade >= 60 ? 2 : 1;
}

/* Configure the current cursor effect; mode three also configures a valid previous slot.
 * A negative current slot prevents every configuration, including the previous slot. */
void mnuTerminalConfigureEffects(u32 mode, MenuSlotState *state) {
    s32 *slot = state->selectedSlots;

    if (*slot < 0) {
        return;
    }
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(state->resourceBank[0], *slot, state->effect[4], 0, 5, 2);
        break;
    case 2:
        effConfigureWithDefaultSetting(state->resourceBank[0], *slot, state->effect[5], 0, 0, 2);
        break;
    case 3:
        effConfigureWithDefaultSetting(state->resourceBank[0], *slot, state->effect[4], 0, 0, 2);
        if (slot[1] >= 0) {
            effConfigureWithDefaultSetting(state->resourceBank[0], slot[1], state->effect[5], 0, 0, 2);
        }
        break;
    }
}

/* Map a command index to the native five-slot table, then configure its effect.
 * Mode-one index one remaps to three; -2 clears only the previous slot.
 * Other negative indices retain both slots. Nonnegative indices require caller bounds. */
void mnuTerminalSelectSlot(s32 mode, s32 index, MenuSlotState *state) {
    s32 table[5] = {3, 1, 2, 0x2D, 0x52};

    if (state->mode == 1 && index == state->mode) {
        index = 3;
    }
    if (index >= 0) {
        state->selectedSlots[1] = state->selectedSlots[0];
        state->selectedSlots[0] = table[index];
    } else if (index == -2) {
        state->selectedSlots[1] = -1;
    }
    mnuTerminalConfigureEffects(mode, state);
}

/* Draw valid current/previous slots only while DDS2's native terminal draw gate is set. */
void mnuDrawTerminalSelectedSlots(s32 context) {
    MenuSlotState *state = (MenuSlotState *)context;
    EffectPair position = D_00437878[0];
    s32 *slot;
    u32 i;

    if (D_00437859 != 0) {
        for (i = 0, slot = state->selectedSlots; i < MNU_SELECTED_SLOT_COUNT; i++, slot++) {
            if (*slot >= 0) {
                itfDrawGridWithResolvedSlot(position.firstValue, position.secondValue, 0, 0x81,
                                            state->resourceBank[0], *slot, MNU_TEXT_DRAW_PRIORITY);
            }
        }
    }
}

/* Scene modes 1 and 2 select different entries from the same frame table. */
s32 fldGetModeFrameRecordIndex(s32 object) {
    switch (((MenuSlotState *)object)->mode) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_00268C08(MenuSlotState *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex((s32)scene);
    return *(u8 *)&((EffectSlotSet *)scene->resourceBank[0])->workEntries[index].geometry.cornerColors[0];
}



void mnuConfigureSelectedSceneModeEffect(s32 mode, MenuSlotState *host) {
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(host->resourceBank[0], 6, host->effect[0], 0, 0, 2);
        return;
    case 2:
        effConfigureWithDefaultSetting(host->resourceBank[0], 6, host->effect[3], 0, 0, 2);
        break;
    }
}


void func_00268CC0(u32 mode, s32 context) {
    MenuSlotState *state = (MenuSlotState *)context;
    s32 index;
    u32 *entries;
    s32 i;

    index = fldGetModeFrameRecordIndex(context);
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(state->resourceBank[0], index, state->effect[0], 0, 0, 2);
        effConfigureWithDefaultSetting(state->resourceBank[0], 4, state->effect[6], 0, 0, 14);
        effConfigureWithDefaultSetting(state->resourceBank[0], 6, state->effect[0], 0, 0, 2);
        itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0], 7, 0, 0, -0x400, 0);
        effConfigureWithDefaultSetting(state->resourceBank[0], 7, state->effect[5], 0, 5, 3);
        i = 0;
        entries = state->alternateBatch->workEntries[0].geometry.cornerColors;
        for (; i < 4; i++) {
            entries[i] = 0;
        }
        break;
    case 2:
        effConfigureWithDefaultSetting(state->resourceBank[0], index, state->effect[3], 0, 0xF, 2);
        effConfigureWithDefaultSetting(state->resourceBank[0], 6, state->effect[3], 0, 0xF, 2);
        itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0], 7, 0, 0, 0, 0);
        effConfigureWithDefaultSetting(state->resourceBank[0], 7, state->effect[3], 0, 0, 2);
        return;
    case 3:
        itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0], 7, 0, 0, -0x400, 0);
        effConfigureWithDefaultSetting(state->resourceBank[0], 7, state->effect[5], 0, 0, 3);
        effConfigureWithDefaultSetting((u32)state->alternateBatch, 0, state->effect[5], 0, 0, 2);
        break;
    }
}

/* Draw terminal grids and the selected effect text. */
void func_00268EC8(s32 context) {
    MenuSlotState *state = (MenuSlotState *)context;
    EffectPair position[6] = {
        {0, 0}, {0x200, 0xF0}, {0x1280, 0xC8},
        {0x840, 0x220}, {0x1500, 0xD0}, {0x1410, 0x1E0}
    };
    s32 index = fldGetModeFrameRecordIndex(context);
    EffectSlotSet *batch;
    BdWork *record;
    u32 progress;

    if (D_00437859 == 0) {
        return;
    }
    itfDrawGridWithResolvedSlot(position[0].firstValue, position[0].secondValue, 0, 0x81,
                                state->resourceBank[0], index, MNU_TEXT_DRAW_PRIORITY);
    batch = (EffectSlotSet *)state->resourceBank[0];
    record = (BdWork *)(sizeof(*batch->workEntries) * index + (u32)batch->workEntries);
    progress = ((u32)*(u8 *)&record->geometry.cornerColors[0] << 8) /
        *(u8 *)&record->savedColors[0];
    if (state->reduced != 2) {
        func_00306CD0(position[1].firstValue, position[1].secondValue, 0,
                      progress, 0x81, batch, 4, MNU_TEXT_DRAW_PRIORITY);
        batch = (EffectSlotSet *)state->resourceBank[0];
    }
    if (*(s32 *)((u8 *)state + 0xA0) != 0) {
        position[3].firstValue += 0x320;
    }
    itfDrawGridWithResolvedSlot(position[3].firstValue, position[3].secondValue, 0, 0x81,
                                (u32)batch, 7, MNU_TEXT_DRAW_PRIORITY);
    itfDrawGridWithResolvedSlot(position[3].firstValue, position[3].secondValue, 0, 0x81,
                                (u32)state->alternateBatch, 0, MNU_TEXT_DRAW_PRIORITY);
    if (state->mode != 2) {
        itfDrawGridWithResolvedSlot(position[2].firstValue, position[2].secondValue, 0, 0x81,
                                    state->resourceBank[0], 6, MNU_TEXT_DRAW_PRIORITY);
        mnuQueueFontGlyphFromSelectedAtlasSlot(position[4].firstValue, position[4].secondValue,
                                                0, *(u8 *)&((EffectSlotSet *)state->resourceBank[0])->workEntries[6].geometry.cornerColors[0] | 0xA09DC300,
                                                (s8)state->slotCopy, (s8)state->mode);
    }
}


/* Configure three terminal grid entries from the current mode's effect. */
void func_002690A8(u32 mode, s32 context) {
    MenuSlotState *state = (MenuSlotState *)context;
    s32 index = 0;
    s32 setting = 0;
    s32 kind = 0;
    s32 y = 0;
    s32 z = 0;
    s32 *effect;
    u32 effectAddress;

    switch (mode) {
    case 1:
        y = -0x100;
        kind = 2;
        setting = 5;
        index = 2;
        break;
    case 3:
        y = -0x100;
        z = 0;
        kind = 2;
        setting = 7;
        index = 2;
        break;
    case 4:
        y = 0;
    case 2:
        z = 0x100;
        kind = 2;
        setting = 4;
        index = 3;
        break;
    }
    effectAddress = sizeof(state->effect[0]) * index + (u32)context;
    effectAddress += (u32)((u8 *)&state->effect[0] - (u8 *)state);
    effect = (s32 *)effectAddress;
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0], 8, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(state->resourceBank[0], 8, *effect, 0, setting, kind);
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0], 0x38, 0, y, 0, z);
    effConfigureWithDefaultSetting(state->resourceBank[0], 0x38, *effect, 0, setting, kind);
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0], 0x39, 0, y, 0, z);
    effConfigureWithDefaultSetting(state->resourceBank[0], 0x39, *effect, 0, setting, kind);
}


INCLUDE_ASM(const s32, "game/code_002665B0", func_00269230);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F88);

s32 func_00269418(MenuList *list) {
    switch (list->count) {
    case 2: return 0x3a;
    case 3: return 0x3b;
    case 4: return 0x3c;
    case 5: return 0x3d;
    case 6: return 0x3e;
    default: return 0x3f;
    }
}

/* Configure the terminal selection grid for its entry and transition mode. */
void func_00269478(u32 mode, s32 context) {
    MenuSlotState *state = (MenuSlotState *)context;
    s32 effectIndex = 0;
    s32 materialFlags = 0;
    s32 height = 0;
    s32 color = 0;
    s32 yOffset = 0;
    s32 slot;
    s32 *effect;

    switch (mode) {
    case 2:
        color = 2;
        materialFlags = 4;
        yOffset = 0;
        height = 0x100;
        effectIndex = 3;
        break;
    case 3:
        color = 2;
        materialFlags = 7;
        yOffset = -0x100;
        height = 0;
        effectIndex = 2;
        break;
    case 4:
        materialFlags = 4;
        yOffset = 0;
        height = 0x100;
        effectIndex = 3;
        break;
    }
    slot = func_00269418(state->secondaryList);
    /* The context word transports the terminal state address. */
    effect = (s32 *)(effectIndex * sizeof(state->effect[0]) + context +
        (u32)&((MenuSlotState *)0)->effect);
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0],
        slot, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(state->resourceBank[0], slot, *effect,
        0, materialFlags, color);
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0],
        0x40, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(state->resourceBank[0], 0x40, *effect,
        0, materialFlags, color);
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0],
        0x41, 0, yOffset, 0, height);
    effConfigureWithDefaultSetting(state->resourceBank[0], 0x41, *effect,
        0, materialFlags, color);
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)state->resourceBank[0],
        0x42, 0, yOffset, 0, height);
    effConfigureWithDefaultSetting(state->resourceBank[0], 0x42, *effect,
        0, materialFlags, color);
}


void func_00269638(s32 close, MenuSlotState *host) {
    struct {
        s32 slot;
        s32 x;
        s32 y;
    } rows[4] = {
        {0, 0x1C0, 0x238},
        {0x40, 0x1C0, 0x380},
        {0x41, 0x320, 0x2F8},
        {0x42, 0x3B0, 0x3D8}
    };
    s32 count = host->secondaryList->count;
    s32 slot;
    s32 x;
    s32 y;
    s32 i;
    u32 progress;
    EffectSlotSet *slots;

    if (count > 7) {
        count = 7;
    }
    itfDrawGridWithResolvedSlot(rows[2].x, rows[2].y, 0, 0x80,
                               host->resourceBank[0], rows[2].slot, 0x52);
    slot = rows[3].slot;
    x = rows[3].x;
    y = rows[3].y;
    for (i = 0; i < count; i++) {
        itfDrawGridWithResolvedSlot(x, y, 0, 0,
                                   host->resourceBank[0], slot, 0x52);
        y += 0xB0;
    }
    itfGridLookupValueOrDefault((EffectSlotSet *)host->resourceBank[0], slot);
    mnuCallInitWide(0x3B0, 0x3D8, 0, (s32)host->secondaryList, 0x52);
    slot = func_00269418(host->secondaryList);
    itfDrawGridWithResolvedSlot(rows[0].x, rows[0].y, 0, 0x80,
                               host->resourceBank[0], slot, 0x52);
    itfDrawGridWithResolvedSlot(rows[1].x, rows[1].y + count * 0xB0 - 0xB0, 0, 0x80,
                               host->resourceBank[0], rows[1].slot, 0x52);
    slot = func_00269418(host->secondaryList);
    slots = (EffectSlotSet *)host->resourceBank[0];
    progress = ((u32)*(u8 *)&slots->workEntries[slot].geometry.cornerColors[0] << 8) /
               *(u8 *)&slots->workEntries[slot].savedColors[0];
    if (close != 0) {
        if (host->secondaryList->scale > 0) {
            host->secondaryList->scale -= 0x40;
        }
        if (host->secondaryList->scale < 0) {
            host->secondaryList->scale = 0;
        }
    } else if (progress == 0x100) {
        if (host->secondaryList->scale < 0x100) {
            host->secondaryList->scale += 0x40;
        }
        if (host->secondaryList->scale > 0x100) {
            host->secondaryList->scale = 0x100;
        }
    }
}



/* Reset grid entry 0x1A, then configure it from the panel's setting slot chosen by `kind`. */
void mnuApplyGridPanelHostSetting(u32 kind, MenuSlotState *host) {
    s32 flags = 0;
    s32 value = 0;
    s32 slot = 0;

    switch (kind) {
    case 2:
        flags = 2;
        value = 4;
        slot = 3;
        break;
    case 3:
        flags = 2;
        value = 7;
        slot = 2;
        break;
    case 4:
        value = 4;
        slot = 3;
        break;
    }
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)host->resourceBank[0], 0x1A, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(host->resourceBank[0], 0x1A, host->effect[slot], 0, value, flags);
}


/* Transition host: two optional callbacks at +0xCC and the flag at +0xD4 that
   picks which value they are called with. */
typedef struct TransitionHost {
    u8 pad00[0xCC];
    void (*callbacks[2])(s32, struct TransitionHost *); /* 0xCC */
    u32 forceCallbackIndexOne; /* 0xD4 */
} TransitionHost;



extern void func_0026C900(void);


typedef struct EventMenuSelection {
    s32 kind;       /* 0x00 */
    u8 pad04[0x44];
    u32 flags;      /* 0x48 */
    u8 pad4C[0x14];
    s32 entryIndex; /* 0x60 */
} EventMenuSelection;

typedef struct EventMenuOwner {
    u8 pad00[0x1C];
    EventMenuSelection *selection; /* 0x1C */
    s32 state; /* 0x20 */
    u8 pad24[0x18];
    s32 scale; /* 0x3C: fade scale, 0x100 when fully shown */
} EventMenuOwner;

typedef struct EventDispatchState {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus; /* 0x54 */
    u32 dispatchValue; /* 0x58 */
    u8 pad5C[0x1C];
    EventMenuOwner *visualState; /* 0x78 */
    EventMenuOwner *thresholdOwner; /* 0x7C */
    EventMenuOwner *menuOwner; /* 0x80 */
    s32 menuMode; /* 0x84 */
    s32 unk88;
    u8 pad8C[4];
    s32 displayMode; /* 0x90 */
    u8 pad94[8];
    s32 fileTaskActive; /* 0x9C */
    s32 fadeStarted; /* 0xA0 */
    u8 padA4[0x28];
    u32 callback; /* 0xCC */
    u32 previousCallback; /* 0xD0 */
    s32 exitState; /* 0xD4 */
    s32 menuActive;      /* 0xD8: cleared when the menu command chain ends */
    s32 selectionStep;  /* 0xDC: compared against 2 by evtExitSelectionMenuAndSendSoundCommand */
    u8 padE0[4];
    s32 savedMenuMode;   /* 0xE4: nonzero re-requests the bank resource on entry */
    u8 padE8[0x64];
    s32 stage;           /* 0x14C */
    u8 pad150[4];
    u32 bgmHandle;    /* 0x154: encoded bank/track handle */
} EventDispatchState;

extern void func_00268CC0(u32, s32);






extern void mnuQueueTerminalCurrencyLabel(s32, s32);

extern void mnuSelectFirstListNode(s32);




extern void evtCopyEntryStringToActiveWindow(s32, void *);

extern void dspSetActive(s32);

extern s32 dspStartEntry(s32);

extern void evtSetMessageWindowOptionWhenOpen(s32);

extern void evtStoreValueAndCaptureWindowPanelValue(s32);




extern void func_00269638(s32, MenuSlotState *);

extern void func_00269478(u32, s32);





extern void func_00269978(void);





extern u8 D_003CE97C[];


INCLUDE_ASM(const s32, "game/code_002665B0", func_00269978);

/* Install the new transition callback while retaining the previous callback address. */
void evtRememberDispatchCallback(u32 callback, s32 address) {
    EventDispatchState *state = (EventDispatchState *)address;
    u32 previous;

    previous = state->callback;
    state->callback = callback;
    state->previousCallback = previous;
}

/* Dispatch registered transition callbacks, optionally forcing index one. */
void mnuDispatchTransitionHostCallbacks(s32 state) {
    TransitionHost *host = (TransitionHost *)state;
    u32 i;

    for (i = 0; i < 2; i++) {
        if (host->callbacks[i] != NULL) {
            host->callbacks[i](host->forceCallbackIndexOne ? 1 : (s32)i, host);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269B80);

u32 func_00269C48(void) {
    return 1;
}

extern u32 mnuMapPadMaskToFlags(s32 mask);
extern s32 func_002685F0(s32 action, s32 context);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
extern MenuListNode *mnuRetreatListCursorDefault(u32 list);
extern MenuListNode *mnuAdvanceListCursorDefault(u32 list);
extern void mnuPlayInputSound(s32 mode, s32 buttons, u32 *flags);
extern s32 D_003CE7D0[];
extern char D_003CE848[];
extern char D_003CE880[];
extern char D_003CE8B8[];
extern char D_003CE8F0[];

/* Event panel input: confirm opens the popup for the selected entry's action, cancel opens the back popup, left/right step the list. */
s32 evtBHandleSelectionPanelInput(void *input) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *state = &context->dispatchStatus;
    s32 kind = context->visualState->selection->kind;
    s32 frames;
    s32 action;
    EventMenuSelection *node;
    s32 result;

    result = func_002C4038(context->dispatchWork, state, 0, input);
    if (result != 0) {
        return result;
    }
    frames = fldClassifyRemainingFrames(context);
    if (frames != 2) {
        return 0;
    }
    if (context->visualState->scale < 0x100) {
        return 0;
    }
    if (*state == 0) {
        if (buttons & 1) {
            node = context->visualState->selection;
            action = D_003CE7D0[func_002674F8() * 5 + context->menuMode * 10 + kind];
            if (!(node->flags & 1) || action == 3 || action == frames) {
                if (func_002685F0(action, (s32)context) == 0) {
                    switch (action) {
                    case 2:
                        if (context->menuOwner->state == 1) {
                            mnuSetPopupEntryFlagged(state, D_003CE8F0);
                        } else {
                            mnuSetPopupEntryFlagged(state, D_003CE880);
                        }
                        break;
                    case 1:
                        kwlnFadeInStart(0, 0, 0, 0xF);
                    default:
                        mnuSetPopupEntryFlagged(state, D_003CE848 + action * 28);
                        break;
                    }
                }
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(state, D_003CE8B8);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo((u32 *)context->visualState);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault((u32)context->visualState);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault((u32)context->visualState);
        }
        mnuPlayInputSound(0, buttons, (u32 *)context->visualState);
    }
    return 0;
}

s32 evtDispatchSelectionAfterFieldFrameGate(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    mnuDrawTerminalBackdrop(state);
    mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    if (fldClassifyRemainingFrames((void *)state) != 2) {
        return 0;
    }
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBSetupDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}


extern void evtFinishMessageWindowAndNotify(void);

s32 evtClearDispatchVisualFlag(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    evtRememberDispatchCallback(0, (s32)state);
    state->visualState->scale = 0;
    mnuTerminalSetTrack(0, 0);
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern void func_002690A8(u32, s32);

extern void func_00269230(void);


extern void mnuReleaseResourceGroup(s32);

s32 evtBeginSelectionExitFade(void) {
    s32 state = kwlnTaskGetUserValue();
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    mnuTerminalSelectSlot(0, -2, state);
    mnuTerminalSetTrack(1, 0);
    mnuReleaseResourceGroup(state);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern s32 fileMenuTaskExists(void);
extern void fileSetPreviewLocation();
extern void func_002CE208();

/* Poll the file task after the dispatch/fade barrier, then restore the menu popup. */
s32 func_0026A048(void *request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s32 result = func_002C4038(state->dispatchWork, dispatch, 0, request);

    if (result != 0) {
        return result;
    }
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    if (state->fileTaskActive == 0 && sdfCheckPendingWorkWithInterrupts() == 0) {
        mnuReleaseResourceGroupTextureHandles((u32)state);
        fileSetPreviewLocation(state->menuMode, state->unk88);
        func_002CE208(0);
        state->fileTaskActive = 1;
    }
    if (state->dispatchStatus == 0 && fileMenuTaskExists() == 0 &&
        sdfCheckPendingWorkWithInterrupts() == 0) {
        state->fileTaskActive = 0;
        mnuSetPopupEntryFlagged(dispatch, D_003CE848);
    }
    return 0;
}

s32 evtBDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler(state, 1, (void *)request);
}

s32 evtBSetupDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}

u32 evtInitializeSelectionListWhenReady(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    if (context->menuActive == 0) {
        mnuSelectFirstListNode((s32)context->menuOwner);
        func_00268CC0(3, (s32)context);
        evtRememberDispatchCallback((s32)func_00269638, (s32)context);
        func_00269478(3, (s32)context);
        func_002690A8(4, (s32)context);
        mnuTerminalSelectSlot(3, 1, (s32)context);
    }
    context->menuActive = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}

u32 evtFinishPendingSelectionTransition(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    if (context->menuActive != 0) {
        func_00268CC0(3, (s32)context);
        evtRememberDispatchCallback((s32)func_00269230, (s32)context);
        func_00269478(4, (s32)context);
        func_002690A8(3, (s32)context);
        mnuTerminalSelectSlot(3, 0, (s32)context);
        evtFinishMessageWindowAndNotify();
    }
    context->menuActive = 0;
    return 1;
}

/* Handle owner-panel input only after dispatch completes and the panel is fully shown. */
s32 func_0026A2E0(void *request) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *dispatch = &context->dispatchStatus;
    s32 result = func_002C4038(context->dispatchWork, dispatch, 0, request);

    if (result != 0 || context->menuOwner->scale < 0x100) {
        return result;
    }
    if (*dispatch == 0) {
        if (buttons & 1) {
            mnuSetPopupEntryFlagged(dispatch, D_003CE8F0);
        }
        if (buttons & 2) {
            context->menuActive = 1;
            mnuSetPopupEntryFlagged(dispatch, D_003CE848);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo((u32 *)context->menuOwner);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault((u32)context->menuOwner);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault((u32)context->menuOwner);
        }
        mnuPlayInputSound(0, buttons, (u32 *)context->menuOwner);
    }
    return 0;
}

s32 func_0026A3F8(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    mnuDrawTerminalBackdrop(state);
    mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBSetupDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}

u32 evtEnterThresholdSelectionList(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    mnuRefreshThresholdNodeFlags((s32)context->thresholdOwner);
    mnuSelectFirstListNode((s32)context->thresholdOwner);
    mnuTerminalSelectSlot(3, 2, (s32)context);
    mnuApplyGridPanelHostSetting(3, (MenuSlotState *)context);
    func_002690A8(4, (s32)context);
    evtRememberDispatchCallback((s32)func_00269978, (s32)context);
    return 1;
}

u32 evtBEnterStateA(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuTerminalSelectSlot(3, 0, context);
    mnuApplyGridPanelHostSetting(4, (MenuSlotState *)context);
    func_002690A8(3, context);
    evtRememberDispatchCallback((s32)func_00269230, context);
    mnuHighlightProgressNodeByMode(context);
    return 1;
}

/* Recover the selected affordable party member, then process list input. */
s32 func_0026A598(KwlnTask *task) {
    extern u32 kwlnTaskGetUserValue(KwlnTask *);
    MenuSlotState *host = (MenuSlotState *)kwlnTaskGetUserValue(task);
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *dispatch = &host->dispatchStatus;
    s32 result = func_002C4038(&host->transitionWork, dispatch, 0, task);
    MenuListNode *node;
    MenuThresholdEntry *entry;

    if (result != 0) {
        return result;
    }
    if (*dispatch == 0) {
        if (buttons & 1) {
            node = host->progressList->cursor;
            if (!(node->flags48 & 1)) {
                entry = &node->terminal;
                buttons = 0;
                fldSaveSceneOptionsAndClearFlags(&datGameState->party[entry->entryId]);
                sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
                mnuReleaseSelectedProgressPanel(host);
                datAddCurrencyClamped(-entry->requiredAmount);
                mnuRefreshThresholdNodeFlags(host->progressList);
                if (host->progressList->count == 0) {
                    mnuSetPopupEntryFlagged(dispatch, D_003CE848);
                }
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(dispatch, D_003CE848);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo(&host->progressList->stateFlags);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault((u32)host->progressList);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault((u32)host->progressList);
        }
        mnuPlayInputSound(0, buttons, &host->progressList->stateFlags);
    }
    return 0;
}


extern char D_00437868[];

void mnuQueueTerminalCurrencyLabel(s32 fading, s32 context) {
    EventDispatchState *state = (EventDispatchState *)context;
    s32 index;
    FrFontGlyph *font;
    u32 color;
    char text[16];

    index = fldGetModeFrameRecordIndex(context);
    func_0035C860(text, D_00437868, datGameState->header.currency);
    if (fading == 0) {
        /* The scene record supplies the steady label's low packed-color byte. */
        color = *(u8 *)&((EffectSlotSet *)((MenuSlotState *)context)->resourceBank[0])->workEntries[index].geometry.cornerColors[0] | 0xA09DC300;
    } else {
        color = uiBlendColors(0xA09DC380, 0xA09DC300, state->thresholdOwner->scale);
    }
    font = (FrFontGlyph *)func_0019F6C8(0x1810, 0x1C8, 0, color, text, 0);
    func_0019D550(font, 1, 0x52);
    frFontQueueGlyphInSelectedSlot(font);
}

s32 mnuInitializeSelectionDispatchWhenModeUnset(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    mnuDrawTerminalBackdrop(state);
    mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    mnuDrawTerminalSelectedSlots(state);
    if (dispatchState->menuMode == 0) {
        mnuQueueTerminalCurrencyLabel(1, state);
    }
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBSetupDispatchSyncD(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}

u32 evtSelectFinalVisualNode(void) {
    EventDispatchState *state;

    state = (EventDispatchState *)kwlnTaskGetUserValue();
    mnuSelectLastListNode((u32)state->visualState);
    return 1;
}

s32 evtOpenTerminalFollowupPopupWhenIdle(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    s32 result = func_002C4038((u8 *)state + 8, panel, 0, (void *)request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (fldClassifyRemainingFrames((void *)state) == 0) {
                mnuSetPopupEntry(panel, D_003CE97C);
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026A998);

s32 evtBSetupDispatchSyncE(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}




s32 evtBReleaseImagesAndQueueMenuTransition(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuReleaseCampTextureHandlesAndClearOutput(((MenuSlotState *)state)->imageHandles);
    mnuConfigureSelectedSceneModeEffect(2, (MenuSlotState *)state);
    mnuTerminalSelectSlot(3, 4, state);
    func_002690A8(2, state);
    evtRememberDispatchCallback(0, state);
    *(u32 *)(state + 0xA0) = 0;
    evtFinishMessageWindowAndNotify();
    dspCloseChannel();
    return 1;
}



s32 mnuOpenTerminalSelectionMessageWindow(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuSnapshotCampTextureHandles(((MenuSlotState *)state)->imageHandles);
    mnuConfigureSelectedSceneModeEffect(1, (MenuSlotState *)state);
    mnuTerminalSelectSlot(3, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    evtCreateMessageWindowIfMissing(((MenuSlotState *)state)->messageResources.unk04);
    return 1;
}


extern void mnuCreateResourceTask(void);




extern char D_003CE848[];


/* Dispatch completion waits for the fade and pending resource/graph work;
 * keep the request outstanding until that barrier has drained. */
s32 evtPollDispatchAfterFade(void *request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s32 result = func_002C4038(state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if (kwlnFadeIsActive() == 0) {
            if (state->fadeStarted == 0) {
                state->fadeStarted = 1;
                mnuCreateResourceTask();
            }
            if (*dispatch == 0 && state->fadeStarted == 1 &&
                mnuCheckResourceTask() == 0) {
                if (sdfCheckPendingWorkWithInterrupts() != 0) return 0;
                mnuTerminalSelectResourceBank((MenuSlotState *)state);
                state->fadeStarted = 0;
                mnuSetPopupEntryFlagged(dispatch, D_003CE848);
            }
        }
        result = 0;
    }
    return result;
}


extern void mnuDispatchTransitionHostCallbacks(s32);

extern void func_00268EC8(s32);

extern void mnuDrawTerminalSelectedSlots(s32);

s32 func_0026AC90(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    mnuDrawTerminalBackdrop(state);
    mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler(state, 2, (void *)request);
}

/* Bind the selected text row to message slot zero, then start the entry prompt. */
u32 evtPrepareSelectedMenuEntry(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    EventMenuOwner *owner = state->menuOwner;
    s32 *selectionIndex = &owner->selection->entryIndex;

    if (owner->state == 1) {
        mnuSelectFirstListNode((s32)owner);
    }
    evtCopyEntryStringToActiveWindow(0, D_003A41A8[*selectionIndex].encodedText);
    dspSetActive(1);
    dspStartEntry(0);
    evtSetMessageWindowOptionWhenOpen(1);
    evtStoreValueAndCaptureWindowPanelValue(6);
    return 1;
}

u32 func_0026ADC0(void) {
    return 1;
}

extern s8 evtGetCapturedWindowPanelValue(void);
extern char D_003CE928[];

s32 evtOpenTerminalOwnerStatePopup(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s32 result = func_002C4038(state->dispatchWork, dispatch, 0, (void *)request);

    if (result == 0) {
        /* Choose the follow-up only after menu and message-window work is idle. */
        if (*dispatch == 0 && evtGetMessageWindowControlState() == 0) {
            if (evtGetCapturedWindowPanelValue() == 0) {
                mnuResetProgressModeFromOwner((u8 *)state);
                mnuSetPopupEntryFlagged(dispatch, D_003CE928);
            } else if (state->menuOwner->state >= 2) {
                state->menuActive = 1;
                mnuSetPopupEntryFlagged(dispatch, D_003CE880);
            } else {
                mnuSetPopupEntryFlagged(dispatch, D_003CE848);
            }
        }
        result = 0;
    }
    return result;
}

s32 func_0026AEB0(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    mnuDrawTerminalBackdrop(state);
    mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBSetupDispatchSyncF(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}

u32 evtBCheckPanelMode(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    dspSetActive(1);
    switch (state->displayMode) {
    case 1:
        dspStartEntry(1);
        break;
    case 2:
        dspStartEntry(2);
        break;
    }
    return 1;
}

s32 evtBContinueDispatchOrRestoreTable(void *request) {
    EventDispatchState *state;
    s32 result;
    s32 *dispatch;

    state = (EventDispatchState *)kwlnTaskGetUserValue();
    dispatch = &state->dispatchStatus;
    result = func_002C4038(state->dispatchWork, dispatch, 0, request);
    if (result != 0) {
        return result;
    }
    if (*dispatch == 0 && evtGetMessageWindowControlState() == 0) {
        mnuSetPopupEntry(dispatch, (void *)state->dispatchValue);
    }
    return 0;
}

s32 mnuPrepareDispatchStateAndBindHandler(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    mnuDrawTerminalBackdrop(state);
    mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBSetupDispatchSyncG(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, (void *)request);
}

u32 evtExitSelectionMenuAndSendSoundCommand(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    func_00268CC0(2, (s32)context);
    if (context->selectionStep >= 2) {
        mnuTerminalSelectSlot(2, -1, (s32)context);
        func_00269478(2, (s32)context);
    } else {
        mnuTerminalSelectSlot(2, -1, (s32)context);
        func_002690A8(2, (s32)context);
    }
    evtRememberDispatchCallback(0, (s32)context);
    context->exitState = 1;
    func_00342580(context->bgmHandle);
    evtClearActiveFlag(0);
    return 1;
}


s32 evtBRebuildTerminalMenuAndResetDispatch(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuReleaseWorkResources(state);
    mnuTerminalBuildMenus(state);
    func_00268CC0(1, state);
    mnuTerminalSelectSlot(1, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    ((EventDispatchState *)state)->stage = 1;
    ((EventDispatchState *)state)->exitState = 0;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

/* Selection chain startup: fade in once the field frames are drained, then open the popup when the fade finishes. */
s32 evtBStartSelectionChainAfterFade(void *request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s32 result = func_002C4038(state->dispatchWork, dispatch, 0, request);

    if (result == 0) {
        if (*dispatch == 0 && fldClassifyRemainingFrames((void *)state) == 0) {
            switch (state->stage) {
            case 1:
                kwlnFadeInStart(0, 0, 0, 15);
                if (state->savedMenuMode != 0) {
                    mnuTerminalSelectResourceBank((MenuSlotState *)state);
                }
                state->stage = 2;
                break;
            case 2:
                if (kwlnFadeIsActive() == 0) {
                    mnuSetPopupEntryFlagged(dispatch, D_003CE848);
                }
                break;
            }
        }
        return 0;
    }
    return result;
}

s32 evtBDispatchSyncD2(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    mnuDrawTerminalBackdrop(state);
    if (func_00268C08((MenuSlotState *)state) == 0) {
        mnuDrawAndStepTerminalPanelFade(1, (MenuSlotState *)state);
    } else {
        mnuDrawAndStepTerminalPanelFade(0, (MenuSlotState *)state);
    }
    mnuDispatchTransitionHostCallbacks(state);
    if (dispatchState->stage != 3) {
        func_00268EC8(state);
    }
    mnuDrawTerminalSelectedSlots(state);
    return menuSetHandler(dispatchState, 1, (void *)request);
}

s32 evtBDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler(state, 2, (void *)request);
}

extern void mnuTerminalFadeOrClose(s32, s32);



extern void mnuStartMantraSpriteLoad(void);

s32 evtBCloseTerminalAndLoadMantraSprites(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 mode;
    mnuTerminalFadeOrClose(0, state);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    mode = ((EventDispatchState *)state)->menuMode;
    if (mode < 2) {
        if (mode >= 0) {
            mnuStartMantraSpriteLoad();
        }
    }
    return 1;
}

u32 func_0026B4A0(void) {
    return 1;
}

extern KwlnTask *kwlnTaskGetTaskByName(const char *);
extern s32 mnuHasMantraSpriteTaskFinished(void);
extern char D_00425008[];
extern char D_003CE960[];

/* Wait for the mode's load/fade barrier before binding the terminal exit popup. */
s32 evtWaitForTerminalExitPopup(void *request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s32 ready = 0;
    s32 result = func_002C4038(state->dispatchWork, dispatch, 0, request);

    if (result != 0) {
        return result;
    }
    if (state->menuMode < 2) {
        if (state->menuMode >= 0) {
            result = mnuHasMantraSpriteTaskFinished();
            if (result == 0) {
                return result;
            }
        }
    }
    if (state->dispatchStatus == 0) {
        if (state->menuMode == 0) {
            ready = kwlnTaskGetTaskByName(D_00425008) == NULL;
            if (evtIsActiveFlagSet(0) != 0) {
                ready = 1;
            }
            if (ready != 0) {
                if (state->stage == 0) {
                    ready = 0;
                    func_002665E8((MenuSlotState *)state);
                    kwlnFadeOutStart(0, 0, 0, 15);
                    state->stage = 1;
                }
            }
        } else {
            if (state->stage == 0) {
                func_002665E8((MenuSlotState *)state);
                kwlnFadeOutStart(0, 0, 0, 15);
                state->stage = 1;
            } else {
                ready = kwlnFadeIsActive() == 0;
            }
        }
        if (ready != 0) {
            mnuSetPopupEntryFlagged(dispatch, D_003CE960);
        }
    }
    return 0;
}

s32 evtBLateDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    mnuDrawTerminalBackdrop((s32)state);
    return menuSetHandler(state, 1, (void *)request);
}

s32 evtBDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler(state, 2, (void *)request);
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424FF8);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00425008);

INCLUDE_SDATA(const s32, "game/code_002665B0", mnuTerminalTaskState);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437859);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_0043785C);

INCLUDE_SDATA(const s32, "game/code_002665B0", mnuNumberSpriteFormat);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437868);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437870);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437878);

