#include "mnu_input.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "eff_resource_list.h"
#include "common.h"
#include "fr_font.h"
#include "itf_draw_grid.h"
#include "mnu_staff.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "mnu.h"
#include "eff.h"
#include "mnu_shop.h"
#include "mnu_camp_work.h"
#include "mnu_list.h"
#include "kwln_task_lifecycle.h"

extern struct EffectSlotSet *D_003DC5C8[];

extern const char D_003B2058[16];

extern char *D_0037C380[];

typedef struct ResourceRef8 {
    s32 index;
    s32 pad;
} ResourceRef8;

extern ResourceRef8 D_0037C210[];

extern char D_003B2020[];

#define MNU_STAFF_BASE_RESOURCE_COUNT 7

#define MNU_STAFF_STATUS_BATCH_COUNT 2

#define MNU_STAFF_PARTY_COUNT 5

#define MNU_STAFF_PARTY_PRESENT_BIT 1

#define MNU_STAFF_PARTY_CATEGORY 4

#define MNU_STAFF_RETAIN_RESOURCE 1

/* Load the base image handles into the global table, retaining each resource. */
void mnuLoadStaffImageHandles(void) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        D_003DC5C8[resourceIndex] = effLoadIndexedResource(D_003B2020, (const char *)D_0037C210[resourceIndex].index, MNU_STAFF_RETAIN_RESOURCE);
    }
}

/* Resolve each global handle, then copy its post-call value into destination. */
void mnuResolveStaffImageHandles(u32 *destination) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        struct EffectSlotSet **sourceSlot = &D_003DC5C8[resourceIndex];
        u32 *destinationSlot = &destination[resourceIndex];

        effResolveAndReleaseResource(*sourceSlot);
        *destinationSlot = (u32)*sourceSlot;
    }
}

/* Release textures through the global handles and clear the caller's slots. */
void mnuReleaseStaffImageHandles(u32 *resources) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        u32 *outputSlot = &resources[resourceIndex];
        effReleaseTextureHandlesAndResetSlots(D_003DC5C8[resourceIndex]);
        *outputSlot = 0;
    }
}

/* Return this category's handle array and count; unknown categories have none. */
INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2020);

struct EffectSlotSet **mnuGetStaffCategoryEntries(s32 category, s32 *outEntryCount,
                                                  StaffMenuWork *menu) {
    switch (category) {
    case 1:
        *outEntryCount = 4;
        return menu->categoryGroup;
    case 2:
        *outEntryCount = 2;
        return menu->categoryPair;
    case 3:
        *outEntryCount = 2;
        return menu->staffSlots.pairResources;
    case MNU_STAFF_PARTY_CATEGORY:
        *outEntryCount = 9;
        return menu->partyModels;
    case 5:
        *outEntryCount = 1;
        return &menu->singleResource;
    default:
        *outEntryCount = 0;
        return 0;
    }
}

extern s8 D_003BC6B5;

extern void movReleaseActivePartyCategoryModels(struct EffectSlotSet **, s32, StaffMenuWork *);

/* Release the base model and the adjusted one-based indices of active party models. */
void movReleaseActivePartyCategoryModels(struct EffectSlotSet **modelHandles, s32 unusedCount,
                                         StaffMenuWork *unusedWork) {
    s32 partyIndex;

    effResolveAndReleaseResource(modelHandles[0]);
    for (partyIndex = 0; partyIndex < MNU_STAFF_PARTY_COUNT; partyIndex++) {
        DatPartyRecord *partyRecord = &datGameState->party[partyIndex];

        if ((partyRecord->flags & MNU_STAFF_PARTY_PRESENT_BIT) != 0) {
            s32 modelIndex = partyRecord->unitId + D_003BC6B5;

            effResolveAndReleaseResource((modelHandles + modelIndex)[-1]);
        }
    }
}

/* Resolve all category entries, except party models selected by active records. */
void movReleaseCategoryModels(s32 category, StaffMenuWork *menu) {
    s32 entryCount;
    struct EffectSlotSet **modelHandles = mnuGetStaffCategoryEntries(category, &entryCount, menu);
    if (category != MNU_STAFF_PARTY_CATEGORY) {
        s32 resourceIndex;
        for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++) {
            effResolveAndReleaseResource(modelHandles[resourceIndex]);
        }
    } else {
        movReleaseActivePartyCategoryModels(modelHandles, entryCount, menu);
    }
}

/* Release each category entry's textures without overwriting the handle array. */
void mnuReleaseStaffCategoryTextureHandles(category, menu)
s32 category;
StaffMenuWork *menu;
{
    s32 entryCount;
    s32 resourceIndex = 0;
    struct EffectSlotSet **entries = mnuGetStaffCategoryEntries(category, &entryCount, menu);

    if (entryCount > 0) {
        struct EffectSlotSet **handleCursor = entries;
        do {
            effReleaseTextureHandlesAndResetSlots(*handleCursor++);
        } while (++resourceIndex < entryCount);
    }
}

/* On a category change, release old textures and resolve the new model entries. */
void mnuSetStaffDisplayMode(s32 nextCategory, StaffMenuWork *menu) {
    s32 previousCategory = menu->displayMode;
    if (nextCategory != previousCategory) {
        if (previousCategory != 0) {
            /* Preserve DDS1's short-arity call through the K&R definition. */
            mnuReleaseStaffCategoryTextureHandles(previousCategory);
        }
        if (nextCategory != 0) {
            movReleaseCategoryModels(nextCategory, menu);
        }
        menu->displayMode = nextCategory;
    }
}

/* Load both mapped images and initialize the two status batches' word arrays.
 * Batch categories and the initial 0xF word remain opaque. */
void func_00271368(void *menuData) {
    StaffMenuWork *menu = (StaffMenuWork *)menuData;
    EffMappedResource *batch;
    EffMappedRecord *record;
    u32 *statusWords;
    EffMappedResource *primaryResource;

    primaryResource = effLoadMappedResource(D_003B2058, D_0037C380[0]);
    menu->primaryImage = primaryResource;
    menu->secondaryImage = effLoadMappedResource(D_003B2058, D_0037C380[1]);

    batch = effCreateStatusBatch(6);
    record = batch->records;
    menu->extraImages[0] = batch;
    statusWords = (u32 *)record->status;
    statusWords[0] = 0xF;
    statusWords[1] = 0;
    statusWords[2] = 0;
    statusWords[3] = 0;
    statusWords[4] = 0;

    batch = effCreateStatusBatch(1);
    record = batch->records;
    menu->extraImages[1] = batch;
    statusWords = (u32 *)record->status;
    statusWords[0] = 0xF;
    statusWords[1] = 0;
}

/* Destroy the mapped images followed by both status batches. */
void mnuReleaseStaffSpriteHandles(StaffMenuWork *menu) {
    EffMappedResource **batchCursor = menu->extraImages;
    u32 batchIndex = 0;
    effDestroyPackedBatch(menu->primaryImage);
    effDestroyPackedBatch(menu->secondaryImage);
    do {
        effDestroyPackedBatch(*batchCursor++);
        batchIndex++;
    } while (batchIndex < MNU_STAFF_STATUS_BATCH_COUNT);
}

void mnuInitializeStaffPageWindows(MenuPageWindow *container, StaffSlots *resources,
                                   u32 unused, PartyPanel *partyPanel) {
    mnuInitPageWindow((u32)container, (u32)partyPanel, (u32)resources->baseResources[3], 7,
                      (u32)resources->baseResources[4], 0, (u32)resources->baseResources[0], 0x11);
    func_0027FAA8((u32)container, (u32)resources->baseResources[0]);
    mnuCopyPrimaryWindowHandles(container, resources->mainResources);
    mnuCopySecondaryWindowHandles(container, resources->mainResources + 8);
    mnuRegisterResourceHandles(container, resources->extraResources);
    mnuUpdateHandleStates(container);
}

/* Snapshot base handles, then queue the main, extra and paired sprite groups.
 * Only owner word 1 selects the second column of each image table. */

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2058);

INCLUDE_SDATA(const s32, "game/code_00270FB0", mnuCampTaskState);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6B5);

INCLUDE_SDATA(const s32, "game/code_00270FB0", mnuCampInputTaskName);

