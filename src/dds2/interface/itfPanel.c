#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "itf.h"
#include "itf_panel_api.h"
#include "sdf.h"

#define ITF_PANEL_PACKET_LIST_BYTES 0x20


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
extern void (*itfPanelLayoutCallbacks[])();

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfSetPanelLayoutAndNotify(UiSprite *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue) {
    panel->renderValue = renderValue;
    panel->left = left;
    panel->top = top;
    panel->right = right;
    panel->bottom = bottom;
    if (itfPanelLayoutCallbacks[panel->kind] != 0) {
        itfPanelLayoutCallbacks[panel->kind](panel->payload);
    }
}


/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. */
void itfAdvancePanelLayoutAndNotify(UiSprite *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta) {
    void (*handler)() = itfPanelLayoutCallbacks[panel->kind];

    panel->left += leftDelta;
    panel->top += topDelta;
    panel->right += rightDelta;
    panel->bottom += bottomDelta;
    panel->renderValue += renderValueDelta;
    if (handler != 0) {
        handler(panel->payload, panel->left, panel->top, panel->right, panel->bottom);
    }
}

extern void (*itfPanelColorCallbacks[])();

/* Store the RGBA words in packet order, then notify with context.
 * The panel renderers consume these channels directly. */
void itfPanelUpdateValuesAndNotify(UiSprite *panel, s32 firstValue, s32 secondValue, s32 thirdValue, s32 fourthValue) {
    panel->red = firstValue;
    panel->green = secondValue;
    panel->blue = thirdValue;
    panel->alpha = fourthValue;
    if (itfPanelColorCallbacks[panel->kind] != 0) {
        itfPanelColorCallbacks[panel->kind](panel->payload);
    }
}


#include "sdf_packet_alloc.h"

/* Build the panel list and append it to the SDK draw-surface pool. */
void itfBuildAndSubmitPanelPacket(UiSprite *panel, SdfPoolNode *packetOwner) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(ITF_PANEL_PACKET_LIST_BYTES);

    sdfInitPacketList(packetList);
    itfAppendGsPanelStatePacket(packetList);
    itfPanelDispatchHandler(panel, packetList);
    packetOwner->append(packetOwner, packetList);
}
