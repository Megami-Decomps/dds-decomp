#ifndef KWLN_FADE_H
#define KWLN_FADE_H

#include "common.h"

/* Return nonzero while either foreground fade direction is enabled. */
s32 dds3FadeCheck(void);

/* Foreground alpha transitions, stepped by dds3FadeUpdate. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);
void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);
void dds3FadeIn2(s32 duration);
void dds3FadeOut2(s32 duration);

/* Select the background fade visibility mode used by scene transitions. */
void kwlnFadeSetMode(s32 visibilityMode);

#endif /* KWLN_FADE_H */
