#include "common.h"
#include "sdf_primitive.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf.h"
#include "sdf_sif_command.h"
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


extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

extern KwlnTask *func_00101820(u32 prio);

extern char D_00435D20[];

extern void evtEventViewerDestroyTask(void);

extern char D_00411380[];

extern s32 D_00389780[];

extern void fldStartLmapTask(s32 arg0);


extern u32 scrNamedProcessCount;


extern SdfPoolNode kwlnPositionedTextSurface;

extern s8 D_0037F53B[];

extern void evtDrawHeapUsageOverlay(SdfPoolNode *);

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


/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

extern SdfPoolNode kwlnDrawSurfaces[];

typedef struct SdfDrawPacket SdfDrawPacket;

extern s32 sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(SdfListHead *);

extern void sdfAppendPacket(SdfListHead *, u32);

extern u32 sdfConsFinalizePacketHeader(u32, s32);

extern s32 sdfConsCalculateDrawPacketSize(s32, s32);

extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);

extern s32 sdfConsMeasurePacketWithHeader(s32);

extern u32 kwlnGetDrawBufferIndex(void);

extern u8 kwlnFrameDrawPacketRecords[];

extern void func_0032DB30(const void *, void *, s32);

extern void sdfAppendDmaTagToList(void *, void *);

extern void func_0032DB78(const void *, void *, s32);

extern f32 D_0037F5B0[][4];

extern u32 D_0037F5D0[];

extern void *func_00348158(const f32 (*)[4], const u32 *, s32, u32);

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

/* Set an existing light slot's RGB target immediately or save packed blend endpoints. */
INCLUDE_ASM(const s32, "game/code_00107EF8", kwlnSetLightColorTarget);

/* Normalize the requested light direction; replace it immediately or prepare its blend. */
INCLUDE_ASM(const s32, "game/code_00107EF8", kwlnSetLightDirectionTarget);

extern u16 D_00438DE6;
extern u16 D_00438DE8;
extern u32 D_00438DEC;
extern u32 D_00438DF0;
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
        D_00438DE8 = blendFrames;
        D_00438DE6 = 0;
        VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packedStartColor);
        startColorWords[0] = packedStartColor;
        D_00438DEC = startColorWords[0];
        VU0_LOAD_VF(vf10, color);
        VU0_SET_W_ONE(vf10);
        EE_MMI_RGBA_PACK(packedTargetColor);
        targetColorWords[0] = packedTargetColor;
        D_00438DF0 = targetColorWords[0];
    }
}

extern u32 D_0037F7A0[];
extern u16 D_00438DF4;
extern u16 D_00438DF6;
extern u32 D_00438DFC;
extern u32 D_00438E00;

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
        D_0037F7A0[0] = targetColorWords[0];
    } else {
        kwlnDrawControlFlags |= EVT_DRAW_COLOR_BLEND_FLAG;
        D_00438DF6 = blendFrames;
        D_00438E00 = targetColorWords[0];
        D_00438DFC = D_0037F7A0[0];
        D_00438DF4 = 0;
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
        D_00438DFA = blendFrames;
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

/* Replace the two scalar targets immediately, or retain start/end values for their blend. */
void evtToggleSavedDrawVectors(s32 frames, f32 firstTarget, f32 secondTarget) {
    if (frames == 0) {
        kwlnDrawControlFlags &= ~EVT_SAVED_VECTOR_BLEND_FLAG;
        D_00438A48 = firstTarget;
        D_00438A4C = secondTarget;
    }
    else {
        f32 previousFirst = D_00438A48;
        f32 previousSecond = D_00438A4C;
        kwlnDrawControlFlags |= EVT_SAVED_VECTOR_BLEND_FLAG;
        D_00438E06 = frames;
        D_00438E08 = previousFirst;
        D_00438E0C = firstTarget;
        D_00438E10 = previousSecond;
        D_00438E14 = secondTarget;
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

/* Submit primary-context GS TEST settings. Z testing is always enabled;
   unusedZte is a retained native formal, not the source of the ZTE bit. */
void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = (void *)sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader((u32)packet, EVT_GS_COMMAND_BYTES);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = EVT_GS_TEST_PRIMARY;
    sdfAppendPacket(list, (u32)packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

/* Submit the same TEST bit layout for the secondary GS context, with ZTE forced on. */
void evtSubmitSecondaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = (void *)sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader((u32)packet, EVT_GS_COMMAND_BYTES);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = EVT_GS_TEST_SECONDARY;
    sdfAppendPacket(list, (u32)packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

/* Select a primary-context ALPHA equation by mode; unknown modes use 0x44. */
void evtSubmitPrimaryAlphaBlendMode(s32 blendMode) {
    void *list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;

    sdfInitPacketList(list);
    packet = (void *)sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader((u32)packet, EVT_GS_COMMAND_BYTES);
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
    sdfAppendPacket(list, (u32)packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha) {
    void *list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfInitPacketList(list);
    packet = (void *)sdfAllocPacketAligned(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdfConsFinalizePacketHeader((u32)packet, EVT_GS_COMMAND_BYTES);
    command->data = ((u64)fixedAlpha << 32) | EVT_FIXED_ALPHA_INTERPOLATION;
    command->registerId = EVT_GS_ALPHA_PRIMARY;
    sdfAppendPacket(list, (u32)packet);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

void func_00108D80(void) {
    void *list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *framePacket;
    sdfInitPacketList(list);
    framePacket = (void *)sdfAllocPacketAligned(EVT_FRAME_REFERENCE_PACKET_BYTES);
    func_0032DB30(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * EVT_FRAME_DRAW_RECORD_BYTES, framePacket, 0);
    sdfAppendDmaTagToList(list, framePacket);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

void func_00108E20(void) {
    void *list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    void *framePacket;
    sdfInitPacketList(list);
    framePacket = (void *)sdfAllocPacketAligned(EVT_FRAME_REFERENCE_PACKET_BYTES);
    func_0032DB78(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * EVT_FRAME_DRAW_RECORD_BYTES, framePacket, 0);
    sdfAppendDmaTagToList(list, framePacket);
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108EC0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109028);

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
INCLUDE_ASM(const s32, "game/code_00107EF8", evtSubmitGradientRectAtDepth);

extern void evtSubmitGradientRectAtDepth();

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor) {
    evtSubmitGradientRectAtDepth(x, y, width, height, EVT_QUAD_DEFAULT_DEPTH, topLeftColor, topRightColor, bottomRightColor, bottomLeftColor);
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109538);



extern u64 D_0037F5A0[]; /* index table; only the first 8 bytes are used */
extern void sdfConsAppendClearPacket(void *, s32);
extern void sdfConsAppendAssetPacket(void *, void *, s32);
extern void *func_0033B050(SdfPrimitiveRequest *);
extern void func_003332E8(void *, u32);
extern void sdfQueueAssetRelease(void *);

/* Preserve the native 0,2,3,1 vertex/index order; W components are not initialized here. */
void evtSubmitQuadFromVertices(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 i0, u32 i1, u32 i2, u32 i3) {
    SdfPrimitiveRequest desc;
    DrawVec4 verts[EVT_QUAD_VERTEX_COUNT];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    void *list;
    SdfPoolNode *surface;

    list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    sdfInitPacketList(list);
    sdfConsAppendClearPacket(list, 0);
    sdfConsAppendAssetPacket(list, D_00438DB0, 0);
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
    strip[0] = D_0037F5A0[0];
    sdfAppendPacket(list, (u32)func_0033B050(&desc));
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
    func_003332E8(asset, bits);
    list = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
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
    strip[0] = D_0037F5A0[0];
    uvs[0] = u0;
    uvs[1] = v0;
    uvs[2] = u0;
    uvs[3] = v1;
    uvs[4] = u1;
    uvs[5] = v1;
    uvs[6] = u1;
    uvs[7] = v0;
    sdfAppendPacket(list, (u32)func_0033B050(&desc));
    surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
    surface->append((SdfListHead *)surface, list);
    sdfQueueAssetRelease(asset);
}

void evtSubmitViewParamPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    void *list;
    D_0037F5B0[0][0] = x;
    D_0037F5B0[0][1] = y;
    D_0037F5B0[0][2] = z;
    D_0037F5B0[1][0] = u;
    D_0037F5B0[1][1] = v;
    D_0037F5B0[1][2] = w;
    D_0037F5D0[1] = second;
    D_0037F5D0[0] = first;
    list = (void *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, (u32)func_00348158(D_0037F5B0, D_0037F5D0, 2, 0x80));
    {
        SdfPoolNode *surface = &kwlnDrawSurfaces[kwlnDrawSurfaceIndex];
        surface->append((SdfListHead *)surface, list);
    }
}

extern s32 sdfCreateResetPacketList(void);


void evtDrawPositionedSurfacePacket(s32 x, s32 y, s32 packetArg, s32 drawArg) {
    SifCommand sifParameters;
    void *list;
    void *packet;
    SdfPoolNode *surface;
    list = (void *)sdfCreateResetPacketList();
    packet = (void *)sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket(packet);
    sdfAppendPacket(list, (u32)packet);
    sdfPktInit(&sifParameters, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, packetArg);
    sdfAppendPacket(list, (u32)sdfFormatSifPacket(&sifParameters, (const char *)drawArg));
    surface = &kwlnPositionedTextSurface;
    surface->append((SdfListHead *)surface, list);
}

void evtPrepareSizedDrawResource(s32 width, s32 height, u64 first, u64 second) {
    u64 resource;

    resource = func_0019F448(width << 4, height << 3, 0, first, second, 0);
    frFontDrawGlyphInDefaultMode(resource);
    frFontQueueGlyphInSelectedSlot(resource);
}

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
void evtDispatchSelectionValue(s32 valueSource, s32 *selectionParams) {
    s32 eventSelection = 0;

    D_00435BB0 = 0;
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

/* Source zero forwards an optional parameter pair; other sources resume deferred field work. */
void evtDispatchSelectionCommand(s32 commandSource, s32 *commandParams) {
    if (commandSource == 0) {
        if (commandParams == NULL) {
            func_001A9F30(1, 0, 0);
        }
        else {
            func_001A9F30(0, commandParams[0], commandParams[1]);
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
    if (kwlnTaskGetTaskByName(D_00435D20) != NULL) {
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

/* Stop the battle-stage test task and return the scene callback's native zero status. */
u32 evtEndBattleStageTest(void) {
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
    return kwlnTaskGetTaskByName(D_00411380) == NULL;
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

/* Start the shooting-game task: the callee allocates MnuShootingWork and starts "stgProcess". */
void evtStartShootingGameTask(void) {
    mdlCreateViewerPackageTask();
}

/* Destroy that shooting-game task hierarchy and invoke its work callback; return native zero status. */
u32 evtStopShootingGameTask(void) {
    func_00316EF8();
    return 0;
}

/* Complete the wait only after the named "stgProcess" task is absent. */
u8 evtWaitShootingGameTaskGone(void) {
    s64 taskExists;

    taskExists = func_00316ED0();
    return taskExists == 0;
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

void evtPrintDeveloperConsoleMessage(const char *format, ...) {
    char messageBuffer[EVT_DEV_CONSOLE_BUFFER_BYTES];
    __builtin_va_list formatArgs;

    __builtin_stdarg_start(formatArgs, format);
    if (evtConsoleFontResourceChain != 0) {
        func_00360E78(messageBuffer, format, formatArgs);
        sdfDevConsPrintf(evtConsoleFontResourceChain, "%s", messageBuffer);
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

extern u32 D_00435D4C;
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
    bytesPerRow = D_00435D4C >> 8;
    bytesPerCell = D_00435D4C >> 14;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_00435D4C < remainingBytes || remainingBytes < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 64 - columnIndex;

        if (remainingBytes >= remainingCells / bytesPerCell) {
            sdfAppendFillRectanglePacket(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            remainingBytes -= remainingCells / (D_00435D4C >> 14);
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
    while (remainingBytes >= (D_00435D4C >> 8)) {
        sdfAppendFillRectanglePacket(packetList, color, 0, x, topY,
                                     x + 0x400, topY + 8, 0x0FFFFF80, NULL);
        remainingBytes -= D_00435D4C >> 8;
        topY += 8;
    }
    if (remainingBytes > 0) {
        sdfAppendFillRectanglePacket(packetList, color, 0, x, topY,
                                     x + (remainingBytes / (D_00435D4C >> 14)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B3D8);

typedef struct SdfChipStats {
    u32 totalBytes;
    u32 freeBytes;
    u32 blockCount;
    u32 emptyBlocks;
    u32 partialBlocks;
    u32 usedCells[7];
} SdfChipStats;

extern void sdfGetGeneralHeapStats(s32 *stats);
extern void sdfGetChipHeapStats(SdfChipStats *stats);
extern char D_00435D50[];
extern char D_00435D58[];
extern void func_0035C860(char *buffer, const char *format, ...);
extern void func_0010B3D8(void *list, s32 source, s32 end);

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(SdfPoolNode *surface) {
    s32 generalHeapStats[6];
    SdfChipStats chipHeapStats;
    char statusText[100];
    void *packetList;

    sdfGetGeneralHeapStats(generalHeapStats);
    D_00435D4C = generalHeapStats[0];
    sdfGetChipHeapStats(&chipHeapStats);
    packetList = (void *)sdfAllocPacketAligned(EVT_PACKET_LIST_BYTES);
    sdfInitPacketList(packetList);
    func_0010B3D8(packetList, 0x8AC0, 0x79C0);
    func_0035C860(statusText, D_00435D50, generalHeapStats[1]);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    func_0035C860(statusText, D_00435D58, chipHeapStats.freeBytes);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->append((SdfListHead *)surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&kwlnPositionedTextSurface);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B7B8);

void func_0010B8D0(void) {
}

extern void func_0010B7B8(void);
extern void func_0010B8D0(void);
extern s32 D_00438E68;
extern char D_004113B8[]; /* "DebugTimeGrph" */

void evtToggleAlternateDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_00438E68 = kwlnTaskCreate(D_004113B8, 0x2710, 1, 1, func_0010B7B8, func_0010B8D0, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy((void *)D_00438E68, 0);
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

typedef struct BfFlw0Header {
    u8 pad00[8];
    u32 magic;
    u8 pad0C[4];
    s32 sectionCount;
    s16 intCount;
    s16 floatCount;
    u8 pad18[8];
    ScrSection sections[1];
} BfFlw0Header;

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 itfMesCreateWindow(void *data);

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_004113B8);

ScrData *bfContextCreate(void *rawHeader, ScrSection *sections, ScrLabel *procedures,
                        ScrLabel *labels, ScrInstr *instructions, void *auxiliaryData,
                        char *strings, s32 procedureIndex) {
    BfFlw0Header *header = rawHeader;
    ScrData *process;
    s32 i;
    s8 *types;
    ScrStackValue *values;

    if (header == NULL || sections == NULL || procedures == NULL ||
        instructions == NULL || procedureIndex < 0 ||
        procedureIndex >= sections->count) {
        return NULL;
    }
    process = sdfAllocSizeClassBlock(0xF4);
    if (process == NULL) {
        return NULL;
    }
    i = 0;
    while ((process->name[i] = procedures[procedureIndex].name[i]) != '\0') {
        i++;
    }
    process->pc = procedures[procedureIndex].addr;
    process->sp = 0;
    values = process->stackValues;
    types = process->stackTypes;
    for (i = 27; i >= 0; i--) {
        *types++ = 0;
        values++->i = 0;
    }
    process->sections = sections;
    process->labels = labels;
    process->instructions = instructions;
    process->strings = strings;
    process->procedureIndex = procedureIndex;
    process->resourceIndex = -1;
    process->scriptHeader = header;
    process->procedures = procedures;
    process->auxiliaryData = auxiliaryData;
    process->timer = 0;
    process->cmdTimer = 0;
    process->scriptHandle = NULL;
    process->localInt = NULL;
    process->localFloat = NULL;
    process->task = NULL;
    process->previous = NULL;
    process->next = NULL;
    if (header->intCount > 0) {
        process->localInt = sdfAllocSizeClassBlock(header->intCount * sizeof(s32));
        for (i = 0; i < header->intCount; i++) {
            process->localInt[i] = 0;
        }
    } else {
        process->localInt = NULL;
    }
    if (header->floatCount > 0) {
        process->localFloat = sdfAllocSizeClassBlock(header->floatCount * sizeof(f32));
        for (i = 0; i < header->floatCount; i++) {
            process->localFloat[i] = 0;
        }
    } else {
        process->localFloat = NULL;
    }
    if (auxiliaryData != NULL) {
        process->resourceIndex = itfMesCreateWindow(auxiliaryData);
    }
    evtLinkWorkNode(process);
    evtPrintDeveloperConsoleMessage("start <%s>\n", procedures[procedureIndex].name);
    return process;
}

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

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D40);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D48);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D4C);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D50);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D58);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrNamedProcessCount);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrNamedProcessHead);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrNamedProcessTail);

