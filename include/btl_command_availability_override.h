#ifndef BTL_COMMAND_AVAILABILITY_OVERRIDE_H
#define BTL_COMMAND_AVAILABILITY_OVERRIDE_H

#include "common.h"

struct BtlUnit;

/* Zero rejects command availability; any nonzero result permits it. */
typedef s32 BtlCommandAvailabilityOverrideResult;
typedef BtlCommandAvailabilityOverrideResult (*BtlCommandAvailabilityOverrideCallback)(
    struct BtlUnit *unit, s32 command);

#endif /* BTL_COMMAND_AVAILABILITY_OVERRIDE_H */
