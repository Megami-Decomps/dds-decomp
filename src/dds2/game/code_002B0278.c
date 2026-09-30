#include "common.h"

extern s32 D_00435E5C;

extern s32 func_002C6CE8(void);

extern s32 func_002B86E8(u32);

extern void func_002AAE80();

extern void func_002BC410();

extern void mnuClearEntries();

extern s32 func_0019FEF8();

extern s32 D_00435E54;

extern u32 effMiscRand();

extern u32 evtStageTestCountFlags();

extern s32 func_002C7540();

extern void evtStageTestQueueMotion();

extern s32 D_00435E48;

extern void mnuApplyPackedGroupValues();

extern void func_002C0D18();

extern void func_002C10F0();

extern void mnuDrawSlotIcons();

extern void func_0019D1D0();

extern s32 func_0019CE10();

extern void func_0019D100();

extern void func_0019D110();

extern void frFontSetChildColors();

extern void func_0019D1E0();

extern char D_003E7588[];

extern char D_00380788[];

extern void evtStageTestUpdate();

extern void func_002B96D8();

extern void mnuClearActionFlags();

extern char D_003E7790[];

extern char D_003E7720[];

extern char D_003E7774[];

extern char D_003E773C[];

extern void func_002C48C8();

extern void func_002BD480();

extern char D_003E7758[];

extern s32 func_002B5DE8();

extern u32 func_002C44E8();

extern s32 func_002C50C0();

extern void func_002C42B0();

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

extern u8 *func_002B8A50();

extern u8 *func_002B8BA8();

extern void mnuHideIconGroup();

extern s32 func_0026C6A0();

extern void func_002C42C0();

extern void func_002BAF50();

extern void mnuInitPartyPanelSlots();

extern s32 func_002B06A8();

extern void func_002AFB38();

extern char D_003E7530[];

extern void func_002AAC70();

extern s32 D_00435E6C;

extern void func_002AACB8();

extern s32 func_00305080();

extern void func_00305068();

extern void func_002BD1D0();

extern void func_002AAC98();

extern char D_003E69B0[];

extern void mnuCreateStaffImageSprite();

extern void func_002AA7A0();

extern void func_002BB0E8();

extern void mnuIdleVoiceTimer();

extern void func_002B2408();

extern u32 mnuCreateIconBundle(u32);

extern u32 func_002B9FF8();

extern s32 D_00435DD0;

extern s64 mnuDrawIconPanel(s32, s32, s32, s32, u32 *, s32, s32);

extern void mnuHideWindowHandlesKindFourFive(u32);

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct MenuSlot {
    s32 resources[3];
    u16 unused;
    u16 flags;
} MenuSlot;

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
    u8 pad00[0x60];
    s32 displayHandle;     /* 0x60 */
    s32 resourceHandle;    /* 0x64 */
    void *displayResource; /* 0x68 */
    u8 pad6C[0x5C];
    s32 labelHandle;       /* 0xC8 */
    u8 padCC[0x38];
    s32 imageHandle;       /* 0x104 */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    u32 actionFlags;       /* 0x284 */
    u8 pad288[0xA408];
    s32 selectedPartyList; /* 0xA690 */
    s32 secondPartyList;   /* 0xA694 */
    u8 padA698[0x27C];
    s32 selectionList;     /* 0xA914 */
    u8 padA918[0x10];
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

extern MenuSlot *D_00435E24;

extern void func_002C21F8(s32);

extern s32 func_00101958();

extern void func_002B9EA0(s32, s32, s32, s32, s32);

extern void effResolveAndReleaseResource(s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

/* Menu state handler installer: the call is inlined at each use, so callers
 * return its result through a real call rather than a sibcall. */
typedef struct StaffFadeState {
    u8 pad0[0x1DB0];
    s32 fadeA;
    s32 fadeB;
} StaffFadeState;

typedef struct MenuListNode MenuListNode;

struct MenuListNode {
    s32 index;
    s32 value;
    u8 pad8[0x40];
    u32 flags48;         /* 0x48 */
    u8 pad4C[4];
    s32 fadeCounter;     /* 0x50 */
    u8 pad54[4];
    struct MenuListNode *next;
    struct MenuListNode *prev;
    u32 sortKeyPrimary;   /* 0x60 */
    u32 sortKeySecondary; /* 0x64 */
    u32 sortKeyTertiary;  /* 0x68 */
    u8 pad6C[8];
};

typedef struct MenuList {
    u32 unk0;
    u32 flags;
    u8 pad8[4];
    s32 visibleCount;
    MenuListNode *first;
    MenuListNode *last;
    MenuListNode *head;
    MenuListNode *cursor;
    s32 count;
    s32 windowOffset;
    s32 rowHeight;
} MenuList;

typedef struct MenuWindowContainer {
    s32 id;                /* 0x00 */
    u32 flags;             /* 0x04 */
    u8 pad08[8];
    s32 width;             /* 0x10 */
    s32 height;            /* 0x14 */
    MenuList *list;        /* 0x18 */
    u8 pad1C[0x10];
    u32 layout2C;         /* 0x2C */
    u32 layout30;
    u32 layout34;
    u32 layout38;
    u32 layout3C;
    u32 layout40;
    u32 layout44;
    u32 layout48;
    u32 layout4C;
    u8 pad50[8];
    u8 panel[0xC];         /* 0x58 */
    s32 visible;           /* 0x64 */
    u8 pad68[0x24];
    s32 transition;        /* 0x8C */
    void *resource;        /* 0x90 */
    u32 state;             /* 0x94 */
} MenuWindowContainer;

extern MenuListNode *func_00328E18(s32);

extern void ptyRecomputeMaxHpMp();

extern void scrClearSecondaryScriptFlag();

extern void func_0019D550(s32, s32, s32);

extern void func_0019C5B0(s32);

extern void func_0035B7F8(MenuListNode **, s32, s32, s32 (*)(MenuListNode **, MenuListNode **));

extern s32 func_003292A8(s32);

extern s32 *sdfResourceRetainAddress(s32);

static inline s64 menuSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 8, (s32 *)(context + 0x54), mode, callback);
}

extern void func_0026C900(void);

extern void mnuDestroyWindowContainer(u32);

extern void *memset(void *, s32, u32);

u32 *func_002B9918(u32 first, u32 second, u32 third,
                    u32 fourth, u32 fifth, u32 sixth);

typedef struct MenuListDefaults {
    u32 fields[3];
} MenuListDefaults;

extern MenuListDefaults D_0042AF00;

typedef struct MenuLink {
    u8 unk0[0x48];
    u32 flags;
    u8 unk4C[0xC];
    struct MenuLink *next;
    u8 unk5C[4];
    u16 id;
} MenuLink;

extern void func_002B0278(s32);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0278);

s64 func_002B0578(s32 callback) {
    s32 context = func_00101958();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B05C8(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
}

/* Initialize the menu display using a value from the resource chain. */
u32 mnuEnterSelectedResourceLabel(void) {
    s32 resourceOwner;
    s32 context;

    context = func_00101958();
    resourceOwner = ((MenuContext *)context)->party;
    func_002C1B68(context + 0xaa50, 1);
    func_0026C918(0, D_00435E5C +
                                    *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(resourceOwner + 0x18) + 0x18) + 0x1c) + 100) * 0x19);
    func_0026C5B8(8);
    func_0026C648(0);
    func_0026C618(0xf);
    return 1;
}

u32 func_002B06A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B06A8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0898);

s64 func_002B09C8(s32 callback) {
    s32 context = func_00101958();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B0A18(s32 callback) {
    s32 context = func_00101958();
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
        ((u8 *)D_00435DD0)[recordOffset++] = 0;
    } while (--bytesRemaining >= 0);
    ((u8 *)(selection + D_00435DD0))[0x1e7b0] = 0;
    ptyRecomputeMaxHpMp(context);
}

u32 mnuEnterSlotLabel(void) {
    s32 context = func_00101958();
    s32 slot = D_00435DD0 + **(s32 **)(((MenuContext *)context)->selectionList + 0x1c) * 0x1c4 + 0xa60;
    s32 selectedEntry;
    func_002C1B68(context + 0xaa50, 1);
    selectedEntry = func_002C55C0(slot);
    func_0026C918(0, D_00435E5C + selectedEntry * 0x19);
    func_0026C5B8(0xd);
    func_0026C648(0);
    func_0026C618(0xf);
    return 1;
}

u32 func_002B0B88(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0B90);

s64 func_002B0CB0(s32 callback) {
    s32 context = func_00101958();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B0D00(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
}

u32 func_002B0D48(void) {
    return 1;
}

void func_002B0D50(u32 context) {
    func_002A9460(4, context);
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

void func_002B1150(s32 context) {
    mnuDestroyWindowContainer(*(u32 *)(((MenuContext *)context)->party + 8));
}

typedef struct PartyEntryCopy {
    u16 flags;
    u16 pad02;
    u32 word[0x70];
} PartyEntryCopy; /* 0x1C4 bytes, versus 0x1A4 in DDS1 */

typedef struct PartyMenuData {
    u8 pad00[0x8E0];
    PartyEntryCopy current[5];
    s32 activeCount; /* 0x11B4 */
    PartyEntryCopy backup[5];
    s32 selection;   /* 0x1A8C */
} PartyMenuData;

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
        *destination = *(PartyEntryCopy *)(copyOffset + D_00435DD0 + 0xA60);
        if (((PartyEntryCopy *)(D_00435DD0 + flagOffset))->flags & 1) {
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B15F8);

s32 mnuCountActiveSlots(void) {
    s32 count = 0;
    s32 i;
    u16 *slotFlags = (u16 *)(D_00435DD0 + 0xa60);
    for (i = 4; i >= 0; i--) {
        count += *slotFlags & 1;
        slotFlags += 0xe2;
    }
    if (count > 3) {
        count = 3;
    }
    return count;
}

void func_002B17C0(s32 context) {
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
    for (node = *(s32 *)(*(s32 *)(*(s32 *)((s32)menu + 8) + 0x18) + 0x10); node != 0; node = *(s32 *)(node + 0x58)) {
        ((MenuListNode *)node)->flags48 &= ~1;
    }
}

void func_002B18A0(s32 context) {
    func_002BB8D8(context + 0x284);
    mnuInitPartyPanelSlots(context + 0xA928);
    func_002BCA98(context + 0x284);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B18E8);

u32 func_002B1B90(void) {
    s32 context = func_00101958();
    u32 *selection = *(u32 **)(context + 0xaa48);
    func_002B18A0(context);
    func_002B1150(context);
    func_002B0D70(context);
    func_003297C8(*selection);
    return 1;
}

void func_002B1BF0(s32 menu) {
    s32 party = ((MenuContext *)menu)->party;

    func_002B15F8();
    func_002C42C0(menu + 0x54, D_003E7588);
    func_002BB498(((MenuContext *)menu)->panelHandle, ((MenuContext *)menu)->displayHandle, 0, 1);
    func_002BAF50(((MenuContext *)menu)->imageHandle, menu + 0xB10C);
    *(s32 *)(party + 0x1DD8) = 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1C68);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1EA8);

/* Two-stage fade: B rises first when opening, A falls first when closing. */
void mnuUpdateStaffFade(s32 opening, StaffFadeState *state) {
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

s64 func_002B2698(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    s32 list;
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(0x17);
    list = *(s32 *)(*(s32 *)(menu + 8) + 0x18);
    if (mnuIsFinalItemIndex(**(s32 **)(list + 0x1c), list)) {
        **(u32 **)(*(s32 *)(menu + 8) + 0x18) |= 0x10;
    } else {
        **(u32 **)(*(s32 *)(menu + 8) + 0x18) &= ~0x10;
    }
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    if (*(s32 *)(menu + 0x1dd8) == 0) {
        func_002B2408(context);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B2790(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

u8 mnuIsStateNotOne(void) {
    s64 state;

    state = func_002C6CE8();
    return state != 1;
}

void func_002B27F0(u32 context) {
    func_002A9460(3, context);
}

void func_002B2810(s32 context) {
}

void func_002B2818(s32 menu) {
    u32 *handles = (u32 *)(menu + 8);
    s32 remaining = 1;
    do {
        effResolveAndReleaseResource(*handles++);
    } while (--remaining >= 0);
}

void func_002B2860(s32 menu) {
    u32 *handles = (u32 *)(menu + 8);
    s32 remaining = 1;
    do {
        func_00305068(*handles++);
    } while (--remaining >= 0);
}

u32 mnuCreateSelectState(u32 unused, s32 flag) {
    s32 context = func_00101958();
    u32 handle = func_003292A8(0x30);
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
    func_002BB498(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayResource, 0, 0);
    evtStageTestInit(0);
    return 1;
}

s32 func_002B2970(void) {
    s32 context = func_00101958();
    s32 party = ((MenuContext *)context)->party;

    mnuResetWorkFloats();
    func_002B2810(context);
    func_003297C8(*(s32 *)party);
    return 1;
}

void func_002B29C8(u32 context) {
    mnuCreateSelectState(context, 1);
}

void func_002B29E0(void) {
    func_002B2970();
}

void func_002B29F8(u32 context) {
    mnuCreateSelectState(context, 0);
}

void func_002B2A10(void) {
    func_002B2970();
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2A28);

s64 func_002B2B48(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    if (func_00305080(((MenuContext *)context)->resourceHandle)) {
        func_002AACB8(0, callback);
    } else {
        func_002AACB8(1, callback);
    }
    if (menu[5] == 0) {
        mnuCreateStaffImageSprite(0x16);
    } else {
        mnuCreateStaffImageSprite(0x15);
    }
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    if (menu[9] != 0) {
        func_002AAC98(0, **(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c), D_003E69B0, context, 1, 0x53);
    }
    return menuSetHandler(context, 1, callback);
}

s64 func_002B2C50(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2C88);

INCLUDE_ASM(const s32, "game/code_002B0278", mnuCreatePanels);

s32 mnuDestroyPanels(void) {
    s32 context = func_00101958();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 window;
    func_002BAF50(((MenuContext *)context)->imageHandle, context + 0xb10c);
    window = context + 0x284;
    func_002B2C88(window, 0, menu[5], menu[4]);
    evtStageTestStop();
    mnuClearEntries(window);
    func_002BC410(window);
    if (((MenuContext *)context)->panelGroup != 0) {
        mnuDestroyPanelGroup(((MenuContext *)context)->panelGroup);
        ((MenuContext *)context)->panelGroup = 0;
    }
    if (((MenuContext *)context)->panelRequest != 0) {
        func_002C1050(((MenuContext *)context)->panelRequest);
        ((MenuContext *)context)->panelRequest = 0;
    }
    if (((MenuContext *)context)->panelEffects != 0) {
        func_002C16D8(((MenuContext *)context)->panelEffects);
        ((MenuContext *)context)->panelEffects = 0;
    }
    if (((MenuContext *)context)->resourceList != 0) {
        func_002C3390(((MenuContext *)context)->resourceList);
        ((MenuContext *)context)->resourceList = 0;
    }
    mnuReleaseResourceList(menu[8]);
    return 1;
}

void func_002B3120(s32 context) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(((MenuContext *)context)->selectionList + 0x1c) * 0x2138 + context + 0x3d8) + 0x60) =
              0x100;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3150);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3260);

void mnuDrawSlotIcons(s32 x, s32 context) {
    s32 slot = D_00435DD0 + **(s32 **)(((MenuContext *)context)->selectedPartyList + 0x1c) * 0x1c4 + 0xa60;
    s32 i;
    s32 y = 0xb40;
    s32 handle;
    for (i = 0; i < 2; i++, y += 0xb8) {
        handle = func_0019FEF8(0x3c0, y, 0, 0xa09dc359, D_00435E54 + *(u16 *)(slot + 4) * 45, i);
        if (handle != 0) {
            func_0019D550(handle, 1, 0x53);
            func_0019C5B0(handle);
        }
    }
}

void func_002B34E0(s32 context, u32 *handles) {
    s32 alpha;

    alpha = 0x100 - *(s32 *)(*(s32 *)(**(s32 **)(((MenuContext *)context)->selectedPartyList + 0x1c) * 0x2138 + context
                                                                      + 0x154) + 0x60);
    func_00306CD0(0xa0, 0xa30, 0, alpha, 1, handles[1], 0x55, 0x53);
    func_00306CD0(0x30, 0xaf8, 0, alpha, 1, *handles, 0x1a, 0x53);
}

void mnuDrawTextSprite(s32 x, s32 y, s32 width, u32 color, s32 model, s32 flags) {
    s32 top = y - 0x10;
    s32 handle;
    func_0019D1D0(1);
    handle = func_0019CE10(model, 0, 0, 0, 0);
    func_0019D100(handle, x, top);
    func_0019D110(handle, width << 4);
    frFontSetChildColors(handle, color);
    func_0019D1E0(1);
    func_0019D550(handle, 1, flags);
    func_0019C5B0(handle);
}

void func_002B3648(u8 *entry, s32 id, s32 packedGroup, s32 group, s32 unused, s32 spriteFlags) {
    mnuApplyPackedGroupValues(packedGroup, *(u16 *)(entry + 0x1b2));
    func_002C0D18(0xeb0, 0x518, 0, entry, packedGroup, 0, spriteFlags);
    func_002C10F0(0, 0, 0, entry, group, spriteFlags);
    mnuDrawTextSprite(0x2a0, 0xa50, 0, 0xa09dc380, D_00435E48 + *(u16 *)(entry + 4) * 0x11 + 0x110, spriteFlags);
    mnuDrawSlotIcons(0x14a, id);
}

void func_002B3720(u32 entry, u32 unused1, u32 group, u32 resource,
                                    u32 unused4, u32 spriteFlags) {
    func_002C16F0(0, 0, 0, entry, *(u8 *)((s32)entry + 0x55), group, spriteFlags);
    func_002C3E08(0xe80, 0x5b8, 0, resource, spriteFlags);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3788);

void mnuIdleVoiceTimer(s32 object) {
    u32 count;
    if (*(s32 *)(object + 0x2c) == -1) {
        if (func_002C6CE8() != 1) {
            if (func_002C7540() == 0) {
                *(s32 *)(object + 0x28) += 1;
            }
            if (*(s32 *)(object + 0x28) >= 0x12d) {
                count = evtStageTestCountFlags(0);
                evtStageTestQueueMotion(0, effMiscRand(0) % count);
                *(s32 *)(object + 0x28) = 0;
            }
        }
    }
}

s64 func_002B39F0(s32 callback) {
    s32 context = func_00101958();
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

u32 func_002B40B8(u32 callback) {
    s32 party;

    party = func_00101958();
    party = ((MenuContext *)party)->party;
    mnuDestroyWindowContainer(*(u32 *)(party + 0x24));
    *(u32 *)(party + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B40F8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4180);

void func_002B4270(u32 context) {
    func_002A9460(1, context);
}

void func_002B4290(s32 context) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4298);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B45D8);

typedef struct SkillInfo {
    u8 unk0[0x20];
    u32 count;
    u16 id[14];
} SkillInfo;

extern void func_00315388(u16, SkillInfo *);

u32 *mnuBuildOwnedSkillBits(void) {
    SkillInfo info;
    u32 *bits = (u32 *)func_00328D68(0x58);
    s32 i;
    u32 j;
    memset(bits, 0, 0x58);
    for (i = 0; i < 0xb0; i++) {
        func_00315388(i, &info);
        for (j = 0; j < info.count; j++) {
            u16 id = info.id[j];
            if (id != 0) {
                bits[id >> 5] |= 1 << id;
            }
        }
    }
    return bits;
}

void func_002B47F8(void) {
    func_00328E48();
}

s32 mnuIsSkillCodeInBitset(s32 skillCode, u32 *bits) {
    s32 wordIndex = (skillCode < 0) ? skillCode + 0x1f : skillCode;

    return (bits[wordIndex >> 5] & (1 << skillCode)) != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4848);

void func_002B4C48(s32 context) {
    u32 *menu = *(u32 **)(context + 0xaa48);
    if (menu[2] != 0) {
        u32 i = 0;
        u32 *resource = menu + 4;
        mnuDestroyPanelState(menu[8]);
        func_002B81C8(menu[3]);
        do {
            mnuDestroyWindowContainer(*resource++);
            i++;
        } while (i < 4);
        menu[2] = 0;
    }
}

u32 mnuCreateItemState(s32 callback) {
    s32 context = func_00101958();
    u32 handle = func_003292A8(0x3c);
    u32 *state = (u32 *)sdfResourceRetainAddress(handle);
    *(u32 **)(context + 0xaa48) = state;
    memset(state, 0, 0x3c);
    state[0] = handle;
    func_002B4270(context);
    switch (**(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c)) {
    case 0:
        func_002BB498(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->labelHandle, 0, 0);
        break;
    case 2:
        func_002BB498(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->labelHandle, 0x19, 0);
        break;
    case 3:
        func_002BB498(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->labelHandle, 0xa, 0);
        break;
    }
    mnuSeekListNode(0, (MenuList *)*(s32 *)(((MenuContext *)context)->listHandle + 0x18));
    return 1;
}

s32 func_002B4DE8(s32 selection) {
    s32 context = func_00101958();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    if (menu[9] != 0) {
        func_002B40B8(selection);
    }
    func_002B4290(context);
    func_003297C8(menu[0]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4E58);

s64 func_002B5028(s32 callback) {
    s32 context = func_00101958();
    func_002AAE80(callback);
    if (**(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c) == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xe);
    }
    func_002AAC98(0, **(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c), D_003E69B0, context, 1, 0x53);
    if (**(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c) == 0) {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B5128(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

void func_002B5160(void) {
    s32 context;

    context = func_00101958();
    *(u32 *)(((MenuContext *)context)->party + 0x34) = 0xffffffff;
}

u32 func_002B5190(s32 callback) {
    s32 context;

    context = func_00101958();
    return ~*(u32 *)(((MenuContext *)context)->party + 0x34) >> 0x1f;
}

void func_002B51C8(void) {
    u8 *state = *(u8 **)(func_00101958() + 0xaa48);
    u8 *node = *(u8 **)(*(u8 **)(*(u8 **)(state + 0x24) + 0x18) + 0x10);
    while (node != NULL) {
        if (*(u32 *)node == *(u32 *)(state + 0x34)) {
            ((MenuListNode *)node)->flags48 |= 2;
        } else {
            ((MenuListNode *)node)->flags48 &= ~2;
        }
        node = (u8 *)((MenuListNode *)node)->next;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5240);

u32 func_002B5358(void) {
    s32 context = func_00101958();
    u32 *state = (u32 *)((MenuContext *)context)->party;
    s32 image = ((MenuContext *)context)->imageHandle;
    if (**(s32 **)(*(s32 *)(image + 0x18) + 0x1c) == 0) {
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

void func_002B5450(s32 callback) {
    s32 context = func_00101958();
    s32 menu = ((MenuContext *)context)->party;
    u32 input = func_002C44E8(0x33);
    MenuWindowContainer *window = *(MenuWindowContainer **)(menu + 0x24);
    MenuList *list = window->list;

    list->unk0 &= ~8;
    if (input & 1) {
        MenuListNode *entry = list->cursor;
        s32 target = entry->sortKeyPrimary;

        if (!(entry->flags48 & 1) && target != 0) {
            func_002C42B0(context + 0x54, D_003E7774);
        } else {
            input = 0x8000;
        }
    }
    if (input & 2) {
        func_002C42C0(context + 0x54, D_003E773C);
    }
    if (window != 0) {
        if (!(input & 0x300000)) {
            func_002B9808((s32)window);
        }
        if (input & 0x10) {
            func_002B97F0((s32)window);
        }
        if (input & 0x20) {
            func_002B97D8((s32)window);
        }
        func_002B96D8((s32)window);
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B57A8);

s64 func_002B5980(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    s64 state = func_002C4038(context + 8, (s32 *)(context + 0x54), 0, callback);
    if (state != 0) {
        return state;
    }
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002B5450(callback);
    } else if (menu[12] == 0) {
        func_002B5580(callback);
    } else {
        func_002B57A8(callback);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5A30);

s64 func_002B5BE8(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 label;
    if (**(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c) == 0) {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
        func_002B5A30(context);
    }
    func_002AAE80(callback);
    if (**(s32 **)(*(s32 *)(((MenuContext *)context)->imageHandle + 0x18) + 0x1c) == 0) {
        mnuCreateStaffImageSprite(2);
    } else if (menu[12] != 0) {
        if (func_002B5190(callback) == 0) {
            mnuCreateStaffImageSprite(0x12);
        } else {
            mnuCreateStaffImageSprite(0x13);
        }
    } else if (**(s32 **)(*(s32 *)(*(s32 *)((s32)menu + 0x10 + (**(s32 **)(menu[3] + 0x1c) << 2)) + 0x18) + 0x1c) == 0) {
        mnuCreateStaffImageSprite(0x11);
    } else {
        mnuCreateStaffImageSprite(0x10);
    }
    label = *(s32 *)(*(s32 *)(*(s32 *)(menu[9] + 0x18) + 0x1c) + 0x60);
    if (label != 0xffff && label != 0) {
        func_002AAC70(0, label, D_00435E6C, context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B5DB0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5DE8);

void mnuFlagMatchingEntries(s32 context) {
    s32 slot = D_00435DD0 + **(s32 **)(((MenuContext *)context)->selectionList + 0x1c) * 0x1c4 + 0xa60;
    MenuLink *link = *(MenuLink **)(*(s32 *)(*(s32 *)(((MenuContext *)context)->party + 0x24) + 0x18) + 0x10);
    if (link != NULL) {
        do {
            if (func_002C4FB8(link->id, slot)) {
                link->flags |= 1;
            }
            link = link->next;
        } while (link != NULL);
    }
}

s64 func_002B5FA8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(3);
    s64 state;
    s32 label;
    u16 code;
    u8 *window;
    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    label = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24) + 0x18) + 0x1c) + 0x60);
    code = label;
    if (func_002C50C0(code) == 2) {
        ((MenuContext *)context)->actionFlags |= 0x10;
    }
    if (func_002C50C0(code) == 3) {
        ((MenuContext *)context)->actionFlags |= 0x20;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(8, window);
    if (buttons & 1) {
        buttons = func_002B5DE8(label, context) == 0 ? 0x8000 : 0;
        mnuFlagMatchingEntries(context);
    }
    if (buttons & 2) {
        func_002C42B0(popup, D_003E7758);
        mnuClearActionFlags(1, window);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s64 func_002B60E8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(3);
    func_002AAC70(0, *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24) + 0x18) + 0x1c) + 0x60), D_00435E6C, context, 1, 1, 0x53);
    **(u32 **)(*(s32 *)(menu + 0x24) + 0x18) &= ~8;
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B61C0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

s32 func_002B61F8(s32 callback) {
    s32 context = func_00101958();
    s32 party = ((MenuContext *)context)->party;
    MenuWindowContainer *window;

    func_002BD1D0(context + 0x284, **(s32 **)(((MenuContext *)context)->selectionList + 0x1C));
    ((MenuContext *)context)->actionFlags |= 0x200;
    func_002B3E80(0, callback);
    func_002B4848(context);
    window = *(MenuWindowContainer **)(party + 0x24);
    window->list->unk0 |= 8;
    func_002BAF50(window, context + 0xB10C);
    return 1;
}

u32 func_002B62A8(u32 callback) {
    s32 context = func_00101958();
    func_002BAF50(((MenuContext *)context)->imageHandle, context + 0xb10c);
    func_002B40B8(callback);
    func_002B4C48(context);
    func_002BD2E0(context + 0x284);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6308);

void func_002B63F0(s32 menu, s32 target) {
    s32 *entry = (s32 *)menu + **(s32 **)(*(s32 *)(menu + 4) + 0x1C);
    s32 list = entry[2];
    s32 delta = target - ((MenuList *)((MenuWindowContainer *)list)->list)->windowOffset;
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
                mnuReverseListSelection(list, 1);
            }
            if (dir > 0) {
                mnuAdvanceListSelection(list, 1);
            }
            n--;
        } while (n != 0);
    }
    mnuResetListNodeFadeCounters(((MenuWindowContainer *)list)->list);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6498);

s64 func_002B66D8(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = (s32 *)((MenuContext *)context)->party;
    s32 index = **(s32 **)(menu[3] + 0x1c);
    s32 label = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)((s32)menu + 0x10 + (index << 2)) + 0x18) + 0x1c) + 0x60);
    func_002B5A30(context);
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(0xf);
    if (label != 0xffff && label != 0) {
        func_002AAC70(0, label, D_00435E6C, context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    **(u32 **)(menu[9] + 0x18) |= 8;
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(3, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B6800(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

void mnuDrawSelectionLabel(u16 id) {
    s32 label = func_0019FE00(0x11B0, 0xA88, 0, 0, id, 1);

    frFontSetChildColors(label, 0xA09DC35A);
    func_0019D550(label, 1, 0x53);
    func_0019C5B0(label);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6898);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6B00);

typedef struct PtyFrontlineSlot {
    u16 flags;          /* 0x00: bit 0 present, bit 1 frontline */
} PtyFrontlineSlot;

/* Collect up to max pointers to occupied, frontline party slots. */
void func_002B6C70(s32 **out, s32 max) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < max; i++) {
        out[i] = 0;
    }
    i = 0;
    while (count < max) {
        PtyFrontlineSlot *entry = (PtyFrontlineSlot *)(D_00435DD0 + i * 0x1C4 + 0xA60);

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
    entry = (MenuSlot *)((id << 4) + (s32)D_00435E24);
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

u32 func_002B6FA8(u32 callback) {
    s32 party;

    party = func_00101958();
    party = ((MenuContext *)party)->party;
    mnuDestroyWindowContainer(*(u32 *)(party + 0x24));
    *(u32 *)(party + 0x24) = 0;
    return 1;
}

u32 func_002B6FE8(u32 callback) {
    s32 context;
    u32 *state;
    mnuCreateItemState(callback);
    context = func_00101958(callback);
    state = *(u32 **)(context + 0xaa48);
    func_002B6D78(callback);
    mnuFlagActiveWindows(context + 0x284);
    state[11] = 0;
    func_002BAF50(state[9], context + 0xb10c);
    return 1;
}

u32 func_002B7060(u32 callback) {
    s32 context = func_00101958();
    func_002BAF50(((MenuContext *)context)->imageHandle, context + 0xb10c);
    func_002B6FA8(callback);
    func_002BD3A8(context + 0x284);
    func_002B4DE8(callback);
    return 1;
}

s64 func_002B70C0(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(0xc32);
    s64 state;
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
        func_002B97F0(list[8 + menu[11]]);
    }
    if (buttons & 0x20) {
        func_002B97D8(list[8 + menu[11]]);
    }
    func_002C48C8(list[8 + menu[11]], &buttons);
    func_002B96D8(list[8 + menu[11]]);
    mnuPlayInputSound(0, buttons, (s32)((MenuWindowContainer *)list[8 + menu[11]])->list);
    if (buttons & 2) {
        func_002C42C0(popup, D_003E7720);
        func_002BB498(((MenuContext *)context)->panelHandle, ((MenuContext *)context)->displayHandle, 0, 1);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7228);

void func_002B7588(s32 context) {
    s32 index;

    for (index = **(s32 **)(context + 0x28c); index < 3; index = index + 1) {
    }
}

s64 func_002B75C8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = (u8 *)((MenuContext *)context)->party;
    u32 label;
    func_002AAE80(callback);
    func_002B7588(context);
    mnuCreateStaffImageSprite(0x14);
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    label = *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24 + *(s32 *)(menu + 0x2c) * 4) + 0x18) + 0x1c) + 0x60);
    func_002B7228(context);
    if (label != 0 && label != 0xffff) {
        label = (u16)label;
        mnuDrawSelectionLabel(label);
        func_002B6898(label, ((MenuContext *)context)->resourceHandle, ((MenuContext *)context)->labelHandle);
    }
    func_002AA7A0(2, ((MenuContext *)context)->displayHandle);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B76B0(s32 callback) {
    s32 context = func_00101958();
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

void func_002B7740(s32 sourceBase, s32 destinationBase) {
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

void func_002B7790(u32 first, u32 second, u32 *menu) {
    menu[2] = first;
    menu[15] = second;
}

typedef struct MenuEffectResources {
    MapPacket packet;      /* 0x00–0x3B */
    u32 animationHandle;   /* 0x3C */
} MenuEffectResources;

void func_002B77A0(s32 resources) {
    func_003059E0(((MenuEffectResources *)resources)->packet.unk_08,
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
    mnuInitializeMapPacket(0, D_003E7828, 0xb, (MapPacket *)effect);
    func_002B7740((s32)D_003E7858, (s32)effect);
    ((MenuEffectResources *)effect)->packet.unk_08 = effLoadIndexedResource("/camp/spr/n_min/", D_003E7818[0], 0);
    ((MenuEffectResources *)effect)->animationHandle = effLoadMappedResource("/camp/mot/", D_003E7820[0]);
    func_002B77A0((s32)effect);
}

void mnuRequestEffectResources(u8 *effect) {
    mnuInitializeMapPacket(0, D_003E7828, 0xb, (MapPacket *)effect);
    func_002B7740((s32)D_003E7858, (s32)effect);
    effRequestResourceByMode("/camp/spr/n_min/", D_003E7818[0], 0, (u32 *)(effect + 8));
    effRequestMappedResource("/camp/mot/", D_003E7820[0], (u32 *)(effect + 0x3c));
}

u32 func_002B78C8(u32 *menu) {
    if (menu[2] == 0) {
        return 0;
    }
    if (menu[15] == 0) {
        return 0;
    }
    func_002B77A0(menu);
    return 1;
}

void mnuDestroyEffectResources(u8 *ctx) {
    u32 i;
    for (i = 0; i < 1; i++) {
        func_003054E8(*(u32 *)(ctx + 8 + i * 4));
    }
    effDestroyPackedBatch(((MenuEffectResources *)ctx)->animationHandle);
}

typedef struct MenuSparkSet {
    u8 unk0[0x60];
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

void func_002B7A80(s32 effects, s32 index) {
    *(u32 *)(index * 4 + effects + 0x60) = 0;
    *(s32 *)(effects + 0x160) = *(s32 *)(effects + 0x160) - 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7AA0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7C10);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7E60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7F80);

void func_002B8140(u32 *flags) {
    *flags = *flags & 0xfffffffb;
}

u32 *mnuCreateListState(u32 owner, u32 callback, s32 count) {
    u32 *node = (u32 *)func_00328E18(0x40);
    node[2] = owner;
    node[3] = callback;
    node[10] = count * 8;
    node[15] = 0x100;
    node[6] = 0;
    node[4] = 0;
    node[9] = 0;
    node[7] = 0;
    return node;
}

u32 func_002B81C8(u32 list) {
    s64 result;

    do {
        result = func_002B86E8(list);
    } while (result != 0);
    func_00328E48(list);
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
    MenuListNode *node = func_00328E18(0x74);
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
    func_003071D0(sprite, effect, x, y, x, y);
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8A50);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8BA8);

void func_002B8CF0(u32 list) {
    func_002B8A50(list, 0, 0);
}

void func_002B8D10(u32 list) {
    func_002B8BA8(list, 0, 0);
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

s32 func_002B8FC8(MenuList *list) {
    return list->rowHeight * list->visibleCount;
}

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

void mnuDecreaseListNodeFadeCounters(u8 *menu) {
    u8 *node = (u8 *)((MenuList *)menu)->first;
    if (node != NULL) {
        do {
            s32 timer = ((MenuListNode *)node)->fadeCounter;
            s32 reduced = timer - 0x10;
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

void mnuDrawFourEntries(s32 x, s32 y, s32 layer, s32 selection, s32 entries, s32 opacity) {
    s32 base8 = entries + 8;
    s32 baseC = entries + 0xC;
    u32 i = 0;
    do {
        s32 eq = entries == *(s32 *)(selection + 0x1C);
        s32 off = (eq * 4 + i) * 8;
        s32 value = *(s32 *)(base8 + off);
        if (value != 0) {
            func_00306F80(x, y, layer, 0, value, *(s32 *)(baseC + off), opacity);
        }
        i++;
    } while (i < 4);
}

extern u32 func_00309138(u32 color, u32 previous, s32 blend);

u32 func_002B9138(u32 backup, u8 *node) {
    u32 flags = ((MenuListNode *)node)->flags48;
    u32 color = 0x89bdc940;
    if (!(flags & 1)) {
        color = (flags & 4) ? 0xbbefab80 : 0x89bdc980;
    }
    return func_00309138(color, backup, ((MenuListNode *)node)->fadeCounter);
}

typedef struct MenuSlotEntry {
    u8 pad0[0x14];
    s32 word[4];
    u8 pad24[0x7C];
} MenuSlotEntry;

typedef struct MenuSlotSet {
    u8 pad0[0x18];
    MenuSlotEntry *entries;
} MenuSlotSet;

void mnuDispatchEntryWords(MenuSlotSet *menu, s32 index, u8 *node) {
    s32 j;

    for (j = 0; j < 4; j++) {
        s32 word = menu->entries[index].word[j];

        menu->entries[index].word[j] = func_002B9138(word, node);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9218);

void mnuCallInitWide(s32 x, s32 y, s32 width, s32 context, s32 callback) {
    func_002B9218(x, y, width, 0, 0, 0x100, 0, context, callback);
}

s32 mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 left, s32 right) {
    s32 item = func_00328E18(0x98);
    s32 child;
    ((MenuWindowContainer *)item)->width = width;
    ((MenuWindowContainer *)item)->height = height;
    ((MenuWindowContainer *)item)->id = id;
    child = mnuCreateListState(id, left, right);
    ((MenuWindowContainer *)item)->state = 0;
    ((MenuWindowContainer *)item)->list = (MenuList *)child;
    return item;
}

void mnuDestroyWindowContainer(u32 menu) {
    s32 resource;

    func_002B81C8((u32)((MenuWindowContainer *)menu)->list);
    resource = (s32)((MenuWindowContainer *)menu)->resource;
    if (resource != 0) {
        func_002B99D8(resource);
    }
    func_00328E48(menu);
}

void func_002B9560(s32 menu, u32 layout) {
    ((MenuWindowContainer *)menu)->layout3C = layout;
}

void mnuSetWindowContainerState(s32 menu, u32 state) {
    ((MenuWindowContainer *)menu)->state = state;
}

void mnuSetWindowContainerLayout(s32 menu, u32 layout2C, u32 layout30, u32 layout34,
                                    u32 layout48, u32 layout38, u32 layout3C, u32 layout40,
                                    u32 layout4C) {
    ((MenuWindowContainer *)menu)->layout2C = layout2C;
    ((MenuWindowContainer *)menu)->layout4C = layout4C;
    ((MenuWindowContainer *)menu)->layout30 = layout30;
    ((MenuWindowContainer *)menu)->layout34 = layout34;
    ((MenuWindowContainer *)menu)->layout38 = layout38;
    ((MenuWindowContainer *)menu)->layout48 = layout48;
    ((MenuWindowContainer *)menu)->layout3C = layout3C;
    ((MenuWindowContainer *)menu)->layout40 = layout40;
    ((MenuWindowContainer *)menu)->layout44 = 0;
}

void func_002B95A0(s32 menu, u32 first, u32 second) {
    mnuSetWindowContainerLayout(menu, first, second, 0, 0, 0, 0, 0, 0);
}

void func_002B95D0(u32 first, u32 *menu, u32 second, u32 third, u32 fourth) {
    menu[7] = first;
    menu[8] = second;
    menu[9] = third;
    menu[10] = fourth;
}

typedef struct MenuPanelBounds {
    u8 pad00[4];
    u32 flags;     /* 0x04 */
    u8 pad08[0x74];
    u32 left;      /* 0x7C */
    u32 top;       /* 0x80 */
    u32 right;     /* 0x84 */
    u32 bottom;    /* 0x88 */
} MenuPanelBounds;

void func_002B95E8(u8 *panel, const void *layout, u32 left, u32 top,
                   u32 right, u32 bottom) {
    memcpy(panel + 0x58, layout, 0x38);
    ((MenuPanelBounds *)panel)->left = left;
    ((MenuPanelBounds *)panel)->top = top;
    ((MenuPanelBounds *)panel)->right = right;
    ((MenuPanelBounds *)panel)->bottom = bottom;
    ((MenuPanelBounds *)panel)->flags |= 4;
}

void mnuCreateListWithDefaults(u8 *menu, u32 first, u32 second, u32 third, u32 fourth) {
    MenuListDefaults defaults = D_0042AF00;
    ((MenuWindowContainer *)menu)->resource =
        func_002B9918(first, second, third, fourth, (u32)&defaults, 3);
}

void func_002B96D8(s32 list) {
    ((MenuList *)list)->flags = ((MenuList *)list)->flags & 0xfffffffb;
}

void func_002B96F0(s32 menu) {
    mnuListAppendNode((u32)((MenuWindowContainer *)menu)->list);
}

void func_002B9708(s32 menu) {
    func_002B83A0((u32)((MenuWindowContainer *)menu)->list);
}

void func_002B9720(s32 menu) {
    func_002B86E8((u32)((MenuWindowContainer *)menu)->list);
}

u8 *mnuAdvanceListSelection(u8 *menu, s32 step) {
    u8 *item = func_002B8A50((s32)((MenuWindowContainer *)menu)->list, step, 0);
    if (item != NULL) {
        item[0x54] = 0;
        mnuHideIconGroup(menu + 0x58);
    }
    return item;
}

u8 *mnuReverseListSelection(u8 *menu, s32 step) {
    u8 *item = func_002B8BA8((s32)((MenuWindowContainer *)menu)->list, step, 0);
    if (item != NULL) {
        item[0x54] = 0;
        mnuHideIconGroup(menu + 0x58);
    }
    return item;
}

void func_002B97D8(u32 menu) {
    mnuAdvanceListSelection(menu, 0);
}

void func_002B97F0(u32 menu) {
    mnuReverseListSelection(menu, 0);
}

void func_002B9808(s32 menu) {
    mnuClearListFlagsOneAndTwo((u32)((MenuWindowContainer *)menu)->list);
}

void func_002B9820(s32 menu) {
    mnuTestListFlagTwo((u32)((MenuWindowContainer *)menu)->list);
}

typedef struct MenuIconSprites {
    u32 handle;
    u32 value;
    u32 unk8;
    void *sprite[3];
} MenuIconSprites;

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value, s32 res, s32 *idx, s32 unused) {
    obj->value = value;
    obj->sprite[0] = (void *)effCreateResourceSlotSet(res, idx[0], 1);
    obj->sprite[1] = (void *)effCreateResourceSlotSet(res, idx[1], 1);
    obj->sprite[2] = (void *)effCreateResourceSlotSet(res, idx[2], 1);
    func_003071D0(obj->sprite[0], 0, w, h, w, h);
    func_003071D0(obj->sprite[1], 0, w, h, w, h);
    func_003071D0(obj->sprite[2], 0, w, h, w, h);
}

u32 *func_002B9918(u32 first, u32 second, u32 third,
                    u32 fourth, u32 fifth, u32 sixth) {
    u32 handle = func_003292A8(0x18);
    u32 *resource = (u32 *)sdfResourceRetainAddress(handle);
    memset(resource, 0, 0x18);
    resource[0] = handle;
    mnuInitIconSprites(resource, first, second, third, fourth, fifth, sixth);
    return resource;
}

void func_002B99D8(u32 *menu) {
    u32 i = 0;
    do {
        func_003054E8(menu[i + 3]);
        i++;
    } while (i < 3);
    func_003297C8(menu[0]);
}

void func_002B9A38(void) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9A40);

void func_002B9BB0(s32 x, s32 y, u32 flags, s32 window, u32 option) {
    func_002B9A40(x - 0xf0, y - 8, flags, ((MenuWindowContainer *)window)->state,
                                (u32)((MenuWindowContainer *)window)->list, (u32)((MenuWindowContainer *)window)->resource, option);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9BE0);

void func_002B9CD8(u32 x, u32 y, u32 flags, u8 *entry, u32 option) {
    u8 *data = *(u8 **)(entry + 0x18);
    func_002B9BE0(x, y, flags, entry, *(u32 *)(data + 0xc), option);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9CF8);

void func_002B9DD8(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 param) {
    MenuList *list;
    s32 flag;
    s32 texture = window->state;

    if (window->visible != 0) {
        list = window->list;
        flag = 0;
        if (list->unk0 & 8) {
            flag = 1;
        }
        if (list->unk0 & 0x10) {
            flag = 2;
        }
        y += list->windowOffset * list->rowHeight;
        mnuDrawIconPanel(x, y, depth, texture, (u32 *)window->panel, flag, param);
        mnuHideWindowHandlesKindFourFive((u32)window->panel);
        if (window->flags & 4) {
            window->transition += 0x10;
            if (window->transition >= 0x200) {
                window->transition = 0x200;
            }
        } else {
            window->transition += 0x20;
            if (window->transition >= 0x101) {
                window->transition = 0x100;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9EA0);

void func_002B9FB8(s32 window) {
    s32 remaining;

    remaining = ((MenuWindowContainer *)window)->list->count;
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

extern void func_00304CE0();

typedef struct SprInfo {
    u8 unk0[0x28];
    u32 flags;
} SprInfo;

typedef struct SprObj {
    u8 unk0[0x18];
    SprInfo *info;
} SprObj;

typedef struct SprGroup {
    u32 kind;
    u32 unk4;
    s32 count;
    SprObj *obj[1];
} SprGroup;

void mnuHideIconGroup(SprGroup *group) {
    s32 i;
    if (group->obj[0] != NULL && group->kind < 6) {
        for (i = 0; i < group->count; i++) {
            SprObj *obj = group->obj[i];
            u32 *flags = &obj->info->flags;
            *flags &= ~1;
            func_00304CE0(obj, 0);
        }
    }
}

typedef struct ResourceList {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ s32 count;
    /* 0xC */ u32 items[1];
} ResourceList;

void mnuReleaseResourceList(ResourceList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        if (list->items[i] != 0) {
            func_003054E8(list->items[i]);
        }
    }
    func_00328E48(list);
}

typedef struct MenuPos {
    s32 x;
    s32 y;
} MenuPos;

typedef struct MenuPosTable {
    MenuPos pos[6];
} MenuPosTable;

typedef struct MenuSpriteInner {
    u8 unk0[0xC];
    s32 shade;
    u8 unk10[0x6C];
    s32 shadeSource;
} MenuSpriteInner;

typedef struct MenuSprite {
    u8 unk0[0x18];
    MenuSpriteInner *inner;
} MenuSprite;

typedef struct MenuIconState {
    u32 kind;
    u32 unk4;
    s32 count;
    MenuSprite *sprite[6];
    u8 unk24[0x10];
    s32 fade;
} MenuIconState;

typedef struct MenuPosTable3 {
    MenuPos pos[3];
} MenuPosTable3;

extern MenuPosTable3 D_0042AF48;

s64 mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 arg) {
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

s64 mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 arg) {
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

s32 mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, void **state, s32 arg) {
    MenuOffsets offset = D_0042AF90;
    func_00306CD0(x + offset.x0, y + offset.y0, z, w, 1, state[3], 0, arg);
    func_00306CD0(x + offset.x1, y + offset.y1, z, w, 1, state[4], 0, arg);
}

extern s64 mnuDrawIconPanelFade();

extern s64 mnuDrawIconRow6();

extern s32 mnuDrawIconPair();

s64 mnuDrawIconPanel(s32 a0, s32 a1, s32 a2, s32 a3, u32 *state, s32 a5, s32 a6) {
    switch (*state) {
    case 0: case 1: case 2: case 3:
        return mnuDrawIconPanelFade(a0, a1, a2, a3, state, a5, a6);
    case 4:
        return mnuDrawIconRow6(a0, a1, a2, 0x100, state, a6);
    case 5:
        return mnuDrawIconPair(a0, a1, a2, a3, state, a6);
    }
}

void func_002BA7A8(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6) {
    mnuDrawIconPanel(a0, a1, a2, a3, a4, a5, a6);
}

void func_002BA7C0(u32 a0, u32 a1, u32 a2, s32 state, s32 option) {
    func_002BA7A8(a0, a1, a2, 0x100, state, 0, option);
}

typedef struct MenuGridHandles {
    u32 kind; /* 0x00 */
    u8 pad04[8];
    s32 compact[2]; /* 0x0C–0x10 */
    s32 expanded[4]; /* 0x14–0x20 */
} MenuGridHandles;

void mnuHideWindowHandlesKindFourFive(u32 obj) {
    switch (((MenuGridHandles *)obj)->kind) {
    case 4:
        itfGridLookupValueOrDefault(((MenuGridHandles *)obj)->expanded[0], 0);
        itfGridLookupValueOrDefault(((MenuGridHandles *)obj)->expanded[1], 0);
        itfGridLookupValueOrDefault(((MenuGridHandles *)obj)->expanded[2], 0);
        itfGridLookupValueOrDefault(((MenuGridHandles *)obj)->expanded[3], 0);
        return;
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 5:
        itfGridLookupValueOrDefault(((MenuGridHandles *)obj)->compact[0], 0);
        itfGridLookupValueOrDefault(((MenuGridHandles *)obj)->compact[1], 0);
        break;
    }
}

void func_002BA890(MenuList *list) {
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

void func_002BA8C8(MenuList *list) {
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

void mnuResetNodeLinks(s32 *menu, s32 reset) {
    s32 initial;
    s32 last;
    menu[9] = 0;
    initial = menu[4];
    last = menu[7];
    menu[6] = initial;
    menu[7] = initial;
    if (reset == 1) {
        s32 *node = (s32 *)initial;
        if (node == NULL) {
            return;
        }
        do {
            if ((s32)node == last) {
                return;
            }
            func_002B8CF0(menu);
            node = (s32 *)node[22];
        } while (node != NULL);
    }
}

void mnuLinkItemList(MenuListNode **items, s32 count) {
    s32 i;

    items[0]->prev = NULL;
    items[0]->next = items[1];
    for (i = 1; i < count - 1; i++) {
        items[i]->prev = items[i - 1];
        items[i]->next = items[i + 1];
    }
    items[count - 1]->prev = items[count - 2];
    items[count - 1]->next = NULL;
    for (i = 0; i < count; i++) {
        items[i]->index = i;
    }
}

s32 mnuComparePrimaryKeyDescending(s32 *left, s32 *right) {
    u32 temp_A = ((MenuListNode *)*left)->sortKeyPrimary;
    u32 temp_B = ((MenuListNode *)*right)->sortKeyPrimary;

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 mnuComparePrimaryKeyAscending(s32 *left, s32 *right) {
    u32 temp_A = ((MenuListNode *)*left)->sortKeyPrimary;
    u32 temp_B = ((MenuListNode *)*right)->sortKeyPrimary;

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 mnuCompareSecondaryKeyDescending(s32 *left, s32 *right) {
    u32 temp_A = ((MenuListNode *)*left)->sortKeySecondary;
    u32 temp_B = ((MenuListNode *)*right)->sortKeySecondary;

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 mnuCompareSecondaryKeyAscending(s32 *left, s32 *right) {
    u32 temp_A = ((MenuListNode *)*left)->sortKeySecondary;
    u32 temp_B = ((MenuListNode *)*right)->sortKeySecondary;

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 mnuCompareTertiaryKeyDescending(s32 *left, s32 *right) {
    u32 temp_A = ((MenuListNode *)*left)->sortKeyTertiary;
    u32 temp_B = ((MenuListNode *)*right)->sortKeyTertiary;

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 mnuCompareTertiaryKeyAscending(s32 *left, s32 *right) {
    u32 temp_A = ((MenuListNode *)*left)->sortKeyTertiary;
    u32 temp_B = ((MenuListNode *)*right)->sortKeyTertiary;

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF48);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF60);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF90);

void mnuSortItems(MenuList *menu, s32 sortKey, s32 descending) {
    s32 (*comparators[6])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 count = 0;
    s32 handle = func_003292A8(menu->count * 4);
    MenuListNode **items = (MenuListNode **)sdfResourceRetainAddress(handle);
    MenuListNode **out = items;
    MenuListNode *node;

    for (node = menu->first; node != NULL; node = node->next) {
        *out++ = node;
        count++;
    }
    if (descending != 0) {
        sortKey += 3;
    }
    func_0035B7F8(items, count, 4, comparators[sortKey]);
    mnuLinkItemList(items, count);
    func_002BA890(menu);
    func_002BA8C8(menu);
    mnuResetNodeLinks((s32 *)menu, 0);
    func_003297C8(handle);
}

void mnuAllocateListEntries(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        list[i + 1] = func_00328E18(0x18);
    }
}

extern void func_00328E48();

void mnuFreeListEntries(s32 *list) {
    s32 *entries = list + 1;
    u32 i = 0;
    do {
        func_00328E48(*entries++);
        i++;
    } while (i < 4);
}

void func_002BACF0(s32 firstValue, s32 secondValue, s32 *list) {
    u32 count = *list;
    s32 *slot = list + count;
    s32 *entry;

    if (count < 5) {
        return;
    }
    entry = (s32 *)slot[1];
    *list = count + 1;
    entry[0] = firstValue;
    entry[4] = secondValue;
}

typedef struct MenuFadeEntry {
    u32 active;
    u32 pad4[3];
    void *handle;
    u32 pad14;
} MenuFadeEntry;

void func_002BAD20(s32 *list, u32 index) {
    s32 *entries;
    s32 *slot;
    u32 i;

    if (index >= 5) {
        entries = list + 1;
        slot = entries + index;
        if (((MenuFadeEntry *)*slot)->active != 0) {
            mnuDestroyWindowContainer(((MenuFadeEntry *)*slot)->handle);
        }
        i = list[0] - 1;
        ((MenuFadeEntry *)*slot)->handle = 0;
        for (; index < i; i--) {
            *(MenuFadeEntry *)entries[i - 1] = *(MenuFadeEntry *)entries[i];
            ((MenuFadeEntry *)entries[i])->handle = 0;
        }
        list[0]--;
    }
}

void func_002BAE08(s32 image, s32 *list, s32 option) {
    u32 i;
    for (i = 0; i < (u32)list[0]; i++) {
        s32 *entry = (s32 *)list[i + 1];
        func_002B9EA0(entry[2], entry[3], image, entry[4], option);
    }
}

void mnuUpdateFade(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        s32 *entry = (s32 *)list[i + 1];
        if (entry[5] != 0) {
            entry[5] -= 0x40;
        } else {
            func_002BAD20(list, i);
        }
    }
}

typedef struct MenuFadeFields {
    u8 pad00[0xB4];
    u32 initial; /* 0xB4 */
    u32 opacity; /* 0xB8 */
    u32 offset;  /* 0xBC */
    u32 step;    /* 0xC0 */
} MenuFadeFields;

void func_002BAF10(u8 *menu) {
    memset(menu, 0, 0x98);
    ((MenuFadeFields *)menu)->initial = 0;
    ((MenuFadeFields *)menu)->opacity = 0x200;
    ((MenuFadeFields *)menu)->offset = 0;
    ((MenuFadeFields *)menu)->step = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAF50);

void func_002BB0D0(u8 *menu) {
    ((MenuFadeFields *)menu)->offset = 0;
    ((MenuFadeFields *)menu)->opacity = 0x200;
    ((MenuFadeFields *)menu)->step = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB0E8);

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

extern ScrollHandle *func_00304998(s32);

void mnuInitScrollHandles(u8 *menu) {
    ScrollHandle *handle;

    handle = func_00304998(1);
    *(ScrollHandle **)(menu + 0x3C) = handle;
    handle->inner->params->a = 10;
    handle->inner->params->b = 0;

    handle = func_00304998(3);
    *(ScrollHandle **)(menu + 0x40) = handle;
    handle->inner->params->a = 8;
    handle->inner->params->b = 4;
    handle->inner->params->c = 8;

    handle = func_00304998(1);
    *(ScrollHandle **)(menu + 0x44) = handle;
    handle->inner->params->a = 10;
    handle->inner->params->b = 0;
}

void func_002BB320(menu)
    u32 *menu;
{
    u32 *handles = menu + 15;
    u32 i = 0;
    do {
        effDestroyPackedBatch(*handles++);
        i++;
    } while (i < 3);
}

u8 *func_002BB370(u32 owner) {
    u8 *menu = (u8 *)func_00328D68(0x48);
    memset(menu, 0, 0x48);
    *(u32 *)(menu + 8) = 0;
    *(u32 *)(menu + 0xc) = 0;
    itfGridStorePosition(menu + 0x14, owner, 0x40);
    itfGridStorePosition(menu + 0x1c, owner, 0x41);
    itfGridStorePosition(menu + 0x24, owner, 0x44);
    itfGridStorePosition(menu + 0x2c, 0, 0);
    itfGridStorePosition(menu + 0x34, 0, 0);
    mnuInitScrollHandles(menu);
    return menu;
}

void func_002BB418(u32 menu) {
    func_002BB320();
    func_00328E48(menu);
}

typedef struct MenuSlotResourceState {
    u8 pad00[0x2C];
    s32 activeHandle;  /* 0x2C */
    u32 activeValue;   /* 0x30 */
    s32 pendingHandle; /* 0x34 */
    u32 pendingValue;  /* 0x38 */
    u8 pad3C[8];
    u32 settings;      /* 0x44 */
} MenuSlotResourceState;

void func_002BB440(s32 context) {
    s32 pendingHandle;

    pendingHandle = ((MenuSlotResourceState *)context)->pendingHandle;
    ((MenuSlotResourceState *)context)->activeHandle = pendingHandle;
    ((MenuSlotResourceState *)context)->activeValue = ((MenuSlotResourceState *)context)->pendingValue;
    ((MenuSlotResourceState *)context)->pendingHandle = 0;
    if (pendingHandle != 0) {
        effConfigureWithDefaultSetting(pendingHandle, ((MenuSlotResourceState *)context)->pendingValue,
                                       ((MenuSlotResourceState *)context)->settings, 0, 10, 2);
        return;
    }
}

void func_002BB498(u32 *menu, u32 model, u32 value, u32 color) {
    func_002BB440(menu);
    menu[1] = color;
    menu[13] = model;
    menu[14] = value;
    func_003059E0(model, value, menu[15], 0, 3);
}

u8 func_002BB500(s32 resources) {
    return ((MenuSlotResourceState *)resources)->activeHandle != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB510);

void func_002BB850(s32 slot, u32 model, u32 firstValue, u32 secondValue, s32 thirdValue
                                    ) {
    u32 handle;

    handle = effCreateResourceSlotSet(model, firstValue, 1);
    *(u32 *)(slot + 0xe4) = handle;
    handle = effCreateResourceSlotSet(model, secondValue, 1);
    *(u32 *)(slot + 0xe8) = handle;
    if (-1 < thirdValue) {
        handle = effCreateResourceSlotSet(model, thirdValue, 1);
        *(u32 *)(slot + 0xec) = handle;
    }
}

void func_002BB8D8(s32 menu) {
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
            func_003054E8(slot[0x38]);
        }
        if (slot[0x39] != 0) {
            func_003054E8(slot[0x39]);
        }
        if (slot[0x3a] != 0) {
            func_003054E8(slot[0x3a]);
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

typedef struct MenuPartySlotValues {
    u8 pad00[0x60];
    u32 mainScale; /* 0x60 */
    u32 mainFade;  /* 0x64 */
    u8 pad68[0x48];
    u32 backScale; /* 0xB0 */
    u32 backFade;  /* 0xB4 */
} MenuPartySlotValues;

void func_002BB998(u8 *menu, s32 index, u32 unused, u32 preserve) {
    u8 *entry = menu + index * 0x2138 + 0x78;
    ((MenuPartySlotValues *)entry)->mainFade = 0;
    ((MenuPartySlotValues *)entry)->backFade = 0;
    if (preserve == 0) {
        ((MenuPartySlotValues *)entry)->mainScale = 0x100;
        ((MenuPartySlotValues *)entry)->backScale = 0x100;
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

extern s32 func_003054E8();

extern void func_00328E48();

typedef struct MenuSprites {
    u8 unk0[0x10];
    void *icon[5];
    void *item[11];
    void *cursor[4];
} MenuSprites;

void mnuFreeIconSprites(MenuSprites *menu) {
    u32 i;
    for (i = 0; i < 5; i++) {
        func_003054E8(menu->icon[i]);
    }
    for (i = 0; i < 11; i++) {
        if (menu->item[i] != NULL) {
            func_003054E8(menu->item[i]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (menu->cursor[i] != NULL) {
            func_003054E8(menu->cursor[i]);
        }
    }
    func_00328E48(menu);
}

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct MenuIconSet {
    u8 pad0[0x10];
    s32 icon[5];
    u8 pad24[0x3C];
    s32 sprite;
} MenuIconSet;

void mnuDrawIconRow(s32 a0, s32 a1, s32 x, s32 skip, MenuIconSet *set, s32 arg) {
    u32 i;
    if (skip == 0) {
        for (i = 0; i < 5; i++) {
            func_00306CD0(0xBC0, 0x3C8, x, set->sprite, 0, set->icon[i], 0, arg);
        }
    }
}

extern void *func_002BBA38();

typedef struct MenuWindow {
    u8 unk0[0x154 - 0x78];
    void *handle;
    u8 unk158[0x2138 - 0xDC - 4];
} MenuWindow;

typedef struct MenuWindows {
    u32 flags;
    u8 unk4[0x74];
    MenuWindow win[5];
} MenuWindows;

void mnuSetWindowResource(s32 index, MenuWindows *menu, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6) {
    func_002BD1D0(menu, index);
    menu->win[index].handle = func_002BBA38(0, a2, a3, a4, a5, a6);
    menu->flags |= 0x80;
}

void func_002BC078(s32 index, u8 *menu, u32 first, u32 second) {
    u8 **slot = (u8 **)(menu + index * 0x2138 + 0x154);
    if (*slot != NULL) {
        (*slot)[0x74] = first;
        (*slot)[0x75] = second;
    }
}

void mnuClearEntries(u8 *menu) {
    u8 *entry = menu + 0x154;
    u32 i = 0;
    func_002BD2E0(menu);
    do {
        if (*(u32 *)entry != 0) {
            mnuFreeIconSprites(*(u32 *)entry);
            *(u32 *)entry = 0;
        }
        i++;
        entry += 0x2138;
    } while (i < 5);
    *(u32 *)menu &= ~0x80;
}

extern void func_003071D0();

typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

typedef struct MenuIconBundle {
    u32 unk0[3];
    void *sprite[3];
    u32 unk18[2];
} MenuIconBundle;

extern MenuIconLayout D_0042AFD8;

u32 mnuCreateIconBundle(u32 resource) {
    MenuIconLayout layout = D_0042AFD8;
    MenuIconBundle *set = (MenuIconBundle *)func_00328D68(0x20);
    u32 i;
    memset(set, 0, 0x20);
    for (i = 0; i < 3; i++) {
        void *sprite = (void *)effCreateResourceSlotSet(resource, layout.entry[i].id, 1);
        set->sprite[i] = sprite;
        func_003071D0(sprite, 0, layout.entry[i].x - 0xc80, layout.entry[i].y - 0x20, 0, 0);
    }
    return (u32)set;
}

void func_002BC258(u32 *menu) {
    u32 i = 0;
    do {
        func_003054E8(menu[i + 3]);
        i++;
    } while (i < 3);
    func_00328E48(menu);
}

typedef struct MenuFadeIcons {
    u8 unk0[0xC];
    s32 icon[3];
    s32 fade;
    s32 fadeOut;
} MenuFadeIcons;

void mnuDrawFadeIcons(s32 a0, s32 a1, s32 a2, s32 a3, MenuFadeIcons *obj, s32 a5) {
    s32 fade = obj->fade;
    s32 next;
    func_00306CD0(a0, a1, a2, fade, 0, obj->icon[0], 0, a5);
    func_00306CD0(a0, a1, a2, fade, 0, obj->icon[1], 0, a5);
    func_00306CD0(a0, a1, a2, fade, 0, obj->icon[2], 0, a5);
    if (obj->fadeOut == 0) {
        next = obj->fade;
        if (next < 0x100) {
            obj->fade = next + 0x10;
            next = obj->fade;
        }
        if (next > 0x100) {
            obj->fade = 0x100;
        }
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

void func_002BC3C8(s32 index, s32 menu, u32 resource) {
    u32 bundle;

    bundle = mnuCreateIconBundle(resource);
    *(u32 *)(index * 0x2138 + menu + 0x158) = bundle;
}

void func_002BC410(u8 *menu) {
    u8 *slot = menu + 0x158;
    u32 i = 0;
    do {
        u32 resource = *(u32 *)slot;
        i++;
        if (resource != 0) {
            func_002BC258((u32 *)resource);
            *(u32 *)slot = 0;
        }
        slot += 0x2138;
    } while (i < 5);
}

s32 func_002BC460(s32 value, s32 total) {
    if (total > 0) {
        return value * 100 / total;
    }
    return 100;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC498);

void func_002BC580(u8 *menu) {
    u32 i = 0;
    do {
        func_002C21F8((s32)(menu + 0x94));
        func_002C21F8((s32)(menu + 0xe4));
        menu += 0x2138;
        i++;
    } while (i < 5);
}

void func_002BC5D0(s32 menu, u32 *source) {
    u32 value;
    u32 *destination;
    u32 index;

    destination = (u32 *)(menu + 0x24);
    index = 0;
    do {
        value = *source;
        source = source + 1;
        index = index + 1;
        *destination = value;
        destination = destination + 1;
    } while (index < 8);
}

void func_002BC600(s32 menu, u32 *source) {
    u32 value;
    u32 *destination;
    u32 index;

    destination = (u32 *)(menu + 0x44);
    index = 0;
    do {
        value = *source;
        source = source + 1;
        index = index + 1;
        *destination = value;
        destination = destination + 1;
    } while (index < 8);
}

void mnuRegisterResourceHandles(s32 destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(source[i]);
        *(s32 *)(destination + 0x64 + 4 * i) = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC690);

void mnuRefreshWindowSlots(u8 *menu, s32 flag) {
    u32 i;
    s32 offset;
    u32 *res;
    if (flag == 0) {
        for (i = 0, res = (u32 *)(menu + 0x24); i < 8; i++, res++) {
            if (func_00305080(*res) != 0) {
                func_00305068(*res);
                func_00305068(res[8]);
            }
        }
    }
    for (i = 0, offset = 0; i < 5; i++, offset += 0x34) {
        u8 *entries = *(u8 **)(menu + 8);
        if (*(s32 *)(entries + offset + 0x10) >= 0) {
            if ((s32)i < *(s32 *)entries) {
                func_002BC690(menu, i, 1);
            } else {
                func_002BC690(menu, i, 2);
            }
        } else {
            func_002BC690(menu, i, 0);
        }
    }
}

void func_002BCA98(u32 menu) {
    mnuRefreshWindowSlots(menu, 0);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCAB0);

extern char D_00437C30[];

void mnuInitScrollLists(u8 *menu, s32 *counts) {
    s32 i = 0;
    *(u32 **)(menu + 0xa690) = mnuCreateListState(0, 1, 1);
    *(u32 **)(menu + 0xa694) = mnuCreateListState(0, 1, 1);
    if (counts[0] + counts[1] > 0) {
        do {
            mnuListAppendNode(*(u32 **)(menu + 0xa690), D_00437C30);
            i++;
            mnuListAppendNode(*(u32 **)(menu + 0xa694), D_00437C30);
        } while (i < counts[0] + counts[1]);
    }
}

void func_002BCCB0(context)
    s32 context;
{
    func_002B81C8(((MenuContext *)context)->selectedPartyList);
    func_002B81C8(((MenuContext *)context)->secondPartyList);
}

void func_002BCCF0(u32 context, u32 counts) {
    func_002BCCB0();
    mnuInitScrollLists(context, counts);
}

typedef struct MenuSlotWindow {
    u8 unk0[0xC0];
    u32 fieldC0;
    u8 unkC4[0x110 - 0xC4];
    u32 field110;
    u8 unk114[0x2138 - 0x114];
} MenuSlotWindow;

typedef struct MenuWindowSet {
    u32 flags;
    u8 unk4[0x14];
    MenuSlotWindow slots[5];
    u8 unkA630[0xA698 - 0xA630];
    s32 selected;
} MenuWindowSet;

void func_002BCD28(MenuWindowSet *set) {
    if (set->selected >= 0) {
        MenuSlotWindow *slots = set->slots;
        slots[set->selected].fieldC0 = 0x100;
        slots[set->selected].field110 = 0x100;
        set->selected = -1;
    }
    set->flags &= ~0x200;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCD90);

typedef struct MenuWindowSprites {
    u8 unk0[0x10];
    u32 icon[3];
    u8 unk1C[0xBC - 0x1C];
    u32 frame[8];
} MenuWindowSprites;

void mnuFreeWindowSprites(MenuWindowSprites *win) {
    u32 i;
    for (i = 0; i < 3; i++) {
        if (win->icon[i] != 0) {
            func_003054E8(win->icon[i]);
        }
    }
    if (win->frame[0] != 0) {
        func_003054E8(win->frame[0]);
    }
    if (win->frame[1] != 0) {
        func_003054E8(win->frame[1]);
    }
    if (win->frame[2] != 0) {
        func_003054E8(win->frame[2]);
    }
    if (win->frame[3] != 0) {
        func_003054E8(win->frame[3]);
    }
    if (win->frame[4] != 0) {
        func_003054E8(win->frame[4]);
    }
    if (win->frame[5] != 0) {
        func_003054E8(win->frame[5]);
    }
    if (win->frame[6] != 0) {
        func_003054E8(win->frame[6]);
    }
    if (win->frame[7] != 0) {
        func_003054E8(win->frame[7]);
    }
}

void mnuShutdownContext(u8 *ctx) {
    u8 *slot = ctx + 0x78;
    u32 i;
    for (i = 0; i < 5; i++, slot += 0x2138) {
        mnuFreeWindowSprites(slot);
    }
    func_002BC580(ctx);
    func_002BCCB0(ctx);
}

typedef struct MenuPageGauge {
    s32 id;
    u8 pad4[4];
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    u8 pad18[0xC];
} MenuPageGauge;

typedef struct MenuPageRecord {
    u8 pad0[0xC];
    s32 partyIndex;
    MenuPageGauge gauge;
} MenuPageRecord;

typedef struct MenuPageWindow {
    u32 flags;
    u8 pad4[4];
    MenuPageRecord *records;
    u8 padC[0x18];
    s32 handlesA[8];
    s32 handlesB[8];
    s32 handlesC[5];
    u8 pad78[0xA698 - 0x78];
    s32 selected;
} MenuPageWindow;

void func_002BCFC8(MenuPageWindow *window) {
    s32 selected = window->selected;
    s32 offset = 0;
    u32 i;

    for (i = 0; i < 5; i++, offset += sizeof(MenuPageRecord)) {
        if (i != selected) {
            s32 id = ((MenuPageRecord *)((u8 *)window->records + offset))->gauge.id;

            if (id >= 0) {
                if (func_00305080(window->handlesA[id]) == 0) {
                    effResolveAndReleaseResource(window->handlesA[id]);
                    effResolveAndReleaseResource(window->handlesB[id]);
                }
            }
        }
    }
}

void func_002BD090(MenuPageWindow *window) {
    s32 selected = window->selected;
    u32 i;
    s32 id;
    MenuPageRecord *record;

    for (i = 0; i < 5; i++) {
        record = &window->records[i];
        id = record->gauge.id;
        if (id >= 0) {
            if (func_00305080(window->handlesA[id]) != 0) {
                func_00305068(window->handlesA[id]);
                func_00305068(window->handlesB[id]);
            }
        }
    }
    record = &window->records[selected];
    id = record->gauge.id;
    if (id >= 0) {
        if (func_00305080(window->handlesA[id]) == 0) {
            effResolveAndReleaseResource(window->handlesA[id]);
            effResolveAndReleaseResource(window->handlesB[id]);
        }
    }
}

typedef struct MenuHandleSet {
    u8 pad0[0x20];
    s32 a[8];
    s32 b[8];
    s32 c[5];
} MenuHandleSet;

extern s32 mnuGetSelectionFromFlags(s32);

void func_002BD1D0(MenuPageWindow *window, s32 selected) {
    s32 *resource = window->handlesC;
    u32 i;
    MenuHandleSet *handles = (MenuHandleSet *)((u8 *)window + 4);
    MenuPageRecord *record;
    s32 active;

    for (i = 0; i < 5; i++) {
        func_00305068(*resource++);
    }
    record = &window->records[selected];
    active = mnuGetSelectionFromFlags(D_00435DD0 + record->partyIndex * 0x1C4 + 0xA60);
    for (i = 0; i < 5; i++) {
        if (i == active) {
            effResolveAndReleaseResource(handles->c[i]);
        }
    }
    if (window->selected >= 0) {
        func_002BCFC8(window);
    }
    window->selected = selected;
    func_002BD090(window);
}

void func_002BD2E0(MenuPageWindow *window) {
    s32 *resource = window->handlesC;
    u32 i;

    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(*resource++);
    }
    if (window->selected >= 0) {
        func_002BCFC8(window);
    }
    func_002BCD28((MenuWindowSet *)window);
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

void func_002BD3A8(s32 menu) {
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

