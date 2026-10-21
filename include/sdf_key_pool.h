#ifndef SDF_KEY_POOL_H
#define SDF_KEY_POOL_H

#include "common.h"

/* Pool nodes retain the same next link when chained at an equal tree key.
 * At +0xC, the free-node sentinel shares storage with the float sort key. */
typedef struct SdfKeyPoolNode {
    struct SdfKeyPoolNode *next; /* 0x00 */
    s32 packetHeadAddress;       /* 0x04: first DMA packet address. */
    u8 unk8[4];                  /* 0x08 */
    union {
        s32 kind; /* 0x0C: free-node sentinel */
        f32 key;  /* 0x0C: key-sorted packet nodes */
    };
} SdfKeyPoolNode;


typedef struct SdfKeyTreeNode {
    struct SdfKeyTreeNode *right; /* 0x00 */
    struct SdfKeyTreeNode *left;  /* 0x04 */
    SdfKeyPoolNode *item;         /* 0x08 */
    s32 balance;                  /* 0x0C */
} SdfKeyTreeNode;

/* A key-sorted draw-surface pool: an SdfPoolNode whose append and prepend
 * callbacks release a node into the pool and update its free list. */
typedef struct SdfPool {
    void *link;                                                /* 0x00: the pool entry's link */
    SdfKeyPoolNode *head;                                      /* 0x04: first node in the emitted chain */
    SdfKeyPoolNode *tail;                                      /* 0x08: last node in the emitted chain */
    SdfKeyPoolNode *freeHead;                                  /* 0x0C */
    void (*release)(struct SdfPool *, SdfKeyPoolNode *);       /* 0x10: the entry's append */
    void (*update)(struct SdfPool *, s32, SdfKeyPoolNode *);   /* 0x14: the entry's prepend */
    SdfKeyTreeNode *keyTree[1];                                /* 0x18: first root; allocation continues */
} SdfPool;

void sdfInsertFloatKeyTreeItem(SdfKeyTreeNode **tree, SdfKeyPoolNode *item);

typedef char SdfKeyPoolNode_size_must_be_0x10[(sizeof(SdfKeyPoolNode) == 0x10) ? 1 : -1];
typedef char SdfKeyPoolNode_kind_must_be_at_C[
    ((u32)&((SdfKeyPoolNode *)0)->kind == 0x0C) ? 1 : -1];
typedef char SdfKeyPoolNode_key_must_be_at_C[
    ((u32)&((SdfKeyPoolNode *)0)->key == 0x0C) ? 1 : -1];
typedef char SdfKeyTreeNode_size_must_be_0x10[(sizeof(SdfKeyTreeNode) == 0x10) ? 1 : -1];
typedef char SdfPool_size_must_be_0x1C[(sizeof(SdfPool) == 0x1C) ? 1 : -1];

void sdfReleasePoolNode(SdfPool *pool, SdfKeyPoolNode *node);
void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfKeyPoolNode *node);
void sdfInstallPoolNodeReleaseCallbacks(SdfPool *pool);

#endif
