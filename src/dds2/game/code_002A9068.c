#include "common.h"

extern u32 mnuMovieShutdownCounter;

extern s32 kwlnFadeIsActive(void);

extern u32 kwlnTaskGetUserValue();

extern char mnuCampInputTaskName[]; /* "camp" */

extern char mnuCampDrawTaskName[]; /* "camp_draw" */

extern char mnuCampOwnerTaskName[]; /* "camp_update" */

extern s8 mnuCampTaskState;

extern s32 datGameState;

/* Item quantities are byte-indexed in the shared save-state block. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

extern s8 D_00437B73;

extern void sdfReleaseResourceAllocation(u32);

extern void *memset(void *, s32, u32);

extern s32 func_002AAF70(s32);

extern void func_002AB0E0();

extern s32 mnuFinishStaffConfigPopup(s32);

extern void func_00101968(s32, s32);

extern void kwlnFadeOutStart(s8, s8, s8, s32);

extern void mnuClearPanelTransitionState(u8 *);

extern s32 dds3AdminReadPreviousSignedSample(void);

extern s32 mnuAllocateValueRecord(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void mnuLoadEffectResources(u8 *);

extern void mnuEnableCampBadgeFade(u8 *);

extern void evtCreateMessageWindowIfMissing(s32);

extern u8 D_003E5778[];

extern void func_002A9908(u8 *);

extern void mnuResetGradientFadeColor(u8 *, s32);

extern void func_003425B0(void);

/* One of five 0x1C4-byte party records at datGameState + 0xA60. */
typedef struct PartyRecord {
    u16 flags;
    u16 pad02;
    u16 slotIndex; /* +0x04: selects an entry in the category-model list */
    u8 pad06[0x1AC];
    u16 unk1B2;
    u8 pad1B4[0x10];
} PartyRecord;
/* Global scene's spendable currency is clamped by datAddCurrencyClamped. */
typedef struct CampCurrency {
    u8 pad00[0x3C];
    s32 currency;
} CampCurrency;


extern void scrClearPackedScriptFlags(void *);

extern void mnuClearEntryBlocked(s32);

extern void mdlFlagClear(s32);

extern void scrClearProfileFlagsTable(void);

extern void func_00315A50(void);

extern void scrResetAndSetPairedGlobalFlags(void);

extern void ptyClearProfileRecords(void);

extern void ptyRebuildAllProfiles(void);

extern void mnuDrawCampIconBackdrop(u8 *, s32);

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);

extern void func_002BB510(s32, s32, s32, s32, s32);

extern void mnuDrawPanelListDefault(s32, s32, s32, u8 *, s32);

extern void mnuDrawCampTitleCurrencyAndFade(s32, s32, s32, s32, u8 *, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_0035C860(char *, const char *, ...);

extern u32 uiBlendColors(u32, u32, s32);

extern s32 func_0019F798(s32, s32, s32, s32, char *, s32);

extern char D_00437B80[];

extern u32 mnuGetValueRecordOwner(u32 *);

extern u32 effAppendListEntry(u32 *, char *, u32, u32, u32 *);

extern u32 D_003E6858[][2];

extern u32 D_003E68D8[][2];

extern u32 D_003E6900[][2];

void ptyResetPartyRecordsAndProfiles(void) {
    s32 offset = 0;
    s32 i = 4;

    do {
        PartyRecord *rec = (PartyRecord *)(datGameState + offset + 0xA60);
        offset += 0x1C4;
        if (rec->flags & 1) {
            scrClearPackedScriptFlags(rec);
            rec->unk1B2 = 0;
        }
        i--;
    } while (i >= 0);
    for (i = 0xC0; i < 0x100; i++) {
        ((SaveItemCounts *)datGameState)->counts[i] = 0;
        mnuClearEntryBlocked(i);
    }
    mdlFlagClear(0x901);
    scrClearProfileFlagsTable();
    func_00315A50();
    scrResetAndSetPairedGlobalFlags();
    ptyClearProfileRecords();
    ptyRebuildAllProfiles();
}

extern u32 mnuCampResourceHandles[2];

extern u32 D_003E6848[];

extern char D_0042A950[];

extern u32 effLoadIndexedResource(char *, u32, u32);

#define MNU_STAFF_BASE_RESOURCE_COUNT 2
#define MNU_STAFF_MAIN_RESOURCE_COUNT 16
#define MNU_STAFF_EXTRA_RESOURCE_COUNT 5
#define MNU_STAFF_PAIR_RESOURCE_COUNT 2
#define MNU_STAFF_STATUS_BATCH_COUNT 2
#define MNU_STAFF_PANEL_COUNT 3
#define MNU_STAFF_PARTY_COUNT 5
#define MNU_STAFF_PARTY_RECORD_BYTES 0x1C4
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
void mnuLoadCampResources(void) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        mnuCampResourceHandles[resourceIndex] = effLoadIndexedResource(D_0042A950, D_003E6848[resourceIndex * 2], MNU_STAFF_RETAIN_RESOURCE);
    }
}

extern void effReleaseTextureHandlesAndResetSlots(u32);

extern void effResolveAndReleaseResource(u32);

/* Resolve each global handle, then copy its post-call value into destination. */
void mnuSnapshotCampTextureHandles(u32 *destination) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        effResolveAndReleaseResource(mnuCampResourceHandles[resourceIndex]);
        destination[resourceIndex] = mnuCampResourceHandles[resourceIndex];
    }
}

/* Release textures through the global handles and clear the caller's slots. */
void mnuReleaseCampTextureHandlesAndClearOutput(u32 *destination) {
    s32 resourceCountdown = MNU_STAFF_BASE_RESOURCE_COUNT - 1;
    u32 byteOffset = 0;
    do {
        effReleaseTextureHandlesAndResetSlots(*(u32 *)((u8 *)mnuCampResourceHandles + byteOffset));
        *(u32 *)((u8 *)destination + byteOffset) = 0;
        byteOffset += 4;
    } while (--resourceCountdown >= 0);
}

/* Return this category's handle array and count; unknown categories have none. */
INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A440);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A450);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A460);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A470);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A480);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A490);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A4A8);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A4C0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A4D8);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A4F0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A500);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A518);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A530);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A540);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A558);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A570);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A588);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A598);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A5B0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A5C8);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A5E0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A5F8);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A608);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A620);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A638);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A650);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A668);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A680);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A690);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A6B0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A6C0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A6D0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A6E0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A6F0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A700);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A710);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A720);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A730);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A740);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A750);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A760);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A770);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A780);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A790);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A7A0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A7B0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A7D0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A7E0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A7F0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A800);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A810);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A820);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A830);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A840);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A850);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A870);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A888);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A8A0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A8B0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A8C0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A8D0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A8E0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A8F0);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A900);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A910);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A920);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A930);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A940);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042A950);

u8 *mnuGetStaffCategoryEntries(s32 category, s32 *outEntryCount, u8 *menuBytes) {
    switch (category) {
    case 1:
        *outEntryCount = 1;
        return menuBytes + 0xC8;
    case 2:
        *outEntryCount = 1;
        return menuBytes + 0xC4;
    case 3:
        *outEntryCount = 2;
        return menuBytes + 0x68;
    case MNU_STAFF_PARTY_CATEGORY:
        *outEntryCount = 9;
        return menuBytes + 0xCC;
    case 5:
        *outEntryCount = 1;
        return menuBytes + 0xF0;
    default:
        *outEntryCount = 0;
        return 0;
    }
}

/* Resolve entry zero and the active party's adjusted one-based model indices.
 * The supplied count and work are unused; model indices are not range-clamped. */
void movReleaseActivePartyCategoryModels(s32 modelListAddress, s32 unusedCount, u8 *unusedWork) {
    s32 partyIndex;

    effResolveAndReleaseResource(*(u32 *)modelListAddress);
    for (partyIndex = 0; partyIndex < MNU_STAFF_PARTY_COUNT; partyIndex++) {
        PartyRecord *partyRecord = (PartyRecord *)(datGameState + MNU_STAFF_PARTY_RECORD_BASE + partyIndex * MNU_STAFF_PARTY_RECORD_BYTES);

        if ((partyRecord->flags & MNU_STAFF_PARTY_PRESENT_BIT) != 0) {
            s32 modelIndex = partyRecord->slotIndex + D_00437B73;

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

/* Release each category entry's textures without overwriting the handle array. */
void mnuReleaseStaffCategoryTextureHandles(s32 category, u8 *menuBytes) {
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

typedef struct { u8 pad0[0x20]; s32 *data; } MotSub;

typedef struct { u8 pad0[8]; MotSub *sub; } MotRes;

/* Camp work's drawing parameters; the intervening regions belong to the
 * resource lists and party-panel state initialized elsewhere in this unit. */
typedef struct CampVisualWork {
    u32 allocationHandle;  /* 0x0000 */
    u8 pad04[0x58];
    u32 menuResource;      /* 0x005C */
    s32 drawContext;        /* 0x0060 */
    s32 titleContext;       /* 0x0064 */
    u8 pad68[0x98];
    u32 motionResource;     /* 0x0100 */
    u8 pad104[0xC];
    MotRes *motion[2];      /* 0x0110 and 0x0114 */
    u32 modelHandle;        /* 0x0118 */
    u8 pad11C[0x16C];
    s32 backgroundOpacity;  /* 0x0288 */
    u8 pad28C[0xA7C0];
    s32 categoryKind;      /* 0xAA4C */
    u8 padAA50[0x780];
    s32 titleFadingOut;     /* 0xB1D0 */
    s32 titleOpacity;       /* 0xB1D4, range 0..0x100 */
    s32 titleSlide;         /* 0xB1D8, approaches zero from below */
    s32 highlightOpacity;   /* 0xB1DC, range 0..0x100 */
} CampVisualWork;

/* On a category change, release old textures and resolve the new model entries. */
void mnuSwitchCampVisualCategory(s32 nextCategory, u8 *menuBytes) {
    s32 previousCategory = ((CampVisualWork *)menuBytes)->categoryKind;
    if (nextCategory == previousCategory) {
        return;
    }
    if (previousCategory != 0) {
        mnuReleaseStaffCategoryTextureHandles(previousCategory, menuBytes);
    }
    if (nextCategory != 0) {
        movReleaseCategoryModels(nextCategory, menuBytes);
    }
    ((CampVisualWork *)menuBytes)->categoryKind = nextCategory;
}

extern u32 D_003E6970[];

/* Load the mapped motion resource and initialize both status batches' words.
 * Batch categories and the initial 0xF word remain opaque. */
void movLoadTitleEffects(u8 *menuBytes) {
    s32 *statusWords;

    ((CampVisualWork *)menuBytes)->motionResource = effLoadMappedResource("/camp/mot/", D_003E6970[0]);
    ((CampVisualWork *)menuBytes)->motion[0] = effCreateStatusBatch(6);
    statusWords = ((CampVisualWork *)menuBytes)->motion[0]->sub->data;
    statusWords[0] = 0xF;
    statusWords[1] = 0;
    statusWords[2] = 0;
    statusWords[3] = 0;
    statusWords[4] = 0;
    ((CampVisualWork *)menuBytes)->motion[1] = effCreateStatusBatch(1);
    statusWords = ((CampVisualWork *)menuBytes)->motion[1]->sub->data;
    statusWords[0] = 0xF;
    statusWords[1] = 0;
}

/* Destroy the mapped motion resource followed by both status batches. */
void movReleaseTitleEffects(u32 *resourceSlots) {
    u32 *batchCursor = resourceSlots + 0x110 / 4;
    u32 batchIndex;
    effDestroyPackedBatch(resourceSlots[0x100 / 4]);
    for (batchIndex = 0; batchIndex < MNU_STAFF_STATUS_BATCH_COUNT; batchIndex++) {
        effDestroyPackedBatch(*batchCursor++);
    }
}

void mnuInitializeCampPanelResources(u32 container, u32 *resources, u32 unused, u32 mode) {
    func_002BCD90(container, mode, *resources, 1, resources[1], 0x2d, resources[1], 0x1d);
    func_002BC498(container, resources[1]);
    mnuCopyPrimaryWindowHandles(container, resources + 4);
    mnuCopySecondaryWindowHandles(container, resources + 0xc);
    mnuRegisterResourceHandles(container, resources + 0x14);
    func_002BCA98(container);
    mnuSetPanelSlotValues(container, resources[1]);
}

/* Snapshot base handles, then queue the main, extra and paired sprite groups.
 * Only owner word 1 selects the second column of each image table. */
void mnuAppendCampSpriteRequests(u32 *resourceList, u32 *resourceSlots) {
    s32 resourceIndex;
    s32 tableColumn;

    mnuSnapshotCampTextureHandles(resourceSlots);
    tableColumn = mnuGetValueRecordOwner(resourceList) == 1;
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, D_0042A950, D_003E6858[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, resourceSlots + 0x10 / 4 + resourceIndex);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, D_0042A950, D_003E68D8[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, resourceSlots + 0x50 / 4 + resourceIndex);
    }
    for (resourceIndex = 0; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        effAppendListEntry(resourceList, "/camp/spr/n_sta/", D_003E6900[resourceIndex][tableColumn], MNU_STAFF_RETAIN_RESOURCE, resourceSlots + 0x8 / 4 + resourceIndex);
    }
}

/* Clear base output slots, then destroy the main, extra and paired slot sets.
 * Each countdown decreases while its handle cursor advances forward. */
void mnuReleaseTitleEffectSprites(u32 *resourceSlots) {
    s32 resourceCountdown;
    u32 *mainCursor;
    u32 *extraCursor;
    u32 *pairCursor;

    mnuReleaseCampTextureHandlesAndClearOutput(resourceSlots);
    mainCursor = resourceSlots + 0x10 / 4;
    for (resourceCountdown = MNU_STAFF_MAIN_RESOURCE_COUNT - 1; resourceCountdown >= 0; resourceCountdown--) {
        effDestroyResourceSlotSet(*mainCursor++);
    }
    extraCursor = resourceSlots + 0x50 / 4;
    for (resourceCountdown = MNU_STAFF_EXTRA_RESOURCE_COUNT - 1; resourceCountdown >= 0; resourceCountdown--) {
        effDestroyResourceSlotSet(*extraCursor++);
    }
    pairCursor = resourceSlots + 0x8 / 4;
    for (resourceCountdown = MNU_STAFF_PAIR_RESOURCE_COUNT - 1; resourceCountdown >= 0; resourceCountdown--) {
        effDestroyResourceSlotSet(*pairCursor++);
    }
}

/* Poll the supplied resource list, then require nonzero handles in every group. */
s32 movAreTitleEffectsReady(s32 resourceListAddress, u32 *resourceSlots) {
    u32 *resourceSlot;
    u32 *pairSlot;
    s32 resourceIndex;
    effPollResourceList(resourceListAddress);
    resourceIndex = 0;
    resourceSlot = resourceSlots;
    for (; resourceIndex < MNU_STAFF_BASE_RESOURCE_COUNT; resourceIndex++) {
        if (*resourceSlot++ == 0) {
            return 0;
        }
    }
    resourceIndex = 0;
    resourceSlot = resourceSlots + 4;
    for (; resourceIndex < MNU_STAFF_MAIN_RESOURCE_COUNT; resourceIndex++) {
        if (*resourceSlot++ == 0) {
            return 0;
        }
    }
    resourceIndex = 0;
    resourceSlot = resourceSlots + 0x50 / 4;
    for (; resourceIndex < MNU_STAFF_EXTRA_RESOURCE_COUNT; resourceIndex++) {
        if (*resourceSlot++ == 0) {
            return 0;
        }
    }
    pairSlot = resourceSlots + 2;
    resourceIndex = 0;
    for (; resourceIndex < MNU_STAFF_PAIR_RESOURCE_COUNT; resourceIndex++) {
        if (*pairSlot++ == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002A9068", func_002A9908);

/* Title-effect work: sprite handle list at 0x60, then the resource slot sets. */
typedef struct TitleEffectHandles {
    u8 pad00[0x60];
    u32 sprites[25]; /* 0x60: released through mnuReleaseTitleEffectSprites */
    u32 groupA;      /* 0xC4 */
    u32 groupB;      /* 0xC8 */
    u32 additional[9]; /* 0xCC */
    u32 finalGroup;  /* 0xF0 */
} TitleEffectHandles;

INCLUDE_ASM(const s32, "game/code_002A9068", mnuReleaseTitleEffectResourceGroups);

INCLUDE_ASM(const s32, "game/code_002A9068", func_002A9AB8);

typedef struct StaffResourceHeader {
    u8 pad00[0x64];
    s32 resourceSource;         /* 0x064 */
    u8 pad68[0x8C];
    u32 baseHandles[3];        /* 0x0F4 */
    s32 resourceOptions;        /* 0x100 */
    u32 resourceLists[3];      /* 0x104 */
} StaffResourceHeader;

void mnuDrawCampGridResourceSlot(s32 drawWork, u32 y, u32 z, s32 record, u32 unused,
                   u32 layer) {
    itfDrawGridWithResolvedSlot(drawWork + 0x60, y, z, 1, *(u32 *)(*(s32 *)(record + 0x30) + 100), 10,
                  layer);
}

INCLUDE_ASM(const s32, "game/code_002A9068", func_002A9BF8);

extern s32 func_002B9FF8(s32, s32, s32);

extern s32 func_002A9BF8(void *, s32, s32, s32, u8 *, void *);

extern void mnuSetWindowContainerState(s32, s32);

extern void mnuInitializeWindowFadeState(u8 *);

extern void func_002BAF50(s32, u8 *);

extern u8 D_003E56D0[], D_003E56F0[], D_003E5708[], D_003E6978[], D_003E6998[];

void mnuStaffInitResourceLists(u8 *work) {
    u8 *ctx = work + 0xB10C;
    s32 list;

    ((StaffResourceHeader *)work)->baseHandles[0] = func_002B9FF8(0, ((StaffResourceHeader *)work)->resourceSource, ((StaffResourceHeader *)work)->resourceOptions);
    ((StaffResourceHeader *)work)->baseHandles[1] = func_002B9FF8(1, ((StaffResourceHeader *)work)->resourceSource, ((StaffResourceHeader *)work)->resourceOptions);
    ((StaffResourceHeader *)work)->baseHandles[2] = func_002B9FF8(3, ((StaffResourceHeader *)work)->resourceSource, ((StaffResourceHeader *)work)->resourceOptions);
    ((StaffResourceHeader *)work)->resourceLists[0] = func_002A9BF8(D_003E56D0, 8, 0x1C0, 0x10, work, D_003E6978);
    list = func_002A9BF8(D_003E56F0, 5, 0x1C0, 0x10, work, D_003E6998);
    ((StaffResourceHeader *)work)->resourceLists[1] = list;
    mnuSetWindowContainerState(list, 0x100);
    list = func_002A9BF8(D_003E5708, 2, 0x1C0, 0x10, work, 0);
    ((StaffResourceHeader *)work)->resourceLists[2] = list;
    mnuSetWindowContainerState(list, 0x100);
    mnuInitializeWindowFadeState(ctx);
    func_002BAF50(((StaffResourceHeader *)work)->resourceLists[0], ctx);
}

extern void mnuDestroyWindowContainer(u32);

extern void mnuReleaseResourceList(u32);

/* Destroy the menu windows, then release their associated resource lists. */
void mnuReleaseStaffSpriteAndResourceHandles(u8 *menuBytes) {
    u32 *windowCursor = ((StaffResourceHeader *)menuBytes)->resourceLists;
    u32 windowIndex;

    for (windowIndex = 0; windowIndex < MNU_STAFF_PANEL_COUNT; windowIndex++) {
        mnuDestroyWindowContainer(*windowCursor++);
    }
    mnuReleaseResourceList(((StaffResourceHeader *)menuBytes)->baseHandles[0]);
    mnuReleaseResourceList(((StaffResourceHeader *)menuBytes)->baseHandles[1]);
    mnuReleaseResourceList(((StaffResourceHeader *)menuBytes)->baseHandles[2]);
}

extern u16 D_003E6500[];

extern u16 D_003E6730[];

extern u16 D_003E6788[];

/* Lookup tables inside the list menu work: id minus the table base gives the list slot. */
typedef struct ListSlotWork {
    u8 pad00[0xAA60];
    u16 slotOfA[0x2A0]; /* 0xAA60: ids 0x2A1..0x540 */
    u16 slotOfB[0x40];  /* 0xAFA0: ids 0xC0.. (slot + 1) */
    u16 slotOfC[0x75];  /* 0xB020: skill ids 0x1AB..0x21F */
} ListSlotWork;

void mnuBuildListSlotTableA(ListSlotWork *work) {
    u16 *slot;
    s32 id;
    s32 i;

    for (i = 0x29F, slot = &work->slotOfA[0x29F], id = 0x540; i >= 0; i--, slot--, id--) {
        *slot = id;
    }
    for (i = 0; i < 0x118; i++) {
        work->slotOfA[D_003E6500[i]] = i + 1;
    }
}

void mnuBuildListSlotTableB(ListSlotWork *work) {
    u16 *slot;
    s32 i;
    s32 idx;

    for (i = 0x3F, slot = &work->slotOfB[0x3F]; i >= 0; i--, slot--) {
        *slot = 0;
    }
    for (i = 0; i < 0x2B; i++) {
        idx = D_003E6730[i] - 0xC0;
        work->slotOfB[idx] = i + 1;
    }
}

void mnuBuildSkillSlotTable(ListSlotWork *work) {
    u16 *slot;
    s32 id;
    s32 i;
    s32 idx;

    for (i = 0x74, slot = &work->slotOfC[0x74], id = 0x21F; i >= 0; i--, slot--, id--) {
        *slot = id;
    }
    for (i = 0; i < 0x60; i++) {
        idx = D_003E6788[i] - 0x1AB;
        work->slotOfC[idx] = i;
    }
}

#define MNU_STAFF_WORK_BYTES 0xB1E0

/* Allocate and clear menu work, select its image-table owner word from the
 * previous sample, then initialize the owned UI and resource state. */
u8 *mnuCreateStaffMenuWork(void) {
    s32 allocation;
    u8 *menuBytes;
    u8 *effectBytes;

    allocation = sdfAllocGeneralBlock(MNU_STAFF_WORK_BYTES);
    menuBytes = (u8 *)sdfResourceRetainAddress(allocation);
    memset(menuBytes, 0, MNU_STAFF_WORK_BYTES);
    ((CampVisualWork *)menuBytes)->allocationHandle = allocation;
    effectBytes = menuBytes + 0x11C;
    mnuClearPanelTransitionState(menuBytes + 8);
    if (dds3AdminReadPreviousSignedSample() != 0) {
        ((CampVisualWork *)menuBytes)->menuResource = mnuAllocateValueRecord(1);
    } else {
        ((CampVisualWork *)menuBytes)->menuResource = mnuAllocateValueRecord(0);
    }
    mnuInitPartyPanelSlots((s32)menuBytes + 0xA928);
    mnuLoadEffectResources(effectBytes);
    mnuEnableCampBadgeFade(effectBytes);
    evtCreateMessageWindowIfMissing((s32)D_003E5778);
    movLoadTitleEffects(menuBytes);
    func_002A9908(menuBytes);
    mnuResetGradientFadeColor(menuBytes + 0xAA50, 0x60);
    mnuBuildListSlotTableA((ListSlotWork *)menuBytes);
    mnuBuildListSlotTableB((ListSlotWork *)menuBytes);
    mnuBuildSkillSlotTable((ListSlotWork *)menuBytes);
    func_003425B0();
    return menuBytes;
}

/* Ignore null task userdata; otherwise drain transitions and release owned
 * resources in shutdown order before marking camp cleanup complete. */
void mnuDestroyStaffMenuTask(u32 task) {
    u8 *menuBytes = (u8 *)kwlnTaskGetUserValue(task);
    if (menuBytes == NULL) {
        return;
    }
    mnuDrainPanelTransitions(menuBytes + 8, task);
    mnuReleaseStaffSpriteAndResourceHandles(menuBytes);
    mnuDestroyScrollPanel(((CampVisualWork *)menuBytes)->modelHandle);
    mnuShutdownContext(menuBytes + 0x284);
    dspCloseChannel();
    mnuDestroyEffectResources(menuBytes + 0x11c);
    mnuReleaseTitleEffectResourceGroups(menuBytes);
    movReleaseTitleEffects(menuBytes);
    func_00303D58(((CampVisualWork *)menuBytes)->menuResource);
    sdfReleaseResourceAllocation(((CampVisualWork *)menuBytes)->allocationHandle);
    mnuCampTaskState = MNU_CAMP_STATE_CLEANED_UP;
    func_003425D8();
}

u32 func_002AA278(void) {
    s32 work;

    work = kwlnTaskGetUserValue();
    mnuDrawAndStepGradientFade(work + 0xaa50, 0x53);
    return 0;
}

extern u32 mnuMapPadMaskToFlags(s32);

extern s32 func_002A9AB8(s32);

extern s32 evtGetMessageWindowControlState(void);

extern s32 func_002C6CE8(void);

extern s32 fileConsumeConfigTaskReady(void);

extern void mnuDestroyCampTasks(void);

extern void mnuPlayInputSound();

/* Mapped cancel input closes camp only through the full guard chain.
 * Return -1 on closure, otherwise 0; blocked nested guards use the alternate sound. */
s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 mappedButtons = mnuMapPadMaskToFlags(MNU_CAMP_CANCEL_MASK);
    s32 result;

    if (func_002A9AB8(menu) == 0) {
        return 0;
    }
    result = 0;
    if (evtGetMessageWindowControlState() == 0) {
        if (mappedButtons & MNU_CAMP_CANCEL_MASK) {
            if (func_002C6CE8() != 1) {
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

/* Share menu userdata across the input/draw/owner tasks, attach fade and cancel
 * tasks to drawing, then start the opening fade and mark camp active. */
INCLUDE_RODATA(const s32, "game/code_002A9068", mnuCampDrawTaskName);

INCLUDE_RODATA(const s32, "game/code_002A9068", mnuCampOwnerTaskName);

void mnuCreateCampTasks(void) {
    s32 menuAddress;
    s32 drawTask;

    menuAddress = (s32)mnuCreateStaffMenuWork();
    kwlnTaskCreate(mnuCampInputTaskName, 0x3F2, 1, 0, func_002AAF70, 0, menuAddress);
    drawTask = kwlnTaskCreate(mnuCampDrawTaskName, 0x2B07, 1, 0, func_002AB0E0, 0, menuAddress);
    kwlnTaskCreate(mnuCampOwnerTaskName, 0x520B, 1, 0, mnuFinishStaffConfigPopup, mnuDestroyStaffMenuTask, menuAddress);
    func_00101968(drawTask, kwlnTaskCreate("camp_fade", 0x2B08, 1, 0, func_002AA278, 0, menuAddress));
    func_00101968(drawTask, kwlnTaskCreate("camp_all_cancel", 0x3F3, 1, 0, mnuStaffCampCancelCheck, 0, menuAddress));
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

/* Draw the camp title using the task's draw context and layer, then ease its
 * horizontal slide toward zero and its opacity toward the fade target. */
void mnuDrawCampTitleCurrencyAndFade(s32 unused0, s32 unused1, s32 textParam, s32 drawContext, u8 *work, s32 layer) {
    char buffer[16];
    s32 object;
    s32 offset;
    s32 magnitude;
    CampVisualWork *visual = (CampVisualWork *)work;

    if (mdlFlagTest(0x290) == 0) {
        return;
    }
    func_00306CD0((visual->titleSlide + 0x1A) << 4, 0xCB8, 0, visual->titleOpacity, 1, drawContext, 0x3F, layer);
    func_0035C860(buffer, D_00437B80, ((CampCurrency *)datGameState)->currency);
    /* Keep the RGB channels fixed while the opacity byte fades from 0x80 to zero. */
    object = func_0019F798((visual->titleSlide + 0x33) << 4, 0xCD8, textParam,
                           uiBlendColors(0xA09DC380, 0xA09DC300, visual->titleOpacity), buffer, 0);
    func_0019D550(object, 1, layer);
    frFontQueueGlyphInSelectedSlot(object);
    offset = visual->titleSlide;
    magnitude = offset;
    if (offset < 0) {
        magnitude = -offset;
    }
    if (offset < 0) {
        visual->titleSlide = offset + magnitude / 5 + 1;
    }
    if (visual->titleSlide > 0) {
        visual->titleSlide = 0;
    }
    if (visual->titleFadingOut == 0) {
        if (visual->titleOpacity < 0x100) {
            visual->titleOpacity += 0x14;
        }
        if (visual->titleOpacity > 0x100) {
            visual->titleOpacity = 0x100;
        }
    } else {
        if (visual->titleOpacity > 0) {
            visual->titleOpacity -= 0x1E;
        }
        if (visual->titleOpacity < 0) {
            visual->titleOpacity = 0;
        }
    }
}

extern u32 D_003E5710[];

/* Create and queue the table-selected image sprite; imageIndex is unchecked. */
void mnuCreateStaffImageSprite(s32 imageIndex) {
    u32 *sprite = (u32 *)func_0019F460(0x340, 0x148, 0, 0xa09dc35a,
                                      D_003E5710[imageIndex], 0);
    func_0019D550(sprite, 1, 0x54);
    frFontQueueGlyphInSelectedSlot(sprite);
}

INCLUDE_ASM(const s32, "game/code_002A9068", func_002AA7A0);

INCLUDE_ASM(const s32, "game/code_002A9068", func_002AA9D8);

extern void func_002AA9D8(u32, u32, u32, u32, u32, u32, u32, u32, u32);

void func_002AAC70(u32 kind, u32 labelIndex, u32 textTable, u32 context, u32 drawOption, u32 textOption, u32 layer) {
    func_002AA9D8(kind, labelIndex, textTable, context, drawOption, textOption, 0, 0, layer);
}

void func_002AAC98(u32 kind, u32 labelIndex, u32 textTable, u32 context, u32 drawOption, u32 layer) {
    func_002AAC70(kind, labelIndex, textTable, context, drawOption, 0, layer);
}

typedef struct CampDrawPosition {
    u8 pad00[0xE6C];
    s32 fixedPointOffset; /* 0xE6C: source shifted left by four */
    u8 padE70[0x6C];
    s32 sourceOffset;     /* 0xEDC */
} CampDrawPosition;

typedef struct CampDrawContext {
    u8 pad00[0x18];
    CampDrawPosition *position;
} CampDrawContext;

/* Draw one camp-menu frame for task: kind 2 animates the highlight, kind 1
 * displays the background, and all other kinds reset the highlight alpha. */
void mnuDrawCampIconBackdropByKind(s32 kind, s32 task) {
    u8 *work = (u8 *)kwlnTaskGetUserValue(task);
    CampVisualWork *visual = (CampVisualWork *)work;
    s32 ctx;
    CampDrawPosition *sub;

    mnuDrawCampIconBackdrop(work + 0x11C, 0x20);
    switch (kind) {
    case 2:
        func_00306CD0(0, 0, 0, visual->highlightOpacity, 0, visual->drawContext, 0x19, 0x53);
        ctx = visual->drawContext;
        sub = ((CampDrawContext *)ctx)->position;
        sub->fixedPointOffset = (sub->sourceOffset + 0x40) << 4;
        func_00306CD0(0xD40, 0x2E0, 0, visual->highlightOpacity, 0, ctx, 0x17, 0x53);
        if (visual->highlightOpacity < 0x100) {
            visual->highlightOpacity += 0x10;
        }
        if (visual->highlightOpacity > 0x100) {
            visual->highlightOpacity = 0x100;
        }
        break;
    case 1:
        ctx = visual->drawContext;
        sub = ((CampDrawContext *)ctx)->position;
        sub->fixedPointOffset = sub->sourceOffset << 4;
        func_00306CD0(0x1140, 0x2E0, 0, visual->backgroundOpacity, 0, ctx, 0x17, 0x53);
    default:
        visual->highlightOpacity = 0;
        break;
    }
    if (func_002A9AB8(task) != 0) {
        func_002BB510(-0x10, -8, 0, visual->modelHandle, 0x53);
        mnuDrawPanelListDefault(0, 0, 0, work + 0x284, 0x53);
        mnuDrawCampTitleCurrencyAndFade(0, 0, 0, visual->titleContext, work, 0x53);
    }
}

void func_002AAE80(u32 task) {
    mnuDrawCampIconBackdropByKind(0, task);
}

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AA48);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AC40);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AC70);

INCLUDE_SDATA(const s32, "game/code_002A9068", mnuCampInputTaskName);

INCLUDE_SDATA(const s32, "game/code_002A9068", D_00437B80);

