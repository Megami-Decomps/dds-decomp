#ifndef SDF_PAC_PACKET_H
#define SDF_PAC_PACKET_H

#include "common.h"

/* Fixed serialized header shared by the PAC stream reader and dispatcher.
 * The model packet view names the word at +0x08 as a tag. */
typedef struct SdfPacStreamPacketHeader {
    u8 command; /* 0x00 */
    u8 flags; /* 0x01 */
    u8 reserved02[2];
    s32 payloadSize; /* 0x04: serialized packet bytes */
    u8 reserved08[4];
    s32 decodedSize; /* 0x0C */
    u8 payload[0]; /* 0x10: variable packet contents */
} SdfPacStreamPacketHeader;

/* Model-group consumers interpret the stream header's +0x08 word as a tag. */
typedef struct SdfPacModelPacketHeader {
    u8 command; /* 0x00 */
    u8 flags; /* 0x01 */
    u8 reserved02[2]; /* 0x02 */
    s32 payloadSize; /* 0x04 */
    u32 tag; /* 0x08 */
    s32 decodedSize; /* 0x0C */
    u8 payload[0]; /* 0x10: variable packet contents */
} SdfPacModelPacketHeader;

typedef char SdfPacModelPacketHeader_size_must_be_0x10[
    (sizeof(SdfPacModelPacketHeader) == 0x10) ? 1 : -1];
typedef char SdfPacModelPacketHeader_tag_must_be_at_0x08[
    ((u32)&((SdfPacModelPacketHeader *)0)->tag == 0x08) ? 1 : -1];
typedef char SdfPacModelPacketHeader_decodedSize_must_be_at_0x0C[
    ((u32)&((SdfPacModelPacketHeader *)0)->decodedSize == 0x0C) ? 1 : -1];

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
