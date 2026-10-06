#include "eff.h"
#include "eff_curve.h"

/* Primary FileRecordSlot/Slots and FileKeyBlock definitions from the existing menu owner. These are research inputs; coherent shared-owner promotion is required before production integration. */
typedef struct FileRecordSlot {
    f32 pos[4];       /* 0x00 */
    s32 state;        /* 0x10: frame, -1 available, -2 disabled */
    u32 color;        /* 0x14 */
    f32 scale;        /* 0x18 */
    f32 angle;        /* 0x1C */
} FileRecordSlot;      /* 0x20, ordinary 4-byte field alignment */

typedef struct FileRecordSlots {
    u16 type;
    u8 pad2[2];
    u32 instances;         /* 0x04: target's primary-slot count */
    u32 count;             /* 0x08: primary + trailing group cells */
    u32 flags;             /* 0x0C */
    u32 references;        /* 0x10: acquisition/frame counter */
    f32 spawnRemainder;    /* 0x14 */
    FileRecordSlot *slots; /* 0x18 */
    u8 *unk1C;             /* 0x1C: per-type instance work */
    u8 *data0;             /* 0x20 */
    u8 *data1;             /* 0x24 */
    u32 handle;            /* 0x28 */
} FileRecordSlots;


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

/* Keyframe tracks of a view block (scale, heading and colour curves). */
typedef struct FileKeyBlock {
    f32 pos[4];             /* 0x00 */
    f32 orientation[4];    /* 0x10: emitter quaternion */
    s32 emissionDuration;   /* 0x20 */
    u32 spawnRate;          /* 0x24 */
    f32 spawnVariance;      /* 0x28 */
    u8 unk2C[0x24];         /* 0x2C: color track */
    u8 unk50[4];
    s32 blendMode;         /* 0x54: draw surface and blend mode */
    u8 unk58[8];
    FileKeyScalarTrack scale;   /* 0x60 */
    FileKeyScalarTrack heading; /* 0x8C */
    s32 length;            /* 0xB8 */
    u8 padBC;              /* 0xBC: allocator's relative-position flag */
    u8 prewarm;            /* 0xBD */
    u8 padBE[0x0A];         /* includes grid columns/rows at C0/C4 */
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
    } emitter;
} FileKeyBlock;

/* Caller-built composite draw description; 0x68 bytes. */
typedef struct EffCompositeGsDescriptor {
    f32 position[4];
    f32 scaleX;
    f32 scaleY;
    u8 unk18[8];
    f32 angle;
    u32 color;
    u32 blendMode;
    BillTextureQuad primaryUv;
    BillTextureQuad secondaryUv;
    SdfTex *primaryTexture;
    u64 primaryClamp;
    SdfTex *secondaryTexture;
    u8 unk5C[4];
    u64 secondaryClamp;
} EffCompositeGsDescriptor;

typedef char EffCompositeGsDescriptor_size_must_be_0x68[
    (sizeof(EffCompositeGsDescriptor) == 0x68) ? 1 : -1];


/* Common 0x3C scalar/UV header; kind-specific grid parameters follow. */
typedef struct EffSlotUvConfig {
    EffScalarTrack scale;
    f32 uExtent;
    f32 vExtent;
    f32 vStep;
    f32 uStep;
    u8 gridParameters[0];
} EffSlotUvConfig;
