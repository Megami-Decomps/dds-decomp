#include "common.h"
#include "libsdf.h"

/* The SDK multi-RPC client; only its byte span matters here. */
typedef struct
{
    u8 opaque00[0x24];
} sdfIopRpcClient_t;

/* Allocations round up to this many bytes. */
#define SDF_MEM_ALIGN_MASK 0x7F
/* The request header: two length words plus the name's terminator. */
#define SDF_IOP_FILE_RPC_OVERHEAD 0xC
#define SDF_IOP_FILE_RPC_REPLY_ALIGN_MASK 0x3F

extern void *malloc(u32 size);
extern s32 DIntr(void);
extern void EIntr(void);
extern void *memcpy(void *a, const void *b, u32 c);
extern u32 strlen(const char *s);
extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);
extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);
#include "sdk_libmrpc.h"

extern sdfMemHandleCB_tag sdfMemCB;
extern sdfIopRpcClient_t sdfIopRpcClient;
extern sdfDelayedResrcCB_t sdfMemDelayedResrcCB;
/* Called when the heap cannot satisfy a request; the unit's .sdata holds it. */
extern void (*sdfMemFullCallback)(s32 size);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
void sdfMemFreeMerge(sdfMemHandle_t *aHandle);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr);

/* Align the usable span to 128 bytes and link one free handle between the guards. */
void sdfMemInit(u32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    sdfMemHandle_t *freeHandle;
    u32 alignedTop;
    u32 alignedEnd;

    cb->allocPtr = malloc(aSize);
    cb->allocSize = aSize;
    freeHandle = sdfChipAllocDirect(sizeof(sdfMemHandle_t));
    alignedTop = ((u32)cb->allocPtr + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    alignedEnd = ((u32)cb->allocPtr + aSize) & ~SDF_MEM_ALIGN_MASK;
    cb->headGuard.prev = NULL;
    cb->headGuard.next = freeHandle;
    cb->headGuard.stat = SDF_MEM_STAT_END;
    cb->headGuard.mlock = -1;
    cb->tailGuard.stat = SDF_MEM_STAT_END;
    cb->tailGuard.mlock = -1;
    cb->tailGuard.ptr = (void *)alignedEnd;
    cb->tailGuard.prev = freeHandle;
    cb->tailGuard.next = NULL;
    cb->headGuard.ptr = (void *)alignedTop;
    freeHandle->prev = &cb->headGuard;
    freeHandle->stat = SDF_MEM_STAT_FREE;
    freeHandle->next = &cb->tailGuard;
    freeHandle->ptr = (void *)alignedTop;
    freeHandle->mlock = 0;
    sdfMemFullCallback = NULL;
    sdfDelayedResrcInitCB(&sdfMemDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfMemFreeDirect);
}

u16 sdfMemGetType(sdfMemHandle_t *aHandle)
{
    return aHandle->stat;
}

sdfMemHandle_t *sdfMemGetTop(void)
{
    return sdfMemCB.headGuard.next;
}

/* The next handle, or NULL when that is the end sentinel. */
sdfMemHandle_t *sdfMemGetNext(sdfMemHandle_t *aHandle)
{
    sdfMemHandle_t *next = aHandle->next;

    if (next->stat == SDF_MEM_STAT_END) {
        return NULL;
    }
    return next;
}

/* A handle's span runs to the next handle's start. */
s32 sdfMemGetSize(const sdfMemHandle_t *aHandle)
{
    const sdfMemHandle_t *next;
    s32 size;
    s32 intrEnabled;

    intrEnabled = DIntr();
    next = aHandle->next;
    size = (s32)next->ptr - (s32)aHandle->ptr;
    if (intrEnabled) {
        EIntr();
    }
    return size;
}

void *sdfMemGetPtr(sdfMemHandle_t *aHandle)
{
    return aHandle->ptr;
}

/* First-fit from the low end, splitting the handle it lands in. Never returns
 * empty-handed: when the walk reaches the end sentinel it reports the failure
 * and keeps walking. */
sdfMemHandle_t *sdfMemAllocDirect(s32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    s32 want = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    s32 intrEnabled;
    sdfMemHandle_t *handle;

    intrEnabled = DIntr();
    for (handle = cb->headGuard.next;; handle = handle->next) {
        if (handle->stat != SDF_MEM_STAT_FREE) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
                if (sdfMemFullCallback != NULL) {
                    sdfMemFullCallback(want);
                }
            }
        } else {
            s32 have = (s32)handle->next->ptr - (s32)handle->ptr;

            if (have >= want) {
                if (want < have) {
                    sdfMemHandle_t *split = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    split->prev = handle;
                    split->ptr = (void *)((s32)handle->ptr + want);
                    split->stat = SDF_MEM_STAT_FREE;
                    split->next = handle->next;
                    split->mlock = 0;
                    handle->next->prev = split;
                    handle->next = split;
                }
                handle->stat = SDF_MEM_STAT_USED;
                if (intrEnabled != 0) {
                    EIntr();
                }
                return handle;
            }
        }
    }
}

/* First-fit from the high end: the handle keeps its tail and the remainder
 * becomes a new free handle in front of it. */
sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    s32 want = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    s32 intrEnabled;
    sdfMemHandle_t *handle;

    intrEnabled = DIntr();
    for (handle = cb->tailGuard.prev;; handle = handle->prev) {
        if (handle->stat != SDF_MEM_STAT_FREE) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
            }
        } else {
            s32 have = (s32)handle->next->ptr - (s32)handle->ptr;

            if (have >= want) {
                if (want < have) {
                    sdfMemHandle_t *split = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    split->prev = handle;
                    split->next = handle->next;
                    split->ptr = (void *)((s32)handle->ptr + (have - want));
                    split->stat = SDF_MEM_STAT_FREE;
                    split->mlock = 0;
                    handle->next->prev = split;
                    handle->next = split;
                    handle = split;
                }
                handle->stat = SDF_MEM_STAT_USED;
                if (intrEnabled != 0) {
                    EIntr();
                }
                return handle;
            }
        }
    }
}

/* sdfMemAllocDirect without the full-heap callback: returns NULL instead. */
sdfMemHandle_t *sdfMemAlloc2Direct(s32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    s32 want = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    s32 intrEnabled;
    sdfMemHandle_t *handle;

    intrEnabled = DIntr();
    for (handle = cb->headGuard.next;; handle = handle->next) {
        if (handle->stat != SDF_MEM_STAT_FREE) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
                return NULL;
            }
        } else {
            s32 have = (s32)handle->next->ptr - (s32)handle->ptr;

            if (have >= want) {
                if (want < have) {
                    sdfMemHandle_t *split = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    split->prev = handle;
                    split->ptr = (void *)((s32)handle->ptr + want);
                    split->stat = SDF_MEM_STAT_FREE;
                    split->next = handle->next;
                    split->mlock = 0;
                    handle->next->prev = split;
                    handle->next = split;
                }
                handle->stat = SDF_MEM_STAT_USED;
                if (intrEnabled != 0) {
                    EIntr();
                }
                return handle;
            }
        }
    }
}

/* Resize the span a handle owns, keeping the handle itself. Growing absorbs or
 * trims the free neighbour when it can, and otherwise moves the data and swaps
 * the two handles' contents so the caller's handle survives. */
void sdfMemReallocRecord(sdfMemHandle_t *aHandle, s32 aSize)
{
    sdfMemHandle_t *next;
    sdfMemHandle_t *replacement;
    s32 ptr;
    s32 intrEnabled;
    s32 currentSize;
    s32 available;

    intrEnabled = DIntr();
    next = aHandle->next;
    ptr = (s32)aHandle->ptr;
    currentSize = (s32)next->ptr - ptr;
    aSize = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    if (aSize == currentSize) {
        if (intrEnabled != 0) {
            EIntr();
        }
        return;
    }
    if (currentSize < aSize) {
        available = (s32)next->next->ptr - (s32)next->ptr + currentSize;
        if (next->stat == SDF_MEM_STAT_FREE && available >= aSize) {
            if (available == aSize) {
                next->next->prev = aHandle;
                aHandle->next = next->next;
                sdfChipFreeDirect(next);
            } else {
                next->ptr = (void *)(ptr + aSize);
            }
        } else {
            s32 oldPtr;

            replacement = sdfMemAllocDirect(aSize);
            memcpy(replacement->ptr, aHandle->ptr, currentSize);
            ptr = (s32)aHandle->prev;
            oldPtr = (s32)aHandle->ptr;
            aHandle->prev = replacement->prev;
            aHandle->ptr = replacement->ptr;
            aHandle->next = replacement->next;
            replacement->prev = (sdfMemHandle_t *)ptr;
            replacement->next = next;
            replacement->ptr = (void *)oldPtr;
            aHandle->prev->next = aHandle;
            aHandle->next->prev = aHandle;
            replacement->prev->next = replacement;
            replacement->next->prev = replacement;
            sdfMemFreeDirect(replacement);
        }
    } else if (next->stat == SDF_MEM_STAT_FREE) {
        next->ptr = (void *)(ptr + aSize);
    } else {
        ptr = (s32)sdfChipAllocDirect(sizeof(sdfMemHandle_t));
        ((sdfMemHandle_t *)ptr)->prev = aHandle;
        ((sdfMemHandle_t *)ptr)->next = next;
        next->prev = (sdfMemHandle_t *)ptr;
        ((sdfMemHandle_t *)ptr)->stat = SDF_MEM_STAT_FREE;
        aHandle->next = (sdfMemHandle_t *)ptr;
        ((sdfMemHandle_t *)ptr)->mlock = 0;
        ((sdfMemHandle_t *)ptr)->ptr = (void *)((s32)aHandle->ptr + aSize);
    }
    if (intrEnabled != 0) {
        EIntr();
    }
}

/* Unlink the successor, not the handle itself, and reconnect both links. */
void sdfMemFreeMerge(sdfMemHandle_t *aHandle)
{
    sdfMemHandle_t *following = aHandle->next->next;

    following->prev = aHandle;
    aHandle->next = following;
}

/* Coalesce adjacent free handles, then recycle this handle or mark it free. */
void sdfMemFreeDirect(sdfMemHandle_t *aHandle)
{
    sdfMemHandle_t *next;
    s32 intrEnabled;

    if (aHandle == NULL) {
        return;
    }
    intrEnabled = DIntr();
    next = aHandle->next;
    if (next->stat == SDF_MEM_STAT_FREE) {
        sdfMemFreeMerge(aHandle);
        sdfChipFreeDirect(next);
    }
    if (aHandle->prev->stat == SDF_MEM_STAT_FREE) {
        sdfMemFreeMerge(aHandle->prev);
        sdfChipFreeDirect(aHandle);
    } else {
        aHandle->stat = SDF_MEM_STAT_FREE;
        aHandle->mlock = 0;
    }
    if (intrEnabled != 0) {
        EIntr();
    }
}

/* Resolve a data address to its handle before releasing it. */
void sdfMemFreePtrDirect(void *aPtr)
{
    sdfMemHandle_t *handle;

    handle = sdfMemGetHandleByPtr(aPtr);
    sdfMemFreeDirect(handle);
}

/* Clear the owner's slot before releasing the handle it held. */
void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr)
{
    sdfMemHandle_t *handle;

    handle = *aHandlePtr;
    if (handle != NULL) {
        *aHandlePtr = NULL;
        sdfMemFreeDirect(handle);
        return;
    }
}

/* NULL is a sentinel and is never queued. */
void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle)
{
    if (aHandle != NULL) {
        sdfDelayedResrcEntry(&sdfMemDelayedResrcCB, aHandle);
    }
}

void *sdfMemLock(sdfMemHandle_t *aHandle)
{
    aHandle->mlock = aHandle->mlock + 1;
    return aHandle->ptr;
}

/* Never decrements zero. */
void sdfMemUnlock(sdfMemHandle_t *aHandle)
{
    u16 mlock = aHandle->mlock;

    if (mlock != 0) {
        aHandle->mlock = mlock - 1;
    }
}

/* The address must belong to an existing used handle: the end-sentinel path
 * does not end this scan. */
sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    sdfMemHandle_t *handle;
    s32 intrEnabled;

    intrEnabled = DIntr();
    for (handle = cb->headGuard.next;; handle = handle->next) {
        if (handle->stat != SDF_MEM_STAT_USED) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
            }
        } else if (handle->ptr == aPtr) {
            if (intrEnabled != 0) {
                EIntr();
            }
            return handle;
        }
    }
}

void sdfMemGetInfo(sdfMemInfo_t *aInfo)
{
    sdfMemHandle_t *handle = sdfMemCB.headGuard.next;
    s32 total = 0;
    s32 freeSize = 0;
    s32 maxFreeSize = 0;
    s32 minFreeSize = 0;
    s32 handleCount = 0;
    s32 freeHandleCount = 0;
    s32 size;
    u16 stat;

    for (; handle->stat != SDF_MEM_STAT_END; handle = handle->next) {
        size = (s32)handle->next->ptr - (s32)handle->ptr;
        stat = handle->stat;
        handleCount++;
        total += size;
        if (stat == SDF_MEM_STAT_FREE) {
            if (minFreeSize == 0 || size < minFreeSize) {
                minFreeSize = size;
            }
            if (maxFreeSize < size) {
                maxFreeSize = size;
            }
            freeSize += size;
            freeHandleCount++;
        }
    }
    aInfo->total = total;
    aInfo->freeSize = freeSize;
    aInfo->minFreeSize = minFreeSize;
    aInfo->maxFreeSize = maxFreeSize;
    aInfo->handleCount = handleCount;
    aInfo->freeHandleCount = freeHandleCount;
}

/* The used handle whose span contains `aAddr`; NULL at the end sentinel. */
sdfMemHandle_t *sdfMemSearchHandle(s32 ptr)
{
    sdfMemHandle_t *handle = sdfMemCB.headGuard.next;
    sdfMemHandle_t *next;

    for (;; handle = next) {
        next = handle->next;
        if (handle->stat != SDF_MEM_STAT_USED) {
            if (handle->stat == SDF_MEM_STAT_END) {
                return NULL;
            }
        } else if (ptr >= (s32)handle->ptr && ptr < (s32)next->ptr) {
            return handle;
        }
    }
}

/* Pack the two lengths, the terminated name and the optional payload, then
 * return the transport error or the server's result. */
s32 SdfIopFileRpc(const char *aName, s32 aDataSize, const void *aData, s32 *aOutSize)
{
    u8 replyScratch[0x50];
    s32 nameLength = strlen(aName);
    s32 requestBytes = nameLength + aDataSize + SDF_IOP_FILE_RPC_OVERHEAD;
    u32 *request = (u32 *)sdfMemLock(sdfMemAllocDirect(requestBytes));
    u32 *reply;
    s32 result;

    request[0] = nameLength;
    request[1] = aDataSize;
    memcpy(request + 2, aName, nameLength + 1);
    if (aDataSize != 0) {
        memcpy((u8 *)request + nameLength + 9, aData, aDataSize);
    }
    /* The two-word reply has to start on a 64-byte boundary for the SIF. */
    reply = (u32 *)(((u32)replyScratch + SDF_IOP_FILE_RPC_REPLY_ALIGN_MASK) &
                    ~SDF_IOP_FILE_RPC_REPLY_ALIGN_MASK);
    result = sceSifMCallRpc((sceSifMClientData *)&sdfIopRpcClient, 1, 0, request, requestBytes, reply, 8, NULL, NULL);
    if (result >= 0) {
        if (aOutSize != NULL) {
            *aOutSize = reply[1];
        }
        result = reply[0];
    }
    return result;
}

INCLUDE_SDATA(const s32, "sdf/sdfMemory", sdfMemFullCallback);

INCLUDE_SDATA(const s32, "sdf/sdfMemory", sdfGrDrawFirstFlag);

INCLUDE_SDATA(const s32, "sdf/sdfMemory", sdfFrameEvenOdd);


/* DDS's build defines the heap root in this unit. */
sdfMemHandleCB_tag sdfMemCB __attribute__((section(".bss"), aligned(8)));
