#include "bill_object_api.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_packet_list.h"
#include "sdf_heap_api.h"
#include "eff_blur.h"

extern EffRandState effDefaultRandomState[];




/* Copy only the serialized parameters, leaving the owned work tail intact. */
void dds3EffectPartialMultiBlurSetParam(effBlurScatterWork_t *dst, dds3Effect_DistortionBlurParam_t *src) {
    dst->params = *src;
}

/* Supply a source handle for the first blur variant. */
void dds3EffectPartialMultiBlurSetTex(effBlurScatterWork_t *work, struct sdfTexHandle_t *sourceHandle) {
    work->texture = sourceHandle;
}

/* Acquire the first variant's source handle from the effect manager. */
void dds3EffectPartialMultiBlurSetTexDef(effBlurScatterWork_t *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

void dds3EffectDistBlurPartInit(effBlurScatterWork_t *work, effBlurScatterSlot_t *slot) {
    dds3Effect_Blur_t *quad = &slot->quad;
    f32 spread;
    s32 halfSize;
    s32 centerX;
    s32 centerY;
    slot->delay = dds3GetRandA((EffRandState *)effDefaultRandomState) % (work->params.interval + 1);
    slot->angle = -3.14159265f;
    quad->rotate = work->params.rotate;
    quad->abgr = work->params.abgr;
    quad->blend = work->params.blend;
    spread = work->params.area;
    halfSize = work->params.width;
    quad->center[0] = work->params.center[0] +
        (s32)(spread * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f));
    halfSize >>= 1;
    quad->center[1] = work->params.center[1] +
        (s32)(spread * ((dds3GetRandFloatA(effDefaultRandomState) - 0.5f) * 2.0f));
    centerX = quad->center[0] + 256;
    quad->xy[0][0] = centerX - halfSize;
    quad->xy[1][0] = centerX + halfSize;
    centerY = quad->center[1] + 224;
    quad->xy[0][1] = centerY - halfSize;
    quad->xy[1][1] = centerY + halfSize;
}

effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *params)
{
    struct sdfMemHandle_t *allocation;
    effBlurScatterWork_t *work;
    effBlurScatterSlot_t *slot;
    s32 slotIndex;

    allocation = sdfMemAllocDirect(sizeof(effBlurScatterWork_t) + 100 * sizeof(effBlurScatterSlot_t));
    work = (effBlurScatterWork_t *)sdfMemLock(allocation);
    work->params = *params;
    work->allocation = allocation;
    work->slots = (effBlurScatterSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(2);
    slot = work->slots;
    for (slotIndex = 0; slotIndex < 100; slotIndex++, slot++) {
        dds3EffectDistBlurPartInit(work, slot);
        slot->angle = 3.14159265f + 1.0f;
    }
    return work;
}

/* Release the first variant's owned effect resource. */
void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *work) {
    sdfMemFreeDirect(work->allocation);
}

extern u32 dds3NullDrawProc(void);
#include "sdf_packet_alloc.h"
struct sdfDrawTag_t;
extern void dds3EffectBlurDrawFrameMask(struct sdfDrawTag_t *list, s32 blendControl, struct sdfTexHandle_t *resource);
extern void dds3EffectBlurDrawFrameMaskReset(struct sdfDrawTag_t *list);
extern f64 fabs(f64 value);

void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work)
{
    struct sdfDrawTag_t *list;
    effBlurScatterSlot_t *slot;
    s32 count;
    u32 alpha;

    if (dds3NullDrawProc() == 0) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        dds3EffectBlurDrawFrameMask(list, work->params.blend, work->texture);
        slot = work->slots;
        if (work->params.number > 0) {
            count = work->params.number;
            do {
                if (slot->delay == 0) {
                    if (slot->angle > 3.14159265f) {
                        dds3EffectDistBlurPartInit(work, slot);
                    }
                    slot->quad.scale = work->params.distortion * sdfSin(slot->angle) + 1.0f;
                    alpha = (u32)((f32)(work->params.abgr >> 24) *
                                  (3.14159265f - fabs(slot->angle)) * (1.0f / 3.14159265f));
                    slot->quad.abgr = (slot->quad.abgr & 0xFFFFFF) | (alpha << 24);
                    dds3EffectBlurDrawAlphaMulti(list, &slot->quad, 0);
                    slot->angle += work->params.speed;
                } else {
                    slot->delay--;
                }
                count--;
                slot++;
            } while (count != 0);
        }
        dds3EffectBlurDrawFrameMaskReset(list);
    }
}


/* Update scale parameters but preserve the allocated destination slot count. */
void dds3EffectDistortionBlurSetParam(effBlurScaleWork_t *dst, dds3Effect_RippleBlurParam_t *src) {
    u32 count = dst->params.number;

    dst->params = *src;
    dst->params.number = count;
}

/* The scale variant has its own source-handle setter. */
void effBlurSetSecondSetting(effBlurScaleWork_t *work, struct sdfTexHandle_t *sourceHandle) {
    work->texture = sourceHandle;
}

/* Both acquisition callbacks request selector 2; the second factory uses 3. */
void dds3EffectDistortionBlurSetTex(effBlurScaleWork_t *work) {
    work->texture = dds3BillboardGetGeneralTex(2);
}

/* Fixed-point edges: 16 units per x pixel and 8 per y pixel.
 * Truncate size before the existing signed half-height shift. */
void effBlurSecondUpdateSlotRect(effBlurScaleWork_t *work, effBlurScaleSlot_t *slot) {
    f32 size = (f32)work->params.area * slot->phase * 16.0f;
    s32 cx = (work->params.center[0] + 0x100) << 4;
    s32 cy = (work->params.center[1] + 0xE0) << 3;
    s32 s = (s32)size;

    slot->quad.xy[0][0] = cx - s;
    slot->quad.xy[1][0] = cx + s;
    s >>= 1;
    slot->quad.xy[0][1] = cy - s;
    slot->quad.xy[1][1] = cy + s;
}

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *work, effBlurScaleSlot_t *slot) {
    dds3Effect_Blur_t *quad = &slot->quad;

    slot->phase = 0.0f;
    slot->angle = 0.0f;
    quad->abgr = work->params.abgr;
    quad->blend = work->params.blend;
    quad->rotate = work->params.rotate;
    quad->center[0] = work->params.center[0];
    quad->center[1] = work->params.center[1];
}




