#include "btl_motion_transform.h"
#include "sdf_packet_list.h"
#include "eff_bill.h"
#include "itf_draw_grid.h"
#include "eff_class_work_api.h"
#include "eff_point_set.h"
#include "common.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_dev_state.h"
#include "bill_object_api.h"
#include "sdf_chip.h"
#include "eff_ref_obj.h"
#include "sdf_resource.h"
#include "file_request_api.h"

#include "dds3_path.h"
#include "eff_transform.h"
#include "sdf_model.h"
#include "sdf_chunk.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "eff_curve.h"
#include "file.h"
#include "file_slot.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff_queue.h"
#include "btl_state.h"
#include "evt_unit.h"
#include "mdl.h"
#include "eff.h"
#include "eff_update_flags.h"
#include "eff_resource_list.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "eff_record_bucket.h"
#include "eff_owner_records.h"
#include "sdf.h"

extern void mdlAddEntryPlain(MdlCtx *, s32, s32);

extern void func_00200930(f32 *, f32 *, s32);
typedef struct FileQueue FileQueue;
extern FileQueue *fileCloneQueueEntries(FileQueue *);

typedef struct EffPacketParams {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 unk08;
    u32 *parameters;
    u128 *positions;
    u128 *normals;
    u32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} EffPacketParams;

typedef struct EffGsPacket {
    u64 dmaTag;
    u64 vifTag;
    u64 gifTag;
    u64 registerList;
    u64 registerValue;
    u64 registerAddress;
} EffGsPacket;


static inline void effSubmitSurfacePacket(SdfPoolNode *surface, void *list) {
    surface->append((SdfListHead *)surface, list);
}

extern s32 sdfAllocPacketAligned(s32);
extern void *func_00167A10(EffPacketParams *);
extern u32 D_003E9D80[];
extern SdfPoolNode *D_003E9DC0[];
extern SdfPoolNode *D_003E9F38[];
extern u32 D_003E9BE0[];
extern SdfPoolNode *D_003E9C28[];
extern SdfPoolNode kwlnDrawSurfaces[];

typedef struct EffectStateSnapshot {
    s128 vectors[8];
} EffectStateSnapshot;

extern void effResetFileResourceManager(void);

extern u16 effClassifyResourceMask(s32);

extern u32 D_004386BC;

extern u8 D_003FFDD8[];

extern s32 func_00300578(s32, s32, s32);

extern u8 D_003FFF58[];

extern u8 D_003FFE98[];

extern u32 func_0020E858(u32, u32 *);

extern void btlFormatResourceNameWithoutPrefix(u32, void *);

extern u32 btlCreateResourceNameRecord();

extern void btlFormatResourceNameWithPrefix(u32, void *);

extern void btlSetResourceNameHeaderPairAlternate(u32, u32, u32);

extern u32 func_0020E7B0(u32);

extern void func_0020E380(u32);

typedef struct EffectAssetLink {
    u8 *asset;
    u32 object;
    u8 pad_08[0x14];
} EffectAssetLink;

extern void camFollowOffsetVec(u8 *, void *);

extern u32 D_003FFA78[];

extern u32 D_003FF1C4[];

extern u32 D_003FF294[];

extern void fileQueueRemoveAndDestroyJob(s32, void *);

extern EffectAssetLink *D_003FF128[24];



typedef struct EffViewScale {
    u8 pad00[0x74];
    f32 scale;
} EffViewScale;



typedef struct EffSurfaceEndpoints {
    u8 pad00[0x140];
    EffectVectorRequest start;
    EffectVectorRequest end;
    s16 startIndex;
    s16 endIndex;
} EffSurfaceEndpoints;


typedef struct EffModelBindings {
    EffClassWork *material;
    MdlCtx *model;
} EffModelBindings;

typedef struct EffAimConfig {
    u8 pad00[0x88];
    EffectVectorRequest target;
    s16 targetIndex;
    u8 pad92[2];
    f32 modelParameter;
} EffAimConfig;

typedef struct EffAimState {
    u8 pad00[0x84];
    f32 length;
} EffAimState;

typedef struct EffDrawableAsset {
    u8 pad00[0x1C];
    f32 opacity;
} EffDrawableAsset;

typedef struct EffDrawableAssetWork {
    u32 unk00;
    s32 asset;
    u32 state;
} EffDrawableAssetWork;

typedef struct EffMotionResourceConfig {
    u8 pad00[0x4C];
    s16 mode;
} EffMotionResourceConfig;

typedef struct EffectBlock128 {
    u32 word[32];
} EffectBlock128;

typedef struct EffectSlot {
    u8 pad_0x00[0x28]; // 0x00
    s32 bucket;        // 0x28
    u8 pad_0x2C[0x54]; // 0x2C
} EffectSlot; // 0x80

typedef struct EffectSlotOwner {
    u8 pad_0x00[0x10];  // 0x00
    EffectSlot *slots;  // 0x10
} EffectSlotOwner;

extern void effBattleMiscQueryPosition(void *, EffectVectorRequest *, u128 *);

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOverrideSelector(void);

extern u32 effBTLFieldColorGetFinalSelector(void);

extern u32 effBTLFieldColorGetOriginalSelector(void);

extern void fileJobInvokeRotationCallback(void *, void *);

extern void fileJobInvokeScaleCallback(void *, f32);

extern u8 D_0045C270[];

extern void effComputeBattleCameraPositionVU(u8 *);

extern void fileDispatchJobTypeCallback(void *, u32);

extern void fileJobInvokePositionCallback(void *, void *);

extern s32 func_002D5AA8(void *);

typedef struct EffResourceBankSlot {
    u8 pad_00[0xC8];
    char name[0x34];    // 0xC8
    s32 type;           // 0xFC
    s32 state;          // 0x100
    s32 count;          // 0x104
} EffResourceBankSlot;

extern void effPollResourceBankSlot(char *, u32, EffResourceBankSlot *);

extern EffectBlock128 D_0045C1F0;

typedef struct EffModelResource {
    f32 position[4];
    f32 orientation[4]; /* Quaternion. */
    f32 scale;
    u32 color;
    s32 updateCount;
    s32 kind;
    MdlCtx *model;
    u32 attributes;
    void *childResource;
    void *source;
} EffModelResource;

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    MdlCtx *assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} EffModelCreateRequest;

extern float effMiscRandUnitFloat(void *);


extern s32 func_002FF0B8(EffectStateSnapshot *, s32);

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct EffKindDesc {
    u32 (*create)(void *);        // 0x00
    void (*destroy)();       // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(s32, s32); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} EffKindDesc; // 0x14

/* Native kind work: 0x30-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct EffKindWork {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;         // 0x10
    u32 color;        // 0x14
    f32 scale;        // 0x18
    s32 kind;         // 0x1C
    u32 frame;       // 0x20, incremented by the kind callback dispatcher
    u32 handle;       // 0x24
    void *payload;    // 0x28
    void *target;     // 0x2C
} EffKindWork; // 0x30

/* Source payload's kind selects the initial rendering mode. */
typedef struct EffKindSource {
    u8 pad_00[0x28];
    s32 modeKind;
} EffKindSource;


extern EffKindDesc D_003E9810[];

extern EffKindDesc D_003E98A0[];


extern s32 effFileQueue;

extern EffRecordBucket D_00400508[];

typedef struct BillCellDrawWork {
    u8 pad0[0x10];
    u8 transform[0x10];
    f32 scale;
    s32 baseColor;
    u32 frameLimit;
    u8 pad2C[4];
    u32 *instances;
    u8 *config;
} BillCellDrawWork;


extern void effMiscQuaternionToMatrixVU(void);

extern void func_002DB288(s32, void *);

extern void sdfVuMatrixToQuaternion(f32 matrix[4][4]);

extern u8 D_003E9140[];

extern u8 D_003E9110[];

extern void func_002DB2C0(s32, void *);

extern f32 effComputeProjectedOffsetAngle(void *, void *);

extern f32 func_0015A150(void *, void *);



extern void effCopyVector(u32, void *);

extern void billInvokeCallback(u32);

extern f32 D_0042BC10[4];

extern u8 D_003E9100[];

extern void func_002E5E88(u8 *, void *);

extern void effDrawFourPointGroups(u8 *, void *);

extern void func_002F1888(u8 *, void *);

extern void sdfMotionSampleAtFrame(Motion *, f32);

extern void kwlnPadStartMotor(s32, u8, s32);

extern s32 D_00437E98[2];

extern s32 D_00437EA0[2];

extern u8 D_00437EA8[2];

extern u8 D_00437EB0[2];

typedef struct EffectMapping {
    u8 pad_00[0x0C];
    u32 field0C;
    u32 field10;
    u8 *table;
    u32 count;
} EffectMapping;

extern EffectMapping effMappingState;

extern f32 D_0045C2F0[4];

extern u8 D_004386E0[];

extern u8 D_004386E8[];







extern void *sdfAllocSizeClassBlock(s32);

extern s32 effSharedRibbonReferenceCount;

extern u32 D_00437E78;

extern u32 effFlashTextureHandles;

extern u32 effModelUpdateControlFlags;

extern void mdlLoadPrimaryVectorVU(MdlCtx *);

extern void sdfBuildLightingPacket(void *, SdfLightSources, void *);

extern u128 D_004584B0[];

extern u128 D_00458470[];

extern SdfLightSources D_003E9F50;


extern void sdfTexReleaseReference(SdfTex *texture);


extern u8 btlIsRuntimeAllocated(void);

extern MdlCtx *func_002DC1D0(void *, u32);

extern void dds3DispatchIndexedCallback(s32, f32);



extern struct SdfPoolNode *D_00380828[4];

extern void mdlProcessContextNodesAndTransforms(MdlCtx *, struct SdfPoolNode **);

typedef struct EffectObjectFlag {
    u32 state;
    u8 pad04[8];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *effFloorModelListHead;

extern u32 D_00437E38;

extern u32 D_00437E3C;


extern struct EffExpandedList *func_002DDF48(u32);

extern u32 effWindTextureHandle;

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern u32 effSharedScalyStripResource;


extern s32 btlGetRuntime(void);

extern s32 effTemporaryFileJob;

extern s32 effAuxiliaryFileQueue;

extern u8 D_00439075;

extern s32 D_004386F0;

extern s32 effQueuedFileObject;

extern s32 effResourceBankEntries;

extern s32 effResourceBankDescriptor;

extern s32 effQueuedResourceNameRecord;

extern u32 D_0043876C;

extern u32 D_00438770;

extern u32 D_00438778;

extern u32 D_004387A8;

extern u32 D_004387AC;

extern u32 D_004387B0;

extern u32 D_00438774;

extern void mdlStoreTertiaryVectorVU(MdlCtx *);


extern u32 effSharedTextureReferenceCount;

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

/* Native resource operations add cloning before the frame callbacks.
 * The final word is the copied payload size, not another callback.
 * Factories retain this unit's existing pointer-return interface. */
typedef struct EffResourceOps {
    void (*initialize)();      /* 0x00 */
    void *(*createResource)(); /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    void *(*cloneResource)();  /* 0x0C */
    void (*update)();          /* 0x10 */
    void (*draw)();            /* 0x14 */
    u32 payloadSize;           /* 0x18 */
} EffResourceOps;


/* Native class operations: callbacks followed by the copied payload size.
 * Callback arity varies between creation and frame-notification paths. */
typedef struct EffClassOps {
    void (*initialize)();      /* 0x00 */
    u32 (*createResource)();    /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    void (*update)();          /* 0x0C */
    void (*draw)();            /* 0x10 */
    u32 payloadSize;           /* 0x14 */
} EffClassOps;


extern u8 *D_003E9CA8[];

extern u32 effCurrentRenderPacket;



extern u32 D_00437E6C;

extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);


extern u8 *effAllocateTexturedStripWork();

extern u32 D_00437E7C;


extern u32 D_004386B0;

extern u32 D_003FFA84[];

extern u8 D_0045C110[];

extern u8 D_00438758;

extern u8 D_00438759;

extern u8 D_0043875A;

extern u32 effQueuedFileHandle;

extern EffResourceOps effActiveInstanceOperations[];


extern EffClassOps effClassWorkOperations[];


extern EffResourceOps effBlockResourceOperations[];


extern EffResourceOps effModelBlockOperations[];

extern EffResourceOps effRuntimeResourceOperations[];


extern EffClassOps effModelResourceOperations[];

extern u8 D_0045C1A0[];

extern u8 D_003FFA40[];

extern u8 D_0045C1E0[];

extern u8 *D_004386CC;

extern u8 D_0045C300[];

extern u8 D_00400150[];

extern u8 D_00400250[];

extern EffClassOps effClassResourceWorkOperations[];

extern MdlCtx *func_00232198(s32 group, s32 id);

extern u16 mdlGetContextResourceGroup(MdlCtx *);

extern u16 mdlGetContextResourceId(MdlCtx *);



/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */
struct SdfMemBlock;

extern u8 *effCreatePointSet4(u32);




/* Surface parameters copied into the node: 0x1C bytes at +0x10. */
typedef struct EffSurfaceParams {
    u32 word[7];
} EffSurfaceParams;

/* Slot count of an effect record, capped at max. */
static inline u32 effSlotCount(u8 *p, u32 max) {
    u32 a = (u32)((FileKeyBlock *)p)->emissionDuration;
    u32 n = (a ? a : ((FileKeyBlock *)p)->spawnRate) * (a ? ((FileKeyBlock *)p)->spawnRate : (u32)((FileKeyBlock *)p)->length);
    return n < max + 1 ? n : max;
}

extern void sndLoadAndPlayStationedSe(u32);

extern EffTrackSet *effCreateTrackSetWithSharedReferences(u32, u16, u32);
extern void effReleaseResourceRefs(EffTrackSet *);
extern EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *);



typedef struct EffectNodeHeader {
    u8 *entries;
    u32 unk_04;
    u8 *allocation;
} EffectNodeHeader;

extern u32 effCreateRibbonWithSharedResource(u32, u32, u32);

extern u8 *effCreateRibbonWork(u32, u32);



extern u128 *D_0037F770[];

extern u128 D_00458460[];

extern u128 D_004584A0[];

extern u128 kwlnDefaultColorVector[];

extern s8 D_00437E94;

extern EffClassWork *effAllocateActiveInstanceWork(u16, void *);


extern EffClassWork *effAllocateBlockWithModel(u16, void *);

extern struct EffModelResource *effCreateModelResourceWithInlineData(u16, void *, void *, u32);

extern void fileQueueDestroy(s32);

extern void *fileQueueClone(void *);



MdlCtx *effCloneModelWithVUState(MdlCtx *sourceModel);

void effDestroyModelContext(MdlCtx *owner);

void effInitModelVUState(MdlCtx *model);

RefObj *effRetainSharedReference(RefObj *obj);

typedef struct EffFadeHeaderWork {
    s32 frame;
    s32 payload[2];
} EffFadeHeaderWork;

typedef char EffFadeHeaderWorkSizeCheck[(sizeof(EffFadeHeaderWork) == 0x0C) ? 1 : -1];

EffFadeHeaderWork *effDuplicateSmallHeader(const void *source) {
    EffFadeHeaderWork *buffer = sdfAllocSizeClassBlock(sizeof(EffFadeHeaderWork));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    return buffer;
}

EffFadeHeaderWork *effCreateSmallHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return effDuplicateSmallHeader(resource);
}

void effReleaseFadeHeaderAllocation(EffFadeHeaderWork *allocation) {
    kwlnCancelConfiguredFadeFrames();
    sdfReleaseChipBlock(allocation);
}

EffFadeHeaderWork *effCloneSmallHeaderFromWork(EffFadeHeaderWork *work) {
    return effDuplicateSmallHeader(work->payload);
}

void effFadeFrameReset(EffFadeHeaderWork *counter) {
    counter->frame = 0;
}

void effFadeFrameAdvance(EffFadeHeaderWork *counter) {
    s32 frame;

    frame = counter->frame;
    if (frame == 0) {
        kwlnFadeSetupFrames(counter->payload[0], counter->payload[1]);
        frame = counter->frame;
    }
    counter->frame = frame + 1;
}

EffFadeVectorWork *effCreateFadeVectorWork(source)
const EffLensFlareParams *source;
{
    EffFadeVectorWork *effect = sdfAllocSizeClassBlock(sizeof(EffFadeVectorWork));
    memset(effect, 0, sizeof(EffFadeVectorWork));
    VU0_STORE_VF(vf0, effect->vector);
    memcpy(&effect->source, source, sizeof(effect->source));
    return effect;
}

EffFadeVectorWork *effCreateFadeVectorFromFile(void *work) {
    const EffLensFlareParams *resource;

    resource = fileResolvePrimaryBuffer(work);
    return effCreateFadeVectorWork(resource);
}

void effFreeFadeVectorWork(EffFadeVectorWork *work) {
    sdfReleaseChipBlock(work);
}

void effCloneFadeVectorWork(EffFadeVectorWork *work) {
    effCreateFadeVectorWork(&work->source);
}

void effResetFadeVectorFrame(EffFadeVectorWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002DE460);

void effCopyFadeWorkVector(f32 *dst, const f32 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetFadeVectorColor(EffFadeVectorWork *work, u32 value) {
    work->color = value;
}


void effResetSelectionEntryBuffers(SdfFlagListWork *work) {
    u32 count;
    SdfFlagListMark *entries;
    u32 index;

    index = 0;
    count = work->params.count;
    entries = work->marks;
    if (count != 0) {
        do {
            index = index + 1;
            entries->timer = -1;
            entries++;
        } while (index < count);
    }
    memset(work->colors, 0, count << 3);
}

SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *work) {
    return func_00316528(work);
}

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *file) {
    return sdfInitializeFlagListFromResource(file);
}

void effReleaseSelectionFlagList(SdfFlagListWork *work) {
    sdfReleaseFlagListResource(work);
}

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *p) {
    return effCreateSelectionFlagListFromWork(&p->params);
}

void effResetSelectionEntriesAndState(SdfFlagListWork *p) {
    effResetSelectionEntryBuffers(p);
    p->frame = 0;
}

void func_002DEC08(SdfFlagListWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        func_00316680(work);
        return;
    }
}

void effDrawSelectionEntryVectors(SdfFlagListWork *work) {
    func_00316C88(work);
}

void effUpdateAndDrawSelectionEntries(SdfFlagListWork *p) {
    func_002DEC08(p);
    effDrawSelectionEntryVectors(p);
}

void func_002DEC78(SdfFlagListWork *work, u32 value) {
    work->unk04 = value;
}

typedef struct EffSelectionHeaderWork {
    s32 frame;
    s32 payload[4];
} EffSelectionHeaderWork;

typedef char EffSelectionHeaderWorkSizeCheck[(sizeof(EffSelectionHeaderWork) == 0x14) ? 1 : -1];

EffSelectionHeaderWork *effDuplicatePayloadHeader(const void *source) {
    EffSelectionHeaderWork *buffer = sdfAllocSizeClassBlock(sizeof(EffSelectionHeaderWork));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    return buffer;
}

EffSelectionHeaderWork *effCreateSelectionHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return effDuplicatePayloadHeader(resource);
}

void effReleaseSelectionHeaderAllocation(EffSelectionHeaderWork *allocation) {
    evtDestroySelectionState();
    sdfReleaseChipBlock(allocation);
}

EffSelectionHeaderWork *effCloneSelectionHeaderFromWork(EffSelectionHeaderWork *work) {
    return effDuplicatePayloadHeader(work->payload);
}

void effSelectionFrameReset(EffSelectionHeaderWork *counter) {
    counter->frame = 0;
}

void effSelectionFrameAdvance(EffSelectionHeaderWork *counter) {
    s32 frame;

    frame = counter->frame;
    if (frame == 0) {
        evtSelStateCreate(counter->payload[0], (s16)counter->payload[1], counter->payload[2], counter->payload[3]);
        frame = counter->frame;
    }
    counter->frame = frame + 1;
}

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);



extern void effBlurStepScaleSlotsAndDraw(void *);


extern void func_0018F840(void *);

typedef struct EffFadeConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 padBC[4];
    EffSolidRectParams out; /* 0xC0 */
} EffFadeConfig;

typedef struct EffRateConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    EffBlurQuad out;       /* 0xC0 */
} EffRateConfig;

/* Draw through the configured duration; zero duration uses frame zero.
 * Blend the packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    EffBlurQuad *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->x = 0;
    out->y = 0;
    out->left = 0;
    out->top = 0;
    out->right = 0x200;
    out->bottom = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->displacement = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->angle = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->blendControl = work->mode;
    effDrawBlurRectangle(out);
}

u32 effCreateFadeBlendWorkFromOutput(void *work) {
    return (u32)effCloneBlurTemplate((EffBlurTemplateBody *)((u8 *)work + 0xc0));
}

void effReleaseFadeBlendWork(u32 resourceHandle) {
    effReleaseBlurTemplate((EffBlurTemplate *)resourceHandle);
}

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffBlurTemplate *out = (EffBlurTemplate *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->body.source.x = 0;
        out->body.source.y = 0;
        out->body.extent = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->body.extent = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->body.source.x = px;
        out->body.source.y = py << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->body.source.color = blended[0];
    out->body.source.displacement = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->body.source.angle = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->body.source.blendControl = work->mode;
    effDrawBlurFixedPointRectangle(out);
}

void effSetFadeMapParameter(EffKindWork *work, u32 value) {
    ((EffBlurTemplate *)work->handle)->resourceWord = value;
}

u32 effCreateFixedSlotBlurWorkFromFadeOutput(void *source) {
    return (u32)effBlurCreateScatterWork((EffBlurScatterParams *)((u8 *)source + 0xC0));
}

void effReleaseFixedSlotBlurWork(EffBlurScatterWork *handle) {
    effBlurReleaseFirstResource(handle);
}

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffBlurScatterWork *out = (EffBlurScatterWork *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.positionSpread = (s32)rate;
        out->params.x = 0;
        out->params.y = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.positionSpread = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF(vf10, pos);
        out->params.x = (s32)pos[0] - 0x800;
        out->params.y = ((s32)pos[1] - 0x800) << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->params.color = blended[0];
    out->params.uvDisplacementAmplitude = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.uvDisplacementAngleDegrees = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blendControl = work->mode;
    effBlurStepScatterSlotsAndDraw(out);
}

void effSetWideFadeMapParameter(EffKindWork *work, u32 value) {
    ((EffBlurScatterWork *)work->handle)->sourceHandle = value;
}

u32 effCreateVariableSlotBlurWorkFromFadeOutput(void *source) {
    return (u32)effCloneBlurWorkWithSlots((EffBlurScaleParams *)((u8 *)source + 0xc0));
}

void effReleaseVariableSlotBlurWork(EffBlurScaleWork *work) {
    effBlurReleaseSecondResource(work);
}

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and duration gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffBlurScaleWork *out = (EffBlurScaleWork *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.size = (s32)rate;
        out->params.x = 0;
        out->params.y = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.size = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF(vf10, pos);
        out->params.x = (s32)pos[0] - 0x800;
        out->params.y = ((s32)pos[1] - 0x800) << 1;
    }
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->params.color = blended[0];
    out->params.angleStep = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.uvDisplacementAngleDegrees = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blendControl = work->mode;
    effBlurStepScaleSlotsAndDraw(out);
}

void effSetFadeBlendParameter(EffKindWork *work, u32 value) {
    ((EffBlurScaleWork *)work->handle)->sourceHandle = value;
}

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    EffBlurQuad *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->x = 0;
    out->y = 0;
    out->left = 0;
    out->top = 0;
    out->right = 0x200;
    out->bottom = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->displacement = effSampleScalarCurve(&config->blendB, frame, duration) + 1.0f;
    out->angle = effSampleScalarCurve(&config->rateA.curve, frame, duration);
    out->blendControl = work->mode;
    effBlurDrawFramebufferQuad(out);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002DF6C8);

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(EffKindWork *work) {
    EffFadeConfig *config = work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    EffSolidRectParams *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->left = 0;
    out->top = 0;
    out->right = 0x200;
    out->bottom = 0x1C0;
    second = effSampleColorAlphaTracks(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->blendControl = work->mode;
    func_0018F840(out);
}

extern EffResourceRectWork *effCloneResourceTemplate(EffResourceRectParams *params);
extern void effReleaseResourceTemplate(EffResourceRectWork *work);

u32 effCreateFadeColorWorkFromOutput(void *work) {
    return (u32)effCloneResourceTemplate((EffResourceRectParams *)((u8 *)work + 0xc0));
}

void effReleaseFadeColorWork(EffResourceRectWork *work) {
    effReleaseResourceTemplate(work);
}

/* This projected fade also consumes EffKindWork: position, handle and payload. */
INCLUDE_ASM(const s32, "game/code_002DE248", func_002DFAB0);

void func_002DFC78(EffKindWork *work, u32 value) {
    ((EffResourceRectWork *)work->handle)->sourceHandle = value;
}

EffKindWork *effAllocateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x30;
    u32 size = D_003E9810[kind].payloadSize;
    EffKindWork *work = sdfAllocAndClearQuadwords(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF(vf0, work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((EffKindSource *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_003E9810[kind].create != NULL) {
        work->handle = D_003E9810[kind].create(source);
    }
    return work;
}

extern s32 *effCreateResourceHolderFromSelectedKind(s32 *, u16);

EffKindWork *func_002DFDB8(FileJob *work) {
    void *source = fileResolvePrimaryBuffer((FileJobPayload *)work);
    EffKindWork *effect = effAllocateKindWork(work->option, source);
    s32 *secondary = fileResolveSecondaryBuffer((FileJobPayload *)work);

    if (secondary != NULL) {
        if (D_003E9810[effect->kind].initialize != NULL) {
            s32 *child = effCreateResourceHolderFromSelectedKind(secondary, work->slots[0].selector);
            effect->target = child;
            D_003E9810[effect->kind].initialize((s32)effect, child[2]);
        }
    }
    return effect;
}

extern void effKindAssetReferenceRelease(s32 *);

void effReleaseKindWork(EffKindWork *work) {
    s32 object = work->handle;
    if (object != 0) {
        D_003E9810[work->kind].destroy(object);
    }
    if (work->target != NULL) {
        effKindAssetReferenceRelease(work->target);
    }
    sdfReleaseChipBlock(work);
}

s32 effCloneKindWork(EffKindWork *source) {
    EffKindWork *copy = effAllocateKindWork((u16)source->kind, source->payload);
    if (source->target != NULL && D_003E9810[copy->kind].initialize != NULL) {
        s32 *child = (s32 *)effRetainKindSecondaryAsset(source->target);
        s32 parameter = child[2];
        copy->target = child;
        D_003E9810[copy->kind].initialize((s32)copy, parameter);
    }
    return (s32)copy;
}

void effKindWorkFrameReset(EffKindWork *work) {
    work->frame = 0;
}

void effKindWorkFrameUpdate(EffKindWork *work) {
    D_003E9810[work->kind].update();
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        work->frame++;
    }
}

void effCopyKindWorkPosition(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetKindWorkColor(EffKindWork *work, u32 value) {
    work->color = value;
}

void effSetKindWorkMatrixComponent(EffKindWork *work, float value) {
    work->scale = value;
}

/* Allocate work for the alternate kind table, copying its payload and creating its handle. */
EffKindWork *effAllocateAlternateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x30;
    u32 size = D_003E98A0[kind].payloadSize;
    EffKindWork *work = sdfAllocAndClearQuadwords(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF(vf0, work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((EffKindSource *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_003E98A0[kind].create != NULL) {
        work->handle = D_003E98A0[kind].create(source);
    }
    return work;
}

/* Create alternate-kind work from a primary file payload and a supported secondary asset. */
EffKindWork *effCreateAlternateKindWorkFromFile(FileJob *work) {
    void *source = fileResolvePrimaryBuffer((FileJobPayload *)work);
    EffKindWork *effect = effAllocateAlternateKindWork(work->option, source);
    s32 *secondary = fileResolveSecondaryBuffer((FileJobPayload *)work);

    if (secondary != NULL) {
        if (D_003E98A0[effect->kind].initialize != NULL) {
            s32 *child = effCreateResourceHolderFromSelectedKind(secondary, work->slots[0].selector);
            effect->target = child;
            D_003E98A0[effect->kind].initialize((s32)effect, child[2]);
        }
    }
    return effect;
}


/* Release the alternate-kind handle, retained secondary asset, and work allocation. */
void effReleaseAlternateKindWork(EffKindWork *work) {
    s32 object = work->handle;
    if (object != 0) {
        D_003E98A0[work->kind].destroy(object);
    }
    if (work->target != NULL) {
        effKindAssetReferenceRelease(work->target);
    }
    sdfReleaseChipBlock(work);
}

/* Recreate alternate-kind work, retaining its secondary asset when initialization is supported. */
s32 effCloneAlternateKindWork(EffKindWork *source) {
    EffKindWork *copy = effAllocateAlternateKindWork((u16)source->kind, source->payload);
    if (source->target != NULL && D_003E98A0[copy->kind].initialize != NULL) {
        s32 *child = (s32 *)effRetainKindSecondaryAsset(source->target);
        s32 parameter = child[2];
        copy->target = child;
        D_003E98A0[copy->kind].initialize((s32)copy, parameter);
    }
    return (s32)copy;
}

void effAlternateKindWorkFrameReset(EffKindWork *work) {
    work->frame = 0;
}


void effAlternateKindWorkFrameUpdate(EffKindWork *work) {
    D_003E98A0[work->kind].update();
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        work->frame++;
    }
}

/* Copy the leading vector of alternate kind-table work. */
void effCopyAlternateKindVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Set the alternate kind-table work's packed color. */
void effSetAlternateKindColor(EffKindWork *work, u32 color) {
    work->color = color;
}

/* Set the alternate kind-table work's scalar scale. */
void effSetAlternateKindScale(EffKindWork *object, f32 scale) {
    object->scale = scale;
}

extern SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress);


typedef struct EffKindAssetHolder {
    s32 kind;
    s32 references;
    s32 resource;
} EffKindAssetHolder;

s32 *effCreateResourceHolderFromSelectedKind(s32 *source, u16 kind) {
    s32 *object = sdfAllocAndClearQuadwords(0xC);
    object[0] = kind;
    object[1] = 1;
    switch (kind) {
    case 1:
        object[2] = (s32)sdfTexAcquireResourceTexture(source);
        break;
    case 4:
        object[2] = (s32)effGetBillResourceTexture(*source);
        break;
    }
    return object;
}

extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);

void effKindAssetReferenceRelease(s32 *object) {
    object[1]--;
    if (object[1] == 0) {
        if (object[0] != 4) {
            sdfTexReleaseReferenceViaHandler((SdfTex *)object[2]);
        }
        sdfReleaseChipBlock(object);
    }
}

u32 effRetainKindSecondaryAsset(u32 work) {
    ((EffKindAssetHolder *)work)->references = ((EffKindAssetHolder *)work)->references + 1;
    return work;
}

/* Native 0x68-byte billboard work, shared by construction, cloning and beam update.
 * The copied 0x38-byte configuration begins with height/width scales at 0x2C.
 * Keep natural alignment: both leading vectors are transferred as VU qwords. */
typedef struct EffBillboardWork {
    f32 position[4]; /* 0x00 */
    f32 rotation[4]; /* 0x10: quaternion */
    f32 scale;      /* 0x20 */
    u32 color;      /* 0x24 */
    s32 frame;      /* 0x28 */
    f32 heightScale; /* 0x2C */
    f32 widthScale;  /* 0x30 */
    u8 unk34[0x30];  /* Remaining copied configuration, not alignment padding. */
    BillObj *billboard; /* 0x64: owned billboard handle. */
} EffBillboardWork;

u8 *effCreateBillboardWork(u8 *source) {
    u8 *work = (u8 *)sdfAllocSizeClassBlock(0x68);
    memset(work, 0, 0x68);
    ((EffBillboardWork *)work)->frame = 0;
    ((EffBillboardWork *)work)->color = 0x80808080;
    ((EffBillboardWork *)work)->scale = 1.0f;
    VU0_STORE_VF(vf0, work);
    VU0_STORE_VF(vf0, work + 0x10);
    if (source == NULL) {
        return work;
    }
    memcpy(work + 0x2C, fileResolvePrimaryBuffer((FileJobPayload *)source),
           ((FileJob *)source)->slots[0].size);
    ((EffBillboardWork *)work)->billboard =
        billCreateIndexed(1, (u32)fileResolveSecondaryBuffer((FileJobPayload *)source));
    return work;
}

void effBillboardWorkRelease(u32 work) {
    BillObj *billboard;

    billboard = ((EffBillboardWork *)work)->billboard;
    if (billboard != 0) {
        billDispatchByKind(billboard);
    }
    sdfReleaseChipBlock((void *)work);
}

u8 *effDuplicateBillState(const u8 *source) {
    u8 *effect = (u8 *)effCreateBillboardWork(NULL);
    memcpy(effect + 0x2C, source + 0x2C, 0x38);
    effReplaceBillboardClone((s32)effect, (s32)source);
    return effect;
}

void effReplaceBillboardClone(s32 dst, s32 src) {
    BillObj *billboard;

    if (((EffBillboardWork *)dst)->billboard != 0) {
        billDispatchByKind(((EffBillboardWork *)dst)->billboard);
    }
    billboard = billCloneObjectRetainingSharedData(((EffBillboardWork *)src)->billboard);
    ((EffBillboardWork *)dst)->billboard = billboard;
}

void effBillboardEntryFrameReset(s32 work) {
    billSetAnimationFrameWithOneTickHold(((EffBillboardWork *)work)->billboard, 0);
    ((EffBillboardWork *)work)->frame = 0;
}


/* Update a live billboard frame's projected scale, rotation, and position before its callback. */
void effUpdateScaledBillboardFrame(EffBillboardWork *work) {
    u128 dir;
    f32 angle;
    f32 len;

    if (work->frame < billGetFirstEntryFramePeriod(work->billboard)) {
        billSetAnimationFrameWithOneTickHold(work->billboard, work->frame);
        VU0_LOAD_VF(vf10, work->rotation);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0042BC10);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, &dir);
        angle = effComputeProjectedOffsetAngle(work, &dir);
        len = func_0015A150(work, &dir);
        if (len < 0.3f) {
            len = 0.3f;
        }
        len *= work->scale;
        billSetChildScaleComponents(work->billboard, len * work->widthScale, work->heightScale * work->scale);
        billSetRotationAngle(work->billboard, angle);
        effCopyVector((u32)work->billboard, work);
        billInvokeCallback((u32)work->billboard);
        work->frame++;
    }
}

void effCopyBillboardPosition(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyBillboardOrientation(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetBillboardColor(s32 work, u32 value) {
    ((EffBillboardWork *)work)->color = value;
}

/* Set the billboard's scalar scale; the leading vectors are not a Matrix4. */
void effSetBillboardMatrixComponent(EffBillboardWork *work, float value) {
    work->scale = value;
}

/* DDS1/DDS2 frame asset: storage at +0x1C, count at +0x08. */
typedef struct EffFrameAsset {
    u8 pad_00[8];
    u32 frameCount;
    u8 pad_0C[0xC];
    u32 *animationFrames; // 0x18, per-frame reset flags
    void *frameStorage;
    f32 *transformRows;
    u32 pad_24;
    u32 *colorRows;
} EffFrameAsset;

/* Per-instance frame entries and the descriptor that owns their storage. */
typedef struct EffFrameState {
    u8 *entries;       // 0x00
    u8 *asset;         // 0x04
    struct SdfMemBlock *allocation; // 0x08: allocation descriptor
} EffFrameState;



void effClearBillFrames(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

u8 *effCreateBillFrameNode(EffBillFrameConfig *config, u32 handle) {
    u32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    ((EffBillFrameState *)node)->entries = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(count, 0, handle);
    return node;
}

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E0900);

typedef struct EffBillOutput {
    u32 textureId;      // 0x00
    u32 color;          // 0x04
    u32 field_08;       // 0x08
    u8 outputMode;      // 0x0C
    u8 pad_0D[7];
    u8 mode;            // 0x14
} EffBillOutput;

void billUpdateFrameDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void billResetCellIndices(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x1C;
        } while (remaining);
    }
}

u8 *billCreateCellNode(EffBillCellConfig *config, u32 handle) {
    u32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(count * 0x1C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(count, 1, handle);
    return node;
}

void billReleaseCellNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E11D0);

void billUpdateCellDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void billResetParticleIndices(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x2C;
        } while (remaining);
    }
}

u8 *billCreateParticleNode(EffBillParticleConfig *config, u32 handle) {
    u32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(count, 1, handle);
    return node;
}

void billReleaseParticleNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E1BB0);

void billUpdateParticleDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void effClearAnimatedFrames(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

u8 *billAllocateAnimatedTransformEntries(EffBillAnimatedFrameConfig *config) {
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(config->frame.output.timed.count * 0x18 + headerSize);
    u8 *node = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *entries = node + headerSize;

    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = entries;
    if (config->drawProgress == 0) {
        config->drawProgress = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillTimedHeader *)descriptor)->count;
    if (count != 0) {
        f32 *row = (f32 *)((EffBillFrameState *)work)->asset->columns;
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *billAllocateAnimatedTransformEntries();

u8 *billCreateAnimatedTransform(EffBillAnimatedFrameConfig *descriptor, s32 handle) {
    u8 *work = billAllocateAnimatedTransformEntries(descriptor);

    ((EffBillFrameState *)work)->asset = effCreateTrackSetWithSharedReferences(descriptor->frame.output.timed.count, 3, handle);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

u8 *billCloneAnimatedTransform(u8 *owner) {
    u8 *descriptor = ((EffClassWork *)owner)->payload;
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    u8 *work = billAllocateAnimatedTransformEntries(descriptor);

    ((EffBillFrameState *)work)->asset = effDuplicateResourceRefs(((EffBillFrameState *)source)->asset);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

void billReleaseAlternatingTransformNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E26A0);

void billUpdateAlternatingDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void billResetEmitterIndices(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

u8 *billAllocEmitterNode(u8 *config) {
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(((EffBillTimedHeader *)config)->count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeEmitterRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillTimedHeader *)descriptor)->count;
    if (count != 0) {
        f32 *row = (f32 *)((EffBillFrameState *)work)->asset->columns;
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *billAllocEmitterNode();

u8 *billCreateEmitterTransform(EffBillEmitterFrameConfig *descriptor, s32 handle) {
    u8 *work = billAllocEmitterNode(descriptor);

    ((EffBillFrameState *)work)->asset = effCreateTrackSetWithSharedReferences(descriptor->frame.output.timed.count, 4, handle);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

u8 *billCloneEmitterTransform(u8 *owner) {
    u8 *descriptor = ((EffClassWork *)owner)->payload;
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    u8 *work = billAllocEmitterNode(descriptor);

    ((EffBillFrameState *)work)->asset = effDuplicateResourceRefs(((EffBillFrameState *)source)->asset);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

void billReleaseEmitterNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E3008);

void billUpdateEmitterDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = ((BillCellDrawWork *)work)->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void effClearStripFrames(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x28;
        } while (remaining);
    }
}

u8 *billAllocStripNode(u8 *config) {
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(((EffBillTimedHeader *)config)->count * 0x28 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeStripRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillTimedHeader *)descriptor)->count;
    if (count != 0) {
        f32 *row = (f32 *)((EffBillFrameState *)work)->asset->columns;
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *billAllocStripNode();

u8 *billCreateStripTransform(EffBillStripFrameConfig *descriptor, s32 handle) {
    u8 *work = billAllocStripNode(descriptor);

    ((EffBillFrameState *)work)->asset = effCreateTrackSetWithSharedReferences(descriptor->frame.output.timed.count, 3, handle);
    billInitializeStripRows(work, descriptor);
    return work;
}

u8 *billCloneStripTransform(u8 *owner) {
    u8 *descriptor = ((EffClassWork *)owner)->payload;
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    u8 *work = billAllocStripNode(descriptor);

    ((EffBillFrameState *)work)->asset = effDuplicateResourceRefs(((EffBillFrameState *)source)->asset);
    billInitializeStripRows(work, descriptor);
    return work;
}

void billReleaseStripNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E39B0);

void billUpdateStripDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = ((BillCellDrawWork *)work)->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void billResetTrailIndices(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    u8 *entry = ((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x2C;
        } while (remaining);
    }
}

u8 *billCreateTrailNode(EffBillTrailFrameConfig *config, u32 handle) {
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(config->frame.output.timed.count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    ((EffBillFrameState *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 0, handle);
    return node;
}

void billReleaseTrailNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E4320);

void billUpdateTrailDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = ((BillCellDrawWork *)work)->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}

void billResetQuadIndices(u8 *owner) {
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    EffTrackSet *descriptor = ((EffBillFrameState *)source)->asset;
    EffBillQuadEntry *entry = (EffBillQuadEntry *)((EffBillFrameState *)source)->entries;
    s32 count = ((EffBillTimedHeader *)((EffClassWork *)owner)->payload)->count;

    memset(descriptor->buffer, 0, descriptor->rows * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            entry->timer = -1;
            entry++;
        } while (remaining);
    }
}

u8 *billAllocQuadNode(u8 *config) {
    u32 headerSize = 0x10;
    struct SdfMemBlock *base = sdfAllocGeneralBlock(((EffBillTimedHeader *)config)->count * sizeof(EffBillQuadEntry) + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((EffBillFrameState *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeQuadRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillTimedHeader *)descriptor)->count;
    if (count != 0) {
        f32 *row = (f32 *)((EffBillFrameState *)work)->asset->columns;
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *billAllocQuadNode();

u8 *billCreateQuadTransform(EffBillQuadFrameConfig *descriptor, s32 handle) {
    u8 *work = billAllocQuadNode(descriptor);

    ((EffBillFrameState *)work)->asset = effCreateTrackSetWithSharedReferences(descriptor->frame.output.timed.count, 4, handle);
    billInitializeQuadRows(work, descriptor);
    return work;
}

u8 *billCloneQuadTransform(u8 *owner) {
    u8 *descriptor = ((EffClassWork *)owner)->payload;
    u8 *source = (u8 *)((EffClassWork *)owner)->resource;
    u8 *work = billAllocQuadNode(descriptor);

    ((EffBillFrameState *)work)->asset = effDuplicateResourceRefs(((EffBillFrameState *)source)->asset);
    billInitializeQuadRows(work, descriptor);
    return work;
}

void billReleaseQuadNode(s32 work) {
    effReleaseResourceRefs(((EffBillFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffBillFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E4E80);

void billUpdateQuadDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = ((BillCellDrawWork *)work)->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = ((EffBillFrameHeader *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
}


/* Four-byte owner header followed by its class-work pointer array. */
typedef struct EffClassWorkList {
    EffClassWork **entries;
} EffClassWorkList;

EffClassWork *effAllocateActiveInstanceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effActiveInstanceOperations[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return (EffClassWork *)effect;
}

EffClassWork *effCreateResourceInstanceA(u16 kind, void *source, u32 extra) {
    EffClassWork *work = effAllocateActiveInstanceWork(kind, source);
    work->resource = (u32)effActiveInstanceOperations[kind].createResource(source, extra);
    effActiveInstanceOperations[kind].initialize(work);
    return work;
}

u8 *effCreateFileResourceInstance(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer((FileJobPayload *)work);
    void *source;
    switch (((FileJob *)work)->slots[0].selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return (u8 *)effCreateResourceInstanceA(((FileJob *)work)->option, source, (u32)secondary);
}


void effDestroyActiveInstanceWork(EffClassWork *obj) {

    effActiveInstanceOperations[obj->kind].destroyResource(obj->resource);
    sdfReleaseChipBlock(obj);
}

EffClassWork *effCreateActiveResource(EffClassWork *obj) {
    EffClassWork *work;

    if (effActiveInstanceOperations[obj->kind].cloneResource == NULL) {
        work = effCreateResourceInstanceA((u16)obj->kind, obj->payload, 0);
    } else {
        void *resource;
        s32 kind;

        work = effAllocateActiveInstanceWork((u16)obj->kind, obj->payload);
        resource = effActiveInstanceOperations[obj->kind].cloneResource(obj);
        kind = obj->kind;
        work->resource = (u32)resource;
        effActiveInstanceOperations[kind].initialize(work);
    }
    return work;
}

void effResetActiveInstanceFrame(EffClassWork *work) {
    effActiveInstanceOperations[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceActiveInstanceFrame(EffClassWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        effActiveInstanceOperations[work->kind].update();
        work->frame++;
    }
}

void effDispatchActiveInstanceDraw(EffClassWork *work) {
    effActiveInstanceOperations[work->kind].draw((void *)work);
}

void effUpdateAndDrawActiveInstance(EffClassWork *work) {
    effAdvanceActiveInstanceFrame(work);
    effDispatchActiveInstanceDraw(work);
}

void effCopyActiveInstancePosition(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyActiveInstanceOrientation(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetActiveInstanceColor(s32 work, u32 value) {
    ((EffClassWork *)work)->color = value;
}

void effSetActiveInstanceMatrixComponent(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

extern SdfAsset *sdfCreateAssetWithDrawEntries(void);
extern void sdfQueueAssetRelease(s32 assetAddress);

extern void func_003332D0(void *, f32);

extern EffPacketParams D_004582B0;

extern s32 D_00437E58[2];

extern void *D_00437E60[2];

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BC10);

EffTrackSet *effCreateTrackSet(s32 count, u16 kind) {
    s32 rows;
    s32 cols;
    s32 size;
    struct SdfMemBlock *base;
    u8 *data;
    EffTrackSet *set;

    switch (kind) {
    case 0:
        cols = 0;
        rows = count * 5;
        break;
    case 1:
        cols = 0;
        rows = count * 13;
        break;
    case 2:
        rows = count * 4;
        cols = 0;
        break;
    case 3:
        rows = count * 4;
        cols = rows;
        break;
    case 4:
        rows = count * 4;
        cols = rows;
        break;
    default:
        rows = 0;
        cols = 0;
        break;
    }
    size = ((rows * 2 + cols) * 2 + rows) * 4;
    size = (((size >> 4) + ((size & 0xF) != 0)) << 4);
    base = sdfAllocGeneralBlock(size + 0x30);
    data = (u8 *)sdfResourceRetainAddress(base);
    set = (EffTrackSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    if (cols > 0) {
        set->columns = data;
        data += cols * 8;
    } else {
        set->columns = 0;
    }
    set->color = 0x80808080;
    set->type = 2;
    set->tail = data;
    set->rows = rows;
    set->kind = kind;
    set->count = count;
    set->allocation = base;
    set->flag = 0;
    set->shared = 0;
    set->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(set->handle, 1.0f);
    memset(&D_004582B0, 0, 0x2C);
    D_004582B0.primitive = 0x4000;
    return set;
}

extern u32 D_00437E54;

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E48);

INCLUDE_SDATA(const s32, "game/code_002DE248", effFlashTextureHandles);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E54);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E58);

EffTrackSet *effCreateTrackSetWithSharedReferences(u32 count, u16 kind, u32 sharedRef) {
    EffTrackSet *effect = effCreateTrackSet(count, kind);

    if (effect->columns != 0) {
        if (sharedRef == 0) {
            switch (effect->kind) {
            case 3:
                if (D_00437E58[0] == 0) {
                    D_00437E60[0] = effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)effFlashTextureHandles, 0x100);
                }
                D_00437E58[0]++;
                break;
            case 4:
                if (D_00437E58[1] == 0) {
                    D_00437E60[1] = effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)D_00437E54, 0x101);
                }
                D_00437E58[1]++;
                break;
            }
        } else {
            effect->shared = effCreateSharedTextureReference((struct SdfTextureFileHeader *)sharedRef);
        }
    }
    return (u32)effect;
}





/* The ring source selects a minimum of three segments and repeats its three colors. */
typedef struct EffRingSource {
    u8 pad_00[0x38];
    u32 segments;       // 0x38
    u8 pad_3C[8];
    u32 firstColor;     // 0x44
    u32 middleColor;    // 0x48
    u32 lastColor;      // 0x4C
} EffRingSource;



/* Class kind 3 copies this complete 0x68-byte serialized parameter record.
 * The color/alpha prefix is the existing interpolation provider's layout;
 * alphaTrack.surfaceIndex at 0x28 also selects the point-set draw type. */
typedef struct EffRadialRingParams {
    SdfColorTrack colorTrack; /* 0x00 */
    SdfAlphaTrack alphaTrack; /* 0x24 */
    u32 duration;            /* 0x34 */
    u32 segments;            /* 0x38 */
    u8 flag;                 /* 0x3C */
    u8 pad3D[3];
    f32 baseRadius;          /* 0x40 */
    u32 firstColor;          /* 0x44 */
    u32 middleColor;         /* 0x48 */
    u32 lastColor;           /* 0x4C */
    f32 widths[3];           /* 0x50 */
    f32 speed;               /* 0x5C */
    f32 acceleration;        /* 0x60 */
    u8 reverseTime;          /* 0x64 */
    u8 pad65[3];
} EffRadialRingParams;

/* Class kind 3 owns a separately allocated four-byte point-set reference. */
typedef struct EffRingResource {
    EffPointSet *pointSet;
} EffRingResource;

typedef char EffRadialRingParams_size_must_be_0x68[(sizeof(EffRadialRingParams) == 0x68) ? 1 : -1];
typedef char EffRingResource_size_must_be_0x04[(sizeof(EffRingResource) == 0x04) ? 1 : -1];

/* Release the track set's retained reference, draw asset, and allocation. */
void effReleaseResourceRefs(EffTrackSet *work) {
    s32 *count;
    void **slot;

    if (work->columns != 0) {
        if (work->shared == 0) {
            switch (work->kind) {
            case 3:
                count = &D_00437E58[0];
                if (--*count == 0) {
                    slot = &D_00437E60[0];
                    effReleaseSharedReference(*slot);
                    *slot = 0;
                }
                break;
            case 4:
                count = &D_00437E58[1];
                if (--*count == 0) {
                    slot = &D_00437E60[1];
                    effReleaseSharedReference(*slot);
                    *slot = 0;
                }
                break;
            }
        } else {
            effReleaseSharedReference(work->shared);
        }
    }
    sdfQueueAssetRelease((s32)work->handle);
    sdfReleaseResourceAllocation(work->allocation);
}

/* Copy a track set: same size and kind, retaining the source's shared reference (or counting one more user of the built-in one). */
EffTrackSet *effDuplicateResourceRefs(const EffTrackSet *original) {
    EffTrackSet *effect = effCreateTrackSet(original->count, original->kind);

    if (effect->columns != 0) {
        if (original->shared != 0) {
            effect->shared = effRetainSharedReference(original->shared);
        } else {
            switch (effect->kind) {
            case 3:
                D_00437E58[0]++;
                break;
            case 4:
                D_00437E58[1]++;
                break;
            }
        }
    }
    return effect;
}

extern SdfPoolNode *D_003E9B60[8];
extern u32 D_003E9A50[20];
extern u32 D_003E9AA0[12];
extern u32 D_003E9AD0[20];
extern u32 D_003E9B20[16];
extern SdfTex *func_002DDD60(void *, RefObj *);
struct SdfTextParam;
extern void func_003332E8(struct SdfTextParam *, u32);

void func_002E5E88(u8 *work, void *matrix) {
    EffTrackSet *track = (EffTrackSet *)work;
    SdfPoolNode *surface;
    void *list;
    EffGsPacket *packet;
    s32 remaining;
    u32 kind;
    u32 *colors;
    u128 *positions;
    u32 *texcoords;
    u32 color;

    if ((track->color & 0xFF000000) == 0) {
        return;
    }

    surface = D_003E9B60[track->type];
    list = (void *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    if (matrix == NULL) {
        VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
    } else {
        VU0_LOAD_MATRIX(matrix);
    }
    sdfConsAppendVuPacket(list, 0);

    if (track->columns != NULL) {
        RefObj *reference = track->shared;
        SdfTex *texture;

        if (reference == NULL) {
            switch (track->kind) {
            case 3:
                reference = D_00437E60[0];
                break;
            case 4:
                reference = D_00437E60[1];
                break;
            default:
                texture = NULL;
                goto setTexture;
            }
            texture = func_002DDD60(surface, reference);
        } else {
            texture = func_002DDD60(surface, reference);
        }
setTexture:
        func_003332E8((struct SdfTextParam *)track->handle, (u32)texture);
    }
    sdfConsAppendAssetPacket(list, track->handle, 0);

    if (track->flag == 0) {
        packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x31801;
        packet->registerAddress = 0x47;
        sdfAppendPacket(list, (u32)packet);
    }

    kind = track->kind;
    colors = (u32 *)track->tail;
    positions = (u128 *)track->buffer;
    texcoords = (u32 *)track->columns;
    color = track->color;
    remaining = track->rows;
    D_004582B0.colors = colors;
    D_004582B0.positions = positions;
    D_004582B0.texcoords = texcoords;
    D_004582B0.unk08 = color;

    switch (kind) {
    case 0:
        D_004582B0.parameterCount = 15;
        D_004582B0.vertexCount = 25;
        D_004582B0.parameters = D_003E9A50;
        while (remaining >= 25) {
            remaining -= 25;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
            D_004582B0.positions += 25;
            D_004582B0.colors += 25;
        }
        if (remaining >= 5) {
            D_004582B0.parameterCount = (s16)((remaining / 5) * 3);
            D_004582B0.vertexCount = (s16)remaining;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
        }
        break;
    case 1:
        D_004582B0.parameterCount = 12;
        D_004582B0.vertexCount = 13;
        D_004582B0.parameters = D_003E9AA0;
        while (remaining >= 13) {
            remaining -= 13;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
            D_004582B0.positions += 13;
            D_004582B0.colors += 13;
        }
        break;
    case 2:
        D_004582B0.parameterCount = 16;
        D_004582B0.vertexCount = 32;
        D_004582B0.parameters = D_003E9AD0;
        while (remaining >= 32) {
            remaining -= 32;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
            D_004582B0.positions += 32;
            D_004582B0.colors += 32;
        }
        if (remaining >= 4) {
            D_004582B0.parameterCount = (s16)((remaining / 4) * 2);
            D_004582B0.vertexCount = (s16)remaining;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
        }
        break;
    case 3:
    case 4:
        D_004582B0.parameterCount = 16;
        D_004582B0.vertexCount = 32;
        D_004582B0.parameters = D_003E9B20;
        while (remaining >= 32) {
            remaining -= 32;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
            D_004582B0.positions += 32;
            D_004582B0.texcoords += 64;
            D_004582B0.colors += 32;
        }
        if (remaining >= 4) {
            D_004582B0.parameterCount = (s16)((remaining / 4) * 2);
            D_004582B0.vertexCount = (s16)remaining;
            sdfAppendPacket(list, func_00167A10(&D_004582B0));
        }
        break;
    default:
        break;
    }

    if (track->flag == 0) {
        packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x51801;
        packet->registerAddress = 0x47;
        sdfAppendPacket(list, (u32)packet);
    }
    effSubmitSurfacePacket(surface, list);
}


extern u32 D_00437E48[2];

void effLoadFlashTextures(void) {
    D_00437E48[0] = (u32)sdfReadNamedResource("/effect/flash00.tmx", &effFlashTextureHandles, 0);
    D_00437E48[1] = (u32)sdfReadNamedResource("/effect/flash01.tmx", &effFlashTextureHandles + 1, 0);
}

u32 effGetFlashTextureHandle(s32 index) {
    return (&effFlashTextureHandles)[index];
}

typedef struct EffCounterHeader {
    u32 unk00;
    u32 frame;
} EffCounterHeader;

/* Class draw modes own either a counter header or per-segment scales. */
typedef struct EffClassDrawState {
    union {
        EffCounterHeader *ring;
        f32 *scales;
    };
    EffClassWork *effect;
    EffTrackSet *trackSet;
    struct SdfMemBlock *allocation;
} EffClassDrawState;

void effResetRingResourceFrame(s32 work) {
    ((EffClassDrawState *)((EffClassWork *)work)->resource)->ring->frame = 0;
}

/* Create a point-set reference and initialize four colors per segment. */
u32 *effSegmentPointerSet(u8 *work) {
    u32 *pointSetRef = sdfAllocSizeClassBlock(4);
    u32 segments = ((EffRingSource *)work)->segments;
    EffPointSet *pointSet;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (segments < 3) {
        ((EffRingSource *)work)->segments = 3;
        segments = 3;
    }
    pointSet = (EffPointSet *)effCreatePointSet4(segments);
    first = ((EffRingSource *)work)->firstColor;
    groups = pointSet->rows / 4;
    *pointSetRef = (u32)pointSet;
    entry = (u32 *)pointSet->tail;
    second = ((EffRingSource *)work)->middleColor;
    third = ((EffRingSource *)work)->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return pointSetRef;
}

void effReleaseRingResourceHandle(u32 handle) {
    effAssetQueueRelease(*(u32 *)handle);
    sdfReleaseChipBlock((void *)handle);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E64F0);

void billDrawCellBlendA(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[0];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    if ((packed & 0xFF000000) != 0) {
        ((EffBillOutput *)out)->textureId = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
        ((EffBillOutput *)out)->outputMode = ((EffBillOutputHeader *)config)->outputMode;
        VU0_LOAD_VF(vf10, work->transform);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9100);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALE_MATRIX_ROWS(vf10);
        VU0_LOAD_VF(vf10, work);
        VU0_SET_W_ONE(vf10);
        VU0_MOVE_VF(vf31, vf10);
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups(out, mtx);
    }
}

void effResetClassFrameAndFlags(s32 work) {
    EffClassWork *resource;

    resource = ((EffClassDrawState *)((EffClassWork *)work)->resource)->effect;
    ((EffCounterHeader *)((EffClassDrawState *)((EffClassWork *)work)->resource)->trackSet)->frame = 0;
    effInitializeClassFrame(resource);
}

extern s32 effSharedRandomState[4];
/* The cached class payload is copied from the 0x5C-byte source prefix. */
typedef struct EffRingClassConfig {
    EffRingSource ring;
    u8 pad50[0xC];
    u8 classConfig[0x5C];
    u8 padB8[0x10];
    f32 scale;
    f32 scaleRand;
} EffRingClassConfig;


EffClassDrawState *effCreateScaledClassDrawState(EffRingClassConfig *source) {
    u32 count = source->ring.segments;
    u32 size;
    struct SdfMemBlock *allocation;
    f32 *scales;
    EffClassDrawState *state;
    EffTrackSet *tracks;
    u32 *colors;
    u32 first;
    u32 second;
    u32 i;

    if (count < 3) {
        source->ring.segments = 3;
        count = 3;
    }
    size = count * sizeof(f32);
    allocation = sdfAllocGeneralBlock(size + sizeof(EffClassDrawState));
    scales = (f32 *)sdfResourceRetainAddress(allocation);
    state = (EffClassDrawState *)((u8 *)scales + size);
    state->allocation = allocation;
    state->scales = scales;
    memcpy(source->classConfig, source, sizeof(source->classConfig));
    state->effect = effCreateClassWork(1, source->classConfig);
    tracks = effCreateTrackSetWithSharedReferences(count, 2, 0);
    first = source->ring.firstColor;
    state->trackSet = tracks;
    colors = (u32 *)tracks->tail;
    second = source->ring.middleColor;
    for (i = 0; i < count; i++) {
        colors[0] = first;
        colors[1] = first;
        colors[2] = (first & second) + (((first ^ second) & 0xFEFEFEFE) >> 1);
        colors[3] = first;
        colors += 4;
        *scales++ = source->scale * (effMiscRandUnitFloat(effSharedRandomState) * source->scaleRand + (1.0f - source->scaleRand));
    }
    return state;
}

void effReleaseClassDrawResources(s32 work) {
    effReleaseResourceRefs(((EffClassDrawState *)work)->trackSet);
    effDestroyClassWork(((EffClassDrawState *)work)->effect);
    sdfReleaseResourceAllocation(((EffClassDrawState *)work)->allocation);
}

typedef struct EffClassFrameResource {
    u8 pad_00[8];
    u32 frameCount;
    u8 pad_0C[4];
    u8 *rows;
} EffClassFrameResource;

typedef struct EffClassRowOutput {
    u8 pad_00[0x1C];
    u8 *rows;
} EffClassRowOutput;

extern void effAdvanceClassFrame(EffClassWork *);

void func_002E6B68(BillCellDrawWork *work) {
    u8 *config = work->config;
    s32 limit = work->frameLimit;
    s32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    EffClassWork *classWork;
    f32 *scales;
    EffClassFrameResource **resource;
    EffClassRowOutput *output;
    EffClassFrameResource *frames;
    u8 *src;
    u8 *dst;
    u8 *wrap;
    u32 count;
    u32 lastFrame;
    u32 i;

    classWork = (EffClassWork *)list[1];
    resource = (EffClassFrameResource **)classWork->resource;
    output = (EffClassRowOutput *)list[2];
    scales = (f32 *)list[0];

    if (progress < limit && progress != 0) {
        return;
    }
    effAdvanceClassFrame(classWork);
    count = ((EffBillTimedHeader *)config)->count;
    frames = *resource;
    lastFrame = frames->frameCount - 1;
    dst = output->rows;
    src = frames->rows;
    if (config[0xC4] != 0) {
        return;
    }
    i = 0;
    if (count == 0) {
        return;
    }
    wrap = src + lastFrame * 0x10 - 0x60;
    do {
        VU0_LOAD_VF(vf10, src + 0x10);
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf11, src + 0x30);
        VU0_STORE_VF(vf10, dst + 0x20);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_SCALAR_OP(*scales, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, dst);
        PCP_COPY_VECTOR(dst + 0x10, src + 0x50);
        if (i == 0) {
            PCP_COPY_VECTOR(dst + 0x30, wrap);
        } else {
            PCP_COPY_VECTOR(dst + 0x30, src - 0x30);
        }
        i++;
        dst += 0x40;
        wrap += 0x40;
        src += 0x40;
        scales++;
    } while (i < count);
}

extern void effRunClassPostFrame(EffClassWork *);

void billDrawClassUpdatedCellBlend(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillTimedHeader *)config)->time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[2];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillTimedHeader *)config)->colorTrack, &((EffBillTimedHeader *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_UNCLOBBERED(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->color = packed;
    if (packed & 0xFF000000) {
        EffClassWork *dst = (EffClassWork *)list[1];

        dst->color = work->baseColor;
        dst->scale = work->scale;
        PCP_COPY_VECTOR(dst->transform, work);
        PCP_COPY_VECTOR(dst->transform + 0x10, work->transform);
        effRunClassPostFrame(dst);
        *(u32 *)out = ((EffBillTimedHeader *)config)->alphaTrack.surfaceIndex;
        ((EffBillOutput *)out)->mode = ((EffBillOutputHeader *)config)->outputMode;
        VU0_LOAD_VF(vf10, work->transform);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9100);
        VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALE_MATRIX_ROWS(vf10);
        VU0_LOAD_VF(vf10, work);
        VU0_SET_W_ONE(vf10);
        VU0_MOVE_VF(vf31, vf10);
        VU0_STORE_MATRIX(mtx);
        func_002E5E88(out, mtx);
    }
}

void effResetClassRingFrame(EffClassWork *work) {
    ((EffRingResource *)work->resource)->pointSet->color = 0;
}

/* Create a point-set reference and initialize four colors per segment. */
EffRingResource *effCreateRingHandle(EffRadialRingParams *work) {
    EffRingResource *pointSetRef = sdfAllocSizeClassBlock(sizeof(EffRingResource));
    u32 segments = work->segments;
    EffPointSet *pointSet;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (segments < 3) {
        work->segments = 3;
        segments = 3;
    }
    pointSet = (EffPointSet *)effCreatePointSet4(segments);
    first = work->firstColor;
    groups = pointSet->rows / 4;
    pointSetRef->pointSet = pointSet;
    entry = (u32 *)pointSet->tail;
    second = work->middleColor;
    third = work->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return pointSetRef;
}

void effReleaseRingHandle(EffRingResource *handle) {
    effAssetQueueRelease((u32)handle->pointSet);
    sdfReleaseChipBlock(handle);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E6F48);

void billDrawCellBlendB(EffClassWork *work) {
    EffRadialRingParams *config = work->payload;
    u32 limit = work->frame;
    u32 progress = config->duration;
    EffRingResource *list = (EffRingResource *)work->resource;
    EffPointSet *out = list->pointSet;
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->colorTrack, &config->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->color;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    out->color = blended[0];
    if ((packed & 0xFF000000) != 0) {
        out->type = config->alphaTrack.surfaceIndex;
        out->flag = config->flag;
        VU0_LOAD_VF(vf10, work->vectors.orientation);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9100);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALE_MATRIX_ROWS(vf10);
        VU0_LOAD_VF(vf10, work);
        VU0_SET_W_ONE(vf10);
        VU0_MOVE_VF(vf31, vf10);
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups((u8 *)out, mtx);
    }
}

EffClassWork *effCreateClassWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effClassWorkOperations[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED(vf0, effect);
    VU0_STORE_VF_UNCLOBBERED(vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = effClassWorkOperations[kind].createResource(source);
    effClassWorkOperations[kind].initialize(effect);
    return (EffClassWork *)effect;
}

void effCreateClassWorkFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer((FileJobPayload *)request);
    effCreateClassWork(((FileJob *)request)->option, source);
}

void effDestroyClassWork(EffClassWork *work) {
    effClassWorkOperations[work->kind].destroyResource(work->resource);
    sdfReleaseChipBlock(work);
}

void effCreateClassWorkFromRequest(s32 work) {
    effCreateClassWork(*(u16 *)(work + 0x2c), ((EffClassWork *)work)->payload);
}

void effInitializeClassFrame(EffClassWork *work) {
    effClassWorkOperations[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceClassFrame(EffClassWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        effClassWorkOperations[work->kind].update();
        work->frame++;
    }
}

void effRunClassPostFrame(EffClassWork *work) {
    effClassWorkOperations[work->kind].draw((void *)work);
}

void effUpdateClassFrame(EffClassWork *work) {
    effAdvanceClassFrame(work);
    effRunClassPostFrame(work);
}

void effSetClassWorkPrimaryTransformVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetClassWorkSecondaryTransformVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetClassWorkColor(s32 work, u32 value) {
    ((EffClassWork *)work)->color = value;
}

void effSetClassWorkScale(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}


extern EffPacketParams D_004582E0[];

u8 *effCreatePointSet4(u32 count) {
    s32 rows = count * 4 + 4;
    s32 size = rows * 20;
    struct SdfMemBlock *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x20);
    data = (u8 *)sdfResourceRetainAddress(base);
    set = (EffPointSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(set->handle, 1.0f);
    memset(D_004582E0, 0, 0x2C);
    D_004582E0[0].primitive = 0x4000;
    return (u8 *)set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effAssetQueueRelease(s32 work) {
    sdfQueueAssetRelease((s32)((EffPointSet *)work)->handle);
    sdfReleaseResourceAllocation(((EffPointSet *)work)->allocation);
}

/* vu0 routine: SDK loads the supplied transform or constructs identity. */
void effDrawFourPointGroups(u8 *work, void *matrix) {
    EffPointSet *set = (EffPointSet *)work;
    void *list;
    void *setup;
    EffGsPacket *packet;
    s32 remaining;
    s32 surfaceId;

    if (set->color & 0xFF000000) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        if (matrix == NULL) {
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdfConsAppendVuPacket(list, 0);
        sdfConsAppendAssetPacket(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, (u32)packet);
        }
        remaining = set->rows;
        D_004582E0[0].colors = (u32 *)set->tail;
        D_004582E0[0].positions = (u128 *)set->buffer;
        D_004582E0[0].unk08 = set->color;
        D_004582E0[0].parameterCount = 12;
        D_004582E0[0].vertexCount = 12;
        D_004582E0[0].parameters = D_003E9BE0;
        while (remaining >= 12) {
            remaining -= 8;
            sdfAppendPacket(list, func_00167A10(D_004582E0));
            D_004582E0[0].positions += 8;
            D_004582E0[0].colors += 8;
        }
        if (remaining >= 8) {
            D_004582E0[0].parameterCount = 6;
            D_004582E0[0].vertexCount = remaining;
            sdfAppendPacket(list, func_00167A10(D_004582E0));
        }
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, (u32)packet);
        }
        if (set->type < 5) {
            effSubmitSurfacePacket(D_003E9C28[set->type], list);
        } else {
            EffGsPacket *blendPacket;

            surfaceId = set->type == 5 ? 51 : 56;
            setup = (void *)sdfAllocPacketAligned(0x20);
            sdfInitPacketList(setup);
            blendPacket = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            blendPacket->registerValue = 6;
            blendPacket->dmaTag = 2;
            blendPacket->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            blendPacket->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            blendPacket->registerList = 0xE;
            blendPacket->registerAddress = 0x42;
            sdfAppendPacket(setup, (u32)blendPacket);
            effSubmitSurfacePacket(&kwlnDrawSurfaces[surfaceId], setup);
            blendPacket = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            blendPacket->dmaTag = 2;
            blendPacket->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            blendPacket->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            blendPacket->registerList = 0xE;
            switch (surfaceId) {
            case 51:
                blendPacket->registerValue = 0x44;
                break;
            case 56:
                blendPacket->registerValue = 0x42;
                break;
            }
            blendPacket->registerAddress = 0x42;
            sdfAppendPacket(list, (u32)blendPacket);
            effSubmitSurfacePacket(&kwlnDrawSurfaces[surfaceId], list);
        }
    }
}

typedef struct EffectSlotNode54 {
    u32 count;
    u32 color;
    f32 opacity;
    u32 kind;             // 0x0C
    EffSurfaceParams params; // 0x10
    u32 index;             // 0x2C
    u32 handleBuffer;      // 0x30
    BillObj *billResource; // 0x34
    FileJobPayload **jobs;             // 0x38
    struct SdfMemBlock *jobAllocation;         // 0x3C
    u32 *queues;           // 0x40
    u32 queueBuffer;       // 0x44
    struct EffExpandedList *resourceHolder; // 0x48
    FileSlotTable *record; // 0x4C
    u16 active;            // 0x50
} EffectSlotNode54;

EffectSlotNode54 *effCreateSurfaceNode(u32 count) {
    EffectSlotNode54 *node = sdfAllocAndClearQuadwords(sizeof(EffectSlotNode54));
    node->count = count;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->index = 0;
    node->record = 0;
    node->active = 1;
    return node;
}

s32 effCreateSurfaceNodeForGrid(s32 source) {
    return effCreateSurfaceNode(effSlotCount((u8 *)source, 100));
}

s32 effResourceReferenceReplaceFromFile(u8 *source) {
    s32 primary = (s32)fileResolvePrimaryBuffer((FileJobPayload *)source);
    s32 next = primary + 0x1C;
    s32 object = effCreateSurfaceNodeForGrid(next);

    effRebuildSurfaceHandles(object, ((FileJob *)source)->option, primary);
    effReplaceResourceRef(object, ((FileJob *)source)->option, next);
    return object;
}

extern s32 effCreateSurfaceNodeForGrid(s32);

extern void effRebuildSurfaceHandles(s32, s32, s32);

extern void effReplaceResourceRef(s32, s32, s32);

s32 effCreateSurfaceNodeFromPayload(u16 kind, s32 source) {
    s32 next = source + 0x1C;
    s32 object = effCreateSurfaceNodeForGrid(next);
    effRebuildSurfaceHandles(object, kind, source);
    effReplaceResourceRef(object, kind, next);
    return object;
}

extern void effReplaceSurfacePrimaryBillboard(s32 *, s32 *);

extern void effReplaceSurfaceFlaggedBillboard(s32 *, s32 *);

extern void effSetSurfaceRetainedResource(s32 *, s32);

extern void effRebuildSurfaceJobs(EffectSlotNode54 *, void *);

extern void effSurfaceNodeCreateQueues(EffectSlotNode54 *, void *);

extern void effReplaceSurfaceResourceHolder(s32, u32);

void effConfigureSurfaceNodeByKind(s32 *object, s32 kind, s32 *settings) {
    u16 type = kind;
    switch (type) {
    case 1:
        effReplaceSurfacePrimaryBillboard(object, settings);
        break;
    case 2:
        effReplaceSurfaceFlaggedBillboard(object, settings);
        break;
    case 4:
        effSetSurfaceRetainedResource(object, *settings);
        break;
    case 5:
        effRebuildSurfaceJobs((EffectSlotNode54 *)object, settings);
        break;
    case 6:
        effSurfaceNodeCreateQueues((EffectSlotNode54 *)object, settings);
        break;
    case 7:
        effReplaceSurfaceResourceHolder((s32)object, (u32)settings);
        break;
    }
    object[3] = type;
}

s32 effCreateSurfaceNodeFromFile(s32 *source) {
    s32 *object = (s32 *)effResourceReferenceReplaceFromFile((u8 *)source);
    s32 *data = fileResolveSecondaryBuffer((FileJobPayload *)source);
    if (data != NULL) {
        effConfigureSurfaceNodeByKind(object, ((FileJob *)source)->slots[0].selector, data);
    }
    return (s32)object;
}

extern void effReleaseSurfaceGridBuffers(s32);

void effDestroySurfaceNode(EffectSlotNode54 *node) {
    u32 i;
    u32 count;

    if (node->billResource != 0) {
        billDispatchByKind(node->billResource);
    }
    if (node->jobAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobAllocation);
    }
    if (node->queueBuffer != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(node->queueBuffer));
    }
    if (node->index != 0) {
        for (i = 0; i < node->count; i++) {
            effReleaseSurfaceGridBuffers(((u32 *)node->index)[i]);
        }
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(node->handleBuffer));
    }
    if (node->resourceHolder != 0) {
        effReleaseReferenceHolder(node->resourceHolder);
    }
    if (node->record != 0) {
        fileReleaseGridRecordHandle(node->record);
    }
    sdfReleaseChipBlock(node);
}

s32 effRecreateSurfaceNodeFromWork(u8 *work) {
    FileSlotTable *config = ((EffectSlotNode54 *)work)->record;
    s32 arg = (s32)config->data1;
    s32 object = effCreateSurfaceNodeForGrid(arg);

    effRebuildSurfaceHandles(object, ((EffectSlotNode54 *)work)->record->type, (s32)work + 0x10);
    effReplaceResourceRef(object, ((EffectSlotNode54 *)work)->record->type, arg);
    return object;
}


void func_002E7F60(EffectSlotNode54 *, u8 *);

u32 effCreateSurfaceGridWithConfiguration(u8 *work) {
    FileSlotTable *config = ((EffectSlotNode54 *)work)->record;
    s32 arg = (s32)config->data1;
    s32 object = effCreateSurfaceNodeForGrid(arg);

    effRebuildSurfaceHandles(object, ((EffectSlotNode54 *)work)->record->type, (s32)work + 0x10);
    effReplaceResourceRef(object, ((EffectSlotNode54 *)work)->record->type, arg);
    func_002E7F60(object, work);
    return object;
}

void func_002E7F60(EffectSlotNode54 *dst, u8 *work) {
    EffectSlotNode54 *src = (EffectSlotNode54 *)work;
    u32 count;
    s32 size;
    u32 i;

    switch (src->kind) {
    case 1:
    case 2:
    case 4:
        if (dst->billResource != 0) {
            billDispatchByKind(dst->billResource);
        }
        dst->billResource = billCloneObjectRetainingSharedData(src->billResource);
        billMarkKindOneFlag(dst->billResource);
        if (dst->record != 0) {
            billSetBillboardMode(dst->billResource, (s16)((FileKeyBlock *)dst->record->data0)->alphaTrack.surfaceIndex);
        }
        break;
    case 5:
        count = src->record->count;
        if (dst->jobAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(dst->jobs[i]);
            }
            sdfReleaseResourceAllocation(dst->jobAllocation);
            dst->jobs = 0;
            dst->jobAllocation = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobAllocation = sdfAllocGeneralBlock(size);
        dst->jobs = (FileJobPayload **)sdfResourceRetainAddress(dst->jobAllocation);
        for (i = 0; i < count; i++) {
            dst->jobs[i] = fileJobCreateChild(src->jobs[0]);
        }
        break;
    case 6:
        count = src->record->count;
        if (dst->queueBuffer != 0) {
            for (i = 0; i < count; i++) {
                fileQueueDestroy(dst->queues[i]);
            }
            sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(dst->queueBuffer));
            dst->queues = 0;
            dst->queueBuffer = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->queueBuffer = (u32)sdfAllocGeneralBlock(size);
        dst->queues = (u32 *)sdfResourceRetainAddress((struct SdfMemBlock *)(dst->queueBuffer));
        for (i = 0; i < count; i++) {
            dst->queues[i] = (u32)fileQueueClone((void *)src->queues[0]);
        }
        break;
    case 7:
        if (dst->resourceHolder != 0) {
            effReleaseReferenceHolder(dst->resourceHolder);
        }
        dst->resourceHolder = effReferenceObjectRetain(src->resourceHolder);
        break;
    }
    dst->kind = src->kind;
}



extern u32 effCreateSurfaceGridNode(u32, u32);

extern void effFillSurfaceGridColorGradient(u32, u32 *);

/* Replace the per-cell grid handles from the copied source header. kind is unused here. */
void effRebuildSurfaceHandles(s32 nodeAddr, s32 kind, s32 source) {
    EffectSlotNode54 *node = (EffectSlotNode54 *)nodeAddr;
    u32 *params = (u32 *)source;
    u32 size;
    u32 i;
    u32 handle;

    node->params = *(EffSurfaceParams *)params;
    size = node->count * 4;
    if (size == 0) {
        return;
    }
    if (node->index != 0) {
        for (i = 0; i < node->count; i++) {
            effReleaseSurfaceGridBuffers(((u32 *)node->index)[i]);
        }
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(node->handleBuffer));
    }
    node->handleBuffer = (u32)sdfAllocGeneralBlock(size);
    node->index = sdfResourceRetainAddress((struct SdfMemBlock *)(node->handleBuffer));
    for (i = 0; i < node->count; i++) {
        handle = effCreateSurfaceGridNode(params[0], params[1]);
        ((u32 *)node->index)[i] = handle;
        effFillSurfaceGridColorGradient(handle, params + 2);
    }
}

void effReplaceResourceRef(s32 nodeAddr, s32 entryId, s32 resource) {
    EffectSlotNode54 *node = (EffectSlotNode54 *)nodeAddr;
    if (node->record != NULL) {
        fileReleaseGridRecordHandle(node->record);
    }
    node->record = fileAllocateGridRecordSlots(entryId & 0xffff, node->count, (const void *)resource);
}

void effSetSurfaceRetainedResource(s32 *object, s32 arg) {
    u8 *work = (u8 *)object;
    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = effCreateBillboardSharingIndexedResource(arg);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, (s16)((FileKeyBlock *)((EffectSlotNode54 *)work)->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effReplaceSurfacePrimaryBillboard(s32 *object, s32 *settings) {
    u8 *work = (u8 *)object;

    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = billCreateIndexed(0, (u32)settings);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, (s16)((FileKeyBlock *)((EffectSlotNode54 *)work)->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effReplaceSurfaceFlaggedBillboard(s32 *object, s32 *settings) {
    u8 *work = (u8 *)object;

    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = billCreateIndexed(1, (u32)settings);
    billMarkKindOneFlag(((EffectSlotNode54 *)work)->billResource);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, (s16)((FileKeyBlock *)((EffectSlotNode54 *)work)->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effRebuildSurfaceJobs(EffectSlotNode54 *node, void *source) {
    u32 count = node->record->count;
    u32 i;
    u32 size;

    if (node->jobAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobAllocation);
        node->jobs = 0;
        node->jobAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobAllocation = sdfAllocGeneralBlock(size);
        node->jobs = (FileJobPayload **)sdfResourceRetainAddress(node->jobAllocation);
        node->jobs[0] = fileJobCreateFromJob((FileJobPayload *)source);
        for (i = 1; i < count; i++) {
            node->jobs[i] = fileJobCreateChild(node->jobs[0]);
        }
    }
}

extern void *fileQueueClone(void *);

void effSurfaceNodeCreateQueues(EffectSlotNode54 *node, void *source) {
    u32 count = node->record->count;
    u32 i;
    u32 size;

    if (node->queueBuffer != 0) {
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(node->queueBuffer));
        node->queues = 0;
        node->queueBuffer = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueBuffer = (u32)sdfAllocGeneralBlock(size);
        node->queues = (u32 *)sdfResourceRetainAddress((struct SdfMemBlock *)(node->queueBuffer));
        node->queues[0] = (u32)fileCloneQueueEntries((FileQueue *)source);
        for (i = 1; i < count; i++) {
            node->queues[i] = (u32)fileQueueClone((void *)node->queues[0]);
        }
    }
}

void effReplaceSurfaceResourceHolder(s32 node, u32 resource) {
    struct EffExpandedList *holder;

    if (((EffectSlotNode54 *)node)->resourceHolder != 0) {
        effReleaseReferenceHolder(((EffectSlotNode54 *)node)->resourceHolder);
    }
    holder = func_002DDF48(resource);
    ((EffectSlotNode54 *)node)->resourceHolder = holder;
}

void effClearSurfaceNodeRecordReferences(s32 node) {
    if (((EffectSlotNode54 *)node)->record != 0) {
        fileClearRecordReferences(((EffectSlotNode54 *)node)->record);
        return;
    }
}

void effAcquireSurfaceRecord(s32 node) {
    if (effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) {
        return;
    }
    if (((EffectSlotNode54 *)node)->record != 0) {
        fileAcquireRecord(((EffectSlotNode54 *)node)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E8770);

void effUpdateSurfaceRecordAndDraw(u32 node) {
    effAcquireSurfaceRecord(node);
    func_002E8770(node);
}

void effMenuRecordVectorSet(s32 node) {
    mnuRecordSetVector(((EffectSlotNode54 *)node)->record);
}

void effSetSurfaceRecordSecondaryVector(s32 node) {
    fileSetRecordSecondVector(((EffectSlotNode54 *)node)->record);
}

void effSetSurfaceNodeColor(s32 node, u32 color) {
    ((EffectSlotNode54 *)node)->color = color;
}

void effIndexedFloatCallbackDispatch(u8 *p, f32 value) {
    ((EffectSlotNode54 *)p)->opacity = value;
    dds3DispatchIndexedCallback((s32)((EffectSlotNode54 *)p)->record, value);
}

extern EffPacketParams D_00458310;

extern EffPacketParams D_00458340;

extern u8 D_003E9C40[];

extern u8 D_003E9C90[];

/* Surface node: 0x4C bytes at the end of the retained block, after the vertex rows. */
typedef struct EffSurfaceGridNode {
    u8 pad_00[0x20];
    s32 rows;           // 0x20
    u32 field_24;       // 0x24
    u32 field_28;       // 0x28
    u32 columns;        // 0x2C
    u32 type;           // 0x30
    u8 *buffer;         // 0x34
    u8 *tail;           // 0x38
    SdfAsset *handle;     // 0x3C
    u8 *queueA;         // 0x40
    u8 *queueB;         // 0x44
    struct SdfMemBlock *allocation; // 0x48
} EffSurfaceGridNode;

u32 effCreateSurfaceGridNode(u32 count, u32 columns) {
    s32 rows = count * columns * 3 + 6;
    s32 size = rows * 20 + 0xA0;
    struct SdfMemBlock *base;
    u8 *data;
    EffSurfaceGridNode *node;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x4C);
    data = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    node = (EffSurfaceGridNode *)(data + size);
    node->type = 2;
    node->buffer = data;
    data += rows * 16;
    node->queueA = data;
    data += 0x80;
    node->queueB = data;
    data += 0x20;
    node->field_28 = 3;
    node->rows = rows;
    node->columns = columns;
    node->allocation = base;
    node->tail = data;
    node->field_24 = 0;
    node->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(node->handle, 1.0f);
    memset(&D_00458310, 0, sizeof(EffPacketParams));
    D_00458310.primitive = 0x4000;
    D_00458310.parameters = (u32 *)D_003E9C40;
    memset(&D_00458340, 0, sizeof(EffPacketParams));
    D_00458340.primitive = 0x4000;
    D_00458340.parameters = (u32 *)D_003E9C90;
    D_00458340.parameterCount = 6;
    D_00458340.vertexCount = 8;
    return (u32)node;
}

void effFillSurfaceGridColorGradient(u32 nodeAddr, u32 *colors) {
    EffSurfaceGridNode *node = (EffSurfaceGridNode *)nodeAddr;
    u32 count = node->rows / 3;
    u32 *out = (u32 *)node->tail;
    f32 step = 1.0f / count;
    f32 t = 0.0f;
    f32 corner0[4];
    f32 corner2[4];
    f32 corner1[4];
    f32 corner3[4];
    s32 color0[4];
    s32 color1[4];
    s32 color2[4];
    s32 color3[4];
    s32 blendedA[4];
    s32 blendedB[4];
    u32 packedA;
    u32 packedB;
    u32 i;

    color0[0] = colors[0];
    EE_MMI_RGBA_UNPACK(color0, 1.0f / 128.0f);
    VU0_STORE_VF_UNCLOBBERED(vf10, corner0);
    color1[0] = colors[1];
    EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
    VU0_STORE_VF_UNCLOBBERED(vf10, corner1);
    color2[0] = colors[2];
    EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
    VU0_STORE_VF_UNCLOBBERED(vf10, corner2);
    color3[0] = colors[3];
    EE_MMI_RGBA_UNPACK(color3, 1.0f / 128.0f);
    VU0_STORE_VF_UNCLOBBERED(vf10, corner3);
    for (i = 0; i < count; i++) {
        VU0_LOAD_VF(vf10, corner0);
        VU0_LOAD_VF(vf11, corner2);
        VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf11, vf11, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_UNIT(packedA, 128.0f);
        blendedA[0] = packedA;
        out[1] = blendedA[0];
        VU0_LOAD_VF(vf10, corner1);
        VU0_LOAD_VF(vf11, corner3);
        VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf11, vf11, vf2x");
        VU0_ADD(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_UNIT(packedB, 128.0f);
        blendedB[0] = packedB;
        out[2] = out[0] = blendedB[0];
        t += step;
        out += 3;
    }
}

void effReleaseSurfaceGridBuffers(s32 work) {
    sdfQueueAssetRelease((s32)((EffSurfaceGridNode *)work)->handle);
    sdfReleaseResourceAllocation(((EffSurfaceGridNode *)work)->allocation);
}

void effResetSurfaceGridFrame(s32 work) {
    ((EffSurfaceGridNode *)work)->field_24 = 0;
    ((EffSurfaceGridNode *)work)->field_28 = 3;
}

/* Three-vector ring sampler; DDS2 stores its counters at 0x20-0x2C. */
typedef struct EffRingFrameState {
    u8 pad_00[0x20];
    s32 vectorCapacity; // 0x20, used when wrapping a frame
    u32 pad_24;         // 0x24, initialized to zero
    s32 vectorCount;    // 0x28
    s32 frameStride;    // 0x2C
    u8 pad_30[4];
    s128 *vectors;      // 0x34
} EffRingFrameState;

void effCopyRingFrameVectors(u8 *work, s128 *dst, s32 frame) {
    s32 index = ((EffRingFrameState *)work)->vectorCount - (((EffRingFrameState *)work)->frameStride * (frame - 1) + frame) * 3;
    s128 *src;
    s32 i;

    if (index < 3) {
        index += ((EffRingFrameState *)work)->vectorCapacity - 3;
    }
    src = ((EffRingFrameState *)work)->vectors + index;
    for (i = 0; i < 3; i++) {
        PCP_COPY_VECTOR(dst + i, src + i);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E9B20);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E9BC8);

void effBeginMatrixVuDrawPacket(const Matrix4 *matrix) {
    void *work = (void *)sdfAllocPacketAligned(0x20);
    effCurrentRenderPacket = (u32)work;
    sdfInitPacketList(work);
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket((SdfListHead *)effCurrentRenderPacket, 0);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E9E98);

typedef struct EffDrawEntry {
    u32 handle;
    s32 frame;
    u8 pad08[8];
    void (*draw)(u8 *, u32);
} EffDrawEntry;

void effSubmitIndexedRenderPacket(u32 index) {
    u8 *entry = D_003E9CA8[index];
    ((EffDrawEntry *)entry)->draw(entry, effCurrentRenderPacket);
    effCurrentRenderPacket = 0;
}

extern EffQuadWork *func_002EA120(FileJobPayload *job);
void effDuplicateRenderResourceOwner(EffQuadWork *work, const EffQuadWork *source);

extern EffPacketParams D_00458370;
extern u32 D_003E9CC0[];
extern u32 D_003E9CD0[];

EffQuadWork *func_002EA120(FileJobPayload *job) {
    EffQuadWork *work = sdfAllocSizeClassBlock(sizeof(EffQuadWork));
    void *buffer;

    memset(work, 0, sizeof(EffQuadWork));
    VU0_STORE_VF(vf0, work->position);
    VU0_STORE_VF(vf0, work->orientation);
    work->scale = 1.0f;
    work->color = 0x80808080;
    work->billHandle = NULL;
    work->reference = NULL;
    work->assetHandle = (u32)sdfCreateAssetWithDrawEntries();
    func_003332D0((SdfAsset *)work->assetHandle, 1.0f);
    memset(&D_00458370, 0, sizeof(EffPacketParams));
    D_00458370.primitive = 0x4000;
    D_00458370.parameters = D_003E9CC0;
    D_00458370.colors = D_003E9CD0;
    D_00458370.parameterCount = 2;
    D_00458370.vertexCount = 4;
    if (job == NULL) {
        return work;
    }
    work->sourceKind = job->option;
    buffer = fileResolvePrimaryBuffer(job);
    memcpy(&work->source, buffer, sizeof(work->source));
    buffer = fileResolveSecondaryBuffer(job);
    if (buffer != NULL) {
        switch (job->primary.selector) {
        case 1:
            work->billHandle = billCreateIndexed(0, (u32)buffer);
            break;
        case 2:
            work->billHandle = billCreateIndexed(1, (u32)buffer);
            break;
        case 4:
            work->billHandle = effCreateBillboardSharingIndexedResource(*(s32 *)buffer);
            break;
        case 7:
            work->reference = func_002DDF48((u32)buffer);
            break;
        }
        if (work->billHandle != NULL) {
            billMarkKindOneFlag(work->billHandle);
            billSetBillboardMode(work->billHandle, (s16)work->source.alphaTrack.surfaceIndex);
        }
    }
    return work;
}

void effReleaseRenderResources(EffQuadWork *work) {
    if (work->billHandle != 0) {
        billDispatchByKind(work->billHandle);
    }
    if (work->reference != NULL) {
        effReleaseReferenceHolder(work->reference);
    }
    if (work->assetHandle != 0) {
        sdfQueueAssetRelease(work->assetHandle);
    }
    sdfReleaseChipBlock(work);
}

EffQuadWork *effCloneRenderResourceWork(const EffQuadWork *source) {
    EffQuadWork *effect = func_002EA120(NULL);
    memcpy(&effect->source, &source->source, sizeof(effect->source));
    effDuplicateRenderResourceOwner(effect, source);
    return effect;
}

void effDuplicateRenderResourceOwner(EffQuadWork *work, const EffQuadWork *source) {
    if (source->billHandle != 0) {
        if (work->billHandle != 0) {
            billDispatchByKind(work->billHandle);
        }
        work->billHandle = billCloneObjectRetainingSharedData(source->billHandle);
        billMarkKindOneFlag(work->billHandle);
        billSetBillboardMode(work->billHandle, (s16)work->source.alphaTrack.surfaceIndex);
    } else {
        if (work->reference != NULL) {
            effReleaseReferenceHolder(work->reference);
        }
        work->reference = effReferenceObjectRetain(source->reference);
    }
}

void effResetRenderResourceKind(EffQuadWork *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EA5D8);

void effCopyRenderResourcePosition(EffQuadWork *work, const f32 *source) {
    PCP_COPY_VECTOR(work->position, source);
}

void effCopyRenderResourceOrientation(EffQuadWork *work, const f32 *source) {
    PCP_COPY_VECTOR(work->orientation, source);
}

void effSetRenderResourceColor(EffQuadWork *work, u32 color) {
    work->color = color;
}

void effSetRenderResourceMatrixComponent(EffQuadWork *work, f32 value) {
    work->scale = value;
}


extern s32 effMiscRand(s32 *);

void effRandomizeParticleFields(s32 *work) {
    u32 count = ((EffBillTimedHeader *)work[0x34 / 4])->count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 4) {
        entry[1] = -1 - (effMiscRand(effSharedRandomState) & 3);
    }
}



/* The factory appends the copied parameters to its real 0x40-byte header. */
typedef struct EffPointSetClassWork {
    EffClassWork header;
    EffBillPointConfig parameters;
} EffPointSetClassWork;
typedef char EffBillPointConfig_size[(sizeof(EffBillPointConfig) == 0x88) ? 1 : -1];
typedef char EffPointSetClassWork_size[(sizeof(EffPointSetClassWork) == 0xC8) ? 1 : -1];

typedef struct EffPointSetRow {
    EffPointSet *set; /* 0x00 */
    s32 key;          /* 0x04 */
    u32 color;        /* 0x08: packed color written by the class updater */
    f32 angle;        /* 0x0C: phase of the radial class instance */
} EffPointSetRow;

typedef struct EffPointSetTable {
    EffPointSetRow *rows;
} EffPointSetTable;

EffPointSetTable *effCreateAlphaRampPointSetRows(EffBillPointConfig *src) {
    u32 count = src->timed.count;
    EffPointSetTable *table;
    EffPointSetRow *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;

    table = (EffPointSetTable *)sdfAllocSizeClassBlock(count * sizeof(EffPointSetRow) + 4);
    table->rows = (EffPointSetRow *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    row = table->rows;
    for (i = 0; i < count; i++) {
        EffPointSet *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;

            if (j < rampIn) {
                ratio = (f32)j / (f32)rampIn;
            } else if (j <= rampOut) {
                ratio = 1.0f;
            } else {
                ratio = (f32)(n - j) / (f32)(n - rampOut);
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->key = ~(i * 4);
        row->angle = 0.0f;
        row++;
    }
    return table;
}

extern void effReleasePointSetAsset(s32);

void effFreeIndexedEntries(u8 *work) {
    u32 *header = (u32 *)((EffClassWork *)work)->resource;
    u32 count = ((EffBillTimedHeader *)((EffClassWork *)work)->payload)->count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        effReleasePointSetAsset(*entry);
        entry += 4;
    }
    sdfReleaseChipBlock(header);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EB058);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EB728);

extern void effResetDispatchCounter(EffClassWork *);

void effResetBillboardFrameDispatchCounters(s32 *work) {
    u32 count = ((EffBillTimedHeader *)work[0x34 / 4])->count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++) {
        effResetDispatchCounter((EffClassWork *)*entry++);
    }
}

extern EffClassWork *effCreateClassResourceWork(u16, void *);
extern void *memcpy(void *dst, const void *src, u32 size);

u8 *func_002EB968(EffBillPointConfig *config) {
    u32 count = config->timed.count;
    u8 *allocation = sdfAllocSizeClassBlock(count * 4 + 4);
    u32 *entries = (u32 *)(allocation + 4);
    EffBillPointConfig copy;
    u32 index;
    f32 step;
    f32 position;
    f32 offset;

    *(u32 **)allocation = entries;
    if ((u32)config->layers < 3) {
        config->layers = 3;
    }
    if (count == 0) {
        return allocation;
    }
    memcpy(&copy, config, 0x84);
    copy.timed.count = 1;
    copy.unk84 = config->unk84;
    step = 6.2831852f / (f32)count;
    position = step * ((effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f);
    for (index = 0; index < count; index++) {
        EffClassWork *resourceWork = effCreateClassResourceWork(1, &copy);
        u8 **resourceSlot = (u8 **)resourceWork->resource;

        *entries++ = (u32)resourceWork;
        *(f32 *)(*resourceSlot + 0xC) = position;
        offset = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f;
        position += step + step * offset * 0.25f;
    }
    return allocation;
}

extern void effDestroyClassResourceWork(EffClassWork *);

void effReleaseBillFrameEntries(u8 *work) {
    u32 *header = (u32 *)((EffClassWork *)work)->resource;
    u32 count = ((EffBillTimedHeader *)((EffClassWork *)work)->payload)->count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        effDestroyClassResourceWork((EffClassWork *)*entry++);
    }
    sdfReleaseChipBlock(header);
}

extern void effAdvanceClassResourceFrame(EffClassWork *);

void effReleaseTrackEntriesA(u8 *work) {
    u32 count = ((EffBillTimedHeader *)((EffClassWork *)work)->payload)->count;
    u32 **entry = (u32 **)((EffFrameState *)((EffClassWork *)work)->resource)->entries;
    u32 i;

    for (i = 0; i < count; i++) {
        effAdvanceClassResourceFrame((EffClassWork *)*entry++);
    }
}

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void effCopyClassResourcePosition(void *, void *);
extern void effCopyClassResourceOrientation(void *, void *);
extern void effSetClassResourceMatrixComponent(EffClassWork *, f32);
extern void effSetClassResourceColor(s32, u32);
extern void effDrawClassResourceWork(EffClassWork *);

/* vu0 routine: packed color blend and SDK vector copies. */
void effUpdateRadialClassInstances(EffClassWork *work) {
    EffBillRadialConfig *config = work->payload;
    s32 frame = work->frame;
    s32 progress = config->point.timed.time.duration;
    EffClassWork **entries = ((EffClassWorkList *)work->resource)->entries;
    f32 position[4];
    f32 orientation[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 unit;
    u32 second;
    u32 color;
    u32 count;
    u32 i;
    f32 radius;
    f32 scale;

    if (progress < frame && progress != 0) {
        return;
    }
    count = config->point.timed.count;
    radius = config->radius;
    second = effSampleColorAlphaTracks(&config->point.timed.colorTrack, &config->point.timed.alphaTrack, frame, progress);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(blended[0]);
    color = blended[0];
    scale = work->scale;
    PCP_COPY_VECTOR(position, work->vectors.position);
    PCP_COPY_VECTOR(orientation, work->vectors.orientation);
    for (i = 0; i < count; i++, entries++) {
        EffClassWork *entry = *entries;
        EffPointSetTable *points = (EffPointSetTable *)entry->resource;
        f32 angle = points->rows->angle;

        position[0] = work->vectors.position[0] - sdfSinPoly(angle) * radius;
        position[1] = work->vectors.position[1] - sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
        effCopyClassResourcePosition(*entries, position);
        effCopyClassResourceOrientation(*entries, orientation);
        effSetClassResourceMatrixComponent(*entries, scale);
        effSetClassResourceColor((s32)*entries, color);
        effDrawClassResourceWork((EffClassWork *)*entries);
    }
}

typedef struct EffScaleRange {
    u8 *entries;
    f32 start;
    f32 delta;
    struct SdfMemBlock *allocation;
} EffScaleRange;




void effSeedBillScaleRange(u8 *work) {
    EffBillRangeConfig *config = ((EffClassWork *)work)->payload;
    EffScaleRange *range = (EffScaleRange *)((EffClassWork *)work)->resource;
    s32 steps = config->point.timed.time.duration;
    EffScaleRangeEntry *entry = (EffScaleRangeEntry *)range->entries;
    f32 start = config->startBase * (effMiscRandUnitFloat(effSharedRandomState) * config->startRand + (1.0f - config->startRand));
    u32 index;
    u32 count;

    if (steps > 0) {
        f32 end = config->endBase * (effMiscRandUnitFloat(effSharedRandomState) * config->endRand + (1.0f - config->endRand));
        range->start = start;
        range->delta = (end - start) / (f32)steps;
    } else {
        range->start = start;
        range->delta = 0.0f;
    }
    count = config->point.timed.count;
    index = 0;
    if (count != 0) {
        do {
            index++;
            entry->negativeSeed = -1 - (effMiscRand(effSharedRandomState) & 7);
            entry++;
        } while (index < count);
    }
}

EffScaleRange *effCreateRetainedPointSetColorRows(EffBillPointConfig *src) {
    u32 count = src->timed.count;
    struct SdfMemBlock *allocation;
    EffScaleRange *table;
    EffScaleRangeEntry *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;

    allocation = sdfAllocGeneralBlock(count * sizeof(EffScaleRangeEntry) + sizeof(EffScaleRange));
    table = (EffScaleRange *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)allocation));
    table->allocation = allocation;
    table->entries = (u8 *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    row = (EffScaleRangeEntry *)table->entries;
    for (i = 0; i < count; i++) {
        EffPointSet *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;
            if (j < rampIn) {
                ratio = (f32)j / (f32)rampIn;
            } else if (j <= rampOut) {
                ratio = 1.0f;
            } else {
                ratio = (f32)(n - j) / (f32)(n - rampOut);
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->negativeSeed = ~(i * 4);
        row++;
    }
    return table;
}

void effReleaseBillPointEntries(u8 *work) {
    u32 *header = (u32 *)((EffClassWork *)work)->resource;
    u32 count = ((EffBillTimedHeader *)((EffClassWork *)work)->payload)->count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        effReleasePointSetAsset((s32)((EffScaleRangeEntry *)entry)->set);
        entry += 12;
    }
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)((u32)((EffScaleRange *)header)->allocation));
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EC370);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002ECD10);

void effSeedBillboardFrameCounters(s32 *work) {
    u32 count = ((EffBillTimedHeader *)work[0x34 / 4])->count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        entry[1] = -1 - (effMiscRand(effSharedRandomState) & 3);
    }
}

typedef struct EffAlternatingPointSetRow {
    EffPointSet *set;
    s32 key;
    u32 color;
} EffAlternatingPointSetRow;

typedef struct EffAlternatingPointSetTable {
    EffAlternatingPointSetRow *rows;
} EffAlternatingPointSetTable;

EffAlternatingPointSetTable *func_002ECF78(EffBillPointConfig *src) {
    u32 count = src->timed.count;
    EffAlternatingPointSetTable *table;
    EffAlternatingPointSetRow *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;
    f32 previousRatio;

    table = (EffAlternatingPointSetTable *)sdfAllocSizeClassBlock(count * sizeof(EffAlternatingPointSetRow) + 4);
    table->rows = (EffAlternatingPointSetRow *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    if (!(src->layers & 1)) {
        src->layers++;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    previousRatio = 0.0f;
    row = table->rows;
    for (i = 0; i < count; i++) {
        EffPointSet *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;

            if ((j & 1) || j == 0) {
                if (j < rampIn) {
                    ratio = (f32)j / (f32)rampIn;
                } else if (j <= rampOut) {
                    ratio = 1.0f;
                } else {
                    ratio = (f32)(n - j - 1) / (f32)(n - rampOut);
                }
                previousRatio = ratio;
            } else {
                ratio = previousRatio;
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->key = ~(i * 4);
        row++;
    }
    return table;
}

void effReleaseBillboardFramePointSets(s32 *work) {
    u32 count = ((EffBillTimedHeader *)work[0x34 / 4])->count;
    s32 *entries = (s32 *)work[0x30 / 4];
    s32 *entry = (s32 *)*entries;
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        effReleasePointSetAsset(entry[0]);
    }
    sdfReleaseChipBlock(entries);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002ED3D0);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EDB10);

EffClassWork *effCreateClassResourceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effClassResourceWorkOperations[kind].payloadSize;
    EffClassWork *effect = sdfAllocSizeClassBlock(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->frame = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->vectors.orientation);
    memcpy(effect->payload, source, size);
    effect->resource = effClassResourceWorkOperations[kind].createResource(source);
    effClassResourceWorkOperations[kind].initialize(effect);
    return effect;
}

void effCreateClassResourceFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer((FileJobPayload *)request);
    effCreateClassResourceWork(((FileJob *)request)->option, source);
}

void effDestroyClassResourceWork(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].destroyResource();
    sdfReleaseChipBlock(work);
}

EffClassWork *effCloneClassResourceWork(EffClassWork *work) {
    return effCreateClassResourceWork((u16)work->kind, work->payload);
}

void effResetDispatchCounter(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].initialize(work);
    work->frame = 0;
}


void effAdvanceClassResourceFrame(EffClassWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        effClassResourceWorkOperations[work->kind].update(work);
        work->frame++;
    }
}

void effDrawClassResourceWork(EffClassWork *work) {
    effClassResourceWorkOperations[work->kind].draw((void *)work);
}

void effUpdateAndDrawClassResource(EffClassWork *work) {
    effAdvanceClassResourceFrame(work);
    effDrawClassResourceWork(work);
}

void effCopyClassResourcePosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyClassResourceOrientation(void *dst, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)dst)->vectors.orientation, src);
}

void effSetClassResourceColor(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void effSetClassResourceMatrixComponent(EffClassWork *work, float value) {
    work->scale = value;
}

extern EffPacketParams D_004583A0[];

extern EffPacketParams D_00458430[];

EffPointSet *effCreatePointSet5(s32 count) {
    s32 rows = count * 5 + 5;
    s32 size = rows * 20;
    struct SdfMemBlock *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x20);
    data = (u8 *)sdfResourceRetainAddress(base);
    set = (EffPointSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(set->handle, 1.0f);
    memset(D_004583A0, 0, 0x2C);
    D_004583A0[0].primitive = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleasePointSetAsset(s32 work) {
    sdfQueueAssetRelease((s32)((EffPointSet *)work)->handle);
    sdfReleaseResourceAllocation(((EffPointSet *)work)->allocation);
}

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix) {
    void *list;
    EffGsPacket *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        if (matrix == NULL) {
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdfConsAppendVuPacket(list, 0);
        sdfConsAppendAssetPacket(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, (u32)packet);
        }
        remaining = set->rows;
        D_004583A0[0].colors = (u32 *)set->tail;
        D_004583A0[0].positions = (u128 *)set->buffer;
        D_004583A0[0].unk08 = set->color;
        D_004583A0[0].parameterCount = 0x10;
        D_004583A0[0].vertexCount = 0xF;
        D_004583A0[0].parameters = D_003E9D80;
        while (remaining >= 0xF) {
            remaining -= 0xA;
            sdfAppendPacket(list, func_00167A10(D_004583A0));
            D_004583A0[0].positions += 0xA;
            D_004583A0[0].colors += 0xA;
        }
        if (remaining >= 0xA) {
            D_004583A0[0].parameterCount = 8;
            D_004583A0[0].vertexCount = remaining;
            sdfAppendPacket(list, func_00167A10(D_004583A0));
        }
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, (u32)packet);
        }
        D_003E9DC0[set->type]->append((SdfListHead *)D_003E9DC0[set->type], list);
    }
}

EffectStripNode *effCreateStripNode(u32 percent) {
    EffectStripNode *node = sdfAllocAndClearQuadwords(sizeof(EffectStripNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->trackSet = effCreateTrackSetWithSharedReferences(percent * 4, 2, 0);
    node->resource = effCreateBillboardSharingIndexedResource(0);
    node->count = 1;
    return node;
}

EffectStripNode *effCreateStripNodeFromGrid(u8 *source) {
    return effCreateStripNode(effSlotCount(source, 100));
}

extern void effReplaceFileResourceRef(EffectStripNode *, u32, void *);

EffectStripNode *effFileResourceReferenceReplace(FileJob *work) {
    u8 *primary = fileResolvePrimaryBuffer((FileJobPayload *)work);
    u8 *source = primary + 0x20;
    EffectStripNode *node = effCreateStripNodeFromGrid(source);

    memcpy(node->copiedHeader, primary, sizeof(node->copiedHeader));
    effReplaceFileResourceRef(node, work->option, source);
    return node;
}

void effReleaseModelResources(EffectStripNode *p) {
    if (p->resource != 0) {
        billDispatchByKind(p->resource);
    }
    if (p->trackSet != NULL) {
        effReleaseResourceRefs(p->trackSet);
    }
    if (p->active != 0) {
        fileReleaseGridRecordHandle(p->active);
    }
    sdfReleaseChipBlock(p);
}

EffectStripNode *effCloneStripResourceFromOwner(EffectStripNode *work) {
    u8 *source = work->active->data1;
    EffectStripNode *node = effCreateStripNodeFromGrid(source);

    memcpy(node->copiedHeader, source, sizeof(node->copiedHeader));
    effReplaceFileResourceRef(node, work->active->type, source);
    return node;
}

void effReplaceFileResourceRef(EffectStripNode *obj, u32 id, void *arg) {
    if (obj->active != 0) {
        fileReleaseGridRecordHandle(obj->active);
    }
    obj->active = fileAllocateGridRecordSlots((u16)id, obj->percent, arg);
}

void effClearStripRecordReferences(EffectStripNode *node) {
    if (node->active != 0) {
        fileClearRecordReferences(node->active);
        return;
    }
}

void effAcquireStripRecord(EffectStripNode *node) {
    if (effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) {
        return;
    }
    if (node->active != 0) {
        fileAcquireRecord(node->active);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EE670);

void func_002EED48(EffectStripNode *node) {
    effAcquireStripRecord(node);
    func_002EE670(node);
}

void effSetStripRecordVector(EffectStripNode *node) {
    mnuRecordSetVector(node->active);
}

void effSetStripRecordSecondaryVector(EffectStripNode *node) {
    fileSetRecordSecondVector(node->active);
}

void effSetStripRecordColor(EffectStripNode *node, u32 color) {
    node->color = color;
}

void func_002EEDA8(EffectStripNode *p, f32 value) {
    p->opacity = value;
    dds3DispatchIndexedCallback((s32)p->active, value);
}

typedef struct EffRibbonWork {
    u32 count;          // 0x00
    u32 field_04;       // 0x04
    u32 color;          // 0x08
    s32 rowStride;      // 0x0C
    s32 repeat;         // 0x10
    u8 field_14;        // 0x14
    u8 pad_15[3];
    RefObj *resource; // 0x18: null selects the globally shared wind texture
    u32 *colors;        // 0x1C
    u8 *positions;      // 0x20
    u8 *uvs;            // 0x24
    u8 *extra;          // 0x28
    SdfAsset *handle;     // 0x2C
    struct SdfMemBlock *allocation; // 0x30
} EffRibbonWork;

void effResetBillTable(u8 *p) {
    u8 *a = (u8 *)((EffClassWork *)p)->resource;
    u8 *b = ((EffClassWork *)p)->payload;
    u32 n = ((EffBillEmitterCommon *)b)->header.timed.count;
    u32 *counts = (u32 *)((EffRibbonWork *)((EffFrameState *)a)->asset)->colors;
    EffBillEmitterEntry *rec = (EffBillEmitterEntry *)((EffFrameState *)a)->entries;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        rec->timer = -1;
        rec++;
    }
}

u8 *effAllocateRingFadeEntries(EffBillVortexConfig *config) {
    struct SdfMemBlock *base = sdfAllocGeneralBlock(config->common.header.timed.count * sizeof(EffBillEmitterEntry) + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
void effFillRingFadeGradient(u8 *node, EffBillEmitterCommon *config) {
    EffBillEmitterCommon *cfg = config;
    EffRibbonWork *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffRibbonWork *)((EffFrameState *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u32 *effCreateRingFadeTable(EffBillVortexConfig *p, u32 a1) {
    u32 *buf = (u32 *)effAllocateRingFadeEntries(p);
    buf[1] = effCreateRibbonWithSharedResource(p->common.header.timed.count, p->common.header.segments, a1);
    effFillRingFadeGradient(buf, &p->common);
    return buf;
}

u32 *effAssetPointerSet(u8 *p) {
    EffBillVortexConfig *dst = ((EffClassWork *)p)->payload;
    u32 *src = (u32 *)((EffClassWork *)p)->resource;
    u32 *buf = (u32 *)effAllocateRingFadeEntries(dst);
    buf[1] = effCloneRibbonWithSharedResource((u32 *)((EffFrameState *)src)->asset);
    effFillRingFadeGradient(buf, &dst->common);
    return buf;
}

void effReleaseRingFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EF1C0);

void effBlendBillboardInstanceColorsAndTransforms(u8 *work) {
    EffBillVortexConfig *config = (EffBillVortexConfig *)((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = ((BillCellDrawWork *)work)->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->field_08 = blended[0];
    ((EffBillOutput *)out)->color = config->common.header.timed.alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = config->common.drawMode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002F1888(out, mtx);
}

void effResetBillboardFrameInstanceCounters(u8 *p) {
    u8 *a = (u8 *)((EffClassWork *)p)->resource;
    u8 *b = ((EffClassWork *)p)->payload;
    u32 n = ((EffBillEmitterCommon *)b)->header.timed.count;
    u32 *counts = (u32 *)((EffRibbonWork *)((EffFrameState *)a)->asset)->colors;
    EffBillEmitterEntry *rec = (EffBillEmitterEntry *)((EffFrameState *)a)->entries;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        rec->timer = -1;
        rec++;
    }
}

u8 *effAllocateBillFadeFrameEntries(EffBillColumnConfig *config) {
    struct SdfMemBlock *base = sdfAllocGeneralBlock(config->common.header.timed.count * sizeof(EffBillEmitterEntry) + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

void effFillBillFadeGradient(u8 *node, EffBillEmitterCommon *config) {
    EffBillEmitterCommon *cfg = config;
    EffRibbonWork *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffRibbonWork *)((EffFrameState *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u32 *effCreateBillFadeTable(EffBillColumnConfig *p, u32 a1) {
    u32 *buf = (u32 *)effAllocateBillFadeFrameEntries(p);
    buf[1] = effCreateRibbonWithSharedResource(p->common.header.timed.count, p->common.header.segments, a1);
    effFillBillFadeGradient(buf, &p->common);
    return buf;
}

u32 *effCloneBillFadeTable(u8 *p) {
    EffBillColumnConfig *dst = ((EffClassWork *)p)->payload;
    u32 *src = (u32 *)((EffClassWork *)p)->resource;
    u32 *buf = (u32 *)effAllocateBillFadeFrameEntries(dst);
    buf[1] = effCloneRibbonWithSharedResource((u32 *)((EffFrameState *)src)->asset);
    effFillBillFadeGradient(buf, &dst->common);
    return buf;
}

/* Release the bill-fade frame state's shared asset and backing allocation. */
void effReleaseBillFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EFE20);

void effBillBlendCellColorAndUpdateTransform(BillCellDrawWork *work) {
    EffBillColumnConfig *config = (EffBillColumnConfig *)work->config;
    u32 limit = work->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->field_08 = blended[0];
    ((EffBillOutput *)out)->color = config->common.header.timed.alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = config->common.drawMode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002F1888(out, mtx);
}

void effResetParticleBillFrameCounters(u8 *p) {
    u8 *a = (u8 *)((EffClassWork *)p)->resource;
    u8 *b = ((EffClassWork *)p)->payload;
    u32 n = ((EffBillEmitterCommon *)b)->header.timed.count;
    u32 *counts = (u32 *)((EffRibbonWork *)((EffFrameState *)a)->asset)->colors;
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x2C;
    }
}

u8 *effAllocateCompactRingFadeEntries(EffBillSpiralConfig *config) {
    struct SdfMemBlock *base = sdfAllocGeneralBlock(config->common.header.timed.count * 0x2C + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

void effFillCompactRingFadeGradient(u8 *node, EffBillEmitterCommon *config) {
    EffBillEmitterCommon *cfg = config;
    EffRibbonWork *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffRibbonWork *)((EffFrameState *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u32 *effCreateCompactRingFadeTable(EffBillSpiralConfig *p, u32 a1) {
    u32 *buf = (u32 *)effAllocateCompactRingFadeEntries(p);
    buf[1] = effCreateRibbonWithSharedResource(p->common.header.timed.count, p->common.header.segments, a1);
    effFillCompactRingFadeGradient(buf, &p->common);
    return buf;
}

u32 *effCloneBillboardFrameAsset(u8 *p) {
    EffBillSpiralConfig *dst = ((EffClassWork *)p)->payload;
    u32 *src = (u32 *)((EffClassWork *)p)->resource;
    u32 *buf = (u32 *)effAllocateCompactRingFadeEntries(dst);
    buf[1] = effCloneRibbonWithSharedResource((u32 *)((EffFrameState *)src)->asset);
    effFillCompactRingFadeGradient(buf, &dst->common);
    return buf;
}

void effReleaseCompactRingFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffClassWork *)work)->resource;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F0A98);

void effUpdateCompactRingDrawColorAndTransform(u8 *work) {
    EffBillSpiralConfig *config = (EffBillSpiralConfig *)((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = ((BillCellDrawWork *)work)->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = ((BillCellDrawWork *)work)->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffBillOutput *)out)->field_08 = blended[0];
    ((EffBillOutput *)out)->color = config->common.header.timed.alphaTrack.surfaceIndex;
    ((EffBillOutput *)out)->mode = config->common.drawMode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002F1888(out, mtx);
}

EffClassWork *effAllocateBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effBlockResourceOperations[kind].payloadSize;
    EffClassWork *effect = sdfAllocSizeClassBlock(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind = kind;
    effect->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, (u8 *)effect + 0x10);
    memcpy(effect->payload, source, size);
    return effect;
}

extern EffClassWork *effAllocateBlock(u16, void *);

EffClassWork *effCreateResourceInstanceB(u16 kind, void *source, u32 extra) {
    EffClassWork *work = effAllocateBlock(kind, source);
    work->resource = (u32)effBlockResourceOperations[kind].createResource(source, extra);
    effBlockResourceOperations[kind].initialize(work);
    return work;
}

extern EffClassWork *effCreateResourceInstanceB(u16, void *, u32);

u8 *effCreateFileResourceInstanceB(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer((FileJobPayload *)work);
    void *source;
    switch (((FileJob *)work)->slots[0].selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer((FileJobPayload *)work);
    return (u8 *)effCreateResourceInstanceB(((FileJob *)work)->option, source, (u32)secondary);
}

void effDestroyBlockResourceWork(EffClassWork *obj) {
    effBlockResourceOperations[obj->kind].destroyResource();
    sdfReleaseChipBlock(obj);
}

EffClassWork *effDuplicateActiveResourceB(EffClassWork *obj) {
    EffClassWork *work = effAllocateBlock((u16)obj->kind, obj->payload);
    void *resource = effBlockResourceOperations[obj->kind].cloneResource(obj);
    s32 kind = obj->kind;

    work->resource = (u32)resource;
    effBlockResourceOperations[kind].initialize(work);
    return work;
}

void effResetBlockResourceFrame(EffClassWork *work) {
    effBlockResourceOperations[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceBlockResourceFrame(EffClassWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        effBlockResourceOperations[work->kind].update();
        work->frame++;
    }
}

void effDrawBlockResourceWork(EffClassWork *work) {
    effBlockResourceOperations[work->kind].draw((void *)work);
}

void effUpdateAndDrawBlockResource(EffClassWork *work) {
    effAdvanceBlockResourceFrame(work);
    effDrawBlockResourceWork(work);
}

void effCopyBlockResourcePosition(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyBlockResourceOrientation(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetBlockResourceColor(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void effSetBlockResourceMatrixComponent(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

extern EffPacketParams D_004584C0;

extern EffPacketParams D_00458400;



extern EffPacketParams D_004583D0;

u8 *effCreateRibbonWork(u32 count, u32 repeat) {
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x1C + 4) * count;
    u32 cells = rowStride * count;
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(size + 0x34);
    u8 *p = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)allocation));
    EffRibbonWork *work = (EffRibbonWork *)(p + size);
    u32 i;

    work->positions = p;
    p += cells * 16;
    work->uvs = p;
    p += cells * 8;
    work->extra = p;
    p += cells * 4;
    work->field_04 = 2;
    work->color = 0x80808080;
    work->rowStride = rowStride;
    work->repeat = repeat;
    work->allocation = allocation;
    work->colors = (u32 *)p;
    work->count = count;
    work->field_14 = 0;
    for (i = 0; i < count; i++) {
        ((u32 *)p)[i] = 0x80808080;
    }
    work->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(work->handle, 1.0f);
    memset(&D_004583D0, 0, sizeof(EffPacketParams));
    D_004583D0.primitive = 0x4000;
    return (u8 *)work;
}

u32 effCreateRibbonWithSharedResource(u32 count, u32 repeat, u32 resource) {
    u8 *node = effCreateRibbonWork(count, repeat);

    if (resource == 0) {
        s32 references = effSharedRibbonReferenceCount;
        ((EffRibbonWork *)node)->resource = NULL;
        if (references == 0) {
            D_00437E78 = (u32)effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)effWindTextureHandle, 0x300);
            references = effSharedRibbonReferenceCount;
        }
        references++;
        effSharedRibbonReferenceCount = references;
    } else {
        ((EffRibbonWork *)node)->resource = effCreateSharedTextureReference((struct SdfTextureFileHeader *)resource);
    }
    return (u32)node;
}

void effSharedAssetReferenceRelease(s32 work) {
    if (((EffRibbonWork *)work)->resource == NULL) {
        effSharedRibbonReferenceCount = effSharedRibbonReferenceCount - 1;
        if (effSharedRibbonReferenceCount == 0) {
            effReleaseSharedReference((RefObj *)D_00437E78);
            D_00437E78 = 0;
        }
    }
    else {
        effReleaseSharedReference(((EffRibbonWork *)work)->resource);
    }
    sdfQueueAssetRelease((s32)((EffRibbonWork *)work)->handle);
    sdfReleaseResourceAllocation(((EffRibbonWork *)work)->allocation);
}

u8 *effCloneRibbonWithSharedResource(u32 *source) {
    u8 *node = effCreateRibbonWork(((EffRibbonWork *)source)->count, ((EffRibbonWork *)source)->repeat);

    if (((EffRibbonWork *)source)->resource != NULL) {
        ((EffRibbonWork *)node)->resource = effRetainSharedReference(((EffRibbonWork *)source)->resource);
    } else {
        effSharedRibbonReferenceCount++;
        ((EffRibbonWork *)node)->resource = NULL;
    }
    return node;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F1888);

void effLoadWindTexture(void) {
    D_00437E6C = (u32)sdfReadNamedResource("/effect/wind00.tmx", &effWindTextureHandle, 0);
}

u32 effGetWindTextureHandle(void) {
    return effWindTextureHandle;
}

void effResetAnimationFrameEntries(u8 *p) {
    u8 *a = (u8 *)((EffClassWork *)p)->resource;
    u8 *b = ((EffClassWork *)p)->payload;
    u32 n = ((EffBillFlameConfig *)b)->header.timed.count;
    u32 *counts = ((EffStripWork *)((EffFrameState *)a)->asset)->colors;
    EffBillEmitterEntry *rec = (EffBillEmitterEntry *)((EffFrameState *)a)->entries;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        rec->timer = -1;
        rec++;
    }
}

u8 *effAllocateAnimationBuffer(EffBillFlameConfig *config) {
    struct SdfMemBlock *base = sdfAllocGeneralBlock(config->header.timed.count * sizeof(EffBillEmitterEntry) + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    u32 count = config->header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->header.segments = 3;
    }
    return (u8 *)node;
}

void effFillFadeColorRows(u8 *work, u8 *config) {
    EffBillFlameConfig *cfg = (EffBillFlameConfig *)config;
    u32 rows = cfg->header.timed.count;
    u32 i;

    if (rows != 0) {
        s32 width = cfg->header.segments;
        f32 fw = width;
        s32 fadeInEnd = cfg->header.fadeIn * fw;
        s32 fadeOutStart = cfg->header.fadeOut * fw;
        u32 cols = width + 1;
        u32 stride = cols * 4;
        u32 *color = ((EffFrameAsset *)((EffFrameState *)work)->asset)->colorRows;
        u32 *first = color;

        for (i = 0; i < cols; i++) {
            f32 t;
            u32 alpha;

            if (i < fadeInEnd) {
                t = (f32)i / (f32)fadeInEnd;
            } else if (fadeOutStart < i) {
                t = (f32)(width - i) / (f32)(width - fadeOutStart);
            } else {
                t = 1.0f;
            }
            alpha = (u32)(t * 128.0f) << 24;
            color[0] = 0x808080;
            color[1] = alpha | 0x808080;
            color[2] = alpha | 0x808080;
            color[3] = 0x808080;
            color += 4;
        }
        for (i = 1; i < rows; i++) {
            memcpy(color, first, stride * 4);
            color += stride;
        }
    }
}

u32 *effPrepareTextureAnimation(EffBillFlameConfig *src) {
    u32 *buf = (u32 *)effAllocateAnimationBuffer(src);
    buf[1] = effCreateTexturedStripWithSharedTexture(src->header.timed.count, src->header.segments);
    effFillFadeColorRows(buf, src);
    return buf;
}

u32 *effPrepareOwnedTextureAnimation(u8 *p) {
    u8 *dst = ((EffClassWork *)p)->payload;
    u32 *src = (u32 *)((EffClassWork *)p)->resource;
    u32 *buf = (u32 *)effAllocateAnimationBuffer(dst);
    buf[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)src)->asset);
    effFillFadeColorRows(buf, dst);
    return buf;
}

void effReleaseTextureAnimationWork(s32 work) {
    s32 state;

    state = (s32)((EffClassWork *)work)->resource;
    effReleaseScalyStripResources((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F2050);

typedef struct EffMeshOutput {
    u32 field_00;       // 0x00
    u32 textureId;      // 0x04
    u32 color;          // 0x08
    u8 pad_0C[8];
    u8 mode;            // 0x14
} EffMeshOutput;

void func_002F2760(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillFlameConfig *)config)->header.timed.time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillFlameConfig *)config)->header.timed.colorTrack, &((EffBillFlameConfig *)config)->header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    ((EffMeshOutput *)out)->color = blended[0];
    ((EffMeshOutput *)out)->textureId = ((EffBillFlameConfig *)config)->header.timed.alphaTrack.surfaceIndex;
    ((EffMeshOutput *)out)->mode = ((EffBillFlameConfig *)config)->meshMode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002F3F70(out, mtx);
}

/* Per-animation allocation: positions follow this four-word owner header. */
typedef struct EffAnimationState {
    f32 *positions;       // 0x00
    u32 textureHandle;    // 0x04
    FileSlotTable *record; // 0x08
    struct SdfMemBlock *allocation;       // 0x0C
} EffAnimationState;

void effInitializeAnimationPositions(u8 *work) {
    EffAnimationState *state = (EffAnimationState *)((EffClassWork *)work)->resource;
    FileSlotTable *record = state->record;
    float *positions = state->positions;
    u32 count = record->count;
    u32 i = 0;

    fileClearRecordReferences(record);
    for (; i < count; i++) {
        positions[0] = effMiscRandUnitFloat(effSharedRandomState);
        positions[1] = effMiscRandUnitFloat(effSharedRandomState);
        positions += 2;
    }
}

u32 effClampSlotCount(u8 *p) {
    return effSlotCount(p, 200);
}

EffAnimationState *effCreateAnimationState(u32 unused, u32 count) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(count * 8 + 0x10);
    EffAnimationState *state = (EffAnimationState *)sdfResourceRetainAddress(allocation);

    state->allocation = allocation;
    state->positions = (f32 *)(state + 1);
    state->record = 0;
    state->textureHandle = effRetainScalyTextureReference();
    return state;
}

EffAnimationState *effActivateAnimationState(s32 work) {
    EffAnimationState *owner = (EffAnimationState *)((EffClassWork *)work)->resource;
    FileSlotTable *resource = owner->record;
    EffAnimationState *state = effCreateAnimationState((u32)((EffClassWork *)work)->payload, resource->count);

    resource = owner->record;
    state->record = fileAllocateGridRecordSlots(resource->type, resource->count,
                               resource->data1);
    return state;
}


extern void effReleaseScalyTextureReference(u32);

void effReleaseAnimationFrameResources(u8 *work) {
    EffAnimationState *state = (EffAnimationState *)((EffClassWork *)work)->resource;

    effReleaseScalyTextureReference(state->textureHandle);
    if (state->record != 0) {
        fileReleaseGridRecordHandle(state->record);
    }
    sdfReleaseResourceAllocation(state->allocation);
}

void effSynchronizeFileTransform(u8 *work) {
    EffAnimationState *state = (EffAnimationState *)((EffClassWork *)work)->resource;

    if (state->record != 0) {
        mnuRecordSetVector(state->record, work);
        fileSetRecordSecondVector(state->record, work + 0x10);
        dds3DispatchIndexedCallback((s32)state->record, ((EffClassWork *)work)->scale);
        fileAcquireRecord(state->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F2AE8);

EffAnimationState *effCreatePrimarySlotAnimationState(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = effClampSlotCount(mapping);
    EffAnimationState *state = effCreateAnimationState((u32)work, count);

    state->record = fileAllocateGridRecordSlots(1, count, mapping);
    return state;
}

EffAnimationState *effCreateAlternateSlotAnimationState(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = effClampSlotCount(mapping);
    EffAnimationState *state = effCreateAnimationState((u32)work, count);

    state->record = fileAllocateGridRecordSlots(3, count, mapping);
    return state;
}

void effResetSlotAnimationRecord(s32 work) {
    ((EffFrameAsset *)((EffFrameState *)((EffClassWork *)work)->resource)->asset)->frameCount = 0;
}


u32 *effAllocateQuantizedBuffer(EffBillQuantizedConfig *work) {
    void *allocation = sdfAllocGeneralBlock(0xC);
    u32 *buffer = (u32 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)allocation));
    u32 count = work->samples.quantizedSamples;

    buffer[2] = (u32)allocation;
    if (count < 4) {
        work->samples.quantizedSamples = 4;
        count = 4;
    }
    buffer[0] = count >> 2;
    if ((work->samples.quantizedSamples & 3) != 0) {
        buffer[0] = (count >> 2) + 1;
    }
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F3258);

extern void func_002F3258(u32 *, EffBillQuantizedConfig *);

u32 *effPrepareQuantizedTexture(EffBillQuantizedConfig *src) {
    u32 *buf = effAllocateQuantizedBuffer(src);
    buf[1] = effCreateTexturedStripWithSharedTexture(buf[0], src->samples.quantizedSamples);
    func_002F3258(buf, src);
    return buf;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *p) {
    EffBillQuantizedConfig *config = (EffBillQuantizedConfig *)((EffClassWork *)p)->payload;
    u32 *src = (u32 *)((EffClassWork *)p)->resource;
    u32 *buf = effAllocateQuantizedBuffer(config);
    buf[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)src)->asset);
    func_002F3258(buf, config);
    return buf;
}

void effReleaseBillboardFrameAsset(s32 work) {
    s32 state;

    state = (s32)((EffClassWork *)work)->resource;
    effReleaseScalyStripResources((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}


void effOffsetNodeRowsVU(u8 *work) {
    s32 *list = (s32 *)((EffClassWork *)work)->resource;
    u8 *config = ((EffClassWork *)work)->payload;
    s32 rows = ((EffBillQuantizedConfig *)config)->samples.signedRows + 1;
    s32 count = list[0];
    u8 *node = *(u8 **)&list[1];
    u8 *entry = ((EffStripWork *)node)->uvsB;
    s32 i;
    s32 j;
    s32 k;
    f32 *v;

    for (i = 0; i < count; i++) {
        for (j = 0; j < rows; j++) {
            v = (f32 *)entry + 1;
            for (k = 0; k < 4; k++) {
                *v += ((EffBillQuantizedConfig *)config)->rowOffset;
                v += 2;
            }
            entry += 0x20;
        }
    }
}

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateFadedMeshTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillQuantizedConfig *)config)->drawProgress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;
    f32 scale;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effSampleColorAlphaTracks(&((EffBillQuantizedConfig *)config)->colorTrack, &((EffBillQuantizedConfig *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    ((EffMeshOutput *)out)->color = blended[0];
    ((EffMeshOutput *)out)->textureId = ((EffBillQuantizedConfig *)config)->alphaTrack.surfaceIndex;
    ((EffMeshOutput *)out)->mode = ((EffBillQuantizedConfig *)config)->meshMode;
    scale = effSampleScalarCurve(&((EffBillQuantizedConfig *)config)->scaleCurve, limit, progress) * work->scale;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002F3F70(out, mtx);
}

EffClassWork *effAllocateBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effModelBlockOperations[kind].payloadSize;
    EffClassWork *effect = sdfAllocSizeClassBlock(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind = kind;
    effect->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, effect->vectors.orientation);
    memcpy(effect->payload, source, size);
    return effect;
}

EffClassWork *effCreateResourceInstanceC(u16 kind, void *source) {
    EffClassWork *work = effAllocateBlockWithModel(kind, source);
    work->resource = (u32)effModelBlockOperations[kind].createResource(source);
    effModelBlockOperations[kind].initialize(work);
    return work;
}

void effResourceInstanceCreateFromFile(s32 work) {
    void *source;

    source = fileResolvePrimaryBuffer((FileJobPayload *)work);
    effCreateResourceInstanceC(((FileJob *)work)->option, source);
}

void effDestroyModelBlockWork(EffClassWork *work) {
    effModelBlockOperations[work->kind].destroyResource();
    sdfReleaseChipBlock(work);
}

EffClassWork *effRecreateActiveByClass(EffClassWork *obj) {
    EffClassWork *work = effAllocateBlockWithModel((u16)obj->kind, obj->payload);
    void *resource = effModelBlockOperations[obj->kind].cloneResource(obj);
    s32 kind = obj->kind;

    work->resource = (u32)resource;
    effModelBlockOperations[kind].initialize(work);
    return work;
}

void effResetModelBlockFrame(EffClassWork *work) {
    effModelBlockOperations[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceModelBlockFrame(EffClassWork *work) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        effModelBlockOperations[work->kind].update();
        work->frame++;
    }
}

void effDrawModelBlock(EffClassWork *work) {
    effModelBlockOperations[work->kind].draw((void *)work);
}

void effUpdateAndDrawModelBlock(EffClassWork *work) {
    effAdvanceModelBlockFrame(work);
    effDrawModelBlock(work);
}

void effCopyModelBlockPosition(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyModelBlockOrientation(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetModelBlockColor(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void effSetModelBlockMatrixComponent(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

/* Retain and return the lazy shared reference to /effect/scaly00.tmx. */
u32 effRetainScalyTextureReference(void) {
    if (effSharedStripReferenceCount == 0) {
        effSharedScalyStripResource = (u32)effCloneSharedReferenceWithValue((struct SdfTextureFileHeader *)effScalyTextureHandle, 0x200);
    }
    effSharedStripReferenceCount = effSharedStripReferenceCount + 1;
    return effSharedScalyStripResource;
}

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(u32 unused) {
    effSharedStripReferenceCount = effSharedStripReferenceCount - 1;
    if (effSharedStripReferenceCount == 0) {
        effReleaseSharedReference((RefObj *)effSharedScalyStripResource);
        effSharedScalyStripResource = 0;
    }
}

/* Allocate position/UV/color rows and a draw handle using count and repeat. */
u8 *effAllocateTexturedStripWork(count, repeat)
u32 count;
u32 repeat;
{
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x24 + 4) * count;
    u32 cells = rowStride * count;
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(size + 0x34);
    u8 *p = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)allocation));
    EffStripWork *work = (EffStripWork *)(p + size);
    u32 i;

    work->positions = p;
    p += cells * 16;
    work->uvsA = p;
    p += cells * 8;
    work->uvsB = p;
    p += cells * 8;
    work->extra = p;
    p += cells * 4;
    work->type = 2;
    work->color = 0x80808080;
    work->rowStride = rowStride;
    work->repeat = repeat;
    work->allocation = allocation;
    work->colors = (u32 *)p;
    work->count = count;
    work->flag = 0;
    for (i = 0; i < count; i++) {
        ((u32 *)p)[i] = 0x80808080;
    }
    work->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(work->handle, 1.0f);
    memset(&D_00458400, 0, sizeof(EffPacketParams));
    D_00458400.primitive = 0x4000;
    return (u8 *)work;
}

/* Create a scaly strip and retain its texture; preserve the argument-less allocator call. */
u64 effCreateTexturedStripWithSharedTexture(void) {
    u64 strip;

    strip = effAllocateTexturedStripWork();
    effRetainScalyTextureReference();
    return strip;
}

void effReleaseScalyStripResources(u8 *work) {
    effReleaseScalyTextureReference(effSharedScalyStripResource);
    sdfQueueAssetRelease((s32)((EffStripWork *)work)->handle);
    sdfReleaseResourceAllocation(((EffStripWork *)work)->allocation);
}

/* Recreate the source strip's dimensions and retain another shared texture reference. */
void effAllocateStripFromWorkAndRetainTexture(u8 *work) {
    effAllocateTexturedStripWork(((EffStripWork *)work)->count, ((EffStripWork *)work)->repeat);
    effSharedStripReferenceCount++;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F3F70);

void effLoadScalyTexture(void) {
    D_00437E7C = (u32)sdfReadNamedResource("/effect/scaly00.tmx", &effScalyTextureHandle, 0);
}

u32 effGetScalyTextureHandle(void) {
    return effScalyTextureHandle;
}

typedef struct EffSpanEntry {
    f32 first;
    f32 second;
    u32 referenceAge;
} EffSpanEntry;

typedef struct EffSpanRecord {
    EffSpanEntry *entries;
    EffPointSet *pointSet;
    EffTrackSet *references;
    u32 pointCount;
} EffSpanRecord;

typedef struct EffSpanTable {
    EffSpanRecord *records;
    u32 count;
    u16 total;
    u8 pad0A[2];
    struct SdfMemBlock *allocation;
} EffSpanTable;

typedef struct EffSpanConfig {
    SdfColorTrack pointColorTrack;
    SdfAlphaTrack pointAlphaTrack;
    u32 progress;
    u8 drawPoints;
    u8 pad39[3];
    u32 middleColor;
    u32 edgeColor;
    u8 pad44[4];
    f32 drawScale;
    u8 pad4C[4];
    SdfColorTrack referenceColorTrack;
    SdfAlphaTrack referenceAlphaTrack;
    s32 referenceLifetime;
    u8 drawReferences;
    u8 allowMultipleReferenceStarts;
    u8 pad8A[2];
    s32 geometryStartUpdateCount;
    u32 referenceColorA;
    u32 referenceColorB;
    f32 positionScaleA;
    f32 positionScaleB;
    f32 firstRand;
    f32 secondBase;
    f32 rangeRand;
    s32 referenceRampDuration;
    u32 perSpan;
    f32 rotationAngularVelocity;
    f32 rotationAngularAcceleration;
    u8 pointSetFlag;
} EffSpanConfig;


void effSeedParticleSpanParameters(EffModelResource *work) {
    u32 index = 0;
    EffSpanTable *table = work->childResource;
    EffSpanConfig *config = work->source;
    u32 total = table->total;
    u32 per = config->perSpan;
    u32 spans = total / per;
    EffSpanRecord *record = table->records;
    u32 span;
    EffSpanEntry *entry;

    if (total % per != 0) {
        spans++;
    }
    if (table->count != 0) {
        do {
            record->pointCount = 0;
            entry = record->entries;
            span = 0;
            if (spans != 0) {
                do {
                    span++;
                    entry->first = effMiscRandUnitFloat(effSharedRandomState) * config->firstRand + (1.0f - config->firstRand);
                    entry->second = config->secondBase * (effMiscRandUnitFloat(effSharedRandomState) * config->rangeRand + (1.0f - config->rangeRand));
                    entry->referenceAge = 0;
                    entry++;
                } while (span < spans);
            }
            index++;
            record++;
        } while (index < table->count);
    }
}

extern EffPointSet *effCreatePointSet3(s32 count);

/* Build the point and reference sets for each model map-position record. */
EffSpanTable *effCreateParticleSpanTable(EffSpanConfig *config, MdlCtx *model) {
    u32 total = model->first->frameCount;
    u32 count = sdfCountMapPositionRecords(model->inner);
    u32 spans;
    u32 partialSpan;
    struct SdfMemBlock *allocation;
    EffSpanTable *table;
    EffSpanRecord *record;
    EffSpanEntry *entries;
    u32 i;
    u32 j;
    u32 triplets;
    u32 *colors;
    u32 middleColor;
    u32 edgeColor;
    EffTrackSet *tracks;

    model->first->frameStep = 1.0f;
    mdlAddEntryPlain(model, 0, 0);
    if (config->perSpan == 0) {
        config->perSpan = 1;
    }
    partialSpan = total % config->perSpan != 0;
    spans = partialSpan + total / config->perSpan;
    if (config->referenceRampDuration == 0) {
        config->referenceRampDuration = 1;
    }
    allocation = sdfAllocGeneralBlock(sizeof(EffSpanTable) +
                 count * sizeof(EffSpanRecord) + count * spans * sizeof(EffSpanEntry));
    table = (EffSpanTable *)sdfResourceRetainAddress(allocation);
    table->allocation = allocation;
    table->records = (EffSpanRecord *)(table + 1);
    entries = (EffSpanEntry *)(table->records + count);
    table->total = total;
    table->count = count;
    for (i = 0, record = table->records; i < count; i++, record++) {
        record->pointSet = effCreatePointSet3(total);
        record->pointSet->type = (u32)config->pointAlphaTrack.surfaceIndex;
        record->pointSet->flag = config->pointSetFlag;
        if (config->drawPoints) {
            triplets = record->pointSet->rows / 3;
            colors = (u32 *)record->pointSet->tail;
            middleColor = config->middleColor;
            edgeColor = config->edgeColor;
            for (j = 0; j < triplets; j++, colors += 3) {
                colors[0] = edgeColor;
                colors[1] = middleColor;
                colors[2] = edgeColor;
            }
        }
        record->entries = entries;
        entries += spans;
        if (config->drawReferences) {
            tracks = effCreateTrackSetWithSharedReferences(spans, 0, 0);
            record->references = tracks;
            tracks->type = (u32)config->referenceAlphaTrack.surfaceIndex;
            tracks->flag = config->pointSetFlag;
        } else {
            record->references = 0;
        }
    }
    return table;
}

extern void effReleaseModelPointSetAsset(EffPointSet *);

void effReleaseParticleList(EffSpanTable *list) {
    EffSpanRecord *entry = list->records;
    u32 i;

    for (i = 0; i < list->count; i++) {
        effReleaseModelPointSetAsset(entry->pointSet);
        if (entry->references != 0) {
            effReleaseResourceRefs(entry->references);
        }
        entry++;
    }
    sdfReleaseResourceAllocation(list->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", effUpdateParticleSpanGeometry);

INCLUDE_ASM(const s32, "game/code_002DE248", effDrawParticleSpanPointsAndReferences);


EffModelResource *effCreateModelResourceWithInlineData(u16 kind, void *source, void *secondary, u32 param) {
    u32 headerSize = 0x40;
    u32 size = effModelResourceOperations[kind].payloadSize;
    EffModelResource *effect = (EffModelResource *)sdfAllocSizeClassBlock(size + headerSize);

    effect->source = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->updateCount = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->position);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->orientation);
    memcpy(effect->source, source, size);
    if (secondary != NULL) {
        effect->model = func_002DC1D0(secondary, param);
        effect->attributes = param;
        effect->childResource = (void *)effModelResourceOperations[kind].createResource(effect->source, effect->model);
        effModelResourceOperations[kind].initialize(effect);
    }
    return effect;
}

u32 effCreateModelResourceFromFile(u8 *work) {
    void *first = fileResolvePrimaryBuffer((FileJobPayload *)work);
    void *second = fileResolveSecondaryBuffer((FileJobPayload *)work);
    return (u32)effCreateModelResourceWithInlineData(((FileJob *)work)->option, first, second, ((FileJob *)work)->slots[1].size);
}

void effDestroyModelResource(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].destroyResource(effect->childResource);
    effDestroyModelContext(effect->model);
    sdfReleaseChipBlock(effect);
}

EffModelResource *effCreateModelResource(EffModelCreateRequest *work) {
    EffModelResource *effect = effCreateModelResourceWithInlineData(work->kind, work->source, 0, 0);
    s32 x = mdlGetContextResourceGroup(work->assetId);
    s32 y = mdlGetContextResourceId(work->assetId);
    MdlCtx *model = func_00232198(x, y);

    effect->model = model;
    effInitModelVUState(model);
    effect->attributes = work->attributes;
    effect->childResource = (void *)effModelResourceOperations[effect->kind].createResource(effect->source, effect->model);
    effModelResourceOperations[effect->kind].initialize(effect);
    return effect;
}

void effResetModelResourceUpdateCount(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].initialize(effect);
    effect->updateCount = 0;
}

void effDispatchModelResourceUpdate(EffModelResource *effect) {
    if ((effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        effModelResourceOperations[effect->kind].update(effect);
        effect->updateCount++;
    }
}

void effDispatchModelResourceCallback(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].draw(effect);
}

void effStepModelResourceCallbacks(EffModelResource *effect) {
    effDispatchModelResourceUpdate(effect);
    effDispatchModelResourceCallback(effect);
}

void effSetModelResourcePrimaryTransformVector(EffModelResource *effect, const f32 *position) {
    PCP_COPY_VECTOR(effect->position, position);
}

void effSetModelResourceSecondaryTransformVector(EffModelResource *effect, const f32 *orientation) {
    PCP_COPY_VECTOR(effect->orientation, orientation);
}

void effSetModelResourceColor(EffModelResource *effect, u32 color) {
    effect->color = color;
}

void effSetModelResourceScale(EffModelResource *effect, f32 scale) {
    effect->scale = scale;
}

EffPointSet *effCreatePointSet3(s32 count) {
    s32 rows = count * 3 + 3;
    s32 size = rows * 20;
    struct SdfMemBlock *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x20);
    data = (u8 *)sdfResourceRetainAddress(base);
    set = (EffPointSet *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfCreateAssetWithDrawEntries();
    func_003332D0(set->handle, 1.0f);
    memset(D_00458430, 0, 0x2C);
    D_00458430[0].primitive = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleaseModelPointSetAsset(EffPointSet *set) {
    sdfQueueAssetRelease((s32)set->handle);
    sdfReleaseResourceAllocation(set->allocation);
}

void effDrawThreePointGroups(EffPointSet *set, Matrix4 *matrix) {
    void *list;
    EffGsPacket *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = (void *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        if (matrix == NULL) {
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdfConsAppendVuPacket(list, 0);
        sdfConsAppendAssetPacket(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, (u32)packet);
        }
        remaining = set->rows;
        D_00458430[0].colors = (u32 *)set->tail;
        D_00458430[0].positions = (u128 *)set->buffer;
        D_00458430[0].unk08 = set->color;
        D_00458430[0].parameterCount = 0x10;
        D_00458430[0].vertexCount = 0xF;
        D_00458430[0].parameters = (u32 *)D_003E9C40;
        while (remaining >= 0xF) {
            remaining -= 0xC;
            sdfAppendPacket(list, func_00167A10(D_00458430));
            D_00458430[0].positions += 0xC;
            D_00458430[0].colors += 0xC;
        }
        if (remaining >= 6) {
            D_00458430[0].parameterCount = 4;
            D_00458430[0].vertexCount = remaining;
            sdfAppendPacket(list, func_00167A10(D_00458430));
        }
        if (set->flag == 0) {
            packet = (EffGsPacket *)sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, (u32)packet);
        }
        D_003E9F38[set->type]->append((SdfListHead *)D_003E9F38[set->type], list);
    }
}

void effGetWorldVector(u32 which) {
    EffectVectorRequest request;
    u128 result;
    u32 handle = effBTLFieldColorGetOriginalSelector();

    request.kind = 0xB;
    request.count = 1;
    request.size = 8;
    request.unk04 = 0;
    switch (which) {
    case 0:
        break;
    case 1:
        handle = effBTLFieldColorGetOriginalSelector();
        request.kind = 0;
        break;
    case 2:
        handle = effBTLFieldColorGetVariantSelector();
        request.kind = 0;
        break;
    case 3:
        request.kind = 1;
        break;
    case 4:
        request.kind = 2;
        break;
    case 5:
        request.kind = 3;
        break;
    case 6:
        handle = effBTLFieldColorGetOverrideSelector();
        request.kind = 6;
        break;
    case 7:
        handle = effBTLFieldColorGetFinalSelector();
        request.kind = 7;
        break;
    }
    if (request.kind != 0xB) {
        u128 *vec = &result;

        effBattleMiscQueryPosition((void *)handle, &request, vec);
        VU0_LOAD_VF_MEMORY(vf10, vec);
    } else {
        VU0_MOVE_VF(vf10, vf0);
    }
}

s32 effResolveRequestPositionIntoVu(EffectVectorRequest *source, s16 index) {
    EffectVectorRequest request;
    u128 result;
    u32 handle = 0;

    if (index < 0) {
        switch (source->kind) {
        case 0:
        case 1:
            handle = effBTLFieldColorGetOriginalSelector();
            request.kind = 0;
            break;
        case 2:
            handle = effBTLFieldColorGetVariantSelector();
            request.kind = 0;
            break;
        case 3:
            handle = effBTLFieldColorGetOriginalSelector();
            request.kind = 1;
            break;
        case 4:
            handle = effBTLFieldColorGetVariantSelector();
            request.kind = 2;
            break;
        case 5:
            return 0;
        case 6:
            handle = effBTLFieldColorGetOverrideSelector();
            request.kind = 6;
            break;
        case 7:
            handle = effBTLFieldColorGetFinalSelector();
            request.kind = 7;
            break;
        }
        request.count = source->count;
        request.size = source->size;
        request.unk04 = source->unk04;
    } else {
        switch (source->kind) {
        case 0:
        case 1:
            handle = effBTLFieldColorGetOriginalSelector();
            break;
        case 2:
            handle = effBTLFieldColorGetVariantSelector();
            break;
        case 3:
        case 4:
        case 5:
            return 0;
        case 6:
            handle = effBTLFieldColorGetOverrideSelector();
            break;
        case 7:
            handle = effBTLFieldColorGetFinalSelector();
            break;
        default:
            return 0;
        }
        request.kind = 8;
        request.count = index;
        request.size = 0;
    }
    if (handle == 0) {
        return 0;
    }
    {
        u128 *vec = &result;

        effBattleMiscQueryPosition((void *)handle, &request, vec);
        VU0_LOAD_VF_MEMORY(vf10, vec);
        return 1;
    }
}



s32 effCollectModelEffectActors(BtlUnit **out, u32 kind) {
    s32 count = 0;
    u32 mask = 0;
    u8 *state = (u8 *)btlGetRuntime();
    u8 *actor = (u8 *)effBTLFieldColorGetOriginalSelector();
    u8 *other = (u8 *)effBTLFieldColorGetVariantSelector();

    switch (kind) {
    case 1:
        if ((((BtlUnit *)actor)->flags & 2) && ((BtlUnit *)actor)->ext != 0) {
            out[0] = (BtlUnit *)actor;
            count = 1;
        }
        break;
    case 4:
        mask = ((BtlUnit *)other)->flags & 0x600;
        break;
    case 3:
        mask = ((BtlUnit *)actor)->flags & 0x600;
        break;
    case 5:
        mask = 0x600;
        break;
    case 6:
        other = (u8 *)effBTLFieldColorGetOverrideSelector();
        if ((((BtlUnit *)other)->flags & 2) && ((BtlUnit *)other)->ext != 0) {
            out[0] = (BtlUnit *)other;
            count = 1;
        }
        break;
    case 7:
        other = (u8 *)effBTLFieldColorGetFinalSelector();
        if ((((BtlUnit *)other)->flags & 2) && ((BtlUnit *)other)->ext != 0) {
            out[0] = (BtlUnit *)other;
            count = 1;
        }
        break;
    case 0:
    case 2:
        if ((((BtlUnit *)other)->flags & 2) && ((BtlUnit *)other)->ext != 0) {
            out[0] = (BtlUnit *)other;
            count = 1;
        }
        break;
    }
    if (mask != 0) {
        u8 *link;

        for (link = (u8 *)((BtlState *)state)->units; link != NULL; link = (u8 *)((BtlUnit *)link)->nextActor) {
            u32 flags = ((BtlUnit *)link)->flags;

            if (flags & 1) {
                if (flags & 2) {
                    if (((BtlUnit *)link)->ext != 0) {
                        if (flags & mask) {
                            out[count++] = (BtlUnit *)link;
                        }
                    }
                }
            }
        }
    }
    return count;
}

extern void evtConfigureUnitTransition(EvtUnit *, s32);

/* Restore actor light endpoints and directions when the field-light effect ends. */
void func_002F5EF0(u32 unusedResource) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    u32 first[4];
    u32 second[4];

    if ((state->battleFlags & 0x6000000) != 0x6000000) {
        return;
    }
    unit = state->units;
    while (unit != NULL) {
        if (unit->flags & 2) {
            EvtUnit *effect = unit->ext;

            if (effect != NULL) {
                u32 firstColor;
                u32 secondColor;

                VU0_LOAD_VF(vf10, unit->colorStart);
                EE_MMI_RGBA_PACK_UNIT(first[0], 128.0f);
                firstColor = first[0];
                VU0_LOAD_VF(vf10, unit->colorEnd);
                EE_MMI_RGBA_PACK_UNIT(second[0], 128.0f);
                secondColor = second[0];
                effect->color0C = effect->firstCurrent = effect->color = firstColor;
                effect->color5C = effect->color54 = effect->color50 = secondColor;
                VU0_LOAD_VF(vf10, unit->lightDirection);
                VU0_STORE_VF(vf10, unit->ext->vec10);
                VU0_STORE_VF(vf10, unit->ext->vec20);
                VU0_STORE_VF(vf10, unit->ext->vec40);
                evtConfigureUnitTransition(unit->ext, 0);
            }
        }
        unit = unit->nextActor;
    }
}

/* Header common to the resource-instance constructors and callback dispatchers. */
typedef struct EffActiveResource {
    f32 position[4];
    f32 orientation[4];  // 0x10: quaternion consumed by model callbacks
    f32 scale;           // 0x20
    u32 color;           // 0x24
    u32 frame;           // 0x28
    union {
        u32 index;       // 0x2C
        s32 signedIndex;
        u16 shortIndex;
    } kind;
    u32 resource;        // 0x30
    u8 pad_34[4];
    void *payload;       // 0x38
    u8 pad_3C[4];
} EffActiveResource;
typedef char EffActiveResourceSizeCheck[sizeof(EffActiveResource) == 0x40 ? 1 : -1];

/* Entire 40-byte payload copied for resource kind 1. */
typedef struct EffActorLightConfig {
    u32 duration;
    s32 colorFadeIn;
    s32 colorFadeOut;
    s32 directionFadeIn;
    s32 directionFadeOut;
    u32 firstColor;
    u32 secondColor;
    u8 actorSelection;
    u8 pad1D[3];
    EffectVectorRequest direction;
} EffActorLightConfig;
typedef char EffActorLightConfigSizeCheck[sizeof(EffActorLightConfig) == 0x28 ? 1 : -1];
extern void evtSetUnitStatusFlags(EvtUnit *);
extern void evtSetUnitNormalizedDirection(EvtUnit *, s32);
extern void btlUnitGetEffectPosVU(BtlUnit *);
extern void btlUnitGetMuzzlePosVU(BtlUnit *);
extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);
extern u32 btlCameraVectorHasNaN(void);
extern u32 btlBlendColorVec(f32 *, f32 *, f32);
extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);

void func_002F6000(EffActiveResource *work) {
    EffectVectorRequest request;
    BtlUnit *actors[16];
    f32 direction[4];
    f32 origin[4];
    u32 packedStart[4];
    u32 packedEnd[4];
    BtlState *battle = (BtlState *)btlGetRuntime();
    EffActorLightConfig *config = work->payload;
    u32 frame = work->frame;
    u32 count = effCollectModelEffectActors(actors, config->actorSelection);
    u32 index;
    EvtUnit *effect;

    request.count = config->direction.count;
    request.size = config->direction.size;
    request.unk04 = config->direction.unk04;
    switch (config->direction.kind) {
    case 0:
        request.kind = 0;
        break;
    case 1:
        {
            void *owner = (void *)effBTLFieldColorGetOriginalSelector();
            request.kind = 0;
            effBattleMiscQueryPosition(owner, &request, (u128 *)origin);
            request.kind = 0xB;
        }
        break;
    case 2:
        {
            void *owner = (void *)effBTLFieldColorGetVariantSelector();
            request.kind = 0;
            effBattleMiscQueryPosition(owner, &request, (u128 *)origin);
            request.kind = 0xB;
        }
        break;
    case 3:
        request.kind = 1;
        break;
    case 4:
        request.kind = 2;
        break;
    case 5:
        request.kind = 3;
        break;
    case 6:
        {
            void *owner = (void *)effBTLFieldColorGetOverrideSelector();
            request.kind = 6;
            effBattleMiscQueryPosition(owner, &request, (u128 *)origin);
            request.kind = 0xB;
        }
        break;
    case 7:
        {
            void *owner = (void *)effBTLFieldColorGetFinalSelector();
            request.kind = 7;
            effBattleMiscQueryPosition(owner, &request, (u128 *)origin);
            request.kind = 0xB;
        }
        break;
    }
    if (frame == 0) {
        for (index = 0; index < count; index++) {
            if (actors[index]->stateFlags & 0x10) {
                effect = actors[index]->ext;
                evtSetUnitStatusFlags(effect);
                if (config->colorFadeIn != -1 && config->colorFadeOut != -1) {
                    evtInitializeUnitColorTransition(effect, config->colorFadeIn, config->firstColor, config->secondColor);
                }
                if (config->directionFadeIn != -1 && config->directionFadeOut != -1) {
                    if (request.kind == 3) {
                        if (actors[index]->stateFlags & 0x8000) {
                            btlUnitGetEffectPosVU(actors[index]);
                        } else {
                            btlUnitGetMuzzlePosVU(actors[index]);
                        }
                        VU0_LOAD_VF(vf11, work->position);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_NORMALIZE_VF10();
                    } else if (request.kind < 0xB) {
                        effBattleMiscDirectionTo(actors[index], &request, direction);
                        VU0_LOAD_VF(vf10, direction);
                    } else {
                        if (actors[index]->stateFlags & 0x8000) {
                            btlUnitGetEffectPosVU(actors[index]);
                        } else {
                            btlUnitGetMuzzlePosVU(actors[index]);
                        }
                        VU0_LOAD_VF(vf11, origin);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_NORMALIZE_VF10();
                    }
                    evtSetUnitNormalizedDirection(effect, config->directionFadeIn);
                }
            }
        }
    }
    if (config->duration != 0 && frame == config->duration - config->colorFadeOut) {
        for (index = 0; index < count; index++) {
            if (actors[index]->stateFlags & 0x10) {
                u32 firstColor;
                u32 secondColor;
                effect = actors[index]->ext;
                if (btlCameraVectorHasNaN()) {
                    firstColor = btlBlendColorVec(battle->lightColor, actors[index]->colorStart, 0.3f);
                    secondColor = btlBlendColorVec(battle->ambientColor, actors[index]->colorEnd, 0.3f);
                } else {
                    VU0_LOAD_VF(vf10, actors[index]->colorStart);
                    EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
                    firstColor = packedStart[0];
                    VU0_LOAD_VF(vf10, actors[index]->colorEnd);
                    EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
                    secondColor = packedEnd[0];
                }
                evtInitializeUnitColorTransition(effect, config->colorFadeOut, firstColor, secondColor);
            }
        }
    }
    if (config->duration != 0 && frame == config->duration - config->directionFadeOut) {
        for (index = 0; index < count; index++) {
            if (actors[index]->stateFlags & 0x10) {
                effect = actors[index]->ext;
                VU0_LOAD_VF(vf10, actors[index]->lightDirection);
                evtSetUnitNormalizedDirection(effect, config->directionFadeOut);
            }
        }
    }
}


extern void evtSetUnitRgbTransition(EvtUnit *, s32, u32);

void effSyncLinkedActorChildParameter(void) {
    s32 owner = btlGetRuntime();
    BtlUnit *entry;

    if ((((BtlState *)owner)->battleFlags & 0x6000000) != 0x6000000) {
        return;
    }
    entry = ((BtlState *)owner)->units;
    while (entry != NULL) {
        if (entry->flags & 2) {
            EvtUnit *child = entry->ext;
            if (child != 0) {
                child->color60 = entry->baseColor;
                evtSetUnitRgbTransition(child, 0, entry->baseColor);
            }
        }
        entry = entry->nextActor;
    }
}


/* Entire payload copied for resource kind 2 by effAllocateResourcePayload. */
typedef struct EffActorTintConfig {
    u32 duration;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u8 actorSelection;
    u8 pad11[3];
} EffActorTintConfig;
typedef char EffActorTintConfigSizeCheck[sizeof(EffActorTintConfig) == 0x14 ? 1 : -1];

extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);

/* Tint selected eligible actors, then restore their original RGB at fade-out. */
void func_002F64D8(EffActiveResource *work) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *actors[16];
    EffActorTintConfig *config = work->payload;
    u32 frame = work->frame;
    u32 color = config->color;
    u32 count = effCollectModelEffectActors(actors, config->actorSelection);
    u32 i;

    if (frame == 0) {
        for (i = 0; i < count; i++) {
            if (actors[i]->flags & 2) {
                if (actors[i]->flags & 0xE0) {
                    s32 handled;

                    if (actors[i]->flags & 0x200) {
                        continue;
                    }
                    handled = 0;
                    if (state->unk618 != NULL) {
                        handled = state->unk618(actors[i]);
                    }
                    if (!(btlGetEntryFlagsUnlessDisabled(&actors[i]->partyRecord) & 0x200) || handled == 1) {
                        continue;
                    }
                }
                {
                    u32 baseColor = actors[i]->baseColor;
                    EvtUnit *effect = actors[i]->ext;
                    u32 blended;

                    if ((baseColor & 0xFFFFFF) != 0x808080) {
                        blended = (color & baseColor) + (((color ^ baseColor) & 0xFEFEFEFE) >> 1);
                    } else {
                        blended = color;
                    }
                    evtSetUnitRgbTransition(effect, config->fadeIn, blended);
                }
            }
        }
    }
    if (config->duration != 0 && frame == config->duration - config->fadeOut) {
        for (i = 0; i < count; i++) {
            if (actors[i]->flags & 2) {
                if (actors[i]->flags & 0xE0) {
                    s32 handled;

                    if (actors[i]->flags & 0x200) {
                        continue;
                    }
                    handled = 0;
                    if (state->unk618 != NULL) {
                        handled = state->unk618(actors[i]);
                    }
                    if (!(btlGetEntryFlagsUnlessDisabled(&actors[i]->partyRecord) & 0x200) || handled == 1) {
                        continue;
                    }
                }
                evtSetUnitRgbTransition(actors[i]->ext, config->fadeOut, actors[i]->baseColor);
            }
        }
    }
}

s32 effComputeLightDirectionVU(MdlCtx *model, SdfLightingPacketStorage *target) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    if (D_00437E94 == 0) {
        return 0;
    }
    mdlLoadPrimaryVectorVU(model);
    VU0_LOAD_VF(vf11, D_004584B0);
    VU0_SUB(vf10, vf10, vf11);
    VU0_CLEAR_W(vf10);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, D_00458470);
    sdfBuildLightingPacket(target, D_003E9F50, D_004584A0);
    return 1;
}

void effResetDefaultColorTables(void) {
    u128 *dst = D_00458460;
    u128 *src = D_0037F770[0];
    PCP_COPY_VECTOR(dst, src);
    dst++;
    src++;
    PCP_COPY_VECTOR(dst, src);
    PCP_COPY_VECTOR(D_004584A0, kwlnDefaultColorVector);
    D_00437E94 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F67D8);

/* Copied payload plus the five effect/target slots released together. */
typedef struct EffCopiedPayload {
    u8 *body;            // 0x00
    s32 size;            // 0x04
    u32 slotCount;       // 0x08: populated effect/target pairs
    u32 selectedTargetIndex; // 0x0C: chosen target path slot
    EffWorldNode *effects[5]; // 0x10
    Dds3PathCurveWork *targets[5]; // 0x24
    SdfMemBlock *allocation; // 0x38
    u8 pad3C[4];
} EffCopiedPayload;

typedef struct EffCopiedPayloadWork {
    u8 pad00[0x30];
    EffCopiedPayload *payload;
    u8 pad34[4];
    u32 parameter;
} EffCopiedPayloadWork;

void effResetCopiedPayloadTargets(EffCopiedPayloadWork *work) {
    Dds3PathCurveWork **objects = work->payload->targets;
    u32 i;
    for (i = 0; i < 5; i++) {
        Dds3PathCurveWork *object = objects[i];
        if (object != 0) {
            object->direction = 0;
            object->time = 0.0f;
        }
    }
}

EffCopiedPayload *effAllocateCopiedEffectPayload(u32 owner, const void *source, s32 size) {
    u32 headerSize = 0x40;
    SdfMemBlock *base = sdfAllocGeneralBlock(size + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((struct SdfMemBlock *)((u32)base));
    EffCopiedPayload *node = (EffCopiedPayload *)body;

    body += headerSize;
    if (size <= 0) {
        body = 0;
    }
    node->allocation = base;
    node->size = size;
    node->body = body;
    node->slotCount = 0;
    memcpy(body, source, size);
    return node;
}

INCLUDE_ASM(const s32, "game/code_002DE248", effInitializeCopiedPayloadSlots);

struct FldTransferChunk;
extern void fldRelocatePackedTransferChunk(u32, struct FldTransferChunk *);

extern void effInitializeCopiedPayloadSlots(EffCopiedPayload *payload);

EffCopiedPayload *effCreateAndInitializeCopiedPayload(u32 owner, u32 unused, const void *source, s32 size) {
    EffCopiedPayload *work = effAllocateCopiedEffectPayload(owner, source, size);
    u32 bodyAddress = (u32)work->body;
    fldRelocatePackedTransferChunk(bodyAddress,
                                   (struct FldTransferChunk *)(bodyAddress + 8));
    effInitializeCopiedPayloadSlots(work);
    return work;
}

EffCopiedPayload *effCloneEffectPayloadFromOwner(EffCopiedPayloadWork *owner) {
    EffCopiedPayload *work;

    work = effAllocateCopiedEffectPayload(owner->parameter, owner->payload->body,
                                                owner->payload->size);
    effInitializeCopiedPayloadSlots(work);
    return work;
}

extern void dds3FreePathObject(Dds3PathCurveWork *path);

extern void dds3RemoveWorldObjectNode(EffWorldNode *node);

void effDestroyCopiedEffectPayload(EffCopiedPayload *payload) {
    EffWorldNode **tails = payload->effects;
    Dds3PathCurveWork **heads = payload->targets;
    u32 i;
    for (i = 0; i < 5; i++) {
        if (*heads != 0) {
            dds3FreePathObject(*heads);
        }
        heads++;
        if (*tails != 0) {
            dds3RemoveWorldObjectNode(*tails);
        }
        tails++;
    }
    sdfReleaseResourceAllocation(payload->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F6D00);


typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} EffAnimInfo;




extern void btlApplyScaledUnitEffectParameter(BtlUnit *, u16, s32, f32);

extern void btlStartMoveOtherUnitsTask();

void effApplySelectedActorEffects(EffActiveResource *owner) {
    BtlUnit *actor[16];
    EffAnimInfo *info;
    u32 count;
    u32 i;
    if ((s32)owner->frame <= 0) {
        info = owner->payload;
        count = effCollectModelEffectActors(actor, info->actorSelection);
        for (i = 0; i < count; i++) {
            if (actor[i]->flags & 2) {
                if (!(actor[i]->flags & 0x20) && !(actor[i]->effectLink.flags & 0x10)) {
                    if (actor[i]->effectLink.flags & 0x40) {
                        switch (info->id) {
                        case 0:
                        case 2:
                        case 0xa:
                            continue;
                        }
                    }
                    if (mdlGetNodeRefHalf(actor[i]->ext->owner, 0) > info->id) {
                        btlApplyScaledUnitEffectParameter(actor[i], info->id, info->flags | 0x100, 1.0f);
                        if (info->loop == 0) {
                            btlStartMoveOtherUnitsTask(actor[i], info->id);
                        }
                    }
                }
            }
        }
    }
}

void effReportResourceStatus(u8 *work) {
    u32 status;

    if ((s32)((EffActiveResource *)work)->frame > 0) {
        return;
    }
    status = *(u32 *)((EffActiveResource *)work)->payload;
    switch (status) {
    case 0:
        sndLoadAndPlayStationedSe(0x1000A);
        break;
    case 1:
        sndLoadAndPlayStationedSe(0x1000B);
        break;
    }
}

/* Apply the selected battle state's stored tint vectors when both mask bits are set. */
void effApplyBattleStateTint(void) {
    s32 actor;

    actor = btlGetRuntime();
    if ((((BtlState *)actor)->battleFlags & 0x6000000) == 0x6000000) {
        func_00200930((f32 *)(actor + 0x50), (f32 *)(actor + 0x60), 0);
        return;
    }
}

/* Start the payload's packed-color tint transition on frame zero. */
void effStartTintTransitionFromColors(u8 *work) {
    f32 from[3];
    f32 to[3];
    u32 *colors;
    u32 c;

    btlGetRuntime();
    colors = ((EffActiveResource *)work)->payload;
    if ((s32)((EffActiveResource *)work)->frame == 0) {
        c = colors[0];
        from[0] = (c & 0xFF) / 255.0f;
        from[1] = ((c >> 8) & 0xFF) / 255.0f;
        from[2] = ((c >> 16) & 0xFF) / 255.0f;
        c = colors[1];
        to[0] = (c & 0xFF) / 255.0f;
        to[1] = ((c >> 8) & 0xFF) / 255.0f;
        to[2] = ((c >> 16) & 0xFF) / 255.0f;
        func_00200930(from, to, colors[2]);
    }
}

void effResetSlots(void) {
    D_00437E98[0] = 0;
    D_00437EA0[0] = 0;
    D_00437EA8[0] = 0;
    D_00437EB0[0] = 0;
    D_00437E98[1] = 0;
    D_00437EA0[1] = 0;
    D_00437EA8[1] = 0;
    D_00437EB0[1] = 0;
    kwlnPadResetMotorLevelsAndOutput();
}

void effTickSlotVolumeFade(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (D_00437EA8[i] != 0) {
            if (D_00437EA0[i] > 0 && D_00437E98[i] > 0) {
                f32 ratio = (f32)D_00437EA0[i] / (f32)D_00437E98[i];

                if (D_00437EB0[i] != 0) {
                    ratio = 1.0f - ratio;
                }
                kwlnPadStartMotor(i, (u8)(D_00437EA8[i] * ratio), 100);
                D_00437EA0[i] -= 1;
            } else if (D_00437EB0[i] == 1) {
                kwlnPadStartMotor(i, D_00437EA8[i], 100);
            } else {
                kwlnPadStartMotor(i, 0, 0);
                D_00437E98[i] = 0;
            }
        }
    }
}

void effSetDormantSlot(u32 slot, u8 value, u32 data) {
    if (slot < 2) {
        D_00437E98[slot] = data;
        D_00437EA0[slot] = data;
        D_00437EA8[slot] = value;
        D_00437EB0[slot] = 0;
    }
}

void effSetActiveSlot(u32 slot, u8 value, u32 data) {
    if (slot < 2) {
        D_00437E98[slot] = data;
        D_00437EA0[slot] = data;
        D_00437EA8[slot] = value;
        D_00437EB0[slot] = 1;
    }
}

void effResetActiveEffectSlots(void) {
    s32 actor;

    actor = btlGetRuntime();
    if ((((BtlState *)actor)->battleFlags & 0x6000000) == 0x6000000) {
        effResetSlots();
        return;
    }
}

void effUpdateEffectSlotTransitions(s32 work) {
    s32 elapsed;
    u32 duration;
    u32 *slotData;
    u8 *slotColor;
    u32 index;

    index = 0;
    btlGetRuntime();
    elapsed = (s32)((EffActiveResource *)work)->frame;
    slotColor = (u8 *)((EffActiveResource *)work)->payload + 0x18;
    slotData = (u32 *)((u8 *)((EffActiveResource *)work)->payload + 0x10);
    do {
        duration = slotData[-4];
        if (duration < *slotData) {
            return;
        }
        if (elapsed == 0) {
            effSetActiveSlot(index, *slotColor, slotData[-2]);
            duration = slotData[-4];
        }
        if ((duration != 0) && (elapsed == duration - *slotData)) {
            effSetDormantSlot(index, *slotColor, *slotData);
        }
        index = index + 1;
        slotColor = slotColor + 1;
        slotData = slotData + 1;
    } while (index < 2);
}

void effBattleTintTransitionInitialize(void) {
    btlInitTintTransitionDefault(0xc);
}

/* Halfword IDs overlaid on the timer pair's word-oriented payload. */
typedef struct EffTintTransitionIds {
    u8 pad00[8];
    u16 resourceKind;
    u8 pad0A[2];
    u16 defaultKind;
} EffTintTransitionIds;

void effUpdateSlotTimerPair(u8 *work) {
    u32 *slot;
    u32 first;
    s32 phase;

    btlGetRuntime();
    slot = ((EffActiveResource *)work)->payload;
    first = slot[0];
    phase = (s32)((EffActiveResource *)work)->frame;
    if (first < slot[3]) {
        return;
    }
    if (phase == 0) {
        btlInitTintTransitionResource(slot[1], ((EffTintTransitionIds *)slot)->resourceKind);
        first = slot[0];
    }
    if (first != 0 && phase == first - slot[3]) {
        btlInitTintTransitionDefault(((EffTintTransitionIds *)slot)->defaultKind);
    }
}

extern f32 D_00437E90;


void effApplyKeyframeAngle(u8 *work) {
    s32 owner = btlGetRuntime();
    f32 *keys = ((EffActiveResource *)work)->payload;
    u32 total = *(u32 *)keys;
    s32 frame;
    f32 ratio;
    f32 value;

    if (total != 0) {
        frame = ((EffActiveResource *)work)->frame;
        if (total < frame) {
            return;
        }
        ratio = (f32)frame / (f32)total;
        value = ((keys[2] - keys[1]) * ratio + keys[1]) * 0.017453293f;
        btlSetMotionTransformFieldOfView(&((BtlState *)owner)->cameraCommand.camera, value);
        D_00437E90 = value;
    }
}

u32 *effAllocateClassResourceSlot(u32 owner) {
    u32 *work = (u32 *)sdfAllocSizeClassBlock(4);
    *work = 0;
    return work;
}

u32 *func_002F7A00(u32 owner) {
    u32 *work = effAllocateClassResourceSlot(owner);
    *work = (u32)effCreateClassResourceWork(4, (void *)owner);
    return work;
}

u32 *effCreatePayloadPointerWorkFromRequest(u8 *request) {
    u32 *source = (u32 *)((EffActiveResource *)request)->resource;
    u32 *work = effAllocateClassResourceSlot((u32)((EffActiveResource *)request)->payload);
    *work = (u32)effCloneClassResourceWork((EffClassWork *)*source);
    return work;
}

void effReleaseOwnedClassResourceWork(u32 handle) {
    if (*(s32 *)handle != 0) {
        effDestroyClassResourceWork((EffClassWork *)*(s32 *)handle);
    }
    sdfReleaseChipBlock((void *)handle);
}

/* vu0 routine: orient along the vector between two requested positions and advance the resource. */
void func_002F7AC8(u8 *work) {
    u8 *object = ((EffActiveResource *)work)->payload;
    s32 *handle = (s32 *)((EffActiveResource *)work)->resource;
    u128 mtx[4];
    f32 first[4];
    f32 second[4];
    f32 look[4];
    f32 length;
    u8 *state;

    if (effResolveRequestPositionIntoVu((EffectVectorRequest *)(object + 0x88), *(s16 *)(object + 0x98)) == 0) {
        VU0_LOAD_VF(vf10, work);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, first);
    if (effResolveRequestPositionIntoVu((EffectVectorRequest *)(object + 0x90), *(s16 *)(object + 0x9A)) == 0) {
        VU0_LOAD_VF(vf10, work);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, second);
    effCopyClassResourcePosition((s128 *)handle[0], (s128 *)first);
    state = ((EffClassWork *)handle[0])->payload;
    VU0_LOAD_VF(vf10, first);
    VU0_LOAD_VF(vf11, second);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    ((EffAimState *)state)->length = length;
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, D_003E9140);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf29, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_LOAD_VF(vf31, D_003E9110);
    VU0_SET_W_ONE(vf31);
    VU0_STORE_MATRIX_UNCLOBBERED(mtx);
    sdfVuMatrixToQuaternion((f32 (*)[4])mtx);
    VU0_STORE_VF_UNCLOBBERED(vf10, look);
    effCopyClassResourceOrientation((s128 *)handle[0], (s128 *)look);
    effAdvanceClassResourceFrame((EffClassWork *)handle[0]);
}

void effDrawActiveClassResource(s32 owner) {
    effDrawClassResourceWork((EffClassWork *)*(u32 *)((EffActiveResource *)owner)->resource);
}

u32 *effAllocateSurfaceNodeSlot(u32 owner) {
    u32 *work = (u32 *)sdfAllocSizeClassBlock(4);
    *work = 0;
    return work;
}

u32 *effCreateConfiguredSurfaceNodeWork(s32 source, u16 kind, s32 *settings) {
    u32 *work = effAllocateSurfaceNodeSlot(source);
    *work = effCreateSurfaceNodeFromPayload(7, source);
    effConfigureSurfaceNodeByKind((s32 *)*work, kind, settings);
    return work;
}

extern u32 effCreateSurfaceGridWithConfiguration(u8 *);

u32 *effCreateSurfaceGridWorkFromRequest(u8 *request) {
    s32 *source = (s32 *)((EffActiveResource *)request)->resource;
    u32 *work = effAllocateSurfaceNodeSlot((u32)((EffActiveResource *)request)->payload);
    *work = effCreateSurfaceGridWithConfiguration(*source);
    return work;
}

void effReleaseOwnedSurfaceNodeWork(u32 handle) {
    if (*(s32 *)handle != 0) {
        effDestroySurfaceNode(*(EffectSlotNode54 **)handle);
    }
    sdfReleaseChipBlock((void *)handle);
}

/* Resolve and apply the surface's endpoint requests, then acquire its retained record. */
void effResolveAndApplySurfaceEndpoints(u8 *work) {
    u8 *object = ((EffActiveResource *)work)->payload;
    s32 *handle = (s32 *)((EffActiveResource *)work)->resource;
    u8 *slot;
    f32 start[4];
    f32 end[4];
    f32 offset[4];

    if (effResolveRequestPositionIntoVu(&((EffSurfaceEndpoints *)object)->start, ((EffSurfaceEndpoints *)object)->startIndex) == 0) {
        VU0_LOAD_VF(vf10, work);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, start);
    slot = (u8 *)&((EffSurfaceEndpoints *)object)->end;
    if (effResolveRequestPositionIntoVu((EffectVectorRequest *)slot, ((EffSurfaceEndpoints *)object)->endIndex) == 0) {
        /* Matching: preserve this fallback's partial initialization and untouched lanes. */
        if (*slot == 5) {
            offset[0] = 0.0f;
            offset[2] = 0.0f;
            switch (((EffSurfaceEndpoints *)object)->end.count) {
            case 0:
                break;
            case 1:
                offset[1] = -200.0f;
                break;
            case 2:
                offset[1] = -400.0f;
                break;
            case 3:
                offset[1] = -800.0f;
                break;
            case 4:
                break;
            case 5:
                break;
            }
        }
        VU0_LOAD_VF(vf10, offset);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, end);
    func_002DB288(((EffectSlotNode54 *)*handle)->record, start);
    func_002DB2C0(((EffectSlotNode54 *)*handle)->record, end);
    effAcquireSurfaceRecord(*handle);
}

void func_002F7E88(s32 owner) {
    func_002E8770(*(u32 *)((EffActiveResource *)owner)->resource);
}

MdlCtx **effAllocateModelObjectSlot(u32 owner) {
    MdlCtx **work = sdfAllocSizeClassBlock(sizeof(*work));
    *work = NULL;
    return work;
}


extern void mdlAddEntryFlagged(MdlCtx *, s32, s32);

MdlCtx **effCreateAndAttachModelEffectObject(u32 *owner, u32 kind, void *source, u32 settings) {
    MdlCtx **work = effAllocateModelObjectSlot((u32)owner);
    MdlCtx *object = func_002DC1D0(source, settings);
    Motion *active = object->first;
    *work = object;
    if (active != 0) {
        if (*owner != 0) {
            mdlAddEntryPlain(object, 0, 0);
        } else {
            mdlAddEntryFlagged(object, 0, 0);
        }
    }
    return work;
}

MdlCtx **effCreateAndAttachModelFromResourceDescriptor(u8 *request) {
    u32 *owner = ((EffActiveResource *)request)->payload;
    MdlCtx **source = (MdlCtx **)((EffActiveResource *)request)->resource;
    MdlCtx **work = effAllocateModelObjectSlot((u32)owner);
    s32 a = mdlGetContextResourceGroup(*source);
    s32 b = mdlGetContextResourceId(*source);
    MdlCtx *object = func_00232198(a, b);
    *work = object;
    effInitModelVUState(object);
    if ((*work)->first != NULL) {
        if (*owner != 0) {
            mdlAddEntryPlain(*work, 0, 0);
        } else {
            mdlAddEntryFlagged(*work, 0, 0);
        }
    }
    return work;
}

void effReleaseOwnedModelContextWork(MdlCtx **handle) {
    if (*handle != NULL) {
        effDestroyModelContext(*handle);
    }
    sdfReleaseChipBlock(handle);
}

/* Native 0x40-byte payload copied for the model-resource callback table. */
typedef struct EffModelCallbackConfig {
    s32 duration;          /* 0x00: signed update-count threshold */
    f32 frameStep;         /* 0x04 */
    u8 selector;           /* 0x08 */
    u8 pad09[0x37];
} EffModelCallbackConfig;
typedef char EffModelCallbackConfigSizeCheck[sizeof(EffModelCallbackConfig) == 0x40 ? 1 : -1];

extern void mdlStorePrimaryVectorVU(MdlCtx *);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);
extern void mdlStoreTertiaryVectorVU(MdlCtx *);
extern void effObjSetInnerFirstVec(EffWorldNode *, u128 *);
extern void effObjSetInnerSecondVec(EffWorldNode *, u128 *);

void func_002F8040(EffActiveResource *resource)
{
    MdlCtx **modelHandle = (MdlCtx **)resource->resource;
    EffModelCallbackConfig *config = (EffModelCallbackConfig *)resource->payload;
    MdlCtx *model;
    SdfModel *inner;
    BtlUnit *unit;
    s32 elapsedFrame;
    s32 duration;
    f32 matrix[4][4];
    u128 quaternion;

    VU0_LOAD_VF(vf10, resource->position);
    mdlStorePrimaryVectorVU(*modelHandle);
    VU0_LOAD_VF(vf10, resource->orientation);
    mdlUpdateContextRotationBasisFromQuaternion(*modelHandle);
    VU0_SET_ONES_XYZ(vf10);
    VU0_SCALAR_OP(resource->scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(*modelHandle);

    model = *modelHandle;
    model->first->frameStep = config->frameStep;
    mdlProcessContextNodesAndTransforms(model, D_00380828);
    elapsedFrame = (s32)resource->frame;
    duration = config->duration;
    inner = (*modelHandle)->inner;

    if (elapsedFrame < duration || duration == 0) {
        unit = NULL;
        switch (config->selector) {
        case 0:
        case 1:
        case 3:
        case 5:
            unit = (BtlUnit *)effBTLFieldColorGetOriginalSelector();
            break;
        case 2:
        case 4:
            unit = (BtlUnit *)effBTLFieldColorGetVariantSelector();
            break;
        case 6:
            unit = (BtlUnit *)effBTLFieldColorGetOverrideSelector();
            break;
        case 7:
            unit = (BtlUnit *)effBTLFieldColorGetFinalSelector();
            break;
        }

        sdfLoadMapRecordLookAtBasis(inner, 0);
        VU0_STORE_MATRIX_UNCLOBBERED(matrix);
        sdfVuMatrixToQuaternion(matrix);
        VU0_STORE_VF_UNCLOBBERED(vf10, &quaternion);
        effObjSetInnerFirstVec(unit->effectObject, (u128 *)matrix[3]);
        effObjSetInnerSecondVec(unit->effectObject, &quaternion);
        unit->stateFlags |= 0x200000;
    }
}

EffModelBindings *func_002F81A8(s32 *owner) {
    EffModelBindings *work = sdfAllocSizeClassBlock(sizeof(*work));
    work->material = 0;
    work->model = NULL;
    return work;
}

EffModelBindings *effCreateMaterialAndModelEffectWork(s32 *owner, u32 kind, void *source, u32 settings) {
    EffModelBindings *work = func_002F81A8(owner);
    MdlCtx *object;
    Motion *active;

    work->material = effCreateClassResourceWork(4, owner);
    object = func_002DC1D0(source, settings);
    active = object->first;
    work->model = object;
    if (active != 0) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(object, 0, 0);
        } else {
            mdlAddEntryFlagged(object, 0, 0);
        }
    }
    return work;
}

EffModelBindings *effCreateModelEffectWorkFromPayload(u8 *request) {
    s32 *owner = ((EffActiveResource *)request)->payload;
    EffModelBindings *source = (EffModelBindings *)((EffActiveResource *)request)->resource;
    EffModelBindings *work = func_002F81A8(owner);
    s32 a;
    s32 b;
    MdlCtx *object;
    EffClassWork *material;
    MdlCtx *modelSource;

    material = effCloneClassResourceWork(source->material);
    modelSource = source->model;
    work->material = material;
    a = mdlGetContextResourceGroup(modelSource);
    b = mdlGetContextResourceId(source->model);
    object = func_00232198(a, b);
    work->model = object;
    effInitModelVUState(object);
    if (work->model->first != NULL) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(work->model, 0, 0);
        } else {
            mdlAddEntryFlagged(work->model, 0, 0);
        }
    }
    return work;
}

extern void effDestroyClassResourceWork(EffClassWork *);

void effDestroyMaterialAndModelEffectWork(EffModelBindings *work) {
    if (work->model != NULL) {
        effDestroyModelContext(work->model);
    }
    if (work->material != 0) {
        effDestroyClassResourceWork(work->material);
    }
    sdfReleaseChipBlock(work);
}

extern s32 sdfLoadMapRecordLookAtBasis(SdfModel *model, s32 id);

/* vu0 routine: orient along target minus model origin, save distance, and advance the resource. */
void effOrientClassResourceAlongTargetOffset(u8 *work) {
    u8 *object = ((EffActiveResource *)work)->payload;
    EffModelBindings *handle = (EffModelBindings *)((EffActiveResource *)work)->resource;
    u128 mtx[4];
    f32 target[4];
    f32 origin[4];
    f32 look[4];
    f32 length;
    u8 *state;

    if (effResolveRequestPositionIntoVu(&((EffAimConfig *)object)->target, ((EffAimConfig *)object)->targetIndex) == 0) {
        VU0_LOAD_VF(vf10, work);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, target);
    sdfLoadMapRecordLookAtBasis(handle->model->inner, 0);
    VU0_STORE_VF_UNCLOBBERED(vf31, origin);
    effCopyClassResourcePosition((s128 *)handle->material, (s128 *)target);
    state = handle->material->payload;
    VU0_LOAD_VF(vf10, target);
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    ((EffAimState *)state)->length = length;
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, D_003E9140);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf29, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_LOAD_VF(vf31, D_003E9110);
    VU0_SET_W_ONE(vf31);
    VU0_STORE_MATRIX_UNCLOBBERED(mtx);
    sdfVuMatrixToQuaternion((f32 (*)[4])mtx);
    VU0_STORE_VF_UNCLOBBERED(vf10, look);
    effCopyClassResourceOrientation((s128 *)handle->material, (s128 *)look);
    effAdvanceClassResourceFrame(handle->material);
}

extern void mdlStorePrimaryVectorVU(MdlCtx *);

extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);

/* The model helpers read vf10, following the SDK's VU0 macro-mode convention. */
void effApplyModelTransform(u8 *work) {
    EffModelBindings *modelContext = (EffModelBindings *)((EffActiveResource *)work)->resource;
    u8 *animation = ((EffActiveResource *)work)->payload;
    u32 bits;
    float scale;
    VU0_LOAD_VF_MEMORY(vf10, work);
    mdlStorePrimaryVectorVU(modelContext->model);
    VU0_LOAD_VF_MEMORY(vf10, work + 0x10);
    mdlUpdateContextRotationBasisFromQuaternion(modelContext->model);
    VU0_SET_ONES_XYZ(vf10);
    scale = ((EffActiveResource *)work)->scale;
    VU0_SCALAR_OP_TMP_MEMORY(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(modelContext->model);
    modelContext->model->first->frameStep =
        ((EffAimConfig *)animation)->modelParameter;
    mdlProcessContextNodesAndTransforms(modelContext->model, D_00380828);
    effDrawClassResourceWork(modelContext->material);
}

extern SdfAsset *sdfCreateAssetWithDrawEntries(void);

s32 *effCreateDrawableAssetWithDefaultOpacity() {
    s32 *work = sdfAllocAndClearQuadwords(0xC);
    s32 *position;
    work[2] = 0;
    position = (s32 *)sdfCreateAssetWithDrawEntries();
    work[1] = (s32)position;
    ((EffDrawableAsset *)position)->opacity = 1.0f;
    return work;
}

void func_002F85D8(void) {
    effCreateDrawableAssetWithDefaultOpacity();
}

void func_002F85F0(s32 owner) {
    effCreateDrawableAssetWithDefaultOpacity((u32)((EffActiveResource *)owner)->payload);
}

void effReleaseQueuedDrawableAssetWork(u32 work) {
    s32 resource;

    resource = ((EffDrawableAssetWork *)work)->asset;
    if (resource != 0) {
        sdfQueueAssetRelease(resource);
    }
    sdfReleaseChipBlock((void *)work);
}

void func_002F8640(void) {
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F8648);


extern void evtSetUnitAlphaTransition(EvtUnit *, s32, u32);

void effSyncFadeColorToTargets(void) {
    s32 owner = btlGetRuntime();

    if ((((BtlState *)owner)->battleFlags & 0x6000000) == 0x6000000) {
        BtlUnit *node = ((BtlState *)owner)->units;

        while (node != 0) {
            if (node->flags & 2) {
                EvtUnit *target = node->ext;

                if (target != 0) {
                    node->overlayColor = (node->overlayColor & 0xFFFFFF) | (node->baseColor & 0xFF000000);
                    target->color60 = node->baseColor;
                    evtSetUnitAlphaTransition(target, 0, node->baseColor);
                }
            }
            node = node->nextActor;
        }
    }
}

typedef struct EffActorAlphaConfig {
    u32 duration;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u8 actorSelection;
    u8 transition;
    u8 updatePackedAlpha;
    u8 pad13;
} EffActorAlphaConfig;

void effUpdateSelectedActorAlpha(EffActiveResource *work) {
    BtlUnit *actors[16];
    EffActorAlphaConfig *config;
    u32 frame;
    u32 color;
    u32 count;
    u32 i;
    u32 alpha;
    f32 blend;

    config = work->payload;
    frame = work->frame;
    color = config->color;
    count = effCollectModelEffectActors(actors, config->actorSelection);
    if (frame > config->duration) {
        frame = config->duration;
    }
    if (config->updatePackedAlpha) {
        if (frame < config->fadeIn) {
            blend = (f32)frame / (f32)config->fadeIn;
        } else if (frame > config->duration - config->fadeOut) {
            blend = (f32)(config->duration - frame) / (f32)config->fadeOut;
        } else {
            blend = 0.0f;
            if (frame < config->duration) {
                blend = 1.0f;
            }
        }
        alpha = 0x80 - (u32)((f32)(0x80 - (color >> 24)) * blend);
        for (i = 0; i < count; i++) {
            if (actors[i]->flags & 2) {
                actors[i]->overlayColor = (actors[i]->overlayColor & 0xFFFFFF) | (alpha << 24);
            }
        }
    }
    if (config->transition) {
        if (frame == 0) {
            for (i = 0; i < count; i++) {
                if (actors[i]->flags & 2) {
                    evtSetUnitAlphaTransition(actors[i]->ext, config->fadeIn, color);
                }
            }
        }
        if (config->duration != 0 && frame == config->duration - config->fadeOut) {
            for (i = 0; i < count; i++) {
                if (actors[i]->flags & 2) {
                    evtSetUnitAlphaTransition(actors[i]->ext, config->fadeOut, actors[i]->baseColor);
                }
            }
        }
    }
}

extern u8 D_003E9FF0[];

extern void func_003332D0(void *, f32);

s32 *effCreateMotionResource(s32 *context) {
    s32 *work = (s32 *)sdfAllocSizeClassBlock(8);

    work[0] = 0;
    work[1] = (s32)sdfCreateAssetWithDrawEntries();
    func_003332D0((void *)work[1], 1.0f);
    memset(&D_004584C0, 0, sizeof(EffPacketParams));
    D_004584C0.primitive = 0x4000;
    D_004584C0.parameters = (u32 *)D_003E9FF0;
    D_004584C0.parameterCount = 4;
    D_004584C0.vertexCount = 6;
    return work;
}

s32 *effBillboardMotionResourceCreate(s32 *context, u16 kind, s32 *source) {
    s32 *resource = effCreateMotionResource(context);

    switch (kind) {
    case 1:
        resource[0] = (s32)billCreateIndexed(0, (u32)source);
        break;
    case 2:
        resource[0] = (s32)billCreateIndexed(1, (u32)source);
        break;
    case 4:
        resource[0] = (s32)effCreateBillboardSharingIndexedResource(source[0]);
        break;
    }
    billMarkKindOneFlag((struct BillObj *)(resource[0]));
    billSetBillboardMode((struct BillObj *)resource[0], ((EffMotionResourceConfig *)context)->mode);
    return resource;
}

s32 *effBillboardMotionResourceInitialize(s32 *request) {
    s32 *source = (s32 *)request[0x30 / 4];
    s32 *context = (s32 *)request[0x38 / 4];
    s32 *resource = effCreateMotionResource(context);
    resource[0] = (s32)billCloneObjectRetainingSharedData((struct BillObj *)*source);
    billMarkKindOneFlag((struct BillObj *)(resource[0]));
    billSetBillboardMode((struct BillObj *)resource[0], ((EffMotionResourceConfig *)context)->mode);
    return resource;
}

void effDestroyBillboardAndOwnedAssetWork(s32 *work) {
    if (work[0] != 0) {
        billDispatchByKind((BillObj *)(u32)work[0]);
    }
    if (work[1] != 0) {
        sdfQueueAssetRelease(work[1]);
    }
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F91D0);


EffActiveResource *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effRuntimeResourceOperations[kind].payloadSize;
    EffActiveResource *effect = sdfAllocSizeClassBlock(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind.index = kind;
    effect->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, (u8 *)effect + 0x10);
    memcpy(effect->payload, source, size);
    return effect;
}

EffActiveResource *effCreateResourceInstance(u16 kind, void *source, u16 secondaryKind, void *secondary, u32 param) {
    EffActiveResource *effect = effAllocateResourcePayload(kind, source);

    if (btlIsRuntimeAllocated() != 0) {
        if (effRuntimeResourceOperations[kind].createResource != NULL) {
            effect->resource = (u32)effRuntimeResourceOperations[kind].createResource(source, secondaryKind, secondary, param);
        }
        if (effRuntimeResourceOperations[kind].initialize != NULL) {
            effRuntimeResourceOperations[kind].initialize(effect);
        }
    }
    return effect;
}

EffActiveResource *effCreateActiveResourceFromFile(FileJobPayload *source) {
    void *primary = fileResolvePrimaryBuffer(source);
    void *secondary = fileResolveSecondaryBuffer(source);
    return effCreateResourceInstance(source->option, primary,
                         source->primary.selector, secondary, source->secondary.size);
}

void effDestroyResourceInstance(EffActiveResource *obj) {
    if (btlIsRuntimeAllocated()) {
        if (effRuntimeResourceOperations[obj->kind.index].destroyResource != NULL) {
            effRuntimeResourceOperations[obj->kind.index].destroyResource(obj->resource);
        }
    }
    sdfReleaseChipBlock(obj);
}

EffActiveResource *effDuplicateActiveResource(EffActiveResource *source) {
    EffActiveResource *effect;
    u32 kind = source->kind.index;

    if (effRuntimeResourceOperations[kind].cloneResource == NULL) {
        effect = effCreateResourceInstance(source->kind.shortIndex, source->payload, 0, 0, 0);
    } else {
        u32 resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(source->kind.shortIndex, source->payload);
        resource = (u32)effRuntimeResourceOperations[source->kind.signedIndex].cloneResource(source);
        activeKind = source->kind.index;
        effect->resource = resource;
        if (effRuntimeResourceOperations[activeKind].initialize != NULL) {
            effRuntimeResourceOperations[activeKind].initialize(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(EffActiveResource *obj) {
    if (btlIsRuntimeAllocated()) {
        if (effRuntimeResourceOperations[obj->kind.index].initialize != NULL) {
            effRuntimeResourceOperations[obj->kind.index].initialize(obj);
        }
        obj->frame = 0;
    }
}

void effAdvanceCallbackFrame(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0 &&
        (effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) == 0) {
        s32 kind = work->kind.signedIndex;
        EffResourceOps *entry = &effRuntimeResourceOperations[kind];
        void (*callback)(void *) = entry->update;
        if (callback != NULL) {
            callback(work);
        }
        work->frame++;
    }
}

void effDispatchEnabledCallback(EffActiveResource *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = effRuntimeResourceOperations[work->kind.signedIndex].draw;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void effAdvanceActiveResourceCallbacks(EffActiveResource *work) {
    effAdvanceCallbackFrame(work);
    effDispatchEnabledCallback(work);
}

void effCopyActiveResourceVector(EffActiveResource *dst, const void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyActiveResourceSecondaryVector(EffActiveResource *dst, const void *src) {
    PCP_COPY_VECTOR((u8 *)dst + 0x10, src);
}

void effSetActiveResourceColor(EffActiveResource *work, u32 color) {
    work->color = color;
}

void effSetActiveResourceScale(EffActiveResource *work, f32 value) {
    work->scale = value;
}

/* Alternate particle handles share a header but occupy distinct slots. */
typedef struct EffParticleShared {
    u32 color;
    u32 option;
    u32 state;
    u8 pad0C[0x98];
    BillObj *billHandle;  // 0xA4: retained billboard cloned from the source resource.
    struct EffExpandedList *reference; // 0xA8
} EffParticleShared;

u8 *func_002F99E8(FileJobPayload *source) {
    EffParticleShared *work = sdfAllocSizeClassBlock(sizeof(EffParticleShared));
    void *buffer;

    memset(work, 0, sizeof(EffParticleShared));
    work->color = 0x80808080;
    work->billHandle = NULL;
    work->reference = NULL;
    if (source == NULL) {
        return (u8 *)work;
    }
    work->option = source->option;
    buffer = fileResolvePrimaryBuffer(source);
    memcpy(work->pad0C, buffer, sizeof(work->pad0C));
    buffer = fileResolveSecondaryBuffer(source);
    if (buffer != NULL) {
        switch (source->primary.selector) {
        case 1:
            work->billHandle = billCreateIndexed(0, (u32)buffer);
            break;
        case 2:
            work->billHandle = billCreateIndexed(1, (u32)buffer);
            break;
        case 4:
            work->billHandle = effCreateBillboardSharingIndexedResource(*(s32 *)buffer);
            break;
        case 7:
            work->reference = func_002DDF48((u32)buffer);
            break;
        }
        if (work->billHandle != NULL) {
            billMarkKindOneFlag(work->billHandle);
        }
    }
    return (u8 *)work;
}

void effReleaseParticleResources(u32 *p) {
    BillObj *billboard = ((EffParticleShared *)p)->billHandle;
    if (billboard != NULL) {
        billDispatchByKind(billboard);
    }
    if (p[0xA8 / 4] != 0) {
        effReleaseReferenceHolder((struct EffExpandedList *)p[0xA8 / 4]);
    }
    sdfReleaseChipBlock(p);
}

u8 *effCloneParticleSharedResource(u8 *source) {
    u8 *effect = func_002F99E8(NULL);
    memcpy(effect + 0xC, source + 0xC, 0x98);
    effReplaceSharedResource(effect, source);
    return effect;
}

void effReplaceSharedResource(u8 *dst, u8 *src) {
    BillObj *billboard;

    if (((EffParticleShared *)src)->billHandle != 0) {
        if (((EffParticleShared *)dst)->billHandle != 0) {
            billDispatchByKind(((EffParticleShared *)dst)->billHandle);
        }
        billboard = billCloneObjectRetainingSharedData(
            ((EffParticleShared *)src)->billHandle);
        ((EffParticleShared *)dst)->billHandle = billboard;
        billMarkKindOneFlag(billboard);
    } else {
        if (((EffParticleShared *)dst)->reference != 0) {
            effReleaseReferenceHolder(((EffParticleShared *)dst)->reference);
        }
        ((EffParticleShared *)dst)->reference = effReferenceObjectRetain(((EffParticleShared *)src)->reference);
    }
}

void effResetParticleStateWord(s32 work) {
    ((EffParticleShared *)work)->state = 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F9DF0);

void func_002FA388(u32 *target, u32 value) {
    *target = value;
}

extern f32 effMiscRandUnitFloat(void *);

typedef struct EffectSlotNode80 {
    u32 count;
    u32 color;
    f32 opacity;
    s32 resourceKind;       // 0x0C
    u8 randomDirection;     // 0x10
    u8 pad11[3];
    f32 seedDirection[3];   // 0x14
    u8 pad20[4];
    f32 angularSpeed;       // 0x24
    f32 speedRandomness;    // 0x28
    f32 angularAcceleration; // 0x2C
    u8 pad30[0x30];
    MdlCtx *model;           // 0x60
    void *deviceSlot;       // 0x64
    u32 resourceEntries;    // 0x68
    struct SdfMemBlock *entryAllocation;    // 0x6C: queue-clone array descriptor
    u32 record;             // 0x70
    u8 *positions;          // 0x74
    struct SdfMemBlock *positionAllocation; // 0x78: particle-position array descriptor
    u16 active;
} EffectSlotNode80;

/* Normalize the seed vector with the original VU0 macro-mode operations. */
void effInitializeParticleDirection(u8 *work, float *entry) {
    float vector[4];
    if (((EffectSlotNode80 *)work)->randomDirection == 0) {
        vector[0] = ((EffectSlotNode80 *)work)->seedDirection[0];
        vector[1] = ((EffectSlotNode80 *)work)->seedDirection[1];
        vector[2] = ((EffectSlotNode80 *)work)->seedDirection[2];
    } else {
        vector[0] = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f;
        vector[1] = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f;
        vector[2] = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f;
    }
    VU0_NORMALIZE_PACKED_VECTOR(vector);
    entry[0] = vector[0];
    entry[2] = vector[2];
    entry[1] = vector[1];
    entry[5] =
        ((EffectSlotNode80 *)work)->angularSpeed *
        (effMiscRandUnitFloat(effSharedRandomState) * ((EffectSlotNode80 *)work)->speedRandomness +
         (1.0f - ((EffectSlotNode80 *)work)->speedRandomness));
    entry[4] = effMiscRandUnitFloat(effSharedRandomState) * 6.2831853f;
}

extern void sdfBuildVuRotationFromAxisAngle(f32 *, f32);

void effSetRotationFromAcceleratedFrameAngle(u8 *work, f32 *params, s32 frame) {
    f32 axis[4];
    f32 time = (f32)frame;
    f32 angle = params[5] * time;

    angle += ((EffectSlotNode80 *)work)->angularAcceleration * time * time * 0.5f;
    if (angle < 0.0f) {
        angle = 0.0f;
    }
    axis[0] = params[0];
    axis[1] = params[1];
    axis[2] = params[2];
    axis[3] = 0.0f;
    sdfBuildVuRotationFromAxisAngle(axis, angle + params[4]);
}

EffectSlotNode80 *effAllocateActiveEffectSlotNode(u32 count) {
    EffectSlotNode80 *node = sdfAllocAndClearQuadwords(sizeof(EffectSlotNode80));
    node->count = count;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->model = 0;
    node->deviceSlot = 0;
    node->record = 0;
    node->resourceEntries = 0;
    node->entryAllocation = 0;
    node->active = 1;
    return node;
}

s32 *func_002FA5B8(s32 *source) {
    return effAllocateActiveEffectSlotNode(effSlotCount((u8 *)source, 300));
}

extern void effRebuildResourceEntries(u8 *, u32, s32 *);

extern void effReplaceEffectSlotModelAndDeviceResources(EffectSlotNode80 *, void *, u32);

extern void effRebuildResourceEntryClones(EffectSlotNode80 *, FileQueue *);

s32 *func_002FA5F0(FileJobPayload *request) {
    u8 *buffer = fileResolvePrimaryBuffer(request);
    s32 *size = (s32 *)(buffer + 0x50);
    s32 *work = func_002FA5B8(size);
    void *secondary;
    u32 kind;

    memcpy(&((EffectSlotNode80 *)work)->randomDirection, buffer, 0x50);
    effRebuildResourceEntries((u8 *)work, request->option, size);
    secondary = fileResolveSecondaryBuffer(request);
    if (secondary != 0) {
        kind = request->primary.selector;
        switch (kind) {
        case 3:
            effReplaceEffectSlotModelAndDeviceResources((EffectSlotNode80 *)work, secondary, request->secondary.size);
            kind = request->primary.selector;
            break;
        case 6:
            effRebuildResourceEntryClones(work, secondary);
            kind = request->primary.selector;
            break;
        }
        ((EffectSlotNode80 *)work)->resourceKind = kind;
    }
    return work;
}

extern void fileQueueDestroy(s32);

void effDestroyOwnedResources(s32 *work) {
    u32 i;
    if (((EffectSlotNode80 *)work)->model != 0) {
        effDestroyModelContext(((EffectSlotNode80 *)work)->model);
    }
    if (((EffectSlotNode80 *)work)->deviceSlot != 0) {
        sdfReleaseDevSlot(((EffectSlotNode80 *)work)->deviceSlot, 1, 1);
    }
    if (((EffectSlotNode80 *)work)->entryAllocation != 0) {
        u32 count = ((s32 *)((EffectSlotNode80 *)work)->record)[2];
        for (i = 0; i < count; i++) {
            fileQueueDestroy(((s32 *)((EffectSlotNode80 *)work)->resourceEntries)[i]);
        }
        sdfReleaseResourceAllocation(((EffectSlotNode80 *)work)->entryAllocation);
    }
    if (((EffectSlotNode80 *)work)->record != 0) {
        fileReleaseGridRecordHandle((FileSlotTable *)((EffectSlotNode80 *)work)->record);
    }
    if (((EffectSlotNode80 *)work)->positionAllocation != 0) {
        sdfReleaseResourceAllocation(((EffectSlotNode80 *)work)->positionAllocation);
    }
    sdfReleaseChipBlock(work);
}

extern s32 *func_002FA5B8(s32 *);

s32 *effCloneOwnedState(u8 *owner) {
    s32 *source = (s32 *)((FileSlotTable *)((EffectSlotNode80 *)owner)->record)->data1;
    s32 *work = func_002FA5B8(source);
    memcpy(&((EffectSlotNode80 *)work)->randomDirection,
        &((EffectSlotNode80 *)owner)->randomDirection, 0x50);
    effRebuildResourceEntries((u8 *)work, ((FileSlotTable *)((EffectSlotNode80 *)owner)->record)->type, source);
    func_002FA978(work, owner);
    return work;
}

void func_002FA978(EffectSlotNode80 *dst, EffectSlotNode80 *src) {
    s32 kind = src->resourceKind;
    MdlCtx *model;
    BattleGroupNode *modelData;
    u32 count;
    u32 i;

    switch (kind) {
    case 3:
        if (dst->model != 0) {
            effDestroyModelContext(dst->model);
        }
        if (dst->deviceSlot != 0) {
            sdfReleaseDevSlot(dst->deviceSlot, 1, 1);
        }
        model = effCloneModelWithVUState(src->model);
        modelData = model->sub;
        dst->model = model;
        dst->deviceSlot = sdfModelCreateWithAlternateItems(modelData->resourceList, modelData->itemList);
        kind = src->resourceKind;
        break;
    case 6:
        count = ((FileSlotTable *)src->record)->count;
        if (count == 0) {
            return;
        }
        if (dst->entryAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileQueueDestroy(((s32 *)dst->resourceEntries)[i]);
            }
            sdfReleaseResourceAllocation(dst->entryAllocation);
            dst->resourceEntries = 0;
            dst->entryAllocation = 0;
        }
        if (count * 4 == 0) {
            return;
        }
        dst->entryAllocation = sdfAllocGeneralBlock(count * 4);
        dst->resourceEntries = sdfResourceRetainAddress(dst->entryAllocation);
        for (i = 0; i < count; i++) {
            ((void **)dst->resourceEntries)[i] = fileQueueClone(((void **)src->resourceEntries)[0]);
        }
        kind = src->resourceKind;
        break;
    }
    dst->resourceKind = kind;
}


void effRebuildResourceEntries(u8 *work, u32 kind, s32 *config) {
    s32 previous = ((EffectSlotNode80 *)work)->record;
    struct SdfMemBlock *allocation;
    u32 count;
    u32 i;
    u8 *entries;
    if (previous != 0) {
        fileReleaseGridRecordHandle((FileSlotTable *)previous);
    }
    ((EffectSlotNode80 *)work)->record = (u32)fileAllocateGridRecordSlots(kind, *(s32 *)work, config);
    allocation = ((EffectSlotNode80 *)work)->positionAllocation;
    if (allocation != 0) {
        sdfReleaseResourceAllocation(allocation);
    }
    count = ((FileSlotTable *)((EffectSlotNode80 *)work)->record)->count;
    ((EffectSlotNode80 *)work)->positionAllocation = sdfAllocGeneralBlock(count * 0x18);
    ((EffectSlotNode80 *)work)->positions = (u8 *)sdfResourceRetainAddress(((EffectSlotNode80 *)work)->positionAllocation);
    entries = ((EffectSlotNode80 *)work)->positions;
    for (i = 0; i < count; i++, entries += 0x18) {
        effInitializeParticleDirection(work, (float *)entries);
    }
}

void effReplaceEffectSlotModelAndDeviceResources(EffectSlotNode80 *work, void *source, u32 config) {
    BattleGroupNode *modelData;
    MdlCtx *model;
    void *deviceSlot;

    if (work->model != 0) {
        effDestroyModelContext(work->model);
        work->model = 0;
    }
    if (work->deviceSlot != 0) {
        sdfReleaseDevSlot(work->deviceSlot, 1, 1);
        work->deviceSlot = 0;
    }
    model = func_002DC1D0(source, config);
    modelData = model->sub;
    work->model = model;
    deviceSlot = sdfModelCreateWithAlternateItems(modelData->resourceList, modelData->itemList);
    work->deviceSlot = deviceSlot;
}

/* Rebuilds the surface job queue of one record bucket and clones the first job. */
void effRebuildResourceEntryClones(EffectSlotNode80 *obj, FileQueue *secondary) {
    u32 count = ((EffRecordBucket *)obj->record)->count;
    u32 i;
    s32 size;

    if (obj->entryAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileQueueDestroy((s32)(u32)((void **)obj->resourceEntries)[i]);
        }
        sdfReleaseResourceAllocation(obj->entryAllocation);
        obj->resourceEntries = 0;
        obj->entryAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        obj->entryAllocation = sdfAllocGeneralBlock(size);
        obj->resourceEntries = sdfResourceRetainAddress(obj->entryAllocation);
        ((void **)obj->resourceEntries)[0] = fileCloneQueueEntries(secondary);
        for (i = 1; i < count; i++) {
            ((void **)obj->resourceEntries)[i] = fileQueueClone(((void **)obj->resourceEntries)[0]);
        }
    }
}

void effClearSurfaceRecordReferences(s32 node) {
    if (((EffectSlotNode80 *)node)->record != 0) {
        fileClearRecordReferences(((EffectSlotNode80 *)node)->record);
        return;
    }
}

void effAcquireSlotRecordWhenRuntimeFlagClear(s32 node) {
    if (effModelUpdateControlFlags & EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE) {
        return;
    }
    if (((EffectSlotNode80 *)node)->record != 0) {
        fileAcquireRecord(((EffectSlotNode80 *)node)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FAD98);

void func_002FB400(u32 node) {
    effAcquireSlotRecordWhenRuntimeFlagClear(node);
    func_002FAD98(node);
}

void func_002FB428(s32 node) {
    mnuRecordSetVector(((EffectSlotNode80 *)node)->record);
}

void func_002FB440(s32 node) {
    fileSetRecordSecondVector(((EffectSlotNode80 *)node)->record);
}

/* Set the active effect slot's packed color. */
void effSetActiveSlotColor(s32 node, u32 color) {
    ((EffectSlotNode80 *)node)->color = color;
}

/* Set slot opacity and forward it to the retained record's indexed callback. */
void effSetActiveSlotOpacity(s32 *work, f32 opacity) {
    ((EffectSlotNode80 *)work)->opacity = opacity;
    dds3DispatchIndexedCallback(work[0x70 / 4], opacity);
}


extern void mdlSetResourceAmount(MdlCtx *, MdlResourceItem *, f32);
extern void mdlSetAllResourceFrames(MdlCtx *, u32);

typedef struct EffectBlob {
    u32 unk00;
    f32 scale;
    u16 entryId;
    u8 flagged;
    u8 pad0B;
    u32 frame;
    u8 pad10[0x10];
    f32 amounts[0xFF];
    u8 pad41C[0x30];
} EffectBlob;

typedef struct EffSharedEffectResource {
    MdlCtx *model;
    u32 deviceSlot;
    SdfLightingPacketStorage *data;
    u32 unk0C;
    u16 references;
    u8 pad12[2];
    void *entries;
    u32 entriesSize;
    SdfMemBlock *allocation;
} EffSharedEffectResource;
typedef struct EffSharedEffectWork {
    f32 position[4];
    f32 rotation[4];
    u32 unk20;
    f32 unk24;
    u32 unk28;
    EffectBlob data;       // 0x2C through 0x477
    EffSharedEffectResource *resource; // 0x478, shared resource whose reference count is at +0x10
    SdfMemBlock *allocation; /* 0x47C: general-heap descriptor, not the retained address */
} EffSharedEffectWork;

void effApplySharedModelParameters(EffSharedEffectWork *work) {
    f32 *amount;
    f32 scale;
    u32 i;
    SdfMapPositionRecord *record;
    MdlResourceItem *item;

    work->unk28 = 0;
    if (work->resource->model == NULL) {
        return;
    }
    if (work->resource->model->first != NULL) {
        if (work->data.flagged != 0) {
            mdlAddEntryFlagged(work->resource->model, 0, work->data.entryId);
        } else {
            mdlAddEntryPlain(work->resource->model, 0, work->data.entryId);
        }
        work->resource->model->first->frameStep = 1.0f;
    }

    scale = work->data.scale;
    i = 0;
    amount = work->data.amounts;
    for (; i < 0xFF; i++, amount++) {
        record = sdfChunkFindRecordById(work->resource->model->inner, i);
        for (item = work->resource->model->resourceItems; item != NULL; item = item->next) {
            if (item->payload.part.record == record) {
                mdlSetResourceAmount(work->resource->model, item, *amount * scale);
                break;
            }
        }
    }
    mdlSetAllResourceFrames(work->resource->model, work->data.frame);
}

/* General-heap descriptors and retained buffer addresses are distinct owners. */
u8 *func_002FB5C0(FileJobPayload *source) {
    SdfMemBlock *allocation = sdfAllocGeneralBlock(sizeof(EffSharedEffectWork));
    EffSharedEffectWork *work = (EffSharedEffectWork *)sdfResourceRetainAddress(allocation);
    void *secondary;

    memset(work, 0, sizeof(*work));
    work->allocation = allocation;
    /* Retail initializes both vectors with sqc2 vf0 through SDK-style VU macros. */
    VU0_STORE_VF(vf0, work->position);
    VU0_STORE_VF(vf0, work->rotation);
    work->unk20 = 0x80808080;
    work->unk24 = 1.0f;
    if (source != NULL) {
        work->data = *(EffectBlob *)fileResolvePrimaryBuffer(source);
        work->resource = sdfAllocSizeClassBlock(sizeof(*work->resource));
        work->resource->model = NULL;
        work->resource->deviceSlot = 0;
        work->resource->data = NULL;
        work->resource->unk0C = func_001003F8();
        work->resource->references = 1;
        work->resource->data = sdfAllocAndClearQuadwords(0xE0);
        secondary = fileResolveSecondaryBuffer(source);
        work->resource->allocation = sdfAllocGeneralBlock(source->secondary.size);
        work->resource->entriesSize = source->secondary.size;
        work->resource->entries = (void *)sdfResourceRetainAddress(work->resource->allocation);
        memcpy(work->resource->entries, secondary, work->resource->entriesSize);
    }
    return (u8 *)work;
}




/* Release this work; only the final reference releases the shared backing resources. */
void effReleaseSharedResourceReference(EffSharedEffectWork *work) {
    EffSharedEffectResource *resource = work->resource;

    resource->references--;
    if (resource->references == 0) {
        SdfLightingPacketStorage *data = resource->data;
        if (data != NULL) {
            sdfReleaseChipBlock(data);
        }
        resource = work->resource;
        if (resource->model != NULL) {
            effDestroyModelContext(resource->model);
        }
        resource = work->resource;
        if (resource->deviceSlot != 0) {
            sdfReleaseDevSlot(resource->deviceSlot, 1, 1);
        }
        resource = work->resource;
        if (resource->allocation != 0) {
            sdfReleaseResourceAllocation(resource->allocation);
        }
        sdfReleaseChipBlock(work->resource);
    }
    sdfReleaseResourceAllocation(work->allocation);
}



s32 effCloneEffectRequest(u8 *src) {
    u8 *dst = func_002FB5C0(0);

    ((EffSharedEffectWork *)dst)->data = ((EffSharedEffectWork *)src)->data;
    effShareReferenceCountedEffectObject((s32)dst, (s32)src);
    return (s32)dst;
}

void effShareReferenceCountedEffectObject(s32 target, s32 source) {
    ((EffSharedEffectWork *)target)->resource = ((EffSharedEffectWork *)source)->resource;
    ((EffSharedEffectWork *)source)->resource->references++;
}

void func_002FB968(EffSharedEffectWork *work) {
    MdlCtx *context = work->resource->model;
    if (context != NULL) {
        sdfMotionSampleAtFrame(context->first, 0.0f);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FB998);

void func_002FC0A8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002FC0B8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002FC0D0(s32 work, u32 value) {
    ((EffSharedEffectWork *)work)->unk20 = value;
}

void func_002FC0D8(s32 *work, f32 value) {
    ((EffSharedEffectWork *)work)->unk24 = value;
}

void btlInitializeEffectWork(void) {
    D_004386CC = D_003FFA40;
    D_0045C1A0[0] = 0;
    D_004386B0 = 0;
    D_00439075 = 0;
        VU0_STORE_VF(vf0, D_0045C1E0);
}

s32 btlUpdateEffectWork(void) {
    if (D_004386CC != NULL) {
        if ((func_002FCA58(0) & 1) == 0) {
            effResetFileResourceStores();
            return 0;
        }
    }
    return 1;
}

void func_002FC158(void) {
}

void effResetFileResourceStores(void) {
    effResetFileResources();
    effResetFileResourceManager();
}

void effSetBattleOffset(void *src) {
    PCP_COPY_VECTOR(D_0045C1E0, src);
}

void effComputeBattleCameraPositionVU(u8 *effect) {
    u128 direction;
    u32 flags = ((FileJob *)effect)->xformFlags;

    if ((flags & 0x18) != 0) {
        camFollowOffsetVec(effect, &direction);
        VU0_LOAD_VF(vf10, &direction);
        flags = ((FileJob *)effect)->xformFlags;
    } else {
        VU0_LOAD_VF(vf10, ((FileJob *)effect)->offset);
    }
    if ((flags & 0x80) != 0) {
        VU0_SCALAR_OP_CLOBBER(((EffViewScale *)effFileQueue)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    }
    VU0_LOAD_VF(vf11, D_0045C1E0);
    VU0_ADD(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, effFileQueue);
    VU0_ADD(vf10, vf10, vf11);
    if ((flags & 4) != 0) {
        VU0_SCALAR_OP_CLOBBER(-5.0f, "vaddx.y vf10, vf0, vf2x");
    }
}

extern void effMiscQuatMultiplyVU();

extern void camAimRotation();

void effMultiplyQuatWithFlag(u8 *work) {
    s128 rotation;

    if (((FileJob *)work)->xformFlags & 0x60) {
        camAimRotation(work, &rotation);
        VU0_LOAD_VF_MEMORY(vf10, effFileQueue + 0x50);
        VU0_LOAD_VF_MEMORY(vf11, &rotation);
        effMiscQuatMultiplyVU();
    } else {
    VU0_LOAD_VF_MEMORY(vf10, effFileQueue + 0x50);
    VU0_LOAD_VF_MEMORY(vf11, ((FileJob *)work)->quat);
        effMiscQuatMultiplyVU();
    }
}

void effApplyBattleCameraToObject(work)
    void *work;
{
    u128 rotation[2];

    effComputeBattleCameraPositionVU(D_0045C270);
    VU0_STORE_VF_UNCLOBBERED(vf10, &rotation[0]);
    fileJobInvokePositionCallback(work, &rotation[0]);
    effMultiplyQuatWithFlag(D_0045C270);
    VU0_STORE_VF_UNCLOBBERED(vf10, &rotation[1]);
    fileJobInvokeRotationCallback(work, &rotation[1]);
    fileJobInvokeScaleCallback(work, ((FileJob *)D_0045C270)->scale * ((EffViewScale *)effFileQueue)->scale);
    fileDispatchJobTypeCallback(work, ((FileJob *)D_0045C270)->color);
}

INCLUDE_ASM(const s32, "game/code_002DE248", effQueueEffectFileJob);

/* Effect asset/creation descriptor (0x20), separate from the runtime FileJob. */
typedef struct EffFileJobRequest {
    char *name;
    u16 fileKind;
    u8 pad6[2];
    u16 transferMode;
    u8 padA[2];
    void *output;
    u32 size;
    struct SdfMemBlock *allocation;
    u16 resourceMode;
    u8 pad1A[2];
    u32 relatedResource;
} EffFileJobRequest;

FileJobPayload *effLoadFileJobPayload(EffFileJobRequest *descriptor, s32 source) {
    FileJobPayload *job;

    if (source != 0) {
        void *sourceBuffer;
        job = fileJobCreateFromCommandState((const char *)source);
        sourceBuffer = fileResolvePrimaryBuffer(job);
        memcpy(descriptor->output, sourceBuffer, descriptor->size);
    } else {
        job = fileCreateJob(descriptor->fileKind);
        if (descriptor->output != NULL) {
            fileJobSetPrimaryData(job, descriptor->output,
                          (s32)descriptor->size, descriptor->transferMode);
        }
        if (descriptor->relatedResource != 0) {
            fileJobCopyCommandIntoSecondaryData(job, (const char *)descriptor->relatedResource,
                          descriptor->resourceMode);
        } else {
            s32 zero = 0;
            fileJobSetSecondaryData(job, &zero, 4, 4);
        }
    }
    effResetFileResourceManager();
    return job;
}

void effInvokeFileJobWithBattleCamera(u32 work) {
    if (effAuxiliaryFileQueue == 0) {
        effApplyBattleCameraToObject();
        fileJobInvokeTypeCallback(work);
        return;
    }
}

void effFileJobQueueRelease(u32 job) {
    if (effAuxiliaryFileQueue != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    fileJobDestroy((FileJobPayload *)job);
}

extern char D_0042CF58[];

extern char D_0042CF70[];

extern char D_004386D0[];

extern char D_004386D8[];

extern void fileWriteToPfs(u32, char *);

extern s32 func_0035C860(char *, char *, ...);

extern void func_002D50D8(u32, char *);

extern void func_002D55B0(u32, char *);



s32 effPollPrimaryFile(void) {
    EffQueueRecord request;
    char path[0x70];
    s32 result = 0x400001;

    effUpdateResourceQueue(D_0042CF58, D_004386D0, &request);
    if (request.completion.signedState == 2) {
        result = 0x400000;
    } else if (request.completion.signedState == 1) {
        if (effQueuedFileHandle != 0) {
            func_0035C860(path, D_004386D8, D_0042CF58, request.nameWithPrefix);
            result = 0x400002;
            fileWriteToPfs(effQueuedFileHandle, path);
        }
    }
    return result;
}

void effQueueNamedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_004386E0, D_0045C1A0);
}

s32 effPollNamedFile(void) {
    EffQueueRecord request;
    char path[0x70];
    s32 result = 0x400001;

    effUpdateResourceQueue(D_0042CF70, D_004386E0, &request);
    if (request.completion.signedState == 2) {
        result = 0x400000;
    } else if (request.completion.signedState == 1) {
        if (effFileQueue != 0) {
            strcpy((char *)D_0045C1A0, request.name);
            func_0035C860(path, D_004386D8, D_0042CF70, request.nameWithPrefix);
            result = 0x400002;
            func_002D50D8(effFileQueue, path);
        }
    }
    return result;
}

void effQueueAttachedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_004386E8, D_0045C1A0);
}

s32 effPollAttachedFile(void) {
    EffQueueRecord request;
    char path[0x70];
    s32 result = 0x400001;

    effUpdateResourceQueue(D_0042CF70, D_004386E8, &request);
    if (request.completion.signedState == 2) {
        result = 0x400000;
    } else if (request.completion.signedState == 1) {
        if (effFileQueue != 0) {
            strcpy((char *)D_0045C1A0, request.name);
            func_0035C860(path, D_004386D8, D_0042CF70, request.nameWithPrefix);
            result = 0x400002;
            func_002D55B0(effFileQueue, path);
        }
    }
    return result;
}


typedef struct EffQueuedFileObject {
    u8 pad00[0x34];
    u8 *linkedState;
} EffQueuedFileObject;

/* Runtime option (+0x0C) selects the descriptor transfer mode (+0x08). */
u8 *effFindAssetData(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = D_003FF128[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffFileJobRequest *)asset)->fileKind != 6) {
                        if (((EffFileJobRequest *)asset)->fileKind == type && ((EffFileJobRequest *)asset)->transferMode == option) {
                            return asset;
                        }
                    } else if (type == 6) {
                        s32 index = ((EffFileJobRequest *)asset)->transferMode;
                        if (index < 2) {
                            if (index >= 0) {
                                return asset;
                            }
                        }
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

u32 effFindAssetObject(FileJob *entry) {
    FileJobPayload *requested = (FileJobPayload *)entry->id;
    u16 type = requested->type;
    u16 option = requested->option;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = D_003FF128[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffFileJobRequest *)asset)->fileKind == type && ((EffFileJobRequest *)asset)->transferMode == option) {
                        return current->object;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return 0;
}

u32 func_002FCA40(void) {
    return D_003FFA84[0] + D_004386B0;
}

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BEA0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BEB0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BEC0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BED0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BEE0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BEF0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF00);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF10);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF20);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF30);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF40);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF50);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF60);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF70);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF80);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BF90);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BFA0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BFB0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BFC0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BFD0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BFE0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BFF0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C000);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C010);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C020);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C030);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C040);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C050);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C060);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C070);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C080);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C090);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C0A0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C0B0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C0C0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C0E0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C100);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C120);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C140);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C160);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C180);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C1A0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C1C0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C1E0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C200);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C220);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C240);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C260);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C280);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C2A0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C2C0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C2E0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C300);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C320);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C340);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C360);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C380);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C3A0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C3C0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C3E0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C400);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C420);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C440);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C460);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C480);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C4A0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C4C0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C4E0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C500);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C520);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C548);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C568);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C588);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C5A8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C5C8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C5E8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C608);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C628);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C648);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C668);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C688);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C6A8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C6C8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C6E8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C708);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C728);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C748);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C768);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C788);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C7A8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C7C8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C7E8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C808);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C828);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C848);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C868);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C888);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C8A8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C8C8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C8E8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C908);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C928);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C948);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C968);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C988);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C9A8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C9C8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042C9E8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CA08);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CA28);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CA48);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CA68);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CA88);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CAB0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CAD8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CB00);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CB28);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CB48);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CB68);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CB88);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CBA8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CBC8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CBE8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CC08);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CC28);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CC48);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CC68);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CC88);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CCA8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CCC8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CCE8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD08);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD28);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD38);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD48);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD58);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD68);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD78);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD88);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CD98);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CDA8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CDB8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CDC8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CDD8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CDE8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CDF8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE08);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE18);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE28);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE38);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE48);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE58);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE68);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE78);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE88);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CE98);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CEA8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CEB8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CEC8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CED8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CEE8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CEF8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF08);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF18);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF28);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF38);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF48);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF58);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF70);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF80);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042CF90);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FCA58);


typedef struct EffCameraCreateRequest {
    EffFileJobRequest *resource;
    EffQueuedFileObject *object;
    u32 existingJob;
    f32 position[3];
} EffCameraCreateRequest;

extern void fileQueueInitTransform(void *);
extern FileJob *fileAppendJob(FileQueue *, u32);
extern s32 fileQueueCountLinkedJobs(FileQueue *);
extern u32 effCurrentFileQueueEntry;

u32 effAppendPositionedCameraFileJob(EffCameraCreateRequest *request) {
    FileJob *entry;
    u32 count;

    fileQueueInitTransform(D_0045C270);
    entry = fileAppendJob((FileQueue *)effFileQueue, (u32)effLoadFileJobPayload(request->resource, request->existingJob));
    effCurrentFileQueueEntry = (u32)entry;
    strcpy(entry->name, request->resource->name);
    entry->offset[0] = request->position[0];
    entry->offset[1] = request->position[1];
    entry->offset[2] = request->position[2];
    entry->unk88[0] = 8;
    entry->unk88[1] = 0;
    entry->unk88[2] = 0;
    memcpy(D_0045C270, entry, 0x80);
    effQueuedFileHandle = entry->id;
    D_004386BC = effQueueEffectFileJob((u8 *)request->resource);
    effQueuedFileObject = (s32)request->object;
    count = fileQueueCountLinkedJobs((FileQueue *)effFileQueue);
    if (count > 21) {
        D_003FFA78[3] = 20;
        D_004386B0 = count - 21;
    } else {
        D_004386B0 = 0;
        D_003FFA78[3] = count - 1;
    }
    request->object->linkedState = (u8 *)D_003FFA78;
    effResetFileResourceManager();
    return 0x800000;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FD900);

u32 effReinitializeFileQueue(void) {
    if (effTemporaryFileJob != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    if (effFileQueue != 0) {
        fileQueueDestroy(effFileQueue);
    }
    effFileQueue = fileQueueCreate();
    D_003FF1C4[0] = 0;
    D_0045C1A0[0] = 0;
    D_004386B0 = 0;
    D_003FFA78[3] = 0;
    effQueuedFileObject = (s32)D_003FFA78;
    return 0;
}

extern u32 D_003FFA78[];

extern u8 *fileQueueGetAt(u32, u32);

extern u32 D_003FF22C[11];

extern void fileJobCopyHeader(s32 resource, s32 entry);

u32 effResetFileQueueState(void) {
    fileQueueRemoveAndDestroyJob(effFileQueue, fileQueueGetAt(effFileQueue, func_002FCA40()));
    D_003FF294[0] = 0;
    D_004386B0 = 0;
    D_003FFA78[3] = 0;
    effQueuedFileObject = (s32)D_003FFA78;
    return 0;
}

u32 effFinalizeQueuedFile(void) {
    s32 resource;
    s32 entry;
    entry = (s32)fileQueueGetAt(effFileQueue, func_002FCA40());
    resource = fileJobDuplicateAfter(effFileQueue, entry);
    D_003FF22C[0] = 0;
    fileJobCopyHeader(resource, entry);
    effQueuedFileObject = (s32)D_003FFA78;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FDDD8);

u32 effFlagFileQueueActive(void) {
    D_00439075 = 1;
    effSelectLinkedFileState(0);
    return 0;
}

/* Select the queued file object's linked state when present; return zero in either case. */
u32 effSelectLinkedFileState(void) {
    if (((EffQueuedFileObject *)D_004386F0)->linkedState != NULL) {
        effQueuedFileObject = (s32)((EffQueuedFileObject *)D_004386F0)->linkedState;
    }
    return 0;
}

typedef struct EffectFileSelection {
    u8 reserved[0xC];
    s32 selected;
    u32 reserved_10;
    const void *table;
    s32 count;
    u8 tail[0x1C];
} EffectFileSelection;

extern EffectFileSelection effBattleCameraSnapshotSelection;

extern u8 D_003FF390[], D_003FF3D8[], D_0045C270[], D_00439074;

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

u32 effLoadBattleCameraSnapshot(u32 arg0) {
    u8 *record = fileQueueGetAt(effFileQueue, func_002FCA40());

    *(EffectBlock90 *)D_0045C110 = *(EffectBlock90 *)record;
    *(EffectAlignedBlock128 *)D_0045C110 = *(EffectAlignedBlock128 *)D_0045C270;
    if (((EffectBlock90 *)D_0045C110)->word[26] & 2) {
        effBattleCameraSnapshotSelection.table = D_003FF390;
        effBattleCameraSnapshotSelection.count = 3;
        if (effBattleCameraSnapshotSelection.selected > effBattleCameraSnapshotSelection.count) {
            effBattleCameraSnapshotSelection.selected = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.table = D_003FF3D8;
        effBattleCameraSnapshotSelection.count = 9;
        if (effBattleCameraSnapshotSelection.selected == 0) {
            effBattleCameraSnapshotSelection.selected = 1;
        }
    }
    return arg0;
}

u32 effStoreBattleCameraSnapshot(u32 arg0) {
    u8 *record = fileQueueGetAt(effFileQueue, func_002FCA40());

    *(EffectBlock90 *)record = *(EffectBlock90 *)D_0045C110;
    *(EffectAlignedBlock128 *)D_0045C270 = *(EffectAlignedBlock128 *)D_0045C110;
    D_00439074 = 1;
    if (((EffectBlock90 *)D_0045C110)->word[26] & 2) {
        effBattleCameraSnapshotSelection.table = D_003FF390;
        effBattleCameraSnapshotSelection.count = 3;
        if (effBattleCameraSnapshotSelection.selected > effBattleCameraSnapshotSelection.count) {
            effBattleCameraSnapshotSelection.selected = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.table = D_003FF3D8;
        effBattleCameraSnapshotSelection.count = 9;
        if (effBattleCameraSnapshotSelection.selected == 0) {
            effBattleCameraSnapshotSelection.selected = 1;
        }
    }
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FE5B8);

extern u32 func_002FE5B8(const char *, const void *, u32);

extern char D_0042CEF8[];

extern u8 D_003FE280[];

void effCreateParticleTask(void) {
    func_002FE5B8(D_0042CEF8, D_003FE280, 0xF);
}

extern char D_0042CED8[];

extern u8 D_003FE430[];

void effCreatePolyTrackControlTask(void) {
    func_002FE5B8(D_0042CED8, D_003FE430, 7);
}

extern u8 D_003FE500[];

void effCreatePolyTrackTask(void) {
    func_002FE5B8("POLY TRACK S", D_003FE500, 7);
}

extern char D_00437F30[];

extern u8 D_003FE610[];

void effCreateBlurTask(void) {
    func_002FE5B8(D_00437F30, D_003FE610, 7);
}

extern char D_004386A8[];

extern u8 D_003FE6E0[];

void effCreateFilterTask(void) {
    func_002FE5B8(D_004386A8, D_003FE6E0, 3);
}

extern char D_0042CE48[];

extern u8 D_003FE7C0[];

void effCreateEnvironmentTask(void) {
    func_002FE5B8(D_0042CE48, D_003FE7C0, 5);
}

extern char D_0042CEB8[];

extern u8 D_003FE780[];

void effCreatePolyTextureTask(void) {
    func_002FE5B8(D_0042CEB8, D_003FE780, 2);
}

extern char D_0042CEA8[];

extern u8 D_003FE850[];

void effCreatePolyFlashTask(void) {
    func_002FE5B8(D_0042CEA8, D_003FE850, 0xB);
}

extern char D_0042CE98[]; /* "POLY RING" */

extern u8 D_003FE990[];

void effCreatePolyRingTask(void) {
    func_002FE5B8(D_0042CE98, D_003FE990, 6);
}

extern char D_0042CE88[];

extern u8 D_003FEA40[];

void effCreatePolyThunderTask(void) {
    func_002FE5B8(D_0042CE88, D_003FEA40, 5);
}

extern u8 D_003FEAD0[];

void func_002FEAD8(void) {
    func_002FE5B8("POLY TWINKLE", D_003FEAD0, 6);
}

extern char D_0042CE78[];

extern u8 D_003FEB80[];

void effCreatePolyWindTask(void) {
    func_002FE5B8(D_0042CE78, D_003FEB80, 7);
}

extern char D_0042CE68[];

extern u8 D_003FEC50[];

void effCreatePolyScalyTask(void) {
    func_002FE5B8(D_0042CE68, D_003FEC50, 5);
}

extern char D_0042CE58[];

extern u8 D_003FED20[];

void effCreatePolyCrackTask(void) {
    func_002FE5B8(D_0042CE58, D_003FED20, 2);
}

extern char D_0042CDF8[];

extern u8 D_003FED60[];

void effCreateBattleOnlyTask(void) {
    func_002FE5B8(D_0042CDF8, D_003FED60, 0x12);
}

extern char D_00438640[]; /* "2D" */

extern u8 D_003FEFE0[];

void effCreateTwoDimensionalTask(void) {
    func_002FE5B8(D_00438640, D_003FEFE0, 2);
}

extern char D_0042CEE8[];

extern u8 D_003FF020[];

void effCreateObjectParticleTask(void) {
    func_002FE5B8(D_0042CEE8, D_003FF020, 7);
}

extern char D_0042CF58[];

extern FileJob *fileAppendJobFromEntry(FileQueue *queue, void *entry);

s32 effPollPartResource(void) {
    u8 request[0x110];
    s32 state;
    s32 result = 0x400001;
    FileJob *entry;

    effPollResourceBankSlot(D_0042CF58, 0x20, request);
    state = ((EffResourceBankSlot *)request)->state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            entry = fileAppendJobFromEntry((FileQueue *)effFileQueue, request);
            strcpy(entry->name, *(char **)effFindAssetData(entry));
        }
        result = 0x400002;
    }
    return result;
}

u32 effPollNamedFileJob(void) {
    u8 record[0x110];
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(D_0042CF70, 0x10, record);
    state = ((EffResourceBankSlot *)record)->state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            fileQueueDestroy(effFileQueue);
        }
        strcpy((char *)D_0045C1A0, ((EffResourceBankSlot *)record)->name);
        effFileQueue = func_002D5AA8(record);
        D_0045C1F0 = *(EffectBlock128 *)effFileQueue;
        D_004386B0 = 0;
        D_003FFA84[0] = 0;
        result = 0x400002;
    }
    return result;
}

extern void btlBossDebugPrintfN(s32, s32, s32, const char *, ...);
extern void func_00336538(f32);
extern char D_0042D030[], D_0042D048[];
extern s8 sdfPadButtonStates[];
extern f32 D_003FFCB0[4];

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D030);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D048);

s32 func_002FEDD0(void) {
    f32 vector[4];
    f32 step;
    f32 length;
    f32 angle;
    s32 flags = 1;

    btlBossDebugPrintfN(8, 8, 0, D_0042D030);
    btlBossDebugPrintfN(8, 20, 0, D_0042D048);
    btlBossDebugPrintfN(8, 32, 0, "R2   & L2   : RADIUS");
    if (sdfPadButtonStates[6] & 2) {
        D_0045C2F0[1] -= 10.0f;
        flags = 3;
    } else if (sdfPadButtonStates[7] & 2) {
        D_0045C2F0[1] += 10.0f;
        flags = 3;
    }
    if (sdfPadButtonStates[11] & 2) {
        step = -10.0f;
        flags |= 2;
    } else if (sdfPadButtonStates[9] & 2) {
        step = 10.0f;
        flags |= 2;
    } else {
        step = 0.0f;
    }
    /* Project onto the horizontal plane to find the movement direction. */
    vector[0] = D_0045C2F0[0];
    vector[1] = 0.0f;
    vector[2] = D_0045C2F0[2];
    vector[3] = 0.0f;
    if (vector[0] != 0.0f || vector[2] != 0.0f) {
        VU0_LOAD_VF(vf10, vector);
        VU0_LENGTH_VF10(length);
        angle = 0.0f;
        if (sdfPadButtonStates[5] & 2) {
            angle = (100.0f / length) * 0.08726646f;
            flags |= 2;
        } else if (sdfPadButtonStates[4] & 2) {
            angle = (100.0f / length) * -0.08726646f;
            flags |= 2;
        }
        if (angle != 0.0f) {
            func_00336538(angle);
            VU0_LOAD_VF(vf10, vector);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, vector);
        } else {
            VU0_LOAD_VF(vf10, vector);
        }
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, D_003FFCB0);
    }
    vector[0] += D_003FFCB0[0] * step;
    vector[2] += D_003FFCB0[2] * step;
    D_0045C2F0[0] = vector[0];
    D_0045C2F0[2] = vector[2];
    if (sdfPadButtonStates[3] < 0) {
        flags &= ~1;
    } else {
        flags |= 0x200000;
    }
    return flags;
}

s32 effResetStaticState(void) {
    D_0045C2F0[0] = 0;
    D_0045C2F0[1] = 0;
    D_0045C2F0[2] = 0;
    D_0045C2F0[3] = 0;
    D_0045C110[0x88] = 8;
    D_0045C110[0x89] = 0;
    D_0045C110[0x8a] = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FF0B8);

void effCopyCameraSnapshotSelection(void) {
    D_00438758 = D_0045C110[0x88];
    D_00438759 = D_0045C110[0x89];
    D_0043875A = D_0045C110[0x8A];
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FF360);

s32 effResetCameraSnapshotSelection(void) {
    D_0045C110[0x88] = 8;
    D_0045C110[0x89] = 0;
    D_0045C110[0x8a] = 0;
    VU0_STORE_VF(vf0, D_0045C300);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FF8A0);

u32 effLoadFileSlotAndPoll(void) {
    u8 *file = fileQueueGetAt(effFileQueue, func_002FCA40());
    u32 result;

    memcpy(D_0045C110, file, 0x90);
    result = func_002FF0B8(D_0045C110, 1);
    memcpy(file, D_0045C110, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

s32 effRunWithStateBackup(void) {
    EffectStateSnapshot snapshot = *(EffectStateSnapshot *)&D_0045C1F0;
    s128 *backup = &snapshot.vectors[4];
    s32 result;

    PCP_COPY_VECTOR(backup, &D_0045C1F0);
    result = func_002FF0B8(&snapshot, 0);
    PCP_COPY_VECTOR(&D_0045C1F0, backup);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

extern u32 func_002FF8A0(u8 *, s32);

u32 effProcessQueuedFileRecordAndPersistChanges(void) {
    u8 *file = fileQueueGetAt(effFileQueue, func_002FCA40());
    u32 result;
    memcpy(D_0045C110, file, 0x90);
    result = func_002FF8A0(D_0045C110, 1);
    memcpy(file, D_0045C110, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

extern u8 D_00439074;

void effResetFileResources(void) {
    D_003FFA84[0] = 0;
    D_00439074 = 0;
    D_004386B0 = 0;
    if (effFileQueue != 0) {
        fileQueueDestroy(effFileQueue);
        effFileQueue = 0;
    }
    if (effAuxiliaryFileQueue != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
}

void effBattleResourceDescriptorRelease(void) {
    if (effResourceBankDescriptor != 0) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
    }
    if (effResourceBankEntries != 0) {
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

void effPollResourceBankSlot(char *path, u32 unused, EffResourceBankSlot *slot) {
    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory();
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceNameHeaderPair(effResourceBankDescriptor, 0xBA, 0x1C);
    } else {
        func_0020DAB8(effResourceBankDescriptor);
        slot->state = func_0020DFC8(effResourceBankDescriptor);
        slot->type = btlFormatSelectedResourceName(effResourceBankDescriptor, slot);
        slot->count = btlGetResourcePathVariant(effResourceBankDescriptor);
        btlTrimResourceName(effResourceBankDescriptor, slot->name);
        if (slot->state == 1) {
            btlDestroyResourceDescriptor(effResourceBankDescriptor);
            effResourceBankDescriptor = 0;
            btlDestroyEntryList(effResourceBankEntries);
            effResourceBankEntries = 0;
        }
    }
}

extern s32 effFileQueueNameRecord;

extern u32 D_004384E8[];

void effInitializeResourceQueue(void) {
    u8 *queueFile;

    if (effFileQueueNameRecord != 0) {
        func_0020E368(effFileQueueNameRecord);
    }
    effFileQueueNameRecord = btlCreateResourceNameRecord(D_004384E8);
    btlSetResourceNameHeaderPairAlternate(effFileQueueNameRecord, 0xC2, 0xC8);
    func_0020E850(effFileQueueNameRecord, 9);
    queueFile = fileQueueGetAt(effFileQueue, func_002FCA40());
    btlResourceRecordSetName(effFileQueueNameRecord, queueFile + 0x9C);
}

u32 effPollResourceQueue(void) {
    u32 result;
    func_0020E380(effFileQueueNameRecord);
    btlFormatResourceNameWithoutPrefix(effFileQueueNameRecord, fileQueueGetAt(effFileQueue, func_002FCA40()) + 0x9C);
    result = func_0020E7B0(effFileQueueNameRecord);
    if ((u32)(result - 1) < 2) {
        func_0020E368(effFileQueueNameRecord);
        effFileQueueNameRecord = 0;
        return 0;
    }
    return 0x200001;
}

void effReleaseQueuedResourceName(void) {
    if (effQueuedResourceNameRecord != 0) {
        func_0020E368(effQueuedResourceNameRecord);
        effQueuedResourceNameRecord = 0;
    }
}

void effQueueResource(s32 unused, s32 entry) {
    s32 queue = effQueuedResourceNameRecord;
    if (queue == 0) {
        queue = btlCreateResourceNameRecord();
        effQueuedResourceNameRecord = queue;
        btlSetResourceNameHeaderPairAlternate(queue, 0xC2, 0xC8);
    }
    btlResourceRecordSetName(effQueuedResourceNameRecord, entry);
}

void effUpdateResourceQueue(u32 *result, void *queueData, EffQueueRecord *record) {
    u32 state;

    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord(queueData);
        btlSetResourceNameHeaderPairAlternate(effQueuedResourceNameRecord, 0xC2, 0xC8);
        return;
    }
    func_0020E380(effQueuedResourceNameRecord);
    btlFormatResourceNameWithPrefix(effQueuedResourceNameRecord, record->nameWithPrefix);
    btlFormatResourceNameWithoutPrefix(effQueuedResourceNameRecord, record->name);
    state = func_0020E7B0(effQueuedResourceNameRecord);
    record->completion.state = state;
    if (state == 1) {
        if (func_0020E858(effQueuedResourceNameRecord, result) != 0) {
            func_0020E368(effQueuedResourceNameRecord);
            effQueuedResourceNameRecord = 0;
        } else {
            record->completion.state = 0;
        }
    }
}

void effReleaseResourceBankDescriptors(void) {
    if (effResourceBankDescriptor != 0) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
    }
    if (effResourceBankEntries != 0) {
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

typedef struct EffBankStatus {
    u8 pad_00[0xC8];
    s32 type;
    s32 state;
    s32 count;
} EffBankStatus;

extern void btlAppendEntry(s32, char *, s32, s32, s32);

void effPollResourceBank(u32 mode, EffBankStatus *status) {
    if (effResourceBankEntries == 0) {
        u32 i;
        u32 count;
        char name[0x70];

        effResourceBankEntries = btlScanDirectory(0, mode);
        if (mode & 8) {
            count = func_00159BB0();
            for (i = 0; i < count; i++) {
                func_0035C860(name, "GENERAL %d", i);
                btlAppendEntry(effResourceBankEntries, name, 8, i, 0);
            }
        }
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceNameHeaderPair(effResourceBankDescriptor, 0xBA, 0x1C);
    } else {
        func_0020DAB8(effResourceBankDescriptor);
        status->state = func_0020DFC8(effResourceBankDescriptor);
        status->type = btlFormatSelectedResourceName(effResourceBankDescriptor, status);
        status->count = btlGetResourcePathVariant(effResourceBankDescriptor);
        if (status->state == 1) {
            btlDestroyResourceDescriptor(effResourceBankDescriptor);
            effResourceBankDescriptor = 0;
            btlDestroyEntryList(effResourceBankEntries);
            effResourceBankEntries = 0;
        }
    }
}

void effResetMappingFlags(void) {
    D_0043876C = 0;
    D_00438778 = 0;
    D_00438770 = 0;
    D_00438774 = 1;
    effMappingState.field0C = 1;
    effMappingState.field10 |= 0x10;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_00300578);

/* The effect mapping dispatcher stores its data pointer at +0x0C. */
typedef struct EffMappingRequest {
    u8 pad_00[0x0C];
    s32 object;          // 0x0C
} EffMappingRequest;

/* Parameters passed to the mapping dispatcher from distinct object channels. */
typedef struct EffMappingObject {
    u32 value00;
    u8 pad04[0x30];
    union { u32 bits; s32 signedValue; } value34; // 0x34
    u32 value38;       // 0x38
    s32 value3C;
    u8 pad40[0xC];
    u32 value4C;
    u8 pad50[0x20];
    u32 value70;       // 0x70
    s32 value74;
    u8 pad78[8];
    u32 value80;       // 0x80
    u32 value84;       // 0x84
    u8 pad_88[4];
    s32 value8C;       // 0x8C
    u8 pad_90[0x28];
    union { u32 bits; s32 signedValue; } valueB8; // 0xB8
    u8 pad_BC[0x18];
    s32 valueD4;       // 0xD4
    s32 valueD8;       // 0xD8
    u8 pad_DC[0x18];
    s32 valueF4;       // 0xF4
    u8 pad_F8[0x10];
    s32 value108;      // 0x108
} EffMappingObject;

s32 effMapObjectWithTemporaryMappingTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    /* Keep this parameter load raw: a typed member changes the table-write scheduling. */
    result = func_00300578(object + 0x2c, object + 0x50, *(s32 *)(object + 0xb8));
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

s32 func_00301318(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

void func_00301380(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->valueB8.bits);
}

extern u8 D_00459E30[] __attribute__((aligned(4)));

void func_003013A0(void) {
    func_00300578(D_00459E30, D_00459E30 + 0x24, ((EffMappingObject *)D_00459E30)->value34.bits);
}

s32 func_003013C8(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

void func_00301428(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_00301448(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value38);
}

void func_00301468(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

s32 func_00301488(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFE98;
    effMappingState.count = 8;
    /* Keep this parameter load raw: a typed member changes the table-write scheduling. */
    result = func_00300578(object, object + 0x24, *(s32 *)(object + 0x34));
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

void func_003014E8(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_00301508(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

s32 effResolveMappingWithTemporaryTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object + 0x4c, object + 0x70, ((EffMappingObject *)object)->valueD8);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

void func_00301590(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_003015B0(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

void func_003015D8(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_003015F8(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

s32 func_00301620(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object + 0x68, object + 0x8c, ((EffMappingObject *)object)->valueF4);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

void func_00301688(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value70);
}

void func_003016A8(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_003016C8(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_00300578(object + 0x50, object + 0x74, ((EffMappingObject *)object)->value84);
}

s32 func_003016F0(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFE98;
    effMappingState.count = 8;
    result = func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

s32 func_00301750(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

s32 func_003017B8(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object + 0x7c, object + 0xa0, ((EffMappingObject *)object)->value108);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

s32 func_00301820(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_003FFF58;
    effMappingState.count = 8;
    result = func_00300578(object + 0x24, object + 0x48, *(s32 *)object);
    effMappingState.table = D_003FFDD8;
    effMappingState.count = 8;
    return result;
}

typedef struct EffMappingState {
    u8 pad00[0xC];
    u32 unk0C;
    u32 flags;
} EffMappingState;

void btlResetEffectWork(void) {
    u8 *first = D_00400150;
    u8 *second = D_00400250;

    ((EffMappingState *)first)->flags |= 0x10;
    ((EffMappingState *)first)->unk0C = 0;
    ((EffMappingState *)second)->flags |= 0x10;
    ((EffMappingState *)second)->unk0C = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_003018C8);

extern u8 D_00400068[];

extern void func_003018C8(void *, s32, const void *, const void *);

void func_003021A8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, ((EffMappingObject *)source)->valueB8.signedValue, D_00400150, D_00400068);
}

extern u8 D_004001A0[];

void func_003021D8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x8C, ((EffMappingObject *)source)->valueB8.signedValue, D_00400250, D_004001A0);
}

void func_00302208(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x7C, ((EffMappingObject *)source)->valueD4, D_00400150, D_00400068);
}

extern u8 D_00400250[];

void func_00302238(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xA8, ((EffMappingObject *)source)->valueD4, D_00400250, D_004001A0);
}

void func_00302268(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, ((EffMappingObject *)source)->valueB8.signedValue, D_00400150, D_00400068);
}

void func_00302298(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, ((EffMappingObject *)source)->valueB8.signedValue, D_00400150, D_00400068);
}

void func_003022C8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x8C, ((EffMappingObject *)source)->valueB8.signedValue, D_00400150, D_00400068);
}

void func_003022F8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, ((EffMappingObject *)source)->value8C, D_00400150, D_00400068);
}

void func_00302328(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, ((EffMappingObject *)source)->value8C, D_00400150, D_00400068);
}

void func_00302358(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x80, ((EffMappingObject *)source)->valueD8, D_00400150, D_00400068);
}

void func_00302388(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xAC, ((EffMappingObject *)source)->valueD8, D_00400250, D_004001A0);
}

extern u8 D_004000B8[];

void func_003023B8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x90, ((EffMappingObject *)source)->value80, D_00400150, D_004000B8);
}

void func_003023E8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x9C, ((EffMappingObject *)source)->valueF4, D_00400150, D_00400068);
}

void func_00302418(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xC8, ((EffMappingObject *)source)->valueF4, D_00400250, D_004001A0);
}

void func_00302448(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source, ((EffMappingObject *)source)->valueF4, D_00400150, D_004000B8);
}

void func_00302478(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, ((EffMappingObject *)source)->value70, D_00400150, D_004000B8);
}

void func_003024A8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, ((EffMappingObject *)source)->value8C, D_00400150, D_00400068);
}

void func_003024D8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, ((EffMappingObject *)source)->value8C, D_00400150, D_00400068);
}

void func_00302508(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x7C, ((EffMappingObject *)source)->valueD4, D_00400150, D_00400068);
}

void func_00302538(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xA8, ((EffMappingObject *)source)->valueD4, D_00400250, D_004001A0);
}

void func_00302568(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xB0, ((EffMappingObject *)source)->value108, D_00400150, D_00400068);
}

void effMarkMappingUpdatePending(void) {
    D_00438774 = 1;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_003025A8);

void func_00302AD0(s32 work) {
    func_003025A8(((EffMappingRequest *)work)->object + 0x3c, ((EffMappingObject *)((EffMappingRequest *)work)->object)->value4C);
}

void func_00302AF0(s32 work) {
    func_003025A8(((EffMappingRequest *)work)->object + 0x60, ((EffMappingObject *)((EffMappingRequest *)work)->object)->value3C + 1);
}

void func_00302B18(s32 work) {
    func_003025A8(((EffMappingRequest *)work)->object + 0x70, ((EffMappingObject *)((EffMappingRequest *)work)->object)->value8C + 1);
}

void func_00302B40(s32 work) {
    func_003025A8(((EffMappingRequest *)work)->object + 0x70, ((EffMappingObject *)((EffMappingRequest *)work)->object)->value8C + 1);
}

void func_00302B68(s32 work) {
    func_003025A8(((EffMappingRequest *)work)->object + 0x60, ((EffMappingObject *)((EffMappingRequest *)work)->object)->value74 + 1);
}

void func_00302B90(s32 work) {
    func_003025A8((u32 *)((EffMappingRequest *)work)->object + 4, ((EffMappingObject *)((EffMappingRequest *)work)->object)->value00);
}


s32 effPollFileQueueRecord(s32 mode) {
    u8 status[0xE0];
    u8 *entry;
    s32 result = 0x600001;

    effPollResourceBank(mode, (EffBankStatus *)status);
    if (((EffBankStatus *)status)->state == 2) {
        result = 0x400000;
    } else if (((EffBankStatus *)status)->state == 1) {
        if (((EffBankStatus *)status)->type != 8) {
            entry = fileQueueGetAt(effFileQueue, -((EffBankStatus *)status)->count);
            if (effCurrentFileQueueEntry != (u32)entry) {
                fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
                fileQueueLinkJobToSectorLeader(effFileQueue, effCurrentFileQueueEntry, entry);
            }
        } else {
            fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
            fileJobSetSecondaryData((FileJobPayload *)effQueuedFileHandle, status + 0xD0, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

void func_00302C78(void) {
    effPollFileQueueRecord(0x4b);
}

void func_00302C90(void) {
    effPollFileQueueRecord(0x4b);
}

void func_00302CA8(void) {
    effPollFileQueueRecord(0xb);
}

s32 effPollFileRecord(const char *resourceName, s32 mode) {
    u8 status[0x110];
    s32 result = 0x600001;

    effPollResourceBankSlot(resourceName, mode, status);
    if (((EffResourceBankSlot *)status)->state == 2) {
        result = 0x400000;
    } else if (((EffResourceBankSlot *)status)->state == 1) {
        fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
        if (((EffResourceBankSlot *)status)->type != 8) {
            fileJobCopyCommandIntoSecondaryData((FileJobPayload *)effQueuedFileHandle,
                           (const char *)status,
                           effClassifyResourceMask(((EffResourceBankSlot *)status)->type));
        } else {
            fileJobSetSecondaryData((FileJobPayload *)effQueuedFileHandle, status + 0x104, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

extern char D_0042D128[]; /* "/tool/effect/mat/" */

extern char D_0042CF70[]; /* "/tool/effect/" */

extern char D_0042D140[]; /* "/tool/effect/hlp/" */

void func_00302D70(void) {
    effPollFileRecord(D_0042D128, 0x43);
}

void func_00302D90(void) {
    effPollFileRecord(D_0042D128, 0x43);
}

void func_00302DB0(void) {
    effPollFileRecord(D_0042CF58, 0x20);
}

void func_00302DD0(void) {
    effPollFileRecord(D_0042CF70, 0x10);
}

void func_00302DF0(void) {
    effPollFileRecord(D_0042CF58, 0x20);
}

void func_00302E10(void) {
    effPollFileRecord(D_0042CF70, 0x10);
}

void func_00302E30(void) {
    effPollFileRecord(D_0042D128, 1);
}

void func_00302E50(void) {
    effPollFileRecord(D_0042D128, 1);
}

void func_00302E70(void) {
    effPollFileRecord(D_0042D128, 1);
}

u32 effFileJobSecondaryDataSet(void) {
    u32 zero = 0;
    fileJobSetSecondaryData((FileJobPayload *)effQueuedFileHandle, &zero, 4, 4);
    return 0x400002;
}

void func_00302EC8(void) {
    effPollFileRecord(D_0042D140, 4);
}

void func_00302EE8(void) {
    effPollFileRecord(D_0042D128, 4);
}

void func_00302F08(void) {
    effPollFileRecord(D_0042CF70, 0x10);
}

void func_00302F28(void) {
    effPollFileRecord(D_0042D140, 4);
}


extern EffFileJobRequest D_003FB948;

extern EffFileJobRequest D_003F01D0;



u32 fileLoadEffectSlotA(void) {
    u8 fileInfo[0x110];
    FileJobPayload *job;
    FileJob *entry;
    EffFileJobRequest *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_0042D128, 4, fileInfo);
    status = ((EffResourceBankSlot *)fileInfo)->state;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = fileCreateJob(3);
        fileJobSetPrimaryData(job, D_003F01D0.output, D_003F01D0.size,
                      D_003F01D0.transferMode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)fileInfo,
                      effClassifyResourceMask(((EffResourceBankSlot *)fileInfo)->type));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = (s32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileJobRequest *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer((FileJobPayload *)effQueuedFileHandle);
        memcpy(resource->output, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob((u8 *)resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern u8 D_0045C270[];

extern u32 effCurrentFileQueueEntry;

extern u32 D_004386BC;

extern void effResetFileResourceManager(void);

extern u8 D_003F0DA8[] __attribute__((aligned(4)));

typedef struct EffFileQueryInfo {
    u8 pad0[0xFC];
    u32 resourceMask;
    s32 status;
    u8 pad104[0xC];
} EffFileQueryInfo;

u32 effQueueGeneratedFileJob(void) {
    EffFileQueryInfo fileInfo;
    u32 result;
    s32 status;
    FileJobPayload *job;
    FileJob *entry;
    EffFileJobRequest *resource;
    u8 *buffer;
    DevState *command;
    u32 totalLength;
    u32 dataLength;
    struct SdfMemBlock *allocation;

    effPollResourceBankSlot(D_0042D140, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        u32 headerBytes = 0x80;
        struct SdfMemBlock *oldAllocation;
        u32 queuedFile;
        job = fileCreateJob(6);
        command = sdfDevCreateCommandState(&fileInfo);
        dataLength = sdfDevQueueControlAndWait(command);
        totalLength = dataLength + headerBytes;
        allocation = sdfAllocGeneralBlock(totalLength);
        buffer = (u8 *)sdfResourceRetainAddress(allocation);
        memset(buffer, 0, headerBytes);
        sdfDevQueueReadAndWait(command, buffer + headerBytes, dataLength);
        sdfDevWaitThenReleaseCommandState(command);
        fileJobSetPrimaryData(job, buffer, totalLength, 1);
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = (u32)entry;
        resource = (EffFileJobRequest *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        memcpy(D_0045C270, entry, 0x80);
        queuedFile = entry->id;
        oldAllocation = resource->allocation;
        effQueuedFileHandle = queuedFile;
        if (oldAllocation != 0) {
            sdfReleaseResourceAllocation(oldAllocation);
        }
        resource->allocation = allocation;
        resource->output = buffer;
        resource->size = dataLength + headerBytes;
        resource->transferMode = 1;
        memcpy(D_00459E30, D_003F0DA8, 0x74);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffFileJobRequest D_003F9060;

u32 effPollAndQueueCopiedFileResource(void) {
    EffFileQueryInfo fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileJobRequest *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_0042D128, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = fileCreateJob(0x12);
        fileJobSetPrimaryData(job, D_003F9060.output, D_003F9060.size,
                      D_003F9060.transferMode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileJobRequest *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer((FileJobPayload *)effQueuedFileHandle);
        memcpy(resource->output, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D0B8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D0C8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D0D8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D0E8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D0F8);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D108);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D118);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D128);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D140);

u32 effLoadFileSlotF2(void) {
    EffFileQueryInfo fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileJobRequest *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot("/tool/effect/f2/", 0x80, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = fileCreateJob(0x14);
        fileJobSetPrimaryData(job, D_003FB948.output, D_003FB948.size,
                      D_003FB948.transferMode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileJobRequest *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer((FileJobPayload *)effQueuedFileHandle);
        memcpy(resource->output, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffFileJobRequest D_003FD988;

u32 effLoadMaterialFile(void) {
    EffFileQueryInfo fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileJobRequest *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_0042D128, 2, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = fileCreateJob(0x16);
        fileJobSetPrimaryData(job, D_003FD988.output, D_003FD988.size,
                      D_003FD988.transferMode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileJobRequest *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer((FileJobPayload *)effQueuedFileHandle);
        memcpy(resource->output, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffFileJobRequest D_003FE040;

u32 effPollAndQueueFileResourceWithUnitFloats(void) {
    EffFileQueryInfo fileInfo;
    FileJobPayload *job;
    FileJob *entry;
    EffFileJobRequest *resource;
    void *fileData;
    s32 status;
    u32 result;
    u32 index;
    f32 *coordinates;

    effPollResourceBankSlot(D_0042D128, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = fileCreateJob(0x19);
        coordinates = (f32 *)((u8 *)D_003FE040.output + 0x20);
        for (index = 0; index < 0xFF; index++) {
            *coordinates++ = 1.0f;
        }
        fileJobSetPrimaryData(job, D_003FE040.output, D_003FE040.size,
                      D_003FE040.transferMode);
        fileJobCopyCommandIntoSecondaryData(job, (const char *)&fileInfo,
                      effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, (u32)job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileJobRequest *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer((FileJobPayload *)effQueuedFileHandle);
        memcpy(resource->output, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy((FileJobPayload *)effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

void effResetFileResourceManager(void) {
    D_0043876C = 0;
    D_00438774 = 1;
    D_00438770 = 0;
    D_00438778 = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

u16 effClassifyResourceMask(s32 mask) {
    switch (mask) {
    case 1: return 1;
    case 2: return 2;
    case 4: return 3;
    case 8: return 4;
    case 0x20: return 5;
    case 0x10: return 6;
    case 0x40: return 7;
    case 0x80: return 8;
    default: return 0;
    }
}

EffectList *mnuAllocateValueRecord(u32 mode) {
    EffectList *list = sdfAllocSizeClassBlock(sizeof(EffectList));
    memset(list, 0, sizeof(EffectList));
    list->mode = mode;
    list->count = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

void effDestroyEffectList(EffectList *list) {
    sdfReleaseChipBlock(list);
}

u32 mnuGetValueRecordOwner(const EffectList *list) {
    return list->mode;
}

typedef struct EffLoadedItem {
    struct EffLoadedItem *next;
    u8 pad_04[4];
    u32 buffer; // 0x08
    u8 pad_0C[4];
    u8 kind;    // 0x10: 1 once the job's buffer is ready
} EffLoadedItem;

typedef struct EffRequest {
    u8 pad_00[0x60];
    EffLoadedItem *items; // 0x60
} EffRequest;



extern void func_002C7CE8(void *);



u32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference) {
    EffectListNode *node = (EffectListNode *)sdfAllocSizeClassBlock(0x18);
    memset(node, 0, 0x18);
    node->next = 0;
    node->kind = kind;
    node->value = value;
    node->length = length;
    node->reference = (void **)reference;
    if (list->last == 0) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    return ++list->count;
}

u32 effRemoveListEntry(EffectList *list) {
    EffectListNode *node = list->first;
    EffectListNode *next = node->next;
    sdfReleaseChipBlock(node);
    list->first = next;
    list->count -= 1;
    if (list->count == 0) {
        list->last = 0;
    }
    return list->count;
}

/* Drives the head request of the list: mode 0 asks for a resource by name, modes 1 and 2 stream a package and hand each
 * finished job's buffer to the entry's destination. Returns the remaining entry count. */
s32 effPollResourceList(EffectList *list) {
    EffectListNode *node = list->first;
    EffLoadedItem *item;
    u32 buffer;

    if (node != NULL) {
        switch (list->mode) {
        case 0:
            if (node->state == 0) {
                effRequestResourceByMode(node->value, node->length, node->kind, node->reference);
                node->state = 1;
            } else if (*node->reference != NULL) {
                effRemoveListEntry(list);
            }
            break;
        case 1:
        case 2:
            if (node->state == 0 && node->length != 0) {
                if (list->request != NULL) {
                    func_002C7CE8(list->request);
                }
                list->request = (EffRequest *)fileQueuePlainDispatchRequest((const char *)node->length);
                if (list->mode == 2) {
                    func_002C81D0((struct FileRequest *)list->request);
                }
                node->state = 1;
            } else if (fileRequestIsReady((struct FileRequest *)list->request) != 0) {
                for (item = list->request->items; item != NULL; item = item->next) {
                    if (item->kind == 1) {
                        node = list->first;
                        buffer = item->buffer;
                        *node->reference = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)buffer, node->kind);
                        if (node->kind == 0) {
                            sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(buffer));
                        }
                        effRemoveListEntry(list);
                    }
                }
            }
            if (list->count == 0) {
                func_002C7CE8(list->request);
                list->request = NULL;
            }
            break;
        }
    }
    return list->count;
}

extern char D_004387E8[];

#define EFF_RESOURCE_PATH_BYTES 0x80
#define EFF_RESOURCE_KEEP_ALLOCATION 1
#define EFF_RESOURCE_TRANSIENT 0
#define EFF_OWNER_LIST_BYTES 0x44
#define EFF_RECORD_BUCKETS 16
#define EFF_RECORD_LAST_BUCKET 15
#define EFF_STATUS_RECORD_BYTES 0x24
#define EFF_PACKED_STATUS_HEADER_BYTES 0x20
#define EFF_STATUS_PARAM_STRIDE 0x18
#define EFF_STATUS_PARAM_HEADER_BYTES 4
#define EFF_BATCH_HEADER_BYTES 0xC
#define EFF_SLOT_WORK_BYTES 0xA0
#define EFF_PHASE_FULL 0x10000
#define EFF_PAYLOAD_RECORD_BYTES 0x6C
#define EFF_RESOURCE_TABLE_ENTRY_BYTES 8

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, s32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    u32 allocation;
    EffectSlotSet *instance;
    func_0035C860(path, D_004387E8, base, name);
    allocation = sdfReadNamedResource(path, &sourceAddress, 0);
    instance = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)allocation, keepAllocation);
    if (keepAllocation == EFF_RESOURCE_TRANSIENT) {
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    }
    return instance;
}

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(struct FileRequest *job, u32 *outInstance) {
    u32 allocation;
    EffectSlotSet *instance;

    allocation = fileGetResourceHandle(job);
    instance = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)allocation, EFF_RESOURCE_TRANSIENT);
    *outInstance = (u32)instance;
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    filePollEntryCleanup(job);
}

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(struct FileRequest *job, u32 *outInstance) {
    u32 allocation;
    EffectSlotSet *instance;

    allocation = fileGetResourceHandle(job);
    instance = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)allocation, EFF_RESOURCE_KEEP_ALLOCATION);
    *outInstance = (u32)instance;
    filePollEntryCleanup(job);
}

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(const char *prefix, const char *name, s32 mode, u32 *outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];
    func_0035C860(path, D_004387E8, prefix, name);
    *outInstance = 0;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        fileCreateCallbackRequest(path, 0, (s32)effCompleteRetainedResourceJob, (s32)outInstance);
    } else {
        fileCreateCallbackRequest(path, 0, (s32)effCompleteTransientResourceJob, (s32)outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
EffMappedResource *effLoadMappedResource(const char *base, const char *name) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    u32 allocation;
    EffMappedResource *mappedResource;
    func_0035C860(path, D_004387E8, base, name);
    allocation = sdfReadNamedResource(path, &sourceAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    return mappedResource;
}

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(struct FileRequest *job, u32 *outMappedResource) {
    u32 allocation;
    u32 sourceAddress;
    EffMappedResource *mappedResource;

    allocation = fileGetResourceHandle(job);
    sourceAddress = sdfResourceRetainAddress((struct SdfMemBlock *)(allocation));
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    *outMappedResource = (u32)mappedResource;
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    filePollEntryCleanup(job);
}

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(const char *base, const char *name, u32 *outMappedResource) {
    char path[EFF_RESOURCE_PATH_BYTES];
    func_0035C860(path, D_004387E8, base, name);
    *outMappedResource = 0;
    fileCreateCallbackRequest(path, 0, (s32)effCompleteMappedResourceJob, (s32)outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
EffectOwnerRecord *effCreateOwnerRecordList(u32 ownerAddress) {
    EffectOwnerRecord *list = (EffectOwnerRecord *)sdfAllocSizeClassBlock(EFF_OWNER_LIST_BYTES);
    memset(list, 0, EFF_OWNER_LIST_BYTES);
    list->owner = (void *)ownerAddress;
    return list;
}

/* Clamp only the upper bucket bound and prepend; the former head's prev link is not rewritten. */
void effInsertSlotRecord(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucket;
    EffectRecord *record;

    if (bucketIndex >= EFF_RECORD_BUCKETS) {
        bucketIndex = EFF_RECORD_LAST_BUCKET;
    }
    record = sdfAllocSizeClassBlock(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slotIndex;
    record->next = list->entries[bucketIndex];
    list->entries[bucketIndex] = record;
}

/* Remove the first matching slot from the raw bucket index; unlike insertion, this does not clamp it. */
s32 effRemoveSlotRecord(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucket;
    EffectRecord *record = list->entries[bucketIndex];

    while (record != 0) {
        if (record->slot == slotIndex) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == list->entries[bucketIndex]) {
                list->entries[bucketIndex] = record->next;
            }
            sdfReleaseChipBlock(record);
            return 1;
        }
        record = record->next;
    }
    return 0;
}

/* Release all sixteen buckets; retain the original traversal that reads next after releasing a record. */
void effReleaseRecordBuckets(EffectOwnerRecord *list) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown;
    for (bucketCountdown = EFF_RECORD_LAST_BUCKET; bucketCountdown >= 0; bucketCountdown--, bucketHead++) {
        EffectRecord *record = *bucketHead;
        while (record != 0) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == *bucketHead) {
                *bucketHead = record->next;
            }
            sdfReleaseChipBlock(record);
            record = record->next;
        }
    }
}

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
s32 effDispatchRecordBuckets(s32 refresh, EffectOwnerRecord *list, s32 drawOption) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown;
    for (bucketCountdown = EFF_RECORD_LAST_BUCKET; bucketCountdown >= 0; bucketCountdown--, bucketHead++) {
        EffectRecord *record = *bucketHead;
        while (record != 0) {
            itfDrawGridWithResolvedSlot(0, 0, 0, 0, (EffectSlotSet *)(u32)list->owner, record->slot, drawOption);
            if (refresh != 0) {
                itfGridLookupValueOrDefault((u32)list->owner, record->slot);
            }
            record = record->next;
        }
    }
    return 1;
}

/* Release bucket records before freeing the owner list. */
u32 effDestroyOwnerRecordList(u32 buckets) {
    effReleaseRecordBuckets((EffectOwnerRecord *)buckets);
    sdfReleaseChipBlock((void *)buckets);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DE248", effConvertParamValue);


/* Sum the status-storage byte requirements for the selected record category. */
u32 effSumRecordStatuses(const EffMappedRecord *record) {
    EffRecordBucket *group = &D_00400508[record->category];
    u32 statusBytes = 0;
    u32 recordIndex;

    for (recordIndex = 0; recordIndex < group->count; recordIndex++) {
        statusBytes += effConvertParamValue(group->records + recordIndex * EFF_STATUS_PARAM_STRIDE + EFF_STATUS_PARAM_HEADER_BYTES, 0, 0, 0);
    }
    return statusBytes;
}

/* Serialized file header; this is distinct from the live batch header below. */
typedef struct EffMappedHeader {
    u8 pad_00[0x14];
    u32 count;        // 0x14
    u8 pad_18[8];
} EffMappedHeader;    // 0x20

/* Copy packed headers and status data into live records, zero-filling extra status capacity.
 * The required-size calculation uses the first record, not the current row.
 * Returns the record-allocation handle, not its retained address.
 */
struct SdfMemBlock *effLoadMappedStatusRecords(const u8 *source, EffMappedHeader *headerOut) {
    EffMappedHeader header;
    struct SdfMemBlock *allocation;
    EffMappedRecord *records;
    u32 recordIndex = 0;
    u32 statusBytes;

    memcpy(&header, source, sizeof(header));
    source += sizeof(header);
    allocation = sdfAllocGeneralBlock(header.count * EFF_STATUS_RECORD_BYTES);
    records = (EffMappedRecord *)sdfResourceRetainAddress(allocation);
    for (; recordIndex < header.count; recordIndex++) {
        EffMappedRecord *record = &records[recordIndex];

        memcpy(record, source, EFF_PACKED_STATUS_HEADER_BYTES);
        source += EFF_PACKED_STATUS_HEADER_BYTES;
        statusBytes = effSumRecordStatuses(records);
        if (statusBytes < record->statusBytes) {
            statusBytes = record->statusBytes;
        }
        record->status = sdfAllocSizeClassBlock(statusBytes);
        memset(record->status, 0, statusBytes);
        memcpy(record->status, source, record->statusBytes);
        source += record->statusBytes;
        if (record->statusBytes < statusBytes) {
            record->statusBytes = statusBytes;
        }
    }
    if (headerOut != 0) {
        memcpy(headerOut, &header, sizeof(header));
    }
    return allocation;
}


/* Build the live batch header from the serialized count and owned record array. */
EffMappedResource *effCreateMappedResource(const u8 *source) {
    EffMappedResource *mappedResource = (EffMappedResource *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    EffMappedHeader header;

    mappedResource->allocation = effLoadMappedStatusRecords(source, &header);
    mappedResource->records = (EffMappedRecord *)sdfResourceRetainAddress(mappedResource->allocation);
    mappedResource->count = header.count;
    return mappedResource;
}

/* Build one zeroed status record and allocate the category's required status storage. */
EffMappedResource *effCreateStatusBatch(u32 category) {
    EffMappedResource *batch = (EffMappedResource *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    struct SdfMemBlock *allocation;
    u32 statusBytes;
    u8 *statuses;

    batch->count = 1;
    allocation = sdfAllocGeneralBlock(EFF_STATUS_RECORD_BYTES);
    batch->allocation = allocation;
    batch->records = (EffMappedRecord *)sdfResourceRetainAddress(allocation);
    memset(batch->records, 0, EFF_STATUS_RECORD_BYTES);
    {
        EffMappedRecord *record = batch->records;
        record->category = category;
        statusBytes = effSumRecordStatuses(record);
    }
    statuses = (u8 *)sdfAllocSizeClassBlock(statusBytes);
    batch->records->status = statuses;
    memset(statuses, 0, statusBytes);
    batch->records->statusBytes = statusBytes;
    return batch;
}

/* Release each record's status storage, then the record allocation and batch header. */
s32 effDestroyPackedBatch(EffMappedResource *batch) {
    s32 recordIndex;

    for (recordIndex = 0; recordIndex < batch->count; recordIndex++) {
        sdfReleaseChipBlock(batch->records[recordIndex].status);
    }
    sdfReleaseResourceAllocation(batch->allocation);
    sdfReleaseChipBlock(batch);
    return 1;
}


typedef struct EffPackedResourceHeader {
    u8 pad00[0xC];
    u32 entriesOffset;
} EffPackedResourceHeader;

typedef struct EffPackedResourceEntry {
    u32 unk00;
    u32 resourceOffset;
} EffPackedResourceEntry;



u32 effReleaseSlotWorkAllocation(EffectSlotSet *owner) {
    sdfReleaseResourceAllocation(owner->workAllocation);
    return 1;
}

/* Return the normal slot address unless its stored alternate address is nonzero. */
void *effGetSlotWorkOrOverride(EffectSlotSet *owner, s32 slotIndex) {
    s32 alternateAddress;
    s32 entryAddress;

    entryAddress = slotIndex * EFF_SLOT_WORK_BYTES + (s32)owner->workEntries;
    alternateAddress = ((BdWork *)entryAddress)->alternate.address;
    if (alternateAddress != 0) {
        entryAddress = alternateAddress;
    }
    return (void *)entryAddress;
}

/* Start at zero when moving forward, otherwise at the full 16.16 endpoint. */
void effInitializeSlotPhase(EffTimedState *state) {
    if (state->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
        state->value = 0;
    } else {
        state->value = EFF_PHASE_FULL;
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", effInitializeSlotWorkFromDescription);

/* Initialize the normal slot entry at the unchanged 0xA0-byte stride. */
void effInitializeSlotWork(EffectSlotSet *owner, s32 slotIndex) {
    effInitializeSlotWorkFromDescription(owner, slotIndex, &owner->workEntries[slotIndex]);
}

/* Reset every normal work slot in source order. */
void effInitializeAllSlotWork(EffectSlotSet *owner) {
    u32 slotIndex = 0;
    if (owner->count != 0) {
        do {
            effResetSlotWork(owner, slotIndex++);
        } while (slotIndex < owner->count);
    }
}

/* Store the owner and slot index before invoking the work initializer. */
void effAttachSlotWorkOwner(EffectSlotSet *owner, s32 slotIndex, void *payload) {
    /* Alternate records share these owner/index fields without a full work allocation. */
    BdWork *entry = (BdWork *)payload;
    entry->owner = owner;
    entry->slotIndex = slotIndex;
    effInitializeSlotWorkFromDescription(owner, slotIndex, payload);
}

/* Clear the complete normal slot, then restore owner/index and initialize it. */
void effResetSlotWork(EffectSlotSet *owner, u32 slotIndex) {
    BdWork *entry;

    entry = &owner->workEntries[slotIndex];
    memset(entry, 0, EFF_SLOT_WORK_BYTES);
    effAttachSlotWorkOwner(owner, slotIndex, entry);
}


/* Acquire missing selected handles or clear slots; clearAllSlots does not release textures.
 * Return the address following the processed resource-table entries.
 */
u8 *effResolveResourceSlots(EffectSlotSet *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot) {
    u32 slotIndex = 0;
    u8 *entryBytes = resourceBytes;

    entryBytes += ((EffPackedResourceHeader *)entryBytes)->entriesOffset;

    if (set->textureCount != 0) {
        do {
            s32 *resourceData = (s32 *)(resourceBytes + ((EffPackedResourceEntry *)entryBytes)->resourceOffset);

            entryBytes += EFF_RESOURCE_TABLE_ENTRY_BYTES;
            if (clearAllSlots == 0) {
                if (selectedSlot == -1 || selectedSlot == slotIndex) {
                    if (set->textureReferences[slotIndex] == 0) {
                        set->textureReferences[slotIndex] = sdfTexAcquireResourceTexture(resourceData);
                    }
                } else {
                    set->textureReferences[slotIndex] = 0;
                }
            } else {
                set->textureReferences[slotIndex] = 0;
            }
            slotIndex++;
        } while (slotIndex < set->textureCount);
    }
    return entryBytes;
}

void effResolveAndReleaseResource(EffectSlotSet *owner) {
    if (owner->sourceAllocation != 0) {
        struct SdfMemBlock *resource = owner->sourceAllocation;
        u8 *mapped = (u8 *)sdfResourceRetainAddress(resource);
        effResolveResourceSlots(owner, mapped, 0, -1);
        sdfDecrementAllocationReferenceCount(owner->sourceAllocation);
    }
}

void effResolveAndReleaseSelectedResource(EffectSlotSet *owner, s32 mapping) {
    if (owner->sourceAllocation != 0) {
        struct SdfMemBlock *resource = owner->sourceAllocation;
        u8 *mapped = (u8 *)sdfResourceRetainAddress(resource);
        effResolveResourceSlots(owner, mapped, 0, mapping);
        sdfDecrementAllocationReferenceCount(owner->sourceAllocation);
    }
}

extern void sdfTexReleaseReference(SdfTex *texture);

void effReleaseSlotTextureReferencesAndResetWork(EffectSlotSet *owner, s32 preserveWork) {
    u32 i = 0;
    u32 count = owner->textureCount;
    u32 *resources;

    if (count != 0) {
        resources = (u32 *)owner->textureReferences;
        do {
            if (resources[i] != 0) {
                u32 *current;
                sdfTexReleaseReference((SdfTex *)resources[i]);
                current = (u32 *)owner->textureReferences;
                count = owner->textureCount;
                resources = current;
                current[i] = 0;
            }
            i++;
        } while (i < count);
    }
    if (!preserveWork) {
        effInitializeAllSlotWork(owner);
    }
}

void effReleaseTextureHandlesAndResetSlots(EffectSlotSet *owner) {
    effReleaseSlotTextureReferencesAndResetWork(owner, 0);
}

u8 effHasFirstTextureHandle(EffectSlotSet *owner) {
    return owner->textureReferences[0] != NULL;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
EffPayload *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    EffPayload *payload = (EffPayload *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(recordBytes);
    payload->count = recordCount;
    payload->allocation = allocation;
    payload->records = (u8 *)sdfResourceRetainAddress(allocation);
    memset(payload->records, 0, recordBytes);
    return payload;
}

/* Release the record allocation before freeing its small header. */
u32 effDestroyPayload(EffPayload *payload) {
    sdfReleaseResourceAllocation(payload->allocation);
    sdfReleaseChipBlock(payload);
    return 1;
}

EffectSlotSet *effCreateResourceSlotSetFromAllocation(struct SdfMemBlock *resourceAllocation, u32 keepAllocation) {
    EffectSlotSet *set;
    u8 *resource;
    u8 *entries;
    u32 index;
    u32 sourceOffset;

    set = (EffectSlotSet *)sdfAllocSizeClassBlock(0x30);
    memset(set, 0, 0x30);
    set->sharesTextureReferences = 0;
    set->sourceAllocation = keepAllocation != 0 ? resourceAllocation : 0;
    resource = (u8 *)sdfResourceRetainAddress(resourceAllocation);
    set->textureCount = *(u16 *)(resource + 0x14);
    set->textureAllocation = sdfAllocGeneralBlock(
        set->textureCount * 4);
    set->textureReferences = (SdfTex **)sdfResourceRetainAddress(set->textureAllocation);
    memset(set->textureReferences, 0, set->textureCount * 4);
    entries = effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->count = *(u16 *)(resource + 0x16);
    set->descriptionAllocation =
        sdfAllocGeneralBlock(set->count * 0x80);
    set->descriptions = (EffectSlotDescription *)sdfResourceRetainAddress(set->descriptionAllocation);
    set->workAllocation = sdfAllocGeneralBlock(set->count * 0xA0);
    set->workEntries = (BdWork *)sdfResourceRetainAddress(set->workAllocation);
    for (index = 0; index < set->count; index++) {
        sourceOffset = *(u32 *)(entries + 4);
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork(set, index);
        entries += 8;
    }
    return set;
}

EffectSlotSet *effCreateResourceSlotSet(EffectSlotSet *source, u32 slot, u32 count) {
    EffectSlotSet *effect = (EffectSlotSet *)sdfAllocSizeClassBlock(0x30);
    u32 index = 0;
    effect->sharesTextureReferences = 1;
    {
        u32 mode = source->textureCount;
        SdfTex **textureReferences = source->textureReferences;
        effect->textureCount = mode;
        effect->textureReferences = textureReferences;
    }
    effect->sourceAllocation = 0;
    effect->textureAllocation = 0;
    effect->count = count;
    effect->descriptionAllocation = sdfAllocGeneralBlock(count * 0x80);
    effect->descriptions = (EffectSlotDescription *)sdfResourceRetainAddress(effect->descriptionAllocation);
    effect->workAllocation = sdfAllocGeneralBlock(effect->count * 0xA0);
    effect->workEntries = (BdWork *)sdfResourceRetainAddress(effect->workAllocation);
    if (effect->count != 0) {
        do {
            memcpy(&effect->descriptions[index], &source->descriptions[slot], 0x80);
            effResetSlotWork(effect, index);
            index++;
        } while (index < effect->count);
    }
    return effect;
}

u32 effDestroyResourceSlotSet(EffectSlotSet *set) {
    if (set->sourceAllocation != 0) {
        sdfReleaseResourceAllocation(set->sourceAllocation);
    }
    if (set->sharesTextureReferences == 0) {
        effReleaseTextureHandlesAndResetSlots(set);
        sdfReleaseResourceAllocation(set->textureAllocation);
    }
    sdfReleaseResourceAllocation(set->descriptionAllocation);
    effReleaseSlotWorkAllocation(set);
    sdfReleaseChipBlock(set);
    return 1;
}

u32 effSetSlotResourceAndFlags(EffTimedState *effect, EffMappedRecord *resource, u32 flags) {
    effect->flags = flags;
    effect->source = resource;
    if ((flags & EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND) != 0) {
        effInitializeSlotPhase(effect);
    }
    effect->delay = effect->delay + 1;
    return 1;
}

u32 effSetSlotIndexedResource(EffTimedState *target, EffMappedResource *resources, s32 index,
                              u32 flags) {
    effSetSlotResourceAndFlags(target, &resources->records[index], flags);
    return 1;
}


u32 effSlotTransitionClearTarget(EffTimedState *work, u32 unused) {
    work->source = NULL;
    return 1;
}

s32 effClampSlotPhaseAtEnd(EffectSlotSet *owner, u32 slot, EffTimedState *state) {
    if (0x10000 < state->value) {
        u32 flags = state->flags;
        state->value = 0x10000;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                state->flags = flags & ~EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, slot);
            }
            return 0;
        }
    }
    return 1;
}

s32 effClampSlotPhaseAtStart(EffectSlotSet *owner, u32 slot, EffTimedState *state) {
    if (state->value < 0) {
        u32 flags = state->flags;
        state->value = 0;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                state->flags = flags | EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, slot);
            }
            return 0;
        }
    }
    return 1;
}

extern u32 effResetRecordRun(u8 *, u32, u32);

EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData) {
    /* Alternate payloads may be only 0x6C bytes; direct access here is limited to timed states.
     * Bucket callbacks retain their existing kind-specific pointer contract.
     */
    BdWork *entry = (BdWork *)entryData;
    EffTimedState *states = entry->states;
    BdWork *record = &effect->workEntries[slot];
    s32 idle = 1;
    u32 i;

    for (i = 0; i < 2; i++) {
        EffTimedState *state = &states[i];
        EffMappedRecord *source = state->source;

        if (source != 0 && source->category != 0) {
            EffRecordBucket *bucket = &D_00400508[source->category];
            s32 step = bucket->step(record, entry, state);

            if (state->delay > 0) {
                step = 0;
                state->delay -= 1;
            }
            if (step >= 0) {
                if (state->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
                    if (state->value != 0x10000) {
                        state->value += step;
                        idle = 0;
                        if (effClampSlotPhaseAtEnd(effect, slot, state) == 0) {
                            state->delay = state->delayMax;
                            return 0;
                        }
                    }
                } else if (state->value != 0) {
                    state->value -= step;
                    idle = 0;
                    if (effClampSlotPhaseAtStart(effect, slot, state) == 0) {
                        state->delay = state->delayMax;
                        return 0;
                    }
                }
            }
        }
    }
    if (idle != 0) {
        effResetRecordRun((u8 *)effect, slot, -1);
        return 0;
    }
    return effect;
}

s32 effSetSlotOverrideWork(EffectSlotSet *owner, u32 slot, void *payload) {
    if (owner->workEntries[slot].alternate.bits == 0) {
        effAttachSlotWorkOwner(owner, slot, payload);
    }
    owner->workEntries[slot].alternate.bits = (u32)payload;
    return 1;
}

u32 effSetMaterialSlots(EffectSlotSet *owner, s32 index, u32 materialFlags, void *payload) {
    u32 i;
    EffTimedState *states;
    if (owner->workEntries[index].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.payload = payload;
    /* Both timed states lie within the shared 0x6C-byte payload region. */
    states = ((BdWork *)payload)->states;
    for (i = 0; i < 2; i++) {
        states[i].materialFlags = materialFlags;
    }
    return 1;
}

u32 effClearSlotOverrideWork(s32 effect, s32 slot) {
    ((EffectSlotSet *)effect)->workEntries[slot].alternate.bits = 0;
    return 1;
}

s32 effConfigureSlotResource(u8 *effect, u32 slot, u32 resource, u32 flags) {
    BdWork *entry = &((EffectSlotSet *)effect)->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], (EffMappedRecord *)(u32)resource, flags);
    effUpdateTimedStates((EffectSlotSet *)effect, slot, entry);
    return 1;
}

s32 effConfigureIndexedSlotResource(EffectSlotSet *effect, u32 slot,
                                    EffMappedResource *resources, u32 index, u32 flags) {
    BdWork *entry = &effect->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], &resources->records[index], flags);
    effUpdateTimedStates(effect, slot, entry);
    return 1;
}

s32 effConfigureIndexedSlotMaterial(EffectSlotSet *effect, u32 slot,
                  EffMappedResource *resources, u32 index,
                  u32 materialFlags, u32 materialValue, u32 stateFlags) {
    BdWork *entry = &effect->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], &resources->records[index], stateFlags);
    effUpdateTimedStates(effect, slot, entry);
    entry->states[0].materialFlags = materialFlags;
    entry->states[0].materialValue = materialValue;
    return 1;
}

u32 effConfigureWithDefaultSetting(EffectSlotSet *effect, u32 slot,
                                   EffMappedResource *resources, u32 item,
                                   u32 materialFlags, u32 stateFlags) {
    effConfigureIndexedSlotMaterial(effect, slot, resources, item,
                                    materialFlags, 0, stateFlags);
    return 1;
}

u32 effResetRecordRun(u8 *table, u32 first, u32 arg) {
    u32 i = 0;
    u32 index;

    do {
        effSlotTransitionClearTarget((EffTimedState *)((first + i) * 0xA0 +
                                    (s32)((EffectSlotSet *)table)->workEntries + 0x28), arg);
        i++;
        index = first + i;
    } while (index < ((EffectSlotSet *)table)->count && (((EffectSlotSet *)table)->descriptions[index].flags & EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION));
    return 1;
}

extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);

void effSelectPresetByKind(u32 kind, u32 arg) {
    switch (kind) {
    case 0:
        sdfSubmitGsAlphaOneRegisterPacket(0x44, arg);
        return;
    case 1:
        sdfSubmitGsAlphaOneRegisterPacket(0x48, arg);
        return;
    case 2:
        sdfSubmitGsAlphaOneRegisterPacket(0x42, arg);
        break;
    }
}

typedef struct SdfTex SdfTex;
typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct EffSpriteUV {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} EffSpriteUV;

typedef union EffSpriteColor {
    u32 rgba;
    struct {
        u32 alpha : 8;
        u32 blue : 8;
        u32 green : 8;
        u32 red : 8;
    } channels;
} EffSpriteColor;

extern void sdfTexSetPrimaryBufferModeBits(SdfTex *, s32, s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern void effSelectPresetByKind(u32, u32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);

void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *color, u32 flipFlags,
                   u32 blendKind, s32 mode, SdfTex *texture, s32 surfaceId) {
    u32 textureCoordinates[4];
    void *packet;
    SdfListHead *packetList;
    u64 *packetWords;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 savedCoordinate;

    if (mode == 0) {
        sdfTexSetPrimaryBufferModeBits(texture, 0, 1);
    } else {
        sdfTexSetPrimaryBufferModeBits(texture, 1, 1);
    }
    packet = (void *)sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, 1));
    sdfConsInitPacketHeader(packet, 0x156, 5, 0x43431, 1);
    /* The SDK size helper also skips the packet's two header quadwords. */
    packetWords = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    left = x + 0x7000;
    top = y + 0x7900;
    textureCoordinates[0] = uvRect->u0 * 16;
    textureCoordinates[2] = uvRect->u1 * 16;
    textureCoordinates[1] = uvRect->v0 * 16;
    textureCoordinates[3] = uvRect->v1 * 16;
    right = left + width;
    bottom = top + height;
    if (flipFlags & 1) {
        savedCoordinate = left;
        left = right;
        right = savedCoordinate;
    }
    if (flipFlags & 2) {
        savedCoordinate = top;
        top = bottom;
        bottom = savedCoordinate;
    }
    if (color == NULL) {
        packetWords[0] = ((u64)0x80 << 32) | 0x80;
        packetWords[1] = ((u64)0x80 << 32) | 0x80;
    } else {
        u32 rgba = color->rgba;

        packetWords[0] = color->channels.red | ((u64)color->channels.green << 32);
        packetWords[1] = ((rgba >> 8) & 0xFF) | ((u64)(rgba & 0xFF) << 32);
    }
    packetWords[2] = textureCoordinates[0] | ((u64)textureCoordinates[1] << 32);
    packetWords[4] = (u64)(u32)left | ((u64)top << 32);
    packetWords[6] = textureCoordinates[2] | ((u64)textureCoordinates[3] << 32);
    packetWords[8] = (u64)(u32)right | ((u64)bottom << 32);
    packetWords[5] = depth;
    packetWords[9] = depth;
    effSelectPresetByKind(blendKind, surfaceId);
    packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packetList);
    sdfConsCreateDrawPacket(packetList, texture, 0);
    sdfAppendPacket(packetList, (u32)packet);
    effSubmitSurfacePacket(&kwlnDrawSurfaces[surfaceId], packetList);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

void func_00305EB0(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle, s32 mode) {
    f32 aspect = 1.75f;
    f32 radians = angle * 0.017453293f;
    f32 adjustedY;
    f32 rotatedX;
    f32 rotatedY;
    if (mode == 0) {
        aspect = 2.0f;
    }
    adjustedY = (s32)(y * aspect);
    rotatedX = sdfEvaluateCosineViaSinePhaseShift(radians) * x - sdfSinPoly(radians) * adjustedY;
    rotatedY = sdfSinPoly(radians) * x + sdfEvaluateCosineViaSinePhaseShift(radians) * adjustedY;
    if (rotatedX > 0.0f) {
        rotatedX += 0.1f;
    } else {
        rotatedX -= 0.1f;
    }
    if (rotatedY > 0.0f) {
        rotatedY += 0.1f;
    } else {
        rotatedY -= 0.1f;
    }
    *outX = (s32)rotatedX;
    *outY = (s32)rotatedY;
    *outX += centerX;
    *outY = (s32)(*outY / aspect + centerY);
}


INCLUDE_ASM(const s32, "game/code_002DE248", func_00306030);

extern void func_00306030(u32, u32, u32, u32, u32, const u32 *, const u32 *, u32,
                          f32, u32, u32, u32, u32, u32);

void itfDrawRotatedTexturedRect(u32 a, u32 b, u32 c, u32 d, u32 e,
                   const u32 *textureCoordinates, const u32 *cornerColors, u32 h,
                   f32 rotation, u32 x, u32 y, u32 width, u32 height) {
    func_00306030(a, b, c, d, e, textureCoordinates, cornerColors, h, rotation, x, y, 1, width, height);
}

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void effSelectPresetAndDispatch(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    effSelectPresetByKind(arg6, arg7);
    uiDrawGradientColorRect(arg0, arg1, arg2, arg3, arg4, (const u32 *)arg5, arg7);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, arg7);
}

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D170);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D180);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D190);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D1A0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D1B0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D1C0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D1D0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D1E0);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D1F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E60);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E64);

INCLUDE_SDATA(const s32, "game/code_002DE248", effCurrentRenderPacket);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E6C);

INCLUDE_SDATA(const s32, "game/code_002DE248", effWindTextureHandle);

INCLUDE_SDATA(const s32, "game/code_002DE248", effSharedRibbonReferenceCount);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E78);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E7C);

INCLUDE_SDATA(const s32, "game/code_002DE248", effScalyTextureHandle);

INCLUDE_SDATA(const s32, "game/code_002DE248", effSharedStripReferenceCount);

INCLUDE_SDATA(const s32, "game/code_002DE248", effSharedScalyStripResource);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E90);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E94);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E98);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EA0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EA8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EB0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EB4);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EB8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EC0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EC8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437ED0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437ED8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EE0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EE8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EF0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437EF8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F00);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F08);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F10);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F18);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F20);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F28);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F30);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F38);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F40);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F48);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F50);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F58);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F60);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F68);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F70);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F78);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F80);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F88);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F90);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437F98);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FA0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FA8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FB0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FB8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FC0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FC8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FD0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FD8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FE0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FE8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FF0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437FF8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438000);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438008);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438010);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438018);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438020);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438028);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438030);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438038);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438040);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438048);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438050);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438058);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438060);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438068);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438070);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438078);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438080);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438088);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438090);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438098);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004380F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438100);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438108);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438110);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438118);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438120);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438128);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438130);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438138);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438140);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438148);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438150);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438158);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438160);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438168);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438170);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438178);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438180);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438188);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438190);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438198);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004381F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438200);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438208);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438210);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438218);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438220);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438228);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438230);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438238);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438240);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438248);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438250);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438258);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438260);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438268);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438270);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438278);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438280);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438288);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438290);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438298);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004382F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438300);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438308);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438310);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438318);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438320);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438328);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438330);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438338);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438340);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438348);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438350);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438358);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438360);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438368);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438370);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438378);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438380);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438388);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438390);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438398);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004383F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438400);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438408);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438410);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438418);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438420);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438428);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438430);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438438);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438440);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438448);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438450);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438458);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438460);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438468);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438470);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438478);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438480);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438488);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438490);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438498);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004384F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438500);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438508);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438510);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438518);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438520);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438528);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438530);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438538);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438540);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438548);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438550);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438558);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438560);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438568);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438570);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438578);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438580);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438588);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438590);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438598);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004385F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438600);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438608);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438610);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438618);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438620);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438628);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438630);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438638);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438640);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438644);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438648);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438650);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438658);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438660);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438668);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438670);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438678);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438680);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438688);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438690);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438698);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", effAuxiliaryFileQueue);

INCLUDE_SDATA(const s32, "game/code_002DE248", effFileQueue);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386BC);

INCLUDE_SDATA(const s32, "game/code_002DE248", effQueuedFileHandle);

INCLUDE_SDATA(const s32, "game/code_002DE248", effTemporaryFileJob);

INCLUDE_SDATA(const s32, "game/code_002DE248", effCurrentFileQueueEntry);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386CC);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", effQueuedFileObject);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004386F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438700);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438708);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438710);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438718);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438720);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438728);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438730);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438738);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438740);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438748);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438750);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438758);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438759);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_0043875A);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_0043875B);

INCLUDE_SDATA(const s32, "game/code_002DE248", effResourceBankEntries);

INCLUDE_SDATA(const s32, "game/code_002DE248", effResourceBankDescriptor);

INCLUDE_SDATA(const s32, "game/code_002DE248", effFileQueueNameRecord);

INCLUDE_SDATA(const s32, "game/code_002DE248", effQueuedResourceNameRecord);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_0043876C);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438770);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438774);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438778);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_0043877C);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438780);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438788);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438790);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438798);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387A0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387A8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387AC);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387B0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387B4);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387B8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387BC);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387C0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387C8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387D0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387D8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387E0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387E8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387F0);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_004387F8);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438800);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438808);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438810);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438818);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438820);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438828);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438830);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438838);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438840);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438848);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438850);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438858);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00438860);

