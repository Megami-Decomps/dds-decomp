#include "common.h"
#include "mnu_result.h"
struct EffectSlotSet;
extern void func_00306C28(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void frFontSetChainFlag();
extern s32 frFontQueueGlyphInSelectedSlot();
extern s32 func_0019D550();


extern u32 kwlnTaskGetUserValue();


INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029BFB8);

s32 itfRunPanelMode1(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    func_0029AA48(panel);
    func_0029AC20(panel, 0);
    return func_002C4038(&panel->transition, &panel->transition.state, 1, (void *)request);
}

s32 itfRunPanelMode2(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    func_0026C8E8(0);
    return func_002C4038(&panel->transition, &panel->transition.state, 2, (void *)request);
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C3F0);

extern u32 uiBlendColors(u32, u32, s32);

/* The opacity is latched at 0x80; later fade steps leave it untouched. */
void itfUpdateFadeColor(BrsSkillPackageWork *work) {
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

extern const s32 D_004284E0[9][3];
extern s32 mdlFlagTest(u32);
extern void func_00306CD0(s32, s32, s32, u32, s32, void *, s32, s32);

void brsDrawResultPanelSprites(BrsSkillPackageWork *work) {
    s32 table[9][3];
    s32 count;
    s32 i;
    u32 scale;

    memcpy(table, D_004284E0, sizeof(table));
    count = mdlFlagTest(0x290) ? 9 : 8;
    if (work->teardownHandle != 0) {
        for (i = 0; i < count; i++) {
            scale = (work->opacity << 8) >> 7;
            func_00306CD0(table[i][0] << 4, table[i][1] << 3,
                         0, scale, 0, (void *)work->teardownHandle,
                         table[i][2], 0x53);
            if (i == 0) {
                mnuTitleDrawBurstSprites(scale, 0x53);
                func_00306CD0(0, 0, 0, 0x100, 0,
                             (void *)work->teardownHandle, 0x25, 0x53);
            }
        }
    }
}

void func_0029C800(void) {
}

void func_0029C808(void) {
}

void func_0029C810(BrsSkillPackageWork *work) {
    work->fadeProgress = (s32)((f32)work->fadeProgress / 1.19999998f);
}

/* Getter/clear pair for one word at byte offset 0xB6E0 of the work block. */
s32 func_0029C848(BrsSkillPackageWork *work) {
    return work->fadeProgress;
}

void func_0029C860(BrsSkillPackageWork *work) {
    work->fadeProgress = 0;
}

void func_0029C878(void *work) {
}

INCLUDE_ASM(const s32, "game/code_0029BFB8", func_0029C880);

extern u32 D_004379C8[];

extern s32 func_0019F6C8();

extern s32 func_0035C860();

void mnuCampDrawMenuIconLayer(s32 x, s32 y, s32 z, u32 alpha, const BrsRewardSummary *res, s32 arg5, BrsSkillPackageWork *work) {
    char name[32];
    u32 color[4];
    s32 sprite;
    u32 packed;

    packed = (alpha & 0xFF) | 0xA09DC300;
    func_0035C860(name, D_004379C8, res->totalExp);
    sprite = func_0019F6C8(x, y, z, packed, name, 0);
    frFontSetChainFlag(sprite, 3);
    func_0019D550(sprite, 1, arg5);
    frFontQueueGlyphInSelectedSlot(sprite);
    color[0] = alpha;
    color[1] = alpha;
    color[2] = alpha;
    color[3] = alpha;
    func_00306C28(x + 0x180, y + 0x78, 0, color, 0, (struct EffectSlotSet *)work->teardownHandle, 0x1B, 0x53);
}

extern s32 mdlFlagTest();

void func_0029CB70(s32 x, s32 y, s32 z, u32 alpha, const BrsRewardSummary *res, s32 arg5, BrsSkillPackageWork *work) {
    char name[32];
    u32 color[4];
    s32 sprite;
    u32 packed;

    if (mdlFlagTest(0x290) != 0) {
        packed = (alpha & 0xFF) | 0xA09DC300;
        func_0035C860(name, D_004379C8, res->macca);
        sprite = func_0019F6C8(x, y, z, packed, name, 0);
        frFontSetChainFlag(sprite, 3);
        func_0019D550(sprite, 1, arg5);
        frFontQueueGlyphInSelectedSlot(sprite);
        color[0] = alpha;
        color[1] = alpha;
        color[2] = alpha;
        color[3] = alpha;
        func_00306C28(x + 0x180, y + 0x78, 0, color, 0, (struct EffectSlotSet *)work->teardownHandle, 0x1B, 0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0029BFB8", D_004284E0);

