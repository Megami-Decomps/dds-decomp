#ifndef SDF_GRAPH_DISPLAY_H
#define SDF_GRAPH_DISPLAY_H

#include "common.h"

/* Partial SCE display-environment view: the provider initializes the opaque
 * prefix, then this code writes the nine-bit framebuffer page at +0x10 before
 * submitting the same environment. The wider environment extent is unknown. */
typedef struct SdfDisplayEnvView {
    u8 opaque_00[0x10];
    u32 framebufferPage : 9;
    u32 opaque_10 : 23;
} SdfDisplayEnvView;

#endif
