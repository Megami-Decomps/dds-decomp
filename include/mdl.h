#ifndef MDL_H
#define MDL_H

#include "common.h"
#include "sdf_draw.h"

typedef struct dds3ModelHandle_t dds3ModelHandle_t;

/* Model-context update gates; these do not share the inner sdfModelRootNode_t flag byte. */
#define MDL_SKIP_TRANSFORMS 1
#define MDL_SKIP_ANCHORS 2
#define MDL_REQUIRE_ANCHOR_ENABLE 4

struct MdlPartEntry;
struct MdlObj;
struct SdfPoolNode;
/* Relative-linked viewer records share packed payload and parameter words.
 * Kinds 1/2 use the count and part-index halfwords at 0x0C/0x0E; kind 3
 * reads the whole parameter word. Kind 4 reads both selector halfwords at 0x08/0x0A. */
typedef struct MdlRecord {
    s32 kind;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    union {
        u32 word;               /* 0x08 */
        s32 id;                 /* Kind 1/2/5 resource-record ID. */
        struct {
            u16 selectorA;
            u16 selectorB;
        } stream;
    } payload;
    union {
        u32 word;               /* 0x0C */
        s32 minimumTime;        /* Kind 5 deferred initialization threshold. */
        struct {
            u16 count;
            u16 partIndex;
        } part;
    } parameter;
    union {
        u32 word10; /* 0x10: kind-four stream parameter */
        f32 anchorScale; /* Optional kind 1/2 extension when nextOffset >= 0x11. */
        struct {
            u8 playbackCadenceStep;
            u8 loopMode;
            u16 partIndex;
        } object;
        struct {
            u16 slotCount; /* Kind 1/2 group records: following entity-slot packets. */
            u16 unk12;
        } half;
    } tail;
} MdlRecord;

typedef char MdlRecord_size_must_be_0x14[(sizeof(MdlRecord) == 0x14) ? 1 : -1];
typedef char MdlRecord_slotCount_offset_must_be_0x10[
    ((u32)&((MdlRecord *)0)->tail.half.slotCount == 0x10) ? 1 : -1];
struct SdfMapPositionRecord;
struct sdfModelDef_t;
struct sdfTexHandle_t;
struct EffTrackPolyWork;

/* Known resource-item kinds; the owner stores this domain in a halfword. */
typedef enum MdlResourceKind {
    MDL_RESOURCE_BILLBOARD = 0,
    MDL_RESOURCE_EFFECT = 1,
    MDL_RESOURCE_TRACK_POLY = 2,
    MDL_RESOURCE_OBJECT = 3
} MdlResourceKind;

typedef struct MdlObjectAttachment {
    dds3ModelHandle_t *owner;
    struct MdlObj *object; /* Borrowed from the owner's heterogeneous part list. */
    struct sdfTexHandle_t *texture; /* Borrowed texture-list node used for stream creation. */
    s32 minimumTime;
    u8 attributes[8];
} MdlObjectAttachment;

/* The paired viewer allocators clear 0x20 bytes and prepend this tagged item
 * to dds3ModelHandle_t's list at +0x14. The part and object payloads share that allocation. */
typedef struct dds3ModelEffectCtrlr_t {
    struct dds3ModelEffectCtrlr_t *next;
    u16 type; /* MdlResourceKind value, with native halfword storage. */
    s16 motionSlotIndex; /* Owning sdfMotionCtrlr_t.slotIndex; -1 tags context-wide entries. */
    union {
        struct {
            /* Type 2 stores a track; other part kinds store their created instance. */
            union {
                void *instance;
                struct EffTrackPolyWork *track;
            };
            struct MdlPartEntry *slot;
            struct SdfMapPositionRecord *mapPositionRecord;
            f32 anchorScale;
            u8 pad18[8];
        } part;
        MdlObjectAttachment object;
    } payload;
} dds3ModelEffectCtrlr_t;

#define MDL_MOTION_SLOT_CREATE_ON_MODEL_LOAD 1

typedef struct BattleGroupSlot {
    s32 flags; /* 0x00: bit 0 creates a motion owner for new model contexts. */
    s16 motionSlotIndex;
    s16 motionIndex;
    MotionTable *motionTable;
    struct sdfMemHandle_t *resourceHandle;
} BattleGroupSlot;

/* Group owners allocate 0xB4 bytes and retain eight resource records. */
typedef struct dds3ModelResrc_t {
    struct dds3ModelResrc_t *next;
    struct dds3ModelResrc_t *prev;
    u16 group;
    u16 type;
    u8 ownsResources;
    u8 pad0D[3];
    dds3ModelHandle_t *modelContext;
    DevRequest *resourceList;
    struct sdfModelDef_t *itemList;
    struct sdfMemHandle_t *requestAllocation;
    BattleGroupSlot slots[8];
    struct sdfMemHandle_t *resourceHandle;
    struct MdlRecord *partInfo; /* Borrowed relative-linked record-list view. */
    DevRequest *partList;
    f32 unk_AC;
    f32 objectRadius; /* Copied into the model context and object radius. */
} dds3ModelResrc_t;

typedef struct MdlLoadPayload {
    DevRequest *resourceList;
    struct sdfModelDef_t *itemList;
    struct sdfMemHandle_t *requestAllocation;
    MotionTable *motionTable;
    struct sdfMemHandle_t *motionResource;
    void *partInfo; /* Borrowed relative-linked record-list transport. */
    struct sdfMemHandle_t *resourceHandle;
    DevRequest *partList;
} MdlLoadPayload;

/* mdlPrepare allocates this 0x2C-byte callback work record. */
typedef struct MdlLoadRequest {
    u16 group;
    u16 id;
    u8 resourceListRequested;
    u8 itemsRequested;
    u8 awaitingMotionLoad;
    u8 pad7;
    u32 options; /* MdlRegistrationMode consumed when this request completes. */
    MdlLoadPayload payload;
} MdlLoadRequest;

typedef struct MdlDevSlot {
    struct MdlDevSlot *next;
    sdfModelRootNode_t *model;
    s32 remaining;
} MdlDevSlot;

/* This owner has inline color interpolation data, not a request buffer at +0xC. */
typedef struct MdlDevList {
    MdlDevSlot *first;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    s32 byteCount;
    f32 color[4];
    f32 colorStep[4];
} MdlDevList;

/* Context constructors allocate 0x38 bytes; their SDK model slot lives at +0x18. */
struct dds3ModelHandle_t {
    u32 flags;
    dds3ModelHandle_t *next;
    dds3ModelHandle_t *previous;
    dds3ModelResrc_t *sub;
    union {
        u32 word;
        struct {
            s16 id;
            s16 motionIndex; /* Entry index passed to sdfMotionChangeBlend. */
        } h;
    } current;
    dds3ModelEffectCtrlr_t *resourceItems;
    sdfModelRootNode_t *inner;
    sdfMotionCtrlr_t *first;
    sdfMotionCtrlr_t *slots[4];
    MdlDevList *devList;
    f32 objectRadius; /* Propagated to the rebuilt object's inner radius. */
};

typedef char BattleGroupSlot_size_must_be_0x10[(sizeof(BattleGroupSlot) == 0x10) ? 1 : -1];
typedef char BattleGroupNode_size_must_be_0xB4[(sizeof(dds3ModelResrc_t) == 0xB4) ? 1 : -1];
typedef char BattleGroupNode_objectRadius_at_B0[
    ((u32)&((dds3ModelResrc_t *)0)->objectRadius == 0xB0) ? 1 : -1];
typedef char MdlDevSlot_size_must_be_0xC[(sizeof(MdlDevSlot) == 0xC) ? 1 : -1];
typedef char MdlDevSlot_model_offset_must_be_4[
    ((u32)&((MdlDevSlot *)0)->model == 4) ? 1 : -1];
typedef char MdlDevSlot_remaining_offset_must_be_8[
    ((u32)&((MdlDevSlot *)0)->remaining == 8) ? 1 : -1];
typedef char MdlDevList_size_must_be_0x30[(sizeof(MdlDevList) == 0x30) ? 1 : -1];
typedef char MdlCtx_size_must_be_0x38[(sizeof(dds3ModelHandle_t) == 0x38) ? 1 : -1];
typedef char MdlCtx_objectRadius_at_34[
    ((u32)&((dds3ModelHandle_t *)0)->objectRadius == 0x34) ? 1 : -1];
typedef char MdlLoadPayload_size_must_be_0x20[(sizeof(MdlLoadPayload) == 0x20) ? 1 : -1];
typedef char MdlLoadRequest_size_must_be_0x2C[(sizeof(MdlLoadRequest) == 0x2C) ? 1 : -1];
typedef char MdlResourceItem_size_must_be_0x20[(sizeof(dds3ModelEffectCtrlr_t) == 0x20) ? 1 : -1];
typedef char MdlObjectAttachment_size_must_be_0x18[(sizeof(MdlObjectAttachment) == 0x18) ? 1 : -1];

/* Model-context lifetime, per-frame update, and broadcast accessors. */
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(dds3ModelHandle_t *ctx);
void mdlLink(dds3ModelHandle_t *ctx, struct SdfPoolNode **surfaces);
/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id);
u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);
u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);
u32 mdlGetColor(const dds3ModelHandle_t *ctx);
void mdlSetEffectColor(dds3ModelHandle_t *ctx, u32 value);
void mdlSetEffectColorSub(dds3ModelHandle_t *ctx, dds3ModelEffectCtrlr_t *item, s32 frame);
void mdlSetEffectScaleSub(dds3ModelHandle_t *ctx, dds3ModelEffectCtrlr_t *item, f32 amount);
void mdlSetColor(dds3ModelHandle_t *ctx, u32 value);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);
void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);
void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);
void mdlSmoothLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);
/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

/* Count of following entity-slot packets in a grouped record. */
u16 mdlPibGetMotionCount(const MdlRecord *record);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);
void mdlSetPosition(dds3ModelHandle_t *ctx);
void mdlGetRotate(dds3ModelHandle_t *ctx);
void mdlSetRotate(dds3ModelHandle_t *ctx);
void mdlGetScale(dds3ModelHandle_t *ctx);
void mdlSetScale(dds3ModelHandle_t *ctx);

#endif /* MDL_H */
