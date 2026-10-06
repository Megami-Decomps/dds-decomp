#ifndef MDL_H
#define MDL_H

#include "common.h"
#include "sdf_draw.h"

typedef struct MdlCtx MdlCtx;

struct MdlPartEntry;

typedef struct MdlObjectAttachment {
    MdlCtx *owner;
    s32 objectAddress;
    s32 data;
    s32 minimumTime;
    u8 attributes[8];
} MdlObjectAttachment;

/* The paired viewer allocators clear 0x20 bytes and prepend this tagged item
 * to MdlCtx's list at +0x14. The part and object payloads share that allocation. */
typedef struct MdlResourceItem {
    struct MdlResourceItem *next;
    u16 type;
    s16 subtype;
    union {
        struct {
            s32 handle;
            struct MdlPartEntry *slot;
            void *record;
            f32 value;
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
    s32 resourceHandle;
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
    s32 requestHandle;
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
    s32 requestHandle;
    void *motionData;
    s32 motionResource;
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
    void *slot;
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
typedef char MdlDevList_size_must_be_0x30[(sizeof(MdlDevList) == 0x30) ? 1 : -1];
typedef char MdlCtx_size_must_be_0x38[(sizeof(MdlCtx) == 0x38) ? 1 : -1];
typedef char MdlLoadPayload_size_must_be_0x20[(sizeof(MdlLoadPayload) == 0x20) ? 1 : -1];
typedef char MdlLoadRequest_size_must_be_0x2C[(sizeof(MdlLoadRequest) == 0x2C) ? 1 : -1];
typedef char MdlResourceItem_size_must_be_0x20[(sizeof(MdlResourceItem) == 0x20) ? 1 : -1];
typedef char MdlObjectAttachment_size_must_be_0x18[(sizeof(MdlObjectAttachment) == 0x18) ? 1 : -1];

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNodeRefHalf(MdlCtx *ctx, s32 searchId);

#endif /* MDL_H */
