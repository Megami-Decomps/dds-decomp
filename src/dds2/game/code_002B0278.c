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

extern void menuFreeWindowSprites();

extern u8 *func_002B8A50();

extern u8 *func_002B8BA8();

extern void menuHideIconGroup();

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

extern void func_002AAC98();

extern char D_003E69B0[];

extern void mnuCreateStaffImageSprite();

extern void func_002AA7A0();

extern void func_002BB0E8();

extern void mnuIdleVoiceTimer();

extern void func_002B2408();

extern u32 menuCreateIconBundle(u32);

extern u32 func_002B9FF8();

extern s32 D_00435DD0;

typedef struct MenuSlot {
    s32 resources[3];
    u16 unused;
    u16 flags;
} MenuSlot;

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
    u8 pad8[0x50];
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
} MenuList;

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

extern void func_002B9520(u32);

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
    resourceOwner = *(s32 *)(context + 0xaa48);
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
    s32 slot = D_00435DD0 + **(s32 **)(*(s32 *)(context + 0xa914) + 0x1c) * 0x1c4 + 0xa60;
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

void func_002B0D50(u32 arg0) {
    func_002A9460(4, arg0);
}

void func_002B0D70(u32 callback) {
}

s32 mnuIsFinalItemIndex(s32 index, s32 item) {
    if (index < (*(s32 *)(item + 0x20) - 1)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0D90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0FA0);

void func_002B1150(s32 context) {
    func_002B9520(*(u32 *)(*(s32 *)(context + 0xaa48) + 8));
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
void menuCopyPartyEntries(context)
s32 context;
{
    PartyMenuData *menu = (PartyMenuData *)*(s32 *)(context + 0xAA48);
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
    PartyMenuData *menu = (PartyMenuData *)*(s32 *)(context + 0xAA48);
    s32 i;
    s32 node;

    menuCopyPartyEntries();
    menu->selection = 0;
    memset(menu->backup, 0, 0x8D4);
    *(s32 *)(context + 0xA928) = 1;
    *(s32 *)(context + 0xA92C) = mnuCountActiveSlots() - 1;
    func_002BCA98(context + 0x284);
    for (i = 0; i < 5; i++) {
        *(u32 *)(context + 0x300 + i * 0x2138) |= 0x40;
    }
    for (node = *(s32 *)(*(s32 *)(*(s32 *)((s32)menu + 8) + 0x18) + 0x10); node != 0; node = *(s32 *)(node + 0x58)) {
        *(u32 *)(node + 0x48) &= ~1;
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
    s32 party = *(s32 *)(menu + 0xAA48);

    func_002B15F8();
    func_002C42C0(menu + 0x54, D_003E7588);
    func_002BB498(*(s32 *)(menu + 0x118), *(s32 *)(menu + 0x60), 0, 1);
    func_002BAF50(*(s32 *)(menu + 0x104), menu + 0xB10C);
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
    u8 *menu = *(u8 **)(context + 0xaa48);
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
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B2790(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

u8 func_002B27C8(void) {
    s64 temp_v0;

    temp_v0 = func_002C6CE8();
    return temp_v0 != 1;
}

void func_002B27F0(u32 arg0) {
    func_002A9460(3, arg0);
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

u32 menuCreateSelectState(u32 arg0, s32 flag) {
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
    func_002BB498(*(s32 *)(context + 0x118), *(void **)(context + 0x68), 0, 0);
    evtStageTestInit(0);
    return 1;
}

s32 func_002B2970(void) {
    s32 context = func_00101958();
    s32 party = *(s32 *)(context + 0xAA48);

    mnuResetWorkFloats();
    func_002B2810(context);
    func_003297C8(*(s32 *)party);
    return 1;
}

void func_002B29C8(u32 arg0) {
    menuCreateSelectState(arg0, 1);
}

void func_002B29E0(void) {
    func_002B2970();
}

void func_002B29F8(u32 arg0) {
    menuCreateSelectState(arg0, 0);
}

void func_002B2A10(void) {
    func_002B2970();
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2A28);

s64 func_002B2B48(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    if (func_00305080(*(s32 *)(context + 0x64))) {
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
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    if (menu[9] != 0) {
        func_002AAC98(0, **(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c), D_003E69B0, context, 1, 0x53);
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
    s32 *menu = *(s32 **)(context + 0xaa48);
    s32 window;
    func_002BAF50(*(s32 *)(context + 0x104), context + 0xb10c);
    window = context + 0x284;
    func_002B2C88(window, 0, menu[5], menu[4]);
    evtStageTestStop();
    mnuClearEntries(window);
    func_002BC410(window);
    if (*(s32 *)(context + 0xaa34) != 0) {
        mnuDestroyPanelGroup(*(s32 *)(context + 0xaa34));
        *(s32 *)(context + 0xaa34) = 0;
    }
    if (*(s32 *)(context + 0xaa38) != 0) {
        func_002C1050(*(s32 *)(context + 0xaa38));
        *(s32 *)(context + 0xaa38) = 0;
    }
    if (*(s32 *)(context + 0xaa3c) != 0) {
        func_002C16D8(*(s32 *)(context + 0xaa3c));
        *(s32 *)(context + 0xaa3c) = 0;
    }
    if (*(s32 *)(context + 0xaa5c) != 0) {
        func_002C3390(*(s32 *)(context + 0xaa5c));
        *(s32 *)(context + 0xaa5c) = 0;
    }
    mnuReleaseResourceList(menu[8]);
    return 1;
}

void func_002B3120(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0xa914) + 0x1c) * 0x2138 + arg0 + 0x3d8) + 0x60) =
              0x100;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3150);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3260);

void mnuDrawSlotIcons(s32 x, s32 context) {
    s32 slot = D_00435DD0 + **(s32 **)(*(s32 *)(context + 0xa690) + 0x1c) * 0x1c4 + 0xa60;
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

void func_002B34E0(s32 arg0, u32 *arg1) {
    s32 temp_v0;

    temp_v0 = 0x100 - *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0xa690) + 0x1c) * 0x2138 + arg0
                                                                      + 0x154) + 0x60);
    func_00306CD0(0xa0, 0xa30, 0, temp_v0, 1, arg1[1], 0x55, 0x53);
    func_00306CD0(0x30, 0xaf8, 0, temp_v0, 1, *arg1, 0x1a, 0x53);
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

void func_002B3648(u8 *entry, s32 id, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    mnuApplyPackedGroupValues(arg2, *(u16 *)(entry + 0x1b2));
    func_002C0D18(0xeb0, 0x518, 0, entry, arg2, 0, arg5);
    func_002C10F0(0, 0, 0, entry, arg3, arg5);
    mnuDrawTextSprite(0x2a0, 0xa50, 0, 0xa09dc380, D_00435E48 + *(u16 *)(entry + 4) * 0x11 + 0x110, arg5);
    mnuDrawSlotIcons(0x14a, id);
}

void func_002B3720(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5) {
    func_002C16F0(0, 0, 0, arg0, *(u8 *)((s32)arg0 + 0x55), arg2, arg5);
    func_002C3E08(0xe80, 0x5b8, 0, arg3, arg5);
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
    s32 *menu = *(s32 **)(context + 0xaa48);
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
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B40F8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4180);

void func_002B4270(u32 arg0) {
    func_002A9460(1, arg0);
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

u32 *menuBuildOwnedSkillBits(void) {
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

s32 mnuIsSkillCodeInBitset(s32 arg0, u32 *arg1) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;

    return (arg1[temp_v0 >> 5] & (1 << arg0)) != 0;
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
            func_002B9520(*resource++);
            i++;
        } while (i < 4);
        menu[2] = 0;
    }
}

u32 menuCreateItemState(s32 callback) {
    s32 context = func_00101958();
    u32 handle = func_003292A8(0x3c);
    u32 *state = (u32 *)sdfResourceRetainAddress(handle);
    *(u32 **)(context + 0xaa48) = state;
    memset(state, 0, 0x3c);
    state[0] = handle;
    func_002B4270(context);
    switch (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c)) {
    case 0:
        func_002BB498(*(s32 *)(context + 0x118), *(s32 *)(context + 0xc8), 0, 0);
        break;
    case 2:
        func_002BB498(*(s32 *)(context + 0x118), *(s32 *)(context + 0xc8), 0x19, 0);
        break;
    case 3:
        func_002BB498(*(s32 *)(context + 0x118), *(s32 *)(context + 0xc8), 0xa, 0);
        break;
    }
    mnuSeekListNode(0, (MenuList *)*(s32 *)(*(s32 *)(context + 0x10c) + 0x18));
    return 1;
}

s32 func_002B4DE8(s32 selection) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
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
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xe);
    }
    func_002AAC98(0, **(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c), D_003E69B0, context, 1, 0x53);
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    }
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B5128(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

void func_002B5160(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) = 0xffffffff;
}

u32 func_002B5190(s32 callback) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) >> 0x1f;
}

void func_002B51C8(void) {
    u8 *state = *(u8 **)(func_00101958() + 0xaa48);
    u8 *node = *(u8 **)(*(u8 **)(*(u8 **)(state + 0x24) + 0x18) + 0x10);
    while (node != NULL) {
        if (*(u32 *)node == *(u32 *)(state + 0x34)) {
            *(u32 *)(node + 0x48) |= 2;
        } else {
            *(u32 *)(node + 0x48) &= ~2;
        }
        node = *(u8 **)(node + 0x58);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5240);

u32 func_002B5358(void) {
    s32 context = func_00101958();
    u32 *state = *(u32 **)(context + 0xaa48);
    s32 image = *(s32 *)(context + 0x104);
    if (**(s32 **)(*(s32 *)(image + 0x18) + 0x1c) == 0) {
        func_002BAF50(image, context + 0xb10c);
    }
    state[12] = 0;
    return 1;
}

void func_002B53B8(s32 obj, s32 id, s32 slot) {
    u16 code = id;

    if (ptyHasSkill(obj, code) == 0) {
        *(u16 *)(obj + slot * 2 + 0x22) = code;
        ptyRecomputeMaxHpMp(obj);
        scrClearSecondaryScriptFlag(obj, code);
    }
}

void func_002B5430(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 2 + arg0 + 0x22) = 0;
    ptyRecomputeMaxHpMp();
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5450);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5580);

void func_002B5778(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0 = (u8 *)(arg0 + 2);
    s32 temp_v1 = arg1 * 2 + 32;
    s32 temp_v2 = arg2 * 2 + 32;
    u16 temp_v3 = *(u16 *)(temp_v0 + temp_v1);
    u16 temp_v4 = *(u16 *)(temp_v0 + temp_v2);

    *(u16 *)(temp_v0 + temp_v1) = temp_v4;
    *(u16 *)(temp_v0 + temp_v2) = temp_v3;
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
    s32 *menu = *(s32 **)(context + 0xaa48);
    s32 label;
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
        func_002B5A30(context);
    }
    func_002AAE80(callback);
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
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
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B5DB0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5DE8);

void menuFlagMatchingEntries(s32 context) {
    s32 slot = D_00435DD0 + **(s32 **)(*(s32 *)(context + 0xa914) + 0x1c) * 0x1c4 + 0xa60;
    MenuLink *link = *(MenuLink **)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0xaa48) + 0x24) + 0x18) + 0x10);
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
        *(u32 *)(context + 0x284) |= 0x10;
    }
    if (func_002C50C0(code) == 3) {
        *(u32 *)(context + 0x284) |= 0x20;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(8, window);
    if (buttons & 1) {
        buttons = func_002B5DE8(label, context) == 0 ? 0x8000 : 0;
        menuFlagMatchingEntries(context);
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
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B61C0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B61F8);

u32 func_002B62A8(u32 callback) {
    s32 context = func_00101958();
    func_002BAF50(*(u32 *)(context + 0x104), context + 0xb10c);
    func_002B40B8(callback);
    func_002B4C48(context);
    func_002BD2E0(context + 0x284);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6308);

void func_002B63F0(s32 menu, s32 target) {
    s32 *entry = (s32 *)menu + **(s32 **)(*(s32 *)(menu + 4) + 0x1C);
    s32 list = entry[2];
    s32 delta = target - *(s32 *)(*(s32 *)(list + 0x18) + 0x24);
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
    func_002B8FD8(*(s32 *)(list + 0x18));
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6498);

s64 func_002B66D8(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
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
    func_002AA7A0(3, *(s32 *)(context + 0x60));
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
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

u32 func_002B6FE8(u32 callback) {
    s32 context;
    u32 *state;
    menuCreateItemState(callback);
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
    func_002BAF50(*(u32 *)(context + 0x104), context + 0xb10c);
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
    mnuPlayInputSound(0, buttons, *(s32 *)(list[8 + menu[11]] + 0x18));
    if (buttons & 2) {
        func_002C42C0(popup, D_003E7720);
        func_002BB498(*(s32 *)(context + 0x118), *(s32 *)(context + 0x60), 0, 1);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7228);

void func_002B7588(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = **(s32 **)(arg0 + 0x28c); temp_v0 < 3; temp_v0 = temp_v0 + 1) {
    }
}

s64 func_002B75C8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
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
        func_002B6898(label, *(s32 *)(context + 0x64), *(s32 *)(context + 0xc8));
    }
    func_002AA7A0(2, *(s32 *)(context + 0x60));
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

void func_002B7730(u32 arg0, u32 *arg1) {
    *arg1 = *arg1 | arg0;
}

void func_002B7740(s32 arg0, s32 arg1) {
    u32 *puVar1;
    s32 temp_v0;
    u32 *puVar3;
    s32 temp_v1;
    s32 temp_v2;
    u32 temp_v3;

    temp_v3 = 0;
    temp_v2 = 0;
    do {
        puVar3 = (u32 *)(arg1 + 0x40);
        temp_v0 = temp_v2 << 2;
        temp_v1 = 3;
        do {
            puVar1 = (u32 *)(temp_v0 + arg0);
            temp_v0 = temp_v0 + 4;
            temp_v1 = temp_v1 - 1;
            *puVar3 = *puVar1;
            puVar3 = puVar3 + 1;
        } while (-1 < temp_v1);
        temp_v3 = temp_v3 + 1;
        arg1 = arg1 + 0x10;
        temp_v2 = temp_v2 + 4;
    } while (temp_v3 < 2);
}

void func_002B7790(u32 first, u32 second, u32 *menu) {
    menu[2] = first;
    menu[15] = second;
}

void func_002B77A0(s32 arg0) {
    func_003059E0(*(u32 *)(arg0 + 8), *(u32 *)(arg0 + 0x1c),
                                *(u32 *)(arg0 + 0x3c), 0, 4);
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
    *(u32 *)(effect + 8) = effLoadIndexedResource("/camp/spr/n_min/", D_003E7818[0], 0);
    *(u32 *)(effect + 0x3c) = effLoadMappedResource("/camp/mot/", D_003E7820[0]);
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
    effDestroyPackedBatch(*(u32 *)(ctx + 0x3c));
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

void func_002B7A80(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 4 + arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x160) = *(s32 *)(arg0 + 0x160) - 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7AA0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7C10);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7E60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7F80);

void func_002B8140(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffb;
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

u32 func_002B81C8(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_002B86E8(arg0);
    } while (temp_v0 != 0);
    func_00328E48(arg0);
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

void func_002B8988(MenuList *list) {
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

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B89E0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8A50);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8BA8);

void func_002B8CF0(u32 arg0) {
    func_002B8A50(arg0, 0, 0);
}

void func_002B8D10(u32 arg0) {
    func_002B8BA8(arg0, 0, 0);
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

void func_002B8F98(u32 *arg0) {
    *arg0 &= ~1;
    *arg0 &= ~2;
}

u32 func_002B8FB8(u32 *arg0) {
    return *arg0 & 2;
}

s32 func_002B8FC8(s32 arg0) {
    return *(s32 *)(arg0 + 0x28) * *(s32 *)(arg0 + 0xc);
}

void func_002B8FD8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x50) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x58), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x50) = 0;
        }
    }
}

void func_002B9010(u8 *menu) {
    u8 *node = *(u8 **)(menu + 0x10);
    if (node != NULL) {
        do {
            s32 timer = *(s32 *)(node + 0x50);
            s32 reduced = timer - 0x10;
            if (timer > 0) {
                *(s32 *)(node + 0x50) = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                *(s32 *)(node + 0x50) = 0;
            }
            node = *(u8 **)(node + 0x58);
        } while (node != NULL);
    }
}

void mnuDrawFourEntries(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 base8 = arg4 + 8;
    s32 baseC = arg4 + 0xC;
    u32 i = 0;
    do {
        s32 eq = arg4 == *(s32 *)(arg3 + 0x1C);
        s32 off = (eq * 4 + i) * 8;
        s32 value = *(s32 *)(base8 + off);
        if (value != 0) {
            func_00306F80(arg0, arg1, arg2, 0, value, *(s32 *)(baseC + off), arg5);
        }
        i++;
    } while (i < 4);
}

extern u32 func_00309138(u32 color, u32 previous, s32 blend);

u32 func_002B9138(u32 backup, u8 *node) {
    u32 flags = *(u32 *)(node + 0x48);
    u32 color = 0x89bdc940;
    if (!(flags & 1)) {
        color = (flags & 4) ? 0xbbefab80 : 0x89bdc980;
    }
    return func_00309138(color, backup, *(s32 *)(node + 0x50));
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

void mnuCallInitWide(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_002B9218(arg0, arg1, arg2, 0, 0, 0x100, 0, arg3, arg4);
}

s32 mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 left, s32 right) {
    s32 item = func_00328E18(0x98);
    s32 child;
    *(s32 *)(item + 0x10) = width;
    *(s32 *)(item + 0x14) = height;
    *(s32 *)item = id;
    child = mnuCreateListState(id, left, right);
    *(s32 *)(item + 0x94) = 0;
    *(s32 *)(item + 0x18) = child;
    return item;
}

void func_002B9520(u32 arg0) {
    s32 temp_v0;

    func_002B81C8(*(u32 *)((s32)arg0 + 0x18));
    temp_v0 = *(s32 *)((s32)arg0 + 0x90);
    if (temp_v0 != 0) {
        func_002B99D8(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002B9560(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_002B9568(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x94) = arg1;
}

void func_002B9570(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7,
                                    u32 arg8) {
    *(u32 *)(arg0 + 0x2c) = arg1;
    *(u32 *)(arg0 + 0x4c) = arg8;
    *(u32 *)(arg0 + 0x30) = arg2;
    *(u32 *)(arg0 + 0x34) = arg3;
    *(u32 *)(arg0 + 0x38) = arg5;
    *(u32 *)(arg0 + 0x48) = arg4;
    *(u32 *)(arg0 + 0x3c) = arg6;
    *(u32 *)(arg0 + 0x40) = arg7;
    *(u32 *)(arg0 + 0x44) = 0;
}

void func_002B95A0(s32 menu, u32 first, u32 second) {
    func_002B9570(menu, first, second, 0, 0, 0, 0, 0, 0);
}

void func_002B95D0(u32 first, u32 *menu, u32 second, u32 third, u32 fourth) {
    menu[7] = first;
    menu[8] = second;
    menu[9] = third;
    menu[10] = fourth;
}

void func_002B95E8(u8 *panel, const void *layout, u32 left, u32 top,
                   u32 right, u32 bottom) {
    memcpy(panel + 0x58, layout, 0x38);
    *(u32 *)(panel + 0x7c) = left;
    *(u32 *)(panel + 0x80) = top;
    *(u32 *)(panel + 0x84) = right;
    *(u32 *)(panel + 0x88) = bottom;
    *(u32 *)(panel + 4) |= 4;
}

void mnuCreateListWithDefaults(u8 *menu, u32 first, u32 second, u32 third, u32 fourth) {
    MenuListDefaults defaults = D_0042AF00;
    *(u32 **)(menu + 0x90) =
        func_002B9918(first, second, third, fourth, (u32)&defaults, 3);
}

void func_002B96D8(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_002B96F0(s32 arg0) {
    mnuListAppendNode(*(u32 *)(arg0 + 0x18));
}

void func_002B9708(s32 arg0) {
    func_002B83A0(*(u32 *)(arg0 + 0x18));
}

void func_002B9720(s32 arg0) {
    func_002B86E8(*(u32 *)(arg0 + 0x18));
}

u8 *mnuAdvanceListSelection(u8 *menu, s32 arg1) {
    u8 *item = func_002B8A50(*(s32 *)(menu + 0x18), arg1, 0);
    if (item != NULL) {
        item[0x54] = 0;
        menuHideIconGroup(menu + 0x58);
    }
    return item;
}

u8 *mnuReverseListSelection(u8 *menu, s32 arg1) {
    u8 *item = func_002B8BA8(*(s32 *)(menu + 0x18), arg1, 0);
    if (item != NULL) {
        item[0x54] = 0;
        menuHideIconGroup(menu + 0x58);
    }
    return item;
}

void func_002B97D8(u32 arg0) {
    mnuAdvanceListSelection(arg0, 0);
}

void func_002B97F0(u32 arg0) {
    mnuReverseListSelection(arg0, 0);
}

void func_002B9808(s32 arg0) {
    func_002B8F98(*(u32 *)(arg0 + 0x18));
}

void func_002B9820(s32 arg0) {
    func_002B8FB8(*(u32 *)(arg0 + 0x18));
}

typedef struct MenuIconSprites {
    u32 handle;
    u32 value;
    u32 unk8;
    void *sprite[3];
} MenuIconSprites;

void menuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value, s32 res, s32 *idx, s32 unused) {
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
    menuInitIconSprites(resource, first, second, third, fourth, fifth, sixth);
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

void func_002B9BB0(s32 arg0, s32 arg1, u32 arg2, s32 arg3, u32 arg4) {
    func_002B9A40(arg0 - 0xf0, arg1 - 8, arg2, *(u32 *)(arg3 + 0x94),
                                *(u32 *)(arg3 + 0x18), *(u32 *)(arg3 + 0x90), arg4);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9BE0);

void func_002B9CD8(u32 x, u32 y, u32 flags, u8 *entry, u32 option) {
    u8 *data = *(u8 **)(entry + 0x18);
    func_002B9BE0(x, y, flags, entry, *(u32 *)(data + 0xc), option);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9CF8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9DD8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9EA0);

void func_002B9FB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 0x20);
    if (0 < temp_v0) {
        do {
            temp_v0 = temp_v0 - 1;
        } while (temp_v0 != 0);
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

void menuHideIconGroup(SprGroup *group) {
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

s64 menuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 arg) {
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

s64 menuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 arg) {
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

s32 menuDrawIconPair(s32 x, s32 y, s32 z, s32 w, void **state, s32 arg) {
    MenuOffsets offset = D_0042AF90;
    func_00306CD0(x + offset.x0, y + offset.y0, z, w, 1, state[3], 0, arg);
    func_00306CD0(x + offset.x1, y + offset.y1, z, w, 1, state[4], 0, arg);
}

extern s64 menuDrawIconPanelFade();

extern s64 menuDrawIconRow6();

extern s32 menuDrawIconPair();

s64 menuDrawIconPanel(s32 a0, s32 a1, s32 a2, s32 a3, u32 *state, s32 a5, s32 a6) {
    switch (*state) {
    case 0: case 1: case 2: case 3:
        return menuDrawIconPanelFade(a0, a1, a2, a3, state, a5, a6);
    case 4:
        return menuDrawIconRow6(a0, a1, a2, 0x100, state, a6);
    case 5:
        return menuDrawIconPair(a0, a1, a2, a3, state, a6);
    }
}

void func_002BA7A8(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6) {
    menuDrawIconPanel(a0, a1, a2, a3, a4, a5, a6);
}

void func_002BA7C0(u32 a0, u32 a1, u32 a2, s32 arg3, s32 arg4) {
    func_002BA7A8(a0, a1, a2, 0x100, arg3, 0, arg4);
}

void func_002BA7E8(u32 obj) {
    switch (*(u32 *)obj) {
    case 4:
        itfGridLookupValueOrDefault(*(s32 *)(obj + 0x14), 0);
        itfGridLookupValueOrDefault(*(s32 *)(obj + 0x18), 0);
        itfGridLookupValueOrDefault(*(s32 *)(obj + 0x1C), 0);
        itfGridLookupValueOrDefault(*(s32 *)(obj + 0x20), 0);
        return;
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 5:
        itfGridLookupValueOrDefault(*(s32 *)(obj + 0xC), 0);
        itfGridLookupValueOrDefault(*(s32 *)(obj + 0x10), 0);
        break;
    }
}

void func_002BA890(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x5c);
    }
    *(s32 *)(arg0 + 0x10) = temp_v1;
}

void func_002BA8C8(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x58);
    }
    *(s32 *)(arg0 + 0x14) = temp_v1;
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

s32 mnuComparePrimaryKeyDescending(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 mnuComparePrimaryKeyAscending(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 mnuCompareSecondaryKeyDescending(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 mnuCompareSecondaryKeyAscending(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 mnuCompareTertiaryKeyDescending(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 mnuCompareTertiaryKeyAscending(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF48);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF60);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF90);

void mnuSortItems(s32 menu, s32 sortKey, s32 descending) {
    s32 (*comparators[6])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 count = 0;
    s32 handle = func_003292A8(((MenuList *)menu)->count * 4);
    MenuListNode **items = (MenuListNode **)sdfResourceRetainAddress(handle);
    MenuListNode **out = items;
    MenuListNode *node;

    for (node = ((MenuList *)menu)->first; node != NULL; node = node->next) {
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

void func_002BACF0(s32 arg0, s32 arg1, s32 *arg2) {
    u32 temp_v0 = *arg2;
    s32 *temp_v1 = arg2 + temp_v0;
    s32 *temp_v2;

    if (temp_v0 < 5) {
        return;
    }
    temp_v2 = (s32 *)temp_v1[1];
    *arg2 = temp_v0 + 1;
    temp_v2[0] = arg0;
    temp_v2[4] = arg1;
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
            func_002B9520(((MenuFadeEntry *)*slot)->handle);
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

void func_002BAF10(u8 *menu) {
    memset(menu, 0, 0x98);
    *(u32 *)(menu + 0xb4) = 0;
    *(u32 *)(menu + 0xb8) = 0x200;
    *(u32 *)(menu + 0xbc) = 0;
    *(u32 *)(menu + 0xc0) = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAF50);

void func_002BB0D0(u8 *menu) {
    *(u32 *)(menu + 0xbc) = 0;
    *(u32 *)(menu + 0xb8) = 0x200;
    *(u32 *)(menu + 0xc0) = 0;
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

void func_002BB418(u32 arg0) {
    func_002BB320();
    func_00328E48(arg0);
}

void func_002BB440(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x34);
    *(s32 *)(arg0 + 0x2c) = temp_v0;
    *(u32 *)(arg0 + 0x30) = *(u32 *)(arg0 + 0x38);
    *(u32 *)(arg0 + 0x34) = 0;
    if (temp_v0 != 0) {
        effConfigureWithDefaultSetting(temp_v0, *(u32 *)(arg0 + 0x38), *(u32 *)(arg0 + 0x44), 0, 10, 2);
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

u8 func_002BB500(s32 arg0) {
    return *(s32 *)(arg0 + 0x2c) != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB510);

void func_002BB850(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = effCreateResourceSlotSet(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe4) = temp_v0;
    temp_v0 = effCreateResourceSlotSet(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = effCreateResourceSlotSet(arg1, arg4, 1);
        *(u32 *)(arg0 + 0xec) = temp_v0;
    }
}

void func_002BB8D8(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x7c);
    puVar3 = (u32 *)(arg0 + 0x164);
    puVar4 = (u32 *)(arg0 + 0x160);
    temp_v1 = 0;
    do {
        if (puVar2[0x38] != 0) {
            func_003054E8(puVar2[0x38]);
        }
        if (puVar2[0x39] != 0) {
            func_003054E8(puVar2[0x39]);
        }
        if (puVar2[0x3a] != 0) {
            func_003054E8(puVar2[0x3a]);
        }
        temp_v0 = *puVar2;
        temp_v1 = temp_v1 + 1;
        puVar2[0x38] = 0;
        *puVar4 = 0;
        *puVar2 = temp_v0 & 0xffffffbf;
        puVar2 = puVar2 + 0x84e;
        *puVar3 = 0;
        puVar3 = puVar3 + 0x84e;
        puVar4 = puVar4 + 0x84e;
    } while (temp_v1 < 5);
}

void func_002BB998(u8 *menu, s32 index, u32 unused, u32 preserve) {
    u8 *entry = menu + index * 0x2138 + 0x78;
    *(u32 *)(entry + 0x64) = 0;
    *(u32 *)(entry + 0xb4) = 0;
    if (preserve == 0) {
        *(u32 *)(entry + 0x60) = 0x100;
        *(u32 *)(entry + 0xb0) = 0x100;
    }
}

void func_002BB9C8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
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

void menuFreeIconSprites(MenuSprites *menu) {
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

void menuDrawIconRow(s32 a0, s32 a1, s32 x, s32 skip, MenuIconSet *set, s32 arg) {
    u32 i;
    if (skip == 0) {
        for (i = 0; i < 5; i++) {
            func_00306CD0(0xBC0, 0x3C8, x, set->sprite, 0, set->icon[i], 0, arg);
        }
    }
}

extern void func_002BD1D0();

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

void menuSetWindowResource(s32 index, MenuWindows *menu, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6) {
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
            menuFreeIconSprites(*(u32 *)entry);
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

u32 menuCreateIconBundle(u32 resource) {
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

void menuDrawFadeIcons(s32 a0, s32 a1, s32 a2, s32 a3, MenuFadeIcons *obj, s32 a5) {
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

void func_002BC3C8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = menuCreateIconBundle(arg2);
    *(u32 *)(arg0 * 0x2138 + arg1 + 0x158) = temp_v0;
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

void func_002BC5D0(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x24);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

void func_002BC600(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x44);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

void mnuRegisterResourceHandles(s32 destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(source[i]);
        *(s32 *)(destination + 0x64 + 4 * i) = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC690);

void menuRefreshWindowSlots(u8 *menu, s32 flag) {
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

void func_002BCA98(u32 arg0) {
    menuRefreshWindowSlots(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCAB0);

extern char D_00437C30[];

void menuInitScrollLists(u8 *menu, s32 *counts) {
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

void func_002BCCB0(arg0)
    s32 arg0;
{
    func_002B81C8(*(u32 *)(arg0 + 0xa690));
    func_002B81C8(*(u32 *)(arg0 + 0xa694));
}

void func_002BCCF0(u32 arg0, u32 arg1) {
    func_002BCCB0();
    menuInitScrollLists(arg0, arg1);
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

void menuFreeWindowSprites(MenuWindowSprites *win) {
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
        menuFreeWindowSprites(slot);
    }
    func_002BC580(ctx);
    func_002BCCB0(ctx);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCFC8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD090);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD1D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD2E0);

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

void func_002BD3A8(s32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0;
    do {
        temp_v0 = temp_v0 + 1;
        *puVar1 = *puVar1 & 0xfffffffe;
        puVar1 = puVar1 + 0x84e;
    } while (temp_v0 < 5);
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

