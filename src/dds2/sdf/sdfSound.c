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
#include "sdf_handler_node.h"
#include "eeregs.h"
#include "mnu_voice_resource_path_api.h"
#include "sdf_heap_api.h"
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
#include "ee_mmi.h"
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
#include "mnu_title_sound.h"
#include "sdf_movie_draw_packet.h"

extern s8 sdfSndBurstReqMode;

u32 sdfSndIopAlloc(s32 size) {
    return (u32)sceSifAllocIopHeap(size);
}

extern SndRingPacket sndCommandRing[32];

/* Shared with the producer and its worker-thread wakeups. */
extern vu16 sdfSndCmdIndex;

extern vu16 sdfSndCmdTailIndex;

SndQueuedCommandDispatchResult sdfSndCmdTrans(void) {
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
        while (sdfSndCmdTrans() == SND_QUEUED_COMMAND_DISPATCH_RESULT_SENT) {
        }
    }
}

extern SdfKernelThreadParam sndCommandThreadParameters;

extern u8 sndCommandThreadStack[];

extern u8 sndRpcServerThreadStack[];

extern void *_gp;

extern s32 sdfSndCmdBuf;

extern s32 D_00438B94;

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);

extern void SifRpcServerThreadProc(void);

s32 sdfSndInitCmdIF(void) {
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
    D_00438B94 = threadId;
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

/* Sound command work block: header, 16 channels/buffers, 13 track slots. */
extern SndWork sdfSndMap;

extern u32 sdfSndIopAlloc(s32 size);

extern s32 D_004391EC;

extern s32 sdfSndInitCmdIF(void);

extern void sdfSndTransToIOP(s32, const void *, s32);

extern SndRingPacket sndCommandRing[SND_COMMAND_RING_ENTRY_COUNT];

extern u32 sdfSndSendCmd_cmd_id;

/* The worker consumes entries while this producer publishes and polls cursors. */
extern vu16 sdfSndCmdIndex;

u32 sdfSndSendCmd(u32 command, u32 channel, const void *packet, s32 payloadBytes) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    SndRingPacket *entry;
    u32 transferQuadwords;

    if (++sdfSndSendCmd_cmd_id == 0) {
        sdfSndSendCmd_cmd_id = 1;
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
    entry->sequence = sdfSndSendCmd_cmd_id;
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
        while (sdfSndCmdTrans() != SND_QUEUED_COMMAND_DISPATCH_RESULT_SENT) {
        }
    } else {
        WakeupThread(sdfSndCmdBuf);
    }
    return sdfSndSendCmd_cmd_id;
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
void sdfSndLoadIRXModules(void) {
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

void sdfSndVoiceMapInit(char *unused, s32 header) {
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
    sdfSndMap.channels[0].iopWorkAddress = sdfSndIopAlloc(0x3B600);
    sdfSndMap.opaque200 = 0;
    sdfSndMap.opaque204 = 0;
    for (i = 0; i < SND_TRACK_SLOT_COUNT; i++) {
        sdfSndMap.slots[i].id = -1;
        sdfSndMap.slots[i].balanceFirstComponent = 0;
        sdfSndMap.slots[i].balanceSecondComponent = 0;
    }
}

/* Allocate one aligned IOP block and distribute its address among active buffers. */
void sdfSndSeqMapInit(const s32 *bufferSizes, s32 bufferCount) {
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
    iopAddress = sdfSndIopAlloc(totalBytes);
    bufferIndex = 0;
    do {
        sdfSndMap.buffers[bufferIndex].iopAddress = iopAddress;
        iopAddress += sdfSndMap.buffers[bufferIndex].size;
        bufferIndex++;
    } while (bufferIndex != bufferCount);
}

void sdfSndInit(char *unused, s32 header, const s32 *bufferSizes, s32 bufferCount) {
    sdfSndBurstReqMode = 1;
    sdfSndLoadIRXModules();
    sceSifInitIopHeap();
    sdfSndInitCmdIF();
    sdfSndMap.iopBufferAddress = sdfSndIopAlloc(0x4000);
    sdfSndMap.iopBufferBytes = 0x4000;
    sdfSndVoiceMapInit(unused, header);
    sdfSndSeqMapInit(bufferSizes, bufferCount);
    D_004391EC = sdfSndIopAlloc(0x8D0);
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
void p2sdPlaySeDeg(s32 trackId, f32 angle, f32 distance) {
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

extern SndTrackSlot LastLoad[];

extern SndPackedTrackLoadStatus p2sdLoadCheckMidi(s32 packedTrack);

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
SndPackedTrackLoadStatus p2sdLoadCheckMidi(s32 packed) {
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

extern FileManagerQueuedRequestResult (*p2sdSmgBuf)(void);

/* Make sure the MIDI bank named by the packed track id is resident, loading it if not. */
void p2sdLoadSMGFile(s32 packed) {
    char name[0x10];
    char path[0x100];
    SndPackedTrackLoadStatus status = p2sdLoadCheckMidi(packed);
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
        } while (p2sdLoadCheckMidi(packed) != SND_PACKED_TRACK_LOAD_RESIDENT);
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
    if (p2sdLoadCheckMidi(id) != SND_PACKED_TRACK_LOAD_NOT_PRESENT) {
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

extern u32 p2sdPcmState;

extern u32 p2sdPcmBusy;

extern SdfSoundCommand p2sdPcmCallback;

extern s32 sdfPrintf(const char *fmt, ...);

extern char D_0042E590[];

extern char D_0042E5B0[];

extern char D_0042E5C8[];

extern char D_0042E5F0[];

extern char D_0042E610[];

extern char *sdfDmaTagNameTable[];

extern char D_0040BAF8[];

extern s32 sifRpcServerSema;

extern u32 sdfFileRpcQueue[16];

extern char sdfGsMemoryDumpHeader[]; /* " <<< GS memory information >>>..." */

extern char sdfGsMemoryDumpRowFormat[]; /* " %08X : %08X %08X %8s %08X %d\n" */

extern char sdfGsMemoryTypeFormat[]; /* "%d" */

extern char *D_0040BBD8[];

extern char *D_0040BBE8[];

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
    strcpy(D_0040BAF8, name);
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

void sdfSoundSetChannelCount(u32 channels) {
    if (0x10 < channels) {
        channels = 0x10;
    }
    if (channels == 0) {
        channels = 1;
    }
    sdfSndSendSyncCmd((channels - 1) | 0x1d0, 0, 0, 0);
}

SdfSoundReadCallback p2sdSetPcmCallback(SdfSoundReadCallback callback) {
    if (p2sdPcmBusy != 0) {
        return NULL;
    }
    p2sdPcmCallback.read = callback;
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

/* Event 5 invalidates the EE cache before waking the waiting sound thread. */
s32 sdfFileRpcCallback(s32 unused, SdfDevEvent event) {
    switch (event) {
    case SDF_DEV_EVENT_READ_REPLY:
        FlushCache(0);
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

extern SdfSoundRpcRequest *sdfFileRpcResult;

extern s32 sdfFileRpcCmdState;

extern sdfFileCmdHandle_t *sdfFileRpcArg;

extern SdfSoundResidentBuffer p2sdPcmBuffer;

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void *callback, void *callbackContext);

/* Handle sound-file/buffer RPC commands and return the shared reply words. */
u32 *sdfFileRpcHandleCmd(u32 command, SdfSoundRpcRequest *request) {
    char path[0x100];

    switch (command) {
    case SDF_SOUND_RPC_OPEN_VOICE_FILE: {
        sdfFileCmdHandle_t *state;
        if (D_0040BAF8[0] == 0) {
            sdfFileRpcQueue[0] = 0;
            break;
        }
        p2sdPcmState = MNU_TITLE_SOUND_BUSY;
        mnuBuildVoiceResourcePath(path, D_0040BAF8);
        state = sdfFileCmdReadOpen(path, sdfFileRpcCallback, 0);
        sdfFileRpcArg = state;
        if (state != NULL) {
            WaitSema(sifRpcServerSema);
            state = sdfFileRpcArg;
            if (state->result != 0) {
                sdfFileCmdClearError(state);
                sdfFileCmdFree(sdfFileRpcArg);
                sdfFileRpcArg = NULL;
                state = NULL;
            }
        }
        sdfFileRpcQueue[0] = (u32)state;
        break;
    }
    case SDF_SOUND_RPC_READ_VOICE_FILE: {
        sdfMemHandle_t *allocation;
        void *buffer;
        sdfFileRpcResult = request;
        allocation = sdfMemAllocDirect(request->byteCount);
        buffer = (void *)sdfMemLock(allocation);
        if (buffer != NULL) {
            sdfFileCmdRead(sdfFileRpcArg, buffer, sdfFileRpcResult->byteCount);
            WaitSema(sifRpcServerSema);
            sdfSndTransToIOP(sdfFileRpcResult->destination, buffer, sdfFileRpcResult->byteCount);
            sdfMemFreeDirect(allocation);
            sdfFileRpcQueue[0] = sdfFileRpcResult->byteCount;
        } else {
            sdfFileRpcQueue[0] = 0;
        }
        break;
    }
    case SDF_SOUND_RPC_CLOSE_VOICE_FILE:
        sdfFileCmdClose(sdfFileRpcArg);
        WaitSema(sifRpcServerSema);
        sdfFileCmdFree(sdfFileRpcArg);
        sdfFileRpcArg = NULL;
        p2sdPcmState = MNU_TITLE_SOUND_IDLE;
        sdfFileRpcQueue[0] = 0;
        break;
    case SDF_SOUND_RPC_RESET_RESIDENT_BUFFER_OFFSET:
        sdfFileRpcCmdState = 0;
        sdfFileRpcQueue[0] = 0;
        break;
    case SDF_SOUND_RPC_READ_RESIDENT_BUFFER: {
        s32 bytes;
        sdfFileRpcResult = request;
        sdfSndTransToIOP(request->destination, p2sdPcmBuffer.data + sdfFileRpcCmdState, request->byteCount);
        bytes = sdfFileRpcResult->byteCount;
        sdfFileRpcQueue[0] = bytes;
        sdfFileRpcCmdState += bytes;
        break;
    }
    case 5:
        sdfFileRpcQueue[0] = 0;
        break;
    case SDF_SOUND_RPC_GET_PCM_CALLBACK_WORD: {
        u32 word = p2sdPcmCallback.word;
        if (word != 0) {
            p2sdPcmBusy = 1;
        }
        sdfFileRpcQueue[0] = word;
        break;
    }
    case SDF_SOUND_RPC_READ_PCM_CALLBACK_BUFFER: {
        const u8 *buffer;
        sdfFileRpcResult = request;
        buffer = p2sdPcmCallback.read(request->byteCount);
        if (buffer != NULL) {
            sdfSndTransToIOP(sdfFileRpcResult->destination, buffer, sdfFileRpcResult->byteCount);
            sdfFileRpcQueue[0] = sdfFileRpcResult->byteCount;
        } else {
            sdfFileRpcQueue[0] = -1;
        }
        break;
    }
    case SDF_SOUND_RPC_CLEAR_PCM_CALLBACK_BUSY:
        p2sdPcmBusy = 0;
        sdfFileRpcQueue[0] = 0;
        break;
    }
    return sdfFileRpcQueue;
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
    sceSifRegisterRpc(server, 0x54524E53, sdfFileRpcHandleCmd, sdfFileRpcQueue, 0, 0, queue);
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
    sdfSndTransToIOP(D_004391EC, &sdfSndMap, 0x8D0);
    sdfSndSendSyncCmd(0x1F0, 0, 0, 0);
}

/* Refresh the sound table, then copy the selected slice into dst. */
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
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 6:
        table = (u8 *)&sdfSndMap;
        src = table + 0x510;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 7:
        table = (u8 *)&sdfSndMap;
        src = table + 0x590;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 8:
        table = (u8 *)&sdfSndMap;
        src = table + 0x610;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 9:
        table = (u8 *)&sdfSndMap;
        src = table + 0x690;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 10:
        table = (u8 *)&sdfSndMap;
        src = table + 0x710;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 11:
        table = (u8 *)&sdfSndMap;
        src = table + 0x790;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    case 12:
        table = (u8 *)&sdfSndMap;
        src = table + 0x810;
        for (i = 0; i < 2; i++) {
            dst[i] = *src++;
        }
        return;
    }
}

extern char D_00438BE0[];

extern char D_00438BE8[];

extern char D_00438BF0[];

extern char D_00438BF8[];

extern char D_00438C00[];

extern char sdfDumpLineBuf[];

extern char D_00438C10[];

void sdfDumpHexLine(const u8 *data, s32 size) {
    s32 column = 0;
    s32 i;

    while (size > 0) {
        if (column == 0) {
            sdfPrintf(D_00438BE0, data);
            column = (u32)data & 0xF;
            for (i = column; i > 0; i--) {
                sdfPrintf(D_00438BE8);
            }
        } else if (column == 8) {
            sdfPrintf(D_00438BF0);
        } else {
            sdfPrintf(D_00438BF8);
        }
        sdfPrintf(D_00438C00, *data++);
        column++;
        if (column == 0x10) {
            column = 0;
            sdfPrintf(sdfDumpLineBuf);
        }
        size--;
    }
    if (column == 0) {
        sdfPrintf(sdfDumpLineBuf);
    } else {
        sdfPrintf(D_00438C10);
    }
}

/* Print a DMA chain, retaining the native two-entry CALL/RET stack limits. */
INCLUDE_RODATA(const s32, "sdf/sdfSound", D_0042E590);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_0042E5B0);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_0042E5C8);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_0042E5F0);

INCLUDE_RODATA(const s32, "sdf/sdfSound", D_0042E610);

void sdfDumpDmaChain(SdfDmaTag *dmaTag) {
    SdfDmaTag *returnStack[2];
    SdfDmaTag *cursor;
    u32 tag;
    u32 tagAddress;
    u32 tagDataCount;
    s32 tagType;
    s32 depth = 0;
    s32 done = 0;

    sdfPrintf(D_0042E590, dmaTag);
    cursor = dmaTag;
    for (;;) {
        if (((u32)cursor & 0xF) != 0) {
            sdfPrintf(D_0042E5B0, cursor);
            return;
        }

        tag = ((u32 *)cursor)[0];
        tagAddress = cursor->address;
        tagType = (tag >> 28) & 7;
        tagDataCount = tag & 0xFFFF;
        sdfPrintf(D_0042E5C8, cursor, sdfDmaTagNameTable[tagType], tag, tagAddress, ((u32 *)cursor)[2], ((u32 *)cursor)[3]);
        cursor++;

        switch (tagType) {
        case 0:
            done = 1;
            break;
        case 1:
            cursor += tagDataCount;
            break;
        case 2:
            cursor = (SdfDmaTag *)tagAddress;
            break;
        case 3:
        case 4:
            break;
        case 5:
            if (depth == 2) {
                sdfPrintf(D_0042E5F0);
                return;
            }
            returnStack[depth++] = cursor;
            cursor = (SdfDmaTag *)tagAddress;
            break;
        case 6:
            if (depth == 0) {
                done = 1;
            } else {
                cursor = returnStack[--depth];
            }
            break;
        case 7:
            cursor += tagDataCount;
            done = 1;
            break;
        }

        if (done != 0) {
            sdfPrintf(D_0042E610, cursor);
            return;
        }
    }
}

extern char D_00438C18[];

extern char D_00438C20[];

extern char D_00438CD0[];

extern char D_00438CD8[];

extern char *D_0040BB38[16];

extern char *D_0040BB78[8];

extern s32 D_0040BB98[16];

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
                    sdfPrintf(D_0042E5B0, cursor);
                    return;
                }
                tag = ((u32 *)cursor)[0];
                tagAddress = cursor->address;
                tagId = (tag >> 28) & 7;
                qwc = tag & 0xFFFF;
                sdfPrintf("\n%08X [DMA %s] : %08X %08X\n",
                                           cursor, sdfDmaTagNameTable[tagId], tag, tagAddress);
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
                                    (immediate & 0x8000) ? D_00438C18 : D_00438C20);
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
                        if (D_0040BB38[unpackFormat] == NULL) {
                            sdfPrintf("illegal unpack type !\n");
                            return;
                        }
                        format = (((s32)immediate >> 13) & 6) | (((s32)opcode >> 4) & 1);
                        dataMode = 7;
                        sdfPrintf("VIF UNPACK%s %s,%04X,%04X\n",
                                                   D_0040BB78[format],
                                                   D_0040BB38[unpackFormat],
                                                   immediate & 0x3FF, num);
                        bits = D_0040BB98[unpackFormat];
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
                    sdfPrintf(D_00438CD0);
                } else {
                    sdfPrintf(D_00438BF8);
                }
                directRow++;
                sdfPrintf(D_00438CD8, value);
                if (directRow == 4) {
                    directRow = 0;
                    sdfPrintf(sdfDumpLineBuf);
                }
                if (--n == 0) {
                    if (directRow != 0) {
                        sdfPrintf(sdfDumpLineBuf);
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
        if (node->allocationMode < 4) {
            name = D_0040BBD8[node->allocationMode];
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
        if (node->allocationMode < 4) {
            name = D_0040BBE8[node->allocationMode];
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
    sdfPrintf(sdfDumpLineBuf, first);
}
INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdSmgHandle);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdPcmCallback);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdPcmState);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdPcmBusy);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfSndSendCmd_cmd_id);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438B94);

INCLUDE_SDATA(const s32, "sdf/sdfSound", p2sdSmgBuf);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BA0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BA8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BB0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BB8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BC0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BC8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BD0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BD8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BE0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BE8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BF0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438BF8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C00);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDumpLineBuf);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C10);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C18);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C20);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C28);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C30);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C38);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C40);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C48);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C50);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C58);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C60);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C68);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C70);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C78);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C80);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C88);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C90);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438C98);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CA0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CA8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CB0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CB8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CC0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CC8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CD0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CD8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CE0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CE8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CF0);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_00438CF8);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfGsMemoryTypeFormat);

