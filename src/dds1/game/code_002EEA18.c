#include "common.h"
#include "sdf.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"

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

typedef struct SdfPacCounter {
    u32 count;     /* 0x0 */
    u32 progress;  /* 0x4 */
} SdfPacCounter;

typedef struct SdfPacList {
    u8 pad00[8];
    DevRequest *list; /* 0x8 */
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
extern DevRequest *sdfCreateConfiguredBufferedResourceList(u32 count);
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
extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfPacReadListAllocationCount();

void sdfQueueAndResetPacketWork(SdfAllocWork *work) {
    sdfPacEnqueuePacket(work);
    work->buffer = sdfAllocSizeClassBlock(0x30);
    work->handler = sdfPacReadListAllocationCount;
}

enum {
    SDF_DECODE_READ_COMMAND = 1,
    SDF_DECODE_READ_COUNT_LOW = 2,
    SDF_DECODE_READ_COUNT_HIGH = 3,
    SDF_DECODE_DISPATCH = 4,
    SDF_DECODE_COPY_LITERAL = 5,
    SDF_DECODE_FILL_BYTE = 6,
    SDF_DECODE_COPY_SHORT_BACKREF = 7,
    SDF_DECODE_READ_BACKREF_LOW = 8,
    SDF_DECODE_READ_BACKREF_HIGH = 9,
    SDF_DECODE_EXPAND_BYTES = 10
};

typedef struct SdfPacDecoder {
    u8 state;
    u8 command;
    u16 pendingBytes;
    u8 *output;
    u8 *input;
    s32 inputBytes;
    u8 backrefLow;
    u8 pad11[3];
} SdfPacDecoder;

extern s32 D_003BDAC0;
extern u8 *D_003FF300[16];

/* Incrementally decode the PAC byte stream, preserving partial commands when
 * the caller's input chunk ends. */
s32 func_002EEAE0(SdfPacDecoder *decoder, u8 *input, s32 inputBytes) {
    s32 count;
    s32 offset;
    u8 *output;
    u32 sourceAddress;

    if (decoder->state == 0) {
        return 1;
    }

    D_003BDAC0 = 0;
    output = decoder->output;
    if (inputBytes > 0) {
        do {
            switch (decoder->state) {
                case SDF_DECODE_READ_COMMAND: {
                    u8 command;
                    s32 commandBytes;
                    s32 historyIndex = D_003BDAC0;

                    inputBytes--;
                    D_003FF300[historyIndex] = input;
                    D_003BDAC0 = (historyIndex + 1) & 0xF;
                    command = *input++;
                    if (command >= 0xE0) {
                        if (command == 0xFF) {
                            decoder->output = output;
                            decoder->input = input;
                            decoder->inputBytes = inputBytes;
                            decoder->state = 0;
                            return 1;
                        }
                    } else {
                        decoder->command = command & 0xE0;
                        commandBytes = command & 0x1F;
                        if (commandBytes == 0) {
                            decoder->state = SDF_DECODE_READ_COUNT_LOW;
                        } else {
                            decoder->pendingBytes = commandBytes;
                            decoder->state = SDF_DECODE_DISPATCH;
                        }
                    }
                    break;
                }
                case SDF_DECODE_READ_COUNT_LOW:
                    decoder->pendingBytes = *input++;
                    inputBytes--;
                    decoder->state = SDF_DECODE_READ_COUNT_HIGH;
                    break;
                case SDF_DECODE_READ_COUNT_HIGH:
                    decoder->pendingBytes |= *input++ << 8;
                    inputBytes--;
                    decoder->state = SDF_DECODE_DISPATCH;
                    break;
                case SDF_DECODE_DISPATCH:
                    switch (decoder->command) {
                    case 0:
                        decoder->state = SDF_DECODE_COPY_LITERAL;
                        break;
                    case 0x20:
                        count = decoder->pendingBytes;
                        memset(output, 0, count);
                        output += count;
                        decoder->state = SDF_DECODE_READ_COMMAND;
                        break;
                    case 0x40:
                        decoder->state = SDF_DECODE_FILL_BYTE;
                        break;
                    case 0x60:
                        decoder->state = SDF_DECODE_COPY_SHORT_BACKREF;
                        break;
                    case 0x80:
                        decoder->state = SDF_DECODE_READ_BACKREF_LOW;
                        break;
                    case 0xA0:
                        decoder->state = SDF_DECODE_EXPAND_BYTES;
                        break;
                    }
                    break;
                case SDF_DECODE_COPY_LITERAL:
                    count = decoder->pendingBytes;
                    if (inputBytes < count) {
                        count = inputBytes;
                    }
                    memcpy(output, input, count);
                    output += count;
                    input += count;
                    inputBytes -= count;
                    decoder->pendingBytes -= count;
                    if (decoder->pendingBytes == 0) {
                        decoder->state = SDF_DECODE_READ_COMMAND;
                    }
                    break;
                case SDF_DECODE_FILL_BYTE:
                    count = decoder->pendingBytes;
                    memset(output, *input++, count);
                    output += count;
                    inputBytes--;
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                case SDF_DECODE_COPY_SHORT_BACKREF:
                    sourceAddress = (u32)(output - *input++);
                    count = decoder->pendingBytes;
                    inputBytes--;
                    do {
                        *output++ = *(u8 *)sourceAddress++;
                    } while (--count != 0);
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                case SDF_DECODE_READ_BACKREF_LOW:
                    decoder->backrefLow = *input++;
                    inputBytes--;
                    if (inputBytes == 0) {
                        decoder->state = SDF_DECODE_READ_BACKREF_HIGH;
                        break;
                    }
                    /* fallthrough */
                case SDF_DECODE_READ_BACKREF_HIGH: {
                    s32 combinedOffset;

                    offset = *input++ << 8;
                    sourceAddress = decoder->backrefLow;
                    inputBytes--;
                    combinedOffset = sourceAddress | offset;
                    sourceAddress = (u32)(output - combinedOffset);
                    count = decoder->pendingBytes;
                    do {
                        *output++ = *(u8 *)sourceAddress++;
                    } while (--count != 0);
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                }
                case SDF_DECODE_EXPAND_BYTES:
                    count = decoder->pendingBytes;
                    if (inputBytes < count) {
                        count = inputBytes;
                        inputBytes = 0;
                        decoder->pendingBytes -= count;
                    } else {
                        decoder->pendingBytes = 0;
                        decoder->state = SDF_DECODE_READ_COMMAND;
                        inputBytes -= count;
                    }
                    do {
                        *output++ = *input++;
                        *output++ = 0;
                    } while (--count != 0);
                    break;
            }
        } while (inputBytes > 0);
    }
    decoder->input = input;
    decoder->inputBytes = inputBytes;
    decoder->output = output;
    return 0;
}

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

/* Initialize a zeroed TMX0 header; retain the caller-supplied flag bytes. */
void sdfInitializeTmxImageHeader(SdfTmxHeader *header, s32 width, s32 height, s32 depth, s32 flagA, s32 flagB) {
    memset(header, 0, sizeof(SdfTmxHeader));
    header->flagB = flagB;
    header->width = width;
    header->height = height;
    header->depth = depth;
    header->flagA = flagA;
    header->magic = SDF_TMX_MAGIC;
    header->type = 2;
}

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
    s32 (*callback)(struct DevState *, s32, s32, s32, s32);
    s32 callbackContext;
} DevState;

/* Command 34 carries a buffer address and byte count after an opaque first word. */
typedef struct SdfStreamReadRequest {
    u8 pad00[4];
    s32 destinationAddress;
    s32 byteCount;
} SdfStreamReadRequest;

typedef struct SdfStreamCfg {
    DevState *deviceState;
    s32 semaphore;
    s32 readResult;
    s32 remainingBytes;
    s32 destinationAddress;
    s32 transferredBytes;
} SdfStreamCfg;

extern SdfStreamCfg D_003FF340;
extern u8 D_003FF4C0[];
extern void sdfDevQueueRead();
extern s32 D_003FF480[];
extern DevState *sdfDevCreateCallbackState(s32, s32 (*)(DevState *, s32, s32, s32, s32), s32);
extern s32 sdfDevReactivate(DevState *);
extern s32 sdfDevQueueReleaseState(DevState *);
extern s32 sdfDevQueueControlRequest();
extern s32 sdfDevQueueActiveOperation();
extern s32 WaitSema(s32);
extern s32 func_002EF408(DevState *, s32, s32, s32, s32);

/* Queue at most one staging-buffer-sized read; the completion callback advances the byte counts. */
void sdfStreamSendChunk(void) {
    s32 chunkBytes = D_003FF340.remainingBytes;

    if (chunkBytes > SDF_STREAM_CHUNK_BYTES) {
        chunkBytes = SDF_STREAM_CHUNK_BYTES;
    }
    sdfDevQueueRead(D_003FF340.deviceState, D_003FF4C0, chunkBytes);
}

extern void FlushCache(s32);
extern s32 SignalSema(s32);
extern s32 sceSifSetDma(void *, s32);
extern s32 sceSifDmaStat(s32);
extern s32 func_002CF930(void);
extern s32 sdfGetElapsedTimerTicks(s32);

s32 func_002EF408(DevState *deviceState, s32 command, s32 sourceAddress,
                  s32 byteCount, s32 callbackArg) {
    SdfStreamCfg *stream = &D_003FF340;
    s32 dmaId;
    s32 timer;

    switch (command) {
    case 4:
        stream->readResult = byteCount;
        SignalSema(stream->semaphore);
        break;
    case 5: {
        struct {
            s32 source;
            s32 destination;
            s32 size;
            s32 attributes;
        } transfer;

        FlushCache(0);
        transfer.source = sourceAddress;
        transfer.destination = stream->destinationAddress;
        transfer.size = byteCount;
        transfer.attributes = 0;
        dmaId = sceSifSetDma(&transfer, 1);
        while (sceSifDmaStat(dmaId) >= 0) {
            timer = func_002CF930();
            while (sdfGetElapsedTimerTicks(timer) <= 0) {
            }
        }
        timer = func_002CF930();
        while (sdfGetElapsedTimerTicks(timer) < 0x64) {
        }
        stream->destinationAddress += byteCount;
        stream->transferredBytes += byteCount;
        stream->remainingBytes -= byteCount;
        if (stream->remainingBytes > 0) {
            sdfStreamSendChunk();
        } else {
            SignalSema(stream->semaphore);
        }
        break;
    }
    case 2:
    case 7:
        SignalSema(stream->semaphore);
        break;
    case 0:
        SignalSema(stream->semaphore);
        break;
    }
    return 0;
}

/* Dispatch a stream command, wait for completion, and return the shared reply buffer.
 * request is a scalar argument except for a read command, which carries a read-request address.
 * An unrecognized command leaves the shared reply unchanged. */
s32 *sdfStreamDispatchSynchronousCommand(u32 command, s32 request) {
    SdfStreamCfg *state = &D_003FF340;
    DevState *deviceState;
    switch (command) {
    case SDF_STREAM_COMMAND_CREATE_STATE:
        state->deviceState = sdfDevCreateCallbackState(request, func_002EF408, 0);
        if (state->deviceState != 0) {
            WaitSema(state->semaphore);
            deviceState = state->deviceState;
            if (deviceState->result != 0) {
                sdfDevReactivate(deviceState);
                sdfDevQueueReleaseState(state->deviceState);
                state->deviceState = 0;
                deviceState = 0;
            }
        } else {
            deviceState = 0;
        }
        D_003FF480[0] = (s32)deviceState;
        break;
    case SDF_STREAM_COMMAND_CONTROL:
        sdfDevQueueControlRequest(state->deviceState, request, request);
        WaitSema(state->semaphore);
        D_003FF480[0] = state->readResult;
        break;
    case SDF_STREAM_COMMAND_READ:
        state->remainingBytes = ((SdfStreamReadRequest *)request)->byteCount;
        state->transferredBytes = 0;
        state->destinationAddress = ((SdfStreamReadRequest *)request)->destinationAddress;
        sdfStreamSendChunk();
        WaitSema(state->semaphore);
        D_003FF480[0] = state->transferredBytes;
        break;
    case SDF_STREAM_COMMAND_RELEASE_STATE:
        sdfDevQueueActiveOperation(state->deviceState, request, request);
        WaitSema(state->semaphore);
        sdfDevQueueReleaseState(state->deviceState);
        state->deviceState = 0;
        D_003FF480[0] = 0;
        break;
    }
    return D_003FF480;
}

typedef struct SceIoStat {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} SceIoStat;

extern s32 sdfCreateSemaphore(s32, s32, s32);
extern s32 GetThreadId(void);
extern void sceSifSetRpcQueue(void *, s32);
extern void sceSifRegisterRpc(void *, s32, void *, void *, s32, s32, void *);
extern void sceSifRpcLoop(void *);
extern s32 func_00310478(const char *, SceIoStat *);
extern s32 sdfSendNamedResourceRequest(char *, s32, void *, s32 *);
extern s32 WakeupThread(s32);
extern s32 D_003BDAC4;
extern u8 D_003FF380[];
extern u8 sdfPfsDebugMode;
void func_002EF698(void) {
    SdfStreamCfg *stream = &D_003FF340;
    SceIoStat info;
    u8 queue[0x20];
    u8 server[0x50];
    char *module;

    stream->semaphore = sdfCreateSemaphore(0, 1, 0);
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x66504F49, sdfStreamDispatchSynchronousCommand,
                      D_003FF380, 0, 0, queue);
    module = "cdrom0:\\USERIRX\\SDFIOPFL.IRX;1";
    if (sdfPfsDebugMode != 0 &&
        func_00310478("pfs0:/userirx/sdfiopfl.irx", &info) == 0 &&
        (info.mode & 0xF000) == 0x2000) {
        module = "pfs0:/userirx/sdfiopfl.irx";
    }
    sdfSendNamedResourceRequest(module, 0, NULL, NULL);
    WakeupThread(D_003BDAC4);
    sceSifRpcLoop(queue);
}

extern s32 sdfCreateThreadWithAllocatedWorkspace();
extern void _StartThread();
extern void SleepThread(void);

/* Start the newly created worker, then record and suspend the calling thread. */
void sdfStartAndSuspendWorkerThread(void) {
    s32 workerId = sdfCreateThreadWithAllocatedWorkspace(func_002EF698, 0x1000, 0x4C);

    _StartThread(workerId, 0);
    D_003BDAC4 = GetThreadId();
    SleepThread();
}

extern char *D_00398C80[];
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern char *strcpy(char *, const char *);
extern char *strcat(char *, const char *);
extern void func_002F5E30(void);

/* Load the selected pair of IOP module paths from prefix; loader results are not checked. */
void sdfLoadIopModulePair(const char *prefix, s32 moduleGroup) {
    char path[0x100];
    char **moduleNames = &D_00398C80[moduleGroup * SDF_IOP_MODULES_PER_GROUP];
    s32 moduleIndex;

    for (moduleIndex = 0; moduleIndex != SDF_IOP_MODULES_PER_GROUP; moduleIndex++) {
        strcpy(path, prefix);
        strcat(path, moduleNames[moduleIndex]);
        sceSifLoadModule(path, 0, 0);
    }
    func_002F5E30();
}


struct SdfKeyTreeItem;

/* Larger keys use the right link, which precedes the left link in memory. */
typedef struct SdfKeyTreeNode {
    struct SdfKeyTreeNode *right; /* 0x0 */
    struct SdfKeyTreeNode *left;  /* 0x4 */
    struct SdfKeyTreeItem *item;  /* 0x8 */
    s32 balance;                 /* 0xC */
} SdfKeyTreeNode;
/* Rebalance a right-heavy parent using its heavy child; return the new subtree root. */
SdfKeyTreeNode *sdfRotateBalancedTreeSecondLink(SdfKeyTreeNode *heavyChild, SdfKeyTreeNode *parent) {
    SdfKeyTreeNode *childRoot = heavyChild;
    SdfKeyTreeNode *pivot;

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
SdfKeyTreeNode *sdfRotateBalancedTreeFirstLink(SdfKeyTreeNode *heavyChild, SdfKeyTreeNode *parent) {
    SdfKeyTreeNode *childRoot = heavyChild;
    SdfKeyTreeNode *pivot;

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
void sdfRebalanceFloatKeyTreeAfterInsert(SdfKeyTreeNode **path, s32 depth, SdfKeyTreeNode *child,
                   SdfKeyTreeNode **root) {
    SdfKeyTreeNode *parent;
    SdfKeyTreeNode *replacement = NULL;
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
        SdfKeyTreeNode *ancestor = path[--depth];
        if (ancestor->right == parent) {
            ancestor->right = replacement;
        } else {
            ancestor->left = replacement;
        }
    } else if (replacement != NULL) {
        *root = replacement;
    }
}

typedef struct SdfKeyTreeItem {
    struct SdfKeyTreeItem *replaced; /* 0x0: previous item when a duplicate key replaces it */
    u8 pad04[8];
    f32 key;                      /* 0xC */
} SdfKeyTreeItem;


/* Insert `item` into the key-ordered tree; an equal key swaps the item in place. */
void sdfInsertFloatKeyTreeItem(SdfKeyTreeNode **tree, SdfKeyTreeItem *item) {
    SdfKeyTreeNode *path[SDF_KEY_TREE_PATH_LIMIT];
    s32 depth = 0;
    SdfKeyTreeNode **link = tree;
    SdfKeyTreeNode *cur = *tree;
    SdfKeyTreeNode *node;
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
            link = &cur->right;
        } else {
            link = &cur->left;
        }
        cur = *link;
    }
    item->replaced = NULL;
    node = sdfAllocPacketAligned(0x10);
    node->right = NULL;
    node->left = NULL;
    node->item = item;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        sdfRebalanceFloatKeyTreeAfterInsert(path, depth, node, tree);
    }
}

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
    SdfPoolNode *freeHead;
    u8 unk10[8];
    SdfKeyTreeNode *keyTree[1]; /* 0x18: key tree root */
} SdfPool;

extern void sdfInsertFloatKeyTreeItem();
extern void sdfRebuildPoolChain();

void sdfReleasePoolNode(SdfPool *pool, SdfPoolNode *node) {
    if (node->unk4 != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
            node->next = pool->freeHead;
            pool->freeHead = node;
        } else {
            sdfInsertFloatKeyTreeItem(pool->keyTree, node);
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

/* Append items in descending key order (right, item, left). The starting node must be non-null. */
void sdfKeyTreeApply(SdfKeyTreeNode *node, SdfPoolNode **head, SdfPoolNode **tail) {
    SdfKeyTreeNode *next;

    do {
        if (node->right != NULL) {
            sdfKeyTreeApply(node->right, head, tail);
        }
        sdfAppendPoolNodeChain(node->item, head, tail);
        next = node->left;
        node = next;
    } while (next != NULL);
}

/* Rebuild the pool's chain: collect the free list's chain, then append every key tree node's chain, and store the resulting head and tail. */
void sdfRebuildPoolChain(SdfPool *pool) {
    SdfPoolNode *head = NULL;
    SdfPoolNode *tail = NULL;

    sdfAppendPoolNodeChain(pool->freeHead, &head, &tail);
    if (pool->keyTree[0] != NULL) {
        sdfKeyTreeApply(pool->keyTree[0], &head, &tail);
    }
    pool->head = head;
    pool->tail = tail;
}

void sdfUpdatePoolFreeListByMode(SdfPool *pool, s32 mode, SdfPoolNode *node) {
    switch (mode) {
    case SDF_POOL_REBUILD_CHAIN:
        sdfRebuildPoolChain(pool);
        break;
    case SDF_POOL_PUSH_FREE:
        if (node->unk4 != 0) {
            node->next = pool->freeHead;
            pool->freeHead = node;
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

