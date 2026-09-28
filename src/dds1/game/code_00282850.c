#include "common.h"

extern s32 D_003BC7B4;

extern s32 D_003BAA4C;

extern s32 D_003BAA50;

extern u32 func_002BD258(u32);

extern s32 func_002860B8(u16);

extern u32 func_0027D4A0(u32);

extern s32 func_002CFEB8(u32);

extern s32 func_002877A8(void);
extern void func_002830F8(s32);

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

extern u32 D_003DC5EC[];

extern void func_002874E8(void);

extern void func_00288500(void);

INCLUDE_ASM(const s32, "game/code_00282850", func_00282850);

INCLUDE_ASM(const s32, "game/code_00282850", func_002828D0);

void func_002829C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002829E0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00282B08);

void func_00282BE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_00282B08(arg0, arg1, arg2, 0, arg3, arg4);
}

void *func_00282C10(s32 width, s32 height) {
    u8 *item = func_002CFEB8(0x64);
    memset(item, 0, 0x64);
    *(s32 *)(item + 0xC) = width;
    *(s32 *)(item + 0x10) = height;
    return item;
}

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00282DA0);

s32 func_00282F98(s32 parent) {
    s32 *list = func_002CFEB8(0x28);
    s32 i;
    for (i = 0; i < 5; i++) {
        s32 child = func_00284888();
        func_002848E0(child, parent, i);
        list[i + 3] = child;
    }
    func_002830F8(list);
    list[9] = 0x100;
    return (s32)list;
}

void func_00283038(s32 *list) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_00284C30(list[i + 3]);
    }
    func_002CFF98(list);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283090);

void func_002830F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_002830F8(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xffffffff;
}

u32 func_00283108(s32 arg0) {
    return *(u32 *)(arg0 + 0x20);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283110);

INCLUDE_ASM(const s32, "game/code_00282850", func_00283238);

void *func_00283280(s32 x, s32 y, s32 z) {
    u8 *item = func_002CFEB8(0x20);
    memset(item, 0, 0x20);
    *(s32 *)(item + 0x10) = x;
    *(s32 *)(item + 0x14) = y;
    *(s32 *)(item + 0x18) = z;
    *(s32 *)(item + 0x1C) = 0x100;
    return item;
}

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

INCLUDE_ASM(const s32, "game/code_00282850", func_002833B0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00283788);

void func_00283820(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283838);

void func_00283BE0(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 4) = arg1;
    *(s32 *)arg0 = 0;
    *(s32 *)(arg0 + 8) = 0;
}

void func_00283BF0(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283BF8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00283CA8);

s32 func_00283CE8(s32 arg0) {
    s32 temp_v0 = *(s32 *)(arg0 + 0x10);

    if (temp_v0 < 0x32) {
        return (temp_v0 >= 0x14) ? 1 : 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283D10);

INCLUDE_ASM(const s32, "game/code_00282850", func_00283E60);

INCLUDE_ASM(const s32, "game/code_00282850", func_00283EE0);

void func_00284080(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_002BD258(3);
    *(u32 *)(arg0 + 0x40) = temp_v0;
    temp_v0 = func_002BD258(3);
    *(u32 *)(arg0 + 0x44) = temp_v0;
}

void func_002840B8(s32 *list) {
    u32 i;
    for (i = 0; i < 2; i++) {
        func_002BD2F8(list[i + 16]);
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00284108);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284258);

void func_00284340(s32 *object) {
    u32 i;
    for (i = 0; i < 9; i++) {
        func_002BDD60(object[i + 7]);
    }
    func_002840B8(object);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002843A0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284418);

INCLUDE_ASM(const s32, "game/code_00282850", func_002845F8);

void func_00284880(void) {
}

s32 func_00284888(void) {
    s32 item = func_002CFEB8(0x90);
    memset(item, 0, 0x90);
    *(s32 *)(item + 0x14) = 0x63;
    *(s32 *)(item + 0x10) = 0x8c;
    *(s32 *)(item + 0x88) = 0x100;
    return item;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002848E0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284A90);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00284C48);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284EB8);

void func_002850C8(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0x10) = arg1;
    *(s32 *)(arg0 + 0x14) = arg2;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002850D8);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00285208);

INCLUDE_ASM(const s32, "game/code_00282850", func_00285440);

void func_00285490(u32 arg0) {
    memset(arg0, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002854B0);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00285670);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00285960);

INCLUDE_ASM(const s32, "game/code_00282850", func_00285A68);

INCLUDE_ASM(const s32, "game/code_00282850", func_00285B20);

INCLUDE_ASM(const s32, "game/code_00282850", func_00285E00);

INCLUDE_ASM(const s32, "game/code_00282850", func_00285F00);

void func_00286050(u32 arg0) {
    func_00285F00(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00286068);

INCLUDE_ASM(const s32, "game/code_00282850", func_002860B8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286138);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286170);

INCLUDE_ASM(const s32, "game/code_00282850", func_002861B8);

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

u16 func_002862E0(s32 id, s32 object) {
    s32 entry = (id & 0xFFFF) * 0x38 + D_003BAA50;
    u16 base = *(u16 *)(entry + 4);
    u16 addition = *(u16 *)(entry + 6);
    if (func_002862C0(id & 0xFFFF) == 1) {
        base = addition + *(u16 *)(object + 8) * base / 100;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00286368);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286440);

INCLUDE_ASM(const s32, "game/code_00282850", func_002864D8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286540);

INCLUDE_ASM(const s32, "game/code_00282850", func_002865B8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286648);

void func_002866B0(u16 arg0) {
    func_00118E38(arg0);
}

u32 func_002866C8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002866D0);

u8 func_002868C0(u32 arg0) {
    return *(s8 *)((arg0 & 0xffff) * 2 + D_003BAA4C) == '\x01';
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002868E0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286990);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00286A18);

u32 func_00286AC0(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x52) = arg1;
    return 1;
}

u16 func_00286AD0(s32 arg0) {
    return *(u16 *)(arg0 + 0x52);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00286AD8);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2420);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2430);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2450);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2468);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2480);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B24A8);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B24D0);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B24E8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286B48);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286D20);

s32 func_00286E50(s32 entry) {
    u16 flags = *(u16 *)(entry + 0xe);
    if (flags & 0x400) return 0;
    if (flags & 0x100) return 1;
    if (flags & 0x80) return 2;
    if (flags & 0x40) return 3;
    if (flags & 0x10) return 4;
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00286EA0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00286EF8);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00286FA0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287040);

INCLUDE_ASM(const s32, "game/code_00282850", func_002870D8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287138);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00287258);

void func_00287420(f32 arg0) {
    D_003245E0[5] = 2048.0f;
    D_003245E0[4] = arg0 + 2048.0f;
    func_00287258();
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287450);

void func_002874E8(void) {
    if (D_003DC5E8[0] != 1) {
        D_003DC5E8[27] = -1;
        if (D_003DC5E8[26] != 0) {
            func_00288190();
        }
        if (D_003DC5E8[2] != 0) {
            func_002177D0(D_003DC5E8[2]);
            D_003DC5E8[2] = 0;
        }
    }
}

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

INCLUDE_ASM(const s32, "game/code_00282850", func_002875F8);

void func_00287678(void) {
    D_003DC5E8[5] &= ~1;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287698);

void func_00287788(u16 arg0, u32 arg1) {
    func_00287698(arg0, 0xffffffffffffffff, arg1);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002877A8);

INCLUDE_ASM(const s32, "game/code_00282850", func_002878D8);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287998);

void func_00287A18(void) {
    *(s32 *)(D_0037CE70 + 0) = 0;
    *(s32 *)(D_0037CE70 + 4) = 0;
    *(f32 *)(D_0037CE70 + 8) = -400.0f;
    *(s32 *)(D_0037CE60 + 0) = 0;
    *(s32 *)(D_0037CE60 + 4) = 0;
    *(s32 *)(D_0037CE60 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287A50);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287B88);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287C20);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287D98);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287E60);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287EC8);

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

INCLUDE_ASM(const s32, "game/code_00282850", func_00288008);

void func_00288148(void) {
    if (D_003DC5E8[26] != 0) {
        func_00288190();
    }
    D_003DC5E8[26] = func_001F3118(D_003DC5E8[2], 0x30);
}

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

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2520);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2530);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2540);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2550);

INCLUDE_ASM(const s32, "game/code_00282850", func_002881F0);

u32 func_00288458(void) {
    func_002881F0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00288478);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B25A0);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B25B0);

INCLUDE_ASM(const s32, "game/code_00282850", func_00288500);

void func_002886A0(void) {
    func_0021FE38();
}

void func_002886B8(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B2608, 1);
    func_00104600(0);
}

void func_002886E0(void) {
    func_00104600(1);
    kwlnTaskCreate(D_003B2608, 0x2b0c, 1, 1, func_00288500, func_002886A0, 0);
}

s32 func_00288728(object)
    s32 object;
{
    if (*(u8 *)(object + 1) == 6) {
        s32 resource = *(s32 *)(object + 0xC);
        if (resource != 0) {
            func_002E6E88(resource);
        }
        func_002EDC50(object + 0x30);
        func_002CFF98(*(void **)(object + 8));
        func_002CFF98((void *)object);
        return 0;
    }
    return 1;
}

void func_00288788(void) {
    func_00288728();
}








INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2608);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC778);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC780);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC788);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC790);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC798);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7A0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7A8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B4);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7BC);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7C0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7C8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7D0);


INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7D4);

