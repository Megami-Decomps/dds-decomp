#ifndef SDF_PAC_PACKET_H
#define SDF_PAC_PACKET_H

#include "common.h"

/* Fixed serialized header shared by the PAC stream reader and dispatcher.
 * Model-part packet records use their own +0x0C tag view. */
typedef struct SdfPacStreamPacketHeader {
    u8 command; /* 0x00 */
    u8 flags; /* 0x01 */
    u8 reserved02[2];
    s32 payloadSize; /* 0x04: serialized packet bytes */
    u8 reserved08[4];
    s32 decodedSize; /* 0x0C */
    u8 payload[0]; /* 0x10: variable packet contents */
} SdfPacStreamPacketHeader;

typedef char SdfPacStreamPacketHeader_size_must_be_0x10[
    (sizeof(SdfPacStreamPacketHeader) == 0x10) ? 1 : -1];
typedef char SdfPacStreamPacketHeader_payloadSize_offset_must_be_4[
    ((u32)&((SdfPacStreamPacketHeader *)0)->payloadSize == 4) ? 1 : -1];
typedef char SdfPacStreamPacketHeader_decodedSize_offset_must_be_0x0C[
    ((u32)&((SdfPacStreamPacketHeader *)0)->decodedSize == 0x0C) ? 1 : -1];

struct PacState;
s32 sdfPacDispatchPacket(struct PacState *state, s32 status,
                         SdfPacStreamPacketHeader *packet);

#endif /* SDF_PAC_PACKET_H */
