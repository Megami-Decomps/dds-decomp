#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_pac_packet.h"
#include "sdf_pac_state.h"
#include "sdf_pac_work.h"
#include "sdf_pac_reloc.h"
#include "sdf_resource_list_item.h"
#include "sdf_texture_file.h"
#include "sdf_pac_decoder.h"

enum {
    PAC_COMMAND_PAYLOAD = 1,
    PAC_COMMAND_ALLOCATION_LIST = 2,
    PAC_COMMAND_END = 0xFF,
    PAC_ENCODING_RAW = 0,
    PAC_ENCODING_COMPRESSED = 1,
    PAC_HEADER_BYTES = 0x10,
    PAC_EXTENSION_BYTES_MASK = 0xF0,
    PAC_ENCODING_MASK = 0xF,
    PAC_STATE_USE_PACKET_MEMORY = 1,
    PAC_STATE_ALLOCATE_HIGH = 2
};



void sdfPacMemDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacModelDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacMotionDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacTmxDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacMemDecoderFuncInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfDecodePacNodeAndAdvanceTail(PacState *state);

void sdfPacModelDecoderEndFunc(PacState *state);

void sdfPacMotionDecoderEndFunc(PacState *state);






void sdfPacTmxDecoderInitFunc(PacState *state, SdfPacStreamPacketHeader *packet, PacBuf *buffer);

void sdfPacTmxDecoderEndFunc(PacState *state);

void sdfPacAdvanceAllocationEntry(PacState *state);

void sdfPacResetOutputToAllocationEntry(PacState *state);






extern void sdfPacAdvanceCallbackBoundary(PacState *state);

/* Feed one input span through packet-boundary, skip, and decoder phases. */
s32 sdfPacDecode(PacState *state, const u8 *input, s32 inputByteCount) {
    s32 consumeBytes;

    state->inputCursor = input;
    state->inputAvailable = inputByteCount;
    while (state->inputAvailable > 0) {
        switch (state->phase) {
        case -1:
            return -1;
        case 0:
            sdfPacAdvanceCallbackBoundary(state);
            break;
        case 1:
            consumeBytes = state->pendingBytes;
            if (state->inputAvailable < consumeBytes) {
                consumeBytes = state->inputAvailable;
            }
            sdfPacUseSrc(state, consumeBytes);
            state->pendingBytes -= consumeBytes;
            if (state->pendingBytes == 0) {
                state->phase = 0;
            }
            break;
        case 2:
            state->onInput(state);
            break;
        }
    }
    return state->phase;
}

/* Queue a private copy of the packet header and any extension bytes. */
PacWork *sdfPacNewResrc(PacState *state, const SdfPacStreamPacketHeader *packet) {
    s32 extensionBytes = packet->flags & PAC_EXTENSION_BYTES_MASK;
    PacWork *node = sdfAllocAndClearQuadwords(extensionBytes + sizeof(PacWork) + PAC_HEADER_BYTES);
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
PacWork *sdfPacFreeResrc(PacWork *work) {
    PacState *state = work->owner;
    /* Keep a node-shaped link so the queue head can be unlinked like any next pointer. */
    PacWork *unlinkLink = (PacWork *)&state->queueHead;
    PacWork *currentWork = state->queueHead;
    PacWork *previousWork = NULL;
    PacWork *nextWork;
    if (currentWork != work) {
        do {
            previousWork = currentWork;
            currentWork = previousWork->next;
            unlinkLink = previousWork;
        } while (currentWork != work);
    }
    nextWork = work->next;
    unlinkLink->next = nextWork;
    if (state->queueTail == work) {
        state->queueTail = previousWork;
    }
    sdfChipFreeDirect(work);
    return nextWork;
}

/* Dispatch recognized PAC commands; one marks end-of-stream. */
s32 sdfPacStdCallback(PacState *state, s32 status, SdfPacStreamPacketHeader *packet) {
    if (status == 0) {
        switch (packet->command) {
        case PAC_COMMAND_PAYLOAD:
            sdfPacMemDecoderInit(state, packet);
            return 0;
        case PAC_COMMAND_ALLOCATION_LIST:
            sdfPacTmxDecoderInit(state, packet);
            return 0;
        case 6:
            sdfPacModelDecoderInit(state, packet);
            return 0;
        case 8:
            sdfPacMotionDecoderInit(state, packet);
            return 0;
        case 9:
            sdfPacTexListDecoderInit(state, packet);
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
u8 *sdfPacGetResrcFileName(SdfPacStreamPacketHeader *header) {
    SdfPacStreamPacketHeader *extension = (SdfPacStreamPacketHeader *)header->payload;
    s32 extensionBytes = extension->flags & PAC_EXTENSION_BYTES_MASK;
    if (extensionBytes <= 0) {
        return NULL;
    }
    return extension->payload;
}

/* Incrementally copy raw payload bytes, invoking completion at zero remaining. */
void sdfPacMemDecoderFunc(PacState *state) {
    s32 copyBytes = state->pendingBytes;
    s32 inputBytes = state->inputAvailable;
    if (inputBytes < copyBytes) {
        copyBytes = inputBytes;
    }
    /* Completion is tested only after consuming a nonzero chunk. */
    if (copyBytes != 0) {
        memcpy(state->outputCursor, state->inputCursor, copyBytes);
        sdfPacUseSrc(state, copyBytes);
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
void sdfPacMemDecompDecoderFunc(PacState *state) {
    s32 inputBytes = state->inputAvailable;
    s32 finished = sdfDecomp((SdfPacDecoder *)state->decoder, state->inputCursor, inputBytes);
    sdfPacUseSrc(state, inputBytes - ((SdfPacDecoder *)state->decoder)->remainingInputBytes);
    if (finished == 0) {
        return;
    }
    sdfChipFreeDirect(state->decoder);
    state->onComplete(state);
}

/* Consume a packet's bytes without allocating its decoded payload. */
void sdfPacMemStayDecoderFunc(PacState *state) {
    s32 skipBytes = state->pendingBytes;
    if (state->inputAvailable < skipBytes) {
        skipBytes = state->inputAvailable;
    }
    if (skipBytes != 0) {
        sdfPacUseSrc(state, skipBytes);
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
void sdfPacMemDecoderFuncInit(PacState *state, SdfPacStreamPacketHeader *packet) {
    s32 allocationSize = packet->decodedSize;
    if (allocationSize == 0) {
        allocationSize = packet->payloadSize + (packet->flags & PAC_EXTENSION_BYTES_MASK) - PAC_HEADER_BYTES;
    }
    if (state->flags & PAC_STATE_USE_PACKET_MEMORY) {
        PacWork *node = sdfPacNewResrc(state, packet);
        node->dataCursor = packet->payload;
        state->onInput = sdfPacMemStayDecoderFunc;
    } else {
        PacWork *node;
        state->phase = 2;
        node = sdfPacNewResrc(state, packet);
        if (state->flags & PAC_STATE_ALLOCATE_HIGH) {
            node->resourceHandle = sdfMemAllocFromTailDirect(allocationSize);
        } else {
            node->resourceHandle = sdfMemAllocDirect(allocationSize);
        }
        state->outputCursor = node->dataCursor = (u8 *)sdfMemLock((struct sdfMemHandle_t *)(node->resourceHandle));
        switch (packet->flags & PAC_ENCODING_MASK) {
        case PAC_ENCODING_RAW:
            state->onInput = sdfPacMemDecoderFunc;
            break;
        case PAC_ENCODING_COMPRESSED: {
            SdfPacDecoder *decoder = sdfChipAllocDirect(0x20);
            state->decoder = (u8 *)decoder;
            sdfDecompInit(decoder, state->outputCursor);
            state->onInput = sdfPacMemDecompDecoderFunc;
            break;
        }
        default:
            break;
        }
    }
}

/* Begin a regular payload; completion is the normal packet finalizer. */
void sdfPacMemDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet) {
    sdfPacMemDecoderFuncInit(state, packet);
    state->onComplete = sdfDecodePacNodeAndAdvanceTail;
}

/* Apply the relocation record at the front of the queued payload. */
void sdfPacModelDecoderEndFunc(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 tableBytes = record->tableBytes;
    work->dataCursor = payload;
    if (tableBytes != 0) {
        sdfAddressResolver((s32 *)payload, (s32)payload, payload + record->tableOffset, tableBytes);
        record->tableBytes = 0;
    }
    sdfDecodePacNodeAndAdvanceTail(state);
}

/* Begin a payload that must be relocated before normal finalization. */
void sdfPacModelDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet) {
    sdfPacMemDecoderFuncInit(state, packet);
    state->onComplete = sdfPacModelDecoderEndFunc;
}

/* Complete the second relocation-command variant with the same word fixups. */
void sdfPacMotionDecoderEndFunc(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 tableBytes = record->tableBytes;
    work->dataCursor = payload;
    if (tableBytes != 0) {
        sdfAddressResolver((s32 *)payload, (s32)payload, payload + record->tableOffset, tableBytes);
        record->tableBytes = 0;
    }
    sdfDecodePacNodeAndAdvanceTail(state);
}

/* Begin the second relocation-command variant. */
void sdfPacMotionDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet) {
    sdfPacMemDecoderFuncInit(state, packet);
    state->onComplete = sdfPacMotionDecoderEndFunc;
}

/* Incrementally copy a resource chunk before processing its resource slot. */
void sdfPacTmxDecoderFunc(PacState *state) {
    PacBuf *resourceBuffer = state->resourceBuffer;
    s32 copyBytes = resourceBuffer->remainingBytes;
    if (state->inputAvailable < copyBytes) {
        copyBytes = state->inputAvailable;
    }
    if (copyBytes != 0) {
        memcpy(resourceBuffer->cursor, state->inputCursor, copyBytes);
        sdfPacUseSrc(state, copyBytes);
        resourceBuffer->cursor += copyBytes;
        {
            s32 bytesRemaining = resourceBuffer->remainingBytes - copyBytes;
            resourceBuffer->remainingBytes = bytesRemaining;
            if (bytesRemaining != 0) {
                return;
            }
        }
        resourceBuffer->texture = sdfTexSet((SdfTextureFileHeader *)sdfMemLock(resourceBuffer->resourceSlot));
        sdfMemFreeHandleDirect((s32 *)&resourceBuffer->resourceSlot);
        state->onComplete(state);
    }
}

/* Decode a chunk before processing and releasing its resource slot. */
void sdfPacTmxDecompDecoderFunc(PacState *state) {
    /* This handler offers pendingBytes, unlike the packet-level decoder. */
    s32 inputBytes = state->pendingBytes;
    s32 finished = sdfDecomp((SdfPacDecoder *)state->decoder, state->inputCursor, inputBytes);
    sdfPacUseSrc(state, inputBytes - ((SdfPacDecoder *)state->decoder)->remainingInputBytes);
    if (finished == 0) {
        return;
    }
    {
        PacBuf *resourceBuffer = state->resourceBuffer;
        resourceBuffer->texture = sdfTexSet((SdfTextureFileHeader *)sdfMemLock(resourceBuffer->resourceSlot));
        sdfMemFreeHandleDirect((s32 *)&resourceBuffer->resourceSlot);
    }
    sdfChipFreeDirect(state->decoder);
    state->onComplete(state);
}

/* Skip the remaining resource chunk and process its existing cursor. */
void sdfPacTmxStayDecoderFunc(PacState *state) {
    PacBuf *resourceBuffer = state->resourceBuffer;
    s32 skipBytes = resourceBuffer->remainingBytes;
    if (state->inputAvailable < skipBytes) {
        skipBytes = state->inputAvailable;
    }
    if (skipBytes != 0) {
        sdfPacUseSrc(state, skipBytes);
        {
            s32 bytesRemaining = resourceBuffer->remainingBytes - skipBytes;
            resourceBuffer->remainingBytes = bytesRemaining;
            if (bytesRemaining != 0) {
                return;
            }
        }
        resourceBuffer->texture = sdfTexSetDirect((SdfTextureFileHeader *)resourceBuffer->cursor);
        state->onComplete(state);
    }
}

/* Point the resource buffer at one allocation packet: keep it in packet memory, or allocate a
 * block and copy (raw) or decode (compressed) the payload into it. */
void sdfPacTmxDecoderInitFunc(PacState *state, SdfPacStreamPacketHeader *packet, PacBuf *buffer) {
    state->resourceBuffer = buffer;
    buffer->remainingBytes = packet->payloadSize - PAC_HEADER_BYTES;
    if (state->flags & PAC_STATE_USE_PACKET_MEMORY) {
        state->onInput = sdfPacTmxStayDecoderFunc;
        buffer->cursor = (u8 *)(state->inputCursor - PAC_HEADER_BYTES);
        return;
    }
    if (state->flags & PAC_STATE_ALLOCATE_HIGH) {
        buffer->resourceSlot = sdfMemAllocFromTailDirect(packet->payloadSize);
    } else {
        buffer->resourceSlot = sdfMemAllocDirect(packet->payloadSize);
    }
    buffer->cursor = (u8 *)sdfMemLock(buffer->resourceSlot);
    memcpy(buffer->cursor, packet, PAC_HEADER_BYTES);
    buffer->cursor += PAC_HEADER_BYTES;
    switch (packet->flags) {
    case PAC_ENCODING_RAW:
        state->onInput = sdfPacTmxDecoderFunc;
        break;
    case PAC_ENCODING_COMPRESSED: {
        SdfPacDecoder *decoder = sdfChipAllocDirect(0x20);
        state->decoder = (u8 *)decoder;
        sdfDecompInit(decoder, buffer->cursor);
        state->onInput = sdfPacTmxDecompDecoderFunc;
        break;
    }
    }
}

void sdfPacTmxDecoderEndFunc(PacState *state) {
    state->queueTail->resourceHandle = state->slot.resource->texture;
    sdfChipFreeDirect(state->slot.resource);
    sdfDecodePacNodeAndAdvanceTail(state);
}

/* Allocate decoder state for a complete resource packet. */
void sdfPacTmxDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet) {
    sdfPacNewResrc(state, packet);
    {
        PacBuf *allocation = (PacBuf *)sdfChipAllocDirect(0x10);
        state->slot.resource = allocation;
        sdfPacTmxDecoderInitFunc(state, packet, allocation);
    }
    state->onComplete = sdfPacTmxDecoderEndFunc;
}

/* Start the next allocation entry at its inline descriptor. */
void sdfPacTexListDecoderTmxHeaderEndFunc(PacState *state) {
    sdfPacTmxDecoderInitFunc(state, &state->slot.list->entry, &state->slot.list->buffer);
    state->onComplete = sdfPacAdvanceAllocationEntry;
}

/* Reset the output cursor to the current allocation entry and hook the copy
   and completion callbacks. */
void sdfPacResetOutputToAllocationEntry(PacState *state) {
    state->outputCursor = (u8 *)&state->slot.list->entry;
    state->pendingBytes = 0x10;
    state->onInput = sdfPacMemDecoderFunc;
    state->onComplete = sdfPacTexListDecoderTmxHeaderEndFunc;
}

/* Advance the entry index and complete or request the next entry. */
void sdfPacAdvanceAllocationEntry(PacState *state) {
    PacListAllocation *allocation = state->slot.list;
    sdfTexListAdd(state->queueTail->resourceHandle, allocation->buffer.texture);
    {
        s32 nextIndex = allocation->entryIndex + 1;
        allocation->entryIndex = nextIndex;
        if (nextIndex == allocation->entryCount) {
            sdfChipFreeDirect(allocation);
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
    sdfPacUseSrc(state, skipBytes);
    {
        s32 bytesRemaining = state->pendingBytes - skipBytes;
        state->pendingBytes = bytesRemaining;
        if (bytesRemaining != 0) {
            return;
        }
    }
    sdfPacResetOutputToAllocationEntry(state);
}
