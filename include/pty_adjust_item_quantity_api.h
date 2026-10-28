#ifndef PTY_ADJUST_ITEM_QUANTITY_API_H
#define PTY_ADJUST_ITEM_QUANTITY_API_H

#include "pty_item_id.h"

/* Flag-backed IDs set their model flag; other valid IDs update their quantity. */
void ptyAdjustItemQuantity(PtyItemId itemId, s32 quantityDelta);

#endif /* PTY_ADJUST_ITEM_QUANTITY_API_H */
