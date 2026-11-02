#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"
#include "sdf_named_resource_request_api.h"
#include "sce_io.h"
#include "sdf_dev_state.h"
#include "sdf_dev_event.h"
#include "sdf_pac_state.h"
#include "sdf_pac_decoder.h"
#include "sdf_pac_work.h"
#include "sdf_key_pool.h"
#include "sdf_packet_alloc.h"
#include "sdf_stream_state.h"

enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};

/* Rebalance a right-heavy parent using its heavy child; return the new subtree root. */
sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = heavyChild;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance < 0) {
        pivot = childRoot->left;
        parent->right = pivot->left;
        childRoot->left = pivot->right;
        pivot->left = parent;
        pivot->right = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->right = childRoot->left;
        childRoot->balance = 0;
        childRoot->left = parent;
        pivot = childRoot;
    }
    return pivot;
}

/* Rebalance a left-heavy parent using its heavy child; return the new subtree root. */
sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = heavyChild;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance > 0) {
        pivot = childRoot->right;
        parent->left = pivot->right;
        childRoot->right = pivot->left;
        pivot->right = parent;
        pivot->left = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->left = childRoot->right;
        childRoot->balance = 0;
        childRoot->right = parent;
        pivot = childRoot;
    }
    return pivot;
}

/* Walk the insertion path upward, updating balance flags and reconnecting a rotated subtree. */
void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **path, s32 depth, sdfSortOtNode_tag *child,
                   sdfSortOtNode_tag **root) {
    sdfSortOtNode_tag *parent;
    sdfSortOtNode_tag *replacement = NULL;
    s32 balance;

    if (depth > 0) {
        do {
            parent = path[--depth];
            if (child == parent->right) {
                balance = ++parent->balance;
            } else {
                balance = --parent->balance;
            }
            if (balance == 0) {
                return;
            }
            if (balance > 1) {
                replacement = sdfSortOtAvlRotateLR(child, parent);
                break;
            }
            if (balance < -1) {
                replacement = sdfSortOtAvlRotateRL(child, parent);
                break;
            }
            child = parent;
        } while (depth > 0);
    }
    if (depth > 0) {
        sdfSortOtNode_tag *ancestor = path[--depth];
        if (ancestor->right == parent) {
            ancestor->right = replacement;
        } else {
            ancestor->left = replacement;
        }
    } else if (replacement != NULL) {
        *root = replacement;
    }
}

/* Insert `item` into the key-ordered tree; an equal key swaps the item in place. */
void sdfSortOtAvlRegist(sdfSortOtNode_tag **tree, SdfKeyPoolNode *item) {
    sdfSortOtNode_tag *path[SDF_KEY_TREE_PATH_LIMIT];
    s32 depth = 0;
    sdfSortOtNode_tag **link = tree;
    sdfSortOtNode_tag *cur = *tree;
    sdfSortOtNode_tag *node;
    f32 key = item->key;

    while (cur != NULL) {
        f32 currentKey = cur->item->key;

        if (key == currentKey) {
            item->next = cur->item;
            cur->item = item;
            return;
        }
        path[depth] = cur;
        depth++;
        if (currentKey < key) {
            link = &cur->right;
        } else {
            link = &cur->left;
        }
        cur = *link;
    }
    item->next = NULL;
    node = sdfGrAllocPacketBuff(0x10);
    node->right = NULL;
    node->left = NULL;
    node->item = item;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        sdfSortOtAvlCheckBalance(path, depth, node, tree);
    }
}

void sdfGrSortOtReg(SdfPool *pool, SdfKeyPoolNode *node) {
    if (node->packetHeadAddress != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
            node->next = pool->freeHead;
            pool->freeHead = node;
        } else {
            sdfSortOtAvlRegist(pool->keyTree, node);
        }
    }
}

extern void sdfGrJoinDrawTag(void *left, void *right);

/* Append a pool-node chain after the list's tail, or make it the head of an empty list, then advance the tail to the chain's last node. */
void sdfGrSortOtFixup(void *first, SdfKeyPoolNode **head, SdfKeyPoolNode **tail) {
    SdfKeyPoolNode *current;
    SdfKeyPoolNode *next;

    if (first != NULL) {
        if (*tail == NULL) {
            *head = first;
        } else {
            sdfGrJoinDrawTag(*tail, first);
        }
        current = first;
        while ((next = current->next) != NULL) {
            sdfGrJoinDrawTag(current, next);
            current = next;
        }
        *tail = current;
    }
}

/* Append items in descending key order (right, item, left). The starting node must be non-null. */
void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, SdfKeyPoolNode **head, SdfKeyPoolNode **tail) {
    sdfSortOtNode_tag *next;

    do {
        if (node->right != NULL) {
            sdfGrSortOtTraverse(node->right, head, tail);
        }
        sdfGrSortOtFixup(node->item, head, tail);
        next = node->left;
        node = next;
    } while (next != NULL);
}

/* Rebuild the pool's chain: collect the free list's chain, then append every key tree node's chain, and store the resulting head and tail. */
void sdfGrSortOtSetup(SdfPool *pool) {
    SdfKeyPoolNode *head = NULL;
    SdfKeyPoolNode *tail = NULL;

    sdfGrSortOtFixup(pool->freeHead, &head, &tail);
    if (pool->keyTree[0] != NULL) {
        sdfGrSortOtTraverse(pool->keyTree[0], &head, &tail);
    }
    pool->head = head;
    pool->tail = tail;
}

void sdfGrSortOtCallback(SdfPool *pool, s32 mode, SdfKeyPoolNode *freeNode) {
    switch (mode) {
    case SDF_POOL_REBUILD_CHAIN:
        sdfGrSortOtSetup(pool);
        break;
    case SDF_POOL_PUSH_FREE:
        if (freeNode->packetHeadAddress != 0) {
            freeNode->next = pool->freeHead;
            pool->freeHead = freeNode;
        }
        break;
    }
}

void sdfModelSetOtCallbacks(SdfPool *pool) {
    pool->release = sdfGrSortOtReg;
    pool->update = sdfGrSortOtCallback;
}

