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

extern u32 D_003BC62C;

extern s32 D_003BC630;

extern char D_003B1AC8[];

extern s32 kwlnTaskGetUserValue();

extern u32 D_003DC5C8[];

extern char D_003BC6B8[]; /* "camp" */

extern char D_003B20C0[]; /* "camp_draw" */

extern char D_003B20D0[]; /* "camp_update" */

extern s8 D_003BC6B4;

extern void effResolveAndReleaseResource(u32);

extern s32 D_003BC614;

extern u8 D_0037B950[];

extern u8 D_0037B970[];

extern u8 D_0037B980[];

extern u8 D_0037C388[];

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

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1AF0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B00);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B10);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B20);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B30);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B48);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B60);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B78);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1B88);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1BA0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1BB8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1BC8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1BE0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1BF8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C10);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C28);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C38);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C48);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C60);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C78);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1C90);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1CA8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1CB8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1CC8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1CD8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1CE8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1CF8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D08);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D28);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D38);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D48);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D58);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D68);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D78);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D88);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1D98);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1DA8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1DB8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1DC8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1DD8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1DE8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1DF8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E08);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E18);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E28);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E48);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E58);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E68);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E78);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E88);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1E98);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1EA8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1EB8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1EC8);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1EE0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F00);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F18);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F30);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F48);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F60);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F70);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F80);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1F90);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1FA0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1FB0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1FC0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1FD0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1FE0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B1FF0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2000);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2010);

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

extern u8 *D_003BAA00;

extern s8 D_003BC6B5;

void movReleaseActivePartyCategoryModels(s32 list, s32 count, u8 *work) {
    s32 i;

    effResolveAndReleaseResource(*(u32 *)list);
    for (i = 0; i < 5; i++) {
        u8 *slot = D_003BAA00 + 0xA60 + i * 0x1A4;

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
typedef struct StaffDisplayModeState {
    u8 pad00[0x910];
    s32 displayMode;
} StaffDisplayModeState;

void mnuSetStaffDisplayMode(s32 next, u8 *context) {
    s32 previous = ((StaffDisplayModeState *)context)->displayMode;
    if (next != previous) {
        if (previous != 0) {
            mnuReleaseStaffCategoryTextureHandles(previous);
        }
        if (next != 0) {
            movReleaseCategoryModels(next, context);
        }
        ((StaffDisplayModeState *)context)->displayMode = next;
    }
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00271368);

typedef struct StaffSpriteHandles {
    u8 pad00[0x118];
    u32 primaryImage;
    u32 resourceList;
    u32 secondaryImage;
    u32 images[3];
    u32 extraImages[2];
    u32 scrollPanel; /* 0x138: created panel drawn and destroyed with this task */
} StaffSpriteHandles;

void mnuReleaseStaffSpriteHandles(StaffSpriteHandles *handles) {
    u32 *image = handles->extraImages;
    u32 index = 0;
    effDestroyPackedBatch(handles->primaryImage);
    effDestroyPackedBatch(handles->secondaryImage);
    do {
        effDestroyPackedBatch(*image++);
        index++;
    } while (index < 2);
}

void mnuInitializeStaffPageWindows(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    mnuInitPageWindow(arg0, arg3, arg1[3], 7, arg1[4], 0, *arg1, 0x11);
    func_0027FAA8(arg0, *arg1);
    func_0027FBE0(arg0, arg1 + 9);
    func_0027FC10(arg0, arg1 + 0x11);
    mnuRegisterResourceHandles(arg0, arg1 + 0x19);
    mnuUpdateHandleStates(arg0);
}

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

void mnuCreateStaffPanelSet(StaffSpriteHandles *menu) {
    menu->resourceList = func_0027D4A0(0, *(u32 *)((u8 *)menu + 0x6C), menu->secondaryImage);
    menu->images[0] = func_00271B50(D_0037B950, 8, 0x300, menu, D_0037C388);
    mnuForwardDupArg(menu->images[0], *(u32 *)((u8 *)menu + 0x74), 0, 0, 0);
    menu->images[1] = func_00271B50(D_0037B970, 3, 0x2C0, menu, 0);
    mnuSetWindowContainerState(menu->images[1], 0x100);
    menu->images[2] = func_00271B50(D_0037B980, 2, 0x200, menu, 0);
    mnuSetWindowContainerState(menu->images[2], 0x100);
}

void mnuReleaseStaffSpriteAndResourceHandles(StaffSpriteHandles *handles) {
    u32 *image = handles->images;
    u32 index = 0;
    do {
        mnuDestroyWindowContainer(*image++);
    } while (++index < 3);
    mnuReleaseResourceList(handles->resourceList);
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00271E58);

extern s32 kwlnTaskGetUserValue();

extern s8 D_003BC6B4;

extern void func_002D0918(u32);

void mnuDestroyStaffMenuTask(u32 task) {
    u8 *work = (u8 *)kwlnTaskGetUserValue(task);
    if (work == NULL) {
        return;
    }
    mnuDrainPanelTransitions(work + 8, task);
    mnuReleaseStaffSpriteAndResourceHandles((StaffSpriteHandles *)work);
    mnuDestroyScrollPanel(((StaffSpriteHandles *)work)->scrollPanel);
    mnuShutdownContext(work + 0x15C);
    dspCloseChannel();
    mnuReleaseAssets(work + 0x13C);
    mnuReleaseStaffResourceSlotGroups(work);
    mnuReleaseStaffSpriteHandles((StaffSpriteHandles *)work);
    func_002BC618(*(u32 *)(work + 0x5c));
    func_002D0918(*(u32 *)work);
    D_003BC6B4 = 2;
    func_002E9730();
}

u32 func_00271FC8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_00283BF8(context + 0x914, 0x53);
    return 0;
}

s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 buttons = func_00285B20(8);
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

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B20C0);

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B20D0);

INCLUDE_ASM(const s32, "game/code_00270FB0", mnuCreateCampTasks);

void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC6B8, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B20C0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B20D0, 0);
}

s32 mnuAcknowledgeCampState(void) {
    s8 state = D_003BC6B4;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC6B4 = 0;
    }
    return 0;
}

u8 mnuIsFadeIdle(void) {
    s64 fadeActive;

    fadeActive = kwlnFadeIsActive();
    return fadeActive == 0;
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00272280);

extern u32 D_0037B988[];

void mnuCreateStaffImageSprite(s32 index) {
    u32 *object = (u32 *)func_00197760(0x2F0, 0x1E0, 0, 0xa09dc35a,
                                      D_0037B988[index], 0);
    func_001958A0(object, 1, 0x54);
    frFontQueueGlyphInSelectedSlot(object);
}

INCLUDE_ASM(const s32, "game/code_00270FB0", func_002723B0);

INCLUDE_ASM(const s32, "game/code_00270FB0", func_00272518);

void func_00272668(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_00272518(arg0, arg1, arg2, arg3, arg4, 0, arg5);
}

void mnuDrawStaffCampScreen(s32 arg0, s32 arg1) {
    u8 *menu = (u8 *)kwlnTaskGetUserValue(arg1);

    mnuDrawBackdrop(menu + 0x13C, 0x20);
    if (func_002719F0(arg1) == 0) {
        return;
    }
    func_0027E8D8(-0x10, -8, 0, (s32)((StaffSpriteHandles *)menu)->scrollPanel, 0x53);
    mnuDrawPanelListDefault(0, 0, 0, menu + 0x15C, 0x53);
    if (arg0 == 0) {
        itfDrawGridWithResolvedSlot(0x1AB0, 0x70, 0, 1, *(s32 *)(menu + 0x64), 6, 0x53);
        itfDrawGridWithResolvedSlot(0x17A0, 0x78, 0, 1, *(s32 *)(menu + 0x60), 0xF, 0x53);
        itfDrawGridWithResolvedSlot(0x1E40, 0x78, 0, 1, *(s32 *)(menu + 0x60), 0x10, 0x53);
    }
}

void func_00272778(u32 arg0) {
    mnuDrawStaffCampScreen(0, arg0);
}

INCLUDE_RODATA(const s32, "game/code_00270FB0", D_003B2100);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6B4);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6B5);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6B8);

INCLUDE_SDATA(const s32, "game/code_00270FB0", D_003BC6C0);

