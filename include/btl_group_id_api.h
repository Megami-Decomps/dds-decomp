#ifndef BTL_GROUP_ID_API_H
#define BTL_GROUP_ID_API_H

#include "common.h"

/* Group-local IDs track model loads that are pending completion. Duplicate
 * IDs are allowed; removal releases only the first matching list entry. */
s32 btlGroupContainsId(s32 group, s32 id);
void btlAddGroupId(s32 group, s32 id);
void btlRemoveGroupId(s32 group, s32 id);

#endif /* BTL_GROUP_ID_API_H */
