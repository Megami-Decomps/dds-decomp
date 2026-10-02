#ifndef GS_PACKET_H
#define GS_PACKET_H

#include "common.h"

/* RGBA8888 pack from byte components (counter plates and similar packets). */
#define PACK(r, g, b, a) ((u8)(r) | ((a) << 24) | (((u8)(b) << 16) | ((u8)(g) << 8)))

/* Screen dimensions the kernel draw packets target. */
#define DRAW_VIEWPORT_WIDTH 0x200
#define DRAW_VIEWPORT_HEIGHT 0x1C0

#endif /* GS_PACKET_H */
