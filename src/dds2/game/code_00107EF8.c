#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern s32 func_00316ED0(void);

extern u32 D_00435D28;

extern u64 func_0019F448(s32, s32, u64, u64, u64, u64);

extern u32 kwlnDrawSurfaceIndex;

extern s32 func_002CE920(void);

extern s32 fileMenuTaskExists(void);

extern s32 fldIsFieldResourceWaitFinished(void);

extern u32 D_00435BB0;

extern s32 mnuAcknowledgeCampState(void);

extern s32 brsTaskConsumeDone(void);

extern s32 fileConsumeConfigTaskReady(void);

extern s32 mnuCampConsumePanelTaskCompletion(void);

extern s32 fldPollSceneState(void);

extern s32 fldLmapTaskExists(void);

extern s32 evtConsoleFontResourceChain;

extern u32 kwlnDrawControlFlags;

extern f32 D_00438A48;

extern f32 D_00438A4C;

extern u16 D_00438E06;

extern f32 D_00438E08;

extern f32 D_00438E0C;

extern f32 D_00438E10;

extern f32 D_00438E14;

extern u16 D_00438E04;

extern void *sdfCreateAssetWithDrawEntries(void);

extern void *D_00438DB0;

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern char D_00411370[];

extern void fldStartSequenceRecord(void);

typedef struct KwlnTask KwlnTask;

extern KwlnTask *func_00101740(const char *name);

extern KwlnTask *func_00101820(u32 prio);

extern char D_00435D20[];

extern void evtEventViewerDestroyTask(void);

extern char D_00411380[];

extern s32 D_00389780[];

extern void fldStartLmapTask(s32 arg0);

extern void *scrNamedProcessHead;

extern void *scrNamedProcessTail;

extern u32 scrNamedProcessCount;

typedef struct {
    u8 pad00[0xE8];
    void *previous;
    void *next;
} B728Work;

extern u8 kwlnPositionedTextSurface[];

extern s8 D_0037F53B[];

extern void func_0010B650(void *arg0);

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

extern DrawVec4 kwlnDrawVector;

extern DrawVec4 D_0043E3A0;

extern DrawVec4 D_0043E3B0;

extern u16 D_00438DFA;

extern u16 D_00438DF8;

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

extern EvtDrawSurface kwlnDrawSurfaces[];

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, void *);

extern u8 *sdfConsFinalizePacketHeader(void *, s32);

extern s32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_0032DB30(const void *, void *, s32);

extern void sdfAppendDmaTagToList(void *, void *);

extern void func_0032DB78(const void *, void *, s32);

extern f32 D_0037F5B0[];

extern u32 D_0037F5D0[];

extern void *func_00348158(const void *, const void *, s32, s32);

extern s8 evtSelectionStateActive;

extern void *evtSelectionState;

extern void evtSelStateDestroy(void);

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

extern void func_00109E60(void);

extern u8 D_0037F590[];

extern s32 kwlnTaskCreate(const char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);

extern u32 func_00104150(void);

extern s32 evtPendingEventSelection;

extern void evtCreateEventScriptProcess(s32 arg0);

extern void evtCreateSkyTask(void);

extern void evtStopTestTasks(void);

extern void evtDestroySkyTask(void);

extern void kwlnFadeBackgroundStartOut();

extern s32 sdfDevConsNodeCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern s32 sdfDevConsSetControlByte(s32 arg0, s32 arg1);

extern void sdfDevConsSetTextAttribute(s32 arg0, s32 arg1);

extern void dds3AdminSetControlFlag(void);

extern void func_001A9F30(s32 arg0, s32 arg1, s32 arg2);

extern void kwlnTaskDestroyWithHierarchy(void *, s32);

extern void func_0010AEC0(void);

extern void *D_00438E64;

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00107EF8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00107FF8);

extern u16 D_00438DE6;
extern u16 D_00438DE8;
extern u32 D_00438DEC;
extern u32 D_00438DF0;
extern u8 kwlnDefaultColorVector[];

/* vu0 routine: set the background colour target: immediately (mode 0) or blend from the default over `mode` frames. */
void kwlnSetBackgroundColorTarget(s32 mode, f32 *color) {
    s32 first[4];
    s32 second[4];
    u32 packedFirst;
    u32 packedSecond;

    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x100;
        VU0_LOAD_VF(vf10, color);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, kwlnDefaultColorVector);
    } else {
        kwlnDrawControlFlags |= 0x100;
        D_00438DE8 = mode;
        D_00438DE6 = 0;
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packedFirst);
        first[0] = packedFirst;
        D_00438DEC = first[0];
        VU0_LOAD_VF(vf10, color);
        VU0_SET_W_ONE(vf10);
        EE_MMI_RGBA_PACK(packedSecond);
        second[0] = packedSecond;
        D_00438DF0 = second[0];
    }
}

extern u32 D_0037F7A0[];
extern u16 D_00438DF4;
extern u16 D_00438DF6;
extern u32 D_00438DFC;
extern u32 D_00438E00;

/* vu0 routine: set the draw colour target: immediately (mode 0) or interpolate from the previous colour. */
void kwlnSetDrawColorTarget(s32 mode, f32 *color) {
    u32 color32[4];
    u32 packed;

    VU0_LOAD_VF(vf10, color);
    VU0_CLEAR_W(vf10);
    EE_MMI_RGBA_PACK_F255(packed);
    color32[0] = packed;
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x200;
        D_0037F7A0[0] = color32[0];
    } else {
        kwlnDrawControlFlags |= 0x200;
        D_00438DF6 = mode;
        D_00438E00 = color32[0];
        D_00438DFC = D_0037F7A0[0];
        D_00438DF4 = 0;
    }
}


/* Either replace the draw vector immediately or interpolate from its prior value. */
void evtSetDrawVectorTarget(s32 mode, f32 x, f32 y, f32 z, f32 w) {
    if (mode == 0) {
        kwlnDrawControlFlags &= ~0x400;
        kwlnDrawVector.x = x;
        kwlnDrawVector.y = y;
        kwlnDrawVector.z = z;
        kwlnDrawVector.w = w;
    } else {
        f32 previousX = kwlnDrawVector.x;
        f32 previousY = kwlnDrawVector.y;
        f32 previousZ = kwlnDrawVector.z;
        f32 previousW = kwlnDrawVector.w;
        kwlnDrawControlFlags |= 0x400;
        D_00438DFA = mode;
        D_0043E3A0.x = previousX;
        D_0043E3A0.y = previousY;
        D_0043E3A0.z = previousZ;
        D_0043E3A0.w = previousW;
        D_0043E3B0.x = x;
        D_0043E3B0.y = y;
        D_0043E3B0.z = z;
        D_0043E3B0.w = w;
        D_00438DF8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108318);

void evtToggleSavedDrawVectors(s32 frames, f32 first, f32 second) {
    if (frames == 0) {
        kwlnDrawControlFlags &= ~0x4000;
        D_00438A48 = first;
        D_00438A4C = second;
    }
    else {
        f32 previousFirst = D_00438A48;
        f32 previousSecond = D_00438A4C;
        kwlnDrawControlFlags |= 0x4000;
        D_00438E06 = frames;
        D_00438E08 = previousFirst;
        D_00438E0C = first;
        D_00438E10 = previousSecond;
        D_00438E14 = second;
        D_00438E04 = 0;
    }
}

void evtEnsureDrawVectorState(void) {
    if (D_00438DB0 == NULL) {
        D_00438DB0 = sdfCreateAssetWithDrawEntries();
        ((EvtDrawState *)D_00438DB0)->value1C = 1.0f;
    }
}

void evtSetDrawSurfaceIndex(u32 surfaceIndex) {
    kwlnDrawSurfaceIndex = surfaceIndex;
}

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
        EvtDrawSurface *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
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
        EvtDrawSurface *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->submit(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108BD8);

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
        EvtDrawSurface *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->submit(surface, list);
    }
}

void func_00108D80(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        EvtDrawSurface *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->submit(surface, list);
    }
}

void func_00108E20(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, texture);
    {
        EvtDrawSurface *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->submit(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108EC0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109028);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109248);

extern void func_00109248();

void func_001094F8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    func_00109248(a0, a1, a2, a3, 0xFFFFFF, a4, a5, a6, a7);
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109538);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109780);

typedef struct EvtQuadDesc {
    s16 kind;
    s16 count;
    u8 pad4[4];
    s32 color;
    u64 *strip;
    f32 *verts;
    u8 pad14[4];
    f32 *uvs;
    u8 pad1C[4];
    s32 *indices;
    u8 pad24[8];
} EvtQuadDesc; /* 0x2C bytes */

extern u64 D_0037F5A0[]; /* index table; only the first 8 bytes are used */
extern void sdfConsAppendClearPacket(void *, s32);
extern void sdfConsAppendAssetPacket(void *, void *, s32);
extern void *func_0033B050(EvtQuadDesc *);
extern void func_003332E8(void *, u32);
extern void sdfQueueAssetRelease(void *);

void evtSubmitTexturedQuadFromVertices(s32 i0, f32 x0, f32 y0, f32 z0, s32 i1, f32 x1, f32 y1, f32 z1, s32 i2, f32 x2, f32 y2, f32 z2, s32 i3, f32 x3, f32 y3, f32 z3, u32 bits, f32 u0, f32 v0, f32 u1, f32 v1) {
    EvtQuadDesc desc;
    f32 verts[16];
    s32 indices[4];
    u64 strip[2];
    f32 uvs[8];
    void *asset;
    void *list;
    EvtDrawSurface *surface;

    asset = sdfCreateAssetWithDrawEntries();
    func_003332E8(asset, bits);
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    sdfConsAppendAssetPacket(list, asset, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.kind = 2;
    desc.count = 4;
    desc.verts = verts;
    desc.indices = indices;
    desc.strip = strip;
    desc.uvs = uvs;
    verts[0] = x0;
    verts[1] = y0;
    verts[2] = z0;
    verts[4] = x2;
    verts[5] = y2;
    verts[6] = z2;
    verts[8] = x3;
    verts[9] = y3;
    verts[10] = z3;
    verts[12] = x1;
    verts[13] = y1;
    verts[14] = z1;
    indices[0] = i0;
    indices[1] = i2;
    indices[2] = i3;
    indices[3] = i1;
    strip[0] = D_0037F5A0[0];
    uvs[0] = u0;
    uvs[1] = v0;
    uvs[2] = u0;
    uvs[3] = v1;
    uvs[4] = u1;
    uvs[5] = v1;
    uvs[6] = u1;
    uvs[7] = v0;
    sdfAppendPacket(list, func_0033B050(&desc));
    surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
    surface->submit(surface, list);
    sdfQueueAssetRelease(asset);
}

void evtSubmitViewParamPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    void *list;
    D_0037F5B0[0] = x;
    D_0037F5B0[1] = y;
    D_0037F5B0[2] = z;
    D_0037F5B0[4] = u;
    D_0037F5B0[5] = v;
    D_0037F5B0[6] = w;
    D_0037F5D0[1] = second;
    D_0037F5D0[0] = first;
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_00348158(D_0037F5B0, D_0037F5D0, 2, 0x80));
    {
        EvtDrawSurface *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->submit(surface, list);
    }
}

extern void sdfPktInit();
extern void *sdfFormatSifPacket();

void evtDrawPositionedSurfacePacket(s32 x, s32 y, s32 packetArg, s32 drawArg) {
    u8 pkt[16];
    void *list;
    void *packet;
    EvtDrawSurface *surface;
    list = (void *)sdfCreateResetPacketList();
    packet = sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket(packet);
    sdfAppendPacket(list, packet);
    sdfPktInit(pkt, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, packetArg);
    sdfAppendPacket(list, sdfFormatSifPacket(pkt, drawArg));
    surface = (EvtDrawSurface *)kwlnPositionedTextSurface;
    surface->submit(surface, list);
}

void evtPrepareSizedDrawResource(s32 width, s32 height, u64 first, u64 second) {
    u64 resource;

    resource = func_0019F448(width << 4, height << 3, 0, first, second, 0);
    frFontDrawGlyphInDefaultMode(resource);
    frFontQueueGlyphInSelectedSlot(resource);
}

s32 evtSelStateCreate(s32 limit, s16 frames, s32 value08, s32 value0C) {
    EvtSelState *node;
    if (frames == 0) {
        return 0;
    }
    if (evtSelectionStateActive != 0) {
        evtSelStateDestroy();
    }
    node = sdfAllocAndClearQuadwords(0x28);
    evtSelectionState = node;
    if (limit == 0) {
        node->limit = -1;
    } else {
        node->limit = limit;
    }
    ((EvtSelState *)evtSelectionState)->unk_08 = value08;
    ((EvtSelState *)evtSelectionState)->unk_0C = value0C;
    ((EvtSelState *)evtSelectionState)->unk_10 = frames;
    ((EvtSelState *)evtSelectionState)->unk_12 = frames;
    ((EvtSelState *)evtSelectionState)->unk_14 = frames;
    evtSelectionStateActive = 1;
    return 1;
}

u32 evtCheckSelectionState(void) {
    EvtSelState *sel;
    if (evtSelectionStateActive == 0) {
        return 0;
    }
    sel = evtSelectionState;
    if (sel->limit > sel->count || sel->limit == -1) {
        func_00109E60();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109E60);

void evtDestroySelectionState(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (evtSelectionStateActive != 0) {
        sdfReleaseChipBlock(evtSelectionState);
        evtSelectionStateActive = 0;
        VU0_MOVE_VF(vf10, vf0);
        VU0_CLEAR_W(vf10);
        VU0_STORE_VF(vf10, D_0037F590);
    }
}

s32 evtStartSelCreate(void) {
    return kwlnTaskCreate(D_00411370, 0x2AF9, 0, 0, (s32)func_00104150, 0, 0);
}

u32 evtStartSelDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00411370, 1);
    return 1;
}

u32 func_0010A350(void) {
    return 0;
}

void evtOpenFileMenuModeThree(void) {
    func_002CE208(3);
}

u32 evtPollFileMenuModeThree(void) {
    s64 operationResult;
    u32 status;

    func_002CE750();
    fileReleaseMenuResources();
    operationResult = func_002CE920();
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

void evtOpenTitleMenuWithOptionalValue(u32 unused, u32 *value) {
    if (value == 0) {
        func_002A3A50(0);
        return;
    }
    func_002A3A50(*value);
}

u32 evtCloseTitleMenu(void) {
    mnuDestroyTitleMenuTask();
    return 0;
}

u32 func_0010A418(void) {
    return 0;
}

void evtOpenFileMenuModeOne(void) {
    func_002CE208(1);
}

s32 evtCheckSelectionInput(void) {
    func_002CE750();
    fileReleaseMenuResources();
    if (func_002CE920() != 0) {
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

u32 func_0010A4D0(void) {
    return 0;
}

void evtOpenFileMenuModeTwo(void) {
    func_002CE208(2);
}

u32 evtPollFileMenuModeTwo(void) {
    s64 operationResult;
    u32 status;

    func_002CE750();
    fileReleaseMenuResources();
    operationResult = func_002CE920();
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
    func_001283B8(area, selection);
}

u32 evtPollSelectedAreaReady(void) {
    s64 operationResult;
    u32 status;

    fldShutdownSceneTasksAndWorld();
    operationResult = fldIsFieldResourceWaitFinished();
    status = 0xffffffff;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u32 func_0010A5B8(void) {
    return 0;
}

void evtStartSelectionFadeOut(void) {
    kwlnFadeBackgroundStartOut(0);
}

u32 func_0010A5D8(void) {
    return 0;
}

u32 func_0010A5E0(void) {
    return 0;
}

/* Source zero takes an explicit value; source one uses the pending event mode. */
void evtDispatchSelectionValue(s32 source, s32 *params) {
    s32 selection = 0;

    D_00435BB0 = 0;
    switch (source) {
    case 0:
        selection = params[0];
        break;
    case 1:
        selection = evtPendingEventSelection;
        break;
    }
    if (selection <= 0) {
        return;
    }
    evtCreateSkyTask();
    evtCreateEventScriptProcess(selection);
}

u32 evtCloseSkyEventTask(void) {
    D_00435BB0 = 1;
    evtDestroySkyTask();
    fldDispatchDeferredFieldCommand();
    return 0;
}

u32 func_0010A680(void) {
    return 0;
}

void func_0010A688(void) {
    mnuCreateCampTasks();
}

u32 evtDestroyCampTasks(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 evtWaitCampStateAcknowledged(void) {
    s64 state;

    state = mnuAcknowledgeCampState();
    return state == 0;
}

void func_0010A6E0(void) {
}

u32 func_0010A6E8(void) {
    return 0;
}

u32 func_0010A6F0(void) {
    return 1;
}

void func_0010A6F8(void) {
}

u32 func_0010A700(void) {
    return 0;
}

u32 func_0010A708(void) {
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
    s64 done;

    done = brsTaskConsumeDone();
    return done == 0;
}

void evtCreateConfigTasks(void) {
    mnuCreateConfigTasks(1);
}

u32 evtDestroyConfigTasks(void) {
    mnuConfigTasksDestroy();
    return 0;
}

u8 evtWaitConfigTaskDone(void) {
    s64 result;

    result = fileConsumeConfigTaskReady();
    return result == 0;
}

void evtDispatchSelectionCommand(s32 source, s32 *params) {
    if (source == 0) {
        if (params == NULL) {
            func_001A9F30(1, 0, 0);
        }
        else {
            func_001A9F30(0, params[0], params[1]);
        }
    }
    else {
        dds3AdminSetControlFlag();
        fldDispatchDeferredFieldCommand();
    }
}

void func_0010A820(void) {
    btlExitWhenAudioAndTasksIdle();
}

u32 evtEnsureSelectionTask(void) {
    if (func_00101740(D_00435D20) != NULL) {
        return 0;
    }
    if (func_00101820(0x3FA) == NULL) {
        fldDispatchDeferredFieldCommand();
        return 1;
    }
    return 0;
}

void evtCreateBattleStageTestTask(void) {
    btlCreateStageTestTask();
}

u32 func_0010A8A0(void) {
    evtBattleStageTestStopTask();
    return 0;
}

u32 func_0010A8C0(void) {
    return 0;
}

void evtCreateTestAndSkyTasks(void) {
    D_00435BB0 = 0;
    D_00435D28 = 0;
    evtStartTestTask();
    evtCreateSkyTask();
}

u32 evtInitializeSelectionScene(void) {
    D_00435BB0 = 1;
    kwlnFadeBackgroundStartOut(0);
    evtStopTestTasks();
    evtDestroySkyTask();
    return 1;
}

u32 func_0010A920(void) {
    return 0;
}

void evtClearSecondaryWorldAndViewer(void) {
    D_00435BB0 = 0;
    evtDestroySecondaryWorldNode();
    evtViewerCreateTaskWithSky();
}

u32 evtInitializeSelectionScreen(void) {
    evtEventViewerDestroyTask();
    D_00435BB0 = 1;
    kwlnFadeBackgroundStartOut(0);
    return 0;
}

u32 func_0010A978(void) {
    return 0;
}

void func_0010A980(void) {
}

u32 func_0010A988(void) {
    return 0;
}

u32 func_0010A990(void) {
    return 0;
}

void func_0010A998(void) {
    mnuCreateCampTasks();
}

u32 evtExitCampTaskGroup(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 mnuCheckCampStateAndAcknowledge(void) {
    s64 state;

    state = mnuAcknowledgeCampState();
    return state == 0;
}

void evtCreateSkyAndCampTasks(u32 unused, u32 campMode) {
    evtCreateSkyTask();
    func_00260708(campMode);
}

u32 evtDestroySkyAndCampTasks(void) {
    evtDestroySkyTask();
    mnuCampDestroyPanelTasks();
    fldProcessDeferredSceneCommand();
    return 0;
}

u8 evtWaitSkyCampTaskState(void) {
    s64 result;

    result = mnuCampConsumePanelTaskCompletion();
    return result == 0;
}

void evtCreateSkyAndFieldTasks(u32 unused, u32 *fieldArgs) {
    evtCreateSkyTask();
    if (fieldArgs != 0) {
        func_00268380(fieldArgs[0], fieldArgs[1]);
        return;
    }
    func_00268380(0, 1);
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
    s64 done;

    done = brsTaskConsumeDone();
    return done == 0;
}

void func_0010AB50(void) {
}

u32 func_0010AB58(void) {
    return 0;
}

u32 func_0010AB60(void) {
    return 0;
}

void func_0010AB68(void) {
}

u32 func_0010AB70(void) {
    return 0;
}

u32 func_0010AB78(void) {
    return 0;
}

void evtLaunchFontTestScene(void) {
    itfStartFontTestScene();
}

u32 func_0010AB98(void) {
    return 0;
}

u32 evtTestFontCheck(void) {
    return func_00101740(D_00411380) == NULL;
}

void evtCreateMovieViewerTask(void) {
    mnuCreateMovieViewerTask();
}

u32 evtDestroyMovieViewerTask(void) {
    mnuDestroyMovieViewerTask();
    return 0;
}

u32 func_0010AC00(void) {
    return 0;
}

void func_0010AC08(void) {
    func_003091E8();
}

u32 func_0010AC20(void) {
    func_003091F0();
    return 0;
}

u32 func_0010AC40(void) {
    return 0;
}

void evtSelectFontResource(s32 unused, s32 *lmapArgs) {
    if (lmapArgs == NULL) {
        fldStartLmapTask(0);
    }
    else {
        fldStartLmapTask(lmapArgs[0]);
    }
    D_00389780[0] = 0x3E7;
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

void func_0010ACD0(void) {
    mdlCreateViewerPackageTask();
}

u32 func_0010ACE8(void) {
    func_00316EF8();
    return 0;
}

u8 func_0010AD08(void) {
    s64 result;

    result = func_00316ED0();
    return result == 0;
}

void evtOpenFileMenuFromParams(u32 unused, u32 *menuArgs) {
    u32 menuArg;

    menuArg = 0;
    if (menuArgs != 0) {
        menuArg = *menuArgs;
    }
    fileMenuWorkCreate(menuArg);
    func_002CE208(4);
}

extern void fileReleaseMenuFlowResource();

s32 evtPollFileMenuAndRelease(void) {
    func_002CE750();
    fileReleaseMenuResources();
    if (func_002CE920() == 0) {
        return -1;
    }
    fileReleaseMenuFlowResource();
    return 0;
}

u8 evtWaitFileMenuTaskFour(void) {
    s64 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

u32 evtEnsureFontResourceChain(void) {
    if (evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsNodeCreate(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetControlByte(evtConsoleFontResourceChain, 2);
        sdfDevConsSetTextAttribute(evtConsoleFontResourceChain, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (evtConsoleFontResourceChain != 0) {
        sdfDevConsNodeDestroy(evtConsoleFontResourceChain);
        evtConsoleFontResourceChain = 0;
    }
}

extern s32 func_00360E78();
extern void sdfDevConsPrintf();

/* printf into the dev console node when one exists. */
INCLUDE_SDATA(const s32, "game/code_00107EF8", evtSelectionStateActive);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D20);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D28);

INCLUDE_SDATA(const s32, "game/code_00107EF8", evtConsoleFontResourceChain);

void evtPrintDeveloperConsoleMessage(const char *fmt, ...) {
    char buffer[0x200];
    __builtin_va_list args;

    __builtin_stdarg_start(args, fmt);
    if (evtConsoleFontResourceChain != 0) {
        func_00360E78(buffer, fmt, args);
        sdfDevConsPrintf(evtConsoleFontResourceChain, "%s", buffer);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010AEC0);

void func_0010B110(void) {
}

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_00411370);

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_00411380);

void evtToggleDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_00438E64 = kwlnTaskCreate("DebugTimeGrph", 0x2710, 1, 1, func_0010AEC0, func_0010B110, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_00438E64, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B190);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B3D8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B650);

s32 func_0010B780(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    func_0010B650(kwlnPositionedTextSurface);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B7B8);

void func_0010B8D0(void) {
}

extern void func_0010B7B8(void);
extern void func_0010B8D0(void);
extern s32 D_00438E68;
extern char D_004113B8[]; /* "DebugTimeGrph" */

void func_0010B8D8(s8 mode) {
    if (mode == 1) {
        D_00438E68 = kwlnTaskCreate(D_004113B8, 0x2710, 1, 1, func_0010B7B8, func_0010B8D0, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy((void *)D_00438E68, 0);
    }
}

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void evtLinkWorkNode(B728Work *node) {
    B728Work *tail = scrNamedProcessTail;

    if (tail == NULL) {
        scrNamedProcessHead = node;
        scrNamedProcessTail = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->previous = tail;
        tail->next = node;
        node->next = NULL;
        scrNamedProcessTail = node;
    }
    scrNamedProcessCount++;
}

/* Detach from either end or the middle, and clear the old links. */
void evtUnlinkWorkNode(B728Work *node) {
    if ((B728Work *)scrNamedProcessHead == node) {
        scrNamedProcessHead = node->next;
    }
    else {
        ((B728Work *)node->previous)->next = node->next;
    }
    if ((B728Work *)scrNamedProcessTail == node) {
        scrNamedProcessTail = node->previous;
    }
    else {
        ((B728Work *)node->next)->previous = node->previous;
    }
    node->previous = NULL;
    node->next = NULL;
    scrNamedProcessCount--;
}

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_004113B8);

INCLUDE_ASM(const s32, "game/code_00107EF8", bfContextCreate);

INCLUDE_ASM(const s32, "game/code_00107EF8", bfParseFLW0);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D40);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D48);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D4C);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D50);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D58);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrNamedProcessCount);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrNamedProcessHead);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrNamedProcessTail);

