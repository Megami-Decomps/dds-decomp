#ifndef SDF_DEV_SLOT_API_H
#define SDF_DEV_SLOT_API_H

#include "common.h"

struct sdfModelRootNode_t;

void sdfModelRootFree(struct sdfModelRootNode_t *slot, s32 recycle, s32 release);

#endif /* SDF_DEV_SLOT_API_H */
