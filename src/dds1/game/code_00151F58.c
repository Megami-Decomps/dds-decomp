#include "common.h"

extern s32 D_003D6438[];
extern s32 D_003D6480[];

s32 func_002D3FD0(s32 arg0);
void func_002D4010(s32 arg0);
s32 func_00151398(s32 arg0, s32 arg1);
void func_00152E40(s32 arg0, s32 arg1);
void func_00153128(s32 arg0, s32 arg1);
void func_00153740(s32 arg0);
void func_001539D0(s32 arg0);
extern void *memset(void *s, s32 c, u32 n);
extern void *memcpy(void *dest, const void *src, u32 n);
extern void *func_002CFEB8(s32 size);
u32 func_00151FC0(void) {
    return 0xf;
}

s32 func_00151FC8(s32 arg0) {
    return *(s32 *)(*(s32 *)(D_003D6438[arg0] + 0x30));
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00151FE8);

void func_00151FF8(s32 arg0, float arg1) {
    *(float *)(arg0 + 0x20) = arg1;
}

void func_00152000(s32 arg0, float arg1, float arg2) {
    *(float *)(arg0 + 0x10) = arg1;
    *(float *)(arg0 + 0x14) = arg2;
}

void func_00152010(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152018);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152050);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152100);

s32 func_00152170(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        return *(s32 *)(*(s32 *)(arg0 + 0x30));
    }
    return 0;
}

u16 func_00152190(s32 arg0) {
    return *(u16 *)(arg0 + 0x2c);
}

void func_00152198(s32 arg0, s32 arg1) {
    s32 v = arg1 & 0xffff;

    switch (*(u16 *)(arg0 + 0x2c)) {
    case 0:
        *(s16 *)(*(s32 *)(arg0 + 0x30) + 4) = v;
        break;
    case 1:
        *(s16 *)(arg0 + 0x3c) = v;
        break;
    }
}

u16 func_001521D0(s32 arg0) {
    switch (*(u16 *)(arg0 + 0x2c)) {
    case 0:
        return *(u16 *)(*(s32 *)(arg0 + 0x30) + 4);
    case 1:
        return *(u16 *)(arg0 + 0x3c);
    default:
        return 0;
    }
}

void func_00152200(s32 arg0, s32 arg1) {
    if ((*(u16 *)(arg0 + 0x2c) == 1) && (*(s32 *)(arg0 + 0x58) != arg1)) {
        func_001518D8(arg0, arg1);
    }
}

s32 func_00152240(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x58);
    }
    return 0;
}

s32 func_00152260(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 4);
    }
    return 0;
}

void func_00152288(s32 arg0, u32 arg1) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        s32 n = *(s32 *)(arg0 + 0x5c);

        if (n > 0) {
            s32 p = *(s32 *)(arg0 + 0x60);
            s32 i = n;

            do {
                s32 q = *(s32 *)(p + 0xc);
                u32 r = arg1 % *(u32 *)(q + 0xc);
                i -= 1;
                *(s32 *)(p + 8) = 0;
                *(u32 *)(p + 4) = r;
                p += 0x14;
            } while (i != 0);
        }
    }
}

void func_001522E8(s32 arg0, u32 arg1) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        s32 n = *(s32 *)(arg0 + 0x5c);

        if (n > 0) {
            s32 p = *(s32 *)(arg0 + 0x60);
            s32 i = n;

            do {
                s32 q = *(s32 *)(p + 0xc);
                u32 r = arg1 % *(u32 *)(q + 0xc);
                i -= 1;
                *(s32 *)(p + 8) = 1;
                *(u32 *)(p + 4) = r;
                p += 0x14;
            } while (i != 0);
        }
    }
}

s32 func_00152348(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x60) + 0xc) + 0xc);
    }
    return 0;
}

u16 func_00152370(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(u16 *)(arg0 + 0x50);
    }
    return 0;
}

s32 func_00152390(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x54);
    }
    return 0;
}

void func_001523B0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        *(s32 *)(arg0 + 0x54) |= 0x1000000;
    }
}

void func_001523D8(s32 arg0, float arg1, float arg2) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        s32 tmp = *(s32 *)(arg0 + 0x30);
        *(float *)(tmp + 0x24) = arg1 * 0.5f;
        *(float *)(tmp + 0x28) = arg2 * 0.5f;
    }
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152408);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152560);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001525C8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152650);

void func_00152720(u32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x84));
    func_00151F00(*(u32 *)(arg0 + 0x80));
    func_002CFF98(arg0);
}

void func_00152758(s32 arg0, s128 *arg1) {
    func_00151FE8(*(s128 **)(arg0 + 0x80), arg1);
}

void func_00152770(s32 arg0, float arg1) {
    func_00152000(*(s32 *)(arg0 + 0x80), arg1, arg1);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152790);

void func_001527C0(s32 arg0, u32 arg1) {
    func_00152010(*(s32 *)(arg0 + 0x80), arg1);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001527D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152800);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001528C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152E40);

void func_001530C0(s32 arg0, s32 arg1) {
    s32 tmp = func_002D3FD0(0x20);

    func_002D4010(tmp);
    func_00152E40(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153128);

void func_00153618(s32 arg0, s32 arg1) {
    s32 tmp = func_002D3FD0(0x20);

    func_002D4010(tmp);
    func_00153128(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

void func_00153680(void) {
    func_002E84A0(&D_003D6480);
}

void func_001536A0(void) {
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001536A8);

void func_00153728(u32 *arg0) {
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153740);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153920);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001539D0);

void func_00153A40(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_00153A90(s32 arg0) {
    s32 obj = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, (void *)arg0, *(s32 *)(arg0 + 0xa0));
    memcpy((void *)(obj + 0x150), (void *)(arg0 + *(s32 *)(arg0 + 0xa0)), tailLen);
    func_00153740(obj);
    func_001539D0(obj);
    return obj;
}

void func_00153B10(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153B38);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153D98);

void func_00154048(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00154090(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001540E0);

void func_00154160(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154188);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154430);

void func_00154698(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_001546E0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154730);

void func_001547B0(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001547D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154FD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155200);

void func_001552A0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001552D8);

void func_00155358(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155380);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155618);

void func_00155830(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00155878(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001558B8);

void func_00155938(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155960);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155CC0);

void func_00155F30(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00155F78(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155FC8);

void func_00156048(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156070);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156378);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001565E0);

void func_00156650(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156688);

void func_00156708(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156730);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156920);

void func_00156B98(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00156BE0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156C30);

void func_00156CB0(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156CD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156F30);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157140);

void func_00157188(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001571D8);

void func_00157258(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157280);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001575D0);

void func_00157970(void) {
    func_00157A58();
}

void func_00157988(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001579B0);

void func_00157A30(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157A58);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157B58);

void func_00157BE8(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00157C30(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157C80);

void func_00157D00(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157D28);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001584B8);

void func_00158718(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00158760(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001587B0);

void func_00158848(u32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x17c));
    func_00153920(arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158880);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158B10);

void func_00158D88(s32 arg0) {
    s32 p;
    u32 i;

    i = 0;
    p = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(p + 0x20) = 0xf0000001;
            i = i + 1;
            p = p + 0x40;
        } while (i < *(u32 *)(arg0 + 0x20));
    }
}

void func_00158DD0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158E10);
