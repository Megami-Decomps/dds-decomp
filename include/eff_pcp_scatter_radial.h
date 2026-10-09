#ifndef EFF_PCP_SCATTER_RADIAL_H
#define EFF_PCP_SCATTER_RADIAL_H

#include "eff.h"
#include "eff_param.h"

/* Serialized radial scatter parameters copied into each runtime work block. */
typedef struct PcpScatterRadialParams {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    u32 radialSegments;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    s32 fadeDuration;
    f32 initialHeightStep;
    f32 heightDamping;
    f32 initialAngleStep;
    f32 angleDamping;
    f32 startRadius;
    f32 endRadius;
    f32 radiusJitter;
    f32 targetRadiusJitter;
    f32 speedJitter;
    u8 pulseEnabled;
    u8 pad61[3];
    f32 pulseAngle;
    u8 duplicateParticles;
    u8 pad69[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterRadialParams;

typedef char PcpScatterRadialParams_size_must_be_0x74[
    (sizeof(PcpScatterRadialParams) == 0x74) ? 1 : -1];
typedef char PcpScatterRadialParams_initialHeightStep_offset_must_be_0x3C[
    ((u32)&((PcpScatterRadialParams *)0)->initialHeightStep == 0x3C) ? 1 : -1];
typedef char PcpScatterRadialParams_initialAngleStep_offset_must_be_0x44[
    ((u32)&((PcpScatterRadialParams *)0)->initialAngleStep == 0x44) ? 1 : -1];

/* Per-particle age and radial/vertical motion state. */
typedef struct PcpScatterRadialParticle {
    s32 age;
    f32 radiusStep;
    f32 angleStep;
    f32 heightStep;
    f32 radius;
    f32 angle;
    f32 height;
} PcpScatterRadialParticle;

typedef char PcpScatterRadialParticle_size_must_be_0x1C[
    (sizeof(PcpScatterRadialParticle) == 0x1C) ? 1 : -1];

/* Runtime owner followed inline by its variable-length particle array. */
typedef struct PcpScatterRadialWork {
    PcpScatterRadialParams params;
    PcpScatterRadialParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemBlock *allocation;
    u32 duplicateGroupCount;
    EffParamWork **duplicatedHandles;
    SdfMemBlock *duplicateAllocation;
} PcpScatterRadialWork;

typedef char PcpScatterRadialWork_size_must_be_0x94[
    (sizeof(PcpScatterRadialWork) == 0x94) ? 1 : -1];
typedef char PcpScatterRadialWork_particles_offset_must_be_0x74[
    ((u32)&((PcpScatterRadialWork *)0)->particles == 0x74) ? 1 : -1];
typedef char PcpScatterRadialWork_allocation_offset_must_be_0x84[
    ((u32)&((PcpScatterRadialWork *)0)->allocation == 0x84) ? 1 : -1];
typedef char PcpScatterRadialWork_duplicateGroupCount_offset_must_be_0x88[
    ((u32)&((PcpScatterRadialWork *)0)->duplicateGroupCount == 0x88) ? 1 : -1];
typedef char PcpScatterRadialWork_duplicateAllocation_offset_must_be_0x90[
    ((u32)&((PcpScatterRadialWork *)0)->duplicateAllocation == 0x90) ? 1 : -1];

#endif
