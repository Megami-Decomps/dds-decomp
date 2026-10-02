#include "common.h"

extern u64 func_00312810(u32, u64);

extern u32 mnuMantraSelectionResource;

extern u8 D_00426060[];

extern void func_00286F18(s32, s32);

extern u8 D_003CFCC0[];

extern void mnuReleaseSelectionWorkResources();

extern void mnuDestroyListState(s32);

extern void mnuCloseCurrentProfilePanel(s32);

extern void mnuReleaseMantraMenuDrawResources(void *);

extern void dspCloseChannel(void);
extern void sdfQueueNonzeroResourceId(u32);
extern void mnuReleaseFirstMantraSpriteSlots(void);
extern void mnuReleaseStaffAndTitleVisualResources(u32 *);
extern void evtPrintDeveloperConsoleMessage(const char *);
extern void func_003297C8(u32);
extern void mnuReleasePanelEntryPool(void);
INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286BA8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286E20);

extern u32 func_003292A8(s32 size);
extern u32 sdfMemoryGetBlockAddress(void *block);
extern u32 mnuCreateProgressHost(void);
extern void mnuInitPanelSoundEntries(void);

/* Allocate and zero the 0xC08 status resource, then wire up its host pointer,
 * console banner and panel sound entries. */
void *func_00286E98(void) {
    void *handle = (void *)func_003292A8(0xC08);
    u8 *work = (u8 *)sdfMemoryGetBlockAddress(handle);

    memset(work, 0, 0xC08);
    *(u32 *)work = (u32)handle;
    *(u32 **)(work + 0x48) = (u32 *)mnuCreateProgressHost();
    evtPrintDeveloperConsoleMessage("trmLoadStartStatusResource()!!!! \n");
    evtPrintDeveloperConsoleMessage("mtrInit\n");
    mnuInitPanelSoundEntries();
    return work;
}

void func_00286F18(s32 arg0, s32 work) {
    if (work != 0) {
        dspCloseChannel();
        sdfQueueNonzeroResourceId(*(u32 *)(work + 0x38));
        sdfQueueNonzeroResourceId(*(u32 *)(work + 0x3C));
        mnuReleaseFirstMantraSpriteSlots();
        mnuReleaseStaffAndTitleVisualResources(*(u32 **)(work + 0x48));
        evtPrintDeveloperConsoleMessage("trmDestroyStatusResource()!!!! \n");
        func_003297C8(*(u32 *)work);
        mnuReleasePanelEntryPool();
    }
    evtPrintDeveloperConsoleMessage("mtrRelease\n");
}

/* Store the task handle so the existence probe and explicit stop can
 * invalidate or destroy the same resource group. */
void mnuCreateResourceTask(void) {
    void *menuData = func_00286E98();
    mnuMantraSelectionResource = sdfCreateTaskWorker(D_00426060, 0x402, 0x2B12, D_003CFCC0, func_00286F18, menuData);
}

s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(D_00426060) != 0) {
        return 1;
    }
    mnuMantraSelectionResource = 0;
    return 0;
}

void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(mnuMantraSelectionResource);
    mnuMantraSelectionResource = 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287030);



INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426060);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287078);

s32 mtrUnitSelectInit(void) {
    u64 selected = func_00312810(mnuMantraSelectionResource, 0xffffffffffffffff);

    func_002885E8(selected);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectInit\n");
    return 0;
}

void mtrUnitSelectRelease(void) {
    u64 selected = func_00312810(mnuMantraSelectionResource, 0xffffffffffffffff);

    mnuReleaseSelectionWorkResources(selected);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectRelease\n");
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287670);

/* Both handlers consume the current resource-task selection, but report
 * completion independently of the selected value. */
u64 func_00287768(void) {
    u64 selected;

    selected = func_00312810(mnuMantraSelectionResource, 0xffffffffffffffff);
    func_00288920(selected);
    return 0;
}

s32 mtrMantraSelectInit(void) {
    u64 selected = func_00312810(mnuMantraSelectionResource, 0xffffffffffffffff);

    func_00267F68(0);
    mnuOpenMantraSelectionAndLoadTitleStream(selected);
    evtPrintDeveloperConsoleMessage("mtrMantraSelectInit\n");
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", mtrMantraSelectRelease);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287848);

u64 func_00287900(void) {
    u64 selected;

    selected = func_00312810(mnuMantraSelectionResource, 0xffffffffffffffff);
    func_0028B1B0(selected);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287930);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287AF8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287C20);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288158);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002882B8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002884C0);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002885E8);

void mnuReleaseSelectionWorkResources(u8 *work) {
    mnuDestroyListState(*(s32 *)(work + 4));
    mnuCloseCurrentProfilePanel(*(s32 *)(work + 0x48));
    mnuReleaseMantraMenuDrawResources(work);
}

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426280);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288748);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288920);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288A70);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288BD8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288DD0);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_004262B0);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437918);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437920);

INCLUDE_SDATA(const s32, "game/code_00286BA8", mnuMantraSelectionResource);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437928);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_0043792C);

