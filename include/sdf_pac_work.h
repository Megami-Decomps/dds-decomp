#ifndef SDF_PAC_WORK_H
#define SDF_PAC_WORK_H

#include "common.h"

struct PacState;
struct SdfPacStreamPacketHeader;

/* Queue node allocated with an inline, variable-length PAC packet. */
typedef struct PacWork {
    struct PacWork *next; /* 0x00 */
    struct PacState *owner; /* 0x04 */
    s32 resourceHandle; /* 0x08: generic stored resource word */
    u8 *dataCursor; /* 0x0C */
    u8 packet[0]; /* 0x10: copied packet header, extensions, and data */
} PacWork;

typedef char PacWork_size_must_be_0x10[(sizeof(PacWork) == 0x10) ? 1 : -1];
typedef char PacWork_owner_must_be_at_0x04[
    ((u32)&((PacWork *)0)->owner == 0x04) ? 1 : -1];
typedef char PacWork_resource_must_be_at_0x08[
    ((u32)&((PacWork *)0)->resourceHandle == 0x08) ? 1 : -1];
typedef char PacWork_dataCursor_must_be_at_0x0C[
    ((u32)&((PacWork *)0)->dataCursor == 0x0C) ? 1 : -1];
typedef char PacWork_packet_must_be_at_0x10[
    ((u32)&((PacWork *)0)->packet == 0x10) ? 1 : -1];

PacWork *sdfPacEnqueuePacket(struct PacState *state,
                             struct SdfPacStreamPacketHeader *packet);
PacWork *sdfPacRemovePacket(PacWork *work);

#endif /* SDF_PAC_WORK_H */
