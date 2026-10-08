#ifndef FILE_SLOT_H
#define FILE_SLOT_H

#include "common.h"
#include "eff_curve.h"
#include "sdf.h"

/* Primary cells and their address-valued record, shared by both file engines. */
typedef struct FileSlot {
    f32 pos[4];       /* 0x00 */
    s32 state;        /* 0x10: frame, -1 available, -2 disabled */
    u32 color;        /* 0x14 */
    f32 scale;        /* 0x18 */
    f32 angle;        /* 0x1C */
} FileSlot;

typedef struct FileSlotTable {
    u16 type;
    u8 pad02[2];
    u32 instances;         /* 0x04: target's primary-slot count */
    u32 count;             /* 0x08: primary + trailing group cells */
    u32 flags;             /* 0x0C */
    u32 references;        /* 0x10: acquisition/frame counter */
    f32 spawnRemainder;    /* 0x14 */
    FileSlot *slots; /* 0x18 */
    u8 *unk1C;             /* 0x1C: per-type instance work */
    u8 *data0;             /* 0x20 */
    u8 *data1;             /* 0x24 */
    u32 handle;            /* 0x28 */
} FileSlotTable;

/* The track's +0x0C word is a real emitter random multiplier; the ordinary
 * curve sampler treats it as reserved. Both are members of the serialized
 * parameter format, not a second runtime-record view. */
typedef union FileKeyScalarTrack {
    EffScalarTrack track;
    struct {
        u8 mode;
        u8 reserved01[3];
        f32 initialValue;
        f32 finalValue;
        f32 randomness;
        u8 headingMode;
        u8 reserved11[3];
        f32 firstValue;
        f32 firstFraction;
        f32 secondValue;
        f32 secondFraction;
        u8 reserved24[8];
    } emitter;
} FileKeyScalarTrack;

/* FileSlotTable.type selects the serialized emitter tail. DDS2 003E95C0's
 * descriptors copy E0..124 bytes; FileKeyBlock is their existing primary
 * parameter owner. Its +54 surface index is also consumed as signed low 16
 * bits by billboard openers. Grid-group dimensions occupy C0/C4. */
typedef struct FileKeyBlock {
    f32 pos[4];             /* 0x00 */
    f32 orientation[4];    /* 0x10: emitter quaternion */
    s32 emissionDuration;   /* 0x20 */
    u32 spawnRate;          /* 0x24: emitter samplers convert this count unsigned */
    f32 spawnVariance;      /* 0x28 */
    SdfColorTrack colorTrack; /* 0x2C */
    SdfAlphaTrack alphaTrack; /* 0x50: surfaceIndex is read word-wide or narrowed to s16 */
    FileKeyScalarTrack scale;   /* 0x60 */
    FileKeyScalarTrack heading; /* 0x8C */
    s32 length;            /* 0xB8 */
    u8 padBC;              /* 0xBC: allocator's relative-position flag */
    u8 prewarm;            /* 0xBD */
    u8 padBE[2];
    s32 columns;            /* 0xC0: trailing-group columns */
    s32 rows;               /* 0xC4: trailing-group rows */
    union {                /* 0xC8: record-type-specific emitter parameters */
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
        } radial;
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 spread;
            f32 spreadRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
        } directed;
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 spread;
            f32 spreadRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
            s32 azimuthDegrees; /* 0xE8: signed angular extent */
        } sector;
        struct {
            f32 initialRadius;
            f32 axialSpeed;
            f32 axialSpeedRandomness;
            f32 axialDeceleration;
            f32 initialAmplitude;
            f32 initialAmplitudeRandomness;
            f32 finalAmplitude;
            f32 finalAmplitudeRandomness;
            f32 phaseStep;
            f32 phaseStepRandomness;
        } wave;
        struct {
            f32 initialAxialExtent;
            f32 initialRadius;
            f32 initialRadiusRandomness;
            f32 finalRadius;
            f32 finalRadiusRandomness;
            f32 angularSpeed;
            f32 angularSpeedRandomness;
            f32 axialSpeed;
            f32 axialSpeedRandomness;
            f32 angularAcceleration;
            f32 gravity;
        } circular;
        struct {
            f32 initialRadius;
            f32 initialRadiusRandomness;
            f32 finalRadius;
            f32 finalRadiusRandomness;
            f32 angularSpeed;
            f32 angularSpeedRandomness;
            f32 angularAcceleration;
            f32 gravity;
        } orientedRing;
        struct {
            s16 firstPercent;       /* C8 */
            u8 padCA[2];
            f32 firstOffset;
            f32 firstRandomness;
            s16 secondPercent;      /* D4 */
            u8 padD6[2];
            f32 secondOffset;
            f32 secondRandomness;
            s16 spreadDegrees;      /* E0 */
            u8 padE2[2];
            f32 angularSpeed;
            f32 angularRandomness;
            f32 angularAcceleration;
            f32 travelSpeed;
            f32 travelRandomness;
            f32 travelAcceleration;
            f32 start[3];           /* FC */
            f32 end[3];             /* 108 */
            u8 reserved114[8];
            f32 endpointRadius;     /* 11C */
            f32 endpointDrop;
        } curve;
    } emitter;
} FileKeyBlock;

/* The existing root-pointer conventions are 4-byte aligned, not VU types. */
typedef char FileSlotSizeCheck[(sizeof(FileSlot) == 0x20) ? 1 : -1];
typedef char FileSlotTableSizeCheck[(sizeof(FileSlotTable) == 0x2C) ? 1 : -1];
typedef char FileKeyScalarTrackSizeCheck[(sizeof(FileKeyScalarTrack) == 0x2C) ? 1 : -1];
typedef char FileKeyBlockColorOffsetCheck[((u32)&((FileKeyBlock *)0)->colorTrack == 0x2C) ? 1 : -1];
typedef char FileKeyBlockSurfaceOffsetCheck[((u32)&((FileKeyBlock *)0)->alphaTrack.surfaceIndex == 0x54) ? 1 : -1];
typedef char FileKeyBlockColumnsOffsetCheck[((u32)&((FileKeyBlock *)0)->columns == 0xC0) ? 1 : -1];
typedef char FileKeyBlockRowsOffsetCheck[((u32)&((FileKeyBlock *)0)->rows == 0xC4) ? 1 : -1];

#endif
