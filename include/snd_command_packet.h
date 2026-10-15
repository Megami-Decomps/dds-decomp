#ifndef SND_COMMAND_PACKET_H
#define SND_COMMAND_PACKET_H

#include "common.h"

/* Sixteen-byte track-command payload shared by both titles. */
typedef struct CmdPacket {
    u32 trackId; /* 0x00 */
    u32 unk4;    /* 0x04 */
    u16 setting; /* 0x08 */
    u16 unkA;    /* 0x0A */
    u32 unkC;    /* 0x0C */
} CmdPacket;

/* Queue sound command 0x40 without a payload. */
void sndQueueNoPayloadCommand40(void);

typedef char CmdPacket_size_must_be_0x10[(sizeof(CmdPacket) == 0x10) ? 1 : -1];
typedef char CmdPacket_trackId_offset_must_be_0[
    ((u32)&((CmdPacket *)0)->trackId == 0) ? 1 : -1];
typedef char CmdPacket_unk4_offset_must_be_4[
    ((u32)&((CmdPacket *)0)->unk4 == 0x04) ? 1 : -1];
typedef char CmdPacket_setting_offset_must_be_8[
    ((u32)&((CmdPacket *)0)->setting == 0x08) ? 1 : -1];
typedef char CmdPacket_unkA_offset_must_be_A[
    ((u32)&((CmdPacket *)0)->unkA == 0x0A) ? 1 : -1];
typedef char CmdPacket_unkC_offset_must_be_C[
    ((u32)&((CmdPacket *)0)->unkC == 0x0C) ? 1 : -1];

#endif /* SND_COMMAND_PACKET_H */
