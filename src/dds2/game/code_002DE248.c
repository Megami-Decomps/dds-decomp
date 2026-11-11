#include "sdk_ps2.h"
#include "sdf_model_packet_callback.h"
#include "eff_surface_node.h"
#include "eff_gs_packet.h"
#include "btl_command.h"
#include "sdf_packet_list.h"
#include "eff_bill.h"
#include "common.h"
#include "bill_object_api.h"
#include "sdf_chip.h"
#include "sdf_vu_matrix.h"
#include "eff_event_draw.h"
#include "ps2asm.h"
#include "eff_update_flags.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "dds3_fade_timing.h"
#include "sdf_packet_alloc.h"
#include "sdf_column_packet_api.h"
#include "dds3_effect_sprite.h"

static inline void UnpackRGBAWord(u32 color, u32 unitBits) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unitBits);
}

static inline u32 PackRGBAWord(f32 unit) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

#ifdef NON_MATCHING

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

#endif

extern void evtDestroySelectionState(void);

extern s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);

extern sdfGrOt_t sdfGrOtTable[];

typedef struct EffKindAssetHolder {
    s32 kind;
    s32 references;
    sdfTexHandle_t *resource;
} EffKindAssetHolder;

struct EffKindWork;

extern EffKindAssetHolder *effRetainKindSecondaryAsset(EffKindAssetHolder *work);

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct EffKindDesc {
    u32 (*create)(void *);        // 0x00
    void (*destroy)();       // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(struct EffKindWork *, sdfTexHandle_t *); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} EffKindDesc; // 0x14

/* Native kind work: 0x30-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct EffKindWork {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;         // 0x10
    u32 color;        // 0x14
    f32 scale;        // 0x18
    s32 kind;         // 0x1C
    u32 frame;       // 0x20, incremented by the kind callback dispatcher
    u32 handle;       // 0x24
    void *payload;    // 0x28
    EffKindAssetHolder *target;     // 0x2C
} EffKindWork; // 0x30

/* Source payload's kind selects the initial rendering mode. */
typedef struct EffKindSource {
    u8 pad_00[0x28];
    s32 modeKind;
} EffKindSource;

extern EffKindDesc D_003E9810[];

extern EffKindDesc D_003E98A0[];

struct Matrix4;

struct Matrix4;

extern u32 effModelUpdateControlFlags;

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

typedef struct EffFadeHeaderWork {
    s32 frame;
    s32 payload[2];
} EffFadeHeaderWork;

EffFadeHeaderWork *effDuplicateSmallHeader(const void *source) {
    EffFadeHeaderWork *buffer = sdfChipAllocDirect(sizeof(EffFadeHeaderWork));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    return buffer;
}

EffFadeHeaderWork *effCreateSmallHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return effDuplicateSmallHeader(resource);
}

void effReleaseFadeHeaderAllocation(EffFadeHeaderWork *allocation) {
    dds3StopQuake();
    sdfChipFreeDirect(allocation);
}

EffFadeHeaderWork *effCloneSmallHeaderFromWork(EffFadeHeaderWork *work) {
    return effDuplicateSmallHeader(work->payload);
}

void effFadeFrameReset(EffFadeHeaderWork *counter) {
    counter->frame = 0;
}

void effFadeFrameAdvance(EffFadeHeaderWork *counter) {
    s32 frame;

    frame = counter->frame;
    if (frame == 0) {
        dds3SetQuake(counter->payload[0], counter->payload[1]);
        frame = counter->frame;
    }
    counter->frame = frame + 1;
}

EffFadeVectorWork *effCreateFadeVectorWork(source)
const EffLensFlareParams *source;
{
    EffFadeVectorWork *effect = sdfChipAllocDirect(sizeof(EffFadeVectorWork));
    memset(effect, 0, sizeof(EffFadeVectorWork));
    SDF_VU0_STOREIDENTITYVECTOR(effect->vector);
    memcpy(&effect->source, source, sizeof(effect->source));
    return effect;
}

EffFadeVectorWork *effCreateFadeVectorFromFile(void *work) {
    const EffLensFlareParams *resource;

    resource = fileResolvePrimaryBuffer(work);
    return effCreateFadeVectorWork(resource);
}

void effFreeFadeVectorWork(EffFadeVectorWork *work) {
    sdfChipFreeDirect(work);
}

EffFadeVectorWork *effCloneFadeVectorWork(EffFadeVectorWork *work) {
    return effCreateFadeVectorWork(&work->source);
}

void effResetFadeVectorFrame(EffFadeVectorWork *work) {
    work->frame = 0;
}

#ifdef NON_MATCHING

extern s32 sdfProjectPointInForwardConeVU(void);

extern sdfGrOt_t sdfGrOtTable[];

extern EffLensFlareElement effLensFlareSets[][13];

/* The flare's base colour: the sampled colour times the work colour, each
 * unpacked to 1/128 units. */
static inline void effBlendFlareBaseColor(u32 color, u32 workColor, f32 *baseColor) {
    UnpackRGBAWord(color, 0x3C000000);
    SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
    UnpackRGBAWord(workColor, 0x3C000000);
    SDF_VU0_MULVECTOR_EXTENDED();
    SDF_VU0_STOREVECTOR(baseColor);
}

/* One textured sprite quadrant of a flare element. */
static inline void effSetFlareSprite(dds3EffectSpriteVertex_t *vertex, u32 red, u32 green, u32 blue, u32 alpha, s32 u0,
                                     s32 v0, s32 x0, s32 y0, s32 u1, s32 v1, s32 x1, s32 y1) {
    vertex->r = red;
    vertex->g = green;
    vertex->b = blue;
    vertex->a = alpha;
    vertex->corner[0].u = u0;
    vertex->corner[0].v = v0;
    vertex->corner[0].x = x0;
    vertex->corner[0].y = y0;
    vertex->corner[1].u = u1;
    vertex->corner[1].v = v1;
    vertex->corner[1].x = x1;
    vertex->corner[1].y = y1;
    vertex->corner[0].mask = -1;
    vertex->corner[0].flag = 0;
    vertex->corner[1].mask = -1;
    vertex->corner[1].flag = 0;
}

/* A lens flare's per-frame draw: projects the world-space light vector to the
 * screen (skipping the frame when it is behind the camera or outside the view
 * window), fades by its distance from the screen centre, blends the sampled
 * flare colour with the work colour, and draws each non-transparent element of
 * the selected flare set as four textured sprite quadrants along the line
 * through the centre; then advances the frame. DDS1's routine set only the
 * additive blend (ALPHA_1 0x48); DDS2's setup packet writes TEST_1 first
 * (0x33801: every pixel fails the alpha test into an RGB-only write, so the
 * flare leaves depth and frame alpha untouched), as its neighbours' packets
 * do. Upstream: INCLUDE_ASM in game/code_002DE248. */
void effDrawLensFlare(EffFadeVectorWork *work) {
    f32 projected[4];
    f32 baseColor[4];
    f32 screenX;
    f32 screenY;
    f32 distance;
    f32 maximum;
    f32 fade;
    s32 clipX;
    s32 clipY;
    s32 frame;
    s32 limit;
    u32 color;
    sdfDrawTag_t *list;
    EffGsPairPacket *setup;
    EffLensFlareElement *element;
    sdfTexHandle_t *texture;
    u32 i;

    SDF_VU0_LOADVECTOR(work->vector);
    if (sdfProjectPointInForwardConeVU() == 0) {
        work->frame++;
        return;
    }
    SDF_VU0_STOREVECTOR(projected);
    clipX = (s32)projected[0];
    clipY = (s32)projected[1];
    clipY -= 0x800;
    clipY *= 2;
    if ((u32)(clipX - 0x700) > 0x200 || clipY > 0xE0 || clipY < -0xE0) {
        work->frame++;
        return;
    }
    screenX = projected[0] - 2048.0f;
    screenY = (projected[1] - 2048.0f) * 2.0f;
    limit = work->source.limit;
    frame = work->frame;
    if (limit > 0 && frame >= limit) {
        return;
    }
    distance = fsqrtf(screenX * screenX + screenY * screenY);
    if (distance <= 0.0f) {
        return;
    }
    maximum = fsqrtf(115712.0f);
    fade = (maximum - distance) / maximum;
    fade *= work->source.strength;
    color = effSampleColorAlphaTracks(&work->source.colorCurve, &work->source.alphaCurve, frame, limit);
    effBlendFlareBaseColor(color, work->color, baseColor);

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    setup = (EffGsPairPacket *)sdfGrAllocPacketBuff(0x40);
    setup->dmaTag = 3;
    setup->vifTag = (((u64)0x50000003 << 16) | 0x1000) << 16;
    setup->gifTag = ((u64)0x10000000 << 32) | 0x8002;
    setup->registerList = 0xE;
    setup->firstValue = 0x33801;
    setup->firstAddress = 0x47;
    setup->secondValue = 0x48;
    setup->secondAddress = 0x42;
    sdfDrawTagLink(list, (u32)setup);
    element = effLensFlareSets[work->source.flareSet];
    for (i = 0; i < 13; i++, element++) {
        if (element->color & 0xFF000000) {
            f32 x;
            f32 y;
            f32 scale;
            s32 centerX;
            s32 centerY;
            s32 radiusX;
            s32 radiusY;
            s32 textureWidth;
            s32 textureHeight;
            s32 left;
            s32 top;
            s32 right;
            s32 bottom;
            s32 middleX;
            s32 middleY;
            s32 gsCenterX;
            u32 r;
            u32 g;
            u32 b;
            u32 a;
            SdfDrawPacket *packet;
            dds3EffectSpriteVertex_t *vertex;

            texture = dds3BillboardGetGeneralTex(element->textureIndex);
            x = screenX * element->positionScale;
            y = screenY * element->positionScale;
            scale = element->sizeScale * 2048.0f;
            scale *= fsqrtf(x * x + y * y) / distance * fade;
            centerX = (s32)(x * 16.0f);
            centerY = (s32)(y * 16.0f);
            radiusX = (s32)(scale * 0.5f);
            radiusY = radiusX >> 1;
            UnpackRGBAWord(element->color, 0x3C000000);
            SDF_VU0_LOADVECTOR_B(baseColor);
            SDF_VU0_MULVECTOR_EXTENDED();
            SDF_VU0_dds3ColorFtoI(color, 128.0f);
            radiusX >>= 1;
            radiusY >>= 1;
            textureWidth = texture->width;
            textureHeight = texture->height;
            packet = sdf2DAllocSprite(1);
            vertex = (dds3EffectSpriteVertex_t *)(u32)sdf2DGetPrimPtr((s32)packet);
            radiusX -= 16;
            radiusY -= 8;
            centerY >>= 1;
            left = centerX - radiusX + 0x8000;
            top = centerY - radiusY + 0x8000;
            middleX = left + radiusX;
            middleY = top + radiusY;
            gsCenterX = centerX + 0x8000;
            centerY += 0x8000;
            right = gsCenterX + radiusX;
            bottom = centerY + radiusY;
            r = color & 0xFF;
            g = (color >> 8) & 0xFF;
            b = (color >> 16) & 0xFF;
            a = color >> 24;
            textureWidth -= 2;
            textureWidth <<= 4;
            textureHeight -= 2;
            textureHeight <<= 4;
            effSetFlareSprite(vertex, r, g, b, a, 0, 0, left, top, textureWidth, textureHeight, middleX, middleY);
            sdf2DLinkTexPacket(list, texture, 0);
            sdfDrawTagLink(list, (u32)packet);

            packet = sdf2DAllocSprite(1);
            vertex = (dds3EffectSpriteVertex_t *)(u32)sdf2DGetPrimPtr((s32)packet);
            effSetFlareSprite(vertex, r, g, b, a, textureWidth, 0, gsCenterX, top, 0, textureHeight, right, middleY);
            sdf2DLinkTexPacket(list, texture, 0);
            sdfDrawTagLink(list, (u32)packet);

            packet = sdf2DAllocSprite(1);
            vertex = (dds3EffectSpriteVertex_t *)(u32)sdf2DGetPrimPtr((s32)packet);
            effSetFlareSprite(vertex, r, g, b, a, 0, textureHeight, left, centerY, textureWidth, 0, middleX, bottom);
            sdf2DLinkTexPacket(list, texture, 0);
            sdfDrawTagLink(list, (u32)packet);

            packet = sdf2DAllocSprite(1);
            vertex = (dds3EffectSpriteVertex_t *)(u32)sdf2DGetPrimPtr((s32)packet);
            effSetFlareSprite(vertex, r, g, b, a, textureWidth, textureHeight, gsCenterX, centerY, 0, 0, right, bottom);
            sdf2DLinkTexPacket(list, texture, 0);
            sdfDrawTagLink(list, (u32)packet);
        }
    }
    sdfGrOtTable[83].regFunc(&sdfGrOtTable[83], list);
    work->frame++;
}

#else

INCLUDE_ASM(const s32, "game/code_002DE248", effDrawLensFlare);

#endif

void effCopyFadeWorkVector(f32 *dst, const f32 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effSetFadeVectorColor(EffFadeVectorWork *work, u32 value) {
    work->color = value;
}

void effResetSelectionEntryBuffers(SdfFlagListWork *work) {
    u32 count;
    SdfFlagListMark *entries;
    u32 index;

    index = 0;
    count = work->params.count;
    entries = work->marks;
    if (count != 0) {
        do {
            index = index + 1;
            entries->timer = -1;
            entries++;
        } while (index < count);
    }
    memset(work->colors, 0, count << 3);
}

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *work) {
    return sdfCreateFlagListWork(work);
}

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *file) {
    return sdfInitializeFlagListFromResource(file);
}

void effReleaseSelectionFlagList(SdfFlagListWork *work) {
    sdfReleaseFlagListResource(work);
}

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *p) {
    return effCreateSelectionFlagListFromWork(&p->params);
}

void effResetSelectionEntriesAndState(SdfFlagListWork *p) {
    effResetSelectionEntryBuffers(p);
    p->frame = 0;
}

/* DDS2 suppresses the entry-state update while effect-frame advancement is paused. */
void effUpdateSelectionEntryState(SdfFlagListWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        sdfUpdateFlagListEntries(work);
        return;
    }
}

void effDrawSelectionEntryVectors(SdfFlagListWork *work) {
    sdfDrawFlagListVertices(work);
}

void effUpdateAndDrawSelectionEntries(SdfFlagListWork *p) {
    effUpdateSelectionEntryState(p);
    effDrawSelectionEntryVectors(p);
}

/* Set the selection/drop-list's packed modulation color. */
void effSetSelectionFlagListColor(SdfFlagListWork *work, u32 value) {
    work->unk04 = value;
}

typedef struct EffSelectionHeaderWork {
    s32 frame;
    s32 payload[4];
} EffSelectionHeaderWork;

typedef char EffSelectionHeaderWorkSizeCheck[(sizeof(EffSelectionHeaderWork) == 0x14) ? 1 : -1];

EffSelectionHeaderWork *effDuplicatePayloadHeader(const void *source) {
    EffSelectionHeaderWork *buffer = sdfChipAllocDirect(sizeof(EffSelectionHeaderWork));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    return buffer;
}

EffSelectionHeaderWork *effCreateSelectionHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return effDuplicatePayloadHeader(resource);
}

void effReleaseSelectionHeaderAllocation(EffSelectionHeaderWork *allocation) {
    evtDestroySelectionState();
    sdfChipFreeDirect(allocation);
}

EffSelectionHeaderWork *effCloneSelectionHeaderFromWork(EffSelectionHeaderWork *work) {
    return effDuplicatePayloadHeader(work->payload);
}

void effSelectionFrameReset(EffSelectionHeaderWork *counter) {
    counter->frame = 0;
}

void effSelectionFrameAdvance(EffSelectionHeaderWork *counter) {
    s32 frame;

    frame = counter->frame;
    if (frame == 0) {
        evtSelStateCreate(counter->payload[0], (s16)counter->payload[1], counter->payload[2], counter->payload[3]);
        frame = counter->frame;
    }
    counter->frame = frame + 1;
}

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);

extern void dds3EffectRippleBlurDraw(void *);

extern void dds3EffectFilter(void *);

typedef struct EffFadeConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 padBC[4];
    dds3Effect_Filter_t out; /* 0xC0 */
} EffFadeConfig;

typedef struct EffRateConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    dds3Effect_Blur_t out;       /* 0xC0 */
} EffRateConfig;

/* Draw through the configured duration; zero duration uses frame zero.
 * Blend the packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(packed, 128.0f);
    blended[0] = packed;
    out->abgr = blended[0];
    out->scale = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->blend = work->mode;
    dds3EffectBlur(out);
}

void *effCreateFadeBlendWorkFromOutput(void *work) {
    return dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)((u8 *)work + 0xc0));
}

void effReleaseFadeBlendWork(void *resourceHandle) {
    dds3EffectPartialMultiBlurDelete((effBlurTemplate_t *)resourceHandle);
}

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    effBlurTemplate_t *out = (effBlurTemplate_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->body.Blur.center[0] = 0;
        out->body.Blur.center[1] = 0;
        out->body.area = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->body.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->body.Blur.center[0] = px;
        out->body.Blur.center[1] = py << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(packed, 128.0f);
    blended[0] = packed;
    out->body.Blur.abgr = blended[0];
    out->body.Blur.scale = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->body.Blur.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->body.Blur.blend = work->mode;
    effDrawBlurFixedPointRectangle(out);
}

void effSetFadeMapParameter(EffKindWork *work, sdfTexHandle_t *value) {
    ((effBlurTemplate_t *)work->handle)->texture = value;
}

u32 effCreateFixedSlotBlurWorkFromFadeOutput(void *source) {
    return (u32)dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)((u8 *)source + 0xC0));
}

void effReleaseFixedSlotBlurWork(effBlurScatterWork_t *handle) {
    dds3EffectDistortionBlurDelete(handle);
}

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    effBlurScatterWork_t *out = (effBlurScatterWork_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.area = (s32)rate;
        out->params.center[0] = 0;
        out->params.center[1] = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR(pos);
        out->params.center[0] = (s32)pos[0] - 0x800;
        out->params.center[1] = ((s32)pos[1] - 0x800) << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(packed, 128.0f);
    blended[0] = packed;
    out->params.abgr = blended[0];
    out->params.distortion = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blend = work->mode;
    dds3EffectDistortionBlurDraw(out);
}

void effSetWideFadeMapParameter(EffKindWork *work, sdfTexHandle_t *value) {
    ((effBlurScatterWork_t *)work->handle)->texture = value;
}

u32 effCreateVariableSlotBlurWorkFromFadeOutput(void *source) {
    return (u32)dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)((u8 *)source + 0xc0));
}

void effReleaseVariableSlotBlurWork(effBlurScaleWork_t *work) {
    dds3EffectRippleBlurDelete(work);
}

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and duration gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    effBlurScaleWork_t *out = (effBlurScaleWork_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.area = (s32)rate;
        out->params.center[0] = 0;
        out->params.center[1] = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR(pos);
        out->params.center[0] = (s32)pos[0] - 0x800;
        out->params.center[1] = ((s32)pos[1] - 0x800) << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(packed, 128.0f);
    blended[0] = packed;
    out->params.abgr = blended[0];
    out->params.frequency = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blend = work->mode;
    dds3EffectRippleBlurDraw(out);
}

void effSetFadeBlendParameter(EffKindWork *work, sdfTexHandle_t *value) {
    ((effBlurScaleWork_t *)work->handle)->texture = value;
}

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(packed, 128.0f);
    blended[0] = packed;
    out->abgr = blended[0];
    out->scale = effSampleScalarCurve(&config->blendB, frame, duration) + 1.0f;
    out->rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration);
    out->blend = work->mode;
    effBlurDrawFramebufferQuad(out);
}

extern f32 aViewTarget[4];

extern f32 aViewEye[4];

extern f32 sdfViewMatrix[4][4];

extern f32 sdfProjectionMatrix[4][4];

extern f32 D_004582A0[4];

void func_002DF6C8(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    f32 delta[4];
    f32 ahead[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;
    f32 stretch;
    f32 length;
    f32 zero;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    {
        s32 *src = color1;

        unit = 0x3C000000;
        EE_MMI_RGBA_UNPACK(src, unit);
    }
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(packed, 128.0f);
    blended[0] = packed;
    out->abgr = blended[0];
    stretch = effSampleScalarCurve(&config->blendB, frame, duration);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(100.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(ahead);
    SDF_VU0_LOADMATRIX(sdfViewMatrix);
    MultMatrixRev((const struct Matrix4 *)sdfProjectionMatrix);
    SDF_VU0_LOADVECTOR(ahead);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_004582A0);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_SUBVECTOR();
    do {
        SDF_VU0_STOREVECTOR(delta);
    } while (0);
    zero = 0.0f;
    length = fsqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
    if (delta[0] != zero || delta[1] != zero) {
        out->rotate = atan2f(delta[0], delta[1]) * 57.29577637f;
    }
    out->scale = stretch * length * 10.0f + 1.0f;
    SDF_VU0_COPYVECTOR(D_004582A0, ahead);
    out->blend = work->mode;
    effBlurDrawFramebufferQuad(out);
}

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(EffKindWork *work) {
    EffFadeConfig *config = work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Filter_t *out = &config->out;
    u32 packed;
    u32 unit;
    u32 second;
    u32 baseColor;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    /* Capture the base word before the unit: see docs/idioms.md,
     * "Trace-justified base color capture: effUpdateFadeBlendC". */
    baseColor = work->color;
    unit = 0x3C000000;
    UnpackRGBAWord(baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    UnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = PackRGBAWord(128.0f);
    out->abgr = packed;
    out->blend = work->mode;
    dds3EffectFilter(out);
}

u32 effCreateFadeColorWorkFromOutput(void *work) {
    return (u32)dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)((u8 *)work + 0xc0));
}

void effReleaseFadeColorWork(dds3Effect_PartialMultiFilter_t *work) {
    dds3EffectPartialMultiFilterDelete(work);
}

/* This projected fade also consumes EffKindWork: position, handle and payload. */
extern void effResourceRectDrawGsCoords(dds3Effect_PartialMultiFilter_t *work);

/* Update the fade rectangle's projected extent, center and multiplied tint. */
void effUpdateProjectedFadeColor(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    dds3Effect_PartialMultiFilter_t *out = (dds3Effect_PartialMultiFilter_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->param.center[0] = 0;
        out->param.center[1] = 0;
        out->param.area = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->param.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->param.center[0] = px;
        out->param.center[1] = py << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
    SDF_VU0_MULVECTOR();
    MMI_RGBA_PACK_128_S2(packed);
    blended[0] = packed;
    *(u32 *)out->param.Filter.color = blended[0];
    out->param.Filter.blendControl = work->mode;
    effResourceRectDrawGsCoords(out);
}

/* Replace the rectangle handle's texture without changing the kind-work asset reference. */
void effSetKindRectangleTexture(EffKindWork *work, sdfTexHandle_t *value) {
    ((dds3Effect_PartialMultiFilter_t *)work->handle)->hAlphaTex = value;
}

EffKindWork *effAllocateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x30;
    u32 size = D_003E9810[kind].payloadSize;
    EffKindWork *work = sdfChipAllocZeroDirect(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    SDF_VU0_STOREIDENTITYVECTOR(work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((EffKindSource *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_003E9810[kind].create != NULL) {
        work->handle = D_003E9810[kind].create(source);
    }
    return work;
}

extern EffKindAssetHolder *effCreateResourceHolderFromSelectedKind(s32 *, u16);

/* Build kind work from a file job and retain its optional secondary asset holder. */
EffKindWork *effCreateKindWorkFromFile(FileJobPayload *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateKindWork(work->option, source);
    s32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_003E9810[effect->kind].initialize != NULL) {
            EffKindAssetHolder *child = effCreateResourceHolderFromSelectedKind(secondary, work->primary.selector);
            effect->target = child;
            D_003E9810[effect->kind].initialize(effect, child->resource);
        }
    }
    return effect;
}

extern void effKindAssetReferenceRelease(EffKindAssetHolder *);

void effReleaseKindWork(EffKindWork *work) {
    s32 object = work->handle;
    if (object != 0) {
        D_003E9810[work->kind].destroy(object);
    }
    if (work->target != NULL) {
        effKindAssetReferenceRelease(work->target);
    }
    sdfChipFreeDirect(work);
}

EffKindWork *effCloneKindWork(EffKindWork *source) {
    EffKindWork *copy = effAllocateKindWork((u16)source->kind, source->payload);
    if (source->target != NULL && D_003E9810[copy->kind].initialize != NULL) {
        EffKindAssetHolder *child = effRetainKindSecondaryAsset(source->target);
        sdfTexHandle_t *parameter = child->resource;
        copy->target = child;
        D_003E9810[copy->kind].initialize(copy, parameter);
    }
    return copy;
}

void effKindWorkFrameReset(EffKindWork *work) {
    work->frame = 0;
}

void effKindWorkFrameUpdate(EffKindWork *work) {
    D_003E9810[work->kind].update(work);
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        work->frame++;
    }
}

void effCopyKindWorkPosition(s128 *dst, s128 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effSetKindWorkColor(EffKindWork *work, u32 value) {
    work->color = value;
}

void effSetKindWorkMatrixComponent(EffKindWork *work, float value) {
    work->scale = value;
}

/* Allocate work for the alternate kind table, copying its payload and creating its handle. */
EffKindWork *effAllocateAlternateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x30;
    u32 size = D_003E98A0[kind].payloadSize;
    EffKindWork *work = sdfChipAllocZeroDirect(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    SDF_VU0_STOREIDENTITYVECTOR(work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((EffKindSource *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_003E98A0[kind].create != NULL) {
        work->handle = D_003E98A0[kind].create(source);
    }
    return work;
}

/* Create alternate-kind work from a primary file payload and a supported secondary asset. */
EffKindWork *effCreateAlternateKindWorkFromFile(FileJobPayload *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateAlternateKindWork(work->option, source);
    s32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_003E98A0[effect->kind].initialize != NULL) {
            EffKindAssetHolder *child = effCreateResourceHolderFromSelectedKind(secondary, work->primary.selector);
            effect->target = child;
            D_003E98A0[effect->kind].initialize(effect, child->resource);
        }
    }
    return effect;
}

/* Release the alternate-kind handle, retained secondary asset, and work allocation. */
void effReleaseAlternateKindWork(EffKindWork *work) {
    s32 object = work->handle;
    if (object != 0) {
        D_003E98A0[work->kind].destroy(object);
    }
    if (work->target != NULL) {
        effKindAssetReferenceRelease(work->target);
    }
    sdfChipFreeDirect(work);
}

/* Recreate alternate-kind work, retaining its secondary asset when initialization is supported. */
EffKindWork *effCloneAlternateKindWork(EffKindWork *source) {
    EffKindWork *copy = effAllocateAlternateKindWork((u16)source->kind, source->payload);
    if (source->target != NULL && D_003E98A0[copy->kind].initialize != NULL) {
        EffKindAssetHolder *child = effRetainKindSecondaryAsset(source->target);
        sdfTexHandle_t *parameter = child->resource;
        copy->target = child;
        D_003E98A0[copy->kind].initialize(copy, parameter);
    }
    return copy;
}

void effAlternateKindWorkFrameReset(EffKindWork *work) {
    work->frame = 0;
}

void effAlternateKindWorkFrameUpdate(EffKindWork *work) {
    D_003E98A0[work->kind].update(work);
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        work->frame++;
    }
}

/* Copy the leading vector of alternate kind-table work. */
void effCopyAlternateKindVector(s128 *dst, s128 *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

/* Set the alternate kind-table work's packed color. */
void effSetAlternateKindColor(EffKindWork *work, u32 color) {
    work->color = color;
}

/* Set the alternate kind-table work's scalar scale. */
void effSetAlternateKindScale(EffKindWork *object, f32 scale) {
    object->scale = scale;
}

EffKindAssetHolder *effCreateResourceHolderFromSelectedKind(s32 *source, u16 kind) {
    EffKindAssetHolder *object = sdfChipAllocZeroDirect(sizeof(*object));
    object->kind = kind;
    object->references = 1;
    switch (kind) {
    case 1:
        object->resource = sdfTexSet((SdfTextureFileHeader *)(source));
        break;
    case 4:
        object->resource = dds3BillboardGetGeneralTex(*source);
        break;
    }
    return object;
}

void effKindAssetReferenceRelease(EffKindAssetHolder *object) {
    object->references--;
    if (object->references == 0) {
        if (object->kind != 4) {
            sdfTexFreeDelayed(object->resource);
        }
        sdfChipFreeDirect(object);
    }
}

EffKindAssetHolder *effRetainKindSecondaryAsset(EffKindAssetHolder *work) {
    work->references = work->references + 1;
    return work;
}
