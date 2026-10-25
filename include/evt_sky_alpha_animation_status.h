#ifndef EVT_SKY_ALPHA_ANIMATION_STATUS_H
#define EVT_SKY_ALPHA_ANIMATION_STATUS_H

#include "common.h"

typedef u16 EvtSkyAlphaAnimationStatus;
enum {
    EVT_SKY_ALPHA_ANIMATION_INACTIVE = 0,
    EVT_SKY_ALPHA_ANIMATION_ACTIVE = 1
};

EvtSkyAlphaAnimationStatus evtCheckSkyAlphaAnime(void);

#endif /* EVT_SKY_ALPHA_ANIMATION_STATUS_H */
