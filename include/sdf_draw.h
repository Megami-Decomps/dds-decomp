#ifndef SDF_DRAW_H
#define SDF_DRAW_H

#include "common.h"

struct SdfModel;

/* Each model draw node owns a circular list of child draw nodes. */
typedef struct SdfDrawNode {
    u8 pad00[4];
    struct SdfDrawNode *next;     /* 0x04 */
    u8 pad08[4];
    struct SdfDrawNode *children; /* 0x0C */
    struct SdfModel *root;        /* 0x10: backlink installed by sdfAppendBufferedRouteNode */
    u8 pad14[4];
    s32 nodeId;                   /* 0x18: lookup key when model flags bit 0 is set */
    u8 pad1C[0x14];
    u32 address;                  /* 0x30 */
    s32 boundsAddress;            /* 0x34: optional address of two local xyz box corners */
    void *sourceItem;             /* 0x38: item this node was built from */
    u8 pad3C[0x14];
    u8 quaternion[0x10];          /* 0x50 */
    u8 vectors[5][0x10];          /* 0x60-0xAF: COP2 inputs */
    u8 localTranslationRow[0x10]; /* 0xB0: fourth row written by sdfDrawNodeBuildMatrix */
    u8 worldMatrix[0x40];         /* 0xC0: local transform composed with its parent */
} SdfDrawNode;

struct SdfMotionManager;
struct ArrObj;

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
    struct SdfMotionManager *owner;
    MotionTable *motionTable;
    s32 unkC;
    struct ArrObj *request;
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
