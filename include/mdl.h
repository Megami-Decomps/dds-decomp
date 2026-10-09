#ifndef MDL_H
#define MDL_H

#include "common.h"
#include "sdf_draw.h"

typedef struct MdlCtx MdlCtx;

/* Model-context update gates; these do not share the inner SdfModel flag byte. */
#define MDL_SKIP_TRANSFORMS 1
#define MDL_SKIP_ANCHORS 2
#define MDL_REQUIRE_ANCHOR_ENABLE 4

struct MdlPartEntry;
struct MdlObj;
struct SdfMapPositionRecord;
struct SdfTex;
struct EffTrackPolyWork;

/* Known resource-item kinds; the owner stores this domain in a halfword. */
typedef enum MdlResourceKind {
    MDL_RESOURCE_BILLBOARD = 0,
    MDL_RESOURCE_EFFECT = 1,
    MDL_RESOURCE_TRACK_POLY = 2,
    MDL_RESOURCE_OBJECT = 3
} MdlResourceKind;

typedef struct MdlObjectAttachment {
    MdlCtx *owner;
    struct MdlObj *object; /* Borrowed from the owner's heterogeneous part list. */
    struct SdfTex *texture; /* Borrowed texture-list node used for stream creation. */
    s32 minimumTime;
    u8 attributes[8];
} MdlObjectAttachment;

/* The paired viewer allocators clear 0x20 bytes and prepend this tagged item
 * to MdlCtx's list at +0x14. The part and object payloads share that allocation. */
typedef struct MdlResourceItem {
    struct MdlResourceItem *next;
    u16 type; /* MdlResourceKind value, with native halfword storage. */
    s16 subtype;
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
} MdlResourceItem;

typedef struct BattleGroupSlot {
    s32 flags;
    s16 slot;
    s16 motionIndex;
    void *data;
    struct SdfMemBlock *resourceHandle;
} BattleGroupSlot;

/* Group owners allocate 0xB4 bytes and retain eight resource records. */
typedef struct BattleGroupNode {
    struct BattleGroupNode *next;
    struct BattleGroupNode *prev;
    u16 group;
    u16 type;
    u8 ownsResources;
    u8 pad0D[3];
    MdlCtx *modelContext;
    DevRequest *resourceList;
    void *itemList;
    struct SdfMemBlock *requestAllocation;
    BattleGroupSlot slots[8];
    s32 resourceHandle;
    void *partInfo;
    DevRequest *partList;
    f32 unk_AC;
    f32 unk_B0;
} BattleGroupNode;

typedef struct MdlLoadPayload {
    DevRequest *resourceList;
    void *itemList;
    struct SdfMemBlock *requestAllocation;
    void *motionData;
    struct SdfMemBlock *motionResource;
    void *partInfo;
    s32 resourceHandle;
    DevRequest *partList;
} MdlLoadPayload;

/* mdlRequestAsset allocates this 0x2C-byte callback work record. */
typedef struct MdlLoadRequest {
    u16 group;
    u16 id;
    u8 resourceListRequested;
    u8 itemsRequested;
    u8 deferred;
    u8 pad7;
    u32 options;
    MdlLoadPayload payload;
} MdlLoadRequest;

typedef struct MdlDevSlot {
    struct MdlDevSlot *next;
    SdfModel *model;
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
struct MdlCtx {
    u32 flags;
    MdlCtx *next;
    MdlCtx *previous;
    BattleGroupNode *sub;
    union {
        u32 word;
        struct {
            s16 id;
            s16 arg;
        } h;
    } current;
    MdlResourceItem *resourceItems;
    SdfModel *inner;
    Motion *first;
    Motion *slots[4];
    MdlDevList *devList;
    f32 unk34;
};

typedef char BattleGroupSlot_size_must_be_0x10[(sizeof(BattleGroupSlot) == 0x10) ? 1 : -1];
typedef char BattleGroupNode_size_must_be_0xB4[(sizeof(BattleGroupNode) == 0xB4) ? 1 : -1];
typedef char MdlDevSlot_size_must_be_0xC[(sizeof(MdlDevSlot) == 0xC) ? 1 : -1];
typedef char MdlDevSlot_model_offset_must_be_4[
    ((u32)&((MdlDevSlot *)0)->model == 4) ? 1 : -1];
typedef char MdlDevSlot_remaining_offset_must_be_8[
    ((u32)&((MdlDevSlot *)0)->remaining == 8) ? 1 : -1];
typedef char MdlDevList_size_must_be_0x30[(sizeof(MdlDevList) == 0x30) ? 1 : -1];
typedef char MdlCtx_size_must_be_0x38[(sizeof(MdlCtx) == 0x38) ? 1 : -1];
typedef char MdlLoadPayload_size_must_be_0x20[(sizeof(MdlLoadPayload) == 0x20) ? 1 : -1];
typedef char MdlLoadRequest_size_must_be_0x2C[(sizeof(MdlLoadRequest) == 0x2C) ? 1 : -1];
typedef char MdlResourceItem_size_must_be_0x20[(sizeof(MdlResourceItem) == 0x20) ? 1 : -1];
typedef char MdlObjectAttachment_size_must_be_0x18[(sizeof(MdlObjectAttachment) == 0x18) ? 1 : -1];

/* Model-context lifetime, per-frame update, and broadcast accessors. */
void mdlDestroyContext(MdlCtx *ctx);
void mdlProcessContextNodesAndTransforms(MdlCtx *ctx, struct SdfPoolNode **surfaces);
MdlCtx *mdlCreateContextFromResourceKey(u32 group, u32 id);
u16 mdlGetContextResourceGroup(MdlCtx *ctx);
u16 mdlGetContextResourceId(MdlCtx *ctx);
u32 mdlGetBroadcastValue(MdlCtx *ctx);
void mdlSetAllResourceFrames(MdlCtx *ctx, u32 value);
void mdlBroadcastMasked(MdlCtx *ctx, u32 value);

/* Motion-record selection and lookup. */
void mdlAddEntryPlain(MdlCtx *ctx, s32 searchId, s32 motionIndex);
void mdlAddEntryFlagged(MdlCtx *ctx, s32 searchId, s32 motionIndex);
void mdlAddEntryPlainEx(MdlCtx *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);
void mdlAddEntryFlaggedEx(MdlCtx *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);
u8 mdlHasNode(MdlCtx *ctx, s32 searchId);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNodeRefHalf(MdlCtx *ctx, s32 searchId);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlLoadPrimaryVectorVU(MdlCtx *ctx);
void mdlStorePrimaryVectorVU(MdlCtx *ctx);
void mdlLoadRotationQuaternionVU(MdlCtx *ctx);
void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *ctx);
void mdlLoadTertiaryVectorVU(MdlCtx *ctx);
void mdlStoreTertiaryVectorVU(MdlCtx *ctx);

#endif /* MDL_H */
