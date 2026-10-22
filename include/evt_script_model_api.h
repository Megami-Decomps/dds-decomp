#ifndef EVT_SCRIPT_MODEL_API_H
#define EVT_SCRIPT_MODEL_API_H

#include "eff_transform.h"
#include "sdf_draw.h"

/* Slot 3 contains the script-model world node consumed by these helpers. */
sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *object);
sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *object, sdfModelRootNode_t *model);

#endif /* EVT_SCRIPT_MODEL_API_H */
