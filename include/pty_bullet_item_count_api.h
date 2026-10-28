#ifndef PTY_BULLET_ITEM_COUNT_API_H
#define PTY_BULLET_ITEM_COUNT_API_H

#include "common.h"

/* The counter adds inventory stock and matching party menu values, without
 * checking party-slot occupancy. Its accepted item-key range is [0xA0, 0xBF),
 * narrower than DDS2's bullet-item predicate, which also recognizes 0xBF. */
typedef s32 PtyBulletItemId;
typedef s32 PtyBulletItemCount;

enum {
    PTY_BULLET_ITEM_ID_FIRST = 0xA0,
    PTY_BULLET_ITEM_ID_LIMIT = 0xBF,
};

PtyBulletItemCount ptyCountBulletItem(PtyBulletItemId bulletItemId);

#endif /* PTY_BULLET_ITEM_COUNT_API_H */
