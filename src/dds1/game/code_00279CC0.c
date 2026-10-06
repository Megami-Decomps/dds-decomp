#include "mnu.h"
#include "dat_state.h"

extern void *sdfAllocAndClearQuadwords(s32);
struct MenuListNode;
extern struct MenuListNode *mnuAdvanceListCursorDefault(u32 list);
extern struct MenuListNode *mnuRetreatListCursorDefault(u32 list);

typedef struct MenuWindowContainer MenuWindowContainer;

typedef struct MenuList MenuList;

extern void itfSetGridEntryQuantizedAndRefresh(s32, s32, s32, s32, s32, s32);

extern s32 D_003BAA98;

extern void func_001958A0(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);


typedef struct MenuListNode MenuListNode;

extern s32 kwlnTaskGetUserValue();

extern void func_00272778(s32);

extern void mnuCreateStaffImageSprite(s32);

extern void func_002723B0(s32, s32);

extern void ptySkillMenuCopyPageState(s32);

extern void mnuDrawStaffCampScreen(s32, s32);

extern void func_00272518(s32, s32, s32, s32, s32, s32, s32);

extern void func_00272668(s32, s32, s32, s32, s32, s32);

extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, s32);

extern void mnuSetPopupEntryFlagged(s32 *, char *);

extern char D_0037CC20[];

typedef struct MenuSlot {
    s32 resources[3];
    u16 unused;
    u16 flags;
} MenuSlot;

extern MenuSlot *datAffinityRecords;



extern void mnuDrawWindowContainer(s32, s32, s32, MenuWindowContainer *, s32);

extern void sdfReleaseChipBlock(void *);

extern s32 func_0027B888(u32);

extern s32 kwlnTaskGetUserValue();

extern s32 func_00285670(s32, s32 *, u64, u64);

#include "mnu_list.h"
/* Window prefix shared with the drawable container in code_0027BF00. */
struct MenuWindowContainer {
    s32 id;
    u32 flags;
    s32 width;
    s32 height;
    u8 pad10[4];
    MenuList *list; /* Its initial stateFlags word controls navigation sounds. */
};

/* Selected child window and page of the party skill-menu runtime. */
typedef struct MenuPartyRuntime {
    u8 pad00[4];
    s32 selectionList; /* 0x04: list whose cursor selects the active window */
    u8 pad08[0x1C];
    s32 selectedWindow; /* 0x24 */
    u8 pad28[4];
    s32 selectedPage;   /* 0x2C */
} MenuPartyRuntime;

/* Shift the chosen child window to target's row; the cursor index selects its slot. */
void mnuSeekSelectedWindowRow(s32 menu, s32 target) {
    s32 *entry = (s32 *)menu + **(s32 **)(((MenuPartyRuntime *)menu)->selectionList + 0x1C);
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

INCLUDE_ASM(const s32, "game/code_00279CC0", ptySkillMenuBrowseCandidatePages);

s32 mnuOpenSkillDetailPanel(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *menu = *(s32 **)(context + 0x90C);
    s32 index = ((MenuList *)menu[3])->cursor->index;
    s32 label = ((MenuWindowContainer *)*(s32 *)((s32)menu + 0x10 + (index << 2)))->list->cursor->sortKeyPrimary;
    s32 window;
    ptySkillMenuCopyPageState(context);
    mnuDrawStaffCampScreen(1, callback);
    mnuCreateStaffImageSprite(0xB);
    if (label != 0xFFFF && label != 0) {
        func_00272518(1, label, D_003BAA98, context, 1, 1, 0x53);
    } else {
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    window = menu[9];
    ((MenuWindowContainer *)window)->list->stateFlags |= 8;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, window, 0x53);
    func_002723B0(3, *(s32 *)(context + 0x78));
    return menuRunPanel(context, 1, callback);
}

s32 func_0027A0A8(s32 callback) {
    return menuRunPanel(kwlnTaskGetUserValue(), 2, callback);
}

void mnuDrawSelectionLabel(s32 selection) {
    s32 item = itfDrawTextWithSelectedFontMode(0xCB0, 0xA80, 0, 0, selection & 0xFFFF, 1);
    frFontSetChildColors(item, 0xA09DC366);
    func_001958A0(item, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(item);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A140);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A300);


/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(DatPartyRecord **out, s32 max) {
    s32 count = 0;
    s32 i = 0;

    while (count < max) {
        DatPartyRecord *entry = &datGameState->party[i];

        *out = 0;
        if ((entry->flags & 1) != 0 &&
            (entry->flags & 2) != 0) {
            *out = entry;
            out++;
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

INCLUDE_ASM(const s32, "game/code_00279CC0", ptySkillMenuBuildLinkageSkills);

u32 mnuDestroySkillSelectionWindow() {
    s32 window;

    window = kwlnTaskGetUserValue();
    window = *(s32 *)(window + 0x90c);
    mnuDestroyWindowContainer(((MenuPartyRuntime *)window)->selectedWindow);
    ((MenuPartyRuntime *)window)->selectedWindow = 0;
    return 1;
}

s32 mnuResetSelection(s32 selection) {
    s32 context;
    s32 menu;
    mnuCampMenuInit();
    context = kwlnTaskGetUserValue(selection);
    menu = *(s32 *)(context + 0x90C);
    ptySkillMenuBuildLinkageSkills(selection);
    mnuFlagActiveWindows(context + 0x15C);
    ((MenuPartyRuntime *)menu)->selectedPage = 0;
    return 1;
}

s32 mnuCloseSkillSelection(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    mnuDestroySkillSelectionWindow(selection);
    mnuClearPartyPanelActiveFlags(context + 0x15c);
    mnuCloseItemSelectionState(selection);
    return 1;
}

s32 mnuUpdateSkillListInput(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *popup = (s32 *)(context + 0x54);
    s32 *menu = *(s32 **)(context + 0x90C);
    u32 buttons = mnuMapPadMaskToFlags(0x32);
    s32 state;
    s32 *list;

    state = menuRunPanel(context, 0, callback);
    if (state == 0) {
        if ((buttons & 0x300000) == 0) {
            list = menu + 1;
            func_0027C788(list[8 + menu[11]]);
        }
        list = menu + 1;
        if (buttons & 0x10) {
            mnuRetreatWindowListSelection(list[8 + menu[11]]);
        }
        if (buttons & 0x20) {
            mnuAdvanceWindowListSelection(list[8 + menu[11]]);
        }
        mnuClearWindowPanelTransitionFlag(list[8 + menu[11]]);
        mnuPlayInputSound(0, buttons, (s32)((MenuWindowContainer *)list[8 + menu[11]])->list);
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(popup, D_0037CC20);
            mnuActivatePanelAndConfigureGridResources(*(u32 **)(context + 0x138), *(s32 *)(context + 0x6C), 0, 1);
        }
        return 0;
    }
    return state;
}

/* Sprite slots of the active skill-menu context; intervening state is opaque. */
typedef struct MenuContextSprites {
    u8 pad00[0x64];
    u32 sprite64;
    s32 label68;
    u32 frame6C;
    u8 pad70[4];
    u32 sprite74;
    u32 sprite78;
    u8 pad7C[0x64];
    u32 spriteE0;
    u32 spriteE4;
} MenuContextSprites;

void mnuDrawSkillMenuFrameIcons(s32 context) {
    itfDrawGridWithResolvedSlot(0x1c0, 0xa60, 0, 1, ((MenuContextSprites *)context)->sprite74, 0x1f, 0x53);
    itfDrawGridWithResolvedSlot(0x150, 0xa00, 0, 1, ((MenuContextSprites *)context)->sprite74, 0, 0x53);
    itfDrawGridWithResolvedSlot(0xbb0, 0xa00, 0, 1, ((MenuContextSprites *)context)->sprite74, 0, 0x53);
    itfDrawGridWithResolvedSlot(0x250, 0x9c0, 0, 1, ((MenuContextSprites *)context)->spriteE4, 0x18, 0x53);
    itfDrawGridWithResolvedSlot(0xce0, 0x9e0, 0, 1, ((MenuContextSprites *)context)->sprite64, 2, 0x53);
}

extern void func_002BF4E0(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuDrawListFrames(s32 menu) {
    s32 i;

    for (i = **(s32 **)(menu + 0x164); i < 3; i++) {
        s32 x = 0xF10;

        if (i == 1) {
            x = 0xE70;
        }
        func_002BF4E0(x, i * 0x320 + 0xC8, 0, 0x100, 1, ((MenuContextSprites *)menu)->frame6C, 7, 0x53);
    }
}

s32 mnuCampMenuDrawStatus(s32 param) {
    s32 context = kwlnTaskGetUserValue();
    s32 menu = *(s32 *)(context + 0x90C);
    s32 slots;
    MenuList *list;
    s32 label;

    func_00272778(param);
    mnuDrawListFrames(context);
    mnuCreateStaffImageSprite(0x10);
    slots = menu + 4;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, *(s32 *)(slots + ((MenuPartyRuntime *)menu)->selectedPage * 4 + 0x20), 0x53);
    list = ((MenuWindowContainer *)*(s32 *)(slots + ((MenuPartyRuntime *)menu)->selectedPage * 4 + 0x20))->list;
    label = list->cursor->sortKeyPrimary;
    mnuDrawSkillMenuFrameIcons(context);
    if (label != 0) {
        if (label != 0xFFFF) {
            label &= 0xFFFF;
            mnuDrawSelectionLabel(label);
            func_0027A140(label, ((MenuContextSprites *)context)->label68, ((MenuContextSprites *)context)->spriteE0);
        }
    }
    func_002723B0(2, ((MenuContextSprites *)context)->sprite78);
    return menuRunPanel(context, 1, param);
}

s32 func_0027AC00(s32 callback) {
    return menuRunPanel(kwlnTaskGetUserValue(), 2, callback);
}

typedef struct MenuEffectPayload {
    u8 pad0[8];
    u8 *data;
} MenuEffectPayload;

typedef struct MenuAssets {
    u32 sprites[5];
    u32 material;
    MenuEffectPayload *layerA;
    MenuEffectPayload *layerB;
} MenuAssets;

void mnuBindAssetEffectPayloads(MenuAssets *assets) {
    s32 packet;
    MenuEffectPayload *first;
    MenuEffectPayload *second;

    first = (MenuEffectPayload *)effCreatePayload(2);
    assets->layerA = first;
    second = (MenuEffectPayload *)effCreatePayload(2);
    packet = (s32)second->data;
    assets->layerB = second;
    effSetSlotIndexedResource(packet + 0x28, assets->material, 0, 0xc);
    effSetSlotIndexedResource((s32)assets->layerB->data + 0x94, assets->material, 1, 0xc);
    effSetMaterialSlots(assets->sprites[4], 0, 0, (u32)assets->layerB->data);
    effSetMaterialSlots(assets->sprites[4], 1, 0, (s32)assets->layerB->data + 0x6c);
    effSetMaterialSlots(assets->sprites[4], 2, 0, (s32)assets->layerB->data + 0x6c);
    effSetMaterialSlots(assets->sprites[4], 3, 0, (u32)assets->layerB->data);
    effSetMaterialSlots(assets->sprites[4], 4, 0, (u32)assets->layerB->data);
    effSetSlotIndexedResource((s32)assets->layerA->data + 0x28, assets->material, 2, 0xd);
    effSetSlotOverrideWork(assets->sprites[1], 0, (u32)assets->layerA->data);
    effConfigureIndexedSlotResource(assets->sprites[2], 0, assets->material, 3, 4);
    effConfigureIndexedSlotResource(assets->sprites[3], 0, assets->material, 4, 4);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AD80);

extern char D_003B2330[];

extern char D_003B2348[];

extern u32 D_0037CD18[];

extern u32 D_0037CD20[];

void mnuRequestBaseAssets(MenuAssets *assets) {
    effRequestResourceByMode(D_003B2330, D_0037CD18[1], 0, &assets->sprites[0]);
    effRequestResourceByMode(D_003B2330, D_0037CD18[0], 0, &assets->sprites[4]);
    effRequestMappedResource(D_003B2348, D_0037CD20[0], &assets->material);
}

typedef struct MenuColorEntry {
    u8 pad0[0x84];
    u32 color[4];
    u8 pad94[0xC];
} MenuColorEntry;

typedef struct MenuColorSet {
    u8 pad0[0x18];
    MenuColorEntry *entries;
} MenuColorSet;

typedef struct MenuColorHost {
    u32 resource;      /* 0x00 */
    u32 sprite[3];     /* 0x04 */
    MenuColorSet *set; /* 0x10 */
    s32 ready;         /* 0x14 */
} MenuColorHost;

s32 mnuInitializeCampAssetSprites(MenuColorHost *obj) {
    s32 i;
    s32 j;

    if (obj->resource == 0) {
        return 0;
    }
    if (obj->set == NULL) {
        return 0;
    }
    if (obj->ready == 0) {
        return 0;
    }
    obj->sprite[0] = effCreateResourceSlotSet(obj->resource, 0, 1);
    obj->sprite[1] = effCreateResourceSlotSet(obj->resource, 0, 1);
    obj->sprite[2] = effCreateResourceSlotSet(obj->resource, 0, 1);
    mnuBindAssetEffectPayloads(obj);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            obj->set->entries[i].color[j] = 0x8080805A;
        }
    }
    return 1;
}

void mnuReleaseAssets(MenuAssets *assets) {
    u32 i;
    for (i = 0; i < 4; i++) {
        effDestroyResourceSlotSet(assets->sprites[i]);
    }
    effDestroyResourceSlotSet(assets->sprites[4]);
    effDestroyPackedBatch(assets->material);
    effDestroyPayload(assets->layerA);
    effDestroyPayload(assets->layerB);
}

void mnuDrawCampBackdropDecoration(MenuAssets *assets, u32 drawArg) {
    uiDrawTexturedSurfaceAtFarDepth(drawArg);
    itfDrawGridWithResolvedSlot(0xffffffffffffff90, 0xa0, 0, 0x61, assets->sprites[4], 0, drawArg);
    itfDrawGridWithResolvedSlot(0xfffffffffffffb90, 0x808, 0, 0x61, assets->sprites[4], 1, drawArg);
    itfDrawGridWithResolvedSlot(0x1050, 0xfffffffffffffc18, 0, 0x61, assets->sprites[4], 2, drawArg);
    itfDrawGridWithResolvedSlot(0x10b0, 0x3c0, 0, 0x61, assets->sprites[4], 3, drawArg);
    itfDrawGridWithResolvedSlot(0x1300, 0xb70, 0, 0x61, assets->sprites[4], 4, drawArg);
    func_002C1548(0, drawArg);
    itfGridLookupValueOrDefault(assets->sprites[4], 0);
    itfGridLookupValueOrDefault(assets->sprites[4], 1);
    itfDrawGridWithResolvedSlot(0, 0, 0, 0x60, assets->sprites[1], 0, drawArg);
    itfGridLookupValueOrDefault(assets->sprites[1], 0);
    uiDrawSurfaceAtNearDepth(drawArg);
}

extern void itfGridLookupValueOrDefault(s32, s32);

void mnuDrawCursorIcons(MenuAssets *assets, s32 arg) {
    s32 icon = assets->sprites[2];
    s32 *state = *(s32 **)(icon + 0x18);

    state[3] = 0x9000;
    state[4] = 0x3F00;
    func_002BF4E0(-0x4800, -0x1F80, 0, 0x50, 0, icon, 0, arg);
    itfGridLookupValueOrDefault(assets->sprites[2], 0);
    icon = assets->sprites[3];
    state = *(s32 **)(icon + 0x18);
    state[3] = 0x9000;
    state[4] = 0x3F00;
    func_002BF4E0(-0x2800, -0x1180, 0, 0x50, 0, icon, 0, arg);
    itfGridLookupValueOrDefault(assets->sprites[3], 0);
}

void mnuDrawBackdrop(MenuAssets *assets, s32 option) {
    sdfSubmitGsTestOneRegisterPacket(0x30000);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x80808080, option);
    itfDrawGridWithResolvedSlot(0, 0, 0, 0, assets->sprites[0], 0, option);
    mnuDrawCursorIcons(assets, option);
    mnuDrawCampBackdropDecoration(assets, option);
}


MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing) {
    MenuList *item = (MenuList *)sdfAllocAndClearQuadwords(0x40);
    item->id = id;
    item->visibleCount = visibleCount;
    item->rowStep = rowSpacing * 8;
    item->scale = 0x100;
    item->head = NULL;
    item->first = NULL;
    item->windowOffset = 0;
    item->cursor = NULL;
    return item;
}

u32 mnuDestroyListState(MenuList *list) {
    s64 pending;

    do {
        pending = func_0027B888((u32)list);
    } while (pending != 0);
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

MenuListNode *mnuListAdvanceCursor(MenuList *, s32, s32);
MenuListNode *mnuListRetreatCursor(MenuList *, s32, s32);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B540);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B888);

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

void *mnuWalkNodeList(s32 targetIndex, MenuList *list) {
    MenuListNode *node = list->first;
    s32 index = 0;

    if (node != NULL && targetIndex != index) {
        do {
            node = node->next;
            index++;
        } while (node != NULL && index != targetIndex);
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

/* Advance the visible head when a prior window offset can be reduced. */
s32 mnuAdvanceListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *last = list->last;
    MenuListNode *head = list->head;

    if (cursor == last) {
        return (s32)cursor;
    }
    head = head->next;
    if (head == NULL) {
        return (s32)cursor;
    }
    list->head = head;
    list->windowOffset--;
    return (s32)cursor;
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

void mnuClearListFlagsOneAndTwo(u32 *flags) {
    *flags &= ~1;
    *flags &= ~2;
}

u32 mnuTestListFlagTwo(u32 *flags) {
    return *flags & 2;
}

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2330);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2348);


