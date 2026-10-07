#include "common.h"
#include "btl_state.h"
#include "evt_unit.h"
#include "eff_blur.h"
#include "eff_curve.h"
#include "file.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "eff_queue.h"
#include "mdl.h"
#include "sdf.h"
#include "eff.h"


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

extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfConsAppendVuPacket();
extern void sdfConsAppendAssetPacket();
extern void *func_0015FE20(EffPacketParams *);
extern u32 D_0037ECB0[];
extern SdfPoolNode *D_0037ECF0[];
extern SdfPoolNode *D_0037EE68[];
extern u32 D_0037EB30[];
extern SdfPoolNode *D_0037EB78[];
extern SdfPoolNode kwlnDrawSurfaces[];

extern char D_003B39C8[]; /* "/tool/effect/ep/" */

extern char D_003B39E0[]; /* "/tool/effect/" */

extern char D_003B3B88[]; /* "/tool/effect/mat/" */

extern char D_003B3BA0[]; /* "/tool/effect/hlp/" */

extern u32 sdfResourceRetainAddress(u32);

extern void *sdfAllocGeneralBlock(u32);

extern void effMiscQuaternionToMatrixVU(void);

extern s32 billGetFirstEntryFramePeriod(u32);

extern void billSetEntryFrameMode1(u32, s32);

extern f32 effComputeProjectedOffsetAngle(u8 *, void *);

extern f32 func_00152560(u8 *, void *);

extern void billSetChildScaleComponents(u32, f32, f32);

extern void billSetLengthExtent(u32, f32);

extern void effCopyVector(u32, u8 *);

extern void billInvokeCallback(u32);

extern u8 D_003B2B10[];

extern u32 D_003BD11C;

extern u32 D_003BD120;

extern u32 D_003BD128;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 D_003BD124;

extern s32 effQueuedResourceNameRecord;

extern s32 effResourceBankEntries;

extern s32 effResourceBankDescriptor;

extern s32 D_003BD098;

extern s32 effQueuedFileObject;

extern u8 D_003BD955;

extern s32 effTemporaryFileJob;

extern s32 effAuxiliaryFileQueue;

extern s32 btlGetRuntime(void);

extern void kwlnPadStartMotor(s32, u8, s32);

extern u32 effAllocateCopiedEffectPayload(u32, u32, s32);

extern void billDispatchByKind(void *);

extern u8 *effAllocateTexturedStripWork();

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern u32 effSharedScalyStripResource;

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern u32 effWindTextureHandle;

extern u32 D_003BC984;

extern u32 D_003BC994;

extern u32 sdfReadNamedResource(const char *, u32 *, s32);

extern u8 D_0038E000[];

extern u8 D_0038E570[];

extern u8 *D_0037EBF8[];

extern u32 effQueuedFileHandle;

extern u32 effCurrentRenderPacket;

extern s32 effSharedRibbonReferenceCount;

extern u32 D_003BC990;

extern u32 func_0029C230(u32);

extern u32 effFlashTextureHandles;

extern u32 billCloneObjectRetainingSharedData(u32);

extern void *fileResolvePrimaryBuffer();

extern u32 *fileResolveSecondaryBuffer(void *);

extern u32 effCreateModelResourceWithInlineData(u16, void *, void *, u32);

extern u32 effCreateResourceInstance(u16, void *, void *, u32);

extern void sdfTexReleaseReferenceViaHandler(u32);

extern u32 effCreateSurfaceGridNode(u32, u32);

extern void effFillSurfaceGridColorGradient(u32, u32 *);

extern void fileJobDestroy(u32);

extern u32 fileJobCreateFromJob(u32);

extern u32 fileJobCreateChild(u32);

extern void effReleaseSurfaceGridBuffers(s32);

extern s32 btlScanDirectory(s32, s32);

extern s32 func_00151FC0(void);

extern void btlAppendEntry(s32, char *, s32, s32, s32);

extern s32 btlCreateResourceDescriptor(s32);

extern void btlSetResourceNameHeaderPair(s32, s32, s32);

extern s32 *sdfCreateAssetWithDrawEntries(void);

extern void func_002DA420(void *, f32);

extern u16 D_003DC9E0[];

extern u32 effCreateTrackSetWithSharedReferences(u32, u32, u32);

extern void func_001FBA38(s32);

extern s32 func_001FBF48(s32);

extern s32 btlFormatSelectedResourceName(s32, void *);

extern s32 btlGetResourcePathVariant(s32);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 func_00296F58(const void *, const void *, s32, s32);

extern void func_00187C08(void *);

extern f32 func_00297270(EffScalarCurve *, s32, s32);

extern void effBlurDrawFramebufferQuad(void *);

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);


extern void effBlurStepScaleSlotsAndDraw(void *);

extern s32 effRequestResourceByMode(u32, u32, u32, void **);

extern s32 fileRequestIsReady(void *);

extern void func_00288788(void *);

extern void *fileQueuePlainDispatchRequest(u32);

extern void func_00288C50(void *);

extern void *func_002BD9C0(u32, u32);

/* Battle state: resource headers for texture slots 1 and 2. */

extern u32 D_003BC950;

extern u32 D_003BC954;


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

extern u8 btlIsRuntimeAllocated(void);

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

extern u32 D_003BD058;

extern u32 D_0038F2FC[];

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 refCount;      // 0x14 incremented with the global reference count
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern s32 D_003BC970[2];

extern RefObj *D_003BC978[2];

extern u32 effSharedTextureReferenceCount;

extern s32 D_003BC9B0[2];

extern s32 D_003BC9B8[2];

extern u8 D_003BC9C0[2];

extern u8 D_003BC9C8[2];

/* Native resource operations add cloning before the frame callbacks.
 * The final word is the copied payload size, not another callback.
 * Factories retain this unit's existing address-word interface. */
typedef struct EffResourceOps {
    void (*initialize)();      /* 0x00 */
    u32 (*createResource)();    /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
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

extern EffResourceOps D_0037E8A0[];

extern EffClassOps D_0037EAD0[];

extern EffClassOps D_0037EC50[];

extern EffResourceOps D_0037ED08[];

extern EffResourceOps D_0037ED90[];

extern EffResourceOps D_0037EEE0[];

extern EffClassOps effModelResourceOperations[];



/* Shared work object for the func_002BA538 helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

extern s32 func_002BA538(void *, s32, void *, void *);

extern u8 D_0038F9D0[];

extern u8 D_0038FAD0[];

extern u8 D_0038F8E8[];

extern u8 D_0038FA20[];

extern u8 D_0038F938[];

extern void effReleaseQueuedResourceName(void);

extern void effQueueResource(void *, void *);

extern u32 btlCreateResourceNameRecord();

extern void btlSetResourceNameHeaderPairAlternate(u32, u32, u32);

extern void btlResourceRecordSetName(u32, void *);

extern u8 D_003BD088[];

extern u8 D_003BD090[];

extern u8 D_003DF8D0[];

extern u8 D_0038F2B8[];

extern u8 D_003DF910[];

extern u8 *D_003BD074;

extern void *func_0029BD90(void *);

extern void func_002A6440(s32);

extern void func_002AB290(s32);

extern s32 func_002B7388(s32, void *, u32);

extern u8 D_003B3968[];

extern u8 D_003B3958[];

extern u8 D_003BCA40[];

extern u8 D_003BD050[];

extern u8 D_003B38C8[];

extern u8 D_003B3938[];

extern u8 D_003B3928[];

extern u8 D_003B3918[];

extern u8 D_003B3908[];

extern u8 D_003B38F8[];

extern u8 D_003B38E8[];

extern u8 D_003B38D8[];

extern u8 D_003B3888[];

extern u8 D_003BD000[];

extern u8 D_0038DE70[];

extern u8 D_0038DF50[];

extern u8 D_0038E0F0[];

extern u8 D_0038E1A0[];

extern u8 D_0038E280[];

extern u8 D_0038E240[];

extern u8 D_0038E310[];

extern u8 D_0038E450[];

extern u8 D_0038E500[];

extern u8 D_0038E620[];

extern u8 D_0038E6F0[];

extern u8 D_0038E7C0[];

extern u8 D_0038E800[];

extern u8 D_0038E9A0[];

extern u8 D_003DF840[];

extern u8 D_003DFA30[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

extern s32 func_002B9320(s32, s32, s32);

extern u8 D_003DE148[] __attribute__((aligned(4)));

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

typedef struct ValPtr34 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x28]; // 0x0C
    u16 *p34;          // 0x34
} ValPtr34; // 0x38

extern s32 effPollFileRecord(char *, s32);

extern void dds3DispatchIndexedCallback(u16 *, float);

/* Grid-sized effect object: width/height pair with an alternate pair at 0xB8. */
typedef struct EffGrid {
    u8 pad00[0x20]; // 0x00
    s32 width;         // 0x20
    s32 height;        // 0x24
    u8 pad28[0x90]; // 0x28
    s32 altHeight;     // 0xB8
} EffGrid; // 0xBC

/* Active grid record exposes its kind, instance count, billboard mode and source grid. */
typedef struct EffGridBillMode {
    u8 pad00[0x54];
    s16 mode;
} EffGridBillMode;

typedef struct EffGridRecord {
    u16 kind;
    u8 pad02[6];
    u32 count;
    u8 pad0C[0x14];
    EffGridBillMode *bill;
    EffGrid *params;
} EffGridRecord;

typedef struct EffSurfaceParams {
    u32 word[7];
} EffSurfaceParams;

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct EffKindDesc {
    u32 (*create)(void *);   // 0x00
    void (*destroy)();       // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(s32, s32); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} EffKindDesc; // 0x14

/* Native kind work: 0x40-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct EffKindWork {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;       // 0x10
    u32 color;      // 0x14
    f32 scale;      // 0x18
    s32 kind;       // 0x1C
    u32 frame;        // 0x20, incremented by the kind callback dispatcher
    u32 handle;     // 0x24
    void *payload;  // 0x28
    u32 sourceKind; // 0x2C
    u32 target;     // 0x30
    u8 pad_0x34[0xC]; // 0x34
} EffKindWork; // 0x40

/* Source payload's kind selects the initial rendering mode. */
typedef struct EffKindSource {
    u8 pad_00[0x28];
    s32 modeKind;
} EffKindSource;

extern EffKindDesc D_0037E770[];

extern EffKindDesc D_0037E7E8[];

extern u32 sdfTexAcquireResourceTexture(void *);

extern u32 effGetResourceFirstWord(u32);

/* VU0 model helpers consume vf10 directly, matching the original macro-mode setup. */
extern void func_002B0B70(u8 *, void *);

extern u16 D_003BC944;

extern MdlCtx *func_00217680(s32, s32);
extern u16 mdlGetContextResourceGroup(MdlCtx *);
extern u16 mdlGetContextResourceId(MdlCtx *);

extern void effInitModelVUState(MdlCtx *);

extern s32 effComputeLightDirectionVU(MdlCtx *, void *);

extern void *sdfAllocAndClearQuadwords(u32);

extern char D_003B2AA0[];

extern void fileQueueDestroy(u32);


void effDestroyModelContext(MdlCtx *model);

MdlCtx *effLoadViewerModelWithVUState(void *first, u32 second);

RefObj *effReferenceObjectRetain(RefObj *obj);

void effReleaseReferenceHolder(u8 *holder);

void effReleaseSharedReference(RefObj *obj);

RefObj *effRetainSharedReference(RefObj *obj);

u32 effDuplicateSmallHeader(const void *source) {
    u32 *buffer = (u32 *)sdfAllocSizeClassBlock(12);
    *buffer = 0;
    memcpy(buffer + 1, source, 8);
    return (u32)buffer;
}

void effCreateSmallHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer(work);
    effDuplicateSmallHeader(resource);
}

void effReleaseFadeHeaderAllocation(u32 allocation) {
    kwlnCancelConfiguredFadeFrames();
    sdfReleaseChipBlock(allocation);
}

void effCloneSmallHeaderFromWork(s32 work) {
    effDuplicateSmallHeader((const void *)(work + 4));
}

void effFadeFrameReset(u32 *counter) {
    *counter = 0;
}

void effFadeFrameAdvance(s32 *counter) {
    s32 frame;

    frame = *counter;
    if (frame == 0) {
        kwlnFadeSetupFrames(counter[1], counter[2]);
        frame = *counter;
    }
    *counter = frame + 1;
}

/* 0x18-byte effect header followed by a copied 0x40-byte fade payload. */
typedef struct EffFadeVectorWork {
    u8 vector[0x10];
    u32 frame;
    u32 color;
    u8 source[0x40];
} EffFadeVectorWork;

u8 *effCreateFadeVectorWork(source)
const u8 *source;
{
    u8 *effect = (u8 *)sdfAllocSizeClassBlock(0x58);
    memset(effect, 0, 0x58);
    VU0_STORE_VF($vf0, effect);
    memcpy(effect + 0x18, source, 0x40);
    return effect;
}

void effCreateFadeVectorFromFile(void *work) {
    const u8 *resource;

    resource = fileResolvePrimaryBuffer(work);
    effCreateFadeVectorWork(resource);
}

void effFreeFadeVectorWork(void) {
    sdfReleaseChipBlock();
}

void effCloneFadeVectorWork(s32 work) {
    effCreateFadeVectorWork(((EffFadeVectorWork *)work)->source);
}

void effResetFadeVectorFrame(s32 work) {
    ((EffFadeVectorWork *)work)->frame = 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_0029C748);

void effCopyFadeWorkVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetFadeVectorColor(s32 work, u32 value) {
    ((EffFadeVectorWork *)work)->color = value;
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
    return func_002CEAE8(work);
}

SdfFlagListWork *effCreateSelectionFlagListFromFile(void *file) {
    return sdfInitializeFlagListFromResource(file);
}

void effReleaseSelectionFlagList(SdfFlagListWork *work) {
    sdfReleaseFlagListResource(work);
}

SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *work) {
    return effCreateSelectionFlagListFromWork(&work->params);
}

void effResetSelectionEntriesAndState(SdfFlagListWork *p) {
    effResetSelectionEntryBuffers(p);
    p->frame = 0;
}

void effUpdateSelectionEntryState(SdfFlagListWork *work) {
    func_002CEC40(work);
}

void effDrawSelectionEntryVectors(SdfFlagListWork *work) {
    func_002CF248(work);
}

void effUpdateAndDrawSelectionEntries(SdfFlagListWork *work) {
    effUpdateSelectionEntryState(work);
    effDrawSelectionEntryVectors(work);
}

void func_0029CF30(SdfFlagListWork *work, u32 value) {
    work->unk04 = value;
}

u32 effDuplicatePayloadHeader(const void *source) {
    u32 *buffer = (u32 *)sdfAllocSizeClassBlock(0x14);
    *buffer = 0;
    memcpy(buffer + 1, source, 16);
    return (u32)buffer;
}

void effCreateSelectionHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer(work);
    effDuplicatePayloadHeader(resource);
}

void effReleaseSelectionHeaderAllocation(u32 allocation) {
    evtDestroySelectionState();
    sdfReleaseChipBlock(allocation);
}

void effCloneSelectionHeaderFromWork(s32 work) {
    effDuplicatePayloadHeader((const void *)(work + 4));
}

void effSelectionFrameReset(u32 *counter) {
    *counter = 0;
}

void effSelectionFrameAdvance(s32 *counter) {
    s32 frame;

    frame = *counter;
    if (frame == 0) {
        evtSelStateCreate(counter[1], (s16)counter[2], counter[3], counter[4]);
        frame = *counter;
    }
    *counter = frame + 1;
}

typedef struct EffFadeOut {
    u32 color;    // 0x00
    u32 param;    // 0x04
    u32 unk8;     // 0x08
    u32 unkC;     // 0x0C
    s32 unk10;    // 0x10
    s32 unk14;    // 0x14
} EffFadeOut;



typedef struct EffFadeConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 progress;         /* 0xB8 */
    u8 padBC[4];
    EffFadeOut out;       /* 0xC0 */
} EffFadeConfig;

typedef struct EffMapOut {
    u8 pad_00[0xC];
    u32 color;  // 0x0C
    u32 param;  // 0x10
    f32 rateB;  // 0x14
    f32 rateA;  // 0x18
    s32 posX;   // 0x1C
    s32 posY;   // 0x20
    s32 mode;   // 0x24
} EffMapOut;

/* Same as EffMapOut with one more word before the rate/position fields. */
typedef struct EffMapOutWide {
    u8 pad_00[0xC];
    u32 color;  // 0x0C
    u32 param;  // 0x10
    f32 rateB;  // 0x14
    u32 unk18;
    f32 rateA;  // 0x1C
    s32 posX;   // 0x20
    s32 posY;   // 0x24
    s32 mode;   // 0x28
    u32 target; // 0x2C, retained kind-linked texture/resource
} EffMapOutWide;


typedef struct EffRateOut {
    u32 color;   // 0x00
    u32 param;   // 0x04
    f32 rateB;   // 0x08
    f32 rateA;   // 0x0C
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    s32 unk20;
    union {
        s32 unk24;
        u32 target;
    };
} EffRateOut;

typedef struct EffRateConfig {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    SdfColorTrack blendA;   /* 0x00 */
    SdfAlphaTrack blendB2;  /* 0x24 */
    EffScalarCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffScalarTrack rateA;   /* 0x60 */
    EffScalarTrack rateB;   /* 0x8C */
    s32 progress;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    EffRateOut out;       /* 0xC0 */
} EffRateConfig;

/* Draw a fade rectangle when progress is zero or reaches the signed frame limit.
 * Blend its packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 progress = config->progress;
    s32 limit = 0;
    EffRateOut *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress != 0) {
        limit = work->frame;
    }
    if (progress < limit) {
        return;
    }
    out->unk10 = 0;
    out->unk14 = 0;
    out->unk18 = 0;
    out->unk1C = 0;
    out->unk20 = 0x200;
    out->unk24 = 0x1C0;
    second = func_00296F58(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_00297270(&config->blendB, limit, progress) * 0.01f + 1.0f;
    out->rateB = func_00297270(&config->rateA.curve, limit, progress) * 0.01f;
    out->param = work->mode;
    effDrawBlurRectangle(out);
}

void effCreateFadeBlendWorkFromOutput(s32 work) {
    effCloneBlurTemplate(work + 0xc0);
}

void effReleaseFadeBlendWork(void) {
    effReleaseBlurTemplate();
}

typedef struct EffMapOutB {
    s32 mode;   // 0x00
    u32 color;  // 0x04
    u32 param;  // 0x08
    f32 rateB;  // 0x0C
    f32 rateA;  // 0x10
    s32 posX;   // 0x14
    s32 posY;   // 0x18
} EffMapOutB;

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffMapOutB *out = (EffMapOutB *)work->handle;
    s32 progress = config->progress;
    s32 limit = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress != 0) {
        limit = work->frame;
    }
    if (progress < limit) {
        return;
    }
    rate = func_00297270(&config->rateB.curve, limit, progress);
    if (config->fixedMode != 0) {
        out->posX = 0;
        out->posY = 0;
        out->mode = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->mode = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->posX = px;
        out->posY = py << 1;
    }
    second = func_00296F58(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_00297270(&config->blendB, limit, progress) * 0.01f + 1.0f;
    out->rateB = func_00297270(&config->rateA.curve, limit, progress) * 0.01f;
    out->param = work->mode;
    effDrawBlurFixedPointRectangle(out);
}

void effUpdateTarget(EffKindWork *work, u32 target) {
    u32 previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexReleaseReferenceViaHandler(previous);
        }
        work->target = target;
    }
    ((EffMapOutWide *)work->handle)->target = target;
}

u32 effCreateFixedSlotBlurWorkFromFadeOutput(void *source) {
    return (u32)func_00186F90((u8 *)source + 0xC0);
}

void effReleaseFixedSlotBlurWork(void *handle) {
    effBlurReleaseFirstResource(handle);
}

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffMapOut *out = (EffMapOut *)work->handle;
    s32 progress = config->progress;
    s32 limit = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress != 0) {
        limit = work->frame;
    }
    if (progress < limit) {
        return;
    }
    rate = func_00297270(&config->rateB.curve, limit, progress);
    if (config->fixedMode != 0) {
        out->mode = (s32)rate;
        out->posX = 0;
        out->posY = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->mode = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF(vf10, pos);
        out->posX = (s32)pos[0] - 0x800;
        out->posY = ((s32)pos[1] - 0x800) << 1;
    }
    second = func_00296F58(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_00297270(&config->blendB, limit, progress) * 0.01f;
    out->rateB = func_00297270(&config->rateA.curve, limit, progress) * 0.01f;
    out->param = work->mode;
    func_00187098((EffBlurScatterWork *)out);
}

void effTextureReferenceRelease(EffKindWork *work, u32 target) {
    u32 previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexReleaseReferenceViaHandler(previous);
        }
        work->target = target;
    }
    ((EffMapOutWide *)work->handle)->target = target;
}

void effCreateVariableSlotBlurWorkFromFadeOutput(s32 work) {
    effCloneBlurWorkWithSlots(work + 0xc0);
}

void effReleaseVariableSlotBlurWork(void) {
    effBlurReleaseSecondResource();
}

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and progress gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffMapOutWide *out = (EffMapOutWide *)work->handle;
    s32 progress = config->progress;
    s32 limit = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress != 0) {
        limit = work->frame;
    }
    if (progress < limit) {
        return;
    }
    rate = func_00297270(&config->rateB.curve, limit, progress);
    if (config->fixedMode != 0) {
        out->mode = (s32)rate;
        out->posX = 0;
        out->posY = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->mode = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF(vf10, pos);
        out->posX = (s32)pos[0] - 0x800;
        out->posY = ((s32)pos[1] - 0x800) << 1;
    }
    second = func_00296F58(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_00297270(&config->blendB, limit, progress) * 0.01f;
    out->rateB = func_00297270(&config->rateA.curve, limit, progress) * 0.01f;
    out->param = work->mode;
    effBlurStepScaleSlotsAndDraw(out);
}

void effReplaceKindLinkedTarget(EffKindWork *work, u32 target) {
    u32 previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexReleaseReferenceViaHandler(previous);
        }
        work->target = target;
    }
    ((EffMapOutWide *)work->handle)->target = target;
}

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    s32 progress = config->progress;
    s32 limit = 0;
    EffRateOut *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress != 0) {
        limit = work->frame;
    }
    if (progress < limit) {
        return;
    }
    out->unk10 = 0;
    out->unk14 = 0;
    out->unk18 = 0;
    out->unk1C = 0;
    out->unk20 = 0x200;
    out->unk24 = 0x1C0;
    second = func_00296F58(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_00297270(&config->blendB, limit, progress) + 1.0f;
    out->rateB = func_00297270(&config->rateA.curve, limit, progress);
    out->param = work->mode;
    effBlurDrawFramebufferQuad(out);
}

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(EffKindWork *work) {
    EffFadeConfig *config = work->payload;
    s32 progress = config->progress;
    s32 limit = 0;
    EffFadeOut *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress != 0) {
        limit = work->frame;
    }
    if (progress < limit) {
        return;
    }
    out->unk8 = 0;
    out->unkC = 0;
    out->unk10 = 0x200;
    out->unk14 = 0x1C0;
    second = func_00296F58(&config->blendA, &config->blendB2, limit, progress);
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
    out->param = work->mode;
    func_00187C08(out);
}

void effCreateFadeColorWorkFromOutput(s32 work) {
    effCloneResourceTemplate(work + 0xc0);
}

void effReleaseFadeColorWork(void) {
    func_00188050();
}

/* This projected fade also consumes EffKindWork: position, handle and payload. */
INCLUDE_ASM(const s32, "game/code_0029C530", func_0029DBA8);

void effReplaceLinkedKindWorkTarget(EffKindWork *work, u32 target) {
    u32 previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexReleaseReferenceViaHandler(previous);
        }
        work->target = target;
    }
    ((EffRateOut *)work->handle)->target = target;
}

EffKindWork *effAllocateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037E770[kind].payloadSize;
    EffKindWork *work = sdfAllocAndClearQuadwords(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF($vf0, work);
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
    if (D_0037E770[kind].create != NULL) {
        work->handle = D_0037E770[kind].create(source);
    }
    return work;
}

EffKindWork *effCreateKindWorkFromFile(FileJob *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateKindWork(work->option, source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_0037E770[effect->kind].initialize != NULL) {
            u32 kind = work->slots[0].selector;
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = sdfTexAcquireResourceTexture(secondary);
                break;
            case 4:
                effect->target = effGetResourceFirstWord(secondary[0]);
                break;
            }
            D_0037E770[effect->kind].initialize(effect, effect->target);
        }
    }
    return effect;
}

void effReleaseLinkedTarget(EffKindWork *work) {
    void *handle = (void *)work->handle;
    if (handle != NULL) {
        D_0037E770[work->kind].destroy(handle);
    }
    if (work->target != 0 && work->sourceKind != 4) {
        sdfTexReleaseReferenceViaHandler(work->target);
    }
    sdfReleaseChipBlock(work);
}

void effCloneKindWork(EffKindWork *work) {
    effAllocateKindWork((u16)work->kind, work->payload);
}

void effKindWorkFrameReset(EffKindWork *work) {
    work->frame = 0;
}

void effAdvanceKindWorkFrame(EffKindWork *work) {
    D_0037E770[work->kind].update((void *)work);
    work->frame = work->frame + 1;
}

void effCopyKindWorkPosition(void *dst, void *src) {
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
    u32 headerSize = 0x40;
    u32 size = D_0037E7E8[kind].payloadSize;
    EffKindWork *work = sdfAllocAndClearQuadwords(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF($vf0, work);
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
    if (D_0037E7E8[kind].create != NULL) {
        work->handle = D_0037E7E8[kind].create(source);
    }
    return work;
}

EffKindWork *effCreateKindWorkFromFileB(FileJob *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateAlternateKindWork(work->option, source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_0037E7E8[effect->kind].initialize != NULL) {
            u32 kind = work->slots[0].selector;
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = sdfTexAcquireResourceTexture(secondary);
                break;
            case 4:
                effect->target = effGetResourceFirstWord(secondary[0]);
                break;
            }
            D_0037E7E8[effect->kind].initialize(effect, effect->target);
        }
    }
    return effect;
}

/* Release the alternate-kind handle, owned secondary reference, and work allocation. */
void effReleaseAlternateKindWork(EffKindWork *work) {
    void *handle = (void *)work->handle;
    if (handle != NULL) {
        D_0037E7E8[work->kind].destroy(handle);
    }
    if (work->target != 0 && work->sourceKind != 4) {
        sdfTexReleaseReferenceViaHandler(work->target);
    }
    sdfReleaseChipBlock(work);
}

/* Reallocate the source kind and payload; this path does not copy its secondary target. */
void effCloneAlternateKindWork(EffKindWork *work) {
    effAllocateAlternateKindWork((u16)work->kind, work->payload);
}

/* Reset the alternate kind-table work's frame counter. */
void effAlternateKindWorkFrameReset(EffKindWork *work) {
    work->frame = 0;
}

/* Invoke the alternate kind-table update callback, then advance its frame counter. */
void effAlternateKindWorkFrameUpdate(EffKindWork *work) {
    D_0037E7E8[work->kind].update((void *)work);
    work->frame = work->frame + 1;
}

/* Copy the leading vector of alternate kind-table work. */
void effCopyAlternateKindVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Set the alternate kind-table work's packed color. */
void effSetAlternateKindColor(EffKindWork *work, u32 color) {
    work->color = color;
}

/* Set the alternate kind-table work's scalar scale. */
void effSetAlternateKindScale(EffKindWork *work, float scale) {
    work->scale = scale;
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
    u32 billboard;   /* 0x64 */
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
    memcpy(work + 0x2C, fileResolvePrimaryBuffer(source),
           ((FileJob *)source)->slots[0].size);
    ((EffBillboardWork *)work)->billboard =
        billCreateIndexed(1, fileResolveSecondaryBuffer(source));
    return work;
}

void effBillboardWorkRelease(u32 work) {
    s32 billboard;

    billboard = ((EffBillboardWork *)work)->billboard;
    if (billboard != 0) {
        billDispatchByKind(billboard);
    }
    sdfReleaseChipBlock(work);
}

u8 *effDuplicateBillState(const u8 *source) {
    u8 *effect = (u8 *)effCreateBillboardWork(NULL);
    memcpy(effect + 0x2C, source + 0x2C, 0x38);
    effReplaceBillboardClone((s32)effect, (s32)source);
    return effect;
}

void effReplaceBillboardClone(s32 dst, s32 src) {
    u32 billboard;

    if (((EffBillboardWork *)dst)->billboard != 0) {
        billDispatchByKind(((EffBillboardWork *)dst)->billboard);
    }
    billboard = billCloneObjectRetainingSharedData(((EffBillboardWork *)src)->billboard);
    ((EffBillboardWork *)dst)->billboard = billboard;
}

void effBillboardEntryFrameReset(s32 work) {
    billSetEntryFrameMode1(((EffBillboardWork *)work)->billboard, 0);
    ((EffBillboardWork *)work)->frame = 0;
}


/* Update a live billboard frame's projected scale, length, and position before its callback. */
void effUpdateScaledBillboardFrame(EffBillboardWork *work) {
    u128 vec;
    f32 length;
    f32 scale;
    f32 width;
    f32 height;
    s32 frame;
    s32 limit;

    limit = billGetFirstEntryFramePeriod(work->billboard);
    frame = work->frame;
    if (frame < limit) {
        billSetEntryFrameMode1(work->billboard, frame);
        VU0_LOAD_VF(vf10, work->rotation);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003B2B10);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, &vec);
        length = effComputeProjectedOffsetAngle((u8 *)work, &vec);
        scale = func_00152560((u8 *)work, &vec);
        if (scale < 0.3f) {
            scale = 0.3f;
        }
        scale *= work->scale;
        width = scale * work->widthScale;
        height = work->heightScale * work->scale;
        billSetChildScaleComponents(work->billboard, width, height);
        billSetLengthExtent(work->billboard, length);
        effCopyVector(work->billboard, (u8 *)work);
        billInvokeCallback(work->billboard);
        work->frame++;
    }
}

void effCopyBillboardPosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyBillboardOrientation(void *work, void *src) {
    PCP_COPY_VECTOR(((EffBillboardWork *)work)->rotation, src);
}

void effSetBillboardColor(s32 work, u32 color) {
    ((EffBillboardWork *)work)->color = color;
}

/* Set the billboard's scalar scale; the leading vectors are not a Matrix4. */
void effSetBillboardMatrixComponent(EffBillboardWork *work, float value) {
    work->scale = value;
}

/* Frame builder owns an entry array, a resource, and its allocation handle. */
typedef struct EffFrameState {
    u8 *entries;        // 0x00
    u8 *asset;          // 0x04
    u32 allocation;     // 0x08
} EffFrameState;

/* Per-instance frame storage and transform rows, as accessed by reset/initialization. */
typedef struct EffFrameAsset {
    u8 pad_00[8];       // 0x00
    u32 frameCount;     // 0x08
    u8 pad_0C[0xC];     // 0x0C
    s32 *animationFrames; // 0x18
    void *frameStorage; // 0x1C
    f32 *transformRows; // 0x20
    u32 pad_24;
    u32 *colorRows; // 0x28
} EffFrameAsset;

/* Frame-reset callbacks read the saved frame state and its configuration. */
typedef struct EffBillFrameWork {
    u8 pad00[0x30];
    u8 *frameState; /* 0x30 */
    u8 *config;     /* 0x34 */
} EffBillFrameWork;

/* Billboard configuration shared by the frame builders and draw callbacks. */
typedef struct EffBillConfig {
    u8 pad_00[0x28];
    u32 textureId;      // 0x28, copied into the output record
    u8 pad_2C[8];
    union {
        struct {
            u32 progress; /* 0x34, frame-animation variant */
            union {
                u32 count;
                s32 signedCount;
            } frames; /* 0x38 */
            u8 outputMode; /* 0x3C */
            u8 pad_3D[0x19];
            u8 mode; /* 0x56 */
            u8 pad_57;
        };
        /* The quantized-mesh resource copies a 0x98-byte configuration;
         * its draw callback samples this scalar track at 0x34. */
        EffScalarCurve scaleCurve;
    };
    u8 pad_58[0x18];
    u32 drawProgress;    // 0x70, progress of the mesh-draw variant
    union {
        u32 quantizedSamples; // 0x74, clamped to at least four
        s32 signedRows;
    } samples;
    f32 fadeInEnd;     // 0x78, ramp-up length as a fraction of `resourceId`
    f32 fadeOutStart;  // 0x7C, start of the ramp-down
    u8 pad_80[8];
    f32 rowOffset;        // 0x88, row spacing or circular radius
    u32 resourceId;     // 0x8C
    u8 pad_90[4];
    u8 meshMode;        // 0x94, copied to the mesh output
    u8 pad_95[0x24];
    u8 alternateMode;   // 0xB9, animation variants use this instead of mode
    u8 pad_BA[0x23];
    u8 alternateMeshMode; // 0xDD, copied to the other mesh-draw variant
} EffBillConfig;

void effClearBillFrames(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}


u8 *effCreateBillFrameNode(u8 *config, u32 resource) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(count, 0, resource);
    return node;
}

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_0029E898);

extern u8 D_0037E0E0[];

extern void func_002A3E10(u8 *, void *);

typedef struct EffBillOutput {
    u32 textureId;      // 0x00
    u32 color;          // 0x04
    u32 field_08;       // 0x08
    u8 outputMode;      // 0x0C
    u8 pad_0D[7];
    u8 mode;            // 0x14
} EffBillOutput;

typedef struct EffMeshOutput {
    u32 field_00;       // 0x00
    u32 textureId;      // 0x04
    u32 color;          // 0x08
    u8 pad_0C[8];
    u8 mode;            // 0x14
} EffMeshOutput;

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

void billUpdateFrameDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void billResetCellIndices(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x1C;
        }
    }
}

u8 *billCreateCellNode(u8 *config, u32 resource) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(count * 0x1C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(count, 1, resource);
    return node;
}

void billReleaseCellNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_0029F168);

void billUpdateCellDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void billResetParticleIndices(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x2C;
        }
    }
}

u8 *billCreateParticleNode(u8 *config, u32 resource) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(count, 1, resource);
    return node;
}

void billReleaseParticleNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_0029FB48);

void billUpdateParticleDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void effClearAnimatedFrames(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *billAllocateAnimatedTransformEntries(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x18 + headerSize);
    u8 *node = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *entries = node + headerSize;

    *(u8 **)(node + 8) = base;
    *(u8 **)node = entries;
    if (((EffBillConfig *)config)->drawProgress == 0) {
        ((EffBillConfig *)config)->drawProgress = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *node, u8 *config) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = ((EffFrameAsset *)((EffFrameState *)node)->asset)->transformRows;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateAnimatedTransform(u8 *config, u32 resource) {
    u8 *node = billAllocateAnimatedTransformEntries(config);
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(((EffBillConfig *)config)->frames.count, 3, resource);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

extern u8 *billAllocEmitterNode(u8 *);

extern u8 *billAllocQuadNode(u8 *);

extern u32 effDuplicateResourceRefs(u32);

extern void billInitializeEmitterRows(u8 *, u8 *);

extern void billInitializeQuadRows(u8 *, u8 *);

u8 *billCloneAnimatedTransform(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = billAllocateAnimatedTransformEntries(config);
    ((EffFrameState *)node)->asset = (u8 *)effDuplicateResourceRefs((u32)((EffFrameState *)state)->asset);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

void billReleaseAlternatingTransformNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A0638);

void billUpdateAlternatingDrawColorAndTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void billResetEmitterIndices(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *billAllocEmitterNode(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    return node;
}

void billInitializeEmitterRows(u8 *node, u8 *config) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = ((EffFrameAsset *)((EffFrameState *)node)->asset)->transformRows;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateEmitterTransform(u8 *config, u32 resource) {
    u8 *node = billAllocEmitterNode(config);
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(((EffBillConfig *)config)->frames.count, 4, resource);
    billInitializeEmitterRows(node, config);
    return node;
}

u8 *billCloneEmitterTransform(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = billAllocEmitterNode(config);
    ((EffFrameState *)node)->asset = (u8 *)effDuplicateResourceRefs((u32)((EffFrameState *)state)->asset);
    billInitializeEmitterRows(node, config);
    return node;
}

void billReleaseEmitterNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A0FA0);

void billUpdateEmitterDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void effClearStripFrames(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x28;
        }
    }
}

u8 *billAllocStripNode(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x28 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    return node;
}

void billInitializeStripRows(u8 *node, u8 *config) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = ((EffFrameAsset *)((EffFrameState *)node)->asset)->transformRows;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

extern u8 *billAllocStripNode(u8 *);

u8 *billCreateStripTransform(u8 *config, u32 resource) {
    u8 *node = billAllocStripNode(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(((EffBillConfig *)config)->frames.count, 3, resource);
    billInitializeStripRows(node, config);
    return node;
}

u8 *billCloneStripTransform(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = billAllocStripNode(config);
    ((EffFrameState *)node)->asset = (u8 *)effDuplicateResourceRefs((u32)((EffFrameState *)state)->asset);
    billInitializeStripRows(node, config);
    return node;
}

void billReleaseStripNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A1948);

void billUpdateStripDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void billResetTrailIndices(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x2C;
        }
    }
}

u8 *billCreateTrailNode(u8 *config, u32 resource) {
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x2C + headerSize);
    u8 *cursor = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *header = cursor;
    /* Read the configured count before writing the retained header. */
    u32 frameCount = ((EffBillConfig *)config)->frames.count;

    cursor += headerSize;
    *(u8 **)(header + 8) = base;
    *(u8 **)header = cursor;
    ((EffFrameState *)header)->asset = (u8 *)effCreateTrackSetWithSharedReferences(frameCount, 0, resource);
    return header;
}

void billReleaseTrailNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A22B8);

void billUpdateTrailDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

void billResetQuadIndices(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *asset = ((EffFrameState *)state)->asset;
    s32 remaining = ((EffBillConfig *)config)->frames.signedCount;
    u8 *entry = ((EffFrameState *)state)->entries;

    memset(((EffFrameAsset *)asset)->frameStorage, 0, ((EffFrameAsset *)asset)->frameCount * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x20;
        }
    }
}

u8 *billAllocQuadNode(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x20 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    return node;
}

void billInitializeQuadRows(u8 *node, u8 *config) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = ((EffFrameAsset *)((EffFrameState *)node)->asset)->transformRows;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateQuadTransform(u8 *config, u32 resource) {
    u8 *node = billAllocQuadNode(config);
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(((EffBillConfig *)config)->frames.count, 4, resource);
    billInitializeQuadRows(node, config);
    return node;
}

u8 *billCloneQuadTransform(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = billAllocQuadNode(config);
    ((EffFrameState *)node)->asset = (u8 *)effDuplicateResourceRefs((u32)((EffFrameState *)state)->asset);
    billInitializeQuadRows(node, config);
    return node;
}

void billReleaseQuadNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A2E18);

void billUpdateQuadDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002A3E10(out, mtx);
}

/* Common header of class-dispatched billboard/resource work (0x40-byte prefix). */
typedef struct EffClassWork {
    union {
        u8 transform[0x20];
        struct {
            f32 position[4];
            f32 orientation[4];
        } vectors;
    };
    f32 scale;           // 0x20
    u32 color;           // 0x24
    u32 frame;           // 0x28
    s32 kind;            // 0x2C
    u32 resource;        // 0x30
    void *payload;       // 0x34
    u8 pad_38[8];
} EffClassWork;

/* Four-byte owner header followed by its class-work pointer array. */
typedef struct EffClassWorkList {
    EffClassWork **entries;
} EffClassWorkList;

u8 *effAllocateActiveInstanceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037E8A0[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return effect;
}

/* Render instance owns either a billboard or a reference, plus an asset slot. */
typedef struct EffRenderResourceState {
    u8 pad_00[0x58];
    s16 billMode;       // 0x58
    u8 pad_5A[0x6E];
    u32 billHandle;     // 0xC8
    RefObj *reference;  // 0xCC
    u32 assetHandle;    // 0xD0
} EffRenderResourceState;

/* The ring source selects a minimum of three segments and repeats its three colors. */
typedef struct EffRingSource {
    u8 pad_00[0x38];
    u32 segments;       // 0x38
    u8 pad_3C[8];
    u32 firstColor;     // 0x44
    u32 middleColor;    // 0x48
    u32 lastColor;      // 0x4C
} EffRingSource;

/* Point-set node: `rows` 16-byte entries in `buffer`, then `tail`. */
typedef struct EffPointSet {
    u32 type;       // 0x00
    u32 color;      // 0x04
    s32 rows;       // 0x08
    u8 flag;        // 0x0C
    u8 pad_0D[3];
    u8 *buffer;     // 0x10
    u8 *tail;       // 0x14
    s32 *handle;    // 0x18
    u8 *allocation; // 0x1C
} EffPointSet;

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

u8 *effCreateResourceInstanceA(u16 kind, void *source, u32 option) {
    u8 *effect = effAllocateActiveInstanceWork(kind, source);
    ((EffClassWork *)effect)->resource = D_0037E8A0[kind].createResource(source, option);
    D_0037E8A0[kind].initialize(effect);
    return effect;
}

u8 *effCreateFileResourceInstance(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer(work);
    void *source;
    switch (((FileJob *)work)->slots[0].selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(work);
    return effCreateResourceInstanceA(((FileJob *)work)->option, source, (u32)secondary);
}

void effDispatchDestroyOp(EffClassWork *work) {
    D_0037E8A0[work->kind].destroyResource((void *)work->resource);
    sdfReleaseChipBlock(work);
}

u8 *effCreateActiveResource(EffClassWork *work) {
    u8 *effect;
    if (D_0037E8A0[work->kind].cloneResource == NULL) {
        effect = effCreateResourceInstanceA(work->kind, work->payload, 0);
    } else {
        u32 clonedResource;
        s32 kind;
        effect = effAllocateActiveInstanceWork(work->kind, work->payload);
        clonedResource = D_0037E8A0[work->kind].cloneResource(work);
        /* Capture the clone's class before publishing its resource. */
        kind = work->kind;
        ((EffClassWork *)effect)->resource = clonedResource;
        D_0037E8A0[kind].initialize(effect);
    }
    return effect;
}

void effResetActiveInstanceFrame(EffClassWork *work) {
    D_0037E8A0[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceActiveInstanceFrame(EffClassWork *work) {
    D_0037E8A0[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDispatchActiveInstanceDraw(EffClassWork *work) {
    D_0037E8A0[work->kind].draw(work);
}

void effUpdateAndDrawActiveInstance(EffClassWork *work) {
    effAdvanceActiveInstanceFrame(work);
    effDispatchActiveInstanceDraw(work);
}

void effCopyActiveInstancePosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyActiveInstanceOrientation(void *work, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)work)->vectors.orientation, src);
}

void effSetActiveInstanceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetActiveInstanceMatrixComponent(EffClassWork *work, float value) {
    work->scale = value;
}

typedef struct EffTrackSet {
    u32 type;      // 0x00
    u32 color;     // 0x04
    s32 rows;      // 0x08
    u16 kind;      // 0x0C
    u8 pad_0E[2];
    s32 count;     // 0x10
    u8 flag;       // 0x14
    u8 pad_15[3];
    void *shared;    // 0x18: optional retained reference
    u8 *buffer;    // 0x1C
    u8 *columns;   // 0x20
    u8 *tail;      // 0x24
    s32 *handle;   // 0x28
    u8 *allocation; // 0x2C
} EffTrackSet;

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2B10);

EffTrackSet *effCreateTrackSet(s32 count, u16 kind) {
    s32 rows;
    s32 cols;
    s32 size;
    u8 *base;
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
    data = (u8 *)sdfResourceRetainAddress((u32)base);
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
    func_002DA420(set->handle, 1.0f);
    memset(D_003DC9E0, 0, 0x2C);
    D_003DC9E0[2] = 0x4000;
    return set;
}

extern u32 D_003BC96C;

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC960);

INCLUDE_SDATA(const s32, "game/code_0029C530", effFlashTextureHandles);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC96C);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC970);

u32 effCreateTrackSetWithSharedReferences(u32 count, u32 kind, u32 sharedRef) {
    EffTrackSet *effect = effCreateTrackSet(count, (u16)kind);

    if (effect->columns != 0) {
        if (sharedRef == 0) {
            switch (effect->kind) {
            case 3:
                if (D_003BC970[0] == 0) {
                    D_003BC978[0] = (RefObj *)effCloneSharedReferenceWithValue(effFlashTextureHandles, 0x100);
                }
                D_003BC970[0]++;
                break;
            case 4:
                if (D_003BC970[1] == 0) {
                    D_003BC978[1] = (RefObj *)effCloneSharedReferenceWithValue(D_003BC96C, 0x101);
                }
                D_003BC970[1]++;
                break;
            }
        } else {
            effect->shared = func_0029BD90((void *)sharedRef);
        }
    }
    return (u32)effect;
}



/* Release the track set's retained reference, draw asset, and allocation. */
void effReleaseResourceRefs(u8 *work) {
    EffTrackSet *refs = (EffTrackSet *)work;
    if (refs->columns != 0) {
        RefObj *ref = refs->shared;
        if (ref == NULL) {
            switch (refs->kind) {
            case 3:
                if (--D_003BC970[0] == 0) {
                    effReleaseSharedReference(D_003BC978[0]);
                    D_003BC978[0] = NULL;
                }
                break;
            case 4:
                if (--D_003BC970[1] == 0) {
                    effReleaseSharedReference(D_003BC978[1]);
                    D_003BC978[1] = NULL;
                }
                break;
            }
        } else {
            effReleaseSharedReference(ref);
        }
    }
    sdfQueueAssetRelease((u32)refs->handle);
    sdfReleaseResourceAllocation((u32)refs->allocation);
}

/* Copy a track set: same size and kind, retaining the source's shared reference (or counting one more user of the built-in one). */
u32 effDuplicateResourceRefs(u32 source) {
    EffTrackSet *original = (EffTrackSet *)source;
    EffTrackSet *effect = effCreateTrackSet(original->count, original->kind);

    if (effect->columns != 0) {
        if (original->shared != 0) {
            effect->shared = effRetainSharedReference(original->shared);
        } else {
            switch (effect->kind) {
            case 3:
                D_003BC970[0]++;
                break;
            case 4:
                D_003BC970[1]++;
                break;
            }
        }
    }
    return (u32)effect;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A3E10);

extern u32 D_003BC960[2];

void effLoadFlashTextures(void) {
    D_003BC960[0] = sdfReadNamedResource("/effect/flash00.tmx", &effFlashTextureHandles, 0);
    D_003BC960[1] = sdfReadNamedResource("/effect/flash01.tmx", &effFlashTextureHandles + 1, 0);
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
    u32 effect;
    u32 references;
    u32 allocation;
} EffClassDrawState;

void effResetRingResourceFrame(s32 work) {
    ((EffClassDrawState *)((EffBillFrameWork *)work)->frameState)->ring->frame = 0;
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
    sdfReleaseChipBlock(handle);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A4478);

extern void effDrawFourPointGroups(u8 *, void *);

void billDrawCellBlendA(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
        ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
        ((EffBillOutput *)out)->outputMode = ((EffBillConfig *)config)->outputMode;
        VU0_LOAD_VF(vf10, work->transform);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E0E0);
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
    u32 effect;

    effect = ((EffClassDrawState *)((EffBillFrameWork *)work)->frameState)->effect;
    ((EffCounterHeader *)((EffClassDrawState *)((EffBillFrameWork *)work)->frameState)->references)->frame = 0;
    effInitializeClassFrame(effect);
}

extern s8 effSharedRandomState[];
extern float effMiscRandUnitFloat(void *);
/* The cached class payload is copied from the 0x5C-byte source prefix. */
typedef struct EffRingClassConfig {
    EffRingSource ring;
    u8 pad50[0xC];
    u8 classConfig[0x5C];
    u8 padB8[0x10];
    f32 scale;
    f32 scaleRand;
} EffRingClassConfig;

extern u8 *effPayloadPointerSet(u16, void *);

EffClassDrawState *effCreateScaledClassDrawState(EffRingClassConfig *source) {
    u32 count = source->ring.segments;
    u32 size;
    void *allocation;
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
    scales = (f32 *)sdfResourceRetainAddress((u32)allocation);
    state = (EffClassDrawState *)((u8 *)scales + size);
    state->allocation = (u32)allocation;
    state->scales = scales;
    memcpy(source->classConfig, source, sizeof(source->classConfig));
    state->effect = (u32)effPayloadPointerSet(1, source->classConfig);
    tracks = (EffTrackSet *)effCreateTrackSetWithSharedReferences(count, 2, 0);
    first = source->ring.firstColor;
    state->references = (u32)tracks;
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
    effReleaseResourceRefs(((EffClassDrawState *)work)->references);
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

void func_002A4AF0(BillCellDrawWork *work) {
    u8 *config = work->config;
    s32 limit = work->frameLimit;
    s32 progress = ((EffBillConfig *)config)->progress;
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
    count = ((EffBillConfig *)config)->frames.count;
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
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
        effRunClassPostFrame((s32)dst);
        *(u32 *)out = ((EffBillConfig *)config)->textureId;
        ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->outputMode;
        VU0_LOAD_VF(vf10, work->transform);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E0E0);
        VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALE_MATRIX_ROWS(vf10);
        VU0_LOAD_VF(vf10, work);
        VU0_SET_W_ONE(vf10);
        VU0_MOVE_VF(vf31, vf10);
        VU0_STORE_MATRIX(mtx);
        func_002A3E10(out, mtx);
    }
}

void effResetClassRingFrame(EffClassWork *work) {
    ((EffRingResource *)work->resource)->pointSet->color = 0;
}

extern u8 *effCreatePointSet4(u32);

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
    sdfReleaseChipBlock((u32)handle);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A4ED0);

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
    second = func_00296F58(&config->colorTrack, &config->alphaTrack, limit, progress);
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
        VU0_LOAD_VF(vf10, D_0037E0E0);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALE_MATRIX_ROWS(vf10);
        VU0_LOAD_VF(vf10, work);
        VU0_SET_W_ONE(vf10);
        VU0_MOVE_VF(vf31, vf10);
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups((u8 *)out, mtx);
    }
}

u8 *effPayloadPointerSet(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037EAD0[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = D_0037EAD0[kind].createResource(source);
    D_0037EAD0[kind].initialize(effect);
    return effect;
}

void effCreateClassWorkFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effPayloadPointerSet(((FileJob *)request)->option, source);
}

void effDestroyClassWork(EffClassWork *work) {
    D_0037EAD0[work->kind].destroyResource((void *)work->resource);
    sdfReleaseChipBlock(work);
}

void effCreateClassWorkFromRequest(EffClassWork *work) {
    effPayloadPointerSet(work->kind, work->payload);
}

void effInitializeClassFrame(EffClassWork *work) {
    D_0037EAD0[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceClassFrame(EffClassWork *work) {
    D_0037EAD0[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effRunClassPostFrame(EffClassWork *work) {
    D_0037EAD0[work->kind].draw(work);
}

void effUpdateClassFrame(EffClassWork *work) {
    effAdvanceClassFrame(work);
    effRunClassPostFrame(work);
}

void effSetClassWorkPrimaryTransformVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetClassWorkSecondaryTransformVector(void *work, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)work)->vectors.orientation, src);
}

void effSetClassWorkColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetClassWorkScale(EffClassWork *work, float value) {
    work->scale = value;
}


extern EffPacketParams D_003DCA10[];

u8 *effCreatePointSet4(u32 count) {
    s32 rows = count * 4 + 4;
    s32 size = rows * 20;
    u8 *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x20);
    data = (u8 *)sdfResourceRetainAddress((u32)base);
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
    func_002DA420(set->handle, 1.0f);
    memset(D_003DCA10, 0, 0x2C);
    D_003DCA10[0].primitive = 0x4000;
    return (u8 *)set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effAssetQueueRelease(s32 work) {
    sdfQueueAssetRelease((u32)((EffPointSet *)work)->handle);
    sdfReleaseResourceAllocation((u32)((EffPointSet *)work)->allocation);
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
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        if (matrix == NULL) {
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdfConsAppendVuPacket(list, 0);
        sdfConsAppendAssetPacket(list, set->handle, 0);
        if (set->flag == 0) {
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        remaining = set->rows;
        D_003DCA10[0].colors = (u32 *)set->tail;
        D_003DCA10[0].positions = (u128 *)set->buffer;
        D_003DCA10[0].unk08 = set->color;
        D_003DCA10[0].parameterCount = 12;
        D_003DCA10[0].vertexCount = 12;
        D_003DCA10[0].parameters = D_0037EB30;
        while (remaining >= 12) {
            remaining -= 8;
            sdfAppendPacket(list, func_0015FE20(D_003DCA10));
            D_003DCA10[0].positions += 8;
            D_003DCA10[0].colors += 8;
        }
        if (remaining >= 8) {
            D_003DCA10[0].parameterCount = 6;
            D_003DCA10[0].vertexCount = remaining;
            sdfAppendPacket(list, func_0015FE20(D_003DCA10));
        }
        if (set->flag == 0) {
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        if (set->type < 5) {
            effSubmitSurfacePacket(D_0037EB78[set->type], list);
        } else {
            EffGsPacket *blendPacket;

            surfaceId = set->type == 5 ? 51 : 56;
            setup = sdfAllocPacketAligned(0x20);
            sdfInitPacketList(setup);
            blendPacket = sdfAllocPacketAligned(0x30);
            blendPacket->registerValue = 6;
            blendPacket->dmaTag = 2;
            blendPacket->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            blendPacket->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            blendPacket->registerList = 0xE;
            blendPacket->registerAddress = 0x42;
            sdfAppendPacket(setup, blendPacket);
            effSubmitSurfacePacket(&kwlnDrawSurfaces[surfaceId], setup);
            blendPacket = sdfAllocPacketAligned(0x30);
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
            sdfAppendPacket(list, blendPacket);
            effSubmitSurfacePacket(&kwlnDrawSurfaces[surfaceId], list);
        }
    }
}

typedef struct EffectSurfaceNode {
    u32 handleCount;
    u32 color;
    f32 opacity;
    u32 kind;
    EffSurfaceParams params; /* 0x10: copied source header */
    u32 index;
    u32 handleBuffer;   // 0x30
    void *resource;
    u32 *jobs;          // 0x38
    u32 jobBuffer;      // 0x3C
    RefObj *resourceHolder; // 0x40, released separately from the grid record
    u32 record;         // 0x44: fileAllocateGridRecordSlots result
    u16 count;          // 0x48: initialized to 1; remaining role unknown
} EffectSurfaceNode;

/* Create an empty surface node for handleCount grid cells; no handles are allocated yet. */
EffectSurfaceNode *effCreateSurfaceNode(u32 handleCount) {
    EffectSurfaceNode *node = sdfAllocAndClearQuadwords(sizeof(EffectSurfaceNode));
    node->handleCount = handleCount;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->index = 0;
    node->record = 0;
    node->count = 1;
    return node;
}

EffectSurfaceNode *effCreateSurfaceNodeForGrid(EffGrid *grid) {
    u32 n;
    s32 w = grid->width;
    n = (w ? w : grid->height) * (w ? grid->height : grid->altHeight);
    return effCreateSurfaceNode(n > 100 ? 100 : n);
}

EffectSurfaceNode *effResourceReferenceReplaceFromFile(u8 *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    EffGrid *params = (EffGrid *)(source + 0x1C);
    EffectSurfaceNode *node = effCreateSurfaceNodeForGrid(params);

    effRebuildSurfaceHandles(node, ((FileJob *)work)->option, source);
    effReplaceResourceRef(node, ((FileJob *)work)->option, params);
    return node;
}

extern EffectSurfaceNode *effResourceReferenceReplaceFromFile(u8 *);

extern void effRebuildSurfaceJobs(EffectSurfaceNode *, void *);

extern void effReplaceSurfaceResourceHolder(s32, u32);

EffectSurfaceNode *effInitializeSurfaceForKind(u8 *work) {
    EffectSurfaceNode *node = effResourceReferenceReplaceFromFile(work);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary == NULL) {
        return node;
    }
    switch (((FileJob *)work)->slots[0].selector) {
    case 1:
        effReplaceSurfacePrimaryBillboard(node, (u32)secondary);
        break;
    case 2:
        effReplaceSurfaceFlaggedBillboard(node, (u32)secondary);
        break;
    case 3:
        break;
    case 4:
        effSetSurfaceRetainedResource(node, *secondary);
        break;
    case 5:
        effRebuildSurfaceJobs(node, secondary);
        break;
    case 6:
        break;
    case 7:
        effReplaceSurfaceResourceHolder((s32)node, (u32)secondary);
        break;
    }
    node->kind = ((FileJob *)work)->slots[0].selector;
    return node;
}

void effDestroySurfaceNode(EffectSurfaceNode *node) {
    u32 i;
    u32 count;

    if (node->resource != NULL) {
        billDispatchByKind(node->resource);
    }
    if (node->jobBuffer != 0) {
        count = ((EffGridRecord *)node->record)->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobBuffer);
    }
    if (node->index != 0) {
        for (i = 0; i < node->handleCount; i++) {
            effReleaseSurfaceGridBuffers(((u32 *)node->index)[i]);
        }
        sdfReleaseResourceAllocation(node->handleBuffer);
    }
    if (node->resourceHolder != 0) {
        effReleaseReferenceHolder(node->resourceHolder);
    }
    if (node->record != 0) {
        fileReleaseGridRecordHandle(node->record);
    }
    sdfReleaseChipBlock(node);
}

extern void effRebuildSurfaceHandles(EffectSurfaceNode *, u16, void *);

EffectSurfaceNode *effRecreateSurfaceNodeFromWork(u8 *work) {
    EffGrid *params = ((EffGridRecord *)((EffectSurfaceNode *)work)->record)->params;
    EffectSurfaceNode *node = effCreateSurfaceNodeForGrid(params);
    effRebuildSurfaceHandles(node, ((EffGridRecord *)((EffectSurfaceNode *)work)->record)->kind, &((EffectSurfaceNode *)work)->params);
    effReplaceResourceRef(node, ((EffGridRecord *)((EffectSurfaceNode *)work)->record)->kind, params);
    return node;
}

extern void func_002A5DE0(EffectSurfaceNode *, u8 *);

EffectSurfaceNode *effCreateSurfaceGridWithConfiguration(u8 *work) {
    EffGrid *params = ((EffGridRecord *)((EffectSurfaceNode *)work)->record)->params;
    EffectSurfaceNode *node = effCreateSurfaceNodeForGrid(params);
    effRebuildSurfaceHandles(node, ((EffGridRecord *)((EffectSurfaceNode *)work)->record)->kind, &((EffectSurfaceNode *)work)->params);
    effReplaceResourceRef(node, ((EffGridRecord *)((EffectSurfaceNode *)work)->record)->kind, params);
    func_002A5DE0(node, work);
    return node;
}

void func_002A5DE0(EffectSurfaceNode *dst, u8 *work) {
    EffectSurfaceNode *src = (EffectSurfaceNode *)work;
    u32 selector = src->kind;

    switch (selector) {
    case 1:
    case 2:
    case 4:
        if (dst->resource != NULL) {
            billDispatchByKind(dst->resource);
        }
        dst->resource = (void *)billCloneObjectRetainingSharedData((u32)src->resource);
        billMarkKindOneFlag((u32)dst->resource);
        if (dst->record != 0) {
            EffGridRecord *record = (EffGridRecord *)dst->record;
            billSetBillboardMode((u32)dst->resource, record->bill->mode);
        }
        break;
    case 5: {
        u32 count = ((EffGridRecord *)src->record)->count;
        s32 size;
        u32 i;

        if (dst->jobBuffer != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(dst->jobs[i]);
            }
            sdfReleaseResourceAllocation(dst->jobBuffer);
            dst->jobs = 0;
            dst->jobBuffer = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobBuffer = (u32)sdfAllocGeneralBlock(size);
        dst->jobs = (u32 *)sdfResourceRetainAddress(dst->jobBuffer);
        for (i = 0; i < count; i++) {
            dst->jobs[i] = fileJobCreateChild(src->jobs[0]);
        }
        break;
    }
    case 7:
        if (dst->resourceHolder != 0) {
            effReleaseReferenceHolder((u8 *)dst->resourceHolder);
        }
        dst->resourceHolder = effReferenceObjectRetain(src->resourceHolder);
        break;
    }
    dst->kind = src->kind;
}


/* Replace the per-cell grid handles from the copied source header. kind is unused here. */
void effRebuildSurfaceHandles(EffectSurfaceNode *node, u16 kind, void *source) {
    u32 *params = source;
    u32 size;
    u32 i;
    u32 handle;

    node->params = *(EffSurfaceParams *)params;
    size = node->handleCount * 4;
    if (size == 0) {
        return;
    }
    if (node->index != 0) {
        for (i = 0; i < node->handleCount; i++) {
            effReleaseSurfaceGridBuffers(((u32 *)node->index)[i]);
        }
        sdfReleaseResourceAllocation(node->handleBuffer);
    }
    node->handleBuffer = (u32)sdfAllocGeneralBlock(size);
    node->index = sdfResourceRetainAddress(node->handleBuffer);
    for (i = 0; i < node->handleCount; i++) {
        handle = effCreateSurfaceGridNode(params[0], params[1]);
        ((u32 *)node->index)[i] = handle;
        effFillSurfaceGridColorGradient(handle, params + 2);
    }
}

extern u32 fileAllocateGridRecordSlots(u16, u32, void *);

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, void *resource) {
    if (node->record != 0) {
        fileReleaseGridRecordHandle(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

extern u32 effRetainResource(u32);

void effSetSurfaceRetainedResource(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = effRetainResource(resourceId);
    node->resource = (void *)resource;
    if (node->record != 0) {
        billSetBillboardMode(resource, ((EffGridRecord *)node->record)->bill->mode);
    }
}

extern u32 billCreateIndexed(u32, u32);

void effReplaceSurfacePrimaryBillboard(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = billCreateIndexed(0, resourceId);
    node->resource = (void *)resource;
    if (node->record != 0) {
        billSetBillboardMode(resource, ((EffGridRecord *)node->record)->bill->mode);
    }
}

void effReplaceSurfaceFlaggedBillboard(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = billCreateIndexed(1, resourceId);
    node->resource = (void *)resource;
    billMarkKindOneFlag(resource);
    if (node->record != 0) {
        billSetBillboardMode(node->resource, ((EffGridRecord *)node->record)->bill->mode);
    }
}

void effRebuildSurfaceJobs(EffectSurfaceNode *node, void *source) {
    u32 count = ((EffGridRecord *)node->record)->count;
    u32 i;
    u32 size;

    if (node->jobBuffer != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobBuffer);
        node->jobs = 0;
        node->jobBuffer = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobBuffer = (u32)sdfAllocGeneralBlock(size);
        node->jobs = (u32 *)sdfResourceRetainAddress(node->jobBuffer);
        node->jobs[0] = fileJobCreateFromJob((u32)source);
        for (i = 1; i < count; i++) {
            node->jobs[i] = fileJobCreateChild(node->jobs[0]);
        }
    }
}

void effReplaceSurfaceResourceHolder(s32 node, u32 resource) {
    u32 holder;

    if (((EffectSurfaceNode *)node)->resourceHolder != 0) {
        effReleaseReferenceHolder(((EffectSurfaceNode *)node)->resourceHolder);
    }
    holder = func_0029C230(resource);
    ((EffectSurfaceNode *)node)->resourceHolder = holder;
}

void effClearSurfaceNodeRecordReferences(s32 node) {
    if (((EffectSurfaceNode *)node)->record != 0) {
        fileClearRecordReferences(((EffectSurfaceNode *)node)->record);
        return;
    }
}

void effAcquireSurfaceRecord(s32 node) {
    if (((EffectSurfaceNode *)node)->record != 0) {
        fileAcquireRecord(((EffectSurfaceNode *)node)->record);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A6440);

void effUpdateSurfaceRecordAndDraw(s32 node) {
    effAcquireSurfaceRecord(node);
    func_002A6440(node);
}

void effMenuRecordVectorSet(s32 node) {
    mnuRecordSetVector(((EffectSurfaceNode *)node)->record);
}

void effSetSurfaceRecordSecondaryVector(s32 node) {
    fileSetRecordSecondVector(((EffectSurfaceNode *)node)->record);
}

void effSetSurfaceNodeColor(s32 node, u32 value) {
    ((EffectSurfaceNode *)node)->color = value;
}

void effIndexedFloatCallbackDispatch(ValPtr44 *p, float v) {
    p->f08 = v;
    dds3DispatchIndexedCallback(p->p44, v);
}

typedef struct EffMotionSetup {
    u16 mode;    // 0x00
    u16 kind;    // 0x02
    u16 flags;   // 0x04
    u8 pad_06[6];
    void *table; // 0x0C
    u8 pad_10[0x1C];
} EffMotionSetup; // 0x2C

extern EffMotionSetup D_003DCA40;

extern EffMotionSetup D_003DCA70;

extern u8 D_0037EB90[];

extern u8 D_0037EBE0[];

/* Surface node: 0x3C bytes at the end of the retained block, after the vertex rows. */
typedef struct EffSurfaceGridNode {
    u8 pad_00[0x10];
    s32 rows;       // 0x10
    u32 field_14;   // 0x14
    u32 field_18;   // 0x18
    u32 columns;    // 0x1C
    u32 type;       // 0x20
    u8 *buffer;     // 0x24
    u8 *tail;       // 0x28
    s32 *handle;    // 0x2C
    u8 *queueA;     // 0x30
    u8 *queueB;     // 0x34
    u8 *allocation; // 0x38
} EffSurfaceGridNode;

/* vu0 routine: grid surface node with rows of three vertex columns */
u32 effCreateSurfaceGridNode(u32 count, u32 columns) {
    s32 rows = count * columns * 3 + 6;
    s32 size = rows * 20 + 0xA0;
    u8 *base;
    u8 *data;
    EffSurfaceGridNode *node;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x3C);
    data = (u8 *)sdfResourceRetainAddress((u32)base);
    node = (EffSurfaceGridNode *)(data + size);
    node->type = 2;
    node->buffer = data;
    data += rows * 16;
    node->queueA = data;
    data += 0x80;
    node->queueB = data;
    data += 0x20;
    node->field_18 = 3;
    node->rows = rows;
    node->columns = columns;
    node->allocation = base;
    node->tail = data;
    node->field_14 = 0;
    node->handle = sdfCreateAssetWithDrawEntries();
    func_002DA420(node->handle, 1.0f);
    memset(&D_003DCA40, 0, sizeof(EffMotionSetup));
    D_003DCA40.flags = 0x4000;
    D_003DCA40.table = D_0037EB90;
    memset(&D_003DCA70, 0, sizeof(EffMotionSetup));
    D_003DCA70.flags = 0x4000;
    D_003DCA70.table = D_0037EBE0;
    D_003DCA70.mode = 6;
    D_003DCA70.kind = 8;
    return (u32)node;
}

/* vu0 routine: fade-blended vertex colours written into the surface node's tail */
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
    sdfQueueAssetRelease((u32)((EffSurfaceGridNode *)work)->handle);
    sdfReleaseResourceAllocation((u32)((EffSurfaceGridNode *)work)->allocation);
}

void effResetSurfaceGridFrame(s32 work) {
    ((EffSurfaceGridNode *)work)->field_14 = 0;
    ((EffSurfaceGridNode *)work)->field_18 = 3;
}

/* Three-vector ring sampler; DDS1 stores its counters at 0x10-0x1C. */
typedef struct EffRingFrameState {
    u8 pad_00[0x10];
    s32 vectorCapacity; // 0x10, used when wrapping a frame
    u32 pad_14;         // 0x14, initialized to zero
    s32 vectorCount;    // 0x18
    s32 frameStride;    // 0x1C
    u8 pad_20[4];
    u128 *vectors;      // 0x24
} EffRingFrameState;

void effCopyRingFrameVectors(u8 *work, u128 *destination, s32 frame) {
    s32 index = ((EffRingFrameState *)work)->vectorCount - 3 * (((EffRingFrameState *)work)->frameStride * (frame - 1) + frame);
    u128 *source;
    s32 i;

    if (index < 3) {
        index += ((EffRingFrameState *)work)->vectorCapacity - 3;
    }
    source = ((EffRingFrameState *)work)->vectors + index;
    for (i = 0; i < 3; i++) {
        PCP_COPY_VECTOR(destination + i, source + i);
    }
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A7568);

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A7610);

void effBeginMatrixVuDrawPacket(const Matrix4 *matrix) {
    void *work = sdfAllocPacketAligned(0x20);
    effCurrentRenderPacket = (u32)work;
    sdfInitPacketList(work);
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(effCurrentRenderPacket, 0);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A78E0);

typedef struct EffDrawEntry {
    u32 handle;
    s32 frame;
    u8 pad08[8];
    void (*draw)(u8 *, u32);
} EffDrawEntry;

void effSubmitIndexedRenderPacket(u32 index) {
    u8 *entry = D_0037EBF8[index];
    ((EffDrawEntry *)entry)->draw(entry, effCurrentRenderPacket);
    effCurrentRenderPacket = 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A7B68);

void effReleaseRenderResources(u8 *work) {
    if (((EffRenderResourceState *)work)->billHandle != 0) {
        billDispatchByKind((void *)((EffRenderResourceState *)work)->billHandle);
    }
    if (((EffRenderResourceState *)work)->reference != NULL) {
        effReleaseReferenceHolder(((EffRenderResourceState *)work)->reference);
    }
    if (((EffRenderResourceState *)work)->assetHandle != 0) {
        sdfQueueAssetRelease((void *)((EffRenderResourceState *)work)->assetHandle);
    }
    sdfReleaseChipBlock(work);
}

u8 *effCloneRenderResourceWork(u8 *source) {
    u8 *effect = func_002A7B68(NULL);
    memcpy(effect + 0x30, source + 0x30, 0x98);
    effDuplicateRenderResourceOwner(effect, source);
    return effect;
}

extern void billMarkKindOneFlag(u32);

extern void billSetBillboardMode(u32, s16);

void effDuplicateRenderResourceOwner(u8 *work, u8 *source) {
    u32 resource;

    if (((EffRenderResourceState *)source)->billHandle != 0) {
        if (((EffRenderResourceState *)work)->billHandle != 0) {
            billDispatchByKind((void *)((EffRenderResourceState *)work)->billHandle);
        }
        resource = billCloneObjectRetainingSharedData(((EffRenderResourceState *)source)->billHandle);
        ((EffRenderResourceState *)work)->billHandle = resource;
        billMarkKindOneFlag(resource);
        billSetBillboardMode(((EffRenderResourceState *)work)->billHandle, ((EffRenderResourceState *)work)->billMode);
        return;
    }
    if (((EffRenderResourceState *)work)->reference != NULL) {
        effReleaseReferenceHolder(((EffRenderResourceState *)work)->reference);
    }
    ((EffRenderResourceState *)work)->reference = effReferenceObjectRetain(((EffRenderResourceState *)source)->reference);
}

void effResetRenderResourceKind(s32 work) {
    ((EffClassWork *)work)->kind = 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A8020);

void effCopyRenderResourcePosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyRenderResourceOrientation(void *work, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)work)->vectors.orientation, src);
}

void effSetRenderResourceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetRenderResourceMatrixComponent(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}


extern u32 effMiscRand(void *);

typedef struct EffParticleFrameEntry {
    u32 unk00;
    s32 frame;
    u32 unk08;
    u32 unk0C;
} EffParticleFrameEntry;

void effRandomizeParticleFields(u8 *work) {
    u32 index = 0;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u8 *entry = ((EffFrameState *)((EffBillFrameWork *)work)->frameState)->entries;

    if (count != 0) {
        do {
            ((EffParticleFrameEntry *)entry)->frame = -1 - (effMiscRand(effSharedRandomState) & 3);
            entry += 0x10;
            index++;
        } while (index < count);
    }
}

typedef struct EffPointSetTableSource {
    u8 pad00[0x38];
    u32 count;     /* 0x38: rows, one point set each */
    s32 layers;    /* 0x3C: at least 3 */
    u8 pad40[0x28];
    f32 unk68;     /* 0x68: fade-in share of a set */
    f32 unk6C;     /* 0x6C: end of the full-alpha span */
    u32 colorA;    /* 0x70: low 24 bits kept, top byte ramped */
    u32 colorB;    /* 0x74 */
    u32 colorC;    /* 0x78 */
    u8 pad7C[8];
    f32 unk84;     /* 0x84: copied separately from the descriptor prefix */
} EffPointSetTableSource;

typedef struct EffPointSetRow {
    EffPointSet *set; /* 0x00 */
    s32 key;          /* 0x04 */
    u32 pad08;
    f32 angle;        /* 0x0C: phase of the radial class instance */
} EffPointSetRow;

typedef struct EffPointSetTable {
    EffPointSetRow *rows;
} EffPointSetTable;

EffPointSetTable *effCreateAlphaRampPointSetRows(EffPointSetTableSource *src) {
    u32 count = src->count;
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

    table = sdfAllocSizeClassBlock(count * sizeof(EffPointSetRow) + 4);
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
    rampIn = (s32)(src->unk68 * (f32)(src->layers + 1));
    rampOut = (s32)(src->unk6C * (f32)(src->layers + 1));
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
    u32 index = 0;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u8 *pool = ((EffBillFrameWork *)work)->frameState;
    u8 *entry = *(u8 **)pool;

    if (count != 0) {
        do {
            effReleasePointSetAsset(*(s32 *)entry);
            entry += 0x10;
            index++;
        } while (index < count);
    }
    sdfReleaseChipBlock(pool);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A8A90);

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A9160);

extern void effResetDispatchCounter(EffClassWork *);

void effResetIndexedInstanceFrames(u8 *work) {
    u32 index = 0;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u8 *entry = ((EffFrameState *)((EffBillFrameWork *)work)->frameState)->entries;

    if (count != 0) {
        do {
            effResetDispatchCounter(*(u8 **)entry);
            entry += 4;
            index++;
        } while (index < count);
    }
}

extern u8 *effCreateClassResourceWork(u16, void *);

u8 *func_002A93A0(EffPointSetTableSource *config) {
    u32 count = config->count;
    u8 *allocation = sdfAllocSizeClassBlock(count * 4 + 4);
    u32 *entries = (u32 *)(allocation + 4);
    EffPointSetTableSource copy;
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
    copy.count = 1;
    copy.unk84 = config->unk84;
    step = 6.2831852f / (f32)count;
    position = step * ((effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f);
    for (index = 0; index < count; index++) {
        u8 *resourceWork = effCreateClassResourceWork(1, &copy);
        u8 **resourceSlot = (u8 **)((EffClassWork *)resourceWork)->resource;

        *entries++ = (u32)resourceWork;
        *(f32 *)(*resourceSlot + 0xC) = position;
        offset = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f;
        position += step + step * offset * 0.25f;
    }
    return allocation;
}

extern void effDestroyClassResourceWork(EffClassWork *);

void effReleaseBillFrameEntries(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 index = 0;
    u8 *entry = *(u8 **)state;

    if (count != 0) {
        do {
            effDestroyClassResourceWork(*(u8 **)entry);
            entry += 4;
        } while (++index < count);
    }
    sdfReleaseChipBlock(state);
}

extern void effAdvanceClassResourceFrame(EffClassWork *);

void effReleaseTrackEntriesA(u8 *work) {
    u32 index = 0;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u8 *entry = *(u8 **)((EffBillFrameWork *)work)->frameState;

    if (count != 0) {
        do {
            effAdvanceClassResourceFrame(*(s32 *)entry);
            entry += 4;
            index++;
        } while (index < count);
    }
}

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void effCopyClassResourcePosition(void *, void *);
extern void effCopyClassResourceOrientation(void *, void *);
extern void effSetClassResourceMatrixComponent(EffClassWork *, f32);
extern void effSetClassResourceColor(EffClassWork *, u32);
extern void effDrawClassResourceWork(EffClassWork *);

/* vu0 routine: packed color blend and SDK vector copies. */
void effUpdateRadialClassInstances(EffClassWork *work) {
    EffBillConfig *config = work->payload;
    s32 frame = work->frame;
    s32 progress = config->progress;
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
    count = config->frames.count;
    radius = config->rowOffset;
    second = func_00296F58((u8 *)config, (u8 *)config + 0x24, frame, progress);
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
        effSetClassResourceColor(*entries, color);
        effDrawClassResourceWork(*entries);
    }
}

typedef struct EffScaleRange {
    u8 *entries;
    f32 start;
    f32 delta;
    struct MemBlock *allocation;
} EffScaleRange;

typedef struct EffScaleRangeConfig {
    u8 pad_00[0x34];
    s32 steps;
    u32 count;
    u8 pad_3C[0x50];
    f32 startBase;
    f32 startRand;
    f32 endBase;
    f32 endRand;
} EffScaleRangeConfig;

/* The seed is initialized to one of eight negative sentinel values. */
typedef struct EffScaleRangeEntry {
    EffPointSet *set;
    u8 pad04[0x10];
    s32 negativeSeed;
    u8 pad_18[0x18];
} EffScaleRangeEntry;

typedef struct EffScaleRangeWork {
    u8 pad_00[0x30];
    EffScaleRange *range;
    EffScaleRangeConfig *config;
} EffScaleRangeWork;

extern float effMiscRandUnitFloat(void *);

void effSeedBillScaleRange(u8 *work) {
    EffScaleRangeConfig *config = ((EffScaleRangeWork *)work)->config;
    EffScaleRange *range = ((EffScaleRangeWork *)work)->range;
    s32 steps = config->steps;
    u8 *entry = range->entries;
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
    count = config->count;
    index = 0;
    if (count != 0) {
        do {
            index++;
            ((EffScaleRangeEntry *)entry)->negativeSeed = -1 - (effMiscRand(effSharedRandomState) & 7);
            entry += 0x30;
        } while (index < count);
    }
}

EffScaleRange *effCreateRetainedPointSetColorRows(EffPointSetTableSource *src) {
    u32 count = src->count;
    struct MemBlock *allocation;
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
    table = (EffScaleRange *)sdfResourceRetainAddress((u32)allocation);
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
    rampIn = (s32)(src->unk68 * (f32)(src->layers + 1));
    rampOut = (s32)(src->unk6C * (f32)(src->layers + 1));
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

extern void effReleasePointSetAsset(s32);

void effReleaseBillPointEntries(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 index = 0;
    u8 *entry = *(u8 **)state;

    if (count != 0) {
        do {
            effReleasePointSetAsset((s32)((EffScaleRangeEntry *)entry)->set);
            entry += 0x30;
        } while (++index < count);
    }
    sdfReleaseResourceAllocation((u32)((EffScaleRange *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002A9DA8);

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AA748);

u8 *effCreateClassResourceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037EC50[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = D_0037EC50[kind].createResource(source);
    D_0037EC50[kind].initialize(effect);
    return effect;
}

void effCreateClassResourceFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effCreateClassResourceWork(((FileJob *)request)->option, source);
}

void effDestroyClassResourceWork(EffClassWork *work) {
    D_0037EC50[work->kind].destroyResource();
    sdfReleaseChipBlock(work);
}

void effPayloadPointerGet(EffClassWork *work) {
    effCreateClassResourceWork(work->kind, work->payload);
}

void effResetDispatchCounter(EffClassWork *work) {
    D_0037EC50[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceClassResourceFrame(EffClassWork *work) {
    D_0037EC50[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDrawClassResourceWork(EffClassWork *work) {
    D_0037EC50[work->kind].draw(work);
}

void effUpdateAndDrawClassResource(EffClassWork *work) {
    effAdvanceClassResourceFrame(work);
    effDrawClassResourceWork(work);
}

void effCopyClassResourcePosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyClassResourceOrientation(void *work, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)work)->vectors.orientation, src);
}

void effSetClassResourceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetClassResourceMatrixComponent(EffClassWork *work, float value) {
    work->scale = value;
}

extern EffPacketParams D_003DCAD0[];

EffPointSet *effCreatePointSet5(s32 count) {
    s32 rows = count * 5 + 5;
    s32 size = rows * 20;
    u8 *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x20);
    data = (u8 *)sdfResourceRetainAddress((u32)base);
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
    func_002DA420(set->handle, 1.0f);
    memset(D_003DCAD0, 0, 0x2C);
    D_003DCAD0[0].primitive = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleasePointSetAsset(s32 work) {
    sdfQueueAssetRelease((u32)((EffPointSet *)work)->handle);
    sdfReleaseResourceAllocation((u32)((EffPointSet *)work)->allocation);
}

void effDrawFivePointGroups(EffPointSet *set, Matrix4 *matrix) {
    void *list;
    EffGsPacket *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        if (matrix == NULL) {
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdfConsAppendVuPacket(list, 0);
        sdfConsAppendAssetPacket(list, set->handle, 0);
        if (set->flag == 0) {
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        remaining = set->rows;
        D_003DCAD0[0].colors = (u32 *)set->tail;
        D_003DCAD0[0].positions = (u128 *)set->buffer;
        D_003DCAD0[0].unk08 = set->color;
        D_003DCAD0[0].parameterCount = 0x10;
        D_003DCAD0[0].vertexCount = 0xF;
        D_003DCAD0[0].parameters = D_0037ECB0;
        while (remaining >= 0xF) {
            remaining -= 0xA;
            sdfAppendPacket(list, func_0015FE20(D_003DCAD0));
            D_003DCAD0[0].positions += 0xA;
            D_003DCAD0[0].colors += 0xA;
        }
        if (remaining >= 0xA) {
            D_003DCAD0[0].parameterCount = 8;
            D_003DCAD0[0].vertexCount = remaining;
            sdfAppendPacket(list, func_0015FE20(D_003DCAD0));
        }
        if (set->flag == 0) {
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        D_0037ECF0[set->type]->append((SdfListHead *)D_0037ECF0[set->type], list);
    }
}

typedef struct EffectStripNode {
    u32 percent;
    u32 color;
    f32 opacity;
    u8 pad_0C[0x20];
    u32 transform;
    u32 resource;
    u32 active;
    u16 count;
} EffectStripNode;

EffectStripNode *effCreateStripNode(u32 percent) {
    EffectStripNode *node = sdfAllocAndClearQuadwords(sizeof(EffectStripNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->transform = effCreateTrackSetWithSharedReferences(percent * 4, 2, 0);
    node->resource = effRetainResource(0);
    node->count = 1;
    return node;
}

EffectStripNode *effCreateStripNodeFromGrid(EffGrid *grid) {
    u32 n;
    s32 w = grid->width;
    n = (w ? w : grid->height) * (w ? grid->height : grid->altHeight);
    return effCreateStripNode(n > 100 ? 100 : n);
}

EffectStripNode *effFileResourceReferenceReplace(u8 *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    EffGrid *params = (EffGrid *)(source + 0x20);
    EffectStripNode *node = effCreateStripNodeFromGrid(params);

    memcpy(node->pad_0C, source, sizeof(node->pad_0C));
    effReplaceFileResourceRef(node, ((FileJob *)work)->option, params);
    return node;
}

void effReleaseModelResources(u8 *work) {
    void *resource = (void *)((EffectStripNode *)work)->resource;
    if (resource != NULL) {
        billDispatchByKind(resource);
    }
    if (((EffectStripNode *)work)->transform != 0) {
        effReleaseResourceRefs(((EffectStripNode *)work)->transform);
    }
    if (((EffectStripNode *)work)->active != 0) {
        fileReleaseGridRecordHandle(((EffectStripNode *)work)->active);
    }
    sdfReleaseChipBlock(work);
}

extern void effReplaceFileResourceRef(EffectStripNode *, u32, void *);

EffectStripNode *effCloneStripResourceFromOwner(u8 *work) {
    EffGrid *params = ((EffGridRecord *)((EffectStripNode *)work)->active)->params;
    EffectStripNode *node = effCreateStripNodeFromGrid(params);
    memcpy(node->pad_0C, params, sizeof(node->pad_0C));
    effReplaceFileResourceRef(node, ((EffGridRecord *)((EffectStripNode *)work)->active)->kind, params);
    return node;
}

void effReplaceFileResourceRef(EffectStripNode *node, u32 entryId, void *resource) {
    if (node->active != 0) {
        fileReleaseGridRecordHandle(node->active);
    }
    node->active = fileAllocateGridRecordSlots((u16)entryId, node->percent, resource);
}

void effClearStripRecordReferences(s32 node) {
    if (((EffectStripNode *)node)->active != 0) {
        fileClearRecordReferences(((EffectStripNode *)node)->active);
        return;
    }
}

void effAcquireStripRecord(s32 node) {
    if (((EffectStripNode *)node)->active != 0) {
        fileAcquireRecord(((EffectStripNode *)node)->active);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AB290);

void func_002AB968(s32 node) {
    effAcquireStripRecord(node);
    func_002AB290(node);
}

void effSetStripRecordVector(s32 node) {
    mnuRecordSetVector(((EffectStripNode *)node)->active);
}

void effSetStripRecordSecondaryVector(s32 node) {
    fileSetRecordSecondVector(((EffectStripNode *)node)->active);
}

void effSetStripRecordColor(s32 node, u32 value) {
    ((EffectStripNode *)node)->color = value;
}

void func_002AB9C8(ValPtr34 *p, float v) {
    p->f08 = v;
    dds3DispatchIndexedCallback(p->p34, v);
}

void effResetBillTable(u8 *work) {
    u32 index = 0;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    s32 *entry = *(s32 **)state;
    s32 *flags = (s32 *)((EffFrameAsset *)((EffFrameState *)state)->asset)->frameStorage;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x30);
        } while (index < count);
    }
}

typedef struct EffectNodeHeader {
    u8 *entries;
    u32 unk_04;
    u8 *allocation;
} EffectNodeHeader;

u8 *effAllocateRingFadeEntries(u8 *config) {
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x30 + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        ((EffBillConfig *)config)->resourceId = 3;
    }
    return (u8 *)node;
}

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
typedef struct EffectFadeTable {
    u8 pad00[0x24];
    f32 *radii;
    u32 *colors;
} EffectFadeTable;

typedef struct EffectFadeConfig {
    u8 pad00[0x38];
    u32 layers;
    u8 pad3C[0x3C];
    f32 fadeIn;
    f32 fadeOut;
    u8 pad80[0xC];
    s32 segments;
    f32 radius;
} EffectFadeConfig;

void effFillRingFadeGradient(u8 *node, u8 *config) {
    EffectFadeConfig *cfg = (EffectFadeConfig *)config;
    EffectFadeTable *table;
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

    layers = cfg->layers;
    if (layers == 0) {
        return;
    }
    segments = cfg->segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffectFadeTable *)((EffFrameState *)node)->asset;
    colors = table->colors;
    radii = table->radii;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->fadeOut;
    fadeIn = cfg->fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->radius / 3.0f;
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

extern u32 effCreateRibbonWithSharedResource(u32, u32, u32);

u8 *effCreateRingFadeTable(u8 *config, u32 resource) {
    u8 *node = effAllocateRingFadeEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(((EffBillConfig *)config)->frames.count, ((EffBillConfig *)config)->resourceId, resource);
    effFillRingFadeGradient(node, config);
    return node;
}

extern u32 effCloneRibbonWithSharedResource(u32);

u8 *effAssetPointerSet(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = effAllocateRingFadeEntries(config);
    ((EffFrameState *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((u32)((EffFrameState *)state)->asset);
    effFillRingFadeGradient(node, config);
    return node;
}

void effReleaseRingFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002ABDE0);

extern void func_002AE498(u8 *, void *);

void effBlendBillboardInstanceColorsAndTransforms(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->color = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->alternateMode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002AE498(out, mtx);
}

void effResetBillboardFrameInstanceCounters(u8 *work) {
    u32 index = 0;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    s32 *entry = *(s32 **)state;
    s32 *flags = (s32 *)((EffFrameAsset *)((EffFrameState *)state)->asset)->frameStorage;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x30);
        } while (index < count);
    }
}

u8 *effAllocateBillFadeFrameEntries(u8 *config) {
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x30 + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        ((EffBillConfig *)config)->resourceId = 3;
    }
    return (u8 *)node;
}

void effFillBillFadeGradient(u8 *node, u8 *config) {
    EffectFadeConfig *cfg = (EffectFadeConfig *)config;
    EffectFadeTable *table;
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

    layers = cfg->layers;
    if (layers == 0) {
        return;
    }
    segments = cfg->segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffectFadeTable *)((EffFrameState *)node)->asset;
    colors = table->colors;
    radii = table->radii;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->fadeOut;
    fadeIn = cfg->fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->radius / 3.0f;
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

u8 *effCreateBillFadeTable(u8 *config, u32 resource) {
    u8 *node = effAllocateBillFadeFrameEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(((EffBillConfig *)config)->frames.count, ((EffBillConfig *)config)->resourceId, resource);
    effFillBillFadeGradient(node, config);
    return node;
}

u8 *effCloneBillFadeTable(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = effAllocateBillFadeFrameEntries(config);
    ((EffFrameState *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((u32)((EffFrameState *)state)->asset);
    effFillBillFadeGradient(node, config);
    return node;
}

void effReleaseBillFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002ACA40);

void effBillBlendCellColorAndUpdateTransform(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->color = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->alternateMode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002AE498(out, mtx);
}

void effResetParticleBillFrameCounters(u8 *work) {
    u32 index = 0;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    s32 *entry = *(s32 **)state;
    s32 *flags = (s32 *)((EffFrameAsset *)((EffFrameState *)state)->asset)->frameStorage;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x2C);
        } while (index < count);
    }
}

u8 *effAllocateCompactRingFadeEntries(u8 *config) {
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x2C + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        ((EffBillConfig *)config)->resourceId = 3;
    }
    return (u8 *)node;
}

void effFillCompactRingFadeGradient(u8 *node, u8 *config) {
    EffectFadeConfig *cfg = (EffectFadeConfig *)config;
    EffectFadeTable *table;
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

    layers = cfg->layers;
    if (layers == 0) {
        return;
    }
    segments = cfg->segments;
    entries = segments + 1;
    words = entries * 4;
    table = (EffectFadeTable *)((EffFrameState *)node)->asset;
    colors = table->colors;
    radii = table->radii;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->fadeOut;
    fadeIn = cfg->fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->radius / 3.0f;
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

u8 *effCreateCompactRingFadeTable(u8 *config, u32 resource) {
    u8 *node = effAllocateCompactRingFadeEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(((EffBillConfig *)config)->frames.count, ((EffBillConfig *)config)->resourceId, resource);
    effFillCompactRingFadeGradient(node, config);
    return node;
}

u8 *effCloneBillboardFrameAsset(u8 *work) {
    u8 *config = ((EffBillFrameWork *)work)->config;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u8 *node = effAllocateCompactRingFadeEntries(config);

    ((EffFrameState *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((u32)((EffFrameState *)state)->asset);
    effFillCompactRingFadeGradient(node, config);
    return node;
}

void effReleaseCompactRingFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AD6B8);

void effUpdateCompactRingDrawColorAndTransform(u8 *work) {
    u8 *config = ((BillCellDrawWork *)work)->config;
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffBillOutput *)out)->color = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->alternateMode;
    VU0_LOAD_VF(vf10, work + 0x10);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002AE498(out, mtx);
}

u8 *effAllocateBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037ED08[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return effect;
}

extern u8 *effAllocateBlock(u16, void *);

u8 *effCreateResourceInstanceB(u16 kind, void *source, u32 option) {
    u8 *effect = effAllocateBlock(kind, source);
    ((EffClassWork *)effect)->resource = D_0037ED08[kind].createResource(source, option);
    D_0037ED08[kind].initialize(effect);
    return effect;
}

u8 *effCreateFileResourceInstanceB(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer(work);
    void *source;
    switch (((FileJob *)work)->slots[0].selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(work);
    return effCreateResourceInstanceB(((FileJob *)work)->option, source, (u32)secondary);
}

void effDestroyBlockResourceWork(EffClassWork *work) {
    D_0037ED08[work->kind].destroyResource();
    sdfReleaseChipBlock(work);
}

u8 *effDuplicateActiveResourceB(EffClassWork *work) {
    u8 *effect = effAllocateBlock(work->kind, work->payload);
    u32 active = D_0037ED08[work->kind].cloneResource(work);
    s32 kind = work->kind;
    ((EffClassWork *)effect)->resource = active;
    D_0037ED08[kind].initialize(effect);
    return effect;
}

void effResetBlockResourceFrame(EffClassWork *work) {
    D_0037ED08[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceBlockResourceFrame(EffClassWork *work) {
    D_0037ED08[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDrawBlockResourceWork(EffClassWork *work) {
    D_0037ED08[work->kind].draw(work);
}

void effUpdateAndDrawBlockResource(EffClassWork *work) {
    effAdvanceBlockResourceFrame(work);
    effDrawBlockResourceWork(work);
}

void effCopyBlockResourcePosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyBlockResourceOrientation(void *work, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)work)->vectors.orientation, src);
}

void effSetBlockResourceColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetBlockResourceMatrixComponent(EffClassWork *work, float value) {
    work->scale = value;
}

typedef struct EffRibbonWork {
    u32 count;      // 0x00
    u32 field_04;   // 0x04
    u32 color;      // 0x08
    s32 rowStride;  // 0x0C
    s32 repeat;     // 0x10
    u8 field_14;    // 0x14
    u8 pad_15[3];
    RefObj *resource; // 0x18: null selects the globally shared wind texture
    u32 *colors;    // 0x1C
    u8 *positions;  // 0x20
    u8 *uvs;        // 0x24
    u8 *extra;      // 0x28
    s32 *handle;    // 0x2C
    u8 *allocation; // 0x30
} EffRibbonWork;

extern EffMotionSetup D_003DCB00;

u8 *effCreateRibbonWork(u32 count, u32 repeat) {
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x1C + 4) * count;
    u32 cells = rowStride * count;
    u8 *allocation = sdfAllocGeneralBlock(size + 0x34);
    u8 *p = (u8 *)sdfResourceRetainAddress((u32)allocation);
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
    func_002DA420(work->handle, 1.0f);
    memset(&D_003DCB00, 0, sizeof(EffMotionSetup));
    D_003DCB00.flags = 0x4000;
    return (u8 *)work;
}

extern u8 *effCreateRibbonWork(u32, u32);

u32 effCreateRibbonWithSharedResource(u32 count, u32 repeat, u32 resource) {
    u8 *node = effCreateRibbonWork(count, repeat);

    if (resource == 0) {
        s32 references = effSharedRibbonReferenceCount;
        ((EffRibbonWork *)node)->resource = NULL;
        if (references == 0) {
            D_003BC990 = effCloneSharedReferenceWithValue(effWindTextureHandle, 0x300);
            references = effSharedRibbonReferenceCount;
        }
        references++;
        effSharedRibbonReferenceCount = references;
    } else {
        ((EffRibbonWork *)node)->resource = func_0029BD90((void *)resource);
    }
    return (u32)node;
}

void effSharedAssetReferenceRelease(s32 work) {
    if (((EffRibbonWork *)work)->resource == NULL) {
        effSharedRibbonReferenceCount = effSharedRibbonReferenceCount - 1;
        if (effSharedRibbonReferenceCount == 0) {
            effReleaseSharedReference(D_003BC990);
            D_003BC990 = 0;
        }
    }
    else {
        effReleaseSharedReference(((EffRibbonWork *)work)->resource);
    }
    sdfQueueAssetRelease(((EffRibbonWork *)work)->handle);
    sdfReleaseResourceAllocation(((EffRibbonWork *)work)->allocation);
}

u32 effCloneRibbonWithSharedResource(u32 work) {
    u8 *node = effCreateRibbonWork(((EffRibbonWork *)work)->count, ((EffRibbonWork *)work)->repeat);
    RefObj *texture = ((EffRibbonWork *)work)->resource;

    if (texture != NULL) {
        ((EffRibbonWork *)node)->resource = effRetainSharedReference(texture);
    } else {
        effSharedRibbonReferenceCount++;
        ((EffRibbonWork *)node)->resource = NULL;
    }
    return (u32)node;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AE498);

void effLoadWindTexture(void) {
    D_003BC984 = sdfReadNamedResource("/effect/wind00.tmx", &effWindTextureHandle, 0);
}

u32 effGetWindTextureHandle(void) {
    return effWindTextureHandle;
}

void effResetAnimationFrameEntries(u8 *work) {
    u32 index = 0;
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    s32 *entry = *(s32 **)state;
    s32 *flags = ((EffFrameAsset *)((EffFrameState *)state)->asset)->animationFrames;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x30);
        } while (index < count);
    }
}

u32 *effAllocateAnimationBuffer(u8 *work) {
    u32 age;
    u32 *buffer;
    void *allocation = sdfAllocGeneralBlock(((EffBillConfig *)work)->frames.count * 0x30 + 0xC);

    buffer = (u32 *)sdfResourceRetainAddress((u32)allocation);
    age = ((EffBillConfig *)work)->resourceId;

    buffer[0] = (u32)(buffer + 3);
    buffer[2] = (u32)allocation;
    if (age < 3) {
        ((EffBillConfig *)work)->resourceId = 3;
    }
    return buffer;
}

void effFillFadeColorRows(u8 *work, u8 *config) {
    EffBillConfig *cfg = (EffBillConfig *)config;
    u32 rows = cfg->frames.count;
    u32 i;

    if (rows != 0) {
        s32 width = cfg->resourceId;
        f32 fw = width;
        s32 fadeInEnd = cfg->fadeInEnd * fw;
        s32 fadeOutStart = cfg->fadeOutStart * fw;
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

extern u32 effCreateTexturedStripWithSharedTexture(u32, u32);

u32 *effPrepareTextureAnimation(u8 *work) {
    u32 *buffer = effAllocateAnimationBuffer(work);

    buffer[1] = effCreateTexturedStripWithSharedTexture(((EffBillConfig *)work)->frames.count, ((EffBillConfig *)work)->resourceId);
    effFillFadeColorRows(buffer, work);
    return buffer;
}

extern u32 effAllocateStripFromWorkAndRetainTexture(u8 *);

u32 *effPrepareOwnedTextureAnimation(u8 *work) {
    u8 *anim = ((EffBillFrameWork *)work)->config;
    u8 *owner = ((EffBillFrameWork *)work)->frameState;
    u32 *buffer = effAllocateAnimationBuffer(anim);

    buffer[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)owner)->asset);
    effFillFadeColorRows(buffer, anim);
    return buffer;
}

void effReleaseTextureAnimationWork(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effReleaseScalyStripResources((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AEC60);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void func_002AF370(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffMeshOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffMeshOutput *)out)->mode = ((EffBillConfig *)config)->alternateMeshMode;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002B0B70(out, mtx);
}

extern float effMiscRandUnitFloat(void *);

/* Per-animation allocation: positions follow this four-word owner header. */
typedef struct EffAnimationState {
    f32 *positions;       // 0x00
    u32 textureHandle;    // 0x04
    u32 record;           // 0x08
    u32 allocation;       // 0x0C
} EffAnimationState;

void effInitializeAnimationPositions(u8 *work) {
    u8 *resource = ((EffBillFrameWork *)work)->frameState;
    u8 *payload = (u8 *)((EffAnimationState *)resource)->record;
    float *positions = ((EffAnimationState *)resource)->positions;
    u32 count = ((EffGridRecord *)payload)->count;
    u32 i = 0;

    fileClearRecordReferences((s32)payload);
    for (; i < count; i++) {
        positions[0] = effMiscRandUnitFloat(effSharedRandomState);
        positions[1] = effMiscRandUnitFloat(effSharedRandomState);
        positions += 2;
    }
}

u32 effClampSlotCount(EffGrid *grid) {
    u32 n;
    s32 w = grid->width;
    n = (w ? w : grid->height) * (w ? grid->height : grid->altHeight);
    return n > 200 ? 200 : n;
}

u32 *effCreateAnimationState(u32 unused, u32 count) {
    u32 allocation = (u32)sdfAllocGeneralBlock(count * 8 + 0x10);
    u32 *state = (u32 *)sdfResourceRetainAddress(allocation);

    ((EffAnimationState *)state)->allocation = allocation;
    ((EffAnimationState *)state)->positions = (f32 *)(state + 4);
    ((EffAnimationState *)state)->record = 0;
    ((EffAnimationState *)state)->textureHandle = effRetainScalyTextureReference();
    return state;
}

u32 *effActivateAnimationState(s32 work) {
    s32 owner = (s32)((EffBillFrameWork *)work)->frameState;
    s32 resource = ((EffAnimationState *)owner)->record;
    u32 *state = effCreateAnimationState((u32)((EffBillFrameWork *)work)->config, ((EffGridRecord *)resource)->count);

    resource = ((EffAnimationState *)owner)->record;
    ((EffAnimationState *)state)->record = fileAllocateGridRecordSlots(((EffGridRecord *)resource)->kind, ((EffGridRecord *)resource)->count,
                               ((EffGridRecord *)resource)->params);
    return state;
}

extern void effReleaseScalyTextureReference(s32);

void effReleaseAnimationFrameResources(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;

    effReleaseScalyTextureReference(((EffAnimationState *)state)->textureHandle);
    if (((EffAnimationState *)state)->record != 0) {
        fileReleaseGridRecordHandle(((EffAnimationState *)state)->record);
    }
    sdfReleaseResourceAllocation(((EffAnimationState *)state)->allocation);
}

void effSynchronizeFileTransform(u8 *work) {
    u8 *state = ((EffBillFrameWork *)work)->frameState;
    u32 handle = ((EffAnimationState *)state)->record;

    if (handle != 0) {
        mnuRecordSetVector(handle, work);
        fileSetRecordSecondVector(((EffAnimationState *)state)->record, work + 0x10);
        dds3DispatchIndexedCallback((u16 *)((EffAnimationState *)state)->record, ((EffClassWork *)work)->scale);
        fileAcquireRecord(((EffAnimationState *)state)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AF6F8);

extern u32 effClampSlotCount(EffGrid *);

u32 *effCreatePrimarySlotAnimationState(u8 *work) {
    EffGrid *mapping = (EffGrid *)(work + 0x3C);
    u32 count = effClampSlotCount(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    ((EffAnimationState *)state)->record = fileAllocateGridRecordSlots(1, count, mapping);
    return state;
}

u32 *effCreateAlternateSlotAnimationState(u8 *work) {
    EffGrid *mapping = (EffGrid *)(work + 0x3C);
    u32 count = effClampSlotCount(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    ((EffAnimationState *)state)->record = fileAllocateGridRecordSlots(3, count, mapping);
    return state;
}

void effResetSlotAnimationRecord(s32 work) {
    ((EffFrameAsset *)((EffFrameState *)((EffBillFrameWork *)work)->frameState)->asset)->frameCount = 0;
}

u32 *effAllocateQuantizedBuffer(u8 *work) {
    void *allocation = sdfAllocGeneralBlock(0xC);
    u32 *buffer = (u32 *)sdfResourceRetainAddress((u32)allocation);
    u32 count = ((EffBillConfig *)work)->samples.quantizedSamples;

    buffer[2] = (u32)allocation;
    if (count < 4) {
        ((EffBillConfig *)work)->samples.quantizedSamples = 4;
        count = 4;
    }
    buffer[0] = count >> 2;
    if ((((EffBillConfig *)work)->samples.quantizedSamples & 3) != 0) {
        buffer[0] = (count >> 2) + 1;
    }
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002AFE68);

extern void func_002AFE68(u32 *, u8 *);

u32 *effPrepareQuantizedTexture(u8 *work) {
    u32 *buffer = effAllocateQuantizedBuffer(work);

    buffer[1] = effCreateTexturedStripWithSharedTexture(buffer[0], ((EffBillConfig *)work)->samples.quantizedSamples);
    func_002AFE68(buffer, work);
    return buffer;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *work) {
    u8 *anim = ((EffBillFrameWork *)work)->config;
    u8 *owner = ((EffBillFrameWork *)work)->frameState;
    u32 *buffer = effAllocateQuantizedBuffer(anim);

    buffer[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)owner)->asset);
    func_002AFE68(buffer, anim);
    return buffer;
}

void effReleaseBillboardFrameAsset(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effReleaseScalyStripResources((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

typedef struct EffStripWork {
    u32 count;          // 0x00
    u32 type;           // 0x04
    u32 color;          // 0x08
    s32 rowStride;      // 0x0C
    s32 repeat;         // 0x10
    u8 flag;            // 0x14
    u8 pad_15[3];
    u32 *colors;        // 0x18
    u8 *positions;      // 0x1C
    u8 *uvsA;           // 0x20
    u8 *uvsB;           // 0x24
    u8 *extra;          // 0x28
    s32 *handle;        // 0x2C
    u8 *allocation;     // 0x30
} EffStripWork;

void effOffsetNodeRowsVU(u8 *work) {
    s32 *list = (s32 *)((EffBillFrameWork *)work)->frameState;
    u8 *config = ((EffBillFrameWork *)work)->config;
    s32 rows = ((EffBillConfig *)config)->samples.signedRows + 1;
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
                *v += ((EffBillConfig *)config)->rowOffset;
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
    u32 progress = ((EffBillConfig *)config)->drawProgress;
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
    second = func_00296F58(config, config + 0x24, limit, progress);
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
    ((EffMeshOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffMeshOutput *)out)->mode = ((EffBillConfig *)config)->meshMode;
    scale = func_00297270(&((EffBillConfig *)config)->scaleCurve, limit, progress) * work->scale;
    VU0_LOAD_VF(vf10, work->transform);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E0E0);
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002B0B70(out, mtx);
}

u8 *effAllocateBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037ED90[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return effect;
}

extern u8 *effAllocateBlockWithModel(u16, void *);

u8 *effCreateResourceInstanceC(u16 kind, void *source) {
    u8 *effect = effAllocateBlockWithModel(kind, source);
    ((EffClassWork *)effect)->resource = D_0037ED90[kind].createResource(source);
    D_0037ED90[kind].initialize(effect);
    return effect;
}

void effResourceInstanceCreateFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effCreateResourceInstanceC(((FileJob *)request)->option, source);
}

void effDispatchCleanupOp(EffClassWork *work) {
    D_0037ED90[work->kind].destroyResource();
    sdfReleaseChipBlock(work);
}

u8 *effRecreateActiveByClass(EffClassWork *work) {
    u8 *effect = effAllocateBlockWithModel(work->kind, work->payload);
    u32 resource = D_0037ED90[work->kind].cloneResource(work);
    s32 kind = work->kind;
    ((EffClassWork *)effect)->resource = resource;
    D_0037ED90[kind].initialize(effect);
    return effect;
}

void effResetModelBlockFrame(EffClassWork *work) {
    D_0037ED90[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceDispatchCounter(EffClassWork *work) {
    D_0037ED90[work->kind].update(work);
    work->frame++;
}

void effDrawModelBlock(EffClassWork *work) {
    D_0037ED90[work->kind].draw(work);
}

void effUpdateAndDrawModelBlock(EffClassWork *work) {
    effAdvanceDispatchCounter(work);
    effDrawModelBlock(work);
}

void effCopyModelBlockPosition(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyModelBlockOrientation(void *work, void *src) {
    PCP_COPY_VECTOR(((EffClassWork *)work)->vectors.orientation, src);
}

void effSetModelBlockColor(EffClassWork *work, u32 color) {
    work->color = color;
}

void effSetModelBlockMatrixComponent(EffClassWork *work, float value) {
    work->scale = value;
}

/* Retain and return the lazy shared reference to /effect/scaly00.tmx. */
u32 effRetainScalyTextureReference(void) {
    if (effSharedStripReferenceCount == 0) {
        effSharedScalyStripResource = effCloneSharedReferenceWithValue(effScalyTextureHandle, 0x200);
    }
    effSharedStripReferenceCount = effSharedStripReferenceCount + 1;
    return effSharedScalyStripResource;
}

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(s32 unused) {
    effSharedStripReferenceCount = effSharedStripReferenceCount - 1;
    if (effSharedStripReferenceCount == 0) {
        effReleaseSharedReference(effSharedScalyStripResource);
        effSharedScalyStripResource = 0;
    }
}

extern EffMotionSetup D_003DCB30;

/* Allocate position/UV/color rows and a draw handle using count and repeat. */
u8 *effAllocateTexturedStripWork(count, repeat)
u32 count;
u32 repeat;
{
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x24 + 4) * count;
    u32 cells = rowStride * count;
    u8 *allocation = sdfAllocGeneralBlock(size + 0x34);
    u8 *p = (u8 *)sdfResourceRetainAddress((u32)allocation);
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
    func_002DA420(work->handle, 1.0f);
    memset(&D_003DCB30, 0, sizeof(EffMotionSetup));
    D_003DCB30.flags = 0x4000;
    return (u8 *)work;
}

/* Create a scaly strip and retain its shared texture reference. */
u32 effCreateTexturedStripWithSharedTexture(u32 count, u32 repeat) {
    u32 strip = effAllocateTexturedStripWork(count, repeat);
    effRetainScalyTextureReference();
    return strip;
}

void effReleaseScalyStripResources(s32 work) {
    effReleaseScalyTextureReference(effSharedScalyStripResource);
    sdfQueueAssetRelease(((EffStripWork *)work)->handle);
    sdfReleaseResourceAllocation(((EffStripWork *)work)->allocation);
}

/* Recreate the source strip's dimensions and retain another shared texture reference. */
u32 effAllocateStripFromWorkAndRetainTexture(u8 *work) {
    u32 strip = effAllocateTexturedStripWork(((EffStripWork *)work)->count, ((EffStripWork *)work)->repeat);
    effSharedStripReferenceCount++;
    return strip;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B0B70);

void effLoadScalyTexture(void) {
    D_003BC994 = sdfReadNamedResource("/effect/scaly00.tmx", &effScalyTextureHandle, 0);
}

u32 effGetScalyTextureHandle(void) {
    return effScalyTextureHandle;
}

typedef struct EffSpanEntry {
    f32 first;
    f32 second;
    u32 pad_08;
} EffSpanEntry;

typedef struct EffModelResource {
    u8 transform[0x20];
    f32 scale;
    u32 color;
    s32 updateCount;
    s32 kind;
    MdlCtx *model;
    u32 attributes;
    u32 childResource;
    void *source;
} EffModelResource;
typedef struct EffSpanRecord {
    EffSpanEntry *entries;
    EffPointSet *pointSet;
    u32 references;
    u32 pointCount;
} EffSpanRecord;

typedef struct EffSpanTable {
    EffSpanRecord *records;
    u32 count;
    u16 total;
    u8 pad0A[2];
    u32 allocation;
} EffSpanTable;

typedef struct EffSpanConfig {
    u8 pad00[0x28];
    u32 pointSetType;
    u8 pad2C[8];
    u32 progress;
    u8 drawPoints;
    u8 pad39[3];
    u32 middleColor;
    u32 edgeColor;
    u8 pad44[4];
    f32 drawScale;
    u8 pad4C[0x2C];
    u32 referenceType;
    u8 pad7C[0x0C];
    u8 drawReferences;
    u8 pad89[0x17];
    f32 firstRand;
    f32 secondBase;
    f32 rangeRand;
    u32 unkAC;
    u32 perSpan;
    u8 padB4[8];
    u8 pointSetFlag;
} EffSpanConfig;


void effSeedParticleSpanParameters(u8 *work) {
    u32 index = 0;
    EffSpanTable *table = (EffSpanTable *)((EffModelResource *)work)->childResource;
    EffSpanConfig *config = ((EffModelResource *)work)->source;
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
                    entry->pad_08 = 0;
                    entry++;
                } while (span < spans);
            }
            index++;
            record++;
        } while (index < table->count);
    }
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B12D8);

extern void effReleaseModelPointSetAsset(s32);

void effReleaseParticleList(u8 *list) {
    u32 i;
    EffSpanRecord *entry = ((EffSpanTable *)list)->records;

    for (i = 0; i < ((EffSpanTable *)list)->count; i++, entry++) {
        effReleaseModelPointSetAsset((s32)entry->pointSet);
        if (entry->references != 0) {
            effReleaseResourceRefs(entry->references);
        }
    }
    sdfReleaseResourceAllocation(((EffSpanTable *)list)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B1560);

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B1D68);


u32 effCreateModelResourceWithInlineData(u16 kind, void *source, void *secondary, u32 param) {
    u32 headerSize = 0x40;
    u32 size = effModelResourceOperations[kind].payloadSize;
    EffModelResource *effect = (EffModelResource *)sdfAllocSizeClassBlock(size + headerSize);

    effect->source = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->updateCount = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, &effect->transform[0x10]);
    memcpy(effect->source, source, size);
    if (secondary != NULL) {
        effect->model = effLoadViewerModelWithVUState(secondary, param);
        effect->attributes = param;
        effect->childResource = effModelResourceOperations[kind].createResource(effect->source, effect->model);
        effModelResourceOperations[kind].initialize(effect);
    }
    return (u32)effect;
}

u32 effCreateModelResourceFromFile(u8 *work) {
    void *first = fileResolvePrimaryBuffer(work);
    void *second = fileResolveSecondaryBuffer(work);
    return effCreateModelResourceWithInlineData(((FileJob *)work)->option, first, second, ((FileJob *)work)->slots[1].size);
}

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    MdlCtx *assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} EffModelCreateRequest;

/* Battle actor subset used by effect filters (matches the battle unit offsets). */

void effDestroyModelResource(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].destroyResource((void *)effect->childResource);
    effDestroyModelContext(effect->model);
    sdfReleaseChipBlock(effect);
}

EffModelResource *effCreateModelResource(EffModelCreateRequest *work) {
    EffModelResource *effect = (EffModelResource *)effCreateModelResourceWithInlineData(work->kind, work->source, 0, 0);
    s32 x = mdlGetContextResourceGroup(work->assetId);
    s32 y = mdlGetContextResourceId(work->assetId);
    MdlCtx *model = func_00217680(x, y);

    effect->model = model;
    effInitModelVUState(model);
    effect->attributes = work->attributes;
    effect->childResource = effModelResourceOperations[effect->kind].createResource(effect->source, effect->model);
    effModelResourceOperations[effect->kind].initialize(effect);
    return effect;
}

void effResetModelResourceUpdateCount(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].initialize(effect);
    effect->updateCount = 0;
}

void effAdvanceModelResourceUpdateCount(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].update(effect);
    effect->updateCount = effect->updateCount + 1;
}

void effDispatchModelResourceCallback(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].draw(effect);
}

void effStepModelResourceCallbacks(EffModelResource *effect) {
    effAdvanceModelResourceUpdateCount(effect);
    effDispatchModelResourceCallback(effect);
}

void effSetModelResourcePrimaryTransformVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetModelResourceSecondaryTransformVector(void *work, void *src) {
    PCP_COPY_VECTOR(&((EffModelResource *)work)->transform[0x10], src);
}

void effSetModelResourceColor(s32 work, u32 color) {
    ((EffModelResource *)work)->color = color;
}

void effSetModelResourceScale(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

extern EffPacketParams D_003DCB60[];

EffPointSet *effCreatePointSet3(s32 count) {
    s32 rows = count * 3 + 3;
    s32 size = rows * 20;
    u8 *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x20);
    data = (u8 *)sdfResourceRetainAddress((u32)base);
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
    func_002DA420(set->handle, 1.0f);
    memset(D_003DCB60, 0, 0x2C);
    D_003DCB60[0].primitive = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleaseModelPointSetAsset(s32 work) {
    sdfQueueAssetRelease((u32)((EffPointSet *)work)->handle);
    sdfReleaseResourceAllocation((u32)((EffPointSet *)work)->allocation);
}

void effDrawThreePointGroups(EffPointSet *set, Matrix4 *matrix) {
    void *list;
    EffGsPacket *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        if (matrix == NULL) {
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdfConsAppendVuPacket(list, 0);
        sdfConsAppendAssetPacket(list, set->handle, 0);
        if (set->flag == 0) {
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        remaining = set->rows;
        D_003DCB60[0].colors = (u32 *)set->tail;
        D_003DCB60[0].positions = (u128 *)set->buffer;
        D_003DCB60[0].unk08 = set->color;
        D_003DCB60[0].parameterCount = 0x10;
        D_003DCB60[0].vertexCount = 0xF;
        D_003DCB60[0].parameters = (u32 *)D_0037EB90;
        while (remaining >= 0xF) {
            remaining -= 0xC;
            sdfAppendPacket(list, func_0015FE20(D_003DCB60));
            D_003DCB60[0].positions += 0xC;
            D_003DCB60[0].colors += 0xC;
        }
        if (remaining >= 6) {
            D_003DCB60[0].parameterCount = 4;
            D_003DCB60[0].vertexCount = remaining;
            sdfAppendPacket(list, func_0015FE20(D_003DCB60));
        }
        if (set->flag == 0) {
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        D_0037EE68[set->type]->append((SdfListHead *)D_0037EE68[set->type], list);
    }
}


extern u32 effBTLFieldColorGetOriginalSelector(void);

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOverrideSelector(void);

extern u32 effBTLFieldColorGetFinalSelector(void);

extern void effBattleMiscQueryPosition(u32, void *, void *);

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

        effBattleMiscQueryPosition(handle, &request, vec);
        VU0_LOAD_VF_MEMORY(vf10, vec);
    } else {
        VU0_MOVE_VF(vf10, vf0);
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

        for (link = (u8 *)((BtlState *)state)->units; link != NULL; link = (u8 *)((BtlUnit *)link)->next) {
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
void func_002B2938(u32 unusedResource) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    u32 first[4];
    u32 second[4];

    if ((state->battleFlags & 0x6000000) == 0) {
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
        unit = unit->next;
    }
}


INCLUDE_ASM(const s32, "game/code_0029C530", func_002B2A48);

extern void evtSetUnitRgbTransition(EvtUnit *, s32, u32);

void effSyncLinkedActorChildParameter(void) {
    u8 *state = (u8 *)btlGetRuntime();
    u8 *effect;

    if ((((BtlState *)state)->battleFlags & 0x6000000) == 0) {
        return;
    }
    effect = (u8 *)((BtlState *)state)->units;
    while (effect != NULL) {
        if (((BtlUnit *)effect)->flags & 2) {
            u8 *work = (u8 *)((BtlUnit *)effect)->ext;
            if (work != NULL) {
                ((EvtUnit *)work)->color60 = ((BtlUnit *)effect)->baseColor;
                evtSetUnitRgbTransition((EvtUnit *)work, 0, ((BtlUnit *)effect)->baseColor);
            }
        }
        effect = (u8 *)((BtlUnit *)effect)->next;
    }
}

/* Header common to the resource-instance constructors and callback dispatchers. */
typedef struct EffActiveResource {
    f32 position[4];
    f32 orientation[4];
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

/* Entire copied payload for resource kind 2. */
typedef struct EffActorTintConfig {
    u32 duration;
    u32 fadeIn;
    u32 fadeOut;
    u32 color;
    u8 actorSelection;
    u8 pad11[3];
} EffActorTintConfig;
typedef char EffActorTintConfigSizeCheck[sizeof(EffActorTintConfig) == 0x14 ? 1 : -1];

/* Apply a temporary tint to eligible actors, then restore their original RGB. */
void func_002B2F20(EffActiveResource *work) {
    BtlUnit *actors[16];
    EffActorTintConfig *config = work->payload;
    u32 frame = work->frame;
    u32 color = config->color;
    u32 count = effCollectModelEffectActors(actors, config->actorSelection);
    u32 i;

    if (frame == 0) {
        for (i = 0; i < count; i++) {
            u32 flags = actors[i]->flags;

            if (flags & 2) {
                if ((flags & 0xE0) == 0) {
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
            u32 flags = actors[i]->flags;

            if (flags & 2) {
                if ((flags & 0xE0) == 0) {
                    evtSetUnitRgbTransition(actors[i]->ext, config->fadeOut, actors[i]->baseColor);
                }
            }
        }
    }
}

extern void mdlLoadPrimaryVectorVU(MdlCtx *);

extern u128 D_003DCBD0[];

extern s8 D_003BC9AC;

extern u128 D_003DCBE0[];

extern u128 D_003DCBA0[];

extern SdfLightSources D_0037EE80;

extern void func_002E1938(void *, SdfLightSources, void *);

s32 effComputeLightDirectionVU(MdlCtx *vector, void *target) {
    s32 result = btlIsRuntimeAllocated();

    if (result != 0) {
        if (D_003BC9AC == 0) {
            return 0;
        }
        mdlLoadPrimaryVectorVU(vector);
        VU0_LOAD_VF($vf11, D_003DCBE0);
        VU0_SUB($vf10, $vf10, $vf11);
        VU0_CLEAR_W(vf10);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF($vf10, D_003DCBA0);
        func_002E1938(target, D_0037EE80, D_003DCBD0);
        return 1;
    }
    return result;
}

extern u128 *D_00324770[];

extern u128 D_003DCB90[];

extern u128 kwlnDefaultColorVector[];

void effResetDefaultColorTables(void) {
    u128 *dst = D_003DCB90;
    u128 *src = D_00324770[0];
    PCP_COPY_VECTOR(dst, src);
    dst++;
    src++;
    PCP_COPY_VECTOR(dst, src);
    PCP_COPY_VECTOR(D_003DCBD0, kwlnDefaultColorVector);
    D_003BC9AC = 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B3178);

/* Copied payload plus the five effect/target slots released together. */
typedef struct EffCopiedPayload {
    u8 *body;            // 0x00
    s32 size;            // 0x04
    u32 state;           // 0x08
    u32 unk0C;
    u32 effects[5];      // 0x10
    u32 targets[5];      // 0x24
    u8 *allocation;      // 0x38
    u8 pad3C[4];
} EffCopiedPayload;

typedef struct EffCopiedPayloadWork {
    u8 pad00[0x30];
    EffCopiedPayload *payload;
    u8 pad34[4];
    u32 parameter;
} EffCopiedPayloadWork;

void effResetObjectSlots(u8 *work) {
    u32 *objects = ((EffCopiedPayloadWork *)work)->payload->targets;
    u32 i;
    for (i = 0; i < 5; i++) {
        u32 object = objects[i];
        if (object != 0) {
            ((EffectObjectFlag *)object)->state = 0;
            ((EffectObjectFlag *)object)->flags = 0;
        }
    }
}

u32 effAllocateCopiedEffectPayload(u32 owner, u32 source, s32 size) {
    u32 headerSize = 0x40;
    u8 *base = sdfAllocGeneralBlock(size + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    if (size <= 0) {
        body = 0;
    }
    ((EffCopiedPayload *)node)->allocation = base;
    ((EffCopiedPayload *)node)->size = size;
    ((EffCopiedPayload *)node)->body = body;
    ((EffCopiedPayload *)node)->state = 0;
    memcpy(body, (void *)source, size);
    return (u32)node;
}


INCLUDE_ASM(const s32, "game/code_0029C530", func_002B3420);

extern void fldRelocatePackedTransferChunk(u32, u32);
extern void func_002B3420(u32);



u32 effCreateInitializedObject(u32 type, u32 parameter, u32 index) {
    u32 *effect = (u32 *)effAllocateCopiedEffectPayload(type, parameter, index);
    u32 child = *effect;
    fldRelocatePackedTransferChunk(child, child + 8);
    func_002B3420((u32)effect);
    return (u32)effect;
}

u32 effCloneEffectPayloadFromOwner(s32 work) {
    u32 effect;

    effect = effAllocateCopiedEffectPayload(((EffCopiedPayloadWork *)work)->parameter, ((EffCopiedPayloadWork *)work)->payload->body,
                                                ((EffCopiedPayloadWork *)work)->payload->size);
    func_002B3420(effect);
    return effect;
}

void effReleaseTargetSlots(u8 *work) {
    u32 *effects = ((EffCopiedPayload *)work)->effects;
    u32 *targets = ((EffCopiedPayload *)work)->targets;
    u32 i;

    for (i = 0; i < 5; i++) {
        if (*targets != 0) {
            dds3FreePathObject(*targets);
        }
        targets++;
        if (*effects != 0) {
            dds3RemoveWorldObjectNode(*effects);
        }
        effects++;
    }
    sdfReleaseResourceAllocation(((EffCopiedPayload *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B3698);


extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern void btlStartMoveOtherUnitsTask(void *, s32);




typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} EffAnimInfo;

void effApplyOverlaySpecs(u8 *work) {
    BtlUnit *objects[16];
    u16 *spec;
    u32 count;
    u32 i;

    if ((s32)((EffActiveResource *)work)->frame > 0) {
        return;
    }
    spec = ((EffActiveResource *)work)->payload;
    count = effCollectModelEffectActors(objects, ((EffAnimInfo *)spec)->actorSelection);
    for (i = 0; i < count; i++) {
        u32 flags = ((BtlUnit *)objects[i])->flags;
        if (flags & 2) {
            if ((flags & 0x20) == 0) {
                if ((((BtlUnit *)objects[i])->unk310 & 0x10) == 0) {
                    if (mdlGetNodeRefHalf(objects[i]->ext->owner, 0) > ((EffAnimInfo *)spec)->id) {
                        btlApplyScaledUnitEffectParameter((u8 *)objects[i], ((EffAnimInfo *)spec)->id, ((EffAnimInfo *)spec)->flags | 0x100, 1.0f);
                        if (((EffAnimInfo *)spec)->loop == 0) {
                            btlStartMoveOtherUnitsTask(objects[i], ((EffAnimInfo *)spec)->id);
                        }
                    }
                }
            }
        }
    }
}

extern void sndLoadAndPlayStationedSe(u32);

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

/* Apply the selected battle state's stored tint vectors immediately when its mask is set. */
void effApplyBattleStateTint(void) {
    s32 actor;

    actor = btlGetRuntime();
    if ((((BtlState *)actor)->battleFlags & 0x6000000) != 0) {
        func_001EFD58(actor + 0x50, actor + 0x60, 0);
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
        func_001EFD58(from, to, colors[2]);
    }
}

void effResetSlots(void) {
    D_003BC9B0[0] = 0;
    D_003BC9B8[0] = 0;
    D_003BC9C0[0] = 0;
    D_003BC9C8[0] = 0;
    D_003BC9B0[1] = 0;
    D_003BC9B8[1] = 0;
    D_003BC9C0[1] = 0;
    D_003BC9C8[1] = 0;
    kwlnPadResetMotorLevelsAndOutput();
}

void effTickSlotVolumeFade(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (D_003BC9C0[i] != 0) {
            if (D_003BC9B8[i] > 0 && D_003BC9B0[i] > 0) {
                f32 ratio = (f32)D_003BC9B8[i] / (f32)D_003BC9B0[i];

                if (D_003BC9C8[i] != 0) {
                    ratio = 1.0f - ratio;
                }
                kwlnPadStartMotor(i, (u8)(D_003BC9C0[i] * ratio), 100);
                D_003BC9B8[i] -= 1;
            } else if (D_003BC9C8[i] == 1) {
                kwlnPadStartMotor(i, D_003BC9C0[i], 100);
            } else {
                kwlnPadStartMotor(i, 0, 0);
                D_003BC9B0[i] = 0;
            }
        }
    }
}

void effSetDormantSlot(u32 index, u8 color, u32 value) {
    if (index < 2) {
        D_003BC9B0[index] = value;
        D_003BC9C0[index] = color;
        D_003BC9C8[index] = 0;
        D_003BC9B8[index] = value;
    }
}

void effSetActiveSlot(u32 index, u8 color, u32 value) {
    if (index < 2) {
        D_003BC9B0[index] = value;
        D_003BC9C0[index] = color;
        D_003BC9C8[index] = 1;
        D_003BC9B8[index] = value;
    }
}

void effResetActiveEffectSlots(void) {
    s32 actor;

    actor = btlGetRuntime();
    if ((((BtlState *)actor)->battleFlags & 0x6000000) != 0) {
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
    elapsed = ((EffActiveResource *)work)->frame;
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
    u32 *slotData;
    u32 elapsed;
    u32 duration;

    btlGetRuntime();
    elapsed = ((EffActiveResource *)work)->frame;
    slotData = ((EffActiveResource *)work)->payload;
    duration = slotData[0];
    if (duration < slotData[3]) {
        return;
    }
    if (elapsed == 0) {
        btlInitTintTransitionResource(slotData[1], ((EffTintTransitionIds *)slotData)->resourceKind);
        duration = slotData[0];
    }
    if (duration != 0 && elapsed == duration - slotData[3]) {
        btlInitTintTransitionDefault(((EffTintTransitionIds *)slotData)->defaultKind);
    }
}

extern f32 D_003BC9A8;

extern void func_001DC2A0(u8 *, f32);

/* Eases D_003BC9A8 (a rotation in radians) along the span's [start, end] degrees. */
void effApplyKeyframeAngle(u8 *work) {
    u8 *state = (u8 *)btlGetRuntime();
    f32 *span = ((EffActiveResource *)work)->payload;
    u32 count = *(u32 *)span;
    f32 value;

    if (count != 0 && count >= ((EffActiveResource *)work)->frame) {
        f32 t = (f32)(s32)((EffActiveResource *)work)->frame / (f32)count;
        value = ((span[2] - span[1]) * t + span[1]) * 0.017453293f;
        func_001DC2A0(state + 0x70, value);
        D_003BC9A8 = value;
    }
}

u8 *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037EEE0[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffActiveResource *)effect)->payload = effect + headerSize;
    ((EffActiveResource *)effect)->color = 0x80808080;
    ((EffActiveResource *)effect)->scale = 1.0f;
    ((EffActiveResource *)effect)->kind.index = kind;
    ((EffActiveResource *)effect)->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect + 0x10);
    memcpy(((EffActiveResource *)effect)->payload, source, size);
    return effect;
}

u32 effCreateResourceInstance(u16 kind, void *source, void *secondary, u32 param) {
    u8 *effect = effAllocateResourcePayload(kind, source);

    if (btlIsRuntimeAllocated() != 0) {
        if (D_0037EEE0[kind].createResource != NULL) {
            ((EffActiveResource *)effect)->resource = D_0037EEE0[kind].createResource(source, secondary, param);
        }
        if (D_0037EEE0[kind].initialize != NULL) {
            D_0037EEE0[kind].initialize(effect);
        }
    }
    return (u32)effect;
}

u32 effCreateActiveResourceFromFile(u8 *work) {
    void *first = fileResolvePrimaryBuffer(work);
    void *second = fileResolveSecondaryBuffer(work);
    return effCreateResourceInstance(((FileJob *)work)->option, first, second, ((FileJob *)work)->slots[1].size);
}

void effDestroyResourceInstance(u8 *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = D_0037EEE0[((EffActiveResource *)work)->kind.signedIndex].destroyResource;
        if (callback != NULL) {
            callback((void *)((EffActiveResource *)work)->resource);
        }
    }
    sdfReleaseChipBlock(work);
}

u8 *effDuplicateActiveResource(u8 *source) {
    u8 *effect;
    u32 kind = ((EffActiveResource *)source)->kind.index;

    if (D_0037EEE0[kind].cloneResource == NULL) {
        effect = (u8 *)effCreateResourceInstance(((EffActiveResource *)source)->kind.shortIndex, ((EffActiveResource *)source)->payload, 0, 0);
    } else {
        u32 resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(((EffActiveResource *)source)->kind.shortIndex, ((EffActiveResource *)source)->payload);
        resource = D_0037EEE0[((EffActiveResource *)source)->kind.signedIndex].cloneResource(source);
        activeKind = ((EffActiveResource *)source)->kind.index;
        ((EffActiveResource *)effect)->resource = resource;
        if (D_0037EEE0[activeKind].initialize != NULL) {
            D_0037EEE0[activeKind].initialize(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(u8 *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = D_0037EEE0[((EffActiveResource *)work)->kind.signedIndex].initialize;
        if (callback != NULL) {
            callback(work);
        }
        ((EffActiveResource *)work)->frame = 0;
    }
}

void effDispatchIndexedCallback(u8 *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = D_0037EEE0[((EffActiveResource *)work)->kind.signedIndex].update;
        if (callback != NULL) {
            callback(work);
        }
        ((EffActiveResource *)work)->frame = (s32)((EffActiveResource *)work)->frame + 1;
    }
}

void effDispatchEnabledCallback(u8 *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = D_0037EEE0[((EffActiveResource *)work)->kind.signedIndex].draw;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void effAdvanceActiveResourceCallbacks(u8 *work) {
    effDispatchIndexedCallback(work);
    effDispatchEnabledCallback(work);
}

void effCopyActiveResourceVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyActiveResourceSecondaryVector(void *work, void *src) {
    PCP_COPY_VECTOR(((EffActiveResource *)work)->orientation, src);
}

void effSetActiveResourceColor(EffActiveResource *work, u32 color) {
    work->color = color;
}

void func_002B4790(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B4798);

/* Alternate particle handles share a header but occupy distinct slots. */
typedef struct EffParticleShared {
    u8 pad00[8];
    u32 state;
    u8 pad0C[0x98];
    u32 billHandle;       // 0xA4
    RefObj *reference;    // 0xA8
} EffParticleShared;

void effReleaseParticleResources(u8 *work) {
    void *particle = (void *)((EffParticleShared *)work)->billHandle;
    if (particle != NULL) {
        billDispatchByKind(particle);
    }
    if (((EffParticleShared *)work)->reference != NULL) {
        effReleaseReferenceHolder(((EffParticleShared *)work)->reference);
    }
    sdfReleaseChipBlock(work);
}

u8 *effCloneParticleSharedResource(u8 *source) {
    u8 *effect = func_002B4798(NULL);
    memcpy(effect + 0xC, source + 0xC, 0x98);
    effReplaceSharedResource(effect, source);
    return effect;
}

void effReplaceSharedResource(u8 *work, u8 *source) {
    u32 resource;
    if (((EffParticleShared *)source)->billHandle != 0) {
        if (((EffParticleShared *)work)->billHandle != 0) {
            billDispatchByKind((void *)((EffParticleShared *)work)->billHandle);
        }
        resource = billCloneObjectRetainingSharedData(((EffParticleShared *)source)->billHandle);
        ((EffParticleShared *)work)->billHandle = resource;
        billMarkKindOneFlag(resource);
        return;
    }
    if (((EffParticleShared *)work)->reference != NULL) {
        effReleaseReferenceHolder(((EffParticleShared *)work)->reference);
    }
    ((EffParticleShared *)work)->reference = effReferenceObjectRetain(((EffParticleShared *)source)->reference);
}

void effResetParticleStateWord(s32 work) {
    ((EffParticleShared *)work)->state = 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B4BA0);

void func_002B5128(u32 *dst, u32 value) {
    *dst = value;
}

void btlInitializeEffectWork(void) {
    D_003BD074 = D_0038F2B8;
    D_003DF8D0[0] = 0;
    D_003BD058 = 0;
    D_003BD955 = 0;
    VU0_STORE_VF($vf0, D_003DF910);
}

s32 btlUpdateEffectWork(void) {
    if (D_003BD074 != NULL) {
        if ((func_002B59A8(0) & 1) == 0) {
            effResetFileResourceStores();
            return 0;
        }
    }
    return 1;
}

void func_002B51A8(void) {
}

void effResetFileResourceStores(void) {
    effResetFileResources();
    effResetFileResourceManager();
}

void effSetBattleOffset(void *src) {
    PCP_COPY_VECTOR(D_003DF910, src);
}

extern s32 effFileQueue;

extern void camFollowOffsetVec(u8 *, void *);


/* Shared view transform contributes the scale used by camera-relative effects. */
typedef struct EffViewScale {
    u8 pad00[0x50];
    f32 quaternion[4]; /* Shared file queue rotation quaternion. */
    u8 pad60[0x14];
    f32 scale;
} EffViewScale;

void effComputeBattleCameraPositionVU(u8 *effect) {
    u128 direction;
    u32 flags = ((FileJob *)effect)->xformFlags;

    if ((flags & 0x18) != 0) {
        camFollowOffsetVec(effect, &direction);
        VU0_LOAD_VF($vf10, &direction);
        flags = ((FileJob *)effect)->xformFlags;
    } else {
        VU0_LOAD_VF($vf10, ((FileJob *)effect)->offset);
    }
    if ((flags & 0x80) != 0) {
        VU0_SCALAR_OP_CLOBBER(((EffViewScale *)effFileQueue)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    }
    VU0_LOAD_VF($vf11, D_003DF910);
    VU0_ADD($vf10, $vf10, $vf11);
    VU0_LOAD_VF($vf11, effFileQueue);
    VU0_ADD($vf10, $vf10, $vf11);
    if ((flags & 4) != 0) {
        VU0_SCALAR_OP_CLOBBER(-5.0f, "vaddx.y vf10, vf0, vf2x");
    }
}

extern s32 effFileQueue;

extern void camAimRotation(void *, void *);

extern void effMiscQuatMultiplyVU(void);

void effMultiplyQuatWithFlag(u8 *effect) {
    if ((((FileJob *)effect)->xformFlags & 0x60) != 0) {
        u128 quaternion;
        camAimRotation(effect, &quaternion);
        VU0_LOAD_VF($vf10, ((EffViewScale *)effFileQueue)->quaternion);
        VU0_LOAD_VF($vf11, &quaternion);
        effMiscQuatMultiplyVU();
    } else {
        VU0_LOAD_VF($vf10, ((EffViewScale *)effFileQueue)->quaternion);
        VU0_LOAD_VF($vf11, ((FileJob *)effect)->quat);
        effMiscQuatMultiplyVU();
    }
}

extern u8 D_003DF9A0[];

extern void effComputeBattleCameraPositionVU(u8 *);

extern void fileJobInvokePositionCallback(void *, void *);

extern void fileJobInvokeRotationCallback(void *, void *);

extern void fileJobInvokeScaleCallback(void *, f32);

extern void fileDispatchJobTypeCallback(void *, u32);

void effApplyBattleCameraToObject(work)
    void *work;
{
    u128 rotation[2];

    effComputeBattleCameraPositionVU(D_003DF9A0);
    VU0_STORE_VF_UNCLOBBERED($vf10, &rotation[0]);
    fileJobInvokePositionCallback(work, &rotation[0]);
    effMultiplyQuatWithFlag(D_003DF9A0);
    VU0_STORE_VF_UNCLOBBERED($vf10, &rotation[1]);
    fileJobInvokeRotationCallback(work, &rotation[1]);
    fileJobInvokeScaleCallback(work, ((FileJob *)D_003DF9A0)->scale * ((EffViewScale *)effFileQueue)->scale);
    fileDispatchJobTypeCallback(work, ((FileJob *)D_003DF9A0)->color);
}

extern void fileJobSetPrimaryData(u32, void *, u32, u16);

/* A 0x20-byte creation command, not the runtime FileJob queue entry. */
typedef struct EffFileJobRequest {
    char *name;
    u16 fileKind;
    u8 pad6[2];
    u16 transferMode;
    u8 padA[2];
    void *output;
    u32 size;
    u8 pad14[4];
    u16 resourceMode;
    u8 pad1A[2];
    u32 relatedResource;
} EffFileJobRequest;

u32 effCreateBattleCameraJob(u8 *request) {
    void *output = ((EffFileJobRequest *)request)->output;
    u32 job;

    if (output != NULL) {
        if (((EffFileJobRequest *)request)->fileKind == 6) {
            memcpy(output, D_003DE148, 0x3C);
            output = ((EffFileJobRequest *)request)->output;
        }
        fileJobSetPrimaryData(effQueuedFileHandle, output, ((EffFileJobRequest *)request)->size, ((EffFileJobRequest *)request)->transferMode);
    }
    job = fileJobCreateFromJob(effQueuedFileHandle);
    effApplyBattleCameraToObject(job);
    return job;
}

extern u32 fileJobCreateFromCommandState(u32);

extern u32 fileCreateJob(u16);

extern void fileJobSetPrimaryData(u32, void *, u32, u16);

extern void fileJobCopyCommandIntoSecondaryData(u32, u32, u32);

extern void fileJobSetSecondaryData(u32, void *, u32, u32);

u32 effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob) {
    u32 job;
    if (existingJob != 0) {
        void *source;
        job = fileJobCreateFromCommandState(existingJob);
        source = fileResolvePrimaryBuffer(job);
        memcpy(request->output, source, request->size);
    } else {
        job = fileCreateJob(request->fileKind);
        if (request->output != NULL) {
            fileJobSetPrimaryData(job, request->output, request->size, request->transferMode);
        }
        if (request->relatedResource != 0) {
            fileJobCopyCommandIntoSecondaryData(job, request->relatedResource, request->resourceMode);
        } else {
            u32 value = 0;
            fileJobSetSecondaryData(job, &value, 4, 4);
        }
    }
    effResetFileResourceManager();
    return job;
}

void effInvokeFileJobWithBattleCamera(u32 job) {
    if (effAuxiliaryFileQueue == 0) {
        effApplyBattleCameraToObject();
        fileJobInvokeTypeCallback(job);
        return;
    }
}

void effFileJobQueueRelease(u32 job) {
    if (effAuxiliaryFileQueue != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    fileJobDestroy(job);
}

extern char D_003BD080[];

extern s32 func_003014F0(char *, const char *, ...);

extern u8 D_003BD078[];

/* Asset selection request and table entry use different ID offsets. */
typedef struct EffAssetRequest {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[6];
    u16 id;         // 0x0C
} EffAssetRequest;

typedef struct EffAssetIdentifier {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[2];
    u16 id;         // 0x08
} EffAssetIdentifier;

u32 effPollPrimaryFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue((u32 *)D_003B39C8, D_003BD078, &record);
    state = record.completion.state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effQueuedFileHandle != 0) {
            func_003014F0(path, D_003BD080, D_003B39C8, record.nameWithPrefix);
            fileWriteToPfs(effQueuedFileHandle, path);
            result = 0x400002;
        }
    }
    return result;
}

void effQueueNamedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_003BD088, D_003DF8D0);
}

extern s32 effFileQueue;

u32 effPollNamedFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue((u32 *)D_003B39E0, D_003BD088, &record);
    state = record.completion.state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            strcpy((char *)D_003DF8D0, record.name);
            func_003014F0(path, D_003BD080, D_003B39E0, record.nameWithPrefix);
            func_00295018(effFileQueue, path);
            result = 0x400002;
        }
    }
    return result;
}

void effQueueAttachedResourceRequest(void) {
    effReleaseQueuedResourceName();
    effQueueResource(D_003BD090, D_003DF8D0);
}

u32 effPollAttachedFile(void) {
    EffQueueRecord record;
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue((u32 *)D_003B39E0, D_003BD090, &record);
    state = record.completion.state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            strcpy((char *)D_003DF8D0, record.name);
            func_003014F0(path, D_003BD080, D_003B39E0, record.nameWithPrefix);
            func_002954F0(effFileQueue, path);
            result = 0x400002;
        }
    }
    return result;
}

typedef struct EffectAssetLink {
    u8 *asset;
    u32 object;
    u8 pad_08[0x14];
} EffectAssetLink;

extern EffectAssetLink *D_0038E9D8[22];

typedef struct EffAssetQuery {
    u8 pad00[0x90];
    u8 *request;
} EffAssetQuery;

u8 *effFindAssetData(u8 *work) {
    u8 *requested = ((EffAssetQuery *)work)->request;
    u16 type = ((EffAssetRequest *)requested)->type;
    u16 id = ((EffAssetRequest *)requested)->id;
    u16 i;
    for (i = 0; i < 22; i++) {
        EffectAssetLink *links = D_0038E9D8[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == id) {
                        return asset;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

u32 effFindAssetObject(u8 *work) {
    u8 *requested = ((EffAssetQuery *)work)->request;
    u16 type = ((EffAssetRequest *)requested)->type;
    u16 id = ((EffAssetRequest *)requested)->id;
    u16 i;
    for (i = 0; i < 22; i++) {
        EffectAssetLink *links = D_0038E9D8[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == id) {
                        return current->object;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return 0;
}

u32 func_002B5990(void) {
    return D_0038F2FC[0] + D_003BD058;
}

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D20);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D30);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D40);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D50);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D60);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D70);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D80);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2D90);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2DA0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2DB0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2DC0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2DD0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2DE0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2DF0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E00);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E10);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E20);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E30);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E40);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E50);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E60);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E70);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E80);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2E90);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2EB0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2ED0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2EF0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2F10);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2F30);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2F50);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2F70);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2F90);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2FB0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2FD0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B2FF0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3010);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3030);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3050);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3070);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3090);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B30B0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B30D0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B30F0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3110);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3130);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3150);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3170);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3190);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B31B0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B31D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B31F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3218);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3238);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3258);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3278);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3298);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B32B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B32D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B32F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3318);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3338);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3358);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3378);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3398);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B33B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B33D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B33F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3418);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3438);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3458);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3478);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3498);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B34B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B34D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B34F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3518);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3538);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3558);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3578);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3598);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B35B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B35D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B35F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3618);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3638);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3658);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3678);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3698);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B36B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B36D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B36F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3718);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3738);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3758);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3778);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3798);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B37B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B37C8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B37D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B37E8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B37F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3808);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3818);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3828);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3838);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3848);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3858);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3868);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3878);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3888);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3898);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B38A8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B38B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B38C8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B38D8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B38E8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B38F8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3908);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3918);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3928);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3938);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3948);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3958);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3968);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3978);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3988);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3998);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B39A8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B39B8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B39C8);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B39E0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B39F0);

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B59A8);

/* Asset object slot linked to the shared effect-file state. */
typedef struct EffQueuedFileObject {
    u8 pad00[0x34];
    u8 *linkedState; /* 0x34 */
} EffQueuedFileObject;


typedef struct EffCameraCreateRequest {
    EffFileJobRequest *resource;
    EffQueuedFileObject *object;
    u32 existingJob;
    f32 position[3];
} EffCameraCreateRequest;

typedef struct FileQueue FileQueue;
extern void fileQueueInitTransform(void *);
extern FileJob *fileAppendJob(FileQueue *, u32);
extern s32 fileQueueCountLinkedJobs(FileQueue *);
extern s32 effCurrentFileQueueEntry;
extern u32 D_003BD064;
extern u32 D_0038F2F0[];

u32 effAppendPositionedCameraFileJob(EffCameraCreateRequest *request) {
    FileJob *entry;
    u32 count;

    fileQueueInitTransform(D_003DF9A0);
    entry = fileAppendJob((FileQueue *)effFileQueue, effLoadFileJobPayload(request->resource, request->existingJob));
    effCurrentFileQueueEntry = (s32)entry;
    strcpy(entry->name, request->resource->name);
    entry->offset[0] = request->position[0];
    entry->offset[1] = request->position[1];
    entry->offset[2] = request->position[2];
    entry->unk88[0] = 8;
    entry->unk88[1] = 0;
    entry->unk88[2] = 0;
    memcpy(D_003DF9A0, entry, 0x80);
    effQueuedFileHandle = entry->id;
    D_003BD064 = effCreateBattleCameraJob((u8 *)request->resource);
    effQueuedFileObject = (s32)request->object;
    count = fileQueueCountLinkedJobs((FileQueue *)effFileQueue);
    if (count > 21) {
        D_0038F2F0[3] = 20;
        D_003BD058 = count - 21;
    } else {
        D_003BD058 = 0;
        D_0038F2F0[3] = count - 1;
    }
    request->object->linkedState = (u8 *)D_0038F2F0;
    effResetFileResourceManager();
    return 0x800000;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B6778);



extern u32 D_0038EA6C[];

extern s32 fileQueueCreate(void);

u32 effReinitializeFileQueue(void) {
    if (effTemporaryFileJob != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    if (effFileQueue != 0) {
        fileQueueDestroy(effFileQueue);
    }
    effFileQueue = fileQueueCreate();
    D_0038EA6C[0] = 0;
    D_003DF8D0[0] = 0;
    D_003BD058 = 0;
    D_0038F2F0[3] = 0;
    effQueuedFileObject = (s32)D_0038F2F0;
    return 0;
}

extern u32 D_0038EB3C[];

extern void *fileQueueGetAt(s32, s32);

extern void fileQueueRemoveAndDestroyJob(s32, void *);

u32 effResetFileQueueState(void) {
    fileQueueRemoveAndDestroyJob(effFileQueue, fileQueueGetAt(effFileQueue, func_002B5990()));
    D_0038EB3C[0] = 0;
    D_003BD058 = 0;
    D_0038F2F0[3] = 0;
    effQueuedFileObject = (s32)D_0038F2F0;
    return 0;
}

extern u32 D_0038EAD4[];

extern void *fileJobDuplicateAfter(s32, void *);

extern void fileJobCopyHeader(void *, void *);

u32 effFinalizeQueuedFile(void) {
    s32 index = func_002B5990();
    void *record = fileQueueGetAt(effFileQueue, index);
    void *copy = fileJobDuplicateAfter(effFileQueue, record);
    D_0038EAD4[0] = 0;
    fileJobCopyHeader(copy, record);
    effQueuedFileObject = (s32)D_0038F2F0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B6C00);

u32 effFlagFileQueueActive(void) {
    D_003BD955 = 1;
    effSelectLinkedFileState(0);
    return 0;
}

/* Select the queued file object's linked state when present; return zero in either case. */
u32 effSelectLinkedFileState(void) {
    if (((EffQueuedFileObject *)D_003BD098)->linkedState != NULL) {
        effQueuedFileObject = (s32)((EffQueuedFileObject *)D_003BD098)->linkedState;
    }
    return 0;
}

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

typedef struct EffectPlayState {
    u8 pad_00[0x0C];
    s32 cursor;       // 0x0C
    u8 pad_10[4];
    void *table;      // 0x14
    s32 tableCount;   // 0x18
} EffectPlayState;

extern EffectPlayState effBattleCameraSnapshotSelection;

extern u8 D_0038EC38[];

extern u8 D_0038EC80[];

extern u8 D_003BD954;

u32 effLoadBattleCameraSnapshot(u32 value) {
    void *record = fileQueueGetAt(effFileQueue, func_002B5990());

    *(EffectBlock90 *)D_003DF840 = *(EffectBlock90 *)record;
    *(EffectAlignedBlock128 *)D_003DF840 = *(EffectAlignedBlock128 *)D_003DF9A0;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.table = D_0038EC38;
        effBattleCameraSnapshotSelection.tableCount = 3;
        if (effBattleCameraSnapshotSelection.cursor > effBattleCameraSnapshotSelection.tableCount) {
            effBattleCameraSnapshotSelection.cursor = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.table = D_0038EC80;
        effBattleCameraSnapshotSelection.tableCount = 9;
        if (effBattleCameraSnapshotSelection.cursor == 0) {
            effBattleCameraSnapshotSelection.cursor = 1;
        }
    }
    return value;
}

u32 effStoreBattleCameraSnapshot(u32 value) {
    void *record = fileQueueGetAt(effFileQueue, func_002B5990());

    *(EffectBlock90 *)record = *(EffectBlock90 *)D_003DF840;
    *(EffectAlignedBlock128 *)D_003DF9A0 = *(EffectAlignedBlock128 *)D_003DF840;
    D_003BD954 = 1;
    if (((EffectBlock90 *)D_003DF840)->word[26] & 2) {
        effBattleCameraSnapshotSelection.table = D_0038EC38;
        effBattleCameraSnapshotSelection.tableCount = 3;
        if (effBattleCameraSnapshotSelection.cursor > effBattleCameraSnapshotSelection.tableCount) {
            effBattleCameraSnapshotSelection.cursor = 0;
        }
    } else {
        effBattleCameraSnapshotSelection.table = D_0038EC80;
        effBattleCameraSnapshotSelection.tableCount = 9;
        if (effBattleCameraSnapshotSelection.cursor == 0) {
            effBattleCameraSnapshotSelection.cursor = 1;
        }
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B7388);

s32 effCreateParticleTask(void) {
    return func_002B7388((s32)D_003B3968, D_0038DE70, 8);
}

s32 effCreatePolyTrackControlTask(void) {
    return func_002B7388((s32)D_003B3958, D_0038DF50, 6);
}

s32 effCreatePolyTrackTask(void) {
    return func_002B7388((s32)"POLY TRACK S", D_0038E000, 6);
}

s32 effCreateBlurTask(void) {
    return func_002B7388((s32)D_003BCA40, D_0038E0F0, 6);
}

s32 effCreateFilterTask(void) {
    return func_002B7388((s32)D_003BD050, D_0038E1A0, 3);
}

s32 effCreateEnvironmentTask(void) {
    return func_002B7388((s32)D_003B38C8, D_0038E280, 5);
}

s32 effCreatePolyTextureTask(void) {
    return func_002B7388((s32)D_003B3938, D_0038E240, 2);
}

s32 effCreatePolyFlashTask(void) {
    return func_002B7388((s32)D_003B3928, D_0038E310, 0xB);
}

s32 effCreatePolyRingTask(void) {
    return func_002B7388((s32)D_003B3918, D_0038E450, 6);
}

s32 effCreatePolyThunderTask(void) {
    return func_002B7388((s32)D_003B3908, D_0038E500, 4);
}

s32 effCreatePolyTwinkleTask(void) {
    return func_002B7388((s32)"POLY TWINKLE", D_0038E570, 6);
}

s32 effCreatePolyWindTask(void) {
    return func_002B7388((s32)D_003B38F8, D_0038E620, 7);
}

s32 effCreatePolyScalyTask(void) {
    return func_002B7388((s32)D_003B38E8, D_0038E6F0, 5);
}

s32 effCreatePolyCrackTask(void) {
    return func_002B7388((s32)D_003B38D8, D_0038E7C0, 2);
}

s32 effCreateBattleOnlyTask(void) {
    return func_002B7388((s32)D_003B3888, D_0038E800, 0xA);
}

s32 effCreateTwoDimensionalTask(void) {
    return func_002B7388((s32)D_003BD000, D_0038E9A0, 2);
}

/* Resource-bank request shared with the DDS2 effect loader. */
typedef struct EffResourceBankSlot {
    u8 pad_00[0xC8];
    char name[0x34];  /* 0xC8 */
    s32 type;         /* 0xFC */
    s32 state;        /* 0x100 */
    s32 count;        /* 0x104 */
} EffResourceBankSlot;

extern void effPollResourceBankSlot(char *, u32, void *);

extern u8 *fileAppendJobFromEntry(s32, void *);

u32 effPollPartResource(void) {
    u8 record[0x110];
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(D_003B39C8, 0x20, record);
    state = ((EffResourceBankSlot *)record)->state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            u8 *job = fileAppendJobFromEntry(effFileQueue, record);
            u8 *asset = effFindAssetData(job);
            strcpy(((FileJob *)job)->name, *(char **)asset);
        }
        result = 0x400002;
    }
    return result;
}


typedef struct EffectBlock128 {
    u32 word[32];
} EffectBlock128;

extern EffectBlock128 D_003DF920;

extern s32 func_002959E8(void *);

u32 effPollNamedFileJob(void) {
    u8 record[0x110];
    u32 state;
    u32 result = 0x400001;

    effPollResourceBankSlot(D_003B39E0, 0x10, record);
    state = ((EffResourceBankSlot *)record)->state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            fileQueueDestroy(effFileQueue);
        }
        strcpy((char *)D_003DF8D0, ((EffResourceBankSlot *)record)->name);
        effFileQueue = func_002959E8(record);
        D_003DF920 = *(EffectBlock128 *)effFileQueue;
        D_003BD058 = 0;
        D_0038F2FC[0] = 0;
        result = 0x400002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3A90);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3AA8);

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B7B78);

extern u32 D_003DFA20[];

s32 effResetStaticState(void) {
    D_003DFA20[0] = 0;
    D_003DFA20[1] = 0;
    D_003DFA20[2] = 0;
    D_003DFA20[3] = 0;
    D_003DF840[0x88] = 8;
    D_003DF840[0x89] = 0;
    D_003DF840[0x8A] = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B7E60);

void effCopyCameraSnapshotSelection(void) {
    D_003BD108 = D_003DF840[0x88];
    D_003BD109 = D_003DF840[0x89];
    D_003BD10A = D_003DF840[0x8A];
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B8108);

s32 effResetCameraSnapshotSelection(void) {
    D_003DF840[0x88] = 8;
    D_003DF840[0x89] = 0;
    D_003DF840[0x8a] = 0;
    VU0_STORE_VF($vf0, D_003DFA30);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B8648);

u32 effLoadFileSlotAndPoll(void) {
    u8 *file = fileQueueGetAt(effFileQueue, func_002B5990());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = func_002B7E60(D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

typedef struct EffectStateSnapshot {
    s128 vectors[8];
} EffectStateSnapshot;

extern s32 func_002B7E60(EffectStateSnapshot *, s32);

s32 effRunWithStateBackup(void) {
    EffectStateSnapshot snapshot = *(EffectStateSnapshot *)&D_003DF920;
    s128 *backup = &snapshot.vectors[4];
    s32 result;

    PCP_COPY_VECTOR(backup, &D_003DF920);
    result = func_002B7E60(&snapshot, 0);
    PCP_COPY_VECTOR(&D_003DF920, backup);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

extern u32 func_002B8648(u8 *, s32);

u32 effProcessQueuedFileRecordAndPersistChanges(void) {
    u8 *file = fileQueueGetAt(effFileQueue, func_002B5990());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = func_002B8648(D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

extern u8 D_003BD954;

void effResetFileResources(void) {
    D_0038F2FC[0] = 0;
    D_003BD954 = 0;
    D_003BD058 = 0;
    if (effFileQueue != 0) {
        fileQueueDestroy(effFileQueue);
        effFileQueue = 0;
    }
    if (effAuxiliaryFileQueue != 0) {
        fileQueueDestroy(effAuxiliaryFileQueue);
        effAuxiliaryFileQueue = 0;
    }
    if (effTemporaryFileJob != 0) {
        fileJobDestroy(effTemporaryFileJob);
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

typedef struct EffBankSelection {
    u8 pad_00[0xC8];
    char name[0x34]; // 0xC8
    s32 label;       // 0xFC
    s32 state;       // 0x100: 1 once the selection is finished
    s32 variant;     // 0x104
} EffBankSelection;

void effPollResourceBankSlot(char *path, u32 slot, void *record) {
    EffBankSelection *selection = record;

    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory((s32)path, slot);
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceNameHeaderPair(effResourceBankDescriptor, 0xBA, 0x1C);
        return;
    }
    func_001FBA38(effResourceBankDescriptor);
    selection->state = func_001FBF48(effResourceBankDescriptor);
    selection->label = btlFormatSelectedResourceName(effResourceBankDescriptor, selection);
    selection->variant = btlGetResourcePathVariant(effResourceBankDescriptor);
    btlTrimResourceName(effResourceBankDescriptor, selection->name);
    if (selection->state == 1) {
        btlDestroyResourceDescriptor(effResourceBankDescriptor);
        effResourceBankDescriptor = 0;
        btlDestroyEntryList(effResourceBankEntries);
        effResourceBankEntries = 0;
    }
}

extern u32 effFileQueueNameRecord;

extern u8 D_003BCF30[];

extern void func_001FC7D0(u32, s32);

void effInitializeResourceQueue(void) {
    void *record;

    if (effFileQueueNameRecord != 0) {
        func_001FC2E8(effFileQueueNameRecord);
    }
    effFileQueueNameRecord = btlCreateResourceNameRecord(D_003BCF30);
    btlSetResourceNameHeaderPairAlternate(effFileQueueNameRecord, 0xC2, 0xC8);
    func_001FC7D0(effFileQueueNameRecord, 9);
    record = fileQueueGetAt(effFileQueue, func_002B5990());
    btlResourceRecordSetName(effFileQueueNameRecord, ((FileJob *)record)->name);
}

extern void func_001FC300(u32);

extern void btlFormatResourceNameWithoutPrefix(u32, void *);

extern u32 func_001FC730(u32);

u32 effPollResourceQueue(void) {
    u32 state;

    func_001FC300(effFileQueueNameRecord);
    btlFormatResourceNameWithoutPrefix(effFileQueueNameRecord, ((FileJob *)fileQueueGetAt(effFileQueue, func_002B5990()))->name);
    state = func_001FC730(effFileQueueNameRecord);
    if ((u32)(state - 1) < 2) {
        func_001FC2E8(effFileQueueNameRecord);
        effFileQueueNameRecord = 0;
        return 0;
    }
    return 0x200001;
}

void effReleaseQueuedResourceName(void) {
    if (effQueuedResourceNameRecord != 0) {
        func_001FC2E8(effQueuedResourceNameRecord);
        effQueuedResourceNameRecord = 0;
    }
}

void effQueueResource(void *unused, void *effect) {
    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord();
        btlSetResourceNameHeaderPairAlternate(effQueuedResourceNameRecord, 0xC2, 0xC8);
    }
    btlResourceRecordSetName(effQueuedResourceNameRecord, effect);
}

extern void btlFormatResourceNameWithPrefix(u32, void *);

extern u32 func_001FC7D8(u32, u32 *);

void effUpdateResourceQueue(u32 *result, void *queueData, EffQueueRecord *record) {
    u32 state;

    if (effQueuedResourceNameRecord == 0) {
        effQueuedResourceNameRecord = btlCreateResourceNameRecord(queueData);
        btlSetResourceNameHeaderPairAlternate(effQueuedResourceNameRecord, 0xC2, 0xC8);
        return;
    }
    func_001FC300(effQueuedResourceNameRecord);
    btlFormatResourceNameWithPrefix(effQueuedResourceNameRecord, record->nameWithPrefix);
    btlFormatResourceNameWithoutPrefix(effQueuedResourceNameRecord, record->name);
    state = func_001FC730(effQueuedResourceNameRecord);
    record->completion.state = state;
    if (state == 1) {
        if (func_001FC7D8(effQueuedResourceNameRecord, result) != 0) {
            func_001FC2E8(effQueuedResourceNameRecord);
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

typedef struct EffectPollRecord {
    u8 pad_00[0xC8];
    s32 type;   // 0xC8
    s32 state;  // 0xCC
    s32 value;  // 0xD0
} EffectPollRecord;

void effPollResourceBank(s32 flags, void *out) {
    EffectPollRecord *record = out;

    if (effResourceBankEntries == 0) {
        effResourceBankEntries = btlScanDirectory(0, flags);
        if (flags & 8) {
            u32 i = 0;
            u32 count = func_00151FC0();
            if (count != 0) {
                do {
                    char name[0x70];
                    func_003014F0(name, "GENERAL %d", i);
                    btlAppendEntry(effResourceBankEntries, name, 8, i, 0);
                    i++;
                } while (i < count);
            }
        }
        effResourceBankDescriptor = btlCreateResourceDescriptor(effResourceBankEntries);
        btlSetResourceNameHeaderPair(effResourceBankDescriptor, 0xBA, 0x1C);
    } else {
        func_001FBA38(effResourceBankDescriptor);
        record->state = func_001FBF48(effResourceBankDescriptor);
        record->type = btlFormatSelectedResourceName(effResourceBankDescriptor, record);
        record->value = btlGetResourcePathVariant(effResourceBankDescriptor);
        if (record->state == 1) {
            btlDestroyResourceDescriptor(effResourceBankDescriptor);
            effResourceBankDescriptor = 0;
            btlDestroyEntryList(effResourceBankEntries);
            effResourceBankEntries = 0;
        }
    }
}

typedef struct EffectMapping {
    u8 pad_00[0x0C];
    u32 field0C;
    u32 field10;
    u8 *table;
    u32 count;
} EffectMapping;

extern EffectMapping effMappingState;

void effResetMappingFlags(void) {
    D_003BD11C = 0;
    D_003BD128 = 0;
    D_003BD120 = 0;
    D_003BD124 = 1;
    effMappingState.field0C = 1;
    effMappingState.field10 |= 0x10;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002B9320);

extern u8 D_0038F7D8[];

extern u8 D_0038F718[];

extern u8 D_0038F658[];

/* The effect mapping dispatcher stores its data pointer at +0x0C. */
typedef struct EffMappingRequest {
    u8 pad_00[0x0C];
    s32 object;          // 0x0C
} EffMappingRequest;

/* Parameters passed to the mapping dispatcher from distinct object channels. */
typedef struct EffMappingObject {
    u8 pad_00[0x34];
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
} EffMappingObject;

s32 effMapObjectWithTemporaryMappingTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F7D8;
    effMappingState.count = 8;
    /* Keep this parameter load raw: a typed member changes the table-write scheduling. */
    result = func_002B9320(object + 0x2c, object + 0x50, *(s32 *)(object + 0xb8));
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

s32 func_002BA0C0(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F7D8;
    effMappingState.count = 8;
    result = func_002B9320(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

void func_002BA128(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->valueB8.bits);
}

s32 func_002BA148(void) {
    return func_002B9320((s32)D_003DE148, (s32)D_003DE148 + 0x24, ((EffMappingObject *)D_003DE148)->value34.signedValue);
}

s32 func_002BA170(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F7D8;
    effMappingState.count = 8;
    result = func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

void func_002BA1D0(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_002BA1F0(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value38);
}

void func_002BA210(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

s32 func_002BA230(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F718;
    effMappingState.count = 8;
    /* Keep this parameter load raw: a typed member changes the table-write scheduling. */
    result = func_002B9320(object, object + 0x24, *(s32 *)(object + 0x34));
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

void func_002BA290(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_002BA2B0(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

s32 effResolveMappingWithTemporaryTable(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F7D8;
    effMappingState.count = 8;
    result = func_002B9320(object + 0x4c, object + 0x70, ((EffMappingObject *)object)->valueD8);
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

void func_002BA338(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_002BA358(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

void func_002BA380(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_002BA3A0(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object + 0x3c, object + 0x60, ((EffMappingObject *)object)->value80);
}

s32 func_002BA3C8(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F7D8;
    effMappingState.count = 8;
    result = func_002B9320(object + 0x68, object + 0x8c, ((EffMappingObject *)object)->valueF4);
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

void func_002BA430(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value70);
}

void func_002BA450(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value34.bits);
}

void func_002BA470(s32 request) {
    s32 object;

    object = ((EffMappingRequest *)request)->object;
    func_002B9320(object + 0x50, object + 0x74, ((EffMappingObject *)object)->value84);
}

s32 func_002BA498(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    effMappingState.table = D_0038F718;
    effMappingState.count = 8;
    result = func_002B9320(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    effMappingState.table = D_0038F658;
    effMappingState.count = 8;
    return result;
}

typedef struct EffMappingState {
    u8 pad00[0xC];
    u32 unk0C;
    u32 flags;
} EffMappingState;

void btlResetEffectWork(void) {
    u8 *first = D_0038F9D0;
    u8 *second = D_0038FAD0;

    ((EffMappingState *)first)->flags |= 0x10;
    ((EffMappingState *)first)->unk0C = 0;
    ((EffMappingState *)second)->flags |= 0x10;
    ((EffMappingState *)second)->unk0C = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002BA538);

s32 func_002BAE18(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, ((EffMappingObject *)p->x0C)->valueB8.signedValue, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAE48(BaObj *p) {
    return func_002BA538(p->x0C + 0x8C, ((EffMappingObject *)p->x0C)->valueB8.signedValue, D_0038FAD0, D_0038FA20);
}

s32 func_002BAE78(BaObj *p) {
    return func_002BA538(p->x0C + 0x7C, ((EffMappingObject *)p->x0C)->valueD4, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAEA8(BaObj *p) {
    return func_002BA538(p->x0C + 0xA8, ((EffMappingObject *)p->x0C)->valueD4, D_0038FAD0, D_0038FA20);
}

s32 func_002BAED8(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, ((EffMappingObject *)p->x0C)->valueB8.signedValue, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF08(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, ((EffMappingObject *)p->x0C)->valueB8.signedValue, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF38(BaObj *p) {
    return func_002BA538(p->x0C + 0x8C, ((EffMappingObject *)p->x0C)->valueB8.signedValue, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF68(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF98(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAFC8(BaObj *p) {
    return func_002BA538(p->x0C + 0x80, ((EffMappingObject *)p->x0C)->valueD8, D_0038F9D0, D_0038F8E8);
}

s32 func_002BAFF8(BaObj *p) {
    return func_002BA538(p->x0C + 0xAC, ((EffMappingObject *)p->x0C)->valueD8, D_0038FAD0, D_0038FA20);
}

s32 func_002BB028(BaObj *p) {
    return func_002BA538(p->x0C + 0x90, ((EffMappingObject *)p->x0C)->value80, D_0038F9D0, D_0038F938);
}

s32 func_002BB058(BaObj *p) {
    return func_002BA538(p->x0C + 0x9C, ((EffMappingObject *)p->x0C)->valueF4, D_0038F9D0, D_0038F8E8);
}

s32 func_002BB088(BaObj *p) {
    return func_002BA538(p->x0C + 0xC8, ((EffMappingObject *)p->x0C)->valueF4, D_0038FAD0, D_0038FA20);
}

s32 func_002BB0B8(BaObj *p) {
    return func_002BA538(p->x0C, ((EffMappingObject *)p->x0C)->valueF4, D_0038F9D0, D_0038F938);
}

s32 func_002BB0E8(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, ((EffMappingObject *)p->x0C)->value70, D_0038F9D0, D_0038F938);
}

s32 func_002BB118(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

s32 func_002BB148(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, ((EffMappingObject *)p->x0C)->value8C, D_0038F9D0, D_0038F8E8);
}

void func_002BB178(void) {
    D_003BD124 = 1;
}

INCLUDE_ASM(const s32, "game/code_0029C530", func_002BB188);

void func_002BB6B0(s32 work) {
    func_002BB188((s32)((BaObj *)work)->x0C + 0x3c, ((EffMappingObject *)((BaObj *)work)->x0C)->value4C);
}

void func_002BB6D0(s32 work) {
    func_002BB188((s32)((BaObj *)work)->x0C + 0x60, ((EffMappingObject *)((BaObj *)work)->x0C)->value3C + 1);
}

void func_002BB6F8(s32 work) {
    func_002BB188((s32)((BaObj *)work)->x0C + 0x70, ((EffMappingObject *)((BaObj *)work)->x0C)->value8C + 1);
}

void func_002BB720(s32 work) {
    func_002BB188((s32)((BaObj *)work)->x0C + 0x70, ((EffMappingObject *)((BaObj *)work)->x0C)->value8C + 1);
}

void func_002BB748(s32 work) {
    func_002BB188((s32)((BaObj *)work)->x0C + 0x60, ((EffMappingObject *)((BaObj *)work)->x0C)->value74 + 1);
}

extern void fileQueueDetachSectorFollower(s32, s32);

extern void fileQueueLinkJobToSectorLeader(s32, s32, void *);

/* Poll record from the resource-bank queue; count is negated to find its entry. */
typedef struct EffBankStatus {
    u8 pad_00[0xC8];
    s32 type;         // 0xC8
    s32 state;        // 0xCC
    s32 count;        // 0xD0
} EffBankStatus;

extern void effPollResourceBank(s32, void *);

s32 effPollFileQueueRecord(s32 request) {
    u8 record[0xE0];
    s32 kind;
    s32 result = 0x600001;

    effPollResourceBank(request, record);
    kind = ((EffBankStatus *)record)->state;
    if (kind == 2) {
        result = 0x400000;
    } else if (kind == 1) {
        if (((EffBankStatus *)record)->type != 8) {
            void *entry = fileQueueGetAt(effFileQueue, -((EffBankStatus *)record)->count);
            if (effCurrentFileQueueEntry != (s32)entry) {
                fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
                fileQueueLinkJobToSectorLeader(effFileQueue, effCurrentFileQueueEntry, entry);
            }
        } else {
            fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
            fileJobSetSecondaryData(effQueuedFileHandle, record + 0xD0, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

void func_002BB838(void) {
    effPollFileQueueRecord(0x4b);
}

void func_002BB850(void) {
    effPollFileQueueRecord(0x4b);
}

void func_002BB868(void) {
    effPollFileQueueRecord(0xb);
}

extern s32 effClassifyResourceMask(s32);

typedef struct EffFileQueryInfo {
    u8 pad0[0xFC];
    u32 resourceMask;
    s32 status;
    u8 pad104[0xC];
} EffFileQueryInfo;

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    u32 allocationHandle;
} EffFileResourceRecord;


s32 effPollFileRecord(char *path, s32 slot) {
    u8 record[0x110];
    s32 kind;
    s32 result = 0x600001;

    effPollResourceBankSlot(path, slot, record);
    kind = ((EffFileQueryInfo *)record)->status;
    if (kind == 2) {
        result = 0x400000;
    } else if (kind == 1) {
        s32 type;
        fileQueueDetachSectorFollower(effFileQueue, effCurrentFileQueueEntry);
        type = ((EffFileQueryInfo *)record)->resourceMask;
        if (type != 8) {
            fileJobCopyCommandIntoSecondaryData(effQueuedFileHandle, (u32)record, effClassifyResourceMask(type));
        } else {
            fileJobSetSecondaryData(effQueuedFileHandle, record + 0x104, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

s32 func_002BB930(void) {
    return effPollFileRecord(D_003B3B88, 0x43);
}

s32 func_002BB950(void) {
    return effPollFileRecord(D_003B3B88, 0x43);
}

s32 func_002BB970(void) {
    return effPollFileRecord(D_003B39C8, 0x20);
}

s32 func_002BB990(void) {
    return effPollFileRecord(D_003B39E0, 0x10);
}

s32 func_002BB9B0(void) {
    return effPollFileRecord(D_003B39C8, 0x20);
}

s32 func_002BB9D0(void) {
    return effPollFileRecord(D_003B3B88, 1);
}

s32 func_002BB9F0(void) {
    return effPollFileRecord(D_003B3B88, 1);
}

s32 func_002BBA10(void) {
    return effPollFileRecord(D_003B3B88, 1);
}

u32 effFileJobSecondaryDataSet(void) {
    u32 zero = 0;
    fileJobSetSecondaryData(effQueuedFileHandle, &zero, 4, 4);
    return 0x400002;
}

s32 func_002BBA68(void) {
    return effPollFileRecord(D_003B3BA0, 4);
}

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;


extern void effResetFileResourceManager(void);

extern EffectFileHeader D_0038DC08;

extern EffectFileHeader D_00383F18;

extern EffectFileHeader D_0038C758;

u32 fileLoadEffectSlotA(void) {
    u8 fileInfo[0x110];
    u8 *job;
    u8 *entry;
    u8 *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, 4, fileInfo);
    status = ((EffFileQueryInfo *)fileInfo)->status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(3);
        fileJobSetPrimaryData(job, D_00383F18.start, D_00383F18.length,
                      D_00383F18.mode);
        fileJobCopyCommandIntoSecondaryData(job, fileInfo, effClassifyResourceMask(((EffFileQueryInfo *)fileInfo)->resourceMask));
        entry = (u8 *)fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        effQueuedFileHandle = ((FileJob *)entry)->id;
        resource = (u8 *)effFindAssetData(entry);
        strcpy(((FileJob *)entry)->name, ((EffFileResourceRecord *)resource)->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(((EffFileResourceRecord *)resource)->buffer, fileData,
               ((EffFileResourceRecord *)resource)->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_0038F2F0;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern u8 D_00384A08[] __attribute__((aligned(4)));

u32 fileLoadEffectSlotHelp(void) {
    EffFileQueryInfo fileInfo;
    u32 result;
    s32 status;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    u8 *buffer;
    u32 command;
    u32 totalLength;
    u32 dataLength;
    u32 allocation;

    effPollResourceBankSlot(D_003B3BA0, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        u32 headerBytes = 0x40;
        u32 oldAllocation;
        u32 queuedFile;
        job = (u8 *)fileCreateJob(6);
        command = sdfDevCreateCommandState(&fileInfo);
        dataLength = sdfDevQueueControlAndWait(command);
        totalLength = dataLength + headerBytes;
        allocation = (u32)sdfAllocGeneralBlock(totalLength);
        buffer = (u8 *)sdfResourceRetainAddress(allocation);
        memset(buffer, 0, headerBytes);
        sdfDevQueueReadAndWait(command, buffer + headerBytes, dataLength);
        sdfDevWaitThenReleaseCommandState(command);
        fileJobSetPrimaryData(job, buffer, totalLength, 0);
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (u32)entry;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        memcpy(D_003DF9A0, entry, 0x80);
        queuedFile = entry->id;
        oldAllocation = resource->allocationHandle;
        effQueuedFileHandle = queuedFile;
        if (oldAllocation != 0) {
            sdfReleaseResourceAllocation(oldAllocation);
        }
        resource->allocationHandle = allocation;
        resource->buffer = buffer;
        resource->size = dataLength + headerBytes;
        resource->mode = 0;
        memcpy(D_003DE148, D_00384A08, 0x3C);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_0038F2F0;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 fileLoadEffectSlotB(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(18);
        fileJobSetPrimaryData(job, D_0038C758.start, D_0038C758.length,
                      D_0038C758.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_0038F2F0;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_0038D470;

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B18);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B28);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B38);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B48);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B58);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B68);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B78);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3B88);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3BA0);

u32 effLoadFileSlotF2(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot("/tool/effect/f2/", 0x80, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x14);
        fileJobSetPrimaryData(job, D_0038D470.start, D_0038D470.length,
                      D_0038D470.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_0038F2F0;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 effLoadMaterialFile(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_003B3B88, 2, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x16);
        fileJobSetPrimaryData(job, D_0038DC08.start, D_0038DC08.length,
                      D_0038DC08.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_003BD064 = effCreateBattleCameraJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_0038F2F0;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

void effResetFileResourceManager(void) {
    D_003BD11C = 0;
    D_003BD124 = 1;
    D_003BD120 = 0;
    D_003BD128 = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

s32 effClassifyResourceMask(s32 flags) {
    switch (flags) {
        case 1:   return 1;
        case 2:   return 2;
        case 4:   return 3;
        case 8:   return 4;
        case 16:  return 6;
        case 32:  return 5;
        case 64:  return 7;
        case 128: return 8;
        default:  return 0;
    }
}

extern void *sdfAllocSizeClassBlock(u32);
extern void sdfReleaseChipBlock(void *);

EffectList *mnuAllocateValueRecord(u32 mode) {
    EffectList *list = sdfAllocSizeClassBlock(sizeof(EffectList));
    memset(list, 0, sizeof(EffectList));
    list->mode = mode;
    list->count = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

void func_002BC618(EffectList *list) {
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


s32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference) {
    EffectListNode *node = sdfAllocSizeClassBlock(sizeof(EffectListNode));
    memset(node, 0, sizeof(EffectListNode));
    node->next = NULL;
    node->kind = kind;
    node->value = value;
    node->length = length;
    node->reference = (void **)reference;
    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    return ++list->count;
}


s32 effRemoveListEntry(EffectList *list) {
    EffectListNode *node = list->first;
    EffectListNode *next = node->next;
    sdfReleaseChipBlock(node);
    list->first = next;
    if (--list->count == 0) {
        list->last = NULL;
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
                    func_00288788(list->request);
                }
                list->request = fileQueuePlainDispatchRequest(node->length);
                if (list->mode == 2) {
                    func_00288C50(list->request);
                }
                node->state = 1;
            } else if (fileRequestIsReady(list->request) != 0) {
                for (item = list->request->items; item != NULL; item = item->next) {
                    if (item->kind == 1) {
                        node = list->first;
                        buffer = item->buffer;
                        *node->reference = func_002BD9C0(buffer, node->kind);
                        if (node->kind == 0) {
                            sdfReleaseResourceAllocation(buffer);
                        }
                        effRemoveListEntry(list);
                    }
                }
            }
            if (list->count == 0) {
                func_00288788(list->request);
                list->request = NULL;
            }
            break;
        }
    }
    return list->count;
}

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3BD0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3BE0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3BF0);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3C00);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3C10);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3C20);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3C30);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3C40);

INCLUDE_RODATA(const s32, "game/code_0029C530", D_003B3C50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC978);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC97C);

INCLUDE_SDATA(const s32, "game/code_0029C530", effCurrentRenderPacket);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC984);

INCLUDE_SDATA(const s32, "game/code_0029C530", effWindTextureHandle);

INCLUDE_SDATA(const s32, "game/code_0029C530", effSharedRibbonReferenceCount);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC990);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC994);

INCLUDE_SDATA(const s32, "game/code_0029C530", effScalyTextureHandle);

INCLUDE_SDATA(const s32, "game/code_0029C530", effSharedStripReferenceCount);

INCLUDE_SDATA(const s32, "game/code_0029C530", effSharedScalyStripResource);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9A8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9AC);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9B0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9B8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9C0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9C8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9D0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9D8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9E0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9E8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9F0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BC9F8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA00);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA08);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA10);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA18);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA20);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA28);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA30);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA38);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA40);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA48);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA58);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA60);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA68);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA70);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA78);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA80);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA88);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA90);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCA98);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAA0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAA8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAB0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAB8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAC0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAC8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAD0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAD8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAE0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAE8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAF0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCAF8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB00);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB08);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB10);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB18);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB20);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB28);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB30);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB38);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB40);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB48);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB58);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB60);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB68);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB70);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB78);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB80);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB88);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB90);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCB98);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBA0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBA8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBB0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBB8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBC0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBC8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBD0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBD8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBE0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBE8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBF0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCBF8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC00);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC08);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC10);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC18);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC20);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC28);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC30);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC38);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC40);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC48);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC58);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC60);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC68);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC70);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC78);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC80);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC88);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC90);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCC98);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCA0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCA8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCB0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCB8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCC0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCC8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCD0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCD8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCE0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCE8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCF0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCCF8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD00);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD08);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD10);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD18);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD20);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD28);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD30);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD38);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD40);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD48);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD58);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD60);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD68);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD70);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD78);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD80);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD88);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD90);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCD98);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDA0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDA8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDB0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDB8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDC0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDC8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDD0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDD8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDE0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDE8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDF0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCDF8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE00);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE08);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE10);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE18);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE20);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE28);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE30);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE38);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE40);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE48);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE58);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE60);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE68);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE70);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE78);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE80);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE88);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE90);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCE98);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEA0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEA8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEB0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEB8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEC0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEC8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCED0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCED8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEE0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEE8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEF0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCEF8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF00);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF08);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF10);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF18);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF20);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF28);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF30);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF38);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF40);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF48);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF50);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF58);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF60);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF68);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF70);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF78);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF80);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF88);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF90);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCF98);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFA0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFA8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFB0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFB8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFC0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFC8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFD0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFD8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFE0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFE8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFF0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BCFF8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD000);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD008);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD010);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD018);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD020);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD028);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD030);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD038);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD040);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD048);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD050);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD058);

INCLUDE_SDATA(const s32, "game/code_0029C530", effAuxiliaryFileQueue);

INCLUDE_SDATA(const s32, "game/code_0029C530", effFileQueue);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD064);

INCLUDE_SDATA(const s32, "game/code_0029C530", effQueuedFileHandle);

INCLUDE_SDATA(const s32, "game/code_0029C530", effTemporaryFileJob);

INCLUDE_SDATA(const s32, "game/code_0029C530", effCurrentFileQueueEntry);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD074);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD078);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD080);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD088);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD090);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD098);

INCLUDE_SDATA(const s32, "game/code_0029C530", effQueuedFileObject);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0A0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0A8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0B0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0B8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0C0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0C8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0D0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0D8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0E0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0E8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0F0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD0F8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD100);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD108);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD109);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD10A);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD10B);

INCLUDE_SDATA(const s32, "game/code_0029C530", effResourceBankEntries);

INCLUDE_SDATA(const s32, "game/code_0029C530", effResourceBankDescriptor);

INCLUDE_SDATA(const s32, "game/code_0029C530", effFileQueueNameRecord);

INCLUDE_SDATA(const s32, "game/code_0029C530", effQueuedResourceNameRecord);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD11C);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD120);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD124);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD128);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD12C);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD130);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD138);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD140);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD148);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD150);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD158);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD15C);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD160);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD164);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD168);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD16C);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD170);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD178);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD180);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD188);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD190);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD198);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1A0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1A8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1B0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1B8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1C0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1C8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1D0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1D8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1E0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1E8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1F0);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD1F8);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD200);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD208);

INCLUDE_SDATA(const s32, "game/code_0029C530", D_003BD210);

