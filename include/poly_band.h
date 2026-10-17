#ifndef DDS_POLY_BAND_H
#define DDS_POLY_BAND_H

#include "poly_ring_head.h"
#include "poly_ring_records.h"

struct ParSystem;
struct SdfMemBlock;

/* Band owner and parameter tail copied from the serialized ring template. */
typedef struct PolyBand {
    PolyRingHead head;
    u32 spawnDelayStep;
    u32 segments;
    f32 radialWidth;
    f32 initialRadius;
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    f32 liftStep;
    u8 padE0[4];
    void *dispatchArgB;             /* +0xE4 */
    u8 padE8[4];
    void *dispatchArgA;             /* +0xEC */
    struct ParSystem *strip;         /* +0xF0 */
    PolyBandRecord *records;         /* +0xF4 */
    struct SdfMemBlock *allocation;  /* +0xF8 */
    u8 padFC[4];
} PolyBand;

typedef char PolyBand_size_must_be_0x100[
    (sizeof(PolyBand) == 0x100) ? 1 : -1];
typedef char PolyBand_head_offset_must_be_0x00[
    ((u32)&((PolyBand *)0)->head == 0x00) ? 1 : -1];
typedef char PolyBand_spawnDelayStep_offset_must_be_0xC0[
    ((u32)&((PolyBand *)0)->spawnDelayStep == 0xC0) ? 1 : -1];
typedef char PolyBand_segments_offset_must_be_0xC4[
    ((u32)&((PolyBand *)0)->segments == 0xC4) ? 1 : -1];
typedef char PolyBand_radialWidth_offset_must_be_0xC8[
    ((u32)&((PolyBand *)0)->radialWidth == 0xC8) ? 1 : -1];
typedef char PolyBand_initialRadius_offset_must_be_0xCC[
    ((u32)&((PolyBand *)0)->initialRadius == 0xCC) ? 1 : -1];
typedef char PolyBand_targetRadius_offset_must_be_0xD0[
    ((u32)&((PolyBand *)0)->targetRadius == 0xD0) ? 1 : -1];
typedef char PolyBand_initialRadiusJitter_offset_must_be_0xD4[
    ((u32)&((PolyBand *)0)->initialRadiusJitter == 0xD4) ? 1 : -1];
typedef char PolyBand_targetRadiusJitter_offset_must_be_0xD8[
    ((u32)&((PolyBand *)0)->targetRadiusJitter == 0xD8) ? 1 : -1];
typedef char PolyBand_liftStep_offset_must_be_0xDC[
    ((u32)&((PolyBand *)0)->liftStep == 0xDC) ? 1 : -1];
typedef char PolyBand_dispatchArgB_offset_must_be_0xE4[
    ((u32)&((PolyBand *)0)->dispatchArgB == 0xE4) ? 1 : -1];
typedef char PolyBand_dispatchArgA_offset_must_be_0xEC[
    ((u32)&((PolyBand *)0)->dispatchArgA == 0xEC) ? 1 : -1];
typedef char PolyBand_strip_offset_must_be_0xF0[
    ((u32)&((PolyBand *)0)->strip == 0xF0) ? 1 : -1];
typedef char PolyBand_records_offset_must_be_0xF4[
    ((u32)&((PolyBand *)0)->records == 0xF4) ? 1 : -1];
typedef char PolyBand_allocation_offset_must_be_0xF8[
    ((u32)&((PolyBand *)0)->allocation == 0xF8) ? 1 : -1];

#endif
