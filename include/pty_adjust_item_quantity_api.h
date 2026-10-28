#ifndef PTY_ADJUST_ITEM_QUANTITY_API_H
#define PTY_ADJUST_ITEM_QUANTITY_API_H

#include "pty_item_id.h"

/* Signed change applied to an item's stored inventory count. */
typedef s32 PtyItemQuantityDelta;

/* Flag-backed IDs set their model flag; other valid IDs update their quantity. */
void ptyAdjustItemQuantity(PtyItemId itemId, PtyItemQuantityDelta quantityDelta);

#endif /* PTY_ADJUST_ITEM_QUANTITY_API_H */
