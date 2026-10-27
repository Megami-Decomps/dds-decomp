#ifndef BTL_ACTOR_GROUP_AVERAGE_API_H
#define BTL_ACTOR_GROUP_AVERAGE_API_H

#include "common.h"

/* Selects actors by matching bits in their copied datUnitWork_t flags word. */
typedef u32 BtlPartyRecordFlagsMask;

s32 btlAverageAllCurrentForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);
s32 btlAverageAllMaximumForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);
s32 btlAverageAllLevelsForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

#endif /* BTL_ACTOR_GROUP_AVERAGE_API_H */
