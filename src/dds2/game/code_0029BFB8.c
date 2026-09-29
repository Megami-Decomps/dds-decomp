#include "common.h"

extern s32 func_002C4038();

extern s32 func_00101958();

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

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029BFB8);

s64 itfRunPanelMode1(s32 arg0) {
    s32 ctx = func_00101958();
    func_0029AA48(ctx);
    func_0029AC20(ctx, 0);
    return func_002C4038(ctx + 8, ctx + 0x54, 1, arg0);
}

s64 itfRunPanelMode2(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C8E8(0);
    return func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C3F0);

extern u32 func_00309138(u32, u32, s32);

void func_0029C450(u8 *work) {
    s32 remaining = 0x100 - ((TitleMenuWork *)work)->fadeProgress;
    u32 opacity;
    if (((TitleMenuWork *)work)->opacityReady != 0) {
        return;
    }
    opacity = func_00309138(0x80808080, 0x80808000, remaining) & 0xFF;
    ((TitleMenuWork *)work)->opacity = opacity;
    if (opacity >= 0x80) {
        ((TitleMenuWork *)work)->opacityReady = 1;
    }
}

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;

extern void func_00308620(BurstSprite *, u32 *, s32, s32, s32);

void mnuTitleDrawBurstSprites(s32 arg0, s32 arg1) {
    BurstTable table = D_00428420;
    s32 scaled = arg0 * 0x13 / 256;
    u32 i;

    for (i = 0; i < 8; i++) {
        func_00308620(&table.sprite[i], &table.sprite[i].word[3], 0, scaled, arg1);
    }
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C618);

void func_0029C800(void) {
}

void func_0029C808(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C810);

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C848);

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C860);

void func_0029C878(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C880);

extern u32 D_004379C8[];

extern s32 func_0019F6C8();

extern void func_0035C860();

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
    func_0019C5B0(sprite);
    color[0] = alpha;
    color[1] = alpha;
    color[2] = alpha;
    color[3] = alpha;
    func_00306C28(x + 0x180, y + 0x78, 0, color, 0, ((TitleMenuWork *)work)->iconResource, 0x1B, 0x53);
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029CB70);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_004284E0);

