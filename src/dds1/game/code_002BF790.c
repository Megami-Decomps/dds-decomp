#include "common.h"

extern s32 func_002C2540(u32, u32);

extern s32 func_002C1B30(u32);

typedef struct IntPair {
    s32 x; // 0x00
    s32 y; // 0x04
} IntPair; // 0x08

typedef struct QuadU32 {
    u32 x; // 0x00
    u32 y; // 0x04
    u32 z; // 0x08
    u32 w; // 0x0C
} QuadU32; // 0x10

extern s32 func_002C2568(s32, void *);

void func_002BF790(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 entry = func_002BD398(e, f);
    func_002BF400(a, b, c, d, e, f, entry, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BF828);

s32 func_002BF970(s32 object, s32 key) {
    s32 entry = func_002BD398(object);
    s32 result;

    if (*(s32 *)(entry + 0x30) == 0) {
        func_002BF828(object, key);
    }
    result = func_002BDF28(object, key, entry);
    if (result == 0) {
        result = *(s32 *)(object + 0x28);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BF9E0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFA80);

void func_002BFAF8(s32 a, s32 b, s32 x, s32 y, s32 width, s32 height) {
    s32 object = func_002BD398(a, b);
    *(s32 *)(object + 0x50) = x;
    *(s32 *)(object + 0x54) = y;
    *(s32 *)(object + 0x58) = width;
    *(s32 *)(object + 0x5c) = height;
}

void func_002BFB50(s32 arg0, s32 arg1) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 3;
    puVar1 = (u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x14);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = puVar1[0x1c];
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

void func_002BFB98(IntPair *p, s32 a, s32 b) {
    p->x = a;
    p->y = b;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFBA8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFCE0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002BFE78);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0038);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0178);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0200);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0340);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C04C8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C05F0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0628);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0778);

void func_002C0858(u8 arg0, u32 arg1) {
    func_002C0778(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0878);

void func_002C0950(u32 arg0, u32 arg1) {
    func_002C0878(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0970);

void func_002C0A48(u32 arg0, u32 arg1) {
    func_002C0970(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0A68);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0B20);

void func_002C0BD8(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

void func_002C0BF0(u32 a, u32 b, u32 c, u32 value, u32 e, u32 f, u32 g, u32 h) {
    u32 rgb[3] = {value, value, value};
    func_002C0C20(a, b, c, rgb, e, f, g, h);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0C20);

void func_002C0DA8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 value, u32 g, u32 h) {
    u32 rgba[4] = {value, value, value, value};
    func_002C0DF8(a, b, c, d, e, rgba, g, h);
}

void func_002C0DD8(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002C0DA8(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0DF8);

void func_002C0F88(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002C0DF8(a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C0FA8);

void func_002C1098(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 value) {
    u32 range[2] = {value, value};
    func_002C10C0(a, b, c, d, e, f, range);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C10C0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1228);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1380);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1430);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C14E0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1548);

void func_002C1588(u32 arg0) {
    func_002C0950(0x30000, arg0);
    func_002C0A48(0x44, arg0);
    func_002C0DD8(0, 0, 0, 0x2000, 0xe00, 0, arg0);
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C15E0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1630);

void func_002C16E0(void) {
}

void func_002C16E8(void) {
}

u32 func_002C16F0(void) {
    return 0;
}

s32 func_002C16F8(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    *(s32 *)(arg0 + 0xC) = (*(s32 *)(arg0 + 0xC) & -2) | 2;
    return 1;
}

s32 setWidgetFlagsAndActivateChild(u8 *object, u32 flags) {
    s32 child;
    if (object == 0) {
        return 0;
    }
    *(u32 *)(object + 0xc) = (*(u32 *)(object + 0xc) & ~2) | flags;
    child = *(s32 *)(object + 0x18);
    if (child != 0) {
        s32 node = *(s32 *)(child + 0x20);
        if (node != 0) {
            *(u32 *)(node + 0xc) |= 2;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1770);

void setGridDimensions(u8 *work, s32 columns, s32 rows) {
    s32 columnWidth = columns * 12 + 6;
    s32 rowHeight = rows * 14 + 6;

    if (columns != 0) {
        *(s32 *)(work + 0x28) = columnWidth;
    }
    if (rows != 0) {
        *(s32 *)(work + 0x2c) = rowHeight;
        *(s16 *)(work + 6) = rows;
    }
}

u32 func_002C18C8(u32 arg0) {
    s64 temp_v0;

    func_002CFF98(*(u32 *)arg0);
    do {
        temp_v0 = func_002C1B30(arg0);
    } while (temp_v0 != 0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1910);

void expandWidgetColumnWidth(s32 columns, u8 *work) {
    s32 flags = *(s32 *)(work + 0xc);
    s32 width;
    if (flags & 0x100) {
        columns += 4;
        if (flags & 0x200) {
            columns += 2;
        }
    }
    width = columns * 12 + 6;
    if (*(s32 *)(work + 0x28) < width) {
        *(s32 *)(work + 0x28) = width;
    }
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C19C0);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1A30);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1B30);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1CC8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C1D78);

void func_002C1F18(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_002C2540(arg1, arg0);
    temp_v0 = *(s32 *)(temp_v1 + 8);
    pfVar3 = (float *)(temp_v1 + 0xc);
    temp_v2 = *(float *)(temp_v0 + 0xc);
    if (1 < arg2) {
        temp_v2 = temp_v2 * (float)(s32)arg2;
    }
    temp_v3 = *pfVar3;
    *pfVar3 = temp_v3 + temp_v2;
    if (*(float *)(temp_v0 + 8) < temp_v3 + temp_v2) {
        *pfVar3 = *(float *)(temp_v0 + 4);
    }
}

void func_002C1F88(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_002C1F18(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

void func_002C1FD0(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_002C2540(arg1, arg0);
    temp_v0 = *(s32 *)(temp_v1 + 8);
    pfVar3 = (float *)(temp_v1 + 0xc);
    temp_v2 = *(float *)(temp_v0 + 0xc);
    if (1 < arg2) {
        temp_v2 = temp_v2 * (float)(s32)arg2;
    }
    temp_v3 = *pfVar3;
    *pfVar3 = temp_v3 - temp_v2;
    if (temp_v3 - temp_v2 < *(float *)(temp_v0 + 4)) {
        *pfVar3 = *(float *)(temp_v0 + 8);
    }
}

void func_002C2040(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_002C1FD0(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C2088);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C20F8);

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C22F0);

s32 func_002C2540(u32 key, u32 head) {
    u32 n;

    n = *(u32 *)(head + 0x14);
    while (n != 0 && *(u16 *)(n + 6) != key) {
        n = *(u32 *)(n + 0x1C);
    }
    return n;
}

INCLUDE_ASM(const s32, "game/code_002BF790", func_002C2568);

void func_002C25E0(u32 arg0) {
    func_002C2568(0, arg0);
}

s32 func_002C2600(s32 arg0) {
    return func_002C2568(*(s16 *)(arg0 + 0xa) - 1, (void *)arg0);
}

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD218);

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD220);

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD228);

INCLUDE_SDATA(const s32, "game/code_002BF790", D_003BD230);

