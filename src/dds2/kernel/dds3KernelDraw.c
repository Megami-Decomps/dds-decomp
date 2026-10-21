#include "sdk_ps2.h"
#include "common.h"
#include "kwln.h"
#include "gs_packet.h"
#include "pcp_vu0.h"
#include "eff_blur.h"
#include "eff_rectangle_blur_api.h"
#include "eff_event_draw.h"
#include "eff_event_work_api.h"
#include "eff.h"

extern f32 *D_0037F770[4]; /* three light sources and a null */

extern u32 kwlnDrawControlFlags;

extern u8 kwlnDrawOverlayEnabled;

extern s16 kwlnDrawOverlayAlpha;

extern s16 kwlnDrawOverlayScale;

extern u16 kwlnDrawOverlayTransitionCounter;

extern u16 kwlnDrawOverlayTransitionDuration;

extern s16 kwlnDrawOverlayTransitionStartAlpha;

extern s16 kwlnDrawOverlayTransitionStartScale;

extern s16 kwlnDrawOverlayTransitionTargetAlpha;

extern s16 kwlnDrawOverlayTransitionTargetScale;


extern EffResourceRectParams kwlnTexturedSquareParameters;

extern EffBlurTemplateBody kwlnTexturedBlurParameters;

extern EffBlurScatterParams kwlnFilterBlurParameters;

extern u32 kwlnDistanceBlurErrorCount;

/* These snapshot buffers are eight-byte aligned; the parameter body is 0x2C bytes. */
extern EffBlurScaleParams kwlnStaggeredBlurParameters __attribute__((aligned(8)));

extern u32 kwlnRippleBlurErrorCount;

extern u16 kwlnTexturedSquareTargetAlpha;

extern u16 kwlnTexturedSquareStartAlpha;

extern u16 kwlnTexturedSquareFadeCounter;

extern u16 kwlnTexturedSquareFadeDuration;


extern u16 kwlnTexturedBlurStartAlpha;

extern u16 kwlnTexturedBlurFadeCounter;

extern u16 kwlnTexturedBlurTargetAlpha;

extern u16 kwlnTexturedBlurFadeDuration;


extern u16 kwlnFilterBlurStartAlpha;

extern u16 kwlnFilterBlurFadeCounter;

extern u16 kwlnFilterBlurTargetAlpha;

extern u16 kwlnFilterBlurFadeDuration;


extern u16 kwlnStaggeredBlurTargetAlpha;

extern u16 kwlnStaggeredBlurStartAlpha;

extern u16 kwlnStaggeredBlurFadeCounter;

extern u16 kwlnStaggeredBlurFadeDuration;

extern void effMiscNormalizeVU(void);

extern EffSolidRectParams kwlnColorRectangleParameters;

extern u16 kwlnColorRectangleStartAlpha;

extern u16 kwlnColorRectangleFadeCounter;

extern u16 kwlnColorRectangleTargetAlpha;

extern u16 kwlnColorRectangleFadeDuration;


extern EffBlurQuad kwlnRectangleBlurParameters;

extern u16 kwlnRectangleBlurStartAlpha;

extern u16 kwlnRectangleBlurFadeCounter;

extern u16 kwlnRectangleBlurTargetAlpha;

extern u16 kwlnRectangleBlurFadeDuration;


extern u128 kwlnDefaultColorVector;

extern KwlnDrawVectorParams kwlnDrawVector;

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
void kwlnDrawCopyRow128(u128 *source) {
    PCP_COPY_VECTOR(&kwlnDefaultColorVector, source);
}


/* Copy the five-word draw vector parameter body, without interpreting its words. */
void kwlnDrawCopyWords20(KwlnDrawVectorParams *source) {
    memcpy(&kwlnDrawVector, source, KWLN_DRAW_VECTOR_PARAM_BYTES);
}

/* Store a light-source pointer in the indexed light table. */
void dds3DrawSetLightSourcePointer(void *lightSource, s32 index) {
    D_0037F770[index] = lightSource;
}

/* Initialize bounds covering the default 512-by-448 draw viewport. */
void kwlnDrawInitRect(s32 corners[2][2]) {
    corners[1][0] = DRAW_VIEWPORT_WIDTH;
    corners[1][1] = DRAW_VIEWPORT_HEIGHT;
    corners[0][1] = 0;
    corners[0][0] = 0;
}

/* Stage the solid rectangle's blend control word. */
void kwlnDrawSetDc8Second(u32 blendControl) {
    kwlnColorRectangleParameters.blendControl = blendControl;
}

/* Stage the solid rectangle's packed RGBA color. */
void kwlnDrawSetDc8First(u32 rgba) {
    kwlnColorRectangleParameters.color = rgba;
}

extern u16 kwlnColorRectangleTransitionCounter;
extern u16 kwlnColorRectangleTransitionDuration;

extern EffSolidRectParams kwlnColorRectangleTransitionStartParameters;

/* Capture the live solid rectangle before applying or interpolating staged params.
 * Duration storage is 16-bit; the immediate test uses the full argument. */
void kwlnDrawSnapshotSolidRect(s32 duration) {
    kwlnColorRectangleTransitionStartParameters = *effGetCh74Params();
    kwlnColorRectangleTransitionCounter = 0;
    kwlnColorRectangleTransitionDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        kwlnDrawInitRect(kwlnColorRectangleParameters.corners);
        effCopyColorRectangleParameters(&kwlnColorRectangleParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_COLOR_TRANSITION_BIT;
    }
}

/* Fade solid-rectangle alpha from zero to the staged color; zero applies now. */
void kwlnDrawSetupDc8(s32 duration) {
    EffSolidRectParams *params = &kwlnColorRectangleParameters;
    s32 requestedDuration = duration;

    kwlnColorRectangleStartAlpha = 0;
    kwlnColorRectangleFadeCounter = 0;
    kwlnColorRectangleTargetAlpha = (params->color >> 24);
    kwlnColorRectangleFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_FADE_BIT;
        kwlnDrawInitRect(params->corners);
        effCopyColorRectangleParameters(params);
        effEnableColorRectangle();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_COLOR_FADE_BIT;
    }
}

/* Fade staged solid-rectangle alpha toward zero; zero disables it immediately. */
void kwlnDrawEnableDc8(s32 duration) {
    kwlnColorRectangleTargetAlpha = 0;
    kwlnColorRectangleStartAlpha = (kwlnColorRectangleParameters.color >> 24);
    kwlnColorRectangleFadeCounter = 0;
    kwlnColorRectangleFadeDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        effDisableColorRectangle();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_COLOR_FADE_BIT;
    }
}

/* Stage the textured square's blend control word. */
void kwlnDrawSetE08Fifth(u32 blendControl) {
    kwlnTexturedSquareParameters.draw.blendControl = blendControl;
}

/* Stage the textured square's packed RGBA color. */
void kwlnDrawSetE08Fourth(u32 rgba) {
    kwlnTexturedSquareParameters.draw.rgba = rgba;
}

/* Stage textured-square extent and center coordinates. */
void kwlnDrawSetE08Triple(u32 extent, u32 centerX, u32 centerY) {
    kwlnTexturedSquareParameters.extent = extent;
    kwlnTexturedSquareParameters.center[0] = centerX;
    kwlnTexturedSquareParameters.center[1] = centerY;
}

extern EffResourceRectParams kwlnTexturedSquareTransitionStartParameters;
extern u16 kwlnTexturedSquareTransitionCounter;
extern u16 kwlnTexturedSquareTransitionDuration;

/* Save the live textured-square params; zero copies the staged body immediately. */
void kwlnDrawSetupE08FromCh75(s32 duration) {
    kwlnTexturedSquareTransitionStartParameters = effGetCh75Work()->params;
    kwlnTexturedSquareTransitionCounter = 0;
    kwlnTexturedSquareTransitionDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_SQUARE_TRANSITION;
        effCopyTexturedSquareParameters(&kwlnTexturedSquareParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_SQUARE_TRANSITION_BIT;
    }
}

/* Fade square alpha from zero to its staged color, or apply/enable it now. */
void kwlnDrawSetupE08(s32 duration) {
    EffResourceRectParams *params = &kwlnTexturedSquareParameters;
    s32 requestedDuration = duration;

    kwlnTexturedSquareStartAlpha = 0;
    kwlnTexturedSquareFadeCounter = 0;
    kwlnTexturedSquareTargetAlpha = params->draw.color[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnTexturedSquareFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        effCopyTexturedSquareParameters(params);
        effEnableTexturedSquare();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_SQUARE_FADE_BIT;
    }
}

/* Fade staged square alpha toward zero, or disable it now when duration is zero. */
void kwlnDrawEnableE08(s32 duration) {
    kwlnTexturedSquareTargetAlpha = 0;
    kwlnTexturedSquareStartAlpha = kwlnTexturedSquareParameters.draw.color[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnTexturedSquareFadeCounter = 0;
    kwlnTexturedSquareFadeDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_TRANSITION_BIT;
        effDisableTexturedSquare();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_SQUARE_FADE_BIT;
    }
}

/* Apply vignette alpha and Q12 scale now, or interpolate them over duration updates.
 * Despite the legacy function name, these values do not specify position offsets. */
void kwlnDrawSetOverlayTransition(s32 duration, s32 alpha, s32 scale) {
    if (duration == 0) {
        kwlnDrawOverlayAlpha = (s16)alpha;
        kwlnDrawOverlayScale = (s16)scale;
        if ((alpha == 0) && (scale == 0)) {
            kwlnDrawOverlayEnabled = 0;
        }
        else {
            kwlnDrawOverlayEnabled = 1;
        }
        kwlnDrawControlFlags = kwlnDrawControlFlags & KWLN_DRAW_CLEAR_OVERLAY_TRANSITION;
        return;
    }
    kwlnDrawOverlayTransitionStartAlpha = kwlnDrawOverlayAlpha;
    kwlnDrawOverlayTransitionStartScale = kwlnDrawOverlayScale;
    kwlnDrawOverlayTransitionTargetAlpha = (s16)alpha;
    kwlnDrawOverlayTransitionTargetScale = (s16)scale;
    kwlnDrawOverlayTransitionDuration = (s16)duration;
    kwlnDrawControlFlags = kwlnDrawControlFlags | KWLN_DRAW_OVERLAY_TRANSITION_BIT;
    kwlnDrawOverlayTransitionCounter = 0;
}

#define KWLN_DRAW_RECT_FADE_BIT 0x20000
#define KWLN_DRAW_RECT_TRANSITION_BIT 0x40000
#define KWLN_DRAW_CLEAR_RECT_TRANSITION 0xFFFBFFFF
#define KWLN_DRAW_TEXTURE_FADE_BIT 0x1000
#define KWLN_DRAW_TEXTURE_TRANSITION_BIT 0x8000
#define KWLN_DRAW_CLEAR_TEXTURE_TRANSITION 0xFFFF7FFF

/* Stage rectangle-blur rotation, scale and blend control, in the original order. */
void kwlnDrawSetD88FloatTriple(f32 rotation, f32 scale, u32 blendControl) {
    kwlnRectangleBlurParameters.angle = rotation;
    kwlnRectangleBlurParameters.displacement = scale;
    kwlnRectangleBlurParameters.blendControl = blendControl;
}

/* Stage the rectangle blur's packed RGBA color. */
void kwlnDrawSetD88First(u32 rgba) {
    kwlnRectangleBlurParameters.color = rgba;
}

/* Stage the rectangle blur's center coordinates. */
void kwlnDrawSetD88Pair(u32 centerX, u32 centerY) {
    kwlnRectangleBlurParameters.position[0] = centerX;
    kwlnRectangleBlurParameters.position[1] = centerY;
}

extern EffBlurQuad kwlnRectangleBlurTransitionStartParameters;
extern u16 kwlnRectangleBlurTransitionCounter;
extern u16 kwlnRectangleBlurTransitionDuration;

/* Save live rectangle-blur params, then apply now or request interpolation. */
void kwlnSetRectangleBlurParameterTransition(s32 duration) {
    kwlnRectangleBlurTransitionStartParameters = effGetCh70Params()->source;
    kwlnRectangleBlurTransitionCounter = 0;
    kwlnRectangleBlurTransitionDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_RECT_TRANSITION;
        kwlnDrawInitRect(kwlnRectangleBlurParameters.corners);
        effCopyRectangleBlurParameters(&kwlnRectangleBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_RECT_TRANSITION_BIT;
    }
}

/* Fade rectangle-blur alpha from zero to the staged color; zero applies/enables now. */
void kwlnDrawSetupD88(s32 duration) {
    EffBlurQuad *params = &kwlnRectangleBlurParameters;
    s32 requestedDuration = duration;

    kwlnRectangleBlurStartAlpha = 0;
    kwlnRectangleBlurFadeCounter = 0;
    kwlnRectangleBlurTargetAlpha = (params->color >> 24);
    kwlnRectangleBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_FADE_BIT;
        kwlnDrawInitRect(params->corners);
        effCopyRectangleBlurParameters(params);
        effEnableRectangleBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_RECT_FADE_BIT;
    }
}

/* Fade staged rectangle-blur alpha toward zero; zero disables it immediately. */
void kwlnDrawEnableD88(s32 duration) {
    kwlnRectangleBlurTargetAlpha = 0;
    kwlnRectangleBlurStartAlpha = (kwlnRectangleBlurParameters.color >> 24);
    kwlnRectangleBlurFadeCounter = 0;
    kwlnRectangleBlurFadeDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_TRANSITION_BIT;
        effDisableRectangleBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_RECT_FADE_BIT;
    }
}

/* Stage textured-blur source rotation, scale and blend control. */
void kwlnDrawSetC70FloatTriple(f32 rotation, f32 scale, u32 blendControl) {
    kwlnTexturedBlurParameters.source.angle = rotation;
    kwlnTexturedBlurParameters.source.displacement = scale;
    kwlnTexturedBlurParameters.source.blendControl = blendControl;
}

/* Stage the textured blur's packed source RGBA color. */
void kwlnDrawSetC70Second(u32 rgba) {
    kwlnTexturedBlurParameters.source.color = rgba;
}

/* Stage textured-blur extent and source center coordinates. */
void kwlnDrawSetC70Triple(u32 extent, u32 centerX, u32 centerY) {
    kwlnTexturedBlurParameters.extent = extent;
    kwlnTexturedBlurParameters.source.position[0] = centerX;
    kwlnTexturedBlurParameters.source.position[1] = centerY;
}

extern EffBlurTemplateBody kwlnTexturedBlurTransitionStartParameters;
extern u16 kwlnTexturedBlurTransitionCounter;
extern u16 kwlnTexturedBlurTransitionDuration;

/* Save live textured-blur params; zero copies the staged body immediately. */
void kwlnDrawSetupC70FromCh71(s32 duration) {
    kwlnTexturedBlurTransitionStartParameters = effGetCh71Work()->body;
    kwlnTexturedBlurTransitionCounter = 0;
    kwlnTexturedBlurTransitionDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_TEXTURE_TRANSITION;
        effCopyTexturedBlurParameters(&kwlnTexturedBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_TEXTURE_TRANSITION_BIT;
    }
}

/* Fade textured-blur alpha from zero to staged alpha, or apply/enable it now. */
void kwlnDrawSetupC70(s32 duration) {
    EffBlurTemplateBody *params = &kwlnTexturedBlurParameters;
    s32 requestedDuration = duration;

    kwlnTexturedBlurStartAlpha = 0;
    kwlnTexturedBlurFadeCounter = 0;
    kwlnTexturedBlurTargetAlpha = (params->source.color >> 24);
    kwlnTexturedBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        effCopyTexturedBlurParameters(params);
        effEnableTexturedBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_TEXTURE_FADE_BIT;
    }
}

/* Fade staged textured-blur alpha toward zero. Immediate disable also copies params. */
void kwlnDrawSetupC70B(s32 duration) {
    EffBlurTemplateBody *params = &kwlnTexturedBlurParameters;
    s32 requestedDuration = duration;

    kwlnTexturedBlurTargetAlpha = 0;
    kwlnTexturedBlurFadeCounter = 0;
    kwlnTexturedBlurStartAlpha = (params->source.color >> 24);
    kwlnTexturedBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_TRANSITION_BIT;
        effCopyTexturedBlurParameters(params);
        effDisableTexturedBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_TEXTURE_FADE_BIT;
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
void kwlnDrawSetCd0Clamped(s32 filterCount, s32 size, s32 delaySpread, s32 blendControl,
                           f32 angleStep, f32 uvDisplacementAngleDegrees,
                           f32 uvDisplacementAmplitude) {
    if (filterCount >= KWLN_DRAW_FILTER_COUNT_LIMIT) {
        kwlnDistanceBlurErrorCount++;
        filterCount = KWLN_DRAW_FILTER_MAX_COUNT;
    }
    kwlnFilterBlurParameters.count = filterCount;
    kwlnFilterBlurParameters.size = size;
    kwlnFilterBlurParameters.delaySpread = delaySpread;
    kwlnFilterBlurParameters.angleStep = angleStep;
    kwlnFilterBlurParameters.uvDisplacementAngleDegrees = uvDisplacementAngleDegrees;
    kwlnFilterBlurParameters.uvDisplacementAmplitude = uvDisplacementAmplitude;
    kwlnFilterBlurParameters.blendControl = blendControl;
}

/* Stage the filter blur's packed RGBA color. */
void kwlnDrawSetCd0Fourth(u32 rgba) {
    kwlnFilterBlurParameters.color = rgba;
}

/* Stage filter-blur position spread and coordinates. */
void kwlnDrawSetCd0Triple(u32 positionSpread, u32 x, u32 y) {
    kwlnFilterBlurParameters.positionSpread = positionSpread;
    kwlnFilterBlurParameters.position[0] = x;
    kwlnFilterBlurParameters.position[1] = y;
}

extern EffBlurScatterParams kwlnFilterBlurTransitionStartParameters;
extern u16 kwlnFilterBlurTransitionCounter;
extern u16 kwlnFilterBlurTransitionDuration;

/* Save live filter-blur params; zero copies the staged body immediately. */
void kwlnSetFilterBlurParameterTransition(s32 duration) {
    kwlnFilterBlurTransitionStartParameters = effGetCh72Work()->params;
    kwlnFilterBlurTransitionCounter = 0;
    kwlnFilterBlurTransitionDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_FILTER_TRANSITION;
        effCopyFilterBlurParameters(&kwlnFilterBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_FILTER_TRANSITION_BIT;
    }
}

/* Fade filter alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void kwlnDrawSetupCd0(s32 duration) {
    EffBlurScatterParams *params = &kwlnFilterBlurParameters;
    s32 requestedDuration = duration;

    kwlnFilterBlurStartAlpha = 0;
    kwlnFilterBlurFadeCounter = 0;
    kwlnFilterBlurTargetAlpha = (params->color >> 24);
    kwlnFilterBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_FADE_BIT;
        effCopyFilterBlurParameters(params);
        effEnableFilterBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_FILTER_FADE_BIT;
        effCopyFilterBlurParameters(params);
        effDisableFilterBlur();
    }
}

/* Fade staged filter alpha toward zero; zero disables it immediately. */
void kwlnDrawEnableCd0(s32 duration) {
    kwlnFilterBlurTargetAlpha = 0;
    kwlnFilterBlurStartAlpha = (kwlnFilterBlurParameters.color >> 24);
    kwlnFilterBlurFadeCounter = 0;
    kwlnFilterBlurFadeDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_TRANSITION_BIT;
        effDisableFilterBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_FILTER_FADE_BIT;
    }
}

/* Stage staggered-blur parameters, counting/clamping only counts above 40.
 * Blend and UV-displacement inputs map to the corresponding effect fields. */
void kwlnDrawSetD30Clamped(s32 count, s32 blendControl, f32 phaseStep, f32 spacing,
                           f32 uvDisplacementAngleDegrees,
                           f32 uvDisplacementAmplitude, f32 angleStep) {
    if (count >= KWLN_DRAW_STAGGERED_COUNT_LIMIT) {
        kwlnRippleBlurErrorCount++;
        count = KWLN_DRAW_STAGGERED_MAX_COUNT;
    }
    kwlnStaggeredBlurParameters.count = count;
    kwlnStaggeredBlurParameters.phaseStep = phaseStep;
    kwlnStaggeredBlurParameters.spacing = spacing;
    kwlnStaggeredBlurParameters.uvDisplacementAngleDegrees = uvDisplacementAngleDegrees;
    kwlnStaggeredBlurParameters.uvDisplacementAmplitude = uvDisplacementAmplitude;
    kwlnStaggeredBlurParameters.angleStep = angleStep;
    kwlnStaggeredBlurParameters.blendControl = blendControl;
}

/* Stage the staggered blur's packed RGBA color. */
void kwlnDrawSetD30Fourth(u32 rgba) {
    kwlnStaggeredBlurParameters.color = rgba;
}

/* Stage staggered-blur size and coordinates. */
void kwlnDrawSetD30Triple(u32 size, u32 x, u32 y) {
    kwlnStaggeredBlurParameters.size = size;
    kwlnStaggeredBlurParameters.position[0] = x;
    kwlnStaggeredBlurParameters.position[1] = y;
}

extern EffBlurScaleParams kwlnStaggeredBlurTransitionStartParameters __attribute__((aligned(8)));
extern u16 kwlnStaggeredBlurTransitionCounter;
extern u16 kwlnStaggeredBlurTransitionDuration;

/* Save live staggered-blur params; zero copies the staged body immediately. */
void kwlnSetStaggeredBlurParameterTransition(s32 duration) {
    kwlnStaggeredBlurTransitionStartParameters = effGetCh76Work()->params;
    kwlnStaggeredBlurTransitionCounter = 0;
    kwlnStaggeredBlurTransitionDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_STAGGERED_TRANSITION;
        effCopyStaggeredBlurParameters(&kwlnStaggeredBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_STAGGERED_TRANSITION_BIT;
    }
}

/* Fade staggered alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void kwlnDrawSetupD30(s32 duration) {
    EffBlurScaleParams *params = &kwlnStaggeredBlurParameters;
    s32 requestedDuration = duration;

    kwlnStaggeredBlurStartAlpha = 0;
    kwlnStaggeredBlurFadeCounter = 0;
    kwlnStaggeredBlurTargetAlpha = (params->color >> 24);
    kwlnStaggeredBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        effCopyStaggeredBlurParameters(params);
        effEnableStaggeredBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_STAGGERED_FADE_BIT;
        effCopyStaggeredBlurParameters(params);
        effDisableStaggeredBlur();
    }
}

/* Fade staged staggered alpha toward zero; zero disables it immediately. */
void kwlnDrawEnableD30(s32 duration) {
    kwlnStaggeredBlurTargetAlpha = 0;
    kwlnStaggeredBlurStartAlpha = (kwlnStaggeredBlurParameters.color >> 24);
    kwlnStaggeredBlurFadeCounter = 0;
    kwlnStaggeredBlurFadeDuration = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_TRANSITION_BIT;
        effDisableStaggeredBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_STAGGERED_FADE_BIT;
    }
}

/* Interpolated parameter bodies of the per-frame update below. The event
 * polygon-movie blend helpers combine the block saved when the transition
 * started with the staged block into one of these, and the effCopy* call then
 * hands the result to the effect. */
extern EffBlurTemplateBody kwlnTexturedBlurFrameParameters;
extern EffBlurScatterParams kwlnFilterBlurFrameParameters;
extern EffBlurScaleParams kwlnStaggeredBlurFrameParameters __attribute__((aligned(8)));
extern EffBlurQuad kwlnRectangleBlurFrameParameters;
extern EffSolidRectParams kwlnColorRectangleFrameParameters;
extern EffResourceRectParams kwlnTexturedSquareFrameParameters;

/* The blend helpers replace the parameters, which are the leading fields the
 * staged block shares with their own parameter block. */
extern void evtBlendParamsA(s32 enable, f32 t, EffBlurQuad *from, EffBlurQuad *to, EffBlurQuad *out);
extern void evtBlendParamsB(s32 enable, f32 t, EffBlurTemplateBody *from, EffBlurTemplateBody *to, EffBlurTemplateBody *out);
extern void evtBlendParamsD(s32 enable, f32 t, EffResourceRectParams *from, EffResourceRectParams *to, EffResourceRectParams *out);
extern void evtBlendParamsE(s32 enable, f32 t, EffBlurScatterParams *from, EffBlurScatterParams *to, EffBlurScatterParams *out);
extern void evtBlendParamsF(s32 enable, f32 t, EffBlurScaleParams *from, EffBlurScaleParams *to, EffBlurScaleParams *out);
extern void evtPolygonMovieBlendMatrixParam(s32 enable, f32 t, EffSolidRectParams *from, EffSolidRectParams *to, EffSolidRectParams *out);

/* Advance every running kernel-draw fade and transition by one frame. Each arm
 * keeps its control bit set while its timer runs: it reinterpolates its
 * parameter block from the effect's saved live copy or its staged copy, hands
 * that block to the effect, and clears its bit on the final frame. The overlay
 * arm ramps the vignette alpha and scale before the vignette is drawn. */
void kwlnAdvanceDrawFadesAndTransitions(void) {
    s32 alpha;

    if (kwlnDrawControlFlags & KWLN_DRAW_OVERLAY_TRANSITION_BIT) {
        kwlnDrawOverlayTransitionCounter++;
        kwlnDrawOverlayAlpha = kwlnDrawOverlayTransitionStartAlpha + kwlnDrawOverlayTransitionCounter * (kwlnDrawOverlayTransitionTargetAlpha - kwlnDrawOverlayTransitionStartAlpha) / kwlnDrawOverlayTransitionDuration;
        kwlnDrawOverlayScale = kwlnDrawOverlayTransitionStartScale + kwlnDrawOverlayTransitionCounter * (kwlnDrawOverlayTransitionTargetScale - kwlnDrawOverlayTransitionStartScale) / kwlnDrawOverlayTransitionDuration;
        if (kwlnDrawOverlayAlpha == 0 && kwlnDrawOverlayScale == 0) {
            kwlnDrawOverlayEnabled = 0;
        }
        else {
            kwlnDrawOverlayEnabled = 1;
        }
        if (kwlnDrawOverlayTransitionCounter >= kwlnDrawOverlayTransitionDuration) {
            kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_OVERLAY_TRANSITION;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_RECT_TRANSITION_BIT) {
        if (kwlnRectangleBlurTransitionCounter < kwlnRectangleBlurTransitionDuration) {
            kwlnRectangleBlurTransitionCounter++;
        }
        evtBlendParamsA(1, (f32)kwlnRectangleBlurTransitionCounter / (f32)kwlnRectangleBlurTransitionDuration, &kwlnRectangleBlurTransitionStartParameters,
                        &kwlnRectangleBlurParameters, &kwlnRectangleBlurFrameParameters);
        kwlnDrawInitRect(kwlnRectangleBlurFrameParameters.corners);
        effCopyRectangleBlurParameters(&kwlnRectangleBlurFrameParameters);
        if ((kwlnRectangleBlurFrameParameters.color >> 24) != 0) {
            effEnableRectangleBlur();
        }
        else {
            effDisableRectangleBlur();
        }
        if (kwlnRectangleBlurTransitionCounter >= kwlnRectangleBlurTransitionDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_RECT_FADE_BIT) {
        if (kwlnRectangleBlurFadeCounter < kwlnRectangleBlurFadeDuration) {
            kwlnRectangleBlurFadeCounter++;
        }
        kwlnRectangleBlurFrameParameters = kwlnRectangleBlurParameters;
        alpha = kwlnRectangleBlurStartAlpha + kwlnRectangleBlurFadeCounter *
                (kwlnRectangleBlurTargetAlpha - kwlnRectangleBlurStartAlpha) /
                kwlnRectangleBlurFadeDuration;
        kwlnRectangleBlurFrameParameters.color = (kwlnRectangleBlurFrameParameters.color & 0xFFFFFF) | (alpha << 24);
        kwlnDrawInitRect(kwlnRectangleBlurFrameParameters.corners);
        effCopyRectangleBlurParameters(&kwlnRectangleBlurFrameParameters);
        if (alpha == 0) {
            effDisableRectangleBlur();
        }
        else {
            effEnableRectangleBlur();
        }
        if (kwlnRectangleBlurFadeCounter >= kwlnRectangleBlurFadeDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_FADE_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_TEXTURE_TRANSITION_BIT) {
        if (kwlnTexturedBlurTransitionCounter < kwlnTexturedBlurTransitionDuration) {
            kwlnTexturedBlurTransitionCounter++;
        }
        evtBlendParamsB(1, (f32)kwlnTexturedBlurTransitionCounter / (f32)kwlnTexturedBlurTransitionDuration, &kwlnTexturedBlurTransitionStartParameters,
                        &kwlnTexturedBlurParameters, &kwlnTexturedBlurFrameParameters);
        effCopyTexturedBlurParameters(&kwlnTexturedBlurFrameParameters);
        if ((kwlnTexturedBlurFrameParameters.source.color >> 24) != 0) {
            effEnableTexturedBlur();
        }
        else {
            effDisableTexturedBlur();
        }
        if (kwlnTexturedBlurTransitionCounter >= kwlnTexturedBlurTransitionDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_TEXTURE_FADE_BIT) {
        if (kwlnTexturedBlurFadeCounter < kwlnTexturedBlurFadeDuration) {
            kwlnTexturedBlurFadeCounter++;
        }
        kwlnTexturedBlurFrameParameters = kwlnTexturedBlurParameters;
        alpha = kwlnTexturedBlurStartAlpha + kwlnTexturedBlurFadeCounter *
                (kwlnTexturedBlurTargetAlpha - kwlnTexturedBlurStartAlpha) /
                kwlnTexturedBlurFadeDuration;
        kwlnTexturedBlurFrameParameters.source.color = (kwlnTexturedBlurFrameParameters.source.color & 0xFFFFFF) | (alpha << 24);
        effCopyTexturedBlurParameters(&kwlnTexturedBlurFrameParameters);
        if (alpha == 0) {
            effDisableTexturedBlur();
        }
        else {
            effEnableTexturedBlur();
        }
        if (kwlnTexturedBlurFadeCounter >= kwlnTexturedBlurFadeDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_FILTER_TRANSITION_BIT) {
        if (kwlnFilterBlurTransitionCounter < kwlnFilterBlurTransitionDuration) {
            kwlnFilterBlurTransitionCounter++;
        }
        evtBlendParamsE(1, (f32)kwlnFilterBlurTransitionCounter / (f32)kwlnFilterBlurTransitionDuration, &kwlnFilterBlurTransitionStartParameters,
                        &kwlnFilterBlurParameters, &kwlnFilterBlurFrameParameters);
        effCopyFilterBlurParameters(&kwlnFilterBlurFrameParameters);
        if ((kwlnFilterBlurFrameParameters.color >> 24) != 0) {
            effEnableFilterBlur();
        }
        else {
            effDisableFilterBlur();
        }
        if (kwlnFilterBlurTransitionCounter >= kwlnFilterBlurTransitionDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_FILTER_FADE_BIT) {
        if (kwlnFilterBlurFadeCounter < kwlnFilterBlurFadeDuration) {
            kwlnFilterBlurFadeCounter++;
        }
        kwlnFilterBlurFrameParameters = kwlnFilterBlurParameters;
        alpha = kwlnFilterBlurStartAlpha + kwlnFilterBlurFadeCounter *
                (kwlnFilterBlurTargetAlpha - kwlnFilterBlurStartAlpha) /
                kwlnFilterBlurFadeDuration;
        kwlnFilterBlurFrameParameters.color = (kwlnFilterBlurFrameParameters.color & 0xFFFFFF) | (alpha << 24);
        effCopyFilterBlurParameters(&kwlnFilterBlurFrameParameters);
        if (alpha == 0) {
            effDisableFilterBlur();
        }
        else {
            effEnableFilterBlur();
        }
        if (kwlnFilterBlurFadeCounter >= kwlnFilterBlurFadeDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_FADE_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_STAGGERED_TRANSITION_BIT) {
        if (kwlnStaggeredBlurTransitionCounter < kwlnStaggeredBlurTransitionDuration) {
            kwlnStaggeredBlurTransitionCounter++;
        }
        evtBlendParamsF(1, (f32)kwlnStaggeredBlurTransitionCounter / (f32)kwlnStaggeredBlurTransitionDuration, &kwlnStaggeredBlurTransitionStartParameters,
                        &kwlnStaggeredBlurParameters, &kwlnStaggeredBlurFrameParameters);
        effCopyStaggeredBlurParameters(&kwlnStaggeredBlurFrameParameters);
        if ((kwlnStaggeredBlurFrameParameters.color >> 24) != 0) {
            effEnableStaggeredBlur();
        }
        else {
            effDisableStaggeredBlur();
        }
        if (kwlnStaggeredBlurTransitionCounter >= kwlnStaggeredBlurTransitionDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_STAGGERED_FADE_BIT) {
        if (kwlnStaggeredBlurFadeCounter < kwlnStaggeredBlurFadeDuration) {
            kwlnStaggeredBlurFadeCounter++;
        }
        memcpy(&kwlnStaggeredBlurFrameParameters, &kwlnStaggeredBlurParameters,
               sizeof(kwlnStaggeredBlurFrameParameters));
        alpha = kwlnStaggeredBlurStartAlpha + kwlnStaggeredBlurFadeCounter *
                (kwlnStaggeredBlurTargetAlpha - kwlnStaggeredBlurStartAlpha) /
                kwlnStaggeredBlurFadeDuration;
        kwlnStaggeredBlurFrameParameters.color = (kwlnStaggeredBlurFrameParameters.color & 0xFFFFFF) | (alpha << 24);
        effCopyStaggeredBlurParameters(&kwlnStaggeredBlurFrameParameters);
        if (alpha == 0) {
            effDisableStaggeredBlur();
        }
        else {
            effEnableStaggeredBlur();
        }
        if (kwlnStaggeredBlurFadeCounter >= kwlnStaggeredBlurFadeDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_COLOR_TRANSITION_BIT) {
        if (kwlnColorRectangleTransitionCounter < kwlnColorRectangleTransitionDuration) {
            kwlnColorRectangleTransitionCounter++;
        }
        evtPolygonMovieBlendMatrixParam(1, (f32)kwlnColorRectangleTransitionCounter / (f32)kwlnColorRectangleTransitionDuration, &kwlnColorRectangleTransitionStartParameters,
                                        &kwlnColorRectangleParameters, &kwlnColorRectangleFrameParameters);
        kwlnDrawInitRect(kwlnColorRectangleFrameParameters.corners);
        effCopyColorRectangleParameters(&kwlnColorRectangleFrameParameters);
        if ((kwlnColorRectangleFrameParameters.color >> 24) != 0) {
            effEnableColorRectangle();
        }
        else {
            effDisableColorRectangle();
        }
        if (kwlnColorRectangleTransitionCounter >= kwlnColorRectangleTransitionDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_COLOR_FADE_BIT) {
        if (kwlnColorRectangleFadeCounter < kwlnColorRectangleFadeDuration) {
            kwlnColorRectangleFadeCounter++;
        }
        kwlnColorRectangleFrameParameters = kwlnColorRectangleParameters;
        alpha = kwlnColorRectangleStartAlpha + kwlnColorRectangleFadeCounter *
                (kwlnColorRectangleTargetAlpha - kwlnColorRectangleStartAlpha) /
                kwlnColorRectangleFadeDuration;
        kwlnColorRectangleFrameParameters.color = (kwlnColorRectangleFrameParameters.color & 0xFFFFFF) | (alpha << 24);
        kwlnDrawInitRect(kwlnColorRectangleFrameParameters.corners);
        effCopyColorRectangleParameters(&kwlnColorRectangleFrameParameters);
        if (alpha == 0) {
            effDisableColorRectangle();
        }
        else {
            effEnableColorRectangle();
        }
        if (kwlnColorRectangleFadeCounter >= kwlnColorRectangleFadeDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_FADE_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_SQUARE_TRANSITION_BIT) {
        if (kwlnTexturedSquareTransitionCounter < kwlnTexturedSquareTransitionDuration) {
            kwlnTexturedSquareTransitionCounter++;
        }
        evtBlendParamsD(1, (f32)kwlnTexturedSquareTransitionCounter / (f32)kwlnTexturedSquareTransitionDuration, &kwlnTexturedSquareTransitionStartParameters,
                        &kwlnTexturedSquareParameters, &kwlnTexturedSquareFrameParameters);
        effCopyTexturedSquareParameters(&kwlnTexturedSquareFrameParameters);
        if (kwlnTexturedSquareFrameParameters.draw.color[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableTexturedSquare();
        }
        else {
            effDisableTexturedSquare();
        }
        if (kwlnTexturedSquareTransitionCounter >= kwlnTexturedSquareTransitionDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_SQUARE_FADE_BIT) {
        if (kwlnTexturedSquareFadeCounter < kwlnTexturedSquareFadeDuration) {
            kwlnTexturedSquareFadeCounter++;
        }
        kwlnTexturedSquareFrameParameters = kwlnTexturedSquareParameters;
        alpha = kwlnTexturedSquareStartAlpha + kwlnTexturedSquareFadeCounter *
                (kwlnTexturedSquareTargetAlpha - kwlnTexturedSquareStartAlpha) /
                kwlnTexturedSquareFadeDuration;
        kwlnTexturedSquareFrameParameters.draw.rgba = (kwlnTexturedSquareFrameParameters.draw.rgba & 0xFFFFFF) | (alpha << 24);
        effCopyTexturedSquareParameters(&kwlnTexturedSquareFrameParameters);
        if (alpha == 0) {
            effDisableTexturedSquare();
        }
        else {
            effEnableTexturedSquare();
        }
        if (kwlnTexturedSquareFadeCounter >= kwlnTexturedSquareFadeDuration) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        }
    }
}


extern float fabsf(float);

/* vu0 routine: shortest-arc quaternion rotating direction vf10 to vf11. */
void kwlnBuildShortestArcQuaternion(void) {
    f32 identity[4];
    f32 axis[4];
    f32 dot;
    f32 component;

    memset(identity, 0, sizeof(identity));
    identity[3] = 1.0f;

    VU0_CLEAR_W(vf10);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf11);
    VU0_MOVE_VF(vf11, vf2);
    VU0_CLEAR_W(vf10);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf2, vf10);
    VU0_MOVE_VF(vf10, vf11);
    VU0_MOVE_VF(vf11, vf2);

    VU0_DOT_XYZ(dot, vf10, vf11);
    if (dot >= 0.9999f) {
        VU0_LOAD_VF_FROM(vf10, *(u128 *)identity);
        return;
    }

    if (dot <= -0.9999f) {
        VU0_GET_VF10_Y(component);
        if (0.9999f <= fabsf(component)) {
            VU0_MOVE_VF(vf10, vf0);
            VU0_CLEAR_W(vf10);
            VU0_SET_VF10_COMPONENT(z, -1.0f);
        } else {
            VU0_GET_VF10_Z(component);
            axis[0] = -component;
            axis[1] = 0.0f;
            VU0_GET_VF10_X(axis[2]);
            axis[3] = 0.0f;
            VU0_LOAD_VF(vf10, axis);
            VU0_NORMALIZE_VF10();
        }

        VU0_DOT_XYZ(dot, vf10, vf11);
        VU0_CROSS_XYZ(vf10, vf10, vf11);
        VU0_SET_VF10_W(dot);
        return;
    }

    VU0_MOVE_VF(vf12, vf10);
    VU0_ADD(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf12);
    VU0_DOT_XYZ(dot, vf10, vf11);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, axis);
    VU0_SET_VF10_W(dot);
    effMiscNormalizeVU();
}
