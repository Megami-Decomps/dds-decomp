#ifndef SND_SEQUENCE_VOLUME_PAN_H
#define SND_SEQUENCE_VOLUME_PAN_H

#include "common.h"

/* Apply volume and pan to a sequence track through the command queue. */
void sndSetSequenceVolumePan(s32 trackId, s32 volume, s32 pan);

/* Local packet object; only the first 0xC bytes are sent to the sound worker. */
typedef struct SndSequenceVolumePanPacket {
    u32 trackId;          /* +0x00 */
    u32 opaque04;         /* +0x04 */
    u16 volume;           /* +0x08 */
    u16 pan;              /* +0x0A */
    u32 opaqueNotSent0C;  /* +0x0C */
} SndSequenceVolumePanPacket;

typedef char SndSequenceVolumePanPacket_trackId_at_0[
    ((u32)&((SndSequenceVolumePanPacket *)0)->trackId == 0) ? 1 : -1];
typedef char SndSequenceVolumePanPacket_volume_at_8[
    ((u32)&((SndSequenceVolumePanPacket *)0)->volume == 8) ? 1 : -1];
typedef char SndSequenceVolumePanPacket_pan_at_A[
    ((u32)&((SndSequenceVolumePanPacket *)0)->pan == 0xA) ? 1 : -1];

#endif /* SND_SEQUENCE_VOLUME_PAN_H */
