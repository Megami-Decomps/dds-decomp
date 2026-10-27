#ifndef MNU_ITEM_REQUIREMENT_API_H
#define MNU_ITEM_REQUIREMENT_API_H

#include "common.h"

/* Zero means no selected item; nonzero keys must be in [0xC0, 0x100). */
typedef s32 MnuRequirementItemId;

enum {
    MNU_REQUIREMENT_ITEM_ID_FIRST = 0xC0,
    MNU_REQUIREMENT_ITEM_ID_LIMIT = 0x100,
};

u8 mnuGetItemRequirementCount(MnuRequirementItemId itemId);
s8 mnuGetItemRequirementValue(MnuRequirementItemId itemId);

#endif /* MNU_ITEM_REQUIREMENT_API_H */
