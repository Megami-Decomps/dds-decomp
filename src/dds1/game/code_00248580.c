#include "common.h"
#include "mnu.h"
#include "kwln.h"

#define MNU_PARTY_SLOT_COUNT 5
#define MNU_PARTY_RECORD_BYTES 0x1A4
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


extern s32 func_0027B888(u32);

extern s8 D_003BC3E1;

extern s32 dds3GetWorldObject(void);

extern s32 mdlFlagTest(u32);


extern s32 datGameState;

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
    s32 panel; /* 0x70: allocated panel resource */
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

typedef struct MenuVisualWork {
    u8 pad00[8];
    u32 color;           /* 0x08 */
    u8 pad0C[4];
    u32 y;               /* 0x10 */
    u32 z;               /* 0x14 */
    u8 pad18[4];
    u32 x;               /* 0x1C */
    u8 pad20[4];
    u32 texture;         /* 0x24 */
    u32 grid;            /* 0x28 */
    u8 pad2C[0x38];
    u32 firstResource;   /* 0x64 */
    u32 secondResource;  /* 0x68 */
    u8 pad6C[0x124];
    u8 window[0x67C];    /* 0x190: window prefix before its list pointers */
    MenuProgressOwner *windowList; /* 0x80C */
    u8 pad810[0x10];
    u32 panelGroup;      /* 0x820 */
    u32 displayResource; /* 0x824 */
    u32 effectResource;  /* 0x828 */
} MenuVisualWork;

extern s32 mnuFindMatchingPartyEntryIndex(s32);
extern s32 mnuSeekListNode(s32, MenuProgressOwner *);
extern void mnuSetWindowResource(s32, s32, s32, s32);
extern void mnuAttachPartyIconBundle(s32, s32, u32);
extern s32 mnuCreatePanelGroup(s32);
extern u32 *mnuAllocateSimpleSprite(u32, u32, u32, u32, u32);
extern u32 *mnuCreateProfilePanel(s32);
extern void mnuCacheProfilePanelGridPositions(s32, u32, u32, u32, u32);
extern void func_00276720(s32, s32, s32, s32);

typedef struct SceneFrameTable SceneFrameTable;

typedef struct MenuTerminalWork {
    s32 allocation;          /* 0x00 */
    s32 groupResource;       /* 0x04 */
    u8 pad08[0x54];
    s32 messageResources[2]; /* 0x5C: second handle opens the message window */
    SceneFrameTable *batch;  /* 0x64 */
    u32 secondResource;     /* 0x68 */
    u8 pad6C[4];
    MenuProgressOwner *listResource; /* 0x70 */
    MenuProgressOwner *list; /* 0x74 */
    MenuProgressOwner *owner;/* 0x78 */
    s32 mode;                /* 0x7C */
    s32 initState;           /* 0x80 */
    u8 pad84[0x18];
    s32 panelFade;          /* 0x9C: 0..0x100 color blend weight */
    s32 effect[7];           /* 0xA0: effect batches; [4] and [5] are the pair selected via cursor */
    s32 cursor[2];           /* 0xBC: current and previous node, -1 until selected */
    u8 padC4[0x14];
    s32 selectedSlot;        /* 0xD8 */
    s32 reduced;             /* 0xDC */
    u8 padE0[4];
    s32 unkE4;               /* 0xE4 */
    u8 padE8[0x78];
    u32 bgmHandle;           /* 0x160: encoded bank/track handle */
} MenuTerminalWork; /* 0x164 allocation (mnuTerminalCreateScene) */

extern s32 mnuCreateDualPercentPanel(s32, s32);

extern s32 sdfAllocSizeClassBlock(s32);

extern s32 mnuPercentOrHundred(u16, u16);

extern void mnuDrawPanelSequenceByRow(s32, s32, s32, s32, s32, s32);

extern s32 mnuCreateListState();

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

extern s32 mnuWalkNodeList(s32, s32);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 uiBlendColors(u32, u32, s32);

extern s32 func_001978E8(s32, s32, s32, s32, s32, s32);

extern char mnuNumberSpriteFormat[];

extern void func_001958A0(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);

typedef struct EffectPair {
    s32 firstValue;
    s32 secondValue;
} EffectPair;

extern EffectPair D_003BC400[];
extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern void func_0024BF48(s32, s32);
extern char D_003BC3F0[];
extern u32 func_001979C8(s32, s32, s32, s32, char *, s32);

typedef struct EffectInner {
    u8 pad00[0x20];
    EffectPair *pair; /* 0x20 */
} EffectInner;

typedef struct EffectObject {
    u8 pad00[8];
    EffectInner *inner; /* 0x08 */
} EffectObject;

extern EffectObject *effCreateStatusBatch(s32);

extern s32 effDestroyPackedBatch(s32);

/* Release both visual resources in order; the work object itself is retained. */
void mnuReleaseVisualResources(MenuVisualWork *work) {
    effResolveAndReleaseResource(work->firstResource);
    effResolveAndReleaseResource(work->secondResource);
}

/* Release/reset the two resources' texture slots without freeing the work object. */
void mnuReleaseBothVisualResourceTextures(MenuVisualWork *work) {
    effReleaseTextureHandlesAndResetSlots(work->firstResource);
    effReleaseTextureHandlesAndResetSlots(work->secondResource);
}

extern s32 func_00197760(s32, s32, s32, s32, s32, s32);
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
    s32 handle;

    if (alternate == 0) {
        text = D_00347C68[slot].encodedText;
    } else {
        text = D_003482A8[slot].encodedText;
    }
    handle = func_00197760(gridX, gridY, depth, value, (s32)text, 0);
    func_001958A0(handle, 1, MNU_TEXT_DRAW_PRIORITY);
    frFontQueueGlyphInSelectedSlot(handle);
}

/* Party vitals and status word, not a screen rectangle. Full stride is 0x1A4. */
typedef struct BoxRecord {
    u16 unitFlags; /* 0x00: bit 0 set when the party slot is active */
    u8 pad02[4];
    u16 hp;        /* 0x06 */
    u16 maxHp;     /* 0x08 */
    u16 mp;        /* 0x0A */
    u16 maxMp;     /* 0x0C */
    u16 statusFlags; /* 0x0E */
} BoxRecord;

/* Price recovery from missing HP/MP plus the five charged status bits.
 * Preserve DDS1's arithmetic and separate truncations; deficits are not clamped. */
s32 mnuTerminalScoreBox(BoxRecord *unit) {
    f32 missingMp = unit->maxMp - unit->mp;
    f32 missingHp = unit->maxHp - unit->hp;
    s32 statusCost = 0;

    if (unit->statusFlags & 0x400) {
        statusCost = 100;
    }
    if (unit->statusFlags & 0x100) {
        statusCost += 50;
    }
    if (unit->statusFlags & 0x80) {
        statusCost += 100;
    }
    if (unit->statusFlags & 0x40) {
        statusCost += 100;
    }
    if (unit->statusFlags & 0x10) {
        statusCost += 100;
    }
    return (s32)missingHp + (s32)(missingMp * (missingMp / 200.0f + 3.0f)) + statusCost;
}

/* Mark nodes unaffordable when their required amount exceeds current currency. */
void mnuRefreshThresholdNodeFlags(MenuProgressOwner *owner) {
    MenuProgressNode *node = owner->firstProgressNode;
    if (node != 0) {
        s32 base = datGameState;
        do {
            u32 currency = *(u32 *)(base + 0x3c);
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
    s32 sprite;

    func_003014F0(text, mnuNumberSpriteFormat, number);
    sprite = func_001978E8(x, y, layer, uiBlendColors(color, color & ~MNU_COLOR_LOW_BYTE_MASK, blendWeight), (s32)text, 0);
    func_001958A0(sprite, 1, priority);
    frFontQueueGlyphInSelectedSlot(sprite);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248810);





/* Allocate adjacent HP/MP percentage panels, preserving the native 0x54 stride.
 * The source is a party-vitals record; the context supplies the panel style. */
s32 mnuCreateDualPercentPanel(s32 unitAddress, s32 workAddress) {
    s32 panel = sdfAllocSizeClassBlock(MNU_PERCENT_PAIR_BYTES);
    mnuDrawPanelSequenceByRow(panel, 0, 0, 0x1e,
        mnuPercentOrHundred(*(u16 *)(unitAddress + 6), *(u16 *)(unitAddress + 8)),
        *(s32 *)(workAddress + 0xe0));
    mnuDrawPanelSequenceByRow(panel + MNU_PERCENT_PANEL_BYTES, 1, 0, 0x1e,
        mnuPercentOrHundred(*(u16 *)(unitAddress + 0xa), *(u16 *)(unitAddress + 0xc)),
        *(s32 *)(workAddress + 0xe0));
    return panel;
}

/* Release both texture sets and the backing allocation for a nonzero panel pair.
 * Retain the legacy zero-argument first texture-release call. */
void mnuReleaseDualPercentPanel(s32 panel) {
    if (panel != 0) {
        mnuReleaseSpriteTextures();
        mnuReleaseSpriteTextures((s32)panel + MNU_PERCENT_PANEL_BYTES);
        sdfReleaseChipBlock(panel);
        return;
    }
}

/* Rebuild each node's panel from its corresponding party entry. */
void mnuUpdateGroupResources(u8 *scene) {
    MenuProgressNode *node = *(MenuProgressNode **)(*(u8 **)(scene + 0x74) + 0x10);

    while (node != NULL) {
        node->panel = mnuCreateDualPercentPanel(datGameState + node->entryIndex * MNU_PARTY_RECORD_BYTES + 0xA60, (s32)scene);
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

typedef struct MenuThresholdEntry {
    s32 entryId;        /* 0x00 */
    s32 requiredAmount; /* 0x04 */
} MenuThresholdEntry;

extern s32 mnuTerminalScoreBox(BoxRecord *box);

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
        s32 unitAddress = datGameState + partyIndex * MNU_PARTY_RECORD_BYTES + 0xA60;

        if ((u16)(*(u16 *)unitAddress & 1)) {
            s32 recoveryCost = mnuTerminalScoreBox(unitAddress);

            if (recoveryCost != 0) {
                MenuProgressNode *node =
                    (MenuProgressNode *)mnuListAppendNode(host->list, (s32)D_003BC3F8);
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
    mnuDestroyListState((u32)work->list);
}

/* Release the selected recovery panel, then pass its owning list to the follow-up. */
void mnuReleaseSelectedProgressPanel(MenuTerminalWork *work) {
    mnuReleaseDualPercentPanel(work->list->selectedNode->panel);
    func_0027B888((u32)work->list);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248E68);

typedef struct MenuSlotKind {
    s16 kind;
    s16 unk2;
} MenuSlotKind;

extern MenuSlotKind D_0032EF18[];

/* Same slot kind, or both kinds in the 30/31 pair. */
s32 mnuSlotKindMatchesGroupOrSpecial(s32 index, s32 requestedKind) {
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

INCLUDE_ASM(const s32, "game/code_00248580", func_00249058);

/* Return whether model flag 0x902 is clear; its storyline meaning is not asserted. */
u8 func_00249198(void) {
    s64 flagSet;

    flagSet = mdlFlagTest(0x902);
    return flagSet == 0;
}


INCLUDE_ASM(const s32, "game/code_00248580", func_002491B8);





/* Omit the input-array position `excluded`, not all entries with that same value. */
s32 mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    s32 list = mnuCreateListState(0, count, 0x15, callback);
    s32 entryIndex;
    *(s32 *)(list + 0x30) = callback;
    *(s32 *)(list + 0x2c) = (s32)func_002491B8;
    *(s32 *)(list + 0x3c) = 0;
    for (entryIndex = 0; entryIndex < count; entryIndex++) {
        if (entryIndex != excluded) {
            s32 node = mnuListAppendNode(list, (s32)D_003BC3F8);
            *(s32 *)(node + 0x60) = items[entryIndex];
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
            s32 selected = mnuWalkNodeList(2 - func_00249198(),
                                              (s32)((MenuTerminalWork *)object)->listResource);
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
        s32 node = mnuWalkNodeList(selectedIndex, (s32)((MenuTerminalWork *)object)->listResource);
        ((MenuProgressNode *)node)->flags |= 1;
    }
}

extern void mnuResolveStaffImageHandles(u8 *);

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
    mnuResolveStaffImageHandles((u8 *)host + 0xE0);
    mnuUpdateGroupResources((u8 *)host);
    func_00249058(host);
    mnuHighlightProgressNodeFromOwnerSelection(host);
    mnuHighlightProgressNodeByMode(host);
}

extern void mnuDestroyListState(u32);

extern void mnuReleaseStaffImageHandles(u8 *);

/* Release command/progress lists, child percentage panels and staff image handles.
 * Preserve their existing order and the single-iteration list loop. */
void mnuReleaseWorkResources(u8 *work) {
    u32 i;

    for (i = 0; i < 1; i++) {
        mnuDestroyListState(*(u32 *)(work + 0x70 + i * 4));
    }
    mnuDestroyThresholdNodePanels((s32)work);
    mnuReleaseStaffImageHandles(work + 0xE0);
    mnuReleaseProgressWorkList((s32)work);
    mnuDestroyListState((u32)((MenuTerminalWork *)work)->owner);
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

extern s32 sdfAllocGeneralBlock(s32);

extern s32 sdfResourceRetainAddress(s32);

extern void *memset(void *, s32, u32);

extern s32 mnuAllocateValueRecord(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void mnuAppendCampSpriteRequests(s32, s32);

/* Allocate/zero the visual host, retain its allocation, and begin resource setup. */
u8 *mnuCreateWorkBlock(void) {
    s32 handle = sdfAllocGeneralBlock(MNU_MENU_HOST_BYTES);
    u8 *work = (u8 *)sdfResourceRetainAddress(handle);

    memset(work, 0, MNU_MENU_HOST_BYTES);
    *(s32 *)work = handle;
    ((MenuTerminalWork *)work)->groupResource = mnuAllocateValueRecord(1);
    mnuInitPartyPanelSlots((s32)(work + 0x84));
    mnuAppendCampSpriteRequests(((MenuTerminalWork *)work)->groupResource, (s32)(work + 8));
    ((MenuTerminalWork *)work)->initState = 1;
    return work;
}

/* Release staff window/texture/resource work before the value record and allocation. */
void mnuReleaseStaffMenuContextAndResources(u32 *workWords) {
    mnuShutdownContext(workWords + 100);
    mnuReleaseStaffMenuTextureHandles(workWords + 2);
    mnuReleaseStaffResourceGroups(workWords + 2);
    func_002BC618(workWords[1]);
    sdfReleaseResourceAllocation(*workWords);
}

extern s32 mnuStaffSlotsAllFilled(s32, s32 *);

extern void mnuReleaseStaffMenuResources(s32 *);

extern void mnuInitializeStaffPageWindows(s32, s32 *, s32, s32);

/* Return one while initialization is pending (including state zero), zero when ready.
 * On resource readiness, release loading resources, initialize windows, and store state two. */
s32 mnuTickInitState(u8 *work) {
    s32 state = ((MenuTerminalWork *)work)->initState;
    s32 *group;

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    group = (s32 *)(work + 8);
    if (mnuStaffSlotsAllFilled(((MenuTerminalWork *)work)->groupResource, group) == 0) {
        return 1;
    }
    mnuReleaseStaffMenuResources(group);
    mnuInitializeStaffPageWindows((s32)(work + 0x190), group, 0, (s32)(work + 0x84));
    ((MenuTerminalWork *)work)->initState = 2;
    return 0;
}

/* Bind the party selection's textures/grid, then create its panel and profile visuals. */
void func_00249850(s32 source, MenuVisualWork *work) {
    s32 window = (s32)work->window;
    s32 index;

    mnuSeekListNode(mnuFindMatchingPartyEntryIndex(source), work->windowList);
    index = work->windowList->selectedNode->index;
    mnuSetWindowResource(index, window, work->texture, work->grid);
    mnuAttachPartyIconBundle(index, window, work->texture);
    work->panelGroup = mnuCreatePanelGroup(work->texture);
    work->displayResource = (u32)mnuAllocateSimpleSprite(work->x, work->y, work->z, work->color, work->texture);
    work->effectResource = (u32)mnuCreateProfilePanel(source);
    mnuCacheProfilePanelGridPositions(work->effectResource, work->grid, 5, 14, 15);
    func_00276720(window, 1, 1, 1);
}

/* Release the window's entries/icons before its panel, sprite and profile allocations. */
void mnuReleaseMenuVisualWorkResources(MenuVisualWork *work) {
    mnuClearEntries((s32)work + 400);
    mnuReleasePartyIconBundles((s32)work + 400);
    mnuDestroyPanelGroup(work->panelGroup);
    mnuFreeSimpleSpriteWork(work->displayResource);
    mnuFreeProfilePanelWork(work->effectResource);
}

/* Forward coordinates/mode to the retained profile panel; do not advance other visuals. */
void effUpdateAttached(s32 x, s32 y, s32 mode, MenuVisualWork *work) {
    mnuDrawAndAdvanceProfilePanel(x, y, mode, work->effectResource);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249998);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249A60);

/* Only the exact signed-byte value one enables the world/menu flags; all others disable. */
void mnuSetWorldObjectAndMenuEnabled(s8 enabled) {
    s64 worldObject;

    if (enabled == '\x01') {
        worldObject = dds3GetWorldObject();
        if (worldObject != 0) {
            dds3SetWorldObjectDataValue(worldObject, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        worldObject = dds3GetWorldObject();
        if (worldObject != 0) {
            dds3SetWorldObjectDataValue(worldObject, 0);
        }
        D_003BC3E1 = 0;
    }
}

/* Allocate seven effect batches and seed their two opaque parameter words.
 * EffectPair also carries drawing positions elsewhere, so its fields stay role-neutral. */
void mnuTerminalCreateEffects(MenuTerminalWork *state) {
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
    s32 *batch = ((MenuTerminalWork *)object)->effect;
    u32 i;

    for (i = 0; i < MNU_EFFECT_BATCH_COUNT; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

extern s32 fldGetCurrentBgmHandle(void);
extern void sndEnsureMidiBankResident(u32);

/* Select the native default/current BGM handle and make only its bank bits resident.
 * DDS1's default includes track one; DDS2's default has a zero low halfword. */
void mnuSelectTerminalResourceBank(MenuTerminalWork *work) {
    if (work->mode == 0) {
        work->bgmHandle = MNU_TERMINAL_DEFAULT_BGM;
    } else {
        work->bgmHandle = fldGetCurrentBgmHandle();
    }
    sndEnsureMidiBankResident(work->bgmHandle & MNU_BGM_BANK_MASK);
}

extern void mnuClearPanelTransitionState(void *);

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

    handle = sdfAllocGeneralBlock(MNU_TERMINAL_SCENE_BYTES);
    obj = (u8 *)sdfResourceRetainAddress(handle);
    memset(obj, 0, MNU_TERMINAL_SCENE_BYTES);
    ((MenuTerminalWork *)obj)->allocation = handle;
    mnuClearPanelTransitionState(obj + 8);
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
    mnuSelectTerminalResourceBank((MenuTerminalWork *)obj);
    return obj;
}

extern s32 kwlnTaskGetUserValue();
extern void mnuReleaseResourceHandles(u32 *work);
extern void mnuDrainPanelTransitions(u8 *state, s32 arg);
extern void dspCloseChannel(void);
extern void evtReleaseResourcePairHandle(u32 *record);
extern s32 mnuCheckResourceTask(void);
extern void mnuStopResourceTask(void);
extern void func_00126038(s32 a, s32 b);
extern void fldProcessDeferredSceneCommand(void);
extern void sdfReleaseResourceAllocation(s32 handle);
extern s8 mnuTerminalTaskState;

/* Release the terminal task's resources/effects, then hand its mode/slot to the field.
 * Native final work-field reads remain after allocation release and outside the NULL guard. */
void mnuReleaseTerminalWorkAndResumeField(s32 arg) {
    MenuTerminalWork *work = (MenuTerminalWork *)kwlnTaskGetUserValue();

    if (work != NULL) {
        mnuReleaseWorkResources((u8 *)work);
        mnuReleaseResourceHandles((u32 *)work);
        mnuDestroyAllMenuSlotEffectBatches((s32)work);
        mnuDrainPanelTransitions((u8 *)work + 8, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle((u32 *)work->messageResources);
        sdfReleaseResourceAllocation(work->allocation);
        mnuTerminalTaskState = 2;
    }
    if (mnuCheckResourceTask() != 0) {
        mnuStopResourceTask();
    }
    func_00126038(work->mode, work->initState);
    fldProcessDeferredSceneCommand();
}


extern s32 kwlnFadeIsActive(void);


extern s64 evtGetMessageWindowControlState(void);

extern s64 func_00285670(s32, s32 *, u64, u64);


extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_003AF658[];

extern const char D_003AF668[];

extern const char D_003AF678[];

extern s32 D_003BC3E4;

extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);

extern s32 kwlnTaskCreate(const char *, s32, s32, s32, void (*)(s32), void (*)(s32), void *);
extern s64 mnuPrepareTerminalPopupAndDispatch(s32);
extern s64 func_0024A138(s32);
extern s64 func_0024A170(s32);
extern void mnuReleaseTerminalWorkAndResumeField(s32);

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

extern void mnuSetPopupEntry(s32 *, char *);

/* Seed the task's popup slot before running panel mode zero with the supplied request. */
s64 mnuPrepareTerminalPopupAndDispatch(s32 value) {
    s32 context = kwlnTaskGetUserValue();
    s32 *state = (s32 *)(context + 0x54);

    mnuSetPopupEntry(state, D_0036ADF4);
    return menuRunPanel(context, 0, value);
}

/* Dispatch current task work through panel mode one; distinct callback role unknown. */
s64 func_0024A138(s32 value) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 1, value);
}

/* Dispatch current task work through panel mode two; distinct callback role unknown. */
s64 func_0024A170(s32 value) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, value);
}

/* Require the fade to be inactive before testing message-window control for idle. */
s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A1D8);

typedef struct {
    u8 pad00[6];
    u16 hp;          /* 0x06 */
    u16 maxHp;       /* 0x08 */
    u16 mp;          /* 0x0A */
    u16 maxMp;       /* 0x0C */
    u16 statusFlags; /* 0x0E */
} SceneOptionRecord;

/* Restore current HP/MP to their stored maxima and clear exactly the charged status bits.
 * No boosted-max calculation or range validation is performed here. */
void fldSaveSceneOptionsAndClearFlags(SceneOptionRecord *option) {
    u16 statusFlags = option->statusFlags;
    u16 maxHp = option->maxHp;
    u16 maxMp = option->maxMp;
    u16 retainedStatus = statusFlags & MNU_RECOVERY_STATUS_KEEP_MASK;

    option->hp = maxHp;
    option->mp = maxMp;
    option->statusFlags = retainedStatus;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A2D8);

extern s32 D_003AF688[3][2];
extern void func_002BF4E0(s32, s32, s32, s32, s32, u32, s32, s32);

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
                1, work->secondResource, i, 0x53);
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
        effConfigureWithDefaultSetting((s32)state->batch, *slot, state->effect[4], 0, 5, 2);
        break;
    case 2:
        effConfigureWithDefaultSetting((s32)state->batch, *slot, state->effect[5], 0, 0, 2);
        break;
    case 3:
        effConfigureWithDefaultSetting((s32)state->batch, *slot, state->effect[4], 0, 0, 2);
        if (slot[1] >= 0) {
            effConfigureWithDefaultSetting((s32)state->batch, slot[1], state->effect[5], 0, 0, 2);
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

void mnuSelectTerminalCursorSlot(u32 mode, s32 index, MenuTerminalWork *state) {
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
                                        (s32)state->batch, *slot, MNU_TEXT_DRAW_PRIORITY);
        }
    }
    if (state->mode == 2) {
        func_0024BF48(0, context);
    }
}

typedef struct {
    u8 pad00[0x14];
    u8 unk14;
    u8 pad15[0x6F];
    u8 unk84;
    u8 pad85[0x1B];
} SceneFrameRecord;

struct SceneFrameTable {
    u8 pad00[0x18];
    SceneFrameRecord *records;
};

typedef struct {
    u8 pad00[0x64];
    SceneFrameTable *frameTable; /* 0x64 */
    u8 pad68[0xC];
    struct EvtBSelectionList *list; /* 0x74 */
    u8 pad78[0x64];
    s32 mode; /* 0xDC */
} SceneFrameOwner;

extern s32 fldGetModeFrameRecordIndex(SceneFrameOwner *);

/* Scene modes 1 and 2 select different entries from the same frame table. */
s32 fldGetModeFrameRecordIndex(SceneFrameOwner *scene) {
    switch (scene->mode) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_0024A6E8(SceneFrameOwner *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex(scene);
    return scene->frameTable->records[index].unk14;
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


INCLUDE_ASM(const s32, "game/code_00248580", func_0024A728);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A930);

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

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AB70);


extern void itfSetGridEntryQuantizedAndRefresh(s32, s32, s32, s32, s32, s32);
extern s32 D_003AF6E0[2][2];
extern void mnuCallInitWide(s32, s32, s32, s32, s32);

void func_0024ACD8(s32 close, s32 context) {
    MenuTerminalWork *work = (MenuTerminalWork *)context;
    s32 positions[2][2];
    u32 progress;
    SceneFrameTable *frames;

    memcpy(positions, D_003AF6E0, sizeof(positions));
    itfDrawGridWithResolvedSlot(positions[1][0], positions[1][1], 0, 0x80,
        (s32)work->batch, func_0024AB28((MenuSelectorContext *)work), 0x53);
    mnuCallInitWide(0x330, 0x340, 0, (s32)work->listResource, 0x53);
    itfDrawGridWithResolvedSlot(positions[0][0], positions[0][1], 0, 0x80,
        (s32)work->batch, 8, 0x53);
    frames = work->batch;
    progress = ((u32)frames->records[8].unk14 << 8) / frames->records[8].unk84;
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

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AF58);

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
    effConfigureWithDefaultSetting(host->grid, 0x1A, host->settings[slot], 0, value, flags);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B168);

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

extern void func_0024A2D8(s32);

extern void func_0024DD78(void);




extern void func_0024A340(s32, s32);


extern void func_0024A930(s32);

extern void mnuDrawTerminalSelectedSlots(s32);

/* Offsets shared by the event-B menu/dispatch handlers in this unit. */
typedef struct EvtBContext {
    u8 pad00[0x54];
    s32 dispatchState; /* 0x54 */
    u32 dispatchTable; /* 0x58 */
    u8 pad5C[0x14];
    u32 visualList; /* 0x70 */
    s32 thresholdList; /* 0x74 */
    s32 selectionList; /* 0x78 */
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

typedef struct EvtBSelectionNode {
    s32 kind;       /* 0x00 */
    u8 pad04[0x44];
    u32 flags;      /* 0x48 */
    u8 pad4C[0x14];
    s32 entryIndex; /* 0x60 */
} EvtBSelectionNode;

typedef struct EvtBSelectionList {
    u8 pad00[0x1C];
    EvtBSelectionNode *selected; /* 0x1C */
    s32 mode; /* 0x20 */
    u8 pad24[0x18];
    s32 scale; /* 0x3C: fade scale, 0x100 when fully shown */
} EvtBSelectionList;

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

extern s32 mnuMapPadMaskToFlags(s32 mask);
extern s32 func_0024A1D8(s32 action, s32 context);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void mnuSetPopupEntryFlagged(s32 *state, void *entry);
extern void mnuClearListFlagsOneAndTwo(u32 list);
extern void mnuRetreatListCursorDefault(u32 list);
extern void mnuAdvanceListCursorDefault(u32 list);
extern void mnuPlayInputSound(s32 mode, u32 buttons, u32 list);
extern s32 D_0036AC80[];
extern u8 D_0036ACF8[];
extern u8 D_0036AD30[];
extern u8 D_0036AD68[];
extern u8 D_0036ADA0[];

/* Event-B panel input: confirm opens the popup for the selected entry's action, cancel opens the back popup, left/right step the list. */
s64 evtBHandleSelectionPanelInput(u64 input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *state = &context->dispatchState;
    s32 kind = ((EvtBSelectionList *)context->visualList)->selected->kind;
    s32 frames;
    s32 action;
    EvtBSelectionNode *node;
    s64 result;

    result = func_00285670((s32)context + 8, state, 0, input);
    if (result != 0) {
        return result;
    }
    frames = fldClassifyRemainingFrames((MenuTerminalWork *)context);
    if (frames != 2) {
        return 0;
    }
    if (((EvtBSelectionList *)context->visualList)->scale < 0x100) {
        return 0;
    }
    if (*state == 0) {
        if (buttons & 1) {
            node = ((EvtBSelectionList *)context->visualList)->selected;
            action = D_0036AC80[func_00249198() * 5 + context->state7C * 10 + kind];
            if (!(node->flags & 1) || action == 3 || action == frames) {
                if (func_0024A1D8(action, (s32)context) == 0) {
                    switch (action) {
                    case 2:
                        if (((EvtBSelectionList *)context->selectionList)->mode == 1) {
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
            mnuClearListFlagsOneAndTwo(context->visualList);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault(context->visualList);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault(context->visualList);
        }
        mnuPlayInputSound(0, buttons, context->visualList);
    }
    return 0;
}


s64 evtDispatchSelectionAfterFieldFrameGate(u64 request) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    func_0024A340(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, request);
}

s64 evtBSetupDispatchSync(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

s32 evtBClearAndReset(void) {
    s32 context = kwlnTaskGetUserValue();

    evtRememberDispatchCallback(0, context);
    ((EvtBSelectionList *)((EvtBContext *)context)->visualList)->scale = 0;
    mnuSetWorldObjectAndMenuEnabled(0);
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern void func_0024AB70(s32, s32);

u32 evtBeginSelectionExitFade(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    mnuSelectTerminalCursorSlot(0, -2, context);
    mnuSetWorldObjectAndMenuEnabled(1);
    mnuReleaseVisualResources(context);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern s32 fileMenuTaskExists(void);
extern void fileSetPreviewLocation();
extern void fileEnterMcPackScene(s32);

s64 func_0024B868(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    s32 *dispatch = (s32 *)(context + 0x54);
    s64 result = func_00285670(context + 8, dispatch, 0, request);

    if (result != 0) {
        return result;
    }
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    if (*(s32 *)(context + 0x94) == 0 &&
        sdfCheckPendingWorkWithInterrupts() == 0) {
        mnuReleaseBothVisualResourceTextures((MenuVisualWork *)context);
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

s64 evtBDispatchStart(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 1, request);
}

s64 evtBSetupDispatchSyncB(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

extern void mnuSelectFirstListNode(s32);
extern void func_0024A728(s32, s32);
extern void func_0024AF58(void);
extern void func_0024AE18(s32, s32);


u32 evtInitializeSelectionListWhenReady(void) {
    s32 context = kwlnTaskGetUserValue();

    if (((EvtBContext *)context)->transitionPending == 0) {
        mnuSelectFirstListNode(((EvtBContext *)context)->selectionList);
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024AF58, context);
        func_0024AE18(3, context);
        func_0024AB70(4, context);
        mnuSelectTerminalCursorSlot(3, 1, context);
    }
    ((EvtBContext *)context)->transitionPending = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}


u32 evtFinishPendingSelectionTransition(void) {
    s32 context = kwlnTaskGetUserValue();

    if (((EvtBContext *)context)->transitionPending != 0) {
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024ACD8, context);
        func_0024AE18(4, context);
        func_0024AB70(3, context);
        mnuSelectTerminalCursorSlot(3, 0, context);
        evtFinishMessageWindowAndNotify();
    }
    ((EvtBContext *)context)->transitionPending = 0;
    return 1;
}

s64 func_0024BB00(u64 input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();
    s32 *state = &context->dispatchState;
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s64 result;

    result = func_00285670((s32)context + 8, state, 0, input);
    if (result != 0) {
        return result;
    }
    if (((EvtBSelectionList *)context->selectionList)->scale < 0x100) {
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
            mnuClearListFlagsOneAndTwo(context->selectionList);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault(context->selectionList);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault(context->selectionList);
        }
        mnuPlayInputSound(0, buttons, context->selectionList);
    }
    return 0;
}

s64 func_0024BC18(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncC(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

extern void func_0024B168(void);

u32 evtEnterThresholdSelectionList(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuRefreshThresholdNodeFlags(((EvtBContext *)context)->thresholdList);
    mnuSelectFirstListNode(((EvtBContext *)context)->thresholdList);
    mnuSelectTerminalCursorSlot(3, 2, context);
    mnuApplyGridPanelHostSetting(3, context);
    func_0024AB70(4, context);
    evtRememberDispatchCallback((s32)func_0024B168, context);
    return 1;
}

extern void mnuHighlightProgressNodeByMode(s32);

u32 evtBEnterStateA(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuSelectTerminalCursorSlot(3, 0, context);
    mnuApplyGridPanelHostSetting(4, context);
    func_0024AB70(3, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    mnuHighlightProgressNodeByMode(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024BDB8);

void func_0024BF48(s32 fading, s32 context) {
    SceneFrameOwner *scene = (SceneFrameOwner *)context;
    char text[16];
    s32 index;
    s32 color;
    u32 sprite;

    index = fldGetModeFrameRecordIndex(scene);
    func_003014F0(text, D_003BC3F0, *(s32 *)(datGameState + 0x3C));
    if (fading == 0) {
        color = scene->frameTable->records[index].unk14 | 0xA09DC300;
    } else {
        color = uiBlendColors(0xA09DC380, 0xA09DC300, scene->list->scale);
    }
    sprite = func_001979C8(0x1740, 0x210, 0, color, text, 0);
    func_001958A0(sprite, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(sprite);
}

extern void func_0024BF48(s32, s32);

s64 mnuInitializeSelectionDispatchWhenModeUnset(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    if (*(s32 *)(state + 0x7C) == 0) {
        func_0024BF48(1, state);
    }
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncD(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

u32 evtSelectFinalVisualNode(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSelectLastListNode(((EvtBContext *)context)->visualList);
    return 1;
}

/* Terminal panel poll: once the message window is idle and the field frames are drained, open the follow-up popup. */
extern u8 D_0036AE2C[];
s64 evtOpenTerminalFollowupPopupWhenIdle(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    s64 result = func_00285670(state + 8, panel, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (fldClassifyRemainingFrames(state) == 0) {
                mnuSetPopupEntry((s32)panel, (s32)D_0036AE2C);
            }
        }
    }
    return 0;
}



INCLUDE_ASM(const s32, "game/code_00248580", func_0024C1B8);

s64 evtBSetupDispatchSyncE(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}


u32 evtBReleaseImagesAndQueueMenuTransition(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseStaffImageHandles(context + 0xE0);
    func_0024A728(2, context);
    mnuSelectTerminalCursorSlot(2, -1, context);
    func_0024AB70(2, context);
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    *(s32 *)(context + 0x98) = 0;
    evtFinishMessageWindowAndNotify();
    dspCloseChannel();
    func_002E96D8(((EvtBContext *)context)->bgmHandle);
    return 1;
}

s32 mnuOpenTerminalSelectionMessageWindow(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuResolveStaffImageHandles((u8 *)context + 0xE0);
    func_0024A728(1, context);
    mnuSelectTerminalCursorSlot(1, 0, (MenuTerminalWork *)context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    ((EvtBContext *)context)->exitPending = 0;
    evtCreateMessageWindowIfMissing(((MenuTerminalWork *)context)->messageResources[1]);
    if (((EvtBContext *)context)->state7C != 0) {
        sndStartTrackExtended(((EvtBContext *)context)->bgmHandle);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024C3F8);

/* While the panel fade is nonzero, choose its transition direction from the selection chain, then run the panel. */
s64 evtBPollSelectionChainPanel(s32 item) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    if (fldClassifyRemainingFrames((MenuTerminalWork *)state) != 0) {
        if (((EvtBContext *)state)->selectionStep == 0) {
            func_0024A340(1, state);
        } else if (func_0024A6E8((SceneFrameOwner *)state) == 0) {
            func_0024A340(1, state);
        } else {
            func_0024A340(0, state);
        }
        mnuDispatchTransitionHostCallbacks((TransitionHost *)state);
        func_0024A930(state);
        mnuDrawTerminalSelectedSlots(state);
    }
    return menuRunPanel(state, 1, item);
}

s64 evtBDispatchSync(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
}

extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern void dspSetActive(s32);
extern void dspStartEntry(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void evtCaptureMessageWindowSoundMode(s32);

/* Bind the selected text row to message slot zero, then start the entry prompt. */
u32 evtPrepareSelectedMenuEntry(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 owner = ((EvtBContext *)state)->selectionList;
    s32 *selectionIndex = &((EvtBSelectionList *)owner)->selected->entryIndex;

    if (((EvtBSelectionList *)owner)->mode == 1) {
        mnuSelectFirstListNode(owner);
    }
    evtCopyEntryStringToActiveWindow(0, D_00347C68[*selectionIndex].encodedText);
    dspSetActive(1);
    dspStartEntry(0);
    evtSetMessageWindowOptionWhenOpen(1);
    evtCaptureMessageWindowSoundMode(6);
    return 1;
}

u32 func_0024C6F8(void) {
    return 1;
}

extern s32 evtGetCapturedMessageWindowSoundMode(void);
extern u8 D_0036ADD8[];

s64 func_0024C700(u64 input) {
    EvtBContext *context;
    s32 *state;
    s64 result;

    context = (EvtBContext *)kwlnTaskGetUserValue();
    state = &context->dispatchState;
    result = func_00285670((s32)context + 8, state, 0, input);
    if (result == 0) {
        if (*state == 0) {
            result = evtGetMessageWindowControlState();
            if (result == 0) {
                if (evtGetCapturedMessageWindowSoundMode() == 0) {
                    mnuResetProgressModeFromOwner((u8 *)context);
                    mnuSetPopupEntryFlagged(state, D_0036ADD8);
                } else if (((EvtBSelectionList *)context->selectionList)->mode >= 2) {
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

s64 func_0024C7E8(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncF(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}


u32 evtBCheckPanelMode(void) {
    s32 context = kwlnTaskGetUserValue();

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

s64 evtBContinueDispatchOrRestoreTable(u64 input) {
    s32 context;
    s64 dispatchResult;
    s32 *dispatchState;

    context = kwlnTaskGetUserValue();
    dispatchState = &((EvtBContext *)context)->dispatchState;
    dispatchResult = func_00285670(context + 8, dispatchState, 0, input);
    if (dispatchResult == 0) {
        if ((*dispatchState == 0) && (dispatchResult = evtGetMessageWindowControlState(), dispatchResult == 0)) {
            mnuSetPopupEntry(dispatchState, ((EvtBContext *)context)->dispatchTable);
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

s64 mnuPrepareDispatchStateAndBindHandler(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncG(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}


u32 evtExitSelectionMenuAndSendSoundCommand(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A728(2, context);
    if (((EvtBContext *)context)->transitionStage >= 2) {
        mnuSelectTerminalCursorSlot(2, -1, context);
        func_0024AE18(2, context);
    } else {
        mnuSelectTerminalCursorSlot(2, -1, context);
        func_0024AB70(2, context);
    }
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    func_002E96D8(((EvtBContext *)context)->bgmHandle);
    evtClearActiveFlag(0);
    return 1;
}


u32 evtBRebuildTerminalMenuAndResetDispatch(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseWorkResources(context);
    mnuTerminalBuildMenus(context);
    func_0024A728(1, context);
    mnuSelectTerminalCursorSlot(1, 0, context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    ((EvtBContext *)context)->dispatchMode = 0;
    ((EvtBContext *)context)->exitPending = 0;
    return 1;
}

extern void mnuReleaseEffectResource();

s64 func_0024CB80(u64 input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();
    s32 *state = &context->dispatchState;
    s64 result;

    result = func_00285670((s32)context + 8, state, 0, input);
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
                            mnuReleaseEffectResource((void *)context->effectHandle);
                            context->effectHandle = 0;
                        }
                        mnuFadeOrPlayCloseSfx(0, (u8 *)context);
                        mnuSelectTerminalResourceBank((MenuTerminalWork *)context);
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


s64 evtBDispatchSyncD2(s32 item) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    if (func_0024A6E8(state) == 0) {
        func_0024A340(1, state);
    } else {
        func_0024A340(0, state);
    }
    mnuDispatchTransitionHostCallbacks(state);
    if (((EvtBContext *)state)->dispatchMode != 3) {
        func_0024A930(state);
    }
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBDispatchSyncB(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
}

extern s32 mnuRequestEffectResource(char *, char *);
extern char D_003AF590[];
extern char D_003AF620[];

u32 evtBEndDispatchAndReloadEffectResource(void) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();

    mnuFadeOrPlayCloseSfx(0, (s32)context);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    if (context->state7C != 0) {
        context->effectHandle = mnuRequestEffectResource(D_003AF590, D_003AF620);
    }
    return 1;
}

u32 func_0024CE20(void) {
    return 1;
}

typedef struct {
    u8 pad00[0x6C];
    u32 resourceHandle;
} MenuResourceWork;

extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
extern s32 evtIsActiveFlagSet(s32 flagIndex);
extern u8 mnuHasEffectResourceHandle(MenuResourceWork *work);
extern const char D_003AF710[];
extern u8 D_0036AE10[];

s64 func_0024CE28(u64 input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();
    s32 *state = &context->dispatchState;
    s32 canOpen = 0;
    s64 result = func_00285670((s32)context + 8, state, 0, input);

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

s64 evtBLateDispatchStart(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A2D8(context);
    return menuRunPanel(context, 1, request);
}

s64 evtBDispatchSyncC(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
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

