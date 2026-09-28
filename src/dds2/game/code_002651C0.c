#include "common.h"

extern s32 mdlFlagTest(u32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_00101958();

extern void func_0026C900(void);

extern void func_0025FD78(s32);

extern void func_00297970(s32);

extern s32 func_0026C768(void);

extern void func_002C42C0(s32 *, char *);

extern char D_003CE63C[];

INCLUDE_ASM(const s32, "game/code_002651C0", func_002651C0);

u32 func_002652D8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002652E0);

s64 func_00265360(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002653B8);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265408);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265500);

INCLUDE_ASM(const s32, "game/code_002651C0", func_002655C0);

s64 func_002657F8(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00265850(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265898);

u32 func_00265980(void) {
    return 1;
}

u32 func_00265988(s32 arg0) {
    u32 temp_v0;
    s64 temp_v1;

    if (((*(s32 *)(arg0 + 8) == 2) && (temp_v1 = mdlFlagTest(4), temp_v1 != 0)) &&
          (temp_v1 = mdlFlagTest(0x290), temp_v1 == 0)) {
        mdlFlagSet(0x290);
        temp_v0 = 1;
    }
    else {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002659E0);

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265A60);
