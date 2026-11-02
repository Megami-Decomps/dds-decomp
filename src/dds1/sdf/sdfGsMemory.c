#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

/* GS pixel storage modes, the values of sdfGsMemSlice_t.pixelFormat. */
enum sdf_psm
{
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
    SDF_PSMT4HL = 0x24,
    SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30,
    SDF_PSMZ24 = 0x31,
    SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A
};

extern s32 DIntr(void);

extern void EIntr(void);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern sdfGsMemSlice_t *gsMemHandleLinkHead;

extern sdfGsMemSlice_t *gsMemHandleLinkTail;

extern sdfDelayedResrcCB_t gsMemDelayedResrcCB;

extern void (*sdfGsErrorHandler)(s32 size, s32 type);

/* sdfImageSetPacket_tag.freeMode: which heap owns the uploaded pixels. */
enum SdfImageSetMem
{
    SDF_IMAGESET_MEM_IGNORE = 0,
    SDF_IMAGESET_MEM_HANDLE = 1,
    SDF_IMAGESET_MEM_CHIP = 2
};

s32 sdfGsMemGetBitCount(u32 aFormat);

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 aSize, s32 aType);

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType);

s32 sdfGsMemCheckMerge(sdfGsMemSlice_t *aHandle);

void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

s32 sdfGsMemGetBitCount(u32 aFormat)
{
    switch (aFormat) {
    case SDF_PSMCT32:
    case SDF_PSMCT24:
    case SDF_PSMT8H:
    case SDF_PSMT4HL:
    case SDF_PSMT4HH:
    case SDF_PSMZ32:
    case SDF_PSMZ24:
        return 0x20;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        return 0x10;
    case SDF_PSMT8:
        return 8;
    case SDF_PSMT4:
        return 4;
    default:
        return 0;
    }
}

s32 sdfGetTexelBitCount(u32 aFormat)
{
    switch (aFormat) {
    case SDF_PSMCT32:
    case SDF_PSMZ32:
        return 0x20;
    case SDF_PSMCT24:
    case SDF_PSMZ24:
        return 0x18;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        return 0x10;
    case SDF_PSMT8:
    case SDF_PSMT8H:
        return 8;
    case SDF_PSMT4:
    case SDF_PSMT4HL:
    case SDF_PSMT4HH:
        return 4;
    default:
        return 0;
    }
}

s32 sdfGsMemCheckHandle(sdfGsMemSlice_t *aHandle) {
    sdfGsMemSlice_t *node = gsMemHandleLinkHead;

    if (node == NULL) {
        return 0;
    }
    do {
        if (node == (void *)aHandle) {
            return 1;
        }
        node = node->next;
    } while (node != NULL);
    return 0;
}

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 aSize, s32 aType)
{
    sdfGsMemSlice_t *slice;
    sdfGsMemSlice_t *alloc;
    s32 reenable;

    reenable = DIntr();
    for (slice = gsMemHandleLinkHead;; slice = slice->next) {
        if (slice == NULL) {
            if (reenable != 0) {
                EIntr();
            }
            if (sdfGsErrorHandler != NULL) {
                sdfGsErrorHandler(aSize, aType);
            }
        }
        if (slice->type != 0 || slice->size < aSize) {
            continue;
        }
        if (slice->size == aSize) {
            slice->type = aType;
            if (reenable != 0) {
                EIntr();
            }
            return slice;
        }
        alloc = sdfChipAllocDirect(sizeof(*alloc));
        alloc->prev = slice->prev;
        if (alloc->prev == NULL) {
            gsMemHandleLinkHead = alloc;
        } else {
            slice->prev->next = alloc;
        }
        alloc->type = aType;
        slice->prev = alloc;
        alloc->addr = slice->addr;
        alloc->size = aSize;
        alloc->next = slice;
        slice->addr += aSize;
        slice->size -= aSize;
        if (reenable != 0) {
            EIntr();
        }
        return alloc;
    }
}

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType)
{
    sdfGsMemSlice_t *slice;
    sdfGsMemSlice_t *alloc;
    s32 reenable;

    reenable = DIntr();
    for (slice = gsMemHandleLinkTail;; slice = slice->prev) {
        if (slice == NULL) {
            if (reenable != 0) {
                EIntr();
            }
            if (sdfGsErrorHandler != NULL) {
                sdfGsErrorHandler(aSize, aType);
            }
        }
        if (slice->type != 0 || slice->size < aSize) {
            continue;
        }
        if (slice->size == aSize) {
            slice->type = aType;
            if (reenable != 0) {
                EIntr();
            }
            return slice;
        }
        alloc = sdfChipAllocDirect(sizeof(*alloc));
        alloc->next = slice->next;
        if (alloc->next == NULL) {
            gsMemHandleLinkTail = alloc;
        } else {
            slice->next->prev = alloc;
        }
        alloc->type = aType;
        slice->next = alloc;
        alloc->addr = slice->addr + (slice->size - aSize);
        alloc->size = aSize;
        alloc->prev = slice;
        slice->size -= aSize;
        if (reenable != 0) {
            EIntr();
        }
        return alloc;
    }
}

sdfGsMemSlice_t *sdfGsMemAllocRect(s32 aWidth, s32 aHeight, s32 aFormat, s32 aType, s32 aFromHead)
{
    s32 bitCount = sdfGsMemGetBitCount(aFormat);
    s32 words = (aWidth * aHeight * bitCount) >> 5;
    sdfGsMemSlice_t *slice;

    if (aFromHead != 0) {
        slice = sdfGsMemAllocFromHead(words, aType);
    } else {
        slice = sdfGsMemAllocFromTail(words, aType);
    }
    slice->w = aWidth;
    slice->h = aHeight;
    slice->pixelFormat = aFormat;
    return slice;
}

s32 sdfGsMemCheckMerge(sdfGsMemSlice_t *aHandle)
{
    sdfGsMemSlice_t *below = aHandle->next;

    if (below == NULL || below->type != 0) {
        return 0;
    }
    aHandle->size += below->size;
    if ((aHandle->next = below->next) != NULL) {
        below->next->prev = aHandle;
    } else {
        gsMemHandleLinkTail = aHandle;
    }
    sdfChipFreeDirect(below);
    return 1;
}

void sdfGsMemCheckLinkConsistence(void) {
    sdfGsMemSlice_t *node;

    node = gsMemHandleLinkHead;
    while (node->next != NULL) {
        node = node->next;
    }
    node = gsMemHandleLinkTail;
    while (node->prev != NULL) {
        node = node->prev;
    }
}

void sdfGsMemFree(sdfGsMemSlice_t *aHandle)
{
    s32 reenable;

    if (aHandle == NULL) {
        return;
    }
    reenable = DIntr();
    aHandle->type = 0;
    sdfGsMemCheckMerge(aHandle);
    if (aHandle->prev != NULL) {
        if (aHandle->prev->type == 0) {
            sdfGsMemCheckMerge(aHandle->prev);
        }
    }
    if (reenable != 0) {
        EIntr();
    }
}

void sdfGsMemFreeDelayed(sdfGsMemSlice_t *aHandle) {
    sdfDelayedResrcEntry(&gsMemDelayedResrcCB, aHandle);
}

void sdfGsMemInit(void)
{
    sdfGsMemSlice_t *slice;

    slice = sdfChipAllocDirect(sizeof(sdfGsMemSlice_t));
    slice->size = 0x100000;
    slice->prev = NULL;
    slice->next = NULL;
    slice->type = 0;
    slice->addr = 0;
    gsMemHandleLinkHead = slice;
    gsMemHandleLinkTail = slice;
    sdfDelayedResrcInitCB(&gsMemDelayedResrcCB, sdfGsMemFree);
}

sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 aW, s32 aH, s32 aPixelFormat)
{
    s32 rowWidth = (aW + 0x3F) & -0x40;
    s32 rowCount = (aH + 0x1F) & -0x20;
    sdfGsMemSlice_t *slice;

    switch (aPixelFormat) {
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        rowWidth >>= 1;
        break;
    case SDF_PSMCT32:
    case SDF_PSMCT24:
    case SDF_PSMZ32:
    case SDF_PSMZ24:
        break;
    }
    slice = sdfGsMemAllocFromHead((rowWidth * rowCount + 0x7FF) & -0x800, 1);
    slice->w = aW;
    slice->h = aH;
    slice->pixelFormat = aPixelFormat;
    return slice;
}

sdfGsMemSlice_t *sdfGsMemGetLinkHead(void) {
    return gsMemHandleLinkHead;
}

sdfGsMemSlice_t *sdfGsMemGetLinkTail(void) {
    return gsMemHandleLinkTail;
}

INCLUDE_SDATA(const s32, "sdf/sdfGsMemory", sdfGsErrorHandler);

