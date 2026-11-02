#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf_movie_api.h"
#include "sdf_stream_read_api.h"
#include "sdf_semaphore_api.h"
#include "file_pac.h"
#include "sdf_pac_callback.h"
#include "sdf_movie_stream.h"
#include "sdf_movie_state.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf.h"
#include "sdf_pac_packet.h"
#include "sdf_pac_state.h"
#include "sdf_pac_decode.h"
#include "sdf_pac_work.h"
#include "sdf_pac_reloc.h"
#include "sdf_resource_list_item.h"
#include "sdf_texture_file.h"
#include "sdf_pac_decoder.h"
#include "sdf_line_pair_packet_api.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_timer.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "ee_mmi.h"
#include "sdf_named_resource_request_api.h"
#include "sce_io.h"
#include "sdf_dev_event.h"
#include "sdf_key_pool.h"
#include "sdf_packet_alloc.h"
#include "sdf_stream_state.h"

enum {
    PAC_HEADER_BYTES = 0x10,
    PAC_EXTENSION_BYTES_MASK = 0xF0,
    PAC_ALIGNMENT_BYTES = 0x40,
    PAC_ALIGNMENT_MASK = 0x3F,
    PAC_PHASE_FINISHED = -1,
    PAC_INITIAL_PACKET_COUNTER = -1,
    PAC_STATE_USE_PACKET_MEMORY = 1,
    PAC_STATE_ALLOCATE_HIGH = 2
};

typedef struct PacAlloc {
    s32 entryCount;
    s32 entryIndex;
    u8 pad8[24];
    s32 resource;
} PacAlloc;

extern u8 D_003BD638[];

/* Start the packet counter at -1 and select a custom or built-in packet callback. */
void sdfPacInit(PacState *packet, void *callbackAddress) {
    memset(packet, 0, sizeof(*packet));
    packet->packetCounter = PAC_INITIAL_PACKET_COUNTER;
    if (callbackAddress != NULL) {
        packet->packetCallback = (s32 (*)())callbackAddress;
    } else {
        packet->packetCallback = sdfPacStdCallback;
    }
}

/* Retain packet-owned payload memory rather than allocating a separate resource. */
void sdfPacSetStay(PacState *state) {
    state->flags = state->flags | PAC_STATE_USE_PACKET_MEMORY;
}

/* Select the high-address allocator for packet payload resources. */
void sdfPacSetFromTail(PacState *state) {
    state->flags = state->flags | PAC_STATE_ALLOCATE_HIGH;
}

/* Advance the queue cursor before returning each node to the chip allocator. */
void sdfPacFree(PacState *state) {
    PacWork *cursor = state->queueHead;
    PacWork *current = cursor;

    while (cursor != NULL) {
        cursor = cursor->next;
        sdfChipFreeDirect(current);
        current = cursor;
    }
}

/* Mark the signed PAC state as finished. */
void sdfPacMarkPhaseFinished(s8 *phase) {
    *phase = PAC_PHASE_FINISHED;
}

/* Consume byteCount bytes without bounds checks, updating the cursor and both counters. */
void sdfPacUseSrc(PacState *state, s32 byteCount) {
    state->inputCursor = state->inputCursor + byteCount;
    state->inputAvailable = state->inputAvailable - byteCount;
    state->consumedBytes = state->consumedBytes + byteCount;
}

/* Align the stream, then report a packet header and its payload byte count to the callback. */
void sdfPacNewBlock(PacState *state) {
    SdfPacStreamPacketHeader *inputHeader;
    u32 alignmentOffset;
    u32 packetBytes;
    u32 headerBytes;
    u32 payloadBytes;
    SdfPacPacketCallbackResult (*packetCallback)();
    SdfPacPacketCallbackResult callbackResult;

    alignmentOffset = state->consumedBytes & PAC_ALIGNMENT_MASK;
    if (alignmentOffset != 0) {
        state->phase = 1;
        state->pendingBytes = PAC_ALIGNMENT_BYTES - alignmentOffset;
        return;
    }
    inputHeader = (SdfPacStreamPacketHeader *)state->inputCursor;
    /* Advance only the fixed header; retain its original address for the callback. */
    sdfPacUseSrc(state, PAC_HEADER_BYTES);
    packetBytes = inputHeader->packetByteCount;
    state->packetCounter++;
    /* The high nibble encodes extension bytes, not a bit offset. */
    headerBytes = (inputHeader->flags & PAC_EXTENSION_BYTES_MASK) + PAC_HEADER_BYTES;
    payloadBytes = packetBytes - headerBytes;
    state->pendingBytes = payloadBytes;
    packetCallback = state->packetCallback;
    callbackResult = packetCallback(state, SDF_PAC_CALLBACK_EVENT_PACKET_HEADER, inputHeader, payloadBytes);
    if (callbackResult == SDF_PAC_CALLBACK_FINISHED) {
        sdfPacMarkPhaseFinished(&state->phase);
        return;
    }
    if (callbackResult == SDF_PAC_CALLBACK_ALIGN) {
        state->phase = 1;
        state->pendingBytes = (state->pendingBytes + PAC_ALIGNMENT_MASK) & ~PAC_ALIGNMENT_MASK;
    } else {
        state->phase = 2;
    }
}

/* Notify completion: FINISHED stops decoding; DROP_TAIL releases the queue tail. */
void sdfPacEndBlock(PacState *state) {
    SdfPacPacketCallbackResult callbackResult;
    PacWork *tailPacket;
    PacWork *scanPacket;
    PacWork *nextPacket;

    callbackResult = state->packetCallback(state, SDF_PAC_CALLBACK_EVENT_PACKET_COMPLETE, state->queueTail->packet);
    if (callbackResult == SDF_PAC_CALLBACK_FINISHED) {
        sdfPacMarkPhaseFinished(&state->phase);
        return;
    }
    if (callbackResult == SDF_PAC_CALLBACK_DROP_TAIL) {
        tailPacket = state->queueTail;
        scanPacket = state->queueHead;
        if (scanPacket == tailPacket) {
            state->queueTail = NULL;
            state->queueHead = NULL;
        } else {
            while ((nextPacket = scanPacket->next) != tailPacket) {
                scanPacket = nextPacket;
            }
            /* The multi-item path clears the link without reassigning queueTail here. */
            scanPacket->next = NULL;
        }
        sdfChipFreeDirect(tailPacket);
    }
    state->phase = 0;
}

enum {
    PAC_COMMAND_PAYLOAD = 1,
    PAC_COMMAND_ALLOCATION_LIST = 2,
    PAC_COMMAND_MODEL = 6,
    PAC_COMMAND_MOTION = 8,
    PAC_COMMAND_TEXTURE_LIST = 9,
    PAC_COMMAND_END = 0xFF,
    PAC_ENCODING_RAW = 0,
    PAC_ENCODING_COMPRESSED = 1,
    PAC_ENCODING_MASK = 0xF
};

void sdfPacMemDecoderFuncInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacEndBlock(PacState *state);

void sdfPacNewBlock(PacState *state);

void sdfPacTmxDecoderInitFunc(PacState *state, SdfPacStreamPacketHeader *packet, PacBuf *buffer);

void sdfPacModelDecoderEndFunc(PacState *state);

void sdfPacMotionDecoderEndFunc(PacState *state);

void sdfPacTmxDecoderEndFunc(PacState *state);

void sdfPacResetOutputToAllocationEntry(PacState *state);

void sdfPacTexListDecoderTmxBodyEndFunc(PacState *state);

void sdfPacMemDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacModelDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacMotionDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

void sdfPacTmxDecoderInit(PacState *state, SdfPacStreamPacketHeader *packet);

/* Feed one input span through packet-boundary, skip, and decoder phases. */
SdfPacPhase sdfPacDecode(PacState *state, const u8 *input, s32 inputByteCount) {
    s32 consumeBytes;

    state->inputCursor = input;
    state->inputAvailable = inputByteCount;
    while (state->inputAvailable > 0) {
        switch (state->phase) {
        case SDF_PAC_PHASE_COMPLETE:
            return SDF_PAC_PHASE_COMPLETE;
        case SDF_PAC_PHASE_CALLBACK_BOUNDARY:
            sdfPacNewBlock(state);
            break;
        case SDF_PAC_PHASE_SKIP_BYTES:
            consumeBytes = state->pendingBytes;
            if (state->inputAvailable < consumeBytes) {
                consumeBytes = state->inputAvailable;
            }
            sdfPacUseSrc(state, consumeBytes);
            state->pendingBytes -= consumeBytes;
            if (state->pendingBytes == 0) {
                state->phase = SDF_PAC_PHASE_CALLBACK_BOUNDARY;
            }
            break;
        case SDF_PAC_PHASE_INPUT_HANDLER:
            state->onInput(state);
            break;
        }
    }
    return state->phase;
}

/* Queue a private copy of the packet header and any extension bytes. */
PacWork *sdfPacNewResrc(PacState *state, const SdfPacStreamPacketHeader *packet) {
    s32 extensionBytes = packet->flags & PAC_EXTENSION_BYTES_MASK;
    PacWork *node = sdfChipAllocZeroDirect(extensionBytes + sizeof(PacWork) + PAC_HEADER_BYTES);
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
    PacWork **incomingLink = &state->queueHead;
    PacWork *currentWork = state->queueHead;
    PacWork *previousWork = NULL;
    PacWork *nextWork;
    while (currentWork != work) {
        previousWork = currentWork;
        incomingLink = &currentWork->next;
        currentWork = *incomingLink;
    }
    nextWork = work->next;
    *incomingLink = nextWork;
    if (state->queueTail == work) {
        state->queueTail = previousWork;
    }
    sdfChipFreeDirect(work);
    return nextWork;
}

/* Dispatch recognized PAC commands; one marks end-of-stream. */
SdfPacPacketCallbackResult sdfPacStdCallback(PacState *state, SdfPacPacketCallbackEvent event, SdfPacStreamPacketHeader *packet) {
    if (event == SDF_PAC_CALLBACK_EVENT_PACKET_HEADER) {
        switch (packet->command) {
        case PAC_COMMAND_PAYLOAD:
            sdfPacMemDecoderInit(state, packet);
            return SDF_PAC_CALLBACK_CONTINUE;
        case PAC_COMMAND_ALLOCATION_LIST:
            sdfPacTmxDecoderInit(state, packet);
            return SDF_PAC_CALLBACK_CONTINUE;
        case PAC_COMMAND_MODEL:
            sdfPacModelDecoderInit(state, packet);
            return SDF_PAC_CALLBACK_CONTINUE;
        case PAC_COMMAND_MOTION:
            sdfPacMotionDecoderInit(state, packet);
            return SDF_PAC_CALLBACK_CONTINUE;
        case PAC_COMMAND_TEXTURE_LIST:
            sdfPacTexListDecoderInit(state, packet);
            return SDF_PAC_CALLBACK_CONTINUE;
        case PAC_COMMAND_END:
            return SDF_PAC_CALLBACK_FINISHED;
        default:
            return SDF_PAC_CALLBACK_UNHANDLED_COMMAND;
        }
    }
    return SDF_PAC_CALLBACK_CONTINUE;
}

/* Return the resource-extension payload when its extension-length nibble is nonzero. */
const char *sdfPacGetResrcFileName(PacWork *work) {
    SdfPacStreamPacketHeader *extension = (SdfPacStreamPacketHeader *)work->packet;
    s32 extensionBytes = extension->flags & PAC_EXTENSION_BYTES_MASK;
    if (extensionBytes <= 0) {
        return NULL;
    }
    return (const char *)extension->payload;
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
    SdfPacDecoderResult result = sdfDecomp((SdfPacDecoder *)state->decoder, state->inputCursor, inputBytes);
    sdfPacUseSrc(state, inputBytes - ((SdfPacDecoder *)state->decoder)->remainingInputBytes);
    if (result == SDF_PAC_DECODE_INCOMPLETE) {
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
        allocationSize = packet->packetByteCount + (packet->flags & PAC_EXTENSION_BYTES_MASK) - PAC_HEADER_BYTES;
    }
    if (state->flags & PAC_STATE_USE_PACKET_MEMORY) {
        PacWork *node = sdfPacNewResrc(state, packet);
        node->dataCursor = packet->payload;
        state->onInput = sdfPacMemStayDecoderFunc;
    } else {
        PacWork *node;
        state->phase = SDF_PAC_PHASE_INPUT_HANDLER;
        node = sdfPacNewResrc(state, packet);
        if (state->flags & PAC_STATE_ALLOCATE_HIGH) {
            node->resourceHandle = sdfMemAllocFromTailDirect(allocationSize);
        } else {
            node->resourceHandle = sdfMemAllocDirect(allocationSize);
        }
        state->outputCursor = node->dataCursor = (u8 *)sdfMemLock((struct sdfMemHandle_t *)(PAC_WORK_ALLOCATION(node)));
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
    state->onComplete = sdfPacEndBlock;
}

/* Apply the relocation record at the front of the queued payload. */
void sdfPacModelDecoderEndFunc(PacState *state) {
    PacWork *work = state->queueTail;
    PacReloc *record = (PacReloc *)work->dataCursor;
    u8 *payload = record->payload;
    s32 tableBytes = record->tableBytes;
    work->dataCursor = payload;
    if (tableBytes != 0) {
        sdfAddressResolver((s32 *)payload, (u32)payload, payload + record->tableOffset, tableBytes);
        record->tableBytes = 0;
    }
    sdfPacEndBlock(state);
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
        sdfAddressResolver((s32 *)payload, (u32)payload, payload + record->tableOffset, tableBytes);
        record->tableBytes = 0;
    }
    sdfPacEndBlock(state);
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
        sdfMemFreeHandleDirect(&resourceBuffer->resourceSlot);
        state->onComplete(state);
    }
}

/* Decode a chunk before processing and releasing its resource slot. */
void sdfPacTmxDecompDecoderFunc(PacState *state) {
    /* This handler offers pendingBytes, unlike the packet-level decoder. */
    s32 inputBytes = state->pendingBytes;
    SdfPacDecoderResult result = sdfDecomp((SdfPacDecoder *)state->decoder, state->inputCursor, inputBytes);
    sdfPacUseSrc(state, inputBytes - ((SdfPacDecoder *)state->decoder)->remainingInputBytes);
    if (result == SDF_PAC_DECODE_INCOMPLETE) {
        return;
    }
    {
        PacBuf *resourceBuffer = state->resourceBuffer;
        resourceBuffer->texture = sdfTexSet((SdfTextureFileHeader *)sdfMemLock(resourceBuffer->resourceSlot));
        sdfMemFreeHandleDirect(&resourceBuffer->resourceSlot);
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
    buffer->remainingBytes = packet->packetByteCount - PAC_HEADER_BYTES;
    if (state->flags & PAC_STATE_USE_PACKET_MEMORY) {
        state->onInput = sdfPacTmxStayDecoderFunc;
        buffer->cursor = (u8 *)(state->inputCursor - PAC_HEADER_BYTES);
        return;
    }
    if (state->flags & PAC_STATE_ALLOCATE_HIGH) {
        buffer->resourceSlot = sdfMemAllocFromTailDirect(packet->packetByteCount);
    } else {
        buffer->resourceSlot = sdfMemAllocDirect(packet->packetByteCount);
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
    sdfPacEndBlock(state);
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
    state->onComplete = sdfPacTexListDecoderTmxBodyEndFunc;
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
void sdfPacTexListDecoderTmxBodyEndFunc(PacState *state) {
    PacListAllocation *allocation = state->slot.list;
    /* Transfer the decoded texture's acquired reference into the resource list. */
    sdfTexListAdd(PAC_WORK_TEXTURE_LIST(state->queueTail), allocation->buffer.texture);
    {
        s32 nextIndex = allocation->entryIndex + 1;
        allocation->entryIndex = nextIndex;
        if (nextIndex == allocation->entryCount) {
            sdfChipFreeDirect(allocation);
            sdfPacEndBlock(state);
        } else {
            sdfPacResetOutputToAllocationEntry(state);
        }
    }
}

/* Discard pending input until the next allocation entry can start; check the
   boundary even when no bytes are consumed. */
void sdfPacTexListDecoder1(PacState *state) {
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

extern DevRequest *sdfTexListNew(s32 count);

/* Read the entry count word, size the list for it and arm the skip handler. */
void sdfPacTexListDecoder0(PacState *state) {
    u32 count;
    PacListAllocation *allocation;
    PacWork *work;

    if (state->inputAvailable != 0) {
        /* The aligned packet count is read with native lw semantics. */
        count = *(const u32 *)state->inputCursor;
        sdfPacUseSrc(state, 4);
        allocation = state->slot.list;
        work = state->queueTail;
        allocation->entryCount = count;
        allocation->entryIndex = 0;
        work->resourceHandle = sdfTexListNew(count);
        state->pendingBytes = ((count * 4 + 0x53) & -0x40) - 0x14;
        state->onInput = sdfPacTexListDecoder1;
    }
}

void sdfPacTexListDecoderInit(PacState *state,
                                SdfPacStreamPacketHeader *packet) {
    sdfPacNewResrc(state, packet);
    state->slot.list = sdfChipAllocDirect(sizeof(PacListAllocation));
    state->onInput = sdfPacTexListDecoder0;
}
