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
    u32 sequenceWord38; /* copied from the sequence packet */
    u32 sequenceWord3C; /* copied from the sequence packet */
    char entryName[0x10]; /* 0x40: kind-0x11 transform-source name */
    s32 area; /* 0x50: sequence destination area */
    s32 room; /* 0x54: one-based destination room */
    u32 unk58;
    u32 secondaryState;
    u16 sequenceCode; /* 0x60: copied from FieldSequenceRecord.code */
    s16 sequenceMode; /* 0x62: copied by the sequence initializer */
    s32 cameraLinkMode; /* 0x64: sequence packet link mode */
    u8 pad68[8];
    char cameraName[0x10]; /* 0x70: kind-4 camera-node name */
    char resourceName[0x10]; /* 0x80: sequence note; nonzero source is "elvstart_label". */
    s32 sequenceOption; /* 0x90: read by DDS2 func_00125380, separately copied from packet options. */
    u8 pad94[0xC];
} FieldPlayerSceneWork;

typedef char FieldPlayerSceneWork_size_must_be_0xA0[(sizeof(FieldPlayerSceneWork) == 0xA0) ? 1 : -1];
typedef char FieldPlayerSceneWork_unk58_at_58[((u32)&((FieldPlayerSceneWork *)0)->unk58 == 0x58) ? 1 : -1];
typedef char FieldPlayerSceneWork_resourceName_at_80[((u32)&((FieldPlayerSceneWork *)0)->resourceName == 0x80) ? 1 : -1];
typedef char FieldPlayerSceneWork_sequenceOption_at_90[((u32)&((FieldPlayerSceneWork *)0)->sequenceOption == 0x90) ? 1 : -1];
typedef char FieldPlayerSceneWork_sequenceWord38_at_38[((u32)&((FieldPlayerSceneWork *)0)->sequenceWord38 == 0x38) ? 1 : -1];
typedef char FieldPlayerSceneWork_sequenceWord3C_at_3C[((u32)&((FieldPlayerSceneWork *)0)->sequenceWord3C == 0x3C) ? 1 : -1];
typedef char FieldPlayerSceneWork_entryName_at_40[((u32)&((FieldPlayerSceneWork *)0)->entryName == 0x40) ? 1 : -1];
typedef char FieldPlayerSceneWork_area_at_50[((u32)&((FieldPlayerSceneWork *)0)->area == 0x50) ? 1 : -1];
typedef char FieldPlayerSceneWork_room_at_54[((u32)&((FieldPlayerSceneWork *)0)->room == 0x54) ? 1 : -1];
typedef char FieldPlayerSceneWork_sequenceCode_at_60[((u32)&((FieldPlayerSceneWork *)0)->sequenceCode == 0x60) ? 1 : -1];
typedef char FieldPlayerSceneWork_cameraLinkMode_at_64[((u32)&((FieldPlayerSceneWork *)0)->cameraLinkMode == 0x64) ? 1 : -1];
typedef char FieldPlayerSceneWork_cameraName_at_70[((u32)&((FieldPlayerSceneWork *)0)->cameraName == 0x70) ? 1 : -1];

FieldStageCoordinate *fldFindStageCoordinateRecord(s32 x, s32 y);

#endif /* FIELD_STAGE_H */
