#ifndef DDS3_FADE_COLOR_H
#define DDS3_FADE_COLOR_H

#include "common.h"

/* The screen fade's colour. Its setters store each component; the fade
 * overlay (dds3DrawFade) reads each one unsigned
 * (lbu), and its alpha as 0x80 minus the component. */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} dds3FadeColor_t;

extern dds3FadeColor_t fadergba;

#endif /* DDS3_FADE_COLOR_H */
