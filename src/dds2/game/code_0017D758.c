#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_scatter_draw.h"
#include "eff_pcp_scatter_rings.h"


extern PcpScatterRes *effPcpScatterResCreate(u32);
extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *);
extern void effPcpScatterResRelease(PcpScatterRes *);

/* The four setters below belong to the flat-ring effect's 0x13C-byte
 * instance, not to the drawable used by the resource helpers below. */

/* Copy the flat effect's source vector; the renderer reads the embedded copy. */
void effScatterCopyFlatParameterVector(PcpScatterPlainInstance *work, void *src) {
    PCP_COPY_VECTOR(work->params.origin, src);
}

/* Set the instance scale that its update forwards to the drawable. */
void effScatterSetFlatInstanceScale(PcpScatterPlainInstance *work, f32 value) {
    work->scale = value;
}

/* Set the instance color used by the particle fade pass. */
void effScatterSetFlatInstanceColor(PcpScatterPlainInstance *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix through vf28-vf31 */
void effScatterCopyFlatInstanceMatrix(PcpScatterPlainInstance *work, void *src) {
    VU0_COPY_MATRIX(work->matrix, src);
}

INCLUDE_ASM(const s32, "game/code_0017D758", effScatterCreateDrawObject);

/* Release the shared scatter resource before the object's private assets. */
void effReleaseScatterObject(PcpScatterDraw *object) {
    if (object->sharedResource != 0) {
        effPcpScatterResRelease(object->sharedResource);
    }
    sdfQueueAssetRelease((s32)object->asset);
    sdfReleaseResourceAllocation(object->allocation);
    sdfReleaseChipBlock(object);
}

/* Render the scatter strips in sixteen-vertex batches and submit the tail. */
typedef struct ScatterRenderState {
    u16 primitiveCount;
    u16 vertexCount;
    u16 flags;
    u8 pad06[2];
    u32 color;
    void *parameters;
    f32 *points;
    u8 pad14[4];
    f32 *uv;
    u8 pad1C[4];
    u32 *colors;
    u8 pad24[8];
} ScatterRenderState;

extern ScatterRenderState D_00452050;
extern u8 D_003B14C0[];
extern SdfPoolNode *D_003B1520[];
extern s32 sdfAllocPacketAligned(s32);
extern void sdfComposeVuMatrixFromRegisters(void);
extern s32 func_00167A10(ScatterRenderState *);
struct SdfAsset;
extern void sdfSetAssetPrimaryTextureAddress(struct SdfAsset *, u32);
extern f32 *effGetScatterWideBlock(PcpScatterDraw *, s32);
extern f32 *effGetScatterNarrowBlock(PcpScatterDraw *, s32);
extern u32 effGetScatterEntry(PcpScatterDraw *, s32);

void effScatterDrawObject(PcpScatterDraw *object) {
    f32 matrix[16] __attribute__((aligned(16)));
    SdfListHead *packet;
    ScatterRenderState *draw;
    s32 count;
    s32 remaining;
    s32 index;
    SdfPoolNode *surface;

    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[10] = matrix[5] = matrix[0] = object->scale;
    object->matrix[12] = object->origin[0];
    object->matrix[13] = object->origin[1];
    object->matrix[14] = object->origin[2];
    VU0_LOAD_MATRIX(object->matrix);
    VU0_LOAD_MATRIX_B(matrix);
    sdfComposeVuMatrixFromRegisters();
    sdfConsAppendVuPacket(packet, 0);
    if (object->sharedResource != NULL) {
        sdfSetAssetPrimaryTextureAddress((struct SdfAsset *)object->asset, (u32)object->sharedResource->textureHandle);
    }
    sdfConsAppendAssetPacket(packet, (SdfAsset *)object->asset, 0);
    count = (s32)object->particleCount;
    D_00452050.parameters = D_003B14C0;
    for (index = 0; index < count; index++) {
        draw = &D_00452050;
        remaining = object->vectorsPerParticle;
        draw->points = effGetScatterWideBlock(object, index);
        draw->uv = effGetScatterNarrowBlock(object, index);
        draw->colors = object->vertexColors;
        draw->color = effGetScatterEntry(object, index);
        draw->primitiveCount = 16;
        draw->vertexCount = 18;
        if ((draw->color & 0xFF000000) != 0) {
            while (remaining >= 18) {
                remaining -= 16;
                sdfAppendPacket(packet, func_00167A10(&D_00452050));
                D_00452050.points += 64;
                D_00452050.colors += 16;
                D_00452050.uv += 32;
            }
            if (remaining >= 4) {
                draw->primitiveCount = remaining - 2;
                draw->vertexCount = remaining;
                sdfAppendPacket(packet, func_00167A10(draw));
            }
        }
    }
    surface = D_003B1520[object->packetQueueIndex];
    surface->append((SdfListHead *)surface, packet);
}


/* Give this object its own reference to a newly created scatter resource. */
void effCreateScatterResource(PcpScatterDraw *object, u32 resource) {
    PcpScatterRes *shared;

    shared = effPcpScatterResCreate(resource);
    object->sharedResource = shared;
}

/* Share another object's scatter resource while retaining a separate reference. */
void effShareScatterResource(PcpScatterDraw *object, PcpScatterDraw *source) {
    PcpScatterRes *shared;

    shared = effPcpScatterResAddRef(source->sharedResource);
    object->sharedResource = shared;
}

/* Compute the address of a 16-byte-wide block within the stride. */
f32 *effGetScatterWideBlock(PcpScatterDraw *object, s32 index) {
    return object->points + index * object->vectorsPerParticle * 4;
}

/* Compute the address of an 8-byte-wide block within the stride. */
f32 *effGetScatterNarrowBlock(PcpScatterDraw *object, s32 index) {
    return object->uv + index * object->vectorsPerParticle * 2;
}

u32 effGetScatterEntry(PcpScatterDraw *object, s32 index) {
    return object->colors[index];
}

/* vu0 routine: copy a 4x4 matrix into the destination's second slot */
void effScatterStoreSourceTransformMatrix(PcpScatterDraw *object, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(object->matrix);
}

extern void parDispatchSub(u32, s32, s32, s32);
extern u32 effMiscRand(void *);
extern f32 effMiscRandUnitFloat(void *);
extern u8 effDefaultRandomState[];

/* effNeedleCreateWork */
EffPCPNeedleWork *effNeedleCreateWork(EffPCPNeedleParams *params) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(params->count * sizeof(EffPCPNeedleSlot) +
                                                  sizeof(EffPCPNeedleWork));
    EffPCPNeedleWork *work = (EffPCPNeedleWork *)sdfResourceRetainAddress(allocation);
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
    resource = effCreateResourceEntryWork(params->count);
    resource->mode = params->mode;
    work->resource = resource;
    effBuildRadialFanStreams(resource, params->fanSegments, params->centerColor,
                            params->outerColor, params->radiusScale, params->viewOffset);
    work->system = parAllocateCellSystem(work->count, work->params.unk54, 0, PAR_CELL_TOPOLOGY_PAIR);
    parDispatchSub((u32)work->system, 1, work->params.unk58, work->params.unk58);
    parSetCellDrawBucket(work->system, (u16)work->params.mode);

    count = work->count;
    delayRange = params->randomDelayRange;
    slot = work->slots;
    if (delayRange == 0) {
        delayRange = 1;
    }
    radiusBase = work->params.radiusBase;
    radiusJitter = work->params.radiusJitter;
    for (i = 0; i < count; i++, slot++) {
        slot->angle = effMiscRandUnitFloat(effDefaultRandomState) * EFFECT_RING_FULL_TURN;
        slot->radius = radiusBase * (effMiscRandUnitFloat(effDefaultRandomState) *
                                   radiusJitter + (1.0f - radiusJitter));
        slot->age = -(effMiscRand(effDefaultRandomState) % delayRange);
        effSetResourceEntryValue(work->resource, i, 0x808080);
    }
    return work;
}
