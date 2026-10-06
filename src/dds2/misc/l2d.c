#include "common.h"
#include "sdf.h"

/* Rectangle in object space: origin, extent, depth and 15-bit colour. */
typedef struct L2dRect {
    u8 pad00[0x10];
    s32 left;   /* 0x10 */
    s32 top;    /* 0x14 */
    s32 width;  /* 0x18 */
    s32 height; /* 0x1C */
    s32 depth;  /* 0x20 */
    s32 color;  /* 0x24 */
} L2dRect;


extern SdfPoolNode D_003805A8;

extern s32 sdfAllocatePacketList(s32 (*alloc)(s32));

extern void sdfAppendClosedRectanglePacket(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top,
                          s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));

/* Emit one filled rectangle into a fresh packet list. */
void l2dDrawColoredRect(L2dRect *rect)
{
    SdfListHead *list;

    list = (SdfListHead *)sdfAllocatePacketList(0);
    sdfAppendClosedRectanglePacket(list, rect->color, 0, rect->left, rect->top,
                  rect->left + rect->width, rect->top + rect->height,
                  rect->depth, 0);
    D_003805A8.append((SdfListHead *)&D_003805A8, list);
}

extern void *sdfAllocAndClearQuadwords(s32 size);
extern void dds3RegisterOwnedIntrusiveNode(void *node, void *owner);
extern s32 D_00435EB0;

/* Allocate a rectangle record and register it with its owner list. */
L2dRect *l2dCreateOwnedColoredRectangle(s32 left, s32 top, s32 depth, s32 width, s32 height, s32 color) {
    L2dRect *rect = sdfAllocAndClearQuadwords(0x28);

    rect->left = left;
    rect->top = top;
    rect->width = width;
    rect->height = height;
    rect->depth = depth;
    rect->color = color;
    dds3RegisterOwnedIntrusiveNode(rect, &D_00435EB0);
    return rect;
}

INCLUDE_SDATA(const s32, "misc/l2d", D_00435EB0);

