#ifndef SDF_PAC_CALLBACK_H
#define SDF_PAC_CALLBACK_H

#include "common.h"
#include "sdf_pac_packet.h"

/* The decoder honors FINISHED at either callback event, ALIGN for packet
 * headers, and DROP_TAIL at packet completion; other values continue. */
typedef s32 SdfPacPacketCallbackResult;
enum SdfPacPacketCallbackResultValue {
    SDF_PAC_CALLBACK_CONTINUE = 0,
    SDF_PAC_CALLBACK_FINISHED = 1,
    SDF_PAC_CALLBACK_ALIGN = 2,
    SDF_PAC_CALLBACK_UNHANDLED_COMMAND = 3,
    SDF_PAC_CALLBACK_DROP_TAIL = 4
};

/* Header events pass a fourth payload-byte-count argument; completion events
 * use only the three shared callback arguments. */
typedef s32 SdfPacPacketCallbackEvent;
enum SdfPacPacketCallbackEventValue {
    SDF_PAC_CALLBACK_EVENT_PACKET_HEADER = 0,
    SDF_PAC_CALLBACK_EVENT_PACKET_COMPLETE = 1
};

struct PacState;
SdfPacPacketCallbackResult sdfPacStdCallback(
    struct PacState *state, SdfPacPacketCallbackEvent event,
    SdfPacStreamPacketHeader *packet);

#endif /* SDF_PAC_CALLBACK_H */
