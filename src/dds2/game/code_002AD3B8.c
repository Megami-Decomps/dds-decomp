#include "mnu.h"

/* Staff callbacks receive a task handle as an integer word. Preserve the
 * native parameter widths and the short-arity task-user-value calls. */
#define MNU_STAFF_INPUT_CONFIRM 1
#define MNU_STAFF_INPUT_CANCEL 2
#define MNU_STAFF_INPUT_PREVIOUS_ROW 0x10
#define MNU_STAFF_INPUT_NEXT_ROW 0x20
#define MNU_STAFF_INPUT_PREVIOUS_PAGE 0x100
#define MNU_STAFF_INPUT_NEXT_PAGE 0x200

extern u32 kwlnTaskGetUserValue();

extern void func_0026C900(void);
extern void func_002AAE80(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void mnuUpdateAndDrawWindowTransition(s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];
extern char D_003E7207[20];
extern char D_003E7202[];
extern void mnuBeginWindowFadeTransition(s32, s32);
extern void func_002ABEB0(s32);
extern void func_002AC660(s32);
extern void func_002ACA98(s32);
extern void func_002B2C88(s32, s32, s32, s32);
extern void mnuClearPageSelectionHandles(s32);
extern void mnuClearEntries(s32);
extern void mnuDestroyPanelGroup(s32);
extern void mnuFreeSpriteStateWork(s32);
extern void func_002C1B68(s32, s32);
extern void mnuReleaseStaffMenuTextureHandles(s32);
extern void func_002C2AA8(s32, s32);
extern char D_00437BD0[];
extern char D_00437BD8[];
extern s32 D_003E7400[];
extern s32 func_002BDA50();
extern s32 func_002BDA78();
extern s32 itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern s32 func_0019F5E8(s32, s32, s32, s32, s32, s32);
extern void frFontSetChainFlag(s32, s32);
extern s32 func_0019D550(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);
extern s32 evtGetIndexedEventRecordId(s32);
extern s32 datCommandRecords;
extern s32 D_00435E5C;
extern s32 D_00435E48;
extern s32 mnuGetPartyEntryMenuValue();
extern s32 mnuGetPartyEntryCurrentId();
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 dspStartEntry(s32);
extern void ptyAdjustItemQuantity();
extern void mnuRefreshStaffWindowDescription();
extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);
extern char D_003E74F8[];
extern char D_003E7514[];
extern char D_003E7530[];
extern char D_003E7434[];
extern u32 mnuMapPadMaskToFlags();
extern void mnuStepPartyPanelListFromInput();
extern void mnuSetPopupEntryFlagged();
extern void mnuClearActionFlags();
extern void mnuPlayInputSound();
extern void func_002B9808();
extern void mnuRetreatWindowListSelection();
extern void mnuAdvanceWindowListSelection();
extern void mnuHandlePanelListPageJumpInput();
extern void mnuClearWindowPanelTransitionFlag();
extern void mnuRetreatListCursorDefault();
extern void mnuAdvanceListCursorDefault();
extern void mnuClearListFlagsOneAndTwo();
extern void mnuSeekListNode();
extern void mnuResetListNodeFadeCounters();
extern void sndSetSequenceVolumePan();
extern void mnuSelectPage(u32 *, s32);
extern void func_002ABD60(void *);
extern void mnuCreateOrderedStaffItemWindow(void *);
extern void mnuCreateOwnedCatalogItemWindow(void *);
extern s32 mdlFlagTest();
extern void func_002BB9C8(s32, s32);
extern void mnuReleaseStaffMenuResources(s32 *);
extern void mnuSetWindowResource(s32, u32 *, s32, s32, s32, s32, s32);
extern void mnuSetIndexedWindowPageSpriteFlags(s32, u32 *, s32, s32);
extern void *mnuCreatePanelGroup(s32, s32, s32);
extern void *mnuCreateSpriteState(s32, s32, s32);

/* Prefixes of the menu list and node used to read the current page index.
 * The primary list implementation also stores its cursor at +0x1C. */
typedef struct MenuListNode {
    s32 index;
} MenuListNode;

typedef struct MenuList {
    u8 pad00[0x1C];
    MenuListNode *cursor;
} MenuList;

typedef struct MenuSceneConfig {
    u8 pad00[0x10];
    s32 entries[5];
} MenuSceneConfig;

typedef struct MenuStaffObject {
    u8 pad00[0x18];
    MenuStaffWindow *window;
    u8 pad1C[0x78];
    s32 spriteAlpha;
} MenuStaffObject;

/* Staff menu state: selected objects, three list variants, and pending transitions. */
typedef struct MenuStaffChoices {
    u8 pad00[8];
    MenuStaffObject *primaryObject;   /* 0x08 */
    MenuStaffObject *secondaryObject; /* 0x0C */
    MenuStaffList *firstList;         /* 0x10 */
    MenuStaffList *secondList;        /* 0x14 */
    MenuStaffList *thirdList;         /* 0x18 */
    s32 currentSelection;   /* 0x1C */
    s32 thirdListEnabled;   /* 0x20 */
    s32 previous;           /* 0x24 */
    s32 requested;          /* 0x28 */
    s32 alternatePrevious;  /* 0x2C */
    s32 alternateRequested; /* 0x30 */
    s32 firstListState;     /* 0x34 */
    s32 secondListState;    /* 0x38 */
    s32 secondListReset;    /* 0x3C */
    s32 thirdListState;     /* 0x40 */
    s32 thirdListIndex;     /* 0x44 */
    s32 thirdListValue;     /* 0x48 */
    s32 thirdListReset;     /* 0x4C */
} MenuStaffChoices;

/* Prepare the primary staff object, then enter the image state. */
s32 mnuStaffImageEnterA(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    MenuStaffObject *object;

    func_002AAE80(task);
    mnuCreateStaffImageSprite(5);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    object = menu->primaryObject;
    if (object->window->panelActive != 0) {
        mnuRefreshStaffWindowDescription(context, 0);
    } else {
        if (menu->secondListState == 0) {
            func_00306CD0(0x390, 0x570, 0, object->spriteAlpha, 1, ((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        }
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, task);
}

/* Request value one from the message-window worker, then run the teardown phase. */
s32 mnuStaffImageExitA(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, task);
}

u32 func_002AD508(void) {
    return 1;
}

u32 func_002AD510(void) {
    return 1;
}

/* Once the popup is idle, confirmation captures the page-selection cursor index. */
s32 mnuStaffImageInputA(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, task);
    if (state != 0) {
        return state;
    }
    window = (u8 *)(context + 0x284);
    mnuStepPartyPanelListFromInput(4, window);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((MenuList *)((MenuStaffContext *)context)->selection)->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_003E74F8);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearActionFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

/* Set up a staff image and its associated menu resources before entering the state. */
s32 mnuPrepareStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(task);
    mnuCreateStaffImageSprite(7);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, task);
}

/* Run the label-image state's teardown phase and return its scheduler word. */
s32 mnuExitStaffImageAndSelectionLabel(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, task);
}

u32 func_002AD6F8(void) {
    return 1;
}

u32 func_002AD700(void) {
    return 1;
}

s32 mnuPollStaffSlotSelectionConfirmation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, task);
    if (state != 0) {
        return state;
    }
    window = (u8 *)(context + 0x284);
    mnuStepPartyPanelListFromInput(4, window);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((MenuList *)((MenuStaffContext *)context)->selection)->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_003E7514);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearActionFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

/* The three staff image states share the same setup, but select different images. */
s32 func_002AD808(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(task);
    mnuCreateStaffImageSprite(9);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, task);
}

/* Run this image variant's teardown phase and return its scheduler word. */
s32 func_002AD8B0(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, task);
}

u32 func_002AD8E8(void) {
    return 1;
}

u32 func_002AD8F0(void) {
    return 1;
}

s32 mnuPollStaffValueSelectionConfirmation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, task);
    if (state != 0) {
        return state;
    }
    window = (u8 *)(context + 0x284);
    mnuStepPartyPanelListFromInput(4, window);
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        menu->currentSelection = ((MenuList *)((MenuStaffContext *)context)->selection)->cursor->index;
        mnuSetPopupEntryFlagged(popup, D_003E7530);
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuClearActionFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s32 func_002AD9F8(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(task);
    mnuCreateStaffImageSprite(11);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, task);
}

/* Run this image variant's teardown phase and return its scheduler word. */
s32 func_002ADAA0(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, task);
}

u32 mnuRefreshSecondaryStaffObject(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition((u32)((MenuStaffChoices *)((MenuStaffContext *)context)->menu)->secondaryObject, context + 0xb10c);
    return 1;
}

u32 mnuRefreshActiveStaffWindow(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuBeginWindowFadeTransition((u32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    return 1;
}

/* Handle input on the secondary object; its window supplies the sound flags. */
s32 mnuHandleSecondaryStaffObjectInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(0xc33);
    s32 state;
    MenuStaffObject *object;

    state = func_002C4038(context + 8, popup, 0, task);
    if (state != 0) {
        return state;
    }
    if (buttons & MNU_STAFF_INPUT_CONFIRM) {
        buttons = 0;
    }
    if (buttons & MNU_STAFF_INPUT_CANCEL) {
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    object = menu->secondaryObject;
    if (object != 0) {
        if (!(buttons & 0x300000)) {
            func_002B9808((s32)object);
        }
        if (buttons & MNU_STAFF_INPUT_PREVIOUS_ROW) {
            mnuRetreatWindowListSelection((s32)object);
        }
        if (buttons & MNU_STAFF_INPUT_NEXT_ROW) {
            mnuAdvanceWindowListSelection((s32)object);
        }
        mnuHandlePanelListPageJumpInput(object, &buttons);
        mnuClearWindowPanelTransitionFlag(object);
        mnuPlayInputSound(0, buttons, (s32)object->window);
    }
    return 0;
}

s32 mnuStaffImageEnterD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    MenuStaffObject *object;

    func_002AAE80(task);
    mnuCreateStaffImageSprite(0xD);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    object = menu->secondaryObject;
    if (object->window->panelActive != 0) {
        mnuRefreshStaffWindowDescription(context, 1);
    } else {
        func_00306CD0(0x390, 0x570, 0, object->spriteAlpha, 1, ((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(2, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, task);
}

/* Run the secondary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitD(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, task);
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002ADDA0);

s32 mnuStaffImageEnterB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    MenuStaffObject *object;

    func_002AAE80(task);
    mnuCreateStaffImageSprite(6);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    object = menu->primaryObject;
    if (object->window->panelActive != 0) {
        mnuRefreshStaffWindowDescription(context, 0);
    } else {
        func_002AAC98(0,
            ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
            (s32)D_003E7050, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, task);
}

/* Run the alternate primary-image state's teardown phase and return its scheduler word. */
s32 mnuStaffImageExitB(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, task);
}

s32 mnuInitializeSelectedStaffPage(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    u32 *window = &context->windowFlags;
    s32 index = ((MenuList *)context->selection)->cursor->index;

    mnuSelectPage(window, index);
    func_002ABD60(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->windowResource, 0, 0);
    mnuSetIndexedWindowPageSpriteFlags(index, window, 1, 0);
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, 0);
    context->spriteHandle = mnuCreateSpriteState(context->spriteArg0, context->spriteArg1, context->group);
    context->windowFlags |= 0x200;
    context->windowFlags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->firstListState = 0;
    mnuBeginWindowFadeTransition((s32)menu->firstList, (s32)context->tail);
    return 1;
}

/* Release the current entry list, its panel group and its auxiliary resource. */
s32 mnuReleaseSelectedStaffPageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    mnuBeginWindowFadeTransition((s32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    func_002ABEB0(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork((s32)((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuPrepareStaffSelectionChangeDialog(s32 context, u8 *entry, s32 target) {
    u8 *menu = ((MenuStaffContext *)context)->menu;
    s32 current = mnuGetPartyEntryMenuValue(entry);

    func_002C1B68(context + 0xaa50, 1);
    if (current != target) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
        evtCopyEntryStringToActiveWindow(1, D_00435E5C + current * 0x19);
        evtCopyEntryStringToActiveWindow(2, D_00435E5C + target * 0x19);
        dspStartEntry(0);
        if (current != 0) {
            ptyAdjustItemQuantity(current, 1);
        }
        ptyAdjustItemQuantity(target, -1);
        ((MenuStaffChoices *)menu)->previous = current;
        ((MenuStaffChoices *)menu)->requested = target;
    } else {
        evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
        dspStartEntry(1);
        ((MenuStaffChoices *)menu)->previous = 0;
        ((MenuStaffChoices *)menu)->requested = 0;
    }
}

/* Switch pages and replay the first list's saved cursor steps. */
s32 mnuStaffListInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    MenuStaffWindow *window = menu->firstList->window;
    MenuStaffNode *cursor = window->cursor;
    s32 savedNodeIndex;
    s32 savedRowCount;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = cursor->index;
        savedRowCount = window->rowCount;
    } else {
        savedNodeIndex = 0;
        savedRowCount = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseSelectedStaffPageResources(task);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseSelectedStaffPageResources(task);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((MenuStaffContext *)context)->selection);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeSelectedStaffPage(task);
    if (savedNodeIndex != 0 || savedRowCount != 0) {
        mnuSeekListNode(savedNodeIndex, (s32)menu->firstList->window);
        if (savedRowCount > 0) {
            for (i = savedRowCount; i != 0; i--) {
                mnuAdvanceWindowListSelection((s32)menu->firstList);
            }
        }
        mnuResetListNodeFadeCounters((s32)menu->firstList->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE580);

void mnuDrawStaffCaption(s32 entryId, u8 *panel) {
    char captionText[16];
    s32 fontHandle;

    itfDrawGridWithResolvedSlot(0x1C0, 0xA10, 0, 0, ((MenuStaffContext *)panel)->spriteArg2, 2, 0x53);
    if (entryId != 0) {
        func_0035C860(captionText, D_00437BD0, *(s16 *)(evtGetIndexedEventRecordId(entryId) * 0x38 + datCommandRecords + 0x18));
        fontHandle = func_0019F5E8(0x620, 0xA20, 0, 0xA09DC380, (s32)captionText, 0);
        frFontSetChainFlag(fontHandle, 4);
        func_0019D550(fontHandle, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(fontHandle);
    }
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE888);

/* Request value one from the message-window worker before the teardown phase. */
s32 func_002AEA58(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, task);
}

/* One 0x2138-byte page slot supplies the resource checked before page setup. */
typedef struct MenuStaffPanelSlot {
    u8 pad00[0xDC];
    s32 resourceHandle;
    u8 padE0[0x2058];
} MenuStaffPanelSlot;

s32 mnuInitializeStaffPageWithSlotAsset(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    u32 *window = &context->windowFlags;
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 index = ((MenuList *)context->selection)->cursor->index;
    u8 *slot = (u8 *)context + index * 0x2138 + 0x2FC;

    mnuSelectPage(window, index);
    mnuCreateOrderedStaffItemWindow(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->windowResource, context->spriteArg0,
                         context->spriteArg2);
    mnuSetIndexedWindowPageSpriteFlags(index, window, 0, 2);
    if (mdlFlagTest(0x990) != 0) {
        func_002BB9C8(((MenuStaffPanelSlot *)slot)->resourceHandle, 1);
    }
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, 0);
    context->spriteHandle = mnuCreateSpriteState(context->spriteArg0, context->spriteArg1, context->group);
    context->windowFlags |= 0x200;
    context->windowFlags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->secondListReset = 0;
    mnuBeginWindowFadeTransition((s32)menu->secondList, (s32)context->tail);
    return 1;
}

/* Variant cleanup for the adjacent menu state; keep the same release ordering. */
s32 mnuReleaseStaffSelectionPageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    mnuBeginWindowFadeTransition((s32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    func_002AC660(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork((s32)((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuStaffEntrySwapLabels(s32 context, u8 *entry, s32 target) {
    u8 *menu = ((MenuStaffContext *)context)->menu;
    s32 current = mnuGetPartyEntryCurrentId(entry);

    func_002C1B68(context + 0xaa50, 1);
    if (target == 0) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
        evtCopyEntryStringToActiveWindow(1, D_00435E5C + current * 0x19);
        dspStartEntry(6);
        ((MenuStaffChoices *)menu)->alternatePrevious = current;
        ((MenuStaffChoices *)menu)->alternateRequested = 0;
    } else if (current != target) {
        if (current != 0) {
            evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
            evtCopyEntryStringToActiveWindow(1, D_00435E5C + current * 0x19);
            evtCopyEntryStringToActiveWindow(2, D_00435E5C + target * 0x19);
            dspStartEntry(3);
        } else {
            evtCopyEntryStringToActiveWindow(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
            evtCopyEntryStringToActiveWindow(1, D_00435E5C + target * 0x19);
            dspStartEntry(4);
        }
        ((MenuStaffChoices *)menu)->alternatePrevious = current;
        ((MenuStaffChoices *)menu)->alternateRequested = target;
    } else {
        evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
        dspStartEntry(5);
        ((MenuStaffChoices *)menu)->alternatePrevious = 0;
        ((MenuStaffChoices *)menu)->alternateRequested = 0;
    }
}

/* Preserve the second list's position while switching its selected page.
 * Previous-page input wins when both page-direction flags are present. */
s32 mnuHandleStaffSelectionListNavigation(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    MenuStaffWindow *window = menu->secondList->window;
    MenuStaffNode *cursor = window->cursor;
    s32 savedNodeIndex;
    s32 savedRowCount;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = cursor->index;
        savedRowCount = window->rowCount;
    } else {
        savedNodeIndex = 0;
        savedRowCount = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseStaffSelectionPageResources(task);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseStaffSelectionPageResources(task);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((MenuStaffContext *)context)->selection);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeStaffPageWithSlotAsset(task);
    if (savedNodeIndex != 0 || savedRowCount != 0) {
        mnuSeekListNode(savedNodeIndex, (s32)menu->secondList->window);
        if (savedRowCount > 0) {
            for (i = savedRowCount; i != 0; i--) {
                mnuAdvanceWindowListSelection((s32)menu->secondList);
            }
        }
        mnuResetListNodeFadeCounters((s32)menu->secondList->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF020);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF2E0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACA0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACC8);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF5E0);

/* Request value one from the message-window worker before the value-page teardown. */
s32 mnuExitStaffValuePage(s32 task) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, task);
}

s32 mnuInitializeStaffValuePage(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    u32 *window = &context->windowFlags;
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 index = ((MenuList *)context->selection)->cursor->index;
    u8 *slot = (u8 *)context + index * 0x2138 + 0x2FC;

    mnuSelectPage(window, index);
    mnuCreateOwnedCatalogItemWindow(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->windowResource, context->spriteArg0,
                         context->spriteArg2);
    mnuSetIndexedWindowPageSpriteFlags(index, window, 0, 2);
    if (mdlFlagTest(0x990) != 0) {
        func_002BB9C8(((MenuStaffPanelSlot *)slot)->resourceHandle, 1);
    }
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, context->spriteArg2);
    context->spriteHandle = mnuCreateSpriteState(context->spriteArg0, context->spriteArg1, context->group);
    context->windowFlags |= 0x200;
    context->windowFlags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->thirdListState = 0;
    if (menu->thirdListEnabled != 0) {
        menu->thirdListReset = 0;
    }
    mnuBeginWindowFadeTransition((s32)menu->thirdList, (s32)context->tail);
    return 1;
}

/* Third menu-state cleanup uses the matching state-specific pre-release. */
s32 mnuReleaseStaffValuePageResources(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    mnuBeginWindowFadeTransition((s32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    func_002ACA98(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        mnuFreeSpriteStateWork((s32)((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuPrepareStaffValueChangeDialog(s32 context, u8 *entry, s32 unused, s32 flag) {
    char valueText[16];
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 current;
    s32 base;

    func_002C1B68(context + 0xaa50, 1);
    current = mnuGetPartyEntryCurrentId(entry);
    evtCopyEntryStringToActiveWindow(0, D_00435E5C + current * 0x19);
    evtCopyEntryStringToActiveWindow(1, D_003E7400[menu->thirdListIndex]);
    func_0035C860(valueText, D_00437BD8, menu->thirdListValue);
    evtCopyEntryStringToActiveWindow(2, (s32)valueText);
    base = func_002BDA50(current);
    func_0035C860(valueText, D_00437BD8, func_002BDA78(current) - base);
    evtCopyEntryStringToActiveWindow(3, (s32)valueText);
    if (flag == 0) {
        dspStartEntry(9);
    } else {
        dspStartEntry(0xA);
    }
}

/* Preserve the third list's position while switching its selected page.
 * Returns one after rebuilding the page, zero when neither direction wins. */
s32 mnuHandleStaffValuePageInput(s32 task) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 pageChanged = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);
    MenuStaffWindow *window = menu->thirdList->window;
    MenuStaffNode *cursor = window->cursor;
    s32 savedNodeIndex;
    s32 savedRowCount;
    s32 i;

    if (cursor != 0) {
        savedNodeIndex = cursor->index;
        savedRowCount = window->rowCount;
    } else {
        savedNodeIndex = 0;
        savedRowCount = 0;
    }
    if (buttons & MNU_STAFF_INPUT_PREVIOUS_PAGE) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    if (buttons & MNU_STAFF_INPUT_NEXT_PAGE && pageChanged == 0) {
        pageChanged = 1;
        mnuReleaseStaffValuePageResources(task);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((MenuStaffContext *)context)->selection);
    if (pageChanged == 0) {
        return 0;
    }
    mnuInitializeStaffValuePage(task);
    if (savedNodeIndex != 0 || savedRowCount != 0) {
        mnuSeekListNode(savedNodeIndex, (s32)menu->thirdList->window);
        if (savedRowCount > 0) {
            for (i = savedRowCount; i != 0; i--) {
                mnuAdvanceWindowListSelection((s32)menu->thirdList);
            }
        }
        mnuResetListNodeFadeCounters((s32)menu->thirdList->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

typedef struct MenuRequirementRecord {
    u8 pad00[5];
    s8 requiredCount;
} MenuRequirementRecord;

extern s32 datGameState;
extern s32 D_00435E3C;

/* A zero entry has no requirement. Keep the signed requiredCount comparison:
 * the available count comes from a byte-sized game-state entry. */
s32 mnuIsStaffRequirementUnmet(s32 entryId) {
    s32 requirementIndex = entryId - 0xC0;
    u8 availableCount;

    if (entryId == 0) {
        return 0;
    }
    availableCount = *(u8 *)(entryId + datGameState + 0x20000 - 0x1910);
    return availableCount < ((MenuRequirementRecord *)D_00435E3C)[requirementIndex].requiredCount;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFE18);

void mnuSetPanelItemsFromRow(MenuSceneConfig *config, s32 rowIndex) {
    char *activeSlots = D_003E7207 + rowIndex * 0x1C;
    char *entryFlags;
    s32 *configEntry;
    u32 lastActiveSlot = 0;
    s32 i;

    for (i = 0; i < sizeof(D_003E7207); i++, activeSlots++) {
        if (*activeSlots) {
            lastActiveSlot = i;
        }
    }
    configEntry = config->entries;
    entryFlags = D_003E7202 + rowIndex * 0x1C;
    for (i = 4; i >= 0; i--, entryFlags++) {
        func_002C2AA8(*configEntry++, *entryFlags ? lastActiveSlot : 0);
    }
}

void mnuClearStaffSceneConfigEntries(MenuSceneConfig *config) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_002C2AA8(config->entries[i], 0);
    }
}

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD0);

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD8);

