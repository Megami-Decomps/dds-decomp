#include "mnu_result.h"

extern void ptyRecomputeMaxVitals(DatPartyRecord *, const s32 *);

extern u32 ptyBuildProfileCapSkillList(DatPartyRecord *, PrfSkillList *);

extern s32 mdlFlagTest(u32);

extern s32 kwlnTaskGetUserValue();

extern void dspSetActive(s32);

extern s32 dspStartEntry(s32);

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);

extern s32 prfGetCapValue(u16);

extern s32 ptyTestProfileFlag0(DatPartyRecord *, u16);

extern void ptyApplyProfile(DatPartyRecord *, u16);

extern void mdlFlagSet(s32);

extern void mnuRefreshPanelLayer(BrsSkillPackageWork *);

extern s32 func_00285670(s32, s32 *, u64, u64);

extern void func_0024DD78(void);


/* Apply a capped selected profile and record whether the selection was applied. */
void kwlnItemApplySelection(BrsSkillPackageWork *scene) {
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    DatProfileRecord *data = ptyGetCurrentProfileRecord(item);
    s8 selection = item->profileId;
    if (selection != 0 && prfGetCapValue((u16)selection) == data->value &&
        ptyTestProfileFlag0(item, item->profileId) == 0) {
        ptyApplyProfile(item, item->profileId);
        scene->selectionApplied = 1;
        if (mdlFlagTest(0x910) == 0) {
            scene->overlayFlags |= 1;
            mdlFlagSet(0x910);
        }
    } else {
        scene->selectionApplied = 0;
    }
}

/* Build the capped skill list and cache its pending count; return 1. */
u32 mnuProcessItemSelection(BrsSkillPackageWork *scene) {
    u32 skillCount;
    skillCount = ptyBuildProfileCapSkillList(
        scene->selectedRewardRow->unit, &scene->skillList);
    scene->pendingSkillCount = skillCount;
    kwlnItemApplySelection(scene);
    return 1;
}

typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

extern DspUnitName *D_003BAA70;
extern DspUnitName *D_003BAA8C;
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern s32 func_002CD240(u16, s32 *);

void prfCapPresentMessages(BrsSkillPackageWork *scene) {
    s32 message;
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 profileId = ptyGetCurrentProfileId(item);
    u16 skillId;

    if (scene->selectionApplied != 0) {
        evtCopyEntryStringToActiveWindow(
            0, (s32)D_003BAA70[item->unitId].encodedText);
        func_002CD240(profileId & 0xFFFF, &message);
        evtCopyEntryStringToActiveWindow(1, message);
        dspSetActive(1);
        dspStartEntry(item->unitId + 2);
        scene->selectionApplied = 0;
    } else if (scene->pendingSkillCount > 0) {
        skillId = scene->skillList.skills[scene->pendingSkillIndex];
        evtCopyEntryStringToActiveWindow(
            0, (s32)D_003BAA70[item->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, (s32)D_003BAA8C[skillId].encodedText);
        dspSetActive(1);
        dspStartEntry(0);
        scene->pendingSkillIndex++;
        scene->pendingSkillCount--;
    }
}

s32 kwlnItemDismissOverlay(BrsSkillPackageWork *scene) {
    if (scene->overlayFlags & 1) {
        dspSetActive(1);
        dspStartEntry(1);
        scene->overlayFlags &= ~1;
        return 1;
    }
    return 0;
}

/* Record the first visit to the item-selection scene. */
u32 mnuMarkItemSelectionSceneVisited(BrsSkillPackageWork *scene) {
    s64 alreadyVisited;

    alreadyVisited = mdlFlagTest(0x911);
    if (alreadyVisited == 0) {
        mdlFlagSet(0x911);
    }
    return 0;
}

extern s32 evtGetMessageWindowControlState(void);
extern void brsSelectNextUnit(BrsSkillPackageWork *, s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void mnuSetPopupEntryFlagged(s32 *, void *);
extern s32 btlHasPendingRuntimeActivity(void);
extern s32 mnuStaffInitPanel(BrsSkillPackageWork *);
extern void func_002E8E50(void);
extern char D_0036D494[];
extern char D_0036D408[];

s32 prfCapTaskStep(u64 request) {
    s32 result;
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    s32 *dispatchStatus = &scene->transition.state;

    result = func_00285670((s32)scene->transition.data, dispatchStatus, 0,
                          request);
    if (result == 0) {
        if (*dispatchStatus == 0 &&
            (result = evtGetMessageWindowControlState(), result == 0)) {
            if (scene->selectedRow < scene->secondaryRewards.count ||
                scene->pendingSkillCount > 0 ||
                scene->selectionApplied != 0) {
                if ((scene->pendingSkillCount == 0 ||
                     scene->selectedRow == 0) &&
                    scene->selectionApplied == 0) {
                    brsSelectNextUnit(scene, 0);
                    mnuProcessItemSelection(scene);
                }
                prfCapPresentMessages(scene);
                return 0;
            }

            if (mnuMarkItemSelectionSceneVisited(scene) != 0) {
                return 0;
            }
            if (kwlnItemDismissOverlay(scene) != 0) {
                return 0;
            }
            if (scene->primaryRewards.count == 0) {
                kwlnFadeInStart(0, 0, 0, 0xF);
                mnuSetPopupEntryFlagged(dispatchStatus, D_0036D494);
                return 0;
            }
            if (btlHasPendingRuntimeActivity() != 0) {
                return 0;
            }
            mnuStaffInitPanel(scene);
            mnuSetPopupEntryFlagged(dispatchStatus, D_0036D408);
            func_002E8E50();
        }
        result = 0;
    }
    return result;
}

s32 func_00263570(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    mnuRefreshPanelLayer(context);
    return menuRunPanel((s32)context, 1, request);
}

s32 func_002635C0(s32 request) {
    s32 context = kwlnTaskGetUserValue();
    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

/* Clear the scene's two selection-processing markers; return 1. */
s32 mnuResetItemSelectionMarkers(void) {
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    scene->selectedRow = 0;
    scene->resetStateB = 0;
    return 1;
}

u32 func_00263638(void) {
    return 1;
}

typedef struct ActiveItemSlots {
    s32 indices[5];
    s32 values[5];
    s32 count;
} ActiveItemSlots;

void func_00263640(BrsSkillPackageWork *scene) {
    ActiveItemSlots active;
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 i;

    memset(&active, 0, sizeof(active));
    for (i = 0; i < 5; i++) {
        if (scene->statGains[i] > 0) {
            active.indices[active.count] = i;
            active.values[active.count] = scene->statGains[i];
            active.count++;
        }
    }

    evtCopyEntryStringToActiveWindow(0,
                                     (s32)D_003BAA70[item->unitId].encodedText);
    dspSetActive(1);
    if (scene->extentExhausted == 0) {
        dspStartEntry(0x14);
    } else {
        dspStartEntry(0x15);
    }
}

extern char D_003BC550[];
extern s32 func_003014F0(char *, const char *, ...);
extern void func_00263640(BrsSkillPackageWork *);

/* Cap the available extent by the remaining capacity of five components. */
void func_00263728(BrsSkillPackageWork *scene) {
    char text[16];
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 available = scene->selectedRewardRow->values.amount * 3;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < 5; i++) {
        sum += item->baseStats[i];
    }
    if (495 - sum < available) {
        available = 495 - sum;
    }
    if (available == 0) {
        scene->extentExhausted = 1;
    } else {
        scene->extentExhausted = 0;
    }
    if (item->unitId == 1) {
        evtCopyEntryStringToActiveWindow(0, (s32)D_003BAA70[item->unitId].encodedText);
        func_003014F0(text, D_003BC550, available);
        evtCopyEntryStringToActiveWindow(1, (s32)text);
        dspSetActive(1);
        if (scene->extentExhausted == 0) {
            dspStartEntry(0x12);
        } else {
            dspStartEntry(0x13);
        }
        return;
    }
    func_00263640(scene);
}

extern void evtStageTestUpdateCamera(void);
extern s32 brsAdvanceSkillPackagePanel(BrsSkillPackageWork *);
extern void ptyAccumulateStatGains(s32 *, s32, DatPartyRecord *);
extern s32 mnuAdvanceTitleEntryAnimation(DatPartyRecord *);
extern void mnuStaffCopyPanelBlock(DatPartyRecord *, BrsSkillPackageWork *);
extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);
extern u32 mnuInitializeItemSelectionExtent(u64);
extern void mnuSetPopupEntry(s32 *, void *);
extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern void func_002E96D8(u32);
extern char D_0036D424[];
extern char D_0036D45C[];

s32 func_00263838(u64 request) {
    s32 result;
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    s32 *dispatchStatus;
    s32 *slots;

    evtStageTestUpdateCamera();
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    dispatchStatus = &scene->transition.state;
    result = func_00285670((s32)scene->transition.data, dispatchStatus, 0,
                          request);
    if (result != 0) {
        return result;
    }
    if (brsAdvanceSkillPackagePanel(scene) != 0) {
        return 0;
    }
    if (*dispatchStatus == 0) {
        if (scene->selectedRow < scene->primaryRewards.count) {
            brsSelectNextUnit(scene, 1);
            slots = scene->statGains;
            ptyAccumulateStatGains(slots, scene->selectedRewardRow->values.amount,
                                   scene->selectedRewardRow->unit);
            func_00263728(scene);
            mnuAdvanceTitleEntryAnimation(scene->selectedRewardRow->unit);
            mnuStaffCopyPanelBlock(scene->selectedRewardRow->unit, scene);
            mnuRefreshSelectedUnitPanels(scene->selectedRewardRow->unit, scene);

            if (scene->selectedRewardRow->unit->unitId == 1) {
                mnuInitializeItemSelectionExtent(request);
                if (scene->extentExhausted == 0) {
                    mnuSetPopupEntry(dispatchStatus, D_0036D424);
                    return 0;
                }
            } else {
                btlAddBaseStats(slots, scene->selectedRewardRow->unit);
                mnuRefreshSelectedUnitPanels(scene->selectedRewardRow->unit,
                                             scene);
            }
            mnuSetPopupEntry(dispatchStatus, D_0036D45C);
        } else {
            func_002E96D8(0x50001);
            kwlnFadeInStart(0, 0, 0, 0xF);
            mnuSetPopupEntryFlagged(dispatchStatus, D_0036D494);
        }
    }
    return 0;
}

extern void mnuDrawBackdrop(MenuAssets *, s32);

void mnuDrawItemPanelBackdrop(BrsSkillPackageWork *scene) {
    mnuDrawBackdrop(&scene->assets, 0x20);
}

extern void func_002BF4E0(s32, s32, s32, s32, s32, u32, s32, s32);
extern u32 uiBlendColors(u32, u32, s32);
extern u32 func_001979C8(s32, s32, s32, u32, char *, s32);
extern void func_001958A0(u32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(u32);

void func_00263A00(BrsSkillPackageWork *scene) {
    char text[16];
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 fade = scene->iconFade;
    s32 delta;
    s32 x;
    u32 glyph;
    u32 color;

    func_002BF4E0(0x9A0, 0x348, 0, fade, 1, scene->unitHandle, 8, 0x53);
    delta = scene->commitComplete != 0
                ? 0
                : scene->availableStatPoints - scene->assignedStatPoints;

    x = 0xB70;
    if (delta / 100 <= 0) {
        x = delta / 10 > 0 ? 0xBE0 : 0xC30;
    }

    color = uiBlendColors(0xFFF06480, 0xFFF06400, fade);
    func_003014F0(text, D_003BC550, delta);
    glyph = func_001979C8(x, 0x408, 0, color, text, 0);
    func_001958A0(glyph, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(glyph);

    if (item->unitId == 1) {
        if (scene->iconFade < 0x100) {
            scene->iconFade += 0x20;
        }
        if (scene->iconFade > 0x100) {
            scene->iconFade = 0x100;
        }
    } else {
        scene->iconFade = 0;
    }
}

extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);
extern void mnuDrawPanelListDefault();
extern void func_002833B0(s32, s32, s32, void *, s32, s32);
extern void func_00263A00(BrsSkillPackageWork *);
extern s8 evtStageTestUpdate(s32);
extern u8 D_00325788[];

void func_00263B78(BrsSkillPackageWork *scene, s32 copyOptions) {
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 i;

    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x19, 0x53);
    scene->partyWindow.flags |= 0x400;
    mnuDrawPanelListDefault(0, 0, 0, &scene->partyWindow, 0x53);

    for (i = 0; i < 5; i++) {
        if (copyOptions == 0) {
            mnuSetGroupSelection(scene->panelHandle, i, scene->statGains[i], 0);
        } else {
            mnuSetGroupSelection(scene->panelHandle, i, scene->statGains[i],
                                 scene->statGains[i]);
        }
    }
    func_00283110(0xEB0, 0x518, 0, item, scene->panelHandle, 0x53);
    func_002833B0(0, 0, 0, item, scene->spriteHandle, 0x53);
    func_00263A00(scene);
    evtStageTestUpdate((s32)D_00325788);
}


s32 mnuAdvanceSkillPackageToItemPanel(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    mnuDrawItemPanelBackdrop(context);
    func_00263B78(context, 0);
    return menuRunPanel((s32)context, 1, request);
}

s32 mnuAdvanceSkillPanelToNextMenu(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    func_0024DD78();
    return menuRunPanel((s32)context, 2, request);
}

/* Bound the selection extent by the remaining capacity after five components. */
u32 mnuInitializeItemSelectionExtent(u64 unused) {
    s8 component;
    BrsSkillPackageWork *scene;
    s32 *slot;
    s8 *byteCursor;
    s32 remaining;
    s32 extent;
    s32 sum;

    scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    sum = 0;
    remaining = 4;
    extent = scene->selectedRewardRow->values.amount * 3;
    byteCursor = scene->selectedRewardRow->unit->baseStats;
    do {
        component = *byteCursor;
        byteCursor = byteCursor + 1;
        remaining = remaining - 1;
        sum = sum + component;
    } while (-1 < remaining);
    scene->assignedStatPoints = 0;
    remaining = 4;
    slot = &scene->statGains[4];
    if (0x1ef - sum < extent) {
        extent = 0x1ef - sum;
    }
    scene->availableStatPoints = extent;
    do {
        remaining = remaining - 1;
        *slot = 0;
        slot = slot + -1;
    } while (-1 < remaining);
    if (scene->selectionInitialized != 0) {
        mnuSetPanelGroupSelection(scene->panelHandle, 0);
    }
    return 1;
}

u32 func_00263E30(void) {
    return 1;
}

void mnuClearItemSelectionSlots(BrsSkillPackageWork *scene) {
    s32 remaining;
    s32 *slot;

    scene->assignedStatPoints = 0;
    slot = &scene->statGains[4];
    remaining = 4;
    do {
        remaining = remaining - 1;
        *slot = 0;
        slot = slot + -1;
    } while (-1 < remaining);
}

void mnuRefreshPartyUnitVitalsPanels(DatPartyRecord *unit, BrsSkillPackageWork *menu) {
    ptyRecomputeMaxVitals(unit, menu->statGains);
    mnuRefreshSelectedUnitPanels(unit, menu);
}

INCLUDE_SDATA(const s32, "game/code_00263148", D_003BC550);

