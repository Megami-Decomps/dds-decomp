#include "common.h"

extern s32 scrReadIntParameter(s32);
/* The native VM commands pass operands to these parameterless party stubs. */
extern u32 func_0011B140();
extern u32 func_0011B148();
extern u32 func_0011B150();

extern u32 dds3OwnedNodeListHead;

typedef struct Dds3Node Dds3Node;

/* Intrusive list node callbacks; font nodes own a glyph at +0x10. */
typedef struct Dds3NodeVTable {
    void (*destroy)(Dds3Node *node);
    void (*update)(Dds3Node *node);
} Dds3NodeVTable;

struct Dds3Node {
    struct Dds3Node *prev;
    struct Dds3Node *next;
    s32 value;
    Dds3NodeVTable *vtable;
    u32 glyph;
};

extern s32 func_001951C8(u32, s32, s32, s32, s32);
extern void frFontSetContextPair(s32, u32, u32);
extern s32 sdfAllocAndClearQuadwords(s32);
extern Dds3NodeVTable dds3FontNodeVTable;

/* Remove the party unit specified by script operand 0 and return success to
 * the script VM, while writing whether a unit was actually removed. */
u32 ptyScriptRemoveUnitAndReturnResult(void) {
    scrSetIntegerReturnValue(ptyRemoveUnit(scrReadIntParameter(0)) == 1);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
u32 func_0011CEF0(void) {
    s32 firstOperand;
    s32 secondOperand;

    firstOperand = scrReadIntParameter(0);
    secondOperand = scrReadIntParameter(1);
    firstOperand = func_0011B140(firstOperand, secondOperand);
    scrSetIntegerReturnValue(firstOperand);
    return 1;
}

u32 func_0011CF38(void) {
    s32 firstOperand;
    s32 secondOperand;

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
    dds3AppendIntrusiveNode((s32 *)&dds3OwnedNodeListHead, (s32)node, 0);
    node->vtable = vtable;
}

void dds3DestroyLinkedNode(Dds3Node *node) {
    dds3UnlinkNodeFromList((s32 *)&dds3OwnedNodeListHead, (s32)node, 0);
    node->vtable->destroy(node);
}

void dds3DestroyAllOwnedIntrusiveNodes(void) {
    Dds3Node *current;
    while ((current = (Dds3Node *)dds3OwnedNodeListHead) != 0) {
        dds3DestroyLinkedNode(current);
    }
}

void dds3SetLinkedNodeValue(Dds3Node *node, u32 value) {
    node->value = value;
}

void dds3DestroyNodesWithValue(s32 value) {
    Dds3Node *node = (Dds3Node *)dds3OwnedNodeListHead;

    while (node != NULL) {
        Dds3Node *next = node->next;
        if (node->value == value) {
            dds3DestroyLinkedNode(node);
        }
        node = next;
    }
}

void dds3UpdateLinkedNodes(void) {
    Dds3Node *node = (Dds3Node *)dds3OwnedNodeListHead;

    while (node != NULL) {
        node->vtable->update(node);
        node = node->next;
    }
}

void frFontSubmitAndFreeGlyphOwner(Dds3Node *node) {
    frFontQueueGlyphInSelectedSlot(node->glyph);
    sdfReleaseChipBlock(node);
}

void frFontDrawOwnedGlyph(Dds3Node *node) {
    frFontDrawGlyphInDefaultMode(node->glyph);
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
    node->glyph = obj;
    dds3RegisterOwnedIntrusiveNode(node, &dds3FontNodeVTable);
    return node;
}

void itfConfigureOwnedGlyphChainFlag(Dds3Node *node, u8 value) {
    frFontSetChainFlag(node->glyph, value);
}

void frFontReleaseOwnerStorage(void) {
    sdfReleaseChipBlock();
}

Dds3NodeVTable dds3FontNodeVTable __attribute__((section(".sdata"))) = {
    frFontSubmitAndFreeGlyphOwner,
    frFontDrawOwnedGlyph,
};
