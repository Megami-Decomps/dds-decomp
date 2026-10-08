#include "common.h"
#include "sdf_resource.h"
#include "sdf_chip.h"
#include "eff_blur.h"

extern s32 effGetResourceFirstWord(s32);
extern u32 effMiscRand(void *);
extern f32 effMiscRandUnitFloat(void *);
extern u8 D_003AA868[];




void effReleaseBlurTemplate(EffBlurTemplate *owner) {
    sdfReleaseChipBlock(owner);
}

extern u32 func_001200E0(void);

/* Update pixel-coordinate edges and draw only when the eligibility check allows. */
void effDrawBlurPixelRectWithResource(EffBlurTemplate *rect) {
    s32 x, y, w;

    if (func_001200E0() == 0) {
        x = rect->body.source.x + 0x100;
        y = rect->body.source.y + 0xE0;
        w = rect->body.extent;
        rect->body.source.left = x - w;
        rect->body.source.top = y - w;
        rect->body.source.right = x + w;
        rect->body.source.bottom = y + w;
        effDrawBlurSource(&rect->body.source, rect->resourceWord, 0);
    }
}

/* Fixed-point input keeps the SDK's vertical halving before drawing. */
void effDrawBlurFixedPointRectangle(EffBlurTemplate *owner) {
    s32 x, y, w;

    if (func_001200E0() == 0) {
        x = owner->body.source.x + 0x1000;
        y = (owner->body.source.y + 0xE00) >> 1;
        w = owner->body.extent;
        owner->body.source.left = x - w;
        owner->body.source.right = x + w;
        w >>= 1;
        owner->body.source.top = y - w;
        owner->body.source.bottom = y + w;
        effDrawBlurSource(&owner->body.source, owner->resourceWord, 1);
    }
}

/* Copy only the serialized parameters, leaving the owned work tail intact. */
void effBlurCopyParams(EffBlurScatterWork *dst, EffBlurScatterParams *src) {
    dst->params = *src;
}

/* Select the source handle used by the first blur variant. */
void effBlurSetHandle(EffBlurScatterWork *work, u32 sourceHandle) {
    work->sourceHandle = sourceHandle;
}

/* Acquire the same handle through the effect resource manager. */
void effBlurAcquireHandle(EffBlurScatterWork *work) {
    work->sourceHandle = effGetResourceFirstWord(2);
}

void effBlurInitializeScatterSlot(EffBlurScatterWork *work, EffBlurScatterSlot *slot) {
    EffBlurQuad *quad = &slot->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    slot->delay = effMiscRand(D_003AA868) % (work->params.delaySpread + 1);
    slot->angle = -3.14159265f;
    quad->angle = work->params.uvDisplacementAngleDegrees;
    quad->color = work->params.color;
    quad->blendControl = work->params.blendControl;
    spread = work->params.positionSpread;
    halfSize = work->params.size;
    quad->x = work->params.x +
        (s32)(spread * ((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->y = work->params.y +
        (s32)(spread * ((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f));
    centerX = quad->x + 256;
    quad->left = centerX - halfSize;
    quad->right = centerX + halfSize;
    centerY = quad->y + 224;
    quad->top = centerY - halfSize;
    quad->bottom = centerY + halfSize;
}

EffBlurScatterWork *effBlurCreateScatterWork(EffBlurScatterParams *params)
{
    struct SdfMemBlock *allocation;
    EffBlurScatterWork *work;
    EffBlurScatterSlot *slot;
    s32 i;

    allocation = sdfAllocGeneralBlock(sizeof(EffBlurScatterWork) + 100 * sizeof(EffBlurScatterSlot));
    work = (EffBlurScatterWork *)sdfResourceRetainAddress(allocation);
    work->params = *params;
    work->allocation = allocation;
    work->slots = (EffBlurScatterSlot *)(work + 1);
    work->sourceHandle = effGetResourceFirstWord(2);
    slot = work->slots;
    for (i = 0; i < 100; i++, slot++) {
        effBlurInitializeScatterSlot(work, slot);
        slot->angle = 3.14159265f + 1.0f;
    }
    return work;
}

/* Release the first variant's owned effect resource. */
void effBlurReleaseFirstResource(EffBlurScatterWork *work) {
    sdfReleaseResourceAllocation(work->allocation);
}


extern s32 sdfAllocPacketAligned(s32 size);
struct SdfListHead;
extern void sdfInitPacketList(struct SdfListHead *list);
extern void effAppendBlurRenderState(void *list, s32 blendControl, u32 resource);
extern void effAppendBlurRectanglePackets(void *list, EffBlurQuad *quad, u8 fixedPoint);
extern void effDrawBlurListWithFramePacket(void *list);
extern f32 sdfSinPoly(f32 angle);
extern f64 fabs(f64 value);

void effBlurStepScatterSlotsAndDraw(EffBlurScatterWork *work)
{
    void *list;
    EffBlurScatterSlot *slot;
    s32 count;
    u32 alpha;

    if (func_001200E0() == 0) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        effAppendBlurRenderState(list, work->params.blendControl, work->sourceHandle);
        slot = work->slots;
        if (work->params.count > 0) {
            count = work->params.count;
            do {
                if (slot->delay == 0) {
                    if (slot->angle > 3.14159265f) {
                        effBlurInitializeScatterSlot(work, slot);
                    }
                    slot->quad.displacement = work->params.uvDisplacementAmplitude * sdfSinPoly(slot->angle) + 1.0f;
                    alpha = (u32)((f32)(work->params.color >> 24) *
                                  (3.14159265f - fabs(slot->angle)) * (1.0f / 3.14159265f));
                    slot->quad.color = (slot->quad.color & 0xFFFFFF) | (alpha << 24);
                    effAppendBlurRectanglePackets(list, &slot->quad, 0);
                    slot->angle += work->params.angleStep;
                } else {
                    slot->delay--;
                }
                count--;
                slot++;
            } while (count != 0);
        }
        effDrawBlurListWithFramePacket(list);
    }
}


/* Update scale parameters but preserve the allocated destination slot count. */
void effBlurCopyParamsKeepHeader(EffBlurScaleWork *dst, EffBlurScaleParams *src) {
    u32 count = dst->params.count;

    dst->params = *src;
    dst->params.count = count;
}

/* The scale variant has its own source-handle setter. */
void effBlurSetSecondSetting(EffBlurScaleWork *work, u32 sourceHandle) {
    work->sourceHandle = sourceHandle;
}

/* Both acquisition callbacks request selector 2; the second factory uses 3. */
void effBlurAcquireSecondHandle(EffBlurScaleWork *work) {
    work->sourceHandle = effGetResourceFirstWord(2);
}

/* Fixed-point edges: 16 units per x pixel and 8 per y pixel.
 * Truncate size before the existing signed half-height shift. */
void effBlurSecondUpdateSlotRect(EffBlurScaleWork *work, EffBlurScaleSlot *slot) {
    f32 size = (f32)work->params.size * slot->phase * 16.0f;
    s32 cx = (work->params.x + 0x100) << 4;
    s32 cy = (work->params.y + 0xE0) << 3;
    s32 s = (s32)size;

    slot->quad.left = cx - s;
    slot->quad.right = cx + s;
    s >>= 1;
    slot->quad.top = cy - s;
    slot->quad.bottom = cy + s;
}

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void effBlurResetScaleSlot(EffBlurScaleWork *work, EffBlurScaleSlot *slot) {
    EffBlurQuad *quad = &slot->quad;

    slot->phase = 0.0f;
    slot->angle = 0.0f;
    quad->color = work->params.color;
    quad->blendControl = work->params.blendControl;
    quad->angle = work->params.uvDisplacementAngleDegrees;
    quad->x = work->params.x;
    quad->y = work->params.y;
}






