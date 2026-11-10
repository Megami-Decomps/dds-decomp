#ifndef KWLN_H
#define KWLN_H

#include "common.h"

/* What sdf3DMakeFogPacket is handed: the two fog densities and the two
 * distances they span, plus the colour. sdf3DFogParam_t in draw3d.h, which
 * these units cannot include until they take libsdf.h. */
typedef struct KwlnDrawVectorParams {
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
    u32 color;
} KwlnDrawVectorParams;

typedef char KwlnDrawVectorParams_size[(sizeof(KwlnDrawVectorParams) == 0x14) ? 1 : -1];

#endif /* KWLN_H */
