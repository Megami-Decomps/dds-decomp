#include "mnu.h"

extern s32 mnuUseStaffItem(s32, s32);

extern s32 kwlnTaskGetUserValue();

extern s32 datGameState;

extern s64 func_00285670(s32, s32 *, u64, u64);
extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern void *memset(void *, s32, u32);
extern void func_00272D50(s32);
extern void mnuForwardDupArg(s32, s32, s32, s32, s32);
extern void mnuActivatePanelAndConfigureGridResources(s32, s32, s32, s32);
extern s32 mnuSeekListNode(s32, s32);

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
    u8 pad00[0x74];
    s32 unk74;  /* 0x74 */
    u8 pad78[0x60];
    s32 unkD8;  /* 0xD8 */
    u8 padDC[0x4C];
    s32 unk128; /* 0x128 */
    u8 pad12C[0xC];
    s32 unk138; /* 0x138 */
    u8 pad13C[0x7D0];
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
s32 mnuIsStaffWindowReadyForItem(s32 itemId, s32 context) {
    StaffWindowResources *resources = ((StaffDisplayContext *)context)->resources;

    if (*(u8 *)((itemId & 0xFFFF) + datGameState + 0x12A0) == 0) {
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

s32 mnuInitializeStaffDisplayResources(void) {
    StaffDisplayContext *context;
    StaffWindowResources *resources;
    s32 handle;

    context = (StaffDisplayContext *)kwlnTaskGetUserValue();
    handle = sdfAllocGeneralBlock(0x2C);
    resources = sdfResourceRetainAddress(handle);
    context->resources = resources;
    memset(resources, 0, 0x2C);
    resources->allocation = handle;
    func_00273390((u32)context);
    func_00272D50((s32)context);
    mnuForwardDupArg(context->unk128, context->unk74, 0, 0, 0);
    mnuActivatePanelAndConfigureGridResources(context->unk138, context->unkD8, 0, 1);
    mnuSeekListNode(0, *(s32 *)(context->unk128 + 0x14));
    return 1;
}

s32 mnuStaffFreeDisplayResources(void) {
    StaffDisplayContext *context = (StaffDisplayContext *)kwlnTaskGetUserValue();
    StaffWindowResources *resources = context->resources;
    mnuReleaseStaffPrimaryWindows(context);
    func_002733B0(context);
    sdfReleaseResourceAllocation(resources->allocation);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002734C0);

extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, s32, s32);
extern void func_002723B0(s32, s32);
extern u8 D_0037C860[];

typedef struct StaffImageNode {
    s32 label;
} StaffImageNode;

typedef struct StaffImageWindow {
    u8 pad00[0x1C];
    StaffImageNode *selectedNode; /* 0x1C */
} StaffImageWindow;

typedef struct StaffImageList {
    u8 pad00[0x14];
    StaffImageWindow *window; /* 0x14 */
} StaffImageList;

typedef struct StaffImageContext {
    u8 pad00[0x78];
    s32 group; /* 0x78 */
    u8 pad7C[0xAC];
    StaffImageList *activeWindow; /* 0x128 */
} StaffImageContext;

s64 mnuPrepareStaffImageAndSelectionLabelB(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_00272778(callback);
    mnuCreateStaffImageSprite(4);
    func_00272668(1, ((StaffImageContext *)context)->activeWindow->window->selectedNode->label, (s32)D_0037C860, context, 1, 0x53);
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)((StaffImageContext *)context)->activeWindow, 0x53);
    func_002723B0(0, ((StaffImageContext *)context)->group);
    return menuRunPanel(context, 1, callback);
}

s64 mnuStaffRunPanel2b(u64 request) {
    s32 state = kwlnTaskGetUserValue();

    return menuRunPanel(state, 2, request);
}

extern s32 btlItemApplyDirectEffect(s32, s32, s32, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern s32 evtGetIndexedEventRecordId(s32);
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
    s32 targetUnit = datGameState + *(((StaffItemContext *)context)->list->selectedIndex) * 0x1A4 + 0xA60;
    s32 result = btlItemApplyDirectEffect(partyPanel, itemId & 0xFFFF, targetUnit, targetUnit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (ptySkillApplyFieldUseEffect(partyPanel, evtGetIndexedEventRecordId(itemId) & 0xFFFF, targetUnit, targetUnit) == 0) {
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
                  (u32)*(u8 *)(selection + datGameState + 0x12a0);
        resources->selection = selection;
    }
    func_00283BF0(context + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273A30);
