#include "common.h"

extern u32 func_002AB598(void);

extern s32 func_0011C0B0(s32 param0, s32 param1);

extern s32 scrReadIntParameter(s32 idx);

extern s32 func_0010D818(s32 arg0);

/* Shared glyph owner layout: handle at +0x10, released by the font routines. */
typedef struct GlyphOwner {
    u8 pad00[0x10];
    u32 glyph;
} GlyphOwner;

extern u32 D_00438EB0;

s32 func_0011EC90(void) {
    s32 unitId = scrReadIntParameter(0);

    func_0010D818(func_0011C680(unitId) == 1);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
s32 func_0011ECC8(void) {
    s32 firstOperand = scrReadIntParameter(0);
    s32 secondOperand = scrReadIntParameter(1);

    func_0010D818(func_0011C0B0(firstOperand, secondOperand));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED10);

s32 func_0011ED60(void) {
    func_0010D818(func_002AB598());
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED88);

/* Append an intrusive node; linkOffset selects its previous/next pair. */
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

void func_0011EF18(void) {
    u32 current;
    while ((current = D_00438EB0) != 0) {
        func_0011EED8(current);
    }
}

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
    frFontSetChainFlag(owner->glyph, flag);
}

void func_0011F0E0(void) {
    func_00328E48();
}

INCLUDE_SDATA(const s32, "game/code_0011EC90", D_00435EA8);

