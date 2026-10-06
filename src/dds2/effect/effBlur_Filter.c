#include "common.h"
#include "eff_blur.h"

extern s32 effGetResourceFirstWord(s32 index);
extern u32 effMiscRand(void *);
extern f32 effMiscRandUnitFloat(void *);
extern u8 D_003AA868[];




/* Random-position variant parameters copied into its work (0x2C). */
typedef struct {
    s32 count;
    s32 delaySpread;
    f32 angleStep;
    u32 color;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    s32 x;
    s32 y;
    s32 positionSpread;
    s32 size;
} EffBlurScatterParams;

typedef struct {
    s32 delay;
    f32 angle;
    EffBlurQuad quad;
} EffBlurScatterSlot; /* 0x30 */

/* The first factory allocates this header followed by 100 scatter slots. */
typedef struct {
    EffBlurScatterParams params;
    u32 sourceHandle;
    u32 allocation;
    EffBlurScatterSlot *slots;
} EffBlurScatterWork; /* 0x38 */

/* Second blur variant: one 0x30-byte slot per step, led by a float phase. */
typedef struct {
    f32 phase;
    f32 angle;
    EffBlurQuad quad;
} EffBlurScaleSlot; /* 0x30 */

typedef struct {
    s32 count;           /* 0x00: number of slots */
    f32 phaseStep;        /* 0x04: advances the size factor toward 1 */
    f32 spacing;         /* 0x08: phase step between slots */
    u32 color;           /* 0x0C */
    s32 unk10;           /* 0x10 */
    f32 unk14;           /* 0x14 */
    f32 unk18;           /* 0x18 */
    f32 angleStep;       /* 0x1C */
    s32 x;               /* 0x20 */
    s32 y;               /* 0x24 */
    s32 size;            /* 0x28 */
} EffBlurScaleParams; /* 0x2C */

/* The second factory allocates this header followed by count scale slots. */
typedef struct {
    EffBlurScaleParams params;
    u32 sourceHandle;
    u32 allocation;
    EffBlurScaleSlot *slots;
} EffBlurScaleWork; /* 0x38 */

void effReleaseBlurTemplate(void) {
    sdfReleaseChipBlock();
}

/* Standalone rectangle input, not either particle-array work (0x30). */
typedef struct {
    s32 extent;
    EffBlurQuad quad;
    u32 sourceHandle;
} EffBlurRect;

extern u32 func_001200E0(void);

/* Update pixel-coordinate edges and draw only when the eligibility check allows. */
void effDrawBlurPixelRectWithResource(EffBlurRect *rect) {
    s32 x, y, w;

    if (func_001200E0() == 0) {
        x = rect->quad.x + 0x100;
        y = rect->quad.y + 0xE0;
        w = rect->extent;
        rect->quad.left = x - w;
        rect->quad.top = y - w;
        rect->quad.right = x + w;
        rect->quad.bottom = y + w;
        effDrawBlurSource(&rect->quad, rect->sourceHandle, 0);
    }
}

/* Fixed-point input keeps the SDK's vertical halving before drawing. */
void effDrawBlurFixedPointRectangle(EffBlurRect *rect) {
    s32 x, y, w;

    if (func_001200E0() == 0) {
        x = rect->quad.x + 0x1000;
        y = (rect->quad.y + 0xE00) >> 1;
        w = rect->extent;
        rect->quad.left = x - w;
        rect->quad.right = x + w;
        w >>= 1;
        rect->quad.top = y - w;
        rect->quad.bottom = y + w;
        effDrawBlurSource(&rect->quad, rect->sourceHandle, 1);
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
    quad->angle = work->params.unk14;
    quad->color = work->params.color;
    quad->blendControl = work->params.unk10;
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

INCLUDE_ASM(const s32, "effect/effBlur_Filter", func_0018EBC8);

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

void func_0018ECD0(EffBlurScatterWork *work)
{
    void *list;
    EffBlurScatterSlot *slot;
    s32 count;
    u32 alpha;

    if (func_001200E0() == 0) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        effAppendBlurRenderState(list, work->params.unk10, work->sourceHandle);
        slot = work->slots;
        if (work->params.count > 0) {
            count = work->params.count;
            do {
                if (slot->delay == 0) {
                    if (slot->angle > 3.14159265f) {
                        effBlurInitializeScatterSlot(work, slot);
                    }
                    slot->quad.displacement = work->params.unk18 * sdfSinPoly(slot->angle) + 1.0f;
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
    quad->blendControl = work->params.unk10;
    quad->angle = work->params.unk14;
    quad->x = work->params.x;
    quad->y = work->params.y;
}






