#include "common.h"

extern s32 D_003BC7B4;

extern s32 D_003BAA4C;

extern s32 D_003BAA50;

extern u32 func_002BD258(u32);

extern s32 func_002860B8(u16);

extern u32 func_0027D4A0(u32);

extern u32 func_0027F730(u32);

extern s32 func_002CFEB8(u32);

extern u32 func_0027D148(u32, u32, u32, u32);

extern s32 func_0027B888(u32);

extern s32 func_002877A8(void);

extern char D_003B2608[]; /* "battle stage test" */

extern u8 D_0037CE60[];

extern u8 D_0037CE70[];

extern f32 D_003245E0[];

extern u32 D_003DC5F0[];

extern u32 D_003DC608[];

extern u32 D_003DC618[];

extern u32 D_003DC650[];

extern u32 D_003DC654[];

extern u32 D_003DC5E8[];

extern u32 D_003DC5F8[];

extern u32 D_003DC600[];

extern u8 D_003DC5F4[];

extern s32 D_003BAA50;

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

extern u32 D_003DC5EC[];

extern s32 D_003BAA00;

extern void func_002874E8(void);

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

INCLUDE_ASM(const s32, "game/code_00274B80", func_002758D8);

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

void func_002762D0(void) {
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002762D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276320);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276368);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276428);

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

u32 func_00277C80(void) {
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

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279328);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002793D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279568);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279728);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279760);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279860);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002798F8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279A30);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279AF8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279B30);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279BA8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279BF8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279CC0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279D68);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00279F88);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A0A8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A0E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A140);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A300);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A468);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A4D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A540);

u32 func_0027A778(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A7B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A810);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027A860);

void func_0027A9A8(s32 arg0) {
    func_002BF790(0x1c0, 0xa60, 0, 1, *(u32 *)(arg0 + 0x74), 0x1f, 0x53);
    func_002BF790(0x150, 0xa00, 0, 1, *(u32 *)(arg0 + 0x74), 0, 0x53);
    func_002BF790(0xbb0, 0xa00, 0, 1, *(u32 *)(arg0 + 0x74), 0, 0x53);
    func_002BF790(0x250, 0x9c0, 0, 1, *(u32 *)(arg0 + 0xe4), 0x18, 0x53);
    func_002BF790(0xce0, 0x9e0, 0, 1, *(u32 *)(arg0 + 100), 2, 0x53);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027AA68);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027AB10);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027AC00);

void func_0027AC38(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_002BD908(2);
    *(u32 *)(arg0 + 0x18) = temp_v1;
    temp_v2 = func_002BD908(2);
    temp_v0 = *(s32 *)(temp_v2 + 8);
    *(s32 *)(arg0 + 0x1c) = temp_v2;
    func_002BDE18(temp_v0 + 0x28, *(u32 *)(arg0 + 0x14), 0, 0xc);
    func_002BDE18(*(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x94, *(u32 *)(arg0 + 0x14), 1, 0xc)
    ;
    func_002BE128(*(u32 *)(arg0 + 0x10), 0, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BE128(*(u32 *)(arg0 + 0x10), 1, 0, *(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x6c);
    func_002BE128(*(u32 *)(arg0 + 0x10), 2, 0, *(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x6c);
    func_002BE128(*(u32 *)(arg0 + 0x10), 3, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BE128(*(u32 *)(arg0 + 0x10), 4, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BDE18(*(s32 *)(*(s32 *)(arg0 + 0x18) + 8) + 0x28, *(u32 *)(arg0 + 0x14), 2, 0xd)
    ;
    func_002BE0C0(*(u32 *)(arg0 + 4), 0, *(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
    func_002BE258(*(u32 *)(arg0 + 8), 0, *(u32 *)(arg0 + 0x14), 3, 4);
    func_002BE258(*(u32 *)(arg0 + 0xc), 0, *(u32 *)(arg0 + 0x14), 4, 4);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027AD80);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027AEA8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027AF28);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B010);

void func_0027B088(s32 arg0, u32 arg1) {
    func_002C14E0(arg1);
    func_002BF790(0xffffffffffffff90, 0xa0, 0, 0x61, *(u32 *)(arg0 + 0x10), 0, arg1);
    func_002BF790(0xfffffffffffffb90, 0x808, 0, 0x61, *(u32 *)(arg0 + 0x10), 1, arg1);
    func_002BF790(0x1050, 0xfffffffffffffc18, 0, 0x61, *(u32 *)(arg0 + 0x10), 2, arg1);
    func_002BF790(0x10b0, 0x3c0, 0, 0x61, *(u32 *)(arg0 + 0x10), 3, arg1);
    func_002BF790(0x1300, 0xb70, 0, 0x61, *(u32 *)(arg0 + 0x10), 4, arg1);
    func_002C1548(0, arg1);
    func_002BF970(*(u32 *)(arg0 + 0x10), 0);
    func_002BF970(*(u32 *)(arg0 + 0x10), 1);
    func_002BF790(0, 0, 0, 0x60, *(u32 *)(arg0 + 4), 0, arg1);
    func_002BF970(*(u32 *)(arg0 + 4), 0);
    func_002C1588(arg1);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B1B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B268);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B2F8);

u32 func_0027B368(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_0027B888(arg0);
    } while (temp_v0 != 0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B3A8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B440);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B4F0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B540);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027B888);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BA00);

void *func_0027BA48(s32 arg0, void *arg1) {
    void *temp_node = *(void **)((s32)arg1 + 0x10);
    s32 temp_i = 0;

    if (temp_node != NULL && arg0 != temp_i) {
        do {
            temp_node = *(void **)((s32)temp_node + 0x58);
            temp_i++;
        } while (temp_node != NULL && temp_i != arg0);
    }
    return temp_node;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BA90);

void func_0027BB08(u32 arg0) {
    func_0027BA90(0, arg0);
}

void func_0027BB28(s32 *arg0) {
    func_0027BA90(arg0[8] - 1, arg0);
}

s32 func_0027BB48(s32 *arg0) {
    s32 temp_1C = arg0[7];
    s32 temp_14 = arg0[5];
    s32 *temp_18 = (s32 *)arg0[6];

    if (temp_1C == temp_14) {
        return temp_1C;
    }
    temp_18 = (s32 *)temp_18[22];
    if (temp_18 == NULL) {
        return temp_1C;
    }
    arg0[6] = (s32)temp_18;
    arg0[9]--;
    return temp_1C;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BB80);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BBF0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BD48);

void func_0027BE90(u32 arg0) {
    func_0027BBF0(arg0, 0, 0);
}

void func_0027BEB0(u32 arg0) {
    func_0027BD48(arg0, 0, 0);
}

void func_0027BED0(u32 *arg0) {
    *arg0 &= ~1;
    *arg0 &= ~2;
}

u32 func_0027BEF0(u32 *arg0) {
    return *arg0 & 2;
}

s32 func_0027BF00(s32 arg0) {
    return *(s32 *)(arg0 + 0x28) * *(s32 *)(arg0 + 0xc);
}

void func_0027BF10(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x50) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x58), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x50) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BF48);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027BF90);

void func_0027C070(s32 arg0, s32 arg1) {
    func_002C1630((*(s32 *)(arg1 + 0x48) & 1) ? 0x89BDC940 : 0x89BDC980, arg0, *(s32 *)(arg1 + 0x50));
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C0B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C140);

void func_0027C370(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C140(arg0, arg1, arg2, 0, 0, 0x100, 0, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C3A0);

void func_0027C430(u32 arg0) {
    s32 temp_v0;

    func_0027B368(*(u32 *)((s32)arg0 + 0x14));
    temp_v0 = *(s32 *)((s32)arg0 + 0x84);
    if (temp_v0 != 0) {
        func_0027D1E8(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_0027C470(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_0027C478(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x88) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C480);

void func_0027C558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C480(arg0, arg1, arg2, arg3, arg4, arg4);
}

void func_0027C570(s32 arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    arg1[6] = arg0;
    arg1[7] = arg2;
    arg1[9] = arg3 + 3;
    arg1[8] = arg3;
    arg1[10] = arg4;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C590);

void func_0027C620(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    u32 temp_v0;

    temp_v0 = func_0027D148(arg1, arg2, arg3, arg4);
    *(u32 *)(arg0 + 0x84) = temp_v0;
}

void func_0027C658(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_0027C670(s32 arg0) {
    func_0027B440(*(u32 *)(arg0 + 0x14));
}

void func_0027C688(s32 arg0) {
    func_0027B540(*(u32 *)(arg0 + 0x14));
}

void func_0027C6A0(s32 arg0) {
    func_0027B888(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C6B8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C708);

void func_0027C758(u32 arg0) {
    func_0027C6B8(arg0, 0);
}

void func_0027C770(u32 arg0) {
    func_0027C708(arg0, 0);
}

void func_0027C788(s32 arg0) {
    func_0027BED0(*(u32 *)(arg0 + 0x14));
}

void func_0027C7A0(s32 arg0) {
    func_0027BEF0(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027C7B8);

void func_0027CA78(void) {
    func_0027C7B8();
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027CA90);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027CCD0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027CDD0);

void func_0027CEE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x14) + 0x20);
    if (0 < temp_v0) {
        do {
            temp_v0 = temp_v0 - 1;
        } while (temp_v0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027CF28);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D148);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D1E8);

void func_0027D248(s32 arg0, u32 arg1) {
    func_002BE378(*(u32 *)(arg0 + 0x10), 0, arg1, 0, 0x14, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x14), 0, arg1, 1, 10, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x18), 0, arg1, 2, 0, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x1c), 0, arg1, 2, 0, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x20), 0, arg1, 1, 10, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x24), 0, arg1, 0, 0x14, 0xc);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D318);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D4A0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D740);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D7E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027D850);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027DA80);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027DBD0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027DCE8);

void func_0027DD58(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_0027DCE8(arg0, arg1, arg2, arg3, arg4, 0, arg5);
}

void func_0027DD78(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027DD58(arg0, arg1, arg2, 0x100, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027DDA0);

void func_0027DE60(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x5c);
    }
    *(s32 *)(arg0 + 0x10) = temp_v1;
}

void func_0027DE98(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x58);
    }
    *(s32 *)(arg0 + 0x14) = temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027DED0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027DF48);

s32 func_0027DFF8(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_0027E020(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 func_0027E050(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_0027E078(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 func_0027E0A8(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_0027E0D0(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E100);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E228);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E270);

void func_0027E2C0(s32 arg0, s32 arg1, s32 *arg2) {
    u32 temp_v0 = *arg2;
    s32 *temp_v1 = arg2 + temp_v0;
    s32 *temp_v2;

    if (temp_v0 < 5) {
        return;
    }
    temp_v2 = (s32 *)temp_v1[1];
    *arg2 = temp_v0 + 1;
    temp_v2[0] = arg0;
    temp_v2[4] = arg1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E2F0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E3D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E468);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E4E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E570);

s32 func_0027E5C0(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v0 = func_002CFEB8(0x4c);
    memset(temp_v0, 0, 0x4c);
    *(u32 *)(temp_v0 + 8) = 0;
    *(u32 *)(temp_v0 + 0xc) = 0;
    temp_v2 = 1;
    func_002BFB98(temp_v0 + 0x10, arg0, arg1);
    func_002BFB98(temp_v0 + 0x18, arg2, arg3);
    temp_v1 = temp_v0;
    do {
        temp_v2 = temp_v2 - 1;
        func_002BFB98(temp_v1 + 0x20, 0, 0);
        func_002BFB98(temp_v1 + 0x30, 0, 0);
        temp_v1 = temp_v1 + 8;
    } while (-1 < temp_v2);
    func_0027E4E0(temp_v0);
    return temp_v0;
}

void func_0027E690(u32 arg0) {
    func_0027E570();
    func_002CFF98(arg0);
}

void func_0027E6B8(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002BFB98(arg0 + 0x10);
    *(u32 *)(arg0 + 4) = arg3;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E6F0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E790);

u8 func_0027E850(s32 arg0) {
    return *(s32 *)(arg0 + 0x20) != 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E860);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027E8D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027EAF0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027EC40);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027ECD8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027EF18);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027EFD0);

void func_0027F050(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = func_002BDBC0(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    temp_v0 = func_002BDBC0(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xec) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = func_002BDBC0(arg1, arg4, 1);
        *(u32 *)(arg0 + 0xf0) = temp_v0;
    }
}

void func_0027F0D8(s32 arg0) {
    u32 temp_v0;
    s32 *piVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 temp_v1;

    piVar2 = (s32 *)(arg0 + 0x168);
    puVar3 = (u32 *)(arg0 + 0x7c);
    puVar4 = (u32 *)(arg0 + 0x164);
    temp_v1 = 0;
    do {
        if (piVar2[-2] != 0) {
            func_002BDD60(piVar2[-2]);
        }
        if (piVar2[-1] != 0) {
            func_002BDD60(piVar2[-1]);
        }
        if (*piVar2 != 0) {
            func_002BDD60(*piVar2);
        }
        temp_v0 = *puVar3;
        temp_v1 = temp_v1 + 1;
        piVar2[-2] = 0;
        *puVar4 = 0;
        *puVar3 = temp_v0 & 0xffffff7f;
        puVar3 = puVar3 + 0x4d;
        *piVar2 = 0;
        piVar2 = piVar2 + 0x4d;
        puVar4 = puVar4 + 0x4d;
    } while (temp_v1 < 5);
}

void func_0027F198(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0 = arg0 + arg1 * 0x134 + 0x78;

    *(s32 *)(temp_v0 + 0x6C) = 0;
    *(s32 *)(temp_v0 + 0xC0) = 0;
    if (arg3 != 0) {
        return;
    }
    *(s32 *)(temp_v0 + 0x68) = 0x100;
    *(s32 *)(temp_v0 + 0xBC) = 0x100;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F1C8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F230);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F4B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F588);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F638);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F6B8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F730);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F838);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027F898);

void func_0027F9D8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = func_0027F730(arg2);
    *(u32 *)(arg0 * 0x134 + arg1 + 0x15c) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FA20);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FA70);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FAA8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FB90);

void func_0027FBE0(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x24);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

void func_0027FC10(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x44);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FC40);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FCA0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_0027FF60);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280048);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280170);

void func_00280228(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x67c));
    func_0027B368(*(u32 *)(arg0 + 0x680));
}

void func_00280258(s32 arg0, s32 arg1) {
    func_00280228(arg0);
    func_00280170(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280290);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002802E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002803A0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280488);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002804F0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002805B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002806E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002807E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280858);

void func_002808A8(s32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0;
    do {
        temp_v0 = temp_v0 + 1;
        *puVar1 = *puVar1 & 0xfffffffe;
        puVar1 = puVar1 + 0x4d;
    } while (temp_v0 < 5);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002808E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280978);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280A90);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280BC0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280D98);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00280E08);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281108);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002811D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002812E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002814D0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002815F0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281688);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281780);

void func_00281898(u32 arg0) {
    memset(arg0, 0, 0x20);
}

void func_002818B8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 1;
    temp_v0 = arg1 * 0x134 + arg0 + 0x16c;
    do {
        temp_v1 = temp_v1 - 1;
        func_00281898(temp_v0);
        temp_v0 = temp_v0 + 0x20;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281908);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002819F8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281AE8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281BE0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22F0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2310);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2320);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2330);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2348);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2358);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2368);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2380);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B23A0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B23B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00281D40);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B23D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00282360);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00282850);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002828D0);

void func_002829C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002829E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00282B08);

void func_00282BE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_00282B08(arg0, arg1, arg2, 0, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00282C10);

void func_00282C70(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x60);
    if (temp_v0 != 0) {
        func_0027D7E0(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_00282CA8(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u32 *)(arg0 + 0x1c) = arg2;
    func_002BFB98(arg0 + 0x20, arg3, arg4);
}

void func_00282CD0(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x28) = arg1;
    *(u32 *)(arg0 + 0x2c) = arg2;
    func_002BFB98(arg0 + 0x30, arg3, arg4);
}

void func_00282CF8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_0027D4A0(2);
    *(u32 *)(arg0 + 0x60) = temp_v0;
}

void func_00282D28(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x40) = arg1;
    *(u32 *)(arg0 + 0x44) = arg2;
    func_002BFB98(arg0 + 0x38, arg3, arg4);
}

void func_00282D50(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5) {
    *(u32 *)(arg0 + 0x50) = arg1;
    *(u32 *)(arg0 + 0x54) = arg2;
    func_002BFB98(arg0 + 0x48, arg3, arg4);
    *(u32 *)(arg0 + 0x58) = arg5;
}

void func_00282D98(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00282DA0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00282F98);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283038);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283090);

void func_002830F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_002830F8(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xffffffff;
}

u32 func_00283108(s32 arg0) {
    return *(u32 *)(arg0 + 0x20);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283110);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283238);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283280);

void func_002832F8(void) {
    func_002CFF98();
}

void func_00283310(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u16 arg4, s32 arg5, u32 arg6, u32 arg7) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0x11;
    if (arg5 != 0) {
        temp_v1 = 0x12;
    }
    temp_v0 = func_002860B8(arg4);
    func_002BF4E0(arg0, arg1, arg2, arg3, 1, arg6, temp_v0 * 2 + temp_v1, arg7);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002833B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283788);

void func_00283820(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283838);

void func_00283BE0(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 4) = arg1;
    *(s32 *)arg0 = 0;
    *(s32 *)(arg0 + 8) = 0;
}

void func_00283BF0(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283BF8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283CA8);

s32 func_00283CE8(s32 arg0) {
    s32 temp_v0 = *(s32 *)(arg0 + 0x10);

    if (temp_v0 < 0x32) {
        return (temp_v0 >= 0x14) ? 1 : 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283D10);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283E60);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00283EE0);

void func_00284080(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_002BD258(3);
    *(u32 *)(arg0 + 0x40) = temp_v0;
    temp_v0 = func_002BD258(3);
    *(u32 *)(arg0 + 0x44) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002840B8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284108);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284258);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284340);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002843A0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284418);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002845F8);

void func_00284880(void) {
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284888);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002848E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284A90);

void func_00284BF8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_00284C00(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0x18) = arg1;
    *(s32 *)(arg0 + 0x1c) = arg2;
}

void func_00284C10(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0x20) != arg1) {
        *(u32 *)(arg0 + 0x8c) = 0x100;
    }
    *(s32 *)(arg0 + 0x20) = arg1;
}

void func_00284C28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_00284C30(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284C48);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00284EB8);

void func_002850C8(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0x10) = arg1;
    *(s32 *)(arg0 + 0x14) = arg2;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002850D8);

void func_00285160(void) {
    func_002CFF98();
}

void func_00285178(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    func_002BFB98((u32 *)(arg0 + 0x18));
    func_002BF9E0(*(u32 *)(arg0 + 0x18), *(u32 *)(arg0 + 0x1c), 0, 0, 0, 0);
    func_002BFB98(arg0 + 0x20, arg1, arg3);
    func_002BFB98(arg0 + 0x28, arg1, arg4);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285208);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285440);

void func_00285490(u32 arg0) {
    memset(arg0, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002854B0);

void func_00285600(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    while (temp_v0 != 0) {
        func_002854B0(1, 0, arg0, arg1);
        temp_v0 = *(s32 *)arg0;
    }
}

s32 func_00285658(s32 *arg0) {
    return (*arg0 & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285670);

u8 func_002858D8(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x44) == arg1;
}

void func_002858E8(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0;
}

void func_002858F8(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0 | 0x20000;
}

void func_00285910(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0 | 0x60000;
}

void func_00285928(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_002858F8(arg1, *(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285960);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285A68);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285B20);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285E00);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00285F00);

void func_00286050(u32 arg0) {
    func_00285F00(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286068);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002860B8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286138);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286170);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002861B8);

u16 func_00286288(s32 arg0) {
    s32 temp_v0 = (arg0 & 0xffff) * 56 + D_003BAA50;

    if (*(u8 *)(temp_v0 + 0x24) != 2) {
        return 0;
    }
    return *(u16 *)(temp_v0 + 0x26);
}

u8 func_002862C0(u32 arg0) {
    return *(u8 *)((arg0 & 0xffff) * 0x38 + D_003BAA50 + 3);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002862E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286368);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286440);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002864D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286540);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002865B8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286648);

void func_002866B0(u16 arg0) {
    func_00118E38(arg0);
}

u32 func_002866C8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002866D0);

u8 func_002868C0(u32 arg0) {
    return *(s8 *)((arg0 & 0xffff) * 2 + D_003BAA4C) == '\x01';
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002868E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286990);

s32 func_002869E8(s32 arg0) {
    if (arg0 < 0xa0) {
        return 0;
    }
    return arg0 < 0xbf;
}

s32 func_00286A00(s32 arg0) {
    if (arg0 < 0x60) {
        return 0;
    }
    return arg0 < 0x7f;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286A18);

u32 func_00286AC0(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x52) = arg1;
    return 1;
}

u16 func_00286AD0(s32 arg0) {
    return *(u16 *)(arg0 + 0x52);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286AD8);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2420);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2430);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2450);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2468);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2480);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B24A8);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B24D0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B24E8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286B48);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286D20);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286E50);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286EA0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286EF8);

u8 func_00286F48(void) {
    return D_003BC7B4 != 0;
}

void func_00286F58(s32 arg0) {
    if (arg0 == 0) {
        D_003DC5F4[0] = 0;
    } else {
        D_003DC5F4[0] = 1;
    }
}

u32 func_00286F80(void) {
    return D_003DC5F0[0];
}

u32 func_00286F90(void) {
    return D_003DC608[0];
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00286FA0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287040);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002870D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287138);

u8 func_00287198(s32 arg0) {
    return *(u8 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 1);
}

f32 func_002871C0(s32 arg0) {
    return *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x18);
}

void func_002871E8(s32 arg0, f32 *arg1) {
    arg1[0] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x1c);
    arg1[1] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x20);
    arg1[2] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x24);
}

void func_00287220(s32 arg0, f32 *arg1) {
    arg1[0] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x2c);
    arg1[1] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x30);
    arg1[2] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x34);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287258);

void func_00287420(f32 arg0) {
    D_003245E0[5] = 2048.0f;
    D_003245E0[4] = arg0 + 2048.0f;
    func_00287258();
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287450);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002874E8);

void func_00287548(void) {
    func_002874E8();
    D_003245E0[4] = 2048.0f;
    D_003245E0[5] = 2048.0f;
}

void func_00287580(s32 arg0, s32 arg1, s32 arg2) {
    D_003DC5EC[0] = func_00217068();
}

void func_002875A8(s32 arg0) {
    func_00287580(D_003DC600[1], *(u8 *)(D_003DC600[-2] + (arg0 & 0xffff) * 60), 0);
}

u32 func_002875E8(u32 *arg0) {
    return *arg0 & 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002875F8);

void func_00287678(void) {
    D_003DC5E8[5] &= ~1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287698);

void func_00287788(u16 arg0, u32 arg1) {
    func_00287698(arg0, 0xffffffffffffffff, arg1);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002877A8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002878D8);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287998);

void func_00287A18(void) {
    *(s32 *)(D_0037CE70 + 0) = 0;
    *(s32 *)(D_0037CE70 + 4) = 0;
    *(f32 *)(D_0037CE70 + 8) = -400.0f;
    *(s32 *)(D_0037CE60 + 0) = 0;
    *(s32 *)(D_0037CE60 + 4) = 0;
    *(s32 *)(D_0037CE60 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287A50);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287B88);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287C20);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287D98);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287E60);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00287EC8);

void func_00287FA8(s32 arg0, f32 arg1, f32 arg2) {
    D_003DC600[6] = 1;
    D_003DC600[7] = arg0;
    D_003DC600[8] = (s32)arg1;
    D_003DC600[9] = (s32)arg2;
}

void func_00287FD0(void) {
    D_003DC618[0] = 4;
}

s32 func_00287FE0(void) {
    s32 temp_v0 = D_003DC618[0];

    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00288008);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00288148);

void func_00288190(void) {
    u32 *temp_v0 = D_003DC5E8;
    u32 temp_v1 = temp_v0[26];

    if (temp_v1 == 0) {
        return;
    }
    func_001F3200(temp_v1);
    temp_v0[26] = 0;
}

void func_002881D0(u32 arg0) {
    D_003DC654[0] = arg0;
}

s32 func_002881E0(void) {
    return D_003DC650[0] != 0;
}

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2520);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2530);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2540);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2550);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002881F0);

u32 func_00288458(void) {
    func_002881F0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00288478);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B25A0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B25B0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00288500);

void func_002886A0(void) {
    func_0021FE38();
}

void func_002886B8(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B2608, 1);
    func_00104600(0);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002886E0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00288728);

void func_00288788(void) {
    func_00288728();
}



INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2608);

