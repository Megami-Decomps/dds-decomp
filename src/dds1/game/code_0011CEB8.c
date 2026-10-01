#include "common.h"

extern u64 scrReadIntParameter(u64);
extern u64 func_0011B140(u64, u64);
extern u32 func_0011B148(u64, u64);
extern u32 func_0011B150(u64);

extern u32 D_003BD7A8;

/* Intrusive list node: next at +4, payload at +8, vtable at +0xC. */
typedef struct Dds3NodeVTable {
    void (*destroy)(s32);
    void (*update)(s32);
} Dds3NodeVTable;

typedef struct Dds3Node {
    s32 unk0;
    struct Dds3Node *next;
    s32 value;
    Dds3NodeVTable *vtable;
    s32 unk10;
} Dds3Node;

extern s32 func_001951C8(u32, s32, s32, s32, s32);
extern void frFontSetContextPair(s32, u32, u32);
extern s32 sdfAllocAndClearQuadwords(s32);
extern u32 D_003BAAD0;

/* Shared glyph owner layout: handle at +0x10, released by the font routines. */
typedef struct GlyphOwner {
    u8 pad00[0x10];
    u32 glyph;
} GlyphOwner;

/* Remove the party unit specified by script operand 0 and return success to
 * the script VM, while writing whether a unit was actually removed. */
u32 ptyScriptRemoveUnitAndReturnResult(void) {
    scrSetIntegerReturnValue(ptyRemoveUnit(scrReadIntParameter(0)) == 1);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
u32 func_0011CEF0(void) {
    u64 firstOperand;
    u64 secondOperand;

    firstOperand = scrReadIntParameter(0);
    secondOperand = scrReadIntParameter(1);
    firstOperand = func_0011B140(firstOperand, secondOperand);
    scrSetIntegerReturnValue(firstOperand);
    return 1;
}

u32 func_0011CF38(void) {
    u64 firstOperand;
    u64 secondOperand;

    firstOperand = scrReadIntParameter(0);
    secondOperand = scrReadIntParameter(1);
    scrSetIntegerReturnValue(func_0011B148(firstOperand, secondOperand) == 1);
    return 1;
}

u32 func_0011CF88(void) {
    scrSetIntegerReturnValue(func_0011B150(scrReadIntParameter(0)) == 1);
    return 1;
}

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

void dds3UnlinkNodeFromList(s32 *list, s32 node, s32 linkOffset) {
    s32 prev = *(s32 *)(node + linkOffset);
    s32 next = *(s32 *)(node + linkOffset + 4);

    if (prev == 0) {
        list[0] = next;
    } else {
        *(s32 *)(prev + linkOffset + 4) = next;
    }
    if (next == 0) {
        list[1] = prev;
    } else {
        *(s32 *)(next + linkOffset) = prev;
    }
}

void dds3RegisterOwnedIntrusiveNode(Dds3Node *node, Dds3NodeVTable *vtable) {
    dds3AppendIntrusiveNode((s32 *)&D_003BD7A8, (s32)node, 0);
    node->vtable = vtable;
}

void dds3DestroyLinkedNode(Dds3Node *node) {
    dds3UnlinkNodeFromList((s32 *)&D_003BD7A8, (s32)node, 0);
    node->vtable->destroy((s32)node);
}

void dds3DestroyAllOwnedIntrusiveNodes(void) {
    u32 current;
    while ((current = D_003BD7A8) != 0) {
        dds3DestroyLinkedNode(current);
    }
}

void dds3SetLinkedNodeValue(s32 node, u32 value) {
    *(u32 *)(node + 8) = value;
}

void dds3DestroyNodesWithValue(s32 value) {
    Dds3Node *node = (Dds3Node *)D_003BD7A8;

    while (node != NULL) {
        Dds3Node *next = node->next;
        if (node->value == value) {
            dds3DestroyLinkedNode(node);
        }
        node = next;
    }
}

void dds3UpdateLinkedNodes(void) {
    Dds3Node *node = (Dds3Node *)D_003BD7A8;

    while (node != NULL) {
        node->vtable->update((s32)node);
        node = node->next;
    }
}

void frFontSubmitAndFreeGlyphOwner(GlyphOwner *owner) {
    frFontQueueGlyphInSelectedSlot(owner->glyph);
    sdfReleaseChipBlock(owner);
}

void func_0011D1A8(GlyphOwner *owner) {
    func_00195868(owner->glyph);
}

Dds3Node *dds3CreateFontNode(u32 arg0, u32 arg1, u32 arg2) {
    s32 obj;
    Dds3Node *node;

    obj = func_001951C8(arg2, 0, 0, 0, 0);
    if (arg0 == 0x800000) {
        arg0 = (0x200 - *(s32 *)(obj + 0xC)) * 8;
    }
    frFontSetContextPair(obj, arg0, arg1);
    node = (Dds3Node *)sdfAllocAndClearQuadwords(0x14);
    node->unk10 = obj;
    dds3RegisterOwnedIntrusiveNode(node, (Dds3NodeVTable *)&D_003BAAD0);
    return node;
}

void func_0011D258(GlyphOwner *owner, u8 value) {
    frFontSetChainFlag(owner->glyph, value);
}

void func_0011D278(void) {
    sdfReleaseChipBlock();
}

INCLUDE_SDATA(const s32, "game/code_0011CEB8", D_003BAAD0);

