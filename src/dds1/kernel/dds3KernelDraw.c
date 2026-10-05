#include "common.h"
#include "gs_packet.h"
#include "pcp_vu0.h"

extern u32 kwlnDrawControlFlags;
extern u8 kwlnDrawOverlayEnabled;
extern s16 kwlnDrawOverlayAlpha;
extern s16 kwlnDrawOverlayScale;
extern u16 D_003BD6B4;
extern u16 D_003BD6B6;
extern s16 D_003BD6B8;
extern s16 D_003BD6BA;
extern s16 D_003BD6BC;
extern s16 D_003BD6BE;

extern u32 D_00324770[4];

/* Draw state blocks (D_003C2xxx) with dirty flags in kwlnDrawControlFlags. Each
 * setter writes its block, snapshots a flag byte to D_003BD7xx and
 * sets/clears its dirty bit. Overlapping word/byte views are unions;
 * byte symbols (e.g. D_003C2DCB = block+3) are declared separately
 * because those functions address the byte directly.
 */

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

/* Staggered (ripple) blur parameters. The staged block and the interpolated
 * body are both 8-byte aligned and the compiler copies between them with
 * aligned 8-byte moves, so the type carries that alignment; a body reached
 * through a cast pointer is still copied four bytes at a time. */
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
} KwlnScaleBlurParams __attribute__((aligned(8)));

extern KwlnSolidRectParams kwlnColorRectangleParameters;
extern KwlnResourceRectParams kwlnTexturedSquareParameters;
extern u32 D_003C2E14;
extern u32 D_003C2E18;
extern KwlnBlurRectParams kwlnRectangleBlurParameters;
extern KwlnPixelBlurParams kwlnTexturedBlurParameters;
extern KwlnScatterBlurParams kwlnFilterBlurParameters;
extern KwlnScaleBlurParams kwlnStaggeredBlurParameters;
extern KwlnScatterBlurParams D_003C2CA0;
extern KwlnScaleBlurParams D_003C2D00;
extern u32 D_003C2DCC;
extern u8 D_003C2DCB;
extern u8 D_003C2E17;
extern u8 D_003C2D8B;
extern u8 D_003C2CDF;
extern u8 D_003C2D3F;
extern u128 kwlnDefaultColorVector;
extern KwlnDrawVectorParams kwlnDrawVector;
extern u16 kwlnColorRectangleStartAlpha;
extern u16 kwlnColorRectangleFadeCounter;
extern u16 kwlnColorRectangleTargetAlpha;
extern u16 kwlnColorRectangleFadeDuration;
extern u16 kwlnRectangleBlurStartAlpha;
extern u16 kwlnRectangleBlurFadeCounter;
extern u16 kwlnRectangleBlurTargetAlpha;
extern u16 kwlnRectangleBlurFadeDuration;
extern u16 kwlnTexturedBlurStartAlpha;
extern u16 kwlnTexturedBlurFadeCounter;
extern u16 kwlnTexturedBlurTargetAlpha;
extern u16 kwlnTexturedBlurFadeDuration;
extern u16 kwlnFilterBlurStartAlpha;
extern u16 kwlnFilterBlurFadeCounter;
extern u16 kwlnFilterBlurTargetAlpha;
extern u16 D_003BD72C;
extern u16 D_003BD72E;
extern u16 kwlnFilterBlurFadeDuration;
extern u16 kwlnStaggeredBlurTargetAlpha;
extern u16 D_003BD738;
extern u16 D_003BD73A;
extern u16 kwlnStaggeredBlurStartAlpha;
extern u16 kwlnStaggeredBlurFadeCounter;
extern u16 kwlnStaggeredBlurFadeDuration;
extern u16 kwlnTexturedSquareTargetAlpha;
extern u16 kwlnTexturedSquareStartAlpha;
extern u16 kwlnTexturedSquareFadeCounter;
extern u16 kwlnTexturedSquareFadeDuration;
extern u32 kwlnDistanceBlurErrorCount;
extern u32 kwlnRippleBlurErrorCount;
extern void effCopyColorRectangleParameters(void *);
extern void effEnableColorRectangle(void);
extern void effCopyRectangleBlurParameters(void *);
extern void effEnableRectangleBlur(void);
extern void effDisableColorRectangle(void);
extern void effDisableTexturedSquare(void);
extern void effDisableRectangleBlur(void);
extern void effCopyTexturedBlurParameters(void *);
extern u32 effGetCh72Work(void);
extern void effEnableTexturedBlur(void);
extern void effCopyFilterBlurParameters(void *);
extern void effEnableFilterBlur(void);
extern void effDisableFilterBlur(void);
extern void effDisableStaggeredBlur(void);
extern void effCopyTexturedSquareParameters(void *);
extern void effEnableTexturedSquare(void);
extern void effDisableTexturedBlur(void);
extern void effCopyStaggeredBlurParameters(void *);
extern u32 effGetCh76Work(void);
extern void effEnableStaggeredBlur(void);
extern void effMiscNormalizeVU(void);

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
void kwlnDrawCopyRow128(void *source) {
    PCP_COPY_VECTOR(&kwlnDefaultColorVector, source);
}

/* Copy the five-word draw vector parameter body, without interpreting its words. */
void kwlnDrawCopyWords20(KwlnDrawVectorParams *source) {
    memcpy(&kwlnDrawVector, source, KWLN_DRAW_VECTOR_PARAM_BYTES);
}

/* Store one indexed draw word; the caller supplies the array index. */
void dds3DrawSetIndexedWord(u32 value, s32 index) {
    D_00324770[index] = value;
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
extern u16 D_003BD750;
extern u16 D_003BD752;

extern KwlnSolidRectParams D_003C2DB0;
/* Alias of kwlnColorRectangleParameters.bounds; the effect setter needs the preceding header. */
extern KwlnRectBounds D_003C2DD0;

/* Capture the live solid rectangle before applying or interpolating staged params.
 * Duration storage is 16-bit; the immediate test uses the full argument. */
void kwlnDrawSnapshotSolidRect(s32 duration) {
    D_003C2DB0 = *effGetCh74Params();
    D_003BD750 = 0;
    D_003BD752 = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        kwlnDrawInitRect(&D_003C2DD0);
        effCopyColorRectangleParameters((void *)((u8 *)&D_003C2DD0 - 8));
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

extern KwlnResourceRectParams D_003C2DE0;
extern u16 D_003BD75C;
extern u16 D_003BD75E;
extern u32 effGetCh75Work(void);

/* Save the live textured-square params; zero copies the staged body immediately. */
void kwlnDrawSetupE08FromCh75(s32 duration) {
    D_003C2DE0 = *(KwlnResourceRectParams *)effGetCh75Work();
    D_003BD75C = 0;
    D_003BD75E = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_SQUARE_TRANSITION;
        effCopyTexturedSquareParameters(&kwlnTexturedSquareParameters);
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
    D_003BD6B8 = kwlnDrawOverlayAlpha;
    D_003BD6BA = kwlnDrawOverlayScale;
    D_003BD6BC = (s16)alpha;
    D_003BD6BE = (s16)scale;
    D_003BD6B6 = (s16)duration;
    kwlnDrawControlFlags = kwlnDrawControlFlags | KWLN_DRAW_OVERLAY_TRANSITION_BIT;
    D_003BD6B4 = 0;
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

extern KwlnBlurRectParams D_003C2D60;
extern u16 D_003BD744;
extern u16 D_003BD746;
extern u32 effGetCh70Params(void);

/* Save live rectangle-blur params, then apply now or request interpolation. */
void kwlnSetRectangleBlurParameterTransition(s32 duration) {
    D_003C2D60 = *(KwlnBlurRectParams *)effGetCh70Params();
    D_003BD744 = 0;
    D_003BD746 = duration;
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

extern KwlnPixelBlurParams D_003C2C40;
extern u16 D_003BD720;
extern u16 D_003BD722;
extern u32 effGetCh71Work(void);

/* Save live textured-blur params; zero copies the staged body immediately. */
void kwlnDrawSetupC70FromCh71(s32 duration) {
    D_003C2C40 = *(KwlnPixelBlurParams *)effGetCh71Work();
    D_003BD720 = 0;
    D_003BD722 = duration;
    if (duration == 0) {
        kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_TEXTURE_TRANSITION;
        effCopyTexturedBlurParameters(&kwlnTexturedBlurParameters);
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
        effCopyTexturedBlurParameters(params);
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

/* Save live filter-blur params; zero copies the staged body immediately. */
void kwlnSetFilterBlurParameterTransition(s32 duration) {
    D_003C2CA0 = *(KwlnScatterBlurParams *)effGetCh72Work();
    D_003BD72C = 0;
    D_003BD72E = duration;
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
    KwlnScatterBlurParams *params = &kwlnFilterBlurParameters;
    s32 requestedDuration = duration;

    kwlnFilterBlurStartAlpha = 0;
    kwlnFilterBlurFadeCounter = 0;
    kwlnFilterBlurTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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

/* Save live staggered-blur params; zero copies the staged body immediately. */
void kwlnSetStaggeredBlurParameterTransition(s32 duration) {
    D_003C2D00 = *(KwlnScaleBlurParams *)effGetCh76Work();
    D_003BD738 = 0;
    D_003BD73A = duration;
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
    KwlnScaleBlurParams *params = &kwlnStaggeredBlurParameters;
    s32 requestedDuration = duration;

    kwlnStaggeredBlurStartAlpha = 0;
    kwlnStaggeredBlurFadeCounter = 0;
    kwlnStaggeredBlurTargetAlpha = params->color.channels[KWLN_DRAW_ALPHA_CHANNEL];
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

/* Interpolated parameter bodies of the per-frame update below. The event
 * polygon-movie blend helpers combine the block saved when the transition
 * started with the staged block into one of these, and the effCopy* call then
 * hands the result to the effect. */
extern KwlnPixelBlurParams D_003C2510;
extern KwlnScatterBlurParams D_003C2540;
extern KwlnScaleBlurParams D_003C2570;
extern KwlnBlurRectParams D_003C25A0;
extern KwlnSolidRectParams D_003C25D0;
extern KwlnResourceRectParams D_003C25F0;

/* The blend helpers replace the parameters, which are the leading fields the
 * staged block shares with their own parameter block. */
extern void evtBlendParamsA(s32 enable, f32 t, KwlnBlurRectParams *from, KwlnBlurRectParams *to, KwlnBlurRectParams *out);
extern void evtBlendParamsB(s32 enable, f32 t, KwlnPixelBlurParams *from, KwlnPixelBlurParams *to, KwlnPixelBlurParams *out);
extern void evtBlendParamsD(s32 enable, f32 t, KwlnResourceRectParams *from, KwlnResourceRectParams *to, KwlnResourceRectParams *out);
extern void evtBlendParamsE(s32 enable, f32 t, KwlnScatterBlurParams *from, KwlnScatterBlurParams *to, KwlnScatterBlurParams *out);
extern void evtBlendParamsF(s32 enable, f32 t, KwlnScaleBlurParams *from, KwlnScaleBlurParams *to, KwlnScaleBlurParams *out);
extern void evtPolygonMovieBlendMatrixParam(s32 enable, f32 t, KwlnSolidRectParams *from, KwlnSolidRectParams *to, KwlnSolidRectParams *out);

/* Advance every running kernel-draw fade and transition by one frame. Each arm
 * keeps its control bit set while its timer runs: it reinterpolates its
 * parameter block from the effect's saved live copy or its staged copy, hands
 * that block to the effect, and clears its bit on the final frame. The overlay
 * arm ramps the vignette alpha and scale before the vignette is drawn. */
void func_001071E8(void) {
    s32 alpha;

    if (kwlnDrawControlFlags & KWLN_DRAW_OVERLAY_TRANSITION_BIT) {
        D_003BD6B4++;
        kwlnDrawOverlayAlpha = D_003BD6B8 + D_003BD6B4 * (D_003BD6BC - D_003BD6B8) / D_003BD6B6;
        kwlnDrawOverlayScale = D_003BD6BA + D_003BD6B4 * (D_003BD6BE - D_003BD6BA) / D_003BD6B6;
        if (kwlnDrawOverlayAlpha == 0 && kwlnDrawOverlayScale == 0) {
            kwlnDrawOverlayEnabled = 0;
        }
        else {
            kwlnDrawOverlayEnabled = 1;
        }
        if (D_003BD6B4 >= D_003BD6B6) {
            kwlnDrawControlFlags &= KWLN_DRAW_CLEAR_OVERLAY_TRANSITION;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_RECT_TRANSITION_BIT) {
        if (D_003BD744 < D_003BD746) {
            D_003BD744++;
        }
        evtBlendParamsA(1, (f32)D_003BD744 / (f32)D_003BD746, &D_003C2D60,
                        &kwlnRectangleBlurParameters, &D_003C25A0);
        kwlnDrawInitRect(&D_003C25A0.bounds);
        effCopyRectangleBlurParameters(&D_003C25A0);
        if (D_003C25A0.color.channels[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableRectangleBlur();
        }
        else {
            effDisableRectangleBlur();
        }
        if (D_003BD744 >= D_003BD746) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_RECT_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_RECT_FADE_BIT) {
        if (kwlnRectangleBlurFadeCounter < kwlnRectangleBlurFadeDuration) {
            kwlnRectangleBlurFadeCounter++;
        }
        D_003C25A0 = kwlnRectangleBlurParameters;
        alpha = kwlnRectangleBlurStartAlpha + kwlnRectangleBlurFadeCounter *
                (kwlnRectangleBlurTargetAlpha - kwlnRectangleBlurStartAlpha) /
                kwlnRectangleBlurFadeDuration;
        D_003C25A0.color.rgba = (D_003C25A0.color.rgba & 0xFFFFFF) | (alpha << 24);
        kwlnDrawInitRect(&D_003C25A0.bounds);
        effCopyRectangleBlurParameters(&D_003C25A0);
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
        if (D_003BD720 < D_003BD722) {
            D_003BD720++;
        }
        evtBlendParamsB(1, (f32)D_003BD720 / (f32)D_003BD722, &D_003C2C40,
                        &kwlnTexturedBlurParameters, &D_003C2510);
        effCopyTexturedBlurParameters(&D_003C2510);
        if (D_003C2510.source.color.channels[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableTexturedBlur();
        }
        else {
            effDisableTexturedBlur();
        }
        if (D_003BD720 >= D_003BD722) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_TEXTURE_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_TEXTURE_FADE_BIT) {
        if (kwlnTexturedBlurFadeCounter < kwlnTexturedBlurFadeDuration) {
            kwlnTexturedBlurFadeCounter++;
        }
        D_003C2510 = kwlnTexturedBlurParameters;
        alpha = kwlnTexturedBlurStartAlpha + kwlnTexturedBlurFadeCounter *
                (kwlnTexturedBlurTargetAlpha - kwlnTexturedBlurStartAlpha) /
                kwlnTexturedBlurFadeDuration;
        D_003C2510.source.color.rgba = (D_003C2510.source.color.rgba & 0xFFFFFF) | (alpha << 24);
        effCopyTexturedBlurParameters(&D_003C2510);
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
        if (D_003BD72C < D_003BD72E) {
            D_003BD72C++;
        }
        evtBlendParamsE(1, (f32)D_003BD72C / (f32)D_003BD72E, &D_003C2CA0,
                        &kwlnFilterBlurParameters, &D_003C2540);
        effCopyFilterBlurParameters(&D_003C2540);
        if (D_003C2540.color.channels[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableFilterBlur();
        }
        else {
            effDisableFilterBlur();
        }
        if (D_003BD72C >= D_003BD72E) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_FILTER_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_FILTER_FADE_BIT) {
        if (kwlnFilterBlurFadeCounter < kwlnFilterBlurFadeDuration) {
            kwlnFilterBlurFadeCounter++;
        }
        D_003C2540 = kwlnFilterBlurParameters;
        alpha = kwlnFilterBlurStartAlpha + kwlnFilterBlurFadeCounter *
                (kwlnFilterBlurTargetAlpha - kwlnFilterBlurStartAlpha) /
                kwlnFilterBlurFadeDuration;
        D_003C2540.color.rgba = (D_003C2540.color.rgba & 0xFFFFFF) | (alpha << 24);
        effCopyFilterBlurParameters(&D_003C2540);
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
        if (D_003BD738 < D_003BD73A) {
            D_003BD738++;
        }
        evtBlendParamsF(1, (f32)D_003BD738 / (f32)D_003BD73A, &D_003C2D00,
                        &kwlnStaggeredBlurParameters, &D_003C2570);
        effCopyStaggeredBlurParameters(&D_003C2570);
        if (D_003C2570.color.channels[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableStaggeredBlur();
        }
        else {
            effDisableStaggeredBlur();
        }
        if (D_003BD738 >= D_003BD73A) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_STAGGERED_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_STAGGERED_FADE_BIT) {
        if (kwlnStaggeredBlurFadeCounter < kwlnStaggeredBlurFadeDuration) {
            kwlnStaggeredBlurFadeCounter++;
        }
        D_003C2570 = kwlnStaggeredBlurParameters;
        alpha = kwlnStaggeredBlurStartAlpha + kwlnStaggeredBlurFadeCounter *
                (kwlnStaggeredBlurTargetAlpha - kwlnStaggeredBlurStartAlpha) /
                kwlnStaggeredBlurFadeDuration;
        D_003C2570.color.rgba = (D_003C2570.color.rgba & 0xFFFFFF) | (alpha << 24);
        effCopyStaggeredBlurParameters(&D_003C2570);
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
        if (D_003BD750 < D_003BD752) {
            D_003BD750++;
        }
        evtPolygonMovieBlendMatrixParam(1, (f32)D_003BD750 / (f32)D_003BD752, &D_003C2DB0,
                                        &kwlnColorRectangleParameters, &D_003C25D0);
        kwlnDrawInitRect(&D_003C25D0.bounds);
        effCopyColorRectangleParameters(&D_003C25D0);
        if (D_003C25D0.color.channels[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableColorRectangle();
        }
        else {
            effDisableColorRectangle();
        }
        if (D_003BD750 >= D_003BD752) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_COLOR_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_COLOR_FADE_BIT) {
        if (kwlnColorRectangleFadeCounter < kwlnColorRectangleFadeDuration) {
            kwlnColorRectangleFadeCounter++;
        }
        D_003C25D0 = kwlnColorRectangleParameters;
        alpha = kwlnColorRectangleStartAlpha + kwlnColorRectangleFadeCounter *
                (kwlnColorRectangleTargetAlpha - kwlnColorRectangleStartAlpha) /
                kwlnColorRectangleFadeDuration;
        D_003C25D0.color.rgba = (D_003C25D0.color.rgba & 0xFFFFFF) | (alpha << 24);
        kwlnDrawInitRect(&D_003C25D0.bounds);
        effCopyColorRectangleParameters(&D_003C25D0);
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
        if (D_003BD75C < D_003BD75E) {
            D_003BD75C++;
        }
        evtBlendParamsD(1, (f32)D_003BD75C / (f32)D_003BD75E, &D_003C2DE0,
                        &kwlnTexturedSquareParameters, &D_003C25F0);
        effCopyTexturedSquareParameters(&D_003C25F0);
        if (D_003C25F0.color.channels[KWLN_DRAW_ALPHA_CHANNEL] != 0) {
            effEnableTexturedSquare();
        }
        else {
            effDisableTexturedSquare();
        }
        if (D_003BD75C >= D_003BD75E) {
            kwlnDrawControlFlags &= ~KWLN_DRAW_SQUARE_TRANSITION_BIT;
        }
    }
    if (kwlnDrawControlFlags & KWLN_DRAW_SQUARE_FADE_BIT) {
        if (kwlnTexturedSquareFadeCounter < kwlnTexturedSquareFadeDuration) {
            kwlnTexturedSquareFadeCounter++;
        }
        D_003C25F0 = kwlnTexturedSquareParameters;
        alpha = kwlnTexturedSquareStartAlpha + kwlnTexturedSquareFadeCounter *
                (kwlnTexturedSquareTargetAlpha - kwlnTexturedSquareStartAlpha) /
                kwlnTexturedSquareFadeDuration;
        D_003C25F0.color.rgba = (D_003C25F0.color.rgba & 0xFFFFFF) | (alpha << 24);
        effCopyTexturedSquareParameters(&D_003C25F0);
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

/* vu0 routine: shortest-arc quaternion rotating direction vf10 to vf11. */
void func_00107DE8(void) {
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
