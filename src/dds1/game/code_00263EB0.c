#include "common.h"

extern s32 func_00101A70();

extern void func_0024DD78(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

s32 mnuCheckTableSums(s32 bytes, s32 table) {
    s32 *tableValues = (s32 *)(table + 0x3d0);
    s8 *byteValues = (s8 *)(bytes + 0x16);
    s32 index = 0;

    do {
        s32 total = *byteValues + *tableValues;

        byteValues++;
        tableValues++;
        if (total < 0x63) {
            return 0;
        }
        index++;
    } while (index < 5);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00263EF8);

extern void func_002639E0(s32);
extern void func_00263B78(s32, s32);

void func_002641E0(s32 request) {
    s32 context = func_00101A70();

    func_002639E0(context);
    func_00263B78(context, 1);
    func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_00264238(s32 input) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, input);
}

u32 func_00264280(void) {
    s32 context;

    context = func_00101A70();
    func_002830F0(*(u32 *)(context + 0xd10), 0xffffffffffffffff);
    func_0024DA58(0x16);
    func_0024DAE8(0);
    func_0024DAB8(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002642D0);

void func_00264498(s32 request) {
    s32 context = func_00101A70();

    func_002639E0(context);
    func_00263B78(context, 1);
    func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_002644F0(s32 input) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, input);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264538);

u32 func_00264608(void) {
    return 1;
}

extern s32 func_00287C20(void);
extern s64 func_0024DC08(void);
extern void func_002858F8(s32 *, char *);
extern char D_0036D478[];

s64 func_00264610(u64 request) {
    s32 context = func_00101A70();
    s32 *state = (s32 *)(context + 0x54);
    s64 result;

    func_00287C20();
    result = func_00285670(context + 8, state, 0, request);
    if (result == 0) {
        if ((*state == 0) && (result = func_0024DC08(), result == 0)) {
            func_002858F8(state, D_0036D478);
        }
        result = 0;
    }
    return result;
}

void func_002646A0(s32 request) {
    s32 context = func_00101A70();

    func_002639E0(context);
    func_00263B78(context, 0);
    func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

void func_002646F8(s32 input) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, input);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264740);

u32 func_002647B0(void) {
    func_0024DBB0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002647D0);

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC558);

