#include "common.h"
#include "mnu_result.h"

extern const char *D_003D62F0[6];
extern char (*D_00435E48)[17];
extern char (*D_00435E5C)[25];
struct EffRandState;
extern u32 effMiscRand(struct EffRandState *state);
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern s32 mnuSelectEventFlagCode(void);
extern s32 dspStartEntry(s32);
extern void evtStageTestQueueMotion(s32, u32);
extern void evtStageTestSetPendingEffect(u32);

void func_0029BC58(BrsSkillPackageWork *work) {
    DatPartyRecord *unit = work->selectedRewardRow->unit;
    s32 mode = work->rewardMode;
    s32 rewardIndex;

    switch (mode) {
    case 4: {
        rewardIndex = work->rewardIndex;
        work->statGains[rewardIndex]++;
        evtCopyEntryStringToActiveWindow(1, D_003D62F0[rewardIndex]);
    }
        /* Fall through to display the rewarded unit. */
    case 1:
    case 2:
    case 3:
        evtCopyEntryStringToActiveWindow(0, D_00435E48[unit->unitId]);
        dspStartEntry(work->rewardMode + 25);
        break;
    case 5: {
        s32 roll;
        rewardIndex = unit->unitId * 4 - 4;
        roll = effMiscRand(NULL) & 3;

        evtCopyEntryStringToActiveWindow(0, D_00435E48[unit->unitId]);
        evtCopyEntryStringToActiveWindow(1, D_00435E5C[work->earnedItem]);
        switch (mnuSelectEventFlagCode()) {
        case 1:
            dspStartEntry(rewardIndex + roll + 30);
            break;
        case 5:
            dspStartEntry(rewardIndex + roll + 62);
            break;
        case 2:
            dspStartEntry(rewardIndex + roll + 94);
            break;
        case 8:
            dspStartEntry(rewardIndex + roll + 126);
            break;
        }
        break;
    }
    }

    switch (work->rewardMode) {
    case 1:
    case 2:
    case 3:
        evtStageTestQueueMotion(2, 1);
        evtStageTestSetPendingEffect(0);
        return;
    case 4:
        evtStageTestQueueMotion(2, 1);
        return;
    case 5:
        evtStageTestQueueMotion(2, 2);
        break;
    }
}


extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);

extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);

void mnuTitleApplySequenceState(BrsSkillPackageWork *work) {
    s32 state = work->rewardMode;
    DatPartyRecord *seq = work->selectedRewardRow->unit;

    switch (state) {
    case 5:
        break;
    case 4:
        btlAddBaseStats(work->statGains, seq);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        seq->hp = seq->maxHp;
        seq->mp = seq->maxMp;
        seq->status = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        seq->hp = seq->maxHp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        seq->mp = seq->maxMp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    mnuRefreshSelectedUnitPanels(seq, work);
}

#include "common.h"
#include "fr_font.h"
#include "kwln.h"
#include "mnu_result.h"
struct EffectSlotSet;
extern void func_00306C28(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void frFontSetChildChainFirstOption(struct FrFontGlyph *, u8);
extern s32 frFontDrawGlyphChain();




extern void evtStageTestUpdateCamera(void);
extern s32 evtGetMessageWindowControlState(void);
extern char D_003D6458[];

/* Terminal panel poll: once the message window is idle, apply the pending reward step or open the popup. */
s32 func_0029BFB8(KwlnTask *request) {
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);
    s32 result;

    evtStageTestUpdateCamera();
    result = func_002C4038(&panel->transition, &panel->transition.state, 0, request);
    if (result != 0) {
        return result;
    }
    if (panel->transition.state == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (panel->rewardMode != 0) {
                func_0029BC58(panel);
                mnuTitleApplySequenceState(panel);
                panel->rewardMode = 0;
            } else {
                mnuSetPopupEntryFlagged(&panel->transition.state, D_003D6458);
            }
        }
    }
    return 0;
}

s32 itfRunPanelMode1(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    func_0029AA48(panel);
    func_0029AC20(panel, 0);
    return menuSetHandler(panel, 1, request);
}

s32 itfRunPanelMode2(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    func_0026C8E8(0);
    return menuSetHandler(panel, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C3F0);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C880);

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
    frFontSetChildChainFirstOption((struct FrFontGlyph *)(u32)sprite, 3);
    frFontDrawGlyphChain(sprite, 1, arg5);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)sprite);
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
        frFontSetChildChainFirstOption((struct FrFontGlyph *)(u32)sprite, 3);
        frFontDrawGlyphChain(sprite, 1, arg5);
        frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)sprite);
        color[0] = alpha;
        color[1] = alpha;
        color[2] = alpha;
        color[3] = alpha;
        func_00306C28(x + 0x180, y + 0x78, 0, color, 0, (struct EffectSlotSet *)work->teardownHandle, 0x1B, 0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004284E0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuTitleSoundTask);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379FC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A00);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A08);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A10);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A18);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A1C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A20);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A28);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A2C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A30);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A34);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A38);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A3C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuMovieMenuState);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A48);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A50);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A58);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A60);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A68);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A70);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A78);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A80);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A88);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A90);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A98);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuMovieWork);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", mnuMovieDrawTask);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ADC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE4);

