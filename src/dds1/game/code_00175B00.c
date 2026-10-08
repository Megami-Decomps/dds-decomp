#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff.h"


/* Drawable allocation is 0x80 bytes; its resource and geometry arrays are
 * independent of the effect instance that supplies the transform and scale. */
typedef struct PcpScatterDraw {
    f32 origin[4];
    f32 matrix[16];
    u32 unk50;
    u32 color;
    u32 particleCount;
    s32 stride;
    f32 scale;
    f32 *points;
    f32 *uv;
    u32 *vertexColors;
    u32 *colors;
    u32 asset;
    u32 allocation;
    PcpScatterRes *sharedResource;
} PcpScatterDraw;

typedef struct PcpScatterPlainParams {
    f32 vec[4];
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
    s32 colorParam;
    u32 vCount;
    u32 vTail;
    u8 pad68[0x80];
} PcpScatterPlainParams;

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

/* The four setter callbacks belong to the flat-ring effect's 0x13C-byte
 * instance, not to the drawable used by the resource helpers below. */
typedef struct PcpScatterPlainInstance {
    f32 matrix[16];
    PcpScatterPlainParams params;
    PcpScatterPlainParticle *particles;
    f32 scale;
    u32 color;
    u32 scatterObject;
    u32 ownedBuffer;
} PcpScatterPlainInstance;

extern PcpScatterRes *effPcpScatterResAddRef(PcpScatterRes *);

extern PcpScatterRes *effPcpScatterResCreate(u32);
extern void effPcpScatterResRelease(PcpScatterRes *);

/* Copy the flat effect's source vector; the renderer reads the embedded copy. */
void effScatterCopyFlatParameterVector(PcpScatterPlainInstance *work, void *src) {
    PCP_COPY_VECTOR(work->params.vec, src);
}

/* Set the instance scale that its update forwards to the drawable. */
void effScatterSetFlatInstanceScale(PcpScatterPlainInstance *work, f32 value) {
    work->scale = value;
}

/* Set the instance color used by the particle fade pass. */
void effScatterSetFlatInstanceColor(PcpScatterPlainInstance *work, u32 value) {
    work->color = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effScatterCopyFlatInstanceMatrix(PcpScatterPlainInstance *work, void *src) {
    VU0_COPY_MATRIX(work->matrix, src);
}

INCLUDE_ASM(const s32, "game/code_00175B00", func_00175B50);

/* Release the shared scatter resource before the object's private assets. */
void effReleaseScatterObject(PcpScatterDraw *object) {
    PcpScatterDraw *current;

    current = object;
    if (current->sharedResource != 0) {
        effPcpScatterResRelease(current->sharedResource);
    }
    sdfQueueAssetRelease(current->asset);
    sdfReleaseResourceAllocation(current->allocation);
    sdfReleaseChipBlock(object);
}

/* Per-draw state consumed by the scatter packet builder (0x30 bytes). */
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

extern ScatterRenderState D_003D65B0;
extern u8 D_00354B90[];
extern SdfPoolNode *D_00354BF0[];
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfComposeVuMatrixFromRegisters(void);
extern void sdfConsAppendVuPacket(s32, s32 (*)(s32));
extern void sdfConsAppendAssetPacket(s32, void *, s32 (*)(s32));
extern void sdfAppendPacket(SdfListHead *, u32);
extern s32 func_0015FE20(ScatterRenderState *);
struct SdfTextParam;
extern void func_002DA438(struct SdfTextParam *, u32);

/* Render scatter strips in batches of sixteen vertices, then submit the tail. */
void func_00175DD0(PcpScatterDraw *object) {
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
    sdfConsAppendVuPacket((s32)packet, 0);
    if (object->sharedResource != NULL) {
        func_002DA438((struct SdfTextParam *)object->asset,
                      (u32)object->sharedResource->textureHandle);
    }
    sdfConsAppendAssetPacket((s32)packet, (void *)object->asset, 0);
    count = (s32)object->particleCount;
    D_003D65B0.parameters = D_00354B90;
    for (index = 0; index < count; index++) {
        draw = &D_003D65B0;
        remaining = object->stride;
        draw->points = (f32 *)effGetScatterWideBlock(object, index);
        draw->uv = (f32 *)effGetScatterNarrowBlock(object, index);
        draw->colors = object->vertexColors;
        draw->color = effGetScatterEntry(object, index);
        draw->primitiveCount = 16;
        draw->vertexCount = 18;
        if ((draw->color & 0xFF000000) != 0) {
            while (remaining >= 18) {
                remaining -= 16;
                sdfAppendPacket(packet, func_0015FE20(&D_003D65B0));
                D_003D65B0.points += 64;
                D_003D65B0.colors += 16;
                D_003D65B0.uv += 32;
            }
            if (remaining >= 4) {
                draw->primitiveCount = remaining - 2;
                draw->vertexCount = remaining;
                sdfAppendPacket(packet, func_0015FE20(draw));
            }
        }
    }
    surface = D_00354BF0[object->unk50];
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
s32 effGetScatterWideBlock(PcpScatterDraw *object, s32 index) {
    return (s32)object->points + index * object->stride * 0x10;
}

/* Compute the address of an 8-byte-wide block within the stride. */
s32 effGetScatterNarrowBlock(PcpScatterDraw *object, s32 index) {
    return (s32)object->uv + index * object->stride * 8;
}

u32 effGetScatterEntry(PcpScatterDraw *object, s32 index) {
    return object->colors[index];
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effScatterStoreSourceTransformMatrix(PcpScatterDraw *object, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(object->matrix);
}

extern SdfMemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern u32 parAllocateCellSystem(u32, s32, s32, s32);
extern void parDispatchSub(u32, s32, s32, s32);
extern void func_0015D078(u32, u16);
extern u32 effMiscRand(void *);
extern f32 effMiscRandUnitFloat(void *);
extern u8 D_0034DF38[];

/* effNeedleCreateWork */
EffPCPNeedleWork *effNeedleCreateWork(EffPCPNeedleParams *params) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(params->count * sizeof(EffPCPNeedleSlot) + sizeof(EffPCPNeedleWork));
    EffPCPNeedleWork *work = (EffPCPNeedleWork *)sdfResourceRetainAddress(allocation);
    u32 i;
    EffResourceWork *resource;
    u32 count, delayRange;
    EffPCPNeedleSlot *slot;
    f32 radiusBase, radiusJitter;

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
    work->system = parAllocateCellSystem(work->count, work->params.unk54, 0, 0);
    parDispatchSub(work->system, 1, work->params.unk58, work->params.unk58);
    func_0015D078(work->system, (u16)work->params.mode);
    count = work->count;
    delayRange = params->randomDelayRange;
    slot = work->slots;
    if (delayRange == 0) delayRange = 1;
    radiusBase = work->params.radiusBase;
    radiusJitter = work->params.radiusJitter;
    for (i = 0; i < count; i++, slot++) {
        slot->angle = effMiscRandUnitFloat(D_0034DF38) * EFFECT_RING_FULL_TURN;
        slot->radius = radiusBase * (effMiscRandUnitFloat(D_0034DF38) * radiusJitter + (1.0f - radiusJitter));
        slot->age = -(effMiscRand(D_0034DF38) % delayRange);
        effSetResourceEntryValue(work->resource, i, 0x808080);
    }
    return work;
}
