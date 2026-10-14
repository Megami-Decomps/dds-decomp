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

/* Select the background fade visibility mode used by scene transitions. */
void kwlnFadeSetMode(s32 visibilityMode);

#endif /* KWLN_FADE_H */
