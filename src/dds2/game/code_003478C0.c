#include "common.h"
#include "sdf.h"
#include "pcp_vu0.h"

typedef struct SdfRequest {
    u8 active;
    u8 pad01[3];
    u32 value;
} SdfRequest;

extern void sdfReleasePoolNode();

extern void sdfUpdatePoolFreeListByMode();

typedef struct SdfAllocWork {
    u8 unk0[8];
    void (*handler)();
    u8 unkC[0x20];
    void *buffer;
} SdfAllocWork;

extern void sdfPacEnqueuePacket(SdfAllocWork *);

extern void *sdfAllocSizeClassBlock(s32 size);

extern void sdfPacReadListAllocationCount();

extern u8 D_0040B620[];

extern void sdfPostmultiplyVuMatrixFromMemory();

extern void func_00347D50();

/* Device state returned by sdfDevCreateCallbackState; layout shared with the device manager. */
typedef struct DevState {
    struct DevState *next;
    struct DevState *previous;
    struct DevState *workerNext;
    struct DevState *workerPrev;
    void *resource;
    u8 workerIndex;
    u8 operation;
    s8 state;
    u8 pad17;
    s32 operationArg;
    s32 requestExtra;
    void *requestData;
    s32 options;
    s32 resourceId;
    s32 result;
    u8 pad30[8];
    void (*callback)(struct DevState *, s32, s32, s32, s32);
    s32 callbackContext;
} DevState;

/* Command 34 carries a buffer address and byte count after an opaque first word. */
typedef struct SdfStreamReadRequest {
    u8 pad00[4];
    s32 buffer;
    s32 length;
} SdfStreamReadRequest;

typedef struct SdfStreamCfg {
    DevState *dest;
    s32 semaphore;
    s32 readResult;
    s32 length;
    s32 buffer;
    s32 transferred;
} SdfStreamCfg;

extern SdfStreamCfg D_0047BCC0;

extern u8 D_0047BE40[];

extern void sdfDevQueueRead();
extern s32 D_0047BE00[];
extern DevState *sdfDevCreateCallbackState(s32, void (*)(DevState *, s32, s32, s32, s32), s32);
extern s32 sdfDevReactivate(DevState *);
extern s32 sdfDevQueueReleaseState(DevState *);
extern s32 sdfDevQueueControlRequest();
extern s32 sdfDevQueueActiveOperation();
extern s32 WaitSema(s32);
extern void func_003482B0(DevState *, s32, s32, s32, s32);

extern void func_00348540();

extern s32 sdfCreateThreadWithAllocatedWorkspace();

extern void _StartThread();

extern s32 GetThreadId(void);

extern void SleepThread(void);

extern s32 D_00439224;

typedef struct SdfPoolNode {
    struct SdfPoolNode *next;
    s32 unk4;
    u8 unk8[4];
    s32 kind;
} SdfPoolNode;

typedef struct SdfPool {
    u8 unk0[4];
    SdfPoolNode *head;  /* 0x04: first node of the chain */
    SdfPoolNode *tail;  /* 0x08: last node of the chain */
    SdfPoolNode *free;
    u8 unk10[8];
    struct SdfTreeNode *sub[1]; /* 0x18: key tree root */
} SdfPool;

extern void sdfInsertFloatKeyTreeItem();

extern void sdfRebuildPoolChain();

typedef struct SdfPacCounter {
    u32 count;     /* 0x0 */
    u32 progress;  /* 0x4 */
} SdfPacCounter;

typedef struct SdfPacList {
    u8 pad00[8];
    void *list;    /* 0x8 */
} SdfPacList;

typedef struct SdfPacRead {
    u8 pad00[8];
    void (*handler)();      /* 0x08 */
    u8 pad0C[4];
    u32 *input;             /* 0x10 */
    s32 active;             /* 0x14 */
    u8 pad18[8];
    u32 skipBytes;          /* 0x20 */
    u8 pad24[8];
    SdfPacCounter *counter; /* 0x2C */
    u8 pad30[4];
    SdfPacList *dest;       /* 0x34 */
} SdfPacRead;

extern void sdfPacAdvanceInput();
extern void *sdfCreateConfiguredBufferedResourceList();
extern void sdfPacSkipAllocationEntryBytes();

/* Read the entry count word, size the list for it and arm the skip handler. */
void sdfPacReadListAllocationCount(SdfPacRead *state) {
    u32 count;
    SdfPacList *dest;

    if (state->active != 0) {
        count = *state->input;
        sdfPacAdvanceInput(state, 4);
        dest = state->dest;
        state->counter->count = count;
        state->counter->progress = 0;
        dest->list = sdfCreateConfiguredBufferedResourceList(count);
        state->skipBytes = ((count * 4 + 0x53) & -0x40) - 0x14;
        state->handler = sdfPacSkipAllocationEntryBytes;
    }
}

void sdfQueueAndResetPacketWork(SdfAllocWork *work) {
    sdfPacEnqueuePacket(work);
    work->buffer = sdfAllocSizeClassBlock(0x30);
    work->handler = sdfPacReadListAllocationCount;
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347988);

void sdfStoreWordAndSetState(SdfRequest *request, u32 value) {
    request->value = value;
    request->active = 1;
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00347D50);

void func_00348158(void) {
    VU0_LOAD_MATRIX(D_0040B620);
    func_00347D50();
}

void func_00348188(s32 a, s32 b, s32 c, s32 d) {
    sdfPostmultiplyVuMatrixFromMemory(D_0040B620);
    func_00347D50(a, b, c, d);
}

typedef struct SdfTmxHeader {
    u8 type;          /* 0x00 */
    u8 pad01[7];
    u32 magic;        /* 0x08: 'TMX0' */
    u8 pad0C[4];
    u8 flagA;         /* 0x10 */
    u8 flagB;         /* 0x11 */
    u16 width;        /* 0x12 */
    u16 height;       /* 0x14 */
    u8 depth;         /* 0x16 */
    u8 pad17[0x29];
} SdfTmxHeader;

void sdfInitializeTmxImageHeader(SdfTmxHeader *hdr, s32 width, s32 height, s32 depth, s32 flagA, s32 flagB) {
    memset(hdr, 0, sizeof(SdfTmxHeader));
    hdr->flagB = flagB;
    hdr->width = width;
    hdr->height = height;
    hdr->depth = depth;
    hdr->flagA = flagA;
    hdr->magic = 0x30584D54;
    hdr->type = 2;
}

void sdfStreamSendChunk(void) {
    s32 length = D_0047BCC0.length;

    if (length > 0x4000) {
        length = 0x4000;
    }
    sdfDevQueueRead(D_0047BCC0.dest, D_0047BE40, length);
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_003482B0);

/* Dispatch a stream command, wait for completion, and return the shared reply buffer.
 * request is a scalar argument except for command 34, which carries a read-request address. */
s32 *sdfStreamDispatchSynchronousCommand(u32 command, s32 request) {
    SdfStreamCfg *state = &D_0047BCC0;
    DevState *deviceState;
    switch (command) {
    case 32:
        state->dest = sdfDevCreateCallbackState(request, func_003482B0, 0);
        if (state->dest != 0) {
            WaitSema(state->semaphore);
            deviceState = state->dest;
            if (deviceState->result != 0) {
                sdfDevReactivate(deviceState);
                sdfDevQueueReleaseState(state->dest);
                state->dest = 0;
                deviceState = 0;
            }
        } else {
            deviceState = 0;
        }
        D_0047BE00[0] = (s32)deviceState;
        break;
    case 33:
        sdfDevQueueControlRequest(state->dest, request, request);
        WaitSema(state->semaphore);
        D_0047BE00[0] = state->readResult;
        break;
    case 34:
        state->length = ((SdfStreamReadRequest *)request)->length;
        state->transferred = 0;
        state->buffer = ((SdfStreamReadRequest *)request)->buffer;
        sdfStreamSendChunk();
        WaitSema(state->semaphore);
        D_0047BE00[0] = state->transferred;
        break;
    case 35:
        sdfDevQueueActiveOperation(state->dest, request, request);
        WaitSema(state->semaphore);
        sdfDevQueueReleaseState(state->dest);
        state->dest = 0;
        D_0047BE00[0] = 0;
        break;
    }
    return D_0047BE00;
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348540);

void sdfStartAndSuspendWorkerThread(void) {
    s32 stack = sdfCreateThreadWithAllocatedWorkspace(func_00348540, 0x1000, 0x4C);

    _StartThread(stack, 0);
    D_00439224 = GetThreadId();
    SleepThread();
}

extern char *D_0040BE00[];
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern char *strcpy(char *, const char *);
extern char *strcat(char *, const char *);
extern void func_0034EC40(void);

/* Load the pair of IOP modules of `group` from the directory `prefix`. */
void sdfLoadIopModulePair(const char *prefix, s32 group) {
    char path[0x100];
    char **names = &D_0040BE00[group * 2];
    s32 i;

    for (i = 0; i != 2; i++) {
        strcpy(path, prefix);
        strcat(path, names[i]);
        sceSifLoadModule(path, 0, 0);
    }
    func_0034EC40();
}

struct SdfTreeItem;

typedef struct SdfTreeNode {
    struct SdfTreeNode *first;   /* 0x0 */
    struct SdfTreeNode *second;  /* 0x4 */
    struct SdfTreeItem *item;    /* 0x8 */
    s32 balance;                 /* 0xC */
} SdfTreeNode;

/* Balance-flag rotation on the second link: `node` takes the place under
 * `a`'s second link; returns the new subtree root. */
SdfTreeNode *sdfRotateBalancedTreeSecondLink(SdfTreeNode *a, SdfTreeNode *node) {
    SdfTreeNode *root = a;
    SdfTreeNode *pivot;

    if (root->balance < 0) {
        pivot = root->second;
        node->first = pivot->second;
        root->second = pivot->first;
        pivot->second = node;
        pivot->first = root;
        if (pivot->balance > 0) {
            node->balance = -1;
            root->balance = 0;
        } else {
            node->balance = 0;
            root->balance = 1;
        }
        pivot->balance = 0;
    } else {
        node->balance = 0;
        node->first = root->second;
        root->balance = 0;
        root->second = node;
        pivot = root;
    }
    return pivot;
}

/* Balance-flag rotation: `node` takes the place under `a`'s first link; returns the new subtree root. */
SdfTreeNode *sdfRotateBalancedTreeFirstLink(SdfTreeNode *a, SdfTreeNode *node) {
    SdfTreeNode *root = a;
    SdfTreeNode *pivot;

    if (root->balance > 0) {
        pivot = root->first;
        node->second = pivot->first;
        root->first = pivot->second;
        pivot->first = node;
        pivot->second = root;
        if (pivot->balance > 0) {
            node->balance = -1;
            root->balance = 0;
        } else {
            node->balance = 0;
            root->balance = 1;
        }
        pivot->balance = 0;
    } else {
        node->balance = 0;
        node->second = root->first;
        root->balance = 0;
        root->first = node;
        pivot = root;
    }
    return pivot;
}

void func_00348800(SdfTreeNode **path, s32 depth, SdfTreeNode *child,
                   SdfTreeNode **root) {
    SdfTreeNode *parent;
    SdfTreeNode *replacement = NULL;
    s32 balance;

    if (depth > 0) {
        do {
            parent = path[--depth];
            if (child == parent->first) {
                balance = ++parent->balance;
            } else {
                balance = --parent->balance;
            }
            if (balance == 0) {
                return;
            }
            if (balance > 1) {
                replacement = sdfRotateBalancedTreeSecondLink(child, parent);
                break;
            }
            if (balance < -1) {
                replacement = sdfRotateBalancedTreeFirstLink(child, parent);
                break;
            }
            child = parent;
        } while (depth > 0);
    }
    if (depth > 0) {
        SdfTreeNode *ancestor = path[--depth];
        if (ancestor->first == parent) {
            ancestor->first = replacement;
        } else {
            ancestor->second = replacement;
        }
    } else if (replacement != NULL) {
        *root = replacement;
    }
}

typedef struct SdfTreeItem {
    struct SdfTreeItem *replaced; /* 0x0: previous item when a duplicate key replaces it */
    u8 pad04[8];
    f32 key;                      /* 0xC */
} SdfTreeItem;

extern void *sdfAllocPacketAligned();

/* Insert `item` into the key-ordered tree; an equal key swaps the item in place. */
void sdfInsertFloatKeyTreeItem(SdfTreeNode **tree, SdfTreeItem *item) {
    SdfTreeNode *path[32];
    s32 depth = 0;
    SdfTreeNode **link = tree;
    SdfTreeNode *cur = *tree;
    SdfTreeNode *node;
    f32 key = item->key;

    while (cur != NULL) {
        if (key == cur->item->key) {
            item->replaced = cur->item;
            cur->item = item;
            return;
        }
        path[depth] = cur;
        depth++;
        if (cur->item->key < key) {
            link = &cur->first;
        } else {
            link = &cur->second;
        }
        cur = *link;
    }
    item->replaced = NULL;
    node = sdfAllocPacketAligned(0x10);
    node->first = NULL;
    node->second = NULL;
    node->item = item;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        func_00348800(path, depth, node, tree);
    }
}

void sdfReleasePoolNode(SdfPool *pool, SdfPoolNode *node) {
    if (node->unk4 != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
            node->next = pool->free;
            pool->free = node;
        } else {
            sdfInsertFloatKeyTreeItem(pool->sub, node);
        }
    }
}

extern void sdfConnectPacketLists(void *left, void *right);

/* Append a pool-node chain after the list's tail, or make it the head of an empty list, then advance the tail to the chain's last node. */
void sdfAppendPoolNodeChain(void *first, SdfPoolNode **head, SdfPoolNode **tail) {
    SdfPoolNode *current;
    SdfPoolNode *next;

    if (first != NULL) {
        if (*tail == NULL) {
            *head = first;
        } else {
            sdfConnectPacketLists(*tail, first);
        }
        current = first;
        while ((next = current->next) != NULL) {
            sdfConnectPacketLists(current, next);
            current = next;
        }
        *tail = current;
    }
}

/* Walk a key tree depth first, applying sdfAppendPoolNodeChain to every node's item (sdfRebuildPoolChain passes the addresses of its two accumulators here). */
void sdfKeyTreeApply(SdfTreeNode *node, SdfPoolNode **head, SdfPoolNode **tail) {
    SdfTreeNode *next;

    do {
        if (node->first != NULL) {
            sdfKeyTreeApply(node->first, head, tail);
        }
        sdfAppendPoolNodeChain(node->item, head, tail);
        next = node->second;
        node = next;
    } while (next != NULL);
}

/* Rebuild the pool's chain: collect the free list's chain, then append every key tree node's chain, and store the resulting head and tail. */
void sdfRebuildPoolChain(SdfPool *pool) {
    SdfPoolNode *head = NULL;
    SdfPoolNode *tail = NULL;

    sdfAppendPoolNodeChain(pool->free, &head, &tail);
    if (pool->sub[0] != NULL) {
        sdfKeyTreeApply(pool->sub[0], &head, &tail);
    }
    pool->head = head;
    pool->tail = tail;
}

void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfPoolNode *node) {
    switch (mode) {
    case 0:
        sdfRebuildPoolChain(pool);
        break;
    case 1:
        if (node->unk4 != 0) {
            node->next = pool->free;
            pool->free = node;
        }
        break;
    }
}

void sdfInstallPoolNodeReleaseCallbacks(u32 *work) {
    work[4] = (u32)sdfReleasePoolNode;
    work[5] = (u32)sdfUpdatePoolFreeListByMode;
}

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDD0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDE0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EDF0);

INCLUDE_RODATA(const s32, "game/code_003478C0", D_0042EE00);

