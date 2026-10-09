#ifndef SDF_PAC_STATE_H
#define SDF_PAC_STATE_H

#include "common.h"
#include "sdf_pac_packet.h"
#include "sdf_pac_work.h"

struct PacAlloc;
struct SdfMemBlock;
struct SdfTex;

/* Complete 0x10-byte packet buffer: texture is the acquired output, while
 * resourceSlot owns the optional general-heap allocation descriptor. */
typedef struct PacBuf {
    struct SdfTex *texture;
    struct SdfMemBlock *resourceSlot;
    u8 *cursor;
    s32 remainingBytes;
} PacBuf;

typedef char PacBuf_size_must_be_0x10[(sizeof(PacBuf) == 0x10) ? 1 : -1];
typedef char PacBuf_resourceSlot_offset_must_be_4[
    ((u32)&((PacBuf *)0)->resourceSlot == 4) ? 1 : -1];
typedef char PacBuf_remainingBytes_offset_must_be_C[
    ((u32)&((PacBuf *)0)->remainingBytes == 0xC) ? 1 : -1];

/* Complete 0x38-byte PAC decoder state shared by file and model loaders. */
typedef struct PacState {
    s8 phase; /* 0x00: -1 complete, 0 boundary, 1 skip, 2 input handler */
    u8 flags; /* 0x01 */
    u16 packetCounter; /* 0x02: wraps from the initial 0xFFFF */
    s32 (*packetCallback)(); /* 0x04: event callbacks have different arities */
    void (*onInput)(struct PacState *); /* 0x08 */
    void (*onComplete)(struct PacState *); /* 0x0C */
    u8 *inputCursor; /* 0x10 */
    s32 inputAvailable; /* 0x14 */
    s32 consumedBytes; /* 0x18 */
    u8 *outputCursor; /* 0x1C */
    s32 pendingBytes; /* 0x20 */
    u8 *decoder; /* 0x24: decoder work block, held as a byte pointer */
    PacBuf *resourceBuffer; /* 0x28 */
    union {
        PacBuf *resource;
        struct PacAlloc *list;
    } slot; /* 0x2C */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;

typedef char PacState_size_must_be_0x38[(sizeof(PacState) == 0x38) ? 1 : -1];
typedef char PacState_phase_must_be_at_0x00[
    ((u32)&((PacState *)0)->phase == 0x00) ? 1 : -1];
typedef char PacState_packetCallback_must_be_at_0x04[
    ((u32)&((PacState *)0)->packetCallback == 0x04) ? 1 : -1];
typedef char PacState_slot_must_be_at_0x2C[
    ((u32)&((PacState *)0)->slot == 0x2C) ? 1 : -1];
typedef char PacState_queueHead_must_be_at_0x30[
    ((u32)&((PacState *)0)->queueHead == 0x30) ? 1 : -1];
typedef char PacState_queueTail_must_be_at_0x34[
    ((u32)&((PacState *)0)->queueTail == 0x34) ? 1 : -1];

void sdfPacInitializeDispatchPacket(PacState *state, void *callbackAddress);
s32 sdfPacFeedInput(PacState *state, u8 *input, s32 available);
void sdfPacUsePacketPayloadMemory(PacState *state);
void sdfPacReleasePacketQueueNodes(PacState *state);

#endif /* SDF_PAC_STATE_H */
