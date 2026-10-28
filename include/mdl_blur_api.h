#ifndef MDL_BLUR_API_H
#define MDL_BLUR_API_H

#include "common.h"

typedef struct dds3ModelHandle_t dds3ModelHandle_t;

/* The kind-four stream record supplies two independent blur controls: its
 * first selector sets the update interval between cloned snapshots; its
 * second sets the total snapshot budget. The model owner stores the remaining
 * budget and the interval countdown separately. */
typedef s32 MdlBlurSpawnInterval;
typedef s32 MdlBlurSnapshotCapacity;

void mdlSetBlur(dds3ModelHandle_t *ctx, MdlBlurSpawnInterval spawnInterval,
                MdlBlurSnapshotCapacity snapshotCapacity,
                u32 endColor, u32 startColor);

#endif /* MDL_BLUR_API_H */
