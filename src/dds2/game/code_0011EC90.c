#include "common.h"

extern u32 mtrHasEnoughOwnedMantras(void);

extern s32 func_0011C0B0(s32 param0, s32 param1);

extern s32 func_0011C340(s32 param0, s32 param1);

extern s32 scrReadIntParameter(s32 idx);

extern s32 dds3FindEntryIndex(s32 rosterIndex);

extern s32 scrSetFlag(u8 *work, u16 index);

extern void scrClearAllSecondaryScriptFlags(u8 *work);

extern void scrSetSecondaryScriptFlag(u8 *work, u16 index);

extern s32 datGameState;

extern s32 scrSetIntegerReturnValue(s32 arg0);

extern void *func_0019CE78(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void frFontSetContextPair(void *ctx, u32 arg1, u32 arg2);

extern void *sdfAllocAndClearQuadwords(s32 size);

/* Shared glyph owner layout: handle at +0x10, released by the font routines. */
typedef struct GlyphOwner {
    u8 pad00[0x10];
    u32 glyph;
} GlyphOwner;

/* Intrusive list node; the virtual table at +0xC drives per-node callbacks. */
typedef struct Dds3NodeOps {
    void (*destroy)(s32 node);
    void (*update)(s32 node);
} Dds3NodeOps;

typedef struct Dds3Node {
    struct Dds3Node *prev;
    struct Dds3Node *next;
    s32 key;
    Dds3NodeOps *ops;
} Dds3Node;

extern u32 dds3OwnedNodeListHead;

extern Dds3NodeOps dds3FontNodeVTable;

s32 ptyScriptRemoveUnitAndReturnResult(void) {
    s32 unitId = scrReadIntParameter(0);

    scrSetIntegerReturnValue(func_0011C680(unitId) == 1);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
s32 func_0011ECC8(void) {
    s32 firstOperand = scrReadIntParameter(0);
    s32 secondOperand = scrReadIntParameter(1);

    scrSetIntegerReturnValue(func_0011C0B0(firstOperand, secondOperand));
    return 1;
}

s32 func_0011ED10(void) {
    s32 firstOperand = scrReadIntParameter(0);
    s32 secondOperand = scrReadIntParameter(1);

    scrSetIntegerReturnValue(func_0011C340(firstOperand, secondOperand) == 1);
    return 1;
}

s32 func_0011ED60(void) {
    scrSetIntegerReturnValue(mtrHasEnoughOwnedMantras());
    return 1;
}

s32 scrCmdSetEntryFlagsInBothStores(void) {
    s32 a = scrReadIntParameter(0);
    u16 b = scrReadIntParameter(1);
    s32 index = dds3FindEntryIndex(a);
    s32 result = 0;

    if (index >= 0) {
        u8 *entry = (u8 *)(datGameState + index * 0x1C4 + 0xA60);

        scrSetFlag(entry, b);
        scrClearAllSecondaryScriptFlags(entry);
        scrSetSecondaryScriptFlag(entry, b);
        result = 1;
    }
    scrSetIntegerReturnValue(result);
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

/* Unlink an intrusive node; linkOffset selects its previous/next pair. */
void dds3UnlinkNodeFromList(s32 *list, s32 node, s32 linkOffset) {
    s32 *link = (s32 *)(node + linkOffset);
    s32 prev = link[0];
    s32 next = link[1];

    if (prev == 0) {
        *list = next;
    }
    else {
        ((s32 *)(prev + linkOffset))[1] = next;
    }
    if (next == 0) {
        list[1] = prev;
    }
    else {
        *(s32 *)(next + linkOffset) = prev;
    }
}

void dds3RegisterOwnedIntrusiveNode(Dds3Node *node, Dds3NodeOps *ops) {
    dds3AppendIntrusiveNode((s32 *)&dds3OwnedNodeListHead, (s32)node, 0);
    node->ops = ops;
}

void dds3DestroyLinkedNode(Dds3Node *node) {
    dds3UnlinkNodeFromList((s32 *)&dds3OwnedNodeListHead, (s32)node, 0);
    node->ops->destroy((s32)node);
}

void dds3DestroyAllOwnedIntrusiveNodes(void) {
    Dds3Node *current;
    while ((current = (Dds3Node *)dds3OwnedNodeListHead) != 0) {
        dds3DestroyLinkedNode(current);
    }
}

void dds3SetLinkedNodeValue(s32 object, u32 value) {
    *(u32 *)(object + 8) = value;
}

void dds3DestroyNodesWithValue(s32 key) {
    Dds3Node *node = (Dds3Node *)dds3OwnedNodeListHead;

    while (node != 0) {
        Dds3Node *next = node->next;

        if (node->key == key) {
            dds3DestroyLinkedNode(node);
        }
        node = next;
    }
}

void dds3UpdateLinkedNodes(void) {
    Dds3Node *node = (Dds3Node *)dds3OwnedNodeListHead;

    while (node != 0) {
        node->ops->update((s32)node);
        node = node->next;
    }
}

void frFontSubmitAndFreeGlyphOwner(GlyphOwner *owner) {
    frFontQueueGlyphInSelectedSlot(owner->glyph);
    sdfReleaseChipBlock(owner);
}

void func_0011F010(GlyphOwner *owner) {
    func_0019D518(owner->glyph);
}

s32 dds3CreateFontNode(s32 size, s32 a1, s32 a2) {
    s32 obj = (s32)func_0019CE78((void *)a2, 0, 0, 0, 0);
    Dds3Node *node;

    if (size == 0x800000) {
        size = (0x200 - *(s32 *)(obj + 0xC)) * 8;
    }
    frFontSetContextPair((void *)obj, size, a1);
    node = (Dds3Node *)sdfAllocAndClearQuadwords(0x14);
    *(s32 *)((u8 *)node + 0x10) = obj;
    dds3RegisterOwnedIntrusiveNode(node, &dds3FontNodeVTable);
    return (s32)node;
}

void itfConfigureOwnedGlyphChainFlag(GlyphOwner *owner, u8 flag) {
    frFontSetChainFlag(owner->glyph, flag);
}

void func_0011F0E0(void) {
    sdfReleaseChipBlock();
}

INCLUDE_SDATA(const s32, "game/code_0011EC90", dds3FontNodeVTable);

