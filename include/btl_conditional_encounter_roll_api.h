#ifndef BTL_CONDITIONAL_ENCOUNTER_ROLL_API_H
#define BTL_CONDITIONAL_ENCOUNTER_ROLL_API_H

#include "common.h"

typedef s32 BtlConditionalEncounterGaugeThreshold;
typedef s32 BtlConditionalEncounterRollResult;

#define BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE 0
#define BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION 0

/* Zero means no usable encounter interval; positive values are gauge thresholds. */
BtlConditionalEncounterGaugeThreshold
btlGetConditionalEncounterInterval(s32 encounterIndex);

/* Zero means no selection; otherwise the signed word packs group, row, and scene. */
BtlConditionalEncounterRollResult
btlAdvanceConditionalEncounterRoll(f32 step, s32 encounterIndex);

#endif /* BTL_CONDITIONAL_ENCOUNTER_ROLL_API_H */
