#ifndef SND_RING_H
#define SND_RING_H

#include "common.h"

/* Native SIF sound-command ring entry. The transport owns the first three words. */
typedef struct SndRingPacket {
    u32 transportHeader[3];
    u32 command;
    u32 sequence;
    u8 payload[108];
} SndRingPacket;

typedef char SndRingPacket_size_must_be_0x80[(sizeof(SndRingPacket) == 0x80) ? 1 : -1];
typedef char SndRingPacket_command_at_0x0C[((u32)&((SndRingPacket *)0)->command == 0x0C) ? 1 : -1];
typedef char SndRingPacket_sequence_at_0x10[((u32)&((SndRingPacket *)0)->sequence == 0x10) ? 1 : -1];
typedef char SndRingPacket_payload_at_0x14[((u32)&((SndRingPacket *)0)->payload == 0x14) ? 1 : -1];
typedef char SndRingPacket_payload_size_108[(sizeof(((SndRingPacket *)0)->payload) == 108) ? 1 : -1];

#endif
