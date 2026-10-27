#ifndef BTL_ACTION_ELIGIBILITY_H
#define BTL_ACTION_ELIGIBILITY_H

#include "common.h"

/* Both status-action gates return exactly NOT_ELIGIBLE or ELIGIBLE. */
typedef s32 BtlActionEligibilityResult;
enum {
    BTL_ACTION_NOT_ELIGIBLE = 0,
    BTL_ACTION_ELIGIBLE = 1,
};

#if defined(VERSION_DDS1)
struct BtlTask;
BtlActionEligibilityResult btlCheckStatusActionEligibility(struct BtlTask *task);
BtlActionEligibilityResult btlAiCheckStatusRollEligibility(struct BtlTask *task);
#elif defined(VERSION_DDS2)
struct ActionStateLink;
BtlActionEligibilityResult btlCheckStatusActionEligibility(struct ActionStateLink *task);
BtlActionEligibilityResult btlAiCheckStatusRollEligibility(struct ActionStateLink *task);
#else
#error "btl_action_eligibility.h requires a DDS title version"
#endif

#endif /* BTL_ACTION_ELIGIBILITY_H */
