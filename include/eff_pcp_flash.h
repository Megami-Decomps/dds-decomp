#ifndef EFF_PCP_FLASH_H
#define EFF_PCP_FLASH_H

#include "eff.h"

/* Copied input prefixes for the larger, title-local flash work records. */
typedef struct PcpFlashScalingOrbitParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
} PcpFlashScalingOrbitParams;

typedef struct PcpFlashAccumulatingParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 spanBase;
    f32 unk34;
    f32 orbitRadius;
    f32 radialHalfThicknessBase;
    f32 tilt;
    f32 increment;
    f32 radialStep;
    u32 drawMode;
} PcpFlashAccumulatingParams;

typedef struct PcpFlashOrbitArcParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpanStart;
    f32 acrossSpanStart;
    f32 orbitRadiusStart;
    f32 orbitRadiusEnd;
    f32 normalSpanStart;
    f32 normalSpanEnd;
    f32 heightStart;
    f32 heightEnd;
    f32 angularStep;
    u32 drawMode;
} PcpFlashOrbitArcParams;

typedef struct PcpFlashFadingOrbitParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
} PcpFlashFadingOrbitParams;

typedef struct PcpFlashRotatingQuadParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u8 pad24[4];
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 angularStepRange;
    u32 drawMode;
} PcpFlashRotatingQuadParams;

typedef struct PcpFlashRadialTriangleParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
} PcpFlashRadialTriangleParams;

typedef struct PcpFlashRadialStripParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 spanBase;
    f32 spanVariation;
    f32 maxThickness;
    f32 angularStepRange;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
    u8 pad50[0x80];
} PcpFlashRadialStripParams;

typedef struct PcpFlashOffsetRadialParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    f32 originOffset;
    u32 drawMode;
} PcpFlashOffsetRadialParams;

/* The rotating-streak factory copies this 0x40-byte parameter prefix into
 * its larger work allocation before appending particle and runtime state. */
typedef struct PcpFlashStreakParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 rotationStepRange;
    u32 drawMode;
} PcpFlashStreakParams;

/* Paired Flash runtime owners and their variable particle records.
 * Factory allocations place each particle array immediately after its owner. */
/* Three consecutive packed colors returned by effGetGroupIndexRecord. */
typedef struct PcpFlashColorSlot {
    s32 first;
    s32 second;
    s32 third;
} PcpFlashColorSlot;

typedef struct PcpFlashStreakWork PcpFlashStreakWork;

typedef struct PcpFlashRotatingParticle PcpFlashRotatingParticle;

/* Spawn, rotation and draw passes share this 0x58-byte streak work.
   The position-rotation routine uses the same parts pointer at 0x40. */
struct PcpFlashStreakWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 rotationStepRange;
    u32 drawMode;
    PcpFlashRotatingParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashOrbitParticle PcpFlashOrbitParticle;

struct PcpFlashOrbitParticle {
    u32 color;
    s32 age;
    f32 scale;
    f32 initialScale;
    f32 angle;
};

typedef struct PcpFlashScalingOrbitWork PcpFlashScalingOrbitWork;

struct PcpFlashScalingOrbitWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
    PcpFlashOrbitParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashAccumulatingParticle PcpFlashAccumulatingParticle;

struct PcpFlashAccumulatingParticle {
    u32 color;
    s32 age;
    f32 radialHalfThickness;
    f32 span;
    f32 unk10;
    f32 radius;
    f32 orbitAngle;
};

typedef struct PcpFlashAccumulatingWork PcpFlashAccumulatingWork;

struct PcpFlashAccumulatingWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 spanBase;
    f32 unk34;
    f32 orbitRadius;
    f32 radialHalfThicknessBase;
    f32 tilt;
    f32 increment;
    f32 radialStep;
    u32 drawMode;
    PcpFlashAccumulatingParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

/* Shared 0x10-byte motion state: accumulator is an angle for orbit arcs and
   a radius for radial triangles; stepSpeed is radial speed or arc height. */
typedef struct PcpFlashMotionParticle PcpFlashMotionParticle;

struct PcpFlashMotionParticle {
    u32 color;
    s32 age;
    f32 accumulator;
    f32 stepSpeed; /* 0x0C: multiplied by decay each step, then added to
                       * accumulator; also read as the particle's height */
};

typedef struct PcpFlashOrbitArcWork PcpFlashOrbitArcWork;

/* The orbit constructor and renderer share this 0x80-byte work record.
   The 0x58-byte copied parameters are followed by the motion-particle array
   pointer and draw/resource state. */
struct PcpFlashOrbitArcWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpanStart;
    f32 acrossSpanStart;
    f32 orbitRadiusStart;
    f32 orbitRadiusEnd;
    f32 normalSpanStart;
    f32 normalSpanEnd;
    f32 heightStart;
    f32 heightEnd;
    f32 angularStep;
    u32 drawMode;
    PcpFlashMotionParticle *parts;
    u32 shapeProgressFrame;
    u32 tintColor;
    f32 renderScale;
    f32 orbitRadius;
    f32 normalSpan;
    f32 upSpan;
    f32 acrossSpan;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashRotatingQuadParticle PcpFlashRotatingQuadParticle;

struct PcpFlashRotatingQuadParticle {
    u32 color;
    s32 age;
    f32 angularStep;
    f32 scale;
    f32 angle;
    f32 upSpan;
    f32 acrossSpan;
    f32 initialScale;
};

typedef struct PcpFlashRotatingQuadWork PcpFlashRotatingQuadWork;

struct PcpFlashRotatingQuadWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u8 pad24[0x04];
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 angularStepRange;
    u32 drawMode;
    PcpFlashRotatingQuadParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashRadialTriangleWork PcpFlashRadialTriangleWork;

struct PcpFlashRadialTriangleWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
    PcpFlashMotionParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashRadialStripParticle PcpFlashRadialStripParticle;

struct PcpFlashRadialStripParticle {
    u32 color;
    s32 age;
    f32 angularStep;
    f32 thickness;
    f32 radius;
    f32 radialSpeed;
    f32 angle;
    f32 span;
};

typedef struct PcpFlashRadialStripWork PcpFlashRadialStripWork;

struct PcpFlashRadialStripWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 spanBase;
    f32 spanVariation;
    f32 maxThickness;
    f32 angularStepRange;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
    u8 pad50[0x80];
    PcpFlashRadialStripParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashFadingOrbitWork PcpFlashFadingOrbitWork;

struct PcpFlashFadingOrbitWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
    PcpFlashOrbitParticle *parts;
    u32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

typedef struct PcpFlashOffsetRadialWork PcpFlashOffsetRadialWork;

struct PcpFlashOffsetRadialWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    f32 originOffset;
    u32 drawMode;
    PcpFlashMotionParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
};

struct PcpFlashRotatingParticle {
    u32 color;
    s32 age;
    f32 rotationStep;
    f32 scale;
    f32 position[3];
    f32 unk1C;
    f32 upSpan;
    f32 acrossSpan;
    f32 initialScale;
};


typedef struct PcpFlashQuadColorSlot {
    s32 color[5];
} PcpFlashQuadColorSlot;

/* The orbit-arc particles follow the same work header used by its draw and
   angle-update passes; this is an allocation container, not a second view. */
typedef struct PcpFlashOrbitArcBlock {
    PcpFlashOrbitArcWork header;
    PcpFlashMotionParticle parts[1];
} PcpFlashOrbitArcBlock;


/* The factory copies this 0x30-byte parameter prefix into its larger work
 * allocation before appending particle and resource state. */
typedef struct PcpFlashTrianglePulseParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 colorA;
    u32 colorB;
    f32 maxScale;
    u32 drawMode;
} PcpFlashTrianglePulseParams;

PcpFlashTrianglePulseWork *effFlashRecordCreate(PcpFlashTrianglePulseParams *params);

typedef char PcpFlashScalingOrbitParamsSizeCheck[
    sizeof(PcpFlashScalingOrbitParams) == 0x48 ? 1 : -1];
typedef char PcpFlashAccumulatingParamsSizeCheck[
    sizeof(PcpFlashAccumulatingParams) == 0x50 ? 1 : -1];
typedef char PcpFlashOrbitArcParamsSizeCheck[
    sizeof(PcpFlashOrbitArcParams) == 0x58 ? 1 : -1];
typedef char PcpFlashFadingOrbitParamsSizeCheck[
    sizeof(PcpFlashFadingOrbitParams) == 0x50 ? 1 : -1];
typedef char PcpFlashRotatingQuadParamsSizeCheck[
    sizeof(PcpFlashRotatingQuadParams) == 0x4C ? 1 : -1];
typedef char PcpFlashRadialTriangleParamsSizeCheck[
    sizeof(PcpFlashRadialTriangleParams) == 0x3C ? 1 : -1];
typedef char PcpFlashRadialStripParamsSizeCheck[
    sizeof(PcpFlashRadialStripParams) == 0xD0 ? 1 : -1];
typedef char PcpFlashOffsetRadialParamsSizeCheck[
    sizeof(PcpFlashOffsetRadialParams) == 0x40 ? 1 : -1];
typedef char PcpFlashStreakParamsSizeCheck[
    sizeof(PcpFlashStreakParams) == 0x40 ? 1 : -1];
typedef char PcpFlashStreakParamsParticleCountOffsetCheck[
    ((u32)&((PcpFlashStreakParams *)0)->particleCount) == 0x10 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsSizeCheck[
    sizeof(PcpFlashTrianglePulseParams) == 0x30 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsCountOffsetCheck[
    ((u32)&((PcpFlashTrianglePulseParams *)0)->particleCount) == 0x10 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashTrianglePulseParams *)0)->drawMode) == 0x2C ? 1 : -1];

/* Native constructors copy each field unchanged into EffRecordPool.drawMode. */
typedef char PcpFlashStreakParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashStreakParams *)0)->drawMode) == 0x3C ? 1 : -1];
typedef char PcpFlashScalingOrbitParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashScalingOrbitParams *)0)->drawMode) == 0x44 ? 1 : -1];
typedef char PcpFlashAccumulatingParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashAccumulatingParams *)0)->drawMode) == 0x4C ? 1 : -1];
typedef char PcpFlashOrbitArcParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashOrbitArcParams *)0)->drawMode) == 0x54 ? 1 : -1];
typedef char PcpFlashRotatingQuadParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashRotatingQuadParams *)0)->drawMode) == 0x48 ? 1 : -1];
typedef char PcpFlashRadialTriangleParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashRadialTriangleParams *)0)->drawMode) == 0x38 ? 1 : -1];
typedef char PcpFlashRadialStripParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashRadialStripParams *)0)->drawMode) == 0x4C ? 1 : -1];
typedef char PcpFlashFadingOrbitParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashFadingOrbitParams *)0)->drawMode) == 0x4C ? 1 : -1];
typedef char PcpFlashOffsetRadialParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashOffsetRadialParams *)0)->drawMode) == 0x3C ? 1 : -1];
typedef char PcpFlashStreakWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashStreakWork *)0)->drawMode) == 0x3C ? 1 : -1];
typedef char PcpFlashScalingOrbitWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashScalingOrbitWork *)0)->drawMode) == 0x44 ? 1 : -1];
typedef char PcpFlashAccumulatingWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashAccumulatingWork *)0)->drawMode) == 0x4C ? 1 : -1];
typedef char PcpFlashOrbitArcWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashOrbitArcWork *)0)->drawMode) == 0x54 ? 1 : -1];
typedef char PcpFlashRotatingQuadWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashRotatingQuadWork *)0)->drawMode) == 0x48 ? 1 : -1];
typedef char PcpFlashRadialTriangleWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashRadialTriangleWork *)0)->drawMode) == 0x38 ? 1 : -1];
typedef char PcpFlashRadialStripWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashRadialStripWork *)0)->drawMode) == 0x4C ? 1 : -1];
typedef char PcpFlashFadingOrbitWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashFadingOrbitWork *)0)->drawMode) == 0x4C ? 1 : -1];
typedef char PcpFlashOffsetRadialWorkDrawModeOffsetCheck[
    ((u32)&((PcpFlashOffsetRadialWork *)0)->drawMode) == 0x3C ? 1 : -1];

typedef char PcpFlashColorSlotSizeCheck[sizeof(PcpFlashColorSlot) == 0x0C ? 1 : -1];
typedef char PcpFlashQuadColorSlotSizeCheck[sizeof(PcpFlashQuadColorSlot) == 0x14 ? 1 : -1];
typedef char PcpFlashRotatingParticleSizeCheck[sizeof(PcpFlashRotatingParticle) == 0x2C ? 1 : -1];
typedef char PcpFlashOrbitParticleSizeCheck[sizeof(PcpFlashOrbitParticle) == 0x14 ? 1 : -1];
typedef char PcpFlashAccumulatingParticleSizeCheck[sizeof(PcpFlashAccumulatingParticle) == 0x1C ? 1 : -1];
typedef char PcpFlashMotionParticleSizeCheck[sizeof(PcpFlashMotionParticle) == 0x10 ? 1 : -1];
typedef char PcpFlashRotatingQuadParticleSizeCheck[sizeof(PcpFlashRotatingQuadParticle) == 0x20 ? 1 : -1];
typedef char PcpFlashRadialStripParticleSizeCheck[sizeof(PcpFlashRadialStripParticle) == 0x20 ? 1 : -1];

typedef char PcpFlashStreakWorkSizeCheck[sizeof(PcpFlashStreakWork) == 0x58 ? 1 : -1];
typedef char PcpFlashStreakWorkPartsOffsetCheck[
    ((u32)&((PcpFlashStreakWork *)0)->parts) == 0x40 ? 1 : -1];
typedef char PcpFlashStreakWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashStreakWork *)0)->allocationHandle) == 0x50 ? 1 : -1];
typedef char PcpFlashStreakWorkResourceOffsetCheck[
    ((u32)&((PcpFlashStreakWork *)0)->resourceHandle) == 0x54 ? 1 : -1];

typedef char PcpFlashScalingOrbitWorkSizeCheck[sizeof(PcpFlashScalingOrbitWork) == 0x60 ? 1 : -1];
typedef char PcpFlashScalingOrbitWorkPartsOffsetCheck[
    ((u32)&((PcpFlashScalingOrbitWork *)0)->parts) == 0x48 ? 1 : -1];
typedef char PcpFlashScalingOrbitWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashScalingOrbitWork *)0)->allocationHandle) == 0x58 ? 1 : -1];
typedef char PcpFlashScalingOrbitWorkResourceOffsetCheck[
    ((u32)&((PcpFlashScalingOrbitWork *)0)->resourceHandle) == 0x5C ? 1 : -1];

typedef char PcpFlashAccumulatingWorkSizeCheck[sizeof(PcpFlashAccumulatingWork) == 0x68 ? 1 : -1];
typedef char PcpFlashAccumulatingWorkPartsOffsetCheck[
    ((u32)&((PcpFlashAccumulatingWork *)0)->parts) == 0x50 ? 1 : -1];
typedef char PcpFlashAccumulatingWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashAccumulatingWork *)0)->allocationHandle) == 0x60 ? 1 : -1];
typedef char PcpFlashAccumulatingWorkResourceOffsetCheck[
    ((u32)&((PcpFlashAccumulatingWork *)0)->resourceHandle) == 0x64 ? 1 : -1];

typedef char PcpFlashOrbitArcWorkSizeCheck[sizeof(PcpFlashOrbitArcWork) == 0x80 ? 1 : -1];
typedef char PcpFlashOrbitArcWorkPartsOffsetCheck[
    ((u32)&((PcpFlashOrbitArcWork *)0)->parts) == 0x58 ? 1 : -1];
typedef char PcpFlashOrbitArcWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashOrbitArcWork *)0)->allocationHandle) == 0x78 ? 1 : -1];
typedef char PcpFlashOrbitArcWorkResourceOffsetCheck[
    ((u32)&((PcpFlashOrbitArcWork *)0)->resourceHandle) == 0x7C ? 1 : -1];
typedef char PcpFlashOrbitArcBlockSizeCheck[sizeof(PcpFlashOrbitArcBlock) == 0x90 ? 1 : -1];

typedef char PcpFlashRotatingQuadWorkSizeCheck[sizeof(PcpFlashRotatingQuadWork) == 0x64 ? 1 : -1];
typedef char PcpFlashRotatingQuadWorkPartsOffsetCheck[
    ((u32)&((PcpFlashRotatingQuadWork *)0)->parts) == 0x4C ? 1 : -1];
typedef char PcpFlashRotatingQuadWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashRotatingQuadWork *)0)->allocationHandle) == 0x5C ? 1 : -1];
typedef char PcpFlashRotatingQuadWorkResourceOffsetCheck[
    ((u32)&((PcpFlashRotatingQuadWork *)0)->resourceHandle) == 0x60 ? 1 : -1];

typedef char PcpFlashRadialTriangleWorkSizeCheck[sizeof(PcpFlashRadialTriangleWork) == 0x54 ? 1 : -1];
typedef char PcpFlashRadialTriangleWorkPartsOffsetCheck[
    ((u32)&((PcpFlashRadialTriangleWork *)0)->parts) == 0x3C ? 1 : -1];
typedef char PcpFlashRadialTriangleWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashRadialTriangleWork *)0)->allocationHandle) == 0x4C ? 1 : -1];
typedef char PcpFlashRadialTriangleWorkResourceOffsetCheck[
    ((u32)&((PcpFlashRadialTriangleWork *)0)->resourceHandle) == 0x50 ? 1 : -1];

typedef char PcpFlashRadialStripWorkSizeCheck[sizeof(PcpFlashRadialStripWork) == 0xE8 ? 1 : -1];
typedef char PcpFlashRadialStripWorkPartsOffsetCheck[
    ((u32)&((PcpFlashRadialStripWork *)0)->parts) == 0xD0 ? 1 : -1];
typedef char PcpFlashRadialStripWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashRadialStripWork *)0)->allocationHandle) == 0xE0 ? 1 : -1];
typedef char PcpFlashRadialStripWorkResourceOffsetCheck[
    ((u32)&((PcpFlashRadialStripWork *)0)->resourceHandle) == 0xE4 ? 1 : -1];

typedef char PcpFlashFadingOrbitWorkSizeCheck[sizeof(PcpFlashFadingOrbitWork) == 0x68 ? 1 : -1];
typedef char PcpFlashFadingOrbitWorkPartsOffsetCheck[
    ((u32)&((PcpFlashFadingOrbitWork *)0)->parts) == 0x50 ? 1 : -1];
typedef char PcpFlashFadingOrbitWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashFadingOrbitWork *)0)->allocationHandle) == 0x60 ? 1 : -1];
typedef char PcpFlashFadingOrbitWorkResourceOffsetCheck[
    ((u32)&((PcpFlashFadingOrbitWork *)0)->resourceHandle) == 0x64 ? 1 : -1];

typedef char PcpFlashOffsetRadialWorkSizeCheck[sizeof(PcpFlashOffsetRadialWork) == 0x58 ? 1 : -1];
typedef char PcpFlashOffsetRadialWorkPartsOffsetCheck[
    ((u32)&((PcpFlashOffsetRadialWork *)0)->parts) == 0x40 ? 1 : -1];
typedef char PcpFlashOffsetRadialWorkAllocationOffsetCheck[
    ((u32)&((PcpFlashOffsetRadialWork *)0)->allocationHandle) == 0x50 ? 1 : -1];
typedef char PcpFlashOffsetRadialWorkResourceOffsetCheck[
    ((u32)&((PcpFlashOffsetRadialWork *)0)->resourceHandle) == 0x54 ? 1 : -1];

#endif
