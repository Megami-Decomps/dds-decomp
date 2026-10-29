#include "mnu_variant_sprite_placement.h"
#include "common.h"
#include "mnu_mantra_sprite_draw.h"
#include "dsp_channel.h"
#include "mnu_title_stream.h"
#include "sdf_chip.h"
#include "eff_resource_slots.h"
#include "eff_resource_source_lifetime.h"
#include "kwln.h"
#include "kwln_process_registration.h"
#include "sdf_resource.h"
#include "eff.h"
#include "mnu_list.h"
#include "mnu_profile_progress.h"
#include "mnu_sprite_resource.h"
#include "prf_requirement.h"
#include "sdf_grid.h"
#include "sdf_task_work.h"
#include "mnu_scene_work.h"
#include "evt_stage_test_api.h"

enum { MNU_VARIANT_X_OFFSET = 2, MNU_VARIANT_Y_OFFSET, MNU_VARIANT_SPRITE_GROUPS };
extern EffectSlotSet *mnuMantraSpriteSlots[];
extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];
extern s32 mnuMantraVariantSpriteSlotView[];
extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, s32, s32, s32);
void mnuDrawVariantSpriteBySelector(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                  s32 selector, s32 flags, f32 scaleX, f32 scaleY, s32 context) {
    s8 spriteMap[15] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 17, 18 };
    s32 sprite;

    sprite = spriteMap[(mnuMantraVariantSpritePlacements[placementIndex][MNU_VARIANT_SPRITE_GROUPS] >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraVariantSpritePlacements[placementIndex]
                                           [MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + mnuMantraVariantSpritePlacements[placementIndex]
                                           [MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags, mnuMantraVariantSpriteSlotView[0], sprite, context);
}

void mnuDrawVariantSpriteAtCoordinatePlacement(s32 x, s32 y, s32 z, s32 alpha, s32 groupPlacementIndex,
                   s32 coordinatePlacementIndex, s32 selector, s32 flags,
                   f32 scaleX, f32 scaleY, s32 context) {
    s8 spriteMap[12] = { -1, 12, 26, 27, 13, 14, 15, 16, 20, 21, 23, 24 };
    s32 sprite;

    selector += 2;
    sprite = spriteMap[(mnuMantraVariantSpritePlacements[groupPlacementIndex][MNU_VARIANT_SPRITE_GROUPS] >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraVariantSpritePlacements[coordinatePlacementIndex]
                                              [MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + mnuMantraVariantSpritePlacements[coordinatePlacementIndex]
                                              [MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  mnuMantraVariantSpriteSlotView[0],
                  sprite,
                  context);
}

void mnuDrawScaledVariantSpriteByIndex(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 spriteIndex, s32 flags, f32 scaleX, f32 scaleY, s32 context) {
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[mnuMantraVariantSpritePlacements[placementIndex][0]];

        resource->workEntries[spriteIndex].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[spriteIndex].sourceWidth) << 4;
        resource->workEntries[spriteIndex].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[spriteIndex].sourceHeight) << 3;
    }
    effDrawSpriteWithCornerFade(
        (s32)((f32)(x + mnuMantraVariantSpritePlacements[placementIndex][MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
        (s32)((f32)(y + mnuMantraVariantSpritePlacements[placementIndex][MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
        z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
        (s32)mnuMantraSpriteSlots[5], spriteIndex, context);
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[mnuMantraVariantSpritePlacements[placementIndex][0]];

        resource->workEntries[spriteIndex].geometry.bounds[2] =
            resource->workEntries[spriteIndex].sourceWidth << 4;
        resource->workEntries[spriteIndex].geometry.bounds[3] =
            resource->workEntries[spriteIndex].sourceHeight << 3;
    }
}


#include "sdk_ps2.h"
#include "mnu_variant_sprite_placement.h"
#include "common.h"
#include "mnu_mantra_sprite_draw.h"
#include "dsp_channel.h"
#include "mnu_title_stream.h"
#include "sdf_chip.h"
#include "eff_resource_slots.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "eff.h"
#include "mnu_list.h"
#include "mnu_profile_progress.h"
#include "mnu_sprite_resource.h"
#include "prf_requirement.h"
#include "sdf_grid.h"
#include "sdf_task_work.h"
#include "mnu_scene_work.h"
#include "snd_sequence_volume_pan.h"
#include "mnu_mantra_grid.h"
#include "dsp_entry_api.h"
#include "evt_message_window_api.h"
#include "dsp_name.h"
#include "mnu.h"
#include "dat_state.h"
#include "mdl_flag_api.h"
#include "prf_profile_cap.h"
#include "prf_requirement_fallback.h"
#include "sdf_trig.h"

#define MNU_MANTRA_RESOURCE_SLOT_COUNT 14

#define MNU_MANTRA_SOURCE_ACTIVE_BIT 0x20

#define MNU_PARTY_RECORD_BYTES 0x1A4

#define MNU_GAME_PARTY_RECORD_OFFSET ((u32)&((DatGameState *)0)->party)

#define MNU_PARTY_ORDER_COUNT 32

#define MNU_VISIBLE_PARTY_COUNT 5

#define MNU_LIST_DRAW_VALUE_BYTES 8

#define MNU_RESOURCE_LIST_WORK_BYTES 0x10

#define MNU_SCENE_FADE_FRAMES 10.0f

#define MNU_SCENE_ALPHA_SCALE 128.0f

#define MNU_SCENE_FULL_ALPHA 0x80

#define MNU_REQUIREMENT_SLOT_COUNT 2

#define MNU_REQUIREMENT_MIN_COUNT 2

typedef u32 MnuProfileRequirementFlags;
enum {
    MNU_PROFILE_REQUIREMENT_FLAG_HAS_AT_LEAST_THREE_IDS = 1,
    MNU_PROFILE_REQUIREMENT_FLAG_RULE_PAIR_SET = 2
};

enum MnuMantraScenePhase {
    MNU_MANTRA_PHASE_INITIALIZE = 0,
    MNU_MANTRA_PHASE_WAIT_INITIAL_SHADE = 1,
    MNU_MANTRA_PHASE_DELAYED_TASK_DEACTIVATION = 2,
    MNU_MANTRA_PHASE_ACTIVE = 3,
    MNU_MANTRA_PHASE_FADE_THEN_ATTACH_TASK_A = 4,
    MNU_MANTRA_PHASE_FADE_THEN_ATTACH_TASK_B = 5,
    MNU_MANTRA_PHASE_WAIT_ENTRY_MESSAGE = 6,
    MNU_MANTRA_PHASE_FADE_OUT_RESOURCE_ANIMATION = 7,
    MNU_MANTRA_PHASE_RESET_RESOURCE_ANIMATION = 8,
    MNU_MANTRA_PHASE_WAIT_TASK_A_SHADE = 9,
    MNU_MANTRA_PHASE_START_PENDING_MESSAGE = 10,
    MNU_MANTRA_PHASE_WAIT_PENDING_MESSAGE = 11,
    MNU_MANTRA_PHASE_APPLY_PENDING_SELECTION = 12,
    MNU_MANTRA_PHASE_WAIT_TASK_B_SHADE = 13
};


extern s8 scrGetSelectedOperandIndex(struct datUnitWork_t *);

extern u32 ptyGetProfileRecordValue(const struct datUnitWork_t *, u16);

extern u32 prfGetCapValue(u16);

extern u8 mnuResourceTaskName[];

extern char D_003AF758[];

extern char D_003AF780[];

extern TaskWork *mnuSceneResourceContext;

extern EffectSlotSet *mnuMantraSpriteSlots[];

extern u8 mnuMantraResourceTaskDescriptor[];

extern void mnuDestroyMantraDrawPool(MnuSpriteResourceGroup *);

extern void mnuReleaseStaffMenuContextAndResources(MenuProgressHost *);

static inline void mnuFinalizeProgressHostVia(
        MenuProgressHost *work, void (*release)(MenuProgressHost *)) {
    release(work);
}

extern void mnuTitleStreamUpdateAndLogBgm(void);

extern MnuSpriteResourceGroup *mnuCreateSpriteResource(s32, u8, u8);

extern MenuProgressHost *mnuCreateWorkBlock(void);

typedef struct MnuResourceTaskWork {
    struct sdfMemHandle_t *allocation;
    u8 pad04[4];
    struct sdfMemHandle_t *messageResource1;
    u32 messageResourceInfo1;
    struct sdfMemHandle_t *messageResource2;
    u32 messageResourceInfo2;
    u8 pad18[0xC];
    MenuProgressHost *staffMenuContext;
    u8 pad28[0x210];
    MnuSpriteResourceGroup *drawPool;
    u8 pad23C[0xC];
} MnuResourceTaskWork;

void mnuRequestMantraResources(MnuResourceTaskWork *work);

/* Return one only when every mantra sprite resource slot is occupied. */
extern u8 mnuMantraSpriteResourceNames[];

extern void effRequestResourceByMode(char *, void *, s32, void *);

void mnuRequestMantraResources(MnuResourceTaskWork *work) {
    s32 i;

    for (i = 0; i < 14; i++) {
        if (mnuMantraSpriteSlots[i] == 0) {
            effRequestResourceByMode("/facility/spr/mantra/", &mnuMantraSpriteResourceNames[i * 0x10], EFF_RESOURCE_SOURCE_TRANSIENT, (u32 *)&mnuMantraSpriteSlots[i]);
        }
    }
}

s32 mnuAreResourceSlotsOccupied(MenuSceneMetadata *unusedMetadata) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_MANTRA_RESOURCE_SLOT_COUNT; ++slotIndex) {
        if (mnuMantraSpriteSlots[slotIndex] == 0) {
            return 0;
        }
    }
    return 1;
}

/* Release nonzero mantra sprite slots and clear their stored handles; the work argument is unused. */
void mnuReleaseResourceSlots(MnuResourceTaskWork *unusedWork) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_MANTRA_RESOURCE_SLOT_COUNT; ++slotIndex) {
        if (mnuMantraSpriteSlots[slotIndex] != 0) {
            effDestroyResourceSlotSet(mnuMantraSpriteSlots[slotIndex]);
            mnuMantraSpriteSlots[slotIndex] = 0;
        }
    }
}


/* Allocate and clear resource-task work, load both message resources and prepare the mantra visuals. */
MnuResourceTaskWork *mnuAllocateMantraResourceWork(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(sizeof(MnuResourceTaskWork));
    MnuResourceTaskWork *resourceWork = (MnuResourceTaskWork *)sdfMemGetPtr(allocation);

    memset(resourceWork, 0, sizeof(MnuResourceTaskWork));
    resourceWork->allocation = allocation;
    resourceWork->messageResource1 = sdfDevLoadFileEx(D_003AF758,
                                                  &resourceWork->messageResourceInfo1, 0);
    resourceWork->messageResource2 = sdfDevLoadFileEx(D_003AF780,
                                                  &resourceWork->messageResourceInfo2, 0);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    mnuQueueTitleSoundStreamEntry(0x10);
    mnuTitleStreamUpdateAndLogBgm();
    memset(mnuMantraSpriteSlots, 0, sizeof(mnuMantraSpriteSlots[0]) * MNU_MANTRA_RESOURCE_SLOT_COUNT);
    mnuRequestMantraResources(resourceWork);
    resourceWork->drawPool = mnuCreateSpriteResource(0x3C, 8, 0);
    if (resourceWork->staffMenuContext == NULL) {
        resourceWork->staffMenuContext = mnuCreateWorkBlock();
    }
    return resourceWork;
}

/* Release task-owned slots, visuals and message resources before freeing the backing allocation. */
void mnuReleaseMantraResourceWork(s32 unused, MnuResourceTaskWork *resourceWork) {
    if (resourceWork == NULL) {
        return;
    }
    mnuReleaseResourceSlots(resourceWork);
    mnuDestroyMantraDrawPool(resourceWork->drawPool);
    if (resourceWork->staffMenuContext != NULL) {
        mnuFinalizeProgressHostVia(resourceWork->staffMenuContext, mnuReleaseStaffMenuContextAndResources);
    }
    fclDestroyMessage();
    sdfMemFreeDelayedDirect(resourceWork->messageResource1);
    sdfMemFreeDelayedDirect(resourceWork->messageResource2);
    sdfMemFreeDirect(resourceWork->allocation);
    resourceWork = NULL;
}

/* The task handle is shared by the existence probe and explicit stop;
 * both clear it when the resource group is no longer active. */
void mnuCreateResourceTask(void) {
    MnuResourceTaskWork *resourceWork = mnuAllocateMantraResourceWork();
    mnuSceneResourceContext = sdfCreateTaskWorker(mnuResourceTaskName, 0x402, 0x2B12, (const SdfTaskItemDesc *)mnuMantraResourceTaskDescriptor, mnuReleaseMantraResourceWork, resourceWork);
}

/* Return whether the named resource task exists; invalidate the cached handle when it does not. */
s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(mnuResourceTaskName) != KWLN_PROCESS_NOT_REGISTERED) {
        return 1;
    }
    mnuSceneResourceContext = 0;
    return 0;
}

/* Destroy the cached resource-task group and clear its handle. */
void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(mnuSceneResourceContext);
    mnuSceneResourceContext = 0;
}

/* Pick the value of the first active slot, preferring slot 0. */
s32 mnuGetMantraSourceValue(u16 profileId) {
    s32 defaultValue = 0;
    PrfDds1RequirementRecord *sourceEntry = prfReqGetEntryRecord(profileId);
    s32 slotIndex = 0;

    if (sourceEntry->slots[0].operation & MNU_MANTRA_SOURCE_ACTIVE_BIT) {
        slotIndex = 0;
    } else if (sourceEntry->slots[1].operation & MNU_MANTRA_SOURCE_ACTIVE_BIT) {
        slotIndex = 1;
    } else {
        return defaultValue;
    }
    return sourceEntry->slots[slotIndex].minimum;
}

/* Populate a progress record from a party row's selected profile and its current/cap values. */
void mnuInitializeProfileProgress(u16 partyIndex, MnuProfileProgress *progress) {
    struct datUnitWork_t *partyRecord = &DDS3_GBWK->party[partyIndex];

    progress->partyRecord = partyRecord;
    progress->profileId = scrGetSelectedOperandIndex(partyRecord);
    progress->value = ptyGetProfileRecordValue(
        &DDS3_GBWK->party[partyIndex],
        progress->profileId);
    progress->cap = prfGetCapValue(progress->profileId);
}

/* Resource-task -> list -> selection chain used by the mantra display. */
typedef struct MnuResourceTask {
    struct sdfMemHandle_t *allocation;
    u32 state;
    u32 frameCounter;
    struct MenuList *menuList;
} MnuResourceTask;

enum MnuResourceTaskState {
    MNU_RESOURCE_TASK_STATE_WAIT_RESOURCES = 0,
    MNU_RESOURCE_TASK_STATE_WAIT_PROFILE_PANEL = 1,
    MNU_RESOURCE_TASK_STATE_WAIT_EXIT_FADE = 2,
    MNU_RESOURCE_TASK_STATE_WAIT_TITLE_STREAM = 3,
    MNU_RESOURCE_TASK_STATE_INPUT_READY = 4,
    MNU_RESOURCE_TASK_STATE_WAIT_REOPENED_PROFILE_PANEL = 5,
    MNU_RESOURCE_TASK_STATE_OPEN_INITIAL_PROFILE = 6,
    MNU_RESOURCE_TASK_STATE_REOPEN_SELECTED_PROFILE = 7,
    MNU_RESOURCE_TASK_STATE_RESET_TITLE_STREAM = 8,
};


extern void mnuDrawMantraPartyListEntry();


typedef char MnuResourceTask_size_must_be_0x10[(sizeof(MnuResourceTask) == 0x10) ? 1 : -1];

typedef char MnuResourceTask_menuList_offset_must_be_0x0C[
    ((u32)&((MnuResourceTask *)0)->menuList == 0x0C) ? 1 : -1];

struct MenuList;
extern struct MenuList * mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing);

/* Build party selections in unit-ID order from the five present rows.
 * i honestly serves first as a row index, then as the remaining order-table counter. */
void mnuBuildMantraPartyList(MnuResourceTask *task) {
    u16 partyOrder[MNU_PARTY_ORDER_COUNT];
    s32 i = 0;
    u16 *orderCursor;
    struct MenuList *list = mnuCreateListState(0, 6, 0x1A);

    list->drawCallback = mnuDrawMantraPartyListEntry;
    list->context = sdfChipAllocDirect(MNU_LIST_DRAW_VALUE_BYTES);
    memset(list->context, 0, MNU_LIST_DRAW_VALUE_BYTES);
    memset(partyOrder, 0, sizeof(partyOrder));
    do {
        s32 recordOffset = i * sizeof(datUnitWork_t) + MNU_GAME_PARTY_RECORD_OFFSET;
        datUnitWork_t *partyRecord = (datUnitWork_t *)((u8 *)DDS3_GBWK + recordOffset);
        u16 active = partyRecord->flags & 1;
        if (active != 0) {
            partyOrder[partyRecord->unitId] = i + 1;
        }
        i++;
    } while (i < MNU_VISIBLE_PARTY_COUNT);
    orderCursor = partyOrder;
    i = MNU_PARTY_ORDER_COUNT - 1;
    do {
        if (*orderCursor != 0) {
            struct MenuListNode *selectionNode = mnuListAppendNode(list, NULL);
            MnuProfileProgress *progress = sdfChipAllocDirect(sizeof(MnuProfileProgress));
            u16 partySlot = *orderCursor;

            selectionNode->userValue = (u32)progress;
            mnuInitializeProfileProgress(partySlot - 1, progress);
        }
        orderCursor++;
    } while (--i >= 0);
    task->menuList = list;
}

/* Return the selection record address used by labels and transition IDs. */
MnuProfileProgress *mnuGetSelectedNodeValue(void) {
    MnuResourceTask *task;

    task = (MnuResourceTask *)sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    return (MnuProfileProgress *)task->menuList->cursor->userValue;
}

/* Clear the list's two animation flags and request its default retreat. */
static inline struct MenuListNode *mnuMoveResourceListCursor(
        struct MenuList *list, struct MenuListNode *(*move)(struct MenuList *)) {
    return move(list);
}

extern void mnuClearListFlagsOneAndTwo(u32 *flags);

void mnuStopResourceAnimation(void) {
    s32 taskAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(&((MnuResourceTask *)taskAddress)->menuList->stateFlags);
    mnuMoveResourceListCursor((struct MenuList *)((MnuResourceTask *)taskAddress)->menuList, mnuRetreatListCursorDefault);
}

/* Clear the list's two animation flags and request its default advance. */
void mnuResetResourceAnimation(void) {
    s32 taskAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(&((MnuResourceTask *)taskAddress)->menuList->stateFlags);
    mnuMoveResourceListCursor((struct MenuList *)((MnuResourceTask *)taskAddress)->menuList, mnuAdvanceListCursorDefault);
}

/* Allocate the four-word list task work, construct its party list and clear both remaining words. */
MnuResourceTask *mnuAllocateEmptyResourceListState(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(MNU_RESOURCE_LIST_WORK_BYTES);
    MnuResourceTask *task = (MnuResourceTask *)sdfMemGetPtr(allocation);

    memset(task, 0, MNU_RESOURCE_LIST_WORK_BYTES);
    task->allocation = allocation;
    mnuBuildMantraPartyList(task);
    task->state = MNU_RESOURCE_TASK_STATE_WAIT_RESOURCES;
    task->frameCounter = 0;
    return task;
}

/* Tear down the linked resource nodes and release the task's allocation. */
void mnuReleaseResourceTaskData(s32 unused, MnuResourceTask *task) {
    struct MenuList *list = task->menuList;
    struct MenuListNode *nodeCursor = list->cursor;
    MenuSceneMetadata *sceneMetadata = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    while (nodeCursor != NULL) {
        sdfChipFreeDirect((void *)(u32)nodeCursor->userValue);
        nodeCursor = nodeCursor->next;
    }
    sdfChipFreeDirect(list->context);
    mnuDestroyListState(list);
    mnuFinalizeProgressHostVia(sceneMetadata->attachedEffect, mnuReleaseMenuVisualWorkResources);
    sdfMemFreeDirect(task->allocation);
    task = NULL;
}

extern s8 dds3PadButtonState[];

extern SdfTaskItemDesc D_0036C65C;

extern MnuProgressLoadPollResult mnuTickInitState(MenuProgressHost *);

extern void mnuCommitTitleStreamReadyState(void);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF758);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF780);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", mnuResourceTaskName);

s32 mnuUpdateResourceTaskState(s32 key, MnuResourceTask *task) {
    s32 action=0;
    struct MenuList *list=task->menuList;
    u32 taskState;
    MenuSceneMetadata *metadata=(MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext,-1);
    s32 frame;
    datUnitWork_t **selectedParty;
    datUnitWork_t *snapshot;
    taskState = task->state;
    switch (taskState) {
    case MNU_RESOURCE_TASK_STATE_WAIT_RESOURCES:
        if (mnuTickInitState(metadata->attachedEffect) == MNU_PROGRESS_LOAD_POLL_COMPLETE &&
            mnuAreResourceSlotsOccupied(metadata) == 1) {
            task->state = MNU_RESOURCE_TASK_STATE_OPEN_INITIAL_PROFILE;
        }
        return 0;
    case MNU_RESOURCE_TASK_STATE_OPEN_INITIAL_PROFILE:
        selectedParty=(datUnitWork_t **)(u32)list->cursor->userValue;
        task->state=MNU_RESOURCE_TASK_STATE_WAIT_PROFILE_PANEL;
        snapshot=&metadata->partySnapshot;
        *snapshot=**selectedParty;
        mnuSetupStaffMenuProfilePage(snapshot,metadata->attachedEffect);
        return 0;
    case MNU_RESOURCE_TASK_STATE_REOPEN_SELECTED_PROFILE:
        selectedParty=(datUnitWork_t **)(u32)list->cursor->userValue;
        task->state=MNU_RESOURCE_TASK_STATE_WAIT_REOPENED_PROFILE_PANEL;
        snapshot=&metadata->partySnapshot;
        *snapshot=**selectedParty;
        mnuSetupStaffMenuProfilePage(snapshot,metadata->attachedEffect);
        return 0;
    case MNU_RESOURCE_TASK_STATE_WAIT_PROFILE_PANEL:
    case MNU_RESOURCE_TASK_STATE_WAIT_REOPENED_PROFILE_PANEL:
        frame=(s32)task->frameCounter+1;
        task->frameCounter=frame;
        if ((f32)frame>=10.0f) {task->frameCounter=0;task->state=MNU_RESOURCE_TASK_STATE_INPUT_READY;}
        return 0;
    case MNU_RESOURCE_TASK_STATE_WAIT_EXIT_FADE:
        frame=(s32)task->frameCounter+1;
        task->frameCounter=frame;
        if ((f32)frame>=10.0f) {
            sdfAttachTaskItem(mnuSceneResourceContext,&D_0036C65C);
            sdfSetTaskItemMode(mnuSceneResourceContext, key, SDF_TASK_ITEM_MODE_SUSPENDED);
            task->state=MNU_RESOURCE_TASK_STATE_REOPEN_SELECTED_PROFILE;task->frameCounter=0;
            mnuReleaseMenuVisualWorkResources(metadata->attachedEffect);
        }
        return 0;
    case MNU_RESOURCE_TASK_STATE_WAIT_TITLE_STREAM: {
        s32 frames=(s32)task->frameCounter+1;
        task->frameCounter=frames;
        if(frames>=10) {task->state=MNU_RESOURCE_TASK_STATE_RESET_TITLE_STREAM;return 0;}
        break;
    }
    case MNU_RESOURCE_TASK_STATE_RESET_TITLE_STREAM:
        frame=(s32)task->frameCounter+1;
        task->frameCounter=frame;
        if (frame>=5) {mnuMarkTitleStreamResetPending();mnuResetTitleStreamLocked();return -1;}
        return 0;
    default:
        if (dds3PadButtonState[0x26]&2) {
            if (mnuMoveResourceListCursor(list, mnuRetreatListCursorDefault)!=NULL) {
                action=1;
                ((s32 (*)())mnuReleaseMenuVisualWorkResources)(metadata->attachedEffect);
                selectedParty=(datUnitWork_t **)(u32)list->cursor->userValue;
                snapshot=&metadata->partySnapshot;
                *snapshot=**selectedParty;
                mnuSetupStaffMenuProfilePage(snapshot,metadata->attachedEffect);
            }
        } else if (dds3PadButtonState[0x27]&2) {
            if (mnuMoveResourceListCursor(list, mnuAdvanceListCursorDefault)!=NULL) {
                action=1;
                ((s32 (*)())mnuReleaseMenuVisualWorkResources)(metadata->attachedEffect);
                selectedParty=(datUnitWork_t **)(u32)list->cursor->userValue;
                snapshot=&metadata->partySnapshot;
                *snapshot=**selectedParty;
                mnuSetupStaffMenuProfilePage(snapshot,metadata->attachedEffect);
            }
        } else if (dds3PadButtonState[0x21]<0) {
            action=2; task->frameCounter=0; task->state=MNU_RESOURCE_TASK_STATE_WAIT_EXIT_FADE;
        } else if (dds3PadButtonState[0x23]<0) {
            mnuCommitTitleStreamReadyState();
            task->state=MNU_RESOURCE_TASK_STATE_WAIT_TITLE_STREAM; task->frameCounter=0;
            action=3;
        }
        if ((dds3PadButtonState[0x26]==0) & (dds3PadButtonState[0x27]==0)) mnuClearListFlagsOneAndTwo(&list->stateFlags);
        if (action != 0) {
            switch (action) {
            case 1:
                p2sdPlaySE(0,0x7F,0x3F);
                break;
            case 2:
                p2sdPlaySE(8,0x7F,0x3F);
                break;
            case 3:
                p2sdPlaySE(10,0x7F,0x3F);
                break;
            }
        }
        return 0;
    }
    return 0;
}

extern void mnuDrawDisplaySpriteAndPanelMarks(MenuSceneMetadata *, s32, s32);

extern void mnuDrawMantraSceneSpriteSequence(s32, s32);

extern void mnuDrawMantraScenePlacementSprite(s32, s32, s32, s32, s32);

extern void mnuDrawMantraSceneSpriteGroup(s32, s32, s32, s32, s32);

struct DspDisplayModeWindowContext;
extern void mnuDrawDisplayModeSprites(s32, s32, s32, s32, struct DspDisplayModeWindowContext *, s32);

struct DspWindowContext;
extern void itfDspDrawSelectionStrip(s32, s32, s32, s32, struct DspWindowContext *, s32);

struct DspWindowContext;
extern void itfDspInitSelectedWindow(s32, s32, s32, s32, struct DspWindowContext *, s32);

struct DspTableWindowContext;
extern void mnuDrawSelectedMantraProfile(s32, s32, s32, s32, struct DspTableWindowContext *, s32);

/* Scene draw state: fade phase 1..5 and its frame counter. */
typedef struct MenuFadeWork {
    u8 pad00[4];
    s32 phase;      /* 0x04 */
    s32 timer;      /* 0x08 */
} MenuFadeWork;

/* Draw phase-specific sprite/window alpha and vertical offsets; every path returns zero.
 * Phases 1/5 use the timer ratio, 2/3 its inverse, and 4 full alpha. */
s32 mnuDrawMenuFadePhase(s32 unused, MenuFadeWork *work) {
    f32 shadeFactor;
    f32 elapsedFrames;
    f32 fadeRatio;
    s32 sceneMetadata = sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    s32 alpha;
    s32 elapsedTicks;

    switch (work->phase) {
    case 1:
        fadeRatio = (f32)work->timer / MNU_SCENE_FADE_FRAMES;
        shadeFactor = fadeRatio + fadeRatio;
        if (shadeFactor > 1.0f) {
            shadeFactor = 1.0f;
        }
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        mnuDrawDisplaySpriteAndPanelMarks((MenuSceneMetadata *)sceneMetadata, alpha, 0x52);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (struct DspDisplayModeWindowContext *)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - shadeFactor) * -24.0f), 1, alpha, (struct DspWindowContext *)work, 0x53);
        if (fadeRatio < 0.5f) {
            shadeFactor = 0.0f;
        } else {
            shadeFactor = (fadeRatio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (struct DspWindowContext *)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE), (struct DspTableWindowContext *)work, 0x53);
        return 0;
    case 5:
        fadeRatio = (f32)work->timer / MNU_SCENE_FADE_FRAMES;
        shadeFactor = fadeRatio + fadeRatio;
        if (shadeFactor > 1.0f) {
            shadeFactor = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks((MenuSceneMetadata *)sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (struct DspDisplayModeWindowContext *)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - shadeFactor) * -24.0f), 1, alpha, (struct DspWindowContext *)work, 0x53);
        if (fadeRatio < 0.5f) {
            shadeFactor = 0.0f;
        } else {
            shadeFactor = (fadeRatio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (struct DspWindowContext *)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE), (struct DspTableWindowContext *)work, 0x53);
        return 0;
    case 2:
        elapsedTicks = work->timer;
        fadeRatio = (f32)elapsedTicks / MNU_SCENE_FADE_FRAMES;
        if (elapsedTicks < 4) {
            shadeFactor = (f32)elapsedTicks * 0.25f;
        } else {
            shadeFactor = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks((MenuSceneMetadata *)sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        alpha = (s32)((1.0f - fadeRatio) * MNU_SCENE_ALPHA_SCALE);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (struct DspDisplayModeWindowContext *)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)(fadeRatio * 36.0f), 1, alpha, (struct DspWindowContext *)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)((1.0f - shadeFactor) * MNU_SCENE_ALPHA_SCALE), (struct DspWindowContext *)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, alpha, (struct DspTableWindowContext *)work, 0x53);
        return 0;
    case 3:
        elapsedTicks = work->timer;
        elapsedFrames = elapsedTicks;
        fadeRatio = elapsedFrames / MNU_SCENE_FADE_FRAMES;
        if (elapsedTicks < 4) {
            shadeFactor = elapsedFrames * 0.25f;
            shadeFactor = 1.0f - shadeFactor;
        } else {
            shadeFactor = 0.0f;
        }
        fadeRatio = 1.0f - fadeRatio;
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        mnuDrawDisplaySpriteAndPanelMarks((MenuSceneMetadata *)sceneMetadata, alpha, 0x52);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (struct DspDisplayModeWindowContext *)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - fadeRatio) * 36.0f), 1, alpha, (struct DspWindowContext *)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (struct DspWindowContext *)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, alpha, (struct DspTableWindowContext *)work, 0x53);
        return 0;
    case 4:
        mnuDrawDisplaySpriteAndPanelMarks((MenuSceneMetadata *)sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        mnuDrawMantraSceneSpriteSequence(MNU_SCENE_FULL_ALPHA, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, MNU_SCENE_FULL_ALPHA, (struct DspDisplayModeWindowContext *)work, 0x53);
        itfDspDrawSelectionStrip(0, 0, 1, MNU_SCENE_FULL_ALPHA, (struct DspWindowContext *)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, MNU_SCENE_FULL_ALPHA, (struct DspWindowContext *)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, MNU_SCENE_FULL_ALPHA, (struct DspTableWindowContext *)work, 0x53);
        break;
    case 0:
    case 6:
    case 7:
        break;
    }
    return 0;
}

/* Summarize the profile's qualifying ID list and paired requirement flags. */
MnuProfileRequirementFlags mnuGetProfileRequirementFlags(u16 profileId) {
    MnuProfileRequirementFlags resultFlags = 0;
    u8 *profileRecord = (u8 *)prfReqGetEntryRecord(profileId);
    s32 requirementWordIndex = 0;
    u8 *slotStatusBase = profileRecord + 0xC;
    s32 slotOffset = 0x20;
    s32 remainingSlots = MNU_REQUIREMENT_SLOT_COUNT - 1;

    do {
        if (*(u32 *)(slotStatusBase + slotOffset) & 1) {
            u8 *requirementsBase = profileRecord + 0x18;
            s32 requirementCount = 0;
            if (requirementsBase[slotOffset] != 0) {
                s32 requirementByteOffset = requirementWordIndex * sizeof(s32);
                u8 *requirementCursor =
                    (u8 *)(requirementByteOffset + (s32)requirementsBase + 0x20);
                do {
                    requirementCursor++;
                    requirementCount++;
                } while (*requirementCursor != 0);
            }
            if (requirementCount >= MNU_REQUIREMENT_MIN_COUNT) {
                resultFlags |= MNU_PROFILE_REQUIREMENT_FLAG_HAS_AT_LEAST_THREE_IDS;
            }
        }
        remainingSlots--;
        requirementWordIndex += 5;
        slotOffset += 0x14;
    } while (remainingSlots >= 0);
    if ((*(u32 *)(profileRecord + 4) & 4) != 0 && (*(u32 *)(profileRecord + 0x18) & 4) != 0) {
        resultFlags |= MNU_PROFILE_REQUIREMENT_FLAG_RULE_PAIR_SET;
    }
    return resultFlags;
}

/* Return the first status-exactly-one requirement list when summary bit 0 is set, otherwise NULL. */
u8 *mnuGetProfileRequirementIds(u16 profileId) {
    s32 slotIndex = 0;
    PrfDds1RequirementRecord *profileRecord;

    if ((mnuGetProfileRequirementFlags(profileId) &
         MNU_PROFILE_REQUIREMENT_FLAG_HAS_AT_LEAST_THREE_IDS) != 0) {
        profileRecord = prfReqGetEntryRecord(profileId);
        while (profileRecord->slots[slotIndex].operation != 1) {
            slotIndex++;
            if (slotIndex >= MNU_REQUIREMENT_SLOT_COUNT) {
                return NULL;
            }
        }
        return profileRecord->slots[slotIndex].profileIds;
    }
    return NULL;
}

/* Return whether the selected party row has capped both required profiles.
 * No qualifying requirement list is treated as satisfied; only its first two IDs are checked. */
s32 mnuAreRequiredProfilesCapped(u16 profileId) {
    MnuResourceTask *task = (MnuResourceTask *)sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    MnuProfileProgress *progress =
        (MnuProfileProgress *)task->menuList->cursor->userValue;
    u8 *requiredProfiles = mnuGetProfileRequirementIds(profileId);

    if (requiredProfiles == NULL) {
        return 1;
    }
    if (prfGetCapValue(requiredProfiles[0]) ==
        ptyGetProfileRecordValue(progress->partyRecord, requiredProfiles[0])) {
        if (prfGetCapValue(requiredProfiles[1]) ==
            ptyGetProfileRecordValue(progress->partyRecord, requiredProfiles[1])) {
            return 1;
        }
    }
    return 0;
}

extern SdfGrid *sdfGridCreate(s32, s32, s32, s32, s32, s32, void *, s32);

extern void sdfSetShortPairValues(SdfGrid *, s32, s32);

extern s32 mnuFreeTaskData(u32, u32);

extern void mnuDrawMantraEntryStatus(s32, s32, s32, SdfGrid *, SdfGridCell *, s32);

extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);

extern void mnuBuildMantraGridAndGetProfileCoordinates(MenuSceneWork *, MnuProfileProgress *, s32 *, s32 *);

extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *, u32, u32);

extern void mnuUpdateMantraSelectionPositions(MenuSceneWork *, s32);

/* Construct the selection grid and callbacks, select its initial coordinates, then reset the cached scroll position. */
void mnuInitializeMantraSelectionGrid(MenuSceneWork *sceneWork) {
    s32 selectedCoordinates[2];
    MnuResourceTask *resourceTask;
    MnuProfileProgress *selection;

    sceneWork->gridHandle = sdfGridCreate(0xF, 0x11, 0x40, 0x43, 4, 4,
                                          sceneWork->nodes, 0);
    sdfSetShortPairValues(sceneWork->gridHandle, 1, 1);
    sceneWork->gridHandle->releaseCell = mnuFreeTaskData;
    sceneWork->gridHandle->drawCell = mnuDrawMantraEntryStatus;
    resourceTask = (MnuResourceTask *)sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    selection = (MnuProfileProgress *)resourceTask->menuList->cursor->userValue;
    mnuBuildMantraGridAndGetProfileCoordinates(sceneWork, selection,
                  &selectedCoordinates[0], &selectedCoordinates[1]);
    if (sdfGridSelectFilledCell(sceneWork->gridHandle,
                                selectedCoordinates[0], selectedCoordinates[1]) == NULL) {
        sdfGridCursorLeftToFilledCell(sceneWork->gridHandle);
    }
    sceneWork->scrollX = 0;
    sceneWork->scrollY = 0;
    mnuUpdateMantraSelectionPositions(sceneWork, 1);
}

extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];
extern s8 dds3PadButtonState[64];
extern SdfGridCell *sdfGridCursorUpToFilledCell(SdfGrid *grid);
extern SdfGridCell *sdfGridCursorDownToFilledCell(SdfGrid *grid);
extern s32 mnuSelectMantraGridLeftNeighbor(SdfGrid *grid);
extern s32 mnuSelectMantraGridRightNeighbor(SdfGrid *grid);
extern MnuProfileProgress *mnuGetSelectedNodeValue(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void mnuReinitializeSceneGrid(s32);

void mnuBeginTransition(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendDisplayListNode(&work->sceneTransitionList);

    if (transition != NULL) {
        if (work->transitionBlendCounter >= 6) {
            mnuStopResourceAnimation();
            mnuReinitializeSceneGrid((s32)work);
        } else if (work->transitionBlendCounter < -5) {
            mnuResetResourceAnimation();
            mnuReinitializeSceneGrid((s32)work);
        }
        transition->payload.transition.mode = mode;
        if (mode == 1) {
            work->transitionBlendCounter = 10;
            transition->payload.transition.fromValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuStopResourceAnimation();
            transition->payload.transition.toValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->transitionBlendCounter = -10;
            transition->payload.transition.fromValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuResetResourceAnimation();
            transition->payload.transition.toValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuStopResourceAnimation();
        }
    }
}

void mnuHandleMantraGridInput(MenuSceneWork *work) {
    s32 feedback = 0;
    s8 direction;
    MnuMantraGridEntry *entry;

    if (dds3PadButtonState[0x26] != 0) {
        work->cursorInputMask = 0x80;
    } else if (dds3PadButtonState[0x27] != 0) {
        work->cursorInputMask = 0x40;
    } else if (dds3PadButtonState[0x24] != 0) {
        work->cursorInputMask = 0x20;
    } else if (dds3PadButtonState[0x25] != 0) {
        work->cursorInputMask = 0x10;
    } else {
        work->cursorInputMask = 0;
    }
    direction = work->cursorInputMask;
    if (work->cursorMoving == 0) {
        if ((dds3PadButtonState[0x26] & 2) && (direction & 0x80)) {
            if (sdfGridCursorUpToFilledCell(work->gridHandle) != NULL) {
                feedback = 1;
            }
        } else if ((dds3PadButtonState[0x27] & 2) && (direction & 0x40)) {
            if (sdfGridCursorDownToFilledCell(work->gridHandle) != NULL) {
                feedback = 1;
            }
        } else if ((dds3PadButtonState[0x24] & 2) && (direction & 0x20)) {
            if (mnuSelectMantraGridLeftNeighbor(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if ((dds3PadButtonState[0x25] & 2) && (direction & 0x10)) {
            if (mnuSelectMantraGridRightNeighbor(work->gridHandle) != 0) {
                feedback = 1;
            }
        }
    }
    if (dds3PadButtonState[0x21] < 0 && work->transitionBlendCounter == 0) {
        entry = (MnuMantraGridEntry *)work->gridHandle->cursor->value;
        if (entry != NULL &&
            work->entryPosition.x == mnuMantraVariantSpritePlacements[entry->sceneId][2] &&
            work->entryPosition.y == mnuMantraVariantSpritePlacements[entry->sceneId][3]) {
            if (entry->state == MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED) {
                feedback = 3;
            } else {
                feedback = 2;
                work->scenePhase = MNU_MANTRA_PHASE_FADE_THEN_ATTACH_TASK_A;
            }
        }
    } else if (dds3PadButtonState[0x23] < 0 && work->transitionBlendCounter == 0) {
        feedback = 3;
        work->scenePhase = MNU_MANTRA_PHASE_DELAYED_TASK_DEACTIVATION;
    } else if (dds3PadButtonState[0x28] < 0) {
        feedback = 4;
        mnuBeginTransition(work, 1);
    } else if (dds3PadButtonState[0x2A] < 0) {
        feedback = 4;
        mnuBeginTransition(work, 2);
    } else if (dds3PadButtonState[0x20] < 0 && work->transitionBlendCounter == 0) {
        feedback = 2;
        work->scenePhase = MNU_MANTRA_PHASE_FADE_THEN_ATTACH_TASK_B;
    }
    switch (feedback) {
    case 1:
        return p2sdPlaySE(2, 127, 63);
    case 2:
        return p2sdPlaySE(8, 127, 63);
    case 3:
        return p2sdPlaySE(10, 127, 63);
    case 4:
        p2sdPlaySE(4, 127, 63);
        break;
    }
}

/* Declared before first use (were implicit int). */

extern MnuProfileProgress *mnuGetSelectedNodeValue(void);
#define MNU_SCENE_WORK_SIZE 0x5B0
#define MNU_SCENE_SHADE_FRAME_LIMIT 10
#define MNU_SCENE_SHADE_FRAME_SCALE 10.0f
#define MNU_SCENE_SHADE_ALPHA_SCALE 112.0f
#define MNU_SCENE_DRAW_CONTEXT 0x53

extern void mnuUpdateMantraSelectionPositions(MenuSceneWork *, s32);

extern void mnuInitializeMantraSelectionGrid(MenuSceneWork *);

extern void mnuCopySceneCoordinates(MenuSceneWork *);

extern void sdfDestroyGridWork(SdfGrid *);

extern void mnuResetWorkFloats(void);


extern TaskWork *mnuSceneResourceContext;

typedef struct {
    u32 unk0;
    u16 entryX;
    u16 entryY;
    u32 unk8;
} SceneEntry;

extern SceneEntry mnuMantraEntrySpritePlacements[];

extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];

void mnuScrollSceneViewportToEntry(s32 *xCoordinate, s32 *yCoordinate, u16 index,
                   s32 maximumX, s32 maximumY) {
    s32 x = *xCoordinate;
    s32 deltaX = mnuMantraVariantSpritePlacements[index][2] - x;
    s32 deltaY = mnuMantraVariantSpritePlacements[index][3] - *yCoordinate;

    if (deltaX >= 0x13D) {
        *xCoordinate += deltaX - 0x13C;
        x = *xCoordinate;
    }
    if (maximumX < x) {
        *xCoordinate = maximumX;
    }
    if (deltaY >= 0x9C) {
        *yCoordinate += deltaY - 0x9B;
    }
    if (maximumY < *yCoordinate) {
        *yCoordinate = maximumY;
    }
    if (deltaX < 0x82) {
        /* Reload X after updating Y; the caller's coordinate pointers may alias. */
        s32 adjustedX = *xCoordinate + deltaX - 0x82;
        *xCoordinate = adjustedX < 0 ? 0 : adjustedX;
    }
    if (deltaY < 0x19) {
        s32 adjustedY = *yCoordinate + deltaY - 0x19;
        *yCoordinate = adjustedY < 0 ? 0 : adjustedY;
    }
}

#ifdef NON_MATCHING
#include "mnu_variant_sprite_placement.h"
#include "dsp_entry_api.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "dsp_name.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"
#include "sdf_task_work.h"
#include "sdf_chip.h"
#include "mdl_flag_api.h"
#include "prf_profile_cap.h"
#include "prf_requirement_fallback.h"
#include "common.h"
#include "sdf_trig.h"
#include "snd_sequence_volume_pan.h"
#include "sdk_libc.h"
extern void mnuScrollSceneViewportToEntry(s32 *xCoordinate, s32 *yCoordinate, u16 index, s32 maximumX, s32 maximumY);
extern float vecNormalize(float *vector);

extern s32 D_003BC420;
extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];
/* Advances the mantra scene view each frame: scrolls the viewport toward the
 * selected grid entry (with the per-scene horizontal limit from the requirement
 * flags), moves the scroll cursor and the entry marker toward their targets at
 * a ramping or fixed speed without overshooting, and records whether the marker
 * is still moving. Upstream: INCLUDE_ASM in game/code_00250E88; written from
 * the retail routine. It differs from retail only in register allocation and
 * scheduling. */
void mnuAdvanceMantraSceneScroll(MenuSceneWork *work) {
    SdfGrid *grid = work->gridHandle;
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
    s32 maximumX = 0x307;
    u8 flags = work->boundsFlags;
    f32 vector[4];
    s32 position[2];
    s32 deltaX;
    s32 deltaY;
    s32 step;
    s32 moving = 0;

    if ((flags & MENU_SCENE_REQUIREMENT_GROUP_2_MET) == 0) {
        maximumX = 0x2C8;
        if ((flags & MENU_SCENE_REQUIREMENT_GROUP_1_MET) == 0) {
            maximumX = (flags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) != 0 ? 0x24C : 0x1BE;
        }
    }
    position[0] = work->scrollX;
    position[1] = work->scrollY;
    mnuScrollSceneViewportToEntry(&position[0], &position[1], entry->sceneId, maximumX, 0x38E);
    step = D_003BC420 * 4 + 4;
    work->scrollX = position[0];
    if (step >= 0x15) {
        step = 0x14;
    }
    work->scrollY = position[1];

    memset(vector, 0, sizeof(vector));
    vector[0] = (f32)(position[0] - work->cursorPosition.x);
    vector[1] = (f32)(position[1] - work->cursorPosition.y);
    vecNormalize(vector);

    deltaX = position[0] - work->cursorPosition.x;
    if (deltaX != 0) {
        work->cursorPosition.x = (s32)((f32)work->cursorPosition.x + vector[0] * (f32)step);
        if (deltaX * (position[0] - work->cursorPosition.x) < 0) {
            work->cursorPosition.x = position[0];
        }
    }
    deltaY = position[1] - work->cursorPosition.y;
    if (deltaY != 0) {
        work->cursorPosition.y = (s32)((f32)work->cursorPosition.y + vector[1] * (f32)step);
        if (deltaY * (position[1] - work->cursorPosition.y) < 0) {
            work->cursorPosition.y = position[1];
        }
    }

    entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
    position[0] = mnuMantraVariantSpritePlacements[entry->sceneId][2];
    position[1] = mnuMantraVariantSpritePlacements[entry->sceneId][3];
    memset(vector, 0, sizeof(vector));
    vector[0] = (f32)(position[0] - work->entryPosition.x);
    vector[1] = (f32)(position[1] - work->entryPosition.y);
    vecNormalize(vector);

    deltaX = position[0] - work->entryPosition.x;
    if (deltaX != 0) {
        moving = 1;
        work->entryPosition.x = (s32)((f32)work->entryPosition.x + vector[0] * 20.0f);
        if (deltaX * (position[0] - work->entryPosition.x) < 0) {
            work->entryPosition.x = position[0];
        }
    }
    deltaY = position[1] - work->entryPosition.y;
    if (deltaY != 0) {
        moving = 1;
        work->entryPosition.y = (s32)((f32)work->entryPosition.y + vector[1] * 20.0f);
        if (deltaY * (position[1] - work->entryPosition.y) < 0) {
            work->entryPosition.y = position[1];
        }
    }

    if (moving != 0) {
        D_003BC420++;
    } else {
        D_003BC420 = 0;
    }
    work->cursorMoving = moving;
}
#else
INCLUDE_ASM(const s32, "game/code_0024F0D0", mnuAdvanceMantraSceneScroll);
#endif

void mnuUpdateMantraSceneScroll(MenuSceneWork *work) {
    MnuMantraGridEntry *entry =
        (MnuMantraGridEntry *)(u32)work->gridHandle->cursor->value;
    s32 maximumX = 0x307;
    u8 flags = work->boundsFlags;
    s32 position[2];

    if ((flags & MENU_SCENE_REQUIREMENT_GROUP_2_MET) == 0) {
        maximumX = 0x2C8;
        if ((flags & MENU_SCENE_REQUIREMENT_GROUP_1_MET) == 0) {
            maximumX = (flags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) != 0
                           ? 0x24C
                           : 0x1BE;
        }
    }
    position[0] = work->scrollX;
    position[1] = work->scrollY;
    mnuScrollSceneViewportToEntry(&position[0], &position[1], entry->sceneId, maximumX, 0x38E);
    work->scrollX = position[0];
    work->scrollY = position[1];
}

extern f32 vecNormalize(f32 *vector);
s32 D_003BC420 = 0;
s32 D_003BC424 = 0;

void mnuUpdateMantraSelectionPositions(MenuSceneWork *work, s32 mode) {
    SdfGrid *grid = work->gridHandle;
    MnuMantraGridEntry *entry;
    f32 vector[4];
    s32 targetX;
    s32 targetY;
    s32 deltaX;
    s32 deltaY;
    s32 candidate;
    s32 step;
    s32 moving = 0;

    mnuUpdateMantraSceneScroll(work);
    targetX = work->scrollX;
    targetY = work->scrollY;

    if (mode == 1) {
        work->cursorPosition.y = targetY;
        work->cursorPosition.x = targetX;
        entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
        targetX = mnuMantraVariantSpritePlacements[entry->sceneId][2];
        targetY = mnuMantraVariantSpritePlacements[entry->sceneId][3];
        work->entryPosition.x = targetX;
        work->entryPosition.y = targetY;
        return;
    }

    step = D_003BC424 * 10 + 4;
    if (step >= 0x2D) {
        step = 0x2C;
    }

    memset(vector, 0, sizeof(vector));
    vector[0] = (f32)(targetX - work->cursorPosition.x);
    vector[1] = (f32)(targetY - work->cursorPosition.y);
    vecNormalize(vector);

    deltaX = targetX - work->cursorPosition.x;
    if (deltaX != 0) {
        candidate = (s32)((f32)work->cursorPosition.x + vector[0] * (f32)step);
        work->cursorPosition.x = candidate;
        if (deltaX * (targetX - work->cursorPosition.x) < 0) {
            work->cursorPosition.x = targetX;
        }
    }
    deltaY = targetY - work->cursorPosition.y;
    if (deltaY != 0) {
        candidate = (s32)((f32)work->cursorPosition.y + vector[1] * (f32)step);
        work->cursorPosition.y = candidate;
        if (deltaY * (targetY - work->cursorPosition.y) < 0) {
            work->cursorPosition.y = targetY;
        }
    }

    entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
    targetX = mnuMantraVariantSpritePlacements[entry->sceneId][2];
    targetY = mnuMantraVariantSpritePlacements[entry->sceneId][3];
    memset(vector, 0, sizeof(vector));
    vector[0] = (f32)(targetX - work->entryPosition.x);
    vector[1] = (f32)(targetY - work->entryPosition.y);
    vecNormalize(vector);

    deltaX = targetX - work->entryPosition.x;
    if (deltaX != 0) {
        moving = 1;
        candidate = (s32)((f32)work->entryPosition.x + vector[0] * 40.0f);
        work->entryPosition.x = candidate;
        if (deltaX * (targetX - work->entryPosition.x) < 0) {
            work->entryPosition.x = targetX;
        }
    }
    deltaY = targetY - work->entryPosition.y;
    if (deltaY != 0) {
        moving = 1;
        candidate = (s32)((f32)work->entryPosition.y + vector[1] * 40.0f);
        work->entryPosition.y = candidate;
        if (deltaY * (targetY - work->entryPosition.y) < 0) {
            work->entryPosition.y = targetY;
        }
    }

    if (moving != 0) {
        D_003BC424++;
    } else {
        D_003BC424 = 0;
    }
    work->cursorMoving = moving;
}

/* Release a nonnull grid-cell payload; the cell index is unused. */
s32 mnuFreeTaskData(u32 unused, u32 taskData) {
    if (taskData != 0) {
        sdfChipFreeDirect((void *)taskData);
    }
}

extern void sdfGridGetCursorCoordinates(SdfGrid *, u32 *, u32 *);
extern u32 ptyGetProfileRecordValue(const datUnitWork_t *, u16);
extern const s32 D_003AF810[8];
extern const u16 D_003AF830[8];

s32 mnuScheduleUnlockedMantraMessages(MenuSceneWork *work) {
    s32 pendingFlagBases[7];
    u16 requirementIds[8];
    MnuProfileProgress *selection;
    s32 *flagBase;
    s32 i;
    u32 cap;
    u32 value;
    s32 pending = 0;

    memcpy(pendingFlagBases, D_003AF810, 0x1C);
    memcpy(requirementIds, D_003AF830, 0x10);
    /* The native clear includes 32 reserved bytes after the eight flags. */
    memset((u8 *)work + (u32)&((MenuSceneWork *)0)->pendingMantras, 0, 0x40);

    selection = mnuGetSelectedNodeValue();
    flagBase = &pendingFlagBases[selection->partyRecord->unitId];

    for (i = 0; i < 8; i++) {
        if (evtBitCheck(*flagBase + i) == 0 &&
            (prfCheckRequirementRules(0, selection->partyRecord,
                              requirementIds[i]) != 0 ||
             (i == 6 && evtBitCheck(0x908) != 0))) {
            cap = prfGetCapValue(requirementIds[i]);
            value = ptyGetProfileRecordValue(selection->partyRecord, requirementIds[i]);
            if (cap == value) {
                evtBitOn(*flagBase + i);
            } else {
                work->pendingMantras[i] = 1;
                evtBitOn(*flagBase + i);
                if (pending == 0) {
                    sdfGridGetCursorCoordinates(work->gridHandle,
                                                &work->pendingGridColumn,
                                                &work->pendingGridRow);
                    work->pendingCursorCoordinates.x = work->cursorPosition.x;
                    work->pendingCursorCoordinates.y = work->cursorPosition.y;
                }
                pending = 1;
            }
        }
    }

    return pending;
}


extern DspUnitName *D_003BAA70;
extern DspMantraName *D_003BAA78;
extern const s32 D_003AF840[];
extern const s32 D_003AF850[];
extern MnuMantraGridEntry *fldGetSceneMetadataNode(void);
extern void evtCopyEntryStringToActiveWindow(s32, const char *);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *, u32, u32);

/* Display the selected mantra and move the scene grid to its filled cell. */
s32 mnuDisplayNextPendingMantra(MenuSceneWork *work) {
    s16 mantraIds[8];
    s32 coordinates[8][2];
    MnuProfileProgress *selection;
    s32 *pendingFlags;
    s32 i;
    SdfGrid *grid;
    s32 x;
    s32 y;

    memcpy(mantraIds, D_003AF840, sizeof(mantraIds));
    memcpy(coordinates, D_003AF850, sizeof(coordinates));
    selection = mnuGetSelectedNodeValue();
    fldGetSceneMetadataNode();
    pendingFlags = work->pendingMantras;
    for (i = 0; i < 8; i++) {
        if (pendingFlags[i] != 0) {
            evtCopyEntryStringToActiveWindow(
                0, D_003BAA70[selection->partyRecord->unitId].encodedText);
            evtCopyEntryStringToActiveWindow(
                1, D_003BAA78[mantraIds[i]].encodedText);
            fclStartMessage(1);
            pendingFlags[i] = 0;
            grid = work->gridHandle;
            y = coordinates[i][1];
            x = coordinates[i][0];
            sdfGridSelectFilledCell(grid, x, y);
            return 0;
        }
    }
    return 1;
}



/* Allocate and clear scene work before registering its grid and coordinates. */
MenuSceneWork *mnuCreateSceneWork(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(MNU_SCENE_WORK_SIZE);
    u8 *sceneWork = (u8 *)sdfMemGetPtr(allocation);

    memset(sceneWork, 0, MNU_SCENE_WORK_SIZE);
    ((MenuSceneWork *)sceneWork)->allocation = allocation;
    mnuInitializeMantraSelectionGrid((MenuSceneWork *)sceneWork);
    ((MenuSceneWork *)sceneWork)->scenePhase = MNU_MANTRA_PHASE_INITIALIZE;
    ((MenuSceneWork *)sceneWork)->phaseFrame = 0;
    ((MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY))->displayedCurrency = DDS3_GBWK->header.currency;
    mnuCopySceneCoordinates((MenuSceneWork *)sceneWork);
    return (MenuSceneWork *)sceneWork;
}

/* Retain the native metadata lookup, then release grid/list/allocation resources and reset projection state. */
void mnuReleaseSceneContext(s32 unused, MenuSceneWork *scene) {
    sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    sdfDestroyGridWork(scene->gridHandle);
    mnuReleaseDisplayListNodes(&scene->sceneTransitionList);
    sdfMemFreeDirect(scene->allocation);
    mnuResetWorkFloats();
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    scene = NULL; /* the scene work was released above */
}

extern SdfTaskItemDesc D_0036C670;
extern SdfTaskItemDesc D_0036C684;
extern void evtStageTestInit(s32);
extern void btlStopStage(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void mnuHandleMantraGridInput(MenuSceneWork *);
extern void mnuReinitializeSceneGrid(s32);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF850);

s32 mnuUpdateMantraSceneTaskState(s32 key, MenuSceneWork *work) {
    MenuSceneMetadata *metadata;
    s32 blend;
    u16 x;
    u16 y;

    metadata = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext,
                                                       SDF_TASK_VALUE_USER_DATA_KEY);
    switch (work->scenePhase) {
    case MNU_MANTRA_PHASE_INITIALIZE:
        evtStageTestInit(0);
        work->hasPendingUnlockedMantraMessages = mnuScheduleUnlockedMantraMessages(work);
        work->scenePhase = MNU_MANTRA_PHASE_WAIT_INITIAL_SHADE;
        return 0;
    case MNU_MANTRA_PHASE_APPLY_PENDING_SELECTION:
        work->phaseFrame++;
        if (work->phaseFrame >= 6) {
            sdfGridSelectFilledCell(work->gridHandle, work->pendingGridColumn,
                                    work->pendingGridRow);
            mnuUpdateMantraSelectionPositions(work, 1);
            x = work->pendingCursorCoordinates.x;
            y = work->pendingCursorCoordinates.y;
            work->scrollX = x;
            work->scrollY = y;
            work->cursorPosition.x = x;
            work->cursorPosition.y = y;
            if (work->pendingPhase13) {
                work->pendingPhase13 = 0;
                work->scenePhase = MNU_MANTRA_PHASE_WAIT_TASK_B_SHADE;
                work->phaseFrame = 0;
                sdfAttachTaskItem(mnuSceneResourceContext, &D_0036C684);
                sdfSetTaskItemMode(mnuSceneResourceContext, 1,
                                   SDF_TASK_ITEM_MODE_SUSPENDED);
                return 0;
            } else {
                work->phaseFrame = 0;
                work->scenePhase = MNU_MANTRA_PHASE_WAIT_INITIAL_SHADE;
            }
        }
        break;
    case MNU_MANTRA_PHASE_START_PENDING_MESSAGE:
        fclDestroyMessage();
        fclInitMessage(metadata->entryMessageWindowResource);
        if (mnuDisplayNextPendingMantra(work)) {
            fclDestroyMessage();
            work->phaseFrame = 0;
            work->hasPendingUnlockedMantraMessages = 0;
            work->scenePhase = MNU_MANTRA_PHASE_APPLY_PENDING_SELECTION;
            return 0;
        } else {
            work->scenePhase = MNU_MANTRA_PHASE_WAIT_PENDING_MESSAGE;
        }
        break;
    case MNU_MANTRA_PHASE_WAIT_PENDING_MESSAGE:
        if (work->phaseFrame < 10) {
            work->phaseFrame++;
        }
        mnuUpdateMantraSelectionPositions(work, 0);
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            work->scenePhase = MNU_MANTRA_PHASE_START_PENDING_MESSAGE;
            evtFinishMessageWindowAndNotify();
        }
        return 0;
    case MNU_MANTRA_PHASE_DELAYED_TASK_DEACTIVATION:
        work->phaseFrame++;
        if (work->phaseFrame >= 10) {
            sdfSetTaskItemMode(mnuSceneResourceContext, 0,
                               SDF_TASK_ITEM_MODE_PENDING_ACTIVATION);
            return -1;
        }
        break;
    case MNU_MANTRA_PHASE_FADE_THEN_ATTACH_TASK_A:
        work->phaseFrame++;
        if (work->phaseFrame > MNU_SCENE_SHADE_FRAME_LIMIT) {
            work->phaseFrame = 0;
            work->scenePhase = MNU_MANTRA_PHASE_WAIT_TASK_A_SHADE;
            sdfAttachTaskItem(mnuSceneResourceContext, &D_0036C670);
            sdfSetTaskItemMode(mnuSceneResourceContext, 1,
                               SDF_TASK_ITEM_MODE_SUSPENDED);
        }
        return 0;
    case MNU_MANTRA_PHASE_FADE_THEN_ATTACH_TASK_B:
        work->phaseFrame++;
        if (work->phaseFrame > MNU_SCENE_SHADE_FRAME_LIMIT) {
            work->phaseFrame = 0;
            work->scenePhase = MNU_MANTRA_PHASE_WAIT_TASK_B_SHADE;
            sdfAttachTaskItem(mnuSceneResourceContext, &D_0036C684);
            sdfSetTaskItemMode(mnuSceneResourceContext, 1,
                               SDF_TASK_ITEM_MODE_SUSPENDED);
        }
        return 0;
    case MNU_MANTRA_PHASE_WAIT_INITIAL_SHADE:
    case MNU_MANTRA_PHASE_WAIT_TASK_A_SHADE:
    case MNU_MANTRA_PHASE_WAIT_TASK_B_SHADE:
        work->phaseFrame++;
        if (work->phaseFrame > MNU_SCENE_SHADE_FRAME_LIMIT) {
            work->phaseFrame = 0;
            work->scenePhase = MNU_MANTRA_PHASE_ACTIVE;
        }
        break;
    case MNU_MANTRA_PHASE_WAIT_ENTRY_MESSAGE:
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            if (work->entryMessageInProgress == 0) {
                fclDestroyMessage();
                fclInitMessage(metadata->entryMessageWindowResource);
                fclStartMessage(0);
                work->entryMessageInProgress = 1;
            } else {
                work->scenePhase = MNU_MANTRA_PHASE_ACTIVE;
                evtFinishMessageWindowAndNotify();
                work->entryMessageInProgress = 0;
            }
        }
        return 0;
    case MNU_MANTRA_PHASE_FADE_OUT_RESOURCE_ANIMATION:
        blend = work->transitionBlendCounter;
        if (blend > 0) {
            blend = --work->transitionBlendCounter;
        } else {
            work->scenePhase = MNU_MANTRA_PHASE_ACTIVE;
        }
        if (blend != 5) {
            break;
        }
        mnuStopResourceAnimation();
        goto reinitializeScene;
    case MNU_MANTRA_PHASE_RESET_RESOURCE_ANIMATION:
        blend = work->transitionBlendCounter;
        if (blend > 0) {
            blend = --work->transitionBlendCounter;
        } else {
            work->scenePhase = MNU_MANTRA_PHASE_ACTIVE;
        }
        if (blend != 5) {
            break;
        }
        mnuResetResourceAnimation();
        /* Native251D40 forwards to251D80; this is a shared action, not the epilogue. */
    reinitializeScene:
        mnuReinitializeSceneGrid((s32)work);
        return 0;
    case MNU_MANTRA_PHASE_ACTIVE:
        if (work->hasPendingUnlockedMantraMessages) {
            work->scenePhase = MNU_MANTRA_PHASE_START_PENDING_MESSAGE;
        }
        if (work->transitionBlendCounter > 0) {
            work->transitionBlendCounter--;
            if (work->transitionBlendCounter == 5) {
                btlStopStage();
                mnuStopResourceAnimation();
                mnuReinitializeSceneGrid((s32)work);
                work->hasPendingUnlockedMantraMessages = mnuScheduleUnlockedMantraMessages(work);
                if (work->hasPendingUnlockedMantraMessages) {
                    work->transitionBlendCounter = 0;
                }
            }
        } else if (work->transitionBlendCounter < 0) {
            work->transitionBlendCounter++;
            if (work->transitionBlendCounter == -5) {
                btlStopStage();
                mnuResetResourceAnimation();
                mnuReinitializeSceneGrid((s32)work);
                work->hasPendingUnlockedMantraMessages = mnuScheduleUnlockedMantraMessages(work);
                if (work->hasPendingUnlockedMantraMessages) {
                    work->transitionBlendCounter = 0;
                }
            }
        }
        mnuHandleMantraGridInput(work);
        mnuAdvanceMantraSceneScroll(work);
        break;
    }
    return 0;
}

/* Draw the mantra scene layers for the current task phase. */
s32 mnuDrawMantraSceneFrame(s32 key, MenuSceneWork *work) {
    extern void mnuDrawPlacedSpriteFromTable(s32, s32, s32, s32, s32, s32);
    extern void mnuDrawMantraSparkScene(MenuSceneMetadata *, s32, s32);
    extern void mnuDrawMantraSceneBackground(MenuSceneMetadata *, s32, s32);
    extern void mnuDrawMantraPulseFrame(s32, s32, s32, s32, MenuSceneWork *, s32);
    extern void itfDspDrawMarksA(s32, s32);
    extern void itfDspDrawMarksB(s32, s32);
    extern void mnuDrawAnimatedCurrencyCounter(s32, s32, s32, s32, MenuSceneMetadata *, s32);
    extern void mnuDrawMantraGridPulseEntry(s32, s32, s32, MenuSceneWork *, s32, s32);
    extern s32 itfAdvanceDisplayList(MnuSceneListHead *, s32, s32);
    extern void itfDspDrawPartyRecordWithLevel(s32, s32, s32, datUnitWork_t *, s32, s32);
    extern void mnuDrawMantraNavigationPulses(MenuSceneWork *, s32, s32);
    extern void mnuDrawMantraPulseStripAndKind(MenuSceneWork *, s32, s32);
    extern void mnuDrawSelectedMantraEntry(MenuSceneWork *, s32, s32);
    extern void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *, s32, s32);
    extern void dspRequestMessageWindowModeOne(void);
    MenuSceneMetadata *metadata;
    MnuSceneListHead *pendingDisplayList;
    f32 weight = 0.0f;
    f32 entryWeight;
    f32 stripWeight;
    s32 alpha;
    s32 entryAlpha;

    eglSetOt(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 2, 128, 3, 0, 0, 1, 1);
    mnuGetSelectedNodeValue();
    metadata = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, -1);
    dspRequestMessageWindowModeOne();
    switch (work->scenePhase) {
    case 0:
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        break;
    case 1:
        if (work->phaseFrame < 7) entryWeight = 0.0f;
        else entryWeight = (work->phaseFrame - 7) / 3.0f;
        weight = work->phaseFrame / 5.0f;
        if (weight > 1.0f) weight = 1.0f;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        mnuDrawMantraSceneBackground(metadata, (s32)(weight * 128.0f), 0x52);
        if (work->phaseFrame < 5) weight = 0.0f;
        else weight = (work->phaseFrame - 5) / 5.0f;
        mnuDrawPlacedSpriteFromTable(0, 0, 0, 128, 0x5a, 0x52);
        entryAlpha = (s32)(entryWeight * 128.0f);
        mnuDrawMantraPulseFrame(0, 0, 1, entryAlpha, work, 0x52);
        itfDspDrawMarksA(128, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 128, 0x53);
        itfDspDrawMarksB((s32)(weight * 128.0f), 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, (s32)(weight * 128.0f), metadata, 0x53);
        mnuDrawMantraGridPulseEntry((s32)((1.0f - weight) * 64.0f), 0, 0, work, entryAlpha, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, (s32)(weight * 128.0f), 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, (s32)(weight * 128.0f), 0x53);
        }
        mnuDrawMantraNavigationPulses(work, (s32)(weight * 128.0f), 0x53);
        break;
    case 2:
        weight = work->phaseFrame / 10.0f;
        weight = 1.0f - weight;
        /* Copied-case trace exception, not recovered original source: this
         * phase-1 entry channel is unused during the outgoing fade. Qualified
         * cc1 traces delete its values in 13.life, but keep its integer test
         * through combine/allocation until 21.flow2. This retains retail's
         * 002520D8 lw / 002520EC mtc1 and the separate unit-constant CSE regions.
         * See docs/matching/mantra-scene-frame.md. */
        if (work->phaseFrame < 7) entryWeight = 0.0f;
        else entryWeight = (work->phaseFrame - 7) / 3.0f;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        alpha = (s32)(weight * 128.0f);
        mnuDrawMantraSceneBackground(metadata, alpha, 0x52);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, 128, 0x5a, 0x52);
        mnuDrawMantraPulseFrame(0, 0, 1, alpha, work, 0x52);
        itfDspDrawMarksA(128, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 128, 0x53);
        itfDspDrawMarksB(alpha, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, alpha, metadata, 0x53);
        mnuDrawMantraGridPulseEntry((s32)((1.0f - weight) * 64.0f), 0, 0, work, alpha, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, alpha, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, alpha, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, alpha, 0x53);
        break;
    case 4:
        weight = work->phaseFrame / 10.0f;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        weight = 1.0f - weight;
        alpha = (s32)(weight * 128.0f);
        mnuDrawMantraSceneBackground(metadata, alpha, 0x52);
        mnuDrawMantraPulseFrame(0, 0, 1, alpha, work, 0x52);
        itfDspDrawMarksA(alpha, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        itfDspDrawMarksB(alpha, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, (s32)((1.0f - weight) * -27.0f), 0, 128, metadata, 0x53);
        mnuDrawMantraPulseStripAndKind(work, alpha, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, alpha, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, alpha, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, alpha, 0x53);
        break;
    case 9:
        weight = work->phaseFrame / 10.0f;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        mnuDrawMantraSceneBackground(metadata, 128, 0x52);
        alpha = (s32)(weight * 128.0f);
        mnuDrawMantraPulseFrame(0, 0, 1, alpha, work, 0x52);
        itfDspDrawMarksA(alpha, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        itfDspDrawMarksB(alpha, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, (s32)((1.0f - weight) * -27.0f), 0, 128, metadata, 0x53);
        mnuDrawMantraPulseStripAndKind(work, alpha, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, alpha, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, alpha, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, alpha, 0x53);
        break;
    case 5:
        weight = work->phaseFrame / 10.0f;
        weight = 1.0f - weight;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        alpha = (s32)(weight * 128.0f);
        mnuDrawMantraSceneBackground(metadata, alpha, 0x52);
        mnuDrawMantraPulseFrame(0, 0, 1, alpha, work, 0x52);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x3a, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x38, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x39, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 0, 128, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, 128, 0xf, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x10, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x12, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, alpha, metadata, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x4, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x5, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0xa, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0xb, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0xc, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, 128, 0xd, 0x53);
        mnuDrawSelectedMantraEntry(work, alpha, 0x53);
        mnuChooseDisplaySpriteKindFromEntryFlags(work, 128, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, alpha, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, alpha, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, alpha, 0x53);
        break;
    case 13:
        weight = work->phaseFrame / 10.0f;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        alpha = (s32)(weight * 128.0f);
        mnuDrawMantraSceneBackground(metadata, alpha, 0x52);
        mnuDrawMantraPulseFrame(0, 0, 0, alpha, work, 0x52);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x3a, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x38, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x39, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 0, 128, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, 128, 0xf, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x10, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x12, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, alpha, metadata, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x4, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0x5, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0xa, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0xb, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, alpha, 0xc, 0x53);
        mnuDrawPlacedSpriteFromTable(0, 0, 0, 128, 0xd, 0x53);
        mnuDrawSelectedMantraEntry(work, alpha, 0x53);
        mnuChooseDisplaySpriteKindFromEntryFlags(work, 128, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, alpha, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, alpha, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, alpha, 0x53);
        break;
    case 12:
        weight = work->phaseFrame / 10.0f;
        weight = 1.0f - weight;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        mnuDrawMantraSceneBackground(metadata, 128, 0x52);
        alpha = (s32)(weight * 128.0f);
        mnuDrawMantraPulseFrame(0, 0, 1, alpha, work, 0x52);
        itfDspDrawMarksA(alpha, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        itfDspDrawMarksB(alpha, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, alpha, metadata, 0x53);
        stripWeight = weight * 32.0f;
        alpha = (s32)stripWeight;
        mnuDrawMantraPulseStripAndKind(work, alpha, 0x53);
        /* Acquire the next list stage after drawing the strip. The live
         * address definition also preserves the native shared call tail. */
        pendingDisplayList = &work->sceneTransitionList;
        goto advance_pending_display;
    case 10:
    case 11:
        weight = work->phaseFrame / 10.0f;
        weight = 1.0f - weight;
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        mnuDrawMantraSceneBackground(metadata, 128, 0x52);
        mnuDrawMantraPulseFrame(0, 0, 1, 128, work, 0x52);
        itfDspDrawMarksA(128, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 128, 0x53);
        itfDspDrawMarksB(128, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, 128, metadata, 0x53);
        stripWeight = weight * 96.0f + 32.0f;
        alpha = (s32)stripWeight;
        mnuDrawMantraPulseStripAndKind(work, alpha, 0x53);
        pendingDisplayList = &work->sceneTransitionList;
    advance_pending_display:
        if (itfAdvanceDisplayList(pendingDisplayList, alpha, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, alpha, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, 128, 0x53);
        break;
    case 3:
    case 6:
        if (work->transitionBlendCounter > 0) weight = work->transitionBlendCounter / 10.0f;
        else if (work->transitionBlendCounter < 0) weight = -work->transitionBlendCounter / 10.0f;
        weight = 1.0f - sdfSin(weight * 3.1415926f);
        mnuDrawMantraSparkScene(metadata, 128, 0x52);
        mnuDrawMantraSceneBackground(metadata, 128, 0x52);
        mnuDrawMantraPulseFrame(0, 0, 1, (s32)(weight * 128.0f), work, 0x52);
        itfDspDrawMarksA(128, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 128, 0x53);
        itfDspDrawMarksB(128, 0x53);
        mnuDrawAnimatedCurrencyCounter(0, 0, 0, 128, metadata, 0x53);
        mnuDrawMantraPulseStripAndKind(work, 128, 0x53);
        if (itfAdvanceDisplayList(&work->sceneTransitionList, 128, 0x53)) {
            itfDspDrawPartyRecordWithLevel(0, 0, 0, mnuGetSelectedNodeValue()->partyRecord, 128, 0x53);
        }
        mnuDrawMantraNavigationPulses(work, 128, 0x53);
        break;
    case 7:
    case 8:
        break;
    default:
        return 0;
    }
    return 0;
}

extern u8 D_0036B410[17][15];
s32 mnuSelectMantraGridLeftCell(SdfGrid *grid, s32 x, s32 y, u32 from) {
    if (from != 0 && sdfGridSelectFilledCell(grid, x, y) != NULL) {
        return 1;
    }
    if (D_0036B410[y][x] & 0x20) {
        if (mnuSelectMantraGridLeftCell(grid, x - 1, y, 0x20) != 0) {
            return 1;
        }
    } else if ((D_0036B410[y][x] & 0x8) || (D_0036B410[y][x] & 0x4)) {
        if (mnuSelectMantraGridLeftCell(grid, x - 1, y, 0x20) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x80) && (from & 0x40) == 0) {
        if (mnuSelectMantraGridLeftCell(grid, x, y - 1, 0x80) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x40) && (from & 0x80) == 0) {
        if (mnuSelectMantraGridLeftCell(grid, x, y + 1, 0x40) != 0) {
            return 1;
        }
    }
    return 0;
}

extern u8 D_0036B410[17][15];
s32 mnuSelectMantraGridRightCell(SdfGrid *grid, s32 x, s32 y, u32 from) {
    if ((from & 0x10) && sdfGridSelectFilledCell(grid, x, y) != NULL) {
        return 1;
    }
    if (D_0036B410[y][x] & 0x10) {
        if (mnuSelectMantraGridRightCell(grid, x + 1, y, 0x10) != 0) {
            return 1;
        }
    } else if ((D_0036B410[y][x] & 0x8) || (D_0036B410[y][x] & 0x4)) {
        if (mnuSelectMantraGridRightCell(grid, x + 1, y, 0x10) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x80) && !(from & 0x40)) {
        if (mnuSelectMantraGridRightCell(grid, x, y - 1, 0x80) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x40) && !(from & 0x80)) {
        if (mnuSelectMantraGridRightCell(grid, x, y + 1, 0x40) != 0) {
            return 1;
        }
    }
    return 0;
}

extern u8 D_0036B410[17][15];

extern s32 mnuSelectMantraGridLeftCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);

/* Select the connected left neighbor, or fall back to the previous filled cell. */
s32 mnuSelectMantraGridLeftNeighbor(SdfGrid *grid) {
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
    MnuMantraNodeState *coordinates = grid->userData;
    s32 x = coordinates[entry->sceneId].x;
    s32 y = coordinates[entry->sceneId].y;

    if (x == 0) {
        if (sdfGridCursorLeftToFilledCell(grid) != NULL) {
            return 1;
        }
    } else if ((D_0036B410[y][x - 1] & 0x1C) != 0) {
        if (mnuSelectMantraGridLeftCell(grid, x - 1, y, 1) != 0) {
            return 1;
        }
    } else {
        if (sdfGridCursorLeftToFilledCell(grid) != NULL) {
            return 1;
        }
    }
    return 0;
}

extern s32 mnuSelectMantraGridRightCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *);

s32 mnuSelectMantraGridRightNeighbor(SdfGrid *grid) {
    MenuSceneWork *scene = (MenuSceneWork *)sdfGetTaskValueByKey(
        mnuSceneResourceContext, 1);
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)grid->cursor->value;
    MnuMantraNodeState *coordinates = grid->userData;
    s32 x = coordinates[entry->sceneId].x;
    s32 y = coordinates[entry->sceneId].y;

    if ((x != 10 || scene->boundsFlags == 0) &&
        (D_0036B410[y][x] & 0x3C) != 0) {
        if (mnuSelectMantraGridRightCell(grid, x, y, 0) != 0) {
            return 1;
        }
    } else {
        if (sdfGridCursorRightToFilledCell(grid) != NULL) {
            return 1;
        }
    }
    return 0;
}

extern u16 prfGetRequiredProfileLevel(u16);
extern u8 prfGetProfileParameterByte5(u16);
extern s32 prfBuildRawSkillList(u16, PrfSkillList *);
extern s32 ptyTestSecondaryProfileFlag(datUnitWork_t *, u16);
extern u32 ptyGetProfileRecordValue(const datUnitWork_t *, u16);

/* Build the 0x58-byte list entry for one profile, or NULL when it is not available (profile 0x4E is still
 * listed once flag 0x908 is set). The entry's state is 1 when requirement 1 passes or the profile flag is set,
 * 2 when only requirement 0 passes, and 3 otherwise. The scheduler argument is unused. */
MnuMantraGridEntry *mnuCreateMantraGridEntry(s32 unused, u16 profileId, MnuProfileProgress *selection) {
    MnuMantraGridEntry *entry;

    if (prfReqCheckWithFallback(selection->partyRecord, profileId) == 0) {
        if (profileId != 0x4E || evtBitCheck(0x908) == 0) {
            return NULL;
        }
    }

    entry = sdfChipAllocDirect(sizeof(MnuMantraGridEntry));
    memset(entry, 0, sizeof(MnuMantraGridEntry));
    entry->sceneId = profileId;
    entry->requiredLevel = prfGetRequiredProfileLevel(entry->sceneId);
    entry->backgroundColorIndex = prfGetProfileParameterByte5(entry->sceneId);
    prfBuildRawSkillList(entry->sceneId, &entry->skills);
    entry->cap = prfGetCapValue(entry->sceneId);
    entry->frame = 0x3C;
    entry->profileFlag = ptyTestSecondaryProfileFlag(selection->partyRecord, entry->sceneId);

    if (prfCheckRequirementRules(0, selection->partyRecord, entry->sceneId) != 0 ||
        entry->sceneId == 0x4E) {
        if (prfCheckRequirementRules(1, selection->partyRecord, entry->sceneId) != 0) {
            entry->state = MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG;
        } else if (entry->profileFlag != 0) {
            entry->state = MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG;
        } else {
            entry->state = MNU_MANTRA_GRID_ENTRY_FIRST_CHECK_OR_SPECIAL_PROFILE;
        }
    } else {
        entry->state = MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED;
    }
    entry->value = ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId);
    return entry;
}

extern MantraPrerequisiteRecord mnuMantraPrerequisiteRecords[];
extern void sdfGridSetCellValue(SdfGrid *grid, s32 column, s32 row, u32 cellValue);

/* Rebuild the 88-entry mantra grid from the prerequisite coordinates (tenths, or required level / 5
 * when the record's x is -1), report the selected profile's cell, then set the three
 * requirement-group bits in boundsFlags. */
void mnuBuildMantraGridAndGetProfileCoordinates(MenuSceneWork *work, MnuProfileProgress *selection, s32 *outX, s32 *outY) {
    MnuMantraNodeState *nodes = work->gridHandle->userData;
    MnuMantraGridEntry *entry;
    SdfGrid *grid;
    u32 column;
    u32 row;
    s32 foundX = 0;
    s32 foundY = 0;
    u32 i;

    memset(nodes, 0, 0x480);
    for (i = 1; i < 0x59; i++) {
        entry = mnuCreateMantraGridEntry((s32)work, i, selection);
        if (entry == NULL) {
            continue;
        }
        if (mnuMantraPrerequisiteRecords[i].x == -1) {
            column = entry->requiredLevel / 5;
            nodes[i].xRemainder = 0;
        } else {
            column = mnuMantraPrerequisiteRecords[i].x / 10;
            nodes[i].xRemainder = mnuMantraPrerequisiteRecords[i].x % 10;
        }
        row = mnuMantraPrerequisiteRecords[i].y / 10;
        grid = work->gridHandle;
        if (column >= grid->width) {
            column = grid->width - 1;
        }
        if (row >= grid->cellCount / grid->width) {
            row = grid->cellCount / grid->width - 1;
        }
        nodes[i].yRemainder = mnuMantraPrerequisiteRecords[i].y % 10;
        nodes[i].state = entry->state;
        nodes[i].y = row;
        nodes[i].x = column;
        sdfGridSetCellValue(grid, column, row, (u32)entry);
        if (entry->sceneId == selection->profileId) {
            foundX = column;
            foundY = row;
        }
    }
    if (outX != NULL) {
        *outX = foundX;
    }
    if (outY != NULL) {
        *outY = foundY;
    }
    {
        u16 groupA[3] = {0x13, 0x2E, 0x48};
        s8 groupB[4] = {'P', 'Q', 'R', 'S'};
        s8 groupC[2] = {'N', 'O'};
        s32 j;

        work->boundsFlags = 0;
        for (j = 0; j < 3; j++) {
            if (prfReqCheckWithFallback(selection->partyRecord, groupA[j])) {
                work->boundsFlags |= 1;
            }
        }
        for (j = 0; j < 4; j++) {
            if (prfReqCheckWithFallback(selection->partyRecord, groupB[j])) {
                work->boundsFlags |= 2;
            }
        }
        for (j = 0; j < 2; j++) {
            if (prfReqCheckWithFallback(selection->partyRecord, groupC[j])) {
                work->boundsFlags |= 4;
                if (j == 0) {
                    evtBitOn(0x908);
                }
            }
        }
        if (evtBitCheck(0x908)) {
            work->boundsFlags |= 4;
        }
    }
}

/* Recreate the scene's selection grid and copy its resulting coordinates. */
void mnuReinitializeSceneGrid(s32 sceneAddress) {
    SdfGrid *oldGrid = ((MenuSceneWork *)sceneAddress)->gridHandle;

    sdfDestroyGridWork(oldGrid);
    mnuInitializeMantraSelectionGrid((MenuSceneWork *)(u32)sceneAddress);
    mnuCopySceneCoordinates((MenuSceneWork *)(u32)sceneAddress);
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    oldGrid = NULL; /* handle released by the first call */
}

extern void sdfGridClear(SdfGrid *grid);
extern void mnuBuildMantraGridAndGetProfileCoordinates(MenuSceneWork *work, MnuProfileProgress *selection, s32 *x, s32 *y);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *grid, u32 x, u32 y);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *grid);


s32 mnuRestoreMantraGridSelection(MenuSceneWork *work) {
    SdfGrid *grid = work->gridHandle;
    SdfGridCell *cursor = grid->cursor;
    MnuMantraGridEntry *selected = (MnuMantraGridEntry *)(u32)cursor->value;
    u16 entryId = selected->sceneId;
    MnuMantraNodeState *entries = grid->userData;
    s16 x = entries[entryId].x;
    s16 y = entries[entryId].y;
    MnuResourceTask *resourceTask;
    MnuProfileProgress *selection;

    sdfGridClear(grid);
    resourceTask = (MnuResourceTask *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    selection = (MnuProfileProgress *)resourceTask->menuList->cursor->userValue;
    mnuBuildMantraGridAndGetProfileCoordinates(work, selection, NULL, NULL);
    if (sdfGridSelectFilledCell(
            work->gridHandle, x, y) == NULL) {
        sdfGridCursorLeftToFilledCell(work->gridHandle);
    }
}

/* Return the selected entry address through the scene-work/grid/slot chain, or zero when scene work is absent. */
MnuMantraGridEntry *fldGetSceneMetadataNode(void) {
    MenuSceneWork *scene = (MenuSceneWork *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);

    if (scene == NULL) {
        return 0;
    }
    return (MnuMantraGridEntry *)(u32)scene->gridHandle->cursor->value;
}


s32 mnuBeginMantraProfileSelection(void) {
    MnuMantraGridEntry *entry = fldGetSceneMetadataNode();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    scene->state = MENU_SCENE_STATE_SELECTING;
    scene->pendingProfileId = entry->sceneId;
    fclDestroyMessage();
    fclInitMessage(scene->messageWindowResource);
    memcpy(&scene->partySnapshot, (mnuGetSelectedNodeValue())->partyRecord,
           sizeof(scene->partySnapshot));
    scene->partySnapshot.profileId = scene->pendingProfileId;
    mnuSetupStaffMenuProfilePage(&scene->partySnapshot, scene->attachedEffect);
    scene->stageFinished = 0;
    scene->stageStarted = 0;
    return 0;
}

extern s8 scrSelectOperandIndex(datUnitWork_t *, s32);
extern s8 scrGetSelectedOperandIndex(datUnitWork_t *);
extern void mnuSetMantraGridFeedbackMode(MnuGridFeedbackState *, MnuGridFeedbackMode);
void mnuFinishMantraProfileSelection(void) {
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    scene->messageShadeFrames = 0;
    if (scene->pendingProfileId != 0) {
        MnuProfileProgress *selection = mnuGetSelectedNodeValue();
        MenuSceneWork *work;

        scrSelectOperandIndex(selection->partyRecord, scene->pendingProfileId);
        selection->profileId = scrGetSelectedOperandIndex(selection->partyRecord);
        selection->value = ptyGetProfileRecordValue(selection->partyRecord, (u16)selection->profileId);
        selection->cap = prfGetCapValue((u16)selection->profileId);

        work = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
        mnuReinitializeSceneGrid((s32)work);
        mnuSetMantraGridFeedbackMode(&work->gridFeedback, MNU_GRID_FEEDBACK_FADE_WITH_CUE);
    }

    evtFinishMessageWindowAndNotify();
    mnuFinalizeProgressHostVia(scene->attachedEffect, mnuReleaseMenuVisualWorkResources);
}

extern s8 dds3PadButtonState[];
extern s32 mnuAreRequiredProfilesCapped(u16);
extern s32 mnuGetMantraSourceValue(u16);
extern void itfDspPopulatePrimaryLabels(void);
extern void itfDspPopulateAlternateLabels(void);
extern void itfDspPopulateThirdLabels(void);
extern void itfDspSignalA(void);
extern void itfDspSignalB(void);
extern void itfDspSignalC(void);
extern void itfDspSignalD(void);
extern void itfDspSignalE(void);
extern s8 evtGetCapturedWindowPanelValue(void);
s32 mnuUpdateMantraSceneState(void) {
    MnuMantraGridEntry *entry = fldGetSceneMetadataNode();
    MnuProfileProgress *selection = mnuGetSelectedNodeValue();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    switch (scene->state) {
    case MENU_SCENE_STATE_SELECTING:
        if (dds3PadButtonState[0x21] < 0) {
            if (selection->profileId == scene->pendingProfileId) {
                if (prfGetCapValue(entry->sceneId) ==
                    ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId)) {
                    scene->state = MENU_SCENE_STATE_SIGNAL_E;
                } else {
                    scene->state = MENU_SCENE_STATE_SIGNAL_B;
                }
                scene->pendingProfileId = 0;
            } else if (prfGetCapValue(entry->sceneId) ==
                       ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId)) {
                scene->state = MENU_SCENE_STATE_SIGNAL_D;
                scene->pendingProfileId = 0;
            } else if (entry->state != MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG) {
                if (mnuAreRequiredProfilesCapped(entry->sceneId) == 0) {
                    scene->state = MENU_SCENE_STATE_SIGNAL_C;
                } else {
                    scene->state = MENU_SCENE_STATE_SIGNAL_A;
                }
                scene->pendingProfileId = 0;
            } else if (entry->profileFlag != 0) {
                scene->state = MENU_SCENE_STATE_ALTERNATE_LABELS;
            } else {
                scene->state = entry->state;
            }
        } else if (dds3PadButtonState[0x23] < 0 && scene->stageFinished != 0) {
            scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
            scene->pendingProfileId = 0;
        }
        break;
    case MENU_SCENE_STATE_PRIMARY_LABELS:
        itfDspPopulatePrimaryLabels();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW;
        break;
    case MENU_SCENE_STATE_ALTERNATE_LABELS:
        itfDspPopulateAlternateLabels();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW;
        break;
    case MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW:
        if (fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            if (evtGetCapturedWindowPanelValue() != 0) {
                scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
                scene->pendingProfileId = 0;
            } else {
                scene->state = MENU_SCENE_STATE_THIRD_LABELS;
            }
        }
        break;
    case MENU_SCENE_STATE_THIRD_LABELS:
        itfDspPopulateThirdLabels();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        if (entry->profileFlag != 0) {
            return 0;
        }
        DDS3_GBWK->header.currency -= mnuGetMantraSourceValue(entry->sceneId);
        break;
    case MENU_SCENE_STATE_SIGNAL_A:
        itfDspSignalA();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_B:
        itfDspSignalB();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_C:
        itfDspSignalC();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_D:
        itfDspSignalD();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_E:
        itfDspSignalE();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION:
        if (scene->stageFinished != 0 && fclChkMessage() == EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
            evtFinishMessageWindowAndNotify();
            sdfSetTaskItemMode(mnuSceneResourceContext, 1,
                               SDF_TASK_ITEM_MODE_PENDING_ACTIVATION);
            return -1;
        }
        break;
    default:
        break;
    }
    return 0;
}

extern void dspRequestMessageWindowModeOne(void);
extern void mnuDrawMantraSparkScene(MenuSceneMetadata *, s32, s32);
extern void mnuDrawAnimatedCurrencyCounter(s32, s32, s32, s32, MenuSceneMetadata *, s32);
extern s32 mnuDrawLoadedProgressPanels(datUnitWork_t *partyRecord, MenuProgressHost *work, s32 context);
struct SdfPoolNode;
extern s8 evtStageTestUpdate(struct SdfPoolNode **);
extern struct SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

/* Update attached visuals and stage completion, then ramp the message-window shade over ten frames.
 * Only the final two rectangle colors receive alpha; preserve both native selection lookups and the zero return. */
s32 mnuUpdateMantraSceneDisplay(void) {
    MenuSceneMetadata *sceneMetadata = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    u32 shadeColors[4];

    mnuGetSelectedNodeValue();
    fldGetSceneMetadataNode();
    dspRequestMessageWindowModeOne();
    mnuDrawMantraSparkScene(sceneMetadata, 0x80, 0x4A);
    mnuDrawAnimatedCurrencyCounter(0, -27, 0, 0x80, sceneMetadata, MNU_SCENE_DRAW_CONTEXT);
    if (mnuDrawLoadedProgressPanels(&sceneMetadata->partySnapshot, sceneMetadata->attachedEffect, MNU_SCENE_DRAW_CONTEXT) != 0) {
        if (sceneMetadata->stageStarted == 0) {
            evtStageTestSelectEntryWithoutInitialValue(
                sceneMetadata->entryId, MDL_ASSET_REQUEST_NONBLOCKING);
        }
        sceneMetadata->stageStarted = 1;
    }
    effUpdateAttached(0x1200, 0x730, 1, sceneMetadata->attachedEffect, MNU_SCENE_DRAW_CONTEXT);
    if (evtStageTestUpdate(kwlnSceneSurfaceSets[9]) >= 2) {
        sceneMetadata->stageFinished = 1;
    }
    if (fclChkMessage() != EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL) {
        if (sceneMetadata->messageShadeFrames < MNU_SCENE_SHADE_FRAME_LIMIT) {
            sceneMetadata->messageShadeFrames++;
        }
    } else if (sceneMetadata->messageShadeFrames > 0) {
        sceneMetadata->messageShadeFrames--;
    }
    if (sceneMetadata->messageShadeFrames != 0) {
        memset(shadeColors, 0, sizeof(shadeColors));
        shadeColors[3] = shadeColors[2] = (u32)((f32)sceneMetadata->messageShadeFrames / MNU_SCENE_SHADE_FRAME_SCALE * MNU_SCENE_SHADE_ALPHA_SCALE);
        uiDrawGradientColorRect(0, 0x700, 2, 0x2000, 0x700, shadeColors, MNU_SCENE_DRAW_CONTEXT);
    }
    return 0;
}

/* Copy the active scene entry coordinates selected by the grid metadata. */
void fldUpdateSceneEntryMetadata(MenuSceneWork *scene) {
    MnuMantraGridEntry *selectedEntry =
        (MnuMantraGridEntry *)(u32)scene->gridHandle->cursor->value;
    u16 entryIndex = selectedEntry->sceneId;

    scene->entryPosition.x = mnuMantraEntrySpritePlacements[entryIndex].entryX;
    entryIndex = selectedEntry->sceneId;
    scene->entryPosition.y = mnuMantraEntrySpritePlacements[entryIndex].entryY;
}

/* Refresh the active scene-work entry coordinates and return zero. */
s32 fldResetSceneState(void) {
    fldUpdateSceneEntryMetadata((MenuSceneWork *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 1));
    return 0;
}

/* Update and copy scene coordinates before releasing its node list; retain the native call order. */
void mnuCopySceneCoordinatesAndReleaseNodeList(void) {
    s32 sceneAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    MnuSceneListHead *nodes;
    mnuUpdateMantraSelectionPositions((MenuSceneWork *)(u32)sceneAddress, 1);
    mnuCopySceneCoordinates((MenuSceneWork *)(u32)sceneAddress);
    nodes = &((MenuSceneWork *)(u32)sceneAddress)->costTransitionList;
    mnuReleaseListNodes(nodes);
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    nodes = NULL; /* list released by the call above */
}


extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern s32 mnuRestoreMantraGridSelection(MenuSceneWork *work);

void mnuBeginTransitionAlt(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendNodeToDisplayList(&work->costTransitionList);

    if (transition != NULL) {
        if (work->transitionBlendCounter >= 6) {
            mnuStopResourceAnimation();
            mnuRestoreMantraGridSelection(work);
        } else if (work->transitionBlendCounter < -5) {
            mnuResetResourceAnimation();
            mnuRestoreMantraGridSelection(work);
        }
        transition->payload.transition.mode = mode;
        if (mode == 1) {
            work->transitionBlendCounter = 10;
            transition->payload.transition.fromValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuStopResourceAnimation();
            transition->payload.transition.toValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->transitionBlendCounter = -10;
            transition->payload.transition.fromValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuResetResourceAnimation();
            transition->payload.transition.toValue = (u32)(mnuGetSelectedNodeValue())->partyRecord;
            mnuStopResourceAnimation();
        }
    }
}


extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);
/* Shared draw-callback helper: the retail draw wrappers reach the placed-sprite
 * provider through an operation callback and keep `jal` + epilogue tails. */
static inline void mnuDrawPlacedSpriteVia(void (*draw)(s32, s32, s32, s32, s32, s32), s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context) {
    draw(x, y, z, alpha, placementIndex, context);
}
extern TaskWork *mnuSceneResourceContext;
extern void mnuDrawScaledPlacedSprite(s32, s32, s32, s32, s32, s32, f32, f32);
extern void mnuDrawMantraProfileSelectionSurface(s32, s32, s32, s32, MenuSceneWork *, s32);
extern void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *, s32, s32);
extern void mnuDrawMantraScenePlacementSprite(s32, s32, s32, s32, s32);

typedef struct {
    u8 pad00[0x38];
    s32 shade; /* 0x38 */
} DspEffectShade;

typedef struct {
    u8 pad00[0x828];
    DspEffectShade *shade; /* 0x828 */
} DspEffectRoot;

typedef struct {
    u8 pad00[0x1C];
    s32 phase;
    s32 frame;
    DspEffectRoot *effectRoot; /* 0x24 */
} DspTableScene;

extern s8 dds3PadButtonState[64];
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern s32 mnuRestoreMantraGridSelection(MenuSceneWork *);
extern s32 mnuScheduleUnlockedMantraMessages(MenuSceneWork *);
extern void mnuUpdateMantraSelectionCursorPosition(MenuSceneWork *);
extern void mnuBeginTransitionAlt(MenuSceneWork *, s32);
extern SdfGridCell *sdfGridCursorUpToFilledCell(SdfGrid *);
extern SdfGridCell *sdfGridCursorDownToFilledCell(SdfGrid *);
extern s32 mnuSelectMantraGridLeftNeighbor(SdfGrid *);
extern s32 mnuSelectMantraGridRightNeighbor(SdfGrid *);
extern u8 D_0036B410[17][15];
extern s32 mnuSelectMantraGridLeftCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);
extern s32 mnuSelectMantraGridRightCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);
extern SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *);

s32 mnuHandleMantraSceneInput(void) {
    DspTableScene *host;
    MenuSceneWork *work;
    s32 feedback = 0;
    u32 direction;

    host = (DspTableScene *)sdfGetTaskValueByKey(mnuSceneResourceContext,
        SDF_TASK_VALUE_USER_DATA_KEY);
    work = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    switch (host->phase) {
    case 0:
        if (host->frame < 10) {
            host->frame++;
        } else {
            host->phase = 1;
            host->frame = 0;
        }
        return 0;
    case 2:
        if (host->frame < 10) {
            host->frame++;
            return 0;
        }
        host->frame = 0;
        host->phase = 0;
        sdfSetTaskItemMode(mnuSceneResourceContext, 1, SDF_TASK_ITEM_MODE_PENDING_ACTIVATION);
        return -1;
    case 1:
        if (work->hasPendingUnlockedMantraMessages != 0) {
            host->phase = 2;
            host->frame = 0;
            work->pendingPhase13 = 1;
            work->scenePhase = MNU_MANTRA_PHASE_START_PENDING_MESSAGE;
            return 0;
        }
        if (work->transitionBlendCounter > 0) {
            work->transitionBlendCounter--;
            if (work->transitionBlendCounter == 5) {
                mnuStopResourceAnimation();
                mnuRestoreMantraGridSelection(work);
                work->hasPendingUnlockedMantraMessages = mnuScheduleUnlockedMantraMessages(work);
                if (work->hasPendingUnlockedMantraMessages != 0) {
                    work->transitionBlendCounter = 0;
                }
            }
        } else if (work->transitionBlendCounter < 0) {
            work->transitionBlendCounter++;
            if (work->transitionBlendCounter == -5) {
                mnuResetResourceAnimation();
                mnuRestoreMantraGridSelection(work);
                work->hasPendingUnlockedMantraMessages = mnuScheduleUnlockedMantraMessages(work);
                if (work->hasPendingUnlockedMantraMessages != 0) {
                    work->transitionBlendCounter = 0;
                }
            }
        }
        break;
    }
    mnuUpdateMantraSelectionCursorPosition(work);
    if (dds3PadButtonState[0x26] != 0) {
        work->cursorInputMask = 0x80;
    } else if (dds3PadButtonState[0x27] != 0) {
        work->cursorInputMask = 0x40;
    } else if (dds3PadButtonState[0x24] != 0) {
        work->cursorInputMask = 0x20;
    } else if (dds3PadButtonState[0x25] != 0) {
        work->cursorInputMask = 0x10;
    } else {
        work->cursorInputMask = 0;
    }
    direction = work->cursorInputMask;
    if (work->cursorMoving == 0) {
        if ((dds3PadButtonState[0x26] & 2) && (direction & 0x80)) {
            if (sdfGridCursorUpToFilledCell(work->gridHandle) != NULL) {
                feedback = 1;
            }
        } else if ((dds3PadButtonState[0x27] & 2) && (direction & 0x40)) {
            if (sdfGridCursorDownToFilledCell(work->gridHandle) != NULL) {
                feedback = 1;
            }
        } else if ((dds3PadButtonState[0x24] & 2) && (direction & 0x20)) {
            if (mnuSelectMantraGridLeftNeighbor(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if ((dds3PadButtonState[0x25] & 2) && (direction & 0x10)) {
            if (mnuSelectMantraGridRightNeighbor(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if (dds3PadButtonState[0x28] < 0) {
            mnuBeginTransitionAlt(work, 1);
            feedback = 4;
        } else if (dds3PadButtonState[0x2A] < 0) {
            mnuBeginTransitionAlt(work, 2);
            feedback = 4;
        }
    }
    if ((dds3PadButtonState[0x23] < 0 || dds3PadButtonState[0x20] < 0 ||
        dds3PadButtonState[0x21] < 0) && work->transitionBlendCounter == 0) {
        host->frame = 0;
        host->phase = 2;
        feedback = 3;
    }
    if (feedback != 0) {
        switch (feedback) {
        case 1:
            p2sdPlaySE(2, 127, 63);
            break;
        case 2:
            p2sdPlaySE(8, 127, 63);
            break;
        case 3:
            p2sdPlaySE(10, 127, 63);
            break;
        case 4:
            p2sdPlaySE(4, 127, 63);
            break;
        }
    }
    return 0;
}

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void mnuDrawPanelWithPackedColorPattern(s32 x, s32 y, s32 width, s32 fade, s32 t0) {
    u32 arr[4];
    s32 v;

    v = (s32)(((f32)(fade << 4)) * 0.0078125f);
    v |= 0x0A050700;
    arr[0] = v;
    arr[1] = v;
    arr[2] = v;
    arr[3] = v;
    y += 0x32;
    y *= 8;
    uiDrawGradientColorRect(x * 16, y, width, 0x2000, 0xC70, arr, t0);
}

/* Fill-level dispatch: direct, inverted, or sin-pulsed, then the shared layers. */
s32 mnuDrawMantraCostTransition(void) {
    s32 nodePrev;
    s32 nodeNext;
    MenuSceneWork *scene;
    s32 level;
    s32 scaled;
    s32 raw;
    f32 frac;
    MnuProfileProgress *cost;

    frac = 0.0f;
    mnuGetSelectedNodeValue();
    nodePrev = sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    nodeNext = sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    scene = (MenuSceneWork *)(u32)nodeNext;
    mnuDrawMantraSparkScene((MenuSceneMetadata *)nodePrev, 0x80, 0x52);
    level = *(s32 *)(nodePrev + 0x1C);
    switch (level) {
    case 0:
        frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
        scaled = (s32)(frac * 128.0f);
        mnuDrawScaledPlacedSprite(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
        mnuDrawMantraProfileSelectionSurface(0, 0, 1, scaled, scene, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xF, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scaled, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(cost->partyRecord, &scene->costTransitionList, scaled, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0x20, 0, 0, scaled, 0xB, 0x53);
        break;
    case 2:
        frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
        frac = 1.0f - frac;
        scaled = (s32)(frac * 128.0f);
        mnuDrawScaledPlacedSprite(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
        mnuDrawMantraProfileSelectionSurface(0, 0, 1, scaled, scene, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 0, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xF, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scaled, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(cost->partyRecord, &scene->costTransitionList, scaled, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0x20, 0, 0, scaled, 0xB, 0x53);
        break;
    case 1:
        /* frac keeps its 0.0f initializer when raw == 0. */
        raw = scene->transitionBlendCounter;
        if (raw > 0) {
            frac = (f32)raw / 10.0f;
        } else if (raw < 0) {
            frac = (f32)(-raw) / 10.0f;
        }
        frac = 1.0f - sdfSin(frac * 3.14159265f);
        mnuDrawScaledPlacedSprite(0, 0, 0, 0x80, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, 0x80, 0x53);
        mnuDrawMantraProfileSelectionSurface(0, 0, 1, (s32)(frac * 128.0f), scene, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xF, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(cost->partyRecord, &scene->costTransitionList, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0x20, 0, 0, 0x80, 0xB, 0x53);
        break;
    default:
        break;
    }
    mnuChooseDisplaySpriteKindFromEntryFlags(scene, 0x80, 0x53);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xD, 0x53);
    return 0;
}

extern void itfDspDrawMarksA(s32, s32);
/* Shared draw-callback helper for the TU's eight itfDspDrawMarksA sites (retail keeps jal + epilogue tails, like mnuDrawPlacedSpriteVia). */
static inline void itfDrawMarksVia(void (*draw)(s32, s32), s32 scale, s32 context) {
    draw(scale, context);
}

void mnuDrawDisplaySpriteAndPanelMarks(MenuSceneMetadata *scene, s32 alpha, s32 context) {
    mnuDrawMantraSparkScene(scene, alpha, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, alpha, 0x5A, context);
    itfDrawMarksVia(itfDspDrawMarksA, alpha, context);
}

/* Three sprite layers drawn at the origin for one draw context. */
void mnuDrawMantraSceneSpriteSequence(s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x4F, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0xC, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0xD, context);
}

void mnuDrawMantraScenePlacementSprite(s32 x, s32 y, s32 z, s32 drawContext, s32 drawArgument) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, drawContext, 14, drawArgument);
}

void mnuDrawMantraSceneSpriteGroup(s32 x, s32 y, s32 z, s32 alpha, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x47, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x48, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x49, context);
}

typedef struct {
    s8 spriteIndices[7];
} DspUnitSpriteTable;


typedef struct {
    u8 pad00[4];
    u16 unitId; /* 0x4 */
} DspTablePartyRecord;

typedef struct {
    DspTablePartyRecord *partyRecord; /* 0x0 */
    s32 profileId;                    /* 0x4: index into the 19-byte mantra-name rows */
} DspTableProgress;

typedef struct {
    u8 pad00[0x1C];
    struct { u8 pad00[0x70]; DspTableProgress *progress; } *selectedNode; /* 0x1C */
    u8 pad20[0x10];
    s32 *selectedValue; /* 0x30 */
} DspTableUnit;

typedef struct DspTableWindowContext {
    u8 pad00[0xC];
    DspTableUnit *unit; /* 0xC */
} DspTableWindowContext;


extern DspUnitSpriteTable D_003BC440[];
extern DspMantraName *D_003BAA78;
extern s32 frFontQueueTextAndOptionallyMeasure(s32 x, s32 y, u32 first, u32 second, s8 type, const u8 *encodedText, s32 flag, s32 option);

/* Draw the selected mantra's display: sprite, fade-driven effect and its name (shifted left for long names). */
void mnuDrawSelectedMantraProfile(s32 x, s32 y, s32 z, s32 alpha, DspTableWindowContext *windowContext, s32 drawContext) {
    DspUnitSpriteTable sprites = D_003BC440[0];
    DspTableUnit *unit = windowContext->unit;
    s32 *selectedValue = unit->selectedValue;
    DspTableProgress *progress = unit->selectedNode->progress;
    DspTableScene *scene = (DspTableScene *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    u8 scratch[0x20];

    selectedValue[1] = alpha;
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, sprites.spriteIndices[progress->partyRecord->unitId] + 0x4A, drawContext);
    scene->effectRoot->shade->shade = (s32)((f32)(alpha << 8) * 0.0078125f);
    effUpdateAttached(0xD80, 0xC30, 1, scene->effectRoot, 0x53);
    memset(scratch, 0, sizeof(scratch));
    if (strlen((const char *)D_003BAA78[progress->profileId].encodedText) >= 5) {
        frFontQueueTextAndOptionallyMeasure(x + 0xEF, y + 0x15E, z, alpha | 0xA09DC300, 0,
                                            D_003BAA78[progress->profileId].encodedText, 0x80000000, drawContext);
    } else {
        frFontQueueTextAndOptionallyMeasure(x + 0xF8, y + 0x15E, z, alpha | 0xA09DC300, 0,
                                            D_003BAA78[progress->profileId].encodedText, 0x80000000, drawContext);
    }
}


#include "common.h"

typedef struct DspDisplayModeUnit {
    u8 pad00[0x20];
    s32 displayMode;
    u8 pad24[0xC];
    s32 *selectedValue;
} DspDisplayModeUnit;

typedef struct DspDisplayModeWindowContext {
    u8 pad00[0xC];
    DspDisplayModeUnit *unit;
} DspDisplayModeWindowContext;

extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);

void mnuDrawDisplayModeSprites(s32 x, s32 y, s32 layer, s32 scale,
                   DspDisplayModeWindowContext *window, s32 context) {
    switch (window->unit->displayMode) {
    case 3:
        mnuDrawPlacedSpriteFromTable(x, y, layer, scale, 0x50, context);
        mnuDrawPlacedSpriteFromTable(x, y, layer, scale, 0x53, context);
        return;
    case 4:
        mnuDrawPlacedSpriteFromTable(x, y, layer, scale, 0x51, context);
        mnuDrawPlacedSpriteFromTable(x, y, layer, scale, 0x54, context);
        return;
    case 5:
        mnuDrawPlacedSpriteFromTable(x, y, layer, scale, 0x52, context);
        mnuDrawPlacedSpriteFromTable(x, y, layer, scale, 0x55, context);
        break;
    }
}


#include "sdk_ps2.h"
#include "dsp_entry.h"
#include "dsp_entry_api.h"
#include "dsp_name.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "mnu_scene.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"
#include "mnu_scene_list.h"
#include "mnu_sprite_placement.h"
#include "evt_message_window_api.h"

struct EffectSlotSet;

#define MNU_DISPLAY_TEXT_RGB 0xA09DC300
#define MNU_DISPLAY_DIM_TEXT_RGB 0xA09D7D00
#define MNU_DISPLAY_ALPHA_MASK 0xFF
#define MNU_DISPLAY_DIM_ALPHA_SCALE 0.6f
#define MNU_DISPLAY_TEXT_BUFFER_BYTES 0x20
#define MNU_SELECTION_ROW_HEIGHT 26
#define MNU_DISPLAY_FIXED_X_SHIFT 4
#define MNU_DISPLAY_FIXED_Y_SHIFT 3
#define MNU_DISPLAY_RECORD_ID_MASK 0xFFFF
#define MNU_DISPLAY_TEXT_FLAGS 0x80000000
#define MNU_SELECTED_ROW_SPRITE 12
#define MNU_UNSELECTED_ROW_SPRITE 14

extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);



extern void mnuCallInitWide(s32, s32, s32, s32, s32);


typedef struct DspUnit {
    u8 pad0[0x1C];
    s32 *selectedIndex; /* 0x1C */
    s32 entryCount; /* 0x20 */
    u8 pad24[0xC];
    s32 *selectedValue; /* 0x30 */
} DspUnit;

typedef struct DspWindowContext {
    u8 pad0[0xC];
    DspUnit *unit; /* 0xC */
} DspWindowContext;

typedef struct {
    u8 pad00[0x1C];
    void *selectedNode; /* 0x1C */
    u8 pad20[0xC];
    void (*drawCallback)();
    s32 *drawValues; /* 0x30 */
} DspMenuList;

typedef struct {
    u8 pad00[0x70];
    MnuProfileProgress *progress; /* 0x70 */
} DspMenuListNode;

typedef struct {
    s8 spriteIndices[7];
} DspEntrySpriteLookup;

extern DspEntrySpriteLookup D_003BC448[];
extern DspEntrySpriteLookup D_003BC480[];
extern char D_003BC450[];
extern void mnuDrawPlacedSpriteByIndex(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                                       s32 mode, s32 context);
extern s32 itfDrawGlyphChainWithWidthQuery(s32, s32, s32, u32, u8, char *, s32, u32);

/* Draw the selection strip from the last row back to the first. */
void itfDspDrawSelectionStrip(s32 x, s32 y, s32 z, s32 alpha, DspWindowContext *windowContext, s32 drawContext) {
    DspUnit *displayUnit = windowContext->unit;
    s32 selectedIndex = *displayUnit->selectedIndex;
    s32 rowIndex;

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x56, drawContext);
    rowIndex = displayUnit->entryCount - 1;
    if (rowIndex >= 0) {
        do {
            if (rowIndex == selectedIndex) {
                mnuDrawPlacedSpriteByIndex(x, y + rowIndex * MNU_SELECTION_ROW_HEIGHT, z, alpha, 0x57, MNU_SELECTED_ROW_SPRITE, drawContext);
            } else {
                mnuDrawPlacedSpriteByIndex(x, y + rowIndex * MNU_SELECTION_ROW_HEIGHT, z, alpha, 0x57, MNU_UNSELECTED_ROW_SPRITE, drawContext);
            }
            rowIndex--;
        } while (rowIndex >= 0);
    }
}

/* Store the selected value in the display unit and open its scaled window. */
static inline void itfDspInitializeSelectedWindowWith(
    void (*initialize)(s32, s32, s32, s32, s32),
    s32 x, s32 y, s32 z, s32 value, DspUnit *unit, s32 context) {
    *unit->selectedValue = value;
    initialize(x << MNU_DISPLAY_FIXED_X_SHIFT, y << MNU_DISPLAY_FIXED_Y_SHIFT,
               z, (s32)unit, context);
}

void itfDspInitSelectedWindow(s32 x, s32 y, s32 z, s32 drawValue, DspWindowContext *windowContext, s32 drawContext) {
    DspUnit *displayUnit = windowContext->unit;

    itfDspInitializeSelectedWindowWith(mnuCallInitWide, x, y, z, drawValue,
                                       displayUnit, drawContext);
}

/* Draw one party entry, highlighting the currently selected list node. */
void mnuDrawMantraPartyListEntry(s32 x, s32 y, s32 layer, DspMenuList *list, DspMenuListNode *node,
                   s32 drawContext) {
    char levelText[MNU_DISPLAY_TEXT_BUFFER_BYTES];
    DspEntrySpriteLookup spriteLookup = D_003BC448[0];
    MnuProfileProgress *profileProgress = node->progress;
    s32 alpha = list->drawValues[0];
    u32 textColor;

    if (list->selectedNode == node) {
        mnuDrawPlacedSpriteByIndex(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x58,
                      spriteLookup.spriteIndices[profileProgress->partyRecord->unitId] + 1, drawContext);
        mnuDrawPlacedSpriteByIndex(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x59, 0xB, drawContext);
        sprintf(levelText, D_003BC450, profileProgress->partyRecord->level);
        textColor = alpha & MNU_DISPLAY_ALPHA_MASK;
        textColor |= MNU_DISPLAY_TEXT_RGB;
        itfDrawGlyphChainWithWidthQuery((x >> MNU_DISPLAY_FIXED_X_SHIFT) + 0xD5, (y >> MNU_DISPLAY_FIXED_Y_SHIFT) + 0x7D, layer,
                                       textColor,
                                       4, levelText, 0, drawContext);
    } else {
        mnuDrawPlacedSpriteByIndex(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x58,
                      spriteLookup.spriteIndices[profileProgress->partyRecord->unitId], drawContext);
        mnuDrawPlacedSpriteByIndex(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x59, 0xA, drawContext);
        sprintf(levelText, D_003BC450, profileProgress->partyRecord->level);
        textColor = alpha & MNU_DISPLAY_ALPHA_MASK;
        textColor |= MNU_DISPLAY_TEXT_RGB;
        itfDrawGlyphChainWithWidthQuery((x >> MNU_DISPLAY_FIXED_X_SHIFT) + 0xD5, (y >> MNU_DISPLAY_FIXED_Y_SHIFT) + 0x7D, layer,
                                       textColor,
                                       0, levelText, 0, drawContext);
    }
}

extern void prfCopyMantraNamePart(u32, s32, void *);
extern s32 frFontDrawStyledGlyphChainAndMeasure(s32, s32, s32, u32, u8, const u8 *, s32, u32);

/* Fetch the low-16-bit display record ID and draw its text with the supplied attributes and fixed RGB. */
void itfDspDrawIndexedRecord(s32 x, s32 y, s32 layer, u32 textAttributes, u32 packedEntryId, s32 drawContext) {
    u32 textColor = textAttributes | MNU_DISPLAY_TEXT_RGB;
    u32 recordId = packedEntryId & MNU_DISPLAY_RECORD_ID_MASK;
    u8 recordText[MNU_DISPLAY_TEXT_BUFFER_BYTES];

    memset(recordText, 0, MNU_DISPLAY_TEXT_BUFFER_BYTES);
    prfCopyMantraNamePart(recordId, 1, recordText);
    frFontDrawStyledGlyphChainAndMeasure(x + 0x35, y + 0x136, layer, textColor, 4, recordText, MNU_DISPLAY_TEXT_FLAGS, drawContext);
}

extern char D_003BC458[];
extern char D_003BC460[];
extern char D_003BC468[];
/* Draw the placeholder sprites and dim text. Keep both native formatting calls:
 * the second replaces the first call's buffer contents before drawing. */
void mnuDrawDisplayPlaceholder(s32 x, s32 y, s32 layer, s32 alpha, s32 drawContext) {
    u32 textColor = (u32)((f32)alpha * MNU_DISPLAY_DIM_ALPHA_SCALE) | MNU_DISPLAY_DIM_TEXT_RGB;
    char placeholderText[MNU_DISPLAY_TEXT_BUFFER_BYTES];

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, alpha, 0x21, drawContext);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, alpha, 0x11, drawContext);
    sprintf(placeholderText, "%s %s %s", D_003BC458, D_003BC458, D_003BC458);
    sprintf(placeholderText, D_003BC460);
    frFontDrawStyledGlyphChainAndMeasure(x + 0xB3, y + 0x171, layer, textColor,
                                      0, placeholderText, MNU_DISPLAY_TEXT_FLAGS, drawContext);
}

/* Draw a formatted integer value with the same dim sprite/text treatment as the placeholder. */
void mnuDrawMantraCostValue(s32 x, s32 y, s32 layer, s32 alpha, s32 displayValue, s32 drawContext) {
    u32 textColor = (u32)((f32)alpha * MNU_DISPLAY_DIM_ALPHA_SCALE) | MNU_DISPLAY_DIM_TEXT_RGB;
    char valueText[MNU_DISPLAY_TEXT_BUFFER_BYTES];

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, alpha, 0x21, drawContext);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, alpha, 0x11, drawContext);
    sprintf(valueText, D_003BC468, displayValue);
    itfDrawGlyphChainWithWidthQuery(x + 0xB3, y + 0x173, layer, textColor,
                                  0, valueText, MNU_DISPLAY_TEXT_FLAGS, drawContext);
}

extern MnuSpritePlacement mnuMantraCommonSpritePlacements[];
extern struct EffectSlotSet *mnuMantraSpriteSlots[];
extern void effDrawSpriteWithCornerFade(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                                        s32 textureSetHandle, s32 slotIndex, s32 layer);

/* Submit a corner-faded sprite through the selected effect renderer. */
static inline void mnuSubmitCornerSpriteWith(
    void (*draw)(s32, s32, s32, u32, s32, s32, s32, s32),
    s32 x, s32 y, s32 z, u32 alpha, s32 flags, s32 slot,
    s32 sprite, s32 context) {
    draw(x, y, z, alpha, flags, slot, sprite, context);
}

void mnuDrawMantraPanelSprite(s32 x, s32 y, s32 z, s32 alpha,
                              u16 unusedSceneId, s32 unusedContext) {
    struct EffectSlotSet *resource = mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[41].resourceIndex];

    mnuSubmitCornerSpriteWith(effDrawSpriteWithCornerFade, (x + mnuMantraCommonSpritePlacements[41].x) << 4,
                  (y + mnuMantraCommonSpritePlacements[41].y) << 3, z,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  (s32)resource,
                  mnuMantraCommonSpritePlacements[41].spriteIndex, 0x53);
}

extern const u8 mnuMantraDisplayParameterLabels[][0x18];
extern struct FrFontTextBank *frFontGetColoredGlyphResource(void);
extern s32 prfGetMantraDisplayParameter(u16 profileId);
extern s32 frFontDrawColoredGlyphChainAndMeasure(s32, s32, u32, u32, u8, u16,
                                                  struct FrFontTextBank *, u32, s32, s32);
extern s32 mnuGetMantraSourceValue(u16);

/* Draw the selected mantra's label and remaining source cost. */
/* Numeric and unavailable cost displays use their respective draw interfaces. */
static inline void mnuDrawCostValueWith(
    void (*draw)(s32, s32, s32, s32, s32, s32),
    s32 x, s32 y, s32 z, s32 alpha, s32 value, s32 context) {
    draw(x, y, z, alpha, value, context);
}
static inline void mnuDrawCostPlaceholderWith(
    void (*draw)(s32, s32, s32, s32, s32),
    s32 x, s32 y, s32 z, s32 alpha, s32 context) {
    draw(x, y, z, alpha, context);
}

void mnuDrawMantraCostCounter(s32 x, s32 y, s32 z, s32 alpha, s32 sceneId, s32 drawContext) {
    struct FrFontTextBank *glyphResource = frFontGetColoredGlyphResource();
    u32 textColor = alpha | MNU_DISPLAY_TEXT_RGB;
    u32 dimColor = (u32)((f32)alpha * 0.599999964237213134765625f) | MNU_DISPLAY_DIM_TEXT_RGB;
    u8 label;

    itfDspDrawIndexedRecord(x, y, z, alpha, (u16)sceneId, drawContext);
    frFontDrawColoredGlyphChainAndMeasure(x + 0x39, y + 0x155, z, textColor, 0, (u16)sceneId, glyphResource, 0, 0,
                                          drawContext);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x3B, drawContext);
    label = prfGetMantraDisplayParameter((u16)sceneId);
    if (label != 0) {
        const u8 *style = mnuMantraDisplayParameterLabels[label - 1];
        frFontDrawStyledGlyphChainAndMeasure(x + 0xBA, y + 0x171, z, dimColor, 0, style, 0, drawContext);
    }
    mnuDrawCostValueWith(mnuDrawMantraCostValue, x + 0x18, y + 0x1A, z, alpha, mnuGetMantraSourceValue((u16)sceneId), drawContext);
}

/* Draw the profile label and placeholder for an unavailable mantra cost. */
void mnuDrawMantraCostPlaceholder(s32 x, s32 y, s32 z, s32 alpha, s32 sceneId, s32 drawContext) {
    struct FrFontTextBank *glyphResource = frFontGetColoredGlyphResource();
    u32 textColor = (u32)alpha | MNU_DISPLAY_TEXT_RGB;
    u32 dimColor = (u32)((f32)alpha * 0.599999964237213134765625f) | MNU_DISPLAY_DIM_TEXT_RGB;
    u8 label;

    itfDspDrawIndexedRecord(x, y, z, (u32)alpha, (u16)sceneId, drawContext);
    frFontDrawColoredGlyphChainAndMeasure(x + 0x39, y + 0x155, z, textColor, 0, (u16)sceneId, glyphResource, 0, 0,
                                          drawContext);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x3B, drawContext);

    label = prfGetMantraDisplayParameter((u16)sceneId);
    if (label != 0) {
        const u8 *style = mnuMantraDisplayParameterLabels[label - 1];
        frFontDrawStyledGlyphChainAndMeasure(x + 0xBA, y + 0x171, z, dimColor, 0, style, 0, drawContext);
    }

    mnuDrawCostPlaceholderWith(mnuDrawDisplayPlaceholder, x + 0x18, y + 0x1A, z, alpha, drawContext);
}

extern DspUnitName *D_003BAA70;
extern DspMantraName *D_003BAA78;
extern MnuMantraGridEntry *fldGetSceneMetadataNode(void);
extern void evtCopyEntryStringToActiveWindow(s32, const char *);
extern s32 mnuGetMantraSourceValue(u16);

extern MnuProfileProgress *mnuGetSelectedNodeValue(void);

/* Populate the unit and both mantra labels, plus the selected mantra's cost. */
void itfDspPopulatePrimaryLabels(void) {
    MnuProfileProgress *selection = mnuGetSelectedNodeValue();
    MnuMantraGridEntry *scene = fldGetSceneMetadataNode();
    char text[16];

    evtCopyEntryStringToActiveWindow(0, D_003BAA70[selection->partyRecord->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_003BAA78[selection->profileId].encodedText);
    evtCopyEntryStringToActiveWindow(2, D_003BAA78[scene->sceneId].encodedText);
    sprintf(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    evtCopyEntryStringToActiveWindow(3, text);
    evtSetMessageWindowOptionWhenOpen(0);
    fclStartMessage(0);
    evtStoreValueAndCaptureWindowPanelValue(8);
}

/* Populate the same menu labels, selecting the alternate display signal. */
void itfDspPopulateAlternateLabels(void) {
    MnuProfileProgress *selection = mnuGetSelectedNodeValue();
    MnuMantraGridEntry *scene = fldGetSceneMetadataNode();
    char text[16];

    evtCopyEntryStringToActiveWindow(0, D_003BAA70[selection->partyRecord->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_003BAA78[selection->profileId].encodedText);
    evtCopyEntryStringToActiveWindow(2, D_003BAA78[scene->sceneId].encodedText);
    sprintf(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    evtCopyEntryStringToActiveWindow(3, text);
    evtSetMessageWindowOptionWhenOpen(0);
    fclStartMessage(1);
    evtStoreValueAndCaptureWindowPanelValue(8);
}

/* Populate menu labels for the third display signal. */
void itfDspPopulateThirdLabels(void) {
    MnuProfileProgress *selection = mnuGetSelectedNodeValue();
    MnuMantraGridEntry *scene = fldGetSceneMetadataNode();
    char text[16];

    evtCopyEntryStringToActiveWindow(0, D_003BAA70[selection->partyRecord->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_003BAA78[selection->profileId].encodedText);
    evtCopyEntryStringToActiveWindow(2, D_003BAA78[scene->sceneId].encodedText);
    sprintf(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    evtCopyEntryStringToActiveWindow(3, text);
    fclStartMessage(2);
}

/* Dispatch the menu scene's display-only signals through the message starter.
 * These transitions do not consume the starter's success result. */
static inline void itfDspActivateSignalVia(s32 (*startEntry)(s32), s32 entry) {
    startEntry(entry);
}

void itfDspSignalA(void) {
    itfDspActivateSignalVia(fclStartMessage, 3);
}

void itfDspSignalB(void) {
    itfDspActivateSignalVia(fclStartMessage, 4);
}

void itfDspSignalC(void) {
    itfDspActivateSignalVia(fclStartMessage, 5);
}

void itfDspSignalD(void) {
    itfDspActivateSignalVia(fclStartMessage, 6);
}

void itfDspSignalE(void) {
    itfDspActivateSignalVia(fclStartMessage, 7);
}





void mnuUpdateSparkle(DspParticle *spark) {
    spark->timer = spark->timer - 1;
    if (spark->timer < 0) {
        if (spark->phase != 0) {
            spark->phase = 0;
            spark->timer = spark->timerMax = dds3GetRandFloatA(0) * 30.0f + 1.0f;
        } else if (dds3GetRandFloatA(0) > 0.7f) {
            spark->timer = spark->timerMax = dds3GetRandFloatA(0) * 30.0f + 0.0f;
        } else {
            spark->phase = 1;
            spark->timer = spark->timerMax = dds3GetRandFloatA(0) * 60.0f + 45.0f;
            spark->z = (dds3GetRandFloatA(0) * 2.0f - 1.0f) * 512.0f + 320.0f;
            spark->size = dds3GetRandFloatA(0) * 10.0f;
            spark->x = dds3GetRandFloatA(0) * 512.0f + -256.0f;
            spark->y = dds3GetRandFloatA(0) * 448.0f + -128.0f;
        }
    }
}




void mnuTickMantraSparkParticles(DspParticleState *state) {
    DspParticle *particle;
    s32 duration;
    s32 i;

    state->countdown = state->countdown - 1;
    if (state->countdown < 0) {
        duration = dds3GetRandFloatA(0) * 60.0f + 60.0f;
        state->period = duration;
        state->countdown = duration;
        state->strength = dds3GetRandFloatA(0) * 0.20000005f + 0.4f;
    }
    /* Advance every particle, including those waiting for their next phase. */
    particle = state->particles;
    for (i = 7; i >= 0; i--) {
        mnuUpdateSparkle(particle);
        particle++;
    }
}

extern void mnuDrawSpriteAtPlacement(s32 x, s32 y, s32 z, s32 alpha, s32 sprite,
                                     s32 placementIndex, s32 flags, s32 context);

static inline void mnuDrawSpriteAtPlacementWith(
    void (*draw)(s32, s32, s32, s32, s32, s32, s32, s32),
    s32 x, s32 y, s32 z, s32 alpha, s32 sprite, s32 placement,
    s32 flags, s32 context) {
    draw(x, y, z, alpha, sprite, placement, flags, context);
}

void mnuDrawMantraSparkParticle(const DspParticle *particle, s32 sprite, s32 surfaceIndex) {
    f32 fraction;
    s32 x;
    s32 y;
    s32 alpha;

    if (particle->phase == 0) {
        return;
    }
    fraction = 1.0f - (f32)particle->timer / particle->timerMax;
    x = (s32)(particle->x + fraction * particle->z);
    y = particle->y;
    alpha = (s32)(sdfSin(fraction * 3.14159265f) * 25.6f);
    mnuDrawSpriteAtPlacementWith(mnuDrawSpriteAtPlacement, x, y, 1, alpha, sprite, 0x2D, 0x20, surfaceIndex);
}

/* Preserve the provider's legacy K&R GS-register argument boundary. */
extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);

/* Finish a mantra effect pass using the shared alpha reset, full-surface
 * clear, and TEST-register restore. */
static inline void mnuRestoreMantraSurface(
    void (*submitTest)(u64, u32), s32 surface) {
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    submitTest(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, surface);
    submitTest(0x5100DL, surface);
}

void mnuDrawMantraSparkBackdrop(DspParticleState *state, s32 sprite, s32 surfaceIndex) {
    s32 i;
    DspParticle *particle;

    sdfSubmitGsTestOneRegisterPacket(0x30000, surfaceIndex);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, surfaceIndex);
    sdfSubmitGsTestOneRegisterPacket(0x3000D, surfaceIndex);
    uiDrawActiveSurfaceRegion(surfaceIndex);
    for (i = 0; i < 7; i++) {
        s32 y = i * 64;

        mnuDrawSpriteAtPlacement(0, y, 0, 0x80, sprite, 0x2C, 0x20, surfaceIndex);
        mnuDrawSpriteAtPlacement(0x100, y, 0, 0x80, sprite, 0x2C, 0x20, surfaceIndex);
    }
    sdfDispatchSurfaceWithPreparedTexturePacket(surfaceIndex);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceIndex);
    sdfSubmitGsTestOneRegisterPacket(0x50000, surfaceIndex);
    particle = state->particles;
    for (i = 7; i >= 0; i--) {
        mnuDrawMantraSparkParticle(particle, sprite, surfaceIndex);
        particle++;
    }
    mnuRestoreMantraSurface(sdfSubmitGsTestOneRegisterPacket, surfaceIndex);
}


#include "sdf_task_work.h"

extern TaskWork *mnuSceneResourceContext;
extern void mnuUpdateMantraSpriteCueGroups(struct MnuSpriteResourceGroup *resources);
extern void mnuDrawMantraSpriteCueGroups(s32 x, s32 y, s32 depth, s32 alpha,
                          struct MnuSpriteResourceGroup *resources, s32 drawArg);

/* Scroll the sprite cues opposite the scene cursor and draw them. */
static inline void mnuDrawSpriteCueGroupsWith(
    void (*draw)(s32, s32, s32, s32, struct MnuSpriteResourceGroup *, s32),
    s32 x, s32 y, s32 z, s32 alpha, struct MnuSpriteResourceGroup *resources,
    s32 context) {
    draw(x, y, z, alpha, resources, context);
}

void mnuDrawMantraSceneBackground(MenuSceneMetadata *scene, s32 alpha, s32 drawArg) {
    MenuSceneWork *work = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);

    if (work != NULL) {
        s32 x = 0.0f - (f32)work->cursorPosition.x / 775.0f * 256.0f;
        s32 y = 0.0f - (f32)work->cursorPosition.y / 910.0f * 224.0f;

        mnuUpdateMantraSpriteCueGroups(scene->spriteResources);
        mnuDrawSpriteCueGroupsWith(mnuDrawMantraSpriteCueGroups, x, y, 1, (f32)alpha * 0.15f, scene->spriteResources, drawArg);
    }
}

static inline void mnuDrawSparkBackdropWith(
    void (*draw)(DspParticleState *, s32, s32),
    DspParticleState *state, s32 sprite, s32 context) {
    draw(state, sprite, context);
}

void mnuDrawMantraSparkScene(MenuSceneMetadata *scene, s32 alpha, s32 context) {
    DspParticleState *sparkles;
    f32 factor;

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, alpha, 0x2A, context);
    factor = (f32)scene->sparkles.countdown / (f32)scene->sparkles.period;
    factor = 1.0f - factor;
    factor = scene->sparkles.strength * sdfSin(factor * 3.14159265f) +
             0.2f;
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, (f32)alpha * factor, 0x2B, context);
    sparkles = &scene->sparkles;
    mnuTickMantraSparkParticles(sparkles);
    mnuDrawSparkBackdropWith(mnuDrawMantraSparkBackdrop, sparkles, (s32)mnuMantraSpriteSlots[1], context);
}

void itfDspDrawMarksA(s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x3A, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x38, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x39, context);
}

void itfDspDrawMarksB(s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0xF, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x10, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x12, context);
}

extern u32 prfGetCapValue(u16);
extern u32 ptyGetProfileRecordValue(const struct datUnitWork_t *, u16);
extern MnuProfileRequirementFlags mnuGetProfileRequirementFlags(u16);

u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *entry, MnuProfileProgress *target) {
    u32 flags = 0;
    u32 cap;

    if (entry->sceneId == target->profileId) {
        flags |= MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH;
    }
    cap = prfGetCapValue(entry->sceneId);
    if (cap == ptyGetProfileRecordValue(target->partyRecord, entry->sceneId)) {
        flags |= MNU_MANTRA_DISPLAY_FLAG_AT_CAP;
    }
    if (entry->state == MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG) {
        flags |= MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_1;
    } else if (entry->state == MNU_MANTRA_GRID_ENTRY_FIRST_CHECK_OR_SPECIAL_PROFILE) {
        flags |= MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_2;
    } else if ((mnuGetProfileRequirementFlags(entry->sceneId) &
                MNU_PROFILE_REQUIREMENT_FLAG_RULE_PAIR_SET) != 0) {
        flags |= MNU_MANTRA_DISPLAY_FLAG_REQUIREMENT_PAIR_SET_FALLBACK;
    } else {
        flags |= MNU_MANTRA_DISPLAY_FLAG_REQUIREMENT_PAIR_CLEAR_FALLBACK;
    }
    return flags;
}

void itfDspDrawStrip(s32 x, s32 y, s32 layer, s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 4, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 5, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 0xA, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 0xB, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 0xC, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 0xD, context);
}

typedef struct DspUnitSpriteLookup {
    s8 spriteIndices[7];
} DspUnitSpriteLookup;

extern DspUnitSpriteLookup D_003BC478[];
extern DspUnitSpriteLookup D_003BC470[];
extern char D_003BC450[];
extern s32 frFontQueueFlaggedGlyphAndMeasure(s32, s32, s32, u32, u8, char *, s32, u32);

/* Draw the unit-specific display sprite and its two-column numeric level. */
void itfDspDrawEntryWithLevel(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context) {
    DspUnitSpriteLookup lookup = D_003BC470[0];
    char text[0x10];
    u32 textColor = scale | 0xA09DC300;

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, lookup.spriteIndices[entry->unitId], context);
    sprintf(text, D_003BC450, entry->level);
    frFontQueueFlaggedGlyphAndMeasure(x + 0x1BB, y + 0x173, layer, textColor,
                                     4, text, 0, context);
}

/* Select the unit's display sprite; retain the complete signed-byte table copy. */
void mnuDrawDisplayEntrySpriteFromLookup(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context) {
    DspUnitSpriteLookup lookup = D_003BC478[0];

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, lookup.spriteIndices[entry->unitId], context);
}

/* Draw both layers of a display entry and queue its formatted value. */
void itfDspDrawPartyRecordWithLevel(s32 x, s32 y, s32 layer, datUnitWork_t *entry, s32 scale, s32 context) {
    DspEntrySpriteLookup lookup = D_003BC480[0];
    char text[0x10];
    u32 tag = scale | 0xA09DC300;

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, lookup.spriteIndices[entry->unitId], context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, lookup.spriteIndices[entry->unitId] + 0x14, context);
    sprintf(text, D_003BC450, entry->level);
    frFontQueueFlaggedGlyphAndMeasure(x + 0x1BB, y + 0x173, layer, tag,
                                     4, text, 0, context);
}


s32 mnuDrawPulsingDisplaySprites(s32 frame, s32 scale, s32 context) {
    f32 wave = (f32)frame / 60.0f;
    f32 baseScale = (f32)scale;
    s32 drawScale;

    wave = sdfSin(wave * 3.1415926f);

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x1F, context);
    drawScale = (s32)(baseScale * (wave * 0.5f + 0.5f));
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, drawScale, 0x16, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, drawScale, 0x17, context);
    drawScale = (s32)(baseScale * 0.5f);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, drawScale, 0x18, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, drawScale, 0x19, context);
    if (frame < 60) {
        return 0;
    }
    return 1;
}

#ifdef NON_MATCHING
#include "dsp_entry.h"
#include "dsp_entry_api.h"
#include "dsp_name.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "mnu_scene.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"
#include "mnu_scene_list.h"
#include "mnu_sprite_placement.h"
#include "evt_message_window_api.h"
#include "sdf_task_work.h"
#include "eff.h"
extern void itfDspDrawEntryWithLevel(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context);
extern void mnuDrawDisplayEntrySpriteFromLookup(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context);

extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);
extern struct EffectSlotSet *mnuMantraSpriteSlots[];


extern EffectSlotSet *mnuMantraSpriteSlots[];
extern void mnuDrawScaledPlacedSprite(s32, s32, s32, s32, s32, s32, f32, f32);
/* Advances one display-list node of the mantra scene by a frame: during frames
 * 0-6 the node's entry fades in, the last node of the list (no next) draws its
 * entry and then the scaled bracket sprites (opening in frames 0-6, closing in
 * 7-10), the node draws its kind-specific side sprites in frames 0-6 and a
 * sine-pulsed sprite in frames 4-8, and the frame counter advances; it returns
 * 1 once the node is finished (frame 11 or later). Upstream: INCLUDE_ASM in
 * game/code_00254B30; written from the retail routine. It differs from retail
 * in register allocation, scheduling and in reading the node's record address
 * and kind once (retail re-reads them for every call) and testing the next
 * pointer once, merging the identical kind 1 and kind 2 calls and using range
 * checks for frame 4 to 8 (the compiler here emits sltiu range tests where
 * retail has paired compares). */
s32 itfUpdateDisplayListNode(MnuSceneListNode *node, s32 unused, s32 context) {
    s32 frame = node->frame;
    DspEntry *record = (DspEntry *)(u32)node->payload.timedDraw.recordAddress;
    s32 kind = node->payload.timedDraw.kind;
    f32 fade;

    if (frame < 7) {
        s32 fadeAlpha;

        fade = (f32)frame / 6.0f;
        if (fade > 1.0f) {
            fade = 1.0f;
        }
        fadeAlpha = fade * 128.0f;
        itfDspDrawEntryWithLevel(0, 0, 0, record, fadeAlpha, context);
        if (kind == 1 || kind == 2) {
            mnuDrawDisplayEntrySpriteFromLookup(0, 0, 1, record, fadeAlpha, context);
        }
    }
    if (node->next == NULL) {
        if (frame >= 7) {
            itfDspDrawEntryWithLevel(0, 0, 0, record, 128, context);
            if (kind == 1 || kind == 2) {
                mnuDrawDisplayEntrySpriteFromLookup(0, 0, 1, record, 128, context);
            }
        }
        if (frame < 7) {
            BdWork *sprite = &mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[20].resourceIndex]
                                  ->workEntries[mnuMantraCommonSpritePlacements[20].spriteIndex];
            f32 scale;
            f32 inset;
            f32 x;
            f32 y;
            s32 alpha;

            fade = (f32)frame / 6.0f;
            scale = 2.0f - fade;
            inset = fade * 16.0f;
            x = (f32)(sprite->geometry.bounds[2] >> 5) * (scale - 1.0f);
            y = (f32)-(sprite->geometry.bounds[3] >> 4) * (scale - 1.0f);
            alpha = fade * 128.0f;
            mnuDrawScaledPlacedSprite((inset + -16.0f) - x, y, 1, alpha, 20, context, scale, scale);
            mnuDrawScaledPlacedSprite((16.0f - inset) - x, y, 1, alpha, 21, context, scale, scale);
        } else if (frame < 11) {
            BdWork *sprite = &mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[20].resourceIndex]
                                  ->workEntries[mnuMantraCommonSpritePlacements[20].spriteIndex];
            f32 scale;
            f32 inset;
            f32 x;
            f32 y;
            s32 alpha;

            fade = (f32)(17 - frame) / 10.0f;
            scale = 2.0f - fade;
            inset = fade * 16.0f;
            x = (f32)(sprite->geometry.bounds[2] >> 5) * (scale - 1.0f);
            y = (f32)-(sprite->geometry.bounds[3] >> 4) * (scale - 1.0f);
            alpha = ((f32)(11 - frame) / 5.0f) * 128.0f;
            mnuDrawScaledPlacedSprite((inset + -16.0f) - x, y, 1, alpha, 20, context, scale, scale);
            mnuDrawScaledPlacedSprite((16.0f - inset) - x, y, 1, alpha, 21, context, scale, scale);
        }
    }
    if (frame < 7) {
        fade = (f32)frame / 6.0f;
        if (kind == 1) {
            mnuDrawPlacedSpriteFromTable((s32)(fade * -16.0f), 0, 1, (s32)(fade * 96.0f + 32.0f), 24, context);
        } else if (kind == 2) {
            mnuDrawPlacedSpriteFromTable((s32)(fade * 16.0f), 0, 1, (s32)(fade * 96.0f + 32.0f), 25, context);
        }
    }
    if (frame >= 4 && frame < 9) {
        fade = sdfSin(((f32)(frame - 3) / 6.0f) * 3.1415926f);
        if (kind == 1) {
            mnuDrawPlacedSpriteFromTable(0, 0, 1, (s32)(fade * 128.0f), 22, context);
        } else if (kind == 2) {
            mnuDrawPlacedSpriteFromTable(0, 0, 1, (s32)(fade * 128.0f), 23, context);
        }
    }
    node->frame++;
    return frame >= 11;
}
#else
INCLUDE_ASM(const s32, "game/code_0024F0D0", itfUpdateDisplayListNode);
#endif

MnuSceneListNode *mnuAllocateDisplayListNode(void) {
    MnuSceneListNode *node = sdfChipAllocDirect(sizeof(MnuSceneListNode));

    memset(node, 0, 0x14);
    return node;
}

MnuSceneListNode *mnuAppendDisplayListNode(MnuSceneListHead *head) {
    MnuSceneListNode *node = head->first;
    if (node == NULL) {
        node = mnuAllocateDisplayListNode();
        head->first = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = mnuAllocateDisplayListNode();
        node = node->next;
    }
    return node;
}

MnuSceneListNode *mnuReleaseDisplayListNodeAndGetNext(MnuSceneListNode *node) {
    MnuSceneListNode *next;

    next = node->next;
    sdfChipFreeDirect(node);
    return next;
}

void mnuReleaseDisplayListNodes(MnuSceneListHead *head) {
    MnuSceneListNode *node = head->first;

    while (node != NULL) {
        node = mnuReleaseDisplayListNodeAndGetNext(node);
    }
}

extern s32 itfUpdateDisplayListNode(MnuSceneListNode *, s32, s32);

/* Advance the display list's timer and release completed nodes in order. */
s32 itfAdvanceDisplayList(MnuSceneListHead *head, s32 scale, s32 context) {
    s32 *counter = &head->frameCount;
    MnuSceneListNode *node = head->first;
    s32 index = 0;

    if (mnuDrawPulsingDisplaySprites(*counter, scale, context) != 0) {
        *counter = 0;
    } else {
        *counter = *counter + 1;
    }
    if (node == NULL) {
        return 1;
    }
    do {
        s32 completed = itfUpdateDisplayListNode(node, index, context);

        index++;
        if (completed != 0) {
            node = mnuReleaseDisplayListNodeAndGetNext(node);
            head->first = node;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 0;
}

void itfDspDrawScaledA(s32 unused, s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x16, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale * 0.5f, 0x18, context);
}

void itfDspDrawScaledB(s32 unused, s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x17, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale * 0.5f, 0x19, context);
}

void itfDspDrawPair(s32 x, s32 y, s32 layer, s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 0x14, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, layer, scale, 0x15, context);
}

void func_00256E88(void) {
}

#include "eff.h"

/* The common placement table entry's sprite, re-derived at each use as retail does. */
#define MANTRA_COMMON_SPRITE(i)                                                                              \
    (&mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[i].resourceIndex]                                 \
          ->workEntries[mnuMantraCommonSpritePlacements[i].spriteIndex])

extern void mnuDrawScaledPlacedSprite(s32, s32, s32, s32, s32, s32, f32, f32);
void func_00256E90(s32 frame, s32 unused, s32 alpha, s32 context) {
    f32 alphaValue;
    f32 fade;
    f32 scale;
    f32 inset;
    f32 halfWidth;
    f32 angle;
    s32 y;

    if (frame < 5) {
        fade = ((f32)frame) * 0.25f;
    } else {
        fade = 1.0f;
    }
    alphaValue = (f32)alpha;
    scale = 2.0f - fade;
    inset = fade * 16.0f;
    halfWidth = ((f32)(MANTRA_COMMON_SPRITE(20)->geometry.bounds[2] >> 5)) * (scale - 1.0f);
    y = ((f32)(-(MANTRA_COMMON_SPRITE(20)->geometry.bounds[3] >> 4))) * (scale - 1.0f);
    {
        s32 scaledAlpha = alphaValue * fade;
        mnuDrawScaledPlacedSprite((inset + (-16.0f)) - halfWidth, y, 0, scaledAlpha, 20, context, scale,
                                  scale);
        mnuDrawScaledPlacedSprite((16.0f - inset) - halfWidth, y, 0, scaledAlpha, 21, context, scale, scale);
    }
    fade = ((f32)frame) / 5.0f;
    angle = fade * 3.141592503f;
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, alphaValue * sdfSin(angle), 20, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, alphaValue * sdfSin(angle), 21, context);
}

#undef MANTRA_COMMON_SPRITE


void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *obj, s32 scale, s32 context) {
    MnuProfileProgress *target = mnuGetSelectedNodeValue();
    MnuMantraGridEntry *entry =
        (MnuMantraGridEntry *)(u32)obj->gridHandle->cursor->value;
    u32 flags;
    s32 kind;

    if (entry != NULL) {
        flags = mnuGetMantraDisplayFlags(entry, target);
        if (flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) {
            kind = 9;
        } else if (flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP) {
            kind = 8;
        } else if (flags & MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_1) {
            kind = 6;
        } else {
            kind = 7;
        }
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 1, scale, kind, context);
    }
}


#include "common.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"

extern void mnuDrawMantraPanelSprite(s32, s32, s32, s32, u16, s32);
extern void mnuDrawMantraCostCounter(s32, s32, s32, s32, s32, s32);
extern void mnuDrawMantraCostPlaceholder(s32, s32, s32, s32, s32, s32);

void mnuDrawSelectedMantraEntry(MenuSceneWork *object, s32 alpha, s32 context) {
    MnuMantraGridEntry *entry =
        (MnuMantraGridEntry *)(u32)object->gridHandle->cursor->value;

    if (entry != NULL) {
        if (object->entryMessageInProgress == 0) {
            if (entry->state != MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED) {
                if (entry->profileFlag != 0) {
                    mnuDrawMantraCostPlaceholder(0, 0, 1, alpha, entry->sceneId, context);
                } else {
                    mnuDrawMantraCostCounter(0, 0, 1, alpha, entry->sceneId, context);
                }
                return;
            }
            mnuDrawMantraPanelSprite(0, 0, 1, alpha, entry->sceneId, context);
        }
    }
}




#include "common.h"
#include "mnu_mantra_sprite_draw.h"
#include "sdf_task_work.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"

extern void itfDspDrawStrip(s32, s32, s32, s32, s32);
extern void mnuDrawSelectedMantraEntry(MenuSceneWork *, s32, s32);
extern void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *, s32, s32);
extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);

/* Compose the selected-entry detail and its status-kind overlay. Both draw
 * stages use the same scene, opacity, and render context. */
static inline void mnuDrawMantraSelectionDetails(
    void (*drawEntry)(MenuSceneWork *, s32, s32),
    void (*drawKind)(MenuSceneWork *, s32, s32),
    MenuSceneWork *object, s32 scale, s32 context) {
    drawEntry(object, scale, context);
    drawKind(object, scale, context);
}

void mnuDrawMantraPulseStripAndKind(MenuSceneWork *object, s32 scale, s32 context) {
    itfDspDrawStrip(0, 0, 0, scale, context);
    mnuDrawMantraSelectionDetails(mnuDrawSelectedMantraEntry,
                                 mnuChooseDisplaySpriteKindFromEntryFlags,
                                 object, scale, context);
}

void mnuDrawMantraGridPulseEntry(s32 x, s32 y, s32 z, MenuSceneWork *work, s32 alpha, s32 surface) {
    MnuProfileProgress *selection;
    MnuMantraGridEntry *entry;
    u32 flags;
    s32 placementIndex;

    selection = mnuGetSelectedNodeValue();
    itfDspDrawStrip(x, y, z, alpha, surface);
    entry = (MnuMantraGridEntry *)(u32)work->gridHandle->cursor->value;
    if (entry != NULL) {
        if (work->entryMessageInProgress == 0) {
            if (entry->state != MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED) {
                if (entry->profileFlag != 0) {
                    mnuDrawMantraCostPlaceholder(x, y, z, alpha, entry->sceneId, surface);
                } else {
                    mnuDrawMantraCostCounter(x, y, z, alpha, entry->sceneId, surface);
                }
            } else {
                mnuDrawMantraPanelSprite(x, y, z, alpha, entry->sceneId, surface);
            }
        }
        flags = mnuGetMantraDisplayFlags(entry, selection);
        if (flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) {
            placementIndex = 9;
        } else if (flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP) {
            placementIndex = 8;
        } else {
            placementIndex = (flags & MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_1) ? 6 : 7;
        }
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, placementIndex, surface);
    }
}

extern s16 mnuMantraVariantSpritePlacements[][6];
extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);
extern void mnuAdvanceWrappingFrame(s32 *);

/* Draw the two mantra-entry passes, then restore the surface's GS state. */
void mnuDrawMantraPulseGridPasses(MenuSceneWork *work, s32 surface) {
    SdfGrid *grid;
    SdfGridCell *cell;
    MnuMantraGridEntry *entry;
    s32 row;
    s32 col;
    s32 x;
    s32 y;

    grid = work->gridHandle;
    x = -9 - work->cursorPosition.x;
    y = 0x45 - work->cursorPosition.y;
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        for (col = 0; col < 15; col++) {
            if (cell[col].value != 0) {
                entry = (MnuMantraGridEntry *)(u32)cell[col].value;
                grid->drawCell(x + mnuMantraVariantSpritePlacements[entry->sceneId][2],
                               y + mnuMantraVariantSpritePlacements[entry->sceneId][3],
                               0, grid, &cell[col], surface);
            }
        }
    }
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0x190, 0, surface);
    uiDrawUniformColorRect(0, 0xA50, -1, 0x2000, 0x4B0, 0, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        for (col = 0; col < 15; col++) {
            if (cell[col].value != 0) {
                entry = (MnuMantraGridEntry *)(u32)cell[col].value;
                grid->drawCell(x + mnuMantraVariantSpritePlacements[entry->sceneId][2],
                               y + mnuMantraVariantSpritePlacements[entry->sceneId][3],
                               1, grid, &cell[col], surface);
            }
        }
    }
    mnuRestoreMantraSurface(sdfSubmitGsTestOneRegisterPacket, surface);
}

extern void mnuUpdateMantraGridEntry(MnuProfileProgress *, SdfGrid *, SdfGridCell *);
extern TaskWork *mnuSceneResourceContext;

void mnuAdvanceMantraPulseGridEntries(MnuProfileProgress *selection, SdfGrid *grid) {
    SdfGridCell *cell;
    s32 row;
    s32 col;

    sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        for (col = 0; col < 15; col++) {
            if (cell[col].value != 0) {
                mnuUpdateMantraGridEntry(selection, grid, &cell[col]);
            }
        }
    }
}


extern void mnuDrawMantraGridEntry(s32, s32, s32, s32, MnuProfileProgress *, SdfGrid *, SdfGridCell *, s32);
extern void mnuDrawScaledMantraGridEntry(s32, s32, s32, s32, MnuProfileProgress *, SdfGrid *, f32, f32, SdfGridCell *, s32);
extern void mnuDrawMantraGridEntryIndicators(s32, s32, s32, s32, MnuProfileProgress *, SdfGrid *, f32, f32, SdfGridCell *, s32);
extern void mnuDrawMantraCursorTrail(s32, s32, s32, s32, MenuSceneWork *, s32);
extern void mnuDrawMantraGridFeedback(s32, s32, s32, s32, MnuGridFeedbackState *, s32);
extern MnuProfileProgress *mnuGetSelectedNodeValue(void);
extern TaskWork *mnuSceneResourceContext;

/* Draw the mantra pulse band and both entry passes at unit scale, placing the
 * selected entry with its offsets from the display table. */
void mnuDrawMantraPulseGridAtUnitScale(s32 x, s32 y, s32 z, s32 alpha,
                   MnuProfileProgress *selection, SdfGrid *grid,
                   s32 surface) {
    MenuSceneWork *display;
    SdfGridCell *cell;
    s32 row;
    s32 n;

    display = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    for (row = 0; row < 0xD; row++) {
        mnuDrawScaledSpriteAtCoordinatePlacement(x, y, z, alpha, row + 0x1F, 0x20, 1.0f, 1.0f, surface);
    }
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                mnuDrawMantraGridEntry(x, y, z, alpha, selection, grid, cell, surface);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
    n = (display->boundsFlags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) != 0
            ? 0x1E
            : 0x1A;
    for (row = 0; row <= n; row++) {
        mnuDrawScaledSpriteAtCoordinatePlacement(x, y, z, alpha, row, 0x20, 1.0f, 1.0f, surface);
    }
    mnuDrawMantraCursorTrail(0, 0, z, alpha, display, surface);
    mnuDrawMantraGridFeedback(x + mnuMantraVariantSpritePlacements[selection->profileId][2],
                  y + mnuMantraVariantSpritePlacements[selection->profileId][3],
                  z, alpha, &display->gridFeedback, surface);
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                mnuDrawMantraGridEntryIndicators(x, y, z, alpha, selection, grid, 1.0f, 1.0f, cell, surface);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
}

/* One local `n` serves as both the per-row column countdown in the two entry
 * loops and the final-row bound in the band loop: one declared type, one
 * neutral name, as a C89 programmer with all declarations at the top would
 * write. That reuse is what retail's bytes require. */
void mnuDrawMantraPulseGridAtScale(s32 x, s32 y, s32 z, s32 alpha,
                   MnuProfileProgress *selection, SdfGrid *grid, s32 surface,
                   f32 scaleX, f32 scaleY) {
    MenuSceneWork *display;
    SdfGridCell *cell;
    s32 row;
    s32 n;

    display = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    for (row = 0; row < 0xD; row++) {
        mnuDrawScaledSpriteAtCoordinatePlacement(x, y, z, alpha, row + 0x1F, 0x20, scaleX, scaleY, surface);
    }
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                mnuDrawScaledMantraGridEntry(x, y, z, alpha, selection, grid, scaleX, scaleY, cell, surface);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
    n = (display->boundsFlags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) != 0
            ? 0x1E
            : 0x1A;
    for (row = 0; row <= n; row++) {
        mnuDrawScaledSpriteAtCoordinatePlacement(x, y, z, alpha, row, 0x20, scaleX, scaleY, surface);
    }
    for (row = 0; row < 0x11; row++) {
        cell = grid->cells + row * grid->width;
        n = 0xE;
        do {
            if (cell->value != 0) {
                mnuDrawMantraGridEntryIndicators(x, y, z, alpha, selection, grid, scaleX, scaleY, cell, surface);
            }
            cell++;
            n--;
        } while (n >= 0);
    }
}

/* Advance entry animation before the coordinate trail's wrapping clock. */
static inline void mnuAdvanceMantraPulseState(
    void (*advanceEntries)(MnuProfileProgress *, SdfGrid *),
    void (*advanceFrame)(s32 *),
    MenuSceneWork *work, MnuProfileProgress *selection) {
    advanceEntries(selection, work->gridHandle);
    advanceFrame(&work->coordinateRing.frame);
}

void mnuUpdateMantraPulseGrid(MenuSceneWork *work, MnuProfileProgress *selection) {
    mnuAdvanceMantraPulseState(mnuAdvanceMantraPulseGridEntries,
                               mnuAdvanceWrappingFrame, work, selection);
}

/* The grid pass supplies its own depth; the frame preserves the caller's amount. */
void mnuDrawMantraPulseFrame(s32 x, s32 y, s32 depth, s32 amount,
                            MenuSceneWork *work, s32 surface) {
    MnuProfileProgress *selection;
    SdfGrid *grid;
    s32 offsetX;
    s32 offsetY;

    selection = mnuGetSelectedNodeValue();
    mnuUpdateMantraPulseGrid(work, selection);
    grid = work->gridHandle;
    offsetX = -9 - work->cursorPosition.x;
    offsetY = 0x45 - work->cursorPosition.y;
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0x190, 0, 0x2000, 0x8C0, 0x80, surface);
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0x190, 0, surface);
    uiDrawUniformColorRect(0, 0xA50, -1, 0x2000, 0x4B0, 0, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
    mnuDrawMantraPulseGridAtUnitScale(x + offsetX, y + offsetY, 1, amount, selection, grid, surface);
    mnuRestoreMantraSurface(sdfSubmitGsTestOneRegisterPacket, surface);
}

void mnuDrawMantraGridSelectionMarkers(MenuSceneWork *work, s32 layer) {
    s32 x = work->entryPosition.x - work->cursorPosition.x - 9;
    s32 y = work->entryPosition.y - work->cursorPosition.y + 0x45;

    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, 0, 0x80, 0x3E, layer);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, 0, 0x80, 0x3D, layer);
}

void mnuCopySceneCoordinates(MenuSceneWork *work) {
    s32 x = work->entryPosition.x;
    s32 y = work->entryPosition.y;
    MenuSceneCoordinate *point = work->coordinateRing.coordinates;
    s32 remaining = 9;

    do {
        point->x = x;
        point->y = y;
        point++;
        remaining--;
    } while (remaining >= 0);
}

void mnuAdvanceWrappingFrame(s32 *frame) {
    s32 previous;

    previous = *frame;
    *frame = previous + 1;
    if (0x3c < previous + 1) {
        *frame = 0;
    }
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_task_work.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"
#include "eff.h"

extern void mnuDrawMantraCursorTrail(s32, s32, s32, s32, MenuSceneWork *, s32);

extern void mnuDrawPlacedSpriteWithFlags(s32, s32, s32, s32, s32, s32, s32);
extern void mnuDrawScaledPlacedSpriteWithFlags(s32, s32, s32, s32, s32, s32, s32, f32, f32);
extern EffectSlotSet *mnuMantraSpriteSlots[];

/* Draws the mantra scene's cursor sprite and a trail of its ten most recent
 * positions: each older position is drawn 80% of the way back to its old place,
 * scaled up and faded by age, and positions equal to the current one are
 * skipped; the current cursor sprite is drawn last. Upstream: INCLUDE_ASM in
 * game/code_00257200; written from the retail routine. It differs from retail
 * only in register allocation, scheduling, the addressing of the ring entries
 * and the placement of float and double literals; retail's 0.2 is the double
 * 0x3FC9999999999999 from D_003AF950 and the float literals 0.8, 0.3 and 0.8
 * are lit4 words D_003BA2B4, D_003BA2B8 and D_003BA2BC. */
void mnuDrawMantraCursorTrail(s32 x, s32 y, s32 z, s32 alpha, MenuSceneWork *work, s32 context) {
    MenuSceneCoordinateRing *ring = &work->coordinateRing;
    s32 cursorX = work->entryPosition.x - work->cursorPosition.x - 9;
    s32 cursorY = work->entryPosition.y - work->cursorPosition.y + 0x45;
    f32 baseAlpha;
    s32 i;

    mnuDrawPlacedSpriteWithFlags(x + cursorX, y + cursorY, 0, alpha, 0x20, 0x3E, context);
    ring->coordinates[ring->cursor].x = work->entryPosition.x;
    ring->coordinates[ring->cursor].y = work->entryPosition.y;
    ring->cursor++;
    if (ring->cursor >= 10) {
        ring->cursor = 0;
    }
    baseAlpha = (f32)alpha;
    for (i = 0; i < 10; i++) {
        MenuSceneCoordinate *point = &ring->coordinates[i];
        s32 age;
        s32 drawX;
        s32 drawY;
        f32 fade;
        f32 expansion;
        f32 scale;
        BdWork *sprite;

        if (point->x == work->entryPosition.x && point->y == work->entryPosition.y) {
            continue;
        }
        age = i - ring->cursor;
        if (age < 0) {
            age += 10;
        }
        fade = (f32)(age - 4) / 5.0f;
        drawX = point->x - work->cursorPosition.x - 9;
        drawY = point->y - work->cursorPosition.y + 0x45;
        drawX = (s32)((f32)cursorX - (f32)(cursorX - drawX) * 0.8f);
        drawY = (s32)((f32)cursorY - (f32)(cursorY - drawY) * 0.8f);
        if (fade < 0.0f) {
            fade = 0.0f;
        }
        expansion = -(1.0f - fade) * 0.2;
        scale = expansion + 1.0f;
        sprite = &mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[0x3D].resourceIndex]
                      ->workEntries[mnuMantraCommonSpritePlacements[0x3D].spriteIndex];
        mnuDrawScaledPlacedSpriteWithFlags(
            (s32)((f32)(x + drawX) - (f32)(sprite->geometry.bounds[2] >> 5) * expansion),
            (s32)((f32)(y + drawY) - (f32)(sprite->geometry.bounds[3] >> 4) * expansion), 0,
            (s32)(baseAlpha * fade * 0.3f), 0x20, 0x3D, context + 1, scale, scale);
    }
    cursorX = work->entryPosition.x - work->cursorPosition.x - 9;
    cursorY = work->entryPosition.y - work->cursorPosition.y + 0x45;
    mnuDrawPlacedSpriteWithFlags(x + cursorX, y + cursorY, 0, (s32)(baseAlpha * 0.8f), 0x20, 0x3D,
                                 context + 1);
}
#else
INCLUDE_ASM(const s32, "game/code_0024F0D0", mnuDrawMantraCursorTrail);
#endif



#include "sdk_ps2.h"
#include "mnu_variant_sprite_placement.h"
#include "common.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_task_work.h"
#include "mnu_mantra_grid.h"
#include "mnu_profile_progress.h"
#include "prf_profile_cap.h"
#include "mnu_scene_work.h"
#include "sdf_grid.h"

extern u32 ptyGetProfileRecordValue(const struct datUnitWork_t *, u16);


extern u32 D_003E274C[];

extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];

extern void cmpDrawPanelEffCell(u32 startX, u32 startY, u32 startZ, u32 endX, u32 endY, u32 endZ, u32 color, u32 surfaceIndex);

/* Connect prerequisite cells with a straight stroke or a three-stroke bend. */
void mnuDrawMantraPrerequisiteLink(s32 x, s32 y, s32 z, s32 cellWidth,
                                 s32 cellHeight, u32 color, s32 direction,
                                 MnuMantraGridEntry *scene,
                                 MnuMantraNodeState *states, s32 surface) {
    MantraPrerequisiteRecord *record;
    s32 targetId;
    s16 startX;
    s16 startY;
    s16 endX;
    s16 endY;
    s32 startOffset = 0;
    s8 bendInset = 0;
    s8 targetOffset = 0;

    record = &mnuMantraPrerequisiteRecords[scene->sceneId];
    targetId = record->ids[direction];
    startX = x + 0x10;
    startY = y + 0xA;
    endX = x + (mnuMantraVariantSpritePlacements[targetId][2] - mnuMantraVariantSpritePlacements[scene->sceneId][2]);
    endY = y + (mnuMantraVariantSpritePlacements[targetId][3] - mnuMantraVariantSpritePlacements[scene->sceneId][3]);
    endX -= 4;
    endY += 0xA;

    if (record->flags[direction] & 4) {
        u8 sourceRow = states[scene->sceneId].y;
        u8 targetRow = states[targetId].y;

        targetOffset = (sourceRow - targetRow) * 4;
        bendInset = ((f32)(endY - startY) / (f32)cellHeight) * 4.0f;
        if (bendInset < 0) {
            bendInset = -bendInset;
        }
    }
    if (record->flags[direction] & 2) {
        startOffset = endY - startY >= 0 ? 4 : -4;
    }
    if (startY != endY) {
        s32 turnX;
        s32 startPixelY;
        s32 endPixelY;

        turnX = (endX - (cellWidth >> 3) * 3 + bendInset) << 4;
        startPixelY = (startY + startOffset) << 3;
        cmpDrawPanelEffCell(startX << 4, startPixelY, z,
                               turnX, startPixelY, z, color, surface);
        endPixelY = (endY + targetOffset) << 3;
        cmpDrawPanelEffCell(turnX, startPixelY, z,
                               turnX, endPixelY, z, color, surface);
        cmpDrawPanelEffCell(turnX, endPixelY, z,
                               endX << 4, endPixelY, z, color, surface);
    } else {
        cmpDrawPanelEffCell(startX << 4, (endY + startOffset) << 3, z,
                               endX << 4, endY << 3, z, color, surface);
        /* Native flat stroke uses jal before attachment cleanup. Qualified trace:
         * sandbox_MatchA/deep_link_F2_trace, rtl.01.sibling call545/reset549;
         * reset549 is gone by rtl.13.life, after the sibling-call decision. */
        startOffset = 0;
    }
}

u32 mnuGetMantraPrerequisiteFlags(s32 direction, MnuMantraGridEntry *scene, MnuMantraNodeState *states, MnuProfileProgress *target) {
    MantraPrerequisiteRecord *record = &mnuMantraPrerequisiteRecords[scene->sceneId];
    u32 flags = 0x100;
    if (prfGetCapValue(scene->sceneId) == ptyGetProfileRecordValue(target->partyRecord, scene->sceneId)) {
        flags = 0x101;
    }
    if (states[record->ids[direction]].state == MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG) {
        flags |= 0x12;
    }
    if (states[record->ids[direction]].state == MNU_MANTRA_GRID_ENTRY_FIRST_CHECK_OR_SPECIAL_PROFILE) {
        flags |= 0x10;
    }
    if (prfGetCapValue((u16)record->ids[direction]) ==
        ptyGetProfileRecordValue(target->partyRecord, (u16)record->ids[direction])) {
        flags |= 4;
    }
    if (record->flags[direction] & 1) {
        flags |= 8;
    }
    return flags;
}

extern char D_003BC458[];
extern char D_003BC488[];
extern char D_003BC490[];
extern char D_003BC498[];
extern TaskWork *mnuSceneResourceContext;
extern MnuProfileProgress *mnuGetSelectedNodeValue(void);
extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *scene, MnuProfileProgress *target);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern s32 frFontMeasureAndQueueGlyph(s32, s32, s32, u32, const u8 *, s32);

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF950);

void mnuDrawMantraGraphCell(s32 x, s32 y, s32 z, SdfGrid *grid, SdfGridCell *cell, s32 surface) {
    u32 prerequisiteFlags[4];
    MnuMantraGridEntry *scene;
    MnuProfileProgress *selection;
    MnuMantraNodeState *states;
    MantraPrerequisiteRecord *record;
    s32 cellWidth;
    s32 cellHeight;
    s8 i;
    u32 flags;
    u32 color;

    scene = (MnuMantraGridEntry *)(u32)cell->value;
    if (scene == NULL) {
        return;
    }
    selection = mnuGetSelectedNodeValue();
    states = grid->userData;
    sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    record = &mnuMantraPrerequisiteRecords[scene->sceneId];
    memset(prerequisiteFlags, 0, sizeof(prerequisiteFlags));
    if (record->kind == 0) {
        cellWidth = grid->cellWidth;
        cellHeight = grid->cellHeight;
        for (i = 0; i < 4 && record->ids[i] != 0; i++) {
            if (states[record->ids[i]].state != 0) {
                prerequisiteFlags[i] = mnuGetMantraPrerequisiteFlags(i, scene, states, selection);
            }
        }
        for (i = 0; i < 4; i++) {
            if (prerequisiteFlags[i] & 0x100) {
                color = 0x50505080;
                if (prerequisiteFlags[i] & 2) {
                    color = 0x80808080;
                }
                mnuDrawMantraPrerequisiteLink(x, y, z, cellWidth, cellHeight, color, i, scene, states, surface);
            }
        }
    }
    switch (scene->backgroundColorIndex) {
    case 1:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x10707020, surface);
        break;
    case 2:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x60401020, surface);
        break;
    case 3:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x10601020, surface);
        break;
    case 4:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x70701020, surface);
        break;
    case 5:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x70107020, surface);
        break;
    case 6:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x10107020, surface);
        break;
    case 7:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x70707020, surface);
        break;
    }
    flags = mnuGetMantraDisplayFlags(scene, selection);
    if (flags & 1) {
        uiDrawUniformColorRect((x - 4) << 4, (y - 4) << 3, 0, 0x1C0, 0xE0, 0x60501050, surface);
    }
    if (flags & 2) {
        frFontMeasureAndQueueGlyph(x + 4, y, z, 0x808020F0, (const u8 *)D_003BC458, surface);
        frFontMeasureAndQueueGlyph(x, y, z, 0x808020F0, (const u8 *)D_003BC488, surface);
    } else if (flags & 4) {
        frFontMeasureAndQueueGlyph(x, y, z, 0x108080F0, (const u8 *)D_003BC488, surface);
    } else if (flags & 8) {
        frFontMeasureAndQueueGlyph(x + 4, y, z, 0x404040F0, (const u8 *)D_003BC488, surface);
    } else if (flags & 0x20) {
        frFontMeasureAndQueueGlyph(x, y, z, 0x40404080, (const u8 *)D_003BC490, surface);
    } else {
        frFontMeasureAndQueueGlyph(x, y, z, 0x40404080, (const u8 *)D_003BC498, surface);
    }
}

void mnuDrawSpriteAtScaledPlacement(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 scaleX, f32 scaleY);

void mnuDrawMantraEntryStatusIcon(s32 x, s32 y, s32 z, s32 alpha, u32 flags,
                   f32 scaleX, f32 scaleY, s32 context) {
    if (flags & 2) {
        mnuDrawSpriteAtScaledPlacement(x, y, z, alpha, 0x20, 0x33, context, scaleX, scaleY);
        return;
    }
    if (flags & 4) {
        mnuDrawSpriteAtScaledPlacement(x, y, z, alpha, 0x20, 0x34, context, scaleX, scaleY);
        return;
    }
    if (flags & 8) {
        mnuDrawSpriteAtScaledPlacement(x, y, z, alpha, 0x20, 0x35, context, scaleX, scaleY);
    }
}


void mnuSetMantraGridFeedbackMode(MnuGridFeedbackState *state, MnuGridFeedbackMode mode) {
    state->mode = mode;
    state->frame = 0;
}
/* Step the notification frame (wraps at 31); the fade-with-cue mode plays once at frame 0.
 * Its unused implicit-int result preserves the call's non-sibling epilogue. */
mnuAdvanceMantraGridFeedback(MnuGridFeedbackState *voice) {
    s32 notify;

    notify = 0;
    switch (voice->mode) {
    case MNU_GRID_FEEDBACK_LOOPING:
        voice->frame = voice->frame + 1;
        if (voice->frame >= 0x1F) {
            voice->frame = 0;
        }
        break;
    case MNU_GRID_FEEDBACK_FADE_WITH_CUE:
        notify = voice->frame == 0;
        voice->frame = voice->frame + 1;
        if (voice->frame >= 0x1F) {
            voice->frame = 0;
            voice->mode = MNU_GRID_FEEDBACK_LOOPING;
        }
        break;
    }
    if (notify == 1) {
        p2sdPlaySE(0x15, 0x7F, 0x3F);
    }
}

void mnuDrawPlacedSpriteWithFlags(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context);
void mnuDrawRotatedPlacedSprite(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 rotation);

void mnuDrawMantraGridFeedback(s32 x, s32 y, s32 z, s32 alpha, MnuGridFeedbackState *state,
                   s32 context) {
    f32 progress;

    /* Both modes interpolate in radians and draw the ring pieces in degrees. */
    switch (state->mode) {
    case MNU_GRID_FEEDBACK_LOOPING: {
        f32 rotation;
        f32 degrees;
        progress = (f32)state->frame / 30.0f;
        rotation = 6.2831853f;
        degrees = 57.29578f;
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x43, context);
        rotation = progress * rotation;
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x42, context, rotation * degrees);
        rotation += 3.14159265f;
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x42, context, rotation * degrees);
        return;
    }
    case MNU_GRID_FEEDBACK_FADE_WITH_CUE: {
        f32 originalAlpha;
        f32 fade;
        f32 secondary;
        f32 degrees;
        s32 count;
        progress = (f32)state->frame / 30.0f;
        progress = 1.0f - progress;
        originalAlpha = (f32)alpha;
        mnuDrawPlacedSpriteWithFlags(x, y, z, (s32)(originalAlpha * progress), 0x20, 0x41, context);
        count = state->frame;
        if (count < 10) {
            progress = (f32)count / 10.0f;
            fade = 1.0f;
        } else {
            progress = 1.0f;
            fade = (f32)(30 - count) / 20.0f;
        }
        secondary = 0.0f;
        if (count >= 7) {
            if (count >= 11) {
                secondary = (f32)(30 - count) / 19.0f;
            } else {
                secondary = (f32)(count - 6) * 0.25f;
            }
        }
        alpha = (s32)(originalAlpha * fade);
        degrees = 57.29578f;
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x3F, context, progress * -1.5707963f * degrees);
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x40, context, progress * 1.5707963f * degrees);
        mnuDrawRotatedPlacedSprite(x, y, z, (s32)(originalAlpha * secondary), 0x20, 0x40, context,
                      1.5707963f * 57.29578f);
        progress = (f32)state->frame / 30.0f;
        mnuDrawPlacedSpriteWithFlags(x, y, z, (s32)(originalAlpha * progress), 0x20, 0x43, context);
        break;
    }
    }
}





static inline s16 mnuRandomPulseDuration(void *state) {
    return dds3GetRandFloatA(state) * 120.0f + 30.0f;
}

void mnuUpdateMantraGridEntryPulse(MnuMantraGridEntry *entry) {
    MnuMantraGridPulse *pulse = &entry->pulse;
    s32 timer = pulse->timer + 1;
    f32 random;

    pulse->timer = timer;
    if ((s16)timer < pulse->duration) {
        return;
    }
    pulse->timer = 0;
    random = dds3GetRandFloatA(0);
    switch (pulse->mode) {
    case MNU_MANTRA_GRID_PULSE_WAITING:
        if (random < 0.3f) {
            pulse->mode = MNU_MANTRA_GRID_PULSE_OPENING;
            pulse->duration = 50;
            return;
        }
        if (random < 0.5f) {
            pulse->mode = MNU_MANTRA_GRID_PULSE_FLASH;
            pulse->duration = 60;
            return;
        }
        pulse->duration = mnuRandomPulseDuration(0);
        return;
    case MNU_MANTRA_GRID_PULSE_OPENING:
        pulse->mode = MNU_MANTRA_GRID_PULSE_OPEN;
        pulse->duration = mnuRandomPulseDuration(0);
        return;
    case MNU_MANTRA_GRID_PULSE_OPEN:
        if (dds3GetRandFloatA(0) < 0.1f) {
            pulse->mode = MNU_MANTRA_GRID_PULSE_CLOSING;
            pulse->duration = 50;
            return;
        }
        pulse->duration = mnuRandomPulseDuration(0);
        return;
    case MNU_MANTRA_GRID_PULSE_INITIAL:
    case MNU_MANTRA_GRID_PULSE_CLOSING:
    case MNU_MANTRA_GRID_PULSE_FLASH:
        pulse->mode = MNU_MANTRA_GRID_PULSE_WAITING;
        pulse->duration = mnuRandomPulseDuration(0);
        return;
    default:
        return;
    }
}


void mnuDrawMantraGridEntryPulse(s32 x, s32 y, s32 z, s32 alpha, MnuMantraGridEntry *entry,
                   s32 context) {
    MnuMantraGridPulse *pulse = &entry->pulse;
    MnuMantraGridPulseMode mode = pulse->mode;
    f32 progress;

    if (mode >= MNU_MANTRA_GRID_PULSE_MODE_COUNT) {
        return;
    }

    /* The first two ring pieces interpolate in radians, then draw in degrees. */
    switch (mode) {
    case MNU_MANTRA_GRID_PULSE_INITIAL:
    case MNU_MANTRA_GRID_PULSE_WAITING:
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x44, context);
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x45, context);
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x46, context);
        return;

    case MNU_MANTRA_GRID_PULSE_OPENING: {
        progress = (f32)(s16)pulse->timer / (f32)pulse->duration;

        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x44, context,
                      progress * -1.0210175f * 57.29578f);
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x45, context,
                      progress * 0.31415924f * 57.29578f);
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x46, context);
        return;
    }

    case MNU_MANTRA_GRID_PULSE_OPEN:
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x44, context,
                      -1.0210175f * 57.29578f);
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x45, context,
                      0.31415924f * 57.29578f);
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x46, context);
        return;

    case MNU_MANTRA_GRID_PULSE_CLOSING: {
        progress = (f32)(s16)pulse->timer / (f32)pulse->duration;
        progress = 1.0f - progress;

        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x44, context,
                      progress * -1.0210175f * 57.29578f);
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x45, context,
                      progress * 0.31415924f * 57.29578f);
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x46, context);
        return;
    }

    case MNU_MANTRA_GRID_PULSE_FLASH: {
        s16 timer = (s16)pulse->timer;

        if (timer < 50) {
            progress = (f32)timer / 50.0f;
        } else {
            progress = (f32)(60 - timer) / 10.0f;
        }
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x44, context,
                      progress * -1.0210175f * 57.29578f);
        mnuDrawRotatedPlacedSprite(x, y, z, alpha, 0x20, 0x45, context,
                      progress * 0.31415924f * 57.29578f);
        mnuDrawPlacedSpriteWithFlags(x, y, z, alpha, 0x20, 0x46, context);
        break;
    }
    }
}

extern TaskWork *mnuSceneResourceContext;





extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *scene, MnuProfileProgress *target);
extern void mnuUpdateMantraGridEntryPulse(MnuMantraGridEntry *entry);

void mnuUpdateMantraGridEntry(MnuProfileProgress *target, SdfGrid *grid, SdfGridCell *entry) {
    MenuSceneWork *scene;
    MnuMantraGridEntry *displayEntry;
    u32 flags;

    scene = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    displayEntry = (MnuMantraGridEntry *)(u32)entry->value;
    displayEntry->frame += 1;
    if ((f32)displayEntry->frame > 60.0f) {
        displayEntry->frame = 0;
    }
    flags = mnuGetMantraDisplayFlags(displayEntry, target);
    if ((flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) != 0) {
        mnuAdvanceMantraGridFeedback(&scene->gridFeedback);
        return;
    }
    if ((flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP) != 0) {
        mnuUpdateMantraGridEntryPulse(displayEntry);
    }
}



#include "mnu_mantra_sprite_draw.h"
#include "sdk_ps2.h"
#include "mnu_variant_sprite_placement.h"
#include "mnu.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_task_work.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "mnu_scene.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_sprite_resource.h"
#include "mnu_sprite_placement.h"
#include "sdf_grid.h"
#include "mnu_scene_work.h"
#include "sdf_chip.h"



extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);


extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];
extern TaskWork *mnuSceneResourceContext;
extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *, MnuProfileProgress *);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void mnuDrawScaledScenePlacementSprite(s32, s32, s32, s32, s32, s32, f32, f32, s32);
extern void mnuDrawMantraEntryStatusIcon(s32, s32, s32, s32, u32, f32, f32, s32);

extern const u8 D_003BC4A0[];
extern void mnuDrawPlacedSpriteWithFlags(s32, s32, s32, s32, s32, s32, s32);
extern void mnuDrawScaledVariantSprite(s32, s32, s32, s32, s32, s32, f32, f32, s32);
extern void mnuDrawMantraGridEntryPulse(s32, s32, s32, s32, MnuMantraGridEntry *, s32);

void mnuDrawMantraGridEntry(s32 x, s32 y, s32 depth, s32 alpha,
                          MnuProfileProgress *profileOwner, SdfGrid *grid,
                          SdfGridCell *entry, s32 context) {
    u8 mappedIds[6];
    MnuMantraGridEntry *scene;
    MenuSceneWork *display;
    s32 drawX;
    s32 drawY;
    s32 i;
    s32 atCap;
    u32 flags;
    f32 pulse;
    f32 progress;

    memcpy(mappedIds, D_003BC4A0, sizeof(mappedIds));
    display = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    scene = (MnuMantraGridEntry *)entry->value;
    pulse = (sdfSin(((f32)scene->frame / 60.0f) * 6.2831853f -
                       1.5707963f) + 1.0f) * 0.5f;
    progress = (f32)(scene->frame - 20) / 20.0f;
    if (progress > 1.0f) {
        progress = 1.0f;
    }
    if (progress < 0.0f) {
        progress = 0.0f;
    }
    progress = sdfSin(progress * 3.1415926f) * 0.3f;
    drawX = x + mnuMantraVariantSpritePlacements[scene->sceneId][2];
    drawY = y + mnuMantraVariantSpritePlacements[scene->sceneId][3];
    flags = mnuGetMantraDisplayFlags(scene, profileOwner);
    if (display->boundsFlags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) {
        mnuDrawScaledVariantSprite(x, y, depth, alpha, scene->sceneId, 0x20,
                                  1.0f, 1.0f, context);
    } else {
        for (i = 0; i < 6; i++) {
            if (scene->sceneId == mappedIds[i]) {
                mnuDrawScaledVariantSprite(x, y, depth, alpha, i + 0x59, 0x20,
                                          1.0f, 1.0f, context);
                break;
            }
        }
        if (i == 6) {
            mnuDrawScaledVariantSprite(x, y, depth, alpha, scene->sceneId, 0x20,
                                      1.0f, 1.0f, context);
        }
    }
    pulse = pulse * 0.5f + 0.4f;
    if (flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) {
        atCap = flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP;
        if (atCap) {
            f32 fade = (f32)alpha;
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, (s32)(fade * 0.5f),
                                        0x20, 0x3C, context);
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, (s32)(fade * 0.3f),
                                        0x20, 0x3C, context);
        } else {
            f32 fade = (f32)alpha;
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, (s32)(fade * pulse),
                                        0x20, 0x3C, context);
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, (s32)(fade * progress),
                                        0x20, 0x3C, context);
        }
    } else {
        atCap = flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP;
        if (atCap) {
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, alpha,
                                        0x20, 0x33, context);
        } else if (flags & (MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_1 |
                            MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_2)) {
            f32 fade = (f32)alpha;
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, (s32)(fade * pulse),
                                        0x20, 0x34, context);
            mnuDrawPlacedSpriteWithFlags(drawX, drawY, depth, (s32)(fade * progress),
                                        0x20, 0x34, context);
        }
    }
    if (atCap) {
        mnuDrawMantraGridEntryPulse(drawX, drawY, depth, alpha, scene, context);
    }
}

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC440);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC448);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC450);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC458);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC460);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC468);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC470);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC478);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC480);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC488);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC490);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC498);

INCLUDE_SDATA(const s32, "game/code_0024F0D0", D_003BC4A0);

void mnuDrawScaledMantraGridEntry(s32 x, s32 y, s32 depth, s32 alpha,
                   MnuProfileProgress *profileOwner, SdfGrid *grid,
                   f32 scaleX, f32 scaleY, SdfGridCell *entry,
                   s32 context) {
    u8 mappedIds[6] = { 5, 15, 45, 52, 63, 68 };
    MnuMantraGridEntry *scene;
    s32 drawX;
    s32 drawY;
    s32 i;
    f32 pulse;

    {
        u32 flags;
        MenuSceneWork *display;

        display = (MenuSceneWork *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
        scene = (MnuMantraGridEntry *)(u32)entry->value;
        pulse = 0.0f;
        scene->frame++;
        if ((f32)scene->frame > 60.0f) {
            scene->frame = 0;
        }
        pulse = sdfSin(pulse);
        drawX = x + mnuMantraVariantSpritePlacements[scene->sceneId][2];
        drawY = y + mnuMantraVariantSpritePlacements[scene->sceneId][3];
        flags = mnuGetMantraDisplayFlags(scene, profileOwner);

        if (flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) {
            uiDrawUniformColorRect(drawX << 4, drawY << 3, 0, 0x300, 0x180,
                                   (s32)((f32)((alpha * 5) << 4) * 0.0078125f) |
                                       0x60501000,
                                   context);
        }
        {
            s32 displayFlags = (u8)display->boundsFlags;

            if (displayFlags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) {
                mnuDrawScaledScenePlacementSprite(x, y, depth, alpha, scene->sceneId, 0x20,
                              scaleX, scaleY, context);
            } else {
                for (i = 0; i < 6; i++) {
                    if (scene->sceneId == mappedIds[i]) {
                        mnuDrawScaledScenePlacementSprite(x, y, depth, alpha, i + 0x59, 0x20,
                                      scaleX, scaleY, context);
                        break;
                    }
                }
                if (i == 6) {
                    mnuDrawScaledScenePlacementSprite(x, y, depth, alpha, scene->sceneId, 0x20,
                                  scaleX, scaleY, context);
                }
            }
        }
        mnuDrawMantraEntryStatusIcon(drawX, drawY, depth,
                      (s32)((f32)alpha * (pulse * 0.4f + 0.2f)),
                      flags, scaleX, scaleY, context);
    }
}

extern u32 prfGetCapValue(u16);
extern u32 ptyGetProfileRecordValue(const struct datUnitWork_t *, u16);
extern u32 mnuGetMantraPrerequisiteFlags(s32, MnuMantraGridEntry *, MnuMantraNodeState *, MnuProfileProgress *);
extern const s8 D_003AF9B8[];

/* Fresh native reconstruction; not compiled or claimed matching.
 * Retail 00259B40..0025A680, 0xB40 bytes, frame 0x120.
 * Preserve the native partial clear: 21 bytes are cleared and populated,
 * while the final eight-entry draw loop reads active[14] through active[21].
 * The unexplained final-byte lifetime is retained, not zero-filled or diagnosed
 * as a retail bug. See SOURCE_REVIEW.md for the exact stack-write audit. */
void mnuDrawMantraGridEntryIndicators(s32 x, s32 y, s32 depth, s32 alpha,
                                    MnuProfileProgress *profileOwner, SdfGrid *grid,
                                    f32 scaleX, f32 scaleY, SdfGridCell *cell,
                                    s32 context) {
    u32 prerequisiteFlags[4];
    s32 prerequisiteIds[4];
    s8 firstCapGroup[3] = {24, 29, 34};
    struct MantraSecondaryMarkerCaps {
        s8 ids[2];
    } secondCapGroup = {{39, 44}};
    struct MantraSpecialSceneMarkerCaps {
        s8 ids[2];
    } specialCapGroup = {{52, 63}};
    s8 active[22];
    s8 profileIds[22];
    MnuMantraGridEntry *scene = (MnuMantraGridEntry *)(u32)cell->value;
    MantraPrerequisiteRecord *record;
    MnuMantraNodeState *states;
    s8 i;
    u32 cap;
    u32 sceneCap;
    u32 packedFlags;

    if (scene->sceneId == 45) {
        for (i = 0; i < 3; i++) {
            cap = prfGetCapValue((u16)firstCapGroup[i]);
            if (cap == ptyGetProfileRecordValue(profileOwner->partyRecord,
                                               (u16)firstCapGroup[i])) {
                mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, scene->sceneId, 0x1C, 0x20, scaleX, scaleY, context);
                break;
            }
        }
        for (i = 0; i < 2; i++) {
            cap = prfGetCapValue((u16)secondCapGroup.ids[i]);
            if (cap == ptyGetProfileRecordValue(profileOwner->partyRecord,
                                               (u16)secondCapGroup.ids[i])) {
                mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, scene->sceneId, 0x1D, 0x20, scaleX, scaleY, context);
                break;
            }
        }
    }
    if (scene->sceneId == 72) {
        for (i = 0; i < 2; i++) {
            cap = prfGetCapValue((u16)specialCapGroup.ids[i]);
            if (cap == ptyGetProfileRecordValue(profileOwner->partyRecord,
                                               (u16)specialCapGroup.ids[i])) {
                mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, scene->sceneId, 0x1C, 0x20, scaleX, scaleY, context);
                break;
            }
        }
    }

    sceneCap = prfGetCapValue(scene->sceneId);
    if (sceneCap != ptyGetProfileRecordValue(profileOwner->partyRecord, scene->sceneId)) {
        return;
    }
    memset(prerequisiteIds, 0, sizeof(prerequisiteIds));
    record = &mnuMantraPrerequisiteRecords[scene->sceneId];
    states = (MnuMantraNodeState *)(u32)grid->userData;
    if (record->kind != 0) {
        return;
    }
    packedFlags = 0;
    i = 0;
    if (record->ids[i] != 0) {
        do {
            if (states[record->ids[i]].state != 0) {
                prerequisiteFlags[i] = mnuGetMantraPrerequisiteFlags(i, scene, states, profileOwner);
                prerequisiteIds[i] = record->ids[i];
                if (prerequisiteFlags[i] & 4) {
                    packedFlags |= 1 << (i * 8);
                }
                if (prerequisiteFlags[i] & 2) {
                    packedFlags |= 2 << (i * 8);
                }
                if (prerequisiteFlags[i] & 0x10) {
                    packedFlags |= 4 << (i * 8);
                }
            }
            i++;
            if (i >= 4) {
                break;
            }
        } while (record->ids[i] != 0);
    }

    if (mnuMantraVariantSpritePlacements[scene->sceneId][5] & 0x100) {
        mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, scene->sceneId, 0x1D, 0x20, scaleX, scaleY, context);
    }
    if (mnuMantraVariantSpritePlacements[scene->sceneId][5] & 0x10) {
        mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, scene->sceneId, 0x1C, 0x20, scaleX, scaleY, context);
    }
    if (mnuMantraVariantSpritePlacements[scene->sceneId][5] & 1) {
        if (prerequisiteIds[0] != 0 && (packedFlags & 4)) {
            mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, prerequisiteIds[0], 0x1C, 0x20, scaleX, scaleY, context);
        }
        if (prerequisiteIds[1] != 0 && (packedFlags & 0x400)) {
            mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, prerequisiteIds[1], 0x1C, 0x20, scaleX, scaleY, context);
        }
    }
    if (mnuMantraVariantSpritePlacements[scene->sceneId][5] & 2) {
        if (prerequisiteIds[0] != 0 && (packedFlags & 4)) {
            mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, prerequisiteIds[0], 0x1D, 0x20, scaleX, scaleY, context);
        }
        if (prerequisiteIds[1] != 0 && (packedFlags & 0x400)) {
            mnuDrawScaledVariantSpriteByIndex(x, y, depth, alpha, prerequisiteIds[1], 0x1D, 0x20, scaleX, scaleY, context);
        }
    }

    if (scene->sceneId == 67) {
        mnuDrawVariantSpriteBySelector(x, y, depth, alpha, scene->sceneId, 0, 0x20, scaleX, scaleY, context);
        mnuDrawVariantSpriteBySelector(x, y, depth, alpha, scene->sceneId, 1, 0x20, scaleX, scaleY, context);
    } else if (scene->sceneId != 76 && scene->sceneId != 77) {
        if ((packedFlags & 0xFF00) == 0) {
            mnuDrawVariantSpriteBySelector(x, y, depth, alpha, scene->sceneId, 0, 0x20, scaleX, scaleY, context);
            if ((packedFlags & 5) == 4) {
                mnuDrawVariantSpriteAtCoordinatePlacement(x, y, depth, alpha,
                    scene->sceneId, prerequisiteIds[0], 0, 0x20, scaleX, scaleY, context);
            } else if (prerequisiteIds[0] == 72 || prerequisiteIds[0] == 19 ||
                       prerequisiteIds[0] == 56 || prerequisiteIds[0] == 75 ||
                       prerequisiteIds[0] == 67 || prerequisiteIds[0] == 45 ||
                       prerequisiteIds[0] == 46) {
                mnuDrawVariantSpriteAtCoordinatePlacement(x, y, depth, alpha,
                    scene->sceneId, prerequisiteIds[0], 0, 0x20, scaleX, scaleY, context);
            }
        } else if ((packedFlags & 0xFF) == 0) {
            mnuDrawVariantSpriteBySelector(x, y, depth, alpha, scene->sceneId, 1, 0x20, scaleX, scaleY, context);
            if ((packedFlags & 0x500) == 0x400) {
                mnuDrawVariantSpriteAtCoordinatePlacement(x, y, depth, alpha,
                    scene->sceneId, prerequisiteIds[1], 1, 0x20, scaleX, scaleY, context);
            }
        } else {
            mnuDrawVariantSpriteBySelector(x, y, depth, alpha, scene->sceneId, 0, 0x20, scaleX, scaleY, context);
            if ((packedFlags & 5) == 4) {
                mnuDrawVariantSpriteAtCoordinatePlacement(x, y, depth, alpha,
                    scene->sceneId, prerequisiteIds[0], 0, 0x20, scaleX, scaleY, context);
            }
            mnuDrawVariantSpriteBySelector(x, y, depth, alpha, scene->sceneId, 1, 0x20, scaleX, scaleY, context);
            if ((packedFlags & 0x500) == 0x400) {
                mnuDrawVariantSpriteAtCoordinatePlacement(x, y, depth, alpha,
                    scene->sceneId, prerequisiteIds[1], 1, 0x20, scaleX, scaleY, context);
            }
        }
    }

    /* This shared literal is owned by code_00258258, not this translation unit. */
    memcpy(profileIds, D_003AF9B8, sizeof(profileIds));
    memset(active, 0, 21);
    for (i = 0; i < 21; i++) {
        if (scene->sceneId == profileIds[i]) {
            active[i] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (active[i] != 0) {
            mnuDrawScaledSpriteAtCoordinatePlacement(x, y, depth, alpha, i + 0x2C, 0x20, scaleX, scaleY, context);
            mnuDrawScaledFixedSpritePlacement(x, y, depth, alpha, i * 2, 0x20, scaleX, scaleY, context);
            mnuDrawScaledFixedSpritePlacement(x, y, depth, alpha, i * 2 + 1, 0x20, scaleX, scaleY, context);
        }
    }
    for (i = 0; i < 5; i++) {
        if (active[i * 2 + 4] != 0) {
            mnuDrawScaledSpriteAtCoordinatePlacement(x, y, depth, alpha, i * 2 + 0x30, 0x20, scaleX, scaleY, context);
            mnuDrawScaledFixedSpritePlacement(x, y, depth, alpha, i * 3 + 8, 0x20, scaleX, scaleY, context);
        }
        if (active[i * 2 + 5] != 0) {
            mnuDrawScaledSpriteAtCoordinatePlacement(x, y, depth, alpha, i * 2 + 0x31, 0x20, scaleX, scaleY, context);
            mnuDrawScaledFixedSpritePlacement(x, y, depth, alpha, i * 3 + 10, 0x20, scaleX, scaleY, context);
        }
        if (active[i * 2 + 4] != 0 || active[i * 2 + 5] != 0) {
            mnuDrawScaledFixedSpritePlacement(x, y, depth, alpha, i * 3 + 9, 0x20, scaleX, scaleY, context);
        }
    }
    for (i = 0; i < 8; i++) {
        if (active[i + 14] != 0) {
            mnuDrawScaledFixedSpritePlacement(x, y, depth, alpha, i + 0x17, 0x20, scaleX, scaleY, context);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0024F0D0", D_003AF9B8);
