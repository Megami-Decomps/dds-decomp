#ifndef BTL_COMMAND_TARGET_ELIGIBILITY_H
#define BTL_COMMAND_TARGET_ELIGIBILITY_H

#include "btl_command_id.h"

typedef s32 BtlCommandTargetEligibilityResult;

enum {
    BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_NONE = 0,
    BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_REQUIREMENTS_MET = 3,
    BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_ENTRY_FLAG_40 = 5,
};

#if defined(VERSION_DDS1)
enum {
    BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_COMMAND_RESTRICTED = 8,
};
#elif defined(VERSION_DDS2)
enum {
    BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_ENTRY_FLAG_8000 = 7,
    BTL_COMMAND_TARGET_ELIGIBILITY_RESULT_COMMAND_RESTRICTED = 9,
};
#endif

struct BtlIndexList;

/* Check the selected targets against the command's target requirements. */
BtlCommandTargetEligibilityResult btlGetCommandTargetEligibility(
    struct BtlIndexList *indexList, BtlCommandId commandId);

#endif /* BTL_COMMAND_TARGET_ELIGIBILITY_H */
