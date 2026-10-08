#include "common.h"
#include "kwln.h"
#include "eff.h"
#include "mnu_result.h"
#include "itf.h"


INCLUDE_ASM(const s32, "game/code_002649B0", func_002649B0);


extern void mnuDrawItemPanelBackdrop(BrsSkillPackageWork *);
extern void func_00263B78(BrsSkillPackageWork *, s32);

/* The dispatcher consumes the inline work area and its adjacent status word. */

s32 itfRunPanelMode1(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    mnuDrawItemPanelBackdrop(panel);
    func_00263B78(panel, 0);
    return func_00285670(&panel->transition, &panel->transition.state, 1, request);
}


extern void func_0024DC98(s32);

s32 itfRunPanelMode2(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    func_0024DC98(0);
    return func_00285670(&panel->transition, &panel->transition.state, 2, request);
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_00264B08);
INCLUDE_ASM(const s32, "game/code_002649B0", func_00264D90);

extern u32 uiBlendColors(u32, u32, s32);

/* The opacity update latches at its threshold. Keep fadeProgress signed:
 * the decay path converts it through a signed float. */

void itfUpdateFadeColor(BrsSkillPackageWork *work) {
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

void mnuDrawTitleFadeSprites(BrsSkillPackageWork *work) {
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

    if (work->teardownHandle) {
        for (i = 0; i < 8; i++) {
            u32 color = work->opacity | 0x80808000;

            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            func_002BF438(positions[i][0] << 4, positions[i][1] << 3, 0,
                         colors, 0, (EffectSlotSet *)work->teardownHandle, positions[i][2], 0x53);
        }
    }
}

void func_00265078(void) {
}

void func_00265080(void) {
}

/* Decay only the fade progress; opacity stops updating once it crosses 0x80. */
void brsStepAnimDecay(BrsSkillPackageWork *work) {
    work->fadeProgress = (s32)((f32)work->fadeProgress / 1.2f);
}


u32 mnuGetTitleState(BrsSkillPackageWork *work) {
    return work->fadeProgress;
}

void mnuClearTitleState(BrsSkillPackageWork *work) {
    work->fadeProgress = 0;
}

void func_002650C0(void *work) {
}

extern u8 *D_003BAA84;
extern char D_003BC560[];
extern FrFontGlyph *itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, FrFontGlyph *);
extern s32 func_003014F0(char *, const char *, ...);
extern FrFontGlyph *func_001979C8(s32, s32, s32, s32, char *, FrFontGlyph *);
extern u32 frFontMeasureLines(FrFontGlyph *);
extern s32 func_001958A0(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern char D_003BC568[];

/* Draw each nonempty reward icon row with its name and formatted parameter. */
void func_002650C8(s32 x, s32 y, s32 depth, u32 color, BrsRewardSummary *summary,
                  u32 textStyle, BrsSkillPackageWork *work) {
    char formatted[32];
    u32 colors[4];
    MenuIconRef *icon = summary->icons;
    u32 i;

    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    for (i = 0; i < 3; i++) {
        u16 id = icon->id;
        u8 parameter = icon->param;
        FrFontGlyph *iconGlyph;
        FrFontGlyph *valueGlyph;

        icon++;
        if (id != 0) {
            const u8 *name = D_003BAA84 + id * 25;

            iconGlyph = itfCreateConvertedTextGlyph(x + 0x300, y, depth, color, name, NULL);
            func_002BF438(x + 0xB20, y + 0x28, 0, colors, 0,
                          (EffectSlotSet *)work->teardownHandle, 0x1C, 0x53);
            func_003014F0(formatted, D_003BC560, parameter);
            valueGlyph = func_001979C8(x + 0xC60, y + 0x18, depth, color,
                                       formatted, iconGlyph);
            func_001958A0(valueGlyph, 1, textStyle);
            frFontQueueGlyphInSelectedSlot(valueGlyph);
            y += 0xB0;
        }
    }
}

void itfDrawCountText(s32 x, s32 y, s32 z, s32 w, const BrsRewardSummary *info, s32 color) {
    char text[32];
    FrFontGlyph *glyph;

    func_003014F0(text, D_003BC568, info->totalExp);
    glyph = func_001979C8(x, y, z, w, text, 0);
    frFontSetContextPair(glyph, x + ((0xBE - frFontMeasureLines(glyph)) << 4), y);
    func_001958A0(glyph, 1, color);
    frFontQueueGlyphInSelectedSlot(glyph);
}

void mnuQueueRightAlignedFormattedInfoText(s32 x, s32 y, s32 z, s32 w, const BrsRewardSummary *info, s32 color) {
    char text[32];
    FrFontGlyph *glyph;

    func_003014F0(text, D_003BC568, info->macca);
    glyph = func_001979C8(x, y, z, w, text, 0);
    frFontSetContextPair(glyph, x + ((0xBE - frFontMeasureLines(glyph)) << 4), y);
    func_001958A0(glyph, 1, color);
    frFontQueueGlyphInSelectedSlot(glyph);
}

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC560);

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC568);
