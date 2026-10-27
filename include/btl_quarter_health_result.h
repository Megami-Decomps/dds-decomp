#ifndef BTL_QUARTER_HEALTH_RESULT_H
#define BTL_QUARTER_HEALTH_RESULT_H

#include "common.h"

struct BtlUnit;

typedef s32 BtlQuarterHealthThresholdResult;

/* The title predicates compare hp * 100 / maxHp with 25 in DDS1 and 26 in DDS2. */
#define BTL_QUARTER_HEALTH_NOT_BELOW 0
#define BTL_QUARTER_HEALTH_BELOW 1

BtlQuarterHealthThresholdResult btlIsCurrentValueBelowQuarterThreshold(
    struct BtlUnit *unit);

#endif /* BTL_QUARTER_HEALTH_RESULT_H */
