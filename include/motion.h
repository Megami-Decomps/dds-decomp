#ifndef MOTION_H
#define MOTION_H

/* sdfMotion: one controller per playing animation, one node per animated
 * channel, over the key streams of a motion file.
 * See include/README.md for the evidence per struct. */

#include "common.h"
#include "libsdf.h"
#include "model.h"

typedef struct sdfMotionCtrlr_t sdfMotionCtrlr_t;
typedef struct sdfMotionNode_t sdfMotionNode_t;
typedef struct sdfMotionNodeList_t sdfMotionNodeList_t;
typedef struct sdfMotionPlayBook_t sdfMotionPlayBook_t;

/* One animated channel. sdfMotionNodeVTBL_t's five slots are the only way the
 * controller reaches a node. */
typedef struct
{
    void *destroy;
    void *setCut;
    void *seek;
    void *initBlend;
    void *blendSeek;
} sdfMotionNodeVTBL_t;

/* One channel's keys inside a cut: a header, then the key frame numbers as
 * halfwords, then the key data. sdfMotGetKey binary-searches the frame list
 * and indexes the data by keyElemSize; the data starts after the frame list
 * rounded up to a word. size advances to the next channel's chunk. */
typedef struct
{
    u32 size;
    u16 numOfKey;
    u16 keyElemSize;
    u16 keyFrameList[1];
} sdfMotionCutChunk_t;

/* One cut (one animation). sdfMotionChangeBlend reads frameLength, then walks
 * one chunk per node from +4. */
typedef struct
{
    u16 frameLength;
    u16 pad;
    sdfMotionCutChunk_t chunk[1];
} sdfMotionCutDef_t;

/* Which channel generator builds a node, and which model node, material or
 * morph entry it drives. */
typedef struct
{
    u32 typeId;
    u32 nodeId;
} sdfMotionNodeDef_t;

/* The head of a motion file's data, as sdfFixupMotionBinary returns it.
 * cutList is an array of pointers, one per cut; the node defs are inline. */
struct sdfMotionPlayBook_t
{
    u16 numOfCut;
    u16 numOfNode;
    sdfMotionCutDef_t **cutList;
    sdfMotionNodeDef_t nodeDefList[1];
};

struct sdfMotionNodeList_t
{
    sdfMemHandle_t *memHandle;
    s16 count;
    s16 capacity;
    s16 elemSize;
    s16 growStep;
    sdfMotionNode_t **items;
};

/* sdfMotionCtrlrNew allocates 52 bytes. next chains the controllers a root
 * owns, newest first; the root's head is sdfModelRootNode_t.motion. */
struct sdfMotionCtrlr_t
{
    sdfMotionCtrlr_t *next;
    sdfModelRootNode_t *rootNode;
    sdfMotionPlayBook_t *playbook;
    sdfMemHandle_t *playbookMemHandle; /* unconfirmed */
    sdfMotionNodeList_t *nodeList;
    f32 blendLength;
    f32 blendPreLength;
    f32 frame;
    f32 speed;
    void *userData; /* unconfirmed */
    s16 userId;
    s16 userId2;
    u16 cutNumber;
    u16 frameLength;
    u8 stat;
    u8 statBeforePause;
    u8 playType;
};

/* The base every channel node starts with. The derived types add their own
 * target and blend state after it. */
struct sdfMotionNode_t
{
    sdfMotionNodeVTBL_t *vtbl;
    sdfMotionCtrlr_t *owner;
    sdfMotionCutChunk_t *table;
};

/* sdfMotGenModelInit: resolves the node id through sdfModelSearchNode. */
typedef struct
{
    sdfMotionNode_t nodeBase;
    sdfModelNode_t *modelNode;
} sdfMotNodeModelBase_t;

/* sdfMotGenMatInit: indexes the root's material list by the node id. */
typedef struct
{
    sdfMotionNode_t nodeBase;
    sdfMat_t *mat;
} sdfMotNodeMatBase_t;

/* The pos, rotate and scale channels: 32 bytes, the extra quadword holding
 * what the blend starts from. */
typedef struct
{
    sdfMotNodeModelBase_t modelBase;
    sdfVector4_t blendFrom;
} sdfMotNodeModelVec_t;

/* What sdfMotGetKey fills in for its caller: the two keys the frame falls
 * between and how far between them it is. */
typedef struct
{
    void *key0;
    void *key1;
    f32 rate;
} sdfMotGetKeyIf_t;

/* sdfMotionCtrlr_t.stat. The values are the ones the code writes; the names
 * describe which function writes them. */
enum sdf_motion_stat
{
    SDF_MOTION_STAT_SET = 1,   /* sdfMotionChangeBlend, cut found */
    SDF_MOTION_STAT_SEEK = 4,  /* sdfMotionSeek */
    SDF_MOTION_STAT_NOCUT = 5, /* sdfMotionChangeBlend, cut missing */
    SDF_MOTION_STAT_PAUSE = 6  /* sdfMotionPause */
};

#endif /* MOTION_H */
