#include "common.h"
#include "itf.h"

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


/* The layout table accepts context alone or context plus four corners. */
extern void (*D_00357A00[])();
extern void (*D_00357A28[])();

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfSetPanelLayoutAndNotify(UiSprite *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue) {
    panel->unk0C = renderValue;
    panel->left = left;
    panel->top = top;
    panel->right = right;
    panel->bottom = bottom;
    if (D_00357A00[panel->kind] != 0) {
        D_00357A00[panel->kind](panel->payload);
    }
}

/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. */
void itfAdvancePanelLayoutAndNotify(UiSprite *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta) {
    void (*handler)() = D_00357A00[panel->kind];

    panel->left += leftDelta;
    panel->top += topDelta;
    panel->right += rightDelta;
    panel->bottom += bottomDelta;
    panel->unk0C += renderValueDelta;
    if (handler != 0) {
        handler(panel->payload, panel->left, panel->top, panel->right, panel->bottom);
    }
}

/* Store the four opaque secondary values in order, then notify with context.
 * Keep write-only fields unnamed rather than assuming these are RGBA channels. */
void itfPanelUpdateValuesAndNotify(UiSprite *panel, s32 firstValue, s32 secondValue, s32 thirdValue, s32 fourthValue) {
    panel->unk2C = firstValue;
    panel->unk30 = secondValue;
    panel->unk34 = thirdValue;
    panel->unk38 = fourthValue;
    if (D_00357A28[panel->kind] != 0) {
        D_00357A28[panel->kind](panel->payload);
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
s32 itfBuildAndSubmitPanelPacket(UiSprite *panel, s32 packetOwner) {
    s32 packetList = sdfAllocPacketAligned(ITF_PANEL_PACKET_LIST_BYTES, packetOwner);

    sdfInitPacketList(packetList);
    itfAppendGsPanelStatePacket(packetList);
    itfPanelDispatchHandler(panel, packetList);
    return ((PanelPacketDispatch *)packetOwner)->handler(packetOwner, packetList);
}
