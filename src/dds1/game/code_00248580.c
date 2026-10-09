#include "mnu_input.h"
#include "evt_world.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "eff_resource_list.h"
#include "eff.h"
#include "common.h"
#include "sdf_chip.h"
#include "fr_font.h"
#include "itf_draw_grid.h"
#include "mnu_staff.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "mnu_list.h"
#include "kwln.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"

#define MNU_PARTY_SLOT_COUNT 5
#define MNU_PERCENT_PAIR_BYTES 0xA8
#define MNU_PERCENT_PANEL_BYTES 0x54
#define MNU_TERMINAL_SCENE_BYTES 0x164
#define MNU_MENU_HOST_BYTES 0x82C
#define MNU_EFFECT_BATCH_COUNT 7
#define MNU_SELECTED_SLOT_COUNT 2
#define MNU_TEXT_DRAW_PRIORITY 0x53
#define MNU_TERMINAL_DEFAULT_BGM 0x20001
#define MNU_BGM_BANK_MASK 0xFFFF0000
#define MNU_RECOVERY_STATUS_KEEP_MASK 0xFA2F
#define MNU_TERMINAL_EXIT_PROCESS 0x322
#define MNU_COLOR_LOW_BYTE_MASK 0xFF


extern void mnuCreateResourceTask(void);


extern s8 D_003BC3E1;

extern s32 dds3GetWorldObject(void);

extern s32 mdlFlagTest(u32);



typedef struct MenuProgressNode {
    s32 index;
    u8 pad04[0x44];
    u32 flags;
    u8 pad4C[0xC];
    struct MenuProgressNode *next;
    u8 pad5C[4];
    u32 entryIndex; /* 0x60: party slot or command-list entry */
    u32 requiredAmount; /* 0x64 */
    u8 pad68[8];
    void *panel; /* 0x70: allocated paired percentage-panel owner */
} MenuProgressNode;

typedef struct {
    u8 pad00[0x10];
    MenuProgressNode *firstProgressNode; /* 0x10 */
    u8 pad14[8];
    MenuProgressNode *selectedNode;      /* 0x1C */
    s32 selectionState;                   /* 0x20 */
    u8 pad24[0x18];
    s32 scale; /* 0x3C */
} MenuProgressOwner;


extern s32 mnuFindMatchingPartyEntryIndex(DatPartyRecord *targetEntry);
extern void mnuSetWindowResource(s32, MenuPageWindow *, s32, s32);
extern void mnuAttachPartyIconBundle(s32, MenuPageWindow *, u32);
extern MenuProfilePanel *mnuCreateProfilePanel(DatPartyRecord *selectionState);
extern void mnuCacheProfilePanelGridPositions(MenuProfilePanel *, u32, u32, u32, u32);
extern void mnuFreeProfilePanelWork(MenuProfilePanel *);
extern void mnuDrawAndAdvanceProfilePanel(s32, s32, s32, MenuProfilePanel *, s32);
extern void func_00276720(MenuPageWindow *, s32, s32, s32);



typedef struct MenuTerminalWork {
    s32 allocation;          /* 0x00 */
    s32 groupResource;       /* 0x04 */
    MenuPopupState transitionWork; /* 0x08 */
    s32 popupState;         /* 0x54 */
    u8 pad58[4];
    s32 messageResources[2]; /* 0x5C: second handle opens the message window */
    EffectSlotSet *batch;  /* 0x64 */
    EffectSlotSet *secondResource;     /* 0x68 */
    EffectSlotSet *alternateBatch; /* 0x6C */
    MenuProgressOwner *listResource; /* 0x70 */
    MenuProgressOwner *list; /* 0x74 */
    MenuProgressOwner *owner;/* 0x78 */
    s32 mode;                /* 0x7C */
    s32 initState;           /* 0x80 */
    u8 pad84[0x14];
    s32 resourcePhase;      /* 0x98 */
    s32 panelFade;          /* 0x9C: 0..0x100 color blend weight */
    EffMappedResource *effect[7];           /* 0xA0: effect batches; [4] and [5] are the pair selected via cursor */
    s32 cursor[2];           /* 0xBC: current and previous node, -1 until selected */
    u8 padC4[0x14];
    s32 selectedSlot;        /* 0xD8 */
    s32 reduced;             /* 0xDC */
    u32 imageHandles[7];     /* 0xE0: copied by mnuResolveStaffImageHandles */
    u8 padFC[0x5C];
    u32 effectHandle; /* 0x158: effect-resource address word. */
    s32 effectStage; /* 0x15C */
    u32 bgmHandle;           /* 0x160: encoded bank/track handle */
} MenuTerminalWork; /* 0x164 allocation (mnuTerminalCreateScene) */
typedef char MenuTerminalWork_size[(sizeof(MenuTerminalWork) == 0x164) ? 1 : -1];

typedef struct MenuResourceWork MenuResourceWork;

extern u8 mnuHasEffectResourceHandle(MenuResourceWork *);
extern void mnuReleaseEffectResource(MenuResourceWork *);
extern MenuResourceWork *mnuRequestEffectResource(const char *, const char *);

extern void *mnuCreateDualPercentPanel(DatPartyRecord *, s32);


extern s32 mnuPercentOrHundred(u16, u16);

extern void mnuDrawPanelSequenceByRow(s32, s32, s32, s32, s32, s32);

extern struct MenuList *mnuCreateListState();


extern void func_002491B8(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);

extern u8 D_003BC3F8[];


extern s32 func_003014F0(char *, const char *, ...);

extern u32 uiBlendColors(u32, u32, s32);

extern void func_00276F70(MenuPageWindow *window, StaffSlots *work);
extern void mnuDrawStaffPanelGridBackdrop(s32 flag, StaffSlots *work);
extern void mnuDrawStageTestList(s32 x, s32 y, s32 z, s32 overrideValue, void *menu, s32 param);
extern void func_00283838(s32, s32, s32, s32, s32, s32, s32);
extern struct FrFontGlyph *func_001978E8(s32, s32, s32, u32, char *, struct FrFontGlyph *);

extern char mnuNumberSpriteFormat[];

extern s32 frFontDrawGlyphChain(struct FrFontGlyph *, s8, u32);
typedef struct EffectPair {
    s32 firstValue;
    s32 secondValue;
} EffectPair;

extern EffectPair D_003BC400[];
extern void mnuDrawTerminalAmountText(s32, s32);
extern char D_003BC3F0[];
extern u32 func_001979C8(s32, s32, s32, s32, char *, s32);

/* Release both visual resources in order; the work object itself is retained. */
void mnuReleaseVisualResources(MenuTerminalWork *work) {
    effResolveAndReleaseResource(work->batch);
    effResolveAndReleaseResource(work->secondResource);
}

/* Release/reset the two resources' texture slots without freeing the work object. */
void mnuReleaseBothVisualResourceTextures(MenuTerminalWork *work) {
    effReleaseTextureHandlesAndResetSlots(work->batch);
    effReleaseTextureHandlesAndResetSlots(work->secondResource);
}

extern struct FrFontGlyph *itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, struct FrFontGlyph *);
/* Fixed-width text rows used by both font drawing and message substitution.
 * The font helper decodes single-byte and two-byte characters from this data. */
typedef struct MenuTextEntry {
    u8 encodedText[32];
} MenuTextEntry;

extern MenuTextEntry D_00347C68[];
extern MenuTextEntry D_003482A8[];

/* Select an encoded text row, draw it at the supplied grid cell, and queue the glyph.
 * The signed-byte slot is not bounds checked; DDS1 has no DDS2 x-origin adjustment. */
void mnuQueueFontGlyphFromAtlasSlot(s32 gridX, s32 gridY, s32 depth, s32 value, s8 slot, s8 alternate) {
    u8 *text;
    struct FrFontGlyph *handle;

    if (alternate == 0) {
        text = D_00347C68[slot].encodedText;
    } else {
        text = D_003482A8[slot].encodedText;
    }
    handle = itfCreateConvertedTextGlyph(gridX, gridY, depth, value, text, 0);
    frFontDrawGlyphChain(handle, 1, MNU_TEXT_DRAW_PRIORITY);
    frFontQueueGlyphForCurrentDrawBuffer(handle);
}


/* Price recovery from missing HP/MP plus the five charged status bits.
 * Preserve DDS1's arithmetic and separate truncations; deficits are not clamped. */
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
    return (s32)missingHp + (s32)(missingMp * (missingMp / 200.0f + 3.0f)) + statusCost;
}

/* Mark nodes unaffordable when their required amount exceeds current currency. */
void mnuRefreshThresholdNodeFlags(MenuProgressOwner *owner) {
    MenuProgressNode *node = owner->firstProgressNode;
    if (node != 0) {
        DatGameState *state = datGameState;
        do {
            u32 currency = state->header.currency;
            if (currency < node->requiredAmount) {
                node->flags |= 1;
            } else {
                node->flags &= ~1u;
            }
            node = node->next;
        } while (node != 0);
    }
}

/* Draw formatted numeric text using a blend toward the color with its low byte clear. */
void mnuCreateNumberSprite(s32 x, s32 y, s32 layer, s32 blendWeight, s32 number, u32 color, s32 priority) {
    char text[16];
    struct FrFontGlyph *sprite;

    func_003014F0(text, mnuNumberSpriteFormat, number);
    sprite = func_001978E8(x, y, layer, uiBlendColors(color, color & ~MNU_COLOR_LOW_BYTE_MASK, blendWeight), text, 0);
    frFontDrawGlyphChain(sprite, 1, priority);
    frFontQueueGlyphForCurrentDrawBuffer(sprite);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248810);





/* Allocate adjacent HP/MP percentage panels, preserving the native 0x54 stride.
 * The source is a party-vitals record; the context supplies the panel style. */
void *mnuCreateDualPercentPanel(DatPartyRecord *unit, s32 workAddress) {
    s32 panel = (s32)sdfAllocSizeClassBlock(MNU_PERCENT_PAIR_BYTES);
    mnuDrawPanelSequenceByRow(panel, 0, 0, 0x1e,
        mnuPercentOrHundred(unit->hp, unit->maxHp),
        *(s32 *)(workAddress + 0xe0));
    mnuDrawPanelSequenceByRow(panel + MNU_PERCENT_PANEL_BYTES, 1, 0, 0x1e,
        mnuPercentOrHundred(unit->mp, unit->maxMp),
        *(s32 *)(workAddress + 0xe0));
    return (void *)panel;
}

/* Release both texture sets and the backing allocation for a nonzero panel pair. */
void mnuReleaseDualPercentPanel(void *panelOwner) {
    s32 panel = (s32)panelOwner;
    if (panelOwner != NULL) {
        mnuReleaseSpriteTextures((s32 *)panel);
        mnuReleaseSpriteTextures((s32 *)((s32)panel + MNU_PERCENT_PANEL_BYTES));
        sdfReleaseChipBlock(panel);
        return;
    }
}

/* Rebuild each node's panel from its corresponding party entry. */
void mnuUpdateGroupResources(u8 *scene) {
    MenuProgressNode *node = *(MenuProgressNode **)(*(u8 **)(scene + 0x74) + 0x10);

    while (node != NULL) {
        node->panel = mnuCreateDualPercentPanel(&datGameState->party[node->entryIndex], (s32)scene);
        node = node->next;
    }
}

/* Release each progress node's child panel, leaving the nodes/list intact. */
void mnuDestroyThresholdNodePanels(s32 owner) {
    s32 entry;

    for (entry = (s32)((MenuTerminalWork *)owner)->list->firstProgressNode; entry != 0; entry = (s32)((MenuProgressNode *)entry)->next) {
        mnuReleaseDualPercentPanel(((MenuProgressNode *)entry)->panel);
    }
}

typedef struct MenuProgressList {
    u8 pad00[0x2C];
    s32 updateCallback; /* 0x2C */
    s32 callback;       /* 0x30 */
    u8 pad34[8];
    s32 visible;        /* 0x3C */
} MenuProgressList;


extern s32 mnuTerminalScoreBox(DatPartyRecord *unit);

extern s32 func_00248810(s32);

/* Build recovery-cost nodes for active party slots with a nonzero computed cost.
 * Node values are party indices here, unlike the command-list builder below. */
void mnuBuildTerminalNodeList(MenuTerminalWork *host) {
    MenuProgressList *list;
    s32 partyIndex;

    list = (MenuProgressList *)mnuCreateListState(0, MNU_PARTY_SLOT_COUNT, 0x24);
    list->callback = (s32)host;
    *(s32 *)&host->list = (s32)list;
    list->updateCallback = (s32)func_00248810;
    list->visible = 0;
    for (partyIndex = 0; partyIndex < MNU_PARTY_SLOT_COUNT; partyIndex++) {
        DatPartyRecord *unit = &datGameState->party[partyIndex];

        if ((u16)(unit->flags & 1)) {
            s32 recoveryCost = mnuTerminalScoreBox(unit);

            if (recoveryCost != 0) {
                MenuProgressNode *node =
                    (MenuProgressNode *)mnuListAppendNode(host->list, D_003BC3F8);
                MenuThresholdEntry *entry = (MenuThresholdEntry *)&node->entryIndex;

                node->panel = 0;
                entry->requiredAmount = recoveryCost;
                entry->entryId = partyIndex;
            }
        }
    }
    mnuRefreshThresholdNodeFlags(host->list);
}

/* Destroy the progress-list allocation retained by the terminal work. */
void mnuReleaseProgressWorkList(MenuTerminalWork *work) {
    mnuDestroyListState((struct MenuList *)work->list);
}

/* Release the selected recovery panel, then pass its owning list to the follow-up. */
void mnuReleaseSelectedProgressPanel(MenuTerminalWork *work) {
    mnuReleaseDualPercentPanel(work->list->selectedNode->panel);
    mnuRemoveListCursorNode((struct MenuList *)work->list);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248E68);

typedef struct MenuSlotKind {
    s16 kind;
    s16 unk2;
} MenuSlotKind;

extern MenuSlotKind D_0032EF18[];

/* Same slot kind, or both kinds in the 30/31 pair. */
s32 mnuSlotKindsInSameGroup(s32 index, s32 requestedKind) {
    s16 current = D_0032EF18[index].kind;

    if (requestedKind == current) {
        return 1;
    }
    if (requestedKind == 30 || requestedKind == 31) {
        if (current == 30) {
            return 1;
        }
        if (current == 31) {
            return 1;
        }
    }
    return 0;
}

extern MenuSlotKind D_0032EFE0[];

extern void func_00248E68(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);

/* Build eligible slot labels for this terminal mode. */
void mnuBuildEligibleSlotList(MenuTerminalWork *host) {
    struct MenuList *list;
    s32 requestedKind;
    s32 i;

    list = mnuCreateListState(0, 4, 0x15);
    list->scale = 0;
    list->drawCallback = func_00248E68;
    list->context = host;
    host->owner = (MenuProgressOwner *)list;
    if (host->mode == 0) {
        requestedKind = D_0032EF18[host->initState].kind;
    } else {
        requestedKind = D_0032EFE0[host->initState].kind;
    }
    for (i = 0; i < 50; i++) {
        if (host->mode == 0 && host->initState == i) {
            continue;
        }
        if (mdlFlagTest(D_0032EF18[i].unk2) == 0 &&
            D_0032EF18[i].unk2 != 0) {
            continue;
        }
        if (mnuSlotKindsInSameGroup(i, requestedKind) == 0) {
            continue;
        }
        if (strlen((char *)D_00347C68[i].encodedText) != 0) {
            struct MenuListNode *node =
                mnuListAppendNode((struct MenuList *)host->owner, D_003BC3F8);

            node->terminal.entryId = i;
            node->title = (char *)D_00347C68[i].encodedText;
        }
    }
}

/* Return whether model flag 0x902 is clear; its storyline meaning is not asserted. */
u8 func_00249198(void) {
    s64 flagSet;

    flagSet = mdlFlagTest(0x902);
    return flagSet == 0;
}


extern void func_002BF4E0(s32, s32, s32, s32, s32, void *, s32, s32);

/* Draw selected-row accents in eighth-pixel units, then the indexed panel. */
void func_002491B8(s32 x, s32 y, s32 arg2, struct MenuList *list,
                   struct MenuListNode *entry, s32 drawContext) {
    s32 value = list->scale;
    MenuTerminalWork *owner = (MenuTerminalWork *)list->context;
    s32 index = entry->sortKeyPrimary;
    s32 selected = entry == list->cursor;

    if (entry->flags48 & 1) {
        value /= 2;
    }
    if (selected) {
        s32 row = list->windowOffset;
        s32 base = (row * 21 + 0x76) << 3;

        index++;
        func_002BF4E0(x - 0x40, base, 0, value, 0,
                      owner->batch, 0x16, drawContext);
        func_002BF4E0(x + 0x4C0, base, 0, value, 0,
                      owner->batch, 0x17, drawContext);
        func_002BF4E0(x + 0xB0, base + 0x18, 0, value, 0,
                      owner->batch, 0x15, drawContext);
    }
    func_002BF4E0(x + 0x1C0, y + 0x68, 0, value, 0,
                  owner->batch, index, drawContext);
}





/* Omit the input-array position `excluded`, not all entries with that same value. */
struct MenuList *mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    struct MenuList *list = mnuCreateListState(0, count, 0x15, callback);
    s32 entryIndex;
    list->context = (void *)callback;
    list->drawCallback = func_002491B8;
    list->scale = 0;
    for (entryIndex = 0; entryIndex < count; entryIndex++) {
        if (entryIndex != excluded) {
            struct MenuListNode *node = mnuListAppendNode(list, D_003BC3F8);
            node->camp.value = items[entryIndex];
        }
    }
    return list;
}


/* Mark the selected command-list node only for modes zero/one and an idle owner. */
void mnuHighlightProgressNodeFromOwnerSelection(s32 object) {
    s32 state = ((MenuTerminalWork *)object)->mode;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (((MenuTerminalWork *)object)->owner->selectionState == 0) {
            struct MenuListNode *selected = mnuWalkNodeList(2 - func_00249198(),
                                                              ((MenuTerminalWork *)object)->listResource);
            ((MenuProgressNode *)selected)->flags |= 1;
        }
    }
}


/* Highlight the mode-zero or mode-two command node only while the progress list is idle. */
void mnuHighlightProgressNodeByMode(s32 object) {
    s32 state = ((MenuTerminalWork *)object)->mode;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_00249198();
    }
    if (((MenuTerminalWork *)object)->list->selectionState == 0) {
        struct MenuListNode *node = mnuWalkNodeList(selectedIndex,
                                                    ((MenuTerminalWork *)object)->listResource);
        ((MenuProgressNode *)node)->flags |= 1;
    }
}

extern void mnuResolveStaffImageHandles(u32 *);

extern s32 mnuTerminalMenuTemplate[];

/* Build the mode-specific command list and the separate party recovery list.
 * DDS1 retains its copied table and model-flag exclusion rather than DDS2's literals. */
void mnuTerminalBuildMenus(MenuTerminalWork *host) {
    s32 table[15];
    s32 row;
    s32 count;
    s32 excluded = -1;

    memcpy(table, mnuTerminalMenuTemplate, 0x3C);


    switch (host->mode) {
    case 0:
        row = 0;
        count = 5;
        if (func_00249198() != 0) {
            excluded = 1;
        }
        break;
    case 1:
        row = 1;
        count = 4;
        if (func_00249198() != 0) {
            excluded = 1;
        }
        break;
    default:
        row = 2;
        count = 2;
        break;
    }
    host->listResource = (MenuProgressOwner *)mnuBuildThresholdNodeList(table + row * 5, count, excluded, (s32)host);
    mnuBuildTerminalNodeList(host);
    mnuResolveStaffImageHandles(host->imageHandles);
    mnuUpdateGroupResources((u8 *)host);
    mnuBuildEligibleSlotList(host);
    mnuHighlightProgressNodeFromOwnerSelection(host);
    mnuHighlightProgressNodeByMode(host);
}

extern void mnuReleaseStaffImageHandles(u32 *);

/* Release command/progress lists, child percentage panels and staff image handles.
 * Preserve their existing order and the single-iteration list loop. */
void mnuReleaseWorkResources(u8 *work) {
    u32 i;

    for (i = 0; i < 1; i++) {
        mnuDestroyListState((struct MenuList *)*(u32 *)(work + 0x70 + i * 4));
    }
    mnuDestroyThresholdNodePanels((s32)work);
    mnuReleaseStaffImageHandles(((MenuTerminalWork *)work)->imageHandles);
    mnuReleaseProgressWorkList((s32)work);
    mnuDestroyListState((struct MenuList *)((MenuTerminalWork *)work)->owner);
}

extern void kwlnFadeOutStart(s32, s32, s32, s32);

extern void evtCreateEventScriptProcess(s32);

extern void evtClearActiveFlag(s32);

extern void evtSetBoundedDisplayValue(s32, s32);

/* Close via a black fade for modes one/two, otherwise request process 0x322.
 * All paths clear active flag zero and set display slot one. No sound call occurs here. */
void mnuFadeOrPlayCloseSfx(s32 skip, u8 *work) {
    if (skip == 0) {
        s32 mode = ((MenuTerminalWork *)work)->mode;

        if (mode < 3) {
            if (mode > 0) {
                kwlnFadeOutStart(0, 0, 0, 15);
            } else {
                evtCreateEventScriptProcess(MNU_TERMINAL_EXIT_PROCESS);
            }
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
    u8 *owner = (u8 *)((MenuTerminalWork *)work)->owner;
    ((MenuTerminalWork *)work)->mode = 0;
    ((MenuTerminalWork *)work)->initState = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
}



extern void *memset(void *, s32, u32);

extern struct EffectList *mnuAllocateValueRecord(u32);
extern void mnuAppendCampSpriteRequests(struct EffectList *, StaffSlots *);
extern void mnuReleaseStaffResourceGroups(StaffSlots *);
extern void mnuClearEntries(MenuPageWindow *);
extern void mnuReleasePartyIconBundles(MenuPageWindow *);

/* Allocate/zero the visual host, retain its allocation, and begin resource setup. */
MenuProgressHost *mnuCreateWorkBlock(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(MNU_MENU_HOST_BYTES);
    MenuProgressHost *work = (MenuProgressHost *)sdfResourceRetainAddress(allocation);

    memset(work, 0, MNU_MENU_HOST_BYTES);
    work->allocation = allocation;
    work->titleEffectHandle = mnuAllocateValueRecord(1);
    mnuInitPartyPanelSlots(&work->partyPanel);
    mnuAppendCampSpriteRequests(work->titleEffectHandle, &work->staffSlots);
    work->loadState = 1;
    return work;
}

/* Release staff window/texture/resource work before the value record and allocation. */
void mnuReleaseStaffMenuContextAndResources(MenuProgressHost *work) {
    mnuShutdownContext(&work->partyWindow);
    mnuReleaseStaffMenuTextureHandles(&work->staffSlots);
    mnuReleaseStaffResourceGroups(&work->staffSlots);
    effDestroyEffectList(work->titleEffectHandle);
    sdfReleaseResourceAllocation(work->allocation);
}

extern s32 mnuStaffSlotsAllFilled(struct EffectList *, StaffSlots *);


extern void mnuInitializeStaffPageWindows(MenuPageWindow *, StaffSlots *, u32, PartyPanel *);

/* Return one while initialization is pending (including state zero), zero when ready.
 * On resource readiness, release loading resources, initialize windows, and store state two. */
s32 mnuTickInitState(MenuProgressHost *work) {
    s32 state = work->loadState;
    StaffSlots *group;

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    group = &work->staffSlots;
    if (mnuStaffSlotsAllFilled(work->titleEffectHandle, group) == 0) {
        return 1;
    }
    mnuReleaseStaffMenuResources(group);
    mnuInitializeStaffPageWindows(&work->partyWindow, group, 0, &work->partyPanel);
    work->loadState = 2;
    return 0;
}

/* Bind the party selection's textures/grid, then create its panel and profile visuals. */
void mnuSetupStaffMenuProfilePage(DatPartyRecord *source, MenuProgressHost *work) {
    MenuPageWindow *window = &work->partyWindow;
    s32 index;

    mnuSeekListNode(mnuFindMatchingPartyEntryIndex(source), work->partyWindow.lists[0]);
    index = work->partyWindow.lists[0]->cursor->index;
    mnuSetWindowResource(index, window, (s32)work->staffSlots.pairResources[0], (s32)work->staffSlots.pairResources[1]);
    mnuAttachPartyIconBundle(index, window, (u32)work->staffSlots.pairResources[0]);
    work->panelGroup = mnuCreatePanelGroup(work->staffSlots.pairResources[0]);
    work->effectResource = mnuAllocateSimpleSprite(
        work->staffSlots.baseResources[5],
        work->staffSlots.baseResources[2],
        work->staffSlots.baseResources[3],
        work->staffSlots.baseResources[0],
        work->staffSlots.pairResources[0]);
    work->currentEffect = mnuCreateProfilePanel(source);
    mnuCacheProfilePanelGridPositions(work->currentEffect, (u32)work->staffSlots.pairResources[1], 5, 14, 15);
    func_00276720(window, 1, 1, 1);
}

/* Release the window's entries/icons before its panel, sprite and profile allocations. */
void mnuReleaseMenuVisualWorkResources(MenuProgressHost *work) {
    mnuClearEntries(&work->partyWindow);
    mnuReleasePartyIconBundles(&work->partyWindow);
    mnuDestroyPanelGroup(work->panelGroup);
    mnuFreeSimpleSpriteWork(work->effectResource);
    mnuFreeProfilePanelWork(work->currentEffect);
}

/* Forward coordinates/mode to the retained profile panel; do not advance other visuals. */
void effUpdateAttached(s32 x, s32 y, s32 mode, void *owner, s32 layer) {
    MenuProgressHost *work = (MenuProgressHost *)owner;
    mnuDrawAndAdvanceProfilePanel(x, y, mode, work->currentEffect, layer);
}

s32 func_00249998(u8 *control, MenuProgressHost *work, s32 context) {
    if (work->loadState != 2) {
        return 0;
    }

    func_00276F70(&work->partyWindow, &work->staffSlots);
    mnuDrawStaffPanelGridBackdrop(0, &work->staffSlots);
    work->partyWindow.flags |= 0x500;
    mnuDrawStageTestList(0, 0, 0, ((s8 *)control)[0x55],
        &work->partyWindow, context);
    func_00283838(0, 0, 0, (s32)control, ((s8 *)control)[0x55],
        (s32)work->effectResource, context);
    return 1;
}

extern s32 D_003BC3E4;
extern char D_003AF590[];
extern char D_003AF620[];
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern s32 kwlnFadeIsActive(void);
extern s32 evtIsActiveFlagSet(s32);

/* Wait for the terminal effect or world-menu transition to finish. */
s32 func_00249A60(s8 closing) {
    MenuTerminalWork *work = (MenuTerminalWork *)kwlnTaskGetUserValue((KwlnTask *)D_003BC3E4);

    if (closing == 1) {
        if (work->mode == 0) {
            if (D_003BC3E1 == 0) {
                mnuFadeOrPlayCloseSfx(0, (u8 *)work);
                D_003BC3E1 = 1;
            } else if (evtIsActiveFlagSet(0) != 0) {
                return 1;
            }
        } else if (D_003BC3E1 == 0) {
            if (work->effectHandle == 0) {
                work->effectHandle = (u32)mnuRequestEffectResource(D_003AF590, D_003AF620);
            } else if (mnuHasEffectResourceHandle((MenuResourceWork *)work->effectHandle) != 0) {
                kwlnFadeOutStart(0, 0, 0, 15);
                D_003BC3E1 = 1;
                work->effectStage = 1;
            }
        } else {
            if (kwlnFadeIsActive() == 0) {
                return 1;
            }
        }
    } else if (work->mode == 0) {
        if (D_003BC3E1 == 1) {
            evtClearActiveFlag(0);
            evtSetBoundedDisplayValue(0, 5);
            evtSetBoundedDisplayValue(1, 0);
            D_003BC3E1 = 0;
        } else if (evtIsActiveFlagSet(0) != 0) {
            return 1;
        }
    } else if (D_003BC3E1 == 1) {
        kwlnFadeInStart(0, 0, 0, 15);
        D_003BC3E1 = -1;
    } else if (kwlnFadeIsActive() == 0) {
        if (work->effectHandle != 0) {
            mnuReleaseEffectResource((MenuResourceWork *)work->effectHandle);
            work->effectHandle = 0;
            work->effectStage = 0;
        }
        D_003BC3E1 = 0;
        return 1;
    }
    return 0;
}

/* Only the exact signed-byte value one enables the world/menu flags; all others disable. */
void mnuSetWorldObjectAndMenuEnabled(s8 enabled) {
    EffWorldNode *worldObject;

    if (enabled == '\x01') {
        worldObject = (EffWorldNode *)(u32)dds3GetWorldObject();
        if (worldObject != 0) {
            dds3SetWorldObjectDrawEnabled(worldObject, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        worldObject = (EffWorldNode *)(u32)dds3GetWorldObject();
        if (worldObject != 0) {
            dds3SetWorldObjectDrawEnabled(worldObject, 0);
        }
        D_003BC3E1 = 0;
    }
}

/* Allocate seven effect batches and seed their two opaque parameter words.
 * EffectPair also carries drawing positions elsewhere, so its fields stay role-neutral. */
void mnuTerminalCreateEffects(MenuTerminalWork *state) {
    EffMappedResource *batch;

    batch = effCreateStatusBatch(1);
    state->effect[0] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 0x14;
    ((EffectPair *)batch->records[0].status)->secondValue = 1;
    batch = effCreateStatusBatch(1);
    state->effect[1] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 0xF;
    ((EffectPair *)batch->records[0].status)->secondValue = 0;
    batch = effCreateStatusBatch(8);
    state->effect[2] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 6;
    ((EffectPair *)batch->records[0].status)->secondValue = 1;
    batch = effCreateStatusBatch(8);
    state->effect[3] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 6;
    ((EffectPair *)batch->records[0].status)->secondValue = 0;
    batch = effCreateStatusBatch(1);
    state->effect[4] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 6;
    ((EffectPair *)batch->records[0].status)->secondValue = 1;
    batch = effCreateStatusBatch(1);
    state->effect[5] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 6;
    ((EffectPair *)batch->records[0].status)->secondValue = 0;
    batch = effCreateStatusBatch(1);
    state->effect[6] = batch;
    ((EffectPair *)batch->records[0].status)->firstValue = 0x78;
    ((EffectPair *)batch->records[0].status)->secondValue = 0;
}

/* Destroy every retained effect batch; the slots and terminal allocation are not cleared. */
void mnuDestroyAllMenuSlotEffectBatches(s32 object) {
    EffMappedResource **batch = ((MenuTerminalWork *)object)->effect;
    u32 i;

    for (i = 0; i < MNU_EFFECT_BATCH_COUNT; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

extern s32 fldGetCurrentBgmHandle(void);
extern void sndEnsureMidiBankResident(u32);

/* Select the native default/current BGM handle and make only its bank bits resident.
 * DDS1's default includes track one; DDS2's default has a zero low halfword. */
void mnuTerminalSelectResourceBank(MenuTerminalWork *work) {
    if (work->mode == 0) {
        work->bgmHandle = MNU_TERMINAL_DEFAULT_BGM;
    } else {
        work->bgmHandle = fldGetCurrentBgmHandle();
    }
    sndEnsureMidiBankResident(work->bgmHandle & MNU_BGM_BANK_MASK);
}

extern void mnuLoadResourceHandles(s32);

extern void mnuTerminalBuildMenus(MenuTerminalWork *host);

extern void evtLoadResourcePair(const char *, void *);

extern s32 evtCreateMessageWindowIfMissing(s32);

/* Allocate the terminal scene and its lists/effects/message resource.
 * Keep the K&R definition and native calls; both cursor slots start at -1. */
INCLUDE_RODATA(const s32, "game/code_00248580", mnuTerminalMenuTemplate);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF620);

u8 *mnuTerminalCreateScene(reduced, slot)
    s32 reduced;
    s32 slot;
{
    s32 handle;
    u8 *obj;
    u32 i;

    handle = (u32)sdfAllocGeneralBlock(MNU_TERMINAL_SCENE_BYTES);
    obj = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)(handle));
    memset(obj, 0, MNU_TERMINAL_SCENE_BYTES);
    ((MenuTerminalWork *)obj)->allocation = handle;
    mnuClearPanelTransitionState(&((MenuTerminalWork *)obj)->transitionWork);
    mnuLoadResourceHandles(obj);
    mnuTerminalCreateEffects((MenuTerminalWork *)obj);
    ((MenuTerminalWork *)obj)->mode = reduced;
    ((MenuTerminalWork *)obj)->reduced = reduced;
    ((MenuTerminalWork *)obj)->initState = slot;
    ((MenuTerminalWork *)obj)->selectedSlot = slot;
    mnuTerminalBuildMenus((MenuTerminalWork *)obj);
    evtLoadResourcePair("/facility/msg/terminal/mes_data.bmd", ((MenuTerminalWork *)obj)->messageResources);
    evtCreateMessageWindowIfMissing(((MenuTerminalWork *)obj)->messageResources[1]);
    for (i = 0; i < MNU_SELECTED_SLOT_COUNT; i++) {
        ((MenuTerminalWork *)obj)->cursor[i] = -1;
    }
    mnuTerminalSelectResourceBank((MenuTerminalWork *)obj);
    return obj;
}

extern void mnuReleaseResourceHandles(u32 *work);
extern void dspCloseChannel(void);
extern void evtReleaseResourcePairHandle(u32 *record);
extern s32 mnuCheckResourceTask(void);
extern void mnuStopResourceTask(void);
extern void func_00126038(s32 a, s32 b);
extern void fldProcessDeferredSceneCommand(void);
extern s8 mnuTerminalTaskState;

/* Release the terminal task's resources/effects, then hand its mode/slot to the field.
 * Native final work-field reads remain after allocation release and outside the NULL guard. */
void mnuReleaseTerminalWorkAndResumeField(KwlnTask *arg) {
    MenuTerminalWork *work = (MenuTerminalWork *)kwlnTaskGetUserValue(arg);

    if (work != NULL) {
        mnuReleaseWorkResources((u8 *)work);
        mnuReleaseResourceHandles((u32 *)work);
        mnuDestroyAllMenuSlotEffectBatches((s32)work);
        mnuDrainPanelTransitions(&work->transitionWork, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle((u32 *)work->messageResources);
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(work->allocation));
        mnuTerminalTaskState = 2;
    }
    if (mnuCheckResourceTask() != 0) {
        mnuStopResourceTask();
    }
    func_00126038(work->mode, work->initState);
    fldProcessDeferredSceneCommand();
}


extern s32 kwlnFadeIsActive(void);


extern s32 evtGetMessageWindowControlState(void);




extern const char D_003AF658[];

extern const char D_003AF668[];

extern const char D_003AF678[];

extern s32 D_003BC3E4;


extern s32 kwlnTaskCreate(const char *, s32, s32, s32, s32 (*)(KwlnTask *), void (*)(KwlnTask *), void *);
extern s32 mnuPrepareTerminalPopupAndDispatch(KwlnTask *task);
extern s32 func_0024A138(KwlnTask *task);
extern s32 func_0024A170(KwlnTask *task);
extern void mnuReleaseTerminalWorkAndResumeField(KwlnTask *task);

/* Create the terminal update/draw/exit tasks sharing one scene.
 * Preserve the legacy no-argument scene constructor call. */
s32 mnuTerminalCreateTasks(void) {
    s32 result;
    void *work = mnuTerminalCreateScene();

    D_003BC3E4 = kwlnTaskCreate(D_003AF658, 0x404, 1, 1, mnuPrepareTerminalPopupAndDispatch, 0, work);
    kwlnTaskCreate(D_003AF668, 0x2B14, 1, 1, func_0024A138, 0, work);
    result = kwlnTaskCreate(D_003AF678, 0x5210, 1, 1, func_0024A170, mnuReleaseTerminalWorkAndResumeField, work);
    mnuTerminalTaskState = 1;
    return result;
}

/* Destroy the three named terminal tasks and clear the retained task handle. */
void fldStopSceneTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003AF658, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF668, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF678, 0);
    D_003BC3E4 = 0;
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

extern char D_0036ADF4[];

/* Seed the task's popup slot before running panel mode zero with the supplied request. */
s32 mnuPrepareTerminalPopupAndDispatch(KwlnTask *value) {
    s32 context = kwlnTaskGetUserValue(value);
    s32 *state = (s32 *)(context + 0x54);

    mnuSetPopupEntry(state, D_0036ADF4);
    return menuRunPanel((void *)context, 0, (void *)value);
}

/* Dispatch current task work through panel mode one; distinct callback role unknown. */
s32 func_0024A138(KwlnTask *value) {
    s32 context = kwlnTaskGetUserValue(value);

    return menuRunPanel((void *)context, 1, (void *)value);
}

/* Dispatch current task work through panel mode two; distinct callback role unknown. */
s32 func_0024A170(KwlnTask *value) {
    s32 context = kwlnTaskGetUserValue(value);

    return menuRunPanel((void *)context, 2, (void *)value);
}

/* Require the fade to be inactive before testing message-window control for idle. */
s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

extern u8 D_0036ACF8[];

s32 func_0024A1D8(s32 action, s32 context) {
    switch (action) {
    case 3:
        if (*(s32 *)(*(s32 *)(context + 0x74) + 0x20) == 0) {
            *(u8 **)(context + 0x58) = D_0036ACF8;
            mnuSetPopupEntry((s32 *)(context + 0x54), D_0036ACF8 + 0xC4);
            *(s32 *)(context + 0x88) = 1;
            **(u32 **)(context + 0x54) |= 0x20000;
            return 1;
        }
        break;
    case 2:
        if (*(s32 *)(*(s32 *)(context + 0x78) + 0x20) == 0) {
            *(u8 **)(context + 0x58) = D_0036ACF8;
            mnuSetPopupEntry((s32 *)(context + 0x54), D_0036ACF8 + 0xC4);
            *(s32 *)(context + 0x88) = 2;
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

extern s32 func_0025C0D8(void *, s32, s32);

s32 func_0024A2D8(s32 state) {
    MenuTerminalWork *work = (MenuTerminalWork *)state;
    s32 result = D_003BC3E1;

    if (result != 0) {
        result = work->reduced < 3;
        if (result && work->reduced > 0) {
            result = work->effectHandle;
            if (result != 0) {
                result = mnuHasEffectResourceHandle((MenuResourceWork *)work->effectHandle);
                if (result != 0) {
                    result = func_0025C0D8((void *)work->effectHandle, 0x80, 0x53);
                }
            }
        }
    }
    return result;
}

extern s32 D_003AF688[3][2];
extern void func_002BF4E0(s32, s32, s32, s32, s32, void *, s32, s32);

/* Draw the terminal panels before advancing their 0..256 blend weight.
 * Reduced modes one/two set an endpoint immediately; normal opening adds twelve,
 * closing subtracts seventeen. Keep the native draw-before-update order. */
void func_0024A340(s32 close, s32 context) {
    MenuTerminalWork *work = (MenuTerminalWork *)context;
    s32 positions[3][2];
    s32 (*position)[2];
    u32 i;

    memcpy(positions, D_003AF688, sizeof(positions));
    if (D_003BC3E1 != 0) {
        if (work->reduced < 3) {
            if (work->reduced > 0) {
                if (close == 0) {
                    work->panelFade = 0x100;
                    return;
                }
                work->panelFade = 0;
                return;
            }
        }
        for (i = 0, position = positions; i < 3; i++, position++) {
            func_002BF4E0((*position)[0], (*position)[1], 0, work->panelFade,
                1, (s32)work->secondResource, i, 0x53);
        }
        if (close == 0) {
            if (work->panelFade < 0x100) {
                work->panelFade += 12;
            }
            if (work->panelFade > 0x100) {
                work->panelFade = 0x100;
            }
        } else {
            if (work->panelFade > 0) {
                work->panelFade -= 17;
            }
            if (work->panelFade < 0) {
                work->panelFade = 0;
            }
        }
    }
}

/* Classify panel fade: zero, nonzero below sixty, or at least sixty.
 * This field is a blend weight, not a remaining-frame countdown. */
s32 fldClassifyRemainingFrames(MenuTerminalWork *work) {
    s32 fade = work->panelFade;
    if (fade == 0) {
        return 0;
    }
    return fade >= 60 ? 2 : 1;
}

/* Configure the current cursor effect; mode three also configures a valid previous slot.
 * A negative current slot prevents every configuration, including the previous slot. */
void mnuTerminalConfigureEffects(u32 mode, MenuTerminalWork *state) {
    s32 *slot = &state->cursor[0];

    if (*slot < 0) {
        return;
    }
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(state->batch, *slot,
                                       state->effect[4], 0, 5, 2);
        break;
    case 2:
        effConfigureWithDefaultSetting(state->batch, *slot,
                                       state->effect[5], 0, 0, 2);
        break;
    case 3:
        effConfigureWithDefaultSetting(state->batch, *slot,
                                       state->effect[4], 0, 0, 2);
        if (slot[1] >= 0) {
            effConfigureWithDefaultSetting(state->batch, slot[1],
                                           state->effect[5], 0, 0, 2);
        }
        break;
    }
}

/* Map a command index to the native four-slot table, then configure its effect.
 * Mode-one index one remaps to three; -2 clears only the previous slot.
 * Other negative indices retain both slots. Nonnegative indices require caller bounds. */
INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF688);

void mnuTerminalSelectSlot(u32 mode, s32 index, MenuTerminalWork *state) {
    s32 table[4] = {3, 1, 2, 0x2D};

    if (state->reduced == 1) {
        if (index == state->reduced) {
            index = 3;
        }
    }
    if (index >= 0) {
        state->cursor[1] = state->cursor[0];
        state->cursor[0] = table[index];
    } else if (index == -2) {
        state->cursor[1] = -1;
    }
    mnuTerminalConfigureEffects(mode, state);
}

/* Draw valid current/previous slots; mode two also runs its native extra draw step. */
void mnuDrawTerminalSelectedSlots(s32 context) {
    MenuTerminalWork *state = (MenuTerminalWork *)context;
    EffectPair position = D_003BC400[0];
    s32 *slot;
    u32 i;

    for (i = 0, slot = state->cursor; i < MNU_SELECTED_SLOT_COUNT; i++, slot++) {
        if (*slot >= 0) {
            itfDrawGridWithResolvedSlot(position.firstValue, position.secondValue, 0, 0x81,
                                        state->batch, *slot, MNU_TEXT_DRAW_PRIORITY);
        }
    }
    if (state->mode == 2) {
        mnuDrawTerminalAmountText(0, context);
    }
}

/* These fades read only the low byte of the live and saved first corner colors. */
extern s32 fldGetModeFrameRecordIndex(MenuTerminalWork *);

/* Terminal display variants 1 and 2 select different effect work entries. */
s32 fldGetModeFrameRecordIndex(MenuTerminalWork *scene) {
    switch (scene->reduced) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_0024A6E8(MenuTerminalWork *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex(scene);
    return (*(u8 *)&scene->batch->workEntries[index].geometry.cornerColors[0]);
}


/* Transition host: two optional callbacks at +0xC4 and the flag at +0xCC that
   picks which value they are called with. */
typedef struct TransitionHost {
    u8 pad00[0xC4];
    void (*callbacks[2])(s32, struct TransitionHost *); /* 0xC4 */
    u32 forceCallbackIndexOne; /* 0xCC */
} TransitionHost;

typedef struct MenuFadeHost {
    u8 pad00[0x7C];
    s32 reduced;      /* 0x7C */
    u8 pad80[0xE0];
    s32 bgmHandle;    /* 0x160: encoded bank/track handle */
} MenuFadeHost;

extern void sndStartTrackExtended(s32);

extern void func_002E9708(void);

extern void func_002E96D8(s32);

extern void func_002E9730(void);


struct EffectSlotSet;
extern void itfSetGridEntryQuantizedAndRefresh(struct EffectSlotSet *, s32, s32, s32, s32, s32);

void func_0024A728(u32 mode, s32 context) {
    MenuTerminalWork *work = (MenuTerminalWork *)context;
    s32 index;
    s32 *entries;
    s32 i;

    index = fldGetModeFrameRecordIndex(work);
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(work->batch, index,
                                       work->effect[0], 0, 0, 2);
        effConfigureWithDefaultSetting(work->batch, 4,
                                       work->effect[6], 0, 0, 14);
        effConfigureWithDefaultSetting(work->batch, 6,
                                       work->effect[0], 0, 0, 2);
        itfSetGridEntryQuantizedAndRefresh(work->batch, 7, 0, 0, -0x400, 0);
        effConfigureWithDefaultSetting(work->batch, 7,
                                       work->effect[5], 0, 5, 3);
        i = 0;
        entries = (s32 *)work->alternateBatch->workEntries[0].geometry.cornerColors;
        for (; i < 4; i++) {
            entries[i] = 0;
        }
        break;
    case 2:
        effConfigureWithDefaultSetting(work->batch, index,
                                       work->effect[3], 0, 0xF, 2);
        effConfigureWithDefaultSetting(work->batch, 6,
                                       work->effect[3], 0, 0xF, 2);
        itfSetGridEntryQuantizedAndRefresh(work->batch, 7, 0, 0, 0, 0);
        effConfigureWithDefaultSetting(work->batch, 7,
                                       work->effect[3], 0, 0, 2);
        return;
    case 3:
        itfSetGridEntryQuantizedAndRefresh(work->batch, 7, 0, 0, -0x400, 0);
        effConfigureWithDefaultSetting(work->batch, 7,
                                       work->effect[5], 0, 0, 3);
        effConfigureWithDefaultSetting(work->alternateBatch, 0,
                                       work->effect[5], 0, 0, 2);
        break;
    }
}

extern const s32 D_003AF6B0[6][2];

/* Draw the terminal's frame layers and the selected resource's progress. */
void func_0024A930(MenuTerminalWork *work) {
    s32 positions[6][2];
    s32 index;
    u32 scale;

    memcpy(positions, D_003AF6B0, sizeof(positions));
    index = fldGetModeFrameRecordIndex(work);
    itfDrawGridWithResolvedSlot(positions[0][0], positions[0][1], 0, 0x81,
        work->batch, index, MNU_TEXT_DRAW_PRIORITY);
    scale = ((u32)(*(u8 *)&work->batch->workEntries[index].geometry.cornerColors[0]) << 8) /
        (*(u8 *)&work->batch->workEntries[index].savedColors[0]);
    if (work->mode != 2) {
        func_002BF4E0(positions[1][0], positions[1][1], 0, scale, 0x81,
            work->batch, 4, MNU_TEXT_DRAW_PRIORITY);
    }
    itfDrawGridWithResolvedSlot(positions[3][0], positions[3][1], 0, 0x81,
        work->batch, 7, MNU_TEXT_DRAW_PRIORITY);
    itfDrawGridWithResolvedSlot(positions[3][0], positions[3][1], 0, 0x81,
        work->alternateBatch, 0, MNU_TEXT_DRAW_PRIORITY);
    if (work->reduced != 2) {
        itfDrawGridWithResolvedSlot(positions[2][0], positions[2][1], 0, 0x81,
            work->batch, 6, MNU_TEXT_DRAW_PRIORITY);
        mnuQueueFontGlyphFromAtlasSlot(positions[4][0], positions[4][1], 0,
            (*(u8 *)&work->batch->workEntries[index].geometry.cornerColors[0]) | 0xA09DC300,
            work->selectedSlot, work->reduced);
    } else {
        func_002BF4E0(positions[5][0], positions[5][1], 0, scale, 0x81,
            work->batch, 0x33, MNU_TEXT_DRAW_PRIORITY);
    }
}


typedef struct {
    u8 pad00[0x70];
    MenuProgressOwner *owner;
} MenuSelectorContext;

s32 func_0024AB28(MenuSelectorContext *context) {
    s32 value = context->owner->selectionState;

    switch (value) {
    case 2:
        return 0x35;
    case 3:
        return 0x37;
    case 4:
        return 0x34;
    default:
        return 5;
    }
}

/* Update the old and selected terminal-effect slots from the chosen mode. */
void func_0024AB70(u32 mode, s32 context) {
    MenuTerminalWork *work = (MenuTerminalWork *)context;
    s32 effect = 0;
    s32 style = 0;
    s32 layer = 0;
    s32 width = 0;
    s32 height = 0;
    s32 selected;
    s32 resourceOffset;
    EffMappedResource **resource;

    switch (mode) {
    case 1:
        width = -256;
        layer = 2;
        style = 5;
        effect = 2;
        break;
    case 3:
        width = -256;
        height = 0;
        layer = 2;
        style = 7;
        effect = 2;
        break;
    case 4:
        width = 0;
    case 2:
        height = 256;
        layer = 2;
        style = 4;
        effect = 3;
        break;
    }
    resourceOffset = sizeof(work->effect[0]) * effect;
    resourceOffset += (s32)((u8 *)&work->effect[0] - (u8 *)work);
    resource = (EffMappedResource **)((u8 *)work + resourceOffset);
    itfSetGridEntryQuantizedAndRefresh(work->batch, 8, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(work->batch, 8,
                                   *resource, 0, style, layer);
    selected = func_0024AB28((MenuSelectorContext *)work);
    itfSetGridEntryQuantizedAndRefresh(work->batch, selected, 0, width, 0, height);
    effConfigureWithDefaultSetting(work->batch, selected,
                                   *resource, 0, style, layer);
}



extern s32 D_003AF6E0[2][2];
typedef struct MenuGridPositions {
    EffectPair entries[2];
} MenuGridPositions;
extern const MenuGridPositions D_003AF6F0;
extern void mnuCallInitWide(s32, s32, s32, s32, s32);

void func_0024ACD8(s32 close, s32 context) {
    MenuTerminalWork *work = (MenuTerminalWork *)context;
    s32 positions[2][2];
    u32 progress;
    EffectSlotSet *frames;

    memcpy(positions, D_003AF6E0, sizeof(positions));
    itfDrawGridWithResolvedSlot(positions[1][0], positions[1][1], 0, 0x80,
        work->batch, func_0024AB28((MenuSelectorContext *)work), 0x53);
    mnuCallInitWide(0x330, 0x340, 0, (s32)work->listResource, 0x53);
    itfDrawGridWithResolvedSlot(positions[0][0], positions[0][1], 0, 0x80,
        work->batch, 8, 0x53);
    frames = work->batch;
    progress = ((u32)(*(u8 *)&frames->workEntries[8].geometry.cornerColors[0]) << 8) /
               (*(u8 *)&frames->workEntries[8].savedColors[0]);
    if (close != 0) {
        if (work->listResource->scale > 0) {
            work->listResource->scale -= 0x40;
        }
        if (work->listResource->scale < 0) {
            work->listResource->scale = 0;
        }
    } else if (progress == 0x100) {
        if (work->listResource->scale < 0x100) {
            work->listResource->scale += 0x40;
        }
        if (work->listResource->scale > 0x100) {
            work->listResource->scale = 0x100;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AE18);

void mnuDrawOwnerProgressAndFade(s32 close, s32 context) {
    MenuTerminalWork *work = (MenuTerminalWork *)context;
    MenuGridPositions positions = D_003AF6F0;
    u32 progress;
    EffectSlotSet *frames;

    itfDrawGridWithResolvedSlot(positions.entries[1].firstValue, positions.entries[1].secondValue, 0, 0x80,
                                work->batch, 10, 0x53);
    mnuCallInitWide(0x330, 0x2E8, 0, (s32)work->owner, 0x53);
    itfDrawGridWithResolvedSlot(positions.entries[0].firstValue, positions.entries[0].secondValue, 0, 0x80,
                                work->batch, 9, 0x53);
    frames = work->batch;
    progress = ((u32)(*(u8 *)&frames->workEntries[9].geometry.cornerColors[0]) << 8) /
               (*(u8 *)&frames->workEntries[9].savedColors[0]);
    if (close != 0) {
        if (work->owner->scale > 0) {
            work->owner->scale -= 0x40;
        }
        if (work->owner->scale < 0) {
            work->owner->scale = 0;
        }
    } else if (progress == 0x100) {
        if (work->owner->scale < 0x100) {
            work->owner->scale += 0x40;
        }
        if (work->owner->scale > 0x100) {
            work->owner->scale = 0x100;
        }
    }
}

typedef struct GridPanelHost {
    u8 pad00[0x64];
    s32 grid;           /* 0x64 */
    u8 pad68[0x38];
    s32 settings[1];    /* 0xA0 */
} GridPanelHost;



/* Reset grid entry 0x1A, then configure it from the panel's setting slot chosen by `kind`. */
void mnuApplyGridPanelHostSetting(u32 kind, GridPanelHost *host) {
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
    itfSetGridEntryQuantizedAndRefresh(host->grid, 0x1A, 0, 0, 0, 0);
    effConfigureWithDefaultSetting((struct EffectSlotSet *)(u32)host->grid, 0x1A,
                                   (struct EffMappedResource *)(u32)host->settings[slot], 0, value, flags);
}

extern const MenuGridPositions D_003AF700;

/* Slide the threshold list out while closing, or in after its grid entry completes. */
void func_0024B168(s32 close, MenuTerminalWork *work) {
    MenuGridPositions positions = D_003AF700;
    s32 offset = 0;
    s32 direction = 0;
    s32 scale;

    itfDrawGridWithResolvedSlot(positions.entries[0].firstValue,
        positions.entries[0].secondValue, 0, 0x80, work->batch, 0x1A, 0x53);
    scale = ((u32)(*(u8 *)&work->batch->workEntries[0x1A].geometry.cornerColors[0]) << 8) /
        (*(u8 *)&work->batch->workEntries[0x1A].savedColors[0]);
    if (close != 0) {
        if (work->list->scale > 0) {
            work->list->scale -= 0x40;
        }
        if (work->list->scale < 0) {
            work->list->scale = 0;
        }
        direction = -1;
    } else if (scale == 0x100) {
        if (work->list->scale < 0x100) {
            work->list->scale += 0x40;
        }
        if (work->list->scale > 0x100) {
            work->list->scale = 0x100;
        }
        direction = 1;
    }
    scale = work->list->scale;
    if (direction < 0) {
        offset = (0x100 - scale) / 2;
    }
    if (direction > 0) {
        offset = -((0x100 - scale) / 2);
    }
    mnuCallInitWide(0, offset + 0x4F0, 0, (s32)work->list, 0x53);
}

typedef struct {
    u8 pad00[0xC4];
    u32 callback;         /* 0xC4 */
    u32 previousCallback; /* 0xC8 */
} SceneTransition;

/* Install the new transition callback while retaining the previous callback address. */
void evtRememberDispatchCallback(u32 callback, SceneTransition *transition) {
    u32 previous;

    previous = transition->callback;
    transition->callback = callback;
    transition->previousCallback = previous;
}

/* Dispatch registered transition callbacks, optionally forcing index one. */
void mnuDispatchTransitionHostCallbacks(TransitionHost *host) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (host->callbacks[i] != NULL) {
            host->callbacks[i](host->forceCallbackIndexOne ? 1 : (s32)i, host);
        }
    }
}

/* Mode zero starts the selected BGM in full mode or invokes the reduced-mode action.
 * Other modes use their counterpart actions; keep the native selection field. */
void mnuApplyFadeTrackMode(s32 mode, MenuFadeHost *host) {
    if (mode == 0) {
        if (host->reduced == 0) {
            sndStartTrackExtended(host->bgmHandle);
        } else {
            func_002E9708();
        }
    } else if (host->reduced == 0) {
        func_002E96D8(host->bgmHandle);
    } else {
        func_002E9730();
    }
}




extern void evtFinishMessageWindowAndNotify(void);

extern void func_0024DD78(void);




extern void func_0024A340(s32, s32);



extern void mnuDrawTerminalSelectedSlots(s32);

/* Offsets shared by the event-B menu/dispatch handlers in this unit. */

typedef struct EvtBContext {
    u8 pad00[0x54];
    s32 dispatchState; /* 0x54 */
    u32 dispatchTable; /* 0x58 */
    u8 pad5C[0x14];
    struct MenuList *visualList; /* 0x70 */
    struct MenuList *thresholdList; /* 0x74 */
    struct MenuList *selectionList; /* 0x78 */
    s32 state7C;       /* 0x7C: nonzero also re-requests the effect resource */
    u8 pad80[0x8];
    s32 panelMode; /* 0x88 */
    u8 pad8C[0x40]; /* 0x98: reset flag meaning still unclear */
    s32 exitPending; /* 0xCC */
    s32 transitionPending; /* 0xD0 */
    s32 transitionStage; /* 0xD4 */
    s32 menuActive;     /* 0xD8: cleared when the menu command chain ends */
    s32 selectionStep;  /* 0xDC: nonzero once the selection chain is running */
    u8 padE0[0x78];
    s32 effectHandle;   /* 0x158: effect resource handle */
    s32 dispatchMode; /* 0x15C */
    u32 bgmHandle;    /* 0x160: encoded bank/track handle */
} EvtBContext;





INCLUDE_ASM(const s32, "game/code_00248580", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

extern s32 func_0024A1D8(s32 action, s32 context);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
extern void mnuPlayInputSound(s32 mode, s32 buttons, u32 *flags);
extern s32 D_0036AC80[];
extern u8 D_0036ACF8[];
extern u8 D_0036AD30[];
extern u8 D_0036AD68[];
extern u8 D_0036ADA0[];

/* Event-B panel input: confirm opens the popup for the selected entry's action, cancel opens the back popup, left/right step the list. */
s32 evtBHandleSelectionPanelInput(KwlnTask *input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue(input);
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *state = &context->dispatchState;
    s32 kind = context->visualList->cursor->index;
    s32 frames;
    s32 action;
    struct MenuListNode *node;
    s32 result;

    result = menuRunPanel(context, 0, input);
    if (result != 0) {
        return result;
    }
    frames = fldClassifyRemainingFrames((MenuTerminalWork *)context);
    if (frames != 2) {
        return 0;
    }
    if (context->visualList->scale < 0x100) {
        return 0;
    }
    if (*state == 0) {
        if (buttons & 1) {
            node = context->visualList->cursor;
            action = D_0036AC80[func_00249198() * 5 + context->state7C * 10 + kind];
            if (!(node->flags48 & 1) || action == 3 || action == frames) {
                if (func_0024A1D8(action, (s32)context) == 0) {
                    switch (action) {
                    case 2:
                        if (context->selectionList->count == 1) {
                            mnuSetPopupEntryFlagged(state, D_0036ADA0);
                        } else {
                            mnuSetPopupEntryFlagged(state, D_0036AD30);
                        }
                        break;
                    case 1:
                        kwlnFadeInStart(0, 0, 0, 0xF);
                    default:
                        mnuSetPopupEntryFlagged(state, D_0036ACF8 + action * 28);
                        break;
                    }
                }
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(state, D_0036AD68);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo(&context->visualList->stateFlags);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault(context->visualList);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault(context->visualList);
        }
        mnuPlayInputSound(0, buttons, &context->visualList->stateFlags);
    }
    return 0;
}


s32 evtDispatchSelectionAfterFieldFrameGate(KwlnTask *request) {
    s32 state = kwlnTaskGetUserValue(request);

    func_0024A2D8(state);
    func_0024A340(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930((MenuTerminalWork *)state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel((void *)state, 1, request);
}

s32 evtBSetupDispatchSync(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}

s32 evtBClearAndReset(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->visualList->scale = 0;
    mnuSetWorldObjectAndMenuEnabled(0);
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern void func_0024AB70(u32, s32);

u32 evtBeginSelectionExitFade(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    mnuTerminalSelectSlot(0, -2, context);
    mnuSetWorldObjectAndMenuEnabled(1);
    mnuReleaseVisualResources((MenuTerminalWork *)context);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern s32 fileMenuTaskExists(void);
extern void fileSetPreviewLocation();
extern void fileEnterMcPackScene(s32 mode);

s32 func_0024B868(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    s32 *dispatch = (s32 *)(context + 0x54);
    s32 result = menuRunPanel((void *)context, 0, (void *)request);

    if (result != 0) {
        return result;
    }
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    if (*(s32 *)(context + 0x94) == 0 &&
        sdfCheckPendingWorkWithInterrupts() == 0) {
        mnuReleaseBothVisualResourceTextures((MenuTerminalWork *)context);
        fileSetPreviewLocation(*(s32 *)(context + 0x7C),
            *(s32 *)(context + 0x80));
        fileEnterMcPackScene(0);
        *(s32 *)(context + 0x94) = 1;
    }
    if (*(s32 *)(context + 0x54) == 0 && fileMenuTaskExists() == 0 &&
        sdfCheckPendingWorkWithInterrupts() == 0) {
        *(s32 *)(context + 0x94) = 0;
        mnuSetPopupEntryFlagged(dispatch, D_0036ACF8);
    }
    return 0;
}

s32 evtBDispatchStart(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    return menuRunPanel((void *)context, 1, (void *)request);
}

s32 evtBSetupDispatchSyncB(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}

extern void mnuSelectFirstListNode(struct MenuList *);
extern void mnuSelectLastListNode(struct MenuList *);
extern void func_0024A728(u32, s32);
extern void func_0024AE18(s32, s32);


u32 evtInitializeSelectionListWhenReady(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    if (((EvtBContext *)context)->transitionPending == 0) {
        mnuSelectFirstListNode(((EvtBContext *)context)->selectionList);
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)mnuDrawOwnerProgressAndFade, context);
        func_0024AE18(3, context);
        func_0024AB70(4, context);
        mnuTerminalSelectSlot(3, 1, context);
    }
    ((EvtBContext *)context)->transitionPending = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}


u32 evtFinishPendingSelectionTransition(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    if (((EvtBContext *)context)->transitionPending != 0) {
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024ACD8, context);
        func_0024AE18(4, context);
        func_0024AB70(3, context);
        mnuTerminalSelectSlot(3, 0, context);
        evtFinishMessageWindowAndNotify();
    }
    ((EvtBContext *)context)->transitionPending = 0;
    return 1;
}

s32 func_0024BB00(KwlnTask *input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue(input);
    s32 *state = &context->dispatchState;
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 result;

    result = menuRunPanel(context, 0, input);
    if (result != 0) {
        return result;
    }
    if (context->selectionList->scale < 0x100) {
        return 0;
    }
    if (*state == 0) {
        if (buttons & 1) {
            mnuSetPopupEntryFlagged(state, D_0036ADA0);
        }
        if (buttons & 2) {
            context->transitionPending = 1;
            mnuSetPopupEntryFlagged(state, D_0036ACF8);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo(&context->selectionList->stateFlags);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault(context->selectionList);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault(context->selectionList);
        }
        mnuPlayInputSound(0, buttons, &context->selectionList->stateFlags);
    }
    return 0;
}

s32 func_0024BC18(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930((MenuTerminalWork *)state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBSetupDispatchSyncC(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}

extern void func_0024B168(s32, MenuTerminalWork *);

u32 evtEnterThresholdSelectionList(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    mnuRefreshThresholdNodeFlags(((EvtBContext *)context)->thresholdList);
    mnuSelectFirstListNode(((EvtBContext *)context)->thresholdList);
    mnuTerminalSelectSlot(3, 2, context);
    mnuApplyGridPanelHostSetting(3, context);
    func_0024AB70(4, context);
    evtRememberDispatchCallback((s32)func_0024B168, context);
    return 1;
}

extern void mnuHighlightProgressNodeByMode(s32);

u32 evtBEnterStateA(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    mnuTerminalSelectSlot(3, 0, context);
    mnuApplyGridPanelHostSetting(4, context);
    func_0024AB70(3, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    mnuHighlightProgressNodeByMode(context);
    return 1;
}

/* Process recovery input only while popup dispatch is idle. A successful
 * purchase restores the selected party member, releases its panel and charges
 * the stored cost; unavailable entries replace confirmation with error sound. */
s32 func_0024BDB8(KwlnTask *request) {
    MenuTerminalWork *host = (MenuTerminalWork *)kwlnTaskGetUserValue(request);
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *dispatch = &host->popupState;
    s32 result = func_00285670(&host->transitionWork, dispatch, 0, request);
    struct MenuListNode *node;
    MenuThresholdEntry *entry;

    if (result != 0) {
        return result;
    }
    if (*dispatch == 0) {
        if (buttons & 1) {
            node = ((struct MenuList *)host->list)->cursor;
            if (!(node->flags48 & 1)) {
                entry = &node->terminal;
                buttons = 0;
                fldSaveSceneOptionsAndClearFlags(&datGameState->party[entry->entryId]);
                sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
                mnuReleaseSelectedProgressPanel(host);
                datAddCurrencyClamped(-entry->requiredAmount);
                mnuRefreshThresholdNodeFlags(host->list);
                if (((struct MenuList *)host->list)->count == 0) {
                    mnuSetPopupEntryFlagged(dispatch, D_0036ACF8);
                }
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(dispatch, D_0036ACF8);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo(&((struct MenuList *)host->list)->stateFlags);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault((struct MenuList *)host->list);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault((struct MenuList *)host->list);
        }
        mnuPlayInputSound(0, buttons, &((struct MenuList *)host->list)->stateFlags);
    }
    return 0;
}


void mnuDrawTerminalAmountText(s32 fading, s32 context) {
    MenuTerminalWork *scene = (MenuTerminalWork *)context;
    char text[16];
    s32 index;
    s32 color;
    u32 sprite;

    index = fldGetModeFrameRecordIndex(scene);
    func_003014F0(text, D_003BC3F0, datGameState->header.currency);
    if (fading == 0) {
        color = (*(u8 *)&scene->batch->workEntries[index].geometry.cornerColors[0]) | 0xA09DC300;
    } else {
        color = uiBlendColors(0xA09DC380, 0xA09DC300, scene->list->scale);
    }
    sprite = func_001979C8(0x1740, 0x210, 0, color, text, 0);
    frFontDrawGlyphChain(sprite, 1, 0x53);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)sprite);
}

extern void mnuDrawTerminalAmountText(s32, s32);

s32 mnuInitializeSelectionDispatchWhenModeUnset(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930((MenuTerminalWork *)state);
    mnuDrawTerminalSelectedSlots(state);
    if (*(s32 *)(state + 0x7C) == 0) {
        mnuDrawTerminalAmountText(1, state);
    }
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBSetupDispatchSyncD(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}

u32 evtSelectFinalVisualNode(KwlnTask *task) {
    s32 context;

    context = kwlnTaskGetUserValue(task);
    mnuSelectLastListNode(((EvtBContext *)context)->visualList);
    return 1;
}

/* Terminal panel poll: once the message window is idle and the field frames are drained, open the follow-up popup. */
extern u8 D_0036AE2C[];
s32 evtOpenTerminalFollowupPopupWhenIdle(KwlnTask *request) {
    s32 state = kwlnTaskGetUserValue(request);
    s32 *panel = (s32 *)(state + 0x54);
    s32 result = menuRunPanel((void *)state, 0, (void *)request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (fldClassifyRemainingFrames(state) == 0) {
                mnuSetPopupEntry(panel, (void *)D_0036AE2C);
            }
        }
    }
    return 0;
}



s32 func_0024C1B8(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);

    if (evtGetMessageWindowControlState() == 0 && ((EvtBContext *)state)->exitPending == 0) {
        func_0024A728(2, state);
        mnuTerminalSelectSlot(2, -1, (MenuTerminalWork *)state);
        func_0024AB70(2, state);
        ((EvtBContext *)state)->exitPending = 1;
    }
    func_0024A2D8(state);
    if (((EvtBContext *)state)->selectionStep == 0) {
        func_0024A340(1, state);
    } else if (func_0024A6E8((MenuTerminalWork *)state) == 0) {
        func_0024A340(1, state);
    } else {
        func_0024A340(0, state);
    }
    mnuDispatchTransitionHostCallbacks((TransitionHost *)state);
    func_0024A930((MenuTerminalWork *)state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBSetupDispatchSyncE(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}


u32 evtBReleaseImagesAndQueueMenuTransition(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    mnuReleaseStaffImageHandles(((MenuTerminalWork *)context)->imageHandles);
    func_0024A728(2, context);
    mnuTerminalSelectSlot(2, -1, context);
    func_0024AB70(2, context);
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    *(s32 *)(context + 0x98) = 0;
    evtFinishMessageWindowAndNotify();
    dspCloseChannel();
    func_002E96D8(((EvtBContext *)context)->bgmHandle);
    return 1;
}

s32 mnuOpenTerminalSelectionMessageWindow(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    mnuResolveStaffImageHandles(((MenuTerminalWork *)context)->imageHandles);
    func_0024A728(1, context);
    mnuTerminalSelectSlot(1, 0, (MenuTerminalWork *)context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    ((EvtBContext *)context)->exitPending = 0;
    evtCreateMessageWindowIfMissing(((MenuTerminalWork *)context)->messageResources[1]);
    if (((EvtBContext *)context)->state7C != 0) {
        sndStartTrackExtended(((EvtBContext *)context)->bgmHandle);
    }
    return 1;
}

/* Sequence resource teardown, reload and fade before returning to the terminal menu. */
s32 func_0024C3F8(KwlnTask *request) {
    MenuTerminalWork *work = (MenuTerminalWork *)kwlnTaskGetUserValue(request);
    s32 result = menuRunPanel(work, 0, (void *)request);

    if (result != 0) {
        return result;
    }
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    if (work->resourcePhase == 0) {
        if (fldClassifyRemainingFrames(work) == 0) {
            mnuReleaseBothVisualResourceTextures(work);
            func_00249A60(0);
            work->resourcePhase = 1;
        }
    } else if (work->resourcePhase == 1) {
        if (func_00249A60(0) != 0 && sdfCheckPendingWorkWithInterrupts() == 0) {
            mnuCreateResourceTask();
            kwlnFadeOutStart(0, 0, 0, 0);
            work->resourcePhase = 2;
        }
    }
    if (work->popupState == 0) {
        if (work->resourcePhase == 2) {
            if (mnuCheckResourceTask() != 0 || sdfCheckPendingWorkWithInterrupts() != 0) {
                return 0;
            }
            mnuReleaseVisualResources(work);
            func_00249A60(1);
            work->resourcePhase = 3;
        } else if (work->resourcePhase == 3) {
            if (func_00249A60(1) != 0) {
                mnuTerminalSelectResourceBank(work);
                work->resourcePhase = 0;
                mnuSetPopupEntryFlagged(&work->popupState, D_0036ACF8);
            }
        }
    }
    return 0;
}

/* While the panel fade is nonzero, choose its transition direction from the selection chain, then run the panel. */
s32 evtBPollSelectionChainPanel(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);

    func_0024A2D8(state);
    if (fldClassifyRemainingFrames((MenuTerminalWork *)state) != 0) {
        if (((EvtBContext *)state)->selectionStep == 0) {
            func_0024A340(1, state);
        } else if (func_0024A6E8((MenuTerminalWork *)state) == 0) {
            func_0024A340(1, state);
        } else {
            func_0024A340(0, state);
        }
        mnuDispatchTransitionHostCallbacks((TransitionHost *)state);
        func_0024A930((MenuTerminalWork *)state);
        mnuDrawTerminalSelectedSlots(state);
    }
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBDispatchSync(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    return menuRunPanel((void *)context, 2, (void *)request);
}

extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern void dspSetActive(s32);
extern void dspStartEntry(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void evtStoreValueAndCaptureWindowPanelValue(s32);

/* Bind the selected text row to message slot zero, then start the entry prompt. */
u32 evtPrepareSelectedMenuEntry(KwlnTask *task) {
    s32 state = kwlnTaskGetUserValue(task);
    struct MenuList *owner = ((EvtBContext *)state)->selectionList;
    u32 *selectionIndex = &owner->cursor->sortKeyPrimary;

    if (owner->count == 1) {
        mnuSelectFirstListNode(owner);
    }
    evtCopyEntryStringToActiveWindow(0, D_00347C68[*selectionIndex].encodedText);
    dspSetActive(1);
    dspStartEntry(0);
    evtSetMessageWindowOptionWhenOpen(1);
    evtStoreValueAndCaptureWindowPanelValue(6);
    return 1;
}

u32 func_0024C6F8(void) {
    return 1;
}

extern s32 evtGetCapturedWindowPanelValue(void);
extern u8 D_0036ADD8[];

s32 evtBChooseSelectionCompletionPopup(KwlnTask *input) {
    EvtBContext *context;
    s32 *state;
    s32 result;

    context = (EvtBContext *)kwlnTaskGetUserValue(input);
    state = &context->dispatchState;
    result = menuRunPanel(context, 0, input);
    if (result == 0) {
        if (*state == 0) {
            result = evtGetMessageWindowControlState();
            if (result == 0) {
                if (evtGetCapturedWindowPanelValue() == 0) {
                    mnuResetProgressModeFromOwner((u8 *)context);
                    mnuSetPopupEntryFlagged(state, D_0036ADD8);
                } else if (context->selectionList->count >= 2) {
                    context->transitionPending = 1;
                    mnuSetPopupEntryFlagged(state, D_0036AD30);
                } else {
                    mnuSetPopupEntryFlagged(state, D_0036ACF8);
                }
            }
        }
        result = 0;
    }
    return result;
}

s32 func_0024C7E8(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930((MenuTerminalWork *)state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBSetupDispatchSyncF(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}


u32 evtBCheckPanelMode(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    dspSetActive(1);
    switch (((EvtBContext *)context)->panelMode) {
    case 1:
        dspStartEntry(1);
        break;
    case 2:
        dspStartEntry(2);
        break;
    }
    return 1;
}

s32 evtBContinueDispatchOrRestoreTable(KwlnTask *input) {
    s32 context;
    s32 dispatchResult;
    s32 *dispatchState;

    context = kwlnTaskGetUserValue(input);
    dispatchState = &((EvtBContext *)context)->dispatchState;
    dispatchResult = menuRunPanel((void *)context, 0, input);
    if (dispatchResult == 0) {
        if ((*dispatchState == 0) && (dispatchResult = evtGetMessageWindowControlState(), dispatchResult == 0)) {
            mnuSetPopupEntry(dispatchState, ((EvtBContext *)context)->dispatchTable);
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

s32 mnuPrepareDispatchStateAndBindHandler(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930((MenuTerminalWork *)state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBSetupDispatchSyncG(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)request);
}


u32 evtExitSelectionMenuAndSendSoundCommand(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    func_0024A728(2, context);
    if (((EvtBContext *)context)->transitionStage >= 2) {
        mnuTerminalSelectSlot(2, -1, context);
        func_0024AE18(2, context);
    } else {
        mnuTerminalSelectSlot(2, -1, context);
        func_0024AB70(2, context);
    }
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    func_002E96D8(((EvtBContext *)context)->bgmHandle);
    evtClearActiveFlag(0);
    return 1;
}


u32 evtBRebuildTerminalMenuAndResetDispatch(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);

    mnuReleaseWorkResources(context);
    mnuTerminalBuildMenus(context);
    func_0024A728(1, context);
    mnuTerminalSelectSlot(1, 0, context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    ((EvtBContext *)context)->dispatchMode = 0;
    ((EvtBContext *)context)->exitPending = 0;
    return 1;
}


s32 func_0024CB80(KwlnTask *input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue(input);
    s32 *state = &context->dispatchState;
    s32 result;

    result = menuRunPanel(context, 0, input);
    if (result != 0) {
        return result;
    }
    if (*state == 0) {
        if (fldClassifyRemainingFrames((MenuTerminalWork *)context) == 0) {
            if (context->selectionStep == 0) {
                evtSetBoundedDisplayValue(0, 4);
            } else {
                switch (context->dispatchMode) {
                case 1:
                    kwlnFadeInStart(0, 0, 0, 15);
                    context->dispatchMode = 2;
                    break;
                case 2:
                    if (kwlnFadeIsActive() == 0) {
                        if (context->effectHandle != 0) {
                            mnuReleaseEffectResource((MenuResourceWork *)context->effectHandle);
                            context->effectHandle = 0;
                        }
                        mnuFadeOrPlayCloseSfx(0, (u8 *)context);
                        mnuTerminalSelectResourceBank((MenuTerminalWork *)context);
                        context->dispatchMode = 3;
                    }
                    break;
                }
            }
        }
        if (evtIsActiveFlagSet(0) != 0) {
            evtClearActiveFlag(0);
            evtSetBoundedDisplayValue(0, 2);
            mnuSetPopupEntryFlagged(state, D_0036ACF8);
        }
    }
    return 0;
}


s32 evtBDispatchSyncD2(KwlnTask *item) {
    s32 state = kwlnTaskGetUserValue(item);

    func_0024A2D8(state);
    if (func_0024A6E8((MenuTerminalWork *)state) == 0) {
        func_0024A340(1, state);
    } else {
        func_0024A340(0, state);
    }
    mnuDispatchTransitionHostCallbacks(state);
    if (((EvtBContext *)state)->dispatchMode != 3) {
        func_0024A930((MenuTerminalWork *)state);
    }
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel((void *)state, 1, (void *)item);
}

s32 evtBDispatchSyncB(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    return menuRunPanel((void *)context, 2, (void *)request);
}

extern char D_003AF590[];
extern char D_003AF620[];

u32 evtBEndDispatchAndReloadEffectResource(KwlnTask *task) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue(task);

    mnuFadeOrPlayCloseSfx(0, (s32)context);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    if (context->state7C != 0) {
        context->effectHandle = (u32)mnuRequestEffectResource(D_003AF590, D_003AF620);
    }
    return 1;
}

u32 func_0024CE20(void) {
    return 1;
}

extern const char D_003AF710[];
extern u8 D_0036AE10[];

s32 func_0024CE28(KwlnTask *input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue(input);
    s32 *state = &context->dispatchState;
    s32 canOpen = 0;
    s32 result = menuRunPanel(context, 0, input);

    if (result == 0) {
        if (*state == 0) {
            if (context->state7C == 0) {
                canOpen = kwlnTaskGetTaskByName(D_003AF710) == NULL;
                if (evtIsActiveFlagSet(0) != 0) {
                    canOpen = 1;
                }
            } else {
                if (mnuHasEffectResourceHandle((MenuResourceWork *)context->effectHandle) != 0) {
                    if (context->dispatchMode == 0) {
                        kwlnFadeOutStart(0, 0, 0, 15);
                        context->dispatchMode = 1;
                    }
                }
                if (context->dispatchMode != 0) {
                    canOpen = kwlnFadeIsActive() == 0;
                }
            }
            if (canOpen != 0) {
                mnuSetPopupEntryFlagged(state, D_0036AE10);
            }
        }
        return 0;
    }
    return result;
}

s32 evtBLateDispatchStart(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    func_0024A2D8(context);
    return menuRunPanel((void *)context, 1, (void *)request);
}

s32 evtBDispatchSyncC(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);

    return menuRunPanel((void *)context, 2, (void *)request);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF700);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF710);

INCLUDE_SDATA(const s32, "game/code_00248580", mnuTerminalTaskState);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E1);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E4);

INCLUDE_SDATA(const s32, "game/code_00248580", mnuNumberSpriteFormat);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F0);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F8);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC400);

