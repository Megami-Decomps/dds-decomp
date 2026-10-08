#include "mnu.h"
#include "mnu_list.h"
#include "eff.h"
#include "mnu_shop.h"
#include "dat_state.h"
#include "dat_command.h"
struct MenuListNode;
struct FrFontGlyph;
struct FrFontCtx;
struct TextStyleNode;

enum MenuPanelKind {
    MNU_PANEL_KIND_SIX_SLOTS = 0,
    MNU_PANEL_KIND_FOUR_OFFSET_ICONS = 1,
    MNU_PANEL_KIND_FIXED_ICON_PAIRS = 2,
    MNU_PANEL_KIND_COUNT = 3
};

#define MNU_ENTRY_SPRITE_COUNT 4
#define MNU_ENTRY_COLOR_COUNT 4
#define MNU_ENTRY_MARKED_COLOR 0x89BDC940
#define MNU_ENTRY_DEFAULT_COLOR 0x89BDC980
#define MNU_NODE_FADE_STEP 0x10
#define MNU_FULL_FADE 0x100
#define MNU_PANEL_FADE_LIMIT 0x200
#define MNU_PANEL_FADE_THRESHOLD 0x101
#define MNU_WINDOW_FADE_STEP 0x20
#define MNU_WINDOW_TRANSITION_FLAG 4
#define MNU_WINDOW_TRANSITION_CLEAR_MASK 0xFFFFFFFB
#define MNU_LIST_SELECTION_FLAG 8
#define MNU_WINDOW_CONTAINER_BYTES 0x8C
#define MNU_PANEL_LAYOUT_BYTES 0x38
#define MNU_WINDOW_RESOURCE_SPRITES 7
#define MNU_SORT_KEY_COUNT 3
#define MNU_SORT_COMPARATOR_COUNT 6
#define MNU_LIST_POINTER_BYTES 4
#define MNU_FADING_SLOT_COUNT 4
#define MNU_FADING_SLOT_BYTES 0x18
#define MNU_FADING_MUTATION_THRESHOLD 5
#define MNU_FADING_WORD_STEP 0x40


typedef struct MenuList MenuList;


extern void func_0027CA90();

extern void mnuReleasePageHandlesAndClearSelection();

extern void effReleaseTextureHandlesAndResetSlots(s32);

extern void itfSetGridEntryQuantizedAndRefresh(s32, s32, s32, s32, s32, s32);

extern void func_0027D850(s32, s32, s32, s32, MenuPanelHandles *, s32, s32);

extern void *func_0027F230(s32, s32, s32);

extern void mnuDrawFourPanelIconsAtOffsets(s32, s32, s32, s32, MenuPanelHandles *, s32);

extern void mnuDrawPanelIconPairsAtFixedPositions(s32, s32, s32, s32, MenuPanelHandles *, s32);

extern void func_0027C140();

extern void mnuUpdateWindowPanelHandleStates(MenuPanelHandles *);

extern void mnuDrawWindowSprites();

extern s32 ptyGetCurrentProfileId(DatPartyRecord *);

extern s32 func_002CD240(s32, s32 *);

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern void func_00196088(s32, s32, s32);

extern s32 func_001958A0(struct FrFontGlyph *, s8, u32);

extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);


typedef struct MenuListNode MenuListNode;

extern void func_00300508(MenuListNode **, s32, s32, s32 (*)(MenuListNode **, MenuListNode **));

extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);




extern void mnuSelectPage(MenuPageWindow *window, s32 selected);

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

extern MenuPanelHandles *mnuCreatePanelSpriteHandles(u32, s32, s32);

extern u32 mnuCreateFadeSpriteResourceSet(u32);

extern s32 sdfAllocSizeClassBlock(u32);

extern struct MenuWindowSpriteGroup *mnuCreateWindowState(u32, u32, u32, u32);

extern s32 func_0027B888(u32);

extern void func_002BF4E0(s32, s32, s32, s32, s32, s32, s32, s32);

extern char D_003B2330[];

extern char D_003B2348[];

extern void itfGridLookupValueOrDefault(s32, s32);


extern MenuListNode *sdfAllocAndClearQuadwords(s32);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;



void mnuClearListFlagsOneAndTwo(u32 *flags);

MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing);

u32 mnuDestroyListState(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);


MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);

s32 mnuSeekListNode(s32 index, MenuList *list);

u32 mnuTestListFlagTwo(u32 *flags);

/* Return the stored row step times the visible row count, in native units. */
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

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(MenuList *list) {
    MenuListNode *node = list->first;
    if (node != NULL) {
        do {
            s32 timer = node->animationTimer;
            s32 reduced = timer - MNU_NODE_FADE_STEP;
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

/* Blend the flag-selected packed color with the caller's previous color.
 * DDS1 has two color choices; DDS2 additionally checks flag four. */
s32 mnuDispatchByFlag(s32 previousColor, s32 entry) {
    return uiBlendColors((((MenuListNode *)entry)->flags48 & 1) ? MNU_ENTRY_MARKED_COLOR : MNU_ENTRY_DEFAULT_COLOR,
                         previousColor, ((MenuListNode *)entry)->animationTimer);
}



/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 index, s32 entry) {
    s32 colorIndex;

    for (colorIndex = 0; colorIndex < MNU_ENTRY_COLOR_COUNT; colorIndex++) {
        s32 previousColor = menu->workEntries[index].geometry.cornerColors[colorIndex];

        menu->workEntries[index].geometry.cornerColors[colorIndex] = mnuDispatchByFlag(previousColor, entry);
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027C140);

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg) {
    func_0027C140(x, y, depth, 0, 0, MNU_FULL_FADE, 0, menu, drawArg);
}



typedef struct MenuPanelPosition {
    s32 x;
    s32 y;
} MenuPanelPosition;

typedef struct MenuPanelPositionTable4 {
    MenuPanelPosition positions[4];
} MenuPanelPositionTable4;

typedef struct MenuPanelPositionTable2 {
    MenuPanelPosition positions[2];
} MenuPanelPositionTable2;

extern MenuPanelPositionTable4 D_003B2380;

extern MenuPanelPositionTable2 D_003B23A0;


/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
s32 mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 visibleCount, s32 rowSpacing) {
    MenuWindowContainer *window = (MenuWindowContainer *)sdfAllocAndClearQuadwords(MNU_WINDOW_CONTAINER_BYTES);
    MenuList *list;
    window->width = width;
    window->height = height;
    window->id = id;
    list = mnuCreateListState(id, visibleCount, rowSpacing);
    window->fade = 0;
    window->list = list;
    return (s32)window;
}

/* Destroy the owned list and optional sprite resources before freeing the window. */
void mnuDestroyWindowContainer(MenuWindowContainer *window) {
    struct MenuWindowSpriteGroup *textures;

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

void mnuAttachWindowTextureState(MenuWindowContainer *panel, u32 source, u32 mode, u32 variant,
                                    u32 option) {
    struct MenuWindowSpriteGroup *textures;

    textures = mnuCreateWindowState(source, mode, variant, option);
    panel->textures = textures;
}

/* Clear only the window's panel-transition bit. */
void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *window) {
    window->flags = window->flags & MNU_WINDOW_TRANSITION_CLEAR_MASK;
}

MenuListNode *mnuAppendWindowListNode(MenuWindowContainer *window, const void *value) {
    return mnuListAppendNode(window->list, value);
}


MenuListNode *func_0027C688(MenuWindowContainer *window, MenuListNode *anchor, const void *value, s32 mode, u32 options) {
    return func_0027B540(window->list, anchor, value, mode, options);
}

void mnuRemoveWindowListCursorNode(MenuWindowContainer *window) {
    func_0027B888(window->list);
}

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *window, s32 direction) {
    MenuListNode *selected = mnuListAdvanceCursor(window->list, direction, 0);
    if (selected != 0) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&window->panel);
    }
    return selected;
}

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
MenuListNode *mnuReverseListSelection(MenuWindowContainer *window, s32 direction) {
    MenuListNode *selected = mnuListRetreatCursor(window->list, direction, 0);
    if (selected != 0) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&window->panel);
    }
    return selected;
}

void mnuAdvanceWindowListSelection(MenuWindowContainer *window) {
    mnuAdvanceListSelection(window, 0);
}

void mnuRetreatWindowListSelection(MenuWindowContainer *window) {
    mnuReverseListSelection(window, 0);
}

void func_0027C788(MenuWindowContainer *window) {
    mnuClearListFlagsOneAndTwo(&window->list->stateFlags);
}

void func_0027C7A0(MenuWindowContainer *window) {
    mnuTestListFlagTwo(&window->list->stateFlags);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027C7B8);

void func_0027CA78(s32 x, s32 y, s32 depth, s32 menu, s32 param) {
    func_0027C7B8();
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027CA90);

/* Draw at the selected row before advancing the panel's transition value.
 * DDS1 also normalizes the transition range before drawing; DDS2 does not. */
void mnuDrawWindowSelectionPanel(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 drawArg) {
    MenuList *list;
    MenuPanelHandles *panel;
    s32 selectionMode;
    s32 fadeScale = window->fade;

    if (window->panel.handles[0] != NULL) {
        if (window->flags & MNU_WINDOW_TRANSITION_FLAG) {
            if (window->panel.transition == 0) {
                window->panel.transition = MNU_PANEL_FADE_LIMIT;
            } else if (window->panel.transition < MNU_FULL_FADE) {
                window->panel.transition = MNU_FULL_FADE;
            }
        } else if (window->panel.transition >= MNU_PANEL_FADE_THRESHOLD) {
            window->panel.transition = 0;
        }
        list = window->list;
        panel = &window->panel;
        selectionMode = 0;
        if (list->stateFlags & MNU_LIST_SELECTION_FLAG) {
            selectionMode = 1;
        }
        y += list->windowOffset * list->rowStep;
        mnuDrawIconPanel(x, y, depth, fadeScale, panel, selectionMode, drawArg);
        mnuUpdateWindowPanelHandleStates(panel);
        if (window->flags & MNU_WINDOW_TRANSITION_FLAG) {
            window->panel.transition += MNU_NODE_FADE_STEP;
            if (window->panel.transition >= MNU_PANEL_FADE_LIMIT) {
                window->panel.transition = MNU_PANEL_FADE_LIMIT;
            }
        } else {
            window->panel.transition += MNU_WINDOW_FADE_STEP;
            if (window->panel.transition >= MNU_PANEL_FADE_THRESHOLD) {
                window->panel.transition = MNU_FULL_FADE;
            }
        }
    }
}

/* Draw the window, then advance its fade scale without a post-addition clamp. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, MenuWindowContainer *menu, s32 drawArg) {
    s32 fadeScale = menu->fade;
    s32 value;
    struct MenuWindowSpriteGroup *textures;

    menu->list->scale = fadeScale;
    func_0027CA90();
    func_0027CA78(x, y, depth, menu, drawArg);
    if (menu->list->count != 0) {
        mnuDrawWindowSelectionPanel(x, y, depth, menu, drawArg);
    }
    func_0027C140(x, y, depth, menu->width, menu->height, fadeScale, menu->flags,
                  menu->list, drawArg);
    textures = menu->textures;
    if (textures != NULL) {
        mnuDrawWindowSprites(x, y, depth, menu->list->flags, textures, drawArg);
    }
    value = menu->fade;
    if (value < MNU_FULL_FADE) {
        menu->fade = value + MNU_WINDOW_FADE_STEP;
    }
    menu->flags |= MNU_WINDOW_TRANSITION_FLAG;
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

extern void func_0027CF28(struct MenuWindowSpriteGroup *, u32, u32, u32, u32);

typedef struct MenuWindowSpriteGroup {
    s32 resourceHandle;
    u8 pad4[8];
    s32 sprites[7];
} MenuWindowSpriteGroup;

/* Allocate/clear the native seven-sprite resource group before its initializer runs. */
MenuWindowSpriteGroup *mnuCreateWindowState(u32 source, u32 mode, u32 variant, u32 option) {
    s32 allocationHandle = sdfAllocGeneralBlock(sizeof(MenuWindowSpriteGroup));
    MenuWindowSpriteGroup *group = (MenuWindowSpriteGroup *)sdfResourceRetainAddress(allocationHandle);

    memset(group, 0, sizeof(MenuWindowSpriteGroup));
    group->resourceHandle = allocationHandle;
    func_0027CF28(group, source, mode, variant, option);
    return group;
}

/* Destroy every native sprite slot, then release the group's allocation handle. */
void mnuReleaseWindowTextures(MenuWindowSpriteGroup *group) {
    u32 spriteIndex;
    for (spriteIndex = 0; spriteIndex < MNU_WINDOW_RESOURCE_SPRITES; spriteIndex++) {
        effDestroyResourceSlotSet(group->sprites[spriteIndex]);
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

/* Always draw slot zero; masks one/two select the two three-slot banks.
 * Reset the six auxiliary slots through the existing grid lookup after drawing. */
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

typedef struct MenuPanelSlotIndices {
    s32 slots[6];
} MenuPanelSlotIndices;

extern MenuPanelSlotIndices D_003B2368;
extern u32 *effCreateResourceSlotSet(u32 *, u32, u32);
extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);
extern void effConfigureIndexedSlotMaterial(s32, s32, s32, s32, s32, s32, s32);

/* Panel kind chooses the native sprite-slot layout. */
MenuPanelHandles *mnuCreatePanelSpriteHandles(u32 panelKind, s32 resource, s32 target) {
    MenuPanelSlotIndices indices = D_003B2368;
    MenuPanelHandles *panel = (MenuPanelHandles *)sdfAllocAndClearQuadwords(sizeof(MenuPanelHandles));
    s32 i;

    panel->panelKind = panelKind;
    switch (panelKind) {
    case MNU_PANEL_KIND_SIX_SLOTS:
        panel->count = 6;
        for (i = 0; i < panel->count; i++) {
            panel->handles[i] = (EffectSlotSet *)effCreateResourceSlotSet((u32 *)resource, indices.slots[i], 1);
        }
        effConfigureWithDefaultSetting((s32)panel->handles[4], 0, target, 0, 0, 12);
        effConfigureWithDefaultSetting((s32)panel->handles[5], 0, target, 0, 0, 12);
        break;
    case MNU_PANEL_KIND_FOUR_OFFSET_ICONS:
        panel->count = 4;
        panel->handles[0] = (EffectSlotSet *)effCreateResourceSlotSet((u32 *)resource, 23, 1);
        panel->handles[1] = (EffectSlotSet *)effCreateResourceSlotSet((u32 *)resource, 23, 1);
        panel->handles[2] = (EffectSlotSet *)effCreateResourceSlotSet((u32 *)resource, 22, 1);
        panel->handles[3] = (EffectSlotSet *)effCreateResourceSlotSet((u32 *)resource, 22, 1);
        effConfigureIndexedSlotMaterial((s32)panel->handles[0], 0, target, 1, 10, 10, 12);
        effConfigureIndexedSlotMaterial((s32)panel->handles[1], 0, target, 1, 0, 10, 12);
        effConfigureIndexedSlotMaterial((s32)panel->handles[2], 0, target, 1, 0, 10, 12);
        effConfigureIndexedSlotMaterial((s32)panel->handles[3], 0, target, 1, 10, 10, 12);
        break;
    case MNU_PANEL_KIND_FIXED_ICON_PAIRS:
        panel->count = 4;
        for (i = 0; i < panel->count; i++) {
            panel->handles[i] = (EffectSlotSet *)effCreateResourceSlotSet((u32 *)resource, indices.slots[i + 2], 1);
        }
        effConfigureWithDefaultSetting((s32)panel->handles[2], 0, target, 0, 0, 12);
        effConfigureWithDefaultSetting((s32)panel->handles[3], 0, target, 0, 0, 12);
        break;
    }
    return panel;
}

extern void effInitializeSlotWork(s32, s32);

/* Reset low sprite flags only for a present first handle and a supported panel kind. */
void mnuClearEntryFlags(MenuPanelHandles *group) {
    s32 spriteIndex;

    if (group->handles[0] != NULL && group->panelKind < MNU_PANEL_KIND_COUNT) {
        for (spriteIndex = 0; spriteIndex < group->count; spriteIndex++) {
            EffectSlotSet *entry = group->handles[spriteIndex];
            u32 *flags = &entry->workEntries->states[0].flags;

            *flags &= ~1;
            effInitializeSlotWork((s32)entry, 0);
        }
    }
}

/* Destroy nonzero slots, reloading the native count after each destruction, then free. */
void mnuReleaseResourceList(MenuPanelHandles *panel) {
    s32 slotIndex;
    s32 count = panel->count;

    for (slotIndex = 0; slotIndex < count; slotIndex++) {
        if (panel->handles[slotIndex] != NULL) {
            effDestroyResourceSlotSet((s32)panel->handles[slotIndex]);
            count = panel->count;
        }
    }
    sdfReleaseChipBlock(panel);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027D850);

/* Draw the four row icons at their table-owned offsets. */
void mnuDrawFourPanelIconsAtOffsets(s32 x, s32 y, s32 depth, s32 alpha, MenuPanelHandles *panel, s32 drawArg) {
    MenuPanelPositionTable4 table = D_003B2380;

    func_002BF4E0(x + table.positions[0].x, y + table.positions[0].y, depth, alpha, 1,
                  (s32)panel->handles[0], 0, drawArg);
    func_002BF4E0(x + table.positions[1].x, y + table.positions[1].y, depth, alpha, 1,
                  (s32)panel->handles[1], 0, drawArg);
    func_002BF4E0(x + table.positions[2].x, y + table.positions[2].y, depth, alpha, 1,
                  (s32)panel->handles[2], 0, drawArg);
    func_002BF4E0(x + table.positions[3].x, y + table.positions[3].y, depth, alpha, 1,
                  (s32)panel->handles[3], 0, drawArg);
}

/* Draw two stacked icon pairs; the table already contains absolute screen positions. */
void mnuDrawPanelIconPairsAtFixedPositions(s32 x, s32 y, s32 depth, s32 alpha, MenuPanelHandles *panel, s32 drawArg) {
    MenuPanelPositionTable2 table = D_003B23A0;
    s32 positionX = table.positions[0].x;
    s32 positionY = table.positions[0].y;

    func_002BF4E0(positionX, positionY, depth, alpha, 1, (s32)panel->handles[1], 0, drawArg);
    func_002BF4E0(positionX, positionY, depth, alpha, 1, (s32)panel->handles[3], 0, drawArg);
    positionX = table.positions[1].x;
    positionY = table.positions[1].y;
    func_002BF4E0(positionX, positionY, depth, alpha, 1, (s32)panel->handles[0], 0, drawArg);
    func_002BF4E0(positionX, positionY, depth, alpha, 1, (s32)panel->handles[2], 0, drawArg);
}

/* Dispatch the three DDS1 panel kinds; only kind one forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuPanelHandles *panel, s32 selectionMode, s32 drawArg) {
    switch (panel->panelKind) {
    case MNU_PANEL_KIND_SIX_SLOTS:
        func_0027D850(x, y, depth, fade, panel, selectionMode, drawArg);
        return;
    case MNU_PANEL_KIND_FOUR_OFFSET_ICONS:
        mnuDrawFourPanelIconsAtOffsets(x, y, depth, MNU_FULL_FADE, panel, drawArg);
        return;
    case MNU_PANEL_KIND_FIXED_ICON_PAIRS:
        mnuDrawPanelIconPairsAtFixedPositions(x, y, depth, fade, panel, drawArg);
        break;
    }
}

void mnuDrawIconPanelDefaultFlag(s32 x, s32 y, s32 depth, s32 fade, MenuPanelHandles *list, s32 drawArg) {
    mnuDrawIconPanel(x, y, depth, fade, list, 0, drawArg);
}

void mnuDrawIconPanelFullFade(s32 x, s32 y, s32 depth, MenuPanelHandles *list, s32 drawArg) {
    mnuDrawIconPanelDefaultFlag(x, y, depth, MNU_FULL_FADE, list, drawArg);
}

void mnuUpdateWindowPanelHandleStates(MenuPanelHandles *panel) {
    switch (panel->panelKind) {
    case MNU_PANEL_KIND_SIX_SLOTS:
        itfGridLookupValueOrDefault(panel->handles[4], 0);
        itfGridLookupValueOrDefault(panel->handles[5], 0);
        return;
    case MNU_PANEL_KIND_FOUR_OFFSET_ICONS:
        itfGridLookupValueOrDefault(panel->handles[0], 0);
        itfGridLookupValueOrDefault(panel->handles[1], 0);
        itfGridLookupValueOrDefault(panel->handles[2], 0);
        itfGridLookupValueOrDefault(panel->handles[3], 0);
        return;
    case MNU_PANEL_KIND_FIXED_ICON_PAIRS:
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

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(MenuList *list, s32 restoreCursor) {
    MenuListNode *first;
    MenuListNode *oldCursor;
    list->windowOffset = 0;
    first = list->first;
    oldCursor = list->cursor;
    list->head = first;
    list->cursor = first;
    if (restoreCursor == 1) {
        MenuListNode *node = first;
        if (node == NULL) {
            return;
        }
        do {
            if (node == oldCursor) {
                return;
            }
            mnuAdvanceListCursorDefault(list);
            node = node->next;
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
s32 mnuComparePrimaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* Three-way comparison of unsigned primary keys, ascending without subtraction. */
s32 mnuComparePrimaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* Three-way comparison of unsigned secondary keys, descending without subtraction. */
s32 mnuCompareSecondaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* Three-way comparison of unsigned secondary keys, ascending without subtraction. */
s32 mnuCompareSecondaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* Three-way comparison of unsigned tertiary keys, descending without subtraction. */
s32 mnuCompareTertiaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

/* Three-way comparison of unsigned tertiary keys, ascending without subtraction. */
s32 mnuCompareTertiaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

/* Sort the walked node pointers and rebuild the list from the cursor.
 * Nonzero ascending selects the last three comparators, not descending order.
 * Allocation uses the stored count; key bounds and the relinker's minimum count remain unchecked. */
INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B2358);

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B2368);

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B2380);

INCLUDE_RODATA(const s32, "game/code_0027BF00", D_003B23A0);

void mnuSortItems(s32 menu, s32 keyIndex, s32 ascending) {
    s32 (*comparators[MNU_SORT_COMPARATOR_COUNT])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 nodeCount = 0;
    s32 allocationHandle = sdfAllocGeneralBlock(((MenuList *)menu)->count * MNU_LIST_POINTER_BYTES);
    MenuListNode **items = (MenuListNode **)sdfResourceRetainAddress(allocationHandle);
    MenuListNode **writeCursor = items;
    MenuListNode *node;

    for (node = ((MenuList *)menu)->first; node != NULL; node = node->next) {
        *writeCursor++ = node;
        nodeCount++;
    }
    if (ascending != 0) {
        keyIndex += MNU_SORT_KEY_COUNT;
    }
    func_00300508(items, nodeCount, MNU_LIST_POINTER_BYTES, comparators[keyIndex]);
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

/* Free the four record blocks, not their nested window pointers. */
void mnuFreeListEntries(s32 *list) {
    u32 slotIndex;
    s32 *entry = list + 1;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++, entry++) {
        sdfReleaseChipBlock((void *)*entry);
    }
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

    panel = (MenuScrollPanel *)sdfAllocSizeClassBlock(0x4c);
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
    MenuPageResources *item = (MenuPageResources *)sdfAllocSizeClassBlock(0x3C);

    memset(item, 0, 0x3C);
    item->sprites[0] = effCreateResourceSlotSet((u32 *)secondaryResource, 0, 1);
    item->sprites[1] = effCreateResourceSlotSet((u32 *)secondaryResource, 1, 1);
    item->sprites[2] = effCreateResourceSlotSet((u32 *)mainResource, 2, 1);
    item->sprites[3] = effCreateResourceSlotSet((u32 *)mainResource, 5, 1);
    item->sprites[4] = effCreateResourceSlotSet((u32 *)mainResource, 6, 1);
    item->sprites[5] = effCreateResourceSlotSet((u32 *)mainResource, 7, 1);
    item->sprites[6] = effCreateResourceSlotSet((u32 *)mainResource, 8, 1);
    item->sprites[7] = effCreateResourceSlotSet((u32 *)mainResource, 0xA, 1);
    item->sprites[9] = effCreateResourceSlotSet((u32 *)extraResource, extraIndex, 1);
    item->sprites[8] = effCreateResourceSlotSet((u32 *)finalResource, finalIndex, 1);
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

void mnuCreatePartyPageResources(MenuPageWindow *menu, s32 x, s32 y, s32 style, s32 color) {
    u32 i;
    for (i = 0; i < 5; i++) {
        s32 entry = menu->records->slots[i].unk8;
        if (entry >= 0) {
            menu->slots[i].resources = mnuCreatePartyPageSpriteBundle(x, y, style, 0, color, entry);
        }
    }
}

void mnuReleaseSlotResources(MenuPageWindow *context) {
    u32 i;
    for (i = 0; i < 5; i++) {
        s32 node = context->records->slots[i].unk8;
        if (node >= 0 && context->slots[i].resources != 0) {
            mnuDestroyResources((s32 *)context->slots[i].resources);
            context->slots[i].resources = 0;
        }
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

    handle = effCreateResourceSlotSet((u32 *)resource, left, 1);
    panel->leftHandle = handle;
    handle = effCreateResourceSlotSet((u32 *)resource, center, 1);
    panel->centerHandle = handle;
    if (-1 < right) {
        handle = effCreateResourceSlotSet((u32 *)resource, right, 1);
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
    MenuPageSlot *page = &((MenuPageWindow *)menu)->slots[index];

    page->offsetA = 0;
    page->offsetB = 0;
    if (retainScale != 0) {
        return;
    }
    page->scaleA = 0x100;
    page->scaleB = 0x100;
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

void *func_0027F230(s32 value, s32 mainResource, s32 secondaryResource) {
    MenuSprites *page = (MenuSprites *)sdfAllocSizeClassBlock(0x50);

    memset(page, 0, 0x50);
    page->unkC = value;
    page->firstSprite = effCreateResourceSlotSet((u32 *)secondaryResource, 6, 1);
    itfSetGridEntryQuantizedAndRefresh(page->firstSprite, 0, 0xE60, 0x430, 0, 0);
    page->sprites[0] = effCreateResourceSlotSet((u32 *)secondaryResource, 8, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[0], 0, 0xF50, 0x648, 0, 0);
    page->sprites[1] = effCreateResourceSlotSet((u32 *)mainResource, 0x3B, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[1], 0, 0x1C20, 0x6D0, 0, 0);
    page->sprites[2] = effCreateResourceSlotSet((u32 *)secondaryResource, 9, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[2], 0, 0x1120, 0x708, 0, 0);
    page->sprites[3] = effCreateResourceSlotSet((u32 *)secondaryResource, 0xA, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[3], 0, 0x1BB0, 0x708, 0, 0);
    page->sprites[4] = effCreateResourceSlotSet((u32 *)secondaryResource, 0xD, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[4], 0, 0x1450, 0x640, 0, 0);
    page->sprites[5] = effCreateResourceSlotSet((u32 *)secondaryResource, 0xB, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[5], 0, 0x1540, 0x5E0, 0, 0);
    page->sprites[6] = effCreateResourceSlotSet((u32 *)secondaryResource, 0xC, 1);
    itfSetGridEntryQuantizedAndRefresh(page->sprites[6], 0, 0x1980, 0x660, 0, 0);
    page->primarySprite = effCreateResourceSlotSet((u32 *)mainResource, 0x3C, 1);
    itfSetGridEntryQuantizedAndRefresh(page->primarySprite, 0, 0x110, 0x280, 0, 0);
    page->overlaySprites[0] = effCreateResourceSlotSet((u32 *)mainResource, 0x35, 1);
    itfSetGridEntryQuantizedAndRefresh(page->overlaySprites[0], 0, 0x8E0, 0x288, 0, 0);
    page->overlaySprites[1] = effCreateResourceSlotSet((u32 *)mainResource, 0x36, 1);
    itfSetGridEntryQuantizedAndRefresh(page->overlaySprites[1], 0, 0xEE0, 0x288, 0, 0);
    mnuSetPageParams(page, 0);
    return page;
}

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

void mnuDrawIconSpriteGroup(s32 unusedX, s32 unusedY, s32 depth, s32 skip, MenuSprites *menu, s32 param) {
    u32 i;

    if (skip == 0) {
        func_002BF4E0(0, 0, depth, menu->profileFade, 0, menu->firstSprite, 0, param);
        for (i = 0; i < 4; i++) {
            func_002BF4E0(0, 0, depth, menu->profileFade, 0, menu->sprites[i], 0, param);
        }
    }
}

void mnuSetWindowResource(s32 index, s32 window, s32 resource, s32 option) {
    mnuSelectPage((MenuPageWindow *)window, index);
    ((MenuPageWindow *)window)->slots[index].windowSprites = func_0027F230(0, resource, option);
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

u32 mnuCreateFadeSpriteResourceSet(u32 resource) {
    MenuIconBundle *item = (MenuIconBundle *)sdfAllocSizeClassBlock(0x24);
    s32 sprite;

    memset(item, 0, 0x24);
    sprite = effCreateResourceSlotSet((u32 *)resource, 0x1F, 1);
    item->sprite[0] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0, 0x40, 0, 0);
    sprite = effCreateResourceSlotSet((u32 *)resource, 0x1E, 1);
    item->sprite[1] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0x5E0, -0x20, 0, 0);
    sprite = effCreateResourceSlotSet((u32 *)resource, 7, 1);
    item->sprite[2] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0x5E0, -0x18, 0, 0);
    sprite = effCreateResourceSlotSet((u32 *)resource, 0x1D, 1);
    item->sprite[3] = sprite;
    itfSetGridEntryQuantizedAndRefresh(sprite, 0, 0xB40, 0x40, 0, 0);
    return (u32)item;
}

void mnuReleaseFourResourceList(MenuIconBundle *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        effDestroyResourceSlotSet(list->sprite[i]);
    }
    sdfReleaseChipBlock(list);
}

void mnuDrawAndUpdateFadingSprites(s32 x, s32 y, s32 z, s32 unused, MenuIconBundle *sprites, s32 param) {
    s32 px = x + 0x120;
    s32 py = y + 0x30;
    s32 alpha = sprites->fade;
    s32 fade;
    s32 nextAlpha;
    s32 lowerAlpha;

    func_002BF4E0(px, py, z, alpha, 0, sprites->sprite[0], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprite[1], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprite[2], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprite[3], 0, param);
    if (sprites->fadeOut == 0) {
        fade = sprites->fade;
        nextAlpha = fade + 0x10;
        if (fade < 0x100) {
            sprites->fade = nextAlpha;
            fade = nextAlpha;
        }
        if (fade >= 0x101) {
            sprites->fade = 0x100;
        }
    } else {
        fade = sprites->fade;
        lowerAlpha = fade - 0x10;
        if (fade > 0) {
            sprites->fade = lowerAlpha;
            fade = lowerAlpha;
        }
        if (fade < 0) {
            sprites->fade = 0;
        }
    }
}


void mnuAttachPartyIconBundle(s32 index, s32 window, u32 resource) {
    u32 sprites;

    sprites = mnuCreateFadeSpriteResourceSet(resource);
    ((MenuPageWindow *)window)->slots[index].iconBundle = sprites;
}

void mnuReleasePartyIconBundles(s32 window) {
    u32 i;
    for (i = 0; i < 5; i++) {
        if (((MenuPageWindow *)window)->slots[i].iconBundle != 0) {
            mnuReleaseFourResourceList((MenuIconBundle *)((MenuPageWindow *)window)->slots[i].iconBundle);
            ((MenuPageWindow *)window)->slots[i].iconBundle = 0;
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
void mnuCopyPrimaryWindowHandles(MenuPageWindow *window, u32 *source) {
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
void mnuCopySecondaryWindowHandles(MenuPageWindow *window, u32 *source) {
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

void mnuRegisterResourceHandles(MenuPageWindow *destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(source[i]);
        destination->handlesC[i] = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_0027FCA0);

void mnuUpdateHandleStates(MenuPageWindow *obj) {
    s32 *handle = obj->handlesA;
    s32 i;

    for (i = 0; i < 8U; i++, handle++) {
        if (effHasFirstTextureHandle(*handle) != 0) {
            effReleaseTextureHandlesAndResetSlots(*handle);
            effReleaseTextureHandlesAndResetSlots(handle[8]);
        }
    }
    for (i = 0; i < 5U; i++) {
        PartyPanelEntry *entry = &obj->records->slots[i];

        if (entry->unk8 >= 0) {
            if (i < obj->records->unk0) {
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

void mnuFillPanelLists(MenuPageWindow *window, s32 *counts) {
    s32 i = 0;

    window->lists[0] = mnuCreateListState(0, 1, 1);
    window->lists[1] = mnuCreateListState(0, 1, 1);
    for (i = 0; i < counts[0] + counts[1]; i++) {
        mnuListAppendNode(window->lists[0], D_003BC728);
        mnuListAppendNode(window->lists[1], D_003BC728);
    }
}

void mnuDestroyWindowOwnedLists(MenuPageWindow *window) {
    mnuDestroyListState(window->lists[0]);
    mnuDestroyListState(window->lists[1]);
}

void mnuRebuildScrollLists(MenuPageWindow *menu, s32 *counts) {
    mnuDestroyWindowOwnedLists(menu);
    mnuFillPanelLists(menu, counts);
}

extern void mnuClearPageSelection(MenuPageWindow *);

/* The resource and panel bank begins at the window's +0x20 word. */
typedef struct MenuWindowResourceBank {
    s32 unk20;
    s32 handlesA[8];
    s32 handlesB[8];
    s32 handlesC[5];
    MenuPageSlot slots[5];
    MenuList *lists[2];
    s32 selected;
    s32 scrollOffset;
    s32 fade;
} MenuWindowResourceBank;

typedef union MenuWindowBankView {
    MenuPageWindow fields;
    struct {
        u8 prefix[0x20];
        MenuWindowResourceBank resources;
    } bank;
} MenuWindowBankView;

typedef char MenuWindowBankView_size_check[
    sizeof(MenuWindowBankView) == sizeof(MenuPageWindow) ? 1 : -1];
typedef char MenuWindowBankView_bank_offset_check[
    (u32)&((MenuWindowBankView *)0)->bank.resources == 0x20 ? 1 : -1];

void mnuClearPageSelection(MenuPageWindow *window) {
    s32 selected = window->selected;

    if (selected >= 0) {
        u32 bankAddress = (u32)&window->unk20;

        *(s32 *)(bankAddress + selected * (s32)sizeof(MenuPageSlot)
                 + (u32)&((MenuWindowResourceBank *)0)->slots[0].scaleA) = 0x100;
        *(s32 *)(bankAddress + window->selected * (s32)sizeof(MenuPageSlot)
                 + (u32)&((MenuWindowResourceBank *)0)->slots[0].scaleB) = 0x100;
        window->selected = -1;
    }
    window->flags &= ~0x400;
}


INCLUDE_ASM(const s32, "game/code_0027BF00", mnuInitPageWindow);


void mnuReleaseHandles(s32 obj) {
    MenuPageSlot *page = (MenuPageSlot *)obj;
    s32 *handle = page->icon;
    u32 i;

    for (i = 0; i < 3; i++, handle++) {
        if (*handle != 0) {
            effDestroyResourceSlotSet(*handle);
        }
    }
    if (page->frame[0] != 0) {
        effDestroyResourceSlotSet(page->frame[0]);
    }
    if (page->frame[1] != 0) {
        effDestroyResourceSlotSet(page->frame[1]);
    }
    if (page->frame[2] != 0) {
        effDestroyResourceSlotSet(page->frame[2]);
    }
    if (page->frame[3] != 0) {
        effDestroyResourceSlotSet(page->frame[3]);
    }
    if (page->frame[4] != 0) {
        effDestroyResourceSlotSet(page->frame[4]);
    }
    if (page->frame[5] != 0) {
        effDestroyResourceSlotSet(page->frame[5]);
    }
}

extern void mnuReleaseHandles(s32);

void mnuShutdownContext(s32 context) {
    u32 i;
    for (i = 0; i < 5; i++) {
        mnuReleaseHandles((s32)&((MenuPageWindow *)context)->slots[i]);
    }
    mnuReleasePartyPanelSpriteTextures(context);
    mnuDestroyWindowOwnedLists(context);
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

void mnuReleasePageTexturesAndSelectedResources(MenuPageWindow *window) {
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

INCLUDE_ASM(const s32, "game/code_0027BF00", mnuSelectPage);

void mnuReleasePageHandlesAndClearSelection(window)
    MenuPageWindow *window;
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

void mnuClearListFlags(s32 which, MenuPageWindow *menu) {
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

extern void mnuPlayInputSound(s32, s32, u32 *);

/* Step the selected party-panel list from the pad: left/right move its cursor, any input restarts the fade. */
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
            mnuRetreatListCursorDefault(list);
        }
        if (input & 0x20) {
            if (mnuTestListFlagTwo((u32 *)list) == 0) {
                window->fade = 0x100;
            }
            mnuAdvanceListCursorDefault(list);
        }
        mnuPlayInputSound(0, input, &list->stateFlags);
    }
    if (window->fade > 0) {
        s32 fade = window->fade - 0x10;

        window->fade = fade < 0 ? 0 : fade;
    }
    mnuClearPageSelection(window);
}

INCLUDE_ASM(const s32, "game/code_0027BF00", func_00280A90);

extern s32 func_00280A90(MenuPageWindow *, s32);
extern void uiDrawSurfaceAtNearDepth(s32);

/* Draw the party row's frame variant and restore the near-depth surface. */
void mnuDrawPartyRowFrameVariant(s32 x, s32 y, s32 z, MenuPageWindow *menu,
    s32 index, s32 force, s32 context) {
    s32 alpha = menu->fade / 2 + 0x80;
    u16 *partyFlags = &datGameState->party[menu->records->slots[index].index].flags;

    if (func_00280A90(menu, index) == 1 || force != 0) {
        if (*partyFlags & 2) {
            func_002BF4E0(x + 0x150, y + 0xA8, z, alpha, 1, menu->source, 11, context);
            func_002BF4E0(x + 0x660, y + 0x110, z, alpha, 1, menu->source, 16, context);
            func_002BF4E0(x + 0x130, y + 0x250, z, alpha, 1, menu->source, 20, context);
        } else if (force == 0) {
            func_002BF4E0(x + 0x1A0, y + 0x60, z, alpha, 1, menu->source, 21, context);
            uiDrawSurfaceAtNearDepth(context);
            func_002BF4E0(x + 0x1E0, y + 0x58, z, alpha, 1, menu->source, 10, context);
        } else {
            func_002BF4E0(x + 0x1C0, y + 0xE0, z, alpha, 1, menu->source, 20, context);
        }
    }
    uiDrawSurfaceAtNearDepth(context);
}

s32 mnuClearWindowPendingFlagAfterSelection(s32 unusedX, s32 unusedY, s32 unusedDepth, MenuPageWindow *window, s32 option) {
    s32 result = func_00280A90(window, option);

    switch (result) {
    case 1:
        window->flags &= ~2;
        return 1;
    case 2:
        window->flags &= ~2;
        return 1;
    default:
        return 0;
    }
}

extern u16 mnuGetPartyEntryMenuValue(DatPartyRecord *);
extern u16 evtGetIndexedEventRecordId(s32);
extern u32 func_001978E8(s32, s32, s32, u32, char *, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern u8 *D_003BAA84;
extern char D_003BC720[];
extern u8 D_003BC730[];
extern char D_003BC738[];

void func_00280E08(s32 x, s32 y, s32 z, s32 partyIndex, MenuSprites *page, s32 param) {
    char text[0x20];
    s32 *sprite = page->overlaySprites;
    u32 i = 0;
    s32 value;
    s32 alpha;
    s32 color;
    s32 glyphAddress;

    value = mnuGetPartyEntryMenuValue(&datGameState->party[partyIndex]);
    alpha = page->drawAlpha;
    color = uiBlendColors(0xA09DC380, 0xA09DC300, alpha);
    x += page->slideOffset * 16;
    func_002BF4E0(x, y, z, alpha, 0, page->primarySprite, 0, param);
    do {
        func_002BF4E0(x, y, z, alpha, 0, *sprite++, 0, param);
        i++;
    } while (i < 2);
    if (value != 0) {
        glyphAddress = itfCreateConvertedTextGlyph(x + 0x6F0, y + 0x330, z, color, D_003BAA84 + value * 25, 0);
        func_003014F0(text, D_003BC720, datCommandRecords[evtGetIndexedEventRecordId(value)].stat18);
        glyphAddress = func_001978E8(x + 0xF70, y + 0x348, z, color, text, glyphAddress);
    } else {
        glyphAddress = itfCreateConvertedTextGlyph(x + 0x6F0, y + 0x330, z, color, D_003BC730, 0);
        glyphAddress = func_001978E8(x + 0xF70, y + 0x348, z, color, D_003BC738, glyphAddress);
    }
    func_001958A0((struct FrFontGlyph *)glyphAddress, 1, param);
    frFontQueueGlyphInSelectedSlot((struct FrFontGlyph *)glyphAddress);
    if (page->fadeOut == 0) {
        if (page->drawAlpha < 256) {
            page->drawAlpha += 16;
        }
        if (page->drawAlpha > 256) {
            page->drawAlpha = 256;
        }
        page->slideOffset -= page->slideSpeed / 256;
        if (page->slideOffset < 0) {
            page->slideOffset = 0;
        }
        if (page->slideOffset != 0) {
            page->slideSpeed *= 1.5f;
        }
    } else {
        if (page->drawAlpha > 0) {
            page->drawAlpha -= 32;
        }
        if (page->drawAlpha < 0) {
            page->drawAlpha = 0;
        }
        page->slideOffset += page->slideSpeed / 256;
        if (page->slideOffset != 0) {
            page->slideSpeed /= 1.5f;
        }
    }
}

extern void func_002CD0D8(u32 textId, s32 arg1, char *out);

extern struct FrFontGlyph *func_001951C8(void *, s8, s8, s8, struct FrFontGlyph *);

extern u32 frFontMeasureGlyphChain(void *);

extern void frFontSetContextPair(struct FrFontCtx *, u32, u32);
extern void frFontSetChildColors(struct TextStyleNode *, u32);

void mnuDrawCenteredLabel(s32 x, s32 y, s32 unused, s32 color, s32 textId, s32 param) {
    char text[0x40];
    struct FrFontGlyph *glyph;
    s32 width;

    func_002CD0D8(textId & 0xFFFF, 1, text);
    glyph = func_001951C8(text, 0, 0, 0, 0);
    frFontSetChildColors((struct TextStyleNode *)glyph, color);
    width = frFontMeasureGlyphChain(glyph) + 8;
    frFontSetContextPair((struct FrFontCtx *)glyph, x - (width * 0x10 >> 1) + 0x5F0, y);
    func_001958A0(glyph, 1, param);
    frFontQueueGlyphInSelectedSlot(glyph);
}

void mnuDrawSelectedPartyProfileLabel(s32 unusedX, s32 unusedY, s32 depth, s32 fade, s32 selectedCode, MenuPageSlot *unusedSlot,
                     s32 partyIndex, s32 param) {
    s32 outValue;
    s32 profileId = ptyGetCurrentProfileId(&datGameState->party[partyIndex]);
    s32 code;
    s32 color;
    s32 glyphAddress;

    color = uiBlendColors(0xA09DC380, 0xA09DC300, fade);
    code = selectedCode != 0 ? selectedCode : profileId;
    if (code != 0) {
        if (func_002CD240(code & 0xFFFF, &outValue) != 0) {
            mnuDrawCenteredLabel(0x1120, 0x5F0, depth, color, code, param);
            return;
        }
        glyphAddress = itfCreateConvertedTextGlyph(0, 0, depth, color, (const u8 *)outValue, 0);
        func_00196088(0x1710, 0x5F0, glyphAddress);
        func_001958A0((struct FrFontGlyph *)glyphAddress, 1, param);
        frFontQueueGlyphInSelectedSlot((struct FrFontGlyph *)glyphAddress);
    }
}

extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern char D_003BC740[];

void func_002812E8(s32 x, s32 y, s32 depth, MenuPageSlot *slot,
                   s32 partyIndex, u32 flags, s32 surface) {
    char text[0x40];
    DatPartyRecord *unit = &datGameState->party[partyIndex];
    s32 color;
    s32 remainingExp;
    struct FrFontGlyph *glyph;

    if (slot->iconBundle != 0) {
        mnuDrawAndUpdateFadingSprites(x, y, depth, slot->kind,
            (MenuIconBundle *)slot->iconBundle, surface);
        color = uiBlendColors(0xFFF06480, 0xFFF06400,
            ((MenuIconBundle *)slot->iconBundle)->fade);
        remainingExp = ptyComputeTotalExp(unit, 1) - unit->totalExp;
        if (remainingExp != 0) {
            func_003014F0(text, D_003BC740, remainingExp);
            glyph = (struct FrFontGlyph *)func_001978E8(x + 0xDF0,
                0x160, depth, color, text, 0);
            func_001958A0(glyph, 1, surface);
            frFontQueueGlyphInSelectedSlot(glyph);
        }
    }
    if (slot->windowSprites != NULL) {
        if (flags & 0x400) {
            mnuDrawIconSpriteGroup(x, y, depth, 0,
                slot->windowSprites, surface);
        }
        /* This profile fade is independent of the sprite slide's drawAlpha. */
        mnuDrawSelectedPartyProfileLabel(x, y, depth,
            slot->windowSprites->profileFade, slot->windowSprites->unkC, slot,
            partyIndex, surface);
        if (slot->windowSprites->fadeOut == 0) {
            if (slot->windowSprites->profileFade > 0) {
                slot->windowSprites->profileFade -= 0x20;
            }
            if (slot->windowSprites->profileFade < 0) {
                slot->windowSprites->profileFade = 0;
            }
        } else {
            if (slot->windowSprites->profileFade < 0x100) {
                slot->windowSprites->profileFade += 0x20;
            }
            if (slot->windowSprites->profileFade > 0x100) {
                slot->windowSprites->profileFade = 0x100;
            }
        }
    }
}

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

