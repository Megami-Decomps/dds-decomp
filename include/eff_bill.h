#ifndef EFF_BILL_H
#define EFF_BILL_H

#include "common.h"
#include "sdf.h"

#include "eff_curve.h"

/* Class operations select the copied parameter format and own this 0x40-byte
 * header. The resource word is the class factory's returned handle/address. */
typedef struct EffClassWork {
    union {
        u8 transform[0x20];
        struct {
            f32 position[4];
            f32 orientation[4];
        } vectors;
    };
    f32 scale;
    u32 color;
    u32 frame;
    s32 kind;
    u32 resource;
    void *payload;
    u8 pad38[8];
} EffClassWork;

/* Count-bearing serialized formats share the first 0x3C bytes. Native
 * point callbacks compare the time word signed; emitter callbacks compare it
 * unsigned. These are two real interpretations of the same serialized word. */
typedef struct EffBillTimedHeader {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    union {
        u32 progress;
        s32 duration;
    } time;
    u32 count;
} EffBillTimedHeader;

typedef struct EffBillOutputHeader {
    EffBillTimedHeader timed;
    u8 outputMode;
    u8 pad3D[3];
} EffBillOutputHeader;

typedef struct EffBillFrameHeader {
    EffBillOutputHeader output;
    u8 unk40[0x16];
    u8 mode;
    u8 pad57;
} EffBillFrameHeader;

/* DDS2 003E9950 / DDS1 0037E8A0 resource kinds 1..8 copy the following
 * record lengths. Only the header and animated-duration word are recovered;
 * the remaining bytes are still serialized data, not matching scratch space. */
typedef struct EffBillFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x28];
} EffBillFrameConfig; /* kind 1, 0x80 */

typedef struct EffBillCellConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x34];
} EffBillCellConfig; /* kind 2, 0x8C */

typedef struct EffBillParticleConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x44];
} EffBillParticleConfig; /* kind 3, 0x9C */

typedef struct EffBillAnimatedFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x18];
    u32 drawProgress;
    u8 unk74[0xC];
} EffBillAnimatedFrameConfig; /* kind 4, 0x80 */

typedef struct EffBillEmitterFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x30];
} EffBillEmitterFrameConfig; /* kind 5, 0x88 */

typedef struct EffBillStripFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x4C];
} EffBillStripFrameConfig; /* kind 6, 0xA4 */

typedef struct EffBillTrailFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x50];
} EffBillTrailFrameConfig; /* kind 7, 0xA8 */

typedef struct EffBillQuadFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x40];
} EffBillQuadFrameConfig; /* kind 8, 0x98 */

typedef struct EffBillPointConfig {
    EffBillTimedHeader timed;
    s32 layers;
    u8 pad40[0x1C];
    u8 drawFlag;
    u8 pad5D[0xB];
    f32 rangeFadeInEnd;
    f32 rangeFadeOutStart;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    u8 pad7C[8];
    f32 unk84;
} EffBillPointConfig;

typedef struct EffBillRangeConfig {
    EffBillPointConfig point;
    u8 pad88[4];
    f32 startBase;
    f32 startRand;
    f32 endBase;
    f32 endRand;
    u8 pad9C[0x10];
} EffBillRangeConfig;

/* DDS2 resource-op rows at 003E9DF4/003E9E10/003E9E2C copy
 * 0xF8/0xF4/0xD8 bytes for vortex/column/spiral. The row at 003E9E7C
 * copies 0x10C bytes for the animated flame. Kind is local to its operation
 * table; EffClassWork.kind together with that table selects the record. */
typedef struct EffBillEmitterHeader {
    EffBillTimedHeader timed;
    SdfColorTrack emitterColorTrack;
    SdfAlphaTrack emitterAlphaTrack;
    u8 pad70[8];
    f32 fadeIn;
    f32 fadeOut;
    u32 life;
    u32 spawnRate;
    u8 respawn;
    u8 pad89[3];
    u32 segments;
} EffBillEmitterHeader;

typedef struct EffBillEmitterCommon {
    EffBillEmitterHeader header;
    f32 rowRadius;
    u8 pad94[4];
    f32 height;
    f32 heightJitter;
    u8 padA0[4];
    f32 rise;
    /* Angular sweep for vortex/spiral; longitudinal extent for column. */
    f32 extent;
    f32 extentJitter;
    f32 width;
    f32 widthJitter;
    u8 burst;
    u8 drawMode;
    u8 padBA[2];
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
} EffBillEmitterCommon;

typedef struct EffBillVortexConfig {
    EffBillEmitterCommon common;
    f32 climbStart;
    f32 climbStartJitter;
    f32 climbEnd;
    f32 climbEndJitter;
    u8 padDC[4];
    f32 flareJitter;
    f32 flareCenter;
    f32 flare;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillVortexConfig;

typedef struct EffBillColumnConfig {
    EffBillEmitterCommon common;
    f32 velocity;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
    f32 drag;
    f32 dragJitter;
    f32 gravity;
    f32 flareJitter;
    f32 flareCenter;
    f32 flare;
} EffBillColumnConfig;

typedef struct EffBillSpiralConfig {
    EffBillEmitterCommon common;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillSpiralConfig;

typedef struct EffBillFlameConfig {
    EffBillEmitterHeader header;
    EffScalarCurve scaleCurve;
    u8 padB4[5];
    u8 drawMode;
    u8 padBA[2];
    f32 spreadA;
    f32 spreadB;
    f32 driftA;
    f32 driftB;
    f32 length;
    f32 lengthJitter;
    f32 width;
    f32 widthJitter;
    u8 burst;
    u8 meshMode;
    u8 padDE[2];
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 velocity;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
    f32 drag;
    f32 dragJitter;
    f32 gravity;
} EffBillFlameConfig;

typedef struct EffBillRadialConfig {
    EffBillPointConfig point;
    f32 radius;
} EffBillRadialConfig;

/* Animation resource kind 4 (DDS2 003E9ED0) copies this 0x98 record.
 * Samples are consumed as an unsigned allocation count and a signed row bound. */
typedef struct EffBillQuantizedConfig {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    EffScalarCurve scaleCurve;
    u8 pad58[0x10];
    f32 alphaFadeInFraction;
    f32 alphaFadeOutFraction;
    u32 drawProgress;
    union {
        u32 quantizedSamples;
        s32 signedRows;
    } samples;
    f32 fadeInEnd;
    f32 fadeOutStart;
    f32 uvARowScale;
    f32 uvBVertexScale;
    f32 rowOffset;
    f32 radius;
    f32 angularSpanDegrees;
    u8 meshMode;
    u8 pad95[3];
} EffBillQuantizedConfig;

/* Other class records selected by DDS2 003E9B80 / DDS1 0037EAD0.
 * Their count/time header and output byte are recovered; tails remain unknown. */
typedef struct EffBillClass1Config {
    EffBillOutputHeader output;
    u8 unk40[0x2C];
} EffBillClass1Config;

typedef struct EffBillClass2Config {
    EffBillOutputHeader output;
    u8 unk40[0x90];
} EffBillClass2Config;

typedef struct EffBillClass3Config {
    EffBillOutputHeader output;
    u8 unk40[0x28];
} EffBillClass3Config;

typedef union EffBillConfig {
    EffBillFrameConfig frame;
    EffBillCellConfig cell;
    EffBillParticleConfig particle;
    EffBillAnimatedFrameConfig animatedFrame;
    EffBillEmitterFrameConfig emitterFrame;
    EffBillStripFrameConfig stripFrame;
    EffBillTrailFrameConfig trailFrame;
    EffBillQuadFrameConfig quadFrame;
    EffBillClass1Config class1;
    EffBillClass2Config class2;
    EffBillClass3Config class3;
    EffBillPointConfig point;
    EffBillRadialConfig radial;
    EffBillRangeConfig range;
    EffBillVortexConfig vortex;
    EffBillColumnConfig column;
    EffBillSpiralConfig spiral;
    EffBillFlameConfig flame;
    EffBillQuantizedConfig quantized;
} EffBillConfig;

struct EffPointSet;
typedef struct EffScaleRangeEntry {
    struct EffPointSet *set;
    u8 pad04[0x10];
    s32 negativeSeed;
    u32 color;
    u8 pad1C[0x14];
} EffScaleRangeEntry;

typedef char EffClassWorkSizeCheck[(sizeof(EffClassWork) == 0x40) ? 1 : -1];
typedef char EffClassWorkFrameOffsetCheck[((u32)&((EffClassWork *)0)->frame == 0x28) ? 1 : -1];
typedef char EffBillTimedHeaderSizeCheck[(sizeof(EffBillTimedHeader) == 0x3C) ? 1 : -1];
typedef char EffBillTimeOffsetCheck[((u32)&((EffBillTimedHeader *)0)->time == 0x34) ? 1 : -1];
typedef char EffBillCountOffsetCheck[((u32)&((EffBillTimedHeader *)0)->count == 0x38) ? 1 : -1];
typedef char EffBillOutputHeaderSizeCheck[(sizeof(EffBillOutputHeader) == 0x40) ? 1 : -1];
typedef char EffBillFrameHeaderSizeCheck[(sizeof(EffBillFrameHeader) == 0x58) ? 1 : -1];
typedef char EffBillFrameConfigSizeCheck[(sizeof(EffBillFrameConfig) == 0x80) ? 1 : -1];
typedef char EffBillCellConfigSizeCheck[(sizeof(EffBillCellConfig) == 0x8C) ? 1 : -1];
typedef char EffBillParticleConfigSizeCheck[(sizeof(EffBillParticleConfig) == 0x9C) ? 1 : -1];
typedef char EffBillAnimatedFrameConfigSizeCheck[(sizeof(EffBillAnimatedFrameConfig) == 0x80) ? 1 : -1];
typedef char EffBillEmitterFrameConfigSizeCheck[(sizeof(EffBillEmitterFrameConfig) == 0x88) ? 1 : -1];
typedef char EffBillStripFrameConfigSizeCheck[(sizeof(EffBillStripFrameConfig) == 0xA4) ? 1 : -1];
typedef char EffBillTrailFrameConfigSizeCheck[(sizeof(EffBillTrailFrameConfig) == 0xA8) ? 1 : -1];
typedef char EffBillQuadFrameConfigSizeCheck[(sizeof(EffBillQuadFrameConfig) == 0x98) ? 1 : -1];
typedef char EffBillClass1ConfigSizeCheck[(sizeof(EffBillClass1Config) == 0x6C) ? 1 : -1];
typedef char EffBillClass2ConfigSizeCheck[(sizeof(EffBillClass2Config) == 0xD0) ? 1 : -1];
typedef char EffBillClass3ConfigSizeCheck[(sizeof(EffBillClass3Config) == 0x68) ? 1 : -1];
typedef char EffBillQuantizedConfigSizeCheck[(sizeof(EffBillQuantizedConfig) == 0x98) ? 1 : -1];
typedef char EffBillPointConfigSizeCheck[(sizeof(EffBillPointConfig) == 0x88) ? 1 : -1];
typedef char EffBillPointFlagOffsetCheck[((u32)&((EffBillPointConfig *)0)->drawFlag == 0x5C) ? 1 : -1];
typedef char EffBillRangeConfigSizeCheck[(sizeof(EffBillRangeConfig) == 0xAC) ? 1 : -1];
typedef char EffBillEmitterHeaderSizeCheck[(sizeof(EffBillEmitterHeader) == 0x90) ? 1 : -1];
typedef char EffBillEmitterCommonSizeCheck[(sizeof(EffBillEmitterCommon) == 0xCC) ? 1 : -1];
typedef char EffBillVortexConfigSizeCheck[(sizeof(EffBillVortexConfig) == 0xF8) ? 1 : -1];
typedef char EffBillColumnConfigSizeCheck[(sizeof(EffBillColumnConfig) == 0xF4) ? 1 : -1];
typedef char EffBillSpiralConfigSizeCheck[(sizeof(EffBillSpiralConfig) == 0xD8) ? 1 : -1];
typedef char EffBillFlameConfigSizeCheck[(sizeof(EffBillFlameConfig) == 0x10C) ? 1 : -1];
typedef char EffBillRadialConfigSizeCheck[(sizeof(EffBillRadialConfig) == 0x8C) ? 1 : -1];
typedef char EffBillVortexSpinOffsetCheck[((u32)&((EffBillVortexConfig *)0)->spin == 0xEC) ? 1 : -1];
typedef char EffBillFlameGravityOffsetCheck[((u32)&((EffBillFlameConfig *)0)->gravity == 0x108) ? 1 : -1];
typedef char EffBillConfigSizeCheck[(sizeof(EffBillConfig) == 0x10C) ? 1 : -1];
typedef char EffScaleRangeEntrySizeCheck[(sizeof(EffScaleRangeEntry) == 0x30) ? 1 : -1];

#endif
