#include "sdk_ps2.h"
#include "snd_queued_command_dispatch_api.h"
#include "snd_ring.h"
#include "common.h"
#include "eff_random.h"
#include "eff_random_mod.h"
#include "clock_time.h"
#include "sdf_cd_clock.h"
#include "sdf_kernel_thread_param.h"
#include "snd_packed_track_load_status.h"
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
#include "file_manager_api.h"
#include "eeregs.h"
#include "mnu_voice_resource_path_api.h"
#include "ee_mmi.h"
#include "sdf_resource.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_reloc.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_semaphore_api.h"
#include "sdf_movie_stream.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "sdf_texture_lists.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "sdf_stream_read.h"
#include "sdf_stream_read_api.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf_texture_offset_list.h"
#include "sdf_texture_file.h"
#include "sdf_texture_queue.h"
#include "sdf_stream_input_dma.h"
#include "sdf_sound_rpc_records.h"
#include "sdf_handler_node.h"
#include "mnu_title_sound.h"
#include "sdf_movie_draw_packet.h"

extern s8 sdfSndBurstReqMode;

u32 sndReserveIopWorkMemory(s32 size) {
    return (u32)sceSifAllocIopHeap(size);
}

extern SndRingPacket sndCommandRing[32];

/* Shared with the producer and its worker-thread wakeups. */
extern vu16 sdfSndCmdIndex;

extern vu16 sdfSndCmdTailIndex;

SndQueuedCommandDispatchResult sndTryDispatchQueuedCommand(void) {
    s32 readIndex = (s16)sdfSndCmdIndex;
    SndRingPacket *packet;

    if (readIndex == (s16)sdfSndCmdTailIndex) {
        return SND_QUEUED_COMMAND_DISPATCH_RESULT_NOT_SENT;
    }
    packet = &sndCommandRing[readIndex];
    if (sceSifSendCmd(0, packet, (packet->command >> 24) & 0xf0, 0, 0, 0) == 0) {
        return SND_QUEUED_COMMAND_DISPATCH_RESULT_NOT_SENT;
    }
    readIndex = (readIndex + 1) & 31;
    sdfSndCmdIndex = readIndex;
    return SND_QUEUED_COMMAND_DISPATCH_RESULT_SENT;
}

void sdfSndCmdIFThread(void) {
    for (;;) {
        do {
            SleepThread();
        } while (sdfSndBurstReqMode != 0);
        while (sndTryDispatchQueuedCommand() == SND_QUEUED_COMMAND_DISPATCH_RESULT_SENT) {
        }
    }
}

extern SdfKernelThreadParam sndCommandThreadParameters;

extern u8 sndCommandThreadStack[];

extern u8 sndRpcServerThreadStack[];

extern void *_gp;

extern s32 sdfSndCmdBuf;

extern s32 D_003BD4A4;

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);

extern void SifRpcServerThreadProc(void);

s32 sndStartIopCommandAndRpcWorkers(void) {
    s32 threadId;

    while (sceSifGetSreg(31) == 0) {
    }

    sndCommandThreadParameters.entry = sdfSndCmdIFThread;
    sndCommandThreadParameters.stack = sndCommandThreadStack;
    sndCommandThreadParameters.stackSize = 0x1000;
    sndCommandThreadParameters.priority = 0x42;
    sndCommandThreadParameters.gp = &_gp;
    sdfSndCmdIndex = 0;
    sdfSndCmdTailIndex = 0;
    threadId = CreateThread(&sndCommandThreadParameters);
    sdfSndCmdBuf = threadId;
    _StartThread(threadId, 0);
    threadId = CreateThreadSimple(SifRpcServerThreadProc, sndRpcServerThreadStack, 0x1000, 0x43);
    D_003BD4A4 = threadId;
    return _StartThread(threadId, 0);
}

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

/* Sound command work block: header, 16 channels/buffers, 13 track slots (see code_002E9140). */
extern SndWork sdfSndMap;

extern u32 sndReserveIopWorkMemory(s32 size);

extern s32 D_003BDA8C;

extern s32 sndStartIopCommandAndRpcWorkers(void);

extern void sdfSndTransToIOP(s32, const void *, s32);

extern SndRingPacket sndCommandRing[SND_COMMAND_RING_ENTRY_COUNT];

extern u32 D_003BD4A0;

/* The worker consumes entries while this producer publishes and polls cursors. */
extern vu16 sdfSndCmdIndex;

u32 sdfSndSendCmd(u32 command, u32 channel, const void *packet, s32 payloadBytes) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    SndRingPacket *entry;
    u32 transferQuadwords;

    if (++D_003BD4A0 == 0) {
        D_003BD4A0 = 1;
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
    entry->sequence = D_003BD4A0;
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
    return D_003BD4A0;
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

void sndInitializeChannelAndTrackState(char *unused, s32 header) {
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
    sdfSndMap.channels[0].iopWorkAddress = sndReserveIopWorkMemory(0x21600);
    sdfSndMap.opaque200 = 0;
    sdfSndMap.opaque204 = 0;
    for (i = 0; i < SND_TRACK_SLOT_COUNT; i++) {
        sdfSndMap.slots[i].id = -1;
        sdfSndMap.slots[i].balanceFirstComponent = 0;
        sdfSndMap.slots[i].balanceSecondComponent = 0;
    }
}

/* Allocate one aligned IOP block and distribute its address among active buffers. */
void sndAllocateAlignedIopBuffers(const s32 *bufferSizes, s32 bufferCount) {
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

void sndInitializeIopSoundSystem(char *unused, s32 header, const s32 *bufferSizes, s32 bufferCount) {
    sdfSndBurstReqMode = 1;
    sndLoadIopSoundModules();
    sceSifInitIopHeap();
    sndStartIopCommandAndRpcWorkers();
    sdfSndMap.iopBufferAddress = sndReserveIopWorkMemory(0x4000);
    sdfSndMap.iopBufferBytes = 0x4000;
    sndInitializeChannelAndTrackState(unused, header);
    sndAllocateAlignedIopBuffers(bufferSizes, bufferCount);
    D_003BDA8C = sndReserveIopWorkMemory(0x8D0);
    sdfSndMap.eeWorkAddress = &sdfSndMap;
    sdfSndMap.eeWorkByteCount = 0x8D0;
    sdfSndTransToIOP(D_003BDA8C, &sdfSndMap, 0x8D0);
    sdfSndSendSyncCmd(0, 0, &D_003BDA8C, 4);
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
    u32 length = strlen(text);

    return sdfSndSendSyncCmd(command | 0x80, 0, text, length);
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

extern SndPackedTrackLoadStatus sndFindPackedTrackLoadStatus(s32 packedTrack);

extern s32 p2sdSmgHandle;

/* Eight-byte view retained for the separate MIDI channel table. */
typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
} SndTrackVolume;

extern void InvalidDCache(void *dst, void *src);

extern SndTrackVolume D_003FE0D0[];

extern SndTrackSlot LastLoad[];

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
    s32 slotIndex;

    InvalidDCache(work, (u8 *)work + 0x8D0);
    slot = work->slots;
    for (slotIndex = 0; slotIndex < 13; slotIndex++) {
        if (slot->id == id) {
            return SND_PACKED_TRACK_LOAD_RESIDENT;
        }
        slot++;
    }
    return p2sdSmgHandle == id ? SND_PACKED_TRACK_LOAD_PENDING
                               : SND_PACKED_TRACK_LOAD_NOT_PRESENT;
}

extern FileManagerQueuedRequestResult (*p2sdSmgBuf)(void);

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
        /* The queue copies this stack path before returning. */
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

extern SndFileRequestState D_003FE990;

extern u32 D_003BDA70;

void p2sdSetPcmBuffer(const void *data, s32 size) {
    u32 *packet;

    D_003FE990.size = size;
    D_003FE990.pending = 1;
    D_003FE990.data = (u32)data;
    if (data != 0 && size > 0) {
        packet = &D_003BDA70;
        *packet = (u32)data;
        sdfSndSendSyncCmd(0x1B0, 0, packet, 4);
    }
    p2sdSmgHandle = -1;
}

u32 sndSendChannelControlCommand(u32 channel) {
    return sndSendCommand((channel & 0xF) | 0x1C0, NULL, 0);
}

/* Channel-group command packets: the group is bits 3-6 of the channel byte. */
/* Submit opcode 0x140 for the selected channel group, without a payload. */
u32 sndSendChannelGroupCommand140(u8 channel) {
    return sndSendCommand(((channel >> 3) & 0xF) | 0x140, 0, 0);
}

/* Submit opcode 0x150 for the selected channel group, without a payload. */
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
SndTrackVolume *sndGetMidiChannelTable(void) {
    return D_003FE0D0;
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
        s32 byteIndex;

        for (byteIndex = 0; byteIndex < 24; byteIndex++) {
            *out++ = state[0x890 + group * 0x20 + byteIndex];
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

extern u32 p2sdPcmBusy;

extern u32 p2sdPcmState;

extern char D_00398948[];

extern s32 sifRpcServerSema;

extern u32 D_003FEAC0[16];

extern s32 sdfPrintf(const char *fmt, ...);

extern char D_003B4880[];

extern char D_003B48A0[];

extern char D_003B48B8[];

extern char D_003B48E0[];

extern char D_003B4900[];

extern char *D_00398968[];

extern SdfSoundCommand D_003BD494;

void p2sdBgmVolDown(void) {
    sdfSndSendCmd(0x180, 0, 0, 0);
}

void p2sdBgmVolUp(void) {
    sdfSndSendCmd(400, 0, 0, 0);
}

void p2sdSetBgmTrack(s32 channel) {
    sdfSndSendCmd(((channel + 1U) & 0xf) | 0xe0, 0, 0, 0);
}

s32 p2sdLoadPcmFile(const char *name, u8 channel) {
    if (p2sdPcmState != MNU_TITLE_SOUND_IDLE) {
        return 1;
    }
    strcpy(D_00398948, name);
    sdfSndSendSyncCmd((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

MnuTitleSoundBusyState p2sdGetPcmState(void) {
    return p2sdPcmState;
}

void p2sdSyncSound(void) {
    sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

void sndSendChannelGroupCommand110(u8 channel) {
    sdfSndSendSyncCmd(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

void sdfSoundSetChannelCount(u32 channelCount) {
    if (0x10 < channelCount) {
        channelCount = 0x10;
    }
    if (channelCount == 0) {
        channelCount = 1;
    }
    sdfSndSendSyncCmd((channelCount - 1) | 0x1d0, 0, 0, 0);
}

SdfSoundReadCallback p2sdSetPcmCallback(SdfSoundReadCallback callback) {
    if (p2sdPcmBusy != 0) {
        return NULL;
    }
    D_003BD494.read = callback;
    return callback;
}

/* Send opcode 0x120 with the channel's group (bits 3-6) unless a command is
 * pending; returns nonzero when busy. */
s32 p2sdPlayPcm(u8 channel) {
    if (p2sdPcmBusy) {
        return 1;
    }
    channel = (channel >> 3) & 0xF;
    sdfSndSendSyncCmd(0x120 | channel, 0, 0, 0);
    return 0;
}

u32 sdfSoundGetCommandStatus(void) {
    return p2sdPcmBusy;
}

/* Stop the movie's named sound playback after its stream is released. */
void p2sdSyncMovie(void) {
    sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

/* Separate native copy of the command-0x110 channel-group sender. */
void sndSendChannelGroupCommand110Duplicate(u8 channel) {
    sdfSndSendSyncCmd(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

s32 sdfFileRpcCallback(s32 unused, SdfDevEvent event) {
    switch (event) {
    case 1:
        break;
    case SDF_DEV_EVENT_SEEK_REPLY:
        break;
    case SDF_DEV_EVENT_WRITE_REPLY:
        break;
    case SDF_DEV_EVENT_READ_REPLY:
        FlushCache(0);
        /* Fall through: flush and wake the waiting thread. */
    case SDF_DEV_EVENT_SIZE_REPLY:
        SignalSema(sifRpcServerSema);
        break;
    case SDF_DEV_EVENT_INACTIVE:
    case SDF_DEV_EVENT_OPENED:
    case SDF_DEV_EVENT_CLOSED:
        SignalSema(sifRpcServerSema);
        break;
    }
    return 0;
}

extern SdfSoundRpcRequest *D_003BDA74;

extern s32 D_003BDA78;

extern sdfFileCmdHandle_t *D_003BDA7C;

extern SdfSoundResidentBuffer D_003FEA98;

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void *callback, void *callbackContext);

/* Handle sound-file/buffer RPC commands and return the shared reply words. */
u32 *sdfFileRpcHandleCmd(u32 command, SdfSoundRpcRequest *request) {
    char path[0x100];

    switch (command) {
    case SDF_SOUND_RPC_OPEN_VOICE_FILE: {
        sdfFileCmdHandle_t *state;
        if (D_00398948[0] == 0) {
            D_003FEAC0[0] = 0;
            break;
        }
        p2sdPcmState = MNU_TITLE_SOUND_BUSY;
        mnuBuildVoiceResourcePath(path, D_00398948);
        state = sdfFileCmdReadOpen(path, sdfFileRpcCallback, 0);
        D_003BDA7C = state;
        if (state != NULL) {
            WaitSema(sifRpcServerSema);
            state = D_003BDA7C;
            if (state->result != 0) {
                sdfFileCmdClearError(state);
                sdfFileCmdFree(D_003BDA7C);
                D_003BDA7C = NULL;
                state = NULL;
            }
        }
        D_003FEAC0[0] = (u32)state;
        break;
    }
    case SDF_SOUND_RPC_READ_VOICE_FILE: {
        sdfMemHandle_t *allocation;
        void *buffer;
        D_003BDA74 = request;
        allocation = sdfMemAllocDirect(request->byteCount);
        buffer = (void *)sdfMemLock(allocation);
        if (buffer != NULL) {
            sdfFileCmdRead(D_003BDA7C, buffer, D_003BDA74->byteCount);
            WaitSema(sifRpcServerSema);
            sdfSndTransToIOP(D_003BDA74->destination, buffer, D_003BDA74->byteCount);
            sdfMemFreeDirect(allocation);
            D_003FEAC0[0] = D_003BDA74->byteCount;
        } else {
            D_003FEAC0[0] = 0;
        }
        break;
    }
    case SDF_SOUND_RPC_CLOSE_VOICE_FILE:
        sdfFileCmdClose(D_003BDA7C);
        WaitSema(sifRpcServerSema);
        sdfFileCmdFree(D_003BDA7C);
        D_003BDA7C = NULL;
        p2sdPcmState = MNU_TITLE_SOUND_IDLE;
        D_003FEAC0[0] = 0;
        break;
    case SDF_SOUND_RPC_RESET_RESIDENT_BUFFER_OFFSET:
        D_003BDA78 = 0;
        D_003FEAC0[0] = 0;
        break;
    case SDF_SOUND_RPC_READ_RESIDENT_BUFFER: {
        s32 bytes;
        D_003BDA74 = request;
        sdfSndTransToIOP(request->destination, D_003FEA98.data + D_003BDA78, request->byteCount);
        bytes = D_003BDA74->byteCount;
        D_003FEAC0[0] = bytes;
        D_003BDA78 += bytes;
        break;
    }
    case 5:
        D_003FEAC0[0] = 0;
        break;
    case SDF_SOUND_RPC_GET_PCM_CALLBACK_WORD: {
        u32 word = D_003BD494.word;
        if (word != 0) {
            p2sdPcmBusy = 1;
        }
        D_003FEAC0[0] = word;
        break;
    }
    case SDF_SOUND_RPC_READ_PCM_CALLBACK_BUFFER: {
        const u8 *buffer;
        D_003BDA74 = request;
        buffer = D_003BD494.read(request->byteCount);
        if (buffer != NULL) {
            sdfSndTransToIOP(D_003BDA74->destination, buffer, D_003BDA74->byteCount);
            D_003FEAC0[0] = D_003BDA74->byteCount;
        } else {
            D_003FEAC0[0] = -1;
        }
        break;
    }
    case SDF_SOUND_RPC_CLEAR_PCM_CALLBACK_BUSY:
        p2sdPcmBusy = 0;
        D_003FEAC0[0] = 0;
        break;
    }
    return D_003FEAC0;
}

void SifRpcServerThreadProc(void) {
    u8 queue[0x20];
    u8 server[0x50];
    s32 semaphore = sdfCreateSema(0, 1, 0);
    sifRpcServerSema = semaphore;
    if (semaphore <= 0) {
        for (;;) {
        }
    }
    sceSifSetRpcQueue(queue, GetThreadId());
    sceSifRegisterRpc(server, 0x54524E53, sdfFileRpcHandleCmd, D_003FEAC0, 0, 0, queue);
    sceSifRpcLoop(queue);
}

extern struct SndWork sdfSndMap;

void sdfSoundSetTableEntry(u32 kind, u8 *src) {
    u8 *table;
    u8 *dst;
    s32 i;

    switch (kind) {
    case 0:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x210;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 1:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x290;
        for (i = 0; i < 0x80; i++) {
            *dst++ = src[i];
        }
        break;
    case 2:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x310;
        for (i = 0; i < 0x80; i++) {
            *dst++ = src[i];
        }
        break;
    case 3:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x390;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 4:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x410;
        for (i = 0; i < 0x20; i++) {
            *dst++ = src[i];
        }
        break;
    case 5:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x490;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 6:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x510;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 7:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x590;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 8:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x610;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 9:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x690;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 10:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x710;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 11:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x790;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    case 12:
        table = (u8 *)&sdfSndMap;
        dst = table + 0x810;
        for (i = 0; i < 2; i++) {
            *dst++ = src[i];
        }
        break;
    }
    sdfSndTransToIOP(D_003BDA8C, &sdfSndMap, 0x8D0);
    sdfSndSendSyncCmd(0x1F0, 0, 0, 0);
}

void sdfSoundGetTableEntry(u32 kind, u8 *dst) {
    u8 *table;
    u8 *src;
    s32 i;

    sdfSndSendSyncCmd(0x200, 0, 0, 0);
    switch (kind) {
    case 0:
        table = (u8 *)&sdfSndMap;
        src = table + 0x210;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 1:
        table = (u8 *)&sdfSndMap;
        src = table + 0x290;
        for (i = 0; i < 0x80; i++) {
            dst[i] = *src++;
        }
        return;
    case 2:
        table = (u8 *)&sdfSndMap;
        src = table + 0x310;
        for (i = 0; i < 0x80; i++) {
            dst[i] = *src++;
        }
        return;
    case 3:
        table = (u8 *)&sdfSndMap;
        src = table + 0x390;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 4:
        table = (u8 *)&sdfSndMap;
        src = table + 0x410;
        for (i = 0; i < 0x20; i++) {
            dst[i] = *src++;
        }
        return;
    case 5:
        table = (u8 *)&sdfSndMap;
        src = table + 0x490;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 6:
        table = (u8 *)&sdfSndMap;
        src = table + 0x510;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 7:
        table = (u8 *)&sdfSndMap;
        src = table + 0x590;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 8:
        table = (u8 *)&sdfSndMap;
        src = table + 0x610;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 9:
        table = (u8 *)&sdfSndMap;
        src = table + 0x690;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 10:
        table = (u8 *)&sdfSndMap;
        src = table + 0x710;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 11:
        table = (u8 *)&sdfSndMap;
        src = table + 0x790;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    case 12:
        table = (u8 *)&sdfSndMap;
        src = table + 0x810;
        for (i = 0; i < 0x2; i++) {
            dst[i] = *src++;
        }
        return;
    }
}

extern char D_003BD4F0[];

extern char D_003BD4F8[];

extern char D_003BD500[];

extern char D_003BD508[];

extern char D_003BD510[];

extern char D_003BD518[];

extern char D_003BD520[];

/* Print size bytes in address-aligned, sixteen-column hexadecimal rows. */
void sdfDumpHexLine(const u8 *data, s32 size) {
    s32 column = 0;
    s32 i;

    while (size > 0) {
        if (column == 0) {
            sdfPrintf(D_003BD4F0, data);
            column = (u32)data & 0xF;
            for (i = column; i > 0; i--) {
                sdfPrintf(D_003BD4F8);
            }
        } else if (column == 8) {
            sdfPrintf(D_003BD500);
        } else {
            sdfPrintf(D_003BD508);
        }
        sdfPrintf(D_003BD510, *data++);
        column++;
        if (column == 0x10) {
            column = 0;
            sdfPrintf(D_003BD518);
        }
        size--;
    }
    if (column == 0) {
        sdfPrintf(D_003BD518);
    } else {
        sdfPrintf(D_003BD520);
    }
}

/* Print a DMA chain, retaining the native two-entry CALL/RET stack limits. */
INCLUDE_RODATA(const s32, "sdf/sdfSound", D_003B4880);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_003B48A0);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_003B48B8);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_003B48E0);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_003B4900);

void sdfDumpDmaChain(SdfDmaTag *src) {
    SdfDmaTag *callStack[2];
    SdfDmaTag *cursor;
    u32 tag;
    u32 tagAddress;
    s32 tagType;
    s32 tagCount;
    s32 depth = 0;
    s32 done = 0;

    sdfPrintf(D_003B4880, src);
    cursor = src;
    for (;;) {
        if ((u32)cursor & 0xF) {
            sdfPrintf(D_003B48A0, cursor);
            return;
        }
        tag = ((u32 *)cursor)[0];
        tagAddress = cursor->address;
        tagType = (tag >> 28) & 7;
        tagCount = tag & 0xFFFF;
        sdfPrintf(D_003B48B8, cursor, D_00398968[tagType], tag, tagAddress, ((u32 *)cursor)[2], ((u32 *)cursor)[3]);
        cursor++;
        switch (tagType) {
        case 0:
            done = 1;
            break;
        case 1:
            cursor += tagCount;
            break;
        case 2:
            cursor = (SdfDmaTag *)tagAddress;
            break;
        case 3:
        case 4:
            break;
        case 5:
            if (depth == 2) {
                sdfPrintf(D_003B48E0);
                return;
            }
            callStack[depth++] = cursor;
            cursor = (SdfDmaTag *)tagAddress;
            break;
        case 6:
            if (depth == 0) {
                done = 1;
            } else {
                cursor = callStack[--depth];
            }
            break;
        case 7:
            cursor += tagCount;
            done = 1;
            break;
        }
        if (done != 0) {
            sdfPrintf(D_003B4900, cursor);
            return;
        }
    }
}

extern char D_003BD528[];

extern char D_003BD530[];

extern char D_003BD5E0[];

extern char D_003BD5E8[];

extern char *D_00398988[16];

extern char *D_003989C8[8];

extern s32 D_003989E8[16];

typedef s32 SdfGrFlushOtChainPhase;

enum {
    SDF_GR_FLUSH_OT_CHAIN_PHASE_READ_TAG = 0,
    SDF_GR_FLUSH_OT_CHAIN_PHASE_SETUP_TAG_DATA = 1,
    SDF_GR_FLUSH_OT_CHAIN_PHASE_DISPATCH_TAG = 2,
};

void sdfGrFlushOtChain(u32 sourceAddress) {
    SdfDmaTag *returnStack[2];
    SdfDmaTag *cursor;
    u32 *wordCursor = NULL;
    s32 dataMode = 0;
    s32 tagId = 0;
    s32 depth = 0;
    s32 n = 0;
    s32 directRow;
    SdfGrFlushOtChainPhase phase;
    s32 wordsLeft;
    u32 tagAddress;
    s32 qwc;
    s32 done;
    u32 value;

    sdfPrintf("<<< DMA-VIF dump start [%p] >>>\n", (void *)sourceAddress);
    cursor = (SdfDmaTag *)sourceAddress;
    directRow = 0;
    tagAddress = 0;
    qwc = 0;
    phase = SDF_GR_FLUSH_OT_CHAIN_PHASE_READ_TAG;
    wordsLeft = 0;
    done = 0;
    for (;;) {
        if (wordsLeft == 0) {
            switch (phase) {
            case SDF_GR_FLUSH_OT_CHAIN_PHASE_READ_TAG: {
                u32 tag;
                if ((u32)cursor & 0xF) {
                    sdfPrintf(D_003B48A0, cursor);
                    return;
                }
                tag = ((u32 *)cursor)[0];
                tagAddress = cursor->address;
                tagId = (tag >> 28) & 7;
                qwc = tag & 0xFFFF;
                sdfPrintf("\n%08X [DMA %s] : %08X %08X\n",
                                           cursor, D_00398968[tagId], tag, tagAddress);
                wordCursor = ((u32 *)cursor) + 2;
                cursor++;
                wordsLeft = 2;
                phase = SDF_GR_FLUSH_OT_CHAIN_PHASE_SETUP_TAG_DATA;
                break;
            }
            case SDF_GR_FLUSH_OT_CHAIN_PHASE_SETUP_TAG_DATA:
                wordsLeft = qwc * 4;
                switch (tagId) {
                case 0:
                case 3:
                case 4:
                    wordCursor = (u32 *)tagAddress;
                    break;
                default:
                    wordCursor = (u32 *)cursor;
                    break;
                }
                phase = SDF_GR_FLUSH_OT_CHAIN_PHASE_DISPATCH_TAG;
                break;
            case SDF_GR_FLUSH_OT_CHAIN_PHASE_DISPATCH_TAG:
                switch (tagId) {
                case 0:
                case 7:
                    done = 1;
                    break;
                case 1:
                    cursor += qwc;
                    break;
                case 2:
                    cursor = (SdfDmaTag *)tagAddress;
                    break;
                case 3:
                case 4:
                    break;
                case 5:
                    if (depth == 2) {
                        sdfPrintf("\ncall stack nest overflow!\n");
                        return;
                    }
                    returnStack[depth++] = cursor + qwc;
                    cursor = (SdfDmaTag *)tagAddress;
                    break;
                case 6:
                    if (depth == 0) {
                        done = 1;
                    } else {
                        depth--;
                        cursor = returnStack[depth];
                    }
                    break;
                }
                phase = SDF_GR_FLUSH_OT_CHAIN_PHASE_READ_TAG;
                break;
            }
        } else {
            value = *wordCursor++;
            wordsLeft--;
            switch (dataMode) {
            case 0: {
                u32 opcode = (value >> 24) & 0x7F;
                u32 num = (value >> 16) & 0xFF;
                u32 immediate = value & 0xFFFF;
                s32 interrupt = (value >> 31) ? 'i' : ' ';
                s32 bits;
                s32 format;
                s32 unpackFormat;

                sdfPrintf("%08X %c%08X : ", (u32)(wordCursor - 1),
                                           interrupt, value);
                switch (opcode) {
                case 0x00:
                    sdfPrintf("VIF NOP\n");
                    break;
                case 0x01:
                    sdfPrintf("VIF STCYCL (CL=%d, WL=%d)\n", (s32)(immediate & 0xFF),
                                    (s32)(immediate >> 8));
                    break;
                case 0x02:
                    sdfPrintf("VIF OFFSET (%04X)\n", immediate & 0x3FF);
                    break;
                case 0x03:
                    sdfPrintf("VIF BASE   (%04X)\n", immediate & 0x3FF);
                    break;
                case 0x04:
                    sdfPrintf("VIF ITOP   (%04X)\n", immediate & 0x3FF);
                    break;
                case 0x05:
                    sdfPrintf("VIF STMOD  (%d)\n", (s32)(immediate & 3));
                    break;
                case 0x06:
                    sdfPrintf("VIF MSKPATH3 (%s)\n",
                                    (immediate & 0x8000) ? D_003BD528 : D_003BD530);
                    break;
                case 0x07:
                    sdfPrintf("VIF MARK   (%04X)\n", immediate);
                    break;
                case 0x10:
                    sdfPrintf("VIF FLUSHE\n");
                    break;
                case 0x11:
                    sdfPrintf("VIF FLUSH\n");
                    break;
                case 0x13:
                    sdfPrintf("VIF FLUSHA\n");
                    break;
                case 0x14:
                    sdfPrintf("VIF MSCAL  (%04X)\n", immediate);
                    break;
                case 0x15:
                    sdfPrintf("VIF MSCALF (%04X)\n", immediate);
                    break;
                case 0x17:
                    sdfPrintf("VIF MSCNT\n");
                    break;
                case 0x20:
                    dataMode = 1;
                    sdfPrintf("VIF STMASK\n");
                    break;
                case 0x30:
                    dataMode = 2;
                    n = 4;
                    sdfPrintf("VIF STROW\n");
                    break;
                case 0x31:
                    dataMode = 3;
                    n = 4;
                    sdfPrintf("VIF STCOL\n");
                    break;
                case 0x4A:
                    n = num * 2;
                    sdfPrintf("VIF MPG   (%04X)\n", immediate);
                    if (n == 0) {
                        n = 0x200;
                    }
                    dataMode = 4;
                    break;
                case 0x50:
                    n = immediate * 4;
                    if (n == 0) {
                        n = 0x40000;
                    }
                    sdfPrintf("VIF DIRECT (%06X bytes)\n", (u32)(n * 4));
                    dataMode = 5;
                    directRow = 0;
                    break;
                case 0x51:
                    n = immediate * 4;
                    if (n == 0) {
                        n = 0x40000;
                    }
                    sdfPrintf("VIF DIRECTHL (%06X bytes)\n", (u32)(n * 4));
                    dataMode = 6;
                    directRow = 0;
                    break;
                default:
                    if ((opcode & 0x60) == 0x60) {
                        unpackFormat = opcode & 0xF;
                        if (D_00398988[unpackFormat] == NULL) {
                            sdfPrintf("illegal unpack type !\n");
                            return;
                        }
                        format = (((s32)immediate >> 13) & 6) | (((s32)opcode >> 4) & 1);
                        dataMode = 7;
                        sdfPrintf("VIF UNPACK%s %s,%04X,%04X\n",
                                                   D_003989C8[format],
                                                   D_00398988[unpackFormat],
                                                   immediate & 0x3FF, num);
                        bits = D_003989E8[unpackFormat];
                        n = (bits * (s32)num + 0x1F) >> 5;
                    } else {
                        sdfPrintf("unkown vif command\n");
                        return;
                    }
                    break;
                }
                break;
            }
            case 1:
                sdfPrintf("    STMASK : %08X\n", value);
                dataMode = 0;
                break;
            case 2:
                n--;
                sdfPrintf("    STROW : %08X\n", value);
                if (n == 0) {
                    dataMode = 0;
                }
                break;
            case 3:
                n--;
                sdfPrintf("    STCOL : %08X\n", value);
                if (n == 0) {
                    dataMode = 0;
                }
                break;
            case 4:
                if (--n == 0) {
                    dataMode = 0;
                }
                break;
            case 5:
                if (directRow == 0) {
                    sdfPrintf(D_003BD5E0);
                } else {
                    sdfPrintf(D_003BD508);
                }
                directRow++;
                sdfPrintf(D_003BD5E8, value);
                if (directRow == 4) {
                    directRow = 0;
                    sdfPrintf(D_003BD518);
                }
                if (--n == 0) {
                    if (directRow != 0) {
                        sdfPrintf(D_003BD518);
                    }
                    dataMode = 0;
                }
                break;
            case 6:
                if (--n == 0) {
                    dataMode = 0;
                }
                break;
            case 7:
                if (--n == 0) {
                    dataMode = 0;
                }
                break;
            }
        }
        if (done != 0) {
            sdfPrintf("<<< DMA-VIF normal end [%p] >>>\n", (void *)wordCursor);
            return;
        }
    }
}

extern char sdfGsMemoryDumpHeader[]; /* " <<< GS memory information >>>..." */

extern char sdfGsMemoryDumpRowFormat[]; /* " %08X : %08X %08X %8s %08X %d\n" */

extern char sdfGsMemoryTypeFormat[]; /* "%d" */

extern char *D_00398A28[];

extern char *D_00398A38[];

void sdfDumpGsMemory(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;

    sdfPrintf(sdfGsMemoryDumpHeader);
    head = sdfGsMemGetLinkHead();
    node = head;
    while (node != 0) {
        if (node->allocationMode < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = D_00398A28[node->allocationMode];
        } else {
            sdfPrintf(buf, sdfGsMemoryTypeFormat, node->allocationMode);
            name = buf;
        }
        sdfPrintf(sdfGsMemoryDumpRowFormat, node, node->next, node->prev, name, node->word, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->prev;
        }
        node = node->prev;
    }
}

void sdfDumpGsMemory2(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;

    sdfPrintf(sdfGsMemoryDumpHeader);
    head = sdfGsMemGetLinkTail();
    node = head;
    while (node != 0) {
        if (node->allocationMode < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = D_00398A38[node->allocationMode];
        } else {
            sdfPrintf(buf, sdfGsMemoryTypeFormat, node->allocationMode);
            name = buf;
        }
        sdfPrintf(sdfGsMemoryDumpRowFormat, node, node->next, node->prev, name, node->word, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->next;
        }
        node = node->next;
    }
}

INCLUDE_RODATA(const s32, "sdf/sdfSound", sdfGsMemoryDumpHeader);

INCLUDE_RODATA(const s32, "sdf/sdfSound", sdfGsMemoryDumpRowFormat);

void sdfDumpMemory(void) {
    SdfGeneralHeapStats info;
    sdfMemGetInfo(&info);
    sdfPrintf(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info.totalBytes, info.freeBytes,
                    info.largestFreeBytes, info.smallestFreeBytes,
                    info.blockCount, info.freeBlockCount);
}

/* Print the chip heap totals and how many cells are in use per size class (1..16, 17..32, ...). */
void sdfDumpChipMemory(void) {
    SdfChipHeapStats stats;
    u32 limit = 16;
    s32 i = 0;
    u32 first;
    u32 *used;

    sdfChipGetInfo(&stats);
    sdfPrintf(" <<< chip memory information >>>\n                 total : 0x%06X\n            free total : 0x%06X\n            page count : %d\n       free page count : %d\n fragmented page count : %d\n", stats.totalBytes, stats.freeBytes, stats.blockCount, stats.emptyBlockCount, stats.partialBlockCount);
    sdfPrintf("\n several size alloc count...\n");
    first = 0;
    used = stats.usedCells;
    do {
        sdfPrintf("       for %3d .. %4d : %d\n", first, limit, *used++);
        i++;
        first = limit + 1;
        limit *= 2;
    } while (i != 7);
    sdfPrintf(D_003BD518, first);
}
INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdSmgHandle);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD494);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdPcmState);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdPcmBusy);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4A0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4A4);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdSmgBuf);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4B0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4B8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4C0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4C8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4D0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4D8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4E0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4E8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4F0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD4F8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD500);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD508);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD510);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD518);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD520);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD528);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD530);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD538);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD540);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD548);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD550);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD558);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD560);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD568);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD570);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD578);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD580);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD588);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD590);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD598);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5A0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5A8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5B0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5B8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5C0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5C8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5D0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5D8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5E0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5E8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5F0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD5F8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD600);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_003BD608);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfGsMemoryTypeFormat);

