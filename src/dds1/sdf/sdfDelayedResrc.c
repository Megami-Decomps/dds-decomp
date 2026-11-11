#include "common.h"
#include "sdf_types.h"

/* Declared here because no header carries these. */
extern s32 SignalSema(s32 aSemaId);
extern s32 WaitSema(s32 sema);

#define SDF_PENDING_LAST_SLOT 1

struct sdfDelayedResrcCB_t;

struct sdfDelayedResrcItems_t;

typedef struct sdfDelayedResrcChunk_t sdfDelayedResrcChunk_t;

/* Deferred handlers share one address-word argument at the native dispatch
 * boundary, while their C consumers interpret that word as different owners. */
typedef void (*SdfPendingCallback)(void *);

void sdfChipFreeDirect(void *aPtr);

#define SDF_PENDING_BUFFER_CAPACITY 0x3F

#define SDF_PENDING_SLOT_COUNT 2

extern s8 sdfDelayedResrcFlag;

/* Interrupt-synchronized deferred callback owner (0x08). */
typedef struct sdfDelayedResrcCB_t {
    SdfPendingCallback freeFuncPtr;
    sdfDelayedResrcChunk_t *chunk;
} sdfDelayedResrcCB_t;

/* A queued batch links its next chunk before 63 deferred owner addresses. */
typedef struct sdfDelayedResrcItems_t {
    struct sdfDelayedResrcItems_t *next;
    void *items[SDF_PENDING_BUFFER_CAPACITY];
} sdfDelayedResrcItems_t;

extern sdfDelayedResrcChunk_t *sdfDelayedResrcLink;

void *sdfChipAllocDirect(s32 aSize);

extern s32 EIntr(void);

/* DDS names the pending slots; SMT3 has no name for them yet. */
extern saddr_t sdfPendingQueueSlots[2];

extern s32 DIntr(void);

extern s32 sdfDelayedResrcSemaId;

extern s32 sdfCreateSema(u32, u32, u32);

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

struct sdfDelayedResrcChunk_t {
    sdfDelayedResrcChunk_t *next;
    sdfDelayedResrcCB_t *owner;
    sdfDelayedResrcItems_t *items;
    s32 itemsFreeCount;
};

void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, SdfPendingCallback aFreeFunc);

void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void);

void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *aChunk);

u64 sdfDelayedResrcCheckFunc(void);

u64 sdfDelayedResrcCheck(void);

void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, SdfPendingCallback aFreeFunc) {
    if (sdfDelayedResrcSemaId < 0) {
        sdfDelayedResrcSemaId = sdfCreateSema(1, 0x7f, 0);
    }
    aCB->freeFuncPtr = aFreeFunc;
    aCB->chunk = NULL;
}

void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem) {
    sdfDelayedResrcChunk_t *pendingNode;
    sdfDelayedResrcItems_t *entryBuffer;
    s32 freeEntries;

    if (aItem != NULL) {
        WaitSema(sdfDelayedResrcSemaId);
        pendingNode = aCB->chunk;
        if (pendingNode == NULL) {
            pendingNode = (sdfDelayedResrcChunk_t *)sdfChipAllocDirect(sizeof(sdfDelayedResrcChunk_t));
            pendingNode->itemsFreeCount = 0;
            pendingNode->next = sdfDelayedResrcLink;
            pendingNode->items = NULL;
            pendingNode->owner = aCB;
            aCB->chunk = pendingNode;
            sdfDelayedResrcLink = pendingNode;
        }
        freeEntries = pendingNode->itemsFreeCount;
        entryBuffer = pendingNode->items;
        if (freeEntries == 0) {
            sdfDelayedResrcItems_t *newBuffer = (sdfDelayedResrcItems_t *)sdfChipAllocDirect(sizeof(sdfDelayedResrcItems_t));
            newBuffer->next = entryBuffer;
            pendingNode->items = newBuffer;
            entryBuffer = newBuffer;
            freeEntries = SDF_PENDING_BUFFER_CAPACITY;
        }
        entryBuffer->items[SDF_PENDING_BUFFER_CAPACITY - freeEntries] = aItem;
        pendingNode->itemsFreeCount = freeEntries - 1;
        SignalSema(sdfDelayedResrcSemaId);
    }
}

sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void) {
    sdfDelayedResrcChunk_t *head = sdfDelayedResrcLink;
    sdfDelayedResrcChunk_t *node;

    sdfDelayedResrcLink = NULL;
    for (node = head; node != NULL; node = node->next) {
        node->owner->chunk = NULL;
    }
    return head;
}

void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *aChunk) {
    sdfDelayedResrcChunk_t *nextPendingNode;
    sdfDelayedResrcItems_t *entryBuffer;
    sdfDelayedResrcItems_t *nextEntryBuffer;
    SdfPendingCallback entryHandler;
    s32 entryCount;
    s32 entryIndex;

    if (aChunk != NULL) {
        do {
            entryCount = SDF_PENDING_BUFFER_CAPACITY - aChunk->itemsFreeCount;
            entryHandler = aChunk->owner->freeFuncPtr;
            entryBuffer = aChunk->items;
            if (entryBuffer != NULL) {
                do {
                    entryIndex = 0;
                    do {
                        entryHandler(entryBuffer->items[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < entryCount);
                    nextEntryBuffer = entryBuffer->next;
                    sdfChipFreeDirect(entryBuffer);
                    entryBuffer = nextEntryBuffer;
                    entryCount = SDF_PENDING_BUFFER_CAPACITY;
                } while (entryBuffer != NULL);
            }
            nextPendingNode = aChunk->next;
            sdfChipFreeDirect(aChunk);
            aChunk = nextPendingNode;
        } while (aChunk != NULL);
    }
}

void sdfDelayedResrcProc(void) {
    u32 slotIndex;

    sdfDelayedResrcFlag = 1;
    WaitSema(sdfDelayedResrcSemaId);
    sdfDelayedResrcRelease(EE_ADDR_AS(sdfDelayedResrcChunk_t *, sdfPendingQueueSlots[0]));
    for (slotIndex = 0; slotIndex < SDF_PENDING_LAST_SLOT; slotIndex++) {
        sdfPendingQueueSlots[slotIndex] = sdfPendingQueueSlots[slotIndex + 1];
    }
    sdfPendingQueueSlots[SDF_PENDING_LAST_SLOT] = EE_ADDR_S(sdfDelayedResrcClose());
    SignalSema(sdfDelayedResrcSemaId);
    sdfDelayedResrcFlag = 0;
}

u64 sdfDelayedResrcCheckFunc(void) {
    u32 slotIndex;
    saddr_t *slotCursor;
    if (sdfDelayedResrcFlag != 0) {
        return 1;
    }
    if (sdfDelayedResrcLink != NULL) {
        return 1;
    }
    slotIndex = 0;
    slotCursor = sdfPendingQueueSlots;
    do {
        if (*slotCursor != 0) {
            return 1;
        }
        slotCursor++;
        slotIndex++;
    } while (slotIndex < SDF_PENDING_SLOT_COUNT);
    return 0;
}

u64 sdfDelayedResrcCheck(void) {
    s64 interruptState;
    u64 pendingWork;

    interruptState = DIntr();
    pendingWork = sdfDelayedResrcCheckFunc();
    if (interruptState != 0) {
        EIntr();
    }
    return pendingWork;
}

INCLUDE_SDATA(const s32, "sdf/sdfDelayedResrc", sdfDelayedResrcSemaId);

