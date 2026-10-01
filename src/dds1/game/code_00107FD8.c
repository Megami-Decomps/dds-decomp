#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "common.h"

#include "kwln.h"

extern s32 D_003BA960;

extern s32 fldLmapTaskExists(void);

extern s32 fldPollSceneState(void);

extern s32 mnuPollTaskState(void);

extern u32 D_003BA958;

extern s32 fileConsumeConfigTaskReady(void);

extern s32 brsTaskConsumeDone(void);

extern s32 mnuAcknowledgeCampState(void);

extern u32 D_003BA730;

extern s32 fldIsFieldResourceWaitFinished(void);

extern s32 fileMenuTaskExists(void);

extern s32 func_0028F5F8(void);

extern u64 func_00197748(s32, s32, u64, u64, u64, u64);

extern u32 D_003BA8E8;

extern u32 D_003BA904;

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

extern KwlnTask *kwlnTaskFindByPriority(u32 prio);

extern u32 func_00104260(void);

extern void fldStartSequenceRecord(void);

extern void dds3AdminSetControlFlag(void);

extern void func_001A11F0(s32 arg0, s32 arg1, s32 arg2);

extern void evtEventViewerDestroyTask(void);

extern void evtStopTestTasks(void);

extern void evtDestroySkyTask(void);

extern void *sdfCreateAssetWithDrawEntries(void);

extern s8 D_003BA948;

extern void *D_003BD760;

extern u8 D_00324590[];

extern char D_003BA950[];

extern void *D_003BD6B0;

extern f32 D_003BD358;

extern f32 D_003BD35C;

extern u16 D_003BD706;

extern f32 D_003BD708;

extern f32 D_003BD70C;

extern f32 D_003BD710;

extern f32 D_003BD714;

extern u16 D_003BD704;

extern char D_0039E1F0[];

extern char D_0039E200[];

extern s32 D_003BBDA8;

extern void evtCreateEventScriptProcess(s32 arg0);

extern void evtCreateSkyTask(void);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct EvtDrawState {
    u8 pad00[0x1C];
    f32 value1C;
} EvtDrawState;

extern DrawVec4 D_00324790;

extern DrawVec4 D_003C2C20;

extern DrawVec4 D_003C2C30;

extern u16 D_003BD6FA;

extern u16 D_003BD6F8;

extern void func_00109D20(void);

extern void evtSelStateDestroy(void);

extern s32 D_0032E3C0[];

extern void fldStartLmapTask(s32 arg0);

extern s32 sdfDevConsNodeCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern s32 sdfDevConsSetControlByte(s32 arg0, s32 arg1);

extern void sdfDevConsSetTextAttribute(s32 arg0, s32 arg1);

extern void *D_003BA994;

extern void *D_003BA998;

extern u32 D_003BA990;

typedef struct B728Work {
    u8 pad00[0xE8];
    struct B728Work *previous;
    struct B728Work *next;
} B728Work;

extern u8 D_00325748[];

extern s8 D_0032453B[];

extern void func_0010B428(void *arg0);

extern void kwlnTaskDestroyWithHierarchy(void *, s32);

extern void func_0010AC98(void);

extern void *D_003BD764;

extern void func_00109108();

typedef struct KwlnResourceNode {
    s32 unk0;
    struct KwlnResourceNode *next;
    s32 *ready;
} KwlnResourceNode;

extern s32 D_003BD3C8;

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void func_0010B590(void);

extern void *D_003BD768;

extern char D_0039E238[]; /* "DebugTimeGrph" */

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00107FD8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001080D8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108218);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001082D8);

/* Either replace the draw vector immediately or interpolate from its prior value. */
void evtSetDrawVectorTarget(s32 mode, f32 x, f32 y, f32 z, f32 w) {
    if (mode == 0) {
        D_003BA904 &= ~0x400;
        D_00324790.x = x;
        D_00324790.y = y;
        D_00324790.z = z;
        D_00324790.w = w;
    } else {
        f32 b0 = D_00324790.x;
        f32 b1 = D_00324790.y;
        f32 b2 = D_00324790.z;
        f32 b3 = D_00324790.w;
        D_003BA904 |= 0x400;
        D_003BD6FA = mode;
        D_003C2C20.x = b0;
        D_003C2C20.y = b1;
        D_003C2C20.z = b2;
        D_003C2C20.w = b3;
        D_003C2C30.x = x;
        D_003C2C30.y = y;
        D_003C2C30.z = z;
        D_003C2C30.w = w;
        D_003BD6F8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001083F8);

void evtToggleSavedDrawVectors(s32 frames, f32 first, f32 second) {
    if (frames == 0) {
        D_003BA904 &= ~0x4000;
        D_003BD358 = first;
        D_003BD35C = second;
    }
    else {
        f32 previousFirst = D_003BD358;
        f32 previousSecond = D_003BD35C;
        D_003BA904 |= 0x4000;
        D_003BD706 = frames;
        D_003BD708 = previousFirst;
        D_003BD70C = first;
        D_003BD710 = previousSecond;
        D_003BD714 = second;
        D_003BD704 = 0;
    }
}

void evtEnsureDrawVectorState(void) {
    if (D_003BD6B0 == NULL) {
        D_003BD6B0 = sdfCreateAssetWithDrawEntries();
        ((EvtDrawState *)D_003BD6B0)->value1C = 1.0f;
    }
}

void evtSetDrawSurfaceIndex(u32 surfaceIndex) {
    D_003BA8E8 = surfaceIndex;
}

typedef struct EvtDrawSurface {
    u8 unk_00[0x10];
    void (*submit)(struct EvtDrawSurface *, void *);
    u8 unk_14[0xC];
} EvtDrawSurface;

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

extern EvtDrawSurface D_00324B48[];

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, void *);

extern u8 *sdfConsFinalizePacketHeader(void *, s32);

extern void *sdfCreateResetPacketList(void);

extern void func_002D5608(void *);

extern void sdfPktInit(void *, s32, s32, s32, s32);

extern void *sdfFormatSifPacket();

void evtSubmitGsRegister47(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, 0x30);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = 0x47;
    sdfAppendPacket(list, packet);
    {
        EvtDrawSurface *surface = &D_00324B48[D_003BA8E8];
        surface->submit(surface, list);
    }
}

void evtSubmitGsRegister48(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, 0x30);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = 0x48;
    sdfAppendPacket(list, packet);
    {
        EvtDrawSurface *surface = &D_00324B48[D_003BA8E8];
        surface->submit(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108CB8);

void evtSubmitTexturePacket(s32 value) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, 0x30);
    command->data = ((u64)value << 32) | 0x64;
    command->registerId = 0x42;
    sdfAppendPacket(list, packet);
    {
        EvtDrawSurface *surface = &D_00324B48[D_003BA8E8];
        surface->submit(surface, list);
    }
}

extern s32 func_00100518(void);

extern u8 D_00326ED0[];

extern void func_002D4C80(const void *, void *, s32);

extern void sdfAppendDmaTagToList(void *, void *);

void func_00108E60(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80(D_00326ED0 + func_00100518() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        EvtDrawSurface *surface = &D_00324B48[D_003BA8E8];
        surface->submit(surface, list);
    }
}

extern void func_002D4CC8(const void *, void *, s32);

void func_00108F00(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8(D_00326ED0 + func_00100518() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        EvtDrawSurface *surface = &D_00324B48[D_003BA8E8];
        surface->submit(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108FA0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109108);

void func_001093B8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    func_00109108(a0, a1, a2, a3, 0xFFFFFF, a4, a5, a6, a7);
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001093F8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109640);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109810);

extern f32 D_003245B0[];

extern u32 D_003245D0[];

extern void *func_002EF2B0(const void *, const void *, s32, s32);

void evtSubmitViewParamPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    void *list;
    D_003245B0[0] = x;
    D_003245B0[1] = y;
    D_003245B0[2] = z;
    D_003245B0[4] = u;
    D_003245B0[5] = v;
    D_003245B0[6] = w;
    D_003245D0[1] = second;
    D_003245D0[0] = first;
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_002EF2B0(D_003245B0, D_003245D0, 2, 0x80));
    {
        EvtDrawSurface *surface = &D_00324B48[D_003BA8E8];
        surface->submit(surface, list);
    }
}

void evtDrawPositionedSurfacePacket(s32 x, s32 y, s32 packetArg, s32 drawArg) {
    u8 pkt[16];
    void *list;
    void *packet;
    EvtDrawSurface *surface;
    list = (void *)sdfCreateResetPacketList();
    packet = sdfAllocPacketAligned(0x40);
    func_002D5608(packet);
    sdfAppendPacket(list, packet);
    sdfPktInit(pkt, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, packetArg);
    sdfAppendPacket(list, sdfFormatSifPacket(pkt, drawArg));
    surface = (EvtDrawSurface *)D_00325748;
    surface->submit(surface, list);
}

void evtPrepareSizedDrawResource(s32 width, s32 height, u64 first, u64 second) {
    u64 resource;

    resource = func_00197748(width << 4, height << 3, 0, first, second, 0);
    func_00195868(resource);
    frFontQueueGlyphInSelectedSlot(resource);
}

typedef struct EvtSelState {
    u8 unk_00[4];
    s32 limit;
    s32 unk_08;
    s32 unk_0C;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    u8 unk_16[2];
    s32 count;
} EvtSelState;

s32 evtSelStateCreate(s32 limit, s16 frames, s32 value08, s32 value0C) {
    EvtSelState *node;
    if (frames == 0) {
        return 0;
    }
    if (D_003BA948 != 0) {
        evtSelStateDestroy();
    }
    node = sdfAllocAndClearQuadwords(0x28);
    D_003BD760 = node;
    if (limit == 0) {
        node->limit = -1;
    } else {
        node->limit = limit;
    }
    ((EvtSelState *)D_003BD760)->unk_08 = value08;
    ((EvtSelState *)D_003BD760)->unk_0C = value0C;
    ((EvtSelState *)D_003BD760)->unk_10 = frames;
    ((EvtSelState *)D_003BD760)->unk_12 = frames;
    ((EvtSelState *)D_003BD760)->unk_14 = frames;
    D_003BA948 = 1;
    return 1;
}

u32 evtCheckSelectionState(void) {
    EvtSelState *sel;
    if (D_003BA948 == 0) {
        return 0;
    }
    sel = D_003BD760;
    if (sel->limit > sel->count || sel->limit == -1) {
        func_00109D20();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109D20);

void evtDestroySelectionState(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (D_003BA948 != 0) {
        sdfReleaseChipBlock(D_003BD760);
        D_003BA948 = 0;
        VU0_MOVE_VF(vf10, vf0);
        VU0_CLEAR_W(vf10);
        VU0_STORE_VF(vf10, D_00324590);
    }
}

s32 evtStartSelCreate(void) {
    return kwlnTaskCreate(D_0039E1F0, 0x2AF9, 0, 0, (s32)func_00104260, 0, 0);
}

u32 evtStartSelDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0039E1F0, 1);
    return 1;
}

u32 func_0010A210(void) {
    return 0;
}

void evtOpenFileMenuModeThree(void) {
    fileEnterMcPackScene(3);
}

u32 evtPollFileMenuModeThree(void) {
    s64 operationResult;
    u32 status;

    func_0028F458();
    fileReleaseMenuResources();
    operationResult = func_0028F5F8();
    status = 0xffffffff;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u8 evtWaitFileMenuTaskThree(void) {
    s64 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

void func_0010A288(u32 unused, u32 *value) {
    if (value == 0) {
        func_0026BCE8(0);
        return;
    }
    func_0026BCE8(*value);
}

u32 func_0010A2B8(void) {
    func_0026BD08();
    return 0;
}

u32 func_0010A2D8(void) {
    return 0;
}

void evtOpenFileMenuModeOne(void) {
    fileEnterMcPackScene(1);
}

s32 evtCheckSelectionInput(void) {
    func_0028F458();
    fileReleaseMenuResources();
    if (func_0028F5F8() != 0) {
        fldStartSequenceRecord();
        return 0;
    }
    return -1;
}

u8 evtIsFileMenuTaskAbsent(void) {
    s64 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

void evtStartStaffMovieRequest(void) {
    mnuStartStaffMovieRequest();
}

u32 evtStopStaffTasks(void) {
    mnuStopStaffTasks();
    return 0;
}

u32 func_0010A390(void) {
    return 0;
}

void evtOpenFileMenuModeTwo(void) {
    fileEnterMcPackScene(2);
}

u32 evtPollFileMenuModeTwo(void) {
    s64 operationResult;
    u32 status;

    func_0028F458();
    fileReleaseMenuResources();
    operationResult = func_0028F5F8();
    status = 0xffffffff;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u8 evtWaitFileMenuTaskExit(void) {
    s64 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

void evtStartAreaFromSelection(u32 selection, u32 area) {
    kwlnFadeBackgroundStartOut(0);
    func_00125E08(area, selection);
}

u32 evtPollSelectedAreaReady(void) {
    s64 operationResult;
    u32 status;

    fldCleanupFieldScene();
    operationResult = fldIsFieldResourceWaitFinished();
    status = 0xffffffff;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u32 func_0010A478(void) {
    return 0;
}

void evtStartSelectionFadeOut(void) {
    kwlnFadeBackgroundStartOut(0);
}

u32 func_0010A498(void) {
    return 0;
}

u32 func_0010A4A0(void) {
    return 0;
}

/* Source zero takes an explicit value; source one uses the pending event mode. */
void evtDispatchSelectionValue(s32 source, s32 *params) {
    s32 selection = 0;

    D_003BA730 = 0;
    switch (source) {
    case 0:
        selection = params[0];
        break;
    case 1:
        selection = D_003BBDA8;
        break;
    }
    if (selection <= 0) {
        return;
    }
    evtCreateSkyTask();
    evtCreateEventScriptProcess(selection);
}

u32 evtCloseSkyEventTask(void) {
    D_003BA730 = 1;
    evtDestroySkyTask();
    fldDispatchDeferredFieldCommand();
    return 0;
}

u32 func_0010A540(void) {
    return 0;
}

void func_0010A548(void) {
    mnuCreateCampTasks();
}

u32 evtDestroyCampTasks(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 evtWaitCampStateAcknowledged(void) {
    s64 campState;

    campState = mnuAcknowledgeCampState();
    return campState == 0;
}

void func_0010A5A0(void) {
}

u32 func_0010A5A8(void) {
    return 0;
}

u32 func_0010A5B0(void) {
    return 1;
}

void func_0010A5B8(void) {
}

u32 func_0010A5C0(void) {
    return 0;
}

u32 func_0010A5C8(void) {
    return 1;
}

void evtCreateStaffTasks(void) {
    mnuStaffCreateTasks();
}

u32 evtDestroyStaffTasks(void) {
    fldDispatchDeferredFieldCommand();
    mnuStaffDestroyTasks();
    return 0;
}

u8 evtWaitStaffTaskDone(void) {
    s64 taskResult;

    taskResult = brsTaskConsumeDone();
    return taskResult == 0;
}

void evtCreateConfigTasks(void) {
    mnuCreateConfigTasks(1);
}

u32 evtDestroyConfigTasks(void) {
    mnuConfigTasksDestroy();
    return 0;
}

u8 evtWaitConfigTaskDone(void) {
    s64 taskResult;

    taskResult = fileConsumeConfigTaskReady();
    return taskResult == 0;
}

void evtDispatchSelectionCommand(s32 source, s32 *params) {
    if (source == 0) {
        if (params == NULL) {
            func_001A11F0(1, 0, 0);
        }
        else {
            func_001A11F0(0, params[0], params[1]);
        }
    }
    else {
        dds3AdminSetControlFlag();
        fldDispatchDeferredFieldCommand();
    }
}

void func_0010A6E0(void) {
    func_001A1068();
}

u32 evtEnsureSelectionTask(void) {
    if (kwlnTaskGetTaskByName(D_003BA950) != NULL) {
        return 0;
    }
    if (kwlnTaskFindByPriority(0x3FA) == NULL) {
        fldDispatchDeferredFieldCommand();
        return 1;
    }
    return 0;
}

void evtCreateBattleStageTestTask(void) {
    btlCreateStageTestTask();
}

u32 func_0010A760(void) {
    evtBattleStageTestStopTask();
    return 0;
}

u32 func_0010A780(void) {
    return 0;
}

void evtCreateTestAndSkyTasks(void) {
    D_003BA730 = 0;
    D_003BA958 = 0;
    evtStartTestTask();
    evtCreateSkyTask();
}

extern void kwlnFadeBackgroundStartOut();

u32 evtInitializeSelectionScene(void) {
    D_003BA730 = 1;
    kwlnFadeBackgroundStartOut(0);
    evtStopTestTasks();
    evtDestroySkyTask();
    return 1;
}

u32 func_0010A7E0(void) {
    return 0;
}

void evtClearSecondaryWorldAndViewer(void) {
    D_003BA730 = 0;
    evtDestroySecondaryWorldNode();
    evtViewerCreateTaskWithSky();
}

u32 evtInitializeSelectionScreen(void) {
    evtEventViewerDestroyTask();
    D_003BA730 = 1;
    kwlnFadeBackgroundStartOut(0);
    return 0;
}

u32 func_0010A838(void) {
    return 0;
}

void func_0010A840(void) {
}

u32 func_0010A848(void) {
    return 0;
}

u32 func_0010A850(void) {
    return 0;
}

void func_0010A858(void) {
    mnuCreateCampTasks();
}

u32 func_0010A870(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 func_0010A890(void) {
    s64 campState;

    campState = mnuAcknowledgeCampState();
    return campState == 0;
}

void evtCreateSkyAndCampTasks(u32 unused, u32 campMode) {
    evtCreateSkyTask();
    func_002449F0(campMode);
}

u32 evtDestroySkyAndCampTasks(void) {
    evtDestroySkyTask();
    mnuCampDestroyPanelTasks();
    fldProcessDeferredSceneCommand();
    return 0;
}

u8 evtWaitSkyCampTaskState(void) {
    s64 taskState;

    taskState = mnuPollTaskState();
    return taskState == 0;
}

void evtCreateSkyAndFieldTasks(u32 unused, u32 *fieldArgs) {
    evtCreateSkyTask();
    if (fieldArgs != 0) {
        func_00249FA8(fieldArgs[0], fieldArgs[1]);
        return;
    }
    func_00249FA8(0, 1);
}

u32 evtDestroySkyAndFieldTasks(void) {
    evtDestroySkyTask();
    fldStopSceneTasks();
    return 0;
}

u8 evtWaitFieldSceneState(void) {
    s64 sceneState;

    sceneState = fldPollSceneState();
    return sceneState == 0;
}

void evtCreateStaffTasksAlternate(void) {
    mnuStaffCreateTasks();
}

u32 evtDestroyStaffTasksAlternate(void) {
    mnuStaffDestroyTasks();
    return 0;
}

u8 evtWaitStaffTaskDoneAlternate(void) {
    s64 taskResult;

    taskResult = brsTaskConsumeDone();
    return taskResult == 0;
}

void func_0010AA10(void) {
}

u32 func_0010AA18(void) {
    return 0;
}

u32 func_0010AA20(void) {
    return 0;
}

void func_0010AA28(void) {
}

u32 func_0010AA30(void) {
    return 0;
}

u32 func_0010AA38(void) {
    return 0;
}

void evtLaunchFontTestScene(void) {
    func_001983A8();
}

u32 func_0010AA58(void) {
    return 0;
}

u32 evtTestFontCheck(void) {
    return kwlnTaskGetTaskByName(D_0039E200) == NULL;
}

void evtCreateMovieViewerTask(void) {
    mnuCreateMovieViewerTask();
}

u32 evtDestroyMovieViewerTask(void) {
    mnuDestroyMovieViewerTask();
    return 0;
}

u32 func_0010AAC0(void) {
    return 0;
}

void func_0010AAC8(void) {
    func_002C16E0();
}

u32 func_0010AAE0(void) {
    func_002C16E8();
    return 0;
}

u32 func_0010AB00(void) {
    return 0;
}

void evtSelectFontResource(s32 unused, s32 *lmapArgs) {
    if (lmapArgs == NULL) {
        fldStartLmapTask(0);
    }
    else {
        fldStartLmapTask(lmapArgs[0]);
    }
    D_0032E3C0[0] = 0x3E7;
}

u32 evtDestroyLmapTask(void) {
    fldStopLmapTask();
    fldRunPendingSceneAction();
    return 0;
}

u8 evtWaitLmapTaskGone(void) {
    s64 taskExists;

    taskExists = fldLmapTaskExists();
    return taskExists == 0;
}

u32 evtEnsureFontResourceChain(void) {
    if (D_003BA960 == 0) {
        D_003BA960 = sdfDevConsNodeCreate(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetControlByte(D_003BA960, 2);
        sdfDevConsSetTextAttribute(D_003BA960, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (D_003BA960 != 0) {
        sdfDevConsNodeDestroy(D_003BA960);
        D_003BA960 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AC10);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AC98);

void func_0010AEE8(void) {
}

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E1F0);

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E200);

void evtToggleDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_003BD764 = kwlnTaskCreate("DebugTimeGrph", 0x2710, 1, 1, func_0010AC98, func_0010AEE8, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_003BD764, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AF68);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B1B0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B428);

s32 func_0010B558(void) {
    if (D_0032453B[0] != 0) {
        return 0;
    }
    func_0010B428(D_00325748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B590);

void func_0010B6A8(void) {
}

void func_0010B6B0(s8 mode) {
    if (mode == 1) {
        D_003BD768 = kwlnTaskCreate(D_0039E238, 0x2710, 1, 1, func_0010B590, func_0010B6A8, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_003BD768, 0);
    }
}

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void evtLinkWorkNode(B728Work *node) {
    B728Work *tail = D_003BA998;

    if (tail == NULL) {
        D_003BA994 = node;
        D_003BA998 = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->previous = tail;
        tail->next = node;
        node->next = NULL;
        D_003BA998 = node;
    }
    D_003BA990++;
}

/* Detach from either end or the middle, and clear the old links. */
void evtUnlinkWorkNode(B728Work *node) {
    if ((B728Work *)D_003BA994 == node) {
        D_003BA994 = node->next;
    }
    else {
        node->previous->next = node->next;
    }
    if ((B728Work *)D_003BA998 == node) {
        D_003BA998 = node->previous;
    }
    else {
        node->next->previous = node->previous;
    }
    node->previous = NULL;
    node->next = NULL;
    D_003BA990--;
}

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E238);

INCLUDE_ASM(const s32, "game/code_00107FD8", bfContextCreate);

INCLUDE_ASM(const s32, "game/code_00107FD8", bfParseFLW0);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA948);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA950);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA958);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA960);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA968);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA970);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA978);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA97C);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA980);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA988);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA990);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA994);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA998);

