#ifndef PTY_ITEM_QUANTITY_UPDATE_VIEW_H
#define PTY_ITEM_QUANTITY_UPDATE_VIEW_H

#include "common.h"

/* Reader view: inventory update code consumes two four-byte-stride records.
 * Byte 3 is opaque, and these readers do not establish the table's full extent. */
typedef struct PtyItemQuantityUpdateView {
    u16 itemId;       /* 0x0 */
    u8 quantityDelta; /* 0x2 */
    u8 opaque03;      /* 0x3 */
} PtyItemQuantityUpdateView;

typedef char PtyItemQuantityUpdateView_itemId_offset_must_be_0x0[
    ((u32)&((PtyItemQuantityUpdateView *)0)->itemId == 0x0) ? 1 : -1];
typedef char PtyItemQuantityUpdateView_quantityDelta_offset_must_be_0x2[
    ((u32)&((PtyItemQuantityUpdateView *)0)->quantityDelta == 0x2) ? 1 : -1];
typedef char PtyItemQuantityUpdateView_opaque03_offset_must_be_0x3[
    ((u32)&((PtyItemQuantityUpdateView *)0)->opaque03 == 0x3) ? 1 : -1];
typedef char PtyItemQuantityUpdateView_size_must_be_4[
    (sizeof(PtyItemQuantityUpdateView) == 4) ? 1 : -1];

#endif /* PTY_ITEM_QUANTITY_UPDATE_VIEW_H */
