#ifndef BTL_ACTOR_ATTRIBUTE_API_H
#define BTL_ACTOR_ATTRIBUTE_API_H

#include "common.h"
#include "btl_actor_group_average_api.h"
#include "dat_stat.h"

/* These actor aggregators index the five base-stat slots in party records. */
s32 btlSumOrAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask,
    DatBaseStatIndex baseStatIndex, s8 actorFilterMode);
void btlAverageFilteredActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask,
    DatBaseStatIndex baseStatIndex);
s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask,
    DatBaseStatIndex baseStatIndex);

#endif /* BTL_ACTOR_ATTRIBUTE_API_H */
