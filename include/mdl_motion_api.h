#ifndef MDL_MOTION_API_H
#define MDL_MOTION_API_H

#include "common.h"
#include "mdl_motion_search_id.h"

typedef struct dds3ModelHandle_t dds3ModelHandle_t;

typedef u8 MdlMotionPresence;
enum {
    MDL_MOTION_ABSENT = 0,
    MDL_MOTION_PRESENT = 1
};

/* A controller's u16 motion index widened to s32; -1 means no node was found. */
typedef s32 MdlMotionIndexResult;
enum {
    MDL_MOTION_INDEX_MISSING = -1
};

/* Context-level motion APIs; per-sdfMotionCtrlr_t lifecycle methods are in sdf_motion.h. */
MdlMotionIndexResult mdlGetMotionNum(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);
u16 mdlGetMotionLengthEx(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);
s32 mdlGetMotionFrameEx(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);
f32 mdlGetMotionSpeedEx(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);
MdlMotionPresence mdlHasMotionEx(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

void mdlSetMotionSpeedEx(dds3ModelHandle_t *ctx, MdlMotionSearchId searchId, f32 frameStep);
void mdlMotionPause(dds3ModelHandle_t *ctx);
void mdlMotionPauseOff(dds3ModelHandle_t *ctx);

#endif /* MDL_MOTION_API_H */
