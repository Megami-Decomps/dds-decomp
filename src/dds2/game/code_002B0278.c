#include "mnu.h"
#include "fpu.h"

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

typedef struct MenuWindowContainer MenuWindowContainer;
typedef struct MenuList MenuList;
typedef struct MenuIconSprites MenuIconSprites;
typedef struct MenuIconState MenuIconState;
typedef struct MenuFadeFields MenuFadeFields;


extern s32 dspStartEntry(s32 entry);
extern s32 D_00435E5C;

extern void itfDrawGridWithResolvedSlot();

extern void mnuClearListFlagsOneAndTwo();

extern s32 func_002C6CE8(void);

extern s32 func_002C6480();

extern char D_003E75C4[];

extern void func_002B9CF8(s32, s32, s32, s32, s32);

extern s32 func_002B86E8(u32);

extern void func_002AAE80();

extern void mnuReleasePartyIconBundles();

extern void mnuClearEntries();

extern s32 itfDrawUnderscoreTextSegment();

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

extern void func_002BD480();
extern char D_003E7758[];

extern s32 ptySkillMenuApplyFieldUseAndCost();

extern u32 mnuMapPadMaskToFlags();

extern s32 mnuGetAbilityByteCategory();

extern void mnuSetPopupEntry();

extern void mnuPlayInputSound();

extern u32 D_003E7828[];

extern u32 D_003E7858[];

extern char *D_003E7818[];

extern char *D_003E7820[];

extern u32 effLoadIndexedResource(char *, char *, s32);

extern u32 effLoadMappedResource(char *, char *);

extern void effRequestResourceByMode(char *, char *, s32, u32 *);

extern void effRequestMappedResource(char *, char *, u32 *);

extern void mnuFreeWindowSprites();

extern void mnuHideIconGroup();

extern s32 evtGetCapturedWindowPanelValue();

extern void mnuSetPopupEntryFlagged();

extern void func_002BAF50();

extern void mnuInitPartyPanelSlots();

extern s32 func_002B06A8();

extern void mnuPrepareStaffValueChangeDialog();

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

extern void mnuUpdateAndDrawWindowTransition(s32, s32, s32, s32, s32);

extern void mnuIdleVoiceTimer();

extern void func_002B2408();

extern u32 mnuCreateIconBundle(u32);

extern u32 func_002B9FF8();

extern s32 datGameState;

extern void mnuDrawIconPanel(s32, s32, s32, s32, MenuIconState *, s32, s32);

extern void mnuHideWindowHandlesKindFourFive(MenuIconState *);

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);
extern char D_003E75E0[];
extern char D_003E75A8[];

extern void sndSetSequenceVolumePan();

typedef struct MenuSlot {
    s32 resources[3];
    u16 unused;
    u16 flags;
} MenuSlot;

typedef struct MenuSprites {
    u8 unk0[0x10];
    void *icon[5];
    void *item[11];
    void *cursor[4];
    s32 fade;
    u8 pad64[0x10];
    u8 unk74;
    u8 unk75;
} MenuSprites;
typedef struct MenuPageGauge {
    s32 resourceIndex; /* Negative values have no entry in the window's resource banks. */
    u8 pad4[4];
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    u8 pad18[0x18];
} MenuPageGauge;

typedef struct MenuPageEntry {
    s32 partyIndex;
    MenuPageGauge gauge;
} MenuPageEntry; /* 0x34-byte party row */

/* Counts belong to the table header, not to every party row. */
typedef struct MenuPageRecord {
    s32 visibleCount;
    s32 additionalCount;
    u32 unk8;
    MenuPageEntry entries[5];
} MenuPageRecord;

/* HP/MP display record; the trailing reset values have no named read here. */
typedef struct MenuPageBar {
    u8 pad00[0x10];
    s32 percentage;
    u8 pad14[0x30];
    u32 unk44;
    u32 unk48;
    u8 pad4C[4];
} MenuPageBar;

/* A DDS2 party slot contains both stat displays and its owned sprite sets. */
typedef struct MenuPageSlot {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    u32 icon[3];
    MenuPageBar hp;
    MenuPageBar mp;
    u32 frame[8];
    MenuSprites *windowSprites;
    u32 iconBundle;
    u8 padE4[0x2138 - 0xE4];
} MenuPageSlot;

typedef struct MenuPageWindow {
    u32 flags;
    u8 pad4[4];
    MenuPageRecord *records;
    u8 padC[0x18];
    s32 handlesA[8];
    s32 handlesB[8];
    s32 handlesC[5];
    MenuPageSlot slots[5];
    MenuList *lists[2];
    s32 selected;
    u32 unkA69C;
    s32 fade;
} MenuPageWindow;
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

extern MenuSlot *datAffinityRecords;

extern void mnuReleaseSpriteTextures(s32);

extern s32 kwlnTaskGetUserValue();

extern void mnuDrawWindowContainer(s32, s32, s32, MenuWindowContainer *, s32);

extern void effResolveAndReleaseResource(s32);

extern s32 func_002C4038(s32, s32 *, u64, u64);

/* Menu state handler installer: the call is inlined at each use, so callers
 * return its result through a real call rather than a sibcall. */

typedef struct MenuListNode MenuListNode;

struct MenuListNode {
    s32 index;
    s32 value;
    u8 pad8[0x40];
    u32 flags48;         /* 0x48 */
    u8 pad4C[4];
    s32 fadeCounter;     /* 0x50 */
    u8 selectionByte54; /* 0x54: cleared on moving the list selection */
    u8 pad55[3];
    struct MenuListNode *next;
    struct MenuListNode *prev;
    u32 sortKeyPrimary;   /* 0x60 */
    u32 sortKeySecondary; /* 0x64 */
    u32 sortKeyTertiary;  /* 0x68 */
    u8 pad6C[8];
};

struct MenuList {
    u32 stateFlags;     /* 0x00: cursor and selection-control bits */
    u32 flags;
    u32 id; /* 0x08: owner/list identifier */
    s32 visibleCount;
    MenuListNode *first;
    MenuListNode *last;
    MenuListNode *head;
    MenuListNode *cursor;
    s32 count;
    s32 windowOffset;
    s32 rowHeight;
    u8 pad2C[0x10];
    s32 scale; /* 0x3C: 8.8 fixed-point list scale */
};

typedef struct MenuSpriteInner {
    u8 unk0[0xC];
    s32 shade;
    u8 pad10[0x18];
    u32 flags; /* 0x28: low flag cleared when the selection moves */
    u8 pad2C[0x50];
    s32 shadeSource;
} MenuSpriteInner;

typedef struct MenuSprite {
    u8 unk0[0x18];
    MenuSpriteInner *inner;
} MenuSprite;

struct MenuIconState {
    u32 kind;
    u32 unk4;
    s32 count;
    MenuSprite *sprite[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 fade;
};

struct MenuWindowContainer {
    s32 id;                /* 0x00 */
    u32 flags;             /* 0x04 */
    s32 originX;           /* 0x08 */
    s32 originY;           /* 0x0C */
    s32 width;             /* 0x10 */
    s32 height;            /* 0x14 */
    MenuList *list;        /* 0x18 */
    s32 field1C;           /* 0x1C */
    s32 sprite20;          /* 0x20 */
    s32 param24;           /* 0x24 */
    s32 param28;           /* 0x28 */
    u32 layout2C;         /* 0x2C */
    u32 layout30;
    u32 layout34;
    u32 layout38;
    u32 layout3C;
    u32 layout40;
    u32 layout44;
    u32 layout48;
    u32 layout4C;
    s32 scale50;           /* 0x50 */
    s32 scale54;           /* 0x54 */
    MenuIconState panel; /* 0x58: embedded drawable panel layout */
    MenuIconSprites *resource; /* 0x90: owned sprite-resource bundle */
    u32 state;             /* 0x94 */
};

typedef struct PartyEntryCopy {
    u16 flags;
    u16 pad02;
    u16 displayId; /* 0x04: used to select a party display asset */
    u16 pad06;
    u32 word[0x6F];
} PartyEntryCopy; /* 0x1C4 bytes, versus 0x1A4 in DDS1 */

/* One allocated party-selection work area: original/current/backup entries,
 * saved panel payloads, and fade state all belong to this same allocation. */
typedef struct PartyMenuData {
    s32 allocation;
    u8 pad04[4];
    MenuWindowContainer *primaryWindow; /* 0x08 */
    PartyEntryCopy original[5];         /* 0x0C */
    PartyEntryCopy current[5];          /* 0x8E0 */
    s32 activeCount;                    /* 0x11B4 */
    PartyEntryCopy backup[5];           /* 0x11B8 */
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

extern MenuListNode *sdfAllocAndClearQuadwords(s32);

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


extern void func_002B0278(s32);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0278);

s32 mnuAdvanceStaffValuePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s32 mnuFinishStaffValuePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
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
    s32 slot = datGameState + ((MenuContext *)context)->partyWindow.lists[0]->cursor->index * 0x1c4 + 0xa60;
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    s32 state = func_002C4038(context + 8, popup, 0, callback);
    s32 label;
    if (state != 0) {
        return state;
    }
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    if (evtGetCapturedWindowPanelValue() == 0) {
        label = ((MenuWindowContainer *)*(s32 *)(menu + 0x18))->list->cursor->sortKeySecondary;
        mnuPrepareStaffValueChangeDialog(context, slot, label, func_002B06A8(menu, slot, label));
        mnuInitPartyPanelSlots(context + 0xA928);
        func_002BCAB0(context + 0x284);
        *(s32 *)(menu + 0x40) = label;
    }
    mnuSetPopupEntryFlagged(popup, D_003E7530);
    return 0;
}

s32 mnuStartPanelDispatch(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s32 mnuStartPanelExit(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
}

/* Clear a selected party record's five-byte stat group and companion flag,
 * then recalculate the party's maximum HP and MP. */
void mnuClearPartySelectionValues(u32 context, s32 selection) {
    s32 recordOffset;
    s32 bytesRemaining;
    if (selection == 0) {
        return;
    }
    selection -= 0xc0;
    recordOffset = 0x1e670 + selection * 5;
    bytesRemaining = 4;
    do {
        ((u8 *)datGameState)[recordOffset++] = 0;
    } while (--bytesRemaining >= 0);
    ((u8 *)(selection + datGameState))[0x1e7b0] = 0;
    ptyRecomputeMaxHpMp(context);
}

u32 mnuEnterSlotLabel(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 slot = datGameState + ((MenuContext *)context)->partyWindow.lists[0]->cursor->index * 0x1c4 + 0xa60;
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
    s32 slot = datGameState + ((MenuContext *)context)->partyWindow.lists[0]->cursor->index * 0x1c4 + 0xa60;
    s32 state = func_002C4038(context + 8, popup, 0, callback);
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
        mnuInitPartyPanelSlots(context + 0xA928);
        func_002BCAB0(context + 0x284);
    }
    mnuSetPopupEntryFlagged(popup, D_003E7530);
    return 0;
}

s32 mnuAdvancePartyClearPopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s32 mnuFinishPartyClearPopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0D90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0FA0);

void mnuDestroyPartySelectionWindow(s32 context) {
    mnuDestroyWindowContainer((u32)((PartyMenuData *)((MenuContext *)context)->party)->primaryWindow);
}


/* Snapshot the five party entries and cap the menu's displayed slot count. */
void mnuCopyPartyEntries(context)
s32 context;
{
    PartyMenuData *menu = (PartyMenuData *)((MenuContext *)context)->party;
    PartyEntryCopy *destination = menu->current;
    s32 i;
    s32 flagOffset = 0xA60;
    s32 copyOffset = 0;

    menu->activeCount = 0;
    for (i = 0; i < 5; i++) {
        *destination = *(PartyEntryCopy *)(copyOffset + datGameState + 0xA60);
        if (((PartyEntryCopy *)(datGameState + flagOffset))->flags & 1) {
            menu->activeCount = menu->activeCount + 1;
        }
        flagOffset += 0x1C4;
        destination++;
        copyOffset += 0x1C4;
    }
    if (menu->activeCount >= 4) {
        menu->activeCount = 3;
    }
    menu->selection = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B12B0);

extern void func_002B12B0();
extern void func_002BCAB0();

void mnuRestorePartyEntriesAndRefresh(context)
s32 context;
{
    PartyMenuData *menu = (PartyMenuData *)((MenuContext *)context)->party;
    PartyEntryCopy *entry = menu->current;
    s32 i;
    s32 backupOffset;
    s32 panel;

    for (i = 0; i < 5; i++) {
        if (entry->flags & 1) {
            func_002B12B0(i, -3, 1, context);
        }
        entry++;
    }
    backupOffset = 0;
    for (i = 4; i >= 0; i--) {
        *(PartyEntryCopy *)(backupOffset + datGameState + 0xA60) = *(PartyEntryCopy *)(backupOffset + (s32)menu + PARTY_BACKUP_OFFSET);
        backupOffset += 0x1C4;
    }
    panel = context + 0x284;
    mnuReleasePartyPanelTextures(panel);
    mnuInitPartyPanelSlots(context + 0xA928);
    func_002BCA98(panel);
    func_002BCAB0(panel);
}

s32 mnuCountActiveSlots(void) {
    s32 count = 0;
    s32 i;
    u16 *slotFlags = (u16 *)(datGameState + 0xa60);
    for (i = 4; i >= 0; i--) {
        count += *slotFlags & 1;
        slotFlags += 0xe2;
    }
    if (count > 3) {
        count = 3;
    }
    return count;
}

void mnuClearPartySelectionAndActivateSlots(s32 context) {
    PartyMenuData *menu = (PartyMenuData *)((MenuContext *)context)->party;
    s32 i;
    s32 node;

    mnuCopyPartyEntries();
    menu->selection = 0;
    memset(menu->backup, 0, 0x8D4);
    ((MenuContext *)context)->partyPanelActive = 1;
    ((MenuContext *)context)->partyPanelLast = mnuCountActiveSlots() - 1;
    func_002BCA98(context + 0x284);
    for (i = 0; i < 5; i++) {
        *(u32 *)(context + 0x300 + i * 0x2138) |= 0x40;
    }
    for (node = (s32)menu->primaryWindow->list->first;
         node != 0; node = (s32)((MenuListNode *)node)->next) {
        ((MenuListNode *)node)->flags48 &= ~1;
    }
}

void mnuRefreshPartyPanelSlots(s32 context) {
    mnuReleasePartyPanelTextures(context + 0x284);
    mnuInitPartyPanelSlots(context + 0xA928);
    func_002BCA98(context + 0x284);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B18E8);

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
    mnuSetPopupEntryFlagged(menu + 0x54, D_003E7588);
    mnuConfigurePanelResource(((MenuContext *)menu)->panelHandle, ((MenuContext *)menu)->displayHandle, 0, 1);
    func_002BAF50(((MenuContext *)menu)->imageHandle, menu + 0xB10C);
    party->freezePanel = 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1C68);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1EA8);

/* Two-stage fade: B rises first when opening, A falls first when closing. */
void mnuUpdateStaffFade(s32 opening, PartyMenuData *state) {
    if (opening == 0) {
        if (state->fadeA > 0) {
            state->fadeA -= 0x10;
        }
        if (state->fadeA < 0) {
            state->fadeA = 0;
        }
        if (state->fadeA < 0x50) {
            if (state->fadeB > 0) {
                state->fadeB -= 0x10;
            }
            if (state->fadeB < 0) {
                state->fadeB = 0;
            }
        }
    } else {
        if (state->fadeB < 0x100) {
            state->fadeB += 0x10;
        }
        if (state->fadeB > 0x100) {
            state->fadeB = 0x100;
        }
        if (state->fadeB > 0xB0) {
            if (state->fadeA < 0x100) {
                state->fadeA += 0x10;
            }
            if (state->fadeA > 0x100) {
                state->fadeA = 0x100;
            }
        }
    }
}

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
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    if (menu->freezePanel == 0) {
        func_002B2408(context);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s32 mnuStepPartySelectionControl(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
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

void mnuReleaseStaffMenuResources(s32 menu) {
    u32 *handles = (u32 *)(menu + 8);
    s32 remaining = 1;
    do {
        effResolveAndReleaseResource(*handles++);
    } while (--remaining >= 0);
}

void mnuReleaseStaffMenuTextureHandles(s32 menu) {
    u32 *handles = (u32 *)(menu + 8);
    s32 remaining = 1;
    do {
        effReleaseTextureHandlesAndResetSlots(*handles++);
    } while (--remaining >= 0);
}

u32 mnuCreateSelectState(u32 unused, s32 flag) {
    s32 context = kwlnTaskGetUserValue();
    u32 handle = sdfAllocGeneralBlock(0x30);
    u32 *state = (u32 *)sdfResourceRetainAddress(handle);
    *(u32 **)(context + 0xaa48) = state;
    memset(state, 0, 0x30);
    state[0] = handle;
    state[5] = flag;
    if (flag == 0) {
        state[4] = 0;
    } else {
        state[4] = 1;
    }
    func_002B27F0(context);
    state[9] = 1;
    mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayResource, 0, 0);
    evtStageTestInit(0);
    return 1;
}

s32 mnuStaffCloseSelectionState(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 party = ((MenuContext *)context)->party;

    mnuResetWorkFloats();
    mnuReleaseMenuWindowHandles(context);
    sdfReleaseResourceAllocation(*(s32 *)party);
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

s32 mnuStaffPopupUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    s32 *popup = ((MenuContext *)context)->popupState;
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    u8 *window;
    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    if (*popup == 0) {
        window = (u8 *)(context + 0x284);
        func_002BD480(4, window);
        if (buttons & 1) {
            ((MenuPartyRuntime *)menu)->staffSelection = ((MenuContext *)context)->partyWindow.lists[0]->cursor->index;
            mnuSetPopupEntry(popup, D_003E75E0);
            *(s32 *)(menu + 0x24) = 1;
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(popup, D_003E75A8);
            mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayHandle, 0, 1);
            mnuClearActionFlags(0, window);
        }
        mnuPlayInputSound(0, buttons, 0);
    }
    return 0;
}

s32 mnuDrawStaffCampPageWithImage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    if (effHasFirstTextureHandle(((MenuContext *)context)->resourceHandle)) {
        mnuDrawCampIconBackdropByKind(0, callback);
    } else {
        mnuDrawCampIconBackdropByKind(1, callback);
    }
    if (menu[5] == 0) {
        mnuCreateStaffImageSprite(0x16);
    } else {
        mnuCreateStaffImageSprite(0x15);
    }
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    if (menu[9] != 0) {
        func_002AAC98(0, ((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index, D_003E69B0, context, 1, 0x53);
    }
    return menuSetHandler(context, 1, callback);
}

s32 mnuStepStaffCampPageControl(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2C88);

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

s32 mnuCreatePanels(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuContext *menuContext = (MenuContext *)context;
    MenuList *list = *(MenuList **)(context + 0xA914);
    s32 index = list->cursor->index;
    s32 data = datGameState + index * 0x1C4 + 0xA60;
    s32 window = context + 0x284;
    s32 *party = (s32 *)menuContext->party;
    s32 profile;

    mnuSetWindowResource(index, (u32 *)window, menuContext->displayHandle,
                         (s32)menuContext->displayResource,
                         menuContext->alternateResource, 0, 0);
    mnuAttachPartyIconBundle(index, window, (u32)menuContext->displayResource);
    menuContext->panelGroup = mnuCreatePanelGroup(menuContext->resourceHandle,
                                                   (s32)menuContext->displayResource, 0);
    menuContext->panelRequest = mnuCreateSpriteState(menuContext->resourceHandle,
                                                       (s32)menuContext->displayResource,
                                                       menuContext->displayHandle);
    menuContext->panelEffects = mnuAllocateSimpleSprite(menuContext->resourceHandle,
                                                         menuContext->alternateResource,
                                                         menuContext->displayHandle);
    profile = mnuCreateProfilePanel(data);
    menuContext->resourceList = profile;
    mnuSetGroupProperties(profile, menuContext->displayHandle,
                          menuContext->alternateResource, 1, 2);
    party[8] = func_002B9FF8(4, menuContext->displayHandle, *(s32 *)(context + 0x100));
    if (mnuClassifyQuarterHalfPercent(*(u16 *)(data + 6), *(u16 *)(data + 8)) < 2) {
        party[11] = -1;
    } else {
        party[11] = 0xA;
    }
    evtStageTestSelectEntry(*(u16 *)(data + 4), party[11], 0);
    func_002B2C88(window, 1, party[5], party[4]);
    func_002BAF50(0, context + 0xB10C);
    return 1;
}

s32 mnuDestroyPanels(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 window;
    func_002BAF50(((MenuContext *)context)->imageHandle, context + 0xb10c);
    window = context + 0x284;
    func_002B2C88(window, 0, menu[5], menu[4]);
    evtStageTestStop();
    mnuClearEntries(window);
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
    mnuReleaseResourceList(menu[8]);
    return 1;
}

void mnuResetSelectedPanelOpacity(s32 context) {
    ((MenuContext *)context)->partyWindow.slots[((MenuContext *)context)->partyWindow.lists[0]->cursor->index].windowSprites->fade = 0x100;
}

s32 mnuStaffSwitchPartyPage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuPartyRuntime *menu = (MenuPartyRuntime *)((MenuContext *)context)->party;
    s32 changed = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    if (buttons & 0x100) {
        mnuDestroyPanels(callback);
        mnuRetreatListCursorDefault(((MenuContext *)context)->partyWindow.lists[0]);
        changed = 1;
    }
    if ((buttons & 0x200) && changed == 0) {
        mnuDestroyPanels(callback);
        mnuAdvanceListCursorDefault(((MenuContext *)context)->partyWindow.lists[0]);
        changed = 1;
    }
    mnuClearListFlagsOneAndTwo(((MenuContext *)context)->partyWindow.lists[0]);
    if (changed != 0) {
        mnuCreatePanels(callback);
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        menu->activeMark = 0;
        if (menu->staffMode == 1) {
            mnuResetSelectedPanelOpacity(context);
        }
        return 1;
    }
    return 0;
}

s32 mnuStaffBrowsePartyUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    s32 *popup;
    u32 buttons;
    s32 state;
    if (((MenuPartyRuntime *)menu)->staffView == 0) {
        buttons = mnuMapPadMaskToFlags(0xC2);
    } else {
        buttons = mnuMapPadMaskToFlags(2);
    }
    popup = ((MenuContext *)context)->popupState;
    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    if (func_002C6480() != 0) {
        return 0;
    }
    state = *popup;
    ((MenuPartyRuntime *)menu)->staffExit = 0;
    if (state == 0) {
        if (mnuStaffSwitchPartyPage(callback) != 0) {
            return 0;
        }
        if (buttons & 0xC0) {
            if (((MenuPartyRuntime *)menu)->staffMode == 0) {
                ((MenuPartyRuntime *)menu)->staffMode = 1;
                func_002B2C88(context + 0x284, 3, ((MenuPartyRuntime *)menu)->staffView, 1);
                mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->alternateResource, 0, 0);
            } else {
                ((MenuPartyRuntime *)menu)->staffMode = 0;
                func_002B2C88(context + 0x284, 2, ((MenuPartyRuntime *)menu)->staffView, 0);
                mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayResource, 0, 0);
            }
            ((MenuPartyRuntime *)menu)->activeMark = 0;
        }
        if (buttons & 2) {
            if (func_002C6CE8() != 1) {
                evtStageTestStop();
                ((MenuPartyRuntime *)menu)->staffExit = 1;
                *(s32 *)(menu + 0x24) = 1;
                mnuSetPopupEntryFlagged(popup, D_003E75C4);
            } else {
                buttons = 0x8000;
            }
        }
        mnuPlayInputSound(0, buttons, 0);
    }
    return 0;
}

void mnuDrawSlotIcons(s32 x, s32 context) {
    s32 slot = datGameState + ((MenuPageWindow *)context)->lists[0]->cursor->index * 0x1c4 + 0xa60;
    s32 i;
    s32 y = 0xb40;
    s32 handle;
    for (i = 0; i < 2; i++, y += 0xb8) {
        handle = itfDrawUnderscoreTextSegment(0x3c0, y, 0, 0xa09dc359, D_00435E54 + *(u16 *)(slot + 4) * 45, i);
        if (handle != 0) {
            func_0019D550(handle, 1, 0x53);
            frFontQueueGlyphInSelectedSlot(handle);
        }
    }
}

void mnuDrawSelectedPartySlotMarkers(s32 context, u32 *handles) {
    s32 alpha;

    alpha = 0x100 - ((MenuPageWindow *)context)->slots[((MenuPageWindow *)context)->lists[0]->cursor->index].windowSprites->fade;
    func_00306CD0(0xa0, 0xa30, 0, alpha, 1, handles[1], 0x55, 0x53);
    func_00306CD0(0x30, 0xaf8, 0, alpha, 1, *handles, 0x1a, 0x53);
}

void mnuDrawTextSprite(s32 x, s32 y, s32 width, u32 color, s32 model, s32 flags) {
    s32 top = y - 0x10;
    s32 handle;
    frFontAddSharedGlyphFlags(1);
    handle = frFontAppendGlyphFromData(model, 0, 0, 0, 0);
    frFontSetContextPair(handle, x, top);
    frFontStoreShiftedContextValue(handle, width << 4);
    frFontSetChildColors(handle, color);
    frFontClearFlagBits(1);
    func_0019D550(handle, 1, flags);
    frFontQueueGlyphInSelectedSlot(handle);
}

void mnuDrawPartySkillAndStatusPanel(u8 *entry, s32 id, s32 packedGroup, s32 group, s32 unused, s32 spriteFlags) {
    mnuApplyPackedGroupValues(packedGroup, *(u16 *)(entry + 0x1b2));
    func_002C0D18(0xeb0, 0x518, 0, entry, packedGroup, 0, spriteFlags);
    func_002C10F0(0, 0, 0, entry, group, spriteFlags);
    mnuDrawTextSprite(0x2a0, 0xa50, 0, 0xa09dc380, D_00435E48 + *(u16 *)(entry + 4) * 0x11 + 0x110, spriteFlags);
    mnuDrawSlotIcons(0x14a, id);
}

void mnuDrawProfilePanelAndSprite(u32 entry, u32 unused1, u32 group, u32 resource,
                                    u32 unused4, u32 spriteFlags) {
    func_002C16F0(0, 0, 0, entry, *(u8 *)((s32)entry + 0x55), group, spriteFlags);
    mnuDrawAndAdvanceProfilePanel(0xe80, 0x5b8, 0, resource, spriteFlags);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3788);

void mnuIdleVoiceTimer(s32 object) {
    u32 count;
    if (((MenuVoiceState *)object)->motionSelection == -1) {
        if (func_002C6CE8() != 1) {
            if (evtStageTestHasPendingMotion() == 0) {
                ((MenuVoiceState *)object)->idleFrames += 1;
            }
            if (((MenuVoiceState *)object)->idleFrames >= 0x12d) {
                count = evtStageTestCountFlags(0);
                evtStageTestQueueMotion(0, effMiscRand(0) % count);
                ((MenuVoiceState *)object)->idleFrames = 0;
            }
        }
    }
}

s32 mnuStaffIdlePartyUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    if (menu[4] == 0) {
        mnuIdleVoiceTimer(menu);
    }
    return menuSetHandler(context, 2, callback);
}

u32 func_002B3A58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3A60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3CA0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3E80);

u32 mnuDestroySelectedPartyWindow(u32 callback) {
    s32 party;

    party = kwlnTaskGetUserValue();
    party = ((MenuContext *)party)->party;
    mnuDestroyWindowContainer((u32)((MenuPartyRuntime *)party)->selectedWindow);
    ((MenuPartyRuntime *)party)->selectedWindow = 0;
    return 1;
}

void mnuSeekFirstAvailableStaffListNode(void) {
    MenuPartyRuntime *party = (MenuPartyRuntime *)((MenuContext *)kwlnTaskGetUserValue())->party;
    MenuListNode *node = party->selectedWindow->list->first;

    while (node != NULL) {
        u32 key = node->sortKeyPrimary;
        if (!(node->flags48 & 1) && key != 0) {
            break;
        }
        node = node->next;
    }
    if (node != NULL) {
        mnuSeekListNode(node->index, party->selectedWindow->list);
    }
}

MenuWindowContainer *mnuSeekSelectedWindowCursor(s32 selectionMode, s32 callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    MenuPartyRuntime *party = (MenuPartyRuntime *)((MenuContext *)context)->party;
    s32 window = context + 0xB10C;
    s32 id;
    MenuWindowContainer *selected;
    id = party->selectedWindow->list->cursor->index;
    mnuDestroySelectedPartyWindow(callback);
    func_002B3E80(selectionMode, callback);
    mnuSeekListNode(id, party->selectedWindow->list);
    selected = party->selectedWindow;
    selected->scale50 = 0x200;
    selected->scale54 = 0x100;
    func_002BAF50(0, window);
    func_002BAF50(party->selectedWindow, window);
    party->selectedWindow->panel.fade = 0x100;
    *(s32 *)(context + 0xB1CC) = 0x200;
    return party->selectedWindow;
}

void func_002B4270(u32 context) {
    mnuSwitchCampVisualCategory(1, context);
}

void func_002B4290(s32 context) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4298);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B45D8);

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

void func_002B47F8(void) {
    sdfReleaseChipBlock();
}

s32 mnuIsSkillCodeInBitset(s32 skillCode, u32 *bits) {
    s32 wordIndex = (skillCode < 0) ? skillCode + 0x1f : skillCode;

    return (bits[wordIndex >> 5] & (1 << skillCode)) != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4848);

void mnuDestroySkillMenuWindows(s32 context) {
    u32 *menu = (u32 *)((MenuContext *)context)->party;
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

u32 mnuCreateItemState(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    u32 handle = sdfAllocGeneralBlock(0x3c);
    u32 *state = (u32 *)sdfResourceRetainAddress(handle);
    *(u32 **)(context + 0xaa48) = state;
    memset(state, 0, 0x3c);
    state[0] = handle;
    func_002B4270(context);
    switch (((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index) {
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
    if (((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xe);
    }
    func_002AAC98(0, ((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index, D_003E69B0, context, 1, 0x53);
    /* Both arms are identical in retail; kept as written. */
    if (((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index == 0) {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s32 mnuStepSkillSlotControl(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
}

void mnuClearSelectedListNodeId() {
    s32 context;

    context = kwlnTaskGetUserValue();
    ((MenuPartyRuntime *)((MenuContext *)context)->party)->selectedIndex = 0xffffffff;
}

u32 mnuHasSelectedListNodeId(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    return ~((MenuPartyRuntime *)((MenuContext *)context)->party)->selectedIndex >> 0x1f;
}

void mnuHighlightSelectedListNode() {
    u8 *state = (u8 *)((MenuContext *)kwlnTaskGetUserValue())->party;
    u8 *node = (u8 *)((MenuPartyRuntime *)state)->selectedWindow->list->first;
    while (node != NULL) {
        if (((MenuListNode *)node)->index == ((MenuPartyRuntime *)state)->selectedIndex) {
            ((MenuListNode *)node)->flags48 |= 2;
        } else {
            ((MenuListNode *)node)->flags48 &= ~2;
        }
        node = (u8 *)((MenuListNode *)node)->next;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5240);

u32 mnuClearSkillSelectionImageState(void) {
    s32 context = kwlnTaskGetUserValue();
    u32 *state = (u32 *)((MenuContext *)context)->party;
    s32 image = ((MenuContext *)context)->imageHandle;
    if (((MenuWindowContainer *)image)->list->cursor->index == 0) {
        func_002BAF50(image, context + 0xb10c);
    }
    state[12] = 0;
    return 1;
}

void mnuAddPartySkillIfMissing(s32 obj, s32 id, s32 slot) {
    u16 code = id;

    if (ptyHasSkill(obj, code) == 0) {
        *(u16 *)(obj + slot * 2 + 0x22) = code;
        ptyRecomputeMaxHpMp(obj);
        scrClearSecondaryScriptFlag(obj, code);
    }
}

void mnuClearPartySkillSlot(s32 party, s32 slot) {
    *(u16 *)(slot * 2 + party + 0x22) = 0;
    ptyRecomputeMaxHpMp();
}

void mnuCampMenuHandleInput(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 menu = ((MenuContext *)context)->party;
    u32 input = mnuMapPadMaskToFlags(0x33);
    MenuWindowContainer *window = ((MenuPartyRuntime *)menu)->selectedWindow;
    MenuList *list = window->list;

    list->stateFlags &= ~8;
    if (input & 1) {
        MenuListNode *entry = list->cursor;
        s32 target = entry->sortKeyPrimary;

        if (!(entry->flags48 & 1) && target != 0) {
            mnuSetPopupEntry(context + 0x54, D_003E7774);
        } else {
            input = 0x8000;
        }
    }
    if (input & 2) {
        mnuSetPopupEntryFlagged(context + 0x54, D_003E773C);
    }
    if (window != 0) {
        if (!(input & 0x300000)) {
            func_002B9808((s32)window);
        }
        if (input & 0x10) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (input & 0x20) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, input, (s32)window->list);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5580);

void mnuSwapPartySkillSlots(s32 party, s32 firstSlot, s32 secondSlot) {
    u8 *entries = (u8 *)(party + 2);
    s32 firstOffset = firstSlot * 2 + 32;
    s32 secondOffset = secondSlot * 2 + 32;
    u16 firstValue = *(u16 *)(entries + firstOffset);
    u16 secondValue = *(u16 *)(entries + secondOffset);

    *(u16 *)(entries + firstOffset) = secondValue;
    *(u16 *)(entries + secondOffset) = firstValue;
}

void ptySkillMenuHandleSlotReorder(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 menu = ((MenuContext *)context)->party;
    u32 input = mnuMapPadMaskToFlags(0x37);
    MenuWindowContainer *window = ((MenuPartyRuntime *)menu)->selectedWindow;
    s32 slot = datGameState + ((MenuContext *)context)->partyWindow.lists[0]->cursor->index * 0x1c4 + 0xa60;
    MenuList *list = window->list;

    list->stateFlags &= ~8;
    if (input & 1) {
        s32 selected = list->cursor->index;

        if (mnuHasSelectedListNodeId(callback) == 0) {
            ((MenuPartyRuntime *)menu)->selectedIndex = selected;
        } else if (selected != ((MenuPartyRuntime *)menu)->selectedIndex) {
            mnuSwapPartySkillSlots(slot, ((MenuPartyRuntime *)menu)->selectedIndex, selected);
            mnuClearSelectedListNodeId(callback);
            window = mnuSeekSelectedWindowCursor(0, callback);
        } else {
            input = 0x8000;
        }
    }
    if (input & 6) {
        input = 2;
        if (mnuHasSelectedListNodeId(callback) == 0) {
            mnuSetPopupEntryFlagged(context + 0x54, D_003E7790);
        }
        mnuClearSelectedListNodeId(callback);
    }
    mnuHighlightSelectedListNode(callback);
    if (window != 0) {
        if (!(input & 0x300000)) {
            func_002B9808((s32)window);
        }
        if (input & 0x10) {
            mnuRetreatWindowListSelection((s32)window);
        }
        if (input & 0x20) {
            mnuAdvanceWindowListSelection((s32)window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        mnuPlayInputSound(0, input, (s32)window->list);
    }
}

s32 ptySkillMenuUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 state = func_002C4038(context + 8, ((MenuContext *)context)->popupState, 0, callback);
    if (state != 0) {
        return state;
    }
    if (((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index == 0) {
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
    if (((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index == 0) {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
        ptySkillMenuCopyPageState(context);
    }
    func_002AAE80(callback);
    if (((MenuWindowContainer *)((MenuContext *)context)->imageHandle)->list->cursor->index == 0) {
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
    return menuSetHandler(context, 1, callback);
}

s32 ptySkillMenuDispatchPageRequest(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
}

s32 ptySkillMenuApplyFieldUseAndCost(id, context)
    u16 id;
    s32 context;
{
    s32 window = context + 0x284;
    s32 slotA = datGameState + ((MenuContext *)context)->partyWindow.lists[0]->cursor->index * 0x1c4 + 0xa60;
    s32 slotB = datGameState + ((MenuContext *)context)->partyWindow.lists[1]->cursor->index * 0x1c4 + 0xa60;
    if (mnuIsEntryCostUnaffordable(id, slotA) != 0) {
        return 0;
    }
    if (ptySkillApplyFieldUseEffect(window, id, slotA, slotB) != 0) {
        mnuConsumeEntryCost(id, slotA);
        mnuInitPartyPanelSlots(context + 0xA928);
        func_002BCA98(window);
        func_002BCAB0(window);
        return 1;
    }
    return 0;
}

/* Mark entries whose field-use cost cannot be paid by the selected party member. */
void mnuFlagMatchingEntries(s32 context) {
    s32 slot = datGameState + ((MenuContext *)context)->partyWindow.lists[0]->cursor->index * 0x1c4 + 0xa60;
    MenuListNode *link = ((MenuPartyRuntime *)((MenuContext *)context)->party)->selectedWindow->list->first;
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
    u8 *window;
    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    label = ((MenuPartyRuntime *)menu)->selectedWindow->list->cursor->sortKeyPrimary;
    code = label;
    if (mnuGetAbilityByteCategory(code) == 2) {
        ((MenuContext *)context)->partyWindow.flags |= 0x10;
    }
    if (mnuGetAbilityByteCategory(code) == 3) {
        ((MenuContext *)context)->partyWindow.flags |= 0x20;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(8, window);
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
    func_002AAC70(0, ((MenuPartyRuntime *)menu)->selectedWindow->list->cursor->sortKeyPrimary, D_00435E6C, context, 1, 1, 0x53);
    ((MenuPartyRuntime *)menu)->selectedWindow->list->stateFlags &= ~8;
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s32 ptySkillMenuDispatchConfirmRequest(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
}

s32 ptySkillMenuOpenPartyPage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 party = ((MenuContext *)context)->party;
    MenuWindowContainer *window;

    mnuSelectPage(&((MenuContext *)context)->partyWindow, ((MenuContext *)context)->partyWindow.lists[0]->cursor->index);
    ((MenuContext *)context)->partyWindow.flags |= 0x200;
    func_002B3E80(0, callback);
    func_002B4848(context);
    window = ((MenuPartyRuntime *)party)->selectedWindow;
    window->list->stateFlags |= 8;
    func_002BAF50(window, context + 0xB10C);
    return 1;
}

u32 ptySkillMenuClosePartyPage(u32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002BAF50(((MenuContext *)context)->imageHandle, context + 0xb10c);
    mnuDestroySelectedPartyWindow(callback);
    mnuDestroySkillMenuWindows(context);
    mnuClearPageSelectionHandles(context + 0x284);
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
    s32 state = func_002C4038(context + 8, popup, 0, callback);
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
    mnuPlayInputSound(0, buttons, menu[3]);
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
    mnuPlayInputSound(0, input, (s32)window->list);
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
    ((MenuPartyRuntime *)menu)->selectedWindow->list->stateFlags |= 8;
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(3, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s32 func_002B6800(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
}

void mnuDrawSelectionLabel(u16 id) {
    s32 label = itfDrawTextWithSelectedFontMode(0x11B0, 0xA88, 0, 0, id, 1);

    frFontSetChildColors(label, 0xA09DC35A);
    func_0019D550(label, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(label);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6898);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6B00);

typedef struct PtyFrontlineSlot {
    u16 flags;          /* 0x00: bit 0 present, bit 1 frontline */
} PtyFrontlineSlot;

/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(s32 **out, s32 max) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < max; i++) {
        out[i] = 0;
    }
    i = 0;
    while (count < max) {
        PtyFrontlineSlot *entry = (PtyFrontlineSlot *)(datGameState + i * 0x1C4 + 0xA60);

        if ((entry->flags & 1) != 0 && (entry->flags & 2) != 0) {
            out[count] = (s32 *)entry;
            count++;
        }
        i++;
        if (i >= 5) {
            break;
        }
    }
}

s32 mnuHasAvailableSlotResource(s32 id) {
    MenuSlot *entry;
    s32 i;
    id -= 0x1ab;
    entry = (MenuSlot *)((id << 4) + (s32)datAffinityRecords);
    if ((entry->flags & 2) != 0) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (entry->resources[i] != -1) {
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
    mnuDestroyWindowContainer((u32)((MenuPartyRuntime *)party)->selectedWindow);
    ((MenuPartyRuntime *)party)->selectedWindow = 0;
    return 1;
}

u32 mnuResetSelection(u32 callback) {
    s32 context;
    u32 *state;
    mnuCreateItemState(callback);
    context = kwlnTaskGetUserValue(callback);
    state = (u32 *)((MenuContext *)context)->party;
    func_002B6D78(callback);
    mnuFlagActiveWindows(context + 0x284);
    state[11] = 0;
    func_002BAF50(state[9], context + 0xb10c);
    return 1;
}

u32 mnuCloseSkillSelection(u32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002BAF50(((MenuContext *)context)->imageHandle, context + 0xb10c);
    mnuDestroySkillSelectionWindow(callback);
    mnuClearPartyPanelActiveFlags(context + 0x284);
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
    state = func_002C4038(context + 8, popup, 0, callback);
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
    mnuPlayInputSound(0, buttons, (s32)((MenuWindowContainer *)list[8 + menu[11]])->list);
    if (buttons & 2) {
        mnuSetPopupEntryFlagged(popup, D_003E7720);
        mnuConfigurePanelResource(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayHandle, 0, 1);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7228);

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
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    label = ((MenuWindowContainer *)*(s32 *)(menu + 0x24 + *(s32 *)(menu + 0x2c) * 4))->list->cursor->sortKeyPrimary;
    func_002B7228(context);
    if (label != 0 && label != 0xffff) {
        label = (u16)label;
        mnuDrawSelectionLabel(label);
        func_002B6898(label, ((MenuContext *)context)->resourceHandle, ((MenuContext *)context)->labelHandle);
    }
    func_002AA7A0(2, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s32 func_002B76B0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, callback);
}

typedef struct MapPacket {
    u32 type;
    u32 value;
    u32 unk_08;
    u32 items[11];
    s32 count;
} MapPacket;

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

void mnuCopyCampEffectRowData(s32 sourceBase, s32 destinationBase) {
    u32 *source;
    s32 offset;
    u32 *destination;
    s32 remaining;
    s32 sourceIndex;
    u32 row;

    row = 0;
    sourceIndex = 0;
    do {
        destination = (u32 *)(destinationBase + 0x40);
        offset = sourceIndex << 2;
        remaining = 3;
        do {
            source = (u32 *)(offset + sourceBase);
            offset = offset + 4;
            remaining = remaining - 1;
            *destination = *source;
            destination = destination + 1;
        } while (-1 < remaining);
        row = row + 1;
        destinationBase = destinationBase + 0x10;
        sourceIndex = sourceIndex + 4;
    } while (row < 2);
}

void mnuSetCampEffectResourceHandles(u32 first, u32 second, u32 *menu) {
    menu[2] = first;
    menu[15] = second;
}

typedef struct MenuEffectResources {
    MapPacket packet;      /* 0x00–0x3B */
    u32 animationHandle;   /* 0x3C */
} MenuEffectResources;

void mnuBindCampEffectAnimation(s32 resources) {
    effConfigureIndexedSlotResource(((MenuEffectResources *)resources)->packet.unk_08,
                   ((MenuEffectResources *)resources)->packet.items[4],
                   ((MenuEffectResources *)resources)->animationHandle, 0, 4);
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

void mnuLoadEffectResources(u8 *effect) {
    MenuEffectResources *resources = (MenuEffectResources *)effect;

    mnuInitializeMapPacket(0, D_003E7828, 0xb, &resources->packet);
    mnuCopyCampEffectRowData((s32)D_003E7858, (s32)effect);
    resources->packet.unk_08 = effLoadIndexedResource("/camp/spr/n_min/", D_003E7818[0], 0);
    resources->animationHandle = effLoadMappedResource("/camp/mot/", D_003E7820[0]);
    mnuBindCampEffectAnimation((s32)effect);
}

void mnuRequestEffectResources(u8 *effect) {
    mnuInitializeMapPacket(0, D_003E7828, 0xb, (MapPacket *)effect);
    mnuCopyCampEffectRowData((s32)D_003E7858, (s32)effect);
    effRequestResourceByMode("/camp/spr/n_min/", D_003E7818[0], 0, (u32 *)(effect + 8));
    effRequestMappedResource("/camp/mot/", D_003E7820[0], (u32 *)(effect + 0x3c));
}

u32 mnuBindCampEffectWhenLoaded(u32 *menu) {
    if (menu[2] == 0) {
        return 0;
    }
    if (menu[15] == 0) {
        return 0;
    }
    mnuBindCampEffectAnimation(menu);
    return 1;
}

void mnuDestroyEffectResources(u8 *ctx) {
    u32 i;
    for (i = 0; i < 1; i++) {
        effDestroyResourceSlotSet(*(u32 *)(ctx + 8 + i * 4));
    }
    effDestroyPackedBatch(((MenuEffectResources *)ctx)->animationHandle);
}

typedef struct MenuSparkSet {
    u32 flags;
    u8 pad04[4];
    s32 sheet;       /* 0x08 */
    s32 handle[8];   /* 0x0C */
    u8 pad2C[0x60 - 0x2C];
    /* 0x060 */ s32 direction[16];
    /* 0x0A0 */ s32 velocity[16][2];
    /* 0x120 */ s32 life[16];
    /* 0x160 */ s32 count;
} MenuSparkSet;

void mnuSpawnSpark(MenuSparkSet *fx) {
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

void mnuRetireCampSpark(s32 effects, s32 index) {
    ((MenuSparkSet *)effects)->direction[index] = 0;
    ((MenuSparkSet *)effects)->count = ((MenuSparkSet *)effects)->count - 1;
}

void mnuDrawAndAdvanceCampSparks(MenuSparkSet *fx, s32 arg) {
    s32 i;
    for (i = 0; i < 0x10; i++) {
        if (fx->direction[i] > 0) {
            itfDrawGridWithResolvedSlot(fx->velocity[i][0], fx->velocity[i][1], 0, 0, fx->sheet, fx->handle[6], arg);
            if (fx->direction[i] == 1) {
                fx->velocity[i][0] += fx->life[i];
                if (fx->velocity[i][0] > 0x2000) {
                    mnuRetireCampSpark((s32)fx, i);
                }
            } else {
                fx->velocity[i][0] -= fx->life[i];
                if (fx->velocity[i][0] < -0xFA0) {
                    mnuRetireCampSpark((s32)fx, i);
                }
            }
        }
    }
    if (fx->flags & 4) {
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

typedef struct MenuBadgeSet {
    u32 flags;
    u8 pad4[4];
    s32 sheet;       /* 0x08 */
    s32 handle[8];   /* 0x0C */
    u8 pad2C[0x164 - 0x2C];
    s32 fade;        /* 0x164 */
} MenuBadgeSet;

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

void mnuDrawBadgeFade(MenuBadgeSet *set, s32 arg) {
    MenuBadgeLayout layout = D_0042AED0;
    s32 handle;
    if (!(set->flags & 4)) {
        handle = set->handle[layout.place[0].slot];
        func_00306CD0(layout.place[0].x, layout.place[0].y, 0, set->fade, 0, set->sheet, handle, arg);
        itfGridLookupValueOrDefault(set->sheet, handle);
        MNU_ADVANCE_FADE(set->fade, 0x10, 0x100);
    }
    if (!(set->flags & 2)) {
        itfDrawGridWithResolvedSlot(layout.place[1].x, layout.place[1].y, 0, 0, set->sheet, set->handle[layout.place[1].slot], arg);
    }
}

extern MenuBadgeLayout D_0042AEE8;

void mnuDrawCampIconBackdrop(MenuBadgeSet *set, s32 arg) {
    MenuBadgePlace blank[1];
    MenuBadgeLayout layout;
    u32 i;

    memset(blank, 0, sizeof(blank));
    layout = D_0042AEE8;
    sdfSubmitGsTestOneRegisterPacket(0x30000, arg);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x80808080, arg);
    for (i = 0; i < 1; i++) {
        itfDrawGridWithResolvedSlot(blank[i].x, blank[i].y, 0, 0, set->sheet, set->handle[blank[i].slot], arg);
    }
    if (!(set->flags & 2)) {
        for (i = 0; i < 2; i++) {
            itfDrawGridWithResolvedSlot(layout.place[i].x, layout.place[i].y, 0, 0, set->sheet, set->handle[layout.place[i].slot], arg);
        }
    }
    if (!(set->flags & 4)) {
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
    node->rowHeight = rowSpacing * 8;
    node->scale = 0x100;
    node->head = NULL;
    node->first = NULL;
    node->windowOffset = 0;
    node->cursor = NULL;
    return node;
}

u32 mnuDestroyListState(MenuList *list) {
    s64 result;

    do {
        result = func_002B86E8((u32)list);
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
    s32 value;
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B83A0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B86E8);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;

typedef struct MenuSpriteGrid {
    s32 pad0[2];
    MenuSpriteRef slots[8];
} MenuSpriteGrid;

void mnuSetGridSpriteSlot(MenuSpriteGrid *grid, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect) {
    grid->slots[row * 4 + col].sprite = sprite;
    grid->slots[row * 4 + col].effect = effect;
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
            cursor->fadeCounter = 0x100;
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
            cursor->fadeCounter = 0x100;
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

void mnuAdvanceListCursorDefault(u32 list) {
    mnuListAdvanceCursor(list, 0, 0);
}

void mnuRetreatListCursorDefault(u32 list) {
    mnuListRetreatCursor(list, 0, 0);
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
s32 mnuGetListViewportHeight(MenuList *list) {
    return list->rowHeight * list->visibleCount;
}

/* Cancel the pending animation on every node in this list. */
void mnuResetListNodeFadeCounters(MenuList *list) {
    s32 node;

    node = (s32)list->first;
    if (node != 0) {
        ((MenuListNode *)node)->fadeCounter = 0;
        while (node = (s32)((MenuListNode *)node)->next, node != 0) {
            ((MenuListNode *)node)->fadeCounter = 0;
        }
    }
}

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(u8 *menu) {
    u8 *node = (u8 *)((MenuList *)menu)->first;
    if (node != NULL) {
        do {
            s32 timer = ((MenuListNode *)node)->fadeCounter;
            s32 reduced = timer - MNU_NODE_FADE_STEP;
            if (timer > 0) {
                ((MenuListNode *)node)->fadeCounter = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                ((MenuListNode *)node)->fadeCounter = 0;
            }
            node = (u8 *)((MenuListNode *)node)->next;
        } while (node != NULL);
    }
}

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, s32 menu, s32 panel, s32 drawArg) {
    s32 spriteBase = panel + 8;
    s32 effectBase = panel + 0xC;
    u32 spriteIndex = 0;
    do {
        s32 selected = panel == (s32)((MenuList *)menu)->cursor;
        s32 slotOffset = (selected * MNU_ENTRY_SPRITE_COUNT + spriteIndex) * 8;
        s32 sprite = *(s32 *)(spriteBase + slotOffset);
        if (sprite != 0) {
            itfDrawGridWithResolvedSlot(x, y, depth, 0, sprite, *(s32 *)(effectBase + slotOffset), drawArg);
        }
        spriteIndex++;
    } while (spriteIndex < MNU_ENTRY_SPRITE_COUNT);
}

extern u32 uiBlendColors(u32 color, u32 previous, s32 blend);

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, u8 *entry) {
    u32 flags = ((MenuListNode *)entry)->flags48;
    u32 color = MNU_ENTRY_MARKED_COLOR;
    if (!(flags & 1)) {
        color = (flags & 4) ? MNU_ENTRY_ALTERNATE_COLOR : MNU_ENTRY_DEFAULT_COLOR;
    }
    return uiBlendColors(color, previousColor, ((MenuListNode *)entry)->fadeCounter);
}

typedef struct MenuSlotEntry {
    u8 pad0[0x14];
    s32 colors[4];
    u8 pad24[0x7C];
} MenuSlotEntry;

typedef struct MenuSlotSet {
    u8 pad0[0x18];
    MenuSlotEntry *entries;
} MenuSlotSet;

/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(MenuSlotSet *menu, s32 index, u8 *entry) {
    s32 colorIndex;

    for (colorIndex = 0; colorIndex < MNU_ENTRY_COLOR_COUNT; colorIndex++) {
        s32 previousColor = menu->entries[index].colors[colorIndex];

        menu->entries[index].colors[colorIndex] = mnuBlendListNodeColorByFlags(previousColor, entry);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9218);

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
    menu->layout3C = layout;
}

void mnuSetWindowContainerState(MenuWindowContainer *menu, u32 state) {
    menu->state = state;
}

void mnuSetWindowContainerLayout(MenuWindowContainer *menu, u32 layout2C, u32 layout30, u32 layout34,
                                    u32 layout48, u32 layout38, u32 layout3C, u32 layout40,
                                    u32 layout4C) {
    menu->layout2C = layout2C;
    menu->layout4C = layout4C;
    menu->layout30 = layout30;
    menu->layout34 = layout34;
    menu->layout38 = layout38;
    menu->layout48 = layout48;
    menu->layout3C = layout3C;
    menu->layout40 = layout40;
    menu->layout44 = 0;
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

void mnuAppendWindowListNode(MenuWindowContainer *menu) {
    mnuListAppendNode((u32)menu->list);
}

void func_002B9708(MenuWindowContainer *menu) {
    func_002B83A0((u32)menu->list);
}

void func_002B9720(MenuWindowContainer *menu) {
    func_002B86E8((u32)menu->list);
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

struct MenuIconSprites {
    u32 handle;
    u32 value;
    u32 unk8;
    void *sprite[3];
};

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value, s32 res, s32 *idx, s32 unused) {
    obj->value = value;
    obj->sprite[0] = (void *)effCreateResourceSlotSet(res, idx[0], 1);
    obj->sprite[1] = (void *)effCreateResourceSlotSet(res, idx[1], 1);
    obj->sprite[2] = (void *)effCreateResourceSlotSet(res, idx[2], 1);
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9A40);

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
            func_00306CD0(x + window->originX, i * window->list->rowHeight + y + window->originY, flags, state, 1,
                          window->sprite20, window->param24, option);
        }
    }
}

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option) {
    mnuDrawWindowIconRows(x, y, flags, window, window->list->visibleCount, option);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9CF8);

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
        y += list->windowOffset * list->rowHeight;
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
    func_002B9CF8(x, y, depth, menu, drawArg);
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

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE90);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AEA0);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AED0);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AEE8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF00);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9FF8);

extern void effInitializeSlotWork();


/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuHideIconGroup(MenuIconState *group) {
    s32 spriteIndex;
    if (group->sprite[0] != NULL && group->kind < MNU_PANEL_KIND_LIMIT) {
        for (spriteIndex = 0; spriteIndex < group->count; spriteIndex++) {
            MenuSprite *obj = group->sprite[spriteIndex];
            u32 *flags = &obj->inner->flags;
            *flags &= ~1;
            effInitializeSlotWork(obj, 0);
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
    s32 shade = state->sprite[0]->inner->shadeSource << 4;
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
    state->sprite[0]->inner->shade = shade;
    if (state->count >= 3) {
        state->sprite[2]->inner->shade = shade;
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
INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF48);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF60);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF90);

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

struct MenuFadeFields {
    MenuWindowContainer previousWindow;
    u8 pad98[4];
    MenuIconSprites savedResource;
    u32 hasResourceCopy;
    s32 previousProgress;
    MenuWindowContainer *currentWindow;
    s32 currentProgress;
};

/* Reset the original 0x98-byte prefix, then initialize the later fade fields. */
void mnuInitializeWindowFadeState(MenuFadeFields *menu) {
    memset(menu, 0, 0x98);
    menu->hasResourceCopy = 0;
    menu->previousProgress = 0x200;
    menu->currentWindow = NULL;
    menu->currentProgress = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAF50);

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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB510);

/* Three resource-slot handles at +0xE4/+0xE8/+0xEC. */
typedef struct MenuSlotEffectHandles {
    u8 pad00[0xE4];
    u32 handles[3];
} MenuSlotEffectHandles;

void mnuLoadPanelSectionResources(MenuSlotEffectHandles *slot, u32 model, u32 firstValue, u32 secondValue, s32 thirdValue
                                    ) {
    u32 handle;

    handle = effCreateResourceSlotSet(model, firstValue, 1);
    slot->handles[0] = handle;
    handle = effCreateResourceSlotSet(model, secondValue, 1);
    slot->handles[1] = handle;
    if (-1 < thirdValue) {
        handle = effCreateResourceSlotSet(model, thirdValue, 1);
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
    entry->hp.unk48 = 0;
    entry->mp.unk48 = 0;
    if (preserve == 0) {
        entry->hp.unk44 = 0x100;
        entry->mp.unk44 = 0x100;
    }
}

void func_002BB9C8(u32 *destination, u32 value) {
    *destination = value;
}

typedef struct MenuPageParams {
    u8 unk0[0x64];
    s32 field64;
    s32 field68;
    s32 field6C;
    s32 field70;
} MenuPageParams;

void mnuSetPageParams(MenuPageParams *page, s32 mode) {
    switch (mode) {
    case 0:
        page->field68 = 0;
        page->field64 = 0;
        page->field6C = 0x40;
        page->field70 = 0x100;
        break;
    case 1:
        page->field68 = 1;
        page->field64 = 0x100;
        page->field6C = 0;
        page->field70 = 0x1000;
        break;
    default:
        page->field68 = 0;
        page->field64 = 0x100;
        page->field6C = 0;
        page->field70 = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBA38);

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
    ((MenuPageWindow *)menu)->slots[index].windowSprites = func_002BBA38(0, a2, a3, a4, a5, a6);
    ((MenuPageWindow *)menu)->flags |= 0x80;
}

void mnuSetIndexedWindowPageSpriteFlags(s32 index, u8 *menu, u32 first, u32 second) {
    MenuSprites **slot = &((MenuPageWindow *)menu)->slots[index].windowSprites;
    if (*slot != NULL) {
        (*slot)->unk74 = first;
        (*slot)->unk75 = second;
    }
}

void mnuClearEntries(u8 *menu) {
    MenuSprites **entry = &((MenuPageWindow *)menu)->slots[0].windowSprites;
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

extern void itfSetGridEntryQuantizedAndRefresh();

typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

/* The allocated three-icon bundle also owns its draw fade and fade direction. */
typedef struct MenuIconBundle {
    u32 unk0[3];
    void *sprite[3];
    s32 fade;
    s32 fadeOut;
} MenuIconBundle;

extern MenuIconLayout D_0042AFD8;

u32 mnuCreateIconBundle(u32 resource) {
    MenuIconLayout layout = D_0042AFD8;
    MenuIconBundle *set = (MenuIconBundle *)sdfAllocSizeClassBlock(0x20);
    u32 i;
    memset(set, 0, 0x20);
    for (i = 0; i < 3; i++) {
        void *sprite = (void *)effCreateResourceSlotSet(resource, layout.entry[i].id, 1);
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
    ((MenuPageWindow *)menu)->slots[index].iconBundle = bundle;
}

void mnuReleasePartyIconBundles(u8 *menu) {
    u32 *bundle = &((MenuPageWindow *)menu)->slots[0].iconBundle;
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC498);

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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC690);

void mnuRefreshWindowSlots(MenuPageWindow *menu, s32 flag) {
    u32 i;
    s32 offset;
    s32 *res;
    if (flag == 0) {
        for (i = 0, res = menu->handlesA; i < 8; i++, res++) {
            if (effHasFirstTextureHandle(*res) != 0) {
                effReleaseTextureHandlesAndResetSlots(*res);
                effReleaseTextureHandlesAndResetSlots(res[8]);
            }
        }
    }
    for (i = 0, offset = 0; i < 5; i++, offset += 0x34) {
        MenuPageRecord *entries = menu->records;
        if (((MenuPageEntry *)((u8 *)entries->entries + offset))->gauge.resourceIndex >= 0) {
            if ((s32)i < entries->visibleCount) {
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCAB0);

extern char D_00437C30[];

/* Create both party-panel lists and append their shared row labels. */
void mnuInitScrollLists(MenuPageWindow *menu, s32 *counts) {
    s32 i = 0;
    menu->lists[0] = mnuCreateListState(0, 1, 1);
    menu->lists[1] = mnuCreateListState(0, 1, 1);
    if (counts[0] + counts[1] > 0) {
        do {
            mnuListAppendNode(menu->lists[0], D_00437C30);
            i++;
            mnuListAppendNode(menu->lists[1], D_00437C30);
        } while (i < counts[0] + counts[1]);
    }
}

void mnuDestroyWindowOwnedLists(context)
    MenuPageWindow *context;
{
    mnuDestroyListState(context->lists[0]);
    mnuDestroyListState(context->lists[1]);
}

void mnuRebuildScrollLists(u32 context, u32 counts) {
    mnuDestroyWindowOwnedLists();
    mnuInitScrollLists(context, counts);
}

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

void mnuClearPageSelection(MenuWindowSet *set) {
    if (set->selected >= 0) {
        MenuSlotWindow *slots = set->slots;
        slots[set->selected].unkC0 = 0x100;
        slots[set->selected].unk110 = 0x100;
        set->selected = -1;
    }
    set->flags &= ~0x200;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCD90);


void mnuFreeWindowSprites(MenuPageSlot *win) {
    u32 i;
    for (i = 0; i < 3; i++) {
        if (win->icon[i] != 0) {
            effDestroyResourceSlotSet(win->icon[i]);
        }
    }
    if (win->frame[0] != 0) {
        effDestroyResourceSlotSet(win->frame[0]);
    }
    if (win->frame[1] != 0) {
        effDestroyResourceSlotSet(win->frame[1]);
    }
    if (win->frame[2] != 0) {
        effDestroyResourceSlotSet(win->frame[2]);
    }
    if (win->frame[3] != 0) {
        effDestroyResourceSlotSet(win->frame[3]);
    }
    if (win->frame[4] != 0) {
        effDestroyResourceSlotSet(win->frame[4]);
    }
    if (win->frame[5] != 0) {
        effDestroyResourceSlotSet(win->frame[5]);
    }
    if (win->frame[6] != 0) {
        effDestroyResourceSlotSet(win->frame[6]);
    }
    if (win->frame[7] != 0) {
        effDestroyResourceSlotSet(win->frame[7]);
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
    s32 offset = 0;
    u32 i;

    for (i = 0; i < 5; i++, offset += sizeof(MenuPageEntry)) {
        if (i != selected) {
            s32 id = ((MenuPageEntry *)((u8 *)window->records->entries + offset))->gauge.resourceIndex;

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
    MenuPageEntry *record;

    for (i = 0; i < 5; i++) {
        record = &window->records->entries[i];
        id = record->gauge.resourceIndex;
        if (id >= 0) {
            if (effHasFirstTextureHandle(window->handlesA[id]) != 0) {
                effReleaseTextureHandlesAndResetSlots(window->handlesA[id]);
                effReleaseTextureHandlesAndResetSlots(window->handlesB[id]);
            }
        }
    }
    record = &window->records->entries[selected];
    id = record->gauge.resourceIndex;
    if (id >= 0) {
        if (effHasFirstTextureHandle(window->handlesA[id]) == 0) {
            effResolveAndReleaseResource(window->handlesA[id]);
            effResolveAndReleaseResource(window->handlesB[id]);
        }
    }
}


extern s32 mnuGetSelectionFromFlags(s32);

void mnuSelectPage(MenuPageWindow *window, s32 selected) {
    s32 *resource = window->handlesC;
    u32 i;
    /* Required to match: retain the header-relative handle walk below. */
    u8 *handles = window->pad4;
    MenuPageEntry *record;
    s32 active;

    for (i = 0; i < 5; i++) {
        effReleaseTextureHandlesAndResetSlots(*resource++);
    }
    record = &window->records->entries[selected];
    active = mnuGetSelectionFromFlags(datGameState + record->partyIndex * 0x1C4 + 0xA60);
    for (i = 0; i < 5; i++) {
        if (i == active) {
            effResolveAndReleaseResource(*(s32 *)(handles + 0x60 + i * 4));
        }
    }
    if (window->selected >= 0) {
        mnuResolveUnselectedPageHandles(window);
    }
    window->selected = selected;
    mnuRefreshPageHandles(window);
}

void mnuClearPageSelectionHandles(MenuPageWindow *window) {
    s32 *resource = window->handlesC;
    u32 i;

    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(*resource++);
    }
    if (window->selected >= 0) {
        mnuResolveUnselectedPageHandles(window);
    }
    mnuClearPageSelection((MenuWindowSet *)window);
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD480);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AFB8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AFD8);

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

