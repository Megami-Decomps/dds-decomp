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

typedef struct L2dDevice {
    u8 unk0[0x10];
    void (*fill)(void *, SdfListHead *);
} L2dDevice;

extern L2dDevice D_003255A8;

extern SdfListHead *sdfAllocatePacketList(void *(*alloc)(s32));

extern void sdfAppendClosedRectanglePacket(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top,
                          s32 right, s32 bottom, s32 depth, void *(*alloc)(s32));

/* Emit one filled rectangle into a fresh packet list. */
void l2dDrawColoredRect(L2dRect *rect)
{
    SdfListHead *list;

    list = sdfAllocatePacketList(0);
    sdfAppendClosedRectanglePacket(list, rect->color, 0, rect->left, rect->top,
                  rect->left + rect->width, rect->top + rect->height,
                  rect->depth, 0);
    D_003255A8.fill(&D_003255A8, list);
}

INCLUDE_ASM(const s32, "misc/l2d", func_0011D308);

INCLUDE_SDATA(const s32, "misc/l2d", D_003BAAD8);

