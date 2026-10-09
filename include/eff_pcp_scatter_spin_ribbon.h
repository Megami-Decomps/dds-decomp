#ifndef EFF_PCP_SCATTER_SPIN_RIBBON_H
#define EFF_PCP_SCATTER_SPIN_RIBBON_H

#include "eff.h"
#include "eff_param.h"
#include "sdf_resource.h"

/* Serialized parameters copied into each spin-scatter work owner. */
typedef struct PcpScatterSpinParams {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    s32 fadeDuration;
    f32 angleStep;
    f32 angleDamping;
    f32 startRadius;
    f32 endRadius;
    f32 startRadiusJitter;
    f32 endRadiusJitter;
    u8 adjustAngle;
    u8 pad51[3];
    f32 endAngleStep;
    u8 duplicateParticles;
    u8 pad59[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterSpinParams;

typedef struct PcpScatterSpinParticle {
    s32 age;
    f32 radiusStep;
    f32 angleStep;
    f32 radius;
    f32 angle;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
} PcpScatterSpinParticle;

/* Runtime owner followed inline by its variable-length particle array. */
typedef struct PcpScatterSpinWork {
    PcpScatterSpinParams params;
    PcpScatterSpinParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemBlock *allocation;
    u32 duplicateGroupCount;
    EffParamWork **duplicatedHandles;
    SdfMemBlock *duplicateAllocation;
} PcpScatterSpinWork;

/* Serialized parameters copied into each ribbon-scatter work owner. */
typedef struct PcpScatterRibbonParams {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    s32 fadeDuration;
    f32 heightStep;
    u8 pad3C[4];
    u8 duplicateParticles;
    u8 pad41[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterRibbonParams;

typedef struct PcpScatterRibbonParticle {
    s32 age;
    f32 height;
    f32 heightStep;
    f32 tiltAngle;
    f32 tiltHalfAngle;
    f32 tiltStep;
    f32 radius;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} PcpScatterRibbonParticle;

/* Runtime owner followed inline by its variable-length particle array. */
typedef struct PcpScatterRibbonWork {
    PcpScatterRibbonParams params;
    PcpScatterRibbonParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemBlock *allocation;
    u32 duplicateGroupCount;
    EffParamWork **duplicatedHandles;
    SdfMemBlock *duplicateAllocation;
} PcpScatterRibbonWork;

typedef char PcpScatterSpinParams_size_must_be_0x64[
    (sizeof(PcpScatterSpinParams) == 0x64) ? 1 : -1];
typedef char PcpScatterSpinParticle_size_must_be_0x20[
    (sizeof(PcpScatterSpinParticle) == 0x20) ? 1 : -1];
typedef char PcpScatterSpinWork_size_must_be_0x84[
    (sizeof(PcpScatterSpinWork) == 0x84) ? 1 : -1];
typedef char PcpScatterSpinWork_particles_offset_must_be_0x64[
    ((u32)&((PcpScatterSpinWork *)0)->particles == 0x64) ? 1 : -1];
typedef char PcpScatterSpinWork_allocation_offset_must_be_0x74[
    ((u32)&((PcpScatterSpinWork *)0)->allocation == 0x74) ? 1 : -1];
typedef char PcpScatterSpinWork_duplicateAllocation_offset_must_be_0x80[
    ((u32)&((PcpScatterSpinWork *)0)->duplicateAllocation == 0x80) ? 1 : -1];

typedef char PcpScatterRibbonParams_size_must_be_0x4C[
    (sizeof(PcpScatterRibbonParams) == 0x4C) ? 1 : -1];
typedef char PcpScatterRibbonParticle_size_must_be_0x28[
    (sizeof(PcpScatterRibbonParticle) == 0x28) ? 1 : -1];
typedef char PcpScatterRibbonWork_size_must_be_0x6C[
    (sizeof(PcpScatterRibbonWork) == 0x6C) ? 1 : -1];
typedef char PcpScatterRibbonWork_particles_offset_must_be_0x4C[
    ((u32)&((PcpScatterRibbonWork *)0)->particles == 0x4C) ? 1 : -1];
typedef char PcpScatterRibbonWork_allocation_offset_must_be_0x5C[
    ((u32)&((PcpScatterRibbonWork *)0)->allocation == 0x5C) ? 1 : -1];
typedef char PcpScatterRibbonWork_duplicateAllocation_offset_must_be_0x68[
    ((u32)&((PcpScatterRibbonWork *)0)->duplicateAllocation == 0x68) ? 1 : -1];

#endif
