#include "common.h"
#include "itf.h"
#include "sdf.h"

#define ITF_PANEL_PACKET_LIST_BYTES 0x20

extern void sdfReleaseResourceAllocation(void *resource);

/* A sprite with a retained payload owns a secondary allocation. Release that
 * before its primary allocation; NULL is a no-op. */
void itfPanelReleasePrimitiveResources(UiSprite *sprite) {
    if (sprite != NULL) {
        if (sprite->payload != NULL) {
            sdfReleaseResourceAllocation(sprite->payloadAllocation);
        }
        sdfReleaseResourceAllocation(sprite->allocation);
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


extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void itfAppendGsPanelStatePacket(SdfListHead *);
extern void itfPanelDispatchHandler(UiSprite *, SdfListHead *);

/* Build the panel list and append it to the SDK draw-surface pool. */
void itfBuildAndSubmitPanelPacket(UiSprite *panel, SdfPoolNode *packetOwner) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(ITF_PANEL_PACKET_LIST_BYTES);

    sdfInitPacketList(packetList);
    itfAppendGsPanelStatePacket(packetList);
    itfPanelDispatchHandler(panel, packetList);
    packetOwner->append((SdfListHead *)packetOwner, packetList);
}
