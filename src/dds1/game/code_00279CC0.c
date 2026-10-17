#include "sdf_chip.h"
#include "itf_draw_grid.h"
#include "eff_resource_slots.h"
#include "mnu_input.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "mnu_scroll_panel.h"
#include "dat_state.h"
#include "pty_roster_api.h"
#include "eff.h"
#include "eff_resource_records.h"
#include "itf.h"
#include "fr_font.h"

extern EffPayload *effCreatePayload(u32);
extern u32 effDestroyPayload(EffPayload *);

struct MenuListNode;


typedef struct MenuList MenuList;

extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *, s32, s32, s32, s32, s32);

extern s32 D_003BAA98;

extern s32 frFontDrawGlyphChain(struct FrFontGlyph *, s8, u32);
extern FrFontGlyph *itfDrawTextWithSelectedFontMode(s32, s32, s32, s8, u16, s32);


typedef struct MenuListNode MenuListNode;


extern void mnuDrawCampScreenDefault(s32);

extern void mnuCreateStaffImageSprite(s32);


extern void ptySkillMenuCopyPageState(s32);

extern void mnuDrawStaffCampScreen(s32, KwlnTask *);

extern void mnuDrawCampGridAndLabel(s32, s32, s32, s32, s32, s32, s32);

extern void mnuDrawCampGridAndLabelDefault(s32, s32, s32, s32, s32, s32);

extern void mnuPlayInputSound(s32, s32, u32 *);

extern char mnuCampFieldSkillPopupEntry[];







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

extern char mnuSkillMenuPagePopupEntry[];
extern char mnuSkillMenuEquippedSlotsPopupEntry[];
extern s32 ptySkillMenuHandlePageSwitch(KwlnTask *);
extern void mnuClearWindowListSelectionFlags(MenuWindowContainer *);
extern void mnuClearListFlagsOneAndTwo(u32 *flags);

/* The camp initializer allocates and clears this complete 0x38-byte child. */
typedef struct SkillBrowseRuntime {
    struct SdfMemBlock *allocation;
    u8 pad04[4];
    s32 active;
    struct MenuList *categoryList;
    MenuWindowContainer *windows[4];
    void *panel;
    MenuWindowContainer *selectedWindow;
    s32 activeMark;
    s32 motionSelection;
    u32 selectionFlags;
    u32 selectedIndex;
} SkillBrowseRuntime;

typedef char SkillBrowseRuntimeSizeCheck[(sizeof(SkillBrowseRuntime) == 0x38) ? 1 : -1];

/* Browse the candidate pages: pad direction moves the window selection and the confirm/cancel buttons open popups. */
s32 ptySkillMenuBrowseCandidatePages(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);
    SkillBrowseRuntime *menu = *(SkillBrowseRuntime **)(context + 0x90C);
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(0x37);
    u32 moves = mnuMapPadMaskToFlags(0xC0);
    MenuWindowContainer *window;
    struct MenuListNode *node;
    s32 windowOffset;
    s32 state;
    MenuWindowContainer **windows;
    s32 index;

    state = menuRunPanel((void *)context, 0, (void *)task);
    if (state == 0) {
        if (ptySkillMenuHandlePageSwitch(task) != 0) {
            return 0;
        }
        windows = menu->windows;
        index = menu->categoryList->cursor->index;
        window = *(MenuWindowContainer **)((s32)windows + (index << 2));
        if ((buttons & 0x300000) == 0) {
            mnuClearWindowListSelectionFlags(window);
        }
        if (buttons & 0x10) {
            mnuRetreatWindowListSelection(window);
        }
        if (buttons & 0x20) {
            mnuAdvanceWindowListSelection(window);
        }
        mnuClearWindowPanelTransitionFlag(window);
        if ((moves & 0xC00000) == 0) {
            mnuClearListFlagsOneAndTwo(&menu->categoryList->stateFlags);
        }
        windowOffset = window->list->windowOffset;
        if (moves & 0x40) {
            mnuRetreatListCursorDefault(menu->categoryList);
            mnuSeekSelectedWindowRow((s32)menu + 8, windowOffset);
        }
        if (moves & 0x80) {
            mnuAdvanceListCursorDefault(menu->categoryList);
            mnuSeekSelectedWindowRow((s32)menu + 8, windowOffset);
        }
        mnuPlayInputSound(0, moves, &menu->categoryList->stateFlags);
        index = menu->categoryList->cursor->index;
        window = *(MenuWindowContainer **)((s32)windows + (index << 2));
        if (buttons & 1) {
            node = window->list->cursor;
            if (node->camp.value != 0xFFFF && (node->flags48 & 1) == 0) {
                mnuSetPopupEntry(popup, mnuSkillMenuPagePopupEntry);
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 4) {
            menu->selectionFlags = 1;
            buttons = 1;
            mnuSetPopupEntry(popup, mnuSkillMenuPagePopupEntry);
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(popup, mnuSkillMenuEquippedSlotsPopupEntry);
        }
        mnuPlayInputSound(0, buttons, &window->list->stateFlags);
        return 0;
    }
    return state;
}

s32 mnuOpenSkillDetailPanel(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    s32 *menu = *(s32 **)(context + 0x90C);
    s32 index = ((MenuList *)menu[3])->cursor->index;
    s32 label = ((MenuWindowContainer *)*(s32 *)((s32)menu + 0x10 + (index << 2)))->list->cursor->sortKeyPrimary;
    s32 window;
    ptySkillMenuCopyPageState(context);
    mnuDrawStaffCampScreen(1, callback);
    mnuCreateStaffImageSprite(0xB);
    if (label != 0xFFFF && label != 0) {
        mnuDrawCampGridAndLabel(1, label, D_003BAA98, context, 1, 1, 0x53);
    } else {
        mnuDrawCampGridAndLabelDefault(1, 0, 0, context, 1, 0x53);
    }
    window = menu[9];
    ((MenuWindowContainer *)window)->list->stateFlags |= 8;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, window, 0x53);
    mnuDrawStaffGridLabelsForKind(3, (struct EffectSlotSet *)(u32)(*(s32 *)(context + 0x78)));
    return menuRunPanel((void *)context, 1, (void *)callback);
}

/* Run the skill-detail panel's phase-two control and return its result. */
s32 mnuStepSkillDetailControl(KwlnTask *callback) {
    return menuRunPanel((void *)kwlnTaskGetUserValue(callback), 2, (void *)callback);
}

void mnuDrawSelectionLabel(s32 selection) {
    FrFontGlyph *item = itfDrawTextWithSelectedFontMode(0xCB0, 0xA80, 0, 0, selection & 0xFFFF, 1);
    frFontSetChildColors(item, 0xA09DC366);
    frFontDrawGlyphChain(item, 1, 0x53);
    frFontQueueGlyphForCurrentDrawBuffer(item);
}

extern s32 ptyGetAffinityKind(s32 affinityId, s32 index);
extern s32 ptyGetAffinityFlagsWithoutOverride(s32 affinityId, s32 index);
extern s32 mnuLookupRangeEntry(u16);
extern const u8 *mnuComboGunRequirementTexts[16];
extern const u8 *D_003BAA8C;

/* Draw each affinity requirement, including range markers and optional text. */
void mnuDrawAffinityParticipantLabels(u16 affinityId, s32 placeholderResource, s32 rangeResource) {
    s32 slot;
    s32 y = 0xA80;

    for (slot = 0; slot < 3; slot++, y += 0xC8) {
        s32 kind = ptyGetAffinityKind((s32)affinityId, slot);
        FrFontGlyph *glyph = 0;

        if (kind < 0) {
            if (kind == -1) {
                s32 flags = ptyGetAffinityFlagsWithoutOverride((s32)affinityId, slot);

                if (flags > 0) {
                    s32 rangeIndex = mnuLookupRangeEntry((u16)flags);
                    itfDrawGridWithResolvedSlot(
                        0x2C0, y - 0x10, 0, 1, (EffectSlotSet *)(u32)rangeResource,
                        rangeIndex * 2 + 10, 0x53);
                    glyph = itfCreateConvertedTextGlyph(
                        0x420, y, 0, 0xA09DC380,
                        D_003BAA8C + flags * 17, 0);
                } else {
                    itfDrawGridWithResolvedSlot(
                        0x420, y + 0x38, 0, 1, (EffectSlotSet *)(u32)placeholderResource, 9, 0x53);
                }
            } else {
                s32 flags = ptyGetAffinityFlagsWithoutOverride((s32)affinityId, slot);

                glyph = itfCreateConvertedTextGlyph(
                    0x420, y, 0, 0xA09DC380,
                    D_003BAA8C + flags * 17 + 0x2860, 0);
            }
        } else {
            glyph = itfCreateConvertedTextGlyph(
                0x420, y, 0, 0xA09DC380, mnuComboGunRequirementTexts[kind], 0);
        }

        if (glyph != 0) {
            frFontDrawGlyphChain(glyph, 1, 0x53);
            frFontQueueGlyphForCurrentDrawBuffer(glyph);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", ptyDrawLinkageSkillEntry);


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

extern void ptyDrawLinkageSkillEntry(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);
extern s32 btlAccessRecordedLinkageSkillFlag(s32 id, s32 operation);
extern u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate);
extern const u8 D_003BC708[];
extern void mnuAttachWindowTextureState(MenuWindowContainer *, u32, u32, u32, u32);
extern void mnuSortItems(MenuList *, s32, s32);
extern void mnuSetWindowFadeScale(MenuWindowContainer *, u32);
extern void mnuSetWindowPanelBounds(MenuWindowContainer *, const void *, u32, u32, u32, u32);

/* Build the linkage-skill window: every recorded linkage skill with an
 * available slot resource, flagged when the frontline party cannot form it,
 * plus blank rows for unrecorded skills that have a resource. */
void ptySkillMenuBuildLinkageSkills(KwlnTask *task) {
    /* Both retail versions place the frontline array at stack +0x10, after
     * the window workspace; only its first window is populated here. */
    MenuWindowContainer *windows[2];
    DatPartyRecord *party[3];
    s32 context = kwlnTaskGetUserValue(task);
    s32 menu = *(s32 *)(context + 0x90C);
    s32 i;
    s32 skill;
    s32 last;
    struct MenuListNode *node;
    MenuWindowContainer **windowCursor;
    MenuWindowContainer **selectedWindow;

    mnuCollectFrontlinePartySlots(party, 3);
    i = 0;
    windowCursor = windows;
    do {
        *windowCursor = mnuCreateWindowContainer(0, 0x60, 0x10, 8, 0x15);
        mnuSetWindowFadeScale(*windowCursor, 0x100);
        mnuForwardDupArg(*windowCursor, *(struct EffectSlotSet **)(context + 0x74), 0,
                         *(struct EffectSlotSet **)(context + 0xE4), 0x17);
        mnuSetWindowPanelBounds(*windowCursor, *(const void **)(context + 0x11C), 0x130, 0x630, -0x90, 0xC20);
        mnuInitializeWindowEntryPlacement(0, *windowCursor, *(struct EffectSlotSet **)(context + 0x74), 0x14, 0x18);
        (*windowCursor)->list->drawCallback = ptyDrawLinkageSkillEntry;
        (*windowCursor)->list->context = (void *)context;
        mnuAttachWindowTextureState(*windowCursor, -0xE0, 0x370, 0, *(u32 *)(context + 0xEC));
        mnuConfigureWindowSpriteSlots((*windowCursor)->textures, *(struct EffMappedResource **)(context + 0x118));
        windowCursor++;
    } while (--i >= 0);
    skill = mnuPickPairedTableValue(0x10, 0);
    last = mnuPickPairedTableValue(0x10, 1);
    if (skill < last) {
        MenuWindowContainer **blankWindow = windows;
        do {
            if (btlAccessRecordedLinkageSkillFlag(skill & 0xFFFF, 2) != 0 && mnuHasAvailableSlotResource(skill) != 0) {
                node = mnuAppendWindowListNode(windows[0], D_003BAA8C + skill * 17);
                node->sortKeyPrimary = skill;
                {
                    s32 unavailable = ptyMatchAffinityPermutation((const s32 *)party, skill);
                    /* The matcher returns 0/1. The consumed local preserves
                     * the inverted Boolean as XOR rather than a comparison. */
                    unavailable ^= 1;
                    if (unavailable != 0) {
                        node->flags48 |= 1;
                    }
                }
            } else if (mnuHasAvailableSlotResource(skill) != 0) {
                node = mnuAppendWindowListNode(*blankWindow, D_003BC708);
                node->sortKeyPrimary = 0xFFFF;
            }
            skill++;
        } while (skill < last);
    }
    selectedWindow = (MenuWindowContainer **)(menu + 0x24);
    mnuSortItems(windows[0]->list, 0, 1);
    *selectedWindow = windows[0];
}

u32 mnuDestroySkillSelectionWindow(KwlnTask *task) {
    s32 window;

    window = kwlnTaskGetUserValue(task);
    window = *(s32 *)(window + 0x90c);
    mnuDestroyWindowContainer(((MenuPartyRuntime *)window)->selectedWindow);
    ((MenuPartyRuntime *)window)->selectedWindow = 0;
    return 1;
}

s32 mnuResetSelection(KwlnTask *selection) {
    s32 context;
    s32 menu;
    mnuCampMenuInit();
    context = kwlnTaskGetUserValue(selection);
    menu = *(s32 *)(context + 0x90C);
    ptySkillMenuBuildLinkageSkills(selection);
    mnuFlagActiveWindows((MenuPageWindow *)(context + 0x15C));
    ((MenuPartyRuntime *)menu)->selectedPage = 0;
    return 1;
}

s32 mnuCloseSkillSelection(KwlnTask *selection) {
    s32 context = kwlnTaskGetUserValue(selection);
    mnuDestroySkillSelectionWindow(selection);
    mnuClearPartyPanelActiveFlags((MenuPageWindow *)(context + 0x15c));
    mnuCloseItemSelectionState(selection);
    return 1;
}

s32 mnuUpdateSkillListInput(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    s32 *popup = (s32 *)(context + 0x54);
    s32 *menu = *(s32 **)(context + 0x90C);
    u32 buttons = mnuMapPadMaskToFlags(0x32);
    s32 state;
    s32 *list;

    state = menuRunPanel((void *)context, 0, (void *)callback);
    if (state == 0) {
        if ((buttons & 0x300000) == 0) {
            list = menu + 1;
            mnuClearWindowListSelectionFlags(list[8 + menu[11]]);
        }
        list = menu + 1;
        if (buttons & 0x10) {
            mnuRetreatWindowListSelection((MenuWindowContainer *)list[8 + menu[11]]);
        }
        if (buttons & 0x20) {
            mnuAdvanceWindowListSelection((MenuWindowContainer *)list[8 + menu[11]]);
        }
        mnuClearWindowPanelTransitionFlag((MenuWindowContainer *)list[8 + menu[11]]);
        mnuPlayInputSound(0, buttons, &((MenuWindowContainer *)list[8 + menu[11]])->list->stateFlags);
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(popup, mnuCampFieldSkillPopupEntry);
            mnuActivatePanelAndConfigureGridResources(
                (MenuScrollPanel *)*(u32 *)(context + 0x138),
                (struct EffectSlotSet *)(u32)*(s32 *)(context + 0x6C), 0, 1);
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

void mnuDrawSkillMenuFrameIcons(s32 menuContext) {
    itfDrawGridWithResolvedSlot(0x1c0, 0xa60, 0, 1, (EffectSlotSet *)(u32)((MenuContextSprites *)menuContext)->sprite74, 0x1f, 0x53);
    itfDrawGridWithResolvedSlot(0x150, 0xa00, 0, 1, (EffectSlotSet *)(u32)((MenuContextSprites *)menuContext)->sprite74, 0, 0x53);
    itfDrawGridWithResolvedSlot(0xbb0, 0xa00, 0, 1, (EffectSlotSet *)(u32)((MenuContextSprites *)menuContext)->sprite74, 0, 0x53);
    itfDrawGridWithResolvedSlot(0x250, 0x9c0, 0, 1, (EffectSlotSet *)(u32)((MenuContextSprites *)menuContext)->spriteE4, 0x18, 0x53);
    itfDrawGridWithResolvedSlot(0xce0, 0x9e0, 0, 1, (EffectSlotSet *)(u32)((MenuContextSprites *)menuContext)->sprite64, 2, 0x53);
}

extern void effDrawSpriteWithCornerFade(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuDrawListFrames(s32 menu) {
    s32 i;

    for (i = **(s32 **)(menu + 0x164); i < 3; i++) {
        s32 x = 0xF10;

        if (i == 1) {
            x = 0xE70;
        }
        effDrawSpriteWithCornerFade(x, i * 0x320 + 0xC8, 0, 0x100, 1, ((MenuContextSprites *)menu)->frame6C, 7, 0x53);
    }
}

s32 mnuCampMenuDrawStatus(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    s32 menu = *(s32 *)(context + 0x90C);
    s32 slots;
    MenuList *list;
    s32 label;

    mnuDrawCampScreenDefault(callback);
    mnuDrawListFrames(context);
    mnuCreateStaffImageSprite(0x10);
    slots = menu + 4;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (MenuWindowContainer *)*(s32 *)(slots + ((MenuPartyRuntime *)menu)->selectedPage * 4 + 0x20), 0x53);
    list = ((MenuWindowContainer *)*(s32 *)(slots + ((MenuPartyRuntime *)menu)->selectedPage * 4 + 0x20))->list;
    label = list->cursor->sortKeyPrimary;
    mnuDrawSkillMenuFrameIcons(context);
    if (label != 0) {
        if (label != 0xFFFF) {
            label &= 0xFFFF;
            mnuDrawSelectionLabel(label);
            mnuDrawAffinityParticipantLabels(label, ((MenuContextSprites *)context)->label68, ((MenuContextSprites *)context)->spriteE0);
        }
    }
    mnuDrawStaffGridLabelsForKind(2, (struct EffectSlotSet *)(u32)(((MenuContextSprites *)context)->sprite78));
    return menuRunPanel((void *)context, 1, (void *)callback);
}

/* Run the camp-status panel's phase-two control and return its result. */
s32 mnuStepCampStatusControl(KwlnTask *callback) {
    return menuRunPanel((void *)kwlnTaskGetUserValue(callback), 2, (void *)callback);
}


void mnuBindAssetEffectPayloads(MenuAssets *assets) {
    s32 packet;
    EffPayload *first;
    EffPayload *second;

    first = effCreatePayload(2);
    assets->layerA = first;
    second = effCreatePayload(2);
    packet = (s32)second->records;
    assets->layerB = second;
    effSetSlotIndexedResource((EffTimedState *)(packet + 0x28), assets->material, 0, 0xc);
    effSetSlotIndexedResource((EffTimedState *)((s32)assets->layerB->records + 0x94),
                              assets->material, 1, 0xc);
    effSetMaterialSlots(assets->sprites[4], 0, 0, assets->layerB->records);
    effSetMaterialSlots(assets->sprites[4], 1, 0, assets->layerB->records + 0x6C);
    effSetMaterialSlots(assets->sprites[4], 2, 0, assets->layerB->records + 0x6C);
    effSetMaterialSlots(assets->sprites[4], 3, 0, assets->layerB->records);
    effSetMaterialSlots(assets->sprites[4], 4, 0, assets->layerB->records);
    effSetSlotIndexedResource((EffTimedState *)((s32)assets->layerA->records + 0x28),
                              assets->material, 2, 0xd);
    effSetSlotOverrideWork(assets->sprites[1], 0, assets->layerA->records);
    effConfigureIndexedSlotResource(assets->sprites[2], 0, assets->material, 3, 4);
    effConfigureIndexedSlotResource(assets->sprites[3], 0, assets->material, 4, 4);
}

extern char D_003B2330[];
extern char D_003B2348[];
extern u32 mnuCampBackdropSpriteResourceNames[];
extern u32 mnuCampBackdropAnimationResourceNames[];

/* Load the base sprite banks synchronously, bind the payloads, and reset the fifth bank's saved corner colors. */
void mnuLoadBackdropAssetsAndTintSlots(MenuAssets *assets) {
    s32 i;
    s32 j;

    assets->sprites[0] = effLoadIndexedResource(D_003B2330, (const char *)mnuCampBackdropSpriteResourceNames[1], 0);
    assets->sprites[1] = effCreateResourceSlotSet(assets->sprites[0], 0, 1);
    assets->sprites[2] = effCreateResourceSlotSet(assets->sprites[0], 0, 1);
    assets->sprites[3] = effCreateResourceSlotSet(assets->sprites[0], 0, 1);
    assets->sprites[4] = effLoadIndexedResource(D_003B2330, (const char *)mnuCampBackdropSpriteResourceNames[0], 0);
    assets->material = effLoadMappedResource(D_003B2348, (const char *)mnuCampBackdropAnimationResourceNames[0]);
    mnuBindAssetEffectPayloads(assets);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            assets->sprites[4]->workEntries[i].savedColors[j] = 0x8080805A;
        }
    }
}

extern char D_003B2330[];

extern char D_003B2348[];

extern u32 mnuCampBackdropSpriteResourceNames[];

extern u32 mnuCampBackdropAnimationResourceNames[];

void mnuRequestBaseAssets(MenuAssets *assets) {
    effRequestResourceByMode(D_003B2330, mnuCampBackdropSpriteResourceNames[1], 0,
                             (u32 *)&assets->sprites[0]);
    effRequestResourceByMode(D_003B2330, mnuCampBackdropSpriteResourceNames[0], 0,
                             (u32 *)&assets->sprites[4]);
    effRequestMappedResource(D_003B2348, mnuCampBackdropAnimationResourceNames[0], &assets->material);
}

extern s32 mnuInitializeCampAssetSprites(MenuAssets *);
s32 mnuInitializeCampAssetSprites(MenuAssets *assets) {
    s32 i;
    s32 j;

    if (assets->sprites[0] == NULL) {
        return 0;
    }
    if (assets->sprites[4] == NULL) {
        return 0;
    }
    if (assets->material == NULL) {
        return 0;
    }
    assets->sprites[1] = effCreateResourceSlotSet(assets->sprites[0], 0, 1);
    assets->sprites[2] = effCreateResourceSlotSet(assets->sprites[0], 0, 1);
    assets->sprites[3] = effCreateResourceSlotSet(assets->sprites[0], 0, 1);
    mnuBindAssetEffectPayloads(assets);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            assets->sprites[4]->workEntries[i].savedColors[j] = 0x8080805A;
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

extern void uiDrawSurfaceAtNearDepth(u32);

void mnuDrawCampBackdropDecoration(MenuAssets *assets, u32 drawArg) {
    uiDrawTexturedSurfaceAtFarDepth(drawArg);
    itfDrawGridWithResolvedSlot(0xffffffffffffff90, 0xa0, 0, 0x61, assets->sprites[4], 0, drawArg);
    itfDrawGridWithResolvedSlot(0xfffffffffffffb90, 0x808, 0, 0x61, assets->sprites[4], 1, drawArg);
    itfDrawGridWithResolvedSlot(0x1050, 0xfffffffffffffc18, 0, 0x61, assets->sprites[4], 2, drawArg);
    itfDrawGridWithResolvedSlot(0x10b0, 0x3c0, 0, 0x61, assets->sprites[4], 3, drawArg);
    itfDrawGridWithResolvedSlot(0x1300, 0xb70, 0, 0x61, assets->sprites[4], 4, drawArg);
    uiPrepareMenuBackgroundSurface(0, drawArg);
    itfGridLookupValueOrDefault((s32)assets->sprites[4], 0);
    itfGridLookupValueOrDefault((s32)assets->sprites[4], 1);
    itfDrawGridWithResolvedSlot(0, 0, 0, 0x60, assets->sprites[1], 0, drawArg);
    itfGridLookupValueOrDefault((s32)assets->sprites[1], 0);
    uiDrawSurfaceAtNearDepth(drawArg);
}

extern void itfGridLookupValueOrDefault(s32, s32);

void mnuDrawCursorIcons(MenuAssets *assets, s32 surface) {
    EffectSlotSet *icon = assets->sprites[2];
    s32 *state = *(s32 **)((u8 *)icon + 0x18);

    state[3] = 0x9000;
    state[4] = 0x3F00;
    effDrawSpriteWithCornerFade(-0x4800, -0x1F80, 0, 0x50, 0, (s32)icon, 0, surface);
    itfGridLookupValueOrDefault((s32)assets->sprites[2], 0);
    icon = assets->sprites[3];
    state = *(s32 **)((u8 *)icon + 0x18);
    state[3] = 0x9000;
    state[4] = 0x3F00;
    effDrawSpriteWithCornerFade(-0x2800, -0x1180, 0, 0x50, 0, (s32)icon, 0, surface);
    itfGridLookupValueOrDefault((s32)assets->sprites[3], 0);
}

extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);

void mnuDrawBackdrop(MenuAssets *assets, s32 option) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, option);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x80808080, option);
    itfDrawGridWithResolvedSlot(0, 0, 0, 0, assets->sprites[0], 0, option);
    mnuDrawCursorIcons(assets, option);
    mnuDrawCampBackdropDecoration(assets, option);
}


MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing) {
    MenuList *item = (MenuList *)sdfAllocAndClearQuadwords(sizeof(MenuList));
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
    MenuListNode *pending;

    do {
        pending = mnuRemoveListCursorNode(list);
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
    const void *value;
{
    MenuListNode *node = sdfAllocAndClearQuadwords(sizeof(MenuListNode));
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

MenuListNode *mnuInsertListNodeRelativeToAnchor(MenuList *list, MenuListNode *anchor,
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
    if (options & MNU_LIST_INSERT_AFTER_ANCHOR) {
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

MenuListNode *mnuRemoveListCursorNode(MenuList *list) {
    MenuListNode *node;
    MenuListNode *cursor;
    MenuListNode *previous;
    MenuListNode *next;

    if (list->count == 0) {
        return NULL;
    }
    node = list->cursor;
    if (node == NULL) {
        return NULL;
    }
    cursor = node;
    do {
        if (node->index > 0) {
            node->index--;
        }
        node = node->next;
    } while (node != NULL);
    node = cursor;
    previous = node->prev;
    next = node->next;
    if (list->last->index - list->head->index + 1 <= list->visibleCount) {
        if (list->head != list->first) {
            cursor = previous;
            list->head = list->head->prev;
            list->cursor = previous;
        } else if (node == list->head) {
            if (next != NULL) {
                list->head = next;
                cursor = next;
                list->cursor = next;
            } else {
                cursor = previous;
                list->head = previous;
                list->cursor = previous;
                list->windowOffset--;
            }
        } else if (next != NULL) {
            list->cursor = next;
            cursor = next;
        } else {
            cursor = previous;
            list->cursor = previous;
            list->windowOffset--;
        }
    } else if (next != NULL) {
        list->cursor = next;
        cursor = next;
    }
    if (cursor == NULL) {
        list->head = NULL;
        list->first = NULL;
        list->last = NULL;
        list->windowOffset = 0;
    }
    if (previous != NULL) {
        previous->next = next;
    }
    if (next != NULL) {
        next->prev = previous;
    }
    if (previous == NULL) {
        list->head = next;
        list->first = next;
    }
    if (next == NULL) {
        list->last = previous;
    }
    sdfReleaseChipBlock(node);
    list->count--;
    mnuUpdateListScrollFlags(list);
    return list->cursor;
}



void mnuSetGridSpriteSlot(MenuListNode *node, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect) {
    node->sprites[row * 4 + col].sprite = (EffectSlotSet *)(u32)sprite;
    node->sprites[row * 4 + col].effect = effect;
    itfSetGridEntryQuantizedAndRefresh((EffectSlotSet *)(u32)sprite, effect, x, y, x, y);
}

struct MenuListNode *mnuWalkNodeList(s32 targetIndex, void *listOwner) {
    struct MenuListNode *node = *(struct MenuListNode **)((u8 *)listOwner + 0x10);
    s32 index = 0;

    if (node != NULL && targetIndex != index) {
        do {
            node = node->next;
            index++;
        } while (node != NULL && index != targetIndex);
    }
    return node;
}

s32 mnuSeekListNode(s32 targetIndex, MenuList *list) {
    s32 size = list->count;

    if (targetIndex >= size) {
        return 0;
    }
    list->windowOffset = 0;
    list->head = list->first;
    list->cursor = list->first;
    if (targetIndex > 0) {
        do {
            if (size - list->head->index <= list->visibleCount) {
                list->windowOffset += 1;
            } else {
                list->head = list->head->next;
            }
            list->cursor = list->cursor->next;
            targetIndex--;
        } while (targetIndex != 0);
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
MenuListNode *mnuAdvanceListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *last = list->last;
    MenuListNode *head = list->head;

    if (cursor == last) {
        return cursor;
    }
    head = head->next;
    if (head == NULL) {
        return cursor;
    }
    list->head = head;
    list->windowOffset--;
    return cursor;
}

/* Step the visible head back one node when a full window follows it. */
MenuListNode *mnuRetreatListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *head = list->head;
    MenuListNode *node;
    s32 i;

    if (cursor == list->first) {
        return cursor;
    }
    node = head;
    for (i = 0; i < list->visibleCount; i++) {
        if (node == NULL) {
            return cursor;
        }
        node = node->next;
    }
    head = head->prev;
    list->head = head;
    list->windowOffset++;
    return cursor;
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
                cursor = mnuAdvanceListWindowStart(list);
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
                cursor = mnuRetreatListWindowStart(list);
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

MenuListNode *mnuAdvanceListCursorDefault(MenuList *list) {
    return mnuListAdvanceCursor(list, 0, 0);
}

MenuListNode *mnuRetreatListCursorDefault(MenuList *list) {
    return mnuListRetreatCursor(list, 0, 0);
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

