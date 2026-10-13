#ifndef SDF_HEAP_OVERLAY_BLOCK_H
#define SDF_HEAP_OVERLAY_BLOCK_H

#include "common.h"

typedef struct SdfHeapOverlayBlock {
    u32 address;   /* 0x00 */
    u8 state;     /* 0x04 */
    u8 opaque05[3];
    s32 size;     /* 0x08 */
} SdfHeapOverlayBlock;

typedef char SdfHeapOverlayBlock_size[(sizeof(SdfHeapOverlayBlock) == 0x0C) ? 1 : -1];

#endif
