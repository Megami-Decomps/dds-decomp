#ifndef SDF_MOTION_NODE_API_H
#define SDF_MOTION_NODE_API_H

#include "common.h"

struct sdfMotionCtrlr_t;
struct SdfMotionBindingHead;

struct SdfMotionBindingHead *sdfMotionNodeNew(struct sdfMotionCtrlr_t *motion, u32 commandWord,
                                              s32 targetIndex);

#endif /* SDF_MOTION_NODE_API_H */
