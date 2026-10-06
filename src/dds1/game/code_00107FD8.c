#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "common.h"
#include "sdf_primitive.h"

#include "kwln.h"
#include "sdf.h"
#include "scr.h"

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46,
    BF_FLW0_SECTION_PROCEDURES = 0,
    BF_FLW0_SECTION_LABELS = 1,
    BF_FLW0_SECTION_INSTRUCTIONS = 2,
    BF_FLW0_SECTION_AUXILIARY = 3,
    BF_FLW0_SECTION_STRINGS = 4
};

extern s32 evtConsoleFontResourceChain;

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

extern u32 kwlnDrawSurfaceIndex;

extern u32 kwlnDrawControlFlags;

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

extern s8 evtSelectionStateActive;

extern void *evtSelectionState;

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

extern s32 evtPendingEventSelection;

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

extern DrawVec4 kwlnDrawVector;

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


extern u32 scrNamedProcessCount;


extern SdfPoolNode kwlnPositionedTextSurface;

extern s8 D_0032453B[];

extern void evtDrawHeapUsageOverlay(SdfPoolNode *);

extern void kwlnTaskDestroyWithHierarchy(void *, s32);

extern void func_0010AC98(void);

extern void *D_003BD764;

extern void evtSubmitGradientRectAtDepth();

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

/* Set an existing light slot's RGB target immediately or save packed blend endpoints. */
INCLUDE_ASM(const s32, "game/code_00107FD8", kwlnSetLightColorTarget);

/* Normalize the requested light direction; replace it immediately or prepare its blend. */
INCLUDE_ASM(const s32, "game/code_00107FD8", kwlnSetLightDirectionTarget);

extern u16 D_003BD6E6;
extern u16 D_003BD6E8;
extern u32 D_003BD6EC;
extern u32 D_003BD6F0;
extern u8 kwlnDefaultColorVector[];

/* vu0 routine: replace the background color immediately or blend over blendFrames frames. */
void kwlnSetBackgroundColorTarget(s32 blendFrames, f32 *color) {
    s32 startColorWords[4];
    s32 targetColorWords[4];
    u32 packedStartColor;
    u32 packedTargetColor;

    if (blendFrames == 0) {
        kwlnDrawControlFlags &= ~EVT_BACKGROUND_BLEND_FLAG;
        VU0_LOAD_VF(vf10, color);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, kwlnDefaultColorVector);
    } else {
        kwlnDrawControlFlags |= EVT_BACKGROUND_BLEND_FLAG;
        D_003BD6E8 = blendFrames;
        D_003BD6E6 = 0;
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packedStartColor);
        startColorWords[0] = packedStartColor;
        D_003BD6EC = startColorWords[0];
        VU0_LOAD_VF(vf10, color);
        VU0_SET_W_ONE(vf10);
        EE_MMI_RGBA_PACK(packedTargetColor);
        targetColorWords[0] = packedTargetColor;
        D_003BD6F0 = targetColorWords[0];
    }
}

extern u32 D_003247A0[];
extern u16 D_003BD6F4;
extern u16 D_003BD6F6;
extern u32 D_003BD6FC;
extern u32 D_003BD700;

/* vu0 routine: set the draw colour target: immediately (mode 0) or interpolate from the previous colour. */
void kwlnSetDrawColorTarget(s32 blendFrames, f32 *color) {
    u32 targetColorWords[4];
    u32 packedTargetColor;

    VU0_LOAD_VF(vf10, color);
    VU0_CLEAR_W(vf10);
    EE_MMI_RGBA_PACK_F255(packedTargetColor);
    targetColorWords[0] = packedTargetColor;
    if (blendFrames == 0) {
        kwlnDrawControlFlags &= ~EVT_DRAW_COLOR_BLEND_FLAG;
        D_003247A0[0] = targetColorWords[0];
    } else {
        kwlnDrawControlFlags |= EVT_DRAW_COLOR_BLEND_FLAG;
        D_003BD6F6 = blendFrames;
        D_003BD700 = targetColorWords[0];
        D_003BD6FC = D_003247A0[0];
        D_003BD6F4 = 0;
    }
}


/* Either replace the draw vector immediately or interpolate from its prior value. */
void evtSetDrawVectorTarget(s32 blendFrames, f32 x, f32 y, f32 z, f32 w) {
    if (blendFrames == 0) {
        kwlnDrawControlFlags &= ~EVT_DRAW_VECTOR_BLEND_FLAG;
        kwlnDrawVector.x = x;
        kwlnDrawVector.y = y;
        kwlnDrawVector.z = z;
        kwlnDrawVector.w = w;
    } else {
        f32 previousX = kwlnDrawVector.x;
        f32 previousY = kwlnDrawVector.y;
        f32 previousZ = kwlnDrawVector.z;
        f32 previousW = kwlnDrawVector.w;
        kwlnDrawControlFlags |= EVT_DRAW_VECTOR_BLEND_FLAG;
        D_003BD6FA = blendFrames;
        D_003C2C20.x = previousX;
        D_003C2C20.y = previousY;
        D_003C2C20.z = previousZ;
        D_003C2C20.w = previousW;
        D_003C2C30.x = x;
        D_003C2C30.y = y;
        D_003C2C30.z = z;
        D_003C2C30.w = w;
        D_003BD6F8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001083F8);

/* Replace the two scalar targets immediately, or retain start/end values for their blend. */
void evtToggleSavedDrawVectors(s32 frames, f32 firstTarget, f32 secondTarget) {
    if (frames == 0) {
        kwlnDrawControlFlags &= ~EVT_SAVED_VECTOR_BLEND_FLAG;
        D_003BD358 = firstTarget;
        D_003BD35C = secondTarget;
    }
    else {
        f32 previousFirst = D_003BD358;
        f32 previousSecond = D_003BD35C;
        kwlnDrawControlFlags |= EVT_SAVED_VECTOR_BLEND_FLAG;
        D_003BD706 = frames;
        D_003BD708 = previousFirst;
        D_003BD70C = firstTarget;
        D_003BD710 = previousSecond;
        D_003BD714 = secondTarget;
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
    kwlnDrawSurfaceIndex = surfaceIndex;
}


/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

extern SdfPoolNode kwlnDrawSurfaces[];

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, void *);

extern u8 *sdfConsFinalizePacketHeader(void *, s32);

extern s32 sdfCreateResetPacketList(void);

extern void sdfBuildPrimaryAlphaBlendDmaPacket(void *);

extern void sdfPktInit(void *, s32, s32, s32, s32);

extern void *sdfFormatSifPacket();

/* Submit primary-context GS TEST settings. Z testing is always enabled;
   unusedZte is a retained native formal, not the source of the ZTE bit. */
void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, EVT_GS_COMMAND_BYTES);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = EVT_GS_TEST_PRIMARY;
    sdfAppendPacket(list, packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

/* Submit the same TEST bit layout for the secondary GS context, with ZTE forced on. */
void evtSubmitSecondaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, EVT_GS_COMMAND_BYTES);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = EVT_GS_TEST_SECONDARY;
    sdfAppendPacket(list, packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

/* Select a primary-context ALPHA equation by mode; unknown modes use 0x44. */
void evtSubmitPrimaryAlphaBlendMode(s32 blendMode) {
    void *list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;

    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, EVT_GS_COMMAND_BYTES);
    switch (blendMode) {
    case 1:
        command->data = 0x48;
        break;
    case 2:
        command->data = 0x42;
        break;
    case 3:
        command->data = 0x84;
        break;
    case 4:
        command->data = 0x06;
        break;
    case 5:
        command->data = 0x89;
        break;
    case 6:
        command->data = 0x42;
        break;
    case 10:
        command->data = 0x2A;
        break;
    case 20:
        command->data = 0x4A;
        break;
    default:
        command->data = 0x44;
        break;
    }
    command->registerId = EVT_GS_ALPHA_PRIMARY;
    sdfAppendPacket(list, packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha) {
    void *list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader(packet, EVT_GS_COMMAND_BYTES);
    command->data = ((u64)fixedAlpha << 32) | EVT_FIXED_ALPHA_INTERPOLATION;
    command->registerId = EVT_GS_ALPHA_PRIMARY;
    sdfAppendPacket(list, packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

extern s32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_002D4C80(const void *, void *, s32);

extern void sdfAppendDmaTagToList(void *, void *);

void func_00108E60(void) {
    void *list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *framePacket;
    sdfInitPacketList(list);
    framePacket = sdfAllocPacketAligned(EVT_FRAME_REFERENCE_PACKET_BYTES);
    func_002D4C80(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * EVT_FRAME_DRAW_RECORD_BYTES, framePacket, 0);
    sdfAppendDmaTagToList(list, framePacket);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

extern void func_002D4CC8(const void *, void *, s32);

void func_00108F00(void) {
    void *list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *framePacket;
    sdfInitPacketList(list);
    framePacket = sdfAllocPacketAligned(EVT_FRAME_REFERENCE_PACKET_BYTES);
    func_002D4CC8(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * EVT_FRAME_DRAW_RECORD_BYTES, framePacket, 0);
    sdfAppendDmaTagToList(list, framePacket);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108FA0);

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
INCLUDE_ASM(const s32, "game/code_00107FD8", evtSubmitGradientRectAtDepth);

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor) {
    evtSubmitGradientRectAtDepth(x, y, width, height, EVT_QUAD_DEFAULT_DEPTH, topLeftColor, topRightColor, bottomRightColor, bottomLeftColor);
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001093F8);



extern u64 D_003245A0[]; /* index table; only the first 8 bytes are used */
extern void sdfConsAppendClearPacket(void *, s32);
extern void sdfConsAppendAssetPacket(void *, void *, s32);
extern void *func_002E21A0(SdfPrimitiveRequest *);
extern void func_002DA438(void *, u32);
extern void sdfQueueAssetRelease(void *);

/* Preserve the native 0,2,3,1 vertex/index order; W components are not initialized here. */
void evtSubmitQuadFromVertices(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 i0, u32 i1, u32 i2, u32 i3) {
    SdfPrimitiveRequest desc;
    DrawVec4 verts[EVT_QUAD_VERTEX_COUNT];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    void *list;
    SdfPoolNode *surface;

    list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    sdfConsAppendAssetPacket(list, D_003BD6B0, 0);
    memset(&desc, 0, EVT_QUAD_DESCRIPTOR_BYTES);
    desc.color = 0x80808080;
    desc.stripWordCount = 2;
    desc.vertexCount = EVT_QUAD_VERTEX_COUNT;
    desc.positions = &verts[0].x;
    desc.vertexColors = indices;
    desc.strip = strip;
    verts[0].x = x0;
    verts[0].y = y0;
    verts[0].z = z0;
    verts[1].x = x2;
    verts[1].y = y2;
    verts[1].z = z2;
    verts[2].x = x3;
    verts[2].y = y3;
    verts[2].z = z3;
    verts[3].x = x1;
    verts[3].y = y1;
    verts[3].z = z1;
    indices[0] = i0;
    indices[1] = i2;
    indices[2] = i3;
    indices[3] = i1;
    strip[0] = D_003245A0[0];
    sdfAppendPacket(list, func_002E21A0(&desc));
    surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
    surface->append((SdfListHead *)surface, list);
}

/* Submit the same native vertex permutation with rectangular UVs, then queue asset release. */
void evtSubmitTexturedQuadFromVertices(s32 i0, f32 x0, f32 y0, f32 z0, s32 i1, f32 x1, f32 y1, f32 z1, s32 i2, f32 x2, f32 y2, f32 z2, s32 i3, f32 x3, f32 y3, f32 z3, u32 bits, f32 u0, f32 v0, f32 u1, f32 v1) {
    SdfPrimitiveRequest desc;
    f32 verts[16];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    f32 uvs[8];
    void *asset;
    void *list;
    SdfPoolNode *surface;

    asset = sdfCreateAssetWithDrawEntries();
    func_002DA438(asset, bits);
    list = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    sdfConsAppendAssetPacket(list, asset, 0);
    memset(&desc, 0, EVT_QUAD_DESCRIPTOR_BYTES);
    desc.color = 0x80808080;
    desc.stripWordCount = 2;
    desc.vertexCount = EVT_QUAD_VERTEX_COUNT;
    desc.positions = verts;
    desc.vertexColors = indices;
    desc.strip = strip;
    desc.coordinates = uvs;
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
    strip[0] = D_003245A0[0];
    uvs[0] = u0;
    uvs[1] = v0;
    uvs[2] = u0;
    uvs[3] = v1;
    uvs[4] = u1;
    uvs[5] = v1;
    uvs[6] = u1;
    uvs[7] = v0;
    sdfAppendPacket(list, func_002E21A0(&desc));
    surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
    surface->append((SdfListHead *)surface, list);
    sdfQueueAssetRelease(asset);
}

extern f32 D_003245B0[][4];

extern u32 D_003245D0[];

extern void *func_002EF2B0(const f32 (*)[4], const u32 *, s32, u32);

void evtSubmitViewParamPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    void *list;
    D_003245B0[0][0] = x;
    D_003245B0[0][1] = y;
    D_003245B0[0][2] = z;
    D_003245B0[1][0] = u;
    D_003245B0[1][1] = v;
    D_003245B0[1][2] = w;
    D_003245D0[1] = second;
    D_003245D0[0] = first;
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_002EF2B0(D_003245B0, D_003245D0, 2, 0x80));
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

void evtDrawPositionedSurfacePacket(s32 x, s32 y, s32 packetArg, s32 drawArg) {
    u8 sifParameters[16];
    void *list;
    void *packet;
    SdfPoolNode *surface;
    list = (void *)sdfCreateResetPacketList();
    packet = sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket(packet);
    sdfAppendPacket(list, packet);
    sdfPktInit(sifParameters, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, packetArg);
    sdfAppendPacket(list, sdfFormatSifPacket(sifParameters, drawArg));
    surface = &kwlnPositionedTextSurface;
    surface->append((SdfListHead *)surface, list);
}

void evtPrepareSizedDrawResource(s32 width, s32 height, u64 first, u64 second) {
    u64 resource;

    resource = func_00197748(width << 4, height << 3, 0, first, second, 0);
    frFontDrawGlyphInDefaultMode(resource);
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

/* Zero frames leaves the prior state untouched; zero countLimit selects the -1 sentinel. */
s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue) {
    EvtSelState *selectionState;
    if (frames == 0) {
        return 0;
    }
    if (evtSelectionStateActive != 0) {
        evtSelStateDestroy();
    }
    selectionState = sdfAllocAndClearQuadwords(EVT_SELECTION_ALLOCATION_BYTES);
    evtSelectionState = selectionState;
    if (countLimit == 0) {
        selectionState->limit = -1;
    } else {
        selectionState->limit = countLimit;
    }
    ((EvtSelState *)evtSelectionState)->unk_08 = firstValue;
    ((EvtSelState *)evtSelectionState)->unk_0C = secondValue;
    ((EvtSelState *)evtSelectionState)->unk_10 = frames;
    ((EvtSelState *)evtSelectionState)->unk_12 = frames;
    ((EvtSelState *)evtSelectionState)->unk_14 = frames;
    evtSelectionStateActive = 1;
    return 1;
}

/* Return zero only when inactive on entry; updating or destroying active state returns one. */
u32 evtCheckSelectionState(void) {
    EvtSelState *selectionState;
    if (evtSelectionStateActive == 0) {
        return 0;
    }
    selectionState = evtSelectionState;
    if (selectionState->limit > selectionState->count || selectionState->limit == -1) {
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
    if (evtSelectionStateActive != 0) {
        sdfReleaseChipBlock(evtSelectionState);
        evtSelectionStateActive = 0;
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

void evtOpenTitleMenuWithOptionalValue(u32 unused, u32 *value) {
    if (value == 0) {
        func_0026BCE8(0);
        return;
    }
    func_0026BCE8(*value);
}

u32 evtCloseTitleMenu(void) {
    mnuDestroyTitleMenuTask();
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
void evtDispatchSelectionValue(s32 valueSource, s32 *selectionParams) {
    s32 eventSelection = 0;

    D_003BA730 = 0;
    switch (valueSource) {
    case 0:
        eventSelection = selectionParams[0];
        break;
    case 1:
        eventSelection = evtPendingEventSelection;
        break;
    }
    if (eventSelection <= 0) {
        return;
    }
    evtCreateSkyTask();
    evtCreateEventScriptProcess(eventSelection);
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

/* Source zero forwards an optional parameter pair; other sources resume deferred field work. */
void evtDispatchSelectionCommand(s32 commandSource, s32 *commandParams) {
    if (commandSource == 0) {
        if (commandParams == NULL) {
            func_001A11F0(1, 0, 0);
        }
        else {
            func_001A11F0(0, commandParams[0], commandParams[1]);
        }
    }
    else {
        dds3AdminSetControlFlag();
        fldDispatchDeferredFieldCommand();
    }
}

/* Check battle shutdown; the callee exits only after audio and task work become idle. */
void evtCheckBattleShutdown(void) {
    btlExitWhenAudioAndTasksIdle();
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

/* Stop the battle-stage test task and return the scene callback's native zero status. */
u32 evtEndBattleStageTest(void) {
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

u32 evtExitCampTaskGroup(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 mnuCheckCampStateAndAcknowledge(void) {
    s64 campState;

    campState = mnuAcknowledgeCampState();
    return campState == 0;
}

void evtCreateSkyAndCampTasks(u32 unused, u32 campMode) {
    evtCreateSkyTask();
    mnuOpenShopSceneWithInitialSelection(campMode);
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
        mnuTerminalCreateTasks(fieldArgs[0], fieldArgs[1]);
        return;
    }
    mnuTerminalCreateTasks(0, 1);
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
    itfStartFontTestScene();
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

extern s32 func_00305B08();
extern void sdfDevConsPrintf();

/* printf into the dev console node when one exists. */
INCLUDE_SDATA(const s32, "game/code_00107FD8", evtSelectionStateActive);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA950);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA958);

INCLUDE_SDATA(const s32, "game/code_00107FD8", evtConsoleFontResourceChain);

void evtPrintDeveloperConsoleMessage(const char *format, ...) {
    char messageBuffer[EVT_DEV_CONSOLE_BUFFER_BYTES];
    __builtin_va_list formatArgs;

    __builtin_stdarg_start(formatArgs, format);
    if (evtConsoleFontResourceChain != 0) {
        func_00305B08(messageBuffer, format, formatArgs);
        sdfDevConsPrintf(evtConsoleFontResourceChain, "%s", messageBuffer);
    }
}

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

extern u32 D_003BA97C;
extern void sdfAppendFillRectanglePacket(SdfListHead *, s32, s32, s32, s32,
                                         s32, s32, s32, s32 (*)(s32));

/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(SdfListHead *packetList, s32 x, s32 y,
                   u32 heapBase, u32 address, s32 remainingBytes, u8 kind) {
    s32 color;
    u32 byteOffset;
    s32 rowIndex;
    s32 columnIndex;
    s32 leftX;
    s32 topY;
    u32 bytesPerRow;
    u32 bytesPerCell;

    switch (kind) {
    case 0:
        color = 0x80000000;
        break;
    case 1:
        color = 0x80008080;
        break;
    case 2:
        color = 0x80000080;
        break;
    default:
        color = 0x80800000;
        break;
    }
    byteOffset = address - heapBase;
    bytesPerRow = D_003BA97C >> 8;
    bytesPerCell = D_003BA97C >> 15;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_003BA97C < remainingBytes || remainingBytes < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 128 - columnIndex;

        if (remainingBytes >= remainingCells / bytesPerCell) {
            sdfAppendFillRectanglePacket(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            remainingBytes -= remainingCells / (D_003BA97C >> 15);
        } else {
            s32 cellCount = remainingBytes / bytesPerCell;

            if (cellCount == 0 && remainingBytes > 0) {
                cellCount = 1;
            }
            sdfAppendFillRectanglePacket(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            remainingBytes = 0;
        }
        topY += 8;
    }
    while (remainingBytes >= (D_003BA97C >> 8)) {
        sdfAppendFillRectanglePacket(packetList, color, 0, x, topY,
                                     x + 0x800, topY + 8, 0x0FFFFF80, NULL);
        remainingBytes -= D_003BA97C >> 8;
        topY += 8;
    }
    if (remainingBytes > 0) {
        sdfAppendFillRectanglePacket(packetList, color, 0, x, topY,
                                     x + (remainingBytes / (D_003BA97C >> 15)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B1B0);

typedef struct SdfChipStats {
    u32 totalBytes;
    u32 freeBytes;
    u32 blockCount;
    u32 emptyBlocks;
    u32 partialBlocks;
    u32 usedCells[7];
} SdfChipStats;

extern void sdfGetGeneralHeapStats(s32 *);
extern void sdfGetChipHeapStats(SdfChipStats *);
extern void func_0010B1B0(void *, s32, s32);
extern void func_003014F0(char *, const char *, ...);
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);
extern char D_003BA980[];
extern char D_003BA988[];

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(SdfPoolNode *surface) {
    s32 generalHeapStats[6];
    SdfChipStats chipHeapStats;
    char statusText[100];
    void *packetList;

    sdfGetGeneralHeapStats(generalHeapStats);
    D_003BA97C = generalHeapStats[0];
    sdfGetChipHeapStats(&chipHeapStats);
    packetList = sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    sdfInitPacketList(packetList);
    func_0010B1B0(packetList, 0x86C0, 0x79C0);
    func_003014F0(statusText, D_003BA980, generalHeapStats[1]);
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x86C0,
        (D_003BA97C / (D_003BA97C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    func_003014F0(statusText, D_003BA988, chipHeapStats.freeBytes);
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x86C0,
        (D_003BA97C / (D_003BA97C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->append((SdfListHead *)surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (D_0032453B[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&kwlnPositionedTextSurface);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B590);

void func_0010B6A8(void) {
}

void evtToggleAlternateDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_003BD768 = kwlnTaskCreate(D_0039E238, 0x2710, 1, 1, func_0010B590, func_0010B6A8, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_003BD768, 0);
    }
}

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void evtLinkWorkNode(ScrData *node) {
    ScrData *tail = scrNamedProcessTail;

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
void evtUnlinkWorkNode(ScrData *node) {
    if (scrNamedProcessHead == node) {
        scrNamedProcessHead = node->next;
    }
    else {
        node->previous->next = node->next;
    }
    if (scrNamedProcessTail == node) {
        scrNamedProcessTail = node->previous;
    }
    else {
        node->next->previous = node->previous;
    }
    node->previous = NULL;
    node->next = NULL;
    scrNamedProcessCount--;
}

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E238);

INCLUDE_ASM(const s32, "game/code_00107FD8", bfContextCreate);

typedef struct BfFlw0Header {
    u8 pad00[8];
    u32 magic;
    u8 pad0C[4];
    s32 sectionCount;
    u8 pad14[0xC];
    ScrSection sections[1];
} BfFlw0Header;


/* Resolve supported FLW0 sections relative to the header; reject bad magic/unknown kinds.
   Zero-count auxiliary sections are ignored, and absent sections remain null. */
ScrData *bfParseFLW0(BfFlw0Header *header, s32 procedureIndex) {
    ScrSection *sections;
    ScrLabel *procedures = NULL;
    ScrLabel *labels = NULL;
    ScrInstr *instructions = NULL;
    void *auxiliaryData = NULL;
    char *strings = NULL;
    s32 i;

    sections = header->sections;
    if (header->magic != BF_FLW0_MAGIC) {
        return 0;
    }
    for (i = 0; i < header->sectionCount; i++) {
        switch (sections[i].type) {
            case BF_FLW0_SECTION_PROCEDURES:
                procedures = (ScrLabel *)((u8 *)header + sections[i].offset);
                break;
            case BF_FLW0_SECTION_LABELS:
                labels = (ScrLabel *)((u8 *)header + sections[i].offset);
                break;
            case BF_FLW0_SECTION_INSTRUCTIONS:
                instructions = (ScrInstr *)((u8 *)header + sections[i].offset);
                break;
            case BF_FLW0_SECTION_AUXILIARY:
                if (sections[i].count != 0) {
                    auxiliaryData = (u8 *)header + sections[i].offset;
                }
                break;
            case BF_FLW0_SECTION_STRINGS:
                strings = (char *)header + sections[i].offset;
                break;
            default:
                return 0;
        }
    }
    return bfContextCreate(header, sections, procedures, labels, instructions,
                           auxiliaryData, strings, procedureIndex);
}

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA970);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA978);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA97C);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA980);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA988);

INCLUDE_SDATA(const s32, "game/code_00107FD8", scrNamedProcessCount);

INCLUDE_SDATA(const s32, "game/code_00107FD8", scrNamedProcessHead);

INCLUDE_SDATA(const s32, "game/code_00107FD8", scrNamedProcessTail);

