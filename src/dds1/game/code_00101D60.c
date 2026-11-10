#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_column_packet_api.h"
#include "dds3_draw_buffer.h"
#include "dds3_frame_draw_buffer.h"
#include "sdf_packet_list.h"
#include "sdf_draw_packet_groups.h"
#include "dds3_frame_draw_bank.h"
#include "dds3_effect_sprite.h"
#include "dds3Admin.h"
#include "sdf_linked_packet.h"
#include "sdf_dev_start_load_api.h"
#include "kernel.h"
#include "draw3d.h"
#include "dds3_task_flags.h"
#include "sdf.h"
#include "sdf_pad_mode_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "sdf_packet_alloc.h"
#include "sdf_message.h"

struct dds3Basic_t;

extern struct dds3Basic_t *dds3TheWorld;

extern const s32 sdfDefaultCdMode;

#define DDS3_FRAME_POOL_NODE_COUNT 0x62

extern sdfGrOt_t sdfGrOtTable[DDS3_FRAME_POOL_NODE_COUNT];

extern u8 D_00325860[];

extern dds3FrameDrawBank_t dds3DrawEnv[2];

extern void sdfGrInitNewFrame(void);

extern void sdfGrOtInit(sdfGrOt_t *, s32);

extern void dds3InitializeFrameDrawPackets(s32 bufferIndex);

#define DDS3_FRAME_BUFFER_BYTES 0x1F40

#define DDS3_FRAME_LAST_GROUP_INDEX 12

#define DDS3_FRAME_PACKET_LIST_BYTES 0x20

#define DDS3_FRAME_GS_PACKET_BYTES 0x40

#define DDS3_FRAME_GIF_AD_REGISTER 0xE

#define DDS3_FRAME_GIF_SINGLE_REG_FIELD 0x10000000

#define DDS3_FRAME_GIF_TWO_WRITES_EOP 0x8002

#define DDS3_FRAME_GS_TEXA 0x3B

#define DDS3_FRAME_GS_TEXFLUSH 0x3F

#define DDS3_FRAME_GS_TEST_PRIMARY 0x47

#define DDS3_FRAME_GS_ALPHA_PRIMARY 0x42

#define DDS3_FRAME_NEUTRAL_COLOR 0x80

#define DDS3_FRAME_HALF_WIDTH 0x100

#define DDS3_FRAME_HALF_HEIGHT 0xE0

#define DDS3_FRAME_SCALE_FRACTION_BITS 12

#define DDS3_FRAME_X_UNITS_PER_PIXEL 16

#define DDS3_FRAME_Y_UNITS_PER_PIXEL 8

#define DDS3_FRAME_LEFT_BASE 0x7000

#define DDS3_FRAME_TOP_BASE 0x7900

#define DDS3_FRAME_RIGHT_BASE 0x9000

#define DDS3_FRAME_BOTTOM_BASE 0x8700

#define DDS3_FRAME_U_EXTENT 0x2000

#define DDS3_FRAME_V_EXTENT 0xE00

#define DDS3_FRAME_SKIP_POOL_QUEUE_BIT 0x2000000

#define DDS3_FRAME_CLEAR_SKIP_POOL_QUEUE 0xFDFFFFFF

/* Prepare the selected buffer's thirteen packet groups and link its draw sink.
 * The buffer index is sampled before waiting/selecting; keep that ordering. */
s32 dds3FirstProcess(dds3ProcessID_t *task) {
    s32 bufferIndex = dds3GetFrameID();
    s32 groupIndex;
    SdfDrawPacketGroups *packetGroups;
    sdfGrOt_t *drawSink;

    sdfGrInitNewFrame();
    sdfGrOtInit(sdfGrOtTable, DDS3_FRAME_POOL_NODE_COUNT);
    sdfPreDrawChainInit((SdfPacketChain *)D_00325860);
    dds3InitializeFrameDrawPackets(bufferIndex);
    packetGroups = dds3DrawEnv[bufferIndex].packetGroups;
    for (groupIndex = DDS3_FRAME_LAST_GROUP_INDEX; groupIndex >= 0; groupIndex--) {
        sdfModelSetDrawEnv(packetGroups);
        packetGroups++;
    }
    drawSink = sdfGrOtTable;
    drawSink->regFunc(drawSink, &dds3DrawEnv[bufferIndex].initialList);
    return 0;
}

struct ConsFrustumPacket;

extern u32 dds3RenderTex;

extern u32 dds3RenderDrawEnv;

extern u32 D_003BA8EC;

extern u8 dds3HeldTextureLightingPackets[];

extern u8 dds3HeldTextureMatrixPackets[];

extern u8 dds3OverlayLightingPackets[];

extern u8 dds3OverlayMatrixPackets[];

extern u8 D_003270F0[];

extern sdf3DLight_t *dds3RenderLightParallel[3];

extern sdf3DLight_t *dds3LightParam2[3];

extern sdf3DLight_t *dds3LightParam[3];

extern f32 dds3RenderAmbientColor[4];

extern f32 dds3AmbientColor2[4];

extern f32 dds3AmbientColor[4];

extern f32 dds3RenderTargetVector[4], dds3RenderEyeVector[4], effPCPBossTexRect[4];

extern f32 D_00324A20[4], fldAutoMapEye[4], fldAutoMapUp[4];

extern sdf3DFogParam_t dds3RenderFogParam, dds3FogParam2, dds3FogParamMain;

extern Matrix4 dds3RenderViewportMatrix;

extern Matrix4 D_00329790;

extern f32 aViewTarget[4], aViewEye[4], aViewDown[4];

extern u8 sdfViewShakeVector[];

extern Matrix4 sdfViewMatrix;

extern sdfGrOt_t *sdfModelOtTable[13][4];

extern void dds3RunQuake(void);

extern u32 evtCheckSelectionState(void);

extern void dds3SceneLightUpdateFirst(void);

extern void fldConfigureRenderCleanupMessage(void);

extern void sdf3DMakeFogPacket(struct ConsFrustumPacket *, sdf3DFogParam_t *);

extern void sdf3DSetDefaultLight(const struct SdfDmaTagHeader *lightingPacket);

extern void sdf3DSetDefaultFog(u32);

extern sdfDrawTag_t *dds3LinkZClearDrawTag(s32);

extern void eglSetOt(u32);

extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

/* Rebuild and queue the selected buffer's scene, overlay and view packets. */
s32 dds3DrawFirstProcess(dds3ProcessID_t *task) {
    s128 eye;
    s128 target;
    const u32 bufferIndex = dds3GetFrameID();
    sdfGrOt_t *providerSurfaces;

    dds3RunQuake();
    evtCheckSelectionState();
    dds3SceneLightUpdateFirst();
    if (dds3RenderTex != 0) {
        sdf3DMakeLightParallelPacket((sdf3DLightParallelPacket_t *)(dds3HeldTextureLightingPackets + bufferIndex * DDS3_FRAME_BUFFER_BYTES),
            dds3RenderLightParallel, dds3RenderAmbientColor);
        *(u32 *)(dds3HeldTextureLightingPackets - 0x140 + bufferIndex * DDS3_FRAME_BUFFER_BYTES) = dds3RenderDrawEnv;
        if (dds3RenderTex != 0) {
            sdfGrOt_t *sceneNode;
            sdfDrawTag_t *sceneList;
            u64 *sceneFrustum;
            sdf3DMakeScreenParam(&D_003247B0.camera);
            LookAtMatrix(dds3RenderTargetVector, dds3RenderEyeVector, effPCPBossTexRect);
            VU0_STORE_MATRIX_UNCLOBBERED(&dds3RenderViewportMatrix);
            sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(dds3HeldTextureMatrixPackets + bufferIndex * DDS3_FRAME_BUFFER_BYTES),
                &D_003247B0, &dds3RenderViewportMatrix);
            sceneNode = &sdfGrOtTable[1];
            sceneNode->regFunc(sceneNode,
                (sdfDrawTag_t *)(dds3HeldTextureMatrixPackets - 0x190 + bufferIndex * DDS3_FRAME_BUFFER_BYTES));
            sceneList = dds3FontReqAllocDrawTag();
            sceneFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
            sdf3DMakeFogPacket((struct ConsFrustumPacket *)sceneFrustum, &dds3RenderFogParam);
            sdfDrawTagLink(sceneList, (u32)sceneFrustum);
            sceneNode->regFunc(sceneNode, sceneList);
        }
    }
    {
        sdfGrOt_t *overlayNode;
        sdfDrawTag_t *overlayList;
        u64 *overlayFrustum;
        sdf3DMakeLightParallelPacket((sdf3DLightParallelPacket_t *)(dds3OverlayLightingPackets + bufferIndex * DDS3_FRAME_BUFFER_BYTES),
            dds3LightParam2, dds3AmbientColor2);
        sdf3DMakeScreenParam(&D_00324980.camera);
        LookAtMatrix(D_00324A20, fldAutoMapEye, fldAutoMapUp);
        VU0_STORE_MATRIX_UNCLOBBERED(&D_00329790);
        sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(dds3OverlayMatrixPackets + bufferIndex * DDS3_FRAME_BUFFER_BYTES), &D_00324980, &D_00329790);
        overlayNode = &sdfGrOtTable[85];
        overlayNode->regFunc(overlayNode, (sdfDrawTag_t *)(dds3OverlayMatrixPackets - 0x20 + bufferIndex * DDS3_FRAME_BUFFER_BYTES));
        overlayList = dds3FontReqAllocDrawTag();
        overlayFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket((struct ConsFrustumPacket *)overlayFrustum, &dds3FogParam2);
        sdfDrawTagLink(overlayList, (u32)overlayFrustum);
        overlayNode->regFunc(overlayNode, overlayList);

        sdf3DMakeLightParallelPacket((sdf3DLightParallelPacket_t *)(dds3OverlayMatrixPackets - 0x440 + bufferIndex * DDS3_FRAME_BUFFER_BYTES), dds3LightParam, dds3AmbientColor);
        *(u32 *)(dds3OverlayMatrixPackets - 0x620 + bufferIndex * DDS3_FRAME_BUFFER_BYTES) = D_003BA8EC;
    }
    sdf3DMakeScreenParam(&dds3ScreenParam.camera);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    VU0_SCALAR_OP(600.0f, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(sdfViewShakeVector);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&eye);
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(&target);
    LookAtMatrix((const f32 *)&eye, (const f32 *)&target, aViewDown);
    VU0_STORE_MATRIX_UNCLOBBERED(&sdfViewMatrix);
    {
        sdfGrOt_t *viewNode;
        sdfDrawTag_t *viewList;
        u64 *viewFrustum;
        sdf3DMakeViewportPacket((struct ConsMatrixPacket *)(D_003270F0 + bufferIndex * DDS3_FRAME_BUFFER_BYTES),
            &dds3ScreenParam, &sdfViewMatrix);
        sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
        viewNode = &sdfGrOtTable[7];
        viewNode->regFunc(viewNode, (sdfDrawTag_t *)(D_003270F0 - 0x250 + bufferIndex * DDS3_FRAME_BUFFER_BYTES));
        viewList = dds3FontReqAllocDrawTag();
        viewFrustum = (u64 *)sdfGrAllocPacketBuff(0x50);
        sdf3DMakeFogPacket((struct ConsFrustumPacket *)viewFrustum, &dds3FogParamMain);
        sdfDrawTagLink(viewList, (u32)viewFrustum);
        viewNode->regFunc(viewNode, viewList);

        sdfPreDrawChainLink((SdfPacketChain *)D_00325860,
            (sdfPreDrawTag_t *)(D_003270F0 - 0x230 + bufferIndex * DDS3_FRAME_BUFFER_BYTES));
        providerSurfaces = &sdfGrOtTable[7];
        sdfModelOtInit(sdfModelOtTable[0], &providerSurfaces[32], &providerSurfaces[33],
                                              &providerSurfaces[48], &providerSurfaces[49]);
        fldConfigureRenderCleanupMessage();
        sdfModelOtInit(sdfModelOtTable[8], &providerSurfaces[55], &providerSurfaces[56],
                                              &providerSurfaces[57], &providerSurfaces[58]);
        sdfModelOtInit(sdfModelOtTable[9], &providerSurfaces[67], &providerSurfaces[68],
                                              &providerSurfaces[69], &providerSurfaces[70]);
        sdfModelOtInit(sdfModelOtTable[1], &providerSurfaces[1], &providerSurfaces[2],
                                              &providerSurfaces[3], &providerSurfaces[4]);
        sdfModelOtInit(sdfModelOtTable[2], &providerSurfaces[6], &providerSurfaces[7],
                                              &providerSurfaces[8], &providerSurfaces[9]);
        sdfModelOtInit(sdfModelOtTable[3], &providerSurfaces[11], &providerSurfaces[12],
                                              &providerSurfaces[13], &providerSurfaces[14]);
        sdfModelOtInit(sdfModelOtTable[4], &providerSurfaces[16], &providerSurfaces[17],
                                              &providerSurfaces[18], &providerSurfaces[19]);
        sdfModelOtInit(sdfModelOtTable[5], &providerSurfaces[21], &providerSurfaces[22],
                                              &providerSurfaces[23], &providerSurfaces[24]);
        sdfModelOtInit(sdfModelOtTable[6], &providerSurfaces[37], &providerSurfaces[38],
                                              &providerSurfaces[39], &providerSurfaces[40]);
        sdfModelOtInit(sdfModelOtTable[10], &providerSurfaces[42], &providerSurfaces[43],
                                              &providerSurfaces[46], &providerSurfaces[47]);
        sdfModelOtInit(sdfModelOtTable[11], &providerSurfaces[79], &providerSurfaces[80],
                                              &providerSurfaces[81], &providerSurfaces[82]);
        sdfModelOtInit(sdfModelOtTable[12], &providerSurfaces[-5], &providerSurfaces[-4],
                                              &providerSurfaces[-3], &providerSurfaces[-2]);
        sdf3DSetDefaultLight((const struct SdfDmaTagHeader *)(D_003270F0 + bufferIndex * 0x1F40 + 0xD0));
        sdf3DSetDefaultFog((u32)viewFrustum);

    }
    {
        sdfDrawTag_t *stateList = dds3LinkZClearDrawTag(0);
        sdfGrOt_t *const state0 = &sdfGrOtTable[81];
        state0->regFunc(state0, stateList);
    }
    {
        sdfDrawTag_t *stateList = dds3LinkZClearDrawTag(1);
        sdfGrOt_t *const state1 = &sdfGrOtTable[93];
        state1->regFunc(state1, stateList);
    }
    {
        sdfDrawTag_t *stateList = dds3LinkZClearDrawTag(2);
        sdfGrOt_t *const state2 = &sdfGrOtTable[95];
        state2->regFunc(state2, stateList);
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

extern void dds3StepBackgroundFade(void);

extern void dds3DrawBackgroundFadeBands(void);

extern void dds3FadeUpdate(void);

extern void dds3DrawFade(void);

extern void dds3BlurUpdate(void);

extern u32 dds3NullDrawProc(void);

extern void sdfGrFlushOtChain(u32 sourceAddress);

extern void sdfGrReqDraw(sdfDrawTag_t *, SdfPacketChain *);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern sdfGrOt_t D_00325708;

extern u8 dds3ScreenBlurFlag;

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern s32 D_003BA910[2];

extern u8 D_003BA7FC;

extern sdfDrawTag_t *D_003BA844;

extern u32 dds3DrawFlag;

extern s8 sdfDrawPublishFlag;

/* Update draw controls, submit thirteen packet groups, optionally draw the
 * vignette, then flush pools. The skip bit suppresses one packet-slot publication. */
s32 dds3DrawProcess(dds3ProcessID_t *task) {
    s32 bufferIndex = dds3GetFrameID();
    sdfGrOt_t *(*groupHeads)[4];
    SdfDrawPacketGroups *packetGroups;
    sdfDrawTag_t *packetList;
    u64 *texturePacket;
    u64 *blendPacket;
    dds3EffectSpriteVertex_t *vertex;
    void *spritePacket;
    sdfDrawTag_t *poolHead;
    s32 edgeDistances[4];
    s32 i;

    dds3DrawDistRipOverError();
    dds3StepBackgroundFade();
    dds3DrawBackgroundFadeBands();
    dds3FadeUpdate();
    dds3DrawFade();
    dds3BlurUpdate();
    groupHeads = sdfModelOtTable;
    packetGroups = dds3DrawEnv[bufferIndex].packetGroups;
    for (i = DDS3_FRAME_LAST_GROUP_INDEX; i >= 0; i--) {
        sdfModelDrawEnvLinkOt(*groupHeads, packetGroups);
        groupHeads++;
        packetGroups++;
    }
    if (dds3ScreenBlurFlag != 0 && dds3NullDrawProc() == 0) {
        packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_FRAME_PACKET_LIST_BYTES);
        sdfDrawTagInit(packetList);
        sdfGrLinkDispFrameTex(packetList, (u32)(dds3FrameDrawPacketRecords[bufferIndex].baseBytes), (sdfGrRefPacketHeader_t *)sdfGrAllocPacketBuff(DDS3_FRAME_PACKET_LIST_BYTES));
        texturePacket = (u64 *)sdfGrAllocPacketBuff(DDS3_FRAME_GS_PACKET_BYTES);
        texturePacket[0] = 3;
        /* VIF FLUSHA, then DIRECT for the three following quadwords. */
        texturePacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        texturePacket[2] = ((u64)DDS3_FRAME_GIF_SINGLE_REG_FIELD << 32) | DDS3_FRAME_GIF_TWO_WRITES_EOP;
        texturePacket[3] = DDS3_FRAME_GIF_AD_REGISTER;
        texturePacket[4] = ((u64)DDS3_FRAME_NEUTRAL_COLOR << 32) | DDS3_FRAME_NEUTRAL_COLOR;
        texturePacket[5] = DDS3_FRAME_GS_TEXA;
        texturePacket[6] = 0;
        texturePacket[7] = DDS3_FRAME_GS_TEXFLUSH;
        sdfDrawTagLink(packetList, (u32)texturePacket);
        blendPacket = (u64 *)sdfGrAllocPacketBuff(DDS3_FRAME_GS_PACKET_BYTES);
        blendPacket[0] = 3;
        blendPacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        blendPacket[2] = ((u64)DDS3_FRAME_GIF_SINGLE_REG_FIELD << 32) | DDS3_FRAME_GIF_TWO_WRITES_EOP;
        blendPacket[3] = DDS3_FRAME_GIF_AD_REGISTER;
        blendPacket[4] = 0x31001;
        blendPacket[5] = DDS3_FRAME_GS_TEST_PRIMARY;
        blendPacket[6] = 0x44;
        blendPacket[7] = DDS3_FRAME_GS_ALPHA_PRIMARY;
        sdfDrawTagLink(packetList, (u32)blendPacket);
        edgeDistances[0] = D_003BA910[0] + DDS3_FRAME_HALF_WIDTH;
        edgeDistances[1] = D_003BA910[1] + DDS3_FRAME_HALF_HEIGHT;
        edgeDistances[2] = DDS3_FRAME_HALF_WIDTH - D_003BA910[0];
        edgeDistances[3] = DDS3_FRAME_HALF_HEIGHT - D_003BA910[1];
        /* Signed Q12 scale; X and Y use distinct GS coordinate units below. */
        for (i = 0; i != 4; i++) {
            edgeDistances[i] = edgeDistances[i] * dds3ScreenBlurScale >> DDS3_FRAME_SCALE_FRACTION_BITS;
        }
        spritePacket = sdf2DAllocSprite(1);
        vertex = (dds3EffectSpriteVertex_t *)sdf2DGetPrimPtr(spritePacket);
        vertex->r = DDS3_FRAME_NEUTRAL_COLOR;
        vertex->g = DDS3_FRAME_NEUTRAL_COLOR;
        vertex->b = DDS3_FRAME_NEUTRAL_COLOR;
        vertex->a = dds3ScreenBlurAlpha;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = DDS3_FRAME_LEFT_BASE - edgeDistances[0] * DDS3_FRAME_X_UNITS_PER_PIXEL;
        vertex->corner[0].y = DDS3_FRAME_TOP_BASE - edgeDistances[1] * DDS3_FRAME_Y_UNITS_PER_PIXEL;
        vertex->corner[0].mask = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[1].u = DDS3_FRAME_U_EXTENT;
        vertex->corner[1].v = DDS3_FRAME_V_EXTENT;
        vertex->corner[1].x = DDS3_FRAME_RIGHT_BASE + edgeDistances[2] * DDS3_FRAME_X_UNITS_PER_PIXEL;
        vertex->corner[1].y = DDS3_FRAME_BOTTOM_BASE + edgeDistances[3] * DDS3_FRAME_Y_UNITS_PER_PIXEL;
        vertex->corner[1].mask = 0;
        vertex->corner[1].flag = 0;
        sdfDrawTagLink(packetList, (u32)spritePacket);
        D_00325708.regFunc(&D_00325708, packetList);
    }
    poolHead = sdfGrOtClose(sdfGrOtTable);
    D_003BA844 = poolHead;
    if (D_003BA7FC != 0) {
        sdfGrFlushOtChain(poolHead->head);
        D_003BA7FC = 0;
    }
    if (!(dds3DrawFlag & DDS3_FRAME_SKIP_POOL_QUEUE_BIT)) {
        sdfGrReqDraw(poolHead, (SdfPacketChain *)D_00325860);
    } else {
        dds3DrawFlag &= DDS3_FRAME_CLEAR_SKIP_POOL_QUEUE;
    }
    sdfDrawPublishFlag = 0;
    return 0;
}

extern s32 dds3CallBasicMainProc(struct dds3Basic_t *holder);

/* Invoke the active world's first callback column; this task step returns zero. */
dds3TaskUpdateResult_t dds3BasicMainProcess(dds3ProcessID_t *task) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

extern s32 dds3CallBasicDrawProc(struct dds3Basic_t *holder);

/* Invoke the active world's second callback column; this task step returns zero. */
dds3TaskUpdateResult_t dds3BasicDrawProcess(dds3ProcessID_t *task) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_00101D60", D_003BA844);

