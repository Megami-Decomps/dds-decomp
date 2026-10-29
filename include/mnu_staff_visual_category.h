#ifndef MNU_STAFF_VISUAL_CATEGORY_H
#define MNU_STAFF_VISUAL_CATEGORY_H

#include "common.h"

/* Camp staff panels select one resource category; zero means no category is
 * active. These names cover paired callers; unknown selectors still resolve
 * to no resource entries through each provider's default branch. */
typedef s32 MnuStaffVisualCategory;
enum {
    MNU_STAFF_VISUAL_CATEGORY_NONE = 0,
    MNU_STAFF_VISUAL_CATEGORY_SKILL_SELECTION = 1,
    MNU_STAFF_VISUAL_CATEGORY_STAFF_DISPLAY = 2,
    MNU_STAFF_VISUAL_CATEGORY_STAFF_SELECTION = 3,
    MNU_STAFF_VISUAL_CATEGORY_PARTY_SELECTION = 4,
    MNU_STAFF_VISUAL_CATEGORY_CAMP_CONFIGURATION = 5
};

#endif /* MNU_STAFF_VISUAL_CATEGORY_H */
