#include "common.h"
#include "pcp_vu0.h"

typedef struct EffectStateSnapshot {
    s128 vectors[8];
} EffectStateSnapshot;

extern void func_002D3A68(u32, u32, u32);
extern void func_00303C50(void);
extern void fileJobSetPrimaryData(u32, void *, u32, u16);
extern s32 func_00303C78(s32);
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
extern void func_002D4F10(s32, void *);
extern EffectAssetLink *D_003FF128[24];

typedef struct EffectVectorRequest {
    u8 kind;
    u8 count;
    u8 size;
    u8 pad_03;
    u32 unk04;
} EffectVectorRequest;

typedef struct EffBattleCamera {
    u8 pad_00[0x60];
    f32 scale;
    u32 mode;
} EffBattleCamera;

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
extern u32 func_00169440(void);
extern u32 func_00169448(void);
extern u32 func_00169450(void);
extern u32 func_00169438(void);
extern void func_002D3788(void *, void *);
extern void func_002D37C8(void *, f32);
extern u8 D_0045C270[];
extern void effComputeBattleCameraPositionVU(u8 *);
extern void func_002D3808(void *, u32);
extern void func_002D3748(void *, void *);
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
    u8 pad0[0x28];
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


extern float func_00341240(void *);
extern s32 func_002FF0B8(EffectStateSnapshot *, s32);

#include "ee_mmi.h"

typedef struct EffOp28 {
    void (*run)();
    u32 unk[6];
} EffOp28;

typedef struct EffOp24 {
    void (*run)();
    u32 unk[5];
} EffOp24;
typedef struct EffModelOwner {
    f32 scale;
    s32 model;
    void *ownedBuffer;
} EffModelOwner;

typedef struct EffKindDesc {
    u32 (*create)(void *);        // 0x00
    u8 reserved[8];               // 0x04
    void (*initialize)(s32, s32); // 0x0C
    u32 size;                     // 0x10
} EffKindDesc; // 0x14

/* Effect work created from a kind descriptor: 0x30-byte header followed by a copy of the source payload. */
typedef struct EffKindWork {
    u8 vec[0x10];     // 0x00
    s32 mode;         // 0x10
    u32 color;        // 0x14
    f32 scale;        // 0x18
    s32 kind;         // 0x1C
    u32 frame;       // 0x20, incremented by the kind callback dispatcher
    u32 handle;       // 0x24
    void *payload;    // 0x28
    void *target;     // 0x2C
} EffKindWork; // 0x30

extern EffKindDesc D_003E9810[];
extern EffKindDesc D_003E98A0[];
extern EffOp28 D_003E9E68[];
extern EffOp28 D_003E9DE0[];
extern EffOp24 D_003E9D08[];
extern s32 D_004386B8;

typedef struct EffRecordBucket {
    u8 pad_00[4];
    s32 (*step)(u8 *, u8 *, void *);   // 0x04
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
extern void func_002D31C0(void *);
extern u8 D_003E9140[];
extern u8 D_003E9110[];
extern void func_002DB2C0(s32, void *);
extern s32 billGetFirstEntryFramePeriod(u32);
extern void billSetEntryFrameMode1(u32, s32);
extern f32 func_00159FF8(void *, void *);
extern f32 func_0015A150(void *, void *);
extern void func_00159BF0(u32, f32, f32);
extern void func_00159BE8(u32, f32);
extern void effCopyVector(u32, void *);
extern void billInvokeCallback(u32);
extern f32 D_0042BC10[4];
extern u8 D_003E9100[];
extern void func_002E5E88(u8 *, void *);
extern void func_002E76C8(u8 *, void *);
extern void func_002F1888(u8 *, void *);

extern void func_003343E8(s32, f32);
extern void func_00103F58(s32, u8, s32);
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

extern EffectMapping D_00400018;
extern u32 D_0045C2F0[];
extern u8 D_004386E0[];
extern u8 D_004386E8[];

extern u32 effCreateMappedResource(u32);

extern u32 sdfResourceRetainAddress();

extern u64 fileGetResourceHandle(void);

extern u32 func_00305148();


extern void *sdfModelCreateWithAlternateItems(u32, u32);

extern u32 func_00328D68(u32);

extern s32 D_00437E74;

extern u32 D_00437E78;

extern u32 D_00437E50;

extern u32 D_00437E08;

extern void *fileResolvePrimaryBuffer();

extern s32 fileResolveSecondaryBuffer(void *);

extern void *func_00328E18(s32);

extern void func_00328E48();
extern void mdlLoadPrimaryVectorVU(void *);
extern void func_0033A7E8(u32, void *, void *);
extern u128 D_004584B0[];
extern u128 D_00458470[];
extern u8 D_003E9F50[];
extern EffOp24 D_003E9F10[];

extern void sdfTexReleaseReference();

extern void effReleaseSharedReference();

extern s64 func_001AA308(void);

extern s64 btlIsCurrentActorFullyMarked(void);

extern s32 func_002DC1D0(u32, u32);
extern EffModelOwner *effCreateModelOwner();

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

extern void func_0035B6E0();

extern void dds3DispatchIndexedCallback(s32, f32);

extern void billSetBillboardMode(s32, s16);

extern void func_00159FA0(s32);

extern u8 D_00380828[];
extern void func_00232390(void *, const void *);

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *D_00437E30;

extern u32 D_00437E38;

extern u32 D_00437E3C;

extern u32 func_00159A50(u32);

extern u32 func_002DDF48(u32);

extern u32 D_00437E70;

extern u32 D_00437E80;

extern s32 D_00437E84;

extern u32 D_00437E88;

extern u32 func_002DDCA0(u32, u32);

extern s32 *func_002F69F0(u32, u32, s32);

extern s32 func_001AA6F8(void);

extern s32 D_004386C4;

extern s32 D_004386B4;

extern u8 D_00439075;

extern s32 D_004386F0;

extern s32 D_004386F4;

extern s32 D_0043875C;

extern s32 D_00438760;

extern s32 D_00438768;

extern u32 D_0043876C;

extern u32 D_00438770;

extern u32 D_00438778;

extern u32 D_004387A8;

extern u32 D_004387AC;

extern u32 D_004387B0;

extern u32 D_00438774;

extern void mdlStoreTertiaryVectorVU(void *);

extern void effFloorModelListRemove(EffectObjectNode *);

extern void *func_002DDAA8(void *);

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 refCount;      // 0x14 incremented with the global reference count
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern u32 D_00437E34;

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

/* Function-pointer tables indexed by object fields (entry size inferred). */
typedef struct FnTbl28 {
    void (*fn)();
    void *(*init)();
    u8 pad_0x08[4]; // 0x08
    void *(*duplicate)();
    u8 pad_0x10[0xC]; // 0x10
} FnTbl28; // 0x1C

extern FnTbl28 D_003E9964[];

typedef struct FnTbl24 {
    void (*fn)();
    u32 (*createResource)();
    u8 pad_0x08[0x10]; // 0x08
} FnTbl24; // 0x18

typedef struct FnTbl24Create {
    void (*fn)();
    u32 (*createResource)();
    u8 pad_0x08[0x10]; // 0x08
} FnTbl24Create; // 0x18

extern FnTbl24 D_003E9B90[];

extern FnTbl24 D_003E9B8C[];

extern u8 *D_003E9CA8[];

extern u32 D_00437E68;

extern FnTbl24 D_003E9D10[];

extern FnTbl28 D_003E9DEC[];

extern u32 D_00437E6C;

extern u32 func_00343ED0(const char *, u32 *, s32);

extern FnTbl28 D_003E9E74[];

extern u8 *func_002F3D88();

extern u32 D_00437E7C;

extern FnTbl24 D_003E9F18[];

extern u32 D_004386B0;

extern u32 D_003FFA84[];

extern u8 D_0045C110[];

extern u8 D_00438758;

extern u8 D_00438759;

extern u8 D_0043875A;

extern u32 D_004386C0;

extern FnTbl28 D_003E9950[];

extern FnTbl24Create D_003E9B80[];

extern FnTbl28 D_003E9DD8[];

extern FnTbl28 D_003E9DE8[];

extern FnTbl28 D_003E9E60[];

extern FnTbl28 D_003EA018[];

extern FnTbl28 D_003EA020[];

extern FnTbl24Create D_003E9F08[];

extern u8 D_0045C1A0[];

extern u8 D_003FFA40[];

extern u8 D_0045C1E0[];

extern u8 *D_004386CC;

extern u8 D_0045C300[];

extern u8 D_00400150[];

extern u8 D_00400250[];

extern FnTbl24 D_003E9D00[];

extern FnTbl28 D_003EA028[];

extern FnTbl28 D_003EA02C[];

extern void *func_00232198(s32 group, s32 id);

extern s32 func_00232EE8(void *model);

extern s32 func_00232EF8(void *model);

extern FnTbl28 D_003E9E70[];

extern FnTbl24 D_003E9F14[];

/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */
extern void *func_003292A8(u32);

extern u8 *effCreatePointSet4(u32);

/* Slot count of an effect record, capped at max. */
static inline u32 effSlotCount(u8 *p, u32 max) {
    u32 a = *(u32 *)(p + 0x20);
    u32 n = (a ? a : *(u32 *)(p + 0x24)) * (a ? *(u32 *)(p + 0x24) : *(u32 *)(p + 0xB8));
    return n < max + 1 ? n : max;
}


extern void sndLoadAndPlayStationedSe(u32);

extern u32 func_002E5C50(u32, u16, u32);

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

extern u32 func_002F1740(u32, u32, u32);

extern u8 *effCreateRibbonWork(u32, u32);

/* Battle/display work object (layout inferred from field accesses). */
typedef struct BdWork {
    s32 x0;            // 0x00
    s32 x4;            // 0x04
    u8 pad_0x08[0x58]; // 0x08
    void *x60;         // 0x60
    s32 x64;           // 0x64
} BdWork; // 0x68

typedef struct EffectMaterialSlot {
    u32 value;
    u8 unk_04[0x10];
} EffectMaterialSlot;

extern u128 *D_0037F770[];

extern u128 D_00458460[];

extern u128 D_004584A0[];

extern u128 D_0037F780[];

extern s8 D_00437E94;

typedef struct EffectResourceSizeEntry {
    u32 resourceSize;
    u8 pad_04[0x18];
} EffectResourceSizeEntry;

typedef struct EffectResourceSizeEntry24 {
    u32 resourceSize;
    u8 pad_04[0x14];
} EffectResourceSizeEntry24;

extern EffectResourceSizeEntry24 D_003E9B94[];
extern EffectResourceSizeEntry24 D_003E9D14[];
extern EffectResourceSizeEntry D_003EA030[];
extern EffectResourceSizeEntry D_003E9DF0[];

extern EffectResourceSizeEntry D_003E9968[];

extern u8 *func_002E56A8(u16, void *);

extern EffectResourceSizeEntry D_003E9E78[];

extern u8 *effAllocateBlockWithModel(u16, void *);

extern u32 func_002F5358(u16, void *, void *, u32);

void effInitModelVUState(void *model) {
    VU0_MOVE_VF(vf10, vf0);
    mdlStorePrimaryVectorVU(model);
    VU0_MOVE_VF(vf10, vf0);
    func_00232AD0(model);
    VU0_SET_ONES_XYZ(vf10);
    mdlStoreTertiaryVectorVU(model);
    mdlBroadcastMasked(model, 0x80808080);
    if (*(void **)(model + 0x1C) != NULL) {
        mdlAddEntryPlain(model, 0, 0);
        *(float *)(*(u8 **)(model + 0x1C) + 0x20) = 1.0f;
    }
    *(u32 *)model &= ~1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC1D0);

void effDestroyModelContext(s32 owner) {
    *(u32 *)(*(s32 *)(owner + 0x18) + 0x80) = 0;
    mdlDestroyContext();
}

void *effCloneModelWithVUState(void *sourceModel) {
    s32 group;
    s32 id;
    void *model;

    group = func_00232EE8(sourceModel);
    id = func_00232EF8(sourceModel);
    model = func_00232198(group, id);
    effInitModelVUState(model);
    return model;
}

EffModelOwner *effCreateModelOwner(u8 *source) {
    EffModelOwner *owner = func_00328E18(0x10);
    owner->ownedBuffer = func_00328E18(0xE0);
    if (source != NULL) {
        s32 data;
        *(u32 *)owner = *(u32 *)fileResolvePrimaryBuffer(source);
        data = fileResolveSecondaryBuffer(source);
        if (data != 0) {
            owner->model = func_002DC1D0(data, *(u32 *)(source + 0x24));
            VU0_SET_ONES_XYZ(vf10);
            __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" : : "f"(owner->scale) : "$2", "memory");
            mdlStoreTertiaryVectorVU((void *)owner->model);
        }
    }
    return owner;
}

void effDestroyModelOwner(EffModelOwner *owner) {
    void *buffer = owner->ownedBuffer;
    if (buffer != NULL) {
        func_00328E48(buffer);
    }
    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    func_00328E48(owner);
}

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    *(u32 *)owner = *(u32 *)source;
    effRecreateModelFromSource(owner, source);
    return owner;
}

void effRecreateModelFromSource(EffModelOwner *owner, EffModelOwner *source) {
    s32 group;
    s32 id;
    void *model;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    group = func_00232EE8((void *)source->model);
    id = func_00232EF8((void *)source->model);
    model = func_00232198(group, id);
    effInitModelVUState(model);
    VU0_SET_ONES_XYZ(vf10);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvmulx.xyzw vf10, vf10, vf2x\n\t.set reorder" : : "f"(owner->scale) : "$2");
    mdlStoreTertiaryVectorVU(model);
    owner->model = (s32)model;
}

void effModelAnimationStop(EffModelOwner *owner) {
    func_003343E8(*(s32 *)(owner->model + 0x1C), 0.0f);
}

void effRefreshModelLighting(void **obj) {
    if (effComputeLightDirectionVU(obj[1], obj[2])) {
        *(u32 *)(*(u8 **)((u8 *)obj[1] + 0x18) + 0x80) = (u32)obj[2];
    }
    func_00232390(obj[1], D_00380828);
}

void effApplyModelPrimaryVector(EffModelOwner *owner, u8 *vec) {
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(vec) : "memory");
    mdlStorePrimaryVectorVU((void *)owner->model);
}

void effApplyModelVecB(EffModelOwner *owner, u8 *vec) {
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(vec) : "memory");
    func_00232AD0((void *)owner->model);
}

void effBroadcastModelMask(EffModelOwner *owner) {
    mdlBroadcastMasked(owner->model);
}

void effApplyScaledModelTertiaryVector(s32 model, float scale) {
    float v[3];
    float t;

    t = *(float *)model * scale;
    v[0] = v[1] = v[2] = t;
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (v));
    mdlStoreTertiaryVectorVU(*(void **)(model + 4));
}

/* Texture record read by the floor-model setup (see SdfTex). */
typedef struct EffTexture {
    u8 pad_00[0xC];
    s16 width;   // 0x0C
    s16 height;  // 0x0E
    u8 pad_10[0xA];
    u8 format;   // 0x1A
    u8 pad_1B[9];
    s32 slot;    // 0x24
} EffTexture;

typedef struct EffTexTable {
    u8 pad_00[0xC];
    EffTexture **entries; // 0x0C
} EffTexTable;

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} EffBattleTexHeaders;

extern s32 func_0032B240();
extern u8 *func_0032B500(void *, u32, u8 *, s32);
extern s32 func_0032B3F8(u32, s16, s16, u8, u8 *, s32);
extern u32 sdfTexGetPrimaryResourceWord(void *);
extern u32 sdfTexGetSecondaryResourceWord(void *);

void effUploadModelTextures(EffModelOwner *owner) {
    s32 i = 0;
    EffBattleTexHeaders *battle = (EffBattleTexHeaders *)func_001AA6F8();
    EffTexTable *table = *(EffTexTable **)(*(s32 *)(owner->model + 0x18) + 8);

    do {
        EffTexture *tex = table->entries[i++];
        u8 *header;
        u8 *pixels;
        s16 width;
        s16 height;
        u8 format;

        switch (tex->slot) {
        case 1:
            header = battle->slot1;
            break;
        case 2:
            header = battle->slot2;
            break;
        default:
            header = NULL;
            break;
        }
        pixels = header + (header[1] & 0xF0) + 0x40;
        if (func_0032B240(tex) != 0) {
            pixels = func_0032B500(tex, sdfTexGetSecondaryResourceWord(tex), pixels, 1);
        }
        width = tex->width;
        height = tex->height;
        format = tex->format;
        func_0032B3F8(sdfTexGetPrimaryResourceWord(tex), width, height, format, pixels, 1);
    } while (i < 2);
}

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(u8 *source) {
    EffModelOwner *owner;
    s64 battleActive;

    owner = effCreateModelOwner(source);
    effUploadModelTextures(owner);
    battleActive = func_001AA308();
    if ((battleActive != 0) && (battleActive = btlIsCurrentActorFullyMarked(), battleActive == 0)) {
        effFloorModelListPush(owner);
    }
    return owner;
}

void effMarkFloorModelForDestruction(u32 *p) {
    p[3] |= 2;
    if (!(p[3] & 4)) {
        effDestroyModelOwner(p);
    }
}

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source) {
    EffModelOwner *owner = effCreateModelOwner(0);
    s64 marked;

    *(u32 *)owner = *(u32 *)source;
    effRecreateModelFromSource(owner, source);
    effUploadModelTextures(owner);
    marked = func_001AA308();
    if ((marked != 0) && (marked = btlIsCurrentActorFullyMarked(), marked == 0)) {
        effFloorModelListPush(owner);
    }
    return owner;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC808);

void effMarkFloorModelForUpdate(u32 *p) {
    p[3] |= 1;
    if (!(p[3] & 4)) {
        if (!(D_00437E08 & 1)) {
            func_002DC808(p);
        }
    } else if (D_00437E08 & 1) {
        p[3] |= 0x30;
    }
}

void effFloorModelListPush(EffectObjectFlag *obj) {
    EffectObjectNode *entry = func_00328E18(sizeof(EffectObjectNode));

    entry->object = obj;
    entry->prev = NULL;
    if (D_00437E30 != NULL) {
        D_00437E30->prev = entry;
        entry->next = D_00437E30;
    } else {
        entry->next = NULL;
    }
    D_00437E30 = entry;
    entry->object->flags |= 4;
}

void effFloorModelListRemove(EffectObjectNode *node) {
    if (node->object != NULL) {
        func_0035B6E0("eff:floor model delete[%p]\n", node->object);
        effDestroyModelOwner((EffModelOwner *)node->object);
        node->object = NULL;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        D_00437E30 = node->next;
    }
    func_00328E48(node);
}

void effSweepFloorModelList(void) {
    EffectObjectNode *node = D_00437E30;
    EffectObjectNode *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (!(node->object->flags & 8)) {
                if ((node->object->flags & 0x31) == 1) {
                    func_002DC808(node->object);
                }
                node->object->flags &= ~0x20;
            } else {
                effFloorModelListRemove(node);
            }
            node = next;
        } while (node != NULL);
    }
}

void mdlPropagateObjectFlag(void) {
    EffectObjectNode *node = D_00437E30;
    EffectObjectFlag *object;
    s32 flags;

    if (node != NULL) {
        do {
            object = node->object;
            flags = object->flags;
            if ((flags & 2) != 0) {
                object->flags = flags | 8;
            }
            node = node->next;
        } while (node != NULL);
    }
}

void mdlClearListedObjectFlag(void) {
    EffectObjectFlag *object;
    EffectObjectNode *node;

    node = D_00437E30;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags & 0xffffffef;
    }
}

void mdlSetListedObjectFlag(void) {
    EffectObjectFlag *object;
    EffectObjectNode *node;

    node = D_00437E30;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags | 0x10;
    }
}

void mdlMarkAndProcessObjectNodes(void) {
    EffectObjectNode *node = D_00437E30;
    EffectObjectFlag *object;
    EffectObjectNode *next;
    s32 flags;

    if (node == NULL) {
        return;
    }
    do {
        object = node->object;
        next = node->next;
        flags = object->flags | 0xA;
        object->flags = flags;
        effFloorModelListRemove(node);
        node = next;
    } while (node != NULL);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCCE8);

typedef struct EffResourceOwner {
    u32 count;
    u8 pad_04[0x38];
    u32 plainEntry; // 0x3C: add the model entry plain instead of flagged
    u8 pad_40[0x78];
    void **entries;
    void *buffer;
    u32 model;
} EffResourceOwner;

extern void fileQueueDestroy(s32);

void effDestroyResourceOwner(EffResourceOwner *owner) {
    u32 i;

    if (owner->model != 0) {
        effDestroyModelContext(owner->model);
    }
    if (owner->buffer != 0) {
        for (i = 0; i < owner->count; i++) {
            fileQueueDestroy((s32)owner->entries[i]);
        }
        func_003297C8((u32)owner->buffer);
    }
    func_00328E48(owner);
}

typedef struct { u32 word[0xF]; } EffectBlob3C;
typedef struct { u32 word[0x1D]; } EffectBlob74;
extern u8 *func_002DCCE8(s32);
extern void effRecreateResourceOwnerModel(EffResourceOwner *, EffResourceOwner *);

s32 effDuplicateModelOwner(u8 *src) {
    u8 *dst = func_002DCCE8(0);

    *(EffectBlob3C *)(dst + 8) = *(EffectBlob3C *)(src + 8);
    *(EffectBlob74 *)(dst + 0x44) = *(EffectBlob74 *)(src + 0x44);
    effRecreateResourceOwnerModel((EffResourceOwner *)dst, (EffResourceOwner *)src);
    return (s32)dst;
}

extern u32 sdfCountMapPositionRecords(u32);
extern void *fileQueueClone(void *);

void effRecreateResourceOwnerModel(EffResourceOwner *dst, EffResourceOwner *src) {
    u32 i;

    if (dst->model != 0) {
        effDestroyModelContext(dst->model);
    }
    dst->model = (u32)func_00232198(func_00232EE8((void *)src->model), func_00232EF8((void *)src->model));
    effInitModelVUState((void *)dst->model);
    if (*(s32 *)(dst->model + 0x1C) != 0) {
        if (dst->plainEntry != 0) {
            mdlAddEntryPlain(dst->model, 0, 0);
        } else {
            mdlAddEntryFlagged(dst->model, 0, 0);
        }
    }
    dst->count = sdfCountMapPositionRecords(*(u32 *)(dst->model + 0x18));
    if (src->buffer != 0) {
        if (dst->buffer != 0) {
            for (i = 0; i < dst->count; i++) {
                fileQueueDestroy((s32)dst->entries[i]);
            }
            func_003297C8((u32)dst->buffer);
        }
        dst->buffer = func_003292A8(dst->count * 4);
        dst->entries = (void **)sdfResourceRetainAddress((u32)dst->buffer);
        for (i = 0; i < dst->count; i++) {
            dst->entries[i] = fileQueueClone(*src->entries);
        }
    }
}

extern void func_002D46A0(s32);

extern void func_003343E8(s32, f32);

void func_002DD3C0(s32 *work) {
    if (work[0xBC / 4] != 0) {
        u32 count = work[0];
        u32 i = 0;
        s32 *entries = (s32 *)work[0xB8 / 4];
        if (count != 0) {
            do {
                func_002D46A0(*entries);
                entries++;
                i++;
            } while (i < count);
        }
    }
    func_003343E8(*(s32 *)(work[0xC0 / 4] + 0x1C), 0.0f);
    work[1] = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD448);

void func_002DDA10(u8 *obj, u8 *vec) {
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(vec) : "memory");
    mdlStorePrimaryVectorVU(*(void **)(obj + 0xC0));
}

void func_002DDA30(u8 *obj, u8 *vec) {
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(vec) : "memory");
    func_00232AD0(*(void **)(obj + 0xC0));
}

void func_002DDA50(s32 model) {
    mdlBroadcastMasked(*(u32 *)(model + 0xc0));
}

void effScaleModelVec(u8 *work, float scale) {
    u32 bits;
    VU0_SET_ONES_XYZ(vf10);
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 %0, %1\n\t"
        "qmtc2.ni %0, $vf2\n\t"
        "vmulx.xyzw vf10, vf10, $vf2x\n\t"
        ".set reorder"
        : "=&r"(bits) : "f"(scale) : "memory");
    mdlStoreTertiaryVectorVU(*(void **)(work + 0xC0));
}

void effObjectListCountersReset(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDAA8);

u32 func_002DDCA0(u32 source, u32 value) {
    RefObj *copy;

    copy = (RefObj *)func_002DDAA8((void *)source);
    copy->unk18 = value;
    return (u32)copy;
}

extern u8 *D_00437E40;

void effReleaseSharedReference(RefObj *obj) {
    if (--D_00437E34 == 0) {
        u8 *texture = D_00437E40;

        *(u16 *)(texture + 0xC) = 0x100;
        *(u16 *)(texture + 0xE) = 0x100;
        D_00437E38 = 0xffffffff;
        sdfTexReleaseReference(texture);
    }
    if (--obj->refCount == 0) {
        func_003297C8(obj->cnt1C);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    D_00437E34++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDD60);

/* Each 0x10-byte entry contributes itself plus the number stored in its first word. */
typedef struct EffExpandedList {
    u8 pad0[4];
    u32 count;
    u8 pad8[8];
    u8 *entries;
} EffExpandedList;

s32 effCountExpandedEntries(void *work) {
    EffExpandedList *list = work;
    u32 count = list->count;
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        u8 *entries = list->entries;
        do {
            s32 additionalCount = *(s32 *)entries;
            entries += 0x10;
            i++;
            total++;
            total += additionalCount;
        } while (i < count);
    }
    return total;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDF48);

void effReleaseReferenceHolder(u32 *holder) {
    u32 i;

    if (--holder[0x1C / 4] == 0) {
        for (i = 0; i < holder[1]; i++) {
            effReleaseSharedReference(((u32 *)holder[0x14 / 4])[i]);
        }
        func_003297C8(holder[0x24 / 4]);
    }
}

RefObj *effReferenceObjectRetain(RefObj *obj) {
    obj->cnt1C++;
    return obj;
}

/* One animation track: segments of `length` frames each (plus one), looping if flags & 1. */
typedef struct EffAnimSegment {
    u32 length;     // 0x00
    u8 pad_04[0xC];
} EffAnimSegment;

typedef struct EffAnimSet {
    u8 pad_00[4];
    u32 count;                 // 0x04
    u32 flags;                 // 0x08: 1 loop, 4 double speed
    u8 pad_0C[4];
    EffAnimSegment *segments;  // 0x10
    u8 pad_14[4];
    u32 length;                // 0x18
} EffAnimSet;

typedef struct EffAnimSample {
    f32 weight;     // 0x00
    f32 speed;      // 0x04
    u32 pad_08;
    s32 segment;    // 0x0C
} EffAnimSample;

void effSampleAnimSet(EffAnimSet *set, u32 frame, EffAnimSample *out) {
    u32 count = set->count;
    u32 local = 0;
    s32 segment = -1;
    EffAnimSegment *seg;
    u32 acc;
    u32 i;

    if (count == 1) {
        segment = 0;
    } else {
        if (set->flags & 1) {
            local = frame % set->length;
        } else if (frame >= set->length) {
            segment = count - 1;
        } else {
            local = frame;
        }
        if (segment == -1) {
            seg = set->segments;
            acc = 0;
            for (i = 0; i < count; i++) {
                acc += seg->length;
                if (acc >= local) {
                    segment = i;
                    break;
                }
                acc++;
                seg++;
            }
        }
    }
    out->weight = 1.0f;
    {
        f32 speed = 2.0f;

        if (!(set->flags & 4)) {
            speed = 1.0f;
        }
        out->segment = segment;
        out->speed = speed;
    }
    out->pad_08 = 0;
}

void func_002DE218(s32 owner, u32 target, s32 indexSource) {
    func_002DDD60(target, *(u32 *)(*(s32 *)(indexSource + 0xc) * 4 + *(s32 *)(owner + 0x14)));
}

u32 *effDuplicateSmallHeader(src)
    void *src;
{
    u32 *p = (u32 *)func_00328D68(0xc);
    p[0] = 0;
    memcpy(p + 1, src, 8);
    return p;
}

void func_002DE288(void) {
    u64 resource;

    resource = fileResolvePrimaryBuffer();
    effDuplicateSmallHeader(resource);
}

void func_002DE2A8(u32 allocation) {
    func_001057A8();
    func_00328E48(allocation);
}

void func_002DE2D0(s32 work) {
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

u8 *func_002DE338(source)
const u8 *source;
{
    u8 *effect = (u8 *)func_00328D68(0x58);
    memset(effect, 0, 0x58);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    memcpy(effect + 0x18, source, 0x40);
    return effect;
}

void func_002DE408(void) {
    u64 resource;

    resource = fileResolvePrimaryBuffer();
    func_002DE338(resource);
}

void func_002DE428(void) {
    func_00328E48();
}

void func_002DE440(s32 work) {
    func_002DE338(work + 0x18);
}

void func_002DE458(s32 work) {
    *(u32 *)(work + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE460);

void func_002DEB10(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002DEB20(s32 work, u32 value) {
    *(u32 *)(work + 0x14) = value;
}

void func_002DEB28(s32 work) {
    u32 count;
    u32 *entries;
    u32 index;

    index = 0;
    count = *(u32 *)(work + 0x4c);
    entries = *(u32 **)(work + 8);
    if (count != 0) {
        do {
            index = index + 1;
            *entries = 0xffffffff;
            entries = entries + 2;
        } while (index < count);
    }
    memset(*(u32 *)(work + 0x10), 0, count << 3);
}

void func_002DEB80() {
    func_00316528();
}

void func_002DEB98(void) {
    func_00316648();
}

void func_002DEBB0(void) {
    func_00316668();
}

void func_002DEBC8(s32 p) {
    func_002DEB80(p + 0x14);
}

void func_002DEBE0(s32 *p) {
    func_002DEB28((s32)p);
    *p = 0;
}

void func_002DEC08() {
    if ((D_00437E08 & 2) == 0) {
        func_00316680();
        return;
    }
}

void func_002DEC38() {
    func_00316C88();
}

void func_002DEC50(s32 p) {
    func_002DEC08(p);
    func_002DEC38(p);
}

void func_002DEC78(s32 work, u32 value) {
    *(u32 *)(work + 4) = value;
}

u32 *effDuplicatePayloadHeader(src)
    void *src;
{
    u32 *p = (u32 *)func_00328D68(0x14);
    p[0] = 0;
    memcpy(p + 1, src, 16);
    return p;
}

void func_002DECD0(void) {
    u64 resource;

    resource = fileResolvePrimaryBuffer();
    effDuplicatePayloadHeader(resource);
}

void func_002DECF0(u32 allocation) {
    evtDestroySelectionState();
    func_00328E48(allocation);
}

void func_002DED18(s32 work) {
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
extern f32 func_002D2EB8(f32);
extern void func_0018E0D0(void *);
extern void func_0018ECD0(void *);
extern void func_0018F1D0(void *);
extern void func_0018F5C0(void *);
extern void func_0018F840(void *);

typedef struct EffFadeOut {
    u32 color;    // 0x00
    u32 param;    // 0x04
    u32 unk8;     // 0x08
    u32 unkC;     // 0x0C
    s32 unk10;    // 0x10
    s32 unk14;    // 0x14
} EffFadeOut;

typedef struct EffFadeConfig {
    u8 pad_00[0xB8];
    s32 progress; // 0xB8
    u8 pad_BC[4];
    EffFadeOut out; // 0xC0
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
} EffMapOutWide;

typedef struct EffFadeWork {
    u8 pad_00[0x10];
    u32 param;      // 0x10
    u32 baseColor;  // 0x14
    f32 scale;      // 0x18
    u8 pad_1C[4];
    s32 frameLimit; // 0x20
    EffMapOut *map; // 0x24
    EffFadeConfig *config; // 0x28
} EffFadeWork;

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
    s32 unk24;
} EffRateOut;

typedef struct EffRateConfig {
    u8 pad_00[0xB8];
    s32 progress; // 0xB8
    u8 fixedMode; // 0xBC
    u8 pad_BD[3];
    EffRateOut out; // 0xC0
} EffRateConfig;

void effUpdateFadeBlendA(EffFadeWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->config;
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
        limit = work->frameLimit;
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
    second = func_002D7458((u8 *)config, (u8 *)config + 0x24, limit, progress);
    color1[0] = work->baseColor;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->rateA = func_002D7770((u8 *)config + 0x34, limit, progress) * 0.01f + 1.0f;
    out->rateB = func_002D7770((u8 *)config + 0x60, limit, progress) * 0.01f;
    out->param = work->param;
    func_0018E0D0(out);
}

void func_002DEF00(s32 work) {
    func_0018E850(work + 0xc0);
}

void func_002DEF18(void) {
    func_0018E8F0();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEF30);

void func_002DF138(s32 work, u32 value) {
    *(u32 *)(*(s32 *)(work + 0x24) + 0x2c) = value;
}

void func_002DF148(s32 work) {
    func_0018EBC8(work + 0xc0);
}

void func_002DF160(void) {
    effBlurReleaseFirstResource();
}

void effUpdateFadeMapA(EffFadeWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->config;
    EffMapOut *out = work->map;
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
        limit = work->frameLimit;
    }
    if (progress < limit) {
        return;
    }
    rate = func_002D7770((u8 *)config + 0x8C, limit, progress);
    if (config->fixedMode != 0) {
        out->mode = (s32)rate;
        out->posX = 0;
        out->posY = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)func_002D2EB8(rate);
        out->mode = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF(vf10, pos);
        out->posX = (s32)pos[0] - 0x800;
        out->posY = ((s32)pos[1] - 0x800) << 1;
    }
    second = func_002D7458((u8 *)config, (u8 *)config + 0x24, limit, progress);
    color1[0] = work->baseColor;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->rateA = func_002D7770((u8 *)config + 0x34, limit, progress) * 0.01f;
    out->rateB = func_002D7770((u8 *)config + 0x60, limit, progress) * 0.01f;
    out->param = work->param;
    func_0018ECD0(out);
}

void func_002DF348(s32 work, u32 value) {
    *(u32 *)(*(s32 *)(work + 0x24) + 0x2c) = value;
}

void func_002DF358(s32 work) {
    func_0018F098(work + 0xc0);
}

void func_002DF370(void) {
    effBlurReleaseSecondResource();
}

void effUpdateFadeMapB(EffFadeWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->config;
    EffMapOutWide *out = (EffMapOutWide *)work->map;
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
        limit = work->frameLimit;
    }
    if (progress < limit) {
        return;
    }
    rate = func_002D7770((u8 *)config + 0x8C, limit, progress);
    if (config->fixedMode != 0) {
        out->mode = (s32)rate;
        out->posX = 0;
        out->posY = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        VU0_LOAD_VF(vf10, work);
        mode = (s32)func_002D2EB8(rate);
        out->mode = mode;
        if (mode == 0) {
            return;
        }
        VU0_STORE_VF(vf10, pos);
        out->posX = (s32)pos[0] - 0x800;
        out->posY = ((s32)pos[1] - 0x800) << 1;
    }
    second = func_002D7458((u8 *)config, (u8 *)config + 0x24, limit, progress);
    color1[0] = work->baseColor;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->rateA = func_002D7770((u8 *)config + 0x34, limit, progress) * 0.01f;
    out->rateB = func_002D7770((u8 *)config + 0x60, limit, progress) * 0.01f;
    out->param = work->param;
    func_0018F1D0(out);
}

void func_002DF558(s32 work, u32 value) {
    *(u32 *)(*(s32 *)(work + 0x24) + 0x2c) = value;
}

void effUpdateFadeBlendB(EffFadeWork *work) {
    EffRateConfig *config = (EffRateConfig *)work->config;
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
        limit = work->frameLimit;
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
    second = func_002D7458((u8 *)config, (u8 *)config + 0x24, limit, progress);
    color1[0] = work->baseColor;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->rateA = func_002D7770((u8 *)config + 0x34, limit, progress) + 1.0f;
    out->rateB = func_002D7770((u8 *)config + 0x60, limit, progress);
    out->param = work->param;
    func_0018F5C0(out);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF6C8);

void effUpdateFadeBlendC(EffFadeWork *work) {
    EffFadeConfig *config = work->config;
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
        limit = work->frameLimit;
    }
    if (progress < limit) {
        return;
    }
    out->unk8 = 0;
    out->unkC = 0;
    out->unk10 = 0x200;
    out->unk14 = 0x1C0;
    second = func_002D7458((u8 *)config, (u8 *)config + 0x24, limit, progress);
    color1[0] = work->baseColor;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    out->color = blended[0];
    out->param = work->param;
    func_0018F840(out);
}

void func_002DFA80(s32 work) {
    func_0018FBF8(work + 0xc0);
}

void func_002DFA98(void) {
    func_0018FC88();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFAB0);

void func_002DFC78(s32 work, u32 value) {
    *(u32 *)(*(s32 *)(work + 0x24) + 0x24) = value;
}

EffKindWork *func_002DFC88(u16 kind, u8 *source) {
    u32 headerSize = 0x30;
    u32 size = D_003E9810[kind].size;
    EffKindWork *work = func_00328E18(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(work) : "memory");
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (*(s32 *)(source + 0x28)) {
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

extern s32 *func_002E0378(s32 *, u16);

EffKindWork *func_002DFDB8(u8 *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = func_002DFC88(*(u16 *)(work + 0xC), source);
    s32 *secondary = (s32 *)fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_003E9810[effect->kind].initialize != NULL) {
            s32 *child = func_002E0378(secondary, *(u16 *)(work + 0x1C));
            effect->target = child;
            D_003E9810[effect->kind].initialize((s32)effect, child[2]);
        }
    }
    return effect;
}

typedef struct Dispatch20 {
    void (*callback)();
    u8 reserved[16];
} Dispatch20;

extern Dispatch20 D_003E9814[];

extern Dispatch20 D_003E9818[];

extern void effKindAssetReferenceRelease(s32 *);

void func_002DFE70(s32 *work) {
    s32 object = work[0x24 / 4];
    if (object != 0) {
        D_003E9814[work[0x1C / 4]].callback(object);
    }
    if (work[0x2C / 4] != 0) {
        effKindAssetReferenceRelease((s32 *)work[0x2C / 4]);
    }
    func_00328E48(work);
}

s32 func_002DFED8(s32 *source) {
    s32 *copy = (s32 *)func_002DFC88(*(u16 *)((u8 *)source + 0x1C), source[0x28 / 4]);
    if (source[0x2C / 4] != 0 && D_003E9810[copy[0x1C / 4]].initialize != NULL) {
        s32 *child = (s32 *)func_002E0460(source[0x2C / 4]);
        s32 parameter = child[2];
        copy[0x2C / 4] = (s32)child;
        D_003E9810[copy[0x1C / 4]].initialize((s32)copy, parameter);
    }
    return (s32)copy;
}

void effKindWorkFrameReset(s32 work) {
    ((EffKindWork *)work)->frame = 0;
}

void effKindWorkFrameUpdate(s32 *work) {
    D_003E9818[((EffKindWork *)work)->kind].callback();
    if ((D_00437E08 & 2) == 0) {
        ((EffKindWork *)work)->frame++;
    }
}

void func_002DFFE0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002DFFF0(s32 work, u32 value) {
    ((EffKindWork *)work)->color = value;
}

void func_002DFFF8(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

EffKindWork *func_002E0000(u16 kind, u8 *source) {
    u32 headerSize = 0x30;
    u32 size = D_003E98A0[kind].size;
    EffKindWork *work = func_00328E18(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(work) : "memory");
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (*(s32 *)(source + 0x28)) {
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

EffKindWork *func_002E0130(u8 *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = func_002E0000(*(u16 *)(work + 0xC), source);
    s32 *secondary = (s32 *)fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_003E98A0[effect->kind].initialize != NULL) {
            s32 *child = func_002E0378(secondary, *(u16 *)(work + 0x1C));
            effect->target = child;
            D_003E98A0[effect->kind].initialize((s32)effect, child[2]);
        }
    }
    return effect;
}

extern Dispatch20 D_003E98A4[];

void func_002E01E8(s32 *work) {
    s32 object = work[0x24 / 4];
    if (object != 0) {
        D_003E98A4[work[0x1C / 4]].callback(object);
    }
    if (work[0x2C / 4] != 0) {
        effKindAssetReferenceRelease((s32 *)work[0x2C / 4]);
    }
    func_00328E48(work);
}

s32 func_002E0250(s32 *source) {
    s32 *copy = (s32 *)func_002E0000(*(u16 *)((u8 *)source + 0x1C), source[0x28 / 4]);
    if (source[0x2C / 4] != 0 && D_003E98A0[copy[0x1C / 4]].initialize != NULL) {
        s32 *child = (s32 *)func_002E0460(source[0x2C / 4]);
        s32 parameter = child[2];
        copy[0x2C / 4] = (s32)child;
        D_003E98A0[copy[0x1C / 4]].initialize((s32)copy, parameter);
    }
    return (s32)copy;
}

void effAlternateKindWorkFrameReset(s32 work) {
    ((EffKindWork *)work)->frame = 0;
}

extern Dispatch20 D_003E98A8[];

void effAlternateKindWorkFrameUpdate(s32 *work) {
    D_003E98A8[((EffKindWork *)work)->kind].callback();
    if ((D_00437E08 & 2) == 0) {
        ((EffKindWork *)work)->frame++;
    }
}

void func_002E0358(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E0368(s32 work, u32 value) {
    ((EffKindWork *)work)->color = value;
}

void func_002E0370(s32 object, f32 value) {
    ((EffKindWork *)object)->scale = value;
}

extern s32 func_0032C138(s32 *);

extern s32 func_00159BB8(s32);

s32 *func_002E0378(s32 *source, u16 kind) {
    s32 *object = func_00328E18(0xC);
    object[0] = kind;
    object[1] = 1;
    switch (kind) {
    case 1:
        object[2] = func_0032C138(source);
        break;
    case 4:
        object[2] = func_00159BB8(*source);
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
        func_00328E48(object);
    }
}

u32 func_002E0460(u32 work) {
    *(s32 *)((s32)work + 4) = *(s32 *)((s32)work + 4) + 1;
    return work;
}

u8 *effCreateBillboardWork(u8 *source) {
    u8 *work = (u8 *)func_00328D68(0x68);
    memset(work, 0, 0x68);
    *(s32 *)(work + 0x28) = 0;
    *(u32 *)(work + 0x24) = 0x80808080;
    *(float *)(work + 0x20) = 1.0f;
    __asm__ volatile("sqc2 $vf0, 0(%0)" :: "r"(work) : "memory");
    __asm__ volatile("sqc2 $vf0, 0(%0)" :: "r"(work + 0x10) : "memory");
    if (source == NULL) {
        return work;
    }
    memcpy(work + 0x2C, fileResolvePrimaryBuffer(source),
           *(u32 *)(source + 0x14));
    *(s32 *)(work + 0x64) =
        billCreateIndexed(1, fileResolveSecondaryBuffer(source));
    return work;
}

void effBillboardWorkRelease(u32 work) {
    s32 billboard;

    billboard = *(s32 *)((s32)work + 100);
    if (billboard != 0) {
        billDispatchByKind(billboard);
    }
    func_00328E48(work);
}

u8 *effDuplicateBillState(const u8 *source) {
    u8 *effect = (u8 *)effCreateBillboardWork(NULL);
    memcpy(effect + 0x2C, source + 0x2C, 0x38);
    func_002E0618((s32)effect, (s32)source);
    return effect;
}

void func_002E0618(s32 dst, s32 src) {
    u32 billboard;

    if (*(s32 *)(dst + 100) != 0) {
        billDispatchByKind(*(s32 *)(dst + 100));
    }
    billboard = func_00159A50(*(u32 *)(src + 100));
    *(u32 *)(dst + 100) = billboard;
}

void effBillboardEntryFrameReset(s32 work) {
    billSetEntryFrameMode1(*(u32 *)(work + 100), 0);
    *(u32 *)(work + 0x28) = 0;
}

typedef struct EffBillPlayback {
    u8 vec[0x10];    // 0x00
    u8 orient[0x10]; // 0x10
    f32 scale;       // 0x20
    u8 pad24[4];
    s32 frame;       // 0x28
    f32 speedX;      // 0x2C
    f32 speedY;      // 0x30
    u8 pad34[0x30];
    u32 bill;        // 0x64
} EffBillPlayback;

void func_002E0698(EffBillPlayback *work) {
    u128 dir;
    f32 angle;
    f32 len;

    if (work->frame < billGetFirstEntryFramePeriod(work->bill)) {
        billSetEntryFrameMode1(work->bill, work->frame);
        VU0_LOAD_VF(vf10, work->orient);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0042BC10);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, &dir);
        angle = func_00159FF8(work, &dir);
        len = func_0015A150(work, &dir);
        if (len < 0.3f) {
            len = 0.3f;
        }
        len *= work->scale;
        func_00159BF0(work->bill, len * work->speedY, work->speedX * work->scale);
        func_00159BE8(work->bill, angle);
        effCopyVector(work->bill, work);
        billInvokeCallback(work->bill);
        work->frame++;
    }
}

void func_002E0790(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E07A0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002E07B8(s32 work, u32 value) {
    *(u32 *)(work + 0x24) = value;
}

void func_002E07C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

/* DDS1/DDS2 frame asset: storage at +0x1C, count at +0x08. */
typedef struct EffFrameAsset {
    u8 pad_00[8];
    u32 frameCount;
    u8 pad_0C[0x10];
    void *frameStorage;
    f32 *transformRows;
    u32 pad_24;
    u32 *colorRows;
} EffFrameAsset;

void effClearBillFrames(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

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
    u32 count = *(u32 *)(config + 0x38);
    u32 headerSize = 0x10;
    u8 *base = func_003292A8(count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    *(u32 *)(node + 4) = func_002E5C50(count, 0, handle);
    return node;
}

/* Shared billboard work prefix: retained references and the backing allocation. */
typedef struct EffBillOwnedWork {
    u32 reserved;
    u32 references; /* 0x04 */
    u32 allocation; /* 0x08 */
} EffBillOwnedWork;

void func_002E08D0(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0900);

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
    u8 pad_3C[0x1A];
    u8 mode;            // 0x56
    u8 pad_57[0x21];
    f32 fadeInEnd;      // 0x78, ramp-up length as a fraction of `resourceId`
    f32 fadeOutStart;   // 0x7C, start of the ramp-down
    u8 pad_80[0xC];
    u32 resourceId;     // 0x8C
    u8 pad_90[0x29];
    u8 alternateMode;   // 0xB9, animation variants use this instead of mode
} EffBillConfig;

typedef struct EffBillOutput {
    u32 textureId;      // 0x00
    u32 color;          // 0x04
    u32 field_08;       // 0x08
    u8 pad_0C[8];
    u8 mode;            // 0x14
} EffBillOutput;

void func_002E0F30(BillCellDrawWork *work) {
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
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void billResetCellIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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
    u8 *base = func_003292A8(count * 0x1C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    ((EffBillOwnedWork *)node)->references = func_002E5C50(count, 1, handle);
    return node;
}

void func_002E11A0(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E11D0);

void func_002E1908(BillCellDrawWork *work) {
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
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void billResetParticleIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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
    u8 *base = func_003292A8(count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    ((EffBillOwnedWork *)node)->references = func_002E5C50(count, 1, handle);
    return node;
}

void func_002E1B80(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1BB0);

void func_002E22C8(BillCellDrawWork *work) {
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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void effClearAnimatedFrames(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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

u8 *func_002E24A8(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x18 + headerSize);
    u8 *node = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *entries = node + headerSize;

    *(u8 **)(node + 8) = base;
    *(u8 **)node = entries;
    if (*(u32 *)(config + 0x70) == 0) {
        *(u32 *)(config + 0x70) = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)*(u8 **)(work + 4))->transformRows;
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

extern u8 *func_002E24A8();
u8 *billCreateAnimatedTransform(u8 *descriptor, s32 handle) {
    u8 *work = func_002E24A8(descriptor);

    ((EffBillOwnedWork *)work)->references = func_002E5C50(((EffBillConfig *)descriptor)->frames.count, 3, handle);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

u8 *billCloneAnimatedTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = func_002E24A8(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

void func_002E2670(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E26A0);

void func_002E2C48(BillCellDrawWork *work) {
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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void billResetEmitterIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeEmitterRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)*(u8 **)(work + 4))->transformRows;
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

    ((EffBillOwnedWork *)work)->references = func_002E5C50(((EffBillConfig *)descriptor)->frames.count, 4, handle);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

u8 *billCloneEmitterTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = billAllocEmitterNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

void func_002E2FD8(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3008);

void func_002E35F0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
    u32 *list = *(u32 **)(work + 0x30);
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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(((BillCellDrawWork *)work)->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void effClearStripFrames(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x28 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeStripRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)*(u8 **)(work + 4))->transformRows;
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

    ((EffBillOwnedWork *)work)->references = func_002E5C50(((EffBillConfig *)descriptor)->frames.count, 3, handle);
    billInitializeStripRows(work, descriptor);
    return work;
}

u8 *billCloneStripTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = billAllocStripNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    billInitializeStripRows(work, descriptor);
    return work;
}

void func_002E3980(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E39B0);

void func_002E4070(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
    u32 *list = *(u32 **)(work + 0x30);
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
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(((BillCellDrawWork *)work)->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void billResetTrailIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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
    u8 *base = func_003292A8(*(u32 *)(config + 0x38) * 0x2C + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    ((EffBillOwnedWork *)node)->references = func_002E5C50(*(u32 *)(config + 0x38), 0, handle);
    return node;
}

void func_002E42F0(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4320);

void func_002E4AC8(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
    u32 *list = *(u32 **)(work + 0x30);
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
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(((BillCellDrawWork *)work)->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

void billResetQuadIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = ((EffBillConfig *)*(u8 **)(owner + 0x34))->frames.signedCount;

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
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x20 + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    *(u8 **)(node + 8) = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeQuadRows(u8 *work, u8 *descriptor) {
    u32 count = ((EffBillConfig *)descriptor)->frames.count;
    if (count != 0) {
        f32 *row = ((EffFrameAsset *)*(u8 **)(work + 4))->transformRows;
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

    ((EffBillOwnedWork *)work)->references = func_002E5C50(((EffBillConfig *)descriptor)->frames.count, 4, handle);
    billInitializeQuadRows(work, descriptor);
    return work;
}

u8 *billCloneQuadTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = billAllocQuadNode(descriptor);

    ((EffBillOwnedWork *)work)->references = effDuplicateResourceRefs(((EffBillOwnedWork *)source)->references);
    billInitializeQuadRows(work, descriptor);
    return work;
}

void func_002E4E50(s32 work) {
    effReleaseResourceRefs(((EffBillOwnedWork *)work)->references);
    func_003297C8(((EffBillOwnedWork *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4E80);

void func_002E5540(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
    u32 *list = *(u32 **)(work + 0x30);
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
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->mode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(((BillCellDrawWork *)work)->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002E5E88(out, mtx);
}

u8 *func_002E56A8(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9968[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    *(u8 **)(effect + 0x34) = effect + headerSize;
    *(u32 *)(effect + 0x24) = 0x80808080;
    *(float *)(effect + 0x20) = 1.0f;
    *(u32 *)(effect + 0x2C) = kind;
    *(u32 *)(effect + 0x28) = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(*(void **)(effect + 0x34), source, size);
    return effect;
}

typedef struct EffInstance {
    u8 pad_00[0x30];
    void *resourceHandle;
} EffInstance;

u8 *effCreateResourceInstanceA(u16 kind, void *source, u32 extra) {
    EffInstance *work = (EffInstance *)func_002E56A8(kind, source);
    work->resourceHandle = D_003E9950[kind].init(source, extra);
    D_003E9950[kind].fn(work);
    return (u8 *)work;
}

u8 *effCreateFileResourceInstance(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer(work);
    void *source;
    switch (*(u16 *)(work + 0x1C)) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(work);
    return effCreateResourceInstanceA(*(u16 *)(work + 0xC), source, (u32)secondary);
}

extern EffOp28 D_003E9958[];

void effDispatchDestroyOp(u32 *obj) {

    D_003E9958[obj[0x2C / 4]].run(obj[0x30 / 4]);
    func_00328E48(obj);
}

typedef struct EffActiveInstance {
    u8 pad_00[0x2C];
    s32 kind;
    void *resource;
    void *source;
} EffActiveInstance;

u8 *effCreateActiveResource(EffActiveInstance *obj) {
    EffActiveInstance *work;

    if (D_003E9950[obj->kind].duplicate == NULL) {
        work = (EffActiveInstance *)effCreateResourceInstanceA((u16)obj->kind, obj->source, 0);
    } else {
        work = (EffActiveInstance *)func_002E56A8((u16)obj->kind, obj->source);
        work->resource = D_003E9950[obj->kind].duplicate(obj);
        D_003E9950[obj->kind].fn(work);
    }
    return (u8 *)work;
}

void func_002E5978(u8 *work) {
    D_003E9950[((EffActiveInstance *)work)->kind].fn();
    *(u32 *)(work + 0x28) = 0;
}

extern FnTbl28 D_003E9960[];

void func_002E59C0(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9960[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002E5A20(s32 work) {
    D_003E9964[*(s32 *)(work + 0x2C)].fn((void *)work);
}

void func_002E5A58(u32 work) {
    func_002E59C0();
    func_002E5A20(work);
}

void func_002E5A80(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E5A90(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002E5AA8(s32 work, u32 value) {
    *(u32 *)(work + 0x24) = value;
}

void func_002E5AB0(Matrix4 *mat, float value) {
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
    void *unk18;
    u8 *buffer;    // 0x1C
    u8 *columns;   // 0x20
    u8 *tail;      // 0x24
    s32 *handle;   // 0x28
    u8 *allocation; // 0x2C
} EffTrackSet;

extern s32 *func_003335E0(void);
extern void func_003332D0(void *, f32);
extern u16 D_004582B0[];
extern s32 D_00437E58[2];
extern void *D_00437E60[2];

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BC10);

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
    base = func_003292A8(size + 0x30);
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
    set->unk18 = 0;
    set->handle = func_003335E0();
    func_003332D0(set->handle, 1.0f);
    memset(D_004582B0, 0, 0x2C);
    D_004582B0[2] = 0x4000;
    return set;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5C50);

/* Common header of class-dispatched billboard/resource work (0x40-byte prefix). */
typedef struct EffClassWork {
    u8 transform[0x20]; // 0x00, two VU0 vectors
    f32 scale;           // 0x20
    u32 color;           // 0x24
    u32 frame;           // 0x28
    s32 kind;            // 0x2C
    u32 resource;        // 0x30
    void *payload;       // 0x34
    u8 pad_38[8];
} EffClassWork;

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

typedef struct EffRingBuffer {
    u8 pad_00[8];
    s32 wordCount;      // 0x08, four words per segment
    u8 pad_0C[8];
    u32 *entries;       // 0x14
} EffRingBuffer;


typedef struct EffResourceRefs {
    u8 pad_00[0xC];
    u16 kind;
    u8 pad_0E[0xA];
    void *shared;
    u8 pad_1C[4];
    s32 owned;
    u8 pad_24[4];
    u32 asset;
    u32 buffer;
} EffResourceRefs;


void effReleaseResourceRefs(EffResourceRefs *work) {
    s32 *count;
    void **slot;

    if (work->owned != 0) {
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
    sdfQueueAssetRelease(work->asset);
    func_003297C8(work->buffer);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effDuplicateResourceRefs);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5E88);

extern u32 D_00437E48[2];

void effLoadFlashTextures(void) {
    D_00437E48[0] = func_00343ED0("/effect/flash00.tmx", &D_00437E50, 0);
    D_00437E48[1] = func_00343ED0("/effect/flash01.tmx", &D_00437E50 + 1, 0);
}

u32 func_002E63F0(s32 index) {
    return (&D_00437E50)[index];
}

void func_002E6408(s32 work) {
    *(u32 *)(**(s32 **)(work + 0x30) + 4) = 0;
}

u32 *effSegmentPointerSet(u8 *work) {
    u32 *handle = func_00328D68(4);
    u32 kind = ((EffRingSource *)work)->segments;
    u8 *ring;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (kind < 3) {
        ((EffRingSource *)work)->segments = 3;
        kind = 3;
    }
    ring = effCreatePointSet4(kind);
    first = ((EffRingSource *)work)->firstColor;
    groups = ((EffRingBuffer *)ring)->wordCount / 4;
    *handle = (u32)ring;
    entry = ((EffRingBuffer *)ring)->entries;
    second = ((EffRingSource *)work)->middleColor;
    third = ((EffRingSource *)work)->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return handle;
}

void func_002E64C0(u32 handle) {
    effAssetQueueRelease(*(u32 *)handle);
    func_00328E48(handle);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E64F0);

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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    if ((packed & 0xFF000000) != 0) {
        ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
        *(u8 *)(out + 0xC) = *(u8 *)(config + 0x3C);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
        effMiscQuaternionToMatrixVU();
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
        __asm__ volatile (
            ".set noreorder\n"
            "mfc1 $2, %0\n"
            "qmtc2.ni $2, vf2\n"
            "vmulx.xyzw vf10, vf10, vf2x\n"
            "vmulx.xyzw vf28, vf28, vf10x\n"
            "vmuly.xyzw vf29, vf29, vf10y\n"
            "vmulz.xyzw vf30, vf30, vf10z\n"
            "lqc2 vf10, 0(%1)\n"
            "vmove.w vf10, vf0\n"
            "vmove.xyzw vf31, vf10\n"
            "sqc2 vf28, 0(%2)\n"
            "sqc2 vf29, 0x10(%2)\n"
            "sqc2 vf30, 0x20(%2)\n"
            "sqc2 vf31, 0x30(%2)\n"
            ".set reorder"
            : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
        func_002E76C8(out, mtx);
    }
}

void func_002E68C8(s32 work) {
    u32 resource;

    resource = *(u32 *)(*(s32 *)(work + 0x30) + 4);
    *(u32 *)(*(s32 *)(*(s32 *)(work + 0x30) + 8) + 4) = 0;
    effInitializeClassFrame(resource);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E68F0);

void func_002E6B30(s32 work) {
    effReleaseResourceRefs(*(u32 *)(work + 8));
    effDestroyClassWork(*(u32 *)(work + 4));
    func_003297C8(*(u32 *)(work + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6B68);

extern void effRunClassPostFrame(s32);

void func_002E6CB8(BillCellDrawWork *work) {
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
    *(u32 *)(out + 4) = packed;
    if (packed & 0xFF000000) {
        EffClassWork *dst = (EffClassWork *)list[1];

        dst->color = work->baseColor;
        dst->scale = work->scale;
        PCP_COPY_VECTOR(dst->transform, work);
        PCP_COPY_VECTOR(dst->transform + 0x10, work->transform);
        effRunClassPostFrame((s32)dst);
        *(u32 *)out = ((EffBillConfig *)config)->textureId;
        out[0x14] = *(u8 *)(config + 0x3C);
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

void func_002E6E60(s32 work) {
    *(u32 *)(**(s32 **)(work + 0x30) + 4) = 0;
}

u32 *effCreateRingHandle(u8 *work) {
    u32 *handle = func_00328D68(4);
    u32 kind = ((EffRingSource *)work)->segments;
    u8 *ring;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (kind < 3) {
        ((EffRingSource *)work)->segments = 3;
        kind = 3;
    }
    ring = effCreatePointSet4(kind);
    first = ((EffRingSource *)work)->firstColor;
    groups = ((EffRingBuffer *)ring)->wordCount / 4;
    *handle = (u32)ring;
    entry = ((EffRingBuffer *)ring)->entries;
    second = ((EffRingSource *)work)->middleColor;
    third = ((EffRingSource *)work)->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return handle;
}

void func_002E6F18(u32 handle) {
    effAssetQueueRelease(*(u32 *)handle);
    func_00328E48(handle);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6F48);

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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->color = blended[0];
    if ((packed & 0xFF000000) != 0) {
        ((EffBillOutput *)out)->textureId = ((EffBillConfig *)config)->textureId;
        *(u8 *)(out + 0xC) = *(u8 *)(config + 0x3C);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
        effMiscQuaternionToMatrixVU();
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
        __asm__ volatile (
            ".set noreorder\n"
            "mfc1 $2, %0\n"
            "qmtc2.ni $2, vf2\n"
            "vmulx.xyzw vf10, vf10, vf2x\n"
            "vmulx.xyzw vf28, vf28, vf10x\n"
            "vmuly.xyzw vf29, vf29, vf10y\n"
            "vmulz.xyzw vf30, vf30, vf10z\n"
            "lqc2 vf10, 0(%1)\n"
            "vmove.w vf10, vf0\n"
            "vmove.xyzw vf31, vf10\n"
            "sqc2 vf28, 0(%2)\n"
            "sqc2 vf29, 0x10(%2)\n"
            "sqc2 vf30, 0x20(%2)\n"
            "sqc2 vf31, 0x30(%2)\n"
            ".set reorder"
            : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
        func_002E76C8(out, mtx);
    }
}

u8 *effPayloadPointerSet(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9B94[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect));
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10));
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = D_003E9B80[kind].createResource(source);
    D_003E9B80[kind].fn(effect);
    return effect;
}

void effCreateClassWorkFromFile(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effPayloadPointerSet(*(u16 *)(request + 0xc), source);
}

extern EffOp24 D_003E9B88[];

void effDestroyClassWork(u32 *obj) {
    D_003E9B88[obj[0x2C / 4]].run(obj[0x30 / 4]);
    func_00328E48(obj);
}

void effCreateClassWorkFromRequest(s32 work) {
    effPayloadPointerSet(*(u16 *)(work + 0x2c), ((EffClassWork *)work)->payload);
}

void effInitializeClassFrame(u8 *work) {
    D_003E9B80[((EffClassWork *)work)->kind].fn();
    ((EffClassWork *)work)->frame = 0;
}

void effAdvanceClassFrame(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9B8C[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void effRunClassPostFrame(s32 work) {
    D_003E9B90[((EffClassWork *)work)->kind].fn((void *)work);
}

void effUpdateClassFrame(u32 work) {
    effAdvanceClassFrame();
    effRunClassPostFrame(work);
}

void func_002E7590(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E75A0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002E75B8(s32 work, u32 value) {
    ((EffClassWork *)work)->color = value;
}

void func_002E75C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

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

extern u16 D_004582E0[];

u8 *effCreatePointSet4(u32 count) {
    s32 rows = count * 4 + 4;
    s32 size = rows * 20;
    u8 *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = func_003292A8(size + 0x20);
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
    set->handle = func_003335E0();
    func_003332D0(set->handle, 1.0f);
    memset(D_004582E0, 0, 0x2C);
    D_004582E0[2] = 0x4000;
    return (u8 *)set;
}

/* Shared asset and allocation handles released by the effect cleanup callbacks. */
typedef struct EffAssetOwner {
    u8 pad_00[0x18];
    u32 asset;      /* 0x18 */
    u32 allocation; /* 0x1C */
} EffAssetOwner;

void effAssetQueueRelease(s32 work) {
    sdfQueueAssetRelease(((EffAssetOwner *)work)->asset);
    func_003297C8(((EffAssetOwner *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E76C8);

typedef struct EffectSlotNode54 {
    u32 count;
    u32 color;
    f32 opacity;
    u8 pad_0C[0x20];
    u32 index;             // 0x2C
    u32 handleBuffer;      // 0x30
    u32 billResource;      // 0x34
    u32 *jobs;             // 0x38
    u32 jobBuffer;         // 0x3C
    u32 *queues;           // 0x40
    u32 queueBuffer;       // 0x44
    u32 resourceHolder;    // 0x48
    u32 record;            // 0x4C
    u16 active;            // 0x50
} EffectSlotNode54;

EffectSlotNode54 *effCreateSurfaceNode(u32 count) {
    EffectSlotNode54 *node = func_00328E18(sizeof(EffectSlotNode54));
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

    effRebuildSurfaceHandles(object, *(u16 *)(source + 0xC), primary);
    effReplaceResourceRef(object, *(u16 *)(source + 0xC), next);
    return object;
}

extern s32 effCreateSurfaceNodeForGrid(s32);

extern void effRebuildSurfaceHandles(s32, s32, s32);

extern void effReplaceResourceRef(s32, s32, s32);

s32 func_002E7B90(u16 kind, s32 source) {
    s32 next = source + 0x1C;
    s32 object = effCreateSurfaceNodeForGrid(next);
    effRebuildSurfaceHandles(object, kind, source);
    effReplaceResourceRef(object, kind, next);
    return object;
}

extern void func_002E83C8(s32 *, s32 *);

extern void func_002E8448(s32 *, s32 *);

extern void effSetSurfaceRetainedResource(s32 *, s32);

extern void effRebuildSurfaceJobs(EffectSlotNode54 *, void *);

extern void effSurfaceNodeCreateQueues(EffectSlotNode54 *, void *);

extern void effReplaceSurfaceResourceHolder(s32, u32);

void func_002E7C00(s32 *object, s32 kind, s32 *settings) {
    u16 type = kind;
    switch (type) {
    case 1:
        func_002E83C8(object, settings);
        break;
    case 2:
        func_002E8448(object, settings);
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

s32 func_002E7CB8(s32 *source) {
    s32 *object = (s32 *)effResourceReferenceReplaceFromFile((u8 *)source);
    s32 *data = (s32 *)fileResolveSecondaryBuffer(source);
    if (data != NULL) {
        func_002E7C00(object, *(u16 *)((u8 *)source + 0x1C), data);
    }
    return (s32)object;
}

extern void func_002E9A68(s32);
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
        count = *(u32 *)(node->record + 8);
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        func_003297C8(node->jobBuffer);
    }
    if (node->queueBuffer != 0) {
        count = *(u32 *)(node->record + 8);
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        func_003297C8(node->queueBuffer);
    }
    if (node->index != 0) {
        for (i = 0; i < node->count; i++) {
            func_002E9A68(*(u32 *)(node->index + i * 4));
        }
        func_003297C8(node->handleBuffer);
    }
    if (node->resourceHolder != 0) {
        effReleaseReferenceHolder((u32 *)node->resourceHolder);
    }
    if (node->record != 0) {
        func_002DC028(node->record);
    }
    func_00328E48(node);
}

s32 func_002E7E70(u8 *work) {
    u8 *config = *(u8 **)(work + 0x4C);
    s32 arg = *(s32 *)(config + 0x24);
    s32 object = effCreateSurfaceNodeForGrid(arg);

    effRebuildSurfaceHandles(object, **(u16 **)(work + 0x4C), (s32)work + 0x10);
    effReplaceResourceRef(object, **(u16 **)(work + 0x4C), arg);
    return object;
}

u32 func_002E7EE0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x4C);
    s32 arg = *(s32 *)(config + 0x24);
    s32 object = effCreateSurfaceNodeForGrid(arg);

    effRebuildSurfaceHandles(object, **(u16 **)(work + 0x4C), (s32)work + 0x10);
    effReplaceResourceRef(object, **(u16 **)(work + 0x4C), arg);
    func_002E7F60(object, work);
    return object;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7F60);

typedef struct EffMotionSetup {
    u16 mode;       // 0x00
    u16 kind;       // 0x02
    u16 flags;      // 0x04
    u8 pad_06[6];
    void *table;    // 0x0C
    u8 pad_10[0x1C];
} EffMotionSetup;   // 0x2C

/* Surface parameters copied into the node: 0x1C bytes at +0x10. */
typedef struct EffSurfaceParams {
    u32 word[7];
} EffSurfaceParams;

extern u32 func_002E96E8(u32, u32);
extern void func_002E9840(u32, u32 *);

void effRebuildSurfaceHandles(s32 nodeAddr, s32 kind, s32 source) {
    EffectSlotNode54 *node = (EffectSlotNode54 *)nodeAddr;
    u32 *params = (u32 *)source;
    u32 size;
    u32 i;
    u32 handle;

    *(EffSurfaceParams *)((u8 *)node + 0x10) = *(EffSurfaceParams *)params;
    size = node->count * 4;
    if (size == 0) {
        return;
    }
    if (node->index != 0) {
        for (i = 0; i < node->count; i++) {
            func_002E9A68(*(u32 *)(i * 4 + node->index));
        }
        func_003297C8(node->handleBuffer);
    }
    node->handleBuffer = (u32)func_003292A8(size);
    node->index = sdfResourceRetainAddress(node->handleBuffer);
    for (i = 0; i < node->count; i++) {
        handle = func_002E96E8(params[0], params[1]);
        *(u32 *)(i * 4 + node->index) = handle;
        func_002E9840(handle, params + 2);
    }
}

void effReplaceResourceRef(s32 obj, s32 id, s32 arg) {
    u32 *p = (u32 *)obj;
    if (p[0x4C / 4] != 0) {
        func_002DC028(p[0x4C / 4]);
    }
    p[0x4C / 4] = func_002DBED8(id & 0xffff, p[0], arg);
}

void effSetSurfaceRetainedResource(s32 *object, s32 arg) {
    u8 *work = (u8 *)object;
    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = effRetainResource(arg);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, *(s16 *)(*(u8 **)(*(u8 **)(work + 0x4C) + 0x20) + 0x54));
    }
}

void func_002E83C8(s32 *object, s32 *settings) {
    u8 *work = (u8 *)object;

    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = billCreateIndexed(0, settings);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, *(s16 *)(*(u8 **)(*(u8 **)(work + 0x4C) + 0x20) + 0x54));
    }
}

void func_002E8448(s32 *object, s32 *settings) {
    u8 *work = (u8 *)object;

    if (((EffectSlotNode54 *)work)->billResource != 0) {
        billDispatchByKind(((EffectSlotNode54 *)work)->billResource);
    }
    ((EffectSlotNode54 *)work)->billResource = billCreateIndexed(1, settings);
    func_00159FA0(((EffectSlotNode54 *)work)->billResource);
    if (((EffectSlotNode54 *)work)->record != 0) {
        billSetBillboardMode(((EffectSlotNode54 *)work)->billResource, *(s16 *)(*(u8 **)(*(u8 **)(work + 0x4C) + 0x20) + 0x54));
    }
}

void effRebuildSurfaceJobs(EffectSlotNode54 *node, void *source) {
    u32 count = *(u32 *)(node->record + 8);
    u32 i;
    u32 size;

    if (node->jobBuffer != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        func_003297C8(node->jobBuffer);
        node->jobs = 0;
        node->jobBuffer = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobBuffer = (u32)func_003292A8(size);
        node->jobs = (u32 *)sdfResourceRetainAddress(node->jobBuffer);
        node->jobs[0] = fileJobCreateFromJob((u32)source);
        for (i = 1; i < count; i++) {
            node->jobs[i] = fileJobCreateChild(node->jobs[0]);
        }
    }
}

extern u32 func_002D4138(u32);
extern void *fileQueueClone(void *);

void effSurfaceNodeCreateQueues(EffectSlotNode54 *node, void *source) {
    u32 count = *(u32 *)(node->record + 8);
    u32 i;
    u32 size;

    if (node->queueBuffer != 0) {
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        func_003297C8(node->queueBuffer);
        node->queues = 0;
        node->queueBuffer = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueBuffer = (u32)func_003292A8(size);
        node->queues = (u32 *)sdfResourceRetainAddress(node->queueBuffer);
        node->queues[0] = func_002D4138((u32)source);
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

void effFileRecordReferencesClear(s32 node) {
    if (((EffectSlotNode54 *)node)->record != 0) {
        fileClearRecordReferences(((EffectSlotNode54 *)node)->record);
        return;
    }
}

void effAcquireSurfaceRecord(s32 node) {
    if (D_00437E08 & 2) {
        return;
    }
    if (((EffectSlotNode54 *)node)->record != 0) {
        fileAcquireRecord(((EffectSlotNode54 *)node)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8770);

void func_002E9668(u32 node) {
    effAcquireSurfaceRecord(node);
    func_002E8770(node);
}

void effMenuRecordVectorSet(s32 node) {
    mnuRecordSetVector(((EffectSlotNode54 *)node)->record);
}

void func_002E96A8(s32 node) {
    func_002DC0F0(((EffectSlotNode54 *)node)->record);
}

void func_002E96C0(s32 node, u32 color) {
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

u32 func_002E96E8(u32 count, u32 columns) {
    s32 rows = count * columns * 3 + 6;
    s32 size = rows * 20 + 0xA0;
    u8 *base;
    u8 *data;
    EffSurfaceGridNode *node;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = func_003292A8(size + 0x4C);
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
    node->handle = func_003335E0();
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

void func_002E9840(u32 nodeAddr, u32 *colors) {
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

void func_002E9A68(s32 work) {
    sdfQueueAssetRelease(*(u32 *)(work + 0x3c));
    func_003297C8(*(u32 *)(work + 0x48));
}

void func_002E9A98(s32 work) {
    *(u32 *)(work + 0x24) = 0;
    *(u32 *)(work + 0x28) = 3;
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9B20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9BC8);

s32 func_002E9E48(const Matrix4 *matrix) {
    void *work = sdfAllocPacketAligned(0x20);
    D_00437E68 = (u32)work;
    sdfInitPacketList(work);
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 $vf28, 0(%0)\n\t"
        "lqc2 $vf29, 16(%0)\n\t"
        "lqc2 $vf30, 32(%0)\n\t"
        "lqc2 $vf31, 48(%0)\n\t"
        ".set reorder" :: "r"(matrix) : "memory");
    return sdfConsAppendVuPacket(D_00437E68, 0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9E98);

void func_002EA0E8(u32 index) {
    u8 *entry = D_003E9CA8[index];
    ((void (*)(u8 *, u32))*(void **)(entry + 0x10))(entry, D_00437E68);
    D_00437E68 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA120);

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
    func_00328E48(p);
}

u8 *func_002EA410(u8 *source) {
    u8 *effect = func_002EA120(NULL);
    memcpy(effect + 0x30, source + 0x30, 0x98);
    func_002EA530(effect, source);
    return effect;
}

void func_002EA530(u32 *dst, u32 *src) {
    if (((EffRenderResourceState *)src)->billHandle != 0) {
        if (((EffRenderResourceState *)dst)->billHandle != 0) {
            billDispatchByKind(((EffRenderResourceState *)dst)->billHandle);
        }
        ((EffRenderResourceState *)dst)->billHandle = func_00159A50(((EffRenderResourceState *)src)->billHandle);
        func_00159FA0(((EffRenderResourceState *)dst)->billHandle);
        billSetBillboardMode(((EffRenderResourceState *)dst)->billHandle, ((EffRenderResourceState *)dst)->billMode);
    } else {
        if (((EffRenderResourceState *)dst)->reference != NULL) {
            effReleaseReferenceHolder(((EffRenderResourceState *)dst)->reference);
        }
        ((EffRenderResourceState *)dst)->reference = effReferenceObjectRetain(((EffRenderResourceState *)src)->reference);
    }
}

void func_002EA5D0(s32 work) {
    ((EffClassWork *)work)->kind = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA5D8);

void func_002EAB78(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002EAB88(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002EABA0(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void func_002EABA8(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

extern s32 D_0037F550[4];

extern s32 effMiscRand(s32 *);

void effRandomizeParticleFields(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 4) {
        entry[1] = -1 - (effMiscRand(D_0037F550) & 3);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAC38);

extern void func_002EE0A8(s32);

void effFreeIndexedEntries(u8 *work) {
    u32 *header = *(u32 **)(work + 0x30);
    u32 count = ((EffBillConfig *)*(u8 **)(work + 0x34))->frames.count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        func_002EE0A8(*entry);
        entry += 4;
    }
    func_00328E48(header);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB058);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB728);

extern void effResetDispatchCounter(u8 *);

void func_002EB908(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++) {
        effResetDispatchCounter((u8 *)*entry++);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB968);

extern void func_002EDE10(s32);

void func_002EBB88(u8 *work) {
    u32 *header = *(u32 **)(work + 0x30);
    u32 count = ((EffBillConfig *)*(u8 **)(work + 0x34))->frames.count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        func_002EDE10(*entry++);
    }
    func_00328E48(header);
}

extern void func_002EDEC0();

void effReleaseTrackEntriesA(u8 *work) {
    u32 count = ((EffBillConfig *)*(u8 **)(work + 0x34))->frames.count;
    u32 **entry = *(u32 ***)*(u8 **)(work + 0x30);
    u32 i;

    for (i = 0; i < count; i++) {
        func_002EDEC0(*entry++);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBC58);

typedef struct EffScaleRange {
    u8 *entries;
    f32 start;
    f32 delta;
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

void func_002EBE28(u8 *work) {
    EffScaleRangeConfig *config = *(EffScaleRangeConfig **)(work + 0x34);
    EffScaleRange *range = *(EffScaleRange **)(work + 0x30);
    s32 steps = config->steps;
    u8 *entry = range->entries;
    f32 start = config->startBase * (func_00341240(D_0037F550) * config->startRand + (1.0f - config->startRand));
    u32 index;
    u32 count;

    if (steps > 0) {
        f32 end = config->endBase * (func_00341240(D_0037F550) * config->endRand + (1.0f - config->endRand));
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
            *(s32 *)(entry + 0x14) = -1 - (effMiscRand(D_0037F550) & 7);
            entry += 0x30;
        } while (index < count);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBF40);

void func_002EC300(u8 *work) {
    u32 *header = *(u32 **)(work + 0x30);
    u32 count = ((EffBillConfig *)*(u8 **)(work + 0x34))->frames.count;
    u32 *entry = (u32 *)header[0];
    u32 i;

    for (i = 0; i < count; i++) {
        func_002EE0A8(*entry);
        entry += 12;
    }
    func_003297C8(header[3]);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC370);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECD10);

void func_002ECEF0(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        entry[1] = -1 - (effMiscRand(D_0037F550) & 3);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECF78);

void func_002ED360(s32 *work) {
    u32 count = ((EffBillConfig *)work[0x34 / 4])->frames.count;
    s32 *entries = (s32 *)work[0x30 / 4];
    s32 *entry = (s32 *)*entries;
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        func_002EE0A8(entry[0]);
    }
    func_00328E48(entries);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ED3D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDB10);

u8 *func_002EDCF0(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9D14[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->frame = 0;
    ((EffClassWork *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect + 0x10);
    memcpy(((EffClassWork *)effect)->payload, source, size);
    ((EffClassWork *)effect)->resource = D_003E9D00[kind].createResource(source);
    D_003E9D00[kind].fn(effect);
    return effect;
}

void func_002EDDE0(s32 request) {
    void *source;

    source = fileResolvePrimaryBuffer();
    func_002EDCF0(*(u16 *)(request + 0xc), source);
}

void func_002EDE10(s32 work) {
    u32 *obj = (u32 *)work;
    D_003E9D08[obj[0x2C / 4]].run();
    func_00328E48(obj);
}

u32 effPayloadPointerGet(s32 work) {
    return func_002EDCF0(*(u16 *)(work + 0x2c), ((EffClassWork *)work)->payload);
}

void effResetDispatchCounter(u8 *work) {
    D_003E9D00[((EffClassWork *)work)->kind].fn(work);
    ((EffClassWork *)work)->frame = 0;
}

extern FnTbl24 D_003E9D0C[];

void func_002EDEC0(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9D0C[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002EDF20(s32 work) {
    D_003E9D10[((EffClassWork *)work)->kind].fn((void *)work);
}

void func_002EDF58(u32 work) {
    func_002EDEC0();
    func_002EDF20(work);
}

void func_002EDF80(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002EDF90(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002EDFA8(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void func_002EDFB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

extern u16 D_004583A0[];
extern u16 D_00458430[];

EffPointSet *effCreatePointSet5(s32 count) {
    s32 rows = count * 5 + 5;
    s32 size = rows * 20;
    u8 *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = func_003292A8(size + 0x20);
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
    set->handle = func_003335E0();
    func_003332D0(set->handle, 1.0f);
    memset(D_004583A0, 0, 0x2C);
    D_004583A0[2] = 0x4000;
    return set;
}

void func_002EE0A8(s32 work) {
    sdfQueueAssetRelease(((EffAssetOwner *)work)->asset);
    func_003297C8(((EffAssetOwner *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE0D8);

EffectStripNode *func_002EE348(u32 percent) {
    EffectStripNode *node = func_00328E18(sizeof(EffectStripNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->transform = func_002E5C50(percent * 4, 2, 0);
    node->resource = effRetainResource(0);
    node->count = 1;
    return node;
}

EffectStripNode *func_002EE3C8(u8 *source) {
    return func_002EE348(effSlotCount(source, 100));
}

u8 *effFileResourceReferenceReplace(u8 *work) {
    u8 *primary = fileResolvePrimaryBuffer();
    u8 *source = primary + 0x20;
    u8 *node = (u8 *)func_002EE3C8(source);

    memcpy(node + 0xC, primary, 0x20);
    effReplaceFileResourceRef((s32)node, *(u16 *)(work + 0xC), (s32)source);
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
        func_002DC028(p[0x34 / 4]);
    }
    func_00328E48(p);
}

u8 *func_002EE508(u8 *work) {
    u8 *source = *(u8 **)(*(u8 **)(work + 0x34) + 0x24);
    u8 *node = (u8 *)func_002EE3C8(source);

    memcpy(node + 0xC, source, 0x20);
    effReplaceFileResourceRef((s32)node, **(u16 **)(work + 0x34), (s32)source);
    return node;
}

void effReplaceFileResourceRef(s32 obj, s32 id, s32 arg) {
    u32 *p = (u32 *)obj;
    if (p[0x34 / 4] != 0) {
        func_002DC028(p[0x34 / 4]);
    }
    p[0x34 / 4] = func_002DBED8(id & 0xffff, p[0], arg);
}

void func_002EE608(s32 node) {
    if (((EffectStripNode *)node)->active != 0) {
        fileClearRecordReferences(((EffectStripNode *)node)->active);
        return;
    }
}

void func_002EE638(s32 node) {
    if (D_00437E08 & 2) {
        return;
    }
    if (((EffectStripNode *)node)->active != 0) {
        fileAcquireRecord(((EffectStripNode *)node)->active);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE670);

void func_002EED48(u32 node) {
    func_002EE638(node);
    func_002EE670(node);
}

void func_002EED70(s32 node) {
    mnuRecordSetVector(((EffectStripNode *)node)->active);
}

void func_002EED88(s32 node) {
    func_002DC0F0(((EffectStripNode *)node)->active);
}

void func_002EEDA0(s32 node, u32 color) {
    ((EffectStripNode *)node)->color = color;
}

void func_002EEDA8(u8 *p, f32 value) {
    ((EffectStripNode *)p)->opacity = value;
    dds3DispatchIndexedCallback(((EffectStripNode *)p)->active, value);
}

void effResetBillTable(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x1C);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

u8 *func_002EEE18(u8 *config) {
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x30 + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)node + 0xC;

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

void func_002EEE88(u8 *node, u8 *config) {
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
    table = *(EffectFadeTable **)(node + 4);
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

u32 *func_002EF0D0(u8 *p, u32 a1) {
    u32 *buf = (u32 *)func_002EEE18(p);
    buf[1] = func_002F1740(((EffBillConfig *)p)->frames.count, ((EffBillConfig *)p)->resourceId, a1);
    func_002EEE88(buf, p);
    return buf;
}

u32 *effAssetPointerSet(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)func_002EEE18(dst);
    buf[1] = func_002F1820(src[1]);
    func_002EEE88(buf, dst);
    return buf;
}

void func_002EF190(s32 work) {
    s32 state;

    state = *(s32 *)(work + 0x30);
    effSharedAssetReferenceRelease(*(u32 *)(state + 4));
    func_003297C8(*(u32 *)(state + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF1C0);

void func_002EF8C0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
    u32 *list = *(u32 **)(work + 0x30);
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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->field_08 = blended[0];
    ((EffBillOutput *)out)->color = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->alternateMode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(((BillCellDrawWork *)work)->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002F1888(out, mtx);
}

void func_002EFA28(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x1C);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

u8 *func_002EFA78(u8 *config) {
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x30 + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)node + 0xC;

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        ((EffBillConfig *)config)->resourceId = 3;
    }
    return (u8 *)node;
}

void func_002EFAE8(u8 *node, u8 *config) {
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
    table = *(EffectFadeTable **)(node + 4);
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

u32 *func_002EFD30(u8 *p, u32 a1) {
    u32 *buf = (u32 *)func_002EFA78(p);
    buf[1] = func_002F1740(((EffBillConfig *)p)->frames.count, ((EffBillConfig *)p)->resourceId, a1);
    func_002EFAE8(buf, p);
    return buf;
}

u32 *func_002EFD90(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)func_002EFA78(dst);
    buf[1] = func_002F1820(src[1]);
    func_002EFAE8(buf, dst);
    return buf;
}

void func_002EFDF0(s32 work) {
    s32 state;

    state = *(s32 *)(work + 0x30);
    effSharedAssetReferenceRelease(*(u32 *)(state + 4));
    func_003297C8(*(u32 *)(state + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFE20);

void func_002F0530(BillCellDrawWork *work) {
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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->field_08 = blended[0];
    ((EffBillOutput *)out)->color = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->alternateMode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002F1888(out, mtx);
}

void func_002F0698(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x1C);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x2C;
    }
}

u8 *func_002F06E8(u8 *config) {
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x2C + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)node + 0xC;

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        ((EffBillConfig *)config)->resourceId = 3;
    }
    return (u8 *)node;
}

void func_002F0760(u8 *node, u8 *config) {
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
    table = *(EffectFadeTable **)(node + 4);
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

u32 *func_002F09A8(u8 *p, u32 a1) {
    u32 *buf = (u32 *)func_002F06E8(p);
    buf[1] = func_002F1740(((EffBillConfig *)p)->frames.count, ((EffBillConfig *)p)->resourceId, a1);
    func_002F0760(buf, p);
    return buf;
}

u32 *func_002F0A08(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)func_002F06E8(dst);
    buf[1] = func_002F1820(src[1]);
    func_002F0760(buf, dst);
    return buf;
}

void func_002F0A68(s32 work) {
    s32 state;

    state = *(s32 *)(work + 0x30);
    effSharedAssetReferenceRelease(*(u32 *)(state + 4));
    func_003297C8(*(u32 *)(state + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0A98);

void func_002F10C0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = ((BillCellDrawWork *)work)->frameLimit;
    u32 progress = ((EffBillConfig *)config)->progress;
    u32 *list = *(u32 **)(work + 0x30);
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
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    ((EffBillOutput *)out)->field_08 = blended[0];
    ((EffBillOutput *)out)->color = ((EffBillConfig *)config)->textureId;
    ((EffBillOutput *)out)->mode = ((EffBillConfig *)config)->alternateMode;
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_003E9100));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(((BillCellDrawWork *)work)->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002F1888(out, mtx);
}

u8 *effAllocateBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9DF0[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return effect;
}

extern u8 *effAllocateBlock(u16, void *);

u8 *effCreateResourceInstanceB(u16 kind, void *source, u32 extra) {
    u8 *work = effAllocateBlock(kind, source);
    ((EffClassWork *)work)->resource = (u32)D_003E9DD8[kind].init(source, extra);
    D_003E9DD8[kind].fn(work);
    return work;
}

extern u8 *effCreateResourceInstanceB(u16, void *, u32);

u8 *effCreateFileResourceInstanceB(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer(work);
    void *source;
    switch (*(u16 *)(work + 0x1C)) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(work);
    return effCreateResourceInstanceB(*(u16 *)(work + 0xC), source, (u32)secondary);
}

void func_002F13E0(u32 *obj) {
    D_003E9DE0[obj[0x2C / 4]].run();
    func_00328E48(obj);
}

u8 *effDuplicateActiveResourceB(u8 *obj) {
    u8 *work = effAllocateBlock(*(u16 *)(obj + 0x2C), ((EffClassWork *)obj)->payload);
    *(void **)(work + 0x30) = D_003E9DD8[((EffClassWork *)obj)->kind].duplicate(obj);
    D_003E9DD8[((EffClassWork *)obj)->kind].fn(work);
    return work;
}

void func_002F14B8(u8 *work) {
    D_003E9DD8[((EffClassWork *)work)->kind].fn();
    ((EffClassWork *)work)->frame = 0;
}

void func_002F1500(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9DE8[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002F1560(s32 work) {
    D_003E9DEC[((EffClassWork *)work)->kind].fn((void *)work);
}

void func_002F1598(u32 work) {
    func_002F1500();
    func_002F1560(work);
}

void func_002F15C0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F15D0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F15E8(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void func_002F15F0(Matrix4 *mat, float value) {
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
    u32 resource;       // 0x18
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
    u8 *allocation = func_003292A8(size + 0x34);
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
    work->handle = func_003335E0();
    func_003332D0(work->handle, 1.0f);
    memset(&D_004583D0, 0, sizeof(EffMotionSetup));
    D_004583D0.flags = 0x4000;
    return (u8 *)work;
}

u32 func_002F1740(u32 count, u32 repeat, u32 resource) {
    u8 *node = effCreateRibbonWork(count, repeat);

    if (resource == 0) {
        s32 references = D_00437E74;
        *(u32 *)(node + 0x18) = 0;
        if (references == 0) {
            D_00437E78 = func_002DDCA0(D_00437E70, 0x300);
            references = D_00437E74;
        }
        references++;
        D_00437E74 = references;
    } else {
        *(void **)(node + 0x18) = func_002DDAA8((void *)resource);
    }
    return (u32)node;
}

void effSharedAssetReferenceRelease(s32 work) {
    if (*(s32 *)(work + 0x18) == 0) {
        D_00437E74 = D_00437E74 - 1;
        if (D_00437E74 == 0) {
            effReleaseSharedReference(D_00437E78);
            D_00437E78 = 0;
        }
    }
    else {
        effReleaseSharedReference(*(s32 *)(work + 0x18));
    }
    sdfQueueAssetRelease(*(u32 *)(work + 0x2c));
    func_003297C8(*(u32 *)(work + 0x30));
}

u8 *func_002F1820(u32 *source) {
    u8 *node = effCreateRibbonWork(source[0], source[4]);

    if (source[6] != 0) {
        *(RefObj **)(node + 0x18) = effRetainSharedReference((RefObj *)source[6]);
    } else {
        D_00437E74++;
        *(u32 *)(node + 0x18) = 0;
    }
    return node;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1888);

void effLoadWindTexture(void) {
    D_00437E6C = func_00343ED0("/effect/wind00.tmx", &D_00437E70, 0);
}

u32 func_002F1CC8(void) {
    return D_00437E70;
}

void func_002F1CD0(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = ((EffBillConfig *)b)->frames.count;
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x18);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

u8 *effAllocateAnimationBuffer(u8 *config) {
    u8 *base = func_003292A8(((EffBillConfig *)config)->frames.count * 0x30 + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = ((EffBillConfig *)config)->resourceId;
    u8 *entries = (u8 *)node + 0xC;

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
        u32 *color = ((EffFrameAsset *)*(u8 **)(work + 4))->colorRows;
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
    buf[1] = func_002F3ED8(((EffBillConfig *)src)->frames.count, ((EffBillConfig *)src)->resourceId);
    effFillFadeColorRows(buf, src);
    return buf;
}

u32 *effPrepareOwnedTextureAnimation(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)effAllocateAnimationBuffer(dst);
    buf[1] = func_002F3F40(src[1]);
    effFillFadeColorRows(buf, dst);
    return buf;
}

void func_002F2020(s32 work) {
    s32 state;

    state = *(s32 *)(work + 0x30);
    func_002F3F08(*(u32 *)(state + 4));
    func_003297C8(*(u32 *)(state + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2050);

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
    ((EffMeshOutput *)out)->mode = *(u8 *)(config + 0xDD);
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
    u8 *resource = *(u8 **)(work + 0x30);
    u8 *payload = (u8 *)((EffAnimationState *)resource)->record;
    float *positions = ((EffAnimationState *)resource)->positions;
    u32 count = *(u32 *)(payload + 8);
    u32 i = 0;

    fileClearRecordReferences((s32)payload);
    for (; i < count; i++) {
        positions[0] = func_00341240(D_0037F550);
        positions[1] = func_00341240(D_0037F550);
        positions += 2;
    }
}

u32 effClampSlotCount(u8 *p) {
    return effSlotCount(p, 200);
}

u32 *effCreateAnimationState(u32 unused, u32 count) {
    u32 allocation = (u32)func_003292A8(count * 8 + 0x10);
    u32 *state = (u32 *)sdfResourceRetainAddress(allocation);

    ((EffAnimationState *)state)->allocation = allocation;
    ((EffAnimationState *)state)->positions = (f32 *)(state + 4);
    ((EffAnimationState *)state)->record = 0;
    ((EffAnimationState *)state)->textureHandle = func_002F3D18();
    return state;
}

u32 *effActivateAnimationState(s32 work) {
    s32 owner = *(s32 *)(work + 0x30);
    s32 resource = *(s32 *)(owner + 8);
    u32 *state = effCreateAnimationState(*(u32 *)(work + 0x34), *(u32 *)(resource + 8));

    resource = *(s32 *)(owner + 8);
    ((EffAnimationState *)state)->record = func_002DBED8(*(u16 *)resource, *(u32 *)(resource + 8),
                               *(void **)(resource + 0x24));
    return state;
}

extern void func_002DC028(s32);
extern void func_002F3D58(u32);

void func_002F2A30(u8 *work) {
    u32 *state = *(u32 **)(work + 0x30);

    func_002F3D58(((EffAnimationState *)state)->textureHandle);
    if (((EffAnimationState *)state)->record != 0) {
        func_002DC028(((EffAnimationState *)state)->record);
    }
    func_003297C8(((EffAnimationState *)state)->allocation);
}

void effSynchronizeFileTransform(u8 *work) {
    u32 *record = *(u32 **)(work + 0x30);

    if (((EffAnimationState *)record)->record != 0) {
        mnuRecordSetVector(((EffAnimationState *)record)->record, work);
        func_002DC0F0(((EffAnimationState *)record)->record, work + 0x10);
        dds3DispatchIndexedCallback(((EffAnimationState *)record)->record, *(f32 *)(work + 0x20));
        fileAcquireRecord(((EffAnimationState *)record)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2AE8);

u32 *func_002F3100(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = effClampSlotCount(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    ((EffAnimationState *)state)->record = func_002DBED8(1, count, mapping);
    return state;
}

u32 *func_002F3168(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = effClampSlotCount(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    ((EffAnimationState *)state)->record = func_002DBED8(3, count, mapping);
    return state;
}

void func_002F31D0(s32 work) {
    *(u32 *)(*(s32 *)(*(s32 *)(work + 0x30) + 4) + 8) = 0;
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
    void *allocation = func_003292A8(0xC);
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

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3258);

u32 *effPrepareQuantizedTexture(u8 *src) {
    u32 *buf = (u32 *)effAllocateQuantizedBuffer(src);
    buf[1] = func_002F3ED8(buf[0], ((EffQuantizedConfig *)src)->samples.quantizedSamples);
    func_002F3258(buf, src);
    return buf;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)effAllocateQuantizedBuffer(dst);
    buf[1] = func_002F3F40(src[1]);
    func_002F3258(buf, dst);
    return buf;
}

void func_002F3750(s32 work) {
    s32 state;

    state = *(s32 *)(work + 0x30);
    func_002F3F08(*(u32 *)(state + 4));
    func_003297C8(*(u32 *)(state + 8));
}

void effOffsetNodeRowsVU(u8 *work) {
    s32 *list = *(s32 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    s32 rows = ((EffQuantizedConfig *)config)->samples.signedRows + 1;
    s32 count = list[0];
    u8 *node = *(u8 **)&list[1];
    u8 *entry = *(u8 **)(node + 0x24);
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
void func_002F37F8(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x70);
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
    ((EffMeshOutput *)out)->mode = *(u8 *)(config + 0x94);
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
    u32 size = D_003E9E78[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    ((EffClassWork *)effect)->payload = effect + headerSize;
    ((EffClassWork *)effect)->color = 0x80808080;
    ((EffClassWork *)effect)->scale = 1.0f;
    ((EffClassWork *)effect)->kind = kind;
    ((EffClassWork *)effect)->frame = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(((EffClassWork *)effect)->payload, source, size);
    return effect;
}

u8 *effCreateResourceInstanceC(u16 kind, void *source) {
    u8 *work = effAllocateBlockWithModel(kind, source);
    ((EffClassWork *)work)->resource = (u32)D_003E9E60[kind].init(source);
    D_003E9E60[kind].fn(work);
    return work;
}

void effResourceInstanceCreateFromFile(s32 work) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effCreateResourceInstanceC(*(u16 *)(work + 0xc), source);
}

void effDispatchCleanupOp(u8 *work) {
    D_003E9E68[((EffClassWork *)work)->kind].run();
    func_00328E48(work);
}

u8 *effRecreateActiveByClass(u8 *obj) {
    u8 *work = effAllocateBlockWithModel(*(u16 *)(obj + 0x2C), ((EffClassWork *)obj)->payload);
    *(void **)(work + 0x30) = D_003E9E60[((EffClassWork *)obj)->kind].duplicate(obj);
    D_003E9E60[((EffClassWork *)obj)->kind].fn(work);
    return work;
}

void func_002F3BD8(u8 *work) {
    D_003E9E60[((EffClassWork *)work)->kind].fn();
    ((EffClassWork *)work)->frame = 0;
}

void func_002F3C20(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9E70[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002F3C80(s32 work) {
    D_003E9E74[((EffClassWork *)work)->kind].fn((void *)work);
}

void func_002F3CB8(u32 work) {
    func_002F3C20();
    func_002F3C80(work);
}

void func_002F3CE0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F3CF0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F3D08(s32 work, u32 color) {
    ((EffClassWork *)work)->color = color;
}

void func_002F3D10(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

u32 func_002F3D18(void) {
    if (D_00437E84 == 0) {
        D_00437E88 = func_002DDCA0(D_00437E80, 0x200);
    }
    D_00437E84 = D_00437E84 + 1;
    return D_00437E88;
}

void func_002F3D58(u32 unused) {
    D_00437E84 = D_00437E84 - 1;
    if (D_00437E84 == 0) {
        effReleaseSharedReference(D_00437E88);
        D_00437E88 = 0;
    }
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

u8 *func_002F3D88(count, repeat)
u32 count;
u32 repeat;
{
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x24 + 4) * count;
    u32 cells = rowStride * count;
    u8 *allocation = func_003292A8(size + 0x34);
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
    work->handle = func_003335E0();
    func_003332D0(work->handle, 1.0f);
    memset(&D_00458400, 0, sizeof(EffMotionSetup));
    D_00458400.flags = 0x4000;
    return (u8 *)work;
}

u64 func_002F3ED8(void) {
    u64 texture;

    texture = func_002F3D88();
    func_002F3D18();
    return texture;
}

void func_002F3F08(u8 *work) {
    func_002F3D58(D_00437E88);
    sdfQueueAssetRelease(*(u32 *)(work + 0x2C));
    func_003297C8(*(u32 *)(work + 0x30));
}

void func_002F3F40(u8 *work) {
    func_002F3D88(*(u32 *)work, *(u32 *)(work + 0x10));
    D_00437E84++;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F70);

void effLoadScalyTexture(void) {
    D_00437E7C = func_00343ED0("/effect/scaly00.tmx", &D_00437E80, 0);
}

u32 func_002F45A0(void) {
    return D_00437E80;
}

typedef struct EffSpanEntry {
    f32 first;
    f32 second;
    u32 pad_08;
} EffSpanEntry;

typedef struct EffSpanRecord {
    EffSpanEntry *entries;
    u8 pad_04[8];
    u32 flags;
} EffSpanRecord;

typedef struct EffSpanTable {
    EffSpanRecord *records;
    u32 count;
    u16 total;
} EffSpanTable;

typedef struct EffSpanConfig {
    u8 pad_00[0xA0];
    f32 firstRand;
    f32 secondBase;
    f32 rangeRand;
    u8 pad_AC[4];
    u32 perSpan;
} EffSpanConfig;

void func_002F45A8(u8 *work) {
    u32 index = 0;
    EffSpanTable *table = *(EffSpanTable **)(work + 0x38);
    EffSpanConfig *config = *(EffSpanConfig **)(work + 0x3C);
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
            record->flags = 0;
            entry = record->entries;
            span = 0;
            if (spans != 0) {
                do {
                    span++;
                    entry->first = func_00341240(D_0037F550) * config->firstRand + (1.0f - config->firstRand);
                    entry->second = config->secondBase * (func_00341240(D_0037F550) * config->rangeRand + (1.0f - config->rangeRand));
                    entry->pad_08 = 0;
                    entry++;
                } while (span < spans);
            }
            index++;
            record++;
        } while (index < table->count);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F46D8);

extern void func_002F5820(s32);

void effReleaseParticleList(u32 *list) {
    u32 *entry = (u32 *)list[0];
    u32 i;

    for (i = 0; i < list[1]; i++) {
        func_002F5820(entry[1]);
        if (entry[2] != 0) {
            effReleaseResourceRefs(entry[2]);
        }
        entry += 4;
    }
    func_003297C8(list[3]);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F4960);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5168);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5358);

u32 func_002F5488(u8 *work) {
    void *first = fileResolvePrimaryBuffer();
    void *second = fileResolveSecondaryBuffer(work);
    return func_002F5358(*(u16 *)(work + 0xC), first, second, *(u32 *)(work + 0x24));
}

void effDestroyModelResource(EffModelResource *effect) {
    D_003E9F10[effect->kind].run(effect->childResource);
    effDestroyModelContext((s32)effect->model);
    func_00328E48(effect);
}

EffModelResource *effCreateModelResource(EffModelCreateRequest *work) {
    EffModelResource *effect = (EffModelResource *)func_002F5358(work->kind, work->source, 0, 0);
    u32 x = func_00232EE8(work->assetId);
    u32 y = func_00232EF8(work->assetId);
    void *model = func_00232198(x, y);

    effect->model = model;
    effInitModelVUState(model);
    effect->attributes = work->attributes;
    effect->childResource = D_003E9F08[effect->kind].createResource(effect->source, effect->model);
    D_003E9F08[effect->kind].fn(effect);
    return effect;
}

void effResetModelResourceUpdateCount(u8 *work) {
    D_003E9F08[((EffModelResource *)work)->kind].fn();
    ((EffModelResource *)work)->updateCount = 0;
}

void func_002F5638(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9F14[((EffModelResource *)work)->kind].fn();
        ((EffModelResource *)work)->updateCount++;
    }
}

void func_002F5698(s32 work) {
    D_003E9F18[((EffModelResource *)work)->kind].fn((void *)work);
}

void effStepModelResourceCallbacks(u32 work) {
    func_002F5638();
    func_002F5698(work);
}

void func_002F56F8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F5708(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F5720(s32 work, u32 value) {
    *(u32 *)(work + 0x24) = value;
}

void func_002F5728(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

EffPointSet *effCreatePointSet3(s32 count) {
    s32 rows = count * 3 + 3;
    s32 size = rows * 20;
    u8 *base;
    u8 *data;
    EffPointSet *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = func_003292A8(size + 0x20);
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
    set->handle = func_003335E0();
    func_003332D0(set->handle, 1.0f);
    memset(D_00458430, 0, 0x2C);
    D_00458430[2] = 0x4000;
    return set;
}

void func_002F5820(s32 work) {
    sdfQueueAssetRelease(((EffAssetOwner *)work)->asset);
    func_003297C8(((EffAssetOwner *)work)->allocation);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5850);

void effGetWorldVector(u32 which) {
    EffectVectorRequest request;
    u128 result;
    u32 handle = func_00169438();

    request.kind = 0xB;
    request.count = 1;
    request.size = 8;
    request.unk04 = 0;
    switch (which) {
    case 0:
        break;
    case 1:
        handle = func_00169438();
        request.kind = 0;
        break;
    case 2:
        handle = func_00169440();
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
        handle = func_00169448();
        request.kind = 6;
        break;
    case 7:
        handle = func_00169450();
        request.kind = 7;
        break;
    }
    if (request.kind != 0xB) {
        u128 *vec = &result;

        effBattleMiscQueryPosition(handle, &request, vec);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(vec) : "memory");
    } else {
        __asm__ volatile (".set noreorder\nvmove.xyzw vf10, vf0\n.set reorder");
    }
}

s32 func_002F5BC8(EffectVectorRequest *source, s16 index) {
    EffectVectorRequest request;
    u128 result;
    u32 handle = 0;

    if (index < 0) {
        switch (source->kind) {
        case 0:
        case 1:
            handle = func_00169438();
            request.kind = 0;
            break;
        case 2:
            handle = func_00169440();
            request.kind = 0;
            break;
        case 3:
            handle = func_00169438();
            request.kind = 1;
            break;
        case 4:
            handle = func_00169440();
            request.kind = 2;
            break;
        case 5:
            return 0;
        case 6:
            handle = func_00169448();
            request.kind = 6;
            break;
        case 7:
            handle = func_00169450();
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
            handle = func_00169438();
            break;
        case 2:
            handle = func_00169440();
            break;
        case 3:
        case 4:
        case 5:
            return 0;
        case 6:
            handle = func_00169448();
            break;
        case 7:
            handle = func_00169450();
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
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(vec) : "memory");
        return 1;
    }
}

typedef struct EffActor {
    u8 pad000[0x54];
    u32 effectValue; /* 0x54: forwarded to the linked effect */
    u8 pad058[0xB8];
    u32 flags;
    u8 unk114[0x21C];
    u16 animFlags;
    u8 unk332[0xE];
    struct EffModelRef *model; /* 0x340 */
    u8 pad344[0x20];
    struct EffActor *next; /* 0x364 */
} EffActor;

s32 func_002F5D70(struct EffActor **out, u32 kind) {
    s32 count = 0;
    u32 mask = 0;
    u8 *state = (u8 *)func_001AA6F8();
    u8 *actor = (u8 *)func_00169438();
    u8 *other = (u8 *)func_00169440();

    switch (kind) {
    case 1:
        if ((((EffActor *)actor)->flags & 2) && ((EffActor *)actor)->model != 0) {
            out[0] = (struct EffActor *)actor;
            count = 1;
        }
        break;
    case 4:
        mask = ((EffActor *)other)->flags & 0x600;
        break;
    case 3:
        mask = ((EffActor *)actor)->flags & 0x600;
        break;
    case 5:
        mask = 0x600;
        break;
    case 6:
        other = (u8 *)func_00169448();
        if ((((EffActor *)other)->flags & 2) && ((EffActor *)other)->model != 0) {
            out[0] = (struct EffActor *)other;
            count = 1;
        }
        break;
    case 7:
        other = (u8 *)func_00169450();
        if ((((EffActor *)other)->flags & 2) && ((EffActor *)other)->model != 0) {
            out[0] = (struct EffActor *)other;
            count = 1;
        }
        break;
    case 0:
    case 2:
        if ((((EffActor *)other)->flags & 2) && ((EffActor *)other)->model != 0) {
            out[0] = (struct EffActor *)other;
            count = 1;
        }
        break;
    }
    if (mask != 0) {
        u8 *link;

        for (link = *(u8 **)(state + 0x24C); link != NULL; link = (u8 *)((EffActor *)link)->next) {
            u32 flags = ((EffActor *)link)->flags;

            if (flags & 1) {
                if (flags & 2) {
                    if (((EffActor *)link)->model != 0) {
                        if (flags & mask) {
                            out[count++] = (struct EffActor *)link;
                        }
                    }
                }
            }
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5EF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6000);

extern void func_0023C978(s32, s32, s32);

void func_002F6450(void) {
    s32 owner = func_001AA6F8();
    s32 *entry;

    if ((*(u32 *)(owner + 0x218) & 0x6000000) != 0x6000000) {
        return;
    }
    entry = *(s32 **)(owner + 0x24C);
    while (entry != NULL) {
        if (entry[0x110 / 4] & 2) {
            s32 child = entry[0x340 / 4];
            if (child != 0) {
                *(s32 *)(child + 0x60) = entry[0x54 / 4];
                func_0023C978(child, 0, entry[0x54 / 4]);
            }
        }
        entry = (s32 *)entry[0x364 / 4];
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F64D8);

s64 effComputeLightDirectionVU(void *model, void *target) {
    if (func_001AA308() == 0) {
        return 0;
    }
    if (D_00437E94 == 0) {
        return 0;
    }
    mdlLoadPrimaryVectorVU(model);
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_004584B0));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vsub.xyzw $vf10, $vf10, $vf11\n\t"
        "vmulx.w $vf10, $vf10, $vf0x\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00458470) : "memory");
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
    PCP_COPY_VECTOR(D_004584A0, D_0037F780);
    D_00437E94 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F67D8);

void effResetObjectSlots(u8 *work) {
    u32 *objects = (u32 *)(*(u32 *)(work + 0x30) + 0x24);
    u32 i;
    for (i = 0; i < 5; i++) {
        u32 object = objects[i];
        if (object != 0) {
            *(u32 *)object = 0;
            *(u32 *)(object + 0x0C) = 0;
        }
    }
}

s32 *func_002F69F0(u32 owner, u32 source, s32 size) {
    u32 headerSize = 0x40;
    u8 *base = func_003292A8(size + headerSize);
    u8 *body = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *node = body;

    body += headerSize;
    if (size <= 0) {
        body = 0;
    }
    *(u8 **)(node + 0x38) = base;
    *(s32 *)(node + 4) = size;
    *(u8 **)node = body;
    *(u32 *)(node + 8) = 0;
    memcpy(body, (void *)source, size);
    return (s32 *)node;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6A80);

extern void fldRelocatePackedTransferChunk(s32, s32);

extern void func_002F6A80(s32 *);

s32 *func_002F6BE8(u32 owner, u32 unused, u32 source, u32 kind) {
    s32 *work = func_002F69F0(owner, source, kind);
    s32 object = *work;
    fldRelocatePackedTransferChunk(object, object + 8);
    func_002F6A80(work);
    return work;
}

s32 *func_002F6C30(s32 owner) {
    s32 *work;

    work = func_002F69F0(*(u32 *)(owner + 0x38), **(u32 **)(owner + 0x30),
                                                (*(u32 **)(owner + 0x30))[1]);
    func_002F6A80(work);
    return work;
}

extern void dds3FreePathObject();
extern void func_00110B50();

void effReleaseTargetSlots(u32 *obj) {
    u32 *tails = obj + 0x10 / 4;
    u32 *heads = obj + 0x24 / 4;
    u32 i;
    for (i = 0; i < 5; i++) {
        if (*heads != 0) {
            dds3FreePathObject(*heads);
        }
        heads++;
        if (*tails != 0) {
            func_00110B50(*tails);
        }
        tails++;
    }
    func_003297C8(obj[0x38 / 4]);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6D00);


typedef struct EffModelRef {
    u8 unk0[0x8C];
    s32 nodes;
} EffModelRef;

typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 unk4;
    u8 unk5;
    u16 loop;
} EffAnimInfo;

typedef struct EffAnimOwner {
    u8 unk0[0x28];
    s32 hold;
    u8 unk2C[0xC];
    EffAnimInfo *info;
} EffAnimOwner;

extern s32 mdlGetNodeRefHalf();
extern void btlApplyScaledUnitEffectParameter(EffActor *, u16, s32, f32);
extern void func_00204D08();

void func_002F7128(EffAnimOwner *owner) {
    EffActor *actor[16];
    EffAnimInfo *info;
    u32 count;
    u32 i;
    if (owner->hold <= 0) {
        info = owner->info;
        count = func_002F5D70(actor, info->unk4);
        for (i = 0; i < count; i++) {
            if (actor[i]->flags & 2) {
                if (!(actor[i]->flags & 0x20) && !(actor[i]->animFlags & 0x10)) {
                    if (actor[i]->animFlags & 0x40) {
                        switch (info->id) {
                        case 0:
                        case 2:
                        case 0xa:
                            continue;
                        }
                    }
                    if (mdlGetNodeRefHalf(actor[i]->model->nodes, 0) > info->id) {
                        btlApplyScaledUnitEffectParameter(actor[i], info->id, info->flags | 0x100, 1.0f);
                        if (info->loop == 0) {
                            func_00204D08(actor[i], info->id);
                        }
                    }
                }
            }
        }
    }
}

void effReportResourceStatus(u8 *work) {
    u32 status;

    if (*(s32 *)(work + 0x28) > 0) {
        return;
    }
    status = **(u32 **)(work + 0x38);
    switch (status) {
    case 0:
        sndLoadAndPlayStationedSe(0x1000A);
        break;
    case 1:
        sndLoadAndPlayStationedSe(0x1000B);
        break;
    }
}

void func_002F72D8(void) {
    s32 actor;

    actor = func_001AA6F8();
    if ((*(u32 *)(actor + 0x218) & 0x6000000) == 0x6000000) {
        func_00200930(actor + 0x50, actor + 0x60, 0);
        return;
    }
}

void effStartTintTransitionFromColors(u8 *work) {
    f32 from[3];
    f32 to[3];
    u32 *colors;
    u32 c;

    func_001AA6F8();
    colors = *(u32 **)(work + 0x38);
    if (*(s32 *)(work + 0x28) == 0) {
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
    func_00104020();
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
                func_00103F58(i, (u8)(D_00437EA8[i] * ratio), 100);
                D_00437EA0[i] -= 1;
            } else if (D_00437EB0[i] == 1) {
                func_00103F58(i, D_00437EA8[i], 100);
            } else {
                func_00103F58(i, 0, 0);
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

void func_002F7798(void) {
    s32 actor;

    actor = func_001AA6F8();
    if ((*(u32 *)(actor + 0x218) & 0x6000000) == 0x6000000) {
        effResetSlots();
        return;
    }
}

void func_002F77D0(s32 work) {
    s32 elapsed;
    u32 duration;
    u32 *slotData;
    u8 *slotColor;
    u32 index;

    index = 0;
    func_001AA6F8();
    elapsed = *(s32 *)(work + 0x28);
    slotColor = (u8 *)(*(s32 *)(work + 0x38) + 0x18);
    slotData = (u32 *)(*(s32 *)(work + 0x38) + 0x10);
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

void effUpdateSlotTimerPair(u8 *work) {
    u32 *slot;
    u32 first;
    s32 phase;

    func_001AA6F8();
    slot = *(u32 **)(work + 0x38);
    first = slot[0];
    phase = *(s32 *)(work + 0x28);
    if (first < slot[3]) {
        return;
    }
    if (phase == 0) {
        btlInitTintTransitionResource(slot[1], *(u16 *)(slot + 2));
        first = slot[0];
    }
    if (first != 0 && phase == first - slot[3]) {
        btlInitTintTransitionDefault(*(u16 *)(slot + 3));
    }
}

extern f32 D_00437E90;
extern void func_001E95C8(s32, f32);

void effApplyKeyframeAngle(u8 *work) {
    s32 owner = func_001AA6F8();
    f32 *keys = *(f32 **)(work + 0x38);
    u32 total = *(u32 *)keys;
    s32 frame;
    f32 ratio;
    f32 value;

    if (total != 0) {
        frame = *(u32 *)(work + 0x28);
        if (total < frame) {
            return;
        }
        ratio = (f32)frame / (f32)total;
        value = ((keys[2] - keys[1]) * ratio + keys[1]) * 0.017453293f;
        func_001E95C8(owner + 0x70, value);
        D_00437E90 = value;
    }
}

u32 *func_002F79E0(u32 owner) {
    u32 *work = (u32 *)func_00328D68(4);
    *work = 0;
    return work;
}

u32 *func_002F7A00(u32 owner) {
    u32 *work = func_002F79E0(owner);
    *work = func_002EDCF0(4, owner);
    return work;
}

u32 *func_002F7A48(u8 *request) {
    u32 *source = *(u32 **)(request + 0x30);
    u32 *work = func_002F79E0(*(u32 *)(request + 0x38));
    *work = effPayloadPointerGet(*source);
    return work;
}

void func_002F7A90(u32 handle) {
    if (*(s32 *)handle != 0) {
        func_002EDE10(*(s32 *)handle);
    }
    func_00328E48(handle);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7AC8);

void func_002F7C40(s32 owner) {
    func_002EDF20(**(u32 **)(owner + 0x30));
}

u32 *func_002F7C60(u32 owner) {
    u32 *work = (u32 *)func_00328D68(4);
    *work = 0;
    return work;
}

u32 *func_002F7C80(s32 source, u16 kind, s32 *settings) {
    u32 *work = func_002F7C60(source);
    *work = func_002E7B90(7, source);
    func_002E7C00((s32 *)*work, kind, settings);
    return work;
}

extern u32 func_002E7EE0(u8 *);

u32 *func_002F7CF0(u8 *request) {
    s32 *source = *(s32 **)(request + 0x30);
    u32 *work = func_002F7C60(*(u32 *)(request + 0x38));
    *work = func_002E7EE0(*source);
    return work;
}

void func_002F7D38(u32 handle) {
    if (*(s32 *)handle != 0) {
        effDestroySurfaceNode(*(EffectSlotNode54 **)handle);
    }
    func_00328E48(handle);
}

void func_002F7D70(u8 *work) {
    u8 *object = *(u8 **)(work + 0x38);
    s32 *handle = *(s32 **)(work + 0x30);
    u8 *slot;
    f32 start[4];
    f32 end[4];
    f32 offset[4];

    if (func_002F5BC8((EffectVectorRequest *)(object + 0x140), *(s16 *)(object + 0x150)) == 0) {
        VU0_LOAD_VF(vf10, work);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, start);
    slot = object + 0x148;
    if (func_002F5BC8((EffectVectorRequest *)slot, *(s16 *)(object + 0x152)) == 0) {
        if (*slot == 5) {
            offset[0] = 0.0f;
            offset[2] = 0.0f;
            switch (*(u8 *)(object + 0x149)) {
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
    func_002DB288(*(s32 *)(*handle + 0x4C), start);
    func_002DB2C0(*(s32 *)(*handle + 0x4C), end);
    effAcquireSurfaceRecord(*handle);
}

void func_002F7E88(s32 owner) {
    func_002E8770(**(u32 **)(owner + 0x30));
}

u32 *func_002F7EA8(u32 owner) {
    u32 *work = (u32 *)func_00328D68(4);
    *work = 0;
    return work;
}

extern void mdlAddEntryPlain(s32, u32, u32);

extern void mdlAddEntryFlagged(s32, u32, u32);

u32 *func_002F7EC8(u32 *owner, u32 kind, u32 source, u32 settings) {
    u32 *work = func_002F7EA8((u32)owner);
    s32 object = func_002DC1D0(source, settings);
    s32 active = *(s32 *)(object + 0x1C);
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

u32 *func_002F7F58(u8 *request) {
    u32 *owner = *(u32 **)(request + 0x38);
    void **source = *(void ***)(request + 0x30);
    u32 *work = func_002F7EA8((u32)owner);
    s32 a = func_00232EE8(*source);
    s32 b = func_00232EF8(*source);
    void *object = func_00232198(a, b);
    *work = (u32)object;
    effInitModelVUState(object);
    if (*(s32 *)(*work + 0x1C) != 0) {
        if (*owner != 0) {
            mdlAddEntryPlain(*work, 0, 0);
        } else {
            mdlAddEntryFlagged(*work, 0, 0);
        }
    }
    return work;
}

void func_002F8008(u32 handle) {
    if (*(s32 *)handle != 0) {
        effDestroyModelContext(*(s32 *)handle);
    }
    func_00328E48(handle);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8040);

s32 *func_002F81A8(s32 *owner) {
    s32 *work = (s32 *)func_00328D68(8);
    work[0] = 0;
    work[1] = 0;
    return work;
}

s32 *func_002F81D0(s32 *owner, u32 kind, u32 source, u32 settings) {
    s32 *work = func_002F81A8(owner);
    s32 object;
    s32 active;

    work[0] = func_002EDCF0(4, (u32)owner);
    object = func_002DC1D0(source, settings);
    active = *(s32 *)(object + 0x1C);
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

s32 *func_002F8270(u8 *request) {
    s32 *owner = *(s32 **)(request + 0x38);
    u32 *source = *(u32 **)(request + 0x30);
    s32 *work = func_002F81A8(owner);
    s32 a;
    s32 b;
    void *object;
    s32 material;
    u32 modelSource;

    material = effPayloadPointerGet(source[0]);
    modelSource = source[1];
    work[0] = material;
    a = func_00232EE8((void *)modelSource);
    b = func_00232EF8((void *)source[1]);
    object = func_00232198(a, b);
    work[1] = (s32)object;
    effInitModelVUState(object);
    if (*(s32 *)(work[1] + 0x1C) != 0) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(work[1], 0, 0);
        } else {
            mdlAddEntryFlagged(work[1], 0, 0);
        }
    }
    return work;
}

extern void func_002EDE10(s32);

void func_002F8328(s32 *work) {
    if (work[1] != 0) {
        effDestroyModelContext(work[1]);
    }
    if (work[0] != 0) {
        func_002EDE10(work[0]);
    }
    func_00328E48(work);
}

/* vu0 routine: aim matrix from an object's target point toward its origin, plus the distance stored in the node state */
void func_002F8378(u8 *work) {
    u8 *object = *(u8 **)(work + 0x38);
    s32 *handle = *(s32 **)(work + 0x30);
    u128 mtx[4];
    f32 target[4];
    f32 origin[4];
    f32 look[4];
    f32 length;
    u8 *state;

    if (func_002F5BC8((EffectVectorRequest *)(object + 0x88), *(s16 *)(object + 0x90)) == 0) {
        VU0_LOAD_VF(vf10, work);
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, target);
    func_00332D08(*(s32 *)(handle[1] + 0x18), 0);
    VU0_STORE_VF_UNCLOBBERED(vf31, origin);
    func_002EDF80((s128 *)handle[0], (s128 *)target);
    state = *(u8 **)(handle[0] + 0x34);
    VU0_LOAD_VF(vf10, target);
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    *(f32 *)(state + 0x84) = length;
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
    func_002D31C0(mtx);
    VU0_STORE_VF_UNCLOBBERED(vf10, look);
    func_002EDF90((s128 *)handle[0], (s128 *)look);
    func_002EDEC0(handle[0]);
}

extern void mdlStorePrimaryVectorVU(void *);
extern void func_00232AD0(void *);

/* The model helpers read vf10, following the SDK's VU0 macro-mode convention. */
void effApplyModelTransform(u8 *work) {
    u8 *modelContext = *(u8 **)(work + 0x30);
    u8 *animation = *(u8 **)(work + 0x38);
    u32 bits;
    float scale;
    VU0_LOAD_VF_MEMORY(vf10, work);
    mdlStorePrimaryVectorVU(*(void **)(modelContext + 4));
    VU0_LOAD_VF_MEMORY(vf10, work + 0x10);
    func_00232AD0(*(void **)(modelContext + 4));
    VU0_SET_ONES_XYZ(vf10);
    scale = *(float *)(work + 0x20);
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 %0, %1\n\t"
        "qmtc2.ni %0, $vf2\n\t"
        "vmulx.xyzw vf10, vf10, $vf2x\n\t"
        ".set reorder"
        : "=&r"(bits) : "f"(scale) : "memory");
    mdlStoreTertiaryVectorVU(*(void **)(modelContext + 4));
    *(float *)(*(u8 **)(*(u8 **)(modelContext + 4) + 0x1C) + 0x20) =
        *(float *)(animation + 0x94);
    func_00232390(*(void **)(modelContext + 4), D_00380828);
    func_002EDF20(*(s32 *)modelContext);
}

extern s32 *func_003335E0(void);

s32 *func_002F8590() {
    s32 *work = func_00328E18(0xC);
    s32 *position;
    work[2] = 0;
    position = func_003335E0();
    work[1] = (s32)position;
    *(f32 *)((u8 *)position + 0x1C) = 1.0f;
    return work;
}

void func_002F85D8(void) {
    func_002F8590();
}

void func_002F85F0(s32 owner) {
    func_002F8590(*(u32 *)(owner + 0x38));
}

void func_002F8608(u32 work) {
    s32 resource;

    resource = *(s32 *)((s32)work + 4);
    if (resource != 0) {
        sdfQueueAssetRelease(resource);
    }
    func_00328E48(work);
}

void func_002F8640(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8648);

typedef struct EffFadeTarget {
    u8 pad_00[0x60];
    u32 color;              // 0x60
} EffFadeTarget;

typedef struct EffFadeNode {
    u8 pad_00[0x54];
    u32 color;              // 0x54
    u8 pad_58[0x2C];
    u32 packedColor;        // 0x84
    u8 pad_88[0x88];
    u32 flags;              // 0x110
    u8 pad_114[0x22C];
    EffFadeTarget *target;  // 0x340
    u8 pad_344[0x20];
    struct EffFadeNode *next; // 0x364
} EffFadeNode;

extern void func_0023CA60(void *, u32, u32);

void effSyncFadeColorToTargets(void) {
    s32 owner = func_001AA6F8();

    if ((*(u32 *)(owner + 0x218) & 0x6000000) == 0x6000000) {
        EffFadeNode *node = *(EffFadeNode **)(owner + 0x24C);

        while (node != 0) {
            if (node->flags & 2) {
                EffFadeTarget *target = node->target;

                if (target != 0) {
                    node->packedColor = (node->packedColor & 0xFFFFFF) | (node->color & 0xFF000000);
                    target->color = node->color;
                    func_0023CA60(target, 0, node->color);
                }
            }
            node = node->next;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8CF0);

extern u8 D_003E9FF0[];
extern void func_003332D0(void *, f32);

s32 *effCreateMotionResource(s32 *context) {
    s32 *work = (s32 *)func_00328D68(8);

    work[0] = 0;
    work[1] = (s32)func_003335E0();
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
    func_00159FA0(resource[0]);
    billSetBillboardMode(resource[0], *(s16 *)((u8 *)context + 0x4C));
    return resource;
}

s32 *effBillboardMotionResourceInitialize(s32 *request) {
    s32 *source = (s32 *)request[0x30 / 4];
    s32 *context = (s32 *)request[0x38 / 4];
    s32 *resource = effCreateMotionResource(context);
    resource[0] = func_00159A50(*source);
    func_00159FA0(resource[0]);
    billSetBillboardMode(resource[0], *(s16 *)((u8 *)context + 0x4C));
    return resource;
}

void func_002F9180(s32 *work) {
    if (work[0] != 0) {
        billDispatchByKind(work[0]);
    }
    if (work[1] != 0) {
        sdfQueueAssetRelease(work[1]);
    }
    func_00328E48(work);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F91D0);

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

u8 *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003EA030[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    ((EffActiveResource *)effect)->payload = effect + headerSize;
    ((EffActiveResource *)effect)->color = 0x80808080;
    ((EffActiveResource *)effect)->scale = 1.0f;
    ((EffActiveResource *)effect)->kind.index = kind;
    ((EffActiveResource *)effect)->frame = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(((EffActiveResource *)effect)->payload, source, size);
    return effect;
}

u8 *func_002F9608(u16 kind, void *source, u16 secondaryKind, s32 secondary, u32 param) {
    u8 *effect = effAllocateResourcePayload(kind, source);

    if (func_001AA308() != 0) {
        if (D_003EA018[kind].init != NULL) {
            ((EffActiveResource *)effect)->resource = (u32)D_003EA018[kind].init(source, secondaryKind, secondary, param);
        }
        if (D_003EA018[kind].fn != NULL) {
            D_003EA018[kind].fn(effect);
        }
    }
    return effect;
}

void func_002F96D0(s32 *source) {
    void *primary = fileResolvePrimaryBuffer();
    s32 secondary = fileResolveSecondaryBuffer(source);
    func_002F9608(*(u16 *)((u8 *)source + 0xC), primary,
                  *(u16 *)((u8 *)source + 0x1C), secondary, source[0x24 / 4]);
}

void effDestroyResourceInstance(u32 *obj) {
    if (func_001AA308()) {
        if (D_003EA020[obj[0x2C / 4]].fn != NULL) {
            D_003EA020[obj[0x2C / 4]].fn(obj[0x30 / 4]);
        }
    }
    func_00328E48(obj);
}

u8 *func_002F9780(u8 *source) {
    u8 *effect;
    u32 kind = ((EffActiveResource *)source)->kind.index;

    if (D_003EA018[kind].duplicate == NULL) {
        effect = func_002F9608(((EffActiveResource *)source)->kind.shortIndex, ((EffActiveResource *)source)->payload, 0, 0, 0);
    } else {
        u32 resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(((EffActiveResource *)source)->kind.shortIndex, ((EffActiveResource *)source)->payload);
        resource = (u32)D_003EA018[((EffActiveResource *)source)->kind.signedIndex].duplicate(source);
        activeKind = ((EffActiveResource *)source)->kind.index;
        ((EffActiveResource *)effect)->resource = resource;
        if (D_003EA018[activeKind].fn != NULL) {
            D_003EA018[activeKind].fn(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(u32 *obj) {
    if (func_001AA308()) {
        if (D_003EA018[obj[0x2C / 4]].fn != NULL) {
            D_003EA018[obj[0x2C / 4]].fn(obj);
        }
        obj[0x28 / 4] = 0;
    }
}

void effAdvanceCallbackFrame(work)
u8 *work;
{
    if (func_001AA308() != 0 && (D_00437E08 & 2) == 0) {
        s32 kind = ((EffActiveResource *)work)->kind.signedIndex;
        FnTbl28 *entry = &D_003EA028[kind];
        void (*callback)(void *) = entry->fn;
        if (callback != NULL) {
            callback(work);
        }
        ((EffActiveResource *)work)->frame++;
    }
}

void effDispatchEnabledCallback(u8 *work) {
    if (func_001AA308() != 0) {
        void (*callback)(void *) = D_003EA02C[((EffActiveResource *)work)->kind.signedIndex].fn;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void func_002F9988(u32 work) {
    effAdvanceCallbackFrame();
    effDispatchEnabledCallback(work);
}

void func_002F99B0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F99C0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F99D8(s32 work, u32 color) {
    ((EffActiveResource *)work)->color = color;
}

void func_002F99E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99E8);
/* Alternate particle handles share a header but occupy distinct slots. */
typedef struct EffParticleShared {
    u8 pad_00[0xA4];
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
    func_00328E48(p);
}

u8 *func_002F9C38(u8 *source) {
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
        handle = func_00159A50(((EffParticleShared *)src)->billHandle);
        ((EffParticleShared *)dst)->billHandle = handle;
        func_00159FA0(handle);
    } else {
        if (((EffParticleShared *)dst)->reference != 0) {
            effReleaseReferenceHolder(((EffParticleShared *)dst)->reference);
        }
        ((EffParticleShared *)dst)->reference = effReferenceObjectRetain(((EffParticleShared *)src)->reference);
    }
}

void func_002F9DE8(s32 work) {
    *(u32 *)(work + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9DF0);

void func_002FA388(u32 *target, u32 value) {
    *target = value;
}

extern f32 func_00341240(void *);

/* Normalize the seed vector with the original VU0 macro-mode operations. */
void effInitializeParticleDirection(u8 *work, float *entry) {
    float vector[4];
    if (work[0x10] == 0) {
        vector[0] = *(float *)(work + 0x14);
        vector[1] = *(float *)(work + 0x18);
        vector[2] = *(float *)(work + 0x1C);
    } else {
        vector[0] = (func_00341240(D_0037F550) - 0.5f) * 2.0f;
        vector[1] = (func_00341240(D_0037F550) - 0.5f) * 2.0f;
        vector[2] = (func_00341240(D_0037F550) - 0.5f) * 2.0f;
    }
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 $vf10, %0\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        "sqc2 $vf10, %0\n\t"
        ".set reorder"
        : "+m"(vector) :: "memory");
    entry[0] = vector[0];
    entry[2] = vector[2];
    entry[1] = vector[1];
    entry[5] =
        *(float *)(work + 0x24) *
        (func_00341240(D_0037F550) * *(float *)(work + 0x28) +
         (1.0f - *(float *)(work + 0x28)));
    entry[4] = func_00341240(D_0037F550) * 6.2831853f;
}

extern void func_00336768(f32 *, f32);

void func_002FA4D0(u8 *work, f32 *params, s32 frame) {
    f32 axis[4];
    f32 time = (f32)frame;
    f32 angle = params[5] * time;

    angle += *(f32 *)(work + 0x2C) * time * time * 0.5f;
    if (angle < 0.0f) {
        angle = 0.0f;
    }
    axis[0] = params[0];
    axis[1] = params[1];
    axis[2] = params[2];
    axis[3] = 0.0f;
    func_00336768(axis, angle + params[4]);
}

typedef struct EffectSlotNode80 {
    u32 count;
    u32 color;
    f32 opacity;
    s32 resourceKind;       // 0x0C
    u8 pad_10[0x50];
    u32 model;              // 0x60
    void *deviceSlot;       // 0x64
    u32 resourceEntries;    // 0x68
    u32 entryAllocation;    // 0x6C
    u32 record;             // 0x70
    u8 *positions;          // 0x74
    u32 positionAllocation; // 0x78
    u16 active;
} EffectSlotNode80;

EffectSlotNode80 *func_002FA550(u32 count) {
    EffectSlotNode80 *node = func_00328E18(sizeof(EffectSlotNode80));
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
    return func_002FA550(effSlotCount((u8 *)source, 300));
}

extern void effRebuildResourceEntries(u8 *, u32, s32 *);
extern void func_002FABA8(s32, u32, u32);
extern void func_002FAC38(s32 *, s32);

s32 *func_002FA5F0(u8 *request) {
    u8 *buffer = fileResolvePrimaryBuffer(request);
    s32 *size = (s32 *)(buffer + 0x50);
    s32 *work = func_002FA5B8(size);
    s32 secondary;
    u32 kind;

    memcpy((u8 *)work + 0x10, buffer, 0x50);
    effRebuildResourceEntries((u8 *)work, *(u16 *)(request + 0xC), size);
    secondary = fileResolveSecondaryBuffer(request);
    if (secondary != 0) {
        kind = *(u16 *)(request + 0x1C);
        switch (kind) {
        case 3:
            func_002FABA8((s32)work, secondary, *(u32 *)(request + 0x24));
            kind = *(u16 *)(request + 0x1C);
            break;
        case 6:
            func_002FAC38(work, secondary);
            kind = *(u16 *)(request + 0x1C);
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
        func_003297C8(((EffectSlotNode80 *)work)->entryAllocation);
    }
    if (((EffectSlotNode80 *)work)->record != 0) {
        func_002DC028(((EffectSlotNode80 *)work)->record);
    }
    if (((EffectSlotNode80 *)work)->positionAllocation != 0) {
        func_003297C8(((EffectSlotNode80 *)work)->positionAllocation);
    }
    func_00328E48(work);
}

extern s32 *func_002FA5B8(s32 *);

s32 *effCloneOwnedState(u8 *owner) {
    s32 *source = *(s32 **)(*(u8 **)(owner + 0x70) + 0x24);
    s32 *work = func_002FA5B8(source);
    memcpy((u8 *)work + 0x10, owner + 0x10, 0x50);
    effRebuildResourceEntries((u8 *)work, *(u16 *)(*(u8 **)(owner + 0x70)), source);
    func_002FA978(work, owner);
    return work;
}

void func_002FA978(s32 *work, u8 *owner) {
    EffectSlotNode80 *dst = (EffectSlotNode80 *)work;
    EffectSlotNode80 *src = (EffectSlotNode80 *)owner;
    s32 kind = src->resourceKind;
    s32 model;
    s32 modelData;
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
        modelData = *(s32 *)(model + 0xC);
        dst->model = model;
        dst->deviceSlot = sdfModelCreateWithAlternateItems(*(u32 *)(modelData + 0x14), *(u32 *)(modelData + 0x18));
        kind = src->resourceKind;
        break;
    case 6:
        count = *(u32 *)(src->record + 8);
        if (count == 0) {
            return;
        }
        if (dst->entryAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileQueueDestroy(((s32 *)dst->resourceEntries)[i]);
            }
            func_003297C8(dst->entryAllocation);
            dst->resourceEntries = 0;
            dst->entryAllocation = 0;
        }
        if (count * 4 == 0) {
            return;
        }
        dst->entryAllocation = (u32)func_003292A8(count * 4);
        dst->resourceEntries = sdfResourceRetainAddress(dst->entryAllocation);
        for (i = 0; i < count; i++) {
            ((void **)dst->resourceEntries)[i] = fileQueueClone(((void **)src->resourceEntries)[0]);
        }
        kind = src->resourceKind;
        break;
    }
    dst->resourceKind = kind;
}

extern s32 func_002DBED8(u16, s32, s32);

void effRebuildResourceEntries(u8 *work, u32 kind, s32 *config) {
    s32 previous = ((EffectSlotNode80 *)work)->record;
    s32 allocation;
    u32 count;
    u32 i;
    u8 *entries;
    if (previous != 0) {
        func_002DC028(previous);
    }
    ((EffectSlotNode80 *)work)->record = func_002DBED8(kind, *(s32 *)work, (s32)config);
    allocation = ((EffectSlotNode80 *)work)->positionAllocation;
    if (allocation != 0) {
        func_003297C8(allocation);
    }
    count = *(u32 *)(((EffectSlotNode80 *)work)->record + 8);
    ((EffectSlotNode80 *)work)->positionAllocation = func_003292A8(count * 0x18);
    ((EffectSlotNode80 *)work)->positions = (u8 *)sdfResourceRetainAddress(((EffectSlotNode80 *)work)->positionAllocation);
    entries = ((EffectSlotNode80 *)work)->positions;
    for (i = 0; i < count; i++, entries += 0x18) {
        effInitializeParticleDirection(work, (float *)entries);
    }
}

void func_002FABA8(s32 work, u32 kind, u32 config) {
    s32 modelData;
    s32 model;
    void *deviceSlot;

    if (((EffectSlotNode80 *)work)->model != 0) {
        effDestroyModelContext(((EffectSlotNode80 *)work)->model);
        ((EffectSlotNode80 *)work)->model = 0;
    }
    if (((EffectSlotNode80 *)work)->deviceSlot != 0) {
        sdfReleaseDevSlot(((EffectSlotNode80 *)work)->deviceSlot, 1, 1);
        ((EffectSlotNode80 *)work)->deviceSlot = 0;
    }
    model = func_002DC1D0(kind, config);
    modelData = *(s32 *)(model + 0xc);
    ((EffectSlotNode80 *)work)->model = model;
    deviceSlot = sdfModelCreateWithAlternateItems(*(u32 *)(modelData + 0x14), *(u32 *)(modelData + 0x18));
    ((EffectSlotNode80 *)work)->deviceSlot = deviceSlot;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAC38);

void effClearSurfaceRecordReferences(s32 node) {
    if (((EffectSlotNode80 *)node)->record != 0) {
        fileClearRecordReferences(((EffectSlotNode80 *)node)->record);
        return;
    }
}

void func_002FAD60(s32 node) {
    if (D_00437E08 & 2) {
        return;
    }
    if (((EffectSlotNode80 *)node)->record != 0) {
        fileAcquireRecord(((EffectSlotNode80 *)node)->record);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD98);

void func_002FB400(u32 node) {
    func_002FAD60(node);
    func_002FAD98(node);
}

void func_002FB428(s32 node) {
    mnuRecordSetVector(((EffectSlotNode80 *)node)->record);
}

void func_002FB440(s32 node) {
    func_002DC0F0(((EffectSlotNode80 *)node)->record);
}

void func_002FB458(s32 node, u32 value) {
    *(u32 *)(node + 4) = value;
}

void func_002FB460(s32 *work, f32 value) {
    *(f32 *)((u8 *)work + 8) = value;
    dds3DispatchIndexedCallback(work[0x70 / 4], value);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB480);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB5C0);

void effReleaseSharedResourceReference(s32 *work) {
    s32 *resource = (s32 *)work[0x478 / 4];
    u32 references = *(u16 *)((u8 *)resource + 0x10) + 0xFFFF;
    *(u16 *)((u8 *)resource + 0x10) = references;
    if ((u16)references == 0) {
        s32 data = resource[2];
        if (data != 0) {
            func_00328E48(data);
        }
        resource = (s32 *)work[0x478 / 4];
        if (resource[0] != 0) {
            effDestroyModelContext(resource[0]);
        }
        resource = (s32 *)work[0x478 / 4];
        if (resource[1] != 0) {
            sdfReleaseDevSlot(resource[1], 1, 1);
        }
        resource = (s32 *)work[0x478 / 4];
        if (resource[0x1C / 4] != 0) {
            func_003297C8(resource[0x1C / 4]);
        }
        func_00328E48(work[0x478 / 4]);
    }
    func_003297C8(work[0x47C / 4]);
}

typedef struct { u32 word[0x113]; } EffectBlob;
extern u8 *func_002FB5C0(s32);

s32 effCloneEffectRequest(u8 *src) {
    u8 *dst = func_002FB5C0(0);

    *(EffectBlob *)(dst + 0x2C) = *(EffectBlob *)(src + 0x2C);
    func_002FB948((s32)dst, (s32)src);
    return (s32)dst;
}

void func_002FB948(s32 target, s32 source) {
    *(u32 *)(target + 0x478) = *(u32 *)(source + 0x478);
    *(s16 *)(*(s32 *)(source + 0x478) + 0x10) = *(s16 *)(*(s32 *)(source + 0x478) + 0x10) + 1;
}

void func_002FB968(s32 *work) {
    s32 *context = *(s32 **)work[0x478 / 4];
    if (context != NULL) {
        func_003343E8(context[0x1C / 4], 0.0f);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB998);

void func_002FC0A8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002FC0B8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002FC0D0(s32 work, u32 value) {
    *(u32 *)(work + 0x20) = value;
}

void func_002FC0D8(s32 *work, f32 value) {
    *(f32 *)((u8 *)work + 0x24) = value;
}

void btlInitializeEffectWork(void) {
    D_004386CC = D_003FFA40;
    D_0045C1A0[0] = 0;
    D_004386B0 = 0;
    D_00439075 = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_0045C1E0) : "memory");
}

s32 btlUpdateEffectWork(void) {
    if (D_004386CC != NULL) {
        if ((func_002FCA58(0) & 1) == 0) {
            func_002FC160();
            return 0;
        }
    }
    return 1;
}

void func_002FC158(void) {
}

void func_002FC160(void) {
    effResetFileResources();
    func_00303C50();
}

void effSetBattleOffset(void *src) {
    PCP_COPY_VECTOR(D_0045C1E0, src);
}

void effComputeBattleCameraPositionVU(u8 *effect) {
    u128 direction;
    u32 flags = *(u32 *)(effect + 0x68);

    if ((flags & 0x18) != 0) {
        camFollowOffsetVec(effect, &direction);
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&direction));
        flags = *(u32 *)(effect + 0x68);
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(effect + 0x40));
    }
    if ((flags & 0x80) != 0) {
        __asm__ volatile(
            ".set noreorder\n\t"
            "mfc1 $2, %0\n\t"
            "qmtc2.ni $2, $vf2\n\t"
            "vmulx.xyzw $vf10, $vf10, $vf2x\n\t"
            ".set reorder"
            : : "f"(*(f32 *)((u8 *)D_004386B8 + 0x74)) : "$2");
    }
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_0045C1E0));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_004386B8));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    if ((flags & 4) != 0) {
        __asm__ volatile(
            ".set noreorder\n\t"
            "mfc1 $2, %0\n\t"
            "qmtc2.ni $2, $vf2\n\t"
            "vaddx.y $vf10, $vf0, $vf2x\n\t"
            ".set reorder"
            : : "f"(-5.0f) : "$2");
    }
}

extern void effMiscQuatMultiplyVU();
extern void camAimRotation();

void effMultiplyQuatWithFlag(u8 *work) {
    s128 rotation;

    if (*(u32 *)(work + 0x68) & 0x60) {
        camAimRotation(work, &rotation);
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" :: "r"(D_004386B8 + 0x50) : "memory");
        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" :: "r"(&rotation) : "memory");
        effMiscQuatMultiplyVU();
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" :: "r"(D_004386B8 + 0x50) : "memory");
        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" :: "r"(work + 0x50) : "memory");
        effMiscQuatMultiplyVU();
    }
}

void effApplyBattleCameraToObject(work)
    void *work;
{
    u128 rotation[2];

    effComputeBattleCameraPositionVU(D_0045C270);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&rotation[0]));
    func_002D3748(work, &rotation[0]);
    effMultiplyQuatWithFlag(D_0045C270);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&rotation[1]));
    func_002D3788(work, &rotation[1]);
    func_002D37C8(work, ((EffBattleCamera *)D_0045C270)->scale * *(f32 *)((u8 *)D_004386B8 + 0x74));
    func_002D3808(work, ((EffBattleCamera *)D_0045C270)->mode);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effQueueEffectFileJob);

typedef struct EffFileJobRequest {
    u8 pad0[4];
    u16 fileKind;
    u8 pad6[2];
    u16 transferMode;
    u8 padA[2];
    void *output;
    u32 size;
    u8 pad14[4];
    u16 resourceMode;
    u8 pad1A[2];
    s32 relatedResource;
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
            func_002D3A68(job, descriptor->relatedResource,
                          descriptor->resourceMode);
        } else {
            s32 zero = 0;
            fileJobSetSecondaryData(job, &zero, 4, 4);
        }
    }
    func_00303C50();
    return job;
}

void func_002FC5C8(u32 work) {
    if (D_004386B4 == 0) {
        effApplyBattleCameraToObject();
        func_002D3710(work);
        return;
    }
}

void effFileJobQueueRelease(u32 job) {
    if (D_004386B4 != 0) {
        fileQueueDestroy(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        fileJobDestroy(D_004386C4);
        D_004386C4 = 0;
    }
    fileJobDestroy(job);
}

extern char D_0042CF58[];
extern char D_0042CF70[];
extern char D_004386D0[];
extern char D_004386D8[];
extern void effUpdateResourceQueue(u32 *, void *, u8 *);
extern void fileWriteToPfs(u32, char *);
extern s32 func_0035C860(char *, char *, ...);
extern void func_002D50D8(u32, char *);
extern void func_002D55B0(u32, char *);

/* Queue output: filename at 0x32 and completion state at 0x64. */
typedef struct EffQueueRecord {
    u8 pad_00[0x32];
    char name[0x32];
    union {
        s32 signedState; // 0x64
        u32 state;
    } completion;
    u8 pad_68[8];
} EffQueueRecord;

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
    u8 request[0x70];
    char path[0x70];
    s32 result = 0x400001;

    effUpdateResourceQueue(D_0042CF58, D_004386D0, request);
    if (((EffQueueRecord *)request)->completion.signedState == 2) {
        result = 0x400000;
    } else if (((EffQueueRecord *)request)->completion.signedState == 1) {
        if (D_004386C0 != 0) {
            func_0035C860(path, D_004386D8, D_0042CF58, request);
            result = 0x400002;
            fileWriteToPfs(D_004386C0, path);
        }
    }
    return result;
}

void func_002FC718(void) {
    func_003002A8();
    effQueueResource(D_004386E0, D_0045C1A0);
}

s32 effPollNamedFile(void) {
    u8 request[0x70];
    char path[0x70];
    s32 result = 0x400001;

    effUpdateResourceQueue(D_0042CF70, D_004386E0, request);
    if (((EffQueueRecord *)request)->completion.signedState == 2) {
        result = 0x400000;
    } else if (((EffQueueRecord *)request)->completion.signedState == 1) {
        if (D_004386B8 != 0) {
            strcpy((char *)D_0045C1A0, ((EffQueueRecord *)request)->name);
            func_0035C860(path, D_004386D8, D_0042CF70, request);
            result = 0x400002;
            func_002D50D8(D_004386B8, path);
        }
    }
    return result;
}

void func_002FC808(void) {
    func_003002A8();
    effQueueResource(D_004386E8, D_0045C1A0);
}

s32 effPollAttachedFile(void) {
    u8 request[0x70];
    char path[0x70];
    s32 result = 0x400001;

    effUpdateResourceQueue(D_0042CF70, D_004386E8, request);
    if (((EffQueueRecord *)request)->completion.signedState == 2) {
        result = 0x400000;
    } else if (((EffQueueRecord *)request)->completion.signedState == 1) {
        if (D_004386B8 != 0) {
            strcpy((char *)D_0045C1A0, ((EffQueueRecord *)request)->name);
            func_0035C860(path, D_004386D8, D_0042CF70, request);
            result = 0x400002;
            func_002D55B0(D_004386B8, path);
        }
    }
    return result;
}

u8 *effFindAssetData(u8 *work) {
    u8 *requested = *(u8 **)(work + 0x90);
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
    u8 *requested = *(u8 **)(work + 0x90);
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

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BED0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF10);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF20);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF30);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF40);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF50);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF60);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF90);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFD0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C000);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C010);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C020);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C040);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C050);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C060);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C070);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C080);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C090);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C100);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C120);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C140);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C160);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C200);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C220);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C240);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C260);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C280);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C300);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C320);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C340);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C360);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C380);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C400);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C420);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C440);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C460);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C480);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C500);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C520);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C548);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C568);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C588);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C608);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C628);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C648);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C668);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C688);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C708);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C728);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C748);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C768);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C788);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C808);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C828);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C848);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C868);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C888);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C908);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C928);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C948);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C968);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C988);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CED8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FCA58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD900);

u32 effReinitializeFileQueue(void) {
    if (D_004386C4 != 0) {
        fileQueueDestroy(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        fileJobDestroy(D_004386C4);
        D_004386C4 = 0;
    }
    if (D_004386B8 != 0) {
        fileQueueDestroy(D_004386B8);
    }
    D_004386B8 = fileQueueCreate();
    D_003FF1C4[0] = 0;
    D_0045C1A0[0] = 0;
    D_004386B0 = 0;
    D_003FFA78[3] = 0;
    D_004386F4 = (s32)D_003FFA78;
    return 0;
}

extern u32 D_003FFA78[];

extern u8 *fileQueueGetAt(u32, u32);
extern u32 D_003FF22C[11];
extern void fileJobCopyHeader(s32 resource, s32 entry);

u32 effResetFileQueueState(void) {
    func_002D4F10(D_004386B8, fileQueueGetAt(D_004386B8, func_002FCA40()));
    D_003FF294[0] = 0;
    D_004386B0 = 0;
    D_003FFA78[3] = 0;
    D_004386F4 = (s32)D_003FFA78;
    return 0;
}

u32 effFinalizeQueuedFile(void) {
    s32 resource;
    s32 entry;
    entry = (s32)fileQueueGetAt(D_004386B8, func_002FCA40());
    resource = fileJobDuplicateAfter(D_004386B8, entry);
    D_003FF22C[0] = 0;
    fileJobCopyHeader(resource, entry);
    D_004386F4 = (s32)D_003FFA78;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDDD8);

u32 func_002FE208(void) {
    D_00439075 = 1;
    func_002FE230(0);
    return 0;
}

u32 func_002FE230(void) {
    if (*(s32 *)(D_004386F0 + 0x34) != 0) {
        D_004386F4 = *(s32 *)(D_004386F0 + 0x34);
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
extern EffectFileSelection D_003FF4B0;
extern u8 D_003FF390[], D_003FF3D8[], D_0045C270[], D_00439074;

typedef struct EffectBlock90 {
    u32 word[36];
} EffectBlock90;

typedef struct EffectAlignedBlock128 {
    u64 word[16];
} EffectAlignedBlock128;

u32 effLoadBattleCameraSnapshot(u32 arg0) {
    u8 *record = fileQueueGetAt(D_004386B8, func_002FCA40());

    *(EffectBlock90 *)D_0045C110 = *(EffectBlock90 *)record;
    *(EffectAlignedBlock128 *)D_0045C110 = *(EffectAlignedBlock128 *)D_0045C270;
    if (((EffectBlock90 *)D_0045C110)->word[26] & 2) {
        D_003FF4B0.table = D_003FF390;
        D_003FF4B0.count = 3;
        if (D_003FF4B0.selected > D_003FF4B0.count) {
            D_003FF4B0.selected = 0;
        }
    } else {
        D_003FF4B0.table = D_003FF3D8;
        D_003FF4B0.count = 9;
        if (D_003FF4B0.selected == 0) {
            D_003FF4B0.selected = 1;
        }
    }
    return arg0;
}


u32 effStoreBattleCameraSnapshot(u32 arg0) {
    u8 *record = fileQueueGetAt(D_004386B8, func_002FCA40());

    *(EffectBlock90 *)record = *(EffectBlock90 *)D_0045C110;
    *(EffectAlignedBlock128 *)D_0045C270 = *(EffectAlignedBlock128 *)D_0045C110;
    D_00439074 = 1;
    if (((EffectBlock90 *)D_0045C110)->word[26] & 2) {
        D_003FF4B0.table = D_003FF390;
        D_003FF4B0.count = 3;
        if (D_003FF4B0.selected > D_003FF4B0.count) {
            D_003FF4B0.selected = 0;
        }
    } else {
        D_003FF4B0.table = D_003FF3D8;
        D_003FF4B0.count = 9;
        if (D_003FF4B0.selected == 0) {
            D_003FF4B0.selected = 1;
        }
    }
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE5B8);

extern void func_002FE5B8(const char *, const void *, s32);
extern char D_0042CEF8[];
extern u8 D_003FE280[];

void func_002FE948(void) {
    func_002FE5B8(D_0042CEF8, D_003FE280, 0xF);
}

extern char D_0042CED8[];
extern u8 D_003FE430[];

void func_002FE970(void) {
    func_002FE5B8(D_0042CED8, D_003FE430, 7);
}

extern u8 D_003FE500[];

void effCreatePolyTrackTask(void) {
    func_002FE5B8("POLY TRACK S", D_003FE500, 7);
}

extern char D_00437F30[];
extern u8 D_003FE610[];

void func_002FE9C0(void) {
    func_002FE5B8(D_00437F30, D_003FE610, 7);
}

extern char D_004386A8[];
extern u8 D_003FE6E0[];

void func_002FE9E8(void) {
    func_002FE5B8(D_004386A8, D_003FE6E0, 3);
}

extern char D_0042CE48[];
extern u8 D_003FE7C0[];

void func_002FEA10(void) {
    func_002FE5B8(D_0042CE48, D_003FE7C0, 5);
}

extern char D_0042CEB8[];
extern u8 D_003FE780[];

void func_002FEA38(void) {
    func_002FE5B8(D_0042CEB8, D_003FE780, 2);
}

extern char D_0042CEA8[];
extern u8 D_003FE850[];

void func_002FEA60(void) {
    func_002FE5B8(D_0042CEA8, D_003FE850, 0xB);
}

extern char D_0042CE98[]; /* "POLY RING" */

extern u8 D_003FE990[];

void func_002FEA88(void) {
    func_002FE5B8(D_0042CE98, D_003FE990, 6);
}

extern char D_0042CE88[];

extern u8 D_003FEA40[];

void func_002FEAB0(void) {
    func_002FE5B8(D_0042CE88, D_003FEA40, 5);
}

extern u8 D_003FEAD0[];

void func_002FEAD8(void) {
    func_002FE5B8("POLY TWINKLE", D_003FEAD0, 6);
}

extern char D_0042CE78[];

extern u8 D_003FEB80[];

void func_002FEB00(void) {
    func_002FE5B8(D_0042CE78, D_003FEB80, 7);
}

extern char D_0042CE68[];

extern u8 D_003FEC50[];

void func_002FEB28(void) {
    func_002FE5B8(D_0042CE68, D_003FEC50, 5);
}

extern char D_0042CE58[];

extern u8 D_003FED20[];

void func_002FEB50(void) {
    func_002FE5B8(D_0042CE58, D_003FED20, 2);
}

extern char D_0042CDF8[];

extern u8 D_003FED60[];

void func_002FEB78(void) {
    func_002FE5B8(D_0042CDF8, D_003FED60, 0x12);
}

extern char D_00438640[]; /* "2D" */

extern u8 D_003FEFE0[];

void func_002FEBA0(void) {
    func_002FE5B8(D_00438640, D_003FEFE0, 2);
}

extern char D_0042CEE8[];

extern u8 D_003FF020[];

void func_002FEBC8(void) {
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
        if (D_004386B8 != 0) {
            entry = fileAppendJobFromEntry(D_004386B8, request);
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
        if (D_004386B8 != 0) {
            fileQueueDestroy(D_004386B8);
        }
        strcpy((char *)D_0045C1A0, (char *)record + 0xC8);
        D_004386B8 = func_002D5AA8(record);
        D_0045C1F0 = *(EffectBlock128 *)D_004386B8;
        D_004386B0 = 0;
        D_003FFA84[0] = 0;
        result = 0x400002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D048);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEDD0);

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

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF0B8);

void func_002FF338(void) {
    D_00438758 = D_0045C110[0x88];
    D_00438759 = D_0045C110[0x89];
    D_0043875A = D_0045C110[0x8A];
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF360);

s32 func_002FF870(void) {
    D_0045C110[0x88] = 8;
    D_0045C110[0x89] = 0;
    D_0045C110[0x8a] = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_0045C300) : "memory");
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF8A0);

u32 effLoadFileSlotAndPoll(void) {
    u8 *file = fileQueueGetAt(D_004386B8, func_002FCA40());
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

u32 func_002FFE48(void) {
    u8 *file = fileQueueGetAt(D_004386B8, func_002FCA40());
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
    if (D_004386B8 != 0) {
        fileQueueDestroy(D_004386B8);
        D_004386B8 = 0;
    }
    if (D_004386B4 != 0) {
        fileQueueDestroy(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        fileJobDestroy(D_004386C4);
        D_004386C4 = 0;
    }
}

void effBattleResourceDescriptorRelease(void) {
    if (D_00438760 != 0) {
        btlDestroyResourceDescriptor(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        btlDestroyEntryList(D_0043875C);
        D_0043875C = 0;
    }
}

void effPollResourceBankSlot(char *path, u32 unused, EffResourceBankSlot *slot) {
    if (D_0043875C == 0) {
        D_0043875C = btlScanDirectory();
        D_00438760 = btlCreateResourceDescriptor(D_0043875C);
        btlSetResourceNameHeaderPair(D_00438760, 0xBA, 0x1C);
    } else {
        func_0020DAB8(D_00438760);
        slot->state = func_0020DFC8(D_00438760);
        slot->type = btlFormatSelectedResourceName(D_00438760, slot);
        slot->count = btlGetResourcePathVariant(D_00438760);
        btlTrimResourceName(D_00438760, slot->name);
        if (slot->state == 1) {
            btlDestroyResourceDescriptor(D_00438760);
            D_00438760 = 0;
            btlDestroyEntryList(D_0043875C);
            D_0043875C = 0;
        }
    }
}

extern s32 D_00438764;

extern u32 D_004384E8[];

void effInitializeResourceQueue(void) {
    u8 *queueFile;

    if (D_00438764 != 0) {
        func_0020E368(D_00438764);
    }
    D_00438764 = btlCreateResourceNameRecord(D_004384E8);
    btlSetResourceNameHeaderPairAlternate(D_00438764, 0xC2, 0xC8);
    func_0020E850(D_00438764, 9);
    queueFile = fileQueueGetAt(D_004386B8, func_002FCA40());
    btlResourceRecordSetName(D_00438764, queueFile + 0x9C);
}

u32 effPollResourceQueue(void) {
    u32 result;
    func_0020E380(D_00438764);
    btlFormatResourceNameWithoutPrefix(D_00438764, fileQueueGetAt(D_004386B8, func_002FCA40()) + 0x9C);
    result = func_0020E7B0(D_00438764);
    if ((u32)(result - 1) < 2) {
        func_0020E368(D_00438764);
        D_00438764 = 0;
        return 0;
    }
    return 0x200001;
}

void func_003002A8(void) {
    if (D_00438768 != 0) {
        func_0020E368(D_00438768);
        D_00438768 = 0;
    }
}

void effQueueResource(s32 unused, s32 entry) {
    s32 queue = D_00438768;
    if (queue == 0) {
        queue = btlCreateResourceNameRecord();
        D_00438768 = queue;
        btlSetResourceNameHeaderPairAlternate(queue, 0xC2, 0xC8);
    }
    btlResourceRecordSetName(D_00438768, entry);
}

void effUpdateResourceQueue(u32 *result, void *queueData, u8 *record) {
    u32 state;

    if (D_00438768 == 0) {
        D_00438768 = btlCreateResourceNameRecord(queueData);
        btlSetResourceNameHeaderPairAlternate(D_00438768, 0xC2, 0xC8);
        return;
    }
    func_0020E380(D_00438768);
    btlFormatResourceNameWithPrefix(D_00438768, record);
    btlFormatResourceNameWithoutPrefix(D_00438768, record + 0x32);
    state = func_0020E7B0(D_00438768);
    ((EffQueueRecord *)record)->completion.state = state;
    if (state == 1) {
        if (func_0020E858(D_00438768, result) != 0) {
            func_0020E368(D_00438768);
            D_00438768 = 0;
        } else {
            ((EffQueueRecord *)record)->completion.state = 0;
        }
    }
}

void func_003003E0(void) {
    if (D_00438760 != 0) {
        btlDestroyResourceDescriptor(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        btlDestroyEntryList(D_0043875C);
        D_0043875C = 0;
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
    if (D_0043875C == 0) {
        u32 i;
        u32 count;
        char name[0x70];

        D_0043875C = btlScanDirectory(0, mode);
        if (mode & 8) {
            count = func_00159BB0();
            for (i = 0; i < count; i++) {
                func_0035C860(name, "GENERAL %d", i);
                btlAppendEntry(D_0043875C, name, 8, i, 0);
            }
        }
        D_00438760 = btlCreateResourceDescriptor(D_0043875C);
        btlSetResourceNameHeaderPair(D_00438760, 0xBA, 0x1C);
    } else {
        func_0020DAB8(D_00438760);
        status->state = func_0020DFC8(D_00438760);
        status->type = btlFormatSelectedResourceName(D_00438760, status);
        status->count = btlGetResourcePathVariant(D_00438760);
        if (status->state == 1) {
            btlDestroyResourceDescriptor(D_00438760);
            D_00438760 = 0;
            btlDestroyEntryList(D_0043875C);
            D_0043875C = 0;
        }
    }
}

void effResetMappingFlags(void) {
    D_0043876C = 0;
    D_00438778 = 0;
    D_00438770 = 0;
    D_00438774 = 1;
    D_00400018.field0C = 1;
    D_00400018.field10 |= 0x10;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300578);

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
    u8 pad_3C[0x34];
    u32 value70;       // 0x70
    u8 pad_74[0x0C];
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

s32 func_003012B0(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x2c, object + 0x50, *(s32 *)(object + 0xb8));
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
    return result;
}

s32 func_00301318(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
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

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
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

    D_00400018.table = D_003FFE98;
    D_00400018.count = 8;
    result = func_00300578(object, object + 0x24, *(s32 *)(object + 0x34));
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
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

s32 func_00301528(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x4c, object + 0x70, ((EffMappingObject *)object)->valueD8);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
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

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x68, object + 0x8c, ((EffMappingObject *)object)->valueF4);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
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

    D_00400018.table = D_003FFE98;
    D_00400018.count = 8;
    result = func_00300578(object, object + 0x24, ((EffMappingObject *)object)->value8C);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
    return result;
}

s32 func_00301750(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x48, object + 0x6c, ((EffMappingObject *)object)->valueD4);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
    return result;
}

s32 func_003017B8(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x7c, object + 0xa0, ((EffMappingObject *)object)->value108);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
    return result;
}

s32 func_00301820(s32 request) {
    s32 result;
    s32 object = ((EffMappingRequest *)request)->object;

    D_00400018.table = D_003FFF58;
    D_00400018.count = 8;
    result = func_00300578(object + 0x24, object + 0x48, *(s32 *)object);
    D_00400018.table = D_003FFDD8;
    D_00400018.count = 8;
    return result;
}

void btlResetEffectWork(void) {
    u8 *first = D_00400150;
    u8 *second = D_00400250;

    *(u32 *)(first + 0x10) |= 0x10;
    *(u32 *)(first + 0x0c) = 0;
    *(u32 *)(second + 0x10) |= 0x10;
    *(u32 *)(second + 0x0c) = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003018C8);

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

void func_00302598(void) {
    D_00438774 = 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003025A8);

void func_00302AD0(s32 work) {
    func_003025A8(*(s32 *)(work + 0xc) + 0x3c, *(u32 *)(*(s32 *)(work + 0xc) + 0x4c));
}

void func_00302AF0(s32 work) {
    func_003025A8(*(s32 *)(work + 0xc) + 0x60, *(s32 *)(*(s32 *)(work + 0xc) + 0x3c) + 1);
}

void func_00302B18(s32 work) {
    func_003025A8(*(s32 *)(work + 0xc) + 0x70, *(s32 *)(*(s32 *)(work + 0xc) + 0x8c) + 1);
}

void func_00302B40(s32 work) {
    func_003025A8(*(s32 *)(work + 0xc) + 0x70, *(s32 *)(*(s32 *)(work + 0xc) + 0x8c) + 1);
}

void func_00302B68(s32 work) {
    func_003025A8(*(s32 *)(work + 0xc) + 0x60, *(s32 *)(*(s32 *)(work + 0xc) + 0x74) + 1);
}

void func_00302B90(s32 work) {
    func_003025A8(*(u32 **)(work + 0xc) + 4, **(u32 **)(work + 0xc));
}

extern u32 D_004386C8;

s32 effPollFileQueueRecord(s32 mode) {
    u8 status[0xE0];
    u8 *entry;
    s32 result = 0x600001;

    effPollResourceBank(mode, (EffBankStatus *)status);
    if (((EffBankStatus *)status)->state == 2) {
        result = 0x400000;
    } else if (((EffBankStatus *)status)->state == 1) {
        if (((EffBankStatus *)status)->type != 8) {
            entry = fileQueueGetAt(D_004386B8, -((EffBankStatus *)status)->count);
            if (D_004386C8 != (u32)entry) {
                func_002D4E60(D_004386B8, D_004386C8);
                func_002D4CF0(D_004386B8, D_004386C8, entry);
            }
        } else {
            func_002D4E60(D_004386B8, D_004386C8);
            fileJobSetSecondaryData(D_004386C0, status + 0xD0, 4, 4);
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
        func_002D4E60(D_004386B8, D_004386C8);
        if (((EffResourceBankSlot *)status)->type != 8) {
            func_002D3A68(D_004386C0, status,
                           func_00303C78(((EffResourceBankSlot *)status)->type));
        } else {
            fileJobSetSecondaryData(D_004386C0, status + 0x104, 4, 4);
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
    fileJobSetSecondaryData(D_004386C0, &zero, 4, 4);
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
        func_002D3A68(job, fileInfo, func_00303C78(((EffResourceBankSlot *)fileInfo)->type));
        entry = (u8 *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (s32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = *(u32 *)(entry + 0x90);
        resource = (u8 *)effFindAssetData(entry);
        strcpy((char *)(entry + 0x9C), *(char **)resource);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(*(void **)(resource + 0xC), fileData,
               *(u32 *)(resource + 0x10));
        D_004386BC = effQueueEffectFileJob(resource);
        D_004386F4 = effFindAssetObject(entry);
        *(u8 **)(D_004386F4 + 0x34) = (u8 *)D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}
extern u8 D_0045C270[];
extern u32 D_004386C8;
extern u32 D_004386BC;
extern void func_00303C50(void);

extern u8 D_003F0DA8[] __attribute__((aligned(4)));

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

typedef struct EffFileJobEntry {
    u8 pad0[0x90];
    u32 fileHandle;
    u8 pad94[8];
    char filename[1];
} EffFileJobEntry;

u32 effQueueGeneratedFileJob(void) {
    EffFileQueryInfo fileInfo;
    u32 result;
    s32 status;
    u8 *job;
    EffFileJobEntry *entry;
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
        dataLength = func_0033EB30(command);
        totalLength = dataLength + headerBytes;
        allocation = func_003292A8(totalLength);
        buffer = (u8 *)sdfResourceRetainAddress(allocation);
        memset(buffer, 0, headerBytes);
        func_0033EB10(command, buffer + headerBytes, dataLength);
        func_0033EAE0(command);
        fileJobSetPrimaryData(job, buffer, totalLength, 1);
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->filename, resource->name);
        memcpy(D_0045C270, entry, 0x80);
        queuedFile = entry->fileHandle;
        oldAllocation = resource->allocationHandle;
        D_004386C0 = queuedFile;
        if (oldAllocation != 0) {
            func_003297C8(oldAllocation);
        }
        resource->allocationHandle = allocation;
        resource->buffer = buffer;
        resource->size = dataLength + headerBytes;
        resource->mode = 1;
        memcpy(D_00459E30, D_003F0DA8, 0x74);
        D_004386BC = effQueueEffectFileJob(resource);
        D_004386F4 = effFindAssetObject(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003F9060;

u32 func_00303478(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
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
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        D_004386F4 = effFindAssetObject(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0B8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0D8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0F8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D108);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D118);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D128);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D140);

u32 effLoadFileSlotF2(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
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
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        D_004386F4 = effFindAssetObject(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003FD988;

u32 effLoadMaterialFile(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
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
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        D_004386F4 = effFindAssetObject(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003FE040;

u32 func_00303A30(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
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
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)effFindAssetData(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = effQueueEffectFileJob(resource);
        D_004386F4 = effFindAssetObject(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

void func_00303C50(void) {
    D_0043876C = 0;
    D_00438774 = 1;
    D_00438770 = 0;
    D_00438778 = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

s32 func_00303C78(s32 mask) {
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

void *func_00303D00(u32 value) {
    u8 *record = (u8 *)func_00328D68(0x14);
    memset(record, 0, 0x14);
    *(u32 *)record = value;
    *(u32 *)(record + 4) = 0;
    *(u32 *)(record + 8) = 0;
    *(u32 *)(record + 0xC) = 0;
    return record;
}

void func_00303D58(void) {
    func_00328E48();
}

u32 func_00303D70(u32 *value) {
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
extern EffRequest *func_002C7FF0(u32);
extern void func_002C81D0(void *);

u32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference) {
    EffectListNode *node = (EffectListNode *)func_00328D68(0x18);
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
    func_00328E48(node);
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
                list->request = func_002C7FF0(node->length);
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
                            func_003297C8(buffer);
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

u32 effLoadIndexedResource(s32 category, s32 index, s32 preserve) {
    char path[0x80];
    u32 source;
    u32 handle;
    u32 resource;
    func_0035C860(path, D_004387E8, category, index);
    handle = func_00343ED0(path, &source, 0);
    resource = func_00305148(handle, preserve);
    if (preserve == 0) {
        func_003297C8(handle);
    }
    return resource;
}

void effCompleteTransientResourceJob(u64 job, u32 *result) {
    u64 handle;
    u32 resource;

    handle = fileGetResourceHandle();
    resource = func_00305148(handle, 0);
    *result = resource;
    func_003297C8(handle);
    func_002C7D00(job);
}

void effCompleteRetainedResourceJob(u64 job, u32 *result) {
    u64 handle;
    u32 resource;

    handle = fileGetResourceHandle();
    resource = func_00305148(handle, 1);
    *result = resource;
    func_002C7D00(job);
}

void effRequestResourceByMode(s32 category, s32 index, s32 mode, u32 *result) {
    char path[0x80];
    func_0035C860(path, D_004387E8, category, index);
    *result = 0;
    if (mode == 1) {
        func_002C8040(path, 0, effCompleteRetainedResourceJob, result);
    } else {
        func_002C8040(path, 0, effCompleteTransientResourceJob, result);
    }
}

u32 effLoadMappedResource(s32 category, s32 index) {
    char path[0x80];
    u32 buffer;
    u32 handle;
    u32 resource;
    func_0035C860(path, D_004387E8, category, index);
    handle = func_00343ED0(path, &buffer, 0);
    resource = effCreateMappedResource(buffer);
    func_003297C8(handle);
    return resource;
}

void effCompleteMappedResourceJob(u64 job, u32 *result) {
    u64 handle;
    u32 buffer;
    u32 resource;

    handle = fileGetResourceHandle();
    buffer = sdfResourceRetainAddress(handle);
    resource = effCreateMappedResource(buffer);
    *result = resource;
    func_003297C8(handle);
    func_002C7D00(job);
}

void effRequestMappedResource(s32 category, s32 index, u32 *result) {
    char path[0x80];
    func_0035C860(path, D_004387E8, category, index);
    *result = 0;
    func_002C8040(path, 0, effCompleteMappedResourceJob, result);
}

void *effCreateOwnerRecordList(u32 value) {
    u8 *record = (u8 *)func_00328D68(0x44);
    memset(record, 0, 0x44);
    *(u32 *)record = value;
    return record;
}

void effInsertSlotRecord(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slot) {
    EffectSlot *entry = &work->slots[slot];
    s32 bucket = entry->bucket;
    EffectRecord *record;

    if (bucket >= 16) {
        bucket = 15;
    }
    record = func_00328D68(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slot;
    record->next = list->entries[bucket];
    list->entries[bucket] = record;
}

typedef struct EffectRecordNode {
    u32 value;
    u32 index;
    struct EffectRecordNode *previous;
    struct EffectRecordNode *next;
} EffectRecordNode;

s32 effRemoveSlotRecord(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slot) {
    EffectSlot *slotData = &work->slots[slot];
    s32 bucket = slotData->bucket;
    EffectRecord *record = list->entries[bucket];

    while (record != 0) {
        if (record->slot == slot) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == list->entries[bucket]) {
                list->entries[bucket] = record->next;
            }
            func_00328E48(record);
            return 1;
        }
        record = record->next;
    }
    return 0;
}

void effReleaseRecordBuckets(u8 *buckets) {
    EffectRecordNode **head = (EffectRecordNode **)(buckets + 4);
    s32 index;
    for (index = 0xF; index >= 0; index--, head++) {
        EffectRecordNode *node = *head;
        while (node != 0) {
            if (node->previous != 0) {
                node->previous->next = node->next;
            }
            if (node->next != 0) {
                node->next->previous = node->previous;
            }
            if (node == *head) {
                *head = node->next;
            }
            func_00328E48(node);
            node = node->next;
        }
    }
}

s32 effDispatchRecordBuckets(s32 refresh, u8 *buckets, s32 context) {
    EffectRecordNode **head = (EffectRecordNode **)(buckets + 4);
    s32 index;
    for (index = 0xF; index >= 0; index--, head++) {
        EffectRecordNode *node = *head;
        while (node != 0) {
            func_00306F80(0, 0, 0, 0, *(u32 *)buckets, node->index, context);
            if (refresh != 0) {
                itfGridLookupValueOrDefault(*(u32 *)buckets, node->index);
            }
            node = node->next;
        }
    }
    return 1;
}

u32 effDestroyOwnerRecordList(u32 buckets) {
    effReleaseRecordBuckets((u8 *)buckets);
    func_00328E48(buckets);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effConvertParamValue);

u32 effSumRecordStatuses(u8 *buffer) {
    EffRecordBucket *record = &D_00400508[*(u32 *)(buffer + 0x14)];
    u32 total = 0;
    u32 i;

    for (i = 0; i < record->count; i++) {
        total += effConvertParamValue(record->records + i * 0x18 + 4, 0, 0, 0);
    }
    return total;
}

typedef struct EffMappedHeader {
    u8 pad_00[0x14];
    u32 count;        // 0x14
    u8 pad_18[8];
} EffMappedHeader;    // 0x20

typedef struct EffMappedRecord {
    u8 pad_00[0x18];
    u32 size;         // 0x18
    u8 pad_1C[4];
    u8 *status;       // 0x20
} EffMappedRecord;    // 0x24

void *func_00304768(u8 *source, EffMappedHeader *headerOut) {
    EffMappedHeader header;
    u32 allocation;
    EffMappedRecord *records;
    u32 index = 0;
    u32 needed;

    memcpy(&header, source, sizeof(header));
    source += sizeof(header);
    allocation = func_003292A8(header.count * 0x24);
    records = (EffMappedRecord *)sdfResourceRetainAddress(allocation);
    for (; index < header.count; index++) {
        EffMappedRecord *record = &records[index];

        memcpy(record, source, 0x20);
        source += 0x20;
        needed = effSumRecordStatuses((u8 *)records);
        if (needed < record->size) {
            needed = record->size;
        }
        record->status = func_00328D68(needed);
        memset(record->status, 0, needed);
        memcpy(record->status, source, record->size);
        source += record->size;
        if (record->size < needed) {
            record->size = needed;
        }
    }
    if (headerOut != 0) {
        memcpy(headerOut, &header, sizeof(header));
    }
    return (void *)allocation;
}

typedef struct EffMappedResource {
    u32 count;        // 0x00
    void *records;    // 0x04
    void *allocation; // 0x08
} EffMappedResource;

u32 effCreateMappedResource(u32 source) {
    EffMappedResource *work = (EffMappedResource *)func_00328D68(0xC);
    EffMappedHeader header;

    work->records = func_00304768((u8 *)source, &header);
    work->allocation = (void *)sdfResourceRetainAddress((u32)work->records);
    work->count = header.count;
    return (u32)work;
}

u32 *effCreateStatusBatch(u32 category) {
    u32 *batch = (u32 *)func_00328D68(0xC);
    u32 allocation;
    u32 amount;
    u8 *statuses;

    batch[0] = 1;
    allocation = func_003292A8(0x24);
    batch[1] = allocation;
    batch[2] = sdfResourceRetainAddress(allocation);
    memset((void *)batch[2], 0, 0x24);
    {
        u8 *buffer = (u8 *)batch[2];
        *(u32 *)(buffer + 0x14) = category;
        amount = effSumRecordStatuses(buffer);
    }
    statuses = (u8 *)func_00328D68(amount);
    *(u8 **)(batch[2] + 0x20) = statuses;
    memset(statuses, 0, amount);
    *(u32 *)(batch[2] + 0x18) = amount;
    return batch;
}

s32 effDestroyPackedBatch(s32 *batch) {
    s32 i;

    for (i = 0; i < batch[0]; i++) {
        func_00328E48(*(u32 *)(*(u8 **)&batch[2] + i * 0x24 + 0x20));
    }
    func_003297C8(batch[1]);
    func_00328E48(batch);
    return 1;
}

/* Resource slot set: source records and 0xA0-byte live entries. */
typedef struct EffSlotSet {
    u8 pad00[8];
    u32 count;        /* 0x08 */
    u8 pad0C[4];
    u32 sourceRecords; /* 0x10 */
    u32 allocation;    /* 0x14 */
    u32 entries;       /* 0x18 */
    u32 entryCount;    /* 0x1C */
} EffSlotSet;

u32 effReleaseSlotWorkAllocation(s32 owner) {
    func_003297C8(((EffSlotSet *)owner)->allocation);
    return 1;
}

s32 effGetSlotWorkOrOverride(s32 owner, s32 index) {
    s32 override;
    s32 entry;

    entry = index * 0xa0 + ((EffSlotSet *)owner)->entries;
    override = *(s32 *)(entry + 0x9c);
    if (override != 0) {
        entry = override;
    }
    return entry;
}

u32 effInitializeSlotPhase(u32 *state) {
    u32 active = state[0] & 1;
    if (active != 0) {
        state[1] = 0;
    } else {
        active = 0x10000;
        state[1] = active;
    }
    return active;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304B18);

void effInitializeSlotWork(s32 owner, s32 index) {
    func_00304B18(owner, index, ((EffSlotSet *)owner)->entries + index * 0xa0);
}

extern void effResetSlotWork(u32, u32);

void effInitializeAllSlotWork(u8 *owner) {
    u32 index = 0;
    if (((EffSlotSet *)owner)->count != 0) {
        do {
            effResetSlotWork((u32)owner, index++);
        } while (index < ((EffSlotSet *)owner)->count);
    }
}

void effAttachSlotWorkOwner(a, b, obj)
    u32 a;
    u32 b;
    u32 *obj;
{
    obj[0x60 / 4] = a;
    obj[0x64 / 4] = b;
    func_00304B18(a, b, obj);
}

void effResetSlotWork(u32 owner, u32 index) {
    s32 entry;

    entry = ((EffSlotSet *)owner)->entries + (s32)index * 0xa0;
    memset(entry, 0, 0xa0);
    effAttachSlotWorkOwner(owner, index, entry);
}

typedef struct EffResourceSet {
    u8 pad_00[0x1C];
    u32 count;          // 0x1C
    u8 pad_20[4];
    void **slots;       // 0x24
} EffResourceSet;

u8 *effResolveResourceSlots(EffResourceSet *owner, u8 *data, s32 release, s32 only) {
    u32 i = 0;
    u8 *entry = data;

    entry += *(u32 *)(entry + 0xC);

    if (owner->count != 0) {
        do {
            s32 *resource = (s32 *)(data + *(u32 *)(entry + 4));

            entry += 8;
            if (release == 0) {
                if (only == -1 || only == i) {
                    if (owner->slots[i] == 0) {
                        owner->slots[i] = (void *)func_0032C138(resource);
                    }
                } else {
                    owner->slots[i] = 0;
                }
            } else {
                owner->slots[i] = 0;
            }
            i++;
        } while (i < owner->count);
    }
    return entry;
}

void effResolveAndReleaseResource(u32 *owner) {
    if (owner[0] != 0) {
        u32 resource = owner[0];
        u32 mapped = sdfResourceRetainAddress(resource);
        effResolveResourceSlots(owner, mapped, 0, -1);
        func_00329910(owner[0]);
    }
}

void effResolveAndReleaseSelectedResource(u32 *owner, s32 mapping) {
    if (owner[0] != 0) {
        u32 resource = owner[0];
        u32 mapped = sdfResourceRetainAddress(resource);
        effResolveResourceSlots(owner, mapped, 0, mapping);
        func_00329910(owner[0]);
    }
}

extern void sdfTexReleaseReference(s32, u32, u32);

void func_00304FB0(u8 *owner, s32 preserve) {
    u32 i = 0;
    u32 count = ((EffSlotSet *)owner)->entryCount;
    u32 *resources;

    if (count != 0) {
        resources = *(u32 **)(owner + 0x24);
        do {
            if (resources[i] != 0) {
                u32 *current;
                sdfTexReleaseReference(resources[i], (u32)resources, count);
                current = *(u32 **)(owner + 0x24);
                count = *(u32 *)(owner + 0x1C);
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
    func_00304FB0(owner, 0);
}

u8 effHasFirstTextureHandle(s32 owner) {
    return **(s32 **)(owner + 0x24) != 0;
}

u32 *effCreatePayload(u32 count) {
    u32 bytes = count * 0x6C;
    u32 *payload = (u32 *)func_00328D68(0xC);
    u32 allocation = func_003292A8(bytes);
    payload[1] = count;
    payload[0] = allocation;
    payload[2] = sdfResourceRetainAddress(allocation);
    memset((void *)payload[2], 0, bytes);
    return payload;
}

u32 effDestroyPayload(u32 payload) {
    func_003297C8(*(u32 *)payload);
    func_00328E48(payload);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305148);

u32 *effCreateResourceSlotSet(u32 *source, u32 slot, u32 count) {
    u32 *effect = (u32 *)func_00328D68(0x30);
    u32 index = 0;
    effect[1] = 1;
    {
        u32 mode = source[7];
        u32 size = source[9];
        effect[7] = mode;
        effect[9] = size;
    }
    effect[0] = 0;
    effect[8] = 0;
    effect[2] = count;
    effect[3] = func_003292A8(count * 0x80);
    effect[4] = sdfResourceRetainAddress(effect[3]);
    effect[5] = func_003292A8(effect[2] * 0xA0);
    effect[6] = sdfResourceRetainAddress(effect[5]);
    if (effect[2] != 0) {
        do {
            memcpy((void *)(effect[4] + index * 0x80),
                   (void *)(source[4] + slot * 0x80), 0x80);
            effResetSlotWork((u32)effect, index);
            index++;
        } while (index < effect[2]);
    }
    return effect;
}

u32 effDestroyResourceSlotSet(u32 effect) {
    s32 *piVar1;

    piVar1 = (s32 *)effect;
    if (*piVar1 != 0) {
        func_003297C8(*piVar1);
    }
    if (piVar1[1] == 0) {
        effReleaseTextureHandlesAndResetSlots(effect);
        func_003297C8(piVar1[8]);
    }
    func_003297C8(piVar1[3]);
    effReleaseSlotWorkAllocation(effect);
    func_00328E48(effect);
    return 1;
}

u32 effSetSlotResourceAndFlags(u32 *effect, u32 resource, u32 flags) {
    *effect = flags;
    effect[4] = resource;
    if ((flags & 2) != 0) {
        effInitializeSlotPhase(effect);
    }
    effect[2] = effect[2] + 1;
    return 1;
}

u32 effSetSlotIndexedResource(u32 effect, s32 owner, s32 index, u32 flags) {
    effSetSlotResourceAndFlags(effect, *(s32 *)(owner + 8) + index * 0x24, flags);
    return 1;
}

u32 effSlotTransitionClearTarget(s32 work, u32 unused) {
    *(u32 *)(work + 0x10) = 0;
    return 1;
}

s32 effClampSlotPhaseAtEnd(u32 effect, u32 slot, u32 *state) {
    if (0x10000 < (s32)state[1]) {
        u32 flags = state[0];
        state[1] = 0x10000;
        if (flags & 4) {
            if (flags & 8) {
                state[0] = flags & ~1;
            } else {
                effInitializeSlotWork(effect, slot);
            }
            return 0;
        }
    }
    return 1;
}

s32 effClampSlotPhaseAtStart(u32 effect, u32 slot, u32 *state) {
    if ((s32)state[1] < 0) {
        u32 flags = state[0];
        state[1] = 0;
        if (flags & 4) {
            if (flags & 8) {
                state[0] = flags | 1;
            } else {
                effInitializeSlotWork(effect, slot);
            }
            return 0;
        }
    }
    return 1;
}

typedef struct EffTimedState {
    u32 flags;      // 0x00
    s32 value;      // 0x04
    s32 delay;      // 0x08
    s32 delayMax;   // 0x0C
    u8 *source;     // 0x10
} EffTimedState;    // 0x14

typedef struct EffStateSource {
    u8 pad_00[0x14];
    u32 kind;       // 0x14
} EffStateSource;

extern u32 effResetRecordRun(u8 *, u32, u32);

u8 *effUpdateTimedStates(u8 *effect, u32 slot, u8 *entry) {
    EffTimedState *states = (EffTimedState *)(entry + 0x28);
    u8 *record = *(u8 **)(effect + 0x18) + slot * 0xA0;
    s32 idle = 1;
    u32 i;

    for (i = 0; i < 2; i++) {
        EffTimedState *state = &states[i];
        EffStateSource *source = (EffStateSource *)state->source;

        if (source != 0 && source->kind != 0) {
            EffRecordBucket *bucket = &D_00400508[source->kind];
            s32 step = bucket->step(record, entry, state);

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
    u32 offset = slot * 0xA0;
    if (*(u32 *)(offset + ((EffSlotSet *)effect)->entries + 0x9C) == 0) {
        effAttachSlotWorkOwner(effect, slot);
    }
    *(u32 *)(offset + ((EffSlotSet *)effect)->entries + 0x9C) = material;
    return 1;
}

u32 effSetMaterialSlots(s32 work, s32 index, u32 value, BdWork *asset) {
    s32 offset = index * 0xA0;
    u32 i;
    EffectMaterialSlot *slots;
    if (*(void **)(offset + ((EffSlotSet *)work)->entries + 0x9C) == NULL) {
        effAttachSlotWorkOwner((void *)work, index, asset);
    }
    *(BdWork **)(offset + ((EffSlotSet *)work)->entries + 0x9C) = asset;
    slots = (EffectMaterialSlot *)((u8 *)asset + 0x30);
    for (i = 0; i < 2; i++) {
        slots[i].value = value;
    }
    return 1;
}

u32 effClearSlotOverrideWork(s32 effect, s32 slot) {
    *(u32 *)(slot * 0xa0 + ((EffSlotSet *)effect)->entries + 0x9c) = 0;
    return 1;
}

s32 effConfigureSlotResource(u8 *effect, u32 slot, u32 resource, u32 flags) {
    u8 *entry = (u8 *)((EffSlotSet *)effect)->entries + slot * 0xA0;
    effSetSlotResourceAndFlags((u32 *)(entry + 0x28), resource, flags);
    effUpdateTimedStates(effect, slot, entry);
    return 1;
}

s32 effConfigureIndexedSlotResource(u8 *effect, u32 slot, u8 *resources, u32 index, u32 flags) {
    u8 *entry = (u8 *)((EffSlotSet *)effect)->entries + slot * 0xA0;
    u32 resource = *(u32 *)(resources + 8) + index * 0x24;
    effSetSlotResourceAndFlags((u32 *)(entry + 0x28), resource, flags);
    effUpdateTimedStates(effect, slot, entry);
    return 1;
}

s32 effConfigureIndexedSlotMaterial(u8 *effect, u32 slot, u8 *resources, u32 index,
                  u32 flags, u32 option, u32 color) {
    u8 *entry = (u8 *)((EffSlotSet *)effect)->entries + slot * 0xA0;
    u32 resource = *(u32 *)(resources + 8) + index * 0x24;
    effSetSlotResourceAndFlags((u32 *)(entry + 0x28), resource, color);
    effUpdateTimedStates(effect, slot, entry);
    *(u32 *)(entry + 0x30) = flags;
    *(u32 *)(entry + 0x34) = option;
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
        effSlotTransitionClearTarget((first + i) * 0xA0 + ((EffSlotSet *)table)->entries + 0x28, arg);
        i++;
        index = first + i;
    } while (index < ((EffSlotSet *)table)->count && (*(u32 *)(index * 0x80 + ((EffSlotSet *)table)->sourceRecords + 0x18) & 0x20));
    return 1;
}

extern void func_00308478(u32, u32);

void effSelectPresetByKind(u32 kind, u32 arg) {
    switch (kind) {
    case 0:
        func_00308478(0x44, arg);
        return;
    case 1:
        func_00308478(0x48, arg);
        return;
    case 2:
        func_00308478(0x42, arg);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305C40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305EB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306030);

extern void func_00306030(u32, u32, u32, u32, u32, u32, u32, u32,
                          u32, u32, u32, u32, u32);

void func_003064C0(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h,
                   u32 x, u32 y, u32 width, u32 height) {
    func_00306030(a, b, c, d, e, f, g, h, x, y, 1, width, height);
}

void effSelectPresetAndDispatch(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    effSelectPresetByKind(arg6, arg7);
    func_003089B8(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_00308478(0x44, arg7);
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D170);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D190);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1D0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E2C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E34);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E3C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E40);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E48);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E50);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E54);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E58);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E5C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E60);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E64);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E68);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E6C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E70);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E74);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E78);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E7C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E80);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E84);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E88);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E90);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E94);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E98);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EA0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EA8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EC0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EC8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437ED0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437ED8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EE0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EE8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EF0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EF8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F00);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F08);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F10);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F18);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F20);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F28);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F40);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F48);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F50);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F58);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F60);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F68);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F70);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F78);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F80);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F88);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F90);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F98);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FA0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FA8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FB0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FB8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FC0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FC8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FD0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FD8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FE0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FE8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FF0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FF8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438000);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438008);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438010);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438018);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438020);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438028);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438030);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438038);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438040);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438048);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438050);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438058);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438060);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438068);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438070);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438078);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438080);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438088);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438090);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438098);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438100);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438108);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438110);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438118);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438120);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438128);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438130);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438138);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438140);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438148);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438150);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438158);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438160);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438168);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438170);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438178);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438180);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438188);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438190);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438198);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438200);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438208);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438210);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438218);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438220);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438228);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438230);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438238);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438240);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438248);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438250);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438258);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438260);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438268);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438270);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438278);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438280);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438288);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438290);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438298);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438300);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438308);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438310);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438318);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438320);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438328);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438330);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438338);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438340);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438348);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438350);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438358);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438360);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438368);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438370);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438378);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438380);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438388);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438390);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438398);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438400);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438408);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438410);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438418);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438420);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438428);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438430);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438438);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438440);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438448);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438450);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438458);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438460);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438468);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438470);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438478);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438480);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438488);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438490);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438498);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438500);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438508);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438510);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438518);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438520);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438528);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438530);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438538);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438540);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438548);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438550);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438558);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438560);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438568);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438570);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438578);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438580);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438588);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438590);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438598);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438600);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438608);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438610);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438618);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438620);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438628);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438630);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438638);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438640);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438644);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438648);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438650);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438658);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438660);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438668);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438670);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438678);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438680);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438688);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438690);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438698);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386BC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386CC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438700);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438708);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438710);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438718);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438720);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438728);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438730);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438738);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438740);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438748);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438750);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438758);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438759);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875A);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875B);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438760);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438764);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438768);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043876C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438770);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438774);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438778);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043877C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438780);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438788);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438790);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438798);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387AC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387BC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438800);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438808);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438810);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438818);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438820);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438828);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438830);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438838);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438840);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438848);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438850);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438858);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438860);

