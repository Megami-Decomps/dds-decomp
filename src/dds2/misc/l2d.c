#include "common.h"
#include "sdf_chip.h"
#include "sdf.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "dds3_owned_node.h"

extern sdfGrOt_t D_003805A8;

/* Emit one filled rectangle into a fresh packet list. */
void dds3FontReqDrawBoxLine(const Dds3IntrusiveNode *owner)
{
    const Dds3L2dRectangle *rect = (const Dds3L2dRectangle *)owner;
    sdfDrawTag_t *list;

    list = (sdfDrawTag_t *)sdfPrimAllocDrawTag(0);
    sdfPrimLinkBoxLine(list, rect->color, 0, rect->left, rect->top,
                  rect->left + rect->width, rect->top + rect->height,
                  rect->depth, 0);
    D_003805A8.append(&D_003805A8, list);
}

extern Dds3IntrusiveNodeCallbacks D_00435EB0;

/* Allocate a rectangle record and register it with its owner list. */
Dds3L2dRectangle *dds3FontReqAddBox(s32 left, s32 top, s32 depth, s32 width, s32 height, s32 color) {
    Dds3L2dRectangle *rect = sdfAllocAndClearQuadwords(0x28);

    rect->left = left;
    rect->top = top;
    rect->width = width;
    rect->height = height;
    rect->depth = depth;
    rect->color = color;
    dds3FontReqListAdd(&rect->owner, &D_00435EB0);
    return rect;
}

Dds3IntrusiveNodeCallbacks D_00435EB0 __attribute__((section(".sdata"))) = {
    dds3FontReqFree,
    dds3FontReqDrawBoxLine,
};
