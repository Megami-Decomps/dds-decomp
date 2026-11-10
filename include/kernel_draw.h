#ifndef KERNEL_DRAW_H
#define KERNEL_DRAW_H

#include "sdf.h"
#include "draw3d.h"

/* Native offsets: parallel lights 0x00, ambient 0x10, fog 0x20. */
typedef struct
{
    sdf3DLight_t *parallel[4];
    sdfVector4_t ambient;
    sdf3DFogParam_t fog;
} dds3LightState_t;

/* Native frame renderer ordering table: 98 entries in DDS1 and DDS2. */
extern sdfGrOt_t sdfGrOtTable[0x62];

#endif /* KERNEL_DRAW_H */
