#include "common.h"

extern s32 func_00309638(u32);

extern s32 func_0030A048(u32, u32);

typedef struct QuadU32 {
    u32 x; // 0x00
    u32 y; // 0x04
    u32 z; // 0x08
    u32 w; // 0x0C
} QuadU32; // 0x10

INCLUDE_ASM(const s32, "game/code_00306F80", func_00306F80);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307018);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307160);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003071D0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307270);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003072E8);

void func_00307340(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307388);

void func_00307398(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    func_0032C9D8(arg4, arg0, 0, 0, arg1, arg2, arg3, 0);
}

u8 func_003073D0(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003073D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307428);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003074F0);

void func_003075A0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg1 * 4 + *(s32 *)(arg0 + 0x24)) + 0x28);
    *(u64 *)(temp_v0 + 0x20) = (*(u64 *)(temp_v0 + 0x20) & 0x1fffffffffffffff) | 0x4000000000000000;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003075D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307710);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003078A8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307A68);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307BA8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307C30);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307D70);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00307EF8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308020);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308058);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003081A8);

void func_00308288(u8 arg0, u32 arg1) {
    func_003081A8(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003082A8);

void func_00308380(u32 arg0, u32 arg1) {
    func_003082A8(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003083A0);

void func_00308478(u32 arg0, u32 arg1) {
    func_003083A0(arg0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308498);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308550);

void func_00308608(QuadU32 *q, u32 value) {
    q->x = value;
    q->y = value;
    q->z = value;
    q->w = value;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308620);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308650);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003087D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308808);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308828);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003089B8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003089D8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308AC8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308AF0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308C58);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308DB0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308E60);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F10);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308F78);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00308FE8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309050);

void func_00309090(u32 arg0) {
    func_00308380(0x30000, arg0);
    func_00308478(0x44, arg0);
    func_00308808(0, 0, 0, 0x2000, 0xe00, 0, arg0);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_003090E8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309138);

void func_003091E8(void) {
}

void func_003091F0(void) {
}

u32 func_003091F8(void) {
    return 0;
}

s32 func_00309200(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    *(s32 *)(arg0 + 0xC) = (*(s32 *)(arg0 + 0xC) & -2) | 2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309230);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309278);

void func_00309390(u8 *work, s32 columns, s32 rows) {
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

u32 func_003093D0(u32 arg0) {
    s64 temp_v0;

    func_00328E48(*(u32 *)arg0);
    do {
        temp_v0 = func_00309638(arg0);
    } while (temp_v0 != 0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309418);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309480);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003094C8);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309538);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309638);

INCLUDE_ASM(const s32, "game/code_00306F80", func_003097D0);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309880);

void func_00309A20(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_0030A048(arg1, arg0);
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

void func_00309A90(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_00309A20(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

void func_00309AD8(u32 arg0, u32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    float *pfVar3;
    float temp_v2;
    float temp_v3;

    temp_v1 = func_0030A048(arg1, arg0);
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

void func_00309B48(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 0xc) & 1) != 0) {
        func_00309AD8(arg0, (u32)*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 6) + *(s32 *)(temp_v0 + 0x3c),
                                    arg1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309B90);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309C00);

INCLUDE_ASM(const s32, "game/code_00306F80", func_00309DF8);

s32 func_0030A048(u32 key, u32 head) {
    u32 n;

    n = *(u32 *)(head + 0x14);
    while (n != 0 && *(u16 *)(n + 6) != key) {
        n = *(u32 *)(n + 0x1C);
    }
    return n;
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_0030A070);

void func_0030A0E8(u32 arg0) {
    func_0030A070(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_00306F80", func_0030A108);
INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438868);

INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438870);

INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438878);


INCLUDE_SDATA(const s32, "game/code_00306F80", D_00438880);

