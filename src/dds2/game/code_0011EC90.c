#include "common.h"

extern u32 func_002AB598(void);

extern s32 func_0011C0B0(s32 param0, s32 param1);

extern s32 func_0010D650(s32 idx);

extern s32 func_0010D818(s32 arg0);

typedef struct GlyphOwner {
    u8 pad00[0x10];
    u32 glyph;
} GlyphOwner;

s32 func_0011EC90(void) {
    s32 val = func_0010D650(0);

    func_0010D818(func_0011C680(val) == 1);
    return 1;
}

s32 func_0011ECC8(void) {
    s32 firstOperand = func_0010D650(0);
    s32 secondOperand = func_0010D650(1);

    func_0010D818(func_0011C0B0(firstOperand, secondOperand));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED10);

s32 func_0011ED60(void) {
    func_0010D818(func_002AB598());
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED88);

void dds3AppendIntrusiveNode(s32 *list, s32 node, s32 linkOffset) {
    s32 last;

    last = list[1];
    if (last == 0) {
        *list = node;
    }
    else {
        *(s32 *)(last + linkOffset + 4) = node;
    }
    *(s32 *)(node + linkOffset) = last;
    ((s32 *)(node + linkOffset))[1] = 0;
    list[1] = node;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EE58);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EE98);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EED8);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EF18);

void func_0011EF48(s32 object, u32 value) {
    *(u32 *)(object + 8) = value;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EF50);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EFA0);

void func_0011EFE0(GlyphOwner *owner) {
    func_0019C5B0(owner->glyph);
    func_00328E48(owner);
}

void func_0011F010(GlyphOwner *owner) {
    func_0019D518(owner->glyph);
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F028);

void func_0011F0C0(GlyphOwner *owner, u8 flag) {
    func_0019D120(owner->glyph, flag);
}

void func_0011F0E0(void) {
    func_00328E48();
}

INCLUDE_SDATA(const s32, "game/code_0011EC90", D_00435EA8);
