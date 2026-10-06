#include "snd_ring.h"
#include "common.h"

typedef struct CmdPacket {
    /* 0x0 */ u32 trackId;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u16 setting;
    /* 0xA */ u16 unkA;
    /* 0xC */ u32 unkC;
} CmdPacket;

u32 func_00341650(u32 command, u32 channel, void *packet, s32 size);

void sndEnsureMidiBankResident(s32 trackId);

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size);

/* Sound command work block: header, 16 channels/buffers, 13 track slots. */
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
    SndChannel channels[16];   /* 0x010 */
    SndIopBuffer buffers[16];  /* 0x110 */
    SndTrackSlot slots[13];    /* 0x190 */
    u32 unk1F8;              /* 0x1F8 */
    u32 unk1FC;              /* 0x1FC */
    u32 unk200;                /* 0x200 */
    u32 unk204;                /* 0x204 */
} SndWork;

extern SndWork sndMidiTrackState;
extern u32 sndReserveIopWorkMemory(s32 size);
extern s8 D_004391E0;
extern s32 D_004391EC;
extern s32 sceSifInitIopHeap(void);
extern s32 func_003415A8(void);
extern void func_003417E0(s32, void *, s32);

extern SndRingPacket D_00477A00[32];
extern u32 D_00438B90;
/* The worker consumes entries while this producer publishes and polls cursors. */
extern vu16 D_004391E2;
extern vu16 D_004391E4;
extern s32 D_004391E8;
extern s32 WakeupThread(s32 thread);
extern s32 func_003414E8(void);
extern void *memcpy(void *, const void *, u32);

u32 func_00341650(u32 command, u32 channel, void *packet, s32 size) {
    s32 next;
    s32 retries;
    SndRingPacket *entry;
    u32 packetQuadwords;

    if (++D_00438B90 == 0) {
        D_00438B90 = 1;
    }
    next = ((s16)D_004391E4 + 1) & 31;
    if (next == (s16)D_004391E2) {
        for (retries = 8; retries != 0; retries--) {
            do {
                WakeupThread(D_004391E8);
            } while ((s16)D_004391E2 != next);
        }
        return 0;
    }
    entry = &D_00477A00[(s16)D_004391E4];
    entry->sequence = D_00438B90;
    if (size != 0) {
        memcpy(entry->payload, packet, size);
    }
    packetQuadwords = (size + 0x14 + 15) >> 4;
    entry->command = (command << 16) | (channel & 0xffff) | (packetQuadwords << 28);
    D_004391E4 = next;
    if (D_004391E0 != 0) {
        while (func_003414E8() != 0) {
        }
    } else {
        WakeupThread(D_004391E8);
    }
    return D_00438B90;
}

void func_003417A0(u32 unused) {
}

u32 sndSendCommandPacket(u32 command, u32 channel, void *packet, u32 size) {
    u32 result = func_00341650(command, channel, packet, size);

    func_003417A0(result);
    return result;
}

INCLUDE_ASM(const s32, "game/code_00341650", func_003417E0);

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
extern s32 func_0036B6E0(const char *, SceIoStat *);
extern s32 sdfSendNamedResourceRequest(char *, s32, void *, s32 *);

void func_00341878(void) {
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
    for (i = 0; i < 16; i++) {
        channel->index = i;
        channel->unk1 = 0x20;
        channel->unk2 = 0;
        channel->unk8 = 0;
        channel->unk4 = 0;
        channel->unkC = 0;
        channel++;
    }
    sndMidiTrackState.channels[0].unk8 = sndReserveIopWorkMemory(0x3B600);
    sndMidiTrackState.unk200 = 0;
    sndMidiTrackState.unk204 = 0;
    for (i = 0; i < 13; i++) {
        sndMidiTrackState.slots[i].id = -1;
        sndMidiTrackState.slots[i].flagA = 0;
        sndMidiTrackState.slots[i].flagB = 0;
    }
}

/* Allocate one aligned IOP block and distribute its address among active buffers. */
void sndAllocateAlignedIopBuffers(s32 *sizes, s32 count) {
    SndIopBuffer *buffer;
    s32 total, size, i;
    u32 address;

    sndMidiTrackState.bufferCount = count;
    buffer = sndMidiTrackState.buffers;
    total = 0;
    i = 0;
    do {
        size = (*sizes++ + 15) & ~15;
        buffer->size = size;
        buffer++;
        total += size;
        i++;
    } while (i != count);
    for (; i < 16; i++) {
        sndMidiTrackState.buffers[i].size = 0;
        sndMidiTrackState.buffers[i].address = 0;
    }
    address = sndReserveIopWorkMemory(total);
    i = 0;
    do {
        sndMidiTrackState.buffers[i].address = address;
        address += sndMidiTrackState.buffers[i].size;
        i++;
    } while (i != count);
}

void func_00341AD8(s32 arg0, s32 arg1, s32 *sizes, s32 count) {
    D_004391E0 = 1;
    func_00341878();
    sceSifInitIopHeap();
    func_003415A8();
    sndMidiTrackState.unk008 = sndReserveIopWorkMemory(0x4000);
    sndMidiTrackState.unk00C = 0x4000;
    sndInitializeChannelAndTrackState(arg0, arg1);
    sndAllocateAlignedIopBuffers(sizes, count);
    D_004391EC = sndReserveIopWorkMemory(0x8D0);
    sndMidiTrackState.unk1F8 = (u32)&sndMidiTrackState;
    sndMidiTrackState.unk1FC = 0x8D0;
    func_003417E0(D_004391EC, &sndMidiTrackState, 0x8D0);
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
    func_00341650(0x20, 0, &packet, 0x10);
}

/* Sends a single-word command payload in a 16-byte packet. */

void sndSendSingleWordControlPacket(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */

void sndStartTrackAlternate(s32 trackId) {
    CmdPacket packet;

    sndEnsureMidiBankResident(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    func_00341650(0x130, 0, &packet, 0x10);
}

void func_00341C78(u32 value) {
    u32 packet[4];

    packet[0] = value;
    func_00341650(0x30, 0, packet, 0x10);
}
