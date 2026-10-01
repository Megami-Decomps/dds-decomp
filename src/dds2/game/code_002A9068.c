#include "common.h"

extern u32 D_00437AE8;

extern s32 kwlnFadeIsActive(void);

extern s32 kwlnTaskGetUserValue();

extern char D_00437B78[]; /* "camp" */

extern char D_0042AA08[]; /* "camp_draw" */

extern char D_0042AA18[]; /* "camp_update" */

extern s8 D_00437B72;

extern s32 D_00435DD0;

/* Item quantities are byte-indexed in the shared save-state block. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

extern s8 D_00437B73;

extern void func_003297C8(u32);

extern void *memset(void *, s32, u32);

extern void func_002AAF70();

extern void func_002AB0E0();

extern void func_002AB1B0();

extern void func_00101968(s32, s32);

extern void kwlnFadeOutStart(s8, s8, s8, s32);

extern void mnuClearPanelTransitionState(u8 *);

extern s32 dds3AdminReadPreviousSignedSample(void);

extern s32 mnuAllocateValueRecord(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void mnuLoadEffectResources(u8 *);

extern void func_002B8140(u8 *);

extern void evtCreateMessageWindowIfMissing(s32);

extern u8 D_003E5778[];

extern void func_002A9908(u8 *);

extern void func_002C1B58(u8 *, s32);

extern void func_003425B0(void);

/* One of five 0x1C4-byte party records at D_00435DD0 + 0xA60. */
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

extern void func_002AA530(s32, s32, s32, s32, u8 *, s32);

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
        PartyRecord *rec = (PartyRecord *)(D_00435DD0 + offset + 0xA60);
        offset += 0x1C4;
        if (rec->flags & 1) {
            scrClearPackedScriptFlags(rec);
            rec->unk1B2 = 0;
        }
        i--;
    } while (i >= 0);
    for (i = 0xC0; i < 0x100; i++) {
        ((SaveItemCounts *)D_00435DD0)->counts[i] = 0;
        mnuClearEntryBlocked(i);
    }
    mdlFlagClear(0x901);
    scrClearProfileFlagsTable();
    func_00315A50();
    scrResetAndSetPairedGlobalFlags();
    ptyClearProfileRecords();
    ptyRebuildAllProfiles();
}

extern u32 D_00438FF8[2];

extern u32 D_003E6848[];

extern char D_0042A950[];

extern u32 effLoadIndexedResource(char *, u32, u32);

void mnuLoadCampResources(void) {
    s32 i;
    for (i = 0; i < 2; i++) {
        D_00438FF8[i] = effLoadIndexedResource(D_0042A950, D_003E6848[i * 2], 1);
    }
}

extern void effReleaseTextureHandlesAndResetSlots(u32);

extern void effResolveAndReleaseResource(u32);

void func_002A91A0(u32 *destination) {
    s32 i;
    for (i = 0; i < 2; i++) {
        effResolveAndReleaseResource(D_00438FF8[i]);
        destination[i] = D_00438FF8[i];
    }
}

void func_002A9200(u32 *destination) {
    s32 remaining = 1;
    u32 offset = 0;
    do {
        effReleaseTextureHandlesAndResetSlots(*(u32 *)((u8 *)D_00438FF8 + offset));
        *(u32 *)((u8 *)destination + offset) = 0;
        offset += 4;
    } while (--remaining >= 0);
}

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

u8 *mnuGetStaffCategoryEntries(s32 kind, s32 *count, u8 *work) {
    switch (kind) {
    case 1:
        *count = 1;
        return work + 0xC8;
    case 2:
        *count = 1;
        return work + 0xC4;
    case 3:
        *count = 2;
        return work + 0x68;
    case 4:
        *count = 9;
        return work + 0xCC;
    case 5:
        *count = 1;
        return work + 0xF0;
    default:
        *count = 0;
        return 0;
    }
}

/* Release the first category model plus one model per occupied party slot;
 * slotIndex is offset by the active party selection before indexing list. */
void movReleaseActivePartyCategoryModels(s32 list, s32 count, u8 *work) {
    s32 i;

    effResolveAndReleaseResource(*(u32 *)list);
    for (i = 0; i < 5; i++) {
        PartyRecord *slot = (PartyRecord *)(D_00435DD0 + 0xA60 + i * 0x1C4);

        if ((slot->flags & 1) != 0) {
            s32 index = slot->slotIndex + D_00437B73;

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

void mnuReleaseStaffCategoryTextureHandles(s32 kind, u8 *work) {
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

/* Switch the active category, releasing its old handles and resolving the
 * next category's models; a repeated category needs no work. */
void func_002A9460(s32 kind, u8 *work) {
    s32 old = ((CampVisualWork *)work)->categoryKind;
    if (kind == old) {
        return;
    }
    if (old != 0) {
        mnuReleaseStaffCategoryTextureHandles(old, work);
    }
    if (kind != 0) {
        movReleaseCategoryModels(kind, work);
    }
    ((CampVisualWork *)work)->categoryKind = kind;
}

extern u32 D_003E6970[];

void movLoadTitleEffects(u8 *work) {
    s32 *data;

    ((CampVisualWork *)work)->motionResource = effLoadMappedResource("/camp/mot/", D_003E6970[0]);
    ((CampVisualWork *)work)->motion[0] = effCreateStatusBatch(6);
    data = ((CampVisualWork *)work)->motion[0]->sub->data;
    data[0] = 0xF;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    ((CampVisualWork *)work)->motion[1] = effCreateStatusBatch(1);
    data = ((CampVisualWork *)work)->motion[1]->sub->data;
    data[0] = 0xF;
    data[1] = 0;
}

void movReleaseTitleEffects(u32 *state) {
    u32 *handles = state + 0x110 / 4;
    u32 i;
    effDestroyPackedBatch(state[0x100 / 4]);
    for (i = 0; i < 2; i++) {
        effDestroyPackedBatch(*handles++);
    }
}

void func_002A95B0(u32 container, u32 *resources, u32 unused, u32 mode) {
    func_002BCD90(container, mode, *resources, 1, resources[1], 0x2d, resources[1], 0x1d);
    func_002BC498(container, resources[1]);
    func_002BC5D0(container, resources + 4);
    func_002BC600(container, resources + 0xc);
    mnuRegisterResourceHandles(container, resources + 0x14);
    func_002BCA98(container);
    mnuSetPanelSlotValues(container, resources[1]);
}

void mnuAppendCampSpriteRequests(u32 *list, u32 *state) {
    s32 i;
    s32 flag;

    func_002A91A0(state);
    flag = mnuGetValueRecordOwner(list) == 1;
    for (i = 0; i < 16; i++) {
        effAppendListEntry(list, D_0042A950, D_003E6858[i][flag], 1, state + 0x10 / 4 + i);
    }
    for (i = 0; i < 5; i++) {
        effAppendListEntry(list, D_0042A950, D_003E68D8[i][flag], 1, state + 0x50 / 4 + i);
    }
    for (i = 0; i < 2; i++) {
        effAppendListEntry(list, "/camp/spr/n_sta/", D_003E6900[i][flag], 1, state + 0x8 / 4 + i);
    }
}

/* Releases the sprite handle groups held by the movie/title effect work. */
void mnuReleaseTitleEffectSprites(u32 *work) {
    s32 i;
    u32 *group0;
    u32 *group1;
    u32 *group2;

    func_002A9200(work);
    group0 = work + 0x10 / 4;
    for (i = 0xF; i >= 0; i--) {
        effDestroyResourceSlotSet(*group0++);
    }
    group1 = work + 0x50 / 4;
    for (i = 4; i >= 0; i--) {
        effDestroyResourceSlotSet(*group1++);
    }
    group2 = work + 0x8 / 4;
    for (i = 1; i >= 0; i--) {
        effDestroyResourceSlotSet(*group2++);
    }
}

s32 movAreTitleEffectsReady(s32 mode, u32 *state) {
    u32 *entry;
    u32 *tail;
    s32 i;
    effPollResourceList(mode);
    i = 0;
    entry = state;
    for (; i < 2; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    i = 0;
    entry = state + 4;
    for (; i < 16; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    i = 0;
    entry = state + 0x50 / 4;
    for (; i < 5; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    tail = state + 2;
    i = 0;
    for (; i < 2; i++) {
        if (*tail++ == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002A9068", func_002A9908);

/* Final title-effect handles follow the four packet groups and sprite list. */
typedef struct TitleEffectHandles {
    u8 pad00[0xC4];
    u32 groupA;
    u32 groupB;
    u32 additional[9];
    u32 finalGroup;
} TitleEffectHandles;

/* Release the title screen's effect sprites and resource slot sets. */
s64 func_002A9A40(u8 *work) {
    u32 *handles;
    s32 i;

    mnuReleaseTitleEffectSprites((u32 *)(work + 0x60));
    effDestroyResourceSlotSet(((TitleEffectHandles *)work)->groupA);
    effDestroyResourceSlotSet(((TitleEffectHandles *)work)->groupB);
    handles = ((TitleEffectHandles *)work)->additional;
    for (i = 8; i >= 0; i--) {
        effDestroyResourceSlotSet(*handles++);
    }
    return effDestroyResourceSlotSet(((TitleEffectHandles *)work)->finalGroup);
}

INCLUDE_ASM(const s32, "game/code_002A9068", func_002A9AB8);

typedef struct StaffResourceHeader {
    u8 pad00[0x64];
    s32 resourceSource;         /* 0x064 */
    u8 pad68[0x8C];
    u32 baseHandles[3];        /* 0x0F4 */
    s32 resourceOptions;        /* 0x100 */
    u32 resourceLists[3];      /* 0x104 */
} StaffResourceHeader;

void func_002A9BC8(s32 drawWork, u32 arg1, u32 arg2, s32 record, u32 unused,
                   u32 layer) {
    itfDrawGridWithResolvedSlot(drawWork + 0x60, arg1, arg2, 1, *(u32 *)(*(s32 *)(record + 0x30) + 100), 10,
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

void mnuReleaseStaffSpriteAndResourceHandles(u8 *work) {
    u32 *handles = ((StaffResourceHeader *)work)->resourceLists;
    u32 i;

    for (i = 0; i < 3; i++) {
        mnuDestroyWindowContainer(*handles++);
    }
    mnuReleaseResourceList(((StaffResourceHeader *)work)->baseHandles[0]);
    mnuReleaseResourceList(((StaffResourceHeader *)work)->baseHandles[1]);
    mnuReleaseResourceList(((StaffResourceHeader *)work)->baseHandles[2]);
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

u8 *mnuCreateStaffMenuWork(void) {
    s32 handle;
    u8 *work;
    u8 *effects;

    handle = func_003292A8(0xB1E0);
    work = (u8 *)sdfResourceRetainAddress(handle);
    memset(work, 0, 0xB1E0);
    ((CampVisualWork *)work)->allocationHandle = handle;
    effects = work + 0x11C;
    mnuClearPanelTransitionState(work + 8);
    if (dds3AdminReadPreviousSignedSample() != 0) {
        ((CampVisualWork *)work)->menuResource = mnuAllocateValueRecord(1);
    } else {
        ((CampVisualWork *)work)->menuResource = mnuAllocateValueRecord(0);
    }
    mnuInitPartyPanelSlots((s32)work + 0xA928);
    mnuLoadEffectResources(effects);
    func_002B8140(effects);
    evtCreateMessageWindowIfMissing((s32)D_003E5778);
    movLoadTitleEffects(work);
    func_002A9908(work);
    func_002C1B58(work + 0xAA50, 0x60);
    mnuBuildListSlotTableA((ListSlotWork *)work);
    mnuBuildListSlotTableB((ListSlotWork *)work);
    mnuBuildSkillSlotTable((ListSlotWork *)work);
    func_003425B0();
    return work;
}

void mnuDestroyStaffMenuTask(u32 task) {
    u8 *work = (u8 *)kwlnTaskGetUserValue(task);
    if (work == NULL) {
        return;
    }
    mnuDrainPanelTransitions(work + 8, task);
    mnuReleaseStaffSpriteAndResourceHandles(work);
    mnuDestroyScrollPanel(((CampVisualWork *)work)->modelHandle);
    mnuShutdownContext(work + 0x284);
    dspCloseChannel();
    mnuDestroyEffectResources(work + 0x11c);
    func_002A9A40(work);
    movReleaseTitleEffects(work);
    func_00303D58(((CampVisualWork *)work)->menuResource);
    func_003297C8(((CampVisualWork *)work)->allocationHandle);
    D_00437B72 = 2;
    func_003425D8();
}

u32 func_002AA278(void) {
    s32 work;

    work = kwlnTaskGetUserValue();
    func_002C1B70(work + 0xaa50, 0x53);
    return 0;
}

extern u32 func_002C44E8(s32);

extern s32 func_002A9AB8(s32);

extern s32 evtGetMessageWindowControlState(void);

extern s32 func_002C6CE8(void);

extern s32 fileConsumeConfigTaskReady(void);

extern void mnuDestroyCampTasks(void);

extern void mnuPlayInputSound();

/* On button 8, exit the camp only when both nested guards permit it;
 * otherwise play the alternate sound without destroying its tasks. */
s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 buttons = func_002C44E8(8);
    s32 result;

    if (func_002A9AB8(menu) == 0) {
        return 0;
    }
    result = 0;
    if (evtGetMessageWindowControlState() == 0) {
        if (buttons & 8) {
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

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AA08);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AA18);

void mnuCreateCampTasks(void) {
    s32 work;
    s32 draw;

    work = (s32)mnuCreateStaffMenuWork();
    kwlnTaskCreate(D_00437B78, 0x3F2, 1, 0, func_002AAF70, 0, work);
    draw = kwlnTaskCreate(D_0042AA08, 0x2B07, 1, 0, func_002AB0E0, 0, work);
    kwlnTaskCreate(D_0042AA18, 0x520B, 1, 0, func_002AB1B0, mnuDestroyStaffMenuTask, work);
    func_00101968(draw, kwlnTaskCreate("camp_fade", 0x2B08, 1, 0, func_002AA278, 0, work));
    func_00101968(draw, kwlnTaskCreate("camp_all_cancel", 0x3F3, 1, 0, mnuStaffCampCancelCheck, 0, work));
    kwlnFadeOutStart(0, 0, 0, 0xF);
    D_00437B72 = 1;
}

void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437B78, 0);
    kwlnTaskDestroyWithHierarchyByName(D_0042AA08, 0);
    kwlnTaskDestroyWithHierarchyByName(D_0042AA18, 0);
}

s32 mnuAcknowledgeCampState(void) {
    s8 state = D_00437B72;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437B72 = 0;
    }
    return 0;
}

u8 mnuIsFadeIdle(void) {
    s64 fadeActive;

    fadeActive = kwlnFadeIsActive();
    return fadeActive == 0;
}

/* Draw the camp title using the task's draw context and layer, then ease its
 * horizontal slide toward zero and its opacity toward the fade target. */
void func_002AA530(s32 unused0, s32 unused1, s32 textParam, s32 drawContext, u8 *work, s32 layer) {
    char buffer[16];
    s32 object;
    s32 offset;
    s32 magnitude;
    CampVisualWork *visual = (CampVisualWork *)work;

    if (mdlFlagTest(0x290) == 0) {
        return;
    }
    func_00306CD0((visual->titleSlide + 0x1A) << 4, 0xCB8, 0, visual->titleOpacity, 1, drawContext, 0x3F, layer);
    func_0035C860(buffer, D_00437B80, ((CampCurrency *)D_00435DD0)->currency);
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

void mnuCreateStaffImageSprite(s32 index) {
    u32 *object = (u32 *)func_0019F460(0x340, 0x148, 0, 0xa09dc35a,
                                      D_003E5710[index], 0);
    func_0019D550(object, 1, 0x54);
    frFontQueueGlyphInSelectedSlot(object);
}

INCLUDE_ASM(const s32, "game/code_002A9068", func_002AA7A0);

INCLUDE_ASM(const s32, "game/code_002A9068", func_002AA9D8);

extern void func_002AA9D8(u32, u32, u32, u32, u32, u32, u32, u32, u32);

void func_002AAC70(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002AA9D8(a, b, c, d, e, f, 0, 0, g);
}

void func_002AAC98(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    func_002AAC70(a, b, c, d, e, 0, f);
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
void func_002AACB8(s32 kind, s32 task) {
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
        func_002AA530(0, 0, 0, visual->titleContext, work, 0x53);
    }
}

void func_002AAE80(u32 task) {
    func_002AACB8(0, task);
}

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AA48);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AC40);

INCLUDE_RODATA(const s32, "game/code_002A9068", D_0042AC70);

INCLUDE_SDATA(const s32, "game/code_002A9068", D_00437B78);

INCLUDE_SDATA(const s32, "game/code_002A9068", D_00437B80);

