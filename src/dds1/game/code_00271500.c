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

extern u32 D_0037C248[][2];

extern u32 D_0037C2C8[][2];

extern u32 D_0037C2F0[][2];

extern char D_003B2020[];

#define MNU_STAFF_BASE_RESOURCE_COUNT 7

#define MNU_STAFF_MAIN_RESOURCE_COUNT 16

#define MNU_STAFF_EXTRA_RESOURCE_COUNT 5

#define MNU_STAFF_PAIR_RESOURCE_COUNT 2

#define MNU_STAFF_RETAIN_RESOURCE 1

void mnuInitializeStaffPageWindows(MenuPageWindow *container, StaffSlots *resources, u32 unused, PartyPanel *partyPanel);

/* Release textures through the global handles and clear the caller's slots. */ void mnuReleaseStaffImageHandles(u32 *resources);

/* Resolve each global handle, then copy its post-call value into destination. */ void mnuResolveStaffImageHandles(u32 *destination);

void mnuAppendCampSpriteRequests(EffectList *resourceList, StaffSlots *resourceSlots) {
    s32 resourceIndex;
    s32 tableColumn;

    mnuResolveStaffImageHandles((u32 *)resourceSlots->baseResources);
    tableColumn = mnuGetValueRecordOwner(resourceList) == 1;
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, (u32)D_003B2020, D_0037C248[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&resourceSlots->mainResources[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, (u32)D_003B2020, D_0037C2C8[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&resourceSlots->extraResources[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, (u32)"/camp/spr/n_sta/", D_0037C2F0[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&resourceSlots->pairResources[resourceIndex]);
    }
}

/* Clear base output slots, then destroy the main, extra and paired slot sets. */
void mnuReleaseStaffResourceGroups(StaffSlots *resources) {
    s32 resourceIndex;

    mnuReleaseStaffImageHandles((u32 *)resources->baseResources);
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet(resources->mainResources[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet(resources->extraResources[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet(resources->pairResources[resourceIndex]);
    }
}

/* Poll once, then require nonzero handles in every resource bank. */
s32 mnuStaffSlotsAllFilled(EffectList *resourceList, StaffSlots *slots) {
    s32 resourceIndex;

    effPollResourceList(resourceList);
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        if (slots->baseResources[resourceIndex] == 0) {
            return 0;
        }
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        if (slots->mainResources[resourceIndex] == 0) {
            return 0;
        }
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        if (slots->extraResources[resourceIndex] == 0) {
            return 0;
        }
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        if (slots->pairResources[resourceIndex] == 0) {
            return 0;
        }
    }
    return 1;
}

const char D_003B2080[] = "/camp/spr/";
const char D_003B2090[] = "/camp/spr/n_pty/";
const char D_003B20A8[] = "/camp/spr/n_con/";

extern u32 D_0037C300[][2];
extern u32 D_0037C310[][2];
extern u32 D_0037C330[][2];
extern u32 D_0037C378[][2];

/* Queue the category, group, party and single staff resources after the base banks. */
void func_002717D8(StaffMenuWork *menu) {
    s32 resourceIndex;
    s32 tableColumn;
    EffectList *resourceList = (EffectList *)menu->resourceQueue;

    mnuAppendCampSpriteRequests(resourceList, &menu->staffSlots);
    tableColumn = mnuGetValueRecordOwner(resourceList) == 1;
    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        effAppendListEntry(resourceList, (u32)D_003B2080, D_0037C300[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&menu->categoryPair[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < 4; resourceIndex++) {
        effAppendListEntry(resourceList, (u32)D_003B2080, D_0037C310[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&menu->categoryGroup[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < 9; resourceIndex++) {
        effAppendListEntry(resourceList, (u32)D_003B2090, D_0037C330[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&menu->partyModels[resourceIndex]);
    }
    effAppendListEntry(resourceList, (u32)D_003B20A8, D_0037C378[0][tableColumn], MNU_STAFF_RETAIN_RESOURCE, (u32)&menu->singleResource);
}

s64 mnuReleaseStaffResourceSlotGroups(StaffMenuWork *menu) {
    s32 i;

    mnuReleaseStaffResourceGroups(&menu->staffSlots);
    for (i = 0; i < 2; i++) {
        effDestroyResourceSlotSet(menu->categoryPair[i]);
    }
    for (i = 0; i < 4; i++) {
        effDestroyResourceSlotSet(menu->categoryGroup[i]);
    }
    for (i = 0; i < 9; i++) {
        effDestroyResourceSlotSet(menu->partyModels[i]);
    }
    return (s32)effDestroyResourceSlotSet(menu->singleResource);
}

INCLUDE_ASM(const s32, "game/code_00271500", func_002719F0);

void func_00271B40(void) {
}

void func_00271B48(void) {
}

extern s32 mdlFlagTest(s32);

extern void mnuInitializeWindowEntryPlacement(s32, MenuWindowContainer *, s32, s32, s32);

extern void mnuSetWindowPanelBounds(MenuWindowContainer *, const void *, u32, u32, u32, u32);

/* Filter staff entries, size the window and preserve each entry's original ordinal. */
MenuWindowContainer *mnuCreateFilteredStaffEntryWindow(void *const *entries, s32 count, s32 width,
                                     StaffMenuWork *work, const s32 *flagIds) {
    MenuWindowContainer *window;
    s32 visibleCount = 0;
    s32 index;
    s32 style;
    void *const *entryCursor;
    u32 entryOffset;

    if (flagIds != NULL) {
        index = 0;
        if (count > 0) {
            const s32 *flagCursor = flagIds;
            do {
                s32 flag = *flagCursor;
                if (flag == 0 || mdlFlagTest(flag) != 0) {
                    visibleCount++;
                }
                index++;
                flagCursor++;
            } while (index < count);
        }
    } else {
        visibleCount = count;
    }

    window = mnuCreateWindowContainer(0, width, 0x10, visibleCount, 0x15);

    switch (visibleCount) {
    case 2:
        style = 0x26;
        break;
    case 3:
        style = 0x28;
        break;
    case 4:
        style = 0x2A;
        break;
    case 5:
        style = 0x2C;
        break;
    default:
        window->unk10 = 0xA8;
        style = 0xE;
        break;
    }
    mnuInitializeWindowEntryPlacement(0, window, (s32)work->staffSlots.baseResources[5], 0xA, style);

    index = 0;
    if (count > 0) {
        /* Flags and entry handles use parallel word offsets. */
        entryCursor = entries;
        entryOffset = 0;
        do {
            struct MenuListNode *node = NULL;

            if (flagIds != NULL) {
                s32 flag = *(const s32 *)(entryOffset + (u32)flagIds);
                if (flag != 0) {
                    if (mdlFlagTest(flag) != 0) {
                        node = mnuAppendWindowListNode(window, *entryCursor);
                    }
                } else {
                    node = mnuAppendWindowListNode(window,
                        *(void *const *)(entryOffset + (u32)entries));
                }
            } else {
                node = mnuAppendWindowListNode(window, *entryCursor);
            }
            if (node != NULL) {
                node->sortKeyPrimary = index;
            }
            index++;
            entryCursor++;
            entryOffset += sizeof(*entries);
        } while (index < count);
    }

    mnuSetWindowPanelBounds(window, work->resourceList,
                            0x30, 0x530, 0xA0, 0x8D0);
    return window;
}

INCLUDE_RODATA(const s32, "game/code_00271500", mnuCampDrawTaskName);

INCLUDE_RODATA(const s32, "game/code_00271500", mnuCampOwnerTaskName);

