#ifndef SDF_PACKET_SLOT_H
#define SDF_PACKET_SLOT_H

#include "sdf_packet_list.h"

/* One 0x10-byte entry in the three-slot frame packet submission ring. */
typedef struct SdfPacketSlot {
    struct SdfListHead *packetList;
    void *callbackHead;
    u8 slotUpdatePhase;
    u8 queuedBufferIndex;
    u8 bufferIndex;
    u8 bufferSlotIndex;
    u8 pad0C[4];
} SdfPacketSlot;

typedef char SdfPacketSlot_size_must_be_0x10[
    sizeof(SdfPacketSlot) == 0x10 ? 1 : -1];
typedef char SdfPacketSlot_packetList_offset_must_be_0[
    ((u32)&((SdfPacketSlot *)0)->packetList) == 0x00 ? 1 : -1];
typedef char SdfPacketSlot_callbackHead_offset_must_be_4[
    ((u32)&((SdfPacketSlot *)0)->callbackHead) == 0x04 ? 1 : -1];
typedef char SdfPacketSlot_slotUpdatePhase_offset_must_be_8[
    ((u32)&((SdfPacketSlot *)0)->slotUpdatePhase) == 0x08 ? 1 : -1];
typedef char SdfPacketSlot_queuedBufferIndex_offset_must_be_9[
    ((u32)&((SdfPacketSlot *)0)->queuedBufferIndex) == 0x09 ? 1 : -1];
typedef char SdfPacketSlot_bufferIndex_offset_must_be_A[
    ((u32)&((SdfPacketSlot *)0)->bufferIndex) == 0x0A ? 1 : -1];
typedef char SdfPacketSlot_bufferSlotIndex_offset_must_be_B[
    ((u32)&((SdfPacketSlot *)0)->bufferSlotIndex) == 0x0B ? 1 : -1];

#endif /* SDF_PACKET_SLOT_H */
