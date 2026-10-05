#include "common.h"
#include "eff.h"


INCLUDE_ASM(const s32, "game/code_002649B0", func_002649B0);


extern void mnuDrawItemPanelBackdrop(s32);
extern void func_00263B78(s32, s32);

/* The dispatcher consumes the inline work area and its adjacent status word. */
typedef struct {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus;     /* 0x54 */
} PanelDispatchContext;

s32 itfRunPanelMode1(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    PanelDispatchContext *panel = (PanelDispatchContext *)context;

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return func_00285670((s32)panel->dispatchWork, &panel->dispatchStatus, 1, request);
}

extern s32 func_00285670(s32, s32 *, u64, u64);
extern s32 kwlnTaskGetUserValue();

extern void func_0024DC98(s32);

s32 itfRunPanelMode2(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    PanelDispatchContext *panel = (PanelDispatchContext *)context;

    func_0024DC98(0);
    return func_00285670((s32)panel->dispatchWork, &panel->dispatchStatus, 2, request);
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_00264B08);
INCLUDE_ASM(const s32, "game/code_002649B0", func_00264D90);

extern u32 uiBlendColors(u32, u32, s32);

/* The opacity update latches at its threshold. Keep fadeProgress signed:
 * the decay path converts it through a signed float. */
typedef struct TitleFadeWork {
    u8 pad00[0xD3C];
    s8 opacityReady; /* 0xD3C */
    u8 padD3D[7];
    s32 spriteResource;
    u32 opacity;      /* 0xD48 */
    u8 padD4C[0x828];
    s32 fadeProgress; /* 0x1574 */
} TitleFadeWork;

void itfUpdateFadeColor(TitleFadeWork *work) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->opacityReady == 0) {
        u32 opacity = uiBlendColors(0x80808080, 0x80808000, remaining) & 0xFF;

        work->opacity = opacity;
        if (opacity >= 0x80) {
            work->opacityReady = 1;
        }
    }
}

extern void func_002BF438(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB20);

void mnuDrawTitleFadeSprites(TitleFadeWork *work) {
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 positions[8][3] = {
        {-5, -5, 0x23},
        {0x12, 6, 0x22},
        {0x5E, 0x3B, 0x21},
        {0x57, 0x16, 0x20},
        {0x87, 0x38, 0x19},
        {0x10B, 0x16, 0x1F},
        {0xC8, 0x6F, 0x10},
        {0x157, 0x6F, 0x11}
    };
    s32 i;

    if (work->spriteResource) {
        for (i = 0; i < 8; i++) {
            u32 color = work->opacity | 0x80808000;

            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            func_002BF438(positions[i][0] << 4, positions[i][1] << 3, 0,
                         colors, 0, (EffectSlotSet *)work->spriteResource, positions[i][2], 0x53);
        }
    }
}

void func_00265078(void) {
}

void func_00265080(void) {
}

/* Decay only the fade progress; opacity stops updating once it crosses 0x80. */
void brsStepAnimDecay(TitleFadeWork *work) {
    work->fadeProgress = (s32)((f32)work->fadeProgress / 1.2f);
}


u32 mnuGetTitleState(TitleFadeWork *work) {
    return work->fadeProgress;
}

void mnuClearTitleState(TitleFadeWork *work) {
    work->fadeProgress = 0;
}

void func_002650C0(void *work) {
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_002650C8);

extern char D_003BC568[];
extern void func_003014F0(char *, char *, s32);
extern u32 func_001979C8(s32, s32, s32, s32, char *, s32);
extern s32 frFontMeasureLines(u32);
extern void frFontSetContextPair(u32, s32, s32);
struct FrFontGlyph;
extern s32 func_001958A0(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);

void itfDrawCountText(s32 x, s32 y, s32 z, s32 w, u8 *info, s32 color) {
    char text[32];
    u32 handle;

    func_003014F0(text, D_003BC568, *(s32 *)(info + 0x10));
    handle = func_001979C8(x, y, z, w, text, 0);
    frFontSetContextPair(handle, x + ((0xBE - frFontMeasureLines(handle)) << 4), y);
    func_001958A0((struct FrFontGlyph *)handle, 1, color);
    frFontQueueGlyphInSelectedSlot((struct FrFontGlyph *)handle);
}

void mnuQueueRightAlignedFormattedInfoText(s32 x, s32 y, s32 z, s32 w, u8 *info, s32 color) {
    char text[32];
    u32 handle;

    func_003014F0(text, D_003BC568, *(s32 *)(info + 0xC));
    handle = func_001979C8(x, y, z, w, text, 0);
    frFontSetContextPair(handle, x + ((0xBE - frFontMeasureLines(handle)) << 4), y);
    func_001958A0((struct FrFontGlyph *)handle, 1, color);
    frFontQueueGlyphInSelectedSlot((struct FrFontGlyph *)handle);
}

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC560);

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC568);

