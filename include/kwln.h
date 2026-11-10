#ifndef KWLN_H
#define KWLN_H

#include "common.h"

/* Shared fog/depth state consumed by sdf3DMakeFogPacket (0x14).
 * The viewer config serializes fogNear, depthNear, fogFar, depthFar, fogColor;
 * timeline payloads store fogFar before fogNear instead. */
typedef struct KwlnDrawVectorParams {
    f32 fogNear;    /* 0x00: minimum fog range. */
    f32 fogFar;     /* 0x04: maximum fog range. */
    f32 depthNear;  /* 0x08: near depth used in the projection coefficients. */
    f32 depthFar;   /* 0x0C: far depth used in the projection coefficients. */
    u32 fogColor;   /* 0x10: packed GS FOGCOL value. */
} KwlnDrawVectorParams;

typedef char KwlnDrawVectorParams_size[(sizeof(KwlnDrawVectorParams) == 0x14) ? 1 : -1];

#endif /* KWLN_H */
