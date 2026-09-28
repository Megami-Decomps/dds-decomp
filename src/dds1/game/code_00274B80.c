#include "common.h"

extern s32 func_002877A8(void);

extern s32 func_00101A70();

extern s32 D_003BAA00;

void func_00274B80(u32 arg0) {
    func_00271308(4, arg0);
}

void func_00274BA0(void) {
}

s32 func_00274BA8(s32 arg0, s32 arg1) {
    if (arg0 < (*(s32 *)(arg1 + 0x20) - 1)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274BC0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274D48);

void func_00274EE0(s32 arg0) {
    func_0027C430(*(u32 *)(*(s32 *)(arg0 + 0x90c) + 8));
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274F00);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275030);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275328);

s32 func_002754A0(void) {
    s32 i;
    s32 temp_v0 = 0;
    u16 *temp_v1 = (u16 *)(D_003BAA00 + 0xa60);

    for (i = 4; i >= 0; i--) {
        temp_v0 += *temp_v1 & 1;
        temp_v1 += 210;
    }
    return (temp_v0 < 4) ? temp_v0 : 3;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002754E0);

void func_002755A0(s32 arg0) {
    func_0027F0D8(arg0 + 0x15c);
    func_00285A68(arg0 + 0x7ec);
    func_0027FF60(arg0 + 0x15c);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002755E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275880);

void func_002758D8(s32 menu) {
    extern u8 D_0037CA58[];
    func_00275328();
    func_002858F8(menu + 0x54, (s32)D_0037CA58);
    func_0027E790(*(s32 *)(menu + 0x138), *(s32 *)(menu + 0x6c), 0, 1);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275920);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275B40);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275F48);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276018);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002761C0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276250);

u8 func_00276288(void) {
    s64 temp_v0;

    temp_v0 = func_002877A8();
    return temp_v0 != 1;
}

void func_002762B0(u32 arg0) {
    func_00271308(3, arg0);
}

void func_002762D0() {
}

void func_002762D8(s32 *menu) {
    s32 i;
    for (i = 0; i < 2; i++) {
        func_002BD7A0(menu[7 + i]);
    }
}

void func_00276320(s32 *menu) {
    s32 i;
    for (i = 0; i < 2; i++) {
        func_002BD870(menu[7 + i]);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276368);

s32 func_00276428(void) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90c);
    func_00287548();
    func_002762D0(context);
    func_002D0918(*(s32 *)menu);
    return 1;
}

void func_00276478(u32 arg0) {
    func_00276368(arg0, 1);
}

void func_00276490(void) {
    func_00276428();
}

void func_002764A8(u32 arg0) {
    func_00276368(arg0, 0);
}

void func_002764C0(void) {
    func_00276428();
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002764D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002765E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002766E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276720);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276898);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276A18);

void func_00276B10(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0x7d8) + 0x1c) * 0x134 + arg0 + 0x2b4) + 0x3c) = 0x100
    ;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276B38);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276C28);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276DA0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276E90);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276F70);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277158);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277220);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277328);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277390);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277528);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002775D8);

u32 func_00277638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277640);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277848);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2208);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2260);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2270);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2280);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2290);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22A0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22B0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22C0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277A50);

u32 func_00277C80() {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277CB8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277D38);

void func_00277DD0(u32 arg0) {
    func_00271308(1, arg0);
}

void func_00277DF0(void) {
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277DF8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002780D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278218);

void func_002782E0(void) {
    func_002CFF98();
}

s32 func_002782F8(s32 arg0, u32 *arg1) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;

    return (arg1[temp_v0 >> 5] & (1 << arg0)) != 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278330);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002786E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278760);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278868);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002788D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278A90);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278B90);

void func_00278BC8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x34) = 0xffffffff;
}

u32 func_00278BF0(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x34) >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278C20);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278C90);

u32 func_00278D68(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x30) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278D90);

void func_00278E08(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 2 + arg0 + 0x22) = 0;
    func_002CD0C0();
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278E28);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278F50);

void func_00279130(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0 = (u8 *)(arg0 + 2);
    s32 temp_v1 = arg1 * 2 + 32;
    s32 temp_v2 = arg2 * 2 + 32;
    u16 temp_v3 = *(u16 *)(temp_v0 + temp_v1);
    u16 temp_v4 = *(u16 *)(temp_v0 + temp_v2);

    *(u16 *)(temp_v0 + temp_v1) = temp_v4;
    *(u16 *)(temp_v0 + temp_v2) = temp_v3;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279160);









INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22F0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2310);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2320);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6E0);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6E8);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6F0);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6F8);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC700);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC708);


INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC710);

