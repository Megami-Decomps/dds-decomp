#ifndef BTL_GROUP_ID_API_H
#define BTL_GROUP_ID_API_H

#include "common.h"

typedef s32 MdlGroupLoadPendingResult;

/* Group-local IDs track model loads that are pending completion. Duplicate
 * IDs are allowed; removal releases only the first matching list entry. */
/* Return 1 when this group has a registered pending load for id; otherwise 0. */
MdlGroupLoadPendingResult mdlCheckLoading(s32 group, s32 id);
void mdlRegLoading(s32 group, s32 id);
void mdlUnRegLoading(s32 group, s32 id);

#endif /* BTL_GROUP_ID_API_H */
