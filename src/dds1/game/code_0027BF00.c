#include "common.h"

typedef struct MenuWindowContainer MenuWindowContainer;

typedef struct MenuList MenuList;

typedef struct MenuPanelHandles MenuPanelHandles;

extern void func_0027CA90();

extern void mnuReleasePageHandlesAndClearSelection();

extern s32 mnuGetSelectionFromFlags(s32);

extern void effReleaseTextureHandlesAndResetSlots(s32);

extern void itfSetGridEntryQuantizedAndRefresh(s32, s32, s32, s32, s32, s32);

extern void func_0027D850(s32, s32, s32, s32, MenuPanelHandles *, s32, s32);

extern void *func_0027F230(s32, s32, s32);

extern void func_0027DA80(s32, s32, s32, s32, MenuPanelHandles *, s32);

extern void func_0027DBD0(s32, s32, s32, s32, MenuPanelHandles *, s32);

extern void func_0027C140();

extern void mnuHideWindowHandles(MenuPanelHandles *);

extern void mnuDrawWindowSprites();

extern s32 ptyGetCurrentProfileId(s32);

extern s32 func_002CD240(s32, s32 *);

extern s32 func_00197760(s32, s32, s32, s32, s32, s32);

extern void func_00196088(s32, s32, s32);

extern void func_001958A0(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);

extern s32 datGameState;

typedef struct MenuListNode MenuListNode;

extern void func_00300508(MenuListNode **, s32, s32, s32 (*)(MenuListNode **, MenuListNode **));

extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);

/* Page records of a menu window, stride 0x134, starting at window + 0x20. */
typedef struct MenuPage {
    u8 pad0[0x58];
    s32 kind;          /* 0x58: active-page kind */
    u32 flags;         /* 0x5C */
    u8 pad60[0x60];
    s32 scaleA;        /* 0xC0 */
    s32 offsetA;       /* 0xC4 */
    u8 padC8[0x4C];
    s32 scaleB;        /* 0x114 */
    s32 offsetB;       /* 0x118 */
    u8 pad11C[0x18];
} MenuPage;

typedef struct MenuGauge {
    s32 id;
    u8 pad4[4];
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    u8 pad18[0xC];
} MenuGauge;

typedef struct MenuRecord {
    s32 visibleCount; /* 0x00: threshold for highlighted slots */
    u8 pad4[8];
    s32 partyIndex;
    MenuGauge gauge;
} MenuRecord;

typedef struct MenuWindow {
    u32 flags;
    u8 pad4[4];
    MenuRecord *records;
    s32 source;
    s32 slot;
    s32 field14;
    s32 field18;
    s32 field1C;
    s32 field20;
    s32 handlesA[8];
    s32 handlesB[8];
    s32 handlesC[5];
    u8 pad78[0x604];
    MenuList *lists[2]; /* 0x67C: parallel party-panel lists */
    s32 selected;
    s32 pad688;
    s32 fade;
} MenuWindow;

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

extern ScrollHandle *effCreateStatusBatch(s32);

extern void func_0027FCA0(s32, s32, s32);

extern void mnuDrawWindowContainer(s32, s32, s32, MenuWindowContainer *, s32);

extern void mnuReleaseSpriteTextures(s32);

extern void sdfReleaseChipBlock(void *);

extern u32 func_0027D4A0(u32);

extern u32 mnuCreateFadeSpriteResourceSet(u32);

extern s32 func_002CFEB8(u32);

extern u32 mnuCreateWindowState(u32, u32, u32, u32);

extern s32 func_0027B888(u32);

extern void func_002BF4E0(s32, s32, s32, s32, s32, s32, s32, s32);

extern char D_003B2330[];

extern char D_003B2348[];

extern void itfGridLookupValueOrDefault(s32, s32);

struct MenuList {
    u32 stateFlags;     /* 0x00: cursor and selection-control bits */
    u32 flags;
    s32 id;             /* 0x08: owner/list identifier */
    s32 visibleCount;
    MenuListNode *first;
    MenuListNode *last;
    MenuListNode *head;
    MenuListNode *cursor;
    s32 count;
    s32 windowOffset;
    s32 rowStep;        /* 0x28: constructor argument scaled by eight */
    u8 pad2C[0x10];
    s32 scale;          /* 0x3C: 8.8 fixed-point default */
};

struct MenuListNode {
    s32 index;
    s32 value;
    u8 pad8[0x40];
    u32 flags48;        /* 0x48 */
    u8 pad4C[4];
    s32 animationTimer; /* 0x50: stepped down to zero while a list is visible */
    u8 selectionByte54; /* 0x54: cleared on moving the list selection */
    u8 pad55[3];
    struct MenuListNode *next;
    struct MenuListNode *prev;
    u32 sortKeyPrimary;   /* 0x60 */
    u32 sortKeySecondary; /* 0x64 */
    u32 sortKeyTertiary;  /* 0x68 */
    u8 pad6C[8];
};

extern MenuListNode *sdfAllocAndClearQuadwords(s32);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;

typedef struct MenuSpriteGrid {
    s32 pad0[2];
    MenuSpriteRef slots[8];
} MenuSpriteGrid;

void mnuAdvanceListCursorDefault(u32 list);

void mnuClearListFlagsOneAndTwo(u32 *flags);

MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing);

u32 mnuDestroyListState(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuListAppendNode();

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);

s32 mnuSeekListNode(s32 index, MenuList *list);

u32 mnuTestListFlagTwo(u32 *flags);

s32 mnuGetListViewportHeight(s32 list) {
    return ((MenuList *)list)->rowStep * ((MenuList *)list)->visibleCount;
}

/* Cancel the pending animation on every node in this list. */
void mnuResetListNodeFadeCounters(MenuList *list) {
    MenuListNode *node;

    node = list->first;
    if (node != 0) {
        node->animationTimer = 0;
        while (node = node->next, node != 0) {
            node->animationTimer = 0;
        }
    }
}

/* Tick every node's animation down in units of 16, clamping at zero. */
void mnuDecreaseListNodeFadeCounters(MenuList *list) {
    MenuListNode *node = list->first;
    if (node != NULL) {
        do {
            s32 timer = node->animationTimer;
            s32 reduced = timer - 0x10;
            if (timer > 0) {
                node->animationTimer = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                node->animationTimer = 0;
            }
            node = node->next;
        } while (node != NULL);
    }
}

void mnuDrawFourEntries(s32 x, s32 y, s32 depth, s32 menu, s32 panel, s32 drawArg) {
    MenuSpriteGrid *grid = (MenuSpriteGrid *)panel;
    u32 i = 0;
    do {
        s32 selected = panel == (s32)((MenuList *)menu)->cursor;
        s32 index = selected * 4 + i;
        s32 sprite = grid->slots[index].sprite;
        if (sprite != 0) {
            itfDrawGridWithResolvedSlot(x, y, depth, 0, sprite, grid->slots[index].effect, drawArg);
        }
        i++;
    } while (i < 4);
}

s32 mnuDispatchByFlag(s32 value, s32 node) {
    return uiBlendColors((((MenuListNode *)node)->flags48 & 1) ? 0x89BDC940 : 0x89BDC980,
                         value, ((MenuListNode *)node)->animationTimer);
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

void mnuDispatchEntryWords(MenuSlotSet *menu, s32 index, s32 node) {
    s32 j;

    for (j = 0; j < 4; j++) {
        s32 word = menu->entries[index].word[j];

        menu->entries[index].word[j] = mnuDispatchByFlag(word, node);
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027C140);

void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 param) {
    func_0027C140(x, y, depth, 0, 0, 0x100, 0, menu, param);
}

/* Sprite work points to the state whose low flag is cleared on selection. */
typedef struct MenuPanelSpriteState {
    u8 pad00[0x28];
    u32 flags;
} MenuPanelSpriteState;

typedef struct MenuPanelSprite {
    u8 pad00[0x18];
    MenuPanelSpriteState *state;
} MenuPanelSprite;

/* The 0x38-byte panel is embedded at window +0x4C and copied as one layout. */
struct MenuPanelHandles {
    u32 mode;
    u8 pad04[4];
    s32 count;
    MenuPanelSprite *handles[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 transition;
};

/* One 0x8c-byte drawable window owns a list and its panel sprite handles. */
struct MenuWindowContainer {
    s32 id;             /* 0x00 */
    u32 flags;          /* 0x04 */
    s32 width;          /* 0x08 */
    s32 height;         /* 0x0c */
    u8 pad10[4];
    MenuList *list;     /* 0x14: owned list state */
    s32 entryValue;      /* 0x18 */
    s32 entryX;          /* 0x1C */
    s32 entryY;          /* 0x20 */
    s32 alternateEntryY; /* 0x24: entryY + 3 */
    s32 entryOption;     /* 0x28 */
    s32 x;              /* 0x2c */
    s32 y;              /* 0x30 */
    u32 sprite;         /* 0x34 */
    u32 effect;         /* 0x38 */
    u32 overlaySprite;  /* 0x3c */
    u32 overlayColor;   /* 0x40 */
    u8 pad44[8];
    MenuPanelHandles panel; /* 0x4C: embedded drawable panel layout */
    s32 textures;       /* 0x84 */
    s32 fade;           /* 0x88 */
};

s32 mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 left, s32 right) {
    MenuWindowContainer *item = (MenuWindowContainer *)sdfAllocAndClearQuadwords(0x8C);
    MenuList *child;
    item->width = width;
    item->height = height;
    item->id = id;
    child = mnuCreateListState(id, left, right);
    item->fade = 0;
    item->list = child;
    return (s32)item;
}

void mnuDestroyWindowContainer(MenuWindowContainer *window) {
    s32 textures;

    mnuDestroyListState(window->list);
    textures = window->textures;
    if (textures != 0) {
        mnuReleaseWindowTextures(textures);
    }
    sdfReleaseChipBlock(window);
}

void mnuSetWindowOverlaySprite(MenuWindowContainer *window, u32 sprite) {
    window->overlaySprite = sprite;
}

void mnuSetWindowContainerState(MenuWindowContainer *window, u32 fade) {
    window->fade = fade;
}

void mnuConfigureWindowSpriteAndGrid(MenuWindowContainer *window, s32 x, s32 y, u32 sprite,
                   u32 effect, u32 color) {
    window->x = x;
    window->y = y;
    itfSetGridEntryQuantizedAndRefresh(x, y, -0x70, -0x68, -0x70, -0x68);
    window->sprite = sprite;
    window->effect = effect;
    if (sprite != 0) {
        itfSetGridEntryQuantizedAndRefresh(sprite, effect, 0x60, -0xd0, 0, 0);
    }
    window->overlaySprite = sprite;
    window->overlayColor = color;
    if (sprite != 0) {
        itfSetGridEntryQuantizedAndRefresh(sprite, color, 0x60, -0xd0, 0, 0);
    }
}

void mnuForwardDupArg(MenuWindowContainer *panel, s32 x, s32 y, s32 sprite, s32 effect) {
    mnuConfigureWindowSpriteAndGrid(panel, x, y, sprite, effect, effect);
}

void mnuInitializeWindowEntryPlacement(s32 value, MenuWindowContainer *entry, s32 x, s32 y, s32 option) {
    entry->entryValue = value;
    entry->entryX = x;
    entry->alternateEntryY = y + 3;
    entry->entryY = y;
    entry->entryOption = option;
}

void mnuSetWindowPanelBounds(MenuWindowContainer *panel, const void *layout, u32 left, u32 top,
                   u32 right, u32 bottom) {
    memcpy(&panel->panel, layout, 0x38);
    panel->panel.left = left;
    panel->panel.top = top;
    panel->panel.right = right;
    panel->panel.bottom = bottom;
    panel->flags |= 4;
}

void mnuAttachWindowTextureState(MenuWindowContainer *panel, u32 source, u32 mode, u32 variant,
                                    u32 option) {
    u32 textures;

    textures = mnuCreateWindowState(source, mode, variant, option);
    panel->textures = textures;
}

void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *window) {
    window->flags = window->flags & 0xfffffffb;
}

void mnuAppendWindowListNode(MenuWindowContainer *window) {
    mnuListAppendNode(window->list);
}

void func_0027C688(MenuWindowContainer *window) {
    func_0027B540(window->list);
}

void func_0027C6A0(MenuWindowContainer *window) {
    func_0027B888(window->list);
}

MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *window, s32 direction) {
    MenuListNode *item = mnuListAdvanceCursor(window->list, direction, 0);
    if (item != 0) {
        item->selectionByte54 = 0;
        mnuClearEntryFlags(&window->panel);
    }
    return item;
}

MenuListNode *mnuReverseListSelection(MenuWindowContainer *window, s32 direction) {
    MenuListNode *item = mnuListRetreatCursor(window->list, direction, 0);
    if (item != 0) {
        item->selectionByte54 = 0;
        mnuClearEntryFlags(&window->panel);
    }
    return item;
}

void mnuAdvanceWindowListSelection(MenuWindowContainer *window) {
    mnuAdvanceListSelection(window, 0);
}

void mnuRetreatWindowListSelection(MenuWindowContainer *window) {
    mnuReverseListSelection(window, 0);
}

void func_0027C788(MenuWindowContainer *window) {
    mnuClearListFlagsOneAndTwo(window->list);
}

void func_0027C7A0(MenuWindowContainer *window) {
    mnuTestListFlagTwo(window->list);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027C7B8);

void func_0027CA78(s32 x, s32 y, s32 depth, s32 menu, s32 param) {
    func_0027C7B8();
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027CA90);

void mnuDrawWindowSelectionPanel(s32 x, s32 base, s32 depth, MenuWindowContainer *window, s32 param) {
    MenuList *node;
    MenuPanelHandles *panel;
    s32 flag;
    s32 texture = window->fade;

    if (window->panel.handles[0] != NULL) {
        if (window->flags & 4) {
            if (window->panel.transition == 0) {
                window->panel.transition = 0x200;
            } else if (window->panel.transition < 0x100) {
                window->panel.transition = 0x100;
            }
        } else if (window->panel.transition >= 0x101) {
            window->panel.transition = 0;
        }
        node = window->list;
        panel = &window->panel;
        flag = 0;
        if (node->stateFlags & 8) {
            flag = 1;
        }
        base += node->windowOffset * node->rowStep;
        mnuDrawIconPanel(x, base, depth, texture, panel, flag, param);
        mnuHideWindowHandles(panel);
        if (window->flags & 4) {
            window->panel.transition += 0x10;
            if (window->panel.transition >= 0x200) {
                window->panel.transition = 0x200;
            }
        } else {
            window->panel.transition += 0x20;
            if (window->panel.transition >= 0x101) {
                window->panel.transition = 0x100;
            }
        }
    }
}

/* Draw the window's list and panels, then advance its fade-in scale. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, MenuWindowContainer *menu, s32 param) {
    s32 texture = menu->fade;
    s32 count;

    menu->list->scale = texture;
    func_0027CA90();
    func_0027CA78(x, y, depth, menu, param);
    if (menu->list->count != 0) {
        mnuDrawWindowSelectionPanel(x, y, depth, menu, param);
    }
    func_0027C140(x, y, depth, menu->width, menu->height, texture, menu->flags,
                  menu->list, param);
    count = menu->textures;
    if (count != 0) {
        mnuDrawWindowSprites(x, y, depth, menu->list->flags, count, param);
    }
    count = menu->fade;
    if (count < 0x100) {
        menu->fade = count + 0x20;
    }
    menu->flags |= 4;
}

void func_0027CEE8(MenuWindowContainer *window) {
    s32 remaining;

    remaining = window->list->count;
    if (0 < remaining) {
        do {
            remaining = remaining - 1;
        } while (remaining != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027CF28);

extern s32 sdfAllocGeneralBlock(s32);

extern s32 *sdfResourceRetainAddress(s32);

extern void func_0027CF28(s32 *, u32, u32, u32, u32);

typedef struct MenuWindowSpriteGroup {
    s32 resourceHandle;
    u8 pad4[8];
    s32 sprites[7];
} MenuWindowSpriteGroup;

u32 mnuCreateWindowState(u32 source, u32 mode, u32 variant, u32 option) {
    s32 handle = sdfAllocGeneralBlock(sizeof(MenuWindowSpriteGroup));
    MenuWindowSpriteGroup *group = (MenuWindowSpriteGroup *)sdfResourceRetainAddress(handle);

    memset(group, 0, sizeof(MenuWindowSpriteGroup));
    group->resourceHandle = handle;
    func_0027CF28((s32 *)group, source, mode, variant, option);
    return (u32)group;
}

void mnuReleaseWindowTextures(MenuWindowSpriteGroup *group) {
    u32 i;
    for (i = 0; i < 7; i++) {
        effDestroyResourceSlotSet(group->sprites[i]);
    }
    sdfReleaseResourceAllocation(group->resourceHandle);
}

void mnuConfigureWindowSpriteSlots(MenuWindowSpriteGroup *group, u32 target) {
    effConfigureWithDefaultSetting(group->sprites[1], 0, target, 0, 0x14, 0xc);
    effConfigureWithDefaultSetting(group->sprites[2], 0, target, 1, 10, 0xc);
    effConfigureWithDefaultSetting(group->sprites[3], 0, target, 2, 0, 0xc);
    effConfigureWithDefaultSetting(group->sprites[4], 0, target, 2, 0, 0xc);
    effConfigureWithDefaultSetting(group->sprites[5], 0, target, 1, 10, 0xc);
    effConfigureWithDefaultSetting(group->sprites[6], 0, target, 0, 0x14, 0xc);
}

void mnuDrawWindowSprites(s32 x, s32 y, s32 z, s32 mask, MenuWindowSpriteGroup *group, s32 param) {
    itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[0], 0, param);
    if (mask & 1) {
        itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[1], 0, param);
        itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[2], 0, param);
        itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[3], 0, param);
    }
    if (mask & 2) {
        itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[4], 0, param);
        itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[5], 0, param);
        itfDrawGridWithResolvedSlot(x, y, z, 0, group->sprites[6], 0, param);
    }
    itfGridLookupValueOrDefault(group->sprites[1], 0);
    itfGridLookupValueOrDefault(group->sprites[2], 0);
    itfGridLookupValueOrDefault(group->sprites[3], 0);
    itfGridLookupValueOrDefault(group->sprites[4], 0);
    itfGridLookupValueOrDefault(group->sprites[5], 0);
    itfGridLookupValueOrDefault(group->sprites[6], 0);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027D4A0);

extern void effInitializeSlotWork(void *, s32);

void mnuClearEntryFlags(MenuPanelHandles *group) {
    s32 i;

    if (group->handles[0] != NULL && group->mode < 3) {
        for (i = 0; i < group->count; i++) {
            MenuPanelSprite *entry = group->handles[i];
            u32 *flags = &entry->state->flags;

            *flags &= ~1;
            effInitializeSlotWork(entry, 0);
        }
    }
}

void mnuReleaseResourceList(s32 *object) {
    s32 i;
    s32 count = object[2];
    for (i = 0; i < count; i++) {
        if (object[i + 3] != 0) {
            effDestroyResourceSlotSet(object[i + 3]);
            count = object[2];
        }
    }
    sdfReleaseChipBlock(object);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027D850);

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027DA80);

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027DBD0);

void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuPanelHandles *list, s32 flag, s32 drawArg) {
    switch (list->mode) {
    case 0:
        func_0027D850(x, y, depth, fade, list, flag, drawArg);
        return;
    case 1:
        func_0027DA80(x, y, depth, 0x100, list, drawArg);
        return;
    case 2:
        func_0027DBD0(x, y, depth, fade, list, drawArg);
        break;
    }
}

void mnuDrawIconPanelDefaultFlag(s32 x, s32 y, s32 depth, s32 fade, MenuPanelHandles *list, s32 drawArg) {
    mnuDrawIconPanel(x, y, depth, fade, list, 0, drawArg);
}

void mnuDrawIconPanelFullFade(s32 x, s32 y, s32 depth, MenuPanelHandles *list, s32 drawArg) {
    mnuDrawIconPanelDefaultFlag(x, y, depth, 0x100, list, drawArg);
}

void mnuHideWindowHandles(MenuPanelHandles *panel) {
    switch (panel->mode) {
    case 0:
        itfGridLookupValueOrDefault(panel->handles[4], 0);
        itfGridLookupValueOrDefault(panel->handles[5], 0);
        return;
    case 1:
        itfGridLookupValueOrDefault(panel->handles[0], 0);
        itfGridLookupValueOrDefault(panel->handles[1], 0);
        itfGridLookupValueOrDefault(panel->handles[2], 0);
        itfGridLookupValueOrDefault(panel->handles[3], 0);
        return;
    case 2:
        itfGridLookupValueOrDefault(panel->handles[2], 0);
        itfGridLookupValueOrDefault(panel->handles[3], 0);
        break;
    }
}

/* Rebuild the first-node pointer by walking backward from the cursor. */
void mnuRebuildListFirstFromCursor(MenuList *list) {
    MenuListNode *node;
    MenuListNode *first;
    MenuListNode *previous;

    previous = list->cursor;
    first = list->cursor;
    while (node = previous, node != 0) {
        first = node;
        previous = node->prev;
    }
    list->first = first;
}

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(MenuList *list) {
    MenuListNode *node;
    MenuListNode *last;
    MenuListNode *next;

    next = list->cursor;
    last = list->cursor;
    while (node = next, node != 0) {
        last = node;
        next = node->next;
    }
    list->last = last;
}

/* Reset the cursor to the beginning, optionally replaying its old position. */
void mnuResetNodeLinks(MenuList *list, s32 restoreOffset) {
    MenuListNode *first;
    MenuListNode *oldCursor;
    list->windowOffset = 0;
    first = list->first;
    oldCursor = list->cursor;
    list->head = first;
    list->cursor = first;
    if (restoreOffset == 1) {
        MenuListNode *node = first;
        if (node == NULL) {
            return;
        }
        do {
            if (node == oldCursor) {
                return;
            }
            mnuAdvanceListCursorDefault((u32)list);
            node = node->next;
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

s32 mnuComparePrimaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

s32 mnuComparePrimaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

s32 mnuCompareSecondaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

s32 mnuCompareSecondaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

s32 mnuCompareTertiaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

s32 mnuCompareTertiaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B2358);

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B2368);

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B2380);

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B23A0);

void mnuSortItems(s32 menu, s32 sortKey, s32 descending) {
    s32 (*comparators[6])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 count = 0;
    s32 handle = sdfAllocGeneralBlock(((MenuList *)menu)->count * 4);
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
    func_00300508(items, count, 4, comparators[sortKey]);
    mnuLinkItemList(items, count);
    mnuRebuildListFirstFromCursor(menu);
    mnuRebuildListLastFromCursor(menu);
    mnuResetNodeLinks((s32 *)menu, 0);
    sdfReleaseResourceAllocation(handle);
}

void mnuAllocateListEntries(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        list[i + 1] = sdfAllocAndClearQuadwords(0x18);
    }
}

void mnuFreeListEntries(s32 *list) {
    u32 i;
    s32 *entry = list + 1;
    for (i = 0; i < 4; i++, entry++) {
        sdfReleaseChipBlock((void *)*entry);
    }
}

void mnuAppendFadingWindowEntry(s32 x, s32 y, s32 *list) {
    u32 slotIndex = *list;
    s32 *slot = list + slotIndex;
    s32 *entry;

    if (slotIndex < 5) {
        return;
    }
    entry = (s32 *)slot[1];
    *list = slotIndex + 1;
    entry[0] = x;
    entry[4] = y;
}

typedef struct MenuFadeEntry {
    u32 active;
    u32 pad4[3];
    void *handle;
    u32 pad14;
} MenuFadeEntry;

void mnuRemoveFadingWindowEntry(s32 *list, u32 index) {
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

void mnuDrawFadingWindows(s32 image, s32 *list, s32 option) {
    u32 i;
    for (i = 0; i < (u32)list[0]; i++) {
        s32 *entry = (s32 *)list[i + 1];
        mnuDrawWindowContainer(entry[2], entry[3], image, entry[4], option);
    }
}

void mnuUpdateFade(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        s32 *entry = (s32 *)list[i + 1];
        if (entry[5] != 0) {
            entry[5] -= 0x40;
        } else {
            mnuRemoveFadingWindowEntry(list, i);
        }
    }
}

/* 0x4C-byte scroll panel with three linked animation handles. */
typedef struct MenuScrollPanel {
    u8 pad00[4];
    u32 selection;            /* 0x04 */
    u32 firstSprite;          /* 0x08 */
    u32 secondSprite;         /* 0x0C */
    MenuSpriteRef positions[2]; /* 0x10: leading value pairs */
    MenuSpriteRef active[2];    /* 0x20 */
    MenuSpriteRef pending[2];   /* 0x30 */
    ScrollHandle *handles[3]; /* 0x40 */
} MenuScrollPanel;

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

void mnuReleaseScrollPanelAnimations(MenuScrollPanel *list) {
    u32 i;
    for (i = 0; i < 3; i++) {
        effDestroyPackedBatch(list->handles[i]);
    }
}

MenuScrollPanel *mnuCreateScrollPanel(u32 startX, u32 startY, u32 endX, u32 endY) {
    MenuScrollPanel *panel;
    s32 position;
    s32 remaining;

    panel = (MenuScrollPanel *)func_002CFEB8(0x4c);
    memset(panel, 0, 0x4c);
    panel->firstSprite = 0;
    panel->secondSprite = 0;
    remaining = 1;
    itfGridStorePosition(&panel->positions[0], startX, startY);
    itfGridStorePosition(&panel->positions[1], endX, endY);
    position = (s32)panel;
    do {
        remaining = remaining - 1;
        itfGridStorePosition(position + 0x20, 0, 0);
        itfGridStorePosition(position + 0x30, 0, 0);
        position = position + 8;
    } while (-1 < remaining);
    mnuInitScrollHandles(panel);
    return panel;
}

void mnuDestroyScrollPanel(MenuScrollPanel *panel) {
    mnuReleaseScrollPanelAnimations(panel);
    sdfReleaseChipBlock(panel);
}

void mnuStoreScrollPanelSelectionAndGridPosition(MenuScrollPanel *panel, u32 unused0, u32 unused1, u32 selection) {
    itfGridStorePosition(&panel->positions[0]);
    panel->selection = selection;
}

void mnuActivatePendingPanelResource(MenuScrollPanel *menu) {
    s32 *source = &menu->active[0].effect;
    s32 *dest = &menu->pending[0].sprite;
    s32 remaining = 1;
    do {
        u32 previous = *dest;
        u32 next = source[4];
        dest[-4] = previous;
        *source = next;
        *dest = 0;
        source += 2;
        dest += 2;
    } while (--remaining >= 0);
    if (menu->active[0].sprite != 0) {
        itfSetGridEntryQuantizedAndRefresh(menu->active[0].sprite, menu->active[0].effect, 0, 0, 0x400, 0);
        effConfigureWithDefaultSetting(menu->active[0].sprite, menu->active[0].effect, menu->handles[2],
                                          0, 10, 2);
    }
}

void mnuActivatePanelAndConfigureGridResources(MenuScrollPanel *menu, s32 x, s32 y, s32 color) {
    mnuActivatePendingPanelResource(menu);
    menu->pending[0].sprite = x;
    menu->pending[0].effect = y;
    menu->pending[1].sprite = x;
    menu->pending[1].effect = color;
    itfSetGridEntryQuantizedAndRefresh(x, y, 0, 0, -0x400, 0);
    effConfigureIndexedSlotResource(x, y, menu->handles[0], 0, 3);
    itfSetGridEntryQuantizedAndRefresh(x, color, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(x, color, menu->handles[1], 0, 10, 0);
}

u8 mnuHasScrollPanelOverlay(MenuScrollPanel *panel) {
    return panel->active[0].sprite != 0;
}

void mnuDrawPanelGridAndSubmitSurface(u8 *panel, s32 y, s32 unknown,
                   u32 *sprite, s32 flag) {
    uiDrawActiveSurfaceRegion(flag);
    itfDrawGridWithResolvedSlot((s32)(panel + 0x10), y + 0xf8, 0xffffff, 1,
                   sprite[6], sprite[7], flag);
    sdfDispatchSurfaceWithPreparedTexturePacket(flag);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027E8D8);

/* Ten resource handles occupy offsets 0x0c through 0x30 in each page bundle. */
typedef struct MenuPageResources {
    u8 pad0[0xC];
    s32 sprites[10]; /* 0x0c */
    u8 pad34[8];
} MenuPageResources;

/* Create a ten-sprite page bundle; the caller chooses its last two slots. */
MenuPageResources *mnuCreatePartyPageSpriteBundle(s32 mainResource, s32 secondaryResource, s32 extraResource, s32 extraIndex,
                     s32 finalResource, s32 finalIndex) {
    MenuPageResources *item = (MenuPageResources *)func_002CFEB8(0x3C);

    memset(item, 0, 0x3C);
    item->sprites[0] = effCreateResourceSlotSet(secondaryResource, 0, 1);
    item->sprites[1] = effCreateResourceSlotSet(secondaryResource, 1, 1);
    item->sprites[2] = effCreateResourceSlotSet(mainResource, 2, 1);
    item->sprites[3] = effCreateResourceSlotSet(mainResource, 5, 1);
    item->sprites[4] = effCreateResourceSlotSet(mainResource, 6, 1);
    item->sprites[5] = effCreateResourceSlotSet(mainResource, 7, 1);
    item->sprites[6] = effCreateResourceSlotSet(mainResource, 8, 1);
    item->sprites[7] = effCreateResourceSlotSet(mainResource, 0xA, 1);
    item->sprites[9] = effCreateResourceSlotSet(extraResource, extraIndex, 1);
    item->sprites[8] = effCreateResourceSlotSet(finalResource, finalIndex, 1);
    return item;
}

/* Keep the original word walk: structured indexing exceeds the retail body. */
void mnuDestroyResources(s32 *object) {
    u32 i;
    for (i = 0; i < 2; i++) {
        effDestroyResourceSlotSet(object[i + 3]);
    }
    for (i = 0; i < 6; i++) {
        effDestroyResourceSlotSet(object[i + 5]);
    }
    effDestroyResourceSlotSet(object[12]);
    effDestroyResourceSlotSet(object[11]);
    sdfReleaseChipBlock(object);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027ECD8);

void mnuCreatePartyPageResources(MenuWindow *menu, s32 x, s32 y, s32 style, s32 color) {
    MenuPageResources **output = (MenuPageResources **)((u8 *)menu + 0x154);
    u32 i = 0;
    s32 offset = 0;
    for (; i < 5; i++) {
        s32 entry = ((MenuRecord *)((u8 *)menu->records + offset))->gauge.id;
        offset += 0x34;
        if (entry >= 0) {
            *output = mnuCreatePartyPageSpriteBundle(x, y, style, 0, color, entry);
        }
        output = (MenuPageResources **)((u8 *)output + 0x134);
    }
}

void mnuReleaseSlotResources(MenuWindow *context) {
    MenuPageResources **slot = (MenuPageResources **)((u8 *)context + 0x154);
    u32 i = 0;
    s32 offset = 0;
    for (; i < 5; i++) {
        s32 node = ((MenuRecord *)((u8 *)context->records + offset))->gauge.id;
        offset += 0x34;
        if (node >= 0 && *slot != 0) {
            mnuDestroyResources((s32 *)*slot);
            *slot = 0;
        }
        slot = (MenuPageResources **)((u8 *)slot + 0x134);
    }
}

typedef struct MenuPanelResources {
    u8 pad00[0x10];
    s32 primary;            /* 0x10 */
    u8 pad14[0xD4];
    u32 leftHandle;          /* 0xE8 */
    u32 centerHandle;        /* 0xEC */
    u32 rightHandle;         /* 0xF0 */
} MenuPanelResources;

void mnuLoadPanelSectionResources(MenuPanelResources *panel, u32 resource, u32 left, u32 center, s32 right
                                    ) {
    u32 handle;

    handle = effCreateResourceSlotSet(resource, left, 1);
    panel->leftHandle = handle;
    handle = effCreateResourceSlotSet(resource, center, 1);
    panel->centerHandle = handle;
    if (-1 < right) {
        handle = effCreateResourceSlotSet(resource, right, 1);
        panel->rightHandle = handle;
    }
}

void mnuReleasePartyPanelTextures(s32 menu) {
    u32 flags;
    s32 *resource;
    u32 *pageFlags;
    u32 *pageHandle;
    u32 index;

    resource = (s32 *)(menu + 0x168);
    pageFlags = (u32 *)(menu + 0x7c);
    pageHandle = (u32 *)(menu + 0x164);
    index = 0;
    do {
        if (resource[-2] != 0) {
            effDestroyResourceSlotSet(resource[-2]);
        }
        if (resource[-1] != 0) {
            effDestroyResourceSlotSet(resource[-1]);
        }
        if (*resource != 0) {
            effDestroyResourceSlotSet(*resource);
        }
        flags = *pageFlags;
        index = index + 1;
        resource[-2] = 0;
        *pageHandle = 0;
        *pageFlags = flags & 0xffffff7f;
        pageFlags = pageFlags + 0x4d;
        *resource = 0;
        resource = resource + 0x4d;
        pageHandle = pageHandle + 0x4d;
    } while (index < 5);
}

void mnuResetPartyPanelFade(s32 menu, s32 index, s32 unused, s32 retainScale) {
    s32 page = menu + index * 0x134 + 0x78;

    ((MenuPage *)(page - 0x58))->offsetA = 0;
    ((MenuPage *)(page - 0x58))->offsetB = 0;
    if (retainScale != 0) {
        return;
    }
    ((MenuPage *)(page - 0x58))->scaleA = 0x100;
    ((MenuPage *)(page - 0x58))->scaleB = 0x100;
}

typedef struct MenuPageMotion {
    u8 unk0[0x40];
    s32 field40;
    s32 field44;
    s32 field48;
    s32 field4C;
} MenuPageMotion;

void mnuSetPageParams(MenuPageMotion *page, s32 mode) {
    switch (mode) {
    case 0:
        page->field44 = 0;
        page->field40 = 0;
        page->field48 = 0x40;
        page->field4C = 0x100;
        break;
    case 1:
        page->field44 = 1;
        page->field40 = 0x100;
        page->field48 = 0;
        page->field4C = 0x1000;
        break;
    default:
        page->field44 = 0;
        page->field40 = 0x100;
        page->field48 = 0;
        page->field4C = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027F230);

void mnuFreeIconSprites(u32 *menu) {
    u32 *entry;
    u32 *base;
    u32 i;
    entry = menu + 4;
    for (i = 0; i < 2; i++) {
        effDestroyResourceSlotSet(*entry++);
    }
    i = 0;
    base = menu + 2;
    entry = base + 4;
    for (; i < 4; i++) {
        effDestroyResourceSlotSet(*entry++);
    }
    entry = base + 8;
    for (i = 0; i < 3; i++) {
        effDestroyResourceSlotSet(*entry++);
    }
    i = 0;
    entry = menu + 13;
    for (; i < 2; i++) {
        effDestroyResourceSlotSet(*entry++);
    }
    sdfReleaseChipBlock(menu);
}

void mnuDrawIconSpriteGroup(s32 unusedX, s32 unusedY, s32 depth, s32 skip, s32 *menu, s32 param) {
    u32 i;

    if (skip == 0) {
        func_002BF4E0(0, 0, depth, menu[15], 0, menu[4], 0, param);
        for (i = 0; i < 4; i++) {
            func_002BF4E0(0, 0, depth, menu[15], 0, menu[6 + i], 0, param);
        }
    }
}

void mnuSetWindowResource(s32 index, s32 window, s32 resource, s32 option) {
    mnuSelectPage((MenuWindow *)window, index);
    *(void **)(index * 0x134 + window + 0x158) = func_0027F230(0, resource, option);
    *(u32 *)window |= 0x100;
}

void mnuClearEntries(s32 *menu) {
    u32 i;
    s32 *entry = menu + 0x56;
    mnuReleasePageHandlesAndClearSelection();
    for (i = 0; i < 5; i++, entry += 0x4D) {
        if (*entry != 0) {
            mnuFreeIconSprites(*entry);
            *entry = 0;
        }
    }
    *menu &= ~0x100;
}

typedef struct MenuFadeSpriteSet {
    u8 pad0[0xC];
    s32 sprites[4];
    s32 alpha;
    s32 fadeOut;
} MenuFadeSpriteSet;

u32 mnuCreateFadeSpriteResourceSet(u32 resource) {
    MenuFadeSpriteSet *item = (MenuFadeSpriteSet *)func_002CFEB8(0x24);
    s32 sprite;

    memset(item, 0, 0x24);
    sprite = effCreateResourceSlotSet(resource, 0x1F, 1);
    item->sprites[0] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0, 0x40, 0, 0);
    sprite = effCreateResourceSlotSet(resource, 0x1E, 1);
    item->sprites[1] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0x5E0, -0x20, 0, 0);
    sprite = effCreateResourceSlotSet(resource, 7, 1);
    item->sprites[2] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0x5E0, -0x18, 0, 0);
    sprite = effCreateResourceSlotSet(resource, 0x1D, 1);
    item->sprites[3] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0xB40, 0x40, 0, 0);
    return (u32)item;
}

void mnuReleaseFourResourceList(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        effDestroyResourceSlotSet(list[i + 3]);
    }
    sdfReleaseChipBlock(list);
}

void mnuDrawAndUpdateFadingSprites(s32 x, s32 y, s32 z, s32 unused, MenuFadeSpriteSet *sprites, s32 param) {
    s32 px = x + 0x120;
    s32 py = y + 0x30;
    s32 alpha = sprites->alpha;
    s32 fade;
    s32 nextAlpha;
    s32 lowerAlpha;

    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[0], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[1], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[2], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[3], 0, param);
    if (sprites->fadeOut == 0) {
        fade = sprites->alpha;
        nextAlpha = fade + 0x10;
        if (fade < 0x100) {
            sprites->alpha = nextAlpha;
            fade = nextAlpha;
        }
        if (fade >= 0x101) {
            sprites->alpha = 0x100;
        }
    } else {
        fade = sprites->alpha;
        lowerAlpha = fade - 0x10;
        if (fade > 0) {
            sprites->alpha = lowerAlpha;
            fade = lowerAlpha;
        }
        if (fade < 0) {
            sprites->alpha = 0;
        }
    }
}

/* Icon resource stored at the start of each 0x134-byte party panel slot. */
typedef struct MenuPartyIconSlot {
    u32 bundle;
    u8 pad04[0x130];
} MenuPartyIconSlot;

void mnuAttachPartyIconBundle(s32 index, s32 window, u32 resource) {
    u32 sprites;

    sprites = mnuCreateFadeSpriteResourceSet(resource);
    ((MenuPartyIconSlot *)(window + 0x15c))[index].bundle = sprites;
}

void mnuReleasePartyIconBundles(s32 window) {
    u32 i;
    for (i = 0; i < 5; i++) {
        MenuPartyIconSlot *item = &((MenuPartyIconSlot *)(window + 0x15c))[i];
        if (item->bundle != 0) {
            mnuReleaseFourResourceList(item->bundle);
            item->bundle = 0;
        }
    }
}

s32 mnuPercentOrHundred(s32 value, s32 total) {
    if (total > 0) {
        return value * 100 / total;
    }
    return 100;
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027FAA8);

void mnuReleasePartyPanelSpriteTextures(s32 window) {
    u32 i;
    for (i = 0; i < 5; i++, window += 0x134) {
        mnuReleaseSpriteTextures(window + 0x94);
        mnuReleaseSpriteTextures(window + 0xe8);
    }
}

/* Copy eight resource handles into the window's primary handle bank. */
void mnuCopyPrimaryWindowHandles(MenuWindow *window, u32 *source) {
    u32 value;
    s32 *destination;
    u32 index;

    destination = window->handlesA;
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
void mnuCopySecondaryWindowHandles(MenuWindow *window, u32 *source) {
    u32 value;
    s32 *destination;
    u32 index;

    destination = window->handlesB;
    index = 0;
    do {
        value = *source;
        source = source + 1;
        index = index + 1;
        *destination = value;
        destination = destination + 1;
    } while (index < 8);
}

extern void effResolveAndReleaseResource(s32);

void mnuRegisterResourceHandles(MenuWindow *destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(source[i]);
        destination->handlesC[i] = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027FCA0);

void mnuUpdateHandleStates(MenuWindow *obj) {
    s32 *handle = obj->handlesA;
    s32 i;
    s32 offset;

    for (i = 0; i < 8U; i++, handle++) {
        if (effHasFirstTextureHandle(*handle) != 0) {
            effReleaseTextureHandlesAndResetSlots(*handle);
            effReleaseTextureHandlesAndResetSlots(handle[8]);
        }
    }
    for (i = 0, offset = 0; i < 5U; i++) {
        MenuRecord *entry = (MenuRecord *)((u8 *)obj->records + offset);

        offset += 0x34;
        if (entry->gauge.id >= 0) {
            if (i < obj->records->visibleCount) {
                func_0027FCA0(obj, i, 1);
            } else {
                func_0027FCA0(obj, i, 2);
            }
        } else {
            func_0027FCA0(obj, i, 0);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00280048);

extern char D_003BC728[];

void mnuFillPanelLists(MenuWindow *window, s32 *counts) {
    s32 i = 0;

    window->lists[0] = mnuCreateListState(0, 1, 1);
    window->lists[1] = mnuCreateListState(0, 1, 1);
    for (i = 0; i < counts[0] + counts[1]; i++) {
        mnuListAppendNode(window->lists[0], D_003BC728);
        mnuListAppendNode(window->lists[1], D_003BC728);
    }
}

void mnuDestroyWindowOwnedLists(MenuWindow *window) {
    mnuDestroyListState(window->lists[0]);
    mnuDestroyListState(window->lists[1]);
}

void mnuRebuildScrollLists(MenuWindow *menu, s32 *counts) {
    mnuDestroyWindowOwnedLists(menu);
    mnuFillPanelLists(menu, counts);
}

void mnuClearPageSelection(MenuWindow *window) {
    if (window->selected >= 0) {
        MenuPage *pages = (MenuPage *)((u8 *)window + 0x20);

        pages[window->selected].scaleA = 0x100;
        pages[window->selected].scaleB = 0x100;
        window->selected = -1;
    }
    window->flags &= ~0x400;
}

INCLUDE_ASM(const s32, "game/code_0027BF00", mnuInitPageWindow);

/* Resource handles within a 0x134-byte page at context + 0x78. */
typedef struct MenuPageHandles {
    u8 pad0[0x10];
    s32 base[3];     /* 0x10 */
    u8 pad1C[0xA8];
    s32 overlays[6]; /* 0xC4 */
    u8 padDC[0x58];
} MenuPageHandles;

void mnuReleaseHandles(s32 obj) {
    MenuPageHandles *page = (MenuPageHandles *)obj;
    s32 *handle = page->base;
    u32 i;

    for (i = 0; i < 3; i++, handle++) {
        if (*handle != 0) {
            effDestroyResourceSlotSet(*handle);
        }
    }
    if (page->overlays[0] != 0) {
        effDestroyResourceSlotSet(page->overlays[0]);
    }
    if (page->overlays[1] != 0) {
        effDestroyResourceSlotSet(page->overlays[1]);
    }
    if (page->overlays[2] != 0) {
        effDestroyResourceSlotSet(page->overlays[2]);
    }
    if (page->overlays[3] != 0) {
        effDestroyResourceSlotSet(page->overlays[3]);
    }
    if (page->overlays[4] != 0) {
        effDestroyResourceSlotSet(page->overlays[4]);
    }
    if (page->overlays[5] != 0) {
        effDestroyResourceSlotSet(page->overlays[5]);
    }
}

extern void mnuReleaseHandles(s32);

void mnuShutdownContext(s32 context) {
    u32 i;
    for (i = 0; i < 5; i++) {
        mnuReleaseHandles(context + 0x78 + i * 0x134);
    }
    mnuReleasePartyPanelSpriteTextures(context);
    mnuDestroyWindowOwnedLists(context);
}

void mnuResolveUnselectedPageHandles(MenuWindow *window) {
    s32 selected = window->selected;
    s32 offset = 0;
    u32 i;

    for (i = 0; i < 5; i++, offset += sizeof(MenuRecord)) {
        if (i != selected) {
            s32 id = ((MenuRecord *)((u8 *)window->records + offset))->gauge.id;

            if (id >= 0) {
                if (effHasFirstTextureHandle(window->handlesA[id]) == 0) {
                    effResolveAndReleaseResource(window->handlesA[id]);
                    effResolveAndReleaseResource(window->handlesB[id]);
                }
            }
        }
    }
}

void mnuReleasePageTexturesAndSelectedResources(MenuWindow *window) {
    s32 selected = window->selected;
    u32 i;
    s32 id;
    MenuRecord *record;

    for (i = 0; i < 5; i++) {
        record = &window->records[i];
        id = record->gauge.id;
        if (id >= 0) {
            if (effHasFirstTextureHandle(window->handlesA[id]) != 0) {
                effReleaseTextureHandlesAndResetSlots(window->handlesA[id]);
                effReleaseTextureHandlesAndResetSlots(window->handlesB[id]);
            }
        }
    }
    record = &window->records[selected];
    id = record->gauge.id;
    if (id >= 0) {
        if (effHasFirstTextureHandle(window->handlesA[id]) == 0) {
            effResolveAndReleaseResource(window->handlesA[id]);
            effResolveAndReleaseResource(window->handlesB[id]);
        }
    }
}

void mnuSelectPage(MenuWindow *window, s32 selected) {
    s32 *resource = window->handlesC;
    u32 i;
    /* Required to match: retain the header-relative handle walk below. */
    u8 *handles = window->pad4;
    MenuRecord *record;
    s32 active;

    for (i = 0; i < 5; i++) {
        effReleaseTextureHandlesAndResetSlots(*resource++);
    }
    record = &window->records[selected];
    active = mnuGetSelectionFromFlags(datGameState + record->partyIndex * 0x1A4 + 0xA60);
    for (i = 0; i < 5; i++) {
        if (i == active) {
            effResolveAndReleaseResource(*(s32 *)(handles + 0x60 + i * 4));
        }
    }
    if (window->selected >= 0) {
        mnuResolveUnselectedPageHandles(window);
    }
    window->selected = selected;
    mnuReleasePageTexturesAndSelectedResources(window);
}

void mnuReleasePageHandlesAndClearSelection(window)
    MenuWindow *window;
{
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
    u8 *flags = menu + 12;
    u32 i = 0;
    s32 activeKind = 2;
    s32 offset = 0x70;
    do {
        if (*(s32 *)(kind + offset) == activeKind) {
            *(u32 *)(flags + offset) |= 1;
        }
        i++;
        offset += 0x134;
    } while (i < 5);
}

void mnuClearPartyPanelActiveFlags(s32 menu) {
    u32 *flags;
    u32 index;

    flags = (u32 *)(menu + 0x7c);
    index = 0;
    do {
        index = index + 1;
        *flags = *flags & 0xfffffffe;
        flags = flags + 0x4d;
    } while (index < 5);
}

void mnuClearListFlags(s32 which, MenuWindow *menu) {
    mnuSeekListNode(0, menu->lists[which]);
    if (which == 0) {
        menu->flags &= ~2;
        menu->flags &= ~4;
        menu->flags &= ~8;
        menu->flags &= ~0x10;
        menu->flags &= ~0x20;
    } else {
        menu->flags &= ~2;
        menu->flags &= ~8;
        menu->flags &= ~0x10;
        menu->flags &= ~0x20;
    }
}

extern u32 mnuMapPadMaskToFlags(u32);

extern void mnuRetreatListCursorDefault(u32 list);
extern void mnuPlayInputSound(s32, u32, s32);

/* Step the selected party-panel list from the pad: left/right move its cursor, any input restarts the fade. */
void mnuUpdateWindowListFromInput(s32 mode, MenuWindow *window) {
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
        mnuPlayInputSound(0, input, (s32)list);
    }
    if (window->fade > 0) {
        s32 fade = window->fade - 0x10;

        window->fade = fade < 0 ? 0 : fade;
    }
    mnuClearPageSelection(window);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00280A90);

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00280BC0);

s32 mnuClearWindowPendingFlagAfterSelection(s32 unusedX, s32 unusedY, s32 unusedDepth, u32 *window, s32 option) {
    s32 result = func_00280A90(window, option);

    switch (result) {
    case 1:
        *window &= ~2;
        return 1;
    case 2:
        *window &= ~2;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00280E08);

extern void func_002CD0D8(u32 textId, s32 arg1, char *out);

extern s32 func_001951C8(char *text, s32, s32, s32, s32);

extern s32 frFontMeasureGlyphChain(s32 item);

extern void frFontSetContextPair(s32 item, s32 x, s32 y);

void mnuDrawCenteredLabel(s32 x, s32 y, s32 unused, s32 color, s32 textId, s32 param) {
    char text[0x40];
    s32 item;
    s32 width;

    func_002CD0D8(textId & 0xFFFF, 1, text);
    item = func_001951C8(text, 0, 0, 0, 0);
    frFontSetChildColors(item, color);
    width = frFontMeasureGlyphChain(item) + 8;
    frFontSetContextPair(item, x - (width * 0x10 >> 1) + 0x5F0, y);
    func_001958A0(item, 1, param);
    frFontQueueGlyphInSelectedSlot(item);
}

void mnuDrawSelectedPartyProfileLabel(s32 unusedX, s32 unusedY, s32 depth, s32 fade, s32 selectedCode, s32 unused,
                     s32 partyIndex, s32 param) {
    s32 outValue;
    s32 cost = ptyGetCurrentProfileId(datGameState + partyIndex * 0x1A4 + 0xA60);
    s32 code;
    s32 texture;
    s32 item;

    texture = uiBlendColors(0xA09DC380, 0xA09DC300, fade);
    code = selectedCode != 0 ? selectedCode : cost;
    if (code != 0) {
        if (func_002CD240(code & 0xFFFF, &outValue) != 0) {
            mnuDrawCenteredLabel(0x1120, 0x5F0, depth, texture, code, param);
            return;
        }
        item = func_00197760(0, 0, depth, texture, outValue, 0);
        func_00196088(0x1710, 0x5F0, item);
        func_001958A0(item, 1, param);
        frFontQueueGlyphInSelectedSlot(item);
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_002812E8);

INCLUDE_SDATA(const s32, "game/code_0027BF00", D_003BC718);

INCLUDE_SDATA(const s32, "game/code_0027BF00", D_003BC720);

INCLUDE_SDATA(const s32, "game/code_0027BF00", D_003BC728);

INCLUDE_SDATA(const s32, "game/code_0027BF00", D_003BC730);

INCLUDE_SDATA(const s32, "game/code_0027BF00", D_003BC738);

INCLUDE_SDATA(const s32, "game/code_0027BF00", D_003BC740);

void mnuDrawPartyPanelResourceIcons(s32 x, s32 y, s32 z, s32 obj, s32 mode, s32 param) {
    s32 pos[2] = {0x180, 0xE0};

    if (mode == 0) {
        func_002BF4E0(x, y, z, 0x100, 1, ((MenuPanelResources *)obj)->primary, 0, param);
        x += pos[0];
        y += pos[1];
        func_002BF4E0(x, y, z, 0x100, 1, ((MenuPanelResources *)obj)->leftHandle, 0, param);
        if (((MenuPanelResources *)obj)->rightHandle == 0) {
            func_002BF4E0(x + 0x360, y + 0x68, z, 0x100, 1, ((MenuPanelResources *)obj)->centerHandle, 0, param);
        } else {
            func_002BF4E0(x + 0x360, y + 0x68, z, 0x100, 1, ((MenuPanelResources *)obj)->centerHandle, 0, param);
            func_002BF4E0(x + 0x790, y + 0x70, z, 0x100, 1, ((MenuPanelResources *)obj)->rightHandle, 0, param);
        }
    }
}

/* Each panel owns the color state blended with the neighboring panel. */
typedef struct MenuBlendOwner {
    u8 pad00[0x18];
    s32 colorState;
} MenuBlendOwner;

typedef struct MenuBlendColors {
    u8 pad00[0x14];
    s32 output[4]; /* 0x14 */
    u8 pad24[0x60];
    s32 input[4];  /* 0x84 */
} MenuBlendColors;

void mnuBlendPanelSlots(s32 dst, s32 src, u32 amount) {
    s32 i;
    s32 ctx = ((MenuBlendOwner *)dst)->colorState;

    for (i = 0; i < 4; i++) {
        s32 result = uiBlendColors(((MenuBlendColors *)ctx)->input[i],
                                   ((MenuBlendColors *)((MenuBlendOwner *)src)->colorState)->input[i],
                                   (s32)amount / 2 + 0x80, ctx);
        s32 current = ((MenuBlendOwner *)dst)->colorState;
        ctx = current;
        ((MenuBlendColors *)current)->output[i] = result;
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00281688);

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00281780);

void mnuClearPanelWorkState(u32 panel) {
    memset(panel, 0, 0x20);
}
