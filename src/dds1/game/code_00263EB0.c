#include "common.h"

extern s32 func_00101A70();

extern void func_0024DD78(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

s32 mnuCheckTableSums(s32 arg0, s32 arg1) {
    s32 *temp_p = (s32 *)(arg1 + 0x3d0);
    s8 *temp_q = (s8 *)(arg0 + 0x16);
    s32 temp_i = 0;

    do {
        s32 temp_sum = *temp_q + *temp_p;

        temp_q++;
        temp_p++;
        if (temp_sum < 0x63) {
            return 0;
        }
        temp_i++;
    } while (temp_i < 5);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00263EF8);

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002641E0);

void func_00264238(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

u32 func_00264280(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_002830F0(*(u32 *)(temp_v0 + 0xd10), 0xffffffffffffffff);
    func_0024DA58(0x16);
    func_0024DAE8(0);
    func_0024DAB8(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002642D0);

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264498);

void func_002644F0(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264538);

u32 func_00264608(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264610);

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002646A0);

void func_002646F8(s32 arg0) {
    s32 temp_v0 = func_00101A70();

    func_0024DD78();
    func_00285670(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264740);

u32 func_002647B0(void) {
    func_0024DBB0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_002647D0);

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC558);

