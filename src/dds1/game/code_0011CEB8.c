#include "common.h"

extern u64 scrReadIntParameter(u64);
extern u64 func_0011B140(u64, u64);

extern u32 D_003BD7A8;

/* Shared glyph owner layout: handle at +0x10, released by the font routines. */
typedef struct GlyphOwner {
    u8 pad00[0x10];
    u32 glyph;
} GlyphOwner;

/* Remove the party unit specified by script operand 0 and return success to
 * the script VM, while writing whether a unit was actually removed. */
u32 func_0011CEB8(void) {
    func_0010D5F0(ptyRemoveUnit(scrReadIntParameter(0)) == 1);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
u32 func_0011CEF0(void) {
    u64 firstOperand;
    u64 secondOperand;

    firstOperand = scrReadIntParameter(0);
    secondOperand = scrReadIntParameter(1);
    firstOperand = func_0011B140(firstOperand, secondOperand);
    func_0010D5F0(firstOperand);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CF38);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CF88);

/* Append an intrusive node; linkOffset selects its previous/next pair. */
void dds3AppendLinkedNode(s32 *list, s32 node, s32 linkOffset) {
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

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CFF0);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D030);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D070);

void func_0011D0B0(void) {
    u32 current;
    while ((current = D_003BD7A8) != 0) {
        func_0011D070(current);
    }
}

void func_0011D0E0(s32 node, u32 value) {
    *(u32 *)(node + 8) = value;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D0E8);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D138);

void func_0011D178(GlyphOwner *owner) {
    func_00194920(owner->glyph);
    func_002CFF98(owner);
}

void func_0011D1A8(GlyphOwner *owner) {
    func_00195868(owner->glyph);
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D1C0);

void func_0011D258(GlyphOwner *owner, u8 value) {
    frFontSetChainFlag(owner->glyph, value);
}

void func_0011D278(void) {
    func_002CFF98();
}

INCLUDE_SDATA(const s32, "game/code_0011CEB8", D_003BAAD0);

