#include "mnu.h"
#include "mnu_result.h"
extern s32 mdlFlagTest(u32 flagId);

extern void mdlFlagSet(u32 flagId);

extern void dspSetActive(s32 index);

extern s32 dspStartEntry(s32 index);



extern u32 func_0029D790(DatPartyRecord *, PrfSkillList *);

extern void func_0026C900(void);

extern u32 kwlnTaskGetUserValue();

extern s32 mnuMapPadMaskToFlags(s32);

extern void mnuTitleRenderFadeAndPanels(BrsSkillPackageWork *);

extern s32 brsTaskIsUiUpdateAllowed(s32);

extern void brsDecaySharedAnimCounter(BrsSkillPackageWork *);


/* Reward/AP coefficients and scene-message thresholds share this 0xE0 record. */
typedef struct FieldResourceRecord {
    s16 category;
    s16 id;
    s16 unk04;
    u8 unk06[2];
    s16 unk08;
    s16 apMultiplier;
    s16 unk0C;
    s16 unk0E;
    u32 thresholds[4];
    u8 pad20[0xC0];
} FieldResourceRecord;

extern FieldResourceRecord *D_00435E18;
extern s32 func_001514A8(void);
extern s16 func_001514B8(void);
extern s32 evtGetMessageWindowControlState(void);
extern void func_00299B98(BrsSkillPackageWork *, s32);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern s32 brsStartPartyPanelResourcesOnce(s32);
extern void func_00341CF8(void);
extern s32 btlHasPendingRuntimeActivity(void);
extern char D_003D64E4[];
extern char D_003D6458[];

extern void func_0029C878(void *);
extern void func_0029C860(BrsSkillPackageWork *);
extern s32 brsPollResultCounterCompletion(void);
extern u8 D_0037F530[];
extern u8 D_003D643C[];

s32 brsMessageInputStep(void *input) {
    BrsSkillPackageWork *context;
    s32 *window;
    s32 buttons;
    s32 result;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    window = &context->transition.state;
    buttons = mnuMapPadMaskToFlags(0x33);
    if (brsTaskIsUiUpdateAllowed((s32)context) == 0) {
        return 0;
    }
    result = func_002C4038(&context->transition, window, 0, input);
    if (result != 0) {
        return result;
    }
    if (*window == 0) {
        func_0029C878(context);
        if (brsPollResultCounterCompletion() != 0 &&
            ((buttons & 1) != 0 || (D_0037F530[3] & 2) != 0)) {
            func_0029C860(context);
            mnuSetPopupEntry(window, D_003D643C);
        }
    }
    return 0;
}

s32 mnuStaffRunPanel1(s32 input) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed((s32)context) != 0) {
        mnuTitleRenderFadeAndPanels(context);
        return menuSetHandler(context, 1, (void *)input);
    }
}

s32 mnuStaffRunPanel2(s32 input) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed((s32)context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuSetHandler(context, 2, (void *)input);
    }
}

u32 func_00299EF0(void) {
    return 1;
}

u32 func_00299EF8(void) {
    return 1;
}

s32 mnuStaffRunPanel0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuMapPadMaskToFlags(0x33);
    return menuSetHandler((void *)context, 0, (void *)input);
}

s32 mnuRefreshAndDispatchCurrentPanel(s32 input) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, (void *)input);
}

s32 func_00299FA0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler((void *)context, 2, (void *)input);
}

u32 func_00299FD8(void) {
    return 1;
}

u32 func_00299FE0(void) {
    return 1;
}

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);
extern s32 func_00314990(DatPartyRecord *, u16);
extern void func_00314868(DatPartyRecord *, u16);

/* Apply a capped selected profile and record whether the selection was applied. */
void mnuApplyCompletedProfile(BrsSkillPackageWork *state) {
    DatPartyRecord *item = state->selectedRewardRow->unit;
    DatProfileRecord *profile = ptyGetCurrentProfileRecord(item);

    if (item->profileId != 0 &&
        ptyGetProfileRecordCap(item->profileId) == profile->value &&
        func_00314990(item, item->profileId) == 0) {
        func_00314868(item, item->profileId);
        state->selectionApplied = 1;
        state->overlayFlags = state->overlayFlags | 1;
        return;
    }
    state->selectionApplied = 0;
}

/* Build the capped skill list and cache its pending count; return 1. */
u32 mnuProcessItemSelection(BrsSkillPackageWork *scene) {
    u32 skillCount;
    skillCount = func_0029D790(scene->selectedRewardRow->unit, &scene->skillList);
    scene->pendingSkillCount = skillCount;
    mnuApplyCompletedProfile(scene);
    return 1;
}

typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

extern DspUnitName *D_00435E48;
extern DspUnitName *D_00435E64;
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern s32 scrGetIndexedRecordAddress(u16, s32 *);

void prfCapPresentMessages(BrsSkillPackageWork *scene) {
    s32 message;
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 profileId = ptyGetCurrentProfileId(item);
    u16 skillId;

    if (scene->selectionApplied != 0) {
        evtCopyEntryStringToActiveWindow(
            0, (s32)D_00435E48[item->unitId].encodedText);
        scrGetIndexedRecordAddress(profileId & 0xFFFF, &message);
        evtCopyEntryStringToActiveWindow(1, message);
        dspSetActive(1);
        dspStartEntry(item->unitId + 3);
        scene->selectionApplied = 0;
    } else if (scene->pendingSkillCount > 0) {
        skillId = scene->skillList.skills[scene->pendingSkillIndex];
        evtCopyEntryStringToActiveWindow(
            0, (s32)D_00435E48[item->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, (s32)D_00435E64[skillId].encodedText);
        dspSetActive(1);
        dspStartEntry(0);
        scene->pendingSkillIndex++;
        scene->pendingSkillCount--;
    }
}

u32 func_0029A1E0(BrsSkillPackageWork *scene) {
    return 0;
}

s32 mnuRequestContextLatchedSceneDsp(BrsSkillPackageWork *state)
{
    if (mdlFlagTest(0xB8F) == 0) {
        if (state->overlayFlags & 1) {
            if (mdlFlagTest(0x915) != 0) {
                return 0;
            }
            dspSetActive(1);
            dspStartEntry(1);
            mdlFlagSet(0x915);
            state->overlayFlags |= 2;
            return 1;
        }
    }
    return 0;
}

s32 mnuRequestContextClearSceneDsp(BrsSkillPackageWork *state)
{
    if (mdlFlagTest(0xB8F) == 0) {
        if (mdlFlagTest(0x290) != 0) {
            if (state->overlayFlags & 2) {
                return 0;
            }
            if (mdlFlagTest(0x817) != 0) {
                return 0;
            }
            dspSetActive(1);
            dspStartEntry(2);
            mdlFlagSet(0x817);
            return 1;
        }
        return 0;
    }
    return 0;
}

/* Request the weighted-profile message bucket only once. */
s32 mnuRequestWeightedProfileMessage(BrsSkillPackageWork *scene) {
    s32 profileIndex;
    u32 value;
    s32 rank;
    FieldResourceRecord *record;

    if (scene->thresholdMessageShown == 0) {
        profileIndex = func_001514A8();
        if (profileIndex == -1) {
            return 0;
        }
        value = scene->rewards.mitama * D_00435E18[profileIndex].unk0E;
        value += func_001514B8() * D_00435E18[profileIndex].unk0C;
        record = &D_00435E18[profileIndex];
        rank = 0;
        while (rank < 4 && value < record->thresholds[rank]) {
            rank++;
        }
        dspSetActive(1);
        dspStartEntry(rank + 0x9E);
        scene->thresholdMessageShown = 1;
        return 1;
    }
    return 0;
}


s32 mnuDispatchProfileSelectionScene(void *task) {
    s32 result;
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue(task);
    s32 *dispatchStatus = &scene->transition.state;

    result = func_002C4038(&scene->transition, dispatchStatus, 0, task);
    if (result == 0) {
        if (*dispatchStatus == 0 &&
            (result = evtGetMessageWindowControlState(), result == 0)) {
            if (scene->selectedRow < scene->secondaryRewards.count ||
                scene->pendingSkillCount > 0 ||
                scene->selectionApplied != 0) {
                if ((scene->pendingSkillCount == 0 ||
                     scene->selectedRow == 0) &&
                    scene->selectionApplied == 0) {
                    func_00299B98(scene, 0);
                    mnuProcessItemSelection(scene);
                }
                prfCapPresentMessages(scene);
                return 0;
            }
            if (func_0029A1E0(scene) != 0) {
                return 0;
            }
            if (mnuRequestContextLatchedSceneDsp(scene) != 0) {
                return 0;
            }
            if (mnuRequestContextClearSceneDsp(scene) != 0) {
                return 0;
            }
            if (mnuRequestWeightedProfileMessage(scene) != 0) {
                return 0;
            }
            if (scene->primaryRewards.count == 0) {
                kwlnFadeInStart(0, 0, 0, 0xF);
                mnuSetPopupEntryFlagged(dispatchStatus, D_003D64E4);
                return 0;
            }
            if (btlHasPendingRuntimeActivity() != 0) {
                return 0;
            }
            brsStartPartyPanelResourcesOnce((s32)scene);
            mnuSetPopupEntryFlagged(dispatchStatus, D_003D6458);
            func_00341CF8();
        }
        result = 0;
    }
    return result;
}


s32 func_0029A588(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, (void *)request);
}

s32 func_0029A5D8(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((void *)context, 2, (void *)request);
}

/* Clear the scene's two selection-processing markers; return 1. */
s32 mnuResetItemSelectionMarkers(void) {
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    scene->selectedRow = 0;
    scene->resetStateB = 0;
    return 1;
}

u32 func_0029A650(void) {
    return 1;
}

void func_0029A658(BrsSkillPackageWork *scene) {
    struct ActiveItemSlots {
        s32 indices[DAT_BASE_STAT_COUNT];
        s32 values[DAT_BASE_STAT_COUNT];
        s32 count;
    } active;
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 i;

    memset(&active, 0, sizeof(active));
    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
        if (scene->statGains[i] > 0) {
            active.indices[active.count] = i;
            active.values[active.count] = scene->statGains[i];
            active.count++;
        }
    }
    evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[item->unitId].encodedText);
    dspSetActive(1);
    if (scene->extentExhausted == 0) {
        dspStartEntry(0x15);
    } else {
        dspStartEntry(0x16);
    }
}

u32 mnuKindIsSelectable(u32 kind) {
    if (kind == 1) {
        return 1;
    }
    return (kind ^ 8) < 1;
}

extern char D_004379B0[];
extern s32 func_0035C860(char *, const char *, ...);
extern void func_0029A658(BrsSkillPackageWork *);


/* Cap the available extent by the remaining capacity of five components. */
void func_0029A768(BrsSkillPackageWork *scene) {
    char text[16];
    DatPartyRecord *item = scene->selectedRewardRow->unit;
    s32 available = scene->selectedRewardRow->values.amount * 3;
    s32 sum = 0;
    s32 i;
    u16 kind;

    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
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
    kind = item->unitId;
    if (mnuKindIsSelectable(kind) != 0) {
        evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[kind].encodedText);
        func_0035C860(text, D_004379B0, available);
        evtCopyEntryStringToActiveWindow(1, (s32)text);
        dspSetActive(1);
        if (scene->extentExhausted == 0) {
            dspStartEntry(0x13);
        } else {
            dspStartEntry(0x14);
        }
    } else {
        func_0029A658(scene);
    }
}

extern void evtStageTestUpdateCamera(void);
extern s32 brsAdvanceSkillPackagePanel(s32);
extern void ptyAccumulateStatGains(s32 *, s32, DatPartyRecord *);
extern s32 mnuAdvanceTitleEntryAnimation(DatPartyRecord *);
extern void mnuStaffCopyPanelBlock(DatPartyRecord *, BrsSkillPackageWork *);
extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);
/* The legacy call forwards its request; this initializer reads current task data. */
extern u32 mnuResetSelectionWidthsFromConfig();
extern s32 btlAddBaseStats(s32 *, DatPartyRecord *);
extern void func_00342580(u32);
extern char D_003D6474[];
extern char D_003D64AC[];

/* Advance the selected party member, apply its gains, and choose the next popup. */
s32 func_0029A898(void *request) {
    s32 result;
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    s32 *dispatchStatus;
    s32 *slots;
    DatPartyRecord *item;

    evtStageTestUpdateCamera();
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    dispatchStatus = &scene->transition.state;
    result = func_002C4038(&scene->transition, dispatchStatus, 0, request);
    if (result != 0) {
        return result;
    }
    if (brsAdvanceSkillPackagePanel((s32)scene) != 0) {
        return 0;
    }
    if (*dispatchStatus == 0) {
        if (scene->selectedRow < scene->primaryRewards.count) {
            func_00299B98(scene, 1);
            slots = scene->statGains;
            ptyAccumulateStatGains(slots, scene->selectedRewardRow->values.amount,
                                   scene->selectedRewardRow->unit);
            func_0029A768(scene);
            mnuAdvanceTitleEntryAnimation(scene->selectedRewardRow->unit);
            mnuStaffCopyPanelBlock(scene->selectedRewardRow->unit, scene);
            mnuRefreshSelectedUnitPanels(scene->selectedRewardRow->unit, scene);
            item = scene->selectedRewardRow->unit;
            if (mnuKindIsSelectable(item->unitId) != 0) {
                mnuResetSelectionWidthsFromConfig(request);
                if (scene->extentExhausted == 0) {
                    mnuSetPopupEntry(dispatchStatus, D_003D6474);
                    return 0;
                }
            } else {
                btlAddBaseStats(slots, item);
                mnuRefreshSelectedUnitPanels(scene->selectedRewardRow->unit, scene);
            }
            mnuSetPopupEntry(dispatchStatus, D_003D64AC);
        } else {
            func_00342580(0x50001);
            kwlnFadeInStart(0, 0, 0, 0xF);
            mnuSetPopupEntryFlagged(dispatchStatus, D_003D64E4);
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);

