#ifndef MNU_ABILITY_TARGET_CATEGORY_H
#define MNU_ABILITY_TARGET_CATEGORY_H

#include "common.h"

/* Menu-view category returned from a command's target_type metadata. */
typedef s32 MnuAbilityTargetCategory;
enum MnuAbilityTargetCategoryValue {
    MNU_ABILITY_TARGET_CATEGORY_UNSUPPORTED = 0,
    MNU_ABILITY_TARGET_CATEGORY_SINGLE_ENTRY = 1,
    MNU_ABILITY_TARGET_CATEGORY_PARTY_GROUP_1 = 2,
    MNU_ABILITY_TARGET_CATEGORY_PARTY_GROUP_2 = 3
};

/* Command zero maps to single-entry use; target_type values 0..2 map to
 * categories 1..3, and other values map to unsupported. */
MnuAbilityTargetCategory mnuGetAbilityTargetCategory(u16 commandId);

#endif /* MNU_ABILITY_TARGET_CATEGORY_H */
