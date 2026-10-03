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
    u8 pad20[0xC];
    /* Secondary four-value parameter set, stored before the kind-specific
       D_003B4420 notification; the values' individual roles are unknown. */
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u8 kind; /* 0x3C: index into the handler table */
} PanelDefinition;

extern void (*D_003B43F8[])(void *);

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfSetPanelLayoutAndNotify(PanelDefinition *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue) {
    panel->unk0C = renderValue;
    panel->left = left;
    panel->top = top;
    panel->right = right;
    panel->bottom = bottom;
    if (D_003B43F8[panel->kind] != 0) {
        D_003B43F8[panel->kind](panel->context);
    }
}


/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. Preserve the existing wider callback cast. */
void itfAdvancePanelLayoutAndNotify(PanelDefinition *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta) {
    void (*handler)(void *, s32, s32, s32, s32) =
        (void (*)(void *, s32, s32, s32, s32))D_003B43F8[panel->kind];

    panel->left += leftDelta;
    panel->top += topDelta;
    panel->right += rightDelta;
    panel->bottom += bottomDelta;
    panel->unk0C += renderValueDelta;
    if (handler != 0) {
        handler(panel->context, panel->left, panel->top, panel->right, panel->bottom);
    }
}

extern void (*D_003B4420[])(void *);

/* Store the four opaque secondary values in order, then notify with context.
 * Keep write-only fields unnamed rather than assuming these are RGBA channels. */
void itfPanelUpdateValuesAndNotify(PanelDefinition *panel, s32 firstValue, s32 secondValue, s32 thirdValue, s32 fourthValue) {
    panel->unk2C = firstValue;
    panel->unk30 = secondValue;
    panel->unk34 = thirdValue;
    panel->unk38 = fourthValue;
    if (D_003B4420[panel->kind] != 0) {
        D_003B4420[panel->kind](panel->context);
    }
}


extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(s32);
extern void itfAppendGsPanelStatePacket(s32);

/* Allocate/init a list, append GS state and panel packets, then return the
 * owner's submission result. Owner callback and allocation must be valid.
 * Keep DDS2's single allocator argument and existing raw owner callback access. */
s32 itfBuildAndSubmitPanelPacket(PanelDefinition *panel, s32 packetOwner) {
    s32 packetList = sdfAllocPacketAligned(ITF_PANEL_PACKET_LIST_BYTES);

    sdfInitPacketList(packetList);
    itfAppendGsPanelStatePacket(packetList);
    itfPanelDispatchHandler(panel, packetList);
    return ((s32 (*)(s32, s32))*(u32 *)(packetOwner + 0x10))(packetOwner, packetList);
}
