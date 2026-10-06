#include "common.h"
#include "sdf.h"
#include "eff_blur.h"
#include "eff.h"
#include "pcp_vu0.h"


typedef struct {
    u8 pad00[0x124];
    u16 kind; /* 0x124: kind 0x109 selects separate slots for the two resources */
} BlurActor;

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
extern void effSetResourceBlendColor(u32 handle, u32 color);
extern u32 func_001673C0(u32 address);
extern void btlSetActorEffectParameterOrMuzzlePosition(u32 unit, s32 arg1);

void effFreePairedResources(PairedEffectResources *pair) {
    extern void sdfReleaseChipBlock(void *work);

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
                if (((BlurActor *)actor)->kind == 0x109) {
                    btlSetActorEffectParameterOrMuzzlePosition(actor, resourceIndex + 0x14);
                } else {
                    btlSetActorEffectParameterOrMuzzlePosition(actor, 0xB);
                }
                VU0_STORE_VF(vf10, actorPosition);
                record = (BlurVectorRecord *)func_001673C0((u32)work->resource[resourceIndex]);
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
                effSetResourceBlendColor((u32)work->resource[resourceIndex], blendColor);
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
extern s32 billGetWorkTransformMatrix(s32 packet);


struct SdfListHead;
struct SdfDmaNode;
extern void sdfAppendPacket(struct SdfListHead *list, u32 packet);


/* Per-frame draw state; DMA builders select packet ranges within each record. */
typedef struct {
    u8 dmaPacket[0x40];
    u8 pad40[0x1F00];
} BlurFramePacketRecord;

extern SdfPoolNode D_003253E8;
extern BlurFramePacketRecord kwlnFrameDrawPacketRecords[];
extern s32 sdfAllocPacketAligned(s32 size);
extern u32 kwlnGetDrawBufferIndex(void);
extern void func_002D4CC8(const void *, void *, s32);
extern void sdfAppendDmaTagToList(struct SdfListHead *list, u32 packet);

extern s32 func_0011E278();
extern s32 kwlnFadeIsBackgroundOverlayActive(void);
extern void sdfInitPacketList(void *);
extern void sdfAppendDmaPrimary(s32 list, u32 source, struct SdfDmaNode *node);

/* One ST/XYZ2 pair in the packed draw payload. */
typedef struct BlurPacketVertex {
    f32 s, t;
    u8 pad08[8];
    s32 x, y;
    u32 depth;
    u16 xyzControl;
    u8 pad1E[2];
} BlurPacketVertex;

typedef struct BlurPacketQuad {
    u32 color[4];
    BlurPacketVertex vertices[4];
} BlurPacketQuad;

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
        quad->vertices[0].x = (source->left << 4) + 0x7000;
        quad->vertices[0].y = (source->top << 3) + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = (source->bottom << 3) + 0x7900;
        quad->vertices[2].x = (source->right << 4) + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
        quad->vertices[0].s = source->left * (1.0f / 512.0f);
        quad->vertices[0].t = source->top * (1.0f / 512.0f);
        quad->vertices[1].s = quad->vertices[0].s;
        quad->vertices[1].t = source->bottom * (1.0f / 512.0f);
        quad->vertices[2].s = source->right * (1.0f / 512.0f);
        quad->vertices[2].t = quad->vertices[0].t;
        quad->vertices[3].s = quad->vertices[2].s;
        quad->vertices[3].t = quad->vertices[1].t;
    } else {
        quad->vertices[0].x = source->left + 0x7000;
        quad->vertices[0].y = source->top + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = source->bottom + 0x7900;
        quad->vertices[2].x = source->right + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
        quad->vertices[0].s = source->left * (1.0f / 8192.0f);
        quad->vertices[0].t = source->top * (1.0f / 4096.0f);
        quad->vertices[1].s = quad->vertices[0].s;
        quad->vertices[1].t = source->bottom * (1.0f / 4096.0f);
        quad->vertices[2].s = source->right * (1.0f / 8192.0f);
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
        u = source->x + 256.0f;
        v = source->y + 224.0f;
        centerX = u * (1.0f / 512.0f);
        centerY = v * (1.0f / 448.0f);
    } else {
        u = source->x + 4096.0f;
        v = source->y + 1792.0f;
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
        quad->vertices[0].x = (source->left << 4) + 0x7000;
        quad->vertices[0].y = (source->top << 3) + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = (source->bottom << 3) + 0x7900;
        quad->vertices[2].x = (source->right << 4) + 0x7000;
        quad->vertices[2].y = quad->vertices[0].y;
        quad->vertices[3].x = quad->vertices[2].x;
        quad->vertices[3].y = quad->vertices[1].y;
    } else {
        quad->vertices[0].x = source->left + 0x7000;
        quad->vertices[0].y = source->top + 0x7900;
        quad->vertices[1].x = quad->vertices[0].x;
        quad->vertices[1].y = source->bottom + 0x7900;
        quad->vertices[2].x = source->right + 0x7000;
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


void effDrawBlurRectangle(BlurSource *source)
{
    SdfListHead *list;
    void *tag;
    u64 *samplingPacket, *textureAlphaPacket, *blendPacket, *clampPacket;
    void *drawPacket;

    if (func_0011E278() == 0) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        tag = (void *)sdfAllocPacketAligned(0x20);
        sdfAppendDmaPrimary((s32)list, (u32)kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].dmaPacket, tag);
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
        effBuildBlurTransformedQuad(source, (BlurPacketQuad *)billGetWorkTransformMatrix((s32)drawPacket), 0);
        sdfAppendPacket(list, (u32)drawPacket);
        D_003253E8.append((SdfListHead *)&D_003253E8, list);
    }
}

extern void func_002D4C80(s32 source, u32 packet, s32 variant);
extern s32 sdfConsCreateDrawPacket(SdfListHead *list, SdfTex *texture, s32 context);

void effAppendBlurRenderState(void *list, s32 blendControl, u32 resource)
{
    void *framePacket;
    u64 *blendPacket;
    u64 *samplingPacket, *textureAlphaPacket, *clampPacket;
    void *tag;

    framePacket = (void *)sdfAllocPacketAligned(0x40);
    func_002D4C80((s32)&kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()], (u32)framePacket, 1);
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
    sdfConsCreateDrawPacket(list, (SdfTex *)resource, 1);

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
    sdfAppendDmaPrimary((s32)list, (u32)&kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()], tag);

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
    effBuildBlurUnitTextureQuad(source, (BlurPacketQuad *)billGetWorkTransformMatrix((s32)packet), fixedPointCoordinates);
    sdfAppendPacket(list, (u32)packet);
    packet = effCreateSizedDrawPacket(1, 0);
    effBuildBlurTransformedQuad(source, (BlurPacketQuad *)billGetWorkTransformMatrix((s32)packet), fixedPointCoordinates);
    sdfAppendPacket(list, (u32)packet);
}


/* Queue a 0x40-byte textured packet for the current frame buffer onto `list`, then let the filter ops draw it. */
void effDrawBlurListWithFramePacket(void *list) {
    void *packet = (void *)sdfAllocPacketAligned(0x40);

    func_002D4CC8(kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].dmaPacket, packet, 1);
    sdfAppendDmaTagToList(list, (u32)packet);
    D_003253E8.append((SdfListHead *)&D_003253E8, list);
}

extern s32 func_0011E278();
extern void sdfInitPacketList();
extern void effAppendBlurRenderState();

/* Queue blend setup and both rectangle packets, then finish with the filter draw. */
void effDrawBlurSource(BlurSource *source, s32 resource, u8 fixedPointCoordinates) {
    void *list;

    if (func_0011E278(source) == 0) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        effAppendBlurRenderState(list, source->blendControl, resource);
        effAppendBlurRectanglePackets(list, source, fixedPointCoordinates);
        effDrawBlurListWithFramePacket(list);
    }
}

typedef struct EffBlurPixelRect {
    s32 extent;        /* 0x00 */
    u32 color;         /* 0x04: first word of the BlurSource */
    s32 blendControl;  /* 0x08 */
    f32 rotation;      /* 0x0C */
    f32 scale;         /* 0x10 */
    s32 centerX;       /* 0x14 */
    s32 centerY;       /* 0x18 */
    s32 left;          /* 0x1C */
    s32 top;           /* 0x20 */
    s32 right;         /* 0x24 */
    s32 bottom;        /* 0x28 */
} EffBlurPixelRect;

extern void effDrawBlurRectangle(BlurSource *source);

void effDrawBlurPixelRectangle(EffBlurPixelRect *work) {
    s32 x = work->centerX + 0x100;
    s32 y = work->centerY + 0xE0;
    s32 extent = work->extent;

    work->left = x - extent;
    work->top = y - extent;
    work->right = x + extent;
    work->bottom = y + extent;
    effDrawBlurRectangle((BlurSource *)&work->color);
}

typedef struct EffBlurTemplateBody {
    s32 extent;        /* 0x00 */
    BlurSource source; /* 0x04: same source view used for draw packet construction */
} EffBlurTemplateBody;

typedef struct EffBlurTemplate {
    EffBlurTemplateBody body;  /* 0x00: copied from the source template */
    u32 resourceWord;          /* 0x2C */
} EffBlurTemplate;

extern void *sdfAllocSizeClassBlock(s32 size);
extern u32 effGetResourceFirstWord(s32 index);

/* Clone a blur template into a fresh allocation. */
EffBlurTemplate *effCloneBlurTemplate(EffBlurTemplate *src) {
    EffBlurTemplate *dst = sdfAllocSizeClassBlock(sizeof(EffBlurTemplate));

    dst->resourceWord = effGetResourceFirstWord(2);
    dst->body = src->body;
    return dst;
}

void effReleaseBlurTemplate(void) {
    sdfReleaseChipBlock();
}

typedef struct BlurRect {
    s32 extent;     /* 0x00 half-size source */
    BlurSource source; /* 0x04: passed to blur packet construction */
    /* Center and bounds are part of the packet source, not a separate layout. */
    u32 resource;   /* 0x2C */
} BlurRect;

/* Pixel-coordinate variant of the fixed-point rectangle below: the extent is not halved for Y. */
void effDrawBlurPixelRectWithResource(BlurRect *rect) {
    s32 centerX, centerY, halfExtent;

    if (func_0011E278(rect) == 0) {
        centerX = rect->source.x + 0x100;
        centerY = rect->source.y + 0xE0;
        halfExtent = rect->extent;
        rect->source.left = centerX - halfExtent;
        rect->source.top = centerY - halfExtent;
        rect->source.right = centerX + halfExtent;
        rect->source.bottom = centerY + halfExtent;
        effDrawBlurSource(&rect->source, rect->resource, 0);
    }
}

/* GS coordinates use sixteenth-pixel X and half-height Y; extent is halved for Y. */
void effDrawBlurFixedPointRectangle(BlurRect *rect) {
    s32 centerX, centerY, halfExtent;

    if (func_0011E278(rect) == 0) {
        centerX = rect->source.x + 0x1000;
        centerY = (rect->source.y + 0xE00) >> 1;
        halfExtent = rect->extent;
        rect->source.left = centerX - halfExtent;
        rect->source.right = centerX + halfExtent;
        halfExtent >>= 1;
        rect->source.top = centerY - halfExtent;
        rect->source.bottom = centerY + halfExtent;
        effDrawBlurSource(&rect->source, rect->resource, 1);
    }
}
