#include "sdk_ps2.h"
#include "common.h"
#include "eeregs.h"
#include "sdf_timer.h"
#include "sdf_texture_queue.h"
#include "sdf_image_upload.h"
#include "sdf_thread.h"
#include "ee_mmi.h"
#include "sdf_chip.h"
#include "sdf_chip_range_api.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_graph_buffer_state.h"
#include "sdf_display_thread_state.h"
#include "sdf_draw_frame_execution_state.h"
#include "sdf_semaphore_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_texture_submit.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_pending.h"
#include "sdf_texture_release.h"
#include "sdf_image_packets.h"
#include "sdf_tex_copy_image_data_api.h"
#include "sdf_vu_transform_api.h"
#include "sdf_handler_node.h"

/* GS pixel-storage modes, using the same private names as sdfTex.c. */
enum {
    SDF_PSMCT32 = 0x0, SDF_PSMCT24 = 0x1, SDF_PSMCT16 = 0x2,
    SDF_PSMCT16S = 0xA, SDF_PSMT8 = 0x13, SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B, SDF_PSMT4HL = 0x24, SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30, SDF_PSMZ24 = 0x31, SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A
};

extern sdfGsMemSlice_t *gsMemHandleLinkTail;

extern sdfGsMemSlice_t *gsMemHandleLinkHead;

extern SdfPendingRequest gsMemDelayedResrcCB;

extern void (*sdfGrCaptureTarget)(u8 *);

extern void (*sdfGsErrorHandler)(s32 size, s32 allocationMode);

struct sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode);

/* Storage depth for supported formats: listed 24-bit/high-plane modes use 32 bits; unlisted is zero. */
s32 sdfGsMemGetBitCount(u32 format) {
    switch (format) {
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

/* Transfer pixel depth, including indexed high-plane and depth formats; unlisted values return zero. */
s32 sdfGetTexelBitCount(u32 format) {
    switch (format) {
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

s32 sdfGsMemCheckHandle(sdfTexHandle_t *target) {
    sdfGsMemSlice_t *node = gsMemHandleLinkHead;

    if (node == NULL) {
        return 0;
    }
    do {
        if (node == (void *)target) {
            return 1;
        }
        node = node->prev;
    } while (node != NULL);
    return 0;
}

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode) {
    sdfGsMemSlice_t *block;
    sdfGsMemSlice_t *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = DIntr();
    for (block = gsMemHandleLinkHead;; block = block->prev) {
        if (block == NULL) {
            if (interruptsEnabled != 0) {
                EIntr();
            }
            if (sdfGsErrorHandler != NULL) {
                sdfGsErrorHandler(size, allocationMode);
            }
        }
        if (block->allocationMode != 0 || block->size < size) {
            continue;
        }
        if (block->size == size) {
            block->allocationMode = allocationMode;
            if (interruptsEnabled != 0) {
                EIntr();
            }
            return block;
        }
        allocated = sdfChipAllocDirect(sizeof(*allocated));
        allocated->next = block->next;
        if (allocated->next == NULL) {
            gsMemHandleLinkHead = allocated;
        } else {
            block->next->prev = allocated;
        }
        allocated->allocationMode = allocationMode;
        block->next = allocated;
        allocated->word = block->word;
        allocated->size = size;
        allocated->prev = block;
        block->word += size;
        block->size -= size;
        if (interruptsEnabled != 0) {
            EIntr();
        }
        return allocated;
    }
}

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 size, s32 allocationMode) {
    sdfGsMemSlice_t *block;
    sdfGsMemSlice_t *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = DIntr();
    for (block = gsMemHandleLinkTail;; block = block->next) {
        if (block == NULL) {
            if (interruptsEnabled != 0) {
                EIntr();
            }
            if (sdfGsErrorHandler != NULL) {
                sdfGsErrorHandler(size, allocationMode);
            }
        }
        if (block->allocationMode != 0 || block->size < size) {
            continue;
        }
        if (block->size == size) {
            block->allocationMode = allocationMode;
            if (interruptsEnabled != 0) {
                EIntr();
            }
            return block;
        }
        allocated = sdfChipAllocDirect(sizeof(*allocated));
        allocated->prev = block->prev;
        if (allocated->prev == NULL) {
            gsMemHandleLinkTail = allocated;
        } else {
            block->prev->next = allocated;
        }
        allocated->allocationMode = allocationMode;
        block->prev = allocated;
        allocated->word = block->word + (block->size - size);
        allocated->size = size;
        allocated->next = block;
        block->size -= size;
        if (interruptsEnabled != 0) {
            EIntr();
        }
        return allocated;
    }
}

/* Convert storage bits to 32-bit VRAM words before selecting an allocation direction. */
sdfGsMemSlice_t *sdfGsMemAllocRect(s32 width, s32 height, s32 format, s32 allocationMode, s32 useFirstAllocator) {
    s32 storageBitsPerPixel = sdfGsMemGetBitCount(format);
    s32 sizeWords = (width * height * storageBitsPerPixel) >> 5;
    sdfGsMemSlice_t *textureBlock;

    if (useFirstAllocator != 0) {
        textureBlock = sdfGsMemAllocFromHead(sizeWords, allocationMode);
    } else {
        textureBlock = sdfGsMemAllocFromTail(sizeWords, allocationMode);
    }
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

/* Absorb the previous free range into block; return one when its record is released. */
s32 sdfGsMemCheckMerge(sdfGsMemSlice_t *block) {
    sdfGsMemSlice_t *previousBlock = block->prev;

    if (previousBlock == NULL || previousBlock->allocationMode != 0) {
        return 0;
    }
    block->size += previousBlock->size;
    if ((block->prev = previousBlock->prev) != NULL) {
        previousBlock->prev->next = block;
    } else {
        gsMemHandleLinkTail = block;
    }
    sdfChipFreeDirect((sdfTexHandle_t *)previousBlock);
    return 1;
}

/* Walk both list endpoints without returning them or changing either list head. */
void sdfGsMemCheckLinkConsistence(void) {
    sdfGsMemSlice_t *node;

    node = gsMemHandleLinkHead;
    while (node->prev != NULL) {
        node = node->prev;
    }
    node = gsMemHandleLinkTail;
    while (node->next != NULL) {
        node = node->next;
    }
}

/* Mark a range free and coalesce its neighbors while interrupts are masked. */
void sdfGsMemFree(sdfGsMemSlice_t *textureBlock) {
    s32 restoreInterrupts;

    if (textureBlock == NULL) {
        return;
    }
    restoreInterrupts = DIntr();
    textureBlock->allocationMode = 0;
    sdfGsMemCheckMerge(textureBlock);
    if (textureBlock->next != NULL) {
        if (textureBlock->next->allocationMode == 0) {
            sdfGsMemCheckMerge(textureBlock->next);
        }
    }
    if (restoreInterrupts != 0) {
        EIntr();
    }
}

void sdfGsMemFreeDelayed(sdfGsMemSlice_t *texture) {
    sdfDelayedResrcEntry(&gsMemDelayedResrcCB, texture);
}

void sdfGsMemInit(void) {
    sdfGsMemSlice_t *head;

    head = sdfChipAllocDirect(sizeof(sdfGsMemSlice_t));
    head->size = 0x100000;
    head->next = NULL;
    head->prev = NULL;
    head->allocationMode = 0;
    head->word = 0;
    gsMemHandleLinkHead = head;
    gsMemHandleLinkTail = head;
    sdfDelayedResrcInitCB(&gsMemDelayedResrcCB, (SdfPendingEntryCallback)sdfGsMemFree);
}

/* Align the storage rows, pack 16-bit formats two pixels per word, then allocate whole GS pages. */
sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format) {
    s32 rowWidth = (width + 0x3F) & -0x40;
    s32 rowCount = (height + 0x1F) & -0x20;
    sdfGsMemSlice_t *textureBlock;

    switch (format) {
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
    textureBlock = sdfGsMemAllocFromHead((rowWidth * rowCount + 0x7FF) & -0x800, 1);
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

sdfGsMemSlice_t *sdfGsMemGetLinkHead(void) {
    return gsMemHandleLinkHead;
}

sdfGsMemSlice_t *sdfGsMemGetLinkTail(void) {
    return gsMemHandleLinkTail;
}

INCLUDE_SDATA(const s32, "sdf/sdfGsMemory", sdfGsErrorHandler);

