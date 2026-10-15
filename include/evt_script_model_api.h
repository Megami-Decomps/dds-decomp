#ifndef EVT_SCRIPT_MODEL_API_H
#define EVT_SCRIPT_MODEL_API_H

#include "eff_transform.h"
#include "sdf_draw.h"

/* Slot 3 contains the script-model world node consumed by these helpers. */
SdfModel *evtCreateModelFromObject(EffWorldNode *object);
Motion *evtAttachScriptToObject(EffWorldNode *object, SdfModel *model);

#endif /* EVT_SCRIPT_MODEL_API_H */
