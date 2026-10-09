#ifndef SDF_DRAW_H
#define SDF_DRAW_H

#include "common.h"

struct SdfChunkHeader;

/* Three optional SDK light sources; each points to color and direction vec4.
 * The callee advances through three pointer words, not scalar slot metadata. */
typedef f32 (*SdfLightSources[3])[4];

typedef char SdfLightSources_size_must_be_0xC[(sizeof(SdfLightSources) == 0xC) ? 1 : -1];

/* Opaque storage for the complete 0xE0 lighting packet built by the renderer. */
typedef struct SdfLightingPacketStorage {
    u8 bytes[0xE0];
} SdfLightingPacketStorage;

typedef char SdfLightingPacketStorage_size_must_be_0xE0[
    (sizeof(SdfLightingPacketStorage) == 0xE0) ? 1 : -1];
typedef char SdfLightingPacketStorage_alignment_must_be_1[
    (__alignof__(SdfLightingPacketStorage) == 1) ? 1 : -1];

/* Buffered SDK storage: live entries are distinct from allocated capacity. */
struct SdfMemBlock;

typedef struct DevRequest {
    struct SdfMemBlock *backingAllocation;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;

typedef char DevRequest_size_must_be_0x10[(sizeof(DevRequest) == 0x10) ? 1 : -1];

DevRequest *sdfDevCreateBufferedRequest(s32 count, s32 stride, s32 growStep);

/* Command-list node: allocation owns the node and trailing command data.
 * A null descriptor identifies a separately chip-allocated node. */
typedef struct SdfCommandNode {
    struct SdfCommandNode *next;
    u8 kind;
    s8 packetSelector;
    s16 quadwordCount;
    void *payload; /* Packet bytes for kind 1; serialized commands for kinds 2/3. */
    struct SdfMemBlock *allocation;
} SdfCommandNode;

typedef char SdfCommandNode_size_must_be_0x10[(sizeof(SdfCommandNode) == 0x10) ? 1 : -1];
typedef char SdfCommandNode_payload_offset_must_be_8[
    ((u32)&((SdfCommandNode *)0)->payload == 0x08) ? 1 : -1];
typedef char SdfCommandNode_allocation_offset_must_be_C[
    ((u32)&((SdfCommandNode *)0)->allocation == 0x0C) ? 1 : -1];

/* Serialized kinds 5/7 name draw nodes by index in the model's buffered list. */
typedef struct SdfObjectRefCommand {
    u8 kind;
    u8 pad01[0x15];
    u16 flags;
    u16 count;
    u16 objectIndices[1];
} SdfObjectRefCommand;

typedef char SdfObjectRefCommand_flags_at_16[
    ((u32)&((SdfObjectRefCommand *)0)->flags == 0x16) ? 1 : -1];
typedef char SdfObjectRefCommand_count_at_18[
    ((u32)&((SdfObjectRefCommand *)0)->count == 0x18) ? 1 : -1];
typedef char SdfObjectRefCommand_indices_at_1A[
    ((u32)&((SdfObjectRefCommand *)0)->objectIndices == 0x1A) ? 1 : -1];

struct SdfModel;
struct Motion;

/* Allocated as 0x100-byte nodes; each owns a circular list of child draw nodes. */
typedef struct SdfDrawNode {
    struct SdfDrawNode *previous; /* 0x00 */
    struct SdfDrawNode *next;     /* 0x04 */
    struct SdfDrawNode *parent;   /* 0x08 */
    struct SdfDrawNode *children; /* 0x0C */
    struct SdfModel *root;        /* 0x10: backlink installed by sdfAppendBufferedRouteNode */
    u16 flags;                   /* 0x14 */
    s16 unk16;                   /* 0x16: constructor sentinel is -1 */
    s32 nodeId;                   /* 0x18: lookup key when model flags bit 0 is set */
    u32 color;                   /* 0x1C */
    u8 pad20[8];
    SdfCommandNode *lists[2];     /* 0x28: command lists for both buffered frames */
    u8 *workspace; /* Two 0x80-byte frame packets, allocated on first use. */                  /* 0x30 */
    const f32 *boundsCorners;     /* 0x34: optional two xyz corners used by clipping */
    void *sourceItem;             /* 0x38: item this node was built from */
    u8 pad3C[0x14];
    f32 quaternion[4];           /* 0x50 */
    f32 translation[4];          /* 0x60 */
    f32 scale[4];                /* 0x70 */
    f32 localMatrix[4][4];       /* 0x80: all four local basis/translation rows */
    f32 worldMatrix[4][4];       /* 0xC0: local transform composed with its parent */
} SdfDrawNode;

#define SDF_DRAW_NODE_FLAG_SKIP_RENDER 0x0001
#define SDF_DRAW_NODE_FLAG_USE_NODE_COLOR 0x0002

/* Two index/weight pairs stored in each 0x10-byte model slot entry. */
typedef struct SdfSlotPair {
    s32 index;
    f32 weight;
} SdfSlotPair;

typedef struct SdfSlotEntry {
    SdfSlotPair pair[2];
} SdfSlotEntry;

typedef char SdfDrawNode_boundsCorners_at_34[
    ((u32)&((SdfDrawNode *)0)->boundsCorners == 0x34) ? 1 : -1];
typedef char SdfSlotPair_size_must_be_8[(sizeof(SdfSlotPair) == 8) ? 1 : -1];
typedef char SdfSlotEntry_size_must_be_0x10[(sizeof(SdfSlotEntry) == 0x10) ? 1 : -1];


typedef char SdfDrawNode_workspace_at_30[
    ((u32)&((SdfDrawNode *)0)->workspace == 0x30) ? 1 : -1];

/* Model flags select indexed lookup, scalar overrides and alternate item setup. */
#define SDF_MODEL_FIND_DRAW_NODE_BY_ID 0x01
#define SDF_MODEL_USE_SCALAR_OVERRIDES 0x02
#define SDF_MODEL_ALTERNATE_ITEM_SETUP 0x04
#define SDF_MODEL_HAS_ANCHOR_RECORDS 0x10


/* The buffered-transform constructor allocates and clears all 0x9C bytes. */
typedef struct SdfModel {
    DevRequest *list;
    SdfDrawNode *rootNode;
    void *assetData;
    DevRequest *resources;
    DevRequest *slotPairs;
    struct Motion *motionList;
    u8 unk18;
    u8 flags;
    s16 unk1A;
    u32 color;
    f32 matrix[4][4];
    f32 rotationQuaternion[4];
    f32 scaleVector[4];
    SdfLightingPacketStorage *lighting;
    u32 unk84;
    f32 scalarOverrideFirst;
    f32 scalarOverrideSecond;
    struct SdfChunkHeader *chunkTable; /* Borrowed from the model resource data. */
    /* Float scalar (DDS2 0x232074 / Nocturne 0x2B07D4), forwarded
     * as raw packet bits by DDS1 0x2D9530. */
    union {
        u32 word;
        f32 scalar;
    } unk94;
    u8 lodIndex; /* Native model-viewer and MODEL_LOD_CHG setters use byte 0x98. */
    u8 pad99[3];
} SdfModel;

typedef char SdfModel_size_must_be_0x9C[(sizeof(SdfModel) == 0x9C) ? 1 : -1];
typedef char SdfModel_chunkTable_at_90[
    ((u32)&((SdfModel *)0)->chunkTable == 0x90) ? 1 : -1];
typedef char SdfModel_scalarOverrideFirst_at_88[
    ((u32)&((SdfModel *)0)->scalarOverrideFirst == 0x88) ? 1 : -1];
typedef char SdfModel_scalarOverrideSecond_at_8C[
    ((u32)&((SdfModel *)0)->scalarOverrideSecond == 0x8C) ? 1 : -1];

SdfDrawNode *sdfModelFindDrawNode(SdfModel *model, s32 id);
void sdfProcessReferencedObjects(SdfModel *model, SdfObjectRefCommand *source);


/* Serialized clip duration/reserved remain halfwords (tools/fld.py).
 * The viewer MAXFRAME diagnostic reads their packed little-endian word. */
typedef struct MotionEntry {
    union {
        struct {
            u16 frameCount;
            u16 unk02;
        };
        u32 packedHeader;
    };
    u32 bindingData[1];
} MotionEntry;
typedef char MotionEntry_header_at_0[((u32)&((MotionEntry *)0)->packedHeader == 0) ? 1 : -1];
typedef char MotionEntry_frameCount_at_0[((u32)&((MotionEntry *)0)->frameCount == 0) ? 1 : -1];
typedef char MotionEntry_reserved_at_2[((u32)&((MotionEntry *)0)->unk02 == 2) ? 1 : -1];
typedef char MotionEntry_binding_at_4[((u32)&((MotionEntry *)0)->bindingData == 4) ? 1 : -1];
typedef char MotionEntry_header_extent[(sizeof(((MotionEntry *)0)->packedHeader) == 4) ? 1 : -1];
typedef char MotionEntry_size_must_be_8[(sizeof(MotionEntry) == 8) ? 1 : -1];

typedef struct SdfMotionCommand {
    u32 command;
    u32 argument;
} SdfMotionCommand;

/* Resource header followed by the binding commands used to construct a motion. */
typedef struct MotionTable {
    u16 unk00;
    u16 commandCount;
    MotionEntry **entries;
    SdfMotionCommand commands[1];
} MotionTable;

/* SDK motions are allocated as 0x34-byte records; model slots retain these nodes. */
typedef struct Motion {
    struct Motion *next;
    SdfModel *owner;
    MotionTable *motionTable;
    s32 unkC;
    DevRequest *request;
    f32 blendDurationFrames;
    f32 blendStartFrame;
    f32 currentFrame;
    f32 frameStep;
    s32 unk24;
    s16 searchId;
    s16 slotIndex;
    u16 motionIndex;
    u16 frameCount;
    u8 state;
    u8 previousState;
    u8 loopEnabled;
    u8 pad33;
} Motion;

#endif /* SDF_DRAW_H */
