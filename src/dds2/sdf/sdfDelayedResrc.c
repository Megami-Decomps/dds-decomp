#include "sdk_ps2.h"
#include "ee_mmi.h"
#include "sdf_scene_packet.h"
#include "sdf_gs_gouraud_textured.h"
#include "sdf_gs_scene_state.h"
#include "sdf_gs_textured_shapes.h"
#include "sdf_request.h"
#include "sdf_gs_context.h"
#include "sdf_gs_gouraud_shapes.h"
#include "sdf_gs_header.h"
#include "sdf_gs_geometry.h"
#include "sdf_texture_flush.h"
#include "sdf_tex_lifetime.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "sdf_delayed_resource_check_api.h"
#include "sdf_vu0.h"
#include "sdf_timer.h"
#include "sdf_packet_list.h"
#include "sdf_packet_slot.h"
#include "sdf_draw_frame_execution_state.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_texture_registry.h"
#include "sdf_image_packets.h"
#include "sdf_pending.h"
#include "sdf_queued_resource_api.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "sdf_gs_flat_shapes.h"
#include "sdf_gs_packet.h"
#include "sdf_draw.h"
#include "sdf_model.h"
#include "sdf_dev_slot_api.h"
#include "sdf_dma_tag.h"
#include "sdf_packet_alloc.h"

#define SDF_PENDING_SLOT_COUNT 2

#define SDF_PENDING_LAST_SLOT 1

extern s32 sdfDelayedResrcSemaId;

extern u8 sdfDelayedResrcCheckFunc(void);

extern SdfPendingNode *sdfDelayedResrcLink;

extern s8 sdfDelayedResrcFlag;

extern SdfPendingNode *sdfPendingQueueSlots[2];

/* Lazily create the shared semaphore and reset this request's state. */
void sdfDelayedResrcInitCB(SdfPendingRequest *request, SdfPendingEntryCallback callback) {
    if (sdfDelayedResrcSemaId < 0) {
        sdfDelayedResrcSemaId = sdfCreateSema(1, 0x7f, 0);
    }
    request->handler = (SdfPendingCallback)callback;
    request->pending = NULL;
}

/* Queue an entry on the owner's pending node, adding a node or buffer chunk as needed. */
void sdfDelayedResrcEntry(SdfPendingRequest *owner, void *entry) {
    SdfPendingNode *pendingNode;
    SdfPendingBuffer *entryBuffer;
    SdfPendingBufferFreeEntryCount freeEntries;

    if (entry != NULL) {
        WaitSema(sdfDelayedResrcSemaId);
        pendingNode = owner->pending;
        if (pendingNode == NULL) {
            pendingNode = (SdfPendingNode *)sdfChipAllocDirect(SDF_PENDING_NODE_BYTES);
            pendingNode->freeEntryCount = 0;
            pendingNode->next = sdfDelayedResrcLink;
            pendingNode->buffer = NULL;
            pendingNode->owner = owner;
            owner->pending = pendingNode;
            sdfDelayedResrcLink = pendingNode;
        }
        freeEntries = pendingNode->freeEntryCount;
        entryBuffer = pendingNode->buffer;
        if (freeEntries == 0) {
            SdfPendingBuffer *newBuffer = (SdfPendingBuffer *)sdfChipAllocDirect(SDF_PENDING_BUFFER_BYTES);
            newBuffer->next = entryBuffer;
            pendingNode->buffer = newBuffer;
            entryBuffer = newBuffer;
            freeEntries = SDF_PENDING_BUFFER_CAPACITY;
        }
        entryBuffer->entry[SDF_PENDING_BUFFER_CAPACITY - freeEntries] = entry;
        pendingNode->freeEntryCount = freeEntries - 1;
        SignalSema(sdfDelayedResrcSemaId);
    }
}

/* Detach the pending chain and clear each callback owner's pending node. */
SdfPendingNode *sdfDelayedResrcClose(void) {
    SdfPendingNode *head = sdfDelayedResrcLink;
    SdfPendingNode *link;

    sdfDelayedResrcLink = NULL;
    link = head;
    if (head != NULL) {
        do {
            link->owner->pending = NULL;
            link = link->next;
        } while (link != NULL);
    }
    return head;
}

/* Flush the newest buffer's populated entries first, then older full buffers.
 * Invoke the owner's handler in entry order within each buffer before freeing it. */
void sdfDelayedResrcRelease(SdfPendingNode *pendingNode) {
    SdfPendingNode *nextPendingNode;
    SdfPendingBuffer *entryBuffer;
    SdfPendingBuffer *nextEntryBuffer;
    SdfPendingCallback entryHandler;
    s32 entryCount;
    s32 entryIndex;

    if (pendingNode != NULL) {
        do {
            entryCount = SDF_PENDING_BUFFER_CAPACITY - pendingNode->freeEntryCount;
            entryHandler = pendingNode->owner->handler;
            entryBuffer = pendingNode->buffer;
            if (entryBuffer != NULL) {
                do {
                    entryIndex = 0;
                    do {
                        entryHandler(entryBuffer->entry[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < entryCount);
                    nextEntryBuffer = entryBuffer->next;
                    sdfChipFreeDirect(entryBuffer);
                    entryBuffer = nextEntryBuffer;
                    entryCount = SDF_PENDING_BUFFER_CAPACITY;
                } while (entryBuffer != NULL);
            }
            nextPendingNode = pendingNode->next;
            sdfChipFreeDirect(pendingNode);
            pendingNode = nextPendingNode;
        } while (pendingNode != NULL);
    }
}

/* Hand the oldest pending slot to the worker, shift the slot list down and
 * refill the last slot with the newly detached list. */
void sdfDelayedResrcProc(void) {
    u32 slotIndex;

    sdfDelayedResrcFlag = 1;
    WaitSema(sdfDelayedResrcSemaId);
    sdfDelayedResrcRelease(sdfPendingQueueSlots[0]);
    for (slotIndex = 0; slotIndex < SDF_PENDING_LAST_SLOT; slotIndex++) {
        sdfPendingQueueSlots[slotIndex] = sdfPendingQueueSlots[slotIndex + 1];
    }
    sdfPendingQueueSlots[SDF_PENDING_LAST_SLOT] = sdfDelayedResrcClose();
    SignalSema(sdfDelayedResrcSemaId);
    sdfDelayedResrcFlag = 0;
}

/* Test all three pending-work sources: flag, chain, and two slots. */
u8 sdfDelayedResrcCheckFunc(void) {
    u32 slotIndex;
    SdfPendingNode **slotCursor;
    if (sdfDelayedResrcFlag != 0) {
        return 1;
    }
    if (sdfDelayedResrcLink != NULL) {
        return 1;
    }
    slotIndex = 0;
    slotCursor = sdfPendingQueueSlots;
    do {
        if (*slotCursor != NULL) {
            return 1;
        }
        slotCursor++;
        slotIndex++;
    } while (slotIndex < SDF_PENDING_SLOT_COUNT);
    return 0;
}

/* Preserve interrupt state while querying pending work. */
SdfDelayedResourceCheckResult sdfDelayedResrcCheck(void) {
    s32 interruptState;
    u8 hasPendingWork;

    interruptState = DIntr();
    hasPendingWork = sdfDelayedResrcCheckFunc();
    if (interruptState != 0) {
        EIntr();
    }
    return hasPendingWork;
}

INCLUDE_SDATA(const s32, "sdf/sdfDelayedResrc", sdfDelayedResrcSemaId);

