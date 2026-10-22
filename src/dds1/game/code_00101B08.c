#include "sdk_ps2.h"
#include "common.h"
#include "sdf_column_packet_api.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "sdf_packet_list.h"
#include "sdf_draw_packet_groups.h"
#include "kwln_frame_draw_bank.h"
#include "kwln_sprite.h"
#include "dds3Admin.h"
#include "sdf_linked_packet.h"
#include "sdf_dev_start_load_api.h"
#include "kwln.h"
#include "kwln_task_flags.h"
#include "sdf.h"
#include "sdf_pad_mode_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"

struct EffWorldNode;
extern struct EffWorldNode *dds3ActiveWorld;

/* Detach a task from its parent's child list. Detached tasks are left untouched;
 * an attached task must already occur in that parent's sibling chain.
 * The hierarchy link is next, separate from scheduler listNext/listPrev. */
void dds3UnlinkProcess(KwlnTask *task) {
    KwlnTask *parent = task->parent;

    if (parent == 0) {
        return;
    }
    if (parent->childList == task) {
        parent->childList = task->next;
    } else {
        KwlnTask *previousSibling = parent->childList;

        while (previousSibling->next != task) {
            previousSibling = previousSibling->next;
        }
        previousSibling->next = task->next;
    }
    task->parent = 0;
    task->next = 0;
}

/* Set mask bits on the object selection described by object and scope. */
void dds3SetProcessFlag(KwlnTask *object, u32 mask, s32 scope) {
    ChangeProcessFlag(1, object, mask, scope);
}

/* Clear the same scoped object mask bits without changing the selection. */
void dds3ResetProcessFlag(KwlnTask *object, u32 mask, s32 scope) {
    ChangeProcessFlag(0, object, mask, scope);
}

typedef struct SdfThreadNode SdfThreadNode;

extern void __main(void);
extern void sdfInitDeviceSemaphores(void);
extern s32 sceSifSyncIop(void);
extern void sceCdInit(s32);
extern void sceCdMmode(u32);
extern void LoadMcModules(const char *prefix, s32 moduleGroup);
extern void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes);
extern void sdfFileSetHostRoot(const char *path);
extern void sdfFileSetDefaultSrc(s32 worker);
extern void sdfFileCmdStartThread(s32 index);
extern void sdfLoadModulesAndBindNamedResourceRpc(const char *modulePath, const char *optionalModulePath);
extern void sdfStartFileRpcServer(void);
extern void kwlnRunMainLoop(void);

extern char D_0039DF28[]; /* "cdrom0:\IRX\IOPRP255.IMG;1" */
extern char D_0039DF48[]; /* "cdrom0:\IRX\SIO2MAN.IRX;1" */
extern char D_0039DF68[]; /* "cdrom0:\IRX\PADMAN.IRX;1" */
extern char D_0039DF88[]; /* "\DDS3.IMG;1" */
extern char D_0039DF98[]; /* "cdrom0:\DDS3.DDT;1" */
extern char D_0039DFB0[]; /* "cdrom0:\IRX" */
extern char D_0039DFC0[]; /* "../../../dds3data" */
extern char D_0039DFD8[]; /* "cdrom0:\USERIRX\SDFIOP.IRX;1" */
extern char D_0039DFF8[]; /* "cdrom0:\IRX\MSIFRPC.IRX;1" */
extern const s32 D_003BA83C;
extern u32 sdfDiscType;
extern u8 D_003BD39C;
extern SdfThreadNode kwlnMainLoopThreadNode;
extern u8 D_003BDC90[];

s32 sdfInitializeRuntimeAndStartMainLoop(void) {
    __main();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sdfDiscType = D_003BA83C;
    sdfInitDeviceSemaphores();
    while (sceSifRebootIop(D_0039DF28) == 0) {
    }
    while (sceSifSyncIop() == 0) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(D_003BA83C);
    while (sceSifLoadModule(D_0039DF48, 0, 0) < 0) {
    }
    while (sceSifLoadModule(D_0039DF68, 0, 0) < 0) {
    }
    sdfCdImageInit(D_0039DF88, D_0039DF98);
    LoadMcModules(D_0039DFB0, 1);
    dds3InitSubsystems(0x17C0000, 0x200000, 0x380000);
    D_003BD39C = 1;
    sdfPadChangeMode(0, 3);
    sdfPadChangeMode(1, 3);
    sdfFileSetHostRoot(D_0039DFC0);
    sdfFileSetDefaultSrc(3);
    sdfFileCmdStartThread(3);
    sdfFileCmdStartThread(0);
    sdfLoadModulesAndBindNamedResourceRpc(D_0039DFD8, D_0039DFF8);
    sdfStartFileRpcServer();
    sdfStartThreadAndWait(&kwlnMainLoopThreadNode, kwlnRunMainLoop, D_003BDC90, 0x4000, 0x60, 0);
    sdfSetMainThreadPriority();
    return 0;
}

#define KWLN_FRAME_POOL_NODE_COUNT 0x62

extern SdfPoolNode kwlnDrawSurfaces[KWLN_FRAME_POOL_NODE_COUNT];
extern u8 D_00325860[];
extern KwlnFrameDrawBank D_00325870[2];
extern void sdfWaitAndSelectBuffer(void);
extern void sdfGrOtInit(SdfPoolNode *, s32);
extern void kwlnInitializeFrameDrawPackets(s32 bufferIndex);

#define KWLN_FRAME_BUFFER_BYTES 0x1F40
#define KWLN_FRAME_LAST_GROUP_INDEX 12
#define KWLN_FRAME_PACKET_LIST_BYTES 0x20
#define KWLN_FRAME_GS_PACKET_BYTES 0x40
#define KWLN_FRAME_GIF_AD_REGISTER 0xE
#define KWLN_FRAME_GIF_SINGLE_REG_FIELD 0x10000000
#define KWLN_FRAME_GIF_TWO_WRITES_EOP 0x8002
#define KWLN_FRAME_GS_TEXA 0x3B
#define KWLN_FRAME_GS_TEXFLUSH 0x3F
#define KWLN_FRAME_GS_TEST_PRIMARY 0x47
#define KWLN_FRAME_GS_ALPHA_PRIMARY 0x42
#define KWLN_FRAME_NEUTRAL_COLOR 0x80
#define KWLN_FRAME_HALF_WIDTH 0x100
#define KWLN_FRAME_HALF_HEIGHT 0xE0
#define KWLN_FRAME_SCALE_FRACTION_BITS 12
#define KWLN_FRAME_X_UNITS_PER_PIXEL 16
#define KWLN_FRAME_Y_UNITS_PER_PIXEL 8
#define KWLN_FRAME_LEFT_BASE 0x7000
#define KWLN_FRAME_TOP_BASE 0x7900
#define KWLN_FRAME_RIGHT_BASE 0x9000
#define KWLN_FRAME_BOTTOM_BASE 0x8700
#define KWLN_FRAME_U_EXTENT 0x2000
#define KWLN_FRAME_V_EXTENT 0xE00
#define KWLN_FRAME_SKIP_POOL_QUEUE_BIT 0x2000000
#define KWLN_FRAME_CLEAR_SKIP_POOL_QUEUE 0xFDFFFFFF

/* Prepare the selected buffer's thirteen packet groups and link its draw sink.
 * The buffer index is sampled before waiting/selecting; keep that ordering. */
s32 kwlnPrepareFrameDrawPackets(KwlnTask *task) {
    s32 bufferIndex = dds3GetFrameID();
    s32 groupIndex;
    SdfDrawPacketGroups *packetGroups;
    SdfPoolNode *drawSink;

    sdfWaitAndSelectBuffer();
    sdfGrOtInit(kwlnDrawSurfaces, KWLN_FRAME_POOL_NODE_COUNT);
    sdfPreDrawChainInit((SdfPacketChain *)D_00325860);
    kwlnInitializeFrameDrawPackets(bufferIndex);
    packetGroups = D_00325870[bufferIndex].packetGroups;
    for (groupIndex = KWLN_FRAME_LAST_GROUP_INDEX; groupIndex >= 0; groupIndex--) {
        sdfModelSetDrawEnv(packetGroups);
        packetGroups++;
    }
    drawSink = kwlnDrawSurfaces;
    drawSink->append(drawSink, &D_00325870[bufferIndex].initialList);
    return 0;
}

#include "sdf_projection.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"

struct ConsFrustumPacket;
extern u32 kwlnTextureReferenceFlag;
extern u32 D_003BA900;
extern u32 D_003BA8EC;
extern u8 kwlnHeldTextureLightingPackets[];
extern u8 kwlnHeldTextureMatrixPackets[];
extern u8 kwlnOverlayLightingPackets[];
extern u8 kwlnOverlayMatrixPackets[];
extern u8 D_003270F0[];
extern SdfLightSources kwlnHeldTextureLightSources;
extern SdfLightSources kwlnOverlayLightSources;
extern SdfLightSources D_00324770;
extern f32 kwlnHeldTextureAmbientColor[4];
extern f32 kwlnOverlayAmbientColor[4];
extern f32 kwlnDefaultColorVector[4];
extern f32 kwlnHeldTextureTargetVector[4], kwlnHeldTextureEyeVector[4], kwlnHeldTextureUpVector[4];
extern f32 D_00324A20[4], kwlnOverlayEyeVector[4], kwlnOverlayUpVector[4];
extern KwlnDrawVectorParams kwlnHeldTextureFrustumParameters, kwlnOverlayFrustumParameters, kwlnDrawVector;
extern u8 kwlnHeldTextureViewMatrix[0x40];
extern u8 D_00329790[];
extern u8 sdfViewEyeVector[], sdfViewTargetVector[], sdfViewUpVector[];
extern u8 D_00324590[];
extern u8 sdfViewMatrix[0x40];
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern void kwlnAdvanceShakeOffsets(void);
extern u32 evtCheckSelectionState(void);
extern void dds3SceneLightUpdateFirst(void);
extern void fldConfigureRenderCleanupMessage(void);
#include "sdf_packet_alloc.h"
#include "sdf_message.h"
extern void sdf3DMakeFogPacket(struct ConsFrustumPacket *, KwlnDrawVectorParams *);
extern void sdf3DSetDefaultLight(u32);
extern void sdf3DSetDefaultFog(u32);
extern SdfListHead *dds3LinkZClearDrawTag(s32);
extern void eglSetOt(u32);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);
extern void evtSubmitPrimaryAlphaBlendMode(s32);

/* Rebuild and queue the selected buffer's scene, overlay and view packets. */
s32 kwlnBuildFrameScenePackets(KwlnTask *task) {
    s128 eye;
    s128 target;
    const u32 bufferIndex = dds3GetFrameID();
    SdfPoolNode *providerSurfaces;

    kwlnAdvanceShakeOffsets();
    evtCheckSelectionState();
    dds3SceneLightUpdateFirst();
    if (kwlnTextureReferenceFlag != 0) {
        sdf3DMakeLightParallelPacket((SdfLightingPacketStorage *)(kwlnHeldTextureLightingPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
            kwlnHeldTextureLightSources, kwlnHeldTextureAmbientColor);
        *(u32 *)(kwlnHeldTextureLightingPackets - 0x140 + bufferIndex * KWLN_FRAME_BUFFER_BYTES) = D_003BA900;
        if (kwlnTextureReferenceFlag != 0) {
            SdfPoolNode *sceneNode;
            SdfListHead *sceneList;
            u64 *sceneFrustum;
            sdfCameraBuildProjection(&D_003247B0.camera);
            LookAtMatrix(kwlnHeldTextureTargetVector, kwlnHeldTextureEyeVector, kwlnHeldTextureUpVector);
            VU0_STORE_MATRIX_UNCLOBBERED(kwlnHeldTextureViewMatrix);
            sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(kwlnHeldTextureMatrixPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
                &D_003247B0, kwlnHeldTextureViewMatrix);
            sceneNode = &kwlnDrawSurfaces[1];
            sceneNode->append(sceneNode,
                (SdfListHead *)(kwlnHeldTextureMatrixPackets - 0x190 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
            sceneList = dds3FontReqAllocDrawTag();
            sceneFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
            sdf3DMakeFogPacket((struct ConsFrustumPacket *)sceneFrustum, &kwlnHeldTextureFrustumParameters);
            sdfDrawTagLink(sceneList, (u32)sceneFrustum);
            sceneNode->append(sceneNode, sceneList);
        }
    }
    {
        SdfPoolNode *overlayNode;
        SdfListHead *overlayList;
        u64 *overlayFrustum;
        sdf3DMakeLightParallelPacket((SdfLightingPacketStorage *)(kwlnOverlayLightingPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
            kwlnOverlayLightSources, kwlnOverlayAmbientColor);
        sdfCameraBuildProjection(&D_00324980.camera);
        LookAtMatrix(D_00324A20, kwlnOverlayEyeVector, kwlnOverlayUpVector);
        VU0_STORE_MATRIX_UNCLOBBERED(D_00329790);
        sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(kwlnOverlayMatrixPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES), &D_00324980, D_00329790);
        overlayNode = &kwlnDrawSurfaces[85];
        overlayNode->append(overlayNode, (SdfListHead *)(kwlnOverlayMatrixPackets - 0x20 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
        overlayList = dds3FontReqAllocDrawTag();
        overlayFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket((struct ConsFrustumPacket *)overlayFrustum, &kwlnOverlayFrustumParameters);
        sdfDrawTagLink(overlayList, (u32)overlayFrustum);
        overlayNode->append(overlayNode, overlayList);

        sdf3DMakeLightParallelPacket((SdfLightingPacketStorage *)(kwlnOverlayMatrixPackets - 0x440 + bufferIndex * KWLN_FRAME_BUFFER_BYTES), D_00324770, kwlnDefaultColorVector);
        *(u32 *)(kwlnOverlayMatrixPackets - 0x620 + bufferIndex * KWLN_FRAME_BUFFER_BYTES) = D_003BA8EC;
    }
    sdfCameraBuildProjection(&sdfSceneProjectionParameters.camera);
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(600.0f, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324590);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, &eye);
    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, &target);
    LookAtMatrix(&eye, &target, sdfViewUpVector);
    VU0_STORE_MATRIX_UNCLOBBERED(sdfViewMatrix);
    {
        SdfPoolNode *viewNode;
        SdfListHead *viewList;
        u64 *viewFrustum;
        sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(D_003270F0 + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
            &sdfSceneProjectionParameters, sdfViewMatrix);
        sdf3DSetCamera(&sdfSceneProjectionParameters, sdfViewMatrix);
        viewNode = &kwlnDrawSurfaces[7];
        viewNode->append(viewNode, (SdfListHead *)(D_003270F0 - 0x250 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
        viewList = dds3FontReqAllocDrawTag();
        viewFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket((struct ConsFrustumPacket *)viewFrustum, &kwlnDrawVector);
        sdfDrawTagLink(viewList, (u32)viewFrustum);
        viewNode->append(viewNode, viewList);

        sdfPreDrawChainLink((SdfPacketChain *)D_00325860,
            (SdfLinkedPacketList *)(D_003270F0 - 0x230 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
        providerSurfaces = &kwlnDrawSurfaces[7];
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[0], &providerSurfaces[32], &providerSurfaces[33],
                                              &providerSurfaces[48], &providerSurfaces[49]);
        fldConfigureRenderCleanupMessage();
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[8], &providerSurfaces[55], &providerSurfaces[56],
                                              &providerSurfaces[57], &providerSurfaces[58]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[9], &providerSurfaces[67], &providerSurfaces[68],
                                              &providerSurfaces[69], &providerSurfaces[70]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[1], &providerSurfaces[1], &providerSurfaces[2],
                                              &providerSurfaces[3], &providerSurfaces[4]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[2], &providerSurfaces[6], &providerSurfaces[7],
                                              &providerSurfaces[8], &providerSurfaces[9]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[3], &providerSurfaces[11], &providerSurfaces[12],
                                              &providerSurfaces[13], &providerSurfaces[14]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[4], &providerSurfaces[16], &providerSurfaces[17],
                                              &providerSurfaces[18], &providerSurfaces[19]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[5], &providerSurfaces[21], &providerSurfaces[22],
                                              &providerSurfaces[23], &providerSurfaces[24]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[6], &providerSurfaces[37], &providerSurfaces[38],
                                              &providerSurfaces[39], &providerSurfaces[40]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[10], &providerSurfaces[42], &providerSurfaces[43],
                                              &providerSurfaces[46], &providerSurfaces[47]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[11], &providerSurfaces[79], &providerSurfaces[80],
                                              &providerSurfaces[81], &providerSurfaces[82]);
        sdfStoreMessageWordsAndNotifyConsumer(kwlnSceneSurfaceSets[12], &providerSurfaces[-5], &providerSurfaces[-4],
                                              &providerSurfaces[-3], &providerSurfaces[-2]);
        sdf3DSetDefaultLight((u32)(D_003270F0 + bufferIndex * 0x1F40 + 0xD0));
        sdf3DSetDefaultFog((u32)viewFrustum);

    }
    {
        SdfListHead *stateList = dds3LinkZClearDrawTag(0);
        SdfPoolNode *const state0 = &kwlnDrawSurfaces[81];
        state0->append(state0, stateList);
    }
    {
        SdfListHead *stateList = dds3LinkZClearDrawTag(1);
        SdfPoolNode *const state1 = &kwlnDrawSurfaces[93];
        state1->append(state1, stateList);
    }
    {
        SdfListHead *stateList = dds3LinkZClearDrawTag(2);
        SdfPoolNode *const state2 = &kwlnDrawSurfaces[95];
        state2->append(state2, stateList);
    }

    eglSetOt(0x2C);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    evtSubmitPrimaryAlphaBlendMode(0);
    eglSetOt(0x2D);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    evtSubmitPrimaryAlphaBlendMode(0);
    eglSetOt(0x2E);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    evtSubmitPrimaryAlphaBlendMode(1);
    eglSetOt(0x2F);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    evtSubmitPrimaryAlphaBlendMode(2);
    return 0;
}


extern s32 sdf2DGetPrimPtr(s32);
extern void dds3DrawDistRipOverError(void);
extern void kwlnStepBackgroundFade(void);
extern void kwlnDrawBackgroundFadeBands(void);
extern void dds3FadeUpdate(void);
extern void kwlnQueueBackgroundFadeOverlayPacket(void);
extern void dds3BlurUpdate(void);
extern u32 dds3NullDrawProc(void);
extern void sdfGrFlushOtChain(s32);
extern void sdfGrReqDraw(SdfListHead *, SdfPacketChain *);
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];
extern SdfPoolNode D_00325708;
extern u8 kwlnDrawOverlayEnabled;
extern s16 kwlnDrawOverlayAlpha;
extern s16 kwlnDrawOverlayScale;
extern s32 D_003BA910[2];
extern u8 D_003BA7FC;
extern SdfListHead *D_003BA844;
extern u32 kwlnDrawControlFlags;
extern s8 sdfGrFrameComplete;


/* Update draw controls, submit thirteen packet groups, optionally draw the
 * vignette, then flush pools. The skip bit suppresses one packet-slot publication. */
s32 kwlnRenderFrame(KwlnTask *task) {
    s32 bufferIndex = dds3GetFrameID();
    SdfPoolNode *(*groupHeads)[4];
    SdfDrawPacketGroups *packetGroups;
    SdfListHead *packetList;
    u64 *texturePacket;
    u64 *blendPacket;
    KwlnSpriteVertex *vertex;
    void *spritePacket;
    SdfListHead *poolHead;
    s32 edgeDistances[4];
    s32 i;

    dds3DrawDistRipOverError();
    kwlnStepBackgroundFade();
    kwlnDrawBackgroundFadeBands();
    dds3FadeUpdate();
    kwlnQueueBackgroundFadeOverlayPacket();
    dds3BlurUpdate();
    groupHeads = kwlnSceneSurfaceSets;
    packetGroups = D_00325870[bufferIndex].packetGroups;
    for (i = KWLN_FRAME_LAST_GROUP_INDEX; i >= 0; i--) {
        sdfModelDrawEnvLinkOt(*groupHeads, packetGroups);
        groupHeads++;
        packetGroups++;
    }
    if (kwlnDrawOverlayEnabled != 0 && dds3NullDrawProc() == 0) {
        packetList = (SdfListHead *)sdfGrAllocPacketBuff(KWLN_FRAME_PACKET_LIST_BYTES);
        sdfDrawTagInit(packetList);
        sdfGrLinkDispFrameTex(packetList, (u32)(kwlnFrameDrawPacketRecords[bufferIndex].baseBytes), (SdfDmaNode *)sdfGrAllocPacketBuff(KWLN_FRAME_PACKET_LIST_BYTES));
        texturePacket = (u64 *)sdfGrAllocPacketBuff(KWLN_FRAME_GS_PACKET_BYTES);
        texturePacket[0] = 3;
        /* VIF FLUSHA, then DIRECT for the three following quadwords. */
        texturePacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        texturePacket[2] = ((u64)KWLN_FRAME_GIF_SINGLE_REG_FIELD << 32) | KWLN_FRAME_GIF_TWO_WRITES_EOP;
        texturePacket[3] = KWLN_FRAME_GIF_AD_REGISTER;
        texturePacket[4] = ((u64)KWLN_FRAME_NEUTRAL_COLOR << 32) | KWLN_FRAME_NEUTRAL_COLOR;
        texturePacket[5] = KWLN_FRAME_GS_TEXA;
        texturePacket[6] = 0;
        texturePacket[7] = KWLN_FRAME_GS_TEXFLUSH;
        sdfDrawTagLink(packetList, (u32)texturePacket);
        blendPacket = (u64 *)sdfGrAllocPacketBuff(KWLN_FRAME_GS_PACKET_BYTES);
        blendPacket[0] = 3;
        blendPacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        blendPacket[2] = ((u64)KWLN_FRAME_GIF_SINGLE_REG_FIELD << 32) | KWLN_FRAME_GIF_TWO_WRITES_EOP;
        blendPacket[3] = KWLN_FRAME_GIF_AD_REGISTER;
        blendPacket[4] = 0x31001;
        blendPacket[5] = KWLN_FRAME_GS_TEST_PRIMARY;
        blendPacket[6] = 0x44;
        blendPacket[7] = KWLN_FRAME_GS_ALPHA_PRIMARY;
        sdfDrawTagLink(packetList, (u32)blendPacket);
        edgeDistances[0] = D_003BA910[0] + KWLN_FRAME_HALF_WIDTH;
        edgeDistances[1] = D_003BA910[1] + KWLN_FRAME_HALF_HEIGHT;
        edgeDistances[2] = KWLN_FRAME_HALF_WIDTH - D_003BA910[0];
        edgeDistances[3] = KWLN_FRAME_HALF_HEIGHT - D_003BA910[1];
        /* Signed Q12 scale; X and Y use distinct GS coordinate units below. */
        for (i = 0; i != 4; i++) {
            edgeDistances[i] = edgeDistances[i] * kwlnDrawOverlayScale >> KWLN_FRAME_SCALE_FRACTION_BITS;
        }
        spritePacket = sdf2DAllocSprite(1);
        vertex = (KwlnSpriteVertex *)sdf2DGetPrimPtr((s32)spritePacket);
        vertex->r = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->g = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->b = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->a = kwlnDrawOverlayAlpha;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = KWLN_FRAME_LEFT_BASE - edgeDistances[0] * KWLN_FRAME_X_UNITS_PER_PIXEL;
        vertex->corner[0].y = KWLN_FRAME_TOP_BASE - edgeDistances[1] * KWLN_FRAME_Y_UNITS_PER_PIXEL;
        vertex->corner[0].mask = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[1].u = KWLN_FRAME_U_EXTENT;
        vertex->corner[1].v = KWLN_FRAME_V_EXTENT;
        vertex->corner[1].x = KWLN_FRAME_RIGHT_BASE + edgeDistances[2] * KWLN_FRAME_X_UNITS_PER_PIXEL;
        vertex->corner[1].y = KWLN_FRAME_BOTTOM_BASE + edgeDistances[3] * KWLN_FRAME_Y_UNITS_PER_PIXEL;
        vertex->corner[1].mask = 0;
        vertex->corner[1].flag = 0;
        sdfDrawTagLink(packetList, (u32)spritePacket);
        D_00325708.append(&D_00325708, packetList);
    }
    poolHead = sdfFlushPoolNodes(kwlnDrawSurfaces);
    D_003BA844 = poolHead;
    if (D_003BA7FC != 0) {
        sdfGrFlushOtChain(poolHead->first);
        D_003BA7FC = 0;
    }
    if (!(kwlnDrawControlFlags & KWLN_FRAME_SKIP_POOL_QUEUE_BIT)) {
        sdfGrReqDraw(poolHead, (SdfPacketChain *)D_00325860);
    } else {
        kwlnDrawControlFlags &= KWLN_FRAME_CLEAR_SKIP_POOL_QUEUE;
    }
    sdfGrFrameComplete = 0;
    return 0;
}

struct EffWorldNode;
extern s32 dds3CallBasicMainProc(struct EffWorldNode *holder);

/* Invoke the active world's first callback column; this task step returns zero. */
s32 dds3BasicMainProcess(KwlnTask *task) {
    dds3CallBasicMainProc(dds3ActiveWorld);
    return 0;
}

struct EffWorldNode;
extern s32 dds3CallBasicDrawProc(struct EffWorldNode *holder);

/* Invoke the active world's second callback column; this task step returns zero. */
s32 dds3BasicDrawProcess(KwlnTask *task) {
    dds3CallBasicDrawProc(dds3ActiveWorld);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DF28);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DF48);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DF68);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DF88);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DF98);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DFB0);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DFC0);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DFD8);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039DFF8);

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039E018);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA83C);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA844);

