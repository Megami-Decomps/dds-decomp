#include "bill_object_api.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "eff_blur.h"

extern EffRandState effDefaultRandomState[];




/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(EffBlurScatterWork *dst, EffBlurScatterParams *src) {
    dst->params = *src;
}

/* Supply a source handle for the first blur variant. */
void dds3EffectPartialMultiBlurSetTex(EffBlurScatterWork *work, struct SdfTex *sourceHandle) {
    work->texture = sourceHandle;
}

/* Acquire the first variant's source handle from the effect manager. */
void dds3EffectPartialMultiBlurSetTexDef(EffBlurScatterWork *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

void dds3EffectDistBlurPartInit(EffBlurScatterWork *work, EffBlurScatterSlot *slot) {
    EffBlurQuad *quad = &slot->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    slot->delay = dds3GetRandA((EffRandState *)effDefaultRandomState) % (work->params.delaySpread + 1);
    slot->angle = -3.14159265f;
    quad->angle = work->params.uvDisplacementAngleDegrees;
    quad->color = work->params.color;
    quad->blendControl = work->params.blendControl;
    spread = work->params.positionSpread;
    halfSize = work->params.size;
    quad->position[0] = work->params.position[0] +
        (s32)(spread * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->position[1] = work->params.position[1] +
        (s32)(spread * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f));
    centerX = quad->position[0] + 256;
    quad->corners[0][0] = centerX - halfSize;
    quad->corners[1][0] = centerX + halfSize;
    centerY = quad->position[1] + 224;
    quad->corners[0][1] = centerY - halfSize;
    quad->corners[1][1] = centerY + halfSize;
}

EffBlurScatterWork *dds3EffectDistortionBlurCreate(EffBlurScatterParams *params)
{
    struct SdfMemBlock *allocation;
    EffBlurScatterWork *work;
    EffBlurScatterSlot *slot;
    s32 slotIndex;

    allocation = sdfMemAllocDirect(sizeof(EffBlurScatterWork) + 100 * sizeof(EffBlurScatterSlot));
    work = (EffBlurScatterWork *)sdfMemLock(allocation);
    work->params = *params;
    work->allocation = allocation;
    work->slots = (EffBlurScatterSlot *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(2);
    slot = work->slots;
    for (slotIndex = 0; slotIndex < 100; slotIndex++, slot++) {
        dds3EffectDistBlurPartInit(work, slot);
        slot->angle = 3.14159265f + 1.0f;
    }
    return work;
}

/* Release the first variant's owned effect resource. */
void dds3EffectDistortionBlurDelete(EffBlurScatterWork *work) {
    sdfMemFreeDirect(work->allocation);
}

extern u32 dds3NullDrawProc(void);
#include "sdf_packet_alloc.h"
struct SdfListHead;
extern void effAppendBlurRenderState(void *list, s32 blendControl, struct SdfTex *resource);
extern void effDrawBlurListWithFramePacket(void *list);
extern f64 fabs(f64 value);

void effBlurStepScatterSlotsAndDraw(EffBlurScatterWork *work)
{
    void *list;
    EffBlurScatterSlot *slot;
    s32 count;
    u32 alpha;

    if (dds3NullDrawProc() == 0) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        effAppendBlurRenderState(list, work->params.blendControl, work->texture);
        slot = work->slots;
        if (work->params.count > 0) {
            count = work->params.count;
            do {
                if (slot->delay == 0) {
                    if (slot->angle > 3.14159265f) {
                        dds3EffectDistBlurPartInit(work, slot);
                    }
                    slot->quad.displacement = work->params.uvDisplacementAmplitude * sdfSin(slot->angle) + 1.0f;
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
void dds3EffectDistortionBlurSetParam(EffBlurScaleWork *dst, EffBlurScaleParams *src) {
    u32 count = dst->params.count;

    dst->params = *src;
    dst->params.count = count;
}

/* The scale variant has its own source-handle setter. */
void effBlurSetSecondSetting(EffBlurScaleWork *work, struct SdfTex *sourceHandle) {
    work->texture = sourceHandle;
}

/* Both acquisition callbacks request selector 2; the second factory uses 3. */
void dds3EffectDistortionBlurSetTex(EffBlurScaleWork *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

/* Fixed-point edges: 16 units per x pixel and 8 per y pixel.
 * Truncate size before the existing signed half-height shift. */
void effBlurSecondUpdateSlotRect(EffBlurScaleWork *work, EffBlurScaleSlot *slot) {
    f32 size = (f32)work->params.size * slot->phase * 16.0f;
    s32 cx = (work->params.position[0] + 0x100) << 4;
    s32 cy = (work->params.position[1] + 0xE0) << 3;
    s32 s = (s32)size;

    slot->quad.corners[0][0] = cx - s;
    slot->quad.corners[1][0] = cx + s;
    s >>= 1;
    slot->quad.corners[0][1] = cy - s;
    slot->quad.corners[1][1] = cy + s;
}

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(EffBlurScaleWork *work, EffBlurScaleSlot *slot) {
    EffBlurQuad *quad = &slot->quad;

    slot->phase = 0.0f;
    slot->angle = 0.0f;
    quad->color = work->params.color;
    quad->blendControl = work->params.blendControl;
    quad->angle = work->params.uvDisplacementAngleDegrees;
    quad->position[0] = work->params.position[0];
    quad->position[1] = work->params.position[1];
}






