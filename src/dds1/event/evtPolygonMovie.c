#include "common.h"
#include "eff.h"
#include "eff_event_draw.h"
#include "sdf_chip.h"
#include "file_request_api.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "fld.h"
#include "evt_polygon_movie.h"
#include "evt_unit.h"
#include "mdl.h"
#include "dds3obj.h"

/* Global event state behind kwlnTaskGetUserValue; the polygon-movie word at 0x0 and
 * the pointer to the shared flag word at 0x8. */
typedef struct EvtGlobal {
    u32 movieFlags; /* 0x0 */
    u32 unk_4;      /* 0x4 */
    u32 *flags;     /* 0x8 */
} EvtGlobal;

/* Polygon-movie clip: time is stored as a float, but compared as whole frames. */
typedef struct PolyMovieClip {
    u8 pad[8];       /* 0x0 */
    f32 duration;    /* 0x8 */
    f32 position;    /* 0xc */
} PolyMovieClip;

typedef struct PolyMovieState {
    u8 pad[4];
    PolyMovieClip *clip;
} PolyMovieState;

typedef struct PolyMovieObject {
    u8 pad[0x18];
    PolyMovieState *state;
} PolyMovieObject;

/* Polygon-movie event parameter blocks blended by the functions below.
 * Each field is named for how the blend functions use it:
 *   lerpN  -> evtMovieInterpolateFloatIfEnabled, the float lerp
 *   valueN -> evtMovieInterpolateUintIfEnabled, the u32 lerp
 *   v, m   -> evtMovieInterpolateIntIfEnabled, the s32 lerp over a 2-vector and a 2x2 matrix
 *   blendControl is copied from the source record and never blended. */







u32 evtPolygonMovieBlendColor(s32 enable, f32 t, u32 a, u32 b);

/* "PMD2" resource: a 0x20-byte header followed by 16-byte entries whose
 * offsets are relative to the start of the block. */

extern EvtUnit *effObjGetTransitionWork(EffWorldNode *object);
extern void dds3SetObjectFlags(PolyMovieObject *obj, s32 flags);
extern void dds3ClearObjectFlags(PolyMovieObject *obj, s32 flags);
extern void evtScaleValueByMultiplier(PolyMovieClip *clip, f32 multiplier);
extern void sdfFreezeFloatCounter(PolyMovieClip *clip);
extern void sdfUnfreezeFloatCounter(PolyMovieClip *clip);
extern void *memset(void *dst, s32 value, u32 size);
extern void *memcpy(void *dst, const void *src, u32 size);

extern f32 D_00368540[4];
extern f32 D_00368550[4];
extern f32 D_00368560[4];
extern EffBlurQuad D_00368590;
extern EffBlurTemplateBody D_003685C0;
extern EffSolidRectParams D_003685F0;
extern EffResourceRectParams D_00368610;
extern EffBlurScatterParams D_00368640;
extern EffBlurScaleParams D_00368670;
extern EffBlurQuad D_003686A0;
extern s32 itfMesCreateWindow(u8 *arg);
extern void itfMesDestroyWindowIfPresent(s32 handle);
extern void fileWaitIdle(void);
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void func_003003F0(const char *fmt, ...);

s32 evtPolygonMovieTestFlag(KwlnTask *task)
{
    EvtGlobal *state;
    s32 set;

    state = (EvtGlobal *)kwlnTaskGetUserValue(task);
    set = 0;
    if (state->movieFlags & 8) {
        set = 1;
    }
    return set;
}

f32 evtMovieInterpolateFloatIfEnabled(s32 enable, f32 t, f32 a, f32 b)
{
    if (enable) {
        return a * (1.0f - t) + b * t;
    }
    return a;
}

s32 evtMovieInterpolateIntIfEnabled(s32 enable, f32 t, s32 a, s32 b)
{
    s32 result = a;

    if (enable) {
        result = (s32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return result;
}

u32 evtMovieInterpolateUintIfEnabled(s32 enable, f32 t, u32 a, u32 b)
{
    if (enable) {
        return (u32)((f32)a * (1.0f - t) + (f32)b * t);
    }
    return a;
}

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 evtPolygonMovieBlendColor(s32 enable, f32 t, u32 a, u32 b)
{
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;

    if (enable) {
        color1[0] = a;
        EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
        VU0_SCALE_VF(vf10, 1.0f - t);
        VU0_MOVE_VF(vf11, vf10);
        color2[0] = b;
        EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
        VU0_SCALE_VF(vf10, t);
        VU0_ADD(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
        blended[0] = packed;
        return blended[0];
    }
    return a;
}

/* vu0 routine: blend three rows of vectors a and b by t into out (missing input = default rows) */
void evtBlendVectorRows(s32 enable, f32 t, f32 *a, f32 *b, f32 *out)
{
    if (enable == 0) {
        t = 0.0f;
    }
    if (a != NULL) {
        VU0_LOAD_VF(vf10, a);
    } else {
        VU0_LOAD_VF(vf10, D_00368540);
    }
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    if (b != NULL) {
        VU0_LOAD_VF(vf11, b);
    } else {
        VU0_LOAD_VF(vf11, D_00368540);
    }
    VU0_SCALAR_OP(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out);
    if (a != NULL) {
        VU0_LOAD_VF(vf10, a + 4);
    } else {
        VU0_LOAD_VF(vf10, D_00368550);
    }
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    if (b != NULL) {
        VU0_LOAD_VF(vf11, b + 4);
    } else {
        VU0_LOAD_VF(vf11, D_00368550);
    }
    VU0_SCALAR_OP(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out + 4);
    if (a != NULL) {
        VU0_LOAD_VF(vf10, a + 8);
    } else {
        VU0_LOAD_VF(vf10, D_00368560);
    }
    VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    if (b != NULL) {
        VU0_LOAD_VF(vf11, b + 8);
    } else {
        VU0_LOAD_VF(vf11, D_00368560);
    }
    VU0_SCALAR_OP(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, out + 8);
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233308);

void evtBlendParamsA(s32 enable, f32 t, EffBlurQuad *a, EffBlurQuad *b, EffBlurQuad *out)
{
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_00368590;
    }
    if (b == NULL) {
        b = &D_00368590;
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->angle = evtMovieInterpolateFloatIfEnabled(enable, t, a->angle, b->angle);
    out->displacement = evtMovieInterpolateFloatIfEnabled(enable, t, a->displacement, b->displacement);
    for (i = 0; i < 2; i++) {
        out->position[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->position[i], b->position[i]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->corners[i][j] = evtMovieInterpolateIntIfEnabled(enable, t, a->corners[i][j], b->corners[i][j]);
        }
    }
}

void evtBlendParamsB(s32 enable, f32 t, EffBlurTemplateBody *a, EffBlurTemplateBody *b, EffBlurTemplateBody *out)
{
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003685C0;
    }
    if (b == NULL) {
        b = &D_003685C0;
    }
    out->extent = evtMovieInterpolateUintIfEnabled(enable, t, a->extent, b->extent);
    out->source.color = evtPolygonMovieBlendColor(enable, t, a->source.color, b->source.color);
    out->source.blendControl = a->source.blendControl;
    out->source.angle = evtMovieInterpolateFloatIfEnabled(enable, t, a->source.angle, b->source.angle);
    out->source.displacement = evtMovieInterpolateFloatIfEnabled(enable, t, a->source.displacement, b->source.displacement);
    for (i = 0; i < 2; i++) {
        out->source.position[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->source.position[i], b->source.position[i]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->source.corners[i][j] = evtMovieInterpolateIntIfEnabled(enable, t, a->source.corners[i][j], b->source.corners[i][j]);
        }
    }
}

void evtPolygonMovieBlendMatrixParam(s32 enable, f32 t, EffSolidRectParams *a, EffSolidRectParams *b, EffSolidRectParams *out)
{
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003685F0;
    }
    if (b == NULL) {
        b = &D_003685F0;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->corners[i][j] = evtMovieInterpolateIntIfEnabled(enable, t, a->corners[i][j], b->corners[i][j]);
        }
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
}

void evtBlendParamsD(s32 enable, f32 t, EffResourceRectParams *a, EffResourceRectParams *b, EffResourceRectParams *out)
{
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_00368610;
    }
    if (b == NULL) {
        b = &D_00368610;
    }
    out->extent = evtMovieInterpolateUintIfEnabled(enable, t, a->extent, b->extent);
    for (i = 0; i < 2; i++) {
        out->center[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->center[i], b->center[i]);
    }
    out->draw.rgba = evtPolygonMovieBlendColor(enable, t, a->draw.rgba, b->draw.rgba);
    out->draw.blendControl = a->draw.blendControl;
}

void evtBlendParamsE(s32 enable, f32 t, EffBlurScatterParams *a, EffBlurScatterParams *b, EffBlurScatterParams *out)
{
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_00368640;
    }
    if (b == NULL) {
        b = &D_00368640;
    }
    out->count = evtMovieInterpolateUintIfEnabled(enable, t, a->count, b->count);
    out->delaySpread = evtMovieInterpolateUintIfEnabled(enable, t, a->delaySpread, b->delaySpread);
    out->angleStep = evtMovieInterpolateFloatIfEnabled(enable, t, a->angleStep, b->angleStep);
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->uvDisplacementAngleDegrees = evtMovieInterpolateFloatIfEnabled(enable, t, a->uvDisplacementAngleDegrees, b->uvDisplacementAngleDegrees);
    out->uvDisplacementAmplitude = evtMovieInterpolateFloatIfEnabled(enable, t, a->uvDisplacementAmplitude, b->uvDisplacementAmplitude);
    out->positionSpread = evtMovieInterpolateUintIfEnabled(enable, t, a->positionSpread, b->positionSpread);
    for (i = 0; i < 2; i++) {
        out->position[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->position[i], b->position[i]);
    }
    out->size = evtMovieInterpolateUintIfEnabled(enable, t, a->size, b->size);
}

void evtBlendParamsF(s32 enable, f32 t, EffBlurScaleParams *a, EffBlurScaleParams *b, EffBlurScaleParams *out)
{
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_00368670;
    }
    if (b == NULL) {
        b = &D_00368670;
    }
    out->count = evtMovieInterpolateUintIfEnabled(enable, t, a->count, b->count);
    out->phaseStep = evtMovieInterpolateFloatIfEnabled(enable, t, a->phaseStep, b->phaseStep);
    out->spacing = evtMovieInterpolateFloatIfEnabled(enable, t, a->spacing, b->spacing);
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->uvDisplacementAngleDegrees = evtMovieInterpolateFloatIfEnabled(enable, t, a->uvDisplacementAngleDegrees, b->uvDisplacementAngleDegrees);
    out->uvDisplacementAmplitude = evtMovieInterpolateFloatIfEnabled(enable, t, a->uvDisplacementAmplitude, b->uvDisplacementAmplitude);
    out->angleStep = evtMovieInterpolateFloatIfEnabled(enable, t, a->angleStep, b->angleStep);
    for (i = 0; i < 2; i++) {
        out->position[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->position[i], b->position[i]);
    }
    out->size = evtMovieInterpolateUintIfEnabled(enable, t, a->size, b->size);
}

void evtBlendParamsG(s32 enable, f32 t, EffBlurQuad *a, EffBlurQuad *b, EffBlurQuad *out)
{
    s32 i;
    s32 j;

    if (enable == 0) {
        t = 0.0f;
    }
    if (a == NULL) {
        a = &D_003686A0;
    }
    if (b == NULL) {
        b = &D_003686A0;
    }
    out->color = evtPolygonMovieBlendColor(enable, t, a->color, b->color);
    out->blendControl = a->blendControl;
    out->angle = evtMovieInterpolateFloatIfEnabled(enable, t, a->angle, b->angle);
    out->displacement = evtMovieInterpolateFloatIfEnabled(enable, t, a->displacement, b->displacement);
    for (i = 0; i < 2; i++) {
        out->position[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->position[i], b->position[i]);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            out->corners[i][j] = evtMovieInterpolateIntIfEnabled(enable, t, a->corners[i][j], b->corners[i][j]);
        }
    }
}

void evtBlendParamsH(s32 enable, f32 t, EvtBlendKey *a, EvtBlendKey *b, EvtBlendKey *out)
{
    s32 i;

    if (enable == 0) {
        t = 0.0f;
    }
    out->x = evtMovieInterpolateIntIfEnabled(enable, t, a->x, b->x);
    out->w[0] = evtMovieInterpolateIntIfEnabled(enable, t, a->w[0], b->w[0]);
    out->w[1] = evtMovieInterpolateIntIfEnabled(enable, t, a->w[1], b->w[1]);
    out->w[2] = evtMovieInterpolateIntIfEnabled(enable, t, a->w[2], b->w[2]);
    out->w[3] = evtMovieInterpolateIntIfEnabled(enable, t, a->w[3], b->w[3]);
    out->flagWord = a->flagWord;
    for (i = 0; i < 3; i++) {
        out->y[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->y[i], b->y[i]);
        out->z[i] = evtMovieInterpolateIntIfEnabled(enable, t, a->z[i], b->z[i]);
    }
}

void evtPolygonMovieSetObjectMode(PolyMovieObject *obj, u32 mode, s32 setFlags, s32 clearFlags)
{
    switch (mode) {
    case 0:
        effObjGetTransitionWork((EffWorldNode *)obj)->owner->flags &= ~MDL_SKIP_TRANSFORMS;
        break;
    case 1:
        effObjGetTransitionWork((EffWorldNode *)obj)->owner->flags |= MDL_SKIP_TRANSFORMS;
        break;
    case 2:
        dds3ClearObjectFlags(obj, 0x400);
        dds3ClearObjectFlags(obj, 0x200);
        dds3ClearObjectFlags(obj, 0x4000);
        dds3ClearObjectFlags(obj, 0x8000);
        return;
    case 3:
        dds3SetObjectFlags(obj, 0x400);
        dds3ClearObjectFlags(obj, 0x200);
        if (setFlags != 0) {
            dds3SetObjectFlags(obj, setFlags);
        }
        if (clearFlags != 0) {
            dds3ClearObjectFlags(obj, clearFlags);
        }
        return;
    case 4:
        dds3ClearObjectFlags(obj, 0x400);
        dds3SetObjectFlags(obj, 0x200);
        if (setFlags != 0) {
            dds3SetObjectFlags(obj, setFlags);
        }
        if (clearFlags != 0) {
            dds3ClearObjectFlags(obj, clearFlags);
        }
        break;
    }
}

/* Apply a caller-provided mask to the event state's flag word. */
void evtPolygonMovieSetFlagBits(KwlnTask *task, u32 bits)
{
    EvtGlobal *state;

    state = (EvtGlobal *)kwlnTaskGetUserValue(task);
    *state->flags = *state->flags | bits;
}

void evtPolygonMovieClearFlagBits(KwlnTask *task, u32 bits)
{
    EvtGlobal *state;

    state = (EvtGlobal *)kwlnTaskGetUserValue(task);
    *state->flags = *state->flags & ~bits;
}

/* Scale the clip to the elapsed fraction of its duration, then apply or undo it. */
s32 evtPolygonMovieScaleByProgress(PolyMovieObject *movie, s32 undo, s32 start, s32 end)
{
    PolyMovieClip *clip;
    s32 frame;
    s32 duration;

    frame = end - start;
    if (frame < 0) {
        frame = 0;
    }
    clip = movie->state->clip;
    if (clip != NULL) {
        duration = (s32)clip->duration;
        if (duration <= frame) {
            frame = duration;
        }
        evtScaleValueByMultiplier(clip, (f32)frame / (f32)duration);
        if (undo == 0) {
            sdfFreezeFloatCounter(clip);
        } else {
            sdfUnfreezeFloatCounter(clip);
        }
    }
}

/* Store elapsed frames, bounded below by zero and above by clip duration. */
void evtPolygonMovieClampTime(PolyMovieObject *movie, s32 unused, s32 start, s32 end)
{
    PolyMovieClip *clip;
    s32 frame;

    frame = end - start;
    if (frame < 0) {
        frame = 0;
    }
    clip = movie->state->clip;
    if (clip != NULL) {
        if ((s32)clip->duration <= frame) {
            frame = (s32)clip->duration;
        }
        clip->position = (f32)frame;
    }
}

/* Allocate and clear a polygon-movie event work block. */
PolyMovieWork *evtPolygonMovieAllocWork(void)
{
    PolyMovieWork *work;

    work = sdfAllocSizeClassBlock(0x11C);
    if (work == NULL) {
        return work;
    }
    memset(work, 0, 0x11C);
    return work;
}

PolyMovieWork *evtPolygonMovieInitWork(PolyMovieWork *work, PmdHeader *data, PmdHeader *sub, PmdHeader *sub2)
{
    s32 i;

    if (data == NULL) {
        return NULL;
    }
    work->unk_114 = 0;
    work->handle = -1;
    work->buffer = sdfAllocSizeClassBlock(0x1FC);
    for (i = 0; i < 0x7F; i++) {
        work->buffer[i] = 0;
    }
    work->unk_1C = 0;
    work->unk_24 = 0;
    work->unk_38 = 0;
    work->unk_44 = 0;
    work->unk_54 = 0;
    work->entries = data->entries;
    work->data = data;
    work->mainEntry1Data = NULL;
    work->mainEntry2Data = NULL;
    work->mainEntry10Data = NULL;
    work->mainEntry11Data = NULL;
    work->mainEntry12Data = NULL;
    work->mainEntry3Data = NULL;
    work->mainEntry9Data = NULL;
    work->mainEntry7Data = NULL;
    work->mainEntry8Data = NULL;
    work->mainEntry22Data = NULL;
    work->mainEntry23Data = NULL;
    for (i = 0; i < work->data->count; i++) {
        switch (work->entries[i].type) {
        case 2:
            work->mainEntry2Data = (u8 *)data + work->entries[i].offset;
            work->unk_24 = work->entries[i].value;
            break;
        case 10:
            work->mainEntry10Data = (u8 *)data + work->entries[i].offset;
            break;
        case 11:
            work->mainEntry11Data = (u8 *)data + work->entries[i].offset;
            break;
        case 12:
            work->mainEntry12Data = (u8 *)data + work->entries[i].offset;
            break;
        case 3:
            work->mainEntry3Data = (u8 *)data + work->entries[i].offset;
            work->unk_38 = work->entries[i].value;
            break;
        case 9:
            work->mainEntry9Data = (u8 *)data + work->entries[i].offset;
            break;
        case 1:
            work->mainEntry1Data = (u8 *)data + work->entries[i].offset;
            work->unk_1C = work->entries[i].value;
            break;
        case 6:
            work->mainEntry6Data = (u8 *)data + work->entries[i].offset;
            if (work->entries[i].value == 0) {
                work->handle = -1;
            } else {
                work->handle = itfMesCreateWindow(work->mainEntry6Data);
            }
            break;
        case 7:
            work->mainEntry7Data = (u8 *)data + work->entries[i].offset;
            work->unk_44 = work->entries[i].value;
            break;
        case 8:
            work->mainEntry8Data = (u8 *)data + work->entries[i].offset;
            break;
        case 22:
            work->mainEntry22Data = (u8 *)data + work->entries[i].offset;
            work->unk_54 = work->entries[i].value;
            break;
        case 23:
            work->mainEntry23Data = (u8 *)data + work->entries[i].offset;
            break;
        }
    }
    work->sub = sub;
    if (sub != NULL) {
        work->subEntries = sub->entries;
    } else {
        work->subEntries = NULL;
    }
    work->subEntry1Data = NULL;
    work->unk_80 = 0;
    work->subEntry0Data = NULL;
    work->subEntry4Kind4Data = NULL;
    work->unk_A0 = 0;
    work->subEntry5Data = NULL;
    work->unk_A8 = 0;
    work->subEntry13Data = NULL;
    work->unk_B0 = 0;
    work->subEntry14Data = NULL;
    work->unk_B8 = 0;
    work->subEntry15Data = NULL;
    work->unk_C0 = 0;
    work->subEntry16Data = NULL;
    work->unk_C8 = 0;
    work->subEntry17Data = NULL;
    work->unk_D0 = 0;
    work->subEntry18Data = NULL;
    work->unk_D8 = 0;
    work->subEntry19Data = NULL;
    work->unk_E0 = 0;
    work->subEntry20Data = NULL;
    work->unk_E8 = 0;
    work->subEntry21Data = NULL;
    work->unk_F8 = 0;
    work->subEntry24Data = NULL;
    work->unk_F0 = 0;
    work->subEntry25Data = NULL;
    work->unk_100 = 0;
    if (sub == NULL) {
        return work;
    }
    for (i = 0; i < work->sub->count; i++) {
        switch (work->subEntries[i].type) {
        case 0:
            work->subEntry0Data = (u8 *)sub + work->subEntries[i].offset;
            break;
        case 4:
            if (work->sub->kind == 4) {
                work->subEntry4OtherData = NULL;
                work->subEntry4Kind4Data = (u8 *)sub + work->subEntries[i].offset;
            } else {
                work->subEntry4Kind4Data = NULL;
                work->subEntry4OtherData = (u8 *)sub + work->subEntries[i].offset;
            }
            work->unk_A0 = work->subEntries[i].value;
            break;
        case 1:
            work->subEntry1Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_80 = work->subEntries[i].value;
            break;
        case 5:
            work->subEntry5Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_A8 = work->subEntries[i].value;
            break;
        case 13:
            work->subEntry13Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_B0 = work->subEntries[i].value;
            break;
        case 14:
            work->subEntry14Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_B8 = work->subEntries[i].value;
            break;
        case 15:
            work->subEntry15Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_C0 = work->subEntries[i].value;
            break;
        case 16:
            work->subEntry16Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_C8 = work->subEntries[i].value;
            break;
        case 17:
            work->subEntry17Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_D0 = work->subEntries[i].value;
            break;
        case 18:
            work->subEntry18Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_D8 = work->subEntries[i].value;
            break;
        case 19:
            work->subEntry19Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_E0 = work->subEntries[i].value;
            break;
        case 20:
            work->subEntry20Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_E8 = work->subEntries[i].value;
            break;
        case 24:
            work->subEntry24Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_F0 = work->subEntries[i].value;
            break;
        case 21:
            work->subEntry21Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_F8 = work->subEntries[i].value;
            func_003003F0("object table set ok.\n");
            break;
        case 25:
            work->subEntry25Data = (u8 *)sub + work->subEntries[i].offset;
            work->unk_100 = work->subEntries[i].value;
            break;
        }
    }
    work->sub2 = sub2;
    if (sub2 != NULL) {
        work->sub2Entries = sub2->entries;
    } else {
        work->sub2Entries = NULL;
    }
    work->secondEntry4Data = NULL;
    work->unk_9C = 0;
    if (sub2 == NULL) {
        return work;
    }
    for (i = 0; i < work->sub2->count; i++) {
        if (work->sub2Entries[i].type == 4) {
            work->secondEntry4Data = (u8 *)sub2 + work->sub2Entries[i].offset;
            work->unk_9C = work->sub2Entries[i].value;
        }
    }
    return work;
}

void evtPolygonMovieFreeWork(PolyMovieWork *work)
{
    if (work != NULL) {
        if (work->buffer != NULL) {
            sdfReleaseChipBlock(work->buffer);
        }
        if (work->handle >= 0) {
            itfMesDestroyWindowIfPresent(work->handle);
            work->handle = -1;
        }
        fileWaitIdle();
        if (work->mainResource.request != 0) {
            filePollEntryCleanup(work->mainResource.request);
        }
        if (work->secondaryResource.request != 0) {
            filePollEntryCleanup(work->secondaryResource.request);
        }
        if (work->mainResource.handle != 0) {
            sdfReleaseResourceAllocation(work->mainResource.handle);
        }
        if (work->secondaryResource.handle != 0) {
            sdfReleaseResourceAllocation(work->secondaryResource.handle);
        }
        if (work->tertiaryResource.handle != 0) {
            sdfReleaseResourceAllocation(work->tertiaryResource.handle);
        }
        if (mnuQueryTitleSoundBusy() == 1) {
            mnuStopTitleVoicePlayback();
        }
        func_003003F0("sound stop all. \n");
        sdfReleaseChipBlock(work);
    }
}

/* Allocate a resource block holding a fresh "PMD2" header; returns the handle. */
SdfMemBlock *evtPolygonMovieCreateHeader(void **out)
{
    s32 header[16] = {0, 0, 0x32444D50, 0, 1, 9, 0, 0, 0, 0x10, 1, 0x30, 0, 999, 1000, 0};
    s32 size;
    SdfMemBlock *handle;
    void *block;

    size = 0x40;
    handle = sdfAllocGeneralBlock(size);
    block = (void *)sdfResourceRetainAddress(handle);
    memcpy(block, header, size);
    *out = block;
    return handle;
}

#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "evt_viewer.h"
#include "kwln.h"
#include "sdf.h"
#include "evt_world.h"
#include "evt_picture.h"
#include "evt_polygon_movie.h"
#include "file.h"
#include "file_request_api.h"
#include "kwln_task_lifecycle.h"

extern SdfTex *itfLoadTextureFromAsset(const char *);



extern char D_003BBF78[];

/* Build the 0x20-byte PMD3 resource header, copy it into a fresh allocation and
 * hand the retained address back through `out`. */
SdfMemBlock *func_00234C18(u8 **out) {
    u8 buffer[0x20];
    s32 size = 0x20;
    SdfMemBlock *handle;
    u8 *address;

    memset(buffer, 0, size);
    memcpy(buffer + 8, D_003BBF78, 4);
    *(s32 *)(buffer + 0x14) = 9;
    handle = sdfAllocGeneralBlock(size);
    address = (u8 *)sdfResourceRetainAddress(handle);
    memcpy(address, buffer, size);
    *out = address;
    return handle;
}

s32 func_003014F0(char *output, const char *format, ...);

s32 evtFormatPolygonMoviePaths(s32 event, s32 id, char *path1, char *path2, char *path3) {
    func_003014F0(path1, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM1",
                  event / 10 * 10, event, event, id, event, id);
    func_003014F0(path2, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
                  event / 10 * 10, event, event, id, event, id);
    return func_003014F0(path3, "/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
                         event / 10 * 10, event, event, id, event, id);
}

extern s32 sdfPathExists(char *path);
const char D_003ADD10[] = "evtGetPolygonMovieWorkData2:0\n";
const char D_003ADD30[] = "evtGetPolygonMovieWorkData2:1\n";
const char D_003ADD50[] = "eventedit: load pm3 file\n";
const char D_003ADD70[] = "evtGetPolygonMovieWorkData2:2\n";
const char D_003ADD90[] = "evtGetPolygonMovieWorkData3:3\n";

/* Load or synthesize the three PMD resources for an event scene and bind them to a new work object. */
PolyMovieWork *func_00234DA8(s32 eventId, s32 sceneId, s32 mode) {
    char path1[0x40];
    char path2[0x40];
    char path3[0x40];
    u32 address1;
    u32 address2;
    u32 address3;
    SdfMemBlock *handle1;
    SdfMemBlock *handle2;
    SdfMemBlock *handle3;
    PolyMovieWork *work;

    evtFormatPolygonMoviePaths(eventId, sceneId, path1, path2, path3);
    func_003003F0(D_003ADD10);
    handle1 = sdfReadNamedResource(path1, &address1, 0);
    if (address1 == 0) {
        return NULL;
    }
    func_003003F0(D_003ADD30);
    if (mode == 0) {
        handle2 = sdfReadNamedResource(path2, &address2, 0);
    } else {
        handle2 = evtPolygonMovieCreateHeader((void **)&address2);
    }
    if (mode == 0 && sdfPathExists(path3) == 1) {
        handle3 = sdfReadNamedResource(path3, &address3, 0);
        func_003003F0(D_003ADD50);
    } else {
        handle3 = (SdfMemBlock *)func_00234C18((u8 **)&address3);
    }
    func_003003F0(D_003ADD70);
    work = evtPolygonMovieAllocWork();
    evtPolygonMovieInitWork(work, (PmdHeader *)address1, (PmdHeader *)address2, (PmdHeader *)address3);
    if (work == NULL) {
        return NULL;
    }
    func_003003F0(D_003ADD90);
    work->mainResource.handle = handle1;
    work->secondaryResource.handle = handle2;
    work->tertiaryResource.handle = handle3;
    work->eventId = eventId;
    work->sceneId = sceneId;
    return work;
}


extern u32 D_003BA8EC;
extern void *memset(void *dst, s32 value, u32 size);
extern s32 sdfPathExists(char *path);
extern KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay, TaskUpdate update, TaskDestroy destroy, u32 userValue);
extern void func_00232D28(KwlnTask *task);
extern void func_00232D48(EvtRuntime *viewer);
extern char D_003ADDB0[]; /* "(ZikkiPlayMode)EventViewer" */

/* Create the event viewer task `taskId` for event `event`/scene `id` and request its movie files. */
KwlnTask *evtViewerCreateTask(s32 taskId, s32 event, s32 id) {
    char path0[0x40];
    char path1[0x40];
    char path2[0x40];
    SdfMemBlock *viewerHandle;
    EvtRuntime *viewer;
    PolyMovieWork *work;
    KwlnTask *task;

    D_003BA8EC = 0x80000000;
    viewerHandle = sdfAllocGeneralBlock(0x2490);
    viewer = (EvtRuntime *)sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x2490);
    viewer->resourceHandle = viewerHandle;
    evtFormatPolygonMoviePaths(event, id, path0, path1, path2);
    work = evtPolygonMovieAllocWork();
    work->eventId = event;
    work->sceneId = id;
    work->mainResource.request = fileQueueDefaultCallbackRequest(path0);
    work->flags |= 2;
    work->secondaryResource.request = fileQueueDefaultCallbackRequest(path1);
    work->flags |= 4;
    if (sdfPathExists(path2) != 0) {
        work->tertiaryResource.request = fileQueueDefaultCallbackRequest(path2);
        work->flags |= 0x10;
    } else {
        work->tertiaryResource.request = 0;
        work->tertiaryResource.address = 0;
    }
    task = kwlnTaskCreate(D_003ADDB0, taskId, 1, 1, evtViewerStartUpdate, func_00232D28, (u32)viewer);
    viewer->windowContext = work;
    work->task = task;
    func_00232D48(viewer);
    return task;
}


/* Flag operations act on the scheduler task's user-value context. */
void evtSetContextFlag(KwlnTask *task) {
    EvtPictureWork *context;

    context = (EvtPictureWork *)kwlnTaskGetUserValue(task);
    context->flags = context->flags | EVT_PICTURE_FLAG_DRAW_ENABLED;
}

void evtClearContextFlag(KwlnTask *task) {
    EvtPictureWork *context;

    context = (EvtPictureWork *)kwlnTaskGetUserValue(task);
    context->flags = context->flags & ~EVT_PICTURE_FLAG_DRAW_ENABLED;
}

void evtDestroyTaskHierarchy(u32 task) {
    kwlnTaskDestroyWithHierarchy((KwlnTask *)task, 1);
}

/* Allocate the picture task's flag and texture state. */
EvtPictureWork *evtAllocateContext(void) {
    EvtPictureWork *context = sdfAllocSizeClassBlock(8);
    context->flags = 0;
    context->texture = NULL;
    return context;
}

void evtSetConvertedContextValue(EvtPictureWork *context, const char *path) {
    context->texture = itfLoadTextureFromAsset(path);
}

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_003ADDB0);

INCLUDE_SDATA(const s32, "event/evtPolygonMovie", D_003BBF78);

