#ifndef SDF_DRAW_H
#define SDF_DRAW_H

#include "common.h"
#include "sdf_dma_tag.h"
#include "sdf_motion.h"
#include "sdf_lighting.h"

struct SdfChunkHeader;

/* Four quadwords: colour, direction, position and the falloff parameters. */
typedef struct sdf3DLight_t
{
    sdfVector4_t color;
    sdfVector4_t v;
    sdfVector4_t p;
    f32 param[4];
} sdf3DLight_t;

void sdf3DMakeLightParallelPacket(struct sdf3DLightParallelPacket_t *packet,
                                  sdf3DLight_t **lights, sdfVector4_t *ambient);

/* Buffered SDK storage: live entries are distinct from allocated capacity. */
struct sdfMemHandle_t;

typedef struct DevRequest {
    struct sdfMemHandle_t *backingAllocation;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;

typedef char DevRequest_size_must_be_0x10[(sizeof(DevRequest) == 0x10) ? 1 : -1];

DevRequest *sdfItemArrayNew(s32 count, s32 stride, s32 growStep);

/* Command-list node: allocation owns the node and trailing command data.
 * A null descriptor identifies a separately chip-allocated node. */
typedef struct SdfCommandNode {
    struct SdfCommandNode *next;
    u8 kind;
    s8 packetSelector;
    s16 quadwordCount;
    void *payload; /* Packet bytes for kind 1; serialized commands for kinds 2/3. */
    struct sdfMemHandle_t *allocation;
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

struct sdfModelRootNode_t;
struct sdfMotionCtrlr_t;

/* Allocated as 0x100-byte nodes; each owns a circular list of child draw nodes. */
typedef struct sdfModelNode_t {
    struct sdfModelNode_t *previous; /* 0x00 */
    struct sdfModelNode_t *next;     /* 0x04 */
    struct sdfModelNode_t *parent;   /* 0x08 */
    struct sdfModelNode_t *children; /* 0x0C */
    struct sdfModelRootNode_t *root;        /* 0x10: backlink installed by sdfModelNodeAdd */
    u16 flags;                   /* 0x14 */
    u16 drawFlagMask;             /* 0x16: all model draw flags enabled by default */
    s32 nodeId;                   /* 0x18: lookup key when model flags bit 0 is set */
    u32 color;                   /* 0x1C */
    u8 pad20[8];
    SdfCommandNode *lists[2];     /* 0x28: command lists for both buffered frames */
    u8 *workspace; /* Two 0x80-byte frame packets, allocated on first use. */                  /* 0x30 */
    const f32 *boundsCorners;     /* 0x34: optional two xyz corners used by clipping */
    const void *sourceData;       /* 0x38: read-only source item or command list */
    u8 pad3C[0x14];
    f32 quaternion[4];           /* 0x50 */
    f32 translation[4];          /* 0x60 */
    f32 scale[4];                /* 0x70 */
    f32 localMatrix[4][4];       /* 0x80: all four local basis/translation rows */
    f32 worldMatrix[4][4];       /* 0xC0: local transform composed with its parent */
} sdfModelNode_t;

#define SDF_DRAW_NODE_FLAG_SKIP_RENDER 0x0001
#define SDF_DRAW_NODE_FLAG_MOTION_HIDDEN 0x0010
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
    ((u32)&((sdfModelNode_t *)0)->boundsCorners == 0x34) ? 1 : -1];
typedef char SdfSlotPair_size_must_be_8[(sizeof(SdfSlotPair) == 8) ? 1 : -1];
typedef char SdfSlotEntry_size_must_be_0x10[(sizeof(SdfSlotEntry) == 0x10) ? 1 : -1];


typedef char SdfDrawNode_workspace_at_30[
    ((u32)&((sdfModelNode_t *)0)->workspace == 0x30) ? 1 : -1];

/* Model flags select indexed lookup, scalar overrides and alternate item setup. */
#define SDF_MODEL_FIND_DRAW_NODE_BY_ID 0x01
#define SDF_MODEL_USE_SCALAR_OVERRIDES 0x02
#define SDF_MODEL_ALTERNATE_ITEM_SETUP 0x04
#define SDF_MODEL_HAS_ANCHOR_RECORDS 0x10
#define SDF_MODEL_FORCE_ASSET_TEXTURE_REFRESH_WITH_ZERO_STATE_WORD 0x20
#define SDF_MODEL_FORCE_ASSET_TEXTURE_REFRESH 0x40


/* The buffered-transform constructor allocates and clears all 0x9C bytes. */
typedef struct sdfModelRootNode_t {
    DevRequest *list;
    sdfModelNode_t *rootNode;
    DevRequest *assetData;
    DevRequest *resources;
    DevRequest *slotPairs;
    struct sdfMotionCtrlr_t *motionList;
    u8 unk18;
    u8 flags;
    u16 drawFlags;
    u32 color;
    f32 matrix[4][4];
    f32 rotationQuaternion[4];
    f32 scaleVector[4];
    struct sdf3DLightParallelPacket_t *lighting;
    struct SdfDmaTagHeader *secondaryLighting; /* Borrowed second DMA reference prefix. */
    f32 scalarOverrideFirst;
    f32 scalarOverrideSecond;
    struct SdfChunkHeader *chunkTable; /* Borrowed from the model resource data. */
    /* Model scale read by sdfCoreDrawSetZBias and copied unchanged into the
     * three-scalar VIF payload (DDS1 0x2D9530). */
    f32 modelScale;
    u8 lodIndex; /* Native model-viewer and MODEL_LOD_CHG setters use byte 0x98. */
    u8 pad99[3];
} sdfModelRootNode_t;

typedef char SdfModel_size_must_be_0x9C[(sizeof(sdfModelRootNode_t) == 0x9C) ? 1 : -1];
typedef char SdfModel_drawFlags_at_1A[
    ((u32)&((sdfModelRootNode_t *)0)->drawFlags == 0x1A) ? 1 : -1];
typedef char SdfModel_chunkTable_at_90[
    ((u32)&((sdfModelRootNode_t *)0)->chunkTable == 0x90) ? 1 : -1];
typedef char SdfModel_secondaryLighting_at_84[
    ((u32)&((sdfModelRootNode_t *)0)->secondaryLighting == 0x84) ? 1 : -1];
typedef char SdfModel_modelScale_at_94[
    ((u32)&((sdfModelRootNode_t *)0)->modelScale == 0x94) ? 1 : -1];
typedef char SdfModel_scalarOverrideFirst_at_88[
    ((u32)&((sdfModelRootNode_t *)0)->scalarOverrideFirst == 0x88) ? 1 : -1];
typedef char SdfModel_scalarOverrideSecond_at_8C[
    ((u32)&((sdfModelRootNode_t *)0)->scalarOverrideSecond == 0x8C) ? 1 : -1];

sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *model, s32 nodeIdOrIndex);
void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *model, SdfObjectRefCommand *source);


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
    union {
        u32 packedCommand;
        struct {
            /* Runtime dispatch shifts the command word right by 16. */
            u16 selector;
            u16 family;
        } binding;
    };
    s32 targetIndex;
} SdfMotionCommand;

typedef char SdfMotionCommand_size_must_be_8[
    (sizeof(SdfMotionCommand) == 8) ? 1 : -1];

/* Resource header followed by the binding commands used to construct a motion. */
typedef struct MotionTable {
    u16 entryCount; /* sdfMotionCtrlr_t-index upper bound returned by mdlGetNumOfCut. */
    u16 commandCount;
    MotionEntry **entries;
    SdfMotionCommand commands[1];
} MotionTable;

/* SDK motions are allocated as 0x34-byte records; model slots retain these nodes. */
typedef struct sdfMotionCtrlr_t {
    struct sdfMotionCtrlr_t *next;
    sdfModelRootNode_t *owner;
    MotionTable *motionTable;
    s32 unkC;
    DevRequest *request;
    f32 blendDurationFrames;
    f32 blendStartFrame;
    f32 currentFrame;
    f32 frameStep;
    s32 unk24;
    s16 searchId; /* MdlMotionSearchId stored in the controller's halfword. */
    s16 slotIndex;
    u16 motionIndex;
    u16 frameCount;
    SdfMotionState state;
    SdfMotionState previousState;
    u8 loopEnabled;
    u8 pad33;
} sdfMotionCtrlr_t;

#endif /* SDF_DRAW_H */
