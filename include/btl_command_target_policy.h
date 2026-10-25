#ifndef BTL_COMMAND_TARGET_POLICY_H
#define BTL_COMMAND_TARGET_POLICY_H

#include "btl_task.h"

typedef u32 BtlCommandTargetPolicy;

/* Returns the raw target category, including values copied from targetType
 * metadata and the provider's default category. Callers interpret the values. */
#ifdef VERSION_DDS1
BtlCommandTargetPolicy btlSelectCommandTargetPolicy(
    BtlTask *source, BtlIndexList *destination, s32 unused);
#elif defined(VERSION_DDS2)
BtlCommandTargetPolicy btlSelectCommandTargetPolicy(
    ActionStateLink *source, BtlIndexList *destination, s32 unused);
#endif

#endif /* BTL_COMMAND_TARGET_POLICY_H */
