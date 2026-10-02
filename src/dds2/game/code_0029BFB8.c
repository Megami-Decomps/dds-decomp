#include "common.h"
extern void frFontSetChainFlag();
extern s32 frFontQueueGlyphInSelectedSlot();
extern s32 func_0019D550();

extern s32 func_002C4038();

extern s32 kwlnTaskGetUserValue();

typedef struct {
    u8 pad0[6];
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
} TitleSeq;

typedef struct TitleMenuWork {
    u8 pad00[0x9C];
    TitleSeq **sequence;      /* 0x009C */
    u8 padA0[0xAE08];
    s8 opacityReady;          /* 0xAEA8 */
    u8 padAEA9[7];
    s32 iconResource;         /* 0xAEB0 */
    u32 opacity;              /* 0xAEB4 */
    u8 padAEB8[0x828];
    s32 fadeProgress;         /* 0xB6E0 */
    u8 padB6E4[0x10];
    s32 sequenceMode;         /* 0xB6F4 */
} TitleMenuWork;

/* Shared dispatcher layout; the title menu keeps its work and status adjacent. */
typedef struct {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus;     /* 0x54 */
} PanelDispatchContext;

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029BFB8);

s32 itfRunPanelMode1(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    PanelDispatchContext *panel = (PanelDispatchContext *)context;

    func_0029AA48(context);
    func_0029AC20(context, 0);
    return func_002C4038(panel->dispatchWork, &panel->dispatchStatus, 1, request);
}

s32 itfRunPanelMode2(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    PanelDispatchContext *panel = (PanelDispatchContext *)context;

    func_0026C8E8(0);
    return func_002C4038(panel->dispatchWork, &panel->dispatchStatus, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C3F0);

extern u32 uiBlendColors(u32, u32, s32);

/* The opacity is latched at 0x80; later fade steps leave it untouched. */
void itfUpdateFadeColor(TitleMenuWork *work) {
    s32 remaining = 0x100 - work->fadeProgress;
    u32 opacity;
    if (work->opacityReady != 0) {
        return;
    }
    opacity = uiBlendColors(0x80808080, 0x80808000, remaining) & 0xFF;
    work->opacity = opacity;
    if (opacity >= 0x80) {
        work->opacityReady = 1;
    }
}

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;

extern void uiDrawUniformRgbRange(BurstSprite *, u32 *, s32, s32, s32);

void mnuTitleDrawBurstSprites(s32 scaleInput, s32 arg1) {
    BurstTable table = D_00428420;
    s32 scaled = scaleInput * 0x13 / 256;
    u32 i;

    for (i = 0; i < 8; i++) {
        uiDrawUniformRgbRange(&table.sprite[i], &table.sprite[i].word[3], 0, scaled, arg1);
    }
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C618);

void func_0029C800(void) {
}

void func_0029C808(void) {
}

void func_0029C810(TitleMenuWork *work) {
    work->fadeProgress = (s32)((f32)work->fadeProgress / 1.19999998f);
}

/* Getter/clear pair for one word at byte offset 0xB6E0 of the work block. */
s32 func_0029C848(s32 *work) {
    return work[0x2DB8];
}

void func_0029C860(s32 *work) {
    work[0x2DB8] = 0;
}

void func_0029C878(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C880);

extern u32 D_004379C8[];

extern s32 func_0019F6C8();

extern s32 func_0035C860();

void mnuCampDrawMenuIconLayer(s32 x, s32 y, s32 z, u32 alpha, u8 *res, s32 arg5, u8 *work) {
    char name[32];
    u32 color[4];
    s32 sprite;
    u32 packed;

    packed = (alpha & 0xFF) | 0xA09DC300;
    func_0035C860(name, D_004379C8, *(u32 *)(res + 0x10));
    sprite = func_0019F6C8(x, y, z, packed, name, 0);
    frFontSetChainFlag(sprite, 3);
    func_0019D550(sprite, 1, arg5);
    frFontQueueGlyphInSelectedSlot(sprite);
    color[0] = alpha;
    color[1] = alpha;
    color[2] = alpha;
    color[3] = alpha;
    func_00306C28(x + 0x180, y + 0x78, 0, color, 0, ((TitleMenuWork *)work)->iconResource, 0x1B, 0x53);
}

extern s32 mdlFlagTest();

void func_0029CB70(s32 x, s32 y, s32 z, u32 alpha, u8 *res, s32 arg5, u8 *work) {
    char name[32];
    u32 color[4];
    s32 sprite;
    u32 packed;

    if (mdlFlagTest(0x290) != 0) {
        packed = (alpha & 0xFF) | 0xA09DC300;
        func_0035C860(name, D_004379C8, *(u32 *)(res + 0xC));
        sprite = func_0019F6C8(x, y, z, packed, name, 0);
        frFontSetChainFlag(sprite, 3);
        func_0019D550(sprite, 1, arg5);
        frFontQueueGlyphInSelectedSlot(sprite);
        color[0] = alpha;
        color[1] = alpha;
        color[2] = alpha;
        color[3] = alpha;
        func_00306C28(x + 0x180, y + 0x78, 0, color, 0, ((TitleMenuWork *)work)->iconResource, 0x1B, 0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_004284E0);

