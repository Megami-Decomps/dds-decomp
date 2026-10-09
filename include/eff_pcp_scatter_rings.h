#ifndef EFF_PCP_SCATTER_RINGS_H
#define EFF_PCP_SCATTER_RINGS_H

#include "eff.h"
#include "eff_scatter_draw.h"
#include "sdf_resource.h"

/* Shared particle record used by B/C and the related copied-parameter ring. */
typedef struct PcpScatterParticle {
    f32 orientationAngle;
    f32 tiltAngle;
    s32 age;
    f32 rise;
    f32 angle;
    f32 tiltSpeed;
    f32 angleStep;
    f32 radius;
    f32 radiusStep;
    f32 heightOffset;
} PcpScatterParticle;

/* B's copied parameter block and runtime owner. Its particle pointer follows
 * the copied parameters, and the variable particle array follows the owner. */
typedef struct PcpScatterParamsB {
    f32 origin[4];
    f32 matrix[16];
    u32 unk50;
    u8 loop;
    u8 pad55[3];
    s32 duration;
    u32 particleCount;
    u32 unk60;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 riseStep;
    f32 tiltScale;
    f32 heightOffsetBase;
    f32 heightOffsetJitter;
    f32 initialRise;
    f32 riseDecay;
    u8 pad90[4];
    f32 initialTiltSpeed;
    f32 tiltDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radiusStepBase;
    f32 radiusStepJitter;
    f32 radiusDamping;
    s32 baseColor;
    u32 uSpan; /* Horizontal UV extent. */
    u32 vSpan; /* Vertical UV extent. */
    u8 padBC[0x80];
} PcpScatterParamsB;

typedef struct PcpScatterInstanceB {
    f32 matrix[16];
    PcpScatterParamsB params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
    s32 age;
    PcpScatterDraw *scatterObject;
    SdfMemBlock *allocationHandle;
} PcpScatterInstanceB;

/* C adds staggered ring motion and two color keys to the copied parameters. */
typedef struct PcpScatterParamsC {
    f32 origin[4];
    f32 matrix[16];
    u32 unk50;
    u8 loop;
    u8 pad55[3];
    s32 duration;
    u32 particleCount;
    u32 unk60;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 tiltScale;
    f32 riseRange;
    f32 radiusRamp;
    f32 heightOffsetBase;
    f32 heightOffsetJitter;
    f32 initialRise;
    f32 riseDecay;
    u8 pad94[4];
    f32 initialTiltSpeed;
    f32 tiltDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radiusStepBase;
    f32 radiusStepJitter;
    f32 radiusDamping;
    s32 startColor;
    s32 endColor;
    u32 uSpan; /* Horizontal UV extent; this variant reverses the legacy names. */
    u32 vSpan;
    u8 padC4[0x80];
} PcpScatterParamsC;

typedef struct PcpScatterInstanceC {
    f32 matrix[16];
    PcpScatterParamsC params;
    PcpScatterParticle *particles;
    f32 scale;
    u32 color;
    s32 age;
    PcpScatterDraw *scatterObject;
    SdfMemBlock *allocationHandle;
} PcpScatterInstanceC;

/* Plain is a distinct 0xE8-byte parameter and 0x28-byte particle variant. */
typedef struct PcpScatterPlainParams {
    f32 origin[4];
    u32 unk10;
    u8 loop;
    u8 pad15[3];
    s32 duration;
    u32 particleCount;
    u32 unk20;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 heightBase;
    f32 heightJitter;
    f32 angularSpeed;
    f32 angularDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 radialSpeed;
    f32 radialDamping;
    s32 radialDecayStart;
    s32 baseColor;
    u32 uSpan; /* Horizontal UV extent. */
    u32 vSpan; /* Vertical UV extent. */
    u8 pad68[0x80];
} PcpScatterPlainParams;

/* The flat particle has three rotation angles before its signed age; it is
 * distinct from the other variants' particle even though both records are 0x28. */
typedef struct PcpScatterPlainParticle {
    f32 rot[3];
    s32 age;
    f32 angle;
    f32 angularSpeed;
    f32 angleStep;
    f32 radius;
    f32 radialSpeed;
    f32 height;
} PcpScatterPlainParticle;

typedef struct PcpScatterPlainInstance {
    f32 matrix[16];
    PcpScatterPlainParams params;
    PcpScatterPlainParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterDraw *scatterObject;
    SdfMemBlock *allocationHandle;
} PcpScatterPlainInstance;

typedef char PcpScatterParamsB_size_must_be_0x13C[
    (sizeof(PcpScatterParamsB) == 0x13C) ? 1 : -1];
typedef char PcpScatterParticle_size_must_be_0x28[
    (sizeof(PcpScatterParticle) == 0x28) ? 1 : -1];
typedef char PcpScatterInstanceB_size_must_be_0x194[
    (sizeof(PcpScatterInstanceB) == 0x194) ? 1 : -1];
typedef char PcpScatterInstanceB_params_offset_must_be_0x40[
    ((u32)&((PcpScatterInstanceB *)0)->params == 0x40) ? 1 : -1];
typedef char PcpScatterInstanceB_particles_offset_must_be_0x17C[
    ((u32)&((PcpScatterInstanceB *)0)->particles == 0x17C) ? 1 : -1];
typedef char PcpScatterInstanceB_allocationHandle_offset_must_be_0x190[
    ((u32)&((PcpScatterInstanceB *)0)->allocationHandle == 0x190) ? 1 : -1];

typedef char PcpScatterParamsC_size_must_be_0x144[
    (sizeof(PcpScatterParamsC) == 0x144) ? 1 : -1];
typedef char PcpScatterInstanceC_size_must_be_0x19C[
    (sizeof(PcpScatterInstanceC) == 0x19C) ? 1 : -1];
typedef char PcpScatterInstanceC_params_offset_must_be_0x40[
    ((u32)&((PcpScatterInstanceC *)0)->params == 0x40) ? 1 : -1];
typedef char PcpScatterInstanceC_particles_offset_must_be_0x184[
    ((u32)&((PcpScatterInstanceC *)0)->particles == 0x184) ? 1 : -1];
typedef char PcpScatterInstanceC_allocationHandle_offset_must_be_0x198[
    ((u32)&((PcpScatterInstanceC *)0)->allocationHandle == 0x198) ? 1 : -1];

typedef char PcpScatterPlainParams_size_must_be_0xE8[
    (sizeof(PcpScatterPlainParams) == 0xE8) ? 1 : -1];
typedef char PcpScatterPlainParticle_size_must_be_0x28[
    (sizeof(PcpScatterPlainParticle) == 0x28) ? 1 : -1];
typedef char PcpScatterPlainInstance_size_must_be_0x13C[
    (sizeof(PcpScatterPlainInstance) == 0x13C) ? 1 : -1];
typedef char PcpScatterPlainInstance_params_offset_must_be_0x40[
    ((u32)&((PcpScatterPlainInstance *)0)->params == 0x40) ? 1 : -1];
typedef char PcpScatterPlainInstance_particles_offset_must_be_0x128[
    ((u32)&((PcpScatterPlainInstance *)0)->particles == 0x128) ? 1 : -1];
typedef char PcpScatterPlainInstance_allocationHandle_offset_must_be_0x138[
    ((u32)&((PcpScatterPlainInstance *)0)->allocationHandle == 0x138) ? 1 : -1];

#endif
