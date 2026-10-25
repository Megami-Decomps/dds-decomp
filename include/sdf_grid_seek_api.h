#ifndef DDS_SDF_GRID_SEEK_API_H
#define DDS_SDF_GRID_SEEK_API_H

#include "itf_grid_text.h"

typedef s32 SdfGridSeekResult;
enum {
    SDF_GRID_SEEK_RESULT_ITEMS_EXHAUSTED = 0,
    SDF_GRID_SEEK_RESULT_SEEK_APPLIED = 1
};

/* An offset at/after itemCount leaves the widget unchanged. Otherwise the
 * cursor is reset to the head and the requested positive offset is applied. */
SdfGridSeekResult sdfGridSeekSelectedNodeByIndex(s32 itemsToAdvance, GridTextWidget *widget);
SdfGridSeekResult sdfGridSeekFirstNode(GridTextWidget *widget);
SdfGridSeekResult sdfGridSeekLastNode(GridTextWidget *widget);

#endif /* DDS_SDF_GRID_SEEK_API_H */
