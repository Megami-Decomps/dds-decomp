#ifndef EFF_SPAN_WORK_H
#define EFF_SPAN_WORK_H

#include "common.h"
#include "sdf.h"

struct EffPointSet;
struct EffTrackSet;
struct SdfMemBlock;

/* Span table layout shared by the DDS1/DDS2 model-effect implementations. */
typedef struct EffSpanEntry {
    f32 first;
    f32 second;
    u32 referenceAge;
} EffSpanEntry;

typedef struct EffSpanRecord {
    EffSpanEntry *entries;
    struct EffPointSet *pointSet;
    struct EffTrackSet *references;
    u32 pointCount;
} EffSpanRecord;

typedef struct EffSpanTable {
    EffSpanRecord *records;
    u32 count;
    u16 total;
    u8 pad0A[2];
    struct SdfMemBlock *allocation;
} EffSpanTable;

/* Inline source prefix consumed by the paired particle-span owners. Bytes in
 * each explicit pad remain opaque; the operation table supplies copy size, so
 * this declaration does not assert the full copied payload extent. */
typedef struct EffSpanConfig {
    SdfColorTrack pointColorTrack;       /* 0x00 */
    SdfAlphaTrack pointAlphaTrack;       /* 0x24 */
    u32 progress;                        /* 0x34 */
    u8 drawPoints;                       /* 0x38 */
    u8 pad39[3];
    u32 middleColor;                     /* 0x3C */
    u32 edgeColor;                       /* 0x40 */
    u8 pad44[4];
    f32 drawScale;                       /* 0x48 */
    u8 pad4C[4];
    SdfColorTrack referenceColorTrack;   /* 0x50 */
    SdfAlphaTrack referenceAlphaTrack;   /* 0x74 */
    s32 referenceLifetime;               /* 0x84 */
    u8 drawReferences;                   /* 0x88 */
    u8 allowMultipleReferenceStarts;     /* 0x89 */
    u8 pad8A[2];
    s32 geometryStartUpdateCount;        /* 0x8C */
    u32 referenceColorA;                 /* 0x90 */
    u32 referenceColorB;                 /* 0x94 */
    f32 positionScaleA;                  /* 0x98 */
    f32 positionScaleB;                  /* 0x9C */
    f32 firstRand;                       /* 0xA0 */
    f32 secondBase;                      /* 0xA4 */
    f32 rangeRand;                       /* 0xA8 */
    s32 referenceRampDuration;           /* 0xAC */
    u32 perSpan;                         /* 0xB0 */
    f32 rotationAngularVelocity;          /* 0xB4 */
    f32 rotationAngularAcceleration;      /* 0xB8 */
    u8 pointSetFlag;                     /* 0xBC */
} EffSpanConfig;

typedef char EffSpanEntry_size_check[
    (sizeof(EffSpanEntry) == 0x0C) ? 1 : -1];
typedef char EffSpanRecord_size_check[
    (sizeof(EffSpanRecord) == 0x10) ? 1 : -1];
typedef char EffSpanTable_size_check[
    (sizeof(EffSpanTable) == 0x10) ? 1 : -1];
typedef char EffSpanTable_allocation_offset_check[
    ((u32)&((EffSpanTable *)0)->allocation == 0x0C) ? 1 : -1];
typedef char EffSpanConfig_progress_offset_check[
    ((u32)&((EffSpanConfig *)0)->progress == 0x34) ? 1 : -1];
typedef char EffSpanConfig_referenceTracks_offset_check[
    ((u32)&((EffSpanConfig *)0)->referenceColorTrack == 0x50) ? 1 : -1];
typedef char EffSpanConfig_referenceLifetime_offset_check[
    ((u32)&((EffSpanConfig *)0)->referenceLifetime == 0x84) ? 1 : -1];
typedef char EffSpanConfig_perSpan_offset_check[
    ((u32)&((EffSpanConfig *)0)->perSpan == 0xB0) ? 1 : -1];
typedef char EffSpanConfig_pointSetFlag_offset_check[
    ((u32)&((EffSpanConfig *)0)->pointSetFlag == 0xBC) ? 1 : -1];

#endif /* EFF_SPAN_WORK_H */
