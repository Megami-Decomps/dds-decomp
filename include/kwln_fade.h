#ifndef KWLN_FADE_H
#define KWLN_FADE_H

#include "common.h"

/* Return nonzero while either foreground fade direction is enabled. */
s32 kwlnFadeIsActive(void);

/* Foreground alpha transitions, stepped by kwlnFadeUpdate. */
void kwlnFadeInStart(s32 red, s32 green, s32 blue, s32 duration);
void kwlnFadeOutStart(s32 red, s32 green, s32 blue, s32 duration);
void kwlnFadeStartIn(s32 duration);
void kwlnFadeStartOut(s32 duration);

/* Configure the foreground fade timing mode and frame duration. */
void kwlnFadeSetupFrames(s32 mode, s32 frames);
/* Disable configured timing while retaining the stored parameters. */
void kwlnCancelConfiguredFadeFrames(void);

/* Select the background fade visibility mode used by scene transitions. */
void kwlnFadeSetMode(s32 visibilityMode);

#endif /* KWLN_FADE_H */
