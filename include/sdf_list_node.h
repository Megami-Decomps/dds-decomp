#ifndef SDF_LIST_NODE_H
#define SDF_LIST_NODE_H

#include "common.h"

/* The 0x14-byte indexed node shared by the native linked-list helpers. */
typedef struct SdfListNode {
    u32 index;                  /* 0x00 */
    s32 key;                    /* 0x04 */
    struct SdfListNode *next;   /* 0x08 */
    struct SdfListNode *prev;   /* 0x0C */
    void *value;                /* 0x10 */
} SdfListNode;

typedef char SdfListNode_size_must_be_0x14[(sizeof(SdfListNode) == 0x14) ? 1 : -1];

#endif
