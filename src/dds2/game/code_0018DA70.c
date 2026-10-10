#include "sdf_gs_header.h"
#include "bill_object_api.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_texture_draw_packet.h"
#include "btl_effect_position.h"
#include "sdf.h"
#include "eff_blur.h"
#include "eff.h"
#include "pcp_vu0.h"


/* The four interpolated VU0 vectors written into each effect resource. */
typedef struct {
    f32 start[4];   /* 0x00: sampled actor position */
    f32 oneThird[4];/* 0x10 */
    f32 twoThirds[4];/* 0x20 */
    f32 end[4];     /* 0x30: work->startVec */
} BlurVectorRecord;

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void effThunderUpdateChainSegments(EffThunderGroup *group);
extern void effThunderGroupRelease(EffThunderGroup *group);
extern void effSetResourceBlendColor(s32 handle, u32 color);
extern u32 func_0016F018(u32 address);

void effFreePairedResources(PairedEffectResources *pair) {

    effThunderGroupRelease(pair->resource[1]);
    effThunderGroupRelease(pair->resource[0]);
    sdfReleaseChipBlock(pair);
}

void effUpdatePairedResources(PairedEffectResources *work) {
    s32 total = work->total;
    s32 frame = work->frame;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    u32 actor = effBTLFieldColorGetOriginalSelector();
    s32 remain;
    f32 t;
    u32 blendColor;
    u32 resourceIndex;
    BlurVectorRecord *record;
    f32 actorPosition[4];
    if (actor != 0) {
        if (frame < total) {
            remain = total - frame;
            if (frame < fadeIn && fadeIn != 0) {
                t = (f32)frame / (f32)fadeIn;
            } else if (remain <= fadeOut && fadeOut != 0) {
                t = (f32)remain / (f32)fadeOut;
            } else {
                t = 1.0f;
            }
            blendColor = effBlendColor(work->colorWithAlpha & 0xFFFFFF, work->colorWithAlpha, t);
            resourceIndex = 0;
            do {
                btlSetActorEffectParameterOrMuzzlePosition((struct BtlUnit *)actor, 0xB);
                VU0_STORE_VF(vf10, actorPosition);
                record = (BlurVectorRecord *)func_0016F018((u32)work->resource[resourceIndex]);
                VU0_LOAD_VF(vf10, actorPosition);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF(vf10, record->start);
                VU0_LOAD_VF(vf11, work->startVec);
                VU0_LERP_VF10(0.33f);
                VU0_STORE_VF(vf10, record->oneThird);
                VU0_MOVE_VF(vf10, vf12);
                VU0_LERP_VF10(0.66f);
                VU0_STORE_VF(vf10, record->twoThirds);
                VU0_STORE_VF(vf11, record->end);
                effSetResourceBlendColor((s32)work->resource[resourceIndex], blendColor);
                effThunderUpdateChainSegments(work->resource[resourceIndex]);
                resourceIndex++;
            } while (resourceIndex < 2);
        }
        work->frame++;
    }
}

/* Copy the endpoint vector; the remaining paired-resource state is unchanged. */
void effCopyPairedResourceEndpoint(PairedEffectResources *dst, PairedEffectResources *src) {
    PCP_COPY_VECTOR(dst->startVec, src->startVec);
}

void effSetPairedResourceColor(PairedEffectResources *pair, u32 colorWithAlpha) {
    pair->colorWithAlpha = colorWithAlpha;
}

typedef EffBlurQuad BlurSource;

extern void *effCreateSizedDrawPacket(s32 height, s32 flags);
extern void *billGetWorkTransformMatrix(void *packet);


struct SdfListHead;
struct SdfDmaNode;


/* Per-frame draw state; DMA builders select packet ranges within each record. */
typedef struct {
    u8 dmaPacket[0x40];
    u8 pad40[0x1F00];
} BlurFramePacketRecord;

extern SdfPoolNode D_003803E8;
extern BlurFramePacketRecord kwlnFrameDrawPacketRecords[];
extern s32 sdfAllocPacketAligned(s32 size);
extern u32 kwlnGetDrawBufferIndex(void);
extern void func_0032DB78(s32, SdfDmaReferenceChainPacket *, s32);

extern u32 func_001200E0(void);
extern s32 kwlnFadeIsBackgroundOverlayActive(void);

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

void effBuildBlurTransformedQuad(BlurSource *source, BlurPacketQuad *quad, u8 fixedPointCoordinates)
{
    f32 centerX, centerY, scale, cosine, sine;
    f32 scaledU, scaledV, u, v;

    /* Scratch coordinates first normalize the center, then rotate each sample. */

    quad->color[0] = ((u8 *)&source->color)[0];
    quad->color[1] = ((u8 *)&source->color)[1];
    quad->color[2] = ((u8 *)&source->color)[2];
    quad->color[3] = ((u8 *)&source->color)[3];
    if (fixedPointCoordinates == 0) {
        quad->vertices[0].x = (source->corners[0][0] << 4) + 0x7000;
        quad->vertices[0].y = (source->corners[0][1] << 3) + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = (source->corners[1][1] << 3) + 0x7900;
        quad->vertices[2].x = (source->corners[1][0] << 4) + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
        quad->vertices[0].s = source->corners[0][0] * (1.0f / 512.0f);
        quad->vertices[0].t = source->corners[0][1] * (1.0f / 512.0f);
        quad->vertices[1].s = quad->vertices[0].s;
        quad->vertices[1].t = source->corners[1][1] * (1.0f / 512.0f);
        quad->vertices[2].s = source->corners[1][0] * (1.0f / 512.0f);
        quad->vertices[2].t = quad->vertices[0].t;
        quad->vertices[3].s = quad->vertices[2].s;
        quad->vertices[3].t = quad->vertices[1].t;
    } else {
        quad->vertices[0].x = source->corners[0][0] + 0x7000;
        quad->vertices[0].y = source->corners[0][1] + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = source->corners[1][1] + 0x7900;
        quad->vertices[2].x = source->corners[1][0] + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
        quad->vertices[0].s = source->corners[0][0] * (1.0f / 8192.0f);
        quad->vertices[0].t = source->corners[0][1] * (1.0f / 4096.0f);
        quad->vertices[1].s = quad->vertices[0].s;
        quad->vertices[1].t = source->corners[1][1] * (1.0f / 4096.0f);
        quad->vertices[2].s = source->corners[1][0] * (1.0f / 8192.0f);
        quad->vertices[2].t = quad->vertices[0].t;
        quad->vertices[3].s = quad->vertices[2].s;
        quad->vertices[3].t = quad->vertices[1].t;
    }
    quad->vertices[0].depth = 0;
    quad->vertices[0].xyzControl = 0;
    quad->vertices[1].depth = 0;
    quad->vertices[1].xyzControl = 0;
    quad->vertices[2].depth = 0;
    quad->vertices[2].xyzControl = 0;
    quad->vertices[3].depth = 0;
    quad->vertices[3].xyzControl = 0;
    if (fixedPointCoordinates == 0) {
        u = source->position[0] + 256.0f;
        v = source->position[1] + 224.0f;
        centerX = u * (1.0f / 512.0f);
        centerY = v * (1.0f / 448.0f);
    } else {
        u = source->position[0] + 4096.0f;
        v = source->position[1] + 1792.0f;
        centerX = u * (1.0f / 8192.0f);
        centerY = v * (1.0f / 3584.0f);
    }
    scale = source->displacement;
    cosine = sdfEvaluateCosineViaSinePhaseShift(source->angle);
    sine = sdfSinPoly(source->angle);
    scaledU = (quad->vertices[0].s - centerX) * scale;
    scaledV = (quad->vertices[0].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vertices[0].s = u + centerX;
    quad->vertices[0].t = v + centerY;
    scaledU = (quad->vertices[1].s - centerX) * scale;
    scaledV = (quad->vertices[1].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vertices[1].s = u + centerX;
    quad->vertices[1].t = v + centerY;
    scaledU = (quad->vertices[2].s - centerX) * scale;
    scaledV = (quad->vertices[2].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vertices[2].s = u + centerX;
    quad->vertices[2].t = v + centerY;
    scaledU = (quad->vertices[3].s - centerX) * scale;
    scaledV = (quad->vertices[3].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vertices[3].s = u + centerX;
    quad->vertices[3].t = v + centerY;
}


void effBuildBlurUnitTextureQuad(BlurSource *source, BlurPacketQuad *quad, u8 fixedPointCoordinates)
{
    quad->color[0] = 0x80;
    quad->color[1] = 0x80;
    quad->color[2] = 0x80;
    quad->color[3] = ((u8 *)&source->color)[3];
    if (fixedPointCoordinates == 0) {
        quad->vertices[0].x = (source->corners[0][0] << 4) + 0x7000;
        quad->vertices[0].y = (source->corners[0][1] << 3) + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = (source->corners[1][1] << 3) + 0x7900;
        quad->vertices[2].x = (source->corners[1][0] << 4) + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
    } else {
        quad->vertices[0].x = source->corners[0][0] + 0x7000;
        quad->vertices[0].y = source->corners[0][1] + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = source->corners[1][1] + 0x7900;
        quad->vertices[2].x = source->corners[1][0] + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
    }
    quad->vertices[0].s = 0.0f;
    quad->vertices[0].t = 0.0f;
    quad->vertices[1].s = 0.0f;
    quad->vertices[1].t = 1.0f;
    quad->vertices[2].s = 1.0f;
    quad->vertices[2].t = 0.0f;
    quad->vertices[3].s = 1.0f;
    quad->vertices[3].t = 1.0f;
    quad->vertices[0].depth = 0;
    quad->vertices[0].xyzControl = 0;
    quad->vertices[1].depth = 0;
    quad->vertices[1].xyzControl = 0;
    quad->vertices[2].depth = 0;
    quad->vertices[2].xyzControl = 0;
    quad->vertices[3].depth = 0;
    quad->vertices[3].xyzControl = 0;
}


void effDrawBlurRectangle(EffBlurQuad *source)
{
    SdfListHead *list;
    void *tag;
    u64 *samplingPacket, *textureAlphaPacket, *blendPacket, *clampPacket;
    void *drawPacket;

    if (func_001200E0() == 0) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        tag = (void *)sdfAllocPacketAligned(0x20);
        sdfAppendDmaPrimary(list, (u32)kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].dmaPacket, tag);
        samplingPacket = (void *)sdfAllocPacketAligned(0x30);
        samplingPacket[0] = 2;
        samplingPacket[1] = 0x5000000210000000ULL;
        samplingPacket[2] = 0x1000000000008001ULL;
        samplingPacket[3] = 0xE;
        samplingPacket[4] = 0x61;
        samplingPacket[5] = 0x14;
        sdfAppendPacket(list, (u32)samplingPacket);
        textureAlphaPacket = (void *)sdfAllocPacketAligned(0x40);
        textureAlphaPacket[0] = 3;
        textureAlphaPacket[1] = 0x5000000310000000ULL;
        textureAlphaPacket[2] = 0x1000000000008002ULL;
        textureAlphaPacket[3] = 0xE;
        textureAlphaPacket[4] = 0x8000000080ULL;
        textureAlphaPacket[5] = 0x3B;
        textureAlphaPacket[6] = 0;
        textureAlphaPacket[7] = 0x3F;
        sdfAppendPacket(list, (u32)textureAlphaPacket);
        blendPacket = (void *)sdfAllocPacketAligned(0x40);
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = source->blendControl;
        blendPacket[7] = 0x42;
        sdfAppendPacket(list, (u32)blendPacket);
        clampPacket = (void *)sdfAllocPacketAligned(0x30);
        clampPacket[0] = 2;
        clampPacket[1] = 0x5000000210000000ULL;
        clampPacket[2] = 0x1000000000008001ULL;
        clampPacket[3] = 0xE;
        if (kwlnFadeIsBackgroundOverlayActive() != 0) {
            clampPacket[4] = 0x2DC19000009ULL;
        } else {
            clampPacket[4] = 0x37C00000009ULL;
        }
        clampPacket[5] = 8;
        sdfAppendPacket(list, (u32)clampPacket);
        drawPacket = effCreateSizedDrawPacket(1, 0);
        effBuildBlurTransformedQuad(source, (BlurPacketQuad *)billGetWorkTransformMatrix(drawPacket), 0);
        sdfAppendPacket(list, (u32)drawPacket);
        D_003803E8.append(&D_003803E8, list);
    }
}

extern void func_0032DB30(s32 source, SdfDmaReferenceChainPacket *packet, s32 variant);

void effAppendBlurRenderState(void *list, s32 blendControl, SdfTex *resource)
{
    SdfDmaReferenceChainPacket *framePacket;
    u64 *blendPacket;
    u64 *samplingPacket, *textureAlphaPacket, *clampPacket;
    void *tag;

    framePacket = (SdfDmaReferenceChainPacket *)sdfAllocPacketAligned(0x40);
    func_0032DB30((s32)&kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()], framePacket, 1);
    sdfAppendDmaTagToList(list, (u32)framePacket);

    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x48;
    blendPacket[6] = 0x44;
    blendPacket[7] = 0x43;
    sdfAppendPacket(list, (u32)blendPacket);
    sdfConsCreateDrawPacket(list, resource, 1);

    blendPacket = (u64 *)sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = blendControl | 0x10;
    blendPacket[7] = 0x42;
    sdfAppendPacket(list, (u32)blendPacket);

    tag = (void *)sdfAllocPacketAligned(0x20);
    sdfAppendDmaPrimary(list, (u32)&kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()], tag);

    samplingPacket = (u64 *)sdfAllocPacketAligned(0x30);
    samplingPacket[0] = 2;
    samplingPacket[1] = 0x5000000210000000ULL;
    samplingPacket[2] = 0x1000000000008001ULL;
    samplingPacket[3] = 0xE;
    samplingPacket[4] = 0x61;
    samplingPacket[5] = 0x14;
    sdfAppendPacket(list, (u32)samplingPacket);

    textureAlphaPacket = (u64 *)sdfAllocPacketAligned(0x40);
    textureAlphaPacket[0] = 3;
    textureAlphaPacket[1] = 0x5000000310000000ULL;
    textureAlphaPacket[2] = 0x1000000000008002ULL;
    textureAlphaPacket[3] = 0xE;
    textureAlphaPacket[4] = 0x8000000080ULL;
    textureAlphaPacket[5] = 0x3B;
    textureAlphaPacket[6] = 0;
    textureAlphaPacket[7] = 0x3F;
    sdfAppendPacket(list, (u32)textureAlphaPacket);

    clampPacket = (u64 *)sdfAllocPacketAligned(0x30);
    clampPacket[0] = 2;
    clampPacket[1] = 0x5000000210000000ULL;
    clampPacket[2] = 0x1000000000008001ULL;
    clampPacket[3] = 0xE;
    if (kwlnFadeIsBackgroundOverlayActive() != 0) {
        clampPacket[4] = 0x2DC19000009ULL;
    } else {
        clampPacket[4] = 0x37C00000009ULL;
    }
    clampPacket[5] = 8;
    sdfAppendPacket(list, (u32)clampPacket);
}



/* Build the two draw packets for `source` and append them to `list`. */
void effAppendBlurRectanglePackets(void *list, BlurSource *source, u8 fixedPointCoordinates) {
    void *packet;

    packet = effCreateSizedDrawPacket(1, 0x200);
    effBuildBlurUnitTextureQuad(source, (BlurPacketQuad *)billGetWorkTransformMatrix(packet), fixedPointCoordinates);
    sdfAppendPacket(list, (u32)packet);
    packet = effCreateSizedDrawPacket(1, 0);
    effBuildBlurTransformedQuad(source, (BlurPacketQuad *)billGetWorkTransformMatrix(packet), fixedPointCoordinates);
    sdfAppendPacket(list, (u32)packet);
}


/* Queue a 0x40-byte textured packet for the current frame buffer onto `list`, then let the filter ops draw it. */
void effDrawBlurListWithFramePacket(void *list) {
    SdfDmaReferenceChainPacket *packet = (SdfDmaReferenceChainPacket *)sdfAllocPacketAligned(0x40);

    func_0032DB78((s32)kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].dmaPacket, packet, 1);
    sdfAppendDmaTagToList(list, (u32)packet);
    D_003803E8.append(&D_003803E8, list);
}

extern u32 func_001200E0(void);
extern void effAppendBlurRenderState();

/* Queue blend setup and both rectangle packets, then finish with the filter draw. */
void effDrawBlurSource(BlurSource *source, SdfTex *resource, u8 fixedPointCoordinates) {
    void *list;

    if (func_001200E0() == 0) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        effAppendBlurRenderState(list, source->blendControl, resource);
        effAppendBlurRectanglePackets(list, source, fixedPointCoordinates);
        effDrawBlurListWithFramePacket(list);
    }
}

void effDrawBlurPixelRectangle(EffBlurTemplateBody *work) {
    s32 x = work->source.position[0] + 0x100;
    s32 y = work->source.position[1] + 0xE0;
    s32 extent = work->extent;

    work->source.corners[0][0] = x - extent;
    work->source.corners[0][1] = y - extent;
    work->source.corners[1][0] = x + extent;
    work->source.corners[1][1] = y + extent;
    effDrawBlurRectangle(&work->source);
}


/* Clone a blur template into a fresh allocation. */
EffBlurTemplate *effCloneBlurTemplate(EffBlurTemplateBody *src) {
    EffBlurTemplate *dst = sdfAllocSizeClassBlock(sizeof(EffBlurTemplate));

    dst->texture = effGetBillResourceTexture(2);
    memcpy(&dst->body, src, sizeof(*src));
    return dst;
}
