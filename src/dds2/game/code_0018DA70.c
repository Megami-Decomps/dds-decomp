#include "sdk_ps2.h"
#include "sdf_gs_header.h"
#include "bill_object_api.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "dds3_background_fade_api.h"
#include "ps2asm.h"
#include "dds3_draw_buffer.h"
#include "sdf_trig.h"
#include "dds3_frame_draw_buffer.h"
#include "sdf_chip.h"
#include "sdf_texture_draw_packet.h"
#include "btl_effect_position.h"
#include "sdf.h"
#include "eff_blur.h"
#include "eff.h"
#include "eff_math.h"


extern void effPCP_ThunderPolyListDraw(EffThunderGroup *group);
extern void effPCP_ThunderPolyListDelete(EffThunderGroup *group);

void effPCPBossMETATOBeamDelete(PairedEffectResources *pair) {

    effPCP_ThunderPolyListDelete(pair->resource[1]);
    effPCP_ThunderPolyListDelete(pair->resource[0]);
    sdfChipFreeDirect(pair);
}

extern u32 effBTLSkillGetInvokeFormation(void);

void effUpdatePairedResources(PairedEffectResources *work) {
    s32 total = work->total;
    s32 frame = work->frame;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    u32 actor = effBTLSkillGetInvokeFormation();
    s32 remain;
    f32 t;
    u32 blendColor;
    u32 resourceIndex;
    EffThunderGroupParams *record;
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
            blendColor = effColorBlend(work->colorWithAlpha & 0xFFFFFF, work->colorWithAlpha, t);
            resourceIndex = 0;
            do {
                effBTLModelGetMarkPosition2((struct BtlUnit *)actor, 0xB);
                SDF_VU0_STOREVECTOR(actorPosition);
                record = effPCP_ThunderPolyListGetParam(work->resource[resourceIndex]);
                SDF_VU0_LOADVECTOR(actorPosition);
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_STOREVECTOR(record->points[0]);
                SDF_VU0_LOADVECTOR_B(work->startVec);
                SDF_VU0_INTERPOLATE(0.33f);
                SDF_VU0_STOREVECTOR(record->points[1]);
                SDF_VU0_COPYVECTOR_C_TO_A();
                SDF_VU0_INTERPOLATE(0.66f);
                SDF_VU0_STOREVECTOR(record->points[2]);
                SDF_VU0_STOREVECTOR_B(record->points[3]);
                effPCP_ThunderPolyListSetColor(work->resource[resourceIndex], blendColor);
                effPCP_ThunderPolyListDraw(work->resource[resourceIndex]);
                resourceIndex++;
            } while (resourceIndex < 2);
        }
        work->frame++;
    }
}

/* Copy the endpoint vector; the remaining paired-resource state is unchanged. */
void effCopyPairedResourceEndpoint(PairedEffectResources *dst, PairedEffectResources *src) {
    SDF_VU0_COPYVECTOR(dst->startVec, src->startVec);
}

void effSetPairedResourceColor(PairedEffectResources *pair, u32 colorWithAlpha) {
    pair->colorWithAlpha = colorWithAlpha;
}

typedef dds3Effect_Blur_t BlurSource;



struct sdfDrawTag_t;
struct sdfGrRefPacketHeader_t;

SYMBOL_AT(sdfGrOt_t, D_003803E8, sdfGrOtTable, 0x8A0);
extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];
#include "sdf_packet_alloc.h"
extern void sdfGrSetFrameMaskReset(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext);

extern u32 dds3NullDrawProc(void);


void dds3EffectBlurDrawQuad(const BlurSource *source, effBlurPrimTriStrip_t *quad, u8 fixedPointCoordinates)
{
    f32 centerX, centerY, scale, cosine, sine;
    f32 scaledU, scaledV, u, v;

    /* Scratch coordinates first normalize the center, then rotate each sample. */

    quad->r = ((u8 *)&source->abgr)[0];
    quad->g = ((u8 *)&source->abgr)[1];
    quad->b = ((u8 *)&source->abgr)[2];
    quad->a = ((u8 *)&source->abgr)[3];
    if (fixedPointCoordinates == 0) {
        quad->vtx[0].x = (source->xy[0][0] << 4) + 0x7000;
        quad->vtx[0].y = (source->xy[0][1] << 3) + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = (source->xy[1][1] << 3) + 0x7900;
        quad->vtx[2].x = (source->xy[1][0] << 4) + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
        quad->vtx[0].s = source->xy[0][0] * (1.0f / 512.0f);
        quad->vtx[0].t = source->xy[0][1] * (1.0f / 512.0f);
        quad->vtx[1].s = quad->vtx[0].s;
        quad->vtx[1].t = source->xy[1][1] * (1.0f / 512.0f);
        quad->vtx[2].s = source->xy[1][0] * (1.0f / 512.0f);
        quad->vtx[2].t = quad->vtx[0].t;
        quad->vtx[3].s = quad->vtx[2].s;
        quad->vtx[3].t = quad->vtx[1].t;
    } else {
        quad->vtx[0].x = source->xy[0][0] + 0x7000;
        quad->vtx[0].y = source->xy[0][1] + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = source->xy[1][1] + 0x7900;
        quad->vtx[2].x = source->xy[1][0] + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
        quad->vtx[0].s = source->xy[0][0] * (1.0f / 8192.0f);
        quad->vtx[0].t = source->xy[0][1] * (1.0f / 4096.0f);
        quad->vtx[1].s = quad->vtx[0].s;
        quad->vtx[1].t = source->xy[1][1] * (1.0f / 4096.0f);
        quad->vtx[2].s = source->xy[1][0] * (1.0f / 8192.0f);
        quad->vtx[2].t = quad->vtx[0].t;
        quad->vtx[3].s = quad->vtx[2].s;
        quad->vtx[3].t = quad->vtx[1].t;
    }
    quad->vtx[0].z = 0;
    quad->vtx[0].adc = 0;
    quad->vtx[1].z = 0;
    quad->vtx[1].adc = 0;
    quad->vtx[2].z = 0;
    quad->vtx[2].adc = 0;
    quad->vtx[3].z = 0;
    quad->vtx[3].adc = 0;
    if (fixedPointCoordinates == 0) {
        u = source->center[0] + 256.0f;
        v = source->center[1] + 224.0f;
        centerX = u * (1.0f / 512.0f);
        centerY = v * (1.0f / 448.0f);
    } else {
        u = source->center[0] + 4096.0f;
        v = source->center[1] + 1792.0f;
        centerX = u * (1.0f / 8192.0f);
        centerY = v * (1.0f / 3584.0f);
    }
    scale = source->scale;
    cosine = sdfCos(source->rotate);
    sine = sdfSin(source->rotate);
    scaledU = (quad->vtx[0].s - centerX) * scale;
    scaledV = (quad->vtx[0].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[0].s = u + centerX;
    quad->vtx[0].t = v + centerY;
    scaledU = (quad->vtx[1].s - centerX) * scale;
    scaledV = (quad->vtx[1].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[1].s = u + centerX;
    quad->vtx[1].t = v + centerY;
    scaledU = (quad->vtx[2].s - centerX) * scale;
    scaledV = (quad->vtx[2].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[2].s = u + centerX;
    quad->vtx[2].t = v + centerY;
    scaledU = (quad->vtx[3].s - centerX) * scale;
    scaledV = (quad->vtx[3].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[3].s = u + centerX;
    quad->vtx[3].t = v + centerY;
}


void effBuildBlurUnitTextureQuad(const BlurSource *source, effBlurPrimTriStrip_t *quad, u8 fixedPointCoordinates)
{
    quad->r = 0x80;
    quad->g = 0x80;
    quad->b = 0x80;
    quad->a = ((u8 *)&source->abgr)[3];
    if (fixedPointCoordinates == 0) {
        quad->vtx[0].x = (source->xy[0][0] << 4) + 0x7000;
        quad->vtx[0].y = (source->xy[0][1] << 3) + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = (source->xy[1][1] << 3) + 0x7900;
        quad->vtx[2].x = (source->xy[1][0] << 4) + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
    } else {
        quad->vtx[0].x = source->xy[0][0] + 0x7000;
        quad->vtx[0].y = source->xy[0][1] + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = source->xy[1][1] + 0x7900;
        quad->vtx[2].x = source->xy[1][0] + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
    }
    quad->vtx[0].s = 0.0f;
    quad->vtx[0].t = 0.0f;
    quad->vtx[1].s = 0.0f;
    quad->vtx[1].t = 1.0f;
    quad->vtx[2].s = 1.0f;
    quad->vtx[2].t = 0.0f;
    quad->vtx[3].s = 1.0f;
    quad->vtx[3].t = 1.0f;
    quad->vtx[0].z = 0;
    quad->vtx[0].adc = 0;
    quad->vtx[1].z = 0;
    quad->vtx[1].adc = 0;
    quad->vtx[2].z = 0;
    quad->vtx[2].adc = 0;
    quad->vtx[3].z = 0;
    quad->vtx[3].adc = 0;
}


void dds3EffectBlur(const dds3Effect_Blur_t *source)
{
    sdfDrawTag_t *list;
    void *tag;
    u64 *samplingPacket, *textureAlphaPacket, *blendPacket, *clampPacket;
    SdfDrawPacket *drawPacket;

    if (dds3NullDrawProc() == 0) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        tag = (void *)sdfGrAllocPacketBuff(0x20);
        sdfGrLinkDispFrameTex(list, (u32)dds3FrameDrawPacketRecords[dds3GetFrameID()].baseBytes, tag);
        samplingPacket = (void *)sdfGrAllocPacketBuff(0x30);
        samplingPacket[0] = 2;
        samplingPacket[1] = 0x5000000210000000ULL;
        samplingPacket[2] = 0x1000000000008001ULL;
        samplingPacket[3] = 0xE;
        samplingPacket[4] = 0x61;
        samplingPacket[5] = 0x14;
        sdfDrawTagLink(list, (u32)samplingPacket);
        textureAlphaPacket = (void *)sdfGrAllocPacketBuff(0x40);
        textureAlphaPacket[0] = 3;
        textureAlphaPacket[1] = 0x5000000310000000ULL;
        textureAlphaPacket[2] = 0x1000000000008002ULL;
        textureAlphaPacket[3] = 0xE;
        textureAlphaPacket[4] = 0x8000000080ULL;
        textureAlphaPacket[5] = 0x3B;
        textureAlphaPacket[6] = 0;
        textureAlphaPacket[7] = 0x3F;
        sdfDrawTagLink(list, (u32)textureAlphaPacket);
        blendPacket = (void *)sdfGrAllocPacketBuff(0x40);
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = source->blend;
        blendPacket[7] = 0x42;
        sdfDrawTagLink(list, (u32)blendPacket);
        clampPacket = (void *)sdfGrAllocPacketBuff(0x30);
        clampPacket[0] = 2;
        clampPacket[1] = 0x5000000210000000ULL;
        clampPacket[2] = 0x1000000000008001ULL;
        clampPacket[3] = 0xE;
        if (dds3FadeIsBackgroundOverlayActive() != DDS3_BACKGROUND_OVERLAY_INACTIVE) {
            clampPacket[4] = 0x2DC19000009ULL;
        } else {
            clampPacket[4] = 0x37C00000009ULL;
        }
        clampPacket[5] = 8;
        sdfDrawTagLink(list, (u32)clampPacket);
        drawPacket = effPkt2DAllocTriStrip(1, 0);
        dds3EffectBlurDrawQuad(source, effPkt2DGetPrimPtr(drawPacket), 0);
        sdfDrawTagLink(list, (u32)drawPacket);
        D_003803E8.regFunc(&D_003803E8, list);
    }
}

extern void sdfGrSetFrameMaskRGB(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext);

void dds3EffectBlurDrawFrameMask(struct sdfDrawTag_t *list, s32 blendControl, sdfTexHandle_t *resource)
{
    SdfDmaReferenceChainPacket *framePacket;
    u64 *blendPacket;
    u64 *samplingPacket, *textureAlphaPacket, *clampPacket;
    void *tag;

    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);
    sdfGrSetFrameMaskRGB(dds3FrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 1);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);

    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x48;
    blendPacket[6] = 0x44;
    blendPacket[7] = 0x43;
    sdfDrawTagLink(list, (u32)blendPacket);
    sdf2DLinkTexPacket(list, resource, SDF_GS_CONTEXT_2);

    blendPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = blendControl | 0x10;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, (u32)blendPacket);

    tag = (void *)sdfGrAllocPacketBuff(0x20);
    sdfGrLinkDispFrameTex(list, (u32)dds3FrameDrawPacketRecords[dds3GetFrameID()].baseBytes, tag);

    samplingPacket = (u64 *)sdfGrAllocPacketBuff(0x30);
    samplingPacket[0] = 2;
    samplingPacket[1] = 0x5000000210000000ULL;
    samplingPacket[2] = 0x1000000000008001ULL;
    samplingPacket[3] = 0xE;
    samplingPacket[4] = 0x61;
    samplingPacket[5] = 0x14;
    sdfDrawTagLink(list, (u32)samplingPacket);

    textureAlphaPacket = (u64 *)sdfGrAllocPacketBuff(0x40);
    textureAlphaPacket[0] = 3;
    textureAlphaPacket[1] = 0x5000000310000000ULL;
    textureAlphaPacket[2] = 0x1000000000008002ULL;
    textureAlphaPacket[3] = 0xE;
    textureAlphaPacket[4] = 0x8000000080ULL;
    textureAlphaPacket[5] = 0x3B;
    textureAlphaPacket[6] = 0;
    textureAlphaPacket[7] = 0x3F;
    sdfDrawTagLink(list, (u32)textureAlphaPacket);

    clampPacket = (u64 *)sdfGrAllocPacketBuff(0x30);
    clampPacket[0] = 2;
    clampPacket[1] = 0x5000000210000000ULL;
    clampPacket[2] = 0x1000000000008001ULL;
    clampPacket[3] = 0xE;
    if (dds3FadeIsBackgroundOverlayActive() != DDS3_BACKGROUND_OVERLAY_INACTIVE) {
        clampPacket[4] = 0x2DC19000009ULL;
    } else {
        clampPacket[4] = 0x37C00000009ULL;
    }
    clampPacket[5] = 8;
    sdfDrawTagLink(list, (u32)clampPacket);
}



/* Build the two draw packets for `source` and append them to `list`. */
void dds3EffectBlurDrawAlphaMulti(struct sdfDrawTag_t *list, const BlurSource *source,
                                   u8 fixedPointCoordinates) {
    SdfDrawPacket *packet;

    packet = effPkt2DAllocTriStrip(1, 0x200);
    effBuildBlurUnitTextureQuad(source, effPkt2DGetPrimPtr(packet), fixedPointCoordinates);
    sdfDrawTagLink(list, (u32)packet);
    packet = effPkt2DAllocTriStrip(1, 0);
    dds3EffectBlurDrawQuad(source, effPkt2DGetPrimPtr(packet), fixedPointCoordinates);
    sdfDrawTagLink(list, (u32)packet);
}


/* Queue a 0x40-byte textured packet for the current frame buffer onto `list`, then let the filter ops draw it. */
void dds3EffectBlurDrawFrameMaskReset(struct sdfDrawTag_t *list) {
    SdfDmaReferenceChainPacket *packet = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(0x40);

    sdfGrSetFrameMaskReset(dds3FrameDrawPacketRecords[dds3GetFrameID()].baseBytes, packet, 1);
    sdfDrawTagLinkRefGif(list, (u32)packet);
    D_003803E8.regFunc(&D_003803E8, list);
}

extern u32 dds3NullDrawProc(void);
extern void dds3EffectBlurDrawFrameMask(struct sdfDrawTag_t *list, s32 blendControl, sdfTexHandle_t *resource);

/* Queue blend setup and both rectangle packets, then finish with the filter draw. */
void dds3EffectBlurDraw(const BlurSource *source, sdfTexHandle_t *resource, u8 fixedPointCoordinates) {
    struct sdfDrawTag_t *list;

    if (dds3NullDrawProc() == 0) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        dds3EffectBlurDrawFrameMask(list, source->blend, resource);
        dds3EffectBlurDrawAlphaMulti(list, source, fixedPointCoordinates);
        dds3EffectBlurDrawFrameMaskReset(list);
    }
}

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work) {
    s32 x = work->Blur.center[0] + 0x100;
    s32 y = work->Blur.center[1] + 0xE0;
    s32 extent = work->area;

    work->Blur.xy[0][0] = x - extent;
    work->Blur.xy[0][1] = y - extent;
    work->Blur.xy[1][0] = x + extent;
    work->Blur.xy[1][1] = y + extent;
    dds3EffectBlur(&work->Blur);
}



/* Clone a blur template into a fresh allocation. */
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *src) {
    effBlurTemplate_t *dst = sdfChipAllocDirect(sizeof(effBlurTemplate_t));

    dst->texture = dds3BillboardGetGeneralTex(2);
    memcpy(&dst->body, src, sizeof(*src));
    return dst;
}
