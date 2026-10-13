#ifndef BTL_AI_SLOT_H
#define BTL_AI_SLOT_H

#include "common.h"

/* Selects a weighted AI slot from the species row; unusedUnit is retained by
 * the established call contract but is not read by either title's provider. */
u32 btlPickWeightedAiSlot(s32 unusedUnit, s32 speciesId, s32 rowIndex);

#endif /* BTL_AI_SLOT_H */
