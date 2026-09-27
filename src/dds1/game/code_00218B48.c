#include "common.h"

extern s32 D_003BAA00;

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218B48);

void func_00218BE8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218C00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218CA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218E20);

u32 func_002192C0(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

u16 func_002192C8(s32 arg0) {
    return *(u16 *)(arg0 + 0xc);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002192D0);

s32 * func_00219350(s32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)(arg0 + 8);
    if (*piVar1 == 0xffff) {
        piVar1 = (s32 *)0x0;
    }
    return piVar1;
}

s32 * func_00219368(s32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)(arg0 + *(s32 *)(arg0 + 4));
    if (*piVar1 == 0xffff) {
        piVar1 = (s32 *)0x0;
    }
    return piVar1;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219388);

u8 func_002193D8(s32 *arg0, s32 arg1) {
    return *arg0 == arg1;
}

u16 func_002193E8(s32 arg0) {
    return *(u16 *)(arg0 + 0xe);
}

u16 func_002193F0(s32 arg0) {
    return *(u16 *)(arg0 + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002193F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219590);

INCLUDE_ASM(const s32, "game/code_00218B48", func_002198D8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219AF8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219B50);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219BB0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219C38);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219C78);

void func_00219CC8(u32 arg0) {
    func_002E75F0(arg0, 0x10, 4);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219CE8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219DD8);

void func_00219E30(s32 arg0) {
    func_00151E60(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

void func_00219E68(s32 arg0) {
    func_0014FEB0(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219EA0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219ED8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219FD8);

void func_0021A088(u32 arg0, s32 arg1) {
    func_00217310(arg0, *(u16 *)(arg1 + 8), *(u16 *)(arg1 + 10),
                                *(u32 *)(arg1 + 0xc), *(u32 *)(arg1 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A0B0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A170);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A1D0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A268);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A2E0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A368);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A3D8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A490);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A560);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A5B8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A608);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A628);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A660);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A718);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A880);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A8F0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A948);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A998);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A9F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AAB8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBB0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBC0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBD0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBE0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBF0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC00);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC10);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC20);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC40);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC50);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ABB8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ACF0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCC8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCD8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AE00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B0B0);

u32 func_0021B4E8(void) {
    func_0021AE00();
    func_0021B0B0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B510);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B950);

u32 func_0021B9D0(void) {
    func_0021B510();
    func_0021B950();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B9F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BDD0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BE50);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BE88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BFB0);

u32 func_0021C1E0(void) {
    func_0021BE88();
    func_0021BFB0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C208);

void func_0021C2E0(void) {
}

u32 func_0021C2E8(void) {
    func_0021C208();
    func_0021C2E0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C310);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C518);

u32 func_0021C678(void) {
    func_0021C310();
    func_0021C518();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C6A0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD58);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD68);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD98);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C6E8);

u32 func_0021C8D0(void) {
    func_0021C6A0();
    func_0021C6E8();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C8F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C920);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE18);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE48);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CB80);

u32 func_0021CE70(void) {
    func_0021C920();
    func_0021CB80();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CE98);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CF00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CFC0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D198);

u32 func_0021D568(void) {
    func_0021CFC0();
    func_0021D198();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D590);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D5D0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D668);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D740);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D780);

INCLUDE_ASM(const s32, "game/code_00218B48", modelViewer);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DAE0);

INCLUDE_ASM(const s32, "game/code_00218B48", modelViewerEnd);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DD88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E068);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E1C8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E360);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E3C0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E450);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E618);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021EB10);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021EB60);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ECF0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F098);

void func_0021F4B8(void) {
    func_0021F4E8();
    func_002286F8(0);
    func_00228778();
    func_00228728();
}

void func_0021F4E8(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0x7f;
    puVar1 = (u32 *)(D_003BAA00 + 0x840);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F520);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F580);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F5C0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F600);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F630);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FA78);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FB30);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FC30);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FD50);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFE8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFF8);

