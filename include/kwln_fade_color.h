#ifndef KWLN_FADE_COLOR_H
#define KWLN_FADE_COLOR_H

#include "common.h"

/* The screen fade's colour. Its setters store each component; the fade
 * overlay (kwlnQueueBackgroundFadeOverlayPacket) reads each one unsigned
 * (lbu), and its alpha as 0x80 minus the component. */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} KwlnFadeColor;

extern KwlnFadeColor kwlnFadeColor;

#endif /* KWLN_FADE_COLOR_H */
