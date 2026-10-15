#ifndef SND_WORK_H
#define SND_WORK_H

#include "common.h"
#include "snd_track_slot.h"

#define SND_WORK_CHANNEL_COUNT 16
#define SND_WORK_IOP_BUFFER_COUNT 16
#define SND_WORK_TRACK_SLOT_COUNT 13

typedef struct SndChannel {
    u8 index;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    u32 iopWorkAddress;
    u32 unkC;
} SndChannel;

typedef struct SndIopBuffer {
    s32 size;
    u32 address;
} SndIopBuffer;

/* Prefix and arrays of the MIDI sound state; trailing words remain opaque. */
typedef struct SndWork {
    s32 header;
    s32 bufferCount;
    u32 iopBufferAddress;
    u32 iopBufferBytes;
    SndChannel channels[SND_WORK_CHANNEL_COUNT];
    SndIopBuffer buffers[SND_WORK_IOP_BUFFER_COUNT];
    SndTrackSlot slots[SND_WORK_TRACK_SLOT_COUNT];
    u32 opaque1F8;
    u32 opaque1FC;
    u32 opaque200;
    u32 opaque204;
} SndWork;

/* sceSifSetDma descriptor: four words filled before one descriptor is submitted. */
typedef struct SifDmaTransfer {
    u32 source;
    s32 destination;
    s32 size;
    s32 attributes;
} SifDmaTransfer;

typedef char SndChannelSizeAssert[(sizeof(SndChannel) == 0x10) ? 1 : -1];
typedef char SndIopBufferSizeAssert[(sizeof(SndIopBuffer) == 8) ? 1 : -1];
typedef char SndWorkArrayOffsetsAssert[
    ((u32)&((SndWork *)0)->channels == 0x10 &&
     (u32)&((SndWork *)0)->buffers == 0x110 &&
     (u32)&((SndWork *)0)->slots == 0x190 &&
     (u32)&((SndWork *)0)->opaque1F8 == 0x1F8) ? 1 : -1];
typedef char SifDmaTransferLayoutAssert[
    (sizeof(SifDmaTransfer) == 0x10 &&
     (u32)&((SifDmaTransfer *)0)->destination == 4 &&
     (u32)&((SifDmaTransfer *)0)->size == 8 &&
     (u32)&((SifDmaTransfer *)0)->attributes == 0xC) ? 1 : -1];

#endif
