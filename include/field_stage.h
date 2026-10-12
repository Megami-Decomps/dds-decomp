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

/* Contiguous player-scene work: saved transform, status words and deferred resource.
 * The data also exports a label at +0x3C for separate object-slot consumers. */
typedef struct FieldPlayerSceneWork {
    u128 position;
    u128 rotation;
    u8 pad20[0x14];
    u32 primaryState;
    u8 pad38[0x20];
    u32 unk58;
    u32 secondaryState;
    u8 pad60[2];
    s16 sequenceMode; /* 0x62: copied by the sequence initializer */
    u8 pad64[0x1C];
    char resourceName[0x10]; /* 0x80: sequence note; nonzero source is "elvstart_label". */
    s32 sequenceOption; /* 0x90: read by DDS2 func_00125380, separately copied from packet options. */
    u8 pad94[0xC];
} FieldPlayerSceneWork;

typedef char FieldPlayerSceneWork_size_must_be_0xA0[(sizeof(FieldPlayerSceneWork) == 0xA0) ? 1 : -1];
typedef char FieldPlayerSceneWork_unk58_at_58[((u32)&((FieldPlayerSceneWork *)0)->unk58 == 0x58) ? 1 : -1];
typedef char FieldPlayerSceneWork_resourceName_at_80[((u32)&((FieldPlayerSceneWork *)0)->resourceName == 0x80) ? 1 : -1];
typedef char FieldPlayerSceneWork_sequenceOption_at_90[((u32)&((FieldPlayerSceneWork *)0)->sequenceOption == 0x90) ? 1 : -1];

FieldStageCoordinate *fldFindStageCoordinateRecord(s32 x, s32 y);

#endif /* FIELD_STAGE_H */
