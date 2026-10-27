#ifndef BTL_COMMAND_HIT_COUNT_OVERRIDE_H
#define BTL_COMMAND_HIT_COUNT_OVERRIDE_H

#include "common.h"

struct BtlUnit;

/* Positive results replace the command-derived hit count; nonpositive results
 * use the normal command range. */
typedef s32 BtlCommandHitCountOverrideResult;
typedef BtlCommandHitCountOverrideResult (*BtlCommandHitCountOverrideCallback)(
    struct BtlUnit *unit, s32 command);

#endif /* BTL_COMMAND_HIT_COUNT_OVERRIDE_H */
