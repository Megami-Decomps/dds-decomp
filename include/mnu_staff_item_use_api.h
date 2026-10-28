#ifndef MNU_STAFF_ITEM_USE_API_H
#define MNU_STAFF_ITEM_USE_API_H

#include "common.h"

/* Reports whether staff-item use actually consumed one inventory entry. */
typedef s32 MnuStaffItemUseResult;
enum {
    MNU_STAFF_ITEM_USE_RESULT_NOT_CONSUMED = 0,
    MNU_STAFF_ITEM_USE_RESULT_CONSUMED = 1
};

#ifdef VERSION_DDS1
struct StaffDisplayContext;
MnuStaffItemUseResult mnuUseStaffItem(
    s32 itemId, struct StaffDisplayContext *context);
#endif

#ifdef VERSION_DDS2
struct MenuStaffContext;
MnuStaffItemUseResult mnuUseStaffItem(
    s32 itemId, struct MenuStaffContext *context);
#endif

#endif /* MNU_STAFF_ITEM_USE_API_H */
