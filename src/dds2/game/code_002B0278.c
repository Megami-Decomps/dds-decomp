#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_list.h"
#include "eff.h"
#include "mnu_shop.h"
#include "dat_state.h"
struct MenuListNode;
struct StaffMenuRuntime;
extern struct MenuListNode *mnuAdvanceListCursorDefault(u32 list);
extern struct MenuListNode *mnuRetreatListCursorDefault(u32 list);
#include "fpu.h"
#include "sdf.h"
#include "itf.h"

extern u16 mnuGetPartyEntryCurrentId(DatPartyRecord *);

#define MNU_ENTRY_SPRITE_COUNT 4
#define MNU_ENTRY_COLOR_COUNT 4
#define MNU_ENTRY_MARKED_COLOR 0x89BDC940
#define MNU_ENTRY_DEFAULT_COLOR 0x89BDC980
#define MNU_ENTRY_ALTERNATE_COLOR 0xBBEFAB80
#define MNU_NODE_FADE_STEP 0x10
#define MNU_FULL_FADE 0x100
#define MNU_PANEL_FADE_LIMIT 0x200
#define MNU_PANEL_FADE_THRESHOLD 0x101
#define MNU_WINDOW_FADE_STEP 0x20
#define MNU_WINDOW_TRANSITION_FLAG 4
#define MNU_WINDOW_TRANSITION_CLEAR_MASK 0xFFFFFFFB
#define MNU_LIST_SELECTION_FLAG 8
#define MNU_LIST_ALTERNATE_SELECTION_FLAG 0x10
#define MNU_WINDOW_CONTAINER_BYTES 0x98
#define MNU_PANEL_LAYOUT_BYTES 0x38
#define MNU_WINDOW_RESOURCE_SPRITES 3
#define MNU_PANEL_KIND_LIMIT 6
#define MNU_SORT_KEY_COUNT 3
#define MNU_SORT_COMPARATOR_COUNT 6
#define MNU_LIST_POINTER_BYTES 4
#define MNU_FADING_SLOT_COUNT 4
#define MNU_FADING_SLOT_BYTES 0x18
#define MNU_FADING_MUTATION_THRESHOLD 5
#define MNU_FADING_WORD_STEP 0x40

#define MNU_STAFF_PARTY_SLOT_COUNT 5
#define MNU_STAFF_PARTY_LAST_SLOT 4
#define MNU_STAFF_DISPLAY_OVERFLOW 4
#define MNU_STAFF_DISPLAY_LIMIT 3
#define MNU_STAFF_BACKUP_BYTES 0x8D4
#define MNU_STAFF_PARTY_ACTIVE_BIT 1
#define MNU_STAFF_NODE_UNAVAILABLE 1
#define MNU_STAFF_NODE_SELECTED 2
#define MNU_STAFF_PARTY_PANEL_BASE 0x284
#define MNU_STAFF_FADE_STEP 0x10
#define MNU_STAFF_FADE_CLOSE_THRESHOLD 0x50
#define MNU_STAFF_FADE_OPEN_THRESHOLD 0xB0
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

typedef struct MenuList MenuList;
typedef struct MenuIconState MenuIconState;

extern u32 uiBlendColors(u32, u32, u32);
extern s32 mnuLookupRangeEntry(u16);
extern u16 mnuGetAdjustedEntryValue(s32, DatPartyRecord *);
extern s32 mnuGetRangeEntryFlatValue(s32);
extern u8 mnuGetRangeEntryKind(u32);
extern s32 func_0035C860(char *, const char *, ...);
extern FrFontGlyph *func_0019F5E8(s32, s32, s32, u32, char *, FrFontGlyph *);
extern void frFontSetChainFlag(FrFontGlyph *, u8);
extern void mnuDrawRepeatedPanelSprites(s32, s32, s32, s32, s32, EffectSlotSet *, s32, s32);
extern char D_00437BF8[];


extern s32 dspStartEntry(s32 entry);
extern s32 D_00435E5C;

extern void itfDrawGridWithResolvedSlot();

extern void mnuClearListFlagsOneAndTwo();

extern void mnuReleaseResourceList(MenuIconState *list);

extern s32 func_002C6CE8(void);

extern s32 func_002C6480();

extern char D_003E75C4[];

extern void mnuDrawWindowDecorations(s32, s32, s32, s32, s32);

extern struct MenuListNode *func_002B86E8(struct MenuList *);

extern void func_002AAE80();

extern void mnuReleasePartyIconBundles();

extern void mnuClearEntries(MenuPageWindow *);

extern FrFontGlyph *itfDrawUnderscoreTextSegment(s32, s32, s32, u32, const u8 *, s32);
extern FrFontGlyph *itfDrawTextWithSelectedFontMode(s32, s32, s32, s8, u16, s32);

extern s32 D_00435E54;

extern u32 effMiscRand();

extern u32 evtStageTestCountFlags();

extern s32 evtStageTestHasPendingMotion();

extern void evtStageTestQueueMotion();

extern s32 D_00435E48;

extern void mnuDrawAndAdvancePanelGroup(s32, s32, s32, DatPartyRecord *, MenuPanelGroup *, s32, s32);

extern void func_002C10F0();

extern void mnuDrawSlotIcons();

extern void frFontAddSharedGlyphFlags(s32);
extern u8 frFontClearFlagBits(u8);

extern char D_003E7588[];

extern char D_00380788[];

extern void evtStageTestUpdate();

extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);


extern char D_003E7790[];

extern char D_003E7720[];

extern char D_003E7774[];

extern char D_003E773C[];

extern void mnuHandlePanelListPageJumpInput();

extern char D_003E7758[];

extern s32 ptySkillMenuApplyFieldUseAndCost();

extern u32 mnuMapPadMaskToFlags();



extern void mnuPlayInputSound(s32, s32, u32 *);

extern u32 D_003E7828[];

extern const CampEffectRows D_003E7858;

extern char *D_003E7818[];

extern char *D_003E7820[];

extern u32 effLoadIndexedResource(char *, char *, s32);

extern u32 effLoadMappedResource(char *, char *);

extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);

extern void effRequestMappedResource(char *, char *, u32 *);

extern void mnuFreeWindowSprites();

extern void mnuClearEntryFlags();

extern s32 evtGetCapturedWindowPanelValue();




extern s32 func_002B06A8();

extern void mnuPrepareStaffValueChangeDialog(MenuStaffContext *, DatPartyRecord *, s32, s32);

extern char D_003E7530[];

extern void func_002AAC70();

extern s32 D_00435E6C;

extern void mnuDrawCampIconBackdropByKind();

extern s32 effHasFirstTextureHandle();

extern void effReleaseTextureHandlesAndResetSlots();

extern void mnuSelectPage();

extern void func_002AAC98();

extern char D_003E69B0[];

extern void mnuCreateStaffImageSprite();

extern void func_002AA7A0();


extern void mnuIdleVoiceTimer(struct StaffMenuRuntime *object);


extern u32 mnuCreateIconBundle(u32);

extern MenuIconState *func_002B9FF8(u32 mode, s32 resource, ...);


extern void mnuDrawIconPanel(s32, s32, s32, s32, MenuIconState *, s32, s32);

extern void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *);

extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);
extern char D_003E75E0[];
extern char D_003E75A8[];

extern void sndSetSequenceVolumePan();

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
    u8 pad00[8];
    MenuPopupState transitionWork; /* +0x08: native saved-entry transition state */
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    s32 displayHandle;     /* 0x60 */
    s32 resourceHandle;    /* 0x64 */
    void *displayResource; /* 0x68 */
    s32 alternateResource; /* 0x6C: used when swapping the staff panel view */
    u8 pad70[0x40];
    s32 selectionResources[6];
    s32 labelHandle;       /* 0xC8 */
    u32 panelModel;       /* 0xCC: model used by the panel resource slots */
    u8 padD0[0x24];
    const void *partySelectionLayout; /* 0xF4: layout copied into the party window panel. */
    const void *skillCategoryLayout; /* 0xF8 */
    const void *equippedSkillLayout; /* 0xFC */
    s32 skillPanelResource; /* 0x100 */
    MenuWindowContainer *imageHandle; /* 0x104: passed to the window fade owner. */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    MenuPageWindow partyWindow; /* 0x284: lists, page slots and selection */
    PartyPanel partyPanel; /* 0xA928: counters and five native 0x34-byte entries */
    MenuPanelGroup *panelGroup; /* 0xAA34 */
    MenuSpriteState *panelRequest; /* 0xAA38 */
    MenuSpriteState *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    u32 *resourceList;     /* 0xAA5C */
    u16 slotOfA[0x2A0]; /* 0xAA60 */
    u16 slotOfB[0x40]; /* 0xAFA0 */
    u16 skillSlots[0x75]; /* 0xB020 */
    u8 padB10A[2];
    MenuFadeFields transition; /* 0xB10C: complete native fade owner. */
    s32 titleFadingOut;
    s32 titleOpacity;
    s32 titleSlide;
    s32 highlightOpacity;
} MenuContext;

extern void mnuReleaseSpriteTextures(s32);

extern u32 kwlnTaskGetUserValue();

extern void mnuDrawWindowContainer(s32, s32, s32, MenuWindowContainer *, s32);

extern void effResolveAndReleaseResource(s32);


/* Menu state handler installer: the call is inlined at each use, so callers
 * return its result through a real call rather than a sibcall. */

typedef struct MenuListNode MenuListNode;



/* One allocated party-selection work area: original/current/backup entries,
 * saved panel payloads, and fade state all belong to this same allocation. */
typedef struct PartyMenuData {
    s32 allocation;
    u8 pad04[4];
    MenuWindowContainer *primaryWindow; /* 0x08 */
    DatPartyRecord original[5];         /* 0x0C */
    DatPartyRecord current[5];          /* 0x8E0 */
    s32 activeCount;                    /* 0x11B4 */
    DatPartyRecord backup[5];           /* 0x11B8 */
    s32 selection;                      /* 0x1A8C */
    MenuPageBar panelSnapshots[5][2]; /* 0x1A90: paired HP/MP snapshots */
    s32 fadeA;                 /* 0x1DB0 */
    s32 fadeB;                 /* 0x1DB4 */
    s32 previousSelection;
    s32 profileSaved;
    s32 profilePhase;
    u32 profileWords[5];
    s32 freezePanel; /* 0x1DD8: set on transition; skips the panel update */
} PartyMenuData; /* 0x1DDC: native party-selection allocation */


/* Native selection-state allocation is 0x30 bytes, distinct from item state. */
typedef struct StaffMenuRuntime {
    SdfMemBlock *allocation;
    u8 pad04[4];
    u32 resources[2];
    s32 staffMode;
    s32 staffView;
    s32 staffSelection;
    s32 staffExit;
    MenuIconState *iconPanel; /* 0x20: created and released with the staff panels. */
    s32 active;
    s32 idleFrames;
    s32 motionSelection;
} StaffMenuRuntime;

/* The item/skill-state constructor allocates and clears this complete 0x3C. */
typedef struct SkillMenuRuntime {
    SdfMemBlock *allocation;
    u8 pad04[4];
    s32 skillMenuActive;
    MenuList *categoryList;
    MenuWindowContainer *skillWindows[4];
    struct MenuPanelState *skillPanel;
    MenuWindowContainer *selectedWindow;
    s32 activeMark;
    u32 unk2C; /* 2B6FE8 clears this after rebuilding selection. */
    u32 unk30; /* 2B5358 clears this after updating the image. */
    u32 selectedIndex;
    s32 fade; /* 0x38: shared skill-category marker fade. */
} SkillMenuRuntime;

extern MenuListNode *sdfAllocAndClearQuadwords(s32);

extern void ptyRecomputeMaxHpMp();

extern void scrClearSecondaryScriptFlag();

extern s32 func_0019D550(FrFontGlyph *, s8, u32);

extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);

extern void func_0035B7F8(MenuListNode **, s32, s32, s32 (*)(MenuListNode **, MenuListNode **));

extern s32 sdfAllocGeneralBlock(s32);

extern u32 sdfResourceRetainAddress(SdfMemBlock *);

extern void func_0026C900(void);


extern void *memset(void *, s32, u32);

MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    u32 resourceHandle, s32 *indices, u32 unused);

typedef struct MenuListDefaults {
    s32 indices[3];
} MenuListDefaults;

extern MenuListDefaults D_0042AF00;
extern const MenuListDefaults D_0042AE18;
extern u8 (*D_00435E64)[17];
extern char D_00437C00[];
extern u8 brsGetLevelStepForValue(s32);
extern s32 mnuGetEntryUseStatus(DatPartyRecord *, u16);
extern void func_002B3CA0(s32, s32, s32, MenuList *, MenuListNode *, s32);

/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
MenuWindowContainer *mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 visibleCount, s32 rowSpacing);
void mnuInitializeBasicWindowLayout(MenuWindowContainer *menu, u32 first, u32 second);
void mnuSetWindowEntryParameters(u32 first, MenuWindowContainer *menu, u32 second, u32 third, u32 fourth);
/* Copy the native panel layout, override its bounds, and mark its transition flag. */
void mnuSetWindowPanelBounds(MenuWindowContainer *panel, const void *layout, u32 left, u32 top,
                   u32 right, u32 bottom);


extern void mnuDrawStaffPartySelectionPanel(s32);

extern const char *D_003E78D0[];
extern s32 D_00435E70;
extern FrFontGlyph *itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, FrFontGlyph *);
extern void func_002AF2E0(s32, s32, s32, MenuContext *);
extern void mnuSetPanelItemsFromRow(MenuPanelGroup *, s32);
extern void mnuClearStaffSceneConfigEntries(MenuPanelGroup *);

void mnuDrawStaffPartySelectionPanel(s32 task) {
    MenuContext *context = (MenuContext *)kwlnTaskGetUserValue(task);
    MenuStaffChoices *choices = (MenuStaffChoices *)context->party;
    DatPartyRecord *unit = &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    s32 selection;
    FrFontGlyph *glyph;

    mnuDrawCampIconBackdropByKind(1, task);
    mnuCreateStaffImageSprite(12);
    mnuApplyPackedGroupValues(context->panelGroup, unit->itemId);
    selection = choices->windows[4]->list->count == 0
                    ? 0 : choices->windows[4]->list->cursor->sortKeySecondary;
    if (selection != 0) {
        mnuSetPanelItemsFromRow(context->panelGroup,
                                choices->windows[4]->list->cursor->sortKeyTertiary);
    }
    mnuDrawAndAdvancePanelGroup(0xEB0, 0x518, 0, unit, context->panelGroup, 2, 0x53);
    if (selection != 0) {
        mnuClearStaffSceneConfigEntries(context->panelGroup);
    }
    mnuUpdateAndDrawWindowTransition(0x1E0, 0x350, 0, &context->transition, 0x53);
    if (selection != 0) {
        if (choices->thirdListReset == 0) {
            if (choices->windows[4]->list->cursor->sortKeyPrimary != 0) {
                func_002AAC70(2, selection, D_00435E70, context, 1, 1, 0x53);
            } else {
                func_002AAC98(2, 0, 0, context, 1, 0x53);
            }
        } else {
            glyph = itfCreateConvertedTextGlyph(0x2B0, 0xA20, 0, 0xA09DC380, (const u8 *)D_003E78D0[0], 0);
            frFontSetChainFlag(glyph, 4);
            func_0019D550(glyph, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(glyph);
            selection = mnuGetPartyEntryCurrentId(unit);
            if (selection != 0) {
                func_002AAC70(3, selection, D_00435E70, context, 1, 1, 0x53);
            } else {
                func_002AAC98(3, 0, 0, context, 1, 0x53);
            }
            func_002AF2E0(0x120, 0xC0, selection, context);
        }
    } else {
        func_002AAC98(2, 0, 0, context, 1, 0x53);
    }
    if (choices->windows[4]->list->cursor != choices->windows[4]->list->first) {
        if (choices->thirdListReset == 0) {
            func_002AA7A0(8, context->displayHandle);
        } else {
            func_002AA7A0(7, context->displayHandle);
        }
    } else {
        func_002AA7A0(1, context->displayHandle);
    }
}

s32 mnuAdvanceStaffValuePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuDrawStaffPartySelectionPanel(callback);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuFinishStaffValuePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

/* Initialize the menu display using a value from the resource chain. */
u32 mnuEnterSelectedResourceLabel(void) {
    s32 resourceOwner;
    s32 context;

    context = kwlnTaskGetUserValue();
    resourceOwner = ((MenuContext *)context)->party;
    func_002C1B68(context + 0xaa50, 1);
    evtCopyEntryStringToActiveWindow(0, D_00435E5C +
                                    *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(resourceOwner + 0x18) + 0x18) + 0x1c) + 100) * 0x19);
    dspStartEntry(8);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0xf);
    return 1;
}

u32 func_002B06A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B06A8);

s32 mnuUpdatePartySlotAssignmentPopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *popup = ((MenuContext *)context)->popupState;
    DatPartyRecord *slot = &datGameState->party[((MenuContext *)context)->partyWindow.lists[0]->cursor->index];
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    s32 state = func_002C4038(&((MenuContext *)context)->transitionWork, popup, 0, (void *)callback);
    s32 label;
    if (state != 0) {
        return state;
    }
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    if (evtGetCapturedWindowPanelValue() == 0) {
        label = ((MenuWindowContainer *)*(s32 *)(menu + 0x18))->list->cursor->sortKeySecondary;
        mnuPrepareStaffValueChangeDialog((MenuStaffContext *)context, slot, label,
                                         func_002B06A8(menu, slot, label));
        mnuInitPartyPanelSlots(&((MenuContext *)context)->partyPanel);
        func_002BCAB0(&((MenuContext *)context)->partyWindow);
        *(s32 *)(menu + 0x40) = label;
    }
    mnuSetPopupEntryFlagged(popup, D_003E7530);
    return 0;
}

s32 mnuStartPanelDispatch(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuDrawStaffPartySelectionPanel(callback);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuStartPanelExit(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

/* Clear the selected item's five stat bonuses and its requirement count,
 * then recalculate the party record's maximum HP and MP. */
void mnuClearPartySelectionValues(DatPartyRecord *entry, s32 selection) {
    s32 i;
    if (selection == 0) {
        return;
    }
    selection -= 0xc0;
    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
        datGameState->itemStatBonuses[selection][i] = 0;
    }
    datGameState->itemRequirementCounts[selection] = 0;
    ptyRecomputeMaxHpMp(entry);
}

u32 mnuEnterSlotLabel(void) {
    s32 context = kwlnTaskGetUserValue();
    DatPartyRecord *slot = &datGameState->party[((MenuContext *)context)->partyWindow.lists[0]->cursor->index];
    s32 selectedEntry;
    func_002C1B68(context + 0xaa50, 1);
    selectedEntry = mnuGetPartyEntryCurrentId(slot);
    evtCopyEntryStringToActiveWindow(0, D_00435E5C + selectedEntry * 0x19);
    dspStartEntry(0xd);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0xf);
    return 1;
}

u32 func_002B0B88(void) {
    return 1;
}

s32 mnuPartySlotConfirmClearUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *popup = ((MenuContext *)context)->popupState;
    DatPartyRecord *slot = &datGameState->party[((MenuContext *)context)->partyWindow.lists[0]->cursor->index];
    s32 state = func_002C4038(&((MenuContext *)context)->transitionWork, popup, 0, (void *)callback);
    s32 selectedEntry;
    if (state != 0) {
        return state;
    }
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    if (evtGetCapturedWindowPanelValue() == 0) {
        selectedEntry = mnuGetPartyEntryCurrentId(slot);
        mnuClearPartySelectionValues(slot, selectedEntry);
        evtCopyEntryStringToActiveWindow(0, D_00435E5C + selectedEntry * 0x19);
        dspStartEntry(0xE);
        mnuInitPartyPanelSlots(&((MenuContext *)context)->partyPanel);
        func_002BCAB0(&((MenuContext *)context)->partyWindow);
    }
    mnuSetPopupEntryFlagged(popup, D_003E7530);
    return 0;
}

s32 mnuAdvancePartyClearPopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuDrawStaffPartySelectionPanel(callback);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuFinishPartyClearPopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 func_002B0D48(void) {
    return 1;
}

void func_002B0D50(u32 context) {
    mnuSwitchCampVisualCategory(4, context);
}

void func_002B0D70(u32 callback) {
}

s32 mnuIsFinalItemIndex(s32 index, s32 item) {
    if (index < (((MenuList *)item)->count - 1)) {
        return 0;
    }
    return 1;
}

extern void func_002B0D90(s32, s32, s32, MenuList *, MenuListNode *, s32);
INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0D90);

extern void func_002B0FA0(MenuContext *);
extern char D_00437BE8[];
void func_002B0FA0(MenuContext *context) {
    PartyMenuData *party = (PartyMenuData *)context->party;
    MenuWindowContainer *window;
    s32 i;
    s32 placement;

    window = mnuCreateWindowContainer(0, 0x1C0, 0x10, 6, 0x16);
    mnuSetWindowContainerState(window, 0x100);
    mnuInitializeBasicWindowLayout(window, context->panelModel, 0xC);
    mnuSetWindowPanelBounds(window, context->partySelectionLayout, 0, 0, 0, 0);
    window->list->context = context;
    window->list->drawCallback = func_002B0D90;

    for (i = 0; i < 5; i++) {
        if ((datGameState->party[i].flags & 1) != 0) {
            s32 id = datGameState->party[i].unitId;
            MenuListNode *node = mnuAppendWindowListNode(window,
                D_00435E48 + id * 17);

            node->sortKeyPrimary = id - 1;
            node->sortKeySecondary = datGameState->party[i].level;
        }
    }
    mnuAppendWindowListNode(window, D_00437BE8);
    window->list->visibleCount = window->list->count;
    switch (window->list->count) {
    case 1:
        placement = 0;
        break;
    case 2:
        placement = 1;
        break;
    case 3:
        placement = 2;
        break;
    case 4:
        placement = 3;
        break;
    case 5:
        placement = 4;
        break;
    case 6:
        placement = 5;
        break;
    default:
        placement = 6;
        break;
    }
    mnuSetWindowEntryParameters(0, window, context->resourceHandle, 0xC, placement);
    party->primaryWindow = window;
}

void mnuDestroyPartySelectionWindow(s32 context) {
    mnuDestroyWindowContainer(((PartyMenuData *)((MenuContext *)context)->party)->primaryWindow);
}


/* Snapshot the five party entries and cap the menu's displayed slot count. */
extern void mnuCopyPartyEntries();
INCLUDE_ASM(const s32, "game/code_002B0278", mnuCopyPartyEntries);

/* Move a current party entry into the selected backup slot and refresh its panel. */
extern void *memcpy(void *, const void *, u32);
extern void func_002C4328(u8 *, s32, u32, PartyPanel *);
extern void mnuRefreshWindowSlots(MenuPageWindow *, s32);

void mnuAssignSelectedPartyEntry(s32 entryIndex, s32 mode, s32 skipRefresh, MenuContext *context) {
    PartyMenuData *menuWork = (PartyMenuData *)context->party;
    s32 activeCount = context->partyPanel.unk0;
    s32 lastSlot = context->partyPanel.unk4;
    s32 i;

    memcpy(&menuWork->backup[menuWork->selection], &menuWork->current[entryIndex],
           sizeof(DatPartyRecord));
    memset(&menuWork->current[entryIndex], 0, sizeof(DatPartyRecord));
    if (mode == 2) {
        s32 selection = menuWork->selection;
        context->partyWindow.slots[selection].flags &= ~0x40;
        menuWork->backup[selection].flags |= 2;
        func_002C4328((u8 *)&menuWork->backup[menuWork->selection], 0, menuWork->selection,
                      &context->partyPanel);
    } else {
        menuWork->backup[menuWork->selection].flags &= ~2;
        func_002C4328((u8 *)&menuWork->backup[menuWork->selection], 0, menuWork->selection,
                      &context->partyPanel);
    }
    context->partyPanel.slots[menuWork->selection].index = entryIndex;
    context->partyPanel.unk0 = activeCount;
    context->partyPanel.unk4 = lastSlot;
    if (skipRefresh == 0) {
        mnuRefreshWindowSlots(&context->partyWindow, 1);
    }
    for (i = context->partyPanel.unk0; i < MNU_STAFF_PARTY_SLOT_COUNT; i++) {
        context->partyWindow.slots[i].flags |= 0x40;
    }
    func_002BCAB0(&context->partyWindow);
    context->partyPanel.unk0++;
    context->partyPanel.unk4--;
    menuWork->selection++;
}


/* Notify active snapshot entries, restore the backup, then refresh panel resources. */
extern void mnuRestorePartyEntriesAndRefresh();
INCLUDE_ASM(const s32, "game/code_002B0278", mnuRestorePartyEntriesAndRefresh);

/* Count active entries in the five-slot party array, capped at three. */
s32 mnuCountActiveSlots(void) {
    s32 activeCount = 0;
    s32 entryCountdown;
    DatPartyRecord *entryCursor = datGameState->party;
    for (entryCountdown = MNU_STAFF_PARTY_LAST_SLOT; entryCountdown >= 0; entryCountdown--) {
        activeCount += entryCursor->flags & MNU_STAFF_PARTY_ACTIVE_BIT;
        entryCursor++;
    }
    if (activeCount > MNU_STAFF_DISPLAY_LIMIT) {
        activeCount = MNU_STAFF_DISPLAY_LIMIT;
    }
    return activeCount;
}

/* Reset selection/backup state, activate all panel slots, and enable list nodes.
 * Keep the native short-arity snapshot call unchanged. */
void mnuClearPartySelectionAndActivateSlots(s32 context) {
    PartyMenuData *menuWork = (PartyMenuData *)((MenuContext *)context)->party;
    s32 entryIndex;
    s32 nodeAddress;

    mnuCopyPartyEntries();
    menuWork->selection = 0;
    memset(menuWork->backup, 0, MNU_STAFF_BACKUP_BYTES);
    ((MenuContext *)context)->partyPanel.unk0 = 1;
    ((MenuContext *)context)->partyPanel.unk4 = mnuCountActiveSlots() - 1;
    func_002BCA98(&((MenuContext *)context)->partyWindow);
    for (entryIndex = 0; entryIndex < MNU_STAFF_PARTY_SLOT_COUNT; entryIndex++) {
        ((MenuContext *)context)->partyWindow.slots[entryIndex].flags |= 0x40;
    }
    for (nodeAddress = (s32)menuWork->primaryWindow->list->first;
         nodeAddress != 0; nodeAddress = (s32)((MenuListNode *)nodeAddress)->next) {
        ((MenuListNode *)nodeAddress)->flags48 &= ~MNU_STAFF_NODE_UNAVAILABLE;
    }
}

/* Release panel textures before reinitializing slots and updating handle state. */
void mnuRefreshPartyPanelSlots(s32 context) {
    mnuReleasePartyPanelTextures(context + MNU_STAFF_PARTY_PANEL_BASE);
    mnuInitPartyPanelSlots(&((MenuContext *)context)->partyPanel);
    func_002BCA98(&((MenuContext *)context)->partyWindow);
}

struct MenuSlotEffectHandles;
typedef struct MenuScrollPanel MenuScrollPanel;
extern void mnuLoadPanelSectionResources(struct MenuSlotEffectHandles *slot, u32 model,
                                         u32 firstValue, u32 secondValue, s32 thirdValue);
extern void mnuConfigurePanelResource(MenuScrollPanel *menu, u32 model, u32 value, u32 color);

s32 func_002B18E8(void) {
    s32 contextAddress = (s32)kwlnTaskGetUserValue();
    MenuContext *context = (MenuContext *)contextAddress;
    s32 allocation = sdfAllocGeneralBlock(sizeof(PartyMenuData));
    PartyMenuData *menuWork =
        (PartyMenuData *)sdfResourceRetainAddress((SdfMemBlock *)allocation);
    s32 slotIndex;

    context->party = (s32)menuWork;
    memset(menuWork, 0, sizeof(*menuWork));
    menuWork->allocation = allocation;
    func_002B0D50((u32)contextAddress);
    func_002B0FA0(context);

    mnuLoadPanelSectionResources(
        (struct MenuSlotEffectHandles *)&context->partyWindow.slots[0],
        context->panelModel, 5, 8, 0xB);
    mnuLoadPanelSectionResources(
        (struct MenuSlotEffectHandles *)&context->partyWindow.slots[1],
        context->panelModel, 5, 9, 0xB);
    mnuLoadPanelSectionResources(
        (struct MenuSlotEffectHandles *)&context->partyWindow.slots[2],
        context->panelModel, 5, 0xA, 0xB);

    mnuClearPartySelectionAndActivateSlots(contextAddress);
    for (slotIndex = 0; slotIndex < 5; slotIndex++) {
        memcpy(&menuWork->original[slotIndex], &datGameState->party[slotIndex],
               sizeof(DatPartyRecord));
        memcpy(menuWork->panelSnapshots[slotIndex],
               &context->partyWindow.slots[slotIndex].hp,
               2 * sizeof(MenuPageBar));
    }

    mnuConfigurePanelResource((MenuScrollPanel *)context->panelHandle,
                              context->panelModel, 0, 0);
    mnuBeginWindowFadeTransition(menuWork->primaryWindow, &context->transition);
    menuWork->fadeA = MNU_FULL_FADE;
    menuWork->fadeB = MNU_FULL_FADE;
    return 1;
}

u32 mnuReleasePartySelectionResources(void) {
    s32 context = kwlnTaskGetUserValue();
    PartyMenuData *selection = (PartyMenuData *)((MenuContext *)context)->party;
    mnuRefreshPartyPanelSlots(context);
    mnuDestroyPartySelectionWindow(context);
    func_002B0D70(context);
    sdfReleaseResourceAllocation(selection->allocation);
    return 1;
}

void mnuPreparePartyPanelTransition(s32 menu) {
    PartyMenuData *party = (PartyMenuData *)((MenuContext *)menu)->party;

    mnuRestorePartyEntriesAndRefresh();
    mnuSetPopupEntryFlagged(((MenuContext *)menu)->popupState, D_003E7588);
    mnuConfigurePanelResource(((MenuContext *)menu)->panelHandle, ((MenuContext *)menu)->displayHandle, 0, 1);
    mnuBeginWindowFadeTransition(((MenuContext *)menu)->imageHandle, &((MenuContext *)menu)->transition);
    party->freezePanel = 1;
}

s32 func_002B1C68(s32 callback) {
    MenuContext *context = (MenuContext *)kwlnTaskGetUserValue();
    PartyMenuData *menuWork = (PartyMenuData *)context->party;
    s32 inputFlags = mnuMapPadMaskToFlags(0x33);
    s32 *popup = context->popupState;
    MenuWindowContainer *window = menuWork->primaryWindow;
    s32 entryIndex = window->list->cursor->index;
    s32 state = func_002C4038(&context->transitionWork, popup, 0, (void *)callback);

    if (state != 0) {
        return state;
    }
    if (*popup == 0) {
        if ((inputFlags & 0x300000) == 0) {
            func_002B9808(window);
        }
        if (inputFlags & 0x10) {
            mnuRetreatWindowListSelection(window);
        }
        if (inputFlags & 0x20) {
            mnuAdvanceWindowListSelection(window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        if (inputFlags & 1) {
            switch (mnuIsFinalItemIndex(window->list->cursor->index, (s32)window->list)) {
            case 0:
                if ((u16)(menuWork->current[entryIndex].flags & 1) != 0) {
                    window->list->cursor->flags48 |= 1;
                    mnuAssignSelectedPartyEntry(entryIndex, 2, 0, context);
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
                func_002BCAB0(&context->partyWindow);
            } else {
                mnuSetPopupEntryFlagged(popup, D_003E7588);
                mnuConfigurePanelResource(context->panelHandle, context->displayHandle, 0, 1);
                mnuBeginWindowFadeTransition(context->imageHandle, &context->transition);
            }
        }
        mnuPlayInputSound(0, inputFlags, &window->list->stateFlags);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1EA8);

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


extern s32 mnuGetSelectionFromFlags(s32);
extern MenuProfilePanel *mnuCreateProfilePanel(DatPartyRecord *selectionState);
extern void mnuSetGroupProperties(u32 *, u32, u32, u32, u32);
extern void mnuDrawAndAdvanceProfilePanel(s32, s32, s32, MenuProfilePanel *, s32);
extern void mnuFreeProfilePanelWork(void *);

extern void func_002B2408(MenuContext *);
INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2408);

/* Update the final-row flag, draw the panel, and suppress its contents update
 * once the party transition has frozen it. */
s32 mnuOpenStaffPartySelectionPanel(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    PartyMenuData *menu = (PartyMenuData *)((MenuContext *)context)->party;
    MenuList *list;
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(0x17);
    list = menu->primaryWindow->list;
    if (mnuIsFinalItemIndex(list->cursor->index, (s32)list)) {
        menu->primaryWindow->list->stateFlags |= 0x10;
    } else {
        menu->primaryWindow->list->stateFlags &= ~0x10;
    }
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    if (menu->freezePanel == 0) {
        func_002B2408((MenuContext *)context);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuStepPartySelectionControl(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u8 mnuIsStateNotOne(void) {
    s64 state;

    state = func_002C6CE8();
    return state != 1;
}

void func_002B27F0(u32 context) {
    mnuSwitchCampVisualCategory(3, context);
}

void mnuReleaseMenuWindowHandles(s32 context) {
}

/* Release both staff resource slots; their menu indices differ between games. */
void mnuReleaseStaffMenuResources(s32 menuWork) {
    u32 *resourceCursor = (u32 *)(menuWork + 8);
    s32 resourceCountdown = 1;
    do {
        effResolveAndReleaseResource(*resourceCursor++);
    } while (--resourceCountdown >= 0);
}

/* Reset texture handles for the same two resource slots. */
void mnuReleaseStaffMenuTextureHandles(s32 menuWork) {
    u32 *resourceCursor = (u32 *)(menuWork + 8);
    s32 resourceCountdown = 1;
    do {
        effReleaseTextureHandlesAndResetSlots(*resourceCursor++);
    } while (--resourceCountdown >= 0);
}

u32 mnuCreateSelectState(u32 unused, s32 flag) {
    s32 context = kwlnTaskGetUserValue();
    u32 handle = sdfAllocGeneralBlock(0x30);
    StaffMenuRuntime *state = (StaffMenuRuntime *)sdfResourceRetainAddress((SdfMemBlock *)handle);
    *(StaffMenuRuntime **)(context + 0xaa48) = state;
    memset(state, 0, 0x30);
    state->allocation = (SdfMemBlock *)handle;
    state->staffView = flag;
    if (flag == 0) {
        state->staffMode = 0;
    } else {
        state->staffMode = 1;
    }
    func_002B27F0(context);
    state->active = 1;
    mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayResource, 0, 0);
    evtStageTestInit(0);
    return 1;
}

s32 mnuStaffCloseSelectionState(void) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menuWork = (StaffMenuRuntime *)((MenuContext *)context)->party;

    mnuResetWorkFloats();
    mnuReleaseMenuWindowHandles(context);
    sdfReleaseResourceAllocation((s32)menuWork->allocation);
    return 1;
}

void func_002B29C8(u32 context) {
    mnuCreateSelectState(context, 1);
}

void func_002B29E0(void) {
    mnuStaffCloseSelectionState();
}

void func_002B29F8(u32 context) {
    mnuCreateSelectState(context, 0);
}

void func_002B2A10(void) {
    mnuStaffCloseSelectionState();
}

/* Update the popup first; accept confirm/cancel only while its state is zero.
 * Return a nonzero popup-update word unchanged, or zero after handling input. */
s32 mnuStaffPopupUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menuWork = (StaffMenuRuntime *)((MenuContext *)context)->party;
    s32 *popupState = ((MenuContext *)context)->popupState;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_POPUP_INPUT_MASK);
    s32 stateWord;
    MenuPageWindow *panelWork;
    stateWord = func_002C4038(&((MenuContext *)context)->transitionWork, popupState, 0, (void *)callback);
    if (stateWord != 0) {
        return stateWord;
    }
    if (*popupState == 0) {
        panelWork = &((MenuContext *)context)->partyWindow;
        mnuStepPartyPanelListFromInput(4, panelWork);
        if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
            menuWork->staffSelection = ((MenuContext *)context)->partyWindow.lists[0]->cursor->index;
            mnuSetPopupEntry(popupState, D_003E75E0);
            menuWork->active = 1;
        }
        if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
            mnuSetPopupEntryFlagged(popupState, D_003E75A8);
            mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayHandle, 0, 1);
            mnuClearActionFlags(0, panelWork);
        }
        mnuPlayInputSound(0, inputFlags, 0);
    }
    return 0;
}

s32 mnuDrawStaffCampPageWithImage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menu = (StaffMenuRuntime *)((MenuContext *)context)->party;
    if (effHasFirstTextureHandle(((MenuContext *)context)->resourceHandle)) {
        mnuDrawCampIconBackdropByKind(0, callback);
    } else {
        mnuDrawCampIconBackdropByKind(1, callback);
    }
    if (menu->staffView == 0) {
        mnuCreateStaffImageSprite(0x16);
    } else {
        mnuCreateStaffImageSprite(0x15);
    }
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    if (menu->active != 0) {
        func_002AAC98(0, ((MenuContext *)context)->imageHandle->list->cursor->index, D_003E69B0, context, 1, 0x53);
    }
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuStepStaffCampPageControl(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2C88);

extern void mnuSetWindowResource(s32 index, MenuPageWindow *menu, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
extern void mnuAttachPartyIconBundle(s32 index, s32 menu, u32 resource);
extern s32 mnuClassifyQuarterHalfPercent(s32 amount, s32 divisor);
extern void evtStageTestSelectEntry(s32, s32, s32);
extern void func_002B2C88(s32, s32, s32, s32);

s32 mnuCreatePanels(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuContext *menuContext = (MenuContext *)context;
    MenuList *list = menuContext->partyWindow.lists[0];
    s32 index = list->cursor->index;
    DatPartyRecord *data = &datGameState->party[index];
    MenuPageWindow *window = &((MenuContext *)context)->partyWindow;
    StaffMenuRuntime *party = (StaffMenuRuntime *)menuContext->party;
    MenuProfilePanel *profile;

    mnuSetWindowResource(index, window, menuContext->displayHandle,
                         (s32)menuContext->displayResource,
                         menuContext->alternateResource, 0, 0);
    mnuAttachPartyIconBundle(index, window, (u32)menuContext->displayResource);
    menuContext->panelGroup = mnuCreatePanelGroup(menuContext->resourceHandle,
                                                   (s32)menuContext->displayResource, 0);
    menuContext->panelRequest = mnuCreateSpriteState((struct EffectSlotSet *)menuContext->resourceHandle,
                                                    (struct EffectSlotSet *)menuContext->displayResource,
                                                    (struct EffectSlotSet *)menuContext->displayHandle);
    menuContext->panelEffects = mnuAllocateSimpleSprite(
        (struct EffectSlotSet *)menuContext->resourceHandle,
        (struct EffectSlotSet *)menuContext->alternateResource,
        (struct EffectSlotSet *)menuContext->displayHandle);
    profile = mnuCreateProfilePanel(data);
    menuContext->resourceList = (u32 *)profile;
    mnuSetGroupProperties((u32 *)profile, menuContext->displayHandle,
                          menuContext->alternateResource, 1, 2);
    party->iconPanel = func_002B9FF8(4, menuContext->displayHandle, menuContext->skillPanelResource);
    if (mnuClassifyQuarterHalfPercent(data->hp, data->maxHp) < 2) {
        party->motionSelection = -1;
    } else {
        party->motionSelection = 0xA;
    }
    evtStageTestSelectEntry(data->unitId, party->motionSelection, 0);
    func_002B2C88(window, 1, party->staffView, party->staffMode);
    mnuBeginWindowFadeTransition(0, &((MenuContext *)context)->transition);
    return 1;
}

s32 mnuDestroyPanels(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menu = (StaffMenuRuntime *)((MenuContext *)context)->party;
    s32 window;
    mnuBeginWindowFadeTransition(((MenuContext *)context)->imageHandle, &((MenuContext *)context)->transition);
    window = context + 0x284;
    func_002B2C88(window, 0, menu->staffView, menu->staffMode);
    evtStageTestStop();
    mnuClearEntries((MenuPageWindow *)window);
    mnuReleasePartyIconBundles(window);
    if (((MenuContext *)context)->panelGroup != 0) {
        mnuDestroyPanelGroup(((MenuContext *)context)->panelGroup);
        ((MenuContext *)context)->panelGroup = 0;
    }
    if (((MenuContext *)context)->panelRequest != 0) {
        mnuFreeSpriteStateWork(((MenuContext *)context)->panelRequest);
        ((MenuContext *)context)->panelRequest = 0;
    }
    if (((MenuContext *)context)->panelEffects != 0) {
        mnuFreeSimpleSpriteWork(((MenuContext *)context)->panelEffects);
        ((MenuContext *)context)->panelEffects = 0;
    }
    if (((MenuContext *)context)->resourceList != 0) {
        mnuFreeProfilePanelWork(((MenuContext *)context)->resourceList);
        ((MenuContext *)context)->resourceList = 0;
    }
    mnuReleaseResourceList(menu->iconPanel);
    return 1;
}

void mnuResetSelectedPanelOpacity(s32 context) {
    ((MenuContext *)context)->partyWindow.slots[((MenuContext *)context)->partyWindow.lists[0]->cursor->index].windowSprites->fade = 0x100;
}

/* Switch the party page, rebuilding its panels; previous takes priority.
 * The opaque request argument is forwarded unchanged. Return one if switched. */
s32 mnuStaffSwitchPartyPage(s32 requestArgument) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menuWork = (StaffMenuRuntime *)((MenuContext *)context)->party;
    s32 pageChanged = 0;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_PAGE_INPUT_MASK);
    if (inputFlags & MNU_STAFF_INPUT_PREV_PAGE) {
        mnuDestroyPanels(requestArgument);
        mnuRetreatListCursorDefault(((MenuContext *)context)->partyWindow.lists[0]);
        pageChanged = 1;
    }
    if ((inputFlags & MNU_STAFF_INPUT_NEXT_PAGE) && pageChanged == 0) {
        mnuDestroyPanels(requestArgument);
        mnuAdvanceListCursorDefault(((MenuContext *)context)->partyWindow.lists[0]);
        pageChanged = 1;
    }
    mnuClearListFlagsOneAndTwo(((MenuContext *)context)->partyWindow.lists[0]);
    if (pageChanged != 0) {
        mnuCreatePanels(requestArgument);
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        menuWork->idleFrames = 0;
        if (menuWork->staffMode == 1) {
            mnuResetSelectedPanelOpacity(context);
        }
        return 1;
    }
    return 0;
}

/* Update popup state before page navigation, view toggles, and exit requests.
 * stateWord holds the update result first, then the stored popup state. */
s32 mnuStaffBrowsePartyUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menuWork = (StaffMenuRuntime *)((MenuContext *)context)->party;
    s32 *popupState;
    u32 inputFlags;
    s32 stateWord;
    if (menuWork->staffView == 0) {
        inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_VIEW_INPUT_MASK);
    } else {
        inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_INPUT_CANCEL);
    }
    popupState = ((MenuContext *)context)->popupState;
    stateWord = func_002C4038(&((MenuContext *)context)->transitionWork, popupState, 0, (void *)callback);
    if (stateWord != 0) {
        return stateWord;
    }
    if (func_002C6480() != 0) {
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
                func_002B2C88(context + MNU_STAFF_PARTY_PANEL_BASE, 3, menuWork->staffView, 1);
                mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->alternateResource, 0, 0);
            } else {
                menuWork->staffMode = 0;
                func_002B2C88(context + MNU_STAFF_PARTY_PANEL_BASE, 2, menuWork->staffView, 0);
                mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayResource, 0, 0);
            }
            menuWork->idleFrames = 0;
        }
        if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
            if (func_002C6CE8() != 1) {
                evtStageTestStop();
                menuWork->staffExit = 1;
                menuWork->active = 1;
                mnuSetPopupEntryFlagged(popupState, D_003E75C4);
            } else {
                inputFlags = MNU_STAFF_INPUT_REJECTED;
            }
        }
        mnuPlayInputSound(0, inputFlags, 0);
    }
    return 0;
}

void mnuDrawSlotIcons(s32 x, s32 context) {
    DatPartyRecord *slot = &datGameState->party[((MenuPageWindow *)context)->lists[0]->cursor->index];
    s32 i;
    s32 y = 0xb40;
    FrFontGlyph *handle;
    for (i = 0; i < 2; i++, y += 0xb8) {
        handle = itfDrawUnderscoreTextSegment(0x3c0, y, 0, 0xa09dc359, (const u8 *)(D_00435E54 + slot->unitId * 45), i);
        if (handle != 0) {
            func_0019D550(handle, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(handle);
        }
    }
}

void mnuDrawSelectedPartySlotMarkers(s32 context, StaffSlots *resources) {
    s32 alpha;

    alpha = 0x100 - ((MenuPageWindow *)context)->slots[((MenuPageWindow *)context)->lists[0]->cursor->index].windowSprites->fade;
    func_00306CD0(0xa0, 0xa30, 0, alpha, 1, (EffectSlotSet *)resources->baseResources[1], 0x55, 0x53);
    func_00306CD0(0x30, 0xaf8, 0, alpha, 1, (EffectSlotSet *)resources->baseResources[0], 0x1a, 0x53);
}

void mnuDrawTextSprite(s32 x, s32 y, s32 width, u32 color, s32 model, s32 flags) {
    s32 top = y - 0x10;
    FrFontGlyph *handle;
    frFontAddSharedGlyphFlags(1);
    handle = frFontAppendGlyphFromData((void *)model, 0, 0, 0, 0);
    frFontSetContextPair(handle, x, top);
    frFontStoreShiftedContextValue(handle, width << 4);
    frFontSetChildColors(handle, color);
    frFontClearFlagBits(1);
    func_0019D550(handle, 1, flags);
    frFontQueueGlyphInSelectedSlot(handle);
}

void mnuDrawPartySkillAndStatusPanel(DatPartyRecord *entry, s32 id, MenuPanelGroup *packedGroup, s32 group, s32 unused, s32 spriteFlags) {
    mnuApplyPackedGroupValues(packedGroup, entry->itemId);
    mnuDrawAndAdvancePanelGroup(0xeb0, 0x518, 0, entry, packedGroup, 0, spriteFlags);
    func_002C10F0(0, 0, 0, entry, group, spriteFlags);
    mnuDrawTextSprite(0x2a0, 0xa50, 0, 0xa09dc380, D_00435E48 + entry->unitId * 0x11 + 0x110, spriteFlags);
    mnuDrawSlotIcons(0x14a, id);
}

void mnuDrawProfilePanelAndSprite(DatPartyRecord *entry, u32 unused1, MenuSpriteState *spriteState,
                                    MenuProfilePanel *resource,
                                    u32 unused4, u32 spriteFlags) {
    func_002C16F0(0, 0, 0, entry, entry->profileId, (s32)spriteState, spriteFlags);
    mnuDrawAndAdvanceProfilePanel(0xe80, 0x5b8, 0, resource, spriteFlags);
}

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg);

s32 func_002B3788(s32 callback) {
    s32 contextAddress = (s32)kwlnTaskGetUserValue();
    MenuContext *context = (MenuContext *)contextAddress;
    DatGameState *gameState = datGameState;
    StaffMenuRuntime *menuWork = (StaffMenuRuntime *)context->party;
    s32 partyIndex = context->partyWindow.lists[0]->cursor->index;
    DatPartyRecord *partyEntry = &gameState->party[partyIndex];

    mnuDrawCampIconBackdropByKind(2, callback);
    mnuDrawSelectedPartySlotMarkers((s32)&context->partyWindow,
                                    (StaffSlots *)&context->displayHandle);

    if (menuWork->staffMode == 0) {
        context->partyWindow.flags = (context->partyWindow.flags | 0x200) & ~0x80;
    } else {
        context->partyWindow.flags |= 0x280;
    }

    if (menuWork->staffMode == 0) {
        mnuDrawPartySkillAndStatusPanel(partyEntry, (s32)&context->partyWindow,
                                        context->panelGroup, (s32)context->panelRequest,
                                        (s32)&context->displayHandle, 0x53);
        func_002AA7A0(5, context->displayHandle);
    } else {
        mnuDrawProfilePanelAndSprite(partyEntry, (u32)&context->partyWindow,
                                     context->panelEffects, (MenuProfilePanel *)context->resourceList,
                                     (u32)&context->displayHandle, 0x53);
        if (menuWork->staffView == 0) {
            func_002AA7A0(6, context->displayHandle);
        } else {
            func_002AA7A0(4, context->displayHandle);
        }
    }

    if (menuWork->staffView == 0) {
        mnuDrawIconPanelFullFade(0, 0, 0, menuWork->iconPanel, 0x53);
        mnuUpdateWindowPanelHandleStatesKindFourFive(menuWork->iconPanel);
    }
    evtStageTestUpdate(D_00380788);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

void mnuIdleVoiceTimer(StaffMenuRuntime *object) {
    u32 count;
    if (object->motionSelection == -1) {
        if (func_002C6CE8() != 1) {
            if (evtStageTestHasPendingMotion() == 0) {
                object->idleFrames += 1;
            }
            if (object->idleFrames >= 0x12d) {
                count = evtStageTestCountFlags(0);
                evtStageTestQueueMotion(0, effMiscRand(0) % count);
                object->idleFrames = 0;
            }
        }
    }
}

s32 mnuStaffIdlePartyUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    StaffMenuRuntime *menu = (StaffMenuRuntime *)((MenuContext *)context)->party;
    if (menu->staffMode == 0) {
        mnuIdleVoiceTimer(menu);
    }
    return menuSetHandler((void *)context, 2, (void *)callback);
}

u32 func_002B3A58(void) {
    return 1;
}

void mnuDrawRangeCostAndIcon(s32 x, s32 y, s32 depth, s32 xOffset, u32 fade,
                   DatPartyRecord *actor, u16 rangeId, s32 style, s32 dim,
                   EffectSlotSet *costResource, u32 texture) {
    char text[16];
    u32 color;
    s32 value;
    FrFontGlyph *glyph;
    u8 chainFlag;
    s32 kind;
    s32 hpOffset = 0;

    color = uiBlendColors(0xA09DC380, 0xA09DC300, fade);
    if (rangeId >= 0x2A1) {
        return;
    }
    if (mnuLookupRangeEntry(rangeId) == 3) {
        mnuDrawRepeatedPanelSprites(x, y, depth, 0x100, 3, costResource, 0x1B, texture);
        return;
    }
    if (actor != 0) {
        value = mnuGetAdjustedEntryValue(rangeId, actor);
    } else {
        value = mnuGetRangeEntryFlatValue(rangeId);
    }
    kind = mnuGetRangeEntryKind(rangeId);
    if (actor == 0) {
        hpOffset = -0xD0;
        if (kind != 1) {
            hpOffset = 0;
        }
    }
    if (value != 0) {
        chainFlag = style != 0 ? 4 : 0;
        if (dim != 0) {
            color = uiBlendColors(color, color & 0xFFFFFF00, 0x80);
        }
        func_0035C860(text, D_00437BF8, value);
        glyph = func_0019F5E8(x + hpOffset, y - 8, depth, color, text, 0);
        frFontSetChainFlag(glyph, chainFlag);
        func_0019D550(glyph, 1, texture);
        frFontQueueGlyphInSelectedSlot(glyph);
    } else {
        mnuDrawRepeatedPanelSprites(x, y, depth, 0x100, 3, costResource, 0x1B, texture);
        return;
    }
    x += xOffset;
    switch (kind) {
    case 1:
    default:
        func_00306CD0(x + hpOffset, y, depth, fade, 1, costResource,
                      actor != 0 ? 0x18 : 0x1A, texture);
        break;
    case 2:
        func_00306CD0(x, y, depth, fade, 1, costResource, 0x19, texture);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3CA0);

s32 ptySkillMenuBuildEquippedSlots(s32 selectionMode, s32 callback) {
    MenuContext *context = (MenuContext *)kwlnTaskGetUserValue(callback);
    MenuListDefaults defaults = D_0042AE18;
    s32 selected = context->partyWindow.lists[0]->cursor->index;
    SkillMenuRuntime *party = (SkillMenuRuntime *)context->party;
    DatPartyRecord *entry = &datGameState->party[selected];
    s32 skillCount = brsGetLevelStepForValue(entry->level);
    s32 placement;
    MenuWindowContainer *window;
    s32 i;

    switch (skillCount) {
    case 4:
        placement = defaults.indices[0];
        break;
    case 6:
        placement = defaults.indices[1];
        break;
    default:
        placement = defaults.indices[2];
        break;
    }
    window = mnuCreateWindowContainer(0, 0x1C0, 0x10, skillCount, 0x16);
    mnuSetWindowContainerState(window, MNU_FULL_FADE);
    mnuInitializeBasicWindowLayout(window, context->labelHandle, 0x1A);
    mnuSetWindowPanelBounds(window, context->equippedSkillLayout, 0, 0, 0, 0);
    mnuSetWindowEntryParameters(0, window, context->resourceHandle, 0xD, placement);
    window->list->context = context;
    window->list->drawCallback = func_002B3CA0;
    for (i = 0; i < skillCount; i++) {
        u16 skill = datGameState->party[selected].effectData[i];
        MenuListNode *node;

        if (skill != 0) {
            node = mnuAppendWindowListNode(window, D_00435E64[skill]);
            node->sortKeySecondary = i;
            node->sortKeyPrimary = skill;
            if (selectionMode != 0 && mnuGetEntryUseStatus(entry, skill) != 0) {
                node->flags48 |= MNU_STAFF_NODE_UNAVAILABLE;
            }
        } else {
            node = mnuAppendWindowListNode(window, D_00437C00);
            node->sortKeySecondary = node->sortKeyPrimary = 0;
        }
    }
    party->selectedWindow = window;
    return 1;
}

u32 mnuDestroySelectedPartyWindow(u32 callback) {
    s32 party;

    party = kwlnTaskGetUserValue();
    party = ((MenuContext *)party)->party;
    mnuDestroyWindowContainer(((SkillMenuRuntime *)party)->selectedWindow);
    ((SkillMenuRuntime *)party)->selectedWindow = 0;
    return 1;
}

/* Seek the first node with a nonzero sort key and no unavailable flag.
 * Leave the cursor unchanged if no such node exists. */
void mnuSeekFirstAvailableStaffListNode(void) {
    SkillMenuRuntime *menuWork = (SkillMenuRuntime *)((MenuContext *)kwlnTaskGetUserValue())->party;
    MenuListNode *listNode = menuWork->selectedWindow->list->first;

    while (listNode != NULL) {
        u32 sortKey = listNode->sortKeyPrimary;
        if (!(listNode->flags48 & MNU_STAFF_NODE_UNAVAILABLE) && sortKey != 0) {
            break;
        }
        listNode = listNode->next;
    }
    if (listNode != NULL) {
        mnuSeekListNode(listNode->index, menuWork->selectedWindow->list);
    }
}

MenuWindowContainer *mnuSeekSelectedWindowCursor(s32 selectionMode, s32 callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    SkillMenuRuntime *party = (SkillMenuRuntime *)((MenuContext *)context)->party;
    MenuFadeFields *window = &((MenuContext *)context)->transition;
    s32 id;
    MenuWindowContainer *selected;
    id = party->selectedWindow->list->cursor->index;
    mnuDestroySelectedPartyWindow(callback);
    ptySkillMenuBuildEquippedSlots(selectionMode, callback);
    mnuSeekListNode(id, party->selectedWindow->list);
    selected = party->selectedWindow;
    selected->scale50 = 0x200;
    selected->scale54 = 0x100;
    mnuBeginWindowFadeTransition(0, window);
    mnuBeginWindowFadeTransition(party->selectedWindow, window);
    party->selectedWindow->panel.fade = 0x100;
    ((MenuContext *)context)->transition.currentProgress = 0x200;
    return party->selectedWindow;
}

void func_002B4270(u32 context) {
    mnuSwitchCampVisualCategory(1, context);
}

void func_002B4290(s32 context) {
}

extern void mnuDrawCampGridResourceSlot(s32, u32, u32, s32, u32, u32);
extern void mnuDispatchEntryWords(EffectSlotSet *, s32, MenuListNode *);
extern void itfGridCopyEntryQuad(s32, s32);
typedef struct MnuCategoryPositions {
    s32 entries[4][2];
} MnuCategoryPositions;
extern const MnuCategoryPositions D_0042AE28;
extern u32 scrGetSecondaryScriptFlag(DatPartyRecord *, u16);

void func_002B4298(s32 x, s32 y, s32 depth, MenuList *list,
                   MenuListNode *node, s32 texture) {
    MnuCategoryPositions categoryPositions = D_0042AE28;
    MenuContext *context = (MenuContext *)list->context;
    SkillMenuRuntime *runtime = (SkillMenuRuntime *)context->party;
    s32 category;
    s32 selected;
    s32 fade;
    s32 opacity;
    s32 markerX;
    s32 markerY;
    u32 skill;
    DatPartyRecord *actor;

    mnuDrawCampGridResourceSlot(x, y, depth, (s32)list, (u32)node, texture);
    selected = list->cursor == node;
    actor = &datGameState->party[
        context->partyWindow.lists[0]->cursor->index];
    if (node == list->head && list->categoryMarkerEnabled != 0) {
        category = list->categoryIndex;
        markerX = categoryPositions.entries[category][0];
        markerY = categoryPositions.entries[category][1];
        fade = runtime->fade;
        if (fade < 0x100) {
            opacity = fade;
        } else {
            opacity = 0x200 - fade;
        }
        func_00306CD0(markerX, markerY, depth, opacity, 1,
                      (EffectSlotSet *)context->resourceHandle, 0x1E, texture);
    }

    if (node->index == 0) {
        if (selected != 0) {
            s32 edgeY = y - 8;
            itfDrawGridWithResolvedSlot(x + 0x2A0, y, depth, 1,
                (u32)context->labelHandle, 0x23, texture);
            itfDrawGridWithResolvedSlot(x - 0x30, edgeY, depth, 1,
                (u32)context->resourceHandle, 0x1F, texture);
            itfDrawGridWithResolvedSlot(x + 0xB10, edgeY, depth, 1,
                (u32)context->resourceHandle, 0x1F, texture);
        } else {
            itfDrawGridWithResolvedSlot(x + 0x60, y, depth, 1,
                (u32)context->labelHandle, 0x1D, texture);
        }
        return;
    }

    skill = node->sortKeyPrimary;
    if ((u32)(skill - 1) <= 0xFFFD &&
        scrGetSecondaryScriptFlag(actor, (u16)skill) != 0) {
        s32 skillFade = runtime->fade;
        if (skillFade < 0x100) {
            opacity = skillFade;
        } else {
            opacity = 0x200 - skillFade;
        }
        func_00306CD0(x + 0x60, y + 0x10, depth, opacity, 1,
                      (EffectSlotSet *)context->resourceHandle, 0x1E, texture);
    }

    if (skill != 0 && skill != 0xFFFF) {
        mnuDrawRangeCostAndIcon(x + 0x970, y + 0x30, depth, 0x1F0,
            list->scale, actor,
            (u16)skill, selected, node->flags48 & 1,
            (EffectSlotSet *)context->resourceHandle,
            texture);
        return;
    }

    {
        EffectSlotSet *resource = (EffectSlotSet *)context->resourceHandle;
        s32 entryIndex = selected + 0x1B;
        markerX = x + 0x1C0;
        markerY = y + 0x40;
        mnuDispatchEntryWords(resource, entryIndex, node);
        mnuDrawRepeatedPanelSprites(markerX, markerY, depth, 0x100, 8,
                                    resource, entryIndex, texture);
        itfGridCopyEntryQuad((s32)resource, entryIndex);
    }
}

extern s32 ptyHasSkill(DatPartyRecord *, s32);
extern u32 scrGetSecondaryScriptFlag(DatPartyRecord *, u16);

/* Secondary script bits enable the category marker independently of learned skills. */
void func_002B45D8(MenuContext *context) {
    MenuWindowContainer **windows = ((SkillMenuRuntime *)context->party)->skillWindows;
    DatPartyRecord *owner = &datGameState->party[context->partyWindow.lists[0]->cursor->index];
    s32 windowIndex;

    for (windowIndex = 0; windowIndex < 4; windowIndex++) {
        MenuWindowContainer *window = windows[windowIndex];
        MenuListNode *node = window->list->first;
        s32 hasSecondary = 0;

        while (node != NULL) {
            u32 id = node->sortKeyPrimary;
            if (id != 0 && id != 0xFFFF) {
                u16 code = id;
                if (ptyHasSkill(owner, code)) {
                    node->flags48 |= 1;
                } else {
                    node->flags48 &= ~1;
                }
                if (scrGetSecondaryScriptFlag(owner, code)) {
                    hasSecondary = 1;
                }
            }
            node = node->next;
        }
        if (hasSecondary != 0) {
            window->list->categoryMarkerEnabled = 1;
        } else {
            window->list->categoryMarkerEnabled = 0;
        }
    }
}

typedef struct SkillInfo {
    u8 pad00[0x20];
    u32 count;
    u16 codes[14];
} SkillInfo;

extern void func_00315388(u16, SkillInfo *);

u32 *mnuBuildOwnedSkillBits(void) {
    SkillInfo info;
    u32 *bits = (u32 *)sdfAllocSizeClassBlock(0x58);
    s32 i;
    u32 j;
    memset(bits, 0, 0x58);
    for (i = 0; i < 0xb0; i++) {
        func_00315388(i, &info);
        for (j = 0; j < info.count; j++) {
            u16 id = info.codes[j];
            if (id != 0) {
                bits[id >> 5] |= 1 << id;
            }
        }
    }
    return bits;
}

void func_002B47F8(u32 *bits) {
    sdfReleaseChipBlock(bits);
}

s32 mnuIsSkillCodeInBitset(s32 skillCode, u32 *bits) {
    s32 wordIndex = (skillCode < 0) ? skillCode + 0x1f : skillCode;

    return (bits[wordIndex >> 5] & (1 << skillCode)) != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4848);

void mnuDestroySkillMenuWindows(s32 context) {
    SkillMenuRuntime *menu = (SkillMenuRuntime *)((MenuContext *)context)->party;
    if (menu->skillMenuActive != 0) {
        u32 i = 0;
        MenuWindowContainer **resource = menu->skillWindows;
        mnuDestroyPanelState(menu->skillPanel);
        mnuDestroyListState(menu->categoryList);
        do {
            mnuDestroyWindowContainer(*resource++);
            i++;
        } while (i < 4);
        menu->skillMenuActive = 0;
    }
}

u32 mnuCreateItemState(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u32 handle = sdfAllocGeneralBlock(0x3c);
    u32 *state = (u32 *)sdfResourceRetainAddress((SdfMemBlock *)handle);
    *(u32 **)(context + 0xaa48) = state;
    memset(state, 0, 0x3c);
    state[0] = handle;
    func_002B4270(context);
    switch (((MenuContext *)context)->imageHandle->list->cursor->index) {
    case 0:
        mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->labelHandle, 0, 0);
        break;
    case 2:
        mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->labelHandle, 0x19, 0);
        break;
    case 3:
        mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->labelHandle, 0xa, 0);
        break;
    }
    mnuSeekListNode(0, ((MenuWindowContainer *)((MenuContext *)context)->listHandle)->list);
    return 1;
}

s32 mnuCloseItemSelectionState(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    if (menu[9] != 0) {
        mnuDestroySelectedPartyWindow(selection);
    }
    func_002B4290(context);
    sdfReleaseResourceAllocation(menu[0]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4E58);

s32 mnuCampMenuDrawSlotLabel(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    if (((MenuContext *)context)->imageHandle->list->cursor->index == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xe);
    }
    func_002AAC98(0, ((MenuContext *)context)->imageHandle->list->cursor->index, D_003E69B0, context, 1, 0x53);
    /* Both arms are identical in retail; kept as written. */
    if (((MenuContext *)context)->imageHandle->list->cursor->index == 0) {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    } else {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 mnuStepSkillSlotControl(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

void mnuClearSelectedListNodeId() {
    s32 context;

    context = kwlnTaskGetUserValue();
    ((SkillMenuRuntime *)((MenuContext *)context)->party)->selectedIndex = 0xffffffff;
}

u32 mnuHasSelectedListNodeId(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return ~((SkillMenuRuntime *)((MenuContext *)context)->party)->selectedIndex >> 0x1f;
}

/* Set the selected flag only on nodes whose index matches the saved selection. */
void mnuHighlightSelectedListNode() {
    u8 *menuWork = (u8 *)((MenuContext *)kwlnTaskGetUserValue())->party;
    u8 *listNode = (u8 *)((SkillMenuRuntime *)menuWork)->selectedWindow->list->first;
    while (listNode != NULL) {
        if (((MenuListNode *)listNode)->index == ((SkillMenuRuntime *)menuWork)->selectedIndex) {
            ((MenuListNode *)listNode)->flags48 |= MNU_STAFF_NODE_SELECTED;
        } else {
            ((MenuListNode *)listNode)->flags48 &= ~MNU_STAFF_NODE_SELECTED;
        }
        listNode = (u8 *)((MenuListNode *)listNode)->next;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5240);

u32 mnuClearSkillSelectionImageState(void) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuRuntime *state = (SkillMenuRuntime *)((MenuContext *)context)->party;
    MenuWindowContainer *image = ((MenuContext *)context)->imageHandle;
    if (image->list->cursor->index == 0) {
        mnuBeginWindowFadeTransition(image, &((MenuContext *)context)->transition);
    }
    state->unk30 = 0;
    return 1;
}

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

/* Clear one skill slot, retaining the native short-arity maxima recomputation. */
void mnuClearPartySkillSlot(DatPartyRecord *partyEntry, s32 skillSlot) {
    partyEntry->effectData[skillSlot] = 0;
    ptyRecomputeMaxHpMp();
}

/* Open the selected skill's popup or cancel, then process list navigation.
 * Native list reads precede the late window guard; preserve that ordering. */
void mnuCampMenuHandleInput(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 menuWork = ((MenuContext *)context)->party;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_SKILL_INPUT_MASK);
    MenuWindowContainer *window = ((SkillMenuRuntime *)menuWork)->selectedWindow;
    MenuList *list = window->list;

    list->stateFlags &= ~MNU_LIST_SELECTION_FLAG;
    if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
        MenuListNode *selectedNode = list->cursor;
        s32 sortKey = selectedNode->sortKeyPrimary;

        if (!(selectedNode->flags48 & MNU_STAFF_NODE_UNAVAILABLE) && sortKey != 0) {
            mnuSetPopupEntry(((MenuContext *)context)->popupState, D_003E7774);
        } else {
            inputFlags = MNU_STAFF_INPUT_REJECTED;
        }
    }
    if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
        mnuSetPopupEntryFlagged(((MenuContext *)context)->popupState, D_003E773C);
    }
    if (window != 0) {
        if (!(inputFlags & MNU_STAFF_INPUT_NAV_STATE_MASK)) {
            func_002B9808((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_PREVIOUS) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_NEXT) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, inputFlags, &window->list->stateFlags);
    }
}

/* Apply a selected skill to the current slot, then process window input. */
void func_002B5580(s32 callback) {
    MenuContext *context = (MenuContext *)kwlnTaskGetUserValue();
    SkillMenuRuntime *menuWork = (SkillMenuRuntime *)context->party;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_SKILL_INPUT_MASK);
    MenuWindowContainer *window = menuWork->selectedWindow;
    MenuList *list = window->list;
    DatPartyRecord *partyEntry;
    MenuWindowContainer **skillWindowSlot;
    MenuWindowContainer *skillWindow;
    MenuListNode *selectedNode;
    s32 selectedSlot;
    s32 skillId;
    s32 partyIndex = context->partyWindow.lists[0]->cursor->index;
    MenuListNode *categoryCursor = menuWork->categoryList->cursor;
    DatGameState *gameState = datGameState;

    list->stateFlags &= ~MNU_LIST_SELECTION_FLAG;
    partyEntry = &gameState->party[partyIndex];
    skillWindowSlot = &menuWork->skillWindows[categoryCursor->index];
    skillWindow = *skillWindowSlot;
    if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
        selectedNode = skillWindow->list->cursor;
        selectedSlot = list->cursor->index;
        if (selectedNode->index == 0) {
            mnuClearPartySkillSlot(partyEntry, selectedSlot);
        } else {
            skillId = selectedNode->sortKeyPrimary;
            if (skillId != 0xFFFF) {
                mnuAddPartySkillIfMissing(partyEntry, (u16)skillId, selectedSlot);
            } else {
                inputFlags = MNU_STAFF_INPUT_REJECTED;
            }
        }
        window = mnuSeekSelectedWindowCursor(0, callback);
        mnuInitPartyPanelSlots(&context->partyPanel);
        func_002BCAB0(&context->partyWindow);
        mnuSetPopupEntryFlagged(context->popupState, D_003E7790);
        func_002B45D8(context);
        window->list->stateFlags |= MNU_LIST_SELECTION_FLAG;
    }
    if (inputFlags & MNU_STAFF_INPUT_CANCEL) {
        mnuSetPopupEntryFlagged(context->popupState, D_003E7790);
    }
    if (window != 0) {
        if (!(inputFlags & MNU_STAFF_INPUT_NAV_STATE_MASK)) {
            func_002B9808((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_PREVIOUS) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_NEXT) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, inputFlags, &window->list->stateFlags);
    }
}

void mnuSwapPartySkillSlots(DatPartyRecord *party, s32 firstSlot, s32 secondSlot) {
    u16 value = party->effectData[firstSlot];

    party->effectData[firstSlot] = party->effectData[secondSlot];
    party->effectData[secondSlot] = value;
}

/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
void ptySkillMenuHandleSlotReorder(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 menuWork = ((MenuContext *)context)->party;
    u32 inputFlags = mnuMapPadMaskToFlags(MNU_STAFF_REORDER_INPUT_MASK);
    MenuWindowContainer *window = ((SkillMenuRuntime *)menuWork)->selectedWindow;
    DatPartyRecord *partyEntry = &datGameState->party[((MenuContext *)context)->partyWindow.lists[0]->cursor->index];
    MenuList *list = window->list;

    list->stateFlags &= ~MNU_LIST_SELECTION_FLAG;
    if (inputFlags & MNU_STAFF_INPUT_CONFIRM) {
        s32 selectedSlot = list->cursor->index;

        if (mnuHasSelectedListNodeId(callback) == 0) {
            ((SkillMenuRuntime *)menuWork)->selectedIndex = selectedSlot;
        } else if (selectedSlot != ((SkillMenuRuntime *)menuWork)->selectedIndex) {
            mnuSwapPartySkillSlots(partyEntry, ((SkillMenuRuntime *)menuWork)->selectedIndex, selectedSlot);
            mnuClearSelectedListNodeId(callback);
            window = mnuSeekSelectedWindowCursor(0, callback);
        } else {
            inputFlags = MNU_STAFF_INPUT_REJECTED;
        }
    }
    if (inputFlags & MNU_STAFF_REORDER_CANCEL_MASK) {
        inputFlags = MNU_STAFF_INPUT_CANCEL;
        if (mnuHasSelectedListNodeId(callback) == 0) {
            mnuSetPopupEntryFlagged(((MenuContext *)context)->popupState, D_003E7790);
        }
        mnuClearSelectedListNodeId(callback);
    }
    mnuHighlightSelectedListNode(callback);
    if (window != 0) {
        if (!(inputFlags & MNU_STAFF_INPUT_NAV_STATE_MASK)) {
            func_002B9808((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_PREVIOUS) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (inputFlags & MNU_STAFF_INPUT_NEXT) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, inputFlags, &window->list->stateFlags);
    }
}

s32 ptySkillMenuUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 state = func_002C4038(&((MenuContext *)context)->transitionWork, ((MenuContext *)context)->popupState, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    if (((MenuContext *)context)->imageHandle->list->cursor->index == 0) {
        mnuCampMenuHandleInput(callback);
    } else if (menu[12] == 0) {
        func_002B5580(callback);
    } else {
        ptySkillMenuHandleSlotReorder(callback);
    }
    return 0;
}

typedef struct MenuPanelBlock {
    u32 word[14];
} MenuPanelBlock;

typedef struct MenuPanelWindow {
    u8 pad0[4];
    s32 field4;
    u8 pad8[0x10];
    MenuList *list; /* 0x18 */
    u8 pad1C[0x3C];
    MenuPanelBlock block; /* 0x58 */
    u8 pad90[4];
    s32 field94;
} MenuPanelWindow;

extern void mnuSetPanelState();
extern void func_002C0958();

void ptySkillMenuCopyPageState(s32 context) {
    s32 *party = (s32 *)((MenuContext *)context)->party;
    MenuPanelWindow **windows = (MenuPanelWindow **)(party + 4);
    MenuPanelWindow **slot;
    MenuPanelWindow *window;
    s32 index = ((MenuList *)party[3])->cursor->index;
    u32 j;

    slot = windows + index;
    window = *slot;
    itfDrawGridWithResolvedSlot(0xED0, 0x2E0, 0, 1, ((MenuContext *)context)->labelHandle, 0x24, 0x53);
    mnuSetPanelState(party[8], index);
    func_002C0958(0xED0, 0x328, 0, party[8], 0x53);
    if (window->list->cursor->index == 0) {
        window->list->stateFlags |= 0x10;
    } else {
        window->list->stateFlags &= ~0x10;
    }
    mnuDrawWindowContainer(0x1190, 0x658, 0, (s32)window, 0x53);
    for (j = 0; j < 4; j++) {
        if (j != index) {
            memcpy(&windows[j]->block, &window->block, sizeof(MenuPanelBlock));
            windows[j]->field94 = window->field94;
            windows[j]->field4 = window->field4;
        }
    }
    if (party[14] >= 0x19D) {
        party[14] -= 0x138;
    }
    party[14] += 6;
}

s32 ptySkillMenuEnterPage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 label;
    if (((MenuContext *)context)->imageHandle->list->cursor->index == 0) {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    } else {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
        ptySkillMenuCopyPageState(context);
    }
    func_002AAE80(callback);
    if (((MenuContext *)context)->imageHandle->list->cursor->index == 0) {
        mnuCreateStaffImageSprite(2);
    } else if (menu[12] != 0) {
        if (mnuHasSelectedListNodeId(callback) == 0) {
            mnuCreateStaffImageSprite(0x12);
        } else {
            mnuCreateStaffImageSprite(0x13);
        }
    } else if (**(s32 **)(*(s32 *)(*(s32 *)((s32)menu + 0x10 + (**(s32 **)(menu[3] + 0x1c) << 2)) + 0x18) + 0x1c) == 0) {
        mnuCreateStaffImageSprite(0x11);
    } else {
        mnuCreateStaffImageSprite(0x10);
    }
    label = ((MenuWindowContainer *)menu[9])->list->cursor->sortKeyPrimary;
    if (label != 0xffff && label != 0) {
        func_002AAC70(0, label, D_00435E6C, context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 ptySkillMenuDispatchPageRequest(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

s32 ptySkillMenuApplyFieldUseAndCost(id, context)
    u16 id;
    s32 context;
{
    MenuPageWindow *window = &((MenuContext *)context)->partyWindow;
    DatPartyRecord *slotA = &datGameState->party[((MenuContext *)context)->partyWindow.lists[0]->cursor->index];
    DatPartyRecord *slotB = &datGameState->party[((MenuContext *)context)->partyWindow.lists[1]->cursor->index];
    if (mnuIsEntryCostUnaffordable(id, slotA) != 0) {
        return 0;
    }
    if (ptySkillApplyFieldUseEffect(window, id, slotA, slotB) != 0) {
        mnuConsumeEntryCost(id, slotA);
        mnuInitPartyPanelSlots(&((MenuContext *)context)->partyPanel);
        func_002BCA98(window);
        func_002BCAB0(window);
        return 1;
    }
    return 0;
}

/* Mark entries whose field-use cost cannot be paid by the selected party member. */
void mnuFlagMatchingEntries(s32 context) {
    DatPartyRecord *slot = &datGameState->party[((MenuContext *)context)->partyWindow.lists[0]->cursor->index];
    MenuListNode *link = ((SkillMenuRuntime *)((MenuContext *)context)->party)->selectedWindow->list->first;
    if (link != NULL) {
        do {
            /* Cost lookup uses the low halfword of the list key. */
            if (mnuIsEntryCostUnaffordable((u16)link->sortKeyPrimary, slot)) {
                link->flags48 |= 1;
            }
            link = link->next;
        } while (link != NULL);
    }
}

s32 ptySkillMenuHandleFieldUse(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    s32 *popup = ((MenuContext *)context)->popupState;
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    s32 label;
    u16 code;
    MenuPageWindow *window;
    state = func_002C4038(&((MenuContext *)context)->transitionWork, popup, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    label = ((SkillMenuRuntime *)menu)->selectedWindow->list->cursor->sortKeyPrimary;
    code = label;
    if (mnuGetAbilityTargetCategory(code) == 2) {
        ((MenuContext *)context)->partyWindow.flags |= 0x10;
    }
    if (mnuGetAbilityTargetCategory(code) == 3) {
        ((MenuContext *)context)->partyWindow.flags |= 0x20;
    }
    window = &((MenuContext *)context)->partyWindow;
    mnuStepPartyPanelListFromInput(8, window);
    if (buttons & 1) {
        buttons = ptySkillMenuApplyFieldUseAndCost(label, context) == 0 ? 0x8000 : 0;
        mnuFlagMatchingEntries(context);
    }
    if (buttons & 2) {
        mnuSetPopupEntry(popup, D_003E7758);
        mnuClearActionFlags(1, window);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s32 ptySkillMenuOpenSelectedSkillPage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(3);
    func_002AAC70(0, ((SkillMenuRuntime *)menu)->selectedWindow->list->cursor->sortKeyPrimary, D_00435E6C, context, 1, 1, 0x53);
    ((SkillMenuRuntime *)menu)->selectedWindow->list->stateFlags &= ~8;
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 ptySkillMenuDispatchConfirmRequest(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

s32 ptySkillMenuOpenPartyPage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 party = ((MenuContext *)context)->party;
    MenuWindowContainer *window;

    mnuSelectPage(&((MenuContext *)context)->partyWindow, ((MenuContext *)context)->partyWindow.lists[0]->cursor->index);
    ((MenuContext *)context)->partyWindow.flags |= 0x200;
    ptySkillMenuBuildEquippedSlots(0, callback);
    func_002B4848(context);
    window = ((SkillMenuRuntime *)party)->selectedWindow;
    window->list->stateFlags |= 8;
    mnuBeginWindowFadeTransition(window, &((MenuContext *)context)->transition);
    return 1;
}

u32 ptySkillMenuClosePartyPage(u32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuContext *)context)->imageHandle, &((MenuContext *)context)->transition);
    mnuDestroySelectedPartyWindow(callback);
    mnuDestroySkillMenuWindows(context);
    mnuClearPageSelectionHandles(&((MenuContext *)context)->partyWindow);
    return 1;
}

s32 ptySkillMenuHandlePageSwitch(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 changed = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    if (buttons & 0x100) {
        ptySkillMenuClosePartyPage(callback);
        mnuRetreatListCursorDefault(((MenuContext *)context)->partyWindow.lists[0]);
        changed = 1;
    }
    if ((buttons & 0x200) && changed == 0) {
        ptySkillMenuClosePartyPage(callback);
        mnuAdvanceListCursorDefault(((MenuContext *)context)->partyWindow.lists[0]);
        changed = 1;
    }
    mnuClearListFlagsOneAndTwo(((MenuContext *)context)->partyWindow.lists[0]);
    if (changed != 0) {
        ptySkillMenuOpenPartyPage(callback);
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        return 1;
    }
    return 0;
}

/* Shift a selected child window's list to the requested row. Keep the
 * double-dereferenced cursor load raw: the typed form does not match. */
void mnuSeekSelectedWindowRow(s32 menu, s32 target) {
    s32 *entry = (s32 *)menu + **(s32 **)(*(s32 *)(menu + 4) + 0x1C);
    s32 window = entry[2];
    s32 delta = target - ((MenuWindowContainer *)window)->list->windowOffset;
    s32 dir;
    s32 n;

    if (delta < 0) {
        dir = -1;
        delta = -delta;
    } else {
        dir = 1;
    }
    if (delta > 0) {
        n = delta;
        do {
            if (dir < 0) {
                mnuReverseListSelection(window, 1);
            }
            if (dir > 0) {
                mnuAdvanceListSelection(window, 1);
            }
            n--;
        } while (n != 0);
    }
    mnuResetListNodeFadeCounters(((MenuWindowContainer *)window)->list);
}

s32 ptySkillMenuBrowseCandidatePages(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    u32 input = mnuMapPadMaskToFlags(0xC37);
    s32 *popup = ((MenuContext *)context)->popupState;
    u32 buttons = mnuMapPadMaskToFlags(0xC0);
    s32 state = func_002C4038(&((MenuContext *)context)->transitionWork, popup, 0, (void *)callback);
    s32 *windows;
    s32 *windowSlot;
    MenuWindowContainer *window;
    s32 offset;

    if (state != 0) {
        return state;
    }
    if (ptySkillMenuHandlePageSwitch(callback) != 0) {
        return 0;
    }
    windows = menu + 4;
    windowSlot = windows + ((MenuList *)menu[3])->cursor->index;
    window = (MenuWindowContainer *)*windowSlot;
    if (!(input & 0x300000)) {
        func_002B9808((s32)window);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection((s32)window);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection((s32)window);
    }
    mnuHandlePanelListPageJumpInput(window, &input);
    if (!(buttons & 0xC00000)) {
        mnuClearListFlagsOneAndTwo(menu[3]);
    }
    offset = window->list->windowOffset;
    if (buttons & 0x40) {
        mnuRetreatListCursorDefault(menu[3]);
        mnuSeekSelectedWindowRow((s32)(menu + 2), offset);
    }
    if (buttons & 0x80) {
        mnuAdvanceListCursorDefault(menu[3]);
        mnuSeekSelectedWindowRow((s32)(menu + 2), offset);
    }
    mnuPlayInputSound(0, buttons, &((MenuList *)menu[3])->stateFlags);
    windowSlot = windows + ((MenuList *)menu[3])->cursor->index;
    window = (MenuWindowContainer *)*windowSlot;
    if (input & 1) {
        MenuListNode *node = window->list->cursor;

        if (node->sortKeyPrimary != 0xFFFF && !(node->flags48 & 1)) {
            mnuSetPopupEntry(popup, D_003E7758);
        } else {
            input = 0x8000;
        }
    }
    if (input & 4) {
        menu[12] = 1;
        mnuSetPopupEntry(popup, D_003E7758);
        input = 1;
    }
    if (input & 2) {
        mnuSetPopupEntryFlagged(popup, D_003E773C);
    }
    mnuPlayInputSound(0, input, &window->list->stateFlags);
    return 0;
}

s32 mnuOpenSkillDetailPanel(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 index = ((MenuList *)menu[3])->cursor->index;
    s32 label = ((MenuWindowContainer *)*(s32 *)((s32)menu + 0x10 + (index << 2)))->list->cursor->sortKeyPrimary;
    ptySkillMenuCopyPageState(context);
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(0xf);
    if (label != 0xffff && label != 0) {
        func_002AAC70(0, label, D_00435E6C, context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    ((SkillMenuRuntime *)menu)->selectedWindow->list->stateFlags |= 8;
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    func_002AA7A0(3, ((MenuContext *)context)->displayHandle);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 func_002B6800(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}

void mnuDrawSelectionLabel(u16 id) {
    FrFontGlyph *label = itfDrawTextWithSelectedFontMode(0x11B0, 0xA88, 0, 0, id, 1);

    frFontSetChildColors(label, 0xA09DC35A);
    func_0019D550(label, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(label);
}

extern MenuPoint D_00437C08[];
extern MenuPoint D_00437C10[];
extern MenuPoint D_00437C18[];
extern const char *D_003E77C8[16];
extern u8 (*D_00435E68)[33];
extern s32 ptyGetAffinityKind(s32, s32);
extern s32 ptyGetAffinityFlagsWithoutOverride(s32, s32);

/* Draw each of the command's three partner requirements. */
void func_002B6898(u16 affinity, s32 resource, s32 labels) {
    MenuPoint position = D_00437C08[0];
    MenuPoint textOffset = D_00437C10[0];
    MenuPoint iconOffset = D_00437C18[0];
    s32 x = position.x;
    s32 y = position.y;
    s32 i;

    for (i = 0; i < 3; i++, y += 0xC0) {
        s32 kind = ptyGetAffinityKind(affinity, i);
        FrFontGlyph *glyph = 0;

        if (kind < 0) {
            if (kind == -1) {
                s32 requirement = ptyGetAffinityFlagsWithoutOverride(affinity, i);
                if (requirement > 0) {
                    s32 range = mnuLookupRangeEntry((u16)requirement);
                    itfDrawGridWithResolvedSlot(x + iconOffset.x, y + iconOffset.y,
                                               0, 1, labels, range + 1, 0x53);
                    glyph = itfCreateConvertedTextGlyph(x + textOffset.x, y + textOffset.y,
                                                       0, 0xA09DC380,
                                                       D_00435E64[requirement], 0);
                }
            } else {
                s32 requirement = ptyGetAffinityFlagsWithoutOverride(affinity, i);
                glyph = itfCreateConvertedTextGlyph(x + textOffset.x, y + textOffset.y,
                                                   0, 0xA09DC380,
                                                   D_00435E68[requirement], 0);
                itfDrawGridWithResolvedSlot(x + iconOffset.x, y + iconOffset.y,
                                           0, 1, resource, 0xA, 0x53);
            }
        } else {
            glyph = itfCreateConvertedTextGlyph(x + textOffset.x, y + textOffset.y,
                                               0, 0xA09DC380, (const u8 *)D_003E77C8[kind], 0);
            itfDrawGridWithResolvedSlot(x + iconOffset.x, y + iconOffset.y,
                                       0, 1, resource, 0xA, 0x53);
        }
        if (glyph != 0) {
            func_0019D550(glyph, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(glyph);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6B00);

/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(DatPartyRecord **out, s32 max) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < max; i++) {
        out[i] = 0;
    }
    i = 0;
    while (count < max) {
        DatPartyRecord *entry = &datGameState->party[i];

        if ((entry->flags & 1) != 0 && (entry->flags & 2) != 0) {
            out[count] = entry;
            count++;
        }
        i++;
        if (i >= 5) {
            break;
        }
    }
}

s32 mnuHasAvailableSlotResource(s32 id) {
    s32 i;
    id -= DAT_AFFINITY_FIRST_COMMAND;
    if ((datAffinityRecords[id].flags & 2) != 0) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (datAffinityRecords[id].requirements[i] != -1) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6D78);

u32 mnuDestroySkillSelectionWindow(u32 callback) {
    s32 party;

    party = kwlnTaskGetUserValue();
    party = ((MenuContext *)party)->party;
    mnuDestroyWindowContainer(((SkillMenuRuntime *)party)->selectedWindow);
    ((SkillMenuRuntime *)party)->selectedWindow = 0;
    return 1;
}

u32 mnuResetSelection(u32 callback) {
    s32 context;
    SkillMenuRuntime *state;
    mnuCreateItemState(callback);
    context = kwlnTaskGetUserValue(callback);
    state = (SkillMenuRuntime *)((MenuContext *)context)->party;
    func_002B6D78(callback);
    mnuFlagActiveWindows(&((MenuContext *)context)->partyWindow);
    state->unk2C = 0;
    mnuBeginWindowFadeTransition(state->selectedWindow, &((MenuContext *)context)->transition);
    return 1;
}

u32 mnuCloseSkillSelection(u32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition(((MenuContext *)context)->imageHandle, &((MenuContext *)context)->transition);
    mnuDestroySkillSelectionWindow(callback);
    mnuClearPartyPanelActiveFlags(&((MenuContext *)context)->partyWindow);
    mnuCloseItemSelectionState(callback);
    return 1;
}

s32 mnuUpdateSkillListInput(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 *popup = ((MenuContext *)context)->popupState;
    u32 buttons = mnuMapPadMaskToFlags(0xc32);
    s32 state;
    s32 *list;
    state = func_002C4038(&((MenuContext *)context)->transitionWork, popup, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    if ((buttons & 0x300000) == 0) {
        list = menu + 1;
        func_002B9808(list[8 + menu[11]]);
    }
    list = menu + 1;
    if (buttons & 0x10) {
        mnuRetreatWindowListSelection(list[8 + menu[11]]);
    }
    if (buttons & 0x20) {
        mnuAdvanceWindowListSelection(list[8 + menu[11]]);
    }
    mnuHandlePanelListPageJumpInput(list[8 + menu[11]], &buttons);
    mnuClearWindowPanelTransitionFlag((MenuWindowContainer *)list[8 + menu[11]]);
    mnuPlayInputSound(0, buttons, &((MenuWindowContainer *)list[8 + menu[11]])->list->stateFlags);
    if (buttons & 2) {
        mnuSetPopupEntryFlagged(popup, D_003E7720);
        mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayHandle, 0, 1);
    }
    return 0;
}

typedef struct MnuCampPanelDrawRecord {
    EffectSlotSet *resource;
    s32 sprite;
    s32 xOffset;
    s32 yOffset;
} MnuCampPanelDrawRecord;

typedef struct MnuCampPanelOrigins {
    MenuPoint primary;
    MenuPoint secondary;
} MnuCampPanelOrigins;

extern const MnuCampPanelOrigins D_0042AE48;

void func_002B7228(MenuContext *context) {
    MnuCampPanelDrawRecord primaryRows[5] = {
        {(EffectSlotSet *)context->resourceHandle, 0x11, 0x1E0, 0xC0},
        {(EffectSlotSet *)context->resourceHandle, 0x09, 0x110, 0x20},
        {(EffectSlotSet *)context->labelHandle, 0x0C, 0x2A0, 0x38},
        {(EffectSlotSet *)context->resourceHandle, 0x0F, 0x1E0, 0xC0},
        {(EffectSlotSet *)context->labelHandle, 0x10, 0x180, 0x138},
    };
    MnuCampPanelDrawRecord secondaryRows[4] = {
        {(EffectSlotSet *)context->resourceHandle, 0x11, 0x1E0, 0xC0},
        {(EffectSlotSet *)context->resourceHandle, 0x09, 0x110, 0x20},
        {(EffectSlotSet *)context->resourceHandle, 0x0E, 0x2C0, 0x50},
        {(EffectSlotSet *)context->resourceHandle, 0x0F, 0x1E0, 0xC0},
    };
    MnuCampPanelOrigins origins = D_0042AE48;
    s32 i;

    i = 0;
    do {
        func_00306CD0(primaryRows[i].xOffset + origins.primary.x, primaryRows[i].yOffset + origins.primary.y,
            0, 0x100, 1, primaryRows[i].resource, primaryRows[i].sprite, 0x53);
        i++;
    } while (i < 4);
    {
        s32 x = primaryRows[4].xOffset + origins.primary.x;
        s32 y = primaryRows[4].yOffset + origins.primary.y;
        i = 2;
        do {
            i--;
            func_00306CD0(x, y, 0, 0x100, 1, primaryRows[4].resource, primaryRows[4].sprite, 0x53);
            y += 0xC0;
        } while (i >= 0);
    }
    i = 0;
    do {
        func_00306CD0(secondaryRows[i].xOffset + origins.secondary.x, secondaryRows[i].yOffset + origins.secondary.y,
            0, 0x100, 1, secondaryRows[i].resource, secondaryRows[i].sprite, 0x53);
        i++;
    } while (i < 4);
}

void func_002B7588(s32 context) {
    s32 index;

    for (index = **(s32 **)(context + 0x28c); index < 3; index = index + 1) {
    }
}

s32 mnuCampMenuDrawStatus(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    u32 label;
    func_002AAE80(callback);
    func_002B7588(context);
    mnuCreateStaffImageSprite(0x14);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, &((MenuContext *)context)->transition, 0x53);
    label = ((MenuWindowContainer *)*(s32 *)(menu + 0x24 + *(s32 *)(menu + 0x2c) * 4))->list->cursor->sortKeyPrimary;
    func_002B7228((MenuContext *)context);
    if (label != 0 && label != 0xffff) {
        label = (u16)label;
        mnuDrawSelectionLabel(label);
        func_002B6898(label, ((MenuContext *)context)->resourceHandle, ((MenuContext *)context)->labelHandle);
    }
    func_002AA7A0(2, ((MenuContext *)context)->displayHandle);
    return menuSetHandler((void *)context, 1, (void *)callback);
}

s32 func_002B76B0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)callback);
}


void mnuInitializeMapPacket(u32 value, u32 *values, s32 count, MapPacket *packet) {
    s32 index = 0;
    packet->value = value;
    packet->type = 4;
    packet->count = count;
    if (count > 0) {
        do {
            packet->items[index] = values[index];
            index++;
        } while (index < count);
    }
}

void mnuOrEntryFlags(u32 flags, u32 *entryFlags) {
    *entryFlags = *entryFlags | flags;
}

void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);
INCLUDE_ASM(const s32, "game/code_002B0278", mnuCopyCampEffectRowData);

void mnuSetCampEffectResourceHandles(u32 first, u32 second, MenuEffectResources *resources) {
    resources->packet.sheets[0] = first;
    resources->animationHandle = second;
}


void mnuBindCampEffectAnimation(MenuEffectResources *resources) {
    effConfigureIndexedSlotResource(resources->packet.sheets[0],
                   resources->packet.items[4],
                   resources->animationHandle, 0, 4);
}

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD38);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD78);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD88);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD98);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADA8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADB8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADC8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADD8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADE8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADF8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE08);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE18);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE28);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE48);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE58);

void mnuLoadEffectResources(MenuEffectResources *resources) {
    mnuInitializeMapPacket(0, D_003E7828, 0xb, &resources->packet);
    mnuCopyCampEffectRowData(&D_003E7858, resources);
    resources->packet.sheets[0] = effLoadIndexedResource("/camp/spr/n_min/", D_003E7818[0], 0);
    resources->animationHandle = effLoadMappedResource("/camp/mot/", D_003E7820[0]);
    mnuBindCampEffectAnimation(resources);
}

void mnuRequestEffectResources(MenuEffectResources *resources) {
    mnuInitializeMapPacket(0, D_003E7828, 0xb, &resources->packet);
    mnuCopyCampEffectRowData(&D_003E7858, resources);
    effRequestResourceByMode("/camp/spr/n_min/", D_003E7818[0], 0, &resources->packet.sheets[0]);
    effRequestMappedResource("/camp/mot/", D_003E7820[0], &resources->animationHandle);
}

u32 mnuBindCampEffectWhenLoaded(MenuEffectResources *resources) {
    if (resources->packet.sheets[0] == 0) {
        return 0;
    }
    if (resources->animationHandle == 0) {
        return 0;
    }
    mnuBindCampEffectAnimation(resources);
    return 1;
}

void mnuDestroyEffectResources(MenuEffectResources *resources) {
    u32 i;
    for (i = 0; i < ARRAY_COUNT(resources->packet.sheets); i++) {
        effDestroyResourceSlotSet(resources->packet.sheets[i]);
    }
    effDestroyPackedBatch(resources->animationHandle);
}


void mnuSpawnSpark(MenuCampEffect *fx) {
    s32 slot = -1;
    s32 i;

    if (fx->count >= 0x10) {
        return;
    }
    for (i = 0; i < 0x10; i++) {
        if (fx->direction[i] == 0) {
            slot = i;
            break;
        }
    }
    if (slot >= 0) {
        fx->direction[slot] = (effMiscRand(0) & 1) + 1;
        fx->life[slot] = ((effMiscRand(0) & 3) + 4) << 4;
        if (fx->direction[slot] == 1) {
            fx->velocity[slot][0] = -0xFA0;
        } else {
            fx->velocity[slot][0] = 0x2FA0;
        }
        fx->velocity[slot][1] = ((s32)(effMiscRand(0) % 0x1C0) - 0x7D) << 3;
        fx->count += 1;
    }
}

void mnuRetireCampSpark(MenuCampEffect *effects, s32 index) {
    effects->direction[index] = 0;
    effects->count = effects->count - 1;
}

void mnuDrawAndAdvanceCampSparks(MenuCampEffect *fx, s32 arg) {
    s32 i;
    for (i = 0; i < 0x10; i++) {
        if (fx->direction[i] > 0) {
            itfDrawGridWithResolvedSlot(fx->velocity[i][0], fx->velocity[i][1], 0, 0, fx->resources.packet.sheets[0], fx->resources.packet.items[6], arg);
            if (fx->direction[i] == 1) {
                fx->velocity[i][0] += fx->life[i];
                if (fx->velocity[i][0] > 0x2000) {
                    mnuRetireCampSpark(fx, i);
                }
            } else {
                fx->velocity[i][0] -= fx->life[i];
                if (fx->velocity[i][0] < -0xFA0) {
                    mnuRetireCampSpark(fx, i);
                }
            }
        }
    }
    if (fx->resources.packet.type & 4) {
        if ((effMiscRand(0) & 3) == 0) {
            mnuSpawnSpark(fx);
        }
    } else if ((effMiscRand(0) & 0x1F) == 0) {
        mnuSpawnSpark(fx);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7C10);

typedef struct MenuBadgePlace {
    s32 slot;
    s32 x;
    s32 y;
} MenuBadgePlace;

typedef struct MenuBadgeLayout {
    MenuBadgePlace place[2];
} MenuBadgeLayout;


extern MenuBadgeLayout D_0042AED0;

/* Windows, badges and icons share this bounded, monotonic fade step. */
#define MNU_ADVANCE_FADE(value, amount, maximum) { \
    if ((value) < (maximum)) { \
        (value) += (amount); \
    } \
    if ((value) > (maximum)) { \
        (value) = (maximum); \
    } \
}

void mnuDrawBadgeFade(MenuCampEffect *set, s32 arg) {
    MenuBadgeLayout layout = D_0042AED0;
    s32 handle;
    if (!(set->resources.packet.type & 4)) {
        handle = set->resources.packet.items[layout.place[0].slot];
        func_00306CD0(layout.place[0].x, layout.place[0].y, 0, set->fade, 0, (EffectSlotSet *)set->resources.packet.sheets[0], handle, arg);
        itfGridLookupValueOrDefault(set->resources.packet.sheets[0], handle);
        MNU_ADVANCE_FADE(set->fade, 0x10, 0x100);
    }
    if (!(set->resources.packet.type & 2)) {
        itfDrawGridWithResolvedSlot(layout.place[1].x, layout.place[1].y, 0, 0, set->resources.packet.sheets[0], set->resources.packet.items[layout.place[1].slot], arg);
    }
}

extern MenuBadgeLayout D_0042AEE8;

void mnuDrawCampIconBackdrop(MenuCampEffect *set, s32 arg) {
    MenuBadgePlace blank[1];
    MenuBadgeLayout layout;
    u32 i;

    memset(blank, 0, sizeof(blank));
    layout = D_0042AEE8;
    sdfSubmitGsTestOneRegisterPacket(0x30000, arg);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x80808080, arg);
    for (i = 0; i < 1; i++) {
        itfDrawGridWithResolvedSlot(blank[i].x, blank[i].y, 0, 0, set->resources.packet.sheets[0], set->resources.packet.items[blank[i].slot], arg);
    }
    if (!(set->resources.packet.type & 2)) {
        for (i = 0; i < 2; i++) {
            itfDrawGridWithResolvedSlot(layout.place[i].x, layout.place[i].y, 0, 0, set->resources.packet.sheets[0], set->resources.packet.items[layout.place[i].slot], arg);
        }
    }
    if (!(set->resources.packet.type & 4)) {
        mnuDrawBadgeFade(set, arg);
    }
    func_002B7C10(set, arg);
}

void mnuEnableCampBadgeFade(u32 *flags) {
    *flags = *flags & 0xfffffffb;
}

MenuList *mnuCreateListState(u32 owner, u32 visibleCount, s32 rowSpacing) {
    MenuList *node = (MenuList *)sdfAllocAndClearQuadwords(0x40);
    node->id = owner;
    node->visibleCount = visibleCount;
    node->rowStep = rowSpacing * 8;
    node->scale = 0x100;
    node->head = NULL;
    node->first = NULL;
    node->windowOffset = 0;
    node->cursor = NULL;
    return node;
}

u32 mnuDestroyListState(MenuList *list) {
    MenuListNode *result;

    do {
        result = func_002B86E8(list);
    } while (result != 0);
    sdfReleaseChipBlock(list);
    return 1;
}

void mnuUpdateListScrollFlags(MenuList *list) {
    MenuListNode *node = list->head;
    s32 i;

    if (node == NULL) {
        list->flags = 0;
        return;
    }
    if (node->prev != NULL) {
        list->flags |= 1;
    } else {
        list->flags &= ~1;
    }
    for (i = 0; i < list->visibleCount; i++) {
        node = node->next;
        if (node == NULL) {
            list->flags &= ~2;
            return;
        }
    }
    list->flags |= 2;
}

MenuListNode *mnuListAppendNode(list, value)
    MenuList *list;
    const void *value;
{
    MenuListNode *node = sdfAllocAndClearQuadwords(0x74);
    s32 index = list->count;
    MenuListNode *last;

    if (index == 0) {
        list->head = node;
        list->cursor = node;
        list->first = node;
    }
    node->prev = list->last;
    node->next = NULL;
    node->value = value;
    last = list->last;
    node->prev = last;
    if (last != NULL) {
        last->next = node;
    }
    node->index = index;
    list->last = node;
    list->count++;
    mnuUpdateListScrollFlags(list);
    if (list->visibleCount < 3) {
        if (list->visibleCount < list->count) {
            list->visibleCount = list->count;
        }
    }
    return node;
}

s32 mnuListContainsFinalNode(MenuList *list) {
    MenuListNode *node = list->head;
    s32 index = 0;
    if (node != NULL) {
        s32 count = list->visibleCount;
        do {
            if (index >= count) {
                return 0;
            }
            if (node == list->last) {
                return 1;
            }
            node = node->next;
            index++;
        } while (node != NULL);
    }
    return 0;
}

MenuListNode *mnuListAdvanceCursor(MenuList *, s32, s32);
MenuListNode *mnuListRetreatCursor(MenuList *, s32, s32);

/* Insert a node holding `value` before (or, with options & 2, after) anchor, keeping the visible window and
 * cursor in place. */
MenuListNode *func_002B83A0(MenuList *list, MenuListNode *anchor,
                         const void *value, s32 mode, u32 options) {
    MenuListNode *node;
    MenuListNode *walk;

    if (list->count == 0 || anchor == NULL || anchor == list->last) {
        node = mnuListAppendNode(list, value);
        if (list->windowOffset >= list->visibleCount - 1 && list->cursor != list->last) {
            list->head = list->head->next;
            list->cursor = list->cursor->next;
            if (mode == -1) {
                mnuListRetreatCursor(list, 0, 1);
            }
        } else if (mode == -2) {
            mnuListAdvanceCursor(list, 0, 1);
        }
        return node;
    }

    node = sdfAllocAndClearQuadwords(sizeof(MenuListNode));
    node->value = value;
    if (options & 2) {
        node->prev = anchor;
        node->index = anchor->index;
        node->next = anchor->next;
        if (anchor->next != NULL) {
            anchor->next->prev = node;
        }
        anchor->next = node;
        walk = node;
        do {
            walk->index++;
            walk = walk->next;
        } while (walk != NULL);
    } else {
        node->prev = anchor->prev;
        node->next = anchor;
        node->index = anchor->index;
        if (anchor->prev != NULL) {
            anchor->prev->next = node;
        }
        anchor->prev = node;
        if (anchor == list->first) {
            list->first = node;
            if (list->cursor == list->head || list->count < list->visibleCount) {
                list->head = node;
            }
        }
        walk = anchor;
        while (walk != NULL) {
            walk->index++;
            walk = walk->next;
        }
    }
    list->count++;
    if (node->index >= list->head->index && node->index < list->cursor->index) {
        list->cursor = list->cursor->prev;
        if (list->first == list->head) {
            if (mode >= 0 || mode == -2) {
                if (list->count >= list->visibleCount + 1 &&
                    list->cursor->index - list->head->index == list->visibleCount - 1) {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                } else if (mode == -2) {
                    mnuListAdvanceCursor(list, 0, 1);
                }
            } else {
                if (list->count >= list->visibleCount + 1 &&
                    list->cursor->index - list->head->index == list->visibleCount - 1) {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                } else {
                    mnuListAdvanceCursor(list, 0, 1);
                }
            }
        } else {
            if (mnuListContainsFinalNode(list)) {
                if (mode < 0 && mode != -2) {
                    list->head = list->head->next;
                }
            } else {
                if (mode >= 0 || mode == -2) {
                    if (mode == -2) {
                        list->head = list->head->next;
                        list->cursor = list->cursor->next;
                    }
                } else {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                }
            }
        }
    }
    mnuUpdateListScrollFlags(list);
    return node;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B86E8);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;


void mnuSetGridSpriteSlot(MenuListNode *node, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect) {
    node->sprites[row * 4 + col].sprite = sprite;
    node->sprites[row * 4 + col].effect = effect;
    itfSetGridEntryQuantizedAndRefresh(sprite, effect, x, y, x, y);
}

void *mnuWalkNodeList(s32 index, MenuList *list) {
    MenuListNode *node = list->first;
    s32 currentIndex = 0;

    if (node != NULL && index != currentIndex) {
        do {
            node = node->next;
            currentIndex++;
        } while (node != NULL && currentIndex != index);
    }
    return node;
}

s32 mnuSeekListNode(s32 index, MenuList *list) {
    s32 size = list->count;

    if (index >= size) {
        return 0;
    }
    list->windowOffset = 0;
    list->head = list->first;
    list->cursor = list->first;
    if (index > 0) {
        do {
            if (size - list->head->index <= list->visibleCount) {
                list->windowOffset += 1;
            } else {
                list->head = list->head->next;
            }
            list->cursor = list->cursor->next;
            index--;
        } while (index != 0);
    }
    return 1;
}

void mnuSelectFirstListNode(MenuList *list) {
    mnuSeekListNode(0, list);
}

void mnuSelectLastListNode(MenuList *list) {
    mnuSeekListNode(list->count - 1, list);
}

s32 mnuAdvanceListWindowStart(MenuList *list) {
    s32 previousCursor = (s32)list->cursor;
    s32 last = (s32)list->last;
    MenuListNode *head = list->head;

    if (previousCursor == last) {
        return previousCursor;
    }
    head = head->next;
    if (head == NULL) {
        return previousCursor;
    }
    list->head = head;
    list->windowOffset--;
    return previousCursor;
}

/* Step the visible head back one node when a full window follows it. */
s32 mnuRetreatListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *head = list->head;
    MenuListNode *node;
    s32 i;

    if (cursor == list->first) {
        return (s32)cursor;
    }
    node = head;
    for (i = 0; i < list->visibleCount; i++) {
        if (node == NULL) {
            return (s32)cursor;
        }
        node = node->next;
    }
    head = head->prev;
    list->head = head;
    list->windowOffset++;
    return (s32)cursor;
}

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    MenuListNode *cursor = list->cursor;
    MenuListNode *last;
    MenuListNode *next;
    s32 offset;

    if (count < 2) {
        list->stateFlags |= 2;
    }
    if (list->stateFlags & 2) {
        list->stateFlags &= ~1;
        return NULL;
    }
    if (count == 1) {
        return cursor;
    }
    last = list->last;
    if (cursor == last) {
        mnuSelectFirstListNode(list);
        mnuUpdateListScrollFlags(list);
        cursor = list->cursor;
    } else {
        if (cursor == NULL) {
            return NULL;
        }
        next = cursor->next;
        if (next == NULL) {
            return NULL;
        }
        if (keepFade == 0) {
            cursor->animationTimer = 0x100;
        }
        offset = list->windowOffset;
        cursor = next;
        list->cursor = cursor;
        list->windowOffset = offset + 1;
        if (list->windowOffset >= list->visibleCount - 1) {
            if (noScroll == 0) {
                cursor = (MenuListNode *)mnuAdvanceListWindowStart(list);
                last = list->last;
            } else if (cursor != last) {
                cursor = cursor->prev;
                list->windowOffset = offset;
                list->cursor = cursor;
            }
        }
        if (cursor == last) {
            list->stateFlags |= 3;
        }
        if (cursor->next == last && (cursor->next->flags48 & 2)) {
            list->stateFlags |= 3;
        }
        mnuUpdateListScrollFlags(list);
    }
    return cursor;
}

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    MenuListNode *cursor = list->cursor;
    MenuListNode *first;
    MenuListNode *prev;
    s32 offset;

    if (count < 2) {
        list->stateFlags |= 2;
    }
    if (list->stateFlags & 2) {
        list->stateFlags &= ~1;
        return NULL;
    }
    if (count == 1) {
        return cursor;
    }
    first = list->first;
    if (cursor == first) {
        mnuSelectLastListNode(list);
        mnuUpdateListScrollFlags(list);
        cursor = list->cursor;
    } else {
        if (cursor == NULL) {
            return NULL;
        }
        prev = cursor->prev;
        if (prev == NULL) {
            return NULL;
        }
        if (keepFade == 0) {
            cursor->animationTimer = 0x100;
        }
        offset = list->windowOffset;
        cursor = prev;
        list->cursor = cursor;
        list->windowOffset = offset - 1;
        if (list->windowOffset <= 0) {
            if (noScroll == 0) {
                cursor = (MenuListNode *)mnuRetreatListWindowStart(list);
                first = list->first;
            } else if (cursor != first) {
                cursor = cursor->next;
                list->windowOffset = offset;
                list->cursor = cursor;
            }
        }
        if (cursor == first) {
            list->stateFlags |= 3;
        }
        if (cursor->prev == first && (cursor->prev->flags48 & 2)) {
            list->stateFlags |= 3;
        }
        mnuUpdateListScrollFlags(list);
    }
    return cursor;
}

MenuListNode *mnuAdvanceListCursorDefault(u32 list) {
    return mnuListAdvanceCursor((MenuList *)list, 0, 0);
}

MenuListNode *mnuRetreatListCursorDefault(u32 list) {
    return mnuListRetreatCursor((MenuList *)list, 0, 0);
}

s32 mnuScrollListToEnd(MenuList *list) {
    s32 i;

    if (list->cursor == NULL) {
        return 0;
    }
    i = 0;
    while (i < list->visibleCount) {
        if (mnuListContainsFinalNode(list)) {
            if (i == 0) {
                if (list->cursor == list->last) {
                    return 0;
                }
                list->cursor = list->last;
                list->windowOffset = list->visibleCount - 1;
                return (s32)list->last;
            }
            break;
        }
        mnuAdvanceListWindowStart(list);
        i++;
        list->cursor = list->cursor->next;
        list->windowOffset += 1;
    }
    if (mnuListContainsFinalNode(list)) {
        list->windowOffset = list->visibleCount - 1;
        list->cursor = list->last;
        return (s32)list->last;
    }
    if (list->cursor == list->head) {
        list->cursor = list->cursor->next;
        list->windowOffset = 1;
    }
    mnuUpdateListScrollFlags((u8 *)list);
    return (s32)list->cursor;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8E30);

void mnuClearListFlagsOneAndTwo(u32 *flags) {
    *flags &= ~1;
    *flags &= ~2;
}

u32 mnuTestListFlagTwo(u32 *flags) {
    return *flags & 2;
}

/* Return the stored row step times the visible row count, in native units. */
s32 mnuGetListViewportHeight(MenuList *list);

/* Cancel the pending animation on every node in this list. */
void mnuResetListNodeFadeCounters(MenuList *list);

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(u8 *menu);

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg);

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry);



/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 index, MenuListNode *entry);



/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);


/* Destroy the owned list and optional sprite resources before freeing the window. */

void mnuSetWindowOverlaySprite(MenuWindowContainer *menu, u32 layout);


void mnuSetWindowContainerLayout(MenuWindowContainer *menu, u32 layout2C, u32 layout30, u32 layout34,
                                    u32 layout48, u32 layout38, u32 layout3C, u32 layout40,
                                    u32 layout4C);





void mnuCreateListWithDefaults(MenuWindowContainer *menu, u32 first, u32 second, u32 third, u32 fourth);

/* Clear only the window's panel-transition bit. */
void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *window);



void mnuRemoveWindowListCursorNode(MenuWindowContainer *menu);

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step);

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step);

void mnuAdvanceWindowListSelection(MenuWindowContainer *menu);

void mnuRetreatWindowListSelection(MenuWindowContainer *menu);

void func_002B9808(MenuWindowContainer *menu);

void func_002B9820(MenuWindowContainer *menu);


void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value, s32 res, s32 *idx, s32 unused);

/* Create an owned three-sprite bundle using the caller's slot-index array. */
MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    u32 resourceHandle, s32 *indices, u32 unused);

/* Destroy every native sprite slot, then release the bundle's allocation handle. */
void mnuReleaseWindowTextures(MenuIconSprites *menu);

void func_002B9A38(void);



void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, u32 option);

void mnuDrawWindowIconRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, s32 count, s32 option);

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option);



/* Draw at the selected row before advancing the panel's transition value.
 * DDS2's alternate selection flag overrides its ordinary selection flag. */
void mnuDrawWindowSelectionPanel(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 drawArg);

/* Draw the window, then advance its fade scale without a post-addition clamp. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, MenuWindowContainer *menu, s32 drawArg);

void func_002B9FB8(MenuWindowContainer *window);













extern void effInitializeSlotWork();


/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuClearEntryFlags(MenuIconState *group);

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
void mnuReleaseResourceList(MenuIconState *list);

typedef struct MenuPos {
    s32 x;
    s32 y;
} MenuPos;

typedef struct MenuPosTable {
    MenuPos pos[6];
} MenuPosTable;


typedef struct MenuPosTable3 {
    MenuPos pos[3];
} MenuPosTable3;

extern MenuPosTable3 D_0042AF48;

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 arg);

extern MenuPosTable D_0042AF60;

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 arg);

typedef struct MenuOffsets {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MenuOffsets;

extern MenuOffsets D_0042AF90;

void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 arg);

extern void mnuDrawIconPanelFade();

extern void mnuDrawIconRow6();

extern void mnuDrawIconPair();

/* Dispatch six DDS2 panel kinds; only kind four forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuIconState *panel, s32 selectionMode, s32 drawArg);

void func_002BA7A8(u32 x, u32 y, u32 depth, u32 fade, MenuIconState *panel, u32 selectionMode, u32 drawArg);

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg);


void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *obj);

/* Rebuild the first-node pointer by walking backward from the cursor. */
void mnuRebuildListFirstFromCursor(MenuList *list);

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(MenuList *list);

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(s32 *menu, s32 restoreCursor);

/* Relink and reindex the pointer array. Native endpoint writes require at
 * least two entries; zero/one-entry calls are not guarded here. */
void mnuLinkItemList(MenuListNode **items, s32 count);

/* Three-way comparison of unsigned primary keys, descending without subtraction. */
s32 mnuComparePrimaryKeyDescending(s32 *left, s32 *right);

/* Three-way comparison of unsigned primary keys, ascending without subtraction. */
s32 mnuComparePrimaryKeyAscending(s32 *left, s32 *right);

/* Three-way comparison of unsigned secondary keys, descending without subtraction. */
s32 mnuCompareSecondaryKeyDescending(s32 *left, s32 *right);

/* Three-way comparison of unsigned secondary keys, ascending without subtraction. */
s32 mnuCompareSecondaryKeyAscending(s32 *left, s32 *right);

/* Three-way comparison of unsigned tertiary keys, descending without subtraction. */
s32 mnuCompareTertiaryKeyDescending(s32 *left, s32 *right);

/* Three-way comparison of unsigned tertiary keys, ascending without subtraction. */
s32 mnuCompareTertiaryKeyAscending(s32 *left, s32 *right);

/* Sort the walked node pointers and rebuild the list from the cursor.
 * Nonzero ascending selects the last three comparators, not descending order.
 * Allocation uses the stored count; key bounds and the relinker's minimum count remain unchecked. */






void mnuSortItems(MenuList *menu, s32 keyIndex, s32 ascending);

/* Allocate four native fade records into pointer slots after the list header. */
void mnuAllocateListEntries(s32 *list);

extern void sdfReleaseChipBlock();

/* Free the four record blocks, not their nested window pointers. */
void mnuFreeListEntries(s32 *list);

/* Store the active word and window address in the header-selected slot.
 * Preserve the native below-five early return; it is not an upper capacity bound. */
void mnuAppendFadingWindowEntry(s32 activeValue, s32 windowAddress, s32 *list);

typedef struct MenuFadeEntry {
    u32 active;
    u32 pad4[3];
    void *window;
    u32 pad14;
} MenuFadeEntry;

/* Native removal only accepts indices at least five. Propagate the tail
 * record backward to the target, clearing each source's window pointer. */
void mnuRemoveFadingWindowEntry(s32 *list, u32 index);

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg);

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list);


/* Reset the original 0x98-byte prefix, then initialize the later fade fields. */



void mnuResetWindowFadeParameters(MenuFadeFields *menu);


typedef struct ScrollParams {
    s32 a;
    s32 b;
    s32 c;
} ScrollParams;

typedef struct ScrollInner {
    u8 unk0[0x20];
    ScrollParams *params;
} ScrollInner;

typedef struct ScrollHandle {
    u8 unk0[8];
    ScrollInner *inner;
} ScrollHandle;

/* Three animation handles at the tail of the 0x48-byte scroll panel. */
typedef struct MenuScrollPanel {
    u8 pad00[4];
    u32 color; /* 0x04: panel resource color */
    u32 firstSprite;
    u32 secondSprite;
    u8 pad10[4];
    MenuSpriteRef positions[3]; /* 0x14 */
    MenuSpriteRef active;       /* 0x2C */
    MenuSpriteRef pending;      /* 0x34 */
    ScrollHandle *handles[3];
} MenuScrollPanel;

extern ScrollHandle *effCreateStatusBatch(s32);

void mnuInitScrollHandles(MenuScrollPanel *menu);

void mnuReleaseScrollPanelAnimations();

MenuScrollPanel *mnuCreateScrollPanel(u32 owner);

void mnuDestroyScrollPanel(MenuScrollPanel *menu);


void mnuActivatePendingPanelResource(MenuScrollPanel *context);

void mnuConfigurePanelResource(MenuScrollPanel *menu, u32 model, u32 value, u32 color);

u8 mnuHasActivePanelResource(MenuScrollPanel *resources);



/* Three resource-slot handles at +0xE4/+0xE8/+0xEC. */
typedef struct MenuSlotEffectHandles {
    u8 pad00[0xE4];
    u32 handles[3];
} MenuSlotEffectHandles;

void mnuLoadPanelSectionResources(MenuSlotEffectHandles *slot, u32 model, u32 firstValue, u32 secondValue, s32 thirdValue
                                    );

void mnuReleasePartyPanelTextures(s32 menu);


void mnuResetPartyPanelFade(u8 *menu, s32 index, u32 unused, u32 preserve);

void func_002BB9C8(MenuSprites *page, u32 flags);

void mnuSetPageParams(MenuSprites *page, s32 mode);



extern s32 effDestroyResourceSlotSet();

extern void sdfReleaseChipBlock();


void mnuFreeIconSprites(MenuSprites *menu);

extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);


void mnuDrawIconRow(s32 unusedA, s32 unusedB, s32 depth, s32 skip, MenuSprites *set, s32 drawArg);

extern void *func_002BBA38();


void mnuSetWindowResource(s32 index, MenuPageWindow *menu, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

void mnuSetIndexedWindowPageSpriteFlags(s32 index, MenuPageWindow *menu, u32 first, u32 second);

void mnuClearEntries(MenuPageWindow *menu);

extern void itfSetGridEntryQuantizedAndRefresh();

typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

extern MenuIconLayout D_0042AFD8;

u32 mnuCreateIconBundle(u32 resource);

void mnuReleaseIconBundleAndSprites(MenuIconBundle *menu);


void mnuDrawFadeIcons(s32 x, s32 y, s32 depth, s32 unused, MenuIconBundle *obj, s32 drawArg);


void mnuAttachPartyIconBundle(s32 index, s32 menu, u32 resource);

void mnuReleasePartyIconBundles(u8 *menu);

s32 mnuPercentOrHundred(s32 value, s32 total);



void mnuReleasePartyPanelSpriteTextures(u8 *menu);



/* Copy eight resource handles into the window's primary handle bank. */
void mnuCopyPrimaryWindowHandles(MenuPageWindow *menu, u32 *source);

/* Copy eight resource handles into the window's secondary handle bank. */
void mnuCopySecondaryWindowHandles(MenuPageWindow *menu, u32 *source);

void mnuRegisterResourceHandles(MenuPageWindow *destination, u32 *source);







extern char D_00437C30[];

/* Create both party-panel lists and append their shared row labels. */
void mnuInitScrollLists(MenuPageWindow *menu, s32 *counts);

void mnuDestroyWindowOwnedLists();

void mnuRebuildScrollLists(u32 context, u32 counts);

typedef struct MenuSlotWindow {
    u8 unk0[0xC0];
    u32 unkC0;
    u8 unkC4[0x110 - 0xC4];
    u32 unk110;
    u8 unk114[0x2138 - 0x114];
} MenuSlotWindow;

typedef struct MenuWindowSet {
    u32 flags;
    u8 unk4[0x14];
    MenuSlotWindow slots[5];
    u8 unkA630[0xA698 - 0xA630];
    s32 selected;
} MenuWindowSet;

void mnuClearPageSelection(MenuWindowSet *set);




void mnuFreeWindowSprites(MenuPageSlot *win);

void mnuShutdownContext(u8 *ctx);


void mnuResolveUnselectedPageHandles(MenuPageWindow *window);

void mnuRefreshPageHandles(MenuPageWindow *window);



void mnuSelectPage(MenuPageWindow *window, s32 selected);

void mnuClearPageSelectionHandles(MenuPageWindow *window);

void mnuFlagActiveWindows(MenuPageWindow *window);

void mnuClearPartyPanelActiveFlags(MenuPageWindow *window);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BE0);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BE8);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BF0);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BF8);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C00);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C08);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C10);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C18);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C20);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C28);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C30);

