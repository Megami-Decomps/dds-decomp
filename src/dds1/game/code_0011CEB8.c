#include "common.h"
#include "scr_integer_return_value_api.h"
#include "sdf_chip.h"
#include "itf.h"
#include "fr_font.h"
#include "dds3_owned_node.h"
#include "pty_roster_api.h"

extern s32 scrGetIntPara(s32);
/* The native VM commands pass operands to these parameterless party stubs. */
extern u32 ptyReturnZero();
extern u32 ptyReturnOneForCommand193();
extern u32 ptyReturnOne();

/* Remove the party unit specified by script operand 0 and return success to
 * the script VM, while writing whether a unit was actually removed. */
u32 ptyScriptRemoveUnitAndReturnResult(void) {
    scrSetIntReturnValue(ptyRemoveUnit(scrGetIntPara(0)) == 1);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
u32 ptyScriptSetZeroResultCommand192(void) {
    s32 firstOperand;
    s32 secondOperand;

    firstOperand = scrGetIntPara(0);
    secondOperand = scrGetIntPara(1);
    firstOperand = ptyReturnZero(firstOperand, secondOperand);
    scrSetIntReturnValue(firstOperand);
    return 1;
}

u32 ptyScriptSetOneResultCommand193(void) {
    s32 firstOperand;
    s32 secondOperand;

    firstOperand = scrGetIntPara(0);
    secondOperand = scrGetIntPara(1);
    scrSetIntReturnValue(ptyReturnOneForCommand193(firstOperand, secondOperand) == 1);
    return 1;
}

u32 ptyScriptSetOneResultCommand194(void) {
    scrSetIntReturnValue(ptyReturnOne(scrGetIntPara(0)) == 1);
    return 1;
}

/* Append an intrusive node; linkOffset selects its previous/next pair. */
void dds3FontReqListInsert(Dds3IntrusiveNodeList *list,
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

void dds3FontReqListRemove(Dds3IntrusiveNodeList *list,
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

void dds3FontReqListAdd(Dds3IntrusiveNode *node,
                                    Dds3IntrusiveNodeCallbacks *vtable) {
    dds3FontReqListInsert(&dds3OwnedNodeListHead, node, 0);
    node->vtable = vtable;
}

void dds3FontReqListRunOne(Dds3IntrusiveNode *node) {
    dds3FontReqListRemove(&dds3OwnedNodeListHead, node, 0);
    node->vtable->destroy(node);
}

void dds3FontReqListRunAll(void) {
    Dds3IntrusiveNode *current;
    while ((current = dds3OwnedNodeListHead.head) != NULL) {
        dds3FontReqListRunOne(current);
    }
}

void dds3SetLinkedNodeValue(Dds3IntrusiveNode *node, u32 nodeValue) {
    node->value = nodeValue;
}

void dds3FontReqListRunTo(s32 nodeValue) {
    Dds3IntrusiveNode *node = dds3OwnedNodeListHead.head;

    while (node != NULL) {
        Dds3IntrusiveNode *next = node->next;
        if (node->value == nodeValue) {
            dds3FontReqListRunOne(node);
        }
        node = next;
    }
}

void dds3FontReqListCount(void) {
    Dds3IntrusiveNode *node = dds3OwnedNodeListHead.head;

    while (node != NULL) {
        node->vtable->update(node);
        node = node->next;
    }
}

void dds3FontReqDestroy(void *owner) {
    Dds3FontNode *node = owner;

    frDestroyFontRequestAfter(node->glyph);
    sdfChipFreeDirect(node);
}

void dds3FontReqDrawFont(void *owner) {
    Dds3FontNode *node = owner;

    frFontDrawGlyphInDefaultMode(node->glyph);
}

extern _FRQ * frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);

Dds3FontNode *dds3CreateFontNode(s32 x, s32 y, const char *text) {
    struct _FRQ *glyph;
    Dds3FontNode *node;

    glyph = frFontBuildGlyphChain(text, 0, 0, 0, NULL);
    if (x == 0x800000) {
        x = (0x200 - glyph->advance) * 8;
    }
    frSetDrawPosition(glyph, x, y);
    node = sdfAllocAndClearQuadwords(sizeof(*node));
    node->glyph = glyph;
    dds3FontReqListAdd(&node->owner, &dds3FontNodeVTable);
    return node;
}

void dds3FontReqSetColorNo(Dds3FontNode *node, u8 value) {
    frSetFontColorNo(node->glyph, value);
}

void dds3FontReqFree(void *memory) {
    sdfChipFreeDirect(memory);
}

Dds3IntrusiveNodeCallbacks dds3FontNodeVTable __attribute__((section(".sdata"))) = {
    dds3FontReqDestroy,
    dds3FontReqDrawFont,
};
