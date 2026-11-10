#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "bill_object_api.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_map_position_api.h"
#include "ps2asm.h"
#include "sdf_vu_rotation_axes.h"
#include "sdf_sif_command.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "eff_param.h"
#include "sdf_heap_api.h"
#include "btl_sound.h"
#include "eff_blur.h"
#include "eff_rectangle_blur_api.h"
#include "eff_event_draw.h"
#include "eff.h"
#include "eff_resource_rect_api.h"
#include "eff_math.h"
#include "eff_channel.h"
#include "eff_event.h"
#include "eff_event_work_api.h"
#include "eff_event_sound.h"
#include "eff_event_setup.h"
#include "eff_event_setup_readiness.h"
#include "eff_event_bill.h"
#include "eff_event_bill_batch.h"
#include "sdf_vu_transform_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_projection.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "sdf_gs_blend.h"
#include "sdf_packet_alloc.h"
#include "eff_packet_params.h"
#include "sdf_asset_create.h"
#include "sdf_asset_state.h"
#include "evt_unit.h"
#include "mdl.h"
#include "eff_channel_layout.h"
#include "fr_font.h"
#include "font_mem_control_api.h"

#define EFF_EVENT_VECTOR_COMPONENTS 4

#define EFF_EVENT_BLUR_SOURCE_BYTES 0x28

#define EFF_EVENT_BLUR_RESOURCE_INDEX 2

#define EFF_EVENT_STAGGERED_RESOURCE_INDEX 3

#define EFF_EVENT_SQUARE_RESOURCE_INDEX 0

#define EFF_EVENT_STAGGERED_SLOT_COUNT 4

#define EFF_EVENT_NEUTRAL_COLOR 0x80808080

#define EFF_EVENT_EVENT_RECORD_BYTES 0x30

#define EFF_EVENT_COMPACT_WORK_BYTES 0x38

#define EFF_EVENT_HALF_TURN 3.14159265f

#define EFF_EVENT_RANDOM_MIDPOINT 0.5f

#define EFF_EVENT_JITTER_RANGE 0.3f

#define EFF_EVENT_JITTER_BASE 0.7f

extern EffScreenDrawParams Eve_Blur;

extern EffBlurTemplate *hEve_PartialMultiBlur;

extern EffBlurScatterWork *hEve_DistortionBlur;

extern EffBlurScaleWork *hEve_RippleBlur;

extern EffScreenDrawParams D_003B2238;

extern dds3Effect_Filter_t Eve_Filter;

extern EffResourceRectWork *hEve_PartialMultiFilter;

extern u8 D_003B2208[];

extern u8 D_003B21D8[];

extern u8 D_003B2278[];

extern u8 D_003B22A0[];

extern EffScreenDrawParams D_003B22D0;

extern dds3Effect_PartialMultiBlurParam_t effTexturedBlurSetupParameters;

extern dds3Effect_DistortionBlurParam_t effScatterBlurSetupParameters;

extern EffScreenDrawParams D_003B2778;

extern dds3Effect_Filter_t D_003B28B8;

extern dds3Effect_PartialMultiFilterParam_t effResourceRectangleSetupParameters;

extern dds3Effect_RippleBlurParam_t effStaggeredBlurSetupParameters;

extern s8 Eve_PartialMultiFilterShow;

extern s8 Eve_FilterShow;

extern s8 D_00436463;

extern s8 Eve_RippleBlurShow;

extern s8 Eve_DistortionBlurShow;

extern s8 Eve_PartialMultiBlurShow;

extern s8 Eve_BlurShow;

extern void dds3EffectRippleBlurDraw(EffBlurScaleWork *arg);

extern void dds3EffectFilter(dds3Effect_Filter_t *arg);

extern sdfGrOt_t sdfGrOtTable[];

extern sdfMat_t *effDebugSelectCursorMat;

extern f32 effDebugSelectCursorAngle;

void Eve_BlurON(void) {
    Eve_BlurShow = 1;
}

void Eve_BlurOFF(void) {
    Eve_BlurShow = 0;
}

/* Copy only the 0x28-byte source prefix; retain the screen record's trailing bytes. */
void Eve_BlurSetParam(const dds3Effect_Blur_t *parameters) {
    memcpy(&Eve_Blur, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the rectangle-blur parameter block by address. */
EffScreenDrawParams *Eve_BlurGetParam(void) {
    return &Eve_Blur;
}

void Eve_PartialMultiBlurON(void) {
    Eve_PartialMultiBlurShow = 1;
}

void Eve_PartialMultiBlurOFF(void) {
    Eve_PartialMultiBlurShow = 0;
}

/* Copy the pixel rectangle only; retain its selected source resource. */
void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *parameters) {
    hEve_PartialMultiBlur->body = *parameters;
}

/* Return the current textured-blur work without retaining it. */
EffBlurTemplate *Eve_PartialMultiBlurGetParams(void) {
    return hEve_PartialMultiBlur;
}

void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *texture) {
    hEve_PartialMultiBlur->texture = texture;
}

/* Select the same resource-table entry used by the filter-blur initializer. */
void Eve_PartialMultiBlurSetDefTex(void) {
    hEve_PartialMultiBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void Eve_DistortionBlurON(void) {
    Eve_DistortionBlurShow = 1;
}

void Eve_DistortionBlurOFF(void) {
    Eve_DistortionBlurShow = 0;
}

/* Update scatter parameters without replacing the owned allocation or slots. */
void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *parameters) {
    hEve_DistortionBlur->params = *parameters;
}

/* Return the current filter-blur work without retaining it. */
EffBlurScatterWork *Eve_DistortionBlurGetParams(void) {
    return hEve_DistortionBlur;
}

void Eve_DistortionBlurSetTex(sdfTexHandle_t *sourceHandle) {
    hEve_DistortionBlur->texture = sourceHandle;
}

/* Install the filter-blur resource-table texture without changing its slots. */
void Eve_DistortionBlurSetDefTex(void) {
    hEve_DistortionBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_BLUR_RESOURCE_INDEX);
}

void Eve_RippleBlurON(void) {
    Eve_RippleBlurShow = 1;
}

void Eve_RippleBlurOFF(void) {
    Eve_RippleBlurShow = 0;
}

/* Update scale parameters without replacing the owned allocation or slots. */
void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *parameters) {
    hEve_RippleBlur->params = *parameters;
}

/* Return the current staggered-blur work without retaining it. */
EffBlurScaleWork *Eve_RippleBlurGetParams(void) {
    return hEve_RippleBlur;
}

void Eve_RippleBlurSetTex(sdfTexHandle_t *sourceHandle) {
    hEve_RippleBlur->texture = sourceHandle;
}

/* Install the staggered-blur resource-table texture without changing its slots. */
void Eve_RippleBlurSetDefTex(void) {
    hEve_RippleBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}

void effEnableFramebufferQuad(void) {
    D_00436463 = 1;
}

void effDisableFramebufferQuad(void) {
    D_00436463 = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void effCopyFramebufferQuadParameters(const dds3Effect_Blur_t *source) {
    memcpy(&D_003B2238, source, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the framebuffer-quad parameter block by address. */
EffScreenDrawParams *effGetCh73Params(void) {
    return &D_003B2238;
}

void Eve_FilterON(void) {
    Eve_FilterShow = 1;
}

void Eve_FilterOFF(void) {
    Eve_FilterShow = 0;
}

/* Replace the complete solid-rectangle parameter block. */
void Eve_FilterSetParam(dds3Effect_Filter_t *parameters) {
    Eve_Filter = *parameters;
}

/* Expose the solid-rectangle parameter block by address. */
dds3Effect_Filter_t *Eve_FilterGetParams(void) {
    return &Eve_Filter;
}

void Eve_PartialMultiFilterON(void) {
    Eve_PartialMultiFilterShow = 1;
}

void Eve_PartialMultiFilterOFF(void) {
    Eve_PartialMultiFilterShow = 0;
}

/* Copy the resource template body while preserving its selected resource. */
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    hEve_PartialMultiFilter->params = *parameters;
}

/* Return the current textured-square work without retaining it. */
EffResourceRectWork *Eve_PartialMultiFilterGetParams(void) {
    return hEve_PartialMultiFilter;
}

void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *texture) {
    hEve_PartialMultiFilter->texture = texture;
}

/* Install the textured-square resource-table texture while retaining its body. */
void Eve_PartialMultiFilterSetDefTex(void) {
    hEve_PartialMultiFilter->texture = dds3BillboardGetGeneralTex(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void Eve_EffectInit(void) {
    hEve_PartialMultiBlur = dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)D_003B2208);
    hEve_DistortionBlur = dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)D_003B21D8);
    hEve_PartialMultiFilter = dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)D_003B2278);
    hEve_RippleBlur = dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)D_003B22A0);
    Eve_RippleBlurGetParams()->params.count = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

/* Dispatch enabled draw families independently, in their native order. */
void effDispatchActive(void) {
    if (Eve_BlurShow) {
        dds3EffectBlur(&Eve_Blur.source);
    }
    if (Eve_PartialMultiBlurShow) {
        dds3EffectPartialMultiBlurDraw(hEve_PartialMultiBlur);
    }
    if (Eve_DistortionBlurShow) {
        dds3EffectDistortionBlurDraw(hEve_DistortionBlur);
    }
    if (Eve_RippleBlurShow) {
        dds3EffectRippleBlurDraw(hEve_RippleBlur);
    }
    if (D_00436463) {
        effBlurDrawFramebufferQuad(&D_003B2238.source);
    }
    if (Eve_FilterShow) {
        dds3EffectFilter(&Eve_Filter);
    }
    if (Eve_PartialMultiFilterShow) {
        effResourceRectDrawPixels(hEve_PartialMultiFilter);
    }
}

/* callbackResult must be valid whenever callback is present. */
extern EffEventSetupRecord effRectangleBlurSetupRecord;

extern s8 effRectangleBlurSetupReady;

extern s8 D_0040B7DB[];

extern void effEventSetupRecordNoOp();

extern void effEventSetupBeforeRecordNoOp();

extern void effEventSetupAfterRecordNoOp();

/* Prepare once per flag cycle, then run the registered operations while ready.
 * A negative control byte clears readiness only after those operations. */
EffEventSetupReadiness effUpdateCh72Params(void) {
    if (effRectangleBlurSetupReady == 0) {
        if (effRectangleBlurSetupRecord.callback != 0) {
            *effRectangleBlurSetupRecord.callbackResult = effRectangleBlurSetupRecord.callback(effRectangleBlurSetupRecord.source);
        }
        if (effRectangleBlurSetupRecord.destination != 0 && effRectangleBlurSetupRecord.source != 0) {
            memcpy(effRectangleBlurSetupRecord.destination, effRectangleBlurSetupRecord.source, effRectangleBlurSetupRecord.copyBytes);
        }
        effRectangleBlurSetupReady = 1;
    }
    if ((u8)effRectangleBlurSetupReady != 0) {
        effReturnEventSetupWord((u32)&effRectangleBlurSetupRecord);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&effRectangleBlurSetupRecord);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        effRectangleBlurSetupReady = 0;
    }
    return effRectangleBlurSetupReady;
}

/* Expose this screen-draw setup block; no parameter copy is made. */
EffScreenDrawParams *effGetLoadDescA(void) {
    return &D_003B22D0;
}

/* Replace the screen-draw source prefix while preserving the trailing bytes. */
void effSetLoadDescA(const dds3Effect_Blur_t *parameters) {
    memcpy(&D_003B22D0, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffEventSetupRecord mblurDebugMenu;

extern s8 mblurDone;

/* Prepare the blur-template callback from its explicit parameter block,
 * then process the channel; its optional copy still uses source/destination. */
EffEventSetupReadiness effEventAdvanceBlurTemplateSetup(void) {
    if (mblurDone == 0) {
        if (mblurDebugMenu.callback != 0) {
            *mblurDebugMenu.callbackResult = mblurDebugMenu.callback(&effTexturedBlurSetupParameters);
        }
        if (mblurDebugMenu.destination != 0 && mblurDebugMenu.source != 0) {
            memcpy(mblurDebugMenu.destination, mblurDebugMenu.source, mblurDebugMenu.copyBytes);
        }
        mblurDone = 1;
    }
    if ((u8)mblurDone != 0) {
        effReturnEventSetupWord((u32)&mblurDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&mblurDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        mblurDone = 0;
    }
    return mblurDone;
}

/* Expose the blur-template setup parameters by address. */
dds3Effect_PartialMultiBlurParam_t *effEventGetBlurTemplateSetupParams(void) {
    return &effTexturedBlurSetupParameters;
}

/* Replace the setup template; this does not modify the active blur work. */
void effEventSetBlurTemplateParameters(dds3Effect_PartialMultiBlurParam_t *parameters) {
    effTexturedBlurSetupParameters = *parameters;
}

extern EffEventSetupRecord dblurDebugMenu;

extern s8 dblurDone;

/* Prepare scatter setup from its explicit parameters, then process the channel. */
EffEventSetupReadiness effEventAdvanceScatterBlurSetup(void) {
    if (dblurDone == 0) {
        if (dblurDebugMenu.callback != 0) {
            *dblurDebugMenu.callbackResult = dblurDebugMenu.callback(&effScatterBlurSetupParameters);
        }
        if (dblurDebugMenu.destination != 0 && dblurDebugMenu.source != 0) {
            memcpy(dblurDebugMenu.destination, dblurDebugMenu.source, dblurDebugMenu.copyBytes);
        }
        dblurDone = 1;
    }
    if ((u8)dblurDone != 0) {
        effReturnEventSetupWord((u32)&dblurDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&dblurDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        dblurDone = 0;
    }
    return dblurDone;
}

/* Expose the scatter-blur setup parameters by address. */
dds3Effect_DistortionBlurParam_t *effEventGetScatterBlurSetupParams(void) {
    return &effScatterBlurSetupParameters;
}

/* Replace setup parameters without replacing the active filter-blur work. */
void effEventSetScatterBlurParameters(dds3Effect_DistortionBlurParam_t *parameters) {
    effScatterBlurSetupParameters = *parameters;
}

extern EffEventSetupRecord effFramebufferQuadSetupRecord;

extern s8 D_00436504;

/* Prepare from the channel's source, process it, then honor the control-byte reset. */
EffEventSetupReadiness effEventAdvanceScreenDrawSetup(void) {
    if (D_00436504 == 0) {
        if (effFramebufferQuadSetupRecord.callback != 0) {
            *effFramebufferQuadSetupRecord.callbackResult = effFramebufferQuadSetupRecord.callback(effFramebufferQuadSetupRecord.source);
        }
        if (effFramebufferQuadSetupRecord.destination != 0 && effFramebufferQuadSetupRecord.source != 0) {
            memcpy(effFramebufferQuadSetupRecord.destination, effFramebufferQuadSetupRecord.source, effFramebufferQuadSetupRecord.copyBytes);
        }
        D_00436504 = 1;
    }
    if ((u8)D_00436504 != 0) {
        effReturnEventSetupWord((u32)&effFramebufferQuadSetupRecord);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&effFramebufferQuadSetupRecord);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436504 = 0;
    }
    return D_00436504;
}

/* Expose the other screen-draw setup block by address. */
EffScreenDrawParams *effGetLoadDescD(void) {
    return &D_003B2778;
}

/* Replace this screen-draw source prefix without changing trailing bytes. */
void effSetLoadDescD(const dds3Effect_Blur_t *parameters) {
    memcpy(&D_003B2778, parameters, EFF_EVENT_BLUR_SOURCE_BYTES);
}

extern EffEventSetupRecord filterDebugMenu;

extern s8 evtViewerDirty;

/* Prepare solid-rectangle setup from the channel source, then process it. */
EffEventSetupReadiness evtViewerCheckDirty(void) {
    if (evtViewerDirty == 0) {
        if (filterDebugMenu.callback != 0) {
            *filterDebugMenu.callbackResult = filterDebugMenu.callback(filterDebugMenu.source);
        }
        if (filterDebugMenu.destination != 0 && filterDebugMenu.source != 0) {
            memcpy(filterDebugMenu.destination, filterDebugMenu.source, filterDebugMenu.copyBytes);
        }
        evtViewerDirty = 1;
    }
    if ((u8)evtViewerDirty != 0) {
        effReturnEventSetupWord((u32)&filterDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&filterDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        evtViewerDirty = 0;
    }
    return evtViewerDirty;
}

/* Expose the solid-rectangle setup parameters by address. */
dds3Effect_Filter_t *effEventGetSolidRectangleSetupParams(void) {
    return &D_003B28B8;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void effEventSetSolidRectangleParameters(dds3Effect_Filter_t *parameters) {
    D_003B28B8 = *parameters;
}

extern EffEventSetupRecord mfilterDebugMenu;

extern s8 D_0043651C;

/* Prepare resource-template setup from its explicit body, then process the channel. */
EffEventSetupReadiness mfilterRun(void) {
    if (D_0043651C == 0) {
        if (mfilterDebugMenu.callback != 0) {
            *mfilterDebugMenu.callbackResult = mfilterDebugMenu.callback(&effResourceRectangleSetupParameters);
        }
        if (mfilterDebugMenu.destination != 0 && mfilterDebugMenu.source != 0) {
            memcpy(mfilterDebugMenu.destination, mfilterDebugMenu.source, mfilterDebugMenu.copyBytes);
        }
        D_0043651C = 1;
    }
    if ((u8)D_0043651C != 0) {
        effReturnEventSetupWord((u32)&mfilterDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&mfilterDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043651C = 0;
    }
    return D_0043651C;
}

/* Expose the resource-template setup body by address. */
dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void) {
    return &effResourceRectangleSetupParameters;
}

/* Replace the setup body without replacing the active textured-square work. */
void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    effResourceRectangleSetupParameters = *parameters;
}

extern EffEventSetupRecord rblurDebugMenu;

extern s8 rblurDone;

/* Prepare scale-blur setup from its explicit parameters, then process the channel. */
EffEventSetupReadiness rblurProcess(void) {
    if (rblurDone == 0) {
        if (rblurDebugMenu.callback != 0) {
            *rblurDebugMenu.callbackResult = rblurDebugMenu.callback(&effStaggeredBlurSetupParameters);
        }
        if (rblurDebugMenu.destination != 0 && rblurDebugMenu.source != 0) {
            memcpy(rblurDebugMenu.destination, rblurDebugMenu.source, rblurDebugMenu.copyBytes);
        }
        rblurDone = 1;
    }
    if ((u8)rblurDone != 0) {
        effReturnEventSetupWord((u32)&rblurDebugMenu);
        effEventSetupBeforeRecordNoOp();
        effEventSetupRecordNoOp(&rblurDebugMenu);
        effEventSetupAfterRecordNoOp();
    }
    if (D_0040B7DB[0] < 0) {
        rblurDone = 0;
    }
    return rblurDone;
}

/* Expose the scale-blur setup parameters by address. */
dds3Effect_RippleBlurParam_t *rblurGetParam(void) {
    return &effStaggeredBlurSetupParameters;
}

/* Replace setup parameters without replacing the active staggered-blur work. */
void rblurSetParam(dds3Effect_RippleBlurParam_t *parameters) {
    effStaggeredBlurSetupParameters = *parameters;
}

SoundMixer *effEveBEDCreate(SoundMixer *source) {
    return sndMixerClone(source);
}

void effEveBEDDelete(SoundMixer *mixer) {
    sndMixerDestroy(mixer);
}

/* Compact event owner: the allocator reserves exactly 0x38 bytes. */
struct effEveBEDCutHandle_t {
    EffEventInit init;
    struct evtUnitHandle_t *actor; /* Linked event unit supplies its model owner. */
    effBTLCutHandle_t *effect;
};

struct BtlUnit;

extern void effBTLCutSetScale(effBTLCutHandle_t *effect, f32 scale);

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
effEveBEDCutHandle_t *effEveBEDCutCreate(SoundMixer *owner, u16 kind, const void *params) {
    const EffEventInit *init = (const EffEventInit *)params;
    effEveBEDCutHandle_t *work = sdfChipAllocDirect(EFF_EVENT_COMPACT_WORK_BYTES);

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = effCreateBattleVoiceOwner(owner, kind, 0, 0);
    effBTLCutSetScale(work->effect, init->param);
    return work;
}

extern s32 D_00436530;

extern billHandle_t *D_00436534;

extern billHandle_t *D_00436538;

extern u8 D_003B2D20[];

/* Release the attached effect before freeing the event work. */
void effEveBEDCutDelete(effEveBEDCutHandle_t *work) {
    effBTLCutDelete(work->effect);
    sdfChipFreeDirect(work);
}

/* Copy the serialized init prefix, retaining the event's actor and effect links. */
void effEveBEDCutSetParams(effEveBEDCutHandle_t *destination, const void *source) {
    memcpy(&destination->init, source, sizeof(destination->init));
}

/* Copy one serialized event record; either byte buffer may be unaligned. */
void effEveBEDCutGetParams(const void *source, void *destination) {
    memcpy(destination, source, EFF_EVENT_EVENT_RECORD_BYTES);
}

/* Forward the effect's scale value without modifying the record's init prefix. */
void effEveBEDCutSetScale(effEveBEDCutHandle_t *work, f32 scale) {
    effBTLCutSetScale(work->effect, scale);
}

void effEveBEDCutSetUnitHandle(effEveBEDCutHandle_t *work, struct evtUnitHandle_t *actor) {
    work->actor = actor;
}

extern void effEveBEDPartGetPositionPtr(EffEventInit *source, EffectVectorRequest *params);

extern const f32 D_00414A00[4];

/* Callbacks consume the VU0 output buffers; see docs/idioms.md,
 * "Event-effect SDK output frontiers". */
void effEventUpdateEffectParameters(effEveBEDCutHandle_t *work) {
    effBTLCutHandle_t *effect;
    EffBattleEntryList *list;
    EffBattleEntry *entry;
    sdfModelRootNode_t *model;
    f32 position[4];
    f32 rotationMatrix[4][4];
    f32 matrix[4][4];
    f32 scale;
    s32 entryIndex;
    u32 frame;
    u32 color;
    u16 count;

    effect = work->effect;
    frame = effect->currentFrame;
    list = effect->list;
    SDF_VU0_LOADVECTOR(work->init.orientation);
    SDF_VU0_LOADVECTOR_B(D_00414A00);
    Mult();
    ToMatrix();
    SDF_VU0_STOREMATRIX_NOMEM(rotationMatrix);
    count = list->count;
    entry = list->entries;
    color = work->init.color;
    for (entryIndex = 0; entryIndex < count; entryIndex++, entry++) {
        if (entry->zeroKindCount < frame && effect->parameterWorks[entryIndex] != NULL) {
            if (entry->aimParams.kind != 5 || work->actor == NULL) {
                effEveBEDPartGetPositionPtr(&work->init, &entry->aimParams);
                SDF_VU0_STOREVECTOR_NOMEM(position);
                dds3EffectPartSetPosition(effect->parameterWorks[entryIndex], position);
                dds3EffectPartSetRotateMatrix(effect->parameterWorks[entryIndex], rotationMatrix);
            } else {
                model = work->actor->owner->inner;
                sdfModelGetMarkPos(model, entry->aimParams.count);
                SDF_VU0_STOREVECTOR_NOMEM(position);
                dds3EffectPartSetPosition(effect->parameterWorks[entryIndex], position);
                /* The callback can change the entry's scale kind. */
                if (entry->aimParams.kind == 5) {
                    sdfModelGetMarkPosMatrix(model, entry->aimParams.count);
                    SDF_VU0_LOADMATRIX_B(rotationMatrix);
                    MultMatrixNoArg();
                    SDF_VU0_STOREMATRIX_NOMEM(matrix);
                    dds3EffectPartSetRotateMatrix(effect->parameterWorks[entryIndex], matrix);
                } else {
                    mdlGetScale(work->actor->owner);
                    SDF_VU0_GETVECTOR_X(scale);
                    sdfModelGetMarkPosMatrix(model, entry->aimParams.count);
                    if (scale != 0.0f) {
                        SDF_VU0_SETSCALE1VECTOR();
                        SDF_VU0_SCALEVECTOR_V0(1.0f / scale);
                        SDF_VU0_SCALE_XYZ();
                    }
                    SDF_VU0_STOREMATRIX_NOMEM(matrix);
                    dds3EffectPartSetRotateMatrix(effect->parameterWorks[entryIndex], matrix);
                }
            }
            dds3EffectPartSetColor(effect->parameterWorks[entryIndex], color);
            dds3EffectPartDraw(effect->parameterWorks[entryIndex]);
        }
    }
    if (effect->triggerFrame == frame && effect->triggerFrameCallback != NULL) {
        effect->triggerFrameCallback(effect);
    }
    effect->currentFrame++;
}

/* Initialize a holder and its effect record; the teardown flag starts set. */
effSimpleBED_t *effSimpleBEDCreate(SoundMixer *mixer, f32 param) {
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->init.orientation[3] = 1.0f;
    work->init.range = 50.0f;
    work->init.height = 180.0f;
    work->init.position[1] = -90.0f;
    work->init.param = param;
    work->init.color = EFF_EVENT_NEUTRAL_COLOR;
    work->init.orientation[0] = 0;
    work->init.orientation[1] = 0;
    work->init.orientation[2] = 0;
    work->init.position[0] = 0;
    work->init.position[2] = 0;
    work->init.position[3] = 0;
    work->handle = effEveBEDCreate(mixer);
    work->owner = effEveBEDCutCreate(work->handle, 0, &work->init);
    work->handleOwnership = EFF_SIMPLE_BED_HANDLE_OWNED;
    return work;
}

/* Release the effect, invoke shared teardown only when flagged, then free the holder. */
void effSimpleBEDDelete(effSimpleBED_t *work) {
    effEveBEDCutDelete(work->owner);
    if (work->handleOwnership != EFF_SIMPLE_BED_HANDLE_BORROWED) {
        effEveBEDDelete(work->handle);
    }
    sdfChipFreeDirect(work);
}

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *src) {
    EffEventInit *block = &src->init;
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->owner = effEveBEDCutCreate(src->handle, 0, block);
    work->handle = src->handle;
    memcpy(&work->init, block, sizeof(work->init));
    work->handleOwnership = EFF_SIMPLE_BED_HANDLE_BORROWED;
    return work;
}

/* Apply the native owner operation to the pointer stored in the record prefix. */
void effSimpleBEDDraw(effSimpleBED_t *work) {
    effEventUpdateEffectParameters(work->owner);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effSimpleBEDSetPosition(effSimpleBED_t *work, f32 *position) {
    work->init.position[0] = position[0];
    work->init.position[1] = position[1] - work->init.height * 0.5f;
    work->init.position[2] = position[2];
    work->init.position[3] = 0;
    effEveBEDCutSetParams(work->owner, &work->init);
}

/* Copy the holder's updated packed tint to its event record. */
void effSimpleBEDSetRGBA(effSimpleBED_t *work, u32 color) {
    work->init.color = color;
    effEveBEDCutSetParams(work->owner, &work->init);
}

extern f32 D_003B2CD0[];

extern f32 D_003B2CE8[];

extern f32 aViewEye[4];

extern f32 aViewDown[4];

/* vu0 routine: leave the selected aim point in vf10, not a C return value.
 * Preserve the non-camera output scratch w, which is not initialized here. */
void effEveBEDPartGetPositionPtr(EffEventInit *source, EffectVectorRequest *params) {
    f32 out[EFF_EVENT_VECTOR_COMPONENTS];
    f32 dir[EFF_EVENT_VECTOR_COMPONENTS];
    f32 pos[EFF_EVENT_VECTOR_COMPONENTS];
    f32 radius;
    f32 half;
    f32 y;
    s32 rangeOverride = params->extentOverride;
    u8 directionMode = params->directionMode;
    u8 aimMode = params->count;

    if (rangeOverride == 0) {
        radius = source->range;
    } else {
        radius = (f32)rangeOverride;
    }
    half = source->height * 0.5f;
    SDF_VU0_COPYVECTOR(pos, source->position);
    if (aimMode == 5) {
        if (directionMode == 8 || directionMode == 10) {
            y = -1.0f;
            if (rangeOverride != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = pos[1] - D_003B2CD0[aimMode] * half;
        if (directionMode == 8 || directionMode == 10) {
            if (rangeOverride != 0) {
                y -= radius;
            }
        }
    }
    if (directionMode == 9 || aimMode == 4) {
        dir[2] = half < radius ? -radius : -half;
        dir[0] = dir[1] = 0.0f;
        pos[1] = y;
        LookAtMatrix(pos, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(pos);
        SDF_VU0_ADDVECTOR();
        return;
    }
    if (directionMode == 8 || directionMode == 10) {
        out[0] = pos[0];
        out[1] = y;
        out[2] = pos[2];
    } else {
        dir[0] = 0.0f;
        dir[2] = 1.0f;
        dir[1] = 0.0f;
        SDF_VU0_LOADVECTOR(source->orientation);
        ToMatrix();
        SetRotateMatrixY_B(D_003B2CE8[directionMode]);
        MultMatrixNoArg();
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(dir);
        out[0] = pos[0] + radius * dir[0];
        out[1] = y + radius * dir[1];
        out[2] = pos[2] + radius * dir[2];
    }
    SDF_VU0_LOADVECTOR(out);
}

extern billHandle_t *billCreateFromResource(s32 kind, const char *path);

/* Allocate particles plus their owner, copy parameters and acquire shared textures. */

INCLUDE_RODATA(const s32, "effect/effEvent", D_00414A00);

EffEventBillSet *effEVECombParSmokeCreate(EffEventBillParams *src) {
    u32 particleCount = src->particleCount;
    u32 particleBytes = particleCount * sizeof(EffEventBillParticle);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(particleBytes + sizeof(EffEventBillSet));
    EffEventBillParticle *particle = (EffEventBillParticle *)sdfMemLock(allocationHandle);
    EffEventBillSet *work = (EffEventBillSet *)((u8 *)particle + particleBytes);
    u32 particleIndex;

    work->head = *src;
    work->allocationHandle = allocationHandle;
    work->particles = particle;
    work->flag = 0;
    if (work->head.fadeIn <= 0) {
        work->head.fadeIn = 1;
    }
    if (work->head.fadeOut <= 0) {
        work->head.fadeOut = 1;
    }
    if (work->head.startDelaySpread <= 0) {
        work->head.startDelaySpread = 1;
    }
    D_00436530 = D_00436530 + 1;
    if (D_00436530 == 1) {
        D_00436534 = billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_00436538 = billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    particleCount = work->head.particleCount;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        particle->age = 0;
        particle++;
    }
    return work;
}

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEVECombParSmokeDelete(EffEventBillSet *work) {
    D_00436530 = D_00436530 - 1;
    if (D_00436530 == 0) {
        dds3BillboardDelete(D_00436534);
        dds3BillboardDelete(D_00436538);
    }
    sdfMemFreeDirect(work->allocationHandle);
}

extern EffRandState effDefaultRandomState[];

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEVECombParSmokePartInit(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *particle = &work->particles[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = work->head.startDelaySpread;
    s32 motionDelaySpread = work->head.motionDelaySpread;

    particle->age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % startDelaySpread);
    particle->motionDelay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % motionDelaySpread);
    scaleFactor = dds3GetRandFloatA(effDefaultRandomState) * work->head.scaleRandomness + (1.0f - work->head.scaleRandomness);
    particle->baseScale = work->head.baseScale * scaleFactor;
    particle->scalePhaseX = dds3GetRandFloatA(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseY = dds3GetRandFloatA(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = work->head.scalePhaseStep * (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f);
    particle->scalePhaseStepY = work->head.scalePhaseStep * (dds3GetRandFloatA(effDefaultRandomState) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = work->head.scaleAmplitudeX * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = work->head.scaleAmplitudeY * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->rotation = dds3GetRandFloatA(effDefaultRandomState) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (dds3GetRandA((EffRandState *)effDefaultRandomState) & 1) {
        particle->angularSpeed = work->head.angularSpeed * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->angularSpeed = -(work->head.angularSpeed * (dds3GetRandFloatA(effDefaultRandomState) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = work->head.spawnRadius;
    direction[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->position[0] = spawnRadius * dds3GetRandFloatA(effDefaultRandomState) * direction[0];
    particle->position[1] = spawnRadius * dds3GetRandFloatA(effDefaultRandomState) * direction[1];
    particle->position[2] = spawnRadius * dds3GetRandFloatA(effDefaultRandomState) * direction[2];
    particle->motion[0] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (dds3GetRandFloatA(effDefaultRandomState) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(particle->motion);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->motion);
    particle->swayAmplitude = work->head.swayAmplitude * (dds3GetRandFloatA(effDefaultRandomState) * work->head.swayRandomness + (1.0f - work->head.swayRandomness));
    particle->swayPhase = 0;
    particle->verticalSpeed = work->head.verticalSpeed * (dds3GetRandFloatA(effDefaultRandomState) * work->head.verticalSpeedRandomness + (1.0f - work->head.verticalSpeedRandomness));
    particle->lateralSpeed = work->head.lateralSpeed * (dds3GetRandFloatA(effDefaultRandomState) * work->head.lateralSpeedRandomness + (1.0f - work->head.lateralSpeedRandomness));
}

extern f32 aViewTarget[4];

extern void dds3BillboardDraw(billHandle_t *obj);

/* vu0 routine: camera-right normalization and position addition use the
 * native SDK pipeline. As in effEVECombParSmokeDraw, position W is untouched. */
void effEventUpdateBillboardParticles(EffEventBillSet *work) {
    f32 position[4], origin[4], lateral[4];
    EffEventBillParticle *particle;
    s32 repeat;
    u32 index, particleCount;
    s32 duration, fadeIn, fadeOut;
    u32 color;
    f32 acceleration, swayPhaseStep, swayAmplitudeStep;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lateral);
    particle = work->particles;
    particleCount = work->head.particleCount;
    acceleration = work->head.accelerationPercent / 100.0f + 1.0f;
    repeat = work->head.repeat;
    duration = work->head.duration;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    color = work->head.color;
    swayPhaseStep = work->head.swayPhaseStep;
    swayAmplitudeStep = work->head.swayAmplitudeStep;
    SDF_VU0_COPYVECTOR(origin, work->head.position);
    dds3BillboardSetMode(D_00436534, work->head.drawMode);
    dds3BillboardSetMode(D_00436538, 3);
    for (index = 0; index < particleCount; index++, particle++) {
        s32 age = particle->age;
        if (age == 0) {
            effEVECombParSmokePartInit(work, index);
        }
        if (age > 0 && age <= duration) {
            f32 swaySine, sway, alpha;
            f32 modulationX, scaleX, scaleY, phaseCosine;
            u32 drawColor;
            if (particle->motionDelay >= 0) {
                f32 lateralSpeed = particle->lateralSpeed;
                particle->position[0] += lateral[0] * lateralSpeed;
                particle->position[1] += lateral[1] * lateralSpeed;
                particle->position[2] += lateral[2] * lateralSpeed;
                swaySine = sdfSin(particle->swayPhase);
                sway = particle->swayAmplitude * swaySine;
                particle->motion[1] += particle->verticalSpeed;
                position[0] = particle->position[0] + particle->motion[0] * sway;
                position[1] = particle->position[1] + particle->motion[1];
                position[2] = particle->position[2] + particle->motion[2] * sway;
                particle->verticalSpeed *= acceleration;
                particle->lateralSpeed *= acceleration;
                particle->swayAmplitude += swayAmplitudeStep;
                particle->swayPhase += swayPhaseStep;
            } else {
                particle->motionDelay++;
                position[0] = particle->position[0];
                position[1] = particle->position[1];
                position[2] = particle->position[2];
            }
            if (age < fadeIn) {
                alpha = (f32)age / (f32)fadeIn;
            } else if (duration - age <= fadeOut) {
                alpha = (f32)(duration - age) / (f32)fadeOut;
            } else {
                alpha = 1.0f;
            }
            drawColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            scaleY = particle->baseScale;
            phaseCosine = sdfCos(particle->scalePhaseX);
            modulationX = particle->scaleAmplitudeX * phaseCosine;
            phaseCosine = sdfCos(particle->scalePhaseY);
            scaleX = scaleY + modulationX;
            scaleY = scaleY + particle->scaleAmplitudeY * phaseCosine;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            effCopyVector(D_00436534->position, position);
            dds3BillboardSetScale(D_00436534, scaleX, scaleY);
            dds3BillboardSetRGBA(D_00436534, drawColor);
            dds3BillboardSetRotate(D_00436534, particle->rotation);
            dds3BillboardDraw(D_00436534);
            effCopyVector(D_00436538->position, position);
            dds3BillboardSetScale(D_00436538, scaleX, scaleY);
            dds3BillboardSetRGBA(D_00436538, drawColor);
            dds3BillboardSetRotate(D_00436538, particle->rotation);
            dds3BillboardDraw(D_00436538);
            particle->scalePhaseX += particle->scalePhaseStepX;
            particle->scalePhaseY += particle->scalePhaseStepY;
            particle->rotation += particle->angularSpeed;
        }
        if (duration == age && repeat) {
            particle->age = 0;
        } else {
            /* Native 001990A0..BC reloads after callbacks and wraps the word. */
            particle->age = (s32)((u32)particle->age + 1U);
        }
    }
}

struct DmaPacketHeader;

extern void sdf3DMakeDirectRefPacket(struct DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes);

extern void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix);

SYMBOL_AT(sdfGrOt_t, D_003802A8, sdfGrOtTable, 0x760);

extern sdfTexHandle_t *dds3BillboardGetTex(billHandle_t *effect);

/* DDS2 event-bill renderer, reconstructed from the native owner and its
 * current providers. The VIF position stream consumes XYZ only: native never
 * initializes the temporary position's W before its full-vector add/copy. */
void effEVECombParSmokeDraw(EffEventBillSet *work) {
    EffEventBillBatch primary, secondary;
    f32 matrix[4][4];
    f32 position[4], origin[4], lateral[4];
    sdfDrawTag_t *primaryList, *secondaryList;
    sdfTexHandle_t *primaryTexture, *secondaryTexture;
    EffEventBillParticle *particle;
    s32 repeat;
    u32 index, particleCount;
    s32 duration, fadeIn, fadeOut, colorStart, colorFrames;
    u32 color;
    f32 acceleration, swayPhaseStep, swayAmplitudeStep;
    struct DmaPacketHeader *texturePacket;
    const sdfTexPacket_t *textureBuffer;
    s32 textureBytes;
    u8 *geometryPacket;
    sdfDrawTag_t *stateList;
    sdfPrimTransPacket_t *statePacket;
    sdfGrOt_t *surface;
    primary.count = 0;
    secondary.count = 0;
    primaryList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(primaryList);
    secondaryList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(secondaryList);
    primaryTexture = dds3BillboardGetTex(D_00436534);
    texturePacket = (struct DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    textureBuffer = sdfTexGetPacket(primaryTexture);
    textureBytes = sdfTexGetPacketLength(primaryTexture);
    sdf3DMakeDirectRefPacket(texturePacket, (u32)textureBuffer, textureBytes);
    sdfDrawTagLinkRef(primaryList, (u32)texturePacket);
    secondaryTexture = dds3BillboardGetTex(D_00436538);
    texturePacket = (struct DmaPacketHeader *)sdfGrAllocPacketBuff(0x20);
    textureBuffer = sdfTexGetPacket(secondaryTexture);
    textureBytes = sdfTexGetPacketLength(secondaryTexture);
    sdf3DMakeDirectRefPacket(texturePacket, (u32)textureBuffer, textureBytes);
    sdfDrawTagLinkRef(secondaryList, (u32)texturePacket);
    SDF_VU0_STOREIDENTITY(matrix);
    geometryPacket = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(geometryPacket, matrix[0]);
    sdfDrawTagLink(primaryList, (u32)geometryPacket);
    geometryPacket = (u8 *)sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(geometryPacket, matrix[0]);
    sdfDrawTagLink(secondaryList, (u32)geometryPacket);
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lateral);
    particle = work->particles;
    particleCount = work->head.particleCount;
    acceleration = work->head.accelerationPercent / 100.0f + 1.0f;
    repeat = work->head.repeat;
    duration = work->head.duration;
    fadeIn = work->head.fadeIn;
    fadeOut = work->head.fadeOut;
    colorStart = work->head.colorStartAge;
    colorFrames = work->head.colorTransitionFrames;
    color = work->head.color;
    swayPhaseStep = work->head.swayPhaseStep;
    swayAmplitudeStep = work->head.swayAmplitudeStep;
    SDF_VU0_COPYVECTOR(origin, work->head.position);
    for (index = 0; index < particleCount; index++, particle++) {
        s32 age = particle->age;
        if (age == 0) {
            effEVECombParSmokePartInit(work, index);
        }
        if (age > 0 && age <= duration) {
            f32 swaySine, sway, alpha, transition;
            f32 modulationX, scaleX, scaleY, phaseCosine;
            f32 halfWidth, halfHeight;
            u32 primaryColor, secondaryColor;
            s32 slot;
            s16 u, v;
            if (particle->motionDelay >= 0) {
                f32 lateralSpeed = particle->lateralSpeed;
                particle->position[0] += lateral[0] * lateralSpeed;
                particle->position[1] += lateral[1] * lateralSpeed;
                particle->position[2] += lateral[2] * lateralSpeed;
                swaySine = sdfSin(particle->swayPhase);
                sway = particle->swayAmplitude * swaySine;
                particle->motion[1] += particle->verticalSpeed;
                position[0] = particle->position[0] + particle->motion[0] * sway;
                position[1] = particle->position[1] + particle->motion[1];
                position[2] = particle->position[2] + particle->motion[2] * sway;
                particle->verticalSpeed *= acceleration;
                particle->lateralSpeed *= acceleration;
                particle->swayAmplitude += swayAmplitudeStep;
                particle->swayPhase += swayPhaseStep;
            } else {
                particle->motionDelay++;
                position[0] = particle->position[0];
                position[1] = particle->position[1];
                position[2] = particle->position[2];
            }
            if (age < fadeIn) {
                alpha = (f32)age / (f32)fadeIn;
            } else if (duration - age <= fadeOut) {
                alpha = (f32)(duration - age) / (f32)fadeOut;
            } else {
                alpha = 1.0f;
            }
            if (age < colorStart) {
                primaryColor = effColorBlend(0, color & 0xFF000000, alpha);
            } else if (age < colorStart + colorFrames) {
                if (colorFrames > 0) {
                    transition = (f32)(age - colorStart) / (f32)colorFrames;
                    if (1.0f < transition) transition = 1.0f;
                } else {
                    transition = 1.0f;
                }
                primaryColor = effColorBlend(color & 0xFF000000, color, transition);
                primaryColor = effColorBlend(primaryColor & 0xFFFFFF, primaryColor, alpha);
            } else {
                primaryColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            }
            secondaryColor = effColorBlend(color & 0xFFFFFF, color, alpha);
            scaleY = particle->baseScale;
            phaseCosine = sdfCos(particle->scalePhaseX);
            modulationX = particle->scaleAmplitudeX * phaseCosine;
            phaseCosine = sdfCos(particle->scalePhaseY);
            scaleX = scaleY + modulationX;
            scaleY = scaleY + particle->scaleAmplitudeY * phaseCosine;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_LOADVECTOR_B(origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(position);
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = primary.count;
            SDF_VU0_COPYVECTOR(&primary.positions[slot], position);
            u = (s16)((u16)primaryTexture->width << 4);
            v = (s16)((u16)primaryTexture->height << 4);
            primary.colors[slot] = primaryColor;
            primary.uv[slot][0] = 0;
            primary.uv[slot][1] = 0;
            primary.uv[slot][2] = u;
            primary.uv[slot][3] = 0;
            primary.uv[slot][4] = u;
            primary.uv[slot][5] = v;
            primary.uv[slot][6] = 0;
            primary.uv[slot][7] = v;
            halfWidth = (f32)(u >> 5) * 0.0625f * scaleX;
            halfHeight = (f32)(v >> 5) * 0.0625f * scaleY;
            cosine = sdfCos(particle->rotation);
            sine = sdfSin(particle->rotation);
            cornerX = center - halfWidth;
            cornerY = center - halfHeight;
            primary.corners[slot][0] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][1] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = center - halfHeight;
            primary.corners[slot][2] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][3] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = halfHeight + center;
            primary.corners[slot][4] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][5] = cornerX * sine + cornerY * cosine;
            cornerX = center - halfWidth;
            cornerY = halfHeight + center;
            primary.corners[slot][6] = cornerX * cosine - cornerY * sine;
            primary.corners[slot][7] = cornerX * sine + cornerY * cosine;
            primary.count++;
            }
            {
                f32 cosine, sine, cornerX, cornerY;
                f32 center = 0.0f;
            slot = secondary.count;
            SDF_VU0_COPYVECTOR(&secondary.positions[slot], position);
            u = (s16)((u16)secondaryTexture->width << 4);
            v = (s16)((u16)secondaryTexture->height << 4);
            secondary.colors[slot] = secondaryColor;
            secondary.uv[slot][0] = 0;
            secondary.uv[slot][1] = 0;
            secondary.uv[slot][2] = u;
            secondary.uv[slot][3] = 0;
            secondary.uv[slot][4] = u;
            secondary.uv[slot][5] = v;
            secondary.uv[slot][6] = 0;
            secondary.uv[slot][7] = v;
            halfWidth = (f32)(u >> 5) * 0.0625f * scaleX;
            halfHeight = (f32)(v >> 5) * 0.0625f * scaleY;
            cosine = sdfCos(particle->rotation);
            sine = sdfSin(particle->rotation);
            cornerX = center - halfWidth;
            cornerY = center - halfHeight;
            secondary.corners[slot][0] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][1] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = center - halfHeight;
            secondary.corners[slot][2] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][3] = cornerX * sine + cornerY * cosine;
            cornerX = halfWidth + center;
            cornerY = halfHeight + center;
            secondary.corners[slot][4] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][5] = cornerX * sine + cornerY * cosine;
            cornerX = center - halfWidth;
            cornerY = halfHeight + center;
            secondary.corners[slot][6] = cornerX * cosine - cornerY * sine;
            secondary.corners[slot][7] = cornerX * sine + cornerY * cosine;
            secondary.count++;
            }
            if (primary.count == 15) {
                sdfDrawTagLink(primaryList, (u32)sdfBillBoardMakeN(primary.positions,
                    primary.colors, primary.uv, primary.corners[0], 15, 0));
                primary.count = 0;
            }
            if (secondary.count == 15) {
                sdfDrawTagLink(secondaryList, (u32)sdfBillBoardMakeN(secondary.positions,
                    secondary.colors, secondary.uv, secondary.corners[0], 15, 0));
                secondary.count = 0;
            }
            particle->scalePhaseX += particle->scalePhaseStepX;
            particle->scalePhaseY += particle->scalePhaseStepY;
            particle->rotation += particle->angularSpeed;
        }
        if (duration == age && repeat) particle->age = 0;
        else particle->age++;
    }
    if (primary.count > 0) {
        sdfDrawTagLink(primaryList, (u32)sdfBillBoardMakeN(primary.positions,
            primary.colors, primary.uv, primary.corners[0], primary.count, 0));
    }
    if (secondary.count > 0) {
        sdfDrawTagLink(secondaryList, (u32)sdfBillBoardMakeN(secondary.positions,
            secondary.colors, secondary.uv, secondary.corners[0], secondary.count, 0));
    }
    stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(stateList);
    statePacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    statePacket->header.dmaTag = 3;
    statePacket->header.vifCommands = 0x5000000310000000ULL;
    statePacket->header.gifTag = 0x1000000000008002ULL;
    statePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    if (work->flag) statePacket->registers.test.value = 0x51801;
    else statePacket->registers.test.value = 0x31801;
    statePacket->registers.test.registerId = SDF_GS_TEST_1;
    statePacket->registers.alpha.value = 0x44;
    statePacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(stateList, (u32)statePacket);
    D_003802A8.regFunc(&D_003802A8, stateList);
    stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(stateList);
    statePacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    statePacket->header.dmaTag = 3;
    statePacket->header.vifCommands = 0x5000000310000000ULL;
    statePacket->header.gifTag = 0x1000000000008002ULL;
    statePacket->header.gifRegisters = SDF_GIF_REGISTER_AD;
    if (work->flag) statePacket->registers.test.value = 0x51801;
    else statePacket->registers.test.value = 0x31801;
    statePacket->registers.test.registerId = SDF_GS_TEST_1;
    statePacket->registers.alpha.value = 0x42;
    statePacket->registers.alpha.registerId = SDF_GS_ALPHA_1;
    sdfDrawTagLink(stateList, (u32)statePacket);
    surface = &sdfGrOtTable[60];
    surface->regFunc(surface, stateList);
    (surface - 1)->regFunc(surface - 1, primaryList);
    surface->regFunc(surface, secondaryList);
}

/* Copy one full vector quadword through vf10, including its fourth component. */
void effEVECombParSmokeSetPosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

/* Set the native byte flag without normalizing it to a boolean. */
void effEVECombParSmokeSetZtest(EffEventBillSet *work, u8 flag) {
    work->flag = flag;
}

/* Copy the serialized emitter parameters without changing their values. */
void effEVECombParSmokeGetParams(const EffEventBillParams *source, EffEventBillParams *destination) {
    *destination = *source;
}

/* Copy parameters and clamp only start-delay spread and fade divisors;
 * motionDelaySpread remains unvalidated on this native path. */
void effEVECombParSmokeSetParams(EffEventBillParams *destination, const EffEventBillParams *source) {
    *destination = *source;
    if (destination->fadeIn <= 0) {
        destination->fadeIn = 1;
    }
    if (destination->fadeOut <= 0) {
        destination->fadeOut = 1;
    }
    if (destination->startDelaySpread <= 0) {
        destination->startDelaySpread = 1;
    }
}

/* Create from the installed default emitter block; the returned pointer is unused. */
void effEVECombParSmokeCreate2(void) {
    effEVECombParSmokeCreate(D_003B2D20);
}

EffChanWork *effAPPoint4Create(const EffChanHead *source, u16 kind, void *params) {
    const EffChanHead *head = source;
    u32 recordCount = head->count;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(recordCount * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = (EffChanWork *)sdfMemLock(allocation);
    EffChanRecord *record = (EffChanRecord *)(work + 1);
    effPartHandle_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = *head;
    work->buffer = allocation;
    work->records = record;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        delayModulus = work->head.spread;
        parameterTemplate = dds3EffectPartCreate(kind, params);
        record->param = parameterTemplate;
        record->delay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->param = dds3EffectPartCopy(parameterTemplate);
            record->delay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayModulus);
            record++;
        }
    }
    return work;
}

/* Forward block 0, block 1's word-2 kind (narrowed to u16), and block 1 unchanged. */
EffChanWork *effAPPoint4CreatePCF(void *data) {
    void *block0 = dds3EffectPCPDataGetData(data, 0);
    void *block1 = dds3EffectPCPDataGetData(data, 1);

    return effAPPoint4Create(block0, dds3EffectPCPDataGetType(data, 1), block1);
}

#define EFF_CURVE_ROW_FLOATS 4

#define EFF_CURVE_SCALAR_SOURCE_OFFSET 16

#define EFF_CURVE_SOURCE_ROW_COUNT 4

#define EFF_CHANNEL_ALPHA_SCALE 127.0f

#define EFF_CHANNEL_ALPHA_SHIFT 24

#define EFF_CHANNEL_RGB_COLOR 0x00808080

extern void effAPPoint4PartInit(EffChanWork *work, u32 recordIndex);

/* Clone channel work, allocate slots and duplicate a parameter template per record.
 * The record count is captured before allocation; a nonpositive delay modulus becomes one. */
EffChanWork *effAPPoint4Copy(EffChanSource *source) {
    u32 recordCount = source->head.count;
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordCount * sizeof(EffChanRecord) + sizeof(EffChanWork));
    EffChanWork *work = (EffChanWork *)sdfMemLock(allocationHandle);
    EffChanRecord *recordCursor = (EffChanRecord *)(work + 1);
    effPartHandle_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->head = source->head;
    work->buffer = allocationHandle;
    work->records = recordCursor;
    if (work->head.spread <= 0) {
        work->head.spread = 1;
    }
    work->slots = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        parameterTemplate = source->owner->param;
        delayModulus = work->head.spread;
        for (recordIndex = 0; recordIndex < recordCount; recordIndex++) {
            recordCursor->param = dds3EffectPartCopy(parameterTemplate);
            recordCursor->delay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayModulus);
            recordCursor++;
        }
    }
    return work;
}

/* Release slot storage, every duplicated parameter and the backing allocation in native order. */
void effAPPoint4Delete(EffChanWork *work) {
    EffChanRecord *recordCursor;
    u32 recordIndex = 0;
    u32 recordCount;

    dds3Bezier4PDelete(work->slots);
    recordCount = work->head.count;
    recordCursor = work->records;
    if (recordCount != 0) {
        do {
            dds3EffectPartDelete(recordCursor->param);
            recordCursor++;
            recordIndex++;
        } while (recordIndex < recordCount);
    }
    sdfMemFreeDirect(work->buffer);
}

/* vu0 routine: Jitter four control points normal to the path and viewing direction.
 * Reuse the final edge normal for the endpoint; retain XYZ-only scale initialization. */
void effAPPoint4PartInit(EffChanWork *work, u32 recordIndex) {
    f32 jitterScale[4];
    f32 lastNormal[4];
    f32 viewDirection[4];
    f32 jitteredPoint[4];
    EffChanRecord *record = work->records + recordIndex;
    EffCubicBezierSlot *bezierSlot;
    f32 cursorStep;
    f32 centeredRandom;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    cursorStep = 1.0f / (f32)work->head.steps;
    bezierSlot = dds3Bezier4PGetPart(work->slots, recordIndex);
    bezierSlot->t = 0;
    bezierSlot->step = cursorStep;

    centeredRandom = dds3GetRandFloatA(effDefaultRandomState) - 0.5f;
    jitterScale[0] = work->head.jitter[0] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[0]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[0][0] = jitteredPoint[0];
    bezierSlot->controlPoints[0][1] = jitteredPoint[1];
    bezierSlot->controlPoints[0][2] = jitteredPoint[2];

    centeredRandom = dds3GetRandFloatA(effDefaultRandomState) - 0.5f;
    jitterScale[0] = work->head.jitter[1] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[1]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[1][0] = jitteredPoint[0];
    bezierSlot->controlPoints[1][1] = jitteredPoint[1];
    bezierSlot->controlPoints[1][2] = jitteredPoint[2];

    centeredRandom = dds3GetRandFloatA(effDefaultRandomState) - 0.5f;
    jitterScale[0] = work->head.jitter[2] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[2]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[2][0] = jitteredPoint[0];
    bezierSlot->controlPoints[2][1] = jitteredPoint[1];
    bezierSlot->controlPoints[2][2] = jitteredPoint[2];

    centeredRandom = dds3GetRandFloatA(effDefaultRandomState) - 0.5f;
    jitterScale[0] = work->head.jitter[3] * (centeredRandom + centeredRandom);
    jitterScale[1] = jitterScale[0];
    jitterScale[2] = jitterScale[0];
    SDF_VU0_LOADVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(jitteredPoint);
    bezierSlot->controlPoints[3][0] = jitteredPoint[0];
    bezierSlot->controlPoints[3][1] = jitteredPoint[1];
    bezierSlot->controlPoints[3][2] = jitteredPoint[2];
    dds3EffectPartSetColor(record->param, 0);
}

/* Step active records, update their packed color/position callbacks, and optionally recycle.
 * The slot-zero lookup precedes the empty-count check; retain the time snapshot and later stored increments. */
void effAPPoint4Draw(EffChanWork *work) {
    EffChanRecord *recordCursor = work->records;
    struct EffCubicBezierArray *bezierSlots = work->slots;
    u32 recordCount = work->head.count;
    u32 recordIndex = 0;
    u8 cycleEnabled = work->head.enabled;
    s32 activeSteps = work->head.steps;
    s32 delayModulus = work->head.spread;
    s32 fadeInSteps = work->head.fadeIn;
    s32 fadeOutSteps = work->head.fadeOut;
    f32 position[3];

    (void)dds3Bezier4PGetPart(bezierSlots, 0);
    if (recordCount == 0) {
        return;
    }
    do {
        s32 recordTime = recordCursor->delay;

        if (recordTime == 0) {
            effAPPoint4PartInit(work, recordIndex);
        }
        if (recordTime > 0 && recordTime <= activeSteps) {
            f32 opacity;
            s32 fadePhase;
            u32 alpha;
            u32 color;

            dds3Bezier4PPartRun(bezierSlots, recordIndex, position);
            fadePhase = fadeInSteps > recordTime;
            if (fadePhase) {
                opacity = (f32)recordTime / (f32)fadeInSteps;
            } else {
                fadePhase = activeSteps - recordTime;
                if (fadePhase <= fadeOutSteps) {
                    opacity = (f32)fadePhase / (f32)fadeOutSteps;
                } else {
                    opacity = 1.0f;
                }
            }
            alpha = (u32)(opacity * EFF_CHANNEL_ALPHA_SCALE);
            color = (alpha << EFF_CHANNEL_ALPHA_SHIFT) | EFF_CHANNEL_RGB_COLOR;
            dds3EffectPartSetColor(recordCursor->param, color);
            dds3EffectPartSetPosition(recordCursor->param, position);
            dds3EffectPartDraw(recordCursor->param);
        }
        if (recordTime < activeSteps) {
            recordCursor->delay++;
        } else if (cycleEnabled != 0) {
            recordCursor->delay = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % delayModulus);
        } else {
            recordCursor->delay++;
        }
        recordCursor++;
        recordIndex++;
    } while (recordIndex < recordCount);
}

/* Copy XYZ from four float4 rows without touching destination W, then copy four trailing scalars. */
void effCopyVertRows(EffChan *channel, f32 *source) {
    f32 *sourceRow;
    u32 rowIndex;
    f32 *scalarCursor;
    f32 *destinationRow;

    sourceRow = source;
    rowIndex = 0;
    source += EFF_CURVE_SCALAR_SOURCE_OFFSET;
    scalarCursor = channel->rows->secondary;
    destinationRow = channel->rows->primary;
    do {
        rowIndex++;
        destinationRow[0] = sourceRow[0];
        destinationRow[1] = sourceRow[1];
        destinationRow[2] = sourceRow[2];
        sourceRow += EFF_CURVE_ROW_FLOATS;
        destinationRow += EFF_CURVE_ROW_FLOATS;
        *scalarCursor = *source++;
        scalarCursor++;
    } while (rowIndex < EFF_CURVE_SOURCE_ROW_COUNT);
}

/* Jitter each slot and choose its initial unsigned-modulo step.
 * Stored record time is one ahead of the Bezier cursor parameter's sampled step. */
void effEveAPP4Randomize(EffEmit *emitter) {
    EffChanWork *channelWork = emitter->primitive;
    u32 stepModulus = channelWork->head.steps;
    u32 recordCount = channelWork->head.count;
    EffChanRecord *recordCursor = channelWork->records;
    u32 recordIndex = 0;
    EffCubicBezierSlot *bezierSlot;
    s32 randomStep;
    f32 cursorStep;

    if (recordCount == 0) {
        return;
    }
    do {
        effAPPoint4PartInit(channelWork, recordIndex);
        recordCursor->delay = dds3GetRandA((EffRandState *)effDefaultRandomState) % stepModulus;
        bezierSlot = dds3Bezier4PGetPart(channelWork->slots, recordIndex);
        recordIndex++;
        randomStep = recordCursor->delay;
        cursorStep = bezierSlot->step;
        recordCursor->delay = randomStep + 1;
        recordCursor++;
        bezierSlot->t = cursorStep * (f32)randomStep;
    } while (recordIndex < recordCount);
}

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_BlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_PartialMultiBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_DistortionBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436463);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_FilterShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_PartialMultiFilterShow);

INCLUDE_SDATA(const s32, "effect/effEvent", Eve_RippleBlurShow);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436468);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436470);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436478);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436480);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436488);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436490);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436498);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436500);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436504);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436508);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436510);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436518);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043651C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436520);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436528);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436530);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436534);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436538);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043653C);

