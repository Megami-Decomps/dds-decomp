#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "snd_queued_command_dispatch_api.h"
#include "snd_ring.h"
#include "common.h"
#include "snd_track_playback_api.h"
#include "snd_command_packet.h"
#include "snd_work.h"
#include "sdf_named_resource_request_api.h"
#include "sce_io.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_alarm.h"
#include "mnu_sound_resource_path_api.h"
#include "snd_midi_state_views.h"
#include "snd_file_request_state.h"
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

void p2sdLoadSMGFile(s32 trackId);

/* Sound command work block: header, 16 channels/buffers, 13 track slots. */
extern SndWork sdfSndMap;
extern u32 sndReserveIopWorkMemory(s32 size);
extern s8 sdfSndBurstReqMode;
extern s32 D_004391EC;
extern s32 sndStartIopCommandAndRpcWorkers(void);
extern void sdfSndTransToIOP(s32, const void *, s32);

extern SndRingPacket sndCommandRing[SND_COMMAND_RING_ENTRY_COUNT];
extern u32 D_00438B90;
/* The worker consumes entries while this producer publishes and polls cursors. */
extern vu16 sdfSndCmdIndex;
extern vu16 sdfSndCmdTailIndex;
extern s32 sdfSndCmdBuf;
u32 sdfSndSendCmd(u32 command, u32 channel, const void *packet, s32 payloadBytes) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    SndRingPacket *entry;
    u32 transferQuadwords;

    if (++D_00438B90 == 0) {
        D_00438B90 = 1;
    }
    nextWriteIndex = ((s16)sdfSndCmdTailIndex + 1) & SND_COMMAND_RING_INDEX_MASK;
    if (nextWriteIndex == (s16)sdfSndCmdIndex) {
        for (wakeAttemptsRemaining = 8; wakeAttemptsRemaining != 0; wakeAttemptsRemaining--) {
            do {
                WakeupThread(sdfSndCmdBuf);
            } while ((s16)sdfSndCmdIndex != nextWriteIndex);
        }
        return 0;
    }
    entry = &sndCommandRing[(s16)sdfSndCmdTailIndex];
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
    sdfSndCmdTailIndex = nextWriteIndex;
    if (sdfSndBurstReqMode != 0) {
        while (sndTryDispatchQueuedCommand() != SND_QUEUED_COMMAND_DISPATCH_RESULT_SENT) {
        }
    } else {
        WakeupThread(sdfSndCmdBuf);
    }
    return D_00438B90;
}

/* Ignore the post-queue sequence; this native entry does not wait or update state. */
void sdfSndSyncCmd(u32 unused) {
}

u32 sdfSndSendSyncCmd(u32 command, u32 channel, const void *packet, u32 size) {
    u32 result = sdfSndSendCmd(command, channel, packet, size);

    sdfSndSyncCmd(result);
    return result;
}

/* Descriptor consumed by sceSifSetDma: EE source, IOP destination, byte size, attributes. */
extern SifDmaTransfer sdfSndTransBuf;

/* Transfer an EE buffer to IOP memory, retrying submission and waiting for completion. */
void sdfSndTransToIOP(s32 destination, const void *source, s32 size) {
    s32 dmaId;
    s32 delay;

    sdfSndTransBuf.source = (u32)source;
    sdfSndTransBuf.destination = destination;
    sdfSndTransBuf.size = size;
    sdfSndTransBuf.attributes = 0;
    FlushCache(0);
    while ((dmaId = sceSifSetDma(&sdfSndTransBuf, 1)) == 0) {
        /* Busy-wait before retrying a full SIF DMA queue. */
        delay = 0x186A0;
        while (delay != 0) {
            delay -= 0x271;
            __asm__ volatile ("nop");
        }
    }
    while (sceSifDmaStat(dmaId) >= 0) {
    }
}

extern u8 sdfHddValid;

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
    if (sdfHddValid != 0 &&
        sceGetstat("pfs:/userirx/SDFSDMAN.IRX", &info) == 0 &&
        (info.mode & 0xF000) == 0x2000) {
        module = "pfs:/userirx/SDFSDMAN.IRX";
    }
    SdfIopFileRpc(module, 0, NULL, NULL);
}

void sndInitializeChannelAndTrackState(s32 unused, s32 header) {
    SndChannel *channel;
    s32 i;

    sdfSndMap.header = header;
    channel = sdfSndMap.channels;
    for (i = 0; i < SND_CHANNEL_COUNT; i++) {
        channel->index = i;
        channel->unk1 = 0x20;
        channel->unk2 = 0;
        channel->iopWorkAddress = 0;
        channel->unk4 = 0;
        channel->unkC = 0;
        channel++;
    }
    sdfSndMap.channels[0].iopWorkAddress = sndReserveIopWorkMemory(0x3B600);
    sdfSndMap.opaque200 = 0;
    sdfSndMap.opaque204 = 0;
    for (i = 0; i < SND_TRACK_SLOT_COUNT; i++) {
        sdfSndMap.slots[i].id = -1;
        sdfSndMap.slots[i].balanceFirstComponent = 0;
        sdfSndMap.slots[i].balanceSecondComponent = 0;
    }
}

/* Allocate one aligned IOP block and distribute its address among active buffers. */
void sndAllocateAlignedIopBuffers(s32 *bufferSizes, s32 bufferCount) {
    SndIopBuffer *buffer;
    s32 totalBytes;
    s32 alignedBytes;
    s32 bufferIndex;
    u32 iopAddress;

    sdfSndMap.bufferCount = bufferCount;
    buffer = sdfSndMap.buffers;
    totalBytes = 0;
    bufferIndex = 0;
    do {
        alignedBytes = (*bufferSizes++ + (SND_IOP_BUFFER_ALIGNMENT - 1)) &
                       ~(SND_IOP_BUFFER_ALIGNMENT - 1);
        buffer->size = alignedBytes;
        buffer++;
        totalBytes += alignedBytes;
        bufferIndex++;
    } while (bufferIndex != bufferCount);
    for (; bufferIndex < SND_IOP_BUFFER_COUNT; bufferIndex++) {
        sdfSndMap.buffers[bufferIndex].size = 0;
        sdfSndMap.buffers[bufferIndex].iopAddress = 0;
    }
    iopAddress = sndReserveIopWorkMemory(totalBytes);
    bufferIndex = 0;
    do {
        sdfSndMap.buffers[bufferIndex].iopAddress = iopAddress;
        iopAddress += sdfSndMap.buffers[bufferIndex].size;
        bufferIndex++;
    } while (bufferIndex != bufferCount);
}

void sndInitializeIopSoundSystem(s32 unused, s32 header, s32 *bufferSizes, s32 bufferCount) {
    sdfSndBurstReqMode = 1;
    sndLoadIopSoundModules();
    sceSifInitIopHeap();
    sndStartIopCommandAndRpcWorkers();
    sdfSndMap.iopBufferAddress = sndReserveIopWorkMemory(0x4000);
    sdfSndMap.iopBufferBytes = 0x4000;
    sndInitializeChannelAndTrackState(unused, header);
    sndAllocateAlignedIopBuffers(bufferSizes, bufferCount);
    D_004391EC = sndReserveIopWorkMemory(0x8D0);
    sdfSndMap.eeWorkAddress = &sdfSndMap;
    sdfSndMap.eeWorkByteCount = 0x8D0;
    sdfSndTransToIOP(D_004391EC, &sdfSndMap, 0x8D0);
    sdfSndSendSyncCmd(0, 0, &D_004391EC, 4);
    sdfSndBurstReqMode = 0;
}

/* Starts a track with the default 0x7f setting after preparing its identifier. */

void p2sdPlayBGM(s32 trackId) {
    CmdPacket packet;

    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */

void p2sdSendStopCmd(u32 controlWord) {
    u32 packet[4];

    packet[0] = controlWord;
    sdfSndSendCmd(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */

void p2sdResumeBgm(s32 trackId) {
    CmdPacket packet;

    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x130, 0, &packet, 0x10);
}

void p2sdStopSe(u32 soundId) {
    u32 packet[4];

    packet[0] = soundId;
    sdfSndSendCmd(0x30, 0, packet, 0x10);
}

void p2sdLoadSMGFile(s32 packedTrackId);

void p2sdStopAllBgm(void) {
    sdfSndSendCmd(0x1a0, 0, 0, 0);
}

void sndQueueNoPayloadCommand210(void) {
    sdfSndSendCmd(0x210, 0, 0, 0);
}

void p2sdStopAllSe(void) {
    sdfSndSendCmd(0x40, 0, 0, 0);
}

/* Dispatch the command family selected by the caller, without a payload. */
void sndDispatchCommandWithoutPayload(u32 command) {
    sdfSndSendCmd(command | 0x50, 0, 0, 0);
}


/* Send a NUL-terminated command string; the transport excludes the terminator. */
u32 sdfSndLoadHDFile(s32 command, char *text) {
    u32 length = strlen(text);

    return sdfSndSendSyncCmd(command | 0x70, 0, text, length);
}

u32 sdfSndLoadBDFile(s32 command, char *text) {
    u32 length = strlen(text);

    return sdfSndSendSyncCmd(command | 0x60, 0, text, length);
}

/* Send text with command bit 0x80; payload length excludes its NUL terminator. */
u32 sdfSndLoadSQFile(s32 command, char *text) {
    s32 cmd = command;
    u32 len = strlen(text);
    return sdfSndSendSyncCmd(cmd | 0x80, 0, text, len);
}

/* Send a 12-byte sequence command; the trailing struct word is not transmitted. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan) {
    SndSequenceVolumePanPacket packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.volume = volume;
    packet.pan = (u8)pan;
    sdfSndSendCmd(0x90, 0, &packet, 0xC);
}

/* Map a wrapped listener-relative angle and distance onto sequence volume/pan. */
void sndSetSpatialSequenceVolumePan(s32 trackId, f32 angle, f32 distance) {
    s32 degrees = (s32)angle % 360;
    s32 side = 1;
    s32 pan;
    f32 volume;

    if (degrees > 180) {
        degrees -= 360;
    }
    if (degrees < -180) {
        degrees += 360;
    }
    if (degrees < 0) {
        degrees = -degrees;
        side = -1;
    }
    volume = 1.0f;
    if (degrees < 45) {
        pan = 63 - (s32)(degrees * 0.71111f) * side;
    } else if (degrees <= 90) {
        pan = 63 - (s32)(degrees * 0.7f) * side;
        volume = 1.0f - (degrees - 45) * 0.004444f;
    } else {
        volume = 0.75f - (degrees - 90) * 0.005556f;
        pan = 63 - (s32)((180 - degrees) * 0.7f) * side;
    }
    if (distance > 1.0f) {
        volume /= distance * 0.333f + 1.0f;
    }
    p2sdPlaySE(trackId, (s32)(volume * 127.0f), pan);
}

void p2sdLoadSMGFile(s32 trackId);

extern SndTrackSlot LastLoad[];

extern SndPackedTrackLoadStatus sndFindPackedTrackLoadStatus(s32 packedTrack);

extern s32 p2sdSmgHandle;

extern u8 D_0047AA50[];

extern u32 D_004391D0;

extern void InvalidDCache(void *a0, void *a1);

static inline u32 sndSendCommand(u32 command, const void *packet, u32 size) {
    return sdfSndSendSyncCmd(command, 0, packet, size);
}

/* Converts world coordinates to the sound engine's one-tenth scale. */
void p2sdPlaySe3DEx(s32 trackId, s32 radiusInHundreds, f32 worldX, f32 worldY, f32 worldZ) {
    u32 packet[8];

    packet[0] = trackId;
    packet[1] = radiusInHundreds;
    packet[2] = (s32)(worldX * 0.1f);
    packet[3] = (s32)(worldY * 0.1f);
    packet[4] = (s32)(worldZ * 0.1f);
    sdfSndSendSyncCmd(0x170, 0, packet, 0x20);
}

extern SndListenerState sndCachedListenerState;

/* Scale six world-space values and send them to the sound engine when they changed. */
void p2sdSetStandPos(f32 firstPointX, f32 firstPointY, f32 firstPointZ,
                                 f32 secondPointX, f32 secondPointY, f32 secondPointZ) {
    SndListenerState state;

    state.scaledValues[0] = firstPointX * 0.1f;
    state.scaledValues[1] = firstPointY * 0.1f;
    state.scaledValues[2] = firstPointZ * 0.1f;
    state.scaledValues[3] = secondPointX * 0.1f;
    state.scaledValues[4] = secondPointY * 0.1f;
    state.scaledValues[5] = secondPointZ * 0.1f;
    if (sndCachedListenerState.scaledValues[0] != state.scaledValues[0] || sndCachedListenerState.scaledValues[1] != state.scaledValues[1] ||
        sndCachedListenerState.scaledValues[2] != state.scaledValues[2] || sndCachedListenerState.scaledValues[3] != state.scaledValues[3] ||
        sndCachedListenerState.scaledValues[4] != state.scaledValues[4] || sndCachedListenerState.scaledValues[5] != state.scaledValues[5]) {
        sdfSndSendSyncCmd(0x160, 0, &state, 0x20);
        sndCachedListenerState = state;
    }
}

/* Prefer a resident slot, then report the single bank request tracked as pending. */
SndPackedTrackLoadStatus sndFindPackedTrackLoadStatus(s32 packed) {
    SndWork *work = &sdfSndMap;
    s32 id = packed >> 16;
    SndTrackSlot *slot;
    s32 i;

    InvalidDCache(work, (u8 *)work + 0x8D0);
    slot = work->slots;
    for (i = 0; i < 13; i++) {
        if (slot->id == id) {
            return SND_PACKED_TRACK_LOAD_RESIDENT;
        }
        slot++;
    }
    return p2sdSmgHandle == id ? SND_PACKED_TRACK_LOAD_PENDING
                               : SND_PACKED_TRACK_LOAD_NOT_PRESENT;
}

extern s32 (*p2sdSmgBuf)(void);

/* Make sure the MIDI bank named by the packed track id is resident, loading it if not. */
void p2sdLoadSMGFile(s32 packed) {
    char name[0x10];
    char path[0x100];
    SndPackedTrackLoadStatus status = sndFindPackedTrackLoadStatus(packed);
    s32 id;

    switch (status) {
    case SND_PACKED_TRACK_LOAD_NOT_PRESENT:
        id = packed >> 16;
        sprintf(name, "MIDI%04X.SMG", id);
        mnuBuildSoundResourcePath(path, name);
        sdfSndSendSyncCmd(0xA0, 0, path, strlen(path) + 1);
        p2sdSmgHandle = id;
        break;
    case SND_PACKED_TRACK_LOAD_RESIDENT:
        break;
    case SND_PACKED_TRACK_LOAD_PENDING:
        do {
            sdfDelayThread(2);
            if (p2sdSmgBuf != NULL) {
                p2sdSmgBuf();
            }
        } while (sndFindPackedTrackLoadStatus(packed) != SND_PACKED_TRACK_LOAD_RESIDENT);
        break;
    }
}

u32 sndSendFilenameCommand(char *filename) {
    u32 length = strlen(filename);
    return sndSendCommand(0xA0, filename, length);
}

extern SndFileRequestState D_0047B310;
void p2sdSetPcmBuffer(const void *data, s32 size) {
    u32 *packet;

    D_0047B310.size = size;
    D_0047B310.pending = 1;
    D_0047B310.data = (u32)data;
    if (data != 0 && size > 0) {
        packet = &D_004391D0;
        *packet = (u32)data;
        sdfSndSendSyncCmd(0x1B0, 0, packet, 4);
    }
    p2sdSmgHandle = -1;
}

u32 sndSendChannelControlCommand(u32 channel) {
    return sndSendCommand((channel & 0xF) | 0x1C0, NULL, 0);
}

/* Channel-group command packets: the group is bits 3-6 of the channel byte. */
u32 sndSendChannelGroupCommand140(u8 channel) {
    return sndSendCommand(((channel >> 3) & 0xF) | 0x140, 0, 0);
}

u32 sndSendChannelGroupCommand150(u8 channel) {
    return sndSendCommand(((channel >> 3) & 0xF) | 0x150, 0, 0);
}

void sndReleaseMidiTrack(s32 id) {
    u32 packet[4];
    if (sndFindPackedTrackLoadStatus(id) != SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
        packet[0] = id;
        sdfSndSendSyncCmd(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (p2sdSmgHandle == id) {
            p2sdSmgHandle = -1;
        }
    }
}

/* Read the byte used as the minuend by the nonnegative balance query. */
u8 sndGetTrackBalanceFirstComponent(s32 trackSlotIndex) {
    return LastLoad[trackSlotIndex].balanceFirstComponent;
}

/* Read the byte subtracted by the nonnegative balance query. */
u8 sndGetTrackBalanceSecondComponent(s32 trackSlotIndex) {
    return LastLoad[trackSlotIndex].balanceSecondComponent;
}

s32 sndGetNonnegativeEntryBalance(s32 trackSlotIndex) {
    SndTrackSlot *entry = &LastLoad[trackSlotIndex];
    s32 difference = entry->balanceFirstComponent - entry->balanceSecondComponent;

    if (difference <= 0) {
        difference = 0;
    }
    return difference;
}

/* Return the MIDI channel-array address, retaining this unit's existing return type. */
u8 *sndGetMidiChannelTable(void) {
    return D_0047AA50;
}

SndTrackSlot *sndGetTrackSlotTable(void) {
    return LastLoad;
}

/* Return one opaque MIDI word and optionally copy its adjacent word. */
u32 sndGetMidiStateWordPair(u32 *outSecondaryValue) {
    if (outSecondaryValue != NULL) {
        *outSecondaryValue = sdfSndMap.opaque20C;
    }
    return sdfSndMap.opaque208;
}

/* Pack two 24-byte MIDI state groups into out; NULL leaves the state untouched. */
void sndCopyGroupedMidiStateBytes(u8 *out) {
    s32 group;
    u8 *state;

    if (out == NULL) {
        return;
    }
    state = (u8 *)&sdfSndMap;
    for (group = 0; group < 2; group++) {
        s32 i;

        for (i = 0; i < 24; i++) {
            *out++ = state[0x890 + group * 0x20 + i];
        }
    }
}

/* Sends a prepared track identifier with the 0x17f setting. */

void p2sdPlayBGMFade(s32 trackId) {
    CmdPacket packet;

    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x17F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}

void p2sdStopBGM(u32 trackHandle) {
    u32 packet[4];

    packet[0] = trackHandle;
    sdfSndSendCmd(0xd0, 0, packet, 0x10);
}

INCLUDE_SDATA(const s32, "game/code_00341650", p2sdSmgHandle);

