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

extern void *func_00328D68(s32 size);

extern void sdfPacReadListAllocationCount();

extern u8 D_0040B620[];

extern void sdfPostmultiplyVuMatrixFromMemory();

extern void func_00347D50();

typedef struct SdfStreamCfg {
    s32 dest;
    u8 unk4[8];
    s32 length;
} SdfStreamCfg;

extern SdfStreamCfg D_0047BCC0;

extern u8 D_0047BE40[];

extern void sdfDevQueueRead();

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
    u8 unk0[0xC];
    SdfPoolNode *free;
    u8 unk10[8];
    u8 sub[1];
} SdfPool;

extern void sdfInsertFloatKeyTreeItem();

extern void func_00348B10();

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
    work->buffer = func_00328D68(0x30);
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

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348408);

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
SdfTreeNode *func_00348700(SdfTreeNode *a, SdfTreeNode *node) {
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

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348800);

typedef struct SdfTreeItem {
    struct SdfTreeItem *replaced; /* 0x0: previous item when a duplicate key replaces it */
    u8 pad04[8];
    f32 key;                      /* 0xC */
} SdfTreeItem;

extern void *sdfAllocPacketAligned();
extern void func_00348800();

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

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348A30);

typedef struct SdfKeyTreeNode {
    struct SdfKeyTreeNode *child; /* 0x00 */
    struct SdfKeyTreeNode *next;  /* 0x04 */
    void *item;                   /* 0x08 */
} SdfKeyTreeNode;

extern void func_00348A30(void *item, s32 first, s32 second);

/* Walk a key tree depth first, applying func_00348A30 to every node's item. */
void func_00348AA0(SdfKeyTreeNode *node, s32 first, s32 second) {
    SdfKeyTreeNode *next;

    do {
        if (node->child != NULL) {
            func_00348AA0(node->child, first, second);
        }
        func_00348A30(node->item, first, second);
        next = node->next;
        node = next;
    } while (next != NULL);
}

INCLUDE_ASM(const s32, "game/code_003478C0", func_00348B10);

void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfPoolNode *node) {
    switch (mode) {
    case 0:
        func_00348B10(pool);
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

