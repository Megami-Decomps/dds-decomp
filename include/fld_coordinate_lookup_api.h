#ifndef FLD_COORDINATE_LOOKUP_API_H
#define FLD_COORDINATE_LOOKUP_API_H

#include "common.h"

/* These lookups address distinct field tables. A miss returns row zero, the
 * reserved fallback row, rather than an out-of-range sentinel. */
typedef s32 FldMapLabelRecordIndex;
typedef s32 FldSecondaryMapLabelRecordIndex;
typedef s32 FldMapCoordinateRecordIndex;

FldMapLabelRecordIndex fldFindMapLabelIndexByCoordinate(s32 mapId, s32 floorIndex);
FldSecondaryMapLabelRecordIndex fldFindSecondaryMapLabelIndexByCoordinate(
    s32 mapId, s32 floorIndex);
FldMapCoordinateRecordIndex fldFindMapCoordinateIndex(s32 mapId, s32 floorIndex);

#endif /* FLD_COORDINATE_LOOKUP_API_H */
