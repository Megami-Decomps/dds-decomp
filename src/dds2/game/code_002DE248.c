#include "common.h"
#include "file.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff_queue.h"
#include "btl_state.h"
#include "evt_unit.h"
#include "mdl.h"
#include "eff.h"

extern void mdlAddEntryPlain(s32, u32, u32);

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

/* Kernel surface entries have a 0x20-byte stride and submit at +0x10. */
typedef struct EffDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct EffDrawSurface *, void *);
    u8 pad14[0xC];
} EffDrawSurface;

static inline void effSubmitSurfacePacket(EffDrawSurface *surface, void *list) {
    surface->submit(surface, list);
}

extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfConsAppendVuPacket();
extern void sdfConsAppendAssetPacket();
extern void *func_00167A10(EffPacketParams *);
extern u32 D_003E9D80[];
extern EffDrawSurface *D_003E9DC0[];
extern EffDrawSurface *D_003E9F38[];
extern u32 D_003E9BE0[];
extern EffDrawSurface *D_003E9C28[];
extern EffDrawSurface kwlnDrawSurfaces[];

typedef struct EffectStateSnapshot {
    s128 vectors[8];
} EffectStateSnapshot;

extern void fileJobCopyCommandIntoSecondaryData(u32, u32, u32);

extern void effResetFileResourceManager(void);

extern void fileJobSetPrimaryData(u32, void *, u32, u16);

extern s32 effClassifyResourceMask(s32);

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

typedef struct EffectVectorRequest {
    u8 kind;
    u8 count;
    u8 size;
    u8 pad_03;
    u32 unk04;
} EffectVectorRequest;


typedef struct EffViewScale {
    u8 pad00[0x74];
    f32 scale;
} EffViewScale;

typedef struct EffModelAssetData {
    u8 pad00[0x14];
    u32 unk14;
    u32 unk18;
} EffModelAssetData;

typedef struct EffModelAssetHeader {
    u8 pad00[0xC];
    EffModelAssetData *data; // pointer to the model data block
} EffModelAssetHeader;


typedef struct EffSurfaceEndpoints {
    u8 pad00[0x140];
    EffectVectorRequest start;
    EffectVectorRequest end;
    s16 startIndex;
    s16 endIndex;
} EffSurfaceEndpoints;

typedef struct EffModelParameter {
    u8 pad00[0x20];
    f32 value;
} EffModelParameter;

typedef struct EffModelContextView {
    u8 pad00[0x18];
    s32 lookAtBasis;
    EffModelParameter *parameters;
} EffModelContextView;

typedef struct EffModelBindings {
    u32 material;
    void *model;
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

typedef struct EffectRecord {
    void *owner;
    s32 slot;
    struct EffectRecord *prev;
    struct EffectRecord *next;
} EffectRecord;

typedef struct EffectOwnerRecord {
    void *owner;
    EffectRecord *entries[16];
} EffectOwnerRecord;

typedef struct EffectSlot {
    u8 pad_0x00[0x28]; // 0x00
    s32 bucket;        // 0x28
    u8 pad_0x2C[0x54]; // 0x2C
} EffectSlot; // 0x80

typedef struct EffectSlotOwner {
    u8 pad_0x00[0x10];  // 0x00
    EffectSlot *slots;  // 0x10
} EffectSlotOwner;

extern void effBattleMiscQueryPosition(u32, void *, void *);

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
    u8 transform[0x20];
    f32 scale;
    u32 color;
    s32 updateCount;
    s32 kind;
    void *model;
    u32 attributes;
    u32 childResource;
    void *source;
} EffModelResource;

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    u32 assetId;
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

typedef struct EffRecordBucket {
    u8 pad_00[4];
    s32 (*step)(BdWork *, BdWork *, EffTimedState *); /* 0x04 */
    u32 count;
    u8 *records;
} EffRecordBucket;

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

extern u32 func_002D7458(u8 *, u8 *, u32, u32);

extern void effMiscQuaternionToMatrixVU(void);

extern void func_002DB288(s32, void *);

extern void sdfVuMatrixToQuaternion(f32 matrix[4][4]);

extern u8 D_003E9140[];

extern u8 D_003E9110[];

extern void func_002DB2C0(s32, void *);

extern s32 billGetFirstEntryFramePeriod(u32);

extern void billSetEntryFrameMode1(u32, s32);

extern f32 effComputeProjectedOffsetAngle(void *, void *);

extern f32 func_0015A150(void *, void *);

extern void billSetChildScaleComponents(u32, f32, f32);

extern void billSetLengthExtent(u32, f32);

extern void effCopyVector(u32, void *);

extern void billInvokeCallback(u32);

extern f32 D_0042BC10[4];

extern u8 D_003E9100[];

extern void func_002E5E88(u8 *, void *);

extern void effDrawFourPointGroups(u8 *, void *);

extern void func_002F1888(u8 *, void *);

extern void sdfMotionSampleAtFrame(s32, f32);

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

extern u32 D_0045C2F0[];

extern u8 D_004386E0[];

extern u8 D_004386E8[];

extern u32 effCreateMappedResource(u32);

extern u32 sdfResourceRetainAddress();

extern u64 fileGetResourceHandle(void);

extern u32 func_00305148();

extern void *sdfModelCreateWithAlternateItems(u32, u32);

extern void *sdfAllocSizeClassBlock(s32);

extern s32 effSharedRibbonReferenceCount;

extern u32 D_00437E78;

extern u32 effFlashTextureHandles;

extern u32 effModelUpdateControlFlags;

extern void *fileResolvePrimaryBuffer();

extern s32 fileResolveSecondaryBuffer(void *);

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock();

extern void mdlLoadPrimaryVectorVU(void *);

extern void func_0033A7E8(u32, void *, void *);

extern u128 D_004584B0[];

extern u128 D_00458470[];

extern u8 D_003E9F50[];


extern void sdfTexReleaseReference();

extern void effReleaseSharedReference();

extern s64 btlIsRuntimeAllocated(void);

extern s32 func_002DC1D0(u32, u32);

extern void dds3DispatchIndexedCallback(s32, f32);

extern void billSetBillboardMode(s32, s16);

extern void billMarkKindOneFlag(s32);

extern u8 D_00380828[];

extern void mdlProcessContextNodesAndTransforms(void *, const void *);

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

extern u32 billCloneObjectRetainingSharedData(u32);

extern u32 func_002DDF48(u32);

extern u32 effWindTextureHandle;

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern u32 effSharedScalyStripResource;

extern u32 effCloneSharedReferenceWithValue(u32, u32);

extern s32 *effAllocateCopiedEffectPayload(u32, u32, s32);

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

extern void mdlStoreTertiaryVectorVU(void *);

extern void *func_002DDAA8(void *);

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 refCount;      // 0x14 incremented with the global reference count
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

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

extern u32 sdfReadNamedResource(const char *, u32 *, s32);


extern u8 *effAllocateTexturedStripWork();

extern u32 D_00437E7C;


extern u32 D_004386B0;

extern u32 D_003FFA84[];

extern u8 D_0045C110[];

extern u8 D_00438758;

extern u8 D_00438759;

extern u8 D_0043875A;

extern u32 effQueuedFileHandle;

extern EffResourceOps D_003E9950[];


extern EffClassOps D_003E9B80[];


extern EffResourceOps D_003E9DD8[];


extern EffResourceOps D_003E9E60[];

extern EffResourceOps D_003EA018[];


extern EffClassOps effModelResourceOperations[];

extern u8 D_0045C1A0[];

extern u8 D_003FFA40[];

extern u8 D_0045C1E0[];

extern u8 *D_004386CC;

extern u8 D_0045C300[];

extern u8 D_00400150[];

extern u8 D_00400250[];

extern EffClassOps D_003E9D00[];

extern void *func_00232198(s32 group, s32 id);

extern s32 mdlGetContextResourceGroup(void *model);

extern s32 mdlGetContextResourceId(void *model);



/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */
extern void *sdfAllocGeneralBlock(u32);

extern u8 *effCreatePointSet4(u32);

/* Grid dimensions used by surface and strip instance allocators. */
typedef struct EffGrid {
    u8 pad00[0x20];
    s32 width;
    s32 height;
    u8 pad28[0x90];
    s32 altHeight;
} EffGrid;

typedef struct EffGridBillMode {
    u8 pad00[0x54];
    s16 mode;
} EffGridBillMode;

/* Record prefix shared by the file/grid allocation and billboard paths. */
typedef struct EffGridRecord {
    u16 kind;
    u8 pad02[6];
    u32 count;
    u8 pad0C[0x14];
    EffGridBillMode *bill;
    EffGrid *params;
} EffGridRecord;

/* Surface parameters copied into the node: 0x1C bytes at +0x10. */
typedef struct EffSurfaceParams {
    u32 word[7];
} EffSurfaceParams;

/* Slot count of an effect record, capped at max. */
static inline u32 effSlotCount(u8 *p, u32 max) {
    u32 a = (u32)((EffGrid *)p)->width;
    u32 n = (a ? a : (u32)((EffGrid *)p)->height) * (a ? (u32)((EffGrid *)p)->height : (u32)((EffGrid *)p)->altHeight);
    return n < max + 1 ? n : max;
}

extern void sndLoadAndPlayStationedSe(u32);

extern u32 effCreateTrackSetWithSharedReferences(u32, u16, u32);

extern u32 effRetainResource(u32);

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

extern u8 *effAllocateActiveInstanceWork(u16, void *);


extern u8 *effAllocateBlockWithModel(u16, void *);

extern u32 effCreateModelResourceWithInlineData(u16, void *, void *, u32);

extern void fileQueueDestroy(s32);

extern void *fileQueueClone(void *);

extern void sdfMotionSampleAtFrame(s32, f32);

extern u8 *D_00437E40;

void *effCloneModelWithVUState(void *sourceModel);

void effDestroyModelContext(s32 owner);

void effInitModelVUState(void *model);

RefObj *effReferenceObjectRetain(RefObj *obj);

void effReleaseReferenceHolder(u32 *holder);

RefObj *effRetainSharedReference(RefObj *obj);

u32 *effDuplicateSmallHeader(src)
    void *src;
{
    u32 *p = (u32 *)sdfAllocSizeClassBlock(0xc);
    p[0] = 0;
    memcpy(p + 1, src, 8);
    return p;
}

void effCreateSmallHeaderFromFile(void) {
    u64 resource;

    resource = fileResolvePrimaryBuffer();
    effDuplicateSmallHeader(resource);
}

void effReleaseFadeHeaderAllocation(u32 allocation) {
    kwlnCancelConfiguredFadeFrames();
    sdfReleaseChipBlock(allocation);
}

void effCloneSmallHeaderFromWork(s32 work) {
    effDuplicateSmallHeader(work + 4);
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
    VU0_STORE_VF(vf0, effect);
    memcpy(effect + 0x18, source, 0x40);
    return effect;
}

void effCreateFadeVectorFromFile(void) {
    u64 resource;

    resource = fileResolvePrimaryBuffer();
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

INCLUDE_ASM(const s32, "game/code_002DE248", func_002DE460);

void effCopyFadeWorkVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetFadeVectorColor(s32 work, u32 value) {
    ((EffFadeVectorWork *)work)->color = value;
}

/* Selection effect stores paired 8-byte entries and their reset buffer. */
typedef struct EffSelectionWork {
    u32 frame;
    u32 value04;
    u32 *entries;
    u8 pad_0C[4];
    u32 buffer;
    u8 pad_14[0x38];
    u32 count;
} EffSelectionWork;

void effResetSelectionEntryBuffers(s32 work) {
    u32 count;
    u32 *entries;
    u32 index;

    index = 0;
    count = ((EffSelectionWork *)work)->count;
    entries = ((EffSelectionWork *)work)->entries;
    if (count != 0) {
        do {
            index = index + 1;
            *entries = 0xffffffff;
            entries = entries + 2;
        } while (index < count);
    }
    memset(((EffSelectionWork *)work)->buffer, 0, count << 3);
}

void effCreateSelectionFlagListFromWork() {
    func_00316528();
}

void effCreateSelectionFlagListFromFile(void) {
    sdfInitializeFlagListFromResource();
}

void effReleaseSelectionFlagList(void) {
    sdfReleaseFlagListResource();
}

void effCreateEmbeddedSelectionFlagList(s32 p) {
    effCreateSelectionFlagListFromWork(p + 0x14);
}

void effResetSelectionEntriesAndState(s32 *p) {
    effResetSelectionEntryBuffers((s32)p);
    *p = 0;
}

void func_002DEC08() {
    if ((effModelUpdateControlFlags & 2) == 0) {
        func_00316680();
        return;
    }
}

void effDrawSelectionEntryVectors() {
    func_00316C88();
}

void effUpdateAndDrawSelectionEntries(s32 p) {
    func_002DEC08(p);
    effDrawSelectionEntryVectors(p);
}

void func_002DEC78(s32 work, u32 value) {
    ((EffSelectionWork *)work)->value04 = value;
}

u32 *effDuplicatePayloadHeader(src)
    void *src;
{
    u32 *p = (u32 *)sdfAllocSizeClassBlock(0x14);
    p[0] = 0;
    memcpy(p + 1, src, 16);
    return p;
}

void effCreateSelectionHeaderFromFile(void) {
    u64 resource;

    resource = fileResolvePrimaryBuffer();
    effDuplicatePayloadHeader(resource);
}

void effReleaseSelectionHeaderAllocation(u32 allocation) {
    evtDestroySelectionState();
    sdfReleaseChipBlock(allocation);
}

void effCloneSelectionHeaderFromWork(s32 work) {
    effDuplicatePayloadHeader(work + 4);
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

extern f32 func_002D7770(u8 *, s32, s32);

extern f32 mnuMeasureProjectedPerpendicularDistance(f32);

extern void effDrawBlurRectangle(void *);

extern void func_0018ECD0(void *);

extern void effBlurStepScaleSlotsAndDraw(void *);

extern void effBlurDrawFramebufferQuad(void *);

extern void func_0018F840(void *);

typedef struct EffFadeOut {
    u32 color;    // 0x00
    u32 param;    // 0x04
    u32 unk8;     // 0x08
    u32 unkC;     // 0x0C
    s32 unk10;    // 0x10
    s32 unk14;    // 0x14
} EffFadeOut;

/* Curve entry the blend helper (func_00296F58 / func_002D7458) reads; 0x24 bytes. */
typedef struct EffFadeCurve {
    u8 mode;          /* 0x00: 0 = ramp from the limit, 1 = hold, 2 = step */
    u8 pad01[3];
    f32 span;         /* 0x04: divisor when mode is 0 */
    f32 rate;         /* 0x08 */
    u8 pad0C[8];
    f32 value2;       /* 0x14 */
    f32 threshold;    /* 0x18: compared against progress in modes 1 and 2 */
    f32 value3;       /* 0x1C */
    f32 value4;       /* 0x20 */
} EffFadeCurve; /* 0x24 */

/* Second curve the blend helper takes; 0x10 bytes at 0x24. */
typedef struct EffFadeCurve2 {
    u8 mode;       /* 0x00 */
    u8 pad01[3];
    f32 span;      /* 0x04 */
    f32 rate;      /* 0x08 */
    f32 value2;    /* 0x0C */
} EffFadeCurve2; /* 0x10 */

/* Curve entry the rate helper (func_00297270) reads; 0x2C bytes. */
typedef struct EffRateCurve {
    u8 mode;          /* 0x00 */
    u8 pad01[3];
    f32 span;         /* 0x04: divisor when mode is 0 */
    f32 rate;         /* 0x08 */
    u8 pad0C[8];
    f32 value2;       /* 0x14 */
    f32 threshold;    /* 0x18 */
    f32 value3;       /* 0x1C */
    f32 value4;       /* 0x20 */
    u8 pad24[8];
} EffRateCurve; /* 0x2C */

typedef struct EffFadeConfig {
    /* Individually placed curves, not a regular array. The two the blend
     * helper takes sit at 0x00 and 0x34; the two the rate helper takes sit at
     * 0x60 and 0x8C. Strides are irregular, so they are named, not indexed. */
    EffFadeCurve blendA;   /* 0x00 */
    EffFadeCurve2 blendB2; /* 0x24 */
    EffFadeCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffRateCurve rateA;   /* 0x60 */
    EffRateCurve rateB;   /* 0x8C */
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
    /* Individually placed curves, not a regular array. The two the blend
     * helper takes sit at 0x00 and 0x34; the two the rate helper takes sit at
     * 0x60 and 0x8C. Strides are irregular, so they are named, not indexed. */
    EffFadeCurve blendA;   /* 0x00 */
    EffFadeCurve2 blendB2; /* 0x24 */
    EffFadeCurve blendB;   /* 0x34 */
    u8 pad58[8];
    EffRateCurve rateA;   /* 0x60 */
    EffRateCurve rateB;   /* 0x8C */
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
    second = func_002D7458(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_002D7770(&config->blendB, limit, progress) * 0.01f + 1.0f;
    out->rateB = func_002D7770(&config->rateA, limit, progress) * 0.01f;
    out->param = work->mode;
    effDrawBlurRectangle(out);
}

void effCreateFadeBlendWorkFromOutput(s32 work) {
    effCloneBlurTemplate(work + 0xc0);
}

void effReleaseFadeBlendWork(void) {
    effReleaseBlurTemplate();
}

typedef struct EffFadeMapOut {
    s32 mode;
    u32 color;
    u32 param;
    f32 rateB;
    f32 rateA;
    s32 posX;
    s32 posY;
} EffFadeMapOut;

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(EffKindWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->payload;
    EffFadeMapOut *out = (EffFadeMapOut *)work->handle;
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
    rate = func_002D7770(&config->rateB, limit, progress);
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
    second = func_002D7458(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_002D7770(&config->blendB, limit, progress) * 0.01f + 1.0f;
    out->rateB = func_002D7770(&config->rateA, limit, progress) * 0.01f;
    out->param = work->mode;
    effDrawBlurFixedPointRectangle(out);
}

void effSetFadeMapParameter(EffKindWork *work, u32 value) {
    ((EffMapOutWide *)work->handle)->target = value;
}

void effCreateFixedSlotBlurWorkFromFadeOutput(s32 work) {
    func_0018EBC8(work + 0xc0);
}

void effReleaseFixedSlotBlurWork(void) {
    effBlurReleaseFirstResource();
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
    rate = func_002D7770(&config->rateB, limit, progress);
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
    second = func_002D7458(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_002D7770(&config->blendB, limit, progress) * 0.01f;
    out->rateB = func_002D7770(&config->rateA, limit, progress) * 0.01f;
    out->param = work->mode;
    func_0018ECD0(out);
}

void effSetWideFadeMapParameter(EffKindWork *work, u32 value) {
    ((EffMapOutWide *)work->handle)->target = value;
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
    rate = func_002D7770(&config->rateB, limit, progress);
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
    second = func_002D7458(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_002D7770(&config->blendB, limit, progress) * 0.01f;
    out->rateB = func_002D7770(&config->rateA, limit, progress) * 0.01f;
    out->param = work->mode;
    effBlurStepScaleSlotsAndDraw(out);
}

void effSetFadeBlendParameter(EffKindWork *work, u32 value) {
    ((EffMapOutWide *)work->handle)->target = value;
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
    second = func_002D7458(&config->blendA, &config->blendB2, limit, progress);
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
    out->rateA = func_002D7770(&config->blendB, limit, progress) + 1.0f;
    out->rateB = func_002D7770(&config->rateA, limit, progress);
    out->param = work->mode;
    effBlurDrawFramebufferQuad(out);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002DF6C8);

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
    second = func_002D7458(&config->blendA, &config->blendB2, limit, progress);
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
    func_0018F840(out);
}

void effCreateFadeColorWorkFromOutput(s32 work) {
    effCloneResourceTemplate(work + 0xc0);
}

void effReleaseFadeColorWork(void) {
    func_0018FC88();
}

/* This projected fade also consumes EffKindWork: position, handle and payload. */
INCLUDE_ASM(const s32, "game/code_002DE248", func_002DFAB0);

void func_002DFC78(EffKindWork *work, u32 value) {
    ((EffRateOut *)work->handle)->target = value;
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
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateKindWork(work->option, source);
    s32 *secondary = (s32 *)fileResolveSecondaryBuffer(work);

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
    if ((effModelUpdateControlFlags & 2) == 0) {
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
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = effAllocateAlternateKindWork(work->option, source);
    s32 *secondary = (s32 *)fileResolveSecondaryBuffer(work);

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
    if ((effModelUpdateControlFlags & 2) == 0) {
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

extern s32 sdfTexAcquireResourceTexture(s32 *);

extern s32 effGetResourceFirstWord(s32);

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
        object[2] = sdfTexAcquireResourceTexture(source);
        break;
    case 4:
        object[2] = effGetResourceFirstWord(*source);
        break;
    }
    return object;
}

extern void sdfTexReleaseReferenceViaHandler(s32);

void effKindAssetReferenceRelease(s32 *object) {
    object[1]--;
    if (object[1] == 0) {
        if (object[0] != 4) {
            sdfTexReleaseReferenceViaHandler(object[2]);
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
    u128 dir;
    f32 angle;
    f32 len;

    if (work->frame < billGetFirstEntryFramePeriod(work->billboard)) {
        billSetEntryFrameMode1(work->billboard, work->frame);
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
        billSetLengthExtent(work->billboard, angle);
        effCopyVector(work->billboard, work);
        billInvokeCallback(work->billboard);
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
    u32 allocation;    // 0x08
} EffFrameState;

/* Frame-reset callbacks read the saved frame state and its configuration. */
typedef struct EffBillFrameWork {
    u8 pad00[0x30];
    u8 *frameState;    // 0x30
    u8 *config;        // 0x34
} EffBillFrameWork;

/* Billboard configuration shared by the frame builders and draw callbacks. */
typedef struct EffBillConfig {
    u8 pad_00[0x28];
    u32 textureId;      // 0x28, copied into the output record
    u8 pad_2C[8];
    u32 progress;       // 0x34
    union {
        u32 count;      // 0x38
        s32 signedCount;
    } frames;
    u8 outputMode;      // 0x3C, copied to the blend output
    u8 pad_3D[0x19];
    u8 mode;            // 0x56
    u8 pad_57[0x19];
    u32 drawProgress;    // 0x70, progress of the mesh-draw variant
    u8 pad_74[4];
    f32 fadeInEnd;      // 0x78, ramp-up length as a fraction of `resourceId`
    f32 fadeOutStart;   // 0x7C, start of the ramp-down
    u8 pad_80[8];
    f32 rowOffset;     // 0x88, row spacing or circular radius
    u32 resourceId;     // 0x8C
    u8 pad_90[4];
    u8 meshMode;        // 0x94, copied to the mesh output
    u8 pad_95[0x24];
    u8 alternateMode;   // 0xB9, animation variants use this instead of mode
    u8 pad_BA[0x23];
    u8 alternateMeshMode; // 0xDD, copied to the other mesh-draw variant
} EffBillConfig;

void effClearBillFrames(u8 *owner) {
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

u8 *effCreateBillFrameNode(u8 *config, u32 handle) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    ((EffFrameState *)node)->entries = body;
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(count, 0, handle);
    return node;
}

/* Shared billboard work prefix: retained references and the backing allocation. */
typedef struct EffBillOwnedWork {
    u32 reserved;
    u32 references; /* 0x04 */
    u32 allocation; /* 0x08 */
} EffBillOwnedWork;

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    sdfReleaseResourceAllocation(((EffBillOwnedWork *)work)->allocation);
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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x1C;
        } while (remaining);
    }
}

u8 *billCreateCellNode(u8 *config, u32 handle) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(count * 0x1C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(count, 1, handle);
    return node;
}

void billReleaseCellNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E11D0);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x2C;
        } while (remaining);
    }
}

u8 *billCreateParticleNode(u8 *config, u32 handle) {
    u32 count = ((EffBillConfig *)config)->frames.count;
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    ((EffFrameState *)node)->asset = (u8 *)effCreateTrackSetWithSharedReferences(count, 1, handle);
    return node;
}

void billReleaseParticleNode(s32 work) {
    effReleaseResourceRefs(((EffFrameState *)work)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E1BB0);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
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

void effInitializeAlternatingTransformRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)((EffFrameState *)work)->asset)->transformRows;
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

u8 *billCreateAnimatedTransform(u8 *descriptor, s32 handle) {
    u8 *work = billAllocateAnimatedTransformEntries(descriptor);

    ((EffBillOwnedWork *)work)->references = effCreateTrackSetWithSharedReferences(((EffBillConfig *)descriptor)->frames.count, 3, handle);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

u8 *billCloneAnimatedTransform(u8 *owner) {
    u8 *descriptor = ((EffBillFrameWork *)owner)->config;
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *work = billAllocateAnimatedTransformEntries(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

void billReleaseAlternatingTransformNode(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    sdfReleaseResourceAllocation(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E26A0);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
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
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    return node;
}

void billInitializeEmitterRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)((EffFrameState *)work)->asset)->transformRows;
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

u8 *billCreateEmitterTransform(u8 *descriptor, s32 handle) {
    u8 *work = billAllocEmitterNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effCreateTrackSetWithSharedReferences(((EffBillConfig *)descriptor)->frames.count, 4, handle);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

u8 *billCloneEmitterTransform(u8 *owner) {
    u8 *descriptor = ((EffBillFrameWork *)owner)->config;
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *work = billAllocEmitterNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

void billReleaseEmitterNode(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    sdfReleaseResourceAllocation(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E3008);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
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
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x28 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    ((EffFrameState *)node)->allocation = (u32)base;
    *(u8 **)node = body;
    return node;
}

void billInitializeStripRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)((EffFrameState *)work)->asset)->transformRows;
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

u8 *billCreateStripTransform(u8 *descriptor, s32 handle) {
    u8 *work = billAllocStripNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effCreateTrackSetWithSharedReferences(((EffBillConfig *)descriptor)->frames.count, 3, handle);
    billInitializeStripRows(work, descriptor);
    return work;
}

u8 *billCloneStripTransform(u8 *owner) {
    u8 *descriptor = ((EffBillFrameWork *)owner)->config;
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *work = billAllocStripNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    billInitializeStripRows(work, descriptor);
    return work;
}

void billReleaseStripNode(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    sdfReleaseResourceAllocation(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E39B0);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x2C;
        } while (remaining);
    }
}

u8 *billCreateTrailNode(u8 *config, u32 handle) {
    u32 headerSize = 0x10;
    u8 *base = sdfAllocGeneralBlock(((EffBillConfig *)config)->frames.count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    ((EffBillOwnedWork *)node)->references = effCreateTrackSetWithSharedReferences(*(u32 *)(config + 0x38), 0, handle);
    return node;
}

void billReleaseTrailNode(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    sdfReleaseResourceAllocation(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E4320);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *descriptor = ((EffFrameState *)source)->asset;
    u8 *entry = ((EffFrameState *)source)->entries;
    s32 count = ((EffBillConfig *)((EffBillFrameWork *)owner)->config)->frames.signedCount;

    memset(((EffFrameAsset *)descriptor)->frameStorage, 0, ((EffFrameAsset *)descriptor)->frameCount * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x20;
        } while (remaining);
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

void billInitializeQuadRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)((EffFrameState *)work)->asset)->transformRows;
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

u8 *billCreateQuadTransform(u8 *descriptor, s32 handle) {
    u8 *work = billAllocQuadNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effCreateTrackSetWithSharedReferences(((EffBillConfig *)descriptor)->frames.count, 4, handle);
    billInitializeQuadRows(work, descriptor);
    return work;
}

u8 *billCloneQuadTransform(u8 *owner) {
    u8 *descriptor = ((EffBillFrameWork *)owner)->config;
    u8 *source = ((EffBillFrameWork *)owner)->frameState;
    u8 *work = billAllocQuadNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    billInitializeQuadRows(work, descriptor);
    return work;
}

void billReleaseQuadNode(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    sdfReleaseResourceAllocation(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E4E80);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002E5E88(out, mtx);
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
    u32 size = D_003E9950[kind].payloadSize;
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

typedef struct EffInstance {
    u8 pad_00[0x30];
    void *resourceHandle;
} EffInstance;

u8 *effCreateResourceInstanceA(u16 kind, void *source, u32 extra) {
    EffInstance *work = (EffInstance *)effAllocateActiveInstanceWork(kind, source);
    work->resourceHandle = D_003E9950[kind].createResource(source, extra);
    D_003E9950[kind].initialize(work);
    return (u8 *)work;
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


void effDispatchDestroyOp(u32 *obj) {

    D_003E9950[obj[0x2C / 4]].destroyResource(obj[0x30 / 4]);
    sdfReleaseChipBlock(obj);
}

typedef struct EffActiveInstance {
    u8 pad_00[0x2C];
    s32 kind;
    void *resource;
    void *source;
} EffActiveInstance;

u8 *effCreateActiveResource(EffActiveInstance *obj) {
    EffActiveInstance *work;

    if (D_003E9950[obj->kind].cloneResource == NULL) {
        work = (EffActiveInstance *)effCreateResourceInstanceA((u16)obj->kind, obj->source, 0);
    } else {
        work = (EffActiveInstance *)effAllocateActiveInstanceWork((u16)obj->kind, obj->source);
        work->resource = D_003E9950[obj->kind].cloneResource(obj);
        D_003E9950[obj->kind].initialize(work);
    }
    return (u8 *)work;
}

void effResetActiveInstanceFrame(EffClassWork *work) {
    D_003E9950[work->kind].initialize();
    work->frame = 0;
}

void effAdvanceActiveInstanceFrame(work)
s32 *work;
{
    if ((effModelUpdateControlFlags & 2) == 0) {
        D_003E9950[work[0x2C / 4]].update();
        work[0x28 / 4]++;
    }
}

void effDispatchActiveInstanceDraw(s32 work) {
    D_003E9950[((EffClassWork *)work)->kind].draw((void *)work);
}

void effUpdateAndDrawActiveInstance(u32 work) {
    effAdvanceActiveInstanceFrame();
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

extern s32 *sdfCreateAssetWithDrawEntries(void);

extern void func_003332D0(void *, f32);

extern u16 D_004582B0[];

extern s32 D_00437E58[2];

extern void *D_00437E60[2];

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042BC10);

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
    func_003332D0(set->handle, 1.0f);
    memset(D_004582B0, 0, 0x2C);
    D_004582B0[2] = 0x4000;
    return set;
}

extern u32 D_00437E54;

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E48);

INCLUDE_SDATA(const s32, "game/code_002DE248", effFlashTextureHandles);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E54);

INCLUDE_SDATA(const s32, "game/code_002DE248", D_00437E58);

u32 effCreateTrackSetWithSharedReferences(u32 count, u16 kind, u32 sharedRef) {
    EffTrackSet *effect = effCreateTrackSet(count, kind);

    if (effect->columns != 0) {
        if (sharedRef == 0) {
            switch (effect->kind) {
            case 3:
                if (D_00437E58[0] == 0) {
                    D_00437E60[0] = (RefObj *)effCloneSharedReferenceWithValue(effFlashTextureHandles, 0x100);
                }
                D_00437E58[0]++;
                break;
            case 4:
                if (D_00437E58[1] == 0) {
                    D_00437E60[1] = (RefObj *)effCloneSharedReferenceWithValue(D_00437E54, 0x101);
                }
                D_00437E58[1]++;
                break;
            }
        } else {
            effect->shared = func_002DDAA8((void *)sharedRef);
        }
    }
    return (u32)effect;
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
    sdfQueueAssetRelease((u32)work->handle);
    sdfReleaseResourceAllocation((u32)work->allocation);
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
                D_00437E58[0]++;
                break;
            case 4:
                D_00437E58[1]++;
                break;
            }
        }
    }
    return (u32)effect;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E5E88);

extern u32 D_00437E48[2];

void effLoadFlashTextures(void) {
    D_00437E48[0] = sdfReadNamedResource("/effect/flash00.tmx", &effFlashTextureHandles, 0);
    D_00437E48[1] = sdfReadNamedResource("/effect/flash01.tmx", &effFlashTextureHandles + 1, 0);
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

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E64F0);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u32 resource;

    resource = ((EffClassDrawState *)((EffBillFrameWork *)work)->frameState)->effect;
    ((EffCounterHeader *)((EffClassDrawState *)((EffBillFrameWork *)work)->frameState)->references)->frame = 0;
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

extern void effAdvanceClassFrame();

void func_002E6B68(BillCellDrawWork *work) {
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

extern void effRunClassPostFrame(s32);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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

void effResetClassRingFrame(s32 work) {
    ((EffClassDrawState *)((EffBillFrameWork *)work)->frameState)->ring->frame = 0;
}

/* Create a point-set reference and initialize four colors per segment. */
u32 *effCreateRingHandle(u8 *work) {
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

void effReleaseRingHandle(u32 handle) {
    effAssetQueueRelease(*(u32 *)handle);
    sdfReleaseChipBlock(handle);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002E6F48);

void billDrawCellBlendB(BillCellDrawWork *work) {
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
    second = func_002D7458(config, config + 0x24, limit, progress);
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

u8 *effPayloadPointerSet(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9B80[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED(vf0, effect);
    VU0_STORE_VF_UNCLOBBERED(vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = D_003E9B80[kind].createResource(source);
    D_003E9B80[kind].initialize(effect);
    return effect;
}

void effCreateClassWorkFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effPayloadPointerSet(((FileJob *)request)->option, source);
}

void effDestroyClassWork(u32 *obj) {
    D_003E9B80[obj[0x2C / 4]].destroyResource(obj[0x30 / 4]);
    sdfReleaseChipBlock(obj);
}

void effCreateClassWorkFromRequest(s32 work) {
    effPayloadPointerSet(*(u16 *)(work + 0x2c), ((EffClassWork *)work)->payload);
}

void effInitializeClassFrame(u8 *work) {
    D_003E9B80[((EffClassWork *)work)->kind].initialize();
    ((EffClassWork *)work)->frame = 0;
}

void effAdvanceClassFrame(work)
s32 *work;
{
    if ((effModelUpdateControlFlags & 2) == 0) {
        D_003E9B80[work[0x2C / 4]].update();
        work[0x28 / 4]++;
    }
}

void effRunClassPostFrame(s32 work) {
    D_003E9B80[((EffClassWork *)work)->kind].draw((void *)work);
}

void effUpdateClassFrame(u32 work) {
    effAdvanceClassFrame();
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
    func_003332D0(set->handle, 1.0f);
    memset(D_004582E0, 0, 0x2C);
    D_004582E0[0].primitive = 0x4000;
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
            effSubmitSurfacePacket(D_003E9C28[set->type], list);
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

typedef struct EffectSlotNode54 {
    u32 count;
    u32 color;
    f32 opacity;
    u32 kind;             // 0x0C
    EffSurfaceParams params; // 0x10
    u32 index;             // 0x2C
    u32 handleBuffer;      // 0x30
    u32 billResource;      // 0x34
    u32 *jobs;             // 0x38
    u32 jobBuffer;         // 0x3C
    u32 *queues;           // 0x40
    u32 queueBuffer;       // 0x44
    RefObj *resourceHolder; // 0x48
    u32 record;            // 0x4C
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
    s32 primary = (s32)fileResolvePrimaryBuffer();
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
    s32 *data = (s32 *)fileResolveSecondaryBuffer(source);
    if (data != NULL) {
        effConfigureSurfaceNodeByKind(object, ((FileJob *)source)->slots[0].selector, data);
    }
    return (s32)object;
}

extern void effReleaseSurfaceGridBuffers(s32);

extern void fileJobDestroy(u32);

extern u32 fileJobCreateFromJob(u32);

extern u32 fileJobCreateChild(u32);

void effDestroySurfaceNode(EffectSlotNode54 *node) {
    u32 i;
    u32 count;

    if (node->billResource != 0) {
        billDispatchByKind(node->billResource);
    }
    if (node->jobBuffer != 0) {
        count = ((EffGridRecord *)node->record)->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobBuffer);
    }
    if (node->queueBuffer != 0) {
        count = ((EffGridRecord *)node->record)->count;
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfReleaseResourceAllocation(node->queueBuffer);
    }
    if (node->index != 0) {
        for (i = 0; i < node->count; i++) {
            effReleaseSurfaceGridBuffers(((u32 *)node->index)[i]);
        }
        sdfReleaseResourceAllocation(node->handleBuffer);
    }
    if (node->resourceHolder != 0) {
        effReleaseReferenceHolder((u32 *)node->resourceHolder);
    }
    if (node->record != 0) {
        fileReleaseGridRecordHandle(node->record);
    }
    sdfReleaseChipBlock(node);
}

s32 effRecreateSurfaceNodeFromWork(u8 *work) {
    EffGridRecord *config = (EffGridRecord *)((EffectSlotNode54 *)work)->record;
    s32 arg = (s32)config->params;
    s32 object = effCreateSurfaceNodeForGrid(arg);

    effRebuildSurfaceHandles(object, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->kind, (s32)work + 0x10);
    effReplaceResourceRef(object, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->kind, arg);
    return object;
}


void func_002E7F60(EffectSlotNode54 *, u8 *);

u32 effCreateSurfaceGridWithConfiguration(u8 *work) {
    EffGridRecord *config = (EffGridRecord *)((EffectSlotNode54 *)work)->record;
    s32 arg = (s32)config->params;
    s32 object = effCreateSurfaceNodeForGrid(arg);

    effRebuildSurfaceHandles(object, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->kind, (s32)work + 0x10);
    effReplaceResourceRef(object, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->kind, arg);
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
            billSetBillboardMode(dst->billResource, ((EffGridRecord *)dst->record)->bill->mode);
        }
        break;
    case 5:
        count = ((EffGridRecord *)src->record)->count;
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
    case 6:
        count = ((EffGridRecord *)src->record)->count;
        if (dst->queueBuffer != 0) {
            for (i = 0; i < count; i++) {
                fileQueueDestroy(dst->queues[i]);
            }
            sdfReleaseResourceAllocation(dst->queueBuffer);
            dst->queues = 0;
            dst->queueBuffer = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->queueBuffer = (u32)sdfAllocGeneralBlock(size);
        dst->queues = (u32 *)sdfResourceRetainAddress(dst->queueBuffer);
        for (i = 0; i < count; i++) {
            dst->queues[i] = (u32)fileQueueClone((void *)src->queues[0]);
        }
        break;
    case 7:
        if (dst->resourceHolder != 0) {
            effReleaseReferenceHolder((u32 *)dst->resourceHolder);
        }
        dst->resourceHolder = effReferenceObjectRetain(src->resourceHolder);
        break;
    }
    dst->kind = src->kind;
}

typedef struct EffMotionSetup {
    u16 mode;       // 0x00
    u16 kind;       // 0x02
    u16 flags;      // 0x04
    u8 pad_06[6];
    void *table;    // 0x0C
    u8 pad_10[0x1C];
} EffMotionSetup;   // 0x2C


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
        sdfReleaseResourceAllocation(node->handleBuffer);
    }
    node->handleBuffer = (u32)sdfAllocGeneralBlock(size);
    node->index = sdfResourceRetainAddress(node->handleBuffer);
    for (i = 0; i < node->count; i++) {
        handle = effCreateSurfaceGridNode(params[0], params[1]);
        ((u32 *)node->index)[i] = handle;
        effFillSurfaceGridColorGradient(handle, params + 2);
    }
}

void effReplaceResourceRef(s32 nodeAddr, s32 entryId, s32 resource) {
    EffectSlotNode54 *node = (EffectSlotNode54 *)nodeAddr;
    if (node->record != 0) {
        fileReleaseGridRecordHandle(node->record);
    }
    node->record = fileAllocateGridRecordSlots(entryId & 0xffff, node->count, resource);
}

void effSetSurfaceRetainedResource(s32 *object, s32 arg) {
    u8 *work = (u8 *)object;
    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = effRetainResource(arg);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->bill->mode);
    }
}

void effReplaceSurfacePrimaryBillboard(s32 *object, s32 *settings) {
    u8 *work = (u8 *)object;

    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = billCreateIndexed(0, settings);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->bill->mode);
    }
}

void effReplaceSurfaceFlaggedBillboard(s32 *object, s32 *settings) {
    u8 *work = (u8 *)object;

    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = billCreateIndexed(1, settings);
    billMarkKindOneFlag(((EffectSlotNode54 *)work)->billResource);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, ((EffGridRecord *)((EffectSlotNode54 *)work)->record)->bill->mode);
    }
}

void effRebuildSurfaceJobs(EffectSlotNode54 *node, void *source) {
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

extern void *fileQueueClone(void *);

void effSurfaceNodeCreateQueues(EffectSlotNode54 *node, void *source) {
    u32 count = ((EffGridRecord *)node->record)->count;
    u32 i;
    u32 size;

    if (node->queueBuffer != 0) {
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfReleaseResourceAllocation(node->queueBuffer);
        node->queues = 0;
        node->queueBuffer = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueBuffer = (u32)sdfAllocGeneralBlock(size);
        node->queues = (u32 *)sdfResourceRetainAddress(node->queueBuffer);
        node->queues[0] = (u32)fileCloneQueueEntries((FileQueue *)source);
        for (i = 1; i < count; i++) {
            node->queues[i] = (u32)fileQueueClone((void *)node->queues[0]);
        }
    }
}

void effReplaceSurfaceResourceHolder(s32 node, u32 resource) {
    u32 holder;

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
    if (effModelUpdateControlFlags & 2) {
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
    dds3DispatchIndexedCallback(((EffectSlotNode54 *)p)->record, value);
}

extern EffMotionSetup D_00458310;

extern EffMotionSetup D_00458340;

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
    s32 *handle;        // 0x3C
    u8 *queueA;         // 0x40
    u8 *queueB;         // 0x44
    u8 *allocation;     // 0x48
} EffSurfaceGridNode;

u32 effCreateSurfaceGridNode(u32 count, u32 columns) {
    s32 rows = count * columns * 3 + 6;
    s32 size = rows * 20 + 0xA0;
    u8 *base;
    u8 *data;
    EffSurfaceGridNode *node;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfAllocGeneralBlock(size + 0x4C);
    data = (u8 *)sdfResourceRetainAddress((u32)base);
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
    memset(&D_00458310, 0, sizeof(EffMotionSetup));
    D_00458310.flags = 0x4000;
    D_00458310.table = D_003E9C40;
    memset(&D_00458340, 0, sizeof(EffMotionSetup));
    D_00458340.flags = 0x4000;
    D_00458340.table = D_003E9C90;
    D_00458340.mode = 6;
    D_00458340.kind = 8;
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
    sdfQueueAssetRelease((u32)((EffSurfaceGridNode *)work)->handle);
    sdfReleaseResourceAllocation((u32)((EffSurfaceGridNode *)work)->allocation);
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
    void *work = sdfAllocPacketAligned(0x20);
    effCurrentRenderPacket = (u32)work;
    sdfInitPacketList(work);
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(effCurrentRenderPacket, 0);
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

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EA120);

void effReleaseRenderResources(u32 *p) {
    if (((EffRenderResourceState *)p)->billHandle != 0) {
        billDispatchByKind(((EffRenderResourceState *)p)->billHandle);
    }
    if (((EffRenderResourceState *)p)->reference != NULL) {
        effReleaseReferenceHolder(((EffRenderResourceState *)p)->reference);
    }
    if (((EffRenderResourceState *)p)->assetHandle != 0) {
        sdfQueueAssetRelease(((EffRenderResourceState *)p)->assetHandle);
    }
    sdfReleaseChipBlock(p);
}

u8 *effCloneRenderResourceWork(u8 *source) {
    u8 *effect = func_002EA120(NULL);
    memcpy(effect + 0x30, source + 0x30, 0x98);
    effDuplicateRenderResourceOwner(effect, source);
    return effect;
}

void effDuplicateRenderResourceOwner(u32 *dst, u32 *src) {
    if (((EffRenderResourceState *)src)->billHandle != 0) {
        if (((EffRenderResourceState *)dst)->billHandle != 0) {
            billDispatchByKind(((EffRenderResourceState *)dst)->billHandle);
        }
        ((EffRenderResourceState *)dst)->billHandle = billCloneObjectRetainingSharedData(((EffRenderResourceState *)src)->billHandle);
        billMarkKindOneFlag(((EffRenderResourceState *)dst)->billHandle);
        billSetBillboardMode(((EffRenderResourceState *)dst)->billHandle, ((EffRenderResourceState *)dst)->billMode);
    } else {
        if (((EffRenderResourceState *)dst)->reference != NULL) {
            effReleaseReferenceHolder(((EffRenderResourceState *)dst)->reference);
        }
        ((EffRenderResourceState *)dst)->reference = effReferenceObjectRetain(((EffRenderResourceState *)src)->reference);
    }
}

void effResetRenderResourceKind(s32 work) {
    ((EffClassWork *)work)->kind = 0;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EA5D8);

void effCopyRenderResourcePosition(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyRenderResourceOrientation(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetRenderResourceColor(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void effSetRenderResourceMatrixComponent(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}


extern s32 effMiscRand(s32 *);

void effRandomizeParticleFields(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 4) {
        entry[1] = -1 - (effMiscRand(effSharedRandomState) & 3);
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
    u32 *header = (u32 *)((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
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

extern void effResetDispatchCounter(u8 *);

void effResetBillboardFrameDispatchCounters(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++) {
        effResetDispatchCounter((u8 *)*entry++);
    }
}

extern u8 *effCreateClassResourceWork(u16, void *);
extern void *memcpy(void *dst, const void *src, u32 size);

u8 *func_002EB968(EffPointSetTableSource *config) {
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

extern void effDestroyClassResourceWork(s32);

void effReleaseBillFrameEntries(u8 *work) {
    u32 *header = (u32 *)((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        effDestroyClassResourceWork(*entry++);
    }
    sdfReleaseChipBlock(header);
}

extern void effAdvanceClassResourceFrame();

void effReleaseTrackEntriesA(u8 *work) {
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u32 **entry = (u32 **)((EffFrameState *)((EffBillFrameWork *)work)->frameState)->entries;
    u32 i;

    for (i = 0; i < count; i++) {
        effAdvanceClassResourceFrame(*entry++);
    }
}

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void effCopyClassResourcePosition(void *, void *);
extern void effCopyClassResourceOrientation(void *, void *);
extern void effSetClassResourceMatrixComponent(EffClassWork *, f32);
extern void effSetClassResourceColor(s32, u32);
extern void effDrawClassResourceWork(s32);

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
    second = func_002D7458((u8 *)config, (u8 *)config + 0x24, frame, progress);
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
        effDrawClassResourceWork((s32)*entries);
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

void effReleaseBillPointEntries(u8 *work) {
    u32 *header = (u32 *)((EffBillFrameWork *)work)->frameState;
    u32 count = ((EffBillConfig *)((EffBillFrameWork *)work)->config)->frames.count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        effReleasePointSetAsset((s32)((EffScaleRangeEntry *)entry)->set);
        entry += 12;
    }
    sdfReleaseResourceAllocation((u32)((EffScaleRange *)header)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EC370);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002ECD10);

void effSeedBillboardFrameCounters(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        entry[1] = -1 - (effMiscRand(effSharedRandomState) & 3);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002ECF78);

void effReleaseBillboardFramePointSets(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
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

u8 *effCreateClassResourceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9D00[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = D_003E9D00[kind].createResource(source);
    D_003E9D00[kind].initialize(effect);
    return effect;
}

void effCreateClassResourceFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effCreateClassResourceWork(((FileJob *)request)->option, source);
}

void effDestroyClassResourceWork(s32 work) {
    u32 *obj = (u32 *)work;
    D_003E9D00[obj[0x2C / 4]].destroyResource();
    sdfReleaseChipBlock(obj);
}

u32 effPayloadPointerGet(s32 work) {
    return effCreateClassResourceWork(*(u16 *)(work + 0x2c), ((EffClassWork *)work)->payload);
}

void effResetDispatchCounter(u8 *work) {
    D_003E9D00[((EffClassWork *)work)->kind].initialize(work);
    ((EffClassWork *)work)->frame = 0;
}


void effAdvanceClassResourceFrame(work)
s32 *work;
{
    if ((effModelUpdateControlFlags & 2) == 0) {
        D_003E9D00[work[0x2C / 4]].update();
        work[0x28 / 4]++;
    }
}

void effDrawClassResourceWork(s32 work) {
    D_003E9D00[((EffClassWork *)work)->kind].draw((void *)work);
}

void effUpdateAndDrawClassResource(u32 work) {
    effAdvanceClassResourceFrame();
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
    func_003332D0(set->handle, 1.0f);
    memset(D_004583A0, 0, 0x2C);
    D_004583A0[0].primitive = 0x4000;
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
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        D_003E9DC0[set->type]->submit(D_003E9DC0[set->type], list);
    }
}

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

EffectStripNode *effCreateStripNodeFromGrid(u8 *source) {
    return effCreateStripNode(effSlotCount(source, 100));
}

u8 *effFileResourceReferenceReplace(u8 *work) {
    u8 *primary = fileResolvePrimaryBuffer();
    u8 *source = primary + 0x20;
    u8 *node = (u8 *)effCreateStripNodeFromGrid(source);

    memcpy(node + 0xC, primary, 0x20);
    effReplaceFileResourceRef((s32)node, ((FileJob *)work)->option, (s32)source);
    return node;
}

void effReleaseModelResources(u32 *p) {
    if (p[0x30 / 4] != 0) {
        billDispatchByKind(p[0x30 / 4]);
    }
    if (p[0x2C / 4] != 0) {
        effReleaseResourceRefs(p[0x2C / 4]);
    }
    if (p[0x34 / 4] != 0) {
        fileReleaseGridRecordHandle(p[0x34 / 4]);
    }
    sdfReleaseChipBlock(p);
}

u8 *effCloneStripResourceFromOwner(u8 *work) {
    u8 *source = (u8 *)((EffGridRecord *)((EffBillFrameWork *)work)->config)->params;
    u8 *node = (u8 *)effCreateStripNodeFromGrid(source);

    memcpy(node + 0xC, source, 0x20);
    effReplaceFileResourceRef((s32)node, ((EffGridRecord *)((EffBillFrameWork *)work)->config)->kind, (s32)source);
    return node;
}

void effReplaceFileResourceRef(s32 obj, s32 id, s32 arg) {
    u32 *p = (u32 *)obj;
    if (p[0x34 / 4] != 0) {
        fileReleaseGridRecordHandle(p[0x34 / 4]);
    }
    p[0x34 / 4] = fileAllocateGridRecordSlots(id & 0xffff, p[0], arg);
}

void effClearStripRecordReferences(s32 node) {
    if (((EffectStripNode *)node)->active != 0) {
        fileClearRecordReferences(((EffectStripNode *)node)->active);
        return;
    }
}

void effAcquireStripRecord(s32 node) {
    if (effModelUpdateControlFlags & 2) {
        return;
    }
    if (((EffectStripNode *)node)->active != 0) {
        fileAcquireRecord(((EffectStripNode *)node)->active);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EE670);

void func_002EED48(u32 node) {
    effAcquireStripRecord(node);
    func_002EE670(node);
}

void effSetStripRecordVector(s32 node) {
    mnuRecordSetVector(((EffectStripNode *)node)->active);
}

void effSetStripRecordSecondaryVector(s32 node) {
    fileSetRecordSecondVector(((EffectStripNode *)node)->active);
}

void effSetStripRecordColor(s32 node, u32 color) {
    ((EffectStripNode *)node)->color = color;
}

void func_002EEDA8(u8 *p, f32 value) {
    ((EffectStripNode *)p)->opacity = value;
    dds3DispatchIndexedCallback(((EffectStripNode *)p)->active, value);
}

void effResetBillTable(u8 *p) {
    u8 *a = ((EffBillFrameWork *)p)->frameState;
    u8 *b = ((EffBillFrameWork *)p)->config;
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = (u32 *)((EffFrameAsset *)((EffFrameState *)a)->asset)->frameStorage;
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

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

u32 *effCreateRingFadeTable(u8 *p, u32 a1) {
    u32 *buf = (u32 *)effAllocateRingFadeEntries(p);
    buf[1] = effCreateRibbonWithSharedResource(((EffBillConfig *)p)->frames.count, ((EffBillConfig *)p)->resourceId, a1);
    effFillRingFadeGradient(buf, p);
    return buf;
}

u32 *effAssetPointerSet(u8 *p) {
    u8 *dst = ((EffBillFrameWork *)p)->config;
    u32 *src = (u32 *)((EffBillFrameWork *)p)->frameState;
    u32 *buf = (u32 *)effAllocateRingFadeEntries(dst);
    buf[1] = effCloneRibbonWithSharedResource((u32 *)((EffFrameState *)src)->asset);
    effFillRingFadeGradient(buf, dst);
    return buf;
}

void effReleaseRingFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EF1C0);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *a = ((EffBillFrameWork *)p)->frameState;
    u8 *b = ((EffBillFrameWork *)p)->config;
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = (u32 *)((EffFrameAsset *)((EffFrameState *)a)->asset)->frameStorage;
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
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

u32 *effCreateBillFadeTable(u8 *p, u32 a1) {
    u32 *buf = (u32 *)effAllocateBillFadeFrameEntries(p);
    buf[1] = effCreateRibbonWithSharedResource(((EffBillConfig *)p)->frames.count, ((EffBillConfig *)p)->resourceId, a1);
    effFillBillFadeGradient(buf, p);
    return buf;
}

u32 *effCloneBillFadeTable(u8 *p) {
    u8 *dst = ((EffBillFrameWork *)p)->config;
    u32 *src = (u32 *)((EffBillFrameWork *)p)->frameState;
    u32 *buf = (u32 *)effAllocateBillFadeFrameEntries(dst);
    buf[1] = effCloneRibbonWithSharedResource((u32 *)((EffFrameState *)src)->asset);
    effFillBillFadeGradient(buf, dst);
    return buf;
}

/* Release the bill-fade frame state's shared asset and backing allocation. */
void effReleaseBillFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002EFE20);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    u8 *a = ((EffBillFrameWork *)p)->frameState;
    u8 *b = ((EffBillFrameWork *)p)->config;
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = (u32 *)((EffFrameAsset *)((EffFrameState *)a)->asset)->frameStorage;
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x2C;
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

u32 *effCreateCompactRingFadeTable(u8 *p, u32 a1) {
    u32 *buf = (u32 *)effAllocateCompactRingFadeEntries(p);
    buf[1] = effCreateRibbonWithSharedResource(((EffBillConfig *)p)->frames.count, ((EffBillConfig *)p)->resourceId, a1);
    effFillCompactRingFadeGradient(buf, p);
    return buf;
}

u32 *effCloneBillboardFrameAsset(u8 *p) {
    u8 *dst = ((EffBillFrameWork *)p)->config;
    u32 *src = (u32 *)((EffBillFrameWork *)p)->frameState;
    u32 *buf = (u32 *)effAllocateCompactRingFadeEntries(dst);
    buf[1] = effCloneRibbonWithSharedResource((u32 *)((EffFrameState *)src)->asset);
    effFillCompactRingFadeGradient(buf, dst);
    return buf;
}

void effReleaseCompactRingFadeTable(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
    effSharedAssetReferenceRelease((u32)((EffFrameState *)state)->asset);
    sdfReleaseResourceAllocation(((EffFrameState *)state)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F0A98);

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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    VU0_LOAD_VF(vf10, D_003E9100);
    VU0_SCALAR_OP_CLOBBER(((BillCellDrawWork *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, work);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
    VU0_STORE_MATRIX(mtx);
    func_002F1888(out, mtx);
}

u8 *effAllocateBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9DD8[kind].payloadSize;
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

u8 *effCreateResourceInstanceB(u16 kind, void *source, u32 extra) {
    u8 *work = effAllocateBlock(kind, source);
    ((EffClassWork *)work)->resource = (u32)D_003E9DD8[kind].createResource(source, extra);
    D_003E9DD8[kind].initialize(work);
    return work;
}

extern u8 *effCreateResourceInstanceB(u16, void *, u32);

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

void effDestroyBlockResourceWork(u32 *obj) {
    D_003E9DD8[obj[0x2C / 4]].destroyResource();
    sdfReleaseChipBlock(obj);
}

u8 *effDuplicateActiveResourceB(u8 *obj) {
    u8 *work = effAllocateBlock(*(u16 *)(obj + 0x2C), ((EffClassWork *)obj)->payload);
    *(void **)(work + 0x30) = D_003E9DD8[((EffClassWork *)obj)->kind].cloneResource(obj);
    D_003E9DD8[((EffClassWork *)obj)->kind].initialize(work);
    return work;
}

void effResetBlockResourceFrame(u8 *work) {
    D_003E9DD8[((EffClassWork *)work)->kind].initialize();
    ((EffClassWork *)work)->frame = 0;
}

void effAdvanceBlockResourceFrame(work)
s32 *work;
{
    if ((effModelUpdateControlFlags & 2) == 0) {
        D_003E9DD8[work[0x2C / 4]].update();
        work[0x28 / 4]++;
    }
}

void effDrawBlockResourceWork(s32 work) {
    D_003E9DD8[((EffClassWork *)work)->kind].draw((void *)work);
}

void effUpdateAndDrawBlockResource(u32 work) {
    effAdvanceBlockResourceFrame();
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

extern EffMotionSetup D_004584C0;

extern EffMotionSetup D_00458400;

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
    s32 *handle;        // 0x2C
    u8 *allocation;     // 0x30
} EffRibbonWork;

extern EffMotionSetup D_004583D0;

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
    func_003332D0(work->handle, 1.0f);
    memset(&D_004583D0, 0, sizeof(EffMotionSetup));
    D_004583D0.flags = 0x4000;
    return (u8 *)work;
}

u32 effCreateRibbonWithSharedResource(u32 count, u32 repeat, u32 resource) {
    u8 *node = effCreateRibbonWork(count, repeat);

    if (resource == 0) {
        s32 references = effSharedRibbonReferenceCount;
        ((EffRibbonWork *)node)->resource = NULL;
        if (references == 0) {
            D_00437E78 = effCloneSharedReferenceWithValue(effWindTextureHandle, 0x300);
            references = effSharedRibbonReferenceCount;
        }
        references++;
        effSharedRibbonReferenceCount = references;
    } else {
        ((EffRibbonWork *)node)->resource = func_002DDAA8((void *)resource);
    }
    return (u32)node;
}

void effSharedAssetReferenceRelease(s32 work) {
    if (((EffRibbonWork *)work)->resource == NULL) {
        effSharedRibbonReferenceCount = effSharedRibbonReferenceCount - 1;
        if (effSharedRibbonReferenceCount == 0) {
            effReleaseSharedReference(D_00437E78);
            D_00437E78 = 0;
        }
    }
    else {
        effReleaseSharedReference(((EffRibbonWork *)work)->resource);
    }
    sdfQueueAssetRelease(((EffRibbonWork *)work)->handle);
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
    D_00437E6C = sdfReadNamedResource("/effect/wind00.tmx", &effWindTextureHandle, 0);
}

u32 effGetWindTextureHandle(void) {
    return effWindTextureHandle;
}

void effResetAnimationFrameEntries(u8 *p) {
    u8 *a = ((EffBillFrameWork *)p)->frameState;
    u8 *b = ((EffBillFrameWork *)p)->config;
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = ((EffFrameAsset *)((EffFrameState *)a)->asset)->animationFrames;
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

u8 *effAllocateAnimationBuffer(u8 *config) {
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

u32 *effPrepareTextureAnimation(u8 *src) {
    u32 *buf = (u32 *)effAllocateAnimationBuffer(src);
    buf[1] = effCreateTexturedStripWithSharedTexture(((EffBillConfig *)src)->frames.count, ((EffBillConfig *)src)->resourceId);
    effFillFadeColorRows(buf, src);
    return buf;
}

u32 *effPrepareOwnedTextureAnimation(u8 *p) {
    u8 *dst = ((EffBillFrameWork *)p)->config;
    u32 *src = (u32 *)((EffBillFrameWork *)p)->frameState;
    u32 *buf = (u32 *)effAllocateAnimationBuffer(dst);
    buf[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)src)->asset);
    effFillFadeColorRows(buf, dst);
    return buf;
}

void effReleaseTextureAnimationWork(s32 work) {
    s32 state;

    state = (s32)((EffBillFrameWork *)work)->frameState;
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
    second = func_002D7458(config, config + 0x24, limit, progress);
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

u32 effClampSlotCount(u8 *p) {
    return effSlotCount(p, 200);
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

extern void fileReleaseGridRecordHandle(s32);

extern void effReleaseScalyTextureReference(u32);

void effReleaseAnimationFrameResources(u8 *work) {
    u32 *state = (u32 *)((EffBillFrameWork *)work)->frameState;

    effReleaseScalyTextureReference(((EffAnimationState *)state)->textureHandle);
    if (((EffAnimationState *)state)->record != 0) {
        fileReleaseGridRecordHandle(((EffAnimationState *)state)->record);
    }
    sdfReleaseResourceAllocation(((EffAnimationState *)state)->allocation);
}

void effSynchronizeFileTransform(u8 *work) {
    u32 *record = (u32 *)((EffBillFrameWork *)work)->frameState;

    if (((EffAnimationState *)record)->record != 0) {
        mnuRecordSetVector(((EffAnimationState *)record)->record, work);
        fileSetRecordSecondVector(((EffAnimationState *)record)->record, work + 0x10);
        dds3DispatchIndexedCallback(((EffAnimationState *)record)->record, ((EffClassWork *)work)->scale);
        fileAcquireRecord(((EffAnimationState *)record)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F2AE8);

u32 *effCreatePrimarySlotAnimationState(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = effClampSlotCount(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    ((EffAnimationState *)state)->record = fileAllocateGridRecordSlots(1, count, mapping);
    return state;
}

u32 *effCreateAlternateSlotAnimationState(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = effClampSlotCount(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    ((EffAnimationState *)state)->record = fileAllocateGridRecordSlots(3, count, mapping);
    return state;
}

void effResetSlotAnimationRecord(s32 work) {
    ((EffFrameAsset *)((EffFrameState *)((EffBillFrameWork *)work)->frameState)->asset)->frameCount = 0;
}

/* Quantized-texture parameters overlaid on the larger billboard configuration. */
typedef struct EffQuantizedConfig {
    u8 pad_00[0x74];
    union {
        u32 quantizedSamples; // 0x74, clamped to at least four
        s32 signedRows;
    } samples;
    u8 pad_78[0x10];
    f32 rowOffset;           // 0x88
} EffQuantizedConfig;

u32 *effAllocateQuantizedBuffer(u8 *work) {
    void *allocation = sdfAllocGeneralBlock(0xC);
    u32 *buffer = (u32 *)sdfResourceRetainAddress((u32)allocation);
    u32 count = ((EffQuantizedConfig *)work)->samples.quantizedSamples;

    buffer[2] = (u32)allocation;
    if (count < 4) {
        ((EffQuantizedConfig *)work)->samples.quantizedSamples = 4;
        count = 4;
    }
    buffer[0] = count >> 2;
    if ((((EffQuantizedConfig *)work)->samples.quantizedSamples & 3) != 0) {
        buffer[0] = (count >> 2) + 1;
    }
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F3258);

u32 *effPrepareQuantizedTexture(u8 *src) {
    u32 *buf = (u32 *)effAllocateQuantizedBuffer(src);
    buf[1] = effCreateTexturedStripWithSharedTexture(buf[0], ((EffQuantizedConfig *)src)->samples.quantizedSamples);
    func_002F3258(buf, src);
    return buf;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *p) {
    u8 *dst = ((EffBillFrameWork *)p)->config;
    u32 *src = (u32 *)((EffBillFrameWork *)p)->frameState;
    u32 *buf = (u32 *)effAllocateQuantizedBuffer(dst);
    buf[1] = effAllocateStripFromWorkAndRetainTexture(((EffFrameState *)src)->asset);
    func_002F3258(buf, dst);
    return buf;
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
    s32 rows = ((EffQuantizedConfig *)config)->samples.signedRows + 1;
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
                *v += ((EffQuantizedConfig *)config)->rowOffset;
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
    second = func_002D7458(config, config + 0x24, limit, progress);
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
    scale = func_002D7770(config + 0x34, limit, progress) * work->scale;
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

u8 *effAllocateBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9E60[kind].payloadSize;
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

u8 *effCreateResourceInstanceC(u16 kind, void *source) {
    u8 *work = effAllocateBlockWithModel(kind, source);
    ((EffClassWork *)work)->resource = (u32)D_003E9E60[kind].createResource(source);
    D_003E9E60[kind].initialize(work);
    return work;
}

void effResourceInstanceCreateFromFile(s32 work) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effCreateResourceInstanceC(((FileJob *)work)->option, source);
}

void effDispatchCleanupOp(u8 *work) {
    D_003E9E60[((EffClassWork *)work)->kind].destroyResource();
    sdfReleaseChipBlock(work);
}

u8 *effRecreateActiveByClass(u8 *obj) {
    u8 *work = effAllocateBlockWithModel(*(u16 *)(obj + 0x2C), ((EffClassWork *)obj)->payload);
    *(void **)(work + 0x30) = D_003E9E60[((EffClassWork *)obj)->kind].cloneResource(obj);
    D_003E9E60[((EffClassWork *)obj)->kind].initialize(work);
    return work;
}

void effResetModelBlockFrame(u8 *work) {
    D_003E9E60[((EffClassWork *)work)->kind].initialize();
    ((EffClassWork *)work)->frame = 0;
}

void effAdvanceModelBlockFrame(work)
s32 *work;
{
    if ((effModelUpdateControlFlags & 2) == 0) {
        D_003E9E60[work[0x2C / 4]].update();
        work[0x28 / 4]++;
    }
}

void effDrawModelBlock(s32 work) {
    D_003E9E60[((EffClassWork *)work)->kind].draw((void *)work);
}

void effUpdateAndDrawModelBlock(u32 work) {
    effAdvanceModelBlockFrame();
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
        effSharedScalyStripResource = effCloneSharedReferenceWithValue(effScalyTextureHandle, 0x200);
    }
    effSharedStripReferenceCount = effSharedStripReferenceCount + 1;
    return effSharedScalyStripResource;
}

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(u32 unused) {
    effSharedStripReferenceCount = effSharedStripReferenceCount - 1;
    if (effSharedStripReferenceCount == 0) {
        effReleaseSharedReference(effSharedScalyStripResource);
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
    func_003332D0(work->handle, 1.0f);
    memset(&D_00458400, 0, sizeof(EffMotionSetup));
    D_00458400.flags = 0x4000;
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
    sdfQueueAssetRelease(((EffStripWork *)work)->handle);
    sdfReleaseResourceAllocation(((EffStripWork *)work)->allocation);
}

/* Recreate the source strip's dimensions and retain another shared texture reference. */
void effAllocateStripFromWorkAndRetainTexture(u8 *work) {
    effAllocateTexturedStripWork(((EffStripWork *)work)->count, ((EffStripWork *)work)->repeat);
    effSharedStripReferenceCount++;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F3F70);

void effLoadScalyTexture(void) {
    D_00437E7C = sdfReadNamedResource("/effect/scaly00.tmx", &effScalyTextureHandle, 0);
}

u32 effGetScalyTextureHandle(void) {
    return effScalyTextureHandle;
}

typedef struct EffSpanEntry {
    f32 first;
    f32 second;
    u32 pad_08;
} EffSpanEntry;

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

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F46D8);

extern void effReleaseModelPointSetAsset(s32);

void effReleaseParticleList(u32 *list) {
    EffSpanRecord *entry = ((EffSpanTable *)list)->records;
    u32 i;

    for (i = 0; i < ((EffSpanTable *)list)->count; i++) {
        effReleaseModelPointSetAsset((s32)entry->pointSet);
        if (entry->references != 0) {
            effReleaseResourceRefs(entry->references);
        }
        entry++;
    }
    sdfReleaseResourceAllocation(((EffSpanTable *)list)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F4960);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F5168);


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
        effect->model = (void *)func_002DC1D0((u32)secondary, param);
        effect->attributes = param;
        effect->childResource = effModelResourceOperations[kind].createResource(effect->source, effect->model);
        effModelResourceOperations[kind].initialize(effect);
    }
    return (u32)effect;
}

u32 effCreateModelResourceFromFile(u8 *work) {
    void *first = fileResolvePrimaryBuffer();
    void *second = fileResolveSecondaryBuffer(work);
    return effCreateModelResourceWithInlineData(((FileJob *)work)->option, first, second, ((FileJob *)work)->slots[1].size);
}

void effDestroyModelResource(EffModelResource *effect) {
    effModelResourceOperations[effect->kind].destroyResource(effect->childResource);
    effDestroyModelContext((s32)effect->model);
    sdfReleaseChipBlock(effect);
}

EffModelResource *effCreateModelResource(EffModelCreateRequest *work) {
    EffModelResource *effect = (EffModelResource *)effCreateModelResourceWithInlineData(work->kind, work->source, 0, 0);
    u32 x = mdlGetContextResourceGroup(work->assetId);
    u32 y = mdlGetContextResourceId(work->assetId);
    void *model = func_00232198(x, y);

    effect->model = model;
    effInitModelVUState(model);
    effect->attributes = work->attributes;
    effect->childResource = effModelResourceOperations[effect->kind].createResource(effect->source, effect->model);
    effModelResourceOperations[effect->kind].initialize(effect);
    return effect;
}

void effResetModelResourceUpdateCount(u8 *work) {
    effModelResourceOperations[((EffModelResource *)work)->kind].initialize();
    ((EffModelResource *)work)->updateCount = 0;
}

void effDispatchModelResourceUpdate(work)
s32 *work;
{
    if ((effModelUpdateControlFlags & 2) == 0) {
        effModelResourceOperations[((EffModelResource *)work)->kind].update();
        ((EffModelResource *)work)->updateCount++;
    }
}

void effDispatchModelResourceCallback(s32 work) {
    effModelResourceOperations[((EffModelResource *)work)->kind].draw((void *)work);
}

void effStepModelResourceCallbacks(u32 work) {
    effDispatchModelResourceUpdate();
    effDispatchModelResourceCallback(work);
}

void effSetModelResourcePrimaryTransformVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetModelResourceSecondaryTransformVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetModelResourceColor(s32 work, u32 value) {
    ((EffModelResource *)work)->color = value;
}

void effSetModelResourceScale(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

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
    func_003332D0(set->handle, 1.0f);
    memset(D_00458430, 0, 0x2C);
    D_00458430[0].primitive = 0x4000;
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
            packet = sdfAllocPacketAligned(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfAppendPacket(list, packet);
        }
        D_003E9F38[set->type]->submit(D_003E9F38[set->type], list);
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

        effBattleMiscQueryPosition(handle, &request, vec);
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

        effBattleMiscQueryPosition(handle, &request, vec);
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

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F5EF0);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F6000);

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
            BtlUnitExt *child = entry->ext;
            if (child != 0) {
                child->color60 = entry->baseColor;
                evtSetUnitRgbTransition((EvtUnit *)child, 0, entry->baseColor);
            }
        }
        entry = entry->nextActor;
    }
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F64D8);

s64 effComputeLightDirectionVU(void *model, void *target) {
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
    func_0033A7E8(target, D_003E9F50, D_004584A0);
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

s32 *effAllocateCopiedEffectPayload(u32 owner, u32 source, s32 size) {
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
    return (s32 *)node;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F6A80);

extern void fldRelocatePackedTransferChunk(s32, s32);

extern void func_002F6A80(s32 *);

s32 *effCreateRelocatedEffectPayload(u32 owner, u32 unused, u32 source, u32 kind) {
    s32 *work = effAllocateCopiedEffectPayload(owner, source, kind);
    s32 object = *work;
    fldRelocatePackedTransferChunk(object, object + 8);
    func_002F6A80(work);
    return work;
}

s32 *effCloneEffectPayloadFromOwner(s32 owner) {
    s32 *work;

    work = effAllocateCopiedEffectPayload(((EffCopiedPayloadWork *)owner)->parameter, ((EffCopiedPayloadWork *)owner)->payload->body,
                                                ((EffCopiedPayloadWork *)owner)->payload->size);
    func_002F6A80(work);
    return work;
}

extern void dds3FreePathObject();

extern void dds3RemoveWorldObjectNode();

void effReleaseTargetSlots(u32 *obj) {
    u32 *tails = ((EffCopiedPayload *)obj)->effects;
    u32 *heads = ((EffCopiedPayload *)obj)->targets;
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
    sdfReleaseResourceAllocation(((EffCopiedPayload *)obj)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F6D00);


typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} EffAnimInfo;

/* Header common to the resource-instance constructors and callback dispatchers. */
typedef struct EffActiveResource {
    u8 pad_00[0x20];
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
} EffActiveResource;


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
                if (!(actor[i]->flags & 0x20) && !(actor[i]->unk330 & 0x10)) {
                    if (actor[i]->unk330 & 0x40) {
                        switch (info->id) {
                        case 0:
                        case 2:
                        case 0xa:
                            continue;
                        }
                    }
                    if (mdlGetNodeRefHalf((MdlCtx *)actor[i]->ext->info, 0) > info->id) {
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

extern void func_001E95C8(s32, f32);

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
        func_001E95C8(owner + 0x70, value);
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
    *work = effCreateClassResourceWork(4, owner);
    return work;
}

u32 *effCreatePayloadPointerWorkFromRequest(u8 *request) {
    u32 *source = (u32 *)((EffActiveResource *)request)->resource;
    u32 *work = effAllocateClassResourceSlot((u32)((EffActiveResource *)request)->payload);
    *work = effPayloadPointerGet(*source);
    return work;
}

void effReleaseOwnedClassResourceWork(u32 handle) {
    if (*(s32 *)handle != 0) {
        effDestroyClassResourceWork(*(s32 *)handle);
    }
    sdfReleaseChipBlock(handle);
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
    effAdvanceClassResourceFrame(handle[0]);
}

void effDrawActiveClassResource(s32 owner) {
    effDrawClassResourceWork(*(u32 *)((EffActiveResource *)owner)->resource);
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
    sdfReleaseChipBlock(handle);
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

u32 *effAllocateModelObjectSlot(u32 owner) {
    u32 *work = (u32 *)sdfAllocSizeClassBlock(4);
    *work = 0;
    return work;
}


extern void mdlAddEntryFlagged(s32, u32, u32);

u32 *effCreateAndAttachModelEffectObject(u32 *owner, u32 kind, u32 source, u32 settings) {
    u32 *work = effAllocateModelObjectSlot((u32)owner);
    s32 object = func_002DC1D0(source, settings);
    s32 active = (s32)((EffModelContextView *)object)->parameters;
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

u32 *effCreateAndAttachModelFromResourceDescriptor(u8 *request) {
    u32 *owner = ((EffActiveResource *)request)->payload;
    void **source = (void **)((EffActiveResource *)request)->resource;
    u32 *work = effAllocateModelObjectSlot((u32)owner);
    s32 a = mdlGetContextResourceGroup(*source);
    s32 b = mdlGetContextResourceId(*source);
    void *object = func_00232198(a, b);
    *work = (u32)object;
    effInitModelVUState(object);
    if (((EffModelContextView *)*work)->parameters != NULL) {
        if (*owner != 0) {
            mdlAddEntryPlain(*work, 0, 0);
        } else {
            mdlAddEntryFlagged(*work, 0, 0);
        }
    }
    return work;
}

void effReleaseOwnedModelContextWork(u32 handle) {
    if (*(s32 *)handle != 0) {
        effDestroyModelContext(*(s32 *)handle);
    }
    sdfReleaseChipBlock(handle);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F8040);

s32 *func_002F81A8(s32 *owner) {
    s32 *work = (s32 *)sdfAllocSizeClassBlock(8);
    work[0] = 0;
    work[1] = 0;
    return work;
}

s32 *effCreateMaterialAndModelEffectWork(s32 *owner, u32 kind, u32 source, u32 settings) {
    s32 *work = func_002F81A8(owner);
    s32 object;
    s32 active;

    work[0] = effCreateClassResourceWork(4, (u32)owner);
    object = func_002DC1D0(source, settings);
    active = (s32)((EffModelContextView *)object)->parameters;
    work[1] = object;
    if (active != 0) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(object, 0, 0);
        } else {
            mdlAddEntryFlagged(object, 0, 0);
        }
    }
    return work;
}

s32 *effCreateModelEffectWorkFromPayload(u8 *request) {
    s32 *owner = ((EffActiveResource *)request)->payload;
    u32 *source = (u32 *)((EffActiveResource *)request)->resource;
    s32 *work = func_002F81A8(owner);
    s32 a;
    s32 b;
    void *object;
    s32 material;
    u32 modelSource;

    material = effPayloadPointerGet(source[0]);
    modelSource = source[1];
    work[0] = material;
    a = mdlGetContextResourceGroup((void *)modelSource);
    b = mdlGetContextResourceId((void *)source[1]);
    object = func_00232198(a, b);
    work[1] = (s32)object;
    effInitModelVUState(object);
    if (((EffModelContextView *)work[1])->parameters != NULL) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(work[1], 0, 0);
        } else {
            mdlAddEntryFlagged(work[1], 0, 0);
        }
    }
    return work;
}

extern void effDestroyClassResourceWork(s32);

void effDestroyMaterialAndModelEffectWork(s32 *work) {
    if (work[1] != 0) {
        effDestroyModelContext(work[1]);
    }
    if (work[0] != 0) {
        effDestroyClassResourceWork(work[0]);
    }
    sdfReleaseChipBlock(work);
}

/* vu0 routine: orient along target minus model origin, save distance, and advance the resource. */
void effOrientClassResourceAlongTargetOffset(u8 *work) {
    u8 *object = ((EffActiveResource *)work)->payload;
    s32 *handle = (s32 *)((EffActiveResource *)work)->resource;
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
    sdfLoadMapRecordLookAtBasis(((EffModelContextView *)handle[1])->lookAtBasis, 0);
    VU0_STORE_VF_UNCLOBBERED(vf31, origin);
    effCopyClassResourcePosition((s128 *)handle[0], (s128 *)target);
    state = ((EffClassWork *)handle[0])->payload;
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
    effCopyClassResourceOrientation((s128 *)handle[0], (s128 *)look);
    effAdvanceClassResourceFrame(handle[0]);
}

extern void mdlStorePrimaryVectorVU(void *);

extern void mdlUpdateContextRotationBasisFromQuaternion(void *);

/* The model helpers read vf10, following the SDK's VU0 macro-mode convention. */
void effApplyModelTransform(u8 *work) {
    u8 *modelContext = (u8 *)((EffActiveResource *)work)->resource;
    u8 *animation = ((EffActiveResource *)work)->payload;
    u32 bits;
    float scale;
    VU0_LOAD_VF_MEMORY(vf10, work);
    mdlStorePrimaryVectorVU(((EffModelBindings *)modelContext)->model);
    VU0_LOAD_VF_MEMORY(vf10, work + 0x10);
    mdlUpdateContextRotationBasisFromQuaternion(((EffModelBindings *)modelContext)->model);
    VU0_SET_ONES_XYZ(vf10);
    scale = ((EffActiveResource *)work)->scale;
    VU0_SCALAR_OP_TMP_MEMORY(bits, scale, "vmulx.xyzw vf10, vf10, vf2x");
    mdlStoreTertiaryVectorVU(((EffModelBindings *)modelContext)->model);
    ((EffModelContextView *)((EffModelBindings *)modelContext)->model)->parameters->value =
        ((EffAimConfig *)animation)->modelParameter;
    mdlProcessContextNodesAndTransforms(((EffModelBindings *)modelContext)->model, D_00380828);
    effDrawClassResourceWork(*(s32 *)modelContext);
}

extern s32 *sdfCreateAssetWithDrawEntries(void);

s32 *effCreateDrawableAssetWithDefaultOpacity() {
    s32 *work = sdfAllocAndClearQuadwords(0xC);
    s32 *position;
    work[2] = 0;
    position = sdfCreateAssetWithDrawEntries();
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
    sdfReleaseChipBlock(work);
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
                BtlUnitExt *target = node->ext;

                if (target != 0) {
                    node->overlayColor = (node->overlayColor & 0xFFFFFF) | (node->baseColor & 0xFF000000);
                    target->color60 = node->baseColor;
                    evtSetUnitAlphaTransition((EvtUnit *)target, 0, node->baseColor);
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
                    evtSetUnitAlphaTransition((EvtUnit *)actors[i]->ext, config->fadeIn, color);
                }
            }
        }
        if (config->duration != 0 && frame == config->duration - config->fadeOut) {
            for (i = 0; i < count; i++) {
                if (actors[i]->flags & 2) {
                    evtSetUnitAlphaTransition((EvtUnit *)actors[i]->ext, config->fadeOut, actors[i]->baseColor);
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
    memset(&D_004584C0, 0, sizeof(EffMotionSetup));
    D_004584C0.flags = 0x4000;
    D_004584C0.table = D_003E9FF0;
    D_004584C0.mode = 4;
    D_004584C0.kind = 6;
    return work;
}

s32 *effBillboardMotionResourceCreate(s32 *context, u16 kind, s32 *source) {
    s32 *resource = effCreateMotionResource(context);

    switch (kind) {
    case 1:
        resource[0] = billCreateIndexed(0, source);
        break;
    case 2:
        resource[0] = billCreateIndexed(1, source);
        break;
    case 4:
        resource[0] = effRetainResource(source[0]);
        break;
    }
    billMarkKindOneFlag(resource[0]);
    billSetBillboardMode(resource[0], ((EffMotionResourceConfig *)context)->mode);
    return resource;
}

s32 *effBillboardMotionResourceInitialize(s32 *request) {
    s32 *source = (s32 *)request[0x30 / 4];
    s32 *context = (s32 *)request[0x38 / 4];
    s32 *resource = effCreateMotionResource(context);
    resource[0] = billCloneObjectRetainingSharedData(*source);
    billMarkKindOneFlag(resource[0]);
    billSetBillboardMode(resource[0], ((EffMotionResourceConfig *)context)->mode);
    return resource;
}

void effDestroyBillboardAndOwnedAssetWork(s32 *work) {
    if (work[0] != 0) {
        billDispatchByKind(work[0]);
    }
    if (work[1] != 0) {
        sdfQueueAssetRelease(work[1]);
    }
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F91D0);


u8 *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003EA018[kind].payloadSize;
    u8 *effect = sdfAllocSizeClassBlock(size + headerSize);
    ((EffActiveResource *)effect)->payload = effect + headerSize;
    ((EffActiveResource *)effect)->color = 0x80808080;
    ((EffActiveResource *)effect)->scale = 1.0f;
    ((EffActiveResource *)effect)->kind.index = kind;
    ((EffActiveResource *)effect)->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, effect + 0x10);
    memcpy(((EffActiveResource *)effect)->payload, source, size);
    return effect;
}

u8 *func_002F9608(u16 kind, void *source, u16 secondaryKind, s32 secondary, u32 param) {
    u8 *effect = effAllocateResourcePayload(kind, source);

    if (btlIsRuntimeAllocated() != 0) {
        if (D_003EA018[kind].createResource != NULL) {
            ((EffActiveResource *)effect)->resource = (u32)D_003EA018[kind].createResource(source, secondaryKind, secondary, param);
        }
        if (D_003EA018[kind].initialize != NULL) {
            D_003EA018[kind].initialize(effect);
        }
    }
    return effect;
}

void effCreateActiveResourceFromFile(s32 *source) {
    void *primary = fileResolvePrimaryBuffer();
    s32 secondary = fileResolveSecondaryBuffer(source);
    func_002F9608(((FileJob *)source)->option, primary,
                  ((FileJob *)source)->slots[0].selector, secondary, ((FileJob *)source)->slots[1].size);
}

void effDestroyResourceInstance(u32 *obj) {
    if (btlIsRuntimeAllocated()) {
        if (D_003EA018[obj[0x2C / 4]].destroyResource != NULL) {
            D_003EA018[obj[0x2C / 4]].destroyResource(obj[0x30 / 4]);
        }
    }
    sdfReleaseChipBlock(obj);
}

u8 *effDuplicateActiveResource(u8 *source) {
    u8 *effect;
    u32 kind = ((EffActiveResource *)source)->kind.index;

    if (D_003EA018[kind].cloneResource == NULL) {
        effect = func_002F9608(((EffActiveResource *)source)->kind.shortIndex, ((EffActiveResource *)source)->payload, 0, 0, 0);
    } else {
        u32 resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(((EffActiveResource *)source)->kind.shortIndex, ((EffActiveResource *)source)->payload);
        resource = (u32)D_003EA018[((EffActiveResource *)source)->kind.signedIndex].cloneResource(source);
        activeKind = ((EffActiveResource *)source)->kind.index;
        ((EffActiveResource *)effect)->resource = resource;
        if (D_003EA018[activeKind].initialize != NULL) {
            D_003EA018[activeKind].initialize(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(u32 *obj) {
    if (btlIsRuntimeAllocated()) {
        if (D_003EA018[obj[0x2C / 4]].initialize != NULL) {
            D_003EA018[obj[0x2C / 4]].initialize(obj);
        }
        obj[0x28 / 4] = 0;
    }
}

void effAdvanceCallbackFrame(work)
u8 *work;
{
    if (btlIsRuntimeAllocated() != 0 && (effModelUpdateControlFlags & 2) == 0) {
        s32 kind = ((EffActiveResource *)work)->kind.signedIndex;
        EffResourceOps *entry = &D_003EA018[kind];
        void (*callback)(void *) = entry->update;
        if (callback != NULL) {
            callback(work);
        }
        ((EffActiveResource *)work)->frame++;
    }
}

void effDispatchEnabledCallback(u8 *work) {
    if (btlIsRuntimeAllocated() != 0) {
        void (*callback)(void *) = D_003EA018[((EffActiveResource *)work)->kind.signedIndex].draw;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void effAdvanceActiveResourceCallbacks(u32 work) {
    effAdvanceCallbackFrame();
    effDispatchEnabledCallback(work);
}

void effCopyActiveResourceVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effCopyActiveResourceSecondaryVector(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void effSetActiveResourceColor(s32 work, u32 color) {
    ((EffActiveResource *)work)->color = color;
}

void func_002F99E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002F99E8);

/* Alternate particle handles share a header but occupy distinct slots. */
typedef struct EffParticleShared {
    u8 pad00[8];
    u32 state;
    u8 pad0C[0x98];
    u32 billHandle;       // 0xA4
    RefObj *reference;    // 0xA8
} EffParticleShared;

void effReleaseParticleResources(u32 *p) {
    if (p[0xA4 / 4] != 0) {
        billDispatchByKind(p[0xA4 / 4]);
    }
    if (p[0xA8 / 4] != 0) {
        effReleaseReferenceHolder(p[0xA8 / 4]);
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
    u32 handle;

    if (((EffParticleShared *)src)->billHandle != 0) {
        if (((EffParticleShared *)dst)->billHandle != 0) {
            billDispatchByKind(((EffParticleShared *)dst)->billHandle);
        }
        handle = billCloneObjectRetainingSharedData(((EffParticleShared *)src)->billHandle);
        ((EffParticleShared *)dst)->billHandle = handle;
        billMarkKindOneFlag(handle);
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
    u32 model;              // 0x60
    void *deviceSlot;       // 0x64
    u32 resourceEntries;    // 0x68
    u32 entryAllocation;    // 0x6C
    u32 record;             // 0x70
    u8 *positions;          // 0x74
    u32 positionAllocation; // 0x78
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

extern void effReplaceEffectSlotModelAndDeviceResources(EffectSlotNode80 *, u32, u32);

extern void effRebuildResourceEntryClones(EffectSlotNode80 *, s32);

s32 *func_002FA5F0(u8 *request) {
    u8 *buffer = fileResolvePrimaryBuffer(request);
    s32 *size = (s32 *)(buffer + 0x50);
    s32 *work = func_002FA5B8(size);
    s32 secondary;
    u32 kind;

    memcpy(&((EffectSlotNode80 *)work)->randomDirection, buffer, 0x50);
    effRebuildResourceEntries((u8 *)work, ((FileJob *)request)->option, size);
    secondary = fileResolveSecondaryBuffer(request);
    if (secondary != 0) {
        kind = ((FileJob *)request)->slots[0].selector;
        switch (kind) {
        case 3:
            effReplaceEffectSlotModelAndDeviceResources((EffectSlotNode80 *)work, secondary, ((FileJob *)request)->slots[1].size);
            kind = ((FileJob *)request)->slots[0].selector;
            break;
        case 6:
            effRebuildResourceEntryClones(work, secondary);
            kind = ((FileJob *)request)->slots[0].selector;
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
        fileReleaseGridRecordHandle(((EffectSlotNode80 *)work)->record);
    }
    if (((EffectSlotNode80 *)work)->positionAllocation != 0) {
        sdfReleaseResourceAllocation(((EffectSlotNode80 *)work)->positionAllocation);
    }
    sdfReleaseChipBlock(work);
}

extern s32 *func_002FA5B8(s32 *);

s32 *effCloneOwnedState(u8 *owner) {
    s32 *source = (s32 *)((EffGridRecord *)((EffectSlotNode80 *)owner)->record)->params;
    s32 *work = func_002FA5B8(source);
    memcpy(&((EffectSlotNode80 *)work)->randomDirection,
        &((EffectSlotNode80 *)owner)->randomDirection, 0x50);
    effRebuildResourceEntries((u8 *)work, ((EffGridRecord *)((EffectSlotNode80 *)owner)->record)->kind, source);
    func_002FA978(work, owner);
    return work;
}

void func_002FA978(EffectSlotNode80 *dst, EffectSlotNode80 *src) {
    s32 kind = src->resourceKind;
    s32 model;
    EffModelAssetData *modelData;
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
        model = (s32)effCloneModelWithVUState((void *)src->model);
        modelData = ((EffModelAssetHeader *)model)->data;
        dst->model = model;
        dst->deviceSlot = sdfModelCreateWithAlternateItems(modelData->unk14, modelData->unk18);
        kind = src->resourceKind;
        break;
    case 6:
        count = ((EffGridRecord *)src->record)->count;
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
        dst->entryAllocation = (u32)sdfAllocGeneralBlock(count * 4);
        dst->resourceEntries = sdfResourceRetainAddress(dst->entryAllocation);
        for (i = 0; i < count; i++) {
            ((void **)dst->resourceEntries)[i] = fileQueueClone(((void **)src->resourceEntries)[0]);
        }
        kind = src->resourceKind;
        break;
    }
    dst->resourceKind = kind;
}

extern s32 fileAllocateGridRecordSlots(u16, s32, s32);

void effRebuildResourceEntries(u8 *work, u32 kind, s32 *config) {
    s32 previous = ((EffectSlotNode80 *)work)->record;
    s32 allocation;
    u32 count;
    u32 i;
    u8 *entries;
    if (previous != 0) {
        fileReleaseGridRecordHandle(previous);
    }
    ((EffectSlotNode80 *)work)->record = fileAllocateGridRecordSlots(kind, *(s32 *)work, (s32)config);
    allocation = ((EffectSlotNode80 *)work)->positionAllocation;
    if (allocation != 0) {
        sdfReleaseResourceAllocation(allocation);
    }
    count = ((EffGridRecord *)((EffectSlotNode80 *)work)->record)->count;
    ((EffectSlotNode80 *)work)->positionAllocation = sdfAllocGeneralBlock(count * 0x18);
    ((EffectSlotNode80 *)work)->positions = (u8 *)sdfResourceRetainAddress(((EffectSlotNode80 *)work)->positionAllocation);
    entries = ((EffectSlotNode80 *)work)->positions;
    for (i = 0; i < count; i++, entries += 0x18) {
        effInitializeParticleDirection(work, (float *)entries);
    }
}

void effReplaceEffectSlotModelAndDeviceResources(EffectSlotNode80 *work, u32 kind, u32 config) {
    EffModelAssetData *modelData;
    s32 model;
    void *deviceSlot;

    if (work->model != 0) {
        effDestroyModelContext(work->model);
        work->model = 0;
    }
    if (work->deviceSlot != 0) {
        sdfReleaseDevSlot(work->deviceSlot, 1, 1);
        work->deviceSlot = 0;
    }
    model = func_002DC1D0(kind, config);
    /* Keep the header load raw: it must precede the parent model-pointer store. */
    modelData = (EffModelAssetData *)*(s32 *)(model + 0xc);
    work->model = model;
    deviceSlot = sdfModelCreateWithAlternateItems(modelData->unk14, modelData->unk18);
    work->deviceSlot = deviceSlot;
}

/* Rebuilds the surface job queue of one record bucket and clones the first job. */
void effRebuildResourceEntryClones(EffectSlotNode80 *obj, s32 secondary) {
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
        obj->entryAllocation = (u32)sdfAllocGeneralBlock(size);
        obj->resourceEntries = sdfResourceRetainAddress(obj->entryAllocation);
        ((void **)obj->resourceEntries)[0] = fileCloneQueueEntries((FileQueue *)secondary);
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
    if (effModelUpdateControlFlags & 2) {
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

typedef struct MdlResourceItem MdlResourceItem;
typedef struct SdfTextParam SdfTextParam;

extern void *sdfChunkFindRecordById(SdfTextParam *, s32);
extern void mdlSetResourceAmount(s32, MdlResourceItem *, f32);
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

typedef struct EffSharedEffectWork {
    u8 pad00[0x20];
    u32 unk20;
    f32 unk24;
    u32 unk28;
    EffectBlob data;       // 0x2C through 0x477
    u32 resource;          // 0x478, shared resource whose reference count is at +0x10
    u32 allocation;        // 0x47C
} EffSharedEffectWork;

void func_002FB480(EffSharedEffectWork *work) {
    f32 *amount;
    f32 scale;
    u32 i;
    void *record;
    u8 *item;

    work->unk28 = 0;
    if (*(u32 *)work->resource == 0) {
        return;
    }
    if (*(u32 *)(*(u32 *)work->resource + 0x1C) != 0) {
        if (work->data.flagged != 0) {
            mdlAddEntryFlagged(*(s32 *)work->resource, 0, work->data.entryId);
        } else {
            mdlAddEntryPlain(*(s32 *)work->resource, 0, work->data.entryId);
        }
        *(f32 *)(*(u32 *)(*(u32 *)work->resource + 0x1C) + 0x20) = 1.0f;
    }

    scale = work->data.scale;
    i = 0;
    amount = work->data.amounts;
    for (; i < 0xFF; i++, amount++) {
        record = sdfChunkFindRecordById(
            (SdfTextParam *)*(u32 *)(*(u32 *)work->resource + 0x18), i);
        for (item = (u8 *)*(u32 *)(*(u32 *)work->resource + 0x14); item != NULL;
             item = *(u8 **)item) {
            if (*(u32 *)(item + 0x10) == (u32)record) {
                mdlSetResourceAmount(*(s32 *)work->resource, (MdlResourceItem *)item, *amount * scale);
                break;
            }
        }
    }
    mdlSetAllResourceFrames((MdlCtx *)*(u32 *)work->resource, work->data.frame);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FB5C0);


typedef struct EffSharedEffectResource {
    u32 model;
    u32 deviceSlot;
    u32 data;
    u8 pad0C[4];
    u16 references;
    u8 pad12[0xA];
    u32 allocation;
} EffSharedEffectResource;


/* Release this work; only the final reference releases the shared backing resources. */
void effReleaseSharedResourceReference(s32 *work) {
    s32 *resource = (s32 *)((EffSharedEffectWork *)work)->resource;
    u32 references = ((EffSharedEffectResource *)resource)->references + 0xFFFF;
    ((EffSharedEffectResource *)resource)->references = references;
    if ((u16)references == 0) {
        s32 data = ((EffSharedEffectResource *)resource)->data;
        if (data != 0) {
            sdfReleaseChipBlock(data);
        }
        resource = (s32 *)((EffSharedEffectWork *)work)->resource;
        if (((EffSharedEffectResource *)resource)->model != 0) {
            effDestroyModelContext(((EffSharedEffectResource *)resource)->model);
        }
        resource = (s32 *)((EffSharedEffectWork *)work)->resource;
        if (((EffSharedEffectResource *)resource)->deviceSlot != 0) {
            sdfReleaseDevSlot(((EffSharedEffectResource *)resource)->deviceSlot, 1, 1);
        }
        resource = (s32 *)((EffSharedEffectWork *)work)->resource;
        if (((EffSharedEffectResource *)resource)->allocation != 0) {
            sdfReleaseResourceAllocation(((EffSharedEffectResource *)resource)->allocation);
        }
        sdfReleaseChipBlock(((EffSharedEffectWork *)work)->resource);
    }
    sdfReleaseResourceAllocation(((EffSharedEffectWork *)work)->allocation);
}


extern u8 *func_002FB5C0(s32);

s32 effCloneEffectRequest(u8 *src) {
    u8 *dst = func_002FB5C0(0);

    ((EffSharedEffectWork *)dst)->data = ((EffSharedEffectWork *)src)->data;
    effShareReferenceCountedEffectObject((s32)dst, (s32)src);
    return (s32)dst;
}

void effShareReferenceCountedEffectObject(s32 target, s32 source) {
    ((EffSharedEffectWork *)target)->resource = ((EffSharedEffectWork *)source)->resource;
    ((EffSharedEffectResource *)((EffSharedEffectWork *)source)->resource)->references =
        (s16)((EffSharedEffectResource *)((EffSharedEffectWork *)source)->resource)->references + 1;
}

void func_002FB968(s32 *work) {
    s32 *context = *(s32 **)work[0x478 / 4];
    if (context != NULL) {
        sdfMotionSampleAtFrame(context[0x1C / 4], 0.0f);
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

/* A 20-byte creation command, not the runtime FileJob queue entry. */
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

s32 effLoadFileJobPayload(EffFileJobRequest *descriptor, s32 source) {
    s32 job;

    if (source != 0) {
        void *sourceBuffer;
        job = fileJobCreateFromCommandState(source);
        sourceBuffer = fileResolvePrimaryBuffer((void *)job);
        memcpy(descriptor->output, sourceBuffer, descriptor->size);
    } else {
        job = fileCreateJob(descriptor->fileKind);
        if (descriptor->output != NULL) {
            fileJobSetPrimaryData(job, (s32)descriptor->output,
                          (s32)descriptor->size, descriptor->transferMode);
        }
        if (descriptor->relatedResource != 0) {
            fileJobCopyCommandIntoSecondaryData(job, descriptor->relatedResource,
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
        fileJobDestroy(effTemporaryFileJob);
        effTemporaryFileJob = 0;
    }
    fileJobDestroy(job);
}

extern char D_0042CF58[];

extern char D_0042CF70[];

extern char D_004386D0[];

extern char D_004386D8[];

extern void fileWriteToPfs(u32, char *);

extern s32 func_0035C860(char *, char *, ...);

extern void func_002D50D8(u32, char *);

extern void func_002D55B0(u32, char *);

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

typedef struct EffAssetQuery {
    u8 pad00[0x90];
    u8 *request;
} EffAssetQuery;

typedef struct EffQueuedFileObject {
    u8 pad00[0x34];
    u8 *linkedState;
} EffQueuedFileObject;

u8 *effFindAssetData(u8 *work) {
    u8 *requested = ((EffAssetQuery *)work)->request;
    u16 type = ((EffAssetRequest *)requested)->type;
    u16 id = ((EffAssetRequest *)requested)->id;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = D_003FF128[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (((EffAssetIdentifier *)asset)->type != 6) {
                        if (((EffAssetIdentifier *)asset)->type == type && ((EffAssetIdentifier *)asset)->id == id) {
                            return asset;
                        }
                    } else if (type == 6) {
                        s32 index = ((EffAssetIdentifier *)asset)->id;
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

u32 effFindAssetObject(u8 *work) {
    u8 *requested = ((EffAssetQuery *)work)->request;
    u16 type = ((EffAssetRequest *)requested)->type;
    u16 id = ((EffAssetRequest *)requested)->id;
    u16 i;
    for (i = 0; i < 24; i++) {
        EffectAssetLink *links = D_003FF128[i];
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
    entry = fileAppendJob((FileQueue *)effFileQueue, effLoadFileJobPayload(request->resource, request->existingJob));
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
        fileJobDestroy(effTemporaryFileJob);
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

extern void func_002FE5B8(const char *, const void *, s32);

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

s32 effPollPartResource(void) {
    u8 request[0x110];
    s32 state;
    s32 result = 0x400001;
    s32 entry;

    effPollResourceBankSlot(D_0042CF58, 0x20, request);
    state = ((EffResourceBankSlot *)request)->state;
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (effFileQueue != 0) {
            entry = fileAppendJobFromEntry(effFileQueue, request);
            strcpy((char *)(entry + 0x9C), *(char **)effFindAssetData(entry));
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

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D030);

INCLUDE_RODATA(const s32, "game/code_002DE248", D_0042D048);

INCLUDE_ASM(const s32, "game/code_002DE248", func_002FEDD0);

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
            fileJobSetSecondaryData(effQueuedFileHandle, status + 0xD0, 4, 4);
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
            fileJobCopyCommandIntoSecondaryData(effQueuedFileHandle, status,
                           effClassifyResourceMask(((EffResourceBankSlot *)status)->type));
        } else {
            fileJobSetSecondaryData(effQueuedFileHandle, status + 0x104, 4, 4);
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
    fileJobSetSecondaryData(effQueuedFileHandle, &zero, 4, 4);
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

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;

extern EffectFileHeader D_003FB948;

extern EffectFileHeader D_003F01D0;

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    u32 allocationHandle;
} EffFileResourceRecord;


u32 fileLoadEffectSlotA(void) {
    u8 fileInfo[0x110];
    u8 *job;
    u8 *entry;
    u8 *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_0042D128, 4, fileInfo);
    status = ((EffResourceBankSlot *)fileInfo)->state;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(3);
        fileJobSetPrimaryData(job, D_003F01D0.start, D_003F01D0.length,
                      D_003F01D0.mode);
        fileJobCopyCommandIntoSecondaryData(job, fileInfo, effClassifyResourceMask(((EffResourceBankSlot *)fileInfo)->type));
        entry = (u8 *)fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (s32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = ((FileJob *)entry)->id;
        resource = (u8 *)effFindAssetData(entry);
        strcpy(((FileJob *)entry)->name, ((EffFileResourceRecord *)resource)->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(((EffFileResourceRecord *)resource)->buffer, fileData,
               ((EffFileResourceRecord *)resource)->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = (u8 *)D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
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
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    u8 *buffer;
    u32 command;
    u32 totalLength;
    u32 dataLength;
    u32 allocation;

    effPollResourceBankSlot(D_0042D140, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        u32 headerBytes = 0x80;
        u32 oldAllocation;
        u32 queuedFile;
        job = (u8 *)fileCreateJob(6);
        command = sdfDevCreateCommandState(&fileInfo);
        dataLength = sdfDevQueueControlAndWait(command);
        totalLength = dataLength + headerBytes;
        allocation = sdfAllocGeneralBlock(totalLength);
        buffer = (u8 *)sdfResourceRetainAddress(allocation);
        memset(buffer, 0, headerBytes);
        sdfDevQueueReadAndWait(command, buffer + headerBytes, dataLength);
        sdfDevWaitThenReleaseCommandState(command);
        fileJobSetPrimaryData(job, buffer, totalLength, 1);
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (u32)entry;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        memcpy(D_0045C270, entry, 0x80);
        queuedFile = entry->id;
        oldAllocation = resource->allocationHandle;
        effQueuedFileHandle = queuedFile;
        if (oldAllocation != 0) {
            sdfReleaseResourceAllocation(oldAllocation);
        }
        resource->allocationHandle = allocation;
        resource->buffer = buffer;
        resource->size = dataLength + headerBytes;
        resource->mode = 1;
        memcpy(D_00459E30, D_003F0DA8, 0x74);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003F9060;

u32 effPollAndQueueCopiedFileResource(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_0042D128, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x12);
        fileJobSetPrimaryData(job, D_003F9060.start, D_003F9060.length,
                      D_003F9060.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
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
        fileJobSetPrimaryData(job, D_003FB948.start, D_003FB948.length,
                      D_003FB948.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003FD988;

u32 effLoadMaterialFile(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    effPollResourceBankSlot(D_0042D128, 2, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x16);
        fileJobSetPrimaryData(job, D_003FD988.start, D_003FD988.length,
                      D_003FD988.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
        effResetFileResourceManager();
        if (effTemporaryFileJob != 0) {
            fileJobDestroy(effTemporaryFileJob);
            effTemporaryFileJob = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003FE040;

u32 effPollAndQueueFileResourceWithUnitFloats(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    FileJob *entry;
    EffFileResourceRecord *resource;
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
        job = (u8 *)fileCreateJob(0x19);
        coordinates = (f32 *)(D_003FE040.start + 0x20);
        for (index = 0; index < 0xFF; index++) {
            *coordinates++ = 1.0f;
        }
        fileJobSetPrimaryData(job, D_003FE040.start, D_003FE040.length,
                      D_003FE040.mode);
        fileJobCopyCommandIntoSecondaryData(job, &fileInfo, effClassifyResourceMask(fileInfo.resourceMask));
        entry = fileAppendJob(effFileQueue, job);
        effCurrentFileQueueEntry = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        effQueuedFileHandle = entry->id;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->name, resource->name);
        fileData = fileResolvePrimaryBuffer(effQueuedFileHandle);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        effQueuedFileObject = effFindAssetObject(entry);
        ((EffQueuedFileObject *)effQueuedFileObject)->linkedState = D_003FFA78;
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
    D_0043876C = 0;
    D_00438774 = 1;
    D_00438770 = 0;
    D_00438778 = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

s32 effClassifyResourceMask(s32 mask) {
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

typedef struct MnuValueRecord {
    u32 value;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u8 pad10[4];
} MnuValueRecord;

void *mnuAllocateValueRecord(u32 value) {
    u8 *record = (u8 *)sdfAllocSizeClassBlock(0x14);
    memset(record, 0, 0x14);
    ((MnuValueRecord *)record)->value = value;
    ((MnuValueRecord *)record)->unk04 = 0;
    ((MnuValueRecord *)record)->unk08 = 0;
    ((MnuValueRecord *)record)->unk0C = 0;
    return record;
}

void func_00303D58(void) {
    sdfReleaseChipBlock();
}

u32 mnuGetValueRecordOwner(u32 *value) {
    return *value;
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

typedef struct EffectListNode {
    u32 state;
    struct EffectListNode *next;
    u32 value;
    u32 length;
    u32 kind;
    void **reference; // 0x14: where the loaded resource goes
} EffectListNode;

typedef struct EffectList {
    u32 mode;
    s32 count;
    EffectListNode *first;
    EffectListNode *last;
    EffRequest *request;
} EffectList;

extern s32 fileRequestIsReady(void *);

extern void func_002C7CE8(void *);

extern void *fileQueuePlainDispatchRequest(const char *path);

extern void func_002C81D0(void *);

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
                list->request = fileQueuePlainDispatchRequest(node->length);
                if (list->mode == 2) {
                    func_002C81D0(list->request);
                }
                node->state = 1;
            } else if (fileRequestIsReady(list->request) != 0) {
                for (item = list->request->items; item != NULL; item = item->next) {
                    if (item->kind == 1) {
                        node = list->first;
                        buffer = item->buffer;
                        *node->reference = (void *)func_00305148(buffer, node->kind);
                        if (node->kind == 0) {
                            sdfReleaseResourceAllocation(buffer);
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
#define EFF_PHASE_START_ZERO_BIT 1
#define EFF_PAYLOAD_RECORD_BYTES 0x6C
#define EFF_RESOURCE_TABLE_ENTRY_BYTES 8

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
u32 effLoadIndexedResource(s32 category, s32 index, s32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    u32 allocation;
    u32 instance;
    func_0035C860(path, D_004387E8, category, index);
    allocation = sdfReadNamedResource(path, &sourceAddress, 0);
    instance = func_00305148(allocation, keepAllocation);
    if (keepAllocation == EFF_RESOURCE_TRANSIENT) {
        sdfReleaseResourceAllocation(allocation);
    }
    return instance;
}

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(u64 job, u32 *outInstance) {
    u64 allocation;
    u32 instance;

    allocation = fileGetResourceHandle();
    instance = func_00305148(allocation, EFF_RESOURCE_TRANSIENT);
    *outInstance = instance;
    sdfReleaseResourceAllocation(allocation);
    filePollEntryCleanup(job);
}

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(u64 job, u32 *outInstance) {
    u64 allocation;
    u32 instance;

    allocation = fileGetResourceHandle();
    instance = func_00305148(allocation, EFF_RESOURCE_KEEP_ALLOCATION);
    *outInstance = instance;
    filePollEntryCleanup(job);
}

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(s32 category, s32 index, s32 mode, u32 *outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];
    func_0035C860(path, D_004387E8, category, index);
    *outInstance = 0;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        fileCreateCallbackRequest(path, 0, effCompleteRetainedResourceJob, outInstance);
    } else {
        fileCreateCallbackRequest(path, 0, effCompleteTransientResourceJob, outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
u32 effLoadMappedResource(s32 category, s32 index) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    u32 allocation;
    u32 mappedResource;
    func_0035C860(path, D_004387E8, category, index);
    allocation = sdfReadNamedResource(path, &sourceAddress, 0);
    mappedResource = effCreateMappedResource(sourceAddress);
    sdfReleaseResourceAllocation(allocation);
    return mappedResource;
}

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(u64 job, u32 *outMappedResource) {
    u64 allocation;
    u32 sourceAddress;
    u32 mappedResource;

    allocation = fileGetResourceHandle();
    sourceAddress = sdfResourceRetainAddress(allocation);
    mappedResource = effCreateMappedResource(sourceAddress);
    *outMappedResource = mappedResource;
    sdfReleaseResourceAllocation(allocation);
    filePollEntryCleanup(job);
}

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(s32 category, s32 index, u32 *outMappedResource) {
    char path[EFF_RESOURCE_PATH_BYTES];
    func_0035C860(path, D_004387E8, category, index);
    *outMappedResource = 0;
    fileCreateCallbackRequest(path, 0, effCompleteMappedResourceJob, outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
void *effCreateOwnerRecordList(u32 ownerAddress) {
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
            itfDrawGridWithResolvedSlot(0, 0, 0, 0, (u32)list->owner, record->slot, drawOption);
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
    sdfReleaseChipBlock(buckets);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DE248", effConvertParamValue);

typedef struct EffMappedRecord {
    u8 pad00[0x14];
    u32 category;     // 0x14, dispatches the status-size calculation
    u32 statusBytes;  // 0x18, expanded to the required capacity when loaded
    u8 pad1C[4];
    u8 *status;       // 0x20
} EffMappedRecord;    // 0x24

/* Sum the status-storage byte requirements for the selected record category. */
u32 effSumRecordStatuses(u8 *recordBytes) {
    EffRecordBucket *group = &D_00400508[((EffMappedRecord *)recordBytes)->category];
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
u32 effLoadMappedStatusRecords(u8 *source, EffMappedHeader *headerOut) {
    EffMappedHeader header;
    u32 allocation;
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
        statusBytes = effSumRecordStatuses((u8 *)records);
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

typedef struct EffMappedResource {
    s32 count;                // 0x00
    u32 allocation;           // 0x04, owns the record array
    EffMappedRecord *records; // 0x08, retained address of that allocation
} EffMappedResource;          // 0x0C

/* Build the live batch header from the serialized count and owned record array. */
u32 effCreateMappedResource(u32 sourceAddress) {
    EffMappedResource *mappedResource = (EffMappedResource *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    EffMappedHeader header;

    mappedResource->allocation = effLoadMappedStatusRecords((u8 *)sourceAddress, &header);
    mappedResource->records = (EffMappedRecord *)sdfResourceRetainAddress(mappedResource->allocation);
    mappedResource->count = header.count;
    return (u32)mappedResource;
}

/* Build one zeroed status record and allocate the category's required status storage. */
u32 *effCreateStatusBatch(u32 category) {
    EffMappedResource *batch = (EffMappedResource *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    u32 allocation;
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
        statusBytes = effSumRecordStatuses((u8 *)record);
    }
    statuses = (u8 *)sdfAllocSizeClassBlock(statusBytes);
    batch->records->status = statuses;
    memset(statuses, 0, statusBytes);
    batch->records->statusBytes = statusBytes;
    return (u32 *)batch;
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



u32 effReleaseSlotWorkAllocation(s32 owner) {
    sdfReleaseResourceAllocation(((EffectSlotSet *)owner)->workAllocation);
    return 1;
}

/* Return the normal slot address unless its stored alternate address is nonzero. */
s32 effGetSlotWorkOrOverride(s32 owner, s32 slotIndex) {
    s32 alternateAddress;
    s32 entryAddress;

    entryAddress = slotIndex * EFF_SLOT_WORK_BYTES + (s32)((EffectSlotSet *)owner)->workEntries;
    alternateAddress = ((BdWork *)entryAddress)->alternate.address;
    if (alternateAddress != 0) {
        entryAddress = alternateAddress;
    }
    return entryAddress;
}

/* Start at zero when bit zero is set, otherwise at the full 16.16 endpoint.
 * The return value remains the original flag mask or full endpoint.
 */
u32 effInitializeSlotPhase(EffTimedState *state) {
    u32 value = state->flags & EFF_PHASE_START_ZERO_BIT;
    if (value != 0) {
        state->value = 0;
    } else {
        value = EFF_PHASE_FULL;
        state->value = value;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_00304B18);

/* Initialize the normal slot entry at the unchanged 0xA0-byte stride. */
void effInitializeSlotWork(s32 owner, s32 slotIndex) {
    func_00304B18(owner, slotIndex, &((EffectSlotSet *)owner)->workEntries[slotIndex]);
}

extern void effResetSlotWork(u32, u32);

/* Reset every normal work slot in source order. */
void effInitializeAllSlotWork(u8 *owner) {
    u32 slotIndex = 0;
    if (((EffectSlotSet *)owner)->count != 0) {
        do {
            effResetSlotWork((u32)owner, slotIndex++);
        } while (slotIndex < ((EffectSlotSet *)owner)->count);
    }
}

/* Store the owner and slot index before invoking the work initializer. */
void effAttachSlotWorkOwner(void *owner, s32 slotIndex, BdWork *entry) {
    entry->owner = owner;
    entry->slotIndex = slotIndex;
    func_00304B18(owner, slotIndex, entry);
}

/* Clear the complete normal slot, then restore owner/index and initialize it. */
void effResetSlotWork(u32 owner, u32 slotIndex) {
    BdWork *entry;

    entry = &((EffectSlotSet *)owner)->workEntries[slotIndex];
    memset(entry, 0, EFF_SLOT_WORK_BYTES);
    effAttachSlotWorkOwner((void *)owner, slotIndex, entry);
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
                    if (set->handles[slotIndex] == 0) {
                        set->handles[slotIndex] = (void *)sdfTexAcquireResourceTexture(resourceData);
                    }
                } else {
                    set->handles[slotIndex] = 0;
                }
            } else {
                set->handles[slotIndex] = 0;
            }
            slotIndex++;
        } while (slotIndex < set->textureCount);
    }
    return entryBytes;
}

void effResolveAndReleaseResource(u32 *owner) {
    if (owner[0] != 0) {
        u32 resource = owner[0];
        u32 mapped = sdfResourceRetainAddress(resource);
        effResolveResourceSlots(owner, mapped, 0, -1);
        sdfDecrementAllocationReferenceCount(owner[0]);
    }
}

void effResolveAndReleaseSelectedResource(u32 *owner, s32 mapping) {
    if (owner[0] != 0) {
        u32 resource = owner[0];
        u32 mapped = sdfResourceRetainAddress(resource);
        effResolveResourceSlots(owner, mapped, 0, mapping);
        sdfDecrementAllocationReferenceCount(owner[0]);
    }
}

extern void sdfTexReleaseReference(s32, u32, u32);

void effReleaseSlotTextureReferencesAndResetWork(u8 *owner, s32 preserve) {
    u32 i = 0;
    u32 count = ((EffectSlotSet *)owner)->textureCount;
    u32 *resources;

    if (count != 0) {
        resources = (u32 *)((EffectSlotSet *)owner)->handles;
        do {
            if (resources[i] != 0) {
                u32 *current;
                sdfTexReleaseReference(resources[i], (u32)resources, count);
                current = (u32 *)((EffectSlotSet *)owner)->handles;
                count = ((EffectSlotSet *)owner)->textureCount;
                resources = current;
                current[i] = 0;
            }
            i++;
        } while (i < count);
    }
    if (!preserve) {
        effInitializeAllSlotWork(owner);
    }
}

void effReleaseTextureHandlesAndResetSlots(u32 owner) {
    effReleaseSlotTextureReferencesAndResetWork(owner, 0);
}

u8 effHasFirstTextureHandle(s32 owner) {
    return *(s32 *)((EffectSlotSet *)owner)->handles != 0;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
u32 *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    u32 *payload = (u32 *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    u32 allocation = sdfAllocGeneralBlock(recordBytes);
    payload[1] = recordCount;
    payload[0] = allocation;
    payload[2] = sdfResourceRetainAddress(allocation);
    memset((void *)payload[2], 0, recordBytes);
    return payload;
}

/* Release the record allocation before freeing its small header. */
u32 effDestroyPayload(u32 payload) {
    sdfReleaseResourceAllocation(*(u32 *)payload);
    sdfReleaseChipBlock(payload);
    return 1;
}

u32 func_00305148(u32 allocationHandle, u32 keepAllocation) {
    EffectSlotSet *set;
    u8 *resource;
    u8 *entries;
    u32 index;
    u32 sourceOffset;

    set = (EffectSlotSet *)sdfAllocSizeClassBlock(0x30);
    memset(set, 0, 0x30);
    set->unk04 = 0;
    set->sourceAllocation = keepAllocation != 0 ? allocationHandle : 0;
    resource = (u8 *)sdfResourceRetainAddress(allocationHandle);
    set->textureCount = *(u16 *)(resource + 0x14);
    set->textureAllocation = (u32)sdfAllocGeneralBlock(
        set->textureCount * 4);
    set->handles = (void **)sdfResourceRetainAddress(set->textureAllocation);
    memset(set->handles, 0, set->textureCount * 4);
    entries = effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->count = *(u16 *)(resource + 0x16);
    set->descriptionAllocation =
        (u32)sdfAllocGeneralBlock(set->count * 0x80);
    set->descriptions = (EffectSlotDescription *)sdfResourceRetainAddress(set->descriptionAllocation);
    set->workAllocation = (u32)sdfAllocGeneralBlock(set->count * 0xA0);
    set->workEntries = (BdWork *)sdfResourceRetainAddress(set->workAllocation);
    for (index = 0; index < set->count; index++) {
        sourceOffset = *(u32 *)(entries + 4);
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork((u32)set, index);
        entries += 8;
    }
    return (u32)set;
}

u32 *effCreateResourceSlotSet(u32 *sourceHandle, u32 slot, u32 count) {
    EffectSlotSet *source = (EffectSlotSet *)sourceHandle;
    EffectSlotSet *effect = (EffectSlotSet *)sdfAllocSizeClassBlock(0x30);
    u32 index = 0;
    effect->unk04 = 1;
    {
        u32 mode = source->textureCount;
        void **handles = source->handles;
        effect->textureCount = mode;
        effect->handles = handles;
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
            effResetSlotWork((u32)effect, index);
            index++;
        } while (index < effect->count);
    }
    return (u32 *)effect;
}

u32 effDestroyResourceSlotSet(u32 effect) {
    EffectSlotSet *set;

    set = (EffectSlotSet *)effect;
    if (set->sourceAllocation != 0) {
        sdfReleaseResourceAllocation(set->sourceAllocation);
    }
    if (set->unk04 == 0) {
        effReleaseTextureHandlesAndResetSlots(effect);
        sdfReleaseResourceAllocation(set->textureAllocation);
    }
    sdfReleaseResourceAllocation(set->descriptionAllocation);
    effReleaseSlotWorkAllocation(effect);
    sdfReleaseChipBlock(effect);
    return 1;
}

u32 effSetSlotResourceAndFlags(EffTimedState *effect, u32 resource, u32 flags) {
    effect->flags = flags;
    effect->source = (u8 *)resource;
    if ((flags & 2) != 0) {
        effInitializeSlotPhase(effect);
    }
    effect->delay = effect->delay + 1;
    return 1;
}

u32 effSetSlotIndexedResource(u32 effect, s32 owner, s32 index, u32 flags) {
    effSetSlotResourceAndFlags((EffTimedState *)effect, (u32)&((EffMappedResource *)owner)->records[index], flags);
    return 1;
}


u32 effSlotTransitionClearTarget(s32 work, u32 unused) {
    ((EffTimedState *)work)->source = NULL;
    return 1;
}

s32 effClampSlotPhaseAtEnd(u32 effect, u32 slot, EffTimedState *state) {
    if (0x10000 < state->value) {
        u32 flags = state->flags;
        state->value = 0x10000;
        if (flags & 4) {
            if (flags & 8) {
                state->flags = flags & ~1;
            } else {
                effInitializeSlotWork(effect, slot);
            }
            return 0;
        }
    }
    return 1;
}

s32 effClampSlotPhaseAtStart(u32 effect, u32 slot, EffTimedState *state) {
    if (state->value < 0) {
        u32 flags = state->flags;
        state->value = 0;
        if (flags & 4) {
            if (flags & 8) {
                state->flags = flags | 1;
            } else {
                effInitializeSlotWork(effect, slot);
            }
            return 0;
        }
    }
    return 1;
}

typedef struct EffStateSource {
    u8 pad_00[0x14];
    u32 kind;       // 0x14
} EffStateSource;

extern u32 effResetRecordRun(u8 *, u32, u32);

u8 *effUpdateTimedStates(u8 *effect, u32 slot, u8 *entry) {
    EffTimedState *states = (EffTimedState *)(entry + 0x28);
    BdWork *record = &((EffectSlotSet *)effect)->workEntries[slot];
    s32 idle = 1;
    u32 i;

    for (i = 0; i < 2; i++) {
        EffTimedState *state = &states[i];
        EffStateSource *source = (EffStateSource *)state->source;

        if (source != 0 && source->kind != 0) {
            EffRecordBucket *bucket = &D_00400508[source->kind];
            s32 step = bucket->step(record, (BdWork *)entry, state);

            if (state->delay > 0) {
                step = 0;
                state->delay -= 1;
            }
            if (step >= 0) {
                if (state->flags & 1) {
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
        effResetRecordRun(effect, slot, -1);
        return 0;
    }
    return effect;
}

s32 effSetSlotOverrideWork(u8 *effect, u32 slot, u32 material) {
    if (((EffectSlotSet *)effect)->workEntries[slot].alternate.bits == 0) {
        effAttachSlotWorkOwner(effect, slot, (BdWork *)material);
    }
    ((EffectSlotSet *)effect)->workEntries[slot].alternate.bits = material;
    return 1;
}

u32 effSetMaterialSlots(s32 work, s32 index, u32 value, BdWork *asset) {
    u32 i;
    EffTimedState *states;
    if (((EffectSlotSet *)work)->workEntries[index].alternate.asset == NULL) {
        effAttachSlotWorkOwner((void *)work, index, asset);
    }
    ((EffectSlotSet *)work)->workEntries[index].alternate.asset = asset;
    states = asset->states;
    for (i = 0; i < 2; i++) {
        states[i].materialFlags = value;
    }
    return 1;
}

u32 effClearSlotOverrideWork(s32 effect, s32 slot) {
    ((EffectSlotSet *)effect)->workEntries[slot].alternate.bits = 0;
    return 1;
}

s32 effConfigureSlotResource(u8 *effect, u32 slot, u32 resource, u32 flags) {
    BdWork *entry = &((EffectSlotSet *)effect)->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], resource, flags);
    effUpdateTimedStates(effect, slot, (u8 *)entry);
    return 1;
}

s32 effConfigureIndexedSlotResource(u8 *effect, u32 slot, u8 *resources, u32 index, u32 flags) {
    BdWork *entry = &((EffectSlotSet *)effect)->workEntries[slot];
    u32 resource = (u32)&((EffMappedResource *)resources)->records[index];
    effSetSlotResourceAndFlags(&entry->states[0], resource, flags);
    effUpdateTimedStates(effect, slot, (u8 *)entry);
    return 1;
}

s32 effConfigureIndexedSlotMaterial(u8 *effect, u32 slot, u8 *resources, u32 index,
                  u32 flags, u32 option, u32 color) {
    BdWork *entry = &((EffectSlotSet *)effect)->workEntries[slot];
    u32 resource = (u32)&((EffMappedResource *)resources)->records[index];
    effSetSlotResourceAndFlags(&entry->states[0], resource, color);
    effUpdateTimedStates(effect, slot, (u8 *)entry);
    entry->states[0].materialFlags = flags;
    entry->states[0].materialValue = option;
    return 1;
}

u32 effConfigureWithDefaultSetting(u32 effect, u32 slot, u32 kind, u32 value, u32 flags, u32 color) {
    effConfigureIndexedSlotMaterial(effect, slot, kind, value, flags, 0, color);
    return 1;
}

u32 effResetRecordRun(u8 *table, u32 first, u32 arg) {
    u32 i = 0;
    u32 index;

    do {
        effSlotTransitionClearTarget((first + i) * 0xA0 + (s32)((EffectSlotSet *)table)->workEntries + 0x28, arg);
        i++;
        index = first + i;
    } while (index < ((EffectSlotSet *)table)->count && (((EffectSlotSet *)table)->descriptions[index].flags & 0x20));
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
extern s32 sdfConsCreateDrawPacket(s32, s32, s32);
extern void effSelectPresetByKind(u32, u32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);

void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 z, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *color, u32 flip,
                   u32 blendKind, s32 mode, SdfTex *texture, s32 surfaceId) {
    u32 uv[4];
    void *packet;
    void *list;
    u64 *dst;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 temp;

    if (mode == 0) {
        sdfTexSetPrimaryBufferModeBits(texture, 0, 1);
    } else {
        sdfTexSetPrimaryBufferModeBits(texture, 1, 1);
    }
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, 1));
    sdfConsInitPacketHeader(packet, 0x156, 5, 0x43431, 1);
    /* The SDK size helper also skips the packet's two header quadwords. */
    dst = (u64 *)sdfConsMeasurePacketWithHeader((s32)packet);
    x0 = x + 0x7000;
    y0 = y + 0x7900;
    uv[0] = uvRect->u0 * 16;
    uv[2] = uvRect->u1 * 16;
    uv[1] = uvRect->v0 * 16;
    uv[3] = uvRect->v1 * 16;
    x1 = x0 + width;
    y1 = y0 + height;
    if (flip & 1) {
        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if (flip & 2) {
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    if (color == NULL) {
        dst[0] = ((u64)0x80 << 32) | 0x80;
        dst[1] = ((u64)0x80 << 32) | 0x80;
    } else {
        u32 rgba = color->rgba;

        dst[0] = color->channels.red | ((u64)color->channels.green << 32);
        dst[1] = ((rgba >> 8) & 0xFF) | ((u64)(rgba & 0xFF) << 32);
    }
    dst[2] = uv[0] | ((u64)uv[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[6] = uv[2] | ((u64)uv[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[5] = z;
    dst[9] = z;
    effSelectPresetByKind(blendKind, surfaceId);
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsCreateDrawPacket((s32)list, (s32)texture, 0);
    sdfAppendPacket(list, packet);
    effSubmitSurfacePacket(&kwlnDrawSurfaces[surfaceId], list);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}

INCLUDE_ASM(const s32, "game/code_002DE248", func_00305EB0);

INCLUDE_ASM(const s32, "game/code_002DE248", func_00306030);

extern void func_00306030(u32, u32, u32, u32, u32, u32, u32, u32,
                          f32, u32, u32, u32, u32, u32);

void itfDrawRotatedTexturedRect(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h,
                   f32 rotation, u32 x, u32 y, u32 width, u32 height) {
    func_00306030(a, b, c, d, e, f, g, h, rotation, x, y, 1, width, height);
}

void effSelectPresetAndDispatch(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    effSelectPresetByKind(arg6, arg7);
    uiDrawGradientColorRect(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
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

