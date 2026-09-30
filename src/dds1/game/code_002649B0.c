#include "common.h"

INCLUDE_ASM(const s32, "game/code_002649B0", func_002649B0);

extern void mnuDrawItemPanelBackdrop(s32);
extern void func_00263B78(s32, s32);

/* The dispatcher consumes the inline work area and its adjacent status word. */
typedef struct {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus;     /* 0x54 */
} PanelDispatchContext;

s64 itfRunPanelMode1(u64 request) {
    s32 context = kwlnTaskGetUserValue();
    PanelDispatchContext *panel = (PanelDispatchContext *)context;

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return func_00285670((s32)panel->dispatchWork, &panel->dispatchStatus, 1, request);
}

extern s32 func_00285670(s32, s32 *, u64, u64);
extern s32 kwlnTaskGetUserValue();

extern void func_0024DC98(s32);

s64 itfRunPanelMode2(u64 request) {
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
typedef struct {
    u8 pad00[0xD3C];
    s8 opacityReady; /* 0xD3C */
    u8 padD3D[0xB];
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

INCLUDE_ASM(const s32, "game/code_002649B0", func_00264EF0);

void func_00265078(void) {
}

void func_00265080(void) {
}

/* Decay only the fade progress; opacity stops updating once it crosses 0x80. */
void brsStepAnimDecay(TitleFadeWork *work) {
    work->fadeProgress = (s32)((f32)work->fadeProgress / 1.2f);
}

typedef struct {
    u8 pad00[0x1574];
    u32 state; /* 0x1574 */
} TitleWork;

u32 mnuGetTitleState(TitleWork *work) {
    return work->state;
}

void mnuClearTitleState(TitleWork *work) {
    work->state = 0;
}

void func_002650C0(void) {
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_002650C8);

extern char D_003BC568[];
extern void func_003014F0(char *, char *, s32);
extern u32 func_001979C8(s32, s32, s32, s32, char *, s32);
extern s32 frFontMeasureLines(u32);
extern void frFontSetContextPair(u32, s32, s32);
extern void func_001958A0(u32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(u32);

void itfDrawCountText(s32 x, s32 y, s32 z, s32 w, u8 *info, s32 color) {
    char text[32];
    u32 handle;

    func_003014F0(text, D_003BC568, *(s32 *)(info + 0x10));
    handle = func_001979C8(x, y, z, w, text, 0);
    frFontSetContextPair(handle, x + ((0xBE - frFontMeasureLines(handle)) << 4), y);
    func_001958A0(handle, 1, color);
    frFontQueueGlyphInSelectedSlot(handle);
}

void func_002652E0(s32 x, s32 y, s32 z, s32 w, u8 *info, s32 color) {
    char text[32];
    u32 handle;

    func_003014F0(text, D_003BC568, *(s32 *)(info + 0xC));
    handle = func_001979C8(x, y, z, w, text, 0);
    frFontSetContextPair(handle, x + ((0xBE - frFontMeasureLines(handle)) << 4), y);
    func_001958A0(handle, 1, color);
    frFontQueueGlyphInSelectedSlot(handle);
}

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB20);

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB30);

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB40);

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC560);

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC568);

