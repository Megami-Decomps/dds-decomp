#include "snd_ring.h"
#include "common.h"

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

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size);

u32 func_002E87A8(u32 command, u32 channel, void *packet, s32 size);

void sndEnsureMidiBankResident(s32 trackId);

/* Sound command work block: header, 16 channels/buffers, 13 track slots (see code_002E9140). */
typedef struct SndChannel {
    u8 index;     /* 0x0 */
    u8 unk1;      /* 0x1 */
    s16 unk2;     /* 0x2 */
    u32 unk4;     /* 0x4 */
    u32 unk8;     /* 0x8 */
    u32 unkC;     /* 0xC */
} SndChannel;

typedef struct SndTrackSlot {
    s32 id;       /* 0x0 */
    u8 flagA;     /* 0x4 */
    u8 flagB;     /* 0x5 */
    u8 pad6[2];
} SndTrackSlot;

typedef struct SndIopBuffer {
    s32 size;
    u32 address;
} SndIopBuffer;

typedef struct SndWork {
    s32 header;                /* 0x000 */
    s32 bufferCount;           /* 0x004 */
    u32 unk008;              /* 0x008 */
    u32 unk00C;              /* 0x00C */
    SndChannel channels[SND_CHANNEL_COUNT];   /* 0x010 */
    SndIopBuffer buffers[SND_IOP_BUFFER_COUNT];  /* 0x110 */
    SndTrackSlot slots[SND_TRACK_SLOT_COUNT];    /* 0x190 */
    u32 unk1F8;              /* 0x1F8 */
    u32 unk1FC;              /* 0x1FC */
    u32 unk200;                /* 0x200 */
    u32 unk204;                /* 0x204 */
} SndWork;

extern SndWork sndMidiTrackState;
extern u32 sndReserveIopWorkMemory(s32 size);
extern s8 D_003BDA80;
extern s32 D_003BDA8C;
extern s32 sceSifInitIopHeap(void);
extern s32 func_002E8700(void);
extern void func_002E8938(s32, void *, s32);

extern SndRingPacket D_003FB080[SND_COMMAND_RING_ENTRY_COUNT];
extern u32 D_003BD4A0;
/* The worker consumes entries while this producer publishes and polls cursors. */
extern vu16 D_003BDA82;
extern vu16 D_003BDA84;
extern s32 D_003BDA88;
extern s32 WakeupThread(s32 thread);
extern s32 func_002E8640(void);
extern void *memcpy(void *, const void *, u32);

u32 func_002E87A8(u32 command, u32 channel, void *packet, s32 payloadBytes) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    SndRingPacket *entry;
    u32 transferQuadwords;

    if (++D_003BD4A0 == 0) {
        D_003BD4A0 = 1;
    }
    nextWriteIndex = ((s16)D_003BDA84 + 1) & SND_COMMAND_RING_INDEX_MASK;
    if (nextWriteIndex == (s16)D_003BDA82) {
        for (wakeAttemptsRemaining = 8; wakeAttemptsRemaining != 0; wakeAttemptsRemaining--) {
            do {
                WakeupThread(D_003BDA88);
            } while ((s16)D_003BDA82 != nextWriteIndex);
        }
        return 0;
    }
    entry = &D_003FB080[(s16)D_003BDA84];
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
    D_003BDA84 = nextWriteIndex;
    if (D_003BDA80 != 0) {
        while (func_002E8640() != 0) {
        }
    } else {
        WakeupThread(D_003BDA88);
    }
    return D_003BD4A0;
}


void func_002E88F8(u32 unused) {
}

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size) {
    u32 result = func_002E87A8(command, channel, packet, size);

    func_002E88F8(result);
    return result;
}

INCLUDE_ASM(const s32, "game/code_002E87A8", func_002E8938);

typedef struct SceIoStat {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} SceIoStat;

extern u8 sdfPfsDebugMode;
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern s32 func_00310478(const char *, SceIoStat *);
extern s32 sdfSendNamedResourceRequest(char *, s32, void *, s32 *);

void func_002E89D0(void) {
    SceIoStat info;
    char *module;

    sceSifLoadModule("cdrom0:\\SOUNDIRX\\LIBSD.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\SDRDRV.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\MODHSYN.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\MODMIDI.IRX;1", 0, NULL);
    sceSifLoadModule("cdrom0:\\SOUNDIRX\\MODMSIN.IRX;1", 0, NULL);
    module = "cdrom0:\\USERIRX\\SDFSDMAN.IRX;1";
    if (sdfPfsDebugMode != 0 &&
        func_00310478("pfs:/userirx/SDFSDMAN.IRX", &info) == 0 &&
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
    sndMidiTrackState.channels[0].unk8 = sndReserveIopWorkMemory(0x21600);
    sndMidiTrackState.unk200 = 0;
    sndMidiTrackState.unk204 = 0;
    for (i = 0; i < SND_TRACK_SLOT_COUNT; i++) {
        sndMidiTrackState.slots[i].id = -1;
        sndMidiTrackState.slots[i].flagA = 0;
        sndMidiTrackState.slots[i].flagB = 0;
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

void func_002E8C30(s32 unused, s32 header, s32 *bufferSizes, s32 bufferCount) {
    D_003BDA80 = 1;
    func_002E89D0();
    sceSifInitIopHeap();
    func_002E8700();
    sndMidiTrackState.unk008 = sndReserveIopWorkMemory(0x4000);
    sndMidiTrackState.unk00C = 0x4000;
    sndInitializeChannelAndTrackState(unused, header);
    sndAllocateAlignedIopBuffers(bufferSizes, bufferCount);
    D_003BDA8C = sndReserveIopWorkMemory(0x8D0);
    sndMidiTrackState.unk1F8 = (u32)&sndMidiTrackState;
    sndMidiTrackState.unk1FC = 0x8D0;
    func_002E8938(D_003BDA8C, &sndMidiTrackState, 0x8D0);
    sndSendCommandPacket(0, 0, &D_003BDA8C, 4);
    D_003BDA80 = 0;
}

/* Starts a track with the default 0x7f setting after preparing its identifier. */
void sndStartTrackDefault(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    func_002E87A8(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */
void sndSendSingleWordControlPacket(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_002E87A8(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */
void sndStartTrackAlternate(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    func_002E87A8(0x130, 0, &packet, 0x10);
}

void func_002E8DD0(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_002E87A8(0x30, 0, packet, 0x10);
}
