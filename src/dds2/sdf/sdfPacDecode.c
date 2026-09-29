#include "common.h"

typedef struct PacHead {
    u8 command; /* 0x0 */
    u8 flags; /* 0x1: high nibble extension length, low nibble encoding */
    u8 pad2[2]; /* 0x2 */
    s32 payloadSize; /* 0x4 */
    u8 pad8[4]; /* 0x8 */
    s32 decodedSize; /* 0xC */
    u8 payload[1]; /* 0x10: variable-length packet data */
} PacHead;

typedef struct PacExtensionHeader {
    u8 pad00[0x11];
    u8 extensionFlags; /* 0x11 */
    u8 pad12[0xE];
    u8 data[1]; /* 0x20 */
} PacExtensionHeader;

typedef struct PacWork {
    struct PacWork *next; /* 0x0: packet queue link */
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
    PacAlloc *unk2C; /* 0x2C */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;

PacWork *sdfPacEnqueuePacket(PacState *arg0, PacHead *arg1);

void *func_00328E18(s32 size);

extern void *memcpy(void *dst, const void *src, u32 n);

void func_00328E48(void *arg0);

void func_00347290(PacState *state, PacHead *packet);

void func_00347320(PacState *state, PacHead *packet);

void func_003473B0(PacState *state, PacHead *packet);

void func_00347710(PacState *state, PacHead *packet);

void func_00347948(PacState *arg0, void *arg1);

void sdfPacAdvanceInput(PacState *arg0, s32 arg1);

s32 func_00347988(void *arg0, void *arg1, s32 arg2);

void sdfPacStartPacketPayload(PacState *arg0, PacHead *arg1);

void func_00346C40(PacState *arg0);

void func_003472C0(PacState *arg0);

void func_00347350(PacState *arg0);

s32 sdfResourceRetainAddress(s32 arg0);

s32 func_0032C138(s32 arg0);

void sdfReleaseMemorySlot(void *arg0);

s32 func_0032C150(void *arg0);

void *func_00328D68(s32 size);

void func_003475A0(PacState *arg0, PacHead *arg1, PacBuf *arg2);

void func_003476D0(PacState *arg0);

void sdfPacAdvanceAllocationEntry(PacState *arg0);

void func_003477A8(PacState *arg0);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00346CF0);

PacWork *sdfPacEnqueuePacket(PacState *state, PacHead *packet) {
    s32 extensionBytes = packet->flags & 0xF0;
    PacWork *node = func_00328E18(extensionBytes + 0x20);
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
    func_00328E48(work);
    return next;
}

s32 sdfPacDispatchPacket(PacState *state, s32 status, PacHead *packet) {
    if (status == 0) {
        switch (packet->command) {
        case 1:
            func_00347290(state, packet);
            return 0;
        case 2:
            func_00347710(state, packet);
            return 0;
        case 6:
            func_00347320(state, packet);
            return 0;
        case 8:
            func_003473B0(state, packet);
            return 0;
        case 9:
            func_00347948(state, packet);
            return 0;
        case 0xFF:
            return 1;
        default:
            return 3;
        }
    }
    return 0;
}

void *sdfPacGetExtensionData(PacExtensionHeader *header) {
    s32 extensionSize = header->extensionFlags & 0xF0;
    if (extensionSize <= 0) {
        return NULL;
    }
    return header->data;
}

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

void sdfPacDecodePendingBytes(PacState *state) {
    s32 available = state->inputAvailable;
    s32 finished = func_00347988(state->decoder, state->inputCursor, available);
    sdfPacAdvanceInput(state, available - state->decoder->remaining);
    if (finished == 0) {
        return;
    }
    func_00328E48(state->decoder);
    state->onComplete(state);
}

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

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", sdfPacStartPacketPayload);

void func_00347290(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = func_00346C40;
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003472C0);

void func_00347320(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = func_003472C0;
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_00347350);

void func_003473B0(PacState *state, PacHead *packet) {
    sdfPacStartPacketPayload(state, packet);
    state->onComplete = func_00347350;
}

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
        buffer->result = func_0032C138(sdfResourceRetainAddress(buffer->resourceSlot));
        sdfReleaseMemorySlot(&buffer->resourceSlot);
        state->onComplete(state);
    }
}

void sdfPacDecodeResourceChunk(PacState *state) {
    s32 count = state->pendingBytes;
    s32 finished = func_00347988(state->decoder, state->inputCursor, count);
    sdfPacAdvanceInput(state, count - state->decoder->remaining);
    if (finished == 0) {
        return;
    }
    {
        PacBuf *buffer = state->buffer;
        buffer->result = func_0032C138(sdfResourceRetainAddress(buffer->resourceSlot));
        sdfReleaseMemorySlot(&buffer->resourceSlot);
    }
    func_00328E48(state->decoder);
    state->onComplete(state);
}

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
        buffer->result = func_0032C150(buffer->cursor);
        state->onComplete(state);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003475A0);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003476D0);

void func_00347710(PacState *state, PacHead *packet) {
    sdfPacEnqueuePacket(state, packet);
    {
        void *allocation = func_00328D68(0x10);
        state->unk2C = (PacAlloc *)allocation;
        func_003475A0(state, packet, allocation);
    }
    state->onComplete = func_003476D0;
}

void func_00347768(PacState *state) {
    func_003475A0(state, (u8 *)state->unk2C + 0x10, (u8 *)state->unk2C + 0x20);
    state->onComplete = sdfPacAdvanceAllocationEntry;
}

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", func_003477A8);

INCLUDE_ASM(const s32, "sdf/sdfPacDecode", sdfPacAdvanceAllocationEntry);

void func_00347850(PacState *state) {
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
    func_003477A8(state);
}
