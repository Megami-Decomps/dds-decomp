#include "common.h"
#include "sdf_vu0.h"
#include "eff_random.h"
#include "eff_param.h"
#include "sdf_heap_api.h"
#include "eff.h"
#include "eff_math.h"
#include "eff_channel.h"
#include "eff_channel_layout.h"
#include "pcp_vu0.h"
#include "fr_font.h"
#include "font_mem_control_api.h"

FntList *effGetFontListHead(void) {
    return &gFGC;
}

s32 effGetFontListCount(void) {
    return gFGC.count;
}

extern s8 D_003BB150;

/* Build the glyph atlas once: COLUMNS x ROWS cells in a circular node list, each with its
 * UV rectangle in 1/16-texel units and an optional RECORDBYTES-sized per-cell record. */
s32 fmInitFontMemControl(u32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes) {
    u8 *records = NULL;
    FntNode *node;
    s32 count;
    s32 i;
    s32 row;
    s32 column;

    if (gFGC.head != NULL && D_003BB150 != 0) {
        return 0;
    }
    count = columns * rows;
    gFGC.cellWidth = cellWidth;
    gFGC.cellHeight = cellHeight;
    gFGC.count = count;
    gFGC.capacity = count;
    gFGC.pixelByteCount = cellWidth * cellHeight / 2;
    gFGC.nodeAllocation = sdfMemAllocDirect(count * sizeof(FntNode));
    gFGC.head = (FntNode *)sdfMemLock(gFGC.nodeAllocation);
    if (recordBytes != 0) {
        gFGC.recordStorageAllocation = sdfMemAllocDirect(recordBytes * gFGC.count);
        records = (u8 *)sdfMemLock(gFGC.recordStorageAllocation);
    }
    for (i = 0; i < gFGC.count - 1; i++) {
        gFGC.head[i].next = &gFGC.head[i + 1];
    }
    gFGC.head[i].next = gFGC.head;
    for (i = gFGC.count - 1; i > 0; i--) {
        gFGC.head[i].prev = &gFGC.head[i - 1];
    }
    gFGC.head[0].prev = &gFGC.head[gFGC.count - 1];
    i = 0;
    for (row = 0; row < rows; row++) {
        for (column = 0; column < columns; column++) {
            node = &gFGC.head[i];
            if (node == NULL) {
                fmDestroyFontMemControl();
                return 0;
            }
            node->nodeIndex = i;
            node->uv.u0 = (column * cellWidth) << 4;
            node->uv.u1 = ((column * cellWidth) << 4) + (cellWidth << 4);
            if (records != NULL) {
                node->item = (GsFont_t *)(records + i * recordBytes);
            } else {
                node->item = NULL;
            }
            node->uv.v0 = (row * cellHeight) << 4;
            node->uv.v1 = ((row * cellHeight) << 4) + (cellHeight << 4);
            i++;
        }
    }
    D_003BB150 = 1;
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00193900", D_003BB150);

INCLUDE_SDATA(const s32, "game/code_00193900", D_003BB15C);

