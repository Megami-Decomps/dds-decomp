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

void mnuLoadStaffImageHandles(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        D_003DC5C8[i] = effLoadIndexedResource(D_003B2020, D_0037C210[i].index, 1);
    }
}

void mnuResolveStaffImageHandles(u32 *dst) {
    s32 i;

    for (i = 0; i < 7; i++) {
        u32 *src = &D_003DC5C8[i];
        u32 *out = &dst[i];

        effResolveAndReleaseResource(*src);
        *out = *src;
    }
}

void mnuReleaseStaffImageHandles(u32 *resources) {
    s32 index;
    for (index = 0; index < 7; index++) {
        u32 *slot = &resources[index];
        effReleaseTextureHandlesAndResetSlots(D_003DC5C8[index]);
        *slot = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2020);

void *mnuGetStaffCategoryEntries(s32 kind, s32 *count, u8 *data) {
    switch (kind) {
    case 1:
        *count = 4;
        return data + 0xe0;
    case 2:
        *count = 2;
        return data + 0xd8;
    case 3:
        *count = 2;
        return data + 0x7c;
    case 4:
        *count = 9;
        return data + 0xf0;
    case 5:
        *count = 1;
        return data + 0x114;
    default:
        *count = 0;
        return 0;
    }
}

extern u8 *datGameState;

extern s8 D_003BC6B5;

void movReleaseActivePartyCategoryModels(s32 list, s32 count, u8 *work) {
    s32 i;

    effResolveAndReleaseResource(*(u32 *)list);
    for (i = 0; i < 5; i++) {
        u8 *slot = datGameState + 0xA60 + i * 0x1A4;

        if ((*(u16 *)slot & 1) != 0) {
            s32 index = *(u16 *)(slot + 4) + D_003BC6B5;

            effResolveAndReleaseResource(*(u32 *)(list + index * 4 - 4));
        }
    }
}

void movReleaseCategoryModels(s32 kind, u8 *work) {
    s32 count;
    s32 *entries = (s32 *)mnuGetStaffCategoryEntries(kind, &count, work);
    if (kind != 4) {
        s32 i;
        for (i = 0; i < count; i++) {
            effResolveAndReleaseResource(entries[i]);
        }
    } else {
        movReleaseActivePartyCategoryModels(entries, count, work);
    }
}

extern void effReleaseTextureHandlesAndResetSlots(u32);

void mnuReleaseStaffCategoryTextureHandles(kind, work)
s32 kind;
u8 *work;
{
    s32 count;
    s32 i = 0;
    u8 *buffer = mnuGetStaffCategoryEntries(kind, &count, work);

    if (count > 0) {
        u32 *handles = (u32 *)buffer;
        do {
            effReleaseTextureHandlesAndResetSlots(*handles++);
        } while (++i < count);
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

void mnuSetStaffDisplayMode(s32 next, u8 *context) {
    s32 previous = ((StaffMenuWork *)context)->displayMode;
    if (next != previous) {
        if (previous != 0) {
            mnuReleaseStaffCategoryTextureHandles(previous);
        }
        if (next != 0) {
            movReleaseCategoryModels(next, context);
        }
        ((StaffMenuWork *)context)->displayMode = next;
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

void func_00271368(void *arg) {
    StaffMenuWork *work = (StaffMenuWork *)arg;
    StaffStatusBatch *batch;
    StaffStatusBatchPayload *batchPayload;
    u32 *payload;
    u32 baseResource;

    baseResource = effLoadMappedResource(D_003B2058, D_0037C380[0]);
    work->primaryImage = baseResource;
    work->secondaryImage = effLoadMappedResource(D_003B2058, D_0037C380[1]);

    batch = (StaffStatusBatch *)effCreateStatusBatch(6);
    batchPayload = (StaffStatusBatchPayload *)batch->payload;
    work->extraImages[0] = (u32)batch;
    payload = batchPayload->values;
    payload[0] = 0xF;
    payload[1] = 0;
    payload[2] = 0;
    payload[3] = 0;
    payload[4] = 0;

    batch = (StaffStatusBatch *)effCreateStatusBatch(1);
    batchPayload = (StaffStatusBatchPayload *)batch->payload;
    work->extraImages[1] = (u32)batch;
    payload = batchPayload->values;
    payload[0] = 0xF;
    payload[1] = 0;
}

void mnuReleaseStaffSpriteHandles(StaffMenuWork *handles) {
    u32 *image = handles->extraImages;
    u32 index = 0;
    effDestroyPackedBatch(handles->primaryImage);
    effDestroyPackedBatch(handles->secondaryImage);
    do {
        effDestroyPackedBatch(*image++);
        index++;
    } while (index < 2);
}

void mnuInitializeStaffPageWindows(u32 container, u32 *resources, u32 unused, u32 mode) {
    mnuInitPageWindow(container, mode, resources[3], 7, resources[4], 0, *resources, 0x11);
    func_0027FAA8(container, *resources);
    mnuCopyPrimaryWindowHandles(container, resources + 9);
    mnuCopySecondaryWindowHandles(container, resources + 0x11);
    mnuRegisterResourceHandles(container, resources + 0x19);
    mnuUpdateHandleStates(container);
}

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2058);

void mnuAppendCampSpriteRequests(u32 *list, u32 *state) {
    s32 i;
    s32 flag;

    mnuResolveStaffImageHandles(state);
    flag = mnuGetValueRecordOwner(list) == 1;
    for (i = 0; i < 16; i++) {
        effAppendListEntry(list, D_003B2020, D_0037C248[i][flag], 1, state + 0x24 / 4 + i);
    }
    for (i = 0; i < 5; i++) {
        effAppendListEntry(list, D_003B2020, D_0037C2C8[i][flag], 1, state + 0x64 / 4 + i);
    }
    for (i = 0; i < 2; i++) {
        effAppendListEntry(list, "/camp/spr/n_sta/", D_0037C2F0[i][flag], 1, state + 0x1C / 4 + i);
    }
}

void mnuReleaseStaffResourceGroups(u32 *resources) {
    u32 *inner = resources + 1;
    s32 i;

    mnuReleaseStaffImageHandles(resources);
    for (i = 0; i < 16; i++) {
        effDestroyResourceSlotSet(resources[9 + i]);
    }
    for (i = 0; i < 5; i++) {
        effDestroyResourceSlotSet(inner[24 + i]);
    }
    for (i = 0; i < 2; i++) {
        effDestroyResourceSlotSet(resources[7 + i]);
    }
}

typedef struct StaffSlots {
    u32 baseResources[7];    /* 0x00 */
    u32 pairResources[2];    /* 0x1C */
    u32 mainResources[16];   /* 0x24 */
    u32 extraResources[5];   /* 0x64 */
} StaffSlots;

s32 mnuStaffSlotsAllFilled(s32 unused, StaffSlots *slots) {
    s32 i;

    effPollResourceList();
    for (i = 0; i < 7; i++) {
        if (slots->baseResources[i] == 0) {
            return 0;
        }
    }
    for (i = 0; i < 16; i++) {
        if (slots->mainResources[i] == 0) {
            return 0;
        }
    }
    for (i = 0; i < 5; i++) {
        if (slots->extraResources[i] == 0) {
            return 0;
        }
    }
    for (i = 0; i < 2; i++) {
        if (slots->pairResources[i] == 0) {
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

void mnuReleaseStaffSpriteAndResourceHandles(StaffMenuWork *handles) {
    u32 *image = handles->images;
    u32 index = 0;
    do {
        mnuDestroyWindowContainer(*image++);
    } while (++index < 3);
    mnuReleaseResourceList(handles->resourceList);
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

StaffMenuWork *mnuCreateStaffCampWork(void) {
    u32 resource = sdfAllocGeneralBlock(sizeof(StaffMenuWork));
    StaffMenuWork *work = (StaffMenuWork *)sdfResourceRetainAddress(resource);

    memset(work, 0, sizeof(*work));
    work->resource = resource;
    mnuClearPanelTransitionState((s32)work->panel);
    /* The opaque owner word selects the normal or alternate staff image table. */
    if (dds3AdminReadPreviousSignedSample() != 0) {
        work->valueRecord = mnuAllocateValueRecord((void *)1);
    } else {
        work->valueRecord = mnuAllocateValueRecord(NULL);
    }
    mnuInitPartyPanelSlots(work->partyPanel);
    func_0027AD80(work->background);
    evtCreateMessageWindowIfMissing((s32)D_0037B9E0);
    func_00271368(work);
    func_002717D8(work);
    mnuResetGradientFadeColor(work->timer, 0x40);
    func_002E9708();
    return work;
}

extern s32 kwlnTaskGetUserValue();

extern s8 mnuCampTaskState;

extern void sdfReleaseResourceAllocation(u32);

void mnuDestroyStaffMenuTask(u32 task) {
    StaffMenuWork *work = (StaffMenuWork *)kwlnTaskGetUserValue(task);
    if (work == NULL) {
        return;
    }
    mnuDrainPanelTransitions(work->panel, task);
    mnuReleaseStaffSpriteAndResourceHandles(work);
    mnuDestroyScrollPanel(work->scrollPanel);
    mnuShutdownContext(work->background + 0x20);
    dspCloseChannel();
    mnuReleaseAssets(work->background);
    mnuReleaseStaffResourceSlotGroups((u32 *)work);
    mnuReleaseStaffSpriteHandles(work);
    func_002BC618((u32)work->valueRecord);
    sdfReleaseResourceAllocation(work->resource);
    mnuCampTaskState = 2;
    func_002E9730();
}

u32 func_00271FC8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuDrawAndStepGradientFade(context + 0x914, 0x53);
    return 0;
}

s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 buttons = mnuMapPadMaskToFlags(8);
    s32 result;

    if (func_002719F0(menu) == 0) {
        return 0;
    }
    result = 0;
    if (evtGetMessageWindowControlState() == 0) {
        if (buttons & 8) {
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

INCLUDE_RODATA(const s32, "game/code_00270FB0", mnuCampDrawTaskName);

INCLUDE_RODATA(const s32, "game/code_00270FB0", mnuCampOwnerTaskName);

void mnuCreateCampTasks(void) {
    s32 work;
    s32 draw;

    work = (s32)mnuCreateStaffCampWork();
    kwlnTaskCreate(mnuCampInputTaskName, 0x3F2, 1, 0, func_00272798, 0, work);
    draw = kwlnTaskCreate(mnuCampDrawTaskName, 0x2B07, 1, 0, func_002728F8, 0, work);
    kwlnTaskCreate(mnuCampOwnerTaskName, 0x520B, 1, 0, func_002729C8, mnuDestroyStaffMenuTask, work);
    func_00101A80(draw, kwlnTaskCreate("camp_fade", 0x2B08, 1, 0, func_00271FC8, 0, work));
    func_00101A80(draw, kwlnTaskCreate("camp_all_cancel", 0x3F3, 1, 0, mnuStaffCampCancelCheck, 0, work));
    kwlnFadeOutStart(0, 0, 0, 0xF);
    mnuCampTaskState = 1;
}

void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(mnuCampInputTaskName, 0);
    kwlnTaskDestroyWithHierarchyByName(mnuCampDrawTaskName, 0);
    kwlnTaskDestroyWithHierarchyByName(mnuCampOwnerTaskName, 0);
}

s32 mnuAcknowledgeCampState(void) {
    s8 state = mnuCampTaskState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        mnuCampTaskState = 0;
    }
    return 0;
}

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

void mnuCreateStaffImageSprite(s32 index) {
    u32 *object = (u32 *)func_00197760(0x2F0, 0x1E0, 0, 0xa09dc35a,
                                      D_0037B988[index], 0);
    func_001958A0(object, 1, 0x54);
    frFontQueueGlyphInSelectedSlot(object);
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

