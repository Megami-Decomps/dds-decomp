#ifndef SDF_DRAW_H
#define SDF_DRAW_H

#include "common.h"

/* Buffered SDK storage: live entries are distinct from allocated capacity. */
typedef struct DevRequest {
    s32 handle;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;

typedef char DevRequest_size_must_be_0x10[(sizeof(DevRequest) == 0x10) ? 1 : -1];

DevRequest *sdfDevCreateBufferedRequest(s32 count, s32 stride, s32 growStep);

/* Allocated command-list node; resourceHandle owns its optional backing block. */
typedef struct SdfCommandNode {
    struct SdfCommandNode *next;
    u8 kind;
    s8 packetSelector;
    s16 quadwordCount;
    u32 unk08;
    s32 resourceHandle;
} SdfCommandNode;

typedef char SdfCommandNode_size_must_be_0x10[(sizeof(SdfCommandNode) == 0x10) ? 1 : -1];

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
    u32 address;                  /* 0x30 */
    s32 boundsAddress;            /* 0x34: optional address of two local xyz box corners */
    void *sourceItem;             /* 0x38: item this node was built from */
    u8 pad3C[0x14];
    f32 quaternion[4];           /* 0x50 */
    f32 translation[4];          /* 0x60 */
    f32 scale[4];                /* 0x70 */
    f32 localMatrix[4][4];       /* 0x80: all four local basis/translation rows */
    f32 worldMatrix[4][4];       /* 0xC0: local transform composed with its parent */
} SdfDrawNode;

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
    f32 unk60[4];
    f32 scaleVector[4];
    void *lighting;
    u32 unk84;
    f32 unk88;
    f32 unk8C;
    u32 unk90;
    u32 unk94;
    u8 lodIndex; /* Native model-viewer and MODEL_LOD_CHG setters use byte 0x98. */
    u8 pad99[3];
} SdfModel;

typedef char SdfModel_size_must_be_0x9C[(sizeof(SdfModel) == 0x9C) ? 1 : -1];


typedef struct MotionEntry {
    u16 frameCount;
    u16 unk02;
    u32 bindingData[1];
} MotionEntry;

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
