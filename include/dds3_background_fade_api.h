#ifndef DDS3_BACKGROUND_FADE_API_H
#define DDS3_BACKGROUND_FADE_API_H

#include "common.h"

typedef s32 dds3BackgroundOverlayActivity_t;

enum {
    DDS3_BACKGROUND_OVERLAY_INACTIVE = 0,
    DDS3_BACKGROUND_OVERLAY_ACTIVE = 1
};

/* Start the background fade-out countdown; zero clears the transition. */
void dds3FadeBackgroundStartOut(s32 duration);
dds3BackgroundOverlayActivity_t dds3FadeIsBackgroundOverlayActive(void);

#endif /* DDS3_BACKGROUND_FADE_API_H */
