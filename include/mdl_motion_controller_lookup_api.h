#ifndef MDL_MOTION_CONTROLLER_LOOKUP_API_H
#define MDL_MOTION_CONTROLLER_LOOKUP_API_H

#include "common.h"

typedef struct dds3ModelHandle_t dds3ModelHandle_t;
struct sdfMotionCtrlr_t;

/* Borrow a controller owned by ctx. The pointer is read-only and expires when
 * that controller is removed or the model context is freed. */
const struct sdfMotionCtrlr_t *mdlGetMotionCtrlr(
    const dds3ModelHandle_t *ctx, s32 searchId);

/* Use only when the caller must update the borrowed controller in place. */
static inline struct sdfMotionCtrlr_t *mdlGetMutableMotionCtrlr(
        dds3ModelHandle_t *ctx, s32 searchId) {
    return (struct sdfMotionCtrlr_t *)mdlGetMotionCtrlr(ctx, searchId);
}

#endif /* MDL_MOTION_CONTROLLER_LOOKUP_API_H */
