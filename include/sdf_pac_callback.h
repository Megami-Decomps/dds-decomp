#ifndef SDF_PAC_CALLBACK_H
#define SDF_PAC_CALLBACK_H

#include "common.h"
#include "sdf_pac_packet.h"

/* Header events pass a fourth payload-byte-count argument; completion events
 * use only the three shared callback arguments. */
typedef s32 SdfPacPacketCallbackEvent;
enum SdfPacPacketCallbackEventValue {
    SDF_PAC_CALLBACK_EVENT_PACKET_HEADER = 0,
    SDF_PAC_CALLBACK_EVENT_PACKET_COMPLETE = 1
};

struct PacState;
s32 sdfPacStdCallback(struct PacState *state, SdfPacPacketCallbackEvent event,
                      SdfPacStreamPacketHeader *packet);

#endif /* SDF_PAC_CALLBACK_H */
