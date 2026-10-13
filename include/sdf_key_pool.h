#ifndef SDF_KEY_POOL_H
#define SDF_KEY_POOL_H

#include "common.h"

struct SdfKeyTreeItem;

/* Pool release logic uses the next link, word at 0x4 and kind at 0xC;
 * bytes at 0x8 remain opaque. DDS1/2 game/code_002EEA18/003478C0.c. */
typedef struct SdfKeyPoolNode {
    struct SdfKeyPoolNode *next; /* 0x00 */
    s32 unk4;                    /* 0x04 */
    u8 unk8[4];                  /* 0x08 */
    s32 kind;                    /* 0x0C */
} SdfKeyPoolNode;

/* Tree view of a pool node: the link word at +0 becomes the replaced-item
 * link, +0x4..+0xB stay opaque, and +0xC is compared as a float key. */
typedef struct SdfKeyTreeItem {
    struct SdfKeyTreeItem *replaced; /* 0x00 */
    u8 pad04[8];                     /* 0x04: opaque */
    f32 key;                         /* 0x0C */
} SdfKeyTreeItem;

typedef struct SdfKeyTreeNode {
    struct SdfKeyTreeNode *right; /* 0x00 */
    struct SdfKeyTreeNode *left;  /* 0x04 */
    struct SdfKeyTreeItem *item;  /* 0x08 */
    s32 balance;                  /* 0x0C */
} SdfKeyTreeNode;

typedef struct SdfPool {
    u8 unk0[4];                    /* 0x00 */
    SdfKeyPoolNode *head;          /* 0x04: first node in the emitted chain */
    SdfKeyPoolNode *tail;          /* 0x08: last node in the emitted chain */
    SdfKeyPoolNode *freeHead;      /* 0x0C */
    u8 unk10[8];                   /* 0x10 */
    SdfKeyTreeNode *keyTree[1];    /* 0x18: first root; allocation continues */
} SdfPool;

typedef char SdfKeyPoolNode_size_must_be_0x10[(sizeof(SdfKeyPoolNode) == 0x10) ? 1 : -1];
typedef char SdfKeyTreeItem_size_must_be_0x10[(sizeof(SdfKeyTreeItem) == 0x10) ? 1 : -1];
typedef char SdfKeyTreeItem_key_must_be_at_C[
    ((u32)&((SdfKeyTreeItem *)0)->key == 0x0C) ? 1 : -1];
typedef char SdfKeyTreeNode_size_must_be_0x10[(sizeof(SdfKeyTreeNode) == 0x10) ? 1 : -1];
typedef char SdfPool_size_must_be_0x1C[(sizeof(SdfPool) == 0x1C) ? 1 : -1];

void sdfReleasePoolNode(SdfPool *pool, SdfKeyPoolNode *node);
void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfKeyPoolNode *node);

#endif
