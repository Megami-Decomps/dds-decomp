#include "sdk_ps2.h"
#include "common.h"
#include "ps2asm.h"
#include "dds3_background_fade_api.h"
#include "dds3_draw_buffer.h"
#include "eff_random_mod.h"
#include "sdf_packet_list.h"
#include "sdf_tex_packet.h"
#include "sdf_projection.h"
#include "btl.h"
#include "dds3_draw_light_binding.h"
#include "gs_packet.h"
#include "eff_rectangle_blur_api.h"
#include "eff_event_draw.h"
#include "eff_event_work_api.h"
#include "dds3_fade_color.h"
#include "sdf_packet_alloc.h"
#include "evt_sel_state.h"
#include "sdf_line_pair_packet_api.h"
#include "sdf_gs_header.h"
#include "sdf_dev_cons.h"
#include "file_menu_start_mode.h"

#include "kernel_draw.h"
#include "sdfFrame.h"

typedef struct
{
    f32 fogMax;
    f32 fogMin;
    f32 fogNear;
    f32 fogFar;
} dds3FogRange_t;

extern dds3LightState_t dds3LightParam;
SYMBOL_AT(sdf3DFogParam_t, dds3FogState, dds3LightParam, 0x20);
SYMBOL_AT(sdfVector4_t, dds3AmbientColor, dds3LightParam, 0x10);
extern dds3FogRange_t dds3FogParam;
extern dds3FogRange_t dds3FogColor;
extern u16 fogparanow; extern u16 fogparamax;
extern f32 sdfFaceDivSoften; extern f32 sdfFaceDivBias;
extern u16 sbnow; extern u16 sbmax;
extern f32 softenbase; extern f32 softentarget;
extern f32 biasbase; extern f32 biastarget;
extern u16 lightcolornow[4];
extern u16 lightcolormax[4];
extern u16 lightdirnow[4];
extern u16 lightdirmax[3];
extern u32 dds3LightColor[];
extern u32 dds3LightTargetColor[];
extern f32 dds3LightArcFrom[][4];
extern f32 dds3LightArcTo[][4];
extern u16 ambientnow, ambientmax;
extern u32 ambientbase, ambienttarget;
extern u16 fogrgbnow, fogrgbmax;
extern u32 fogrgbbase, fogrgbtarget;
void Lerp(f32 aRatio);
void ToMatrix(void);
void dds3RotationArc(void);

static inline u32 PackRGBAWord(f32 unit) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], unit);
    return rgba[0];
}

#include "fld_terminal_scene_state.h"
#include "dds3_frame_draw_buffer.h"
#include "sdf_packet_builders.h"
#include "sdf_packet_header.h"
#include "sdf_packet_append.h"
#include "sdf_chip.h"
#include "kernel.h"
#include "sdf_primitive_packet_api.h"
#include "sdf_asset_state.h"
#include "sdf_sif_command.h"
#include "itf.h"
/* Declared before first use (were implicit int). */
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

extern u32 egl_OT_Offset;

extern u32 dds3DrawFlag;

#include "sdf_asset_create.h"

extern void *eglMaterial;

extern sdfGrOt_t sdfDebugGrOt;

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

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

extern sdfGrOt_t sdfGrOtTable[];

extern void *sdf2DMakeStdPacket(void *, s32);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern f32 eglLineVerts[][4];

extern u32 eglLineColors[];

extern s8 evtSelectionStateActive;

extern EvtSelState *evtSelectionState;

extern void evtSelStateDestroy(void);

extern void evtUpdateSelectionStateInterpolation(void);

extern f32 dds3QuakeOffset[4];

extern u32 dds3DrawFlag;

extern u8 dds3ScreenBlurFlag;

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern u16 blurnow;

extern u16 blurmax;

extern s16 bluralphabase;

extern s16 blurscalebase;

extern s16 bluralphatarget;

extern s16 blurscaletarget;

extern dds3Effect_PartialMultiFilterParam_t mfiltertarget;

extern dds3Effect_PartialMultiBlurParam_t mulblurtarget;

extern dds3Effect_DistortionBlurParam_t distblurtarget;

extern u32 distblurovercnt;

/* These snapshot buffers are eight-byte aligned; the parameter body is 0x2C bytes. */
extern dds3Effect_RippleBlurParam_t ripblurtarget __attribute__((aligned(8)));

extern u32 ripblurovercnt;

extern u16 mfilteratarget;

extern u16 mfilterabase;

extern u16 mfilternow;

extern u16 mfiltermax;

extern u16 mulblurabase;

extern u16 mulblurnow;

extern u16 mulbluratarget;

extern u16 mulblurmax;

extern u16 distblurabase;

extern u16 distblurnow;

extern u16 distbluratarget;

extern u16 distblurmax;

extern u16 ripbluratarget;

extern u16 ripblurabase;

extern u16 ripblurnow;

extern u16 ripblurmax;

extern void Normalize(void);

extern dds3Effect_Filter_t filter2target;

extern u16 filter2abase;

extern u16 filter2now;

extern u16 filter2atarget;

extern u16 filter2max;

extern dds3Effect_Blur_t blur2target;

extern u16 blur2abase;

extern u16 blur2now;

extern u16 blur2atarget;

extern u16 blur2max;

#define DDS3_DRAW_ALPHA_CHANNEL 3
#define DDS3_DRAW_COLOR_FADE_BIT 0x80000
#define DDS3_DRAW_COLOR_TRANSITION_BIT 0x100000
#define DDS3_DRAW_SQUARE_FADE_BIT 0x200000
#define DDS3_DRAW_SQUARE_TRANSITION_BIT 0x400000
#define DDS3_DRAW_CLEAR_SQUARE_TRANSITION 0xFFBFFFFF
#define DDS3_DRAW_OVERLAY_TRANSITION_BIT 0x800
#define DDS3_DRAW_CLEAR_OVERLAY_TRANSITION 0xfffff7ff

extern char D_004112F0[];
extern char D_00411308[];
#define DDS3_DIAG_PACKET_LIST_BYTES 0x20
#define DDS3_HELD_TEXTURE_CLAMP_MODE 5

extern void eglSetOt(u32 surfaceIndex);
extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);
extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);
extern u32 dds3RenderTex;
extern sdfGsMemSlice_t *D_00435CC4;
extern sdfTexHandle_t *dds3RenderTexture;
extern u16 D_00435CE8;
extern u16 fadenow;
extern u16 fademax;
extern s16 D_00435CEA;
extern u16 D_00435CEC;
extern u16 D_00435CEE;
#include "evt_frame_state_packet.h"
extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);
extern s32 D_00435BA4;
SYMBOL_AT(sdfGrOt_t, D_00380708, sdfGrOtTable, 0xBC0);
extern f32 D_0037F5EC[];
extern u8 dds3BackgroundFadeColor[4];
extern s8 dds3BackgroundFadeMode;
extern u16 dds3BackgroundFadeCounter;
extern u16 dds3BackgroundFadeDuration;
extern s16 dds3BackgroundFadeFirstRamp;
extern s16 dds3BackgroundFadeSecondRamp;

/* Two fixed frame banks: thirteen draw groups and their scene/overlay packets. */
#include "dds3_frame_draw_bank.h"
typedef char dds3FrameDrawBank_size_check[sizeof(dds3FrameDrawBank_t) == 0x1F40 ? 1 : -1];

extern dds3FrameDrawBank_t dds3DrawEnv[2];
extern u32 D_00435CBC;
extern sdfGrFrameSet_t sdfGrFrameSet;
extern Matrix4 sdfViewMatrix;
extern Matrix4 D_00384790;
extern sdf3DLight_t *dds3LightParam2[3];
extern sdfVector4_t dds3AmbientColor2;
extern void sdf3DLinkInitPacket(sdfDrawTag_t *, DmaPacketHeader *);

/* Initialize the selected frame bank's program, scene and overlay packet chains. */
void dds3InitDrawEnvSub(s32 bufferIndex) {
    sdfDrawTagInit(&dds3DrawEnv[bufferIndex].initialList);
    sdf3DLinkInitPacket(&dds3DrawEnv[bufferIndex].initialList,
                                        &dds3DrawEnv[bufferIndex].programReference);
    sdfDrawTagInit(&dds3DrawEnv[bufferIndex].sceneList);
    sdfPreDrawTagInit(&dds3DrawEnv[bufferIndex].linkedList);
    sdfGrSetStdDrawEnv(&dds3DrawEnv[bufferIndex].scene, &sdfGrFrameSet);
    /* Set the RGBAQ color word while retaining its floating-point Q word. */
    *(u32 *)&dds3DrawEnv[bufferIndex].scene.centeredBounds.rgbaq.value = D_00435CBC;
    if (dds3DrawFlag & 0x10000000) {
        sdfGrSetClearPacket(&dds3DrawEnv[bufferIndex].scene.centeredBounds, 0, 0,
                                         sdfGrFrameSet.bufferFormat,
                                         sdfGrFrameSet.auxiliaryFormat);
        dds3DrawFlag &= 0xEFFFFFFF;
    }
    sdfGrLinkStdDrawEnv(&dds3DrawEnv[bufferIndex].sceneList,
                                 &dds3DrawEnv[bufferIndex].linkedList,
                                 &dds3DrawEnv[bufferIndex].scene.link);
    sdf3DMakeViewportPacket(&dds3DrawEnv[bufferIndex].sceneMatrix,
                             &dds3ScreenParam, &sdfViewMatrix);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].sceneList, (u32)&dds3DrawEnv[bufferIndex].sceneMatrix);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[bufferIndex].scenePacketStorage, dds3LightParam.parallel, &dds3AmbientColor);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].sceneList, (u32)&dds3DrawEnv[bufferIndex].scenePacketStorage);

    sdfDrawTagInit(&dds3DrawEnv[bufferIndex].thirdList);
    sdf3DMakeViewportPacket(&dds3DrawEnv[bufferIndex].thirdMatrix, &D_0037F980, &D_00384790);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].thirdList, (u32)&dds3DrawEnv[bufferIndex].thirdMatrix);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[bufferIndex].overlayLightPacket, dds3LightParam2, &dds3AmbientColor2);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].thirdList, (u32)&dds3DrawEnv[bufferIndex].overlayLightPacket);
}

extern void sdfGrSetScreen(s32 displayModeIndex);
extern void dds3InitDrawEnvSub(s32 bufferIndex);
extern void dds3InitZClearDrawTag(void);
extern u8 D_00435BC8;
extern void eglInit(void);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void dds3InitDrawEnv(void) {
    sdfGrSetScreen(1);
    sdf3DMakeScreenParam(&dds3ScreenParam.camera);
    sdf3DMakeScreenParam(&D_0037F980.camera);
    dds3InitDrawEnvSub(0);
    dds3InitDrawEnvSub(1);
    dds3InitZClearDrawTag();
    D_00435BC8 = 0;
    eglInit();
}

/* One 0x1F40-byte draw bank of the held-texture render target. */

extern sdfGrFrameSet_t dds3RenderTexId;
extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern u32 dds3RenderDrawEnv;
extern sdf3DLight_t *dds3RenderLightParallel[3];
extern sdfVector4_t dds3RenderAmbientColor;
extern Matrix4 dds3RenderViewportMatrix;

/* Rebuild the held-texture render target's view and both frame banks. */
void dds3InitRenderDrawEnv(void) {
    s32 i;

    dds3RenderTexId.width = D_00435CCC;
    dds3RenderTexId.unk2 = D_00435CCE;
    dds3RenderTexId.height = D_00435CCE;
    dds3RenderTexId.bufferFormat = 0;
    dds3RenderTexId.auxiliaryFormat = 0x30;
    dds3RenderTexId.buffers[0] = D_00435CC4;
    dds3RenderTexId.buffers[1] = 0;
    dds3RenderTexId.buffers[2] = sdfGrFrameSet.buffers[2];
    sdf3DMakeScreenParam(&D_0037F7B0.camera);
    for (i = 0; i != 2; i++) {
        sdfDrawTagInit(&dds3DrawEnv[i].heldTexture.list);
        sdfGrSetDrawEnv(&dds3DrawEnv[i].heldTexture.scene, &dds3RenderTexId, 0);
        *(u32 *)&dds3DrawEnv[i].heldTexture.scene.centeredBounds.rgbaq.value = dds3RenderDrawEnv;
        sdfDrawTagLink(&dds3DrawEnv[i].heldTexture.list, (u32)&dds3DrawEnv[i].heldTexture.scene);
        sdf3DMakeViewportPacket(&dds3DrawEnv[i].heldTexture.matrix, &D_0037F7B0, &dds3RenderViewportMatrix);
        sdfDrawTagLink(&dds3DrawEnv[i].heldTexture.list, (u32)&dds3DrawEnv[i].heldTexture.matrix);
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[i].heldTexture.lighting, dds3RenderLightParallel, &dds3RenderAmbientColor);
        sdfDrawTagLink(&dds3DrawEnv[i].heldTexture.list, (u32)&dds3DrawEnv[i].heldTexture.lighting);
    }
}

extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern void dds3FreeRenderTexture(void);
extern void dds3InitRenderDrawEnv(void);
extern sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);
extern sdfTexHandle_t *sdfTexNew(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary);
/* Replace the held texture with a width x height image buffer; return 1 on success.
 * A texture-creation failure does not roll back the already allocated image buffer. */
s32 dds3AllocRenderTexture(u16 width, u16 height, f32 aspect) {
    sdfTexHandle_t *texture;

    if (dds3RenderTexture != 0) {
        dds3FreeRenderTexture();
    }
    dds3RenderTex = 0;
    D_00435CCC = width;
    D_00435CCE = height;
    D_00435CC4 = sdfGsAllocFrameBuff(width, height, 0);
    if (D_00435CC4 == 0) {
        return 0;
    }
    texture = sdfTexNew(width, height, 0, 0, D_00435CC4, 0, 0, 0);
    if (texture == 0) {
        return 0;
    }
    dds3RenderTexture = texture;
    sdfTexSetWrapMode(texture, DDS3_HELD_TEXTURE_CLAMP_MODE);
    sdfTexMakeGIFPacket(texture);
    D_0037F7B0.camera.aspect = aspect;
    D_0037F7B0.camera.width = width;
    D_0037F7B0.camera.height = height;
    dds3InitRenderDrawEnv();
    return 1;
}

/* Release the held texture if present and clear tracking; no separate buffer free. */
void dds3FreeRenderTexture(void) {
    if (dds3RenderTexture != 0) {
        sdfTexReleaseReference(dds3RenderTexture);
    }
    dds3RenderTexture = 0;
    D_00435CC4 = 0;
    dds3RenderTex = 0;
}

/* Set the flag and return 1 when a held handle exists; otherwise leave it and return 0. */

s32 dds3RenderTextureON(void) {
    s32 result = 0;

    if (dds3RenderTexture == 0) {
        return result;
    }
    dds3RenderTex = 1;
    return 1;
}

void dds3RenderTextureOFF(void) {
    dds3RenderTex = 0;
}

u32 dds3CheckRenderTexture(void) {
    return dds3RenderTex;
}

sdfTexHandle_t *dds3GetRenderTexture(void) {
    return dds3RenderTexture;
}

extern f32 D_00435D10[];
extern f32 fabsf(f32 value);

/* Advance the two bounded shake offsets and taper finite-duration amplitudes. */
void dds3RunQuake(void) {
    f32 limits[2];
    f32 weights[2];
    s32 axis;

    memcpy(weights, D_00435D10, sizeof(weights));
    if (D_00435CE8 == 0) {
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        return;
    }
    if (D_00435CEA == 0) {
        D_00435CE8 = 0;
        return;
    }
    limits[0] = D_00435CEC * 0.01f;
    limits[1] = D_00435CEE * 0.01f;
    for (axis = 0; axis < 2; axis++) {
        if (limits[axis] != 0.0f) {
            f32 step = dds3GetRandFloatA(0) * limits[axis] * 0.5f + limits[axis] * 0.5f;

            if (limits[axis] * weights[axis] <= fabsf(dds3QuakeOffset[axis])) {
                if (dds3QuakeOffset[axis] <= 0.0f) {
                    dds3QuakeOffset[axis] += step;
                } else {
                    dds3QuakeOffset[axis] -= step;
                }
            } else {
                dds3QuakeOffset[axis] += 2.0f * (step * (dds3GetRandFloatA(0) - 0.5f));
            }
            if (dds3QuakeOffset[axis] < -limits[axis]) {
                dds3QuakeOffset[axis] = -limits[axis];
            }
            if (limits[axis] < dds3QuakeOffset[axis]) {
                dds3QuakeOffset[axis] = limits[axis];
            }
        }
    }
    if (D_00435CEA > 0) {
        s32 divisor = D_00435CEA + 5;

        D_00435CEC -= D_00435CEC / divisor;
        D_00435CEE -= D_00435CEE / divisor;
        D_00435CEA--;
    }
}

#define DDS3_FADE_TICKS_PER_FRAME 10
#define DDS3_FADE_MARK_NUMERATOR 4
#define DDS3_FADE_COUNT_DOWN_BIT 1
#define DDS3_FADE_COUNT_UP_BIT 2
#define DDS3_FADE_DIRECTION_BITS 3
#define DDS3_FADE_MAX_ALPHA 0x80
#define DDS3_FADE_ALPHA_SHIFT 7
#define DDS3_COLOR_ALPHA_CHANNEL 3
#define DDS3_BGFADE_COUNT_DOWN_BIT 0x04000000
#define DDS3_BGFADE_COUNT_UP_BIT 0x08000000
#define DDS3_BGFADE_DIRECTION_BITS 0x0C000000
#define DDS3_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF
#define DDS3_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF
#define DDS3_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF
#define DDS3_BGFADE_FIRST_RAMP_MAX 0x31
#define DDS3_BGFADE_SECOND_RAMP_MAX 0x4F
#define DDS3_BGFADE_FIRST_RAMP_SCALE 49.0f
#define DDS3_BGFADE_SECOND_RAMP_SCALE 79.0f

/* Store tenths in the 16-bit duration before deriving its forty-percent mark.
 * Zero frames only disables the configuration; mode zero stores the -1 sentinel. */
void dds3SetQuake(s32 fadeMode, s32 frames) {
    if (frames == 0) {
        D_00435CE8 = 0;
        return;
    }
    D_00435CE8 = 1;
    if (fadeMode == 0) {
        D_00435CEA = -1;
    } else {
        D_00435CEA = fadeMode;
    }
    D_00435CEE = frames * DDS3_FADE_TICKS_PER_FRAME;
    D_00435CEC = D_00435CEE * DDS3_FADE_MARK_NUMERATOR / DDS3_FADE_TICKS_PER_FRAME;
}

/* Disable the configured fade without clearing its stored timing parameters. */
void dds3StopQuake(void) {
    D_00435CE8 = 0;
}

/* Fill each frame state's z-clear packet for both draw buffers. */
void dds3InitZClearDrawTag(void) {
    s32 state;
    s32 buffer;
    u64 topLeft;
    u64 bottomRight;

    for (state = 0; state < 3; state++) {
        for (buffer = 0; buffer < 2; buffer++) {
            dds3ZClearDrawTag[state][buffer].dma.quadwordCount = 10;
            dds3ZClearDrawTag[state][buffer].dma.reservedByte = 0;
            dds3ZClearDrawTag[state][buffer].dma.control = 0x70;
            dds3ZClearDrawTag[state][buffer].dma.address = 0;
            dds3ZClearDrawTag[state][buffer].dma.firstVifCode = 0;
            dds3ZClearDrawTag[state][buffer].dma.secondVifCode = 0x5000000A;
            dds3ZClearDrawTag[state][buffer].testGifTag = 0x1000000000000001;
            dds3ZClearDrawTag[state][buffer].testGifRegisters = 0xE;
            dds3ZClearDrawTag[state][buffer].clearTest.registerId = 0x47;
            dds3ZClearDrawTag[state][buffer].clearTest.value = 0x32001;
            dds3ZClearDrawTag[state][buffer].clearGifTag = 0x1003400000000004;
            dds3ZClearDrawTag[state][buffer].clearGifRegisters = 0xE;
            topLeft = 0x79007000;
            dds3ZClearDrawTag[state][buffer].topLeftColor.registerId = 1;
            dds3ZClearDrawTag[state][buffer].topLeftColor.value = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].topLeftPosition.registerId = 5;
            dds3ZClearDrawTag[state][buffer].topLeftPosition.value = topLeft;
            bottomRight = 0x87009000;
            dds3ZClearDrawTag[state][buffer].bottomRightColor.registerId = 1;
            dds3ZClearDrawTag[state][buffer].bottomRightColor.value = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].bottomRightPosition.registerId = 5;
            dds3ZClearDrawTag[state][buffer].bottomRightPosition.value = bottomRight;
            dds3ZClearDrawTag[state][buffer].blendGifTag = 0x1000000000008002;
            dds3ZClearDrawTag[state][buffer].blendGifRegisters = 0xE;
            dds3ZClearDrawTag[state][buffer].blend.test.registerId = 0x47;
            dds3ZClearDrawTag[state][buffer].blend.test.value = 0x5101B;
            dds3ZClearDrawTag[state][buffer].blend.alpha.registerId = 0x42;
            dds3ZClearDrawTag[state][buffer].blend.alpha.value = 0x44;
        }
    }
}

/* Append the selected state's packet for the current draw buffer to a new list. */
sdfDrawTag_t *dds3LinkZClearDrawTag(s32 stateIndex) {
    sdfDrawTag_t *packetList = dds3FontReqAllocDrawTag();

    sdfDrawTagLink(packetList, (u32)&dds3ZClearDrawTag[stateIndex][dds3GetFrameID()]);
    return packetList;
}

/* Cancel the foreground direction and zero RGBA; timing words are unchanged. */
void dds3FadeClear(void) {
    dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
    fadergba.r = 0;
    fadergba.g = 0;
    fadergba.b = 0;
    fadergba.a = 0;
}

/* Store RGBA and cancel foreground direction flags without changing timing. */
void dds3FadeSet(s32 red, s32 green, s32 blue, s32 alpha) {
    dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = alpha;
}

/* Return the live color address through outColor, rather than copying channels. */
void dds3FadeColorGet(dds3FadeColor_t **outColor) {
    *outColor = &fadergba;
}

/* Replace RGB only; alpha, direction flags and timing are preserved. */
void dds3FadeColorSet(s8 red, s8 green, s8 blue) {
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
}

/* Start the countdown with caller RGB. A zero duration also clears RGB via FadeClear.
 * The signed alpha byte -0x80 carries the raw maximum-alpha value 0x80. */
void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration) {
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = -DDS3_FADE_MAX_ALPHA;
    if (duration == 0) {
        fadergba.a = 0;
        fadenow = 0;
        fademax = 0;
        dds3FadeClear();
        return;
    }
    fademax = duration;
    fadenow = duration;
    dds3DrawFlag = (dds3DrawFlag | DDS3_FADE_COUNT_DOWN_BIT) & ~DDS3_FADE_COUNT_UP_BIT;
}

/* Start the same countdown using existing RGB; zero duration preserves RGB. */
void dds3FadeIn2(s32 duration) {
    fadergba.a = -DDS3_FADE_MAX_ALPHA;
    if (duration == 0) {
        fadergba.a = 0;
        fadenow = 0;
        dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        fademax = 0;
        return;
    }
    fademax = duration;
    fadenow = duration;
    dds3DrawFlag = (dds3DrawFlag | DDS3_FADE_COUNT_DOWN_BIT) & ~DDS3_FADE_COUNT_UP_BIT;
}

/* Start the count-up with caller RGB; zero duration leaves maximum alpha. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration) {
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = 0;
    if (duration == 0) {
        fadenow = 0;
        fademax = 0;
        fadergba.a = -DDS3_FADE_MAX_ALPHA;
        dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        return;
    }
    fademax = duration;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~DDS3_FADE_COUNT_DOWN_BIT) | DDS3_FADE_COUNT_UP_BIT;
}

/* Start the same count-up using existing RGB; zero duration preserves RGB. */
void dds3FadeOut2(s32 duration) {
    fadergba.a = 0;
    if (duration == 0) {
        fadenow = 0;
        fadergba.a = -DDS3_FADE_MAX_ALPHA;
        dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        fademax = 0;
        return;
    }
    fademax = duration;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~DDS3_FADE_COUNT_DOWN_BIT) | DDS3_FADE_COUNT_UP_BIT;
}

/* Return whether either foreground counter direction is enabled. */
s32 dds3FadeCheck(void) {
    return (dds3DrawFlag & DDS3_FADE_DIRECTION_BITS) != 0;
}

/* Step the u16 counter and sample alpha before testing exact completion.
 * Completion clears timing/flags but leaves the sampled endpoint alpha intact. */
void dds3FadeUpdate(void) {
    if (dds3FadeCheck() != 0) {
        if (dds3DrawFlag & DDS3_FADE_COUNT_DOWN_BIT) {
            fadenow -= 1;
        } else {
            fadenow += 1;
        }
        fadergba.a = (fadenow << DDS3_FADE_ALPHA_SHIFT) / fademax;
        if (((dds3DrawFlag & DDS3_FADE_COUNT_DOWN_BIT) && fadenow == 0) || ((dds3DrawFlag & DDS3_FADE_COUNT_UP_BIT) && fadenow == fademax)) {
            fadenow = 0;
            fademax = 0;
            dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        }
    }
}

/* Submit diagnostics when enabled and either counter is nonzero; only positive
 * counters produce text, so negative-only errors still submit an empty list. */
void dds3DrawDistRipOverError(void) {
    sdfDrawTag_t *packetList;

    if (D_00435BA4 != 0) {
        if (distblurovercnt != 0 || ripblurovercnt != 0) {
            packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_DIAG_PACKET_LIST_BYTES);
            sdfDrawTagInit(packetList);
            if ((s32)distblurovercnt > 0) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, D_004112F0, distblurovercnt));
            }
            if ((s32)ripblurovercnt > 0) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x73C0, 0x7B40, 0xFEFFFF, 4, D_00411308, ripblurovercnt));
            }
            D_00380708.regFunc(&D_00380708, packetList);
        }
    }
}

#ifdef NON_MATCHING
#include "common.h"

extern u8 dds3FadePacket[]; /* the overlay packet, one per draw buffer */
extern sdfGrOt_t sdfGrOtTable[];
extern s32 dds3FadeIsBackgroundOverlayActive(void);

#define DDS3_FADE_OVERLAY_SURFACE 92
#define DDS3_FADE_MAX_ALPHA 0x80
#define DDS3_GS_PABE 0x49
#define DDS3_GS_FBA_1 0x4A
#define DDS3_OVERLAY_TEST 0x31003       /* alpha and depth tests always pass */
#define DDS3_OVERLAY_ALPHA 0x01         /* (Cd - Cs) * As / 128 + Cs */
#define DDS3_OVERLAY_DEPTH 0xFEFFFFULL
#define DDS3_OVERLAY_LEFT 0x7000
#define DDS3_OVERLAY_RIGHT 0x9000
#define DDS3_OVERLAY_TOP 0x7900
#define DDS3_OVERLAY_BOTTOM 0x8700

/* An end tag carrying VIF DIRECT of ten quadwords: four A+D register writes,
 * then a blended sprite of two vertices. */
typedef struct dds3FadePacket_t {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite alpha;
    SdfGsRegisterWrite pabe;
    SdfGsRegisterWrite fba;
    SdfGsRegisterWrite test;
    u64 spriteTag;
    u64 spriteRegisters;
    SdfGsRegisterWrite vertices[4];
} dds3FadePacket_t;

typedef char dds3FadePacket_size_must_be_0xB0[(sizeof(dds3FadePacket_t) == 0xB0) ? 1 : -1];

static void dds3WriteRegister(SdfGsRegisterWrite *write, u64 value, u64 registerId) {
    write->value = value;
    write->registerId = registerId;
}

static u64 dds3OverlayVertex(u32 x, u32 y) {
    return (DDS3_OVERLAY_DEPTH << 32) | (u32)(y << 16) | x;
}

/* While the fade colour has any alpha, cover the screen with it: the sprite
 * keeps (128 - alpha) / 128 of what is drawn. An active background overlay
 * narrows it to the rows between its two bands. Upstream: INCLUDE_ASM in
 * both games; same packet as the retail routine. */
void dds3DrawFade(void) {
    u32 buffer = dds3GetFrameID();
    s32 overlayAlpha = DDS3_FADE_MAX_ALPHA - fadergba.a;
    dds3FadePacket_t *packet;
    u32 top = DDS3_OVERLAY_TOP;
    u32 bottom = DDS3_OVERLAY_BOTTOM;
    u64 color;
    sdfDrawTag_t *packets;
    sdfGrOt_t *surface;

    if (overlayAlpha >= DDS3_FADE_MAX_ALPHA) {
        return;
    }
    packet = (dds3FadePacket_t *)(dds3FadePacket + buffer * sizeof(dds3FadePacket_t));
    packet->header.dmaTag = 0x7000000A;              /* end, ten quadwords */
    packet->header.vifCommands = 0x5000000AULL << 32; /* NOP; DIRECT ten quadwords */
    packet->header.gifTag = 0x1000000000000004ULL;    /* NLOOP 4, NREG 1 */
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
    dds3WriteRegister(&packet->alpha, DDS3_OVERLAY_ALPHA, SDF_GS_ALPHA_1);
    dds3WriteRegister(&packet->pabe, 0, DDS3_GS_PABE);
    dds3WriteRegister(&packet->fba, 0, DDS3_GS_FBA_1);
    dds3WriteRegister(&packet->test, DDS3_OVERLAY_TEST, SDF_GS_TEST_1);
    packet->spriteTag = 0x1023400000008004ULL; /* NLOOP 4, EOP, PRE, PRIM blended sprite, NREG 1 */
    packet->spriteRegisters = SDF_GIF_REGISTER_AD;

    if (dds3FadeIsBackgroundOverlayActive()) {
        top = 0x7A88 - dds3BackgroundFadeFirstRamp * 8;
        bottom = 0x8488 + dds3BackgroundFadeSecondRamp * 8;
    }
    /* Retail shifts the alpha in 32 bits, sign-extending a negative one (a
     * fade alpha above 0x80) over Q; the sprite is untextured, so Q goes
     * unread, but the packet keeps retail's bits. */
    color = 0x3F80000000000000ULL /* Q = 1.0 */ | (u64)(s64)(s32)((u32)overlayAlpha << 24) |
            ((u64)fadergba.b << 16) | ((u64)fadergba.g << 8) | fadergba.r;
    dds3WriteRegister(&packet->vertices[0], color, SDF_GS_RGBAQ);
    dds3WriteRegister(&packet->vertices[1], dds3OverlayVertex(DDS3_OVERLAY_LEFT, top), SDF_GS_XYZ2);
    dds3WriteRegister(&packet->vertices[2], color, SDF_GS_RGBAQ);
    dds3WriteRegister(&packet->vertices[3], dds3OverlayVertex(DDS3_OVERLAY_RIGHT, bottom), SDF_GS_XYZ2);

    packets = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(packets, (u32)packet);
    surface = &sdfGrOtTable[DDS3_FADE_OVERLAY_SURFACE];
    surface->regFunc(surface, packets);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawFade);
#endif

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void dds3FadeResetBackground(void) {
    dds3DrawFlag &= DDS3_BGFADE_KEEP_OTHER_BITS;
    dds3BackgroundFadeColor[0] = 0;
    dds3BackgroundFadeColor[1] = 0;
    dds3BackgroundFadeColor[2] = 0;
    dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = 0;
    dds3BackgroundFadeFirstRamp = DDS3_BGFADE_FIRST_RAMP_MAX;
    dds3BackgroundFadeSecondRamp = DDS3_BGFADE_SECOND_RAMP_MAX;
}

/* Start the background countdown; zero duration performs the full background reset. */
void dds3FadeBackgroundStartOut(s32 duration) {
    dds3BackgroundFadeColor[0] = 0;
    dds3BackgroundFadeColor[1] = 0;
    dds3BackgroundFadeColor[2] = 0;
    dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = DDS3_FADE_MAX_ALPHA;
    if (duration == 0) {
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = 0;
        dds3BackgroundFadeCounter = 0;
        dds3BackgroundFadeDuration = 0;
        dds3BackgroundFadeFirstRamp = DDS3_BGFADE_FIRST_RAMP_MAX;
        dds3BackgroundFadeSecondRamp = DDS3_BGFADE_SECOND_RAMP_MAX;
        dds3FadeResetBackground();
    } else {
        dds3BackgroundFadeDuration = duration;
        dds3BackgroundFadeCounter = duration;
        dds3DrawFlag = (dds3DrawFlag | DDS3_BGFADE_COUNT_DOWN_BIT) & DDS3_BGFADE_CLEAR_UP_MASK;
    }
    D_0037F5EC[2] = 2048.0f;
}

/* Start the background count-up; zero duration leaves max alpha and zero ramps. */
void dds3FadeBackgroundStartIn(s32 duration) {
    dds3BackgroundFadeColor[0] = 0;
    dds3BackgroundFadeColor[1] = 0;
    dds3BackgroundFadeColor[2] = 0;
    dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = 0;
    if (duration == 0) {
        dds3BackgroundFadeCounter = 0;
        dds3BackgroundFadeDuration = 0;
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = DDS3_FADE_MAX_ALPHA;
        dds3DrawFlag &= DDS3_BGFADE_KEEP_OTHER_BITS;
        dds3BackgroundFadeFirstRamp = 0;
        dds3BackgroundFadeSecondRamp = 0;
    } else {
        dds3BackgroundFadeDuration = duration;
        dds3BackgroundFadeCounter = 0;
        dds3DrawFlag = (dds3DrawFlag & DDS3_BGFADE_CLEAR_DOWN_MASK) | DDS3_BGFADE_COUNT_UP_BIT;
    }
    D_0037F5EC[2] = 2041.0f;
}

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
dds3BackgroundOverlayActivity_t dds3FadeIsBackgroundOverlayActive(void) {
    if (dds3DrawFlag & DDS3_BGFADE_DIRECTION_BITS) {
        return DDS3_BACKGROUND_OVERLAY_ACTIVE;
    }
    if (dds3BackgroundFadeMode == 0) {
        if (DDS3_FADE_MAX_ALPHA - dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] >= DDS3_FADE_MAX_ALPHA) {
            return DDS3_BACKGROUND_OVERLAY_INACTIVE;
        }
    } else if (dds3BackgroundFadeFirstRamp == DDS3_BGFADE_FIRST_RAMP_MAX) {
        return DDS3_BACKGROUND_OVERLAY_INACTIVE;
    }
    return DDS3_BACKGROUND_OVERLAY_ACTIVE;
}

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void dds3FadeSetMode(s32 visibilityMode) {
    dds3BackgroundFadeMode = visibilityMode;
    if (dds3BackgroundFadeMode == 0) {
        dds3BackgroundFadeFirstRamp = 0;
        dds3BackgroundFadeSecondRamp = 0;
    } else {
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = DDS3_FADE_MAX_ALPHA;
    }
}

/* Step the u16 counter, then derive integer alpha and f32 ramp samples.
 * Completion clears timing/flags after sampling, retaining endpoint values. */
void dds3StepBackgroundFade(void) {
    f32 fadeRatio;

    if (dds3DrawFlag & DDS3_BGFADE_DIRECTION_BITS) {
        if (dds3DrawFlag & DDS3_BGFADE_COUNT_DOWN_BIT) {
            dds3BackgroundFadeCounter--;
        } else {
            dds3BackgroundFadeCounter++;
        }
        fadeRatio = (f32)dds3BackgroundFadeCounter / (f32)dds3BackgroundFadeDuration;
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = (dds3BackgroundFadeCounter << DDS3_FADE_ALPHA_SHIFT) / dds3BackgroundFadeDuration;
        dds3BackgroundFadeFirstRamp = DDS3_BGFADE_FIRST_RAMP_SCALE - fadeRatio * DDS3_BGFADE_FIRST_RAMP_SCALE;
        dds3BackgroundFadeSecondRamp = DDS3_BGFADE_SECOND_RAMP_SCALE - fadeRatio * DDS3_BGFADE_SECOND_RAMP_SCALE;
        if (((dds3DrawFlag & DDS3_BGFADE_COUNT_DOWN_BIT) && dds3BackgroundFadeCounter == 0) ||
            ((dds3DrawFlag & DDS3_BGFADE_COUNT_UP_BIT) && dds3BackgroundFadeCounter == dds3BackgroundFadeDuration)) {
            dds3BackgroundFadeCounter = 0;
            dds3BackgroundFadeDuration = 0;
            dds3DrawFlag &= DDS3_BGFADE_KEEP_OTHER_BITS;
        }
    }
}

void dds3DrawBackgroundFadeBands(void) {
    s16 firstRamp = 0;
    s16 secondRamp = 0;
    s32 fade;
    s32 alpha;

    if (dds3BackgroundFadeMode == 0) {
        fade = DDS3_FADE_MAX_ALPHA - dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL];
    } else {
        firstRamp = dds3BackgroundFadeFirstRamp;
        fade = 0;
        secondRamp = dds3BackgroundFadeSecondRamp;
        if (firstRamp == DDS3_BGFADE_FIRST_RAMP_MAX) {
            return;
        }
    }
    if (fade >= DDS3_FADE_MAX_ALPHA) {
        return;
    }

    eglSetOt(0x4F);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    alpha = (DDS3_FADE_MAX_ALPHA - fade) << 24;
    evtSubmitDefaultDepthGradientRect(0, -2 - firstRamp, 0x200, 0x33, alpha, alpha, alpha, alpha);
    evtSubmitDefaultDepthGradientRect(0, secondRamp + 0x171, 0x200, 0x51, alpha, alpha, alpha, alpha);
}

void dds3SetScreenFovy(f32 fieldOfView) {
    D_0037F5EC[0] = fieldOfView;
}

/* Copy one complete vector into the default draw color state; retain no source pointer. */
void dds3SetAmbient(sdfVector4_t *ambient)
{
    SDF_VU0_COPYVECTOR(&dds3LightParam.ambient, ambient);
}

/* Copy the five-word draw vector parameter body without interpreting or retaining it. */
void dds3SetFog(s32 *fog)
{
    memcpy(&dds3LightParam.fog, fog, 0x14);
}

/* Retain the caller-owned light-data pointer in its draw slot. */
void dds3SetLight(f32 (*lightSource)[4], dds3LightSourceSlot_t slot) {
    dds3LightParam.parallel[slot] = (sdf3DLight_t *)lightSource;
}

/* Initialize bounds covering the default 512-by-448 draw viewport. */
void setdraw_xy(s32 corners[2][2]) {
    corners[1][0] = DRAW_VIEWPORT_WIDTH;
    corners[1][1] = DRAW_VIEWPORT_HEIGHT;
    corners[0][1] = 0;
    corners[0][0] = 0;
}

/* Stage the solid rectangle's blend control word. */
void dds3SetFilter2Para(u32 blendControl) {
    filter2target.blend = blendControl;
}

/* Stage the solid rectangle's packed RGBA color. */
void dds3SetFilter2RGBA(u32 rgba) {
    filter2target.abgr = rgba;
}

extern u16 filter2swapnow;
extern u16 filter2swapmax;

extern dds3Effect_Filter_t dds3Filter2Buf;

/* Capture the live solid rectangle before applying or interpolating staged params.
 * Duration storage is 16-bit; the immediate test uses the full argument. */
void dds3Filter2Swap(s32 duration) {
    dds3Filter2Buf = *Eve_FilterGetParams();
    filter2swapnow = 0;
    filter2swapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_COLOR_TRANSITION_BIT;
        setdraw_xy(filter2target.xy);
        Eve_FilterSetParam(&filter2target);
    } else {
        dds3DrawFlag |= DDS3_DRAW_COLOR_TRANSITION_BIT;
    }
}

/* Fade solid-rectangle alpha from zero to the staged color; zero applies now. */
void dds3Filter2On(s32 duration) {
    dds3Effect_Filter_t *params = &filter2target;
    s32 requestedDuration = duration;

    filter2abase = 0;
    filter2now = 0;
    filter2atarget = (params->abgr >> 24);
    filter2max = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_COLOR_FADE_BIT;
        setdraw_xy(params->xy);
        Eve_FilterSetParam(params);
        Eve_FilterON();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_COLOR_FADE_BIT;
    }
}

/* Fade staged solid-rectangle alpha toward zero; zero disables it immediately. */
void dds3Filter2Off(s32 duration) {
    filter2atarget = 0;
    filter2abase = (filter2target.abgr >> 24);
    filter2now = 0;
    filter2max = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_COLOR_FADE_BIT;
        dds3DrawFlag &= ~DDS3_DRAW_COLOR_TRANSITION_BIT;
        Eve_FilterOFF();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_COLOR_FADE_BIT;
    }
}

/* Stage the textured square's blend control word. */
void dds3SetMFilterPara(u32 blendControl) {
    mfiltertarget.Filter.blendControl = blendControl;
}

/* Stage the textured square's packed RGBA color. */
void dds3SetMFilterRGBA(u32 rgba) {
    mfiltertarget.Filter.rgba = rgba;
}

/* Stage textured-square extent and center coordinates. */
void dds3SetMFilterArea(s32 extent, s32 centerX, s32 centerY) {
    mfiltertarget.area = extent;
    mfiltertarget.center[0] = centerX;
    mfiltertarget.center[1] = centerY;
}

extern dds3Effect_PartialMultiFilterParam_t dds3MFilterBuf;
extern u16 mfilterswapnow;
extern u16 mfilterswapmax;

/* Save the live textured-square params; zero copies the staged body immediately. */
void dds3MFilterSwap(s32 duration) {
    dds3MFilterBuf = Eve_PartialMultiFilterGetParams()->param;
    mfilterswapnow = 0;
    mfilterswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= DDS3_DRAW_CLEAR_SQUARE_TRANSITION;
        Eve_PartialMultiFilterSetParam(&mfiltertarget);
    } else {
        dds3DrawFlag |= DDS3_DRAW_SQUARE_TRANSITION_BIT;
    }
}

/* Fade square alpha from zero to its staged color, or apply/enable it now. */
void dds3MFilterOn(s32 duration) {
    dds3Effect_PartialMultiFilterParam_t *params = &mfiltertarget;
    s32 requestedDuration = duration;

    mfilterabase = 0;
    mfilternow = 0;
    mfilteratarget = params->Filter.color[DDS3_DRAW_ALPHA_CHANNEL];
    mfiltermax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_SQUARE_FADE_BIT;
        Eve_PartialMultiFilterSetParam(params);
        Eve_PartialMultiFilterON();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_SQUARE_FADE_BIT;
    }
}

/* Fade staged square alpha toward zero, or disable it now when duration is zero. */
void dds3MFilterOff(s32 duration) {
    mfilteratarget = 0;
    mfilterabase = mfiltertarget.Filter.color[DDS3_DRAW_ALPHA_CHANNEL];
    mfilternow = 0;
    mfiltermax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_SQUARE_FADE_BIT;
        dds3DrawFlag &= ~DDS3_DRAW_SQUARE_TRANSITION_BIT;
        Eve_PartialMultiFilterOFF();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_SQUARE_FADE_BIT;
    }
}

/* Apply vignette alpha and Q12 scale now, or interpolate them over duration updates.
 * Despite the legacy function name, these values do not specify position offsets. */
void dds3SetBlur(s32 duration, s32 alpha, s32 scale) {
    if (duration == 0) {
        dds3ScreenBlurAlpha = (s16)alpha;
        dds3ScreenBlurScale = (s16)scale;
        if ((alpha == 0) && (scale == 0)) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        dds3DrawFlag = dds3DrawFlag & DDS3_DRAW_CLEAR_OVERLAY_TRANSITION;
        return;
    }
    bluralphabase = dds3ScreenBlurAlpha;
    blurscalebase = dds3ScreenBlurScale;
    bluralphatarget = (s16)alpha;
    blurscaletarget = (s16)scale;
    blurmax = (s16)duration;
    dds3DrawFlag = dds3DrawFlag | DDS3_DRAW_OVERLAY_TRANSITION_BIT;
    blurnow = 0;
}

#define DDS3_DRAW_RECT_FADE_BIT 0x20000
#define DDS3_DRAW_RECT_TRANSITION_BIT 0x40000
#define DDS3_DRAW_CLEAR_RECT_TRANSITION 0xFFFBFFFF
#define DDS3_DRAW_TEXTURE_FADE_BIT 0x1000
#define DDS3_DRAW_TEXTURE_TRANSITION_BIT 0x8000
#define DDS3_DRAW_CLEAR_TEXTURE_TRANSITION 0xFFFF7FFF

/* Stage rectangle-blur rotation, scale and blend control, in the original order. */
void dds3SetBlur2Para(f32 rotation, f32 scale, u32 blendControl) {
    blur2target.rotate = rotation;
    blur2target.scale = scale;
    blur2target.blend = blendControl;
}

/* Stage the rectangle blur's packed RGBA color. */
void dds3SetBlur2RGBA(u32 rgba) {
    blur2target.abgr = rgba;
}

/* Stage the rectangle blur's center coordinates. */
void dds3SetBlur2Area(s32 centerX, s32 centerY) {
    blur2target.center[0] = centerX;
    blur2target.center[1] = centerY;
}

extern dds3Effect_Blur_t dds3RectangleBlurTransitionStartParameters;
extern u16 blur2swapnow;
extern u16 blur2swapmax;

/* Save live rectangle-blur params, then apply now or request interpolation. */
void dds3Blur2Swap(s32 duration) {
    dds3RectangleBlurTransitionStartParameters = Eve_BlurGetParam()->source;
    blur2swapnow = 0;
    blur2swapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= DDS3_DRAW_CLEAR_RECT_TRANSITION;
        setdraw_xy(blur2target.xy);
        Eve_BlurSetParam(&blur2target);
    } else {
        dds3DrawFlag |= DDS3_DRAW_RECT_TRANSITION_BIT;
    }
}

/* Fade rectangle-blur alpha from zero to the staged color; zero applies/enables now. */
void dds3Blur2On(s32 duration) {
    dds3Effect_Blur_t *params = &blur2target;
    s32 requestedDuration = duration;

    blur2abase = 0;
    blur2now = 0;
    blur2atarget = (params->abgr >> 24);
    blur2max = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_RECT_FADE_BIT;
        setdraw_xy(params->xy);
        Eve_BlurSetParam(params);
        Eve_BlurON();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_RECT_FADE_BIT;
    }
}

/* Fade staged rectangle-blur alpha toward zero; zero disables it immediately. */
void dds3Blur2Off(s32 duration) {
    blur2atarget = 0;
    blur2abase = (blur2target.abgr >> 24);
    blur2now = 0;
    blur2max = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_RECT_FADE_BIT;
        dds3DrawFlag &= ~DDS3_DRAW_RECT_TRANSITION_BIT;
        Eve_BlurOFF();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_RECT_FADE_BIT;
    }
}

/* Stage textured-blur source rotation, scale and blend control. */
void dds3SetMultiBlurPara(f32 rotation, f32 scale, u32 blendControl) {
    mulblurtarget.Blur.rotate = rotation;
    mulblurtarget.Blur.scale = scale;
    mulblurtarget.Blur.blend = blendControl;
}

/* Stage the textured blur's packed source RGBA color. */
void dds3SetMultiBlurRGBA(u32 rgba) {
    mulblurtarget.Blur.abgr = rgba;
}

/* Stage textured-blur extent and source center coordinates. */
void dds3SetMultiBlurArea(s32 extent, s32 centerX, s32 centerY) {
    mulblurtarget.area = extent;
    mulblurtarget.Blur.center[0] = centerX;
    mulblurtarget.Blur.center[1] = centerY;
}

extern dds3Effect_PartialMultiBlurParam_t dds3MultiBlurBuf;
extern u16 mulblurswapnow;
extern u16 mulblurswapmax;

/* Save live textured-blur params; zero copies the staged body immediately. */
void dds3MultiBlurSwap(s32 duration) {
    dds3MultiBlurBuf = Eve_PartialMultiBlurGetParams()->body;
    mulblurswapnow = 0;
    mulblurswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= DDS3_DRAW_CLEAR_TEXTURE_TRANSITION;
        Eve_PartialMultiBlurSetParam(&mulblurtarget);
    } else {
        dds3DrawFlag |= DDS3_DRAW_TEXTURE_TRANSITION_BIT;
    }
}

/* Fade textured-blur alpha from zero to staged alpha, or apply/enable it now. */
void dds3MultiBlurOn(s32 duration) {
    dds3Effect_PartialMultiBlurParam_t *params = &mulblurtarget;
    s32 requestedDuration = duration;

    mulblurabase = 0;
    mulblurnow = 0;
    mulbluratarget = (params->Blur.abgr >> 24);
    mulblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_TEXTURE_FADE_BIT;
        Eve_PartialMultiBlurSetParam(params);
        Eve_PartialMultiBlurON();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_TEXTURE_FADE_BIT;
    }
}

/* Fade staged textured-blur alpha toward zero. Immediate disable also copies params. */
void dds3MultiBlurOff(s32 duration) {
    dds3Effect_PartialMultiBlurParam_t *params = &mulblurtarget;
    s32 requestedDuration = duration;

    mulbluratarget = 0;
    mulblurnow = 0;
    mulblurabase = (params->Blur.abgr >> 24);
    mulblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_TEXTURE_FADE_BIT;
        dds3DrawFlag &= ~DDS3_DRAW_TEXTURE_TRANSITION_BIT;
        Eve_PartialMultiBlurSetParam(params);
        Eve_PartialMultiBlurOFF();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_TEXTURE_FADE_BIT;
    }
}

#define DDS3_DRAW_FILTER_FADE_BIT 0x2000
#define DDS3_DRAW_FILTER_TRANSITION_BIT 0x10000
#define DDS3_DRAW_CLEAR_FILTER_TRANSITION 0xFFFEFFFF
#define DDS3_DRAW_STAGGERED_FADE_BIT 0x800000
#define DDS3_DRAW_STAGGERED_TRANSITION_BIT 0x1000000
#define DDS3_DRAW_CLEAR_STAGGERED_TRANSITION 0xFEFFFFFF
#define DDS3_DRAW_FILTER_COUNT_LIMIT 0x65
#define DDS3_DRAW_FILTER_MAX_COUNT 0x64
#define DDS3_DRAW_STAGGERED_COUNT_LIMIT 0x29
#define DDS3_DRAW_STAGGERED_MAX_COUNT 0x28

/* Stage filter-blur parameters, counting/clamping only counts above 100.
 * The secondary values are forwarded into the shared effect parameter body. */
void dds3SetDistBlurPara(s32 filterCount, s32 size, s32 delaySpread, f32 angleStep,
                           f32 uvDisplacementAngleDegrees, f32 uvDisplacementAmplitude,
                           s32 blendControl) {
    if (filterCount >= DDS3_DRAW_FILTER_COUNT_LIMIT) {
        distblurovercnt++;
        filterCount = DDS3_DRAW_FILTER_MAX_COUNT;
    }
    distblurtarget.number = filterCount;
    distblurtarget.width = size;
    distblurtarget.interval = delaySpread;
    distblurtarget.speed = angleStep;
    distblurtarget.rotate = uvDisplacementAngleDegrees;
    distblurtarget.distortion = uvDisplacementAmplitude;
    distblurtarget.blend = blendControl;
}

/* Stage the filter blur's packed RGBA color. */
void dds3SetDistBlurRGBA(u32 rgba) {
    distblurtarget.abgr = rgba;
}

/* Stage filter-blur position spread and coordinates. */
void dds3SetDistBlurArea(s32 positionSpread, s32 x, s32 y) {
    distblurtarget.area = positionSpread;
    distblurtarget.center[0] = x;
    distblurtarget.center[1] = y;
}

extern dds3Effect_DistortionBlurParam_t dds3DistBlurBuf;
extern u16 distblurswapnow;
extern u16 distblurswapmax;

/* Save live filter-blur params; zero copies the staged body immediately. */
void dds3DistBlurSwap(s32 duration) {
    dds3DistBlurBuf = Eve_DistortionBlurGetParams()->params;
    distblurswapnow = 0;
    distblurswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= DDS3_DRAW_CLEAR_FILTER_TRANSITION;
        Eve_DistortionBlurSetParam(&distblurtarget);
    } else {
        dds3DrawFlag |= DDS3_DRAW_FILTER_TRANSITION_BIT;
    }
}

/* Fade filter alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void dds3DistBlurOn(s32 duration) {
    dds3Effect_DistortionBlurParam_t *params = &distblurtarget;
    s32 requestedDuration = duration;

    distblurabase = 0;
    distblurnow = 0;
    distbluratarget = (params->abgr >> 24);
    distblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_FILTER_FADE_BIT;
        Eve_DistortionBlurSetParam(params);
        Eve_DistortionBlurON();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_FILTER_FADE_BIT;
        Eve_DistortionBlurSetParam(params);
        Eve_DistortionBlurOFF();
    }
}

/* Fade staged filter alpha toward zero; zero disables it immediately. */
void dds3DistBlurOff(s32 duration) {
    distbluratarget = 0;
    distblurabase = (distblurtarget.abgr >> 24);
    distblurnow = 0;
    distblurmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_FILTER_FADE_BIT;
        dds3DrawFlag &= ~DDS3_DRAW_FILTER_TRANSITION_BIT;
        Eve_DistortionBlurOFF();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_FILTER_FADE_BIT;
    }
}

/* Stage staggered-blur parameters, counting/clamping only counts above 40.
 * Blend and UV-displacement inputs map to the corresponding effect fields. */
void dds3SetRipBlurPara(s32 count, f32 phaseStep, f32 spacing,
                           f32 uvDisplacementAngleDegrees,
                           f32 uvDisplacementAmplitude, f32 angleStep, s32 blendControl) {
    if (count >= DDS3_DRAW_STAGGERED_COUNT_LIMIT) {
        ripblurovercnt++;
        count = DDS3_DRAW_STAGGERED_MAX_COUNT;
    }
    ripblurtarget.number = count;
    ripblurtarget.speed = phaseStep;
    ripblurtarget.interval = spacing;
    ripblurtarget.rotate = uvDisplacementAngleDegrees;
    ripblurtarget.power = uvDisplacementAmplitude;
    ripblurtarget.frequency = angleStep;
    ripblurtarget.blend = blendControl;
}

/* Stage the staggered blur's packed RGBA color. */
void dds3SetRipBlurRGBA(u32 rgba) {
    ripblurtarget.abgr = rgba;
}

/* Stage staggered-blur size and coordinates. */
void dds3SetRipBlurArea(s32 size, s32 x, s32 y) {
    ripblurtarget.area = size;
    ripblurtarget.center[0] = x;
    ripblurtarget.center[1] = y;
}

extern dds3Effect_RippleBlurParam_t dds3RipBlurBuf __attribute__((aligned(8)));
extern u16 ripblurswapnow;
extern u16 ripblurswapmax;

/* Save live staggered-blur params; zero copies the staged body immediately. */
void dds3RipBlurSwap(s32 duration) {
    dds3RipBlurBuf = Eve_RippleBlurGetParams()->params;
    ripblurswapnow = 0;
    ripblurswapmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= DDS3_DRAW_CLEAR_STAGGERED_TRANSITION;
        Eve_RippleBlurSetParam(&ripblurtarget);
    } else {
        dds3DrawFlag |= DDS3_DRAW_STAGGERED_TRANSITION_BIT;
    }
}

/* Fade staggered alpha from zero to staged alpha. Both paths copy params;
 * a timed fade disables the effect before subsequent updates sample alpha. */
void dds3RipBlurOn(s32 duration) {
    dds3Effect_RippleBlurParam_t *params = &ripblurtarget;
    s32 requestedDuration = duration;

    ripblurabase = 0;
    ripblurnow = 0;
    ripbluratarget = (params->abgr >> 24);
    ripblurmax = requestedDuration;
    if (requestedDuration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_STAGGERED_FADE_BIT;
        Eve_RippleBlurSetParam(params);
        Eve_RippleBlurON();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_STAGGERED_FADE_BIT;
        Eve_RippleBlurSetParam(params);
        Eve_RippleBlurOFF();
    }
}

/* Fade staged staggered alpha toward zero; zero disables it immediately. */
void dds3RipBlurOff(s32 duration) {
    ripbluratarget = 0;
    ripblurabase = (ripblurtarget.abgr >> 24);
    ripblurnow = 0;
    ripblurmax = duration;
    if (duration == 0) {
        dds3DrawFlag &= ~DDS3_DRAW_STAGGERED_FADE_BIT;
        dds3DrawFlag &= ~DDS3_DRAW_STAGGERED_TRANSITION_BIT;
        Eve_RippleBlurOFF();
    }
    else {
        dds3DrawFlag |= DDS3_DRAW_STAGGERED_FADE_BIT;
    }
}

/* Interpolated parameter bodies of the per-frame update below. The event
 * polygon-movie blend helpers combine the block saved when the transition
 * started with the staged block into one of these, and the effCopy* call then
 * hands the result to the effect. */
extern dds3Effect_PartialMultiBlurParam_t key2;
extern dds3Effect_DistortionBlurParam_t dds3DistBlurCurrent;
extern dds3Effect_RippleBlurParam_t dds3BlurState __attribute__((aligned(8)));
extern dds3Effect_Blur_t dds3RectangleBlurFrameParameters;
extern dds3Effect_Filter_t dds3BlurParam;
extern dds3Effect_PartialMultiFilterParam_t dds3MFilterCurrent;

/* The blend helpers replace the parameters, which are the leading fields the
 * staged block shares with their own parameter block. */
extern void evtHokanBlur2(s32 enable, f32 t, const dds3Effect_Blur_t *from, const dds3Effect_Blur_t *to, dds3Effect_Blur_t *out);
extern void evtHokanMultBlur(s32 enable, f32 t, const dds3Effect_PartialMultiBlurParam_t *from, const dds3Effect_PartialMultiBlurParam_t *to, dds3Effect_PartialMultiBlurParam_t *out);
extern void evtHokanMultFilter(s32 enable, f32 t, const dds3Effect_PartialMultiFilterParam_t *from, const dds3Effect_PartialMultiFilterParam_t *to, dds3Effect_PartialMultiFilterParam_t *out);
extern void evtHokanDistBlur(s32 enable, f32 t, const dds3Effect_DistortionBlurParam_t *from, const dds3Effect_DistortionBlurParam_t *to, dds3Effect_DistortionBlurParam_t *out);
extern void evtHokanRipBlur(s32 enable, f32 t, const dds3Effect_RippleBlurParam_t *from, const dds3Effect_RippleBlurParam_t *to, dds3Effect_RippleBlurParam_t *out);
extern void evtHokanFilter(s32 enable, f32 t, const dds3Effect_Filter_t *from, const dds3Effect_Filter_t *to, dds3Effect_Filter_t *out);

/* Advance every running kernel-draw fade and transition by one frame. Each arm
 * keeps its control bit set while its timer runs: it reinterpolates its
 * parameter block from the effect's saved live copy or its staged copy, hands
 * that block to the effect, and clears its bit on the final frame. The overlay
 * arm ramps the vignette alpha and scale before the vignette is drawn. */
void dds3BlurUpdate(void) {
    s32 alpha;

    if (dds3DrawFlag & DDS3_DRAW_OVERLAY_TRANSITION_BIT) {
        blurnow++;
        dds3ScreenBlurAlpha = bluralphabase + blurnow * (bluralphatarget - bluralphabase) / blurmax;
        dds3ScreenBlurScale = blurscalebase + blurnow * (blurscaletarget - blurscalebase) / blurmax;
        if (dds3ScreenBlurAlpha == 0 && dds3ScreenBlurScale == 0) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        if (blurnow >= blurmax) {
            dds3DrawFlag &= DDS3_DRAW_CLEAR_OVERLAY_TRANSITION;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_RECT_TRANSITION_BIT) {
        if (blur2swapnow < blur2swapmax) {
            blur2swapnow++;
        }
        evtHokanBlur2(1, (f32)blur2swapnow / (f32)blur2swapmax, &dds3RectangleBlurTransitionStartParameters,
                        &blur2target, &dds3RectangleBlurFrameParameters);
        setdraw_xy(dds3RectangleBlurFrameParameters.xy);
        Eve_BlurSetParam(&dds3RectangleBlurFrameParameters);
        if ((dds3RectangleBlurFrameParameters.abgr >> 24) != 0) {
            Eve_BlurON();
        }
        else {
            Eve_BlurOFF();
        }
        if (blur2swapnow >= blur2swapmax) {
            dds3DrawFlag &= ~DDS3_DRAW_RECT_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_RECT_FADE_BIT) {
        if (blur2now < blur2max) {
            blur2now++;
        }
        dds3RectangleBlurFrameParameters = blur2target;
        alpha = blur2abase + blur2now *
                (blur2atarget - blur2abase) /
                blur2max;
        dds3RectangleBlurFrameParameters.abgr = (dds3RectangleBlurFrameParameters.abgr & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3RectangleBlurFrameParameters.xy);
        Eve_BlurSetParam(&dds3RectangleBlurFrameParameters);
        if (alpha == 0) {
            Eve_BlurOFF();
        }
        else {
            Eve_BlurON();
        }
        if (blur2now >= blur2max) {
            dds3DrawFlag &= ~DDS3_DRAW_RECT_FADE_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_TEXTURE_TRANSITION_BIT) {
        if (mulblurswapnow < mulblurswapmax) {
            mulblurswapnow++;
        }
        evtHokanMultBlur(1, (f32)mulblurswapnow / (f32)mulblurswapmax, &dds3MultiBlurBuf,
                        &mulblurtarget, &key2);
        Eve_PartialMultiBlurSetParam(&key2);
        if ((key2.Blur.abgr >> 24) != 0) {
            Eve_PartialMultiBlurON();
        }
        else {
            Eve_PartialMultiBlurOFF();
        }
        if (mulblurswapnow >= mulblurswapmax) {
            dds3DrawFlag &= ~DDS3_DRAW_TEXTURE_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_TEXTURE_FADE_BIT) {
        if (mulblurnow < mulblurmax) {
            mulblurnow++;
        }
        key2 = mulblurtarget;
        alpha = mulblurabase + mulblurnow *
                (mulbluratarget - mulblurabase) /
                mulblurmax;
        key2.Blur.abgr = (key2.Blur.abgr & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiBlurSetParam(&key2);
        if (alpha == 0) {
            Eve_PartialMultiBlurOFF();
        }
        else {
            Eve_PartialMultiBlurON();
        }
        if (mulblurnow >= mulblurmax) {
            dds3DrawFlag &= ~DDS3_DRAW_TEXTURE_FADE_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_FILTER_TRANSITION_BIT) {
        if (distblurswapnow < distblurswapmax) {
            distblurswapnow++;
        }
        evtHokanDistBlur(1, (f32)distblurswapnow / (f32)distblurswapmax, &dds3DistBlurBuf,
                        &distblurtarget, &dds3DistBlurCurrent);
        Eve_DistortionBlurSetParam(&dds3DistBlurCurrent);
        if ((dds3DistBlurCurrent.abgr >> 24) != 0) {
            Eve_DistortionBlurON();
        }
        else {
            Eve_DistortionBlurOFF();
        }
        if (distblurswapnow >= distblurswapmax) {
            dds3DrawFlag &= ~DDS3_DRAW_FILTER_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_FILTER_FADE_BIT) {
        if (distblurnow < distblurmax) {
            distblurnow++;
        }
        dds3DistBlurCurrent = distblurtarget;
        alpha = distblurabase + distblurnow *
                (distbluratarget - distblurabase) /
                distblurmax;
        dds3DistBlurCurrent.abgr = (dds3DistBlurCurrent.abgr & 0xFFFFFF) | (alpha << 24);
        Eve_DistortionBlurSetParam(&dds3DistBlurCurrent);
        if (alpha == 0) {
            Eve_DistortionBlurOFF();
        }
        else {
            Eve_DistortionBlurON();
        }
        if (distblurnow >= distblurmax) {
            dds3DrawFlag &= ~DDS3_DRAW_FILTER_FADE_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_STAGGERED_TRANSITION_BIT) {
        if (ripblurswapnow < ripblurswapmax) {
            ripblurswapnow++;
        }
        evtHokanRipBlur(1, (f32)ripblurswapnow / (f32)ripblurswapmax, &dds3RipBlurBuf,
                        &ripblurtarget, &dds3BlurState);
        Eve_RippleBlurSetParam(&dds3BlurState);
        if ((dds3BlurState.abgr >> 24) != 0) {
            Eve_RippleBlurON();
        }
        else {
            Eve_RippleBlurOFF();
        }
        if (ripblurswapnow >= ripblurswapmax) {
            dds3DrawFlag &= ~DDS3_DRAW_STAGGERED_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_STAGGERED_FADE_BIT) {
        if (ripblurnow < ripblurmax) {
            ripblurnow++;
        }
        memcpy(&dds3BlurState, &ripblurtarget,
               sizeof(dds3BlurState));
        alpha = ripblurabase + ripblurnow *
                (ripbluratarget - ripblurabase) /
                ripblurmax;
        dds3BlurState.abgr = (dds3BlurState.abgr & 0xFFFFFF) | (alpha << 24);
        Eve_RippleBlurSetParam(&dds3BlurState);
        if (alpha == 0) {
            Eve_RippleBlurOFF();
        }
        else {
            Eve_RippleBlurON();
        }
        if (ripblurnow >= ripblurmax) {
            dds3DrawFlag &= ~DDS3_DRAW_STAGGERED_FADE_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_COLOR_TRANSITION_BIT) {
        if (filter2swapnow < filter2swapmax) {
            filter2swapnow++;
        }
        evtHokanFilter(1, (f32)filter2swapnow / (f32)filter2swapmax, &dds3Filter2Buf,
                                        &filter2target, &dds3BlurParam);
        setdraw_xy(dds3BlurParam.xy);
        Eve_FilterSetParam(&dds3BlurParam);
        if ((dds3BlurParam.abgr >> 24) != 0) {
            Eve_FilterON();
        }
        else {
            Eve_FilterOFF();
        }
        if (filter2swapnow >= filter2swapmax) {
            dds3DrawFlag &= ~DDS3_DRAW_COLOR_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_COLOR_FADE_BIT) {
        if (filter2now < filter2max) {
            filter2now++;
        }
        dds3BlurParam = filter2target;
        alpha = filter2abase + filter2now *
                (filter2atarget - filter2abase) /
                filter2max;
        dds3BlurParam.abgr = (dds3BlurParam.abgr & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3BlurParam.xy);
        Eve_FilterSetParam(&dds3BlurParam);
        if (alpha == 0) {
            Eve_FilterOFF();
        }
        else {
            Eve_FilterON();
        }
        if (filter2now >= filter2max) {
            dds3DrawFlag &= ~DDS3_DRAW_COLOR_FADE_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_SQUARE_TRANSITION_BIT) {
        if (mfilterswapnow < mfilterswapmax) {
            mfilterswapnow++;
        }
        evtHokanMultFilter(1, (f32)mfilterswapnow / (f32)mfilterswapmax, &dds3MFilterBuf,
                        &mfiltertarget, &dds3MFilterCurrent);
        Eve_PartialMultiFilterSetParam(&dds3MFilterCurrent);
        if (dds3MFilterCurrent.Filter.color[DDS3_DRAW_ALPHA_CHANNEL] != 0) {
            Eve_PartialMultiFilterON();
        }
        else {
            Eve_PartialMultiFilterOFF();
        }
        if (mfilterswapnow >= mfilterswapmax) {
            dds3DrawFlag &= ~DDS3_DRAW_SQUARE_TRANSITION_BIT;
        }
    }
    if (dds3DrawFlag & DDS3_DRAW_SQUARE_FADE_BIT) {
        if (mfilternow < mfiltermax) {
            mfilternow++;
        }
        dds3MFilterCurrent = mfiltertarget;
        alpha = mfilterabase + mfilternow *
                (mfilteratarget - mfilterabase) /
                mfiltermax;
        dds3MFilterCurrent.Filter.rgba = (dds3MFilterCurrent.Filter.rgba & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiFilterSetParam(&dds3MFilterCurrent);
        if (alpha == 0) {
            Eve_PartialMultiFilterOFF();
        }
        else {
            Eve_PartialMultiFilterON();
        }
        if (mfilternow >= mfiltermax) {
            dds3DrawFlag &= ~DDS3_DRAW_SQUARE_FADE_BIT;
        }
    }
}

extern float fabsf(float);

/* vu0 routine: shortest-arc quaternion rotating direction vf10 to vf11. */
void dds3RotationArc(void) {
    f32 identity[4];
    f32 axis[4];
    f32 dot;
    f32 component;

    memset(identity, 0, sizeof(identity));
    identity[3] = 1.0f;

    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SWAPVECTOR_A_B();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SWAPVECTOR_A_B();

    SDF_VU0_INNERPRODUCT(dot);
    if (dot >= 0.9999f) {
        SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)identity);
        return;
    }

    if (dot <= -0.9999f) {
        SDF_VU0_GETVECTOR_Y(component);
        if (0.9999f <= fabsf(component)) {
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_SETVECTOR_Z(-1.0f);
        } else {
            SDF_VU0_GETVECTOR_Z(component);
            axis[0] = -component;
            axis[1] = 0.0f;
            SDF_VU0_GETVECTOR_X(axis[2]);
            axis[3] = 0.0f;
            SDF_VU0_LOADVECTOR(axis);
            SDF_VU0_NORMALIZE();
        }

        SDF_VU0_INNERPRODUCT(dot);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_SETVECTOR_W(dot);
        return;
    }

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(dot);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(axis);
    SDF_VU0_SETVECTOR_W(dot);
    Normalize();
}

/* vu0 routine: stage an immediate or interpolated parallel light color. */
void dds3SetLightColor(s32 frame, s32 no, f32 *col)
{
    sdf3DLight_t *light = dds3LightParam.parallel[no];
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedBaseColor;
    u32 packedTargetColor;
    u32 blendFlag;

    if (light != NULL) {
        blendFlag = 4 << (no * 2);
        if (frame == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(col);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[no]->color);
        } else {
            lightcolormax[no] = SDF_FRAMES(frame);
            lightcolornow[no] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(light);
            MMI_RGBA_PACK_128(packedBaseColor);
            baseColorWords[0] = packedBaseColor;
            dds3LightColor[no] = packedBaseColor;
            SDF_VU0_LOADVECTOR(col);
            SDF_VU0_VECTOR_W_SET_ZERO();
            MMI_RGBA_PACK_128(packedTargetColor);
            dds3LightTargetColor[no] = packedTargetColor;
            targetColorWords[0] = packedTargetColor;
        }
    }
}

void dds3SetLightDir(s32 frame, s32 no, f32 *dir)
{
    u32 blendFlag;

    if (dds3LightParam.parallel[no] != NULL) {
        blendFlag = 8 << (no * 2);
        if (frame == 0) {
            dds3DrawFlag &= ~blendFlag;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_NORMALIZE();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_STOREVECTOR(&dds3LightParam.parallel[no]->v);
        } else {
            lightdirmax[no] = SDF_FRAMES(frame);
            lightdirnow[no] = 0;
            dds3DrawFlag |= blendFlag;
            SDF_VU0_LOADVECTOR(dir);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[no]->v);
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_NORMALIZE();
            /* No memory clobber here: retail recomputes the slot offset after the call. */
            SDF_VU0_STOREVECTOR_NOMEM(dds3LightArcFrom[no]);
            dds3RotationArc();
            SDF_VU0_STOREVECTOR(dds3LightArcTo[no]);
        }
    }
}

/* vu0 routine: stage an immediate or interpolated ambient color. */
void dds3SetAmbientColor(s32 frame, f32 *col)
{
    u32 baseColorWords[4];
    u32 targetColorWords[4];
    u32 packedColor;

    if (frame == 0) {
        dds3DrawFlag &= ~0x100;
        SDF_VU0_LOADVECTOR(col);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&dds3LightParam.ambient);
    } else {
        ambientmax = SDF_FRAMES(frame);
        ambientnow = 0;
        dds3DrawFlag |= 0x100;
        SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        MMI_RGBA_PACK_128(packedColor);
        baseColorWords[0] = packedColor;
        ambientbase = packedColor;
        SDF_VU0_LOADVECTOR(col);
        SDF_VU0_VECTOR_W_SET_ONE();
        MMI_RGBA_PACK_128(packedColor);
        targetColorWords[0] = packedColor;
        ambienttarget = packedColor;
    }
}

/* vu0 routine: stage an immediate or interpolated packed color. */
void dds3SetFogColor(s32 frame, f32 *col)
{
    u32 targetColorWords[4];
    u32 packedTargetColor;

    SDF_VU0_LOADVECTOR(col);
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_255(packedTargetColor);
    targetColorWords[0] = packedTargetColor;
    if (frame == 0) {
        dds3DrawFlag &= ~0x200;
        dds3LightParam.fog.color = targetColorWords[0];
    } else {
        dds3DrawFlag |= 0x200;
        fogrgbmax = SDF_FRAMES(frame);
        fogrgbtarget = targetColorWords[0];
        fogrgbbase = dds3LightParam.fog.color;
        fogrgbnow = 0;
    }
}

void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 near, f32 far)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x400;
        dds3FogState.fogMax = max;
        dds3FogState.fogMin = min;
        dds3FogState.fogNear = near;
        dds3FogState.fogFar = far;
    } else {
        f32 oldMax = dds3FogState.fogMax;
        f32 oldMin = dds3FogState.fogMin;
        f32 oldNear = dds3FogState.fogNear;
        f32 oldFar = dds3FogState.fogFar;
        dds3DrawFlag |= 0x400;
        fogparamax = SDF_FRAMES(frame);
        dds3FogParam.fogMax = oldMax;
        dds3FogParam.fogMin = oldMin;
        dds3FogParam.fogNear = oldNear;
        dds3FogParam.fogFar = oldFar;
        dds3FogColor.fogMax = max;
        dds3FogColor.fogMin = min;
        dds3FogColor.fogNear = near;
        dds3FogColor.fogFar = far;
        fogparanow = 0;
    }
}

/* SDK float-input color conversion, shared by the six light/ambient/fog inputs. */
static inline void dds3UnpackLightColor(u32 color, f32 unit) {
    u32 rgba[4] __attribute__((aligned(16)));
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* vu0 routine: advance staged light, ambient, fog and face divide blends. */
void dds3SceneLightUpdateFirst(void) {
    s32 i;
    u32 mask;
    f32 ratio;

    for (i = 0, mask = 4; i < 3; i++, mask <<= 2) {
        if (dds3LightParam.parallel[i] == NULL) {
            dds3DrawFlag &= ~(mask | (mask << 1));
        } else {
            if (dds3DrawFlag & mask) {
                ++lightcolornow[i];
                ratio = (f32)lightcolornow[i] / lightcolormax[i];
                dds3UnpackLightColor(dds3LightColor[i], 0.0078125f);
                SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
                SDF_VU0_COPYVECTOR_A_TO_B();
                dds3UnpackLightColor(dds3LightTargetColor[i], 0.0078125f);
                SDF_VU0_SCALEVECTOR_V0(ratio);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(dds3LightParam.parallel[i]);
                if (lightcolornow[i] >= lightcolormax[i]) {
                    dds3DrawFlag &= ~mask;
                }
            }
            if (dds3DrawFlag & (mask << 1)) {
                ++lightdirnow[i];
                ratio = (f32)lightdirnow[i] / lightdirmax[i];
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_VECTOR_W_SET_ONE();
                SDF_VU0_LOADVECTOR_B(&dds3LightArcTo[i]);
                Lerp(ratio);
                ToMatrix();
                SDF_VU0_LOADVECTOR(&dds3LightArcFrom[i]);
                SDF_VU0_APPLYMATRIX();
                SDF_VU0_STOREVECTOR_NOMEM(&dds3LightParam.parallel[i]->v);
                if (lightdirnow[i] >= lightdirmax[i]) {
                    dds3DrawFlag &= ~(mask << 1);
                }
            }
        }
    }
    if (dds3DrawFlag & 0x100) {
        ++ambientnow;
        ratio = (f32)ambientnow / ambientmax;
        dds3UnpackLightColor(ambientbase, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3UnpackLightColor(ambienttarget, 0.0078125f);
        SDF_VU0_SCALEVECTOR_V0(ratio);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR_NOMEM(&dds3AmbientColor);
        if (ambientnow >= ambientmax) {
            dds3DrawFlag &= ~0x100;
        }
    }
    if (dds3DrawFlag & 0x200) {
        ++fogrgbnow;
        ratio = (f32)fogrgbnow / fogrgbmax;
        dds3UnpackLightColor(fogrgbbase, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3UnpackLightColor(fogrgbtarget, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(ratio);
        SDF_VU0_ADDVECTOR();
        {
            u32 rgba[4] __attribute__((aligned(16)));
            MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
            dds3FogState.color = rgba[0];
        }
        if (fogrgbnow >= fogrgbmax) {
            dds3DrawFlag &= ~0x200;
        }
    }
    if (dds3DrawFlag & 0x400) {
        ++fogparanow;
        ratio = (f32)fogparanow / fogparamax;
        SDF_VU0_LOADVECTOR(&dds3FogParam);
        SDF_VU0_LOADVECTOR_B(&dds3FogColor);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
        SDF_VU0_SCALEVECTOR_B_V0(ratio);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(dds3FogState.fogMax);
        SDF_VU0_GETVECTOR_Y(dds3FogState.fogMin);
        SDF_VU0_GETVECTOR_Z(dds3FogState.fogNear);
        SDF_VU0_GETVECTOR_W(dds3FogState.fogFar);
        if (fogparanow >= fogparamax) {
            dds3DrawFlag &= ~0x400;
        }
    }
    if (dds3DrawFlag & 0x4000) {
        ++sbnow;
        ratio = (f32)sbnow / sbmax;
        sdfFaceDivSoften = softenbase * (1.0f - ratio) + softentarget * ratio;
        sdfFaceDivBias = biasbase * (1.0f - ratio) + biastarget * ratio;
        if (sbnow >= sbmax) {
            dds3DrawFlag &= ~0x4000;
        }
    }
}

void dds3SetSoftenBias(s32 frame, f32 soften, f32 bias)
{
    if (frame == 0) {
        dds3DrawFlag &= ~0x4000;
        sdfFaceDivSoften = soften;
        sdfFaceDivBias = bias;
    } else {
        f32 psoften = sdfFaceDivSoften;
        f32 pbias = sdfFaceDivBias;
        dds3DrawFlag |= 0x4000;
        sbmax = SDF_FRAMES(frame);
        softenbase = psoften;
        softentarget = soften;
        biasbase = pbias;
        biastarget = bias;
        sbnow = 0;
    }
}

void eglInit(void) {
    if (eglMaterial == NULL) {
        eglMaterial = sdfMatNew();
        ((EvtDrawState *)eglMaterial)->value1C = 1.0f;
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
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
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
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

/* Select a primary-context ALPHA equation by mode; unknown modes use 0x44. */

INCLUDE_RODATA(const s32, "kernel/dds3KernelDraw", D_004112F0);

INCLUDE_RODATA(const s32, "kernel/dds3KernelDraw", D_00411308);

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
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
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
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

void evtSubmitPrimaryFramePacket(void) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    SdfDmaReferenceChainPacket *framePacket;
    sdfDrawTagInit(list);
    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(EVT_FRAME_REFERENCE_PACKET_BYTES);
    sdfGrSetFrameMaskRGB(dds3FrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

void evtSubmitAlternateFramePacket(void) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    SdfDmaReferenceChainPacket *framePacket;
    sdfDrawTagInit(list);
    framePacket = (SdfDmaReferenceChainPacket *)sdfGrAllocPacketBuff(EVT_FRAME_REFERENCE_PACKET_BYTES);
    sdfGrSetFrameMaskReset(dds3FrameDrawPacketRecords[dds3GetFrameID()].baseBytes, framePacket, 0);
    sdfDrawTagLinkRefGif(list, (u32)framePacket);
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
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
    surface->regFunc(surface, list);
}

#ifdef NON_MATCHING
#include "common.h"

extern u32 egl_OT_Offset;
extern sdfGrOt_t sdfGrOtTable[];

/* Submit a textured quad with four corner colours to the current draw
 * surface, as evtSubmitTexturedGradientQuad does, but placed by four screen
 * points (top left, top right, bottom left, bottom right) where that one takes
 * a rectangle. The points are float pixels: x times 16 and y times 8 (the
 * field's half height), each truncated, then offset by 0x7000 and 0x7900 as
 * there. The texture rectangle and the colours (top left, top right, bottom
 * right, bottom left) are as there. Upstream: INCLUDE_ASM in
 * kernel/dds3KernelDraw; DDS2's own, written from the retail routine. Nothing
 * calls it directly, and its float and integer parameters travel in separate
 * registers, so their interleaving is unknown: the order here follows
 * evtSubmitTexturedGradientQuad's. */
void evtSubmitTexturedGradientQuadPoints(f32 topLeftX, f32 topLeftY, f32 topRightX, f32 topRightY,
                                         f32 bottomLeftX, f32 bottomLeftY, f32 bottomRightX,
                                         f32 bottomRightY, s32 u, s32 v, s32 textureWidth,
                                         s32 textureHeight, u32 color0, u32 color1, u32 color2,
                                         u32 color3, sdfTexHandle_t *texture) {
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, texture, 0);
    uLeft = u << 4;
    vTop = v << 4;
    uRight = uLeft + (textureWidth << 4);
    vBottom = vTop + (textureHeight << 4);
    sdfPrimLinkGt4(list, 0x40,
                   (s32)(topLeftX * 16.0f) + 0x7000, (s32)(topLeftY * 8.0f) + 0x7900, uLeft, vTop, color0,
                   (s32)(topRightX * 16.0f) + 0x7000, (s32)(topRightY * 8.0f) + 0x7900, uRight, vTop, color1,
                   (s32)(bottomLeftX * 16.0f) + 0x7000, (s32)(bottomLeftY * 8.0f) + 0x7900, uLeft, vBottom,
                   color3,
                   (s32)(bottomRightX * 16.0f) + 0x7000, (s32)(bottomRightY * 8.0f) + 0x7900, uRight, vBottom,
                   color2,
                   -1, NULL);
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", evtSubmitTexturedGradientQuadPoints);
#endif

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
void evtSubmitGradientRectAtDepth(s32 x, s32 y, s32 w, s32 h, u32 depth, s32 color0, s32 color1, s32 color2, s32 color3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    SdfDrawPacket *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;

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
    descriptor->regFunc(descriptor, command);
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
    sdfGrOt_t *descriptor;

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
    descriptor->regFunc(descriptor, command);
}

extern u64 eglPolyStripWord[]; /* index table; only the first 8 bytes are used */

/* Preserve the native 0,2,3,1 vertex/index order; W components are not initialized here. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 i0, u32 i1, u32 i2, u32 i3) {
    sdf3DPrimPolyParm_t desc;
    DrawVec4 verts[EVT_QUAD_VERTEX_COUNT];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    void *list;
    sdfGrOt_t *surface;

    list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, eglMaterial, 0);
    memset(&desc, 0, EVT_QUAD_DESCRIPTOR_BYTES);
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = EVT_QUAD_VERTEX_COUNT;
    desc.vertex = &verts[0].x;
    desc.vertexColor = indices;
    desc.faceIndex = strip;
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
    strip[0] = eglPolyStripWord[0];
    sdfDrawTagLink(list, (u32)sdf3DPrimMakePoly(&desc));
    surface = &sdfGrOtTable[egl_OT_Offset];
    surface->regFunc(surface, list);
}

/* Submit the same native vertex permutation with rectangular UVs, then queue asset release. */
void eglLinkTexPoly4(s32 i0, f32 x0, f32 y0, f32 z0, s32 i1, f32 x1, f32 y1, f32 z1, s32 i2, f32 x2, f32 y2, f32 z2, s32 i3, f32 x3, f32 y3, f32 z3, u32 bits, f32 u0, f32 v0, f32 u1, f32 v1) {
    sdf3DPrimPolyParm_t desc;
    f32 verts[16];
    s32 indices[EVT_QUAD_VERTEX_COUNT];
    u64 strip[2];
    f32 uvs[8];
    void *asset;
    void *list;
    sdfGrOt_t *surface;

    asset = sdfMatNew();
    sdfMatSetTex(asset, (sdfTexHandle_t *)bits);
    list = (void *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, asset, 0);
    memset(&desc, 0, EVT_QUAD_DESCRIPTOR_BYTES);
    desc.color = 0x80808080;
    desc.faceCount = 2;
    desc.pointCount = EVT_QUAD_VERTEX_COUNT;
    desc.vertex = verts;
    desc.vertexColor = indices;
    desc.faceIndex = strip;
    desc.uv = uvs;
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
    strip[0] = eglPolyStripWord[0];
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
    surface->regFunc(surface, list);
    sdfMatFreeDelayed((sdfMat_t *)asset);
}

void eglLinkLine(f32 x, f32 y, f32 z, f32 u, f32 v, f32 w, u32 first, u32 second) {
    void *list;
    eglLineVerts[0][0] = x;
    eglLineVerts[0][1] = y;
    eglLineVerts[0][2] = z;
    eglLineVerts[1][0] = u;
    eglLineVerts[1][1] = v;
    eglLineVerts[1][2] = w;
    eglLineColors[1] = second;
    eglLineColors[0] = first;
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (u32)effDebugDrawWireModel(eglLineVerts, eglLineColors, 2, 0x80));
    {
        sdfGrOt_t *surface = &sdfGrOtTable[egl_OT_Offset];
        surface->regFunc(surface, list);
    }
}

void eglLinkPrint(s32 x, s32 y, s32 packetArg, s32 drawArg) {
    SifCommand sifParameters;
    void *list;
    void *packet;
    sdfGrOt_t *surface;
    list = dds3FontReqAllocDrawTag();
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDevSetPrintCB(&sifParameters, x * 16 + 0x7000, y * 8 + 0x7900, 0x0FFFFF80, packetArg);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&sifParameters, (const char *)drawArg));
    surface = &sdfDebugGrOt;
    surface->regFunc(surface, list);
}

void eglLinkKanji(s32 width, s32 height, s32 colors, const char *text) {
    struct frFontHandle_t *glyph;

    glyph = itfCreateMeasuredTextGlyph(width << 4, height << 3, 0, colors, text, NULL);
    frDrawFont(glyph);
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
    selectionState = sdfChipAllocZeroDirect(EVT_SELECTION_ALLOCATION_BYTES);
    evtSelectionState = selectionState;
    if (countLimit == 0) {
        selectionState->limit = EVT_SEL_STATE_UNBOUNDED_LIMIT;
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
    if (selectionState->limit > selectionState->count ||
        selectionState->limit == EVT_SEL_STATE_UNBOUNDED_LIMIT) {
        evtUpdateSelectionStateInterpolation();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

#ifdef NON_MATCHING
#include "common.h"
#include "fpu.h"

extern u32 dds3DrawFlag;

extern void evtUpdateSelectionStateInterpolation(void);

extern f32 aViewTarget[];
/* Advances the camera-shake selection state by one frame: ramps the per-axis
 * shake limit up over its first frames, holds it, then ramps it down as the
 * frame limit approaches, and on each frame random-walks the shake offset on
 * all three axes within that limit (adding on odd frames, subtracting on even),
 * keeping the vertical shake from pushing the view eye above its floor unless
 * the draw-control flag is set. Upstream: INCLUDE_ASM in
 * kernel/dds3KernelDraw in both games; written from the retail
 * routine. It differs from retail in structure but not
 * behavior: retail keeps the random, limit and jitter values in stack arrays,
 * reloads the state pointer from memory in each iteration, tests a dead
 * zero-limit case and repeats the whole axis loop for odd and even frames,
 * which the body folds into one loop. */
void evtUpdateSelectionStateInterpolation(void) {
    EvtSelState *state = evtSelectionState;
    f32 *shake = dds3QuakeOffset;
    s32 randomPercent;
    s32 limitPercent;
    f32 jitter;
    s32 axis;

    switch (state->mode) {
    case 0:
        if (state->rampUpFrames <= state->count) {
            state->mode = 1;
            state->offset[0] = state->amplitude[0];
            state->offset[1] = state->amplitude[1];
            state->offset[2] = state->amplitude[2];
        } else {
            for (axis = 0; axis < 3; axis++) {
                state->offset[axis] = (f32)(state->amplitude[axis] * (state->count + 1)) / (f32)state->rampUpFrames;
            }
        }
        break;
    case 1:
        if (state->count >= state->limit - state->rampDownFrames) {
            if (state->limit != -1) {
                state->mode = 2;
            }
        }
        break;
    case 2:
        for (axis = 0; axis < 3; axis++) {
            state->offset[axis] = (f32)(state->amplitude[axis] * (state->limit - state->count)) / (f32)state->rampDownFrames;
        }
        break;
    }

    for (axis = 0; axis < 3; axis++) {
        randomPercent = (s32)(dds3GetRandFloatA(NULL) * 100.0f);
        limitPercent = (s32)(state->offset[axis] * 100.0f);
        jitter = (f32)(randomPercent % limitPercent) / 10.0f;
        if (state->count & 1) {
            shake[axis] += jitter;
            if (state->offset[axis] <= ffabsf(shake[axis])) {
                shake[axis] = state->offset[axis];
            }
        } else {
            shake[axis] -= jitter;
            if (shake[axis] <= -state->offset[axis]) {
                shake[axis] = -state->offset[axis];
            }
        }
        if (!(dds3DrawFlag & 0x20000000) && axis == 1) {
            if (-11.0f < aViewTarget[1] + shake[1]) {
                shake[1] = 0.0f;
            }
        }
    }
    state->count++;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", evtUpdateSelectionStateInterpolation);
#endif

void evtDestroySelectionState(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (evtSelectionStateActive != 0) {
        sdfChipFreeDirect(evtSelectionState);
        evtSelectionStateActive = 0;
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_STOREVECTOR(dds3QuakeOffset);
    }
}

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", egl_OT_Offset);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CBC);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTex);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CC4);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderTexture);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CCC);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CCE);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3RenderDrawEnv);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3DrawFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurFlag);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurAlpha);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3ScreenBlurScale);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CE0);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CE4);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CE8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CEA);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CEC);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CEE);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", fadergba);

SDATA_PAD(4);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3BackgroundFadeColor);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435CFB);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3BackgroundFadeMode);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3BackgroundFadeFirstRamp);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", dds3BackgroundFadeSecondRamp);

SDATA_PAD(2);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", distblurovercnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", ripblurovercnt);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_00435D10);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", evtSelectionStateActive);

