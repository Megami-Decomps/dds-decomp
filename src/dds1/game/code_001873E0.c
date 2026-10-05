#include "common.h"

/* Draw payload shared with effBlur_Filter. Angle/displacement offset texture
   sampling around the rectangle; blendControl is the GS ALPHA_1 word. */
typedef struct {
    union {
        u32 color;
        u8 colorChannels[4];
    };
    s32 blendControl;
    f32 angle;
    f32 displacement;
    s32 x;      /* 0x10 */
    s32 y;      /* 0x14 */
    s32 left;   /* 0x18 */
    s32 top;    /* 0x1C */
    s32 right;  /* 0x20 */
    s32 bottom; /* 0x24 */
} EffBlurQuad; /* 0x28 */

typedef struct EffBlurDrawVertex {
    f32 u;
    f32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    u32 unk18;
    u16 unk1C;
    u16 pad1E;
} EffBlurDrawVertex;

typedef struct EffBlurDrawData {
    u32 color[4];
    EffBlurDrawVertex vertices[4];
} EffBlurDrawData;

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

typedef struct {
    u32 color;
    s32 blendControl;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffSolidRectParams;

/* Scale variant: phase controls its size/lifetime, angle drives displacement. */
typedef struct {
    f32 phase;
    f32 angle;
    EffBlurQuad quad;
} EffBlurScaleSlot; /* 0x30 */

/* Serialized prefix, matching EffBlurScaleParams in effBlur_Filter. */
typedef struct {
    s32 count;
    f32 phaseStep;
    f32 spacing;
    u32 color;
    s32 unk10; /* Copied to quad.blendControl by the slot reset callback. */
    f32 unk14; /* Copied to quad.angle by the slot reset callback. */
    f32 unk18; /* Native update uses this as the displacement amplitude. */
    f32 angleStep;
    s32 x;
    s32 y;
    s32 size;
} EffBlurScaleParams; /* 0x2C */

/* Live owner followed by count slots; its allocation tail is not serialized. */
typedef struct {
    EffBlurScaleParams params;
    u32 sourceHandle;
    void *allocation;
    EffBlurScaleSlot *slots;
} EffBlurScaleWork; /* 0x38 */

extern void *sdfAllocGeneralBlock(s32 size);
extern void *sdfResourceRetainAddress(void *allocation);
extern u32 effGetResourceFirstWord(s32 index);

extern void effBlurResetScaleSlot(EffBlurScaleWork *work, EffBlurScaleSlot *slot);

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void effBlurSecondInitSlots(EffBlurScaleWork *work) {
    EffBlurScaleSlot *slot = work->slots;
    s32 count = work->params.count;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        effBlurResetScaleSlot(work, slot);
        slot->phase = -(work->params.spacing * (f32)i);
    }
}

/* Allocate an owner and slot array from only the serialized parameter prefix. */
EffBlurScaleWork *effCloneBlurWorkWithSlots(EffBlurScaleParams *src) {
    s32 count = src->count;
    void *allocation = sdfAllocGeneralBlock(count * 0x30 + 0x38);
    EffBlurScaleWork *work = (EffBlurScaleWork *)sdfResourceRetainAddress(allocation);
    EffBlurScaleSlot *slot;
    s32 i = 0;

    /* Copy through size, leaving the source handle, allocation and slots owned
       by this new work rather than reading a live-work tail from src. */
    memcpy(work, src, 0x2C);
    work->allocation = allocation;
    work->slots = (EffBlurScaleSlot *)(work + 1);
    work->sourceHandle = effGetResourceFirstWord(3);
    slot = work->slots;
    while (i < count) {
        effBlurResetScaleSlot(work, slot);
        slot->phase = -(work->params.spacing * (f32)i);
        i++;
        slot++;
    }
    return work;
}

/* Release the scale variant's allocation, including its inline slot array. */
void effBlurReleaseSecondResource(EffBlurScaleWork *work) {
    sdfReleaseResourceAllocation(work->allocation);
}

typedef struct BlurFilterOps {
    u8 pad00[0x10];
    void (*draw)(struct BlurFilterOps *self, void *list); /* 0x10 */
} BlurFilterOps;

/* Per-frame packet storage; only its leading 0x40-byte DMA packet is copied here. */
typedef struct {
    u8 dmaPacket[0x40];
    u8 pad40[0x1F00];
} BlurFramePacketRecord;

extern BlurFilterOps D_003253E8;
extern BlurFilterOps D_003253C8;
extern BlurFramePacketRecord kwlnFrameDrawPacketRecords[];
extern void *sdfAllocPacketAligned(s32);
extern u32 kwlnGetDrawBufferIndex(void);

extern s32 func_0011E278();
extern s32 kwlnFadeIsBackgroundOverlayActive(void);
extern void sdfInitPacketList(void *);
extern void sdfAppendDmaPrimary(void *, const void *, void *);
extern void *effCreateSizedDrawPacket();
extern u64 *effBuildDrawPacketWithFlags(u32 flags);
extern void *billGetWorkTransformMatrix();
extern void sdfAppendPacket();

extern void effAppendBlurRenderState(void *, s32, u32);
extern void effAppendBlurRectanglePackets(void *, EffBlurQuad *, u8);
extern void effDrawBlurListWithFramePacket(void *);
extern void effBlurSecondUpdateSlotRect(EffBlurScaleWork *, EffBlurScaleSlot *);

void func_00187598(EffBlurScaleWork *work) {
    void *list;
    EffBlurScaleSlot *slot;
    s32 count;
    u32 sourceColor;
    u32 alpha;

    if (func_0011E278() == 0) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        effAppendBlurRenderState(list, work->params.unk10, work->sourceHandle);
        slot = work->slots;
        if (work->params.count > 0) {
            count = work->params.count;
            do {
                if (slot->phase >= 0.0f) {
                    slot->quad.displacement = work->params.unk18 * sdfSinPoly(slot->angle) *
                                              (1.0f - slot->phase) + 1.0f;
                    sourceColor = work->params.color;
                    alpha = (u32)((f32)(sourceColor >> 24) * (1.0f - slot->phase));
                    slot->quad.color = (sourceColor & 0xFFFFFF) | (alpha << 24);
                    effBlurSecondUpdateSlotRect(work, slot);
                    effAppendBlurRectanglePackets(list, &slot->quad, 1);
                    slot->angle += work->params.angleStep;
                    slot->phase += work->params.phaseStep;
                    if (slot->phase > 1.0f) {
                        effBlurResetScaleSlot(work, slot);
                    }
                } else {
                    slot->phase += work->params.phaseStep;
                }
                count--;
                slot++;
            } while (count != 0);
        }
        effDrawBlurListWithFramePacket(list);
    }
}

void effBlurBuildSamplingQuad(source, data, fixedPoint)
    EffBlurQuad *source;
    EffBlurDrawData *data;
    u8 fixedPoint;
{
    f32 phase = source->angle * 0.017453292519943295f;
    f32 amplitude = source->displacement;
    f32 dx = sdfSinPoly(phase) * amplitude;
    f32 dy = sdfEvaluateCosineViaSinePhaseShift(phase) * amplitude;

    data->color[0] = source->colorChannels[0];
    data->color[1] = source->colorChannels[1];
    data->color[2] = source->colorChannels[2];
    data->color[3] = source->colorChannels[3];
    if (fixedPoint == 0) {
        data->vertices[0].x = (source->left << 4) + 0x7000;
        data->vertices[0].y = (source->top << 3) + 0x7900;
        data->vertices[1].x = data->vertices[0].x;
        data->vertices[1].y = (source->bottom << 3) + 0x7900;
        data->vertices[2].x = (source->right << 4) + 0x7000;
        data->vertices[2].y = data->vertices[0].y;
        data->vertices[3].x = data->vertices[2].x;
        data->vertices[3].y = data->vertices[1].y;
        data->vertices[0].u = (source->left + dx) * 0.001953125f;
        data->vertices[0].v = (source->top + dy) * 0.001953125f;
        data->vertices[1].u = data->vertices[0].u;
        data->vertices[1].v = (source->bottom + dy) * 0.001953125f;
        data->vertices[2].u = (source->right + dx) * 0.001953125f;
        data->vertices[2].v = data->vertices[0].v;
        data->vertices[3].u = data->vertices[2].u;
        data->vertices[3].v = data->vertices[1].v;
    } else {
        data->vertices[0].x = source->left + 0x7000;
        data->vertices[0].y = source->top + 0x7900;
        data->vertices[1].x = data->vertices[0].x;
        data->vertices[1].y = source->bottom + 0x7900;
        data->vertices[2].x = source->right + 0x7000;
        data->vertices[2].y = data->vertices[0].y;
        data->vertices[3].x = data->vertices[2].x;
        data->vertices[3].y = data->vertices[1].y;
        data->vertices[0].u = (source->left + dx) * 0.0001220703125f;
        data->vertices[0].v = (source->top + dy) * 0.000244140625f;
        data->vertices[1].u = data->vertices[0].u;
        data->vertices[1].v = (source->bottom + dy) * 0.000244140625f;
        data->vertices[2].u = (source->right + dx) * 0.0001220703125f;
        data->vertices[2].v = data->vertices[0].v;
        data->vertices[3].u = data->vertices[2].u;
        data->vertices[3].v = data->vertices[1].v;
    }
    data->vertices[0].unk18 = 0;
    data->vertices[0].unk1C = 0;
    data->vertices[1].unk18 = 0;
    data->vertices[1].unk1C = 0;
    data->vertices[2].unk18 = 0;
    data->vertices[2].unk1C = 0;
    data->vertices[3].unk18 = 0;
    data->vertices[3].unk1C = 0;
}

/* Submit a frame-buffer quad with sampling, texture-alpha, blend and clamp packets. */
void effBlurDrawFramebufferQuad(EffBlurQuad *source)
{
    void *list;
    void *tag;
    u64 *samplingPacket, *textureAlphaPacket, *blendPacket, *clampPacket;
    void *drawPacket;

    if (func_0011E278() == 0) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        tag = sdfAllocPacketAligned(0x20);
        sdfAppendDmaPrimary(list, kwlnFrameDrawPacketRecords[kwlnGetDrawBufferIndex()].dmaPacket, tag);
        samplingPacket = sdfAllocPacketAligned(0x30);
        samplingPacket[0] = 2;
        samplingPacket[1] = 0x5000000210000000ULL;
        samplingPacket[2] = 0x1000000000008001ULL;
        samplingPacket[3] = 0xE;
        samplingPacket[4] = 0x61;
        samplingPacket[5] = 0x14;
        sdfAppendPacket(list, samplingPacket);
        textureAlphaPacket = sdfAllocPacketAligned(0x40);
        textureAlphaPacket[0] = 3;
        textureAlphaPacket[1] = 0x5000000310000000ULL;
        textureAlphaPacket[2] = 0x1000000000008002ULL;
        textureAlphaPacket[3] = 0xE;
        textureAlphaPacket[4] = 0x8000000080ULL;
        textureAlphaPacket[5] = 0x3B;
        textureAlphaPacket[6] = 0;
        textureAlphaPacket[7] = 0x3F;
        sdfAppendPacket(list, textureAlphaPacket);
        blendPacket = sdfAllocPacketAligned(0x40);
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = source->blendControl;
        blendPacket[7] = 0x42;
        sdfAppendPacket(list, blendPacket);
        clampPacket = sdfAllocPacketAligned(0x30);
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
        sdfAppendPacket(list, clampPacket);
        drawPacket = effCreateSizedDrawPacket(1, 0);
        effBlurBuildSamplingQuad(source, billGetWorkTransformMatrix(drawPacket), 0);
        sdfAppendPacket(list, drawPacket);
        D_003253E8.draw(&D_003253E8, list);
    }
}

void func_00187C08(EffSolidRectParams *source) {
    void *list;
    u64 *blendPacket;
    u64 *drawPacket;
    u64 color;
    u32 topLeft;
    u32 bottomLeft;
    u32 topRight;
    u32 bottomRight;
    s32 x[4];
    s32 y[4];

    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    blendPacket = sdfAllocPacketAligned(0x40);
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x31001;
    blendPacket[5] = 0x47;
    blendPacket[6] = source->blendControl;
    blendPacket[7] = 0x42;
    sdfAppendPacket(list, blendPacket);

    drawPacket = effBuildDrawPacketWithFlags(0);
    color = source->color | 0x3F80000000000000ULL;
    x[0] = (source->left << 4) + 0x7000;
    x[1] = (source->right << 4) + 0x7000;
    y[0] = (source->top << 3) + 0x7900;
    y[1] = (source->bottom << 3) + 0x7900;
    topLeft = (u16)x[0] | ((u32)(u16)y[0] << 16);
    bottomLeft = (u16)x[0] | ((u32)(u16)y[1] << 16);
    topRight = (u16)x[1] | ((u32)(u16)y[0] << 16);
    bottomRight = (u16)x[1] | ((u32)(u16)y[1] << 16);
    drawPacket[5] = color;
    drawPacket[7] = color;
    drawPacket[9] = color;
    drawPacket[11] = color;
    drawPacket[6] = 0xFF000000000000ULL | topLeft;
    drawPacket[8] = 0xFF000000000000ULL | bottomLeft;
    drawPacket[10] = 0xFF000000000000ULL | topRight;
    drawPacket[12] = 0xFF000000000000ULL | bottomRight;
    sdfAppendPacket(list, drawPacket);
    D_003253C8.draw(&D_003253C8, list);
}
