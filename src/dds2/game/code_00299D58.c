#include "mnu.h"
extern s32 mdlFlagTest(u32 flagId);

extern void mdlFlagSet(u32 flagId);

extern void dspSetActive(s32 index);

extern s32 dspStartEntry(s32 index);

/* DSP request bookkeeping lives in the menu scene's own state word. */
typedef struct MenuDspState {
    u8 pad00[4];
    u32 flags; /* 0x04 */
} MenuDspState;


extern u32 func_0029D790(u32, s32);

extern void func_0026C900(void);

extern s32 kwlnTaskGetUserValue();

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
    u8 pad08[0x94];
    MenuItemSelectionData *selectionData; /* 0x9C */
    u8 padA0[0x1C4];
    u32 resetStateA; /* 0x264 */
    s32 pendingSkillCount; /* 0x268: consumed by capped-skill message processing */
    s32 pendingSkillIndex;
    u32 selectionApplied; /* 0x270 */
    u8 pad274[0x174];
    u32 resetStateB; /* 0x3E8 */
    u8 pad3EC[0xFC];
    ProfileCapSkillList skillList;
    u8 pad51C[0xA820];
    s32 extentExhausted; /* 0xAD3C: selects the exhausted-extent message. */
} MenuItemScene;

INCLUDE_ASM(const s32, "game/code_00299D58", brsMessageInputStep);

s64 mnuStaffRunPanel1(s32 input) {
    s32 context = kwlnTaskGetUserValue();

    if (brsTaskIsUiUpdateAllowed(context) != 0) {
        mnuTitleRenderFadeAndPanels(context);
        return menuSetHandler(context, 1, input);
    }
}

s64 mnuStaffRunPanel2(s32 input) {
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

s64 mnuStaffRunPanel0(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuMapPadMaskToFlags(0x33);
    return menuSetHandler(context, 0, input);
}

s64 mnuRefreshAndDispatchCurrentPanel(s32 input) {
    s32 context = kwlnTaskGetUserValue();
    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, input);
}

s64 func_00299FA0(s32 input) {
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

u32 func_0029A1E0(void) {
    return 0;
}

s32 mnuRequestContextLatchedSceneDsp(MenuDspState *state)
{
    if (mdlFlagTest(0xB8F) == 0) {
        if (state->flags & 1) {
            if (mdlFlagTest(0x915) != 0) {
                return 0;
            }
            dspSetActive(1);
            dspStartEntry(1);
            mdlFlagSet(0x915);
            state->flags |= 2;
            return 1;
        }
    }
    return 0;
}

s32 mnuRequestContextClearSceneDsp(MenuDspState *state)
{
    if (mdlFlagTest(0xB8F) == 0) {
        if (mdlFlagTest(0x290) != 0) {
            if (state->flags & 2) {
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

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A2F8);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A400);

s64 func_0029A588(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    mnuTitleRenderFadeAndPanels(context);
    return menuSetHandler(context, 1, request);
}

s64 func_0029A5D8(s32 request) {
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

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A658);

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

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A898);

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);
