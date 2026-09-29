#include "common.h"

typedef struct IconEntry {
    s16 pad0;
    s16 id;
    s16 x;
    s16 y;
} IconEntry;

extern IconEntry D_0036C728[];
extern u32 D_003BC520;
extern void func_002BF4E0(s32, s32, s32, s32, s32, u32, s32, s32);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025DF68);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E108);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E308);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E420);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E508);

void func_0025E5D8(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    u32 layer = D_003BC520;

    func_002BF4E0(x + (D_0036C728[18].x << 4), y + (D_0036C728[18].y << 3), 0, b, 0, layer, D_0036C728[18].id, c);
    func_002BF4E0(D_0036C728[17].x << 4, D_0036C728[17].y << 3, 0, b, 0, layer, D_0036C728[17].id, c);
    func_002BF4E0(D_0036C728[22].x << 4, D_0036C728[22].y << 3, 0, b, 0, layer, D_0036C728[22].id, c);
}

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E6B0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025E820);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025ECD0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F138);

extern void func_0025F408(s32, s32, s32, u8 *, s32);
extern void func_0025F4E0(s32, s32, s32, s32, u8 *, s32);

void func_0025F378(s32 a, s32 b, s32 c, u8 *obj, s32 d) {
    u8 *inner = *(u8 **)(obj + 0x14);

    if (*(s32 *)(inner + 0x20) != 0) {
        func_0025F408(a, b, c, inner, d);
        func_0025F4E0(a, b, c, 0, obj, d);
        *(u32 *)(obj + 4) |= 4;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F408);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F4E0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F680);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025F7F0);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025FB30);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025FC38);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025FD50);

void func_0025FE68(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c) {
    func_002BF4E0(x + (D_0036C728[26].x << 4), y + (D_0036C728[26].y << 3), z, b, 0, D_003BC520, D_0036C728[26].id, c);
}

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025FEB8);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_0025FFC8);

INCLUDE_ASM(const s32, "game/code_0025DF68", func_00260100);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F0);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC4F8);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC500);

INCLUDE_SDATA(const s32, "game/code_0025DF68", D_003BC508);

