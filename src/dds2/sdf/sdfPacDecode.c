#include "common.h"
#include "sdf.h"

enum {
    PAC_COMMAND_PAYLOAD = 1,
    PAC_COMMAND_ALLOCATION_LIST = 2,
    PAC_COMMAND_END = 0xFF,
    PAC_ENCODING_RAW = 0,
    PAC_ENCODING_COMPRESSED = 1,
    PAC_HEADER_BYTES = 0x10,
    PAC_WORK_BASE_BYTES = 0x20,
    PAC_EXTENSION_BYTES_MASK = 0xF0,
    PAC_ENCODING_MASK = 0xF,
    PAC_STATE_USE_PACKET_MEMORY = 1,
    PAC_STATE_ALLOCATE_HIGH = 2
};

typedef struct PacHead {
    u8 command; /* 0x0 */
    u8 flags; /* 0x1: high nibble extension length, low nibble encoding */
    u8 pad2[2]; /* 0x2 */
    s32 payloadSize; /* 0x4 */
    u8 pad8[4]; /* 0x8 */
    s32 decodedSize; /* 0xC */
    u8 payload[0]; /* 0x10: variable-length packet data */
} PacHead;

typedef struct PacWork {
    struct PacWork *next; /* 0x0 */
    struct PacState *owner; /* 0x4 */
    s32 resourceHandle; /* 0x8 */
    u8 *dataCursor; /* 0xC */
    u8 packet[1]; /* 0x10: copied header and packet data */
} PacWork;

typedef struct PacBuf {
    s32 result; /* 0x0: decoded result word, including completed resource addresses */
    s32 resourceSlot; /* 0x4 */
    u8 *cursor; /* 0x8 */
    s32 remainingBytes; /* 0xC */
} PacBuf;

typedef struct PacAlloc {
    s32 entryCount; /* 0x0 */
    s32 entryIndex; /* 0x4 */
    u8 pad8[8]; /* 0x8 */
    PacHead entry; /* 0x10: current serialized entry header */
    PacBuf buffer; /* 0x20: decoder state; result is the completed resource */
} PacAlloc;


/* Event 0 supplies a payload-size word; event 1 omits it. Keep the
 * packet callback's native short-arity interface unprototyped. */
typedef struct PacState {
    u8 phase; /* 0x0 */
    u8 flags; /* 0x1 */
    u16 packetCounter; /* 0x2: wraps from the initial 0xFFFF */
    s32 (*packetCallback)(); /* 0x4 */
    void (*onInput)(struct PacState *); /* 0x8 */
    void (*onComplete)(struct PacState *); /* 0xC */
    u8 *inputCursor; /* 0x10 */
    s32 inputAvailable; /* 0x14 */
    s32 consumedBytes; /* 0x18 */
    u8 *outputCursor; /* 0x1C */
    s32 pendingBytes; /* 0x20 */
    PacBuf *decoder; /* 0x24 */
    PacBuf *resourceBuffer; /* 0x28 */
    PacAlloc *allocation; /* 0x2C: current allocation-entry list */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;

PacWork *sdfPacEnqueuePacket(PacState *state, PacHead *packet);

void *sdfAllocAndClearQuadwords(s32 size);

extern void *memcpy(void *dst, const void *src, u32 n);

void sdfReleaseChipBlock(void *allocation);

void sdfPacStartRegularPacket(PacState *state, PacHead *packet);

void sdfPacStartRelocatingPacket(PacState *state, PacHead *packet);

void sdfPacBeginRelocatedPayload(PacState *state, PacHead *packet);

void sdfPacStartAllocationList(PacState *state, PacHead *packet);

void sdfQueueAndResetPacketWork(PacState *state, void *packet);

void sdfPacAdvanceInput(PacState *state, s32 consumedBytes);

s32 func_00347988(void *decoder, void *input, s32 available);

void sdfPacStartPacketPayload(PacState *state, PacHead *packet);

void sdfDecodePacNodeAndAdvanceTail(PacState *state);

void sdfPacRelocateQueuedPayload(PacState *state);

void sdfPacFinalizeRelocatedPayload(PacState *state);

s32 sdfResourceRetainAddress(s32 handle);

SdfTex *sdfTexAcquireResourceTexture(void *resource);

void sdfReleaseMemorySlot(void *slot);

SdfTex *sdfTexAcquireAlternateResourceTexture(void *resource);

void *sdfAllocSizeClassBlock(s32 size);

void func_003475A0(PacState *state, PacHead *packet, PacBuf *buffer);

void func_003476D0(PacState *state);

void sdfPacAdvanceAllocationEntry(PacState *state);

void sdfPacResetOutputToAllocationEntry(PacState *state);

s32 sdfAllocGeneralBlockHigh(s32 size);

s32 sdfAllocGeneralBlock(s32 size);

void sdfStoreWordAndSetState(void *decoder, void *destination);

/* Relocation record embedded in the work item's data stream. */
typedef struct PacReloc {
    s32 tableOffset; /* 0x00: relocation table displacement from the payload */
    s32 tableBytes;  /* 0x04: zero after the table has been applied */
    u8 pad08[8];
    u8 payload[1]; /* 0x10 */
} PacReloc;

void sdfRelocatePackedResourceWords(void *words, void *base, void *table, s32 size);

void sdfAppendResourceListItem(s32 handle, s32 resource);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00346CF0);

/* Queue a private copy of the packet header and any extension bytes. */
PacWork *sdfPacEnqueuePacket(PacState *state, PacHead *packet) {
    s32 extensionBytes = packet->flags & PAC_EXTENSION_BYTES_MASK;
    PacWork *node = sdfAllocAndClearQuadwords(extensionBytes + PAC_WORK_BASE_BYTES);
    node->owner = state;
    memcpy(node->packet, packet, extensionBytes + PAC_HEADER_BYTES);
    if (state->queueTail == NULL) {
        state->queueHead = node;
    } else {
        state->queueTail->next = node;
    }
    state->queueTail = node;
    return node;
}

/* Remove an item already in its owner's queue, free it, and return its successor. */
PacWork *sdfPacRemovePacket(PacWork *work) {
    PacState *state = work->owner;
    /* Keep a node-shaped link so the queue head can be unlinked like any next pointer. */
    PacWork *unlinkLink = (PacWork *)&state->queueHead;
    PacWork *current = state->queueHead;
    PacWork *previous = NULL;
    PacWork *nextWork;
    if (current != work) {
        do {
            previous = current;
            current = previous->next;
            unlinkLink = previous;
        } while (current != work);
    }
    nextWork = work->next;
    unlinkLink->next = nextWork;
    if (state->queueTail == work) {
        state->queueTail = previous;
    }
    sdfReleaseChipBlock(work);
    return nextWork;
}

/* Dispatch recognized PAC commands; one marks end-of-stream. */
s32 sdfPacDispatchPacket(PacState *state, s32 status, PacHead *packet) {
    if (status == 0) {
        switch (packet->command) {
        case PAC_COMMAND_PAYLOAD:
            sdfPacStartRegularPacket(state, packet);
            return 0;
        case PAC_COMMAND_ALLOCATION_LIST:
            sdfPacStartAllocationList(state, packet);
            return 0;
        case 6:
            sdfPacStartRelocatingPacket(state, packet);
            return 0;
        case 8:
            sdfPacBeginRelocatedPayload(state, packet);
            return 0;
        case 9:
            sdfQueueAndResetPacketWork(state, packet);
            return 0;
        case PAC_COMMAND_END:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}

/* Return the second header's payload when its extension-length nibble is nonzero. */
void *sdfPacGetExtensionData(PacHead *header) {
    PacHead *extension = (PacHead *)header->payload;
    s32 extensionBytes = extension->flags & PAC_EXTENSION_BYTES_MASK;
    if (extensionBytes <= 0) {
        return NULL;
    }
    return extension->payload;
}

/* Incrementally copy raw payload bytes, invoking completion at zero remaining. */
void sdfPacCopyPendingBytes(PacState *state) {
    s32 copyBytes = state->pendingBytes;
    s32 inputBytes = state->inputAvailable;
    if (inputBytes < copyBytes) {
        copyBytes = inputBytes;
    }
    /* Completion is tested only after consuming a nonzero chunk. */
    if (copyBytes != 0) {
        memcpy(state->outputCursor, state->inputCursor, copyBytes);
        sdfPacAdvanceInput(state, copyBytes);
        state->outputCursor += copyBytes;
        {
            s32 bytesRemaining = state->pendingBytes - copyBytes;
            state->pendingBytes = bytesRemaining;
            if (bytesRemaining != 0) {
                return;
            }
        }
        state->onComplete(state);
    }
}

/* Feed compressed input to the active decoder until it finishes. */
void sdfPacDecodePendingBytes(PacState *state) {
    s32 inputBytes = state->inputAvailable;
    s32 finished = func_00347988(state->decoder, state->inputCursor, inputBytes);
    sdfPacAdvanceInput(state, inputBytes - state->decoder->remainingBytes);
    if (finished == 0) {
        return;
    }
    sdfReleaseChipBlock(state->decoder);
    state->onComplete(state);
}

/* Consume a packet's bytes without allocating its decoded payload. */
void sdfPacSkipPendingBytes(PacState *state) {
    s32 skipBytes = state->pendingBytes;
    if (state->inputAvailable < skipBytes) {
        skipBytes = state->inputAvailable;
    }
    if (skipBytes != 0) {
        sdfPacAdvanceInput(state, skipBytes);
        {
            s32 bytesRemaining = state->pendingBytes - skipBytes;
            state->pendingBytes = bytesRemaining;
            if (bytesRemaining != 0) {
                return;
            }
        }
        state->onComplete(state);
    }
}

/* Allocate or skip a payload and select its raw/compressed input handler. */
void sdfPacStartPacketPayload(PacState *state, PacHead *packet) {
    s32 allocationSize = packet->decodedSize;
    if (allocationSize == 0) {
        allocationSize = packet->payloadSize + (packet->flags & PAC_EXTENSION_BYTES_MASK) - PAC_HEADER_BYTES;
    }
    if (state->flags & PAC_STATE_USE_PACKET_MEMORY) {
        PacWork *node = sdfPacEnqueuePacket(state, packet);
        node->dataCursor = packet->payload;
        state->onInput = sdfPacSkipPendingBytes;
    } else {
        PacWork *node;
        state->phase = 2;
        node = sdfPacEnqueuePacket(state, packet);
        if (state->flags & PAC_STATE_ALLOCATE_HIGH) {
            node->resourceHandle = sdfAllocGeneralBlockHigh(allocationSize);
        } else {
            node->resourceHandle = sdfAllocGeneralBlock(allocationSize);
        }
        state->outputCursor = node->dataCursor = (u8 *)sdfResourceRetainAddress(node->resourceHandle);
        switch (packet->flags & PAC_ENCODING_MASK) {
        case PAC_ENCODING_RAW:
            state->onInput = sdfPacCopyPendingBytes;
            break;
        case PAC_ENCODING_COMPRESSED: {
            PacBuf *decoder = sdfAllocSizeClassBlock(0x20);
            state->decoder = decoder;
            sdfStoreWordAndSetState(decoder, state->outputCursor);
            state->onInput = sdfPacDecodePendingBytes;
            break;
        }
        default:
            break;
        }
    }
}

/* Begin a regular payload; completion is the normal packet finalizer. */
void sdfPacStartRegularPacket(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = sdfDecodePacNodeAndAdvanceTail;
}

/* Apply the relocation record at the front of the queued payload. */
void sdfPacRelocateQueuedPayload(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 tableBytes = record->tableBytes;
    work->dataCursor = payload;
    if (tableBytes != 0) {
        sdfRelocatePackedResourceWords(payload, payload, payload + record->tableOffset, tableBytes);
        record->tableBytes = 0;
    }
    sdfDecodePacNodeAndAdvanceTail(state);
}

/* Begin a payload that must be relocated before normal finalization. */
void sdfPacStartRelocatingPacket(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = sdfPacRelocateQueuedPayload;
}

/* Complete the second relocation-command variant with the same word fixups. */
void sdfPacFinalizeRelocatedPayload(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 tableBytes = record->tableBytes;
    work->dataCursor = payload;
    if (tableBytes != 0) {
        sdfRelocatePackedResourceWords(payload, payload, payload + record->tableOffset, tableBytes);
        record->tableBytes = 0;
    }
    sdfDecodePacNodeAndAdvanceTail(state);
}

/* Begin the second relocation-command variant. */
void sdfPacBeginRelocatedPayload(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = sdfPacFinalizeRelocatedPayload;
}

/* Incrementally copy a resource chunk before processing its resource slot. */
void sdfPacCopyResourceChunk(PacState *state) {
    PacBuf *resourceBuffer = state->resourceBuffer;
    s32 copyBytes = resourceBuffer->remainingBytes;
    if (state->inputAvailable < copyBytes) {
        copyBytes = state->inputAvailable;
    }
    if (copyBytes != 0) {
        memcpy(resourceBuffer->cursor, state->inputCursor, copyBytes);
        sdfPacAdvanceInput(state, copyBytes);
        resourceBuffer->cursor += copyBytes;
        {
            s32 bytesRemaining = resourceBuffer->remainingBytes - copyBytes;
            resourceBuffer->remainingBytes = bytesRemaining;
            if (bytesRemaining != 0) {
                return;
            }
        }
        resourceBuffer->result = (s32)sdfTexAcquireResourceTexture((void *)sdfResourceRetainAddress(resourceBuffer->resourceSlot));
        sdfReleaseMemorySlot(&resourceBuffer->resourceSlot);
        state->onComplete(state);
    }
}

/* Decode a chunk before processing and releasing its resource slot. */
void sdfPacDecodeResourceChunk(PacState *state) {
    /* This handler offers pendingBytes, unlike the packet-level decoder. */
    s32 inputBytes = state->pendingBytes;
    s32 finished = func_00347988(state->decoder, state->inputCursor, inputBytes);
    sdfPacAdvanceInput(state, inputBytes - state->decoder->remainingBytes);
    if (finished == 0) {
        return;
    }
    {
        PacBuf *resourceBuffer = state->resourceBuffer;
        resourceBuffer->result = (s32)sdfTexAcquireResourceTexture((void *)sdfResourceRetainAddress(resourceBuffer->resourceSlot));
        sdfReleaseMemorySlot(&resourceBuffer->resourceSlot);
    }
    sdfReleaseChipBlock(state->decoder);
    state->onComplete(state);
}

/* Skip the remaining resource chunk and process its existing cursor. */
void sdfPacSkipResourceChunk(PacState *state) {
    PacBuf *resourceBuffer = state->resourceBuffer;
    s32 skipBytes = resourceBuffer->remainingBytes;
    if (state->inputAvailable < skipBytes) {
        skipBytes = state->inputAvailable;
    }
    if (skipBytes != 0) {
        sdfPacAdvanceInput(state, skipBytes);
        {
            s32 bytesRemaining = resourceBuffer->remainingBytes - skipBytes;
            resourceBuffer->remainingBytes = bytesRemaining;
            if (bytesRemaining != 0) {
                return;
            }
        }
        resourceBuffer->result = (s32)sdfTexAcquireAlternateResourceTexture(resourceBuffer->cursor);
        state->onComplete(state);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003475A0);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003476D0);

/* Allocate per-packet state for a list of allocation entries. */
void sdfPacStartAllocationList(PacState *state, PacHead *packet) {
    sdfPacEnqueuePacket(state, packet);
    {
        void *allocation = sdfAllocSizeClassBlock(0x10);
        state->allocation = (PacAlloc *)allocation;
        func_003475A0(state, packet, allocation);
    }
    state->onComplete = func_003476D0;
}

/* Start the next allocation entry at its inline descriptor. */
void sdfPacStartNextAllocationEntry(PacState *state) {
    func_003475A0(state, &state->allocation->entry, &state->allocation->buffer);
    state->onComplete = sdfPacAdvanceAllocationEntry;
}

/* Reset the output cursor to the current allocation entry and hook the copy
   and completion callbacks. */
void sdfPacResetOutputToAllocationEntry(PacState *state) {
    state->outputCursor = (u8 *)&state->allocation->entry;
    state->pendingBytes = 0x10;
    state->onInput = sdfPacCopyPendingBytes;
    state->onComplete = sdfPacStartNextAllocationEntry;
}

/* Advance the entry index and complete or request the next entry. */
void sdfPacAdvanceAllocationEntry(PacState *state) {
    PacAlloc *allocation = state->allocation;
    sdfAppendResourceListItem(state->queueTail->resourceHandle, allocation->buffer.result);
    {
        s32 nextIndex = allocation->entryIndex + 1;
        allocation->entryIndex = nextIndex;
        if (nextIndex == allocation->entryCount) {
            sdfReleaseChipBlock(allocation);
            sdfDecodePacNodeAndAdvanceTail(state);
        } else {
            sdfPacResetOutputToAllocationEntry(state);
        }
    }
}

/* Discard pending input until the next allocation entry can start; check the
   boundary even when no bytes are consumed. */
void sdfPacSkipAllocationEntryBytes(PacState *state) {
    s32 skipBytes = state->inputAvailable;
    if (state->pendingBytes < skipBytes) {
        skipBytes = state->pendingBytes;
    }
    sdfPacAdvanceInput(state, skipBytes);
    {
        s32 bytesRemaining = state->pendingBytes - skipBytes;
        state->pendingBytes = bytesRemaining;
        if (bytesRemaining != 0) {
            return;
        }
    }
    sdfPacResetOutputToAllocationEntry(state);
}
