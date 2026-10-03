#include "common.h"
#include "gs_packet.h"
#include "pcp_vu0.h"

extern u32 D_0037F770[4];

extern u32 kwlnDrawControlFlags;

extern u8 kwlnDrawOverlayEnabled;

extern u16 kwlnDrawOverlayAlpha;

extern u16 kwlnDrawOverlayScale;

extern u16 D_00438DB4;

extern u16 D_00438DB6;

extern u16 D_00438DB8;

extern u16 D_00438DBA;

extern u16 D_00438DBC;

extern u16 D_00438DBE;

/* These are pending/saved parameter bodies, not the allocated effect owners.
 * Copies exclude each owner's resource handle, allocation and slot pointers. */
typedef union {
    u32 rgba;
    u8 channels[4];
} KwlnColor;

typedef struct {
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
} KwlnRectBounds;

typedef struct {
    u32 words[5];
} KwlnDrawVectorParams;

typedef struct {
    KwlnColor color;
    u32 blendControl;
    KwlnRectBounds bounds;
} KwlnSolidRectParams;

typedef struct {
    u32 extent;
    u32 centerX;
    u32 centerY;
    KwlnColor color;
    u32 blendControl;
    KwlnRectBounds bounds;
} KwlnResourceRectParams;

typedef struct {
    KwlnColor color;
    u32 blendControl;
    f32 rotation;
    f32 scale;
    u32 centerX;
    u32 centerY;
    KwlnRectBounds bounds;
} KwlnBlurRectParams;

typedef struct {
    u32 extent;
    KwlnBlurRectParams source;
} KwlnPixelBlurParams;

typedef struct {
    u32 count;
    u32 delaySpread;
    f32 angleStep;
    KwlnColor color;
    u32 unk10;
    f32 unk14;
    f32 unk18;
    u32 x;
    u32 y;
    u32 positionSpread;
    u32 size;
} KwlnScatterBlurParams;

typedef struct {
    u32 count;
    f32 phaseStep;
    f32 spacing;
    KwlnColor color;
    u32 unk10;
    f32 unk14;
    f32 unk18;
    f32 angleStep;
    u32 x;
    u32 y;
    u32 size;
} KwlnScaleBlurParams;

extern KwlnResourceRectParams kwlnTexturedSquareParameters;

extern KwlnPixelBlurParams kwlnTexturedBlurParameters;

extern KwlnScatterBlurParams kwlnFilterBlurParameters;

extern u32 kwlnDistanceBlurErrorCount;

extern KwlnScaleBlurParams kwlnStaggeredBlurParameters;

extern u32 kwlnRippleBlurErrorCount;

extern u16 kwlnTexturedSquareTargetAlpha;

extern u16 kwlnTexturedSquareStartAlpha;

extern u16 kwlnTexturedSquareFadeCounter;

extern u16 kwlnTexturedSquareFadeDuration;

extern void effCopyCh75Common(void *);

extern void effEnableTexturedSquare(void);

extern void effDisableTexturedSquare(void);

extern u16 kwlnTexturedBlurStartAlpha;

extern u16 kwlnTexturedBlurFadeCounter;

extern u16 kwlnTexturedBlurTargetAlpha;

extern u16 kwlnTexturedBlurFadeDuration;

extern void effCopyCh71Common(void *);

extern void effEnableTexturedBlur(void);

extern void effDisableTexturedBlur(void);

extern u16 kwlnFilterBlurStartAlpha;

extern u16 kwlnFilterBlurFadeCounter;

extern u16 kwlnFilterBlurTargetAlpha;

extern u16 kwlnFilterBlurFadeDuration;

extern void effCopyCh72Common(void *);

extern void effEnableFilterBlur(void);

extern void effDisableFilterBlur(void);

extern u16 kwlnStaggeredBlurTargetAlpha;

extern u16 kwlnStaggeredBlurStartAlpha;

extern u16 kwlnStaggeredBlurFadeCounter;

extern u16 kwlnStaggeredBlurFadeDuration;

extern void effDisableStaggeredBlur(void);

extern void effCopyCh76Common(void *);

extern void effEnableStaggeredBlur(void);

extern void effMiscNormalizeVU(void);

extern KwlnSolidRectParams kwlnColorRectangleParameters;

extern u16 kwlnColorRectangleStartAlpha;

extern u16 kwlnColorRectangleFadeCounter;

extern u16 kwlnColorRectangleTargetAlpha;

extern u16 kwlnColorRectangleFadeDuration;

extern void effCopyColorRectangleParameters(void *);

extern void effEnableColorRectangle(void);

extern void effDisableColorRectangle(void);

extern KwlnBlurRectParams kwlnRectangleBlurParameters;

extern u16 kwlnRectangleBlurStartAlpha;

extern u16 kwlnRectangleBlurFadeCounter;

extern u16 kwlnRectangleBlurTargetAlpha;

extern u16 kwlnRectangleBlurFadeDuration;

extern void effCopyRectangleBlurParameters(void *);

extern void effEnableRectangleBlur(void);

extern void effDisableRectangleBlur(void);

extern u128 kwlnDefaultColorVector;

extern KwlnDrawVectorParams kwlnDrawVector;

#define KWLN_DRAW_VECTOR_PARAM_BYTES 0x14
#define KWLN_DRAW_ALPHA_CHANNEL 3
#define KWLN_DRAW_COLOR_FADE_BIT 0x80000
#define KWLN_DRAW_COLOR_TRANSITION_BIT 0x100000
#define KWLN_DRAW_SQUARE_FADE_BIT 0x200000
#define KWLN_DRAW_SQUARE_TRANSITION_BIT 0x400000
#define KWLN_DRAW_CLEAR_SQUARE_TRANSITION 0xFFBFFFFF
#define KWLN_DRAW_OFFSET_TRANSITION_BIT 0x800
#define KWLN_DRAW_CLEAR_OFFSET_TRANSITION 0xfffff7ff

/* Copy one complete vector into the default draw color state. */
void kwlnDrawCopyRow128(u128 *source) {
    PCP_COPY_VECTOR(&kwlnDefaultColorVector, source);
}

/* Copy the five-word draw vector parameter body, without interpreting its words. */
void kwlnDrawCopyWords20(KwlnDrawVectorParams *source) {
    memcpy(&kwlnDrawVector, source, KWLN_DRAW_VECTOR_PARAM_BYTES);
}

/* Store one indexed draw word; the caller supplies the array index. */
void dds3DrawSetIndexedWord(u32 value, s32 index) {
    D_0037F770[index] = value;
}

/* Initialize bounds covering the default 512-by-448 draw viewport. */
void kwlnDrawInitRect(KwlnRectBounds *rect) {
    rect->right = DRAW_VIEWPORT_WIDTH;
    rect->bottom = DRAW_VIEWPORT_HEIGHT;
    rect->top = 0;
    rect->left = 0;
}

/* Stage the solid rectangle's blend control word. */
void kwlnDrawSetDc8Second(u32 blendControl) {
    kwlnColorRectangleParameters.blendControl = blendControl;
}

/* Stage the solid rectangle's packed RGBA color. */
void kwlnDrawSetDc8First(u32 rgba) {
    kwlnColorRectangleParameters.color.rgba = rgba;
}

extern KwlnSolidRectParams *effGetCh74Params(void);
extern u16 D_00438E50;
extern u16 D_00438E52;

extern KwlnSolidRectParams D_0043E530;
/* Alias of kwlnColorRectangleParameters.bounds; the effect setter needs the preceding header. */
extern KwlnRectBounds D_0043E550;

/* Capture the live solid rectangle before applying or interpolating staged params.
 * Duration storage is 16-bit; the immediate test uses the full argument. */
void kwlnDrawSnapshotSolidRect(s32 duration) {
    D_0043E530 = *effGetCh74Params();
    D_00438E50 = 0;
    D_00438E52 = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        kwlnDrawInitRect(&D_0043E550);
        effCopyColorRectangleParameters((void *)((u8 *)&D_0043E550 - 8));
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_COLOR_TRANSITION_BIT;
    }
}

/* Fade solid-rectangle alpha from zero to the staged color; zero applies now. */
void kwlnDrawSetupDc8(s32 duration) {
    KwlnSolidRectParams *params = &kwlnColorRectangleParameters;
    s32 requestedDuration = duration;

    kwlnColorRectangleStartAlpha = 0;
    kwlnColorRectangleFadeCounter = 0;
    kwlnColorRectangleTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnColorRectangleFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_FADE_BIT;
        kwlnDrawInitRect(&params->bounds);
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
    kwlnColorRectangleStartAlpha = kwlnColorRectangleParameters.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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
    kwlnTexturedSquareParameters.blendControl = blendControl;
}

/* Stage the textured square's packed RGBA color. */
void kwlnDrawSetE08Fourth(u32 rgba) {
    kwlnTexturedSquareParameters.color.rgba = rgba;
}

/* Stage textured-square extent and center coordinates. */
void kwlnDrawSetE08Triple(u32 extent, u32 centerX, u32 centerY) {
    kwlnTexturedSquareParameters.extent = extent;
    kwlnTexturedSquareParameters.centerX = centerX;
    kwlnTexturedSquareParameters.centerY = centerY;
}

extern KwlnResourceRectParams D_0043E560;
extern u16 D_00438E5C;
extern u16 D_00438E5E;
extern u32 effGetCh75Work(void);

/* Save the live textured-square params; zero copies the staged body immediately. */
void kwlnDrawApplyEffectWord(s32 duration) {
    D_0043E560 = *(KwlnResourceRectParams *)effGetCh75Work();
    D_00438E5C = 0;
    D_00438E5E = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_SQUARE_TRANSITION;
        effCopyCh75Common(&kwlnTexturedSquareParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_SQUARE_TRANSITION_BIT;
    }
}

/* Fade square alpha from zero to its staged color, or apply/enable it now. */
void kwlnDrawSetupE08(s32 duration) {
    KwlnResourceRectParams *params = &kwlnTexturedSquareParameters;
    s32 requestedDuration = duration;

    kwlnTexturedSquareStartAlpha = 0;
    kwlnTexturedSquareFadeCounter = 0;
    kwlnTexturedSquareTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnTexturedSquareFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_FADE_BIT;
        effCopyCh75Common(params);
        effEnableTexturedSquare();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_SQUARE_FADE_BIT;
    }
}

/* Fade staged square alpha toward zero, or disable it now when duration is zero. */
void kwlnDrawEnableE08(s32 duration) {
    kwlnTexturedSquareTargetAlpha = 0;
    kwlnTexturedSquareStartAlpha = kwlnTexturedSquareParameters.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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

/* Apply offsets now or interpolate from the current offsets over duration updates.
 * Existing alpha/scale global names hold the two signed offset components. */
void kwlnDrawSetOffsetTransition(s32 duration, s32 x, s32 y) {
    if (duration == 0) {
        kwlnDrawOverlayAlpha = (s16)x;
        kwlnDrawOverlayScale = (s16)y;
        if ((x == 0) && (y == 0)) {
            kwlnDrawOverlayEnabled = 0;
        }
        else {
            kwlnDrawOverlayEnabled = 1;
        }
        kwlnDrawControlFlags = kwlnDrawControlFlags & KWLN_DRAW_CLEAR_OFFSET_TRANSITION;
        return;
    }
    D_00438DB8 = kwlnDrawOverlayAlpha;
    D_00438DBA = kwlnDrawOverlayScale;
    D_00438DBC = (s16)x;
    D_00438DBE = (s16)y;
    D_00438DB6 = (s16)duration;
    kwlnDrawControlFlags = kwlnDrawControlFlags | KWLN_DRAW_OFFSET_TRANSITION_BIT;
    D_00438DB4 = 0;
}

#define KWLN_DRAW_RECT_FADE_BIT 0x20000
#define KWLN_DRAW_RECT_TRANSITION_BIT 0x40000
#define KWLN_DRAW_CLEAR_RECT_TRANSITION 0xFFFBFFFF
#define KWLN_DRAW_TEXTURE_FADE_BIT 0x1000
#define KWLN_DRAW_TEXTURE_TRANSITION_BIT 0x8000
#define KWLN_DRAW_CLEAR_TEXTURE_TRANSITION 0xFFFF7FFF

/* Stage rectangle-blur rotation, scale and blend control, in the original order. */
void kwlnDrawSetD88FloatTriple(u32 blendControl, f32 rotation, f32 scale) {
    kwlnRectangleBlurParameters.rotation = rotation;
    kwlnRectangleBlurParameters.scale = scale;
    kwlnRectangleBlurParameters.blendControl = blendControl;
}

/* Stage the rectangle blur's packed RGBA color. */
void kwlnDrawSetD88First(u32 rgba) {
    kwlnRectangleBlurParameters.color.rgba = rgba;
}

/* Stage the rectangle blur's center coordinates. */
void kwlnDrawSetD88Pair(u32 centerX, u32 centerY) {
    kwlnRectangleBlurParameters.centerX = centerX;
    kwlnRectangleBlurParameters.centerY = centerY;
}

extern KwlnBlurRectParams D_0043E4E0;
extern u16 D_00438E44;
extern u16 D_00438E46;
extern u32 effGetCh70Params(void);

/* Save live rectangle-blur params, then apply now or request interpolation. */
void kwlnSetRectangleBlurParameterTransition(s32 duration) {
    D_0043E4E0 = *(KwlnBlurRectParams *)effGetCh70Params();
    D_00438E44 = 0;
    D_00438E46 = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_RECT_TRANSITION;
        kwlnDrawInitRect(&kwlnRectangleBlurParameters.bounds);
        effCopyRectangleBlurParameters(&kwlnRectangleBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_RECT_TRANSITION_BIT;
    }
}

/* Fade rectangle-blur alpha from zero to the staged color; zero applies/enables now. */
void kwlnDrawSetupD88(s32 duration) {
    KwlnBlurRectParams *params = &kwlnRectangleBlurParameters;
    s32 requestedDuration = duration;

    kwlnRectangleBlurStartAlpha = 0;
    kwlnRectangleBlurFadeCounter = 0;
    kwlnRectangleBlurTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnRectangleBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_FADE_BIT;
        kwlnDrawInitRect(&params->bounds);
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
    kwlnRectangleBlurStartAlpha = kwlnRectangleBlurParameters.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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
void kwlnDrawSetC70FloatTriple(u32 blendControl, f32 rotation, f32 scale) {
    kwlnTexturedBlurParameters.source.rotation = rotation;
    kwlnTexturedBlurParameters.source.scale = scale;
    kwlnTexturedBlurParameters.source.blendControl = blendControl;
}

/* Stage the textured blur's packed source RGBA color. */
void kwlnDrawSetC70Second(u32 rgba) {
    kwlnTexturedBlurParameters.source.color.rgba = rgba;
}

/* Stage textured-blur extent and source center coordinates. */
void kwlnDrawSetC70Triple(u32 extent, u32 centerX, u32 centerY) {
    kwlnTexturedBlurParameters.extent = extent;
    kwlnTexturedBlurParameters.source.centerX = centerX;
    kwlnTexturedBlurParameters.source.centerY = centerY;
}

extern KwlnPixelBlurParams D_0043E3C0;
extern u16 D_00438E20;
extern u16 D_00438E22;
extern u32 effGetCh71Work(void);

/* Save live textured-blur params; zero copies the staged body immediately. */
void kwlnDrawApplyEffectBlock(s32 duration) {
    D_0043E3C0 = *(KwlnPixelBlurParams *)effGetCh71Work();
    D_00438E20 = 0;
    D_00438E22 = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_TEXTURE_TRANSITION;
        effCopyCh71Common(&kwlnTexturedBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_TEXTURE_TRANSITION_BIT;
    }
}

/* Fade textured-blur alpha from zero to staged alpha, or apply/enable it now. */
void kwlnDrawSetupC70(s32 duration) {
    KwlnPixelBlurParams *params = &kwlnTexturedBlurParameters;
    s32 requestedDuration = duration;

    kwlnTexturedBlurStartAlpha = 0;
    kwlnTexturedBlurFadeCounter = 0;
    kwlnTexturedBlurTargetAlpha = params->source.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnTexturedBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        effCopyCh71Common(params);
        effEnableTexturedBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_TEXTURE_FADE_BIT;
    }
}

/* Fade staged textured-blur alpha toward zero. Immediate disable also copies params. */
void kwlnDrawSetupC70B(s32 duration) {
    KwlnPixelBlurParams *params = &kwlnTexturedBlurParameters;
    s32 requestedDuration = duration;

    kwlnTexturedBlurTargetAlpha = 0;
    kwlnTexturedBlurFadeCounter = 0;
    kwlnTexturedBlurStartAlpha = params->source.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnTexturedBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_FADE_BIT;
        kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_TRANSITION_BIT;
        effCopyCh71Common(params);
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
 * Negative counts and the opaque word/float parameters remain uninterpreted. */
void kwlnDrawSetCd0Clamped(s32 count, s32 size, s32 delaySpread, s32 unknownWord,
                           f32 angleStep, f32 secondFloat, f32 thirdFloat) {
    if (count >= KWLN_DRAW_FILTER_COUNT_LIMIT) {
        kwlnDistanceBlurErrorCount++;
        count = KWLN_DRAW_FILTER_MAX_COUNT;
    }
    kwlnFilterBlurParameters.count = count;
    kwlnFilterBlurParameters.size = size;
    kwlnFilterBlurParameters.delaySpread = delaySpread;
    kwlnFilterBlurParameters.angleStep = angleStep;
    kwlnFilterBlurParameters.unk14 = secondFloat;
    kwlnFilterBlurParameters.unk18 = thirdFloat;
    kwlnFilterBlurParameters.unk10 = unknownWord;
}

/* Stage the filter blur's packed RGBA color. */
void kwlnDrawSetCd0Fourth(u32 rgba) {
    kwlnFilterBlurParameters.color.rgba = rgba;
}

/* Stage filter-blur position spread and coordinates. */
void kwlnDrawSetCd0Triple(u32 positionSpread, u32 x, u32 y) {
    kwlnFilterBlurParameters.positionSpread = positionSpread;
    kwlnFilterBlurParameters.x = x;
    kwlnFilterBlurParameters.y = y;
}

extern KwlnScatterBlurParams D_0043E420;
extern u32 effGetCh72Work(void);
extern s16 D_00438E2C;
extern s16 D_00438E2E;

/* Save live filter-blur params; zero copies the staged body immediately. */
void kwlnSetFilterBlurParameterTransition(s32 duration) {
    D_0043E420 = *(KwlnScatterBlurParams *)effGetCh72Work();
    D_00438E2C = 0;
    D_00438E2E = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_FILTER_TRANSITION;
        effCopyCh72Common(&kwlnFilterBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_FILTER_TRANSITION_BIT;
    }
}

/* Fade filter alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void kwlnDrawSetupCd0(s32 duration) {
    KwlnScatterBlurParams *params = &kwlnFilterBlurParameters;
    s32 requestedDuration = duration;

    kwlnFilterBlurStartAlpha = 0;
    kwlnFilterBlurFadeCounter = 0;
    kwlnFilterBlurTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnFilterBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_FADE_BIT;
        effCopyCh72Common(params);
        effEnableFilterBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_FILTER_FADE_BIT;
        effCopyCh72Common(params);
        effDisableFilterBlur();
    }
}

/* Fade staged filter alpha toward zero; zero disables it immediately. */
void kwlnDrawEnableCd0(s32 duration) {
    kwlnFilterBlurTargetAlpha = 0;
    kwlnFilterBlurStartAlpha = kwlnFilterBlurParameters.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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
 * The opaque word and two remaining float fields keep neutral argument names. */
void kwlnDrawSetD30Clamped(s32 count, s32 unknownWord, f32 phaseStep, f32 spacing,
                           f32 thirdFloat, f32 fourthFloat, f32 angleStep) {
    if (count >= KWLN_DRAW_STAGGERED_COUNT_LIMIT) {
        kwlnRippleBlurErrorCount++;
        count = KWLN_DRAW_STAGGERED_MAX_COUNT;
    }
    kwlnStaggeredBlurParameters.count = count;
    kwlnStaggeredBlurParameters.phaseStep = phaseStep;
    kwlnStaggeredBlurParameters.spacing = spacing;
    kwlnStaggeredBlurParameters.unk14 = thirdFloat;
    kwlnStaggeredBlurParameters.unk18 = fourthFloat;
    kwlnStaggeredBlurParameters.angleStep = angleStep;
    kwlnStaggeredBlurParameters.unk10 = unknownWord;
}

/* Stage the staggered blur's packed RGBA color. */
void kwlnDrawSetD30Fourth(u32 rgba) {
    kwlnStaggeredBlurParameters.color.rgba = rgba;
}

/* Stage staggered-blur size and coordinates. */
void kwlnDrawSetD30Triple(u32 size, u32 x, u32 y) {
    kwlnStaggeredBlurParameters.size = size;
    kwlnStaggeredBlurParameters.x = x;
    kwlnStaggeredBlurParameters.y = y;
}

extern KwlnScaleBlurParams D_0043E480;
extern u32 effGetCh76Work(void);
extern s16 D_00438E38;
extern s16 D_00438E3A;

/* Save live staggered-blur params; zero copies the staged body immediately. */
void kwlnSetStaggeredBlurParameterTransition(s32 duration) {
    D_0043E480 = *(KwlnScaleBlurParams *)effGetCh76Work();
    D_00438E38 = 0;
    D_00438E3A = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_STAGGERED_TRANSITION;
        effCopyCh76Common(&kwlnStaggeredBlurParameters);
    } else {
        kwlnDrawControlFlags |= KWLN_DRAW_STAGGERED_TRANSITION_BIT;
    }
}

/* Fade staggered alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void kwlnDrawSetupD30(s32 duration) {
    KwlnScaleBlurParams *params = &kwlnStaggeredBlurParameters;
    s32 requestedDuration = duration;

    kwlnStaggeredBlurStartAlpha = 0;
    kwlnStaggeredBlurFadeCounter = 0;
    kwlnStaggeredBlurTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
    kwlnStaggeredBlurFadeDuration = requestedDuration;
    if (requestedDuration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_FADE_BIT;
        effCopyCh76Common(params);
        effEnableStaggeredBlur();
    }
    else {
        kwlnDrawControlFlags |= KWLN_DRAW_STAGGERED_FADE_BIT;
        effCopyCh76Common(params);
        effDisableStaggeredBlur();
    }
}

/* Fade staged staggered alpha toward zero; zero disables it immediately. */
void kwlnDrawEnableD30(s32 duration) {
    kwlnStaggeredBlurTargetAlpha = 0;
    kwlnStaggeredBlurStartAlpha = kwlnStaggeredBlurParameters.color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

/* vu0 routine: shortest-arc quaternion rotating direction vf10 to vf11. */
void func_00107D08(void) {
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
        if (!(0.9999f <= fabsf(component))) {
            goto choose_perpendicular;
        }

        VU0_MOVE_VF(vf10, vf0);
        VU0_CLEAR_W(vf10);
        VU0_SET_VF10_COMPONENT(z, -1.0f);
        goto compose_opposite;

choose_perpendicular:
        VU0_GET_VF10_Z(component);
        axis[0] = -component;
        axis[1] = 0.0f;
        VU0_GET_VF10_X(axis[2]);
        axis[3] = 0.0f;
        VU0_LOAD_VF(vf10, axis);
        VU0_NORMALIZE_VF10();

compose_opposite:
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
