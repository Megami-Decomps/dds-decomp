#include "common.h"

enum {
    PAC_COMMAND_PAYLOAD = 1,
    PAC_COMMAND_ALLOCATION_LIST = 2,
    PAC_COMMAND_END = 0xFF,
    PAC_ENCODING_RAW = 0,
    PAC_ENCODING_COMPRESSED = 1
};

typedef struct PacHead {
    u8 command; /* 0x0 */
    u8 flags; /* 0x1: high nibble extension length, low nibble encoding */
    u8 pad2[2]; /* 0x2 */
    s32 payloadSize; /* 0x4 */
    u8 pad8[4]; /* 0x8 */
    s32 decodedSize; /* 0xC */
    u8 payload[1]; /* 0x10: variable-length packet data */
} PacHead;

/* Relocation record embedded in the work item's data stream. */
typedef struct PacReloc {
    s32 offset; /* 0x00: displacement from the payload */
    s32 count;  /* 0x04: number of relocation bytes */
    u8 pad08[8];
    u8 payload[1]; /* 0x10 */
} PacReloc;

/* Extended PAC header: the second header's flags precede data at 0x20. */
typedef struct PacExtensionHeader {
    u8 pad00[0x11];
    u8 extensionFlags; /* 0x11 */
    u8 pad12[0xE];
    u8 data[1];        /* 0x20 */
} PacExtensionHeader;

typedef struct PacWork {
    struct PacWork *next; /* 0x0 */
    struct PacState *owner; /* 0x4 */
    s32 resourceHandle; /* 0x8 */
    u8 *dataCursor; /* 0xC */
    u8 packet[1]; /* 0x10: copied header and packet data */
} PacWork;

typedef struct PacAlloc {
    s32 entryCount; /* 0x0 */
    s32 entryIndex; /* 0x4 */
    u8 pad8[24]; /* 0x8 */
    s32 unk20; /* 0x20 */
} PacAlloc;

typedef struct PacBuf {
    s32 result; /* 0x0 */
    s32 resourceSlot; /* 0x4 */
    u8 *cursor; /* 0x8 */
    s32 remaining; /* 0xC */
} PacBuf;

typedef struct PacState {
    u8 phase; /* 0x0 */
    u8 flags; /* 0x1 */
    u8 pad2[2]; /* 0x2 */
    s32 unk4; /* 0x4 */
    void (*onInput)(struct PacState *); /* 0x8 */
    void (*onComplete)(struct PacState *); /* 0xC */
    u8 *inputCursor; /* 0x10 */
    s32 inputAvailable; /* 0x14 */
    s32 unk18; /* 0x18 */
    u8 *outputCursor; /* 0x1C */
    s32 pendingBytes; /* 0x20 */
    PacBuf *decoder; /* 0x24 */
    PacBuf *buffer; /* 0x28 */
    PacAlloc *allocation; /* 0x2C: current allocation-entry list */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;

void sdfPacStartPacketPayload(PacState *arg0, PacHead *arg1);
void func_002EDD98(PacState *arg0);
void sdfRelocatePackedResourceWords(void *arg0, void *arg1, void *arg2, s32 arg3);
void func_002CFF98(void *arg0);
void sdfPacAdvanceInput(PacState *arg0, s32 arg1);
void func_002EDCC0(PacState *arg0);
PacWork *sdfPacEnqueuePacket(PacState *arg0, PacHead *arg1);
void *func_002CFF68(s32 size);
void *func_002CFEB8(s32 size);
void func_002EE6F8(PacState *arg0, PacHead *arg1, PacBuf *arg2);
void func_002EE418(PacState *arg0);
void func_002EE4A8(PacState *arg0);
void func_002EE828(PacState *arg0);
void func_002EE900(PacState *arg0);
void sdfPacAdvanceAllocationEntry(PacState *arg0);
void sdfPacStartRegularPacket(PacState *arg0, PacHead *arg1);
void func_002EE478(PacState *arg0, PacHead *arg1);
void func_002EE508(PacState *arg0, PacHead *arg1);
void sdfPacStartAllocationList(PacState *arg0, PacHead *arg1);
void func_002EEAA0(PacState *arg0, void *arg1);
void func_002DA058(s32 arg0, s32 arg1);
s32 func_002D32A0(void *arg0);
s32 sdfResourceRetainAddress(s32 arg0);
s32 func_002D3288(s32 arg0);
s32 func_002D0518(s32 arg0);
s32 func_002D03F8(s32 arg0);
void sdfReleaseMemorySlot(void *arg0);
void sdfStoreWordAndSetState(void *arg0, void *arg1);
s32 func_002EEAE0(void *arg0, void *arg1, s32 arg2);
extern void *memcpy(void *dst, const void *src, u32 n);


INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EDE48);

/* Queue a private copy of the packet header and any extension bytes. */
PacWork *sdfPacEnqueuePacket(PacState *state, PacHead *packet) {
    s32 extensionBytes = packet->flags & 0xF0;
    PacWork *node = func_002CFF68(extensionBytes + 0x20);
    node->owner = state;
    memcpy(node->packet, packet, extensionBytes + 0x10);
    if (state->queueTail == NULL) {
        state->queueHead = node;
    } else {
        state->queueTail->next = node;
    }
    state->queueTail = node;
    return node;
}

/* Unlink a queued packet and return the following work item. */
PacWork *sdfPacRemovePacket(PacWork *work) {
    PacState *state = work->owner;
    /* Keep a node-shaped link so the queue head can be unlinked like any next pointer. */
    PacWork *link = (PacWork *)&state->queueHead;
    PacWork *cur = state->queueHead;
    PacWork *prev = NULL;
    PacWork *next;
    if (cur != work) {
        do {
            prev = cur;
            cur = prev->next;
            link = prev;
        } while (cur != work);
    }
    next = work->next;
    link->next = next;
    if (state->queueTail == work) {
        state->queueTail = prev;
    }
    func_002CFF98(work);
    return next;
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
            func_002EE478(state, packet);
            return 0;
        case 8:
            func_002EE508(state, packet);
            return 0;
        case 9:
            func_002EEAA0(state, packet);
            return 0;
        case PAC_COMMAND_END:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}
/* Return the extension payload only when its high-nibble length is nonzero. */
void *sdfPacGetExtensionData(PacExtensionHeader *header) {
    s32 extensionSize = header->extensionFlags & 0xF0;
    if (extensionSize <= 0) {
        return NULL;
    }
    return header->data;
}

/* Incrementally copy raw payload bytes, invoking completion at zero remaining. */
void sdfPacCopyPendingBytes(PacState *state) {
    s32 count = state->pendingBytes;
    s32 available = state->inputAvailable;
    if (available < count) {
        count = available;
    }
    if (count != 0) {
        memcpy(state->outputCursor, state->inputCursor, count);
        sdfPacAdvanceInput(state, count);
        state->outputCursor += count;
        {
            s32 remaining = state->pendingBytes - count;
            state->pendingBytes = remaining;
            if (remaining != 0) {
                return;
            }
        }
        state->onComplete(state);
    }
}

/* Feed compressed input to the active decoder until it finishes. */
void sdfPacDecodePendingBytes(PacState *state) {
    s32 available = state->inputAvailable;
    s32 finished = func_002EEAE0(state->decoder, state->inputCursor, available);
    sdfPacAdvanceInput(state, available - state->decoder->remaining);
    if (finished == 0) {
        return;
    }
    func_002CFF98(state->decoder);
    state->onComplete(state);
}

/* Consume a packet's bytes without allocating its decoded payload. */
void sdfPacSkipPendingBytes(PacState *state) {
    s32 count = state->pendingBytes;
    if (state->inputAvailable < count) {
        count = state->inputAvailable;
    }
    if (count != 0) {
        sdfPacAdvanceInput(state, count);
        {
            s32 remaining = state->pendingBytes - count;
            state->pendingBytes = remaining;
            if (remaining != 0) {
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
        allocationSize = packet->payloadSize + (packet->flags & 0xF0) - 0x10;
    }
    if (state->flags & 1) {
        PacWork *node = sdfPacEnqueuePacket(state, packet);
        node->dataCursor = packet->payload;
        state->onInput = sdfPacSkipPendingBytes;
    } else {
        PacWork *node;
        state->phase = 2;
        node = sdfPacEnqueuePacket(state, packet);
        if (state->flags & 2) {
            node->resourceHandle = func_002D0518(allocationSize);
        } else {
            node->resourceHandle = func_002D03F8(allocationSize);
        }
        state->outputCursor = node->dataCursor = (u8 *)sdfResourceRetainAddress(node->resourceHandle);
        switch (packet->flags & 0xF) {
        case PAC_ENCODING_RAW:
            state->onInput = sdfPacCopyPendingBytes;
            break;
        case PAC_ENCODING_COMPRESSED: {
            PacBuf *decoder = func_002CFEB8(0x20);
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
    state->onComplete = func_002EDD98;
}

/* Apply the relocation record at the front of the queued payload. */
void func_002EE418(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 count = record->count;
    work->dataCursor = payload;
    if (count != 0) {
        sdfRelocatePackedResourceWords(payload, payload, payload + record->offset, count);
        record->count = 0;
    }
    func_002EDD98(state);
}

/* Begin a payload that must be relocated before normal finalization. */
void func_002EE478(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = func_002EE418;
}

/* Complete the second relocation-command variant with the same word fixups. */
void func_002EE4A8(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 count = record->count;
    work->dataCursor = payload;
    if (count != 0) {
        sdfRelocatePackedResourceWords(payload, payload, payload + record->offset, count);
        record->count = 0;
    }
    func_002EDD98(state);
}

/* Begin the second relocation-command variant. */
void func_002EE508(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = func_002EE4A8;
}

/* Incrementally copy a resource chunk before processing its resource slot. */
void sdfPacCopyResourceChunk(PacState *state) {
    PacBuf *buffer = state->buffer;
    s32 count = buffer->remaining;
    if (state->inputAvailable < count) {
        count = state->inputAvailable;
    }
    if (count != 0) {
        memcpy(buffer->cursor, state->inputCursor, count);
        sdfPacAdvanceInput(state, count);
        buffer->cursor += count;
        {
            s32 remaining = buffer->remaining - count;
            buffer->remaining = remaining;
            if (remaining != 0) {
                return;
            }
        }
        buffer->result = func_002D3288(sdfResourceRetainAddress(buffer->resourceSlot));
        sdfReleaseMemorySlot(&buffer->resourceSlot);
        state->onComplete(state);
    }
}

/* Decode a chunk before processing and releasing its resource slot. */
void sdfPacDecodeResourceChunk(PacState *state) {
    s32 count = state->pendingBytes;
    s32 finished = func_002EEAE0(state->decoder, state->inputCursor, count);
    sdfPacAdvanceInput(state, count - state->decoder->remaining);
    if (finished == 0) {
        return;
    }
    {
        PacBuf *buffer = state->buffer;
        buffer->result = func_002D3288(sdfResourceRetainAddress(buffer->resourceSlot));
        sdfReleaseMemorySlot(&buffer->resourceSlot);
    }
    func_002CFF98(state->decoder);
    state->onComplete(state);
}

/* Skip the remaining resource chunk and process its existing cursor. */
void sdfPacSkipResourceChunk(PacState *state) {
    PacBuf *buffer = state->buffer;
    s32 count = buffer->remaining;
    if (state->inputAvailable < count) {
        count = state->inputAvailable;
    }
    if (count != 0) {
        sdfPacAdvanceInput(state, count);
        {
            s32 remaining = buffer->remaining - count;
            buffer->remaining = remaining;
            if (remaining != 0) {
                return;
            }
        }
        buffer->result = func_002D32A0(buffer->cursor);
        state->onComplete(state);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE6F8);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_002EE828);

/* Allocate per-packet state for a list of allocation entries. */
void sdfPacStartAllocationList(PacState *state, PacHead *packet) {
    sdfPacEnqueuePacket(state, packet);
    {
        void *allocation = func_002CFEB8(0x10);
        state->allocation = (PacAlloc *)allocation;
        func_002EE6F8(state, packet, allocation);
    }
    state->onComplete = func_002EE828;
}


/* Start the next allocation entry at its inline descriptor. */
void sdfPacStartNextAllocationEntry(PacState *state) {
    func_002EE6F8(state, (u8 *)state->allocation + 0x10, (u8 *)state->allocation + 0x20);
    state->onComplete = sdfPacAdvanceAllocationEntry;
}


/* Reset the output cursor to the current allocation entry and hook the copy
   and completion callbacks. */
void func_002EE900(PacState *state) {
    state->outputCursor = (u8 *)state->allocation + 0x10;
    state->pendingBytes = 0x10;
    state->onInput = sdfPacCopyPendingBytes;
    state->onComplete = sdfPacStartNextAllocationEntry;
}

/* Advance the entry index and complete or request the next entry. */
void sdfPacAdvanceAllocationEntry(PacState *state) {
    PacAlloc *allocation = state->allocation;
    func_002DA058(state->queueTail->resourceHandle, allocation->unk20);
    {
        s32 nextIndex = allocation->entryIndex + 1;
        allocation->entryIndex = nextIndex;
        if (nextIndex == allocation->entryCount) {
            func_002CFF98(allocation);
            func_002EDD98(state);
        } else {
            func_002EE900(state);
        }
    }
}

/* Discard pending input until the next allocation entry can start. */
void sdfPacSkipAllocationEntryBytes(PacState *state) {
    s32 available = state->inputAvailable;
    if (state->pendingBytes < available) {
        available = state->pendingBytes;
    }
    sdfPacAdvanceInput(state, available);
    {
        s32 remaining = state->pendingBytes - available;
        state->pendingBytes = remaining;
        if (remaining != 0) {
            return;
        }
    }
    func_002EE900(state);
}

