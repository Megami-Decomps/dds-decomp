#include "common.h"
#include "sdf_heap_api.h"
#include "fr_font.h"
#include "font_mem_control_api.h"
#include "sdk_ps2.h"

extern s32 fmInitFontMemControl(u32 bufferBase, s32 columns, s32 rows, s32 cellWidth,
                                       s32 cellHeight, s32 recordBytes);
extern void fmRetFontMem(FontMem_t *node);
extern FontMem_t *fmRentFontMem(void);

FontGsControl_t *fmGetInternalFGC(void) {
    return &gFGC;
}

s32 fmGetRestBufNum(void) {
    return gFGC.unit_nums;
}

extern s8 D_003BB150;

/* Build the glyph atlas once: COLUMNS x ROWS cells in a circular node list, each with its
 * UV rectangle in 1/16-texel units and an optional RECORDBYTES-sized per-cell record. */
s32 fmInitFontMemControl(u32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes) {
    u8 *records = NULL;
    FontMem_t *node;
    s32 count;
    s32 i;
    s32 row;
    s32 column;

    if (gFGC.root != NULL && D_003BB150 != 0) {
        return 0;
    }
    count = columns * rows;
    gFGC.unit_width = cellWidth;
    gFGC.unit_height = cellHeight;
    gFGC.unit_nums = count;
    gFGC.unit_num_max = count;
    gFGC.unit_size = cellWidth * cellHeight / 2;
    gFGC.mh = sdfMemAllocDirect(count * sizeof(FontMem_t));
    gFGC.root = (FontMem_t *)sdfMemLock(gFGC.mh);
    if (recordBytes != 0) {
        gFGC.ud_mh = sdfMemAllocDirect(recordBytes * gFGC.unit_nums);
        records = (u8 *)sdfMemLock(gFGC.ud_mh);
    }
    for (i = 0; i < gFGC.unit_nums - 1; i++) {
        gFGC.root[i].next = &gFGC.root[i + 1];
    }
    gFGC.root[i].next = gFGC.root;
    for (i = gFGC.unit_nums - 1; i > 0; i--) {
        gFGC.root[i].prev = &gFGC.root[i - 1];
    }
    gFGC.root[0].prev = &gFGC.root[gFGC.unit_nums - 1];
    i = 0;
    for (row = 0; row < rows; row++) {
        for (column = 0; column < columns; column++) {
            node = &gFGC.root[i];
            if (node == NULL) {
                fmDestroyFontMemControl();
                return 0;
            }
            node->id = i;
            node->uv.u0 = (column * cellWidth) << 4;
            node->uv.u1 = ((column * cellWidth) << 4) + (cellWidth << 4);
            if (records != NULL) {
                node->user_data = (GsFont_t *)(records + i * recordBytes);
            } else {
                node->user_data = NULL;
            }
            node->uv.v0 = (row * cellHeight) << 4;
            node->uv.v1 = ((row * cellHeight) << 4) + (cellHeight << 4);
            i++;
        }
    }
    D_003BB150 = 1;
    return 1;
}
/* Release the group's handles once and clear its active-node flag. */
FntMemControlDestroyResult fmDestroyFontMemControl(void) {
    if (gFGC.root == 0) {
        return FNT_MEM_CONTROL_DESTROY_ALREADY_INACTIVE;
    }
    sdfMemFreeDirect(gFGC.mh);
    sdfMemFreeDirect(gFGC.ud_mh);
    gFGC.root = 0;
    return FNT_MEM_CONTROL_DESTROY_RELEASED;
}

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FontMem_t *fmRentFontMem(void) {
    FontMem_t *resourceNode = gFGC.root->next;

    if (resourceNode->id == 0) {
        return NULL;
    }
    resourceNode->prev->next = resourceNode->next;
    gFGC.unit_nums -= 1;
    resourceNode->next->prev = resourceNode->prev;
    return resourceNode;
}

/* Insert after the list's fixed anchor and update the entry count. */
void fmRetFontMem(FontMem_t *node) {
    FontMem_t *anchor = gFGC.root;
    FontMem_t *nextNode = anchor->next;

    node->prev = anchor;
    node->next = nextNode;
    gFGC.unit_nums += 1;
    anchor->next = node;
    nextNode->prev = node;
}

INCLUDE_SDATA(const s32, "interface/fmGslCont", D_003BB150);

INCLUDE_SDATA(const s32, "interface/fmGslCont", D_003BB15C);

