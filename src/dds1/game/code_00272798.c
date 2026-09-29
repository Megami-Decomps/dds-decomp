#include "common.h"

extern s32 func_00101A70();

extern s64 func_002913B8(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern u8 D_0037C844[];

extern void func_002858F8();

INCLUDE_ASM(const s32, "game/code_00272798", func_00272798);

INCLUDE_ASM(const s32, "game/code_00272798", func_002728F8);

/* Submit a request to the active menu dispatcher in mode 2. */
void func_002729C8(s32 request) {
    s32 context = func_00101A70();
    func_00285670(context + 8, context + 0x54, 2, request);
}

s32 mnuStartStaffDisplay(void) {
    u8 *context = (u8 *)func_00101A70();
    mnuSetStaffDisplayMode(5, context);
    func_0027E790(*(u32 *)(context + 0x138), *(u32 *)(context + 0x114), 0, 1);
    configTasksCreate(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x6C. */
u32 func_00272A58(void) {
    s32 context;

    context = func_00101A70();
    func_0027E790(*(u32 *)(context + 0x138), *(u32 *)(context + 0x6c), 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s64 func_00272A90(s32 callback) {
    s32 context = func_00101A70();
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s64 state = func_00285670(context + 8, dispatchEntry, 0, callback);
    if (state == 0) {
        if (func_002913B8() == 0) {
            func_002858F8(dispatchEntry, D_0037C844);
        }
        return 0;
    }
    return state;
}

INCLUDE_ASM(const s32, "game/code_00272798", func_00272B00);

void func_00272B80(s32 request) {
    s32 context = func_00101A70();
    func_00285670(context + 8, context + 0x54, 2, request);
}

u32 func_00272BB8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00272798", func_00272BC0);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6C8);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D0);

INCLUDE_SDATA(const s32, "game/code_00272798", D_003BC6D8);

