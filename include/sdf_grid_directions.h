#ifndef SDF_GRID_DIRECTIONS_H
#define SDF_GRID_DIRECTIONS_H

#include "common.h"

/* Three signed column offsets consumed by the grid navigation searches. */
typedef struct SdfGridDirections {
    s8 columns[3];
} SdfGridDirections;

typedef char SdfGridDirectionsSizeCheck[
    sizeof(SdfGridDirections) == 3 ? 1 : -1];

#endif
