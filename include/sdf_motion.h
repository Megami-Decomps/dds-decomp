#ifndef SDF_MOTION_H
#define SDF_MOTION_H

#include "common.h"

struct sdfMotionCtrlr_t;
struct MotionTable;
struct sdfModelRootNode_t;

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} SdfMotionState;

struct sdfMotionCtrlr_t *sdfMotionCtrlrNew(struct sdfModelRootNode_t *model, struct MotionTable *table);
void sdfMotionCtrlrFree(struct sdfMotionCtrlr_t *motion);
void sdfMotionSeek(struct sdfMotionCtrlr_t *motion, f32 frame);
void sdfMotionPause(struct sdfMotionCtrlr_t *motion);
void sdfMotionPauseOff(struct sdfMotionCtrlr_t *motion);

#endif /* SDF_MOTION_H */
