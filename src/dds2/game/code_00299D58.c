#include "sdk_ps2.h"
#include "fld_spark_reward_profile.h"
#include "snd_command_packet.h"
#include "evt_message_window_api.h"
#include "dsp_active_api.h"
#include "dsp_entry_api.h"
#include "mnu_input.h"
#include "kwln.h"
#include "mnu_reset_selection_widths_api.h"
#include "mnu.h"
#include "mnu_result.h"
#include "brs_result_counter_completion_api.h"
#include "brs_ui_update_allowed_api.h"
#include "dsp_name.h"
#include "mdl_flag_api.h"
#include "kwln_fade.h"
#include "prf_current_record_view.h"



extern u32 brsBuildProfileSkillList(datUnitWork_t *, PrfSkillList *);

extern void dspRequestMessageWindowModeOne(void);


extern void mnuTitleRenderFadeAndPanels(BrsSkillPackageWork *);

extern void brsDecaySharedAnimCounter(BrsSkillPackageWork *);


/* Reward/AP coefficients and scene-message thresholds share this 0xE0 record. */
typedef struct FieldResourceRecord {
    s16 category;
    s16 id;
    s16 unk04;
    u8 unk06[2];
    s16 unk08;
    s16 apMultiplier;
    s16 sparkPickupThresholdMultiplier;
    s16 mitamaThresholdMultiplier;
    u32 thresholds[4];
    u8 pad20[0xC0];
} FieldResourceRecord;

extern FieldResourceRecord *D_00435E18;
extern FldSparkRewardProfileIndex fldGetSparkRewardProfileIndex(void);
extern s16 fldGetSparkPickupCount(void);
extern void brsSelectNextUnit(BrsSkillPackageWork *, s32);
extern s32 brsStartPartyPanelResourcesOnce(BrsSkillPackageWork *);
extern s32 btlHasPendingRuntimeActivity(void);
extern MenuPopupEntry brsRewardFadeWaitPopupEntry[];
extern MenuPopupEntry brsPrimaryRewardPopupEntry[];

extern void brsNoOpMessageInputCallback(void *);
extern void mnuClearTitleState(BrsSkillPackageWork *);
extern u8 D_0037F530[];
extern u8 D_003D643C[];

s32 brsMessageInputStep(dds3ProcessID_t *input) {
    BrsSkillPackageWork *context;
    s32 *transitionState;
    s32 buttons;
    s32 result;

    context = (BrsSkillPackageWork *)dds3GetProcessData(input);
    transitionState = &context->transition.state;
    buttons = mnuMapPadMaskToFlags(0x33);
    if (brsTaskIsUiUpdateAllowed(context) == 0) {
        return 0;
    }
    result = mnuDispatchPopupTransition(&context->transition.data, transitionState, 0, input);
    if (result != 0) {
        return result;
    }
    if (*transitionState == 0) {
        brsNoOpMessageInputCallback(context);
        if (brsPollResultCounterCompletion() != BRS_RESULT_COUNTER_COMPLETION_PENDING &&
            ((buttons & 1) != 0 || (D_0037F530[3] & 2) != 0)) {
            mnuClearTitleState(context);
            mnuSetPopupEntry(transitionState, (MenuPopupEntry *)D_003D643C);
        }
    }
    return 0;
}

s32 mnuStaffRunPanel1(dds3ProcessID_t *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(task);

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuTitleRenderFadeAndPanels(context);
        return menuSetHandler(context, 1, (void *)task);
    }
}

s32 mnuStaffRunPanel2(dds3ProcessID_t *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(task);

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuSetHandler(context, 2, (void *)task);
    }
}

/* Registered result message enter hook: no side effects; returns one. */
u32 mnuResultMessageEnterNoOp(void) {
    return 1;
}

/* Registered result message leave hook: no side effects; returns one. */
u32 mnuResultMessageLeaveNoOp(void) {
    return 1;
}

s32 mnuStaffRunPanel0(dds3ProcessID_t *task) {
    s32 context = dds3GetProcessData(task);
    mnuMapPadMaskToFlags(0x33);
    return menuSetHandler((void *)context, 0, (void *)task);
}

s32 mnuRefreshAndDispatchCurrentPanel(dds3ProcessID_t *task) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(task);
    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, (void *)task);
}

s32 mnuAdvanceResultPanel(dds3ProcessID_t *input) {
    s32 context = dds3GetProcessData(input);
    return menuSetHandler((void *)context, 2, (void *)input);
}

/* Registered result selection enter hook: no side effects; returns one. */
u32 mnuResultSelectionEnterNoOp(void) {
    return 1;
}

/* Registered result selection leave hook: no side effects; returns one. */
u32 mnuResultSelectionLeaveNoOp(void) {
    return 1;
}

extern u32 ptyGetProfileRecordCap(u16);
extern s32 ptyTestPrimaryProfileFlag(datUnitWork_t *, u16);
extern void ptyApplyProfile(datUnitWork_t *, u16);

/* Apply a capped selected profile and record whether the selection was applied. */
void mnuApplyCompletedProfile(BrsSkillPackageWork *state) {
    datUnitWork_t *item = state->selectedRewardRow->unit;
    const DatProfileRecord *profile = ptyGetCurrentProfileRecord(item);

    if (item->profileId != 0 &&
        ptyGetProfileRecordCap(item->profileId) == profile->value &&
        ptyTestPrimaryProfileFlag(item, item->profileId) == 0) {
        ptyApplyProfile(item, item->profileId);
        state->selectionApplied = 1;
        state->overlayFlags = state->overlayFlags | 1;
        return;
    }
    state->selectionApplied = 0;
}

/* Build the capped skill list and cache its pending count; return 1. */
u32 mnuProcessItemSelection(BrsSkillPackageWork *scene) {
    u32 skillCount;
    skillCount = brsBuildProfileSkillList(scene->selectedRewardRow->unit, &scene->skillList);
    scene->pendingSkillCount = skillCount;
    mnuApplyCompletedProfile(scene);
    return 1;
}

extern DspUnitName *datUnitNames;
extern DspUnitName *datSkillNames;
extern void evtCopyEntryStringToActiveWindow(s32, const char *);
extern s32 ptyGetCurrentProfileId(const datUnitWork_t *);
extern u32 scrGetIndexedRecordAddress(u32, const u8 **);

void prfCapPresentMessages(BrsSkillPackageWork *scene) {
    const u8 *message;
    datUnitWork_t *item = scene->selectedRewardRow->unit;
    s32 profileId = ptyGetCurrentProfileId(item);
    u16 skillId;

    if (scene->selectionApplied != 0) {
        evtCopyEntryStringToActiveWindow(
            0, datUnitNames[item->unitId].encodedText);
        scrGetIndexedRecordAddress(profileId & 0xFFFF, &message);
        evtCopyEntryStringToActiveWindow(1, (const char *)message);
        dspSetActive(1);
        fclStartMessage(item->unitId + 3);
        scene->selectionApplied = 0;
    } else if (scene->pendingSkillCount > 0) {
        skillId = scene->skillList.skills[scene->pendingSkillIndex];
        evtCopyEntryStringToActiveWindow(
            0, datUnitNames[item->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, datSkillNames[skillId].encodedText);
        dspSetActive(1);
        fclStartMessage(0);
        scene->pendingSkillIndex++;
        scene->pendingSkillCount--;
    }
}

/* Extra profile-selection wait hook: retail never defers the follow-up work. */
u32 mnuShouldDeferProfileSelection(BrsSkillPackageWork *scene) {
    return 0;
}

s32 mnuRequestContextLatchedSceneDsp(BrsSkillPackageWork *state)
{
    if (evtBitCheck(0xB8F) == 0) {
        if (state->overlayFlags & 1) {
            if (evtBitCheck(0x915) != 0) {
                return 0;
            }
            dspSetActive(1);
            fclStartMessage(1);
            evtBitOn(0x915);
            state->overlayFlags |= 2;
            return 1;
        }
    }
    return 0;
}

s32 mnuRequestContextClearSceneDsp(BrsSkillPackageWork *state)
{
    if (evtBitCheck(0xB8F) == 0) {
        if (evtBitCheck(0x290) != 0) {
            if (state->overlayFlags & 2) {
                return 0;
            }
            if (evtBitCheck(0x817) != 0) {
                return 0;
            }
            dspSetActive(1);
            fclStartMessage(2);
            evtBitOn(0x817);
            return 1;
        }
        return 0;
    }
    return 0;
}

/* Request the weighted-profile message bucket only once. */
s32 mnuRequestWeightedProfileMessage(BrsSkillPackageWork *scene) {
    FldSparkRewardProfileIndex profileIndex;
    u32 value;
    s32 rank;
    FieldResourceRecord *record;

    if (scene->thresholdMessageShown == 0) {
        profileIndex = fldGetSparkRewardProfileIndex();
        if (profileIndex == FLD_SPARK_REWARD_PROFILE_INDEX_NONE) {
            return 0;
        }
        value = scene->rewards.mitama * D_00435E18[profileIndex].mitamaThresholdMultiplier;
        value += fldGetSparkPickupCount() * D_00435E18[profileIndex].sparkPickupThresholdMultiplier;
        record = &D_00435E18[profileIndex];
        rank = 0;
        while (rank < 4 && value < record->thresholds[rank]) {
            rank++;
        }
        dspSetActive(1);
        fclStartMessage(rank + 0x9E);
        scene->thresholdMessageShown = 1;
        return 1;
    }
    return 0;
}


s32 mnuDispatchProfileSelectionScene(dds3ProcessID_t *task) {
    s32 result;
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)dds3GetProcessData(task);
    s32 *dispatchStatus = &scene->transition.state;

    result = mnuDispatchPopupTransition(&scene->transition.data, dispatchStatus, 0, task);
    if (result == 0) {
        if (*dispatchStatus == 0 &&
            (result = fclChkMessage(), result == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL)) {
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
            if (mnuShouldDeferProfileSelection(scene) != 0) {
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
                dds3FadeOut(0, 0, 0, 0xF);
                mnuSetPopupEntryFlagged(dispatchStatus, brsRewardFadeWaitPopupEntry);
                return 0;
            }
            if (btlHasPendingRuntimeActivity() != 0) {
                return 0;
            }
            brsStartPartyPanelResourcesOnce(scene);
            mnuSetPopupEntryFlagged(dispatchStatus, brsPrimaryRewardPopupEntry);
            p2sdStopAllSe();
        }
        result = 0;
    }
    return result;
}


s32 mnuRefreshAndRunResultPanel(dds3ProcessID_t *request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)dds3GetProcessData(request);

    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, (void *)request);
}

s32 mnuRequestMessageWindowAndAdvanceResultPanel(dds3ProcessID_t *request) {
    s32 context = dds3GetProcessData(request);

    dspRequestMessageWindowModeOne();
    return menuSetHandler((void *)context, 2, (void *)request);
}

/* Clear the scene's two selection-processing markers; return 1. */
s32 mnuResetItemSelectionMarkers(dds3ProcessID_t *task) {
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)dds3GetProcessData(task);
    scene->selectedRow = 0;
    scene->resetStateB = 0;
    return 1;
}

/* Registered primary-reward panel leave hook: no side effects; returns one. */
u32 mnuPrimaryRewardPanelLeaveNoOp(void) {
    return 1;
}


void brsShowStatSelectionStatus(BrsSkillPackageWork *scene) {
    struct ActiveItemSlots {
        s32 indices[DAT_BASE_STAT_COUNT];
        s32 values[DAT_BASE_STAT_COUNT];
        s32 count;
    } active;
    datUnitWork_t *item = scene->selectedRewardRow->unit;
    s32 i;

    memset(&active, 0, sizeof(active));
    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
        if (scene->statGains[i] > 0) {
            active.indices[active.count] = i;
            active.values[active.count] = scene->statGains[i];
            active.count++;
        }
    }
    evtCopyEntryStringToActiveWindow(0, datUnitNames[item->unitId].encodedText);
    dspSetActive(1);
    if (scene->extentExhausted == 0) {
        fclStartMessage(0x15);
    } else {
        fclStartMessage(0x16);
    }
}

u32 mnuKindIsSelectable(u32 kind) {
    if (kind == 1) {
        return 1;
    }
    return (kind ^ 8) < 1;
}

extern char D_004379B0[];
extern void brsShowStatSelectionStatus(BrsSkillPackageWork *);


/* Cap the available extent by the remaining capacity of five components. */
void brsUpdateStatSelectionExtent(BrsSkillPackageWork *scene) {
    char text[16];
    datUnitWork_t *item = scene->selectedRewardRow->unit;
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
        evtCopyEntryStringToActiveWindow(0, datUnitNames[kind].encodedText);
        sprintf(text, D_004379B0, available);
        evtCopyEntryStringToActiveWindow(1, text);
        dspSetActive(1);
        if (scene->extentExhausted == 0) {
            fclStartMessage(0x13);
        } else {
            fclStartMessage(0x14);
        }
    } else {
        brsShowStatSelectionStatus(scene);
    }
}

extern void evtStageTestUpdateCamera(void);
extern s32 brsAdvanceSkillPackagePanel(BrsSkillPackageWork *);
extern void ptyAccumulateStatGains(s32 *, s32, const datUnitWork_t *);
extern s32 mnuAdvanceTitleEntryAnimation(datUnitWork_t *);
extern void mnuStaffCopyPanelBlock(datUnitWork_t *, BrsSkillPackageWork *);
extern void mnuRefreshSelectedUnitPanels(datUnitWork_t *, BrsSkillPackageWork *);
extern s32 btlAddBaseStats(s32 *, datUnitWork_t *);
extern void p2sdStopBGM(u32);
extern MenuPopupEntry brsStatAssignmentPopupEntry[];
extern MenuPopupEntry brsLevelChangePopupEntry[];

/* Advance the selected party member, apply its gains, and choose the next popup. */
s32 brsProcessNextPrimaryReward(dds3ProcessID_t *task) {
    s32 result;
    BrsSkillPackageWork *scene = (BrsSkillPackageWork *)dds3GetProcessData(task);
    s32 *dispatchStatus;
    s32 *slots;
    datUnitWork_t *item;

    evtStageTestUpdateCamera();
    if (fclChkMessage() != EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
        return 0;
    }
    dispatchStatus = &scene->transition.state;
    result = mnuDispatchPopupTransition(&scene->transition.data, dispatchStatus, 0, task);
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
            brsUpdateStatSelectionExtent(scene);
            mnuAdvanceTitleEntryAnimation(scene->selectedRewardRow->unit);
            mnuStaffCopyPanelBlock(scene->selectedRewardRow->unit, scene);
            mnuRefreshSelectedUnitPanels(scene->selectedRewardRow->unit, scene);
            item = scene->selectedRewardRow->unit;
            if (mnuKindIsSelectable(item->unitId) != 0) {
                mnuResetSelectionWidthsFromConfig(task);
                if (scene->extentExhausted == 0) {
                    mnuSetPopupEntry(dispatchStatus, brsStatAssignmentPopupEntry);
                    return 0;
                }
            } else {
                btlAddBaseStats(slots, item);
                mnuRefreshSelectedUnitPanels(scene->selectedRewardRow->unit, scene);
            }
            mnuSetPopupEntry(dispatchStatus, brsLevelChangePopupEntry);
        } else {
            p2sdStopBGM(0x50001);
            dds3FadeOut(0, 0, 0, 0xF);
            mnuSetPopupEntryFlagged(dispatchStatus, brsRewardFadeWaitPopupEntry);
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);
