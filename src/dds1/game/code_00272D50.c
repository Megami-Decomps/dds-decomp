#include "common.h"

extern s32 func_00273750(s32, s32);

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
    func_0027C430(resources->firstWindow);
    func_0027C430(resources->secondWindow);
}

extern void func_0027C6A0(s32);

/* Refresh the bullet-item row and report whether the target row still has
 * remaining item count. */
s32 func_00273050(s32 arg0, s32 arg1) {
    u8 *ctx = *(u8 **)(arg1 + 0x90C);

    if (*(u8 *)((arg0 & 0xFFFF) + D_003BAA00 + 0x12A0) == 0) {
        func_0027C6A0(*(s32 *)(ctx + 8));
    }
    return *(u32 *)(*(s32 *)(*(s32 *)(ctx + 8) + 0x14) + 0x20) > 0;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002730A0);

void mnuReleaseStaffExtraWindow(StaffDisplayContext *context) {
    func_0027C430(context->resources->thirdWindow);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273220);

void func_00273390(u32 context) {
    mnuSetStaffDisplayMode(2, context);
}

void func_002733B0() {
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002733B8);

s32 freeStaffDisplayResources(void) {
    StaffDisplayContext *context = (StaffDisplayContext *)func_00101A70();
    StaffWindowResources *resources = context->resources;
    mnuReleaseStaffPrimaryWindows(context);
    func_002733B0(context);
    func_002D0918(resources->allocation);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002734C0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273670);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 mnuStaffRunPanel2b(u64 request) {
    s32 state = func_00101A70();

    return menuRunPanel(state, 2, request);
}

extern s32 battleItemApplyDirectEffect(s32, s32, s32, s32);
extern s32 skillApplyFieldUseEffect(s32, s32, s32, s32);
extern s32 func_0011A568(s32);
extern void func_00119900(s32, s32);
extern void initPartyPanelSlots(s32);
extern void menuUpdateHandleStates(s32);
extern void func_00280048(s32);

/* Use a field item: resolve its direct effect (or field-use skill) against the
 * active unit row; on success consume one from the inventory and refresh the
 * party panels. Returns 1 when the item was consumed. */
s32 func_00273750(s32 item, s32 context) {
    s32 panel = context + 0x15C;
    s32 unit = D_003BAA00 + *(s32 *)(*(s32 *)(*(s32 *)(context + 0x7D8) + 0x1C)) * 0x1A4 + 0xA60;
    s32 result = battleItemApplyDirectEffect(panel, item & 0xFFFF, unit, unit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (skillApplyFieldUseEffect(panel, func_0011A568(item) & 0xFFFF, unit, unit) == 0) {
            return 0;
        }
    }
    func_00119900(item, -1);
    initPartyPanelSlots(context + 0x7EC);
    menuUpdateHandleStates(panel);
    func_00280048(panel);
    return 1;
}

void func_00273838(s32 selection, s32 context) {
    StaffWindowResources *resources;
    s32 active;

    resources = ((StaffDisplayContext *)context)->resources;
    active = func_00273750(selection, context);
    if (active != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(resources->firstWindow + 0x14) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(selection + D_003BAA00 + 0x12a0);
        resources->selection = selection;
    }
    func_00283BF0(context + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273A30);
