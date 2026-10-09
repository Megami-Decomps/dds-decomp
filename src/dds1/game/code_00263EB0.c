#include "mnu_input.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_result.h"


extern void func_0024DD78(void);
extern u8 *D_003BAA70;
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern u8 brsGetLevelStepCrossedBy(s32, s32);
extern u8 brsGetLevelStepForValue(s32);
extern s32 func_003014F0(char *, const char *, ...);
extern void dspSetActive(s32);
extern void dspStartEntry(s32);
extern void evtStageTestQueueMotion(s32, u32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern char D_003BC550[];
extern char D_003BC558[];
extern char *D_0036D3A8[];
extern void mnuClearItemSelectionSlots(BrsSkillPackageWork *);
extern void mnuRefreshPartyUnitVitalsPanels(DatPartyRecord *, BrsSkillPackageWork *);


/* All five signed-byte plus table-word totals must meet the minimum. */
s32 mnuCheckTableSums(DatPartyRecord *bytes, BrsSkillPackageWork *table) {
    s32 *tableValues = table->statGains;
    s8 *byteValues = bytes->baseStats;
    s32 index = 0;

    do {
        s32 total = *byteValues + *tableValues;

        byteValues++;
        tableValues++;
        if (total < MENU_SUM_MINIMUM) {
            return 0;
        }
        index++;
    } while (index < MENU_SUM_COUNT);
    return 1;
}

extern s32 evtStageTestUpdateCamera(void);
extern s32 evtGetMessageWindowControlState(void);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern char D_0036D440[];

s32 func_00263EF8(KwlnTask *request) {
    BrsSkillPackageWork *work;
    DatPartyRecord *entry;
    s32 inputFlags;
    s32 selection;
    s32 changed = 0;
    s32 result;

    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);
    inputFlags = mnuMapPadMaskToFlags(0xF3);
    entry = work->selectedRewardRow->unit;
    evtStageTestUpdateCamera();
    result = func_00285670(&work->transition, &work->transition.state, 0, request);
    if (result != 0) {
        return result;
    }
    if (work->transition.state != 0 || evtGetMessageWindowControlState() != 0) {
        return 0;
    }

    if (work->selectionInitialized == 0) {
        mnuSetPanelGroupSelection(work->panelHandle, 0);
        work->selectionInitialized = 1;
    }

    if (work->assignedStatPoints == work->availableStatPoints ||
        mnuCheckTableSums(entry, work) != 0) {
        dspStartEntry(0x16);
        evtSetMessageWindowOptionWhenOpen(0);
        evtStoreValueAndCaptureWindowPanelValue(0x1D);
        mnuSetPopupEntryFlagged(&work->transition.state, D_0036D440);
    } else {
        selection = mnuGetPanelGroupSelection(work->panelHandle);
        if ((inputFlags & 0x81) != 0) {
            if (entry->baseStats[selection] + work->statGains[selection] < 99) {
                inputFlags = 1;
                changed = 1;
                work->assignedStatPoints++;
                work->statGains[selection]++;
            } else {
                inputFlags = 0x8000;
            }
        }
        if ((inputFlags & 0x40) != 0) {
            if (work->assignedStatPoints > 0 && work->statGains[selection] > 0) {
                changed = 1;
                work->assignedStatPoints--;
                work->statGains[selection]--;
            } else {
                inputFlags = 0x8000;
            }
        }
        if ((inputFlags & 2) != 0) {
            changed = 1;
            mnuClearItemSelectionSlots(work);
        }
        if (changed != 0) {
            memcpy(entry, &work->previewUnit, sizeof(*entry));
            mnuRefreshPartyUnitVitalsPanels(entry, work);
        }
        if ((inputFlags & 0x10) != 0) {
            selection--;
        }
        if ((inputFlags & 0x20) != 0) {
            selection++;
        }
        if (selection < 0) {
            selection = DAT_BASE_STAT_COUNT - 1;
        }
        if (selection >= DAT_BASE_STAT_COUNT) {
            selection = 0;
        }
        mnuSetPanelGroupSelection(work->panelHandle, selection);
    }
    mnuPlayInputSound(0, inputFlags, 0);
    return 0;
}

extern void mnuDrawItemPanelBackdrop(BrsSkillPackageWork *);
extern void func_00263B78(BrsSkillPackageWork *, s32);

s32 mnuDrawItemPanelDuringRequest(KwlnTask *request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, (void *)request);
}

s32 func_00264238(KwlnTask *input) {
    s32 context = kwlnTaskGetUserValue(input);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)input);
}

u32 mnuBeginPanelEntryAndCaptureSoundMode(KwlnTask *task) {
    BrsSkillPackageWork *context;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    mnuSetPanelGroupSelection(context->panelHandle, -1);
    dspStartEntry(0x16);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

extern s8 evtGetCapturedWindowPanelValue(void);
extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern char D_0036D45C[];

s32 func_002642D0(KwlnTask *request) {
    BrsSkillPackageWork *work;
    DatPartyRecord *entry;
    s32 result;

    work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);
    entry = work->selectedRewardRow->unit;
    evtStageTestUpdateCamera();
    result = func_00285670(&work->transition, &work->transition.state, 0, request);
    if (result != 0) {
        return result;
    }
    if (work->transition.state == 0 && evtGetMessageWindowControlState() == 0) {
        s8 capturedValue = evtGetCapturedWindowPanelValue();
        s32 *stats = work->statGains;

        if (capturedValue == 0) {
            btlAddBaseStats(stats, work->selectedRewardRow->unit);
            mnuClearItemSelectionSlots(work);
            mnuClearPanelGroupSelection(work->panelHandle);
            mnuSetPopupEntryFlagged(&work->transition.state, D_0036D45C);
            work->commitComplete = 1;
        } else {
            mnuClearItemSelectionSlots(work);
            mnuSetPanelGroupSelection(work->panelHandle, 0);
            memcpy(entry, &work->previewUnit, sizeof(*entry));
            mnuRefreshPartyUnitVitalsPanels(entry, work);
            mnuBindPresentMenuEntry(&work->transition, &work->transition.state);
        }
    }
    return 0;
}

s32 func_00264498(KwlnTask *request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 1);
    return menuRunPanel(context, 1, (void *)request);
}

s32 func_002644F0(KwlnTask *input) {
    s32 context = kwlnTaskGetUserValue(input);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)input);
}

u32 func_00264538(KwlnTask *task) {
    char text[0x20];
    BrsSkillPackageWork *work = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    BrsRewardRow *state = work->selectedRewardRow;
    DatPartyRecord *entry = state->unit;
    u32 step = brsGetLevelStepCrossedBy(entry->level - state->values.amount, state->values.amount);

    work->crossedSteps = step;
    if (step != 0) {
        func_003014F0(text, D_003BC558, D_003BAA70 + entry->unitId * 17);
        evtCopyEntryStringToActiveWindow(0, text);
        func_003014F0(text, D_003BC550, brsGetLevelStepForValue(entry->level));
        evtCopyEntryStringToActiveWindow(1, text);
        dspSetActive(1);
        dspStartEntry(0x17);
        sndSetSequenceVolumePan(7, 0x7F, 0x3F);
    }
    return 1;
}

u32 func_00264608(void) {
    return 1;
}

extern s32 evtStageTestUpdateCamera(void);
extern s32 evtGetMessageWindowControlState(void);
extern char D_0036D478[];

/* On an idle panel, apply the extra fallback only when the auxiliary check also fails. */
s32 mnuRunPanelWithIdleFallback(KwlnTask *request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);
    s32 *panelState = &context->transition.state;
    s32 result;

    evtStageTestUpdateCamera();
    result = func_00285670(&context->transition, panelState, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panelState == 0 && evtGetMessageWindowControlState() == 0) {
        mnuSetPopupEntryFlagged(panelState, D_0036D478);
    }
    return 0;
}

s32 mnuRunItemPanelWithInactiveBackdrop(KwlnTask *request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);

    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel(context, 1, (void *)request);
}

s32 func_002646F8(KwlnTask *input) {
    s32 context = kwlnTaskGetUserValue(input);

    func_0024DD78();
    return menuRunPanel((void *)context, 2, (void *)input);
}


u32 func_00264740(KwlnTask *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    DatPartyRecord *item = context->selectedRewardRow->unit;

    if (context->rewardMode != 0) {
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + item->unitId * 17);
        dspStartEntry(0x18);
    }
    memset(context->statGains, 0, sizeof(context->statGains));
    return 1;
}

u32 func_002647B0(void) {
    evtFinishMessageWindowAndNotify();
    return 1;
}

s32 func_002647D0(BrsSkillPackageWork *work) {
    DatPartyRecord *entry = work->selectedRewardRow->unit;
    s32 result;

    switch (work->rewardMode) {
    case 4: {
        s32 rewardIndex = work->rewardIndex;

        work->statGains[rewardIndex]++;
        evtCopyEntryStringToActiveWindow(1, D_0036D3A8[rewardIndex]);
    }
    /* fall through */
    case 1:
    case 2:
    case 3:
        evtCopyEntryStringToActiveWindow(0, D_003BAA70 + entry->unitId * 17);
        dspStartEntry(work->rewardMode + 0x18);
        break;
    default:
        break;
    }

    evtStageTestQueueMotion(2, 1);
    result = work->rewardMode < 4;
    if (result != 0 && work->rewardMode > 0) {
        result = evtStageTestSetPendingEffect(0);
    }
    return result;
}

#include "common.h"
#include "mnu_result.h"


extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);

void kwlnItemUpdateDisplay(BrsSkillPackageWork *scene) {
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    switch (scene->rewardMode) {
    case 4:
        btlAddBaseStats(scene->statGains, item);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        item->hp = item->maxHp;
        item->mp = item->maxMp;
        item->status = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        item->hp = item->maxHp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        item->mp = item->maxMp;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    mnuRefreshSelectedUnitPanels(item, scene);
}

#include "common.h"
#include "fr_font_measure.h"
#include "kwln.h"
#include "eff.h"
#include "mnu_result.h"
#include "itf.h"


extern char D_0036D408[];

/* Terminal panel poll: once the message window is idle, apply the pending reward step or open the popup. */
s32 func_002649B0(KwlnTask *request) {
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)kwlnTaskGetUserValue(request);
    s32 result;

    evtStageTestUpdateCamera();
    result = menuRunPanel(panel, 0, request);
    if (result != 0) {
        return result;
    }
    if (panel->transition.state == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (panel->rewardMode != 0) {
                func_002647D0(panel);
                kwlnItemUpdateDisplay(panel);
                panel->rewardMode = 0;
            } else {
                mnuSetPopupEntryFlagged(&panel->transition.state, D_0036D408);
            }
        }
    }
    return 0;
}


extern void mnuDrawItemPanelBackdrop(BrsSkillPackageWork *);
extern void func_00263B78(BrsSkillPackageWork *, s32);

/* The dispatcher consumes the inline work area and its adjacent status word. */

s32 itfRunPanelMode1(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    mnuDrawItemPanelBackdrop(panel);
    func_00263B78(panel, 0);
    return menuRunPanel(panel, 1, request);
}


extern void func_0024DC98(s32);

s32 itfRunPanelMode2(KwlnTask *request) {
    s32 context = kwlnTaskGetUserValue(request);
    BrsSkillPackageWork *panel = (BrsSkillPackageWork *)context;

    func_0024DC98(0);
    return menuRunPanel(panel, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264B08);
INCLUDE_ASM(const s32, "game/code_00263EB0", func_00264D90);

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

INCLUDE_RODATA(const s32, "game/code_00263EB0", D_003AFB20);

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

    if (work->teardownResource != NULL) {
        for (i = 0; i < 8; i++) {
            u32 color = work->opacity | 0x80808000;

            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            func_002BF438(positions[i][0] << 4, positions[i][1] << 3, 0,
                         colors, 0, work->teardownResource, positions[i][2], 0x53);
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
extern s32 frFontDrawGlyphChain(FrFontGlyph *, s8, u32);
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
                          work->teardownResource, 0x1C, 0x53);
            func_003014F0(formatted, D_003BC560, parameter);
            valueGlyph = func_001979C8(x + 0xC60, y + 0x18, depth, color,
                                       formatted, iconGlyph);
            frFontDrawGlyphChain(valueGlyph, 1, textStyle);
            frFontQueueGlyphForCurrentDrawBuffer(valueGlyph);
            y += 0xB0;
        }
    }
}

void itfDrawCountText(s32 x, s32 y, s32 z, s32 w, const BrsRewardSummary *info, s32 color) {
    char text[32];
    FrFontGlyph *glyph;

    func_003014F0(text, D_003BC568, info->totalExp);
    glyph = func_001979C8(x, y, z, w, text, 0);
    frFontSetGlyphPosition(glyph, x + ((0xBE - frFontMeasureLines(glyph)) << 4), y);
    frFontDrawGlyphChain(glyph, 1, color);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

void mnuQueueRightAlignedFormattedInfoText(s32 x, s32 y, s32 z, s32 w, const BrsRewardSummary *info, s32 color) {
    char text[32];
    FrFontGlyph *glyph;

    func_003014F0(text, D_003BC568, info->macca);
    glyph = func_001979C8(x, y, z, w, text, 0);
    frFontSetGlyphPosition(glyph, x + ((0xBE - frFontMeasureLines(glyph)) << 4), y);
    frFontDrawGlyphChain(glyph, 1, color);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC558);

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC560);

INCLUDE_SDATA(const s32, "game/code_00263EB0", D_003BC568);

