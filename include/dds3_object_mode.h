#ifndef DDS3_OBJECT_MODE_H
#define DDS3_OBJECT_MODE_H

#include "common.h"

struct dds3Basic_t;

typedef u32 Dds3ObjectMatterAnimeMode;

enum Dds3ObjectMatterAnimeMode {
    DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED = 0,
    DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_FAST = 1,
    DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_SLOW = 2,
    DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO = 3,
    DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_FAST = 4,
    DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_SLOW = 5,
    DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END = 6,
};

/* Full-speed and ramp-down requests start at weight 1; the others start at 0.
 * Unrecognized values leave the current mode and weight unchanged. */
void dds3ObjectBaseMatterAnimeSpeed(struct dds3Basic_t *object,
                                    Dds3ObjectMatterAnimeMode requestedMode);

#endif /* DDS3_OBJECT_MODE_H */
