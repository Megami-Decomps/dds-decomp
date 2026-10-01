#include "common.h"
#include "pcp_vu0.h"

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

typedef struct SdfAllocWork {
    u8 unk0[8];
    void (*handler)();
    u8 unkC[0x20];
    void *buffer;
} SdfAllocWork;

extern void sdfPacEnqueuePacket(SdfAllocWork *);
extern void *func_002CFEB8(s32 size);
extern void sdfPacReadListAllocationCount();

void sdfQueueAndResetPacketWork(SdfAllocWork *work) {
    sdfPacEnqueuePacket(work);
    work->buffer = func_002CFEB8(0x30);
    work->handler = sdfPacReadListAllocationCount;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEAE0);

typedef struct {
    u8 state;    /* 0x00 */
    u8 pad01[3];
    u32 value;   /* 0x04 */
} SdfWordState;

void sdfStoreWordAndSetState(SdfWordState *work, u32 value) {
    work->value = value;
    work->state = 1;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEEA8);

extern u8 D_00398470[];
extern void sdfPostmultiplyVuMatrixFromMemory();
extern void func_002EEEA8();

void func_002EF2B0(void) {
    VU0_LOAD_MATRIX(D_00398470);
    func_002EEEA8();
}

void func_002EF2E0(s32 a, s32 b, s32 c, s32 d) {
    sdfPostmultiplyVuMatrixFromMemory(D_00398470);
    func_002EEEA8(a, b, c, d);
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

typedef struct SdfStreamCfg {
    s32 dest;
    u8 unk4[8];
    s32 length;
} SdfStreamCfg;

extern SdfStreamCfg D_003FF340;
extern u8 D_003FF4C0[];
extern void sdfDevQueueRead();

void sdfStreamSendChunk(void) {
    s32 length = D_003FF340.length;

    if (length > 0x4000) {
        length = 0x4000;
    }
    sdfDevQueueRead(D_003FF340.dest, D_003FF4C0, length);
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF408);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF560);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF698);

extern void func_002EF698();
extern s32 sdfCreateThreadWithAllocatedWorkspace();
extern void _StartThread();
extern s32 GetThreadId(void);
extern void SleepThread(void);
extern s32 D_003BDAC4;

void sdfStartAndSuspendWorkerThread(void) {
    s32 stack = sdfCreateThreadWithAllocatedWorkspace(func_002EF698, 0x1000, 0x4C);

    _StartThread(stack, 0);
    D_003BDAC4 = GetThreadId();
    SleepThread();
}

extern char *D_00398C80[];
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern char *strcpy(char *, const char *);
extern char *strcat(char *, const char *);
extern void func_002F5E30(void);

/* Load the pair of IOP modules of `group` from the directory `prefix`. */
void sdfLoadIopModulePair(const char *prefix, s32 group) {
    char path[0x100];
    char **names = &D_00398C80[group * 2];
    s32 i;

    for (i = 0; i != 2; i++) {
        strcpy(path, prefix);
        strcat(path, names[i]);
        sceSifLoadModule(path, 0, 0);
    }
    func_002F5E30();
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF858);

struct SdfTreeItem;

typedef struct SdfTreeNode {
    struct SdfTreeNode *first;   /* 0x0 */
    struct SdfTreeNode *second;  /* 0x4 */
    struct SdfTreeItem *item;    /* 0x8 */
    s32 balance;                 /* 0xC */
} SdfTreeNode;

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

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF958);

typedef struct SdfTreeItem {
    struct SdfTreeItem *replaced; /* 0x0: previous item when a duplicate key replaces it */
    u8 pad04[8];
    f32 key;                      /* 0xC */
} SdfTreeItem;

extern void *sdfAllocPacketAligned();
extern void func_002EF958();

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
        func_002EF958(path, depth, node, tree);
    }
}

#define SDF_POOL_FREE_KIND 0xFFFF

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
extern void func_002EFC68();

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

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFB88);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFBF8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFC68);

void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfPoolNode *node) {
    switch (mode) {
    case 0:
        func_002EFC68(pool);
        break;
    case 1:
        if (node->unk4 != 0) {
            node->next = pool->free;
            pool->free = node;
        }
        break;
    }
}

extern void sdfReleasePoolNode();
extern void sdfUpdatePoolFreeListByMode();

void sdfInstallPoolNodeReleaseCallbacks(u32 *work) {
    work[4] = (u32)sdfReleasePoolNode;
    work[5] = (u32)sdfUpdatePoolFreeListByMode;
}

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50C0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50D0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50E0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50F0);

