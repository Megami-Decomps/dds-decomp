#include "mnu_input.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "eff_resource_list.h"
#include "common.h"
#include "fr_font.h"
#include "itf_draw_grid.h"
#include "mnu_staff.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "mnu.h"
#include "eff.h"
#include "mnu_shop.h"
#include "mnu_camp_work.h"
#include "mnu_list.h"
#include "kwln_task_lifecycle.h"

extern void mnuDestroyWindowContainer(MenuWindowContainer *);

extern void mnuReleaseResourceList(MenuPanelHandles *);

extern s32 kwlnFadeIsActive(void);

extern char mnuCampInputTaskName[]; /* "camp" */

extern char mnuCampDrawTaskName[]; /* "camp_draw" */

extern char mnuCampOwnerTaskName[]; /* "camp_update" */

extern s8 mnuCampTaskState;

extern void *const D_0037B950[];

extern void *const D_0037B970[];

extern void *const D_0037B980[];

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern const s32 D_0037C388[];

#define MNU_STAFF_PANEL_COUNT 3

#define MNU_CAMP_STATE_IDLE 0

#define MNU_CAMP_STATE_ACTIVE 1

#define MNU_CAMP_STATE_CLEANED_UP 2

#define MNU_CAMP_CANCEL_MASK 8

#define MNU_CAMP_OPEN_FADE_FRAMES 0xF

extern s8 D_003BC6B5;

/* Filter staff entries, size the window and preserve each entry's original ordinal. */ MenuWindowContainer *mnuCreateFilteredStaffEntryWindow(void *const *entries, s32 count, s32 width, StaffMenuWork *work, const s32 *flagIds);

s64 mnuReleaseStaffResourceSlotGroups(StaffMenuWork *menu);

/* Destroy the mapped images followed by both status batches. */ void mnuReleaseStaffSpriteHandles(StaffMenuWork *menu);

void mnuCreateStaffPanelSet(StaffMenuWork *menu) {
    menu->resourceList = mnuCreatePanelSpriteHandles(
        0, menu->staffSlots.baseResources[3], menu->secondaryImage);
    menu->images[0] = mnuCreateFilteredStaffEntryWindow(D_0037B950, 8, 0x300, menu, D_0037C388);
    mnuForwardDupArg(menu->images[0], (s32)menu->staffSlots.baseResources[5], 0, 0, 0);
    menu->images[1] = mnuCreateFilteredStaffEntryWindow(D_0037B970, 3, 0x2C0, menu, 0);
    mnuSetWindowFadeScale(menu->images[1], 0x100);
    menu->images[2] = mnuCreateFilteredStaffEntryWindow(D_0037B980, 2, 0x200, menu, 0);
    mnuSetWindowFadeScale(menu->images[2], 0x100);
}

/* Destroy the menu windows, then release their associated resource list. */
void mnuReleaseStaffSpriteAndResourceHandles(StaffMenuWork *menu) {
    MenuWindowContainer **windowCursor = menu->images;
    u32 windowIndex = 0;
    do {
        mnuDestroyWindowContainer(*windowCursor++);
    } while (++windowIndex < MNU_STAFF_PANEL_COUNT);
    mnuReleaseResourceList(menu->resourceList);
}

extern void *memset(void *, s32, u32);

extern s8 dds3AdminReadPreviousSignedSample(void);

extern EffectList *mnuAllocateValueRecord(u32);

extern void func_0027AD80(void *);

extern void evtCreateMessageWindowIfMissing(s32);

extern char D_0037B9E0[];

extern void func_00271368(void *);

extern void func_002717D8(void *);

extern void mnuResetGradientFadeColor(void *, s32);

extern void func_002E9708(void);

/* Allocate and clear menu work, select its request-list mode from the
 * previous sample, then initialize the owned UI and resource state. */
StaffMenuWork *mnuCreateStaffCampWork(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(sizeof(StaffMenuWork));
    StaffMenuWork *menu = (StaffMenuWork *)sdfResourceRetainAddress(allocation);

    memset(menu, 0, sizeof(*menu));
    menu->resource = allocation;
    mnuClearPanelTransitionState(&menu->panel);
    /* Mode also selects the normal or alternate staff image table. */
    if (dds3AdminReadPreviousSignedSample() != 0) {
        menu->resourceQueue = mnuAllocateValueRecord(1);
    } else {
        menu->resourceQueue = mnuAllocateValueRecord(0);
    }
    mnuInitPartyPanelSlots(&menu->partyPanel);
    func_0027AD80(menu->background);
    evtCreateMessageWindowIfMissing((s32)D_0037B9E0);
    func_00271368(menu);
    func_002717D8(menu);
    mnuResetGradientFadeColor(menu->timer, 0x40);
    func_002E9708();
    return menu;
}

extern s8 mnuCampTaskState;

/* Ignore null task userdata; otherwise drain transitions and release owned
 * resources in shutdown order before marking camp cleanup complete. */
void mnuDestroyStaffMenuTask(KwlnTask *task) {
    StaffMenuWork *menu = (StaffMenuWork *)kwlnTaskGetUserValue(task);
    if (menu == NULL) {
        return;
    }
    mnuDrainPanelTransitions(&menu->panel, task);
    mnuReleaseStaffSpriteAndResourceHandles(menu);
    mnuDestroyScrollPanel(menu->scrollPanel);
    mnuShutdownContext((MenuPageWindow *)(menu->background + 0x20));
    dspCloseChannel();
    mnuReleaseAssets(menu->background);
    mnuReleaseStaffResourceSlotGroups(menu);
    mnuReleaseStaffSpriteHandles(menu);
    effDestroyEffectList(menu->resourceQueue);
    sdfReleaseResourceAllocation(menu->resource);
    mnuCampTaskState = MNU_CAMP_STATE_CLEANED_UP;
    func_002E9730();
}

u32 func_00271FC8(KwlnTask *task) {
    s32 context;

    context = kwlnTaskGetUserValue(task);
    mnuDrawAndStepGradientFade(context + 0x914, 0x53);
    return 0;
}

extern void mnuPlayInputSound(s32, s32, u32 *);

/* Mapped cancel input closes camp only through the full guard chain.
 * Return -1 on closure, otherwise 0; blocked nested guards use the alternate sound. */
s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 mappedButtons = mnuMapPadMaskToFlags(MNU_CAMP_CANCEL_MASK);
    s32 result;

    if (func_002719F0(menu) == 0) {
        return 0;
    }
    result = 0;
    if (evtGetMessageWindowControlState() == 0) {
        if (mappedButtons & MNU_CAMP_CANCEL_MASK) {
            if (func_002877A8() != 1) {
                if (fileConsumeConfigTaskReady() == 0) {
                    mnuDestroyCampTasks();
                    mnuPlayInputSound(0, 2, 0);
                    return -1;
                }
            }
            mnuPlayInputSound(0, 0x8000, 0);
        }
    }
    return result;
}

extern void mnuHandleCampFieldSkillInput();

extern void func_002728F8();

extern s32 func_002729C8(s32);

extern void func_00101A80(s32, s32);

extern void kwlnFadeOutStart(s32, s32, s32, s32);

/* Share menu userdata across the input/draw/owner tasks, attach fade and cancel
 * tasks to drawing, then start the opening fade and mark camp active. */
void mnuCreateCampTasks(void) {
    s32 menuAddress;
    s32 drawTask;

    menuAddress = (s32)mnuCreateStaffCampWork();
    kwlnTaskCreate(mnuCampInputTaskName, 0x3F2, 1, 0, mnuHandleCampFieldSkillInput, 0, menuAddress);
    drawTask = kwlnTaskCreate(mnuCampDrawTaskName, 0x2B07, 1, 0, func_002728F8, 0, menuAddress);
    kwlnTaskCreate(mnuCampOwnerTaskName, 0x520B, 1, 0, func_002729C8, mnuDestroyStaffMenuTask, menuAddress);
    func_00101A80(drawTask, kwlnTaskCreate("camp_fade", 0x2B08, 1, 0, func_00271FC8, 0, menuAddress));
    func_00101A80(drawTask, kwlnTaskCreate("camp_all_cancel", 0x3F3, 1, 0, mnuStaffCampCancelCheck, 0, menuAddress));
    kwlnFadeOutStart(0, 0, 0, MNU_CAMP_OPEN_FADE_FRAMES);
    mnuCampTaskState = MNU_CAMP_STATE_ACTIVE;
}

/* Destroy the three named camp task hierarchies; owner teardown frees userdata. */
void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(mnuCampInputTaskName, 0);
    kwlnTaskDestroyWithHierarchyByName(mnuCampDrawTaskName, 0);
    kwlnTaskDestroyWithHierarchyByName(mnuCampOwnerTaskName, 0);
}

/* Report active camp; acknowledge cleanup only for the exact cleaned-up state.
 * Other non-active states return 0 without being rewritten. */
s32 mnuAcknowledgeCampState(void) {
    s8 campState = mnuCampTaskState;
    if (campState == MNU_CAMP_STATE_ACTIVE) {
        return 1;
    }
    if (campState < MNU_CAMP_STATE_CLEANED_UP) {
        return 0;
    }
    if (campState == MNU_CAMP_STATE_CLEANED_UP) {
        mnuCampTaskState = MNU_CAMP_STATE_IDLE;
    }
    return 0;
}

/* Return 1 exactly when the kernel reports no active fade. */
u8 mnuIsFadeIdle(void) {
    s64 fadeActive;

    fadeActive = kwlnFadeIsActive();
    return fadeActive == 0;
}

extern s32 func_003014F0(char *, const char *, s32);

extern s32 func_00197A98(s32, s32, s32, u32, u32, s32);

typedef struct FrFontGlyph FrFontGlyph;

extern s32 frFontDrawGlyphChain(FrFontGlyph *, s8, u32);

extern char D_003BC6C0[];

void mnuDrawStaffCampSlotsAndCurrency(s32 unused0, s32 unused1, s32 z, s32 firstSlot,
                   s32 secondSlot, s32 drawFlags, s32 unused6, s32 unused7) {
    char text[0x10];
    s32 glyph;

    itfDrawGridWithResolvedSlot(0x150, 0xD08, 0, 1, (EffectSlotSet *)(u32)firstSlot, 0, drawFlags);
    itfDrawGridWithResolvedSlot(0x2B0, 0xCE8, 0, 1, (EffectSlotSet *)(u32)secondSlot, 3, drawFlags);
    func_003014F0(text, D_003BC6C0, datGameState->header.currency);
    glyph = func_00197A98(0x4B0, 0xCD8, z, 0x80808080, (u32)text, 0);
    frFontDrawGlyphChain((FrFontGlyph *)glyph, 1, drawFlags);
    frFontQueueGlyphForCurrentDrawBuffer((FrFontGlyph *)glyph);
}

extern u8 *D_0037B988[];

/* Create and queue the table-selected image sprite; imageIndex is unchecked. */
void mnuCreateStaffImageSprite(s32 imageIndex) {
    FrFontGlyph *sprite = (FrFontGlyph *)itfCreateConvertedTextGlyph(0x2F0, 0x1E0, 0, 0xa09dc35a,
                                      D_0037B988[imageIndex], 0);
    frFontDrawGlyphChain(sprite, 1, 0x54);
    frFontQueueGlyphForCurrentDrawBuffer(sprite);
}

typedef struct StaffGridLabelRow {
    s32 count;
    s32 x[5];
    s8 gridIds[5];
    u8 pad1D[3];
} StaffGridLabelRow;

typedef char StaffGridLabelRow_size_check[sizeof(StaffGridLabelRow) == 0x20 ? 1 : -1];

typedef char StaffGridLabelRow_gridIds_offset_check[
    ((u32)&((StaffGridLabelRow *)0)->gridIds == 0x18) ? 1 : -1];

extern const StaffGridLabelRow D_003B2100[];

void mnuDrawStaffGridLabelsForKind(s32 kind, struct EffectSlotSet *resources) {
    StaffGridLabelRow rows[7];
    s32 i;
    memcpy(rows, D_003B2100, sizeof(rows));
    for (i = 0; i < rows[kind].count; i++) {
        if (rows[kind].x[i] != 0) {
            itfDrawGridWithResolvedSlot(rows[kind].x[i], 0xCF0, 0, 1,
                                       resources, rows[kind].gridIds[i], 0x53);
        }
    }
}

extern void frFontSetChildColors(struct FrFontGlyph *, u32);

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, s32, s32);

void func_00272518(s32 kind, s32 labelIndex, s32 textTable, s32 context,
                   s32 drawOption, s32 textOption, s32 layer) {
    StaffMenuWork *menu = (StaffMenuWork *)context;
    s32 glyph;
    if (kind == 0) {
        itfDrawGridWithResolvedSlot(0x1C0, 0xA20, 0, drawOption, menu->staffSlots.baseResources[5], 0x1E, layer);
    } else {
        itfDrawGridWithResolvedSlot(0x1C0, 0xA20, 0, drawOption, menu->staffSlots.baseResources[5], 0x2E, layer);
    }
    itfDrawGridWithResolvedSlot(0x150, 0x9C0, 0, drawOption, menu->staffSlots.baseResources[5], 0, layer);
    itfDrawGridWithResolvedSlot(0x280, 0x9A0, 0, drawOption, menu->staffSlots.baseResources[1], 2, layer);
    if (textTable != 0) {
        glyph = itfDrawBankTextWithLayoutFlags(0x2C0, 0xA70, 0, labelIndex, textTable, textOption);
        frFontSetChildColors((struct FrFontGlyph *)(u32)glyph, 0xA09DC366);
        frFontDrawGlyphChain((FrFontGlyph *)glyph, 0, layer);
        frFontQueueGlyphForCurrentDrawBuffer((FrFontGlyph *)glyph);
    }
}

void func_00272668(s32 kind, s32 labelIndex, s32 textTable, s32 context, s32 drawOption, s32 layer) {
    func_00272518(kind, labelIndex, textTable, context, drawOption, 0, layer);
}

void mnuDrawStaffCampScreen(s32 kind, KwlnTask *task) {
    StaffMenuWork *menu = (StaffMenuWork *)kwlnTaskGetUserValue(task);

    mnuDrawBackdrop((MenuAssets *)menu->background, 0x20);
    if (func_002719F0(task) == 0) {
        return;
    }
    func_0027E8D8(-0x10, -8, 0, (s32)menu->scrollPanel, 0x53);
    mnuDrawPanelListDefault(0, 0, 0, (MenuPageWindow *)((u8 *)menu + 0x15C), 0x53);
    if (kind == 0) {
        itfDrawGridWithResolvedSlot(0x1AB0, 0x70, 0, 1, menu->staffSlots.baseResources[1], 6, 0x53);
        itfDrawGridWithResolvedSlot(0x17A0, 0x78, 0, 1, menu->staffSlots.baseResources[0], 0xF, 0x53);
        itfDrawGridWithResolvedSlot(0x1E40, 0x78, 0, 1, menu->staffSlots.baseResources[0], 0x10, 0x53);
    }
}

void func_00272778(u32 task) {
    mnuDrawStaffCampScreen(0, task);
}

INCLUDE_RODATA(const s32, "game/code_00271D30", D_003B2100);

INCLUDE_SDATA(const s32, "game/code_00271D30", D_003BC6C0);

