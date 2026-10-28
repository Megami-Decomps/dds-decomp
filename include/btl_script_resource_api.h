#ifndef BTL_SCRIPT_RESOURCE_API_H
#define BTL_SCRIPT_RESOURCE_API_H

#include "btl_find_script_resource_api.h"

typedef s32 BtlPrimaryScriptTaskAvailability;
enum {
    BTL_PRIMARY_SCRIPT_TASK_CANNOT_START = 0,
    BTL_PRIMARY_SCRIPT_TASK_CAN_START = 1,
};

/* Availability of the secondary scene script for the terminal transition. */
typedef s32 BtlSecondaryScriptResourceAvailability;
enum {
    BTL_SECONDARY_SCRIPT_RESOURCE_UNAVAILABLE = 0,
    BTL_SECONDARY_SCRIPT_RESOURCE_AVAILABLE = 1,
};

/* Whether the tracked battle script task still blocks resource-release work. */
typedef s32 BtlScriptResourceReleaseReadiness;
enum {
    BTL_SCRIPT_RESOURCE_RELEASE_PENDING = 0,
    BTL_SCRIPT_RESOURCE_RELEASE_READY = 1,
};

BtlScriptResourceReleaseReadiness btlReleaseScriptResource(void);
BtlScriptResourceReleaseReadiness btlReleaseScriptResourceA(void);
BtlScriptResourceReleaseReadiness btlReleaseScriptResourceB(void);
BtlPrimaryScriptTaskAvailability btlCanStartPrimaryScriptTask(void);
BtlSecondaryScriptResourceAvailability btlHasScriptResource(void);

#endif /* BTL_SCRIPT_RESOURCE_API_H */
