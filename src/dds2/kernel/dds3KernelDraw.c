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

void kwlnDrawCopyRow128(u128 *row) {
    PCP_COPY_VECTOR(&kwlnDefaultColorVector, row);
}

void kwlnDrawCopyWords20(KwlnDrawVectorParams *src) {
    memcpy(&kwlnDrawVector, src, 0x14);
}

void dds3DrawSetIndexedWord(u32 value, s32 index) {
    D_0037F770[index] = value;
}

/* Default draw viewport is 512 by 448 pixels. */
void kwlnDrawInitRect(KwlnRectBounds *rect) {
    rect->right = DRAW_VIEWPORT_WIDTH;
    rect->bottom = DRAW_VIEWPORT_HEIGHT;
    rect->top = 0;
    rect->left = 0;
}

void kwlnDrawSetDc8Second(u32 value) {
    kwlnColorRectangleParameters.blendControl = value;
}

void kwlnDrawSetDc8First(u32 value) {
    kwlnColorRectangleParameters.color.rgba = value;
}

extern KwlnSolidRectParams *effGetCh74Params(void);
extern u16 D_00438E50;
extern u16 D_00438E52;

extern KwlnSolidRectParams D_0043E530;
/* Alias of kwlnColorRectangleParameters.bounds; the effect setter needs the preceding header. */
extern KwlnRectBounds D_0043E550;

void kwlnDrawSnapshotSolidRect(s32 mode) {
    D_0043E530 = *effGetCh74Params();
    D_00438E50 = 0;
    D_00438E52 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x100000;
        kwlnDrawInitRect(&D_0043E550);
        effCopyColorRectangleParameters((void *)((u8 *)&D_0043E550 - 8));
    } else {
        kwlnDrawControlFlags |= 0x100000;
    }
}

void kwlnDrawSetupDc8(s32 mode) {
    KwlnSolidRectParams *blk = &kwlnColorRectangleParameters;
    s32 requestedMode = mode;

    kwlnColorRectangleStartAlpha = 0;
    kwlnColorRectangleFadeCounter = 0;
    kwlnColorRectangleTargetAlpha = blk->color.channels[3];
    kwlnColorRectangleFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawInitRect(&blk->bounds);
        effCopyColorRectangleParameters(blk);
        effEnableColorRectangle();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawEnableDc8(s32 enabled) {
    kwlnColorRectangleTargetAlpha = 0;
    kwlnColorRectangleStartAlpha = kwlnColorRectangleParameters.color.channels[3];
    kwlnColorRectangleFadeCounter = 0;
    kwlnColorRectangleFadeDuration = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawControlFlags &= ~0x100000;
        effDisableColorRectangle();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawSetE08Fifth(u32 value) {
    kwlnTexturedSquareParameters.blendControl = value;
}

void kwlnDrawSetE08Fourth(u32 value) {
    kwlnTexturedSquareParameters.color.rgba = value;
}

void kwlnDrawSetE08Triple(u32 first, u32 second, u32 third) {
    kwlnTexturedSquareParameters.extent = first;
    kwlnTexturedSquareParameters.centerX = second;
    kwlnTexturedSquareParameters.centerY = third;
}

extern KwlnResourceRectParams D_0043E560;
extern u16 D_00438E5C;
extern u16 D_00438E5E;
extern u32 effGetCh75Work(void);

void kwlnDrawApplyEffectWord(s32 mode) {
    D_0043E560 = *(KwlnResourceRectParams *)effGetCh75Work();
    D_00438E5C = 0;
    D_00438E5E = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFBFFFFF;
        effCopyCh75Common(&kwlnTexturedSquareParameters);
    } else {
        kwlnDrawControlFlags |= 0x400000;
    }
}

void kwlnDrawSetupE08(s32 mode) {
    KwlnResourceRectParams *blk = &kwlnTexturedSquareParameters;
    s32 requestedMode = mode;

    kwlnTexturedSquareStartAlpha = 0;
    kwlnTexturedSquareFadeCounter = 0;
    kwlnTexturedSquareTargetAlpha = blk->color.channels[3];
    kwlnTexturedSquareFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        effCopyCh75Common(blk);
        effEnableTexturedSquare();
    }
    else {
        kwlnDrawControlFlags |= 0x200000;
    }
}

void kwlnDrawEnableE08(s32 mode) {
    kwlnTexturedSquareTargetAlpha = 0;
    kwlnTexturedSquareStartAlpha = kwlnTexturedSquareParameters.color.channels[3];
    kwlnTexturedSquareFadeCounter = 0;
    kwlnTexturedSquareFadeDuration = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        kwlnDrawControlFlags &= ~0x400000;
        effDisableTexturedSquare();
    }
    else {
        kwlnDrawControlFlags |= 0x200000;
    }
}

/* Apply an offset immediately, or stage an interpolated move from the old offset. */
void kwlnDrawSetOffsetTransition(s32 transition, s32 x, s32 y) {
    if (transition == 0) {
        kwlnDrawOverlayAlpha = (s16)x;
        kwlnDrawOverlayScale = (s16)y;
        if ((x == 0) && (y == 0)) {
            kwlnDrawOverlayEnabled = 0;
        }
        else {
            kwlnDrawOverlayEnabled = 1;
        }
        kwlnDrawControlFlags = kwlnDrawControlFlags & 0xfffff7ff;
        return;
    }
    D_00438DB8 = kwlnDrawOverlayAlpha;
    D_00438DBA = kwlnDrawOverlayScale;
    D_00438DBC = (s16)x;
    D_00438DBE = (s16)y;
    D_00438DB6 = (s16)transition;
    kwlnDrawControlFlags = kwlnDrawControlFlags | 0x800;
    D_00438DB4 = 0;
}

void kwlnDrawSetD88FloatTriple(u32 value, f32 first, f32 second) {
    kwlnRectangleBlurParameters.rotation = first;
    kwlnRectangleBlurParameters.scale = second;
    kwlnRectangleBlurParameters.blendControl = value;
}

void kwlnDrawSetD88First(u32 value) {
    kwlnRectangleBlurParameters.color.rgba = value;
}

void kwlnDrawSetD88Pair(u32 first, u32 second) {
    kwlnRectangleBlurParameters.centerX = first;
    kwlnRectangleBlurParameters.centerY = second;
}

extern KwlnBlurRectParams D_0043E4E0;
extern u16 D_00438E44;
extern u16 D_00438E46;
extern u32 effGetCh70Params(void);

void kwlnSetRectangleBlurParameterTransition(s32 mode) {
    D_0043E4E0 = *(KwlnBlurRectParams *)effGetCh70Params();
    D_00438E44 = 0;
    D_00438E46 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFBFFFF;
        kwlnDrawInitRect(&kwlnRectangleBlurParameters.bounds);
        effCopyRectangleBlurParameters(&kwlnRectangleBlurParameters);
    } else {
        kwlnDrawControlFlags |= 0x40000;
    }
}

void kwlnDrawSetupD88(s32 mode) {
    KwlnBlurRectParams *blk = &kwlnRectangleBlurParameters;
    s32 requestedMode = mode;

    kwlnRectangleBlurStartAlpha = 0;
    kwlnRectangleBlurFadeCounter = 0;
    kwlnRectangleBlurTargetAlpha = blk->color.channels[3];
    kwlnRectangleBlurFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawInitRect(&blk->bounds);
        effCopyRectangleBlurParameters(blk);
        effEnableRectangleBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawEnableD88(s32 enabled) {
    kwlnRectangleBlurTargetAlpha = 0;
    kwlnRectangleBlurStartAlpha = kwlnRectangleBlurParameters.color.channels[3];
    kwlnRectangleBlurFadeCounter = 0;
    kwlnRectangleBlurFadeDuration = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawControlFlags &= ~0x40000;
        effDisableRectangleBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawSetC70FloatTriple(u32 value, f32 first, f32 second) {
    kwlnTexturedBlurParameters.source.rotation = first;
    kwlnTexturedBlurParameters.source.scale = second;
    kwlnTexturedBlurParameters.source.blendControl = value;
}

void kwlnDrawSetC70Second(u32 value) {
    kwlnTexturedBlurParameters.source.color.rgba = value;
}

void kwlnDrawSetC70Triple(u32 first, u32 second, u32 third) {
    kwlnTexturedBlurParameters.extent = first;
    kwlnTexturedBlurParameters.source.centerX = second;
    kwlnTexturedBlurParameters.source.centerY = third;
}

extern KwlnPixelBlurParams D_0043E3C0;
extern u16 D_00438E20;
extern u16 D_00438E22;
extern u32 effGetCh71Work(void);

void kwlnDrawApplyEffectBlock(s32 mode) {
    D_0043E3C0 = *(KwlnPixelBlurParams *)effGetCh71Work();
    D_00438E20 = 0;
    D_00438E22 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFF7FFF;
        effCopyCh71Common(&kwlnTexturedBlurParameters);
    } else {
        kwlnDrawControlFlags |= 0x8000;
    }
}

void kwlnDrawSetupC70(s32 mode) {
    KwlnPixelBlurParams *blk = &kwlnTexturedBlurParameters;
    s32 requestedMode = mode;

    kwlnTexturedBlurStartAlpha = 0;
    kwlnTexturedBlurFadeCounter = 0;
    kwlnTexturedBlurTargetAlpha = blk->source.color.channels[3];
    kwlnTexturedBlurFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        effCopyCh71Common(blk);
        effEnableTexturedBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x1000;
    }
}

void kwlnDrawSetupC70B(s32 mode) {
    KwlnPixelBlurParams *blk = &kwlnTexturedBlurParameters;
    s32 requestedMode = mode;

    kwlnTexturedBlurTargetAlpha = 0;
    kwlnTexturedBlurFadeCounter = 0;
    kwlnTexturedBlurStartAlpha = blk->source.color.channels[3];
    kwlnTexturedBlurFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        kwlnDrawControlFlags &= ~0x8000;
        effCopyCh71Common(blk);
        effDisableTexturedBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x1000;
    }
}

void kwlnDrawSetCd0Clamped(s32 boundedValue, s32 lastWord, s32 secondWord, s32 fourthWord,
                           f32 firstFloat, f32 secondFloat, f32 thirdFloat) {
    if (boundedValue >= 0x65) {
        kwlnDistanceBlurErrorCount++;
        boundedValue = 0x64;
    }
    kwlnFilterBlurParameters.count = boundedValue;
    kwlnFilterBlurParameters.size = lastWord;
    kwlnFilterBlurParameters.delaySpread = secondWord;
    kwlnFilterBlurParameters.angleStep = firstFloat;
    kwlnFilterBlurParameters.unk14 = secondFloat;
    kwlnFilterBlurParameters.unk18 = thirdFloat;
    kwlnFilterBlurParameters.unk10 = fourthWord;
}

void kwlnDrawSetCd0Fourth(u32 value) {
    kwlnFilterBlurParameters.color.rgba = value;
}

void kwlnDrawSetCd0Triple(u32 first, u32 second, u32 third) {
    kwlnFilterBlurParameters.positionSpread = first;
    kwlnFilterBlurParameters.x = second;
    kwlnFilterBlurParameters.y = third;
}

extern KwlnScatterBlurParams D_0043E420;
extern u32 effGetCh72Work(void);
extern s16 D_00438E2C;
extern s16 D_00438E2E;

void kwlnSetFilterBlurParameterTransition(s32 mode) {
    D_0043E420 = *(KwlnScatterBlurParams *)effGetCh72Work();
    D_00438E2C = 0;
    D_00438E2E = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFEFFFF;
        effCopyCh72Common(&kwlnFilterBlurParameters);
    } else {
        kwlnDrawControlFlags |= 0x10000;
    }
}

void kwlnDrawSetupCd0(s32 mode) {
    KwlnScatterBlurParams *blk = &kwlnFilterBlurParameters;
    s32 requestedMode = mode;

    kwlnFilterBlurStartAlpha = 0;
    kwlnFilterBlurFadeCounter = 0;
    kwlnFilterBlurTargetAlpha = blk->color.channels[3];
    kwlnFilterBlurFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        effCopyCh72Common(blk);
        effEnableFilterBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x2000;
        effCopyCh72Common(blk);
        effDisableFilterBlur();
    }
}

void kwlnDrawEnableCd0(s32 mode) {
    kwlnFilterBlurTargetAlpha = 0;
    kwlnFilterBlurStartAlpha = kwlnFilterBlurParameters.color.channels[3];
    kwlnFilterBlurFadeCounter = 0;
    kwlnFilterBlurFadeDuration = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        kwlnDrawControlFlags &= ~0x10000;
        effDisableFilterBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x2000;
    }
}

void kwlnDrawSetD30Clamped(s32 boundedValue, s32 fourthWord, f32 firstFloat, f32 secondFloat,
                           f32 thirdFloat, f32 fourthFloat, f32 fifthFloat) {
    if (boundedValue >= 0x29) {
        kwlnRippleBlurErrorCount++;
        boundedValue = 0x28;
    }
    kwlnStaggeredBlurParameters.count = boundedValue;
    kwlnStaggeredBlurParameters.phaseStep = firstFloat;
    kwlnStaggeredBlurParameters.spacing = secondFloat;
    kwlnStaggeredBlurParameters.unk14 = thirdFloat;
    kwlnStaggeredBlurParameters.unk18 = fourthFloat;
    kwlnStaggeredBlurParameters.angleStep = fifthFloat;
    kwlnStaggeredBlurParameters.unk10 = fourthWord;
}

void kwlnDrawSetD30Fourth(u32 value) {
    kwlnStaggeredBlurParameters.color.rgba = value;
}

void kwlnDrawSetD30Triple(u32 first, u32 second, u32 third) {
    kwlnStaggeredBlurParameters.size = first;
    kwlnStaggeredBlurParameters.x = second;
    kwlnStaggeredBlurParameters.y = third;
}

extern KwlnScaleBlurParams D_0043E480;
extern u32 effGetCh76Work(void);
extern s16 D_00438E38;
extern s16 D_00438E3A;

void kwlnSetStaggeredBlurParameterTransition(s32 mode) {
    D_0043E480 = *(KwlnScaleBlurParams *)effGetCh76Work();
    D_00438E38 = 0;
    D_00438E3A = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFEFFFFFF;
        effCopyCh76Common(&kwlnStaggeredBlurParameters);
    } else {
        kwlnDrawControlFlags |= 0x1000000;
    }
}

void kwlnDrawSetupD30(s32 mode) {
    KwlnScaleBlurParams *blk = &kwlnStaggeredBlurParameters;
    s32 requestedMode = mode;

    kwlnStaggeredBlurStartAlpha = 0;
    kwlnStaggeredBlurFadeCounter = 0;
    kwlnStaggeredBlurTargetAlpha = blk->color.channels[3];
    kwlnStaggeredBlurFadeDuration = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        effCopyCh76Common(blk);
        effEnableStaggeredBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
        effCopyCh76Common(blk);
        effDisableStaggeredBlur();
    }
}

void kwlnDrawEnableD30(s32 mode) {
    kwlnStaggeredBlurTargetAlpha = 0;
    kwlnStaggeredBlurStartAlpha = kwlnStaggeredBlurParameters.color.channels[3];
    kwlnStaggeredBlurFadeCounter = 0;
    kwlnStaggeredBlurFadeDuration = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        kwlnDrawControlFlags &= ~0x1000000;
        effDisableStaggeredBlur();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
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
