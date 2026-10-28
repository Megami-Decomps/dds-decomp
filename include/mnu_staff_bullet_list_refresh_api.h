#ifndef MNU_STAFF_BULLET_LIST_REFRESH_API_H
#define MNU_STAFF_BULLET_LIST_REFRESH_API_H

#include "common.h"

/* Controls whether the staff menu keeps the item-selection state or returns
 * to the item-category menu after refreshing the equipped-item list. */
typedef s32 MnuStaffBulletListRefreshResult;
enum {
    MNU_STAFF_BULLET_LIST_REFRESH_RETURN_TO_CATEGORY = 0,
    MNU_STAFF_BULLET_LIST_REFRESH_KEEP_SELECTION = 1
};

#endif /* MNU_STAFF_BULLET_LIST_REFRESH_API_H */
