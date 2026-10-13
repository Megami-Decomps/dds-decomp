#include "snd_ring.h"
#include "common.h"
#include "snd_command_packet.h"
#include "snd_work.h"
#include "sce_io.h"

#define SND_COMMAND_RING_ENTRY_COUNT 32
#define SND_COMMAND_RING_INDEX_MASK (SND_COMMAND_RING_ENTRY_COUNT - 1)
#define SND_COMMAND_HEADER_BYTES 0x14
#define SND_COMMAND_QUADWORD_BYTES 16
#define SND_COMMAND_QUADWORD_SHIFT 4
#define SND_COMMAND_CHANNEL_MASK 0xFFFF
#define SND_COMMAND_ID_SHIFT 16
#define SND_COMMAND_LENGTH_SHIFT 28

#define SND_CHANNEL_COUNT 16
#define SND_IOP_BUFFER_COUNT 16
#define SND_TRACK_SLOT_COUNT 13
#define SND_IOP_BUFFER_ALIGNMENT 16

u32 sndQueueCommandPacket(u32 command, u32 channel, void *packet, s32 size);

void sndEnsureMidiBankResident(s32 trackId);

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size);

/* Sound command work block: header, 16 channels/buffers, 13 track slots. */
extern SndWork sndMidiTrackState;
extern u32 sndReserveIopWorkMemory(s32 size);
extern s8 D_004391E0;
extern s32 D_004391EC;
extern s32 sceSifInitIopHeap(void);
extern s32 func_003415A8(void);
extern void sndCopyToIopAndWait(s32, void *, s32);

extern SndRingPacket D_00477A00[SND_COMMAND_RING_ENTRY_COUNT];
extern u32 D_00438B90;
/* The worker consumes entries while this producer publishes and polls cursors. */
extern vu16 D_004391E2;
extern vu16 D_004391E4;
extern s32 D_004391E8;
extern s32 WakeupThread(s32 thread);
extern s32 sndTrySendQueuedCommandPacket(void);
extern void *memcpy(void *, const void *, u32);
extern void FlushCache(s32);
extern s32 sceSifSetDma(void *, s32);
extern s32 sceSifDmaStat(s32);

u32 sndQueueCommandPacket(u32 command, u32 channel, void *packet, s32 payloadBytes) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    SndRingPacket *entry;
    u32 transferQuadwords;

    if (++D_00438B90 == 0) {
        D_00438B90 = 1;
    }
    nextWriteIndex = ((s16)D_004391E4 + 1) & SND_COMMAND_RING_INDEX_MASK;
    if (nextWriteIndex == (s16)D_004391E2) {
        for (wakeAttemptsRemaining = 8; wakeAttemptsRemaining != 0; wakeAttemptsRemaining--) {
            do {
                WakeupThread(D_004391E8);
            } while ((s16)D_004391E2 != nextWriteIndex);
        }
        return 0;
    }
    entry = &D_00477A00[(s16)D_004391E4];
    entry->sequence = D_00438B90;
    if (payloadBytes != 0) {
        memcpy(entry->payload, packet, payloadBytes);
    }
    /* The SIF length includes the transport/command header and rounds up to quadwords. */
    transferQuadwords = (payloadBytes + SND_COMMAND_HEADER_BYTES +
                        (SND_COMMAND_QUADWORD_BYTES - 1)) >> SND_COMMAND_QUADWORD_SHIFT;
    entry->command = (command << SND_COMMAND_ID_SHIFT) |
                     (channel & SND_COMMAND_CHANNEL_MASK) |
                     (transferQuadwords << SND_COMMAND_LENGTH_SHIFT);
    D_004391E4 = nextWriteIndex;
    if (D_004391E0 != 0) {
        while (sndTrySendQueuedCommandPacket() != 0) {
        }
    } else {
        WakeupThread(D_004391E8);
    }
    return D_00438B90;
}

void func_003417A0(u32 unused) {
}

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size) {
    u32 result = sndQueueCommandPacket(command, channel, packet, size);

    func_003417A0(result);
    return result;
}

/* Descriptor consumed by sceSifSetDma: EE source, IOP destination, byte size, attributes. */
extern SifDmaTransfer D_00479A00;

/* Transfer an EE buffer to IOP memory, retrying submission and waiting for completion. */
void sndCopyToIopAndWait(s32 destination, void *source, s32 size) {
    s32 dmaId;
    s32 delay;

    D_00479A00.source = (u32)source;
    D_00479A00.destination = destination;
    D_00479A00.size = size;
    D_00479A00.attributes = 0;
    FlushCache(0);
    goto submit;
retry:
    /* Busy-wait before retrying a full SIF DMA queue. */
    delay = 0x186A0;
    while (delay != 0) {
        delay -= 0x271;
        __asm__ volatile ("nop");
    }
submit:
    dmaId = sceSifSetDma(&D_00479A00, 1);
    if (dmaId == 0) {
        goto retry;
    }
    while (sceSifDmaStat(dmaId) >= 0) {
    }
}


extern u8 sdfPfsDebugMode;
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern s32 func_0036B6E0(const char *, SceIoStat *);
extern s32 sdfSendNamedResourceRequest(char *, s32, void *, s32 *);

/* Load the sound modules; debug PFS may override only the SDF sound manager. */
void sndLoadIopSoundModules(void) {
    SceIoStat info;
    char *module;

    sceSifLoadModule("cdrom0:\\SOUNDIRX\\LIBSD.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\SDRDRV.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\MODHSYN.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\MODMIDI.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\MODMSIN.IRX;1", 0, NULL);
    module = "cdrom0:\\USERIRX\\SDFSDMAN.IRX;1";
    if (sdfPfsDebugMode != 0 &&
        func_0036B6E0("pfs:/userirx/SDFSDMAN.IRX", &info) == 0 &&
        (info.mode & 0xF000) == 0x2000) {
        module = "pfs:/userirx/SDFSDMAN.IRX";
    }
    sdfSendNamedResourceRequest(module, 0, NULL, NULL);
}

void sndInitializeChannelAndTrackState(s32 unused, s32 header) {
    SndChannel *channel;
    s32 i;

    sndMidiTrackState.header = header;
    channel = sndMidiTrackState.channels;
    for (i = 0; i < SND_CHANNEL_COUNT; i++) {
        channel->index = i;
        channel->unk1 = 0x20;
        channel->unk2 = 0;
        channel->unk8 = 0;
        channel->unk4 = 0;
        channel->unkC = 0;
        channel++;
    }
    sndMidiTrackState.channels[0].unk8 = sndReserveIopWorkMemory(0x3B600);
    sndMidiTrackState.opaque200 = 0;
    sndMidiTrackState.opaque204 = 0;
    for (i = 0; i < SND_TRACK_SLOT_COUNT; i++) {
        sndMidiTrackState.slots[i].id = -1;
        sndMidiTrackState.slots[i].unknown_04[0] = 0;
        sndMidiTrackState.slots[i].unknown_04[1] = 0;
    }
}

/* Allocate one aligned IOP block and distribute its address among active buffers. */
void sndAllocateAlignedIopBuffers(s32 *requestedSizes, s32 bufferCount) {
    SndIopBuffer *buffer;
    s32 totalBytes;
    s32 alignedBytes;
    s32 bufferIndex;
    u32 iopAddress;

    sndMidiTrackState.bufferCount = bufferCount;
    buffer = sndMidiTrackState.buffers;
    totalBytes = 0;
    bufferIndex = 0;
    do {
        alignedBytes = (*requestedSizes++ + (SND_IOP_BUFFER_ALIGNMENT - 1)) &
                       ~(SND_IOP_BUFFER_ALIGNMENT - 1);
        buffer->size = alignedBytes;
        buffer++;
        totalBytes += alignedBytes;
        bufferIndex++;
    } while (bufferIndex != bufferCount);
    for (; bufferIndex < SND_IOP_BUFFER_COUNT; bufferIndex++) {
        sndMidiTrackState.buffers[bufferIndex].size = 0;
        sndMidiTrackState.buffers[bufferIndex].address = 0;
    }
    iopAddress = sndReserveIopWorkMemory(totalBytes);
    bufferIndex = 0;
    do {
        sndMidiTrackState.buffers[bufferIndex].address = iopAddress;
        iopAddress += sndMidiTrackState.buffers[bufferIndex].size;
        bufferIndex++;
    } while (bufferIndex != bufferCount);
}

void func_00341AD8(s32 unused, s32 header, s32 *bufferSizes, s32 bufferCount) {
    D_004391E0 = 1;
    sndLoadIopSoundModules();
    sceSifInitIopHeap();
    func_003415A8();
    sndMidiTrackState.iopBufferAddress = sndReserveIopWorkMemory(0x4000);
    sndMidiTrackState.iopBufferBytes = 0x4000;
    sndInitializeChannelAndTrackState(unused, header);
    sndAllocateAlignedIopBuffers(bufferSizes, bufferCount);
    D_004391EC = sndReserveIopWorkMemory(0x8D0);
    sndMidiTrackState.opaque1F8 = (u32)&sndMidiTrackState;
    sndMidiTrackState.opaque1FC = 0x8D0;
    sndCopyToIopAndWait(D_004391EC, &sndMidiTrackState, 0x8D0);
    sndSendCommandPacket(0, 0, &D_004391EC, 4);
    D_004391E0 = 0;
}

/* Starts a track with the default 0x7f setting after preparing its identifier. */

void sndStartTrackDefault(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    sndQueueCommandPacket(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */

void sndSendSingleWordControlPacket(u32 value) {
    u32 packet[4];

    packet[0] = value;
    sndQueueCommandPacket(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */

void sndStartTrackAlternate(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    sndQueueCommandPacket(0x130, 0, &packet, 0x10);
}

void func_00341C78(u32 value) {
    u32 packet[4];

    packet[0] = value;
    sndQueueCommandPacket(0x30, 0, packet, 0x10);
}
