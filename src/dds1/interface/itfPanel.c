#include "common.h"
#include "eff.h"

#define ITF_PANEL_PACKET_LIST_BYTES 0x20

extern void sdfReleaseResourceAllocation(void *resource);

/* Release the secondary resource only for nonzero recordCount, then the
 * primary resource. NULL is ignored; no direct primitive release or slot-word clearing occurs. */
void itfPanelReleasePrimitiveResources(EffPrim *primitive) {
    if (primitive != NULL) {
        if (primitive->recordCount != 0) {
            sdfReleaseResourceAllocation(primitive->secondaryResource);
        }
        sdfReleaseResourceAllocation(primitive->primaryResource);
    }
}

/* Panel definition: a handler table is indexed by `kind` (0x3C) and the entry
   is called with the panel's own context (0x08). */
typedef struct PanelDefinition {
    u8 pad00[8];
    void *context; /* 0x08: passed as $4 to the table entry */
    s32 unk0C;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    u8 pad20[0x1C];
    u8 kind; /* 0x3C: index into the handler table */
} PanelDefinition;

typedef struct PanelDefinition2 {
    u8 pad00[8];
    void *context; /* 0x08 */
    u8 pad0C[0x20];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u8 kind; /* 0x3C */
} PanelDefinition2;

extern void (*D_00357A00[])(void *);
extern void (*D_00357A28[])(void *);

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfSetPanelLayoutAndNotify(PanelDefinition *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue) {
    panel->unk0C = renderValue;
    panel->left = left;
    panel->top = top;
    panel->right = right;
    panel->bottom = bottom;
    if (D_00357A00[panel->kind] != 0) {
        D_00357A00[panel->kind](panel->context);
    }
}

/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. Preserve the existing wider callback cast. */
void itfAdvancePanelLayoutAndNotify(PanelDefinition *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta) {
    void (*handler)(void *, s32, s32, s32, s32) =
        (void (*)(void *, s32, s32, s32, s32))D_00357A00[panel->kind];

    panel->left += leftDelta;
    panel->top += topDelta;
    panel->right += rightDelta;
    panel->bottom += bottomDelta;
    panel->unk0C += renderValueDelta;
    if (handler != 0) {
        handler(panel->context, panel->left, panel->top, panel->right, panel->bottom);
    }
}

/* Store the four opaque secondary values in order, then notify with context.
 * Keep write-only fields unnamed rather than assuming these are RGBA channels. */
void itfPanelUpdateValuesAndNotify(PanelDefinition2 *panel, s32 firstValue, s32 secondValue, s32 thirdValue, s32 fourthValue) {
    panel->unk2C = firstValue;
    panel->unk30 = secondValue;
    panel->unk34 = thirdValue;
    panel->unk38 = fourthValue;
    if (D_00357A28[panel->kind] != 0) {
        D_00357A28[panel->kind](panel->context);
    }
}

/* The packet owner's callback occupies its 0x10 dispatch slot. */
typedef struct PanelPacketDispatch {
    u8 pad00[0x10];
    s32 (*handler)(s32 owner, s32 packet);
} PanelPacketDispatch;

/* Allocate/init a list, append GS state and panel packets, then return the
 * owner's submission result. Owner callback and allocation must be valid.
 * Keep DDS1's extra allocator argument and existing integer packet handles. */
s32 itfBuildAndSubmitPanelPacket(PanelDefinition *panel, s32 packetOwner) {
    s32 packetList = sdfAllocPacketAligned(ITF_PANEL_PACKET_LIST_BYTES, packetOwner);

    sdfInitPacketList(packetList);
    itfAppendGsPanelStatePacket(packetList);
    itfPanelDispatchHandler(panel, packetList);
    return ((PanelPacketDispatch *)packetOwner)->handler(packetOwner, packetList);
}
