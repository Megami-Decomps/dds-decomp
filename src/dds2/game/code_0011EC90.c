#include "common.h"
#include "scr_integer_return_value_api.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "itf.h"
#include "fr_font.h"
#include "dds3_owned_node.h"
#include "pty_roster_api.h"

extern s32 mtrHasEnoughOwnedMantras(void);

extern PtyAddUnitResult ptyAddUnitWithFlags(s32 unitId, s32 initFlags);

extern PtyRemoveUnitResult ptyRemoveUnitWithFlags(s32 param0, s32 param1);

extern s32 scrGetIntPara(s32 idx);

extern s32 dds3FindEntryIndex(s32 unitId);

extern s32 scrSetFlag(datUnitWork_t *work, u16 index);

extern void scrClearAllSecondaryScriptFlags(datUnitWork_t *work);

extern void scrSetSecondaryScriptFlag(datUnitWork_t *work, u16 index);



s32 ptyScriptRemoveUnitAndReturnResult(void) {
    s32 unitId = scrGetIntPara(0);

    scrSetIntReturnValue(ptyRemoveUnit(unitId) == PTY_REMOVE_UNIT_RESULT_REMOVED);
    return 1;
}

/* Evaluate a two-operand VM expression and publish its result. */
s32 ptyScriptAddUnitWithFlagsAndReturnResult(void) {
    s32 unitId = scrGetIntPara(0);
    s32 initFlags = scrGetIntPara(1);

    scrSetIntReturnValue(ptyAddUnitWithFlags(unitId, initFlags));
    return 1;
}

s32 ptyScriptRemoveUnitWithFlagsAndReturnResult(void) {
    s32 firstOperand = scrGetIntPara(0);
    s32 secondOperand = scrGetIntPara(1);

    scrSetIntReturnValue(
        ptyRemoveUnitWithFlags(firstOperand, secondOperand) == PTY_REMOVE_UNIT_RESULT_REMOVED);
    return 1;
}

s32 scrCmdHasEnoughOwnedMantras(void) {
    scrSetIntReturnValue(mtrHasEnoughOwnedMantras());
    return 1;
}

s32 scrCmdSetEntryFlagsInBothStores(void) {
    s32 unitId = scrGetIntPara(0);
    u16 flagId = scrGetIntPara(1);
    s32 index = dds3FindEntryIndex(unitId);
    s32 result = 0;

    if (index >= 0) {
        datUnitWork_t *entry = &DDS3_GBWK->party[index];

        scrSetFlag(entry, flagId);
        scrClearAllSecondaryScriptFlags(entry);
        scrSetSecondaryScriptFlag(entry, flagId);
        result = 1;
    }
    scrSetIntReturnValue(result);
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

void dds3FontReqDestroy(Dds3IntrusiveNode *owner) {
    Dds3FontNode *node = (Dds3FontNode *)owner;

    frDestroyFontRequestAfter(node->glyph);
    sdfChipFreeDirect(node);
}

void dds3FontReqDrawFont(Dds3IntrusiveNode *owner) {
    Dds3FontNode *node = (Dds3FontNode *)owner;

    frFontDrawGlyphInDefaultMode(node->glyph);
}

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

void dds3FontReqFree(Dds3IntrusiveNode *owner) {
    sdfChipFreeDirect(owner);
}

Dds3IntrusiveNodeCallbacks dds3FontNodeVTable __attribute__((section(".sdata"))) = {
    dds3FontReqDestroy,
    dds3FontReqDrawFont,
};
