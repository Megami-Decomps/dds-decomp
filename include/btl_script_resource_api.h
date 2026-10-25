#ifndef BTL_SCRIPT_RESOURCE_API_H
#define BTL_SCRIPT_RESOURCE_API_H

#include "common.h"

typedef s32 BtlPrimaryScriptTaskAvailability;
enum {
    BTL_PRIMARY_SCRIPT_TASK_CANNOT_START = 0,
    BTL_PRIMARY_SCRIPT_TASK_CAN_START = 1,
};

s32 btlReleaseScriptResource(void);
s32 btlReleaseScriptResourceA(void);
BtlPrimaryScriptTaskAvailability btlCanStartPrimaryScriptTask(void);

#endif /* BTL_SCRIPT_RESOURCE_API_H */
