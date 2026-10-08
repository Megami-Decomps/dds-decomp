#include "common.h"
#include "sdf_chip.h"
#include "itf.h"
#include "dds3_owned_node.h"

extern s32 scrReadIntParameter(s32);
/* The native VM commands pass operands to these parameterless party stubs. */
extern u32 func_0011B140();
extern u32 func_0011B148();
extern u32 func_0011B150();

extern struct FrFontGlyph *func_001951C8(void *text, s8 fontIndex,
    s8 firstOption, s8 secondOption, struct FrFontGlyph *previousGlyph);
extern void frFontSetContextPair(struct FrFontGlyph *glyph, u32 x, u32 y);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *glyph);
extern s32 frFontDrawGlyphInDefaultMode(struct FrFontGlyph *glyph);
extern void frFontSetChainFlag(struct FrFontGlyph *glyph, u8 value);

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
void dds3AppendIntrusiveNode(Dds3IntrusiveNodeList *list,
                             Dds3IntrusiveNode *node, s32 linkOffset) {
    Dds3IntrusiveNode *last = list->tail;
    u8 *nodeLinks = (u8 *)node + linkOffset;

    if (last == NULL) {
        list->head = node;
    } else {
        *(Dds3IntrusiveNode **)((u8 *)last + linkOffset + 4) = node;
    }
    *(Dds3IntrusiveNode **)nodeLinks = last;
    *(Dds3IntrusiveNode **)(nodeLinks + 4) = NULL;
    list->tail = node;
}

void dds3UnlinkNodeFromList(Dds3IntrusiveNodeList *list,
                            Dds3IntrusiveNode *node, s32 linkOffset) {
    u8 *nodeLinks = (u8 *)node + linkOffset;
    Dds3IntrusiveNode *prev = *(Dds3IntrusiveNode **)nodeLinks;
    Dds3IntrusiveNode *next = *(Dds3IntrusiveNode **)(nodeLinks + 4);

    if (prev == NULL) {
        list->head = next;
    } else {
        *(Dds3IntrusiveNode **)((u8 *)prev + linkOffset + 4) = next;
    }
    if (next == NULL) {
        list->tail = prev;
    } else {
        *(Dds3IntrusiveNode **)((u8 *)next + linkOffset) = prev;
    }
}

void dds3RegisterOwnedIntrusiveNode(Dds3IntrusiveNode *node,
                                    Dds3IntrusiveNodeCallbacks *vtable) {
    dds3AppendIntrusiveNode(&dds3OwnedNodeListHead, node, 0);
    node->vtable = vtable;
}

void dds3DestroyLinkedNode(Dds3IntrusiveNode *node) {
    dds3UnlinkNodeFromList(&dds3OwnedNodeListHead, node, 0);
    node->vtable->destroy(node);
}

void dds3DestroyAllOwnedIntrusiveNodes(void) {
    Dds3IntrusiveNode *current;
    while ((current = dds3OwnedNodeListHead.head) != NULL) {
        dds3DestroyLinkedNode(current);
    }
}

void dds3SetLinkedNodeValue(Dds3IntrusiveNode *node, u32 value) {
    node->value = value;
}

void dds3DestroyNodesWithValue(s32 value) {
    Dds3IntrusiveNode *node = dds3OwnedNodeListHead.head;

    while (node != NULL) {
        Dds3IntrusiveNode *next = node->next;
        if (node->value == value) {
            dds3DestroyLinkedNode(node);
        }
        node = next;
    }
}

void dds3UpdateLinkedNodes(void) {
    Dds3IntrusiveNode *node = dds3OwnedNodeListHead.head;

    while (node != NULL) {
        node->vtable->update(node);
        node = node->next;
    }
}

void frFontSubmitAndFreeGlyphOwner(void *owner) {
    Dds3FontNode *node = owner;

    frFontQueueGlyphInSelectedSlot(node->glyph);
    sdfReleaseChipBlock(node);
}

void frFontDrawOwnedGlyph(void *owner) {
    Dds3FontNode *node = owner;

    frFontDrawGlyphInDefaultMode(node->glyph);
}

Dds3FontNode *dds3CreateFontNode(s32 x, s32 y, void *text) {
    struct FrFontGlyph *glyph;
    Dds3FontNode *node;

    glyph = func_001951C8(text, 0, 0, 0, NULL);
    if (x == 0x800000) {
        x = (0x200 - glyph->advance) * 8;
    }
    frFontSetContextPair(glyph, x, y);
    node = sdfAllocAndClearQuadwords(sizeof(*node));
    node->glyph = glyph;
    dds3RegisterOwnedIntrusiveNode(&node->owner, &dds3FontNodeVTable);
    return node;
}

void itfConfigureOwnedGlyphChainFlag(Dds3FontNode *node, u8 value) {
    frFontSetChainFlag(node->glyph, value);
}

void frFontReleaseOwnerStorage(void *memory) {
    sdfReleaseChipBlock(memory);
}

Dds3IntrusiveNodeCallbacks dds3FontNodeVTable __attribute__((section(".sdata"))) = {
    frFontSubmitAndFreeGlyphOwner,
    frFontDrawOwnedGlyph,
};
