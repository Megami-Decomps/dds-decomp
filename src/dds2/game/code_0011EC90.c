#include "common.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "itf.h"
#include "dds3_owned_node.h"

extern u32 mtrHasEnoughOwnedMantras(void);

extern s32 func_0011C0B0(s32 param0, s32 param1);

extern s32 func_0011C340(s32 param0, s32 param1);

extern s32 scrReadIntParameter(s32 idx);

extern s32 dds3FindEntryIndex(s32 rosterIndex);

extern s32 scrSetFlag(DatPartyRecord *work, u16 index);

extern void scrClearAllSecondaryScriptFlags(DatPartyRecord *work);

extern void scrSetSecondaryScriptFlag(DatPartyRecord *work, u16 index);


extern s32 scrSetIntegerReturnValue(s32 arg0);

extern s32 frFontDrawGlyphInDefaultMode(struct FrFontGlyph *glyph);

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
        DatPartyRecord *entry = &datGameState->party[index];

        scrSetFlag(entry, b);
        scrClearAllSecondaryScriptFlags(entry);
        scrSetSecondaryScriptFlag(entry, b);
        result = 1;
    }
    scrSetIntegerReturnValue(result);
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

    frFontQueueGlyphForCurrentDrawBuffer(node->glyph);
    sdfReleaseChipBlock(node);
}

void frFontDrawOwnedGlyph(void *owner) {
    Dds3FontNode *node = owner;

    frFontDrawGlyphInDefaultMode(node->glyph);
}

Dds3FontNode *dds3CreateFontNode(s32 x, s32 y, void *text) {
    struct FrFontGlyph *glyph;
    Dds3FontNode *node;

    glyph = func_0019CE78(text, 0, 0, 0, NULL);
    if (x == 0x800000) {
        x = (0x200 - glyph->advance) * 8;
    }
    frFontSetGlyphPosition(glyph, x, y);
    node = sdfAllocAndClearQuadwords(sizeof(*node));
    node->glyph = glyph;
    dds3RegisterOwnedIntrusiveNode(&node->owner, &dds3FontNodeVTable);
    return node;
}

void itfConfigureOwnedGlyphChainFlag(Dds3FontNode *node, u8 value) {
    frFontSetChildChainFirstOption(node->glyph, value);
}

void frFontReleaseOwnerStorage(void *memory) {
    sdfReleaseChipBlock(memory);
}

Dds3IntrusiveNodeCallbacks dds3FontNodeVTable __attribute__((section(".sdata"))) = {
    frFontSubmitAndFreeGlyphOwner,
    frFontDrawOwnedGlyph,
};
