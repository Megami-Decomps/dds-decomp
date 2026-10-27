#ifndef EVT_SCRIPT_MODEL_API_H
#define EVT_SCRIPT_MODEL_API_H

#include "eff_transform.h"
#include "sdf_draw.h"

/* Read slot-3 model metadata without changing its world node. Model creation
 * returns a new root; motion creation attaches a new controller to that root. */
sdfModelRootNode_t *dds3ModelBasicMakeModel(const dds3Basic_t *object);
sdfMotionCtrlr_t *dds3ModelBasicSetMotion(const dds3Basic_t *object, sdfModelRootNode_t *model);

#endif /* EVT_SCRIPT_MODEL_API_H */
