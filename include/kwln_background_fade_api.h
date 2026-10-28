#ifndef KWLN_BACKGROUND_FADE_API_H
#define KWLN_BACKGROUND_FADE_API_H

#include "common.h"

typedef s32 KwlnBackgroundOverlayActivity;

enum {
    KWLN_BACKGROUND_OVERLAY_INACTIVE = 0,
    KWLN_BACKGROUND_OVERLAY_ACTIVE = 1
};

/* Start the background fade-out countdown; zero clears the transition. */
void kwlnFadeBackgroundStartOut(s32 duration);
KwlnBackgroundOverlayActivity kwlnFadeIsBackgroundOverlayActive(void);

#endif /* KWLN_BACKGROUND_FADE_API_H */
