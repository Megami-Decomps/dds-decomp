#include "sdk_ps2.h"
#include "common.h"
#include "eff_packet_params.h"
#include "eff_random.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_scatter_draw.h"
#include "eff_pcp_scatter_rings.h"


extern PcpScatterRes *dds3ScatterPolyListTexResourceCreate(
    const struct SdfTextureFileHeader *textureHeader);
extern PcpScatterRes *dds3ScatterPolyListTexResourceCopy(PcpScatterRes *);
extern void dds3ScatterPolyListTexResourceDelete(PcpScatterRes *);

/* The four setters below belong to the flat-ring effect's 0x13C-byte
 * instance, not to the drawable used by the resource helpers below. */

/* Copy the flat effect's source vector; the renderer reads the embedded copy. */
void effPCPWwSpherePolySetPosition(PcpScatterPlainInstance *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->params.origin, position);
}

/* Set the instance scale that its update forwards to the drawable. */
void effPCPWwSpherePolySetScale(PcpScatterPlainInstance *work, f32 scale) {
    work->scale = scale;
}

/* Set the instance color used by the particle fade pass. */
void effPCPWwSpherePolySetColor(PcpScatterPlainInstance *work, u32 color) {
    work->color = color;
}

/* vu0 routine: copy a 4x4 matrix through vf28-vf31 */
void effPCPWwSpherePolySetRotateMatrix(PcpScatterPlainInstance *work, void *src) {
    VU0_COPY_MATRIX(work->matrix, src);
}

extern EffPacketParams effScatterDrawParams;

/* Allocate the drawable's four streams and initialize its mirrored alpha ramp. */
dds3WhirlwindPolyList_tag *dds3WhirlwindPolyListCreate(u32 count, u32 pairs) {
    s32 vectors = pairs * 2 + 2;
    s32 totalVectors = vectors * count;
    s32 bytes = (totalVectors * 6 + vectors + count) * 4;
    dds3WhirlwindPolyList_tag *object;
    struct sdfMemHandle_t *allocation;
    u8 *cursor;
    u32 quarter;
    u32 i;
    u32 *left;
    u32 *right;
    f32 alpha;
    f32 step;
    u32 color;

    object = sdfChipAllocDirect(sizeof(dds3WhirlwindPolyList_tag));
    allocation = sdfMemAllocDirect(bytes);
    cursor = (u8 *)sdfMemLock(allocation);
    memset(cursor, 0, bytes);
    object->points = (f32 *)cursor;
    cursor += totalVectors * 16;
    object->uv = (f32 *)cursor;
    cursor += totalVectors * 8;
    object->vertexColors = (u32 *)cursor;
    cursor += vectors * 4;
    object->colors = (u32 *)cursor;
    object->color = 0x80808080;
    object->vectorsPerParticle = vectors;
    object->packetQueueIndex = 1;
    object->particleCount = count;
    object->allocation = allocation;
    object->scale = 1.0f;
    object->sharedResource = NULL;
    object->asset = sdfMatNew();
    sdfMatSetConstant(object->asset, 1.0f);
    memset(object->vertexColors, 0x80, vectors * 4);
    quarter = vectors >> 2;
    if (quarter >= 2) {
        left = object->vertexColors;
        right = left + vectors - 2;
        alpha = 0.0f;
        step = 1.0f / quarter;
        for (i = 0; i < quarter; i++) {
            color = ((u32)(alpha * 128.0f) << 24) | 0x808080;
            alpha += step;
            left[0] = color;
            left[1] = left[0];
            right[0] = left[0];
            right[1] = left[0];
            left += 2;
            right -= 2;
        }
    }
    memset(&effScatterDrawParams, 0, sizeof(EffPacketParams));
    effScatterDrawParams.primitive = 0x4000;
    return object;
}

/* Release the shared scatter resource before the object's private assets. */
void dds3WhirlwindPolyListDelete(dds3WhirlwindPolyList_tag *object) {
    if (object->sharedResource != 0) {
        dds3ScatterPolyListTexResourceDelete(object->sharedResource);
    }
    sdfMatFreeDelayed(object->asset);
    sdfMemFreeDirect(object->allocation);
    sdfChipFreeDirect(object);
}

extern u8 effScatterTriangleIndices[];
extern SdfPoolNode *effScatterDrawSurfaces[];
#include "sdf_packet_alloc.h"
extern void *effPkt3DPrimMakePoly(EffPacketParams *);
struct sdfMat_t;
extern f32 *dds3WhirlwindPolyListGetVertex(dds3WhirlwindPolyList_tag *object, s32 particleIndex);
extern f32 *dds3WhirlwindPolyListGetUV(dds3WhirlwindPolyList_tag *object, s32 particleIndex);
extern u32 dds3WhirlwindPolyListGetABGR(dds3WhirlwindPolyList_tag *object, s32 particleIndex);

void dds3WhirlwindPolyListDraw(dds3WhirlwindPolyList_tag *object) {
    f32 matrix[16] __attribute__((aligned(16)));
    sdfDrawTag_t *packet;
    EffPacketParams *draw;
    s32 count;
    s32 remaining;
    s32 particleIndex;
    SdfPoolNode *surface;

    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[10] = matrix[5] = matrix[0] = object->scale;
    object->matrix[12] = object->origin[0];
    object->matrix[13] = object->origin[1];
    object->matrix[14] = object->origin[2];
    VU0_LOAD_MATRIX(object->matrix);
    SDF_VU0_LOADMATRIX_B(matrix);
    sdfComposeVuMatrixFromRegisters();
    sdf3DPrimLinkTm(packet, 0);
    if (object->sharedResource != NULL) {
        sdfSetAssetPrimaryTextureAddress((struct sdfMat_t *)object->asset, object->sharedResource->textureHandle);
    }
    sdf3DPrimLinkMat(packet, (sdfMat_t *)object->asset, 0);
    count = (s32)object->particleCount;
    effScatterDrawParams.parameters = (const u32 *)effScatterTriangleIndices;
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        draw = &effScatterDrawParams;
        remaining = object->vectorsPerParticle;
        draw->positions = (u128 *)dds3WhirlwindPolyListGetVertex(object, particleIndex);
        draw->texcoords = dds3WhirlwindPolyListGetUV(object, particleIndex);
        draw->colors = object->vertexColors;
        draw->color = dds3WhirlwindPolyListGetABGR(object, particleIndex);
        draw->parameterCount = 16;
        draw->vertexCount = 18;
        if ((draw->color & 0xFF000000) != 0) {
            while (remaining >= 18) {
                remaining -= 16;
                sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&effScatterDrawParams));
                effScatterDrawParams.positions += 16;
                effScatterDrawParams.colors += 16;
                effScatterDrawParams.texcoords += 32;
            }
            if (remaining >= 4) {
                draw->parameterCount = remaining - 2;
                draw->vertexCount = remaining;
                sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(draw));
            }
        }
    }
    surface = effScatterDrawSurfaces[object->packetQueueIndex];
    surface->append(surface, packet);
}


/* Give this object its own reference to a newly created scatter resource. */
void dds3WhirlwindPolyListSetTex(
    dds3WhirlwindPolyList_tag *object, const struct SdfTextureFileHeader *resource) {
    PcpScatterRes *shared;

    shared = dds3ScatterPolyListTexResourceCreate(resource);
    object->sharedResource = shared;
}

/* Share another object's scatter resource while retaining a separate reference. */
void dds3WhirlwindPolyListCopyTex(dds3WhirlwindPolyList_tag *object, dds3WhirlwindPolyList_tag *source) {
    PcpScatterRes *shared;

    shared = dds3ScatterPolyListTexResourceCopy(source->sharedResource);
    object->sharedResource = shared;
}

/* Compute the address of a 16-byte-wide block within the stride. */
f32 *dds3WhirlwindPolyListGetVertex(dds3WhirlwindPolyList_tag *object, s32 particleIndex) {
    return object->points + particleIndex * object->vectorsPerParticle * 4;
}

/* Compute the address of an 8-byte-wide block within the stride. */
f32 *dds3WhirlwindPolyListGetUV(dds3WhirlwindPolyList_tag *object, s32 particleIndex) {
    return object->uv + particleIndex * object->vectorsPerParticle * 2;
}

u32 dds3WhirlwindPolyListGetABGR(dds3WhirlwindPolyList_tag *object, s32 particleIndex) {
    return object->colors[particleIndex];
}

/* vu0 routine: copy a 4x4 matrix into the destination's second slot */
void dds3WhirlwindPolyListSetRotateMatrix(dds3WhirlwindPolyList_tag *object, const f32 sourceMatrix[16]) {
    VU0_LOAD_MATRIX(sourceMatrix);
    VU0_STORE_MATRIX(object->matrix);
}

extern EffRandState effDefaultRandomState[];

/* effPCPNeedleSpiralCreate */
EffPCPNeedleWork *effPCPNeedleSpiralCreate(EffPCPNeedleParams *params) {
    sdfMemHandle_t *allocation = sdfMemAllocDirect(params->count * sizeof(EffPCPNeedleSlot) +
                                                  sizeof(EffPCPNeedleWork));
    EffPCPNeedleWork *work = (EffPCPNeedleWork *)sdfMemLock(allocation);
    u32 i;
    EffResourceWork *resource;
    u32 count;
    u32 delayRange;
    EffPCPNeedleSlot *slot;
    f32 radiusBase;
    f32 radiusJitter;

    work->slots = (EffPCPNeedleSlot *)(work + 1);
    work->params = *params;
    work->allocationHandle = allocation;
    work->count = params->count;
    work->color = 0x80808080;
    work->system = 0;
    resource = dds3NeedleModelCreate(params->count);
    resource->mode = params->mode;
    work->resource = resource;
    effBuildRadialFanStreams(resource, params->fanSegments, params->centerColor,
                            params->outerColor, params->radiusScale, params->viewOffset);
    work->system = dds3TrackPolyCreate(work->count, work->params.perCellInput, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->system, 1, work->params.unk58, work->params.unk58);
    dds3TrackPolySetMode(work->system, (u16)work->params.mode);

    count = work->count;
    delayRange = params->randomDelayRange;
    slot = work->slots;
    if (delayRange == 0) {
        delayRange = 1;
    }
    radiusBase = work->params.radiusBase;
    radiusJitter = work->params.radiusJitter;
    for (i = 0; i < count; i++, slot++) {
        slot->angle = dds3GetRandFloatA(effDefaultRandomState) * EFFECT_RING_FULL_TURN;
        slot->radius = radiusBase * (dds3GetRandFloatA(effDefaultRandomState) *
                                   radiusJitter + (1.0f - radiusJitter));
        slot->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayRange);
        dds3NeedleModelSetColor(work->resource, i, 0x808080);
    }
    return work;
}
