#include "mnu.h"
extern s32 mdlFlagTest(u32 flagId);

extern void mdlFlagSet(u32 flagId);

extern void dspSetActive(s32 index);

extern s32 dspStartEntry(s32 index);



extern u32 func_0029D790(u32, s32);

extern void func_0026C900(void);

extern u32 kwlnTaskGetUserValue();

extern void mnuMapPadMaskToFlags(s32);

extern void mnuTitleRenderFadeAndPanels(s32);

extern s32 brsTaskIsUiUpdateAllowed(s32);

extern void brsDecaySharedAnimCounter(s32);

typedef struct MenuItem {
    u8 pad00[4];
    u16 kind;
    u8 pad06[0x10];
    s8 components[5]; /* Summed when bounding the available selection extent. */
    u8 pad1B[0x3A];
    u8 selection;
} MenuItem;

/* The selected item and the multiplier used to derive its available extent. */
typedef struct MenuItemSelectionData {
    MenuItem *item;
    s32 extentFactor;
} MenuItemSelectionData;

typedef struct ProfileCapSkillList {
    u8 pad00[0x20];
    s32 count;
    u16 skillIds[8];
} ProfileCapSkillList;

typedef struct MenuItemScene {
    u8 pad00[4];
    u32 overlayFlags;
    u8 dispatchWork[0x4C];
    s32 dispatchState;
    u8 pad58[0x30];
    u16 unk88;
    u8 pad8A[0x12];
    MenuItemSelectionData *selectionData; /* 0x9C */
    u8 padA0[0x1C4];
    s32 resetStateA; /* 0x264 */
    s32 pendingSkillCount; /* 0x268: consumed by capped-skill message processing */
    s32 pendingSkillIndex;
    u32 selectionApplied; /* 0x270 */
    u8 pad274[0x78];
    s32 extentLimit;
    u8 pad2F0[0x78];
    s32 partyUnitCount; /* 0x368 */
    u8 pad36C[0x7C];
    u32 resetStateB; /* 0x3E8 */
    u8 pad3EC[8];
    s32 slots[5];
    u8 pad408[0xE0];
    ProfileCapSkillList skillList;
    u8 pad51C[0xA820];
    s32 extentExhausted; /* 0xAD3C: selects the exhausted-extent message. */
    u8 padAD40[0x9BC];
    u32 thresholdMessageShown;
} MenuItemScene;

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
extern s32 func_002C4038(s32, s32 *, u64, u64);
extern s32 evtGetMessageWindowControlState(void);
extern void func_00299B98(MenuItemScene *, s32);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void mnuSetPopupEntryFlagged(s32, s32);
extern s32 brsStartPartyPanelResourcesOnce(s32);
extern void func_00341CF8(void);
extern s32 btlHasPendingRuntimeActivity(void);
extern char D_003D64E4[];
extern char D_003D6458[];

INCLUDE_ASM(const s32, "game/code_00299D58", brsMessageInputStep);

s32 mnuStaffRunPanel1(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuTitleRenderFadeAndPanels(context);
        return menuSetHandler(context, 1, input);
    }
}

s32 mnuStaffRunPanel2(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        brsDecaySharedAnimCounter(context);
        return menuSetHandler(context, 2, input);
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
    return menuSetHandler(context, 0, input);
}

s32 mnuRefreshAndDispatchCurrentPanel(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, input);
}

s32 func_00299FA0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    return menuSetHandler(context, 2, input);
}

u32 func_00299FD8(void) {
    return 1;
}

u32 func_00299FE0(void) {
    return 1;
}

extern u32 *ptyGetCurrentProfileRecord(MenuItem *);
extern u32 ptyGetProfileRecordCap(u8);
extern s32 func_00314990(MenuItem *, u8);
extern void func_00314868(MenuItem *, u8);

/* Apply a capped selected profile and record whether the selection was applied. */
void mnuApplyCompletedProfile(u32 context) {
    MenuItemScene *state = (MenuItemScene *)context;
    MenuItem *item = state->selectionData->item;
    u32 *profile = ptyGetCurrentProfileRecord(item);

    if (item->selection != 0 &&
        ptyGetProfileRecordCap(item->selection) == *profile &&
        func_00314990(item, item->selection) == 0) {
        func_00314868(item, item->selection);
        state->selectionApplied = 1;
        state->overlayFlags = state->overlayFlags | 1;
        return;
    }
    state->selectionApplied = 0;
}

/* Build the capped skill list and cache its pending count; return 1. */
u32 mnuProcessItemSelection(u32 context) {
    u32 skillCount;
    MenuItemScene *scene;

    scene = (MenuItemScene *)context;
    skillCount = func_0029D790((u32)scene->selectionData->item,
                               (s32)&scene->skillList);
    scene->pendingSkillCount = skillCount;
    mnuApplyCompletedProfile(context);
    return 1;
}

typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

extern DspUnitName *D_00435E48;
extern DspUnitName *D_00435E64;
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 func_00314C10(s32);
extern s32 scrGetIndexedRecordAddress(u16, s32 *);

void prfCapPresentMessages(MenuItemScene *scene) {
    s32 message;
    MenuItem *item = scene->selectionData->item;
    s32 profileId = func_00314C10((s32)item);
    u16 skillId;

    if (scene->selectionApplied != 0) {
        evtCopyEntryStringToActiveWindow(
            0, (s32)D_00435E48[item->kind].encodedText);
        scrGetIndexedRecordAddress(profileId & 0xFFFF, &message);
        evtCopyEntryStringToActiveWindow(1, message);
        dspSetActive(1);
        dspStartEntry(item->kind + 3);
        scene->selectionApplied = 0;
    } else if (scene->pendingSkillCount > 0) {
        skillId = scene->skillList.skillIds[scene->pendingSkillIndex];
        evtCopyEntryStringToActiveWindow(
            0, (s32)D_00435E48[item->kind].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, (s32)D_00435E64[skillId].encodedText);
        dspSetActive(1);
        dspStartEntry(0);
        scene->pendingSkillIndex++;
        scene->pendingSkillCount--;
    }
}

u32 func_0029A1E0(MenuItemScene *scene) {
    return 0;
}

s32 mnuRequestContextLatchedSceneDsp(MenuItemScene *state)
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

s32 mnuRequestContextClearSceneDsp(MenuItemScene *state)
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
s32 mnuRequestWeightedProfileMessage(MenuItemScene *scene) {
    s32 profileIndex;
    u32 value;
    s32 rank;
    FieldResourceRecord *record;

    if (scene->thresholdMessageShown == 0) {
        profileIndex = func_001514A8();
        if (profileIndex == -1) {
            return 0;
        }
        value = scene->unk88 * D_00435E18[profileIndex].unk0E;
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
    MenuItemScene *scene = (MenuItemScene *)kwlnTaskGetUserValue(task);
    s32 *dispatchStatus = &scene->dispatchState;

    result = func_002C4038((s32)scene + 8, dispatchStatus, 0, (s32)task);
    if (result == 0) {
        if (*dispatchStatus == 0 &&
            (result = evtGetMessageWindowControlState(), result == 0)) {
            if (scene->resetStateA < scene->extentLimit ||
                scene->pendingSkillCount > 0 ||
                scene->selectionApplied != 0) {
                if ((scene->pendingSkillCount == 0 ||
                     scene->resetStateA == 0) &&
                    scene->selectionApplied == 0) {
                    func_00299B98(scene, 0);
                    mnuProcessItemSelection((u32)scene);
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
            if (scene->partyUnitCount == 0) {
                kwlnFadeInStart(0, 0, 0, 0xF);
                mnuSetPopupEntryFlagged((s32)dispatchStatus, (s32)D_003D64E4);
                return 0;
            }
            if (btlHasPendingRuntimeActivity() != 0) {
                return 0;
            }
            brsStartPartyPanelResourcesOnce((s32)scene);
            mnuSetPopupEntryFlagged((s32)dispatchStatus, (s32)D_003D6458);
            func_00341CF8();
        }
        result = 0;
    }
    return result;
}


s32 func_0029A588(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, request);
}

s32 func_0029A5D8(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(context, 2, request);
}

/* Clear the scene's two selection-processing markers; return 1. */
s32 mnuResetItemSelectionMarkers(void) {
    MenuItemScene *scene = (MenuItemScene *)kwlnTaskGetUserValue();
    scene->resetStateA = 0;
    scene->resetStateB = 0;
    return 1;
}

u32 func_0029A650(void) {
    return 1;
}

void func_0029A658(MenuItemScene *scene) {
    struct ActiveItemSlots {
        s32 indices[5];
        s32 values[5];
        s32 count;
    } active;
    MenuItem *item = scene->selectionData->item;
    s32 i;

    memset(&active, 0, sizeof(active));
    for (i = 0; i < 5; i++) {
        if (scene->slots[i] > 0) {
            active.indices[active.count] = i;
            active.values[active.count] = scene->slots[i];
            active.count++;
        }
    }
    evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[item->kind].encodedText);
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
extern void func_0029A658(MenuItemScene *);


/* Cap the available extent by the remaining capacity of five components. */
void func_0029A768(MenuItemScene *scene) {
    char text[16];
    MenuItem *item = scene->selectionData->item;
    s32 available = scene->selectionData->extentFactor * 3;
    s32 sum = 0;
    s32 i;
    u16 kind;

    for (i = 0; i < 5; i++) {
        sum += item->components[i];
    }
    if (495 - sum < available) {
        available = 495 - sum;
    }
    if (available == 0) {
        scene->extentExhausted = 1;
    } else {
        scene->extentExhausted = 0;
    }
    kind = item->kind;
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
extern void ptyAccumulateStatGains(s32 *, s32, u8 *);
extern s32 mnuAdvanceTitleEntryAnimation(u8 *);
extern void mnuStaffCopyPanelBlock(MenuItem *, MenuItemScene *);
extern void mnuRefreshSelectedUnitPanels(u32, s32);
/* The legacy call forwards its request; this initializer reads current task data. */
extern u32 mnuResetSelectionWidthsFromConfig();
extern void mnuSetPopupEntry(s32 *, void *);
extern s32 btlAddBaseStats(s32 *, MenuItem *);
extern void func_00342580(u32);
extern char D_003D6474[];
extern char D_003D64AC[];

/* Advance the selected party member, apply its gains, and choose the next popup. */
s32 func_0029A898(u64 request) {
    s32 result;
    MenuItemScene *scene = (MenuItemScene *)kwlnTaskGetUserValue();
    s32 *dispatchStatus;
    s32 *slots;
    MenuItem *item;

    evtStageTestUpdateCamera();
    if (evtGetMessageWindowControlState() != 0) {
        return 0;
    }
    dispatchStatus = &scene->dispatchState;
    result = func_002C4038((s32)scene->dispatchWork, dispatchStatus, 0, request);
    if (result != 0) {
        return result;
    }
    if (brsAdvanceSkillPackagePanel((s32)scene) != 0) {
        return 0;
    }
    if (*dispatchStatus == 0) {
        if (scene->resetStateA < scene->partyUnitCount) {
            func_00299B98(scene, 1);
            slots = scene->slots;
            ptyAccumulateStatGains(slots, scene->selectionData->extentFactor,
                                   (u8 *)scene->selectionData->item);
            func_0029A768(scene);
            mnuAdvanceTitleEntryAnimation((u8 *)scene->selectionData->item);
            mnuStaffCopyPanelBlock(scene->selectionData->item, scene);
            mnuRefreshSelectedUnitPanels((u32)scene->selectionData->item, (s32)scene);
            item = scene->selectionData->item;
            if (mnuKindIsSelectable(item->kind) != 0) {
                mnuResetSelectionWidthsFromConfig(request);
                if (scene->extentExhausted == 0) {
                    mnuSetPopupEntry(dispatchStatus, D_003D6474);
                    return 0;
                }
            } else {
                btlAddBaseStats(slots, item);
                mnuRefreshSelectedUnitPanels((u32)scene->selectionData->item, (s32)scene);
            }
            mnuSetPopupEntry(dispatchStatus, D_003D64AC);
        } else {
            func_00342580(0x50001);
            kwlnFadeInStart(0, 0, 0, 0xF);
            mnuSetPopupEntryFlagged((s32)dispatchStatus, (s32)D_003D64E4);
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);

