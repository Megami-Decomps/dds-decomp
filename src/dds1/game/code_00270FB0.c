#include "common.h"

extern void mnuDestroyWindowContainer(u32);

extern void mnuReleaseResourceList(u32);

extern s32 kwlnFadeIsActive(void);

extern u32 mnuGetValueRecordOwner(u32 *);

extern u32 effAppendListEntry(u32 *, char *, u32, u32, u32 *);

extern u32 D_0037C248[][2];

extern u32 D_0037C2C8[][2];

extern u32 D_0037C2F0[][2];

extern char D_003B1AD8[];

extern char D_003BC648[];

extern char D_003BC650[];

extern char D_003BC658[];

extern char D_003BC660[];

extern u32 mnuMovieDrawTask;

extern s32 mnuMovieShutdownCounter;

extern char mnuMovieViewerTaskName[];

extern s32 kwlnTaskGetUserValue();

extern u32 D_003DC5C8[];

extern char mnuCampInputTaskName[]; /* "camp" */

extern char mnuCampDrawTaskName[]; /* "camp_draw" */

extern char mnuCampOwnerTaskName[]; /* "camp_update" */

extern s8 mnuCampTaskState;

extern void effResolveAndReleaseResource(u32);

extern s32 D_003BC614;

extern u8 D_0037B950[];

extern u8 D_0037B970[];

extern u8 D_0037B980[];

extern u8 D_0037C388[];

extern const char D_003B2058[16];
extern char *D_0037C380[];
extern u32 effLoadMappedResource(char *base, char *name);
extern u32 *effCreateStatusBatch(u32 kind);

typedef struct ResourceRef8 {
    s32 index;
    s32 pad;
} ResourceRef8;

extern ResourceRef8 D_0037C210[];

extern char D_003B2020[];

extern u32 effLoadIndexedResource(char *, s32, s32);

#define MNU_STAFF_BASE_RESOURCE_COUNT 7
#define MNU_STAFF_MAIN_RESOURCE_COUNT 16
#define MNU_STAFF_EXTRA_RESOURCE_COUNT 5
#define MNU_STAFF_PAIR_RESOURCE_COUNT 2
#define MNU_STAFF_STATUS_BATCH_COUNT 2
#define MNU_STAFF_PANEL_COUNT 3
#define MNU_STAFF_PARTY_COUNT 5
#define MNU_STAFF_PARTY_RECORD_BYTES 0x1A4
#define MNU_STAFF_PARTY_RECORD_BASE 0xA60
#define MNU_STAFF_PARTY_PRESENT_BIT 1
#define MNU_STAFF_PARTY_CATEGORY 4
#define MNU_STAFF_RETAIN_RESOURCE 1
#define MNU_CAMP_STATE_IDLE 0
#define MNU_CAMP_STATE_ACTIVE 1
#define MNU_CAMP_STATE_CLEANED_UP 2
#define MNU_CAMP_CANCEL_MASK 8
#define MNU_CAMP_OPEN_FADE_FRAMES 0xF

/* Load the base image handles into the global table, retaining each resource. */
void mnuLoadStaffImageHandles(void) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        D_003DC5C8[resourceIndex] = effLoadIndexedResource(D_003B2020, D_0037C210[resourceIndex].index, MNU_STAFF_RETAIN_RESOURCE);
    }
}

/* Resolve each global handle, then copy its post-call value into destination. */
void mnuResolveStaffImageHandles(u32 *destination) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        u32 *sourceSlot = &D_003DC5C8[resourceIndex];
        u32 *destinationSlot = &destination[resourceIndex];

        effResolveAndReleaseResource(*sourceSlot);
        *destinationSlot = *sourceSlot;
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

void *mnuGetStaffCategoryEntries(s32 category, s32 *outEntryCount, u8 *menuBytes) {
    switch (category) {
    case 1:
        *outEntryCount = 4;
        return menuBytes + 0xe0;
    case 2:
        *outEntryCount = 2;
        return menuBytes + 0xd8;
    case 3:
        *outEntryCount = 2;
        return menuBytes + 0x7c;
    case MNU_STAFF_PARTY_CATEGORY:
        *outEntryCount = 9;
        return menuBytes + 0xf0;
    case 5:
        *outEntryCount = 1;
        return menuBytes + 0x114;
    default:
        *outEntryCount = 0;
        return 0;
    }
}

extern u8 *datGameState;

extern s8 D_003BC6B5;

/* Resolve entry zero and the active party's adjusted one-based model indices.
 * The supplied count and work are unused; model indices are not range-clamped. */
void movReleaseActivePartyCategoryModels(s32 modelListAddress, s32 unusedCount, u8 *unusedWork) {
    s32 partyIndex;

    effResolveAndReleaseResource(*(u32 *)modelListAddress);
    for (partyIndex = 0; partyIndex < MNU_STAFF_PARTY_COUNT; partyIndex++) {
        u8 *partyRecord = datGameState + MNU_STAFF_PARTY_RECORD_BASE + partyIndex * MNU_STAFF_PARTY_RECORD_BYTES;

        if ((*(u16 *)partyRecord & MNU_STAFF_PARTY_PRESENT_BIT) != 0) {
            s32 modelIndex = *(u16 *)(partyRecord + 4) + D_003BC6B5;

            effResolveAndReleaseResource(*(u32 *)(modelListAddress + modelIndex * 4 - 4));
        }
    }
}

/* Resolve all category entries, except party models selected by active records. */
void movReleaseCategoryModels(s32 category, u8 *menuBytes) {
    s32 entryCount;
    s32 *modelHandles = (s32 *)mnuGetStaffCategoryEntries(category, &entryCount, menuBytes);
    if (category != MNU_STAFF_PARTY_CATEGORY) {
        s32 resourceIndex;
        for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++) {
            effResolveAndReleaseResource(modelHandles[resourceIndex]);
        }
    } else {
        movReleaseActivePartyCategoryModels(modelHandles, entryCount, menuBytes);
    }
}

extern void effReleaseTextureHandlesAndResetSlots(u32);

/* Release each category entry's textures without overwriting the handle array. */
void mnuReleaseStaffCategoryTextureHandles(category, menuBytes)
s32 category;
u8 *menuBytes;
{
    s32 entryCount;
    s32 resourceIndex = 0;
    u8 *entryBytes = mnuGetStaffCategoryEntries(category, &entryCount, menuBytes);

    if (entryCount > 0) {
        u32 *handleCursor = (u32 *)entryBytes;
        do {
            effReleaseTextureHandlesAndResetSlots(*handleCursor++);
        } while (++resourceIndex < entryCount);
    }
}

/* The active resource category is stored in the staff task's display mode. */
typedef struct StaffMenuWork {
    u32 resource;
    u8 pad04[4];
    u8 panel[0x4C];
    u8 pad54[8];
    void *valueRecord;
    u8 pad60[0xB8];
    u32 primaryImage;
    u32 resourceList;
    u32 secondaryImage;
    u32 images[3];
    u32 extraImages[2];
    u32 scrollPanel; /* 0x138: created panel drawn and destroyed with this task */
    u8 background[0x6B0];
    u8 partyPanel[0x124];
    s32 displayMode;
    u8 timer[0x10];
} StaffMenuWork;

/* On a category change, release old textures and resolve the new model entries. */
void mnuSetStaffDisplayMode(s32 nextCategory, u8 *menuBytes) {
    s32 previousCategory = ((StaffMenuWork *)menuBytes)->displayMode;
    if (nextCategory != previousCategory) {
        if (previousCategory != 0) {
            /* Preserve DDS1's short-arity call through the K&R definition. */
            mnuReleaseStaffCategoryTextureHandles(previousCategory);
        }
        if (nextCategory != 0) {
            movReleaseCategoryModels(nextCategory, menuBytes);
        }
        ((StaffMenuWork *)menuBytes)->displayMode = nextCategory;
    }
}

typedef struct StaffStatusBatch {
    u32 references;
    u32 allocation;
    u32 payload;
} StaffStatusBatch;

typedef struct StaffStatusBatchPayload {
    u8 pad00[0x20];
    u32 *values;
} StaffStatusBatchPayload;

/* Load both mapped images and initialize the two status batches' word arrays.
 * Batch categories and the initial 0xF word remain opaque. */
void func_00271368(void *menuData) {
    StaffMenuWork *menu = (StaffMenuWork *)menuData;
    StaffStatusBatch *batch;
    StaffStatusBatchPayload *batchPayload;
    u32 *statusWords;
    u32 primaryResource;

    primaryResource = effLoadMappedResource(D_003B2058, D_0037C380[0]);
    menu->primaryImage = primaryResource;
    menu->secondaryImage = effLoadMappedResource(D_003B2058, D_0037C380[1]);

    batch = (StaffStatusBatch *)effCreateStatusBatch(6);
    batchPayload = (StaffStatusBatchPayload *)batch->payload;
    menu->extraImages[0] = (u32)batch;
    statusWords = batchPayload->values;
    statusWords[0] = 0xF;
    statusWords[1] = 0;
    statusWords[2] = 0;
    statusWords[3] = 0;
    statusWords[4] = 0;

    batch = (StaffStatusBatch *)effCreateStatusBatch(1);
    batchPayload = (StaffStatusBatchPayload *)batch->payload;
    menu->extraImages[1] = (u32)batch;
    statusWords = batchPayload->values;
    statusWords[0] = 0xF;
    statusWords[1] = 0;
}

/* Destroy the mapped images followed by both status batches. */
void mnuReleaseStaffSpriteHandles(StaffMenuWork *menu) {
    u32 *batchCursor = menu->extraImages;
    u32 batchIndex = 0;
    effDestroyPackedBatch(menu->primaryImage);
    effDestroyPackedBatch(menu->secondaryImage);
    do {
        effDestroyPackedBatch(*batchCursor++);
        batchIndex++;
    } while (batchIndex < MNU_STAFF_STATUS_BATCH_COUNT);
}

void mnuInitializeStaffPageWindows(u32 container, u32 *resources, u32 unused, u32 mode) {
    mnuInitPageWindow(container, mode, resources[3], 7, resources[4], 0, *resources, 0x11);
    func_0027FAA8(container, *resources);
    mnuCopyPrimaryWindowHandles(container, resources + 9);
    mnuCopySecondaryWindowHandles(container, resources + 0x11);
    mnuRegisterResourceHandles(container, resources + 0x19);
    mnuUpdateHandleStates(container);
}

/* Snapshot base handles, then queue the main, extra and paired sprite groups.
 * Only owner word 1 selects the second column of each image table. */
INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2058);

void mnuAppendCampSpriteRequests(u32 *resourceList, u32 *resourceSlots) {
    s32 resourceIndex;
    s32 tableColumn;

    mnuResolveStaffImageHandles(resourceSlots);
    tableColumn = mnuGetValueRecordOwner(resourceList) == 1;
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, D_003B2020, D_0037C248[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, resourceSlots + 0x24 / 4 + resourceIndex);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, D_003B2020, D_0037C2C8[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, resourceSlots + 0x64 / 4 + resourceIndex);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, "/camp/spr/n_sta/", D_0037C2F0[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, resourceSlots + 0x1C / 4 + resourceIndex);
    }
}

/* Clear base output slots, then destroy the main, extra and paired slot sets. */
void mnuReleaseStaffResourceGroups(u32 *resources) {
    u32 *shiftedSlots = resources + 1;
    s32 resourceIndex;

    mnuReleaseStaffImageHandles(resources);
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet(resources[9 + resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet(shiftedSlots[24 + resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet(resources[7 + resourceIndex]);
    }
}

typedef struct StaffSlots {
    u32 baseResources[7];    /* 0x00 */
    u32 pairResources[2];    /* 0x1C */
    u32 mainResources[16];   /* 0x24 */
    u32 extraResources[5];   /* 0x64 */
} StaffSlots;

/* Poll once, then return 1 only if every group contains nonzero handles.
 * DDS1 retains its no-argument poll call. */
s32 mnuStaffSlotsAllFilled(s32 unused, StaffSlots *slots) {
    s32 resourceIndex;

    effPollResourceList();
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
INCLUDE_ASM(const s32, "game/code_00270FB0", func_002717D8);


s64 mnuReleaseStaffResourceSlotGroups(u32 *resources) {
    s32 i;

    mnuReleaseStaffResourceGroups(resources + 0x18);
    for (i = 0; i < 2; i++) {
        effDestroyResourceSlotSet(resources[54 + i]);
    }
    for (i = 0; i < 4; i++) {
        effDestroyResourceSlotSet(resources[56 + i]);
    }
    for (i = 0; i < 9; i++) {
        effDestroyResourceSlotSet(resources[60 + i]);
    }
    return effDestroyResourceSlotSet(resources[69]);
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_002719F0);

void func_00271B40(void) {
}

void func_00271B48(void) {
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00271B50);

void mnuCreateStaffPanelSet(StaffMenuWork *menu) {
    menu->resourceList = func_0027D4A0(0, *(u32 *)((u8 *)menu + 0x6C), menu->secondaryImage);
    menu->images[0] = func_00271B50(D_0037B950, 8, 0x300, menu, D_0037C388);
    mnuForwardDupArg(menu->images[0], *(u32 *)((u8 *)menu + 0x74), 0, 0, 0);
    menu->images[1] = func_00271B50(D_0037B970, 3, 0x2C0, menu, 0);
    mnuSetWindowContainerState(menu->images[1], 0x100);
    menu->images[2] = func_00271B50(D_0037B980, 2, 0x200, menu, 0);
    mnuSetWindowContainerState(menu->images[2], 0x100);
}

/* Destroy the menu windows, then release their associated resource list. */
void mnuReleaseStaffSpriteAndResourceHandles(StaffMenuWork *menu) {
    u32 *windowCursor = menu->images;
    u32 windowIndex = 0;
    do {
        mnuDestroyWindowContainer(*windowCursor++);
    } while (++windowIndex < MNU_STAFF_PANEL_COUNT);
    mnuReleaseResourceList(menu->resourceList);
}

extern u32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(u32);
extern void *memset(void *, s32, u32);
extern void mnuClearPanelTransitionState(s32);
extern s8 dds3AdminReadPreviousSignedSample(void);
extern void *mnuAllocateValueRecord(void *);
extern void mnuInitPartyPanelSlots(void *);
extern void func_0027AD80(void *);
extern void evtCreateMessageWindowIfMissing(s32);
extern char D_0037B9E0[];
extern void func_00271368(void *);
extern void func_002717D8(void *);
extern void mnuResetGradientFadeColor(void *, s32);
extern void func_002E9708(void);

/* Allocate and clear menu work, select its image-table owner word from the
 * previous sample, then initialize the owned UI and resource state. */
StaffMenuWork *mnuCreateStaffCampWork(void) {
    u32 allocation = sdfAllocGeneralBlock(sizeof(StaffMenuWork));
    StaffMenuWork *menu = (StaffMenuWork *)sdfResourceRetainAddress(allocation);

    memset(menu, 0, sizeof(*menu));
    menu->resource = allocation;
    mnuClearPanelTransitionState((s32)menu->panel);
    /* The opaque owner word selects the normal or alternate staff image table. */
    if (dds3AdminReadPreviousSignedSample() != 0) {
        menu->valueRecord = mnuAllocateValueRecord((void *)1);
    } else {
        menu->valueRecord = mnuAllocateValueRecord(NULL);
    }
    mnuInitPartyPanelSlots(menu->partyPanel);
    func_0027AD80(menu->background);
    evtCreateMessageWindowIfMissing((s32)D_0037B9E0);
    func_00271368(menu);
    func_002717D8(menu);
    mnuResetGradientFadeColor(menu->timer, 0x40);
    func_002E9708();
    return menu;
}

extern s32 kwlnTaskGetUserValue();

extern s8 mnuCampTaskState;

extern void sdfReleaseResourceAllocation(u32);

/* Ignore null task userdata; otherwise drain transitions and release owned
 * resources in shutdown order before marking camp cleanup complete. */
void mnuDestroyStaffMenuTask(u32 task) {
    StaffMenuWork *menu = (StaffMenuWork *)kwlnTaskGetUserValue(task);
    if (menu == NULL) {
        return;
    }
    mnuDrainPanelTransitions(menu->panel, task);
    mnuReleaseStaffSpriteAndResourceHandles(menu);
    mnuDestroyScrollPanel(menu->scrollPanel);
    mnuShutdownContext(menu->background + 0x20);
    dspCloseChannel();
    mnuReleaseAssets(menu->background);
    mnuReleaseStaffResourceSlotGroups((u32 *)menu);
    mnuReleaseStaffSpriteHandles(menu);
    func_002BC618((u32)menu->valueRecord);
    sdfReleaseResourceAllocation(menu->resource);
    mnuCampTaskState = MNU_CAMP_STATE_CLEANED_UP;
    func_002E9730();
}

u32 func_00271FC8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuDrawAndStepGradientFade(context + 0x914, 0x53);
    return 0;
}

/* Mapped cancel input closes camp only through the full guard chain.
 * Return -1 on closure, otherwise 0; blocked nested guards use the alternate sound. */
s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 mappedButtons = mnuMapPadMaskToFlags(MNU_CAMP_CANCEL_MASK);
    s32 result;

    if (func_002719F0(menu) == 0) {
        return 0;
    }
    result = 0;
    if (evtGetMessageWindowControlState() == 0) {
        if (mappedButtons & MNU_CAMP_CANCEL_MASK) {
            if (func_002877A8() != 1) {
                if (fileConsumeConfigTaskReady() == 0) {
                    mnuDestroyCampTasks();
                    mnuPlayInputSound(0, 2, 0);
                    return -1;
                }
            }
            mnuPlayInputSound(0, 0x8000, 0);
        }
    }
    return result;
}

extern void func_00272798();
extern void func_002728F8();
extern void func_002729C8();
extern void func_00101A80(s32, s32);
extern void kwlnFadeOutStart(s8, s8, s8, s32);

/* Share menu userdata across the input/draw/owner tasks, attach fade and cancel
 * tasks to drawing, then start the opening fade and mark camp active. */
INCLUDE_RODATA(const s32, "game/code_00270FB0", mnuCampDrawTaskName);

INCLUDE_RODATA(const s32, "game/code_00270FB0", mnuCampOwnerTaskName);

void mnuCreateCampTasks(void) {
    s32 menuAddress;
    s32 drawTask;

    menuAddress = (s32)mnuCreateStaffCampWork();
    kwlnTaskCreate(mnuCampInputTaskName, 0x3F2, 1, 0, func_00272798, 0, menuAddress);
    drawTask = kwlnTaskCreate(mnuCampDrawTaskName, 0x2B07, 1, 0, func_002728F8, 0, menuAddress);
    kwlnTaskCreate(mnuCampOwnerTaskName, 0x520B, 1, 0, func_002729C8, mnuDestroyStaffMenuTask, menuAddress);
    func_00101A80(drawTask, kwlnTaskCreate("camp_fade", 0x2B08, 1, 0, func_00271FC8, 0, menuAddress));
    func_00101A80(drawTask, kwlnTaskCreate("camp_all_cancel", 0x3F3, 1, 0, mnuStaffCampCancelCheck, 0, menuAddress));
    kwlnFadeOutStart(0, 0, 0, MNU_CAMP_OPEN_FADE_FRAMES);
    mnuCampTaskState = MNU_CAMP_STATE_ACTIVE;
}

/* Destroy the three named camp task hierarchies; owner teardown frees userdata. */
void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(mnuCampInputTaskName, 0);
    kwlnTaskDestroyWithHierarchyByName(mnuCampDrawTaskName, 0);
    kwlnTaskDestroyWithHierarchyByName(mnuCampOwnerTaskName, 0);
}

/* Report active camp; acknowledge cleanup only for the exact cleaned-up state.
 * Other non-active states return 0 without being rewritten. */
s32 mnuAcknowledgeCampState(void) {
    s8 campState = mnuCampTaskState;
    if (campState == MNU_CAMP_STATE_ACTIVE) {
        return 1;
    }
    if (campState < MNU_CAMP_STATE_CLEANED_UP) {
        return 0;
    }
    if (campState == MNU_CAMP_STATE_CLEANED_UP) {
        mnuCampTaskState = MNU_CAMP_STATE_IDLE;
    }
    return 0;
}

/* Return 1 exactly when the kernel reports no active fade. */
u8 mnuIsFadeIdle(void) {
    s64 fadeActive;

    fadeActive = kwlnFadeIsActive();
    return fadeActive == 0;
}

extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern s32 func_003014F0(char *, const char *, s32);
extern s32 func_00197A98(s32, s32, s32, u32, u32, s32);
extern void func_001958A0(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);
extern u8 *datGameState;
extern char D_003BC6C0[];


void mnuDrawStaffCampSlotsAndCurrency(s32 unused0, s32 unused1, s32 z, s32 firstSlot,
                   s32 secondSlot, s32 drawFlags, s32 unused6, s32 unused7) {
    char text[0x10];
    s32 glyph;

    itfDrawGridWithResolvedSlot(0x150, 0xD08, 0, 1, firstSlot, 0, drawFlags);
    itfDrawGridWithResolvedSlot(0x2B0, 0xCE8, 0, 1, secondSlot, 3, drawFlags);
    func_003014F0(text, D_003BC6C0, *(s32 *)(datGameState + 0x3C));
    glyph = func_00197A98(0x4B0, 0xCD8, z, 0x80808080, (u32)text, 0);
    func_001958A0(glyph, 1, drawFlags);
    frFontQueueGlyphInSelectedSlot(glyph);
}

extern u32 D_0037B988[];

/* Create and queue the table-selected image sprite; imageIndex is unchecked. */
void mnuCreateStaffImageSprite(s32 imageIndex) {
    u32 *sprite = (u32 *)func_00197760(0x2F0, 0x1E0, 0, 0xa09dc35a,
                                      D_0037B988[imageIndex], 0);
    func_001958A0(sprite, 1, 0x54);
    frFontQueueGlyphInSelectedSlot(sprite);
}
INCLUDE_ASM(const s32, "game/code_00270FB0", func_002723B0);

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00272518);

void func_00272668(s32 kind, s32 labelIndex, s32 textTable, s32 context, s32 drawOption, s32 layer) {
    func_00272518(kind, labelIndex, textTable, context, drawOption, 0, layer);
}

void mnuDrawStaffCampScreen(s32 kind, s32 task) {
    u8 *menu = (u8 *)kwlnTaskGetUserValue(task);

    mnuDrawBackdrop(menu + 0x13C, 0x20);
    if (func_002719F0(task) == 0) {
        return;
    }
    func_0027E8D8(-0x10, -8, 0, (s32)((StaffMenuWork *)menu)->scrollPanel, 0x53);
    mnuDrawPanelListDefault(0, 0, 0, menu + 0x15C, 0x53);
    if (kind == 0) {
        itfDrawGridWithResolvedSlot(0x1AB0, 0x70, 0, 1, *(s32 *)(menu + 0x64), 6, 0x53);
        itfDrawGridWithResolvedSlot(0x17A0, 0x78, 0, 1, *(s32 *)(menu + 0x60), 0xF, 0x53);
        itfDrawGridWithResolvedSlot(0x1E40, 0x78, 0, 1, *(s32 *)(menu + 0x60), 0x10, 0x53);
    }
}

void func_00272778(u32 task) {
    mnuDrawStaffCampScreen(0, task);
}

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2100);

INCLUDE_SDATA(const s32, "game/code_00270FB0", mnuCampTaskState);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6B5);

INCLUDE_SDATA(const s32, "game/code_00270FB0", mnuCampInputTaskName);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6C0);

