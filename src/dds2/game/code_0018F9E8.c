#include "sdk_ps2.h"
#include "bill_object_api.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_texture_draw_packet.h"
#include "eff.h"
#include "eff_blur.h"
#include "sdf_chip.h"

#include "sdf_packet_alloc.h"
SYMBOL_AT(sdfGrOt_t, D_003803C8, sdfGrOtTable, 0x880);

void dds3EffectFilterDrawTexQuad(effResourceRectDrawParams_t *params, sdfTexHandle_t *resource, u8 gsCoordinates) {
    sdfDrawTag_t *list;
    u64 *packet;
    SdfDrawPacket *drawPacket;
    effBlurPrimTriStrip_t *quad;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    packet = (u64 *)sdfGrAllocPacketBuff(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000ULL;
    packet[2] = 0x1000000000008002ULL;
    packet[3] = 14;
    packet[4] = 200705;
    packet[5] = 72;
    packet[6] = params->blendControl;
    packet[7] = 67;
    sdfDrawTagLink(list, (u32)packet);
    sdf2DLinkTexPacket(list, resource, SDF_GS_CONTEXT_2);
    drawPacket = effPkt2DAllocTriStrip(1, 0x200);
    quad = effPkt2DGetPrimPtr(drawPacket);
    quad->r = params->color[0];
    quad->g = params->color[1];
    quad->b = params->color[2];
    quad->a = params->color[3];
    if (gsCoordinates == 0) {
        quad->vtx[0].x = (params->bounds.left << 4) + 0x7000;
        quad->vtx[0].y = (params->bounds.top << 3) + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = (params->bounds.bottom << 3) + 0x7900;
        quad->vtx[2].x = (params->bounds.right << 4) + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
    } else {
        quad->vtx[0].x = params->bounds.left + 0x7000;
        quad->vtx[0].y = params->bounds.top + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = params->bounds.bottom + 0x7900;
        quad->vtx[2].x = params->bounds.right + 0x7000;
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
    sdfDrawTagLink(list, (u32)drawPacket);
    D_003803C8.regFunc(&D_003803C8, list);
}



/* Clone rectangle parameters and select a fresh source handle. */
dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *src) {
    dds3Effect_PartialMultiFilter_t *dst = sdfChipAllocDirect(sizeof(dds3Effect_PartialMultiFilter_t));

    dst->hAlphaTex = dds3BillboardGetGeneralTex(0);
    memcpy(&dst->param, src, sizeof(*src));
    return dst;
}

void dds3EffectPartialMultiFilterDelete(dds3Effect_PartialMultiFilter_t *work) {
    sdfChipFreeDirect(work);
}


/* Generate pixel-coordinate bounds; the renderer applies GS coordinate scale. */
void effResourceRectDrawPixels(dds3Effect_PartialMultiFilter_t *work) {
    s32 extent = (s32)((f32)work->param.area * 1.4f);
    s32 x = work->param.center[0] + 0x100;
    s32 y = work->param.center[1] + 0xE0;

    work->param.Filter.bounds.left = x - extent;
    work->param.Filter.bounds.top = y - extent;
    work->param.Filter.bounds.right = x + extent;
    work->param.Filter.bounds.bottom = y + extent;
    dds3EffectFilterDrawTexQuad(&work->param.Filter, work->hAlphaTex, 0);
}

/* Generate already-scaled GS coordinates; the renderer must not scale again. */
void effResourceRectDrawGsCoords(dds3Effect_PartialMultiFilter_t *work) {
    f32 scaledExtent = (f32)work->param.area * 1.4f;
    s32 x = work->param.center[0] + 0x1000;
    s32 y = (work->param.center[1] + 0xE00) >> 1;
    s32 extent = (s32)scaledExtent;
    s32 right = x + extent;
    s32 bottom;

    x -= extent;
    extent >>= 1;
    work->param.Filter.bounds.left = x;
    bottom = y + extent;
    y -= extent;
    work->param.Filter.bounds.right = right;
    work->param.Filter.bounds.top = y;
    work->param.Filter.bounds.bottom = bottom;
    dds3EffectFilterDrawTexQuad(&work->param.Filter, work->hAlphaTex, 1);
}

typedef struct dds3HermitianTrackPoly_tag dds3HermitianTrackPoly_tag;


extern dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 historyLength, s32 steps);
extern void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *data, const u32 gradientColors[4]);
extern void dds3HermitianTrackPolySetDrawTarget(dds3HermitianTrackPoly_tag *data, u32 drawTarget);

/* Clone a model track with independent history and gradient storage. */
EffTrackPolyWork *effTrackPolyCreateWork(EffTrackPolyParams *src) {
    EffTrackPolyWork *dst = sdfChipAllocDirect(sizeof(EffTrackPolyWork));

    dst->params = *src;
    dst->params.unk1C = 0xC;
    dst->updateCount = 0;
    dst->data = dds3HermitianTrackPolyCreate(dst->params.historyLength, 0xC);
    dds3HermitianTrackPolySetVC(dst->data, src->gradientColors);
    dds3HermitianTrackPolySetDrawTarget(dst->data, src->drawKind);
    return dst;
}
