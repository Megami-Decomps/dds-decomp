#include "common.h"
#include "gs_packet.h"
#include "pcp_vu0.h"

extern u32 kwlnDrawControlFlags;
extern u8 kwlnDrawOverlayEnabled;
extern u16 kwlnDrawOverlayAlpha;
extern u16 kwlnDrawOverlayScale;
extern u16 D_003BD6B4;
extern u16 D_003BD6B6;
extern u16 D_003BD6B8;
extern u16 D_003BD6BA;
extern u16 D_003BD6BC;
extern u16 D_003BD6BE;

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

extern KwlnSolidRectParams D_003C2DC8;
extern KwlnResourceRectParams D_003C2E08;
extern u32 D_003C2E14;
extern u32 D_003C2E18;
extern KwlnBlurRectParams D_003C2D88;
extern KwlnPixelBlurParams D_003C2C70;
extern KwlnScatterBlurParams D_003C2CD0;
extern KwlnScaleBlurParams D_003C2D30;
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
extern u16 D_003BD74C;
extern u16 D_003BD748;
extern u16 D_003BD74E;
extern u16 D_003BD74A;
extern u16 D_003BD740;
extern u16 D_003BD73C;
extern u16 D_003BD742;
extern u16 D_003BD73E;
extern u16 D_003BD71C;
extern u16 D_003BD718;
extern u16 D_003BD71E;
extern u16 D_003BD71A;
extern u16 D_003BD728;
extern u16 D_003BD724;
extern u16 D_003BD72A;
extern u16 D_003BD72C;
extern u16 D_003BD72E;
extern u16 D_003BD726;
extern u16 D_003BD736;
extern u16 D_003BD738;
extern u16 D_003BD73A;
extern u16 D_003BD734;
extern u16 D_003BD730;
extern u16 D_003BD732;
extern u16 D_003BD75A;
extern u16 D_003BD758;
extern u16 D_003BD754;
extern u16 D_003BD756;
extern u32 kwlnDistanceBlurErrorCount;
extern u32 kwlnRippleBlurErrorCount;
extern void func_0018F6F0(void *);
extern void func_0018F6D8(void);
extern void func_0018F3B8(void *);
extern void func_0018F3A0(void);
extern void func_0018F6E8(void);
extern void func_0018F750(void);
extern void func_0018F3B0(void);
extern void effCopyCh71Common(void *);
extern u32 effGetCh72Work(void);
extern void func_0018F428(void);
extern void effCopyCh72Common(void *);
extern void func_0018F4E0(void);
extern void func_0018F4F0(void);
extern void func_0018F5A8(void);
extern void effCopyCh75Common(void *);
extern void func_0018F740(void);
extern void func_0018F438(void);
extern void effCopyCh76Common(void *);
extern u32 effGetCh76Work(void);
extern void func_0018F598(void);

void kwlnDrawCopyRow128(void *src) {
    PCP_COPY_VECTOR(&kwlnDefaultColorVector, src);
}

void kwlnDrawCopyWords20(KwlnDrawVectorParams *src) {
    memcpy(&kwlnDrawVector, src, 0x14);
}

void dds3DrawSetIndexedWord(u32 value, s32 index) {
    D_00324770[index] = value;
}

/* Default draw viewport is 512 by 448 pixels. */
void kwlnDrawInitRect(KwlnRectBounds *rect) {
    rect->right = DRAW_VIEWPORT_WIDTH;
    rect->bottom = DRAW_VIEWPORT_HEIGHT;
    rect->top = 0;
    rect->left = 0;
}

void kwlnDrawSetDc8Second(u32 value) {
    D_003C2DC8.blendControl = value;
}

void kwlnDrawSetDc8First(u32 value) {
    D_003C2DC8.color.rgba = value;
}

extern KwlnSolidRectParams *effGetCh74Params(void);
extern u16 D_003BD750;
extern u16 D_003BD752;

extern KwlnSolidRectParams D_003C2DB0;
/* Alias of D_003C2DC8.bounds; the effect setter needs the preceding header. */
extern KwlnRectBounds D_003C2DD0;

void func_00106540(s32 mode) {
    D_003C2DB0 = *effGetCh74Params();
    D_003BD750 = 0;
    D_003BD752 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x100000;
        kwlnDrawInitRect(&D_003C2DD0);
        func_0018F6F0((void *)((u8 *)&D_003C2DD0 - 8));
    } else {
        kwlnDrawControlFlags |= 0x100000;
    }
}

void kwlnDrawSetupDc8(s32 mode) {
    KwlnSolidRectParams *blk = &D_003C2DC8;
    s32 requestedMode = mode;

    D_003BD74C = 0;
    D_003BD748 = 0;
    D_003BD74E = blk->color.channels[3];
    D_003BD74A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawInitRect(&blk->bounds);
        func_0018F6F0(blk);
        func_0018F6D8();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawEnableDc8(s32 enabled) {
    D_003BD74E = 0;
    D_003BD74C = D_003C2DC8.color.channels[3];
    D_003BD748 = 0;
    D_003BD74A = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawControlFlags &= ~0x100000;
        func_0018F6E8();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawSetE08Fifth(u32 value) {
    D_003C2E08.blendControl = value;
}

void kwlnDrawSetE08Fourth(u32 value) {
    D_003C2E08.color.rgba = value;
}

void kwlnDrawSetE08Triple(u32 first, u32 second, u32 third) {
    D_003C2E08.extent = first;
    D_003C2E08.centerX = second;
    D_003C2E08.centerY = third;
}

extern KwlnResourceRectParams D_003C2DE0;
extern u16 D_003BD75C;
extern u16 D_003BD75E;
extern u32 effGetCh75Work(void);

void kwlnDrawApplyEffectWord(s32 mode) {
    D_003C2DE0 = *(KwlnResourceRectParams *)effGetCh75Work();
    D_003BD75C = 0;
    D_003BD75E = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFBFFFFF;
        effCopyCh75Common(&D_003C2E08);
    } else {
        kwlnDrawControlFlags |= 0x400000;
    }
}

void kwlnDrawSetupE08(s32 mode) {
    KwlnResourceRectParams *blk = &D_003C2E08;
    s32 requestedMode = mode;

    D_003BD758 = 0;
    D_003BD754 = 0;
    D_003BD75A = blk->color.channels[3];
    D_003BD756 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        effCopyCh75Common(blk);
        func_0018F740();
    }
    else {
        kwlnDrawControlFlags |= 0x200000;
    }
}

void kwlnDrawEnableE08(s32 enabled) {
    D_003BD75A = 0;
    D_003BD758 = D_003C2E08.color.channels[3];
    D_003BD754 = 0;
    D_003BD756 = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        kwlnDrawControlFlags &= ~0x400000;
        func_0018F750();
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
    D_003BD6B8 = kwlnDrawOverlayAlpha;
    D_003BD6BA = kwlnDrawOverlayScale;
    D_003BD6BC = (s16)x;
    D_003BD6BE = (s16)y;
    D_003BD6B6 = (s16)transition;
    kwlnDrawControlFlags = kwlnDrawControlFlags | 0x800;
    D_003BD6B4 = 0;
}

void kwlnDrawSetD88FloatTriple(u32 value, f32 first, f32 second) {
    D_003C2D88.rotation = first;
    D_003C2D88.scale = second;
    D_003C2D88.blendControl = value;
}

void kwlnDrawSetD88First(u32 value) {
    D_003C2D88.color.rgba = value;
}

void kwlnDrawSetD88Pair(u32 first, u32 second) {
    D_003C2D88.centerX = first;
    D_003C2D88.centerY = second;
}

extern KwlnBlurRectParams D_003C2D60;
extern u16 D_003BD744;
extern u16 D_003BD746;
extern u32 effGetCh70Params(void);

void func_001069A8(s32 mode) {
    D_003C2D60 = *(KwlnBlurRectParams *)effGetCh70Params();
    D_003BD744 = 0;
    D_003BD746 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFBFFFF;
        kwlnDrawInitRect(&D_003C2D88.bounds);
        func_0018F3B8(&D_003C2D88);
    } else {
        kwlnDrawControlFlags |= 0x40000;
    }
}

void kwlnDrawSetupD88(s32 mode) {
    KwlnBlurRectParams *blk = &D_003C2D88;
    s32 requestedMode = mode;

    D_003BD740 = 0;
    D_003BD73C = 0;
    D_003BD742 = blk->color.channels[3];
    D_003BD73E = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawInitRect(&blk->bounds);
        func_0018F3B8(blk);
        func_0018F3A0();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawEnableD88(s32 enabled) {
    D_003BD742 = 0;
    D_003BD740 = D_003C2D88.color.channels[3];
    D_003BD73C = 0;
    D_003BD73E = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawControlFlags &= ~0x40000;
        func_0018F3B0();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawSetC70FloatTriple(u32 value, f32 first, f32 second) {
    D_003C2C70.source.rotation = first;
    D_003C2C70.source.scale = second;
    D_003C2C70.source.blendControl = value;
}

void kwlnDrawSetC70Second(u32 value) {
    D_003C2C70.source.color.rgba = value;
}

void kwlnDrawSetC70Triple(u32 first, u32 second, u32 third) {
    D_003C2C70.extent = first;
    D_003C2C70.source.centerX = second;
    D_003C2C70.source.centerY = third;
}

extern KwlnPixelBlurParams D_003C2C40;
extern u16 D_003BD720;
extern u16 D_003BD722;
extern u32 effGetCh71Work(void);

void kwlnDrawApplyEffectBlock(s32 mode) {
    D_003C2C40 = *(KwlnPixelBlurParams *)effGetCh71Work();
    D_003BD720 = 0;
    D_003BD722 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFF7FFF;
        effCopyCh71Common(&D_003C2C70);
    } else {
        kwlnDrawControlFlags |= 0x8000;
    }
}

void kwlnDrawSetupC70(s32 mode) {
    KwlnPixelBlurParams *blk = &D_003C2C70;
    s32 requestedMode = mode;

    D_003BD71C = 0;
    D_003BD718 = 0;
    D_003BD71E = blk->source.color.channels[3];
    D_003BD71A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        effCopyCh71Common(blk);
        func_0018F428();
    }
    else {
        kwlnDrawControlFlags |= 0x1000;
    }
}

void kwlnDrawSetupC70B(s32 mode) {
    KwlnPixelBlurParams *blk = &D_003C2C70;
    s32 requestedMode = mode;

    D_003BD71E = 0;
    D_003BD718 = 0;
    D_003BD71C = blk->source.color.channels[3];
    D_003BD71A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        kwlnDrawControlFlags &= ~0x8000;
        effCopyCh71Common(blk);
        func_0018F438();
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
    D_003C2CD0.count = boundedValue;
    D_003C2CD0.size = lastWord;
    D_003C2CD0.delaySpread = secondWord;
    D_003C2CD0.angleStep = firstFloat;
    D_003C2CD0.unk14 = secondFloat;
    D_003C2CD0.unk18 = thirdFloat;
    D_003C2CD0.unk10 = fourthWord;
}

void kwlnDrawSetCd0Fourth(u32 value) {
    D_003C2CD0.color.rgba = value;
}

void kwlnDrawSetCd0Triple(u32 first, u32 second, u32 third) {
    D_003C2CD0.positionSpread = first;
    D_003C2CD0.x = second;
    D_003C2CD0.y = third;
}

void func_00106DF0(s32 mode) {
    D_003C2CA0 = *(KwlnScatterBlurParams *)effGetCh72Work();
    D_003BD72C = 0;
    D_003BD72E = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFEFFFF;
        effCopyCh72Common(&D_003C2CD0);
    } else {
        kwlnDrawControlFlags |= 0x10000;
    }
}

void kwlnDrawSetupCd0(s32 mode) {
    KwlnScatterBlurParams *blk = &D_003C2CD0;
    s32 requestedMode = mode;

    D_003BD728 = 0;
    D_003BD724 = 0;
    D_003BD72A = blk->color.channels[3];
    D_003BD726 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        effCopyCh72Common(blk);
        func_0018F4E0();
    }
    else {
        kwlnDrawControlFlags |= 0x2000;
        effCopyCh72Common(blk);
        func_0018F4F0();
    }
}

void kwlnDrawEnableCd0(s32 enabled) {
    D_003BD72A = 0;
    D_003BD728 = D_003C2CD0.color.channels[3];
    D_003BD724 = 0;
    D_003BD726 = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        kwlnDrawControlFlags &= ~0x10000;
        func_0018F4F0();
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
    D_003C2D30.count = boundedValue;
    D_003C2D30.phaseStep = firstFloat;
    D_003C2D30.spacing = secondFloat;
    D_003C2D30.unk14 = thirdFloat;
    D_003C2D30.unk18 = fourthFloat;
    D_003C2D30.angleStep = fifthFloat;
    D_003C2D30.unk10 = fourthWord;
}

void kwlnDrawSetD30Fourth(u32 value) {
    D_003C2D30.color.rgba = value;
}

void kwlnDrawSetD30Triple(u32 first, u32 second, u32 third) {
    D_003C2D30.size = first;
    D_003C2D30.x = second;
    D_003C2D30.y = third;
}

void func_00107018(s32 mode) {
    D_003C2D00 = *(KwlnScaleBlurParams *)effGetCh76Work();
    D_003BD738 = 0;
    D_003BD73A = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFEFFFFFF;
        effCopyCh76Common(&D_003C2D30);
    } else {
        kwlnDrawControlFlags |= 0x1000000;
    }
}

void kwlnDrawSetupD30(s32 mode) {
    KwlnScaleBlurParams *blk = &D_003C2D30;
    s32 requestedMode = mode;

    D_003BD734 = 0;
    D_003BD730 = 0;
    D_003BD736 = blk->color.channels[3];
    D_003BD732 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        effCopyCh76Common(blk);
        func_0018F598();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
        effCopyCh76Common(blk);
        func_0018F5A8();
    }
}

void kwlnDrawEnableD30(s32 enabled) {
    D_003BD736 = 0;
    D_003BD734 = D_003C2D30.color.channels[3];
    D_003BD730 = 0;
    D_003BD732 = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        kwlnDrawControlFlags &= ~0x1000000;
        func_0018F5A8();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001071E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107DE8);
