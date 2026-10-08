#include "mnu.h"
#include "sdf.h"
#include "mnu_shop.h"
#include "mnu_list.h"
#include "dat_state.h"
#include "kwln.h"
#include "eff.h"
#include "itf.h"
struct MenuScrollPanel;
extern u32 kwlnTaskGetUserValue();
extern void effReleaseTextureHandlesAndResetSlots(EffectSlotSet *);
extern void mnuStoreScrollPanelSelectionAndGridPosition(struct MenuScrollPanel *, u32, u32, u32);
extern void mnuSetWindowResource(s32, MenuPageWindow *, s32, s32);
extern void mnuAttachPartyIconBundle(s32, MenuPageWindow *, u32);
extern MenuProfilePanel *mnuCreateProfilePanel(DatPartyRecord *selectionState);
extern void mnuCacheProfilePanelGridPositions(MenuProfilePanel *, u32, u32, u32, u32);
extern void mnuFreeProfilePanelWork(MenuProfilePanel *);
extern void mnuDrawAndAdvanceProfilePanel(s32, s32, s32, MenuProfilePanel *, s32);
extern s32 mnuGetSelectionFromFlags(DatPartyRecord *);
extern MenuPanelHandles *mnuCreatePanelSpriteHandles(u32, s32, s32);
extern s32 mnuClassifyQuarterHalfPercent(s32, s32);
extern s32 evtStageTestSelectEntry(s32, s32, s32);
extern void func_00276720(s32, s32, s32, s32);
extern void mnuInitPartyPanelSlots(PartyPanel *);

extern u32 uiBlendColors(u32, u32, u32);
extern s32 mnuLookupRangeEntry(u16);
extern u16 mnuGetAdjustedEntryValue(s32, DatPartyRecord *);
extern u16 mnuGetAdjustedPartyRangeValue(s32);
extern u8 mnuGetRangeEntryKind(u32);
extern s32 func_003014F0(char *, const char *, ...);
extern FrFontGlyph *func_001978E8(s32, s32, s32, u32, char *, FrFontGlyph *);
extern void frFontSetChainFlag(FrFontGlyph *, u8);
extern s32 func_001958A0(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern void func_002BF4E0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, u32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern s32 func_002CD240(u16, const char **);
extern void func_002845F8(s32, s32, s32, u32, u16, s32, MenuEffectPair *, u32);
extern void mnuDrawCenteredLabel(s32, s32, s32, s32, s32, s32);
extern FrFontGlyph *itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, FrFontGlyph *);
extern void func_00196088(s32, s32, FrFontGlyph *);
extern char D_003BC6F0[], D_003BC6F8[];
extern char D_003BC700[];

#define MNU_STAFF_PARTY_SLOT_COUNT 5
#define MNU_STAFF_PARTY_LAST_SLOT 4
#define MNU_STAFF_DISPLAY_OVERFLOW 4
#define MNU_STAFF_DISPLAY_LIMIT 3
#define MNU_STAFF_PARTY_ENTRY_BYTES 0x1A4
#define MNU_STAFF_PARTY_HALFWORD_STRIDE 210
#define MNU_STAFF_PARTY_BASE 0xA60
#define MNU_STAFF_BACKUP_BYTES 0x834
#define MNU_STAFF_PARTY_ACTIVE_BIT 1
#define MNU_STAFF_NODE_UNAVAILABLE 1
#define MNU_STAFF_NODE_SELECTED 2
#define MNU_STAFF_FADE_STEP 0x10
#define MNU_STAFF_FADE_CLOSE_THRESHOLD 0x50
#define MNU_STAFF_FADE_OPEN_THRESHOLD 0xB0
#define MNU_FULL_FADE 0x100
#define MNU_LIST_SELECTION_FLAG 8
#define MNU_STAFF_INPUT_CONFIRM 1
#define MNU_STAFF_INPUT_CANCEL 2
#define MNU_STAFF_POPUP_INPUT_MASK 3
#define MNU_STAFF_VIEW_INPUT_MASK 0xC2
#define MNU_STAFF_VIEW_TOGGLE_MASK 0xC0
#define MNU_STAFF_PAGE_INPUT_MASK 0x300
#define MNU_STAFF_INPUT_PREV_PAGE 0x100
#define MNU_STAFF_INPUT_NEXT_PAGE 0x200
#define MNU_STAFF_INPUT_PREVIOUS 0x10
#define MNU_STAFF_INPUT_NEXT 0x20
#define MNU_STAFF_INPUT_NAV_STATE_MASK 0x300000
#define MNU_STAFF_INPUT_REJECTED 0x8000
#define MNU_STAFF_SKILL_INPUT_MASK 0x33
#define MNU_STAFF_REORDER_INPUT_MASK 0x37
#define MNU_STAFF_REORDER_CANCEL_MASK 6






























typedef struct PartyEntryCopy {
    u16 flags;
    u16 pad02;
    u16 displayId; /* 0x04: used to select a party display asset */
    u16 pad06;
    u32 word[0x67];
} PartyEntryCopy; /* 0x1A4 bytes */

/* One allocated party-selection work area: original/current/backup entries,
 * saved panel payloads, and fade state all belong to this same allocation. */
typedef struct PartyMenuData {
    s32 allocation;
    u8 pad04[4];
    MenuWindowContainer *primaryWindow;   /* 0x08: owned generic window */
    PartyEntryCopy original[5];               /* 0x0C */
    PartyEntryCopy current[5];
    s32 activeCount; /* 0x1074 */
    PartyEntryCopy backup[5];
    s32 selection; /* 0x18AC */
    u8 panelSnapshots[5][0xA8]; /* 0x18B0: copied panel subrecords */
    s32 fadeA;                 /* 0x1BF8 */
    s32 fadeB;                 /* 0x1BFC */
    s32 selectionKey;          /* 0x1C00: saved menu-list key */
    s32 profilePanelPhase;     /* 0x1C04: profile animation phase */
} PartyMenuData; /* 0x1C08: native party-selection allocation */

/* Byte-offset copies keep their field displacement tied to the owner layout. */
#define PARTY_CURRENT_OFFSET ((s32)&((PartyMenuData *)0)->current)
#define PARTY_BACKUP_OFFSET ((s32)&((PartyMenuData *)0)->backup)

/* Staff/skill menu context: resource handles and current panel work. */
typedef struct CampMenuContext {
    u8 pad00[8];
    MenuPopupState transitionWork; /* 0x08: saved popup transition state */
    s32 popupState; /* 0x54: popup entry word */
    u8 pad58[8];
    s32 unk60;
    s32 resource;             /* 0x64 */
    s32 unk68;
    s32 displayVariant;       /* 0x6C: passed with display to menu drawing */
    u8 pad70[4];
    s32 option;               /* 0x74 */
    s32 actor;                /* 0x78 */
    s32 staffVariant;         /* 0x7C: passed with display to menu drawing */
    s32 staffParam;           /* 0x80 */
    u8 pad84[0x5C];
    s32 variant;              /* 0xE0 */
    u8 padE4[0xC];
    s32 panelResource;        /* 0xF0: grid resource handle */
    u8 padF4[0x28];
    const void *partySelectionLayout; /* 0x11C: copied panel layout address */
    s32 unk120;
    s32 panel;                /* 0x124 */
    u8 pad128[4];
    s32 panelList;            /* 0x12C */
    u8 pad130[8];
    s32 display;              /* 0x138 */
    u8 pad13C[0x20];
    MenuPageWindow partyWindow; /* 0x15C..0x7EB: sole primary page owner */
    PartyPanel partyPanel;    /* 0x7EC..0x8F7: occupied-party display owner */
    MenuPanelGroup *sceneGroup; /* 0x8F8 */
    MenuSpriteState *sprite;  /* 0x8FC */
    MenuSimpleSpriteState *effect; /* 0x900 */
    u8 pad904[8];
    s32 menu;                 /* 0x90C */
    u8 pad910[0x10];
    MenuProfilePanel *extraResource; /* 0x920 */
} CampMenuContext;

typedef struct StaffMenuWork {
    s32 handle;               /* 0x00 */
    u8 pad04[0xC];
    s32 staffMode;            /* 0x10 */
    s32 staffImage;           /* 0x14 */
    u8 pad18[4];
    s32 staffExit;            /* 0x1C */
    MenuPanelHandles *resourceList; /* 0x20 */
    s32 selectedList;         /* 0x24 */
    s32 activeMark;           /* 0x28 */
    s32 motionSelection; /* native initialization writes -1 or 10 */
    u32 selectionFlags;       /* 0x30 */
    u32 selectionId;          /* 0x34 */
} StaffMenuWork;

typedef struct MenuSelectionNode {
    s32 index;                /* 0x00 */
    u8 pad04[0x44];
    u32 flags;                /* 0x48 */
    u8 pad4C[0xC];
    struct MenuSelectionNode *next; /* 0x58 */
    u8 pad5C[4];
    u32 sortKey;              /* 0x60 */
} MenuSelectionNode;

typedef struct MenuSelectionList {
    u32 stateFlags;           /* 0x00 */
    u8 pad04[0xC];
    MenuSelectionNode *first; /* 0x10 */
    u8 pad14[8];
    s32 *selectedSlot;        /* 0x1C: current menu selection */
} MenuSelectionList;

typedef struct MenuSelectionState {
    u8 pad00[0x14];
    MenuSelectionList *list;  /* 0x14 */
} MenuSelectionState;

typedef struct MenuInputInfo {
    u8 pad00[0x48];
    u32 flags;           /* 0x48: selectable state */
    u8 pad4C[0x14];
    s32 target;          /* 0x60: current selection target */
} MenuInputInfo;

typedef struct MenuInputFlags {
    u32 bits;            /* 0x00: clear bit 3 each frame */
    u8 pad04[0x18];
    MenuInputInfo *info; /* 0x1C */
} MenuInputFlags;

typedef struct MenuInputNode {
    u8 pad00[0x14];
    MenuInputFlags *flags; /* 0x14 */
} MenuInputNode;

extern void mnuForwardDupArg(MenuWindowContainer *, s32, s32, s32, s32);
extern void mnuSeekListNode(s32, s32);



typedef struct MenuItemCount {
    u8 pad00[0x20];
    s32 count; /* 0x20 */
} MenuItemCount;

typedef struct MenuSpriteArguments {
    u8 pad00[0x55];
    s8 variant; /* 0x55: passed to sprite renderer */
} MenuSpriteArguments;

extern u32 mnuMapPadMaskToFlags(u32);
extern void mnuClearListFlagsOneAndTwo(u32 *);
extern void sndSetSequenceVolumePan();
extern s32 mnuInitializeStaffPartyScene(KwlnTask *);
extern void mnuSetPopupEntry();
extern void mnuSetPopupEntryFlagged(s32, void *);
extern void func_0027C788(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuClearWindowPanelTransitionFlag(s32);
extern void mnuPlayInputSound(s32, u32, s32);
extern u8 D_0037CC74[];
extern u8 D_0037CC3C[];
extern u8 D_0037CAB0[];
extern u8 D_0037CA78[];
extern void mnuStepPartyPanelListFromInput();
extern void mnuClearListFlags();
extern SdfMemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern void evtStageTestInit(s32);
extern void mnuActivatePanelAndConfigureGridResources(u32 *, s32, s32, s32);

extern s32 func_002877A8(void);

extern u32 kwlnTaskGetUserValue();


extern s32 D_003BAA7C;
extern FrFontGlyph *itfDrawUnderscoreTextSegment(s32, s32, s32, u32, const u8 *, s32);
extern s32 D_003BAA70;
extern void mnuDestroyWindowContainer(u32);

extern u8 D_0037CA58[];

void func_00274B80(u32 context) {
    mnuSetStaffDisplayMode(4, context);
}

void func_00274BA0(s32 context) {
}

s32 mnuIsFinalItemIndex(s32 index, s32 item) {
    if (index < (((MenuItemCount *)item)->count - 1)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274BC0);

extern s32 mnuCreateWindowContainer(s32, s32, s32, s32, s32);
extern void mnuSetWindowContainerState(MenuWindowContainer *, u32);
extern void mnuSetWindowPanelBounds(MenuWindowContainer *, const void *, u32, u32, u32, u32);
extern void mnuInitializeWindowEntryPlacement(s32, MenuWindowContainer *, s32, s32, s32);
extern void func_00274BC0(s32, s32, s32, struct MenuList *, struct MenuListNode *);
extern char D_003BC6E8[];
void func_00274D48(CampMenuContext *context) {
    PartyMenuData *party = (PartyMenuData *)context->menu;
    MenuWindowContainer *window;
    s32 i;
    s32 placement;

    window = (MenuWindowContainer *)mnuCreateWindowContainer(0, 0x140, 0x10, 6, 0x15);
    mnuSetWindowContainerState(window, 0x100);
    mnuForwardDupArg(window, context->option, 0, context->panelResource, 0x20);
    mnuSetWindowPanelBounds(window, context->partySelectionLayout, 0x30, 0x530, -0x90, 0xA10);
    window->list->context = context;
    window->list->drawCallback = func_00274BC0;
    for (i = 0; i < 5; i++) {
        if ((datGameState->party[i].flags & 1) != 0) {
            s32 id = datGameState->party[i].unitId;
            struct MenuListNode *node = mnuAppendWindowListNode(window, D_003BAA70 + id * 17);
            node->sortKeyPrimary = id - 1;
            node->sortKeySecondary = datGameState->party[i].level;
        }
    }
    mnuAppendWindowListNode(window, D_003BC6E8);
    window->list->visibleCount = window->list->count;
    switch (window->list->count) {
    case 2:
        placement = 0x26;
        break;
    case 3:
        placement = 0x28;
        break;
    case 4:
        placement = 0x2A;
        break;
    case 5:
        placement = 0x2C;
        break;
    default:
        placement = 0x12;
        break;
    }
    mnuInitializeWindowEntryPlacement(0, window, context->option, 0xA, placement);
    party->primaryWindow = window;
}

void mnuDestroyPartySelectionWindow(s32 context) {
    mnuDestroyWindowContainer((u32)((PartyMenuData *)((CampMenuContext *)context)->menu)->primaryWindow);
}

/* Snapshot five party entries; at most three active slots are displayed. */
void mnuCopyPartyEntries(context)
    s32 context;
{
    PartyMenuData *menuWork = (PartyMenuData *)((CampMenuContext *)context)->menu;
    PartyEntryCopy *entryCursor = menuWork->current;
    s32 entryIndex;
    s32 flagsByteOffset = MNU_STAFF_PARTY_BASE;
    s32 copyByteOffset = 0;

    menuWork->activeCount = 0;
    for (entryIndex = 0; entryIndex < MNU_STAFF_PARTY_SLOT_COUNT; entryIndex++) {
        *entryCursor = *(PartyEntryCopy *)(copyByteOffset + (s32)datGameState + MNU_STAFF_PARTY_BASE);
        if (((PartyEntryCopy *)((s32)datGameState + flagsByteOffset))->flags & MNU_STAFF_PARTY_ACTIVE_BIT) {
            menuWork->activeCount = menuWork->activeCount + 1;
        }
        flagsByteOffset += MNU_STAFF_PARTY_ENTRY_BYTES;
        entryCursor++;
        copyByteOffset += MNU_STAFF_PARTY_ENTRY_BYTES;
    }
    if (menuWork->activeCount >= MNU_STAFF_DISPLAY_OVERFLOW) {
        menuWork->activeCount = MNU_STAFF_DISPLAY_LIMIT;
    }
    menuWork->selection = 0;
}

extern void func_00285960(DatPartyRecord *, s32, u32, PartyPanel *);
extern void mnuUpdateHandleStates(MenuPageWindow *);
extern void func_00280048(s32);

/* Transfer one current party record and refresh the selected panel slot. */
void func_00275030(s32 partyIndex, s32 mode, s32 skipUpdate,
                   CampMenuContext *context) {
    PartyMenuData *menu = (PartyMenuData *)context->menu;
    s32 previousPanelCount = context->partyPanel.unk0;
    s32 previousPanelOther = context->partyPanel.unk4;
    s32 panelIndex;

    menu->backup[menu->selection] = *(PartyEntryCopy *)(
        partyIndex * (s32)sizeof(PartyEntryCopy) + (s32)menu + PARTY_CURRENT_OFFSET);
    memset(&menu->current[partyIndex], 0, sizeof(menu->current[partyIndex]));

    if (mode == 2) {
        menu->backup[menu->selection].flags |= 2;
        context->partyWindow.slots[menu->selection].flags &= ~0x80;
        func_00285960((DatPartyRecord *)&menu->backup[menu->selection], 0,
                      menu->selection, &context->partyPanel);
    } else {
        menu->backup[menu->selection].flags &= ~2;
        func_00285960((DatPartyRecord *)&menu->backup[menu->selection], 0,
                      menu->selection, &context->partyPanel);
    }

    context->partyPanel.slots[menu->selection].index = partyIndex;
    context->partyPanel.unk0 = previousPanelCount;
    context->partyPanel.unk4 = previousPanelOther;

    if (skipUpdate == 0) {
        mnuUpdateHandleStates(&context->partyWindow);
    }
    for (panelIndex = context->partyPanel.unk0; panelIndex < 5; panelIndex++) {
        context->partyWindow.slots[panelIndex].flags |= 0x80;
    }
    func_00280048((s32)&context->partyWindow);
    context->partyPanel.unk0++;
    context->partyPanel.unk4--;
    menu->selection++;
}

/* Notify active snapshot entries, restore the backup, then refresh panel resources.
 * The second loop counts down while the backup byte offset advances forward. */
void mnuRestorePartyEntriesAndRefresh(context)
    s32 context;
{
    PartyMenuData *menuWork = (PartyMenuData *)((CampMenuContext *)context)->menu;
    PartyEntryCopy *entryCursor = menuWork->current;
    s32 entryCounter;
    s32 backupByteOffset;
    s32 panelWork;

    for (entryCounter = 0; entryCounter < MNU_STAFF_PARTY_SLOT_COUNT; entryCounter++) {
        if (entryCursor->flags & MNU_STAFF_PARTY_ACTIVE_BIT) {
            func_00275030(entryCounter, -3, 1, (CampMenuContext *)context);
        }
        entryCursor++;
    }
    backupByteOffset = 0;
    for (entryCounter = MNU_STAFF_PARTY_LAST_SLOT; entryCounter >= 0; entryCounter--) {
        *(PartyEntryCopy *)(backupByteOffset + (s32)datGameState + MNU_STAFF_PARTY_BASE) = *(PartyEntryCopy *)(backupByteOffset + (s32)menuWork + PARTY_BACKUP_OFFSET);
        backupByteOffset += MNU_STAFF_PARTY_ENTRY_BYTES;
    }
    panelWork = (s32)&((CampMenuContext *)context)->partyWindow;
    mnuReleasePartyPanelTextures(panelWork);
    mnuInitPartyPanelSlots(&((CampMenuContext *)context)->partyPanel);
    mnuUpdateHandleStates((MenuPageWindow *)panelWork);
    func_00280048(panelWork);
}

/* Count active entries in the five-slot party array, capped at three. */
s32 mnuCountActiveSlots(void) {
    s32 entryCountdown;
    s32 activeCount = 0;
    u16 *entryFlagsCursor = (u16 *)((s32)datGameState + MNU_STAFF_PARTY_BASE);

    for (entryCountdown = MNU_STAFF_PARTY_LAST_SLOT; entryCountdown >= 0; entryCountdown--) {
        activeCount += *entryFlagsCursor & MNU_STAFF_PARTY_ACTIVE_BIT;
        entryFlagsCursor += MNU_STAFF_PARTY_HALFWORD_STRIDE;
    }
    return (activeCount < MNU_STAFF_DISPLAY_OVERFLOW) ? activeCount : MNU_STAFF_DISPLAY_LIMIT;
}

extern void mnuCopyPartyEntries();

/* Reset selection/backup state, activate all panel slots, and enable list nodes.
 * Keep the native short-arity snapshot call unchanged. */
void mnuClearPartySelectionAndActivateSlots(s32 context) {
    PartyMenuData *menuWork = (PartyMenuData *)((CampMenuContext *)context)->menu;
    s32 entryIndex;
    s32 nodeAddress;

    mnuCopyPartyEntries();
    menuWork->selection = 0;
    memset(menuWork->backup, 0, MNU_STAFF_BACKUP_BYTES);
    ((CampMenuContext *)context)->partyPanel.unk0 = 1;
    ((CampMenuContext *)context)->partyPanel.unk4 = mnuCountActiveSlots() - 1;
    mnuUpdateHandleStates(&((CampMenuContext *)context)->partyWindow);
    for (entryIndex = 0; entryIndex < MNU_STAFF_PARTY_SLOT_COUNT; entryIndex++) {
        ((CampMenuContext *)context)->partyWindow.slots[entryIndex].flags |= 0x80;
    }
    for (nodeAddress = (s32)menuWork->primaryWindow->list->first; nodeAddress != 0; nodeAddress = (s32)((MenuSelectionNode *)nodeAddress)->next) {
        ((MenuSelectionNode *)nodeAddress)->flags &= ~MNU_STAFF_NODE_UNAVAILABLE;
    }
}

/* Release panel textures before reinitializing slots and updating handle state. */
void mnuRefreshPartyPanelSlots(s32 context) {
    mnuReleasePartyPanelTextures((s32)&((CampMenuContext *)context)->partyWindow);
    mnuInitPartyPanelSlots(&((CampMenuContext *)context)->partyPanel);
    mnuUpdateHandleStates(&((CampMenuContext *)context)->partyWindow);
}

extern u32 sdfResourceRetainAddress(SdfMemBlock *allocation);
struct MenuPanelResources;
extern void mnuLoadPanelSectionResources(struct MenuPanelResources *, u32, u32,
                                         u32, s32);

s32 func_002755E0(KwlnTask *task) {
    CampMenuContext *context = (CampMenuContext *)kwlnTaskGetUserValue(task);
    SdfMemBlock *allocation = sdfAllocGeneralBlock(sizeof(PartyMenuData));
    PartyMenuData *menu = (PartyMenuData *)sdfResourceRetainAddress(allocation);
    s32 i;

    context->menu = (s32)menu;
    memset(menu, 0, sizeof(*menu));
    menu->allocation = (s32)allocation;

    func_00274B80((u32)context);
    func_00274D48(context);
    mnuLoadPanelSectionResources((struct MenuPanelResources *)&context->partyWindow.slots[0],
                                context->panelResource, 0x11, 0x21, -1);
    mnuLoadPanelSectionResources((struct MenuPanelResources *)&context->partyWindow.slots[1],
                                context->panelResource, 0x11, 0x22, 0x23);
    mnuLoadPanelSectionResources((struct MenuPanelResources *)&context->partyWindow.slots[2],
                                context->panelResource, 0x11, 0x22, 0x24);

    mnuClearPartySelectionAndActivateSlots((s32)context);
    for (i = 0; i < MNU_STAFF_PARTY_SLOT_COUNT; i++) {
        memcpy(&menu->original[i], &datGameState->party[i], sizeof(menu->original[i]));
        memcpy(menu->panelSnapshots[i],
               (u8 *)&context->partyWindow.slots[i] + 0x1C,
               sizeof(menu->panelSnapshots[i]));
    }

    mnuActivatePanelAndConfigureGridResources((u32 *)context->display,
                                              context->panelResource, 0, 1);
    menu->fadeB = menu->fadeA = MNU_FULL_FADE;
    return 1;
}

s32 mnuShopReleaseResources(void) {
    s32 context = kwlnTaskGetUserValue();
    PartyMenuData *menu = (PartyMenuData *)((CampMenuContext *)context)->menu;
    mnuRefreshPartyPanelSlots(context);
    mnuDestroyPartySelectionWindow(context);
    func_00274BA0(context);
    sdfReleaseResourceAllocation(menu->allocation);
    return 1;
}

void mnuPreparePartyPanelTransition(s32 menu) {
    mnuRestorePartyEntriesAndRefresh();
    mnuSetPopupEntryFlagged(menu + 0x54, (s32)D_0037CA58);
    mnuActivatePanelAndConfigureGridResources(((CampMenuContext *)menu)->display, ((CampMenuContext *)menu)->displayVariant, 0, 1);
}
s32 func_00275920(s32 callback) {
    CampMenuContext *context = (CampMenuContext *)kwlnTaskGetUserValue((KwlnTask *)callback);
    PartyMenuData *menuWork = (PartyMenuData *)context->menu;
    s32 inputFlags = mnuMapPadMaskToFlags(0x33);
    s32 *popup = &context->popupState;
    MenuWindowContainer *window = menuWork->primaryWindow;
    s32 entryIndex = window->list->cursor->index;
    s32 state = func_00285670(&context->transitionWork, popup, 0, (void *)callback);

    if (state != 0) {
        return state;
    }
    if (*popup == 0) {
        if ((inputFlags & 0x300000) == 0) {
            func_0027C788((s32)window);
        }
        if (inputFlags & 0x10) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (inputFlags & 0x20) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag((s32)window);
        if (inputFlags & 1) {
            switch (mnuIsFinalItemIndex(window->list->cursor->index, (s32)window->list)) {
            case 0:
                if ((u16)(menuWork->current[entryIndex].flags & 1) != 0) {
                    window->list->cursor->flags48 |= 1;
                    func_00275030(entryIndex, 2, 0, context);
                } else {
                    inputFlags = 0x8000;
                }
                break;
            case 1:
                if (menuWork->selection > 0) {
                    mnuPreparePartyPanelTransition((s32)context);
                } else {
                    inputFlags = 0x8000;
                }
                break;
            }
        } else if (menuWork->activeCount == menuWork->selection) {
            mnuPreparePartyPanelTransition((s32)context);
        }
        if (inputFlags & 2) {
            if (menuWork->selection > 0) {
                mnuInitPartyPanelSlots(&context->partyPanel);
                mnuClearPartySelectionAndActivateSlots((s32)context);
                func_00280048((s32)&context->partyWindow);
            } else {
                mnuSetPopupEntryFlagged((s32)popup, D_0037CA58);
                mnuActivatePanelAndConfigureGridResources((u32 *)context->display, context->displayVariant, 0, 1);
            }
        }
        mnuPlayInputSound(0, inputFlags, (s32)&window->list->stateFlags);
    }
    return 0;
}


void func_00275B40(EffectSlotSet **sets, MenuEffectPair *hpBar, MenuEffectPair *mpBar,
                   DatPartyRecord *entry, EffectSlotSet *marker, u32 opacity,
                   u32 iconOpacity, s32 dim) {
    const char *name;
    s32 profile = ptyGetCurrentProfileId(entry);
    u32 color = uiBlendColors(0xA09DC380, 0xA09DC300, opacity);
    FrFontGlyph *glyph;
    s32 i;

    func_002BF4E0(0x350, 0x938, 0, opacity, 1, sets[0], 0x25, 0x53);
    func_002BF4E0(0x350, 0x990, 0, opacity, 1, sets[0], 0x26, 0x53);
    func_002BF4E0(0x350, 0xBA0, 0, opacity, 1, sets[0], 0x27, 0x53);
    for (i = 0; i < 4; i++) {
        if (!dim) {
            sets[entry->unitId]->workEntries[0].geometry.cornerColors[i] =
                sets[entry->unitId]->workEntries[0].savedColors[i];
        } else {
            sets[entry->unitId]->workEntries[0].geometry.cornerColors[i] =
                uiBlendColors(sets[entry->unitId]->workEntries[0].savedColors[i],
                              sets[entry->unitId]->workEntries[0].savedColors[i] & 0xFF,
                              0x80);
        }
    }
    func_002BF4E0(0, 0x840, 0, iconOpacity, 1, sets[entry->unitId], 0, 0x53);
    func_002BF4E0(0x4C0, 0x8B0, 0, opacity, 1, sets[0], 9, 0x53);
    func_002BF4E0(0x4C0, 0xAE8, 0, opacity, 1, sets[0], 10, 0x53);
    func_002BF4E0(0x560, 0xC98, 0, opacity, 1, sets[0], 0x1F, 0x53);
    hpBar->opacity = opacity;
    func_002845F8(0x6E0, 0x9B8, 0, color, entry->hp, -1, hpBar, 0x53);
    mpBar->opacity = opacity;
    func_002845F8(0x6E0, 0xA48, 0, color, entry->mp, -1, mpBar, 0x53);
    func_002BF4E0(0x740, 0xCB0, 0, opacity, 1, sets[0], 0x1D, 0x53);
    func_002BF4E0(0x11D0, 0xCB0, 0, opacity, 1, sets[0], 0x1E, 0x53);
    if (profile) {
        if (func_002CD240(profile, &name)) {
            mnuDrawCenteredLabel(0x650, 0xBC8, 0, color, profile, 0x53);
        } else {
            glyph = itfCreateConvertedTextGlyph(
                0x640, 0xBE8, 0, color, (const u8 *)name, 0);
            func_00196088(0xD20, 0xBE8, glyph);
            func_001958A0(glyph, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(glyph);
        }
    } else {
        glyph = itfCreateConvertedTextGlyph(
            0x640, 0xBE8, 0, color, (const u8 *)D_003BC6F0, 0);
        glyph = itfCreateConvertedTextGlyph(
            0xB50, 0xBF0, 0, color, (const u8 *)D_003BC6F8, glyph);
        func_001958A0(glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(glyph);
    }
    if (marker) {
        func_002BF4E0(0x970, 0x8B0, 0, opacity, 1, sets[0], 0x1C, 0x53);
        func_002BF4E0(0x970, 0x8B0, 0, opacity, 1, marker, 0, 0x53);
    }
}


/* Two-stage fade: B rises first when opening, A falls first when closing.
 * The second ramp begins strictly past its threshold; clamps follow each step. */
void mnuUpdateStaffFade(s32 opening, PartyMenuData *menuWork) {
    if (opening == 0) {
        if (menuWork->fadeA > 0) {
            menuWork->fadeA -= MNU_STAFF_FADE_STEP;
        }
        if (menuWork->fadeA < 0) {
            menuWork->fadeA = 0;
        }
        if (menuWork->fadeA < MNU_STAFF_FADE_CLOSE_THRESHOLD) {
            if (menuWork->fadeB > 0) {
                menuWork->fadeB -= MNU_STAFF_FADE_STEP;
            }
            if (menuWork->fadeB < 0) {
                menuWork->fadeB = 0;
            }
        }
    } else {
        if (menuWork->fadeB < MNU_FULL_FADE) {
            menuWork->fadeB += MNU_STAFF_FADE_STEP;
        }
        if (menuWork->fadeB > MNU_FULL_FADE) {
            menuWork->fadeB = MNU_FULL_FADE;
        }
        if (menuWork->fadeB > MNU_STAFF_FADE_OPEN_THRESHOLD) {
            if (menuWork->fadeA < MNU_FULL_FADE) {
                menuWork->fadeA += MNU_STAFF_FADE_STEP;
            }
            if (menuWork->fadeA > MNU_FULL_FADE) {
                menuWork->fadeA = MNU_FULL_FADE;
            }
        }
    }
}

void func_00276018(s32 contextAddress) {
    CampMenuContext *context = (CampMenuContext *)contextAddress;
    PartyMenuData *menu = (PartyMenuData *)context->menu;
    struct MenuList *list;
    struct MenuListNode *node;
    MenuProfilePanel *profilePanel;
    DatPartyRecord *record;
    EffectSlotSet **sets;
    EffectSlotSet *marker;
    s32 selectionKey;
    s32 statusIndex;
    s32 found = 0;

    node = menu->primaryWindow->list->cursor;
    if (mnuIsFinalItemIndex(node->index, (s32)menu->primaryWindow->list)) {
        selectionKey = menu->selectionKey;
        mnuUpdateStaffFade(0, menu);
    } else {
        selectionKey = menu->primaryWindow->list->cursor->index;
        menu->selectionKey = selectionKey;
        mnuUpdateStaffFade(1, menu);
    }

    list = menu->primaryWindow->list;
    node = list->first;
    if (node != NULL) {
        do {
            if (node->index == selectionKey) {
                if ((node->flags48 & 1) != 0) {
                    found = 1;
                }
            }
            node = node->next;
        } while (node != NULL);
    }

    record = (DatPartyRecord *)&menu->original[selectionKey];
    statusIndex = mnuGetSelectionFromFlags(record);
    if (statusIndex >= 0) {
        marker = *(EffectSlotSet **)((u8 *)context + 0xC4 + statusIndex * 4);
    } else {
        marker = NULL;
    }
    sets = (EffectSlotSet **)((u8 *)context + 0xF0);

    func_00275B40(sets,
                  (MenuEffectPair *)&menu->panelSnapshots[selectionKey][0],
                  (MenuEffectPair *)&menu->panelSnapshots[selectionKey][0x54],
                  record, marker,
                  (u32)menu->fadeA, (u32)menu->fadeB, found);

    profilePanel = mnuCreateProfilePanel(record);
    profilePanel->phase = menu->profilePanelPhase;
    profilePanel->opacity = (u32)menu->fadeA;
    mnuCacheProfilePanelGridPositions(profilePanel, (u32)sets[0],
                                      0x18, 0x28, 0x29);
    mnuDrawAndAdvanceProfilePanel(0x820, 0xCD8, 0, profilePanel, 0x53);
    menu->profilePanelPhase = profilePanel->phase;
    mnuFreeProfilePanelWork(profilePanel);
}

s32 mnuDrawPartySelectionPanelAndStep(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    PartyMenuData *menu = (PartyMenuData *)((CampMenuContext *)context)->menu;

    func_00272778(callback);
    mnuCreateStaffImageSprite(0x13);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, menu->primaryWindow, 0x53);
    func_00276018(context);
    func_002723B0(0, ((CampMenuContext *)context)->actor);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 mnuStepPartySelectionControl(s32 callback) {
    return menuRunPanel((void *)kwlnTaskGetUserValue(), 2, (void *)callback);
}

u8 mnuIsStateNotOne(void) {
    s64 result;

    result = func_002877A8();
    return result != 1;
}

void func_002762B0(u32 context) {
    mnuSetStaffDisplayMode(3, context);
}

void mnuReleaseMenuWindowHandles() {
}

/* Release both staff resource slots; their menu indices differ between games. */
void mnuReleaseStaffMenuResources(s32 *menuWork) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        effResolveAndReleaseResource(menuWork[7 + resourceIndex]);
    }
}

/* Reset texture handles for the same two resource slots. */
void mnuReleaseStaffMenuTextureHandles(s32 *menuWork) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        effReleaseTextureHandlesAndResetSlots((EffectSlotSet *)menuWork[7 + resourceIndex]);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276368);

s32 mnuStaffCloseSelectionState(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 menu = ((CampMenuContext *)context)->menu;
    mnuResetWorkFloats();
    mnuReleaseMenuWindowHandles(context);
    sdfReleaseResourceAllocation(*(s32 *)menu);
    return 1;
}

void func_00276478(u32 context) {
    func_00276368(context, 1);
}

void func_00276490(void) {
    mnuStaffCloseSelectionState();
}

void func_002764A8(u32 context) {
    func_00276368(context, 0);
}

void func_002764C0(void) {
    mnuStaffCloseSelectionState();
}

/* Update the popup first; accept confirm/cancel only while its state is zero.
 * Return a nonzero popup-update word unchanged, or zero after handling input. */
s32 mnuStaffPopupUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u8 *menuWork = (u8 *)((CampMenuContext *)context)->menu;
    s32 *popupState = (s32 *)(context + 0x54);
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_POPUP_INPUT_MASK);
    s32 stateWord;
    s32 panelWork;
    stateWord = menuRunPanel((void *)context, 0, (void *)callback);
    if (stateWord != 0) {
        return stateWord;
    }
    if (*popupState == 0) {
        panelWork = (s32)&((CampMenuContext *)context)->partyWindow;
        mnuStepPartyPanelListFromInput(4, panelWork);
        if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
            *(s32 *)(menuWork + 0x18) = ((CampMenuContext *)context)->partyWindow.lists[0]->cursor->index;
            mnuSetPopupEntry(popupState, D_0037CAB0);
            *(s32 *)(menuWork + 0x24) = 1;
        }
        if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
            mnuSetPopupEntryFlagged((s32)popupState, D_0037CA78);
            mnuActivatePanelAndConfigureGridResources(((CampMenuContext *)context)->display, ((CampMenuContext *)context)->displayVariant, 0, 1);
            mnuClearListFlags(0, panelWork);
        }
        mnuPlayInputSound(0, inputFlags, 0);
    }
    return 0;
}

extern u8 D_0037C3A8[];
extern s32 effHasFirstTextureHandle(s32);
extern void mnuDrawStaffCampScreen();
extern void mnuCreateStaffImageSprite();
extern void func_002723B0();
extern void func_00272668();
extern void mnuDrawWindowContainer();

s32 mnuDrawStaffCampPageWithImage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    CampMenuContext *work = (CampMenuContext *)context;
    StaffMenuWork *menu = (StaffMenuWork *)work->menu;

    if (effHasFirstTextureHandle(work->resource) != 0) {
        mnuDrawStaffCampScreen(0, callback);
    } else {
        mnuDrawStaffCampScreen(1, callback);
    }
    if (menu->staffImage == 0) {
        mnuCreateStaffImageSprite(0x12);
    } else {
        mnuCreateStaffImageSprite(0x11);
    }
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, work->panel, 0x53);
    func_002723B0(0, work->actor);
    if (menu->selectedList != 0) {
        s32 *slot = ((MenuSelectionState *)work->panel)->list->selectedSlot;

        func_00272668(1, *slot, D_0037C3A8, context, 1, 0x53, slot);
    }
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 mnuStepStaffCampPageControl(s32 callback) {
    return menuRunPanel((void *)kwlnTaskGetUserValue(), 2, (void *)callback);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276720);


s32 mnuInitializeStaffPartyScene(KwlnTask *task) {
    CampMenuContext *context = (CampMenuContext *)kwlnTaskGetUserValue(task);
    MenuPageWindow *page = &context->partyWindow;
    StaffMenuWork *menu = (StaffMenuWork *)context->menu;
    s32 index = context->partyWindow.lists[0]->cursor->index;
    DatPartyRecord *record = &datGameState->party[index];
    MenuProfilePanel *profilePanel;

    effReleaseTextureHandlesAndResetSlots((EffectSlotSet *)context->resource);
    mnuStoreScrollPanelSelectionAndGridPosition((struct MenuScrollPanel *)context->display,
                                               context->staffVariant, 0x3D, 1);
    mnuSetWindowResource(index, page, context->staffVariant, context->staffParam);
    mnuAttachPartyIconBundle(index, page, context->staffVariant);
    context->sceneGroup = mnuCreatePanelGroup(context->staffVariant);
    context->sprite = mnuCreateSpriteState((EffectSlotSet *)context->option,
                                        (EffectSlotSet *)context->unk68,
                                        (EffectSlotSet *)context->staffVariant);
    context->effect = mnuAllocateSimpleSprite((EffectSlotSet *)context->option,
                                            (EffectSlotSet *)context->unk68,
                                            (EffectSlotSet *)context->displayVariant,
                                            (EffectSlotSet *)context->unk60,
                                            (EffectSlotSet *)context->staffVariant);
    profilePanel = mnuCreateProfilePanel(record);
    context->extraResource = profilePanel;
    mnuCacheProfilePanelGridPositions(profilePanel, context->staffParam, 5, 0xE, 0xF);
    menu->resourceList = mnuCreatePanelSpriteHandles(1, context->displayVariant, context->unk120);
    if (mnuClassifyQuarterHalfPercent(record->hp, record->maxHp) < 2) {
        menu->motionSelection = -1;
    } else {
        menu->motionSelection = 10;
    }
    evtStageTestSelectEntry(record->unitId, menu->motionSelection, 0);
    func_00276720((s32)page, 1, menu->staffImage, menu->staffMode);
    return 1;
}


extern void btlStopStage();
extern void mnuClearEntries(MenuPageWindow *);
extern void mnuReleasePartyIconBundles(MenuPageWindow *);
extern void mnuFreeProfilePanelWork(MenuProfilePanel *);
extern void mnuReleaseResourceList(MenuPanelHandles *);

/* Tear down the staff panel and all four optional scene-side resources. */
s32 mnuStaffReleasePanelScene(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    MenuPageWindow *entryList = &((CampMenuContext *)context)->partyWindow;
    CampMenuContext *work = (CampMenuContext *)context;

    func_00276720((s32)entryList, 0, menu->staffImage, menu->staffMode);
    btlStopStage();
    mnuClearEntries(entryList);
    mnuReleasePartyIconBundles(entryList);
    if (work->sceneGroup != 0) {
        mnuDestroyPanelGroup(work->sceneGroup);
        work->sceneGroup = 0;
    }
    if (work->sprite != 0) {
        mnuFreeSpriteStateWork(work->sprite);
        work->sprite = 0;
    }
    if (work->effect != 0) {
        mnuFreeSimpleSpriteWork(work->effect);
        work->effect = 0;
    }
    if (work->extraResource != 0) {
        mnuFreeProfilePanelWork(work->extraResource);
        work->extraResource = 0;
    }
    mnuReleaseResourceList(menu->resourceList);
    effResolveAndReleaseResource(work->resource);
    mnuStoreScrollPanelSelectionAndGridPosition((struct MenuScrollPanel *)work->display, work->resource, 0, 0);
    return 1;
}

void mnuResetSelectedPanelOpacity(s32 context) {
    CampMenuContext *work = (CampMenuContext *)context;
    s32 index = work->partyWindow.lists[0]->cursor->index;
    work->partyWindow.slots[index].windowSprites->profileFade = 0x100;
}

/* Switch the party page, rebuilding its panels; previous takes priority.
 * The opaque request argument is forwarded unchanged. Return one if switched. */
s32 mnuStaffSwitchPartyPage(s32 requestArgument) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menuWork = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    s32 pageChanged = 0;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_PAGE_INPUT_MASK);

    if (inputFlags & MNU_STAFF_INPUT_PREV_PAGE) {
        mnuStaffReleasePanelScene(requestArgument);
        mnuRetreatListCursorDefault(((CampMenuContext *)context)->partyWindow.lists[0]);
        pageChanged = 1;
    }
    if (inputFlags & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        mnuStaffReleasePanelScene(requestArgument);
        mnuAdvanceListCursorDefault(((CampMenuContext *)context)->partyWindow.lists[0]);
        pageChanged = 1;
    }
    mnuClearListFlagsOneAndTwo(&((CampMenuContext *)context)->partyWindow.lists[0]->stateFlags);
    if (pageChanged != 0) {
        mnuInitializeStaffPartyScene((KwlnTask *)requestArgument);
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        menuWork->activeMark = 0;
        if (menuWork->staffMode == 1) {
            mnuResetSelectedPanelOpacity(context);
        }
        return 1;
    }
    return 0;
}

extern s32 func_00286F48();
extern u8 D_0037CA94[];

/* Update popup state before page navigation, view toggles, and exit requests.
 * stateWord holds the update result first, then the stored popup state. */
s32 mnuStaffBrowsePartyUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menuWork = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    s32 *popupState;
    u32 inputFlags;
    s32 stateWord;

    if (menuWork->staffImage == 0) {
        inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_VIEW_INPUT_MASK);
    } else {
        inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_INPUT_CANCEL);
    }
    popupState = (s32 *)(context + 0x54);
    stateWord = menuRunPanel((void *)context, 0, (void *)callback);
    if (stateWord != 0) {
        return stateWord;
    }
    if (func_00286F48() != 0) {
        return 0;
    }
    stateWord = *popupState;
    menuWork->staffExit = 0;
    if (stateWord == 0) {
        if (mnuStaffSwitchPartyPage(callback) != 0) {
            return 0;
        }
        if (inputFlags & MNU_STAFF_VIEW_TOGGLE_MASK) {
            if (menuWork->staffMode == 0) {
                menuWork->staffMode = 1;
                func_00276720((s32)&((CampMenuContext *)context)->partyWindow, 3, menuWork->staffImage, 1);
            } else {
                menuWork->staffMode = 0;
                func_00276720((s32)&((CampMenuContext *)context)->partyWindow, 2, menuWork->staffImage, 0);
            }
            menuWork->activeMark = 0;
        }
        if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
            if (func_002877A8() != 1) {
                btlStopStage();
                menuWork->staffExit = 1;
                menuWork->selectedList = 1;
                mnuSetPopupEntryFlagged(popupState, D_0037CA94);
            } else {
                inputFlags = MNU_STAFF_INPUT_REJECTED;
            }
        }
        mnuPlayInputSound(0, inputFlags, 0);
    }
    return 0;
}

void mnuDrawSlotIcons(s32 x, MenuPageWindow *page) {
    DatPartyRecord *slot = &datGameState->party[page->lists[0]->cursor->index];
    s32 i;
    s32 y;
    FrFontGlyph *handle;

    itfSetTextDrawLimit(0x13);
    x = x * 8;
    y = x + 0xbc0;
    for (i = 0; i < 3; i++, y += 0xa8) {
        handle = itfDrawUnderscoreTextSegment(0x190, y, 0, 0xa09dc359, (const u8 *)(D_003BAA7C + slot->unitId * 45), i);
        if (handle != 0) {
            func_001958A0(handle, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(handle);
        }
    }
    itfSetTextDrawLimit(-1);
}

extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);

void mnuDrawStaffPanelGridBackdrop(s32 flag, StaffSlots *slots) {
    s32 y;

    for (y = 0x360; y < 0xE40; y += 0x38) {
        itfDrawGridWithResolvedSlot(0xE80, y, 0, 1, (u32)slots->pairResources[1], 2, 0x53);
    }
    itfDrawGridWithResolvedSlot(0x10F0, 0x358, 0, 1, (u32)slots->pairResources[1], 4, 0x53);
    itfDrawGridWithResolvedSlot(0x1050, 0x500, 0, 1, (u32)slots->pairResources[1], 3, 0x53);
    if (flag == 0) {
        itfDrawGridWithResolvedSlot(-0x140, -0xA0, 0, 1, (u32)slots->pairResources[1], 7, 0x53);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276F70);

extern void frFontAddSharedGlyphFlags(s32);
extern u8 frFontClearFlagBits(u8);

void mnuDrawTextSprite(s32 x, s32 y, s32 scale, s32 color, s32 textId, s32 param) {
    FrFontGlyph *item;
    s32 top = y - 0x10;

    frFontAddSharedGlyphFlags(1);
    item = frFontAppendGlyphFromData((void *)textId, 0, 0, 0, 0);
    frFontSetContextPair(item, x, top);
    frFontStoreShiftedContextValue(item, scale * 0x10);
    frFontSetChildColors(item, color);
    frFontClearFlagBits(1);
    func_001958A0(item, 1, param);
    frFontQueueGlyphInSelectedSlot(item);
}

void mnuDrawPartySkillAndStatusPanel(u8 *entry, MenuPageWindow *page, MenuPanelGroup *packedGroup, MenuSpriteState *spriteState, s32 obj, s32 spriteFlags) {
    mnuDrawAndAdvancePanelGroup(0xeb0, 0x518, 0, entry, packedGroup, spriteFlags);
    mnuDrawPartyInfoSprites(0, 0, 0, entry, spriteState, spriteFlags);
    itfDrawGridWithResolvedSlot(0xb0, 0xa68, 0, 1, *(s32 *)(obj + 0x1c), 0x37, spriteFlags);
    mnuDrawTextSprite(0x220, 0xa20, 0, 0xa09dc380, D_003BAA70 + ((PartyEntryCopy *)entry)->displayId * 17 + 0x110, spriteFlags);
    itfDrawGridWithResolvedSlot(0x120, 0xad0, 0, 1, *(s32 *)(obj + 0x14), 0x25, spriteFlags);
    mnuDrawSlotIcons(-0x16, page);
}

extern void func_00283838(s32, s32, s32, s32, s32, s32, s32);

void mnuDrawProfilePanelAndSprite(s32 obj, s32 unused1, s32 spriteGroup, s32 drawGroup, s32 unused4, s32 spriteFlags) {
    func_00283838(0, 0, 0, obj, ((MenuSpriteArguments *)obj)->variant, spriteGroup, spriteFlags);
    mnuDrawAndAdvanceProfilePanel(0x1200, 0x730, 0, drawGroup, spriteFlags);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277390);

extern u32 effMiscRand(s32);
extern s32 evtStageTestHasPendingMotion();
extern u32 evtStageTestCountFlags(s32);
extern void evtStageTestQueueMotion(s32, u32);

typedef struct MenuIdleVoiceState {
    u8 pad00[0x28];
    s32 idleTicks;
    s32 voiceState; /* -1 permits a new random voice */
} MenuIdleVoiceState;

#define MENU_IDLE_VOICE_TICKS 0x12D

/* Pick another idle voice after MENU_IDLE_VOICE_TICKS eligible ticks. */
void mnuIdleVoiceTimer(MenuIdleVoiceState *voiceTimer) {
    u32 voiceCount;

    if (voiceTimer->voiceState == -1 && func_002877A8() != 1) {
        if (evtStageTestHasPendingMotion() == 0) {
            voiceTimer->idleTicks = voiceTimer->idleTicks + 1;
        }
        if (voiceTimer->idleTicks >= MENU_IDLE_VOICE_TICKS) {
            voiceCount = evtStageTestCountFlags(0);
            evtStageTestQueueMotion(0, effMiscRand(0) % voiceCount);
            voiceTimer->idleTicks = 0;
        }
    }
}

s32 mnuStaffIdlePartyUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;

    if (menu->staffMode == 0) {
        mnuIdleVoiceTimer((MenuIdleVoiceState *)menu);
    }
    return menuRunPanel((void *)context, 2, (void *)callback);
}

u32 func_00277638(void) {
    return 1;
}

void mnuDrawRangeCostAndIcon(s32 x, s32 y, s32 depth, s32 xOffset, u32 fade,
                   s32 actor, u16 rangeId, s32 style, s32 dim,
                   EffectSlotSet *specialResource, EffectSlotSet *costResource, u32 texture) {
    char text[16];
    u32 color;
    s32 value;
    FrFontGlyph *glyph;
    u8 chainFlag;

    color = uiBlendColors(0xA09DC380, 0xA09DC300, fade);
    if (rangeId >= 0x261) {
        return;
    }
    if (mnuLookupRangeEntry(rangeId) == 3) {
        func_002BF4E0(x + xOffset - 0x80, y, depth, fade, 1,
                      specialResource, style + 0xB, texture);
        return;
    }
    if (actor != 0) {
        value = mnuGetAdjustedEntryValue(rangeId, (DatPartyRecord *)actor);
    } else {
        value = mnuGetAdjustedPartyRangeValue(rangeId);
    }
    chainFlag = style != 0 ? 4 : 0;
    if (dim != 0) {
        color = uiBlendColors(color, color & 0xFFFFFF00, 0x80);
    }
    func_003014F0(text, D_003BC700, value);
    glyph = func_001978E8(x - 0x90, y, depth, color, text, 0);
    x += 0x140;
    frFontSetChainFlag(glyph, chainFlag);
    func_001958A0(glyph, 1, texture);
    frFontQueueGlyphInSelectedSlot(glyph);
    switch (mnuGetRangeEntryKind(rangeId)) {
    case 1:
    default:
        func_002BF4E0(x, y, depth, fade, 1, costResource, dim != 0 ? 0xD : 0xC, texture);
        break;
    case 2:
        func_002BF4E0(x, y, depth, fade, 1, costResource, dim != 0 ? 0xF : 0xE, texture);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277848);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2208);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2260);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2270);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2280);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2290);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22A0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22B0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22C0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22D0);

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuBuildEquippedSlots);

u32 mnuDestroySelectedPartyWindow() {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    mnuDestroyWindowContainer(menu->selectedList);
    menu->selectedList = 0;
    return 1;
}

/* Seek the first node with a nonzero sort key and no unavailable flag.
 * Leave the cursor unchanged if no such node exists. */
void mnuSeekFirstAvailableStaffListNode(void) {
    StaffMenuWork *menuWork = (StaffMenuWork *)((CampMenuContext *)kwlnTaskGetUserValue())->menu;
    MenuSelectionNode *listNode = ((MenuSelectionState *)menuWork->selectedList)->list->first;

    while (listNode != NULL) {
        u32 sortKey = listNode->sortKey;
        if (!(listNode->flags & MNU_STAFF_NODE_UNAVAILABLE) && sortKey != 0) {
            break;
        }
        listNode = listNode->next;
    }
    if (listNode != NULL) {
        mnuSeekListNode(listNode->index, (s32)((MenuSelectionState *)menuWork->selectedList)->list);
    }
}

u32 ptySkillMenuRebuildAfterMutation(s32 actor, s32 contextArg) {
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)kwlnTaskGetUserValue(contextArg))->menu;
    s32 selected = *((MenuSelectionState *)menu->selectedList)->list->selectedSlot;
    s32 state;

    mnuDestroySelectedPartyWindow(contextArg);
    ptySkillMenuBuildEquippedSlots(actor, contextArg);
    mnuSeekListNode(selected, (s32)((MenuSelectionState *)menu->selectedList)->list);
    state = menu->selectedList;
    *(s32 *)(state + 0x44) = 0x200;
    *(s32 *)(state + 0x48) = 0x100;
    return state;
}

void func_00277DD0(u32 context) {
    mnuSetStaffDisplayMode(1, context);
}

void func_00277DF0(s32 context) {
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuDrawEntry);

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuRefreshEntries);

typedef struct SkillInfo {
    u8 pad00[0x20];
    u32 count;
    u16 codes[14];
} SkillInfo;

extern s32 sdfAllocSizeClassBlock(s32);
extern void prfBuildRawSkillList(u32, SkillInfo *);

s32 mnuBuildSkillCodeBitset(void) {
    s32 id = 0;
    u32 *bits = (u32 *)sdfAllocSizeClassBlock(0x50);
    SkillInfo info;

    memset(bits, 0, 0x50);
    for (id = 0; id < 0x60; id++) {
        u32 i;

        prfBuildRawSkillList(id & 0xFFFF, &info);
        for (i = 0; i < info.count; i++) {
            u16 code = info.codes[i];

            if (code != 0) {
                bits[code >> 5] |= 1 << code;
            }
        }
    }
    return (s32)bits;
}

extern void sdfReleaseChipBlock(void *);

void func_002782E0(u32 *bits) {
    sdfReleaseChipBlock(bits);
}

s32 mnuIsSkillCodeInBitset(s32 code, u32 *bits) {
    s32 roundedCode = (code < 0) ? code + 0x1f : code;

    return (bits[roundedCode >> 5] & (1 << code)) != 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuInitPages);

void mnuDestroySkillMenuWindows(s32 context) {
    u32 *menu = (u32 *)((CampMenuContext *)context)->menu;
    if (menu[2] != 0) {
        u32 i = 0;
        u32 *resource = menu + 4;
        mnuDestroyPanelState(menu[8]);
        mnuDestroyListState(menu[3]);
        do {
            mnuDestroyWindowContainer(*resource++);
            i++;
        } while (i < 4);
        menu[2] = 0;
    }
}

s32 mnuCampMenuInit(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 handle = sdfAllocGeneralBlock(0x38);
    s32 *menu = sdfResourceRetainAddress(handle);
    CampMenuContext *work = (CampMenuContext *)context;

    work->menu = (s32)menu;
    memset(menu, 0, 0x38);
    *menu = handle;
    func_00277DD0(context);
    mnuForwardDupArg((MenuWindowContainer *)work->panelList, work->option, 0, 0, 0);
    switch (*((MenuSelectionState *)work->panel)->list->selectedSlot) {
    case 0:
        mnuActivatePanelAndConfigureGridResources(work->display, work->variant, 0, 1);
        break;
    case 2:
        mnuActivatePanelAndConfigureGridResources(work->display, work->variant, 0x35, 0x36);
        break;
    default:
        mnuActivatePanelAndConfigureGridResources(work->display, work->variant, 0x33, 0x34);
        break;
    }
    mnuSeekListNode(0, ((MenuSelectionState *)work->panelList)->list);
    return 1;
}

s32 mnuCloseItemSelectionState(s32 contextArg) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;

    if (menu->selectedList != 0) {
        mnuDestroySelectedPartyWindow(contextArg);
    }
    func_00277DF0(context);
    sdfReleaseResourceAllocation(menu->handle);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuShellUpdate);

s32 mnuCampMenuDrawSlotLabel(s32 param) {
    s32 context = kwlnTaskGetUserValue();
    s32 *slot;

    func_00272778(param);
    slot = ((MenuSelectionState *)((CampMenuContext *)context)->panel)->list->selectedSlot;
    if (*slot == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xA);
    }
    func_00272668(1, *((MenuSelectionState *)((CampMenuContext *)context)->panel)->list->selectedSlot, D_0037C3A8, context, 1, 0x53);
    /* Both arms are identical in retail; kept as written. */
    if (*((MenuSelectionState *)((CampMenuContext *)context)->panel)->list->selectedSlot == 0) {
        mnuDrawWindowContainer(0x1C0, 0x3D0, 0, ((CampMenuContext *)context)->panel, 0x53);
    } else {
        mnuDrawWindowContainer(0x1C0, 0x3D0, 0, ((CampMenuContext *)context)->panel, 0x53);
    }
    func_002723B0(0, ((CampMenuContext *)context)->actor);
    return menuRunPanel((void *)context, 1, (void *)param);
}

s32 mnuStepSkillSlotControl(s32 callback) {
    return menuRunPanel((void *)kwlnTaskGetUserValue(), 2, (void *)callback);
}

void mnuClearSelectedListNodeId() {
    s32 context = kwlnTaskGetUserValue();
    ((StaffMenuWork *)((CampMenuContext *)context)->menu)->selectionId = 0xffffffff;
}

u32 mnuHasSelectedListNodeId(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return ~((StaffMenuWork *)((CampMenuContext *)context)->menu)->selectionId >> 0x1f;
}

/* Set the selected flag only on nodes whose index matches the saved selection. */
void mnuHighlightSelectedListNode() {
    StaffMenuWork *menuWork = (StaffMenuWork *)((CampMenuContext *)kwlnTaskGetUserValue())->menu;
    MenuSelectionNode *listNode = ((MenuSelectionState *)menuWork->selectedList)->list->first;

    for (; listNode != 0; listNode = listNode->next) {
        if (listNode->index == menuWork->selectionId) {
            listNode->flags |= MNU_STAFF_NODE_SELECTED;
        } else {
            listNode->flags &= ~MNU_STAFF_NODE_SELECTED;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278C90);

u32 mnuResetStaffSelectionFlags(void) {
    s32 context = kwlnTaskGetUserValue();
    ((StaffMenuWork *)((CampMenuContext *)context)->menu)->selectionFlags = 0;
    return 1;
}

extern s32 ptyHasSkill(DatPartyRecord *unit, s32 skillId);
extern void ptyRecomputeMaxHpMp(DatPartyRecord *unit);
extern void scrClearSecondaryScriptFlag(DatPartyRecord *unit, u16 flagId);

/* Narrow the ID to its native 16-bit skill code before duplicate detection.
 * Insert only missing skills, then recompute maxima and clear the script flag. */
void mnuAddPartySkillIfMissing(DatPartyRecord *partyEntry, s32 skillId, s32 skillSlot) {
    u16 skillCode = skillId;

    if (ptyHasSkill(partyEntry, skillCode) == 0) {
        partyEntry->effectData[skillSlot] = skillCode;
        ptyRecomputeMaxHpMp(partyEntry);
        scrClearSecondaryScriptFlag(partyEntry, skillCode);
    }
}

/* Clear one skill slot and recompute the owning party record's maxima. */
void mnuClearPartySkillSlot(DatPartyRecord *partyEntry, s32 skillSlot) {
    partyEntry->effectData[skillSlot] = 0;
    ptyRecomputeMaxHpMp(partyEntry);
}

/* Open the selected skill's popup or cancel, then process list navigation.
 * Native list reads precede the late window guard; preserve that ordering. */
void mnuCampMenuHandleInput(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 menuWork = ((CampMenuContext *)context)->menu;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_SKILL_INPUT_MASK);
    s32 window = ((StaffMenuWork *)menuWork)->selectedList;
    s32 list = (s32)((MenuInputNode *)window)->flags;

    ((MenuInputFlags *)list)->bits &= ~MNU_LIST_SELECTION_FLAG;
    if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
        s32 selectedNode = (s32)((MenuInputFlags *)list)->info;
        s32 sortKey = ((MenuInputInfo *)selectedNode)->target;

        if (!(((MenuInputInfo *)selectedNode)->flags & MNU_STAFF_NODE_UNAVAILABLE) && sortKey != 0) {
            mnuSetPopupEntry(context + 0x54, D_0037CC74);
        } else {
            inputFlags = MNU_STAFF_INPUT_REJECTED;
        }
    }
    if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
        mnuSetPopupEntryFlagged(context + 0x54, D_0037CC3C);
    }
    if (window != 0) {
        if (!(inputFlags & MNU_STAFF_INPUT_NAV_STATE_MASK)) {
            func_0027C788(window);
        }
        if (inputFlags & MNU_STAFF_INPUT_PREVIOUS) {
            mnuRetreatWindowListSelection(window);
        }
        if (inputFlags & MNU_STAFF_INPUT_NEXT) {
            mnuAdvanceWindowListSelection(window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, inputFlags, (s32)((MenuInputNode *)window)->flags);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuHandleSelection);

void mnuSwapPartySkillSlots(s32 entry, s32 firstSlot, s32 secondSlot) {
    u8 *slotBase = (u8 *)(entry + 2);
    s32 firstOffset = firstSlot * 2 + 32;
    s32 secondOffset = secondSlot * 2 + 32;
    u16 firstCode = *(u16 *)(slotBase + firstOffset);
    u16 secondCode = *(u16 *)(slotBase + secondOffset);

    *(u16 *)(slotBase + firstOffset) = secondCode;
    *(u16 *)(slotBase + secondOffset) = firstCode;
}


extern u8 D_0037CC90[];
/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
void ptySkillMenuHandleSlotReorder(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuWork *menuWork = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_REORDER_INPUT_MASK);
    MenuSelectionState *window = (MenuSelectionState *)menuWork->selectedList;
    s32 partyEntry = (s32)&datGameState->party[((CampMenuContext *)context)->partyWindow.lists[0]->cursor->index];
    MenuSelectionList *list = window->list;

    list->stateFlags &= ~MNU_LIST_SELECTION_FLAG;
    if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
        s32 selectedSlot = *list->selectedSlot;

        if (mnuHasSelectedListNodeId(callback) == 0) {
            menuWork->selectionId = selectedSlot;
        } else if (selectedSlot != menuWork->selectionId) {
            mnuSwapPartySkillSlots(partyEntry, menuWork->selectionId, selectedSlot);
            mnuClearSelectedListNodeId(callback);
            window = (MenuSelectionState *)ptySkillMenuRebuildAfterMutation(0, callback);
        } else {
            inputFlags = MNU_STAFF_INPUT_REJECTED;
        }
    }
    if (inputFlags & MNU_STAFF_REORDER_CANCEL_MASK) {
        inputFlags = MNU_STAFF_INPUT_CANCEL;
        if (mnuHasSelectedListNodeId(callback) == 0) {
            mnuSetPopupEntryFlagged(context + 0x54, D_0037CC90);
        }
        mnuClearSelectedListNodeId(callback);
    }
    mnuHighlightSelectedListNode(callback);
    if (window != 0) {
        if (!(inputFlags & MNU_STAFF_INPUT_NAV_STATE_MASK)) {
            func_0027C788((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_PREVIOUS) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_NEXT) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag((s32)window);
        mnuPlayInputSound(0, inputFlags, (s32)window->list);
    }
}

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22F0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2310);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2320);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6E0);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6E8);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6F0);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6F8);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC700);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC708);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC710);

