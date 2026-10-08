#include "common.h"
#include "eff.h"
#include "eff_blur.h"

extern void sdfInitPacketList(SdfListHead *list);
extern void sdfAppendPacket(SdfListHead *list, u32 packetAddress);
extern void *sdfAllocPacketAligned(s32 size);
extern s32 sdfConsCreateDrawPacket(SdfListHead *list, SdfTex *texture, s32 context);
extern void *effCreateSizedDrawPacket(s32 height, s32 flags);
struct EffectDispatchState;
extern s32 billGetWorkTransformMatrix(struct EffectDispatchState *effect);
extern SdfPoolNode D_003803C8;

void effResourceQuadDraw(EffResourceRectDrawParams *params, u32 resource, u8 gsCoordinates) {
    SdfListHead *list;
    u64 *packet;
    void *drawPacket;
    BlurPacketQuad *quad;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    packet = (u64 *)sdfAllocPacketAligned(0x40);
    packet[0] = 3;
    packet[1] = 0x5000000310000000ULL;
    packet[2] = 0x1000000000008002ULL;
    packet[3] = 14;
    packet[4] = 200705;
    packet[5] = 72;
    packet[6] = params->blendControl;
    packet[7] = 67;
    sdfAppendPacket(list, (u32)packet);
    sdfConsCreateDrawPacket(list, (SdfTex *)resource, 1);
    drawPacket = effCreateSizedDrawPacket(1, 0x200);
    quad = (BlurPacketQuad *)billGetWorkTransformMatrix((struct EffectDispatchState *)drawPacket);
    quad->color[0] = params->color[0];
    quad->color[1] = params->color[1];
    quad->color[2] = params->color[2];
    quad->color[3] = params->color[3];
    if (gsCoordinates == 0) {
        quad->vertices[0].x = (params->bounds.left << 4) + 0x7000;
        quad->vertices[0].y = (params->bounds.top << 3) + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = (params->bounds.bottom << 3) + 0x7900;
        quad->vertices[2].x = (params->bounds.right << 4) + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
    } else {
        quad->vertices[0].x = params->bounds.left + 0x7000;
        quad->vertices[0].y = params->bounds.top + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = params->bounds.bottom + 0x7900;
        quad->vertices[2].x = params->bounds.right + 0x7000;
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
    sdfAppendPacket(list, (u32)drawPacket);
    D_003803C8.append((SdfListHead *)&D_003803C8, list);
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 effGetResourceFirstWord(s32 index);

/* Clone rectangle parameters and select a fresh source handle. */
EffResourceRectWork *effCloneResourceTemplate(EffResourceRectParams *src) {
    EffResourceRectWork *dst = sdfAllocSizeClassBlock(sizeof(EffResourceRectWork));

    dst->sourceHandle = effGetResourceFirstWord(0);
    dst->params = *src;
    return dst;
}

void func_0018FC88(void) {
    sdfReleaseChipBlock();
}


/* Generate pixel-coordinate bounds; the renderer applies GS coordinate scale. */
void effResourceRectDrawPixels(EffResourceRectWork *work) {
    s32 extent = (s32)((f32)work->params.extent * 1.4f);
    s32 x = work->params.centerX + 0x100;
    s32 y = work->params.centerY + 0xE0;

    work->params.draw.bounds.left = x - extent;
    work->params.draw.bounds.top = y - extent;
    work->params.draw.bounds.right = x + extent;
    work->params.draw.bounds.bottom = y + extent;
    effResourceQuadDraw(&work->params.draw, work->sourceHandle, 0);
}

/* Generate already-scaled GS coordinates; the renderer must not scale again. */
void effResourceRectDrawGsCoords(EffResourceRectWork *work) {
    f32 scaledExtent = (f32)work->params.extent * 1.4f;
    s32 x = work->params.centerX + 0x1000;
    s32 y = (work->params.centerY + 0xE00) >> 1;
    s32 extent = (s32)scaledExtent;
    s32 right = x + extent;
    s32 bottom;

    x -= extent;
    extent >>= 1;
    work->params.draw.bounds.left = x;
    bottom = y + extent;
    y -= extent;
    work->params.draw.bounds.right = right;
    work->params.draw.bounds.top = y;
    work->params.draw.bounds.bottom = bottom;
    effResourceQuadDraw(&work->params.draw, work->sourceHandle, 1);
}

typedef struct EffTrackPolyData EffTrackPolyData;


extern EffTrackPolyData *effTrackPolyAllocateHistoryData(s32 historyLength, s32 step);
extern void effTrackPolyFillGradientColors(EffTrackPolyData *data, u32 *colors);
extern void effTrackPolySetDrawKind(EffTrackPolyData *data, u32 kind);

/* Clone a model track with independent history and gradient storage. */
EffTrackPolyWork *effTrackPolyCreateWork(EffTrackPolyParams *src) {
    EffTrackPolyWork *dst = sdfAllocSizeClassBlock(sizeof(EffTrackPolyWork));

    dst->params = *src;
    dst->params.unk1C = 0xC;
    dst->updateCount = 0;
    dst->data = effTrackPolyAllocateHistoryData(dst->params.historyLength, 0xC);
    effTrackPolyFillGradientColors(dst->data, src->gradientColors);
    effTrackPolySetDrawKind(dst->data, src->kind);
    return dst;
}
