#include "mnu.h"

extern s32 mnuUseStaffItem(s32, s32);

extern s32 func_00101A70();

extern s32 D_003BAA00;

extern s64 func_00285670(s32, s32 *, u64, u64);

typedef struct {
    u32 allocation; /* 0x00 */
    u32 pad04;       /* 0x04 */
    u32 firstWindow; /* 0x08 */
    u32 secondWindow; /* 0x0C */
    u32 thirdWindow; /* 0x10 */
    u8 pad14[0x14];
    s32 selection; /* 0x28 */
} StaffWindowResources;

typedef struct {
    u8 pad00[0x90C];
    StaffWindowResources *resources; /* 0x90C */
} StaffDisplayContext;


INCLUDE_ASM(const s32, "game/code_00272D50", func_00272D50);

void mnuReleaseStaffPrimaryWindows(StaffDisplayContext *context) {
    StaffWindowResources *resources;

    resources = context->resources;
    mnuDestroyWindowContainer(resources->firstWindow);
    mnuDestroyWindowContainer(resources->secondWindow);
}

extern void func_0027C6A0(s32);

typedef struct StaffWindowCounter {
    u8 pad00[0x20];
    u32 remaining;           /* 0x20 */
} StaffWindowCounter;

typedef struct StaffWindowHeader {
    u8 pad00[0x14];
    StaffWindowCounter *data; /* 0x14 */
} StaffWindowHeader;

/* Refresh the bullet-item window if inventory is empty; return whether the
 * window still has entries. */
s32 func_00273050(s32 itemId, s32 context) {
    StaffWindowResources *resources = ((StaffDisplayContext *)context)->resources;

    if (*(u8 *)((itemId & 0xFFFF) + D_003BAA00 + 0x12A0) == 0) {
        func_0027C6A0(resources->firstWindow);
    }
    return ((StaffWindowHeader *)resources->firstWindow)->data->remaining > 0;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002730A0);

void mnuReleaseStaffExtraWindow(StaffDisplayContext *context) {
    mnuDestroyWindowContainer(context->resources->thirdWindow);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273220);

void func_00273390(u32 context) {
    mnuSetStaffDisplayMode(2, context);
}

void func_002733B0() {
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002733B8);

s32 mnuStaffFreeDisplayResources(void) {
    StaffDisplayContext *context = (StaffDisplayContext *)func_00101A70();
    StaffWindowResources *resources = context->resources;
    mnuReleaseStaffPrimaryWindows(context);
    func_002733B0(context);
    func_002D0918(resources->allocation);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002734C0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273670);

s64 mnuStaffRunPanel2b(u64 request) {
    s32 state = func_00101A70();

    return menuRunPanel(state, 2, request);
}

extern s32 btlItemApplyDirectEffect(s32, s32, s32, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern s32 func_0011A568(s32);
extern void func_00119900(s32, s32);
extern void mnuInitPartyPanelSlots(s32);
extern void mnuUpdateHandleStates(s32);
extern void func_00280048(s32);

typedef struct StaffSelectionList {
    u8 pad00[0x1C];
    s32 *selectedIndex;         /* 0x1C */
} StaffSelectionList;

typedef struct StaffItemContext {
    u8 pad00[0x7D8];
    StaffSelectionList *list;   /* 0x7D8 */
} StaffItemContext;

/* Use a field item: resolve its direct effect (or field-use skill) against the
 * active unit row; on success consume one from the inventory and refresh the
 * party panels. Returns 1 when the item was consumed. */
s32 mnuUseStaffItem(s32 itemId, s32 context) {
    s32 partyPanel = context + 0x15C;
    s32 targetUnit = D_003BAA00 + *(((StaffItemContext *)context)->list->selectedIndex) * 0x1A4 + 0xA60;
    s32 result = btlItemApplyDirectEffect(partyPanel, itemId & 0xFFFF, targetUnit, targetUnit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (ptySkillApplyFieldUseEffect(partyPanel, func_0011A568(itemId) & 0xFFFF, targetUnit, targetUnit) == 0) {
            return 0;
        }
    }
    func_00119900(itemId, -1);
    mnuInitPartyPanelSlots(context + 0x7EC);
    mnuUpdateHandleStates(partyPanel);
    func_00280048(partyPanel);
    return 1;
}

void mnuRefreshStaffItemSelection(s32 selection, s32 context) {
    StaffWindowResources *resources;
    s32 consumed;

    resources = ((StaffDisplayContext *)context)->resources;
    consumed = mnuUseStaffItem(selection, context);
    if (consumed != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(resources->firstWindow + 0x14) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(selection + D_003BAA00 + 0x12a0);
        resources->selection = selection;
    }
    func_00283BF0(context + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273A30);
