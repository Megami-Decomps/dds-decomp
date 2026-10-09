#ifndef EFF_BILL_H
#define EFF_BILL_H

#include "common.h"
#include "eff_expanded_list.h"
#include "sdf.h"

#include "eff_curve.h"

struct BillObj;

/* Lens-flare constructors copy 0x40 bytes into a 0x58-byte owner at +0x18
 * (DDS1 0029C620 / DDS2 002DE338). Draw callbacks read strength with LWC1,
 * compare frame/limit signed, and select the flare set with LBU at +0x54. */
typedef struct EffLensFlareParams {
    SdfColorTrack colorCurve;
    SdfAlphaTrack alphaCurve;
    f32 strength;
    s32 limit;
    u8 flareSet;
    u8 pad3D[3];
} EffLensFlareParams;

typedef struct EffFadeVectorWork {
    f32 vector[4];
    s32 frame;
    u32 color;
    EffLensFlareParams source;
} EffFadeVectorWork;

typedef char EffLensFlareParamsSizeCheck[(sizeof(EffLensFlareParams) == 0x40) ? 1 : -1];
typedef char EffLensFlareStrengthOffsetCheck[((u32)&((EffLensFlareParams *)0)->strength == 0x34) ? 1 : -1];
typedef char EffLensFlareLimitOffsetCheck[((u32)&((EffLensFlareParams *)0)->limit == 0x38) ? 1 : -1];
typedef char EffLensFlareSetOffsetCheck[((u32)&((EffLensFlareParams *)0)->flareSet == 0x3C) ? 1 : -1];
typedef char EffFadeVectorWorkSizeCheck[(sizeof(EffFadeVectorWork) == 0x58) ? 1 : -1];
typedef char EffFadeVectorSourceOffsetCheck[((u32)&((EffFadeVectorWork *)0)->source == 0x18) ? 1 : -1];

/* DDS1 002AAF70 / DDS2 002EE348 allocate this 0x3C-byte strip owner.
 * File constructors copy a 0x20-byte source header into its +0x0C member. */
typedef struct EffectStripNode {
    u32 percent;
    u32 color;
    f32 opacity;
    u8 copiedHeader[0x20];
    u32 transform;
    struct BillObj *resource;
    u32 active;
    u16 count;
} EffectStripNode;

typedef char EffectStripNodeSizeCheck[(sizeof(EffectStripNode) == 0x3C) ? 1 : -1];
typedef char EffectStripHeaderOffsetCheck[((u32)&((EffectStripNode *)0)->copiedHeader == 0x0C) ? 1 : -1];
typedef char EffectStripRecordOffsetCheck[((u32)&((EffectStripNode *)0)->active == 0x34) ? 1 : -1];
typedef char EffectStripCountOffsetCheck[((u32)&((EffectStripNode *)0)->count == 0x38) ? 1 : -1];

/* DDS1 002A7B68 / DDS2 002EA120 allocate 0xD4 bytes and copy this
 * 0x98-byte parameter record at +0x30. Draws 002A8020 / 002EA5D8
 * scale the +0x64 track's result and use the +0x90 track as the angle. */
typedef struct EffQuadParams {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    EffScalarTrack sizeTrack;
    EffScalarTrack angleTrack;
    s32 duration;
    f32 sizeScale;
    u8 noSetup;
    u8 pad95[3];
} EffQuadParams;

typedef struct EffQuadWork {
    f32 position[4];
    f32 orientation[4];
    f32 scale;
    u32 color;
    u32 sourceKind;
    s32 frame;
    EffQuadParams source;
    struct BillObj *billHandle;
    struct EffExpandedList *reference; /* Kind-7 retained resource wrapper. */
    u32 assetHandle;
} EffQuadWork;

typedef char EffQuadParamsSizeCheck[(sizeof(EffQuadParams) == 0x98) ? 1 : -1];
typedef char EffQuadWorkSizeCheck[(sizeof(EffQuadWork) == 0xD4) ? 1 : -1];
typedef char EffQuadSourceOffsetCheck[((u32)&((EffQuadWork *)0)->source == 0x30) ? 1 : -1];
typedef char EffQuadSizeTrackOffsetCheck[((u32)&((EffQuadWork *)0)->source.sizeTrack == 0x64) ? 1 : -1];
typedef char EffQuadAngleTrackOffsetCheck[((u32)&((EffQuadWork *)0)->source.angleTrack == 0x90) ? 1 : -1];
typedef char EffQuadBillOffsetCheck[((u32)&((EffQuadWork *)0)->billHandle == 0xC8) ? 1 : -1];
typedef char EffQuadReferenceOffsetCheck[((u32)&((EffQuadWork *)0)->reference == 0xCC) ? 1 : -1];
typedef char EffQuadAssetOffsetCheck[((u32)&((EffQuadWork *)0)->assetHandle == 0xD0) ? 1 : -1];

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
    u8 unk40[4];
    f32 fadeInFraction;
    f32 fadeOutFraction;
    s32 lifetime;
    s32 spawnCount;
    u8 respawn;
    u8 spawnAllOnStart;
    u8 mode;
    u8 pad57;
} EffBillFrameHeader;

/* DDS2 003E9950 / DDS1 0037E8A0 resource kinds 1..8 copy the following
 * record lengths. Unrecovered bytes remain serialized data. */
typedef struct EffBillFrameConfig {
    EffBillFrameHeader frame;
    u8 unk58[0x28];
} EffBillFrameConfig; /* kind 1, 0x80 */

typedef struct EffBillCellConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 extent;
    u8 unk64[4];
    f32 extentJitter;
    f32 radius;
    f32 radiusJitter;
    f32 centerOffset;
    f32 centerOffsetJitter;
    f32 velocity;
    f32 velocityJitter;
    f32 acceleration;
    u8 reverse;
    u8 pad89[3];
} EffBillCellConfig; /* kind 2, 0x8C */

struct RefObj;

/* Track-set allocations end with this 0x30-byte header. Kind selects the
 * geometry and optional column strides; tail follows the geometry buffer. */
typedef struct EffTrackSet {
    u32 type;
    u32 color;
    s32 rows;
    u16 kind;
    u8 pad0E[2];
    s32 count;
    u8 flag;
    u8 pad15[3];
    struct RefObj *shared;
    u8 *buffer;
    u8 *columns;
    u8 *tail;
    SdfAsset *handle;
    struct SdfMemBlock *allocation;
} EffTrackSet;

typedef char EffTrackSet_size_must_be_0x30[(sizeof(EffTrackSet) == 0x30) ? 1 : -1];
typedef char EffTrackSet_handle_offset_must_be_0x28[
    ((u32)&((EffTrackSet *)0)->handle == 0x28) ? 1 : -1];
typedef char EffTrackSet_allocation_offset_must_be_0x2C[
    ((u32)&((EffTrackSet *)0)->allocation == 0x2C) ? 1 : -1];

/* Resource kinds 1..8 allocate sixteen bytes before their entry array. */
typedef struct EffBillFrameState {
    u8 *entries;
    EffTrackSet *asset;
    struct SdfMemBlock *allocation;
    u32 unk0C;
} EffBillFrameState;

typedef struct EffBillCellEntry {
    s32 age;
    f32 velocity;
    f32 angle;
    f32 radiusA;
    f32 radiusB;
    f32 centerOffset;
    f32 extent;
} EffBillCellEntry;

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

/* Kind-8 quad updates (DDS1 002A2E18 / DDS2 002E4E80) read these
 * parameters from the copied 0x98-byte record and advance 0x20-byte entries. */
typedef struct EffBillQuadFrameConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    f32 heightStart;
    f32 heightStartJitter;
    f32 heightEnd;
    f32 heightEndJitter;
    f32 tilt;
    f32 tiltJitter;
    f32 velocity;
    f32 velocityJitter;
    f32 acceleration;
    u8 reverse;
    u8 pad95[3];
} EffBillQuadFrameConfig; /* kind 8, 0x98 */

typedef struct EffBillQuadEntry {
    s32 timer;
    f32 velocity;
    f32 angle;
    f32 radius;
    f32 height;
    f32 heightStep;
    f32 tilt;
    f32 width;
} EffBillQuadEntry;

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
typedef char EffBillQuadEntrySizeCheck[(sizeof(EffBillQuadEntry) == 0x20) ? 1 : -1];
typedef char EffBillQuadReverseOffsetCheck[((u32)&((EffBillQuadFrameConfig *)0)->reverse == 0x94) ? 1 : -1];

#endif
