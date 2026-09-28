#include "common.h"

extern u64 func_00279DC8(u32, u64, u64, u64, u64, u64);

extern u64 func_0027A628(u32, u64, u64);

extern u32 func_00284AE0(void);

extern u32 func_002843C0(u32);

extern s32 func_0026FDC8(u32, u32);

extern s32 func_00328D68(u32);

void func_003297C8(u32 sprite);

void func_002844E8(u32 sprite);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DBF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DC48);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DE08);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DF48);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E060);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E198);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E2D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E470);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E4C8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E508);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E560);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E5F8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E650);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E688);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E6E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E998);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EBA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EDB0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EEE8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F008);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F138);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F190);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F1F0);

s32 *func_0026F570(void) {
    s32 *temp_v0 = (s32 *)func_00328D68(0x14);

    memset(temp_v0, 0, 0x14);
    return temp_v0;
}

u32 func_0026F5B0(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_00328E48();
    return temp_v0;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250D8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250E8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250F8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425108);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F5D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F680);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F700);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F778);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F7E8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F870);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F8A0);

void func_0026FAA8(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0x1e;
}

u32 func_0026FAB8(void) {
    return 0;
}

u32 func_0026FAC0(void) {
    return 0;
}

void func_0026FAC8(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FAD0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FB68);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FBD8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FC40);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FC88);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FDC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FE18);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270008);

void func_00270050(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 0);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00270078(u32 arg0, s8 arg1) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 0);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u32 *)(temp_v0 + 4) = (*(u32 *)(temp_v0 + 4) & 0xffffff0f) | (((s32)arg1 & 0xfU) << 4);
    *(u8 *)(temp_v0 + 5) = 5;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002700D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270100);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270128);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270160);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002701D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270210);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270390);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270568);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270848);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270C78);

void func_00270CC0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00270CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 6;
}

void func_00270D10(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 5;
}

void func_00270D38(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 2;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004251C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425258);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270D60);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270DB0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270DD8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270F10);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270FA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271020);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271068);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002710A0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002710D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002711F8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271250);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271290);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002712E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271348);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271368);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271510);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272BD0);

void func_00272C18(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425338);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425348);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425370);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425380);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425408);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425420);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425448);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425458);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425490);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272C40);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272C78);

void func_00272CB0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

void func_00272CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 0xe) = *(u16 *)(temp_v0 + 0xe) ^ 1;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272D20);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272D80);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272DA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272F08);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273450);

void func_00273498(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 7);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002734C0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273530);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002735A0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002735F0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273640);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273668);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273828);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273F88);

void func_00273FD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 8);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273FF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274070);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002740E8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274158);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002741A8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002741D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002743B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002745E0);

void func_00274628(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 9);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274650);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274690);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002746F8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274760);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002747B0);

u32 func_002747F0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 9);
    func_0026FAA8(*(s32 *)(temp_v0 + 0x20) + 0xc);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274820);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274890);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002748D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274A70);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274D88);

void func_00274DD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 10);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00274DF8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 10);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u32 *)(temp_v0 + 0xc) = 0xf;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274E30);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274E88);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274EA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274FF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275218);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275358);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275510);

void func_00275598(u32 *sprite) {
    func_003297C8(*sprite);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002755B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002756E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002758B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002759F8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275B70);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425828);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275CE8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00277F38);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278D80);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278DC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278DF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278E50);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278EA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278EE0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278F18);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278F60);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278FA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00278FF0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279028);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279080);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002790B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002790F0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279148);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279180);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002792D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279308);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279440);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279488);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002795E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279628);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279778);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002797C0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002797F0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279848);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002798A0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002798D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279910);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279968);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002799A0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002799D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279C38);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279C58);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279DC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279F30);

void func_00279F70(u32 *arg0) {
    *arg0 = (*arg0 & 0xfffc03ff) | 0x800;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279F90);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258F0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425928);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A080);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A198);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A4C0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A628);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A798);

void func_0027A7F0(void) {
}

void func_0027A7F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425988);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004259C0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004259F8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A30);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A800);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027AF40);

void func_0027AFD8(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027AFE0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AB8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027B678);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C078);

void func_0027C108(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C110);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C2F0);

void func_0027C418(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C448);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C468);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C558);

void func_0027CD78(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(2);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027CDB0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027CDD0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B78);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B88);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027D3D8);

void func_0027DE30(void) {
}

void func_0027DE38(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027DE40);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425BC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E0E0);

void func_0027E208(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(0);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    temp_v0 = func_00284AE0();
    *(u32 *)(arg1 + 0x28) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E240);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E270);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E360);

void func_0027FB60(void) {
}

void func_0027FB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FB70);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FC90);

void func_0027FDB8(void) {
}

void func_0027FDC0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FDC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00280390);

void func_002803A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002803A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425C98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CA8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CB8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002805E0);

void func_002817B8(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
}

void func_002817C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002817C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281B60);

void func_00281C88(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281CB8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281CD8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281DC0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00282B50);

void func_00282B60(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00282B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425D68);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283090);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283F90);

void func_00283FA0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283FA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284298);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002843C0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002844E8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284508);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284818);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284AE0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284B48);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284B70);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284F00);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002850B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285120);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285148);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285500);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002856E0);

u8 func_00285748(s32 arg0) {
    return *(s32 *)(arg0 + 0x6c) != 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285758);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285800);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002858A0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285A60);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285AC0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285BD8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378D0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378D8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378E0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378E8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437900);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437908);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437910);

