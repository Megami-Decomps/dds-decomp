#ifndef SDF_LIGHTING_H
#define SDF_LIGHTING_H

#include "sdf_types.h"

typedef f32 (*sdfLightSources_t[3])[4];

typedef struct sdf3DLightParallelPacket_t sdf3DLightParallelPacket_t;

/* The parallel-light packet. normal is the three light directions held as a
 * matrix's columns, so VU1 gets all three dot products from one multiply;
 * colorBase is a copy of color[0] and the ambient term lands in both color[3]
 * and color[4]. TM is the lighting matrix corePktEmitRefMapPass builds its
 * specular axis from. */
struct sdf3DLightParallelPacket_t
{
    sdfDmaTag_t dmatag;
    union
    {
        sdfVector4_t normal[3];
        f32 normalComponents[3][4];
    };
    sdfVector4_t colorBase;
    sdfVector4_t color[5];
    sdfMatrix4_t TM;
};

#endif /* SDF_LIGHTING_H */
