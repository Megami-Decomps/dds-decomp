#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"
#include "eff.h"

typedef struct FrFontGlyph FrFontGlyph;
extern u32 func_001978E8(s32, s32, s32, u32, char *, s32);
extern void frFontSetChainFlag(FrFontGlyph *, u8);
extern s32 func_001958A0(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern s32 func_003014F0(char *, const char *, ...);
extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern char D_003BC6C8[];
extern const char D_003BC6D0[];
extern char D_003BC6D8[];

/* The staff/camp constructor allocates and clears this complete owner. */
typedef struct StaffMenuWork {
    u32 resource;
    u8 pad04[4];
    MenuPopupState panel;
    u8 pad54[8];
    EffectList *resourceQueue;
    StaffSlots staffSlots;
    u32 categoryPair[2];
    u32 categoryGroup[4];
    u32 partyModels[9];
    u32 singleResource;
    u32 primaryImage;
    MenuPanelHandles *resourceList;
    u32 secondaryImage;
    u32 images[3];
    u32 extraImages[2];
    u32 scrollPanel;
    u8 background[0x6B0];
    u8 partyPanel[0x124];
    s32 displayMode;
    u8 timer[0x10];
} StaffMenuWork;

typedef char StaffMenuWork_size_must_be_0x924[(sizeof(StaffMenuWork) == 0x924) ? 1 : -1];
typedef char StaffMenuWork_staffSlots_offset_check[
    ((u32)&((StaffMenuWork *)0)->staffSlots == 0x60) ? 1 : -1];

extern s32 kwlnTaskGetUserValue();

extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_0037C844[];

extern void mnuSetPopupEntryFlagged();

extern void mnuDrawBackdrop(s32, s32);

extern void func_0027E8D8(s32, s32, s32, u32, s32);

extern void mnuCreateStaffImageSprite(s32);

extern void func_002723B0(s32, u32);
extern u32 mnuMapPadMaskToFlags(s32);
extern s32 func_002719F0(s32);
extern void mnuSetPopupEntry(s32, s32);
extern void func_0027C788(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_0037C748[];





/* Staff-display fields of CampMenuContext (code_00274B80). */
typedef struct StaffScreenContext {
    u8 pad00[8];
    u8 dispatchState[0x4C];
    s32 popup;
    u8 pad58[0x14];
    u32 displayVariant; /* 0x6C */
    u8 pad70[8];
    u32 actor;          /* 0x78 */
    u8 pad7C[0x98];
    u32 staffResource;  /* 0x114 */
    u8 pad118[0xC];
    MenuWindowContainer *skillPanel;
    u8 pad128[0x10];
    u32 display;        /* 0x138 */
} StaffScreenContext;


s32 mnuHandleCampFieldSkillInput(s32 callback) {
    StaffScreenContext *context;
    s32 *popup;
    u32 input;
    s32 state;

    context = (StaffScreenContext *)kwlnTaskGetUserValue();
    input = mnuMapPadMaskToFlags(0x33);
    popup = &context->popup;
    state = func_00285670(context->dispatchState, popup, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    state = func_002719F0(callback);
    if (state == 0) {
        return state;
    }
    if (*popup == 0) {
        if (input & 1) {
            struct MenuListNode *entry = context->skillPanel->list->cursor;

            if ((entry->flags48 & 1) == 0) {
                u32 index = entry->sortKeyPrimary + 1;

                mnuSetPopupEntry((s32)popup, (s32)(D_0037C748 + index * 0x1C));
            } else {
                input = 0x8000;
            }
        }
        if (input & 2) {
            mnuSetPopupEntry((s32)popup, (s32)D_0037C748);
        }
    }
    if ((input & 0x300000) == 0) {
        func_0027C788(context->skillPanel);
    }
    if (input & 0x10) {
        mnuRetreatWindowListSelection(context->skillPanel);
    }
    if (input & 0x20) {
        mnuAdvanceWindowListSelection(context->skillPanel);
    }
    mnuClearWindowPanelTransitionFlag(context->skillPanel);
    mnuPlayInputSound(0, input, &context->skillPanel->list->stateFlags);
    return 0;
}
INCLUDE_ASM(const s32, "game/code_00272798", func_002728F8);

/* Submit a request to the active menu dispatcher in mode 2. */
s32 func_002729C8(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)request);
}

s32 mnuStartStaffDisplay(void) {
    u8 *context = (u8 *)kwlnTaskGetUserValue();
    mnuSetStaffDisplayMode(5, context);
    mnuActivatePanelAndConfigureGridResources(((StaffScreenContext *)context)->display, ((StaffScreenContext *)context)->staffResource, 0, 1);
    mnuCreateConfigTasks(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x6C. */
u32 mnuConfigureCampDrawContextPanel(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuActivatePanelAndConfigureGridResources(((StaffScreenContext *)context)->display, ((StaffScreenContext *)context)->displayVariant, 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s32 state = menuRunPanel((void *)context, 0, (void *)callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            mnuSetPopupEntryFlagged(dispatchEntry, D_0037C844);
        }
        return 0;
    }
    return state;
}

s32 mnuDrawStaffImageScreen(s32 callback) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuDrawBackdrop(context + 0x13C, 0x20);
    func_0027E8D8(-0x10, -8, 0, ((StaffScreenContext *)context)->display, 0x54);
    mnuCreateStaffImageSprite(0x14);
    func_002723B0(2, ((StaffScreenContext *)context)->actor);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 func_00272B80(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)request);
}

u32 func_00272BB8(void) {
    return 1;
}

/* Draw the item quantity when present, otherwise show both staff-item slots. */
void func_00272BC0(s32 x, s32 y, s32 z, struct MenuList *list,
                  struct MenuListNode *node, s32 drawArg) {
    StaffMenuWork *context = (StaffMenuWork *)list->context;
    u32 itemId = node->sortKeySecondary;
    u8 quantity = datGameState->inventory.counts[itemId];
    u32 color = 0xA09DC380;

    if (quantity != 0) {
        FrFontGlyph *label;
        FrFontGlyph *count;
        char buffer[0x20];

        if (node->flags48 & 1) {
            color = 0xA09DC340;
        }
        label = (FrFontGlyph *)func_001978E8(x + 0x810, y + 0x10, z, color,
                                          D_003BC6C8, 0);
        frFontSetChainFlag(label, 0);
        func_003014F0(buffer, D_003BC6D0,
                       datGameState->inventory.counts[node->sortKeySecondary]);
        count = (FrFontGlyph *)func_001978E8(x + 0x910, y + 0x20, z, color,
                                          buffer, (s32)label);
        func_001958A0(count, 1, drawArg);
        frFontQueueGlyphInSelectedSlot(count);
    } else {
        s32 selected = (list->cursor == node);

        node->value = D_003BC6D8;
        itfDrawGridWithResolvedSlot(x + 0x80, y + 0x30, z, 1,
                                   (s32)context->staffSlots.baseResources[2], selected + 9, drawArg);
        itfDrawGridWithResolvedSlot(x + 0x7F0, y + 0x30, z, 1,
                                   (s32)context->staffSlots.baseResources[2], selected + 0xB, drawArg);
    }
}

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6C8);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D0);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D8);

