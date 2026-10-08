#include "common.h"
#include "dat_state.h"
#include "mnu.h"
#include "eff.h"
#include "mnu_shop.h"

typedef struct StaffMenuWork {
    u32 resource;
    u8 pad04[4];
    MenuPopupState panel;
    u8 pad54[8];
    EffectList *resourceQueue;
    StaffSlots staffSlots;
    u32 categoryPair[2];
    u32 categoryGroup[4];
    u32 partyModels[9];
    u32 singleResource;
    u32 primaryImage;
    MenuPanelHandles *resourceList;
    u32 secondaryImage;
    u32 images[3];
    u32 extraImages[2];
    u32 scrollPanel;
    u8 background[0x6B0];
    u8 partyPanel[0x124];
    s32 displayMode;
    u8 timer[0x10];
} StaffMenuWork;

typedef char StaffMenuWork_size_must_be_0x924[(sizeof(StaffMenuWork) == 0x924) ? 1 : -1];
typedef char StaffMenuWork_staffSlots_offset_check[
    ((u32)&((StaffMenuWork *)0)->staffSlots == 0x60) ? 1 : -1];

extern void mnuDestroyWindowContainer(u32);

extern MenuPanelHandles *mnuCreatePanelSpriteHandles(u32, s32, s32);
extern void mnuReleaseResourceList(MenuPanelHandles *);

extern s32 kwlnFadeIsActive(void);

extern u32 mnuGetValueRecordOwner(const EffectList *);

extern s32 effAppendListEntry(EffectList *, u32, u32, u32, u32);
extern s32 effPollResourceList(EffectList *);
extern void func_002BC618(EffectList *);

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
extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

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

u32 *mnuGetStaffCategoryEntries(s32 category, s32 *outEntryCount, StaffMenuWork *menu) {
    switch (category) {
    case 1:
        *outEntryCount = 4;
        return menu->categoryGroup;
    case 2:
        *outEntryCount = 2;
        return menu->categoryPair;
    case 3:
        *outEntryCount = 2;
        return (u32 *)menu->staffSlots.pairResources;
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

extern void movReleaseActivePartyCategoryModels(u32 *, s32, StaffMenuWork *);
/* Release the base model and the adjusted one-based indices of active party models. */
void movReleaseActivePartyCategoryModels(u32 *modelHandles, s32 unusedCount, StaffMenuWork *unusedWork) {
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
    u32 *modelHandles = mnuGetStaffCategoryEntries(category, &entryCount, menu);
    if (category != MNU_STAFF_PARTY_CATEGORY) {
        s32 resourceIndex;
        for (resourceIndex = 0; resourceIndex < entryCount; resourceIndex++) {
            effResolveAndReleaseResource(modelHandles[resourceIndex]);
        }
    } else {
        movReleaseActivePartyCategoryModels(modelHandles, entryCount, menu);
    }
}

extern void effReleaseTextureHandlesAndResetSlots(u32);

/* Release each category entry's textures without overwriting the handle array. */
void mnuReleaseStaffCategoryTextureHandles(category, menu)
s32 category;
StaffMenuWork *menu;
{
    s32 entryCount;
    s32 resourceIndex = 0;
    u32 *entries = mnuGetStaffCategoryEntries(category, &entryCount, menu);

    if (entryCount > 0) {
        u32 *handleCursor = entries;
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

void mnuInitializeStaffPageWindows(MenuPageWindow *container, StaffSlots *resources,
                                   u32 unused, PartyPanel *partyPanel) {
    mnuInitPageWindow((u32)container, (u32)partyPanel, resources->baseResources[3], 7,
                      resources->baseResources[4], 0, resources->baseResources[0], 0x11);
    func_0027FAA8((u32)container, resources->baseResources[0]);
    mnuCopyPrimaryWindowHandles(container, resources->mainResources);
    mnuCopySecondaryWindowHandles(container, resources->mainResources + 8);
    mnuRegisterResourceHandles(container, resources->extraResources);
    mnuUpdateHandleStates(container);
}

/* Snapshot base handles, then queue the main, extra and paired sprite groups.
 * Only owner word 1 selects the second column of each image table. */
INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2058);

void mnuAppendCampSpriteRequests(EffectList *resourceList, StaffSlots *resourceSlots) {
    s32 resourceIndex;
    s32 tableColumn;

    mnuResolveStaffImageHandles(resourceSlots->baseResources);
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

    mnuReleaseStaffImageHandles(resources->baseResources);
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet((u32)resources->mainResources[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet((u32)resources->extraResources[resourceIndex]);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        effDestroyResourceSlotSet((u32)resources->pairResources[resourceIndex]);
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
INCLUDE_ASM(const s32, "game/code_00270FB0", func_002717D8);


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
    return effDestroyResourceSlotSet(menu->singleResource);
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_002719F0);

void func_00271B40(void) {
}

void func_00271B48(void) {
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00271B50);

void mnuCreateStaffPanelSet(StaffMenuWork *menu) {
    menu->resourceList = mnuCreatePanelSpriteHandles(0, menu->staffSlots.baseResources[3], menu->secondaryImage);
    menu->images[0] = func_00271B50(D_0037B950, 8, 0x300, menu, D_0037C388);
    mnuForwardDupArg(menu->images[0], menu->staffSlots.baseResources[5], 0, 0, 0);
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
extern s8 dds3AdminReadPreviousSignedSample(void);
extern EffectList *mnuAllocateValueRecord(u32);
extern void mnuInitPartyPanelSlots(void *);
extern void func_0027AD80(void *);
extern void evtCreateMessageWindowIfMissing(s32);
extern char D_0037B9E0[];
extern void func_00271368(void *);
extern void func_002717D8(void *);
extern void mnuResetGradientFadeColor(void *, s32);
extern void func_002E9708(void);

/* Allocate and clear menu work, select its request-list mode from the
 * previous sample, then initialize the owned UI and resource state. */
StaffMenuWork *mnuCreateStaffCampWork(void) {
    u32 allocation = sdfAllocGeneralBlock(sizeof(StaffMenuWork));
    StaffMenuWork *menu = (StaffMenuWork *)sdfResourceRetainAddress(allocation);

    memset(menu, 0, sizeof(*menu));
    menu->resource = allocation;
    mnuClearPanelTransitionState(&menu->panel);
    /* Mode also selects the normal or alternate staff image table. */
    if (dds3AdminReadPreviousSignedSample() != 0) {
        menu->resourceQueue = mnuAllocateValueRecord(1);
    } else {
        menu->resourceQueue = mnuAllocateValueRecord(0);
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
    mnuDrainPanelTransitions(&menu->panel, task);
    mnuReleaseStaffSpriteAndResourceHandles(menu);
    mnuDestroyScrollPanel(menu->scrollPanel);
    mnuShutdownContext((MenuPageWindow *)(menu->background + 0x20));
    dspCloseChannel();
    mnuReleaseAssets(menu->background);
    mnuReleaseStaffResourceSlotGroups(menu);
    mnuReleaseStaffSpriteHandles(menu);
    func_002BC618(menu->resourceQueue);
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

extern void mnuPlayInputSound(s32, s32, u32 *);

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

extern void mnuHandleCampFieldSkillInput();
extern void func_002728F8();
extern s32 func_002729C8(s32);
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
    kwlnTaskCreate(mnuCampInputTaskName, 0x3F2, 1, 0, mnuHandleCampFieldSkillInput, 0, menuAddress);
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
typedef struct FrFontGlyph FrFontGlyph;
extern s32 func_001958A0(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern char D_003BC6C0[];


void mnuDrawStaffCampSlotsAndCurrency(s32 unused0, s32 unused1, s32 z, s32 firstSlot,
                   s32 secondSlot, s32 drawFlags, s32 unused6, s32 unused7) {
    char text[0x10];
    s32 glyph;

    itfDrawGridWithResolvedSlot(0x150, 0xD08, 0, 1, firstSlot, 0, drawFlags);
    itfDrawGridWithResolvedSlot(0x2B0, 0xCE8, 0, 1, secondSlot, 3, drawFlags);
    func_003014F0(text, D_003BC6C0, datGameState->header.currency);
    glyph = func_00197A98(0x4B0, 0xCD8, z, 0x80808080, (u32)text, 0);
    func_001958A0((FrFontGlyph *)glyph, 1, drawFlags);
    frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
}

extern u8 *D_0037B988[];

/* Create and queue the table-selected image sprite; imageIndex is unchecked. */
void mnuCreateStaffImageSprite(s32 imageIndex) {
    FrFontGlyph *sprite = (FrFontGlyph *)itfCreateConvertedTextGlyph(0x2F0, 0x1E0, 0, 0xa09dc35a,
                                      D_0037B988[imageIndex], 0);
    func_001958A0(sprite, 1, 0x54);
    frFontQueueGlyphInSelectedSlot(sprite);
}
typedef struct StaffGridLabelRow {
    s32 count;
    s32 x[5];
    s8 gridIds[5];
    u8 pad1D[3];
} StaffGridLabelRow;

typedef char StaffGridLabelRow_size_check[sizeof(StaffGridLabelRow) == 0x20 ? 1 : -1];
typedef char StaffGridLabelRow_gridIds_offset_check[
    ((u32)&((StaffGridLabelRow *)0)->gridIds == 0x18) ? 1 : -1];

extern const StaffGridLabelRow D_003B2100[];

void func_002723B0(s32 kind, s32 slot) {
    StaffGridLabelRow rows[7];
    s32 i;
    memcpy(rows, D_003B2100, sizeof(rows));
    for (i = 0; i < rows[kind].count; i++) {
        if (rows[kind].x[i] != 0) {
            itfDrawGridWithResolvedSlot(rows[kind].x[i], 0xCF0, 0, 1,
                                       slot, rows[kind].gridIds[i], 0x53);
        }
    }
}


typedef struct TextStyleNode TextStyleNode;
extern void frFontSetChildColors(TextStyleNode *, u32);
extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, s32, s32);

void func_00272518(s32 kind, s32 labelIndex, s32 textTable, s32 context,
                   s32 drawOption, s32 textOption, s32 layer) {
    StaffMenuWork *menu = (StaffMenuWork *)context;
    s32 glyph;
    if (kind == 0) {
        itfDrawGridWithResolvedSlot(0x1C0, 0xA20, 0, drawOption, menu->staffSlots.baseResources[5], 0x1E, layer);
    } else {
        itfDrawGridWithResolvedSlot(0x1C0, 0xA20, 0, drawOption, menu->staffSlots.baseResources[5], 0x2E, layer);
    }
    itfDrawGridWithResolvedSlot(0x150, 0x9C0, 0, drawOption, menu->staffSlots.baseResources[5], 0, layer);
    itfDrawGridWithResolvedSlot(0x280, 0x9A0, 0, drawOption, menu->staffSlots.baseResources[1], 2, layer);
    if (textTable != 0) {
        glyph = itfDrawBankTextWithLayoutFlags(0x2C0, 0xA70, 0, labelIndex, textTable, textOption);
        frFontSetChildColors((TextStyleNode *)glyph, 0xA09DC366);
        func_001958A0((FrFontGlyph *)glyph, 0, layer);
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
    }
}

void func_00272668(s32 kind, s32 labelIndex, s32 textTable, s32 context, s32 drawOption, s32 layer) {
    func_00272518(kind, labelIndex, textTable, context, drawOption, 0, layer);
}

void mnuDrawStaffCampScreen(s32 kind, s32 task) {
    StaffMenuWork *menu = (StaffMenuWork *)kwlnTaskGetUserValue(task);

    mnuDrawBackdrop((MenuAssets *)menu->background, 0x20);
    if (func_002719F0(task) == 0) {
        return;
    }
    func_0027E8D8(-0x10, -8, 0, (s32)menu->scrollPanel, 0x53);
    mnuDrawPanelListDefault(0, 0, 0, (MenuPageWindow *)((u8 *)menu + 0x15C), 0x53);
    if (kind == 0) {
        itfDrawGridWithResolvedSlot(0x1AB0, 0x70, 0, 1, menu->staffSlots.baseResources[1], 6, 0x53);
        itfDrawGridWithResolvedSlot(0x17A0, 0x78, 0, 1, menu->staffSlots.baseResources[0], 0xF, 0x53);
        itfDrawGridWithResolvedSlot(0x1E40, 0x78, 0, 1, menu->staffSlots.baseResources[0], 0x10, 0x53);
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

