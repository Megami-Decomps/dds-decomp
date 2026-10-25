#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "kwln.h"
#include "gs_packet.h"
#include "pcp_vu0.h"
#include "eff_blur.h"
#include "eff_rectangle_blur_api.h"
#include "eff_event_draw.h"
#include "eff_event_work_api.h"
#include "eff.h"

extern u32 dds3DrawFlag;
extern u8 dds3ScreenBlurFlag;
extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern u16 blurnow;
extern u16 blurmax;
extern s16 bluralphabase;
extern s16 blurscalebase;
extern s16 bluralphatarget;
extern s16 blurscaletarget;

extern f32 (*D_00324770[4])[4]; /* three light sources and a null */

/* Pending, saved, and frame blocks share the effect owner's parameter type.
 * dds3DrawFlag tracks each family's running fades and transitions. */


extern dds3Effect_Filter_t filter2target;
extern dds3Effect_PartialMultiFilterParam_t mfiltertarget;
extern dds3Effect_Blur_t blur2target;
extern dds3Effect_PartialMultiBlurParam_t mulblurtarget;
extern dds3Effect_DistortionBlurParam_t distblurtarget;
/* These snapshot buffers are eight-byte aligned; the parameter body is 0x2C bytes. */
extern dds3Effect_RippleBlurParam_t ripblurtarget __attribute__((aligned(8)));
extern dds3Effect_DistortionBlurParam_t kwlnFilterBlurTransitionStartParameters;
extern dds3Effect_RippleBlurParam_t dds3RipBlurBuf __attribute__((aligned(8)));
extern u128 kwlnDefaultColorVector;
extern KwlnDrawVectorParams kwlnDrawVector;
extern u16 filter2abase;
extern u16 filter2now;
extern u16 filter2atarget;
extern u16 filter2max;
extern u16 blur2abase;
extern u16 blur2now;
extern u16 blur2atarget;
extern u16 blur2max;
extern u16 mulblurabase;
extern u16 mulblurnow;
extern u16 mulbluratarget;
extern u16 mulblurmax;
extern u16 distblurabase;
extern u16 distblurnow;
extern u16 distbluratarget;
extern u16 distblurswapnow;
extern u16 distblurswapmax;
extern u16 distblurmax;
extern u16 ripbluratarget;
extern u16 ripblurswapnow;
extern u16 ripblurswapmax;
extern u16 ripblurabase;
extern u16 ripblurnow;
extern u16 ripblurmax;
extern u16 mfilteratarget;
extern u16 mfilterabase;
extern u16 mfilternow;
extern u16 mfiltermax;
extern u32 distblurovercnt;
extern u32 ripblurovercnt;
extern void Normalize(void);

#define KWLN_DRAW_VECTOR_PARAM_BYTES 0x14
#define KWLN_DRAW_ALPHA_CHANNEL 3
#define KWLN_DRAW_COLOR_FADE_BIT 0x80000
#define KWLN_DRAW_COLOR_TRANSITION_BIT 0x100000
#define KWLN_DRAW_SQUARE_FADE_BIT 0x200000
#define KWLN_DRAW_SQUARE_TRANSITION_BIT 0x400000
#define KWLN_DRAW_CLEAR_SQUARE_TRANSITION 0xFFBFFFFF
#define KWLN_DRAW_OVERLAY_TRANSITION_BIT 0x800
#define KWLN_DRAW_CLEAR_OVERLAY_TRANSITION 0xfffff7ff

/* Copy one complete vector into the default draw color state. */
void dds3SetAmbient(u128 *source) {
    SDF_VU0_COPYVECTOR(&kwlnDefaultColorVector, source);
}


/* Copy the five-word draw vector parameter body, without interpreting its words. */
void dds3SetFog(KwlnDrawVectorParams *source) {
    memcpy(&kwlnDrawVector, source, KWLN_DRAW_VECTOR_PARAM_BYTES);
}

/* Store a light-source pointer in the indexed light table. */
void dds3SetLight(f32 (*lightSource)[4], s32 index) {
    D_00324770[index] = lightSource;
}

/* Initialize bounds covering the default 512-by-448 draw viewport. */
void setdraw_xy(s32 corners[2][2]) {
    corners[1][0] = DRAW_VIEWPORT_WIDTH;
    corners[1][1] = DRAW_VIEWPORT_HEIGHT;
    corners[0][1] = 0;
    corners[0][0] = 0;
}

/* Stage the solid rectangle's blend control word. */
void dds3SetFilter2Para(u32 blendControl) {
    filter2target.blendControl = blendControl;
}

/* Stage the solid rectangle's packed RGBA color. */
void dds3SetFilter2RGBA(u32 rgba) {
    filter2target.color = rgba;
}

extern u16 filter2swapnow;
extern u16 filter2swapmax;

extern dds3Effect_Filter_t dds3Filter2Buf;

/* Capture the live solid rectangle before applying or interpolating staged params.
 * Duration storage is 16-bit; the immediate test uses the full argument. */
void dds3Filter2Swap(s32 duration) {
    dds3Filter2Buf = *Eve_FilterGetParams();
    filter2swapnow = 0;
    filter2swapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        setdraw_xy(filter2target.corners);
        Eve_FilterSetParam(&filter2target);
    } else {
        dds3DrawFlag |= KWLN_DRAW_COLOR_TRANSITION_BIT;
    }
}

/* Fade solid-rectangle alpha from zero to the staged color; zero applies now. */
void dds3Filter2On(s32 duration) {
    dds3Effect_Filter_t *params = &filter2target;
    s32 requestedDuration = duration;

    filter2abase = 0;
    filter2now = 0;
    filter2atarget = (params->color >> 24);
    filter2max = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_COLOR_FADE_BIT;
        setdraw_xy(params->corners);
        Eve_FilterSetParam(params);
        Eve_FilterON();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_COLOR_FADE_BIT;
    }
}

/* Fade staged solid-rectangle alpha toward zero; zero disables it immediately. */
void dds3Filter2Off(s32 duration) {
    filter2atarget = 0;
    filter2abase = (filter2target.color >> 24);
    filter2now = 0;
    filter2max = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_COLOR_FADE_BIT;
        dds3DrawFlag &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        Eve_FilterOFF();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_COLOR_FADE_BIT;
    }
}

/* Stage the textured square's blend control word. */
void dds3SetMFilterPara(u32 blendControl) {
    mfiltertarget.draw.blendControl = blendControl;
}

/* Stage the textured square's packed RGBA color. */
void dds3SetMFilterRGBA(u32 rgba) {
    mfiltertarget.draw.rgba = rgba;
}

/* Stage textured-square extent and center coordinates. */
void dds3SetMFilterArea(u32 extent, u32 centerX, u32 centerY) {
    mfiltertarget.extent = extent;
    mfiltertarget.center[0] = centerX;
    mfiltertarget.center[1] = centerY;
}

extern dds3Effect_PartialMultiFilterParam_t dds3MFilterBuf;
extern u16 mfilterswapnow;
extern u16 mfilterswapmax;

/* Save the live textured-square params; zero copies the staged body immediately. */
void dds3MFilterSwap(s32 duration) {
    dds3MFilterBuf = Eve_PartialMultiFilterGetParams()->params;
    mfilterswapnow = 0;
    mfilterswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= KWLN_DRAW_CLEAR_SQUARE_TRANSITION;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
    } else {
        dds3DrawFlag |= KWLN_DRAW_SQUARE_TRANSITION_BIT;
    }
}

/* Fade square alpha from zero to its staged color, or apply/enable it now. */
void dds3MFilterOn(s32 duration) {
    dds3Effect_PartialMultiFilterParam_t *params = &mfiltertarget;
    s32 requestedDuration = duration;

    mfilterabase = 0;
    mfilternow = 0;
    mfilteratarget = params->draw.color[KWLN_DRAW_ALPHA_CHANNEL];
    mfiltermax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        Eve_PartialMultiFilterSetParam(params);
        Eve_PartialMultiFilterON();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_SQUARE_FADE_BIT;
    }
}

/* Fade staged square alpha toward zero, or disable it now when duration is zero. */
void dds3MFilterOff(s32 duration) {
    mfilteratarget = 0;
    mfilterabase = mfiltertarget.draw.color[KWLN_DRAW_ALPHA_CHANNEL];
    mfilternow = 0;
    mfiltermax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        dds3DrawFlag &= ~KWLN_DRAW_SQUARE_TRANSITION_BIT;
        Eve_PartialMultiFilterOFF();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_SQUARE_FADE_BIT;
    }
}

/* Apply vignette alpha and Q12 scale now, or interpolate them over duration updates.
 * Despite the legacy function name, these values do not specify position offsets. */
void dds3SetBlur(s32 duration, s32 alpha, s32 scale) {
    if (duration == 0) {
        dds3ScreenBlurAlpha = (s16)alpha;
        dds3ScreenBlurScale = (s16)scale;
        if ((alpha == 0) && (scale == 0)) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        dds3DrawFlag = dds3DrawFlag & KWLN_DRAW_CLEAR_OVERLAY_TRANSITION;
        return;
    }
    bluralphabase = dds3ScreenBlurAlpha;
    blurscalebase = dds3ScreenBlurScale;
    bluralphatarget = (s16)alpha;
    blurscaletarget = (s16)scale;
    blurmax = (s16)duration;
    dds3DrawFlag = dds3DrawFlag | KWLN_DRAW_OVERLAY_TRANSITION_BIT;
    blurnow = 0;
}

#define KWLN_DRAW_RECT_FADE_BIT 0x20000
#define KWLN_DRAW_RECT_TRANSITION_BIT 0x40000
#define KWLN_DRAW_CLEAR_RECT_TRANSITION 0xFFFBFFFF
#define KWLN_DRAW_TEXTURE_FADE_BIT 0x1000
#define KWLN_DRAW_TEXTURE_TRANSITION_BIT 0x8000
#define KWLN_DRAW_CLEAR_TEXTURE_TRANSITION 0xFFFF7FFF

/* Stage rectangle-blur rotation, scale and blend control, in the original order. */
void dds3SetBlur2Para(f32 rotation, f32 scale, u32 blendControl) {
    blur2target.angle = rotation;
    blur2target.displacement = scale;
    blur2target.blendControl = blendControl;
}

/* Stage the rectangle blur's packed RGBA color. */
void dds3SetBlur2RGBA(u32 rgba) {
    blur2target.color = rgba;
}

/* Stage the rectangle blur's center coordinates. */
void dds3SetBlur2Area(u32 centerX, u32 centerY) {
    blur2target.position[0] = centerX;
    blur2target.position[1] = centerY;
}

extern dds3Effect_Blur_t kwlnRectangleBlurTransitionStartParameters;
extern u16 blur2swapnow;
extern u16 blur2swapmax;

/* Save live rectangle-blur params, then apply now or request interpolation. */
void dds3Blur2Swap(s32 duration) {
    kwlnRectangleBlurTransitionStartParameters = Eve_BlurGetParam()->source;
    blur2swapnow = 0;
    blur2swapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= KWLN_DRAW_CLEAR_RECT_TRANSITION;
        setdraw_xy(blur2target.corners);
        Eve_BlurSetParam(&blur2target);
    } else {
        dds3DrawFlag |= KWLN_DRAW_RECT_TRANSITION_BIT;
    }
}

/* Fade rectangle-blur alpha from zero to the staged color; zero applies/enables now. */
void dds3Blur2On(s32 duration) {
    dds3Effect_Blur_t *params = &blur2target;
    s32 requestedDuration = duration;

    blur2abase = 0;
    blur2now = 0;
    blur2atarget = (params->color >> 24);
    blur2max = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_RECT_FADE_BIT;
        setdraw_xy(params->corners);
        Eve_BlurSetParam(params);
        Eve_BlurON();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_RECT_FADE_BIT;
    }
}

/* Fade staged rectangle-blur alpha toward zero; zero disables it immediately. */
void dds3Blur2Off(s32 duration) {
    blur2atarget = 0;
    blur2abase = (blur2target.color >> 24);
    blur2now = 0;
    blur2max = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_RECT_FADE_BIT;
        dds3DrawFlag &= ~KWLN_DRAW_RECT_TRANSITION_BIT;
        Eve_BlurOFF();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_RECT_FADE_BIT;
    }
}

/* Stage textured-blur source rotation, scale and blend control. */
void dds3SetMultiBlurPara(f32 rotation, f32 scale, u32 blendControl) {
    mulblurtarget.source.angle = rotation;
    mulblurtarget.source.displacement = scale;
    mulblurtarget.source.blendControl = blendControl;
}

/* Stage the textured blur's packed source RGBA color. */
void dds3SetMultiBlurRGBA(u32 rgba) {
    mulblurtarget.source.color = rgba;
}

/* Stage textured-blur extent and source center coordinates. */
void dds3SetMultiBlurArea(u32 extent, u32 centerX, u32 centerY) {
    mulblurtarget.extent = extent;
    mulblurtarget.source.position[0] = centerX;
    mulblurtarget.source.position[1] = centerY;
}

extern dds3Effect_PartialMultiBlurParam_t dds3MultiBlurBuf;
extern u16 mulblurswapnow;
extern u16 mulblurswapmax;

/* Save live textured-blur params; zero copies the staged body immediately. */
void dds3MultiBlurSwap(s32 duration) {
    dds3MultiBlurBuf = Eve_PartialMultiBlurGetParams()->body;
    mulblurswapnow = 0;
    mulblurswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= KWLN_DRAW_CLEAR_TEXTURE_TRANSITION;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
    } else {
        dds3DrawFlag |= KWLN_DRAW_TEXTURE_TRANSITION_BIT;
    }
}

/* Fade textured-blur alpha from zero to staged alpha, or apply/enable it now. */
void dds3MultiBlurOn(s32 duration) {
    dds3Effect_PartialMultiBlurParam_t *params = &mulblurtarget;
    s32 requestedDuration = duration;

    mulblurabase = 0;
    mulblurnow = 0;
    mulbluratarget = (params->source.color >> 24);
    mulblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        Eve_PartialMultiBlurSetParam(params);
        Eve_PartialMultiBlurON();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_TEXTURE_FADE_BIT;
    }
}

/* Fade staged textured-blur alpha toward zero. Immediate disable also copies params. */
void dds3MultiBlurOff(s32 duration) {
    dds3Effect_PartialMultiBlurParam_t *params = &mulblurtarget;
    s32 requestedDuration = duration;

    mulbluratarget = 0;
    mulblurnow = 0;
    mulblurabase = (params->source.color >> 24);
    mulblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        dds3DrawFlag &= ~KWLN_DRAW_TEXTURE_TRANSITION_BIT;
        Eve_PartialMultiBlurSetParam(params);
        Eve_PartialMultiBlurOFF();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_TEXTURE_FADE_BIT;
    }
}

#define KWLN_DRAW_FILTER_FADE_BIT 0x2000
#define KWLN_DRAW_FILTER_TRANSITION_BIT 0x10000
#define KWLN_DRAW_CLEAR_FILTER_TRANSITION 0xFFFEFFFF
#define KWLN_DRAW_STAGGERED_FADE_BIT 0x800000
#define KWLN_DRAW_STAGGERED_TRANSITION_BIT 0x1000000
#define KWLN_DRAW_CLEAR_STAGGERED_TRANSITION 0xFEFFFFFF
#define KWLN_DRAW_FILTER_COUNT_LIMIT 0x65
#define KWLN_DRAW_FILTER_MAX_COUNT 0x64
#define KWLN_DRAW_STAGGERED_COUNT_LIMIT 0x29
#define KWLN_DRAW_STAGGERED_MAX_COUNT 0x28

/* Stage filter-blur parameters, counting/clamping only counts above 100.
 * The secondary values are forwarded into the shared effect parameter body. */
void dds3SetDistBlurPara(s32 filterCount, s32 size, s32 delaySpread, s32 blendControl,
                           f32 angleStep, f32 uvDisplacementAngleDegrees,
                           f32 uvDisplacementAmplitude) {
    if (filterCount >= KWLN_DRAW_FILTER_COUNT_LIMIT) {
        distblurovercnt++;
        filterCount = KWLN_DRAW_FILTER_MAX_COUNT;
    }
    distblurtarget.count = filterCount;
    distblurtarget.size = size;
    distblurtarget.delaySpread = delaySpread;
    distblurtarget.angleStep = angleStep;
    distblurtarget.uvDisplacementAngleDegrees = uvDisplacementAngleDegrees;
    distblurtarget.uvDisplacementAmplitude = uvDisplacementAmplitude;
    distblurtarget.blendControl = blendControl;
}

/* Stage the filter blur's packed RGBA color. */
void dds3SetDistBlurRGBA(u32 rgba) {
    distblurtarget.color = rgba;
}

/* Stage filter-blur position spread and coordinates. */
void dds3SetDistBlurArea(u32 positionSpread, u32 x, u32 y) {
    distblurtarget.positionSpread = positionSpread;
    distblurtarget.position[0] = x;
    distblurtarget.position[1] = y;
}

/* Save live filter-blur params; zero copies the staged body immediately. */
void dds3DistBlurSwap(s32 duration) {
    kwlnFilterBlurTransitionStartParameters = Eve_DistortionBlurGetParams()->params;
    distblurswapnow = 0;
    distblurswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= KWLN_DRAW_CLEAR_FILTER_TRANSITION;
        Eve_DistortionBlurSetParam(&distblurtarget);
    } else {
        dds3DrawFlag |= KWLN_DRAW_FILTER_TRANSITION_BIT;
    }
}

/* Fade filter alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void dds3DistBlurOn(s32 duration) {
    dds3Effect_DistortionBlurParam_t *params = &distblurtarget;
    s32 requestedDuration = duration;

    distblurabase = 0;
    distblurnow = 0;
    distbluratarget = (params->color >> 24);
    distblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_FILTER_FADE_BIT;
        Eve_DistortionBlurSetParam(params);
        Eve_DistortionBlurON();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_FILTER_FADE_BIT;
        Eve_DistortionBlurSetParam(params);
        Eve_DistortionBlurOFF();
    }
}

/* Fade staged filter alpha toward zero; zero disables it immediately. */
void dds3DistBlurOff(s32 duration) {
    distbluratarget = 0;
    distblurabase = (distblurtarget.color >> 24);
    distblurnow = 0;
    distblurmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_FILTER_FADE_BIT;
        dds3DrawFlag &= ~KWLN_DRAW_FILTER_TRANSITION_BIT;
        Eve_DistortionBlurOFF();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_FILTER_FADE_BIT;
    }
}

/* Stage staggered-blur parameters, counting/clamping only counts above 40.
 * Blend and UV-displacement inputs map to the corresponding effect fields. */
void dds3SetRipBlurPara(s32 count, s32 blendControl, f32 phaseStep, f32 spacing,
                           f32 uvDisplacementAngleDegrees,
                           f32 uvDisplacementAmplitude, f32 angleStep) {
    if (count >= KWLN_DRAW_STAGGERED_COUNT_LIMIT) {
        ripblurovercnt++;
        count = KWLN_DRAW_STAGGERED_MAX_COUNT;
    }
    ripblurtarget.count = count;
    ripblurtarget.phaseStep = phaseStep;
    ripblurtarget.spacing = spacing;
    ripblurtarget.uvDisplacementAngleDegrees = uvDisplacementAngleDegrees;
    ripblurtarget.uvDisplacementAmplitude = uvDisplacementAmplitude;
    ripblurtarget.angleStep = angleStep;
    ripblurtarget.blendControl = blendControl;
}

/* Stage the staggered blur's packed RGBA color. */
void dds3SetRipBlurRGBA(u32 rgba) {
    ripblurtarget.color = rgba;
}

/* Stage staggered-blur size and coordinates. */
void dds3SetRipBlurArea(u32 size, u32 x, u32 y) {
    ripblurtarget.size = size;
    ripblurtarget.position[0] = x;
    ripblurtarget.position[1] = y;
}

/* Save live staggered-blur params; zero copies the staged body immediately. */
void dds3RipBlurSwap(s32 duration) {
    dds3RipBlurBuf = Eve_RippleBlurGetParams()->params;
    ripblurswapnow = 0;
    ripblurswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= KWLN_DRAW_CLEAR_STAGGERED_TRANSITION;
        Eve_RippleBlurSetParam(&ripblurtarget);
    } else {
        dds3DrawFlag |= KWLN_DRAW_STAGGERED_TRANSITION_BIT;
    }
}

/* Fade staggered alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void dds3RipBlurOn(s32 duration) {
    dds3Effect_RippleBlurParam_t *params = &ripblurtarget;
    s32 requestedDuration = duration;

    ripblurabase = 0;
    ripblurnow = 0;
    ripbluratarget = (params->color >> 24);
    ripblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        Eve_RippleBlurSetParam(params);
        Eve_RippleBlurON();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_STAGGERED_FADE_BIT;
        Eve_RippleBlurSetParam(params);
        Eve_RippleBlurOFF();
    }
}

/* Fade staged staggered alpha toward zero; zero disables it immediately. */
void dds3RipBlurOff(s32 duration) {
    ripbluratarget = 0;
    ripblurabase = (ripblurtarget.color >> 24);
    ripblurnow = 0;
    ripblurmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        dds3DrawFlag &= ~KWLN_DRAW_STAGGERED_TRANSITION_BIT;
        Eve_RippleBlurOFF();
    }
    else {
        dds3DrawFlag |= KWLN_DRAW_STAGGERED_FADE_BIT;
    }
}

/* Interpolated parameter bodies of the per-frame update below. The event
 * polygon-movie blend helpers combine the block saved when the transition
 * started with the staged block into one of these, and the effCopy* call then
 * hands the result to the effect. */
extern dds3Effect_PartialMultiBlurParam_t key2;
extern dds3Effect_DistortionBlurParam_t kwlnFilterBlurFrameParameters;
extern dds3Effect_RippleBlurParam_t dds3BlurState __attribute__((aligned(8)));
extern dds3Effect_Blur_t kwlnRectangleBlurFrameParameters;
extern dds3Effect_Filter_t dds3BlurParam;
extern dds3Effect_PartialMultiFilterParam_t kwlnTexturedSquareFrameParameters;

/* The blend helpers replace the parameters, which are the leading fields the
 * staged block shares with their own parameter block. */
extern void evtHokanBlur2(s32 enable, f32 t, const dds3Effect_Blur_t *from, const dds3Effect_Blur_t *to, dds3Effect_Blur_t *out);
extern void evtHokanMultBlur(s32 enable, f32 t, const dds3Effect_PartialMultiBlurParam_t *from, const dds3Effect_PartialMultiBlurParam_t *to, dds3Effect_PartialMultiBlurParam_t *out);
extern void evtHokanMultFilter(s32 enable, f32 t, const dds3Effect_PartialMultiFilterParam_t *from, const dds3Effect_PartialMultiFilterParam_t *to, dds3Effect_PartialMultiFilterParam_t *out);
extern void evtHokanDistBlur(s32 enable, f32 t, const dds3Effect_DistortionBlurParam_t *from, const dds3Effect_DistortionBlurParam_t *to, dds3Effect_DistortionBlurParam_t *out);
extern void evtHokanRipBlur(s32 enable, f32 t, const dds3Effect_RippleBlurParam_t *from, const dds3Effect_RippleBlurParam_t *to, dds3Effect_RippleBlurParam_t *out);
extern void evtHokanFilter(s32 enable, f32 t, const dds3Effect_Filter_t *from, const dds3Effect_Filter_t *to, dds3Effect_Filter_t *out);

/* Advance every running kernel-draw fade and transition by one frame. Each arm
 * keeps its control bit set while its timer runs: it reinterpolates its
 * parameter block from the effect's saved live copy or its staged copy, hands
 * that block to the effect, and clears its bit on the final frame. The overlay
 * arm ramps the vignette alpha and scale before the vignette is drawn. */
void dds3BlurUpdate(void) {
    s32 alpha;

    if (dds3DrawFlag & KWLN_DRAW_OVERLAY_TRANSITION_BIT) {
        blurnow++;
        dds3ScreenBlurAlpha = bluralphabase + blurnow * (bluralphatarget - bluralphabase) / blurmax;
        dds3ScreenBlurScale = blurscalebase + blurnow * (blurscaletarget - blurscalebase) / blurmax;
        if (dds3ScreenBlurAlpha == 0 && dds3ScreenBlurScale == 0) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        if (blurnow >= blurmax) {
            dds3DrawFlag &= KWLN_DRAW_CLEAR_OVERLAY_TRANSITION;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_RECT_TRANSITION_BIT) {
        if (blur2swapnow < blur2swapmax) {
            blur2swapnow++;
        }
        evtHokanBlur2(1, (f32)blur2swapnow / (f32)blur2swapmax, &kwlnRectangleBlurTransitionStartParameters,
                        &blur2target, &kwlnRectangleBlurFrameParameters);
        setdraw_xy(kwlnRectangleBlurFrameParameters.corners);
        Eve_BlurSetParam(&kwlnRectangleBlurFrameParameters);
        if ((kwlnRectangleBlurFrameParameters.color >> 24) != 0) {
            Eve_BlurON();
        }
        else {
            Eve_BlurOFF();
        }
        if (blur2swapnow >= blur2swapmax) {
            dds3DrawFlag &= ~KWLN_DRAW_RECT_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_RECT_FADE_BIT) {
        if (blur2now < blur2max) {
            blur2now++;
        }
        kwlnRectangleBlurFrameParameters = blur2target;
        alpha = blur2abase + blur2now *
                (blur2atarget - blur2abase) /
                blur2max;
        kwlnRectangleBlurFrameParameters.color = (kwlnRectangleBlurFrameParameters.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(kwlnRectangleBlurFrameParameters.corners);
        Eve_BlurSetParam(&kwlnRectangleBlurFrameParameters);
        if (alpha == 0) {
            Eve_BlurOFF();
        }
        else {
            Eve_BlurON();
        }
        if (blur2now >= blur2max) {
            dds3DrawFlag &= ~KWLN_DRAW_RECT_FADE_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_TEXTURE_TRANSITION_BIT) {
        if (mulblurswapnow < mulblurswapmax) {
            mulblurswapnow++;
        }
        evtHokanMultBlur(1, (f32)mulblurswapnow / (f32)mulblurswapmax, &dds3MultiBlurBuf,
                        &mulblurtarget, &key2);
        Eve_PartialMultiBlurSetParam(&key2);
        if ((key2.source.color >> 24) != 0) {
            Eve_PartialMultiBlurON();
        }
        else {
            Eve_PartialMultiBlurOFF();
        }
        if (mulblurswapnow >= mulblurswapmax) {
            dds3DrawFlag &= ~KWLN_DRAW_TEXTURE_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_TEXTURE_FADE_BIT) {
        if (mulblurnow < mulblurmax) {
            mulblurnow++;
        }
        key2 = mulblurtarget;
        alpha = mulblurabase + mulblurnow *
                (mulbluratarget - mulblurabase) /
                mulblurmax;
        key2.source.color = (key2.source.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiBlurSetParam(&key2);
        if (alpha == 0) {
            Eve_PartialMultiBlurOFF();
        }
        else {
            Eve_PartialMultiBlurON();
        }
        if (mulblurnow >= mulblurmax) {
            dds3DrawFlag &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_FILTER_TRANSITION_BIT) {
        if (distblurswapnow < distblurswapmax) {
            distblurswapnow++;
        }
        evtHokanDistBlur(1, (f32)distblurswapnow / (f32)distblurswapmax, &kwlnFilterBlurTransitionStartParameters,
                        &distblurtarget, &kwlnFilterBlurFrameParameters);
        Eve_DistortionBlurSetParam(&kwlnFilterBlurFrameParameters);
        if ((kwlnFilterBlurFrameParameters.color >> 24) != 0) {
            Eve_DistortionBlurON();
        }
        else {
            Eve_DistortionBlurOFF();
        }
        if (distblurswapnow >= distblurswapmax) {
            dds3DrawFlag &= ~KWLN_DRAW_FILTER_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_FILTER_FADE_BIT) {
        if (distblurnow < distblurmax) {
            distblurnow++;
        }
        kwlnFilterBlurFrameParameters = distblurtarget;
        alpha = distblurabase + distblurnow *
                (distbluratarget - distblurabase) /
                distblurmax;
        kwlnFilterBlurFrameParameters.color = (kwlnFilterBlurFrameParameters.color & 0xFFFFFF) | (alpha << 24);
        Eve_DistortionBlurSetParam(&kwlnFilterBlurFrameParameters);
        if (alpha == 0) {
            Eve_DistortionBlurOFF();
        }
        else {
            Eve_DistortionBlurON();
        }
        if (distblurnow >= distblurmax) {
            dds3DrawFlag &= ~KWLN_DRAW_FILTER_FADE_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_STAGGERED_TRANSITION_BIT) {
        if (ripblurswapnow < ripblurswapmax) {
            ripblurswapnow++;
        }
        evtHokanRipBlur(1, (f32)ripblurswapnow / (f32)ripblurswapmax, &dds3RipBlurBuf,
                        &ripblurtarget, &dds3BlurState);
        Eve_RippleBlurSetParam(&dds3BlurState);
        if ((dds3BlurState.color >> 24) != 0) {
            Eve_RippleBlurON();
        }
        else {
            Eve_RippleBlurOFF();
        }
        if (ripblurswapnow >= ripblurswapmax) {
            dds3DrawFlag &= ~KWLN_DRAW_STAGGERED_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_STAGGERED_FADE_BIT) {
        if (ripblurnow < ripblurmax) {
            ripblurnow++;
        }
        memcpy(&dds3BlurState, &ripblurtarget,
               sizeof(dds3BlurState));
        alpha = ripblurabase + ripblurnow *
                (ripbluratarget - ripblurabase) /
                ripblurmax;
        dds3BlurState.color = (dds3BlurState.color & 0xFFFFFF) | (alpha << 24);
        Eve_RippleBlurSetParam(&dds3BlurState);
        if (alpha == 0) {
            Eve_RippleBlurOFF();
        }
        else {
            Eve_RippleBlurON();
        }
        if (ripblurnow >= ripblurmax) {
            dds3DrawFlag &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_COLOR_TRANSITION_BIT) {
        if (filter2swapnow < filter2swapmax) {
            filter2swapnow++;
        }
        evtHokanFilter(1, (f32)filter2swapnow / (f32)filter2swapmax, &dds3Filter2Buf,
                                        &filter2target, &dds3BlurParam);
        setdraw_xy(dds3BlurParam.corners);
        Eve_FilterSetParam(&dds3BlurParam);
        if ((dds3BlurParam.color >> 24) != 0) {
            Eve_FilterON();
        }
        else {
            Eve_FilterOFF();
        }
        if (filter2swapnow >= filter2swapmax) {
            dds3DrawFlag &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_COLOR_FADE_BIT) {
        if (filter2now < filter2max) {
            filter2now++;
        }
        dds3BlurParam = filter2target;
        alpha = filter2abase + filter2now *
                (filter2atarget - filter2abase) /
                filter2max;
        dds3BlurParam.color = (dds3BlurParam.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3BlurParam.corners);
        Eve_FilterSetParam(&dds3BlurParam);
        if (alpha == 0) {
            Eve_FilterOFF();
        }
        else {
            Eve_FilterON();
        }
        if (filter2now >= filter2max) {
            dds3DrawFlag &= ~KWLN_DRAW_COLOR_FADE_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_SQUARE_TRANSITION_BIT) {
        if (mfilterswapnow < mfilterswapmax) {
            mfilterswapnow++;
        }
        evtHokanMultFilter(1, (f32)mfilterswapnow / (f32)mfilterswapmax, &dds3MFilterBuf,
                        &mfiltertarget, &kwlnTexturedSquareFrameParameters);
        Eve_PartialMultiFilterSetParam(&kwlnTexturedSquareFrameParameters);
        if (kwlnTexturedSquareFrameParameters.draw.color[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            Eve_PartialMultiFilterON();
        }
        else {
            Eve_PartialMultiFilterOFF();
        }
        if (mfilterswapnow >= mfilterswapmax) {
            dds3DrawFlag &= ~KWLN_DRAW_SQUARE_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & KWLN_DRAW_SQUARE_FADE_BIT) {
        if (mfilternow < mfiltermax) {
            mfilternow++;
        }
        kwlnTexturedSquareFrameParameters = mfiltertarget;
        alpha = mfilterabase + mfilternow *
                (mfilteratarget - mfilterabase) /
                mfiltermax;
        kwlnTexturedSquareFrameParameters.draw.rgba = (kwlnTexturedSquareFrameParameters.draw.rgba & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiFilterSetParam(&kwlnTexturedSquareFrameParameters);
        if (alpha == 0) {
            Eve_PartialMultiFilterOFF();
        }
        else {
            Eve_PartialMultiFilterON();
        }
        if (mfilternow >= mfiltermax) {
            dds3DrawFlag &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        }
    }
}

extern float fabsf(float);

/* vu0 routine: shortest-arc quaternion rotating direction vf10 to vf11. */
void dds3RotationArc(void) {
    f32 identity[4];
    f32 axis[4];
    f32 dot;
    f32 component;

    memset(identity, 0, sizeof(identity));
    identity[3] = 1.0f;

    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_B_TO_A();
    VU0_MOVE_VF(vf11, vf2);
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    VU0_MOVE_VF(vf2, vf10);
    SDF_VU0_COPYVECTOR_B_TO_A();
    VU0_MOVE_VF(vf11, vf2);

    SDF_VU0_INNERPRODUCT(dot);
    if (dot >= 0.9999f) {
        SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)identity);
        return;
    }

    if (dot <= -0.9999f) {
        SDF_VU0_GETVECTOR_Y(component);
        if (0.9999f <= fabsf(component)) {
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            VU0_SET_VF10_COMPONENT(z, -1.0f);
        } else {
            SDF_VU0_GETVECTOR_Z(component);
            axis[0] = -component;
            axis[1] = 0.0f;
            SDF_VU0_GETVECTOR_X(axis[2]);
            axis[3] = 0.0f;
            SDF_VU0_LOADVECTOR(axis);
            SDF_VU0_NORMALIZE();
        }

        SDF_VU0_INNERPRODUCT(dot);
        SDF_VU0_OUTERPRODUCT();
        VU0_SET_VF10_W(dot);
        return;
    }

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(dot);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(axis);
    VU0_SET_VF10_W(dot);
    Normalize();
}
