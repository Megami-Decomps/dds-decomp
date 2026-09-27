#include "common.h"

extern s32 D_00435DD0;

INCLUDE_ASM(const s32, "game/code_00233660", func_00233660);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233700);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233718);

INCLUDE_ASM(const s32, "game/code_00233660", func_002337C0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233938);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233DD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233E30);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233E38);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233E40);

s32 * func_00233EC0(s32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)(arg0 + 8);
    if (*piVar1 == 0xffff) {
        piVar1 = (s32 *)0x0;
    }
    return piVar1;
}

s32 * func_00233ED8(s32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)(arg0 + *(s32 *)(arg0 + 4));
    if (*piVar1 == 0xffff) {
        piVar1 = (s32 *)0x0;
    }
    return piVar1;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233EF8);

u8 func_00233F48(s32 *arg0, s32 arg1) {
    return *arg0 == arg1;
}

u16 func_00233F58(s32 arg0) {
    return *(u16 *)(arg0 + 0xe);
}

u16 func_00233F60(s32 arg0) {
    return *(u16 *)(arg0 + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233F68);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234100);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234448);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234668);

INCLUDE_ASM(const s32, "game/code_00233660", func_002346C0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234720);

INCLUDE_ASM(const s32, "game/code_00233660", func_002347A8);

INCLUDE_ASM(const s32, "game/code_00233660", func_002347E8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234838);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234858);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234948);

void func_002349A0(s32 arg0) {
    func_00159A50(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

void func_002349D8(s32 arg0) {
    func_00157A50(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00234A10);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234A48);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234B48);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234BF8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234C20);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234CE0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234D40);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234DD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234E50);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234ED8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234F48);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235000);

INCLUDE_ASM(const s32, "game/code_00233660", func_002350D0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235128);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235178);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235198);

INCLUDE_ASM(const s32, "game/code_00233660", func_002351D0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235288);

INCLUDE_ASM(const s32, "game/code_00233660", func_002353F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235460);

INCLUDE_ASM(const s32, "game/code_00233660", func_002354B8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235508);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235568);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235628);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421120);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421130);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421140);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421150);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421160);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421170);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421180);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421190);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211A0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211B0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211C0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211D0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211E0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235728);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235860);

INCLUDE_ASM(const s32, "game/code_00233660", func_002358E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421238);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421248);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235970);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235C20);

u32 func_00236058(void) {
    func_00235970();
    func_00235C20();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236080);

INCLUDE_ASM(const s32, "game/code_00233660", func_002364C0);

u32 func_00236540(void) {
    func_00236080();
    func_002364C0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236568);

INCLUDE_ASM(const s32, "game/code_00233660", func_00236940);

INCLUDE_ASM(const s32, "game/code_00233660", func_002369C0);

INCLUDE_ASM(const s32, "game/code_00233660", func_002369F8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00236B20);

u32 func_00236D50(void) {
    func_002369F8();
    func_00236B20();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236D78);

void func_00236E50(void) {
}

u32 func_00236E58(void) {
    func_00236D78();
    func_00236E50();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236E80);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237088);

u32 func_002371E8(void) {
    func_00236E80();
    func_00237088();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237210);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212C8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212D8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421308);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237258);

u32 func_00237440(void) {
    func_00237210();
    func_00237258();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237468);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237490);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421388);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213A0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213B8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213D0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213E0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_002376F0);

u32 func_002379E0(void) {
    func_00237490();
    func_002376F0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237A08);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237A70);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237B30);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237D08);

u32 func_002380D8(void) {
    func_00237B30();
    func_00237D08();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00238100);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238140);

INCLUDE_ASM(const s32, "game/code_00233660", func_002381D8);

INCLUDE_ASM(const s32, "game/code_00233660", func_002382B0);

INCLUDE_ASM(const s32, "game/code_00233660", func_002382F0);

INCLUDE_ASM(const s32, "game/code_00233660", modelViewer);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238650);

INCLUDE_ASM(const s32, "game/code_00233660", modelViewerEnd);

INCLUDE_ASM(const s32, "game/code_00233660", func_002388F8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238BD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238D38);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238ED0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238F30);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238FC0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239188);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239680);

INCLUDE_ASM(const s32, "game/code_00233660", func_002396D0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239860);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239C08);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A028);

void func_0023A058(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0x7f;
    puVar1 = (u32 *)(D_00435DD0 + 0x840);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A090);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A0F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A130);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A170);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421508);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421518);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A1A0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A5E8);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A6A0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A7A0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A8C0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421558);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421568);
