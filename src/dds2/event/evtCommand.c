#include "common.h"

extern u64 func_0010D650(u64);

extern s64 func_0023E6D8(u64, u64);

extern s32 func_0010D8A8(void);

extern u64 func_00243330(void);

extern u64 func_00243358(void);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240D20);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240DE0);

u32 func_00240F00(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0023E6D8(7, temp_v0);
    if (temp_v1 != 0) {
        func_0023B078(temp_v1, 1);
        func_00115DF0(temp_v1, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00240F58);

INCLUDE_ASM(const s32, "event/evtCommand", func_00240FE8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241090);

u32 func_002410D0(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0023E6D8(7, temp_v0);
    if (temp_v1 != 0) {
        func_0023B078(temp_v1, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241110);

INCLUDE_ASM(const s32, "event/evtCommand", func_002411A0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241218);

u32 func_002412B0(void) {
    func_001027D8(2, 0, 0, 0);
    return 1;
}

u32 func_002412E0(void) {
    func_001027D8(0xc, 0, 0, 0);
    return 1;
}

u32 func_00241310(void) {
    func_001027D8(4, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241340);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241390);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241410);

u32 func_00241460(void) {
    s64 temp_v0;

    temp_v0 = func_0010D8A8();
    if (temp_v0 == 0) {
        func_00102908();
    }
    return 0;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241490);

u32 func_002414F8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00241490(temp_v0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241528);

u32 func_00241580(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00241528(temp_v0);
    return 1;
}

u32 func_002415A8(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_00241490(temp_v0, temp_v1);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_002415E8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241668);

INCLUDE_ASM(const s32, "event/evtCommand", func_002416D0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241708);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241770);

INCLUDE_ASM(const s32, "event/evtCommand", func_002417C8);

u32 func_00241838(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_0023B0D0(0, temp_v0);
    return 1;
}

u32 func_00241868(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_0023B0D0(1, temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00241898);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241908);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241978);

INCLUDE_ASM(const s32, "event/evtCommand", func_002419B0);

INCLUDE_ASM(const s32, "event/evtCommand", func_002419F8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241A70);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241AF8);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241B98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241C20);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241CB0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241CF0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241D98);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241E70);

INCLUDE_ASM(const s32, "event/evtCommand", func_00241F10);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242100);

INCLUDE_ASM(const s32, "event/evtCommand", func_002422A8);

u32 func_00242368(void) {
    func_00243380();
    return 1;
}

u32 func_00242388(void) {
    func_00243398();
    return 1;
}

u32 func_002423A8(void) {
    u64 temp_v0;

    temp_v0 = func_00243330();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_002423D0(void) {
    u64 temp_v0;

    temp_v0 = func_00243358();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_002423F8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00243368(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00242420);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242500);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242580);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242600);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242660);

u32 func_00242738(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_0024F058(temp_v0, 1);
    func_0010D818(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "event/evtCommand", func_00242778);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242818);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242898);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242918);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242A30);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242B40);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242BC0);

INCLUDE_ASM(const s32, "event/evtCommand", func_00242C40);
