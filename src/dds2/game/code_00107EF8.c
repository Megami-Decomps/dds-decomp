#include "sdk_ps2.h"
#include "evt_sel_state.h"
#include "sdf_line_pair_packet_api.h"
#include "sdf_gs_header.h"
#include "sdf_dev_cons.h"
#include "sdf_dev_console_node.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "sdf_vu0.h"
#include "kwln_background_fade_api.h"
#include "dds3_admin_control_api.h"
#include "sdf_heap_overlay_block.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "evt_gradient_rect.h"
#include "sdf_textured_rect.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_packet_header.h"
#include "sdf_packet_append.h"
#include "fr_font.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "sdf_primitive_packet_api.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff_quaternion_nlerp.h"
#include "sdf_vu_matrix.h"
#include "sdf.h"
#include "sdf_asset_state.h"
#include "sdf_resource.h"
#include "sdf_general_heap_api.h"
#include "sdf_sif_command.h"
#include "scr.h"
#include "bf_flw0_header.h"
#include "kwln_task_lifecycle.h"
#include "brs_task.h"
#include "fld_resource_wait_api.h"
#include "itf.h"
/* Declared before first use (were implicit int). */
extern void btlCreateStageTestTask(void);
extern s32 btlExitWhenAudioAndTasksIdle(void);
extern void evtBattleStageTestStopTask(void);
extern void evtFreeStage(void);
extern void evtInitEventTest(void);
extern void evtViewerCreateTaskWithSky(void);
extern void fileMenuReleaseNoOp(void);
extern void fileMenuWorkCreate(u32 startBranchFlag);
extern void fileReleaseMenuResources(void);
extern void fldProcNextJump(void);
extern void fldProc_CallSisetuReturn(void);
extern void fldRunPendingSceneAction(void);
extern void fldShutdownSceneTasksAndWorld(void);
extern void fldStopLmapTask(void);
extern void fldStopSceneTasks(void);
extern void itfInitializeAdminInterfaceNoop(void);
extern void itfShutdownAdminInterfaceNoop(void);
extern void itfStartFontTestScene(void);
extern void mdlCreateViewerPackageTask(void);
extern u32 mdlStopViewerPackageTask(void);
extern void mnuCampDestroyPanelTasks(void);
extern void dds3ConfigProcessEnd(void);
extern void mnuCreateCampShopPopupTasks(const s32 *stockGroup);
extern void mnuCreateCampTasks(void);
extern void dds3ConfigProcessStart(s32 mode);
extern void InitMovieViewer(void);
extern void mnuCreateTerminalSceneTasks(s32 reduced, s32 slot);
extern void mnuDestroyCampTasks(void);
extern void FinishMovieViewer(void);
extern s32 mnuDestroyTitleMenuTask(void);
extern u32 mnuOpenTitleMenuForMode(s32 mode);
extern dds3ProcessID_t *mnuStaffCreateTasks(void);
extern u32 mnuStaffDestroyTasks(void);
extern u32 mnuStartStaffMovieRequest(void);
extern s32 mnuStopStaffTasks(void);
/* Original-style implicit int, kept: matched code depends on it: fldProcTrigSequence. */

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
    BF_FLW0_MAGIC = 0x30574C46
};

extern s32 mdlViewerPackageTaskExists(void);

extern u32 D_00435D28;

extern u32 egl_OT_Offset;

extern s32 fileMenuOperationSucceeded(void);

extern s32 fileMenuTaskExists(void);
extern void fileStartMemoryCardMenuForMode(s32 mode);

extern u32 D_00435BB0;

extern s32 mnuAcknowledgeCampState(void);

extern s32 checkConfigState(void);

extern s32 mnuCampConsumePanelTaskCompletion(void);

extern s32 fldPollSceneState(void);

extern s32 fldLmapTaskExists(void);

extern sdfDevConsCB_tag *evtConsoleFontResourceChain;

extern u32 dds3DrawFlag;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern u16 D_00438E06;

extern f32 D_00438E08;

extern f32 D_00438E0C;

extern f32 D_00438E10;

extern f32 D_00438E14;

extern u16 D_00438E04;

#include "sdf_asset_create.h"

extern void *D_00438DB0;


extern char D_00411370[];

extern void fldMainLoadStart(void);




extern char D_00435D20[];

extern void evtEventViewerDestroyTask(void);

extern char D_00411380[];

extern s32 D_00389780[];

extern void fldStartLmapTask(s32 mode);


extern u32 scrHandleCount;


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

extern KwlnDrawVectorParams kwlnDrawVector;

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

extern SdfPoolNode sdfGrOtTable[];



#include "sdf_packet_alloc.h"



extern void *sdf2DMakeStdPacket(void *, s32);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);


extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern KwlnFrameDrawBuffer kwlnFrameDrawPacketRecords[];

extern void sdfBuildMaskedFrameRegisterReference(const u8 *, SdfDmaReferenceChainPacket *, s32);


extern void sdfBuildFrameRegisterReference(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern f32 D_0037F5B0[][4];

extern u32 D_0037F5D0[];


extern s8 evtSelectionStateActive;

extern EvtSelState *evtSelectionState;

extern void evtSelStateDestroy(void);

extern void evtUpdateSelectionStateInterpolation(void);

extern u8 sdfViewShakeVector[];


extern s32 kwlnTaskNoopCallback(dds3ProcessID_t *task);

extern s32 evtPendingEventSelection;

extern void evtStartEvent(s32 eventId);

extern dds3ProcessID_t *evtInitSky(void);

extern void evtChkEndProcess(void);

extern void evtExitSky(void);


extern void btlStart(s32 mode, s32 pack, s32 encounter);


extern s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *);

extern void *D_00438E64;

extern f32 *D_0037F770[];
extern u16 D_00438DC8[4];
extern u16 D_00438DD0[4];
extern u32 D_0043E320[4];
extern u32 D_0043E330[4];

/* Set an existing light slot's RGB target immediately or save packed blend endpoints. */
void dds3SetLightColor(s32 blendFrames, s32 lightIndex, f32 *color) {
    u32 startColorWords[4];
    u32 targetColorWords[4];
    u32 packedStartColor;
    u32 packedTargetColor;
    f32 *light = D_0037F770[lightIndex];
    u32 blendFlag;

    if (light != NULL) {
        blendFlag = 4 << (lightIndex * 2);
        if (blendFrames == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(color);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(D_0037F770[lightIndex]);
        } else {
            D_00438DD0[lightIndex] = blendFrames;
            D_00438DC8[lightIndex] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(light);
            EE_MMI_RGBA_PACK(packedStartColor);
            startColorWords[0] = packedStartColor;
            D_0043E320[lightIndex] = startColorWords[0];
            SDF_VU0_LOADVECTOR(color);
            SDF_VU0_VECTOR_W_SET_ZERO();
            EE_MMI_RGBA_PACK_UNCLOBBERED(packedTargetColor);
            targetColorWords[0] = packedTargetColor;
            D_0043E330[lightIndex] = targetColorWords[0];
        }
    }
}

/* Normalize the requested light direction; replace it immediately or prepare its blend. */
extern u16 D_00438DD8[4];
extern u16 D_00438DE0[4];
extern f32 D_0043E340[][4];
extern f32 D_0043E370[][4];
extern void dds3RotationArc(void);

void dds3SetLightDir(s32 blendFrames, s32 lightIndex, f32 *direction) {
    u32 blendFlag;

    if (D_0037F770[lightIndex] != NULL) {
        blendFlag = 8 << (lightIndex * 2);
        if (blendFrames == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(D_0037F770[lightIndex] + 4);
        } else {
            D_00438DE0[lightIndex] = blendFrames;
            D_00438DD8[lightIndex] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(D_0037F770[lightIndex] + 4);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            /* No memory clobber here: retail recomputes the slot offset after the call. */
            SDF_VU0_STOREVECTOR_NOMEM(D_0043E340[lightIndex]);
            dds3RotationArc();
            SDF_VU0_STOREVECTOR(D_0043E370[lightIndex]);
        }
    }
}

extern u16 D_00438DE6;
extern u16 D_00438DE8;
extern u32 D_00438DEC;
extern u32 D_00438DF0;
extern u8 kwlnDefaultColorVector[];

/* vu0 routine: replace the background color immediately or blend over blendFrames frames. */
void dds3SetAmbientColor(s32 blendFrames, f32 *color) {
    s32 startColorWords[4];
    s32 targetColorWords[4];
    u32 packedStartColor;
    u32 packedTargetColor;

    if (blendFrames == 0) {
        dds3DrawFlag &= ~EVT_BACKGROUND_BLEND_FLAG;
        SDF_VU0_LOADVECTOR(color);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(kwlnDefaultColorVector);
    } else {
        dds3DrawFlag |= EVT_BACKGROUND_BLEND_FLAG;
        D_00438DE8 = blendFrames;
        D_00438DE6 = 0;
        SDF_VU0_LOADVECTOR(kwlnDefaultColorVector);
        EE_MMI_RGBA_PACK(packedStartColor);
        startColorWords[0] = packedStartColor;
        D_00438DEC = startColorWords[0];
        SDF_VU0_LOADVECTOR(color);
        SDF_VU0_VECTOR_W_SET_ONE();
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
void dds3SetFogColor(s32 blendFrames, f32 *color) {
    u32 targetColorWords[4];
    u32 packedTargetColor;

    SDF_VU0_LOADVECTOR(color);
    SDF_VU0_VECTOR_W_SET_ZERO();
    EE_MMI_RGBA_PACK_F255(packedTargetColor);
    targetColorWords[0] = packedTargetColor;
    if (blendFrames == 0) {
        dds3DrawFlag &= ~EVT_DRAW_COLOR_BLEND_FLAG;
        D_0037F7A0[0] = targetColorWords[0];
    } else {
        dds3DrawFlag |= EVT_DRAW_COLOR_BLEND_FLAG;
        D_00438DF6 = blendFrames;
        D_00438E00 = targetColorWords[0];
        D_00438DFC = D_0037F7A0[0];
        D_00438DF4 = 0;
    }
}


/* Either replace the draw vector immediately or interpolate from its prior value. */
void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w) {
    if (blendFrames == 0) {
        dds3DrawFlag &= ~EVT_DRAW_VECTOR_BLEND_FLAG;
        kwlnDrawVector.fogNear = x;
        kwlnDrawVector.fogFar = y;
        kwlnDrawVector.depthNear = z;
        kwlnDrawVector.depthFar = w;
    } else {
        f32 previousX = kwlnDrawVector.fogNear;
        f32 previousY = kwlnDrawVector.fogFar;
        f32 previousZ = kwlnDrawVector.depthNear;
        f32 previousW = kwlnDrawVector.depthFar;
        dds3DrawFlag |= EVT_DRAW_VECTOR_BLEND_FLAG;
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

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* The color-blend site passes ratio and the 128.0f pack scale as ordinary GPR operands
   (VU0_SCALE_VF / eePackRGBAWord), so their SF->GPR reloads advance the spill ring
   ($2,$3,$4) and the vector-blend site starts at $4 (cc1probe --reload, DDS1 uid650). */
void dds3SceneLightUpdateFirst(void) {
    s32 i;
    u32 mask;
    f32 ratio;

    for (i = 0, mask = 4; i < 3; i++, mask <<= 2) {
        if (D_0037F770[i] == NULL) {
            dds3DrawFlag &= ~(mask | (mask << 1));
        } else {
            if (dds3DrawFlag & mask) {
                ++D_00438DC8[i];
                ratio = (f32)D_00438DC8[i] / D_00438DD0[i];
                eeUnpackRGBAWordFloatReadOnly(D_0043E320[i], 0.0078125f);
                VU0_SCALE_VF(vf10, 1.0f - ratio);
                SDF_VU0_COPYVECTOR_A_TO_B();
                eeUnpackRGBAWordFloatReadOnly(D_0043E330[i], 0.0078125f);
                VU0_SCALAR_OP(ratio, "vmulx.xyzw vf10, vf10, vf2x");
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(D_0037F770[i]);
                if (D_00438DC8[i] >= D_00438DD0[i]) {
                    dds3DrawFlag &= ~mask;
                }
            }
            if (dds3DrawFlag & (mask << 1)) {
                ++D_00438DD8[i];
                ratio = (f32)D_00438DD8[i] / D_00438DE0[i];
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_VECTOR_W_SET_ONE();
                SDF_VU0_LOADVECTOR_B(&D_0043E370[i]);
                Lerp(ratio);
                effMiscQuaternionToMatrixVU();
                SDF_VU0_LOADVECTOR(&D_0043E340[i]);
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_STOREVECTOR_NOMEM(D_0037F770[i] + 4);
                if (D_00438DD8[i] >= D_00438DE0[i]) {
                    dds3DrawFlag &= ~(mask << 1);
                }
            }
        }
    }
    if (dds3DrawFlag & EVT_BACKGROUND_BLEND_FLAG) {
        ++D_00438DE6;
        ratio = (f32)D_00438DE6 / D_00438DE8;
        eeUnpackRGBAWordFloatReadOnly(D_00438DEC, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        eeUnpackRGBAWordFloatReadOnly(D_00438DF0, 0.0078125f);
        VU0_SCALAR_OP(ratio, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR_NOMEM(kwlnDefaultColorVector);
        if (D_00438DE6 >= D_00438DE8) {
            dds3DrawFlag &= ~EVT_BACKGROUND_BLEND_FLAG;
        }
    }
    if (dds3DrawFlag & EVT_DRAW_COLOR_BLEND_FLAG) {
        ++D_00438DF4;
        ratio = (f32)D_00438DF4 / D_00438DF6;
        eeUnpackRGBAWordFloatReadOnly(D_00438DFC, 0.0078125f);
        VU0_SCALE_VF(vf10, 1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        eeUnpackRGBAWordFloatReadOnly(D_00438E00, 0.0078125f);
        VU0_SCALE_VF(vf10, ratio);
        SDF_VU0_ADDVECTOR();
        D_0037F7A0[0] = eePackRGBAWord(128.0f);
        if (D_00438DF4 >= D_00438DF6) {
            dds3DrawFlag &= ~EVT_DRAW_COLOR_BLEND_FLAG;
        }
    }
    if (dds3DrawFlag & EVT_DRAW_VECTOR_BLEND_FLAG) {
        ++D_00438DF8;
        ratio = (f32)D_00438DF8 / D_00438DFA;
        SDF_VU0_LOADVECTOR(&D_0043E3A0);
        SDF_VU0_LOADVECTOR_B(&D_0043E3B0);
        VU0_SCALE_VF(vf10, 1.0f - ratio);
        VU0_SCALAR_OP(ratio, "vmulx.xyzw vf11, vf11, vf2x");
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(kwlnDrawVector.fogNear);
        SDF_VU0_GETVECTOR_Y(kwlnDrawVector.fogFar);
        SDF_VU0_GETVECTOR_Z(kwlnDrawVector.depthNear);
        SDF_VU0_GETVECTOR_W(kwlnDrawVector.depthFar);
        if (D_00438DF8 >= D_00438DFA) {
            dds3DrawFlag &= ~EVT_DRAW_VECTOR_BLEND_FLAG;
        }
    }
    if (dds3DrawFlag & EVT_SAVED_VECTOR_BLEND_FLAG) {
        ++D_00438E04;
        ratio = (f32)D_00438E04 / D_00438E06;
        sdfFaceDivSoften = D_00438E08 * (1.0f - ratio) + D_00438E0C * ratio;
        sdfFaceDivBias = D_00438E10 * (1.0f - ratio) + D_00438E14 * ratio;
        if (D_00438E04 >= D_00438E06) {
            dds3DrawFlag &= ~EVT_SAVED_VECTOR_BLEND_FLAG;
        }
    }
}

/* Replace the two scalar targets immediately, or retain start/end values for their blend. */
void dds3SetSoftenBias(s32 frames, f32 firstTarget, f32 secondTarget) {
    if (frames == 0) {
        dds3DrawFlag &= ~EVT_SAVED_VECTOR_BLEND_FLAG;
        sdfFaceDivSoften = firstTarget;
        sdfFaceDivBias = secondTarget;
    }
    else {
        f32 previousFirst = sdfFaceDivSoften;
        f32 previousSecond = sdfFaceDivBias;
        dds3DrawFlag |= EVT_SAVED_VECTOR_BLEND_FLAG;
        D_00438E06 = frames;
        D_00438E08 = previousFirst;
        D_00438E0C = firstTarget;
        D_00438E10 = previousSecond;
        D_00438E14 = secondTarget;
        D_00438E04 = 0;
    }
}

void eglInit(void) {
    if (D_00438DB0 == NULL) {
        D_00438DB0 = sdfMatNew();
        ((EvtDrawState *)D_00438DB0)->value1C = 1.0f;
    }
}

void eglSetOt(u32 surfaceIndex) {
    egl_OT_Offset = surfaceIndex;
}

/* Submit primary-context GS TEST settings. Z testing is always enabled;
   unusedZte is a retained native formal, not the source of the ZTE bit. */
void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdf2DMakeStdPacket(packet, EVT_GS_COMMAND_BYTES);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = EVT_GS_TEST_PRIMARY;
    sdfDrawTagLink(list, (u32)packet);
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}

/* Submit the same TEST bit layout for the secondary GS context, with ZTE forced on. */
void evtSubmitSecondaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst) {
    void *list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdf2DMakeStdPacket(packet, EVT_GS_COMMAND_BYTES);
    command->data = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    command->registerId = EVT_GS_TEST_SECONDARY;
    sdfDrawTagLink(list, (u32)packet);
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}

/* Select a primary-context ALPHA equation by mode; unknown modes use 0x44. */
void evtSubmitPrimaryAlphaBlendMode(s32 blendMode) {
    void *list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;

    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdf2DMakeStdPacket(packet, EVT_GS_COMMAND_BYTES);
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
    sdfDrawTagLink(list, (u32)packet);
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha) {
    void *list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    void *packet;
    EvtGsCommand *command;
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(EVT_GS_COMMAND_BYTES);
    command = (EvtGsCommand *)sdf2DMakeStdPacket(packet, EVT_GS_COMMAND_BYTES);
    command->data = ((u64)fixedAlpha << 32) | EVT_FIXED_ALPHA_INTERPOLATION;
    command->registerId = EVT_GS_ALPHA_PRIMARY;
    sdfDrawTagLink(list, (u32)packet);
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}

void evtSubmitPrimaryFramePacket(void) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    SdfDmaReferenceChainPacket *framePacket;
    sdfDrawTagInit(list);
    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(EVT_FRAME_REFERENCE_PACKET_BYTES);
    sdfBuildMaskedFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}

void evtSubmitAlternateFramePacket(void) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    SdfDmaReferenceChainPacket *framePacket;
    sdfDrawTagInit(list);
    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(EVT_FRAME_REFERENCE_PACKET_BYTES);
    sdfBuildFrameRegisterReference(kwlnFrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}

void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    SdfPoolNode *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, texture, SDF_GS_CONTEXT_1);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (width << 4) + 0x7000;
    bottom = top + (height << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, color0,
        right, top, uRight, vTop, color1,
        left, bottom, uLeft, vBottom, color3,
        right, bottom, uRight, vBottom, color2,
        -1, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->append(surface, list);
}

INCLUDE_ASM(const s32, "game/code_00107EF8", evtSubmitTexturedGradientQuadPoints);

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
void evtSubmitGradientRectAtDepth(s32 x, s32 y, s32 w, s32 h, u32 depth, s32 color0, s32 color1, s32 color2, s32 color3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    SdfPoolNode *descriptor;

    u32 r0 = color0 & 0xFF;
    u32 g0 = (color0 >> 8) & 0xFF;
    u32 b0 = (color0 >> 16) & 0xFF;
    u32 a0 = (u32)color0 >> 24;
    u32 r1 = color1 & 0xFF;
    u32 g1 = (color1 >> 8) & 0xFF;
    u32 b1 = (color1 >> 16) & 0xFF;
    u32 a1 = (u32)color1 >> 24;
    u32 r2 = color2 & 0xFF;
    u32 g2 = (color2 >> 8) & 0xFF;
    u32 b2 = (color2 >> 16) & 0xFF;
    u32 a2 = (u32)color2 >> 24;
    u32 r3 = color3 & 0xFF;
    u32 g3 = (color3 >> 8) & 0xFF;
    u32 b3 = (color3 >> 16) & 0xFF;
    u32 a3 = (u32)color3 >> 24;

    coords[0] = x * 16;
    coords[1] = y * 8;
    coords[2] = (x + w) * 16;
    coords[3] = y * 8;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 8;
    coords[6] = x * 16;
    coords[7] = (y + h) * 8;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = depth;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    descriptor->append(descriptor, command);
}

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor) {
    evtSubmitGradientRectAtDepth(x, y, width, height, EVT_QUAD_DEFAULT_DEPTH, topLeftColor, topRightColor, bottomRightColor, bottomLeftColor);
}

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 color0, s32 color1, s32 color2) {
    s32 coords[3][2];
    sdfDrawTag_t *command;
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;
    SdfPoolNode *descriptor;

    u32 r0 = color0 & 0xFF;
    u32 g0 = (color0 >> 8) & 0xFF;
    u32 b0 = (color0 >> 16) & 0xFF;
    u32 a0 = (u32)color0 >> 24;
    u32 r1 = color1 & 0xFF;
    u32 g1 = (color1 >> 8) & 0xFF;
    u32 b1 = (color1 >> 16) & 0xFF;
    u32 a1 = (u32)color1 >> 24;
    u32 r2 = color2 & 0xFF;
    u32 g2 = (color2 >> 8) & 0xFF;
    u32 b2 = (color2 >> 16) & 0xFF;
    u32 a2 = (u32)color2 >> 24;
    coords[0][0] = x0 * 16;
    coords[0][1] = y0 * 8;
    coords[1][0] = x1 * 16;
    coords[1][1] = y1 * 8;
    coords[2][0] = x2 * 16;
    coords[2][1] = y2 * 8;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    dst = (u64 *)sdf2DGetPrimPtr(packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(coords[i][0] + 0x7000) |
            ((u64)(coords[i][1] + 0x7900) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, (u32)packet);
    descriptor = &sdfGrOtTable[egl_OT_Offset];
    descriptor->append(descriptor, command);
}



extern u64 D_0037F5A0[]; /* index table; only the first 8 bytes are used */

/* Preserve the native 0,2,3,1 vertex/index order; W components are not initialized here. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 i0, u32 i1, u32 i2, u32 i3) {
    SdfPrimitiveRequest desc;
    DrawVec4 verts[EVT_QUAD_VERTEX_COUNT];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    void *list;
    SdfPoolNode *surface;

    list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, D_00438DB0, 0);
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
    sdfDrawTagLink(list, (u32)sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->append(surface, list);
}

/* Submit the same native vertex permutation with rectangular UVs, then queue asset release. */
void eglLinkTexPoly4(s32 i0, f32 x0, f32 y0, f32 z0, s32 i1, f32 x1, f32 y1, f32 z1, s32 i2, f32 x2, f32 y2, f32 z2, s32 i3, f32 x3, f32 y3, f32 z3, u32 bits, f32 u0, f32 v0, f32 u1, f32 v1) {
    SdfPrimitiveRequest desc;
    f32 verts[16];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    f32 uvs[8];
    void *asset;
    void *list;
    SdfPoolNode *surface;

    asset = sdfMatNew();
    sdfSetAssetPrimaryTextureAddress(asset, (sdfTexHandle_t *)bits);
    list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, asset, 0);
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
    sdfDrawTagLink(list, (u32)sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->append(surface, list);
    sdfMatFreeDelayed((sdfMat_t *)asset);
}

void eglLinkLine(f32 x, f32 y, f32 z, f32 u, f32 v, f32 w, u32 first, u32 second) {
    void *list;
    D_0037F5B0[0][0] = x;
    D_0037F5B0[0][1] = y;
    D_0037F5B0[0][2] = z;
    D_0037F5B0[1][0] = u;
    D_0037F5B0[1][1] = v;
    D_0037F5B0[1][2] = w;
    D_0037F5D0[1] = second;
    D_0037F5D0[0] = first;
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (u32)effDebugDrawWireModel(D_0037F5B0, D_0037F5D0, 2, 0x80));
    {
        SdfPoolNode *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->append(surface, list);
    }
}



void eglLinkPrint(s32 x, s32 y, s32 packetArg, s32 drawArg) {
    SifCommand sifParameters;
    void *list;
    void *packet;
    SdfPoolNode *surface;
    list = dds3FontReqAllocDrawTag();
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfPktInit(&sifParameters, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, packetArg);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&sifParameters, (const char *)drawArg));
    surface = &kwlnPositionedTextSurface;
    surface->append(surface, list);
}

void eglLinkKanji(s32 width, s32 height, s32 colors, const char *text) {
    struct _FRQ *glyph;

    glyph = itfCreateMeasuredTextGlyph(width << 4, height << 3, 0, colors, text, NULL);
    frFontDrawGlyphInDefaultMode(glyph);
    frDestroyFontRequestAfter(glyph);
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
    evtSelectionState->rampUpFrames = firstValue;
    evtSelectionState->rampDownFrames = secondValue;
    evtSelectionState->amplitude[0] = frames;
    evtSelectionState->amplitude[1] = frames;
    evtSelectionState->amplitude[2] = frames;
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
        evtUpdateSelectionStateInterpolation();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUpdateSelectionStateInterpolation);

void evtDestroySelectionState(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (evtSelectionStateActive != 0) {
        sdfChipFreeDirect(evtSelectionState);
        evtSelectionStateActive = 0;
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_STOREVECTOR(sdfViewShakeVector);
    }
}

dds3ProcessID_t *CallDebug(void) {
    return dds3StartProcess(D_00411370, 0x2AF9, 0, 0, kwlnTaskNoopCallback, 0, 0);
}

u32 ExitDebug(void) {
    dds3KillProcessName(D_00411370, 1);
    return 1;
}

/* Admin mode 0 cleanup: zero leaves the mode pending. */
u32 CheckDebug(void) {
    return 0;
}

void CallBootMc(void) {
    fileStartMemoryCardMenuForMode(3);
}

u32 ExitBootMc(void) {
    s32 operationResult;
    s32 status;

    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    operationResult = fileMenuOperationSucceeded();
    status = -1;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u8 CheckBootMc(void) {
    s32 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

u32 CallTitle(u32 unused, u32 *modeOverride) {
    if (modeOverride == 0) {
        return mnuOpenTitleMenuForMode(0);
    }
    return mnuOpenTitleMenuForMode(*modeOverride);
}

u32 ExitTitle(void) {
    mnuDestroyTitleMenuTask();
    return 0;
}

/* Admin mode 2 cleanup: zero leaves the mode pending. */
u32 CheckTitle(void) {
    return 0;
}

void CallLoad(void) {
    fileStartMemoryCardMenuForMode(1);
}

s32 ExitLoad(void) {
    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    if (fileMenuOperationSucceeded() != 0) {
        fldMainLoadStart();
        return 0;
    }
    return -1;
}

u8 CheckLoad_dds3SequenceList(void) {
    s32 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

u32 CallStaff(void) {
    return mnuStartStaffMovieRequest();
}

u32 ExitStaff(void) {
    mnuStopStaffTasks();
    return 0;
}

/* Admin mode 12 cleanup: zero leaves the mode pending. */
u32 CheckStaff(void) {
    return 0;
}

void CallEndSave(void) {
    fileStartMemoryCardMenuForMode(2);
}

u32 ExitEndSave(void) {
    s32 operationResult;
    s32 status;

    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    operationResult = fileMenuOperationSucceeded();
    status = -1;
    if (operationResult != 0) {
        status = 0;
    }
    return status;
}

u8 CheckEndSave(void) {
    s32 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

struct fldSceneParam_t;
extern void fldProcTrigSequence(struct fldSceneParam_t *record, u32 mode);

void evtStartAreaFromSelection(u32 selection, u32 area) {
    kwlnFadeBackgroundStartOut(0);
    fldProcTrigSequence(area, selection);
}

u32 ExitField(void) {
    FldResourceWaitResult operationResult;
    s32 status;

    fldShutdownSceneTasksAndWorld();
    operationResult = fldIsFieldResourceWaitFinished();
    status = -1;
    if (operationResult != FLD_RESOURCE_WAIT_PENDING) {
        status = 0;
    }
    return status;
}

/* Admin mode 5 cleanup: zero leaves the mode pending. */
u32 CheckField(void) {
    return 0;
}

void evtStartSelectionFadeOut(void) {
    kwlnFadeBackgroundStartOut(0);
}

/* Admin mode 11 destroy callback: no work, zero transition result. */
u32 evtDestroySelectionFadeOutNoOp(void) {
    return 0;
}

/* Admin mode 11 cleanup: zero leaves the mode pending. */
u32 evtKeepSelectionFadeOutPending(void) {
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
    evtInitSky();
    evtStartEvent(eventSelection);
}

u32 evtCloseSkyEventTask(void) {
    D_00435BB0 = 1;
    evtExitSky();
    fldProcNextJump();
    return 0;
}

/* Admin mode 6 cleanup: zero leaves the mode pending. */
u32 evtKeepSkyEventModePending(void) {
    return 0;
}

void evtCreateCampTasks(void) {
    mnuCreateCampTasks();
}

u32 evtDestroyCampTasks(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 evtWaitCampStateAcknowledged(void) {
    s32 state;

    state = mnuAcknowledgeCampState();
    return state == 0;
}

/* Admin mode 8 entry intentionally performs no work. */
void CallRecover(void) {
}

/* Admin mode 8 destroy callback: no work, zero transition result. */
u32 ExitRecover(void) {
    return 0;
}

/* Admin mode 8 cleanup: one immediately reports completion. */
u32 CheckRecover(void) {
    return 1;
}

/* Admin mode 9 entry intentionally performs no work. */
void CallTerminal(void) {
}

/* Admin mode 9 destroy: no work, zero transition result. */
u32 ExitTerminal(void) {
    return 0;
}

/* Admin mode 9 cleanup: one immediately reports completion. */
u32 CheckTerminal(void) {
    return 1;
}

s32 CallResult2(void) {
    return (s32)mnuStaffCreateTasks();
}

u32 ExitResult2(void) {
    fldProcNextJump();
    mnuStaffDestroyTasks();
    return 0;
}

u8 CheckResult2(void) {
    s32 done;

    done = brsTaskConsumeDone();
    return done == 0;
}

void CallConfig(void) {
    dds3ConfigProcessStart(1);
}

u32 ExitConfig(void) {
    dds3ConfigProcessEnd();
    return 0;
}

u8 CheckConfig(void) {
    s32 result;

    result = checkConfigState();
    return result == 0;
}

/* Source zero forwards an optional parameter pair; other sources resume deferred field work. */
void evtDispatchSelectionCommand(s32 commandSource, s32 *commandParams) {
    if (commandSource == 0) {
        if (commandParams == NULL) {
            btlStart(1, 0, 0);
        }
        else {
            btlStart(0, commandParams[0], commandParams[1]);
        }
    }
    else {
        dds3AdminiExitSequence();
        fldProcNextJump();
    }
}

/* Check battle shutdown; the callee exits only after audio and task work become idle. */
s32 evtCheckBattleShutdown(void) {
    return btlExitWhenAudioAndTasksIdle();
}

u32 CheckNewBattleTest(void) {
    if (dds3SearchProcessName(D_00435D20) != NULL) {
        return 0;
    }
    if (dds3SearchProcessPriority(0x3FA) == NULL) {
        fldProcNextJump();
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

/* Admin mode 15 cleanup: zero leaves the mode pending. */
u32 evtKeepBattleStageTestModePending(void) {
    return 0;
}

dds3ProcessID_t *evtCreateTestAndSkyTasks(void) {
    D_00435BB0 = 0;
    D_00435D28 = 0;
    evtInitEventTest();
    return evtInitSky();
}

u32 evtInitializeSelectionScene(void) {
    D_00435BB0 = 1;
    kwlnFadeBackgroundStartOut(0);
    evtChkEndProcess();
    evtExitSky();
    return 1;
}

/* Admin mode 16 cleanup: zero leaves the mode pending. */
u32 evtKeepTestSkyModePending(void) {
    return 0;
}

void evtClearSecondaryWorldAndViewer(void) {
    D_00435BB0 = 0;
    evtFreeStage();
    evtViewerCreateTaskWithSky();
}

u32 evtInitializeSelectionScreen(void) {
    evtEventViewerDestroyTask();
    D_00435BB0 = 1;
    kwlnFadeBackgroundStartOut(0);
    return 0;
}

/* Admin mode 17 cleanup: zero leaves the mode pending. */
u32 evtKeepEventViewerModePending(void) {
    return 0;
}

/* Admin mode 18 entry intentionally performs no work. */
void evtAdminModeEighteenEntryNoOp(void) {
}

/* Admin mode 18 destroy: no work, zero transition result. */
u32 evtAdminModeEighteenDestroyNoOp(void) {
    return 0;
}

/* Admin mode 18 cleanup: zero leaves the mode pending. */
u32 evtKeepAdminModeEighteenPending(void) {
    return 0;
}

void evtCreateCampTasksAlternate(void) {
    mnuCreateCampTasks();
}

u32 evtExitCampTaskGroup(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 mnuCheckCampStateAndAcknowledge(void) {
    s32 state;

    state = mnuAcknowledgeCampState();
    return state == 0;
}

void evtCreateSkyAndCampTasks(u32 unused, u32 campMode) {
    evtInitSky();
    mnuCreateCampShopPopupTasks(campMode);
}

u32 evtDestroySkyAndCampTasks(void) {
    evtExitSky();
    mnuCampDestroyPanelTasks();
    fldProc_CallSisetuReturn();
    return 0;
}

u8 CheckRecoverTest(void) {
    s32 result;

    result = mnuCampConsumePanelTaskCompletion();
    return result == 0;
}

void CallTerminalTest(u32 unused, u32 *fieldArgs) {
    evtInitSky();
    if (fieldArgs != 0) {
        mnuCreateTerminalSceneTasks(fieldArgs[0], fieldArgs[1]);
        return;
    }
    mnuCreateTerminalSceneTasks(0, 1);
}

u32 ExitTerminalTest(void) {
    evtExitSky();
    fldStopSceneTasks();
    return 0;
}

u8 CheckTerminalTest(void) {
    s32 sceneState;

    sceneState = fldPollSceneState();
    return sceneState == 0;
}

s32 evtCreateStaffTasksAlternate(void) {
    return (s32)mnuStaffCreateTasks();
}

u32 evtDestroyStaffTasksAlternate(void) {
    mnuStaffDestroyTasks();
    return 0;
}

u8 evtWaitStaffTaskDoneAlternate(void) {
    s32 done;

    done = brsTaskConsumeDone();
    return done == 0;
}

/* Admin mode 23 entry intentionally performs no work. */
void CallSoundTest(void) {
}

/* Admin mode 23 destroy: no work, zero transition result. */
u32 ExitSoundTest(void) {
    return 0;
}

/* Admin mode 23 cleanup: zero leaves the mode pending. */
u32 CheckSoundTest(void) {
    return 0;
}

/* Admin mode 24 entry intentionally performs no work. */
void CallBillboardTest(void) {
}

/* Admin mode 24 destroy: no work, zero transition result. */
u32 ExitBillboardTest(void) {
    return 0;
}

/* Admin mode 24 cleanup: zero leaves the mode pending. */
u32 CheckBillboardTest(void) {
    return 0;
}

void CallFontTest(void) {
    itfStartFontTestScene();
}

/* Admin mode 25 destroy callback: no work, zero transition result. */
u32 ExitFontTest(void) {
    return 0;
}

u32 CheckFontTest(void) {
    return dds3SearchProcessName(D_00411380) == NULL;
}

void evtCreateMovieViewerTask(void) {
    InitMovieViewer();
}

u32 evtDestroyMovieViewerTask(void) {
    FinishMovieViewer();
    return 0;
}

/* Admin mode 26 cleanup: zero leaves the mode pending. */
u32 evtKeepMovieViewerModePending(void) {
    return 0;
}

void evtEnterInterfaceMode(void) {
    itfInitializeAdminInterfaceNoop();
}

u32 evtDestroyInterfaceMode(void) {
    itfShutdownAdminInterfaceNoop();
    return 0;
}

u32 evtKeepInterfaceModePending(void) {
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
    s32 taskExists;

    taskExists = fldLmapTaskExists();
    return taskExists == 0;
}

/* Start the shooting-game task: the callee allocates MnuShootingWork and starts "stgProcess". */
void evtStartShootingGameTask(void) {
    mdlCreateViewerPackageTask();
}

/* Destroy that shooting-game task hierarchy and invoke its work callback; return native zero status. */
u32 evtStopShootingGameTask(void) {
    mdlStopViewerPackageTask();
    return 0;
}

/* Complete the wait only after the named "stgProcess" task is absent. */
u8 evtWaitShootingGameTaskGone(void) {
    s32 taskExists;

    taskExists = mdlViewerPackageTaskExists();
    return taskExists == 0;
}

void evtOpenFileMenuFromParams(u32 unused, u32 *menuArgs) {
    u32 menuArg;

    menuArg = 0;
    if (menuArgs != 0) {
        menuArg = *menuArgs;
    }
    fileMenuWorkCreate(menuArg);
    fileStartMemoryCardMenuForMode(4);
}

extern void fileReleaseMenuFlowResource();

s32 evtPollFileMenuAndRelease(void) {
    fileMenuReleaseNoOp();
    fileReleaseMenuResources();
    if (fileMenuOperationSucceeded() == 0) {
        return -1;
    }
    fileReleaseMenuFlowResource();
    return 0;
}

u8 evtWaitFileMenuTaskFour(void) {
    s32 operationResult;

    operationResult = fileMenuTaskExists();
    return operationResult == 0;
}

u32 evtEnsureFontResourceChain(void) {
    if (evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsOpen(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetMode(evtConsoleFontResourceChain, 2);
        sdfDevConsAttribute(evtConsoleFontResourceChain, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (evtConsoleFontResourceChain != 0) {
        sdfDevConsClose(evtConsoleFontResourceChain);
        evtConsoleFontResourceChain = 0;
    }
}

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
        vsprintf(messageBuffer, format, formatArgs);
        sdfDevConsPrintf(evtConsoleFontResourceChain, "%s", messageBuffer);
    }
}

extern u16 D_00435D40;
extern u16 D_00435D42;
extern char evtGeneralHeapPercentFormat[];

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task) {
    SdfGeneralHeapStats heapStats;
    sdfDrawTag_t *list;
    void *packet;
    s32 freePercent;
    s32 originX = 0x7100;
    s32 currentColumn;
    s32 peakColumn;

    sdfMemGetInfo(&heapStats);
    freePercent = heapStats.freeBytes * 100 / heapStats.totalBytes;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70D0, 0x85A8, 0xFFFF7F, 0x1260, 0xC0, 0x20000000, 0x40806040));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x85C0, 0xFFFF80, 0, evtGeneralHeapPercentFormat, freePercent));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8080, 0x85C0, 0xFFFF80, 6, evtGeneralHeapPercentFormat, D_00435D40));
    currentColumn = (s32)((f32)freePercent / 100.0f * 200.0f) + 0x2A;
    sdfPrimLinkRect(list, 0x8000C000, 0, 0x73A0, 0x85F0, currentColumn * 16 + originX, 0x8600, 0xFF0000, 0);
    peakColumn = (s32)((f32)D_00435D40 / 100.0f * 200.0f) + 0x2B;
    sdfPrimLinkRect(list, 0x8000FFFF, 0, peakColumn * 16 + originX, 0x85D0, (peakColumn + 2) * 16 + originX, 0x8610, 0xFF0000, 0);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    if (D_00435D42 >= 0x47 || D_00435D40 < freePercent) {
        D_00435D40 = freePercent;
        D_00435D42 = 0;
    } else {
        D_00435D42 = D_00435D42 + 1;
    }
    return 0;
}

void evtGeneralHeapGraphTaskDestroy(dds3ProcessID_t *task) {
}

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_00411370);

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_00411380);

void evtToggleDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_00438E64 = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, evtDrawGeneralHeapUsageGraph, evtGeneralHeapGraphTaskDestroy, 0);
    } else if (mode == 0) {
        dds3KillProcess(D_00438E64, 0);
    }
}

extern u32 D_00435D4C;
/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 state) {
    s32 color;
    u32 byteOffset;
    s32 rowIndex;
    s32 columnIndex;
    s32 leftX;
    s32 topY;
    u32 bytesPerRow;
    u32 bytesPerCell;

    switch (state) {
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
    byteOffset = blockAddress - heapBase;
    bytesPerRow = D_00435D4C >> 8;
    bytesPerCell = D_00435D4C >> 14;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_00435D4C < blockSize || blockSize < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 64 - columnIndex;

        if (blockSize >= remainingCells / bytesPerCell) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize -= remainingCells / (D_00435D4C >> 14);
        } else {
            s32 cellCount = blockSize / bytesPerCell;

            if (cellCount == 0 && blockSize > 0) {
                cellCount = 1;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize = 0;
        }
        topY += 8;
    }
    while (blockSize >= (D_00435D4C >> 8)) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + 0x400, topY + 8, 0x0FFFFF80, NULL);
        blockSize -= D_00435D4C >> 8;
        topY += 8;
    }
    if (blockSize > 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + (blockSize / (D_00435D4C >> 14)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

extern u16 sdfMemGetType(sdfMemHandle_t *);

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y) {
    SdfHeapOverlayBlock *snapshot;
    SdfHeapOverlayBlock *entry;
    sdfMemHandle_t *block;
    u32 count;
    u32 i;
    u32 heapBase;
    u32 offset;
    s32 restoreInterrupts;
    s32 markerX;
    s32 markerY;

    snapshot = (SdfHeapOverlayBlock *)sdfGrOpenPacketBuff();
    restoreInterrupts = DIntr();
    block = sdfMemGetTop();
    entry = snapshot;
    heapBase = sdfMemGetPtr(block);
    /* Count only the interrupt-protected snapshot traversal. */
    count = 0;
    do {
        count++;
        entry->address = sdfMemGetPtr(block);
        entry->state = sdfMemGetType(block);
        entry->size = sdfMemGetSize(block);
        entry++;
        block = sdfMemGetNext(block);
    } while (block != NULL);
    if (restoreInterrupts) {
        EIntr();
    }
    sdfGrClosePacketBuff((s32)entry);

    entry = snapshot;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
        0x0FFFFF7F, 0x440, (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x20,
        0x80000000, 0x80806020));
    for (i = 0; i < count; i++) {
        sdfDrawHeapSpanOverlay(packetList, x, y, heapBase, entry->address,
            entry->size, entry->state);
        entry++;
    }
    markerY = y;
    markerX = x + 0x420;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        x + 0x4C0, markerY, 0x0FFFFF80, NULL);
    for (offset = 0x100000; offset < D_00435D4C; offset += 0x100000) {
        markerY = y + (offset / (D_00435D4C >> 8)) * 8;
        sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
            x + 0x470, markerY, 0x0FFFFF80, NULL);
    }
    markerY = y + (D_00435D4C / (D_00435D4C >> 8)) * 8;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        markerX + 0xA0, markerY, 0x0FFFFF80, NULL);
}


extern char D_00435D50[];
extern char D_00435D58[];
extern void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(SdfPoolNode *surface) {
    SdfGeneralHeapStats generalHeapStats;
    SdfChipHeapStats chipHeapStats;
    char statusText[100];
    sdfDrawTag_t *packetList;

    sdfMemGetInfo(&generalHeapStats);
    D_00435D4C = generalHeapStats.totalBytes;
    sdfChipGetInfo(&chipHeapStats);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawHeapMapOverlay(packetList, 0x8AC0, 0x79C0);
    sprintf(statusText, D_00435D50, generalHeapStats.freeBytes);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    sprintf(statusText, D_00435D58, chipHeapStats.freeBytes);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->append(surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&kwlnPositionedTextSurface);
    return 0;
}

extern s32 frGetFontHandleNum(void);
extern s32 frGetTextHandleNum(void);
extern s32 effGetFontListCount(void);
extern const char D_004113A0[];

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task) {
    SdfGeneralHeapStats heapStats;
    s32 heapRatio;
    sdfDrawTag_t *list;
    void *packet;
    s32 fontCount;
    s32 textCount;
    s32 gsCount;

    sdfMemGetInfo(&heapStats);
    /* The unused heap ratio retains the native zero-divisor check. */
    heapRatio = heapStats.freeBytes / heapStats.totalBytes;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70d0, 0x7968, 0xffff7f,
                                          0xf70, 0x98, 0x20000000, 0x40806040));
    fontCount = frGetFontHandleNum();
    textCount = frGetTextHandleNum();
    gsCount = effGetFontListCount();
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x7980,
                    0xffff80, 0, D_004113A0, fontCount, textCount, gsCount));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    return 0;
}

void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task) {
}

extern s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);
extern void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task);
extern dds3ProcessID_t *D_00438E68;
extern char D_004113B8[]; /* "DebugTimeGrph" */

void evtToggleAlternateDebugTimeGraphTask(s8 mode) {
    if (mode == 1) {
        D_00438E68 = dds3StartProcess(D_004113B8, 0x2710, 1, 1, evtDrawResourceCountDebugTask, evtResourceCountDebugTaskDestroy, 0);
    } else if (mode == 0) {
        dds3KillProcess(D_00438E68, 0);
    }
}

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void scrLinkHandle(scrProcessWork_s *node) {
    scrProcessWork_s *tail = scrHandleLast;

    if (tail == NULL) {
        scrHandleFirst = node;
        scrHandleLast = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->previous = tail;
        tail->next = node;
        node->next = NULL;
        scrHandleLast = node;
    }
    scrHandleCount++;
}

/* Detach from either end or the middle, and clear the old links. */
void scrUnlinkHandle(scrProcessWork_s *node) {
    if (scrHandleFirst == node) {
        scrHandleFirst = node->next;
    }
    else {
        node->previous->next = node->next;
    }
    if (scrHandleLast == node) {
        scrHandleLast = node->previous;
    }
    else {
        node->next->previous = node->previous;
    }
    node->previous = NULL;
    node->next = NULL;
    scrHandleCount--;
}

extern s32 itfMesMngInitialize(ItfMesSub *data);

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_004113A0);

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_004113B8);

scrProcessWork_s *bfContextCreate(BfFlw0Header *header, ScrSection *sections, ScrLabel *procedures,
                        ScrLabel *labels, ScrInstr *instructions, struct ItfMesSub *auxiliaryData,
                        char *strings, ScrProcedureIndex procedureIndex) {
    scrProcessWork_s *process;
    s32 i;
    s8 *types;
    ScrStackValue *values;

    if (header == NULL || sections == NULL || procedures == NULL ||
        instructions == NULL || procedureIndex < 0 ||
        procedureIndex >= sections->count) {
        return NULL;
    }
    process = sdfChipAllocDirect(0xF4);
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
        process->localInt = sdfChipAllocDirect(header->intCount * sizeof(s32));
        for (i = 0; i < header->intCount; i++) {
            process->localInt[i] = 0;
        }
    } else {
        process->localInt = NULL;
    }
    if (header->floatCount > 0) {
        process->localFloat = sdfChipAllocDirect(header->floatCount * sizeof(f32));
        for (i = 0; i < header->floatCount; i++) {
            process->localFloat[i] = 0;
        }
    } else {
        process->localFloat = NULL;
    }
    if (auxiliaryData != NULL) {
        process->resourceIndex = itfMesMngInitialize(auxiliaryData);
    }
    scrLinkHandle(process);
    evtPrintDeveloperConsoleMessage("start <%s>\n", procedures[procedureIndex].name);
    return process;
}

/* Resolve supported FLW0 sections relative to the header; reject bad magic/unknown kinds.
   Zero-count auxiliary sections are ignored, and absent sections remain null. */
scrProcessWork_s *fldFlowParse(BfFlw0Header *header, ScrProcedureIndex procedureIndex) {
    ScrSection *sections;
    ScrLabel *procedures = NULL;
    ScrLabel *labels = NULL;
    ScrInstr *instructions = NULL;
    struct ItfMesSub *auxiliaryData = NULL;
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

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D42);

INCLUDE_SDATA(const s32, "game/code_00107EF8", evtGeneralHeapPercentFormat);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D4C);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D50);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D58);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrHandleCount);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrHandleFirst);

INCLUDE_SDATA(const s32, "game/code_00107EF8", scrHandleLast);
