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

extern KwlnResourceRectParams D_0043E588;

extern KwlnPixelBlurParams D_0043E3F0;

extern KwlnScatterBlurParams D_0043E450;

extern u32 kwlnDistanceBlurErrorCount;

extern KwlnScaleBlurParams D_0043E4B0;

extern u32 kwlnRippleBlurErrorCount;

extern u16 D_00438E5A;

extern u16 D_00438E58;

extern u16 D_00438E54;

extern u16 D_00438E56;

extern void effCopyCh75Common(void *);

extern void func_00197378(void);

extern void func_00197388(void);

extern u16 D_00438E1C;

extern u16 D_00438E18;

extern u16 D_00438E1E;

extern u16 D_00438E1A;

extern void effCopyCh71Common(void *);

extern void func_00197060(void);

extern void func_00197070(void);

extern u16 D_00438E28;

extern u16 D_00438E24;

extern u16 D_00438E2A;

extern u16 D_00438E26;

extern void effCopyCh72Common(void *);

extern void func_00197118(void);

extern void func_00197128(void);

extern u16 D_00438E36;

extern u16 D_00438E34;

extern u16 D_00438E30;

extern u16 D_00438E32;

extern void func_001971E0(void);

extern void effCopyCh76Common(void *);

extern void func_001971D0(void);

extern KwlnSolidRectParams D_0043E548;

extern u16 D_00438E4C;

extern u16 D_00438E48;

extern u16 D_00438E4E;

extern u16 D_00438E4A;

extern void func_00197328(void *);

extern void func_00197310(void);

extern void func_00197320(void);

extern KwlnBlurRectParams D_0043E508;

extern u16 D_00438E40;

extern u16 D_00438E3C;

extern u16 D_00438E42;

extern u16 D_00438E3E;

extern void func_00196FF0(void *);

extern void func_00196FD8(void);

extern void func_00196FE8(void);

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
    D_0043E548.blendControl = value;
}

void kwlnDrawSetDc8First(u32 value) {
    D_0043E548.color.rgba = value;
}

extern KwlnSolidRectParams *effGetCh74Params(void);
extern u16 D_00438E50;
extern u16 D_00438E52;

extern KwlnSolidRectParams D_0043E530;
/* Alias of D_0043E548.bounds; the effect setter needs the preceding header. */
extern KwlnRectBounds D_0043E550;

void func_00106460(s32 mode) {
    D_0043E530 = *effGetCh74Params();
    D_00438E50 = 0;
    D_00438E52 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x100000;
        kwlnDrawInitRect(&D_0043E550);
        func_00197328((void *)((u8 *)&D_0043E550 - 8));
    } else {
        kwlnDrawControlFlags |= 0x100000;
    }
}

void kwlnDrawSetupDc8(s32 mode) {
    KwlnSolidRectParams *blk = &D_0043E548;
    s32 requestedMode = mode;

    D_00438E4C = 0;
    D_00438E48 = 0;
    D_00438E4E = blk->color.channels[3];
    D_00438E4A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawInitRect(&blk->bounds);
        func_00197328(blk);
        func_00197310();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawEnableDc8(s32 enabled) {
    D_00438E4E = 0;
    D_00438E4C = D_0043E548.color.channels[3];
    D_00438E48 = 0;
    D_00438E4A = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x80000;
        kwlnDrawControlFlags &= ~0x100000;
        func_00197320();
    }
    else {
        kwlnDrawControlFlags |= 0x80000;
    }
}

void kwlnDrawSetE08Fifth(u32 value) {
    D_0043E588.blendControl = value;
}

void kwlnDrawSetE08Fourth(u32 value) {
    D_0043E588.color.rgba = value;
}

void kwlnDrawSetE08Triple(u32 first, u32 second, u32 third) {
    D_0043E588.extent = first;
    D_0043E588.centerX = second;
    D_0043E588.centerY = third;
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
        effCopyCh75Common(&D_0043E588);
    } else {
        kwlnDrawControlFlags |= 0x400000;
    }
}

void kwlnDrawSetupE08(s32 mode) {
    KwlnResourceRectParams *blk = &D_0043E588;
    s32 requestedMode = mode;

    D_00438E58 = 0;
    D_00438E54 = 0;
    D_00438E5A = blk->color.channels[3];
    D_00438E56 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        effCopyCh75Common(blk);
        func_00197378();
    }
    else {
        kwlnDrawControlFlags |= 0x200000;
    }
}

void kwlnDrawEnableE08(s32 mode) {
    D_00438E5A = 0;
    D_00438E58 = D_0043E588.color.channels[3];
    D_00438E54 = 0;
    D_00438E56 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x200000;
        kwlnDrawControlFlags &= ~0x400000;
        func_00197388();
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
    D_0043E508.rotation = first;
    D_0043E508.scale = second;
    D_0043E508.blendControl = value;
}

void kwlnDrawSetD88First(u32 value) {
    D_0043E508.color.rgba = value;
}

void kwlnDrawSetD88Pair(u32 first, u32 second) {
    D_0043E508.centerX = first;
    D_0043E508.centerY = second;
}

extern KwlnBlurRectParams D_0043E4E0;
extern u16 D_00438E44;
extern u16 D_00438E46;
extern u32 effGetCh70Params(void);

void func_001068C8(s32 mode) {
    D_0043E4E0 = *(KwlnBlurRectParams *)effGetCh70Params();
    D_00438E44 = 0;
    D_00438E46 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFBFFFF;
        kwlnDrawInitRect(&D_0043E508.bounds);
        func_00196FF0(&D_0043E508);
    } else {
        kwlnDrawControlFlags |= 0x40000;
    }
}

void kwlnDrawSetupD88(s32 mode) {
    KwlnBlurRectParams *blk = &D_0043E508;
    s32 requestedMode = mode;

    D_00438E40 = 0;
    D_00438E3C = 0;
    D_00438E42 = blk->color.channels[3];
    D_00438E3E = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawInitRect(&blk->bounds);
        func_00196FF0(blk);
        func_00196FD8();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawEnableD88(s32 enabled) {
    D_00438E42 = 0;
    D_00438E40 = D_0043E508.color.channels[3];
    D_00438E3C = 0;
    D_00438E3E = enabled;
    if (enabled == 0) {
        kwlnDrawControlFlags &= ~0x20000;
        kwlnDrawControlFlags &= ~0x40000;
        func_00196FE8();
    }
    else {
        kwlnDrawControlFlags |= 0x20000;
    }
}

void kwlnDrawSetC70FloatTriple(u32 value, f32 first, f32 second) {
    D_0043E3F0.source.rotation = first;
    D_0043E3F0.source.scale = second;
    D_0043E3F0.source.blendControl = value;
}

void kwlnDrawSetC70Second(u32 value) {
    D_0043E3F0.source.color.rgba = value;
}

void kwlnDrawSetC70Triple(u32 first, u32 second, u32 third) {
    D_0043E3F0.extent = first;
    D_0043E3F0.source.centerX = second;
    D_0043E3F0.source.centerY = third;
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
        effCopyCh71Common(&D_0043E3F0);
    } else {
        kwlnDrawControlFlags |= 0x8000;
    }
}

void kwlnDrawSetupC70(s32 mode) {
    KwlnPixelBlurParams *blk = &D_0043E3F0;
    s32 requestedMode = mode;

    D_00438E1C = 0;
    D_00438E18 = 0;
    D_00438E1E = blk->source.color.channels[3];
    D_00438E1A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        effCopyCh71Common(blk);
        func_00197060();
    }
    else {
        kwlnDrawControlFlags |= 0x1000;
    }
}

void kwlnDrawSetupC70B(s32 mode) {
    KwlnPixelBlurParams *blk = &D_0043E3F0;
    s32 requestedMode = mode;

    D_00438E1E = 0;
    D_00438E18 = 0;
    D_00438E1C = blk->source.color.channels[3];
    D_00438E1A = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x1000;
        kwlnDrawControlFlags &= ~0x8000;
        effCopyCh71Common(blk);
        func_00197070();
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
    D_0043E450.count = boundedValue;
    D_0043E450.size = lastWord;
    D_0043E450.delaySpread = secondWord;
    D_0043E450.angleStep = firstFloat;
    D_0043E450.unk14 = secondFloat;
    D_0043E450.unk18 = thirdFloat;
    D_0043E450.unk10 = fourthWord;
}

void kwlnDrawSetCd0Fourth(u32 value) {
    D_0043E450.color.rgba = value;
}

void kwlnDrawSetCd0Triple(u32 first, u32 second, u32 third) {
    D_0043E450.positionSpread = first;
    D_0043E450.x = second;
    D_0043E450.y = third;
}

extern KwlnScatterBlurParams D_0043E420;
extern u32 effGetCh72Work(void);
extern s16 D_00438E2C;
extern s16 D_00438E2E;

void func_00106D10(s32 mode) {
    D_0043E420 = *(KwlnScatterBlurParams *)effGetCh72Work();
    D_00438E2C = 0;
    D_00438E2E = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFFFEFFFF;
        effCopyCh72Common(&D_0043E450);
    } else {
        kwlnDrawControlFlags |= 0x10000;
    }
}

void kwlnDrawSetupCd0(s32 mode) {
    KwlnScatterBlurParams *blk = &D_0043E450;
    s32 requestedMode = mode;

    D_00438E28 = 0;
    D_00438E24 = 0;
    D_00438E2A = blk->color.channels[3];
    D_00438E26 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        effCopyCh72Common(blk);
        func_00197118();
    }
    else {
        kwlnDrawControlFlags |= 0x2000;
        effCopyCh72Common(blk);
        func_00197128();
    }
}

void kwlnDrawEnableCd0(s32 mode) {
    D_00438E2A = 0;
    D_00438E28 = D_0043E450.color.channels[3];
    D_00438E24 = 0;
    D_00438E26 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x2000;
        kwlnDrawControlFlags &= ~0x10000;
        func_00197128();
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
    D_0043E4B0.count = boundedValue;
    D_0043E4B0.phaseStep = firstFloat;
    D_0043E4B0.spacing = secondFloat;
    D_0043E4B0.unk14 = thirdFloat;
    D_0043E4B0.unk18 = fourthFloat;
    D_0043E4B0.angleStep = fifthFloat;
    D_0043E4B0.unk10 = fourthWord;
}

void kwlnDrawSetD30Fourth(u32 value) {
    D_0043E4B0.color.rgba = value;
}

void kwlnDrawSetD30Triple(u32 first, u32 second, u32 third) {
    D_0043E4B0.size = first;
    D_0043E4B0.x = second;
    D_0043E4B0.y = third;
}

extern KwlnScaleBlurParams D_0043E480;
extern u32 effGetCh76Work(void);
extern s16 D_00438E38;
extern s16 D_00438E3A;

void func_00106F38(s32 mode) {
    D_0043E480 = *(KwlnScaleBlurParams *)effGetCh76Work();
    D_00438E38 = 0;
    D_00438E3A = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= 0xFEFFFFFF;
        effCopyCh76Common(&D_0043E4B0);
    } else {
        kwlnDrawControlFlags |= 0x1000000;
    }
}

void kwlnDrawSetupD30(s32 mode) {
    KwlnScaleBlurParams *blk = &D_0043E4B0;
    s32 requestedMode = mode;

    D_00438E34 = 0;
    D_00438E30 = 0;
    D_00438E36 = blk->color.channels[3];
    D_00438E32 = requestedMode;
    if (requestedMode == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        effCopyCh76Common(blk);
        func_001971D0();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
        effCopyCh76Common(blk);
        func_001971E0();
    }
}

void kwlnDrawEnableD30(s32 mode) {
    D_00438E36 = 0;
    D_00438E34 = D_0043E4B0.color.channels[3];
    D_00438E30 = 0;
    D_00438E32 = mode;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x800000;
        kwlnDrawControlFlags &= ~0x1000000;
        func_001971E0();
    }
    else {
        kwlnDrawControlFlags |= 0x800000;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107108);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00107D08);
