#include "common.h"
#include "sdf_asset_state.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_param.h"
#include "eff_pcp_scatter_radial.h"
#include "eff_pcp_scatter_spin_ribbon.h"
#include "eff_pcp_scatter_rings.h"
#include "eff_scatter_draw.h"
#include "fpu.h"
#include "sdf_texture_file.h"

#define EFF_SCATTER_NEUTRAL_COLOR 0x80808080
#define EFF_SCATTER_RGB_MASK 0xFFFFFF
/* Keep these spellings distinct: HALF_TURN * 2.0f rounds differently. */
#define EFF_SCATTER_RADIAL_TURN 6.2831852f
#define EFF_SCATTER_HALF_TURN 3.14159265f
#define EFF_SCATTER_RIBBON_TURN 6.2831850051879883f
#define EFF_SCATTER_DEGREES_TO_RADIANS 0.017453292f
#define EFF_SCATTER_RIGHT_ANGLE_DEGREES 90.0f
#define EFF_SCATTER_DIAGONAL_ANGLE_DEGREES 45.0f
#define EFF_SCATTER_TILT_STEP_DEGREES 5.0f
#define EFF_SCATTER_INITIAL_RIBBON_RADIUS 200.0f
#define EFF_SCATTER_RANDOM_MIDPOINT 0.5f
#define EFF_SCATTER_RANDOM_SPAN 2.0f
#define EFF_SCATTER_HALF_SEGMENT 0.5f
#define EFF_SCATTER_POOL_SLOTS_PER_GROUP 3
#define EFF_SCATTER_POOL_RECORD_WORDS_PER_SLOT 8
#define EFF_SCATTER_POOL_AUX_WORDS_PER_SLOT 2
#define EFF_SCATTER_WORD_BYTES 4
#define EFF_SCATTER_POOL_CONTROL_BYTES 0x34
#define EFF_SCATTER_RECORD_BYTES 0x60
#define EFF_SCATTER_AUX_RECORD_BYTES 0x18
#define EFF_SCATTER_RES_BYTES 8
#define EFF_SCATTER_DRAW_TEMPLATE_BYTES 0x2C
#define EFF_SCATTER_PARAM_BLOCK 0
#define EFF_SCATTER_RESOURCE_BLOCK 1
#define EFF_SCATTER_CHILD_BLOCK 2

extern void sdfComposeVuMatrixFromRegisters(void);

extern void *effParamTableGetBlock(void *, s32);

extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *res);

extern u8 effDefaultRandomState[];
extern f32 effMiscRandUnitFloat(void *state);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_003364B8(f32 angle);
extern void func_00336818(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);
extern void vu0RotMatrixXYZFromVec3(f32 *rot);
extern f32 *effGetScatterWideBlock(PcpScatterDraw *object, s32 index);
extern f32 *effGetScatterNarrowBlock(PcpScatterDraw *object, s32 index);


/* Ownership handles refer to SDF allocation nodes, not their retained payloads. */

extern PcpScatterRes *effPcpScatterResCreate(u32 resId);



extern PcpScatterInstanceA *effPcpScatterCreateParticleInstance();

extern void effShareScatterResource(PcpScatterDraw *object, PcpScatterDraw *source);






extern PcpScatterInstanceB *effScatterCreateDampedRing();
extern PcpScatterInstanceC *effScatterCreateTwoColorRing();
extern PcpScatterPlainInstance *effPcpScatterCreatePlainInstance();






extern PcpScatterPool *effPcpScatterPoolCreate(s32 groups);
extern void effPcpScatterCreatePoolResource(PcpScatterPool *work, u32 resource);
extern void effPcpScatterReleasePoolResources(PcpScatterPool *work);
extern void effPcpScatterResRelease(PcpScatterRes *res);
extern u32 effMiscRand(void *state);
extern PcpScatterSpinWork *effScatterCreateSpinWork();
extern PcpScatterRibbonWork *effScatterCreateRibbonWork();


extern void effPcpScatterSharePoolResource(PcpScatterPool *dst, PcpScatterPool *src);



extern void *memset(void *dst, s32 value, u32 size);

typedef struct EffPacketParams {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    u32 *parameters;
    u128 *positions;
    u128 *normals;
    u32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    s32 (*allocate)(s32);
    f32 depth;
} EffPacketParams;


extern EffPacketParams D_00452020[];
extern u32 D_003B13A0[];
extern u32 D_003B13F0[];
extern SdfPoolNode *D_003B14B0[];
extern s32 sdfAllocPacketAligned(s32);
extern s32 func_00167A10(EffPacketParams *);

extern SdfAsset *sdfCreateAssetWithDrawEntries(void);


/* Constructors also serve the legacy parameter-table dispatch surface. */
extern PcpScatterRadialWork *effScatterCreateRadialWork(
    const PcpScatterRadialParams *params, u32 resource, void *particleParams);







/* Create radial work from the parameter, texture-resource and child-work blocks. */
void effScatterCreateFromParameterTriplet(void *parameterTable) {
    void *params;
    u32 resource;
    void *options;

    params = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);
    options = effParamTableGetBlock(parameterTable, EFF_SCATTER_CHILD_BLOCK);
    effScatterCreateRadialWork(params, resource, options);
}

/* Return a radial clone sharing the texture owner; every group clones source group zero.
 * Unlike the ribbon clone, this path does not normalize particlesPerGroup. */
PcpScatterRadialWork *effPcpScatterSharedDuplicate(source)
    PcpScatterRadialWork *source;
{
    PcpScatterRadialWork *work;
    u32 count;
    SdfMemBlock *allocation;
    EffParamWork **handles;
    u32 i;

    work = effScatterCreateRadialWork(&source->params, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, source->childWork);
    if (work->params.duplicateParticles != 0) {
        work->duplicateGroupCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicateGroupCount = work->duplicateGroupCount + 1;
        }
        count = work->duplicateGroupCount;
        allocation = sdfAllocGeneralBlock(count * EFF_SCATTER_WORD_BYTES);
        handles = (EffParamWork **)sdfResourceRetainAddress(allocation);
        work->duplicateAllocation = allocation;
        work->duplicatedHandles = handles;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*source->duplicatedHandles);
        }
    }
    return work;
}


/* Dispatch/free duplicated groups, then release their array, pool and main allocation. */
void effPcpScatterReleaseParticleGroup(PcpScatterRadialWork *work) {
    if (work->duplicateAllocation != 0) {
        u32 count = work->duplicateGroupCount;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->allocation);
}

/* Seed radial particle index, clamping only the local segment count.
 * Duration is unchecked; preserve the independent jitter samples and their order. */
void effPcpScatterInitRadialParticle(PcpScatterRadialWork *work, u32 index) {
    u32 segments = work->params.radialSegments;
    PcpScatterRadialParticle *particle = &work->particles[index];
    f32 angleStep;
    f32 jitter;

    if (segments == 0) {
        segments = 1;
    }
    angleStep = EFF_SCATTER_RADIAL_TURN / segments;
    particle->angle = angleStep * (index % segments) + angleStep * EFF_SCATTER_HALF_SEGMENT * effMiscRandUnitFloat(effDefaultRandomState);
    jitter = work->params.radiusJitter;
    particle->radius = work->params.startRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    particle->height = 0.0f;
    jitter = work->params.targetRadiusJitter;
    particle->radiusStep = (work->params.endRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) - particle->radius) / work->params.duration;
    jitter = work->params.speedJitter;
    particle->heightStep = work->params.initialHeightStep * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    particle->age = 0;
    particle->angleStep = work->params.initialAngleStep;
}

extern u128 *effPcpScatterGetRecordAddress(PcpScatterPool *, s32);
extern u32 *effPcpScatterGetAuxRecordAddress(PcpScatterPool *, s32);
extern void effPcpScatterDrawPool(PcpScatterPool *);
extern u32 effBlendColor(u32, u32, f32);
extern f32 sdfAtan2Poly(f32 ratio);

/* Advance radial particles and build each six-vertex strip in the shared pool. */
void func_00178B80(PcpScatterRadialWork *work) {
    f32 next[4];
    f32 position[4];
    f32 duplicatePosition[4];
    f32 direction[4];
    f32 radial[4];
    f32 halfWidth[4];
    f32 halfHeight[4];
    f32 side[4];
    s32 respawn;
    s32 pulse;
    s32 duplicates;
    u32 i;
    u32 count;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u32 baseColor;
    u32 perGroup;
    s32 duration;
    s32 endAge;
    s32 duplicateStart;
    PcpScatterRadialParticle *particle;
    f32 pulseAngle;
    f32 heightDamping;
    f32 angleDamping;

    i = 0;
    duration = work->params.duration;
    endAge = work->params.fadeDuration + duration;
    duplicates = work->duplicatedHandles != NULL;
    count = work->params.particleCount;
    VEC3_SPLAT(halfWidth, work->params.width * 0.5f);
    VEC3_SPLAT(halfHeight, work->params.height * 0.5f);
    particle = work->particles;
    respawn = work->params.respawn;
    pulse = work->params.pulseEnabled;
    pulseAngle = work->params.pulseAngle;
    fadeIn = work->params.fadeIn;
    fadeOut = work->params.fadeOut;
    heightDamping = work->params.heightDamping;
    angleDamping = work->params.angleDamping;
    duplicateStart = work->params.duplicateStartAge;
    perGroup = work->params.particlesPerGroup;
    baseColor = work->color;

    for (; i < count; i++, particle++) {
        s32 age = particle->age;
        u128 *vertices = effPcpScatterGetRecordAddress(work->childWork, i);
        u32 *colors = effPcpScatterGetAuxRecordAddress(work->childWork, i);
        if (age == 0) {
            effPcpScatterInitRadialParticle(work, i);
        } else if (age > 0) {
            f32 factor = 0.0f;
            s32 moving;
            f32 angle;
            f32 radius;
            f32 height;
            f32 angleStep;
            f32 heightStep;
            u32 j;

            if (age <= endAge) {
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / fadeIn;
                } else if (endAge - age <= fadeOut && fadeOut != 0) {
                    factor = (f32)(endAge - age) / fadeOut;
                } else {
                    factor = 1.0f;
                }
            }
            moving = age < duration;
            color = effBlendColor(baseColor & 0xFFFFFF, baseColor, factor);
            angle = particle->angle;
            radius = particle->radius;
            height = particle->height;
            angleStep = particle->angleStep;
            heightStep = particle->heightStep;
            if (!moving && pulse) {
                if (age == duration) {
                    angle += pulseAngle;
                    height += heightStep * pulseAngle / angleStep;
                } else if (age == duration + 1) {
                    angle -= pulseAngle;
                    height -= heightStep * pulseAngle / angleStep;
                }
            }
            position[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
            position[1] = height;
            position[2] = sdfSinPoly(angle) * radius;
            VU0_LOAD_VF(vf12, position);
            for (j = 0; j < 3; j++, vertices++, colors++) {
                height += heightStep;
                colors[0] = color;
                colors[3] = color;
                angle += sdfAtan2Poly(halfWidth[0] / radius);
                next[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
                next[1] = height;
                next[2] = sdfSinPoly(angle) * radius;
                VU0_LOAD_VF(vf10, next);
                VU0_LOAD_VF(vf11, position);
                VU0_SUB(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                VU0_LOAD_VF(vf11, halfWidth);
                VU0_MUL(vf10, vf10, vf11);
                VU0_ADD(vf10, vf10, vf12);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);
                VU0_STORE_VF_UNCLOBBERED(vf10, radial);
                radial[1] = 0.0f;
                VU0_LOAD_VF(vf10, direction);
                VU0_LOAD_VF(vf11, radial);
                VU0_CROSS_XYZ(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                VU0_LOAD_VF(vf11, halfHeight);
                VU0_MUL(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, side);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, side);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
                VU0_SUB(vf10, vf10, vf11);
                VU0_SUB(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 3);
                if (j == 1) {
                    PCP_COPY_VECTOR(duplicatePosition, position);
                }
                PCP_COPY_VECTOR(position, next);
            }
            if (moving) {
                particle->radius += particle->radiusStep;
                particle->angle += angleStep;
                particle->height += heightStep;
                particle->angleStep = angleStep * angleDamping;
                particle->heightStep = heightStep * heightDamping;
            }
        } else {
            colors[0] = 0;
            colors[1] = 0;
            colors[2] = 0;
            colors[3] = 0;
            colors[4] = 0;
            colors[5] = 0;
        }
        if (respawn && age >= endAge) {
            particle->age = -1;
        }
        if (duplicates && age >= duplicateStart && age >= 0 && i % perGroup == 0) {
            u32 group = i / perGroup;
            effParamWorkCallback3(work->duplicatedHandles[group], baseColor);
            VU0_LOAD_VF(vf10, work->params.origin);
            VU0_LOAD_VF(vf11, duplicatePosition);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, duplicatePosition);
            effParamWorkCallback0(work->duplicatedHandles[group], duplicatePosition);
            effParamWorkInvokeCallback(work->duplicatedHandles[group]);
        }
        particle->age++;
    }
    work->childWork->origin[0] = work->params.origin[0];
    work->childWork->origin[1] = work->params.origin[1];
    work->childWork->origin[2] = work->params.origin[2];
    effPcpScatterDrawPool(work->childWork);
}

/* Copy one packed four-component vector; no scalar reconstruction of the W lane. */
void effScatterCopyRadialVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Store the radial work's packed color; this does not replace duplicated handles. */
void effScatterSetRadialColor(PcpScatterRadialWork *work, u32 color) {
    work->color = color;
}

/* Rescale the initial height step and radius endpoints in place; calls compound. */
void effScatterScaleRadialInputs(float scale, PcpScatterRadialWork *work) {
    work->params.initialHeightStep *= scale;
    work->params.startRadius *= scale;
    work->params.endRadius *= scale;
}

/* Return spin work with trailing particles, a draw pool and optional child groups.
 * Group normalization occurs only when child parameters and duplication are enabled. */
PcpScatterSpinWork *effScatterCreateSpinWork(params, resource, particleParams)
    const PcpScatterSpinParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterSpinWork *work;
    PcpScatterSpinParticle *particle;
    SdfMemBlock *allocation;
    EffParamWork **handles;
    u32 count;
    u32 i;
    s32 delaySpread;

    allocation = sdfAllocGeneralBlock(sizeof(PcpScatterSpinWork) + params->particleCount * sizeof(PcpScatterSpinParticle));
    work = (PcpScatterSpinWork *)sdfResourceRetainAddress(allocation);
    work->particles = (PcpScatterSpinParticle *)(work + 1);
    work->params = *params;
    work->color = EFF_SCATTER_NEUTRAL_COLOR;
    work->allocation = allocation;
    work->scale = 1.0f;
    work->duplicatedHandles = NULL;
    work->duplicateAllocation = NULL;
    work->childWork = effPcpScatterPoolCreate(params->particleCount);
    work->childWork->unk10 = params->poolMode;
    if (resource != 0) {
        effPcpScatterCreatePoolResource(work->childWork, resource);
    }
    if (particleParams != NULL && work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicateGroupCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicateGroupCount++;
        }
        count = work->duplicateGroupCount;
        allocation = sdfAllocGeneralBlock(count * sizeof(u32));
        handles = (EffParamWork **)sdfResourceRetainAddress(allocation);
        work->duplicateAllocation = allocation;
        work->duplicatedHandles = handles;
        /* Native setup seeds group zero even when the computed group count is zero. */
        work->duplicatedHandles[0] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, particleParams);
        for (i = 1; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(work->duplicatedHandles[0]);
        }
    }
    delaySpread = work->params.delaySpread;
    count = work->params.particleCount;
    particle = work->particles;
    if (delaySpread <= 0) {
        delaySpread = 1;
    }
    for (i = 0; i < count; i++, particle++) {
        particle->age = -(effMiscRand(effDefaultRandomState) % delaySpread);
    }
    return work;
}

/* Create spin work from three parameter-table blocks, retaining native local values. */
void effScatterCreateSpinFromTable(void *parameterTable) {
    void *params;
    u32 resource;
    void *options;

    params = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);
    options = effParamTableGetBlock(parameterTable, EFF_SCATTER_CHILD_BLOCK);
    effScatterCreateSpinWork(params, resource, options);
}


/* Return a spin clone sharing the texture owner; each child clones source group zero.
 * The copied group size is used directly, without the constructor's conditional clamp. */
PcpScatterSpinWork *effScatterCloneSpinWork(source)
    PcpScatterSpinWork *source;
{
    PcpScatterSpinWork *work;
    u32 count;
    SdfMemBlock *allocation;
    EffParamWork **handles;
    u32 i;

    work = effScatterCreateSpinWork(&source->params, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, source->childWork);
    if (work->params.duplicateParticles != 0) {
        work->duplicateGroupCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicateGroupCount = work->duplicateGroupCount + 1;
        }
        count = work->duplicateGroupCount;
        allocation = sdfAllocGeneralBlock(count * EFF_SCATTER_WORD_BYTES);
        handles = (EffParamWork **)sdfResourceRetainAddress(allocation);
        work->duplicateAllocation = allocation;
        work->duplicatedHandles = handles;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*source->duplicatedHandles);
        }
    }
    return work;
}


/* Release spin child groups before their array, draw pool and main allocation. */
void effPcpScatterReleaseSharedParticles(PcpScatterSpinWork *work) {
    if (work->duplicateAllocation != 0) {
        u32 count = work->duplicateGroupCount;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->allocation);
}


/* Seed sprite index with independent radius samples and a normalized random direction.
 * Keep RNG order and the uninitialized packed direction W lane; duration is unchecked. */
void effScatterSpriteSpawn(PcpScatterSpinWork *work, s32 index)
{
    PcpScatterSpinParticle *sprite = &work->particles[index];
    f32 direction[4];
    f32 jitter;

    sprite->angle = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_SCATTER_HALF_TURN * 2.0f);
    jitter = work->params.startRadiusJitter;
    sprite->radius = work->params.startRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.endRadiusJitter;
    sprite->radiusStep = (work->params.endRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) - sprite->radius) / (f32)work->params.duration;
    sprite->angleStep = work->params.angleStep;
    direction[0] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_SCATTER_RANDOM_MIDPOINT) * EFF_SCATTER_RANDOM_SPAN;
    direction[1] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_SCATTER_RANDOM_MIDPOINT) * EFF_SCATTER_RANDOM_SPAN;
    direction[2] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_SCATTER_RANDOM_MIDPOINT) * EFF_SCATTER_RANDOM_SPAN;
    VU0_LOAD_VF(vf10, direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, direction);
    sprite->age = 0;
    sprite->dirX = direction[0];
    sprite->dirY = direction[1];
    sprite->dirZ = direction[2];
}

struct RwV3d;
extern void sdfBuildVuRotationFromAxisAngle(const struct RwV3d *, f32);

/* Rotate each particle around its seeded axis and build its six-vertex strip. */
void func_00179780(PcpScatterSpinWork *work) {
    f32 axis[4];
    f32 next[4];
    f32 position[4];
    f32 duplicatePosition[4];
    f32 direction[4];
    f32 radial[4];
    f32 halfWidth[4];
    f32 halfHeight[4];
    f32 side[4];
    s32 respawn;
    s32 pulse;
    s32 duplicates;
    u32 i;
    u32 count;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u32 baseColor;
    u32 perGroup;
    s32 duration;
    s32 endAge;
    s32 duplicateStart;
    PcpScatterSpinParticle *particle;
    f32 pulseAngle;
    f32 angleDamping;

    i = 0;
    duration = work->params.duration;
    endAge = work->params.fadeDuration + duration;
    duplicates = work->duplicatedHandles != NULL;
    count = work->params.particleCount;
    VEC3_SPLAT(halfWidth, work->params.width * 0.5f);
    VEC3_SPLAT(halfHeight, work->params.height * 0.5f);
    particle = work->particles;
    respawn = work->params.respawn;
    pulse = work->params.adjustAngle;
    pulseAngle = work->params.endAngleStep;
    fadeIn = work->params.fadeIn;
    fadeOut = work->params.fadeOut;
    angleDamping = work->params.angleDamping;
    duplicateStart = work->params.duplicateStartAge;
    perGroup = work->params.particlesPerGroup;
    baseColor = work->color;

    for (; i < count; i++, particle++) {
        s32 age = particle->age;
        u128 *vertices = effPcpScatterGetRecordAddress(work->childWork, i);
        u32 *colors = effPcpScatterGetAuxRecordAddress(work->childWork, i);
        if (age == 0) {
            effScatterSpriteSpawn(work, i);
        } else if (age > 0) {
            f32 factor = 0.0f;
            s32 moving;
            f32 angle;
            f32 radius;
            f32 angleStep;
            u32 j;

            if (age <= endAge) {
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / fadeIn;
                } else if (endAge - age <= fadeOut && fadeOut != 0) {
                    factor = (f32)(endAge - age) / fadeOut;
                } else {
                    factor = 1.0f;
                }
            }
            moving = age < duration;
            color = effBlendColor(baseColor & 0xFFFFFF, baseColor, factor);
            angle = particle->angle;
            radius = particle->radius;
            angleStep = particle->angleStep;
            if (!moving && pulse) {
                if (age == duration) {
                    angle += pulseAngle;
                } else if (age == duration + 1) {
                    angle -= pulseAngle;
                }
            }
            axis[0] = particle->dirX;
            axis[1] = particle->dirY;
            axis[2] = particle->dirZ;
            sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)axis, angle);
            position[0] = radius;
            position[1] = 0.0f;
            position[2] = 0.0f;
            VU0_LOAD_VF(vf10, position);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            VU0_MOVE_VF(vf12, vf10);
            for (j = 0; j < 3; j++, vertices++, colors++) {
                colors[0] = color;
                colors[3] = color;
                angle += sdfAtan2Poly(halfWidth[0] / radius);
                sdfBuildVuRotationFromAxisAngle((const struct RwV3d *)axis, angle);
                next[0] = radius;
                next[1] = 0.0f;
                next[2] = 0.0f;
                VU0_LOAD_VF(vf10, next);
                VU0_APPLY_MATRIX(vf10, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, next);
                VU0_LOAD_VF(vf10, next);
                VU0_LOAD_VF(vf11, position);
                VU0_SUB(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                VU0_LOAD_VF(vf11, halfWidth);
                VU0_MUL(vf10, vf10, vf11);
                VU0_ADD(vf10, vf10, vf12);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);
                VU0_STORE_VF_UNCLOBBERED(vf10, radial);
                radial[1] = 0.0f;
                VU0_LOAD_VF(vf10, direction);
                VU0_LOAD_VF(vf11, radial);
                VU0_CROSS_XYZ(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                VU0_LOAD_VF(vf11, halfHeight);
                VU0_MUL(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, side);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, side);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
                VU0_SUB(vf10, vf10, vf11);
                VU0_SUB(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 3);
                if (j == 1) {
                    PCP_COPY_VECTOR(duplicatePosition, position);
                }
                PCP_COPY_VECTOR(position, next);
            }
            if (moving) {
                particle->radius += particle->radiusStep;
                particle->angle += angleStep;
                particle->angleStep = angleStep * angleDamping;
            }
        } else {
            colors[0] = 0;
            colors[1] = 0;
            colors[2] = 0;
            colors[3] = 0;
            colors[4] = 0;
            colors[5] = 0;
        }
        if (respawn && age >= endAge) {
            particle->age = -1;
        }
        if (duplicates && age >= duplicateStart && age >= 0 && i % perGroup == 0) {
            u32 group = i / perGroup;
            effParamWorkCallback3(work->duplicatedHandles[group], baseColor);
            VU0_LOAD_VF(vf10, work->params.origin);
            VU0_LOAD_VF(vf11, duplicatePosition);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, duplicatePosition);
            effParamWorkCallback0(work->duplicatedHandles[group], duplicatePosition);
            effParamWorkInvokeCallback(work->duplicatedHandles[group]);
        }
        particle->age++;
    }
    work->childWork->origin[0] = work->params.origin[0];
    work->childWork->origin[1] = work->params.origin[1];
    work->childWork->origin[2] = work->params.origin[2];
    effPcpScatterDrawPool(work->childWork);
}

/* Copy the spin variant's packed parameter vector, including its existing W lane. */
void effScatterCopySpinVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Replace the spin work's packed color. */
void effScatterSetSpinColor(PcpScatterSpinWork *work, u32 color) {
    work->color = color;
}

/* Rescale spin start/end radius inputs in place; repeated calls compound. */
void effScatterScaleSpinRadii(float scale, PcpScatterSpinWork *work) {
    work->params.startRadius *= scale;
    work->params.endRadius *= scale;
}

/* Return ribbon work with trailing particles and optional duplicated child groups.
 * As with spin work, the zero group-size clamp is conditional on child creation. */
PcpScatterRibbonWork *effScatterCreateRibbonWork(params, resource, particleParams)
    const PcpScatterRibbonParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterRibbonWork *work;
    PcpScatterRibbonParticle *particle;
    SdfMemBlock *allocation;
    EffParamWork **handles;
    u32 count;
    u32 i;
    s32 delaySpread;

    allocation = sdfAllocGeneralBlock(sizeof(PcpScatterRibbonWork) + params->particleCount * sizeof(PcpScatterRibbonParticle));
    work = (PcpScatterRibbonWork *)sdfResourceRetainAddress(allocation);
    work->particles = (PcpScatterRibbonParticle *)(work + 1);
    work->params = *params;
    work->color = EFF_SCATTER_NEUTRAL_COLOR;
    work->allocation = allocation;
    work->scale = 1.0f;
    work->duplicatedHandles = NULL;
    work->duplicateAllocation = NULL;
    work->childWork = effPcpScatterPoolCreate(params->particleCount);
    work->childWork->unk10 = params->poolMode;
    if (resource != 0) {
        effPcpScatterCreatePoolResource(work->childWork, resource);
    }
    if (particleParams != NULL && work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicateGroupCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicateGroupCount++;
        }
        count = work->duplicateGroupCount;
        allocation = sdfAllocGeneralBlock(count * sizeof(u32));
        handles = (EffParamWork **)sdfResourceRetainAddress(allocation);
        work->duplicateAllocation = allocation;
        work->duplicatedHandles = handles;
        /* Native setup seeds group zero even when the computed group count is zero. */
        work->duplicatedHandles[0] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, particleParams);
        for (i = 1; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(work->duplicatedHandles[0]);
        }
    }
    delaySpread = work->params.delaySpread;
    count = work->params.particleCount;
    particle = work->particles;
    if (delaySpread <= 0) {
        delaySpread = 1;
    }
    for (i = 0; i < count; i++, particle++) {
        particle->age = -(effMiscRand(effDefaultRandomState) % delaySpread);
    }
    return work;
}

/* Create ribbon work from the parameter, resource and child-work blocks. */
void effScatterCreateRibbonFromTable(void *parameterTable) {
    void *params;
    u32 resource;
    void *options;

    params = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);
    options = effParamTableGetBlock(parameterTable, EFF_SCATTER_CHILD_BLOCK);
    effScatterCreateRibbonWork(params, resource, options);
}


/* Return a ribbon clone sharing its texture owner and cloning source group zero.
 * This clone normalizes the copied group size even though radial/spin clones do not. */
PcpScatterRibbonWork *effScatterCloneRibbonWork(source)
    PcpScatterRibbonWork *source;
{
    PcpScatterRibbonWork *work;
    u32 count;
    SdfMemBlock *allocation;
    EffParamWork **handles;
    u32 i;

    work = effScatterCreateRibbonWork(&source->params, 0, 0);
    effPcpScatterSharePoolResource(work->childWork, source->childWork);
    if (work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicateGroupCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicateGroupCount = work->duplicateGroupCount + 1;
        }
        count = work->duplicateGroupCount;
        allocation = sdfAllocGeneralBlock(count * EFF_SCATTER_WORD_BYTES);
        handles = (EffParamWork **)sdfResourceRetainAddress(allocation);
        work->duplicateAllocation = allocation;
        work->duplicatedHandles = handles;
        for (i = 0; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(*source->duplicatedHandles);
        }
    }
    return work;
}


/* Release ribbon child groups before their array, draw pool and main allocation. */
void effPcpScatterReleaseLinkedParticles(PcpScatterRibbonWork *work) {
    if (work->duplicateAllocation != 0) {
        u32 count = work->duplicateGroupCount;
        u32 i;

        for (i = 0; i < count; i++) {
            effDispatchParameterDataAndFreeWork(work->duplicatedHandles[i]);
        }
        sdfReleaseResourceAllocation(work->duplicateAllocation);
    }
    effPcpScatterReleasePoolResources(work->childWork);
    sdfReleaseResourceAllocation(work->allocation);
}


/* Seed ribbon index with fixed tilt angles and a normalized random XZ direction.
 * The packed direction W lane and the opaque motion fields retain native setup. */
void effScatterInitRibbonParticle(PcpScatterRibbonWork *work, s32 index)
{
    f32 rightAngle = EFF_SCATTER_RIGHT_ANGLE_DEGREES * EFF_SCATTER_DEGREES_TO_RADIANS;
    f32 diagonalAngle = EFF_SCATTER_DIAGONAL_ANGLE_DEGREES * EFF_SCATTER_DEGREES_TO_RADIANS;
    f32 tiltStep = EFF_SCATTER_TILT_STEP_DEGREES * EFF_SCATTER_DEGREES_TO_RADIANS;
    PcpScatterRibbonParticle *particle = &work->particles[index];
    f32 direction[4];

    particle->height = 0.0f;
    particle->heightStep = work->params.heightStep;
    particle->tiltHalfAngle = rightAngle;
    particle->tiltAngle = diagonalAngle;
    particle->tiltStep = tiltStep;
    particle->unk1C = effMiscRandUnitFloat(effDefaultRandomState) * EFF_SCATTER_RIBBON_TURN;
    particle->radius = EFF_SCATTER_INITIAL_RIBBON_RADIUS;

    direction[0] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_SCATTER_RANDOM_MIDPOINT) * EFF_SCATTER_RANDOM_SPAN;
    direction[1] = 0.0f;
    direction[2] = (effMiscRandUnitFloat(effDefaultRandomState) - EFF_SCATTER_RANDOM_MIDPOINT) * EFF_SCATTER_RANDOM_SPAN;
    VU0_NORMALIZE_PACKED_VECTOR(direction);

    particle->unk20 = direction[0];
    particle->age = 0;
    particle->unk24 = direction[2];
}

/* Advance ribbon particles and build each six-vertex strip in the shared pool. */
void func_0017A340(PcpScatterRibbonWork *work) {
    f32 next[4];
    f32 position[4];
    f32 duplicatePosition[4];
    f32 direction[4];
    f32 normal[4];
    f32 halfWidth[4];
    f32 halfHeight[4];
    f32 side[4];
    s32 respawn;
    s32 duplicates;
    u32 i;
    u32 count;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u32 baseColor;
    u32 perGroup;
    s32 duration;
    s32 endAge;
    s32 duplicateStart;
    PcpScatterRibbonParticle *particle;

    i = 0;
    duration = work->params.duration;
    endAge = work->params.fadeDuration + duration;
    duplicates = work->duplicatedHandles != NULL;
    count = work->params.particleCount;
    VEC3_SPLAT(halfWidth, work->params.width * 0.5f);
    VEC3_SPLAT(halfHeight, work->params.height * 0.5f);
    particle = work->particles;
    respawn = work->params.respawn;
    fadeIn = work->params.fadeIn;
    fadeOut = work->params.fadeOut;
    duplicateStart = work->params.duplicateStartAge;
    perGroup = work->params.particlesPerGroup;
    baseColor = work->color;

    for (; i < count; i++, particle++) {
        s32 age = particle->age;
        u128 *vertices = effPcpScatterGetRecordAddress(work->childWork, i);
        u32 *colors = effPcpScatterGetAuxRecordAddress(work->childWork, i);
        if (age == 0) {
            effScatterInitRibbonParticle(work, i);
        } else if (age > 0) {
            f32 factor = 0.0f;
            f32 angle;
            f32 radius;
            f32 height;
            f32 heightStep;
            f32 tiltStep;
            u32 j;

            if (age <= endAge) {
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / fadeIn;
                } else if (endAge - age <= fadeOut && fadeOut != 0) {
                    factor = (f32)(endAge - age) / fadeOut;
                } else {
                    factor = 1.0f;
                }
            }
            color = effBlendColor(baseColor & 0xFFFFFF, baseColor, factor);
            radius = particle->radius;
            height = particle->height;
            heightStep = particle->heightStep;
            angle = particle->tiltAngle - sdfAtan2Poly(halfWidth[0] / radius);
            tiltStep = particle->tiltStep;
            normal[0] = 0.0f;
            normal[1] = 0.0f;
            normal[2] = 1.0f;
            position[0] = sdfSinPoly(angle) * radius;
            position[1] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius + height;
            position[2] = 0.0f;
            VU0_LOAD_VF(vf10, position);
            VU0_MOVE_VF(vf12, vf10);
            for (j = 0; j < 3; j++, vertices++, colors++) {
                height += heightStep;
                colors[0] = color;
                colors[3] = color;
                angle += sdfAtan2Poly(halfWidth[0] / radius);
                next[0] = sdfSinPoly(angle) * radius;
                next[1] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius + height;
                next[2] = 0.0f;
                VU0_LOAD_VF(vf10, next);
                VU0_LOAD_VF(vf11, position);
                VU0_SUB(vf10, vf10, vf11);
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                VU0_LOAD_VF(vf11, halfWidth);
                VU0_MUL(vf10, vf10, vf11);
                VU0_ADD(vf10, vf10, vf12);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, position);
                VU0_LOAD_VF(vf10, normal);
                VU0_LOAD_VF(vf11, halfHeight);
                VU0_MUL(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, side);
                VU0_LOAD_VF(vf10, position);
                VU0_LOAD_VF(vf11, side);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, vertices);
                VU0_SUB(vf10, vf10, vf11);
                VU0_SUB(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, vertices + 3);
                if (j == 1) {
                    PCP_COPY_VECTOR(duplicatePosition, position);
                }
                PCP_COPY_VECTOR(position, next);
            }
            if (age < duration) {
                f32 tiltLimit = particle->tiltHalfAngle * 0.5f;

                particle->tiltAngle += tiltStep;
                particle->height += heightStep;
                if (ffabsf(particle->tiltAngle) > tiltLimit) {
                    particle->tiltAngle -= tiltStep;
                    particle->tiltStep = -particle->tiltStep;
                }
            }
        } else {
            colors[0] = 0;
            colors[1] = 0;
            colors[2] = 0;
            colors[3] = 0;
            colors[4] = 0;
            colors[5] = 0;
        }
        if (respawn && age >= endAge) {
            particle->age = -1;
        }
        if (duplicates && age >= duplicateStart && age >= 0 && i % perGroup == 0) {
            u32 group = i / perGroup;
            effParamWorkCallback3(work->duplicatedHandles[group], baseColor);
            VU0_LOAD_VF(vf10, work->params.origin);
            VU0_LOAD_VF(vf11, duplicatePosition);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, duplicatePosition);
            effParamWorkCallback0(work->duplicatedHandles[group], duplicatePosition);
            effParamWorkInvokeCallback(work->duplicatedHandles[group]);
        }
        particle->age++;
    }
    work->childWork->origin[0] = work->params.origin[0];
    work->childWork->origin[1] = work->params.origin[1];
    work->childWork->origin[2] = work->params.origin[2];
    effPcpScatterDrawPool(work->childWork);
}


/* Copy the ribbon variant's packed parameter vector without rebuilding components. */
void effScatterCopyRibbonVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Replace the ribbon work's packed color. */
void effScatterSetRibbonColor(PcpScatterRibbonWork *work, u32 color) {
    work->color = color;
}

void func_0017A8B8(void) {
}

/* Return the trailing control block after zeroing two record arrays.
 * Signed group arithmetic and allocation sizes are deliberately not validated here. */
PcpScatterPool *effPcpScatterPoolCreate(s32 groups) {
    PcpScatterPool *pool;
    SdfMemBlock *allocation;
    u32 *recordBlock;
    s32 slotCount;
    s32 recordWords;
    s32 auxWords;
    u32 allocationBytes;

    slotCount = groups * EFF_SCATTER_POOL_SLOTS_PER_GROUP;
    recordWords = slotCount * EFF_SCATTER_POOL_RECORD_WORDS_PER_SLOT;
    auxWords = slotCount * EFF_SCATTER_POOL_AUX_WORDS_PER_SLOT;
    allocationBytes = (recordWords + auxWords) * EFF_SCATTER_WORD_BYTES + EFF_SCATTER_POOL_CONTROL_BYTES;
    allocation = sdfAllocGeneralBlock(allocationBytes);
    recordBlock = (u32 *)sdfResourceRetainAddress(allocation);
    memset(recordBlock, 0, allocationBytes);
    pool = (PcpScatterPool *)(recordBlock + (recordWords + auxWords));
    pool->recordBase = (u8 *)recordBlock;
    pool->unk10 = 1;
    pool->auxRecordBase = (u8 *)(recordBlock + recordWords);
    pool->secondWordCount = auxWords;
    pool->allocation = allocation;
    pool->unk1C = 1.0f;
    pool->color = EFF_SCATTER_NEUTRAL_COLOR;
    pool->sharedResource = 0;
    pool->drawAsset = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat(pool->drawAsset, 1.0f);
    memset(D_00452020, 0, EFF_SCATTER_DRAW_TEMPLATE_BYTES);
    D_00452020->primitive = 0x4000;
    return pool;
}

/* Drop the optional texture-owner reference, queue the draw asset, then free the pool. */
void effPcpScatterReleasePoolResources(PcpScatterPool *pool) {
    if (pool->sharedResource != NULL) {
        effPcpScatterResRelease(pool->sharedResource);
    }
    sdfQueueAssetRelease(pool->drawAsset);
    sdfReleaseResourceAllocation(pool->allocation);
}

/* Submit six-vertex scatter groups using the optional shared texture owner. */
void effPcpScatterDrawPool(PcpScatterPool *pool) {
    f32 matrix[16];
    SdfListHead *packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    s32 remainingVertices;
    SdfPoolNode *surface;

    sdfInitPacketList(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(packet, 0);
    if (pool->sharedResource != NULL) {
        sdfSetAssetPrimaryTextureAddress(pool->drawAsset, pool->sharedResource->textureHandle);
        D_00452020->texcoords = D_003B13F0;
    } else {
        D_00452020->texcoords = NULL;
    }
    sdfConsAppendAssetPacket(packet, pool->drawAsset, 0);
    remainingVertices = pool->secondWordCount;
    D_00452020->colors = (u32 *)pool->auxRecordBase;
    D_00452020->positions = (u128 *)pool->recordBase;
    D_00452020->color = pool->color;
    D_00452020->parameterCount = 0x10;
    D_00452020->vertexCount = 24;
    D_00452020->parameters = D_003B13A0;
    while (remainingVertices >= 24) {
        remainingVertices -= 24;
        sdfAppendPacket(packet, func_00167A10(D_00452020));
        D_00452020->positions += 24;
        D_00452020->colors += 24;
    }
    if (remainingVertices >= 6) {
        D_00452020->parameterCount = remainingVertices / 6 * 4;
        D_00452020->vertexCount = remainingVertices;
        sdfAppendPacket(packet, func_00167A10(D_00452020));
    }
    surface = D_003B14B0[pool->unk10];
    surface->append((SdfListHead *)surface, packet);
}

/* Acquire a new texture owner and store it in the pool. */
void effPcpScatterCreatePoolResource(PcpScatterPool *pool, u32 resId) {
    PcpScatterRes *resource;

    resource = effPcpScatterResCreate(resId);
    pool->sharedResource = resource;
}

/* Take a reference to the source pool's texture owner; no null guard is added. */
void effPcpScatterSharePoolResource(PcpScatterPool *dst, PcpScatterPool *src) {
    PcpScatterRes *resource;

    resource = effPcpScatterResAddRef(src->sharedResource);
    dst->sharedResource = resource;
}

/* Return an unchecked byte address for the indexed primary record. */
u128 *effPcpScatterGetRecordAddress(PcpScatterPool *pool, s32 index) {
    return (u128 *)(pool->recordBase + index * EFF_SCATTER_RECORD_BYTES);
}

/* Return an unchecked byte address for the indexed auxiliary record. */
u32 *effPcpScatterGetAuxRecordAddress(PcpScatterPool *pool, s32 index) {
    return (u32 *)(pool->auxRecordBase + index * EFF_SCATTER_AUX_RECORD_BYTES);
}

/* Return a texture owner with one reference and its acquired texture handle. */
PcpScatterRes *effPcpScatterResCreate(u32 resId)
{
    PcpScatterRes *res;

    res = sdfAllocSizeClassBlock(EFF_SCATTER_RES_BYTES);
    res->textureHandle = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(resId));
    res->refCount = 1;
    return res;
}

extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);


/* Release the texture and owner only when the decremented reference count equals zero. */
void effPcpScatterResRelease(PcpScatterRes *res) {
    res->refCount--;
    if (res->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(res->textureHandle);
        sdfReleaseChipBlock(res);
    }
}

/* Retain the texture owner and return the same pointer. */
PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *resource) {
    resource->refCount = resource->refCount + 1;
    return resource;
}

/* The allocator stores 2 * segmentsPerParticle + 2 vectors for each particle. */
extern PcpScatterDraw *effScatterCreateDrawObject(
    u32 particleCount, u32 segmentsPerParticle);

extern void effCreateScatterResource(PcpScatterDraw *object, u32 resource);

extern u32 effMiscRand(void *state);

extern void *memcpy(void *, const void *, u32);

/* Return ring work with trailing particles and randomized negative initial ages.
 * The delay clamp changes only the local modulus, not the copied parameter head. */
PcpScatterInstanceA *effPcpScatterCreateParticleInstance(src, resource)
    PcpScatterParamsA *src;
    u32 resource;
{
    SdfMemBlock *allocation = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpScatterParticle) + sizeof(PcpScatterInstanceA));
    PcpScatterInstanceA *inst = (PcpScatterInstanceA *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 delayModulus;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
    u32 drawWord;

    particle = (PcpScatterParticle *)(inst + 1);
    /* Preserve the complete serialized parameter block, including padding. */
    memcpy(&inst->params, src, sizeof(inst->params));
    inst->color = EFF_SCATTER_NEUTRAL_COLOR;
    inst->scale = 1.0f;
    inst->allocationHandle = allocation;
    inst->particles = particle;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = effScatterCreateDrawObject(src->particleCount, src->segmentsPerParticle);
    drawWord = src->packetQueueIndex;
    inst->scatterObject = object;
    object->packetQueueIndex = drawWord;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    delayModulus = inst->params.randomDelayRange;
    count = inst->params.particleCount;
    if ((s32)delayModulus <= 0) {
        delayModulus = 1;
    }
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(effDefaultRandomState) % delayModulus);
        particle++;
    }
    return inst;
}

/* Return ring work created from the first two parameter-table blocks. */
PcpScatterInstanceA *effScatterCreateRingFromTable(void *parameterTable) {
    void *particleParams = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    u32 resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);

    return effPcpScatterCreateParticleInstance(particleParams, resource);
}

/* Return newly initialized ring work sharing the source drawable's resource. */
PcpScatterInstanceA *effScatterCloneWithSharedObject(PcpScatterInstanceA *work) {
    PcpScatterInstanceA *child;

    child = effPcpScatterCreateParticleInstance(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

/* Release the drawable before its owning SDF allocation descriptor. */
void effScatterReleaseObjectAndBuffer(PcpScatterInstanceA *work) {
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation(work->allocationHandle);
}




/* Seed ring index and its first vertex/UV pairs, retaining independent jitter samples.
 * Pair-count and duration divisors are unchecked; keep XYZ-only vertex setup. */
void effScatterRingInit(PcpScatterInstanceA *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    f32 *uv = effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 rise;
    f32 riseStep;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    f32 s;
    u32 count;
    u32 i;

    ring = &work->particles[index];
    count = work->scatterObject->vectorsPerParticle >> 1;
    angle = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_SCATTER_HALF_TURN * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->params.riseStep;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.targetRadiusJitter;
    rise = 0.0f;
    ring->radiusStep = (work->params.targetRadius * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) - radius) / (f32)work->params.duration;
    ring->orientationAngle = work->params.tiltScale * effMiscRandUnitFloat(effDefaultRandomState);
    ring->tiltAngle = rise;
    ring->angle = angle;
    ring->radius = radius;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_003364B8(ring->orientationAngle);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    v = (f32)work->params.vSpan;
    u = 0.0f;
    du = (f32)work->params.uSpan / (f32)count;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] = vertex[1] - height;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        rise += riseStep;
        u += du;
        angle += angleStep;
        vertex += 8;
    }
}

/* Advance ring index and rebuild its paired vertices in native matrix order.
 * Keep the narrow-block lookup even though its returned pointer is unused. */
void effScatterRingUpdate(PcpScatterInstanceA *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    radius = ring->radius + ring->radiusStep;
    count = work->scatterObject->vectorsPerParticle >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_003364B8(ring->orientationAngle);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->params.tiltDamping;
    ring->radius = radius;
    ring->rise = rise * work->params.riseDecay;
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] += rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] += rise;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}

extern u32 effMultiplyPackedColors(u32 colorA, u32 colorB);
extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void effScatterDrawObject(PcpScatterDraw *object);
/* Advance delayed particles and submit the ring drawable.
 * Age zero seeds geometry without writing color; expired particles clear color and freeze.
 * Per-particle fade-in takes precedence over fade-out; duration equality still processes. */
void effScatterUpdateLoopedParticleRing(PcpScatterInstanceA *work) {
    s32 loop;
    u32 i;
    u32 count = work->params.particleCount;
    PcpScatterDraw *draw = work->scatterObject;
    PcpScatterParticle *particle = work->particles;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeRange;
    s32 color;
    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    color = effMultiplyPackedColors(work->color, work->params.baseColor);

    for (i = 0; i < count; i++) {
        s32 age = particle->age;
        if (duration < age) {
            draw->colors[i] = 0;
        } else {
            if (age == 0) {
                effScatterRingInit(work, i);
            } else if (age > 0) {
                s32 remaining = duration - age;
                f32 factor;
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / (f32)fadeIn;
                } else if (fadeRange >= remaining && fadeRange != 0) {
                    factor = (f32)remaining / (f32)fadeRange;
                } else {
                    factor = 1.0f;
                }
                draw->colors[i] = effBlendColor(color & EFF_SCATTER_RGB_MASK, color, factor);
                effScatterRingUpdate(work, i);
            }
            if (loop != 0 && age >= duration) {
                particle->age = 0;
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw->origin, work->params.origin);
    effScatterStoreSourceTransformMatrix(draw, work);
    effScatterDrawObject(draw);
}

/* Replace the packed origin at the start of the copied parameters. */
void effScatterCopyParticleParameterVector(PcpScatterInstanceA *work, void *source) {
    PCP_COPY_VECTOR(&work->params, source);
}

/* Set the drawable scale multiplier; unlike radius rescaling, this is an assignment. */
void effScatterSetParticleScale(PcpScatterInstanceA *work, f32 scale)
{
    work->scale = scale;
}

/* Set the packed tint multiplied with the copied base color during updates. */
void effScatterSetParticleColor(PcpScatterInstanceA *work, u32 color) {
    work->color = color;
}

/* vu0 routine: compose params.matrix and source via the native product into work->matrix. */
void effPcpScatterTransformMatrix(PcpScatterInstanceA *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}


/* Return the radius-damped ring variant with a shared instance clock.
 * Normalize the copied delay range too, because loop restarts read that stored value. */
/* Return the radius-damped ring variant with a shared instance clock.
 * Normalize the copied delay range too, because loop restarts read that stored value. */
PcpScatterInstanceB *effScatterCreateDampedRing(src, resource)
    PcpScatterParamsB *src;
    u32 resource;
{
    SdfMemBlock *allocation = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpScatterParticle) + sizeof(PcpScatterInstanceB));
    PcpScatterInstanceB *inst = (PcpScatterInstanceB *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 delayModulus;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
    u32 drawWord;
    s32 delayLimit;

    particle = (PcpScatterParticle *)(inst + 1);
    /* Retain every byte of the serialized parameter block, including padding. */
    memcpy(&inst->params, src, sizeof(inst->params));
    inst->color = EFF_SCATTER_NEUTRAL_COLOR;
    inst->scale = 1.0f;
    inst->allocationHandle = allocation;
    inst->particles = particle;
    inst->age = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = effScatterCreateDrawObject(src->particleCount, src->segmentsPerParticle);
    drawWord = src->packetQueueIndex;
    inst->scatterObject = object;
    object->packetQueueIndex = drawWord;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    delayLimit = inst->params.randomDelayRange;
    if (delayLimit <= 0) {
        inst->params.randomDelayRange = 1;
        delayLimit = 1;
    }
    delayModulus = delayLimit;
    count = inst->params.particleCount;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(effDefaultRandomState) % delayModulus);
        particle++;
    }
    return inst;
}

/* Create radius-damped ring work from its parameter and resource table blocks. */
void effScatterSpawnFromParameterPair(void *parameterTable) {
    void *particleParams;
    u32 resource;

    particleParams = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);
    effScatterCreateDampedRing(particleParams, resource);
}

/* Return freshly initialized radius-damped ring work sharing the source resource. */
PcpScatterInstanceB *effScatterCloneWithSharedResource(PcpScatterInstanceB *work)
{
    PcpScatterInstanceB *child;

    child = effScatterCreateDampedRing(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

/* Release the radius-damped drawable before its owning allocation node. */
void effScatterReleaseInstanceResources(PcpScatterInstanceB *work) {
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation(work->allocationHandle);
}


/* Seed ring index with sampled radius velocity, orientation and initial UV pairs.
 * Preserve RNG order and unchecked pair-count division rather than folding samples. */
void effScatterRingInitScaled(PcpScatterInstanceB *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    f32 *uv = effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 rise;
    f32 riseStep;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    f32 s;
    u32 count;
    u32 i;

    ring = &work->particles[index];
    count = work->scatterObject->vectorsPerParticle >> 1;
    angle = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_SCATTER_HALF_TURN * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) / (f32)count;
    riseStep = work->params.riseStep;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.radiusStepJitter;
    rise = 0.0f;
    ring->radiusStep = work->params.radiusStepBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    ring->orientationAngle = work->params.tiltScale * effMiscRandUnitFloat(effDefaultRandomState);
    ring->tiltAngle = rise;
    ring->angle = angle;
    ring->radius = radius;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_003364B8(ring->orientationAngle);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    v = (f32)work->params.vSpan;
    u = 0.0f;
    du = (f32)work->params.uSpan / (f32)count;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] = vertex[1] - height;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        rise += riseStep;
        u += du;
        angle += angleStep;
        vertex += 8;
    }
}

/* Advance radius-damped ring index, including damping its stored radius velocity.
 * The native unused narrow-block lookup remains part of this sequence. */
void effScatterRingUpdateScaled(PcpScatterInstanceB *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    radius = ring->radius + ring->radiusStep;
    count = work->scatterObject->vectorsPerParticle >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_003364B8(ring->orientationAngle);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->params.tiltDamping;
    ring->radius = radius;
    ring->radiusStep *= work->params.radiusDamping;
    ring->rise = rise * work->params.riseDecay;
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] += rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] += rise;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}




/* Combine particle fade-in with instance-clock fade-out, then submit the drawable.
 * An expired instance returns without submission. Age zero initializes geometry only;
 * at the instance duration boundary, looping reseeds particle delays from the stored range. */
void effScatterUpdateLoopedScaledRing(PcpScatterInstanceB *work) {
    s32 loop;
    s32 duration = work->params.duration;
    PcpScatterDraw *draw = work->scatterObject;
    PcpScatterParticle *particle = work->particles;
    u32 count = work->params.particleCount;
    s32 fadeIn;
    s32 fadeRange;
    u32 delayModulus;
    s32 instanceAge;
    s32 color;
    s32 remaining;
    f32 instanceFade;
    f32 particleFade;
    u32 i;

    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    delayModulus = work->params.randomDelayRange;
    color = effMultiplyPackedColors(work->color, work->params.baseColor);
    instanceAge = work->age;
    if (duration < instanceAge) {
        return;
    }
    remaining = duration - instanceAge;
    if (fadeRange >= remaining && fadeRange != 0) {
        instanceFade = (f32)remaining / (f32)fadeRange;
    } else {
        instanceFade = 1.0f;
    }
    for (i = 0; i < count; i++) {
        s32 particleAge = particle->age;

        if (duration < particleAge) {
            draw->colors[i] = 0;
        } else {
            if (particleAge == 0) {
                effScatterRingInitScaled(work, i);
            } else if (particleAge > 0) {
                if (particleAge < fadeIn && fadeIn != 0) {
                    particleFade = (f32)particleAge / (f32)fadeIn;
                } else {
                    particleFade = 1.0f;
                }
                draw->colors[i] = effBlendColor(color & EFF_SCATTER_RGB_MASK, color, particleFade * instanceFade);
                effScatterRingUpdateScaled(work, i);
            }
            if (loop != 0 && !(instanceAge < duration)) {
                particle->age = -(effMiscRand(effDefaultRandomState) % delayModulus);
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    if (loop != 0 && instanceAge >= duration) {
        work->age = 0;
    } else {
        work->age++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw->origin, work->params.origin);
    effScatterStoreSourceTransformMatrix(draw, work);
    effScatterDrawObject(draw);
}

/* Replace the radius-damped ring's packed parameter origin. */
void effScatterSetScaledRingOrigin(PcpScatterInstanceB *work, void *source) {
    PCP_COPY_VECTOR(&work->params, source);
}

/* Assign the radius-damped instance's drawable scale multiplier. */
void effScatterSetInstanceScale(PcpScatterInstanceB *work, f32 scale)
{
    work->scale = scale;
}

/* Assign the tint multiplied with the radius-damped ring's base color. */
void effScatterSetInstanceColor(PcpScatterInstanceB *work, u32 color) {
    work->color = color;
}

/* vu0 routine: compose params.matrix and source via the native product into work->matrix. */
void effScatterComposeWorkMatrix(PcpScatterInstanceB *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}


/* Return two-color ring work with an instance clock and normalized stored delay range. */
PcpScatterInstanceC *effScatterCreateTwoColorRing(src, resource)
    PcpScatterParamsC *src;
    u32 resource;
{
    SdfMemBlock *allocation = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpScatterParticle) + sizeof(PcpScatterInstanceC));
    PcpScatterInstanceC *inst = (PcpScatterInstanceC *)sdfResourceRetainAddress(allocation);
    PcpScatterParticle *particle;
    u32 delayModulus;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
    u32 drawWord;
    s32 delayLimit;

    particle = (PcpScatterParticle *)(inst + 1);
    /* Copy the complete serialized parameter block, including padding. */
    memcpy(&inst->params, src, sizeof(inst->params));
    inst->color = EFF_SCATTER_NEUTRAL_COLOR;
    inst->scale = 1.0f;
    inst->allocationHandle = allocation;
    inst->particles = particle;
    inst->age = 0;
    VU0_COPY_MATRIX(inst->matrix, src->matrix);
    object = effScatterCreateDrawObject(src->particleCount, src->segmentsPerParticle);
    drawWord = src->packetQueueIndex;
    inst->scatterObject = object;
    object->packetQueueIndex = drawWord;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    delayLimit = inst->params.randomDelayRange;
    if (delayLimit <= 0) {
        inst->params.randomDelayRange = 1;
        delayLimit = 1;
    }
    delayModulus = delayLimit;
    count = inst->params.particleCount;
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(effDefaultRandomState) % delayModulus);
        particle++;
    }
    return inst;
}

/* Create two-color ring work from its parameter and resource table blocks. */
void effScatterCreateFromParameterTable(void *parameterTable) {
    void *particleParams;
    u32 resource;

    particleParams = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);
    effScatterCreateTwoColorRing(particleParams, resource);
}

/* Return freshly initialized two-color ring work sharing the source resource. */
PcpScatterInstanceC *effCreateScatterChildSharingParentResource(PcpScatterInstanceC *work)
{
    PcpScatterInstanceC *child;

    child = effScatterCreateTwoColorRing(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

/* Release the two-color drawable before its owning allocation node. */
void effReleaseScatterObjectAndOwnedBuffer(PcpScatterInstanceC *work) {
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation(work->allocationHandle);
}


/* Seed ring index with index-dependent rise/radius and sampled motion, then vertex/UV pairs.
 * The UV extents retain this variant's native member order and unsigned conversions. */
void effScatterInitStaggeredRing(PcpScatterInstanceC *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    f32 *uv = effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 rise;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    f32 s;
    u32 count;
    u32 i;

    ring = &work->particles[index];
    count = work->scatterObject->vectorsPerParticle >> 1;
    angle = effMiscRandUnitFloat(effDefaultRandomState) * (EFF_SCATTER_HALF_TURN * 2.0f);
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) / (f32)count;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.heightOffsetJitter;
    height = work->params.heightOffsetBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.radiusStepJitter;
    ring->radiusStep = work->params.radiusStepBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    ring->orientationAngle = work->params.tiltScale * effMiscRandUnitFloat(effDefaultRandomState);
    ring->tiltAngle = 0;
    rise = work->params.riseRange / (f32)work->params.particleCount * (f32)index;
    radius = radius + work->params.radiusRamp * (f32)(work->params.particleCount - index);
    ring->angle = angle;
    ring->heightOffset = height;
    ring->angleStep = angleStep;
    ring->radius = radius;
    ring->tiltSpeed = work->params.initialTiltSpeed;
    ring->rise = work->params.initialRise;
    func_003364B8(ring->orientationAngle);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    v = (f32)work->params.vSpan;
    u = 0.0f;
    du = (f32)work->params.uSpan / (f32)count;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = rise;
        s = sdfSinPoly(angle) * radius;
        vertex[4] = vertex[0];
        vertex[6] = vertex[2] = s;
        vertex[5] = vertex[1] - height;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        angle += angleStep;
        u += du;
        vertex += 8;
    }
}


/* Advance the two-color variant's radius-damped geometry in native matrix order.
 * Keep the otherwise unused narrow-block lookup and XYZ-only vertex construction. */
void effScatterRingUpdateScaledLong(PcpScatterInstanceC *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterParticle *ring;
    f32 radius;
    f32 rise;
    f32 angle;
    f32 step;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    radius = ring->radius + ring->radiusStep;
    count = work->scatterObject->vectorsPerParticle >> 1;
    rise = ring->rise;
    angle = ring->angle;
    step = ring->angleStep;
    func_003364B8(ring->orientationAngle);
    func_00336818(ring->tiltAngle);
    sdfMultiplyVuMatrixInPlace();
    ring->tiltAngle += ring->tiltSpeed;
    ring->tiltSpeed *= work->params.tiltDamping;
    ring->radius = radius;
    ring->radiusStep *= work->params.radiusDamping;
    ring->rise = rise * work->params.riseDecay;
    for (i = 0; i < count; i++) {
        f32 s;

        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] += rise;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] += rise;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}


/* Blend start/end colors by particle age, apply tint, then particle/instance fading.
 * Keep this order and age-zero geometry-only initialization. Expired instances do not submit;
 * looping uses the instance boundary to reseed particle delays, not each particle's boundary. */
void effScatterUpdateTwoColor(PcpScatterInstanceC *work) {
    s32 loop;
    s32 duration = work->params.duration;
    PcpScatterDraw *draw = work->scatterObject;
    PcpScatterParticle *particle = work->particles;
    u32 count = work->params.particleCount;
    s32 fadeIn;
    s32 fadeRange;
    u32 delayModulus;
    s32 instanceAge;
    s32 remaining;
    f32 instanceFade;
    f32 particleFade;
    f32 particleAgeFloat;
    s32 tintColor;
    s32 startColor;
    s32 endColor;
    s32 color;
    u32 i;

    instanceAge = work->age;
    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    delayModulus = work->params.randomDelayRange;
    startColor = work->params.startColor;
    endColor = work->params.endColor;
    tintColor = work->color;
    if (duration < instanceAge) {
        return;
    }
    remaining = duration - instanceAge;
    if (fadeRange >= remaining && fadeRange != 0) {
        instanceFade = (f32)remaining / (f32)fadeRange;
    } else {
        instanceFade = 1.0f;
    }
    for (i = 0; i < count; i++) {
        s32 particleAge = particle->age;

        if (duration < particleAge) {
            draw->colors[i] = 0;
        } else {
            if (particleAge == 0) {
                effScatterInitStaggeredRing(work, i);
            } else if (particleAge > 0) {
                particleAgeFloat = (f32)particleAge;
                color = effBlendColor(startColor, endColor, particleAgeFloat / (f32)duration);
                if (particleAge < fadeIn && fadeIn != 0) {
                    particleFade = particleAgeFloat / (f32)fadeIn;
                } else {
                    particleFade = 1.0f;
                }
                color = effMultiplyPackedColors(tintColor, color);
                draw->colors[i] = effBlendColor(color & EFF_SCATTER_RGB_MASK, color, particleFade * instanceFade);
                effScatterRingUpdateScaledLong(work, i);
            }
            if (loop != 0 && !(instanceAge < duration)) {
                particle->age = -(effMiscRand(effDefaultRandomState) % delayModulus);
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    if (loop != 0 && instanceAge >= duration) {
        work->age = 0;
    } else {
        work->age++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw->origin, work->params.origin);
    effScatterStoreSourceTransformMatrix(draw, work);
    effScatterDrawObject(draw);
}

/* Replace the two-color ring's packed parameter origin. */
void effScatterSetDualColorOrigin(PcpScatterInstanceC *work, void *source) {
    PCP_COPY_VECTOR(&work->params, source);
}

/* Assign the two-color instance's drawable scale multiplier. */
void effSetScatterWorkScale(PcpScatterInstanceC *work, f32 scale)
{
    work->scale = scale;
}

/* Assign the tint applied after start/end color interpolation. */
void effSetScatterWorkColor(PcpScatterInstanceC *work, u32 color) {
    work->color = color;
}

/* vu0 routine: compose params.matrix and source via the native product into work->matrix. */
void effScatterComposeParticleMatrix(PcpScatterInstanceC *work, void *source) {
    VU0_LOAD_MATRIX(source);
    VU0_LOAD_MATRIX_B(work->params.matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(work);
}



/* Return flat-ring work with an identity source matrix and randomized negative ages.
 * Like the first ring variant, only the local delay modulus is normalized. */
PcpScatterPlainInstance *effPcpScatterCreatePlainInstance(src, resource)
    PcpScatterPlainParams *src;
    u32 resource;
{
    SdfMemBlock *allocation = sdfAllocGeneralBlock(src->particleCount * sizeof(PcpScatterPlainParticle) + sizeof(PcpScatterPlainInstance));
    PcpScatterPlainInstance *inst = (PcpScatterPlainInstance *)sdfResourceRetainAddress(allocation);
    PcpScatterPlainParticle *particle;
    u32 delayModulus;
    u32 count;
    u32 i;
    PcpScatterDraw *object;
    u32 drawWord;

    particle = (PcpScatterPlainParticle *)(inst + 1);
    /* Preserve the complete serialized parameter block, including padding. */
    memcpy(&inst->params, src, sizeof(inst->params));
    inst->color = EFF_SCATTER_NEUTRAL_COLOR;
    inst->scale = 1.0f;
    inst->allocationHandle = allocation;
    inst->particles = particle;
    EE_MMI_UNIT_MATRIX(inst->matrix);
    object = effScatterCreateDrawObject(src->particleCount, src->segmentsPerParticle);
    drawWord = src->packetQueueIndex;
    inst->scatterObject = object;
    object->packetQueueIndex = drawWord;
    if (resource != 0) {
        effCreateScatterResource(object, resource);
    }
    delayModulus = inst->params.randomDelayRange;
    count = inst->params.particleCount;
    if ((s32)delayModulus <= 0) {
        delayModulus = 1;
    }
    for (i = 0; i < count; i++) {
        particle->age = -(effMiscRand(effDefaultRandomState) % delayModulus);
        particle++;
    }
    return inst;
}

/* Create flat-ring work from its parameter and resource table blocks. */
void effScatterCreatePlainRingFromTable(void *parameterTable) {
    void *particleParams;
    u32 resource;

    particleParams = effParamTableGetBlock(parameterTable, EFF_SCATTER_PARAM_BLOCK);
    resource = (u32)effParamTableGetBlock(parameterTable, EFF_SCATTER_RESOURCE_BLOCK);
    effPcpScatterCreatePlainInstance(particleParams, resource);
}

/* Return freshly initialized flat-ring work sharing the source resource. */
PcpScatterPlainInstance *effCloneScatterWithSharedResource(PcpScatterPlainInstance *work)
{
    PcpScatterPlainInstance *child;

    child = effPcpScatterCreatePlainInstance(&work->params, NULL);
    effShareScatterResource(child->scatterObject, work->scatterObject);
    return child;
}

/* Release the flat-ring drawable before its owning allocation node. */
void effReleaseScatterWorkResources(PcpScatterPlainInstance *work) {
    effReleaseScatterObject(work->scatterObject);
    sdfReleaseResourceAllocation(work->allocationHandle);
}

/* Seed a flat-ring particle and its paired UVs with independent samples. */
void effScatterCreateFlatRing(PcpScatterPlainInstance *work, s32 index) {
    f32 *uv = effGetScatterNarrowBlock(work->scatterObject, index);
    PcpScatterPlainParticle *ring;
    f32 angle;
    f32 angleStep;
    f32 radius;
    f32 height;
    f32 u;
    f32 du;
    f32 v;
    f32 jitter;
    u32 count;
    u32 i;

    ring = &work->particles[index];
    count = work->scatterObject->vectorsPerParticle >> 1;
    angle = effMiscRandUnitFloat(effDefaultRandomState) * EFF_SCATTER_RADIAL_TURN;
    jitter = work->params.angleStepJitter;
    angleStep = work->params.angleStepBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter)) / (f32)count;
    jitter = work->params.radiusJitter;
    radius = work->params.radiusBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    jitter = work->params.heightJitter;
    height = work->params.heightBase * (effMiscRandUnitFloat(effDefaultRandomState) * jitter + (1.0f - jitter));
    /* Euler samples use the retail constant's lower-rounded full turn. */
    ring->rot[0] = effMiscRandUnitFloat(effDefaultRandomState) * EFF_SCATTER_RIBBON_TURN;
    ring->rot[1] = effMiscRandUnitFloat(effDefaultRandomState) * EFF_SCATTER_RIBBON_TURN;
    ring->rot[2] = effMiscRandUnitFloat(effDefaultRandomState) * EFF_SCATTER_RIBBON_TURN;
    ring->angle = angle;
    ring->radius = radius;
    ring->height = height;
    ring->angleStep = angleStep;
    ring->angularSpeed = work->params.angularSpeed;
    ring->radialSpeed = work->params.radialSpeed;
    v = (f32)work->params.vSpan;
    u = 0.0f;
    du = (f32)work->params.uSpan / (f32)count;
    for (i = 0; i < count; i++) {
        uv[0] = u;
        uv[2] = u;
        uv[1] = 0;
        uv[3] = v;
        uv += 4;
        u += du;
    }
}


/* Advance flat-ring index and rebuild paired vertices around its own angles.
 * Preserve the narrow-block lookup and the native XYZ-only setup. */
void effScatterFlatRingUpdate(PcpScatterPlainInstance *work, s32 index)
{
    f32 *vertex = effGetScatterWideBlock(work->scatterObject, index);
    PcpScatterPlainParticle *ring;
    f32 angle;
    f32 radius;
    f32 step;
    f32 height;
    f32 s;
    u32 count;
    u32 i;

    effGetScatterNarrowBlock(work->scatterObject, index);
    ring = &work->particles[index];
    count = work->scatterObject->vectorsPerParticle >> 1;
    angle = ring->angle + ring->angularSpeed;
    radius = ring->radius;
    step = ring->angleStep;
    radius += ring->radialSpeed;
    if (ring->age >= work->params.radialDecayStart) {
        ring->radialSpeed *= work->params.radialDamping;
    }
    vu0RotMatrixXYZFromVec3(ring->rot);
    ring->radius = radius;
    ring->angle = angle;
    ring->angularSpeed *= work->params.angularDamping;
    height = ring->height;
    for (i = 0; i < count; i++) {
        vertex[0] = sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        vertex[1] = 0;
        s = sdfSinPoly(angle);
        vertex[4] = vertex[0];
        vertex[5] = -height;
        vertex[6] = vertex[2] = s * radius;
        VU0_LOAD_VF(vf10, vertex);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex);
        VU0_LOAD_VF(vf10, vertex + 4);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, vertex + 4);
        angle += step;
        vertex += 8;
    }
}


/* Advance delayed flat rings with per-particle lifetime/fading, then submit.
 * Age zero seeds geometry but not color; expired particles clear color and stop aging.
 * Fade-in wins overlapping fade-out, and duration equality is processed before loop reset. */
void effScatterUpdatePlainParticleRing(PcpScatterPlainInstance *work) {
    s32 loop;
    u32 i;
    u32 count = work->params.particleCount;
    PcpScatterDraw *draw = work->scatterObject;
    PcpScatterPlainParticle *particle = work->particles;
    s32 duration = work->params.duration;
    s32 fadeIn;
    s32 fadeRange;
    s32 color;
    loop = work->params.loop;
    fadeIn = work->params.fadeIn;
    fadeRange = work->params.fadeRange;
    color = effMultiplyPackedColors(work->color, work->params.baseColor);

    for (i = 0; i < count; i++) {
        s32 age = particle->age;
        if (duration < age) {
            draw->colors[i] = 0;
        } else {
            if (age == 0) {
                effScatterCreateFlatRing(work, i);
            } else if (age > 0) {
                s32 remaining = duration - age;
                f32 factor;
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / (f32)fadeIn;
                } else if (fadeRange >= remaining && fadeRange != 0) {
                    factor = (f32)remaining / (f32)fadeRange;
                } else {
                    factor = 1.0f;
                }
                draw->colors[i] = effBlendColor(color & EFF_SCATTER_RGB_MASK, color, factor);
                effScatterFlatRingUpdate(work, i);
            }
            if (loop != 0 && age >= duration) {
                particle->age = 0;
            } else {
                particle->age++;
            }
        }
        particle++;
    }
    draw->scale = work->scale;
    PCP_COPY_VECTOR(draw->origin, work->params.origin);
    effScatterStoreSourceTransformMatrix(draw, work);
    effScatterDrawObject(draw);
}
