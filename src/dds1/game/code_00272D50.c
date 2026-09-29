#include "common.h"

extern s64 func_00273750(void);

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

void func_00273020(StaffDisplayContext *context) {
    StaffWindowResources *resources;

    resources = context->resources;
    func_0027C430(resources->firstWindow);
    func_0027C430(resources->secondWindow);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273050);

INCLUDE_ASM(const s32, "game/code_00272D50", func_002730A0);

void func_00273200(StaffDisplayContext *context) {
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
    func_00273020(context);
    func_002733B0(context);
    func_002D0918(resources->allocation);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002734C0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273670);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 func_00273718(u64 request) {
    s32 state = func_00101A70();

    return menuRunPanel(state, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273750);

void func_00273838(s32 selection, s32 context) {
    StaffWindowResources *resources;
    s64 active;

    resources = ((StaffDisplayContext *)context)->resources;
    active = func_00273750();
    if (active != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(resources->firstWindow + 0x14) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(selection + D_003BAA00 + 0x12a0);
        resources->selection = selection;
    }
    func_00283BF0(context + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00272D50", func_002738A0);

INCLUDE_ASM(const s32, "game/code_00272D50", func_00273A30);
