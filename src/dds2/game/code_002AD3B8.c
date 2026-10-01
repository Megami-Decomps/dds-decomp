#include "common.h"

extern s32 kwlnTaskGetUserValue();

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_0026C900(void);
extern void func_002AAE80(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void func_002BB0E8(s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];
extern char D_003E7207[20];
extern char D_003E7202[];
extern void func_002BAF50(s32, s32);
extern void func_002ABEB0(s32);
extern void func_002AC660(s32);
extern void func_002ACA98(s32);
extern void func_002B2C88(s32, s32, s32, s32);
extern void mnuClearPageSelectionHandles(s32);
extern void mnuClearEntries(s32);
extern void mnuDestroyPanelGroup(s32);
extern void func_002C1050(s32);
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
extern s32 D_00435E20;
extern s32 D_00435E5C;
extern s32 D_00435E48;
extern s32 mnuGetPartyEntryMenuValue();
extern s32 func_002C55C0();
extern void func_0026C918(s32, s32);
extern void dspStartEntry(s32);
extern void func_0011A118();
extern void func_002AD330();
extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);
extern char D_003E74F8[];
extern char D_003E7514[];
extern char D_003E7530[];
extern char D_003E7434[];
extern u32 func_002C44E8();
extern void func_002BD480();
extern void mnuSetPopupEntryFlagged();
extern void mnuClearActionFlags();
extern void mnuPlayInputSound();
extern void func_002B9808();
extern void func_002B97F0();
extern void func_002B97D8();
extern void func_002C48C8();
extern void func_002B96D8();
extern void mnuRetreatListCursorDefault();
extern void mnuAdvanceListCursorDefault();
extern void mnuClearListFlagsOneAndTwo();
extern void mnuSeekListNode();
extern void mnuResetListNodeFadeCounters();
extern void sndSetSequenceVolumePan();
extern void mnuSelectPage(u32 *, s32);
extern void func_002ABD60(void *);
extern void func_002AC408(void *);
extern void func_002AC8F0(void *);
extern s32 mdlFlagTest();
extern void func_002BB9C8(s32, s32);
extern void mnuReleaseStaffMenuResources(s32 *);
extern void mnuSetWindowResource(s32, u32 *, s32, s32, s32, s32, s32);
extern void func_002BC078(s32, u32 *, s32, s32);
extern void *mnuCreatePanelGroup(s32, s32, s32);
extern void *mnuCreateSpriteState(s32, s32, s32);

typedef struct MenuSceneConfig {
    u8 pad00[0x10];
    s32 entries[5];
} MenuSceneConfig;

typedef struct MenuStaffWindow MenuStaffWindow;
typedef struct MenuStaffNode MenuStaffNode;
typedef struct MenuStaffList MenuStaffList;

typedef struct MenuStaffContext {
    u8 pad00[0x60];
    s32 group;            /* 0x60 */
    s32 spriteArg0;       /* 0x64 */
    s32 spriteArg1;       /* 0x68 */
    s32 unk6C;
    u8 pad70[0x54];
    s32 spriteArg2;       /* 0xC4 */
    u8 padC8[0x40];
    MenuStaffList *activeWindow; /* 0x108: window used by staff image states */
    u8 pad10C[0x178];
    u32 windowFlags;      /* 0x284 */
    u8 pad288[0xA68C];
    s32 selection;        /* 0xA914 */
    u8 padA918[0x11C];
    void *panelHandle;    /* 0xAA34 */
    void *spriteHandle;   /* 0xAA38 */
    u8 padAA3C[0xC];
    u8 *menu;             /* 0xAA48 */
    u8 padAA4C[0x6C0];
    u8 tail[4];           /* 0xB10C */
} MenuStaffContext;

/* Each staff list owns a cursor-bearing window at +0x18. */
struct MenuStaffList {
    u8 pad00[0x18];
    MenuStaffWindow *window;
};

struct MenuStaffWindow {
    u8 pad00[0x18];
    s32 *cursor;
    MenuStaffNode *selectedNode; /* 0x1C */
    s32 panelActive; /* 0x20: selects the alternate panel drawing path */
    s32 rowCount; /* 0x24 */
};

struct MenuStaffNode {
    u8 pad00[0x60];
    s32 label;
};

typedef struct MenuStaffObject {
    u8 pad00[0x18];
    MenuStaffWindow *window;
    u8 pad1C[0x78];
    s32 spriteAlpha;
} MenuStaffObject;

/* Staff menu state: selected objects, three list variants, and pending transitions. */
typedef struct MenuStaffChoices {
    u8 pad00[8];
    u8 *primaryObject;      /* 0x08 */
    u8 *secondaryObject;    /* 0x0C */
    u8 *firstList;          /* 0x10 */
    u8 *secondList;         /* 0x14 */
    u8 *thirdList;          /* 0x18 */
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

s64 func_002AD3B8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    u8 *object;

    func_002AAE80(callback);
    mnuCreateStaffImageSprite(5);
    func_002BB0E8(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    object = menu->primaryObject;
    if (((MenuStaffObject *)object)->window->panelActive != 0) {
        func_002AD330(context, 0);
    } else {
        if (menu->secondListState == 0) {
            func_00306CD0(0x390, 0x570, 0, ((MenuStaffObject *)object)->spriteAlpha, 1, ((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        }
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

/* Ask the menu state machine to handle a new request after clearing stale state. */
void func_002AD4C0(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, request);
}

u32 func_002AD508(void) {
    return 1;
}

u32 func_002AD510(void) {
    return 1;
}

s64 mnuStaffImageInputA(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(3);
    s64 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(4, window);
    if (buttons & 1) {
        menu->currentSelection = **(s32 **)(((MenuStaffContext *)context)->selection + 0x1C);
        mnuSetPopupEntryFlagged(popup, D_003E74F8);
    }
    if (buttons & 2) {
        mnuClearActionFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

/* Set up a staff image and its associated menu resources before entering the state. */
s64 func_002AD618(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(7);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    func_002BB0E8(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

void func_002AD6C0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    func_002C4038(context + 8, context + 0x54, 2, input);
}

u32 func_002AD6F8(void) {
    return 1;
}

u32 func_002AD700(void) {
    return 1;
}

s64 func_002AD708(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(3);
    s64 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(4, window);
    if (buttons & 1) {
        menu->currentSelection = **(s32 **)(((MenuStaffContext *)context)->selection + 0x1C);
        mnuSetPopupEntryFlagged(popup, D_003E7514);
    }
    if (buttons & 2) {
        mnuClearActionFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

/* The three staff image states share the same setup, but select different images. */
s64 func_002AD808(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(9);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    func_002BB0E8(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

s64 func_002AD8B0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

u32 func_002AD8E8(void) {
    return 1;
}

u32 func_002AD8F0(void) {
    return 1;
}

s64 func_002AD8F8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(3);
    s64 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(4, window);
    if (buttons & 1) {
        menu->currentSelection = **(s32 **)(((MenuStaffContext *)context)->selection + 0x1C);
        mnuSetPopupEntryFlagged(popup, D_003E7530);
    }
    if (buttons & 2) {
        mnuClearActionFlags(0, window);
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s64 func_002AD9F8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(11);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    func_002BB0E8(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

s64 func_002ADAA0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

u32 func_002ADAD8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_002BAF50((u32)((MenuStaffChoices *)((MenuStaffContext *)context)->menu)->secondaryObject, context + 0xb10c);
    return 1;
}

u32 func_002ADB18(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_002BAF50((u32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    return 1;
}

s64 func_002ADB48(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(0xc33);
    s64 state;
    u8 *window;

    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    if (buttons & 1) {
        buttons = 0;
    }
    if (buttons & 2) {
        mnuSetPopupEntryFlagged(popup, D_003E7434);
    }
    window = menu->secondaryObject;
    if (window != 0) {
        if (!(buttons & 0x300000)) {
            func_002B9808((s32)window);
        }
        if (buttons & 0x10) {
            func_002B97F0((s32)window);
        }
        if (buttons & 0x20) {
            func_002B97D8((s32)window);
        }
        func_002C48C8(window, &buttons);
        func_002B96D8(window);
        mnuPlayInputSound(0, buttons, (s32)((MenuStaffObject *)window)->window);
    }
    return 0;
}

s64 mnuStaffImageEnterD(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    u8 *object;

    func_002AAE80(callback);
    mnuCreateStaffImageSprite(0xD);
    func_002BB0E8(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    object = menu->secondaryObject;
    if (((MenuStaffObject *)object)->window->panelActive != 0) {
        func_002AD330(context, 1);
    } else {
        func_00306CD0(0x390, 0x570, 0, ((MenuStaffObject *)object)->spriteAlpha, 1, ((MenuStaffContext *)context)->spriteArg2, 0x11, 0x53);
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(2, ((MenuStaffContext *)context)->group);
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

s64 func_002ADD68(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002ADDA0);

s64 func_002ADF90(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    u8 *object;

    func_002AAE80(callback);
    mnuCreateStaffImageSprite(6);
    func_002BB0E8(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    object = menu->primaryObject;
    if (((MenuStaffObject *)object)->window->panelActive != 0) {
        func_002AD330(context, 0);
    } else {
        func_002AAC98(0,
            ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
            (s32)D_003E7050, context, 1, 0x53);
    }
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

s64 func_002AE078(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

s32 func_002AE0B0(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    u32 *window = &context->windowFlags;
    s32 index = **(s32 **)(context->selection + 0x1C);

    mnuSelectPage(window, index);
    func_002ABD60(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->unk6C, 0, 0);
    func_002BC078(index, window, 1, 0);
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, 0);
    context->spriteHandle = mnuCreateSpriteState(context->spriteArg0, context->spriteArg1, context->group);
    context->windowFlags |= 0x200;
    context->windowFlags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->firstListState = 0;
    func_002BAF50((s32)menu->firstList, (s32)context->tail);
    return 1;
}

/* Release the current entry list, its panel group and its auxiliary resource. */
s32 func_002AE1F0(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    func_002BAF50((s32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    func_002ABEB0(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        func_002C1050((s32)((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void func_002AE2D0(s32 context, u8 *entry, s32 target) {
    u8 *menu = ((MenuStaffContext *)context)->menu;
    s32 current = mnuGetPartyEntryMenuValue(entry);

    func_002C1B68(context + 0xaa50, 1);
    if (current != target) {
        func_0026C918(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
        func_0026C918(1, D_00435E5C + current * 0x19);
        func_0026C918(2, D_00435E5C + target * 0x19);
        dspStartEntry(0);
        if (current != 0) {
            func_0011A118(current, 1);
        }
        func_0011A118(target, -1);
        ((MenuStaffChoices *)menu)->previous = current;
        ((MenuStaffChoices *)menu)->requested = target;
    } else {
        func_0026C918(0, D_00435E5C + current * 0x19);
        dspStartEntry(1);
        ((MenuStaffChoices *)menu)->previous = 0;
        ((MenuStaffChoices *)menu)->requested = 0;
    }
}

s32 mnuStaffListInput(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 changed = 0;
    u32 buttons = func_002C44E8(0x300);
    u8 *window = ((MenuStaffList *)menu->firstList)->window;
    s32 *node = ((MenuStaffWindow *)window)->cursor;
    s32 first;
    s32 count;
    s32 i;

    if (node != 0) {
        first = *node;
        count = ((MenuStaffWindow *)window)->rowCount;
    } else {
        first = 0;
        count = 0;
    }
    if (buttons & 0x100) {
        changed = 1;
        func_002AE1F0(callback);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    if (buttons & 0x200 && changed == 0) {
        changed = 1;
        func_002AE1F0(callback);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((MenuStaffContext *)context)->selection);
    if (changed == 0) {
        return 0;
    }
    func_002AE0B0(callback);
    if (first != 0 || count != 0) {
        mnuSeekListNode(first, (s32)((MenuStaffList *)menu->firstList)->window);
        if (count > 0) {
            for (i = count; i != 0; i--) {
                func_002B97D8((s32)menu->firstList);
            }
        }
        mnuResetListNodeFadeCounters((s32)((MenuStaffList *)menu->firstList)->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE580);

void mnuDrawStaffCaption(s32 id, u8 *panel) {
    char buf[16];
    s32 handle;

    itfDrawGridWithResolvedSlot(0x1C0, 0xA10, 0, 0, ((MenuStaffContext *)panel)->spriteArg2, 2, 0x53);
    if (id != 0) {
        func_0035C860(buf, D_00437BD0, *(s16 *)(evtGetIndexedEventRecordId(id) * 0x38 + D_00435E20 + 0x18));
        handle = func_0019F5E8(0x620, 0xA20, 0, 0xA09DC380, (s32)buf, 0);
        frFontSetChainFlag(handle, 4);
        func_0019D550(handle, 1, 0x53);
        frFontQueueGlyphInSelectedSlot(handle);
    }
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE888);

void func_002AEA58(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, callback);
}

/* One 0x2138-byte page slot supplies the resource checked before page setup. */
typedef struct MenuStaffPanelSlot {
    u8 pad00[0xDC];
    s32 resourceHandle;
    u8 padE0[0x2058];
} MenuStaffPanelSlot;

s32 func_002AEAA0(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    u32 *window = &context->windowFlags;
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 index = **(s32 **)(context->selection + 0x1C);
    u8 *slot = (u8 *)context + index * 0x2138 + 0x2FC;

    mnuSelectPage(window, index);
    func_002AC408(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->unk6C, context->spriteArg0,
                         context->spriteArg2);
    func_002BC078(index, window, 0, 2);
    if (mdlFlagTest(0x990) != 0) {
        func_002BB9C8(((MenuStaffPanelSlot *)slot)->resourceHandle, 1);
    }
    context->panelHandle = mnuCreatePanelGroup(context->spriteArg0, context->spriteArg1, 0);
    context->spriteHandle = mnuCreateSpriteState(context->spriteArg0, context->spriteArg1, context->group);
    context->windowFlags |= 0x200;
    context->windowFlags &= ~0x80;
    func_002B2C88((s32)window, 1, 0, 0);
    menu->secondListReset = 0;
    func_002BAF50((s32)menu->secondList, (s32)context->tail);
    return 1;
}

/* Variant cleanup for the adjacent menu state; keep the same release ordering. */
s32 func_002AEC10(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    func_002BAF50((s32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    func_002AC660(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        func_002C1050((s32)((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void mnuStaffEntrySwapLabels(s32 context, u8 *entry, s32 target) {
    u8 *menu = ((MenuStaffContext *)context)->menu;
    s32 current = func_002C55C0(entry);

    func_002C1B68(context + 0xaa50, 1);
    if (target == 0) {
        func_0026C918(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
        func_0026C918(1, D_00435E5C + current * 0x19);
        dspStartEntry(6);
        ((MenuStaffChoices *)menu)->alternatePrevious = current;
        ((MenuStaffChoices *)menu)->alternateRequested = 0;
    } else if (current != target) {
        if (current != 0) {
            func_0026C918(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
            func_0026C918(1, D_00435E5C + current * 0x19);
            func_0026C918(2, D_00435E5C + target * 0x19);
            dspStartEntry(3);
        } else {
            func_0026C918(0, D_00435E48 + *(u16 *)(entry + 4) * 0x11);
            func_0026C918(1, D_00435E5C + target * 0x19);
            dspStartEntry(4);
        }
        ((MenuStaffChoices *)menu)->alternatePrevious = current;
        ((MenuStaffChoices *)menu)->alternateRequested = target;
    } else {
        func_0026C918(0, D_00435E5C + current * 0x19);
        dspStartEntry(5);
        ((MenuStaffChoices *)menu)->alternatePrevious = 0;
        ((MenuStaffChoices *)menu)->alternateRequested = 0;
    }
}

s32 func_002AEEA8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 changed = 0;
    u32 buttons = func_002C44E8(0x300);
    u8 *window = ((MenuStaffList *)menu->secondList)->window;
    s32 *node = ((MenuStaffWindow *)window)->cursor;
    s32 first;
    s32 count;
    s32 i;

    if (node != 0) {
        first = *node;
        count = ((MenuStaffWindow *)window)->rowCount;
    } else {
        first = 0;
        count = 0;
    }
    if (buttons & 0x100) {
        changed = 1;
        func_002AEC10(callback);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    if (buttons & 0x200 && changed == 0) {
        changed = 1;
        func_002AEC10(callback);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((MenuStaffContext *)context)->selection);
    if (changed == 0) {
        return 0;
    }
    func_002AEAA0(callback);
    if (first != 0 || count != 0) {
        mnuSeekListNode(first, (s32)((MenuStaffList *)menu->secondList)->window);
        if (count > 0) {
            for (i = count; i != 0; i--) {
                func_002B97D8((s32)menu->secondList);
            }
        }
        mnuResetListNodeFadeCounters((s32)((MenuStaffList *)menu->secondList)->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF020);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF2E0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACA0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACC8);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF5E0);

void func_002AF898(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, callback);
}

s32 func_002AF8E0(s32 unused) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    u32 *window = &context->windowFlags;
    MenuStaffChoices *menu = (MenuStaffChoices *)context->menu;
    s32 index = **(s32 **)(context->selection + 0x1C);
    u8 *slot = (u8 *)context + index * 0x2138 + 0x2FC;

    mnuSelectPage(window, index);
    func_002AC8F0(context);
    mnuReleaseStaffMenuResources(&context->group);
    mnuSetWindowResource(index, window, context->group, context->spriteArg1, context->unk6C, context->spriteArg0,
                         context->spriteArg2);
    func_002BC078(index, window, 0, 2);
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
    func_002BAF50((s32)menu->thirdList, (s32)context->tail);
    return 1;
}

/* Third menu-state cleanup uses the matching state-specific pre-release. */
s32 func_002AFA58(s32 unused) {
    s32 context = kwlnTaskGetUserValue();
    s32 entryList = context + 0x284;
    func_002BAF50((s32)((MenuStaffContext *)context)->activeWindow, context + 0xb10c);
    func_002ACA98(context);
    func_002B2C88(entryList, 0, 0, 0);
    mnuClearPageSelectionHandles(entryList);
    mnuClearEntries(entryList);
    if (((MenuStaffContext *)context)->panelHandle != 0) {
        mnuDestroyPanelGroup((s32)((MenuStaffContext *)context)->panelHandle);
        ((MenuStaffContext *)context)->panelHandle = 0;
    }
    if (((MenuStaffContext *)context)->spriteHandle != 0) {
        func_002C1050((s32)((MenuStaffContext *)context)->spriteHandle);
        ((MenuStaffContext *)context)->spriteHandle = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    mnuReleaseStaffMenuTextureHandles((s32)&((MenuStaffContext *)context)->group);
    return 1;
}

void func_002AFB38(s32 context, u8 *entry, s32 unused, s32 flag) {
    char buf[16];
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 current;
    s32 base;

    func_002C1B68(context + 0xaa50, 1);
    current = func_002C55C0(entry);
    func_0026C918(0, D_00435E5C + current * 0x19);
    func_0026C918(1, D_003E7400[menu->thirdListIndex]);
    func_0035C860(buf, D_00437BD8, menu->thirdListValue);
    func_0026C918(2, (s32)buf);
    base = func_002BDA50(current);
    func_0035C860(buf, D_00437BD8, func_002BDA78(current) - base);
    func_0026C918(3, (s32)buf);
    if (flag == 0) {
        dspStartEntry(9);
    } else {
        dspStartEntry(0xA);
    }
}

s32 func_002AFC58(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffChoices *menu = (MenuStaffChoices *)((MenuStaffContext *)context)->menu;
    s32 changed = 0;
    u32 buttons = func_002C44E8(0x300);
    u8 *window = ((MenuStaffList *)menu->thirdList)->window;
    s32 *node = ((MenuStaffWindow *)window)->cursor;
    s32 first;
    s32 count;
    s32 i;

    if (node != 0) {
        first = *node;
        count = ((MenuStaffWindow *)window)->rowCount;
    } else {
        first = 0;
        count = 0;
    }
    if (buttons & 0x100) {
        changed = 1;
        func_002AFA58(callback);
        mnuRetreatListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    if (buttons & 0x200 && changed == 0) {
        changed = 1;
        func_002AFA58(callback);
        mnuAdvanceListCursorDefault(((MenuStaffContext *)context)->selection);
    }
    mnuClearListFlagsOneAndTwo(((MenuStaffContext *)context)->selection);
    if (changed == 0) {
        return 0;
    }
    func_002AF8E0(callback);
    if (first != 0 || count != 0) {
        mnuSeekListNode(first, (s32)((MenuStaffList *)menu->thirdList)->window);
        if (count > 0) {
            for (i = count; i != 0; i--) {
                func_002B97D8((s32)menu->thirdList);
            }
        }
        mnuResetListNodeFadeCounters((s32)((MenuStaffList *)menu->thirdList)->window);
    }
    sndSetSequenceVolumePan(4, 0x7F, 0x3F);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFDD0);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFE18);

void mnuSetPanelItemsFromRow(MenuSceneConfig *cfg, s32 row) {
    char *slots = D_003E7207 + row * 0x1C;
    char *flags;
    s32 *entry;
    u32 last = 0;
    s32 i;

    for (i = 0; i < sizeof(D_003E7207); i++, slots++) {
        if (*slots) {
            last = i;
        }
    }
    entry = cfg->entries;
    flags = D_003E7202 + row * 0x1C;
    for (i = 4; i >= 0; i--, flags++) {
        func_002C2AA8(*entry++, *flags ? last : 0);
    }
}

void func_002B0228(MenuSceneConfig *object) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_002C2AA8(object->entries[i], 0);
    }
}

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD0);

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD8);

