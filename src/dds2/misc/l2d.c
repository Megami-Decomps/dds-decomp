#include "common.h"
#include "sdf_chip.h"
#include "sdf.h"
#include "sdf_packet_list.h"
#include "dds3_owned_node.h"

extern SdfPoolNode D_003805A8;

extern void sdfAppendClosedRectanglePacket(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top,
                          s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));

/* Emit one filled rectangle into a fresh packet list. */
void l2dDrawColoredRect(void *owner)
{
    Dds3L2dRectangle *rect = owner;
    SdfListHead *list;

    list = (SdfListHead *)sdfAllocatePacketList(0);
    sdfAppendClosedRectanglePacket(list, rect->color, 0, rect->left, rect->top,
                  rect->left + rect->width, rect->top + rect->height,
                  rect->depth, 0);
    D_003805A8.append((SdfListHead *)&D_003805A8, list);
}

extern Dds3IntrusiveNodeCallbacks D_00435EB0;

/* Allocate a rectangle record and register it with its owner list. */
Dds3L2dRectangle *l2dCreateOwnedColoredRectangle(s32 left, s32 top, s32 depth, s32 width, s32 height, s32 color) {
    Dds3L2dRectangle *rect = sdfAllocAndClearQuadwords(0x28);

    rect->left = left;
    rect->top = top;
    rect->width = width;
    rect->height = height;
    rect->depth = depth;
    rect->color = color;
    dds3RegisterOwnedIntrusiveNode(&rect->owner, &D_00435EB0);
    return rect;
}

INCLUDE_SDATA(const s32, "misc/l2d", D_00435EB0);

