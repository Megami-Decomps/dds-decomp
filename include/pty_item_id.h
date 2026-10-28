#ifndef PTY_ITEM_ID_H
#define PTY_ITEM_ID_H

#include "common.h"

/* Save-backed item key. IDs in [0x80, 0xA0) use model flags. */
typedef s32 PtyItemId;

enum {
    PTY_ITEM_FLAG_FIRST = 0x80,
    PTY_ITEM_FLAG_COUNT = 0x20,
    PTY_ITEM_MODEL_FLAG_BASE = 0x980
};

#endif /* PTY_ITEM_ID_H */
