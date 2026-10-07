#ifndef FIELD_STAGE_H
#define FIELD_STAGE_H

#include "common.h"

/* Stage-map coordinates: each resource record is exactly sixteen bytes.
 * The index and pointer lookups intentionally have different scan limits. */
typedef struct FieldStageCoordinate {
    s16 x;
    s16 y;
    f32 originX;
    f32 originZ;
    u8 cols;
    u8 rows;
    s16 cellSize;
} FieldStageCoordinate;

typedef char FieldStageCoordinate_size_must_be_0x10[(sizeof(FieldStageCoordinate) == 0x10) ? 1 : -1];
typedef char FieldStageCoordinate_originX_at_4[((u32)&((FieldStageCoordinate *)0)->originX == 4) ? 1 : -1];
typedef char FieldStageCoordinate_originZ_at_8[((u32)&((FieldStageCoordinate *)0)->originZ == 8) ? 1 : -1];
typedef char FieldStageCoordinate_cols_at_C[((u32)&((FieldStageCoordinate *)0)->cols == 0xC) ? 1 : -1];
typedef char FieldStageCoordinate_rows_at_D[((u32)&((FieldStageCoordinate *)0)->rows == 0xD) ? 1 : -1];
typedef char FieldStageCoordinate_cellSize_at_E[((u32)&((FieldStageCoordinate *)0)->cellSize == 0xE) ? 1 : -1];

FieldStageCoordinate *fldFindStageCoordinateRecord(s32 x, s32 y);

#endif /* FIELD_STAGE_H */
