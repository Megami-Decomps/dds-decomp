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
extern u8 D_003CE1A8[];
extern s32 D_00435E5C;
extern char D_00437850[];
extern s32 func_00265038();
extern s32 func_0026C6A0();
extern void func_00260380();
extern void func_00260020();
extern s32 func_0025FE70();
extern void func_002B8CF0();
extern void func_0025FC08();
extern s32 func_0026BC80();
extern void func_0023AC80();
extern void kwlnFadeOutStart();
extern void func_0026CA60();
extern void func_0026CA80();
extern u32 D_003CE460[];
extern s32 D_00435DD0;
extern char D_00437840[];
extern void mdlFlagSet();
extern void mdlFlagClear();
extern void func_0026C7F8();
extern s32 func_002604A0();
extern void func_0026C948();
extern void func_0026C918();
extern void func_0026C5B8();
extern void func_0011A0D0();
extern void func_0011A118();
extern s32 func_0035C860(char *, const char *, ...);

INCLUDE_ASM(const s32, "game/code_002651C0", func_002651C0);

u32 func_002652D8(void) {
    return 1;
}

s64 func_002652E0(s32 callback) {
    s32 context = func_00101958();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (func_0026C768() == 0) {
                func_002C42C0(window, D_003CE63C);
            }
        }
        return 0;
    }
    return state;
}

s64 func_00265360(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002653B8(s32 callback) {
    s32 context = func_00101958();
    func_0026C7F8(1, 0);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265408);

s32 func_00265500(void) {
    s32 context = func_00101958();
    s32 selectedId = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0x7c) + 0x18) + 0x1c) + 0x60);
    s32 node;
    s32 record;
    s32 slot;
    func_00260380(1, context);
    func_00260020(context);
    for (node = *(s32 *)(*(s32 *)(*(s32 *)(context + 0x7c) + 0x18) + 0x10);
         node != 0 && *(s32 *)(node + 0x60) != selectedId; node = *(s32 *)(node + 0x58)) {
        func_002B8CF0(*(s32 *)(*(s32 *)(context + 0x7c) + 0x18));
    }
    record = *(s32 *)(*(s32 *)(*(s32 *)(context + 0x7c) + 0x18) + 0x30);
    slot = func_0025FE70(context);
    *(u16 *)(record + 0x12) = slot;
    *(u16 *)(context + 0xa6) = slot;
    return 1;
}

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

s32 evtStartFadeByState(void) {
    s32 context = func_00101958();
    func_0025FC08(context);
    switch (*(s32 *)(context + 8)) {
    case 2:
        if (mdlFlagTest(0x42a) == 0 && func_0026BC80() == 0) {
            func_0023AC80(0x323);
        } else {
            kwlnFadeOutStart(0, 0, 0, 0xf);
        }
        break;
    case 0:
    case 1:
    case 3:
        kwlnFadeOutStart(0, 0, 0, 0xf);
        break;
    }
    func_0026CA60(0);
    func_0026CA80(0, 0);
    func_0026CA80(1, 1);
    return 1;
}

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

void func_002659E0(void) {
    char text[0x40];
    s32 index = func_002604A0();
    func_0026C948(1);
    func_0035C860(text, D_00437840, index);
    func_0026C918(0, text);
    func_0026C5B8(0x19);
    func_0011A0D0(index);
    func_0011A118(0x81, -*(u8 *)(D_00435DD0 + 0x13c1));
    mdlFlagClear(0xa01);
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265A60);
