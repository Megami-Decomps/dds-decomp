#ifndef SDF_MOTION_NODE_API_H
#define SDF_MOTION_NODE_API_H

#include "common.h"

struct sdfMotionCtrlr_t;
struct SdfMotionBindingHead;

/* Motion factories return the shared binding prefix and receive the selected target. */
typedef struct SdfMotionBindingHead *(*SdfMotionBindingFactory)(
    struct sdfMotionCtrlr_t *motion, u32 selector, s32 targetIndex);

struct SdfMotionBindingHead *sdfMotionNodeNew(struct sdfMotionCtrlr_t *motion, u32 commandWord,
                                              s32 targetIndex);

#endif /* SDF_MOTION_NODE_API_H */
