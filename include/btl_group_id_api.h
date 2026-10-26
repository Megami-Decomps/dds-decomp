#ifndef BTL_GROUP_ID_API_H
#define BTL_GROUP_ID_API_H

#include "common.h"

typedef s32 MdlGroupLoadPendingResult;

enum MdlGroupLoadPendingResultValue {
    MDL_GROUP_LOAD_NOT_PENDING = 0,
    MDL_GROUP_LOAD_PENDING = 1
};

/* Group-local IDs track model loads that are pending completion. Duplicate
 * IDs are allowed; removal releases only the first matching list entry. */
/* Return PENDING when this group has a registered load for id; otherwise
 * NOT_PENDING. */
MdlGroupLoadPendingResult mdlCheckLoading(s32 group, s32 id);
void mdlRegLoading(s32 group, s32 id);
void mdlUnRegLoading(s32 group, s32 id);

#endif /* BTL_GROUP_ID_API_H */
