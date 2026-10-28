#ifndef BTL_SCENE_PARTY_LEVEL_THRESHOLD_API_H
#define BTL_SCENE_PARTY_LEVEL_THRESHOLD_API_H

#include "common.h"

typedef u8 BtlScenePartyLevelThresholdResult;
enum {
    BTL_SCENE_PARTY_LEVEL_THRESHOLD_REJECTED = 0,
    BTL_SCENE_PARTY_LEVEL_THRESHOLD_ACCEPTED = 1
};

/* Compare truncated averages: enemy average + party average / 4 >= party average.
 * A zero scene, empty enemy list, or no eligible frontline party members rejects. */
BtlScenePartyLevelThresholdResult
btlCheckScenePartyLevelThreshold(u32 sceneIndex);

#endif /* BTL_SCENE_PARTY_LEVEL_THRESHOLD_API_H */
