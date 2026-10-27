#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_column_packet_api.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "sdf_packet_list.h"
#include "sdf_draw_packet_groups.h"
#include "kwln_frame_draw_bank.h"
#include "kwln_sprite.h"
#include "dds3Admin.h"
#include "kwln.h"
#include "kwln_task_flags.h"
#include "sdf.h"
#include "sdf_pad_mode_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"
#include "sdf_linked_packet.h"
#include "sdf_dev_start_load_api.h"

struct dds3Basic_t;
extern struct dds3Basic_t *dds3TheWorld;

/* Detach a task from its parent's child list. Detached tasks are left untouched;
 * an attached task must already occur in that parent's sibling chain.
 * The hierarchy link is nextSibling, separate from scheduler listNext/listPrev. */
void dds3UnlinkProcess(dds3ProcessID_t *task) {
    dds3ProcessID_t *parent = task->parent;

    if (parent == 0) {
        return;
    }
    if (parent->childList == task) {
        parent->childList = task->nextSibling;
    } else {
        dds3ProcessID_t *previousSibling = parent->childList;

        while (previousSibling->nextSibling != task) {
            previousSibling = previousSibling->nextSibling;
        }
        previousSibling->nextSibling = task->nextSibling;
    }
    task->parent = 0;
    task->nextSibling = 0;
}

/* Set mask bits on the object selection described by object and scope. */
void dds3SetProcessFlag(dds3ProcessID_t *object, u32 mask, KwlnTaskFlagScope scope) {
    ChangeProcessFlag(1, object, mask, scope);
}

/* Clear the same scoped object mask bits without changing the selection. */
void dds3ResetProcessFlag(dds3ProcessID_t *object, u32 mask, KwlnTaskFlagScope scope) {
    ChangeProcessFlag(0, object, mask, scope);
}

typedef struct SdfThreadNode SdfThreadNode;

extern void __main(void);
extern void sdfInitDeviceSemaphores(void);
extern s32 sceSifSyncIop(void);
extern void sceCdInit(s32);
extern void sceCdMmode(u32 discType);
extern void LoadMcModules(const char *prefix, s32 moduleGroup);
extern void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes);
extern void sdfFileSetHostRoot(const char *path);
extern void sdfFileSetDefaultSrc(s8 worker);
extern void sdfFileCmdStartThread(s32 index);
extern void sdfLoadModulesAndBindNamedResourceRpc(const char *modulePath, const char *optionalModulePath);
extern void sdfStartFileRpcServer(void);
extern void kwlnRunMainLoop(void);

extern const char D_004110A8[];
extern const char D_004110C8[];
extern const char D_004110E8[];
extern const char D_00411108[];
extern const char D_00411118[];
extern const char D_00411130[];
extern const char D_00411140[];
extern const char D_00411158[];
extern const char D_00411178[];
extern const s32 D_00435C0C;
extern u32 sdfDiscType;
extern u8 D_00438A8C;
extern SdfThreadNode kwlnMainLoopThreadNode;
extern u8 D_00439410[];
s32 sdfInitializeRuntimeAndStartMainLoop(void) {
    __main();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sdfDiscType = D_00435C0C;
    sdfInitDeviceSemaphores();
    while (sceSifRebootIop(D_004110A8) == 0) {
    }
    while (sceSifSyncIop() == 0) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(D_00435C0C);
    while (sceSifLoadModule(D_004110C8, 0, 0) < 0) {
    }
    while (sceSifLoadModule(D_004110E8, 0, 0) < 0) {
    }
    sdfCdImageInit(D_00411108, D_00411118);
    LoadMcModules(D_00411130, 1);
    dds3InitSubsystems(0x17C0000, 0x200000, 0x335000);
    D_00438A8C = 1;
    sdfPadChangeMode(0, SDF_PAD_MODE_DUALSHOCK_LOCKED);
    sdfPadChangeMode(1, SDF_PAD_MODE_DUALSHOCK_LOCKED);
    sdfFileSetHostRoot(D_00411140);
    sdfFileSetDefaultSrc(3);
    sdfFileCmdStartThread(3);
    sdfFileCmdStartThread(0);
    sdfLoadModulesAndBindNamedResourceRpc(D_00411158, D_00411178);
    sdfStartFileRpcServer();
    sdfStartThreadAndWait(&kwlnMainLoopThreadNode, kwlnRunMainLoop, D_00439410, 0x4000, 0x60, 0);
    sdfSetMainThreadPriority();
    return 0;
}

#define KWLN_FRAME_POOL_NODE_COUNT 0x62

extern SdfPoolNode sdfGrOtTable[KWLN_FRAME_POOL_NODE_COUNT];
extern u8 D_00380860[];
extern KwlnFrameDrawBank D_00380870[2];
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
s32 kwlnPrepareFrameDrawPackets(dds3ProcessID_t *task) {
    s32 bufferIndex = dds3GetFrameID();
    s32 groupIndex;
    SdfDrawPacketGroups *packetGroups;
    SdfPoolNode *drawSink;

    sdfWaitAndSelectBuffer();
    sdfGrOtInit(sdfGrOtTable, KWLN_FRAME_POOL_NODE_COUNT);
    sdfPreDrawChainInit((SdfPacketChain *)D_00380860);
    kwlnInitializeFrameDrawPackets(bufferIndex);
    packetGroups = D_00380870[bufferIndex].packetGroups;
    for (groupIndex = KWLN_FRAME_LAST_GROUP_INDEX; groupIndex >= 0; groupIndex--) {
        sdfModelSetDrawEnv(packetGroups);
        packetGroups++;
    }
    drawSink = sdfGrOtTable;
    drawSink->append(drawSink, &D_00380870[bufferIndex].initialList);
    return 0;
}

#include "sdf_projection.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"

struct ConsFrustumPacket;
extern u32 dds3RenderTex;
extern u32 D_00435CD0;
extern u32 D_00435CBC;
extern u8 kwlnHeldTextureLightingPackets[];
extern u8 kwlnHeldTextureMatrixPackets[];
extern u8 kwlnOverlayLightingPackets[];
extern u8 kwlnOverlayMatrixPackets[];
extern u8 D_003820F0[];
extern SdfLightSources kwlnHeldTextureLightSources;
extern SdfLightSources kwlnOverlayLightSources;
extern SdfLightSources D_0037F770;
extern f32 kwlnHeldTextureAmbientColor[4];
extern f32 kwlnOverlayAmbientColor[4];
extern f32 kwlnDefaultColorVector[4];
extern f32 kwlnHeldTextureTargetVector[4], kwlnHeldTextureEyeVector[4], kwlnHeldTextureUpVector[4];
extern f32 D_0037FA20[4], kwlnOverlayEyeVector[4], kwlnOverlayUpVector[4];
extern KwlnDrawVectorParams kwlnHeldTextureFrustumParameters, kwlnOverlayFrustumParameters, kwlnDrawVector;
extern Matrix4 kwlnHeldTextureViewMatrix;
extern Matrix4 D_00384790;
extern f32 aViewTarget[4], sdfViewTargetVector[4], aViewDown[4];
extern u8 sdfViewShakeVector[];
extern Matrix4 sdfViewMatrix;
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern void kwlnAdvanceShakeOffsets(void);
extern u32 evtCheckSelectionState(void);
extern void dds3SceneLightUpdateFirst(void);
extern void fldConfigureRenderCleanupMessage(void);
#include "sdf_packet_alloc.h"
#include "sdf_message.h"
extern void sdf3DMakeFogPacket(struct ConsFrustumPacket *, KwlnDrawVectorParams *);
extern void sdf3DSetDefaultLight(const struct SdfDmaTagHeader *lightingPacket);
extern void sdf3DSetDefaultFog(u32);
extern sdfDrawTag_t *dds3LinkZClearDrawTag(s32);
extern void eglSetOt(u32);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);
extern void evtSubmitPrimaryAlphaBlendMode(s32);

/* Rebuild and queue the selected buffer's scene, overlay and view packets. */
s32 kwlnBuildFrameScenePackets(dds3ProcessID_t *task) {
    s128 eye;
    s128 target;
    const u32 bufferIndex = dds3GetFrameID();
    SdfPoolNode *providerSurfaces;

    kwlnAdvanceShakeOffsets();
    evtCheckSelectionState();
    dds3SceneLightUpdateFirst();
    if (dds3RenderTex != 0) {
        sdf3DMakeLightParallelPacket((SdfLightingPacketStorage *)(kwlnHeldTextureLightingPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
            kwlnHeldTextureLightSources, kwlnHeldTextureAmbientColor);
        *(u32 *)(kwlnHeldTextureLightingPackets - 0x140 + bufferIndex * KWLN_FRAME_BUFFER_BYTES) = D_00435CD0;
        if (dds3RenderTex != 0) {
            SdfPoolNode *sceneNode;
            sdfDrawTag_t *sceneList;
            u64 *sceneFrustum;
            sdfCameraBuildProjection(&D_0037F7B0.camera);
            LookAtMatrix(kwlnHeldTextureTargetVector, kwlnHeldTextureEyeVector, kwlnHeldTextureUpVector);
            VU0_STORE_MATRIX_UNCLOBBERED(&kwlnHeldTextureViewMatrix);
            sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(kwlnHeldTextureMatrixPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
                &D_0037F7B0, &kwlnHeldTextureViewMatrix);
            sceneNode = &sdfGrOtTable[1];
            sceneNode->append(sceneNode,
                (sdfDrawTag_t *)(kwlnHeldTextureMatrixPackets - 0x190 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
            sceneList = dds3FontReqAllocDrawTag();
            sceneFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
            sdf3DMakeFogPacket((struct ConsFrustumPacket *)sceneFrustum, &kwlnHeldTextureFrustumParameters);
            sdfDrawTagLink(sceneList, (u32)sceneFrustum);
            sceneNode->append(sceneNode, sceneList);
        }
    }
    {
        SdfPoolNode *overlayNode;
        sdfDrawTag_t *overlayList;
        u64 *overlayFrustum;
        sdf3DMakeLightParallelPacket((SdfLightingPacketStorage *)(kwlnOverlayLightingPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
            kwlnOverlayLightSources, kwlnOverlayAmbientColor);
        sdfCameraBuildProjection(&D_0037F980.camera);
        LookAtMatrix(D_0037FA20, kwlnOverlayEyeVector, kwlnOverlayUpVector);
        VU0_STORE_MATRIX_UNCLOBBERED(&D_00384790);
        sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(kwlnOverlayMatrixPackets + bufferIndex * KWLN_FRAME_BUFFER_BYTES), &D_0037F980, &D_00384790);
        overlayNode = &sdfGrOtTable[85];
        overlayNode->append(overlayNode, (sdfDrawTag_t *)(kwlnOverlayMatrixPackets - 0x20 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
        overlayList = dds3FontReqAllocDrawTag();
        overlayFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket((struct ConsFrustumPacket *)overlayFrustum, &kwlnOverlayFrustumParameters);
        sdfDrawTagLink(overlayList, (u32)overlayFrustum);
        overlayNode->append(overlayNode, overlayList);

        sdf3DMakeLightParallelPacket((SdfLightingPacketStorage *)(kwlnOverlayMatrixPackets - 0x440 + bufferIndex * KWLN_FRAME_BUFFER_BYTES), D_0037F770, kwlnDefaultColorVector);
        *(u32 *)(kwlnOverlayMatrixPackets - 0x620 + bufferIndex * KWLN_FRAME_BUFFER_BYTES) = D_00435CBC;
    }
    sdfCameraBuildProjection(&dds3ScreenParam.camera);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    VU0_SCALAR_OP(600.0f, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(sdfViewShakeVector);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&eye);
    SDF_VU0_LOADVECTOR(sdfViewTargetVector);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&target);
    LookAtMatrix((const f32 *)&eye, (const f32 *)&target, aViewDown);
    VU0_STORE_MATRIX_UNCLOBBERED(&sdfViewMatrix);
    {
        SdfPoolNode *viewNode;
        sdfDrawTag_t *viewList;
        u64 *viewFrustum;
        sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(D_003820F0 + bufferIndex * KWLN_FRAME_BUFFER_BYTES),
            &dds3ScreenParam, &sdfViewMatrix);
        sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
        viewNode = &sdfGrOtTable[7];
        viewNode->append(viewNode, (sdfDrawTag_t *)(D_003820F0 - 0x250 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
        viewList = dds3FontReqAllocDrawTag();
        viewFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket((struct ConsFrustumPacket *)viewFrustum, &kwlnDrawVector);
        sdfDrawTagLink(viewList, (u32)viewFrustum);
        viewNode->append(viewNode, viewList);

        sdfPreDrawChainLink((SdfPacketChain *)D_00380860,
            (sdfPreDrawTag_t *)(D_003820F0 - 0x230 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
        providerSurfaces = &sdfGrOtTable[7];
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
        sdf3DSetDefaultLight((const struct SdfDmaTagHeader *)(D_003820F0 + bufferIndex * 0x1F40 + 0xD0));
        sdf3DSetDefaultFog((u32)viewFrustum);

    }
    {
        sdfDrawTag_t *stateList = dds3LinkZClearDrawTag(0);
        SdfPoolNode *const state0 = &sdfGrOtTable[81];
        state0->append(state0, stateList);
    }
    {
        sdfDrawTag_t *stateList = dds3LinkZClearDrawTag(1);
        SdfPoolNode *const state1 = &sdfGrOtTable[93];
        state1->append(state1, stateList);
    }
    {
        sdfDrawTag_t *stateList = dds3LinkZClearDrawTag(2);
        SdfPoolNode *const state2 = &sdfGrOtTable[95];
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

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);
extern void dds3DrawDistRipOverError(void);
extern void kwlnStepBackgroundFade(void);
extern void kwlnDrawBackgroundFadeBands(void);
extern void dds3FadeUpdate(void);
extern void kwlnQueueBackgroundFadeOverlayPacket(void);
extern void dds3BlurUpdate(void);
extern u32 dds3NullDrawProc(void);
extern void sdfGrFlushOtChain(u32 sourceAddress);
extern void sdfGrReqDraw(sdfDrawTag_t *, SdfPacketChain *);
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];
extern SdfPoolNode D_00380708;
extern u8 dds3ScreenBlurFlag;
extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern s32 D_00435CE0[2];
extern u8 D_00435BC8;
extern sdfDrawTag_t *D_00435C14;
extern u32 dds3DrawFlag;
extern s8 sdfGrFrameComplete;


/* Update draw controls, submit thirteen packet groups, optionally draw the
 * vignette, then flush pools. The skip bit suppresses one packet-slot publication. */
s32 kwlnRenderFrame(dds3ProcessID_t *task) {
    s32 bufferIndex = dds3GetFrameID();
    SdfPoolNode *(*groupHeads)[4];
    SdfDrawPacketGroups *packetGroups;
    sdfDrawTag_t *packetList;
    u64 *texturePacket;
    u64 *blendPacket;
    KwlnSpriteVertex *vertex;
    void *spritePacket;
    sdfDrawTag_t *poolHead;
    s32 edgeDistances[4];
    s32 i;

    dds3DrawDistRipOverError();
    kwlnStepBackgroundFade();
    kwlnDrawBackgroundFadeBands();
    dds3FadeUpdate();
    kwlnQueueBackgroundFadeOverlayPacket();
    dds3BlurUpdate();
    groupHeads = kwlnSceneSurfaceSets;
    packetGroups = D_00380870[bufferIndex].packetGroups;
    for (i = KWLN_FRAME_LAST_GROUP_INDEX; i >= 0; i--) {
        sdfModelDrawEnvLinkOt(*groupHeads, packetGroups);
        groupHeads++;
        packetGroups++;
    }
    if (dds3ScreenBlurFlag != 0 && dds3NullDrawProc() == 0) {
        packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(KWLN_FRAME_PACKET_LIST_BYTES);
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
        edgeDistances[0] = D_00435CE0[0] + KWLN_FRAME_HALF_WIDTH;
        edgeDistances[1] = D_00435CE0[1] + KWLN_FRAME_HALF_HEIGHT;
        edgeDistances[2] = KWLN_FRAME_HALF_WIDTH - D_00435CE0[0];
        edgeDistances[3] = KWLN_FRAME_HALF_HEIGHT - D_00435CE0[1];
        /* Signed Q12 scale; X and Y use distinct GS coordinate units below. */
        for (i = 0; i != 4; i++) {
            edgeDistances[i] = edgeDistances[i] * dds3ScreenBlurScale >> KWLN_FRAME_SCALE_FRACTION_BITS;
        }
        spritePacket = sdf2DAllocSprite(1);
        vertex = (KwlnSpriteVertex *)sdf2DGetPrimPtr(spritePacket);
        vertex->r = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->g = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->b = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->a = dds3ScreenBlurAlpha;
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
        D_00380708.append(&D_00380708, packetList);
    }
    poolHead = sdfFlushPoolNodes(sdfGrOtTable);
    D_00435C14 = poolHead;
    if (D_00435BC8 != 0) {
        sdfGrFlushOtChain(poolHead->first);
        D_00435BC8 = 0;
    }
    if (!(dds3DrawFlag & KWLN_FRAME_SKIP_POOL_QUEUE_BIT)) {
        sdfGrReqDraw(poolHead, (SdfPacketChain *)D_00380860);
    } else {
        dds3DrawFlag &= KWLN_FRAME_CLEAR_SKIP_POOL_QUEUE;
    }
    sdfGrFrameComplete = 0;
    return 0;
}

struct dds3Basic_t;
extern s32 dds3CallBasicMainProc(struct dds3Basic_t *holder);

/* Invoke the active world's first callback column; this task step returns zero. */
s32 dds3BasicMainProcess(dds3ProcessID_t *task) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

struct dds3Basic_t;
extern s32 dds3CallBasicDrawProc(struct dds3Basic_t *holder);

/* Invoke the active world's second callback column; this task step returns zero. */
s32 dds3BasicDrawProcess(dds3ProcessID_t *task) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001019F0", D_004110A8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_004110C8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_004110E8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411108);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411118);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411130);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411140);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411158);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411178);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411198);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C0C);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C14);
