#include "mnu_list.h"
#include "fpu.h"
#include "dat_state.h"
#include "eff.h"
#include "mnu_shop.h"

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
#define MNU_STAFF_PARTY_ENTRY_BYTES 0x1C4
#define MNU_STAFF_PARTY_HALFWORD_STRIDE 0xE2
#define MNU_STAFF_PARTY_BASE 0xA60
#define MNU_STAFF_BACKUP_BYTES 0x8D4
#define MNU_STAFF_PARTY_ACTIVE_BIT 1
#define MNU_STAFF_NODE_UNAVAILABLE 1
#define MNU_STAFF_NODE_SELECTED 2
#define MNU_STAFF_PARTY_PANEL_BASE 0x284
#define MNU_STAFF_PARTY_SLOTS_BASE 0xA928
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


extern void *sdfAllocAndClearQuadwords(s32);
extern void *sdfAllocSizeClassBlock(s32);
extern s32 mdlFlagTest(s32);
extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *, s32, s32, s32, s32, s32);
extern struct EffectSlotSet *effCreateResourceSlotSet(u32 *, u32, u32);

extern s32 dspStartEntry(s32 entry);
extern s32 D_00435E5C;

extern void itfDrawGridWithResolvedSlot();

extern void mnuClearListFlagsOneAndTwo();

extern s32 func_002C6CE8(void);

extern s32 func_002C6480();

extern char D_003E75C4[];

extern void mnuDrawWindowDecorations(s32, s32, s32, MenuWindowContainer *, s32);

extern struct MenuListNode *func_002B86E8(struct MenuList *);

extern void func_002AAE80();

extern void mnuReleasePartyIconBundles();

extern void mnuClearEntries();


extern s32 D_00435E54;

extern u32 effMiscRand();

extern u32 evtStageTestCountFlags();

extern s32 evtStageTestHasPendingMotion();

extern void evtStageTestQueueMotion();

extern s32 D_00435E48;

extern void mnuApplyPackedGroupValues();

extern void func_002C0D18();

extern void func_002C10F0();

extern void mnuDrawSlotIcons();

extern void frFontAddSharedGlyphFlags();

extern s32 frFontAppendGlyphFromData();

extern void frFontSetContextPair();

extern void frFontStoreShiftedContextValue();

extern void frFontSetChildColors();

extern void frFontClearFlagBits();

extern char D_003E7588[];

extern char D_00380788[];

extern void evtStageTestUpdate();

extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);

extern void mnuClearActionFlags();

extern char D_003E7790[];

extern char D_003E7720[];

extern char D_003E7774[];

extern char D_003E773C[];

extern void mnuHandlePanelListPageJumpInput();

extern void mnuStepPartyPanelListFromInput();
extern char D_003E7758[];

extern s32 ptySkillMenuApplyFieldUseAndCost();

extern u32 mnuMapPadMaskToFlags();

extern s32 mnuGetAbilityByteCategory();


extern void mnuPlayInputSound(s32, s32, u32 *);

extern u32 D_003E7828[];

extern const CampEffectRows D_003E7858;

extern char *D_003E7818[];

extern char *D_003E7820[];

extern u32 effLoadIndexedResource(char *, char *, s32);

extern u32 effLoadMappedResource(char *, char *);

extern void effRequestResourceByMode(char *, char *, s32, u32 *);

extern void effRequestMappedResource(char *, char *, u32 *);

extern void mnuFreeWindowSprites();

extern void mnuHideIconGroup();

extern s32 evtGetCapturedWindowPanelValue();


extern void mnuBeginWindowFadeTransition(s32, s32);

extern void mnuInitPartyPanelSlots();

extern s32 func_002B06A8();

extern void mnuPrepareStaffValueChangeDialog();

extern char D_003E7530[];

extern void func_002AAC70();

extern s32 D_00435E6C;

extern void mnuDrawCampIconBackdropByKind();

extern s32 effHasFirstTextureHandle();

extern void effReleaseTextureHandlesAndResetSlots();

extern void func_002AAC98();

extern char D_003E69B0[];

extern void mnuCreateStaffImageSprite();

extern void func_002AA7A0();

extern void mnuUpdateAndDrawWindowTransition(s32, s32, s32, s32, s32);

extern void mnuIdleVoiceTimer();

extern void func_002B2408();

extern u32 mnuCreateIconBundle(u32);

extern u32 func_002B9FF8();


extern void mnuDrawIconPanel(s32, s32, s32, s32, MenuIconState *, s32, s32);

extern void mnuHideWindowHandlesKindFourFive(MenuIconState *);

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);
extern char D_003E75E0[];
extern char D_003E75A8[];

extern void sndSetSequenceVolumePan();

extern void mnuSelectPage(MenuPageWindow *window, s32 selected);

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
    u8 pad00[0x54];
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    s32 displayHandle;     /* 0x60 */
    s32 resourceHandle;    /* 0x64 */
    void *displayResource; /* 0x68 */
    s32 alternateResource; /* 0x6C: used when swapping the staff panel view */
    u8 pad70[0x58];
    s32 labelHandle;       /* 0xC8 */
    u8 padCC[0x38];
    s32 imageHandle;       /* 0x104 */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    MenuPageWindow partyWindow; /* 0x284: lists, page slots and selection */
    s32 partyPanelActive;  /* 0xA928 */
    s32 partyPanelLast;    /* 0xA92C */
    u8 padA930[0x104];
    s32 panelGroup;        /* 0xAA34 */
    s32 panelRequest;      /* 0xAA38 */
    s32 panelEffects;      /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    s32 resourceList;      /* 0xAA5C */
} MenuContext;

extern void mnuReleaseSpriteTextures(s32);

extern s32 kwlnTaskGetUserValue();

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
    u8 panelSnapshots[5][0xA0]; /* 0x1A90: copied panel subrecords */
    s32 fadeA;                 /* 0x1DB0 */
    s32 fadeB;                 /* 0x1DB4 */
    u8 pad1DB8[0x20];
    s32 freezePanel; /* 0x1DD8: set on transition; skips the panel update */
} PartyMenuData; /* 0x1DDC: native party-selection allocation */

/* Byte-offset copies keep their field displacement tied to the owner layout. */
#define PARTY_BACKUP_OFFSET ((s32)&((PartyMenuData *)0)->backup)

/* Staff/skill-menu work prefix, separate from the party-selection allocation. */
typedef struct MenuPartyRuntime {
    u8 pad00[0x10];
    s32 staffMode; /* 0x10 */
    s32 staffView; /* 0x14: selects the input mask and panel view */
    s32 staffSelection; /* 0x18: chosen list entry */
    s32 staffExit; /* 0x1C: signals the close transition */
    u8 pad20[4];
    MenuWindowContainer *selectedWindow; /* 0x24 */
    s32 activeMark; /* 0x28 */
    u8 pad2C[8];
    u32 selectedIndex; /* 0x34: compared with MenuListNode.index */
} MenuPartyRuntime;

/* The idle-motion timer shares the party work area with other menu states. */
typedef struct MenuVoiceState {
    u8 pad00[0x28];
    s32 idleFrames;
    s32 motionSelection; /* -1 enables periodic motion */
} MenuVoiceState;


extern void ptyRecomputeMaxHpMp();

extern void scrClearSecondaryScriptFlag();

extern void func_0019D550(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);

extern void func_0035B7F8(MenuListNode **, s32, s32, s32 (*)(MenuListNode **, MenuListNode **));

extern s32 sdfAllocGeneralBlock(s32);

extern s32 *sdfResourceRetainAddress(s32);

extern void func_0026C900(void);

extern void mnuDestroyWindowContainer(MenuWindowContainer *);

extern void *memset(void *, s32, u32);

MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    u32 resourceHandle, s32 *indices, u32 unused);

typedef struct MenuListDefaults {
    s32 indices[3];
} MenuListDefaults;

extern MenuListDefaults D_0042AF00;


extern void mnuDrawStaffPartySelectionPanel(s32);



s32 mnuAdvanceStaffValuePopup(s32 callback);

s32 mnuFinishStaffValuePopup(s32 callback);

/* Initialize the menu display using a value from the resource chain. */
u32 mnuEnterSelectedResourceLabel(void);

u32 func_002B06A0(void);



s32 mnuUpdatePartySlotAssignmentPopup(s32 callback);

s32 mnuStartPanelDispatch(s32 callback);

s32 mnuStartPanelExit(s32 callback);

/* Clear the selected item's five stat bonuses and its requirement count,
 * then recalculate the party record's maximum HP and MP. */
void mnuClearPartySelectionValues(DatPartyRecord *entry, s32 selection);

u32 mnuEnterSlotLabel(void);

u32 func_002B0B88(void);

s32 mnuPartySlotConfirmClearUpdate(s32 callback);

s32 mnuAdvancePartyClearPopup(s32 callback);

s32 mnuFinishPartyClearPopup(s32 callback);

u32 func_002B0D48(void);

void func_002B0D50(u32 context);

void func_002B0D70(u32 callback);

s32 mnuIsFinalItemIndex(s32 index, s32 item);





void mnuDestroyPartySelectionWindow(s32 context);


/* Snapshot the five party entries and cap the menu's displayed slot count. */
void mnuCopyPartyEntries();



extern void mnuAssignSelectedPartyEntry();
extern void func_002BCAB0();

/* Notify active snapshot entries, restore the backup, then refresh panel resources.
 * The second loop counts down while the backup byte offset advances forward. */
void mnuRestorePartyEntriesAndRefresh();

/* Count active entries in the five-slot party array, capped at three. */
s32 mnuCountActiveSlots(void);

/* Reset selection/backup state, activate all panel slots, and enable list nodes.
 * Keep the native short-arity snapshot call unchanged. */
void mnuClearPartySelectionAndActivateSlots(s32 context);

/* Release panel textures before reinitializing slots and updating handle state. */
void mnuRefreshPartyPanelSlots(s32 context);



u32 mnuReleasePartySelectionResources(void);

void mnuPreparePartyPanelTransition(s32 menu);





/* Two-stage fade: B rises first when opening, A falls first when closing.
 * The second ramp begins strictly past its threshold; clamps follow each step. */
void mnuUpdateStaffFade(s32 opening, PartyMenuData *menuWork);



/* Update the final-row flag, draw the panel, and suppress its contents update
 * once the party transition has frozen it. */
s32 mnuOpenStaffPartySelectionPanel(s32 callback);

s32 mnuStepPartySelectionControl(s32 callback);

u8 mnuIsStateNotOne(void);

void func_002B27F0(u32 context);

void mnuReleaseMenuWindowHandles(s32 context);

/* Release both staff resource slots; their menu indices differ between games. */
void mnuReleaseStaffMenuResources(s32 menuWork);

/* Reset texture handles for the same two resource slots. */
void mnuReleaseStaffMenuTextureHandles(s32 menuWork);

u32 mnuCreateSelectState(u32 unused, s32 flag);

s32 mnuStaffCloseSelectionState(void);

void func_002B29C8(u32 context);

void func_002B29E0(void);

void func_002B29F8(u32 context);

void func_002B2A10(void);

/* Update the popup first; accept confirm/cancel only while its state is zero.
 * Return a nonzero popup-update word unchanged, or zero after handling input. */
s32 mnuStaffPopupUpdate(s32 callback);

s32 mnuDrawStaffCampPageWithImage(s32 callback);

s32 mnuStepStaffCampPageControl(s32 callback);



extern void mnuSetWindowResource(s32 index, u32 *menu, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
extern void mnuAttachPartyIconBundle(s32 index, s32 menu, u32 resource);
extern s32 mnuCreatePanelGroup(s32 owner, s32 texture, s32 mode);
extern s32 mnuCreateSpriteState(s32, s32, s32);
extern s32 mnuAllocateSimpleSprite(s32, s32, s32);
extern s32 mnuCreateProfilePanel(s32 source);
extern void mnuSetGroupProperties(s32, s32, s32, s32, s32);
extern s32 mnuClassifyQuarterHalfPercent(s32 amount, s32 divisor);
extern void evtStageTestSelectEntry(s32, s32, s32);
extern void func_002B2C88(s32, s32, s32, s32);

s32 mnuCreatePanels(s32 callback);

s32 mnuDestroyPanels(s32 callback);

void mnuResetSelectedPanelOpacity(s32 context);

/* Switch the party page, rebuilding its panels; previous takes priority.
 * The opaque request argument is forwarded unchanged. Return one if switched. */
s32 mnuStaffSwitchPartyPage(s32 requestArgument);

/* Update popup state before page navigation, view toggles, and exit requests.
 * stateWord holds the update result first, then the stored popup state. */
s32 mnuStaffBrowsePartyUpdate(s32 callback);

void mnuDrawSlotIcons(s32 x, s32 context);

void mnuDrawSelectedPartySlotMarkers(s32 context, u32 *handles);

void mnuDrawTextSprite(s32 x, s32 y, s32 width, u32 color, s32 model, s32 flags);

void mnuDrawPartySkillAndStatusPanel(DatPartyRecord *entry, s32 id, s32 packedGroup, s32 group, s32 unused, s32 spriteFlags);

void mnuDrawProfilePanelAndSprite(DatPartyRecord *entry, u32 unused1, u32 group, u32 resource,
                                    u32 unused4, u32 spriteFlags);



void mnuIdleVoiceTimer(s32 object);

s32 mnuStaffIdlePartyUpdate(s32 callback);

u32 func_002B3A58(void);







u32 mnuDestroySelectedPartyWindow(u32 callback);

/* Seek the first node with a nonzero sort key and no unavailable flag.
 * Leave the cursor unchanged if no such node exists. */
void mnuSeekFirstAvailableStaffListNode(void);

MenuWindowContainer *mnuSeekSelectedWindowCursor(s32 selectionMode, s32 callback);

void func_002B4270(u32 context);

void func_002B4290(s32 context);





typedef struct SkillInfo {
    u8 pad00[0x20];
    u32 count;
    u16 codes[14];
} SkillInfo;

extern void func_00315388(u16, SkillInfo *);

u32 *mnuBuildOwnedSkillBits(void);

void func_002B47F8(void);

s32 mnuIsSkillCodeInBitset(s32 skillCode, u32 *bits);



void mnuDestroySkillMenuWindows(s32 context);

u32 mnuCreateItemState(s32 callback);

s32 mnuCloseItemSelectionState(s32 selection);



s32 mnuCampMenuDrawSlotLabel(s32 callback);

s32 mnuStepSkillSlotControl(s32 callback);

void mnuClearSelectedListNodeId();

u32 mnuHasSelectedListNodeId(s32 callback);

/* Set the selected flag only on nodes whose index matches the saved selection. */
void mnuHighlightSelectedListNode();



u32 mnuClearSkillSelectionImageState(void);

/* Narrow the ID to its native 16-bit skill code before duplicate detection.
 * Insert only missing skills, then recompute maxima and clear the script flag. */
void mnuAddPartySkillIfMissing(DatPartyRecord *partyEntry, s32 skillId, s32 skillSlot);

/* Clear one skill slot, retaining the native short-arity maxima recomputation. */
void mnuClearPartySkillSlot(DatPartyRecord *partyEntry, s32 skillSlot);

/* Open the selected skill's popup or cancel, then process list navigation.
 * Native list reads precede the late window guard; preserve that ordering. */
void mnuCampMenuHandleInput(s32 callback);



void mnuSwapPartySkillSlots(DatPartyRecord *party, s32 firstSlot, s32 secondSlot);

/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
void ptySkillMenuHandleSlotReorder(s32 callback);

s32 ptySkillMenuUpdate(s32 callback);

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

void ptySkillMenuCopyPageState(s32 context);

s32 ptySkillMenuEnterPage(s32 callback);

s32 ptySkillMenuDispatchPageRequest(s32 callback);

s32 ptySkillMenuApplyFieldUseAndCost();

/* Mark entries whose field-use cost cannot be paid by the selected party member. */
void mnuFlagMatchingEntries(s32 context);

s32 ptySkillMenuHandleFieldUse(s32 callback);

s32 ptySkillMenuOpenSelectedSkillPage(s32 callback);

s32 ptySkillMenuDispatchConfirmRequest(s32 callback);

s32 ptySkillMenuOpenPartyPage(s32 callback);

u32 ptySkillMenuClosePartyPage(u32 callback);

s32 ptySkillMenuHandlePageSwitch(s32 callback);

/* Shift a selected child window's list to the requested row. Keep the
 * double-dereferenced cursor load raw: the typed form does not match. */
void mnuSeekSelectedWindowRow(s32 menu, s32 target);

s32 ptySkillMenuBrowseCandidatePages(s32 callback);

s32 mnuOpenSkillDetailPanel(s32 callback);

s32 func_002B6800(s32 callback);

void mnuDrawSelectionLabel(u16 id);






/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(DatPartyRecord **out, s32 max);

s32 mnuHasAvailableSlotResource(s32 id);



u32 mnuDestroySkillSelectionWindow(u32 callback);

u32 mnuResetSelection(u32 callback);

u32 mnuCloseSkillSelection(u32 callback);

s32 mnuUpdateSkillListInput(s32 callback);



void func_002B7588(s32 context);

s32 mnuCampMenuDrawStatus(s32 callback);

s32 func_002B76B0(s32 callback);


void mnuInitializeMapPacket(u32 value, u32 *values, s32 count, MapPacket *packet);

void mnuOrEntryFlags(u32 flags, u32 *entryFlags);

void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);

void mnuSetCampEffectResourceHandles(u32 first, u32 second, MenuEffectResources *);


void mnuBindCampEffectAnimation(MenuEffectResources *);































void mnuLoadEffectResources(MenuEffectResources *);

void mnuRequestEffectResources(MenuEffectResources *);

u32 mnuBindCampEffectWhenLoaded(MenuEffectResources *);

void mnuDestroyEffectResources(MenuEffectResources *);


void mnuSpawnSpark(MenuCampEffect *fx);

void mnuRetireCampSpark(MenuCampEffect *, s32 index);

void mnuDrawAndAdvanceCampSparks(MenuCampEffect *fx, s32 arg);



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

void mnuDrawBadgeFade(MenuCampEffect *set, s32 arg);

extern MenuBadgeLayout D_0042AEE8;

void mnuDrawCampIconBackdrop(MenuCampEffect *set, s32 arg);

void mnuEnableCampBadgeFade(u32 *flags);

MenuList *mnuCreateListState(u32 owner, u32 visibleCount, s32 rowSpacing);

u32 mnuDestroyListState(MenuList *list);

void mnuUpdateListScrollFlags(MenuList *list);

MenuListNode * mnuListAppendNode();

s32 mnuListContainsFinalNode(MenuList *list);





typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;


void mnuSetGridSpriteSlot(MenuListNode *node, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect);

void *mnuWalkNodeList(s32 index, MenuList *list);

s32 mnuSeekListNode(s32 index, MenuList *list);

void mnuSelectFirstListNode(MenuList *list);

void mnuSelectLastListNode(MenuList *list);

s32 mnuAdvanceListWindowStart(MenuList *list);

/* Step the visible head back one node when a full window follows it. */
s32 mnuRetreatListWindowStart(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);

struct MenuListNode *mnuAdvanceListCursorDefault(u32 list);

struct MenuListNode *mnuRetreatListCursorDefault(u32 list);

s32 mnuScrollListToEnd(MenuList *list);



void mnuClearListFlagsOneAndTwo(u32 *flags);

u32 mnuTestListFlagTwo(u32 *flags);

/* Return the stored row step times the visible row count, in native units. */
s32 mnuGetListViewportHeight(MenuList *list) {
    return list->rowStep * list->visibleCount;
}

/* Cancel the pending animation on every node in this list. */
void mnuResetListNodeFadeCounters(MenuList *list) {
    s32 node;

    node = (s32)list->first;
    if (node != 0) {
        ((MenuListNode *)node)->animationTimer = 0;
        while (node = (s32)((MenuListNode *)node)->next, node != 0) {
            ((MenuListNode *)node)->animationTimer = 0;
        }
    }
}

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(u8 *menu) {
    u8 *node = (u8 *)((MenuList *)menu)->first;
    if (node != NULL) {
        do {
            s32 timer = ((MenuListNode *)node)->animationTimer;
            s32 reduced = timer - MNU_NODE_FADE_STEP;
            if (timer > 0) {
                ((MenuListNode *)node)->animationTimer = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                ((MenuListNode *)node)->animationTimer = 0;
            }
            node = (u8 *)((MenuListNode *)node)->next;
        } while (node != NULL);
    }
}

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg) {
    u32 spriteIndex = 0;
    do {
        s32 selected = node == list->cursor;
        s32 index = selected * MNU_ENTRY_SPRITE_COUNT + spriteIndex;
        u32 sprite = node->sprites[index].sprite;
        if (sprite != 0) {
            itfDrawGridWithResolvedSlot(x, y, depth, 0, sprite, node->sprites[index].effect, drawArg);
        }
        spriteIndex++;
    } while (spriteIndex < MNU_ENTRY_SPRITE_COUNT);
}

extern u32 uiBlendColors(u32 color, u32 previous, s32 blend);

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry) {
    u32 flags = entry->flags48;
    u32 color = MNU_ENTRY_MARKED_COLOR;
    if (!(flags & 1)) {
        color = (flags & 4) ? MNU_ENTRY_ALTERNATE_COLOR : MNU_ENTRY_DEFAULT_COLOR;
    }
    return uiBlendColors(color, previousColor, entry->animationTimer);
}



/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 index, MenuListNode *entry) {
    s32 colorIndex;

    for (colorIndex = 0; colorIndex < MNU_ENTRY_COLOR_COUNT; colorIndex++) {
        u32 previousColor = menu->workEntries[index].cornerColors[colorIndex];

        menu->workEntries[index].cornerColors[colorIndex] = mnuBlendListNodeColorByFlags(previousColor, entry);
    }
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002B9218);

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg) {
    func_002B9218(x, y, depth, 0, 0, MNU_FULL_FADE, 0, menu, drawArg);
}

/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
s32 mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 visibleCount, s32 rowSpacing) {
    MenuWindowContainer *window = (MenuWindowContainer *)sdfAllocAndClearQuadwords(MNU_WINDOW_CONTAINER_BYTES);
    MenuList *list;
    window->width = width;
    window->height = height;
    window->id = id;
    list = mnuCreateListState(id, visibleCount, rowSpacing);
    window->state = 0;
    window->list = list;
    return (s32)window;
}

/* Destroy the owned list and optional sprite resources before freeing the window. */
void mnuDestroyWindowContainer(MenuWindowContainer *menu) {
    s32 resource;

    mnuDestroyListState(menu->list);
    resource = (s32)menu->resource;
    if (resource != 0) {
        mnuReleaseWindowTextures(resource);
    }
    sdfReleaseChipBlock(menu);
}

void mnuSetWindowOverlaySprite(MenuWindowContainer *menu, u32 layout) {
    menu->decorations[2].sprite = layout;
}

void mnuSetWindowContainerState(MenuWindowContainer *menu, u32 state) {
    menu->state = state;
}

void mnuSetWindowContainerLayout(MenuWindowContainer *menu, u32 layout2C, u32 layout30, u32 layout34,
                                    u32 layout48, u32 layout38, u32 layout3C, u32 layout40,
                                    u32 layout4C) {
    menu->decorations[0].sprite = layout2C;
    menu->decorationX[2] = layout4C;
    menu->decorations[0].parameter = layout30;
    menu->decorations[1].sprite = layout34;
    menu->decorations[1].parameter = layout38;
    menu->decorationX[1] = layout48;
    menu->decorations[2].sprite = layout3C;
    menu->decorations[2].parameter = layout40;
    menu->decorationX[0] = 0;
}

void mnuInitializeBasicWindowLayout(MenuWindowContainer *menu, u32 first, u32 second) {
    mnuSetWindowContainerLayout(menu, first, second, 0, 0, 0, 0, 0, 0);
}

void mnuSetWindowEntryParameters(u32 first, MenuWindowContainer *menu, u32 second, u32 third, u32 fourth) {
    menu->field1C = first;
    menu->sprite20 = second;
    menu->param24 = third;
    menu->param28 = fourth;
}


/* Copy the native panel layout, override its bounds, and mark its transition flag. */
void mnuSetWindowPanelBounds(MenuWindowContainer *panel, const void *layout, u32 left, u32 top,
                   u32 right, u32 bottom) {
    memcpy(&panel->panel, layout, MNU_PANEL_LAYOUT_BYTES);
    panel->panel.left = left;
    panel->panel.top = top;
    panel->panel.right = right;
    panel->panel.bottom = bottom;
    panel->flags |= MNU_WINDOW_TRANSITION_FLAG;
}

void mnuCreateListWithDefaults(MenuWindowContainer *menu, u32 first, u32 second, u32 third, u32 fourth) {
    MenuListDefaults defaults = D_0042AF00;
    menu->resource =
        mnuCreateWindowSpriteResources(first, second, third, fourth, defaults.indices, 3);
}

/* Clear only the window's panel-transition bit. */
void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *window) {
    window->flags = window->flags & MNU_WINDOW_TRANSITION_CLEAR_MASK;
}

MenuListNode *mnuAppendWindowListNode(MenuWindowContainer *menu, s32 value) {
    return mnuListAppendNode((u32)menu->list, value);
}

extern MenuListNode *func_002B83A0(MenuList *list, MenuListNode *anchor, s32 value, s32 mode, u32 options);

/* Insert `value` beside anchor in the window's list (see func_002B83A0). */
MenuListNode *func_002B9708(MenuWindowContainer *menu, MenuListNode *anchor, s32 value, s32 mode, u32 options) {
    return func_002B83A0(menu->list, anchor, value, mode, options);
}

void func_002B9720(MenuWindowContainer *menu) {
    func_002B86E8(menu->list);
}

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step) {
    MenuListNode *selected = mnuListAdvanceCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuHideIconGroup(&menu->panel);
    }
    return selected;
}

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step) {
    MenuListNode *selected = mnuListRetreatCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuHideIconGroup(&menu->panel);
    }
    return selected;
}

void mnuAdvanceWindowListSelection(MenuWindowContainer *menu) {
    mnuAdvanceListSelection(menu, 0);
}

void mnuRetreatWindowListSelection(MenuWindowContainer *menu) {
    mnuReverseListSelection(menu, 0);
}

void func_002B9808(MenuWindowContainer *menu) {
    mnuClearListFlagsOneAndTwo((u32)menu->list);
}

void func_002B9820(MenuWindowContainer *menu) {
    mnuTestListFlagTwo((u32)menu->list);
}


void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value, s32 res, s32 *idx, s32 unused) {
    obj->value = value;
    obj->sprite[0] = effCreateResourceSlotSet((u32 *)res, idx[0], 1);
    obj->sprite[1] = effCreateResourceSlotSet((u32 *)res, idx[1], 1);
    obj->sprite[2] = effCreateResourceSlotSet((u32 *)res, idx[2], 1);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[0], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[1], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[2], 0, w, h, w, h);
}

/* Create an owned three-sprite bundle using the caller's slot-index array. */
MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    u32 resourceHandle, s32 *indices, u32 unused) {
    u32 allocationHandle = sdfAllocGeneralBlock(0x18);
    MenuIconSprites *bundle = (MenuIconSprites *)sdfResourceRetainAddress(allocationHandle);
    memset(bundle, 0, 0x18);
    bundle->handle = allocationHandle;
    mnuInitIconSprites(bundle, width, height, value, resourceHandle, indices, unused);
    return bundle;
}

/* Destroy every native sprite slot, then release the bundle's allocation handle. */
void mnuReleaseWindowTextures(MenuIconSprites *menu) {
    u32 spriteIndex = 0;
    do {
        effDestroyResourceSlotSet(menu->sprite[spriteIndex]);
        spriteIndex++;
    } while (spriteIndex < MNU_WINDOW_RESOURCE_SPRITES);
    sdfReleaseResourceAllocation(menu->handle);
}

void func_002B9A38(void) {
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002B9A40);

void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, u32 option) {
    func_002B9A40(x - 0xf0, y - 8, flags, window->state,
                                (u32)window->list, (u32)window->resource, option);
}

void mnuDrawWindowIconRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, s32 count, s32 option) {
    s32 i;
    s32 sprite = window->sprite20;
    s32 state = window->state;
    s32 field = window->field1C;
    if (sprite != 0) {
        if (field == 0) {
            func_00306CD0(x - 0xD0, y - 0xB8, flags, state, 1, sprite, window->param28, option);
        }
        for (i = 0; i < count; i++) {
            func_00306CD0(x + window->originX, i * window->list->rowStep + y + window->originY, flags, state, 1,
                          window->sprite20, window->param24, option);
        }
    }
}

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option) {
    mnuDrawWindowIconRows(x, y, flags, window, window->list->visibleCount, option);
}

/* Draw the three optional decoration sprites at their individual X offsets. */
void mnuDrawWindowDecorations(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 option)
{
    u32 i;
    u32 state = window->state;

    for (i = 0; i < 3; i++) {
        u32 sprite = window->decorations[i].sprite;
        u32 parameter = window->decorations[i].parameter;
        u32 offset = window->decorationX[i];

        if (sprite != 0) {
            func_00306CD0(x + offset + 0xC0, y - 0xB8, depth, state,
                         1, sprite, parameter, option);
        }
    }
}

/* Draw at the selected row before advancing the panel's transition value.
 * DDS2's alternate selection flag overrides its ordinary selection flag. */
void mnuDrawWindowSelectionPanel(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 drawArg) {
    MenuList *list;
    s32 selectionMode;
    s32 fadeScale = window->state;

    if (window->panel.sprite[0] != NULL) {
        list = window->list;
        selectionMode = 0;
        if (list->stateFlags & MNU_LIST_SELECTION_FLAG) {
            selectionMode = 1;
        }
        if (list->stateFlags & MNU_LIST_ALTERNATE_SELECTION_FLAG) {
            selectionMode = 2;
        }
        y += list->windowOffset * list->rowStep;
        mnuDrawIconPanel(x, y, depth, fadeScale, &window->panel, selectionMode, drawArg);
        mnuHideWindowHandlesKindFourFive(&window->panel);
        if (window->flags & MNU_WINDOW_TRANSITION_FLAG) {
            window->panel.fade += MNU_NODE_FADE_STEP;
            if (window->panel.fade >= MNU_PANEL_FADE_LIMIT) {
                window->panel.fade = MNU_PANEL_FADE_LIMIT;
            }
        } else {
            window->panel.fade += MNU_WINDOW_FADE_STEP;
            if (window->panel.fade >= MNU_PANEL_FADE_THRESHOLD) {
                window->panel.fade = MNU_FULL_FADE;
            }
        }
    }
}

/* Draw the window, then advance its fade scale without a post-addition clamp. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, MenuWindowContainer *menu, s32 drawArg) {
    s32 fadeScale = menu->state;
    s32 value;

    menu->list->scale = fadeScale;
    mnuDrawWindowDecorations(x, y, depth, menu, drawArg);
    mnuDrawVisibleWindowIconRows(x, y, depth, menu, drawArg);
    if (menu->list->count != 0) {
        mnuDrawWindowSelectionPanel(x, y, depth, menu, drawArg);
    }
    func_002B9218(x, y, depth, menu->width, menu->height,
                  fadeScale, menu->flags, (s32)menu->list, drawArg);
    if (menu->resource != 0) {
        mnuDrawWindowResourceSpriteRows(x, y, depth, menu, drawArg);
    }
    value = menu->state;
    if (value < MNU_FULL_FADE) {
        menu->state = value + MNU_WINDOW_FADE_STEP;
    }
    menu->flags |= MNU_WINDOW_TRANSITION_FLAG;
}

void func_002B9FB8(MenuWindowContainer *window) {
    s32 remaining;

    remaining = window->list->count;
    if (0 < remaining) {
        do {
            remaining = remaining - 1;
        } while (remaining != 0);
    }
}

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AE90);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AEA0);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AED0);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AEE8);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AF00);

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002B9FF8);

extern void effInitializeSlotWork(s32, s32);


/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuHideIconGroup(MenuIconState *group) {
    s32 spriteIndex;
    if (group->sprite[0] != NULL && group->kind < MNU_PANEL_KIND_LIMIT) {
        for (spriteIndex = 0; spriteIndex < group->count; spriteIndex++) {
            EffectSlotSet *obj = group->sprite[spriteIndex];
            u32 *flags = &obj->workEntries->states[0].flags;
            *flags &= ~1;
            effInitializeSlotWork((s32)obj, 0);
        }
    }
}

typedef struct ResourceList {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ s32 count;
    /* 0xC */ u32 items[1];
} ResourceList;

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
void mnuReleaseResourceList(ResourceList *list) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < list->count; slotIndex++) {
        if (list->items[slotIndex] != 0) {
            effDestroyResourceSlotSet(list->items[slotIndex]);
        }
    }
    sdfReleaseChipBlock(list);
}

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

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 arg) {
    MenuPosTable3 table = D_0042AF48;
    s32 width = state->sprite[0]->workEntries->sourceWidth << 4;
    s32 i;
    s32 a;
    if (state->fade > 0x100) {
        alpha = 0x200 - state->fade;
    }
    for (i = 0; i < state->count; i++) {
        if (i == 0 && mode != 1) {
            a = state->fade < 0x100 ? alpha : 0x100;
        } else {
            a = alpha;
        }
        a = i == 2 ? 0x100 : a;
        if (i == 2 && mode == 2) {
            continue;
        }
        func_00306CD0(x + table.pos[i].x, y + table.pos[i].y, z, a, 1, state->sprite[i], 0, arg);
    }
    state->sprite[0]->workEntries->width = width;
    if (state->count >= 3) {
        state->sprite[2]->workEntries->width = width;
    }
}

extern MenuPosTable D_0042AF60;

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 arg) {
    MenuPosTable table = D_0042AF60;
    s32 i;
    for (i = 0; i < state->count; i++) {
        if (i != 3 && i != 5) {
            func_00306CD0(x + table.pos[i].x, y + table.pos[i].y, z, w, 1, state->sprite[i], 0, arg);
        }
    }
}

typedef struct MenuOffsets {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MenuOffsets;

extern MenuOffsets D_0042AF90;

void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 arg) {
    MenuOffsets offset = D_0042AF90;
    func_00306CD0(x + offset.x0, y + offset.y0, z, w, 1, state->sprite[0], 0, arg);
    func_00306CD0(x + offset.x1, y + offset.y1, z, w, 1, state->sprite[1], 0, arg);
}

extern void mnuDrawIconPanelFade();

extern void mnuDrawIconRow6();

extern void mnuDrawIconPair();

/* Dispatch six DDS2 panel kinds; only kind four forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuIconState *panel, s32 selectionMode, s32 drawArg) {
    switch (panel->kind) {
    case 0:
    case 1:
    case 2:
    case 3:
        mnuDrawIconPanelFade(x, y, depth, fade, panel, selectionMode, drawArg);
        return;
    case 4:
        mnuDrawIconRow6(x, y, depth, MNU_FULL_FADE, panel, drawArg);
        return;
    case 5:
        mnuDrawIconPair(x, y, depth, fade, panel, drawArg);
        break;
    }
}

void func_002BA7A8(u32 x, u32 y, u32 depth, u32 fade, MenuIconState *panel, u32 selectionMode, u32 drawArg) {
    mnuDrawIconPanel(x, y, depth, fade, panel, selectionMode, drawArg);
}

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg) {
    func_002BA7A8(x, y, depth, MNU_FULL_FADE, panel, 0, drawArg);
}


void mnuHideWindowHandlesKindFourFive(MenuIconState *obj) {
    switch (obj->kind) {
    case 4:
        itfGridLookupValueOrDefault(obj->sprite[2], 0);
        itfGridLookupValueOrDefault(obj->sprite[3], 0);
        itfGridLookupValueOrDefault(obj->sprite[4], 0);
        itfGridLookupValueOrDefault(obj->sprite[5], 0);
        return;
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 5:
        itfGridLookupValueOrDefault(obj->sprite[0], 0);
        itfGridLookupValueOrDefault(obj->sprite[1], 0);
        break;
    }
}

/* Rebuild the first-node pointer by walking backward from the cursor. */
void mnuRebuildListFirstFromCursor(MenuList *list) {
    s32 node;
    s32 first;
    s32 previous;

    previous = (s32)list->cursor;
    first = (s32)list->cursor;
    while (node = previous, node != 0) {
        first = node;
        previous = (s32)((MenuListNode *)node)->prev;
    }
    list->first = (MenuListNode *)first;
}

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(MenuList *list) {
    s32 node;
    s32 last;
    s32 next;

    next = (s32)list->cursor;
    last = (s32)list->cursor;
    while (node = next, node != 0) {
        last = node;
        next = (s32)((MenuListNode *)node)->next;
    }
    list->last = (MenuListNode *)last;
}

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(s32 *menu, s32 restoreCursor) {
    s32 first;
    s32 oldCursor;
    menu[9] = 0;
    first = menu[4];
    oldCursor = menu[7];
    menu[6] = first;
    menu[7] = first;
    if (restoreCursor == 1) {
        s32 *node = (s32 *)first;
        if (node == NULL) {
            return;
        }
        do {
            if ((s32)node == oldCursor) {
                return;
            }
            mnuAdvanceListCursorDefault(menu);
            node = (s32 *)node[22];
        } while (node != NULL);
    }
}

/* Relink and reindex the pointer array. Native endpoint writes require at
 * least two entries; zero/one-entry calls are not guarded here. */
void mnuLinkItemList(MenuListNode **items, s32 count) {
    s32 itemIndex;

    items[0]->prev = NULL;
    items[0]->next = items[1];
    for (itemIndex = 1; itemIndex < count - 1; itemIndex++) {
        items[itemIndex]->prev = items[itemIndex - 1];
        items[itemIndex]->next = items[itemIndex + 1];
    }
    items[count - 1]->prev = items[count - 2];
    items[count - 1]->next = NULL;
    for (itemIndex = 0; itemIndex < count; itemIndex++) {
        items[itemIndex]->index = itemIndex;
    }
}

/* Three-way comparison of unsigned primary keys, descending without subtraction. */
s32 mnuComparePrimaryKeyDescending(s32 *left, s32 *right) {
    u32 leftKey = ((MenuListNode *)*left)->sortKeyPrimary;
    u32 rightKey = ((MenuListNode *)*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* Three-way comparison of unsigned primary keys, ascending without subtraction. */
s32 mnuComparePrimaryKeyAscending(s32 *left, s32 *right) {
    u32 leftKey = ((MenuListNode *)*left)->sortKeyPrimary;
    u32 rightKey = ((MenuListNode *)*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* Three-way comparison of unsigned secondary keys, descending without subtraction. */
s32 mnuCompareSecondaryKeyDescending(s32 *left, s32 *right) {
    u32 leftKey = ((MenuListNode *)*left)->sortKeySecondary;
    u32 rightKey = ((MenuListNode *)*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* Three-way comparison of unsigned secondary keys, ascending without subtraction. */
s32 mnuCompareSecondaryKeyAscending(s32 *left, s32 *right) {
    u32 leftKey = ((MenuListNode *)*left)->sortKeySecondary;
    u32 rightKey = ((MenuListNode *)*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* Three-way comparison of unsigned tertiary keys, descending without subtraction. */
s32 mnuCompareTertiaryKeyDescending(s32 *left, s32 *right) {
    u32 leftKey = ((MenuListNode *)*left)->sortKeyTertiary;
    u32 rightKey = ((MenuListNode *)*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* Three-way comparison of unsigned tertiary keys, ascending without subtraction. */
s32 mnuCompareTertiaryKeyAscending(s32 *left, s32 *right) {
    u32 leftKey = ((MenuListNode *)*left)->sortKeyTertiary;
    u32 rightKey = ((MenuListNode *)*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* Sort the walked node pointers and rebuild the list from the cursor.
 * Nonzero ascending selects the last three comparators, not descending order.
 * Allocation uses the stored count; key bounds and the relinker's minimum count remain unchecked. */
INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AF48);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AF60);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AF90);

void mnuSortItems(MenuList *menu, s32 keyIndex, s32 ascending) {
    s32 (*comparators[MNU_SORT_COMPARATOR_COUNT])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 nodeCount = 0;
    s32 allocationHandle = sdfAllocGeneralBlock(menu->count * MNU_LIST_POINTER_BYTES);
    MenuListNode **items = (MenuListNode **)sdfResourceRetainAddress(allocationHandle);
    MenuListNode **writeCursor = items;
    MenuListNode *node;

    for (node = menu->first; node != NULL; node = node->next) {
        *writeCursor++ = node;
        nodeCount++;
    }
    if (ascending != 0) {
        keyIndex += MNU_SORT_KEY_COUNT;
    }
    func_0035B7F8(items, nodeCount, MNU_LIST_POINTER_BYTES, comparators[keyIndex]);
    mnuLinkItemList(items, nodeCount);
    mnuRebuildListFirstFromCursor(menu);
    mnuRebuildListLastFromCursor(menu);
    mnuResetNodeLinks((s32 *)menu, 0);
    sdfReleaseResourceAllocation(allocationHandle);
}

/* Allocate four native fade records into pointer slots after the list header. */
void mnuAllocateListEntries(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        list[slotIndex + 1] = sdfAllocAndClearQuadwords(MNU_FADING_SLOT_BYTES);
    }
}

extern void sdfReleaseChipBlock();

/* Free the four record blocks, not their nested window pointers. */
void mnuFreeListEntries(s32 *list) {
    s32 *entries = list + 1;
    u32 slotIndex = 0;
    do {
        sdfReleaseChipBlock(*entries++);
        slotIndex++;
    } while (slotIndex < MNU_FADING_SLOT_COUNT);
}

/* Store the active word and window address in the header-selected slot.
 * Preserve the native below-five early return; it is not an upper capacity bound. */
void mnuAppendFadingWindowEntry(s32 activeValue, s32 windowAddress, s32 *list) {
    u32 slotIndex = *list;
    s32 *slot = list + slotIndex;
    s32 *entry;

    if (slotIndex < MNU_FADING_MUTATION_THRESHOLD) {
        return;
    }
    entry = (s32 *)slot[1];
    *list = slotIndex + 1;
    entry[0] = activeValue;
    entry[4] = windowAddress;
}

typedef struct MenuFadeEntry {
    u32 active;
    u32 pad4[3];
    void *window;
    u32 pad14;
} MenuFadeEntry;

/* Native removal only accepts indices at least five. Propagate the tail
 * record backward to the target, clearing each source's window pointer. */
void mnuRemoveFadingWindowEntry(s32 *list, u32 index) {
    s32 *entries;
    s32 *slot;
    u32 tailIndex;

    if (index >= MNU_FADING_MUTATION_THRESHOLD) {
        entries = list + 1;
        slot = entries + index;
        if (((MenuFadeEntry *)*slot)->active != 0) {
            mnuDestroyWindowContainer(((MenuFadeEntry *)*slot)->window);
        }
        tailIndex = list[0] - 1;
        ((MenuFadeEntry *)*slot)->window = 0;
        for (; index < tailIndex; tailIndex--) {
            *(MenuFadeEntry *)entries[tailIndex - 1] = *(MenuFadeEntry *)entries[tailIndex];
            ((MenuFadeEntry *)entries[tailIndex])->window = 0;
        }
        list[0]--;
    }
}

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < (u32)list[0]; slotIndex++) {
        s32 *entry = (s32 *)list[slotIndex + 1];
        mnuDrawWindowContainer(entry[2], entry[3], depth, entry[4], drawArg);
    }
}

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        s32 *entry = (s32 *)list[slotIndex + 1];
        if (entry[5] != 0) {
            entry[5] -= MNU_FADING_WORD_STEP;
        } else {
            mnuRemoveFadingWindowEntry(list, slotIndex);
        }
    }
}


/* Reset the original 0x98-byte prefix, then initialize the later fade fields. */
void mnuInitializeWindowFadeState(MenuFadeFields *menu) {
    memset(menu, 0, 0x98);
    menu->hasResourceCopy = 0;
    menu->previousProgress = 0x200;
    menu->currentWindow = NULL;
    menu->currentProgress = 0;
}

/* Snapshot the outgoing window and its resources before starting the next fade. */
void mnuBeginWindowFadeTransition(s32 windowAddress, s32 work) {
    MenuWindowContainer *window = (MenuWindowContainer *)windowAddress;
    MenuFadeFields *menu = (MenuFadeFields *)work;
    if (menu->currentWindow != NULL) {
        menu->previousWindow = *menu->currentWindow;
        menu->previousVisibleCount = menu->currentWindow->list->visibleCount;
        if (menu->currentWindow->resource != NULL) {
            menu->hasResourceCopy = 1;
            menu->savedResource = *menu->currentWindow->resource;
            menu->previousProgress = 0;
        } else {
            menu->hasResourceCopy = 0;
            menu->previousProgress = 0;
        }
    } else {
        menu->previousProgress = MNU_PANEL_FADE_LIMIT;
    }
    if (window != NULL) {
        window->panel.fade = MNU_PANEL_FADE_LIMIT;
        window->flags |= MNU_WINDOW_TRANSITION_FLAG;
    }
    menu->currentWindow = window;
    menu->currentProgress = 0;
}

void mnuResetWindowFadeParameters(MenuFadeFields *menu) {
    menu->currentWindow = NULL;
    menu->previousProgress = 0x200;
    menu->currentProgress = 0;
}

void mnuUpdateAndDrawWindowTransition(s32 x, s32 y, s32 depth, s32 work, s32 option) {
    MenuFadeFields *menu = (MenuFadeFields *)work;
    f32 t;

    if (menu->previousProgress < 0x200) {
        menu->previousWindow.originY = 160 * menu->previousProgress / 512;
        mnuSetWindowContainerState(&menu->previousWindow, 256 - menu->previousProgress / 2);
        if (menu->hasResourceCopy != 0) {
            menu->previousWindow.resource = &menu->savedResource;
        }
        MNU_ADVANCE_FADE(menu->previousProgress, 100, 512);
    }
    if (menu->currentWindow != NULL) {
        t = fsqrtf(40.0f) * (512 - menu->currentProgress) / 512.0f;
        menu->currentWindow->originY = -8 * (s32)(t * t);
        mnuSetWindowContainerState(menu->currentWindow, menu->currentProgress / 2);
        if (menu->currentWindow->panel.fade == 0) {
            menu->currentWindow->flags &= ~4;
        }
        mnuDrawWindowContainer(x, y, depth, menu->currentWindow, option);
        MNU_ADVANCE_FADE(menu->currentProgress, 80, 512);
    }
}

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

void mnuInitScrollHandles(MenuScrollPanel *menu) {
    ScrollHandle *handle;

    handle = effCreateStatusBatch(1);
    menu->handles[0] = handle;
    handle->inner->params->a = 10;
    handle->inner->params->b = 0;

    handle = effCreateStatusBatch(3);
    menu->handles[1] = handle;
    handle->inner->params->a = 8;
    handle->inner->params->b = 4;
    handle->inner->params->c = 8;

    handle = effCreateStatusBatch(1);
    menu->handles[2] = handle;
    handle->inner->params->a = 10;
    handle->inner->params->b = 0;
}

void mnuReleaseScrollPanelAnimations(menu)
    MenuScrollPanel *menu;
{
    ScrollHandle **handles = menu->handles;
    u32 i = 0;
    do {
        effDestroyPackedBatch(*handles++);
        i++;
    } while (i < 3);
}

MenuScrollPanel *mnuCreateScrollPanel(u32 owner) {
    MenuScrollPanel *menu = (MenuScrollPanel *)sdfAllocSizeClassBlock(0x48);
    memset(menu, 0, 0x48);
    menu->firstSprite = 0;
    menu->secondSprite = 0;
    itfGridStorePosition(&menu->positions[0], owner, 0x40);
    itfGridStorePosition(&menu->positions[1], owner, 0x41);
    itfGridStorePosition(&menu->positions[2], owner, 0x44);
    itfGridStorePosition(&menu->active, 0, 0);
    itfGridStorePosition(&menu->pending, 0, 0);
    mnuInitScrollHandles(menu);
    return menu;
}

void mnuDestroyScrollPanel(MenuScrollPanel *menu) {
    mnuReleaseScrollPanelAnimations();
    sdfReleaseChipBlock(menu);
}


void mnuActivatePendingPanelResource(MenuScrollPanel *context) {
    s32 pendingHandle;

    pendingHandle = context->pending.sprite;
    context->active.sprite = pendingHandle;
    context->active.effect = context->pending.effect;
    context->pending.sprite = 0;
    if (pendingHandle != 0) {
        effConfigureWithDefaultSetting(pendingHandle, context->pending.effect,
                                       context->handles[2], 0, 10, 2);
        return;
    }
}

void mnuConfigurePanelResource(MenuScrollPanel *menu, u32 model, u32 value, u32 color) {
    mnuActivatePendingPanelResource(menu);
    menu->color = color;
    menu->pending.sprite = model;
    menu->pending.effect = value;
    effConfigureIndexedSlotResource(model, value, menu->handles[0], 0, 3);
}

u8 mnuHasActivePanelResource(MenuScrollPanel *resources) {
    return resources->active.sprite != 0;
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002BB510);

/* Three resource-slot handles at +0xE4/+0xE8/+0xEC. */
typedef struct MenuSlotEffectHandles {
    u8 pad00[0xE4];
    u32 handles[3];
} MenuSlotEffectHandles;

void mnuLoadPanelSectionResources(MenuSlotEffectHandles *slot, u32 model, u32 firstValue, u32 secondValue, s32 thirdValue
                                    ) {
    u32 handle;

    handle = (u32)effCreateResourceSlotSet((u32 *)model, firstValue, 1);
    slot->handles[0] = handle;
    handle = (u32)effCreateResourceSlotSet((u32 *)model, secondValue, 1);
    slot->handles[1] = handle;
    if (-1 < thirdValue) {
        handle = (u32)effCreateResourceSlotSet((u32 *)model, thirdValue, 1);
        slot->handles[2] = handle;
    }
}

void mnuReleasePartyPanelTextures(s32 menu) {
    u32 flags;
    u32 *slot;
    u32 *secondHandle;
    u32 *firstHandle;
    u32 index;

    slot = (u32 *)(menu + 0x7c);
    secondHandle = (u32 *)(menu + 0x164);
    firstHandle = (u32 *)(menu + 0x160);
    index = 0;
    do {
        if (slot[0x38] != 0) {
            effDestroyResourceSlotSet(slot[0x38]);
        }
        if (slot[0x39] != 0) {
            effDestroyResourceSlotSet(slot[0x39]);
        }
        if (slot[0x3a] != 0) {
            effDestroyResourceSlotSet(slot[0x3a]);
        }
        flags = *slot;
        index = index + 1;
        slot[0x38] = 0;
        *firstHandle = 0;
        *slot = flags & 0xffffffbf;
        slot = slot + 0x84e;
        *secondHandle = 0;
        secondHandle = secondHandle + 0x84e;
        firstHandle = firstHandle + 0x84e;
    } while (index < 5);
}


void mnuResetPartyPanelFade(u8 *menu, s32 index, u32 unused, u32 preserve) {
    MenuPageSlot *entry = &((MenuPageWindow *)menu)->slots[index];
    entry->contents[0].hp.fadeOut = 0;
    entry->contents[0].mp.fadeOut = 0;
    if (preserve == 0) {
        entry->contents[0].hp.fade = 0x100;
        entry->contents[0].mp.fade = 0x100;
    }
}

void func_002BB9C8(MenuSprites *page, u32 flags) {
    page->flags = flags;
}

void mnuSetPageParams(MenuSprites *page, s32 mode) {
    switch (mode) {
    case 0:
        page->fadeOut = 0;
        page->drawAlpha = 0;
        page->slideOffset = 0x40;
        page->slideSpeed = 0x100;
        break;
    case 1:
        page->fadeOut = 1;
        page->drawAlpha = 0x100;
        page->slideOffset = 0;
        page->slideSpeed = 0x1000;
        break;
    default:
        page->fadeOut = 0;
        page->drawAlpha = 0x100;
        page->slideOffset = 0;
        page->slideSpeed = 0;
        break;
    }
}

typedef struct MenuSpritePlacement {
    u32 *source;
    u32 index;
    s32 x;
    s32 y;
} MenuSpritePlacement;

typedef struct MenuGatedSpritePlacement {
    u32 *source;
    u32 index;
    s32 x;
    s32 y;
    s8 requiresFlag;
} MenuGatedSpritePlacement;

void *func_002BBA38(s32 kind, s32 mainResource, s32 itemResource,
                   s32 iconResource, s32 cursorResource, s32 alternateResource) {
    u32 i;
    s32 enabled = mdlFlagTest(0x901);
    MenuSpritePlacement icons[5] = {
        {(u32 *)iconResource, 7, 0x170, 0xA0},
        {(u32 *)mainResource, 0x14, 0x8A0, 0xE8},
        {(u32 *)iconResource, 3, 0x1E0, 0x1E0},
        {(u32 *)iconResource, 4, 0x1020, 0x1E0},
        {(u32 *)iconResource, 5, 0x310, 0xE0}
    };
    MenuSpritePlacement cursors[4] = {
        {(u32 *)itemResource, 0x12, 0x720, 0x358},
        {(u32 *)itemResource, 0x12, 0x720, 0x430},
        {(u32 *)cursorResource, 0x2C, 0, 0},
        {(u32 *)alternateResource, 0x18, 0, 0}
    };
    MenuGatedSpritePlacement items[11] = {
        {(u32 *)itemResource, 0xE, 0x1B0, 0x348, 0},
        {(u32 *)itemResource, 7, 0x410, 0x358, 0},
        {(u32 *)itemResource, 0x10, 0xD0, 0x428, 1},
        {(u32 *)itemResource, 7, 0x410, 0x430, 1},
        {(u32 *)itemResource, 8, 0x570, 0x380, 0},
        {(u32 *)itemResource, 0xA, 0xCB0, 0x380, 0},
        {(u32 *)itemResource, 0xC, 0xDF0, 0x380, 0},
        {(u32 *)itemResource, 0xD, 0x1170, 0x380, 0},
        {(u32 *)itemResource, 9, 0x570, 0x458, 0},
        {(u32 *)itemResource, 0xB, 0xE20, 0x458, 0},
        {(u32 *)itemResource, 0xF, 0xEB0, 0x330, 0}
    };
    MenuSprites *page = sdfAllocSizeClassBlock(sizeof(MenuSprites));
    memset(page, 0, sizeof(MenuSprites));
    page->unkC = kind;
    for (i = 0; i < 5; i++) {
        page->icon[i] = effCreateResourceSlotSet(icons[i].source, icons[i].index, 1);
        itfSetGridEntryQuantizedAndRefresh(page->icon[i], 0, icons[i].x, icons[i].y, 0, 0);
    }
    for (i = 0; i < 11; i++) {
        if (!items[i].requiresFlag || enabled) {
            page->item[i] = effCreateResourceSlotSet(items[i].source, items[i].index, 1);
            itfSetGridEntryQuantizedAndRefresh(page->item[i], 0, items[i].x, items[i].y, 0, 0);
        } else {
            page->item[i] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        if (cursors[i].source != NULL) {
            page->cursor[i] = effCreateResourceSlotSet(cursors[i].source, cursors[i].index, 1);
            itfSetGridEntryQuantizedAndRefresh(page->cursor[i], 0, cursors[i].x, cursors[i].y, 0, 0);
        } else {
            page->cursor[i] = NULL;
        }
    }
    mnuSetPageParams(page, 0);
    page->unk74 = 0;
    page->unk75 = 0;
    return page;
}


extern s32 effDestroyResourceSlotSet();

extern void sdfReleaseChipBlock();


void mnuFreeIconSprites(MenuSprites *menu) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effDestroyResourceSlotSet(menu->icon[i]);
    }
    for (i = 0; i < 11; i++) {
        if (menu->item[i] != NULL) {
            effDestroyResourceSlotSet(menu->item[i]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (menu->cursor[i] != NULL) {
            effDestroyResourceSlotSet(menu->cursor[i]);
        }
    }
    sdfReleaseChipBlock(menu);
}

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);


void mnuDrawIconRow(s32 unusedA, s32 unusedB, s32 depth, s32 skip, MenuSprites *set, s32 drawArg) {
    u32 i;
    if (skip == 0) {
        for (i = 0; i < 5; i++) {
            func_00306CD0(0xBC0, 0x3C8, depth, set->fade, 0, (s32)set->icon[i], 0, drawArg);
        }
    }
}

extern void *func_002BBA38();


void mnuSetWindowResource(s32 index, u32 *menu, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6) {
    mnuSelectPage((MenuPageWindow *)menu, index);
    ((MenuPageWindow *)menu)->slots[index].contents[0].windowSprites = func_002BBA38(0, a2, a3, a4, a5, a6);
    ((MenuPageWindow *)menu)->flags |= 0x80;
}

void mnuSetIndexedWindowPageSpriteFlags(s32 index, u8 *menu, u32 first, u32 second) {
    MenuSprites **slot = &((MenuPageWindow *)menu)->slots[index].contents[0].windowSprites;
    if (*slot != NULL) {
        (*slot)->unk74 = first;
        (*slot)->unk75 = second;
    }
}

void mnuClearEntries(u8 *menu) {
    MenuSprites **entry = &((MenuPageWindow *)menu)->slots[0].contents[0].windowSprites;
    u32 i = 0;
    mnuClearPageSelectionHandles(menu);
    do {
        if (*entry != NULL) {
            mnuFreeIconSprites(*entry);
            *entry = NULL;
        }
        i++;
        entry += sizeof(MenuPageSlot) / sizeof(*entry);
    } while (i < 5);
    ((MenuPageWindow *)menu)->flags &= ~0x80;
}



typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

extern MenuIconLayout D_0042AFD8;

u32 mnuCreateIconBundle(u32 resource) {
    MenuIconLayout layout = D_0042AFD8;
    MenuIconBundle *set = (MenuIconBundle *)sdfAllocSizeClassBlock(0x20);
    u32 i;
    memset(set, 0, 0x20);
    for (i = 0; i < 3; i++) {
        void *sprite = effCreateResourceSlotSet((u32 *)resource, layout.entry[i].id, 1);
        set->sprite[i] = sprite;
        itfSetGridEntryQuantizedAndRefresh(sprite, 0, layout.entry[i].x - 0xc80, layout.entry[i].y - 0x20, 0, 0);
    }
    return (u32)set;
}

void mnuReleaseIconBundleAndSprites(MenuIconBundle *menu) {
    u32 i = 0;
    do {
        effDestroyResourceSlotSet((u32)menu->sprite[i]);
        i++;
    } while (i < 3);
    sdfReleaseChipBlock(menu);
}


void mnuDrawFadeIcons(s32 x, s32 y, s32 depth, s32 unused, MenuIconBundle *obj, s32 drawArg) {
    s32 fade = obj->fade;
    s32 next;
    func_00306CD0(x, y, depth, fade, 0, (s32)obj->sprite[0], 0, drawArg);
    func_00306CD0(x, y, depth, fade, 0, (s32)obj->sprite[1], 0, drawArg);
    func_00306CD0(x, y, depth, fade, 0, (s32)obj->sprite[2], 0, drawArg);
    if (obj->fadeOut == 0) {
        MNU_ADVANCE_FADE(obj->fade, 0x10, 0x100);
    } else {
        next = obj->fade;
        if (next > 0) {
            obj->fade = next - 0x10;
            next = obj->fade;
        }
        if (next < 0) {
            obj->fade = 0;
        }
    }
}


void mnuAttachPartyIconBundle(s32 index, s32 menu, u32 resource) {
    u32 bundle;

    bundle = mnuCreateIconBundle(resource);
    ((MenuPageWindow *)menu)->slots[index].contents[0].iconBundle = bundle;
}

void mnuReleasePartyIconBundles(u8 *menu) {
    u32 *bundle = &((MenuPageWindow *)menu)->slots[0].contents[0].iconBundle;
    u32 i = 0;
    do {
        u32 resource = *bundle;
        i++;
        if (resource != 0) {
            mnuReleaseIconBundleAndSprites((MenuIconBundle *)resource);
            *bundle = 0;
        }
        bundle += sizeof(MenuPageSlot) / sizeof(*bundle);
    } while (i < 5);
}

s32 mnuPercentOrHundred(s32 value, s32 total) {
    if (total > 0) {
        return value * 100 / total;
    }
    return 100;
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002BC498);

void mnuReleasePartyPanelSpriteTextures(u8 *menu) {
    u32 i = 0;
    do {
        mnuReleaseSpriteTextures((s32)(menu + 0x94));
        mnuReleaseSpriteTextures((s32)(menu + 0xe4));
        menu += 0x2138;
        i++;
    } while (i < 5);
}



/* Copy eight resource handles into the window's primary handle bank. */
void mnuCopyPrimaryWindowHandles(MenuPageWindow *menu, u32 *source) {
    u32 value;
    s32 *destination;
    u32 index;

    destination = menu->handlesA;
    index = 0;
    do {
        value = *source;
        source = source + 1;
        index = index + 1;
        *destination = value;
        destination = destination + 1;
    } while (index < 8);
}

/* Copy eight resource handles into the window's secondary handle bank. */
void mnuCopySecondaryWindowHandles(MenuPageWindow *menu, u32 *source) {
    u32 value;
    s32 *destination;
    u32 index;

    destination = menu->handlesB;
    index = 0;
    do {
        value = *source;
        source = source + 1;
        index = index + 1;
        *destination = value;
        destination = destination + 1;
    } while (index < 8);
}

void mnuRegisterResourceHandles(MenuPageWindow *destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(source[i]);
        destination->handlesC[i] = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002BC690);

void mnuRefreshWindowSlots(MenuPageWindow *menu, s32 flag) {
    u32 i;
    s32 *res;
    if (flag == 0) {
        for (i = 0, res = menu->handlesA; i < 8; i++, res++) {
            if (effHasFirstTextureHandle(*res) != 0) {
                effReleaseTextureHandlesAndResetSlots(*res);
                effReleaseTextureHandlesAndResetSlots(res[8]);
            }
        }
    }
    for (i = 0; i < 5; i++) {
        PartyPanel *entries = menu->records;
        if (entries->slots[i].unk8 >= 0) {
            if ((s32)i < entries->unk0) {
                func_002BC690(menu, i, 1);
            } else {
                func_002BC690(menu, i, 2);
            }
        } else {
            func_002BC690(menu, i, 0);
        }
    }
}

void func_002BCA98(MenuPageWindow *menu) {
    mnuRefreshWindowSlots(menu, 0);
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002BCAB0);

extern char D_00437C30[];

/* Create both party-panel lists and append their shared row labels. */
void mnuInitScrollLists(MenuPageWindow *menu, PartyPanel *records) {
    s32 i = 0;
    menu->lists[0] = mnuCreateListState(0, 1, 1);
    menu->lists[1] = mnuCreateListState(0, 1, 1);
    if (records->unk0 + records->unk4 > 0) {
        do {
            mnuListAppendNode(menu->lists[0], D_00437C30);
            i++;
            mnuListAppendNode(menu->lists[1], D_00437C30);
        } while (i < records->unk0 + records->unk4);
    }
}

void mnuDestroyWindowOwnedLists(context)
    MenuPageWindow *context;
{
    mnuDestroyListState(context->lists[0]);
    mnuDestroyListState(context->lists[1]);
}

void mnuRebuildScrollLists(MenuPageWindow *context, PartyPanel *records) {
    mnuDestroyWindowOwnedLists(context);
    mnuInitScrollLists(context, records);
}


void mnuClearPageSelection(MenuPageWindow *menu) {
    if (menu->selected >= 0) {
        menu->slots[menu->selected].contents[0].hp.fade = 0x100;
        menu->slots[menu->selected].contents[0].mp.fade = 0x100;
        menu->selected = -1;
    }
    menu->flags &= ~0x200;
}

INCLUDE_ASM(const s32, "game/code_002B8FC8", func_002BCD90);


void mnuFreeWindowSprites(MenuPageSlot *win) {
    u32 i;
    for (i = 0; i < 3; i++) {
        if (win->contents[0].icon[i] != 0) {
            effDestroyResourceSlotSet(win->contents[0].icon[i]);
        }
    }
    if (win->contents[0].frame[0] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[0]);
    }
    if (win->contents[0].frame[1] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[1]);
    }
    if (win->contents[0].frame[2] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[2]);
    }
    if (win->contents[0].frame[3] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[3]);
    }
    if (win->contents[0].frame[4] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[4]);
    }
    if (win->contents[0].frame[5] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[5]);
    }
    if (win->contents[0].frame[6] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[6]);
    }
    if (win->contents[0].frame[7] != 0) {
        effDestroyResourceSlotSet(win->contents[0].frame[7]);
    }
}

void mnuShutdownContext(u8 *ctx) {
    MenuPageSlot *slot = ((MenuPageWindow *)ctx)->slots;
    u32 i;
    for (i = 0; i < 5; i++, slot++) {
        mnuFreeWindowSprites(slot);
    }
    mnuReleasePartyPanelSpriteTextures(ctx);
    mnuDestroyWindowOwnedLists(ctx);
}


void mnuResolveUnselectedPageHandles(MenuPageWindow *window) {
    s32 selected = window->selected;
    u32 i;

    for (i = 0; i < 5; i++) {
        if (i != selected) {
            s32 id = window->records->slots[i].unk8;

            if (id >= 0) {
                if (effHasFirstTextureHandle(window->handlesA[id]) == 0) {
                    effResolveAndReleaseResource(window->handlesA[id]);
                    effResolveAndReleaseResource(window->handlesB[id]);
                }
            }
        }
    }
}

void mnuRefreshPageHandles(MenuPageWindow *window) {
    s32 selected = window->selected;
    u32 i;
    s32 id;
    PartyPanelEntry *record;

    for (i = 0; i < 5; i++) {
        record = &window->records->slots[i];
        id = record->unk8;
        if (id >= 0) {
            if (effHasFirstTextureHandle(window->handlesA[id]) != 0) {
                effReleaseTextureHandlesAndResetSlots(window->handlesA[id]);
                effReleaseTextureHandlesAndResetSlots(window->handlesB[id]);
            }
        }
    }
    record = &window->records->slots[selected];
    id = record->unk8;
    if (id >= 0) {
        if (effHasFirstTextureHandle(window->handlesA[id]) == 0) {
            effResolveAndReleaseResource(window->handlesA[id]);
            effResolveAndReleaseResource(window->handlesB[id]);
        }
    }
}


INCLUDE_ASM(const s32, "game/code_002B8FC8", mnuSelectPage);

void mnuClearPageSelectionHandles(MenuPageWindow *window) {
    s32 *resource = window->handlesC;
    u32 i;

    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(*resource++);
    }
    if (window->selected >= 0) {
        mnuResolveUnselectedPageHandles(window);
    }
    mnuClearPageSelection(window);
}

void mnuFlagActiveWindows(u8 *menu) {
    u8 *kind = menu + 8;
    u8 *flags = menu + 0xc;
    u32 i;
    for (i = 0; i < 5; i++) {
        s32 offset = 0x70 + i * 0x2138;
        if (*(u32 *)(kind + offset) == 2) {
            *(u32 *)(flags + offset) |= 1;
        }
    }
}

void mnuClearPartyPanelActiveFlags(s32 menu) {
    u32 *flags;
    u32 index;

    flags = (u32 *)(menu + 0x7c);
    index = 0;
    do {
        index = index + 1;
        *flags = *flags & 0xfffffffe;
        flags = flags + 0x84e;
    } while (index < 5);
}

void mnuClearActionFlags(s32 kind, u8 *ctx) {
    mnuSeekListNode(0, (MenuList *)*(s32 *)(ctx + kind * 4 + 0xa690));
    if (kind == 0) {
        *(u32 *)ctx &= ~2;
        *(u32 *)ctx &= ~4;
        *(u32 *)ctx &= ~8;
        *(u32 *)ctx &= ~0x10;
        *(u32 *)ctx &= ~0x20;
    } else {
        *(u32 *)ctx &= ~2;
        *(u32 *)ctx &= ~8;
        *(u32 *)ctx &= ~0x10;
        *(u32 *)ctx &= ~0x20;
    }
}

void mnuStepPartyPanelListFromInput(s32 mode, MenuPageWindow *window) {
    u32 input = mnuMapPadMaskToFlags(0x30);
    u32 flags = window->flags;
    u32 state;
    s32 which = mode == 8;
    MenuList *list = window->lists[which];

    if (flags & 2) {
        window->fade = 0x100;
    }
    state = flags | 2;
    state |= mode;
    window->flags = state;
    if (!(state & 0x30)) {
        if (!(input & 0x300000)) {
            mnuClearListFlagsOneAndTwo((u32 *)list);
        }
        if (input & 0x10) {
            if (mnuTestListFlagTwo((u32 *)list) == 0) {
                window->fade = 0x100;
            }
            mnuRetreatListCursorDefault((u32)list);
        }
        if (input & 0x20) {
            if (mnuTestListFlagTwo((u32 *)list) == 0) {
                window->fade = 0x100;
            }
            mnuAdvanceListCursorDefault((u32)list);
        }
        mnuPlayInputSound(0, input, &list->stateFlags);
    }
    if (window->fade > 0) {
        s32 fade = window->fade - 0x10;

        window->fade = fade < 0 ? 0 : fade;
    }
    mnuClearPageSelection(window);
}

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AFB8);

INCLUDE_RODATA(const s32, "game/code_002B8FC8", D_0042AFD8);

