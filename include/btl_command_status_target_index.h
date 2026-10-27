#ifndef BTL_COMMAND_STATUS_TARGET_INDEX_H
#define BTL_COMMAND_STATUS_TARGET_INDEX_H

#include "btl_task.h"

typedef s32 BtlCommandStatusTargetListIndex;

/* Returns the zero-based index of the first eligible-list entry whose party
 * status intersects a nonzero command status mask. When no match applies,
 * returns 0 as the first-entry fallback; this does not report match presence. */
#ifdef VERSION_DDS1
BtlCommandStatusTargetListIndex btlFindCommandStatusMatchInTargetList(
    BtlTask *actionTask, BtlIndexList *targets);
#elif defined(VERSION_DDS2)
BtlCommandStatusTargetListIndex btlFindCommandStatusMatchInTargetList(
    ActionStateLink *actionTask, BtlIndexList *targets);
#else
#error "btl_command_status_target_index.h requires a DDS title version"
#endif

#endif /* BTL_COMMAND_STATUS_TARGET_INDEX_H */
