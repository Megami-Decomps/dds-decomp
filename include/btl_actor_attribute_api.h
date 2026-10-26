#ifndef BTL_ACTOR_ATTRIBUTE_API_H
#define BTL_ACTOR_ATTRIBUTE_API_H

#include "common.h"
#include "dat_stat.h"

/* These actor aggregators index the five base-stat slots in party records. */
s32 btlSumOrAverageActorAttribute(
    u32 partyRecordFlagsMask, DatBaseStatIndex baseStatIndex, s8 actorFilterMode);
void btlAverageFilteredActorAttribute(
    u32 partyRecordFlagsMask, DatBaseStatIndex baseStatIndex);
s32 btlAverageActorAttribute(
    u32 partyRecordFlagsMask, DatBaseStatIndex baseStatIndex);

#endif /* BTL_ACTOR_ATTRIBUTE_API_H */
