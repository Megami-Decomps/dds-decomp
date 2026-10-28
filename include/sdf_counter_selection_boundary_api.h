#ifndef SDF_COUNTER_SELECTION_BOUNDARY_API_H
#define SDF_COUNTER_SELECTION_BOUNDARY_API_H

#include "common.h"

/* Independent flags for list items before and after the visible selection. */
typedef s32 SdfCounterSelectionBoundaryFlags;
enum {
    SDF_COUNTER_SELECTION_BOUNDARY_NONE = 0,
    SDF_COUNTER_SELECTION_HAS_PREVIOUS_ITEMS = 1,
    SDF_COUNTER_SELECTION_HAS_FOLLOWING_ITEMS = 2,
};

SdfCounterSelectionBoundaryFlags sdfCounterGetSelectionBoundaryFlags(void);
void mnuSetMapTimerFlags(SdfCounterSelectionBoundaryFlags boundaryFlags);

#endif /* SDF_COUNTER_SELECTION_BOUNDARY_API_H */
