#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "eff_param.h"
#include "sdf_vu_matrix.h"
#include "btl_effect_position.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "eff_pcp_event_place.h"
#include "eff_pcp_vector_copy_api.h"
#include "sdf_quaternion.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "eff_thunder_vector.h"
#include "par_cell_api.h"
#include "sdf_heap_api.h"
#include "btl_sound.h"
#include "eff.h"
#include "eff_thunder_particle_records.h"
#include "eff_pcp_thunder_cell.h"
#include "eff_thunder_draw_params.h"
#include "eff_math.h"
#include "eff_thunder_fragment.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "eff_pcp_flash.h"
#include "eff_pcp_thunder_group.h"
#include "pcp_vu0.h"
#include "sdf_packet_alloc.h"
#include "eff_flash_fan_color_slot.h"
#include "eff_packet_params.h"
#include "eff_ring_color_slot.h"
#include "eff_pcp_ring_block.h"
#include "eff_pcp_scatter_radial.h"
#include "ee_mmi.h"

/* The event holder consumes the same packed 0x30-byte record as effEvent.c. */
typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

extern dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 triangleCount);

/* Clone the 0x30-byte parameter block, create the record pool and clear every particle's age. */
PcpFlashTrianglePulseWork *effPCPFlashPolySphere2Create(PcpFlashTrianglePulseParams *params)
{
    sdfMemHandle_t *allocation = sdfMemAllocDirect(params->particleCount * sizeof(PcpFlashPulseParticle) + sizeof(PcpFlashTrianglePulseWork));
    PcpFlashTrianglePulseWork *work = (PcpFlashTrianglePulseWork *)sdfMemLock(allocation);
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, params, 0x30);
    work->parts = (PcpFlashPulseParticle *)(work + 1);
    work->tintColor = 0x80808080;
    work->allocationHandle = allocation;
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

/* Work records keep SDF allocation ownership separate from drawable records.
   tintColor multiplies the age-faded packed color; scaleRampTime governs
   scale growth independently of the color fade. */

extern void dds3FlashPolyList2Delete(dds3FlashPolyList_t *pool);

extern void dds3FlashPolyList3Delete(dds3FlashPolyList_t *pool);

extern void dds3FlashPolyListDelete(dds3FlashPolyList_t *pool);

extern void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *pool, s32 index);

extern u32 effMagaCylinderCalcColor(u32 color, u32 colorB);

extern EffRandState effDefaultRandomState[];

extern f32 *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *pool, s32 index);

extern f32 effPCPFlashPolySphereVertex[];

extern f32 effPCPFlashPolyRingVertex[];

extern f32 effPCPFlashPolyPipeVertex[];

extern f32 effPCPFlashPolyRingExp2Vertex[];

extern f32 effPCPFlashPolyRingExpVertex[];

extern f32 effPCPFlashPolySphereExpVertex[];

extern f32 def_dir[];

extern f32 effPCPFlashPolySphere2Vertex[];

extern f32 effPCPFlashPolyCircleVertex[];

extern f32 *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *pool, s32 index);

extern f32 *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *pool, s32 index);

extern u8 aViewTarget[];

extern u8 sdfViewTargetVector[];

extern void effPCPFlashPolySphere2PartSetVertex(PcpFlashTrianglePulseWork *work, s32 index, const f32 *view);

extern void dds3FlashPolyList2Draw(dds3FlashPolyList_t *);

extern void effPCPFlashPolySpherePartSetVertex(PcpFlashStreakWork *work, s32 index, const f32 *view);

extern void dds3FlashPolyListDraw(dds3FlashPolyList_t *);

extern void effPCPFlashPolyRingPartSetVertex(PcpFlashScalingOrbitWork *, s32);

extern void effPCPFlashPolySphere2ExpPartSetVertex(PcpFlashRadialTriangleWork *, s32, void *);

extern void effPCPFlashPolyRingExp2PartSetVertex(PcpFlashFadingOrbitWork *, s32);

extern void effPCPFlashPolyCirclePartSetVertex(PcpFlashOffsetRadialWork *, s32, void *);

PcpFlashTrianglePulseWork *effPCPFlashPolySphere2CreatePCF(const EffParamTable *table)
{
    return effPCPFlashPolySphere2Create((PcpFlashTrianglePulseParams *)dds3EffectPCPDataGetData(table, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashTrianglePulseWork *effPCPFlashPolySphere2Copy(PcpFlashTrianglePulseParams *params)
{
    return effPCPFlashPolySphere2Create(params);
}

void effPCPFlashPolySphere2Delete(PcpFlashTrianglePulseWork *work)
{
    dds3FlashPolyList2Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphere2SetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphere2SetColor(PcpFlashTrianglePulseWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolySphere2SetScale(PcpFlashTrianglePulseWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySphere2PartSetVC(PcpFlashTrianglePulseWork *work, s32 index, s32 color)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)dds3FlashPolyList2GetVC(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMagaCylinderCalcColor(rgb2, color);
    slot->second = effMagaCylinderCalcColor(rgb2, color);
    slot->third = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
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

extern dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 cellCount);

PcpFlashStreakWork *effPCPFlashPolySphereCreate(PcpFlashStreakParams *src) {
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

PcpFlashStreakWork *effPCPFlashPolySphereCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolySphereCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashStreakWork *effPCPFlashPolySphereCopy(PcpFlashStreakParams *src)
{
    return effPCPFlashPolySphereCreate(src);
}

void effPCPFlashPolySphereDelete(PcpFlashStreakWork *work)
{
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphereSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphereSetColor(PcpFlashStreakWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolySphereSetScale(PcpFlashStreakWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySpherePartSetVC(PcpFlashStreakWork *work, s32 index, s32 color)
{
    EffFlashFanColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = dds3FlashPolyListGetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(rgb2, color);
    slot->color[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

void effPCPFlashPolySpherePartInit(PcpFlashStreakWork *work, s32 index, const f32 *orientation) {
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
void effPCPFlashPolySpherePartSetVertex(PcpFlashStreakWork *work, s32 index, const f32 *view)
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

void effPCPFlashPolySpherePartRotate(PcpFlashStreakWork *work, s32 index, const f32 *orientation)
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
            effPCPFlashPolySpherePartInit(work, index, (const f32 *)&axis);
            effPCPFlashPolySpherePartSetVertex(work, index, (const f32 *)&axis);
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
                effPCPFlashPolySpherePartRotate(work, index, (const f32 *)&axis);
                effPCPFlashPolySpherePartSetVertex(work, index, (const f32 *)&axis);
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

PcpFlashScalingOrbitWork *effPCPFlashPolyRingCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolyRingCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashScalingOrbitWork *effPCPFlashPolyRingCopy(PcpFlashScalingOrbitParams *params)
{
    return effPCPFlashPolyRingCreate(params);
}

void effPCPFlashPolyRingDelete(PcpFlashScalingOrbitWork *work)
{
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyRingSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyRingSetColor(PcpFlashScalingOrbitWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolyRingSetScale(PcpFlashScalingOrbitWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyRingPartSetVC(PcpFlashScalingOrbitWork *work, s32 index, s32 color)
{
    EffFlashFanColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = dds3FlashPolyListGetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(rgb2, color);
    slot->color[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
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

void effPCPFlashPolyRingPartRotate(PcpFlashScalingOrbitWork *work, s32 index)
{
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

PcpFlashAccumulatingWork *effPCPFlashPolyPipeCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolyPipeCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashAccumulatingWork *effPCPFlashPolyPipeCopy(PcpFlashAccumulatingParams *params)
{
    return effPCPFlashPolyPipeCreate(params);
}

void effPCPFlashPolyPipeDelete(PcpFlashAccumulatingWork *work)
{
    dds3FlashPolyList3Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyPipeSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyPipeSetColor(PcpFlashAccumulatingWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolyPipeSetScale(PcpFlashAccumulatingWork *work, f32 value)
{
    work->renderScale = value;
}

typedef struct dds3FlashPolyList_t dds3FlashPolyList_t;

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

void effPCPFlashPolyPipePartRotate(PcpFlashAccumulatingWork *work, s32 index)
{
    PcpFlashAccumulatingParticle *part;

    part = &work->parts[index];
    part->orbitAngle += work->increment;
}

extern void effPCPFlashPolyPipePartSetVC(PcpFlashAccumulatingWork *work, s32 index, u32 colorMultiplier);

extern void effPCPFlashPolyPipePartSetVertex(PcpFlashAccumulatingWork *work, s32 index);

extern void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool);

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

    handle = sdfMemAllocDirect(sizeof(PcpFlashOrbitArcWork) +
                               (u32)params->particleCount * sizeof(PcpFlashMotionParticle));
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

PcpFlashOrbitArcWork *effPCPFlashPolyRingExpCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolyRingExpCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashOrbitArcWork *effPCPFlashPolyRingExpCopy(PcpFlashOrbitArcParams *params)
{
    return effPCPFlashPolyRingExpCreate(params);
}

void effPCPFlashPolyRingExpDelete(PcpFlashOrbitArcWork *work)
{
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyRingExpSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyRingExpSetColor(PcpFlashOrbitArcWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolyRingExpSetScale(PcpFlashOrbitArcWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyRingExpPartSetVC(PcpFlashOrbitArcWork *work, s32 index, s32 color)
{
    EffFlashFanColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = dds3FlashPolyListGetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(rgb2, color);
    slot->color[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
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

void effPCPFlashPolyRingExpPartRotate(PcpFlashOrbitArcWork *work, s32 index)
{
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

PcpFlashRotatingQuadWork *effPCPFlashPolySphereExpCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolySphereExpCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashRotatingQuadWork *effPCPFlashPolySphereExpCopy(PcpFlashRotatingQuadParams *params)
{
    return effPCPFlashPolySphereExpCreate(params);
}

void effPCPFlashPolySphereExpDelete(PcpFlashRotatingQuadWork *work)
{
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphereExpSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphereExpSetColor(PcpFlashRotatingQuadWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolySphereExpSetScale(PcpFlashRotatingQuadWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySphereExpPartSetVC(PcpFlashRotatingQuadWork *work, s32 index, s32 color)
{
    EffFlashFanColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = dds3FlashPolyListGetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(rgb2, color);
    slot->color[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

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
void effPCPFlashPolySphereExpPartSetVertex(PcpFlashRotatingQuadWork *work, s32 index, const f32 *view)
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

void effPCPFlashPolySphereExpPartRotate(PcpFlashRotatingQuadWork *work, s32 index, void *orientation)
{
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
            effPCPFlashPolySphereExpPartSetVertex(work, index, (const f32 *)&axis);
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
                effPCPFlashPolySphereExpPartSetVertex(work, index, (const f32 *)&axis);
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

PcpFlashRadialTriangleWork *effPCPFlashPolySphere2ExpCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolySphere2ExpCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashRadialTriangleWork *effPCPFlashPolySphere2ExpCopy(PcpFlashRadialTriangleParams *params)
{
    return effPCPFlashPolySphere2ExpCreate(params);
}

void effPCPFlashPolySphere2ExpDelete(PcpFlashRadialTriangleWork *work)
{
    dds3FlashPolyList2Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphere2ExpSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphere2ExpSetColor(PcpFlashRadialTriangleWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolySphere2ExpSetScale(PcpFlashRadialTriangleWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolySphere2ExpPartSetVC(PcpFlashRadialTriangleWork *work, s32 index, s32 color)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)dds3FlashPolyList2GetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->first = effMagaCylinderCalcColor(rgb2, color);
    slot->second = effMagaCylinderCalcColor(rgb2, color);
    slot->third = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
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

PcpFlashRadialStripWork *effPCPFlashPolySphere3CreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolySphere3Create(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashRadialStripWork *effPCPFlashPolySphere3Copy(PcpFlashRadialStripParams *params)
{
    return effPCPFlashPolySphere3Create(params);
}

void effPCPFlashPolySphere3Delete(PcpFlashRadialStripWork *work)
{
    dds3FlashPolyList3Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolySphere3SetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolySphere3SetColor(PcpFlashRadialStripWork *work, u32 value)
{
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
void effPCPFlashPolySphere3PartSetVertex(PcpFlashRadialStripWork *work, s32 index, const f32 *view)
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
    mirror = (f32 *)dds3FlashPolyList3GetVertex(work->resourceHandle, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(inner);
    SDF_VU0_STOREVECTOR(mirror);
}

void effPCPFlashPolySphere3PartRotate(PcpFlashRadialStripWork *work, s32 index, void *orientation)
{
    PcpFlashRadialStripParticle *part;

    part = &work->parts[index];
    part->angle += part->angularStep;
}

extern void effPCPFlashPolySphere3PartSetVC(PcpFlashRadialStripWork *, s32, u32);

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
                effPCPFlashPolySphere3PartSetVertex(work, index, (const f32 *)&axis);
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
                effPCPFlashPolySphere3PartSetVertex(work, index, (const f32 *)&axis);
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

PcpFlashFadingOrbitWork *effPCPFlashPolyRingExp2CreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolyRingExp2Create(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashFadingOrbitWork *effPCPFlashPolyRingExp2Copy(PcpFlashFadingOrbitParams *params)
{
    return effPCPFlashPolyRingExp2Create(params);
}

void effPCPFlashPolyRingExp2Delete(PcpFlashFadingOrbitWork *work)
{
    dds3FlashPolyListDelete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyRingExp2SetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyRingExp2SetColor(PcpFlashFadingOrbitWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolyRingExp2SetScale(PcpFlashFadingOrbitWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyRingExp2PartSetVC(PcpFlashFadingOrbitWork *work, s32 index, s32 color)
{
    EffFlashFanColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = dds3FlashPolyListGetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->color[0] = effMagaCylinderCalcColor(rgb2, color);
    slot->color[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        slot->color[2] = effMagaCylinderCalcColor(0x80000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        slot->color[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        slot->color[2] = effMagaCylinderCalcColor(0xFF000000, color);
        slot->color[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
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

void effPCPFlashPolyRingExp2PartRotate(PcpFlashFadingOrbitWork *work, s32 index)
{
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

PcpFlashOffsetRadialWork *effPCPFlashPolyCircleCreatePCF(const EffParamTable *data)
{
    return effPCPFlashPolyCircleCreate(dds3EffectPCPDataGetData(data, 0));
}

/* Duplicate callback: return fresh work from the source parameter prefix. */
PcpFlashOffsetRadialWork *effPCPFlashPolyCircleCopy(PcpFlashOffsetRadialParams *params)
{
    return effPCPFlashPolyCircleCreate(params);
}

void effPCPFlashPolyCircleDelete(PcpFlashOffsetRadialWork *work)
{
    dds3FlashPolyList2Delete(work->resourceHandle);
    sdfMemFreeDirect(work->allocationHandle);
}

void effPCPFlashPolyCircleSetPosition(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPFlashPolyCircleSetColor(PcpFlashOffsetRadialWork *work, u32 value)
{
    work->tintColor = value;
}

void effPCPFlashPolyCircleSetScale(PcpFlashOffsetRadialWork *work, f32 value)
{
    work->renderScale = value;
}

void effPCPFlashPolyCirclePartSetVC(PcpFlashOffsetRadialWork *work, s32 index, s32 color)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)dds3FlashPolyList2GetVC(work->resourceHandle, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    slot->first = effMagaCylinderCalcColor(rgb2, color);
    slot->second = effMagaCylinderCalcColor(rgb2, color);
    slot->third = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
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

#define EFF_RING_PARAM_BYTES 0x58

#define EFF_RING_HEADER_BYTES 0x80

#define EFF_RING_VERTEX_BYTES 16

#define EFF_POOL_HEADER_BYTES 0x70

#define EFF_PACKET_PARAMS_BYTES 0x2C

#define EFF_PACKET_LIST_BYTES 0x20

#define EFF_STATE_PACKET_BYTES 0x30

#define EFF_MATRIX_WORD_COUNT 16

#define EFF_FAN_VERTEX_COUNT 5

#define EFF_FAN_BATCH_VERTICES 0x19

#define EFF_FAN_BATCH_PARAMS 0xF

#define EFF_TRIANGLE_VERTEX_COUNT 3

#define EFF_TRIANGLE_BATCH_VERTICES 0x30

#define EFF_QUAD_VERTEX_COUNT 4

#define EFF_QUAD_BATCH_VERTICES 0x20

#define EFF_TRIANGLE_POSITION_BYTES 0x30

#define EFF_TRIANGLE_COLOR_BYTES 0xc

#define EFF_QUAD_POSITION_BYTES 0x40

#define EFF_QUAD_COLOR_BYTES 0x10

#define EFF_FAN_POSITION_BYTES 0x50

#define EFF_FAN_COLOR_BYTES 0x14

#define EFF_NEUTRAL_COLOR 0x80808080

#define EFF_RGB_MASK 0xFFFFFF

#define EFF_DIRECT_SURFACE_COUNT 4

extern void dds3FlashPolyListHDelete(dds3FlashPolyList_t *pool);

extern u32 effMagaCylinderCalcColor(u32 colorA, u32 colorB);

extern f32 *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *pool, s32 index);

extern u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *pool, s32 index);

extern f32 effPCPFlashPolyRingExpHVertex[];

extern sdf3DPrimPolyParm_t effRecordPoolDrawParams[];

extern u32 effFanPoolTriangleIndices[];

extern sdfGrOt_t *effScaledFanDrawSurfaces[];

extern sdfGrOt_t *effTransformedFanDrawSurfaces[];

extern sdfGrOt_t *effTrianglePoolDrawSurfaces[];

extern sdfGrOt_t *effQuadPoolDrawSurfaces[];

extern u32 effQuadPoolTriangleIndices[];

extern sdfGrOt_t D_00325248;

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

extern dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 count);

/* Allocate a circular fan group. Zero spread becomes one; count is not guarded. */
EffRingWork *effPCPFlashPolyRingExpHCreate(EffRingWork *source)
{
    sdfMemHandle_t *allocation;
    EffRingWork *ring;
    EffPcpRingBlock *block;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    allocation = sdfMemAllocDirect(source->count * EFF_RING_VERTEX_BYTES + EFF_RING_HEADER_BYTES);
    block = (EffPcpRingBlock *)sdfMemLock(allocation);
    ring = &block->header;
    memcpy(ring, source, EFF_RING_PARAM_BYTES);
    ring->vertices = &block->vertices[0];
    ring->allocationHandle = allocation;
    ring->color = EFF_NEUTRAL_COLOR;
    ring->currentRadius = ring->param38;
    ring->currentSideOffsetB = ring->param30;
    ring->currentSideOffsetA = ring->param34;
    ring->updateCount = 0;
    ring->scale = 1.0f;
    if (ring->spread == 0) {
        ring->spread = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    ring->recordPool = dds3FlashPolyListHCreate(ring->count);
    ring->recordPool->scale = 1.0f;
    ring->recordPool->drawMode = ring->drawMode;
    step = EFFECT_RING_FULL_TURN / ring->count;
    spread = ring->spread;
    for (i = 0; i < ring->count; i++) {
        ring->vertices[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % spread);
        ring->vertices[i].angle = angle;
        angle += step;
    }
    return ring;
}

/* Create a ring from the first parameter-table block. */
EffRingWork *effPCPFlashPolyRingExpHCreatePCF(const EffParamTable *table) {
    void *parameterBlock;

    parameterBlock = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyRingExpHCreate(parameterBlock);
}

/* Kind 85 duplicate: recreate the parameter prefix, not the live vertex state.
 * Retail tail-jumps to the constructor, preserving its v0 result despite this void C surface. */
EffRingWork *effPCPFlashPolyRingExpHCopy(EffRingWork *ring) {
    return effPCPFlashPolyRingExpHCreate(ring);
}

/* Release the matrix pool's resources before the ring's backing allocation. */
void effPCPFlashPolyRingExpHDelete(EffRingWork *ring) {
    dds3FlashPolyListHDelete(ring->recordPool);
    sdfMemFreeDirect(ring->allocationHandle);
}

/* Copy one quadword; the fourth component is copied along with XYZ. */
void effPCPFlashPolyRingExpHSetPosition(f32 *destination, const f32 *source) {
    SDF_VU0_COPYVECTOR(destination, source);
}

/* Replace the ring's packed modulation color without touching vertex colors. */
void effPCPFlashPolyRingExpHSetColor(EffRingWork *ring, u32 color) {
    ring->color = color;
}

/* Set the ring's overall render scale. */
void effPCPFlashPolyRingExpHSetScale(EffRingWork *work, f32 scale) {
    work->scale = scale;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPFlashPolyRingExpHSetRotateMatrix(EffRingWork *work, void *matrixSource) {
    VU0_LOAD_MATRIX(matrixSource);
    VU0_STORE_MATRIX(work->recordPool->matrix);
}

/* Fill five fan-vertex colors; odd and even groups use different alpha values. */
void effPCPFlashPolyRingExpHPartSetVC(EffRingWork *work, s32 index, s32 modulationColor) {
    EffRingColorSlot *slot;
    s32 firstRgb;
    s32 secondRgb;

    slot = (EffRingColorSlot *)dds3FlashPolyListHGetVC(work->recordPool, index);
    firstRgb = work->firstColor & EFF_RGB_MASK;
    secondRgb = work->secondColor & EFF_RGB_MASK;
    slot->colors[0] = effMagaCylinderCalcColor(secondRgb, modulationColor);
    slot->colors[1] = effMagaCylinderCalcColor(secondRgb, modulationColor);
    if (index & 1) {
        slot->colors[2] = effMagaCylinderCalcColor(0x80000000, modulationColor);
        slot->colors[3] = effMagaCylinderCalcColor(firstRgb | 0xFF000000, modulationColor);
        slot->colors[4] = effMagaCylinderCalcColor(0x80000000, modulationColor);
    } else {
        slot->colors[2] = effMagaCylinderCalcColor(0xFF000000, modulationColor);
        slot->colors[3] = effMagaCylinderCalcColor(firstRgb | 0x40000000, modulationColor);
        slot->colors[4] = effMagaCylinderCalcColor(0xFF000000, modulationColor);
    }
}

/* vu0 routine: build five orbiting fan positions from a phase-derived basis.
 * No camera matrix is read. Quadword loads retain the native scratch W values. */
void effPCPFlashPolyRingExpHPartSetVertex(EffRingWork *work, s32 index) {
    EffRingParticle *part = &work->vertices[index];
    f32 *positions = dds3FlashPolyListHGetVertex(work->recordPool, index);
    f32 orbitOffset[4];
    f32 radialDirection[4];
    f32 axisOffset[4];
    f32 sideOffsetA[4];
    f32 sideOffsetB[4];
    f32 sine;
    f32 basisFactor;

    VEC3_SPLAT(axisOffset, work->currentAxisOffset);
    VEC3_SPLAT(sideOffsetA, work->currentSideOffsetA);
    VEC3_SPLAT(sideOffsetB, work->currentSideOffsetB);
    radialDirection[0] = sdfCos(part->angle);
    radialDirection[1] = 0;
    sine = sdfSin(part->angle);
    radialDirection[2] = sine;
    orbitOffset[0] = radialDirection[0] * work->currentRadius;
    orbitOffset[1] = 0;
    orbitOffset[2] = sine * work->currentRadius;
    basisFactor = part->basisFactor;
    effPCPFlashPolyRingExpHVertex[0] = radialDirection[0] * basisFactor;
    effPCPFlashPolyRingExpHVertex[1] = basisFactor + -1.0f;
    effPCPFlashPolyRingExpHVertex[2] = sine * basisFactor;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingExpHVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(radialDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(sideOffsetA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(sideOffsetA);
    SDF_VU0_LOADVECTOR(sideOffsetB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(sideOffsetB);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(axisOffset);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(orbitOffset);
    SDF_VU0_LOADVECTOR_B(sideOffsetB);
    SDF_VU0_STOREVECTOR(positions + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(positions + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(positions + 16);
    SDF_VU0_LOADVECTOR(orbitOffset);
    SDF_VU0_LOADVECTOR_B(sideOffsetA);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(positions);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(positions + 4);
}

/* Advance one ring particle's angle; index is unchecked. */
void effPCPFlashPolyRingExpHPartRotate(EffRingWork *work, s32 index) {
    EffRingParticle *particle;

    particle = &work->vertices[index];
    particle->angle = particle->angle + work->increment;
}

extern void dds3FlashPolyListHDraw(dds3FlashPolyList_t *);

void effPCPFlashPolyRingExpHDraw(EffRingWork *work)
{
    s32 index;
    s32 count;
    EffRingParticle *part;
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

    part = work->vertices;
    count = work->count;
    lifetime = work->duration;
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    restart = work->respawn;
    randomRange = work->spread;
    tintColor = work->color;
    heightDelta = work->param4C - work->param48;
    progress = 1.0f;
    if (lifetime > 0) {
        progress = (f32)work->updateCount / (f32)lifetime;
    }
    start = work->param38;
    radiusEnd = work->param3C;
    delta = radiusEnd - start;
    work->currentRadius = start + delta * progress;
    if (start > 0.0f) {
        factor = radiusEnd / start;
    } else {
        factor = radiusEnd;
    }
    upStart = work->param30;
    acrossStart = work->param34;
    upEnd = upStart * factor;
    acrossEnd = acrossStart * factor;
    start = work->param40;
    normalEnd = work->param44;
    delta = upEnd - upStart;
    work->currentSideOffsetB = upStart + delta * progress;
    delta = acrossEnd - acrossStart;
    work->currentSideOffsetA = acrossStart + delta * progress;
    delta = normalEnd - start;
    work->currentAxisOffset = start + delta * progress;

    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;

        if (age == 0) {
            part->basisFactor = work->param48;
            effPCPFlashPolyRingExpHPartSetVertex(work, index);
            effPCPFlashPolyRingExpHPartSetVC(work, index, 0);
            part->color = 0x80808080;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % randomRange);
            }
            color = 0;
            effPCPFlashPolyRingExpHPartSetVC(work, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->basisFactor = heightDelta * progress + work->param48;
            effPCPFlashPolyRingExpHPartRotate(work, index);
            effPCPFlashPolyRingExpHPartSetVertex(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->color, factor), tintColor);
            effPCPFlashPolyRingExpHPartSetVC(work, index, color);
        }
        part->age = part->age + 1;
    }
    if (work->updateCount == lifetime) {
        work->updateCount = 0;
    } else {
        work->updateCount++;
    }
    handle = work->recordPool;
    handle->origin[0] = work->origin[0];
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->scale;
    dds3FlashPolyListHDraw(handle);
}

extern dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 count);

/* Allocate five 16-byte positions and five color words per group, then the 0x70-byte header. */
dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 count) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u8 *block;
    s32 vertexCount;
    s32 recordBytes;
    s32 size;
    s32 quads;

    vertexCount = count * 5;
    recordBytes = vertexCount * 5 * 4;
    quads = recordBytes >> 4;
    recordBytes = ((recordBytes & 0xF) != 0 ? quads + 1 : quads) << 4;
    size = recordBytes + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = (u8 *)sdfMemLock(handle);
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + recordBytes);
    pool->recordBase = (u8 *)block;
    pool->auxRecordBase = (u8 *)(block + vertexCount * 16);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = vertexCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfMatNew();
    sdfMatSetConstant(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->attr = 0x4000;
    return pool;
}

/* Queue asset release, then free the pool allocation; neither handle is cleared. */
void dds3FlashPolyListDelete(dds3FlashPolyList_t *group) {
    sdfMatFreeDelayed(group->resource);
    sdfMemFreeDirect(group->buffer);
}

/* Build scale/origin transform and submit five-vertex groups in batches of 25.
 * The remainder path retains its native vertex count, even for a partial group. */
void dds3FlashPolyListDraw(dds3FlashPolyList_t *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    sdfDrawTag_t *stateList;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = work->scale;
    matrix[5] = work->scale;
    matrix[10] = work->scale;
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, work->resource, 0);
    remaining = work->vertexCount;
    effRecordPoolDrawParams->vertexColor = (u32 *)work->auxRecordBase;
    effRecordPoolDrawParams->vertex = (u128 *)work->recordBase;
    effRecordPoolDrawParams->color = work->color;
    effRecordPoolDrawParams->faceCount = EFF_FAN_BATCH_PARAMS;
    effRecordPoolDrawParams->pointCount = EFF_FAN_BATCH_VERTICES;
    effRecordPoolDrawParams->faceIndex = effFanPoolTriangleIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->vertex += EFF_FAN_BATCH_VERTICES;
        effRecordPoolDrawParams->vertexColor += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effRecordPoolDrawParams->pointCount = remaining;
        effRecordPoolDrawParams->faceCount = fanCount * 3;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        effScaledFanDrawSurfaces[work->drawMode]->append(effScaledFanDrawSurfaces[work->drawMode], list);
    } else {
        stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
        sdfDrawTagInit(stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfDrawTagLink(stateList, (u32)packet);
        D_00325248.append(&D_00325248, stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
        D_00325248.append(&D_00325248, list);
    }
}

/* Address the five-position record for a group; no index bounds check. */
f32 *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (f32 *)(group->recordBase + groupIndex * EFF_FAN_POSITION_BYTES);
}

/* Address the group's five packed color words. */
EffFlashFanColorSlot *dds3FlashPolyListGetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (EffFlashFanColorSlot *)(group->auxRecordBase + groupIndex * EFF_FAN_COLOR_BYTES);
}

/* Store the record pool's raw submission-control bits. */
void dds3FlashPolyListSetBlend(dds3FlashPolyList_t *group, u32 incrementBits) {
    group->drawMode = incrementBits;
}

/* Store the record pool's packed modulation color. */
void dds3FlashPolyListSetABGR(dds3FlashPolyList_t *group, u32 color) {
    group->color = color;
}

/* Set the pool scale. */
void dds3FlashPolyListSetScale(dds3FlashPolyList_t *work, f32 scale) {
    work->scale = scale;
}

/* Allocate three positions and three colors per triangle, then the header.
 * The complete block and shared packet-parameter record are cleared. */
dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 triangleCount) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 vertexCount;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    vertexCount = triangleCount * EFF_TRIANGLE_VERTEX_COUNT;
    positionWordCount = vertexCount * 4;
    colorWordCount = vertexCount;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = (u32 *)sdfMemLock(handle);
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (u8 *)block;
    pool->auxRecordBase = (u8 *)(block + positionWordCount);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfMatNew();
    sdfMatSetConstant(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->attr = 0x4000;
    return pool;
}

/* Release the asset and backing allocation; the header points into that block. */
void dds3FlashPolyList2Delete(dds3FlashPolyList_t *group) {
    sdfMatFreeDelayed(group->resource);
    sdfMemFreeDirect(group->buffer);
}

/* Submit the position/color pool in triangle batches of at most 48 vertices. */
void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    sdfDrawTag_t *packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    sdfGrOt_t *surface;

    sdfDrawTagInit(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(packet, 0);
    sdf3DPrimLinkMat(packet, pool->resource, 0);
    remainingVertices = pool->vertexCount;
    effRecordPoolDrawParams->vertexColor = (u32 *)pool->auxRecordBase;
    effRecordPoolDrawParams->vertex = (u128 *)pool->recordBase;
    effRecordPoolDrawParams->color = pool->color;
    effRecordPoolDrawParams->faceCount = 0x10;
    effRecordPoolDrawParams->pointCount = EFF_TRIANGLE_BATCH_VERTICES;
    effRecordPoolDrawParams->faceIndex = NULL;
    while (remainingVertices >= EFF_TRIANGLE_BATCH_VERTICES) {
        remainingVertices -= EFF_TRIANGLE_BATCH_VERTICES;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->vertex += EFF_TRIANGLE_BATCH_VERTICES;
        effRecordPoolDrawParams->vertexColor += EFF_TRIANGLE_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_TRIANGLE_VERTEX_COUNT) {
        effRecordPoolDrawParams->faceCount = remainingVertices / EFF_TRIANGLE_VERTEX_COUNT;
        effRecordPoolDrawParams->pointCount = remainingVertices;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    surface = effTrianglePoolDrawSurfaces[pool->drawMode];
    surface->append(surface, packet);
}

/* Address three quadword positions for one triangle. */
f32 *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (f32 *)(group->recordBase + groupIndex * EFF_TRIANGLE_POSITION_BYTES);
}

/* Address three packed color words for one triangle. */
void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->auxRecordBase + groupIndex * EFF_TRIANGLE_COLOR_BYTES;
}

/* Allocate four positions and four colors per quad, followed by the header.
 * Both allocation and shared packet parameters are cleared before defaults. */
dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 quadCount) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    positionWordCount = quadCount * 16;
    colorWordCount = quadCount * EFF_QUAD_VERTEX_COUNT;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = (u32 *)sdfMemLock(handle);
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (u8 *)block;
    pool->drawMode = 2;
    pool->auxRecordBase = (u8 *)(block + positionWordCount);
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->color = EFF_NEUTRAL_COLOR;
    pool->resource = sdfMatNew();
    sdfMatSetConstant(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->attr = 0x4000;
    return pool;
}

/* Queue the asset before releasing the allocation containing this header. */
void dds3FlashPolyList3Delete(dds3FlashPolyList_t *group) {
    sdfMatFreeDelayed(group->resource);
    sdfMemFreeDirect(group->buffer);
}

/* Submit the position/color pool in quad batches of at most 32 vertices. */
void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    sdfDrawTag_t *packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    sdfGrOt_t *surface;

    sdfDrawTagInit(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(packet, 0);
    sdf3DPrimLinkMat(packet, pool->resource, 0);
    remainingVertices = pool->vertexCount;
    effRecordPoolDrawParams->vertexColor = (u32 *)pool->auxRecordBase;
    effRecordPoolDrawParams->vertex = (u128 *)pool->recordBase;
    effRecordPoolDrawParams->color = pool->color;
    effRecordPoolDrawParams->faceCount = 0x10;
    effRecordPoolDrawParams->pointCount = EFF_QUAD_BATCH_VERTICES;
    effRecordPoolDrawParams->faceIndex = effQuadPoolTriangleIndices;
    while (remainingVertices >= EFF_QUAD_BATCH_VERTICES) {
        remainingVertices -= EFF_QUAD_BATCH_VERTICES;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->vertex += EFF_QUAD_BATCH_VERTICES;
        effRecordPoolDrawParams->vertexColor += EFF_QUAD_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_QUAD_VERTEX_COUNT) {
        effRecordPoolDrawParams->faceCount = remainingVertices / EFF_QUAD_VERTEX_COUNT * 2;
        effRecordPoolDrawParams->pointCount = remainingVertices;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    surface = effQuadPoolDrawSurfaces[pool->drawMode];
    surface->append(surface, packet);
}

/* Address four quadword positions for one quad. */
f32 *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (f32 *)(group->recordBase + groupIndex * EFF_QUAD_POSITION_BYTES);
}

/* Address four packed color words for one quad. */
void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->auxRecordBase + groupIndex * EFF_QUAD_COLOR_BYTES;
}

/* Allocate five-vertex fan groups, then set the returned header's matrix. */
dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 fanCount) {
    dds3FlashPolyList_t *pool = dds3FlashPolyListCreate(fanCount);

    EE_MMI_UNIT_MATRIX(pool->matrix);
    return pool;
}

void dds3FlashPolyListHDelete(dds3FlashPolyList_t *pool) {
    dds3FlashPolyListDelete(pool);
}

/* Apply the stored matrix/origin, then submit batches of five-vertex groups.
 * Native state-only and drawing lists remain separate for non-direct modes. */
void dds3FlashPolyListHDraw(dds3FlashPolyList_t *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    sdfDrawTag_t *stateList;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    VU0_COPY_MATRIX(matrix, work->matrix);
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, work->resource, 0);
    remaining = work->vertexCount;
    effRecordPoolDrawParams->vertexColor = (u32 *)work->auxRecordBase;
    effRecordPoolDrawParams->vertex = (u128 *)work->recordBase;
    effRecordPoolDrawParams->color = work->color;
    effRecordPoolDrawParams->faceCount = EFF_FAN_BATCH_PARAMS;
    effRecordPoolDrawParams->pointCount = EFF_FAN_BATCH_VERTICES;
    effRecordPoolDrawParams->faceIndex = effFanPoolTriangleIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->vertex += EFF_FAN_BATCH_VERTICES;
        effRecordPoolDrawParams->vertexColor += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effRecordPoolDrawParams->pointCount = remaining;
        effRecordPoolDrawParams->faceCount = fanCount * 3;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        effTransformedFanDrawSurfaces[work->drawMode]->append(effTransformedFanDrawSurfaces[work->drawMode], list);
    } else {
        stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
        sdfDrawTagInit(stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfDrawTagLink(stateList, (u32)packet);
        D_00325248.append(&D_00325248, stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
        D_00325248.append(&D_00325248, list);
    }
}

/* Address the five-position record for one fan group. */
f32 *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (f32 *)(group->recordBase + groupIndex * EFF_FAN_POSITION_BYTES);
}

/* Address the five color words for one fan group. */
u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (u32 *)(group->auxRecordBase + groupIndex * EFF_FAN_COLOR_BYTES);
}

/* Select the record pool's packet submission mode. */
void dds3FlashPolyListHSetBlend(dds3FlashPolyList_t *pool, u32 mode) {
    pool->drawMode = mode;
}

/* Set the packed color word passed to the pool's packet builder. */
void dds3FlashPolyListHSetABGR(dds3FlashPolyList_t *pool, u32 color) {
    pool->color = color;
}

/* Set the scalar used by the scale/origin rendering path. */
void dds3FlashPolyListHSetScale(dds3FlashPolyList_t *work, f32 scale) {
    work->scale = scale;
}
