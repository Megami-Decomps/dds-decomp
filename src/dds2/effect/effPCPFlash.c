#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "eff_param.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_quaternion.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_math.h"
#include "eff_pcp_flash.h"
#include "pcp_vu0.h"

/* Work records keep SDF allocation ownership separate from drawable records.
   tintColor multiplies the age-faded packed color; scaleRampTime governs
   scale growth independently of the color fade. */

extern void dds3FlashPolyList2Delete(dds3FlashPolyList_t *pool);
extern void dds3FlashPolyListDelete(dds3FlashPolyList_t *pool);
extern void dds3FlashPolyList3Delete(dds3FlashPolyList_t *pool);
extern void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool);


extern void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *pool, s32 index);
extern f32 *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *pool, s32 index);
extern f32 effPCPFlashPolySphereVertex[];
extern f32 effPCPFlashPolyRingExpVertex[];
extern f32 effPCPFlashPolyRingVertex[];
extern f32 effPCPFlashPolyPipeVertex[];
extern f32 effPCPFlashPolyRingExp2Vertex[];
extern f32 effPCPFlashPolySphereExpVertex[];
extern f32 def_dir[];
extern f32 effPCPFlashPolyCircleVertex[];
extern f32 effPCPFlashPolySphere2Vertex[];
extern f32 *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *pool, s32 index);
extern f32 *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *pool, s32 index);

extern u32 effMagaCylinderCalcColor(u32 color, u32 colorB);


PcpFlashTrianglePulseWork *effPCPFlashPolySphere2CreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolySphere2Create((PcpFlashTrianglePulseParams *)effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashTrianglePulseWork *effPCPFlashPolySphere2Copy(PcpFlashTrianglePulseParams *params) {
    return effPCPFlashPolySphere2Create(params);
}

void effPCPFlashPolySphere2Delete(PcpFlashTrianglePulseWork *work) {
    dds3FlashPolyList2Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphere2SetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphere2SetColor(PcpFlashTrianglePulseWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolySphere2SetScale(PcpFlashTrianglePulseWork *work, f32 value)
{
    work->renderScale = value;
}

/* Two vertices inherit color B, while the third receives color A with full alpha. */
void effPCPFlashPolySphere2PartSetVC(PcpFlashTrianglePulseWork *work, s32 index, s32 color)
{
    PcpFlashColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot *)dds3FlashPolyList2GetVC(work->resourceHandle, index);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->first = effMagaCylinderCalcColor(colorB, color);
    slot->second = effMagaCylinderCalcColor(colorB, color);
    slot->third = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effPCPFlashPolySphere2PartSetVertex(PcpFlashTrianglePulseWork *work, s32 index, const f32 *view)
{
    PcpFlashPulseParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList2GetVertex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->scale;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(base);
    SetRotateMatrixAxis(angle, (const RwV3d *)view);
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphere2Vertex);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SetRotateMatrixAxis(angle + step, (const RwV3d *)view);
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(effPCPFlashPolySphere2Vertex);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
extern void effPCPFlashPolySphere2PartSetVertex(PcpFlashTrianglePulseWork *work, s32 index, const f32 *view);
extern u8 aViewTarget[];
extern u8 sdfViewTargetVector[];
extern void effPCPFlashPolySpherePartSetVertex(PcpFlashStreakWork *work, s32 index, void *view);
extern void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool);




void effPCPFlashPolySphere2Draw(PcpFlashTrianglePulseWork *work) {
    f32 axis[4];
    s32 particleIndex;
    s32 lifetime;
    s32 particleCount;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    PcpFlashPulseParticle *part;
    dds3FlashPolyList_t *handle;

    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        SDF_VU0_SUBVECTOR();;
        SDF_VU0_NORMALIZE();;
    VU0_STORE_VF($vf10, axis);
    lifetime = work->lifetime;
    particleCount = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    tintColor = work->tintColor;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effPCPFlashPolySphere2PartSetVertex(work, particleIndex, axis);
            effPCPFlashPolySphere2PartSetVC(work, particleIndex, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = 0;
                }
                color = 0;
                effPCPFlashPolySphere2PartSetVC(work, particleIndex, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effPCPFlashPolySphere2PartSetVertex(work, particleIndex, axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor);
                effPCPFlashPolySphere2PartSetVC(work, particleIndex, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyList2Draw(handle);
}

extern EffRandState effDefaultRandomState[];
extern dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 cellCount);

PcpFlashStreakWork *effPCPFlashPolySphereCreate(src)
    PcpFlashStreakParams *src;
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(src->particleCount * sizeof(PcpFlashRotatingParticle) + sizeof(PcpFlashStreakWork));
    PcpFlashStreakWork *work = (PcpFlashStreakWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    u32 range;
    u32 particleIndex;

    memcpy(work, src, 0x40);
    work->parts = (PcpFlashRotatingParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyListCreate(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->drawMode;
    range = work->randomRange;
    for (particleIndex = 0; particleIndex < work->particleCount; particleIndex++) {
        work->parts[particleIndex].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
    }
    return work;
}

PcpFlashStreakWork *effPCPFlashPolySphereCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolySphereCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashStreakWork *effPCPFlashPolySphereCopy(PcpFlashStreakParams *src) {
    return effPCPFlashPolySphereCreate(src);
}

void effPCPFlashPolySphereDelete(PcpFlashStreakWork *work) {
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphereSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphereSetColor(PcpFlashStreakWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolySphereSetScale(PcpFlashStreakWork *work, f32 value)
{
    work->renderScale = value;
}

extern void *dds3FlashPolyListGetVC(dds3FlashPolyList_t *pool, s32 index);



/* Alternate the center alpha arrangement for successive streak vertices. */
void effPCPFlashPolySpherePartSetVC(PcpFlashStreakWork *work, s32 index, s32 color) {
    PcpFlashQuadColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashQuadColorSlot *)dds3FlashPolyListGetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(colorB, color);
    slot->color[1] = effMagaCylinderCalcColor(colorB, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

extern EffRandState effDefaultRandomState[];

void effPCPFlashPolySpherePartInit(PcpFlashStreakWork *work, s32 index, void *orientation) {
    PcpFlashRotatingParticle *part = work->parts + index;
    f32 direction[4];
    f32 factor;
    f32 scale;

    direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f;
    /* Two plain quadword loads, no memory clobber: retail keeps `direction`
     * and `orientation` CSE'd across them. */
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(orientation);
    SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();;
    SDF_VU0_STOREVECTOR(direction);
    part->position[0] = direction[0];
    part->position[1] = direction[1];
    part->position[2] = direction[2];
    factor = dds3GetRandFloatA(effDefaultRandomState) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->rotationStep = work->rotationStepRange * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
}

/* vu0 routine: the four corner offsets of a rotating particle's billboard around its scaled position */
void effPCPFlashPolySpherePartSetVertex(PcpFlashStreakWork *work, s32 index, void *view)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyListGetVertex(work->resourceHandle, index);
    f32 center[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scale, size);
    acrossLen = part->acrossSpan * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->upSpan * ratio;
    VEC3_SPLAT(up, upLen);
    center[0] = part->position[0];
    center[1] = part->position[1];
    center[2] = part->position[2];
    SDF_VU0_LOADVECTOR(center);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(across);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(across);
    SDF_VU0_LOADVECTOR(up);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(up);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereVertex);
    SDF_VU0_LOADVECTOR_B(up);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereVertex);
    SDF_VU0_LOADVECTOR_B(across);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
void effPCPFlashPolySpherePartRotate(PcpFlashStreakWork *work, s32 index, void *orientation)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 position[4];

    position[0] = part->position[0];
    position[1] = part->position[1];
    position[2] = part->position[2];
    SetRotateMatrixAxis(part->rotationStep, (const RwV3d *)orientation);
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(position);
    part->position[0] = position[0];
    part->position[1] = position[1];
    part->position[2] = position[2];
}
extern void dds3FlashPolyListDraw(dds3FlashPolyList_t *pool);
extern EffRandState effDefaultRandomState[];
extern u8 aViewTarget[];
extern u8 sdfViewTargetVector[];

void effPCPFlashPolySphereDraw(PcpFlashStreakWork *work) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    PcpFlashRotatingParticle *part;
    dds3FlashPolyList_t *handle;

    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        SDF_VU0_SUBVECTOR();;
    VU0_STORE_VF($vf10, &axis);
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effPCPFlashPolySpherePartInit(work, index, &axis);
            effPCPFlashPolySpherePartSetVertex(work, index, &axis);
            effPCPFlashPolySpherePartSetVC(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
                }
                color = 0;
                effPCPFlashPolySpherePartSetVC(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effPCPFlashPolySpherePartRotate(work, index, &axis);
                effPCPFlashPolySpherePartSetVertex(work, index, &axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor);
                effPCPFlashPolySpherePartSetVC(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyListDraw(handle);
}


PcpFlashScalingOrbitWork *effPCPFlashPolyRingCreate(PcpFlashScalingOrbitParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashOrbitParticle) + sizeof(PcpFlashScalingOrbitWork));
    PcpFlashScalingOrbitWork *work = (PcpFlashScalingOrbitWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, params, 0x48);
    work->parts = (PcpFlashOrbitParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = dds3FlashPolyListCreate(work->particleCount);
    record->scale = 1.0f;
    record->drawMode = work->drawMode;
    work->resourceHandle = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
        work->parts[i].angle = angle;
        angle += step;
    }
    return work;
}

PcpFlashScalingOrbitWork *effPCPFlashPolyRingCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyRingCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashScalingOrbitWork *effPCPFlashPolyRingCopy(PcpFlashScalingOrbitParams *params)
{
    return effPCPFlashPolyRingCreate(params);
}

void effPCPFlashPolyRingDelete(PcpFlashScalingOrbitWork *work) {
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyRingSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyRingSetColor(PcpFlashScalingOrbitWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolyRingSetScale(PcpFlashScalingOrbitWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyRingPartSetVC(PcpFlashScalingOrbitWork *work, s32 index, s32 color) {
    PcpFlashQuadColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashQuadColorSlot *)dds3FlashPolyListGetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(colorB, color);
    slot->color[1] = effMagaCylinderCalcColor(colorB, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effPCPFlashPolyRingPartSetVertex(PcpFlashScalingOrbitWork *work, s32 index)
{
    PcpFlashOrbitParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyListGetVertex(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scaleA, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = sdfCos(part->angle);
    unit[1] = 0;
    sinv = sdfSin(part->angle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyRingVertex[0] = unit[0] * height;
    effPCPFlashPolyRingVertex[1] = height + -1.0f;
    effPCPFlashPolyRingVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(scaleB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaleB);
    SDF_VU0_LOADVECTOR(scaleC);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaleC);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(scaleA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(scaleC);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(scaleB);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
void effPCPFlashPolyRingPartRotate(PcpFlashScalingOrbitWork *work, s32 index) {
    PcpFlashOrbitParticle *part;

    part = &work->parts[index];
    part->angle += work->angularStep;
}

void effPCPFlashPolyRingDraw(PcpFlashScalingOrbitWork *work) {
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    PcpFlashOrbitParticle *part;
    dds3FlashPolyList_t *handle;

    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            part->initialScale = maxScale;
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            effPCPFlashPolyRingPartSetVertex(work, index);
            effPCPFlashPolyRingPartSetVC(work, index, 0);
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
                }
                color = 0;
                effPCPFlashPolyRingPartSetVC(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effPCPFlashPolyRingPartRotate(work, index);
                effPCPFlashPolyRingPartSetVertex(work, index);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor);
                effPCPFlashPolyRingPartSetVC(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyListDraw(handle);
}

typedef struct dds3FlashPolyList_t dds3FlashPolyList_t;
extern dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 quadCount);

/* Clone the 0x50-byte parameter block, then spread the particles evenly around the orbit from -pi/2 with random negative start ages (two handle slots per particle). */
PcpFlashAccumulatingWork *effPCPFlashPolyPipeCreate(PcpFlashAccumulatingParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashAccumulatingParticle) + sizeof(PcpFlashAccumulatingWork));
    PcpFlashAccumulatingWork *work = (PcpFlashAccumulatingWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, params, 0x50);
    work->parts = (PcpFlashAccumulatingParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = dds3FlashPolyList3Create(work->particleCount * 2);
    record->scale = 1.0f;
    record->drawMode = work->drawMode;
    work->resourceHandle = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
        work->parts[i].orbitAngle = angle;
        angle += step;
    }
    return work;
}

PcpFlashAccumulatingWork *effPCPFlashPolyPipeCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyPipeCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashAccumulatingWork *effPCPFlashPolyPipeCopy(PcpFlashAccumulatingParams *params)
{
    return effPCPFlashPolyPipeCreate(params);
}

void effPCPFlashPolyPipeDelete(PcpFlashAccumulatingWork *work) {
    dds3FlashPolyList3Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyPipeSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyPipeSetColor(PcpFlashAccumulatingWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolyPipeSetScale(PcpFlashAccumulatingWork *work, f32 value)
{
    work->renderScale = value;
}

extern void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *, s32);

void effPCPFlashPolyPipePartSetVC(PcpFlashAccumulatingWork *work, s32 index, u32 colorMultiplier) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)dds3FlashPolyList3GetVC(work->resourceHandle, index * 2);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    colors[0] = effMagaCylinderCalcColor(colorB | 0x80000000, colorMultiplier);
    colors[1] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    colors[2] = effMagaCylinderCalcColor(colorA | 0x80000000, colorMultiplier);
    colors[3] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    mirror = (u32 *)dds3FlashPolyList3GetVC(work->resourceHandle, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}

/* vu0 routine: the orbit offset plus a tilted disc of corner offsets for an
   accumulating particle, whose radius grows by the work record's radial step */
void effPCPFlashPolyPipePartSetVertex(PcpFlashAccumulatingWork *work, s32 index)
{
    PcpFlashAccumulatingParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList3GetVertex(work->resourceHandle, index * 2);
    f32 offset[4];
    f32 unit[4];
    f32 middle[4];
    f32 outer[4];
    f32 inner[4];
    f32 size[4];
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 sinv;
    f32 height;
    f32 *mirror;

    center = part->radius;
    VEC3_SPLAT(middle, center);
    outerEdge = center + part->radialHalfThickness;
    VEC3_SPLAT(outer, outerEdge);
    innerEdge = center - part->radialHalfThickness;
    VEC3_SPLAT(inner, innerEdge);
    part->radius = part->radius + work->radialStep;
    span = part->span;
    VEC3_SPLAT(size, span);
    unit[0] = sdfCos(part->orbitAngle);
    unit[1] = 0;
    sinv = sdfSin(part->orbitAngle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyPipeVertex[0] = unit[0] * height;
    effPCPFlashPolyPipeVertex[1] = height + -1.0f;
    effPCPFlashPolyPipeVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyPipeVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(size);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(outer);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(outer);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(inner);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(inner);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(middle);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(outer);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    mirror = dds3FlashPolyList3GetVertex(work->resourceHandle, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(inner);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(mirror);
}

void effPCPFlashPolyPipePartRotate(PcpFlashAccumulatingWork *work, s32 index) {
    PcpFlashAccumulatingParticle *part;

    part = &work->parts[index];
    part->orbitAngle += work->increment;
}

extern void effPCPFlashPolyPipePartSetVC(PcpFlashAccumulatingWork *work, s32 index, u32 colorMultiplier);
extern void effPCPFlashPolyPipePartSetVertex(PcpFlashAccumulatingWork *work, s32 index);

void effPCPFlashPolyPipeDraw(PcpFlashAccumulatingWork *work)
{
    s32 index;
    s32 count;
    PcpFlashAccumulatingParticle *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    dds3FlashPolyList_t *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    randomRange = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 factor;

        if (age == 0) {
            effPCPFlashPolyPipePartSetVertex(work, index);
            effPCPFlashPolyPipePartSetVC(work, index, 0);
            part->color = 0x80808080;
            factor = dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f;
            part->radialHalfThickness = work->radialHalfThicknessBase * factor;
            factor = dds3GetRandFloatA(effDefaultRandomState) * 0.7f + 0.3f;
            part->span = work->spanBase * factor;
            part->unk10 = work->unk34 * factor;
            part->radius = 0;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % randomRange);
            }
            color = 0;
            effPCPFlashPolyPipePartSetVC(work, index, color);
        } else if (age > 0) {
            effPCPFlashPolyPipePartRotate(work, index);
            effPCPFlashPolyPipePartSetVertex(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->color, factor), tintColor);
            effPCPFlashPolyPipePartSetVC(work, index, color);
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyList3Draw(handle);
}




PcpFlashOrbitArcWork *effPCPFlashPolyRingExpCreate(PcpFlashOrbitArcParams *params)
{
    sdfMemHandle_t *handle;
    PcpFlashOrbitArcBlock *block;
    PcpFlashOrbitArcWork *ring;
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = sdfMemAllocDirect((u32)params->particleCount * 16 + 0x80);
    block = (PcpFlashOrbitArcBlock *)sdfMemLock(handle);
    ring = &block->header;
    memcpy(ring, params, 0x58);
    ring->parts = block->parts;
    ring->allocationHandle = handle;
    ring->tintColor = 0x80808080;
    ring->orbitRadius = ring->orbitRadiusStart;
    ring->upSpan = ring->upSpanStart;
    ring->acrossSpan = ring->acrossSpanStart;
    ring->shapeProgressFrame = 0;
    ring->renderScale = 1.0f;
    if (ring->randomRange == 0) {
        ring->randomRange = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    record = dds3FlashPolyListCreate(ring->particleCount);
    record->scale = 1.0f;
    record->drawMode = ring->drawMode;
    ring->resourceHandle = record;
    step = EFFECT_RING_FULL_TURN / (u32)ring->particleCount;
    spread = ring->randomRange;
    for (i = 0; i < (u32)ring->particleCount; i++) {
        ring->parts[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % spread);
        ring->parts[i].accumulator = angle;
        angle += step;
    }
    return ring;
}

PcpFlashOrbitArcWork *effPCPFlashPolyRingExpCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyRingExpCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashOrbitArcWork *effPCPFlashPolyRingExpCopy(PcpFlashOrbitArcParams *params)
{
    return effPCPFlashPolyRingExpCreate(params);
}

void effPCPFlashPolyRingExpDelete(PcpFlashOrbitArcWork *work) {
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyRingExpSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyRingExpSetColor(PcpFlashOrbitArcWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolyRingExpSetScale(PcpFlashOrbitArcWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyRingExpPartSetVC(PcpFlashOrbitArcWork *work, s32 index, s32 color) {
    PcpFlashQuadColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashQuadColorSlot *)dds3FlashPolyListGetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(colorB, color);
    slot->color[1] = effMagaCylinderCalcColor(colorB, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

/* vu0 routine: billboard corner offsets for a particle on an arc, built from a normalised direction and its perpendicular */
void effPCPFlashPolyRingExpPartSetVertex(PcpFlashOrbitArcWork *work, s32 index)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyListGetVertex(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 sinv;
    f32 height;

    VEC3_SPLAT(scaleA, work->normalSpan);
    VEC3_SPLAT(scaleB, work->acrossSpan);
    VEC3_SPLAT(scaleC, work->upSpan);
    unit[0] = sdfCos(part->accumulator);
    unit[1] = 0;
    sinv = sdfSin(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = part->stepSpeed;
    effPCPFlashPolyRingExpVertex[0] = unit[0] * height;
    effPCPFlashPolyRingExpVertex[1] = height + -1.0f;
    effPCPFlashPolyRingExpVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingExpVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(scaleB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaleB);
    SDF_VU0_LOADVECTOR(scaleC);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaleC);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(scaleA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(scaleC);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(scaleB);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
void effPCPFlashPolyRingExpPartRotate(PcpFlashOrbitArcWork *work, s32 index) {
    PcpFlashMotionParticle *part;

    part = &work->parts[index];
    part->accumulator += work->angularStep;
}

void effPCPFlashPolyRingExpDraw(PcpFlashOrbitArcWork *work)
{
    s32 index;
    s32 count;
    PcpFlashMotionParticle *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    dds3FlashPolyList_t *handle;
    f32 heightDelta;
    f32 delta;
    f32 progress;
    f32 factor;
    f32 start;
    f32 radiusEnd;
    f32 upStart;
    f32 acrossStart;
    f32 upEnd;
    f32 acrossEnd;
    f32 normalEnd;

    part = work->parts;
    count = work->particleCount;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    randomRange = work->randomRange;
    tintColor = work->tintColor;
    heightDelta = work->heightEnd - work->heightStart;
    progress = 1.0f;
    if (lifetime > 0) {
        progress = (f32)work->shapeProgressFrame / (f32)lifetime;
    }
    start = work->orbitRadiusStart;
    radiusEnd = work->orbitRadiusEnd;
    delta = radiusEnd - start;
    work->orbitRadius = start + delta * progress;
    if (start > 0.0f) {
        factor = radiusEnd / start;
    } else {
        factor = radiusEnd;
    }
    upStart = work->upSpanStart;
    acrossStart = work->acrossSpanStart;
    upEnd = upStart * factor;
    acrossEnd = acrossStart * factor;
    start = work->normalSpanStart;
    normalEnd = work->normalSpanEnd;
    delta = upEnd - upStart;
    work->upSpan = upStart + delta * progress;
    delta = acrossEnd - acrossStart;
    work->acrossSpan = acrossStart + delta * progress;
    delta = normalEnd - start;
    work->normalSpan = start + delta * progress;

    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;

        if (age == 0) {
            part->stepSpeed = work->heightStart;
            effPCPFlashPolyRingExpPartSetVertex(work, index);
            effPCPFlashPolyRingExpPartSetVC(work, index, 0);
            part->color = 0x80808080;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % randomRange);
            }
            color = 0;
            effPCPFlashPolyRingExpPartSetVC(work, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->stepSpeed = heightDelta * progress + work->heightStart;
            effPCPFlashPolyRingExpPartRotate(work, index);
            effPCPFlashPolyRingExpPartSetVertex(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->color, factor), tintColor);
            effPCPFlashPolyRingExpPartSetVC(work, index, color);
        }
        part->age = part->age + 1;
    }
    if (work->shapeProgressFrame == lifetime) {
        work->shapeProgressFrame = 0;
    } else {
        work->shapeProgressFrame++;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyListDraw(handle);
}

PcpFlashRotatingQuadWork *effPCPFlashPolySphereExpCreate(PcpFlashRotatingQuadParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashRotatingQuadParticle) + sizeof(PcpFlashRotatingQuadWork));
    PcpFlashRotatingQuadWork *work = (PcpFlashRotatingQuadWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    u32 range;
    u32 i;

    memcpy(work, params, 0x4C);
    work->parts = (PcpFlashRotatingQuadParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyListCreate(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->drawMode;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
    }
    return work;
}

PcpFlashRotatingQuadWork *effPCPFlashPolySphereExpCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolySphereExpCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashRotatingQuadWork *effPCPFlashPolySphereExpCopy(PcpFlashRotatingQuadParams *params)
{
    return effPCPFlashPolySphereExpCreate(params);
}

void effPCPFlashPolySphereExpDelete(PcpFlashRotatingQuadWork *work) {
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphereExpSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphereExpSetColor(PcpFlashRotatingQuadWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolySphereExpSetScale(PcpFlashRotatingQuadWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySphereExpPartSetVC(PcpFlashRotatingQuadWork *work, s32 index, s32 color) {
    PcpFlashQuadColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashQuadColorSlot *)dds3FlashPolyListGetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(colorB, color);
    slot->color[1] = effMagaCylinderCalcColor(colorB, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

extern EffRandState effDefaultRandomState[];

void effPCPFlashPolySphereExpPartInit(PcpFlashRotatingQuadWork *work, s32 index, void *orientation) {
    PcpFlashRotatingQuadParticle *part = work->parts + index;
    f32 factor;
    f32 scale;

    part->angle = dds3GetRandFloatA(effDefaultRandomState) * 6.2831853f;
    factor = dds3GetRandFloatA(effDefaultRandomState) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->angularStep = work->angularStepRange * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
}

/* vu0 routine: corner offsets of a flash particle's billboard, turned around the view axis by the particle's angle */
void effPCPFlashPolySphereExpPartSetVertex(PcpFlashRotatingQuadWork *work, s32 index, void *view)
{
    PcpFlashRotatingQuadParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyListGetVertex(work->resourceHandle, index);
    f32 base[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scale, size);
    acrossLen = part->acrossSpan * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->upSpan * ratio;
    VEC3_SPLAT(up, upLen);
    SetRotateMatrixAxis(part->angle, (const RwV3d *)view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(across);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(across);
    SDF_VU0_LOADVECTOR(up);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(up);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereExpVertex);
    SDF_VU0_LOADVECTOR_B(up);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereExpVertex);
    SDF_VU0_LOADVECTOR_B(across);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
void effPCPFlashPolySphereExpPartRotate(PcpFlashRotatingQuadWork *work, s32 index, void *orientation) {
    PcpFlashRotatingQuadParticle *part;

    part = &work->parts[index];
    part->angle += part->angularStep;
}

void effPCPFlashPolySphereExpDraw(PcpFlashRotatingQuadWork *work) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 ramp;
    s32 fadeIn;
    s32 fadeOut;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    PcpFlashRotatingQuadParticle *part;
    dds3FlashPolyList_t *handle;

    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        SDF_VU0_SUBVECTOR();;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effPCPFlashPolySphereExpPartInit(work, index, &axis);
            effPCPFlashPolySphereExpPartSetVertex(work, index, &axis);
            effPCPFlashPolySphereExpPartSetVC(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
                }
                color = 0;
                effPCPFlashPolySphereExpPartSetVC(work, index, color);
            } else if (part->age > 0) {
                s32 remain;

                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effPCPFlashPolySphereExpPartRotate(work, index, &axis);
                effPCPFlashPolySphereExpPartSetVertex(work, index, &axis);
                age = part->age;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor);
                effPCPFlashPolySphereExpPartSetVC(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyListDraw(handle);
}

extern dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 count);

PcpFlashRadialTriangleWork *effPCPFlashPolySphere2ExpCreate(PcpFlashRadialTriangleParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashMotionParticle) + sizeof(PcpFlashRadialTriangleWork));
    PcpFlashRadialTriangleWork *work = (PcpFlashRadialTriangleWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, params, 0x3C);
    work->parts = (PcpFlashMotionParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}

PcpFlashRadialTriangleWork *effPCPFlashPolySphere2ExpCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolySphere2ExpCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashRadialTriangleWork *effPCPFlashPolySphere2ExpCopy(PcpFlashRadialTriangleParams *params)
{
    return effPCPFlashPolySphere2ExpCreate(params);
}

void effPCPFlashPolySphere2ExpDelete(PcpFlashRadialTriangleWork *work) {
    dds3FlashPolyList2Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphere2ExpSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphere2ExpSetColor(PcpFlashRadialTriangleWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolySphere2ExpSetScale(PcpFlashRadialTriangleWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySphere2ExpPartSetVC(PcpFlashRadialTriangleWork *work, s32 index, s32 color)
{
    PcpFlashColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot *)dds3FlashPolyList2GetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->first = effMagaCylinderCalcColor(colorB, color);
    slot->second = effMagaCylinderCalcColor(colorB, color);
    slot->third = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effPCPFlashPolySphere2ExpPartSetVertex(PcpFlashRadialTriangleWork *work, s32 index, void *view)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList2GetVertex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    SetRotateMatrixAxis(angle, (const RwV3d *)view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(def_dir);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SetRotateMatrixAxis(angle + step, (const RwV3d *)view);
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(def_dir);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolySphere2ExpDraw(PcpFlashRadialTriangleWork *work) {
    s128 axis;
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    PcpFlashMotionParticle *part;
    dds3FlashPolyList_t *handle;

    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        SDF_VU0_SUBVECTOR();;
        SDF_VU0_NORMALIZE();;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    tintColor = work->tintColor;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effPCPFlashPolySphere2ExpPartSetVC(work, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolySphere2ExpPartSetVertex(work, index, &axis);
                effPCPFlashPolySphere2ExpPartSetVC(work, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * decay;
                part->accumulator = part->accumulator + speed;
                effPCPFlashPolySphere2ExpPartSetVertex(work, index, &axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effPCPFlashPolySphere2ExpPartSetVC(work, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    if (active != 0) {
        dds3FlashPolyList2Draw(handle);
    }
}

/* Clone the 0xD0-byte parameter block, then give every particle a random negative start age (two handle slots per particle). */
PcpFlashRadialStripWork *effPCPFlashPolySphere3Create(PcpFlashRadialStripParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashRadialStripParticle) + sizeof(PcpFlashRadialStripWork));
    PcpFlashRadialStripWork *work = (PcpFlashRadialStripWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    u32 range;
    u32 i;

    memcpy(work, params, 0xD0);
    work->parts = (PcpFlashRadialStripParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyList3Create(work->particleCount * 2);
    record->scale = 1.0f;
    record->drawMode = work->drawMode;
    work->resourceHandle = record;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
    }
    return work;
}

PcpFlashRadialStripWork *effPCPFlashPolySphere3CreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolySphere3Create(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashRadialStripWork *effPCPFlashPolySphere3Copy(PcpFlashRadialStripParams *params)
{
    return effPCPFlashPolySphere3Create(params);
}

void effPCPFlashPolySphere3Delete(PcpFlashRadialStripWork *work) {
    dds3FlashPolyList3Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphere3SetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphere3SetColor(PcpFlashRadialStripWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolySphere3SetScale(PcpFlashRadialStripWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySphere3PartSetVC(PcpFlashRadialStripWork *work, s32 index, u32 colorMultiplier) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)dds3FlashPolyList3GetVC(work->resourceHandle, index * 2);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    colors[0] = effMagaCylinderCalcColor(colorB | 0x80000000, colorMultiplier);
    colors[1] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    colors[2] = effMagaCylinderCalcColor(colorA | 0x80000000, colorMultiplier);
    colors[3] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    mirror = (u32 *)dds3FlashPolyList3GetVC(work->resourceHandle, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}

void effPCPFlashPolySphere3PartInit(PcpFlashRadialStripWork *work, s32 index, void *orientation) {
    PcpFlashRadialStripParticle *part = work->parts + index;
    f32 factor;

    part->angle = dds3GetRandFloatA(effDefaultRandomState) * 6.2831853f;
    factor = dds3GetRandFloatA(effDefaultRandomState) * 0.3f + 0.7f;
    part->thickness = work->maxThickness * factor;
    factor = work->spanVariation;
    part->span = work->spanBase * (dds3GetRandFloatA(effDefaultRandomState) * factor + (1.0f - factor));
    part->angularStep = work->angularStepRange * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f);
}

/* vu0 routine: two quads of corner offsets for a flash particle (a strip and its mirror), turned around the view axis by the particle's angle */
void effPCPFlashPolySphere3PartSetVertex(PcpFlashRadialStripWork *work, s32 index, void *view)
{
    PcpFlashRadialStripParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList3GetVertex(work->resourceHandle, index * 2);
    f32 base[4];
    f32 size[4];
    f32 spare[4];
    f32 middle[4];
    f32 outer[4];
    f32 inner[4];
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 *mirror;

    center = part->radius;
    VEC3_SPLAT(middle, center);
    outerEdge = center + part->thickness;
    VEC3_SPLAT(outer, outerEdge);
    innerEdge = center - part->thickness;
    VEC3_SPLAT(inner, innerEdge);
    span = part->span;
    VEC3_SPLAT(size, span);
    SetRotateMatrixAxis(part->angle, (const RwV3d *)view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_COPYVECTOR_A_TO_C();
    /* retail multiplies the (never written) `spare` slot here and stores it back */
    SDF_VU0_LOADVECTOR_B(spare);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(spare);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(size);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(outer);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(outer);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(inner);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(inner);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(middle);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_LOADVECTOR(outer);
    SDF_VU0_STOREVECTOR(quad);
    mirror = dds3FlashPolyList3GetVertex(work->resourceHandle, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(inner);
    SDF_VU0_STOREVECTOR(mirror);
}
void effPCPFlashPolySphere3PartRotate(PcpFlashRadialStripWork *work, s32 index, void *orientation) {
    PcpFlashRadialStripParticle *part;

    part = &work->parts[index];
    part->angle += part->angularStep;
}

void effPCPFlashPolySphere3Draw(PcpFlashRadialStripWork *work) {
    s128 axis;
    s32 index;
    s32 count;
    s32 lifetime;
    f32 maxThickness;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    s32 restart;
    u32 range;
    s32 tintColor;
    PcpFlashRadialStripParticle *part;
    dds3FlashPolyList_t *handle;

    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        SDF_VU0_SUBVECTOR();;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    maxThickness = work->maxThickness;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effPCPFlashPolySphere3PartSetVC(work, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolySphere3PartInit(work, index, &axis);
                effPCPFlashPolySphere3PartSetVertex(work, index, &axis);
                effPCPFlashPolySphere3PartSetVC(work, index, 0);
                part->thickness = maxThickness;
                part->radius = startA;
                part->radialSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->radialSpeed;
                s32 remain;
                f32 blend;

                part->radialSpeed = speed * decay;
                part->radius = part->radius + speed;
                effPCPFlashPolySphere3PartRotate(work, index, &axis);
                effPCPFlashPolySphere3PartSetVertex(work, index, &axis);
                age = part->age;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effPCPFlashPolySphere3PartSetVC(work, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
                effPCPFlashPolySphere3PartSetVC(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyList3Draw(handle);
}

/* Clone the 0x50-byte parameter block, then spread the particles evenly around the orbit from -pi/2 with random negative start ages. */
PcpFlashFadingOrbitWork *effPCPFlashPolyRingExp2Create(PcpFlashFadingOrbitParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashOrbitParticle) + sizeof(PcpFlashFadingOrbitWork));
    PcpFlashFadingOrbitWork *work = (PcpFlashFadingOrbitWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, params, 0x50);
    work->parts = (PcpFlashOrbitParticle *)(work + 1);
    work->allocationHandle = handle;
    work->tintColor = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = dds3FlashPolyListCreate(work->particleCount);
    record->scale = 1.0f;
    record->drawMode = work->drawMode;
    work->resourceHandle = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
        work->parts[i].angle = angle;
        angle += step;
    }
    return work;
}

PcpFlashFadingOrbitWork *effPCPFlashPolyRingExp2CreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyRingExp2Create(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashFadingOrbitWork *effPCPFlashPolyRingExp2Copy(PcpFlashFadingOrbitParams *params)
{
    return effPCPFlashPolyRingExp2Create(params);
}

void effPCPFlashPolyRingExp2Delete(PcpFlashFadingOrbitWork *work) {
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyRingExp2SetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyRingExp2SetColor(PcpFlashFadingOrbitWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolyRingExp2SetScale(PcpFlashFadingOrbitWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyRingExp2PartSetVC(PcpFlashFadingOrbitWork *work, s32 index, s32 color) {
    PcpFlashQuadColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashQuadColorSlot *)dds3FlashPolyListGetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(colorB, color);
    slot->color[1] = effMagaCylinderCalcColor(colorB, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(colorA | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effPCPFlashPolyRingExp2PartSetVertex(PcpFlashFadingOrbitWork *work, s32 index)
{
    PcpFlashOrbitParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyListGetVertex(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scaleA, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = sdfCos(part->angle);
    unit[1] = 0;
    sinv = sdfSin(part->angle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyRingExp2Vertex[0] = unit[0] * height;
    effPCPFlashPolyRingExp2Vertex[1] = height + -1.0f;
    effPCPFlashPolyRingExp2Vertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingExp2Vertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(scaleB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaleB);
    SDF_VU0_LOADVECTOR(scaleC);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaleC);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(scaleA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(scaleC);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(offset);
    SDF_VU0_LOADVECTOR_B(scaleB);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}
void effPCPFlashPolyRingExp2PartRotate(PcpFlashFadingOrbitWork *work, s32 index) {
    PcpFlashOrbitParticle *part;

    part = &work->parts[index];
    part->angle += work->angularStep;
}

void effPCPFlashPolyRingExp2Draw(PcpFlashFadingOrbitWork *work) {
    s32 restart;
    s32 tintColor;
    s32 count;
    s32 index;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    f32 maxScale;
    u32 range;
    PcpFlashOrbitParticle *part;
    dds3FlashPolyList_t *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    ramp = work->scaleRampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    tintColor = work->tintColor;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effPCPFlashPolyRingExp2PartSetVC(work, index, 0);
        } else {
            if (age == 0) {
                part->initialScale = maxScale;
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = 0.0f;
                }
                effPCPFlashPolyRingExp2PartSetVertex(work, index);
                effPCPFlashPolyRingExp2PartSetVC(work, index, 0);
                part->color = 0x80808080;
            } else if (age > 0) {
                s32 remain;
                f32 blend;

                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effPCPFlashPolyRingExp2PartRotate(work, index);
                effPCPFlashPolyRingExp2PartSetVertex(work, index);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effPCPFlashPolyRingExp2PartSetVC(work, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % range);
                effPCPFlashPolyRingExp2PartSetVC(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->renderScale;
    dds3FlashPolyListDraw(handle);
}

PcpFlashOffsetRadialWork *effPCPFlashPolyCircleCreate(PcpFlashOffsetRadialParams *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashMotionParticle) + sizeof(PcpFlashOffsetRadialWork));
    PcpFlashOffsetRadialWork *work = (PcpFlashOffsetRadialWork *)sdfMemLock(handle);
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, params, 0x40);
    work->parts = (PcpFlashMotionParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->resourceHandle = record;
    record->drawMode = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}

PcpFlashOffsetRadialWork *effPCPFlashPolyCircleCreatePCF(void *table) {
    void *effectParams;

    effectParams = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyCircleCreate(effectParams);
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashOffsetRadialWork *effPCPFlashPolyCircleCopy(PcpFlashOffsetRadialParams *params)
{
    return effPCPFlashPolyCircleCreate(params);
}

void effPCPFlashPolyCircleDelete(PcpFlashOffsetRadialWork *work) {
    dds3FlashPolyList2Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyCircleSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyCircleSetColor(PcpFlashOffsetRadialWork *work, u32 value) {
    work->tintColor = value;
}

void effPCPFlashPolyCircleSetScale(PcpFlashOffsetRadialWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyCirclePartSetVC(PcpFlashOffsetRadialWork *work, s32 index, s32 color)
{
    PcpFlashColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot *)dds3FlashPolyList2GetVC(work->resourceHandle, index);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    slot->first = effMagaCylinderCalcColor(colorB, color);
    slot->second = effMagaCylinderCalcColor(colorB, color);
    slot->third = effMagaCylinderCalcColor(colorA | 0xFF000000, color);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effPCPFlashPolyCirclePartSetVertex(PcpFlashOffsetRadialWork *work, s32 index, void *view)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList2GetVertex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    SetRotateMatrixAxis(angle, (const RwV3d *)view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(effPCPFlashPolyCircleVertex);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SetRotateMatrixAxis(angle + step, (const RwV3d *)view);
    SDF_VU0_LOADVECTOR(base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(effPCPFlashPolyCircleVertex);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolyCircleDraw(PcpFlashOffsetRadialWork *work) {
    f32 axis[4];
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    f32 scale;
    PcpFlashMotionParticle *part;
    dds3FlashPolyList_t *handle;

    VU0_LOAD_VF($vf10, aViewTarget);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        SDF_VU0_SUBVECTOR();;
        SDF_VU0_NORMALIZE();;
    VU0_STORE_VF($vf10, axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    tintColor = work->tintColor;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effPCPFlashPolyCirclePartSetVC(work, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolyCirclePartSetVertex(work, index, axis);
                effPCPFlashPolyCirclePartSetVC(work, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * decay;
                part->accumulator = part->accumulator + speed;
                effPCPFlashPolyCirclePartSetVertex(work, index, axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effPCPFlashPolyCirclePartSetVC(work, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    scale = work->originOffset * work->renderScale;
    handle = work->resourceHandle;
    work->updateCount = work->updateCount + 1;
    handle->origin[0] = work->origin[0] + axis[0] * scale;
    handle->origin[1] = work->origin[1] + axis[1] * scale;
    handle->origin[2] = work->origin[2] + axis[2] * scale;
    handle->scale = work->renderScale;
    if (active != 0) {
        dds3FlashPolyList2Draw(handle);
    }
}
